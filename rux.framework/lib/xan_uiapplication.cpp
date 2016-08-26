#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_uiapplication.h>
#include <xan_time.h>
#include <xan_jsonserializer.h>
#include <xan_uipath.h>
namespace rux
{
	namespace gui
	{
		dll_internal ::rux::uint8 _show = 1;
		dll_internal Window* _main_window = NULL;
		dll_internal ::rux::uint8 _is_application_initialized = 0;
		dll_internal XCrtSect _cs_application;
		dll_internal application_event_t _on_event = NULL;
		rux::uint8 application::run( ::rux::gui::Window* window , ::rux::uint8 show )
		{
			rux::engine::initialize();
			if( _is_application_initialized == 0 )
			{
				_show = show;
				_main_window = window;
				::rux::engine::_globals->_gui_globals->_rux_gui_start( gui_on_event , NULL );
			}
			return _is_application_initialized;
		};
		void application::set_OnEvent( application_event_t on_event )
		{
			CRITICAL_SECTION_LOCK( _cs_application );
			_on_event = on_event;
			_cs_application.UnLock();
		};
		void application::quit( void )
		{
			::rux::engine::_globals->_gui_globals->_rux_gui_close_all_windows();
			if( ::rux::engine::_globals->_gui_globals->_rux_gui_windows_count() == 0 )
				::rux::engine::_globals->_gui_globals->_rux_gui_stop( gui_on_event , NULL );
		};
		void application::dispatch( ::rux::gui::rux_dispatcher_t rux_dispatcher , void* param , ::rux::uint8 wait )
		{
			::rux::engine::_globals->_gui_globals->_rux_gui_dispatch( rux_dispatcher , param , NULL , wait );	
		};
		size_t application::add_schedule( ::rux::gui::rux_dispatcher_t rux_dispatcher , void* param , ::rux::uint64 timeout_in_microseconds )
		{
			return ::rux::engine::_globals->_gui_globals->_gui_add_schedule( rux_dispatcher , param , timeout_in_microseconds );	
		};
		void application::remove_schedule( size_t schedule_index )
		{
			::rux::engine::_globals->_gui_globals->_gui_remove_schedule( schedule_index );
		};
		void application::gui_on_event( ::rux::gui::Event& xevent , void* )
		{
			switch( xevent._type )
			{
			case ::rux::gui::XEnum_EventType_GuiInitialize:
				{	
					_is_application_initialized = 1;
					if( _main_window )
					{
						if( _show == 1 )
							_main_window->Show();
						else
							_main_window->Hide();
					}
					CRITICAL_SECTION_LOCK( _cs_application );
					if( _on_event )
						_on_event( ::rux::gui::application_event_type_initialized );
					_cs_application.UnLock();
					break;
				}
			case ::rux::gui::XEnum_EventType_GuiDeInitialize:
				{
					CRITICAL_SECTION_LOCK( _cs_application );
					if( _on_event )
						_on_event( ::rux::gui::application_event_type_deinitialized );
					_cs_application.UnLock();
					_main_window = NULL;
					_is_application_initialized = 0;
					break;
				}
			case ::rux::gui::XEnum_EventType_RenderContextCreated:
				{
					rux::gui::RenderContextCreatedEvent* window_event = (rux::gui::RenderContextCreatedEvent*)&xevent;
					rux::gui::Window* window = (rux::gui::Window*)window_event->_window->get_Owner();
					if( window )
						window->AddRef();
					break;
				}
			case ::rux::gui::XEnum_EventType_WindowNormalState:
				{				
					rux::gui::WindowNormalStateEvent* window_event = (rux::gui::WindowNormalStateEvent*)&xevent;
					rux::gui::Window* window = (rux::gui::Window*)window_event->_window->get_Owner();
					window->raise_OnWindowRestored();
					break;
				}
			case ::rux::gui::XEnum_EventType_WindowLoaded:
				{				
					//rux::gui::WindowLoadedEvent* window_event = (rux::gui::WindowLoadedEvent*)&xevent;
					//Window* window = (rux::gui::Window*)window_event->_window->get_Owner();
					break;
				}
			case ::rux::gui::XEnum_EventType_WindowMinimizedState:
				{				
					rux::gui::WindowMinimizedStateEvent* window_event = (rux::gui::WindowMinimizedStateEvent*)&xevent;
					rux::gui::Window* window = (rux::gui::Window*)window_event->_window->get_Owner();
					window->raise_OnWindowMinimized();
					break;
				}
			case ::rux::gui::XEnum_EventType_WindowMaximizedState:
				{				
					rux::gui::WindowMaximizedStateEvent* window_event = (rux::gui::WindowMaximizedStateEvent*)&xevent;
					Window* window = (rux::gui::Window*)window_event->_window->get_Owner();
					window->raise_OnWindowMaximized();
					break;
				}
			case ::rux::gui::XEnum_EventType_WindowShown:
				{					
					rux::gui::WindowShownEvent* window_event = (rux::gui::WindowShownEvent*)&xevent;
					rux::gui::Window* window = (rux::gui::Window*)window_event->_window->get_Owner();
					window->private_Shown();
					break;
				}
			case ::rux::gui::XEnum_EventType_WindowHided:
				{					
					rux::gui::WindowHidedEvent* window_event = (rux::gui::WindowHidedEvent*)&xevent;
					rux::gui::Window* window = (rux::gui::Window*)window_event->_window->get_Owner();
					window->private_Hided();
					break;
				}
			case ::rux::gui::XEnum_EventType_WindowRender:
				{
					rux::gui::WindowRenderEvent* window_event = (rux::gui::WindowRenderEvent*)&xevent;
					rux::gui::Window* window = (rux::gui::Window*)window_event->_window->get_Owner();
					window->render( window_event->_render_context );
					break;
				}			
			case ::rux::gui::XEnum_EventType_WindowClosed:
				{					
					rux::gui::WindowClosedEvent* window_event = (rux::gui::WindowClosedEvent*)&xevent;
					rux::gui::Window* window = (rux::gui::Window*)window_event->_window->get_Owner();
					window->private_OnClosed();
					window->Release();					
					if( ::rux::engine::_globals->_gui_globals->_rux_gui_windows_count() == 0 )
						::rux::engine::_globals->_gui_globals->_rux_gui_stop( gui_on_event , NULL );
					break;
				}
			case ::rux::gui::XEnum_EventType_WindowKeyUp:
				{					
					rux::gui::WindowKeyUpEvent* window_event = (rux::gui::WindowKeyUpEvent*)&xevent;
					rux::gui::Window* window = (rux::gui::Window*)window_event->_window->get_Owner();
					window->private_KeyUp( window_event->_key , window_event->_shift , window_event->_control , window_event->_alt , window_event->_windows_vk , 1 );
					break;
				}
			case ::rux::gui::XEnum_EventType_WindowKeyDown:
				{					
					rux::gui::WindowKeyDownEvent* window_event = (rux::gui::WindowKeyDownEvent*)&xevent;
					rux::gui::Window* window = (rux::gui::Window*)window_event->_window->get_Owner();
					window->raise_OnKeyDown( window_event->_key , window_event->_shift , window_event->_control , window_event->_alt , window_event->_windows_vk , 1 );
					break;
				}
			case ::rux::gui::XEnum_EventType_WindowMove:
				{				
					rux::gui::WindowMoveEvent* window_event = (rux::gui::WindowMoveEvent*)&xevent;
					rux::gui::Window* window = (rux::gui::Window*)window_event->_window->get_Owner();					
					window->private_Relocation();
					break;
				}
			case ::rux::gui::XEnum_EventType_WindowSize:
				{				
					rux::gui::WindowSizeEvent* window_event = (rux::gui::WindowSizeEvent*)&xevent;
					rux::gui::Window* window = (rux::gui::Window*)window_event->_window->get_Owner();
					window->raise_OnWindowResize( window_event->_width , window_event->_height );
					break;
				}
			case ::rux::gui::XEnum_EventType_WindowKeyChar:
				{				
					rux::gui::WindowKeyCharEvent* window_event = (rux::gui::WindowKeyCharEvent*)&xevent;
					rux::gui::Window* window = (rux::gui::Window*)window_event->_window->get_Owner();
					window->raise_OnKeyChar( ::rux::XString( window_event->_utf8_character , XEnumCodePage_UTF8 , 1 , NULL , __FILE__ , __LINE__ ) , window_event->_shift , window_event->_control , window_event->_alt );
					break;
				}
			case ::rux::gui::XEnum_EventType_WindowMouseWheel:
				{					
					rux::gui::WindowMouseEvent* window_event = (rux::gui::WindowMouseEvent*)&xevent;
					rux::gui::Window* window = (rux::gui::Window*)window_event->_window->get_Owner();
					window->raise_OnMouseWheel( window_event , 1 );
					break;
				}
			case ::rux::gui::XEnum_EventType_WindowMouseMove:
				{					
					rux::gui::WindowMouseEvent* window_event = (rux::gui::WindowMouseEvent*)&xevent;
					rux::gui::Window* window = (rux::gui::Window*)window_event->_window->get_Owner();
					window->raise_OnMouseMove( window_event , 1 );
					break;
				}
			case ::rux::gui::XEnum_EventType_WindowLeftMouseButtonDown:
				{					
					rux::gui::WindowMouseEvent* window_event = (rux::gui::WindowMouseEvent*)&xevent;
					rux::gui::Window* window = (rux::gui::Window*)window_event->_window->get_Owner();
					window->raise_OnLeftMouseButtonDown( window_event , 1 );
					break;
				}
			case ::rux::gui::XEnum_EventType_WindowLeftMouseButtonUp:
				{					
					rux::gui::WindowMouseEvent* window_event = (rux::gui::WindowMouseEvent*)&xevent;
					rux::gui::Window* window = (rux::gui::Window*)window_event->_window->get_Owner();
					window->raise_OnLeftMouseButtonUp( window_event , 1 );	
					break;
				}
			case ::rux::gui::XEnum_EventType_WindowLeftMouseButtonDoubleClick:
				{					
					rux::gui::WindowMouseEvent* window_event = (rux::gui::WindowMouseEvent*)&xevent;
					rux::gui::Window* window = (rux::gui::Window*)window_event->_window->get_Owner();
					window->raise_OnLeftMouseButtonDoubleClick( window_event , 1 );
					break;
				}
			case ::rux::gui::XEnum_EventType_WindowRightMouseButtonDown:
				{					
					rux::gui::WindowMouseEvent* window_event = (rux::gui::WindowMouseEvent*)&xevent;
					rux::gui::Window* window = (rux::gui::Window*)window_event->_window->get_Owner();
					window->raise_OnRightMouseButtonDown( window_event , 1 );
					break;
				}
			case ::rux::gui::XEnum_EventType_WindowRightMouseButtonUp:
				{					
					rux::gui::WindowMouseEvent* window_event = (rux::gui::WindowMouseEvent*)&xevent;
					rux::gui::Window* window = (rux::gui::Window*)window_event->_window->get_Owner();
					window->raise_OnRightMouseButtonUp( window_event , 1 );	
					break;
				}
			case ::rux::gui::XEnum_EventType_WindowMouseWheelDown:
				{					
					rux::gui::WindowMouseEvent* window_event = (rux::gui::WindowMouseEvent*)&xevent;
					rux::gui::Window* window = (rux::gui::Window*)window_event->_window->get_Owner();
					window->raise_OnMouseWheelDown(window_event, 1);
					break;
				}
			case ::rux::gui::XEnum_EventType_WindowMouseWheelUp:
				{					
					rux::gui::WindowMouseEvent* window_event = (rux::gui::WindowMouseEvent*)&xevent;
					rux::gui::Window* window = (rux::gui::Window*)window_event->_window->get_Owner();
					window->raise_OnMouseWheelUp(window_event, 1);	
					break;
				}
			case ::rux::gui::XEnum_EventType_WindowMouseWheelDoubleClick:
				{					
					rux::gui::WindowMouseEvent* window_event = (rux::gui::WindowMouseEvent*)&xevent;
					rux::gui::Window* window = (rux::gui::Window*)window_event->_window->get_Owner();
					window->raise_OnMouseWheelDoubleClick(window_event, 1);
					break;
				}
			case ::rux::gui::XEnum_EventType_WindowRightMouseButtonDoubleClick:
				{					
					rux::gui::WindowMouseEvent* window_event = (rux::gui::WindowMouseEvent*)&xevent;
					rux::gui::Window* window = (rux::gui::Window*)window_event->_window->get_Owner();
					window->raise_OnRightMouseButtonDoubleClick( window_event , 1 );
					break;
				}
			}
		};
	};
};