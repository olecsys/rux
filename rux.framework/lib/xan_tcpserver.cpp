#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_tcpserver.h>
#include <xan_error.h>
#include <xan_keyvaluepair.h>
#include <xan_network_information.h>
#include <xan_uint64.h>
#include <xan_file_descriptor_waiter.h>
#include <xan_network_functions.h>
#include <xan_boolean.h>
begin_implement_rux_class_ns( TcpServer , rux::network );
	_buffer_size = 1024;
	_threads_count = 0;
	_on_receive_callback = NULL;
	_server_sockets = NULL;
	_server_sockets_count = 0;
end_implement_rux_class();
namespace rux
{
	namespace network
	{		
#ifdef __WINDOWS__
#define TCP_SERVER_ERROR() error = XError::ConvertErrorCodeToString( ::rux::engine::_globals->_ws2_32_module.WSAGetLastError() , XEnum_ErrorType_GetLastError )
#else
#define TCP_SERVER_ERROR() error = XError::ConvertErrorCodeToString( errno , XEnum_ErrorType_Errno )
#endif
		TcpServer::~TcpServer()
		{
			CRITICAL_SECTION_LOCK( _cs_listen_thread );
			if( _listen_thread.get_IsStarted() == 1 )
				_listen_thread.Stop();
			CRITICAL_SECTION_UNLOCK( _cs_listen_thread );
			while( XInterlocked::CompareExchange( &_threads_count , 0 , 0 ) != 0 )
			{
				rux::threading::XThread::Sleep( 1 );
			}
		};
		void XTcpServer::set_OnReceived( rux_on_pipe_callback on_receive_callback )
		{
			(*this)()->_on_receive_callback = on_receive_callback;
		};
		void XTcpServer::StartListen( const rux::network::XAddress& network_address , rux::XString& error )
		{
			(*this)()->StartListen( network_address , error );
		};
		void TcpServer::StartListen( const rux::network::XAddress& network_address , rux::XString& error)
		{
			error.Clear();
			if( _network_address()->_address != network_address()->_address 
				|| _network_address()->_port != network_address()->_port )
			{
				_network_address = network_address;
				StopListen();
			}
			CRITICAL_SECTION_LOCK( _cs_listen_thread );
			if( _listen_thread.get_IsStarted() == 0 )
			{
				rux_network_init();
				rux::XArray< rux::XString > addresses;
				if( _network_address()->_address == "0.0.0.0" )
					addresses.set_ByRef( XNetworkInformation::get_IPAddresses( error ) );
				else
					addresses.Add( _network_address()->_address );
				rux::XArray< XAddress > network_addresses;
				for( size_t index0 = 0 ; index0 < addresses.Count() ; index0++ )
				{
					XAddress nw( addresses[ index0 ] , _network_address()->_port , error );
					if( error.Length() == 0 )
					{
						if( _network_address()->_network_address_version == nw()->_network_address_version )
							network_addresses.Add( nw );
					}
					else
						error.Clear();
				}
				if( network_addresses.Count() )
				{
					rux::int32 af = AF_INET;
					if( _network_address()->_network_address_version == XEnum_NetworkAddressVersion_IPv6 )		
						af = AF_INET6;
					_server_sockets = alloc_array_macros( SOCKET , network_addresses.Count() );
					for( size_t index0 = 0 ; index0 < network_addresses.Count() ; index0++ )
					{
						_server_sockets[ index0 ] = ::rux::network::socket( af , SOCK_STREAM , IPPROTO_TCP );
						if( _server_sockets[ index0 ] != INVALID_SOCKET )
						{
							_server_sockets_count++;
							rux::int32 is_reuseaddr = 1;
							if( ::rux::network::setsockopt( _server_sockets[ index0 ] , SOL_SOCKET , SO_REUSEADDR , (char*)&is_reuseaddr , sizeof( is_reuseaddr ) ) != -1 )
							{								
								if( ( _network_address()->_network_address_version == XEnum_NetworkAddressVersion_IPv4 && ::rux::network::bind( _server_sockets[ index0 ] , (sockaddr*)&network_addresses[ index0 ].sockaddr_in_object , sizeof( sockaddr_in ) ) == 0 )
									|| ( _network_address()->_network_address_version == XEnum_NetworkAddressVersion_IPv6 && ::rux::network::bind( _server_sockets[ index0 ] , (sockaddr*)&network_addresses[ index0 ].sockaddr_in6_object , sizeof( sockaddr_in6 ) ) == 0 ) )
								{
#ifdef __WINDOWS__
									DWORD bytes_returned = 0;
									BOOL new_behavior = FALSE;
									::rux::engine::_globals->_ws2_32_module.WSAIoctl( _server_sockets[ index0 ] , _WSAIOW( IOC_VENDOR , 12 ) , &new_behavior , sizeof( new_behavior ) , NULL , 0 , &bytes_returned , NULL , NULL );
#endif													
									if( ::rux::network::listen( _server_sockets[ index0 ] , SOMAXCONN ) != 0 )
										TCP_SERVER_ERROR();
								}
								else
									TCP_SERVER_ERROR();
							}
							else
								TCP_SERVER_ERROR();
						}	
						else
							TCP_SERVER_ERROR();
					}
					if( error.Length() == 0 )
					{
						_listen_thread.set_ThreadMode( XThreadMode_Repeat );
						_listen_thread.set_ThreadName( "tcp listen" );
						_listen_thread.set_Timeout( 10 );
						_listen_thread.Start();
						_listen_thread.Push( XTcpServer::listen_thread , this );	
					}
				}				
			}
			CRITICAL_SECTION_UNLOCK( _cs_listen_thread );
			if( error.Length() > 0 )
			{
				for( size_t index0 = 0 ; index0 < _server_sockets_count ; index0++ )
#ifdef __WINDOWS__
					::rux::engine::_globals->_ws2_32_module.closesocket( _server_sockets[ index0 ] );
#else				
					close( _server_sockets[ index0 ] );
#endif
				rux::engine::free_mem( _server_sockets );
				_server_sockets = NULL;
				_server_sockets_count = 0;
			}
		};
		void XTcpServer::StopListen( void )
		{
			(*this)()->StopListen();
		};
		void TcpServer::StopListen( void )
		{	
			CRITICAL_SECTION_LOCK( _cs_listen_thread );
			if( _listen_thread.get_IsStarted() == 1 )
				_listen_thread.Stop();
			CRITICAL_SECTION_UNLOCK( _cs_listen_thread );
			while( XInterlocked::CompareExchange( &_threads_count , 0 , 0 ) != 0 )
				rux::threading::XThread::Sleep( 1 );
		};
		void XTcpServer::listen_thread( void* param , size_t ___rux__thread_index1986 )
		{	
			UNREFERENCED_PARAMETER( ___rux__thread_index1986 );
			TcpServer* tcp_server = (TcpServer*)param;		
			if( tcp_server )
			{
				rux::threading::XThread current_thread;
				current_thread = rux::threading::XThread::get_CurrentThread();
				XEnum_NetworkAddressVersion network_address_version = tcp_server->_network_address()->_network_address_version;
				XNetworkClient* network_client = rux::engine::alloc_object< XNetworkClient >();
				struct sockaddr* sockaddr_pointer = NULL;
				if( network_address_version == XEnum_NetworkAddressVersion_IPv4 )
				{
					sockaddr_pointer = (sockaddr*)&network_client->_sockaddr_in_object;
					network_client->_sockaddr_size = sizeof( network_client->_sockaddr_in_object );
				}
				else if( network_address_version == XEnum_NetworkAddressVersion_IPv6 )
				{
					sockaddr_pointer = (sockaddr*)&network_client->_sockaddr_in6_object;
					network_client->_sockaddr_size = sizeof( network_client->_sockaddr_in6_object );
				}	
				current_thread()->AddRef();
				network_client->_main_thread = current_thread();
				network_client->_udata = tcp_server;
				::rux::io::file_descriptor_waiter fd_waiter;
				for( size_t index0 = 0 ; index0 < tcp_server->_server_sockets_count ; index0++ )
					fd_waiter.add( tcp_server->_server_sockets[ index0 ] );
				SOCKET socket = INVALID_SOCKET;
				rux::XString error;
				do
				{
					if( fd_waiter.wait( 1000 , 1 , 0 ) )
					{
						socket = fd_waiter.get_read_fd();
						if( socket != INVALID_SOCKET )
						{
							if( ( network_client->_socket = ::rux::network::accept( socket , sockaddr_pointer , &network_client->_sockaddr_size ) ) != INVALID_SOCKET )
							{
								socklen_t intern_sockaddr_size = 0;
								struct sockaddr_in intern_sockaddr_in;
								struct sockaddr_in6 intern_sockaddr_in6;
								if( network_address_version == XEnum_NetworkAddressVersion_IPv4 )	
								{
									intern_sockaddr_size = sizeof( sockaddr_in );
									::rux::network::getsockname( socket , (sockaddr*)&intern_sockaddr_in , &intern_sockaddr_size );
									network_client->_extern_ip.set_ByRef( XNetworkInformation::ToString( sockaddr_pointer , intern_sockaddr_size , error ) );
									network_client->_intern_ip.set_ByRef( XNetworkInformation::ToString( (sockaddr*)&intern_sockaddr_in , intern_sockaddr_size , error ) );
								}										
								else if( network_address_version == XEnum_NetworkAddressVersion_IPv6 )	
								{
									intern_sockaddr_size = sizeof( sockaddr_in6 );
									::rux::network::getsockname( socket , (sockaddr*)&intern_sockaddr_in6 , &intern_sockaddr_size );
									network_client->_extern_ip.set_ByRef( XNetworkInformation::ToString( sockaddr_pointer , intern_sockaddr_size , error ) );
									network_client->_intern_ip.set_ByRef( XNetworkInformation::ToString( (sockaddr*)&intern_sockaddr_in6 , intern_sockaddr_size , error ) );
								}
								XInterlocked::Increment( &tcp_server->_threads_count );
								rux::threading::XThread thread;
								thread.set_ThreadMode( XThreadMode_Repeat );
								thread.set_ThreadName( "tcp srv cli" );
								thread.set_Timeout( 10 );					
								thread.Start();
								thread.Push( client_thread , network_client );
								network_client = rux::engine::alloc_object< XNetworkClient >();
								if( network_address_version == XEnum_NetworkAddressVersion_IPv4 )
								{
									sockaddr_pointer = (sockaddr*)&network_client->_sockaddr_in_object;
									network_client->_sockaddr_size = sizeof( network_client->_sockaddr_in_object );
								}
								else if( network_address_version == XEnum_NetworkAddressVersion_IPv6 )
								{
									sockaddr_pointer = (sockaddr*)&network_client->_sockaddr_in6_object;
									network_client->_sockaddr_size = sizeof( network_client->_sockaddr_in6_object );
								}	
								current_thread()->AddRef();
								network_client->_main_thread = current_thread();
								network_client->_udata = tcp_server;
							}
							else
							{
								if( network_address_version == XEnum_NetworkAddressVersion_IPv4 )
									network_client->_sockaddr_size = sizeof( network_client->_sockaddr_in_object );
								else if( network_address_version == XEnum_NetworkAddressVersion_IPv6 )
									network_client->_sockaddr_size = sizeof( network_client->_sockaddr_in6_object );
							}
						}
					}
					else
						break;
				}
				while( current_thread.get_IsWaitForExit() == 0 );
				for( size_t index0 = 0 ; index0 < tcp_server->_server_sockets_count ; index0++ )
#ifdef __WINDOWS__
					::rux::engine::_globals->_ws2_32_module.closesocket( tcp_server->_server_sockets[ index0 ] );
#else				
					close( tcp_server->_server_sockets[ index0 ] );
#endif
				rux::engine::free_mem( tcp_server->_server_sockets );
				tcp_server->_server_sockets = NULL;
				tcp_server->_server_sockets_count = 0;
				rux::engine::free_object< XNetworkClient >( network_client );
			}
		};
		void XTcpServer::client_thread( void* param , size_t ___rux__thread_index1986 )
		{	
			UNREFERENCED_PARAMETER( ___rux__thread_index1986 );
			XNetworkClient* network_client = (XNetworkClient*)param;		
			if( network_client )
			{		
				TcpServer* tcp_server = (TcpServer*)network_client->_udata;				
				CRITICAL_SECTION_LOCK( tcp_server->_cs_buffer_size );
				XData recv_data( tcp_server->_buffer_size ) , send_data;
				CRITICAL_SECTION_UNLOCK( tcp_server->_cs_buffer_size );				
				rux::threading::XThread current_thread;
				current_thread = rux::threading::XThread::get_CurrentThread();
				::rux::io::file_descriptor_waiter fd_waiter;
				fd_waiter.add( network_client->_socket );
				rux::int32 recv_data_size = 0 , total_recv_data_size = 0;
				rux::uint8 is_client_disconnect = 0;		
#ifdef __WINDOWS__
				u_long ioctl_size = 0;
#else
				rux::int32 ioctl_size = 0;
#endif
				do
				{			
					for( ; ; )
					{
						::rux::int32 timeout = 1000;
						if( recv_data._data_size > 0 )
							timeout = 100;
						if( fd_waiter.wait( timeout , 1 , 0 ) )
						{
							if( fd_waiter.get_except_fd() != -1 )
							{
								is_client_disconnect = 1;
								break;
							}
							if( fd_waiter.get_read_fd() != -1 )
							{
#ifdef __WINDOWS__
								if( ::rux::engine::_globals->_ws2_32_module.ioctlsocket( network_client->_socket , FIONREAD , &ioctl_size ) != SOCKET_ERROR ) 
								{					
#else
								if( ioctl( network_client->_socket , FIONREAD , &ioctl_size ) != -1 ) 
								{
#endif
									if( recv_data._allocated_size - recv_data._data_size < ioctl_size )
										recv_data.ReAlloc( recv_data._allocated_size + ioctl_size );
									recv_data_size = ::rux::network::recv( network_client->_socket , &( (char*)recv_data._data )[ recv_data._data_size ] , recv_data._allocated_size - recv_data._data_size , 0 );
									if( recv_data_size > 0 )
										recv_data._data_size += recv_data_size;							
									else if( recv_data_size == 0 )
									{
										is_client_disconnect = 1;
										break;
									}
									else
									{
	#ifdef __WINDOWS__
										rux::int32 error_int32 = ::rux::engine::_globals->_ws2_32_module.WSAGetLastError();
										if( error_int32 == 10054 )
										{
											is_client_disconnect = 1;
											break;
										}
	#else
										rux::int32 error_int32 = errno;
										if( error_int32 == ECONNRESET )
										{
											is_client_disconnect = 1;
											break;
										}
	#endif
									}
								}
								else
								{
#ifdef __WINDOWS__
									rux::int32 error_int32 = ::rux::engine::_globals->_ws2_32_module.WSAGetLastError();
									if( error_int32 == 10054 )
									{
										is_client_disconnect = 1;
										break;
									}
#else
									rux::int32 error_int32 = errno;
									if( error_int32 == ECONNRESET )
									{
										is_client_disconnect = 1;
										break;
									}
#endif
								}
							}
							else
								break;
						}
						else
							break;
					}
					if( is_client_disconnect == 1 )
						break;
					if( tcp_server->_on_receive_callback && recv_data._data_size > 0 )
					{
						tcp_server->_on_receive_callback( recv_data , send_data , network_client->_extern_ip.str() , network_client->_intern_ip.str() );			
						if( send_data._data_size > 0 )
						{
							total_recv_data_size = 0;
							do
							{
								if( fd_waiter.wait( 1000 , 0 , 1 ) )
								{
									if( fd_waiter.get_except_fd() != -1 )
									{
										is_client_disconnect = 1;
										break;
									}
									if( fd_waiter.get_write_fd() != -1 )
									{
										recv_data_size = ::rux::network::send( network_client->_socket , &( (char*)send_data._data )[ total_recv_data_size ] , send_data._data_size - total_recv_data_size , 0 );
										if( recv_data_size > 0 )
											total_recv_data_size += recv_data_size;
										else
										{
											is_client_disconnect = 1;
											break;
										}
									}
								}
								else
								{
									is_client_disconnect = 1;
									break;
								}
							}
							while( total_recv_data_size != send_data._data_size );
						}
						send_data._data_size = 0;
					}
					else
					{
						is_client_disconnect = 1;
#ifdef __WINDOWS__
						::rux::engine::_globals->_ws2_32_module.closesocket( network_client->_socket );
#else
						close( network_client->_socket );
#endif
					}
					if( is_client_disconnect == 1 )
						break;
					CRITICAL_SECTION_LOCK( tcp_server->_cs_buffer_size );
					if( recv_data._allocated_size > tcp_server->_buffer_size )
						tcp_server->_buffer_size = recv_data._allocated_size;
					CRITICAL_SECTION_UNLOCK( tcp_server->_cs_buffer_size );	
				}
				while( network_client->_main_thread->get_IsWaitForExit() == 0 );			
				rux::engine::free_object< XNetworkClient >( network_client );
				current_thread.Stop();
				XInterlocked::Decrement( &tcp_server->_threads_count );		
			}
		};
	};
};