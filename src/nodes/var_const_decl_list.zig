const std = @import("std");
const NodeVarConstDecl = @import("var_const_decl.zig").NodeVarConstDecl;

pub const NodeVarConstDeclList = struct {
    declarations: []NodeVarConstDecl,
    constant: bool,

    pub fn init(decl: []NodeVarConstDecl, constant: bool) NodeVarConstDeclList {
        return NodeVarConstDeclList{ .declarations = decl, .constant = constant };
    }

    pub fn display(self: NodeVarConstDeclList, indent: usize) void {
        std.debug.print("NodeVarConstDeclList[constant: {}]\n", .{self.constant});
        for (self.declarations) |decl| {
            decl.display(indent + 2);
        }
    }

    pub fn destroy(self: NodeVarConstDeclList) void {
        for (self.declarations) |decl| {
            decl.destroy();
        }
    }
};
