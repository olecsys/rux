#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_network_information.h>
#ifdef __WINDOWS__
#include <IPHlpApi.h>
#endif
#ifdef __LINUX__
#include <ifaddrs.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <netdb.h>
#endif
#include <xan_error.h>
#include <xan_keyvaluepair.h>
#include <xan_network_functions.h>
namespace rux
{
	namespace network
	{
		rux::String& XNetworkInformation::get_HostName( rux::XString& error )
		{
			rux_network_init();
			rux::XString host_name;
			declare_stack_variable( char , name_ptr , 256 );
			if( ::rux::network::gethostname( name_ptr , 256 ) == 0 )
			{
				host_name = name_ptr;
			}
			else
			{
		#ifdef __WINDOWS__
				rux::int32 wsa_last_error = ::rux::engine::_globals->_ws2_32_module.WSAGetLastError();
				if( wsa_last_error == WSAEFAULT )
					error = "The name parameter is a NULL pointer or is not a valid part of the user address space. This error is also returned if the buffer size specified by namelen parameter is too small to hold the complete host name.";
				else if( wsa_last_error == WSANOTINITIALISED )		
					error = "A successful WSAStartup call must occur before using this function.";					
				else if( wsa_last_error == WSAENETDOWN )
					error = "The network subsystem has failed.";
				else if( wsa_last_error == WSAEINPROGRESS )
					error = "A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function.";
		#endif
		#ifdef __UNIX__
				rux::int32 err = errno;
				if( err == EFAULT )
					error = "name is an invalid address";
				else if( err == EINVAL )
					error = "len is negative or, for sethostname(), len is larger than the maximum allowed size, or, for gethostname() on Linux/i386, len is smaller than the actual size. (In this last case glibc 2.1 uses ENAMETOOLONG.)";
				else if( err == EPERM )
					error = "For sethostname(), the caller did not have the CAP_SYS_ADMIN capability.";
		#endif
			}
			return host_name++;
		};
		rux::Array<rux::XString>& XNetworkInformation::get_IPAddresses( const rux::XString& hostname , rux::XString& error )
		{
			rux_network_init();
			rux::XArray<rux::XString> ip_addresses;
			declare_stack_variable( char , name_ptr , 256 );
			if( ::rux::network::gethostname( name_ptr , 256 ) == 0 )
			{		
			}
			else
		#ifdef __WINDOWS__	
				error = XError::ConvertErrorCodeToString( ::rux::engine::_globals->_ws2_32_module.WSAGetLastError() , XEnum_ErrorType_GetLastError );
		#else
				error = XError::ConvertErrorCodeToString( errno , XEnum_ErrorType_Errno );
		#endif
			if( error.Length() == 0 )
			{
				rux::uint8 local = 0;
				rux::XString cp_hostname = hostname;
				if( cp_hostname.Length() == 0 )
					cp_hostname = name_ptr;
				cp_hostname.set_ByRef( cp_hostname.ConvertToAnsi() );
				local = cp_hostname == name_ptr ? 1 : 0;
		#ifdef __LINUX__
				if( local == 1 )
				{
					struct ifaddrs* ifaddr0 = NULL ,* ifaddr1 = NULL;
					rux::int32 family = -1 , result = -1;
					declare_stack_variable( char , host , NI_MAXHOST );
					if( getifaddrs( &ifaddr0 ) != -1 )
					{
						for( ifaddr1 = ifaddr0 ; ifaddr1 != NULL ; ifaddr1 = ifaddr1->ifa_next )
						{
							if( ifaddr1->ifa_addr == NULL )
								continue;
							family = ifaddr1->ifa_addr->sa_family;
							host[ 0 ] = 0;
							if( family == AF_INET )
							{
								if( inet_ntop( family , (void*)&( ( ( struct sockaddr_in* )( ifaddr1->ifa_addr ) )->sin_addr ) , host , NI_MAXHOST ) == NULL )
									host[ 0 ] = 0;
							}
							else if( family == AF_INET6 )
							{
								if( inet_ntop( family , (void*)&( ( ( struct sockaddr_in6* )( ifaddr1->ifa_addr ) )->sin6_addr ) , host , NI_MAXHOST ) == NULL )
									host[ 0 ] = 0;
							}
							if( host[ 0 ] != 0 )
								ip_addresses.Add( host );
						}
						freeifaddrs( ifaddr0 );
					}
					else 
					{
						errno_error_to_string( error );
					}
				}
				else
		#endif
				{
					struct hostent* hostent_ptr = ::rux::network::gethostbyname( cp_hostname.str() );
					if( hostent_ptr )
					{
						for( rux::int32 index0 = 0 ; hostent_ptr->h_addr_list[ index0 ] != NULL ; ++index0 )
						{
							struct in_addr address;
							memcpy( &address , hostent_ptr->h_addr_list[ index0 ] , sizeof( struct in_addr ) );
							ip_addresses.Add( rux::XString( ::rux::network::inet_ntoa( address ) ) );
						}
					}
					else
						error = rux::XString::Format( "'%s' is incorrect host name" , cp_hostname.str() );
				}
				if( local == 1 
					&& error.Length() == 0 )
				{
					size_t index0 = 0;
					for( ; index0 < ip_addresses.Count() ; index0++ )
					{
						if( ip_addresses[ index0 ].Equals( "127.0.0.1" , 9 ) )
							break;
					}
					if( index0 >= ip_addresses.Count() )
						ip_addresses.Add( ::rux::XString( "127.0.0.1" ) );
				}
			}
			return ip_addresses++;
		};
		rux::Array<rux::XString>& XNetworkInformation::get_IPAddresses( rux::XString& error )
		{	
			return get_IPAddresses( "" , error );
		};
		rux::Array<rux::XString>& XNetworkInformation::get_Interfaces( rux::XString& error )
		{
			rux_network_init();
			error.Clear();
			rux::XArray< rux::XString > interfaces;
		#ifdef __WINDOWS__
			DWORD result = 0;
			rux::uint32 index0 = 0;
			ULONG flags = GAA_FLAG_INCLUDE_PREFIX;
			ULONG family = AF_UNSPEC;
			PIP_ADAPTER_ADDRESSES addresses = NULL;
			ULONG out_buffer_len = 15000;
			PIP_ADAPTER_ADDRESSES current_addresses = NULL;
			do
			{
				addresses = (IP_ADAPTER_ADDRESSES*)alloc_array_macros( char , out_buffer_len );		
				result = ::rux::engine::_globals->_iphlpapi_module.GetAdaptersAddresses( family , flags , NULL , addresses , &out_buffer_len );
				if( result == ERROR_BUFFER_OVERFLOW ) 
				{
					rux::engine::free_mem( addresses );
					addresses = NULL;
				}
				else
					break;
				index0++;
			}
			while( ( result == ERROR_BUFFER_OVERFLOW ) && ( index0 < 3 ) );
			if( result == NO_ERROR )
			{
				current_addresses = addresses;
				while( current_addresses )
				{			
					interfaces.Add( rux::XString( current_addresses->AdapterName ) );
					current_addresses = current_addresses->Next;
				}
			}
			else if( result != ERROR_NO_DATA )
				XError::ConvertErrorCodeToString( result , XEnum_ErrorType_GetLastError );
			if( addresses )
				rux::engine::free_mem( addresses );    
		#elif defined( __LINUX__ )
			struct ifaddrs* ifaddr0 = NULL ,* ifaddr1 = NULL;
			rux::int32 family, s;
			declare_stack_variable( char , host , NI_MAXHOST );
			if( getifaddrs( &ifaddr0 ) != -1 )
			{
				for( ifaddr1 = ifaddr0 ; ifaddr1 != NULL ; ifaddr1 = ifaddr1->ifa_next )
				{
					if( ifaddr1->ifa_addr == NULL )
						continue;
					family = ifaddr1->ifa_addr->sa_family;
					size_t index0 = 0;
					for( ; index0 < interfaces.Count() ; index0++ )
					{
						if( interfaces[ index0 ] == ifaddr1->ifa_name )
							break;
					}
					if( index0 >= interfaces.Count() )
						interfaces.Add( ifaddr1->ifa_name );
				}
				freeifaddrs( ifaddr0 );
			}
			else 
			{
				rux_write_error( "getifaddrs is failed" , error );
			}
		#endif
			return interfaces++;
		};
		rux::uint32 XNetworkInformation::get_IPv4( const rux::XString& ip , rux::XString& error )
		{
			error.Clear();
			rux::uint32 address = 0;
			rux::XString current_cp;
			current_cp.set_ByRef( ip.ConvertToAnsi() );
			rux::XArray<rux::XString> ip_parts = current_cp.Split( '.' );
			if( ip_parts.Count() == 4 )
			{				
				rux::int32 ip_part1 = ip_parts[ 0 ].ToInt32();
				rux::int32 ip_part2 = ip_parts[ 1 ].ToInt32();
				rux::int32 ip_part3 = ip_parts[ 2 ].ToInt32();
				rux::int32 ip_part4 = ip_parts[ 3 ].ToInt32();
				address = ( ip_part1 << 24 ) + ( ip_part2 << 16 ) + ( ip_part3 << 8 ) + ip_part4;
			}
			else
				error = "incorrect ip string";	
			return address;
		};
		::rux::String& XNetworkInformation::get_MACAddress( const ::rux::XString& network_interface )
		{
			::rux::XString mac_address;
#ifdef __WINDOWS__
			DWORD result = 0;
			rux::uint32 index0 = 0;
			ULONG flags = GAA_FLAG_INCLUDE_PREFIX;
			ULONG family = AF_UNSPEC;
			PIP_ADAPTER_ADDRESSES addresses = NULL;
			ULONG out_buffer_len = 15000;
			PIP_ADAPTER_ADDRESSES current_addresses = NULL;
			do
			{
				addresses = (IP_ADAPTER_ADDRESSES*)alloc_array_macros( char , out_buffer_len );		
				result = ::rux::engine::_globals->_iphlpapi_module.GetAdaptersAddresses( family , flags , NULL , addresses , &out_buffer_len );
				if( result == ERROR_BUFFER_OVERFLOW ) 
				{
					rux::engine::free_mem( addresses );
					addresses = NULL;
				}
				else
					break;
				index0++;
			}
			while( ( result == ERROR_BUFFER_OVERFLOW ) && ( index0 < 3 ) );
			if( result == NO_ERROR )
			{
				current_addresses = addresses;
				while( current_addresses )
				{	
					if( network_interface == current_addresses->AdapterName )
					{
						char mac_part[ 16 ] = {0};
						if( current_addresses->PhysicalAddressLength != 0 )
						{
							for( index0 = 0 ; index0 < (int)current_addresses->PhysicalAddressLength ; index0++) 
							{
								if( index0 == ( current_addresses->PhysicalAddressLength - 1 ) )
									snprintf( mac_part , 16 , "%.2X" , (int)current_addresses->PhysicalAddress[ index0 ] );
								else
									snprintf( mac_part , 16 , "%.2X-" , (int)current_addresses->PhysicalAddress[ index0 ] );
								mac_address += mac_part;
							}
						}
						break;
					}
					current_addresses = current_addresses->Next;
				}
			}
			else if( result != ERROR_NO_DATA )
				XError::ConvertErrorCodeToString( result , XEnum_ErrorType_GetLastError );
			if( addresses )
				rux::engine::free_mem( addresses );
#else
			::rux::XString cp_network_interface;
			cp_network_interface.set_ByRef( network_interface.ConvertToAnsi() );
			struct ifreq s;
			int fd = ::socket( PF_INET , SOCK_DGRAM , IPPROTO_IP );
			if( fd != -1 )
			{
				::rux::safe_strncpy( s.ifr_name , cp_network_interface.str() , IFNAMSIZ );
				if( ioctl( fd , SIOCGIFHWADDR , &s ) == 0 )
				{
					char mac_part[ 16 ] = {0};
					for( int index0 = 0; index0 < 6 ; ++index0 )
					{
						if( index0 == 5 )
							snprintf( mac_part , 16 , "%02x" , (unsigned char)s.ifr_addr.sa_data[ index0 ] );
						else
							snprintf( mac_part , 16 , "%02x:" , (unsigned char)s.ifr_addr.sa_data[ index0 ] );
						mac_address += mac_part;
					}
				}
				::close( fd );
			}
#endif
			return mac_address++;
		};
		rux::String& XNetworkInformation::get_IPv4( rux::uint32 ip , rux::XString& error )
		{
			error.Clear();
			rux::XString ip_string;
			rux::int32 ip_part1 = ( ip & 0xff000000 ) >> 24;
			if( ip_part1 >= 0 && ip_part1 < 256 )
			{
				rux::int32 ip_part2 = ( ip & 0x00ff0000 ) >> 16;
				if( ip_part2 >= 0 && ip_part2 < 256 )
				{
					rux::int32 ip_part3 = ( ip & 0x0000ff00 ) >> 8;
					if( ip_part3 >= 0 && ip_part3 < 256 )
					{
						rux::int32 ip_part4 = ( ip & 0x000000ff );
						if( ip_part4 >= 0 && ip_part4 < 256 )
						{
							ip_string = rux::XString::Format( "%d.%d.%d.%d" , ip_part1 , ip_part2 , ip_part3 , ip_part4 );
						}
						else
							error = "incorrect unsigned long ip";
					}
					else
						error = "incorrect unsigned long ip";
				}
				else
					error = "incorrect unsigned long ip";
			}
			else
				error = "incorrect unsigned long ip";
			return ip_string++;
		};
		rux::String& XNetworkInformation::get_Interface( const rux::XString& ip , rux::XString& error )
		{
			rux_network_init();
			error.Clear();
			rux::XString interface_string;
		#ifdef __WINDOWS__
			declare_stack_variable( char , ipip , 256 );
			DWORD ipip_size = 256;
			DWORD result = 0;
			rux::uint32 index0 = 0;
			ULONG flags = GAA_FLAG_INCLUDE_PREFIX;
			ULONG family = AF_UNSPEC;
			PIP_ADAPTER_ADDRESSES addresses = NULL;
			ULONG out_buffer_len = 15000;
			PIP_ADAPTER_ADDRESSES current_addresses = NULL;
			do
			{
				addresses = (IP_ADAPTER_ADDRESSES*)alloc_array_macros( char , out_buffer_len );		
				result = ::rux::engine::_globals->_iphlpapi_module.GetAdaptersAddresses( family , flags , NULL , addresses , &out_buffer_len );
				if( result == ERROR_BUFFER_OVERFLOW ) 
				{
					rux::engine::free_mem( addresses );
					addresses = NULL;
				}
				else
					break;
				index0++;
			}
			while( ( result == ERROR_BUFFER_OVERFLOW ) && ( index0 < 3 ) );
			if( result == NO_ERROR )
			{
				current_addresses = addresses;
				while( current_addresses )
				{			
					PIP_ADAPTER_UNICAST_ADDRESS first = current_addresses->FirstUnicastAddress;
					while( first )
					{				
						ipip[ 0 ] = '\0';
						::rux::engine::_globals->_ws2_32_module.WSAAddressToStringA( first->Address.lpSockaddr , first->Address.iSockaddrLength , NULL , ipip , &ipip_size );
						if( ip == ipip )
							break;
						first = first->Next;
					}
					if( first )
					{
						interface_string = current_addresses->AdapterName;
						break;
					}
					current_addresses = current_addresses->Next;
				}
			}
			else if( result != ERROR_NO_DATA )
				XError::ConvertErrorCodeToString( result , XEnum_ErrorType_GetLastError );
			if( addresses )
				rux::engine::free_mem( addresses );    
		#elif defined( __LINUX__ )
			struct ifaddrs* ifaddr0 = NULL ,* ifaddr1 = NULL;
			rux::int32 family, s;
			declare_stack_variable( char , host , NI_MAXHOST );
			if( getifaddrs( &ifaddr0 ) != -1 )
			{
				declare_stack_variable( char , ipip , 256 );
				for( ifaddr1 = ifaddr0 ; ifaddr1 != NULL ; ifaddr1 = ifaddr1->ifa_next )
				{
					if( ifaddr1->ifa_addr == NULL )
						continue;
					family = ifaddr1->ifa_addr->sa_family;
					ipip[ 0 ] = '\0';
					if( family == AF_INET )
						inet_ntop( family , (void*)&( ( ( struct sockaddr_in* )( ifaddr1->ifa_addr ) )->sin_addr ) , ipip , 256 );
					else if( family == AF_INET6 )
						inet_ntop( family , (void*)&( ( ( struct sockaddr_in6* )( ifaddr1->ifa_addr ) )->sin6_addr ) , ipip , 256 );
					if( ip == ipip )
					{
						interface_string = ifaddr1->ifa_name;
						break;
					}
				}
				freeifaddrs( ifaddr0 );
			}
			else 
			{
				rux_write_error( "getifaddrs is failed" , error );
			}
		#endif
			return interface_string++;
		};
		sockaddr* XNetworkInformation::get_Sockaddr( const rux::XString& address , rux::XString& error )
		{
			rux_network_init();
			error.Clear();				
			if( error.Length() == 0 )
			{
				rux::XString _address;
				_address.set_ByRef( address.ConvertToAnsi() );		
				rux::XArray<rux::XString> ip_parts = _address.Split( '.' );
				if( ip_parts.Count() == 4 )
				{				
					rux::int32 ip_part1 = ip_parts[ 0 ].ToInt32();
					rux::int32 ip_part2 = ip_parts[ 1 ].ToInt32();
					rux::int32 ip_part3 = ip_parts[ 2 ].ToInt32();
					rux::int32 ip_part4 = ip_parts[ 3 ].ToInt32();
					sockaddr_in* sockaddr_in_object = rux::engine::alloc_object< sockaddr_in >();
		#ifdef __WINDOWS__
					sockaddr_in_object->sin_addr.S_un.S_addr = ::rux::network::htonl( (rux::uint32)( ( ip_part1 << 24 ) + ( ip_part2 << 16 ) + ( ip_part3 << 8 ) + ip_part4 ) );
		#else
					sockaddr_in_object->sin_addr.s_addr = ::rux::network::htonl( (rux::uint32)( ( ip_part1 << 24 ) + ( ip_part2 << 16 ) + ( ip_part3 << 8 ) + ip_part4 ) );
		#endif
					sockaddr_in_object->sin_port = 0;
					sockaddr_in_object->sin_family = AF_INET;
					return (sockaddr*)sockaddr_in_object;
				}
				else
				{
					ip_parts = _address.Split( ':' );
					if( ip_parts.Count() > 0 )
					{
						rux::uint32 index0 = 0;
						for( index0 ; index0 < ip_parts.Count() ; index0++ )
						{
							if( ip_parts[ index0 ].Length() != 4 )
								break;					
						}
						if( index0 >= ip_parts.Count() )
						{	
							sockaddr_in6* sockaddr_in6_object = rux::engine::alloc_object< sockaddr_in6 >();
							sockaddr_in6_object->sin6_port = 0;
		#ifdef __WINDOWS__					
							rux::int32 size = sizeof( sockaddr_in6 );
							if( ::rux::engine::_globals->_ws2_32_module.WSAStringToAddressA( _address.str() , AF_INET6 , NULL , (sockaddr*)sockaddr_in6_object , &size ) != 0 )
								error = XError::ConvertErrorCodeToString( ::rux::engine::_globals->_ws2_32_module.WSAGetLastError() , XEnum_ErrorType_GetLastError );
		#else
							if( inet_pton( AF_INET6 , _address.str() , &sockaddr_in6_object->sin6_addr ) > 0 )
								sockaddr_in6_object->sin6_family = AF_INET6;
							else
								error = XError::ConvertErrorCodeToString( errno , XEnum_ErrorType_Errno );
		#endif
							if( error.Length() > 0 )
							{
								rux::engine::free_object< sockaddr_in6 >( sockaddr_in6_object );
								sockaddr_in6_object = NULL;
							}
							return (sockaddr*)sockaddr_in6_object;
						}
						else
							error = "incorrect ip string";
					}
					else
						error = "incorrect ip string";		
				}		
			}
			return NULL;
		};
		rux::String& XNetworkInformation::ToString( sockaddr* sockaddr_pointer , socklen_t sockaddr_pointer_size , rux::XString& error )
		{
			declare_variable( rux::XString , ip_string );
			if( sockaddr_pointer && sockaddr_pointer_size > 0 )
			{
				rux_network_init();
				declare_stack_variable( char , ip , 256 );
		#ifdef __WINDOWS__	
				DWORD ip_size = 256;
				USHORT port = 0;
				if( sockaddr_pointer_size == sizeof( sockaddr_in ) )
				{
					port = ( (sockaddr_in*)sockaddr_pointer )->sin_port;
					( (sockaddr_in*)sockaddr_pointer )->sin_port = 0;
				}
				else if( sockaddr_pointer_size == sizeof( sockaddr_in6 ) )
				{
					port = ( (sockaddr_in6*)sockaddr_pointer )->sin6_port;
					( (sockaddr_in6*)sockaddr_pointer )->sin6_port = 0;
				}
				if( ::rux::engine::_globals->_ws2_32_module.WSAAddressToStringA( sockaddr_pointer , sockaddr_pointer_size , NULL , ip , &ip_size ) != 0 )
					error = XError::ConvertErrorCodeToString( ::rux::engine::_globals->_ws2_32_module.WSAGetLastError() , XEnum_ErrorType_GetLastError );
				if( sockaddr_pointer_size == sizeof( sockaddr_in ) )
					( (sockaddr_in*)sockaddr_pointer )->sin_port = port;
				else if( sockaddr_pointer_size == sizeof( sockaddr_in6 ) )
					( (sockaddr_in6*)sockaddr_pointer )->sin6_port = port;
		#else
				rux::int32 af = AF_INET;
				void* src = NULL;
				if( sockaddr_pointer_size == sizeof( sockaddr_in ) )
					src = &( (sockaddr_in*)sockaddr_pointer )->sin_addr;
				else if( sockaddr_pointer_size == sizeof( sockaddr_in6 ) )
				{
					src = &( (sockaddr_in6*)sockaddr_pointer )->sin6_addr;
					af = AF_INET6;
				}
				if( inet_ntop( af , src , ip , 256 ) == NULL )
					error = XError::ConvertErrorCodeToString( errno , XEnum_ErrorType_Errno );
		#endif
				if( error.Length() == 0 )
					ip_string = ip;
			}
			else
				error = "sockaddr pointer is NULL";	
			return ip_string++;
		};
	};
};
