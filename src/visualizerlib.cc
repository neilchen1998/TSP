#include "graph/visualizerlib.hpp"

#include <vector>   // std::vector
#include <iostream> // std::cout
#include <algorithm>
#include <iterator>

#include "constant/constantlib.hpp"     // constants::INF

void graph::print_graph(const std::vector<std::vector<double>>& graph)
{
    for (const auto& row : graph )
    {
        for (const auto& ele : row)
        {
            if (ele < constants::INF)
            {
                std::cout << ele << " ";
            }
            else
            {
                std::cout << "∞" << " ";
            }
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
