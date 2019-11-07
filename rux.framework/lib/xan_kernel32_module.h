#ifndef XAN_KERNEL32_MODULE_H
#define XAN_KERNEL32_MODULE_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_system_module.h"
namespace rux
{
	namespace system
	{
#ifdef __WINDOWS__
		typedef USHORT (WINAPI *RtlCaptureStackBackTrace_t)( ULONG FramesToSkip , ULONG FramesToCapture , PVOID *BackTrace , PULONG BackTraceHash );
		class kernel32_module : public rux::system::module
		{
		public:
			RtlCaptureStackBackTrace_t _RtlCaptureStackBackTrace;
		public:
			kernel32_module( void );
			USHORT WINAPI RtlCaptureStackBackTrace( ULONG FramesToSkip , ULONG FramesToCapture , PVOID *BackTrace , PULONG BackTraceHash );
			virtual void on_loaded( void );
		};
#endif
	};
};
#endif
