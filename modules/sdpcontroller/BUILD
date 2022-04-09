cc_binary(
    name = "sdp_controller",
    srcs = [
        "sdp_controller.h"
        "sdp_controller.cpp"
    ],
    hdrs = [
        "sdp_controller.h"
    ],
    deps = [
        "//comm/commdef:comm_def",
        "//comm/tlog:tlog",
        "//comm/ssltools:ssl_tools",
        "//comm/spatools:spa_tools",
    ],
    copts = [
        "-std=c++11",
    ],
    visibility = ["//visibility:public"],
)
