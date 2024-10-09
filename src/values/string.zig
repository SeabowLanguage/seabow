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

    pub fn add(self: ValueString, other: value.Value) !?value.Value {
        switch (other.value) {
            .String => |string| {
                var str = std.ArrayList(u8).init(std.heap.page_allocator);
                errdefer str.deinit();
                try str.writer().print("{s}{s}", .{ self.value.?, string.value.? });
                const val_str = ValueString.init(try str.toOwnedSlice());
                return value.Value.init(value.ValueElement{ .String = val_str }, value.MODIFIER_NONE);
            },

            else => return null,
        }
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
