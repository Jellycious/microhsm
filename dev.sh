#!/bin/bash

ROOT_DIR=$(dirname $0)
BUILD_DIR=$ROOT_DIR/build

BUILD_CONFIG=Debug

BUILD_CMD="cmake -S ${ROOT_DIR} -B ${BUILD_DIR} -DCMAKE_BUILD_TYPE=Debug && make -C ${BUILD_DIR}"
BUILD_CMD_RELEASE="cmake -S ${ROOT_DIR} -B ${BUILD_DIR} -DCMAKE_BUILD_TYPE=Release && make -C ${BUILD_DIR}"

USAGE_MSG="USAGE: ./dev.sh [OPTIONS]

DESCRIPTION: Utility script that eases development

OPTIONS:
    -r, --release           = Release build
    -t, --test              = Build tests
    -c, --clean             = Clean before building
    -e, --example           = Build examples
    -d, --dev               = Start dev server (rebuilds and tests upon code changes)
    -re, --run-examples     = Run examples
    -rt, --run-tests        = Run tests
    -pc, --print-coverage   = Print coverage info (requires code to be build with coverage enabled)
"


POSITIONAL_ARGS=()

build() {
    cmake -S ${ROOT_DIR} -B ${BUILD_DIR}/${BUILD_CONFIG} -DCMAKE_BUILD_TYPE=${BUILD_CONFIG} ${BUILD_OPTS}
    make -C ${BUILD_DIR}/${BUILD_CONFIG}
}

run_test() {
    ctest -V --output-on-failure --test-dir ${BUILD_DIR}/${BUILD_CONFIG}
}

run_examples() {
    ${BUILD_DIR}/${BUILD_CONFIG}/bin/microhsm_example_basic
    ${BUILD_DIR}/${BUILD_CONFIG}/bin/microhsm_example_macros
}

clean() {
    rm -rf ${BUILD_DIR}
}

# Called with arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -t|--test)
            BUILD_TEST=1
            shift
            ;;
        -d|--dev)
            RUNDEV=1
            BUILD_TEST=1
            shift
            ;;
        -r|--release)
            BUILD_CONFIG=Release
            shift
            ;;
        -c|--clean)
            clean
            exit 0
            ;;
        -e|--example)
            BUILDEXAMPLES=1
            BUILD_OPTS="${BUILD_OPTS} -DBUILD_EXAMPLES=ON"
            shift
            ;;
        -re|--run-examples)
            BUILDEXAMPLES=1
            RUNEXAMPLES=1
            shift
            ;;
        -rt|--run-tests)
            BUILD_TEST=1
            RUNTEST=1
            shift;;
        -pc|--print-coverage)
            BUILD_TEST=1
            PRINT_COVERAGE=1
            shift
            ;;
        *)
            echo "Incorrect parameter: $1"
            echo ""
            echo "$USAGE_MSG"
            exit 1
    esac
done

# Prepare build
if [[ -n $BUILD_TEST ]]; then
    BUILD_OPTS="${BUILD_OPTS} -DMICROHSM_BUILD_TESTS=ON"
fi
if [[ -n $BUILDEXAMPLES ]]; then
    BUILD_OPTS="${BUILD_OPTS} -DMICROHSM_BUILD_EXAMPLES=ON"
fi
if [[ -n $PRINT_COVERAGE ]]; then
    BUILD_OPTS="${BUILD_OPTS} -DMICROHSM_CODE_COVERAGE=ON"
fi

# Always build target
build

if [[ -n $RUNTEST ]]; then
    build
    if [[ $? -eq 0 ]]; then
        run_test
    fi
fi

if [[ -n $RUNEXAMPLES ]]; then
    run_examples
fi

if [[ -n $PRINT_COVERAGE ]]; then
    gcovr -r ${ROOT_DIR}/src ${ROOT_DIR}
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
