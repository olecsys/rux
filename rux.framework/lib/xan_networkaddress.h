#ifndef XAN_NETWORKADDRESS_H
#define XAN_NETWORKADDRESS_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_string.h>
#include <xan_keyvaluepair.h>
#include <xan_thread.h>
typedef void (*NetworkFormatFunction)( const XObject& send_data , rux::uint8*& network_buffer_ptr , rux::uint32& network_buffer_size , rux::XString& error );
typedef rux::uint32 (*NetworkParseFunction)( const XObject& receive_data , rux::uint8* network_buffer_ptr , rux::uint32 network_buffer_size , rux::XString& error );
typedef rux::uint32 (*NetworkGetLengthFunction)( rux::uint8* network_buffer_ptr , rux::uint32 network_buffer_size , rux::XString& error );
namespace rux
{
	namespace network
	{
		class UdpClient;
		class UdpServer;
		class XTcpServer;
		class TcpServer;
		class XTcpClient;
		class TcpClient;
		class IcmpClient;
		dll_internal void AddNetworkIpv4Address( const rux::XString& interface_string , const rux::XString& address , rux::XString& error );
		enum XEnum_NetworkAddressVersion
		{
			XEnum_NetworkAddressVersion_IPv4,
			XEnum_NetworkAddressVersion_IPv6
		};
		struct XNetworkClient
		{
			SOCKET _socket;
			struct sockaddr_in  _sockaddr_in_object;
			struct sockaddr_in6  _sockaddr_in6_object;
			rux::threading::Thread* _main_thread;
			socklen_t _sockaddr_size;
			void* _udata;
			rux::XString _extern_ip;
			rux::XString _intern_ip;
			XNetworkClient()
			{
				_socket = INVALID_SOCKET;
				memset( &_sockaddr_in_object , 0 , sizeof( _sockaddr_in_object ) );
				memset( &_sockaddr_in6_object , 0 , sizeof( _sockaddr_in6_object ) );
				_main_thread = NULL;
				_sockaddr_size = 0;
				_udata = NULL;
			};
			~XNetworkClient()
			{
				if( _main_thread )
					_main_thread->Release();
			};
		};
		begin_declare_rux_class( Address );
			friend class rux::network::XTcpServer;
			friend class rux::network::TcpServer;
			friend class rux::network::UdpServer;
			friend class rux::network::UdpClient;
			friend class rux::network::TcpClient;
			friend class rux::network::IcmpClient;
			friend class rux::network::XTcpClient;
		begin_declare_rux_class_members( Address );
			rux::XString _address;	
			rux::uint16 _port;
			XEnum_NetworkAddressVersion _network_address_version;
			struct sockaddr_in  sockaddr_in_object;
			struct sockaddr_in6  sockaddr_in6_object;
			rux::String& get_Address( void );
			rux::uint16 get_Port( void );
		end_declare_rux_class_members( Address );		
			XAddress( const rux::XString& address , rux::uint16 port , rux::XString& error );
			rux::String& get_Address( void );
			rux::uint16 get_Port( void );
		end_declare_rux_class();
	};	
};
#endif
