cc_binary(
    name = "client_lib",
    srcs = [
        "client.h",
        "client.cpp",
    ],
    hdrs = [
        "client.h",
    ],
    deps = [
        "//comm/commdef:comm_def",
        "//comm/tlog:tlog",
        "//comm/spatools:spa_tools",
        "//comm/ssltools:ssl_tools",
        "//comm/proto:spa_cc_pb",
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
