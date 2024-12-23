#ifndef INCLUDE_GRAPH_NODELIB_H_
#define INCLUDE_GRAPH_NODELIB_H_

#include <vector>   // std::vector

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
        Node();
        Node(size_t startIdx, size_t parentIdx, size_t curIdx, boost::dynamic_bitset<> visited);
        ~Node();

        double GetCost() const;

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
