#include <cmath>

#include "math/mathlib.hpp"

double distance(double x1, double y1, double x2, double y2)
{
    double x_diff = (x1 - x2);
    double y_diff = (y1 - y2);

    return sqrt(x_diff * x_diff + y_diff * y_diff);
}