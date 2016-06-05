#ifndef XAN_CODE_PERFORMANCE_COUNTER_H
#define XAN_CODE_PERFORMANCE_COUNTER_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_defines.h>
namespace rux
{
	namespace performance
	{
#define LAZY_PERFORMANCE ::rux::performance::CodePerformanceCounter __25__07__1986__cpc;\
		__25__07__1986__cpc.Start( __FUNCTION__ , __FILE__ , __LINE__ )		
#define START_PERFORMANCE( cpc ) cpc.Start( __FUNCTION__ , __FILE__ , __LINE__ )
#define STOP_PERFORMANCE( cpc ) cpc.Stop( 1 )
		class CodePerformanceCounter
		{
			const char* _file;
			::rux::int32 _line;
			const char* _function;
			rux::uint64 _performance;
			::rux::byte _is_printf;
			::rux::byte _started;
		public:
			CodePerformanceCounter( ::rux::byte is_printf = 1 );
			~CodePerformanceCounter( void );
			void Start( const char* function , const char* file , ::rux::int32 line );
			rux::uint64 Stop( void );
		};
	};
};
#endif
