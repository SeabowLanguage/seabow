const std = @import("std");
const tok = @import("../utils/token.zig");
const diagnostic = @import("../utils/diagnostic.zig");
const Position = @import("../utils/position.zig").Position;
const SourceText = @import("../utils/source_text.zig").SourceText;

var keywords: std.StringHashMap(tok.TokenKind) = undefined;

pub fn init_keywords() !void {
    keywords = std.StringHashMap(tok.TokenKind).init(std.heap.page_allocator);

    try keywords.put("var", tok.TokenKind.Var);
    try keywords.put("const", tok.TokenKind.Const);
    try keywords.put("func", tok.TokenKind.Func);

    try keywords.put("null", tok.TokenKind.Null);
    try keywords.put("false", tok.TokenKind.False);
    try keywords.put("true", tok.TokenKind.True);

    try keywords.put("if", tok.TokenKind.If);
    try keywords.put("elif", tok.TokenKind.Elif);
    try keywords.put("else", tok.TokenKind.Else);
    try keywords.put("switch", tok.TokenKind.Switch);
    try keywords.put("case", tok.TokenKind.Case);
    try keywords.put("default", tok.TokenKind.Default);

    try keywords.put("for", tok.TokenKind.For);
    try keywords.put("foreach", tok.TokenKind.Foreach);
    try keywords.put("while", tok.TokenKind.While);
    try keywords.put("do", tok.TokenKind.Do);

    try keywords.put("try", tok.TokenKind.Try);
    try keywords.put("catch", tok.TokenKind.Catch);
    try keywords.put("finally", tok.TokenKind.Finally);
}

pub const Lexer = struct {
    code: SourceText,
    diagnostics: *std.ArrayList(diagnostic.Diagnostic),
    index: usize,

    pub fn init(code: SourceText, diags: *std.ArrayList(diagnostic.Diagnostic)) Lexer {
        return Lexer{ .code = code, .diagnostics = diags, .index = 0 };
    }

    pub fn lex(self: *Lexer) !tok.Token {
        self.skip_spaces();
        const current = self.get();
        if (current == 0) {
            return tok.Token.init(tok.TokenKind.EndOfFile, Position.init(self.index, 1), null);
        }

        if ((current >= 'A' and current <= 'Z') or (current >= 'a' and current <= 'z') or current == '_') {
            return try self.lex_word();
        } else if (current >= '0' and current <= '9') {
            return try self.lex_number();
        }

        const after = self.get_at(1);
        switch (current) {
            '+' => {
                if (after == '+') {
                    return self.lex_other(2, tok.TokenKind.PlusPlus);
                } else if (after == '=') {
                    return self.lex_other(2, tok.TokenKind.PlusEquals);
                }
                return self.lex_other(1, tok.TokenKind.Plus);
            },

            '-' => {
                if (after == '-') {
                    return self.lex_other(2, tok.TokenKind.MinusMinus);
                } else if (after == '=') {
                    return self.lex_other(2, tok.TokenKind.MinusEquals);
                }
                return self.lex_other(1, tok.TokenKind.Minus);
            },

            '<' => {
                if (after == '<') {
                    if (self.get_at(2) == '=') {
                        return self.lex_other(3, tok.TokenKind.LeftShiftEquals);
                    }
                    return self.lex_other(2, tok.TokenKind.LeftShift);
                } else if (after == '=') {
                    return self.lex_other(2, tok.TokenKind.LessEquals);
                }
                return self.lex_other(1, tok.TokenKind.LessThan);
            },

            '>' => {
                if (after == '>') {
                    if (self.get_at(2) == '=') {
                        return self.lex_other(3, tok.TokenKind.RightShiftEquals);
                    }
                    return self.lex_other(2, tok.TokenKind.RightShift);
                } else if (after == '=') {
                    return self.lex_other(2, tok.TokenKind.GreaterEquals);
                }
                return self.lex_other(1, tok.TokenKind.GreaterThan);
            },

            '=' => {
                if (after == '=') {
                    return self.lex_other(2, tok.TokenKind.Equals);
                }
                return self.lex_other(1, tok.TokenKind.Assign);
            },

            '!' => {
                if (after == '=') {
                    return self.lex_other(2, tok.TokenKind.NotEquals);
                }
                return self.lex_other(1, tok.TokenKind.Not);
            },

            '(' => return self.lex_other(1, tok.TokenKind.LeftParenthesis),
            ')' => return self.lex_other(1, tok.TokenKind.RightParenthesis),
            '{' => return self.lex_other(1, tok.TokenKind.LeftBrace),
            '}' => return self.lex_other(1, tok.TokenKind.RightBrace),
            ':' => return self.lex_other(1, tok.TokenKind.Colon),
            ';' => return self.lex_other(1, tok.TokenKind.Semicolon),
            '.' => return self.lex_other(1, tok.TokenKind.Dot),

            else => {},
        }

        const message = try std.fmt.allocPrint(std.heap.page_allocator, "Incorrect character found: `{c}`", .{self.get()});
        const diag = diagnostic.Diagnostic.init(message, Position.init(self.index, 1), diagnostic.DiagnosticKind.Error);
        self.advance();
        try self.diagnostics.append(diag);
        return tok.Token.init(tok.TokenKind.BadToken, Position.init(self.index - 1, 1), null);
    }

    fn get(self: Lexer) u8 {
        return if (self.index >= self.code.text.len) 0 else self.code.text[self.index];
    }

    fn get_at(self: Lexer, offset: usize) u8 {
        const index = self.index + offset;
        return if (index >= self.code.text.len) 0 else self.code.text[index];
    }

    fn advance(self: *Lexer) void {
        self.index += 1;
    }

    fn skip_spaces(self: *Lexer) void {
        var current = self.get();
        while ((current >= 0x09 and current <= 0x0d) or current == 0x20) {
            self.advance();
            current = self.get();
        }
    }

    fn lex_other(self: *Lexer, size: comptime_int, kind: tok.TokenKind) !tok.Token {
        const pos = Position.init(self.index, size);
        const value = try std.heap.page_allocator.alloc(u8, size);
        std.mem.copyBackwards(u8, value, self.code.text[self.index .. self.index + size]);
        self.index += size;

        return tok.Token.init(kind, pos, value);
    }

    fn lex_word(self: *Lexer) !tok.Token {
        const start = self.index;
        var current = self.get();
        while ((current >= 'A' and current <= 'Z') or (current >= 'a' and current <= 'z') or (current >= '0' and current <= '9') or current == '_') {
            self.advance();
            current = self.get();
        }

        const value = try std.heap.page_allocator.alloc(u8, self.index - start);
        std.mem.copyBackwards(u8, value, self.code.text[start..self.index]);
        var kind = keywords.get(value);
        if (kind == null) {
            kind = tok.TokenKind.Identifier;
        }

        return tok.Token.init(kind.?, Position.init(start, value.len), value);
    }

    fn lex_number(self: *Lexer) !tok.Token {
        self.advance();
        return tok.Token.init(tok.TokenKind.BadToken, Position.init(0, 0), null);
    }
};
