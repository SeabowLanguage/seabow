const std = @import("std");
const value = @import("value.zig");
const vt = @import("type.zig");

pub const ValueLong = struct {
    value: ?i64,

    pub fn init(val: ?i64) ValueLong {
        return ValueLong{ .value = val };
    }

    pub fn display(self: ValueLong) void {
        std.debug.print("ValueLong[{?d}]\n", .{self.value});
    }

    pub fn add(self: ValueLong, other: value.Value) ?value.Value {
        switch (other.value) {
            .Long => |long| {
                const val_long = ValueLong.init(self.value.? + long.value.?);
                return value.Value.init(value.ValueElement{ .Long = val_long }, value.MODIFIER_NONE);
            },

            .Ulong => |ulong| {
                const rval: i64 = @bitCast(ulong.value.?);
                const val_long = ValueLong.init(self.value.? + rval);
                return value.Value.init(value.ValueElement{ .Long = val_long }, value.MODIFIER_NONE);
            },

            .Double => |double| {
                const lval: f64 = @floatFromInt(self.value.?);
                const val_double = value.ValueDouble.init(lval + double.value.?);
                return value.Value.init(value.ValueElement{ .Double = val_double }, value.MODIFIER_NONE);
            },

            else => return null,
        }
    }

    pub fn substract(self: ValueLong, other: value.Value) ?value.Value {
        switch (other.value) {
            .Long => |long| {
                const val_long = ValueLong.init(self.value.? - long.value.?);
                return value.Value.init(value.ValueElement{ .Long = val_long }, value.MODIFIER_NONE);
            },

            .Ulong => |ulong| {
                const right: i64 = @bitCast(ulong.value.?);
                const val_long = ValueLong.init(self.value.? - right);
                return value.Value.init(value.ValueElement{ .Long = val_long }, value.MODIFIER_NONE);
            },

            .Double => |double| {
                const lval: f64 = @floatFromInt(self.value.?);
                const val_double = value.ValueDouble.init(lval - double.value.?);
                return value.Value.init(value.ValueElement{ .Double = val_double }, value.MODIFIER_NONE);
            },

            else => return null,
        }
    }

    pub fn negative(self: ValueLong) value.Value {
        const long = ValueLong.init(-self.value.?);
        return value.Value.init(value.ValueElement{ .Long = long }, value.MODIFIER_NONE);
    }

    pub fn convert(self: ValueLong, to: vt.ValueType) !?value.Value {
        switch (to.kind) {
            value.ValueKind.String => {
                var str_val: ?[]const u8 = null;
                if (self.value) |val| {
                    var long_str = std.ArrayList(u8).init(std.heap.page_allocator);
                    errdefer long_str.deinit();
                    try long_str.writer().print("{d}", .{val});

                    str_val = try long_str.toOwnedSlice();
                }

                const str = value.ValueString.init(str_val);
                return value.Value.init(value.ValueElement{ .String = str }, value.MODIFIER_NONE);
            },

            value.ValueKind.Type => {
                const vtype = vt.ValueType.init(value.ValueKind.Long, vt.TYPE_DETAIL_NONE);
                return value.Value.init(value.ValueElement{ .Type = vtype }, value.MODIFIER_NONE);
            },

            else => return null,
        }
    }
};
