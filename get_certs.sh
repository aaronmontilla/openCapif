##### Set environment variables
capifregister="capifhost" # Configure hostname in /etc/hosts, if needed
capifregisterport="8084"
capifcore="capifhost" # Configure hostname in /etc/hosts, if needed
capifcoreport="443"

##### Retrieve servers certificates:
echo | openssl s_client -showcerts -connect $capifregister:$capifregisterport 2>/dev/null | openssl x509 -text | sed -ne '/-BEGIN CERTIFICATE-/,/-END CERTIFICATE-/p' > certificates/ca_register.crt
echo | openssl s_client -showcerts -connect $capifcore:$capifcoreport 2>/dev/null | openssl x509 -text | sed -ne '/-BEGIN CERTIFICATE-/,/-END CERTIFICATE-/p' > certificates/ca_capifcore.crt

echo "CA certificates (ca_register.crt and ca_capifcore.crt) obtained."

openssl req -out certificates/tmp_aef.csr -new -subj "/C=ES/ST=Spain/L=Madrid/O=Nokia/OU=ATG/CN=invoker_aaron/emailAddress=atg@nokia.com" -newkey rsa:2048 -nodes -keyout certificates/aaron_aef.key
openssl req -out certificates/tmp_apf.csr -new -subj "/C=ES/ST=Spain/L=Madrid/O=Nokia/OU=ATG/CN=invoker_aaron/emailAddress=atg@nokia.com" -newkey rsa:2048 -nodes -keyout certificates/aaron_apf.key
openssl req -out certificates/tmp_amf.csr -new -subj "/C=ES/ST=Spain/L=Madrid/O=Nokia/OU=ATG/CN=invoker_aaron/emailAddress=atg@nokia.com" -newkey rsa:2048 -nodes -keyout certificates/aaron_amf.key

jq -sR . certificates/tmp_apf.csr | tr -d ""|tr -d '\n' | tr -d '"' > certificates/aaron_apf.csr
jq -sR . certificates/tmp_aef.csr | tr -d ""|tr -d '\n' | tr -d '"' > certificates/aaron_aef.csr
jq -sR . certificates/tmp_amf.csr | tr -d ""|tr -d '\n' | tr -d '"' > certificates/aaron_amf.csr

rm certificates/tmp_aef.csr certificates/tmp_apf.csr certificates/tmp_amf.csr

echo "CSR files (aaron_aef.csr, aaron_apf.csr and aaron_amf.csr) obtained."