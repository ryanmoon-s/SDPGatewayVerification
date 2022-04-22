#pragma once

#include "comm/commdef/comm_def.h"
#include "comm/proto/spa.pb.h" 
#include "comm/tlog/tlog.h"
#include "comm/ssltools/ssl_tool.h"

#include <string>

class SPATools {
public:
    // SPA凭证/票据 -> SPA数据包
    template<typename Data, typename DataPacket>
    int EncryptVoucher(DataPacket& dataPacket, const Data& data);
    
    // SPA凭证/票据 <- SPA数据包
    template<typename Data, typename DataPacket>
    int DecryptVoucher(Data& data, const DataPacket& dataPacket);
};


template<typename Data, typename DataPacket>
int SPATools::EncryptVoucher(DataPacket& dataPacket, const Data& data) 
{
    std::string text;
    data.SerializeToString(&text);

    // rsa
    std::string rsa_en_text;
    int ret = SSLTools().RSAEncrypt(rsa_en_text, text, RSA_PUB_KEY_CONTROLLER, PUB_ENCRYPT);
    iAssert(ret, ("RSAEncrypt faild"));

    // md5
    std::string md5_en_text;
    ret = SSLTools().MD5Encrypt(md5_en_text, text);
    iAssert(ret, ("MD5Encrypt faild"));

    // 拼接
    dataPacket.set_enc_data(rsa_en_text);
    dataPacket.set_md5_data(md5_en_text);
    TLOG_DBG(("EncryptVoucher success, size:%d", dataPacket.enc_data().size()));

    return 0;
}

template<typename Data, typename DataPacket>
int SPATools::DecryptVoucher(Data& data, const DataPacket& dataPacket) 
{
    std::string rsa_en_text;
    std::string rsa_de_text;
    std::string md5_en_text;
    std::string md5_en_text_new;

    rsa_en_text = dataPacket.enc_data();
    md5_en_text = dataPacket.md5_data();
    
    int ret = SSLTools().RSADecrypt(rsa_de_text, rsa_en_text, RSA_PRI_KEY_CONTROLLER, PUB_ENCRYPT);
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
    data.ParseFromString(rsa_de_text);

    return 0;
}

