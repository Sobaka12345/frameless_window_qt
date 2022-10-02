#!/bin/bash

build_conf=${1,,}

if ! command -v python3 &> /dev/null; then
    echo "python3 could not be found"
    exit
fi;

shopt -s nocasematch;
if [[ ${build_conf} != "debug" ]] && [[ ${build_conf} != "release" ]]; then
    build_conf="debug"
    echo "Using default build type: $build_conf"
fi;
shopt -u nocasematch;

mkdir build &> /dev/null
cd build
mkdir $build_conf &> /dev/null
cd $build_conf

python -m venv ./ venv
source ./venv/bin/activate

pip3 install wheel
pip3 install conan

cmake ../.. -DCMAKE_BUILD_TYPE=$build_conf -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DPROFILE_PATH="../../conan/conan_profile_linux_"$build_conf".txt"
