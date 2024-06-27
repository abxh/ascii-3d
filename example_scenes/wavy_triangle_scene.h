#pragma once

#include "scene.h"

void** wavy_triangle_create(void);

void wavy_triangle_destroy(void** context_ptr);

void wavy_triangle_update(void** context_ptr);

void wavy_triangle_render(void** context_ptr);

extern const scene_type g_wavy_triangle_scene;
