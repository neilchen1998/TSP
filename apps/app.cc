#include <iostream> // std::cout, std::endl
#include <stdlib.h> // EXIT_SUCCESS, EXIT_FAILURE

#include "math/mathlib.hpp"
#include "filesystem/loadlib.hpp"

int main(int argc, char* argv[])
{
    std::cout << "hello, this is a travelling salesman problem!" << std::endl;
    // read_file_line_by_line("./data/tsp_sample.txt");
    auto nodes = get_nodes_from_file("./data/tsp_sample.txt");
    auto graph = create_graph(nodes);

    return EXIT_SUCCESS;
}
