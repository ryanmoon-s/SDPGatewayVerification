cc_binary(
    name = "sdp_appgateway",
    srcs = [
        "sdp_appgateway.h",
        "sdp_appgateway.cpp",
        "sdp_appgateway_service_impl.h",
        "sdp_appgateway_service_impl.cpp",
        "sdp_appgateway_whitelist_impl.h",
        "sdp_appgateway_whitelist_impl.cpp",
        "sdp_appgateway_tool.h",
        "sdp_appgateway_tool.cpp",
        "sdp_appgateway_runner.cpp",
    ],
    deps = [
        "//comm/commdef:comm_def_lib",
        "//comm/tlog:tlog_lib",
        "//comm/spatools:spa_tools_lib",
        "//comm/ssltools:ssl_tools_lib",
        "//comm/proto:spa_cc_pb",
        "//comm/erpc:erpc_client_lib",
        "//comm/erpc:erpc_server_lib",
    ],
    copts = [
        "-std=c++11",
    ],
    visibility = ["//visibility:public"],
)
