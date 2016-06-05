#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_hash.h>
namespace rux
{
	namespace cryptography
	{
		::rux::uint32 hash::times33_hash( const char* data , size_t data_len )
		{
			::rux::uint32 hash = 0;
			const ::rux::byte* begin = (const ::rux::uint8*)data;
			if( data_len == SIZE_MAX )
			{
				const ::rux::byte* ptr = begin;
				for( ; *ptr ; ptr++ )
					hash = hash * 33 + *ptr;
			}
			else
			{
				const ::rux::byte* ptr = begin;
				for( size_t index0 = data_len ; index0 ; index0-- , ptr++ )
					hash = hash * 33 + *ptr;
			}
			return hash;
		}
		::rux::uint32 hash::times33_hash( ::rux::int32 count , ... )
		{
			va_list ap;
			va_start( ap , count );
			::rux::uint32 hash = times33_hash( count , ap );
			va_end( ap );
			return hash;
		};
		::rux::uint32 hash::times33_hash( ::rux::int32 count , va_list ap )
		{
			::rux::uint32 hash = 0;
			const ::rux::byte* ptr = 0;
			for( size_t index0 = 0 ; index0 < count ; index0++ )
			{
				ptr = (const ::rux::uint8*)va_arg( ap , char* );
				for( ; *ptr ; ptr++ )
					hash = hash * 33 + *ptr;
			}
			return hash;
		};
	};
};