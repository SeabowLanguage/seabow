const std = @import("std");

pub fn is_digit(char: u8) bool {
    return char >= '0' and char <= '9';
}

pub fn is_letter(char: u8) bool {
    return (char >= 'a' and char <= 'z') or (char >= 'A' and char <= 'Z');
}

pub fn is_hexa_letter(char: u8) bool {
    return (char >= 'a' and char <= 'f') or (char >= 'A' and char <= 'F');
}

pub fn is_digit_or_letter(char: u8) bool {
    return is_digit(char) or is_letter(char);
}

pub fn is_space(char: u8) bool {
    return (char >= 0x09 and char <= 0x0d) or char == ' ';
}

pub fn get_bytes_count(char: u8) u8 {
    var count: u8 = 1;
    if (char & 0b1110_0000 == 0b1100_0000) {
        count = 2;
    } else if (char & 0b1111_0000 == 0b1110_0000) {
        count = 3;
    } else if (char & 0b1111_1000 == 0b1111_0000) {
        count = 4;
    }

    return count;
}

pub fn print_multi_char(char: u8, count: usize) void {
    if (count == 0) return;

    var buffer: [32]u8 = undefined;
    const chunk_size = @min(count, buffer.len);
    for (buffer[0..chunk_size]) |*b| {
        b.* = char;
    }

    var remaining = count;
    while (remaining >= chunk_size) {
        std.debug.print("{s}", .{buffer[0..chunk_size]});
        remaining -= chunk_size;
    }

    if (remaining > 0) {
        std.debug.print("{s}", .{buffer[0..remaining]});
    }
}

fn char_value(c: u8) ?u8 {
    if (c >= '0' and c <= '9') {
        return c - '0';
    } else if (c >= 'A' and c <= 'Z') {
        return c - 'A';
    } else if (c >= 'a' and c <= 'z') {
        return c - 'a';
    }

    return null;
}

pub fn decode_integer(word: []const u8, base: u8) u64 {
    var result: u64 = 0;
    for (word) |c| {
        const digit = char_value(c);
        if (digit) |val| {
            result = result * base + val;
        }
    }

    return result;
}

pub fn decode_decimal(word: []const u8) f64 {
    var result: f64 = 0;
    var is_fraction = false;
    var fraction_multiplier: f64 = 1;

    for (word) |c| {
        if (c == '.') {
            is_fraction = true;
            continue;
        }

        if (c >= '0' and c <= '9') {
            const val: f64 = @floatFromInt(c - '0');
            if (is_fraction) {
                fraction_multiplier /= 10.0;
                result += val * fraction_multiplier;
            } else {
                result = result * 10 + val;
            }
        }
    }

    return result;
}
