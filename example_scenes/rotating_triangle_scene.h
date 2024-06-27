#pragma once

#include "scene.h"

void** rotating_triangle_scene_create(void);

void rotating_triangle_scene_destroy(void** context_ptr);

void rotating_triangle_scene_update(void** context_ptr);

void rotating_triangle_scene_render(void** context_ptr);

extern const scene_type g_rotating_triangle_scene;
