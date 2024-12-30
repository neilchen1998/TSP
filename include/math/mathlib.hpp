#ifndef INCLUDE_MATH_MATHLIB_H_
#define INCLUDE_MATH_MATHLIB_H_

#include <cstddef>  // size_t
#include <vector>   // std::vector
#include <string>   // std::string

#include "graph/nodelib.hpp"    // graph::Coordinate

double distance(double x1, double y1, double x2, double y2);

/// @brief Finds the distance between two coordinates
/// @param a Coordinate A
/// @param b Coordinate B
/// @return The distance between the two coordinates
double distance(const graph::Coordinate& a, const graph::Coordinate& b);

std::vector<std::string> generate_permutation(int N);

/// @brief Reduce the graph (used by branch and bound approach)
/// @param graph The original graph
/// @return [The reduced graph, the reduced cost]
std::tuple<std::vector<std::vector<double>>, double> reduce_graph(const std::vector<std::vector<double>>& graph);

/// @brief Set all the elements from the row and the column to infinity
/// @param graph The target graph
/// @param row The row
/// @param col The column
void set_row_col_inf(std::vector<std::vector<double>>& graph, size_t row, size_t col);

#endif // INCLUDE_MATH_MATHLIB_H_
