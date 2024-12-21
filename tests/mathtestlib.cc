#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>

#include "math/mathlib.hpp"
#include "constant/constantlib.hpp"     // constants::INF

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

TEST_CASE("Reduce Graph", "[main]")
{
    const std::vector<std::vector<double>> graph =
    {
        {constants::INF, 20, 30, 10, 11},
        {15, constants::INF, 16, 4, 2},
        {3, 5, constants::INF, 2, 4},
        {19, 6, 18, constants::INF, 3},
        {16, 4, 7, 16, constants::INF}
    };
    const double ans_val = 25;
    const std::vector<std::vector<double>> ans_graph =
    {
        {constants::INF, 10, 17, 0, 1},
        {12, constants::INF, 11, 2, 0},
        {0, 3, constants::INF, 0, 2},
        {15, 3, 12, constants::INF, 0},
        {11, 0, 0, 12, constants::INF}
    };

    SECTION ("Small", "[main]")
    {
        auto [ret_graph, ret_val] = reduce_graph(graph);
        REQUIRE (ret_val == ans_val);
        REQUIRE (ret_graph == ans_graph);
    }
}

TEST_CASE("Set Row and Column to Inf", "[main]")
{
    std::vector<std::vector<double>> graph =
    {
        {constants::INF, 20, 30, 10, 11},
        {15, constants::INF, 16, 4, 2},
        {3, 5, constants::INF, 2, 4},
        {19, 6, 18, constants::INF, 3},
        {16, 4, 7, 16, constants::INF}
    };

    const std::vector<std::vector<double>> ans_graph =
    {
        {constants::INF, constants::INF, constants::INF, constants::INF, constants::INF},
        {15, constants::INF, 16, 4, 2},
        {3, constants::INF, constants::INF, 2, 4},
        {19, constants::INF, 18, constants::INF, 3},
        {16, constants::INF, 7, 16, constants::INF}
    };

    SECTION ("Small", "[main]")
    {
        const size_t row = 0, col = 1;
        set_row_col_inf(graph, row, col);
        REQUIRE (graph == ans_graph);
    }
}
