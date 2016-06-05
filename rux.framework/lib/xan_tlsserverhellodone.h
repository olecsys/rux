#ifndef XAN_TLSSERVERHELLODONE_H
#define XAN_TLSSERVERHELLODONE_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_gchandle.h>
namespace rux
{
	namespace network
	{
		begin_declare_rux_class( TlsServerHelloDone );
			friend class XTlsRecordLayerPacket;
		begin_declare_rux_class_members( TlsServerHelloDone );
			rux::uint32 _length;
		end_declare_rux_class_members( TlsServerHelloDone );
		end_declare_rux_class();
	};
};
#endif
