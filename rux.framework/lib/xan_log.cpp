#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_log.h"
#include "xan_global_variables.h"
#include "xan_engine_globals.h"
namespace rux
{
	namespace log
	{
		void WriteTrace( const char* format , ... )
		{
			if( ::rux::engine::_globals && ::rux::engine::_globals->_log_globals->_write_log )
			{
				if( ::rux::engine::_globals->_log_level >= ::rux::log::XEnum_LogLevel_Trace )
				{
					va_list ap;
					va_start( ap , format );
					::rux::engine::_globals->_log_globals->_write_log( _rux_current_module_index , ::rux::log::XEnum_LogLevel_Trace , format , ap );
					va_end( ap );
				}
			}
		};
		void WriteError( const char* format , ... )
		{
			if( ::rux::engine::_globals && ::rux::engine::_globals->_log_globals->_write_log )
			{
				if( ::rux::engine::_globals->_log_level >= ::rux::log::XEnum_LogLevel_Error )
				{
					va_list ap;
					va_start( ap , format );
					::rux::engine::_globals->_log_globals->_write_log( _rux_current_module_index , ::rux::log::XEnum_LogLevel_Error , format , ap );
					va_end( ap );
				}
			}
		};
		void WriteDebug( const char* format , ... )
		{
			if( ::rux::engine::_globals && ::rux::engine::_globals->_log_globals->_write_log )
			{
				if( ::rux::engine::_globals->_log_level >= ::rux::log::XEnum_LogLevel_Debug )
				{
					va_list ap;
					va_start( ap , format );
					::rux::engine::_globals->_log_globals->_write_log( _rux_current_module_index , ::rux::log::XEnum_LogLevel_Debug , format , ap );
					va_end( ap );
				}
			}
		};
		void WriteWarn( const char* format , ... )
		{
			if( ::rux::engine::_globals && ::rux::engine::_globals->_log_globals->_write_log )
			{
				if( ::rux::engine::_globals->_log_level >= ::rux::log::XEnum_LogLevel_Warn )
				{
					va_list ap;
					va_start( ap , format );
					::rux::engine::_globals->_log_globals->_write_log( _rux_current_module_index , ::rux::log::XEnum_LogLevel_Warn , format , ap );
					va_end( ap );
				}
			}
		};
		namespace engine
		{
			globals::globals( void )
			{
				_write_log = 0;
			};
		};
	};
};