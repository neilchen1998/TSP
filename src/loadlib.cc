#include <string>   // std::string
#include <fstream>  // std::ifstream
#include <iostream> // std::cerr, std::cout
#include <vector>   // std::vector
#include <sstream>  // std::istringstream

#include "filesystem/loadlib.hpp"
#include "math/mathlib.hpp"             // distance
#include "graph/nodelib.hpp"            // graph::Coordinate
#include "constant/constantlib.hpp"     // constants::INF

void read_file_line_by_line(const std::string& filename)
{
    std::ifstream file(filename);
    std::string str;

    // checks if the file exists
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open the file." << std::endl;
    }

    // skips until we reach "NODE_COORD_SECTION"
    while (std::getline(file, str) && str != "NODE_COORD_SECTION")
    {
        // skipping lines
    }

    // prints out the coordinates
    while (std::getline(file, str))
    {
        std::cout << str << std::endl;
    }
}

std::vector<graph::Coordinate> get_nodes_from_file(const std::string& filename)
{
    std::ifstream file(filename);
    std::string str;
    std::size_t num = 0;
    std::vector<graph::Coordinate> nodes;

    // checks if the file exists
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open the file." << std::endl;
        return {};
    }

    // skips until we reach "NODE_COORD_SECTION"
    while (std::getline(file, str) && str != "NODE_COORD_SECTION")
    {
        // skipping lines
    }

    // counts the number of elemnents
    while (std::getline(file, str))
    {
        ++num;
    }

    #if DEBUG
    std::cout << "Numbers of elements: " << num << std::endl;
    #endif

    // resets the file and goes to the coordinate portion
    file.clear();
    file.seekg(0);
    while (std::getline(file, str) && str != "NODE_COORD_SECTION") {}

    // resizes the graph
    nodes.resize(num);

    // gets the coordinates
    int id;
    double x, y;
    auto itr = nodes.begin();
    while (std::getline(file, str) && itr != nodes.end())
    {
        std::istringstream iss(str);
        iss >> id >> x >> y;
        itr->x = x;
        itr->y = y;
        #if DEBUG
        std::cout << "id: " << id << " x: " << x << " y: " << y << std::endl;
        #endif

        ++itr;
    }

    return nodes;
}

std::vector<std::vector<double>> create_graph(const std::vector<graph::Coordinate>& nodes)
{
    const size_t N = nodes.size();
    std::vector<std::vector<double>> graph(N, std::vector<double>(N, constants::INF));

    // constructs the graph
    // 0 -> 1, 0 -> 2, 0 -> 3, ...
    // 1 -> 2, 1 -> 3, 1 -> 4, ...
    for (size_t i = 0; i < N; ++i)
    {
        for (size_t j = i + 1; j < N; ++j)
        {
            double d = distance(nodes[i].x, nodes[i].y, nodes[j].x, nodes[j].y);

            graph[i][j] = d;
            graph[j][i] = d;    // the distance from B to A is the same as from A to B
        }
    }

    #if DEBUG
    for (auto row : graph)
    {
        for (auto ele : row)
        {
            std::cout << ele << " ";
        }
        std::cout << "\n";
    }
    #endif

    return graph;
}
