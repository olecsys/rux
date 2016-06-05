#ifndef XAN_COMPUTERINFO_H
#define XAN_COMPUTERINFO_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_osversion.h>
namespace rux
{	
	namespace diagnostics
	{	
		begin_declare_rux_class( ComputerInfo );
		begin_declare_rux_class_members( ComputerInfo );
			XEnum_ProcessorArchitecture _processor_architecture;
		end_declare_rux_class_members( ComputerInfo );
			XEnum_ProcessorArchitecture get_ProcessorArchitecture( void );
		end_declare_rux_class();
	};
};
#endif
