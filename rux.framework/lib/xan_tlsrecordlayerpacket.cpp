#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_tlsrecordlayerpacket.h>
#include <xan_time.h>
#include <xan_tls.h>
#include <xan_hmac.h>
#include <xan_keyvaluepair.h>
#include <xan_uint64.h>
#include <xan_boolean.h>
begin_implement_rux_class_ns( TlsRecordLayerPacket , rux::network );
	_tls_major_version = 3;
	_tls_minor_version = 1;	
	_tls = NULL;
end_implement_rux_class();
namespace rux
{
	namespace network
	{
		XTlsRecordLayerPacket::XTlsRecordLayerPacket( Tls* tls )
		{	
			(*this)()->_tls = tls;
		};
		void XTlsRecordLayerPacket::set_TlsContent( const XObject& content )
		{
			(*this)()->_tls_content = content;
		};
		Object& XTlsRecordLayerPacket::get_TlsContent( void )
		{
			return (*this)()->_tls_content++;
		};
		void XTlsRecordLayerPacket::NetworkFormat( const XObject& send_data , rux::uint8*& network_buffer_ptr , rux::uint32& network_buffer_size , rux::XString& error )
		{
			if( rux_is_object( send_data , XTlsRecordLayerPacket ) )
			{
				rux::network::XTlsRecordLayerPacket tls_handshake_packet( send_data );
				if( rux_is_object( tls_handshake_packet()->_tls_content , XTlsClientHello ) )
				{
					XTlsClientHello tls_client_hello( tls_handshake_packet()->_tls_content );
					rux::uint32 size = tls_client_hello.get_PacketSize();
					if( network_buffer_size < 5 + size )
					{
						if( network_buffer_ptr )
							rux::engine::free_mem( network_buffer_ptr );
						network_buffer_size = 5 + size;
						network_buffer_ptr = alloc_array_macros( rux::uint8 , network_buffer_size );
					}
					else
						network_buffer_size = 5 + size;
					network_buffer_ptr[ 0 ] = (rux::uint8)XEnum_TlsContentType_handshake;
					network_buffer_ptr[ 1 ] = tls_handshake_packet()->_tls_major_version;
					network_buffer_ptr[ 2 ] = tls_handshake_packet()->_tls_minor_version;
					if( size <= 0xffff )
					{
						rux_from_big_endian_to_little_endian( (rux::uint8*)&size , &network_buffer_ptr[ 3 ] , 2 );
						network_buffer_ptr[ 5 ] = (rux::uint8)XEnum_TlsHandshakeType_client_hello;
						size -= 4;
						if( size <= 0xffffff )
						{
							rux_from_big_endian_to_little_endian( (rux::uint8*)&size , &network_buffer_ptr[ 6 ] , 3 );
							network_buffer_ptr[ 9 ] = tls_handshake_packet()->_tls_major_version;
							network_buffer_ptr[ 10 ] = tls_handshake_packet()->_tls_minor_version;
							rux::uint32 gmt_unix_now = XTime::get_UnixTimeStamp();
							rux_from_big_endian_to_little_endian( (rux::uint8*)&gmt_unix_now , &network_buffer_ptr[ 11 ] , 4 );
							memcpy( tls_client_hello()->_random , &network_buffer_ptr[ 11 ] , 4 );
							memcpy( &network_buffer_ptr[ 15 ] , &tls_client_hello()->_random[ 4 ] , 28 );
							network_buffer_ptr[ 43 ] = tls_client_hello()->_session_id_length;	
							size = 44;
							if( tls_client_hello()->_session_id_length > 0 )
							{
								rux_from_big_endian_to_little_endian( (rux::uint8*)&tls_client_hello()->_session_id , &network_buffer_ptr[ size ] , tls_client_hello()->_session_id_length );						
								size = tls_client_hello()->_session_id_length;
							}
							rux::uint32 index0 = 2 * tls_client_hello()->_cipher_suites.Count();
							rux_from_big_endian_to_little_endian( (rux::uint8*)&index0 , &network_buffer_ptr[ size ] , 2 );					 
							size += 2;
							for( index0 = 0 ; index0 < tls_client_hello()->_cipher_suites.Count() ; index0++ )
							{
								memcpy(  &network_buffer_ptr[ size ] , tls_cipher_suite_values[ tls_client_hello()->_cipher_suites[ index0 ]() ] , 2 );
								size += 2;
							}
							index0 = tls_client_hello()->_compression_methods.Count();
							network_buffer_ptr[ size ] = ((rux::uint8*)&index0)[ 0 ];
							size++;					
							if( index0 > 0 )
							{
								for( index0 = 0 ; index0 < tls_client_hello()->_compression_methods.Count() ; index0++ )
								{
									network_buffer_ptr[ size ] = (rux::uint8)tls_client_hello()->_compression_methods[ index0 ]();
									size++;
								}
							}
							else
							{
								network_buffer_ptr[ size ] = 0x00;
								size++;					
							}
							network_buffer_ptr[ size ] = 0x00;
							size++;
							network_buffer_ptr[ size ] = 0x00;					
							if( tls_handshake_packet()->_tls )
								tls_handshake_packet()->_tls->AppendHandshakeMessage( &network_buffer_ptr[ 5 ] , size - 4 );
						}
						else
							error = "Client Hello Packet internal size greater 0xffffff";
					}
					else
						error = "Client Hello Packet size greater 0xffff";
				}
				else if( rux_is_object( tls_handshake_packet()->_tls_content , XSslv2ClientHello ) )
				{
					XSslv2ClientHello sslv2_client_hello( tls_handshake_packet()->_tls_content );
					rux::uint32 size = sslv2_client_hello.get_PacketSize();
					if( network_buffer_size < 2 + size )
					{
						if( network_buffer_ptr )
							rux::engine::free_mem( network_buffer_ptr );
						network_buffer_size = 2 + size;
						network_buffer_ptr = alloc_array_macros( rux::uint8 , network_buffer_size );
					}
					else
						network_buffer_size = 2 + size;
					network_buffer_ptr[ 0 ] = 0x80;
					network_buffer_ptr[ 1 ] = (rux::uint8)size;
					network_buffer_ptr[ 2 ] = (rux::uint8)XEnum_TlsHandshakeType_client_hello;
					network_buffer_ptr[ 3 ] = tls_handshake_packet()->_tls_major_version;
					network_buffer_ptr[ 4 ] = tls_handshake_packet()->_tls_minor_version;
					rux::uint32 index0 = 3 * sslv2_client_hello()->_cipher_suites.Count();
					rux_from_big_endian_to_little_endian( (rux::uint8*)&index0 , &network_buffer_ptr[ 5 ] , 2 );
					rux_from_big_endian_to_little_endian( (rux::uint8*)&sslv2_client_hello()->_session_id_length , &network_buffer_ptr[ 7 ] , 2 );
					size = 9;
					if( sslv2_client_hello()->_session_id_length > 0 )
					{
						rux_from_big_endian_to_little_endian( (rux::uint8*)&sslv2_client_hello()->_session_id , &network_buffer_ptr[ size ] , sslv2_client_hello()->_session_id_length );				
						size = sslv2_client_hello()->_session_id_length;
					}
					network_buffer_ptr[ size ] = 0x00;
					size++;
					network_buffer_ptr[ size ] = 0x20;
					size++;
					for( index0 = 0 ; index0 < sslv2_client_hello()->_cipher_suites.Count() ; index0++ )
					{
						memcpy( &network_buffer_ptr[ size ] , sslv2_cipher_suite_values[ sslv2_client_hello()->_cipher_suites[ index0 ]() ] , 3 );
						size += 3;
					}
					rux::uint32 gmt_unix_now = XTime::get_UnixTimeStamp();
					rux_from_big_endian_to_little_endian( (rux::uint8*)&gmt_unix_now , &network_buffer_ptr[ size ] , 4 );
					memcpy( sslv2_client_hello()->_random , &network_buffer_ptr[ size ] , 4 );			
					memcpy( &network_buffer_ptr[ size + 4 ] , &sslv2_client_hello()->_random[ 4 ] , 28 );
					if( tls_handshake_packet()->_tls )
						tls_handshake_packet()->_tls->AppendHandshakeMessage( &network_buffer_ptr[ 2 ] , size + 30 );
				}
				else if( rux_is_object( tls_handshake_packet()->_tls_content , XTlsClientKeyExchange ) )
				{
					XTlsClientKeyExchange tls_client_key_exchange( tls_handshake_packet()->_tls_content );
					rux::uint32 size = tls_client_key_exchange.get_PacketSize();
					if( network_buffer_size < 5 + size )
					{
						if( network_buffer_ptr )
							rux::engine::free_mem( network_buffer_ptr );
						network_buffer_size = 5 + size;
						network_buffer_ptr = alloc_array_macros( rux::uint8 , network_buffer_size );
					}
					else
						network_buffer_size = 5 + size;
					network_buffer_ptr[ 0 ] = (rux::uint8)XEnum_TlsContentType_handshake;
					network_buffer_ptr[ 1 ] = tls_handshake_packet()->_tls_major_version;
					network_buffer_ptr[ 2 ] = tls_handshake_packet()->_tls_minor_version;
					if( size <= 0xffff )
					{
						rux_from_big_endian_to_little_endian( (rux::uint8*)&size , &network_buffer_ptr[ 3 ] , 2 );				 
						network_buffer_ptr[ 5 ] = (rux::uint8)XEnum_TlsHandshakeType_client_key_exchange;
						size -= 4;
						if( size <= 0xffffff )
						{
							rux_from_big_endian_to_little_endian( (rux::uint8*)&size , &network_buffer_ptr[ 6 ] , 3 );				 					
							network_buffer_ptr[ 9 ] = 0x02;
							network_buffer_ptr[ 10 ] = 0x00;
							if( size > 0 )					
								memcpy( &network_buffer_ptr[ 11 ] , tls_client_key_exchange()->_encrypted_data_ptr , tls_client_key_exchange()->_encrypted_data_ptr_length );						
							if( tls_handshake_packet()->_tls )
								tls_handshake_packet()->_tls->AppendHandshakeMessage( &network_buffer_ptr[ 5 ] , 6 + tls_client_key_exchange()->_encrypted_data_ptr_length );
						}
						else
							error = "Client Key Exchange Packet internal size greater 0xffffff";
					}
					else
						error = "Client Key Exchange Packet size greater 0xffff";
				}
				else if( rux_is_object( tls_handshake_packet()->_tls_content , XTlsChangeCipherSpec ) )
				{
					XTlsChangeCipherSpec tls_change_cipher_spec( tls_handshake_packet()->_tls_content );			
					if( network_buffer_size < 6 )
					{
						if( network_buffer_ptr )
							rux::engine::free_mem( network_buffer_ptr );
						network_buffer_size = 6;
						network_buffer_ptr = alloc_array_macros( rux::uint8 , network_buffer_size );
					}
					else
						network_buffer_size = 6;
					network_buffer_ptr[ 0 ] = (rux::uint8)XEnum_TlsContentType_change_cipher_spec;
					network_buffer_ptr[ 1 ] = tls_handshake_packet()->_tls_major_version;
					network_buffer_ptr[ 2 ] = tls_handshake_packet()->_tls_minor_version;
					network_buffer_ptr[ 3 ] = 0x00;
					network_buffer_ptr[ 4 ] = 0x01;
					network_buffer_ptr[ 5 ] = 0x01;			
				}
				else if( rux_is_object( tls_handshake_packet()->_tls_content , XTlsFinished ) )
				{
					XTlsFinished tls_finished( tls_handshake_packet()->_tls_content );
					if( tls_handshake_packet()->_tls )
					{
						rux::uint32 cipher_block_size = 0;
						rux::uint8 padding_length = 0;
						rux::uint32 size = tls_finished._data_ptr_length + tls_handshake_packet()->_tls->_hash_size;
						if( tls_handshake_packet()->_tls->_cipher_type == XEnum_CipherType_block )
						{
							size++;
							if( tls_handshake_packet()->_tls->_bulk_cipher_algorithm == XEnum_TlsBulkCipherAlgorithm_aes )
								cipher_block_size = tls_handshake_packet()->_tls->_key_material_size;
							padding_length = static_cast<rux::uint8>( cipher_block_size - ( size - cipher_block_size * ( size / cipher_block_size ) ) );
							size += padding_length;
						}
						rux::uint8 mac[ 20 ] = {0};
						rux::uint32 data_ptr_length = 13 + tls_finished._data_ptr_length;				
						if( network_buffer_size < data_ptr_length )
						{
							if( network_buffer_ptr )
								rux::engine::free_mem( network_buffer_ptr );
							network_buffer_size = data_ptr_length;
							network_buffer_ptr = alloc_array_macros( rux::uint8 , network_buffer_size );
						}				
						memcpy( network_buffer_ptr , tls_handshake_packet()->_tls->_client_sequence_number , 8 );
						network_buffer_ptr[ 8 ] = (rux::uint8)XEnum_TlsContentType_handshake;
						network_buffer_ptr[ 9 ] = tls_handshake_packet()->_tls->_major_version;
						network_buffer_ptr[ 10 ] = tls_handshake_packet()->_tls->_minor_version;				
						rux_from_big_endian_to_little_endian( (rux::uint8*)&tls_finished._data_ptr_length , &network_buffer_ptr[ 11 ] , 2 );
						memcpy( &network_buffer_ptr[ 13 ] , tls_finished()->_data_ptr , tls_finished._data_ptr_length );
						tls_handshake_packet()->_tls->MAC( network_buffer_ptr , data_ptr_length , tls_handshake_packet()->_tls->_connection_end , mac , error );				
						if( network_buffer_size < 5 + size )
						{
							if( network_buffer_ptr )
								rux::engine::free_mem( network_buffer_ptr );
							network_buffer_size = 5 + size;
							network_buffer_ptr = alloc_array_macros( rux::uint8 , network_buffer_size );
						}
						else
							network_buffer_size = 5 + size;
						network_buffer_ptr[ 0 ] = (rux::uint8)XEnum_TlsContentType_handshake;
						network_buffer_ptr[ 1 ] = tls_handshake_packet()->_tls_major_version;
						network_buffer_ptr[ 2 ] = tls_handshake_packet()->_tls_minor_version;
						if( size <= 0xffff )
						{
							rux_from_big_endian_to_little_endian( (rux::uint8*)&size , &network_buffer_ptr[ 3 ] , 2 );
							memcpy( &network_buffer_ptr[ 5 ] , tls_finished()->_data_ptr , tls_finished._data_ptr_length );
							cipher_block_size = 5 + tls_finished._data_ptr_length;
							memcpy( &network_buffer_ptr[ cipher_block_size ] , mac , tls_handshake_packet()->_tls->_hash_size );
							cipher_block_size += tls_handshake_packet()->_tls->_hash_size;
							if( padding_length > 0 )						
								memset( &network_buffer_ptr[ cipher_block_size ] , padding_length , padding_length + 1 );
							cipher_block_size += padding_length - 4;					
							if( rux_is_object( tls_handshake_packet()->_tls->_bulk_cipher_encrypter , rux::cryptography::XAES ) )
							{
								rux::cryptography::XAES aes( tls_handshake_packet()->_tls->_bulk_cipher_encrypter );						
								aes.Encrypt( &network_buffer_ptr[ 5 ] , cipher_block_size , tls_handshake_packet()->_tls->_encrypted_data_ptr , tls_handshake_packet()->_tls->_encrypted_data_ptr_size , tls_handshake_packet()->_tls->_encrypted_data_ptr_length );						
							}
							else
								error = "Bulk cipher encrypter is not implemented";					
							if( tls_handshake_packet()->_tls->_encrypted_data_ptr_length > 0 )				
							{
								memcpy( &network_buffer_ptr[ 5 ] , tls_handshake_packet()->_tls->_encrypted_data_ptr , tls_handshake_packet()->_tls->_encrypted_data_ptr_length );
								tls_handshake_packet()->_tls->AppendHandshakeMessage( tls_finished()->_data_ptr , tls_finished._data_ptr_length );
							}
							else
								error = "Cipher pointer is NULL";
						}
						else
							error = "Tls Finished Packet size greater 0xffff";
					}
					else
						error = "Tls pointer is NULL";
				}
				else if(  rux_is_object( tls_handshake_packet()->_tls_content , XTlsApplicationData ) )
				{
					XTlsApplicationData tls_application_data( tls_handshake_packet()->_tls_content );
					if( tls_handshake_packet()->_tls )
					{
						if( tls_application_data()->_network_format_function )
						{				
							rux::uint32 network_buffer_ptr_length = network_buffer_size;
							tls_application_data()->_network_format_function( tls_application_data()->_send_data , network_buffer_ptr , network_buffer_ptr_length , error );
							if( network_buffer_ptr_length > network_buffer_size )
								network_buffer_size = network_buffer_ptr_length;
							if( error.Length() == 0 )
							{
								rux::uint32 cipher_block_size = 0;
								rux::uint8 padding_length = 0;
								rux::uint32 size = network_buffer_ptr_length + tls_handshake_packet()->_tls->_hash_size;
								if( tls_handshake_packet()->_tls->_cipher_type == XEnum_CipherType_block )
								{
									size++;
									if( tls_handshake_packet()->_tls->_bulk_cipher_algorithm == XEnum_TlsBulkCipherAlgorithm_aes )
										cipher_block_size = tls_handshake_packet()->_tls->_key_material_size;
									padding_length = static_cast<rux::uint8>( cipher_block_size - ( size - cipher_block_size * ( size / cipher_block_size ) ) );
									size += padding_length;
								}
								rux::uint8 mac[ 20 ] = {0};					
								cipher_block_size = network_buffer_ptr_length;
								if( tls_handshake_packet()->_tls->_encrypted_data_ptr_size < network_buffer_ptr_length + 13 )
								{
									if( tls_handshake_packet()->_tls->_encrypted_data_ptr )
										rux::engine::free_mem( tls_handshake_packet()->_tls->_encrypted_data_ptr );								
									tls_handshake_packet()->_tls->_encrypted_data_ptr_size = network_buffer_ptr_length + 13;
									tls_handshake_packet()->_tls->_encrypted_data_ptr = alloc_array_macros( rux::uint8 , tls_handshake_packet()->_tls->_encrypted_data_ptr_size );
									tls_handshake_packet()->_tls->_encrypted_data_ptr_length = tls_handshake_packet()->_tls->_encrypted_data_ptr_size;
								}
								else
									tls_handshake_packet()->_tls->_encrypted_data_ptr_length = network_buffer_ptr_length + 13;
								if( cipher_block_size > 0 )
									memcpy( &tls_handshake_packet()->_tls->_encrypted_data_ptr[ 13 ] , network_buffer_ptr , cipher_block_size );												
								memcpy( tls_handshake_packet()->_tls->_encrypted_data_ptr , tls_handshake_packet()->_tls->_client_sequence_number , 8 );
								tls_handshake_packet()->_tls->_encrypted_data_ptr[ 8 ] = (rux::uint8)XEnum_TlsContentType_application_data;
								tls_handshake_packet()->_tls->_encrypted_data_ptr[ 9 ] = tls_handshake_packet()->_tls->_major_version;
								tls_handshake_packet()->_tls->_encrypted_data_ptr[ 10 ] = tls_handshake_packet()->_tls->_minor_version;				
								rux_from_big_endian_to_little_endian( (rux::uint8*)&cipher_block_size , &tls_handshake_packet()->_tls->_encrypted_data_ptr[ 11 ] , 2 );
								tls_handshake_packet()->_tls->MAC( tls_handshake_packet()->_tls->_encrypted_data_ptr , tls_handshake_packet()->_tls->_encrypted_data_ptr_length , tls_handshake_packet()->_tls->_connection_end , mac , error );				
								if( network_buffer_size < 5 + size )
								{
									if( network_buffer_ptr )
										rux::engine::free_mem( network_buffer_ptr );
									network_buffer_size = 5 + size;
									network_buffer_ptr = alloc_array_macros( rux::uint8 , network_buffer_size );
								}
								else
									network_buffer_size = 5 + size;
								network_buffer_ptr[ 0 ] = (rux::uint8)XEnum_TlsContentType_application_data;
								network_buffer_ptr[ 1 ] = tls_handshake_packet()->_tls_major_version;
								network_buffer_ptr[ 2 ] = tls_handshake_packet()->_tls_minor_version;
								if( size <= 0xffff )
								{
									rux_from_big_endian_to_little_endian( (rux::uint8*)&size , &network_buffer_ptr[ 3 ] , 2 );
									if( cipher_block_size > 0 )
										memcpy( &network_buffer_ptr[ 5 ] , &tls_handshake_packet()->_tls->_encrypted_data_ptr[ 13 ] , cipher_block_size );
									cipher_block_size += 5;
									memcpy( &network_buffer_ptr[ cipher_block_size ] , mac , tls_handshake_packet()->_tls->_hash_size );
									cipher_block_size += tls_handshake_packet()->_tls->_hash_size;
									if( padding_length > 0 )						
										memset( &network_buffer_ptr[ cipher_block_size ] , padding_length , padding_length + 1 );
									cipher_block_size += padding_length - 4;					
									if( rux_is_object( tls_handshake_packet()->_tls->_bulk_cipher_encrypter , rux::cryptography::XAES ) )
									{
										rux::cryptography::XAES aes( tls_handshake_packet()->_tls->_bulk_cipher_encrypter );						
										aes.Encrypt( &network_buffer_ptr[ 5 ] , cipher_block_size , tls_handshake_packet()->_tls->_encrypted_data_ptr , tls_handshake_packet()->_tls->_encrypted_data_ptr_size , tls_handshake_packet()->_tls->_encrypted_data_ptr_length );						
									}
									else
										error = "Bulk cipher encrypter is not implemented";					
									if( tls_handshake_packet()->_tls->_encrypted_data_ptr_length > 0 )									
										memcpy( &network_buffer_ptr[ 5 ] , tls_handshake_packet()->_tls->_encrypted_data_ptr , tls_handshake_packet()->_tls->_encrypted_data_ptr_length );											
									else
										error = "Cipher pointer is NULL";
								}
								else
									error = "Tls Finished Packet size greater 0xffff";
							}
						}
						else
							error = "Tls application data network format function is NULL";
					}
					else
						error = "Tls pointer is NULL";
				}
				else
					error = "Tls handshake type is not implemented";		
			}
			else
				error = "Send data object has not XTlsRecordLayerPacket type";
		};
		rux::uint32 XTlsRecordLayerPacket::NetworkGetLength( rux::uint8* network_buffer_ptr , rux::uint32 network_buffer_size , rux::XString& error )
		{
			rux::uint32 length = 0;
			if( network_buffer_size >= 5 )
			{
				if( network_buffer_ptr )
				{				
					if( network_buffer_ptr[ 0 ] == (rux::uint8)XEnum_TlsContentType_change_cipher_spec
						|| network_buffer_ptr[ 0 ] == (rux::uint8)XEnum_TlsContentType_alert
						|| network_buffer_ptr[ 0 ] == (rux::uint8)XEnum_TlsContentType_handshake
						|| network_buffer_ptr[ 0 ] == (rux::uint8)XEnum_TlsContentType_application_data )
					{
						if( network_buffer_ptr[ 1 ] >= 3 
							&& network_buffer_ptr[ 2 ] >= 1 )
						{
							((rux::uint8*)&length)[ 0 ] = network_buffer_ptr[ 4 ];
							((rux::uint8*)&length)[ 1 ] = network_buffer_ptr[ 3 ];
							length += 5;
						}
						else 
							error = rux::XString::Format( "Tls version is incorrect %ld.%ld" , (rux::int32)network_buffer_ptr[ 1 ] , (rux::int32)network_buffer_ptr[ 2 ] );
					}
					else
						error = rux::XString::Format( "Incorrect Tls Content Type %02lx" , (rux::int32)network_buffer_ptr[ 0 ] );
				}
				else
					error = "Receive network buffer pointer is NULL";
			}
			else
				error = "Receive network buffer size is less 5";
			return length;
		};
		rux::uint32 XTlsRecordLayerPacket::NetworkParse( const XObject& receive_data , rux::uint8* network_buffer_ptr , rux::uint32 network_buffer_size , rux::XString& error )
		{
			rux::uint32 readen_bytes = 0;
			if( rux_is_object( receive_data , XTlsRecordLayerPacket ) )
			{
				XTlsRecordLayerPacket tls_handshake_packet( receive_data );		
				if( network_buffer_size >= 5 )
				{
					if( network_buffer_ptr )
					{
						if( network_buffer_ptr[ 1 ] >= 3 
								&& network_buffer_ptr[ 2 ] >= 1 )
							{
								rux::uint16 length = 0;
								rux_from_big_endian_to_little_endian( &network_buffer_ptr[ 3 ] , (rux::uint8*)&length , 2 );						
								if( length > 0 )
								{
									if( length + 5 <= static_cast<rux::int32>( network_buffer_size ) )
									{								
										if( network_buffer_ptr[ 0 ] == (rux::uint8)XEnum_TlsContentType_change_cipher_spec )
										{
											XTlsChangeCipherSpec tls_change_cipher_spec;
											if( tls_handshake_packet()->_tls )
											{
												if( tls_handshake_packet()->_tls->_major_version == network_buffer_ptr[ 1 ] )
												{
													if( tls_handshake_packet()->_tls->_minor_version == network_buffer_ptr[ 2 ] )
													{
														length = 0;
														rux_from_big_endian_to_little_endian( &network_buffer_ptr[ 3 ] , (rux::uint8*)&length , 2 );
														if( length == 1 )
														{
															if( network_buffer_ptr[ 5 ] == 1 )
															{
																tls_handshake_packet()->_tls_content = tls_change_cipher_spec;
																readen_bytes = 6;
															}
															else
																error = rux::XString::Format( "Tls Change Cipher Spec packet has incorrect content '%ld'" , (rux::int32)network_buffer_ptr[ 5 ] );
														}
														else
															error = rux::XString::Format( "Tls Change Cipher Spec packet has incorrect length '%ld'" , (rux::int32)length );
													}
													else
														error = rux::XString::Format( "Tls Change Cipher Spec packet has incorrect TLS minor version '%ld'" , network_buffer_ptr[ 2 ] );
												}
												else
													error = rux::XString::Format( "Tls Change Cipher Spec packet has incorrect TLS major version '%ld'" , network_buffer_ptr[ 1 ] );
											}
											else
												error = "Tls Change Cipher Spec packet , tls pointer is NULL";
										}
										else if( network_buffer_ptr[ 0 ] == (rux::uint8)XEnum_TlsContentType_alert )
										{
											error = rux::XString::Format( "Tls alert is not implemented yet" );
										}
										else if( network_buffer_ptr[ 0 ] == (rux::uint8)XEnum_TlsContentType_handshake )
										{								
											if( tls_handshake_packet()->_tls->_wait_for_encrypted_data == 1 )
											{
												tls_handshake_packet()->_tls->_decrypted_data_ptr_length = 0;
												if( rux_is_object( tls_handshake_packet()->_tls->_bulk_cipher_decrypter , rux::cryptography::XAES ) )
												{
													rux::cryptography::XAES decrypter( tls_handshake_packet()->_tls->_bulk_cipher_decrypter );
													decrypter.Decrypt( &network_buffer_ptr[ 5 ] , length , tls_handshake_packet()->_tls->_decrypted_data_ptr , tls_handshake_packet()->_tls->_decrypted_data_ptr_size , tls_handshake_packet()->_tls->_decrypted_data_ptr_length );											
												}
												else
													error = rux::XString::Format( "Tls Handshake Encypted Data packet , %s is not implemented as encrypter class" , tls_handshake_packet()->_tls->_bulk_cipher_decrypter.get_Type().get_TypeName() );
												if( error.Length() == 0 )
												{
													if( tls_handshake_packet()->_tls->_decrypted_data_ptr_length > 0 )
													{
														if( tls_handshake_packet()->_tls->_decrypted_data_ptr[ 0 ] == (rux::uint8)XEnum_TlsHandshakeType_finished )
														{
															rux::uint8 mac[ 20 ] = {0};																											
															XTlsFinished tls_finished;							
															rux::uint32 data_ptr_length = 13 + tls_finished._data_ptr_length;													
															if( tls_handshake_packet()->_tls->_pre_mac_data_ptr_size < data_ptr_length )
															{
																if( tls_handshake_packet()->_tls->_pre_mac_data_ptr )
																	rux::engine::free_mem( tls_handshake_packet()->_tls->_pre_mac_data_ptr );
																tls_handshake_packet()->_tls->_pre_mac_data_ptr_size = data_ptr_length;
																tls_handshake_packet()->_tls->_pre_mac_data_ptr = alloc_array_macros( rux::uint8 , tls_handshake_packet()->_tls->_pre_mac_data_ptr_size );
															}
															else
																tls_handshake_packet()->_tls->_pre_mac_data_ptr_size = data_ptr_length;
															tls_handshake_packet()->_tls->_pre_mac_data_ptr[ 8 ] = (rux::uint8)XEnum_TlsContentType_handshake;
															tls_handshake_packet()->_tls->_pre_mac_data_ptr[ 9 ] = tls_handshake_packet()->_tls->_major_version;
															tls_handshake_packet()->_tls->_pre_mac_data_ptr[ 10 ] = tls_handshake_packet()->_tls->_minor_version;				
															rux_from_big_endian_to_little_endian( (rux::uint8*)&tls_finished._data_ptr_length , &tls_handshake_packet()->_tls->_pre_mac_data_ptr[ 11 ] , 2 );													
															if( tls_handshake_packet()->_tls->_connection_end == XEnum_TlsConnectionEnd_client )						
															{
																tls_finished.set_VerifyData( tls_handshake_packet()->_tls , XEnum_TlsConnectionEnd_server , error );
																memcpy( &tls_handshake_packet()->_tls->_pre_mac_data_ptr[ 13 ] , tls_finished()->_data_ptr , tls_finished._data_ptr_length );
																if( error.Length() == 0 )
																{
																	memcpy( tls_handshake_packet()->_tls->_pre_mac_data_ptr , tls_handshake_packet()->_tls->_server_sequence_number , 8 );
																	tls_handshake_packet()->_tls->MAC( tls_handshake_packet()->_tls->_pre_mac_data_ptr , data_ptr_length , XEnum_TlsConnectionEnd_server , mac , error );
																}
															}
															else if( tls_handshake_packet()->_tls->_connection_end == XEnum_TlsConnectionEnd_server )													
															{
																tls_finished.set_VerifyData( tls_handshake_packet()->_tls , XEnum_TlsConnectionEnd_client , error );
																memcpy( &tls_handshake_packet()->_tls->_pre_mac_data_ptr[ 13 ] , tls_finished()->_data_ptr , tls_finished._data_ptr_length );
																if( error.Length() == 0 )
																{
																	memcpy( tls_handshake_packet()->_tls->_pre_mac_data_ptr , tls_handshake_packet()->_tls->_client_sequence_number , 8 );
																	tls_handshake_packet()->_tls->MAC( tls_handshake_packet()->_tls->_pre_mac_data_ptr , data_ptr_length , XEnum_TlsConnectionEnd_client , mac , error );
																}
															}
															if( error.Length() == 0 )
															{												
																if( memcmp( tls_handshake_packet()->_tls->_decrypted_data_ptr , tls_finished()->_data_ptr , tls_finished._data_ptr_length ) == 0 )
																{
																	if( memcmp( &tls_handshake_packet()->_tls->_decrypted_data_ptr[ tls_finished._data_ptr_length ] , mac , tls_handshake_packet()->_tls->_hash_size ) == 0 )
																	{
																		if( tls_handshake_packet()->_tls->_cipher_type == XEnum_CipherType_block )
																		{
																			readen_bytes += tls_finished._data_ptr_length + tls_handshake_packet()->_tls->_hash_size;
																			mac[ 0 ] = static_cast<rux::uint8>( tls_handshake_packet()->_tls->_decrypted_data_ptr_length - readen_bytes - 1 );
																			for( data_ptr_length = readen_bytes ; data_ptr_length < tls_handshake_packet()->_tls->_decrypted_data_ptr_length ; data_ptr_length++ )
																			{
																				if( tls_handshake_packet()->_tls->_decrypted_data_ptr[ data_ptr_length ] != mac[ 0 ] )
																				{
																					error = "Tls Handshake Encrypted Data packet , incorrect cipher block padding";
																					break;
																				}
																			}
																		}
																		if( error.Length() == 0 )
																		{
																			readen_bytes = length + 5;
																			tls_handshake_packet()->_tls_content = tls_finished;
																			tls_handshake_packet()->_tls->AppendHandshakeMessage( tls_finished()->_data_ptr , tls_finished._data_ptr_length );
																		}
																	}
																	else
																		error = "Tls Handshake Encrypted Data packet , incorrect MAC";
																}
																else
																	error = "Tls Handshake Encrypted Data packet , incorrect Tls Handshake Finished packet verify data";														
															}													
														}
														else
															error = rux::XString::Format( "Tls Handshake Encrypted Data packet , incorrect Tls Handshake Type 0x%02lx" , (rux::int32)tls_handshake_packet()->_tls->_decrypted_data_ptr[ 0 ] );				
													}
													else
														error = "Tls Handshake Encrypted Data packet , decrypted data pointer length is equal 0";
												}
											}
											else
											{
												if( network_buffer_ptr[ 5 ] == (rux::uint8)XEnum_TlsHandshakeType_server_hello )
												{											
													if( length >= 40 )
													{
														rux::uint32 server_hello_length = 0;
														rux_from_big_endian_to_little_endian( &network_buffer_ptr[ 6 ] , (rux::uint8*)&server_hello_length , 3 );											
														if( server_hello_length + 4 == length )
														{
															XTlsServerHello tls_server_hello;
															tls_server_hello()->_tls_major_version = network_buffer_ptr[ 9 ];
															tls_server_hello()->_tls_minor_version = network_buffer_ptr[ 10 ];
															rux_from_big_endian_to_little_endian( &network_buffer_ptr[ 11 ] , (rux::uint8*)&tls_server_hello()->_gmt_unix_timestamp , 4 );
															memcpy( tls_server_hello()->_random , &network_buffer_ptr[ 11 ] , 32 );												
															tls_server_hello()->_session_id_length = network_buffer_ptr[ 43 ];
															if( tls_server_hello()->_session_id_length <= 32 
																&& server_hello_length - 35 >= tls_server_hello()->_session_id_length )
															{
																if( tls_server_hello()->_session_id_length > 0 )
																	memcpy( (rux::uint8*)&tls_server_hello()->_session_id , &network_buffer_ptr[ 44 ] , tls_server_hello()->_session_id_length );														
																readen_bytes = 44 + tls_server_hello()->_session_id_length;
																if( readen_bytes <= network_buffer_size )
																{
																	length = *(rux::uint16*)&network_buffer_ptr[ readen_bytes ];
																	rux::uint32 index0 = 0;
																	for( ; index0 < sizeof( tls_cipher_suite_values ) / sizeof( tls_cipher_suite_values[ 0 ] ) ; index0++ )
																	{
																		if( memcmp( &length , tls_cipher_suite_values[ index0 ] , sizeof( rux::uint16 ) ) == 0 )
																			break;
																	}
																	if( index0 < sizeof( tls_cipher_suite_values ) / sizeof( tls_cipher_suite_values[ 0 ] ) )
																	{
																		tls_server_hello()->_cipher_suite = (XEnum_TlsCipherSuite)index0;
																		readen_bytes += sizeof( rux::uint16 );
																		if( readen_bytes < server_hello_length + 9 )
																		{
																			tls_server_hello()->_compression_method = network_buffer_ptr[ readen_bytes ];
																			readen_bytes++;
																			if( readen_bytes == server_hello_length + 9 )						
																			{
																				tls_handshake_packet()->_tls_content = tls_server_hello;
																				if( tls_handshake_packet()->_tls )
																					tls_handshake_packet()->_tls->AppendHandshakeMessage( &network_buffer_ptr[ 5 ] , readen_bytes - 5 );
																			}
																			else
																				error = "Tls ServerHello extensions parsing is not implemented";
																		}
																		else
																			error = "Tls ServerHello packet has not Compression Method";
																	}
																	else
																		error = "Tls ServerHello has unknown cipher suite";
																}
																else
																	error = "Tls ServerHello packet has not Cipher Suite";
															}
															else
																error = "Tls ServerHello Session Id length is incorrect";
														}
														else
															error = "Tls ServerHello length is incorrect";
													}
													else
														error = "Tls Record Layer packet length is less minimum size ( 40 bytes )";
												}
												else if( network_buffer_ptr[ 5 ] == (rux::uint8)XEnum_TlsHandshakeType_hello_request )
												{
													error = rux::XString::Format( "Tls Handshake Type hello_request is not implemented yet" );
												}
												else if( network_buffer_ptr[ 5 ] == (rux::uint8)XEnum_TlsHandshakeType_client_hello )
												{
													error = rux::XString::Format( "Tls Handshake Type client_hello is not implemented yet" );
												}									
												else if( network_buffer_ptr[ 5 ] == (rux::uint8)XEnum_TlsHandshakeType_certificate )
												{
													if( length >= 3 )
													{
														rux::uint32 certificate_length = 0;
														rux_from_big_endian_to_little_endian( &network_buffer_ptr[ 6 ] , (rux::uint8*)&certificate_length , 3 );											
														if( certificate_length + 4 == length )
														{
															XTlsCertificate tls_certificate;
															rux::uint32 certificate_offset = 0;
															rux::uint32 x509v3_length = 0;
															readen_bytes = 9;
															rux::cryptography::XCertificateX509v3 certificate_x509v3;
															do
															{
																if( readen_bytes + 2 < certificate_length + 9 )
																{
																	rux_from_big_endian_to_little_endian( &network_buffer_ptr[ readen_bytes ] , (rux::uint8*)&certificate_offset , 3 );														
																	readen_bytes += 3;
																	if( readen_bytes + certificate_offset <= certificate_length + 9 )
																	{
																		x509v3_length = 0;
																		do
																		{
																			if( certificate_offset >= 3 )
																			{
																				if( readen_bytes + 2 < certificate_length + 9 )
																				{
																					rux_from_big_endian_to_little_endian( &network_buffer_ptr[ readen_bytes ] , (rux::uint8*)&x509v3_length , 3 );
																					readen_bytes += 3;
																					certificate_offset -= 3;
																					if( x509v3_length <= certificate_offset )
																					{
																						certificate_offset -= x509v3_length;
																						certificate_x509v3 = rux::cryptography::XCertificateX509v3::Parse( (rux::uint8*)&network_buffer_ptr[ readen_bytes ] , x509v3_length , error );
																						if( error.Length() == 0 )
																						{
																							tls_certificate()->_certificates.Add( certificate_x509v3 );
																							readen_bytes += x509v3_length;
																						}
																						else
																							break;
																					}
																					else
																					{
																						error = "Tls x.509v3 Certificate length is incorrect";
																						break;
																					}
																				}
																				else
																				{
																					error = "Tls Certificate length is incorrect";
																					break;
																				}
																			}
																			else 
																			{
																				error = "Tls Certificate length is incorrect";
																				break;
																			}
																		}
																		while( certificate_offset > 0 );
																		if( error.Length() > 0 )
																			break;
																	}
																	else
																	{
																		error = "Tls Certificate length is incorrect";
																		break;
																	}
																}
																else
																{
																	error = "Tls Certificate length is incorrect";
																	break;
																}
															}
															while( readen_bytes < certificate_length + 9 );
															if( error.Length() == 0 )
															{
																if( readen_bytes == certificate_length + 9 )								
																{
																	tls_handshake_packet()->_tls_content = tls_certificate;
																	if( tls_handshake_packet()->_tls )
																		tls_handshake_packet()->_tls->AppendHandshakeMessage( &network_buffer_ptr[ 5 ] , readen_bytes - 5 );
																}
																else
																	error = "Tls Certificate buffer is incorrect";
															}
														}
														else
															error = "Tls Certificate length is incorrect";
													}
													else
														error = "Tls Record Layer packet length is less minimum size ( 40 bytes )";
												}
												else if( network_buffer_ptr[ 5 ] == (rux::uint8)XEnum_TlsHandshakeType_server_key_exchange )
												{
													error = rux::XString::Format( "Tls Handshake Type server_key_exchange is not implemented yet" );
												}
												else if( network_buffer_ptr[ 5 ] == (rux::uint8)XEnum_TlsHandshakeType_certificate_request )
												{
													error = rux::XString::Format( "Tls Handshake Type certificate_request is not implemented yet" );
												}
												else if( network_buffer_ptr[ 5 ] == (rux::uint8)XEnum_TlsHandshakeType_server_hello_done )
												{										
													if( length == 4 )
													{
														XTlsServerHelloDone server_hello_done;
														rux_from_big_endian_to_little_endian( &network_buffer_ptr[ 6 ] , (rux::uint8*)&server_hello_done()->_length , 3 );											
														if( server_hello_done()->_length == 0 )
														{
															tls_handshake_packet()->_tls_content = server_hello_done;
															readen_bytes = 9;
															if( tls_handshake_packet()->_tls )
																tls_handshake_packet()->_tls->AppendHandshakeMessage( &network_buffer_ptr[ 5 ] , 4 );
														}
														else
															error = "Tls Handshake Type server_hello_done has incorrect length";
													}
													else
														error = "Tls Handshake Type server_hello_done has incorrect length";
												}
												else if( network_buffer_ptr[ 5 ] == (rux::uint8)XEnum_TlsHandshakeType_certificate_verify )
												{
													error = rux::XString::Format( "Tls Handshake Type certificate_verify is not implemented yet" );
												}
												else if( network_buffer_ptr[ 5 ] == (rux::uint8)XEnum_TlsHandshakeType_client_key_exchange )
												{
													error = rux::XString::Format( "Tls Handshake Type client_key_exchange is not implemented yet" );
												}
												else if( network_buffer_ptr[ 5 ] == (rux::uint8)XEnum_TlsHandshakeType_finished )
												{
													error = rux::XString::Format( "Tls Handshake Type finished is not implemented yet" );
												}
												else
													error = rux::XString::Format( "Incorrect Tls Handshake Type 0x%02lx" , (rux::int32)network_buffer_ptr[ 5 ] );				
											}
										}
										else if( network_buffer_ptr[ 0 ] == (rux::uint8)XEnum_TlsContentType_application_data )
										{
											if( tls_handshake_packet()->_tls->_wait_for_encrypted_data == 1 )
											{
												tls_handshake_packet()->_tls->_decrypted_data_ptr_length = 0;
												if( rux_is_object( tls_handshake_packet()->_tls->_bulk_cipher_decrypter , rux::cryptography::XAES ) )
												{
													rux::cryptography::XAES decrypter( tls_handshake_packet()->_tls->_bulk_cipher_decrypter );
													decrypter.Decrypt( &network_buffer_ptr[ 5 ] , length , tls_handshake_packet()->_tls->_decrypted_data_ptr , tls_handshake_packet()->_tls->_decrypted_data_ptr_size , tls_handshake_packet()->_tls->_decrypted_data_ptr_length );											
												}
												else
													error = rux::XString::Format( "Tls Application Encypted Data packet , %s is not implemented as encrypter class" , tls_handshake_packet()->_tls->_bulk_cipher_decrypter.get_Type().get_TypeName() );
												if( error.Length() == 0 )
												{
													if( tls_handshake_packet()->_tls->_decrypted_data_ptr_length > 0 )
													{
														rux::uint8 mac[ 20 ] = {0};							
														rux::uint8 padding_length = 0;
														if( tls_handshake_packet()->_tls->_cipher_type == XEnum_CipherType_block )
														{
															padding_length = tls_handshake_packet()->_tls->_decrypted_data_ptr[ tls_handshake_packet()->_tls->_decrypted_data_ptr_length - 1 ];												
															mac[ 0 ] = 1;
															while( mac[ 0 ] < padding_length )
															{
																if( tls_handshake_packet()->_tls->_decrypted_data_ptr[ tls_handshake_packet()->_tls->_decrypted_data_ptr_length - 1 - mac[ 0 ] ] != padding_length )
																{
																	error = "Tls Application Encrypted Data packet , incorrect cipher block padding";
																	break;
																}
																if( tls_handshake_packet()->_tls->_decrypted_data_ptr_length - 1 - mac[ 0 ] == 0 )
																{
																	error = "Tls Application Encrypted Data packet , incorrect cipher block padding";
																	break;
																}
																mac[ 0 ]++;
															}
														}
														if( error.Length() == 0 )
														{
															rux::uint32 data_length = tls_handshake_packet()->_tls->_decrypted_data_ptr_length - padding_length - 1 - tls_handshake_packet()->_tls->_hash_size;													
															if( tls_handshake_packet()->_tls->_pre_mac_data_ptr_size < data_length + 13 )
															{
																if( tls_handshake_packet()->_tls->_pre_mac_data_ptr )
																	rux::engine::free_mem( tls_handshake_packet()->_tls->_pre_mac_data_ptr );
																tls_handshake_packet()->_tls->_pre_mac_data_ptr_size = data_length + 13;
																tls_handshake_packet()->_tls->_pre_mac_data_ptr = alloc_array_macros( rux::uint8 , tls_handshake_packet()->_tls->_pre_mac_data_ptr_size );
															}
															tls_handshake_packet()->_tls->_pre_mac_data_ptr_length = data_length + 13;
															tls_handshake_packet()->_tls->_pre_mac_data_ptr[ 8 ] = (rux::uint8)XEnum_TlsContentType_application_data;
															tls_handshake_packet()->_tls->_pre_mac_data_ptr[ 9 ] = tls_handshake_packet()->_tls->_major_version;
															tls_handshake_packet()->_tls->_pre_mac_data_ptr[ 10 ] = tls_handshake_packet()->_tls->_minor_version;				
															rux_from_big_endian_to_little_endian( (rux::uint8*)&data_length , &tls_handshake_packet()->_tls->_pre_mac_data_ptr[ 11 ] , 2 );													
															if( tls_handshake_packet()->_tls->_connection_end == XEnum_TlsConnectionEnd_client )						
															{														
																memcpy( &tls_handshake_packet()->_tls->_pre_mac_data_ptr[ 13 ] , tls_handshake_packet()->_tls->_decrypted_data_ptr , data_length );
																if( error.Length() == 0 )
																{
																	memcpy( tls_handshake_packet()->_tls->_pre_mac_data_ptr , tls_handshake_packet()->_tls->_server_sequence_number , 8 );
																	tls_handshake_packet()->_tls->MAC( tls_handshake_packet()->_tls->_pre_mac_data_ptr , tls_handshake_packet()->_tls->_pre_mac_data_ptr_length , XEnum_TlsConnectionEnd_server , mac , error );
																}
															}
															else if( tls_handshake_packet()->_tls->_connection_end == XEnum_TlsConnectionEnd_server )													
															{
																memcpy( &tls_handshake_packet()->_tls->_pre_mac_data_ptr[ 13 ] , tls_handshake_packet()->_tls->_decrypted_data_ptr , data_length );
																if( error.Length() == 0 )
																{
																	memcpy( tls_handshake_packet()->_tls->_pre_mac_data_ptr , tls_handshake_packet()->_tls->_client_sequence_number , 8 );
																	tls_handshake_packet()->_tls->MAC( tls_handshake_packet()->_tls->_pre_mac_data_ptr , tls_handshake_packet()->_tls->_pre_mac_data_ptr_length , XEnum_TlsConnectionEnd_client , mac , error );
																}
															}
															if( error.Length() == 0 )
															{	
																if( memcmp( &tls_handshake_packet()->_tls->_decrypted_data_ptr[ data_length ] , mac , tls_handshake_packet()->_tls->_hash_size ) == 0 )
																{
																	readen_bytes = length + 5;
																	if( rux_is_object( tls_handshake_packet()->_tls_content , XTlsApplicationData ) )
																	{
																		XTlsApplicationData tls_application_data( tls_handshake_packet()->_tls_content );
																		if( tls_application_data()->_network_parse_function )
																		{																	
																			tls_application_data()->_network_parse_function( tls_application_data()->_received_data , tls_handshake_packet()->_tls->_decrypted_data_ptr , data_length , error );
																		}
																		else
																			error = "Tls Application Data network parse function is NULL";
																	}
																	else
																		error = "Tls Application Encrypted Data packet , TLS content type is not XTlsApplicationData";																													
																}
																else
																	error = "Tls Application Encrypted Data packet , incorrect MAC";														
															}				
														}
													}
													else
														error = "Tls Application Encrypted Data packet , decrypted data pointer length is equal 0";
												}
											}
											else
												error = "Tls Application Encrypted Data packet , TLS object is not ready for encrypted data";
										}
										else
											error = rux::XString::Format( "Incorrect Tls Content Type %02lx" , (rux::int32)network_buffer_ptr[ 0 ] );								
									}
									else
										error = "Tls Record Layer packet length is higher receive buffer size";
								}
								else
									error = "Tls Record Layer packet length is equal 0";
							}
							else 
								error = rux::XString::Format( "Tls version is incorrect %ld.%ld" , (rux::int32)network_buffer_ptr[ 1 ] , (rux::int32)network_buffer_ptr[ 2 ] );				
					}
					else
						error = "Receive network buffer pointer is NULL";
				}
				else
					error = "Receive network buffer size is less 5";
			}
			return readen_bytes;
		};
	};
};