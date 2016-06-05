#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_tlsclienthello.h>
#include <xan_keyvaluepair.h>
#include <xan_uint64.h>
#include <xan_boolean.h>
begin_implement_rux_class_ns( TlsClientHello , rux::network );
	memset( _session_id , 0 , 32 );
	memset( _random , 0 , 32 );
	_session_id_length = 0;
end_implement_rux_class();
namespace rux
{
	namespace network
	{
		void XTlsClientHello::AddCipherSuite( XEnum_TlsCipherSuite tls_cipher_suite )
		{
			rux::uint32 index0 = 0;
			for( ; index0 < (*this)()->_cipher_suites.Count() ; index0++ )
			{
				if( (*this)()->_cipher_suites[ index0 ]() == tls_cipher_suite )
					break;
			}
			if( index0 >= (*this)()->_cipher_suites.Count() )
				(*this)()->_cipher_suites.Add( rux::XInt32( tls_cipher_suite ) );
		};
		rux::uint32 XTlsClientHello::get_PacketSize( void )
		{	
			return 44 + (*this)()->_session_id_length + 2 * (*this)()->_cipher_suites.Count() + ( (*this)()->_compression_methods.Count() == 0 ? 1 : (*this)()->_compression_methods.Count() );
		/*
			1 //HandshakeType
				3//Length
				1//major
				1//minor
				4//unix timestamp
				28//random
				1//session id length
				session id length//session id
				2//ciphers length
		2* suite_cipher_count//ciphers
		1*compression_methods_count//compressions
		0 or extension_size * count//extensions
		*/
		};
		void XTlsClientHello::set_Random( rux::uint8 random[ 28 ] )
		{
			memcpy( &(*this)()->_random[ 4 ] , random , 28 );
		};
		const rux::uint8* XTlsClientHello::get_Random( void )
		{
			return (*this)()->_random;
		};
		rux::uint32 XTlsClientHello::get_RandomLength( void )
		{
			return 32;
		};
	};
};