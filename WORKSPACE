load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

git_repository(
    name = "com_github_gflags_gflags",
    commit = "827c769e5fc98e0f2a34c47cef953cc6328abced",
    remote = "https://github.com/gflags/gflags.git",
)

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "com_github_google_glog",
    sha256 = "62efeb57ff70db9ea2129a16d0f908941e355d09d6d83c9f7b18557c0a7ab59e",
    strip_prefix = "glog-d516278b1cd33cd148e8989aec488b6049a4ca0b",
    urls = ["https://github.com/google/glog/archive/d516278b1cd33cd148e8989aec488b6049a4ca0b.zip"],
)

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "com_google_protobuf",
    sha256 = "f18a40816260a9a3190a94efb0fc26270b244a2436681602f0a944739095d632",
    strip_prefix = "protobuf-3.15.1",
    urls = ["https://github.com/protocolbuffers/protobuf/archive/v3.15.1.tar.gz"],
#    patches = [
#        "@//third_party:com_google_protobuf_fixes.diff"
#    ],
#    patch_args = [
#        "-p1",
#    ],
)

load("@com_google_protobuf//:protobuf_deps.bzl", "protobuf_deps")
protobuf_deps()

#workspace(name = "toy")
