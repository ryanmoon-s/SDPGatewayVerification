cc_library(
    name = "comm_def_lib",
    srcs = [
    ],
    hdrs = [
        "comm_def.h",
    ],
    deps = [
    ],
    copts = [
        "-std=c++11",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "comm_tool_lib",
    srcs = [
        "comm_tool.cpp",
    ],
    hdrs = [
        "comm_tool.h",
    ],
    deps = [
        "//comm/tlog:tlog_lib",
        "//comm/commdef:comm_def_lib",
        "//comm/proto:spa_cc_pb",
    ],
    copts = [
        "-std=c++11",
    ],
    visibility = ["//visibility:public"],
)
