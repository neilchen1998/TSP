#include "graph/nodelib.hpp"

#include <vector>   // std::vector

#include <boost/dynamic_bitset.hpp> // boost::dynamic_bitset

#include "constant/constantlib.hpp"     // constants::INF

graph::Node::Node()
{
    startIdx = 0;
    parentIdx = 0;
    curIdx = 0;
    isLeaf = true;
    cost = constants::INF;
}

graph::Node::Node(size_t startIdx, size_t parentIdx, size_t curIdx, boost::dynamic_bitset<> visitedParent) :
    startIdx(startIdx),
    parentIdx(parentIdx),
    curIdx(curIdx)
{
    isLeaf = true;
    cost = constants::INF;

    visited = visitedParent;
    visited[curIdx] = 1;
}

graph::Node::~Node()
{

}

double graph::Node::GetCost() const
{
    return this->cost;
}