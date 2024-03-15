import qbs

CppApplication {
    consoleApplication: true
    files: "main.cpp"

    cpp.cxxLanguageVersion: "c++17"

    cpp.cxxFlags: [
                   "--pedantic", // best C++ compatibility
                   "-Wall", "-Wextra"
    ]

    Group {     // Properties for the produced executable
        fileTagsFilter: "application"
        qbs.install: true
        qbs.installDir: "bin"
    }
}
