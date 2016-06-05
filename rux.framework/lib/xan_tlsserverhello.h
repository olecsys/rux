#ifndef XAN_TLSSERVERHELLO_H
#define XAN_TLSSERVERHELLO_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_gchandle.h>
#include <xan_tls_defines.h>
namespace rux
{
	namespace network
	{
		begin_declare_rux_class( TlsServerHello );
			friend class XTlsRecordLayerPacket;
			friend class XTls;
			~TlsServerHello();
		begin_declare_rux_class_members( TlsServerHello );
			rux::uint8 _tls_major_version;
			rux::uint8 _tls_minor_version;	
			rux::uint8 _session_id[ 32 ];
			rux::uint8 _session_id_length;
			XEnum_TlsCipherSuite _cipher_suite;
			rux::uint8 _compression_method;
			rux::uint32 _gmt_unix_timestamp;
			rux::uint8 _random[ 32 ];
		end_declare_rux_class_members( TlsServerHello );
			const rux::uint8* get_Random( void );
			rux::uint32 get_RandomLength( void );
		end_declare_rux_class();
	};
};
#endif
