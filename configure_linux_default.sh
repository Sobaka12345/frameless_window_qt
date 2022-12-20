#!/bin/bash

build_conf=$1

if ! command -v python3 &> /dev/null; then
    echo "python3 could not be found"
    exit
fi;

if [[ ${build_conf} != "Debug" ]] && [[ ${build_conf} != "Release" ]]; then
    build_conf="Debug"
    echo "Using default build type: $build_conf"
fi;

mkdir build &> /dev/null

python -m venv venv
source ./venv/bin/activate

pip3 install conan

conan install . -if=build/$build_conf -s build_type=$build_conf --build=missing