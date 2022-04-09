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
        "//Comm/CommDef:comm_def",
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
        "//Comm/TLog:tlog",
        "//Comm/Proto:spa_cc_pb",
    ],
    copts = [
        "-std=c++11",
    ],
)

