cc_library(
    name = "spa_tools",
    srcs = [
        "spa_tool.h",
        "spa_tool.cpp",
    ],
    hdrs = [
        "spa_tool.h",
    ],
    deps = [
        "//Comm/CommDef:commdef",
        "//Comm/TLog:tlog",
        "//Comm/SSLTools:ssl_tools",
    ],
    copts = [
        "-std=c++11",
    ],
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "spa_test",
    srcs = [
        "spa_test.cpp",
    ],
    deps = [
        "//Comm/SPATools:spa_tools",
    ],
    copts = [
        "-std=c++11",
    ],
)

