#pragma once

template <typename T, uint32_t Length>
struct vec
{
    T data[Length];
};

using vec2 = vec<float, 2>;
using vec3 = vec<float, 3>;
using vec4 = vec<float, 4>;