#include "ssl_tool.h"
#include <functional>

int SSLTools::RSAEncrypt(std::string& to_text, const std::string& text, const std::string& key) 
{ 
    to_text.clear();
    std::string src = text;
    RSA *rsa = NULL;
    FILE *fp = NULL;

    fp = fopen(key.c_str(), "r");
    if (fp == NULL) 
    {
        TLOG_ERR(("fopen error:%s, key:%s", strerror(errno), key.c_str()));
        return -1;
    }

    // -----BEGIN RSA PUBLIC KEY----- 格式
    // rsa = PEM_read_RSAPublicKey(fp, NULL, NULL, NULL);
    
    // -----BEGIN PUBLIC KEY----- 格式
    rsa = PEM_read_RSA_PUBKEY(fp, NULL, NULL, NULL);
    SSL_iAssert_NULL(rsa, ("RSA read key faild"));

    // 打印密钥
    // RSA_print_fp(stdout, rsa, 0);

    int rsa_size = RSA_size(rsa);
    char *to = (char*)malloc(rsa_size);
    memset(to, 0, rsa_size);

    /* 
     * 数据拆分加密 
     *
     * RSA_PKCS1_OAEP_PADDING 模式下，加密时：
     * 每轮输入数据大小不能超过rsa_size - 42，超过时要拆分
     * 每轮输出大小都是rsa_size
     * 
    */
    int limit_size = rsa_size - 42;
    while (!src.empty())
    {
        int split_size = std::min(static_cast<int>(src.size()), limit_size); 
        int to_size = RSA_public_encrypt(split_size, (unsigned char*)src.c_str(), 
                (unsigned char*)to, rsa, RSA_PKCS1_PADDING); 
        SSL_iAssert_LT0(to_size, ("RSA_public_encrypt"));

        // 必须传入to_size，防止数据缺失
        to_text.append(std::string(to, to_size));
        if (src.size() <= limit_size) 
        {
            break;
        }
        src = src.substr(limit_size);
    }

    free(to);
    RSA_free(rsa);
    fclose(fp);

    return 0;
}

int SSLTools::RSADecrypt(std::string& to_text, const std::string& text, const std::string& key) 
{
    to_text.clear();
    std::string src = text;
    RSA *rsa = NULL;
    FILE *fp = NULL;

    fp = fopen(key.c_str(), "r");
    if (fp == NULL) 
    {
        TLOG_ERR(("fopen error:%s, key:%s", strerror(errno), key.c_str()));
        return -1;
    }

    rsa = PEM_read_RSAPrivateKey(fp, NULL, NULL, NULL);
    SSL_iAssert_NULL(rsa, ("RSA read key faild"));

    // 打印密钥
    // RSA_print_fp(stdout, rsa, 0);

    int rsa_size = RSA_size(rsa);
    int limit_size = rsa_size - 42;
    char *to = (char*)malloc(limit_size);
    memset(to, 0, limit_size);

    /* 
     * 数据拆分解密 
     *
     * RSA_PKCS1_OAEP_PADDING 模式下，解密时：
     * 每轮输入数据大小都是rsa_size，超过时要拆分
     * 每轮输出大小都不超过rsa_size - 42
     * 
    */
    int left_size = src.size();
    while (!src.empty()) 
    {
        int to_size = RSA_private_decrypt(rsa_size, (unsigned char*)src.c_str(), (unsigned char*)to, rsa, RSA_PKCS1_PADDING);
        SSL_iAssert_LT0(to_size, ("RSA_private_decrypt"));

        // 必须传入to_size，防止数据缺失
        to_text.append(std::string(to, to_size));
        src = src.substr(rsa_size);
    }

    free(to);
    RSA_free(rsa);
    fclose(fp);

    return 0;
}

int SSLTools::RSASign(std::string& sig_text, const std::string& text, const std::string& key)
{
    sig_text.clear();
    std::string src = text;
    RSA *rsa = NULL;
    FILE *fp = NULL;

    fp = fopen(key.c_str(), "r");
    if (fp == NULL) 
    {
        TLOG_ERR(("fopen error:%s, key:%s", strerror(errno), key.c_str()));
        return -1;
    }

    // -----BEGIN RSA PUBLIC KEY----- 格式
    // rsa = PEM_read_RSAPublicKey(fp, NULL, NULL, NULL);
    
    // -----BEGIN PUBLIC KEY----- 格式
    rsa = PEM_read_RSAPrivateKey(fp, NULL, NULL, NULL);
    SSL_iAssert_NULL(rsa, ("RSA read key faild"));

    // 打印密钥
    // RSA_print_fp(stdout, rsa, 0);

    int rsa_size = RSA_size(rsa);
    char *to = (char*)malloc(rsa_size);
    memset(to, 0, rsa_size);

    /* 
     * 数据拆分签名 
     *
     * 签名仅支持 PKCS #1 v2.0; RSA_PKCS1_OAEP_PADDING; RSA_size(rsa) - 42
     *
     * RSA_PKCS1_OAEP_PADDING 模式下，签名时：
     * 每轮输入数据大小不能超过rsa_size - 42，超过时要拆分
     * 每轮输出签名大小都是rsa_size
     * 
    */
    int limit_size = rsa_size - 42;
    while (!src.empty())
    {
        int to_size = 0;
        int split_size = std::min(static_cast<int>(src.size()), limit_size); 
        RSA_sign(NID_sha1, (unsigned char*)src.c_str(), split_size, 
            (unsigned char*)to, (unsigned int*)&to_size, rsa);
        SSL_iAssert_LT0(to_size, ("RSA_sign"));

        // 必须传入to_size，防止数据缺失
        sig_text.append(std::string(to, to_size));
        if (src.size() <= limit_size) 
        {
            break;
        }
        src = src.substr(limit_size);
    }

    free(to);
    RSA_free(rsa);
    fclose(fp);

    return 0;
}

int SSLTools::RSAVerify(const std::string& sig_text, const std::string& src_text, const std::string& key)
{
    std::string src = src_text;
    std::string sig = sig_text;
    RSA *rsa = NULL;
    FILE *fp = NULL;

    fp = fopen(key.c_str(), "r");
    if (fp == NULL) 
    {
        TLOG_ERR(("fopen error:%s, key:%s", strerror(errno), key.c_str()));
        return -1;
    }

    rsa = PEM_read_RSA_PUBKEY(fp, NULL, NULL, NULL);
    SSL_iAssert_NULL(rsa, ("RSA read key faild"));

    // 打印密钥
    // RSA_print_fp(stdout, rsa, 0);

    int rsa_size = RSA_size(rsa);
    int limit_size = rsa_size - 42;

    /* 
     * 数据拆分验证
     * 
     * 签名仅支持 PKCS #1 v2.0; RSA_PKCS1_OAEP_PADDING; RSA_size(rsa) - 42
     *
     * RSA_PKCS1_OAEP_PADDING 模式下，验证时：
     * 每轮签名数据大小都是 rsa_size
     * 每轮原数据大小都不超过 rsa_size - 42
     * 
    */
    int success = 1;
    while (!src.empty()) 
    {
        int split_size = std::min(static_cast<int>(src.size()), limit_size);
        int ret = RSA_verify(NID_sha1, (unsigned char*)src.c_str(), split_size, (unsigned char*)sig.c_str(), rsa_size, rsa);
        TLOG_DBG(("ret:%d", ret));

        if (ret == 0)
        {
            TLOG_ERR(("RSA Verify faild"));
            success = 0;
            break;
        }

        if (sig.size() == rsa_size)
        {
            break;
        }

        src = src.substr(limit_size);
        sig = sig.substr(rsa_size);
    }

    RSA_free(rsa);
    fclose(fp);

    return success ? 0 : -1;
}

int SSLTools::MD5Encrypt(std::string& to_text, const std::string& text) 
{
    MD5_CTX ctx;
    char md5_result[BUFSIZ];
    int ret = MD5_Init(&ctx);
    SSL_iAssert_LT0(ret, ("MD5_Init"));

    ret = MD5_Update(&ctx, text.c_str(), text.size());
    SSL_iAssert_LT0(ret, ("MD5_Update"));

    ret = MD5_Final((unsigned char*)md5_result, &ctx);
    SSL_iAssert_LT0(ret, ("MD5_Final"));

    to_text = std::string(md5_result, ret);
    return 0;
}

SSLConnector::SSLConnector(const std::string& cert, const std::string& pri_key, int is_server): is_server_(is_server)
{
    int ret = _SSL_Init();
    if (ret < 0)
    {
        TLOG_ERR(("_SSL_Init"));
    }

    // 装载证书
    ret = _SSL_LoadCertificate(cert, SSL_CRT_CA, pri_key);
    if (ret < 0)
    {
        TLOG_ERR(("_SSL_LoadCertificate"));
    }
}

SSLConnector::~SSLConnector(){
    SSL_shutdown(ssl_data_.ssl);
    SSL_free(ssl_data_.ssl);
    SSL_CTX_free(ssl_data_.ctx);
}

int SSLConnector::_SSL_Init() 
{
    // 初始化ssl库
    OpenSSL_add_ssl_algorithms();
    SSL_load_error_strings();

    // 初始化上下文ctx
    // 客户端协议(SSLv2/SSLv3/TLSv1)
    if (is_server_)
    {
        const SSL_METHOD* method = TLSv1_server_method();
        ssl_data_.ctx = SSL_CTX_new(method);
    }
    else
    {
        const SSL_METHOD* method = TLSv1_client_method();
        ssl_data_.ctx = SSL_CTX_new(method);
    }
    SSL_iAssert_NULL(ssl_data_.ctx, ("SSL_CTX_new"));

    return 0;
}

int SSLConnector::_SSL_LoadCertificate(std::string cert, std::string ca_cert, std::string pri_key) 
{
    int ret = 0;

    #if 1
    // 是否要验证对方，双向验证
    SSL_CTX_set_verify(ssl_data_.ctx, SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT, NULL);   
    // 若验证，则放置CA证书
    SSL_CTX_load_verify_locations(ssl_data_.ctx, ca_cert.c_str(), NULL); 
    #else
    // 不验证客户端证书
    SSL_CTX_set_verify(ssl_data_.ctx, SSL_VERIFY_NONE, NULL); 
    #endif

    // 加载自己的证书
    ret = SSL_CTX_use_certificate_file(ssl_data_.ctx, cert.c_str(), SSL_FILETYPE_PEM);
    SSL_iAssert_NE1(ret, ("SSL_CTX_use_certificate_file"));

    // 加载自己的私钥，以用于签名
    ret = SSL_CTX_use_PrivateKey_file(ssl_data_.ctx, pri_key.c_str(), SSL_FILETYPE_PEM);
    SSL_iAssert_NE1(ret, ("SSL_CTX_use_PrivateKey_file"));

    // 调用了以上两个函数后，检验一下自己的证书与私钥是否配对
    ret = SSL_CTX_check_private_key(ssl_data_.ctx);
    SSL_iAssert_NE1(ret, ("SSL_CTX_check_private_key"));

    return 0;
}

int SSLConnector::_SSL_DumpCertInfo() 
{
    X509 *cert = SSL_get_peer_certificate(ssl_data_.ssl);      
    SSL_iAssert_NULL(cert, ("SSL_get_peer_certificate"));
    TLOG_DBG(("Server certificate:/n"));

    char *str = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
    SSL_iAssert_NULL(str, ("X509_NAME_oneline"));
    TLOG_DBG(("/t subject: %s/n", str));
    free (str);

    str = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
    SSL_iAssert_NULL(str, ("X509_NAME_oneline"));
    TLOG_DBG(("/t issuer: %s/n", str));
    free(str);

    X509_free(cert);
    return 0;
}

int SSLConnector::SSLConnect(int fd) 
{
    // 申请SSL套接字
    ssl_data_.ssl = SSL_new(ssl_data_.ctx);
    SSL_iAssert_NULL(ssl_data_.ssl, ("SSL_new"));

    int ret = SSL_set_fd(ssl_data_.ssl, fd);
    SSL_iAssert_NE1(ret, ("SSL_set_fd fd:%d", fd));

    ret = SSL_connect(ssl_data_.ssl);
    SSL_iAssert_NE1(ret, ("SSL_connect fd:%d", fd));

    return 0;
}

int SSLConnector::SSLAccept(int fd) 
{
    ssl_data_.ssl = SSL_new(ssl_data_.ctx);
    SSL_iAssert_NULL(ssl_data_.ssl, ("SSL_new"));

    int ret = SSL_set_fd(ssl_data_.ssl, fd);
    SSL_iAssert_NE1(ret, ("SSL_set_fd fd:%d", fd));

    ret = SSL_accept(ssl_data_.ssl);
    SSL_iAssert_NE1(ret, ("SSL_accept fd:%d", fd));
    
    return 0;
}

int SSLConnector::SSLWrite(const char* buf, int size) 
{
    return SSL_write(ssl_data_.ssl, buf, size); 
}

int SSLConnector::SSLRead(char* buf, int size) 
{
    return SSL_read(ssl_data_.ssl, buf, size);
}
