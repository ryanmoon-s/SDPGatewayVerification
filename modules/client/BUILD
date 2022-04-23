cc_library(
    name = "client_lib",
    srcs = [
        "client.cpp",
    ],
    hdrs = [
        "client.h",
    ],
    deps = [
        "//comm/commdef:comm_def_lib",
        "//comm/commdef:comm_tool_lib",
        "//comm/tlog:tlog_lib",
        "//comm/spatools:spa_tools_lib",
        "//comm/ssltools:ssl_tools_lib",
        "//comm/proto:spa_cc_pb",
        "//comm/proto:erpc_cc_pb",
        "//comm/erpc:erpc_client_lib",
    ],
    copts = [
        "-std=c++11",
    ],
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "client_test",
    srcs = [
        "client_test.cpp",
    ],
    deps = [
        ":client_lib",
    ],
    copts = [
        "-std=c++11",
    ],
)
