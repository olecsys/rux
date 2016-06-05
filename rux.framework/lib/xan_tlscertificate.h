#ifndef XAN_TLSCERTIFICATE_H
#define XAN_TLSCERTIFICATE_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_array.h>
#include <xan_certificatex509v3.h>
namespace rux
{
	namespace network
	{
		begin_declare_rux_class( TlsCertificate );
			friend class XTlsRecordLayerPacket;
		begin_declare_rux_class_members( TlsCertificate );
			rux::XArray<rux::cryptography::XCertificateX509v3> _certificates;
		end_declare_rux_class_members( TlsCertificate );
			rux::Array<rux::cryptography::XCertificateX509v3>& get_Certificates( void );
		end_declare_rux_class();
	};
};
#endif
