#include "graph/solverlib.hpp"

#include <cstddef>  // size_t
#include <vector>   // std::vector
#include <iostream> // std::cout
#include <optional> // std::optional
#include <deque>    // std::deque
#include <algorithm>// std::algorithm
#include <print>    // std::print, std::println

#include <boost/dynamic_bitset.hpp>     // boost::dynamic_bitset
#include "math/mathlib.hpp"
#include "constant/constantlib.hpp"     // constants::INF
#include "graph/nodelib.hpp"            // graph::Node

#if DEBUG
#include <string>   // std::string
#include "graph/visualizerlib.hpp"
#endif

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

std::tuple<std::vector<size_t>, double> graph::solver::branch_and_bound(const std::vector<std::vector<double>> &graph, const size_t maxIteration)
{
    // gets the size of the graph
    const size_t N = graph.size();


    // makes the diagnol elements to be inf
    auto weights = graph;
    for (size_t i = 0; i < N; ++i)
    {
        weights[i][i] = constants::INF;
    }

    // creates a priority queue
    // NOTE: we are accessing the front of the queue
    auto cmp = [](const graph::Node& lhs, const graph::Node& rhs) { return lhs.GetCost() < rhs.GetCost(); };
    std::deque<graph::Node> pq;

    // the unique ID of the node instance
    unsigned long int cnt = 0;

    // creates the start node
    {
        auto [curGraph, curCost] = reduce_graph(weights);
        Node start = graph::Node(std::move(curGraph), curCost);
        pq.push_back(start);
    }

    // finds the optimal journey
    // only stops when either:
    // i. the priority queue is empty
    // ii. the top node has travelled all the nodes
    while (!pq.empty() && !pq.front().IsCompleted() && cnt < maxIteration)
    {
        // get the node from the top of the priority queue (the node that has the least cost)
        auto curNode = pq.front();
        boost::dynamic_bitset<> curVisited = curNode.GetVisited();
        auto curGraph = curNode.GetGraph();
        auto [curIdx, parentIdx] = curNode.GetIndexes();
        auto curCost = curNode.GetCost();

        #if DEBUG
        std::print("*** # {}: Node: {} Parent: {} ***\n", cnt, curIdx, parentIdx);
        graph::print_graph(curGraph);
        std::cout << "visited: " << curVisited << std::endl;
        std::print("cost: {}\n", curCost);
        std::cout << "Path: ";
        curNode.PrintPath();
        #endif

        // explores all nodes
        for (size_t nextIdx = 0; nextIdx < N; ++nextIdx)
        {
            // only explores an unvisited node
            if (!curVisited[nextIdx])
            {
                #if DEBUG
                std::print("curIdx: {} nextIdx: {}\n", curIdx, nextIdx);
                #endif

                // creates a new node
                auto [nextGraph, nextCost] = graph::explore_new_node(curGraph, curIdx, nextIdx, curCost);
                auto nextNode = graph::Node(curIdx, nextIdx, curVisited, std::move(nextGraph), nextCost, curNode.GetPath());

                // push the new node to the priority queue
                pq.push_back(nextNode);
                ++cnt;
            }
        }

        // pops the current node
        pq.pop_front();

        // truncates the pq by removing the trailing elements
        constexpr auto bound = 5000;
        if (pq.size() > bound)
        {
            pq.erase(pq.begin() + bound, pq.end());
        }

        // only sorts the top elements
        constexpr auto topN = 50;
        auto m = pq.begin() + topN;
        std::nth_element(pq.begin(), m, pq.end(), cmp);
    }

    // checks if we time-out
    if (cnt > maxIteration)
    {
        std::cerr << "Error: Timeout!" << std::endl;
        return {{}, constants::INF};
    }

    // checks if the priority queue is empty
    // if so that indicates a path cannot be found
    if (pq.empty())
    {
        std::cerr << "A path can not be found!" << std::endl;
        return {{}, constants::INF};
    }

    #if DEBUG
    std::println("Number of iterations: {}", cnt);
    #endif

    // returns the path and the cost of the top node
    return {pq.front().GetPath(), pq.front().GetCost()};
}
