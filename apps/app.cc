#include <iostream> // std::cout, std::endl
#include <stdlib.h> // EXIT_SUCCESS, EXIT_FAILURE

#include "math/mathlib.hpp"
#include "filesystem/loadlib.hpp"
#include "graph/solverlib.hpp"
#include "graph/visualizerlib.hpp"
#include "constant/constantlib.hpp"

int main(int argc, char* argv[])
{
    std::cout << "hello, this is a travelling salesman problem!" << std::endl;
    auto nodes = get_nodes_from_file("./data/tsp_sample.txt");
    auto graph = create_graph(nodes);
    auto [path, cost] = graph::solver::branch_and_bound(graph);

    if (cost == constants::INF)
    {
        std::cerr << "Error: Cannot find a path!" << std::endl;
        return EXIT_FAILURE;
    }

    graph::print_path(path, "Path");
    std::print("Cost: {}\n", cost);

    return EXIT_SUCCESS;
}
