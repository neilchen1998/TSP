#include <cmath>        // square
#include <algorithm>    // std::prev_permutation, std::
#include <iostream>     // std::cout
#include <string>

#include <boost/math/special_functions/factorials.hpp>  //boost::math::factorial

#include "math/mathlib.hpp"

double distance(double x1, double y1, double x2, double y2)
{
    double x_diff = (x1 - x2);
    double y_diff = (y1 - y2);

    return sqrt(x_diff * x_diff + y_diff * y_diff);
}

std::vector<std::string> generate_permutation(int N)
{
    std::string s(N, 'a');
    std::vector<std::string> ret;

    // calculates the size of the permutation vector (the factorial of N)
    auto sz = boost::math::factorial<double>(N);
    ret.resize(sz);

    // constructs the base permutation string
    for (int i = 0; i < N; ++i)
    {
        s[i] += i;
    }

    #if DEBUG
    std::cout << s << std::endl;
    #endif

    // finds all the permutations
    int i = 0;
    do
    {
        ret[i] = s;
        ++i;
    } while (std::next_permutation(s.begin(), s.end()));
    
    return ret;
}
