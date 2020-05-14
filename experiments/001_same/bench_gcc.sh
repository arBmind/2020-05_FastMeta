#!/bin/bash
V=${1:-001} # select benchmark
N=${2:-300} # N*N = number of types
shopt -s nullglob
F=( ./bench$V*.cpp ./$V*.cpp $V* )
echo "Benchmarking ${F[0]} with N=$N"
/usr/bin/time -f '%M' g++-9 -ftime-report -fsyntax-only -std=c++17 -DN=$N ${F[0]}
