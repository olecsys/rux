#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_tcpclient.h>
#include <xan_error.h>
#include <xan_keyvaluepair.h>
#include <xan_uint64.h>
#include <xan_file_descriptor_waiter.h>
#include <xan_network_functions.h>
#include <xan_boolean.h>
begin_implement_rux_class_ns( TcpClient , rux::network );
	_client_socket = INVALID_SOCKET;
end_implement_rux_class();
namespace rux
{
	namespace network
	{		
		TcpClient::~TcpClient()
		{
			Close();
		};
		void XTcpClient::set_NetworkAddress( const rux::network::XAddress& network_address )
		{
			(*this)()->set_NetworkAddress( network_address );
		}
		void TcpClient::Close( void )
		{
#ifdef __WINDOWS__
			if( _client_socket != INVALID_SOCKET )
				{
					::rux::engine::_globals->_ws2_32_module.closesocket( _client_socket );
					_client_socket = INVALID_SOCKET;
				}
#else
				if( _client_socket != INVALID_SOCKET )
				{
					close( _client_socket );
					_client_socket = INVALID_SOCKET;
				}
#endif
		};
		void TcpClient::Open( rux::XString& error )
		{
			error.Clear();
			if( _client_socket == INVALID_SOCKET )
			{
				rux_network_init();
				if( _network_address()->_network_address_version == XEnum_NetworkAddressVersion_IPv4 )		
					_client_socket = ::rux::network::socket( AF_INET , SOCK_STREAM , IPPROTO_TCP );
				else if( _network_address()->_network_address_version == XEnum_NetworkAddressVersion_IPv6 )		
					_client_socket = ::rux::network::socket( AF_INET6 , SOCK_STREAM , IPPROTO_TCP );
				if( _client_socket != INVALID_SOCKET )
				{
					socklen_t sockaddr_size = 0;
					sockaddr* sockaddr_pointer = NULL;
					if( _network_address()->_network_address_version == XEnum_NetworkAddressVersion_IPv4 )
					{
						sockaddr_pointer = (sockaddr*)&_network_address()->sockaddr_in_object;
						sockaddr_size = sizeof( _network_address()->sockaddr_in_object );
					}
					else if( _network_address()->_network_address_version == XEnum_NetworkAddressVersion_IPv6 )
					{
						sockaddr_pointer = (sockaddr*)&_network_address()->sockaddr_in_object;
						sockaddr_size = sizeof( _network_address()->sockaddr_in6_object );
					}
#ifdef __WINDOWS__
					if( ::rux::engine::_globals->_ws2_32_module.connect( _client_socket , sockaddr_pointer , sockaddr_size ) != 0 )
#else
					if( connect( _client_socket , sockaddr_pointer , sockaddr_size ) != 0 )
#endif
					{
#ifdef __WINDOWS__
						error = XError::ConvertErrorCodeToString( ::rux::engine::_globals->_ws2_32_module.WSAGetLastError() , XEnum_ErrorType_GetLastError );
#endif
#ifdef __UNIX__
						error = XError::ConvertErrorCodeToString( errno , XEnum_ErrorType_Errno );
#endif
#ifdef __WINDOWS__
						::rux::engine::_globals->_ws2_32_module.closesocket( _client_socket );
						_client_socket = INVALID_SOCKET;
#endif 
#ifdef __UNIX__
						close( _client_socket );
						_client_socket = INVALID_SOCKET;
#endif
					}			
				}	
				else
				{
#ifdef __WINDOWS__
					error = XError::ConvertErrorCodeToString( ::rux::engine::_globals->_ws2_32_module.WSAGetLastError() , XEnum_ErrorType_GetLastError );			
#endif
#ifdef __UNIX__
					error = XError::ConvertErrorCodeToString( errno , XEnum_ErrorType_Errno );
#endif
				}
			}
		};
		void TcpClient::set_NetworkAddress( const rux::network::XAddress& network_address )
		{	
			if( _network_address()->_address != network_address()->_address 
				|| _network_address()->_port != network_address()->_port )
			{
				_network_address = network_address;
				Close();
			}
		};
		void XTcpClient::Close( void )
		{
			(*this)()->Close();
		};
		void XTcpClient::Send( char* send_data , size_t send_data_size , rux::XString& error )
		{
			(*this)()->Send( send_data , send_data_size , error );
		};
		void XTcpClient::Receive( XData** recv_data , rux::XString& error )
		{
			(*this)()->Receive( recv_data , error );
		};
		void TcpClient::Receive( XData** recv_data , rux::XString& error )
		{
			Open( error );
			if( error.Length() == 0 )
			{
				if( recv_data )
				{		
					::rux::io::file_descriptor_waiter fd_waiter;
					fd_waiter.add( _client_socket );
#ifdef __WINDOWS__
					u_long ioctl_size = 0;
#else
					rux::int32 ioctl_size = 0;
#endif
					rux::byte readed = 0;
					while( fd_waiter.wait( 1000 , 1 , 0 ) )
					{				
						if( fd_waiter.get_read_fd() != -1 )
						{	
	#ifdef __WINDOWS__
							if( ::rux::engine::_globals->_ws2_32_module.ioctlsocket( _client_socket , FIONREAD , &ioctl_size ) != SOCKET_ERROR ) 
							{	
	#else
							if( ioctl( _client_socket , FIONREAD , &ioctl_size ) != -1 ) 
							{
	#endif
								*recv_data = &_recv_data;
								if( _recv_data._allocated_size - _recv_data._data_size < ioctl_size )
									_recv_data.ReAlloc( _recv_data._allocated_size + ioctl_size );							
								rux::int32 recv_data_size = ::rux::network::recv( _client_socket , &( (char*)_recv_data._data )[ _recv_data._data_size ] , _recv_data._allocated_size - _recv_data._data_size , 0 );
								if( recv_data_size > 0 )
								{
									_recv_data._data_size += recv_data_size;
									readed = 1;
								}
								else if( recv_data_size != 0 )
								{
	#ifdef __WINDOWS__
									error = rux::XString( __FUNCTION__ ) + " , " + XError::ConvertErrorCodeToString( ::rux::engine::_globals->_ws2_32_module.WSAGetLastError() , XEnum_ErrorType_GetLastError );			
	#else
									error = rux::XString( __FUNCTION__ ) + " , " + XError::ConvertErrorCodeToString( errno , XEnum_ErrorType_Errno );
	#endif
								}
							}
							else
							{
	#ifdef __WINDOWS__
								error = rux::XString( __FUNCTION__ ) + " , " + XError::ConvertErrorCodeToString( ::rux::engine::_globals->_ws2_32_module.WSAGetLastError() , XEnum_ErrorType_GetLastError );			
	#else
								error = rux::XString( __FUNCTION__ ) + " , " + XError::ConvertErrorCodeToString( errno , XEnum_ErrorType_Errno );
	#endif
							}
						}
						else
						{
							if( readed == 0 )
								rux_write_error( "Timeout is expired" , error );
						}
					}
				}
			}
		};
		rux::network::Address& TcpClient::get_NetworkAddress( void )
		{
			return _network_address++;
		};
		rux::network::Address& XTcpClient::get_NetworkAddress( void )
		{
			return (*this)()->get_NetworkAddress();
		};
		void TcpClient::Send( char* send_data , size_t send_data_size , rux::XString& error )
		{
			Open( error );
			if( error.Length() == 0 )
			{
				if( send_data && send_data_size > 0 )
				{
					::rux::io::file_descriptor_waiter fd_waiter;
					fd_waiter.add( _client_socket );
#ifdef __WINDOWS__
					u_long ioctl_size = 0;
#else
					rux::int32 ioctl_size = 0;
#endif
					//socklen_t sockaddr_size = 0;
					//if( _network_address()->_network_address_version == XEnum_NetworkAddressVersion_IPv4 )
					//	sockaddr_size = sizeof( _network_address()->sockaddr_in_object );
					//else if( _network_address()->_network_address_version == XEnum_NetworkAddressVersion_IPv6 )
					//	sockaddr_size = sizeof( _network_address()->sockaddr_in6_object );
					rux::int32 recv_data_size = 0 , data_size = 0;
					do
					{
						if( fd_waiter.wait( 1000 , 0 , 1 ) )
						{							
							if( fd_waiter.get_except_fd() != -1 )
							{
								error = "except fd set";
								break;
							}
							if( fd_waiter.get_write_fd() != -1 )
							{											
								if( send_data_size - recv_data_size > 65535 )
									data_size = ::rux::network::send( _client_socket , &send_data[ recv_data_size ] , 65535 , 0 );
								else
									data_size = ::rux::network::send( _client_socket , &send_data[ recv_data_size ] , send_data_size - recv_data_size , 0 );
								if( data_size > 0 )
									recv_data_size += data_size;
								else
								{
	#ifdef __WINDOWS__
									error = XError::ConvertErrorCodeToString( ::rux::engine::_globals->_ws2_32_module.WSAGetLastError() , XEnum_ErrorType_GetLastError );			
	#else
									error = XError::ConvertErrorCodeToString( errno , XEnum_ErrorType_Errno );
	#endif
									break;
								}
							}
						}
						else
						{
							errno_error_to_string( error );
							break;
						}
					}
					while( recv_data_size != send_data_size );
				}
			}
			if( error.Length() > 0 )
				Close();
		}		
	};
};
