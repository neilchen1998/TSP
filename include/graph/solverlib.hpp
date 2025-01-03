#ifndef INCLUDE_GRAPH_SOLVERLIB_H_
#define INCLUDE_GRAPH_SOLVERLIB_H_

#include <cstddef>
#include <vector>   // std::vector
#include <optional> // std::optional

#include <boost/dynamic_bitset.hpp> // boost::dynamic_bitset

namespace graph
{
    namespace solver
    {
        double brute_force(const std::vector<std::vector<double>>& graph);

        /// @brief Solve the TSP with Branch and Bound algorithm
        /// @param graph The weight graph
        /// @param maxIteration The maximum number of iterations before time-out
        /// @return {the shortest path, the min. cost}
        std::tuple<std::vector<size_t>, double> branch_and_bound(const std::vector<std::vector<double>> &graph, const size_t maxIteration = 9999);

        /// @brief Solve the TSP with Depth First Search algorithm
        /// @param graph The weight graph
        /// @return {the shortest path, the min. cost}
        std::tuple<std::vector<size_t>, double> DFS(const std::vector<std::vector<double>> &graph);
    };  // namespace solver

    /// @brief Explore a new node in the branch and bound algorithm
    /// @param graph The start graph
    /// @param from The current index
    /// @param to The next index
    /// @param prevCost The cost from the previous graph
    /// @param start The start index of the journey
    /// @return {the goal graph, the cost}
    std::tuple< std::vector<std::vector<double>>, double> explore_new_node(const std::vector<std::vector<double>> &graph, const size_t from, const size_t to, double prevCost = 0.0, const size_t start = 0);

    /// @brief Find an unvisited node
    /// @param visited The bitset that entails all visited nodes
    /// @return An unvisited node, if all node have been visited, then std::nullopt
    std::optional<size_t> find_unvisited_node(const boost::dynamic_bitset<>& visited);
};  // namespace graph

#endif // INCLUDE_GRAPH_SOLVERLIB_H_
