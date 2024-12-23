#ifndef INCLUDE_GRAPH_SOLVERLIB_H_
#define INCLUDE_GRAPH_SOLVERLIB_H_

#include <vector>   // std::vector

#include <boost/dynamic_bitset.hpp> // boost::dynamic_bitset

namespace graph
{
    namespace solver
    {
        double brute_force(const std::vector<std::vector<double>>& graph);

        double branch_and_bound(const std::vector<std::vector<double>> &graph);
    };  // namespace solver

    /// @brief Explore a new node in the branch and bound algorithm
    /// @param graph The start graph
    /// @param start The start index
    /// @param goal The goal index
    /// @return {the goal graph, the cost}
    std::tuple< std::vector<std::vector<double>>, double> explore_new_node(const std::vector<std::vector<double>> &graph, const size_t start, const size_t goal);

    /// @brief Find an unvisited node
    /// @param visited The bitset that entails all visited nodes
    /// @return An unvisited node, if all node have been visited, then std::nullopt
    std::optional<size_t> find_unvisited_node(const boost::dynamic_bitset<>& visited);
};  // namespace graph

#endif // INCLUDE_GRAPH_SOLVERLIB_H_
