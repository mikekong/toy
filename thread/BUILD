load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "mutex_test",
    srcs = ["mutex.cc"],
    linkopts = ["-pthread"],
)

cc_binary(
    name = "thread_safe",
    srcs = ["thread_safe.cc"],
    linkopts = ["-pthread"],
    copts = ["-Wthread-safety"],
    deps = [
      "//util:mutexlock",
    ],
)
