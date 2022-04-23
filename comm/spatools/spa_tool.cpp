#include "spa_tool.h"
#include "comm/tlog/tlog.h"
#include "comm/ssltools/ssl_tool.h"

int SPATools::EncryptVoucher(spa::SPAVoucherPacket& spaVoucherPacket, const spa::SPAVoucher& spaVoucher, const std::string& key) 
{
    std::string text;
    spaVoucher.SerializeToString(&text);

    // rsa
    std::string rsa_en_text;
    int ret = SSLTools().RSAEncrypt(rsa_en_text, text, key.c_str());
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
    
    int ret = SSLTools().RSADecrypt(rsa_de_text, rsa_en_text, key.c_str());
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

int SPATools::SignTicket(spa::SPATicketPacket& spaTicketPacket, const spa::SPATicket& spaTicket, const std::string& key)
{
    int ret = 0;
    std::string text;
    std::string sign_text;
    spaTicket.SerializeToString(&text);

    ret = SSLTools().RSASign(sign_text, text, key);
    iAssert(ret, ("RSASign faild"));

    spaTicketPacket.mutable_ticket()->CopyFrom(spaTicket);
    spaTicketPacket.set_sign_data(sign_text);

    return 0;
}

int SPATools::VerifyTicket(spa::SPATicketPacket& spaTicketPacket, const std::string& key)
{
    int ret = 0;
    std::string text;
    std::string sign_text;

    spaTicketPacket.mutable_ticket()->SerializeToString(&text);
    sign_text = spaTicketPacket.sign_data();

    ret = SSLTools().RSAVerify(sign_text, text, key);
    iAssert(ret, ("RSAVerify faild"));

    TLOG_MSG(("VerifyTicket: sign matched"));
    return 0;
}