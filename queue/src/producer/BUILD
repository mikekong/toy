load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "producer",
    srcs = ["producer.cpp"],
    deps = [
        "//queue/comm:ring",
    ],
)
