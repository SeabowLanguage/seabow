const std = @import("std");

pub const OptionKind = enum {
    // Special
    Nothing,

    // Commands
    ConsoleInterpret, // <none>
    Interpret, // int <file.sbw>
    RunBytecode, // run <file.sbb>
    Compile, // build <file.sbw>
    NativeCompile, // cmp <file.sbw>

    // Options
    InputFile, // -i <file>
    OutputPath, // -o <path|file>
    NoOptimization, // -opt
};

pub const Option = struct {
    value: ?[]const u8,
    kind: OptionKind,

    pub fn free(self: Option) void {
        if (self.value) |val| {
            std.heap.page_allocator.free(val);
        }
    }

    pub fn parse_command() ![]Option {
        var args = try std.process.ArgIterator.initWithAllocator(std.heap.page_allocator);
        defer args.deinit();

        _ = args.skip();
        var user_options = std.ArrayList(Option).init(std.heap.page_allocator);
        errdefer user_options.deinit();

        var firstCmd: ?[]const u8 = null;
        if (args.next()) |cmd| {
            if (std.mem.eql(u8, cmd, "int")) {
                try user_options.append(Option{ .kind = OptionKind.Interpret, .value = null });
            } else if (std.mem.eql(u8, cmd, "run")) {
                try user_options.append(Option{ .kind = OptionKind.RunBytecode, .value = null });
            } else {
                try user_options.append(Option{ .kind = OptionKind.ConsoleInterpret, .value = null });
                firstCmd = cmd;
            }
        } else {
            try user_options.append(Option{ .kind = OptionKind.ConsoleInterpret, .value = null });
        }

        if (firstCmd) |cmd| {
            const opt = parse_option(cmd, &args);
            if (opt.kind != OptionKind.Nothing) {
                try user_options.append(opt);
            }
        }

        while (args.next()) |next| {
            const opt = parse_option(next, &args);
            if (opt.kind != OptionKind.Nothing) {
                try user_options.append(opt);
            }
        }

        return user_options.toOwnedSlice();
    }

    fn parse_option(arg: []const u8, args: *std.process.ArgIterator) Option {
        if (std.mem.eql(u8, arg, "-i")) {
            if (args.next()) |next| {
                if (next[0] == '-') {
                    std.debug.print("\x1b[33mSeabowOptionWarning: Option `-i` need a file path after it\x1b[0m\n", .{});
                    return parse_option(next, args);
                }

                return Option{ .kind = OptionKind.InputFile, .value = copy_value(next) };
            } else {
                std.debug.print("\x1b[33mSeabowOptionWarning: Option `-i` need a file path after it\x1b[0m\n", .{});
                return Option.nothing();
            }
        } else {
            if (arg[0] != '-') {
                std.debug.print("\x1b[33mSeabowOptionWarning: Command `{s}` should be placed at first position if it is a correct seabow command\x1b[0m\n", .{arg});
            } else {
                std.debug.print("\x1b[33mSeabowOptionWarning: Unknown option `{s}` found\x1b[0m\n", .{arg});
            }

            return Option.nothing();
        }
    }

    fn nothing() Option {
        return Option{ .kind = OptionKind.Nothing, .value = null };
    }

    fn copy_value(value: []const u8) []const u8 {
        const copy = std.heap.page_allocator.alloc(u8, value.len) catch {
            return "";
        };
        std.mem.copyBackwards(u8, copy, value);
        return copy;
    }
};
