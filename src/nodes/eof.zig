const std = @import("std");

pub const NodeEndOfFile = struct {
    pub fn display(_: NodeEndOfFile) void {
        std.debug.print("NodeEndOfFile\n", .{});
    }
};
