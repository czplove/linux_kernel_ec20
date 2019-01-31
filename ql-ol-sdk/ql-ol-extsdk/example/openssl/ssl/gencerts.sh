#!/bin/sh
#Author: Running.Qian
#Date  : 2018-01-08
echo " Quectel Wireless Solutions Co., Ltd. All rights reserved"

rm *.pem *.csr *.srl

#solve the "unable to write 'random state'"problem when openssl genrsa in readonly filesystem
HOME=./

echo "Gen CA Cert..."
openssl genrsa -out rootca_key.pem 2048
openssl req -new -key rootca_key.pem -out rootca_cert.csr -subj "/C=CN/ST=ANHUI/L=HEFEI/O=QUECTEL/OU=OPEN/CN=rootca" -days 7300
openssl x509 -req -days 7300 -sha256 -signkey rootca_key.pem -in rootca_cert.csr -out rootca_cert.pem
echo "Done."

echo "Gen Server Cert..."
openssl genrsa -out server_key.pem 2048
openssl req -new -key ./server_key.pem -out server_cert.csr -subj "/C=CN/ST=ANHUI/L=HEFEI/O=QUECTEL/OU=OPEN/CN=sever" -days 7300
openssl x509 -req -days 7300 -sha256  -CA rootca_cert.pem -CAkey rootca_key.pem -CAcreateserial -in server_cert.csr -out server_cert.pem
openssl verify -CAfile rootca_cert.pem server_cert.pem
echo "Done."

echo "Gen Client Cert..."
openssl genrsa -out client_key.pem 2048
openssl req -new -key ./client_key.pem -out client_cert.csr -subj "/C=CN/ST=ANHUI/L=HEFEI/O=QUECTEL/OU=OPEN/CN=client" -days 7300
openssl x509 -req -days 7300 -sha256  -CA rootca_cert.pem -CAkey rootca_key.pem -CAcreateserial -in client_cert.csr -out client_cert.pem
openssl verify -CAfile rootca_cert.pem client_cert.pem
echo "Done."
