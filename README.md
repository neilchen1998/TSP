# Travelling Salesman Problem

This project attempts to solve the famous travelling salesman problem.

## Requirements

The requirements are:

- CMake 3.11 or better; 3.14+ highly recommended
- A C++17 compatible compiler
- The Boost libararies
- Git
- Doxygen (optional, highly recommended)

## Instructions

To use the script to make your life easier:

```bash
./compile_n_run.sh [OPTIONS]
```

To configure (Debug):

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
```

To configure (Release):

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
```

To configure (Gprof):

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Gprof
```

Add `-GNinja` if you have Ninja.

To build:

```bash
cmake --build build
```

To test (`--target` can be written as `-t` in CMake 3.15+):

```bash
cmake --build build --target test
```

To test by tag
```
cd build && ctest -R <tag>
```

To run
```
./build/apps/app
```

To build and run
```
cmake --build build && ./build/apps/app
```

To build and test
```
cmake --build build && cmake --build build --target test
```

To build docs (requires Doxygen, output in `build/docs/html`):

```bash
cmake --build build --target docs
```


## Reference

* [Test data](https://www.math.uwaterloo.ca/tsp/world/countries.html)
