const std = @import("std");

extern fn terminal_putchar_raw(c: u8) void;
extern fn terminal_setcolor(color: u8) void;
extern fn terminal_clear() void;

const DEFAULT_COLOR: u8 = 0x07;

const VgaColor = struct {
    fg: u8 = 37,
    bg: u8 = 40,

    fn to_vga(self: VgaColor) u8 {
        const fg = csi_to_vga(self.fg);
        const bg = csi_to_vga(self.bg);

        return fg | bg << 4;
    }

    // Drain the accumulator and dispatch it value in correct field
    fn drain(self: *VgaColor, csi_code: u16) void {
        switch (csi_code) {
            30...39, 90...97 => {
                self.fg = @intCast(csi_code);
            },
            40...49 => {
                self.bg = @intCast(csi_code);
            },
            0 => {
                self.fg = 37;
                self.bg = 40;
            },
            else => {},
        }
    }
};

const State = union(enum) {
    normal,
    escape,
    bracket: u16,
};

var current_state: State = .normal;
var active_color: VgaColor = VgaColor{};

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
                ';' => {
                    active_color.drain(val.*);
                    val.* = 0; // reset the accumulator
                },
                'm' => {
                    active_color.drain(val.*);
                    const vga_color = active_color.to_vga();
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
        30, 40 => 0,
        31, 41 => 4,
        32, 42 => 2,
        33, 43 => 6,
        34, 44 => 1,
        35, 45 => 5,
        36, 46 => 3,
        37, 47 => 7,

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
