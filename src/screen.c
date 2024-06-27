#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "misc.h"
#include "screen.h"

char g_framebuf[SCREEN_HEIGHT][SCREEN_WIDTH];

float g_depthbuf[SCREEN_HEIGHT][SCREEN_WIDTH];

colorint_str g_colorbuf[SCREEN_HEIGHT][SCREEN_WIDTH];

size_t g_extra_lines = 0;

static inline void framebuf_clear(void) {
    for (size_t y = 0; y < SCREEN_HEIGHT; y++) {
        for (size_t x = 0; x < SCREEN_WIDTH; x++) {
            g_framebuf[y][x] = ' ';
        }
    }
}

static inline void depthbuf_clear(void) {
    for (size_t y = 0; y < SCREEN_HEIGHT; y++) {
        for (size_t x = 0; x < SCREEN_WIDTH; x++) {
<<<<<<< HEAD
            depthbuf[y][x] = INFINITY;
=======
            g_depthbuf[y][x] = INFINITY;
        }
    }
}

static inline void colorbuf_clear(void) {
    for (size_t y = 0; y < SCREEN_HEIGHT; y++) {
        for (size_t x = 0; x < SCREEN_WIDTH; x++) {
            g_colorbuf[y][x] = (colorint_str){.r = "255", .g = "255", .b = "255"};
>>>>>>> f44c2649a18c14202680230540944f32b55593d9
        }
    }
}

void screen_clear(void) {
    framebuf_clear();
    depthbuf_clear();
    colorbuf_clear();
}

void screen_init(void) {
    printf(CSI_ESC CSI_HIDECURSOR);

    for (size_t y = 0; y < SCREEN_HEIGHT; y++) {
        CLEAR_LINE();
        printf(NEW_LINE);
    }

    screen_clear();
}

void screen_deinit(void) {
    printf(CSI_ESC CSI_SHOWCURSOR);
    printf(CSI_ESC CSI_RESETCOLOR);
}

void screen_restore_line_cursor(void) {
    for (size_t i = 0; i < g_extra_lines; i++) {
        MOVE_UP_LINES(1);
    }
    g_extra_lines = 0;
}

void screen_refresh(void) {
    MOVE_UP_LINES(SCREEN_HEIGHT);
    putchar('\r');

    for (size_t y = 0; y < SCREEN_HEIGHT; y++) {
        for (size_t x = 0; x < SCREEN_WIDTH; x++) {
            printf(CSI_ESC CSI_SETCOLOR_INITIALS "%.3s;%.3s;%.3s;m", g_colorbuf[y][x].r, g_colorbuf[y][x].g, g_colorbuf[y][x].b);
            putchar(g_framebuf[y][x]);
        }
<<<<<<< HEAD
        putchar('\n');
    }
}

// frame buffer related functions:
// -----------------------------------------------------------------------------

int to_framebuf_x(float screen_x) {
    return (screen_x + 1.) / 2. * (SCREEN_WIDTH - 1);
}

int to_framebuf_y(float screen_y) {
    return (-screen_y + 1.) / 2. * (SCREEN_HEIGHT - 1);
}

bool inside_framebuf(int framebuf_x, int framebuf_y) {
    bool inside_framebuf_x = 0 <= framebuf_x && framebuf_x < SCREEN_WIDTH;
    bool inside_framebuf_y = 0 <= framebuf_y && framebuf_y < SCREEN_HEIGHT;

    return inside_framebuf_x && inside_framebuf_y;
}

void draw_framebuf_point_w_no_bounds_checking(int framebuf_x, int framebuf_y, char c) {
    framebuf[framebuf_y][framebuf_x] = c;
}

void draw_framebuf_point_w_bounds_checking(int framebuf_x, int framebuf_y, char c) {
    if (!inside_framebuf(framebuf_x, framebuf_y)) {
        return;
=======
        printf(NEW_LINE);
>>>>>>> f44c2649a18c14202680230540944f32b55593d9
    }

    printf(CSI_ESC CSI_RESETCOLOR);
}

// depth buffer related functions:
// -----------------------------------------------------------------------------

bool depthbuf_should_plot(int depthbuf_x, int depthbuf_y, float depth) {
    return depth < depthbuf[depthbuf_y][depthbuf_x];
}

void depthbuf_plot_w_no_bounds_checking(int depthbuf_x, int depthbuf_y, float depth) {
    depthbuf[depthbuf_y][depthbuf_x] = depth;
}
