file(GLOB MATH_HEADER_LIST CONFIGURE_DEPENDS "${TSP_SOURCE_DIR}/include/math/*.hpp")
file(GLOB FILESYSTEM_HEADER_LIST CONFIGURE_DEPENDS "${TSP_SOURCE_DIR}/include/filesystem/*.hpp")
file(GLOB GRAPH_HEADER_LIST CONFIGURE_DEPENDS "${TSP_SOURCE_DIR}/include/graph/*.hpp")

include_directories(${Boost_INCLUDE_DIRS})

add_library(math_library mathlib.cc ${MATH_HEADER_LIST})
add_library(filesystem_library loadlib.cc ${FILESYSTEM_HEADER_LIST})
add_library(graph_library nodelib.cc ${FILESYSTEM_HEADER_LIST})

target_include_directories(math_library PUBLIC ../include)
target_include_directories(filesystem_library PUBLIC ../include)
target_include_directories(graph_library PUBLIC ../include)

target_link_libraries(math_library PUBLIC Boost::boost)

target_compile_features(math_library PUBLIC cxx_std_20)  # requires C++20 for concept
target_compile_features(filesystem_library PUBLIC cxx_std_11)
target_compile_features(graph_library PUBLIC cxx_std_11)
