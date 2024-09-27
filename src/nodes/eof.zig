const std = @import("std");

pub const NodeEndOfFile = struct {
    pub fn display(_: NodeEndOfFile) void {
        std.debug.print("NodeEndOfFile\n", .{});
    }

    pub fn destroy(_: NodeEndOfFile) void {}
};
