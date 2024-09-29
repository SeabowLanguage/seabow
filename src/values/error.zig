const std = @import("std");
const Position = @import("../utils/position.zig").Position;
const value = @import("value.zig");
const vt = @import("type.zig");

pub const ValueError = struct {
    name: ?[]const u8,
    details: ?[]const u8,
    position: ?Position,

    pub fn init(name: ?[]const u8, dtls: ?[]const u8, pos: ?Position) ValueError {
        return ValueError{ .name = name, .details = dtls, .position = pos };
    }

    pub fn display(self: ValueError) void {
        std.debug.print("ValueError[{?s}: {?s}]\n", .{ self.name, self.details });
    }

    pub fn convert(self: ValueError, to: vt.ValueType) !?value.Value {
        switch (to.kind) {
            value.ValueKind.String => {
                var str_val: ?[]const u8 = null;
                if (self.name) |name| {
                    var size: usize = name.len;
                    if (self.details) |details| {
                        size += details.len;
                    }

                    var err_str = std.ArrayList(u8).init(std.heap.page_allocator);
                    errdefer err_str.deinit();
                    if (self.details) |details| {
                        try err_str.writer().print("{s}: {s}", .{ name, details });
                    } else {
                        try err_str.writer().print("{s}", .{name});
                    }

                    str_val = try err_str.toOwnedSlice();
                }

                const str = value.ValueString.init(str_val);
                return value.Value.init(value.ValueElement{ .String = str }, value.MODIFIER_NONE);
            },

            value.ValueKind.Type => {
                const vtype = vt.ValueType.init(value.ValueKind.Error, vt.TYPE_DETAIL_NONE);
                return value.Value.init(value.ValueElement{ .Type = vtype }, value.MODIFIER_NONE);
            },

            else => return null,
        }
    }
};
