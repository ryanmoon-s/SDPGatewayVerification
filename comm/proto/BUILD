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
    # 引用其它proto在此引spa_proto
    deps = ["spa_proto"],
)

cc_proto_library(
    name = "erpc_cc_pb",
    deps = [":erpc_proto"],
    visibility = ["//visibility:public"],
)
