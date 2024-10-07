const std = @import("std");
const Position = @import("../utils/position.zig").Position;

pub const NodeVarConstAccess = struct {
    name: []const u8,
    position: Position,

    pub fn init(name: []const u8, position: Position) NodeVarConstAccess {
        return NodeVarConstAccess{ .name = name, .position = position };
    }

    pub fn display(self: NodeVarConstAccess) void {
        std.debug.print("NodeVarConstAccess({s})\n", .{self.name});
    }

    pub fn destroy(_: NodeVarConstAccess) void {}
};
