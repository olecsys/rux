#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_udpclient.h>
#include <xan_error.h>
#include <xan_keyvaluepair.h>
#include <xan_uint64.h>
#include <xan_file_descriptor_waiter.h>
#include <xan_network_functions.h>
#include <xan_boolean.h>
begin_implement_rux_class_ns( UdpClient , rux::network );
	_client_socket = INVALID_SOCKET;
	_is_broadcast = 0;
end_implement_rux_class();
namespace rux
{
	namespace network
	{		
		void XUdpClient::set_IsBroadcast( rux::uint8 value )
		{
			(*this)()->_is_broadcast = value;
		};
		UdpClient::~UdpClient()
		{
			Close();
		};
		void XUdpClient::set_NetworkAddress( const rux::network::XAddress& network_address )
		{
			(*this)()->set_NetworkAddress( network_address );
		}
		void UdpClient::Close( void )
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
		void UdpClient::Open( rux::XString& error )
		{
			error.Clear();
			if( _client_socket == INVALID_SOCKET )
			{
				rux_network_init();
				if( _network_address()->_network_address_version == XEnum_NetworkAddressVersion_IPv4 )
					_client_socket = ::rux::network::socket( AF_INET , SOCK_DGRAM, IPPROTO_UDP );
				else if( _network_address()->_network_address_version == XEnum_NetworkAddressVersion_IPv6 )
					_client_socket = ::rux::network::socket( AF_INET6 , SOCK_DGRAM, IPPROTO_UDP );
				if( _client_socket != INVALID_SOCKET )
				{
					rux::int32 is_reuseaddr = 1;
					if( _is_broadcast == 1 && ::rux::network::setsockopt( _client_socket , SOL_SOCKET , SO_BROADCAST , (char*)&is_reuseaddr , sizeof( is_reuseaddr ) ) == -1 )
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
		void UdpClient::set_NetworkAddress( const rux::network::XAddress& network_address )
		{	
			if( _network_address()->_address != network_address()->_address 
				|| _network_address()->_port != network_address()->_port )
			{
				_network_address = network_address;
				Close();
			}
		};
		void XUdpClient::Close( void )
		{
			(*this)()->Close();
		};
		implement_duplicate_internal_function_2( UdpClient , Receive , XData** , rux::XString& );
		void UdpClient::Receive( XData** recv_data , rux::XString& error )
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
					::rux::byte readed = 0;
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
								rux::int32 recv_data_size = ::rux::network::recvfrom( _client_socket , &( (char*)_recv_data._data )[ _recv_data._data_size ] , _recv_data._allocated_size - _recv_data._data_size , 0 , sockaddr_pointer , &sockaddr_size );
								if( recv_data_size > 0 )
								{
									_recv_data._data_size += recv_data_size;
									readed = 1;
								}
								else if( recv_data_size != 0 )
								{
	#ifdef __WINDOWS__
									error = XError::ConvertErrorCodeToString( ::rux::engine::_globals->_ws2_32_module.WSAGetLastError() , XEnum_ErrorType_GetLastError );			
	#else
									error = XError::ConvertErrorCodeToString( errno , XEnum_ErrorType_Errno );
	#endif
									break;
								}
							}
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
						else
						{
							if( readed == 0 )
								rux_write_error( "Timeout is expired" , error );
							break;
						}
					}
				}
			}
		};
		implement_duplicate_internal_function_3( UdpClient , Send , char* , size_t , rux::XString& );
		void UdpClient::Send( char* send_data , size_t send_data_size , rux::XString& error )
		{
			Open( error );
			if( error.Length() == 0 )
			{
				if( send_data && send_data_size > 0 )
				{
					::rux::io::file_descriptor_waiter fd_waiter;
					fd_waiter.add( _client_socket );
					socklen_t sockaddr_size = 0;
					sockaddr* sockaddr_pointer = NULL;
					if( _network_address()->_network_address_version == XEnum_NetworkAddressVersion_IPv4 )
					{
						sockaddr_pointer = (sockaddr*)&_network_address()->sockaddr_in_object;
						sockaddr_size = sizeof( _network_address()->sockaddr_in_object );
					}
					else if( _network_address()->_network_address_version == XEnum_NetworkAddressVersion_IPv6 )
					{
						sockaddr_pointer = (sockaddr*)&_network_address()->sockaddr_in6_object;
						sockaddr_size = sizeof( _network_address()->sockaddr_in6_object );
					}
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
								if( send_data_size - recv_data_size > 955 )
									data_size = ::rux::network::sendto( _client_socket , &send_data[ recv_data_size ] , 955 , 0 , sockaddr_pointer , sockaddr_size );
								else
									data_size = ::rux::network::sendto( _client_socket , &send_data[ recv_data_size ] , send_data_size - recv_data_size , 0 , sockaddr_pointer , sockaddr_size );
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
		};
	};
};
