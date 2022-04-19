cc_library(
    name = "erpc_client_lib",
    srcs = [
        "epoll_dispatcher.cpp",
        "erpc_config.cpp",
        "erpc_handler.cpp",
        "erpc_service.cpp",
        "erpc_client.cpp",
    ],
    hdrs = [
        "erpc_client.h",
        "erpc_def.h",
        "erpc_config.h",
        "epoll_dispatcher.h",
        "erpc_handler.h",
        "erpc_service.h",
    ],
    deps = [
        "//comm/commdef:comm_def_lib",
        "//comm/tlog:tlog_lib",
        "//comm/ssltools:ssl_tools_lib",
        "//comm/proto:erpc_cc_pb",
        "//comm/proto:spa_cc_pb",
        "//comm/spatools:spa_tools_lib",
    ],
    copts = [
        "-std=c++11",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "erpc_server_lib",
    srcs = [
        "erpc_server.cpp",
        "epoll_dispatcher.cpp",
        "erpc_config.cpp",
        "erpc_handler.cpp",
        "erpc_service.cpp",
    ],
    hdrs = [
        "erpc_server.h",
        "erpc_def.h",
        "epoll_dispatcher.h",
        "erpc_config.h",
        "erpc_handler.h",
        "erpc_service.h",
    ],
    deps = [
        "//comm/commdef:comm_def_lib",
        "//comm/tlog:tlog_lib",
        "//comm/ssltools:ssl_tools_lib",
        "//comm/proto:erpc_cc_pb",
        "//comm/proto:spa_cc_pb",
        "//comm/spatools:spa_tools_lib",
    ],
    copts = [
        "-std=c++11",
    ],
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "erpc_test_client",
    srcs = [
        "erpc_test_client.cpp",
    ],
    deps = [
        "//comm/erpc:erpc_client_lib",
    ],
    copts = [
        "-std=c++11",
    ],
)

cc_binary(
    name = "erpc_test_server",
    srcs = [
        "erpc_test_server.cpp",
    ],
    deps = [
        "//comm/erpc:erpc_server_lib",
    ],
    copts = [
        "-std=c++11",
    ],
)
