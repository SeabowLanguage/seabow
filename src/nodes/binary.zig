const std = @import("std");
const Token = @import("../utils/token.zig").Token;
const Node = @import("node.zig").Node;

pub const NodeBinary = struct {
    left: *Node,
    operator: Token,
    right: ?*Node,

    pub fn init(left: *Node, operator: Token, right: ?*Node) NodeBinary {
        return NodeBinary{ .left = left, .operator = operator, .right = right };
    }

    pub fn display(self: NodeBinary, indent: usize) void {
        std.debug.print("NodeBinary({s})\n", .{@tagName(self.operator.kind)});

        self.left.*.display(indent + 2);
        if (self.right) |right| {
            right.*.display(indent + 2);
        }
    }

    pub fn destroy(self: NodeBinary) void {
        self.left.destroy();
        if (self.right) |right| {
            right.destroy();
        }
    }
};
