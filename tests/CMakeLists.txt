FetchContent_Declare(
  catch
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v2.13.6)
FetchContent_MakeAvailable(catch)

add_executable(mathtestlib mathtestlib.cc)
add_executable(graphtestlib graphtestlib.cc)

target_compile_features(mathtestlib PRIVATE cxx_std_23) # requires C+23 for std::print
target_compile_features(graphtestlib PRIVATE cxx_std_17)

target_link_libraries(mathtestlib PRIVATE Catch2::Catch2 math_library)
target_link_libraries(graphtestlib PRIVATE Catch2::Catch2 graph_library)

add_test(NAME mathtestlibtest COMMAND mathtestlib)
add_test(NAME graphtestlibtest COMMAND graphtestlib)
