#include "ssl_tool.h"
#include "comm/commdef/comm_def.h"
#include "comm/tlog/tlog.h"

int main () 
{
    /* ---------- RSA ---------- */

    // encrypt
    SSLTools tool;
    std::string text;
    // text = "arashi,pass,127.0.0.1,ChengDu,1647098221,1";
    for (int i = 0; i < 100; i++) 
    {
        text.append("hello world ");
    }
    TLOG_DBG(("size:%d src_text:%s",text.size(), text.c_str()));
    std::string en_text;
    tool.RSAEncrypt(en_text, text, RSA_PUB_KEY_CONTROLLER, PUB_ENCRYPT);
    TLOG_DBG(("size:%d en_text:%s", en_text.size(), en_text.c_str()));

    // decrypt
    std::string de_text;
    tool.RSADecrypt(de_text, en_text, RSA_PRI_KEY_CONTROLLER, PUB_ENCRYPT);
    TLOG_DBG(("size:%d, de_text:%s", de_text.size(), de_text.c_str()));

    /* ---------- MD5 ---------- */
    std::string md5_result;
    tool.MD5Encrypt(md5_result, text);
    TLOG_DBG(("md5_result:%s", md5_result.c_str()));

    /* ---------- SSL ---------- */

    

    return 0;
}
