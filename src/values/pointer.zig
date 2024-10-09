const std = @import("std");
const value = @import("value.zig");
const vt = @import("type.zig");

pub const ValuePointer = struct {
    pointer: ?[]const u8,
    ptype: vt.ValueType,

    pub fn init(pointer: ?[]const u8, ptype: vt.ValueType) ValuePointer {
        return ValuePointer{ .pointer = pointer, .ptype = ptype };
    }
};
