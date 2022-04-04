cc_binary(
    name = "client",
    srcs = [
        "client.h",
        "client.cpp",
    ],
    hdrs = [
        "client.h",
    ],
    deps = [
        "//Comm/CommDef:commdef",
        "//Comm/TLog:tlog",
        "//Comm/SPATools:spa_tools",
        "//Comm/SSLTools:ssl_tools",
    ],
    copts = [
        "-std=c++11",
    ],
    visibility = ["//visibility:public"],
)

