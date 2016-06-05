#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_md4.h>
namespace rux
{
	namespace cryptography
	{
		void MD4::Hash( const rux::uint8* message_ptr , size_t message_ptr_size , rux::uint8 hash[ 16 ] )
		{
			rux::cryptography::initialize();
			if( (*rux::cryptography::_cryptography)()->_MD4 )
				(*rux::cryptography::_cryptography)()->_MD4( message_ptr , message_ptr_size , hash );
		};
	};
};
