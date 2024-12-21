#ifndef INCLUDE_MATH_MATHLIB_H_
#define INCLUDE_MATH_MATHLIB_H_

#include <vector>   // std::vector
#include <string>

double distance(double x1, double y1, double x2, double y2);

std::vector<std::string> generate_permutation(int N);

/// @brief Reduce the graph (used by branch and bound approach)
/// @param graph The original graph
/// @return [The reduced graph, the reduced cost]
std::tuple<std::vector<std::vector<double>>, double> reduce_graph(const std::vector<std::vector<double>>& graph);

#endif // INCLUDE_MATH_MATHLIB_H_
