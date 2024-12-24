#include "graph/solverlib.hpp"

#include <vector>   // std::vector
#include <iostream> // std::cout
#include <optional> // std::optional
#include <queue>    // std::priority_queue
#include <print>    // std::print

#include <boost/dynamic_bitset.hpp> // boost::dynamic_bitset
#include "math/mathlib.hpp"
#include "constant/constantlib.hpp"     // constants::INF
#include "graph/visualizerlib.hpp"
#include "graph/nodelib.hpp"            // graph::Node

double graph::solver::brute_force(const std::vector<std::vector<double>> &graph)
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

std::tuple< std::vector<std::vector<double>>, double> graph::explore_new_node(const std::vector<std::vector<double>> &graph, const size_t from, const size_t to, double prevCost, const size_t start)
{
    double ret;

    auto [curGraph, _] = reduce_graph(graph);
    double curCost = curGraph[from][to]; // get the cost from node A to node B
    curGraph[to][start] = constants::INF; // makes the weight from the next node to the previous node inf
    set_row_col_inf(curGraph, from, to);
    auto [nextGraph, nextReducedVal] = reduce_graph(curGraph);

    ret = curCost + prevCost + nextReducedVal;

    return {nextGraph, ret};
}

std::optional<size_t> graph::find_unvisited_node(const boost::dynamic_bitset<>& visited)
{
    #if DEBUG
    std::cout << "visited: " << visited << std::endl;
    #endif

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

double graph::solver::branch_and_bound(const std::vector<std::vector<double>> &graph)
{
    const size_t N = graph.size();
    auto cmp = [](const graph::Node& lhs, const graph::Node& rhs) { return lhs.GetCost() > rhs.GetCost(); };
    std::priority_queue<graph::Node, std::vector<graph::Node>, decltype(cmp)> pq(cmp);
    unsigned long int cnt = 0;

    // creates the start node
    auto [curGraph, curCost] = reduce_graph(graph);
    boost::dynamic_bitset<> visited(N);
    visited.reset();
    visited[0] = 1;
    Node start = graph::Node(-1, 0, visited, std::move(curGraph), curCost);
    pq.push(start);

    // finds the optimal journey
    while (!pq.empty())
    {
        if (cnt > 99)
        {
            std::cerr << "Error: Timeout!" << std::endl;
            return constants::INF;
        }

        auto curNode = pq.top();

        // checks if we have visited every all the nodes
        if (curNode.IsCompleted())
        {
            break;
        }
        boost::dynamic_bitset<> curVisited = curNode.GetVisited();
        auto curGraph = curNode.GetGraph();
        auto [curIdx, parentIdx] = curNode.GetIndexes();

        std::print("*** # {}: Node: {} Parent: {} ***\n", cnt, curIdx, parentIdx);
        graph::print_graph(curGraph);
        std::cout << "visited: " << curVisited << std::endl;
        auto c = curNode.GetCost();
        std::print("cost: {}", c);
        std::cout << "\n";

        for (int nextIdx = 0; nextIdx < N; ++nextIdx)
        {
            if (!curVisited[nextIdx])
            {
                std::print("curIdx: {} nextIdx: {}\n", curIdx, nextIdx);
                auto [nextGraph, nextCost] = graph::explore_new_node(curGraph, curIdx, nextIdx, c);
                auto nextNode = graph::Node(curIdx, nextIdx, curVisited, std::move(nextGraph), nextCost);

                pq.push(nextNode);
                ++cnt;
            }
        }

        pq.pop();
    }

    if (pq.empty())
    {
        std::cerr << "A path can not be found!" << std::endl;
        return constants::INF;
    }

    // returns the cost of the top node
    return pq.top().GetCost();
}
