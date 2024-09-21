const std = @import("std");
const tok = @import("../utils/token.zig");
const diagnostic = @import("../utils/diagnostic.zig");
const Position = @import("../utils/position.zig").Position;
const SourceText = @import("../utils/source_text.zig").SourceText;

const chars = @import("../utils/chars.zig");

var keywords: std.StringHashMap(tok.TokenKind) = undefined;

pub fn init_keywords() !void {
    keywords = std.StringHashMap(tok.TokenKind).init(std.heap.page_allocator);

    try keywords.put("var", tok.TokenKind.Var);
    try keywords.put("const", tok.TokenKind.Const);
    try keywords.put("func", tok.TokenKind.Func);

    try keywords.put("return", tok.TokenKind.Return);
    try keywords.put("break", tok.TokenKind.Break);
    try keywords.put("continue", tok.TokenKind.Continue);

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
    try keywords.put("throw", tok.TokenKind.Throw);
}

pub fn delete_keywords() void {
    keywords.deinit();
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
            return tok.Token.init(tok.TokenKind.EndOfFile, Position.init(self.code.text.len - 1, 1));
        } else if (current == '/') {
            const after = self.get_at(1);
            if (after == '/' or after == '*') {
                if (try self.skip_comments(after)) |opt_tok| {
                    return opt_tok;
                } else {
                    return try self.lex();
                }
            } else if (after == '=') {
                return self.lex_other(2, tok.TokenKind.DivideEquals);
            }
            return self.lex_other(1, tok.TokenKind.Divide);
        }

        if (chars.is_letter(current) or current == '_') {
            return self.lex_word();
        } else if (chars.is_digit(current)) {
            return try self.lex_number();
        }

        const after = self.get_at(1);
        switch (current) {
            '\n' => return self.lex_other(1, tok.TokenKind.NewLine),

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

            '*' => {
                if (after == '=') {
                    return self.lex_other(2, tok.TokenKind.StarEquals);
                }
                return self.lex_other(1, tok.TokenKind.Star);
            },

            '%' => {
                if (after == '=') {
                    return self.lex_other(2, tok.TokenKind.ModuloEquals);
                }
                return self.lex_other(1, tok.TokenKind.Modulo);
            },

            '^' => {
                if (after == '=') {
                    return self.lex_other(2, tok.TokenKind.HatEquals);
                }
                return self.lex_other(1, tok.TokenKind.Hat);
            },
            '~' => return self.lex_other(1, tok.TokenKind.Tilde),

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

            '&' => {
                if (after == '=') {
                    return self.lex_other(2, tok.TokenKind.AmpersandEquals);
                } else if (after == '&') {
                    return self.lex_other(2, tok.TokenKind.AmpersandAmpersand);
                }

                return self.lex_other(1, tok.TokenKind.Ampersand);
            },

            '|' => {
                if (after == '=') {
                    return self.lex_other(2, tok.TokenKind.PipeEquals);
                } else if (after == '|') {
                    return self.lex_other(2, tok.TokenKind.PipePipe);
                }

                return self.lex_other(1, tok.TokenKind.Pipe);
            },

            '?' => return self.lex_other(1, tok.TokenKind.Question),

            '(' => return self.lex_other(1, tok.TokenKind.LeftParenthesis),
            ')' => return self.lex_other(1, tok.TokenKind.RightParenthesis),
            '{' => return self.lex_other(1, tok.TokenKind.LeftBrace),
            '}' => return self.lex_other(1, tok.TokenKind.RightBrace),
            '[' => return self.lex_other(1, tok.TokenKind.LeftBracket),
            ']' => return self.lex_other(1, tok.TokenKind.RightBracket),
            ':' => return self.lex_other(1, tok.TokenKind.Colon),
            ';' => return self.lex_other(1, tok.TokenKind.Semicolon),
            '.' => return self.lex_other(1, tok.TokenKind.Dot),
            ',' => return self.lex_other(1, tok.TokenKind.Comma),

            '#' => return self.lex_macro(),
            '$' => return try self.lex_formatted_string(),
            '\'' => return self.lex_character(),
            '"' => return self.lex_string(),
            '`' => return self.lex_raw_string(),

            else => {},
        }

        const message = try std.fmt.allocPrint(std.heap.page_allocator, "Unexpected character `{c}`", .{self.get()});
        const diag = diagnostic.Diagnostic.init(message, Position.init(self.index, 1), diagnostic.DiagnosticKind.Error);
        self.advance();
        try self.diagnostics.append(diag);
        return tok.Token.init(tok.TokenKind.BadToken, Position.init(self.index - 1, 1));
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
        while ((current >= 0x0b and current <= 0x0d) or current == 0x09 or current == ' ') {
            self.advance();
            current = self.get();
        }
    }

    fn skip_comments(self: *Lexer, kind: u8) !?tok.Token {
        if (kind == '/') {
            var current = self.get();
            while (current != '\n' and current != 0) {
                self.advance();
                current = self.get();
            }

            self.advance();
            return tok.Token.init(tok.TokenKind.NewLine, Position.init(self.index - 1, 1));
        } else {
            var current = self.get();
            var new_line_pos: ?Position = null;
            while (current != 0) {
                if (current == '*') {
                    self.advance();
                    current = self.get();
                    if (current == '/') {
                        break;
                    }
                } else if (current == '\n') {
                    new_line_pos = Position.init(self.index, 1);
                }

                self.advance();
                current = self.get();
            }

            if (current == 0) {
                const diag = diagnostic.Diagnostic.init("Unterminated multi-line comment", Position.init(self.index - 1, 1), diagnostic.DiagnosticKind.Error);
                try self.diagnostics.append(diag);
            }

            self.advance();
            return if (new_line_pos != null)
                tok.Token.init(tok.TokenKind.NewLine, new_line_pos.?)
            else
                null;
        }
    }

    fn lex_other(self: *Lexer, size: comptime_int, kind: tok.TokenKind) !tok.Token {
        const pos = Position.init(self.index, size);
        self.index += size;
        return tok.Token.init(kind, pos);
    }

    fn lex_word(self: *Lexer) tok.Token {
        const start = self.index;
        var current = self.get();
        while (chars.is_letter(current) or chars.is_digit(current) or current == '_') {
            self.advance();
            current = self.get();
        }

        var kind = keywords.get(self.code.text[start..self.index]);
        if (kind == null) {
            kind = tok.TokenKind.Identifier;
        }

        return tok.Token.init(kind.?, Position.init(start, self.index - start));
    }

    fn lex_number(self: *Lexer) !tok.Token {
        var start = self.index;
        var mode: u8 = 'i';
        var current = self.get();
        if (current == '0') {
            self.advance();
            current = self.get();
            if (current == 'b' or current == 'o' or current == 'x') {
                start += 2;
                mode = current;
                self.advance();
                current = self.get();
            }
        }

        var has_exp = false;
        while (chars.is_digit(current) or chars.is_hexa_letter(current) or current == '_' or current == '.') {
            if (current == '_') {} else if (current == '.') {
                if (mode != 'i') {
                    return self.lex_incorrect_number(start, "Could not add `.` after a non-integer number");
                }

                mode = 'd';
            } else if (mode != 'x' and (current == 'e' or current == 'E')) {
                if (mode != 'i' and mode != 'd') {
                    return self.lex_incorrect_number(start, "Could not add exponential to binary, octal or hexadecimal number");
                } else if (has_exp) {
                    return self.lex_incorrect_number(start, "Could not add exponential twice in a number");
                }

                mode = 'd';
                has_exp = true;
                self.advance();
                current = self.get();
                if (current != '-' and current != '+') {
                    return self.lex_incorrect_number(start, "Exponential `e` or `E` need a sign after it to be declared");
                }
            } else if (mode == 'b' and current != '0' and current != '1') {
                return self.lex_incorrect_number(start, "Could not use other digits than 0 or 1 for binary number");
            } else if (mode == 'o' and (current < '0' or current > '7')) {
                return self.lex_incorrect_number(start, "Could not use other digits than 0 to 7 for octal number");
            } else if (mode != 'x' and chars.is_hexa_letter(current)) {
                return self.lex_incorrect_number(start, "Could not use letter for non-hexadecimal number");
            }

            self.advance();
            current = self.get();
        }

        const number_position = Position.init(start, self.index - start);
        if (current == 'i') {
            self.advance();
            if (mode == 'd') {
                const err_pos = Position.init(self.index - 1, 1);
                const diag = diagnostic.Diagnostic.init("Number modifier `i` could not be used with decimal number", err_pos, diagnostic.DiagnosticKind.Error);
                try self.diagnostics.append(diag);

                return tok.Token.init(tok.TokenKind.BadToken, err_pos);
            }
            mode = 'I';
        } else if (current == 'l') {
            self.advance();
            mode = 'D';
        }

        return switch (mode) {
            'd' => tok.Token.init(tok.TokenKind.Decimal, number_position),
            'b' => tok.Token.init(tok.TokenKind.BinaryInteger, number_position),
            'o' => tok.Token.init(tok.TokenKind.OctalInteger, number_position),
            'x' => tok.Token.init(tok.TokenKind.HexaInteger, number_position),
            'I' => tok.Token.init(tok.TokenKind.BigInteger, number_position),
            'D' => tok.Token.init(tok.TokenKind.BigDecimal, number_position),
            else => tok.Token.init(tok.TokenKind.Integer, number_position),
        };
    }

    fn lex_incorrect_number(self: *Lexer, start: usize, err_mess: []const u8) !tok.Token {
        var current = self.get();
        while (chars.is_digit(current) or chars.is_hexa_letter(current) or current == '_' or current == '.') {
            if (current == 'e' or current == 'E') {
                const after = self.get_at(1);
                if (after == '-' or after == '+') {
                    self.advance();
                }
            }

            self.advance();
            current = self.get();
        }

        if (current == 'i' or current == 'd') {
            self.advance();
        }

        const err_pos = Position.init(start, self.index - start);
        const diag = diagnostic.Diagnostic.init(err_mess, err_pos, diagnostic.DiagnosticKind.Error);
        try self.diagnostics.append(diag);

        return tok.Token.init(tok.TokenKind.BadToken, err_pos);
    }

    fn lex_special_character(self: *Lexer) !void {
        self.advance();
        var current = self.get();
        switch (current) {
            'b' => {
                self.advance();
                current = self.get();
                if (current != '0' and current != '1') {
                    const diag = diagnostic.Diagnostic.init("`\\b` needs a binary representation of a character after it", Position.init(self.index - 2, 2), diagnostic.DiagnosticKind.Error);
                    try self.diagnostics.append(diag);
                }

                for (0..32) |_| {
                    if (current != '0' and current != '1') {
                        break;
                    }

                    self.advance();
                    current = self.get();
                }
            },

            'o' => {
                self.advance();
                current = self.get();
                if (current < '0' or current > '7') {
                    const diag = diagnostic.Diagnostic.init("`\\o` needs an octal representation of a character after it", Position.init(self.index - 2, 2), diagnostic.DiagnosticKind.Error);
                    try self.diagnostics.append(diag);
                }

                const limit: usize = if (current <= '3') 11 else 10;
                for (0..limit) |_| {
                    if (current < '0' or current > '7') {
                        break;
                    }

                    self.advance();
                    current = self.get();
                }
            },

            'x', 'u' => {
                self.advance();
                current = self.get();
                if (!chars.is_hexa_letter(current) and !chars.is_digit(current)) {
                    const diag = diagnostic.Diagnostic.init("`\\x` and `\\u` need a hexadecimal representation of a character after it", Position.init(self.index - 2, 2), diagnostic.DiagnosticKind.Error);
                    try self.diagnostics.append(diag);
                }

                for (0..8) |_| {
                    if (!chars.is_hexa_letter(current) and !chars.is_digit(current)) {
                        break;
                    }

                    self.advance();
                    current = self.get();
                }
            },

            else => self.advance(),
        }
    }

    fn lex_character(self: *Lexer) !tok.Token {
        const start = self.index;
        self.advance();
        if (self.get() == '\\') {
            try self.lex_special_character();
        } else {
            for (0..chars.get_bytes_count(self.get())) |_| {
                self.advance();
            }
        }

        if (self.get() != '\'') {
            const err_pos = Position.init(start, self.index - start);
            const diag = diagnostic.Diagnostic.init("Unterminated character literal", err_pos, diagnostic.DiagnosticKind.Error);
            try self.diagnostics.append(diag);

            return tok.Token.init(tok.TokenKind.BadToken, err_pos);
        }

        self.advance();
        return tok.Token.init(tok.TokenKind.Character, Position.init(start, self.index - start));
    }

    fn lex_string(self: *Lexer) !tok.Token {
        const start = self.index;
        self.advance();
        var current = self.get();
        while (current != 0 and current != '"') {
            if (current == '\\') {
                try self.lex_special_character();
                current = self.get();
            } else {
                self.advance();
                current = self.get();
            }
        }

        if (current != '"') {
            const err_pos = Position.init(start, self.index - start);
            const diag = diagnostic.Diagnostic.init("Unterminated string literal", err_pos, diagnostic.DiagnosticKind.Error);
            try self.diagnostics.append(diag);

            return tok.Token.init(tok.TokenKind.BadToken, err_pos);
        }

        self.advance();
        return tok.Token.init(tok.TokenKind.String, Position.init(start, self.index - start));
    }

    fn lex_raw_string(self: *Lexer) !tok.Token {
        const start = self.index;
        self.advance();
        var current = self.get();
        while (current != 0 and current != '`') {
            self.advance();
            current = self.get();
        }

        if (current != '`') {
            const err_pos = Position.init(start, self.index - start);
            const diag = diagnostic.Diagnostic.init("Unterminated raw-string literal", err_pos, diagnostic.DiagnosticKind.Error);
            try self.diagnostics.append(diag);

            return tok.Token.init(tok.TokenKind.BadToken, err_pos);
        }

        self.advance();
        return tok.Token.init(tok.TokenKind.RawString, Position.init(start, self.index - start));
    }

    fn lex_formatted_string(self: *Lexer) !tok.Token {
        const start = self.index;
        self.advance();
        var token: tok.Token = undefined;
        const current = self.get();
        if (current == '"') {
            token = try self.lex_string();
        } else if (current == '`') {
            token = try self.lex_raw_string();
        } else {
            const err_pos = Position.init(start, self.index - start);
            const diag = diagnostic.Diagnostic.init("`$` could only be followed by a string declaration to format it", err_pos, diagnostic.DiagnosticKind.Error);
            try self.diagnostics.append(diag);
            return tok.Token.init(tok.TokenKind.BadToken, err_pos);
        }

        return tok.Token.init(tok.TokenKind.FormattedString, token.position);
    }

    fn lex_macro(self: *Lexer) tok.Token {
        const start = self.index;
        self.advance();
        var current = self.get();
        while (chars.is_digit_or_letter(current) or current == '_') {
            self.advance();
            current = self.get();
        }

        return tok.Token.init(tok.TokenKind.Macro, Position.init(start, self.index - start));
    }
};
