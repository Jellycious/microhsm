#!/bin/bash

ROOT_DIR=$(dirname $0)
BUILD_DIR=$ROOT_DIR/build

BUILD_CONFIG=Debug

BUILD_CMD="cmake -S ${ROOT_DIR} -B ${BUILD_DIR} -DCMAKE_BUILD_TYPE=Debug && make -C ${BUILD_DIR}"
BUILD_CMD_RELEASE="cmake -S ${ROOT_DIR} -B ${BUILD_DIR} -DCMAKE_BUILD_TYPE=Release && make -C ${BUILD_DIR}"

USAGE_MSG="USAGE: ./dev.sh [OPTIONS]

DESCRIPTION: Utility script that eases development

OPTIONS:
    -d, --dev       = Watch changes in source code and build/test upon changes
    -t, --test      = Run test library
    -r, --release   = Release build
    -c, --clean     = Clean before building
"


POSITIONAL_ARGS=()

build() {
    cmake -S ${ROOT_DIR} -B ${BUILD_DIR}/${BUILD_CONFIG} -DCMAKE_BUILD_TYPE=${BUILD_CONFIG}
    make -C ${BUILD_DIR}/${BUILD_CONFIG}
}

run_test() {
    ctest -V --output-on-failure --test-dir ${BUILD_DIR}/${BUILD_CONFIG}
}

clean() {
    rm -rf ${BUILD_DIR}
}

# Called with arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -t|--test)
            RUNTEST=1
            break
            ;;
        -d|--dev)
            RUNDEV=1
            break
            ;;
        -r|--release)
            BUILD_CONFIG=Release
            shift
            ;;
        -c|--clean)
            clean
            exit 0
            ;;
        *)
            echo "Incorrect parameter: $1"
            echo ""
            echo "$USAGE_MSG"
            exit 1
    esac
done

# Always build target
build

if [[ -n $RUNTEST ]]; then
    build
    if [[ $? -eq 0 ]]; then
        run_test
    fi
fi

if [[ -n $RUNDEV ]]; then
    echo "Running dev server..."
    run_test
    while inotifywait -r -e close_write $ROOT_DIR/tests $ROOT_DIR/src $ROOT_DIR/include; do
        echo "Building Project"
        build
        if [[ $? -eq 0 ]]; then
            echo "Running tests:"
            echo ""
            run_test
        fi
    done
fi
