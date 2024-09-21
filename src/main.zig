const std = @import("std");
const allocator = std.heap.page_allocator;

const Diagnostic = @import("utils/diagnostic.zig").Diagnostic;
const Parser = @import("core/parser.zig").Parser;
const lx = @import("core/lexer.zig");
const SourceText = @import("utils/source_text.zig").SourceText;

pub fn main() !u8 {
    try lx.init_keywords();
    defer lx.delete_keywords();
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
    defer diagnostics.deinit();

    var parser = try Parser.init(source_text, &diagnostics);
    const root = try parser.parse();

    var return_value: u8 = 0;
    if (diagnostics.items.len == 0) {
        for (parser.tokens) |token| {
            token.display(source_text);
        }
        root.display(0);
    } else {
        for (diagnostics.items) |diag| {
            diag.display(source_text);
        }

        return_value = 1;
    }

    return return_value;
}
