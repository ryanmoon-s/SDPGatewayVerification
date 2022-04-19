cc_binary(
    name = "controller",
    srcs = [
        "controller.h",
        "controller.cpp",
        "controller_erpc_serviceimpl.h",
        "controller_erpc_serviceimpl.cpp",
        "controller_erpc_ipwhitelistimpl.h",
        "controller_erpc_ipwhitelistimpl.cpp",
        "controller_tool.h",
        "controller_tool.cpp",
        "controller_runner.cpp",
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
