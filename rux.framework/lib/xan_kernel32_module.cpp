#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_kernel32_module.h"
namespace rux
{
	namespace system
	{
#ifdef __WINDOWS__
		kernel32_module::kernel32_module( void )
			: rux::system::module( "kernel32.dll" )
		{
			_RtlCaptureStackBackTrace = 0;
		};
		USHORT WINAPI kernel32_module::RtlCaptureStackBackTrace( ULONG FramesToSkip , ULONG FramesToCapture , PVOID *BackTrace , PULONG BackTraceHash )
		{
			load();
			return _RtlCaptureStackBackTrace( FramesToSkip , FramesToCapture , BackTrace , BackTraceHash );
		};
		void kernel32_module::on_loaded( void )
		{
			system_module_dlsym( RtlCaptureStackBackTrace );
		};
#endif
	};
};