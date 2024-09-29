const std = @import("std");
const value = @import("value.zig");

pub const TYPE_DETAIL_NONE: u2 = 0b00;
pub const TYPE_DETAIL_REF: u2 = 0b01;
pub const TYPE_DETAIL_NOT_NULL: u2 = 0b10;

pub const TO_STRING = ValueType.init(value.ValueKind.String, TYPE_DETAIL_NONE);
pub const TO_TYPE = ValueType.init(value.ValueKind.Type, TYPE_DETAIL_NONE);

pub const ValueType = struct {
    kind: value.ValueKind,
    detail: u2,
    contained: ?[]ValueType,

    pub fn init(kind: value.ValueKind, dtl: u2) ValueType {
        return ValueType{ .kind = kind, .detail = dtl, .contained = null };
    }

    pub fn init_with_contained(kind: value.ValueKind, dtl: u2, cntd: []ValueType) ValueType {
        return ValueType{ .kind = kind, .detail = dtl, .contained = cntd };
    }

    pub fn display(self: ValueType) void {
        std.debug.print("ValueType[{s}]\n", .{self.kind.repr()});
    }

    pub fn convert(self: ValueType, to: ValueType) ?value.Value {
        switch (to.kind) {
            value.ValueKind.String => {
                var str_type: []const u8 = self.kind.repr();
                str_type = if (str_type.len == 0) "<null>" else str_type;
                const str = value.ValueString.init(str_type);
                return value.Value.init(value.ValueElement{ .String = str }, value.MODIFIER_NONE);
            },

            value.ValueKind.Type => {
                const vtype = ValueType.init(value.ValueKind.Type, TYPE_DETAIL_NONE);
                return value.Value.init(value.ValueElement{ .Type = vtype }, value.MODIFIER_NONE);
            },

            else => {
                return null;
            },
        }
    }
};