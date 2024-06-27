#include "spiral_scene.h"
#include "color.h"
#include "draw.h"
#include "scene.h"
#include "transform.h"
#include "screen.h"

#ifdef DEBUG
#include "misc.h"
#endif

#include <stdint.h>
#include <stdlib.h>

#ifdef DEBUG
#include <stdio.h>
#endif

typedef struct {
    int64_t angle_deg;
} spiral;

const scene_type g_spiral_scene = {.flags = SCENE_OPS_NOP,
                                   .create = spiral_scene_create,
                                   .destroy = spiral_scene_destroy,
                                   .update = spiral_scene_update,
                                   .render = spiral_scene_render};

#define SPIRAL_OBJ 0

void** spiral_scene_create(void) {
    void** context_ptr = calloc(1, sizeof(void*));
    context_ptr[SPIRAL_OBJ] = malloc(sizeof(spiral));
    ((spiral*)context_ptr[SPIRAL_OBJ])->angle_deg = 0;
    return context_ptr;
}

void spiral_scene_destroy(void** context_ptr) {
    free(context_ptr[SPIRAL_OBJ]);
    free(context_ptr);
}

void spiral_scene_update(void** context_ptr) {
    spiral* spiral_ptr = (spiral*)context_ptr[SPIRAL_OBJ];
    spiral_ptr->angle_deg += (int)(10.f * MS_PER_UPDATE / 400.f);
}

void spiral_scene_render(void** context_ptr) {
    spiral* spiral_ptr = (spiral*)context_ptr[SPIRAL_OBJ];
    int64_t angle_deg = spiral_ptr->angle_deg;

    for (size_t o = 0; o < 6; o++) {
        for (size_t i = 0; i <= 6; i++) {
            float angle_rad = to_angle_in_radians((float)-angle_deg - 20.f * (float)i - 60.f * (float)o);
            vec2 v_base = {.x = 0.6f - 0.1f * (float)i, .y = 0.6f - 0.1f * (float)i};
            vec2 v = rotate_around_origo_vec2(v_base, angle_rad);

            draw_point_2d((vec2[1]){v}, (color[1]){color_white}, '*');

#ifdef DEBUG
            CLEAR_LINE();
            printf("i: %zu, o: %zu" NEW_LINE, i, o);

            CLEAR_LINE();
            printf("angle_deg: %ld, angle_rad: %.2f" NEW_LINE, angle_deg, (double)angle_rad);

            CLEAR_LINE();
            printf("v_base.x: %.2f, v_base.y: %.2f" NEW_LINE, (double)v_base.x, (double)v_base.y);

            CLEAR_LINE();
            printf("v.x: %.2f, v.y: %.2f" NEW_LINE, (double)v.x, (double)v.y);

            g_extra_lines += 4;
#endif
        }
    }
}
