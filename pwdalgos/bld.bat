@echo off
set OPENSSL_PATH=C:\openssl\inc32
set OPENSSL_LIB=C:\openssl\out32\libeay32.lib
cl /nologo /O1 apache\aprcrypt.c /I %OPENSSL_PATH% %OPENSSL_LIB%
cl /nologo /O1 bcrypt\bcrypt.c bcrypt\blf.c
cl /nologo /O1 cisco\cisco_pix.c /I %OPENSSL_PATH% %OPENSSL_LIB%
cl /nologo /O1 cisco\cisco4.c /I %OPENSSL_PATH% %OPENSSL_LIB%
cl /nologo /O1 cisco\cisco5.c /I %OPENSSL_PATH% %OPENSSL_LIB%
cl /nologo /O1 cisco\cisco7.c /I %OPENSSL_PATH% %OPENSSL_LIB%
cl /nologo /O1 cisco\cisco8.c /I %OPENSSL_PATH% %OPENSSL_LIB%
cl /nologo /O1 cisco\cisco9.c /I %OPENSSL_PATH% %OPENSSL_LIB%
cl /nologo /O1 descrypt\crypt.c
cl /nologo /O1 ike_psk\ike_psk.c /I %OPENSSL_PATH% %OPENSSL_LIB%
:cl /nologo /O1 kerberos\krb_rc4.c /I %OPENSSL_PATH% %OPENSSL_LIB%
:cl /nologo /O1 kerberos\krb_aes.c /I %OPENSSL_PATH% %OPENSSL_LIB%
cl /nologo /O1 ldap\ldap_pwd.c /I %OPENSSL_PATH% %OPENSSL_LIB%
cl /nologo /O1 md5crypt\md5crypt.c /I %OPENSSL_PATH% %OPENSSL_LIB%
:cl /nologo /O1 ms_chap\aprcrypt.c /I %OPENSSL_PATH% %OPENSSL_LIB%
cl /nologo /O1 ms_dcc\dcc.c /I %OPENSSL_PATH% %OPENSSL_LIB%
cl /nologo /O1 ms_lm\lm.c /I %OPENSSL_PATH% %OPENSSL_LIB%
cl /nologo /O1 ms_ntlm\ntlm.c /I %OPENSSL_PATH% %OPENSSL_LIB%
:cl /nologo /O1 ms_sql\ms_sql.c /I %OPENSSL_PATH% %OPENSSL_LIB%
cl /nologo /O1 mysql\mysql.c /I %OPENSSL_PATH% %OPENSSL_LIB%
cl /nologo /O1 oracle\oracle.c /I %OPENSSL_PATH% %OPENSSL_LIB%
:cl /nologo /O1 psafe\psafe.c /I %OPENSSL_PATH% %OPENSSL_LIB%
cl /nologo /O1 scrypt\scrypt.c /I %OPENSSL_PATH% %OPENSSL_LIB%
cl /nologo /O1 sha1crypt\sha1crypt.c /I %OPENSSL_PATH% %OPENSSL_LIB%
cl /nologo /O1 sha256crypt\sha256crypt.c /I %OPENSSL_PATH% %OPENSSL_LIB%
cl /nologo /O1 sha512crypt\sha512crypt.c /I %OPENSSL_PATH% %OPENSSL_LIB%
cl /nologo /O1 skype\skype.c /I %OPENSSL_PATH% %OPENSSL_LIB%
cl /nologo /O1 wpa_psk\wpa_psk.c /I %OPENSSL_PATH% %OPENSSL_LIB%
del *.obj *.err