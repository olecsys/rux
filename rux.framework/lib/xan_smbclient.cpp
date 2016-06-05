#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_smbclient.h>
#include <xan_error.h>
#include <xan_keyvaluepair.h>
#include <xan_asn_defines.h>
#include <xan_network_information.h>
#include <xan_des.h>
#include <xan_md4.h>
#include <xan_md5.h>
#include <xan_io_defines.h>
#include <xan_path.h>
#include <xan_uint64.h>
#include <xan_boolean.h>
begin_implement_rux_class_ns( SmbClient , rux::network );
	_is_logon = 0;
end_implement_rux_class();
namespace rux
{
	namespace network
	{			
		SmbClient::~SmbClient()
		{
			Logoff();
		};
		rux::uint8 SmbClient::Logon( const rux::XString& username , const rux::XString& password , const rux::XString& hostname_or_ip , XString& error )
		{
			rux::uint8 res = 0;
			error.Clear();
			rux::network::XAddress address( hostname_or_ip , 445 , error );
			rux::network::XAddress current_address( _tcp_client.get_NetworkAddress() );			
			if( address.get_Address() != current_address.get_Address()
				|| address.get_Port() != current_address.get_Port() )
			{
				Logoff();
				_tcp_client.Close();
				_tcp_client.set_NetworkAddress( address );
			}
			if( _is_logon == 0 )
			{				
				rux::network::smb_com_negotiate_request smb_com_negotiate_request;
				_tcp_client.Send( (char*)&smb_com_negotiate_request , sizeof( smb_com_negotiate_request ) , error );
				if( error.Length() == 0 )
				{
					XData* recv_data = NULL;
					_tcp_client.Receive( &recv_data , error );
					if( recv_data )
					{
						size_t data_size = recv_data->_data_size;
						if( data_size >= sizeof( rux::network::smb_packet_header ) )
						{
							rux::network::smb_com_negotiate_response_extended* smb_com_negotiate_response = (rux::network::smb_com_negotiate_response_extended*)recv_data->Read( data_size );
							if( smb_com_negotiate_response->_smb_packet_header._Status == 0 )
							{
								if( data_size >= sizeof( rux::network::smb_com_negotiate_response_extended ) )
								{
									if( smb_com_negotiate_response->_DialectIndex != 0xffff )
									{
										if( smb_com_negotiate_request._smb_packet_header._PIDHigh == smb_com_negotiate_response->_smb_packet_header._PIDHigh
											&& smb_com_negotiate_request._smb_packet_header._PIDLow == smb_com_negotiate_response->_smb_packet_header._PIDLow )
										{
											rux::uint8 is_NEGOTIATE_USER_SECURITY = 0;
											if( smb_com_negotiate_response->_SecurityMode & rux::network::NEGOTIATE_USER_SECURITY )
												is_NEGOTIATE_USER_SECURITY = 1;
											if( smb_com_negotiate_response->_SecurityMode & rux::network::NEGOTIATE_ENCRYPT_PASSWORDS )
											{
											}
											if( smb_com_negotiate_response->_SecurityMode & rux::network::NEGOTIATE_SECURITY_SIGNATURES_ENABLED )
											{
											}
											if( smb_com_negotiate_response->_SecurityMode & rux::network::NEGOTIATE_SECURITY_SIGNATURES_REQUIRED )
											{
											}
											if( smb_com_negotiate_response->_Capabilities & rux::network::CAP_RAW_MODE )
											{
											}
											if( smb_com_negotiate_response->_Capabilities & rux::network::CAP_MPX_MODE )
											{
											}
											if( smb_com_negotiate_response->_Capabilities & rux::network::CAP_UNICODE  )
											{
											}
											if( smb_com_negotiate_response->_Capabilities & rux::network::CAP_LARGE_FILES )
											{
											}
											if( smb_com_negotiate_response->_Capabilities & rux::network::CAP_NT_SMBS )
											{
											}
											if( smb_com_negotiate_response->_Capabilities & rux::network::CAP_RPC_REMOTE_APIS )
											{
											}
											if( smb_com_negotiate_response->_Capabilities & rux::network::CAP_STATUS32 )
											{
											}
											if( smb_com_negotiate_response->_Capabilities & rux::network::CAP_LEVEL_II_OPLOCKS )
											{
											}
											if( smb_com_negotiate_response->_Capabilities & rux::network::CAP_LOCK_AND_READ )
											{
											}
											if( smb_com_negotiate_response->_Capabilities & rux::network::CAP_NT_FIND )
											{
											}
											if( smb_com_negotiate_response->_Capabilities & rux::network::CAP_BULK_TRANSFER )
											{
											}
											if( smb_com_negotiate_response->_Capabilities & rux::network::CAP_COMPRESSED_DATA )
											{
											}
											if( smb_com_negotiate_response->_Capabilities & rux::network::CAP_DFS )
											{
											}
											if( smb_com_negotiate_response->_Capabilities & rux::network::CAP_QUADWORD_ALIGNED )
											{
											}
											if( smb_com_negotiate_response->_Capabilities & rux::network::CAP_LARGE_READX )
											{
											}
											if( smb_com_negotiate_response->_Capabilities & rux::network::CAP_INFOLEVEL_PASSTHRU )
											{
											}
											if( smb_com_negotiate_response->_Capabilities & rux::network::CAP_LARGE_WRITEX )
											{
											}
											if( smb_com_negotiate_response->_Capabilities & rux::network::CAP_LWIO )
											{
											}
											if( smb_com_negotiate_response->_Capabilities & rux::network::CAP_UNIX )
											{
											}
											if( smb_com_negotiate_response->_Capabilities & rux::network::CAP_DYNAMIC_REAUTH )
											{
											}
											if( smb_com_negotiate_response->_Capabilities & rux::network::CAP_PERSISTENT_HANDLES )
											{
											}
											if( smb_com_negotiate_response->_Capabilities & rux::network::CAP_EXTENDED_SECURITY )
											{
											}
											if( is_NEGOTIATE_USER_SECURITY == 1 )
											{
												rux::network::smb_com_session_setup_andx_request smb_com_session_setup_andx_request( smb_com_negotiate_response );		
												if( smb_com_negotiate_response->_Capabilities & rux::network::CAP_LARGE_READX )
													_MaxBufferSize = smb_com_negotiate_response->_MaxBufferSize;
												else if( smb_com_negotiate_response->_Capabilities & rux::network::CAP_RAW_MODE )
													_MaxBufferSize = smb_com_negotiate_response->_MaxRawSize;
												rux::uint8 is_NTLMSSP_NEGOTIATE_EXTENDED_SESSIONSECURITY = 0;
												smb_com_session_setup_andx_request._Bytes_size = rux::network::ntlm::generate_negotiate_message( smb_com_session_setup_andx_request._Bytes , sizeof( smb_com_session_setup_andx_request._Bytes) , is_NTLMSSP_NEGOTIATE_EXTENDED_SESSIONSECURITY );
												smb_com_session_setup_andx_request.refresh();
												size_t data_size = 0;
												XData send_data;
												for( ; ; )
												{									
													void* data = smb_com_session_setup_andx_request.next_to_send( data_size );
													if( data && data_size > 0 )
														send_data.Concatenate( data , data_size );
													else
														break;
												}					
												data_size = send_data._data_size;
												_tcp_client.Send( (char*)send_data.Read( data_size ) , data_size , error );
												if( error.Length() == 0 )
												{
													recv_data = NULL;
													_tcp_client.Receive( &recv_data , error );
													if( recv_data )
													{
														data_size = recv_data->_data_size;
														if( data_size >= sizeof( rux::network::smb_packet_header ) )
														{
															rux::network::smb_com_session_setup_andx_response* smb_com_session_setup_andx_response = (rux::network::smb_com_session_setup_andx_response*)recv_data->Read( data_size );
															if( smb_com_negotiate_request._smb_packet_header._PIDHigh == smb_com_session_setup_andx_response->_smb_packet_header._PIDHigh
																&& smb_com_negotiate_request._smb_packet_header._PIDLow == smb_com_session_setup_andx_response->_smb_packet_header._PIDLow )
															{																
																if( smb_com_session_setup_andx_response->_smb_packet_header._Status != 0 )
																{
																	if( smb_com_session_setup_andx_response->_smb_packet_header._Status == rux::network::STATUS_MORE_PROCESSING_REQUIRED )
																	{																
																		if( sizeof( rux::network::smb_com_session_setup_andx_response ) + smb_com_session_setup_andx_response->_ByteCount == data_size )
																		{																
																			rux::network::AsnPacket asn_packet;
																			size_t asn_size = rux::network::AsnPacket::parse( asn_packet , &( (rux::uint8*)smb_com_session_setup_andx_response )[ sizeof( rux::network::smb_com_session_setup_andx_response ) ] , smb_com_session_setup_andx_response->_SecurityBlobLength , error );
																			if( asn_size == smb_com_session_setup_andx_response->_SecurityBlobLength )
																			{
																				if( asn_packet._asn_class == XEnum_AsnClass_ContextSpecific
																					&& asn_packet._asn_universal_class_tags == 1
																					&& asn_packet._is_constructed == 1 )
																				{
																					XMallocArray< rux::network::AsnPacket* > asn_packets0;
																					rux::network::AsnPacket::parse( asn_packet , asn_packets0 , error );
																					if( error.Length() == 0 )
																					{
																						if( asn_packets0.Count() == 1 )
																						{
																							if( asn_packets0[ 0 ]->_asn_class == XEnum_AsnClass_Universal
																								&& asn_packets0[ 0 ]->_asn_universal_class_tags == XEnum_AsnUniversalClassTags_SEQUENCE
																								&& asn_packets0[ 0 ]->_is_constructed == 1 )
																							{
																								XMallocArray< rux::network::AsnPacket* > asn_packets1;
																								rux::network::AsnPacket::parse( *asn_packets0[ 0 ] , asn_packets1 , error );
																								if( error.Length() == 0 )
																								{
																									rux::network::AsnPacket* supported_mech = NULL;
																									rux::network::AsnPacket* response_token = NULL;
																									for( size_t index0 = 0 ; index0 < asn_packets1.Count() ; index0++ )
																									{
																										if( asn_packets1[ index0 ]->_asn_class == XEnum_AsnClass_ContextSpecific																								
																											&& asn_packets1[ index0 ]->_is_constructed == 1 )
																										{
																											if( asn_packets1[ index0 ]->_asn_universal_class_tags == 1 )
																												supported_mech = asn_packets1[ index0 ];
																											else if( asn_packets1[ index0 ]->_asn_universal_class_tags == 2 )
																												response_token = asn_packets1[ index0 ];
																										}
																									}
																									if( supported_mech && response_token )
																									{
																										XMallocArray< rux::network::AsnPacket* > asn_packets2;
																										rux::network::AsnPacket::parse( *supported_mech , asn_packets2 , error );
																										if( error.Length() == 0 )
																										{
																											if( asn_packets2.Count() == 1 )
																											{
																												if( asn_packets2[ 0 ]->_asn_class == XEnum_AsnClass_Universal
																													&& asn_packets2[ 0 ]->_asn_universal_class_tags == XEnum_AsnUniversalClassTags_OBJECTIDENTIFIER
																													&& asn_packets2[ 0 ]->_is_constructed == 0 )
																												{
																													rux::XString oid_string;
																													asn_packets2[ 0 ]->oid_to_string( oid_string , error );
																													if( error.Length() == 0 )
																													{
																														if( oid_string.Equals( "1.3.6.1.4.1.311.2.2.10" , 22 ) )
																														{
																															XMallocArray< rux::network::AsnPacket* > asn_packets3;
																															rux::network::AsnPacket::parse( *response_token , asn_packets3 , error );
																															if( error.Length() == 0 )
																															{
																																if( asn_packets3.Count() == 1 )
																																{
																																	rux::network::AsnPacket* security_blob = asn_packets3[ 0 ];
																																	if( security_blob->_asn_class == XEnum_AsnClass_Universal
																																		&& security_blob->_asn_universal_class_tags == XEnum_AsnUniversalClassTags_OCTETSTRING
																																		&& security_blob->_is_constructed == 0 )
																																	{
																																		size_t processed_size = 0;
																																		if( rux::network::ntlm::parse_challenge_message( (rux::uint8*)security_blob->_buffer_ptr , security_blob->_buffer_ptr_length , processed_size ) )
																																		{
																																			rux::network::ntlm_challenge_message* ntlm_challenge_message = (rux::network::ntlm_challenge_message*)security_blob->_buffer_ptr;
																																			smb_com_session_setup_andx_request._Bytes_size = rux::network::ntlm::generate_authenticate_message( username , password , (char*)ntlm_challenge_message->_challenge , smb_com_session_setup_andx_request._Bytes , sizeof( smb_com_session_setup_andx_request._Bytes) , is_NTLMSSP_NEGOTIATE_EXTENDED_SESSIONSECURITY );
																																			smb_com_session_setup_andx_request.refresh();
																																			smb_com_session_setup_andx_request._smb_packet_header._UID = smb_com_session_setup_andx_response->_smb_packet_header._UID;
																																			for( ; ; )
																																			{									
																																				void* data = smb_com_session_setup_andx_request.next_to_send( data_size );
																																				if( data && data_size > 0 )
																																					send_data.Concatenate( data , data_size );
																																				else
																																					break;
																																			}					
																																			data_size = send_data._data_size;
																																			_tcp_client.Send( (char*)send_data.Read( data_size ) , data_size , error );
																																			if( error.Length() == 0 )
																																			{
																																				recv_data = NULL;
																																				_tcp_client.Receive( &recv_data , error );
																																				if( recv_data )
																																				{
																																					data_size = recv_data->_data_size;
																																					if( data_size >= sizeof( rux::network::smb_packet_header ) )
																																					{
																																						smb_com_session_setup_andx_response = (rux::network::smb_com_session_setup_andx_response*)recv_data->Read( data_size );
																																						if( smb_com_session_setup_andx_response->_smb_packet_header._Status == 0 )
																																						{
																																							if( smb_com_negotiate_request._smb_packet_header._PIDHigh == smb_com_session_setup_andx_response->_smb_packet_header._PIDHigh
																																								&& smb_com_negotiate_request._smb_packet_header._PIDLow == smb_com_session_setup_andx_response->_smb_packet_header._PIDLow )
																																							{
																																								if( data_size >= sizeof( rux::network::smb_com_session_setup_andx_response ) )
																																								{
																																									rux::uint8 is_SMB_SETUP_GUEST = 0 , is_SMB_SETUP_USER = 0;
																																									if( smb_com_session_setup_andx_response->_Action == 0 )
																																										is_SMB_SETUP_USER = 1;
																																									else
																																									{
																																										if( smb_com_session_setup_andx_response->_Action & rux::network::SMB_SETUP_GUEST )
																																											is_SMB_SETUP_GUEST = 1;
																																										rux::uint8 is_SMB_SETUP_USE_LANMAN_KEY = 0;
																																										if( smb_com_session_setup_andx_response->_Action & rux::network::SMB_SETUP_USE_LANMAN_KEY )
																																											is_SMB_SETUP_USE_LANMAN_KEY = 1;
																																									}
																																									if( is_SMB_SETUP_GUEST
																																										|| is_SMB_SETUP_USER )
																																									{
																																										_is_logon = 1;
																																										res = 1;
																																										_UID = smb_com_session_setup_andx_response->_smb_packet_header._UID;
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
																																rux::network::AsnPacket::free_packets( asn_packets3 );
																															}
																														}
																													}
																												}
																											}
																											rux::network::AsnPacket::free_packets( asn_packets2 );
																										}
																									}
																									rux::network::AsnPacket::free_packets( asn_packets1 );
																								}
																							}
																						}
																						rux::network::AsnPacket::free_packets( asn_packets0 );
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
						}
					}
				}
			}
			return res;
		};
		rux::uint8 SmbClient::CloseFile( rux::uint16 TID , rux::uint16 FID , rux::XString& error )
		{
			error.Clear();
			rux::uint8 res = 0;
			if( _is_logon == 1 )
			{				
				rux::network::smb_com_close_request smb_com_close_request( _UID , TID , FID , 0 );
				_tcp_client.Send( (char*)&smb_com_close_request , sizeof( smb_com_close_request ) , error );
				if( error.Length() == 0 )
				{
					XData* recv_data = NULL;
					_tcp_client.Receive( &recv_data , error );
					if( recv_data )
					{
						size_t data_size = recv_data->_data_size;
						if( data_size > 0 )
						{
							rux::network::smb_com_close_response* smb_com_close_response = (rux::network::smb_com_close_response*)recv_data->Read( data_size );
							if( data_size >= sizeof( rux::network::smb_packet_header ) )
							{
								if( smb_com_close_request._smb_packet_header._PIDHigh == smb_com_close_response->_smb_packet_header._PIDHigh
									&& smb_com_close_request._smb_packet_header._PIDLow == smb_com_close_response->_smb_packet_header._PIDLow )
								{
									if( smb_com_close_response->_smb_packet_header._Command == rux::network::SMB_COM_CLOSE )
									{										
										if( smb_com_close_response->_smb_packet_header._Status == rux::network::STATUS_OK )
											res = 1;
										else if( smb_com_close_response->_smb_packet_header._Status == STATUS_INVALID_HANDLE
											|| smb_com_close_response->_smb_packet_header._Status == STATUS_SMB_BAD_FID )
											error = "The FID is invalid";
										else if( smb_com_close_response->_smb_packet_header._Status == STATUS_INVALID_SMB )
											error = "The TID specified in the command is invalid";
										else if( smb_com_close_response->_smb_packet_header._Status == STATUS_SMB_BAD_TID )
											error = "The TID specified in the command is invalid";
										else if( smb_com_close_response->_smb_packet_header._Status == STATUS_BAD_DEVICE_TYPE )
											error = "Invalid attempt to close an open spool file OR Invalid device - printer request made to a non-printer connection or non-printer request made to a printer connection";
										else if( smb_com_close_response->_smb_packet_header._Status == STATUS_SMB_BAD_UID )
											error = "The UID specified is not known as a valid ID on this server session, or the user identified by the UID does not have sufficient privileges";
									}
								}
							}
						}
					}
				}
			}
			return res;
		};
		rux::uint8 SmbClient::CopyFile( const rux::XString& src_filename , const rux::XString& dst_path , const rux::XString& username , const rux::XString& password , rux::XString& error )
		{
			rux::XString cp_src_filename;
			cp_src_filename.set_ByRef( src_filename.ConvertToAnsi() );
			rux::io::file src( cp_src_filename.str() );
			if( src.opened() )
			{
				rux::XArray< rux::XString > splits;
				splits.set_ByRef( dst_path.Split( '\\' ) );
				if( splits.Count() >= 2 )
				{
					rux::network::SmbClient cli;
					if( cli.Logon( username , password , splits[ 0 ] , error ) == 1 )
					{
						rux::uint16 TID = 0;
						if( cli.TreeConnect( "\\\\" + splits[ 0 ] + "\\" + splits[ 1 ] , TID , error ) == 1 )
						{
							rux::XString dst_filename;
							for( size_t index0 = 2 ; index0 < splits.Count() ; index0++ )
								dst_filename += splits[ index0 ] + "\\";
							rux::uint16 FID = 0;
							if( cli.CreateFile( dst_filename + rux::io::XPath::GetFileName( src_filename ) , TID , FID , error ) == 1 )
							{
								uint64 filesize = src.size();
								uint64 offset = 0;
								size_t readen = 0;
								rux::uint8* data = alloc_array_macros( rux::uint8 , cli._MaxBufferSize );
								do
								{
									if( src.read_bytes( (char*)data , cli._MaxBufferSize , readen ) )
									{
										cli.WriteFile( TID , FID , data , readen , offset , error );
										offset += readen;
									}
									else
									{
										error = "Cannot read from file '" + src_filename + "'";
										break;
									}
								}
								while( filesize != offset );
								cli.CloseFile( TID , FID , error );
							}
						}
						cli.Logoff();
					}
				}
				src.close();
			}
			else
				error = "Cannot open file '" + src_filename + "'";
			return error.Length() == 0 ? 1 : 0;
		};
		rux::uint8 SmbClient::WriteFile( rux::uint16 TID , rux::uint16 FID , rux::uint8* data , size_t size , uint64 file_offset , rux::XString& error )
		{
			error.Clear();
			if( _is_logon == 1 )
			{				
				rux::network::smb_com_write_andx_request smb_com_write_andx_request( _UID , TID , FID );
				size_t portion_size = size , data_offset = 0 , data_size = 0;
				if( portion_size + sizeof( rux::network::smb_com_write_andx_request ) - sizeof( rux::network::netbios_session_service_packet_header ) > _MaxBufferSize )
					portion_size = _MaxBufferSize - sizeof( rux::network::smb_com_write_andx_request ) + sizeof( rux::network::netbios_session_service_packet_header );
				XData send_data;
				do
				{
					if( portion_size > size - data_offset )
						portion_size = size - data_offset;
					smb_com_write_andx_request.data( &send_data , &data[ data_offset ] , file_offset , portion_size );
					data_size = send_data._data_size;
					_tcp_client.Send( (char*)send_data.Read( data_size ) , data_size , error );
					if( error.Length() == 0 )
					{
						XData* recv_data = NULL;
						_tcp_client.Receive( &recv_data , error );
						if( recv_data )
						{
							size_t data_size = recv_data->_data_size;
							if( data_size > 0 )
							{
								rux::network::smb_com_write_andx_response* smb_com_write_andx_response = (rux::network::smb_com_write_andx_response*)recv_data->Read( data_size );
								if( data_size >= sizeof( rux::network::smb_packet_header ) )
								{
									if( smb_com_write_andx_request._smb_packet_header._PIDHigh == smb_com_write_andx_response->_smb_packet_header._PIDHigh
										&& smb_com_write_andx_request._smb_packet_header._PIDLow == smb_com_write_andx_response->_smb_packet_header._PIDLow )
									{
										if( smb_com_write_andx_response->_smb_packet_header._Command == rux::network::SMB_COM_WRITE_ANDX )
										{										
											if( smb_com_write_andx_response->_smb_packet_header._Status == rux::network::STATUS_OK )
											{
												if( smb_com_write_andx_response->_Count == 0 )
													error = "No space on the file system";
												else
												{
													data_offset += smb_com_write_andx_response->_Count;
													file_offset += smb_com_write_andx_response->_Count;
												}
											}											
											else if( smb_com_write_andx_response->_smb_packet_header._Status == STATUS_FILE_LOCK_CONFLICT )
												error = "A record lock has been taken on the file or the client has attempted to write to a portion of the file that the server detects has been locked";
											else if( smb_com_write_andx_response->_smb_packet_header._Status == STATUS_INVALID_HANDLE
												|| smb_com_write_andx_response->_smb_packet_header._Status == STATUS_SMB_BAD_FID )
												error = "Invalid FID, or FID mapped to a valid server FID but it was not acceptable to the operating system";
											else if( smb_com_write_andx_response->_smb_packet_header._Status == STATUS_INSUFF_SERVER_RESOURCES )
												error = "The server is out of resources";
											else if( smb_com_write_andx_response->_smb_packet_header._Status == STATUS_ACCESS_DENIED )
												error = "Write permission required";
											//else if( smb_com_write_andx_response->_smb_packet_header._Status == STATUS_FILE_LOCK_CONFLICT )
											//	error = "The requested byte range was already locked by a different process (PID)";
											else if( smb_com_write_andx_response->_smb_packet_header._Status == STATUS_PIPE_BUSY )
												error = "Attempted to read from a busy pipe";
											else if( smb_com_write_andx_response->_smb_packet_header._Status == STATUS_PIPE_DISCONNECTED )
												error = "Write to a named pipe with no reader";
											else if( smb_com_write_andx_response->_smb_packet_header._Status == STATUS_INVALID_SMB )
												error = "A corrupt or invalid SMB request was received";
											else if( smb_com_write_andx_response->_smb_packet_header._Status == STATUS_SMB_BAD_TID )
												error = "Invalid TID in request";
											else if( smb_com_write_andx_response->_smb_packet_header._Status == STATUS_PRINT_QUEUE_FULL )
												error = "Print queue is full--too many queued items";
											else if( smb_com_write_andx_response->_smb_packet_header._Status == STATUS_NO_SPOOL_SPACE )
												error = "Print queue is full--too many queued items";
											else if( smb_com_write_andx_response->_smb_packet_header._Status == STATUS_SMB_BAD_UID )
												error = "The UID specified is not known as a valid ID on this server session, or the user identified by the UID does not have sufficient privileges";
											else if( smb_com_write_andx_response->_smb_packet_header._Status == STATUS_DATA_ERROR )
												error = "A problem has occurred in the physical I/O";
											else if( smb_com_write_andx_response->_smb_packet_header._Status == STATUS_DISK_FULL )
												error = "The file system is full";
										}
										else
											error = "Received SMB command is not SMB_COM_WRITE_ANDX";
									}
									else
										error = "Received PID is incorrect";
								}
								else
									error = "Received data size is less than size of SMB packet header";
							}
							else
								error = "Received data size is 0";
						}
					}
					if( error.Length() > 0 )
						break;
				}
				while( data_offset < size );
			}
			return error.Length() == 0 ? 1 : 0;
		};
		rux::uint8 SmbClient::CreateFile( const rux::XString& filename , rux::uint16 TID , rux::uint16& FID , rux::XString& error )
		{
			error.Clear();
			FID = 0;
			rux::uint8 res = 0;
			if( _is_logon == 1 )
			{				
				rux::network::smb_com_nt_create_andx_request smb_com_nt_create_andx_request( _UID , TID , filename );
				_tcp_client.Send( (char*)&smb_com_nt_create_andx_request , smb_com_nt_create_andx_request._smb_com_nt_create_andx_request_size , error );
				if( error.Length() == 0 )
				{
					XData* recv_data = NULL;
					_tcp_client.Receive( &recv_data , error );
					if( recv_data )
					{
						size_t data_size = recv_data->_data_size;
						if( data_size > 0 )
						{
							rux::network::smb_com_nt_create_andx_response* smb_com_nt_create_andx_response = (rux::network::smb_com_nt_create_andx_response*)recv_data->Read( data_size );
							if( data_size >= sizeof( rux::network::smb_packet_header ) )
							{
								if( smb_com_nt_create_andx_request._smb_packet_header._PIDHigh == smb_com_nt_create_andx_response->_smb_packet_header._PIDHigh
									&& smb_com_nt_create_andx_request._smb_packet_header._PIDLow == smb_com_nt_create_andx_response->_smb_packet_header._PIDLow )
								{
									if( smb_com_nt_create_andx_response->_smb_packet_header._Command == rux::network::SMB_COM_NT_CREATE_ANDX )
									{										
										if( smb_com_nt_create_andx_response->_smb_packet_header._Status == rux::network::STATUS_OK )
										{
											res = 1;
											FID = smb_com_nt_create_andx_response->_FID;
										}
										else if( smb_com_nt_create_andx_response->_smb_packet_header._Status == STATUS_OBJECT_PATH_SYNTAX_BAD )
											error = "The file path syntax is invalid";
										else if( smb_com_nt_create_andx_response->_smb_packet_header._Status == STATUS_OS2_TOO_MANY_OPEN_FILES
											|| smb_com_nt_create_andx_response->_smb_packet_header._Status == STATUS_TOO_MANY_OPENED_FILES )
											error = "Too many open files; no more FIDs available";
										else if( smb_com_nt_create_andx_response->_smb_packet_header._Status == STATUS_ACCESS_DENIED )
											error = "Access denied";
										else if( smb_com_nt_create_andx_response->_smb_packet_header._Status == STATUS_FILE_IS_A_DIRECTORY )
											error = "Named file is an existing directory and CreateOptions in the request contains FILE_NON_DIRECTORY_FILE";
										else if( smb_com_nt_create_andx_response->_smb_packet_header._Status == STATUS_INVALID_HANDLE
											|| smb_com_nt_create_andx_response->_smb_packet_header._Status == STATUS_SMB_BAD_FID )
											error = "Invalid FID; RootDirectoryFID is not valid";
										else if( smb_com_nt_create_andx_response->_smb_packet_header._Status == STATUS_INSUFF_SERVER_RESOURCES )
											error = "The server is out of resources";
										//else if( smb_com_nt_create_andx_response->_smb_packet_header._Status == STATUS_ACCESS_DENIED )
										//	error = "Invalid open mode";
										else if( smb_com_nt_create_andx_response->_smb_packet_header._Status == STATUS_SHARING_VIOLATION )
											error = "Sharing violation";
										else if( smb_com_nt_create_andx_response->_smb_packet_header._Status == STATUS_NOT_SUPPORTED )
											error = "This command is not supported by the server";
										else if( smb_com_nt_create_andx_response->_smb_packet_header._Status == STATUS_OBJECT_NAME_COLLISION )
											error = "The file already exists";
										else if( smb_com_nt_create_andx_response->_smb_packet_header._Status == STATUS_INVALID_PARAMETER )
											error = "One of the request values is out of range";
										else if( smb_com_nt_create_andx_response->_smb_packet_header._Status == STATUS_INVALID_SMB )
											error = "Invalid SMB. Not enough parameter bytes were sent or the path extends beyond the end of the message";
										else if( smb_com_nt_create_andx_response->_smb_packet_header._Status == STATUS_SMB_BAD_TID )
											error = "The TID is no longer valid";
										else if( smb_com_nt_create_andx_response->_smb_packet_header._Status == STATUS_INVALID_DEVICE_TYPE )
											error = "Device type and request are inconsistent";
										else if( smb_com_nt_create_andx_response->_smb_packet_header._Status == STATUS_SMB_BAD_UID )
											error = "The UID supplied is not defined to the session";
										else if( smb_com_nt_create_andx_response->_smb_packet_header._Status == STATUS_DATA_ERROR )
											error = "A problem has occurred in the physical I/O";
									}
								}
							}
						}
					}
				}
			}
			return res;
		};
		rux::uint8 SmbClient::TreeConnect( const rux::XString& path , rux::uint16& TID , rux::XString& error )
		{
			error.Clear();
			TID = 0;
			rux::uint8 res = 0;
			if( _is_logon == 1 )
			{				
				rux::network::smb_com_tree_connect_andx_request smb_com_tree_connect_andx_request( _UID , path , "" );
				_tcp_client.Send( (char*)&smb_com_tree_connect_andx_request , smb_com_tree_connect_andx_request._smb_com_tree_connect_andx_request_size , error );
				if( error.Length() == 0 )
				{
					XData* recv_data = NULL;
					_tcp_client.Receive( &recv_data , error );
					if( recv_data )
					{
						size_t data_size = recv_data->_data_size;
						if( data_size > 0 )
						{
							rux::network::smb_com_tree_connect_andx_response* smb_com_tree_connect_andx_response = (rux::network::smb_com_tree_connect_andx_response*)recv_data->Read( data_size );
							if( data_size >= sizeof( rux::network::smb_packet_header ) )
							{
								if( smb_com_tree_connect_andx_request._smb_packet_header._PIDHigh == smb_com_tree_connect_andx_response->_smb_packet_header._PIDHigh
									&& smb_com_tree_connect_andx_request._smb_packet_header._PIDLow == smb_com_tree_connect_andx_response->_smb_packet_header._PIDLow )
								{
									if( smb_com_tree_connect_andx_response->_smb_packet_header._Command == SMB_COM_TREE_CONNECT_ANDX )
									{										
										if( smb_com_tree_connect_andx_response->_smb_packet_header._Status == rux::network::STATUS_OK )
										{
											res = 1;
											TID = smb_com_tree_connect_andx_response->_smb_packet_header._TID;
										}
									}
								}
							}
						}
					}
				}
			}
			return res;
		};
		rux::uint8 SmbClient::Logoff( void )
		{
			rux::uint8 res = 0;
			if( _is_logon == 1 )
			{
				XString error;
				rux::network::smb_com_logoff_andx_request smb_com_logoff_andx_request( _UID );
				_tcp_client.Send( (char*)&smb_com_logoff_andx_request , sizeof( smb_com_logoff_andx_request ) , error );
				if( error.Length() == 0 )
				{
					XData* recv_data = NULL;
					_tcp_client.Receive( &recv_data , error );
					if( recv_data )
					{
						size_t data_size = recv_data->_data_size;
						if( data_size > 0 )
						{
						}
					}
				}
				_tcp_client.Close();
				_is_logon = 0;
			}
			return res;
		};



		size_t packet::size( void )
		{
			return 0;
		};
		netbios_session_service_packet_header::netbios_session_service_packet_header()
		{
			_MessageType = 0;
			_Length[ 2 ] = 0;
			_Length[ 1 ] = 0;
			_Length[ 0 ] = 0;
		};
		netbios_session_service_packet_header::netbios_session_service_packet_header( rux::uint32 size )
		{
			_MessageType = 0;
			_Length[ 2 ] = ( (rux::uint8*)&size )[ 0 ];
			_Length[ 1 ] = ( (rux::uint8*)&size )[ 1 ];
			_Length[ 0 ] = ( (rux::uint8*)&size )[ 2 ];
		};
		void netbios_session_service_packet_header::size( rux::uint32 size )
		{
			_Length[ 2 ] = ( (rux::uint8*)&size )[ 0 ];
			_Length[ 1 ] = ( (rux::uint8*)&size )[ 1 ];
			_Length[ 0 ] = ( (rux::uint8*)&size )[ 2 ];
		};
		smb_packet_header::smb_packet_header()
		{
			_Protocol[ 0 ] = 0xFF;
			_Protocol[ 1 ] = 'S';
			_Protocol[ 2 ] = 'M';
			_Protocol[ 3 ] = 'B';
			_Status = 0;
			_Flags = SMB_FLAGS_CASE_INSENSITIVE | SMB_FLAGS_CANONICALIZED_PATHS;
			_Flags2 = SMB_FLAGS2_NT_STATUS | SMB_FLAGS2_UNICODE | SMB_FLAGS2_LONG_NAMES | SMB_FLAGS2_EXTENDED_SECURITY;
			_Reserved = 0;
			memset( &_SecurityFeatures , 0 , 8 );
			pid_t pid = rux::diagnostics::process::get_process_id();
			_PIDHigh = ( (rux::uint16*)&pid )[ 1 ];
			_TID = 0;
			_PIDLow = ( (rux::uint16*)&pid )[ 0 ];
			_UID = 0;
			_MID = 0;
		};
		smb_packet_header::smb_packet_header( rux::uint32 size )
			: _netbios_session_service_packet_header( size - sizeof( _netbios_session_service_packet_header ) )
		{
			_Protocol[ 0 ] = 0xFF;
			_Protocol[ 1 ] = 'S';
			_Protocol[ 2 ] = 'M';
			_Protocol[ 3 ] = 'B';
			_Status = 0;
			_Flags = SMB_FLAGS_CASE_INSENSITIVE | SMB_FLAGS_CANONICALIZED_PATHS;
			_Flags2 = SMB_FLAGS2_NT_STATUS | SMB_FLAGS2_UNICODE | SMB_FLAGS2_LONG_NAMES | SMB_FLAGS2_EXTENDED_SECURITY;
			_Reserved = 0;
			memset( &_SecurityFeatures , 0 , 8 );
			pid_t pid = rux::diagnostics::process::get_process_id();
			_PIDHigh = ( (rux::uint16*)&pid )[ 1 ];
			_TID = 0;
			_PIDLow = ( (rux::uint16*)&pid )[ 0 ];
			_UID = 0;
			_MID = 0;
		};
		smb_dialect::smb_dialect()
		{
			_BufferFormat = 0x02;
			memcpy( _DialectString , "NT LM 0.12" , 11 );
		};
		smb_com_negotiate_request::smb_com_negotiate_request()
			: _smb_packet_header( sizeof( smb_com_negotiate_request ) )
		{
			_smb_packet_header._Command = SMB_COM_NEGOTIATE;
			_WordCount = 0;
			_ByteCount = sizeof( _smb_dialect );
		};		
		smb_com_close_request::smb_com_close_request( rux::uint16 UID , rux::uint16 TID , rux::uint16 FID , rux::uint32 LastTimeModified )
		{
			_smb_packet_header._Command = SMB_COM_CLOSE;
			_smb_packet_header._TID = TID;
			_smb_packet_header._UID = UID;
			_FID = FID;
			_WordCount = 0x03;
			_ByteCount = 0;
			_smb_packet_header._netbios_session_service_packet_header.size( sizeof( smb_com_close_request ) - sizeof( netbios_session_service_packet_header ) );
		};
		smb_com_write_andx_request::smb_com_write_andx_request( rux::uint16 UID , rux::uint16 TID , rux::uint16 FID )
		{
			_smb_packet_header._Command = SMB_COM_WRITE_ANDX;
			_smb_packet_header._TID = TID;
			_smb_packet_header._UID = UID;
			_WordCount = 0x0E;
			_AndXCommand = 0xFF;
			_AndXReserved = 0;
			_FID = FID;			
			_Timeout = 0;
			_WriteMode = WritethroughMode;
			_Remaining = 0;
			_Reserved = 0;			 
			 _Pad = 0;
		};
		void smb_com_write_andx_request::data( XData* dst , rux::uint8* data , uint64 offset , size_t size )
		{
			_ByteCount = 1 + size;
			 _Offset = ((rux::uint32*)&offset)[ 0 ];
			 _DataLength = size;
			_DataOffset = sizeof( smb_com_write_andx_request )
				- sizeof( netbios_session_service_packet_header );
			 _OffsetHigh = ((rux::uint32*)&offset)[ 1 ];
			_AndXOffset = sizeof( smb_com_write_andx_request )
				- sizeof( netbios_session_service_packet_header )
				+ size;
			_smb_packet_header._netbios_session_service_packet_header.size( _AndXOffset );
			if( dst->_data_size > 0 )
				dst->Read( dst->_data_size );
			dst->Concatenate( this , sizeof( smb_com_write_andx_request ) );
			dst->Concatenate( data , size );
		};
		smb_com_nt_create_andx_request::smb_com_nt_create_andx_request( rux::uint16 UID , rux::uint16 TID , const rux::XString& path )
		{
			_smb_packet_header._Command = SMB_COM_NT_CREATE_ANDX;
			_smb_packet_header._TID = TID;
			_smb_packet_header._UID = UID;
			_WordCount = 0x18;
			_AndXCommand = 0xFF;
			_AndXReserved = 0;
			_Reserved = 0;
			_FileName[ 0 ] = 0;
			rux::XString utf16_path;
			utf16_path.set_ByRef( path.ConvertToUTF16() );
			if( utf16_path.Length() > 0 )
				memcpy( &_FileName[ 1 ] , utf16_path.str() , utf16_path.Size() );
			_NameLength = utf16_path.Length() * 2;
			_Flags = NT_CREATE_REQUEST_OPLOCK | NT_CREATE_REQUEST_OPBATCH;
			_RootDirectoryFID = 0;
			_DesiredAccess = SMB_FILE_READ_DATA | SMB_FILE_WRITE_DATA 
				| SMB_FILE_APPEND_DATA | SMB_FILE_READ_EA | SMB_FILE_WRITE_EA 
				| SMB_FILE_READ_ATTRIBUTES | SMB_FILE_WRITE_ATTRIBUTES 
				| SMB_READ_CONTROL;
			_AllocationSize = 0;
			_ExtFileAttributes = SMB_ATTR_NORMAL;
			_ShareAccess = SMB_FILE_SHARE_READ | SMB_FILE_SHARE_WRITE | SMB_FILE_SHARE_DELETE;
			_CreateDisposition = FILE_CREATE;
			_CreateOptions = FILE_NON_DIRECTORY_FILE;
			_ImpersonationLevel = SEC_IMPERSONATE;
			_ByteCount = _NameLength + 3;
			_AndXOffset = sizeof( smb_com_nt_create_andx_request )
				- sizeof( netbios_session_service_packet_header )
				- sizeof( _FileName ) - sizeof( _smb_com_nt_create_andx_request_size ) 				
				+ _ByteCount;			
			_smb_packet_header._netbios_session_service_packet_header.size( _AndXOffset );
			_smb_com_nt_create_andx_request_size = _AndXOffset + sizeof( _smb_packet_header._netbios_session_service_packet_header );
		};
		smb_com_tree_connect_andx_request::smb_com_tree_connect_andx_request( rux::uint16 UID , const rux::XString& path , const rux::XString& service )
		{
			_smb_packet_header._Command = SMB_COM_TREE_CONNECT_ANDX;
			_smb_packet_header._UID = UID;
			_WordCount = 0x04;
			_AndXCommand = 0xFF;
			_AndXReserved = 0;
			_Flags = 0x0008;
			_PasswordLength = 1;
			size_t offset = 0;
			_data[ offset ] = 0;//pad or password
			offset++;
			rux::XString utf16_path;
			utf16_path.set_ByRef( path.ConvertToUTF16() );
			if( utf16_path.Length() > 0 )
			{
				memcpy( &_data[ offset ] , utf16_path.str() , utf16_path.Size() );
				offset += utf16_path.Size();
			}
			rux::XString oem_service;
			oem_service.set_ByRef( service.ConvertToAnsi() );
			if( oem_service.Length() == 0 )
				oem_service = "?????";
			memcpy( &_data[ offset ] , oem_service.str() , oem_service.Size() );
			offset += oem_service.Size();
			_AndXOffset = sizeof( smb_com_tree_connect_andx_request )
				- sizeof( netbios_session_service_packet_header )
				- sizeof( _data ) - sizeof( _smb_com_tree_connect_andx_request_size )
				+ offset;
			_ByteCount = offset;
			_smb_packet_header._netbios_session_service_packet_header.size( _AndXOffset );
			_smb_com_tree_connect_andx_request_size = _AndXOffset + sizeof( _smb_packet_header._netbios_session_service_packet_header );
		};
		smb_com_logoff_andx_request::smb_com_logoff_andx_request( rux::uint16 UID )
		{
			_smb_packet_header._Command = SMB_COM_LOGOFF_ANDX;
			_smb_packet_header._UID = UID;
			_WordCount = 0x02;
			_AndXCommand = 0xFF;
			_AndXReserved = 0;
			_AndXOffset = sizeof( smb_com_logoff_andx_request ) - sizeof( netbios_session_service_packet_header );
			_ByteCount = 0x0000;
			_smb_packet_header._netbios_session_service_packet_header.size( _AndXOffset );
		};
		smb_com_session_setup_andx_request::smb_com_session_setup_andx_request( rux::network::smb_com_negotiate_response_extended* smb_com_negotiate_response_ptr )
		{
			_smb_packet_header._Command = SMB_COM_SESSION_SETUP_ANDX;
			_WordCount = 12;
			_AndXCommand = 0xFF;
			_AndXReserved = 0;
			_AndXOffset = 0;
			if( smb_com_negotiate_response_ptr->_Capabilities & rux::network::CAP_LARGE_READX )
				_MaxBufferSize = smb_com_negotiate_response_ptr->_MaxBufferSize;
			else if( smb_com_negotiate_response_ptr->_Capabilities & rux::network::CAP_RAW_MODE )
				_MaxBufferSize = smb_com_negotiate_response_ptr->_MaxRawSize;
			_MaxMpxCount = smb_com_negotiate_response_ptr->_MaxMpxCount;
			_VcNumber = 0;
			_SessionKey = smb_com_negotiate_response_ptr->_SessionKey;
			_SecurityBlobLength = 0;
			_Reserved = 0x00000000;
			_Capabilities = CAP_UNICODE | CAP_NT_SMBS | CAP_LEVEL_II_OPLOCKS | CAP_DYNAMIC_REAUTH | CAP_EXTENDED_SECURITY | CAP_STATUS32;
			_Pad[ 0 ] = 0;
			char native_os[] =
			{
				'W' , 0 , 'i' , 0 , 'n' , 0 , 'd' , 0 , 'o' , 0 , 'w' , 0 , 's' , 0 , ' ' , 0 , '2' , 0 , '0' , 0 , '0' , 0 , 
				'2' , 0 , ' ' , 0 , 'S' , 0 , 'e' , 0 , 'r' , 0 , 'v' , 0 , 'i' , 0 , 'c' , 0 , 'e' , 0 , ' ' , 0 , 'P' , 0 , 
				'a' , 0 , 'c' , 0 , 'k' , 0 , ' ' , 0 , '3' , 0 , ' ' , 0 , '2' , 0 , '6' , 0 , '0' , 0 , '0' , 0 , 0 , 0
			};
			memcpy( _NativeOS , native_os , sizeof( native_os ) );
			char native_lan_man[] =
			{
				'W' , 0 , 'i' , 0 , 'n' , 0 , 'd' , 0 , 'o' , 0 , 'w' , 0 , 's' , 0 , ' ' , 0 , '2' , 0 , '0' , 0 , '0' , 0 , 
				'2' , 0 , ' ' , 0 , '5' , 0 , '.' , 0 , '1' , 0 , 0 , 0
			};
			memcpy( _NativeLanMan , native_lan_man , sizeof( native_lan_man ) );
			memset( &_PrimaryDomain , 0 , 2 );
		};
		void* smb_com_session_setup_andx_request::next_to_send( size_t& send_size )
		{
			void* current = _current;
			if( _current == &_smb_packet_header )
			{
				send_size = sizeof( smb_com_session_setup_andx_request ) 
				- sizeof( _Bytes ) - sizeof( _Pad ) - sizeof( _PrimaryDomain ) - sizeof( _NativeOS ) - sizeof( _NativeLanMan ) - sizeof( _current ) - sizeof( _Bytes_size );
				_current = &_Bytes;
			}
			else if( _current == &_Bytes )
			{
				send_size = _Bytes_size;
				_current = &_Pad;
			}
			else if( _current == &_Pad )
			{
				send_size = sizeof( _Pad ) + sizeof( _PrimaryDomain ) + sizeof( _NativeOS ) + sizeof( _NativeLanMan );
				_current = NULL;
			}
			return current;
		};
		void smb_com_session_setup_andx_request::refresh()
		{
			_current = &_smb_packet_header;
			_SecurityBlobLength = _Bytes_size;
			_ByteCount = _Bytes_size + sizeof( _Pad ) + sizeof( _PrimaryDomain ) + sizeof( _NativeOS ) + sizeof( _NativeLanMan );
			_AndXOffset = sizeof( smb_com_session_setup_andx_request ) + _ByteCount - sizeof( _Pad ) - sizeof( _PrimaryDomain )
				- sizeof( _Bytes ) - sizeof( _NativeOS ) - sizeof( _NativeLanMan ) - sizeof( _current ) - sizeof( _Bytes_size ) - sizeof( netbios_session_service_packet_header );
			_smb_packet_header._netbios_session_service_packet_header.size( _AndXOffset );
		};
		rux::uint8 ntlm::parse_challenge_message( rux::uint8* buffer , size_t size , size_t& processed_size )
		{		
			processed_size = 0;
			rux::uint8 res = 0;
			if( size >= sizeof( rux::network::ntlm_challenge_message ) )
			{
				ntlm_challenge_message* _ntlm_challenge_message = (ntlm_challenge_message*)buffer;
				if( memcmp( _ntlm_challenge_message->_Signature , "NTLMSSP" , 8 ) == 0 )
				{
					if( _ntlm_challenge_message->_MessageType == rux::network::NTLM_CHALLENGE_MESSAGE )
					{
						processed_size = _ntlm_challenge_message->_target_name_header._length + _ntlm_challenge_message->_target_info_fields_header._length + sizeof( rux::network::ntlm_challenge_message );
						if( size >= processed_size )
							res = 1;
						else
							processed_size = 0;
					}
				}
			}
			return res;
		};
		size_t ntlm::generate_authenticate_message( const rux::XString& username , const rux::XString& password , char challenge[ 8 ] , rux::uint8* dst , size_t dst_size , rux::uint8 NTLMSSP_NEGOTIATE_EXTENDED_SESSIONSECURITY , char client_challenge[ 8 ] )
		{
			rux::network::AsnPacket response_token;
			rux::XArray< XString > splits;
			splits.set_ByRef( username.Split( '\\' ) );			
			rux::network::ntlm_authenticate_message _ntlm_authenticate_message( challenge , splits.Count() > 1 ? rux::XString( splits[ 0 ] ) : "" , splits.Count() > 1 ? splits[ 1 ] : splits[ 0 ] , "" , password , NTLMSSP_NEGOTIATE_EXTENDED_SESSIONSECURITY );
			response_token.fill( XEnum_AsnClass_Universal , 0 , XEnum_AsnUniversalClassTags_OCTETSTRING , (char*)&_ntlm_authenticate_message , _ntlm_authenticate_message._ntlm_authenticate_message_size );

			rux::network::AsnPacket tag2;
			XMallocArray< rux::network::AsnPacket* > tag2_asn_packets;
			tag2_asn_packets.Add( &response_token );
			tag2.fill( XEnum_AsnClass_ContextSpecific , 1 , (XEnum_AsnUniversalClassTags)2 , tag2_asn_packets );

			rux::network::AsnPacket neg_token_resp;
			XMallocArray< rux::network::AsnPacket* > neg_token_resp_asn_packets;
			neg_token_resp_asn_packets.Add( &tag2 );
			neg_token_resp.fill( XEnum_AsnClass_Universal , 1 , XEnum_AsnUniversalClassTags_SEQUENCE , neg_token_resp_asn_packets );

			rux::network::AsnPacket tag1;
			XMallocArray< rux::network::AsnPacket* > tag1_asn_packets;
			tag1_asn_packets.Add( &neg_token_resp );
			tag1.fill( XEnum_AsnClass_ContextSpecific , 1 , (XEnum_AsnUniversalClassTags)1 , tag1_asn_packets );
			return rux::network::AsnPacket::format( tag1 , dst );
		};
		size_t ntlm::generate_negotiate_message( rux::uint8* dst , size_t dst_size , rux::uint8& is_NTLMSSP_NEGOTIATE_EXTENDED_SESSIONSECURITY )
		{
			XMallocArray< rux::network::AsnPacket* > mechTypes_asn_packets , gss_api_asn_packets ,
				negTokenInit_asn_packets , simple_protected_negotiation_asn_packets , tag2_asn_packets ,
				tag0_asn_packets;
			rux::network::AsnPacket gss_api , negTokenInit , SPNEGO_oid , mechTypes , mechType , 
				simple_protected_negotiation , tag2 , tag0 , mechToken;
			mechType.fill_oid( "1.3.6.1.4.1.311.2.2.10" );//MechType: 1.3.6.1.4.1.311.2.2.10 (NTLMSSP - Microsoft NTLM Security Support Provider)
			mechTypes_asn_packets.Add( &mechType );
			mechTypes.fill( XEnum_AsnClass_Universal , 1 , XEnum_AsnUniversalClassTags_SEQUENCE , mechTypes_asn_packets );
			
			tag0_asn_packets.Add( &mechTypes );
			tag0.fill( XEnum_AsnClass_ContextSpecific , 1 , (XEnum_AsnUniversalClassTags)0 , tag0_asn_packets );

			rux::network::ntlm_negotiate_message _ntlm_negotiate_message( is_NTLMSSP_NEGOTIATE_EXTENDED_SESSIONSECURITY );
			mechToken.fill( XEnum_AsnClass_Universal , 0 , XEnum_AsnUniversalClassTags_OCTETSTRING , (char*)&_ntlm_negotiate_message , sizeof( _ntlm_negotiate_message ) );

			tag2_asn_packets.Add( &mechToken );
			tag2.fill( XEnum_AsnClass_ContextSpecific , 1 , (XEnum_AsnUniversalClassTags)2 , tag2_asn_packets );

			negTokenInit_asn_packets.Add( &tag0 );
			negTokenInit_asn_packets.Add( &tag2 );
			negTokenInit.fill( XEnum_AsnClass_Universal , 1 , XEnum_AsnUniversalClassTags_SEQUENCE , negTokenInit_asn_packets );
			
			simple_protected_negotiation_asn_packets.Add( &negTokenInit );
			simple_protected_negotiation.fill( XEnum_AsnClass_ContextSpecific , 1 , (XEnum_AsnUniversalClassTags)0 , simple_protected_negotiation_asn_packets );
			
			SPNEGO_oid.fill_oid( "1.3.6.1.5.5.2" );//OID: 1.3.6.1.5.5.2 (SPNEGO - Simple Protected Negotiation)
			gss_api_asn_packets.Add( &SPNEGO_oid );			
			gss_api_asn_packets.Add( &simple_protected_negotiation );
			gss_api.fill( XEnum_AsnClass_Application , 1 , (XEnum_AsnUniversalClassTags)0 , gss_api_asn_packets );						
			return rux::network::AsnPacket::format( gss_api , dst );
		};
		void ntlm::LmChallengeResponse( char challenge[ 8 ] , const rux::XString& password , char response[ 24 ] , rux::uint8 NTLMSSP_NEGOTIATE_EXTENDED_SESSIONSECURITY , char client_challenge[ 8 ] )
		{
			if( NTLMSSP_NEGOTIATE_EXTENDED_SESSIONSECURITY )
			{
				if( client_challenge )
					memcpy( response , client_challenge , 8 );
				else
					memset( response , 0xaa , 8 );
				memset( &response[ 8 ] , 0 , 16 );
			}
			else
			{
				declare_stack_variable( char , password_hash , 16 );
				LmPasswordHash( password , password_hash );
				ChallengeResponse( challenge, password_hash, response );
			}
		};				
		void ntlm::DesHash( char clear[ 7 ] , char cypher[ 8 ] )
		{
			DesEncrypt( "KGS!@#$%", clear, cypher );
		};
		void ntlm::DESL( char challenge[ 8 ] , char password_hash[ 16 ] , char response[ 24 ] )
		{
			ChallengeResponse( challenge , password_hash , response );
		};
		void ntlm::ChallengeResponse( char challenge[ 8 ] , char password_hash[ 16 ] , char response[ 24 ] )
		{
			declare_stack_variable( char , z_password_hash , 21 );
			memset( z_password_hash , 0 , 21 );
			memcpy( z_password_hash , password_hash , 16 );
			DesEncrypt( challenge , z_password_hash , response );
			DesEncrypt( challenge , &z_password_hash[ 7 ] , &response[ 8 ] );
			DesEncrypt( challenge , &z_password_hash[ 14 ] , &response[ 16 ] );
		};
		void ntlm::DesEncrypt( const char clear[ 8 ] , char key[ 7 ] , char cypher[ 8 ] )
		{			
			rux::uint8 des_key[ 8 ] = {0};
			rux::cryptography::XDES::make_key( (rux::uint8*)key , des_key );
			rux::cryptography::XDES des;
			des.set_CipherKey( (rux::uint8*)des_key );
			des.Encrypt( (rux::uint8*)clear , (rux::uint8*)cypher );
		};
		void ntlm::NtChallengeResponse( char challenge[ 8 ] , const rux::XString& password , char response[ 24 ] , rux::uint8 NTLMSSP_NEGOTIATE_EXTENDED_SESSIONSECURITY , char client_challenge[ 8 ] )
		{
			if( NTLMSSP_NEGOTIATE_EXTENDED_SESSIONSECURITY )
			{
				declare_stack_variable( char , password_hash , 16 );
				NtPasswordHash( password, password_hash );				
				declare_stack_variable( char , md5_plain , 16 );
				memcpy( md5_plain , challenge , 8 );
				if( client_challenge )
					memcpy( &md5_plain[ 8 ] , client_challenge , 8 );
				else
					memset( &md5_plain[ 8 ] , 0xaa , 8 );
				declare_stack_variable( char , md5_hash , 16 );
				rux::cryptography::XMD5::Hash( (rux::uint8*)md5_plain , 16 , (rux::uint8*)md5_hash );
				DESL( md5_hash , password_hash , response );
			}
			else
			{
				declare_stack_variable( char , password_hash , 16 );
				NtPasswordHash( password, password_hash );
				ChallengeResponse( challenge , password_hash , response );
			}
		};
		void ntlm::NtPasswordHash( const rux::XString& password , char password_hash[ 16 ] )
		{
			rux::XString utf16_password;
			utf16_password.set_ByRef( password.ConvertToUTF16() );
			if( utf16_password.Length() > 0 )
				rux::cryptography::MD4::Hash( (rux::uint8*)utf16_password.str() , utf16_password.Length() * 2 , (rux::uint8*)password_hash ); 		
		};
		void ntlm::LmPasswordHash( const rux::XString& password , char password_hash[ 16 ] )
		{
			rux::XString uppercase_password( password.ConvertToAnsi() );
			uppercase_password.set_ByRef( uppercase_password.ToUpper() );
			declare_stack_variable( char , ucase_password , 14 );
			memset( ucase_password , 0 , 14 );
			memcpy( ucase_password , uppercase_password.str() , uppercase_password.Length() >= 14 ? 14 : uppercase_password.Length() );
			DesHash( ucase_password , password_hash );
			DesHash( &ucase_password[ 7 ] , &password_hash[ 8 ] );
		};
		void ntlm::NTOWFv1( const rux::XString& password , rux::uint8 hash[ 16 ] )
		{
			NtPasswordHash( password , (char*)hash );
		};
		void ntlm::LMOWFv1( const rux::XString& password , rux::uint8 hash[ 16 ] )
		{
			LmPasswordHash( password , (char*)hash );
		};
		ntlm_authenticate_message::ntlm_authenticate_message( char challenge[ 8 ] , const rux::XString& domain_name , const rux::XString& user_name , const rux::XString& workstation , const rux::XString& password , rux::uint8 NTLMSSP_NEGOTIATE_EXTENDED_SESSIONSECURITY , char client_challenge[ 8 ] )
		{
			memcpy( _Signature , "NTLMSSP" , 8 );
			_MessageType = NTLM_AUTHENTICATE_MESSAGE;
			rux::XString error , local_workstation , local_domain_name , local_user_name;
			local_workstation.set_ByRef( workstation.ConvertToUTF16() );
			local_domain_name.set_ByRef( domain_name.ConvertToUTF16() );
			local_user_name.set_ByRef( user_name.ConvertToUTF16() );
			if( local_workstation.Length() == 0 )
			{
				local_workstation.set_ByRef( XNetworkInformation::get_HostName( error ) );
				local_workstation.set_ByRef( local_workstation.ConvertToUTF16() );
			}
			local_workstation.set_ByRef( local_workstation.ToUpper() );
			if( local_domain_name.Length() == 0 )
			{
				local_domain_name.set_ByRef( XNetworkInformation::get_HostName( error ) );
				local_domain_name.set_ByRef( local_domain_name.ConvertToUTF16() );
			}
			local_domain_name.set_ByRef( local_domain_name.ToUpper() );
			if( local_user_name.Length() == 0 )
			{
				local_user_name = "guest";
				local_user_name.set_ByRef( local_user_name.ConvertToUTF16() );
			}
			size_t offset = 0;
			size_t size = local_domain_name.Size() - 2;
			_domain_name_header._length = size;
			_domain_name_header._maximum_length = size;
			_domain_name_header._buffer_offset = 72;			
			memcpy( &_data[ offset ] , local_domain_name.str() , size );
			offset += size;

			size = local_user_name.Size() - 2;
			_user_name_header._length = size;
			_user_name_header._maximum_length = size;
			_user_name_header._buffer_offset = 72 + offset;			
			memcpy( &_data[ offset ] , local_user_name.str() , size );
			offset += size;

			size = local_workstation.Size() - 2;
			_workstation_header._length = size;
			_workstation_header._maximum_length = size;
			_workstation_header._buffer_offset = 72 + offset;			
			memcpy( &_data[ offset ] , local_workstation.str() , size );
			offset += size;

			size = 24;
			_lm_challenge_response_header._length = size;
			_lm_challenge_response_header._maximum_length = size;
			_lm_challenge_response_header._buffer_offset = 72 + offset;			
			declare_stack_variable( char , response , 24 );
			declare_stack_variable( char , local_client_challenge , 8 );
			if( NTLMSSP_NEGOTIATE_EXTENDED_SESSIONSECURITY )
			{
				if( client_challenge )
					memcpy( local_client_challenge , client_challenge , 8 );
				else
				{
					void* ptr = malloc( 1 );
					int64 int64_var = (size_t)ptr;
					free( ptr );
					memcpy( local_client_challenge , &int64_var , 8 );
				}
			}
			ntlm::LmChallengeResponse( challenge , password , response , NTLMSSP_NEGOTIATE_EXTENDED_SESSIONSECURITY , local_client_challenge );
			memcpy( &_data[ offset ] , response , size );
			offset += size;

			size = 24;
			_nt_challenge_response_header._length = size;
			_nt_challenge_response_header._maximum_length = size;
			_nt_challenge_response_header._buffer_offset = 72 + offset;			
			ntlm::NtChallengeResponse( challenge , password , response , NTLMSSP_NEGOTIATE_EXTENDED_SESSIONSECURITY , local_client_challenge );
			memcpy( &_data[ offset ] , response , size );
			offset += size;

			_session_key_header._length = 0;
			_session_key_header._maximum_length = 0;
			_session_key_header._buffer_offset = 72 + offset;
						
			 _authenticate_flags = 0xa2888205;

			_ntlm_authenticate_message_size = sizeof( ntlm_authenticate_message ) - sizeof( _data ) - sizeof( _ntlm_authenticate_message_size ) + offset;

			_version._product_major_version = 5;
			_version._product_minor_version = 1;
			_version._product_build = 0x0A28;
			_version._reserved[ 0 ] = 0;
			_version._reserved[ 1 ] = 0;
			_version._reserved[ 2 ] = 0;
			_version._ntlm_revision_current = 0x0F;			
		};
		ntlm_negotiate_message::ntlm_negotiate_message( rux::uint8& is_NTLMSSP_NEGOTIATE_EXTENDED_SESSIONSECURITY )
		{
			memcpy( _Signature , "NTLMSSP" , 8 );
			_MessageType = 1;
			_NegotiateFlags = NTLMSSP_NEGOTIATE_UNICODE | NTLMSSP_REQUEST_TARGET | NTLMSSP_NEGOTIATE_56 | NTLMSSP_NEGOTIATE_EXTENDED_SESSIONSECURITY;
			if( _NegotiateFlags & NTLMSSP_NEGOTIATE_EXTENDED_SESSIONSECURITY )
				is_NTLMSSP_NEGOTIATE_EXTENDED_SESSIONSECURITY = 1;
			else
				is_NTLMSSP_NEGOTIATE_EXTENDED_SESSIONSECURITY = 0;
			_WorkstationDomainHeader = 0;
			_WorkstationNameHeader = 0;
			_version._product_major_version = 5;
			_version._product_minor_version = 1;
			_version._product_build = 0x0A28;
			_version._reserved[ 0 ] = 0;
			_version._reserved[ 1 ] = 0;
			_version._reserved[ 2 ] = 0;
			_version._ntlm_revision_current = 0x0F;			
		};
	};
};
