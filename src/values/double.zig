const std = @import("std");

pub const ValueDouble = struct {
    value: ?f64,

    pub fn init(val: ?f64) ValueDouble {
        return ValueDouble{ .value = val };
    }

    pub fn display(self: ValueDouble) void {
        std.debug.print("ValueDouble[{?d}]\n", .{self.value});
    }
};
