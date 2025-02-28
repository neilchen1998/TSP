#include "graph/nodelib.hpp"

#include <vector>       // std::vector
#include <iostream>     // std::cout
#include <optional>     // std::optional
#include <algorithm>    // std::ranges::copy
#include <iterator>     // std::ostream_iterator

#include <boost/dynamic_bitset.hpp> // boost::dynamic_bitset

#include "graph/nodelib.hpp"

graph::Node::Node(std::vector<std::vector<double>>&& curGraph, double curCost) :
    parentIdx(-1),
    curIdx(0),
    graph(std::move(curGraph)),
    cost(curCost)
{
    // creates a visited list and marks the start node as visited
    visited.resize(this->graph.size());
    visited.reset();
    visited[curIdx] = 1;

    // pushes the current node to the path
    path.push_back(curIdx);
}

graph::Node::Node(size_t parentIdx, size_t childIdx, boost::dynamic_bitset<> visitedParent, std::vector<std::vector<double>>&& graph, double cost, const std::vector<size_t>& parentPath) :
    parentIdx(parentIdx),
    curIdx(childIdx),
    graph(std::move(graph)),
    cost(cost)
{
    // gets the visited list and marks the current node as visited
    visited = visitedParent;
    visited[curIdx] = 1;

    // gets the path from the parent and adds the current node to the path
    path = parentPath;
    path.push_back(curIdx);
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

std::vector<size_t> graph::Node::GetPath() const
{
    return this->path;
}

void graph::Node::PrintPath() const
{
    std::ranges::copy(path, std::ostream_iterator<size_t>(std::cout, " "));
    std::cout << "\n";
}
