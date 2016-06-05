#ifndef XAN_TLS_H
#define XAN_TLS_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_tlsrecordlayerpacket.h>
#include <xan_tls_defines.h>
#include <xan_tcpclient.h>
#include <xan_rsa.h>
enum XEnum_TlsBulkCipherAlgorithm
{
	XEnum_TlsBulkCipherAlgorithm_null , 
	XEnum_TlsBulkCipherAlgorithm_rc4 ,
	XEnum_TlsBulkCipherAlgorithm_rc2 ,
	XEnum_TlsBulkCipherAlgorithm_des ,
	XEnum_TlsBulkCipherAlgorithm_3des ,
	XEnum_TlsBulkCipherAlgorithm_des40 ,
	XEnum_TlsBulkCipherAlgorithm_aes
};
enum XEnum_TlsCipherType
{ 
	XEnum_CipherType_stream ,
	XEnum_CipherType_block 
};
enum XEnum_TlsMACAlgorithm
{
	XEnum_TlsMACAlgorithm_null , 
	XEnum_TlsMACAlgorithm_md5 ,
	XEnum_TlsMACAlgorithm_sha 
};
namespace rux
{
	namespace network
	{
		begin_declare_rux_class( Tls );
			friend class XTlsFinished;
			friend class XTlsRecordLayerPacket;
			~Tls();
			void AppendHandshakeMessage( const rux::uint8* handshake_message_ptr , rux::uint32 handshake_message_length );
			void PRF( const rux::uint8* secret_ptr , rux::uint32 secret_ptr_length , const char* label_ptr , const rux::uint8* seed_ptr , rux::uint32 seed_ptr_length , rux::uint8* result_ptr , rux::uint32 result_ptr_size , rux::XString& error );
			void P_hash( XEnum_TlsMACAlgorithm hash_algorithm , const rux::uint8* secret_ptr , rux::uint32 secret_ptr_length , const rux::uint8* seed_ptr , rux::uint32 seed_ptr_length , rux::uint8* result_ptr , rux::uint32 result_ptr_size , rux::XString& error );
			void MAC( rux::uint8* data_ptr , rux::uint32 data_ptr_length , XEnum_TlsConnectionEnd connection_end , rux::uint8 mac[ 20 ] , rux::XString& error );
		begin_declare_rux_class_members( Tls );
			rux::uint8 _major_version;
			rux::uint8 _minor_version;	
			//XTcpClient _tcp_client;
			XTlsRecordLayerPacket _tls_record_layer_packet;
			rux::XRandomNumberGenerator _random_generator;
			rux::uint8 _master_secret[ 48 ];
			rux::uint8 _client_random[ 32 ];
			rux::uint8 _server_random[ 32 ];
			rux::uint8 _is_exportable;
			rux::uint8 _key_size;
			rux::uint8 _key_material_size;
			rux::uint8 _hash_size;
			rux::uint8 _compression_algorithm;
			XEnum_TlsBulkCipherAlgorithm _bulk_cipher_algorithm;
			XEnum_TlsCipherType _cipher_type;
			XEnum_TlsMACAlgorithm _mac_algorithm;
			rux::uint32 _iv_size;
			XObject _bulk_cipher_encrypter;
			XObject _bulk_cipher_decrypter;
			rux::uint8* _handshake_messages;
			rux::uint32 _handshake_messages_size;
			rux::uint32 _handshake_messages_length;
			rux::uint8* _client_write_mac_key;
			rux::uint8* _server_write_mac_key;
			rux::uint8* _client_encryption_key;
			rux::uint8* _server_encryption_key;
			rux::uint8* _client_write_iv;
			rux::uint8* _server_write_iv;
			rux::uint8 _client_sequence_number[ 8 ];
			rux::uint8 _server_sequence_number[ 8 ];
			rux::uint8 _wait_for_encrypted_data;
			rux::uint8* _decrypted_data_ptr;
			rux::uint32 _decrypted_data_ptr_size;
			rux::uint32 _decrypted_data_ptr_length;
			rux::uint8* _encrypted_data_ptr;
			rux::uint32 _encrypted_data_ptr_size;
			rux::uint32 _encrypted_data_ptr_length;
			rux::uint8* _pre_mac_data_ptr;
			rux::uint32 _pre_mac_data_ptr_size;	
			rux::uint32 _pre_mac_data_ptr_length;	
			XEnum_TlsConnectionEnd _connection_end;
			rux::uint8 _is_connected;
		end_declare_rux_class_members( Tls );			
			void Connect( const rux::network::XAddress& network_address , rux::XString& error );
			void Disconnect( void );
			//XEnum_TcpClient_ErrorType Send( const XObject& send_data , XObject& receive_data , NetworkFormatFunction network_format , NetworkParseFunction network_parse , NetworkGetLengthFunction network_get_length , rux::XString& error );	
			//XEnum_TcpClient_ErrorType Send( const XObject& send_data , NetworkFormatFunction network_format , rux::XString& error );	
			//XEnum_TcpClient_ErrorType Receive( const XObject& receive_data , NetworkParseFunction network_parse , NetworkGetLengthFunction network_get_length , rux::XString& error );
		private:
			void ExportCipherSuiteFields( XEnum_TlsCipherSuite cipher_suite , XEnum_TlsBulkCipherAlgorithm& bulk_cipher_algorithm , XEnum_TlsCipherType& cipher_type , XEnum_TlsMACAlgorithm& mac_algorithm , rux::uint8& key_size , rux::uint8& key_material_size , rux::uint8& hash_size , rux::uint32& iv_size );	
			void IncrementClientSequenceNumber( void );
			void IncrementServerSequenceNumber( void );
		end_declare_rux_class();
	};
};
#endif
