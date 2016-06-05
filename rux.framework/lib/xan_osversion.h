#ifndef XAN_OSVERSION_H
#define XAN_OSVERSION_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_string.h>
enum XEnum_ProcessorArchitecture
{
	XEnum_ProcessorArchitecture_X64_AMD,
	XEnum_ProcessorArchitecture_X64_IA,
	XEnum_ProcessorArchitecture_X86,
	XEnum_ProcessorArchitecture_X86_X64
};
enum XEnum_OSVersion
{
	XEnum_OSVersion_WindowsXP,
	XEnum_OSVersion_WindowsServer2008,
	XEnum_OSVersion_WindowsServer2008R2,
	XEnum_OSVersion_Windows7,
	XEnum_OSVersion_WindowsVista,
	XEnum_OSVersion_WindowsServer2003,
	XEnum_OSVersion_Windows2000,
	XEnum_OSVersion_Unknown
};
namespace rux
{	
	namespace diagnostics
	{	
		begin_declare_rux_class( OSVersion );
		begin_declare_rux_class_members( OSVersion );
			XEnum_OSVersion _os_version;
			XEnum_ProcessorArchitecture _processor_architecture;
		end_declare_rux_class_members( OSVersion );
			rux::String& ToString( void );
			static OSVersion& get_OSVersion( rux::XString& error );
			uint8 get_IsWindows( void );
			XEnum_ProcessorArchitecture get_ProcessorArchitecture( void );
		end_declare_rux_class();
	};
};
#endif
