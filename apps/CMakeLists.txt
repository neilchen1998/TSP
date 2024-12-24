add_executable(app app.cc)
target_compile_features(app PRIVATE cxx_std_17)

# required libraries
target_link_libraries(app PRIVATE math_library filesystem_library graph_library ${Boost_LIBRARIES})
