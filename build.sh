#!/bin/bash

CURR_DIR=$(pwd);

if [ "$#" -eq 1 ]; then
    BUILD_TYPE="$1";
    echo "Building $1 build";
else
    BUILD_TYPE="Debug";
fi

if [ ! -d "$CURR_DIR/build" ]; then
    mkdir "build";
elif [ $REBUILD ]; then
    rm -rf "build";
    mkdir "build";
fi

cd build;

echo 'Building cmake target'
cmake $CURR_DIR -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DMST_RUN_TESTS=True
echo 'Building mst'
cmake --build . --config $BUILD_TYPE

if [ $TESTING ]; then
    ctest -C $BUILD_TYPE --output-on-failure
fi

cd -;


