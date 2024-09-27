const std = @import("std");

pub const ValueString = struct {
    value: ?[]const u8,

    pub fn init(val: ?[]const u8) ValueString {
        return ValueString{ .value = val };
    }

    pub fn display(self: ValueString) void {
        std.debug.print("ValueString[{?s}]\n", .{self.value});
    }
};
