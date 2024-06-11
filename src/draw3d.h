#pragma once

#include <math.h>
#include <stdint.h>

#include "transform.h"

/*
world space:
                /|\ (1,0,0)
                 |   -/\ (0,0,1)
                 |   /
                 | /
(-1,0,0) <-------.--------> (1,0,0)
                /|
              /  |
           |/_   |
      (0,0,-1)  \|/ (-1, 0, 0)
*/

/* near and far plane distances for clipping */
static const float z_near = 0.1f;
static const float z_far = 10.0f;

/* fov angle */
static const float fov_angle_rad = M_PI / 3; // 60 degrees

/* Fixed details:
    - camera center: (0, 0, 0)
    - counterclockwise winding order: p1 -> p2 -> p3
*/
void draw_triangle_3d(vec3 p1, vec3 p2, vec3 p3, char c); // with backface culling, z-buffering. filled.
