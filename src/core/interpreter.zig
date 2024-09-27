const std = @import("std");

const opt = @import("../utils/options.zig");
const nd = @import("../nodes/node.zig");
const value = @import("../values/value.zig");
const diagnostic = @import("../utils/diagnostic.zig");
const Parser = @import("parser.zig").Parser;
const SourceText = @import("../utils/source_text.zig").SourceText;

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

            errdefer std.heap.page_allocator.free(code);
            const source = SourceText.init(code);
            var parser = try Parser.init(source, &diagnostics);
            const root = try parser.parse();

            if (diagnostics.items.len == 0) {
                std.debug.print("\x1b[90m", .{});
                root.display(0);
                std.debug.print("\x1b[0m", .{});

                const val = interp.interpret_node(root);
                if (val.modifier & value.MODIFIER_DIAGNOSTIC != 0) {
                    std.debug.print("\x1b[31m", .{});
                } else {
                    std.debug.print("\x1b[32m", .{});
                }
                val.value.display(0);
                std.debug.print("\x1b[0m\n", .{});
            } else {
                for (diagnostics.items) |diag| {
                    diag.display(source);
                }
            }

            std.heap.page_allocator.free(code);
            diagnostics.clearAndFree();
        }

        return 0;
    }

    pub fn interpret_file(options: []opt.Option) u8 {
        _ = options;
        return 0;
    }

    fn interpret_node(self: *Interpreter, node: *nd.Node) value.Value {
        return switch (node.*) {
            .NoOp => value.Value.init(value.ValueElement{ .Null = value.ValueNull{} }, value.MODIFIER_NONE),
            .Compound => |comp| self.interpret_node_compound(comp),
            .Literal => |lit| lit.value,

            else => value.Value.init(value.ValueElement{ .Error = value.ValueError.init("TODO", "Implement other interpretations") }, value.MODIFIER_DIAGNOSTIC),
        };
    }

    fn interpret_node_compound(self: *Interpreter, node: nd.NodeCompound) value.Value {
        var ret_val: value.Value = undefined;
        for (node.nodes) |next_node| {
            ret_val = self.interpret_node(next_node);
        }

        return ret_val;
    }
};
