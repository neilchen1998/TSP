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
            {constants::INF, 10, 17, 0, 1},
            {12, constants::INF, 11, 2, 0},
            {0, 3, constants::INF, 0, 2},
            {15, 3, 12, constants::INF, 0},
            {11, 0, 0, 12, constants::INF}
        };
        const double prevCost = 25;
        const double ans_val = 35;
        const std::vector<std::vector<double>> ans_graph =
        {
            {constants::INF, constants::INF, constants::INF, constants::INF, constants::INF},
            {constants::INF, constants::INF, 11, 2, 0},
            {0, constants::INF, constants::INF, 0, 2},
            {15, constants::INF, 12, constants::INF, 0},
            {11, constants::INF, 0, 12, constants::INF}
        };
        const size_t from = 0, to = 1;
        auto [ret_graph, ret_val] = graph::explore_new_node(graph, from, to, prevCost);
        CHECK (ret_val == ans_val);
        CHECK (ret_graph == ans_graph);
    }

    SECTION ("Explore a new node", "[main]")
    {
        const std::vector<std::vector<double>> graph =
        {
            {constants::INF, 10, 17, 0, 1},
            {12, constants::INF, 11, 2, 0},
            {0, 3, constants::INF, 0, 2},
            {15, 3, 12, constants::INF, 0},
            {11, 0, 0, 12, constants::INF}
        };
        const double prevCost = 25;
        const double ans_val = 53;
        const std::vector<std::vector<double>> ans_graph =
        {
            {constants::INF, constants::INF, constants::INF, constants::INF, constants::INF},
            {1, constants::INF, constants::INF, 2, 0},
            {constants::INF, 3, constants::INF, 0, 2},
            {4, 3, constants::INF, constants::INF, 0},
            {0, 0, constants::INF, 12, constants::INF}
        };
        const size_t from = 0, to = 2;
        auto [ret_graph, ret_val] = graph::explore_new_node(graph, from, to, prevCost);
        CHECK (ret_val == ans_val);
        CHECK (ret_graph == ans_graph);
    }

    SECTION ("Explore a new node", "[main]")
    {
        const std::vector<std::vector<double>> graph =
        {
            {constants::INF, 10, 17, 0, 1},
            {12, constants::INF, 11, 2, 0},
            {0, 3, constants::INF, 0, 2},
            {15, 3, 12, constants::INF, 0},
            {11, 0, 0, 12, constants::INF}
        };
        const double prevCost = 25;
        const double ans_val = 25;
        const std::vector<std::vector<double>> ans_graph =
        {
            {constants::INF, constants::INF, constants::INF, constants::INF, constants::INF},
            {12, constants::INF, 11, constants::INF, 0},
            {0, 3, constants::INF, constants::INF, 2},
            {constants::INF, 3, 12, constants::INF, 0},
            {11, 0, 0, constants::INF, constants::INF}
        };
        const size_t from = 0, to = 3;
        auto [ret_graph, ret_val] = graph::explore_new_node(graph, from, to, prevCost);
        CHECK (ret_val == ans_val);
        CHECK (ret_graph == ans_graph);
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
