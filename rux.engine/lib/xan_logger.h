#ifndef XAN_LOGGER_H
#define XAN_LOGGER_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_log.h>
#include <xan_io_defines.h>
#include <xan_async_command_pool.h>
#include <xan_memorymanager.h>
namespace rux
{
	namespace log
	{
		namespace empties
		{
			static void ruxlog_write( int , const char* , va_list )
			{
			};
		};
		typedef void (*ruxlog_write_t)( int level , const char* format , va_list ap );

		class Logger;
#define RUX_MAX_LOG_LINES 512
		class LoggerFile
		{
			friend class rux::log::Logger;
		private:
			rux_volatile _inited;
			::rux::io::file _file;
			rux_volatile _busy[ RUX_MAX_LOG_LINES ];
			::rux::uint64 _times[ RUX_MAX_LOG_LINES ];
			::rux::uint32 _thread_ids[ RUX_MAX_LOG_LINES ];
			::rux::log::XEnum_LogLevel _levels[ RUX_MAX_LOG_LINES ];
			char* _allocated_lines[ RUX_MAX_LOG_LINES ];
			char* _allocated_line;
			size_t _allocated_line_size;
			size_t _allocated_lines_size[ RUX_MAX_LOG_LINES ];
			rux_volatile _write_index;
			size_t _read_index;
			rux_volatile _lines_count;
			size_t _task_index;
			rux_volatile _writing;
		public:
			LoggerFile( const char* module_name );
			~LoggerFile( void );
			rux_interlocked_return busy( void );
			rux_interlocked_return write_log( ::rux::log::XEnum_LogLevel level , const char* format , va_list ap );
			void write_log( void );
		};
		class Logger
		{
			friend class LoggerFile;
		private:
			LoggerFile* _logger_files[ RUX_MAX_MODULES_SIZE ];
			static void write_log_thread( void* param , size_t ___rux__thread_index1986 );
		public:
			Logger( void );
			~Logger( void );
			void write_log( ::rux::int32 module_index , ::rux::log::XEnum_LogLevel level , const char* format , va_list ap );
			void register_module( ::rux::int32 module_index , char* module_name );
			void unregister_module( ::rux::int32 module_index );
		};
		//extern dll_internal Logger* _logger;
		extern dll_internal ruxlog_write_t _ruxlog_write;
	};
};
#endif