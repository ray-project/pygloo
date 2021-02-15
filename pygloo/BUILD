load("@rules_foreign_cc//tools/build_defs:cmake.bzl", "cmake_external")
load("@rules_foreign_cc//tools/build_defs:make.bzl", "make")
load("@pybind11_bazel//:build_defs.bzl", "pybind_library")
load("@pybind11_bazel//:build_defs.bzl", "pybind_extension")

cmake_external(
    name = "libuv",
    # Values to be passed as -Dkey=value on the CMake command line;
    # here are serving to provide some CMake script configuration options
    cache_entries = {},
    lib_source = "@libuv//:all",

    # We are selecting the resulting static library to be passed in C/C++ provider
    # as the result of the build;
    # However, the cmake_external dependants could use other artefacts provided by the build,
    # according to their CMake script
    static_libraries = ["libuv_a.a"],
)

make(
    name = "hiredis",

    cache_entries = {},
    lib_source = "@hiredis//:all",
    # We are selecting the resulting static library to be passed in C/C++ provider
    # as the result of the build;
    # However, the cmake_external dependants could use other artefacts provided by the build,
    # according to their CMake script
    static_libraries = ["libhiredis.a"],
)

cmake_external(
    name = "gloo",
    # Values to be passed as -Dkey=value on the CMake command line;
    # here are serving to provide some CMake script configuration options
    cache_entries = {
        "PKG_CONFIG_PATH": "$EXT_BUILD_ROOT/bazel-out/host/bin/pygloo/copy_libuv/libuv/lib/pkgconfig:$EXT_BUILD_ROOT/bazel-out/host/bin/pygloo/copy_hiredis/hiredis/lib/pkgconfig:$PKG_CONFIG_PATH",

        "libuv_LIBDIR": "$EXT_BUILD_ROOT/bazel-out/host/bin/pygloo/libuv/lib",
        "libuv_INCLUDE_DIRS": "$EXT_BUILD_ROOT/bazel-out/host/bin/pygloo/libuv/include",
        "libuv_LIBRARY": "$EXT_BUILD_ROOT/bazel-out/host/bin/pygloo/copy_hiredis/lib/libuv_a.a",

        "HIREDIS_INCLUDE_DIRS": "$EXT_BUILD_ROOT/bazel-out/host/bin/pygloo/copy_hiredis/hiredis/include",
        "HIREDIS_LIBRARIES": "$EXT_BUILD_ROOT/bazel-out/host/bin/pygloo/copy_hiredis/hiredis/lib",

        "USE_REDIS": "on",
        "USE_IBVERBS": "on",
        "USE_NCCL": "on",
        "USE_RCCL": "on",
        "USE_LIBUV": "off",
        "USE_TCP_OPENSSL": "off",
    },
    # tools_deps = [
    #     ":libuv",
    #     ":hiredis",
    # ],

    deps = [
        ":libuv",
        ":hiredis",
    ],

    lib_source = "@gloo//:all",

    # We are selecting the resulting static library to be passed in C/C++ provider
    # as the result of the build;
    # However, the cmake_external dependants could use other artefacts provided by the build,
    # according to their CMake script
    static_libraries = ["libgloo.a"],
)

pybind_library(
    name = "pygloo-deps",
    srcs = glob(["src/*.cc"]),
    hdrs = glob(["include/*.h"]),
    strip_include_prefix = "include",
    # copts = ["-Iinclude"],
    visibility = ["//visibility:public"],
    deps = [":gloo"]
)

pybind_extension(
    name = "pygloo",
    srcs = ["main.cc"],
    deps = [":gloo", ":pygloo-deps"],
)