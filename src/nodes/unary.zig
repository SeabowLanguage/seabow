const std = @import("std");
const Token = @import("../utils/token.zig").Token;
const Node = @import("node.zig").Node;

pub const NodeUnary = struct {
    operator: Token,
    operand: *const Node,

    pub fn init(operator: Token, operand: *const Node) NodeUnary {
        return NodeUnary{ .operator = operator, .operand = operand };
    }

    pub fn display(self: NodeUnary, indent: usize) void {
        std.debug.print("NodeUnary({s})\n", .{@tagName(self.operator.kind)});
        self.operand.*.display(indent + 2);
    }
};
