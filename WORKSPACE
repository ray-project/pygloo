workspace(name = "pygloo")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

# Group the sources of the library so that CMake rule have access to it
all_content = """filegroup(name = "all", srcs = glob(["**"]), visibility = ["//visibility:public"])"""

# Corrected: Using official GitHub release URL
http_archive(
    name = "rules_python",
    sha256 = "3b8b4cdc991bc9def8833d118e4c850f1b7498b3d65d5698eea92c3528b8cf2c",
    strip_prefix = "rules_python-0.30.0",
    url = "https://github.com/bazelbuild/rules_python/releases/download/0.30.0/rules_python-0.30.0.tar.gz",
)

load("@rules_python//python:repositories.bzl", "py_repositories", "python_register_toolchains")
py_repositories()

python_register_toolchains(
    name = "python_3_11",
    python_version = "3.11",
    ignore_root_user_error = True,
)

# Correct: This is the only, correct definition of rules_foreign_cc
http_archive(
   name = "rules_foreign_cc",
   strip_prefix = "rules_foreign_cc-87df6b25f6c009883da87f07ea680d38780a4d6f",
   url = "https://github.com/bazelbuild/rules_foreign_cc/archive/87df6b25f6c009883da87f07ea680d38780a4d6f.zip",
   sha256 = "a45511a054598dd9b87d4d5765a18df4e5777736026087cf96ffc30704e6c918",
)

load("@rules_foreign_cc//:workspace_definitions.bzl", "rules_foreign_cc_dependencies")
rules_foreign_cc_dependencies()

# REMOVED the second, incorrect definition of rules_foreign_cc that pointed to opencensus-proto.

http_archive(
   name = "pybind11_bazel",
   strip_prefix = "pybind11_bazel-2.13.6",
   urls = ["https://github.com/pybind/pybind11_bazel/releases/download/v2.13.6/pybind11_bazel-2.13.6.zip"],
   sha256 = "9df284330336958c837fb70dc34c0a6254dac52a5c983b3373a8c2bbb79ac35e",
)

# Corrected: Using official GitHub URL and uncommented sha256
http_archive(
    name = "pybind11",
    build_file = "@pybind11_bazel//:pybind11-BUILD.bazel",
    sha256 = "d0a116e91f64a4a2d8fb7590c34242df92258a61ec644b79127951e821b47be6",
    strip_prefix = "pybind11-2.13.6",
    urls = ["https://github.com/pybind/pybind11/archive/refs/tags/v2.13.6.zip"],
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

# Corrected: Using up-to-date official gloo repository
http_archive(
    name = "gloo",
    build_file_content = all_content,
    strip_prefix = "gloo-0.30.4",
    urls = ["https://github.com/facebookincubator/gloo/archive/refs/tags/v0.30.4.tar.gz"],
    sha256 = "2317c2f1505553e144a82a0b784df17d7e3fb6334a170562d475c820f4c0b432",
)
