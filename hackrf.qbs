import qbs

CppApplication {
    consoleApplication: true
    files: "*.cpp"

    cpp.cxxLanguageVersion: "c++17"

    cpp.cxxFlags: [
                   "--pedantic", // best C++ compatibility
                   "-Wall", "-Wextra"
    ]

    cpp.includePaths: [
        ".."
    ]

    Group {     // Properties for the produced executable
        fileTagsFilter: "application"
        qbs.install: true
        qbs.installDir: "bin"
    }
}
