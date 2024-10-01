const std = @import("std");
const value = @import("value.zig");
const vt = @import("type.zig");

pub const ValueDouble = struct {
    value: ?f64,

    pub fn init(val: ?f64) ValueDouble {
        return ValueDouble{ .value = val };
    }

    pub fn display(self: ValueDouble) void {
        std.debug.print("ValueDouble[{?d}]\n", .{self.value});
    }

    pub fn add(self: ValueDouble, other: value.Value) ?value.Value {
        switch (other.value) {
            .Long => |long| {
                const rval: f64 = @floatFromInt(long.value.?);
                const val_double = ValueDouble.init(self.value.? + rval);
                return value.Value.init(value.ValueElement{ .Double = val_double }, value.MODIFIER_NONE);
            },

            .Ulong => |ulong| {
                const rval: f64 = @floatFromInt(ulong.value.?);
                const val_double = ValueDouble.init(self.value.? + rval);
                return value.Value.init(value.ValueElement{ .Double = val_double }, value.MODIFIER_NONE);
            },

            .Double => |double| {
                const val_double = value.ValueDouble.init(self.value.? + double.value.?);
                return value.Value.init(value.ValueElement{ .Double = val_double }, value.MODIFIER_NONE);
            },

            else => return null,
        }
    }

    pub fn substract(self: ValueDouble, other: value.Value) ?value.Value {
        switch (other.value) {
            .Long => |long| {
                const right: f64 = @floatFromInt(long.value.?);
                const val_double = ValueDouble.init(self.value.? - right);
                return value.Value.init(value.ValueElement{ .Double = val_double }, value.MODIFIER_NONE);
            },

            .Ulong => |ulong| {
                const right: f64 = @floatFromInt(ulong.value.?);
                const val_double = ValueDouble.init(self.value.? - right);
                return value.Value.init(value.ValueElement{ .Double = val_double }, value.MODIFIER_NONE);
            },

            .Double => |double| {
                const val_double = ValueDouble.init(self.value.? - double.value.?);
                return value.Value.init(value.ValueElement{ .Double = val_double }, value.MODIFIER_NONE);
            },

            else => return null,
        }
    }

    pub fn negative(self: ValueDouble) value.Value {
        const double = ValueDouble.init(-self.value.?);
        return value.Value.init(value.ValueElement{ .Double = double }, value.MODIFIER_NONE);
    }

    pub fn convert(self: ValueDouble, to: vt.ValueType) !?value.Value {
        switch (to.kind) {
            value.ValueKind.String => {
                var str_val: ?[]const u8 = null;
                if (self.value) |val| {
                    var double_str = std.ArrayList(u8).init(std.heap.page_allocator);
                    errdefer double_str.deinit();
                    try double_str.writer().print("{d}", .{val});

                    str_val = try double_str.toOwnedSlice();
                }

                const str = value.ValueString.init(str_val);
                return value.Value.init(value.ValueElement{ .String = str }, value.MODIFIER_NONE);
            },

            value.ValueKind.Type => {
                const vtype = vt.ValueType.init(value.ValueKind.Double, vt.TYPE_DETAIL_NONE);
                return value.Value.init(value.ValueElement{ .Type = vtype }, value.MODIFIER_NONE);
            },

            else => return null,
        }
    }
};
