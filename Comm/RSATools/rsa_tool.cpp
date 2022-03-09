#include "rsa_tool.h"
#include "tlog.h"

int RSATools::RSAEncrypt(std::string& en_text, std::string text, std::string key_path, int op) { 
    RSA *rsa = NULL;
    FILE *fp = NULL;
    char *en = NULL;
    int len = 0;
    int rsa_len = 0;

    fp = fopen(key_path.c_str(), "r");
    if (fp == NULL) {
        TLOG((ERR, "fopen"));
        return -1;
    }
}
