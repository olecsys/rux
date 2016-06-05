#ifndef XAN_WINMM_MODULE_H
#define XAN_WINMM_MODULE_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_system_module.h>
namespace rux
{
	namespace system
	{
#ifdef __WINDOWS__
		typedef MMRESULT (*timeGetDevCaps_t)( LPTIMECAPS ptc , UINT cbtc );
		typedef MMRESULT (*timeBeginPeriod_t)( UINT uPeriod );
		typedef MMRESULT (*timeEndPeriod_t)( UINT uPeriod );
		typedef DWORD (*timeGetTime_t)( void );
		class winmm_module : public rux::system::module
		{
		protected:
			timeGetDevCaps_t _timeGetDevCaps;
			timeBeginPeriod_t _timeBeginPeriod;
			timeEndPeriod_t _timeEndPeriod;
			timeGetTime_t _timeGetTime;
		public:
			winmm_module( void );
			virtual void on_loaded( void );
			MMRESULT timeGetDevCaps( LPTIMECAPS ptc , UINT cbtc );
			MMRESULT timeBeginPeriod( UINT uPeriod );
			MMRESULT timeEndPeriod( UINT uPeriod );
			DWORD timeGetTime(void);
		};
#endif
	};
};
#endif
