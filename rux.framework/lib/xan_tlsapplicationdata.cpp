#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_tlsapplicationdata.h>
#include <xan_keyvaluepair.h>
#include <xan_uint64.h>
#include <xan_boolean.h>
begin_implement_rux_class_ns( TlsApplicationData , rux::network );
	_network_format_function = NULL;
	_network_parse_function = NULL;
	_network_get_length_function = NULL;
end_implement_rux_class();