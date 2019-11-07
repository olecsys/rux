#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_pipe.h"
#include "xan_security.h"
#include "xan_error.h"
#include "xan_keyvaluepair.h"
#include "xan_uint64.h"
#include "xan_log.h"
#include "xan_file_descriptor_waiter.h"
#include "xan_user_data.h"
#include "xan_time.h"
#include "xan_boolean.h"
begin_implement_rux_class_ns( PipeClient , rux );
	_buffer_size = 5;
end_implement_rux_class();
begin_implement_rux_class_ns( PipeServer , rux );
	_async_command_pool.set_MaxThreads( 32 );
	_buffer_size = 5;
	_on_pipe_callback = NULL;
#ifdef __UNIX__
	_request_fifo = -1;	
	_unique = 0;
#endif
end_implement_rux_class();
namespace rux
{	
	void XPipeClient::Send( const ::rux::XString& pipe_name , void* data , size_t data_size , XData& received_data , ::rux::XString& error )
	{
		error.Clear();
		received_data._data_size = 0;
		if( data && data_size > 0 )
		{
	#ifdef __WINDOWS__
			HANDLE pipe = INVALID_HANDLE_VALUE;
			declare_variable( XString , ansi );
			ansi.set_ByRef( "\\\\.\\pipe\\" + pipe_name.ConvertToAnsi() );
			DWORD last_error = 0;
			for( ; ; )
			{ 
				pipe = CreateFileA( ansi.str() , GENERIC_READ | GENERIC_WRITE , 0 , NULL , OPEN_EXISTING , 0 , NULL );
				if( pipe != INVALID_HANDLE_VALUE ) 
					break;
				if( ( last_error = GetLastError() ) != ERROR_PIPE_BUSY ) 
				{
					rux_write_error_2( XError::ConvertErrorCodeToString( last_error , XEnum_ErrorType_GetLastError ) , error , 0 );
					break;
				}
				if( WaitNamedPipeA( ansi.str() , 20000 ) == FALSE )
					break;
			}
			if( pipe != INVALID_HANDLE_VALUE )
			{
				::rux::engine::_globals->_rux_memory_add_descriptor( _rux_current_module_index , __FILE__ , pipe );
				size_t offset = 0 , send_data_size = data_size;
				DWORD written_size = 0;
				while( offset != data_size )
				{
					if( send_data_size > data_size - offset )
						send_data_size = data_size - offset;
					if( WriteFile( pipe , &( (::rux::uint8*)data )[ offset ] , send_data_size , &written_size , NULL ) == TRUE )
					{
						offset += written_size;
					}
					else
					{
						rux_write_error_2( XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError ) , error , 0 );
						break;
					}
				}
				if( offset == data_size )
				{
					void* buffer = alloc_array_macros( ::rux::uint8 , (*this)()->_buffer_size );
					BOOL is_success = FALSE;							
					offset = 0;
					for( ; ; )
					{
						written_size = 0;					
						is_success = ReadFile( pipe , buffer , (*this)()->_buffer_size , &written_size , NULL );
						if( is_success == FALSE )
						{   
							last_error = GetLastError();
							if( last_error == ERROR_MORE_DATA
								|| last_error == ERROR_HANDLE_EOF )
								is_success = TRUE;
							else
								break;
						}
						if( is_success == TRUE )
							received_data.Concatenate( buffer , written_size );
						if( written_size < (*this)()->_buffer_size )
							break;						
						offset += written_size;
					}
					if( offset > (*this)()->_buffer_size )
						(*this)()->_buffer_size = offset;
					::rux::engine::free_mem( buffer );
				}
				CloseHandle( pipe ); 
				::rux::engine::_globals->_rux_memory_remove_descriptor( pipe , _rux_current_module_index );
			}
	#elif defined( __UNIX__ )
			::rux::XString response_fifo_filename , request_fifo_filename;
			request_fifo_filename.set_ByRef( "/tmp/" + pipe_name.ConvertToUTF8() );
			response_fifo_filename.set_ByRef( request_fifo_filename + "_response" );
			if( rux_is_exists_fifo( request_fifo_filename.str() ) == 1 )
			{
				::rux::int32 last_error = 0;
				mode_t old_umask = umask( 0 );
				if( mkfifo( response_fifo_filename.str() , 0666 ) != -1 || ( last_error = errno ) == EEXIST )
				{
					::rux::int32 response_fifo = open( response_fifo_filename.str() , O_RDONLY | O_NONBLOCK , S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH );
					if( response_fifo != -1 )
					{
						int flags = fcntl( response_fifo , F_GETFL , 0 );
						fcntl( response_fifo , F_SETFL , flags | O_NONBLOCK );
						::rux::int32 request_fifo = open( request_fifo_filename.str() , O_WRONLY | O_NONBLOCK , S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH );
						if( request_fifo != -1 )
						{
							int flags = fcntl( request_fifo , F_GETFL , 0 );
							fcntl( request_fifo , F_SETFL , flags | O_NONBLOCK );
							ssize_t total = 0;
							const char* connect = "connect";
							do
							{
								ssize_t res = write( request_fifo , &connect[ total ] , 8 - total );
								if( res > 0 )
									total += res;
								else if( res == -1 && errno != EAGAIN )
									break;
								else
									::rux::threading::XThread::Sleep( 1 );
							}
							while( total != 8 );
							close( request_fifo );
							request_fifo = -1;
							if( total == 8 )
							{
								::rux::io::file_descriptor_waiter fd_waiter;
								fd_waiter.add( response_fifo );
								ssize_t size = 0;	
								::rux::int32 unique = 0;
								ssize_t read_size = 0;
								if( received_data._allocated_size < sizeof( ssize_t ) + sizeof( ::rux::int32 ) )
									received_data.ReAlloc( sizeof( ssize_t ) + sizeof( ::rux::int32 ) );
								do
								{	
									if( fd_waiter.wait( 5 , 1 , 0 ) )
									{
										if( fd_waiter.get_except_fd() != -1 )
										{
											errno_error_to_string_2( errno , error , 0 );
											break;
										}
										::rux::int32 read_fd = fd_waiter.get_read_fd();
										read_size = read( response_fifo , &( (::rux::uint8*)received_data._data )[ received_data._data_size ] , sizeof( ssize_t ) + sizeof( ::rux::int32 ) - received_data._data_size );
										if( read_size > 0 )
											received_data._data_size += read_size;
										else if( read_size == 0 )
											break;
										else
										{
											::rux::int32 err = errno;
											if( err != EAGAIN )
											{
												errno_error_to_string_2( err , error , 0 );
												break;
											}
										}
									}
									else
									{
										errno_error_to_string_2( errno , error , 0 );
										break;
									}
								}
								while( received_data._data_size != sizeof( ssize_t ) + sizeof( ::rux::int32 ) );
								close( response_fifo );
								response_fifo = -1;
								if( error.Length() == 0 )
								{
									if( received_data._data_size == sizeof( ssize_t ) + sizeof( ::rux::int32 ) )
									{
										size = *( (ssize_t*)received_data._data );
										unique = *( (::rux::int32*)&( (char*)received_data._data )[ sizeof( ssize_t ) ] );
										if( size == sizeof( ::rux::int32 ) )
										{
											received_data._data_size = 0;
											declare_stack_variable( char , client_pipe_name_request , 1024 );
											declare_stack_variable( char , client_pipe_name_response , 1024 );
											::rux::safe_sprintf( client_pipe_name_request , 1024 , "/tmp/%s_cli_request%d" , pipe_name.str() , unique );
											::rux::safe_sprintf( client_pipe_name_response , 1024 , "/tmp/%s_cli_response%d" , pipe_name.str() , unique );
											::rux::int32 last_error = 0;
											mode_t old_umask = umask( 0 );
											if( mkfifo( client_pipe_name_response , 0666 ) != -1 || ( last_error = errno ) == EEXIST )
											{
												::rux::int32 client_response_fifo = open( client_pipe_name_response , O_RDONLY | O_NONBLOCK , S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH );
												if( client_response_fifo != -1 )
												{
													int flags = fcntl( client_response_fifo , F_GETFL , 0 );
													fcntl( client_response_fifo , F_SETFL , flags | O_NONBLOCK );
													::rux::int32 client_request_fifo = open( client_pipe_name_request , O_WRONLY | O_NONBLOCK , S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH );
													if( client_request_fifo != -1 )
													{
														int flags = fcntl( client_request_fifo , F_GETFL , 0 );
														fcntl( client_request_fifo , F_SETFL , flags | O_NONBLOCK );
														size = 0;
														do
														{
															read_size = write( client_request_fifo , &( (char*)data )[ size ] , data_size - size );
															if( read_size > 0 )
																size += read_size;
															else if( read_size == -1 && errno != EAGAIN )
															{
																::rux::log::WriteTrace( "%s, %s:%d" , __FUNCTION__ , __FILE__ , __LINE__ );
																break;
															}
															else
																::rux::threading::XThread::Sleep( 1 );
														}
														while( size != data_size );
														::rux::log::WriteTrace( "%s, %s:%d, client send %u bytes" , __FUNCTION__ , __FILE__ , __LINE__ , (::rux::uint32)size );
														close( client_request_fifo );
														client_request_fifo = -1;
														if( size == data_size )
														{
															fd_waiter.clear();
															fd_waiter.add( client_response_fifo );
															size = 0;
															for( ; ; )
															{
																if( fd_waiter.wait( 100 , 1 , 0 ) )
																{
																	if( fd_waiter.get_except_fd() != -1 )
																	{
																		::rux::log::WriteTrace( "%s, %s:%d" , __FUNCTION__ , __FILE__ , __LINE__ );
																		break;
																	}
																	if( fd_waiter.get_read_fd() != -1 )
																	{																																			
																		if( received_data._allocated_size - received_data._data_size < 1024 )
																			received_data.ReAlloc( received_data._allocated_size + 1024 );
																		read_size = read( client_response_fifo , &( (char*)received_data._data )[ received_data._data_size ] , received_data._allocated_size - received_data._data_size );
																		if( read_size > 0 )
																			received_data._data_size += read_size;
																		else if( read_size == 0 )
																		{
																			::rux::log::WriteTrace( "%s, %s:%d" , __FUNCTION__ , __FILE__ , __LINE__ );
																			break;
																		}
																		else
																		{
																			::rux::int32 err = errno;
																			if( err != EAGAIN )
																			{
																				errno_error_to_string_2( err , error , 0 );
																				break;
																			}
																		}
																	}
																	else
																		break;
																}
																else
																{
																	errno_error_to_string_2( errno , error , 0 );
																	break;
																}
															}
															close( client_response_fifo );
															client_response_fifo = -1;
														}
														else
														{
															rux_write_error_2( "server is disconnected" , error , 0 );
														}
														if( client_request_fifo != -1 )
															close( client_request_fifo );
													}
													else
													{
														errno_error_to_string_2( errno , error , 0 );
													}
													if( client_response_fifo != -1 )
														close( client_response_fifo );
												}
												else
												{
													errno_error_to_string_2( errno , error , 0 );
												}
												unlink( client_pipe_name_response );
											}
											else
											{
												errno_error_to_string_2( errno , error , 0 );
											}
											umask( old_umask );
										}
										else
										{
											rux_write_error_2( "pipe server response is incorrect , size is incorrect" , error , 0 );
										}
									}
									else
									{
										received_data._data_size = 0;
										rux_write_error_2( "pipe server response is incorrect , packet size is incorrect" , error , 0 );
									}	
								}					
							}
							else
							{
								rux_write_error_2( "data was written insufficiently" , error , 0 );
							}
							if( request_fifo != -1 )
								close( request_fifo );
						}
						else
						{
							errno_error_to_string_2( errno , error , 0 );
						}
						if( response_fifo != -1 )
							close( response_fifo );
					}
					else
					{
						errno_error_to_string_2( errno , error , 0 );
					}
				}
				else
				{
					errno_error_to_string_2( last_error , error , 0 );
				}
				umask( old_umask );
			}
			else
			{
				int errnoint = errno;
				if(errnoint != 0)
				{
					errno_error_to_string_2(errnoint, error, 0);
				}
				else
				{
					rux_write_error_2( "request fifo(" + request_fifo_filename + ") does not exist" , error , 0 );
				}
			}
	#endif
		}
	};
	PipeServer::~PipeServer()
	{
		StopListen();
	};
	void XPipeServer::set_OnReceived( rux_on_pipe_callback on_pipe_callback )
	{
		(*this)()->_on_pipe_callback = on_pipe_callback;
	};
	implement_duplicate_internal_function_2( PipeServer , StartListen , const ::rux::XString& , ::rux::XString& );
	void PipeServer::StartListen( const ::rux::XString& pipe_name , ::rux::XString& error )
	{	
		error.Clear();
		if( _pipe_name != pipe_name )
			StopListen();
		CRITICAL_SECTION_LOCK( _cs_listen_thread );
		if( _listen_thread.get_IsStarted() == 0 )
		{
	#ifdef __UNIX__		
			::rux::XString request_fifo_filename;
			request_fifo_filename.set_ByRef( "/tmp/" + pipe_name.ConvertToUTF8() );	
			::rux::XString response_fifo_filename;
			response_fifo_filename.set_ByRef( request_fifo_filename + "_response" );
			::rux::int32 last_error = 0;
			mode_t old_umask = umask( 0 );
			if( mkfifo( request_fifo_filename.str() , 0666 ) != -1 || ( last_error = errno ) == EEXIST )
			{
				if( mkfifo( response_fifo_filename.str() , 0666 ) != -1 || ( last_error = errno ) == EEXIST )
				{
					_request_fifo = open( request_fifo_filename.str() , O_RDONLY | O_NONBLOCK , S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH );
					if( _request_fifo != -1 )
					{
						int flags = fcntl( _request_fifo , F_GETFL , 0 );
						fcntl( _request_fifo , F_SETFL , flags | O_NONBLOCK );
						_pipe_name.set_ByRef( request_fifo_filename );
	#endif
	#ifdef __WINDOWS__
			{
				{
					{
						_pipe_name.set_ByRef( "\\\\.\\pipe\\" + pipe_name.ConvertToAnsi() );
	#endif
						_listen_thread.set_ThreadMode( XThreadMode_Repeat );
						_listen_thread.set_ThreadName( "pipe listen" );
						_listen_thread.set_Timeout( 10 );
						_listen_thread.Start();
						_listen_thread.Push( XPipeServer::listen_thread , this );
					}
	#ifdef __UNIX__
					else
					{
						errno_error_to_string_2( errno , error , 0 );
					}
				}
				else
				{
					errno_error_to_string_2( last_error , error , 0 );
	#endif
				}
			}		
	#ifdef __UNIX__
			else
			{
				errno_error_to_string_2( last_error , error , 0 );
			}
			if( error.Length() > 0 )
				StopListen();
			umask( old_umask );
	#endif
		}	
		CRITICAL_SECTION_UNLOCK( _cs_listen_thread );
	};
	implement_duplicate_internal_function( PipeServer , StopListen );
	void PipeServer::StopListen( void )
	{	
		CRITICAL_SECTION_LOCK( _cs_listen_thread );
		if( _listen_thread.get_IsStarted() == 1 )
			_listen_thread.Stop();
		CRITICAL_SECTION_UNLOCK( _cs_listen_thread );
	#ifdef __UNIX__
		if( _request_fifo != -1 )
		{
			close( _request_fifo );
			_request_fifo = -1;
			::rux::XString request_fifo_filename;
			request_fifo_filename.set_ByRef( _pipe_name.ConvertToUTF8() );
			unlink( request_fifo_filename.str() );
		}
		::rux::XString response_fifo_filename;
		response_fifo_filename.set_ByRef( _pipe_name.ConvertToUTF8() + "_response" );
		unlink( response_fifo_filename.str() );
	#endif
	};
	void XPipeServer::listen_thread( void* param , size_t ___rux__thread_index1986 )
	{	
		CODE_LABEL( NULL , NULL , 100 );
		PipeServer* pipe_server = (PipeServer*)param;		
		if( pipe_server )
		{
			::rux::threading::XThread current_thread;
			current_thread = ::rux::threading::XThread::get_CurrentThread();
	#ifdef __WINDOWS__
			::rux::uint8 is_connected = 0;
			HANDLE pipe = INVALID_HANDLE_VALUE;		
			DWORD ignore = 0;
			OVERLAPPED overlapped = { 0 , 0 , 0 , 0 , NULL };
			::rux::uint32 timeout_index = 0;
			overlapped.hEvent = CreateEventA( NULL , TRUE , FALSE , NULL );
			::rux::engine::_globals->_rux_memory_add_descriptor( _rux_current_module_index , __FILE__ , overlapped.hEvent );
			::rux::uint32 buffer_size = 0;
			::rux::XString error;
			XSecurityData security_data;
			XArray< XString > user_or_group_array;
			::rux::uint8* is_allow_array = NULL;
			DWORD* access_mask_array = NULL;
			if( pipe_server->_allow_user_or_group.Count() + pipe_server->_deny_user_or_group.Count() > 0 )
			{
				is_allow_array = alloc_array_macros( ::rux::uint8 , pipe_server->_allow_user_or_group.Count() + pipe_server->_deny_user_or_group.Count() );
				access_mask_array = alloc_array_macros( DWORD , pipe_server->_allow_user_or_group.Count() + pipe_server->_deny_user_or_group.Count() );
			}		
			for( ::rux::uint32 index0 = 0 ; index0 < pipe_server->_allow_user_or_group.Count() ; index0++ )
			{
				is_allow_array[ index0 ] = 1;
				access_mask_array[ index0 ] = FILE_ALL_ACCESS;			
				user_or_group_array.AddByRef( pipe_server->_allow_user_or_group[ index0 ] );
			}		
			for( ::rux::uint32 index0 = 0 ; index0 < pipe_server->_deny_user_or_group.Count() ; index0++ )
			{
				is_allow_array[ pipe_server->_allow_user_or_group.Count() + index0 ] = 0;
				access_mask_array[ pipe_server->_allow_user_or_group.Count() + index0 ] = FILE_ALL_ACCESS;			
				user_or_group_array.AddByRef( pipe_server->_deny_user_or_group[ index0 ] );
			}
			XSystemSecurity::AddAclToSecurityDescriptor( user_or_group_array , is_allow_array , access_mask_array , security_data , error );		
			for( ; ; ) 
			{			
				overlapped.Internal = 0;
				overlapped.InternalHigh = 0;
				overlapped.Offset = 0;
				overlapped.OffsetHigh = 0;
				overlapped.Pointer = NULL;
				ResetEvent( overlapped.hEvent );
				if( current_thread.get_IsWaitForExit() == 1 )
					break;
				CRITICAL_SECTION_LOCK( pipe_server->_cs_buffer_size );
				buffer_size = pipe_server->_buffer_size;
				CRITICAL_SECTION_UNLOCK( pipe_server->_cs_buffer_size );
				if( pipe == INVALID_HANDLE_VALUE )
				{
					pipe = CreateNamedPipeA( pipe_server->_pipe_name.str() , PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED , PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT , PIPE_UNLIMITED_INSTANCES , buffer_size , buffer_size , 20000 , security_data._security_attributes );
					if( pipe != INVALID_HANDLE_VALUE )
						::rux::engine::_globals->_rux_memory_add_descriptor( _rux_current_module_index , __FILE__ , pipe );
				}
				if( pipe != INVALID_HANDLE_VALUE ) 
				{					
					is_connected = ConnectNamedPipe( pipe , &overlapped ) ? 1 : ( GetLastError() == ERROR_PIPE_CONNECTED ? 1 : 0 ); 
					if( is_connected == 0 ) 
					{
						switch( GetLastError() )
						{
						case ERROR_PIPE_CONNECTED:
							{						
								is_connected = 1;	 
								break;
							}
						case ERROR_IO_PENDING:
							{
								timeout_index = 0;
								while( timeout_index != 19 && ( ignore = WaitForSingleObject( overlapped.hEvent, 1000 ) ) != WAIT_OBJECT_0 ) 
								{
									if( current_thread.get_IsWaitForExit() == 1 )
										break;
									timeout_index++;
								}
								if( ignore == WAIT_OBJECT_0 )
									is_connected = GetOverlappedResult( pipe , &overlapped , &ignore , FALSE ) == TRUE ? 1 : 0;
								else
									CancelIo( pipe );
								break;
							}
						}
					}
					if( is_connected == 1 ) 
					{
						HANDLE old_pipe = pipe;
						CRITICAL_SECTION_LOCK( pipe_server->_cs_buffer_size );
						buffer_size = pipe_server->_buffer_size;
						CRITICAL_SECTION_UNLOCK( pipe_server->_cs_buffer_size );
						pipe = CreateNamedPipeA( pipe_server->_pipe_name.str() , PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED , PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT , PIPE_UNLIMITED_INSTANCES , buffer_size , buffer_size , 20000 , security_data._security_attributes );
						if( pipe != INVALID_HANDLE_VALUE )
							::rux::engine::_globals->_rux_memory_add_descriptor( _rux_current_module_index , __FILE__ , pipe );
						pipe_server_struct* pipe_server_object = ::rux::engine::alloc_object< pipe_server_struct >();
						pipe_server_object->_pipe_server = pipe_server;
						pipe_server_object->_pipe = old_pipe;
						pipe_server->_async_command_pool.Execute( client_thread , pipe_server_object );
					} 
					else 
					{
						CloseHandle( pipe ); 
						::rux::engine::_globals->_rux_memory_remove_descriptor( pipe , _rux_current_module_index );
						pipe = INVALID_HANDLE_VALUE;
					}
				} 
				else
				{
					rux_write_error_2( XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError ) , error , 0 );
				}
			}		
			if( is_allow_array )
				::rux::engine::free_mem( is_allow_array );
			if( access_mask_array )
				::rux::engine::free_mem( access_mask_array );
			if( pipe != INVALID_HANDLE_VALUE )
			{
				CloseHandle( pipe ); 
				::rux::engine::_globals->_rux_memory_remove_descriptor( pipe , _rux_current_module_index );
				pipe = INVALID_HANDLE_VALUE;
			}
			CloseHandle( overlapped.hEvent );
			::rux::engine::_globals->_rux_memory_remove_descriptor( overlapped.hEvent , _rux_current_module_index );
	#elif defined( __UNIX__ )
			declare_stack_variable( char , response , sizeof( ssize_t ) + sizeof( ::rux::int32 ) );
			ssize_t size = sizeof( ::rux::int32 );
			::rux::engine::memcpy( response , 0 , &size , sizeof( ssize_t ) );
			::rux::XString response_fifo_filename;
			response_fifo_filename.set_ByRef( pipe_server->_pipe_name + "_response" );
			::rux::io::file_descriptor_waiter fd_waiter;
			fd_waiter.add( pipe_server->_request_fifo );
			do
			{
				if( fd_waiter.wait( 100 , 1 , 0 ) )
				{				
					if( fd_waiter.get_read_fd() != -1 )
					{					
						char connect[ 8 ] = { 0 };
						ssize_t total = 0;
						do
						{
							ssize_t res = read( pipe_server->_request_fifo , &connect[ total ] , 8 - total );
							if( res > 0 )
								total += res;
							else if( res == 0 )
								break;
							else if( res == -1 && errno != EAGAIN )
							{
								::rux::log::WriteTrace( "%s, %s:%d" , __FUNCTION__ , __FILE__ , __LINE__ );
								break;
							}
						}
						while( total != 8 );
						if( total == 8 )
						{
							if( memcmp( connect , "connect" , 8 ) == 0 )
							{	
								::rux::int32 server_response_fifo = open( response_fifo_filename.str() , O_WRONLY | O_NONBLOCK , S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH );
								if( server_response_fifo != -1 )
								{
									int flags = fcntl( server_response_fifo , F_GETFL , 0 );
									fcntl( server_response_fifo , F_SETFL , flags | O_NONBLOCK );
									
									::rux::int32 last_error = 0;
									declare_stack_variable( char , client_pipe_name_request , 1024 );
									::rux::safe_sprintf( client_pipe_name_request , 1024 , "%s_cli_request%d" , pipe_server->_pipe_name.str() , pipe_server->_unique );
									mode_t old_umask = umask( 0 );
									if( mkfifo( client_pipe_name_request , 0666 ) != -1 || ( last_error = errno ) == EEXIST )
									{
										::rux::int32 client_request = open( client_pipe_name_request , O_RDONLY | O_NONBLOCK , S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH );
										umask( old_umask );
										if( client_request != -1 )
										{
											int flags = fcntl( client_request , F_GETFL , 0 );
											fcntl( client_request , F_SETFL , flags | O_NONBLOCK );
											::rux::engine::memcpy( response , sizeof( ssize_t ) , &pipe_server->_unique , sizeof( ::rux::int32 ) );
											ssize_t total_write_res = 0;
											do
											{
												ssize_t write_res = write( server_response_fifo , &response[ total_write_res ] , sizeof( ssize_t ) + sizeof( ::rux::int32 ) - total_write_res );
												if( write_res > 0 )
													total_write_res += write_res;
												else if( write_res == -1 && errno != EAGAIN )
												{
													::rux::log::WriteTrace( "%s, %s:%d" , __FUNCTION__ , __FILE__ , __LINE__ );
													break;
												}
												else
													::rux::threading::XThread::Sleep( 1 );
											}
											while( total_write_res != sizeof( ssize_t ) + sizeof( ::rux::int32 ) );
											if( total_write_res == sizeof( ssize_t ) + sizeof( ::rux::int32 ) )
											{
												pipe_server_struct* pipe_server_object = ::rux::engine::alloc_object< pipe_server_struct >();
												pipe_server_object->_unique = pipe_server->_unique;
												pipe_server_object->_pipe_server = pipe_server;
												pipe_server_object->_client_request = client_request;
												client_request = -1;
												pipe_server->_async_command_pool.Execute( client_thread , pipe_server_object );
												pipe_server->_unique++;
											}
											else
											{
												declare_variable( ::rux::XString , error_string );
												rux_write_error_2( "data was written insufficiently" , error_string , 0 );
											}
										}
										else
										{
											errno_error;
										}
										if( client_request != -1 )
										{
											close( client_request );
											unlink( client_pipe_name_request );
										}
									}
									else
									{
										declare_variable( ::rux::XString , error_string );
										errno_error_to_string_2( last_error , error_string , 0 );
									}
									close( server_response_fifo );
								}
								else
								{
									errno_error;
								}
							}
						}					
					}
				}
				else
					break;
			}
			while( current_thread.get_IsWaitForExit() == 0 );
	#endif
			pipe_server->_async_command_pool.Stop();
		}
	};
	void XPipeServer::client_thread( void* param , size_t ___rux__thread_index1986 )
	{	
		CODE_LABEL( NULL , NULL , 100 );
		pipe_server_struct* pipe_server_object = (pipe_server_struct*)param;		
		if( pipe_server_object )
		{
			::rux::threading::XThread current_thread( ::rux::threading::XThread::get_CurrentThread() );
	#ifdef __WINDOWS__
			CRITICAL_SECTION_LOCK( pipe_server_object->_pipe_server->_cs_buffer_size );
			::rux::uint32 buffer_size = pipe_server_object->_pipe_server->_buffer_size;
			CRITICAL_SECTION_UNLOCK( pipe_server_object->_pipe_server->_cs_buffer_size );		
			void* buffer = alloc_array_macros( ::rux::uint8 , buffer_size );
			BOOL is_success = FALSE;
			DWORD readed_bytes = 0 , last_error = 0;
			XData received_data , send_data;		
			::rux::uint32 timeout_index = 0;
			size_t offset = 0;
			for( ; ; )
			{	
				if( current_thread.get_IsWaitForExit() == 1 )
					break;
				readed_bytes = 0;
				if( PeekNamedPipe( pipe_server_object->_pipe , NULL , NULL , NULL , &readed_bytes , NULL ) )
				{
					if( readed_bytes > 0 )
					{
						is_success = ReadFile( pipe_server_object->_pipe , buffer , buffer_size , &readed_bytes , NULL );
						if( is_success == FALSE )
						{   
							last_error = GetLastError();
							if( last_error == ERROR_MORE_DATA )
								is_success = TRUE;
							else
								break;
						}
						if( is_success == TRUE )
							received_data.Concatenate( buffer , readed_bytes );		
						offset += readed_bytes;
						if( readed_bytes < buffer_size )
							break;
						else if( PeekNamedPipe( pipe_server_object->_pipe , NULL , NULL , NULL , &readed_bytes , NULL ) )
						{
							if( readed_bytes == 0 )
								break;
						}					
					}
					else
					{
						timeout_index++;
						if( timeout_index == 500 )
							break;
						else
							::rux::threading::XThread::Sleep( 1 );
					}
				}
				else
				{
					timeout_index++;
					if( timeout_index == 500 )
						break;
					else
						::rux::threading::XThread::Sleep( 1 );
				}
			}
			if( pipe_server_object->_pipe_server->_buffer_size < offset )
				pipe_server_object->_pipe_server->_buffer_size = offset;
			if( is_success == TRUE )
			{
				if( pipe_server_object->_pipe_server->_on_pipe_callback )
					pipe_server_object->_pipe_server->_on_pipe_callback( received_data , send_data , NULL , NULL );
				if( send_data._data )
				{
					readed_bytes = 0 , offset = 0;
					size_t send_data_size = send_data._data_size;
					while( offset != send_data._data_size )
					{
						if( send_data_size > send_data._data_size - offset )
							send_data_size = send_data._data_size - offset;
						if( WriteFile( pipe_server_object->_pipe , &( (::rux::uint8*)send_data._data )[ offset ] , send_data_size , &readed_bytes , NULL ) == TRUE )
							offset += readed_bytes;
						else
						{
							last_error = GetLastError();
							break;
						}
					}
				}
			}
			FlushFileBuffers( pipe_server_object->_pipe ); 
			DisconnectNamedPipe( pipe_server_object->_pipe ); 
			CloseHandle( pipe_server_object->_pipe ); 
			::rux::engine::_globals->_rux_memory_remove_descriptor( pipe_server_object->_pipe , _rux_current_module_index );
			::rux::engine::free_mem( buffer );
	#elif defined( __UNIX__ )
			declare_stack_variable( char , client_pipe_name_response , 1024 );
			::rux::safe_sprintf( client_pipe_name_response , 1024 , "%s_cli_response%d" , pipe_server_object->_pipe_server->_pipe_name.str() , pipe_server_object->_unique );
			declare_stack_variable( char , client_pipe_name_request , 1024 );
			::rux::safe_sprintf( client_pipe_name_request , 1024 , "%s_cli_request%d" , pipe_server_object->_pipe_server->_pipe_name.str() , pipe_server_object->_unique );
			CRITICAL_SECTION_LOCK( pipe_server_object->_pipe_server->_cs_buffer_size );
			XData recv_data( pipe_server_object->_pipe_server->_buffer_size ) , send_data;
			CRITICAL_SECTION_UNLOCK( pipe_server_object->_pipe_server->_cs_buffer_size );
			::rux::int32 last_error = 0;
			::rux::io::file_descriptor_waiter fd_waiter;
			fd_waiter.add( pipe_server_object->_client_request );
			::rux::int32 recv_data_size = 0 , total_recv_data_size = 0;
			::rux::uint8 is_client_disconnect = 0;
			::rux::XString error;
			::rux::uint32 read_tickcount = ::rux::XTime::GetTickCount();
			do
			{	
				::rux::uint32 now = ::rux::XTime::GetTickCount();
				if( read_tickcount > now )
					read_tickcount = now;
				if( now - read_tickcount >= 1000 )
				{
					::rux::log::WriteTrace( "%s, %s:%d" , __FUNCTION__ , __FILE__ , __LINE__ );
					break;
				}
				for( ; ; )
				{
					if( fd_waiter.wait( 5 , 1 , 0 ) )
					{
						if( fd_waiter.get_except_fd() != -1 )
						{
							is_client_disconnect = 1;
							::rux::log::WriteTrace( "%s, %s:%d" , __FUNCTION__ , __FILE__ , __LINE__ );
							break;
						}
						::rux::int32 read_fd = fd_waiter.get_read_fd();
						rux::int32 ioctl_size = 0;
						if( read_fd != -1 && ioctl( pipe_server_object->_client_request , FIONREAD , &ioctl_size ) != -1 ) 
						{
							::rux::log::WriteTrace( "%s, %s:%d, ioctl %d bytes" , __FUNCTION__ , __FILE__ , __LINE__ , ioctl_size );
							if( recv_data._allocated_size - recv_data._data_size < ioctl_size )
								recv_data.ReAlloc( recv_data._allocated_size + ioctl_size );
						}
						else
						{
							if( recv_data._allocated_size - recv_data._data_size < 1024 )
								recv_data.ReAlloc( recv_data._allocated_size + 1024 );
						}
						recv_data_size = read( pipe_server_object->_client_request , &( (char*)recv_data._data )[ recv_data._data_size ] , recv_data._allocated_size - recv_data._data_size );
						if( recv_data_size > 0 )
						{
							read_tickcount = ::rux::XTime::GetTickCount();
							recv_data._data_size += recv_data_size;
						}
						else if( recv_data_size != -1 || errno != EAGAIN )
						{
							is_client_disconnect = 1;
							::rux::log::WriteTrace( "%s, %s:%d" , __FUNCTION__ , __FILE__ , __LINE__ );
							break;
						}
					}
					else
					{
						is_client_disconnect = 1;
						::rux::log::WriteTrace( "%s, %s:%d" , __FUNCTION__ , __FILE__ , __LINE__ );
						break;
					}
					now = ::rux::XTime::GetTickCount();
					if( read_tickcount > now )
						read_tickcount = now;
					if( now - read_tickcount >= 1000 )
					{
						::rux::log::WriteTrace( "%s, %s:%d" , __FUNCTION__ , __FILE__ , __LINE__ );
						break;
					}
				}
				::rux::log::WriteTrace( "%s, %s:%d, server received %u bytes" , __FUNCTION__ , __FILE__ , __LINE__ , (::rux::uint32)recv_data._data_size );
				if( pipe_server_object->_pipe_server->_on_pipe_callback && recv_data._data_size > 0 )
				{
					pipe_server_object->_pipe_server->_on_pipe_callback( recv_data , send_data , NULL , NULL );	
					char empty_response = 0;
					char* send_data_ptr = &empty_response;
					size_t send_data_ptr_size = 1;
					if( send_data._data_size > 0 )
					{
						send_data_ptr = (char*)send_data._data;
						send_data_ptr_size = send_data._data_size;
					}
					mode_t old_umask = umask( 0 );
					::rux::int32 client_response = open( client_pipe_name_response , O_WRONLY | O_NONBLOCK , S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH );
					umask( old_umask );
					if( client_response != -1 )
					{
						int flags = fcntl( client_response , F_GETFL , 0 );
						fcntl( client_response , F_SETFL , flags | O_NONBLOCK );
						total_recv_data_size = 0;
						do
						{
							recv_data_size = write( client_response , &send_data_ptr[ total_recv_data_size ] , send_data_ptr_size - total_recv_data_size );
							if( recv_data_size > 0 )
								total_recv_data_size += recv_data_size;
							else if( recv_data_size == -1 && errno != EAGAIN )
							{
								is_client_disconnect = 1;
								::rux::log::WriteTrace( "%s, %s:%d" , __FUNCTION__ , __FILE__ , __LINE__ );
								break;
							}
							else
								::rux::threading::XThread::Sleep( 1 );
						}
						while( total_recv_data_size != send_data_ptr_size );
						close( client_response );
					}
					else
					{
						is_client_disconnect = 1;
						errno_error;
					}
					send_data._data_size = 0;
				}
				else
					is_client_disconnect = 1;								
				CRITICAL_SECTION_LOCK( pipe_server_object->_pipe_server->_cs_buffer_size );
				if( recv_data._allocated_size > pipe_server_object->_pipe_server->_buffer_size )
					pipe_server_object->_pipe_server->_buffer_size = recv_data._allocated_size;
				CRITICAL_SECTION_UNLOCK( pipe_server_object->_pipe_server->_cs_buffer_size );	
				if( is_client_disconnect == 1 )
					break;
			}
			while( current_thread.get_IsWaitForExit() == 0 );
			close( pipe_server_object->_client_request );
			unlink( client_pipe_name_request );
	#endif
			::rux::engine::free_object< pipe_server_struct >( pipe_server_object );
		}
	};
	void XPipeServer::AddAllAllowAccessUserOrGroup( const ::rux::XString& user_or_group )
	{
		(*this)()->_allow_user_or_group.Add( user_or_group );
	};
	void XPipeServer::AddAllDenyAccessUserOrGroup( const ::rux::XString& user_or_group )
	{
		(*this)()->_deny_user_or_group.Add( user_or_group );
	};
};
