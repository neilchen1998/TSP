#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>

#include "math/mathlib.hpp"

TEST_CASE("Permutation", "[main]")
{
    SECTION ("Small", "[main]")
    {
        size_t N = 4;
        size_t sz = 24;

        auto ret = generate_permutation(N);
        REQUIRE (ret.size() == sz);
    }

    SECTION ("Large", "[main]")
    {
        size_t N = 10;
        size_t sz = 3628800;

        auto ret = generate_permutation(N);
        REQUIRE (ret.size() == sz);
    }
}
