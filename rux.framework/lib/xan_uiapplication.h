#ifndef XAN_UIAPPLICATION_H
#define XAN_UIAPPLICATION_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_window.h>
#include <xan_crtsect.h>
namespace rux
{
	namespace gui
	{	
		class application
		{
		public:
			static ::rux::uint8 run( ::rux::gui::Window* window , ::rux::uint8 show = 1 );
			static void gui_on_event( ::rux::gui::Event& xevent , void* owner );
			static void quit( void );
			static void dispatch( ::rux::gui::rux_dispatcher_t rux_dispatcher , void* param , ::rux::uint8 wait = 1 );
			static size_t add_schedule( ::rux::gui::rux_dispatcher_t rux_dispatcher , void* param , ::rux::uint64 timeout_in_microseconds );
			static void remove_schedule( size_t schedule_index );
			static void set_OnEvent( application_event_t on_event );
		};
	};
};
#endif