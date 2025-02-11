#include <cstddef>
#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include <print>    // std::print
#include <random>

#include "math/mathlib.hpp"
#include "constant/constantlib.hpp"     // constants::INF
#include "graph/nodelib.hpp"

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
    SECTION ("Small", "[main]")
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
        auto [ret_graph, ret_val] = reduce_graph(graph);
        REQUIRE (ret_val == ans_val);
        REQUIRE (ret_graph == ans_graph);
    }

    SECTION ("Reduced", "[main]")
    {
        const std::vector<std::vector<double>> graph =
        {
            {constants::INF, constants::INF, constants::INF, constants::INF, constants::INF},
            {constants::INF, constants::INF, 11, 2, 0},
            {0, constants::INF, constants::INF, 0, 2},
            {15, constants::INF, 12, constants::INF, 0},
            {11, constants::INF, 0, 12, constants::INF}
        };
        const double ans_val = 0;
        auto [ret_graph, ret_val] = reduce_graph(graph);
        REQUIRE (ret_val == ans_val);
        REQUIRE (ret_graph == graph);
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

TEST_CASE("K Mean", "[main]")
{
    SECTION ("3 Clusters", "[main]")
    {
        std::vector<graph::Coordinate> coordinates =
        {
            {200, 57},
            {202, 60},
            {219, 63},
            {20, 100},
            {25, 110}
        };

        std::vector<graph::Coordinate> clusters =
        {
            {20, 100},
            {25, 110},
            {207, 60}
        };

        const size_t K = 3;
        const size_t T = 10;

        // calls the functions T times and picks the results that has the lowest variance value
        std::vector<double> vars(T, constants::INF);
        std::vector<std::vector<graph::Coordinate>> rets(T);
        for (size_t i = 0; i < T; ++i)
        {
            auto [a, b] = k_means(coordinates, K, 500);
            rets[i] = a;
            vars[i] = b;
        }

        // finds the best clustering by finding the group that has the smallest variance
        auto itr = std::min_element(vars.cbegin(), vars.cend());
        double ret_var = *itr;
        auto ret_clusters = *(rets.begin() + (itr - vars.cbegin()));

        // calculates the variance of the provided answer
        double ans_var = 0.0;
        ans_var += distance(coordinates[0], clusters[2]);
        ans_var += distance(coordinates[1], clusters[2]);
        ans_var += distance(coordinates[2], clusters[2]);
        ans_var += distance(coordinates[3], clusters[0]);
        ans_var += distance(coordinates[5], clusters[1]);

        REQUIRE (ret_clusters.size() == K);
        REQUIRE (ret_var <= ans_var);

        #if DEBUG
        std::cout << "Centroids from calling K Means function:\n";
        for (auto& c : ret_clusters)
        {
            std::print("({}, {})\t", c.x, c.y);
        }
        std::cout << "\n";
        #endif
    }

    SECTION ("5 Clusters", "[main]")
    {
        std::vector<graph::Coordinate> clusters =
        {
            {20, 10},
            {2, 110},
            {60, 180},
            {580, 600},
            {1100, 2000}
        };

        const size_t K = clusters.size();
        const size_t T = 50;
        const size_t N0 = 3;
        const size_t N1 = 1;
        const size_t N2 = 5;
        const size_t N3 = 12;
        const size_t N4 = 8;
        const size_t Ns[] = {N0, N1, N2, N3, N4};
        const size_t N = N0 + N1 + N2 + N3 + N4;

        std::vector<graph::Coordinate> coordinates;

        // creates the generator
        std::random_device rd{};
        std::mt19937 gen{rd()};

        // creates normal distributions
        std::vector<std::normal_distribution<double>> distXs;
        std::vector<std::normal_distribution<double>> distYs;
        for (size_t i = 0; i < K; ++i)
        {
            distXs.emplace_back(clusters[i].x, 2.0);
            distYs.emplace_back(clusters[i].y, 2.0);
        }

        // generates coordinates based on the predefined centroids points
        for (size_t i = 0; i < clusters.size(); ++i)
        {
            // generates x number of points that are centered at cluster[i]
            for (size_t j = 0; j < Ns[i]; ++j)
            {
                coordinates.emplace_back(distXs[i](gen), distYs[i](gen));
            }
        }

        // calls the functions T times and picks the results that has the lowest variance value
        std::vector<double> vars(T, constants::INF);
        std::vector<std::vector<graph::Coordinate>> rets(T);
        for (size_t i = 0; i < T; ++i)
        {
            auto [a, b] = k_means(coordinates, K, 500);
            rets[i] = a;
            vars[i] = b;
        }

        // finds the best clustering by finding the group that has the smallest variance
        auto itr = std::min_element(vars.cbegin(), vars.cend());
        double ret_var = *itr;
        auto ret_clusters = *(rets.begin() + (itr - vars.cbegin()));

        // calculates the variance of the provided answer
        double ans_var = 0.0;
        for (size_t i = 0, idx = 0; i < clusters.size(); ++i)
        {
            for (size_t j = 0; j < Ns[i]; ++j)
            {
                ans_var += distance(coordinates[idx + j], clusters[i]);
            }
            idx += Ns[i];
        }

        REQUIRE (coordinates.size() == N);
        REQUIRE (ret_clusters.size() == K);
        REQUIRE (ret_var <= ans_var);

        #if DEBUG
        std::cout << "Centroids from calling K Means function:\n";
        for (auto& c : ret_clusters)
        {
            std::print("({}, {})\t", c.x, c.y);
        }
        std::cout << "\n";
        #endif
    }
}
