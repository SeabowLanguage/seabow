const std = @import("std");

const opt = @import("../utils/options.zig");
const nd = @import("../nodes/node.zig");
const value = @import("../values/value.zig");
const vt = @import("../values/type.zig");
const diagnostic = @import("../utils/diagnostic.zig");
const Parser = @import("parser.zig").Parser;
const SourceText = @import("../utils/source_text.zig").SourceText;
const TokenKind = @import("../utils/token.zig").TokenKind;

pub const Interpreter = struct {
    elements: std.StringHashMap(value.Value),

    fn init() !Interpreter {
        var interp = Interpreter{ .elements = std.StringHashMap(value.Value).init(std.heap.page_allocator) };

        try interp.elements.put("long", value.Value.init(value.ValueElement{ .Type = vt.ValueType.init(value.ValueKind.Long, vt.TYPE_DETAIL_NONE) }, value.MODIFIER_CONSTANT));
        try interp.elements.put("ulong", value.Value.init(value.ValueElement{ .Type = vt.ValueType.init(value.ValueKind.Ulong, vt.TYPE_DETAIL_NONE) }, value.MODIFIER_CONSTANT));
        try interp.elements.put("double", value.Value.init(value.ValueElement{ .Type = vt.ValueType.init(value.ValueKind.Double, vt.TYPE_DETAIL_NONE) }, value.MODIFIER_CONSTANT));
        try interp.elements.put("bool", value.Value.init(value.ValueElement{ .Type = vt.ValueType.init(value.ValueKind.Bool, vt.TYPE_DETAIL_NONE) }, value.MODIFIER_CONSTANT));
        try interp.elements.put("string", value.Value.init(value.ValueElement{ .Type = vt.ValueType.init(value.ValueKind.String, vt.TYPE_DETAIL_NONE) }, value.MODIFIER_CONSTANT));
        try interp.elements.put("error", value.Value.init(value.ValueElement{ .Type = vt.ValueType.init(value.ValueKind.Error, vt.TYPE_DETAIL_NONE) }, value.MODIFIER_CONSTANT));
        try interp.elements.put("type", value.Value.init(value.ValueElement{ .Type = vt.ValueType.init(value.ValueKind.Type, vt.TYPE_DETAIL_NONE) }, value.MODIFIER_CONSTANT));

        return interp;
    }

    fn destroy(self: Interpreter) void {
        self.root.destroy();
        self.elements.deinit();
    }

    pub fn interpret_console(_: []opt.Option) !u8 {
        const stdin = std.io.getStdIn().reader();
        var diagnostics = std.ArrayList(diagnostic.Diagnostic).init(std.heap.page_allocator);
        defer diagnostics.deinit();
        var interp = try Interpreter.init();

        while (true) {
            std.debug.print(">>> ", .{});

            const code = try stdin.readUntilDelimiterAlloc(std.heap.page_allocator, '\n', std.math.maxInt(u32));
            if (code.len == 0 or code[0] == '\r') {
                std.heap.page_allocator.free(code);
                return 0;
            }

            defer std.heap.page_allocator.free(code);
            const source = SourceText.init(code);
            var parser = try Parser.init(source, &diagnostics);
            const root = try parser.parse();

            if (diagnostics.items.len == 0) {
                const val = try interp.interpret_node(root);
                const res = try val.convert(vt.TO_STRING);
                if (val.modifier & value.MODIFIER_DIAGNOSTIC != 0) {
                    diagnostic.Diagnostic.display_error(res.value.String.value.?, val.value.Error.position, source);
                } else if (val.value != value.ValueElement.Null) {
                    std.debug.print("\x1b[32m{?s}\x1b[0m\n", .{res.value.String.value});
                }
            } else {
                for (diagnostics.items) |diag| {
                    diag.display(source);
                }
            }

            diagnostics.clearAndFree();
        }

        return 0;
    }

    pub fn interpret_file(options: []opt.Option) !u8 {
        var input_file: ?[]const u8 = null;
        for (options) |option| {
            if (option.kind == opt.OptionKind.InputFile) {
                input_file = option.value;
            }
        }

        if (input_file == null) {
            diagnostic.Diagnostic.display_error("InterpreterError: Could not interpret a seabow source file without using option `-i`", null, null);
            return 0;
        }

        const source_file = std.fs.cwd().openFile(input_file.?, .{}) catch {
            diagnostic.Diagnostic.display_error("InterpreterError: Failed to open the given seabow source file of the `-i` option", null, null);
            return 0;
        };
        defer source_file.close();

        const file_size: usize = @intCast(try source_file.getEndPos()); // 32-bit only
        const code = try source_file.readToEndAlloc(std.heap.page_allocator, file_size);
        defer std.heap.page_allocator.free(code);

        var diagnostics = std.ArrayList(diagnostic.Diagnostic).init(std.heap.page_allocator);
        defer diagnostics.deinit();

        const source = SourceText.init(code);
        var parser = try Parser.init(source, &diagnostics);
        const root = try parser.parse();

        if (diagnostics.items.len == 0) {
            var interp = try Interpreter.init();
            const val = try interp.interpret_node(root);
            const res = try val.convert(vt.TO_STRING);
            if (val.modifier & value.MODIFIER_DIAGNOSTIC != 0) {
                diagnostic.Diagnostic.display_error(res.value.String.value.?, val.value.Error.position, source);
            } else if (val.value != value.ValueElement.Null) {
                std.debug.print("\x1b[32m{?s}\x1b[0m\n", .{res.value.String.value});
            }
        } else {
            for (diagnostics.items) |diag| {
                diag.display(source);
            }
        }

        return 0;
    }

    fn interpret_node(self: *Interpreter, node: *nd.Node) anyerror!value.Value {
        return switch (node.*) {
            .NoOp => value.Value.init(value.ValueElement{ .Null = value.ValueNull{} }, value.MODIFIER_NONE),
            .Compound => |comp| self.interpret_node_compound(comp),
            .Unary => |unary| self.interpret_node_unary(unary),
            .Binary => |binary| self.interpret_node_binary(binary),
            .Literal => |lit| lit.value,
            .VarConstAccess => |vc| self.interpret_node_var_const_access(vc),
            .VarConstDeclList => |vcdl| self.interpret_node_var_const_decl_list(vcdl),

            else => value.Value.init(value.ValueElement{
                .Error = value.ValueError.init("TODO", "Implement other interpretations", null),
            }, value.MODIFIER_DIAGNOSTIC),
        };
    }

    fn interpret_node_compound(self: *Interpreter, node: nd.NodeCompound) !value.Value {
        var ret_val: value.Value = undefined;
        for (node.nodes) |next_node| {
            ret_val = try self.interpret_node(next_node);
            if (ret_val.modifier & value.MODIFIER_DIAGNOSTIC != 0) {
                return ret_val;
            }
        }

        return ret_val;
    }

    fn interpret_node_unary(self: *Interpreter, node: nd.NodeUnary) !value.Value {
        const val = try self.interpret_node(node.operand);
        if (val.modifier & value.MODIFIER_DIAGNOSTIC != 0) {
            return val;
        }

        switch (node.operator.kind) {
            TokenKind.Minus => {
                return try val.negative(node.operator.position);
            },

            TokenKind.Not => {
                return try val.not(node.operator.position);
            },

            else => {
                const verr = value.ValueError.init("SeabowUnaryError", "Incorrect unary operator found", node.operator.position);
                return value.Value.init(value.ValueElement{ .Error = verr }, value.MODIFIER_DIAGNOSTIC);
            },
        }
    }

    fn interpret_node_binary(self: *Interpreter, node: nd.NodeBinary) !value.Value {
        const lval = try self.interpret_node(node.left);
        if (lval.modifier & value.MODIFIER_DIAGNOSTIC != 0) {
            return lval;
        }

        var rval: ?value.Value = null;
        if (node.right) |right| {
            rval = try self.interpret_node(right);
            if (rval.?.modifier & value.MODIFIER_DIAGNOSTIC != 0) {
                return rval.?;
            }
        }

        switch (node.operator.kind) {
            TokenKind.Plus => {
                return try lval.add(rval.?, node.operator.position);
            },

            TokenKind.Minus => {
                return try lval.substract(rval.?, node.operator.position);
            },

            TokenKind.Star => {
                return try lval.times(rval.?, node.operator.position);
            },

            else => {
                const verr = value.ValueError.init("SeabowBinaryError", "Incorrect binary operator found", node.operator.position);
                return value.Value.init(value.ValueElement{ .Error = verr }, value.MODIFIER_DIAGNOSTIC);
            },
        }
    }

    fn interpret_node_var_const_access(self: *Interpreter, node: nd.NodeVarConstAccess) !value.Value {
        const val = self.elements.get(node.name);
        if (val) |ret_val| {
            return ret_val;
        }

        const verr = value.ValueError.init("AccessError", "Cannot access to an undefined variable or constant", node.position);
        return value.Value.init(value.ValueElement{ .Error = verr }, value.MODIFIER_DIAGNOSTIC);
    }

    fn interpret_node_var_const_decl_list(self: *Interpreter, node: nd.NodeVarConstDeclList) !value.Value {
        for (node.declarations) |decl| {
            const val = try self.interpret_node_var_const_decl(decl, node.constant);
            if (val.modifier & value.MODIFIER_DIAGNOSTIC != 0) {
                return val;
            }
        }

        return value.Value.init(value.ValueElement{ .Null = value.ValueNull{} }, value.MODIFIER_NONE);
    }

    fn interpret_node_type(self: *Interpreter, node: nd.NodeType) !value.Value {
        var vtype = self.elements.get(node.base);
        if (vtype != null) {
            if (vtype.?.value == value.ValueElement.Type) {
                vtype.?.value.Type.detail = node.details;
                return vtype.?;
            }
        }

        const verr = value.ValueError.init("TypeError", "Cannot access to an undefined type", node.position);
        return value.Value.init(value.ValueElement{ .Error = verr }, value.MODIFIER_DIAGNOSTIC);
    }

    fn interpret_node_var_const_decl(self: *Interpreter, node: nd.NodeVarConstDecl, constant: bool) !value.Value {
        if (self.elements.contains(node.name)) {
            const verr = value.ValueError.init("DeclarationError", "Cannot declare a variable or a constant twice", node.position);
            return value.Value.init(value.ValueElement{ .Error = verr }, value.MODIFIER_DIAGNOSTIC);
        }

        var val: value.Value = undefined;
        if (node.expression) |expr| {
            val = try self.interpret_node(expr);
            if (val.modifier & value.MODIFIER_DIAGNOSTIC != 0) {
                return val;
            }

            if (node.ntype) |ntype| {
                const vtype = (try self.interpret_node_type(ntype)).value.Type;
                val = try val.auto_convert(vtype);
            }
        } else {
            const vtype = (try self.interpret_node_type(node.ntype.?)).value.Type;
            const null_val = value.Value.init(value.ValueElement{ .Null = value.ValueNull{} }, value.MODIFIER_NONE);
            val = try null_val.auto_convert(vtype);
        }

        if (val.modifier & value.MODIFIER_DIAGNOSTIC != 0) {
            return val;
        }

        const modifier = if (constant) value.MODIFIER_CONSTANT else value.MODIFIER_NONE;
        val.modifier |= modifier;

        var key = std.ArrayList(u8).init(std.heap.page_allocator);
        errdefer key.deinit();
        try key.writer().print("{s}", .{node.name});

        try self.elements.put(try key.toOwnedSlice(), val);

        return value.Value.init(value.ValueElement{ .Null = value.ValueNull{} }, value.MODIFIER_NONE);
    }
};
