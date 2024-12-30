# NOTE: the header lists are optional, they do not affect add_library
file(GLOB MATH_HEADER_LIST CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/include/math/*.hpp")
file(GLOB FILESYSTEM_HEADER_LIST CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/include/filesystem/*.hpp")
file(GLOB GRAPH_HEADER_LIST CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/include/graph/*.hpp")
file(GLOB CONSTANT_HEADER_LIST CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/include/constant/*.hpp")

include_directories(${Boost_INCLUDE_DIR})  # all libraries require Boost

add_library(math_library mathlib.cc ${MATH_HEADER_LIST})
add_library(filesystem_library loadlib.cc ${FILESYSTEM_HEADER_LIST})
add_library(graph_library nodelib.cc solverlib.cc visualizerlib.cc ${FILESYSTEM_HEADER_LIST})

target_include_directories(math_library PUBLIC ../include)
target_include_directories(filesystem_library PUBLIC ../include)
target_include_directories(graph_library PUBLIC ../include)

target_link_libraries(math_library PUBLIC Boost::boost fmt::fmt)
target_link_libraries(graph_library PUBLIC math_library)

target_compile_features(graph_library PUBLIC cxx_std_23)  # requires C+23 for std::print, std::ranges::copy
target_compile_features(math_library PUBLIC cxx_std_20)  # requires C+20 for std::format

# for IDEs
source_group(
  TREE "${PROJECT_SOURCE_DIR}/include"
  PREFIX "Header Files"
  FILES ${MATH_HEADER_LIST} ${FILESYSTEM_HEADER_LIST} ${GRAPH_HEADER_LIST} ${CONSTANT_HEADER_LIST})
