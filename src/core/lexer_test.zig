const std = @import("std");
const lx = @import("lexer.zig");
const Diagnostic = @import("../utils/diagnostic.zig").Diagnostic;
const tok = @import("../utils/token.zig");

test "lex keywords and identifiers" {
    try lx.init_keywords();
    const diagnostics = std.ArrayList(Diagnostic).init(std.heap.page_allocator);
    const expects = .{tok.TokenKind.False};

    var lexer = lx.Lexer.init("null", diagnostics);
    var token = try lexer.lex();
    for (expects) |exp| {
        try std.testing.expectEqual(exp, token.kind);
        token = try lexer.lex();
    }

    try std.testing.expectEqual(0, diagnostics.items.len);
}
