const std = @import("std");

pub const NodeNoOp = struct {
    pub fn display(_: NodeNoOp) void {
        std.debug.print("NodeNoOp\n", .{});
    }

    pub fn destroy(_: NodeNoOp) void {}
};
