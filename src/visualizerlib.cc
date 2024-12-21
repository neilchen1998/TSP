#include "graph/visualizerlib.hpp"

#include <vector>   // std::vector
#include <iostream> // std::cout

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
