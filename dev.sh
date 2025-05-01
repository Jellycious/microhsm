#!/bin/bash

ROOT_DIR=$(dirname $0)
BUILD_DIR=$ROOT_DIR/build
BUILD_CMD="cmake -S ${ROOT_DIR} -B ${BUILD_DIR} && make -C ${BUILD_DIR}"

TEST_BINARY=$BUILD_DIR/bin/microhsm_tests

USAGE_MSG="USAGE: ./dev.sh [OPTIONS]

DESCRIPTION: Utility script that eases development

OPTIONS:
    -d, --dev       = Watch changes in source code and build/test upon changes
    -t, --test      = Build and Test library
"


POSITIONAL_ARGS=()

build() {
    cmake -S ${ROOT_DIR} -B ${BUILD_DIR}
    make -C ${BUILD_DIR}
}

run_test() {
    $TEST_BINARY
}

# No arguments? Simply build
if [[ $# -eq 0 ]]; then
    build
    exit $?
fi;

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
        *)
            echo "$USAGE_MSG"
            exit 1
    esac
done

if [[ -n $RUNTEST ]]; then
    build
    run_test
fi

if [[ -n $RUNDEV ]]; then
    echo "Running dev server..."
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
