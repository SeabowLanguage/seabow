const std = @import("std");
const chars = @import("chars.zig");
const Position = @import("position.zig").Position;
const SourceText = @import("source_text.zig").SourceText;

pub const DiagnosticKind = enum(u3) { Info = 0b000, Warning = 0b001, Error = 0b010, Success = 0b100 };

pub const Diagnostic = struct {
    message: []const u8,
    position: Position,
    kind: DiagnosticKind,

    pub fn init(mess: []const u8, pos: Position, kind: DiagnosticKind) Diagnostic {
        return Diagnostic{ .message = mess, .position = pos, .kind = kind };
    }

    pub fn display(self: Diagnostic, source: SourceText) void {
        const scope = source.get_line_index(self.position);
        const color_code: []const u8 = switch (self.kind) {
            DiagnosticKind.Info => "",
            DiagnosticKind.Warning => "\x1b[33m",
            DiagnosticKind.Error => "\x1b[31m",
            DiagnosticKind.Success => "\x1b[32m",
        };

        for (scope.start..scope.end + 1) |i| {
            const line = source.lines[i];
            std.debug.print("{s}\n", .{source.text[line.start .. line.end() - 1]});

            var pos: usize = line.start;
            if (i == scope.start) {
                chars.print_multi_char(' ', self.position.start - line.start);
                pos += self.position.start - line.start;
            }

            std.debug.print("{s}", .{color_code});
            const limit_arrows = if (self.position.end() < line.end()) self.position.end() else line.end();
            chars.print_multi_char('^', limit_arrows - pos);
            std.debug.print("\x1b[0m\n", .{});
        }

        if (scope.start == scope.end) {
            std.debug.print("{s}[At line {d}, column {d}]: {s}\x1b[0m\n\n", .{ color_code, scope.start + 1, self.position.start - source.lines[scope.start].start + 1, self.message });
        } else {
            std.debug.print("{s}[From line {d} to line {d}]: {s}\x1b[0m\n\n", .{ color_code, scope.start + 1, scope.end + 1, self.message });
        }
    }
};
