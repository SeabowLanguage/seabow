const std = @import("std");
const Position = @import("../utils/position.zig").Position;

pub const NodeType = struct {
    position: Position,
    base: []const u8,
    details: u2,
    contained: ?[]*NodeType,

    pub fn init(pos: Position, base: []const u8, dtl: u2, cntd: ?[]*NodeType) NodeType {
        return NodeType{ .position = pos, .base = base, .details = dtl, .contained = cntd };
    }

    pub fn display(self: NodeType, indent: usize) void {
        std.debug.print("NodeType[{s}]\n", .{self.base});
        if (self.contained) |cntd| {
            const new_indent = indent + 2;
            for (cntd) |other| {
                other.*.display(new_indent);
            }
        }
    }

    pub fn destroy(self: NodeType) void {
        if (self.contained) |cntd| {
            for (cntd) |other| {
                std.heap.page_allocator.destroy(other);
            }
        }
    }

    pub fn copy(self: NodeType) !*NodeType {
        const node_ref = try std.heap.page_allocator.create(NodeType);
        node_ref.* = self;
        return node_ref;
    }
};
