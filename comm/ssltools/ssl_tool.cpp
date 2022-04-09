#include "ssl_tool.h"

int SSLTools::RSAEncrypt(std::string& to_text, std::string text, std::string key_path, RSAOP op) 
{ 
    to_text.clear();

    FILE *fp = fopen(key_path.c_str(), "r");
    if (fp == NULL) 
    {
        TLOG_ERR(("fopen error:%s, key_path:%s", strerror(errno), key_path.c_str()));
        return -1;
    }

    // -----BEGIN RSA PUBLIC KEY-----
    // RSA *rsa = PEM_read_RSAPublicKey(fp, NULL, NULL, NULL);
    
    // -----BEGIN PUBLIC KEY-----
    RSA *rsa = PEM_read_RSA_PUBKEY(fp, NULL, NULL, NULL);
    SSL_iAssert_NULL(rsa, ("PEM_read_RSAPublicKey"));

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
    while (!text.empty()) 
    {
        int split_size = std::min(static_cast<int>(text.size()), limit_size); 
        int to_size = RSA_public_encrypt(split_size, (unsigned char*)text.c_str(), 
                (unsigned char*)to, rsa, RSA_PKCS1_OAEP_PADDING); 
        SSL_iAssert_LT0(to_size, ("RSA_public_encrypt"));

        // 必须传入to_size，防止数据缺失
        to_text.append(std::string(to, to_size));
        if (text.size() <= limit_size) 
        {
            break;
        }
        text = text.substr(limit_size);
    }

    free(to);
    RSA_free(rsa);
    fclose(fp);

    return 0;
}

int SSLTools::RSADecrypt(std::string& to_text, std::string text, std::string key_path, RSAOP op) 
{
    to_text.clear();

    FILE *fp = fopen(key_path.c_str(), "r");
    if (fp == NULL) 
    {
        TLOG_ERR(("fopen error:%s, key_path:%s", strerror(errno), key_path.c_str()));
        return -1;
    }

    RSA *rsa = PEM_read_RSAPrivateKey(fp, NULL, NULL, NULL);
    SSL_iAssert_NULL(rsa, ("PEM_read_RSAPrivateKey"));

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
    while (!text.empty()) 
    {
        int to_size = RSA_private_decrypt(rsa_size, (unsigned char*)text.c_str(), (unsigned char*)to, rsa, RSA_PKCS1_OAEP_PADDING);
        SSL_iAssert_LT0(to_size, ("RSA_private_decrypt"));

        // 必须传入to_size，防止数据缺失
        to_text.append(std::string(to, to_size));
        text = text.substr(rsa_size);
    }

    free(to);
    RSA_free(rsa);
    fclose(fp);

    return 0;
}

int SSLTools::MD5Encrypt(std::string& to_text, std::string text) 
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

int SSLConnector::_SSL_Init() 
{
    // 初始化
    OpenSSL_add_ssl_algorithms();
    SSL_load_error_strings();

    // 客户端协议(SSLv2/SSLv3/TLSv1)
    const SSL_METHOD* meth = TLSv1_client_method();

    // 申请SSL会话环境变量
    ssl_data_.ctx = SSL_CTX_new(meth);
    SSL_iAssert_NULL(ssl_data_.ctx, ("SSL_CTX_new"));

    return 0;
}

int SSLConnector::_SSL_LoadCertificate(std::string cert, std::string pri_key) 
{
    // 加载自己的证书
    int ret = SSL_CTX_use_certificate_file(ssl_data_.ctx, cert.c_str(), SSL_FILETYPE_PEM);
    SSL_iAssert_NE1(ret, ("SSL_CTX_use_certificate_file"));

    // 加载自己的私钥,以用于签名
    ret = SSL_CTX_use_PrivateKey_file(ssl_data_.ctx, pri_key.c_str(), SSL_FILETYPE_PEM);
    SSL_iAssert_NE1(ret, ("SSL_CTX_use_PrivateKey_file"));

    // 调用了以上两个函数后,检验一下自己的证书与私钥是否配对
    SSL_CTX_check_private_key(ssl_data_.ctx);
    SSL_iAssert_NE1(ret, ("SSL_CTX_check_private_key"));

    return 0;
}

int SSLConnector::_SSL_BindSocket(int fd) 
{
    int ret = SSL_set_fd(ssl_data_.ssl, fd);
    SSL_iAssert_NE1(ret, ("SSL_set_fd fd:%d", fd));

    ret = SSL_connect(ssl_data_.ssl);
    SSL_iAssert_NE1(ret, ("SSL_connect fd:%d", fd));

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

int SSLConnector::SSLInit() 
{
    int ret = 0;
    ret = _SSL_Init();
    iAssert(ret, ("_SSL_Init"));

    return 0;
}

int SSLConnector::SSLConnectToServer(std::string ip, int port) 
{
    int ret = 0, fd;
    struct sockaddr_in addr;

    fd = socket(AF_INET, SOCK_STREAM, 0); 
    iAssert(fd, ("socket"));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip.c_str());
    addr.sin_port = htons(port);

    ret = connect(fd, (struct sockaddr*)&addr, sizeof(addr));
    iAssert(ret, ("connect ip:%s port:%d", ip.c_str(), port));

    ret = _SSL_BindSocket(fd);
    iAssert(ret, ("_SSL_BindSocket fd:%d", fd));

    return 0;
}

int SSLConnector::SSLBindClientSocket(int fd) 
{
    int ret = _SSL_BindSocket(fd);
    iAssert(ret, ("_SSL_BindSocket fd:%d", fd));

    return 0;
}

int SSLConnector::SSLSend(const std::string& msg) 
{
    int ret = 0;
    ret = SSL_write(ssl_data_.ssl, msg.c_str(), sizeof(msg)); 
    iAssert(ret, ("SSL_write %s", msg.c_str()));

    return ret;
}

int SSLConnector::SSLRecv(std::string& msg) 
{
    int ret = 0;
    char buf[BUFSIZ];

    while (ret > 0) {
        ret = SSL_read(ssl_data_.ssl, buf, sizeof(buf));
        msg.append(buf);
    }

    return msg.size();
}
