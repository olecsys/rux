#ifndef XAN_HASH_H
#define XAN_HASH_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_includes.h>
namespace rux
{
	namespace cryptography
	{
		class hash
		{
		public:
			static ::rux::uint32 times33_hash( const char* data , size_t data_len );
			static ::rux::uint32 times33_hash( ::rux::int32 count , ... );
			static ::rux::uint32 times33_hash( ::rux::int32 count , va_list ap );
		};
	};
};
#endif
