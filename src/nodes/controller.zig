const std = @import("std");

pub const NodeBreak = struct {
    pub fn display(_: NodeBreak) void {
        std.debug.print("NodeBreak\n", .{});
    }
};

pub const NodeContinue = struct {
    pub fn display(_: NodeContinue) void {
        std.debug.print("NodeContinue\n", .{});
    }
};
