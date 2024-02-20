load("@rules_foreign_cc//tools/build_defs:cmake.bzl", "cmake_external")
load("@rules_foreign_cc//tools/build_defs:make.bzl", "make")
load("@rules_cc//cc:defs.bzl", "cc_library")
load("@pybind11_bazel//:build_defs.bzl", "pybind_library")
load("@pybind11_bazel//:build_defs.bzl", "pybind_extension")

cmake_external(
    name = "libuv",
    # Values to be passed as -Dkey=value on the CMake command line;
    # here are serving to provide some CMake script configuration options
    env_vars = {"CFLAGS": "-fPIC"},
    lib_source = "@libuv//:all",

    out_lib_dir = "lib",
    # We are selecting the resulting static library to be passed in C/C++ provider
    # as the result of the build;
    static_libraries = ["libuv_a.a"],
)

make(
    name = "hiredis",

    out_include_dir =  "include/hiredis",

    lib_source = "@hiredis//:all",
    # We are selecting the resulting static library to be passed in C/C++ provider
    # as the result of the build;
    static_libraries = ["libhiredis.a"],
)

cmake_external(
    name = "gloo",
    # Values to be passed as -Dkey=value on the CMake command line;
    # here are serving to provide some CMake script configuration options
    cache_entries = {
        "libuv_LIBDIR": "$EXT_BUILD_DEPS/libuv/lib",
        "libuv_INCLUDE_DIRS": "$EXT_BUILD_DEPS/libuv/include",

        "HIREDIS_NESTED_INCLUDE": "off", # 'on' use hiredis/hiredis.h, 'off' use hiredis.h.
        "HIREDIS_ROOT_DIR":"$EXT_BUILD_DEPS/hiredis",
        "HIREDIS_INCLUDE_DIR": "$EXT_BUILD_DEPS/hiredis/include/hiredis",
        "HIREDIS_LIB_DIR": "$EXT_BUILD_DEPS/hiredis/lib",

        "USE_REDIS": "on",
        "USE_IBVERBS": "off",
        "USE_NCCL": "off",
        "USE_RCCL": "off",
        "USE_LIBUV": "on",
        "USE_TCP_OPENSSL": "off",
    },

    deps = [
        ":libuv",
        ":hiredis",
    ],

    lib_source = "@gloo//:all",

    # We are selecting the resulting static library to be passed in C/C++ provider
    # as the result of the build;
    static_libraries = ["libgloo.a"],
)

pybind_library(
    name = "pygloo-deps",
    srcs = glob(["src/*.cc"]),
    hdrs = glob(["include/*.h"]),
    strip_include_prefix = "include",
    visibility = ["//visibility:public"],
    deps = [":gloo"]
)

pybind_extension(
    name = "pygloo",
    srcs = ["main.cc"],
    deps = [":gloo", ":pygloo-deps"],
)