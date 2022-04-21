#!/bin/bash

# rsa keys + sign
# 用于建立ssl连接

dir_source=`pwd`

# ----- CA -----

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


# ----- Server -----

cd ${dir_source}
dir_server="${dir_source}/server"
if [ ! -d ${dir_server} ]; then
    mkdir -p ${dir_server}
fi
cd ${dir_server}

echo "# == Server == "

echo "# 生成私钥"
openssl genrsa -des3 -out server.key 1024 

echo "# 去除key文件口令"
openssl rsa -in server.key -out server.key

echo "# 生成证书签署请求文件"
openssl req -new -key server.key -out server.csr

# ----- Client -----

cd ${dir_source}
dir_client="${dir_source}/client"
if [ ! -d ${dir_client} ]; then
    mkdir -p ${dir_client}
fi
cd ${dir_client}

echo "# == Client == "

echo "# 生成私钥"
openssl genrsa -des3 -out client.key 1024 

echo "# 去除key文件口令"
openssl rsa -in client.key -out client.key

echo "# 生成证书签署请求文件"
openssl req -new -key client.key -out client.csr


# ----- Sign -----

echo "# == 用CA证书签名 == "

cd ${dir_source}

sh sign.sh ${dir_server} server.csr
sh sign.sh ${dir_client} client.csr
