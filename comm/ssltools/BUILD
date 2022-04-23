cc_library(
    name = "ssl_tools_lib",
    srcs = [
        "ssl_def.h",
        "ssl_tool.h",
        "ssl_tool.cpp",
    ],
    hdrs = [
        "ssl_def.h",
        "ssl_tool.h",
    ],
    deps = [
        "//comm/commdef:comm_def_lib",
        "//comm/tlog:tlog_lib",
        "//lib3rd/openssl:ssl",
    ],
    copts = [
        "-std=c++11",
    ],
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "ssl_test",
    srcs = [
        "ssl_test.cpp",
    ],
    deps = [
        "//comm/ssltools:ssl_tools_lib",
    ],
    copts = [
        "-std=c++11",
    ],
)


