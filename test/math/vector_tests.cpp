#include <catch2/catch_test_macros.hpp>

#include "math/vector.h"

TEST_CASE("Vector utilities", "[math]")
{
    math::vec2 a;
    REQUIRE(a.size() == 2);
    math::vec3 b;
    REQUIRE(b.size() == 3);
    math::vec4 c;
    REQUIRE(c.size() == 4);
}

TEST_CASE("Vector construction", "[math]")
{
    SECTION("Zero Initialization")
    {
        math::vec2 a = {};
        REQUIRE(a[0] == 0.f);
        REQUIRE(a[1] == 0.f);

        math::vec2 b{};
        REQUIRE(b[0] == 0.f);
        REQUIRE(b[1] == 0.f);
    }
    SECTION("Value initialization")
    {
        math::vec2 a(2.f, 3.f);
        REQUIRE(a[0] == 2.f);
        REQUIRE(a[1] == 3.f);

        math::vec3 b{ 2.f, 3.f, 5.f };
        REQUIRE(b[0] == 2.f);
        REQUIRE(b[1] == 3.f);
        REQUIRE(b[2] == 5.f);

        math::vec2 c = { 2.f, 3.f };
        REQUIRE(c[0] == 2.f);
        REQUIRE(c[1] == 3.f);

        auto d = math::vec4(2.f, 3.f, 5.f, 6.f);
        REQUIRE(d[0] == 2.f);
        REQUIRE(d[1] == 3.f);
        REQUIRE(d[2] == 5.f);
        REQUIRE(d[3] == 6.f);
        auto e = math::vec4{ 2.f, 3.f, 5.f, 6.f };

        auto f = math::vec<float, 5>{ 2.f, 3.f, 5.f, 6.f, 8.f };
        REQUIRE(f[0] == 2.f);
        REQUIRE(f[1] == 3.f);
        REQUIRE(f[2] == 5.f);
        REQUIRE(f[3] == 6.f);
        REQUIRE(f[4] == 8.f);
        auto g = math::vec<float, 5>(2.f, 3.f, 5.f, 6.f, 8.f);
    }
}
TEST_CASE("Vector assignment", "[math]")
{
    math::vec2 a{}, b{};
    SECTION("Memberwise Assignment")
    {
        b[0] = 5.f;
        b[1] = 8.f;
        REQUIRE(b[0] == 5.f);
        REQUIRE(b[1] == 8.f);
    }
    SECTION("Copy Assignment")
    {
        a = b;
        REQUIRE(a[0] == b[0]);
        REQUIRE(a[1] == b[1]);
    }
}
TEST_CASE("Vector equality", "[math]")
{
    REQUIRE(math::vec2{ 2.f, 3.f } == math::vec2{ 2.f, 3.f });
    REQUIRE(math::vec3{ 2.f, 3.f, 4.f } == math::vec3{ 2.f, 3.f, 4.f });
    REQUIRE(math::vec4{ 2.f, 3.f, 4.f, 5.f } == math::vec4{ 2.f, 3.f, 4.f, 5.f });
}
TEST_CASE("Vector addition", "[math]")
{
    SECTION("vector + vector")
    {
        REQUIRE(math::vec2{ 2.f, 3.f } + math::vec2{ 2.f, 3.f } == math::vec2{ 4.f, 6.f });
        REQUIRE(math::vec3{ 2.f, 3.f, 4.f } + math::vec3{ 2.f, 3.f, 4.f } == math::vec3{ 4.f, 6.f, 8.f });
        REQUIRE(math::vec4{ 2.f, 3.f, 4.f, 5.f } + math::vec4{ 2.f, 3.f, 4.f, 5.f } ==
                math::vec4{ 4.f, 6.f, 8.f, 10.f });
    }
    SECTION("vector + scalar")
    {
        REQUIRE(math::vec2{ 2.f, 3.f } + 5.f == math::vec2{ 7.f, 8.f });
        REQUIRE(math::vec3{ 2.f, 3.f, 4.f } + 5.f == math::vec3{ 7.f, 8.f, 9.f });
        REQUIRE(math::vec4{ 2.f, 3.f, 4.f, 5.f } + 5.f == math::vec4{ 7.f, 8.f, 9.f, 10.f });
    }
    SECTION("scalar + vector")
    {
        REQUIRE(5.f + math::vec2{ 2.f, 3.f } == math::vec2{ 7.f, 8.f });
        REQUIRE(5.f + math::vec3{ 2.f, 3.f, 4.f } == math::vec3{ 7.f, 8.f, 9.f });
        REQUIRE(5.f + math::vec4{ 2.f, 3.f, 4.f, 5.f } == math::vec4{ 7.f, 8.f, 9.f, 10.f });
    }
    SECTION("vector += vector")
    {
        math::vec2 a = math::vec2{ 2.f, 3.f };
        a += math::vec2{ 2.f, 3.f };
        REQUIRE(a == math::vec2{ 4.f, 6.f });

        math::vec3 b = math::vec3{ 2.f, 3.f, 4.f };
        b += math::vec3{ 2.f, 3.f, 4.f };
        REQUIRE(b == math::vec3{ 4.f, 6.f, 8.f });

        math::vec4 c = math::vec4{ 2.f, 3.f, 4.f, 5.f };
        c += math::vec4{ 2.f, 3.f, 4.f, 5.f };
        REQUIRE(c == math::vec4{ 4.f, 6.f, 8.f, 10.f });
    }
    SECTION("vector += scalar")
    {
        math::vec2 a = math::vec2{ 2.f, 3.f };
        a += 6.f;
        REQUIRE(a == math::vec2{ 8.f, 9.f });

        math::vec3 b = math::vec3{ 2.f, 3.f, 4.f };
        b += 6.f;
        REQUIRE(b == math::vec3{ 8.f, 9.f, 10.f });

        math::vec4 c = math::vec4{ 2.f, 3.f, 4.f, 5.f };
        c += 6.f;
        REQUIRE(c == math::vec4{ 8.f, 9.f, 10.f, 11.f });
    }
}

TEST_CASE("Vector subtraction", "[math]")
{
    SECTION("vector - vector")
    {
        REQUIRE(math::vec2{ 2.f, 3.f } - math::vec2{ 2.f, 3.f } == math::vec2{ 0.f, 0.f });
        REQUIRE(math::vec3{ 2.f, 3.f, 4.f } - math::vec3{ 2.f, 3.f, 4.f } == math::vec3{ 0.f, 0.f, 0.f });
        REQUIRE(math::vec4{ 2.f, 3.f, 4.f, 5.f } - math::vec4{ 2.f, 3.f, 4.f, 5.f } ==
                math::vec4{ 0.f, 0.f, 0.f, 0.f });
    }
    SECTION("vector - scalar")
    {
        REQUIRE(math::vec2{ 2.f, 3.f } - 5.f == math::vec2{ -3.f, -2.f });
        REQUIRE(math::vec3{ 2.f, 3.f, 4.f } - 5.f == math::vec3{ -3.f, -2.f, -1.f });
        REQUIRE(math::vec4{ 2.f, 3.f, 4.f, 5.f } - 5.f == math::vec4{ -3.f, -2.f, -1.f, 0.f });
    }
    SECTION("scalar - vector")
    {
        REQUIRE(5.f - math::vec2{ 2.f, 3.f } == math::vec2{ 3.f, 2.f });
        REQUIRE(5.f - math::vec3{ 2.f, 3.f, 4.f } == math::vec3{ 3.f, 2.f, 1.f });
        REQUIRE(5.f - math::vec4{ 2.f, 3.f, 4.f, 5.f } == math::vec4{ 3.f, 2.f, 1.f, 0.f });
    }
    SECTION("vector -= vector")
    {
        math::vec2 a = math::vec2{ 2.f, 3.f };
        a -= math::vec2{ 2.f, 3.f };
        REQUIRE(a == math::vec2{ 0.f, 0.f });

        math::vec3 b = math::vec3{ 2.f, 3.f, 4.f };
        b -= math::vec3{ 2.f, 3.f, 4.f };
        REQUIRE(b == math::vec3{ 0.f, 0.f, 0.f });

        math::vec4 c = math::vec4{ 2.f, 3.f, 4.f, 5.f };
        c -= math::vec4{ 2.f, 3.f, 4.f, 5.f };
        REQUIRE(c == math::vec4{ 0.f, 0.f, 0.f, 0.f });
    }
    SECTION("vector -= scalar")
    {
        math::vec2 a = math::vec2{ 2.f, 3.f };
        a -= 1.f;
        REQUIRE(a == math::vec2{ 1.f, 2.f });

        math::vec3 b = math::vec3{ 2.f, 3.f, 4.f };
        b -= 1.f;
        REQUIRE(b == math::vec3{ 1.f, 2.f, 3.f });

        math::vec4 c = math::vec4{ 2.f, 3.f, 4.f, 5.f };
        c -= 1.f;
        REQUIRE(c == math::vec4{ 1.f, 2.f, 3.f, 4.f });
    }
}

TEST_CASE("Vector multiplication", "[math]")
{
    SECTION("vector * vector")
    {
        REQUIRE(math::vec2{ 2.f, 3.f } * math::vec2{ 2.f, 3.f } == math::vec2{ 4.f, 9.f });
        REQUIRE(math::vec3{ 2.f, 3.f, 4.f } * math::vec3{ 2.f, 3.f, 4.f } == math::vec3{ 4.f, 9.f, 16.f });
        REQUIRE(math::vec4{ 2.f, 3.f, 4.f, 5.f } * math::vec4{ 2.f, 3.f, 4.f, 5.f } ==
                math::vec4{ 4.f, 9.f, 16.f, 25.f });
    }
    SECTION("vector * scalar")
    {
        REQUIRE(math::vec2{ 2.f, 3.f } * 5.f == math::vec2{ 10.f, 15.f });
        REQUIRE(math::vec3{ 2.f, 3.f, 4.f } * 5.f == math::vec3{ 10.f, 15.f, 20.f });
        REQUIRE(math::vec4{ 2.f, 3.f, 4.f, 5.f } * 5.f == math::vec4{ 10.f, 15.f, 20.f, 25.f });
    }
    SECTION("scalar * vector")
    {
        REQUIRE(5.f * math::vec2{ 2.f, 3.f } == math::vec2{ 10.f, 15.f });
        REQUIRE(5.f * math::vec3{ 2.f, 3.f, 4.f } == math::vec3{ 10.f, 15.f, 20.f });
        REQUIRE(5.f * math::vec4{ 2.f, 3.f, 4.f, 5.f } == math::vec4{ 10.f, 15.f, 20.f, 25.f });
    }
    SECTION("vector *= vector")
    {
        math::vec2 a = math::vec2{ 2.f, 3.f };
        a *= math::vec2{ 2.f, 3.f };
        REQUIRE(a == math::vec2{ 4.f, 9.f });

        math::vec3 b = math::vec3{ 2.f, 3.f, 4.f };
        b *= math::vec3{ 2.f, 3.f, 4.f };
        REQUIRE(b == math::vec3{ 4.f, 9.f, 16.f });

        math::vec4 c = math::vec4{ 2.f, 3.f, 4.f, 5.f };
        c *= math::vec4{ 2.f, 3.f, 4.f, 5.f };
        REQUIRE(c == math::vec4{ 4.f, 9.f, 16.f, 25.f });
    }
    SECTION("vector *= scalar")
    {
        math::vec2 a = math::vec2{ 2.f, 3.f };
        a *= 3.f;
        REQUIRE(a == math::vec2{ 6.f, 9.f });

        math::vec3 b = math::vec3{ 2.f, 3.f, 4.f };
        b *= 3.f;
        REQUIRE(b == math::vec3{ 6.f, 9.f, 12.f });

        math::vec4 c = math::vec4{ 2.f, 3.f, 4.f, 5.f };
        c *= 3.f;
        REQUIRE(c == math::vec4{ 6.f, 9.f, 12.f, 15.f });
    }
}

TEST_CASE("Vector division", "[math]")
{
    SECTION("vector / vector")
    {
        REQUIRE(math::vec2{ 10.f, 15.f } / math::vec2{ 2.f, 3.f } == math::vec2{ 5.f, 5.f });
        REQUIRE(math::vec3{ 10.f, 15.f, 20.f } / math::vec3{ 2.f, 3.f, 4.f } == math::vec3{ 5.f, 5.f, 5.f });
        REQUIRE(math::vec4{ 10.f, 15.f, 20.f, 25.f } / math::vec4{ 2.f, 3.f, 4.f, 5.f } ==
                math::vec4{ 5.f, 5.f, 5.f, 5.f });
    }
    SECTION("vector / scalar")
    {
        REQUIRE(math::vec2{ 10.f, 15.f } / 5.f == math::vec2{ 2.f, 3.f });
        REQUIRE(math::vec3{ 10.f, 15.f, 20.f } / 5.f == math::vec3{ 2.f, 3.f, 4.f });
        REQUIRE(math::vec4{ 10.f, 15.f, 20.f, 25.f } / 5.f == math::vec4{ 2.f, 3.f, 4.f, 5.f });
    }
    SECTION("scalar / vector")
    {
        REQUIRE(60.f / math::vec2{ 2.f, 3.f } == math::vec2{ 30.f, 20.f });
        REQUIRE(60.f / math::vec3{ 2.f, 3.f, 4.f } == math::vec3{ 30.f, 20.f, 15.f });
        REQUIRE(60.f / math::vec4{ 2.f, 3.f, 4.f, 5.f } == math::vec4{ 30.f, 20.f, 15.f, 12.f });
    }
    SECTION("vector /= vector")
    {
        math::vec2 a = math::vec2{ 10.f, 15.f };
        a /= math::vec2{ 2.f, 3.f };
        REQUIRE(a == math::vec2{ 5.f, 5.f });

        math::vec3 b = math::vec3{ 10.f, 15.f, 20.f };
        b /= math::vec3{ 2.f, 3.f, 4.f };
        REQUIRE(b == math::vec3{ 5.f, 5.f, 5.f });

        math::vec4 c = math::vec4{ 10.f, 15.f, 20.f, 25.f };
        c /= math::vec4{ 2.f, 3.f, 4.f, 5.f };
        REQUIRE(c == math::vec4{ 5.f, 5.f, 5.f, 5.f });
    }
    SECTION("vector /= scalar")
    {
        math::vec2 a = math::vec2{ 10.f, 15.f };
        a /= 5.f;
        REQUIRE(a == math::vec2{ 2.f, 3.f });

        math::vec3 b = math::vec3{ 10.f, 15.f, 20.f };
        b /= 5.f;
        REQUIRE(b == math::vec3{ 2.f, 3.f, 4.f });

        math::vec4 c = math::vec4{ 10.f, 15.f, 20.f, 25.f };
        c /= 5.f;
        REQUIRE(c == math::vec4{ 2.f, 3.f, 4.f, 5.f });
    }
}

TEST_CASE("Vector functions", "[math]")
{
    SECTION("dot product")
    {
        REQUIRE(11.f == math::dot(math::vec2{ 1.f, 2.f }, math::vec2{ 3.f, 4.f }));
        REQUIRE(32.f == math::dot(math::vec3{ 1.f, 2.f, 3.f }, math::vec3{ 4.f, 5.f, 6.f }));
        REQUIRE(70.f == math::dot(math::vec4{ 1.f, 2.f, 3.f, 4.f }, math::vec4{ 5.f, 6.f, 7.f, 8.f }));
    }
    SECTION("min")
    {
        REQUIRE(math::min(math::vec2{ 2.f, 10.f }, math::vec2{ 3.f, 9.f }) == math::vec2{ 2.f, 9.f });
        REQUIRE(math::min(math::vec3{ 2.f, 10.f, 4.f }, math::vec3{ 3.f, 9.f, 5.f }) ==
                math::vec3{ 2.f, 9.f, 4.f });
        REQUIRE(math::min(math::vec4{ 2.f, 10.f, 4.f, 6.f }, math::vec4{ 3.f, 9.f, 5.f, 7.f }) ==
                math::vec4{ 2.f, 9.f, 4.f, 6.f });
    }
    SECTION("max")
    {
        REQUIRE(math::max(math::vec2{ 2.f, 10.f }, math::vec2{ 3.f, 9.f }) == math::vec2{ 3.f, 10.f });
        REQUIRE(math::max(math::vec3{ 2.f, 10.f, 4.f }, math::vec3{ 3.f, 9.f, 5.f }) ==
                math::vec3{ 3.f, 10.f, 5.f });
        REQUIRE(math::max(math::vec4{ 2.f, 10.f, 4.f, 6.f }, math::vec4{ 3.f, 9.f, 5.f, 7.f }) ==
                math::vec4{ 3.f, 10.f, 5.f, 7.f });
    }
    SECTION("abs")
    {
        REQUIRE(math::abs(math::vec2{ -2.f, -10.f }) == math::vec2{ 2.f, 10.f });
        REQUIRE(math::abs(math::vec3{ -2.f, -10.f, -4.f }) == math::vec3{ 2.f, 10.f, 4.f });
        REQUIRE(math::abs(math::vec4{ -2.f, -10.f, -4.f, -6.f }) == math::vec4{ 2.f, 10.f, 4.f, 6.f });
    }
    SECTION("clamp")
    {
        // scalar bounds
        REQUIRE(math::clamp(math::vec2{ 2.f, -3.f }, 0., 2) == math::vec2{ 2.f, 0.f });
        REQUIRE(math::clamp(math::vec3{ 2.f, -2.f, -0.5f }, -1.f, 0) == math::vec3{ 0.f, -1.f, -0.5f });
        REQUIRE(math::clamp(math::vec4{ 2.f, 1.f, -4.f, -0.f }, -1, 1.f) == math::vec4{ 1.f, 1.f, -1.f, 0.f });
        // vector bounds
        REQUIRE(math::clamp(math::vec2{ 2.f, -3.f }, math::vec2{ 0.f, -4.f }, math::vec2{ 1.f, -2.f }) ==
                math::vec2{ 1.f, -3.f });
    }
    SECTION("lerp")
    {
        REQUIRE(math::lerp(math::vec2{ 2.f, 2.f }, math::vec2{ 4.f, 4.f }, 0.5f) == math::vec2{ 3.f, 3.f });
    }
    SECTION("saturate")
    {
        REQUIRE(math::saturate(math::vec3{ 2.f, -2.f, -0.5f }) == math::vec3{ 1.f, 0.f, 0.f });
    }
    SECTION("min_component")
    {
        REQUIRE(math::min_component(math::vec2i{ 5, 2 }) == 2);
        REQUIRE(math::min_component(math::vec3{ 2.f, 5.f, -0.5f }) == -0.5f);
    }
    SECTION("max_component")
    {
        REQUIRE(math::max_component(math::vec2i{ 2, -3 }) == 2);
        REQUIRE(math::max_component(math::vec3{ 2.f, -2.f, -0.5f }) == 2.f);
    }
    SECTION("all")
    {
        REQUIRE(math::all(math::vec<bool, 2>{ true, false }) == false);
        REQUIRE(math::all(math::vec<bool, 3>{ false, false, false }) == false);
        REQUIRE(math::all(math::vec<bool, 4>{ true, true, true, true }) == true);
    }
    SECTION("any")
    {
        REQUIRE(math::any(math::vec<bool, 2>{ true, false }) == true);
        REQUIRE(math::any(math::vec<bool, 3>{ false, false, false }) == false);
        REQUIRE(math::any(math::vec<bool, 4>{ true, true, true, true }) == true);
    }

    SECTION("compare")
    {
        REQUIRE(math::compare(math::vec2{ 2.f, 3.f }, math::vec2{ 1.f, 3.f }) ==
                math::vec<bool, 2>{ false, true });
        REQUIRE(math::compare(math::vec2i{ 1, 4 }, math::vec2i{ 1, 2 }) == math::vec<bool, 2>{ true, false });
    }
}
