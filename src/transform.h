#pragma once

typedef struct {
    float x;
    float y;
} vec2;

typedef struct {
    float x;
    float y;
    float z;
} vec3;

typedef struct {
    float x;
    float y;
    float z;
    float w;
} vec4;

// int math:
int signof_int(int expr);

int abs_int(int expr);

int max_int(int x, int y);
int min_int(int x, int y);
int clamp_int(int x, int min, int max);

// float math:
int cmp_float(float x, float y);

int round_float_to_int(float expr);

float max_float(float x, float y);
float min_float(float x, float y);
float clamp_float(float x, float min, float max);

float inside_range_float(float x, float min, float max);

// angle math:
float to_angle_in_radians(float angle_deg);
float to_angle_in_degrees(float angle_rad);

// vec2 math:
vec2 sum_vec2(vec2 a, vec2 b);
vec2 scaled_vec2(vec2 vec, float scalar);
float dot_vec2(vec2 a, vec2 b);
float length_vec2(vec2 v);

vec2 rotate_around_origo_vec2(vec2 vec, float angle_rad);

// vec3 math:
vec3 sum_vec3(vec3 a, vec3 b);
vec3 scaled_vec3(vec3 vec, float scalar);
float dot_vec3(vec3 a, vec3 b);
float length_vec3(vec3 v);
vec3 cross_vec3(vec3 a, vec3 b);

vec3 rotate_around_x_axis(vec3 vec, float angle_rad);
vec3 rotate_around_y_axis(vec3 vec, float angle_rad);
vec3 rotate_around_z_axis(vec3 vec, float angle_rad);

// vec4 math:
vec4 sum_vec4(vec4 a, vec4 b);
vec4 scaled_vec4(vec4 vec, float scalar);
float dot_vec4(vec4 a, vec4 b);
float length_vec4(vec4 v);

// 3d perspective projection:
vec2 vec3_projected_to_screen_space(vec3 vec, float fov_angle_rad, float aspect_ratio);
vec4 vec3_projection_full(vec3 vec, float fov_angle_rad, float aspect_ratio, float z_near, float z_far); // should take vec4 as input if to be converted into a matrix. no perspective divide is done
