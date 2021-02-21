load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "consumer",
    srcs = ["consumer.cpp"],
    deps = [
        "//queue/comm:ring",
    ],
)
