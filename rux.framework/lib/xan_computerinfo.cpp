#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_computerinfo.h>
#include <xan_keyvaluepair.h>
#include <xan_uint64.h>
#include <xan_boolean.h>
begin_implement_rux_class_ns( ComputerInfo , rux::diagnostics );
	#ifdef __WINDOWS__
#ifdef _M_IA64_
	_processor_architecture = XEnum_ProcessorArchitecture_X64;
#endif
#ifdef _M_X64_
	_processor_architecture = XEnum_ProcessorArchitecture_X64;
#endif
#ifdef _M_IX86
	_processor_architecture = XEnum_ProcessorArchitecture_X86;
#endif
#endif
#ifdef __LINUX__
#ifdef __i386__
	 _processor_architecture = XEnum_ProcessorArchitecture_X86;
#endif
#ifdef __i486__
	 _processor_architecture = XEnum_ProcessorArchitecture_X86;
#endif
#ifdef __i586__
	 _processor_architecture = XEnum_ProcessorArchitecture_X86;
#endif
#ifdef __i686__
	 _processor_architecture = XEnum_ProcessorArchitecture_X86;
#endif
#ifdef __IA64__
	 _processor_architecture = XEnum_ProcessorArchitecture_X64_IA;
#endif
#ifdef __ia64__
	 _processor_architecture = XEnum_ProcessorArchitecture_X64_IA;
#endif
#ifdef _IA64
	 _processor_architecture = XEnum_ProcessorArchitecture_X64_IA;
#endif
#ifdef __amd64__
	 _processor_architecture = XEnum_ProcessorArchitecture_X64_AMD;
#endif
#ifdef __amd64
	 _processor_architecture = XEnum_ProcessorArchitecture_X64_AMD;
#endif
#ifdef __x86_64__
	 _processor_architecture = XEnum_ProcessorArchitecture_X86_X64;
#endif
#ifdef __x86_64
	 _processor_architecture = XEnum_ProcessorArchitecture_X86_X64;
#endif
#endif
end_implement_rux_class();
namespace rux
{
	namespace diagnostics
	{
		XEnum_ProcessorArchitecture XComputerInfo::get_ProcessorArchitecture( void )
		{
			return (*this)()->_processor_architecture;
		};
	};
};