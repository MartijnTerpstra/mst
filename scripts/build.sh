#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
CURR_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"

if [ "$#" -eq 1 ]; then
    BUILD_TYPE="$1";
    echo "Building $1 build";
else
    BUILD_TYPE="Debug";
fi

if [ ! -d "$CURR_DIR/build" ]; then
    mkdir "$CURR_DIR/build";
elif [ $REBUILD ]; then
    rm -rf "$CURR_DIR/build";
    mkdir "$CURR_DIR/build";
fi

cd "$CURR_DIR/build";

echo 'Building cmake target'
cmake "$CURR_DIR" -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DMST_RUN_TESTS=True -DMST_UTILS=True
echo 'Building mst'
cmake --build . --config $BUILD_TYPE --parallel 10

if [ $TESTING ]; then
    ctest -C $BUILD_TYPE --output-on-failure
fi

cd -;
