#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_tlscertificate.h>
#include <xan_keyvaluepair.h>
#include <xan_uint64.h>
#include <xan_boolean.h>
begin_implement_rux_class_ns( TlsCertificate , rux::network );
end_implement_rux_class();
namespace rux
{
	namespace network
	{
		rux::Array<rux::cryptography::XCertificateX509v3>& XTlsCertificate::get_Certificates( void )
		{
			return (*this)()->_certificates++;
		};
	};
};