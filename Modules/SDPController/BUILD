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
        "//Comm/CommDef:commdef",
        "//Comm/TLog:tlog",
        "//Comm/SSLTools:ssl_tools",
        "//Comm/SPATools:spa_tools",
    ],
    copts = [
        "-std=c++11",
    ],
    visibility = ["//visibility:public"],
)
