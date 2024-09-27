const std = @import("std");

pub const ValueUlong = struct {
    value: ?u64,

    pub fn init(val: ?u64) ValueUlong {
        return ValueUlong{ .value = val };
    }

    pub fn display(self: ValueUlong) void {
        std.debug.print("ValueUlong[{?d}]\n", .{self.value});
    }
};
