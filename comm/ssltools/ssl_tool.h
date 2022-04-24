#pragma once

#include "ssl_def.h"

struct SSL_Data {
    SSL_CTX *ctx;
    SSL   *ssl;
};

// 用于管理一个ssl连接
// 建立ssl连接、传输数据
class SSLConnector {
public:
    // for client
    int SSLConnect(int fd);
    // for server
    int SSLAccept(int fd);

    // 数据透传
    int SSLWrite(const char* buf, int size);
    int SSLRead(char* buf, int size);

public:
    SSLConnector(const std::string& cert, const std::string& pri_key, int is_server);
    ~SSLConnector();

private:
    SSL_Data ssl_data_;
    int is_server_;

    int _SSL_Init();
    int _SSL_LoadCertificate(std::string cert, std::string ca_cert, std::string pri_key);
    int _SSL_DumpCertInfo();
};

class SSLTools {
public:
    // 公钥加密、私钥解密
    int RSAEncrypt(std::string& to_text, const std::string& text, const std::string& key);
    int RSADecrypt(std::string& to_text, const std::string& text, const std::string& key);
    
    // 私钥签名、公钥验证
    int RSASign(std::string& sig_text, const std::string& text, const std::string& key);
    int RSAVerify(const std::string& sig_text, const std::string& src_text, const std::string& key);

    // MD5摘要
    int MD5Encrypt(std::string& to_text, const std::string& text);
};
