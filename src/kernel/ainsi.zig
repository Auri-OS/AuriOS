const std = @import("std");

extern fn terminal_set_cursor(x: usize, y: usize) void;
extern fn terminal_clear_until_cursor() void;
extern fn terminal_clear_after_cursor() void;
extern fn terminal_putchar_raw(c: u8) void;
extern fn terminal_setcolor(color: u8) void;
extern fn terminal_clear() void;

const DEFAULT_COLOR: u8 = 0x07;
const MAX_PARAMS = 8;

pub fn panic(msg: []const u8, error_return_trace: ?*std.builtin.StackTrace, ret_addr: ?usize) noreturn {
    _ = msg;
    _ = error_return_trace;
    _ = ret_addr;

    while (true) {
        asm volatile ("cli; hlt");
    }
}

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
    bracket: struct {
        params: [MAX_PARAMS]u16,
        param_index: usize,
    },
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
                current_state = .{ .bracket = .{ .params = [_]u16{0} ** MAX_PARAMS, .param_index = 0 } };
            } else {
                current_state = .normal; // abort
            }
        },
        .bracket => |*val| {
            switch (c) {
                '0'...'9' => {
                    const n = c - '0';
                    val.params[val.param_index] = (val.params[val.param_index] * 10) + n;
                },
                'J' => {
                    switch (val.params[0]) {
                        0 => terminal_clear_after_cursor(),
                        1 => terminal_clear_until_cursor(),
                        2 => terminal_clear(),
                        else => {},
                    }
                    current_state = .normal;
                },
                'H' => {
                    const y = if (val.params[0] == 0) 1 else val.params[0];
                    const x = if (val.params[1] == 0) 1 else val.params[1];
                    terminal_set_cursor(x - 1, y - 1);
                    current_state = .normal;
                },
                ';' => {
                    // active_color.drain(val.params[val.param_index]);
                    val.param_index = (val.param_index + 1) % MAX_PARAMS;
                },
                'm' => {
                    var i: usize = 0;
                    while (i <= val.param_index) : (i += 1) {
                        active_color.drain(val.params[i]);
                    }
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
