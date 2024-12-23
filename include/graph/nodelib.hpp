#ifndef INCLUDE_GRAPH_NODELIB_H_
#define INCLUDE_GRAPH_NODELIB_H_

#include <vector>   // std::vector
#include <iostream> // std::cout
#include <optional> // std::optional

#include <boost/dynamic_bitset.hpp> // boost::dynamic_bitset

namespace graph
{
    struct Coordinate
    {
        double x;
        double y;
    };

    /// @brief The node used in the Branch and Bound algorithm
    class Node
    {
    public:
        Node(size_t N, std::vector<std::vector<double>> graph);
        Node(size_t parentIdx, size_t curIdx, boost::dynamic_bitset<> visitedParent, std::vector<std::vector<double>>&& graph, double cost, size_t startIdx = 0);
        ~Node();

        double GetCost() const;

        bool IsCompleted() const;

        std::optional<size_t> Next() const;

        std::vector<std::vector<double>> GetGraph() const;

        boost::dynamic_bitset<> GetVisited() const;

        size_t GetCurrentIdx() const;

    private:
        size_t startIdx;
        size_t parentIdx;
        size_t curIdx;
        boost::dynamic_bitset<> visited;
        std::vector<std::vector<double>> graph;
        bool isLeaf;
        double cost;
    };

};  // namespace graph

#endif // INCLUDE_GRAPH_NODELIB_H_
