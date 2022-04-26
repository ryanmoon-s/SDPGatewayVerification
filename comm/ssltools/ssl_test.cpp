#include "ssl_tool.h"
#include "comm/commdef/comm_def.h"
#include "comm/tlog/tlog.h"

int main () 
{
    /* ---------- RSA ---------- */
#if 1
    SSLTools tool;
    std::string text;
    for (int i = 0; i < 100; i++) 
    {
        text.append("hello world ");
    }
    TLOG_DBG(("size:%d src_text:%s",text.size(), text.c_str()));

    // pub encrypt
    std::string en_text;
    tool.RSAEncrypt(en_text, text, RSA_PUB_KEY_CONTROLLER);
    TLOG_DBG(("size:%d en_text:%s", en_text.size(), en_text.c_str()));

    std::string de_text;
    tool.RSADecrypt(de_text, en_text, RSA_PRI_KEY_CONTROLLER);
    TLOG_DBG(("size:%d, de_text:%s", de_text.size(), de_text.c_str()));

    /* ---------- Sign ---------- */

    std::string sign_text;
    tool.RSASign(sign_text, de_text, RSA_PRI_KEY_CONTROLLER);
    TLOG_DBG(("size:%d, sign_text:%s", sign_text.size(), sign_text.c_str()));
    int ver = tool.RSAVerify(sign_text, de_text, RSA_PUB_KEY_CONTROLLER);
    TLOG_DBG(("ver:%d", ver));

    /* ---------- MD5 ---------- */
    
    std::string md5_result;
    tool.MD5Encrypt(md5_result, text);
    TLOG_DBG(("md5_result:%s", md5_result.c_str()));
#endif

    return 0;
}
