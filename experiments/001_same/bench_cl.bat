@echo off
:: Select Benchmark
IF "%1"=="" ( SET "V=001" ) ELSE ( SET "V=%1" )
:: N*N = number of types
IF "%2"=="" ( SET "N=100" ) ELSE ( SET "N=%2" )

call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" amd64 >NUL

FOR /r . %%a in (./bench%V%*.cpp ./%V%*.cpp %V%*) do (
    echo Benchmarking %%~nxa with N=%N%
    cl /nologo /std:c++17 /permissive- /Za /Bt /Zs /DN=%N% %%a
    goto :eof
)
