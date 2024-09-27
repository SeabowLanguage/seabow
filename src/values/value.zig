const chars = @import("../utils/chars.zig");

pub const ValueNull = @import("null.zig").ValueNull;
pub const ValueUlong = @import("ulong.zig").ValueUlong;
pub const ValueDouble = @import("double.zig").ValueDouble;
pub const ValueBool = @import("bool.zig").ValueBool;
pub const ValueString = @import("string.zig").ValueString;
pub const ValueError = @import("error.zig").ValueError;

pub const MODIFIER_NONE: u4 = 0b0000;
pub const MODIFIER_POINTER: u4 = 0b0001;
pub const MODIFIER_NOT_NULLABLE: u4 = 0b0010;
pub const MODIFIER_CONSTANT: u4 = 0b0100;
pub const MODIFIER_DIAGNOSTIC: u4 = 0b1000;

pub const ValueElement = union(enum) {
    Null: ValueNull,
    Ulong: ValueUlong,
    Double: ValueDouble,
    Bool: ValueBool,
    String: ValueString,
    Error: ValueError,

    pub fn is_null(self: ValueElement) bool {
        _ = self;
        return true;
    }

    pub fn display(self: ValueElement, indent: usize) void {
        chars.print_multi_char(' ', indent);
        switch (self) {
            .Null => |v_null| v_null.display(),
            .Ulong => |ulong| ulong.display(),
            .Double => |double| double.display(),
            .Bool => |boolean| boolean.display(),
            .String => |string| string.display(),
            .Error => |err| err.display(),
        }
    }
};

pub const Value = struct {
    value: ValueElement,
    modifier: u4,

    pub fn init(val: ValueElement, mod: u4) Value {
        return Value{ .value = val, .modifier = mod };
    }
};
