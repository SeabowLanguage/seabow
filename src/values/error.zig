const std = @import("std");

pub const ValueError = struct {
    name: ?[]const u8,
    details: ?[]const u8,

    pub fn init(name: ?[]const u8, dtls: ?[]const u8) ValueError {
        return ValueError{ .name = name, .details = dtls };
    }

    pub fn display(self: ValueError) void {
        std.debug.print("ValueError[{?s}: {?s}]\n", .{ self.name, self.details });
    }
};
