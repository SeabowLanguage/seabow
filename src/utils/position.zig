const std = @import("std");

pub const Position = struct {
    start: usize,
    length: usize,

    pub fn init(start: usize, length: usize) Position {
        return Position{ .start = start, .length = length };
    }

    pub fn end(self: Position) usize {
        return self.start + self.length;
    }

    pub fn format(self: Position, comptime _: []const u8, _: std.fmt.FormatOptions, writer: anytype) !void {
        try writer.print("Position[start: {}, end: {}]", .{ self.start, self.end() });
    }
};
