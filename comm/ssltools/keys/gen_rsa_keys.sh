#!/bin/bash

# rsa keys 
# 用于SPA敲门

dir_source=`pwd`

dir_rsa="${dir_source}/rsa"
if [ ! -d ${dir_rsa} ]; then
    mkdir -p ${dir_rsa}
fi
cd ${dir_rsa}

# ----- controller -----

echo "# 生成密钥"
openssl genrsa -out controller_key.pem 1024
echo "# 提取公钥"
openssl rsa -in controller_key.pem -pubout -out controller_pubkey.pem

# ----- gateway -----

echo "# 生成密钥"
openssl genrsa -out gateway_key.pem 1024
echo "# 提取公钥"
openssl rsa -in gateway_key.pem -pubout -out gateway_pubkey.pem
