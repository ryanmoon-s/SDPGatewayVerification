#!/bin/bash

src=`pwd`
BAZEL="bazel build --incompatible_blacklisted_protos_requires_proto_info=false"

cd ${src}/client
$BAZEL :client_test

cd ${src}/client
$BAZEL :client_test

cd ${src}/controller
$BAZEL :sdp_controller

cd ${src}/gateway
$BAZEL :sdp_gateway

cd ${src}/application
$BAZEL :sdp_application
