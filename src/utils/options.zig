const std = @import("std");

pub const OptionKind = enum {
    // Commands
    ConsoleInterpret, // <none>
    Interpret, // int <file.sbw>
    RunBytecode, // run <file.sbb>
    Compile, // build <file.sbw>
    NativeCompile, // cmp <file.sbw>

    // Options
    InputFile, // <file>
    OutputPath, // -o <path>
    NoOptimization, // -opt
};

pub const Option = struct {
    value: ?[]u8,
    kind: OptionKind,

    pub fn parse_options() ![]Option {
        var args = try std.process.ArgIterator.initWithAllocator(std.heap.page_allocator);
        defer args.deinit();

        _ = args.skip();
        var user_options = std.ArrayList(Option).init(std.heap.page_allocator);
        errdefer user_options.deinit();

        if (args.next()) |cmd| {
            if (std.mem.eql(u8, cmd, "int")) {
                try user_options.append(Option{ .kind = OptionKind.Interpret, .value = null });
            } else if (std.mem.eql(u8, cmd, "run")) {
                try user_options.append(Option{ .kind = OptionKind.RunBytecode, .value = null });
            } else {
                try user_options.append(Option{ .kind = OptionKind.ConsoleInterpret, .value = null });
            }
        } else {
            try user_options.append(Option{ .kind = OptionKind.ConsoleInterpret, .value = null });
        }

        return user_options.toOwnedSlice();
    }
};
