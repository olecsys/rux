#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_winmm_module.h>
namespace rux
{
	namespace system
	{
#ifdef __WINDOWS__
		winmm_module::winmm_module( void )
			: rux::system::module( "Winmm.dll" )
		{
			_timeGetDevCaps = 0;
			_timeBeginPeriod = 0;
			_timeEndPeriod = 0;
			_timeGetTime = 0;
		};
		void winmm_module::on_loaded( void )
		{
			system_module_dlsym( timeGetDevCaps );
			system_module_dlsym( timeBeginPeriod );
			system_module_dlsym( timeEndPeriod );
			system_module_dlsym( timeGetTime );
		};
		MMRESULT winmm_module::timeGetDevCaps( LPTIMECAPS ptc , UINT cbtc )
		{
			load();
			return _timeGetDevCaps( ptc , cbtc );
		};
		MMRESULT winmm_module::timeBeginPeriod( UINT uPeriod )
		{
			load();
			return _timeBeginPeriod( uPeriod );
		};
		MMRESULT winmm_module::timeEndPeriod( UINT uPeriod )
		{
			load();
			return _timeEndPeriod( uPeriod );
		};
		DWORD winmm_module::timeGetTime( void )
		{
			load();
			if( _timeGetTime )
				return _timeGetTime();
			else
				return ::GetTickCount();
		};
#endif
	};
};