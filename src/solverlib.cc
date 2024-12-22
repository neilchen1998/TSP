#include "graph/solverlib.hpp"

#include <vector>   // std::vector
#include <iostream> // std::cout

#include "math/mathlib.hpp"
#include "constant/constantlib.hpp"     // constants::INF
#include "graph/visualizerlib.hpp"

double graph::brute_force(const std::vector<std::vector<double>> &graph)
{
    const size_t N = graph.size();

    // gets all permutations
    auto per = generate_permutation(N);

    #if DEBUG
    std::cout << "All possible paths:\n";
    for (const auto& path : per)
    {
        std::cout << path << std::endl;
    }
    #endif

    // loops through all permutations
    double ret = (double)INT_MAX;
    for (size_t i = 0; i < per.size(); ++i)
    {
        double cur = 0.0;
        for (size_t j = 0; j < N; ++j)
        {
            int from = per[i][j % N] - 'a';
            int to = per[i][(j + 1) % N] - 'a';

            cur += graph[from][to];
        }

        ret = std::min(cur, ret);
    }

    return ret;
}

std::tuple< std::vector<std::vector<double>>, double> graph::explore_new_node(const std::vector<std::vector<double>> &graph, const size_t start, const size_t end)
{
    double ret = constants::INF;

    auto [curGraph, curReducedVal] = reduce_graph(graph);
    double curCost = curGraph[start][end]; // get the cost from node A to node B
    curGraph[end][start] = constants::INF; // makes the weight from the next node to the previous node inf
    set_row_col_inf(curGraph, start, end);
    auto [nextGraph, nextReducedVal] = reduce_graph(curGraph);

    ret = curCost + curReducedVal + nextReducedVal;

    return {nextGraph, ret};
}

