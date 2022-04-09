cc_library(
    name = "tlog",
    srcs = [
        "tlog.h",
        "tlog.cpp",
    ],
    hdrs = [
        "tlog.h",
    ],
    deps = [
        "//comm/commdef:comm_def",
    ],
    copts = [
        "-std=c++11",
    ],
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "tlog_test",
    srcs = [
        "tlog_test.cpp",
    ],
    deps = [
        "//comm/tlog:tlog",
        "//comm/proto:spa_cc_pb",
    ],
    copts = [
        "-std=c++11",
    ],
)

