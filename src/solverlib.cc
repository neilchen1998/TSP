#include "graph/solverlib.hpp"

#include <cstddef>  // size_t
#include <unordered_map>
#include <vector>   // std::vector
#include <iostream> // std::cout
#include <optional> // std::optional
#include <deque>    // std::deque
#include <algorithm>// std::algorithm
#include <print>    // std::print, std::println
#include <string>   // std::string
#include <set>   // std::set

#include <boost/dynamic_bitset.hpp>     // boost::dynamic_bitset

#include "math/mathlib.hpp"
#include "constant/constantlib.hpp" // constants::INF
#include "graph/nodelib.hpp"        // graph::Node
#include "filesystem/loadlib.hpp"   // create_graph
#include "graph/visualizerlib.hpp"  // graph::print_graph

#if DEBUG
#include "graph/visualizerlib.hpp"
#endif

std::tuple<std::vector<size_t>, double> graph::solver::brute_force(const std::vector<std::vector<double>> &graph)
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
    std::string_view shortestPath;
    for (size_t i = 0; i < per.size(); ++i)
    {
        double cur = 0.0;
        for (size_t j = 0; j < N; ++j)
        {
            int from = per[i][j % N] - 'a';
            int to = per[i][(j + 1) % N] - 'a';

            cur += graph[from][to];
        }

        // if a smaller value is found, then updates the value and the path
        if (cur < ret)
        {
            shortestPath = per[i];
            ret = cur;
        }
    }

    #if DEBUG
    std::cout << "Shortest path: " << shortestPath << std::endl;
    #endif

    // converts alphabetic notations to numeric
    std::vector<size_t> path(N);
    std::transform(shortestPath.cbegin(), shortestPath.cend(), path.begin(), [](char c)
    {
        return c - 'a';
    });

    return {path, ret};
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

std::tuple<std::vector<size_t>, double> dfs(const std::vector<std::vector<double>> &graph, boost::dynamic_bitset<>& visited, size_t cur, std::vector<size_t>& path)
{
    // base case: visited all nodes and return to the start node
    if (visited.all())
    {
        return {path, graph[cur][0]};
    }

    const size_t N = graph.size();

    // sets the current cost to inf
    double bestCost = constants::INF;
    std::vector<size_t> bestPath;

    // explores all options
    for (size_t i = 1; i < N; ++i)
    {
        // makes sure we only explore nodes that have not yet visited
        if (!visited[i])
        {
            // makes a decision
            visited[i] = 1;
            path.push_back(i);

            auto [p, c] = dfs(graph, visited, i, path);
            if ((c + graph[cur][i]) < bestCost)
            {
                bestCost = c + graph[cur][i];
                bestPath = p;
            }

            // undoes the decision
            visited[i] = 0;
            path.pop_back();
        }
    }

    return {bestPath, bestCost};
}

std::tuple<std::vector<size_t>, double> graph::solver::DFS(const std::vector<std::vector<double>> &graph)
{
    const size_t N = graph.size();

    boost::dynamic_bitset<> visited(N, 0);
    visited[0] = 1;
    std::vector<size_t> path({0});

    return dfs(graph, visited, 0, path);
}

std::tuple<std::vector<size_t>, double> graph::solver::divide_n_conquer(const std::vector<graph::Coordinate>& nodes, const std::vector<std::vector<double>> &graph, size_t K)
{
    const size_t N = nodes.size();
    const size_t T = K * 15;
    std::vector<size_t> totalPath;

    std::vector<size_t> assignments(N);

    // calls the functions T times and picks the results that has the lowest variance value
    std::vector<graph::Coordinate> bestClusters;
    {
        std::vector<double> vars(T, constants::INF);
        std::vector<std::vector<graph::Coordinate>> rets(T);
        for (size_t i = 0; i < T; ++i)
        {
            auto [a, b] = k_means(nodes, assignments, K, 500);
            rets[i] = a;
            vars[i] = b;
        }

        // finds the best clustering by finding the group that has the smallest variance
        auto itr = std::min_element(vars.cbegin(), vars.cend());
        bestClusters = *(rets.begin() + (itr - vars.cbegin()));
    }

    #if DEBUG
    std::cout << "Centroids from calling K Means function:\n";
    for (auto& c : bestClusters)
    {
        std::print("({}, {})\t", c.x, c.y);
    }
    std::cout << "\n";
    #endif

    // the index of the cluster of the start point
    size_t startIdx = assignments.front();

    #if DEBUG
    std::println("Assignment:");
    for (size_t i = 0; i < N; ++i)
    {
        std::println("# {}: {}", i, assignments[i]);
    }
    #endif

    #if DEBUG
    std::println("Start idx: {}", startIdx);
    #endif

    // makes the cluster that has the start point to be in the first element since we need to start from that cluster
    std::swap(bestClusters[0], bestClusters[startIdx]);

    // converts clusters to graph
    auto clusterGraph = create_graph(bestClusters);

    // travels among the clusters
    auto [path, cost] = solver::branch_and_bound(clusterGraph);

    #if DEBUG
    std::cout << "Among clusters:\n";
    graph::print_path(path, "Path");
    std::println("Cost: {:.3f}", cost); // a precision of 2 decimal places
    #endif

    // travels within each cluster
    std::vector<std::vector<size_t>> paths(K);
    {
        // each table entails how to convert a local index back to its global index
        // key: local index, value: global index
        std::vector<std::unordered_map<size_t, size_t>> tables(K);

        // each group
        std::vector<std::vector<size_t>> groups;

        std::vector<std::vector<graph::Coordinate>> pointsInClusters(K);

        // the number of points of each cluster
        std::vector<size_t> localIndices(K, 0);

        for (size_t i = 0; i < N; ++i)
        {
            auto curAssignment = assignments[i];
            pointsInClusters[curAssignment].emplace_back(nodes[i]);
            tables[curAssignment][localIndices[curAssignment]] = i;
            ++localIndices[curAssignment];
        }

        #if DEBUG
        std::cout << "size of each cluster:\n";
        for (size_t i = 0; i < K; ++i)
        {
            std::println("Size of # {}: {}", i, pointsInClusters[i].size());
        }
        #endif

        for (size_t i = 0; i < K; ++i)
        {
            if (pointsInClusters[i].size() > 0)
            {
                auto g = create_graph(pointsInClusters[i]);
                auto [p, c] = solver::DFS(g);
                std::vector<size_t> p_global(p);
                for (size_t j = 0; j < p.size(); ++j)
                {
                    p_global[j] = tables[i][p[j]];
                }

                paths[i] = p_global;

                #if DEBUG
                std::cout << "Cluster # " << i << std::endl;
                graph::print_path(p_global, "Path");
                std::println("Cost: {:.3f}", c); // a precision of 2 decimal places
                #endif
            }
        }
    }

    // inserts the path within each cluster
    // makes the cluster that has the start point to be in the first element since we need to start from that cluster
    std::swap(paths[startIdx], paths[0]);
    for (auto& p : paths)
    {
        totalPath.insert(totalPath.end(), p.begin(), p.end());
    }

    double trueCost = 0.0;
    size_t u, v;
    for (size_t i = 0; i < N; ++i)
    {
        u = totalPath[i];
        v = totalPath[(i + 1) % N];
        trueCost += graph[u][v];
    }

    return {totalPath, trueCost};
}
