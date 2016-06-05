#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_tls.h>
#include <xan_hmac.h>
#include <xan_keyvaluepair.h>
#include <xan_uint64.h>
#include <xan_boolean.h>
begin_implement_rux_class_ns( Tls , rux::network );
	_tls_record_layer_packet = XTlsRecordLayerPacket( this );
	_major_version = 3;
	_minor_version = 1;
	memset( _master_secret , 0 , 48 );
	memset( _client_random , 0 , 32 );
	memset( _server_random , 0 , 32 );
	memset( _client_sequence_number , 0 , 8 );
	memset( _server_sequence_number , 0 , 8 );
	_is_exportable = 0;
	_key_size = 0;
	_key_material_size = 0;
	_hash_size = 0;
	_compression_algorithm = 0;
	_bulk_cipher_algorithm = XEnum_TlsBulkCipherAlgorithm_null;
	_cipher_type = XEnum_CipherType_block;
	_mac_algorithm = XEnum_TlsMACAlgorithm_null;
	_iv_size = 0;
	_handshake_messages = NULL;
	_handshake_messages_size = 0;
	_handshake_messages_length = 0;
	_client_write_mac_key = NULL;
	_server_write_mac_key = NULL;
	_client_encryption_key = NULL;
	_server_encryption_key = NULL;
	_client_write_iv = NULL;
	_server_write_iv = NULL;	
	_wait_for_encrypted_data = 0;
	_decrypted_data_ptr = NULL;
	_decrypted_data_ptr_size = 0;
	_decrypted_data_ptr_length = 0;
	_encrypted_data_ptr = NULL;
	_encrypted_data_ptr_size = 0;
	_encrypted_data_ptr_length = 0;
	_connection_end = XEnum_TlsConnectionEnd_client;
	_pre_mac_data_ptr = NULL;
	_pre_mac_data_ptr_size = 0;	
	_pre_mac_data_ptr_length = 0;
	_is_connected = 0;
end_implement_rux_class();
namespace rux
{
	namespace network
	{
		Tls::~Tls()
		{
			if( _handshake_messages )
				rux::engine::free_mem( _handshake_messages );
			if( _client_write_mac_key )
				rux::engine::free_mem( _client_write_mac_key );
			if( _server_write_mac_key )
				rux::engine::free_mem( _server_write_mac_key );
			if( _client_encryption_key )
				rux::engine::free_mem( _client_encryption_key );
			if( _server_encryption_key )
				rux::engine::free_mem( _server_encryption_key );
			if( _client_write_iv )
				rux::engine::free_mem( _client_write_iv );
			if( _server_write_iv )
				rux::engine::free_mem( _server_write_iv );
			if( _decrypted_data_ptr )
				rux::engine::free_mem( _decrypted_data_ptr );
			if( _encrypted_data_ptr )
				rux::engine::free_mem( _encrypted_data_ptr );
			if( _pre_mac_data_ptr )
				rux::engine::free_mem( _pre_mac_data_ptr );
		};
		void Tls::AppendHandshakeMessage( const rux::uint8* handshake_message_ptr , rux::uint32 handshake_message_length )
		{
			if( _handshake_messages == NULL )
			{
				if( handshake_message_length <= 1024 )
					_handshake_messages_size = 1024;
				else
					_handshake_messages_size = handshake_message_length;		
				_handshake_messages = alloc_array_macros( rux::uint8 , _handshake_messages_size );
			}
			else if( _handshake_messages_length + handshake_message_length > _handshake_messages_size )
			{
				_handshake_messages_size *= 2;
				if( handshake_message_length > _handshake_messages_size )
					_handshake_messages_size += handshake_message_length;
				rux::uint8* handshake_messages = alloc_array_macros( rux::uint8 , _handshake_messages_size );
				memcpy( handshake_messages , _handshake_messages , _handshake_messages_length );
				rux::engine::free_mem( _handshake_messages );
				_handshake_messages = handshake_messages;
			}
			memcpy( &_handshake_messages[ _handshake_messages_length ] , handshake_message_ptr , handshake_message_length );
			_handshake_messages_length += handshake_message_length;
		};
		void Tls::MAC( rux::uint8* data_ptr , rux::uint32 data_ptr_length , XEnum_TlsConnectionEnd connection_end , rux::uint8 mac[ 20 ] , rux::XString& error )
		{
			if( data_ptr )
			{
				if( data_ptr_length > 0 )
				{
					rux::uint8* write_mac_key = NULL;
					if( connection_end == XEnum_TlsConnectionEnd_client )
						write_mac_key = _client_write_mac_key;
					else if( connection_end == XEnum_TlsConnectionEnd_server )
						write_mac_key = _server_write_mac_key;
					if( _mac_algorithm == XEnum_TlsMACAlgorithm_sha )				
						rux::cryptography::XHMAC::ComputeSHA1( write_mac_key , _hash_size , data_ptr , data_ptr_length , mac );								
				}
				else
					error = "Data pointer length is equal 0";
			}
			else
				error = "Data pointer is NULL";
		};
		void Tls::P_hash( XEnum_TlsMACAlgorithm hash_algorithm , const rux::uint8* secret_ptr , rux::uint32 secret_ptr_length , const rux::uint8* seed_ptr , rux::uint32 seed_ptr_length , rux::uint8* result_ptr , rux::uint32 result_ptr_size , rux::XString& error )
		{
			error.Clear();
			if( secret_ptr != NULL )
			{
				if( secret_ptr_length > 0 )
				{			
					if( seed_ptr )
					{
						if( seed_ptr_length > 0 )
						{
							if( result_ptr )
							{
								if( result_ptr_size > 0 )
								{
									if( hash_algorithm == XEnum_TlsMACAlgorithm_md5 )
									{
										rux::uint8 a[ 16 ] = {0};
										rux::uint8* b = alloc_array_macros( rux::uint8 , 16 + seed_ptr_length );								
										rux::cryptography::XHMAC::ComputeMD5( secret_ptr , secret_ptr_length , seed_ptr , seed_ptr_length , a );			
										memcpy( &b[ 16 ] , seed_ptr , seed_ptr_length );
										seed_ptr_length += 16;
										for( rux::uint32 index0 = 0 ; index0 < result_ptr_size ; index0 += 16 )
										{
											memcpy( b , a , 16 );
											if( result_ptr_size - index0 < 16 )
											{
												rux::cryptography::XHMAC::ComputeMD5( secret_ptr , secret_ptr_length , b , seed_ptr_length , a );
												memcpy( &result_ptr[ index0 ] , a , result_ptr_size - index0 );
											}
											else
											{
												rux::cryptography::XHMAC::ComputeMD5( secret_ptr , secret_ptr_length , b , seed_ptr_length , &result_ptr[ index0 ] );
												rux::cryptography::XHMAC::ComputeMD5( secret_ptr , secret_ptr_length , b , 16 , a );
											}
										}
									}							
									else if( hash_algorithm == XEnum_TlsMACAlgorithm_sha )
									{
										rux::uint8 a[ 20 ] = {0};
										rux::uint8* b = alloc_array_macros( rux::uint8 , 20 + seed_ptr_length );								
										rux::cryptography::XHMAC::ComputeSHA1( secret_ptr , secret_ptr_length , seed_ptr , seed_ptr_length , a );			
										memcpy( &b[ 20 ] , seed_ptr , seed_ptr_length );
										seed_ptr_length += 20;
										for( rux::uint32 index0 = 0 ; index0 < result_ptr_size ; index0 += 20 )
										{
											memcpy( b , a , 20 );
											if( result_ptr_size - index0 < 20 )
											{
												rux::cryptography::XHMAC::ComputeSHA1( secret_ptr , secret_ptr_length , b , seed_ptr_length , a );
												memcpy( &result_ptr[ index0 ] , a , result_ptr_size - index0 );
											}
											else
											{
												rux::cryptography::XHMAC::ComputeSHA1( secret_ptr , secret_ptr_length , b , seed_ptr_length , &result_ptr[ index0 ] );
												rux::cryptography::XHMAC::ComputeSHA1( secret_ptr , secret_ptr_length , b , 20 , a );
											}
										}
									}
								}
								else
									error = "Result pointer length is equal 0";
							}
							else
								error = "Result pointer is NULL";
						}
						else
							error = "Seed pointer length is equal 0";
					}
					else
						error = "Seed pointer is NULL";			
				}
				else
					error = "Secret pointer length is equal 0";
			}
			else
				error = "Secret pointer is NULL";
		};
		void Tls::PRF( const rux::uint8* secret_ptr , rux::uint32 secret_ptr_length , const char* label_ptr , const rux::uint8* seed_ptr , rux::uint32 seed_ptr_length , rux::uint8* result_ptr , rux::uint32 result_ptr_size , rux::XString& error )
		{
			error.Clear();
			if( secret_ptr != NULL )
			{
				if( secret_ptr_length > 0 )
				{
					rux::uint32 label_ptr_length = rux::XString::LengthA( label_ptr );
					if( label_ptr_length > 0 )
					{
						if( seed_ptr )
						{
							if( seed_ptr_length > 0 )
							{
								if( result_ptr )
								{
									if( result_ptr_size > 0 )
									{
										if( _major_version == 3 
											&& _minor_version == 1 )
										{
											rux::uint32 s_size = secret_ptr_length / 2;
											rux::uint32 sha1_offset = s_size;
											if( s_size * 2 != secret_ptr_length )									
												s_size += 1;
											rux::uint8* data_ptr = alloc_array_macros( rux::uint8 , label_ptr_length + seed_ptr_length );
											memcpy( data_ptr , label_ptr , label_ptr_length );
											memcpy( &data_ptr[ label_ptr_length ] , seed_ptr , seed_ptr_length );
											P_hash( XEnum_TlsMACAlgorithm_md5 , secret_ptr , s_size , data_ptr , label_ptr_length + seed_ptr_length , result_ptr , result_ptr_size , error );
											if( error.Length() == 0 )
											{
												rux::uint8* sha1_ptr = alloc_array_macros( rux::uint8 , result_ptr_size );
												P_hash( XEnum_TlsMACAlgorithm_sha , &secret_ptr[ sha1_offset ] , s_size , data_ptr , label_ptr_length + seed_ptr_length , sha1_ptr , result_ptr_size , error );
												for( label_ptr_length = 0 ; label_ptr_length < result_ptr_size ; label_ptr_length++ )
													result_ptr[ label_ptr_length ] ^= sha1_ptr[ label_ptr_length ];
												rux::engine::free_mem( sha1_ptr );
											}
											rux::engine::free_mem( data_ptr );
										}								
										else
											error = rux::XString::Format( "PRF for TLS version %ld.%ld is not implemented" , _major_version , _minor_version );
									}
									else
										error = "Result pointer length is equal 0";
								}
								else
									error = "Result pointer is NULL";
							}
							else
								error = "Seed pointer length is equal 0";
						}
						else
							error = "Seed pointer is NULL";
					}
					else
						error = "Label length is equal 0";
				}
				else
					error = "Secret pointer length is equal 0";
			}
			else
				error = "Secret pointer is NULL";
		};
		void XTls::Disconnect( void )
		{
			if( (*this)()->_is_connected == 1 )
			{
				//(*this)()->_tcp_client.Close();
				(*this)()->_wait_for_encrypted_data = 0;
				memset( (*this)()->_client_sequence_number , 0 , 8 );
				memset( (*this)()->_server_sequence_number , 0 , 8 );
				(*this)()->_is_connected = 0;
				(*this)()->_handshake_messages_length = 0;
			}
		};
		void XTls::Connect( const rux::network::XAddress& network_address , rux::XString& error )
		{	
			Disconnect();
			(*this)()->_connection_end = XEnum_TlsConnectionEnd_client;	
			error.Clear();	
			//(*this)()->_tcp_client.set_NetworkAddress( network_address );
			rux::uint8 random[ 128 ] = {0};
			(*this)()->_random_generator.Generate( random );
			XSslv2ClientHello sslv2_client_hello;
			sslv2_client_hello.set_Random( &random[ 32 ] );
			sslv2_client_hello.AddCipherSuite( XEnum_CipherSuite_TLS_RSA_WITH_AES_128_CBC_SHA );
			(*this)()->_tls_record_layer_packet.set_TlsContent( sslv2_client_hello );
			//(*this)()->_tcp_client.Send( (*this)()->_tls_record_layer_packet , (*this)()->_tls_record_layer_packet , XTlsRecordLayerPacket::NetworkFormat , XTlsRecordLayerPacket::NetworkParse , XTlsRecordLayerPacket::NetworkGetLength , error );
			if( error.Length() == 0 )
			{		
				XObject tls_content( (*this)()->_tls_record_layer_packet.get_TlsContent() );
				if( rux_is_object( tls_content , XTlsServerHello ) )
				{
					XTlsServerHello tls_server_hello( tls_content );
					(*this)()->_compression_algorithm = tls_server_hello()->_compression_method;
					ExportCipherSuiteFields( tls_server_hello()->_cipher_suite , (*this)()->_bulk_cipher_algorithm , (*this)()->_cipher_type , (*this)()->_mac_algorithm , (*this)()->_key_size , (*this)()->_key_material_size , (*this)()->_hash_size , (*this)()->_iv_size );
					//(*this)()->_tcp_client.Receive( (*this)()->_tls_record_layer_packet , XTlsRecordLayerPacket::NetworkParse , XTlsRecordLayerPacket::NetworkGetLength , error );
					if( error.Length() == 0 )
					{
						tls_content = (*this)()->_tls_record_layer_packet.get_TlsContent();
						if( rux_is_object( tls_content , XTlsCertificate ) )
						{
							XTlsCertificate tls_certificate( tls_content );
							//(*this)()->_tcp_client.Receive( (*this)()->_tls_record_layer_packet , XTlsRecordLayerPacket::NetworkParse , XTlsRecordLayerPacket::NetworkGetLength , error );
							if( error.Length() == 0 )
							{					
								tls_content = (*this)()->_tls_record_layer_packet.get_TlsContent();
								if( rux_is_object( tls_content , XTlsServerHelloDone ) )
								{
									rux::XArray<rux::cryptography::XCertificateX509v3> certificates = tls_certificate.get_Certificates();
									if( certificates.Count() > 0 )
									{
										tls_content = certificates[ 0 ].get_EncryptionPublicKey();
										if( rux_is_object( tls_content , rux::cryptography::XRsaPublicKey ) )
										{
											rux::cryptography::XRsaPublicKey rsa_public_key( tls_content );
											rux::cryptography::XRSA rsa;									
											rsa.set_PublicKey( rsa_public_key );
											XTlsClientKeyExchange tls_client_key_exchange;
											tls_client_key_exchange.set_Encrypter( rsa );												
											tls_client_key_exchange.set_Random( random );
											(*this)()->_tls_record_layer_packet.set_TlsContent( tls_client_key_exchange );
											//(*this)()->_tcp_client.Send( (*this)()->_tls_record_layer_packet , XTlsRecordLayerPacket::NetworkFormat , error );
											if	( error.Length() == 0 )								
											{										
												memcpy( (*this)()->_client_random , sslv2_client_hello()->_random , 32 );
												memcpy( (*this)()->_server_random , tls_server_hello()->_random , 32 );										
												memcpy( random , (*this)()->_client_random , 32 );
												memcpy( &random[ 32 ] , (*this)()->_server_random , 32 );
												(*this)()->PRF( tls_client_key_exchange()->_pre_master_key , 48 , "master secret" , random , 64 , (*this)()->_master_secret , 48 , error );
												rux::uint32 key_block_size = 2 * (*this)()->_hash_size + 2 * (*this)()->_key_material_size + 2 * (*this)()->_iv_size;
												if( key_block_size > 0 )
												{
													if( (*this)()->_client_write_mac_key )
														rux::engine::free_mem( (*this)()->_client_write_mac_key );
													if( (*this)()->_server_write_mac_key )
														rux::engine::free_mem( (*this)()->_server_write_mac_key );
													if( (*this)()->_client_encryption_key )
														rux::engine::free_mem( (*this)()->_client_encryption_key );
													if( (*this)()->_server_encryption_key )
														rux::engine::free_mem( (*this)()->_server_encryption_key );
													if( (*this)()->_client_write_iv )
														rux::engine::free_mem( (*this)()->_client_write_iv );
													if( (*this)()->_server_write_iv )
														rux::engine::free_mem( (*this)()->_server_write_iv );
													rux::uint8* key_block_ptr = alloc_array_macros( rux::uint8 , key_block_size );
													memcpy( random , (*this)()->_server_random , 32 );										
													memcpy( &random[ 32 ] , (*this)()->_client_random , 32 );
													(*this)()->PRF( (*this)()->_master_secret , 48 , "key expansion" , random , 64 , key_block_ptr , key_block_size , error );										
													(*this)()->_client_write_mac_key = alloc_array_macros( rux::uint8 , (*this)()->_hash_size );
													memcpy( (*this)()->_client_write_mac_key , key_block_ptr , (*this)()->_hash_size );
													key_block_size = (*this)()->_hash_size;
													(*this)()->_server_write_mac_key = alloc_array_macros( rux::uint8 , (*this)()->_hash_size );
													memcpy( (*this)()->_server_write_mac_key , &key_block_ptr[ key_block_size ] , (*this)()->_hash_size );
													key_block_size += (*this)()->_hash_size;
													(*this)()->_client_encryption_key = alloc_array_macros( rux::uint8 , (*this)()->_key_material_size );
													memcpy( (*this)()->_client_encryption_key , &key_block_ptr[ key_block_size ] , (*this)()->_key_material_size );
													key_block_size += (*this)()->_key_material_size;
													(*this)()->_server_encryption_key = alloc_array_macros( rux::uint8 , (*this)()->_key_material_size );
													memcpy( (*this)()->_server_encryption_key , &key_block_ptr[ key_block_size ] , (*this)()->_key_material_size );
													key_block_size += (*this)()->_key_material_size;
													(*this)()->_client_write_iv = alloc_array_macros( rux::uint8 , (*this)()->_iv_size );
													memcpy( (*this)()->_client_write_iv , &key_block_ptr[ key_block_size ] , (*this)()->_iv_size );
													key_block_size += (*this)()->_iv_size;
													(*this)()->_server_write_iv = alloc_array_macros( rux::uint8 , (*this)()->_iv_size );
													memcpy( (*this)()->_server_write_iv , &key_block_ptr[ key_block_size ] , (*this)()->_iv_size );
													if( (*this)()->_bulk_cipher_algorithm == XEnum_TlsBulkCipherAlgorithm_aes )
													{
														rux::cryptography::XAES aes_encrypter;
														rux::cryptography::XAES aes_decrypter;
														aes_encrypter.set_InitializationVector( (*this)()->_client_write_iv );
														aes_decrypter.set_InitializationVector( (*this)()->_server_write_iv );
														aes_encrypter.set_CipherMode( XEnum_SymmetricCryptAlgorithm_CipherMode_BlockChaining );
														aes_decrypter.set_CipherMode( XEnum_SymmetricCryptAlgorithm_CipherMode_BlockChaining );
														if( (*this)()->_key_material_size == 16 )
														{
															aes_encrypter.set_CipherKey( (*this)()->_client_encryption_key , XEnum_AES_KeyType_128bits );
															aes_decrypter.set_CipherKey( (*this)()->_server_encryption_key , XEnum_AES_KeyType_128bits );
														}												
														else if( (*this)()->_key_material_size == 32 )
														{
															aes_encrypter.set_CipherKey( (*this)()->_client_encryption_key , XEnum_AES_KeyType_256bits );
															aes_decrypter.set_CipherKey( (*this)()->_server_encryption_key , XEnum_AES_KeyType_256bits );
														}
														(*this)()->_bulk_cipher_encrypter = aes_encrypter;
														(*this)()->_bulk_cipher_decrypter = aes_decrypter;
													}
												}											
												XTlsChangeCipherSpec tls_change_cipher_spec;
												(*this)()->_tls_record_layer_packet.set_TlsContent( tls_change_cipher_spec );
												//(*this)()->_tcp_client.Send( (*this)()->_tls_record_layer_packet , XTlsRecordLayerPacket::NetworkFormat , error );
												if( error.Length() == 0 )
												{											
													XTlsFinished tls_finished;
													tls_finished.set_VerifyData( (*this)() , (*this)()->_connection_end , error );
													if( error.Length() == 0 )
													{
														(*this)()->_tls_record_layer_packet.set_TlsContent( tls_finished );
														//(*this)()->_tcp_client.Send( (*this)()->_tls_record_layer_packet , (*this)()->_tls_record_layer_packet , XTlsRecordLayerPacket::NetworkFormat , XTlsRecordLayerPacket::NetworkParse , XTlsRecordLayerPacket::NetworkGetLength , error );
														if( error.Length() == 0 )
														{
															tls_content = (*this)()->_tls_record_layer_packet.get_TlsContent();
															if( rux_is_object( tls_content , XTlsChangeCipherSpec ) )
															{
																(*this)()->_wait_for_encrypted_data = 1;
																IncrementClientSequenceNumber();
																//(*this)()->_tcp_client.Receive( (*this)()->_tls_record_layer_packet , XTlsRecordLayerPacket::NetworkParse , XTlsRecordLayerPacket::NetworkGetLength , error );
																if( error.Length() == 0 )
																{
																	tls_content = (*this)()->_tls_record_layer_packet.get_TlsContent();
																	if( rux_is_object( tls_content , XTlsFinished ) )
																	{
																		IncrementServerSequenceNumber();
																		(*this)()->_is_connected = 1;
																	}
																}																										
															}
														}
													}
												}
											}
										}
									}
								}						
							}
						}
					}
				}
			}
		};
		void XTls::ExportCipherSuiteFields( XEnum_TlsCipherSuite cipher_suite , XEnum_TlsBulkCipherAlgorithm& bulk_cipher_algorithm , XEnum_TlsCipherType& cipher_type , XEnum_TlsMACAlgorithm& mac_algorithm , rux::uint8& key_size , rux::uint8& key_material_size , rux::uint8& hash_size , rux::uint32& iv_size )
		{
			if( cipher_suite == XEnum_CipherSuite_TLS_RSA_WITH_AES_128_CBC_SHA )
			{
				bulk_cipher_algorithm = XEnum_TlsBulkCipherAlgorithm_aes;
				cipher_type = XEnum_CipherType_block;
				mac_algorithm = XEnum_TlsMACAlgorithm_sha;
				key_size = 16;
				key_material_size = 16;
				hash_size = 20;
				iv_size = 16;
			}
		};
		void XTls::IncrementClientSequenceNumber( void )
		{
			for( rux::uint32 index0 = 7 ; index0 >= 0 ; index0-- )
			{
				++( (*this)()->_client_sequence_number[ index0 ] );
				if( (*this)()->_client_sequence_number[ index0 ] != 0)
					break; 
			}
		};
		void XTls::IncrementServerSequenceNumber( void )
		{
			for( rux::uint32 index0 = 7 ; index0 >= 0 ; index0-- )
			{
				++( (*this)()->_server_sequence_number[ index0 ] );
				if( (*this)()->_server_sequence_number[ index0 ] != 0)
					break; 
			}
		};
		//XEnum_TcpClient_ErrorType XTls::Send( const XObject& send_data , XObject& receive_data , NetworkFormatFunction network_format_function , NetworkParseFunction network_parse_function , NetworkGetLengthFunction network_get_length_function , rux::XString& error )
		//{
		//	XEnum_TcpClient_ErrorType error_type = XEnum_TcpClient_ErrorType_CannotConnect;
		//	if( (*this)()->_is_connected == 1 )
		//	{
		//		XTlsApplicationData tls_application_data;
		//		tls_application_data()->_send_data = send_data;
		//		tls_application_data()->_received_data = receive_data;
		//		tls_application_data()->_network_format_function = network_format_function;
		//		tls_application_data()->_network_parse_function = network_parse_function;
		//		tls_application_data()->_network_get_length_function = network_get_length_function;
		//		(*this)()->_tls_record_layer_packet.set_TlsContent( tls_application_data );
		//		error_type = (*this)()->_tcp_client.Send( (*this)()->_tls_record_layer_packet , (*this)()->_tls_record_layer_packet , XTlsRecordLayerPacket::NetworkFormat , 
		//			XTlsRecordLayerPacket::NetworkParse , XTlsRecordLayerPacket::NetworkGetLength , error );
		//		if( error.Length() == 0 )
		//		{
		//			IncrementClientSequenceNumber();
		//			IncrementServerSequenceNumber();
		//		}
		//	}
		//	else
		//		error = "TLS client was not connected";
		//	return error_type;
		//};
		//XEnum_TcpClient_ErrorType XTls::Send( const XObject& send_data , NetworkFormatFunction network_format_function , rux::XString& error )
		//{
		//	XEnum_TcpClient_ErrorType error_type = XEnum_TcpClient_ErrorType_CannotConnect;
		//	if( (*this)()->_is_connected == 1 )
		//	{
		//		XTlsApplicationData tls_application_data;
		//		tls_application_data()->_send_data = send_data;		
		//		tls_application_data()->_network_format_function = network_format_function;		
		//		(*this)()->_tls_record_layer_packet.set_TlsContent( tls_application_data );
		//		error_type = (*this)()->_tcp_client.Send( (*this)()->_tls_record_layer_packet , XTlsRecordLayerPacket::NetworkFormat , error );
		//		if( error.Length() == 0 )		
		//			IncrementClientSequenceNumber();
		//	}
		//	else
		//		error = "TLS client was not connected";
		//	return error_type;
		//};
		//XEnum_TcpClient_ErrorType XTls::Receive( const XObject& receive_data , NetworkParseFunction network_parse_function , NetworkGetLengthFunction network_get_length_function , rux::XString& error )
		//{
		//	XEnum_TcpClient_ErrorType error_type = XEnum_TcpClient_ErrorType_CannotConnect;
		//	if( (*this)()->_is_connected == 1 )
		//	{
		//		XTlsApplicationData tls_application_data;		
		//		tls_application_data()->_received_data = receive_data;		
		//		tls_application_data()->_network_parse_function = network_parse_function;
		//		tls_application_data()->_network_get_length_function = network_get_length_function;
		//		(*this)()->_tls_record_layer_packet.set_TlsContent( tls_application_data );
		//		error_type = (*this)()->_tcp_client.Receive( (*this)()->_tls_record_layer_packet , XTlsRecordLayerPacket::NetworkParse , XTlsRecordLayerPacket::NetworkGetLength , error );
		//		if( error.Length() == 0 )		
		//			IncrementServerSequenceNumber();
		//	}
		//	else
		//		error = "TLS client was not connected";
		//	return error_type;
		//};
	};
};