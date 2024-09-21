const std = @import("std");
const Node = @import("node.zig").Node;

pub const NodeCompound = struct {
    nodes: []*const Node,

    pub fn init(nodes: []*const Node) NodeCompound {
        return NodeCompound{ .nodes = nodes };
    }

    pub fn display(self: NodeCompound, indent: usize) void {
        if (indent == 0) {
            std.debug.print("ROOT\n", .{});
        } else {
            std.debug.print("NodeCompound\n", .{});
        }

        for (self.nodes) |nd| {
            nd.*.display(indent + 2);
        }
    }
};