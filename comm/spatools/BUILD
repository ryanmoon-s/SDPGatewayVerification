cc_library(
    name = "spa_tools",
    srcs = [
        "spa_tool.h",
        "spa_tool.cpp",
    ],
    hdrs = [
        "spa_tool.h",
    ],
    deps = [
        "//comm/commdef:comm_def",
        "//comm/tlog:tlog",
        "//comm/ssltools:ssl_tools",
        "//comm/proto:spa_cc_pb",
    ],
    copts = [
        "-std=c++11",
    ],
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "spa_test",
    srcs = [
        "spa_test.cpp",
    ],
    deps = [
        "//comm/spatools:spa_tools",
    ],
    copts = [
        "-std=c++11",
    ],
)

