#!/bin/bash

# 生成密钥
# -out 指定生成文件，此文件包含公钥和私钥两部分
# 1024 生成密钥的长度
openssl genrsa -out key.pem 1024

# 提取公钥
# -in 指定输入的密钥文件
# -out 指定提取生成公钥的文件(PEM公钥格式)
openssl rsa -in key.pem -pubout -out pubkey.pem
