const std = @import("std");

pub const ValueNull = struct {
    pub fn display(_: ValueNull) void {
        std.debug.print("ValueNull[null]\n", .{});
    }
};
