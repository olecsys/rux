#ifndef XAN_FIREWALL_H
#define XAN_FIREWALL_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_file.h>
#include <xan_keyvaluepair.h>
namespace rux
{
	namespace network
	{
		class XFirewall
		{
		public:
			static rux::uint8 Allow( const rux::XString& program_path , rux::XString& error );
			static rux::uint8 Allow( const rux::String& program_path , rux::XString& error );
			static rux::uint8 Deny( const rux::XString& program_path , rux::XString& error );
			static rux::uint8 Deny( const rux::String& program_path , rux::XString& error );
		};
	};
};
#endif
