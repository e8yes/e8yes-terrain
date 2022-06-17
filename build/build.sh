#!/bin/bash

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
find ../renderer/shader -type f -name *.vert -exec glslc {} -o {}.spv \;
find ../renderer/shader -type f -name *.frag -exec glslc {} -o {}.spv \;
mv ../renderer/shader/*.spv ./bin

# C++ code compilation.
qmake ../e8islands.pro
make -j `nproc`

find . -name *.so* -not -path "./bin/*" -exec cp -P {} bin/ \;
find . -type f -executable -not -path "./bin/*" -name *main -exec cp -f {} bin/ \;
