#ifndef XAN_SHA1_H
#define XAN_SHA1_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_array.h"
#include "xan_io_defines.h"
namespace rux
{	
	namespace cryptography
	{
		class XSha1
		{
		public:
			static void HashStep( const rux::uint8* message_part , rux::uint64 message_offset , rux::uint64 message_size , rux::uint64 message_padding , rux::uint32 digest[ 5 ] );
			static void Hash( const rux::uint8* message_ptr , rux::uint64 message_ptr_size , rux::uint8 hash[ 20 ] );
			static void Hash( const char* filename , rux::uint8 hash[ 20 ] );
			static void Hash( rux::io::file& file , rux::uint8 hash[ 20 ] );
		};
	};
};
#endif
