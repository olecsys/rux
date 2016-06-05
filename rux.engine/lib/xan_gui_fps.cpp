#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_gui_fps.h>
#include <xan_time.h>
namespace rux
{
	namespace gui
	{
		namespace engine
		{
			Fps::Fps( void )
			{
				_last_frame_time = 0;
				_frames_count = 0;
				_start_time = 0;
			};
			rux::uint64 Fps::get_Runtime( void )
			{
				if( _start_time == 0 )
					return 0;
				else
					return ::rux::XTime::get_NowAsWindowsTime() - _start_time;
			};
			void Fps::Increment( void )
			{
				if( _frames_count == 0 )
					_start_time= ::rux::XTime::get_NowAsWindowsTime();
				_frames_count++;
			};
			double Fps::get_FPS( void )
			{
				::rux::uint64 difference = ::rux::XTime::get_NowAsWindowsTime() - _start_time;
				return ( (double)_frames_count / ( (double)difference / (double)( 1000ULL * 10000ULL ) ) );
			};
			void Fps::Reset( void )
			{
				_frames_count = 0;
				_start_time = ::rux::XTime::get_NowAsWindowsTime();
			};
		};
	};
};
