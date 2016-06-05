#ifndef XAN_MD4_H
#define XAN_MD4_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_cryptography.h>
namespace rux
{
	namespace cryptography
	{
		class MD4
		{
		public:
			static void Hash( const rux::uint8* message_ptr , size_t message_ptr_size , rux::uint8 hash[ 16 ] );
		};
	};
};
#endif
