const std = @import("std");
const SourceText = @import("../utils/source_text.zig").SourceText;
const Lexer = @import("lexer.zig").Lexer;
const nd = @import("../nodes/node.zig");
const diagnostic = @import("../utils/diagnostic.zig");
const tok = @import("../utils/token.zig");
const Position = @import("../utils/position.zig").Position;

const MODIFIER_NONE: u8 = 0b0000_0000;
const MODIFIER_FULL_STAT: u8 = 0b0000_0001;
const MODIFIER_FUNC_SCOPE: u8 = 0b0000_0010;
const MODIFIER_LOOP_SCOPE: u8 = 0b0000_0100;

pub const Parser = struct {
    diagnostics: *std.ArrayList(diagnostic.Diagnostic),
    tokens: []tok.Token,
    index: usize,

    pub fn init(source: SourceText, diags: *std.ArrayList(diagnostic.Diagnostic)) !Parser {
        var lexer = Lexer.init(source, diags);
        var tokens = std.ArrayList(tok.Token).init(std.heap.page_allocator);
        errdefer tokens.deinit();
        var actual = try lexer.lex();
        while (actual.kind != tok.TokenKind.EndOfFile) {
            try tokens.append(actual);
            actual = try lexer.lex();
        }

        try tokens.append(actual);
        return Parser{ .diagnostics = diags, .tokens = try tokens.toOwnedSlice(), .index = 0 };
    }

    pub fn parse(self: *Parser) !nd.NodeCompound {
        var nodes = std.ArrayList(*nd.Node).init(std.heap.page_allocator);
        errdefer nodes.deinit();

        var actual = try self.parse_statement(MODIFIER_FULL_STAT);
        while (actual != nd.Node.Eof) {
            if (actual != nd.Node.NoOp) {
                try nodes.append(&actual);
            }

            actual = try self.parse_statement(MODIFIER_FULL_STAT);
        }

        return nd.NodeCompound.init(try nodes.toOwnedSlice());
    }

    fn skip_new_lines(self: *Parser) void {
        var current = self.get().kind;
        while (current == tok.TokenKind.NewLine) {
            self.skip();
            current = self.get().kind;
        }
    }

    fn advance(self: *Parser) tok.Token {
        const token = self.get();
        self.index += 1;
        return token;
    }

    fn skip(self: *Parser) void {
        self.index += 1;
    }

    fn get(self: *Parser) tok.Token {
        const index = if (self.index < self.tokens.len) self.index else self.tokens.len - 1;
        return self.tokens[index];
    }

    fn get_at(self: *Parser, offset: usize) tok.Token {
        const index = self.index + offset;
        return if (index < self.tokens.len) self.tokens[index] else self.tokens[self.tokens.len - 1];
    }

    fn match(self: *Parser, expected: tok.TokenKind, text: []const u8) !void {
        self.skip_new_lines();
        const current = self.advance();
        if (current.kind != expected and current.kind != tok.TokenKind.BadToken) {
            const diag = diagnostic.Diagnostic.init(text, current.position, diagnostic.DiagnosticKind.Error);
            try self.diagnostics.append(diag);
        }
    }

    fn parse_statement(self: *Parser, mod: u8) anyerror!nd.Node {
        self.skip_new_lines();
        const current = self.get();
        var node: ?nd.Node = null;
        switch (current.kind) {
            tok.TokenKind.LeftBrace => return try self.parse_compound(mod),
            else => {},
        }

        if (node == null) {
            node = try self.parse_controller(mod);
        }

        if (node == null) {
            node = try self.parse_binary_unary_expression(0);
        }

        return node.?;
    }

    fn parse_compound(self: *Parser, mod: u8) !nd.Node {
        self.skip();
        var nodes = std.ArrayList(*const nd.Node).init(std.heap.page_allocator);
        errdefer nodes.deinit();

        const new_mod = if (mod & MODIFIER_FULL_STAT != 0) mod else mod | MODIFIER_FULL_STAT;
        var current = self.get();
        while (current.kind != tok.TokenKind.RightBrace and current.kind != tok.TokenKind.EndOfFile) {
            var stat = try self.parse_statement(new_mod);
            if (stat != nd.Node.NoOp) {
                try nodes.append(&stat);
            }

            current = self.get();
        }

        try self.match(tok.TokenKind.RightBrace, "Expected `}` to end the previous block opened with `{`");
        return nd.Node{ .Compound = nd.NodeCompound{ .nodes = try nodes.toOwnedSlice() } };
    }

    fn parse_controller(self: *Parser, mod: u8) !?nd.Node {
        const current = self.get();
        if (mod & MODIFIER_FUNC_SCOPE != 0 and current.kind == tok.TokenKind.Return) {
            var expr: ?*const nd.Node = null;
            const after_kind = self.get_at(1).kind;
            if (after_kind != tok.TokenKind.NewLine and after_kind != tok.TokenKind.Semicolon) {
                const expression = try self.parse_statement(MODIFIER_NONE);
                expr = &expression;
            }

            return nd.Node{ .Return = nd.NodeReturn.init(expr) };
        } else if (mod & MODIFIER_LOOP_SCOPE != 0) {
            if (current.kind == tok.TokenKind.Break) {
                return nd.Node{ .Break = nd.NodeBreak{} };
            } else if (current.kind == tok.TokenKind.Continue) {
                return nd.Node{ .Continue = nd.NodeContinue{} };
            }
        }

        return null;
    }

    fn parse_binary_unary_expression(self: *Parser, precedence: u8) anyerror!?nd.Node {
        var left: nd.Node = undefined;
        const unary_precedence = self.get().unary_precedence();
        if (unary_precedence != 0 and unary_precedence >= precedence) {
            if (unary_precedence == 2) {
                left = try self.parse_question_operation();
            } else {
                const current = self.advance();
                const operand = try self.parse_binary_unary_expression(unary_precedence);
                left = nd.Node{ .Unary = nd.NodeUnary.init(current, &operand.?) };
            }
        } else {
            left = try self.parse_primary_expression();
        }

        while (true) {
            const current = self.get();
            const binary_precedence = current.binary_precedence();
            if (binary_precedence == 0 or binary_precedence <= precedence) {
                break;
            }

            self.skip();
            if (binary_precedence == 17) {
                return nd.Node{ .Binary = nd.NodeBinary.init(&left, current, null) };
            }

            const right = try self.parse_binary_unary_expression(binary_precedence);
            left = nd.Node{ .Binary = nd.NodeBinary.init(&left, current, &right.?) };
        }

        return left;
    }

    fn parse_question_operation(self: *Parser) !nd.Node {
        self.skip();
        const condition = try self.parse_statement(MODIFIER_NONE);
        try self.match(tok.TokenKind.Colon, "Expected `:` after the condition of a question (?) operation");

        const first = try self.parse_statement(MODIFIER_NONE);
        try self.match(tok.TokenKind.Colon, "Expected `:` after the 'true-value' of a question (?) operation");

        const second = try self.parse_statement(MODIFIER_NONE);
        return nd.Node{ .Question = nd.NodeQuestion.init(&condition, &first, &second) };
    }

    fn parse_primary_expression(self: *Parser) !nd.Node {
        const current = self.advance();
        return switch (current.kind) {
            tok.TokenKind.EndOfFile => nd.Node{ .Eof = nd.NodeEndOfFile{} },
            tok.TokenKind.BadToken => nd.Node{ .NoOp = nd.NodeNoOp{} },

            else => {
                const diag = diagnostic.Diagnostic.init("Incorrect statement found", current.position, diagnostic.DiagnosticKind.Error);
                try self.diagnostics.append(diag);
                return nd.Node{ .NoOp = nd.NodeNoOp{} };
            },
        };
    }
};
