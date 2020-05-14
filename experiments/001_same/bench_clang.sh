#!/bin/bash
V=${1:-001} # select benchmark
N=${2:-1000} # N*N = number of types
shopt -s nullglob
F=( ./bench$V*.cpp ./$V*.cpp $V* )
echo "Benchmarking ${F[0]} with N=$N"
# /usr/bin/time -f '%M' clang++-9 -ftime-report -fsyntax-only -std=c++17 -stdlib=libc++ -DN=$N ${F[0]}
/usr/bin/time -f '%M' clang++-9 -ftime-report -fsyntax-only -std=c++17 -stdlib=libc++ -DN=$N ${F[0]}
