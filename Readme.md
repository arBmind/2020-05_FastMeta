# Faster ►► C++ Meta-Programming

Presentation for C++ Usergroup Dresden (13. Mai 2020)

## Video

[![Youtube Recording of the Talk](http://img.youtube.com/vi/CmMRacxNIB4/0.jpg)](https://youtu.be/CmMRacxNIB4)

[Interactive Slides](https://arbmind.github.io/2020-05_FastMeta/)

## Mentioned Links

* [Metashell](http://metashell.org) - Clang based interactive shell
* [Metabench](http://metaben.ch) - collection of compile time C++ benchmarks
* [Youtube](https://www.youtube.com/watch?v=TyiiNVA1syk) - Jorg Brown (CppCon 2019) “Reducing Template Compilation Overhead, Using C++11, 14, 17, and 20.”

## Experiments

The `experiments` folder contains all my compile time benchmarks and experimental implementations

Build instructions:
* Install [Ruby](https://www.ruby-lang.org)
* Install [Qbs](https://qbs.io) - powerful open source build system
* Configure Qbs profiles for your compilers
* Compile
    * `cd experiments`
    * `qbs build -d [build-folder] -j [N] profile:[Profile]`
    * use `-p [Target]` to only compile one benchmark

Build using QtCreator:
* Install [Ruby](https://www.ruby-lang.org)
* Install QtCreator - this includes Qbs
* Configure some Kits with your compilers
* Open `experiments.qbs` as a project in QtCreator
* Compile

Benchmarked third party libraries:
* [Basicpp17](https://github.com/basicpp17/basicpp17) - A C++17 base library
* [Boost.Hana](https://github.com/boostorg/hana) - Your standard library for metaprogramming
* Co-Cpp19 - some compile time optimized implementations
* [EASTL](https://github.com/electronicarts/EASTL) - EA Standard Template Library
* [ETL](https://github.com/ETLCPP/etl) - Embedded Template Library
* [Folly](https://github.com/facebook/folly) - Facebook Open-source Library
* [Kvasir::mpl](https://github.com/kvasir-io/mpl) - collection of zero cost statically checked libraries
* [Mapbox Variant](https://github.com/mapbox/variant) - An header-only alternative to boost::variant for C++11 and C++14
* [MPark.Variant](https://github.com/mpark/variant) - C++17 `std::variant` for C++11/14/17

## Slides Tech

* [AsciiDoctor](https://github.com/asciidoctor/asciidoctor) as hackable html slide generator with a lot of features
* [Bespoke.js](https://github.com/bespokejs/bespoke) as a flexible html slide framework
* [Patched AsciiDoctor-Bespoke](https://github.com/arBmind/asciidoctor-bespoke/tree/patch-1) that allows the nested code display
* [Patched Bespoke-Bullets](https://github.com/arBmind/bespoke-bullets) to allow better interactions through API
* [Patched Bespoke-Hash](https://github.com/arBmind/bespoke-hash) that uses the extended bullets API
* [Patched Bespoke-OnStage](https://github.com/arBmind/bespoke-onstage) with some customizations and fixes for my setup
* [Gulp](https://github.com/gulpjs/gulp) to automate regeneration and push slide updates to the browser

## License

The slides here are available under the terms of the Creative Commons Attribution-ShareAlike license.
[(cc-by-sa-license)](https://creativecommons.org/licenses/by-sa/2.0/)

Photos

* ["fast"](https://flic.kr/p/21TsL2g) by Derren Hodson (modified: added text) [(cc-by-license)](https://creativecommons.org/licenses/by/2.0/)
