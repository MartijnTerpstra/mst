#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

TESTING=1 "$SCRIPT_DIR/build.sh" "$@"
