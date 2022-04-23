#!/bin/bash

# rsa keys + sign
# 用于建立ssl连接

# Comm Name 必须全部都是不一样的

dir_source=`pwd`

# ----- ca -----

dir_certs="${dir_source}/certs"
if [ ! -d ${dir_certs} ]; then
    mkdir -p ${dir_certs}
fi
cd ${dir_certs}

echo "# == CA证书 == "

echo "# 生成CA的key文件"
openssl genrsa -des3 -out ca.key 1024 

echo "# 生成CA自签名的证书"
openssl req -new -x509 -key ca.key -out ca.crt 

# ----- controller -----

cd ${dir_source}
dir_controller="${dir_source}/controller"
if [ ! -d ${dir_controller} ]; then
    mkdir -p ${dir_controller}
fi
cd ${dir_controller}

echo "# == Controller == "

echo "# 生成私钥"
openssl genrsa -des3 -out controller.key 1024 

echo "# 去除key文件口令"
openssl rsa -in controller.key -out controller.key

echo "# 生成证书签署请求文件"
openssl req -new -key controller.key -out controller.csr

# ----- gateway -----

cd ${dir_source}
dir_gateway="${dir_source}/gateway"
if [ ! -d ${dir_gateway} ]; then
    mkdir -p ${dir_gateway}
fi
cd ${dir_gateway}

echo "# == Gateway == "

echo "# 生成私钥"
openssl genrsa -des3 -out gateway.key 1024 

echo "# 去除key文件口令"
openssl rsa -in gateway.key -out gateway.key

echo "# 生成证书签署请求文件"
openssl req -new -key gateway.key -out gateway.csr

# ----- client -----

cd ${dir_source}
dir_client="${dir_source}/client"
if [ ! -d ${dir_client} ]; then
    mkdir -p ${dir_client}
fi
cd ${dir_client}

echo "# == client == "

echo "# 生成私钥"
openssl genrsa -des3 -out client.key 1024 

echo "# 去除key文件口令"
openssl rsa -in client.key -out client.key

echo "# 生成证书签署请求文件"
openssl req -new -key client.key -out client.csr

# ----- Sign -----

echo "# == 用CA证书签名 == "

cd ${dir_source}

sh sign.sh ${dir_controller} controller.csr
sh sign.sh ${dir_gateway} gateway.csr
sh sign.sh ${dir_client} client.csr
