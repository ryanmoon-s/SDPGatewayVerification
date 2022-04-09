cc_binary(
    name = "client",
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
    ],
    copts = [
        "-std=c++11",
    ],
    visibility = ["//visibility:public"],
)

