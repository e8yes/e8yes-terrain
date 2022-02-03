#!/bin/bash

qmake ../e8islands.pro
make -j `nproc`

find . -name *.so* -not -path "./bin/*" -exec cp -P {} bin/ \;
find . -type f -executable -not -path "./bin/*" -name *main -exec cp -f {} bin/ \;
