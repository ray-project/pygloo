workspace(name = "pygloo")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

# Group the sources of the library so that CMake rule have access to it
all_content = """filegroup(name = "all", srcs = glob(["**"]), visibility = ["//visibility:public"])"""

http_archive(
    name = "rules_python",
    strip_prefix = "rules_python-0.30.0",
    url = "https://wq-boost.oss-cn-beijing.aliyuncs.com/rules_python-0.30.0.tar.gz",
)
# if missing it will trig the error message of the OP
load("@rules_python//python:repositories.bzl", "py_repositories", "python_register_toolchains")
py_repositories()

python_register_toolchains(
    name = "python_3_11",
    python_version = "3.11",
    ignore_root_user_error = True,
)

# Rule repository
http_archive(
   name = "rules_foreign_cc",
   strip_prefix = "rules_foreign_cc-87df6b25f6c009883da87f07ea680d38780a4d6f",
   url = "https://github.com/bazelbuild/rules_foreign_cc/archive/87df6b25f6c009883da87f07ea680d38780a4d6f.zip",
   sha256 = "a45511a054598dd9b87d4d5765a18df4e5777736026087cf96ffc30704e6c918",
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
   strip_prefix = "pybind11_bazel-2.13.6",
   urls = ["https://github.com/pybind/pybind11_bazel/releases/download/v2.13.6/pybind11_bazel-2.13.6.zip"],
   sha256 = "9df284330336958c837fb70dc34c0a6254dac52a5c983b3373a8c2bbb79ac35e",
)

# We still require the pybind library.
http_archive(
   name = "pybind11",
   build_file = "@pybind11_bazel//:pybind11-BUILD.bazel",
   strip_prefix = "pybind11-2.13.6",
   # urls = ["https://github.com/pybind/pybind11/archive/v2.13.6.zip"],
   urls = ["https://wq-boost.oss-cn-beijing.aliyuncs.com/pybind11-2.13.6.zip"],
   # sha256 = "cdbe326d357f18b83d10322ba202d69f11b2f49e2d87ade0dc2be0c5c34f8e2a",
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
   sha256 = "a146136bb6efdac0e3ede952d09aec44b771a87ebc713bd815c3a90a7428c908",
)

# load("@pybind11_bazel//:python_configure.bzl", "python_configure_pybind")
# python_configure_pybind(name = "local_config_python")


