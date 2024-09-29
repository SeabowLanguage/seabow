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
    elements: *std.StringHashMap(value.Value),

    fn init() Interpreter {
        var elements = std.StringHashMap(value.Value).init(std.heap.page_allocator);
        return Interpreter{ .elements = &elements };
    }

    fn destroy(self: Interpreter) void {
        self.root.destroy();
        self.elements.deinit();
    }

    pub fn interpret_console(_: []opt.Option) !u8 {
        const stdin = std.io.getStdIn().reader();
        var diagnostics = std.ArrayList(diagnostic.Diagnostic).init(std.heap.page_allocator);
        defer diagnostics.deinit();
        var interp = Interpreter.init();

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

        const code = try source_file.readToEndAlloc(std.heap.page_allocator, try source_file.getEndPos());
        defer std.heap.page_allocator.free(code);

        var diagnostics = std.ArrayList(diagnostic.Diagnostic).init(std.heap.page_allocator);
        defer diagnostics.deinit();

        const source = SourceText.init(code);
        var parser = try Parser.init(source, &diagnostics);
        const root = try parser.parse();

        if (diagnostics.items.len == 0) {
            var interp = Interpreter.init();
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

            else => value.Value.init(value.ValueElement{
                .Error = value.ValueError.init("TODO", "Implement other interpretations", null),
            }, value.MODIFIER_DIAGNOSTIC),
        };
    }

    fn interpret_node_compound(self: *Interpreter, node: nd.NodeCompound) !value.Value {
        var ret_val: value.Value = undefined;
        for (node.nodes) |next_node| {
            ret_val = try self.interpret_node(next_node);
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

            else => {
                const verr = value.ValueError.init("SeabowBinaryError", "Incorrect binary operator found", node.operator.position);
                return value.Value.init(value.ValueElement{ .Error = verr }, value.MODIFIER_DIAGNOSTIC);
            },
        }
    }
};
