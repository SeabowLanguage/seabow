const std = @import("std");
const chars = @import("../utils/chars.zig");
const SourceText = @import("../utils/source_text.zig").SourceText;
const Lexer = @import("lexer.zig").Lexer;
const nd = @import("../nodes/node.zig");
const diagnostic = @import("../utils/diagnostic.zig");
const tok = @import("../utils/token.zig");
const value = @import("../values/value.zig");
const Position = @import("../utils/position.zig").Position;

const MODIFIER_NONE: u8 = 0b0000_0000;
const MODIFIER_FULL_STAT: u8 = 0b0000_0001;
const MODIFIER_FUNC_SCOPE: u8 = 0b0000_0010;
const MODIFIER_LOOP_SCOPE: u8 = 0b0000_0100;

pub const Parser = struct {
    diagnostics: *std.ArrayList(diagnostic.Diagnostic),
    tokens: []tok.Token,
    index: usize,
    source: SourceText,

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
        return Parser{ .diagnostics = diags, .tokens = try tokens.toOwnedSlice(), .index = 0, .source = source };
    }

    pub fn parse(self: *Parser) !*nd.Node {
        var nodes = std.ArrayList(*nd.Node).init(std.heap.page_allocator);
        errdefer nodes.deinit();

        while (true) {
            const actual = try self.parse_statement(MODIFIER_FULL_STAT);
            if (actual.* == nd.Node.Eof) {
                actual.destroy();
                break;
            }

            try nodes.append(actual);
        }

        const ret_node = nd.Node{ .Compound = nd.NodeCompound.init(try nodes.toOwnedSlice()) };
        return try ret_node.copy();
    }

    fn nothing() !*nd.Node {
        const noop = nd.Node{ .NoOp = nd.NodeNoOp{} };
        return try noop.copy();
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
        const current = self.advance();
        if (current.kind != expected and current.kind != tok.TokenKind.BadToken) {
            const diag = diagnostic.Diagnostic.init(text, current.position, diagnostic.DiagnosticKind.Error);
            try self.diagnostics.append(diag);
        }
    }

    fn parse_statement(self: *Parser, mod: u8) anyerror!*nd.Node {
        self.skip_new_lines();
        const current = self.get();
        var node: ?nd.Node = null;
        switch (current.kind) {
            tok.TokenKind.LeftBrace => return try self.parse_compound(mod),
            tok.TokenKind.Var => return try self.parse_var_const_decl(false),
            tok.TokenKind.Const => return try self.parse_var_const_decl(true),

            else => {},
        }

        if (node == null) {
            const contr_node = try self.parse_controller(mod);
            if (contr_node) |contr| {
                node = contr.*;
            }
        }

        if (node == null) {
            const bin_un_node = try self.parse_binary_unary_expression(0);
            node = bin_un_node.*;
        }

        if (mod & MODIFIER_FULL_STAT != 0 and node.? != nd.Node.NoOp and node.? != nd.Node.Eof) {
            const actual_current = self.get();
            if (actual_current.kind != tok.TokenKind.NewLine and actual_current.kind != tok.TokenKind.Semicolon and actual_current.kind != tok.TokenKind.EndOfFile) {
                const diag = diagnostic.Diagnostic.init("Needs new-line or semi-colon between two statements", current.position, diagnostic.DiagnosticKind.Error);
                try self.diagnostics.append(diag);
            }

            self.skip();
        }

        return try node.?.copy();
    }

    fn parse_compound(self: *Parser, mod: u8) !*nd.Node {
        self.skip();
        var nodes = std.ArrayList(*nd.Node).init(std.heap.page_allocator);
        errdefer nodes.deinit();

        const new_mod = if (mod & MODIFIER_FULL_STAT != 0) mod else mod | MODIFIER_FULL_STAT;
        var current = self.get();
        while (current.kind != tok.TokenKind.RightBrace and current.kind != tok.TokenKind.EndOfFile) {
            const stat = try self.parse_statement(new_mod);
            if (stat.* != nd.Node.NoOp) {
                try nodes.append(stat);
            }

            self.skip_new_lines();
            current = self.get();
        }

        try self.match(tok.TokenKind.RightBrace, "Expecting `}` to end the previous block opened with `{`");
        const ret_node = nd.Node{ .Compound = nd.NodeCompound{ .nodes = try nodes.toOwnedSlice() } };
        return try ret_node.copy();
    }

    fn parse_controller(self: *Parser, mod: u8) !?*nd.Node {
        const current = self.get();
        var ret_node: ?nd.Node = null;

        if (mod & MODIFIER_FUNC_SCOPE != 0 and current.kind == tok.TokenKind.Return) {
            var expr: ?*nd.Node = null;
            const after_kind = self.get_at(1).kind;
            if (after_kind != tok.TokenKind.NewLine and after_kind != tok.TokenKind.Semicolon) {
                expr = try self.parse_statement(MODIFIER_NONE);
            }

            ret_node = nd.Node{ .Return = nd.NodeReturn.init(expr) };
        } else if (mod & MODIFIER_LOOP_SCOPE != 0) {
            if (current.kind == tok.TokenKind.Break) {
                ret_node = nd.Node{ .Break = nd.NodeBreak{} };
            } else if (current.kind == tok.TokenKind.Continue) {
                ret_node = nd.Node{ .Continue = nd.NodeContinue{} };
            }
        }

        return if (ret_node != null) try ret_node.?.copy() else null;
    }

    fn parse_binary_unary_expression(self: *Parser, precedence: u8) anyerror!*nd.Node {
        var left: nd.Node = undefined;
        const unary_precedence = self.get().unary_precedence();
        if (unary_precedence != 0 and unary_precedence >= precedence) {
            if (unary_precedence == 2) {
                const new_left = try self.parse_question_operation();
                left = new_left.*;
            } else {
                const current = self.advance();
                const operand = try self.parse_binary_unary_expression(unary_precedence);
                left = nd.Node{ .Unary = nd.NodeUnary.init(current, operand) };
            }
        } else {
            const new_left = try self.parse_primary_expression();
            left = new_left.*;
        }

        while (true) {
            const current = self.get();
            const binary_precedence = current.binary_precedence();
            if (binary_precedence == 0 or binary_precedence <= precedence) {
                break;
            }

            self.skip();
            if (binary_precedence == 17) {
                left = nd.Node{ .Binary = nd.NodeBinary.init(&left, current, null) };
                return try left.copy();
            }

            const right = try self.parse_binary_unary_expression(binary_precedence);
            const left_ptr = try left.copy();
            left = nd.Node{ .Binary = nd.NodeBinary.init(left_ptr, current, right) };
        }

        return try left.copy();
    }

    fn parse_question_operation(self: *Parser) !*nd.Node {
        self.skip();
        const condition = try self.parse_statement(MODIFIER_NONE);
        try self.match(tok.TokenKind.Colon, "Expecting `:` after the condition of a question (?) operation");

        const first = try self.parse_statement(MODIFIER_NONE);
        try self.match(tok.TokenKind.Colon, "Expecting `:` after the 'true-value' of a question (?) operation");

        const second = try self.parse_statement(MODIFIER_NONE);
        const ret_node = nd.Node{ .Question = nd.NodeQuestion.init(condition, first, second) };
        return try ret_node.copy();
    }

    fn parse_primary_expression(self: *Parser) !*nd.Node {
        const current = self.advance();
        return switch (current.kind) {
            tok.TokenKind.EndOfFile => {
                const eof = nd.Node{ .Eof = nd.NodeEndOfFile{} };
                return try eof.copy();
            },

            tok.TokenKind.Null => {
                const lit_value = value.Value.init(value.ValueElement{ .Null = value.ValueNull{} }, value.MODIFIER_NONE);
                const lit_node = nd.Node{ .Literal = nd.NodeLiteral.init(lit_value, current.position) };
                return try lit_node.copy();
            },

            tok.TokenKind.BadToken => {
                return try Parser.nothing();
            },

            tok.TokenKind.Integer => {
                const int_value = chars.decode_integer(self.source.text[current.position.start..current.position.end()], 10);
                const lit_value = value.Value.init(value.ValueElement{ .Ulong = value.ValueUlong.init(int_value) }, value.MODIFIER_NONE);
                const lit_node = nd.Node{ .Literal = nd.NodeLiteral.init(lit_value, current.position) };
                return try lit_node.copy();
            },

            tok.TokenKind.Decimal => {
                const flt_value = chars.decode_decimal(self.source.text[current.position.start..current.position.end()]);
                const lit_value = value.Value.init(value.ValueElement{ .Double = value.ValueDouble.init(flt_value) }, value.MODIFIER_NONE);
                const lit_node = nd.Node{ .Literal = nd.NodeLiteral.init(lit_value, current.position) };
                return try lit_node.copy();
            },

            tok.TokenKind.BinaryInteger => {
                const bin_value = chars.decode_integer(self.source.text[current.position.start..current.position.end()], 2);
                const lit_value = value.Value.init(value.ValueElement{ .Ulong = value.ValueUlong.init(bin_value) }, value.MODIFIER_NONE);
                const lit_node = nd.Node{ .Literal = nd.NodeLiteral.init(lit_value, current.position) };
                return try lit_node.copy();
            },

            tok.TokenKind.OctalInteger => {
                const oct_value = chars.decode_integer(self.source.text[current.position.start..current.position.end()], 8);
                const lit_value = value.Value.init(value.ValueElement{ .Ulong = value.ValueUlong.init(oct_value) }, value.MODIFIER_NONE);
                const lit_node = nd.Node{ .Literal = nd.NodeLiteral.init(lit_value, current.position) };
                return try lit_node.copy();
            },

            tok.TokenKind.HexaInteger => {
                const hexa_value = chars.decode_integer(self.source.text[current.position.start..current.position.end()], 16);
                const lit_value = value.Value.init(value.ValueElement{ .Ulong = value.ValueUlong.init(hexa_value) }, value.MODIFIER_NONE);
                const lit_node = nd.Node{ .Literal = nd.NodeLiteral.init(lit_value, current.position) };
                return try lit_node.copy();
            },

            tok.TokenKind.String => {
                const str = self.source.text[current.position.start + 1 .. current.position.end() - 1];
                const lit_value = value.Value.init(value.ValueElement{ .String = value.ValueString.init(str) }, value.MODIFIER_NONE);
                const lit_node = nd.Node{ .Literal = nd.NodeLiteral.init(lit_value, current.position) };
                return try lit_node.copy();
            },

            tok.TokenKind.False => {
                const lit_value = value.Value.init(value.ValueElement{ .Bool = value.ValueBool.init(false) }, value.MODIFIER_NONE);
                const lit_node = nd.Node{ .Literal = nd.NodeLiteral.init(lit_value, current.position) };
                return try lit_node.copy();
            },

            tok.TokenKind.True => {
                const lit_value = value.Value.init(value.ValueElement{ .Bool = value.ValueBool.init(true) }, value.MODIFIER_NONE);
                const lit_node = nd.Node{ .Literal = nd.NodeLiteral.init(lit_value, current.position) };
                return try lit_node.copy();
            },

            tok.TokenKind.Identifier => {
                return try self.parse_access(current);
            },

            else => {
                const diag = diagnostic.Diagnostic.init("Incorrect statement found", current.position, diagnostic.DiagnosticKind.Error);
                try self.diagnostics.append(diag);
                return try Parser.nothing();
            },
        };
    }

    fn parse_access(self: *Parser, current: tok.Token) !*nd.Node {
        const actual = self.get();
        if (actual.kind == tok.TokenKind.Dot) {
            // TODO
        } else if (actual.kind == tok.TokenKind.LeftParenthesis) {
            // TODO
        }

        const ret = nd.Node{ .VarConstAccess = nd.NodeVarConstAccess.init(self.source.text[current.position.start..current.position.end()], current.position) };
        return try ret.copy();
    }

    fn parse_type_declaration(self: *Parser) !nd.NodeType {
        const start = self.advance();
        if (start.kind != tok.TokenKind.Identifier) {
            const diag = diagnostic.Diagnostic.init("Seabow types should beggin with an identifier", start.position, diagnostic.DiagnosticKind.Error);
            try self.diagnostics.append(diag);
        }

        var details: u2 = value.vt.TYPE_DETAIL_NONE;
        var current = self.get();
        while (current.kind == tok.TokenKind.Not or current.kind == tok.TokenKind.Ampersand) {
            if (current.kind == tok.TokenKind.Not) {
                if (details & value.vt.TYPE_DETAIL_NOT_NULL != 0) {
                    const diag = diagnostic.Diagnostic.init("Seabow types can only contain 1 `!` to be marked as not-null", current.position, diagnostic.DiagnosticKind.Error);
                    try self.diagnostics.append(diag);
                }
                details |= value.vt.TYPE_DETAIL_NOT_NULL;
            } else {
                if (details & value.vt.TYPE_DETAIL_REF != 0) {
                    const diag = diagnostic.Diagnostic.init("Seabow types can only contain 1 `&` to be marked as a reference", current.position, diagnostic.DiagnosticKind.Error);
                    try self.diagnostics.append(diag);
                }
                details |= value.vt.TYPE_DETAIL_REF;
            }

            self.skip();
            current = self.get();
        }

        var contained: ?std.ArrayList(*nd.NodeType) = null;
        if (current.kind == tok.TokenKind.LessThan) {
            contained = std.ArrayList(*nd.NodeType).init(std.heap.page_allocator);
            self.skip();
            while (self.get().kind == tok.TokenKind.Identifier) {
                const other = try self.parse_type_declaration();
                try contained.?.append(try other.copy());
                if (self.get().kind == tok.TokenKind.Comma) {
                    self.skip();
                }
            }

            try self.match(tok.TokenKind.GreaterThan, "Expecting `>` to close a type who contained other sub-types");
        }

        var final_contained: ?[]*nd.NodeType = null;
        if (contained != null) {
            final_contained = try contained.?.toOwnedSlice();
        }

        return nd.NodeType.init(Position.init(start.position.start, self.get().position.end() - start.position.start), self.source.text[start.position.start..start.position.end()], details, final_contained);
    }

    fn parse_var_const_decl(self: *Parser, constant: bool) !*nd.Node {
        const start = self.advance();
        var declarations = std.ArrayList(nd.NodeVarConstDecl).init(std.heap.page_allocator);

        while (self.get().kind == tok.TokenKind.Identifier) {
            const begin = self.advance();
            if (begin.kind != tok.TokenKind.Identifier) {
                const diag = diagnostic.Diagnostic.init("Variable or constant declaration needs an identifier", begin.position, diagnostic.DiagnosticKind.Error);
                try self.diagnostics.append(diag);
            }

            var decl_type: ?nd.NodeType = null;
            var expr: ?*nd.Node = null;

            if (self.get().kind == tok.TokenKind.Colon) {
                self.skip();
                decl_type = try self.parse_type_declaration();
            }

            if (self.get().kind == tok.TokenKind.Assign) {
                self.skip();
                expr = try self.parse_statement(MODIFIER_NONE);
                if (expr.?.* == nd.Node.Eof) {
                    const diag = diagnostic.Diagnostic.init("`=` after declaration need a correct expression", begin.position, diagnostic.DiagnosticKind.Error);
                    try self.diagnostics.append(diag);
                }
            }

            if (decl_type == null and expr == null) {
                const diag = diagnostic.Diagnostic.init("Variable or constant declaration needs a type or an assignated expression", begin.position, diagnostic.DiagnosticKind.Error);
                try self.diagnostics.append(diag);
            }

            try declarations.append(nd.NodeVarConstDecl.init(self.source.text[begin.position.start..begin.position.end()], begin.position, decl_type, expr));

            const actual = self.get();
            if (actual.kind == tok.TokenKind.Comma) {
                self.skip();
            } else if (actual.kind == tok.TokenKind.Identifier) {
                const diag = diagnostic.Diagnostic.init("Multiple variable or constant declarations need a `,` separator", actual.position, diagnostic.DiagnosticKind.Error);
                try self.diagnostics.append(diag);
            }
        }

        if (declarations.items.len == 0) {
            const diag = diagnostic.Diagnostic.init("`var` or `const` keyword need at least one variable or constant declaration", start.position, diagnostic.DiagnosticKind.Error);
            try self.diagnostics.append(diag);
        }

        const ret = nd.Node{ .VarConstDeclList = nd.NodeVarConstDeclList.init(try declarations.toOwnedSlice(), constant) };
        return try ret.copy();
    }
};
