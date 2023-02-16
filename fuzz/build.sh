#!/usr/bin/env bash

# get dependencies
git clone https://github.com/LedgerHQ/ethereum-plugin-sdk
git clone https://github.com/LedgerHQ/nanos-secure-sdk.git
export BOLOS_SDK=$(realpath nanos-secure-sdk/)

set -e

TARGETS="fuzz_calls"
SCRIPTDIR="$(cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd)"
BUILDDIR="$SCRIPTDIR/cmake-build-fuzz"

# Compile fuzzer
rm -rf "$BUILDDIR"
mkdir "$BUILDDIR"
cd "$BUILDDIR"

cmake -DCMAKE_C_COMPILER=clang ..
make clean
make $TARGETS