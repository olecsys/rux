#ifndef XAN_ENUMS_H
#define XAN_ENUMS_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_includes.h"
namespace rux
{
	namespace log
	{
		enum XEnum_LogLevel
		{
			XEnum_LogLevel_Error = 40000 ,
			XEnum_LogLevel_Warn = 30000 ,
			XEnum_LogLevel_Debug = 10000 ,
			XEnum_LogLevel_Trace = 5000 , 
			XEnum_LogLevel_Verbose = 0
		};
		extern dll_internal const char* _log_levels[];
	};
	namespace engine
	{
		enum XEnum_EngineEvent
		{
			XEnum_EngineEvent_RegisteredThread = 0 ,
			XEnum_EngineEvent_UnregisteredThread = 1 ,
			XEnum_EngineEvent_BeforeLoadModule = 2 ,
			XEnum_EngineEvent_AfterLoadModule = 3 ,
			XEnum_EngineEvent_BeforeModuleDllInit = 4 ,
			XEnum_EngineEvent_AfterModuleDllInit = 5
		};
	};
};
#endif
