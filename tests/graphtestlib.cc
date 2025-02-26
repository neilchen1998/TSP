#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include <vector>   // std::vector
#include <unordered_set>    // std::unordered_set
#include <numeric>    // std::iota

#include "constant/constantlib.hpp"     // constants::INF
#include "graph/solverlib.hpp"
#include "filesystem/loadlib.hpp"   // create_graph

/// @brief Create an unordered set that has elements from 0 to (N - 1)
/// @param N The total number of elements
/// @return An unordered set that has all the elmeents from 0 to (N - 1)
inline std::unordered_set<size_t> create_visited_set(const size_t N)
{
    std::unordered_set<size_t> visited;
    std::vector<int> l(N);
    std::iota(l.begin(), l.end(), 0);
    visited.insert(l.begin(), l.end());

    return visited;
}

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
        auto [_, ret_cost] = graph::solver::brute_force(graph);
        CHECK (ret_cost == 9);
    }
}

TEST_CASE("Find an unvisited node", "[main]")
{
    SECTION ("At least one unvisited node", "[main]")
    {
        const size_t ans = 3;
        boost::dynamic_bitset<> visited(5, 0b00111);
        auto ret = graph::find_unvisited_node(visited).value_or(0);
        CHECK (ret == ans);
    }

    SECTION ("No unvisited node", "[main]")
    {
        boost::dynamic_bitset<> visited(10, 0b1111111111);
        auto ret = graph::find_unvisited_node(visited);
        auto ans = (ret == std::nullopt) ? true : false;
        CHECK (ans);
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

    SECTION ("Solver", "[main]")
    {
        const std::vector<std::vector<double>> graph =
        {
            {constants::INF, 4, 1, 9},
            {3, constants::INF, 6, 11},
            {4, 1, constants::INF, 2},
            {6, 5, -4, constants::INF},
        };
        const std::vector<size_t> ans_path = {0, 3, 2, 1};
        const double ans_cost = 9;
        auto [ret_path, ret_cost] = graph::solver::branch_and_bound(graph);
        CHECK (ret_path == ans_path);
        CHECK (ret_cost == ans_cost);
    }

    SECTION ("Solver", "[main]")
    {
        const std::vector<std::vector<double>> graph =
        {
            {constants::INF, 20, 30, 10, 11},
            {15, constants::INF, 16, 4, 2},
            {3, 5, constants::INF, 2, 4},
            {19, 6, 18, constants::INF, 3},
            {16, 4, 7, 16, constants::INF}
        };
        const std::vector<size_t> ans_path = {0, 3, 1, 4, 2};
        const double ans_cost = 28;
        auto [ret_path, ret_cost] = graph::solver::branch_and_bound(graph);
        CHECK (ret_path == ans_path);
        CHECK (ret_cost == ans_cost);
    }

    SECTION ("Solver", "[main]")
    {
        // source: https://developers.google.com/optimization/routing/tsp#c++
        const std::vector<std::vector<double>> graph{
              {0, 2451, 713, 1018, 1631, 1374, 2408, 213, 2571, 875, 1420, 2145, 1972},
              {2451, 0, 1745, 1524, 831, 1240, 959, 2596, 403, 1589, 1374, 357, 579},
              {713, 1745, 0, 355, 920, 803, 1737, 851, 1858, 262, 940, 1453, 1260},
              {1018, 1524, 355, 0, 700, 862, 1395, 1123, 1584, 466, 1056, 1280, 987},
              {1631, 831, 920, 700, 0, 663, 1021, 1769, 949, 796, 879, 586, 371},
              {1374, 1240, 803, 862, 663, 0, 1681, 1551, 1765, 547, 225, 887, 999},
              {2408, 959, 1737, 1395, 1021, 1681, 0, 2493, 678, 1724, 1891, 1114, 701},
              {213, 2596, 851, 1123, 1769, 1551, 2493, 0, 2699, 1038, 1605, 2300, 2099},
              {2571, 403, 1858, 1584, 949, 1765, 678, 2699, 0, 1744, 1645, 653, 600},
              {875, 1589, 262, 466, 796, 547, 1724, 1038, 1744, 0, 679, 1272, 1162},
              {1420, 1374, 940, 1056, 879, 225, 1891, 1605, 1645, 679, 0, 1017, 1200},
              {2145, 357, 1453, 1280, 586, 887, 1114, 2300, 653, 1272, 1017, 0, 504},
              {1972, 579, 1260, 987, 371, 999, 701, 2099, 600, 1162, 1200, 504, 0},
          };
        const std::vector<size_t> ans_path = {0, 7, 2, 3, 4, 12, 6, 8, 1, 11, 10, 5, 9};
        const double ans_cost = 7293;
        auto [ret_path, ret_cost] = graph::solver::branch_and_bound(graph);
        CHECK (ret_path == ans_path);
        CHECK (ret_cost == ans_cost);
    }
}

TEST_CASE("Dynamic Programming", "[main]")
{
    SECTION ("Solver", "[main]")
    {
        std::vector<std::vector<double>> graph =
        {
            {0, 4, 1, 9},
            {3, 0, 6, 11},
            {4, 1, 0, 2},
            {6, 5, -4, 0},
        };
        auto [_, ret_cost] = graph::solver::DFS(graph);
        CHECK (ret_cost == 9);
    }

    SECTION ("Solver", "[main]")
    {
        const std::vector<std::vector<double>> graph =
        {
            {constants::INF, 20, 30, 10, 11},
            {15, constants::INF, 16, 4, 2},
            {3, 5, constants::INF, 2, 4},
            {19, 6, 18, constants::INF, 3},
            {16, 4, 7, 16, constants::INF}
        };
        const std::vector<size_t> ans_path = {0, 3, 1, 4, 2};
        const double ans_cost = 28;
        auto [ret_path, ret_cost] = graph::solver::DFS(graph);
        CHECK (ret_path == ans_path);
        CHECK (ret_cost == ans_cost);
    }
}

TEST_CASE("Divide and Conquer", "[main]")
{
    SECTION ("Small graph", "[main]")
    {
        const std::vector<graph::Coordinate> nodes {
            {40.7128, 74.0060},
            {34.0522, 118.2437},
            {41.8781, 87.6298},
            {44.9800, 93.2638}
        };
        const size_t N = nodes.size();
        const auto graph = create_graph(nodes);
        auto [ans_path, ans_cost] = graph::solver::DFS(graph);
        auto [ret_path, ret_cost] = graph::solver::divide_n_conquer(nodes, graph, 2);

        // makes sure all nodes have been visited
        REQUIRE (ret_path.size() == N);
        std::unordered_set<size_t> ret_visited;
        const auto ans_visited = create_visited_set(N);
        for (auto& p : ret_path)
        {
            ret_visited.insert(p);
        }
        REQUIRE (ret_visited == ans_visited);

        CHECK (ret_cost == ans_cost);
    }

    SECTION ("Large graph", "[main]")
    {
        // source: https://developers.google.com/optimization/routing/tsp#c++
        // New York: 40.7128° N, 74.0060° W
        // Los Angeles: 34.0522° N, 118.2437° W
        // Chicago: 41.8781° N, 87.6298° W
        // Minneapolis: 44.9800° N, 93.2638° W
        // Denver: 39.7392° N, 104.9903° W
        // Dallas: 32.7157° N, 96.7989° W
        // Seattle: 47.6062° N, 122.3321° W
        // Boston: 42.3601° N, 71.0589° W
        // San Francisco: 37.7749° N, 122.4194° W
        // St. Louis: 38.6270° N, 90.1994° W
        // Houston: 29.7604° N, 95.3698° W
        // Phoenix: 33.4484° N, 112.0740° W
        // Salt Lake City: 40.7608° N, 111.8923° W
        // since those cities are all located in the north western part of the world, those coordinates are simplified as x-y coordinates
        const std::vector<graph::Coordinate> nodes {
            {40.7128, 74.0060},
            {34.0522, 118.2437},
            {41.8781, 87.6298},
            {44.9800, 93.2638},
            {39.7392, 104.9903},
            {32.7157, 96.7989},
            {47.6062, 122.3321},
            {42.3601, 71.0589},
            {37.7749, 122.4194},
            {38.6270, 90.1994},
            {29.7604, 95.3698},
            {33.4484, 112.0740},
            {40.7608, 111.8923}
        };

        const auto graph = create_graph(nodes);

        const size_t N = nodes.size();

        // the optimal route
        const int ans_path[] = {0, 7, 2, 3, 4, 12, 6, 8, 1, 11, 10, 5, 9, 0};
        double ans_cost = 0.0;
        for (size_t i = 0; i + 1 < 14; ++i)
        {
            auto u = ans_path[i];
            auto v = ans_path[i + 1];
            ans_cost += graph[u][v];
        }

        {
            auto [ret_path, ret_cost] = graph::solver::divide_n_conquer(nodes, graph, 3);

            // makes sure all nodes have been visited
            REQUIRE (ret_path.size() == N);
            std::unordered_set<size_t> ret_visited;
            const auto ans_visited = create_visited_set(N);
            for (auto& p : ret_path)
            {
                ret_visited.insert(p);
            }
            REQUIRE (ret_visited == ans_visited);

            // checks if the cost calculated by solver::divide_n_conquer is within 30%
            CHECK (ret_cost <= ans_cost * 1.3);
        }
        
        {
            auto [ret_path, ret_cost] = graph::solver::divide_n_conquer(nodes, graph, 5);

            // makes sure all nodes have been visited
            REQUIRE (ret_path.size() == N);
            std::unordered_set<size_t> ret_visited;
            const auto ans_visited = create_visited_set(N);
            for (auto& p : ret_path)
            {
                ret_visited.insert(p);
            }
            REQUIRE (ret_visited == ans_visited);

            // checks if the cost calculated by solver::divide_n_conquer is within 30%
            CHECK (ret_cost <= ans_cost * 1.3);
        }

        {
            auto [ret_path, ret_cost] = graph::solver::divide_n_conquer(nodes, graph, 7);

            // makes sure all nodes have been visited
            REQUIRE (ret_path.size() == N);
            std::unordered_set<size_t> ret_visited;
            const auto ans_visited = create_visited_set(N);
            for (auto& p : ret_path)
            {
                ret_visited.insert(p);
            }
            REQUIRE (ret_visited == ans_visited);

            // checks if the cost calculated by solver::divide_n_conquer is within 30%
            CHECK (ret_cost <= ans_cost * 1.3);
        }
    }
}
