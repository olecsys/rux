#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_icmpclient.h>
#include <xan_error.h>
#include <xan_keyvaluepair.h>
#include <xan_file_descriptor_waiter.h>
#include <xan_network_functions.h>
#define ICMP_ECHO 8
#define ICMP_ECHOREPLY 0
#define ICMP_MIN_PACKET_SIZE 8
#define ICMP_DEF_PACKET_SIZE 32        // Default packet size
#define ICMP_MAX_PACKET_SIZE 1024      // Max ICMP packet size
#define IP_HEADER_MAX_PACKET_SIZE 60        // Max IP header size w/options
struct ip_header
{
    rux::uint32 _len : 4;        // Length of the header
    rux::uint32 _version : 4;      // Version of IP
    rux::uint8 _tos;            // Type of service
    rux::uint16 _total_len;      // Total length of the packet
    rux::uint16 _ident;          // Unique identifier
    rux::uint16 _frag_and_flags; // Flags
    rux::uint8 _ttl;            // Time to live
    rux::uint8 _proto;          // Protocol (TCP, UDP etc)
    rux::uint16 _checksum;       // IP checksum
    rux::uint32 _source_ip;
    rux::uint32 _dest_ip;
};
struct icmp_header
{
    rux::uint8 _type;
    rux::uint8 _code;                 // Type sub code
    rux::uint16 _cksum;
    rux::uint16 _id;
    rux::uint16 _seq;
    rux::uint32 _timestamp;
};
#include <xan_uint64.h>
#include <xan_boolean.h>
begin_implement_rux_class_ns( IcmpClient , rux::network );
		_client_socket = INVALID_SOCKET;
		_is_broadcast = 0;
end_implement_rux_class();
namespace rux
{
	namespace network
	{		
		void XIcmpClient::set_IsBroadcast( rux::uint8 value )
		{
			(*this)()->_is_broadcast = value;
		};
		IcmpClient::~IcmpClient()
		{
			Close();
		};
		void XIcmpClient::set_NetworkAddress( const rux::network::XAddress& network_address )
		{
			(*this)()->set_NetworkAddress( network_address );
		}
		void IcmpClient::Close( void )
		{
		#ifdef __WINDOWS__
				if( _client_socket != INVALID_SOCKET )
				{
					::rux::engine::_globals->_ws2_32_module.closesocket( _client_socket );
					_client_socket = INVALID_SOCKET;
				}
		#else
				if( _client_socket >= 0 )
				{
					close( _client_socket );
					_client_socket = INVALID_SOCKET;
				}
		#endif
		};
		void IcmpClient::Open( rux::XString& error )
		{
			error.Clear();
		#ifdef __WINDOWS__
			if( _client_socket == INVALID_SOCKET )
		#else
			if( _client_socket == -1 )
		#endif
			{
				rux_network_init();
				if( _network_address()->_network_address_version == XEnum_NetworkAddressVersion_IPv4 )
					_client_socket = ::rux::network::socket( AF_INET , SOCK_RAW , IPPROTO_ICMP );
				else if( _network_address()->_network_address_version == XEnum_NetworkAddressVersion_IPv6 )
					_client_socket = ::rux::network::socket( AF_INET6 , SOCK_RAW , IPPROTO_ICMP );
				if( _client_socket != INVALID_SOCKET )
				{
					rux::int32 timeout = 1000;
					if( ::rux::network::setsockopt( _client_socket , SOL_SOCKET , SO_RCVTIMEO , (char*)&timeout , sizeof( timeout ) ) != -1 )
					{
						timeout = 1000;
						if( ::rux::network::setsockopt( _client_socket , SOL_SOCKET , SO_SNDTIMEO , (char*)&timeout , sizeof( timeout ) ) == -1 )
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
					error = rux::XString( __FUNCTION__ ) + " , " + XError::ConvertErrorCodeToString( ::rux::engine::_globals->_ws2_32_module.WSAGetLastError() , XEnum_ErrorType_GetLastError );			
		#endif
		#ifdef __UNIX__
					error = rux::XString( __FUNCTION__ ) + " , " + XError::ConvertErrorCodeToString( errno , XEnum_ErrorType_Errno );
		#endif
				}
			}
		};
		void IcmpClient::set_NetworkAddress( const rux::network::XAddress& network_address )
		{	
			if( _network_address()->_address != network_address()->_address 
				|| _network_address()->_port != network_address()->_port )
			{
				_network_address = network_address;
				Close();
			}
		};
		void XIcmpClient::Close( void )
		{
			(*this)()->Close();
		};
		void XIcmpClient::Send( char* send_data , size_t send_data_size , rux::XString& error )
		{
			(*this)()->Send( send_data , send_data_size , error );
		};
		rux::uint16 IcmpClient::checksum( rux::uint16* buffer , rux::int32 size )
		{
			rux::uint32 cksum = 0;
			while( size > 1 )
			{
				cksum += *buffer++;
				size -= sizeof( rux::uint16 );
			}
			if( size )
				cksum += *(rux::uint8*)buffer;
			cksum = ( cksum >> 16 ) + ( cksum & 0xffff );
			cksum += ( cksum >> 16 );
			return (rux::uint16)( ~cksum );
		}
		void XIcmpClient::Echo( size_t datasize , size_t packet_count , size_t& received_packets , rux::uint32& max_time , rux::uint32& min_time , rux::uint32& average_time , rux::XString& error )
		{
			(*this)()->Echo( datasize , packet_count , received_packets , max_time , min_time , average_time , error );
		};
		void IcmpClient::Echo( size_t datasize , size_t packet_count , size_t& received_packets , rux::uint32& max_time , rux::uint32& min_time , rux::uint32& average_time , rux::XString& error )
		{
			error.Clear();
			datasize += sizeof( icmp_header );
			if( datasize > ICMP_MAX_PACKET_SIZE )
				datasize = ICMP_MAX_PACKET_SIZE;
			declare_stack_variable( char , send_data , ICMP_MAX_PACKET_SIZE );
			memset( send_data , 0 , ICMP_MAX_PACKET_SIZE );
			icmp_header* icmp_hdr = cpp_ptr_cast( icmp_header* , send_data );
			icmp_hdr->_type = ICMP_ECHO;        // Request an ICMP echo
			icmp_hdr->_code = 0;
			icmp_hdr->_id = (rux::uint16)rux::diagnostics::process::get_process_id();
			icmp_hdr->_cksum = 0;
			icmp_hdr->_seq = 0;
			memset( &send_data[ sizeof( icmp_header ) ] , 'E' , datasize - sizeof( icmp_header ) );
			size_t steps = 0;
			XData* recv_data = NULL;
			min_time = 0xffffffff;
			max_time = 0;
			average_time = 0;
			Open( error );
			if( error.Length() == 0 )
			{
				while( steps < packet_count )
				{
					( cpp_ptr_cast( icmp_header* , send_data ) )->_cksum = 0;
					( cpp_ptr_cast( icmp_header* , send_data ) )->_timestamp = XTime::GetTickCount();
					( cpp_ptr_cast( icmp_header* , send_data ) )->_seq = steps++;
					( cpp_ptr_cast( icmp_header* , send_data ) )->_cksum = checksum( (rux::uint16*)send_data , datasize );
					Send( send_data , datasize , error );
					if( error.Length() > 0 )
						break;
					Receive( &recv_data , error );
					if( error.Length() > 0 )
						break;
					if( recv_data && recv_data->_data_size > 0 )
					{
						rux::uint32 _data_size = recv_data->_data_size;
						rux::uint8* buffer = (rux::uint8*)recv_data->Read( _data_size );
						ip_header* iphdr = cpp_ptr_cast(ip_header* , buffer );
						rux::uint16 iphdrlen = iphdr->_len * 4;
						rux::uint32 tick = XTime::GetTickCount();
						if( _data_size >= iphdrlen + ICMP_MIN_PACKET_SIZE )
						{						
							icmp_header* icmphdr = cpp_ptr_cast(icmp_header* , ( buffer + iphdrlen ) );
							if( icmphdr->_type == ICMP_ECHOREPLY )
							{
								if( icmphdr->_id == (rux::uint16)rux::diagnostics::process::get_process_id() )
								{	
									rux::uint32 elapsed = tick - icmphdr->_timestamp;
									average_time += elapsed;
									if( elapsed > max_time )
										max_time = elapsed;
									if( elapsed < min_time )
										min_time = elapsed;
									received_packets++;
								}
							}
						}
					}
				}
				average_time /= received_packets;
			}
		};
		void IcmpClient::Receive( XData** recv_data , rux::XString& error )
		{
			Open( error );
			if( error.Length() == 0 )
			{
				if( recv_data )
				{		
					::rux::io::file_descriptor_waiter fd_waiter;
					fd_waiter.add( _client_socket );		
					struct sockaddr_in  sockaddr_in_object;		
					struct sockaddr_in6  sockaddr_in6_object;
#ifdef __WINDOWS__
					u_long ioctl_size = 0;
#else
					rux::int32 ioctl_size = 0;
#endif
					socklen_t sockaddr_size = 0;
					sockaddr* sockaddr_pointer = NULL;
					if( _network_address()->_network_address_version == XEnum_NetworkAddressVersion_IPv4 )
					{
						sockaddr_pointer = (sockaddr*)&sockaddr_in_object;
						sockaddr_size = sizeof( sockaddr_in_object );
					}
					else if( _network_address()->_network_address_version == XEnum_NetworkAddressVersion_IPv6 )
					{
						sockaddr_pointer = (sockaddr*)&sockaddr_in6_object;
						sockaddr_size = sizeof( sockaddr_in6_object );
					}
					if( fd_waiter.wait( 1000 , 1 , 0 ) )
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
									_recv_data._data_size += recv_data_size;
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
					}
				}
			}
		};
		void IcmpClient::Send( char* send_data , size_t send_data_size , rux::XString& error )
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
						sockaddr_pointer = (sockaddr*)&_network_address()->sockaddr_in_object;
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
									error = rux::XString( __FUNCTION__ ) + " , " + XError::ConvertErrorCodeToString( ::rux::engine::_globals->_ws2_32_module.WSAGetLastError() , XEnum_ErrorType_GetLastError );			
		#else
									error = rux::XString( __FUNCTION__ ) + " , " + XError::ConvertErrorCodeToString( errno , XEnum_ErrorType_Errno );
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
		}
	};
};
