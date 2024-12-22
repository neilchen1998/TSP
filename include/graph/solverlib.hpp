#ifndef INCLUDE_GRAPH_SOLVERLIB_H_
#define INCLUDE_GRAPH_SOLVERLIB_H_

#include <vector>   // std::vector

namespace graph
{
    double brute_force(const std::vector<std::vector<double>>& graph);

    double branch_and_bound(const std::vector<std::vector<double>> &graph);

    /// @brief Explore a new node in the branch and bound algorithm
    /// @param graph The start graph
    /// @param start The start index
    /// @param goal The goal index
    /// @return {the goal graph, the cost}
    std::tuple< std::vector<std::vector<double>>, double> explore_new_node(const std::vector<std::vector<double>> &graph, const size_t start, const size_t goal);
};  // namespace graph

#endif // INCLUDE_GRAPH_SOLVERLIB_H_
