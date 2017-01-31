#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_code_performance_counter.h>
namespace rux
{
	namespace performance
	{
		CodePerformanceCounter::CodePerformanceCounter( ::rux::byte is_printf )
		{
			_is_printf = is_printf;
		};
		CodePerformanceCounter::~CodePerformanceCounter( void )
		{
			if( _started )
				Stop();
		};
		void CodePerformanceCounter::Start( const char* function , const char* file , ::rux::int32 line )
		{
			_started = 1;
			_file = file;
			_line = line;
			_function = function;
#ifdef __WINDOWS__
			LARGE_INTEGER qpc;
			if( QueryPerformanceCounter( &qpc ) )
				_performance = qpc.QuadPart;
#elif defined( __UNIX__ )
			struct timespec ts = { 0 };
			clock_gettime( CLOCK_MONOTONIC , &ts );
			_performance = ts.tv_sec * 1000000000ULL + ts.tv_nsec;
#endif
		};
		::rux::uint64 CodePerformanceCounter::Stop( void )
		{
#ifdef __WINDOWS__
			LARGE_INTEGER qpc;
			if( QueryPerformanceCounter( &qpc ) )
				_performance = qpc.QuadPart - _performance;
			if( ::rux::engine::_globals->_query_performance_frequency > 0 )
			{		
				if( 1000000000ULL >= ::rux::engine::_globals->_query_performance_frequency )
					_performance *= 1000000000ULL / ::rux::engine::_globals->_query_performance_frequency;
				else
					_performance *= (double)1000000000ULL / (double)::rux::engine::_globals->_query_performance_frequency;
			}
#elif defined( __UNIX__ )
			struct timespec ts = { 0 };
			clock_gettime( CLOCK_MONOTONIC , &ts );
			_performance = ts.tv_sec * 1000000000ULL + ts.tv_nsec - _performance;
#endif	
			::rux::uint64 microseconds = _performance / 1000ULL;
			if( _is_printf )
				printf( "%s, %s:%d, runtime code " I64u" microseconds\n" , _function , _file , _line , microseconds );
			_started = 0;
			return microseconds;
		};
	};
};