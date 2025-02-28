#ifndef INCLUDE_GRAPH_NODELIB_H_
#define INCLUDE_GRAPH_NODELIB_H_

#include <cmath>
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

        Coordinate() : x(0.0), y(0.0)
        {

        }

        Coordinate(int x, int y) : x(x), y(y)
        {

        }

        Coordinate(double x, double y) : x(x), y(y)
        {

        }

        bool operator==(const Coordinate& other) const
        {
            return (this->x == other.x) && (this->y == other.y);
        }

        bool operator!=(const Coordinate& other) const
        {
            return (this->x != other.x) || (this->y != other.y);
        }

        friend std::ostream& operator<<(std::ostream& os, const Coordinate& coord)
        {
            os << "(" << coord.x << ", " << coord.y << ")";
            return os;
        }
    };

    /// @brief The node used in the Branch and Bound algorithm
    class Node
    {
    public:
        Node(std::vector<std::vector<double>>&& graph, double cost);
        Node(size_t parentIdx, size_t childIdx, boost::dynamic_bitset<> visitedParent, std::vector<std::vector<double>>&& graph, double cost, const std::vector<size_t>& parentPath);
        ~Node() = default;

        double GetCost() const;

        bool IsCompleted() const;

        std::optional<size_t> Next() const;

        std::vector<std::vector<double>> GetGraph() const;

        boost::dynamic_bitset<> GetVisited() const;

        std::tuple<size_t, size_t> GetIndexes() const;

        std::vector<size_t> GetPath() const;

        void PrintPath() const;

    private:
        size_t parentIdx;
        size_t curIdx;
        boost::dynamic_bitset<> visited;
        std::vector<std::vector<double>> graph;
        double cost;
        std::vector<size_t> path;
    };

};  // namespace graph

#endif // INCLUDE_GRAPH_NODELIB_H_
