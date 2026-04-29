const std = @import("std");

extern fn terminal_putchar_raw(c: u8) void;
extern fn terminal_setcolor(color: u8) void;
extern fn terminal_clear() void;

const DEFAULT_COLOR: u8 = 0x07;

const State = union(enum) {
    normal,
    escape,
    bracket: u16,
};

var current_state: State = .normal;

export fn ansi_process_char(c: u8) void {
    switch (current_state) {
        .normal => {
            if (c == '\x1b') {
                current_state = .escape;
            } else terminal_putchar_raw(c);
        },
        .escape => {
            if (c == '[') {
                current_state = .{ .bracket = 0 };
            } else {
                current_state = .normal; // abort
            }
        },
        .bracket => |*val| {
            switch (c) {
                '0'...'9' => {
                    const n = c - '0';
                    val.* = (val.* * 10) + n;
                },
                'm' => {
                    const vga_color = csi_to_vga(val.*);
                    terminal_setcolor(vga_color);
                    current_state = .normal;
                },
                else => {
                    current_state = .normal;
                },
            }
        },
    }
}

fn csi_to_vga(ansi_code: u16) u8 {
    return switch (ansi_code) {
        0 => DEFAULT_COLOR,

        // Foreground
        30 => 0,
        31 => 4,
        32 => 2,
        33 => 6,
        34 => 1,
        35 => 5,
        36 => 3,
        37 => 7,

        90 => 8,
        91 => 12,
        92 => 10,
        93 => 14,
        94 => 9,
        95 => 13,
        96 => 11,
        97 => 15,

        else => DEFAULT_COLOR,
    };
}
