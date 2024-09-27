const std = @import("std");

pub const ValueBool = struct {
    value: ?bool,

    pub fn init(val: ?bool) ValueBool {
        return ValueBool{ .value = val };
    }

    pub fn display(self: ValueBool) void {
        std.debug.print("ValueBool[{?}]\n", .{self.value});
    }
};
