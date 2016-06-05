#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_logger.h>
#include <xan_interlocked.h>
#include <xan_time.h>
#include <xan_task.h>
#include <xan_tasker.h>
namespace rux
{
	namespace log
	{
		dll_internal ruxlog_write_t _ruxlog_write = ::rux::log::empties::ruxlog_write;
		//dll_internal Logger* _logger = NULL;
		LoggerFile::LoggerFile( const char* module_name )
		{
			_writing = 0;
			_allocated_line = 0;
			_allocated_line_size = 0;
			for( size_t index0 = 0 ; index0 < RUX_MAX_LOG_LINES ; index0++ )
			{
				_busy[ index0 ] = 0;
				_allocated_lines[ index0 ] = 0;
				_allocated_lines_size[ index0 ] = 0;
			}
			_write_index = 0;
			_read_index = 0;
			_lines_count = 0;
			char filename[ 1024 ] = {0};
			snprintf( filename , 1024 , "%s/rux_logs" , ::rux::engine::_globals->_executable_directory );
			::rux::io::directory::create( filename );
			snprintf( filename , 1024 , "%s/rux_logs/%s.txt" , ::rux::engine::_globals->_executable_directory , module_name );
			_file.open( filename , XCreateWriteBinary );
			if( _file.opened() )
			{	
				::rux::task::add( _task_index , Logger::write_log_thread , this , 500 , XThreadMode_Repeat );
				XInterlocked::Exchange( &_inited , 1 );
			}
			else
			{
				_task_index = SIZE_MAX;
				XInterlocked::Exchange( &_inited , 0 );
			}
		};
		LoggerFile::~LoggerFile( void )
		{
			if( _task_index != SIZE_MAX )
				::rux::task::remove( _task_index );
			if( _file.opened() )
				_file.close();
			for( size_t index0 = 0 ; index0 < RUX_MAX_LOG_LINES ; index0++ )
			{
				if( _allocated_lines[ index0 ] )
					::rux::engine::free_mem( _allocated_lines[ index0 ] );
			}
			if( _allocated_line )
				::rux::engine::free_mem( _allocated_line );
		};
		rux_interlocked_return LoggerFile::write_log( ::rux::log::XEnum_LogLevel level , const char* format , va_list ap )
		{
			rux_interlocked_return lines_count = XInterlocked::Increment( &_lines_count );
			if( XInterlocked::CompareExchange( &_inited , 0 , 0 ) )
			{
				::rux::uint64 now = rux_get_now_as_local_unix_time();
				rux_interlocked_return index = 0;
				index = ( XInterlocked::Increment( &_write_index ) - 1 ) % RUX_MAX_LOG_LINES;
				while( XInterlocked::CompareExchange( &_busy[ index ] , 2 , 0 ) != 0 )
				{
					XInterlocked::Decrement( &_lines_count );
					write_log();
					XInterlocked::Increment( &_lines_count );
					::rux::threading::XThread::Sleep( 1 );
				}
				_times[ index ] = now;
				_levels[ index ] = level;
				_thread_ids[ index ] = ::rux::threading::XThread::get_CurrentThreadId();
				int size = ::rux::_vscprintf( format , ap ) + 1;
				if( size > 1 )
				{
					if( _allocated_lines_size[ index ] < size )
					{
						if( _allocated_lines[ index ] )
							::rux::engine::free_mem( _allocated_lines[ index ] );
						_allocated_lines[ index ] = alloc_array_macros( char , size );
						_allocated_lines_size[ index ] = size;
					}
					vsprintf_s( _allocated_lines[ index ] , _allocated_lines_size[ index ] , format , ap );
				}
				XInterlocked::Exchange( &_busy[ index ] , 1 );
				return lines_count;
			}
			else
			{
				XInterlocked::Decrement( &_lines_count );
				return 0;
			}
		};
		void LoggerFile::write_log( void )
		{
			if( XInterlocked::CompareExchange( &_writing , 1 , 0 ) == 0 )
			{
				if( _file.size() >= 1024ULL * 1024ULL )
				{
					_file.size( 0 );
					_file.seek( 0 );
				}
				size_t written = 0;
				::rux::log::XEnum_LogLevel level = ::rux::log::XEnum_LogLevel_Trace;
				char now_string[ 64 ] = {0};
				rux_interlocked_return count = XInterlocked::CompareExchange( &_lines_count , 0 , 0 );
				while( count > 0 )
				{			
					size_t old_read_index = _read_index;
					if( _read_index >= RUX_MAX_LOG_LINES )
						_read_index = 0;
					if( XInterlocked::CompareExchange( &_busy[ _read_index ] , 2 , 1 ) == 1 )
					{
						if( _allocated_lines[ _read_index ] && _allocated_lines[ _read_index ][ 0 ] != 0 )
						{	
							::rux::XTime::UnixTimeInMicroSecondsToString( _times[ _read_index ] , "%Y.%m.%d %H:%M:%S.%MS" , now_string , 64 );						
							level = _levels[ _read_index ];
							int size = ::rux::_scprintf( "%s [%.2u][%.8x] %s - %s\n" , now_string , _thread_ids[ _read_index ] , _thread_ids[ _read_index ] , ::rux::log::_log_levels[ level ] , _allocated_lines[ _read_index ] ) + 1;
							if( _allocated_line_size < size )
							{
								if( _allocated_line )
									::rux::engine::free_mem( _allocated_line );
								_allocated_line = alloc_array_macros( char , size );
								_allocated_line_size = size;
							}
							size = snprintf( _allocated_line , _allocated_line_size , "%s [%.2u][%.8x] %s - %s\n" , now_string , _thread_ids[ _read_index ] , _thread_ids[ _read_index ] , ::rux::log::_log_levels[ level ] , _allocated_lines[ _read_index ] );
							_allocated_lines[ _read_index ][ 0 ] = 0;
							XInterlocked::Exchange( &_busy[ _read_index ] , 0 );
							_file.write_bytes( _allocated_line , size , written );						
						}
						else
							XInterlocked::Exchange( &_busy[ _read_index ] , 0 );
						_read_index++;
						count = XInterlocked::Decrement( &_lines_count );
					}
					else
					{
						_read_index = old_read_index;
						count = XInterlocked::CompareExchange( &_lines_count , 0 , 0 );
					}
				}
				XInterlocked::Exchange( &_writing , 0 );
			}
		};
		rux_interlocked_return LoggerFile::busy( void )
		{
			return XInterlocked::CompareExchange( &_lines_count , 0 , 0 );
		};
		Logger::Logger( void )
		{
		};
		Logger::~Logger( void )
		{
		};
		void Logger::write_log( ::rux::int32 module_index , ::rux::log::XEnum_LogLevel level , const char* format , va_list ap )
		{
			if( _logger_files[ module_index ] && _logger_files[ module_index ]->write_log( level , format , ap ) )
				::rux::task::force_execute( _logger_files[ module_index ]->_task_index );
		};
		void Logger::register_module( ::rux::int32 module_index , char* module_name )
		{
			if( _logger_files[ module_index ] == 0 )
			{
				LoggerFile* logger_file = alloc_object_1_param_macros( LoggerFile , module_name );
				if( XInterlocked::CompareExchange( &logger_file->_inited , 0 , 0 ) )
					_logger_files[ module_index ] = logger_file;
				else
					::rux::engine::free_object< LoggerFile >( logger_file );
			}
		};
		void Logger::unregister_module( ::rux::int32 module_index )
		{
			if( _logger_files[ module_index ] )
			{
				if( XInterlocked::CompareExchange( &_logger_files[ module_index ]->_inited , 0 , 1 ) == 1 )
				{
					while( _logger_files[ module_index ]->busy() )
						::rux::threading::XThread::Sleep( 1 );
				}
				::rux::engine::free_object< LoggerFile >( _logger_files[ module_index ] );
				_logger_files[ module_index ] = 0;
			}
		};
		void Logger::write_log_thread( void* param , size_t ___rux__thread_index1986 )
		{
			LoggerFile* logger_file = (LoggerFile*)param;
			logger_file->write_log();
		};
	};
};