cc_library(
    name = "ssl_tools",
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
        "//Comm:comm",
        "//Comm/TLog:tlog",
        "//Libs/openssl:ssl",
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
        "//Comm/SSLTools:ssl_tools",
    ],
    copts = [
        "-std=c++11",
    ],
)

