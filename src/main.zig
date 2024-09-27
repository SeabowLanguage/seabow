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

pub fn main() !u8 {
    try init();
    defer destroy();

    var return_value: u8 = 0;
    const arg_options = try options.Option.parse_options();
    switch (arg_options[0].kind) {
        options.OptionKind.ConsoleInterpret => {
            return_value = try Interpreter.interpret_console(arg_options);
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
