const std = @import("std");
const value = @import("value.zig");
const vt = @import("type.zig");

pub const ValueString = struct {
    value: ?[]const u8,

    pub fn init(val: ?[]const u8) ValueString {
        return ValueString{ .value = val };
    }

    pub fn display(self: ValueString) void {
        std.debug.print("ValueString[{?s}]\n", .{self.value});
    }

    pub fn convert(self: ValueString, to: vt.ValueType) ?value.Value {
        switch (to.kind) {
            value.ValueKind.String => {
                const str = ValueString.init(self.value);
                return value.Value.init(value.ValueElement{ .String = str }, value.MODIFIER_NONE);
            },

            value.ValueKind.Type => {
                const vtype = vt.ValueType.init(value.ValueKind.String, vt.TYPE_DETAIL_NONE);
                return value.Value.init(value.ValueElement{ .Type = vtype }, value.MODIFIER_NONE);
            },

            else => return null,
        }
    }
};
