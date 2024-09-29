const std = @import("std");

const Diagnostic = @import("utils/diagnostic.zig").Diagnostic;
const Parser = @import("core/parser.zig").Parser;
const Interpreter = @import("core/interpreter.zig").Interpreter;
const lx = @import("core/lexer.zig");
const SourceText = @import("utils/source_text.zig").SourceText;
const options = @import("utils/options.zig");

fn init() !void {
    try lx.init_keywords();
}

fn destroy() void {
    lx.delete_keywords();
}

fn catch_error(err: anyerror, ret: u8) u8 {
    std.debug.print("\x1b[31mSeabowInternalError: {}\x1b[0m\n", .{err});
    return ret;
}

pub fn main() u8 {
    init() catch |err| {
        destroy();
        return catch_error(err, 1);
    };
    defer destroy();

    var return_value: u8 = 0;
    const arg_options = options.Option.parse_options() catch |err| {
        return catch_error(err, 2);
    };

    switch (arg_options[0].kind) {
        options.OptionKind.ConsoleInterpret => {
            return_value = Interpreter.interpret_console(arg_options) catch |err| {
                return catch_error(err, 3);
            };
        },

        options.OptionKind.Interpret => {
            return_value = Interpreter.interpret_file(arg_options);
        },

        else => {
            std.debug.print("SeabowInfo: Command not yet supported\n", .{});
        },
    }

    return return_value;
}
