const chars = @import("../utils/chars.zig");
const std = @import("std");
const Position = @import("../utils/position.zig").Position;

pub const ValueNull = @import("null.zig").ValueNull;
pub const ValueLong = @import("long.zig").ValueLong;
pub const ValueUlong = @import("ulong.zig").ValueUlong;
pub const ValueDouble = @import("double.zig").ValueDouble;
pub const ValueBool = @import("bool.zig").ValueBool;
pub const ValueString = @import("string.zig").ValueString;
pub const ValueError = @import("error.zig").ValueError;
pub const vt = @import("type.zig");

pub const MODIFIER_NONE: u4 = 0b0000;
pub const MODIFIER_NOT_NULLABLE: u4 = 0b0001;
pub const MODIFIER_CONSTANT: u4 = 0b0010;
pub const MODIFIER_DIAGNOSTIC: u4 = 0b0100;

pub const ValueKind = enum(u8) {
    // Special
    None,

    // Numbers
    Long,
    Ulong,
    Double,
    Bool,

    // Character
    Char,
    String,

    // Complex
    Error,
    Type,

    pub fn repr(self: ValueKind) []const u8 {
        return switch (self) {
            ValueKind.None => "null",
            ValueKind.Long => "long",
            ValueKind.Ulong => "ulong",
            ValueKind.Double => "double",
            ValueKind.Bool => "bool",
            ValueKind.Char => "char",
            ValueKind.String => "string",
            ValueKind.Error => "error",
            ValueKind.Type => "type",
        };
    }
};

pub const ValueElement = union(enum) {
    Null: ValueNull,
    Long: ValueLong,
    Ulong: ValueUlong,
    Double: ValueDouble,
    Bool: ValueBool,
    String: ValueString,
    Error: ValueError,
    Type: vt.ValueType,

    pub fn display(self: ValueElement, indent: usize) void {
        chars.print_multi_char(' ', indent);
        switch (self) {
            .Null => |v_null| v_null.display(),
            .Long => |long| long.display(),
            .Ulong => |ulong| ulong.display(),
            .Double => |double| double.display(),
            .Bool => |boolean| boolean.display(),
            .String => |string| string.display(),
            .Error => |err| err.display(),
            .Type => |tp| tp.display(),
        }
    }

    pub fn kind(self: ValueElement) ValueKind {
        return switch (self) {
            .Null => ValueKind.Null,
            .Long => ValueKind.Long,
            .Ulong => ValueKind.Ulong,
            .Double => ValueKind.Double,
            .Bool => ValueKind.Bool,
            .String => ValueKind.String,
            .Error => ValueKind.Error,
            .Type => vt.ValueType,
        };
    }
};

pub const Value = struct {
    value: ValueElement,
    modifier: u4,

    fn unary_operation_error(left: Value, op: []const u8, pos: Position) anyerror!Value {
        const ltype = try (try left.convert(vt.TO_TYPE)).convert(vt.TO_STRING);

        var err_dtls = std.ArrayList(u8).init(std.heap.page_allocator);
        errdefer err_dtls.deinit();
        try err_dtls.writer().print("Unary `{s}` is not defined for type {s}", .{ op, ltype.value.String.value.? });

        const verr = ValueError.init("OperatorError", try err_dtls.toOwnedSlice(), pos);
        return Value.init(ValueElement{ .Error = verr }, MODIFIER_DIAGNOSTIC);
    }

    fn binary_operation_error(left: Value, op: []const u8, right: Value, pos: Position) anyerror!Value {
        const ltype = try (try left.convert(vt.TO_TYPE)).convert(vt.TO_STRING);
        const rtype = try (try right.convert(vt.TO_TYPE)).convert(vt.TO_STRING);

        var err_dtls = std.ArrayList(u8).init(std.heap.page_allocator);
        errdefer err_dtls.deinit();
        try err_dtls.writer().print("Binary `{s}` is not defined for types {s} and {s}", .{ op, ltype.value.String.value.?, rtype.value.String.value.? });

        const verr = ValueError.init("OperatorError", try err_dtls.toOwnedSlice(), pos);
        return Value.init(ValueElement{ .Error = verr }, MODIFIER_DIAGNOSTIC);
    }

    fn null_operation_error(pos: Position) Value {
        const verr = ValueError.init("OperatorError", "Cannnot use null value as an operand of this operation", pos);
        return Value.init(ValueElement{ .Error = verr }, MODIFIER_DIAGNOSTIC);
    }

    fn convert_error(from: Value, to: vt.ValueType) anyerror!Value {
        const ftype = try (try from.convert(vt.TO_TYPE)).convert(vt.TO_STRING);
        const ttype = (try to.convert(vt.TO_STRING)).?;

        var err_dtls = std.ArrayList(u8).init(std.heap.page_allocator);
        errdefer err_dtls.deinit();
        try err_dtls.writer().print("Undefined convertion from type {s} to type {s}", .{ ftype.value.String.value.?, ttype.value.String.value.? });

        const verr = ValueError.init("ConvertionError", try err_dtls.toOwnedSlice(), null);
        return Value.init(ValueElement{ .Error = verr }, MODIFIER_DIAGNOSTIC);
    }

    fn auto_convert_error(from: Value, to: vt.ValueType) anyerror!Value {
        const ftype = try (try from.convert(vt.TO_TYPE)).convert(vt.TO_STRING);
        const ttype = (try to.convert(vt.TO_STRING)).?;

        var err_dtls = std.ArrayList(u8).init(std.heap.page_allocator);
        errdefer err_dtls.deinit();
        try err_dtls.writer().print("Undefined auto convertion from type {s} to type {s}", .{ ftype.value.String.value.?, ttype.value.String.value.? });

        const verr = ValueError.init("AutoConvertionError", try err_dtls.toOwnedSlice(), null);
        return Value.init(ValueElement{ .Error = verr }, MODIFIER_DIAGNOSTIC);
    }

    pub fn init(val: ValueElement, mod: u4) Value {
        return Value{ .value = val, .modifier = mod };
    }

    pub fn is_null(self: Value) bool {
        switch (self.value) {
            .Null => return true,
            .Long => |long| return long.value == null,
            .Ulong => |ulong| return ulong.value == null,
            .Double => |double| return double.value == null,
            .Bool => |boolean| return boolean.value == null,
            .String => |string| return string.value == null,
            .Error => |err| return err.name == null,
            .Type => |vtype| return vtype.kind == ValueKind.None,
        }
    }

    pub fn add(self: Value, other: Value, pos: Position) !Value {
        if (self.is_null() or other.is_null()) {
            return Value.null_operation_error(pos);
        }

        var ret: ?Value = null;
        switch (self.value) {
            .Long => |long| {
                ret = long.add(other);
            },

            .Ulong => |ulong| {
                ret = ulong.add(other);
            },

            .Double => |double| {
                ret = double.add(other);
            },

            .String => |string| {
                ret = try string.add(other);
            },

            else => {},
        }

        if (ret) |returned| {
            return returned;
        } else {
            return try Value.binary_operation_error(self, "+", other, pos);
        }
    }

    pub fn substract(self: Value, other: Value, pos: Position) !Value {
        if (self.is_null() or other.is_null()) {
            return Value.null_operation_error(pos);
        }

        var ret: ?Value = null;
        switch (self.value) {
            .Long => |long| {
                ret = long.substract(other);
            },

            .Ulong => |ulong| {
                ret = ulong.substract(other);
            },

            .Double => |double| {
                ret = double.substract(other);
            },

            else => {},
        }

        if (ret) |returned| {
            return returned;
        } else {
            return try Value.binary_operation_error(self, "-", other, pos);
        }
    }

    pub fn negative(self: Value, pos: Position) !Value {
        if (self.is_null()) {
            return Value.null_operation_error(pos);
        }

        var ret: ?Value = null;
        switch (self.value) {
            .Long => |long| {
                ret = long.negative();
            },

            .Ulong => |ulong| {
                ret = ulong.negative();
            },

            .Double => |double| {
                ret = double.negative();
            },

            else => {},
        }

        if (ret) |returned| {
            return returned;
        } else {
            return try Value.unary_operation_error(self, "-", pos);
        }
    }

    pub fn times(self: Value, other: Value, pos: Position) !Value {
        if (self.is_null() or other.is_null()) {
            return Value.null_operation_error(pos);
        }

        var ret: ?Value = null;
        switch (self.value) {
            .Long => |long| {
                ret = long.times(other);
            },

            .Ulong => |ulong| {
                ret = ulong.times(other);
            },

            .Double => |double| {
                ret = double.times(other);
            },

            else => {},
        }

        if (ret) |returned| {
            return returned;
        } else {
            return try Value.binary_operation_error(self, "*", other, pos);
        }
    }

    pub fn not(self: Value, pos: Position) !Value {
        if (self.is_null()) {
            const val = ValueBool.init(true);
            return Value.init(ValueElement{ .Bool = val }, MODIFIER_NONE);
        }

        var ret: ?Value = null;
        switch (self.value) {
            .Bool => |boolean| {
                ret = boolean.not();
            },

            else => {},
        }

        if (ret) |returned| {
            return returned;
        } else {
            return try unary_operation_error(self, "!", pos);
        }
    }

    pub fn convert(self: Value, to: vt.ValueType) anyerror!Value {
        var ret: ?Value = null;
        switch (self.value) {
            .Null => |vnull| {
                ret = vnull.convert(to);
            },

            .Long => |long| {
                ret = try long.convert(to);
            },

            .Ulong => |ulong| {
                ret = try ulong.convert(to);
            },

            .Double => |double| {
                ret = try double.convert(to);
            },

            .Bool => |boolean| {
                ret = boolean.convert(to);
            },

            .String => |string| {
                ret = string.convert(to);
            },

            .Error => |err| {
                ret = try err.convert(to);
            },

            .Type => |vtype| {
                ret = try vtype.convert(to);
            },
        }

        if (ret) |returned| {
            return returned;
        } else {
            return try self.convert_error(to);
        }
    }

    pub fn auto_convert(self: Value, to: vt.ValueType) anyerror!Value {
        var ret: ?Value = null;
        switch (self.value) {
            .Null => |nl| ret = nl.auto_convert(to),

            else => {},
        }

        if (ret) |returned| {
            return returned;
        } else {
            return try self.convert(to);
        }
    }
};
