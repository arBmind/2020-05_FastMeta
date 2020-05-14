import qbs.File
import qbs.FileInfo
import qbs.Probes
import qbs.Process
import qbs.TextFile

Module {
    condition: true

    // our result is the html page
    additionalProductTypes: ["html"]

    // enable C++17 mode in QtCreator by default
    // notice: only cpp.includePaths are used by this module when calling the compiler
    Depends { name: "cpp" }
    cpp.cxxLanguageVersion: "c++17"

    property varList inputNumber: []
    PropertyOptions {
        name: "inputNumber"
        description: "Files are benchmarked for each number individually. CPPBENCH_N is defined to the number."
    }

    property int maxSamples: 5
    PropertyOptions {
        name: "maxSamples"
        description: "How many samples should be taken for each inputNumber. Less samples may be taken when maxSamplingTime is reached. Default: 5"
    }

    property int maxSamplingTime: 2*60*1000 // [milliseconds]
    PropertyOptions {
        name: "maxSamplingTime"
        description: "If the total time for compiling samples reached the time limit no more samples are compiled. Default: 2 Minutes"
    }

    property string graphLabel: ""
    PropertyOptions {
        name: "graphLabel"
        description: "Label displayed for a file. Default: empty - derive label from the filename."
    }

    property string compilerLabel: {
        if (product.cpp.compilerName == 'cl.exe') return 'vs2019';
        if (product.cpp.compilerName.contains('clang')) return product.cpp.compilerName.replace('++', '');
        if (product.cpp.compilerName.contains('g++')) return product.cpp.compilerName.replace('++', 'cc');
        return product.cpp.compilerName;
    }
    PropertyOptions {
        name: "compilerLabel"
        description: "Compiler Label used in the description and dataVar. Default: derived from cpp.compilerName"
    }

    property string dataVar: {
        var p = product.name.replace(/[ \+\-_]+/g, '_');
        var c = compilerLabel.replace(/[ \+\-_]+/g, '_');
        return ['data', p, c].join('_');
    }
    PropertyOptions {
        name: "dataVar"
        description: "Javascript variable name that stores the benchmark results for a project. Default: derived from product.name and compilerLabel"
    }

    property var showGroups: []
    PropertyOptions {
        name: "showGroups"
        description: "Array of graphLabels that should be visible without any interaction. Default: empty - all graphs are visible"
    }

    property var hideGroups: []
    PropertyOptions {
        name: "hideGroups"
        description: "Array of graphLabels that should initially be hidden. Default: empty"
    }

    property string headline: 'CppBench for ' + product.name
    PropertyOptions {
        name: "headline"
        description: "Contents of the h1-tag of the result page. Default: \"CppBench for [product.name]\""
    }

    property string description: 'Using ' + product.cpp_bench.compilerLabel + " v" + [product.cpp.compilerVersionMajor, product.cpp.compilerVersionMinor, product.cpp.compilerVersionPatch].join('.') + " on " + product.qbs.hostOS
    PropertyOptions {
        name: "description"
        description: "Contents for a p-tag above the chart on the result page. Default: \"Using [compilerLabel] [compilerVersion] on [qbs.hostOS]\""
    }

    Group {
        name: "[cpp_bench] ERB Template"
        files: "result_page.html.erb"
        fileTags: "cpp_bench_erb_template"
    }
    Group {
        name: "[cpp_bench] Ruby Script"
        files: "result_page.rb"
        fileTags: "cpp_bench_ruby_script"
    }
    Group {
        name: "[cpp_bench] Javascript Assets"
        files: [
            "d3/d3.js",
            "cpp_chart.js",
        ]
        fileTags: ["cpp_bench_assets"]
    }
    Group {
        name: "[cpp_bench] Powershell Wrapper"
        condition: qbs.hostOS.contains('windows')
        files: "peak_memory.ps1"
        fileTags: ["cpp_bench_powershell"]
    }

    // Discover installed Ruby
    property pathList rubySearchPaths: "C:\\C\\Ruby\\27\\bin"
    Probes.BinaryProbe {
        id: rubyProbe
        names: "ruby"
        platformSearchPaths: base.concat(rubySearchPaths)
    }
    property string rubyFilePath: rubyProbe.filePath

    // Discover installed Powershell
    Probes.BinaryProbe {
        id: powershellProbe
        names: "powershell"
    }
    property string powershellFilePath: powershellProbe.filePath

    validate: {
        if (!File.exists(rubyFilePath))
            throw "Ruby '" + rubyFilePath + "' does not exist.";
        if (qbs.hostOS.contains('windows') && !File.exists(powershellFilePath))
            throw "Powershell '" + powershellFilePath + "' does not exist.";
    }

    // cpp -> json_data (sampled data for each cpp file)
    Rule {
        inputs: ["cpp"]
        outputFileTags: ["cpp_bench_json_data"]
        auxiliaryInputs: ["cpp_bench_powershell_copied"]
        outputArtifacts: {
            var artifacts = [];
            for (var outIndex = 0; outIndex < input.cpp_bench.inputNumber.length; outIndex++) {
                var num = input.cpp_bench.inputNumber[outIndex];
                var artifact = {
                    filePath: input.baseName + "." + num + ".json",
                    fileTags: ["cpp_bench_json_data"]
                };
                artifacts.push(artifact);
            }
            return artifacts;
        }

        prepare: {
            var cmds = [];
            for (var outIndex = 0; outIndex < outputs['cpp_bench_json_data'].length; outIndex++) {
                var output = outputs['cpp_bench_json_data'][outIndex];
                var num = output.fileName.match(/\.(\d+)\.json$/).map(Number)[1];
                var cmd = new JavaScriptCommand();
                cmd.description = "benchmarking " + input.fileName + " with N="+ num + " -> " + output.fileName + " (" + input.qbs.toolchain[0] + " on "+input.qbs.hostOS+")";
                cmd.highlight = "compiler";
                cmd.jobPool = "benchmark";
                cmd.num = num;
                cmd.outIndex = outIndex;
                cmd.sourceCode = function() {
                    var inputFilePath = input.filePath;
                    var outputFilePath = outputs['cpp_bench_json_data'][outIndex].filePath;
                    var compileProcess = Process();
                    var compilerPath = input.cpp.compilerPath;
                    var includePaths = input.cpp.includePaths;
                    var astPrint = {};
                    if (input.qbs.toolchain.contains('clang')) {
                        var syntaxArgs = [
                            "-std=c++17", "-pedantic", // best C++17 mode
                            "-Xclang", "-ast-print", // print expanded sourcecode
                            "-fsyntax-only", // checks syntax only
                            "-DCPPBENCH_N="+num,
                            "-stdlib=libc++",
                        ];
                        for (var i in includePaths)
                            syntaxArgs.push('-I' + includePaths[i])

                        var baselineSyntaxArgs = ["-DBASELINE"];

                        var bashPath = '/bin/bash';
                        var awkScript = "| awk '"+'/template<>/ { tc++; } END { print "lines:", NR, "\\ntemplates:", tc }'+"'";

                        var bashArgs = [
                            '-c',
                            [compilerPath, syntaxArgs.concat(baselineSyntaxArgs, inputFilePath).join(' '), awkScript].join(' '),
                        ];
                        var compileResult = compileProcess.exec(bashPath, bashArgs, false);
                        var stdOut = compileProcess.readStdOut();
                        var baselineAstPrint = {
                            // bashPath: bashPath,
                            // bashArgs: bashArgs,
                            // stdOut: stdOut,
                            // stdErr: compileProcess.readStdErr(),
                            // compileResult: compileResult,
                            lineCount: stdOut.match(/lines:\s(\d+)/).map(Number)[1],
                            instanceCount: stdOut.match(/templates:\s(\d+)/).map(Number)[1],
                        };

                        var bashArgs = [
                            '-c',
                            [compilerPath, syntaxArgs.concat(inputFilePath).join(' '), awkScript].join(' '),
                        ];
                        var compileResult = compileProcess.exec(bashPath, bashArgs, false);
                        var stdOut = compileProcess.readStdOut();
                        astPrint = {
                            baseline: baselineAstPrint,
                            // bashPath: bashPath,
                            // bashArgs: bashArgs,
                            // stdOut: stdOut,
                            // stdErr: compileProcess.readStdErr(),
                            // compileResult: compileResult,
                            lineCount: stdOut.match(/lines:\s(\d+)/).map(Number)[1],
                            instanceCount: stdOut.match(/templates:\s(\d+)/).map(Number)[1],
                        };
                    }

                    var baselineCompileArgs;
                    var compileArgs;
                    var timePath;
                    var timeArgs;
                    var extractCompileTime = function(stdOut, stdErr) { return 0; };
                    var extractCompileRam = function(stdOut, stdErr) { return 0; };

                    if (input.qbs.toolchainType === 'msvc') {
                        extractCompileTime = function(stdOut, stdErr) {
                            var m = stdOut.match(/\=([\d\.]+)s[\r\n]/);
                            if (m) return m.map(Number)[1];
                            return 0;
                        };

                        // compilerPath = compilerPath.replace("cl.exe", "c1xx.dll");
                        compileArgs = [
                            "/nologo",
                            "/std:c++17", "/permissive-", "/Za", // best C++17 mode
                            "/Bt", // Generate Time Report
                            "/Zs", // Checks syntax only
                            "/DCPPBENCH_N="+num,
                        ];
                        for (var i in includePaths)
                            compileArgs.push('/I' + FileInfo.toWindowsSeparators(includePaths[i]))
                        baselineCompileArgs = ["/DBASELINE"];
                    }
                    else {
                        if (input.qbs.toolchain.contains('clang')) {
                            extractCompileTime = function(stdOut, stdErr) {
                                var m = stdErr.match(/Time: ([\d\.]+) seconds/);
                                if (m) return m.map(Number)[1];
                                return 0;
                            };
                        }
                        else if (input.qbs.toolchainType === 'gcc') {
                            extractCompileTime = function(stdOut, stdErr) {
                                var m = stdErr.match(/TOTAL\s+:\s+([\d\.]+)\s+([\d\.]+)/);
                                if (m) return m.map(Number)[1] + m.map(Number)[2];
                                return 0;
                            };
                        }

                        compileArgs = [
                            "-std=c++17", "-pedantic", // best C++17 mode
                            "-ftime-report", // Generate Time Report
                            "-fsyntax-only", // Checks syntax only
                            "-DCPPBENCH_N="+num,
                        ];
                        if (input.qbs.toolchain.contains('clang')) compileArgs.push("-stdlib=libc++");
                        for (var i in includePaths)
                            compileArgs.push('-I' + includePaths[i])

                        baselineCompileArgs = ["-DBASELINE"];
                    }

                    if (input.qbs.hostOS.contains('linux')) {
                        extractCompileRam = function(stdOut, stdErr) {
                            var m = stdErr.match(/'(\d+)'\n$/);
                            if (m) return m.map(Number)[1];
                            return 0;
                        };
                        timePath = '/usr/bin/time';
                        timeArgs = [
                            "-f", "'%M'", // print peak memory
                        ];
                    }
                    else if (input.qbs.hostOS.contains('windows')) {
                        extractCompileRam = function(stdOut, stdErr) {
                            var m = stdOut.match(/\{PeakWorkingSet64\=(\d+)\}/);
                            if (m) return m.map(Number)[1] / 1024; // report kb
                            return 0;
                        };
                        timePath = input.cpp_bench.powershellFilePath;
                        timeArgs = [
                            "-File",
                            FileInfo.joinPaths(product.buildDirectory, "peak_memory.ps1"),
                        ];
                    }

                    function compileRun(path, extraArgs) {
                        var args = compileArgs.concat(extraArgs, inputFilePath);
                        if (path == timePath) args = timeArgs.concat(compilerPath, args);
                        var compileResult = compileProcess.exec(path, args, true);
                        var stdOut = compileProcess.readStdOut();
                        var stdErr = compileProcess.readStdErr();
                        return {
                            args: args,
                            result: compileResult,
                            stdOut: stdOut,
                            stdErr: stdErr,
                            time: extractCompileTime(stdOut, stdErr),
                            ram: extractCompileRam(stdOut, stdErr),
                        };
                    }
                    var runPath = timePath || compilerPath;

                    var start = Date.now();
                    // warmup…
                    var warmup = compileRun(compilerPath, []);

                    // baseline
                    var baseline = compileRun(runPath, baselineCompileArgs);

                    // collect samples
                    var samples = [];
                    for (var i = 0; i < input.cpp_bench.maxSamples; i++) {
                        samples.push(compileRun(runPath, []));

                        var end = Date.now();
                        if (end - start > input.cpp_bench.maxSamplingTime) break;
                    }

                    var content = {
                        inputNumber: num,
                        sourceFile: input.filePath,
                        graphLabel: input.cpp_bench.graphLabel,
                        compilerPath: compilerPath,
                        args: compileArgs,
                        warmup: warmup,
                        baseline: baseline,
                        samples: samples,
                        astPrint: astPrint,
                        start: start,
                        clock: end-start,
                    };
                    compileProcess.close(); // done
                    var jsonFile = new TextFile(outputFilePath, TextFile.WriteOnly);
                    jsonFile.write(JSON.stringify(content, null, '  '));
                    jsonFile.close();
                };
                cmds.push(cmd);
            }
            return cmds;
        }
    }

    // link all json_data -> html
    Rule {
        multiplex: true
        inputs: ["cpp_bench_ruby_script", "cpp_bench_erb_template", "cpp_bench_json_data"]
        explicitlyDependsOn: ["cpp_bench_assets_copied"]
        Artifact {
            filePath: product.name + ' - ' + product.cpp_bench.compilerLabel + ".html"
            fileTags: "html"
        }
        prepare: {
            var scriptPath = inputs["cpp_bench_ruby_script"][0].filePath;
            var templatePath = inputs["cpp_bench_erb_template"][0].filePath;
            var dataFiles = inputs["cpp_bench_json_data"].map(function(f) { return f.filePath; });
            var args = [
                scriptPath,
                '--template', templatePath,
                '--output', output.filePath,
                '--dataVar', product.cpp_bench.dataVar,
                '--headline', product.cpp_bench.headline,
                '--description', product.cpp_bench.description,
                '--show', product.cpp_bench.showGroups.join(','),
                '--hide', product.cpp_bench.hideGroups.join(','),
                '--data', dataFiles.join(",")
            ];
            var cmd = new Command(product.cpp_bench.rubyFilePath, args);
            cmd.description = "rendering " + output.filePath;
            cmd.highlight = "linker";

            return [cmd];
        }
    }

    // install assets to output path
    Rule {
        inputs: ["cpp_bench_assets"]
        Artifact {
            filePath: input.fileName
            fileTags: ["cpp_bench_assets_copied"]
        }
        prepare: {
            var output = outputs["cpp_bench_assets_copied"][0];
            var cmd = new JavaScriptCommand();
            cmd.description = "coping " + input.fileName + " -> " + output.filePath;
            cmd.highlight = "codegen";
            cmd.sourceCode = function() {
                File.copy(input.filePath, output.filePath);
            }
            return [cmd];
        }
    }

    // install powershell script to make it usable
    Rule {
        inputs: ["cpp_bench_powershell"]
        Artifact {
            filePath: input.fileName
            fileTags: ["cpp_bench_powershell_copied"]
        }
        prepare: {
            var output = outputs["cpp_bench_powershell_copied"][0];
            var cmd = new JavaScriptCommand();
            cmd.description = "coping " + input.fileName + " -> " + output.filePath;
            cmd.highlight = "codegen";
            cmd.sourceCode = function() {
                File.copy(input.filePath, output.filePath);
            }
            return [cmd];
        }
    }
}
