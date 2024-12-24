#ifndef INCLUDE_GRAPH_VISUALIZERLIB_H_
#define INCLUDE_GRAPH_VISUALIZERLIB_H_

#include <vector>   // std::vector

namespace graph
{
    void print_graph(const std::vector<std::vector<double>>& graph);

    void print_path(const std::vector<size_t>& path, const std::string& name);
};  // namespace graph

#endif // INCLUDE_GRAPH_VISUALIZERLIB_H_
