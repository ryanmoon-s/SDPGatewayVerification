#!/bin/bash

# ---------- RSA ----------

echo "# 生成密钥"
openssl genrsa -out key.pem 1024
echo "# 提取公钥"
openssl rsa -in key.pem -pubout -out pubkey.pem

# ---------- TLS ----------

echo "# == Server == "

echo "# 生成私钥"
openssl genrsa -des3 -out server.key 1024 
echo "# 去除key文件口令"
openssl rsa -in server.key -out server.key
echo "# 生成证书签署请求文件"
openssl req -new -key server.key -out server.csr

echo "# == Client == "

echo "# 生成私钥"
openssl genrsa -des3 -out client.key 1024 
echo "# 去除key文件口令"
openssl rsa -in client.key -out client.key
echo "# 生成证书签署请求文件"
openssl req -new -key client.key -out client.csr

echo "# == CA签名 == "

echo "# 生成CA的key文件"
openssl genrsa -des3 -out ca.key 1024 
echo "# 生成CA自签名的证书"
openssl req -new -x509 -key ca.key -out ca.crt 
echo "# 用生成的CA的证书为server.csr, client.csr文件签名"
./sign.sh ./ server.csr
./sign.sh ./ client.csr