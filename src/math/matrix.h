#pragma once

#include "vector.h"

template <typename T, uint32_t Dimension>
struct matrix
{
    T data[Dimension * Dimension];
};

using matrix3 = matrix<float, 3>;
using matrix4 = matrix<float, 4>;