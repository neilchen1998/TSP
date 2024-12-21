#include <cmath>        // square
#include <algorithm>    // std::prev_permutation, std::min_element
#include <iostream>     // std::cout
#include <string>       // std::string
#include <limits>       // constants::INF
#include <tuple>        // std::tuple
#include <numeric>      // std::reduce

#include <boost/math/special_functions/factorials.hpp>  //boost::math::factorial

#include "math/mathlib.hpp"
#include "constant/constantlib.hpp"     // constants::INF

double distance(double x1, double y1, double x2, double y2)
{
    double x_diff = (x1 - x2);
    double y_diff = (y1 - y2);

    return sqrt(x_diff * x_diff + y_diff * y_diff);
}

std::vector<std::string> generate_permutation(int N)
{
    std::string s(N, 'a');
    std::vector<std::string> ret;

    // calculates the size of the permutation vector (the factorial of N)
    auto sz = boost::math::factorial<double>(N);
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
    } while (std::next_permutation(s.begin(), s.end()));
    
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
