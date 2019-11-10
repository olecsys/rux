#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_networkaddress.h>
#include <xan_error.h>
#include <xan_keyvaluepair.h>
#ifdef __WINDOWS__
#include <IPHlpApi.h>
#else
#include <net/if.h>
#include <net/if_arp.h>
#include <linux/sockios.h>
#if defined(__GLIBC__) && __GLIBC__ >=2 && __GLIBC_MINOR__ >= 1
#include <netpacket/packet.h>
#include <net/ethernet.h>
#else
#include <netinet/if_ether.h>
#endif
#endif
#include <xan_uint64.h>
#include <xan_network_functions.h>
#include <xan_boolean.h>
begin_implement_rux_class_ns( Address , rux::network );
	_port = 0;
	_network_address_version = XEnum_NetworkAddressVersion_IPv4;
end_implement_rux_class();
namespace rux
{
	namespace network
	{	
		XAddress::XAddress( const rux::XString& address , rux::uint16 port , rux::XString& error )
		{
			rux_network_init();
			error.Clear();			
			if( port > 0 && port <= 99999 )
				(*this)()->_port = port;
			else
				error = "port must be higher 0 and less 100000";
			if( error.Length() == 0 )
			{
				(*this)()->_address.set_ByRef( address.ConvertToAnsi() );		
				rux::XArray<rux::XString> ip_parts = (*this)()->_address.Split( '.' );
				if( ip_parts.Count() == 4 )
				{				
					rux::int32 ip_part1 = ip_parts[ 0 ].ToInt32();
					rux::int32 ip_part2 = ip_parts[ 1 ].ToInt32();
					rux::int32 ip_part3 = ip_parts[ 2 ].ToInt32();
					rux::int32 ip_part4 = ip_parts[ 3 ].ToInt32();
		#ifdef __WINDOWS__
					(*this)()->sockaddr_in_object.sin_addr.S_un.S_addr = ::rux::network::htonlF( (rux::uint32)( ( ip_part1 << 24 ) + ( ip_part2 << 16 ) + ( ip_part3 << 8 ) + ip_part4 ) );
		#else
					(*this)()->sockaddr_in_object.sin_addr.s_addr = ::rux::network::htonlF( (rux::uint32)( ( ip_part1 << 24 ) + ( ip_part2 << 16 ) + ( ip_part3 << 8 ) + ip_part4 ) );
		#endif
					(*this)()->sockaddr_in_object.sin_port = ::rux::network::htonsF( (*this)()->_port );
					(*this)()->sockaddr_in_object.sin_family = AF_INET;
				}
				else
				{
					ip_parts = (*this)()->_address.Split( ':' );
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
		#ifdef __WINDOWS__
							rux::int32 size = sizeof( (*this)()->sockaddr_in6_object );
							if( ::rux::engine::_globals->_ws2_32_module.WSAStringToAddressA( (*this)()->_address.str() , AF_INET6 , NULL , (sockaddr*)&(*this)()->sockaddr_in6_object , &size ) == 0 )
								(*this)()->_network_address_version = XEnum_NetworkAddressVersion_IPv6;
							else
								error = XError::ConvertErrorCodeToString( ::rux::engine::_globals->_ws2_32_module.WSAGetLastError() , XEnum_ErrorType_GetLastError );
		#endif
		#ifdef __UNIX__
							if( inet_pton( AF_INET6 , (*this)()->_address.str() , &(*this)()->sockaddr_in6_object.sin6_addr ) > 0 )
							{
								(*this)()->sockaddr_in6_object.sin6_port = ::rux::network::htonsF( (*this)()->_port );
								(*this)()->sockaddr_in6_object.sin6_family = AF_INET6;
							}
							else
								error = XError::ConvertErrorCodeToString( errno , XEnum_ErrorType_Errno );
		#endif
						}
						else
							error = "incorrect ip string";
					}
					else
						error = "incorrect ip string";		
				}		
			}
		};
		rux::String& Address::get_Address( void )
		{
			return _address++;
		};
		rux::uint16 Address::get_Port( void )
		{
			return _port;
		};
		rux::String& XAddress::get_Address( void )
		{
			return (*this)()->get_Address();
		};
		rux::uint16 XAddress::get_Port( void )
		{
			return (*this)()->get_Port();
		};
		dll_internal void AddNetworkIpv4Address( const rux::XString& interface_string , const rux::XString& address , rux::XString& error )
		{
			rux_network_init();
			error.Clear();
#ifdef __WINDOWS__			
			rux::uint8 found = 0;
			ULONG adapter_index = 0;
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
					if( interface_string == current_addresses->AdapterName )
					{
						adapter_index = current_addresses->IfIndex;
						found = 1;
						break;
					}
					current_addresses = current_addresses->Next;
				}
			}
			else if( result != ERROR_NO_DATA )
				XError::ConvertErrorCodeToString( result , XEnum_ErrorType_GetLastError );
			if( addresses )
				rux::engine::free_mem( addresses );    
			if( found == 1 )			
			{
				UINT ip_address = ::rux::engine::_globals->_ws2_32_module.inet_addr( address.str() );
				if( ip_address != INADDR_NONE ) 
				{			
					UINT ip_mask = ::rux::engine::_globals->_ws2_32_module.inet_addr( "255.255.255.0" );
					ULONG nte_context = 0;
					ULONG nte_instance = 0;
					if( ( result = ::rux::engine::_globals->_iphlpapi_module.AddIPAddress( ip_address ,  ip_mask , adapter_index , &nte_context , &nte_instance ) ) != NO_ERROR ) 					
					{
						error = XError::ConvertErrorCodeToString( result , XEnum_ErrorType_GetLastError );
					}
				}
				else
				{
					error = "incorrect IPv4 address";
				}
			}
			else
			{
				error = "interface '" + interface_string + "' is not found";
			}
#else
			rux::XString utf8_interface , utf8_address;
			utf8_interface.set_ByRef( interface_string.ConvertToUTF8() );
			utf8_address.set_ByRef( address.ConvertToUTF8() );
			if( utf8_interface.Length() > 0 )
			{
				rux::int32 _socket = ::rux::network::socket( AF_INET , SOCK_DGRAM , 0 );
				if( _socket != -1)
				{
					struct ifreq ifr;					
					memcpy( ifr.ifr_name , utf8_interface.str() , utf8_interface.Size() );
					if( ioctl( _socket , SIOCGIFFLAGS , &ifr ) != -1 ) 
					{
#ifdef ifr_flags
#define IRFFLAGS ifr_flags
#else
#define IRFFLAGS ifr_flagshigh
#endif
						if( ifr.IRFFLAGS | ~( IFF_UP ) )
						{		
							ifr.IRFFLAGS |= IFF_UP;
							if( ioctl( _socket , SIOCSIFFLAGS , &ifr ) == -1 )
								error = XError::ConvertErrorCodeToString( errno , XEnum_ErrorType_Errno );
						}
						if( error.Length() == 0 )
						{
							struct sockaddr_in sin;
							sin.sin_family = AF_INET;
							inet_aton( utf8_address.str() , (in_addr*)&sin.sin_addr.s_addr );	
							memcpy( &ifr.ifr_addr , &sin , sizeof( struct sockaddr ) );	
							if( ioctl( _socket , SIOCSIFADDR , &ifr ) == -1 )
								error = XError::ConvertErrorCodeToString( errno , XEnum_ErrorType_Errno );
						}
					}
					else
						error = XError::ConvertErrorCodeToString( errno , XEnum_ErrorType_Errno );
					close( _socket );
				}
				else
					error = XError::ConvertErrorCodeToString( errno , XEnum_ErrorType_Errno );
#undef IRFFLAGS
			}
			else
				error = "interface string is empty";
#endif
		};
	};
};
