const std = @import("std");
const chars = @import("../utils/chars.zig");

pub const NodeNoOp = @import("noop.zig").NodeNoOp;
pub const NodeEndOfFile = @import("eof.zig").NodeEndOfFile;
pub const NodeCompound = @import("compound.zig").NodeCompound;
pub const NodeLiteral = @import("literal.zig").NodeLiteral;
pub const NodeUnary = @import("unary.zig").NodeUnary;
pub const NodeBinary = @import("binary.zig").NodeBinary;
pub const NodeQuestion = @import("question.zig").NodeQuestion;
pub const NodeBreak = @import("controller.zig").NodeBreak;
pub const NodeContinue = @import("controller.zig").NodeContinue;
pub const NodeReturn = @import("return.zig").NodeReturn;
pub const NodeType = @import("type.zig").NodeType;
pub const NodeVarConstAccess = @import("var_const_access.zig").NodeVarConstAccess;
pub const NodeVarConstDecl = @import("var_const_decl.zig").NodeVarConstDecl;
pub const NodeVarConstDeclList = @import("var_const_decl_list.zig").NodeVarConstDeclList;

pub const Node = union(enum) {
    NoOp: NodeNoOp,
    Eof: NodeEndOfFile,
    Compound: NodeCompound,
    Literal: NodeLiteral,
    Unary: NodeUnary,
    Binary: NodeBinary,
    Question: NodeQuestion,
    Break: NodeBreak,
    Continue: NodeContinue,
    Return: NodeReturn,
    VarConstAccess: NodeVarConstAccess,
    VarConstDeclList: NodeVarConstDeclList,

    pub fn copy(self: Node) !*Node {
        const node_ref = try std.heap.page_allocator.create(Node);
        node_ref.* = self;
        return node_ref;
    }

    pub fn display(self: Node, indent: usize) void {
        chars.print_multi_char(' ', indent);
        switch (self) {
            .NoOp => |noop| noop.display(),
            .Eof => |eof| eof.display(),
            .Compound => |comp| comp.display(indent),
            .Literal => |lit| lit.display(),
            .Unary => |unary| unary.display(indent),
            .Binary => |bin| bin.display(indent),
            .Question => |question| question.display(indent),
            .Break => |brk| brk.display(),
            .Continue => |cnt| cnt.display(),
            .Return => |ret| ret.display(indent),
            .VarConstAccess => |vca| vca.display(),
            .VarConstDeclList => |vcdl| vcdl.display(indent),
        }
    }

    pub fn destroy(self: *Node) void {
        switch (self.*) {
            .NoOp => |noop| noop.destroy(),
            .Eof => |eof| eof.destroy(),
            .Compound => |comp| comp.destroy(),
            .Literal => |lit| lit.destroy(),
            .Unary => |unary| unary.destroy(),
            .Binary => |bin| bin.destroy(),
            .Question => |question| question.destroy(),
            .Break => |brk| brk.destroy(),
            .Continue => |cnt| cnt.destroy(),
            .Return => |ret| ret.destroy(),
            .VarConstAccess => |vca| vca.destroy(),
            .VarConstDeclList => |vcdl| vcdl.destroy(),
        }

        std.heap.page_allocator.destroy(self);
    }
};
