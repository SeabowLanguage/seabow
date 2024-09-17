const std = @import("std");
const allocator = std.heap.page_allocator;

const Diagnostic = @import("utils/diagnostic.zig").Diagnostic;
const lx = @import("core/lexer.zig");
const tk = @import("utils/token.zig");
const SourceText = @import("utils/source_text.zig").SourceText;

pub fn main() !void {
    try lx.init_keywords();
    var args = try std.process.ArgIterator.initWithAllocator(allocator);
    defer args.deinit();

    var source_text: SourceText = undefined;
    _ = args.skip();
    const test_file_path = args.next();
    if (test_file_path != null) {
        var test_file = try std.fs.cwd().openFile(test_file_path.?, .{});
        defer test_file.close();

        const file_contents = try test_file.readToEndAlloc(std.heap.page_allocator, std.math.maxInt(usize));
        source_text = SourceText.init(file_contents);
    } else {
        source_text = SourceText.init("");
    }

    var diagnostics = std.ArrayList(Diagnostic).init(std.heap.page_allocator);
    var lexer = lx.Lexer.init(source_text, &diagnostics);
    var tokens = std.ArrayList(tk.Token).init(std.heap.page_allocator);
    var tok = try lexer.lex();
    while (tok.kind != tk.TokenKind.EndOfFile) {
        try tokens.append(tok);
        tok = try lexer.lex();
    }

    if (diagnostics.items.len == 0) {
        for (tokens.items) |token| {
            std.debug.print("{}\n", .{token});
        }
    } else {
        for (diagnostics.items) |diag| {
            diag.display(source_text);
        }
        std.process.exit(1);
    }
}
