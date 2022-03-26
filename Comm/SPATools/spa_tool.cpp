#include "spa_tool.h"
#include "Comm/TLog/tlog.h"
#include "Comm/SSLTools/ssl_tool.h"

static std::string meta_data_str[] = {"account:", "password:", "ip:", "address:", "timestamp:", "isvalid:"};

/*
 * account,password,ip,address,timestamp,is_valid(0,1),
 */
int SPATools::EncryptVoucher(SPAPacket& packet, const SPAVoucher& voucher) 
{
    std::string text;
    text.append(meta_data_str[0] + voucher.u_acc.user_account + ",");
    text.append(meta_data_str[1] + voucher.u_acc.user_password + ",");
    text.append(meta_data_str[2] + voucher.u_ip + ",");
    text.append(meta_data_str[3] + voucher.u_address + ",");
    text.append(meta_data_str[4] + std::to_string(voucher.u_timestamp) + ",");
    text.append(meta_data_str[5] + (voucher.is_valid ? "1" : "0") + ",");
    TLOG((DEBUG, "voucher to str: %s", text.c_str()));

    // encrypt text
    std::string rsa_en_text;
    int ret = SSLTools().RSAEncrypt(rsa_en_text, text, RSA_PUB_KEY_PATH, PUB_ENCRYPT);
    if (ret < 0) 
    {
        TLOG((ERR, "RSAEncrypt"));
        return -1;
    }

    // md5 for text
    std::string md5_en_text;
    ret = SSLTools().MD5Encrypt(md5_en_text, text);
    if (ret < 0) 
    {
        TLOG((ERR, "MD5Encrypt"));
        return -1;
    }

    packet.encrypt_spa_voucher = rsa_en_text + SeparatorForStr + md5_en_text;
    // 中间有\0，可能显示不完全
    TLOG((DEBUG, "encrypt_spa_voucher: %s", packet.encrypt_spa_voucher.c_str()));

    return 0;
}

int SPATools::DecryptVoucher(SPAVoucher& voucher, const SPAPacket& packet) 
{
    std::string src = packet.encrypt_spa_voucher;
    std::string rsa_en_text;
    std::string rsa_de_text;
    std::string md5_en_text;
    std::string md5_en_text_new;

    int pos = src.find(SeparatorForStr);
    if (pos == src.size()) 
    {
        TLOG((ERR, "SeparatorForStr not found"));
        voucher.is_valid = false;
        return -1;
    }

    rsa_en_text = src.substr(0, pos);
    md5_en_text = src.substr(std::string(SeparatorForStr).size() + pos);
    
    int ret = SSLTools().RSADecrypt(rsa_de_text, rsa_en_text, RSA_PRI_KEY_PATH, PUB_ENCRYPT);
    if (ret < 0) 
    {
        TLOG((ERR, "RSADecrypt"));
        return -1;
    }

    // 验证md5
    ret = SSLTools().MD5Encrypt(md5_en_text_new, rsa_de_text);
    if (ret < 0) 
    {
        TLOG((ERR, "MD5Encrypt"));
        return -1;
    }
    if (md5_en_text_new != md5_en_text) 
    {
        TLOG((ERR, "MD5 not match"));
        return -1;
    }
    TLOG((DEBUG, "RSADecrypt success and MD5 match, str:%s", rsa_de_text.c_str()));

    // 解包
    ret = _ParseVoucherFromStr(voucher, rsa_de_text);
    if (ret < 0) 
    {
        TLOG((ERR, "ParseVoucherFromStr"));
    }

    return 0;
}

int SPATools::_ParseVoucherFromStr(SPAVoucher& voucher, const std::string& str) 
{
    TLOG((DEBUG, "str: %s", str.c_str()));
    std::string value;

    int ret = _GetValueFromStr(value, str, meta_data_str[0]); 
    iAssert(ret, _GetValueFromStr);
    voucher.u_acc.user_account = value;

    ret = _GetValueFromStr(value, str, meta_data_str[1]); 
    iAssert(ret, _GetValueFromStr);
    voucher.u_acc.user_password = value;

    ret = _GetValueFromStr(value, str, meta_data_str[2]); 
    iAssert(ret, _GetValueFromStr);
    voucher.u_ip = value;

    ret = _GetValueFromStr(value, str, meta_data_str[3]); 
    iAssert(ret, _GetValueFromStr);
    voucher.u_address= value;

    ret = _GetValueFromStr(value, str, meta_data_str[4]); 
    iAssert(ret, _GetValueFromStr);
    voucher.u_timestamp = static_cast<uint32_t>(stoi(value));

    ret = _GetValueFromStr(value, str, meta_data_str[5]); 
    iAssert(ret, _GetValueFromStr);
    voucher.is_valid = stoi(value);

    return 0;
}

int SPATools::_GetValueFromStr(std::string& value, const std::string& str, const std::string& key) 
{
    std::string src = str;

    int pos = src.find(key);
    if (pos == src.size())
    {
        TLOG((ERR, "key not found"));
        return -1;
    }

    // str:    account:xxx
    // key:    account:
    // pos:    p
    int value_begin_index = pos + key.size();
    if (value_begin_index >= src.size()) 
    {
        TLOG((ERR, "value not found"));
        return -1;
    }
    src = src.substr(value_begin_index);
    pos = src.find(",");
    if (pos == src.size()) 
    {
        TLOG((ERR, "format error"));
        return -1;
    }

    // xxx,
    // 0  p
    value = src.substr(0, pos);

    return 0;
}
