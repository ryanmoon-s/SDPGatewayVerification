#ifndef SSL_TOOLS_H
#define SSL_TOOLS_H

#include "ssl_def.h"

enum RSAOP {
    PUB_ENCRYPT, // 公钥加密，私钥解密
    PRI_ENCRYPT, // 私钥加密，公钥解密
};

struct SSL_Data {
    SSL_CTX     *ctx;
    SSL         *ssl;
};

class SSLTools {
public:
    // 加解密方法
    int RSAEncrypt(std::string& to_text, std::string text, std::string key_path, RSAOP op); 
    int RSADecrypt(std::string& to_text, std::string text, std::string key_path, RSAOP op);
    int MD5Encrypt(std::string& to_text, std::string text);

    // SSL加密通道
    // step 1
    int SSL_Init();
    // step 2
    int SSL_LoadCertificate(std::string cert, std::string pri_key);
    // step 3
    int SSL_BindSocket(int fd);
    // step 4
    int SSL_DumpCertInfo();

private:
    SSL_Data ssl_data_;
};

#endif
