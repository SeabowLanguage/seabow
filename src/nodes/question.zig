const std = @import("std");
const Node = @import("node.zig").Node;

pub const NodeQuestion = struct {
    condition: *Node,
    first: *Node,
    second: *Node,

    pub fn init(condition: *Node, first: *Node, second: *Node) NodeQuestion {
        return NodeQuestion{ .condition = condition, .first = first, .second = second };
    }

    pub fn display(self: NodeQuestion, indent: usize) void {
        std.debug.print("NodeQuestionOperation\n", .{});
        self.condition.*.display(indent + 2);
        self.first.*.display(indent + 2);
        self.second.*.display(indent + 2);
    }

    pub fn destroy(self: NodeQuestion) void {
        self.condition.destroy();
        self.first.destroy();
        self.second.destroy();
    }
};
