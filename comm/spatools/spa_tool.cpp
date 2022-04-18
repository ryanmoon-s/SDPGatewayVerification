#include "spa_tool.h"
#include "comm/tlog/tlog.h"
#include "comm/ssltools/ssl_tool.h"

int SPATools::EncryptVoucher(spa::SPAPacket& packet, const spa::SPAVoucher& voucher) 
{
    std::string text;
    voucher.SerializeToString(&text);

    // rsa
    std::string rsa_en_text;
    int ret = SSLTools().RSAEncrypt(rsa_en_text, text, RSA_PUB_KEY_PATH, PUB_ENCRYPT);
    iAssert(ret, ("RSAEncrypt faild"));

    // md5
    std::string md5_en_text;
    ret = SSLTools().MD5Encrypt(md5_en_text, text);
    iAssert(ret, ("MD5Encrypt faild"));

    // 拼接
    packet.set_rsa_data(rsa_en_text);
    packet.set_md5_data(md5_en_text);
    TLOG_DBG(("EncryptVoucher success, size:%d", packet.rsa_data().size()));

    return 0;
}

int SPATools::DecryptVoucher(spa::SPAVoucher& voucher, const spa::SPAPacket& packet) 
{
    std::string rsa_en_text;
    std::string rsa_de_text;
    std::string md5_en_text;
    std::string md5_en_text_new;

    rsa_en_text = packet.rsa_data();
    md5_en_text = packet.md5_data();
    
    int ret = SSLTools().RSADecrypt(rsa_de_text, rsa_en_text, RSA_PRI_KEY_PATH, PUB_ENCRYPT);
    iAssert(ret, ("RSADecrypt"));

    // 验证md5
    ret = SSLTools().MD5Encrypt(md5_en_text_new, rsa_de_text);
    iAssert(ret, ("MD5Encrypt"));

    if (md5_en_text_new != md5_en_text) 
    {
        TLOG_ERR(("MD5 not match:\nold:%s\nnew:%s", md5_en_text.c_str(), md5_en_text_new.c_str()));
        return -1;
    }
    TLOG_DBG(("DecryptVoucher success; MD5 matched"));

    // 解包
    voucher.ParseFromString(rsa_de_text);

    return 0;
}

