cc_library(
    name = "sdp_verify_client_lib",
    srcs = [
        "sdp_verify_client.cpp",
    ],
    hdrs = [
        "sdp_verify_client.h",
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

cc_library(
    name = "sdp_access_client_lib",
    srcs = [
        "sdp_access_client.cpp",
    ],
    hdrs = [
        "sdp_access_client.h",
    ],
    deps = [
        "//comm/tlog:tlog_lib",
        "//comm/proto:erpc_cc_pb",
        "//comm/erpc:erpc_client_lib",
    ],
    copts = [
        "-std=c++11",
    ],
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "test_client",
    srcs = [
        "test_client.cpp",
    ],
    deps = [
        ":sdp_verify_client_lib",
        ":sdp_access_client_lib",
    ],
    copts = [
        "-std=c++11",
    ],
)
