#!/bin/bash

set -e

mkdir -p bin

# Protobuf code compilation.
protoc --cpp_out=../resource/proto \
       --proto_path=../resource/proto \
       `find ../resource/proto -name "*.proto"`

protoc --cpp_out=../content/proto \
       --proto_path=../content/proto \
       `find ../content/proto -name "*.proto"`

protoc --cpp_out=../renderer/proto \
       --proto_path=../renderer/proto \
       `find ../renderer/proto -name "*.proto"`

# Shader code compilation.
shaders=$(find ../renderer/shader -type f -name "*.vert" -or -name "*.frag")
for shader in $shaders
do
    shader_name=$(basename $shader)
    echo "Compiling $shader_name"
    glslc "$shader" -o "bin/$shader_name.spv"
done

# C++ code compilation.
qmake ../e8islands.pro
make

find . -name "*.so*" -not -path "./bin/*" -exec cp -P {} bin/ \;
find . -name "*.dylib*" -not -path "./bin/*" -exec cp -P {} bin/ \;
find . -type f -not -path "./bin/*" -name "*main" -exec cp -f {} bin/ \;
