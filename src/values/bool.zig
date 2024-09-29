const std = @import("std");
const vt = @import("type.zig");
const value = @import("value.zig");

pub const ValueBool = struct {
    value: ?bool,

    pub fn init(val: ?bool) ValueBool {
        return ValueBool{ .value = val };
    }

    pub fn display(self: ValueBool) void {
        std.debug.print("ValueBool[{?}]\n", .{self.value});
    }

    pub fn not(self: ValueBool) value.Value {
        var res: bool = true;
        if (self.value) |val| {
            res = !val;
        }

        const boolean = ValueBool.init(res);
        return value.Value.init(value.ValueElement{ .Bool = boolean }, value.MODIFIER_NONE);
    }

    pub fn convert(self: ValueBool, to: vt.ValueType) ?value.Value {
        switch (to.kind) {
            value.ValueKind.String => {
                var str_val: ?[]const u8 = null;
                if (self.value) |val| {
                    str_val = if (val) "true" else "false";
                }

                const str = value.ValueString.init(str_val);
                return value.Value.init(value.ValueElement{ .String = str }, value.MODIFIER_NONE);
            },

            value.ValueKind.Type => {
                const vtype = vt.ValueType.init(value.ValueKind.Bool, vt.TYPE_DETAIL_NONE);
                return value.Value.init(value.ValueElement{ .Type = vtype }, value.MODIFIER_NONE);
            },

            else => return null,
        }
    }
};
