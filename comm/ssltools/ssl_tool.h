#pragma once

#include "ssl_def.h"

enum RSAOP {
    PUB_ENCRYPT, // 公钥加密，私钥解密
    PRI_ENCRYPT, // 私钥加密，公钥解密
};

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

    // data trans
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
    // 加解密方法
    int RSAEncrypt(std::string& to_text, std::string text, std::string key_path, RSAOP op); 
    int RSADecrypt(std::string& to_text, std::string text, std::string key_path, RSAOP op);
    int MD5Encrypt(std::string& to_text, std::string text);
};
