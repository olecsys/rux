#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_gui_window.h>
#include <xan_gui_engine.h>
#include <xan_thread.h>
#include <xan_log.h>
#include <xan_gl_context.h>
namespace rux
{
	namespace gui
	{
		namespace engine
		{				
			Screen::Screen()
			{	
				_left = 0;
				_top = 0;
				_width = 0;
				_height = 0;
#ifdef __WINDOWS__
				_hdc = NULL;
#endif
			};
			Screen::Screen( ::rux::int32 left , ::rux::int32 top , ::rux::int32 width , ::rux::int32 height )
			{
				_left = left;
				_top = top;
				_width = width;
				_height = height;
#ifdef __WINDOWS__
				_hdc = NULL;
#endif
			};
			Screen::Screen( const Screen& screen )
			{
				_left = screen._left;
				_top = screen._top;
				_width = screen._width;
				_height = screen._height;
#ifdef __WINDOWS__
				_hdc = screen._hdc;
#endif
			};
			Screen& Screen::operator =( const Screen& screen )
			{
				_left = screen._left;
				_top = screen._top;
				_width = screen._width;
				_height = screen._height;
#ifdef __WINDOWS__
				_hdc = screen._hdc;
#endif
				return *this;
			};
#ifdef __WINDOWS__
			Screen::Screen( ::rux::int32 left , ::rux::int32 top , ::rux::int32 width , ::rux::int32 height , HDC hdc )
			{
				_left = left;
				_top = top;
				_width = width;
				_height = height;
				_hdc = hdc;
			};
			BOOL CALLBACK Screen::monitor_enum( HMONITOR , HDC hdc , LPRECT rect , LPARAM lparam )
			{
				XMallocArray< Screen* >* screens_ptr = cpp_ptr_cast( XMallocArray< Screen* >* , reinterpret_cast< void* >( lparam ) );
				screens_ptr->Add( alloc_object_5_params_macros( ::rux::gui::engine::Screen , rect->left , rect->top , rect->right - rect->left , rect->bottom - rect->top , hdc ) );
				return TRUE;
			};
#endif
			void Screen::free( XMallocArray< ScreenBase* >* screens )
			{
				for( ; screens->Count() > 0 ; )
				{
					rux::engine::free_object< ::rux::gui::engine::Screen >( ( (rux::gui::engine::Screen*)screens->operator[]( 0 ) ) );
					screens->RemoveAt( 0 );
				}
				rux::engine::free_object< XMallocArray< ScreenBase* > >( screens );
			};
			XMallocArray< ScreenBase* >* Screen::get_Screens( void )
			{
				XMallocArray< ::rux::gui::ScreenBase* >* screens = alloc_object_macros( XMallocArray< ::rux::gui::ScreenBase* > );
#ifdef __WINDOWS__
				EnumDisplayMonitors( NULL , NULL , monitor_enum , (LPARAM)screens );
#else
#ifdef __ANDROID__
#else
				Display* display = ::rux::engine::_globals->_x11_module.XOpenDisplay( NULL );
				if( display )
				{
					rux::uint32 count = ScreenCount( display );
					::Window window;
					XWindowAttributes window_attributes;
					for( ::rux::uint32 index0 = 0 ; index0 < count ; index0++ )
					{
						window = RootWindow( display , index0 );
						Atom net_wm_strut = ::rux::engine::_globals->_x11_module.XInternAtom
							(display, "_NET_WM_STRUT", False);
						Atom net_wm_strut_partial = ::rux::engine::_globals->_x11_module.XInternAtom
							(display, "_NET_WM_STRUT_PARTIAL", False);
						Atom type = None;
						int form = 0;
						unsigned long remain = 0;
						unsigned long len = 0;
						Atom* list = 0;
						if(net_wm_strut_partial != None
							&& ::rux::engine::_globals->_x11_module.XGetWindowProperty(display, window, net_wm_strut_partial, 0
							, LONG_MAX, False, XA_CARDINAL, &type, &form, &len, &remain, (::rux::uint8**)&list) == Success)
						{
							if(len)
								::rux::log::WriteDebug("_NET_WM_STRUT_PARTIAL, screen#%u, left=%d, right=%d, top=%d, bottom=%d", index0
								, (int)list[0], (int)list[1], (int)list[2], (int)list[3]);

							::rux::engine::_globals->_x11_module.XFree(list);
						}
						else
							len = 0;
							
						if(len == 0 && net_wm_strut != None
							&& ::rux::engine::_globals->_x11_module.XGetWindowProperty(display, window, net_wm_strut, 0
							, LONG_MAX, False, XA_CARDINAL, &type, &form, &len, &remain, (::rux::uint8**)&list) == Success)
						{
							if(len)
								::rux::log::WriteDebug("_NET_WM_STRUT, screen#%u, left=%d, right=%d, top=%d, bottom=%d", index0
								, (int)list[0], (int)list[1], (int)list[2], (int)list[3]);

							::rux::engine::_globals->_x11_module.XFree(list);
						}
						if( ::rux::engine::_globals->_xrandr_module._XRRGetScreenResources
							&& ::rux::engine::_globals->_xrandr_module._XRRFreeScreenResources
							&& ::rux::engine::_globals->_xrandr_module._XRRFreeCrtcInfo
							&& ::rux::engine::_globals->_xrandr_module._XRRGetCrtcInfo )
						{
							XRRScreenResources* screens_res = ::rux::engine::_globals->_xrandr_module.XRRGetScreenResources( display , window );
							XRRCrtcInfo* info = 0;
							for( size_t index0 = 0 ; index0 < screens_res->ncrtc ; index0++ )
							{
								info = ::rux::engine::_globals->_xrandr_module.XRRGetCrtcInfo( display , screens_res , screens_res->crtcs[ index0 ] );
								screens->Add( (rux::gui::ScreenBase*)alloc_object_4_params_macros( ::rux::gui::engine::Screen , info->x , info->y , info->width , info->height ) );
								::rux::engine::_globals->_xrandr_module.XRRFreeCrtcInfo( info );
							}
							::rux::engine::_globals->_xrandr_module.XRRFreeScreenResources( screens_res );
						}
						else
						{
							::rux::engine::_globals->_x11_module.XGetWindowAttributes( display , window , &window_attributes );
							screens->Add( (rux::gui::ScreenBase*)alloc_object_4_params_macros( ::rux::gui::engine::Screen , window_attributes.x , window_attributes.y , window_attributes.width , window_attributes.height ) );
						}
					}
					::rux::engine::_globals->_x11_module.XCloseDisplay( display );
				}
#endif
#endif
				return screens;
			};
			rux::int32 Screen::get_Left( void )
			{
				return _left;
			};
			rux::int32 Screen::get_Top( void )
			{
				return _top;
			};
			rux::int32 Screen::get_Width( void )
			{
				return _width;
			};
			rux::int32 Screen::get_Height( void )
			{
				return _height;
			};			
			dll_internal XMallocArray< ::rux::gui::engine::Window* > _windows;
			Window::Window()
			{
				_is_allow_relocate = 1;
				_is_allow_resize = 1;
				_is_allow_close = 1;
				_both_size_and_move_msg = 0;
				_ref = 1;
				_cursor = ::rux::gui::XEnum_Cursors_Arrow;
				_on_event = 0;
				_left = 0;
				_top = 0;
				_height = 200;
				_width = 200;
				_min_width = 20;
				_min_height = 20;
				_windows_index = 0xffffffff;
				_render_context = 0;
				_is_top_most = 0;
				_is_visible = 0;
#ifdef __WINDOWS__
				_hwnd = 0;
				_hdc = 0;				
				_transparent_window_bitmap = 0;
#else
#ifdef __ANDROID__
#else
				_x_display = 0;
#endif
#endif
				_window_state = ::rux::gui::XEnum_WindowState_Normal;
				_is_closing = 0;
				_is_transparent_window = 0;
			};
			Window::~Window()
			{				
				
			};
			void Window::free_resources( void )
			{
				if( _render_context )
				{
					rux::engine::free_object< ::rux::gui::engine::RenderContext >( _render_context );
					_render_context = 0;
				}
#ifdef __WINDOWS__		
				if( _transparent_window_bitmap )
				{
					DeleteObject( _transparent_window_bitmap );
					_transparent_window_bitmap = NULL;
				}
				if( _hwnd )
				{
					if( _is_transparent_window == 1 )
						DeleteDC( _hdc );
					else
						ReleaseDC( _hwnd , _hdc );
					_hdc = 0;
					SetWindowLongPtrA( _hwnd , GWLP_USERDATA , 0 );
					if( rux::gui::engine::_winapi_trace )
						::rux::log::WriteTrace( "DestroyWindow, hwnd=%p" , _hwnd );
					DestroyWindow( _hwnd );
					_hwnd = 0;
				}
#else
#ifdef __ANDROID__
#else
				if( _x_display )
				{
					::rux::engine::_globals->_x11_module.XDestroyWindow( _x_display , _x_window );
					::rux::engine::_globals->_x11_module.XCloseDisplay( _x_display );
					_x_display = 0;
				}
#endif
#endif
			};
			void Window::raise_event( ::rux::gui::Event& xevent )
			{
				if( _on_event )
					_on_event( xevent , NULL );
			};
#ifdef __UNIX__
#ifdef __ANDROID__
#else
			rux::int32 WaitForMapNotify( Display* d , XEvent* e , char* arg )
			{
				return true;
			};	
			rux::int32 WaitForDelete( Display* d , XEvent* e , char* arg )
			{
				if( d && e && arg && ( e->type == ClientMessage ) && ( e->xclient.window == *(::Window*)arg ) && ( e->xclient.message_type == ::rux::gui::engine::_net_close_window || e->xclient.message_type == ::rux::gui::engine::_rux_delete_window ) )
					return true;
				else
					return false;
			};		
#endif
#endif
			void Window::RestartRender()
			{
				if(_render_context)
					_render_context->_need_restart = true;
			}
			void Window::set_MinWidth( ::rux::int32 width )
			{
				if( _min_width != width )
				{
					_min_width = width;
					if( _width < width )
						set_Width( width );
				}
			};
			rux::gui::XEnum_WindowState Window::get_WindowState( void )
			{
				return _window_state;
			};
			void Window::set_MinHeight( ::rux::int32 height )
			{
				if( _min_width != height )
				{
					_min_height = height;
					if( _height < height )
						set_Height( height );
				}
			};
			void Window::set_Owner( void* owner )
			{
				_owner = owner;
			};
			void* Window::get_Owner( void )
			{
				return _owner;
			};
			void Window::check( const char* __file__ , ::rux::int32 __line__ )
			{
				::rux::log::WriteTrace( "check, %p, %s:%d" , this , __file__ , __line__ );
#ifdef __WINDOWS__
				if( _hwnd == NULL )
#else
#ifdef __ANDROID__
#else
				if( _x_display == NULL )
#endif
#endif
					rux::gui::engine::dispatch( ::rux::gui::engine::dispatch_create_window , this );
			};
			void Window::dispatch_close_window( void* param )
			{
				Window* window = cpp_ptr_cast(Window* , param );
#ifdef __WINDOWS__
				if( window->_hwnd )					
					SendMessage( window->_hwnd , WM_CLOSE , 0 , 0 );
#else
#ifdef __ANDROID__
#else
				if( window->_x_display )
				{	
					rux::gui::engine::lock_display( NULL );
					if( ::rux::gui::engine::is_allowed_atom( ::rux::gui::engine::_x_display_main , window->_x_window , ::rux::engine::_globals->_x11_module.XInternAtom( ::rux::gui::engine::_x_display_main , "_NET_WM_ACTION_CLOSE" , False ) ) == 1 )
					{
						XEvent x_event;
						memset( &x_event , 0 , sizeof( x_event ) );					
						x_event.xclient.type = ClientMessage;
						x_event.xclient.window = window->_x_window;
						x_event.xclient.message_type = ::rux::gui::engine::_net_close_window;
						x_event.xclient.format = 32;
						x_event.xclient.data.l[ 0 ] = CurrentTime;
						x_event.xclient.data.l[ 1 ] = RootWindow( ::rux::gui::engine::_x_display_main , ::rux::gui::engine::_x_visual_info->screen );
						::rux::engine::_globals->_x11_module.XSendEvent( ::rux::gui::engine::_x_display_main , RootWindow( ::rux::gui::engine::_x_display_main , ::rux::gui::engine::_x_visual_info->screen ) , False , SubstructureRedirectMask | SubstructureNotifyMask , &x_event );
					}
					else
					{
						XEvent x_event;
						memset( &x_event , 0 , sizeof( x_event ) );
						x_event.xclient.type = ClientMessage;
						x_event.xclient.window = window->_x_window;		
						x_event.xclient.message_type = ::rux::gui::engine::_rux_delete_window;
						x_event.xclient.format = 32;
						x_event.xclient.data.l[ 0 ] = 0;
						x_event.xclient.data.l[ 1 ] = CurrentTime;								
						::rux::engine::_globals->_x11_module.XSendEvent( ::rux::gui::engine::_x_display_main , window->_x_window , false , SubstructureNotifyMask , &x_event );						
					}
					XEvent x_event;
					for( ; ; )
					{
						rux::gui::engine::lock_display( NULL );
						rux::gui::engine::_is_x_if_event = 1;
						::rux::engine::_globals->_x11_module.XIfEvent( ::rux::gui::engine::_x_display_main , &x_event , WaitForDelete , (char*)&window->_x_window );
						rux::gui::engine::_is_x_if_event = 0;
						rux::gui::engine::unlock_display();
						rux::gui::engine::WndProc( x_event );
						if( x_event.type == ClientMessage && x_event.xclient.window == window->_x_window && ( x_event.xclient.message_type == ::rux::gui::engine::_net_close_window || x_event.xclient.message_type == ::rux::gui::engine::_rux_delete_window ) )
							break;
					}
				}
#endif
#endif
			};
			void Window::AddRef( void ) const
			{
				XInterlocked::Increment( const_cast< rux_volatile* >( &_ref ) );
			};
			void Window::Release( void ) const
			{	
				if(	XInterlocked::Decrement( const_cast< rux_volatile* >( &_ref ) ) == 0 )
					::rux::engine::free_object< ::rux::gui::engine::Window >( const_cast< ::rux::gui::engine::Window* >( this ) );
			};
#ifdef __UNIX__
#ifdef __ANDROID__
#else
			void Window::wait_for_map_notify( Window* window )
			{
				XEvent x_event;
				for( ; ; )
				{
					rux::gui::engine::lock_display( NULL );
					rux::gui::engine::_is_x_if_event = 1;
					::rux::engine::_globals->_x11_module.XIfEvent( ::rux::gui::engine::_x_display_main , &x_event , WaitForMapNotify , (char*)&window->_x_window );
					rux::gui::engine::_is_x_if_event = 0;
					rux::gui::engine::unlock_display();
					rux::gui::engine::WndProc( x_event );
					if( x_event.type == MapNotify && x_event.xmap.window == window->_x_window )
						break;
				}
				window->_is_visible = 1;
			};
#endif
#endif
			void Window::dispatch_create_context( void* param )
			{
				::rux::log::WriteDebug( "%s:%d BEGIN" , __FUNCTION__ , __LINE__ );
				Window* window = cpp_ptr_cast(Window* , param );
				if( window->_render_context == NULL )
				{
#ifdef __WINDOWS__			
					rux::uint32 ex_style = GetWindowLong( window->_hwnd , GWL_EXSTYLE );
					if( window->_is_transparent_window == 1 )
					{
						SetWindowLong( window->_hwnd , GWL_EXSTYLE , ex_style | WS_EX_LAYERED );
						SetLayeredWindowAttributes( window->_hwnd , 0x0 , 255 , LWA_ALPHA );
						UpdateWindow( window->_hwnd );
						void* dib_bits = NULL;
						BITMAPINFOHEADER bitmap_info_header;
						rux::int32 size = sizeof( BITMAPINFOHEADER );
						memset( &bitmap_info_header , 0 , size );
						bitmap_info_header.biSize = size;
						bitmap_info_header.biWidth = window->_width;
						bitmap_info_header.biHeight = window->_height;
						bitmap_info_header.biPlanes = 1;
						bitmap_info_header.biBitCount = 24;
						bitmap_info_header.biCompression = BI_RGB;
						window->_hdc = CreateCompatibleDC( NULL );
						window->_transparent_window_bitmap = CreateDIBSection( window->_hdc , (BITMAPINFO*)&bitmap_info_header , DIB_RGB_COLORS , &dib_bits , NULL , 0 );
						SelectObject( window->_hdc , window->_transparent_window_bitmap );			
					}
					else
					{
						window->_hdc = GetDC( window->_hwnd );
						SetWindowLong( window->_hwnd , GWL_EXSTYLE , ex_style );
					}
					window->_render_context = alloc_object_3_params_macros( ::rux::gui::engine::RenderContext , window , window->_width , window->_height );
					CS_PTR_LOCK( ::rux::gui::engine::_cs_windows );
					if( window->_windows_index == 0xffffffff )
						window->_windows_index = ::rux::gui::engine::_windows.FillEmptyOrAdd( window );
					CS_PTR_UNLOCK( ::rux::gui::engine::_cs_windows );
					rux::gui::RenderContextCreatedEvent xevent( window );
					window->raise_event( xevent );
#else
#ifdef __ANDROID__
#else					
					CS_PTR_LOCK( ::rux::gui::engine::_cs_windows );
					if( window->_windows_index == 0xffffffff )
						window->_windows_index = ::rux::gui::engine::_windows.FillEmptyOrAdd( window );
					CS_PTR_UNLOCK( ::rux::gui::engine::_cs_windows );
					wait_for_map_notify( window );
					window->set_Cursor( window->_cursor );
					window->_render_context = alloc_object_3_params_macros( ::rux::gui::engine::RenderContext , window , window->_width , window->_height );
					rux::gui::RenderContextCreatedEvent xevent( window );
					window->raise_event( xevent );
#endif
#endif			
				}
				else
				{
#ifdef __WINDOWS__
#else
#ifdef __ANDROID__
#else
					wait_for_map_notify( window );
					window->set_Cursor( window->_cursor );
#endif
#endif			
				}
				::rux::log::WriteDebug( "%s:%d END" , __FUNCTION__ , __LINE__ );
			};
			void Window::Show( void )
			{
				::rux::log::WriteDebug( "%s:%d BEGIN" , __FUNCTION__ , __LINE__ );
				check( __FILE__ , __LINE__ );
#ifdef __WINDOWS__
				if( _hwnd && _is_visible == 0 )
#else
#ifdef __ANDROID__
#else
				if( _x_display && _is_visible == 0 )
#endif
#endif
				{	
					if( _width < _min_width )
						_width = _min_width;
					if( _height < _min_height )
						_height = _min_height;
					::rux::byte is_allow_resize = _is_allow_resize;
					::rux::byte is_allow_relocate = _is_allow_relocate;
					::rux::byte is_allow_close = _is_allow_close;
					_is_allow_resize = 1;
					_is_allow_relocate = 1;
					_is_allow_close = 1;
					::rux::gui::XEnum_WindowState window_state = _window_state;
#ifdef __WINDOWS__
					SetWindowPos( _hwnd , _is_top_most == 1 ? HWND_TOPMOST : HWND_NOTOPMOST , _left , _top , _width , _height , SWP_NOACTIVATE );					
					CODE_LABELS_INITIALIZE();
					CODE_LABEL_ANOTHER_CONTEXT_2_PARAMETERS( "WinAPi" , "ShowWindow" , ShowWindow , _hwnd , SW_SHOWNOACTIVATE );
#else
#ifdef __ANDROID__
#else
					rux::gui::engine::lock_display( NULL );
					::rux::engine::_globals->_x11_module.XMapRaised( ::rux::gui::engine::_x_display_main , _x_window );
					XWindowChanges window_changes;
					memset( &window_changes , 0 , sizeof( XWindowChanges) );
					window_changes.x = _left;
					window_changes.y = _top;
					window_changes.width = _width;
					window_changes.height = _height;
					::rux::engine::_globals->_x11_module.XConfigureWindow( ::rux::gui::engine::_x_display_main , _x_window , CWX | CWY | CWWidth | CWHeight , &window_changes );
					rux::gui::engine::unlock_display();
#endif
#endif
					rux::gui::engine::dispatch( dispatch_create_context , this );
					if( window_state == ::rux::gui::XEnum_WindowState_Maximized )
						Maximize( 1 );
					else if( window_state == ::rux::gui::XEnum_WindowState_Minimized )
						Minimize( 1 );
#ifndef __WINDOWS__
#ifndef __ANDROID__
					if(_is_top_most)
					{
						if(_x_display)
						{		
							XEvent x_event;
							memset(&x_event, 0, sizeof(x_event));
							x_event.xclient.type = ClientMessage;
							x_event.xclient.send_event = True;
							x_event.xclient.display = ::rux::gui::engine::_x_display_main;
							x_event.xclient.window = _x_window;
							x_event.xclient.message_type = ::rux::gui::engine::_net_wm_state;
							x_event.xclient.format = 32;
							x_event.xclient.data.l[ 0 ] = _is_top_most ? _NET_WM_STATE_ADD : _NET_WM_STATE_REMOVE;
							x_event.xclient.data.l[ 1 ] = ::rux::gui::engine::_net_wm_state_above;
							::rux::gui::engine::lock_display(0);
							::rux::engine::_globals->_x11_module.XSendEvent(::rux::gui::engine::_x_display_main
								, RootWindow(::rux::gui::engine::_x_display_main, ::rux::gui::engine::_x_visual_info->screen), false
								, SubstructureRedirectMask|SubstructureNotifyMask, &x_event);    
							rux::gui::engine::unlock_display();
						}
					}
#endif
#endif
					set_IsAllowResize( is_allow_resize );
					set_IsAllowRelocate( is_allow_relocate );
					set_IsAllowClose( is_allow_close );
				}
				::rux::log::WriteDebug( "%s:%d END" , __FUNCTION__ , __LINE__ );
			};
			void Window::set_Cursor( ::rux::gui::XEnum_Cursors cursor )
			{				
				_cursor = cursor;
				if( cursor == ::rux::gui::XEnum_Cursors_SizeAll )
				{
#ifdef __WINDOWS__
					HCURSOR hcursor = LoadCursor( NULL , IDC_SIZEALL );
					if( hcursor )
						SetCursor( hcursor );
#else
#ifdef __ANDROID__
#else
					if( rux::gui::engine::_is_x_if_event == 0 )
					{
						rux::gui::engine::lock_display( NULL );
						Cursor cursor = ::rux::engine::_globals->_x11_module.XCreateFontCursor( ::rux::gui::engine::_x_display_main , XC_fleur );
						::rux::engine::_globals->_x11_module.XDefineCursor( ::rux::gui::engine::_x_display_main , _x_window , cursor );		
						rux::gui::engine::unlock_display();
					}
#endif
#endif
				}
				else if( cursor == ::rux::gui::XEnum_Cursors_Arrow )
				{
#ifdef __WINDOWS__
					HCURSOR hcursor = LoadCursor( NULL , IDC_ARROW );
					if( hcursor )
						SetCursor( hcursor );
#else
#ifdef __ANDROID__
#else
					if( rux::gui::engine::_is_x_if_event == 0 )
					{
						rux::gui::engine::lock_display( NULL );
						Cursor cursor = ::rux::engine::_globals->_x11_module.XCreateFontCursor( ::rux::gui::engine::_x_display_main , XC_arrow );
						::rux::engine::_globals->_x11_module.XDefineCursor( ::rux::gui::engine::_x_display_main , _x_window , cursor );		
						rux::gui::engine::unlock_display();
					}
#endif
#endif
				}
				else if( cursor == ::rux::gui::XEnum_Cursors_SizeNS )
				{
#ifdef __WINDOWS__
					HCURSOR hcursor = LoadCursor( NULL , IDC_SIZENS );
					if( hcursor )
						SetCursor( hcursor );
#else
#ifdef __ANDROID__
#else
					if( rux::gui::engine::_is_x_if_event == 0 )
					{
						rux::gui::engine::lock_display( NULL );
						Cursor cursor = ::rux::engine::_globals->_x11_module.XCreateFontCursor( ::rux::gui::engine::_x_display_main , XC_sb_v_double_arrow );
						::rux::engine::_globals->_x11_module.XDefineCursor( ::rux::gui::engine::_x_display_main , _x_window , cursor );		
						rux::gui::engine::unlock_display();
					}
#endif
#endif
				}
				else if( cursor == ::rux::gui::XEnum_Cursors_SizeNWSE )
				{
#ifdef __WINDOWS__
					HCURSOR hcursor = LoadCursor( NULL , IDC_SIZENWSE );
					if( hcursor )
						SetCursor( hcursor );
#else
#ifdef __ANDROID__
#else
					if( rux::gui::engine::_is_x_if_event == 0 )
					{
						rux::gui::engine::lock_display( NULL );
						Cursor cursor = ::rux::engine::_globals->_x11_module.XCreateFontCursor( ::rux::gui::engine::_x_display_main , XC_sb_v_double_arrow );
						::rux::engine::_globals->_x11_module.XDefineCursor( ::rux::gui::engine::_x_display_main , _x_window , cursor );		
						rux::gui::engine::unlock_display();
					}
#endif
#endif
				}
				else if( cursor == ::rux::gui::XEnum_Cursors_SizeWE )
				{
#ifdef __WINDOWS__
					HCURSOR hcursor = LoadCursor( NULL , IDC_SIZEWE );
					if( hcursor )
						SetCursor( hcursor );
#else
#ifdef __ANDROID__
#else
					if( rux::gui::engine::_is_x_if_event == 0 )
					{
						rux::gui::engine::lock_display( NULL );
						Cursor cursor = ::rux::engine::_globals->_x11_module.XCreateFontCursor( ::rux::gui::engine::_x_display_main , XC_sb_h_double_arrow );
						::rux::engine::_globals->_x11_module.XDefineCursor( ::rux::gui::engine::_x_display_main , _x_window , cursor );		
						rux::gui::engine::unlock_display();
					}
#endif
#endif
				}
				else if( cursor == ::rux::gui::XEnum_Cursors_Hand )
				{
#ifdef __WINDOWS__
					HCURSOR hcursor = LoadCursor( NULL , IDC_HAND );
					if( hcursor )
						SetCursor( hcursor );
#else
#ifdef __ANDROID__
#else
					if( rux::gui::engine::_is_x_if_event == 0 )
					{
						rux::gui::engine::lock_display( NULL );
						Cursor cursor = ::rux::engine::_globals->_x11_module.XCreateFontCursor( ::rux::gui::engine::_x_display_main , XC_hand1 );
						::rux::engine::_globals->_x11_module.XDefineCursor( ::rux::gui::engine::_x_display_main , _x_window , cursor );		
						rux::gui::engine::unlock_display();
					}
#endif
#endif
				}
			};
			void Window::get_CursorPosition( ::rux::gui::Point& position )
			{
#ifdef __WINDOWS__
				POINT point;
				GetCursorPos( &point );		
				position._x = point.x;
				position._y = point.y;
#else				
#ifdef __ANDROID__
#else
				rux::int32 root_x = 0 , root_y = 0 , win_x = 0 , win_y = 0;
				rux::uint32 button_mask = 0;
				::Window root_window , child_window;
				rux::gui::engine::lock_display( NULL );
				::rux::engine::_globals->_x11_module.XQueryPointer( ::rux::gui::engine::_x_display_main , _x_window , &root_window , &child_window , &root_x , &root_y , &win_x , &win_y , &button_mask );
				rux::gui::engine::unlock_display();
				position._x = root_x;
				position._y = root_y;
#endif
#endif
			};
			void Window::Hide( void )
			{
				check( __FILE__ , __LINE__ );
#ifdef __WINDOWS__
				if( _hwnd && _is_visible == 1 )
				{
					CODE_LABELS_INITIALIZE();
					CODE_LABEL_ANOTHER_CONTEXT_2_PARAMETERS( "WinAPi" , "ShowWindow" , ShowWindow , _hwnd , SW_HIDE );
				}
#else
#ifdef __ANDROID__
#else
				if( _x_display && _is_visible == 1 )
				{
					rux::gui::engine::lock_display( NULL );
					::rux::engine::_globals->_x11_module.XUnmapWindow( ::rux::gui::engine::_x_display_main , _x_window );	
					rux::gui::engine::unlock_display();
				}
#endif
#endif					
			};
			rux::int32 Window::get_Left( void )
			{
				return _left;
			};
			rux::int32 Window::get_Top( void )
			{
				return _top;
			};
			rux::int32 Window::get_Width( void )
			{
				return _width;
			};
			rux::int32 Window::get_Height( void )
			{
				return _height;
			};
			void Window::restore( void )
			{
#ifdef __WINDOWS__
				if( _hwnd )
#else
#ifdef __ANDROID__
				if( 1 )
					printf( "%s\n" , "Not implemented!!!" );
#else
				if( _x_display && _is_visible == 1 )
#endif
#endif
				{
					if( _window_state != ::rux::gui::XEnum_WindowState_Normal )
						Restore();
				}
			};
			void Window::set_IsAllowResize( ::rux::byte is_allow_resize )
			{
				_is_allow_resize = is_allow_resize;
#ifdef __WINDOWS__
				if( _hwnd )
				{
					DWORD style = GetWindowLong( _hwnd , GWL_STYLE ); 
					HMENU menu = GetSystemMenu( _hwnd , FALSE );
					if( _is_allow_resize )
					{
						//style = style | WS_MAXIMIZE | WS_SIZEBOX | WS_MINIMIZE;
						EnableMenuItem( menu , SC_MAXIMIZE , MF_BYCOMMAND | MF_ENABLED );
						EnableMenuItem( menu , SC_SIZE , MF_BYCOMMAND | MF_ENABLED );
						EnableMenuItem( menu , SC_MINIMIZE , MF_BYCOMMAND | MF_ENABLED );
					}
					else
					{
						style &= ~( WS_MAXIMIZE | WS_SIZEBOX | WS_MINIMIZE );
						EnableMenuItem( menu , SC_MAXIMIZE , MF_BYCOMMAND | MF_DISABLED );
						EnableMenuItem( menu , SC_SIZE , MF_BYCOMMAND | MF_DISABLED );
						EnableMenuItem( menu , SC_MINIMIZE , MF_BYCOMMAND | MF_DISABLED );
					}
					SetWindowLong( _hwnd , GWL_STYLE , style );
				}
#endif
			};
			void Window::set_IsAllowClose( ::rux::byte is_allow_close )
			{
				_is_allow_close = is_allow_close;
#ifdef __WINDOWS__
				if( _hwnd )
				{
					HMENU menu = GetSystemMenu( _hwnd , FALSE );
					if( _is_allow_close )
						EnableMenuItem( menu , SC_CLOSE , MF_BYCOMMAND | MF_ENABLED );
					else
						EnableMenuItem( menu , SC_CLOSE , MF_BYCOMMAND | MF_DISABLED );
				}
#endif
			};
			void Window::set_IsAllowRelocate( ::rux::byte is_allow_relocate )
			{
				_is_allow_relocate = is_allow_relocate;
#ifdef __WINDOWS__
				if( _hwnd )
				{
					HMENU menu = GetSystemMenu( _hwnd , FALSE );
					if( _is_allow_relocate )
						EnableMenuItem( menu , SC_MOVE , MF_BYCOMMAND | MF_ENABLED );
					else
						EnableMenuItem( menu , SC_MOVE , MF_BYCOMMAND | MF_DISABLED );
				}
#endif
			};
			void Window::set_LocationAndSize( ::rux::int32 x , ::rux::int32 y , ::rux::int32 width , ::rux::int32 height )
			{
				::rux::log::WriteTrace( "set_LocationAndSize, %p, %d %d %d %d" , this , x , y , width , height );
				if( _window_state == ::rux::gui::XEnum_WindowState_Normal )
				{
					if( width < _min_width || width <= 0 )
						width = _width;
					if( height < _min_height || height <= 0 )
						height = _height;
					if( _width != width || _height != height || _left != x || _top != y )
					{			
						restore();
#ifdef __WINDOWS__
						if( _hwnd )
						{
							UINT flags = SWP_NOACTIVATE;
							if( width == _width && height == _height )
								flags |= SWP_NOSIZE;
							if( _left == x && _top == y )
								flags |= SWP_NOMOVE;
							SetWindowPos( _hwnd , _is_top_most == 1 ? HWND_TOPMOST : HWND_NOTOPMOST , x , y , width , height , flags );
						}
#else
#ifdef __ANDROID__
						if( 1 )
							printf( "%s\n" , "Not implemented!!!" );
#else
						if( _x_display && _is_visible == 1 )
						{
							rux::gui::engine::lock_display( NULL );
							XWindowChanges window_changes;
							memset( &window_changes , 0 , sizeof( XWindowChanges) );
							unsigned value_mask = 0;
							if( width != _width )
							{
								window_changes.width = width;
								value_mask |= CWWidth;
							}
							if( height != _height )
							{
								window_changes.height = height;
								value_mask |= CWHeight;
							}
							if( _left != x )
							{
								window_changes.x = x;
								value_mask |= CWX;
							}
							if( _top != y )
							{
								window_changes.y = y;
								value_mask |= CWY;
							}
							::rux::engine::_globals->_x11_module.XConfigureWindow( ::rux::gui::engine::_x_display_main , _x_window ,  value_mask , &window_changes );
							::rux::engine::_globals->_x11_module.XFlush( ::rux::gui::engine::_x_display_main );
							rux::gui::engine::unlock_display();							
						}
#endif
#endif
						else
						{
							_left = x;
							_top = y;
							_width = width;
							_height = height;
						}
					}
				}
			};
			void Window::set_Width( ::rux::int32 width )
			{
				::rux::log::WriteTrace( "set_Width, %p, %d" , this , width );
				if( _window_state == ::rux::gui::XEnum_WindowState_Normal )
				{
					if( width >= _min_width )
					{
						if( _width != width && width > 0 )
						{			
							restore();
	#ifdef __WINDOWS__
							if( _hwnd )
								SetWindowPos( _hwnd , _is_top_most == 1 ? HWND_TOPMOST : HWND_NOTOPMOST , _left , _top , width , _height , SWP_NOMOVE|SWP_NOACTIVATE );
	#else
	#ifdef __ANDROID__
							if( 1 )
								printf( "%s\n" , "Not implemented!!!" );
	#else
							if( _x_display && _is_visible == 1 )
							{
								rux::gui::engine::lock_display( NULL );
								XWindowChanges window_changes;
								memset( &window_changes , 0 , sizeof( XWindowChanges) );
								window_changes.width = width;
								::rux::engine::_globals->_x11_module.XConfigureWindow( ::rux::gui::engine::_x_display_main , _x_window , CWWidth , &window_changes );
								::rux::engine::_globals->_x11_module.XFlush( ::rux::gui::engine::_x_display_main );
								rux::gui::engine::unlock_display();							
							}
	#endif
	#endif
							else
								_width = width;
						}
					}
				}
			};
			void Window::set_Height( ::rux::int32 height )
			{
				::rux::log::WriteTrace( "set_Height, %p, %d" , this , height );
				if( _window_state == ::rux::gui::XEnum_WindowState_Normal )
				{
					if( height >= _min_height )
					{
						if( _height != height && height > 0 )
						{
	#ifdef __WINDOWS__
							if( _hwnd )
								SetWindowPos( _hwnd , _is_top_most == 1 ? HWND_TOPMOST : HWND_NOTOPMOST , _left , _top , _width , height , SWP_NOMOVE|SWP_NOACTIVATE );
	#else
	#ifdef __ANDROID__
							if( 1 )
								printf( "%s\n" , "Not implemented!!!" );
	#else
							if( _x_display && _is_visible == 1 )
							{
								rux::gui::engine::lock_display( NULL );
								XWindowChanges window_changes;
								memset( &window_changes , 0 , sizeof( XWindowChanges) );
								window_changes.height = height;
								::rux::engine::_globals->_x11_module.XConfigureWindow( ::rux::gui::engine::_x_display_main , _x_window , CWHeight , &window_changes );
								::rux::engine::_globals->_x11_module.XFlush( ::rux::gui::engine::_x_display_main );
								rux::gui::engine::unlock_display();
							}
	#endif
	#endif
							else
								_height = height;		
						}
					}
				}
			};
			void Window::set_Left( ::rux::int32 left )
			{
				::rux::log::WriteTrace( "set_Left, %p, %d" , this , left );
				if( _left != left )
				{		
					restore();
#ifdef __WINDOWS__
					if( _hwnd )
						SetWindowPos( _hwnd , _is_top_most == 1 ? HWND_TOPMOST : HWND_NOTOPMOST , left , _top , _width , _height , SWP_NOSIZE|SWP_NOACTIVATE );
#else
#ifdef __ANDROID__
					if( 1 )
						printf( "%s\n" , "Not implemented!!!" );
#else
					if( _x_display && _is_visible == 1 )
					{
						rux::gui::engine::lock_display( NULL );
						XWindowChanges window_changes;
						memset( &window_changes , 0 , sizeof( XWindowChanges) );
						window_changes.x = left;
						::rux::engine::_globals->_x11_module.XConfigureWindow( ::rux::gui::engine::_x_display_main , _x_window , CWX , &window_changes );
						::rux::engine::_globals->_x11_module.XFlush( ::rux::gui::engine::_x_display_main );
						rux::gui::engine::unlock_display();
					}	
#endif
#endif
					else
						_left = left;
				}
			};
			void Window::set_Top( ::rux::int32 top )
			{
				::rux::log::WriteTrace( "set_Top, %p, %d" , this , top );
				if( _top != top )
				{
					restore();
#ifdef __WINDOWS__
					if( _hwnd )
						SetWindowPos( _hwnd , _is_top_most == 1 ? HWND_TOPMOST : HWND_NOTOPMOST , _left , top , _width , _height , SWP_NOSIZE|SWP_NOACTIVATE );
#else
#ifdef __ANDROID__
					if( 1 )
						printf( "%s\n" , "Not implemented!!!" );
#else
					if( _x_display && _is_visible == 1 )
					{
						rux::gui::engine::lock_display( NULL );
						XWindowChanges window_changes;
						memset( &window_changes , 0 , sizeof( XWindowChanges) );
						window_changes.y = top;
						::rux::engine::_globals->_x11_module.XConfigureWindow( ::rux::gui::engine::_x_display_main , _x_window , CWY , &window_changes );
						::rux::engine::_globals->_x11_module.XFlush( ::rux::gui::engine::_x_display_main );
						rux::gui::engine::unlock_display();
					}	
#endif
#endif
					else
						_top = top;
				}
			};
			rux::uint8 Window::get_TopMost( void )
			{
				return _is_top_most;
			};
			rux::uint8 Window::get_IsVisible( void )
			{
				return _is_visible;
			};
			void Window::set_TopMost( ::rux::uint8 top_most )
			{
				if( _is_top_most != top_most )
				{
					_is_top_most = top_most;
#ifdef __WINDOWS__
					if( _hwnd )
					{
						if( top_most == 1 )
							SetWindowPos( _hwnd , HWND_TOPMOST , 0 , 0 , 0 , 0 , SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE );
						else
							SetWindowPos( _hwnd , HWND_NOTOPMOST , 0 , 0 , 0 , 0 , SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE );
					}
#else
#ifdef __ANDROID__
#else
					if(_x_display)
					{		
						XEvent x_event;
						memset(&x_event, 0, sizeof(x_event));
						x_event.xclient.type = ClientMessage;
						x_event.xclient.send_event = True;
						x_event.xclient.display = ::rux::gui::engine::_x_display_main;
						x_event.xclient.window = _x_window;
						x_event.xclient.message_type = ::rux::gui::engine::_net_wm_state;
						x_event.xclient.format = 32;
						x_event.xclient.data.l[ 0 ] = _is_top_most ? _NET_WM_STATE_ADD : _NET_WM_STATE_REMOVE;
						x_event.xclient.data.l[ 1 ] = ::rux::gui::engine::_net_wm_state_above;
						::rux::gui::engine::lock_display(0);
						::rux::engine::_globals->_x11_module.XSendEvent(::rux::gui::engine::_x_display_main
							, RootWindow(::rux::gui::engine::_x_display_main, ::rux::gui::engine::_x_visual_info->screen), false
							, SubstructureRedirectMask|SubstructureNotifyMask, &x_event);    
						rux::gui::engine::unlock_display();
					}
#endif
#endif
				}
			};
			void Window::Close( void )
			{
#ifdef __WINDOWS__
				if( _hwnd )
#else
#ifdef __ANDROID__
#else
				if( _x_display )
#endif
#endif
					rux::gui::engine::dispatch( dispatch_close_window , this );
			};
			void Window::Minimize( void )
			{
				Minimize( 0 );
			};
			void Window::Maximize( void )
			{
				Maximize( 0 );
			};
			void Window::Restore( void )
			{
				Restore( 0 );
			};
			void Window::Minimize( ::rux::byte force )
			{
#ifdef __WINDOWS__
				if( _hwnd && ( force || _is_allow_resize == 1 ) )
				{
					CODE_LABELS_INITIALIZE();
					CODE_LABEL_ANOTHER_CONTEXT_2_PARAMETERS( "WinAPi" , "ShowWindow" , ShowWindow , _hwnd , SW_MINIMIZE );
					while( PostMessage( _hwnd , WM_SYSCOMMAND , SC_MINIMIZE , 0 ) == 0 )
						::rux::threading::XThread::Sleep( 1 );
				}
#else
#ifdef __ANDROID__
#else
				if( _x_display && ( force || _is_allow_resize == 1 ) )
				{
					if( force || _window_state == ::rux::gui::XEnum_WindowState_Maximized
						|| _window_state == ::rux::gui::XEnum_WindowState_Normal )
					{
						XEvent x_event;
						memset( &x_event , 0 , sizeof( x_event ) );					
						x_event.xclient.type = ClientMessage;
						x_event.xclient.window = _x_window;
						x_event.xclient.message_type = ::rux::gui::engine::_wm_change_state;
						x_event.xclient.format = 32;
						x_event.xclient.data.l[ 0 ] = IconicState;						
						rux::gui::engine::lock_display( NULL );
						::rux::engine::_globals->_x11_module.XSendEvent( ::rux::gui::engine::_x_display_main , RootWindow( ::rux::gui::engine::_x_display_main , ::rux::gui::engine::_x_visual_info->screen ) , false , SubstructureRedirectMask | SubstructureNotifyMask , &x_event );    
						rux::gui::engine::unlock_display();
					}
				}
#endif
#endif
				else
					_window_state = ::rux::gui::XEnum_WindowState_Minimized;
			};
			void Window::CaptureMouse( void )
			{
#ifdef __WINDOWS__
				if( _hwnd )
					SetCapture( _hwnd );
#else
#ifdef __ANDROID__
#else
				if( _x_display )
				{
					if( rux::gui::engine::_is_x_if_event == 0 )
					{
						rux::gui::engine::lock_display( NULL );
						::rux::engine::_globals->_x11_module.XGrabPointer( ::rux::gui::engine::_x_display_main , _x_window , true , ButtonPressMask | ButtonReleaseMask | PointerMotionMask | FocusChangeMask | EnterWindowMask | LeaveWindowMask , GrabModeAsync , GrabModeAsync , None , None , CurrentTime );
						rux::gui::engine::unlock_display();
					}
				}
#endif
#endif
			};
			void Window::ReleaseMouse( void )
			{
#ifdef __WINDOWS__
				if( _hwnd && GetCapture() == _hwnd )
					ReleaseCapture();
#else
#ifdef __ANDROID__
#else
				if( _x_display )
				{
					if( rux::gui::engine::_is_x_if_event == 0 )
					{
						rux::gui::engine::lock_display( NULL );
						::rux::engine::_globals->_x11_module.XUngrabPointer( ::rux::gui::engine::_x_display_main , CurrentTime );
						rux::gui::engine::unlock_display();
					}
				}				
#endif
#endif
			};			
			void Window::Maximize( ::rux::byte force )
			{
#ifdef __WINDOWS__
				if( _hwnd && ( force || _is_allow_resize == 1 ) )
				{
					CODE_LABELS_INITIALIZE();
					CODE_LABEL_ANOTHER_CONTEXT_2_PARAMETERS( "WinAPi" , "ShowWindow" , ShowWindow , _hwnd , SW_MAXIMIZE );
					while( PostMessage( _hwnd , WM_SYSCOMMAND , SC_MAXIMIZE , 0 ) == 0 )
						::rux::threading::XThread::Sleep( 1 );
				}
#else	
#ifdef __ANDROID__
#else
				if( _x_display && ( force || _is_allow_resize == 1 ) )
				{
					if( force || _window_state == ::rux::gui::XEnum_WindowState_Normal
						|| _window_state == ::rux::gui::XEnum_WindowState_Minimized )
					{
						XEvent x_event;
						memset( &x_event , 0 , sizeof( x_event ) );
						x_event.xclient.type = ClientMessage;
						x_event.xclient.window = _x_window;
						x_event.xclient.message_type = ::rux::gui::engine::_net_wm_state;
						x_event.xclient.format = 32;
						x_event.xclient.data.l[ 0 ] = _NET_WM_STATE_ADD;
						x_event.xclient.data.l[ 1 ] = ::rux::gui::engine::_net_wm_state_max_horz;
						x_event.xclient.data.l[ 2 ] = ::rux::gui::engine::_net_wm_state_max_vert;
						rux::gui::engine::lock_display( NULL );
						::rux::engine::_globals->_x11_module.XSendEvent( ::rux::gui::engine::_x_display_main , RootWindow( ::rux::gui::engine::_x_display_main , ::rux::gui::engine::_x_visual_info->screen ) , false , SubstructureNotifyMask , &x_event );    
						rux::gui::engine::unlock_display();
					}
				}
#endif
#endif	
				else
					_window_state = ::rux::gui::XEnum_WindowState_Maximized;
			};
			void Window::Restore( ::rux::byte force )
			{
#ifdef __WINDOWS__
				if( _hwnd && ( force || _is_allow_resize == 1 ) )
				{
					CODE_LABELS_INITIALIZE();
					CODE_LABEL_ANOTHER_CONTEXT_2_PARAMETERS( "WinAPi" , "ShowWindow" , ShowWindow , _hwnd , SW_RESTORE );
					while( PostMessage( _hwnd , WM_SYSCOMMAND , SC_RESTORE , 0 ) == 0 )
						::rux::threading::XThread::Sleep( 1 );
				}
#else	
#ifdef __ANDROID__
#else
				if( _x_display && ( force || _is_allow_resize == 1 ) )
				{
					if( _window_state == ::rux::gui::XEnum_WindowState_Maximized )
					{
						XEvent x_event;
						memset( &x_event , 0 , sizeof( x_event ) );
						x_event.xclient.type = ClientMessage;
						x_event.xclient.window = _x_window;
						x_event.xclient.message_type = ::rux::gui::engine::_net_wm_state;
						x_event.xclient.format = 32;
						x_event.xclient.data.l[ 0 ] = _NET_WM_STATE_REMOVE;
						x_event.xclient.data.l[ 1 ] = ::rux::gui::engine::_net_wm_state_max_horz;
						x_event.xclient.data.l[ 2 ] = ::rux::gui::engine::_net_wm_state_max_vert;
						rux::gui::engine::lock_display( NULL );
						::rux::engine::_globals->_x11_module.XSendEvent( ::rux::gui::engine::_x_display_main , RootWindow( ::rux::gui::engine::_x_display_main , ::rux::gui::engine::_x_visual_info->screen ) , false , SubstructureNotifyMask , &x_event );
						rux::gui::engine::unlock_display();
					}
					else if( _window_state == ::rux::gui::XEnum_WindowState_Minimized )
					{						
						rux::gui::engine::lock_display( NULL );
						::rux::engine::_globals->_x11_module.XMapRaised( ::rux::gui::engine::_x_display_main , _x_window );
						rux::gui::engine::unlock_display();
					}
				}
#endif
#endif					
				else
					_window_state = ::rux::gui::XEnum_WindowState_Normal;
			};
		};
	};
};
