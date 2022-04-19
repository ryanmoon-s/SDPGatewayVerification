cc_binary(
    name = "sdp_controller",
    srcs = [
        "sdp_controller.h",
        "sdp_controller.cpp",
        "sdp_controller_erpc_serviceimpl.h",
        "sdp_controller_erpc_serviceimpl.cpp",
        "sdp_controller_tool.h",
        "sdp_controller_tool.cpp",
        "sdp_controller_runner.cpp",
    ],
    deps = [
        "//comm/commdef:comm_def_lib",
        "//comm/tlog:tlog_lib",
        "//comm/spatools:spa_tools_lib",
        "//comm/ssltools:ssl_tools_lib",
        "//comm/proto:spa_cc_pb",
        "//comm/erpc:erpc_server_lib",
    ],
    copts = [
        "-std=c++11",
    ],
    visibility = ["//visibility:public"],
)
