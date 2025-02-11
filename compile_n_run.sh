#!/bin/bash

BUILD_TYPE="Release"
RUN_BINARY="No"

# help function
function help()
{
    # display help
    printf "Usage: %s [OPTIONS]\n" "$0"  # includes the script name
    printf "Options:\n"
    printf "  -h, --help      Display this help message\n"
    printf "  -d, --debug     Build in Debug mode\n"
    printf "  -r, --run       Build in Release mode and run the app\n"
    printf "  -b, --build     Build only in Release mode\n"
    printf "  -t, --test      Build and run tests\n"
    printf "  -T, --TEST      Build in debug mode and run tests\n"
    printf "  -m, --mode <mode> Build in specified mode (e.g., -m Debug)\n"
}

# configures and builds the project
function configure_and_build()
{
    # configures the repo
    cmake -S . -B build -DCMAKE_BUILD_TYPE=$BUILD_TYPE

    # # builds the repo
    cmake --build build
}

# runs the binary
function run_binary()
{
    ./build/apps/app
}

# runs all test cases
function run_test_cases()
{
    cmake --build build --target test
}

# main program

# gets the options
# NOTE: the order of the argument list matters
# in this case, '-t' will be processed first, then '-d', etc
while getopts ":hdrbtTm:" option; do
    case $option in
        h) # display Help
            help
            exit;;
        d) # debug
            BUILD_TYPE="Debug";;
        r) # run
            BUILD_TYPE="Release"
            RUN_BINARY="Yes";;
        b) # build
            BUILD_TYPE="Release"
            RUN_BINARY="No";;
        t) # test
            BUILD_TYPE="Test";;
        T) # test
            BUILD_TYPE="TEST";;
        m) # manual select build type
            BUILD_TYPE=$OPTARG;;
        \?) # invalid option
            echo "Error: Invalid option"
            exit;;
    esac
done

configure_and_build &&

if [[ "$BUILD_TYPE" == "Test" || "$BUILD_TYPE" == "TEST" ]]; then
    run_test_cases
elif [[ "$RUN_BINARY" == "Yes" ]]; then
    run_binary
fi
