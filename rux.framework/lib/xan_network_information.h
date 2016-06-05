#ifndef XAN_NETWORKINFORMATION_H
#define XAN_NETWORKINFORMATION_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_string.h>
#include <xan_keyvaluepair.h>
namespace rux
{
	namespace network
	{
		class XNetworkInformation
		{
public:
			static rux::String& get_HostName( rux::XString& error );
			static rux::Array<rux::XString>& get_IPAddresses( rux::XString& error );
			static rux::Array<rux::XString>& get_IPAddresses( const rux::XString& hostname , rux::XString& error );
			static rux::Array<rux::XString>& get_Interfaces( rux::XString& error );
			static rux::String& get_IPv4( rux::uint32 ip , rux::XString& error );
			static rux::uint32 get_IPv4( const rux::XString& ip , rux::XString& error );
			static rux::String& get_Interface( const rux::XString& ip , rux::XString& error );
			static sockaddr* get_Sockaddr( const rux::XString& address , rux::XString& error );
			static rux::String& ToString( sockaddr* sockaddr_pointer , socklen_t sockaddr_pointer_size , rux::XString& error );
			static ::rux::String& get_MACAddress( const ::rux::XString& network_interface );
		};
	};
};
#endif
