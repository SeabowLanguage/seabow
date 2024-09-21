const std = @import("std");
const Node = @import("node.zig").Node;

pub const NodeQuestion = struct {
    condition: *const Node,
    first: *const Node,
    second: *const Node,

    pub fn init(condition: *const Node, first: *const Node, second: *const Node) NodeQuestion {
        return NodeQuestion{ .condition = condition, .first = first, .second = second };
    }

    pub fn display(self: NodeQuestion, indent: usize) void {
        std.debug.print("NodeQuestionOperation\n", .{});
        self.condition.*.display(indent + 2);
        self.first.*.display(indent + 2);
        self.second.*.display(indent + 2);
    }
};
