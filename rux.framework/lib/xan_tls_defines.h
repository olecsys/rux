#ifndef XAN_TLS_DEFINES_H
#define XAN_TLS_DEFINES_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
enum XEnum_TlsConnectionEnd
{
	XEnum_TlsConnectionEnd_server ,
	XEnum_TlsConnectionEnd_client
};
enum XEnum_TlsContentType
{
	XEnum_TlsContentType_change_cipher_spec = 0x14 , 
	XEnum_TlsContentType_alert = 0x15 , 
	XEnum_TlsContentType_handshake = 0x16 ,
	XEnum_TlsContentType_application_data = 0x17
};
enum XEnum_TlsHandshakeType
{
	XEnum_TlsHandshakeType_hello_request = 0x00 ,
	XEnum_TlsHandshakeType_client_hello = 0x01 ,
	XEnum_TlsHandshakeType_server_hello = 0x02 ,
	XEnum_TlsHandshakeType_certificate = 0x0b ,
	XEnum_TlsHandshakeType_server_key_exchange = 0x0c ,
	XEnum_TlsHandshakeType_certificate_request = 0x0d ,
	XEnum_TlsHandshakeType_server_hello_done = 0x0e ,
	XEnum_TlsHandshakeType_certificate_verify = 0x0f ,
	XEnum_TlsHandshakeType_client_key_exchange = 0x10 ,
	XEnum_TlsHandshakeType_finished = 0x14
};
enum XEnum_TlsCipherSuite
{
	XEnum_CipherSuite_TLS_RSA_WITH_AES_128_CBC_SHA = 0x00 ,
	XEnum_CipherSuite_TLS_DH_DSS_WITH_AES_128_CBC_SHA = 0x01 ,
	XEnum_CipherSuite_TLS_DH_RSA_WITH_AES_128_CBC_SHA = 0x02 ,
	XEnum_CipherSuite_TLS_DHE_DSS_WITH_AES_128_CBC_SHA = 0x03 ,
	XEnum_CipherSuite_TLS_DHE_RSA_WITH_AES_128_CBC_SHA = 0x04 ,
	XEnum_CipherSuite_TLS_DH_anon_WITH_AES_128_CBC_SHA = 0x05 ,
	XEnum_CipherSuite_TLS_RSA_WITH_AES_256_CBC_SHA = 0x06 ,
	XEnum_CipherSuite_TLS_DH_DSS_WITH_AES_256_CBC_SHA = 0x07 ,
	XEnum_CipherSuite_TLS_DH_RSA_WITH_AES_256_CBC_SHA = 0x08 ,
	XEnum_CipherSuite_TLS_DHE_DSS_WITH_AES_256_CBC_SHA = 0x09 ,
	XEnum_CipherSuite_TLS_DHE_RSA_WITH_AES_256_CBC_SHA = 0x0a ,
	XEnum_CipherSuite_TLS_DH_anon_WITH_AES_256_CBC_SHA = 0x0b ,
	XEnum_CipherSuite_TLS_SSL_RSA_WITH_RC4_128_MD5 = 0x0c
};
const rux::uint8 tls_cipher_suite_values[][ 2 ] =
{
	{ 0x00, 0x2F } ,	{ 0x00, 0x30 } , { 0x00, 0x31 } , { 0x00, 0x32 } , { 0x00, 0x33 } , { 0x00, 0x34 } ,
  { 0x00, 0x35 } , { 0x00, 0x36 } , { 0x00, 0x37 } , { 0x00, 0x38 } , { 0x00, 0x39 } , { 0x00, 0x3A }	,
	{ 0x00,0x04 }
};
const rux::uint8 sslv2_cipher_suite_values[][ 3 ] =
{
	{ 0x00 , 0x00, 0x2F } ,	{ 0x00 , 0x00, 0x30 } , { 0x00 , 0x00, 0x31 } , { 0x00 , 0x00, 0x32 } , { 0x00 , 0x00, 0x33 } , { 0x00 , 0x00, 0x34 } ,
  { 0x00 , 0x00, 0x35 } , { 0x00 , 0x00, 0x36 } , { 0x00 , 0x00, 0x37 } , { 0x00 , 0x00, 0x38 } , { 0x00 , 0x00, 0x39 } , { 0x00 , 0x00, 0x3A }	,
	{ 0x00 , 0x00,0x04 }
};

#endif
