const std = @import("std");
const Position = @import("position.zig").Position;

pub const TokenKind = enum {
    // Base
    BadToken,
    EndOfFile,
    Identifier,
    Integer,
    BigInteger,
    Decimal,
    BigDecimal,
    BinaryInteger,
    OctalInteger,
    HexaInteger,

    // Keywords
    Var,
    Const,
    Func,
    Null,
    False,
    True,
    If,
    Elif,
    Else,
    Switch,
    Case,
    Default,
    For,
    Foreach,
    While,
    Do,
    Try,
    Catch,
    Finally,

    // Operators
    Plus,
    PlusEquals,
    PlusPlus,
    Minus,
    MinusEquals,
    MinusMinus,
    LessThan,
    LessEquals,
    LeftShift,
    LeftShiftEquals,
    GreaterThan,
    GreaterEquals,
    RightShift,
    RightShiftEquals,
    Assign,
    Equals,
    Not,
    NotEquals,

    // Others
    LeftParenthesis,
    RightParenthesis,
    LeftBrace,
    RightBrace,
    Colon,
    Semicolon,
    Dot,
};

pub const Token = struct {
    kind: TokenKind,
    position: Position,
    value: ?[]const u8,

    pub fn init(kind: TokenKind, pos: Position, value: ?[]const u8) Token {
        return Token{ .kind = kind, .position = pos, .value = value };
    }

    pub fn format(self: Token, comptime _: []const u8, _: std.fmt.FormatOptions, writer: anytype) !void {
        if (self.value == null) {
            try writer.print("Token[kind: {s}, pos: {}]", .{ @tagName(self.kind), self.position });
        } else {
            try writer.print("Token[kind: {s}, pos: {}, value: {s}]", .{ @tagName(self.kind), self.position, self.value.? });
        }
    }
};
