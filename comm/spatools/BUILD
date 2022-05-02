cc_library(
    name = "spa_tools_lib",
    srcs = [
        "spa_tool.h",
        "spa_tool.cpp",
    ],
    hdrs = [
        "spa_tool.h",
    ],
    deps = [
        "//comm/commdef:comm_def_lib",
        "//comm/commdef:comm_tool_lib",
        "//comm/tlog:tlog_lib",
        "//comm/ssltools:ssl_tools_lib",
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
        "//comm/spatools:spa_tools_lib",
    ],
    copts = [
        "-std=c++11",
    ],
)

