#ifndef XAN_SSLV2CLIENTHELLO_H
#define XAN_SSLV2CLIENTHELLO_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_array.h>
#include <xan_int32.h>
#include <xan_tls_defines.h>
namespace rux
{
	namespace network
	{
		begin_declare_rux_class( Sslv2ClientHello );
			friend class XTlsRecordLayerPacket;
			friend class XTls;
		begin_declare_rux_class_members( Sslv2ClientHello );
			rux::uint8 _session_id[ 32 ];
			rux::uint8 _session_id_length;
			rux::XArray<rux::XInt32> _cipher_suites;
			rux::XArray<rux::XInt32> _compression_methods;
			rux::uint8 _random[ 32 ];	
		end_declare_rux_class_members( Sslv2ClientHello );
			void AddCipherSuite( XEnum_TlsCipherSuite tls_cipher_suite );
			rux::uint32 get_PacketSize( void );
			void set_Random( rux::uint8 random[ 28 ] );
			const rux::uint8* get_Random( void );
			rux::uint32 get_RandomLength( void );
		end_declare_rux_class();
	};
};
#endif
