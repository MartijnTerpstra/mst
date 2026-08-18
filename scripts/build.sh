#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
CURR_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"

if [ "$#" -eq 1 ]; then
    BUILD_TYPE="$1";
    echo "Building $1 build";
else
    BUILD_TYPE="Debug";
fi

PRESET="dev-$(echo "$BUILD_TYPE" | tr '[:upper:]' '[:lower:]')"

if [ $REBUILD ]; then
    rm -rf "$CURR_DIR/build";
fi

cd "$CURR_DIR";

echo "Configuring cmake preset $PRESET"
cmake --preset "$PRESET"
echo 'Building mst'
cmake --build --preset "$PRESET" --parallel 10

if [ $TESTING ]; then
    ctest --preset "$PRESET"
fi

cd -;
