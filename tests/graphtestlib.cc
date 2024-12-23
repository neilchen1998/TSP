#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include <vector>   // std::vector

#include "constant/constantlib.hpp"     // constants::INF
#include "graph/solverlib.hpp"

TEST_CASE("Brute Force", "[main]")
{

    SECTION ("", "[main]")
    {
        std::vector<std::vector<double>> graph =
        {
            {0, 4, 1, 9},
            {3, 0, 6, 11},
            {4, 1, 0, 2},
            {6, 5, -4, 0},
        };
        auto ans = graph::solver::brute_force(graph);
        REQUIRE (ans == 9);
    }
}

TEST_CASE("Branch and Bound", "[main]")
{
    SECTION ("Explore a new node", "[main]")
    {
        const std::vector<std::vector<double>> graph =
        {
            {constants::INF, 20, 30, 10, 11},
            {15, constants::INF, 16, 4, 2},
            {3, 5, constants::INF, 2, 4},
            {19, 6, 18, constants::INF, 3},
            {16, 4, 7, 16, constants::INF}
        };
        const double ans_val = 35;
        const std::vector<std::vector<double>> ans_graph =
        {
            {constants::INF, constants::INF, constants::INF, constants::INF, constants::INF},
            {constants::INF, constants::INF, 11, 2, 0},
            {0, constants::INF, constants::INF, 0, 2},
            {15, constants::INF, 12, constants::INF, 0},
            {11, constants::INF, 0, 12, constants::INF}
        };
        const size_t start = 0, goal = 1;
        auto [ret_graph, ret_val] = graph::explore_new_node(graph, start, goal);
        REQUIRE (ret_val == ans_val);
        REQUIRE (ret_graph == ans_graph);
    }

    SECTION ("Explore a new node", "[main]")
    {
        const std::vector<std::vector<double>> graph =
        {
            {constants::INF, 20, 30, 10, 11},
            {15, constants::INF, 16, 4, 2},
            {3, 5, constants::INF, 2, 4},
            {19, 6, 18, constants::INF, 3},
            {16, 4, 7, 16, constants::INF}
        };
        const double ans_val = 53;
        const std::vector<std::vector<double>> ans_graph =
        {
            {constants::INF, constants::INF, constants::INF, constants::INF, constants::INF},
            {1, constants::INF, constants::INF, 2, 0},
            {constants::INF, 3, constants::INF, 0, 2},
            {4, 3, constants::INF, constants::INF, 0},
            {0, 0, constants::INF, 12, constants::INF}
        };
        const size_t start = 0, goal = 2;
        auto [ret_graph, ret_val] = graph::explore_new_node(graph, start, goal);
        REQUIRE (ret_val == ans_val);
        REQUIRE (ret_graph == ans_graph);
    }
}

TEST_CASE("Find an unvisited node", "[main]")
{
    SECTION ("At least one unvisited node", "[main]")
    {
        const size_t ans = 3;
        boost::dynamic_bitset<> visited(5, 0b00111);
        auto ret = graph::find_unvisited_node(visited).value_or(0);
        REQUIRE (ret == ans);
    }

    SECTION ("No unvisited node", "[main]")
    {
        boost::dynamic_bitset<> visited(10, 0b1111111111);
        auto ret = graph::find_unvisited_node(visited);
        auto ans = (ret == std::nullopt) ? true : false;
        REQUIRE (ans);
    }
}
