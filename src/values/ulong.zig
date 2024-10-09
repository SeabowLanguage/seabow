const std = @import("std");
const value = @import("value.zig");
const vt = @import("type.zig");

pub const ValueUlong = struct {
    value: ?u64,

    pub fn init(val: ?u64) ValueUlong {
        return ValueUlong{ .value = val };
    }

    pub fn display(self: ValueUlong) void {
        std.debug.print("ValueUlong[{?d}]\n", .{self.value});
    }

    pub fn add(self: ValueUlong, other: value.Value) ?value.Value {
        switch (other.value) {
            .Long => |long| {
                const lval: i64 = @bitCast(self.value.?);
                const val_long = value.ValueLong.init(lval + long.value.?);
                return value.Value.init(value.ValueElement{ .Long = val_long }, value.MODIFIER_NONE);
            },

            .Ulong => |ulong| {
                const val_ulong = ValueUlong.init(self.value.? + ulong.value.?);
                return value.Value.init(value.ValueElement{ .Ulong = val_ulong }, value.MODIFIER_NONE);
            },

            .Double => |double| {
                const lval: f64 = @floatFromInt(self.value.?);
                const val_double = value.ValueDouble.init(lval + double.value.?);
                return value.Value.init(value.ValueElement{ .Double = val_double }, value.MODIFIER_NONE);
            },

            else => return null,
        }
    }

    pub fn substract(self: ValueUlong, other: value.Value) ?value.Value {
        switch (other.value) {
            .Long => |long| {
                const lval: i64 = @bitCast(self.value.?);
                const val_long = value.ValueLong.init(lval - long.value.?);
                return value.Value.init(value.ValueElement{ .Long = val_long }, value.MODIFIER_NONE);
            },

            .Ulong => |ulong| {
                const left: i64 = @bitCast(self.value.?);
                const right: i64 = @bitCast(ulong.value.?);
                const val_long = value.ValueLong.init(left - right);
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

    pub fn negative(self: ValueUlong) value.Value {
        const val: i64 = @bitCast(~(self.value.?) + 1);
        const long = value.ValueLong.init(val);
        return value.Value.init(value.ValueElement{ .Long = long }, value.MODIFIER_NONE);
    }

    pub fn times(self: ValueUlong, other: value.Value) ?value.Value {
        switch (other.value) {
            .Long => |long| {
                const left_val: i64 = @bitCast(self.value.?);
                const val_long = value.ValueLong.init(left_val * long.value.?);
                return value.Value.init(value.ValueElement{ .Long = val_long }, value.MODIFIER_NONE);
            },

            .Ulong => |ulong| {
                const left_val: i64 = @bitCast(self.value.?);
                const right_val: i64 = @bitCast(ulong.value.?);
                const val_long = value.ValueLong.init(left_val * right_val);
                return value.Value.init(value.ValueElement{ .Long = val_long }, value.MODIFIER_NONE);
            },

            .Double => |double| {
                const left_val: f64 = @floatFromInt(self.value.?);
                const val_double = value.ValueDouble.init(left_val + double.value.?);
                return value.Value.init(value.ValueElement{ .Double = val_double }, value.MODIFIER_NONE);
            },

            else => return null,
        }
    }

    pub fn convert(self: ValueUlong, to: vt.ValueType) !?value.Value {
        switch (to.kind) {
            value.ValueKind.String => {
                var str_val: ?[]const u8 = null;
                if (self.value) |val| {
                    var ulong_str = std.ArrayList(u8).init(std.heap.page_allocator);
                    errdefer ulong_str.deinit();
                    try ulong_str.writer().print("{d}", .{val});

                    str_val = try ulong_str.toOwnedSlice();
                }

                const str = value.ValueString.init(str_val);
                return value.Value.init(value.ValueElement{ .String = str }, value.MODIFIER_NONE);
            },

            value.ValueKind.Type => {
                const vtype = vt.ValueType.init(value.ValueKind.Ulong, vt.TYPE_DETAIL_NONE);
                return value.Value.init(value.ValueElement{ .Type = vtype }, value.MODIFIER_NONE);
            },

            else => return null,
        }
    }
};
