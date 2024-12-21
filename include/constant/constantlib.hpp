#ifndef INCLUDE_CONSTANT_CONSTANTLIB_H_
#define INCLUDE_CONSTANT_CONSTANTLIB_H_

#include <limits>   // std::numeric_limits<double>::max()

// source: https://www.learncpp.com/cpp-tutorial/sharing-global-constants-across-multiple-files-using-inline-variables/

namespace constants
{
    /// @brief ∞ value for double
    inline constexpr double INF { std::numeric_limits<double>::max() };

} // namespace constants

#endif // INCLUDE_CONSTANT_CONSTANTLIB_H_