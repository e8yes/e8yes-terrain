#!/bin/bash

mkdir -p bin

qmake ../e8islands.pro
make -j `nproc`

find ../renderer/shader -type f -exec glslc {} -o {}.spv \;
mv ../renderer/shader/*.spv ./bin

find . -name *.so* -not -path "./bin/*" -exec cp -P {} bin/ \;
find . -type f -executable -not -path "./bin/*" -name *main -exec cp -f {} bin/ \;
