const std = @import("std");
const Node = @import("node.zig").Node;

pub const NodeReturn = struct {
    returned: ?*const Node,

    pub fn init(ret: ?*const Node) NodeReturn {
        return NodeReturn{ .returned = ret };
    }

    pub fn display(self: NodeReturn, indent: usize) void {
        std.debug.print("NodeReturn\n", .{});
        if (self.returned) |returned| {
            returned.*.display(indent + 2);
        }
    }
};
