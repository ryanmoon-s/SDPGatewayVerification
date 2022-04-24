cd ${dir_source}
dir_application="${dir_source}/application"
if [ ! -d ${dir_application} ]; then
    mkdir -p ${dir_application}
fi
cd ${dir_application}

echo "# == Gateway == "

echo "# 生成私钥"
openssl genrsa -des3 -out application.key 1024 

echo "# 去除key文件口令"
openssl rsa -in application.key -out application.key

echo "# 生成证书签署请求文件"
openssl req -new -key application.key -out application.csr

sh sign.sh ${dir_application} application.csr
