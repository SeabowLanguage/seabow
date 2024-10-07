const std = @import("std");
const value = @import("value.zig");
const vt = @import("type.zig");

pub const ValueNull = struct {
    pub fn display(_: ValueNull) void {
        std.debug.print("ValueNull[null]\n", .{});
    }

    pub fn convert(_: ValueNull, to: vt.ValueType) ?value.Value {
        switch (to.kind) {
            value.ValueKind.String => {
                const str = value.ValueString.init(null);
                return value.Value.init(value.ValueElement{ .String = str }, value.MODIFIER_NONE);
            },

            value.ValueKind.Type => {
                const vtype = vt.ValueType.init(value.ValueKind.None, vt.TYPE_DETAIL_NONE);
                return value.Value.init(value.ValueElement{ .Type = vtype }, value.MODIFIER_NONE);
            },

            else => {
                return null;
            },
        }
    }

    pub fn auto_convert(_: ValueNull, to: vt.ValueType) ?value.Value {
        switch (to.kind) {
            value.ValueKind.Long => {
                return value.Value.init(value.ValueElement{ .Long = value.ValueLong.init(null) }, value.MODIFIER_NONE);
            },

            else => {
                return null;
            },
        }
    }
};
