#pragma once

#include "comm/proto/erpc.pb.h"
#include "comm/erpc/erpc_client.h"
#include "comm/tlog/tlog.h"
#include <vector>

class AccessClient {
public:
    AccessClient() :erpc_client_(SSL_CRT_CLIENT, SSL_KEY_CLIENT) {}

public:
    // 访问应用
    int AccessApplication_HTTPS(const std::vector<erpc::AccessItem>& list);

private:
    ErpcClient erpc_client_;
};
