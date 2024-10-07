const std = @import("std");
const NodeType = @import("type.zig").NodeType;
const Node = @import("node.zig").Node;
const Position = @import("../utils/position.zig").Position;

pub const NodeVarConstDecl = struct {
    name: []const u8,
    position: Position,
    ntype: ?NodeType,
    expression: ?*Node,

    pub fn init(name: []const u8, pos: Position, ntype: ?NodeType, expr: ?*Node) NodeVarConstDecl {
        return NodeVarConstDecl{ .name = name, .position = pos, .ntype = ntype, .expression = expr };
    }

    pub fn display(self: NodeVarConstDecl, indent: usize) void {
        std.debug.print("NodeVarConstDecl[{s}]\n", .{self.name});
        if (self.ntype) |nt| {
            nt.display(indent + 2);
        }

        if (self.expression) |expr| {
            expr.*.display(indent + 2);
        }
    }

    pub fn destroy(self: NodeVarConstDecl) void {
        if (self.ntype) |nt| {
            nt.destroy();
        }

        if (self.expression) |expr| {
            expr.destroy();
        }
    }
};
