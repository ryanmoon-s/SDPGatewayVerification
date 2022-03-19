#include "ssl_tool.h"
#include "Comm/comm_def.h"
#include "Comm/TLog/tlog.h"

int SSLTools::RSAEncrypt(std::string& to_text, std::string text, std::string key_path, RSAOP op) { 
    to_text.clear();

    FILE *fp = fopen(key_path.c_str(), "r");
    if (fp == NULL) {
        TLOG((ERR, "fopen error:%s, key_path:%s", strerror(errno), key_path.c_str()));
        return -1;
    }

    // -----BEGIN RSA PUBLIC KEY-----
    // RSA *rsa = PEM_read_RSAPublicKey(fp, NULL, NULL, NULL);
    
    // -----BEGIN PUBLIC KEY-----
    RSA *rsa = PEM_read_RSA_PUBKEY(fp, NULL, NULL, NULL);
    SSL_iAssert_NULL(rsa, "PEM_read_RSAPublicKey");

    // RSA_print_fp(stdout, rsa, 0);

    int rsa_size = RSA_size(rsa);
    char *to = (char*)malloc(rsa_size);
    memset(to, 0, rsa_size);

    /* 
     * 数据拆分加密 
     *
     * RSA_PKCS1_OAEP_PADDING模式下，加密时：
     * 每轮输入数据大小不能超过rsa_size - 42，超过时要拆分
     * 每轮输出大小都是rsa_size
    */
    int limit_size = rsa_size - 42;
    while (!text.empty()) {
        int split_size = std::min(static_cast<int>(text.size()), limit_size); 
        int to_size = RSA_public_encrypt(split_size, (unsigned char*)text.c_str(), 
                (unsigned char*)to, rsa, RSA_PKCS1_OAEP_PADDING); 
        SSL_iAssert_LT0(to_size, "RSA_public_encrypt");

        // 必须传入to_size，防止数据缺失
        to_text.append(std::string(to, to_size));
        if (text.size() <= limit_size) {
            break;
        }
        text = text.substr(limit_size);
    }

    free(to);
    RSA_free(rsa);
    fclose(fp);

    return 0;
}

int SSLTools::RSADecrypt(std::string& to_text, std::string text, std::string key_path, RSAOP op) {
    to_text.clear();

    FILE *fp = fopen(key_path.c_str(), "r");
    if (fp == NULL) {
        TLOG((ERR, "fopen error:%s, key_path:%s", strerror(errno), key_path.c_str()));
        return -1;
    }

    RSA *rsa = PEM_read_RSAPrivateKey(fp, NULL, NULL, NULL);
    SSL_iAssert_NULL(rsa, "PEM_read_RSAPrivateKey");

    // RSA_print_fp(stdout, rsa, 0);

    int rsa_size = RSA_size(rsa);
    int limit_size = rsa_size - 42;
    char *to = (char*)malloc(limit_size);
    memset(to, 0, limit_size);

    /* 
     * 数据拆分解密 
     *
     * RSA_PKCS1_OAEP_PADDING模式下，解密时：
     * 每轮输入数据大小都是rsa_size，超过时要拆分
     * 每轮输出大小都不超过rsa_size - 42
    */
    int left_size = text.size();
    while (!text.empty()) {
        int to_size = RSA_private_decrypt(rsa_size, (unsigned char*)text.c_str(), (unsigned char*)to, rsa, RSA_PKCS1_OAEP_PADDING);
        SSL_iAssert_LT0(to_size, "RSA_private_decrypt");

        // 必须传入to_size，防止数据缺失
        to_text.append(std::string(to, to_size));
        text = text.substr(rsa_size);
    }

    free(to);
    RSA_free(rsa);
    fclose(fp);

    return 0;
}

int SSLTools::MD5Encrypt(std::string& to_text, std::string text) {
    MD5_CTX ctx;
    char md5_result[BUFSIZ];
    int ret = MD5_Init(&ctx);
    SSL_iAssert_LT0(ret, "MD5_Init");

    ret = MD5_Update(&ctx, text.c_str(), text.size());
    SSL_iAssert_LT0(ret, "MD5_Update");

    ret = MD5_Final((unsigned char*)md5_result, &ctx);
    SSL_iAssert_LT0(ret, "MD5_Final");

    to_text = std::string(md5_result);
    return 0;
}

int SSLTools::TLS_Init() {
    // 初始化
    OpenSSL_add_ssl_algorithms();
    SSL_load_error_strings();
    // 客户端协议(SSLv2/SSLv3/TLSv1)
    const SSL_METHOD* meth = TLSv1_client_method();
    // 申请SSL会话环境
    tls_.ctx = SSL_CTX_new(meth);
    SSL_iAssert_NULL(tls_.ctx, "SSL_CTX_new");

    return 0;
}

int SSLTools::TLS_LoadCertificate() {
    // 加载自己的证书
    int ret = SSL_CTX_use_certificate_file(tls_.ctx, CERTF, SSL_FILETYPE_PEM);
    SSL_iAssert_NE1(ret, "SSL_CTX_use_certificate_file");

    // 加载自己的私钥,以用于签名
    ret = SSL_CTX_use_PrivateKey_file(tls_.ctx, KEYF, SSL_FILETYPE_PEM);
    SSL_iAssert_NE1(ret, "SSL_CTX_use_PrivateKey_file");

    // 调用了以上两个函数后,检验一下自己的证书与私钥是否配对
    SSL_CTX_check_private_key(tls_.ctx);
    SSL_iAssert_NE1(ret, "SSL_CTX_check_private_key");

    return 0;
}
