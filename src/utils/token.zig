const std = @import("std");
const Position = @import("position.zig").Position;
const SourceText = @import("source_text.zig").SourceText;

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
    Character,
    String,
    RawString,
    FormattedString,
    Macro,

    // Keywords
    Var,
    Const,
    Func,
    Return,
    Break,
    Continue,
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
    Throw,

    // Operators
    Plus,
    PlusEquals,
    PlusPlus,
    Minus,
    MinusEquals,
    MinusMinus,
    Star,
    StarEquals,
    Divide,
    DivideEquals,
    Modulo,
    ModuloEquals,
    LessThan,
    LessEquals,
    LeftShift,
    LeftShiftEquals,
    GreaterThan,
    GreaterEquals,
    RightShift,
    RightShiftEquals,
    Hat,
    HatEquals,
    Tilde,
    Assign,
    Equals,
    Not,
    NotEquals,
    Ampersand,
    AmpersandEquals,
    AmpersandAmpersand,
    Pipe,
    PipeEquals,
    PipePipe,
    Question,

    // Others
    NewLine,
    LeftParenthesis,
    RightParenthesis,
    LeftBrace,
    RightBrace,
    LeftBracket,
    RightBracket,
    Colon,
    Semicolon,
    Dot,
    Comma,
};

pub const Token = struct {
    kind: TokenKind,
    position: Position,

    pub fn init(kind: TokenKind, pos: Position) Token {
        return Token{ .kind = kind, .position = pos };
    }

    pub fn display(self: Token, source: SourceText) void {
        std.debug.print("Token[kind: {s}, pos: {}, text: {s}]\n", .{ @tagName(self.kind), self.position, source.text[self.position.start..self.position.end()] });
    }

    pub fn unary_precedence(self: Token) u8 {
        return switch (self.kind) {
            TokenKind.Ampersand => 18,
            TokenKind.PlusPlus, TokenKind.MinusMinus => 16,
            TokenKind.Not, TokenKind.Tilde, TokenKind.Plus, TokenKind.Minus => 15,
            TokenKind.Question => 2,

            else => 0,
        };
    }

    pub fn binary_precedence(self: Token) u8 {
        return switch (self.kind) {
            TokenKind.Dot => 20,
            TokenKind.PlusPlus, TokenKind.MinusMinus => 17,
            TokenKind.Star, TokenKind.Divide, TokenKind.Modulo => 13,
            TokenKind.Plus, TokenKind.Minus => 12,

            else => 0,
        };
    }
};
