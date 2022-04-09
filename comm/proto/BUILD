load("@rules_cc//cc:defs.bzl", "cc_proto_library")
load("@rules_proto//proto:defs.bzl", "proto_library")

proto_library(
    name = "spa_proto",
    srcs = ["spa.proto"],
)

cc_proto_library(
    name = "spa_cc_pb",
    deps = [":spa_proto"],
    visibility = ["//visibility:public"],
)
