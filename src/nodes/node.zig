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

    pub fn display(self: Node, indent: usize) void {
        chars.print_multi_char(' ', indent);
        switch (self) {
            .NoOp => |noop| noop.display(),
            .Eof => |eof| eof.display(),
            .Compound => |comp| comp.display(indent),
            .Unary => |unary| unary.display(indent),
            .Binary => |bin| bin.display(indent),
            .Question => |question| question.display(indent),
            .Break => |brk| brk.display(),
            .Continue => |cnt| cnt.display(),
            .Return => |ret| ret.display(indent),
            else => {},
        }
    }
};
