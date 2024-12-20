#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>

#include "math/mathlib.hpp"

TEST_CASE("Dummy", "[main]")
{
    SECTION ("Different Node", "[main]")
    {
        REQUIRE (5.0 == 5);
    }
}
