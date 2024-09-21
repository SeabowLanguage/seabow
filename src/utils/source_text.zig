const std = @import("std");
const Position = @import("position.zig").Position;

pub const TextScope = struct { start: usize, end: usize };

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

    pub fn get_line_index(self: SourceText, pos: Position) TextScope {
        const start_pos = pos.start;
        const end_pos = pos.end();
        var scope = TextScope{ .start = 0, .end = 0 };

        for (0..self.lines.len) |i| {
            if (self.lines[i].start <= start_pos and self.lines[i].end() >= start_pos) {
                scope.start = i;
            }

            if (self.lines[i].start <= end_pos and self.lines[i].end() >= end_pos) {
                scope.end = i;
                break;
            }
        }

        return scope;
    }

    fn init_lines(text: []const u8) ![]Position {
        var text_lines = std.ArrayList(Position).init(std.heap.page_allocator);
        errdefer text_lines.deinit();
        var start: usize = 0;
        for (0..text.len) |i| {
            if (text[i] == '\n' or text[i] == 0) {
                try text_lines.append(Position.init(start, i - start));
                start = i + 1;
            }
        }

        try text_lines.append(Position.init(start, text.len + 1 - start));
        return text_lines.toOwnedSlice();
    }
};
