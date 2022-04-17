#include "spa_tool.h"
#include "comm/tlog/tlog.h"
#include "comm/ssltools/ssl_tool.h"

int SPATools::EncryptVoucher(spa::SPAPacket& packet, const spa::SPAVoucher& voucher) 
{
    std::string text;
    voucher.SerializeToString(&text);
    TLOG_DBG(("voucher.SerializeToString: %s", text.c_str()));

    // encrypt text
    std::string rsa_en_text;
    int ret = SSLTools().RSAEncrypt(rsa_en_text, text, RSA_PUB_KEY_PATH, PUB_ENCRYPT);
    if (ret < 0) 
    {
        TLOG_ERR(("RSAEncrypt"));
        return -1;
    }

    // md5 for text
    std::string md5_en_text;
    ret = SSLTools().MD5Encrypt(md5_en_text, text);
    if (ret < 0) 
    {
        TLOG_ERR(("MD5Encrypt"));
        return -1;
    }

    packet.set_encrypt_spa_voucher(rsa_en_text + SeparatorForStr + md5_en_text);
    // 中间有\0，可能显示不完全
    TLOG_DBG(("encrypt_spa_voucher: %s", packet.encrypt_spa_voucher().c_str()));

    return 0;
}

int SPATools::DecryptVoucher(spa::SPAVoucher& voucher, const spa::SPAPacket& packet) 
{
    std::string src = packet.encrypt_spa_voucher();
    std::string rsa_en_text;
    std::string rsa_de_text;
    std::string md5_en_text;
    std::string md5_en_text_new;

    int pos = src.find(SeparatorForStr);
    if (pos == src.size()) 
    {
        TLOG_ERR(("SeparatorForStr not found"));
        voucher.set_is_valid(false);
        return -1;
    }

    rsa_en_text = src.substr(0, pos);
    md5_en_text = src.substr(std::string(SeparatorForStr).size() + pos);
    
    int ret = SSLTools().RSADecrypt(rsa_de_text, rsa_en_text, RSA_PRI_KEY_PATH, PUB_ENCRYPT);
    if (ret < 0) 
    {
        TLOG_ERR(("RSADecrypt"));
        return -1;
    }

    // 验证md5
    ret = SSLTools().MD5Encrypt(md5_en_text_new, rsa_de_text);
    if (ret < 0) 
    {
        TLOG_ERR(("MD5Encrypt"));
        return -1;
    }
    if (md5_en_text_new != md5_en_text) 
    {
        TLOG_ERR(("MD5 not match:\nold:%s\nnew:%s", md5_en_text.c_str(), md5_en_text_new.c_str()));
        return -1;
    }
    TLOG_DBG(("RSADecrypt success, MD5 match, str:%s", rsa_de_text.c_str()));

    // 解包
    ret = voucher.ParseFromString(rsa_de_text);
    if (ret == false) 
    {
        TLOG_ERR(("voucher.ParseFromString"));
    }

    return 0;
}

