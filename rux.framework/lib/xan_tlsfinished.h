#ifndef XAN_TLSFINISHED_H
#define XAN_TLSFINISHED_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_string.h>
#include <xan_tls_defines.h>
namespace rux
{
	namespace network
	{
		class Tls;
		begin_declare_rux_class( TlsFinished );
			friend class XTlsRecordLayerPacket;
		begin_declare_rux_class_members( TlsFinished );
			rux::uint8 _data_ptr[ 16 ];	
		end_declare_rux_class_members( TlsFinished );
			void set_VerifyData( Tls* tls_ptr , XEnum_TlsConnectionEnd connection_end , rux::XString& error );
			static const rux::uint32 _data_ptr_length = 16;
			static const rux::uint32 _verify_data_length = 12;
		end_declare_rux_class();
	};
};
#endif
