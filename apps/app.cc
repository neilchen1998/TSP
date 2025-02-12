#include <iostream> // std::cout, std::endl
#include <stdlib.h> // EXIT_SUCCESS, EXIT_FAILURE
#include <print>    // std::println
#include <string>   // std::string
#include <vector>   // std::vector
#include <tuple>    // std::tie
#include <unordered_set>    // std::unordered_set

#include <boost/program_options.hpp>    // boost::program_options

#include "filesystem/loadlib.hpp"   // create_graph
#include "graph/solverlib.hpp"      // graph::solver::branch_and_bound, graph::solver::brute_force
#include "graph/visualizerlib.hpp"  // graph::print_graph
#include "constant/constantlib.hpp" // constants::INF

namespace po = boost::program_options;

int main(int argc, char* argv[])
{
    // variables
    std::string input;
    std::string filename;
    std::string solver;
    std::string additional;

    // cretes options
    po::options_description desc("Options:");
    desc.add_options()
        ("help,h", "Display the help menu")
        ("filename,f", po::value<std::string>(&filename)->value_name("<INPUT_FILENAME>")->default_value("./data/tsp_sample.txt"), "the input filename")
        ("solver,s", po::value<std::string>(&solver)->value_name("<SOLVER>")->default_value("brute_force"), "the solver algorithm")
        ("additional,a", po::value<std::string>(&additional)->value_name("<ADDITIONAL_FUNCTIONS>")->default_value(""), "additional function");

    // creates the variables map and stores the inputs to the map
    po::variables_map vm;

    // makes "input" token be the positional option, i.e., token with no option name
    po::positional_options_description p;
    p.add("input", -1);

    // checks if the user inputs are valid
    try
    {
        // parses the arguments and writes the variables according to the variables map
        po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
        po::notify(vm);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }

    // checks if the user selects --help
    if (vm.count("help"))
    {
        std::cout << desc << "\n";
        return EXIT_FAILURE;
    }

    std::cout << "*** Travelling Salesman Problem ***" << std::endl;
    std::println("filename: {}", filename);
    std::println("solver: {}", solver);

    // checks if the solver that the user enters is available
    std::unordered_set<std::string> solvers ({"brute_force", "branch_n_bound", "DFS", "divide_n_conquer"});
    if (solvers.find(solver) == solvers.end())
    {
        std::cerr << "Error: " << solver << " is not available!" << std::endl;
        std::cout << "The available options are: ";

        // prints out all available solvers
        for (const auto& s : solvers)
        {
            std::cout << s << "  ";
        }
        std::cout << std::endl;

        return EXIT_FAILURE;
    }

    // gets the nodes from the file
    auto nodes = get_nodes_from_file(filename);

    if (nodes.empty())
    {
        std::cerr << "Error: Cannot get nodes from the input file!" << std::endl;
        return EXIT_FAILURE;
    }

    auto graph = create_graph(nodes);

    if (additional == "print")
    {
        graph::print_graph(graph);
    }

    // determines the algorithm to solve the TSP problem
    std::vector<size_t> path;
    double cost = constants::INF;
    if (solver == "branch_n_bound")
    {
        auto ret = graph::solver::branch_and_bound(graph, 1000000);

        // unpacks a pair
        std::tie(path, cost) = ret;
    }
    else if (solver == "brute_force")
    {
        std::tie(path, cost) = graph::solver::brute_force(graph);
    }
    else if (solver == "DFS")
    {
        std::tie(path, cost) = graph::solver::DFS(graph);
    }
    else if (solver == "divide_n_conquer")
    {
        std::tie(path, cost) = graph::solver::divide_n_conquer(nodes, graph);
    }

    if (cost == constants::INF)
    {
        std::cerr << "Error: Cannot find a path!" << std::endl;
        return EXIT_FAILURE;
    }

    graph::print_path(path, "Path");
    std::println("Cost: {:.3f}", cost); // a precision of 2 decimal places

    return EXIT_SUCCESS;
}
