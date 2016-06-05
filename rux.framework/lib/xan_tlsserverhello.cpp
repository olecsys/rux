#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_tlsserverhello.h>
#include <xan_keyvaluepair.h>
#include <xan_uint64.h>
#include <xan_boolean.h>
begin_implement_rux_class_ns( TlsServerHello , rux::network );
	_compression_method = 0;
	memset( _session_id , 0 , 32 );
	_session_id_length = 0;
	_cipher_suite = XEnum_CipherSuite_TLS_RSA_WITH_AES_128_CBC_SHA;	
	_gmt_unix_timestamp = 0;
	memset( _random , 0 , 32 );
	_tls_major_version = 3;
	_tls_minor_version = 1;
end_implement_rux_class();
namespace rux
{
	namespace network
	{
		TlsServerHello::~TlsServerHello()
		{	
		};
		const rux::uint8* XTlsServerHello::get_Random( void )
		{
			return (*this)()->_random;
		};
		rux::uint32 XTlsServerHello::get_RandomLength( void )
		{
			return 32;
		};
	};
};
