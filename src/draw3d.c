#include "draw3d.h"
#include "screen.h"
#include "transform.h"

#include <misc.h>
#include <screen.h>

void draw_flat_triangle_fb(int y0, int y1, int scanline_step, float dx0, float dx1, float currx0, float currx1, float currd_y,
                           float dd_x, float dd_y, char c) {
    // assumes currx0 <= currx1 at all times
    for (int scanlineY = y0; scanlineY < y1; scanlineY += scanline_step) {
        if (!(0 <= scanlineY && scanlineY < SCREEN_HEIGHT)) {
            continue;
        }
        float currd_xy = currd_y;
        for (int scanlineX = max_int(0, currx0); scanlineX < min_int(currx1, SCREEN_WIDTH - 1); scanlineX += 1) {
            if (depthbuf_should_plot(scanlineX, scanlineY, currd_xy)) {
                draw_framebuf_point_w_no_bounds_checking(scanlineX, scanlineY, c);
                depthbuf_plot_w_no_bounds_checking(scanlineX, scanlineY, currd_xy);
            }
            currd_xy += dd_x;
        }
        currx0 += dx0;
        currx1 += dx1;
        currd_y += dd_y;
    }
}

float get_inv_slope_fb(framebuf_coords a, framebuf_coords b) {
    return (a.x - b.x) / (float)(a.y - b.y);
}

void draw_bottom_flat_triangle_fb(framebuf_coords v1f, framebuf_coords v2f, framebuf_coords v3f, float v1d, float v2d, float v3d,
                                  char c) {
    /* Flat bottom triangle:
            v1
           /  \
          /    \
        v2 ---- v3
    */
    if (!(v2f.x <= v3f.x)) { // since stuff is drawn left to right in x-axis. impl detail
        SWAP_UNSAFE(framebuf_coords, v2f, v3f);
        SWAP_UNSAFE(float, v2d, v3d);
    }
    float n_x = v3f.x - v2f.x;
    float n_y = v2f.y - v1f.y;
    draw_flat_triangle_fb(v1f.y, v2f.y + 1, +1, get_inv_slope_fb(v1f, v2f), get_inv_slope_fb(v1f, v3f), v1f.x, v1f.x, v1d,
                          (v3d - v2d) / n_x, (v2d - v1d) / n_y, c);
}

void draw_top_flat_triangle_fb(framebuf_coords v1f, framebuf_coords v2f, framebuf_coords v3f, float v1d, float v2d, float v3d,
                               char c) {
    /* Flat top triangle:
        v1 ---- v2
         \      /
          \    /
           \  /
            v3
    */
    if (!(v1f.x <= v2f.x)) {
        SWAP_UNSAFE(framebuf_coords, v1f, v2f);
        SWAP_UNSAFE(float, v1d, v2d);
    }
    float n_x = v2f.x - v1f.x;
    float n_y = v3f.y - v1f.y;
    draw_flat_triangle_fb(v3f.y, v1f.y + 1, -1, -get_inv_slope_fb(v3f, v1f), -get_inv_slope_fb(v3f, v2f), v3f.x, v3f.x, v3d,
                          (v2d - v1d) / n_x, -1 * (v3d - v1d) / n_y, c);
}

void draw_line_horizontal_w_bounds_checking_a(int left_framebuf_x, int framebuf_y, size_t steps, char c) {
    if (!inside_framebuf(left_framebuf_x + steps, framebuf_y) && !(0 <= framebuf_y && framebuf_y < SCREEN_HEIGHT)) {
        return;
    }
    int lim = min_int(left_framebuf_x + steps, SCREEN_WIDTH - 1);
    for (int current = max_int(left_framebuf_x, 0); current < lim; current += 1) {
        draw_framebuf_point_w_no_bounds_checking(current, framebuf_y, c);
    }
}

void draw_filled_bottom_flat_triangle_framebuf_coords_a(framebuf_coords v1, framebuf_coords v2, framebuf_coords v3, char c) {
    // scanline algorithm:
    // http://www.sunshine2k.de/coding/java/TriangleRasterization/TriangleRasterization.html

    if (!(v2.x < v3.x)) {
        SWAP_UNSAFE(framebuf_coords, v2, v3);
    }

    float change_left = (v2.x - v1.x) / (float)(v2.y - v1.y);
    float change_right = (v3.x - v1.x) / (float)(v3.y - v1.y);

    float current_left = v1.x;
    float current_right = v1.x;

    for (int scanlineY = v1.y; scanlineY <= v2.y; scanlineY++) {
        draw_line_horizontal_w_bounds_checking_a(current_left, scanlineY, current_right - current_left, c);
        current_left += change_left;
        current_right += change_right;
    }
}

void draw_filled_top_flat_triangle_framebuf_coords_a(framebuf_coords v1, framebuf_coords v2, framebuf_coords v3, char c) {
    // scanline algorithm:
    // http://www.sunshine2k.de/coding/java/TriangleRasterization/TriangleRasterization.html

    if (!(v1.x < v2.x)) {
        SWAP_UNSAFE(framebuf_coords, v1, v2);
    }

    float change_left = (v3.x - v1.x) / (float)(v3.y - v1.y);
    float change_right = (v3.x - v2.x) / (float)(v3.y - v2.y);

    float current_left = v3.x;
    float current_right = v3.x;

    for (int scanlineY = v3.y; scanlineY > v1.y; scanlineY--) {
        draw_line_horizontal_w_bounds_checking_a(current_left, scanlineY, current_right - current_left, c);
        current_left -= change_left;
        current_right -= change_right;
    }
}

void draw_filled_triangle_framebuf_coords_a(framebuf_coords v1, framebuf_coords v2, framebuf_coords v3, char c) {
    // scanline algorithm:
    // http://www.sunshine2k.de/coding/java/TriangleRasterization/TriangleRasterization.html

    /* sort points and ensure v1.y <= v2.y <= v3.y */
    if (v2.y < v1.y) {
        SWAP_UNSAFE(framebuf_coords, v2, v1);
    }
    if (v3.y < v1.y) {
        SWAP_UNSAFE(framebuf_coords, v3, v1);
    }
    if (v3.y < v2.y) {
        SWAP_UNSAFE(framebuf_coords, v3, v2);
    }

    if (v1.y == v2.y && v2.y == v3.y) {
        draw_framebuf_point_w_bounds_checking(v1.x, v1.y, c);
    } else if (v2.y == v3.y) {
        draw_filled_bottom_flat_triangle_framebuf_coords_a(v1, v2, v3, c);
    } else if (v1.y == v2.y) {
        draw_filled_top_flat_triangle_framebuf_coords_a(v1, v2, v3, c);
    } else {
        framebuf_coords v = {.x = v1.x + ((v2.y - v1.y) / (float)(v3.y - v1.y)) * (v3.x - v1.x), .y = v2.y};
        draw_filled_bottom_flat_triangle_framebuf_coords_a(v1, v2, v, c);
        draw_filled_top_flat_triangle_framebuf_coords_a(v2, v, v3, c);
    }
}

void draw_filled_triangle_2d_a(vec2 p1, vec2 p2, vec2 p3, char c) {
    draw_filled_triangle_framebuf_coords_a(to_framebuf_coords(p1), to_framebuf_coords(p2), to_framebuf_coords(p3), c);
}

void draw_triangle_3d(vec3 p1, vec3 p2, vec3 p3, char c) {
    // vec3 p1_to_p3 = sum_vec3(p3, scaled_vec3(p1, -1));
    // vec3 p1_to_p2 = sum_vec3(p2, scaled_vec3(p1, -1));
    //
    // vec3 triangle_normal = cross_vec3(p1_to_p3, p1_to_p2);
    //
    // // backface culling:
    // if (dot_vec3(p1, triangle_normal) >= 0) {
    //     return;
    // }

    vec4 v1 = vec3_projection_full(p1, fov_angle_rad, aspect_ratio, z_near, z_far);
    vec4 v2 = vec3_projection_full(p2, fov_angle_rad, aspect_ratio, z_near, z_far);
    vec4 v3 = vec3_projection_full(p3, fov_angle_rad, aspect_ratio, z_near, z_far);

    // reject triangles with verticies where z is outside of range [z_near; z_far]
    // if (!inside_range_float(v1.w, z_near, z_far) || !inside_range_float(v2.w, z_near, z_far) ||
    //     !inside_range_float(v3.w, z_near, z_far)) {
    //     return;
    // }

    // now we can happily divide with no concern for division by 0
    v1 = scaled_vec4(v1, 1.f / v1.w);
    v2 = scaled_vec4(v2, 1.f / v2.w);
    v3 = scaled_vec4(v3, 1.f / v3.w);

    draw_filled_triangle_2d_a(*(vec2*)&v1, *(vec2*)&v2, *(vec2*)&v3, c);
}
