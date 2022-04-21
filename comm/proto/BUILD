load("@rules_cc//cc:defs.bzl", "cc_proto_library")
load("@rules_proto//proto:defs.bzl", "proto_library")

############### spa ###############
proto_library(
    name = "spa_proto",
    srcs = ["spa.proto"],
)

cc_proto_library(
    name = "spa_cc_pb",
    deps = [":spa_proto"],
    visibility = ["//visibility:public"],
)

############### erpc ###############
proto_library(
    name = "erpc_proto",
    srcs = ["erpc.proto"],
)

cc_proto_library(
    name = "erpc_cc_pb",
    deps = [":erpc_proto"],
    visibility = ["//visibility:public"],
)

############### controller ###############
proto_library(
    name = "controller_proto",
    srcs = ["controller.proto"],
)

cc_proto_library(
    name = "controller_cc_pb",
    deps = [
        ":controller_proto",
        ":spa_proto",
    ],
    visibility = ["//visibility:public"],
)

############### gateway ###############
proto_library(
    name = "gateway_proto",
    srcs = ["gateway.proto"],
)

cc_proto_library(
    name = "gateway_cc_pb",
    deps = [":gateway_proto"],
    visibility = ["//visibility:public"],
)
