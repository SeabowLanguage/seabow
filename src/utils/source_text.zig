const std = @import("std");
const Position = @import("position.zig").Position;

pub const SourceText = struct {
    text: []const u8,
    lines: []Position,

    pub fn init(text: []const u8) SourceText {
        if (init_lines(text)) |text_lines| {
            return SourceText{ .text = text, .lines = text_lines };
        } else |_| {
            return SourceText{ .text = text, .lines = &[0]Position{} };
        }
    }

    pub fn get_line(self: SourceText, pos: usize) ?Position {
        for (self.lines) |line| {
            if (line.start <= pos and line.end() >= pos) {
                return line;
            }
        }

        return null;
    }

    fn init_lines(text: []const u8) ![]Position {
        var text_lines = std.ArrayList(Position).init(std.heap.page_allocator);
        var start: usize = 0;
        for (0..text.len) |i| {
            if (text[i] == '\n' or text[i] == 0) {
                try text_lines.append(Position.init(start, i - start));
                start = i + 1;
            }
        }

        try text_lines.append(Position.init(start, text.len + 1 - start));
        return text_lines.items;
    }
};
