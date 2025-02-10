#include <cmath>        // square
#include <algorithm>    // std::prev_permutation, std::min_element, std::fill, std::transform
#include <random>       // std::uniform_real_distribution, std::mt19937, std::random_device
#include <string>       // std::string
#include <tuple>        // std::tuple
#include <numeric>      // std::reduce
#include <vector>       // std::vector

#include <boost/math/special_functions/factorials.hpp>  //boost::math::factorial
#include <fmt/ranges.h>

#include "math/mathlib.hpp"
#include "constant/constantlib.hpp"     // constants::INF
#include "graph/nodelib.hpp"    // graph::Coordinate

#if DEBUG
#include <iostream>     // std::cout
#include <string_view>  // std::string_view
#include <format>       // std::format
#include "fmt/base.h"           // fmt::println
#include "graph/visualizerlib.hpp"
#endif

#define MATH_PERMUTATION_SIZE 4000000

void print_cluster(const std::vector<graph::Coordinate>& clusters, std::string_view name)
{
    std::cout << name << "\n";
    for (auto& cluster : clusters)
    {
        std::cout << std::setprecision(2) << std::format("({}, {})\t", cluster.x, cluster.y);
    }
    std::cout << std::endl;
}

double distance(double x1, double y1, double x2, double y2)
{
    double x_diff = (x1 - x2);
    double y_diff = (y1 - y2);

    return sqrt(x_diff * x_diff + y_diff * y_diff);
}

double distance(const graph::Coordinate& a, const graph::Coordinate& b)
{
    return distance(a.x, a.y, b.x, b.y);
}

std::vector<std::string> generate_permutation(int N)
{
    std::string s(N, 'a');
    std::vector<std::string> ret;

    // calculates the size of the permutation vector (the factorial of N)
    auto sz = boost::math::factorial<double>(N);

    sz = (sz > MATH_PERMUTATION_SIZE) ? MATH_PERMUTATION_SIZE : sz;

    ret.resize(sz);

    // constructs the base permutation string
    for (int i = 0; i < N; ++i)
    {
        s[i] += i;
    }

    #if DEBUG
    std::cout << s << std::endl;
    #endif

    // finds all the permutations
    int i = 0;
    do
    {
        ret[i] = s;
        ++i;
    } while (std::next_permutation(s.begin(), s.end()) && i < MATH_PERMUTATION_SIZE);

    return ret;
}

void set_row_col_inf(std::vector<std::vector<double>>& graph, size_t row, size_t col)
{
    const size_t N = graph.size();

    // sets all elements in the row to inf
    for (auto& ele : graph[row])
    {
        ele = constants::INF;
    }

    // sets all elements in the column to inf
    for (size_t r = 0; r < N; r++)
    {
        graph[r][col] = constants::INF;
    }
}

std::tuple<std::vector<std::vector<double>>, double> reduce_graph(const std::vector<std::vector<double>>& graph)
{
    const size_t N = graph.size();
    std::vector<std::vector<double>> ret(graph);
    std::vector<double> reducedValues;

    // loops through each row
    for (auto& row : ret)
    {
        // finds the minimum value
        double min = *std::min_element(row.cbegin(), row.cend());

        if (min < constants::INF)
        {
            reducedValues.push_back(min);
        }

        // subtracts each element with the minimum value
        for (double& ele : row)
        {
            // only subtracts the element if it is not infinite
            ele = (ele < constants::INF) ? (ele - min) : ele;
        }
    }

    // loops through each column
    for (size_t c = 0; c < N; c++)
    {
        // loops through each element in the column
        double min = constants::INF;
        for (size_t r = 0; r < N; r++)
        {
            min = std::min(ret[r][c], min);
        }

        if (min < constants::INF)
        {
            reducedValues.push_back(min);
        }

        // subtracts each element with the minimum value
        for (size_t r = 0; r < N; r++)
        {
            // only subtracts the element if it is not infinite
            ret[r][c] = (ret[r][c] < constants::INF) ? (ret[r][c] - min) : ret[r][c];
        }
    }

    double reducedValue = std::reduce(reducedValues.cbegin(), reducedValues.cend());

    return {ret, reducedValue};
}

std::vector<graph::Coordinate> k_means(const std::vector<graph::Coordinate>& coordinates, const size_t K, const size_t maxItr)
{
    // source: http://www.goldsborough.me/c++/python/cuda/2017/09/10/20-32-46-exploring_k-means_in_python,_c++_and_cuda/

    const size_t N = coordinates.size();

    // creates an int random generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, N - 1);

    // randomly selects a coordinate to be the center of a cluster
    std::vector<graph::Coordinate> clusters(K);
    std::vector<graph::Coordinate> prevClusters(K, graph::Coordinate(constants::INF, constants::INF));
    for (auto& cluster : clusters)
    {
        cluster = coordinates[dis(gen)];
    }

    // each coordinate's assignment
    std::vector<size_t> assignments(K);

    size_t cnt = 0;
    float delta = constants::INF;
    while (cnt != maxItr && delta > 0.05)
    {
        // finds the best assignment
        for (size_t i = 0; i < N; ++i)
        {
            auto curCoordinate = coordinates[i];
            auto itr = std::min_element(clusters.begin(), clusters.end(), [&curCoordinate](const graph::Coordinate& lhs, const graph::Coordinate& rhs)
                {
                    return distance(lhs, curCoordinate) < distance(rhs, curCoordinate);
                });

            assignments[i] = std::distance(clusters.begin(), itr);
        }

        // counts the number of each cluster
        std::vector<size_t> counts(N, 0);
        for (auto assignment : assignments)
        {
            ++counts[assignment];
        }

        #if DEBUG
        fmt::println("assignments: {}", assignments);
        #endif

        // sums up the coordinates to the clusters
        graph::Coordinate zero;
        std::fill(clusters.begin(), clusters.end(), graph::Coordinate(0, 0));
        for (size_t i = 0; i < N; ++i)
        {
            clusters[assignments[i]].x += coordinates[i].x;
            clusters[assignments[i]].y += coordinates[i].y;
        }

        // divides the number of count of each cluster
        std::transform(clusters.begin(), clusters.end(), counts.begin(), clusters.begin(), [](const graph::Coordinate& coordinate, const size_t count)
        {
            // finds the denominator and makes sure the value is at least greater than or equal to one
            auto deno = std::max<size_t> (count, 1);
            graph::Coordinate ret = {coordinate.x / deno, coordinate.y / deno};
            return ret;
        });

        // finds the difference percentage between the previous and the current
        delta = std::transform_reduce(clusters.begin(), clusters.end(), prevClusters.begin(), 0.0, std::plus<double>(), [](const graph::Coordinate& cur, const graph::Coordinate& prev)
        {
            auto diffX = std::abs(cur.x - prev.x) / (prev.x + 0.00001);
            auto diffY = std::abs(cur.y - prev.y) / (prev.y + 0.00001);

            return (diffX + diffY) / 2;
        });

        #if DEBUG
        print_cluster(prevClusters, "prev");
        print_cluster(clusters, "cur");
        fmt::println("# of iterations: {}, delta: {}", cnt, delta);
        #endif

        prevClusters = clusters;

        ++cnt;
    }

    // sorts the clusters based on the distance from the origin point
    std::sort(clusters.begin(), clusters.end(), [](const graph::Coordinate& a, const graph::Coordinate& b)
    {
        return ((a.x * a.x) + (a.y * a.y)) < ((b.x * b.x) + (b.y * b.y));
    });

    // returns the centroids of the clusters
    return clusters;
}
