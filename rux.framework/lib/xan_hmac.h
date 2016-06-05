#ifndef XAN_HMAC_H
#define XAN_HMAC_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_array.h>
namespace rux
{
	namespace cryptography
	{
		class XHMAC
		{
		public:
			static void ComputeSHA1( const rux::uint8* secret_key_ptr , rux::uint32 secret_key_ptr_length , const rux::uint8* data_ptr , rux::uint32 data_ptr_length , rux::uint8 hmac[ 20 ] ); 
			static void ComputeMD5( const rux::uint8* secret_key_ptr , rux::uint32 secret_key_ptr_length , const rux::uint8* data_ptr , rux::uint32 data_ptr_length , rux::uint8 hmac[ 16 ] ); 
		};
	};
};
#endif
