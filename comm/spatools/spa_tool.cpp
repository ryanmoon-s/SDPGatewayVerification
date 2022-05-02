#include "spa_tool.h"
#include "comm/tlog/tlog.h"
#include "comm/ssltools/ssl_tool.h"

int SPATools::EncryptVoucher(spa::SPAVoucherPacket& spaVoucherPacket, const spa::SPAVoucher& spaVoucher, const std::string& key) 
{
    std::string text;
    spaVoucher.SerializeToString(&text);

    // rsa
    std::string rsa_en_text;
    int ret = SSLTools().RSAEncrypt(rsa_en_text, text, key);
    iAssert(ret, ("RSAEncrypt faild"));

    // md5
    std::string md5_en_text;
    ret = SSLTools().MD5Encrypt(md5_en_text, text);
    iAssert(ret, ("MD5Encrypt faild"));

    // 拼接
    spaVoucherPacket.set_enc_data(rsa_en_text);
    spaVoucherPacket.set_md5_data(md5_en_text);
    TLOG_DBG(("EncryptVoucher success, size:%d", spaVoucherPacket.enc_data().size()));

    return 0;
}

int SPATools::DecryptVoucher(spa::SPAVoucher& spaVoucher, const spa::SPAVoucherPacket& spaVoucherPacket, const std::string& key) 
{
    std::string rsa_en_text;
    std::string rsa_de_text;
    std::string md5_en_text;
    std::string md5_en_text_new;

    rsa_en_text = spaVoucherPacket.enc_data();
    md5_en_text = spaVoucherPacket.md5_data();
    
    int ret = SSLTools().RSADecrypt(rsa_de_text, rsa_en_text, key);
    iAssert(ret, ("RSADecrypt"));

    // 验证md5，防篡改。
    ret = SSLTools().MD5Encrypt(md5_en_text_new, rsa_de_text);
    iAssert(ret, ("MD5Encrypt"));

    if (md5_en_text_new != md5_en_text) 
    {
        TLOG_ERR(("MD5 not match:\nold:%s\nnew:%s", md5_en_text.c_str(), md5_en_text_new.c_str()));
        return -1;
    }
    TLOG_DBG(("DecryptVoucher success; MD5 matched"));

    // 解包
    spaVoucher.ParseFromString(rsa_de_text);

    return 0;
}

int SPATools::EncryptTicket(spa::SPATicketPacket& spaTicketPacket, const spa::SPATicket& spaTicket, const std::string& sign_pri_key, const std::string& enc_pub_key)
{
    int ret = 0;
    std::string text;
    std::string rsa_sign_text;
    std::string rsa_en_text;
    std::string md5_en_text;
    spaTicket.SerializeToString(&text);
    
    // 加密
    ret = SSLTools().RSAEncrypt(rsa_en_text, text, enc_pub_key);
    iAssert(ret, ("RSAEncrypt faild"));

    // 摘要
    ret = SSLTools().MD5Encrypt(md5_en_text, text);
    iAssert(ret, ("MD5Encrypt faild"));

    // 签名
    ret = SSLTools().RSASign(rsa_sign_text, text, sign_pri_key);
    iAssert(ret, ("RSASign faild"));

    spaTicketPacket.set_enc_data(rsa_en_text);
    spaTicketPacket.set_md5_data(md5_en_text);
    spaTicketPacket.set_sign_data(rsa_sign_text);

    return 0;
}

int SPATools::DecryptTicket(spa::SPATicket& spaTicket, const spa::SPATicketPacket& spaTicketPacket, const std::string& sign_pub_key, const std::string& enc_pri_key)
{
    int ret = 0;
    std::string sign_text;
    std::string rsa_en_text;
    std::string rsa_de_text;
    std::string md5_en_text;
    std::string md5_en_text_new;

    rsa_en_text = spaTicketPacket.enc_data();
    md5_en_text = spaTicketPacket.md5_data();
    sign_text = spaTicketPacket.sign_data();

    // 解密
    ret = SSLTools().RSADecrypt(rsa_de_text, rsa_en_text, enc_pri_key);
    iAssert(ret, ("RSADecrypt"));

    // 验证md5，防篡改。
    ret = SSLTools().MD5Encrypt(md5_en_text_new, rsa_de_text);
    iAssert(ret, ("MD5Encrypt"));

    if (md5_en_text_new != md5_en_text) 
    {
        TLOG_ERR(("MD5 not match:\nold:%s\nnew:%s", md5_en_text.c_str(), md5_en_text_new.c_str()));
        return -1;
    }
    TLOG_DBG(("DecryptVoucher success; MD5 matched"));

    // 验证签名
    ret = SSLTools().RSAVerify(sign_text, rsa_de_text, sign_pub_key);
    iAssert(ret, ("RSAVerify faild"));

    spaTicket.ParseFromString(rsa_de_text);
    TLOG_MSG(("DecryptTicket: sign matched"));
    return 0;
}
