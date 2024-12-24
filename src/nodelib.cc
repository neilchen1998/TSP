#include "graph/nodelib.hpp"

#include <vector>   // std::vector
#include <iostream> // std::cout
#include <optional> // std::optional

#include <boost/dynamic_bitset.hpp> // boost::dynamic_bitset

#include "constant/constantlib.hpp"     // constants::INF
#include "graph/nodelib.hpp"

graph::Node::Node(size_t N, std::vector<std::vector<double>> graph) : graph(graph)
{
    startIdx = 0;
    parentIdx = 0;
    curIdx = 0;
    isLeaf = true;
    cost = constants::INF;
    visited.resize(N);
    visited.reset();
    visited[0] = 1;
}

graph::Node::Node(size_t parentIdx, size_t childIdx, boost::dynamic_bitset<> visitedParent, std::vector<std::vector<double>>&& graph, double cost, size_t startIdx) :
    startIdx(startIdx),
    parentIdx(parentIdx),
    curIdx(childIdx),
    graph(std::move(graph)),
    cost(cost)
{
    isLeaf = true;

    visited = visitedParent;
    visited[childIdx] = 1;
}

graph::Node::~Node()
{

}

double graph::Node::GetCost() const
{
    return this->cost;
}

bool graph::Node::IsCompleted() const
{
    return this->visited.all();
}

std::optional<size_t> graph::Node::Next() const
{
    // loops through the bitset from the most unsignificant bit
    for (boost::dynamic_bitset<>::size_type i = 0; i < visited.size(); ++i)
    {
        if (visited[i] == 0)
        {
            return (size_t)i;
        }
    }

    // we cannot find an unvisited node
    // all nodes are visited
    return std::nullopt;
}

std::vector<std::vector<double>> graph::Node::GetGraph() const
{
    return this->graph;
}

boost::dynamic_bitset<> graph::Node::GetVisited() const
{
    return this->visited;
}

std::tuple<size_t, size_t> graph::Node::GetIndexes() const
{
    return {this->curIdx, this->parentIdx};
}
