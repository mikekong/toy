load("@rules_cc//cc:defs.bzl", "cc_proto_library")
load("@rules_proto//proto:defs.bzl", "proto_library")

proto_library(
    name = "zip_code_proto",
    srcs = ["zip_code.proto"],
)

cc_proto_library(
    name = "zip_code_pb",
    deps = [":zip_code_proto"],
)

proto_library(
    name = "address_proto",
    srcs = ["address.proto"],
    deps = [":zip_code_proto"],
)

cc_proto_library(
    name = "address_pb",
    deps = [":address_proto"],
)

proto_library(
    name = "person_proto",
    srcs = ["person.proto"],
    deps = [":address_proto"],
)

cc_proto_library(
    name = "person_pb",
    deps = [":person_proto"],
)


