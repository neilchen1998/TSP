#include "graph/visualizerlib.hpp"

#include <string>       // std::to_string
#include <vector>       // std::vector
#include <iostream>     // std::cout
#include <algorithm>    // std::ranges::copy
#include <iterator>     // std::ostream_iterator
#include <iomanip>      // std::setw, std::setprecision

#include "constant/constantlib.hpp"     // constants::INF

void graph::print_graph(const std::vector<std::vector<double>>& graph)
{
    for (const auto& row : graph )
    {
        for (const auto& ele : row)
        {
            if (ele < constants::INF)
            {
                std::cout << std::right << std::setw(7) <<  ele;
            }
            else
            {
                std::cout << std::right << std::setw(7) << "inf";
            }

            std::cout << " ";
        }

        std::cout << "\n";
    }
}

void graph::print_path(const std::vector<size_t>& path, const std::string& name)
{
    std::cout << name << ":\t";
    std::ranges::copy(path, std::ostream_iterator<size_t>(std::cout, " "));
    std::cout << "\n";
}
