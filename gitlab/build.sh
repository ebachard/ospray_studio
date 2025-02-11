#!/bin/bash -ex
## Copyright 2015-2021 Intel Corporation
## SPDX-License-Identifier: Apache-2.0

PACKAGE=false
if [[ $# -gt 0 ]]
then
    if [[ "$1" == "package" ]]
    then
        PACKAGE=true
    fi
fi

THREADS=`nproc`

cmake --version

mkdir -p build && cd build

if [[ "$PACKAGE" == true ]]
then
    cmake -L \
        -D ENABLE_OPENIMAGEIO=OFF \
        -D ENABLE_OPENVDB=OFF \
        -D ENABLE_EXR=OFF \
        -D OSPSTUDIO_SIGN_FILE=$SIGN_FILE_LINUX \
        ..
    cmake --build . --parallel $THREADS --config Release
    cpack -B "${PWD}/package" -V
else
    cmake -L \
        -D CMAKE_INSTALL_PREFIX=install \
        -D ENABLE_OPENIMAGEIO=OFF \
        -D ENABLE_OPENVDB=OFF \
        -D ENABLE_EXR=OFF \
        ..
    cmake --build . --parallel $THREADS --config Release --target install
fi
