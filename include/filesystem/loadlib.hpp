#ifndef INCLUDE_FILESYSTEM_LOADLIB_H_
#define INCLUDE_FILESYSTEM_LOADLIB_H_

#include <string>   // std::string
#include <vector>   // std::vector

#include "graph/nodelib.hpp"    // graph::Node

void read_file_line_by_line(const std::string& filename);

std::vector<graph::Node> get_nodes_from_file(const std::string& filename);

std::vector<std::vector<double>> create_graph(const std::vector<graph::Node>& nodes);

#endif // INCLUDE_FILESYSTEM_LOADLIB_H_