#!/bin/bash

if [ $# -lt 2 ]; then
    echo "Usage: sign.sh dir xx.csr"
    exit 1
fi

# 证书输出的根目录
sslOutputRoot="/etc/feature_ssl"
sslOutputRoot=$1
# 签署的csr文件 
CSR=$2

if [ ! -d ${sslOutputRoot} ]; then
    mkdir -p ${sslOutputRoot}
fi
cd ${sslOutputRoot}


echo "# 开始使用CA根证书签署服务器证书签署文件 ..."
#
# 签署服务器证书，生成server.crt文件
# 参见 http://www.faqs.org/docs/securing/chap24sec195.html
#  sign.sh START
#
#  Sign a SSL Certificate Request (CSR)
#  Copyright (c) 1998-1999 Ralf S. Engelschall, All Rights Reserved.
#


case $CSR in
*.csr ) CERT="`echo $CSR | sed -e 's/\.csr/.crt/'`" ;;
* ) CERT="$CSR.crt" ;;
esac

#   make sure environment exists
if [ ! -d ca.db.certs ]; then
 mkdir ca.db.certs
fi
if [ ! -f ca.db.serial ]; then
 echo '01' >ca.db.serial
fi
if [ ! -f ca.db.index ]; then
 cp /dev/null ca.db.index
fi

 
#   create an own SSLeay config
# 如果需要修改证书的有效期限，请修改下面的 default_days 参数.
# 当前设置为10年.
cat >ca.config <<EOT
[ ca ]
default_ca = CA_own
[ CA_own ]
dir = .
certs = ./certs
new_certs_dir = ./ca.db.certs
database = ./ca.db.index
serial = ./ca.db.serial
RANDFILE = ./ca.db.rand
certificate = ./ca.crt
private_key = ./ca.key
default_days = 3650
default_crl_days = 30
default_md = md5
preserve = no
policy = policy_anything
[ policy_anything ]
countryName = optional
stateOrProvinceName = optional
localityName = optional
organizationName = optional
organizationalUnitName = optional
commonName = supplied
emailAddress = optional
EOT

 
#  sign the certificate
echo "CA signing: $CSR -> $CERT:"
openssl ca -config ca.config -out $CERT -infiles $CSR
echo "CA verifying: $CERT <-> CA cert"
openssl verify -CAfile ./certs/ca.crt $CERT


#  cleanup after SSLeay
rm -f ca.config
rm -f ca.db.serial.old
rm -f ca.db.index.old


# 修改 server.key 的权限，保证密钥安全
chmod 400 server.key


#  die gracefully
echo "# 使用CA根证书签署服务器证书签署文件完毕"
exit 0
