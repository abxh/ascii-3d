#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "transform.h"

// ----------------------------------------------------------------------------
// int math

int signof_int(int expr) {
    return (expr > 0) - (expr < 0);
}

int abs_int(int expr) {
    return (expr > 0) ? expr : -expr;
}

int max_int(int x, int y) {
    return (x > y) ? x : y;
}

int min_int(int x, int y) {
    return (x < y) ? x : y;
}

int clamp_int(int x, int min, int max) {
    return min_int(max_int(x, min), max);
}

// ----------------------------------------------------------------------------
// float math

static const float float_delta = 0.001f;

int round_float_to_int(float expr) {
    return (expr > (floorf(expr) + 0.5f + float_delta)) ? ceilf(expr) : floorf(expr);
}

int cmp_float(float x, float y) {
    return (x > y + float_delta) - (x + float_delta < y);
}

float max_float(float x, float y) {
    return (x > y + float_delta) ? x : y;
}

float min_float(float x, float y) {
    return (x + float_delta < y) ? x : y;
}

float clamp_float(float x, float min, float max) {
    return min_float(max_float(x, min), max);
}

float inside_range_float(float x, float min, float max) {
    return min <= x && x <= max;
}

// ----------------------------------------------------------------------------
// angle math

float to_angle_in_radians(float angle_deg) {
    return angle_deg * M_PI / 180.f;
}

float to_angle_in_degrees(float angle_rad) {
    return angle_rad * 180.f / M_PI;
}

// ----------------------------------------------------------------------------
// vec2 math

vec2 sum_vec2(vec2 a, vec2 b) {
    return (vec2){.x = a.x + b.x, .y = a.y + b.y};
}

vec2 scaled_vec2(vec2 vec, float scalar) {
    return (vec2){.x = scalar * vec.x, .y = scalar * vec.y};
}

float dot_vec2(vec2 a, vec2 b) {
    return a.x * b.x + a.y * b.y;
}

vec2 rotate_around_origo_vec2(vec2 vec, float angle_rad) {
    return (vec2){.x = vec.x * cosf(angle_rad) - vec.y * sinf(angle_rad), .y = vec.x * sinf(angle_rad) + vec.y * cosf(angle_rad)};
}

float length_vec2(vec2 v) {
    return sqrtf(dot_vec2(v, v));
}

// ----------------------------------------------------------------------------
// vec3 math

vec3 sum_vec3(vec3 a, vec3 b) {
    return (vec3){.x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z};
}

vec3 scaled_vec3(vec3 vec, float scalar) {
    return (vec3){.x = scalar * vec.x, .y = scalar * vec.y, .z = scalar * vec.z};
}

float dot_vec3(vec3 a, vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

vec3 cross_vec3(vec3 a, vec3 b) {
    // (a2b3 - a3b2, a3b1 - a1b3, a1b2 - a2b1)
    return (vec3){.x = a.y * b.z - a.z * b.y, .y = a.z * b.x - a.x * b.z, .z = a.x * b.y - a.y * b.x};
}

vec3 rotate_around_x_axis(vec3 vec, float angle_rad) {
    return (vec3){
        .x = vec.x, .y = vec.y * cosf(angle_rad) - vec.z * sinf(angle_rad), .z = vec.y * sinf(angle_rad) + vec.z * cosf(angle_rad)};
}

vec3 rotate_around_y_axis(vec3 vec, float angle_rad) {
    return (vec3){
        .x = vec.x * cosf(angle_rad) + vec.z * sinf(angle_rad), .y = vec.y, .z = vec.z * cosf(angle_rad) - vec.x * sinf(angle_rad)};
}

vec3 rotate_around_z_axis(vec3 vec, float angle_rad) {
    return (vec3){
        .x = vec.x * cosf(angle_rad) - vec.y * sinf(angle_rad), .y = vec.x * sinf(angle_rad) + vec.y * cosf(angle_rad), .z = vec.z};
}

float length_vec3(vec3 v) {
    return sqrtf(dot_vec3(v, v));
}

// ----------------------------------------------------------------------------
// vec4 math

vec4 sum_vec4(vec4 a, vec4 b) {
    return (vec4){.x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z, .w = a.z + b.z};
}

vec4 scaled_vec4(vec4 vec, float scalar) {
    return (vec4){.x = scalar * vec.x, .y = scalar * vec.y, .z = scalar * vec.z, .w = scalar * vec.w};
}

float dot_vec4(vec4 a, vec4 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

float length_vec4(vec4 v) {
    return sqrtf(dot_vec4(v, v));
}

// ----------------------------------------------------------------------------
// 3d perspective projection

vec2 vec3_projected_to_screen_space(vec3 vec, float fov_angle_rad, float aspect_ratio) {
    // consider aspect ratio:
    vec2 res = (vec2){.x = aspect_ratio * vec.x, .y = vec.y};

    // scale by fov scalar and z-divide:
    float scalar = vec.z * tanf(fov_angle_rad / 2.f);
    assert("fov angle or z-value does not lead to division by zero" && cmp_float(scalar, 0) != 0);
    res = scaled_vec2(res, 1.f / scalar);

    return res;
}

vec4 vec3_projection_full(vec3 vec, float fov_angle_rad, float aspect_ratio, float z_near, float z_far) {
    // z-buffering schenigan. used to make near objects more precise, far objects less precise
    float depth_scalar = z_far / (z_far - z_near); 

    // res to be multiplied by the scalar (1 / w) for z-divide
    vec4 res = {.x = aspect_ratio * vec.x, .y = vec.y, .z = depth_scalar * vec.z - depth_scalar * z_near, .w = vec.z};

    // scale by fov scalar:
    float fov_scalar = tanf(fov_angle_rad / 2.f);
    assert("fov angle does not lead to division by zero" && cmp_float(fov_scalar, 0) != 0);
    res = scaled_vec4(res, 1.f / fov_scalar);

    return res;
}
