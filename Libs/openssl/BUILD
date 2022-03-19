cc_library(
    name = "ssl",
    srcs = [

    ]+select({
    "//conditions:default":[        "lib64/libssl.a"],
    }),
    hdrs = [
    ],
    includes = ['.', 'include'],
    deps = [
        "//Libs/openssl:crypto",
    ],
    copts = [
    ],
    linkopts = [
    ],
    visibility = ["//visibility:public"],
)


cc_library(
    name = "crypto",
    srcs = [

    ]+select({
    "//conditions:default":[        "lib64/libcrypto.a"],
    }),
    hdrs = [
    ],
    includes = ['.', 'include'],
    deps = [
    ],
    copts = [
    ],
    linkopts = [
        "-ldl",
        "-lpthread",
    ],
    visibility = ["//visibility:public"],
)
