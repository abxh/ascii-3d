#pragma once

#include "color.h"

#include <stdbool.h>

#ifndef SCREEN_HEIGHT
#define SCREEN_HEIGHT 20
#endif

#ifndef SCREEN_WIDTH
#define SCREEN_WIDTH 40
#endif

#ifndef MS_PER_UPDATE
#define MS_PER_UPDATE 100
#endif

#define ASPECT_RATIO (2.f * (float)SCREEN_HEIGHT / (float)SCREEN_WIDTH)

extern char g_framebuf[SCREEN_HEIGHT][SCREEN_WIDTH];

extern float g_depthbuf[SCREEN_HEIGHT][SCREEN_WIDTH];

extern colorint_str g_colorbuf [SCREEN_HEIGHT][SCREEN_WIDTH];

extern size_t g_extra_lines;

void screen_init(void);

void screen_deinit(void);

void screen_restore_line_cursor(void);
    
void screen_clear(void);

void screen_refresh(void);
<<<<<<< HEAD

// frame buffer related functions:
// -----------------------------------------------------------------------------

void draw_framebuf_point_w_no_bounds_checking(int framebuf_x, int framebuf_y, char c);

void draw_framebuf_point_w_bounds_checking(int framebuf_x, int framebuf_y, char c);

int to_framebuf_x(float screen_x);

int to_framebuf_y(float screen_y);

bool inside_framebuf(int framebuf_x, int framebuf_y); // also works for depthbuf

typedef struct {
    int x;
    int y;
} framebuf_coords;

framebuf_coords to_framebuf_coords(vec2 v);

// depth buffer related functions:
// -----------------------------------------------------------------------------

bool depthbuf_should_plot(int depthbuf_x, int depthbuf_y, float depth);

void depthbuf_plot_w_no_bounds_checking(int depthbuf_x, int depthbuf_y, float depth);
=======
>>>>>>> f44c2649a18c14202680230540944f32b55593d9
