const std = @import("std");
const Value = @import("../values/value.zig").Value;
const Position = @import("../utils/position.zig").Position;

pub const NodeLiteral = struct {
    value: Value,
    position: Position,

    pub fn init(value: Value, position: Position) NodeLiteral {
        return NodeLiteral{ .value = value, .position = position };
    }

    pub fn display(self: NodeLiteral) void {
        std.debug.print("NodeLiteral - ", .{});
        self.value.value.display(0);
    }

    pub fn destroy(_: NodeLiteral) void {}
};
