@echo off
:: Select Benchmark
IF "%1"=="" ( SET "V=001" ) ELSE ( SET "V=%1" )
:: N*N = number of types
IF "%2"=="" ( SET "N=1000" ) ELSE ( SET "N=%2" )

set "path=C:\C\Cpp\LLVM10\bin\;%path%"

FOR /r . %%a in (./bench%V%*.cpp ./%V%*.cpp %V%*) do (
    echo Benchmarking %%~nxa with N=%N%
    clang++.exe -ftime-report -fsyntax-only -std=c++17 -DN=%N% %%a
    goto :eof
)
