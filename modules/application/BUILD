cc_binary(
    name = "sdp_application",
    srcs = [
        "sdp_application.h",
        "sdp_application.cpp",
        "sdp_application_config.h",
        "sdp_application_config.cpp",
        "sdp_application_service_impl.h",
        "sdp_application_service_impl.cpp",
        "sdp_application_runner.cpp",
    ],
    deps = [
        "//comm/commdef:comm_def_lib",
        "//comm/commdef:comm_tool_lib",
        "//comm/tlog:tlog_lib",
        "//comm/spatools:spa_tools_lib",
        "//comm/ssltools:ssl_tools_lib",
        "//comm/proto:spa_cc_pb",
        "//comm/iptools:ip_tools_lib",
        "//comm/erpc:erpc_client_lib",
        "//comm/erpc:erpc_server_lib",
    ],
    copts = [
        "-std=c++11",
    ],
    visibility = ["//visibility:public"],
)
