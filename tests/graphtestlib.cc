#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include <vector>   // std::vector

#include "graph/solverlib.hpp"

TEST_CASE("Brute Force", "[main]")
{
    std::vector<std::vector<double>> graph =
    {
        {0, 4, 1, 9},
        {3, 0, 6, 11},
        {4, 1, 0, 2},
        {6, 5, -4, 0},
    };

    SECTION ("", "[main]")
    {
        auto ans = graph::brute_force(graph);
        REQUIRE (ans == 9);
    }
}
