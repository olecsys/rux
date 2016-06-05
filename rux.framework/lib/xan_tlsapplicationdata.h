#ifndef XAN_TLSAPPLICATIONDATA_H
#define XAN_TLSAPPLICATIONDATA_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_networkaddress.h>
namespace rux
{
	namespace network
	{
		begin_declare_rux_class( TlsApplicationData );
			friend class XTls;
			friend class XTlsRecordLayerPacket;
		begin_declare_rux_class_members( TlsApplicationData );
			XObject _send_data;
			XObject _received_data;
			NetworkFormatFunction _network_format_function;
			NetworkParseFunction _network_parse_function;
			NetworkGetLengthFunction _network_get_length_function;
		end_declare_rux_class_members( TlsApplicationData );
		end_declare_rux_class();
	};
};
#endif
