#ifndef XAN_MD5_H
#define XAN_MD5_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_string.h>
namespace rux
{
	namespace cryptography
	{
		class XMD5
		{
		public:
			static void Hash( const rux::uint8* message_ptr , rux::uint64 message_ptr_size , rux::uint8 hash[ 16 ] );
		};
	};
};
#endif
