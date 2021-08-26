#pragma once

#include <cstdint>
#include <concepts>

template <class T>
concept arithmetic = std::is_arithmetic_v<T>;

namespace math
{

    template <typename T, size_t L> struct vec
    {
        T data[L];
        constexpr vec() noexcept = default;
        template <typename... Pack> constexpr vec(Pack... pack) noexcept : data{ pack... } {}

        constexpr size_t size() const noexcept { return L; };

        constexpr T const& operator[](size_t index) const noexcept { return data[index]; }
        constexpr T& operator[](size_t index) noexcept { return data[index]; }

        constexpr bool operator==(vec<T, L> const& right) const = default;
    };

    // -vec -> vec (memberwise)
    template <arithmetic T, size_t L> constexpr auto operator-(vec<T, L> const& right) noexcept
    {
        vec<T, L> out;
        for (size_t i = 0; i < L; i++)
        {
            out[i] = -right[i];
        }
        return out;
    }

    // vec + vec -> vec (memberwise)
    template <arithmetic T, size_t L>
    constexpr auto operator+(vec<T, L> const& left, vec<T, L> const& right) noexcept
    {
        vec<T, L> out;
        for (size_t i = 0; i < L; i++)
        {
            out[i] = left[i] + right[i];
        }
        return out;
    }
    // vec += vec -> left (memberwise)
    template <arithmetic T, size_t L>
    constexpr auto operator+=(vec<T, L>& left, vec<T, L> const& right) noexcept
    {
        for (size_t i = 0; i < L; i++)
        {
            left[i] += right[i];
        }
        return left;
    }
    // vec += scalar -> left (memberwise)
    template <arithmetic T, size_t L>
    constexpr auto operator+=(vec<T, L>& left, T const& right) noexcept
    {
        for (size_t i = 0; i < L; i++)
        {
            left[i] += right;
        }
        return left;
    }
    // scalar + vec -> vec (memberwise)
    template <arithmetic T, size_t L>
    constexpr auto operator+(T const& left, vec<T, L> const& right) noexcept
    {
        vec<T, L> out;
        for (size_t i = 0; i < L; i++)
        {
            out[i] = left + right[i];
        }
        return out;
    }
    // vec + scalar -> vec (memberwise)
    template <arithmetic T, size_t L>
    constexpr auto operator+(vec<T, L> const& left, T const& right) noexcept
    {
        vec<T, L> out;
        for (size_t i = 0; i < L; i++)
        {
            out[i] = left[i] + right;
        }
        return out;
    }

    // vec - vec -> vec (memberwise)
    template <arithmetic T, size_t L>
    constexpr auto operator-(vec<T, L> const& left, vec<T, L> const& right) noexcept
    {
        vec<T, L> out;
        for (size_t i = 0; i < L; i++)
        {
            out[i] = left[i] - right[i];
        }
        return out;
    }
    // vec -= vec -> left (memberwise)
    template <arithmetic T, size_t L>
    constexpr auto operator-=(vec<T, L>& left, vec<T, L> const& right) noexcept
    {
        for (size_t i = 0; i < L; i++)
        {
            left[i] -= right[i];
        }
        return left;
    }
    // vec -= scalar -> left (memberwise)
    template <arithmetic T, size_t L>
    constexpr auto operator-=(vec<T, L>& left, T const& right) noexcept
    {
        for (size_t i = 0; i < L; i++)
        {
            left[i] -= right;
        }
        return left;
    }
    // scalar - vec -> vec (memberwise)
    template <arithmetic T, size_t L>
    constexpr auto operator-(T const& left, vec<T, L> const& right) noexcept
    {
        vec<T, L> out;
        for (size_t i = 0; i < L; i++)
        {
            out[i] = left - right[i];
        }
        return out;
    }
    // vec - scalar -> vec (memberwise)
    template <arithmetic T, size_t L>
    constexpr auto operator-(vec<T, L> const& left, T const& right) noexcept
    {
        vec<T, L> out;
        for (size_t i = 0; i < L; i++)
        {
            out[i] = left[i] - right;
        }
        return out;
    }

    // vec * vec -> vec (memberwise)
    template <arithmetic T, size_t L>
    constexpr auto operator*(vec<T, L> const& left, vec<T, L> const& right) noexcept
    {
        vec<T, L> out;
        for (size_t i = 0; i < L; i++)
        {
            out[i] = left[i] * right[i];
        }
        return out;
    }
    // vec *= vec -> left (memberwise)
    template <arithmetic T, size_t L>
    constexpr auto operator*=(vec<T, L>& left, vec<T, L> const& right) noexcept
    {
        for (size_t i = 0; i < L; i++)
        {
            left[i] *= right[i];
        }
        return left;
    }
    // vec *= scalar -> left (memberwise)
    template <arithmetic T, size_t L>
    constexpr auto operator*=(vec<T, L>& left, T const& right) noexcept
    {
        for (size_t i = 0; i < L; i++)
        {
            left[i] *= right;
        }
        return left;
    }
    // scalar * vec -> vec (memberwise)
    template <arithmetic T, size_t L>
    constexpr auto operator*(T const& left, vec<T, L> const& right) noexcept
    {
        vec<T, L> out;
        for (size_t i = 0; i < L; i++)
        {
            out[i] = left * right[i];
        }
        return out;
    }
    // vec * scalar -> vec (memberwise)
    template <arithmetic T, size_t L>
    constexpr auto operator*(vec<T, L> const& left, T const& right) noexcept
    {
        vec<T, L> out;
        for (size_t i = 0; i < L; i++)
        {
            out[i] = left[i] * right;
        }
        return out;
    }

    // vec / vec -> vec (memberwise)
    template <arithmetic T, size_t L>
    constexpr auto operator/(vec<T, L> const& left, vec<T, L> const& right) noexcept
    {
        vec<T, L> out;
        for (size_t i = 0; i < L; i++)
        {
            out[i] = left[i] / right[i];
        }
        return out;
    }
    // vec /= vec -> left (memberwise)
    template <arithmetic T, size_t L>
    constexpr auto operator/=(vec<T, L>& left, vec<T, L> const& right) noexcept
    {
        for (size_t i = 0; i < L; i++)
        {
            left[i] /= right[i];
        }
        return left;
    }
    // vec /= scalar -> left (memberwise)
    template <arithmetic T, size_t L>
    constexpr auto operator/=(vec<T, L>& left, T const& right) noexcept
    {
        for (size_t i = 0; i < L; i++)
        {
            left[i] /= right;
        }
        return left;
    }
    // scalar / vec -> vec
    template <arithmetic T, size_t L>
    constexpr auto operator/(T const& left, vec<T, L> const& right) noexcept
    {
        vec<T, L> out;
        for (size_t i = 0; i < L; i++)
        {
            out[i] = left / right[i];
        }
        return out;
    }
    // vec / scalar -> vec
    template <arithmetic T, size_t L>
    constexpr auto operator/(vec<T, L> const& left, T const& right) noexcept
    {
        vec<T, L> out;
        for (size_t i = 0; i < L; i++)
        {
            out[i] = left[i] / right;
        }
        return out;
    }

    // dot(vec, vec) -> scalar
    template <arithmetic T, size_t L>
    constexpr auto dot(vec<T, L> const& left, vec<T, L> const& right) noexcept
    {
        T out{};
        for (size_t i = 0; i < L; i++)
        {
            out += left[i] * right[i];
        }
        return out;
    }

    // min(vec) -> scalar
    template <arithmetic T, size_t L>
    constexpr auto min(vec<T, L> const& left, vec<T, L> const& right) noexcept
    {
        vec<T, L> out;
        for (size_t i = 0; i < L; i++)
        {
            out[i] = (left[i] < right[i]) ? left[i] : right[i];
        }
        return out;
    }

    // max(vec) -> scalar
    template <arithmetic T, size_t L>
    constexpr auto max(vec<T, L> const& left, vec<T, L> const& right) noexcept
    {
        vec<T, L> out;
        for (size_t i = 0; i < L; i++)
        {
            out[i] = (left[i] > right[i]) ? left[i] : right[i];
        }
        return out;
    }

    // abs(vec) -> vec
    template <arithmetic T, size_t L> constexpr auto abs(vec<T, L> const& left) noexcept
    {
        vec<T, L> out;
        for (size_t i = 0; i < L; i++)
        {
            out[i] = (left[i] > 0) ? left[i] : -left[i];
        }
        return out;
    }

    // clamp(in: vec, min: scalar, max: scalar) -> vec
    template <arithmetic T, size_t L, arithmetic U, arithmetic V>
    constexpr auto clamp(vec<T, L> const& value, U min, V max) noexcept->vec<T, L>
    {
        vec<T, L> out;
        for (size_t i = 0; i < L; i++)
        {
            out[i] = value[i] < max ? ((value[i] > min) ? value[i] : min) : max;
        }
        return out;
    }

    // clamp(in: vec, min: vec, max: vec) -> vec
    template <arithmetic T, size_t L>
    constexpr auto clamp(vec<T, L> const& value, vec<T, L> const& min, vec<T, L> const& max) noexcept
    {
        return math::max(math::min(value, max), min);
    }

    // clamp(in: vec, min: 0, max: 1) -> vec
    template <arithmetic T, size_t L> constexpr auto saturate(vec<T, L> const& value) noexcept
    {
        return math::clamp(value, 0, 1);
    }

    // lerp(vec, vec) -> vec
    template <std::floating_point F, size_t L>
    constexpr auto lerp(vec<F, L> const& lower, vec<F, L> const& upper, F interp) noexcept
    {
        return lower + interp * (upper - lower);
    }
    template <std::floating_point F, size_t L>
    constexpr auto lerp(vec<F, L> const& lower, vec<F, L> const& upper, vec<F, L> const& interp) noexcept
    {
        return lower + interp * (upper - lower);
    }

    // minComponent(vec) -> scalar
    template <arithmetic T, size_t L> constexpr auto minComponent(vec<T, L> const& value) noexcept
    {
        T out = value[0];
        for (size_t i = 1; i < L; i++)
        {
            if (out > value[i]) out = value[i];
        }
        return out;
    }
    // maxComponent(vec) -> vec
    template <arithmetic T, size_t L> constexpr auto maxComponent(vec<T, L> const& value) noexcept
    {
        T out = value[0];
        for (size_t i = 1; i < L; i++)
        {
            if (out < value[i]) out = value[i];
        }
        return out;
    }

    // all(vec<bool>) -> bool
    template <size_t L> constexpr auto all(vec<bool, L> const& value) noexcept
    {
        bool result = true;
        for (size_t i = 0; i < L; ++i)
        {
            result &= value[i];
        }
        return result;
    }

    // any(vec<bool>) -> bool
    template <size_t L> constexpr auto any(vec<bool, L> const& value) noexcept
    {
        bool result = false;
        for (size_t i = 0; i < L; ++i)
        {
            result |= value[i];
        }
        return result;
    }

    // compare(vec, vec) -> vec<bool>
    template <arithmetic T, size_t L>
    constexpr auto compare(vec<T, L> const& left, vec<T, L> const& right) noexcept
    {
        vec<bool, L> out;
        for (size_t i = 0; i < L; ++i)
        {
            out[i] = left[i] == right[i];
        }
        return out;
    }

    using vec2 = vec<float, 2>;
    using vec3 = vec<float, 3>;
    using vec4 = vec<float, 4>;

    using vec2i = vec<int32_t, 2>;
    using vec3i = vec<int32_t, 3>;
    using vec4i = vec<int32_t, 4>;

    using vec2u = vec<uint32_t, 2>;
    using vec3u = vec<uint32_t, 3>;
    using vec4u = vec<uint32_t, 4>;

} // namespace math