#ifndef RSA_TOOLS
#define RSA_TOOLS

#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <string>
#include <iostream>

#define PRIKEY "prikey.pem"
#define PUBKEY "pubkey.pem"
#define BUFFSIZE 1024*4

enum RSAOP {
    PUB_ENCRYPT, // 公钥加密，私钥解密
    PRI_ENCRYPT, // 私钥加密，公钥解密
};

class RSATools {
public:
    // op -> RSAOP
    int RSAEncrypt(std::string& en_text, std::string text, std::string key_path, int op); 
    int RSADecrypt(std::string& de_text, std::string text, std::string key_path, int op);
};




#endif
