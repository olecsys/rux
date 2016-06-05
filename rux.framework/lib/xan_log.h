#ifndef XAN_LOG_H
#define XAN_LOG_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_includes.h>
#include <xan_enums.h>
namespace rux
{
	namespace log
	{
		typedef void (*write_log_t)( ::rux::int32 module_index , ::rux::log::XEnum_LogLevel level , const char* format , va_list ap );
		void WriteTrace( const char* format , ... );
		void WriteError( const char* format , ... );
		void WriteDebug( const char* format , ... );
		void WriteWarn( const char* format , ... );
		namespace engine
		{
			class globals
			{
			public:
				globals( void );
				::rux::log::write_log_t _write_log;
			};
		};
	};
};

#endif