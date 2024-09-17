const std = @import("std");
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
        if (source.get_line(self.position.start)) |line| {
            std.debug.print("{s}\n", .{source.text[line.start .. line.end() - 1]});
            for (0..(self.position.start - line.start)) |_| {
                std.debug.print(" ", .{});
            }

            for (0..self.position.length) |_| {
                std.debug.print("^", .{});
            }
            std.debug.print("\n", .{});
        } else {
            @panic("An error is outside the source code");
        }

        switch (self.kind) {
            DiagnosticKind.Info => {},
            DiagnosticKind.Warning => std.debug.print("\x1b[33m", .{}),
            DiagnosticKind.Error => std.debug.print("\x1b[31m", .{}),
            DiagnosticKind.Success => std.debug.print("\x1b[32m", .{}),
        }

        std.debug.print("{s}\x1b[0m\n\n", .{self.message});
    }
};
