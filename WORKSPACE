workspace(name = "pygloo")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

# Group the sources of the library so that CMake rule have access to it
all_content = """filegroup(name = "all", srcs = glob(["**"]), visibility = ["//visibility:public"])"""

# Rule repository
http_archive(
   name = "rules_foreign_cc",
   strip_prefix = "rules_foreign_cc-master",
   url = "https://github.com/bazelbuild/rules_foreign_cc/archive/master.zip",
)

load("@rules_foreign_cc//:workspace_definitions.bzl", "rules_foreign_cc_dependencies")

# Call this function from the WORKSPACE file to initialize rules_foreign_cc
#  dependencies and let neccesary code generation happen
#  (Code generation is needed to support different variants of the C++ Starlark API.).
#
#  Args:
#    native_tools_toolchains: pass the toolchains for toolchain types
#      '@rules_foreign_cc//tools/build_defs:make_toolchain',
#      '@rules_foreign_cc//tools/build_defs:cmake_toolchain' and
#      '@rules_foreign_cc//tools/build_defs:ninja_toolchain' with the needed platform constraints.
#      If you do not pass anything, registered default toolchains will be selected (see below).
#
#    register_default_tools: if True, the make, cmake and ninja toolchains, calling corresponding
#      preinstalled binaries by name (make, cmake, ninja) will be registered after
#      'native_tools_toolchains' without any platform constraints.
#      The default is True.
rules_foreign_cc_dependencies()


http_archive(
    name = "rules_foreign_cc",
    strip_prefix = "opencensus-proto-0.3.0/src",
    urls = ["https://github.com/census-instrumentation/opencensus-proto/archive/v0.3.0.tar.gz"],
    sha256 = "b7e13f0b4259e80c3070b583c2f39e53153085a6918718b1c710caf7037572b0",
)

http_archive(
   name = "pybind11_bazel",
   strip_prefix = "pybind11_bazel-f4f1bd4fa4b368b79dd6f003f8ef8c5a91fad36b",
   urls = ["https://github.com/Ezra-H/pybind11_bazel/archive/f4f1bd4fa4b368b79dd6f003f8ef8c5a91fad36b.zip"],
)

# We still require the pybind library.
http_archive(
   name = "pybind11",
   build_file = "@pybind11_bazel//:pybind11.BUILD",
   strip_prefix = "pybind11-2.6.1",
   urls = ["https://github.com/pybind/pybind11/archive/v2.6.1.tar.gz"],
)

http_archive(
   name = "libuv",
   build_file_content = all_content,
   strip_prefix = "libuv-1.40.0",
   urls = ["https://github.com/libuv/libuv/archive/v1.40.0.tar.gz"],
   sha256 = "70fe1c9ba4f2c509e8166c0ca2351000237da573bb6c82092339207a9715ba6b",
)

http_archive(
   name = "hiredis",
   build_file_content = all_content,
   strip_prefix = "hiredis-1.0.0",
   urls = ["https://github.com/redis/hiredis/archive/v1.0.0.tar.gz"],
   sha256 = "2a0b5fe5119ec973a0c1966bfc4bd7ed39dbce1cb6d749064af9121fe971936f",
)

# gloo source code repository
http_archive(
   name = "gloo",
   build_file_content = all_content,
   strip_prefix = "gloo-add3f38c6a2715e9387f4966b4fc3d92bb786adb",
   urls = ["https://github.com/Ezra-H/gloo/archive/add3f38c6a2715e9387f4966b4fc3d92bb786adb.tar.gz"],
   # add3f38c6a2715e9387f4966b4fc3d92bb786adb remove libuv pkg_search, redis macro added. build find with hiredis
   # c1191d67288f5262577a336a23c546840361d857 activate hiredis/hiredis.h not work with hiredis
   # be8fd671006968d4c39b84fe9c3f8cd4af2e51a2 remove libuv pkg_search command
   # 4279d78bcc7b04e975f94840a6ef30b086a8b6fe worked
)

load("@pybind11_bazel//:python_configure.bzl", "python_configure_pybind")
python_configure_pybind(name = "local_config_python")


