#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_gui_engine.h>
#include <xan_gui_window.h>
#include <xan_thread.h>
#include <xan_log.h>
#include <xan_hash.h>
#include <xan_time.h>
#ifdef __UNIX__
#include <X11/extensions/Xrandr.h>
#endif
namespace rux
{
	namespace gui
	{
		::rux::byte Keyboard::XlibToWinVirtualKey( ::rux::uint16 xlib_key_sym )
		{
			::rux::byte res = 0;
			switch( xlib_key_sym )
			{
			case XK_BackSpace:
				{
					res = VK_BACK;
					break;
				}
			case XK_Tab:
				{
					res = VK_TAB;
					break;
				}
			case XK_Linefeed:
				{
					break;
				}
			case XK_Clear:
				{
					res = VK_CLEAR;
					break;
				}
			case XK_Return:
				{
					res = VK_RETURN;
					break;
				}
			case XK_Pause:
				{
					res = VK_PAUSE;
					break;
				}
			case XK_Scroll_Lock:
				{
					res = VK_SCROLL;
					break;
				}
			case XK_Sys_Req:
				{
					break;
				}
			case XK_Escape:
				{
					res = VK_ESCAPE;
					break;
				}
			case XK_Delete:
				{
					res = VK_DELETE;
					break;
				}
			case XK_Multi_key:
				{
					break;
				}
			case XK_Kanji:
				{
					break;
				}
			case XK_Muhenkan:
				{
					break;
				}
			case XK_Henkan_Mode:
				{
					break;
				}
			case XK_Romaji:
				{
					break;
				}
			case XK_Hiragana:
				{
					break;
				}
			case XK_Katakana:
				{
					break;
				}
			case XK_Hiragana_Katakana:
				{
					break;
				}
			case XK_Zenkaku:
				{
					break;
				}
			case XK_Hankaku:
				{
					break;
				}
			case XK_Zenkaku_Hankaku:
				{
					break;
				}
			case XK_Touroku:
				{
					break;
				}
			case XK_Massyo:
				{
					break;
				}
			case XK_Kana_Lock:
				{
					break;
				}
			case XK_Kana_Shift:
				{
					break;
				}
			case XK_Eisu_Shift:
				{
					break;
				}
			case XK_Eisu_toggle:
				{
					break;
				}
			case XK_Home:
				{
					res = VK_HOME;
					break;
				}
			case XK_Left:
				{
					res = VK_LEFT;
					break;
				}
			case XK_Up:
				{
					res = VK_UP;
					break;
				}
			case XK_Right:
				{
					res = VK_RIGHT;
					break;
				}
			case XK_Down:
				{
					res = VK_DOWN;
					break;
				}
			case XK_Page_Up:
				{
					res = VK_PRIOR;
					break;
				}
			case XK_Page_Down:
				{
					res = VK_NEXT;
					break;
				}
			case XK_End:
				{
					res = VK_END;
					break;
				}
			case XK_Begin:
				{
					res = VK_HOME;
					break;
				}
			case XK_Select:
				{
					res = VK_SELECT;
					break;
				}
			case XK_Print:
				{
					res = VK_PRINT;
					break;
				}
			case XK_Execute:
				{
					res = VK_EXECUTE;
					break;
				}
			case XK_Insert:
				{
					res = VK_INSERT;
					break;
				}
			case XK_Undo:
				{
					break;
				}
			case XK_Redo:
				{
					break;
				}
			case XK_Menu:
				{
					res = VK_MENU;
					break;
				}
			case XK_Find:
				{
					break;
				}
			case XK_Cancel:
				{
					res = VK_CANCEL;
					break;
				}
			case XK_Help:
				{
					res = VK_HELP;
					break;
				}
			case XK_Break:
				{
					break;
				}
			case XK_Mode_switch:
				{
					break;
				}
			case XK_Num_Lock:
				{
					res = VK_NUMLOCK;
					break;
				}
			case XK_KP_Space:
				{
					res = VK_SPACE;
					break;
				}
			case XK_KP_Tab:
				{
					res = VK_TAB;
					break;
				}
			case XK_KP_Enter:
				{
					res = VK_RETURN;
					break;
				}
			case XK_KP_F1:
				{
					res = VK_F1;
					break;
				}
			case XK_KP_F2:
				{
					res = VK_F2;
					break;
				}
			case XK_KP_F3:
				{
					res = VK_F3;
					break;
				}
			case XK_KP_F4:
				{
					res = VK_F4;
					break;
				}
			case XK_KP_Home:
				{
					res = VK_HOME;
					break;
				}
			case XK_KP_Left:
				{
					res = VK_LEFT;
					break;
				}
			case XK_KP_Up:
				{
					res = VK_UP;
					break;
				}
			case XK_KP_Right:
				{
					res = VK_RIGHT;
					break;
				}
			case XK_KP_Down:
				{
					res = VK_DOWN;
					break;
				}
			case XK_KP_Page_Up:
				{
					res = VK_PRIOR;
					break;
				}
			case XK_KP_Page_Down:
				{
					res = VK_NEXT;
					break;
				}
			case XK_KP_End:
				{
					res = VK_END;
					break;
				}
			case XK_KP_Begin:
				{
					res = VK_HOME;
					break;
				}
			case XK_KP_Insert:
				{
					res = VK_INSERT;
					break;
				}
			case XK_KP_Delete:
				{
					res = VK_DELETE;
					break;
				}
			case XK_KP_Equal:
				{
					break;
				}
			case XK_KP_Multiply:
				{
					res = VK_MULTIPLY;
					break;
				}
			case XK_KP_Add:
				{
					res = VK_ADD;
					break;
				}
			case XK_KP_Separator:
				{
					res = VK_SEPARATOR;
					break;
				}
			case XK_KP_Subtract:
				{
					res = VK_SUBTRACT;
					break;
				}
			case XK_KP_Decimal:
				{
					res = VK_DECIMAL;
					break;
				}
			case XK_KP_Divide:
				{
					res = VK_DIVIDE;
					break;
				}
			case XK_KP_0:
				{
					res = VK_NUMPAD0;
					break;
				}
			case XK_KP_1:
				{
					res = VK_NUMPAD1;
					break;
				}
			case XK_KP_2:
				{
					res = VK_NUMPAD2;
					break;
				}
			case XK_KP_3:
				{
					res = VK_NUMPAD3;
					break;
				}
			case XK_KP_4:
				{
					res = VK_NUMPAD4;
					break;
				}
			case XK_KP_5:
				{
					res = VK_NUMPAD5;
					break;
				}
			case XK_KP_6:
				{
					res = VK_NUMPAD6;
					break;
				}
			case XK_KP_7:
				{
					res = VK_NUMPAD7;
					break;
				}
			case XK_KP_8:
				{
					res = VK_NUMPAD8;
					break;
				}
			case XK_KP_9:
				{
					res = VK_NUMPAD9;
					break;
				}
			case XK_F1:
				{
					res = VK_F1;
					break;
				}
			case XK_F2:
				{
					res = VK_F2;
					break;
				}
			case XK_F3:
				{
					res = VK_F3;
					break;
				}
			case XK_F4:
				{
					res = VK_F4;
					break;
				}
			case XK_F5:
				{
					res = VK_F5;
					break;
				}
			case XK_F6:
				{
					res = VK_F6;
					break;
				}
			case XK_F7:
				{
					res = VK_F7;
					break;
				}
			case XK_F8:
				{
					res = VK_F8;
					break;
				}
			case XK_F9:
				{
					res = VK_F9;
					break;
				}
			case XK_F10:
				{
					res = VK_F10;
					break;
				}
			case XK_F11:
				{
					res = VK_F11;
					break;
				}
			case XK_F12:
				{
					res = VK_F12;
					break;
				}
			case XK_F13:
				{
					res = VK_F13;
					break;
				}
			case XK_F14:
				{
					res = VK_F14;
					break;
				}
			case XK_F15:
				{
					res = VK_F15;
					break;
				}
			case XK_F16:
				{
					res = VK_F16;
					break;
				}
			case XK_F17:
				{
					res = VK_F17;
					break;
				}
			case XK_F18:
				{
					res = VK_F18;
					break;
				}
			case XK_F19:
				{
					res = VK_F19;
					break;
				}
			case XK_F20:
				{
					res = VK_F20;
					break;
				}
			case XK_F21:
				{
					res = VK_F21;
					break;
				}
			case XK_F22:
				{
					res = VK_F22;
					break;
				}
			case XK_F23:
				{
					res = VK_F23;
					break;
				}
			case XK_F24:
				{
					res = VK_F24;
					break;
				}
			case XK_F25:
				{
					break;
				}
			case XK_F26:
				{
					break;
				}
			case XK_F27:
				{
					break;
				}
			case XK_F28:
				{
					break;
				}
			case XK_F29:
				{
					break;
				}
			case XK_F30:
				{
					break;
				}
			case XK_F31:
				{
					break;
				}
			case XK_F32:
				{
					break;
				}
			case XK_F33:
				{
					break;
				}
			case XK_F34:
				{
					break;
				}
			case XK_F35:
				{
					break;
				}
			case XK_Shift_L:
				{
					res = VK_LSHIFT;
					break;
				}
			case XK_Shift_R:
				{
					res = VK_RSHIFT;
					break;
				}
			case XK_Control_L:
				{
					res = VK_LCONTROL;
					break;
				}
			case XK_Control_R:
				{
					res = VK_RCONTROL;
					break;
				}
			case XK_Caps_Lock:
				{
					break;
				}
			case XK_Shift_Lock:
				{
					break;
				}
			case XK_Meta_L:
				{
					break;
				}
			case XK_Meta_R:
				{
					break;
				}
			case XK_Alt_L:
				{
					res = VK_LMENU;
					break;
				}
			case XK_Alt_R:
				{
					res = VK_RMENU;
					break;
				}
			case XK_Super_L:
				{
					break;
				}
			case XK_Super_R:
				{
					break;
				}
			case XK_Hyper_L:
				{
					break;
				}
			case XK_Hyper_R:
				{
					break;
				}
			case XK_Win_L:
				{
					res = VK_LWIN;
					break;
				}
			case XK_Win_R:
				{
					res = VK_RWIN;
					break;
				}
			case XK_App:
				{
					res = VK_APPS;
					break;
				}
			}
			return res;
		};
		namespace engine
		{			
			dll_internal ft_structs_h::FT_Library _rux_ft_library = NULL;			
			dll_internal XMallocArray< freetype_font* , 5 > _rux_fonts;			
			dll_internal XMallocArray< ::rux::gui::engine::RenderFont* , 5 > _font_bases;			
			dll_internal ::rux::uint8 _quit = 0;
			dll_internal XMallocArray< ::rux::gui::on_event_t > _on_events;
			dll_internal XMallocArray< void* > _on_event_owners;
			dll_internal rux_volatile _is_initialized = 0;
			dll_internal rux_volatile _dispatchers_count = 0;
			dll_internal ::rux::pid_t _message_processing_thread_id = 0;
			dll_internal XMallocArray< XUserData3* > _dispatchers;
			dll_internal size_t _is_pump_message = 0;
			dll_internal ::rux::uint32 _display_frequencies[ 32 ] = {0};
			dll_internal ::rux::uint32 _display_bits[ 32 ] = {0};
			dll_internal size_t _displays_count = 0;
#ifdef __WINDOWS__
#elif defined( __UNIX__ )
#ifdef __ANDROID__
#else
			dll_internal Display* _x_display_main = NULL;
			dll_internal ::rux::uint8 _is_x_if_event = 0;
			dll_internal ::rux::uint8 _is_x_key_pressed = 0;			
			dll_internal XVisualInfo* _x_visual_info = NULL;
			dll_internal XSetWindowAttributes _x_set_window_attributes;
			dll_internal XVisualInfo* _x_infos = NULL;
			dll_internal Atom _wm_delete_window = None;
			dll_internal Atom _net_wm_state = None;
			dll_internal Atom _net_wm_allowed_actions = None;
			dll_internal Atom _net_close_window = None;
			dll_internal Atom _wm_change_state = None;
			dll_internal Atom _rux_delete_window = None;
			dll_internal Atom _rux_gui_initialize = None;
			dll_internal Atom _net_wm_state_max_horz = None;
			dll_internal Atom _net_wm_state_hidden = None;
			dll_internal Atom _net_wm_state_max_vert = None;
			dll_internal Atom _net_wm_state_fullscreen = None;
			dll_internal Atom _wm_protocols = None;
			dll_internal ::rux::uint8 _is_mouse_down = 0;
			dll_internal ::rux::byte _is_right_mouse_down = 0;
			dll_internal ::rux::byte _is_middle_mouse_down = 0;
			dll_internal ::rux::uint64 _mouse_down_time = 0ULL;
			dll_internal ::rux::uint64 _right_mouse_down_time = 0ULL;
			dll_internal ::rux::uint64 _middle_mouse_down_time = 0ULL;
#endif
#endif
			dll_internal void initialize( void )
			{
				if( ::rux::engine::_globals->_gui_globals->_cs_gui_schedule == 0 )
					::rux::engine::_globals->_gui_globals->_cs_gui_schedule = alloc_object_macros( ::rux::threading::RdWrLock );
				if( ::rux::engine::_globals->_gui_globals->_gui_schedule == 0 )
					::rux::engine::_globals->_gui_globals->_gui_schedule = alloc_object_macros( XMallocArray< ::rux::gui::engine::schedule* > );
#ifdef __WINDOWS__
				DISPLAY_DEVICEA display_device;
				memset( &display_device , 0 , sizeof( DISPLAY_DEVICEA ) );
				display_device.cb = sizeof( display_device );
				char device_name[ 32 ] = { 0 } , device_string[ 128 ] = { 0 };
				::rux::int32 device_index = 0;
				while( EnumDisplayDevicesA( NULL , device_index , &display_device , 0 ) )
				{
					if( ( display_device.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP ) )
					{
						::rux::safe_strncpy( device_name , display_device.DeviceName , 32 );
						::rux::safe_strncpy( device_string, display_device.DeviceString , 128 );
						::rux::int32 monitor_index = 0;
						if( EnumDisplayDevicesA( device_name , monitor_index , &display_device , 0 ) )
						{
							DEVMODEA dev_mode;
							memset( &dev_mode , 0 , sizeof( DEVMODEA ) );
							dev_mode.dmSize = sizeof( DEVMODEA );
							dev_mode.dmDriverExtra = 0;
							if( EnumDisplaySettingsExA( device_name , ENUM_CURRENT_SETTINGS , &dev_mode , EDS_RAWMODE ) )
							{
								::rux::gui::engine::_display_bits[ ::rux::gui::engine::_displays_count ] = dev_mode.dmBitsPerPel;
								::rux::gui::engine::_display_frequencies[ ::rux::gui::engine::_displays_count ] = dev_mode.dmDisplayFrequency;
								::rux::gui::engine::_displays_count++;
							}
							monitor_index++;
						}
					}
					device_index++;
				}
				WNDCLASSA wnd_class;
				if( GetClassInfoA( GetModuleHandle( NULL ) , "rux_ui_window" , &wnd_class ) == false )
				{
					wnd_class.style = CS_DBLCLKS  | CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
					wnd_class.lpfnWndProc = (WNDPROC)rux::gui::engine::WndProc;
					wnd_class.cbClsExtra = 0;
					wnd_class.cbWndExtra = 0;
					wnd_class.hInstance = GetModuleHandle( NULL );
					wnd_class.hIcon = LoadIcon( NULL , IDI_WINLOGO );
					wnd_class.hCursor = LoadCursor( NULL , IDC_ARROW );
					wnd_class.hbrBackground = NULL;
					wnd_class.lpszMenuName = NULL;
					wnd_class.lpszClassName = "rux_ui_window";
					RegisterClassA( &wnd_class );
				}
#elif defined( __UNIX__ )
#ifdef __ANDROID__
#else
				Display* display = ::rux::engine::_globals->_x11_module.XOpenDisplay( NULL );
				if( display )
				{
					size_t count = ScreenCount( display );
					::Window window;
					for( size_t index0 = 0 ; index0 < count ; index0++ )
					{
#if 0
						window = RootWindow( display , index0 );
						//int num_sizes = 0;						
						//XRRScreenSize* xrrs = ::rux::engine::_globals->_xrandr_module.XRRSizes( display , 0 , &num_sizes );

						XRRScreenConfiguration* conf = ::rux::engine::_globals->_xrandr_module.XRRGetScreenInfo( display , window );
						if( conf )
						{
							short current_rate = ::rux::engine::_globals->_xrandr_module.XRRConfigCurrentRate( conf );
						
							Rotation current_rotation;
							SizeID current_size_id = ::rux::engine::_globals->_xrandr_module.XRRConfigCurrentConfiguration( conf , &current_rotation );

							/*int current_width = xrrs[ current_size_id ].width;
							int current_height = xrrs[ current_size_id ].height;
							std::cout << "current_rate = " << current_rate << std::endl;
							std::cout << "current_width = " << current_width << std::endl;
							std::cout << "current_height = " << current_height << std::endl;*/
							::rux::gui::engine::_display_frequencies[ ::rux::gui::engine::_displays_count ] = current_rate;
							::rux::engine::_globals->_xrandr_module.XRRFreeScreenConfigInfo( conf );
						}
						else
#endif
							::rux::gui::engine::_display_frequencies[ ::rux::gui::engine::_displays_count ] = 0;
						::rux::gui::engine::_display_bits[ ::rux::gui::engine::_displays_count ] = 32;
						::rux::gui::engine::_displays_count++;
					}
					::rux::engine::_globals->_x11_module.XCloseDisplay( display );
				}
				::rux::engine::_globals->_x11_module.XInitThreads();
				rux::gui::engine::_x_display_main = ::rux::engine::_globals->_x11_module.XOpenDisplay( NULL );
				if( ::rux::gui::engine::_x_display_main )
				{
					::rux::engine::_globals->_x11_module.XSetErrorHandler( display_error_handler );
					::rux::engine::_globals->_x11_module.XSetIOErrorHandler( display_io_error_handler );
					rux::gui::engine::OpenGL::initialize();
					if( !rux::gui::engine::OpenGL::glXQueryExtension( ::rux::gui::engine::_x_display_main , NULL , NULL ) ) 
					{
						::rux::engine::_globals->_x11_module.XCloseDisplay( ::rux::gui::engine::_x_display_main );
						rux::gui::engine::_x_display_main = NULL;
					}
					else
					{
						rux::int32 visual_info_count = 0 , x_visual_info_index = -1;			
						rux::gui::engine::_x_infos = ::rux::engine::_globals->_x11_module.XGetVisualInfo( ::rux::gui::engine::_x_display_main , 0 , NULL , &visual_info_count );
						if( ::rux::gui::engine::_x_infos )
						{
							rux::int32 value = 0;
							rux::int32 glx_red_size = 0;
							rux::int32 glx_green_size = 0;
							rux::int32 glx_blue_size = 0;
							rux::int32 glx_alpha_size = 0;
							rux::int32 glx_depth_size = 0;
							rux::int32 glx_stencil_size = 0;
							rux::int32 glx_accum_red_size = 0;
							rux::int32 glx_accum_blue_size = 0;
							rux::int32 glx_accum_green_size = 0;
							rux::int32 glx_accum_alpha_size = 0;
							rux::int32 temp_glx_red_size = 0;
							rux::int32 temp_glx_green_size = 0;
							rux::int32 temp_glx_blue_size = 0;
							rux::int32 temp_glx_alpha_size = 0;
							rux::int32 temp_glx_depth_size = 0;
							rux::int32 temp_glx_stencil_size = 0;
							rux::int32 temp_glx_accum_red_size = 0;
							rux::int32 temp_glx_accum_blue_size = 0;
							rux::int32 temp_glx_accum_green_size = 0;
							rux::int32 temp_glx_accum_alpha_size = 0;
							rux::int32 temp_glx_sample_buffers_arb = 0;
							rux::int32 temp_glx_samples_arb = 0;
							for( ::rux::int32 index0 = 0 ; index0 < visual_info_count ; index0++ )
							{
								rux::gui::engine::OpenGL::glXGetConfig( ::rux::gui::engine::_x_display_main , &rux::gui::engine::_x_infos[ index0 ] , GLX_RGBA , &value );
								if( value == true )
								{
									rux::gui::engine::OpenGL::glXGetConfig( ::rux::gui::engine::_x_display_main , &rux::gui::engine::_x_infos[ index0 ] , GLX_DOUBLEBUFFER , &value );
									if( value == true )
									{
										rux::gui::engine::OpenGL::glXGetConfig( ::rux::gui::engine::_x_display_main , &rux::gui::engine::_x_infos[ index0 ] , GLX_RED_SIZE , &temp_glx_red_size );
										rux::gui::engine::OpenGL::glXGetConfig( ::rux::gui::engine::_x_display_main , &rux::gui::engine::_x_infos[ index0 ] , GLX_GREEN_SIZE , &temp_glx_green_size );
										rux::gui::engine::OpenGL::glXGetConfig( ::rux::gui::engine::_x_display_main , &rux::gui::engine::_x_infos[ index0 ] , GLX_BLUE_SIZE , &temp_glx_blue_size );
										rux::gui::engine::OpenGL::glXGetConfig( ::rux::gui::engine::_x_display_main , &rux::gui::engine::_x_infos[ index0 ] , GLX_ALPHA_SIZE , &temp_glx_alpha_size );
										rux::gui::engine::OpenGL::glXGetConfig( ::rux::gui::engine::_x_display_main , &rux::gui::engine::_x_infos[ index0 ] , GLX_DEPTH_SIZE , &temp_glx_depth_size );
										rux::gui::engine::OpenGL::glXGetConfig( ::rux::gui::engine::_x_display_main , &rux::gui::engine::_x_infos[ index0 ] , GLX_STENCIL_SIZE , &temp_glx_stencil_size );
										rux::gui::engine::OpenGL::glXGetConfig( ::rux::gui::engine::_x_display_main , &rux::gui::engine::_x_infos[ index0 ] , GLX_ACCUM_RED_SIZE , &temp_glx_accum_red_size );
										rux::gui::engine::OpenGL::glXGetConfig( ::rux::gui::engine::_x_display_main , &rux::gui::engine::_x_infos[ index0 ] , GLX_ACCUM_GREEN_SIZE , &temp_glx_accum_green_size );
										rux::gui::engine::OpenGL::glXGetConfig( ::rux::gui::engine::_x_display_main , &rux::gui::engine::_x_infos[ index0 ] , GLX_ACCUM_BLUE_SIZE , &temp_glx_accum_blue_size );
										rux::gui::engine::OpenGL::glXGetConfig( ::rux::gui::engine::_x_display_main , &rux::gui::engine::_x_infos[ index0 ] , GLX_ACCUM_ALPHA_SIZE , &temp_glx_accum_alpha_size );
										rux::gui::engine::OpenGL::glXGetConfig( ::rux::gui::engine::_x_display_main , &rux::gui::engine::_x_infos[ index0 ] , GLX_SAMPLE_BUFFERS_ARB , &temp_glx_sample_buffers_arb );
										rux::gui::engine::OpenGL::glXGetConfig( ::rux::gui::engine::_x_display_main , &rux::gui::engine::_x_infos[ index0 ] , GLX_SAMPLES_ARB , &temp_glx_samples_arb );
										if( temp_glx_red_size > glx_red_size 
											|| temp_glx_green_size > glx_green_size
											|| temp_glx_blue_size > glx_blue_size
											|| temp_glx_alpha_size > glx_alpha_size
											|| temp_glx_depth_size > glx_depth_size
											|| temp_glx_stencil_size > glx_stencil_size
											|| temp_glx_accum_red_size > glx_accum_red_size
											|| temp_glx_accum_blue_size > glx_accum_blue_size
											|| temp_glx_accum_green_size > glx_accum_green_size
											|| temp_glx_accum_alpha_size > glx_accum_alpha_size )
										{
											glx_red_size = temp_glx_red_size;
											glx_green_size = temp_glx_green_size;
											glx_blue_size = temp_glx_blue_size;
											glx_alpha_size = temp_glx_alpha_size;
											glx_depth_size = temp_glx_depth_size;
											glx_stencil_size = temp_glx_stencil_size;
											glx_accum_red_size = temp_glx_accum_red_size;
											glx_accum_blue_size = temp_glx_accum_blue_size;
											glx_accum_green_size = temp_glx_accum_green_size;
											glx_accum_alpha_size = temp_glx_accum_alpha_size;
											x_visual_info_index = index0;		
										}
									}
								}
							}
						}
						if( x_visual_info_index == -1 )
						{
							::rux::engine::_globals->_x11_module.XFree( ::rux::gui::engine::_x_infos );
							rux::gui::engine::_x_infos = NULL;
							::rux::engine::_globals->_x11_module.XCloseDisplay( ::rux::gui::engine::_x_display_main );
							rux::gui::engine::_x_display_main = NULL;
						}
						else
						{
							rux::gui::engine::_x_visual_info = &rux::gui::engine::_x_infos[ x_visual_info_index ];								
							rux::gui::engine::_x_set_window_attributes.border_pixel = 0;
							rux::gui::engine::_x_set_window_attributes.background_pixel = 0;
							rux::gui::engine::_x_set_window_attributes.event_mask = NoEventMask;
							rux::gui::engine::_wm_delete_window = ::rux::engine::_globals->_x11_module.XInternAtom( ::rux::gui::engine::_x_display_main , "WM_DELETE_WINDOW" , False );
							rux::gui::engine::_net_wm_state = ::rux::engine::_globals->_x11_module.XInternAtom( ::rux::gui::engine::_x_display_main , "_NET_WM_STATE" , False );
							rux::gui::engine::_net_wm_allowed_actions = ::rux::engine::_globals->_x11_module.XInternAtom( ::rux::gui::engine::_x_display_main , "_NET_WM_ALLOWED_ACTIONS" , False );
							rux::gui::engine::_net_close_window = ::rux::engine::_globals->_x11_module.XInternAtom( ::rux::gui::engine::_x_display_main , "_NET_CLOSE_WINDOW" , False );
							rux::gui::engine::_wm_protocols = ::rux::engine::_globals->_x11_module.XInternAtom( ::rux::gui::engine::_x_display_main , "WM_PROTOCOLS" , False );
							rux::gui::engine::_wm_change_state = ::rux::engine::_globals->_x11_module.XInternAtom( ::rux::gui::engine::_x_display_main , "WM_CHANGE_STATE" , False );
							rux::gui::engine::_rux_delete_window = ::rux::engine::_globals->_x11_module.XInternAtom( ::rux::gui::engine::_x_display_main , "RUX_DELETE_WINDOW" , False );
							rux::gui::engine::_rux_gui_initialize = ::rux::engine::_globals->_x11_module.XInternAtom( ::rux::gui::engine::_x_display_main , "RUX_GUI_INITIALIZE" , False );
							rux::gui::engine::_net_wm_state_max_horz = ::rux::engine::_globals->_x11_module.XInternAtom( ::rux::gui::engine::_x_display_main , "_NET_WM_STATE_MAXIMIZED_HORZ" , False );    
							::rux::gui::engine::_net_wm_state_hidden = ::rux::engine::_globals->_x11_module.XInternAtom( ::rux::gui::engine::_x_display_main , "_NET_WM_STATE_HIDDEN" , False );
							rux::gui::engine::_net_wm_state_fullscreen = ::rux::engine::_globals->_x11_module.XInternAtom( ::rux::gui::engine::_x_display_main , "_NET_WM_STATE_FULLSCREEN" , False );    
							rux::gui::engine::_net_wm_state_max_vert = ::rux::engine::_globals->_x11_module.XInternAtom( ::rux::gui::engine::_x_display_main , "_NET_WM_STATE_MAXIMIZED_VERT" , False );
							::rux::engine::_globals->_x11_module.XSelectInput( ::rux::gui::engine::_x_display_main , RootWindow( ::rux::gui::engine::_x_display_main , ::rux::gui::engine::_x_visual_info->screen ) , SubstructureNotifyMask );	
						}
					}
				}
#endif
#endif
			};
			dll_internal void stop( ::rux::gui::on_event_t on_event , void* owner )
			{
				CS_PTR_LOCK( ::rux::gui::engine::_cs_engine );
				if( _on_events.Count() > 1 )
				{
					for( size_t index0 = 0 ; index0 < _on_events.Count() ; index0++ )
					{
						if( _on_events[ index0 ]  == on_event
							&& _on_event_owners[ index0 ] == owner )
						{
							_on_events.RemoveAt( index0 );
							_on_event_owners.RemoveAt( index0 );
							break;
						}
					}
				}
				if( _on_events.Count() == 1 )
				{
					rux::gui::engine::_quit = 1;
					while( XInterlocked::CompareExchange( &::rux::gui::engine::_dispatchers_count , 0 , 0 ) != 0 )
						pump_message( 0 );
				}
				CS_PTR_UNLOCK( ::rux::gui::engine::_cs_engine );
			};
			dll_internal void raise_event( ::rux::gui::Event& xevent )
			{
				CS_PTR_LOCK( ::rux::gui::engine::_cs_engine );
				for( size_t index0 = 0 ; index0 < _on_events.Count() ; index0++ )
					_on_events[ index0 ]( xevent , _on_event_owners[ index0 ] );
				CS_PTR_UNLOCK( ::rux::gui::engine::_cs_engine );
			};
			dll_internal size_t windows_count( void )
			{
				CS_PTR_LOCK( ::rux::gui::engine::_cs_windows );
				size_t count = ::rux::gui::engine::_windows.ItemsCount();
				CS_PTR_UNLOCK( ::rux::gui::engine::_cs_windows );
				return count;
			};
			void dispatch_close_all_windows( void* )
			{
				::rux::gui::engine::Window* window = NULL;
				size_t index0 = 0;
				CS_PTR_LOCK( ::rux::gui::engine::_cs_windows );
				for( ; ::rux::gui::engine::_windows.ItemsCount() > 0 ; )
				{
					window = ::rux::gui::engine::_windows[ index0 ];
					if( window )
					{
						window->AddRef();
						CS_PTR_UNLOCK( ::rux::gui::engine::_cs_windows );
						window->Close();
						CS_PTR_LOCK( ::rux::gui::engine::_cs_windows );
						window->Release();
					}
					index0++;
					if( ::rux::gui::engine::_windows.Count() <= index0 )
						break;
				}
				CS_PTR_UNLOCK( ::rux::gui::engine::_cs_windows );
			};
			dll_internal void close_all_windows( void )
			{
				dispatch( dispatch_close_all_windows , NULL );
			};
			dll_internal void pump_message( ::rux::uint8 wait_for_message )
			{	
				::rux::gui::engine::_is_pump_message++;
				rux::gui::engine::schedule* schedule = 0;
				if( ::rux::gui::engine::_is_pump_message == 1 )
				{
					::rux::engine::_globals->_gui_globals->_cs_gui_schedule->WriteLock( __FILE__ , __FUNCTION__ , __LINE__ );
					if( ::rux::engine::_globals->_gui_globals->_gui_schedule->ItemsCount() > 0 )
					{
						::rux::uint32 now = ::rux::XTime::GetTickCount();
						for( size_t schedule_index = 0 ; schedule_index < ::rux::engine::_globals->_gui_globals->_gui_schedule->Count() ; schedule_index++ )
						{
							schedule = ::rux::engine::_globals->_gui_globals->_gui_schedule->operator[]( schedule_index );
							if( schedule )
							{
								if( schedule->_execute_time > now )
									schedule->_execute_time = now;
								if( now - schedule->_execute_time >= schedule->_timeout )
								{
									schedule->_execute_time = now;
									schedule->_dispatcher( schedule->_param );
								}
							}
						}
					}
					::rux::engine::_globals->_gui_globals->_cs_gui_schedule->WriteUnlock();
				}
				if( XInterlocked::CompareExchange( &::rux::gui::engine::_dispatchers_count , 0 , 0 ) > 0 )
				{
					size_t dispatcher_index = 0;
					::rux::gui::engine::_cs_dispatchers->Lock( 1 , __FILE__ , __FUNCTION__ , __LINE__ , 0 );
					while( ::rux::gui::engine::_dispatchers.ItemsCount() > 0 && dispatcher_index < ::rux::gui::engine::_dispatchers.Count() )
					{
						XUserData3* user_data3 = ::rux::gui::engine::_dispatchers[ dispatcher_index ];
						if( user_data3 )
						{
							::rux::gui::engine::_dispatchers.set_EmptyAt( dispatcher_index );
							CS_PTR_UNLOCK( ::rux::gui::engine::_cs_dispatchers );
							XInterlocked::Decrement( &::rux::gui::engine::_dispatchers_count );
							rux::gui::rux_dispatcher_t rux_dispatcher = (rux::gui::rux_dispatcher_t)user_data3->_udata2;									
							rux_dispatcher( user_data3->_udata0 );
							if( user_data3->_udata1 )
								( (rux::threading::XEvent*)user_data3->_udata1 )->Set();
							rux::engine::free_object< XUserData3 >( user_data3 );
							::rux::gui::engine::_cs_dispatchers->Lock( 1 , __FILE__ , __FUNCTION__ , __LINE__ , 0 );
						}
						dispatcher_index++;
					}
					CS_PTR_UNLOCK( ::rux::gui::engine::_cs_dispatchers );
				}
#ifdef __WINDOWS__
				MSG msg;				
				if( PeekMessageA( &msg , NULL , 0 , 0 , PM_REMOVE ) )
				{
					TranslateMessage( &msg ); 
					DispatchMessageA( &msg );
				}	
#else
#ifdef __ANDROID__
#else
				XEvent event;					
				rux::uint8 next_event = 0;				
				rux::gui::engine::lock_display();
				if( ::rux::gui::engine::_is_x_if_event == 0
					&& ::rux::engine::_globals->_x11_module.XPending( ::rux::gui::engine::_x_display_main ) > 0 )
				{
						next_event = 1;
						if( wait_for_message == 1 )
							::rux::engine::_globals->_x11_module.XNextEvent( ::rux::gui::engine::_x_display_main , &event );						
						else if( ::rux::engine::_globals->_x11_module.XCheckTypedEvent( ::rux::gui::engine::_x_display_main , ClientMessage , &event ) == False )
							next_event = 0;
				}
				rux::gui::engine::unlock_display();
				if( next_event == 1 )
					WndProc( event );	
#endif
#endif
				if( rux::gui::engine::_opengl_one_thread_swap_buffers )
				{
					::rux::uint32 now_tickcount = ::rux::XTime::GetTickCount();
					CS_PTR_LOCK( ::rux::gui::engine::_cs_windows );
					for( size_t index0 = 0 ; index0 < ::rux::gui::engine::_windows.Count() ; index0++ )
					{
						if( ::rux::gui::engine::_windows[ index0 ] )
						{
							::rux::gui::engine::RenderContext* render_context = ::rux::gui::engine::_windows[ index0 ]->_render_context;
							if( render_context )
							{
								if( render_context->_render_time > now_tickcount )
									render_context->_render_time = now_tickcount;
								if( now_tickcount - render_context->_render_time >= render_context->_render_timeout )
								{
									render_context->_render_time = now_tickcount;
									render_context->_gl->set_CurrentContext( ::rux::gui::engine::_windows[ index0 ] );
									render_context->Render( SIZE_MAX );
									render_context->_gl->SwapBuffers( SIZE_MAX );
									if( rux::gui::engine::_log_enable == 1 )
									{
										if( render_context->_fps.get_Runtime() >= 1000ULL * 10000ULL )
										{
											::rux::log::WriteTrace( "window #%u, OpenGL render fps=%.1f" , (::rux::uint32)index0 , render_context->_fps.get_FPS() );
											render_context->_fps.Reset();
										}
										else
											render_context->_fps.Increment();
									}
									render_context->CleanAfterRender();
									render_context->_gl->set_EmptyContext();
								}
								else
								{
									render_context->_gl->set_CurrentContext( ::rux::gui::engine::_windows[ index0 ] );
									render_context->CleanAfterRender();
									render_context->_gl->set_EmptyContext();
								}
							}
						}
					}
					CS_PTR_UNLOCK( ::rux::gui::engine::_cs_windows );
				}
				::rux::gui::engine::_is_pump_message--;
				rux::threading::XThread::Sleep( 1 );
			};
			dll_internal void start( ::rux::gui::on_event_t on_event , void* owner )
			{
				CS_PTR_LOCK( ::rux::gui::engine::_cs_engine );
				_on_events.Add( on_event );
				_on_event_owners.Add( owner );
				if( XInterlocked::CompareExchange( &rux::gui::engine::_is_initialized , 1 , 0 ) == 0 )
				{
					_message_processing_thread_id = ::rux::threading::XThread::get_CurrentThreadId();
					rux::gui::engine::_quit = 0;
					rux::uint8 initialize_event = 1;
					rux::gui::engine::initialize();
					CS_PTR_UNLOCK( ::rux::gui::engine::_cs_engine );
					for( ; ; )
					{
						if( ::rux::gui::engine::_quit == 1 )
							break;
#ifdef __WINDOWS__
						if( initialize_event == 1 )
						{
							initialize_event = 0;
							if( on_event )
							{
								rux::gui::GuiInitializeEvent xevent;
								raise_event( xevent );
							}
						}
#else
#ifdef __ANDROID__
#else
						if( initialize_event == 1 )
						{
							XEvent x_event;
							memset( &x_event , 0 , sizeof( x_event ) );
							x_event.xclient.type = ClientMessage;
							x_event.xclient.message_type = ::rux::gui::engine::_rux_gui_initialize;
							x_event.xclient.format = 32;
							x_event.xclient.data.l[ 0 ] = 0;
							x_event.xclient.data.l[ 1 ] = CurrentTime;		
							::rux::engine::_globals->_x11_module.XPutBackEvent( ::rux::gui::engine::_x_display_main , &x_event );
						}
#endif
#endif
						pump_message();		
#if !defined( __WINDOWS__ )
						if( initialize_event == 1 )
						{
							initialize_event = 0;
							if( on_event )
							{
								rux::gui::GuiInitializeEvent xevent;
								raise_event( xevent );
							}							
						}
#endif
					}
					CS_PTR_LOCK( ::rux::gui::engine::_cs_engine );									
#ifdef __UNIX__
#ifdef __ANDROID__
#else
					if( ::rux::gui::engine::_x_infos )
					{
						::rux::engine::_globals->_x11_module.XFree( ::rux::gui::engine::_x_infos );
						rux::gui::engine::_x_infos = NULL;
					}
					if( ::rux::gui::engine::_x_display_main )
					{
						::rux::engine::_globals->_x11_module.XCloseDisplay( ::rux::gui::engine::_x_display_main );
						rux::gui::engine::_x_display_main = NULL;
					}			
#endif
#endif
					rux::gui::engine::_message_processing_thread_id = 0;
					XInterlocked::CompareExchange( &rux::gui::engine::_is_initialized , 0 , 1 );
					rux::gui::GuiDeInitializeEvent xevent;
					raise_event( xevent );				
					_on_events.Clear();
					CS_PTR_UNLOCK( ::rux::gui::engine::_cs_engine );
				}
				else
					CS_PTR_UNLOCK( ::rux::gui::engine::_cs_engine );
			};
			dll_internal size_t add_schedule( ::rux::gui::rux_dispatcher_t rux_dispatcher , void* param , ::rux::uint64 timeout_in_microseconds )
			{
				if( ::rux::engine::_globals->_gui_globals->_cs_gui_schedule == 0 )
					::rux::engine::_globals->_gui_globals->_cs_gui_schedule = alloc_object_macros( ::rux::threading::RdWrLock );
				if( ::rux::engine::_globals->_gui_globals->_gui_schedule == 0 )
					::rux::engine::_globals->_gui_globals->_gui_schedule = alloc_object_macros( XMallocArray< ::rux::gui::engine::schedule* > );
				::rux::engine::_globals->_gui_globals->_cs_gui_schedule->WriteLock( __FILE__ , __FUNCTION__ , __LINE__ );
				::rux::gui::engine::schedule* schedule = alloc_object_macros( ::rux::gui::engine::schedule );
				schedule->_dispatcher = rux_dispatcher;
				schedule->_param = param;
				schedule->_timeout = (rux::uint32)( timeout_in_microseconds / 1000ULL );
				schedule->_execute_time = 0;
				size_t schedule_index = ::rux::engine::_globals->_gui_globals->_gui_schedule->FillEmptyOrAdd( schedule );
				::rux::engine::_globals->_gui_globals->_cs_gui_schedule->WriteUnlock();
				return schedule_index;
			};
			dll_internal void remove_schedule( size_t schedule_index )
			{
				::rux::engine::_globals->_gui_globals->_cs_gui_schedule->WriteLock( __FILE__ , __FUNCTION__ , __LINE__ );
				if( ::rux::engine::_globals->_gui_globals->_gui_schedule->Count() > schedule_index )
				{
					::rux::gui::engine::schedule* schedule = ::rux::engine::_globals->_gui_globals->_gui_schedule->operator[]( schedule_index );
					::rux::engine::_globals->_gui_globals->_gui_schedule->set_EmptyAt( schedule_index );
					::rux::engine::free_object< ::rux::gui::engine::schedule >( schedule );
				}
				::rux::engine::_globals->_gui_globals->_cs_gui_schedule->WriteUnlock();
			};
			dll_internal ::rux::uint8 dispatch( ::rux::gui::rux_dispatcher_t rux_dispatcher , void* param , ::rux::gui::WindowBase* window_base , ::rux::uint8 wait )
			{
				rux::uint8 res = 0;
				if( XInterlocked::CompareExchange( &rux::gui::engine::_is_initialized , 0 , 0 ) == 1 
					&& _message_processing_thread_id != ::rux::threading::XThread::get_CurrentThreadId() )
				{		
					if( ::rux::gui::engine::_quit == 0 )
					{
						res = 1;
						rux::gui::engine::Window* window = (rux::gui::engine::Window*)window_base;
						if( window )
						{
							if( window->_is_closing == 0 )
							{
								if( wait == 0 )
								{
									XUserData3* user_data3 = alloc_object_3_params_macros( XUserData3 , param , (void*)NULL , (void*)rux_dispatcher );									
									CS_PTR_LOCK( ::rux::gui::engine::_cs_dispatchers );
									::rux::gui::engine::_dispatchers.FillEmptyOrAdd( user_data3 );
									CS_PTR_UNLOCK( ::rux::gui::engine::_cs_dispatchers );
									XInterlocked::Increment( &::rux::gui::engine::_dispatchers_count );
								}
								else
								{
									rux::threading::XEvent thread_event;
									thread_event.Reset();
									XUserData3* user_data3 = alloc_object_3_params_macros( XUserData3 , param , &thread_event , (void*)rux_dispatcher );
									CS_PTR_LOCK( ::rux::gui::engine::_cs_dispatchers );
									size_t fill_index = ::rux::gui::engine::_dispatchers.FillEmptyOrAdd( user_data3 );
									CS_PTR_UNLOCK( ::rux::gui::engine::_cs_dispatchers );
									XInterlocked::Increment( &::rux::gui::engine::_dispatchers_count );
									while( EVENT_WAIT( thread_event , 100 ) == 0 )
									{
										if( window->_is_closing == 1 
											|| ::rux::gui::engine::_quit == 1 )
										{
											res = 0;
											CS_PTR_LOCK( ::rux::gui::engine::_cs_dispatchers );
											user_data3 = ::rux::gui::engine::_dispatchers[ fill_index ];
											if( user_data3 )
											{
												::rux::gui::engine::_dispatchers.set_EmptyAt( fill_index );
												CS_PTR_UNLOCK( ::rux::gui::engine::_cs_dispatchers );
												XInterlocked::Decrement( &::rux::gui::engine::_dispatchers_count );
												rux::engine::free_object< XUserData3 >( user_data3 );
											}
											else
												CS_PTR_UNLOCK( ::rux::gui::engine::_cs_dispatchers );
											break;
										}
									}
								}
							}
						}
						else
						{
							rux::threading::XEvent thread_event;
							thread_event.Reset();
							XUserData3* user_data3 = wait == 1 ? alloc_object_3_params_macros( XUserData3 , param , &thread_event , (void*)rux_dispatcher ) : alloc_object_3_params_macros( XUserData3 , param , (void*)NULL , (void*)rux_dispatcher );
							CS_PTR_LOCK( ::rux::gui::engine::_cs_dispatchers );
							size_t fill_index = ::rux::gui::engine::_dispatchers.FillEmptyOrAdd( user_data3 );
							CS_PTR_UNLOCK( ::rux::gui::engine::_cs_dispatchers );
							XInterlocked::Increment( &::rux::gui::engine::_dispatchers_count );
							if( wait == 1 )
							{
								while( EVENT_WAIT( thread_event , 100 ) == 0 )
								{
									if( ::rux::gui::engine::_quit == 1 )
									{
										res = 0;
										CS_PTR_LOCK( ::rux::gui::engine::_cs_dispatchers );
										user_data3 = ::rux::gui::engine::_dispatchers[ fill_index ];
										if( user_data3 )
										{
											::rux::gui::engine::_dispatchers.set_EmptyAt( fill_index );
											CS_PTR_UNLOCK( ::rux::gui::engine::_cs_dispatchers );
											XInterlocked::Decrement( &::rux::gui::engine::_dispatchers_count );
											rux::engine::free_object< XUserData3 >( user_data3 );
										}
										else
											CS_PTR_UNLOCK( ::rux::gui::engine::_cs_dispatchers );
										break;
									}
								}
							}
						}
					}
				}
				else
				{
					res = 1;
					rux_dispatcher( param );
				}
				return res;
			};			
			dll_internal void dispatch_create_window( void* param )
			{
				if( XInterlocked::CompareExchange( &rux::gui::engine::_is_initialized , 0 , 0 ) == 1 
					&& param )
				{
					rux::gui::engine::Window* window = (rux::gui::engine::Window*)param;
#ifdef __WINDOWS__
					if( window->_hwnd == NULL )
					{
						rux::uint32 ex_style = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE , style = WS_POPUP;
						rux::uint8 is_fullscreen = 0;
						if( is_fullscreen == 1 )
						{	
							ex_style = WS_EX_APPWINDOW;
							ShowCursor( false );
						}
						if( window->_is_top_most )
							ex_style |= WS_EX_TOPMOST;
						window->_hwnd = CreateWindowExA( ex_style , "rux_ui_window" , "" , WS_CLIPSIBLINGS | WS_CLIPCHILDREN | style , 0 , 0 , 200 , 200 , NULL , NULL , GetModuleHandle( NULL ) , NULL );
						if( window->_hwnd )
						{
							if( rux::gui::engine::_winapi_trace )
								::rux::log::WriteTrace( "CreateWindowExA, hwnd=%p" , window->_hwnd );
							SetWindowLongPtrA( window->_hwnd , GWLP_USERDATA , (LONG_PTR)window );
						}
					}
#elif defined( __UNIX__ )
#ifdef __ANDROID__
#else
					window->_x_display = ::rux::engine::_globals->_x11_module.XOpenDisplay( NULL );	
					rux::gui::engine::_x_set_window_attributes.colormap = ::rux::engine::_globals->_x11_module.XCreateColormap( window->_x_display , RootWindow( ::rux::gui::engine::_x_display_main , ::rux::gui::engine::_x_visual_info->screen ) , ::rux::gui::engine::_x_visual_info->visual , AllocNone );
					if( window->_width < window->_min_width )
						window->_width = window->_min_width;
					if( window->_height < window->_min_height )
						window->_height = window->_min_height;
					window->_x_window = ::rux::engine::_globals->_x11_module.XCreateWindow( window->_x_display , RootWindow( ::rux::gui::engine::_x_display_main , ::rux::gui::engine::_x_visual_info->screen ) , window->_left , window->_top , window->_width , window->_height , 0 , ::rux::gui::engine::_x_visual_info->depth , InputOutput , ::rux::gui::engine::_x_visual_info->visual , CWBorderPixel | CWColormap | CWEventMask | CWOverrideRedirect , &rux::gui::engine::_x_set_window_attributes );					
					::rux::log::WriteTrace( "dispatch_create_window, %p, x=%d, y=%d, width=%d, height=%d" , window , window->_left , window->_top , window->_width , window->_height );
					MWMHints mwmhints;
					Atom prop;
					memset( &mwmhints , 0 , sizeof( mwmhints ) );
					prop = ::rux::engine::_globals->_x11_module.XInternAtom( window->_x_display , "_MOTIF_WM_HINTS" , false );
					mwmhints.flags = MWM_HINTS_DECORATIONS;
					mwmhints.decorations = 0;
					::rux::engine::_globals->_x11_module.XChangeProperty( window->_x_display , window->_x_window , prop , prop , 32 , PropModeReplace , (rux::uint8*)&mwmhints , PROP_MWM_HINTS_ELEMENTS );
					::rux::engine::_globals->_x11_module.XSetWMProtocols( window->_x_display , window->_x_window , &rux::gui::engine::_wm_delete_window , 1 );								
					::rux::engine::_globals->_x11_module.XSelectInput( ::rux::gui::engine::_x_display_main , window->_x_window , NoEventMask | KeyPressMask | KeyReleaseMask | ButtonPressMask |				
						ButtonReleaseMask | EnterWindowMask | LeaveWindowMask | PointerMotionMask |
						Button1MotionMask | Button2MotionMask | Button3MotionMask | Button4MotionMask |
						Button5MotionMask | ButtonMotionMask | KeymapStateMask | ExposureMask |
						VisibilityChangeMask | StructureNotifyMask | SubstructureNotifyMask |
						SubstructureRedirectMask | FocusChangeMask | PropertyChangeMask | ColormapChangeMask | OwnerGrabButtonMask );				
					::rux::engine::_globals->_x11_module.XSetWMProtocols( ::rux::gui::engine::_x_display_main , window->_x_window , &rux::gui::engine::_wm_delete_window , 1 );													
					::rux::engine::_globals->_x11_module.XSetWMProtocols( ::rux::gui::engine::_x_display_main , window->_x_window , &rux::gui::engine::_net_wm_state , 1 );													
					::rux::engine::_globals->_x11_module.XSetWMProtocols( ::rux::gui::engine::_x_display_main , window->_x_window , &rux::gui::engine::_wm_change_state , 1 );
					::rux::engine::_globals->_x11_module.XSetWMProtocols( ::rux::gui::engine::_x_display_main , window->_x_window , &rux::gui::engine::_rux_delete_window , 1 );													
					::rux::engine::_globals->_x11_module.XSetWMProtocols( ::rux::gui::engine::_x_display_main , window->_x_window , &rux::gui::engine::_rux_gui_initialize , 1 );							
					::rux::engine::_globals->_x11_module.XSetWMProtocols( ::rux::gui::engine::_x_display_main , window->_x_window , &rux::gui::engine::_net_wm_allowed_actions , 1 );
					::rux::engine::_globals->_x11_module.XSetWMProtocols( ::rux::gui::engine::_x_display_main , window->_x_window , &rux::gui::engine::_net_close_window , 1 );
					::rux::engine::_globals->_x11_module.XSetWMProtocols( ::rux::gui::engine::_x_display_main , window->_x_window , &rux::gui::engine::_wm_protocols , 1 );
					::rux::engine::_globals->_x11_module.XSetWMProtocols( ::rux::gui::engine::_x_display_main , window->_x_window , &rux::gui::engine::_net_wm_state_max_horz , 1 );
					::rux::engine::_globals->_x11_module.XSetWMProtocols( ::rux::gui::engine::_x_display_main , window->_x_window , &rux::gui::engine::_net_wm_state_hidden , 1 );
					::rux::engine::_globals->_x11_module.XSetWMProtocols( ::rux::gui::engine::_x_display_main , window->_x_window , &rux::gui::engine::_net_wm_state_fullscreen , 1 );
					::rux::engine::_globals->_x11_module.XSetWMProtocols( ::rux::gui::engine::_x_display_main , window->_x_window , &rux::gui::engine::_net_wm_state_max_vert , 1 );
#endif
#endif						
				}
			};
			dll_internal void add_cache( ::rux::gui::RenderCacheBase* cache )
			{
				if( cache )
				{
					RenderUniversalCache* render_cache = (RenderUniversalCache*)cache;
					WRITE_LOCK( render_cache->_gl_context->_cs_caches );
					render_cache->_gl_context->_render_caches.FillEmptyOrAdd( render_cache );
					render_cache->_gl_context->_cs_caches.WriteUnlock();
				}
			};
			dll_internal XMallocArray< ::rux::gui::ScreenBase* >* get_screens( void )
			{
				return ::rux::gui::engine::Screen::get_Screens();
			};
			dll_internal void free_screens( XMallocArray< ::rux::gui::ScreenBase* >* screens )
			{
				return ::rux::gui::engine::Screen::free( screens );
			};
			dll_internal ::rux::gui::FontBase* create_font( const char* font_file_name ,
				rux::uint32 font_size_height ,
				rux::uint32 font_size_width )
			{
				::rux::uint32 font_file_name_hash = ::rux::cryptography::hash::times33_hash( font_file_name , SIZE_MAX );
				rux::gui::engine::RenderFont* fontbase = NULL;
				size_t index0 = 0;
				CS_PTR_LOCK( ::rux::gui::engine::_cs_font_bases );
				for( ; index0 < ::rux::gui::engine::_font_bases.Count() ; index0++ )
				{
					if( ( font_file_name_hash == ::rux::gui::engine::_font_bases[ index0 ]->_font_file_name_hash
						|| font_file_name_hash == ::rux::gui::engine::_font_bases[ index0 ]->_font_short_name_hash )
						&& font_size_width == ::rux::gui::engine::_font_bases[ index0 ]->_font_size_width 
						&& font_size_height == ::rux::gui::engine::_font_bases[ index0 ]->_font_size_height )
					{
						fontbase = ::rux::gui::engine::_font_bases[ index0 ];
						break;
					}
				}
				if( index0 >= ::rux::gui::engine::_font_bases.Count() )
				{
					fontbase = alloc_object_4_params_macros( ::rux::gui::engine::RenderFont , (rux::gui::engine::GLContext*)NULL , font_file_name , font_size_width , font_size_height );
					rux::gui::engine::_font_bases.Add( fontbase );
				}
				CS_PTR_UNLOCK( ::rux::gui::engine::_cs_font_bases );
				return fontbase;
			};
			dll_internal void destroy_window( ::rux::gui::WindowBase* window_base )
			{
				if( window_base )
				{
					rux::gui::engine::Window* window = (rux::gui::engine::Window*)window_base;
					CS_PTR_LOCK( ::rux::gui::engine::_cs_windows );
					if( window->_windows_index != 0xffffffff )
						rux::gui::engine::_windows.set_EmptyAt( window->_windows_index );
					CS_PTR_UNLOCK( ::rux::gui::engine::_cs_windows );
					window->free_resources();
					window->Release();
				}
			};
			dll_internal ::rux::gui::WindowBase* create_window( ::rux::gui::on_event_t on_event )
			{
				rux::gui::engine::Window* window = alloc_object_macros( ::rux::gui::engine::Window );
				window->_on_event = on_event;
				return window;
			};
#ifdef __WINDOWS__
			dll_internal ::rux::gui::engine::Window* find_window( HWND hwnd )
			{
				rux::gui::engine::Window* window = NULL;
				CS_PTR_LOCK( ::rux::gui::engine::_cs_windows );
				for( size_t index0 = 0 ; index0 < ::rux::gui::engine::_windows.Count() ; index0++ )
				{
					window = ::rux::gui::engine::_windows[ index0 ];
					if( window && window->_hwnd == hwnd )
					{
						window->AddRef();
						break;
					}
					else
						window = NULL;
				}
				CS_PTR_UNLOCK( ::rux::gui::engine::_cs_windows );
				return window;
			};
			LRESULT CALLBACK WndProc( HWND hwnd , ::rux::uint32 msg , WPARAM wparam , LPARAM lparam )
			{
				if( rux::gui::engine::_winapi_trace && hwnd )
					::rux::log::WriteTrace( "WndProc, hwnd=%p, msg=0x%x" , hwnd , msg );
				::rux::byte need_release = 0;
				rux::gui::engine::Window* window = (rux::gui::engine::Window*)GetWindowLongPtr( hwnd , GWLP_USERDATA );				
				if( hwnd && window == NULL )
				{
					window = find_window( hwnd );
					need_release = 1;
				}
				switch( msg )
				{				
				case WM_QUIT:
					{
						break;
					}
				case WM_CREATE:
					{						
						if( window )
						{
							rux::gui::WindowLoadedEvent xevent( window );
							window->raise_event( xevent );
						}
						break;
					}
				case WM_SHOWWINDOW:
					{
						if( wparam == TRUE && lparam == 0 )
						{
							window->_is_visible = 1;
							rux::gui::WindowShownEvent xevent( window );
							window->raise_event( xevent );
						}
						else if( wparam == FALSE && lparam == 0 )
						{
							window->_is_visible = 0;
							rux::gui::WindowHidedEvent xevent( window );
							window->raise_event( xevent );
						}
						if( lparam == SW_OTHERUNZOOM )
						{
							if( rux::gui::engine::_winapi_trace )
								::rux::log::WriteTrace( "SW_OTHERUNZOOM" );
						}
						else if( lparam == SW_OTHERZOOM )
						{
							::rux::log::WriteTrace( "SW_OTHERZOOM" );
						}
						else if( lparam == SW_PARENTCLOSING )
						{
							if( rux::gui::engine::_winapi_trace )
								::rux::log::WriteTrace( "SW_PARENTCLOSING" );
						}
						else if( lparam == SW_PARENTOPENING )
						{
							if( rux::gui::engine::_winapi_trace )
								::rux::log::WriteTrace( "SW_PARENTOPENING" );
						}
						break;
					}
				case WM_ACTIVATE:
					{
						if( !HIWORD( wparam ) )
						{									
							//active = true;
						}
						else
						{
							//active = false;
						}		
						if( window && need_release )
							window->Release();
						return 0;
					}
				case WM_SYSCOMMAND:
					{
						switch( wparam )
						{
							case SC_CLOSE:
							case SC_SCREENSAVE:
							case SC_MONITORPOWER:
								{
									if( window && need_release )
										window->Release();
									return 0;
								}
							case SC_MAXIMIZE:
								{
									break;
								}
							case SC_RESTORE:
								{
									break;
								}
							case SC_MINIMIZE:
								{
									break;
								}
							case SC_KEYMENU:
								{
									break;
								}
						}
						break;
					}
				case WM_CLOSE:
					{	
						if( window )
						{
							if( window->_is_allow_close == 1 )
							{
								if( window->_windows_index != 0xffffffff )
								{
									window->_is_closing = 1;
									if( window->_render_context )
										window->_render_context->DestroyRenderThread();
									CS_PTR_LOCK( ::rux::gui::engine::_cs_windows );
									rux::gui::engine::_windows.set_EmptyAt( window->_windows_index );
									window->_windows_index = 0xffffffff;
									CS_PTR_UNLOCK( ::rux::gui::engine::_cs_windows );
									rux::gui::WindowClosedEvent xevent( window );
									window->raise_event( xevent );
									window->free_resources();
									window->Release();							
								}
								if( need_release )
									window->Release();
							}
							else
							{
								if( need_release )
									window->Release();
								return 1;
							}
						}
						return 0;
					}
				case WM_SYSKEYDOWN:
				case WM_KEYDOWN:
					{
						rux::gui::XEnum_Keys key = XEnum_Keys_Unknown;
						switch( wparam )
						{
						case VK_LEFT:
							{
								key = XEnum_Keys_Left;	
								break;
							}
						case VK_TAB:
							{
								key = XEnum_Keys_Tab;	
								break;
							}
						case VK_RIGHT:
							{
								key = 	XEnum_Keys_Right;
								break;
							}
						case VK_UP:
							{
								key = XEnum_Keys_Up;
								break;
							}
						case VK_DOWN:
							{
								key = XEnum_Keys_Down;
								break;
							}
						case VK_HOME:
							{
								key = XEnum_Keys_Home;
								break;
							}
						case VK_END:
							{
								key = XEnum_Keys_End;
								break;
							}
						case VK_INSERT:
							{								
								break;
							}
						case VK_BACK:
							{
								key = XEnum_Keys_Back;
								break;
							}
						case VK_DELETE:
							{
								key = XEnum_Keys_Delete;
								break;
							}
						case VK_F2:
							{								
								break;
							}
						case VK_RETURN:
							{
								key = XEnum_Keys_Enter;
								break;
							}
						case 0x56:
							{
								//key = XEnum_Keys_V;
								break;
							}
						default:
							{
								break;
							}
						}
						rux::uint8 shift = ( ( HIWORD( GetKeyState( VK_LSHIFT ) ) ) & 128 ) == 128 ? 1 : 0;
						if( shift == 0 )
							shift = (  ( HIWORD( GetKeyState( VK_RSHIFT ) ) ) & 128 ) == 128 ? 1 : 0;
						rux::uint8 control = ( ( HIWORD( GetKeyState( VK_LCONTROL ) ) ) & 128 ) == 128 ? 1 : 0;
						if( control == 0 )
							control = ( ( HIWORD( GetKeyState( VK_RCONTROL ) ) ) & 128 ) == 128 ? 1 : 0;
						rux::uint8 alt = ( ( HIWORD( GetKeyState( VK_LMENU ) ) ) & 128 ) == 128 ? 1 : 0;
						if( alt == 0 )
							alt = ( ( HIWORD( GetKeyState( VK_RMENU ) ) ) & 128 ) == 128 ? 1 : 0;
						rux::gui::WindowKeyDownEvent xevent( window , key , alt , control , shift , wparam );
						window->raise_event( xevent );
						if( window && need_release )
							window->Release();
						return 0;
					}
				case WM_SYSKEYUP:
				case WM_KEYUP:
					{
						rux::gui::XEnum_Keys key = XEnum_Keys_Unknown;
						switch( wparam )
						{
						case VK_LEFT:
							{
								key = XEnum_Keys_Left;	
								break;
							}
						case VK_TAB:
							{
								key = XEnum_Keys_Tab;	
								break;
							}
						case VK_RIGHT:
							{
								key = 	XEnum_Keys_Right;
								break;
							}
						case VK_UP:
							{
								key = XEnum_Keys_Up;
								break;
							}
						case VK_DOWN:
							{
								key = XEnum_Keys_Down;
								break;
							}
						case VK_HOME:
							{
								key = XEnum_Keys_Home;
								break;
							}
						case VK_END:
							{
								key = XEnum_Keys_End;
								break;
							}
						case VK_INSERT:
							{								
								break;
							}
						case VK_BACK:
							{
								key = XEnum_Keys_Back;
								break;
							}
						case VK_DELETE:
							{
								key = XEnum_Keys_Delete;
								break;
							}
						case VK_F2:
							{								
								break;
							}
						case VK_RETURN:
							{
								key = XEnum_Keys_Enter;
								break;
							}
						case 0x56:
							{
								//key = XEnum_Keys_V;
								break;
							}
						default:
							{
								break;
							}
						}
						rux::uint8 shift = ( ( HIWORD( GetKeyState( VK_LSHIFT ) ) ) & 128 ) == 128 ? 1 : 0;
						if( shift == 0 )
							shift = (  ( HIWORD( GetKeyState( VK_RSHIFT ) ) ) & 128 ) == 128 ? 1 : 0;
						rux::uint8 control = ( ( HIWORD( GetKeyState( VK_LCONTROL ) ) ) & 128 ) == 128 ? 1 : 0;
						if( control == 0 )
							control = ( ( HIWORD( GetKeyState( VK_RCONTROL ) ) ) & 128 ) == 128 ? 1 : 0;
						rux::uint8 alt = ( ( HIWORD( GetKeyState( VK_LMENU ) ) ) & 128 ) == 128 ? 1 : 0;
						if( alt == 0 )
							alt = ( ( HIWORD( GetKeyState( VK_RMENU ) ) ) & 128 ) == 128 ? 1 : 0;
						rux::gui::WindowKeyUpEvent xevent( window , key , alt , control , shift , wparam );
						window->raise_event( xevent );
						if( window && need_release )
							window->Release();
						return 0;
					}
				case WM_CHAR:
					{
						switch( wparam )
						{
						case 0x08:
						case 0x0A:
						case 0x1B:
						case 0x09:
						case 0x0D:
							{
								break;
							}
						case 0x01:
							{
								rux::uint8 shift = ( ( HIWORD( GetKeyState( VK_LSHIFT ) ) ) & 128 ) == 128 ? 1 : 0;
								if( shift == 0 )
									shift = ( ( HIWORD( GetKeyState( VK_RSHIFT ) ) ) & 128 ) == 128 ? 1 : 0;
								rux::uint8 control = ( ( HIWORD( GetKeyState( VK_LCONTROL ) ) ) & 128 ) == 128 ? 1 : 0;
								if( control == 0 )
									control = ( ( HIWORD( GetKeyState( VK_RCONTROL ) ) ) & 128 ) == 128 ? 1 : 0;
								rux::uint8 alt = ( ( HIWORD( GetKeyState( VK_LMENU ) ) ) & 128 ) == 128 ? 1 : 0;
								if( alt == 0 )
									alt = ( ( HIWORD( GetKeyState( VK_RMENU ) ) ) & 128 ) == 128 ? 1 : 0;
								rux::gui::WindowKeyDownEvent xevent( window , XEnum_Keys_Control_A , alt , control , shift , 0 );
								window->raise_event( xevent );
								break;
							}
						case 0x04:
							{
								rux::uint8 shift = ( ( HIWORD( GetKeyState( VK_LSHIFT ) ) ) & 128 ) == 128 ? 1 : 0;
								if( shift == 0 )
									shift = ( ( HIWORD( GetKeyState( VK_RSHIFT ) ) ) & 128 ) == 128 ? 1 : 0;
								rux::uint8 control = ( ( HIWORD( GetKeyState( VK_LCONTROL ) ) ) & 128 ) == 128 ? 1 : 0;
								if( control == 0 )
									control = ( ( HIWORD( GetKeyState( VK_RCONTROL ) ) ) & 128 ) == 128 ? 1 : 0;
								rux::uint8 alt = ( ( HIWORD( GetKeyState( VK_LMENU ) ) ) & 128 ) == 128 ? 1 : 0;
								if( alt == 0 )
									alt = ( ( HIWORD( GetKeyState( VK_RMENU ) ) ) & 128 ) == 128 ? 1 : 0;
								rux::gui::WindowKeyDownEvent xevent( window , XEnum_Keys_Control_D , alt , control , shift , 0 );
								window->raise_event( xevent );
								break;
							}
						case 0x16:
							{							
								rux::uint8 shift = ( ( HIWORD( GetKeyState( VK_LSHIFT ) ) ) & 128 ) == 128 ? 1 : 0;
								if( shift == 0 )
									shift = ( ( HIWORD( GetKeyState( VK_RSHIFT ) ) ) & 128 ) == 128 ? 1 : 0;
								rux::uint8 control = ( ( HIWORD( GetKeyState( VK_LCONTROL ) ) ) & 128 ) == 128 ? 1 : 0;
								if( control == 0 )
									control = ( ( HIWORD( GetKeyState( VK_RCONTROL ) ) ) & 128 ) == 128 ? 1 : 0;
								rux::uint8 alt = ( ( HIWORD( GetKeyState( VK_LMENU ) ) ) & 128 ) == 128 ? 1 : 0;
								if( alt == 0 )
									alt = ( ( HIWORD( GetKeyState( VK_RMENU ) ) ) & 128 ) == 128 ? 1 : 0;
								rux::gui::WindowKeyDownEvent xevent( window , XEnum_Keys_Control_V , alt , control , shift , 0 );
								window->raise_event( xevent );
								break;
							}
						case 0x18:
							{
								rux::uint8 shift = ( ( HIWORD( GetKeyState( VK_LSHIFT ) ) ) & 128 ) == 128 ? 1 : 0;
								if( shift == 0 )
									shift = ( ( HIWORD( GetKeyState( VK_RSHIFT ) ) ) & 128 ) == 128 ? 1 : 0;
								rux::uint8 control = ( ( HIWORD( GetKeyState( VK_LCONTROL ) ) ) & 128 ) == 128 ? 1 : 0;
								if( control == 0 )
									control = ( ( HIWORD( GetKeyState( VK_RCONTROL ) ) ) & 128 ) == 128 ? 1 : 0;
								rux::uint8 alt = ( ( HIWORD( GetKeyState( VK_LMENU ) ) ) & 128 ) == 128 ? 1 : 0;
								if( alt == 0 )
									alt = ( ( HIWORD( GetKeyState( VK_RMENU ) ) ) & 128 ) == 128 ? 1 : 0;
								rux::gui::WindowKeyDownEvent xevent( window , XEnum_Keys_Control_X , alt , control , shift , 0 );
								window->raise_event( xevent );
								break;
							}
						case 0x03:
							{							
								rux::uint8 shift = ( ( HIWORD( GetKeyState( VK_LSHIFT ) ) ) & 128 ) == 128 ? 1 : 0;
								if( shift == 0 )
									shift = ( ( HIWORD( GetKeyState( VK_RSHIFT ) ) ) & 128 ) == 128 ? 1 : 0;
								rux::uint8 control = ( ( HIWORD( GetKeyState( VK_LCONTROL ) ) ) & 128 ) == 128 ? 1 : 0;
								if( control == 0 )
									control = ( ( HIWORD( GetKeyState( VK_RCONTROL ) ) ) & 128 ) == 128 ? 1 : 0;
								rux::uint8 alt = ( ( HIWORD( GetKeyState( VK_LMENU ) ) ) & 128 ) == 128 ? 1 : 0;
								if( alt == 0 )
									alt = ( ( HIWORD( GetKeyState( VK_RMENU ) ) ) & 128 ) == 128 ? 1 : 0;
								rux::gui::WindowKeyDownEvent xevent( window , XEnum_Keys_Control_C , alt , control , shift , 0 );
								window->raise_event( xevent );
								break;
							}
						default:
							{
								if( wparam >= 0x20 )
								{
									rux::uint8 shift = ( ( HIWORD( GetKeyState( VK_LSHIFT ) ) ) & 128 ) == 128 ? 1 : 0;
									if( shift == 0 )
										shift = ( ( HIWORD( GetKeyState( VK_RSHIFT ) ) ) & 128 ) == 128 ? 1 : 0;
									rux::uint8 control = ( ( HIWORD( GetKeyState( VK_LCONTROL ) ) ) & 128 ) == 128 ? 1 : 0;
									if( control == 0 )
										control = ( ( HIWORD( GetKeyState( VK_RCONTROL ) ) ) & 128 ) == 128 ? 1 : 0;
									rux::uint8 alt = ( ( HIWORD( GetKeyState( VK_LMENU ) ) ) & 128 ) == 128 ? 1 : 0;
									if( alt == 0 )
										alt = ( ( HIWORD( GetKeyState( VK_RMENU ) ) ) & 128 ) == 128 ? 1 : 0;
									declare_variable( rux::XString , symbol );
									symbol.Insert( 0 , (char)wparam );
									symbol.set_ByRef( symbol.ConvertToUTF8() );
									rux::gui::WindowKeyCharEvent xevent( window , symbol.str() , symbol.Size() , alt , control , shift );

									window->raise_event( xevent );
								}
								break;
							}
						}
						if( window && need_release )
							window->Release();
						return 0;
					}
				case WM_WINDOWPOSCHANGING:
					{
						if( window )
						{
							WINDOWPOS* windowpos = (WINDOWPOS*)lparam;
							if( window->_is_allow_relocate == 0 )
								windowpos->flags |= SWP_NOMOVE;
							if( window->_is_allow_resize == 0 )
								windowpos->flags |= SWP_NOSIZE;
							if( ( windowpos->flags & SWP_NOMOVE ) == false && ( windowpos->flags & SWP_NOSIZE ) == false )
								window->_both_size_and_move_msg = 1;
							else
								window->_both_size_and_move_msg = 0;
						}
						break;
					}
				case WM_SIZE:
					{
						if( window )
						{
							rux::int32 width = (int)(short)LOWORD( lparam );
							rux::int32 height = (int)(short)HIWORD( lparam );
							if( wparam == SIZE_MAXIMIZED )
							{
								if( window->_window_state != ::rux::gui::XEnum_WindowState_Maximized )
								{
									window->_prev_window_state = window->_window_state;
									window->_window_state = ::rux::gui::XEnum_WindowState_Maximized;
									rux::gui::WindowMaximizedStateEvent xevent( window );
									window->raise_event( xevent );
								}
							}
							else if( wparam == SIZE_MAXSHOW )
							{
								if( rux::gui::engine::_winapi_trace )
									::rux::log::WriteTrace( "SIZE_MAXSHOW" );
							}
							else if( wparam == SIZE_MINIMIZED )
							{
								if( window->_window_state != ::rux::gui::XEnum_WindowState_Minimized )
								{
									window->_prev_window_state = window->_window_state;
									window->_window_state = ::rux::gui::XEnum_WindowState_Minimized;
									rux::gui::WindowMinimizedStateEvent xevent( window );
									window->raise_event( xevent );
								}
							}
							else if( wparam == SIZE_RESTORED )
							{
								if( window->_window_state != XEnum_WindowState_Normal )
								{
									if( window->_window_state == XEnum_WindowState_Maximized )
									{
										window->_prev_window_state = window->_window_state;
										window->_window_state = ::rux::gui::XEnum_WindowState_Normal;
										rux::gui::WindowNormalStateEvent xevent( window );
										window->raise_event( xevent );
									}
									else
									{
										if( window->_prev_window_state == XEnum_WindowState_Maximized )
										{
											window->_prev_window_state = window->_window_state;
											window->_window_state = XEnum_WindowState_Maximized;
											rux::gui::WindowMaximizedStateEvent xevent( window );
											window->raise_event( xevent );
										}
										else
										{
											window->_prev_window_state = window->_window_state;
											window->_window_state = ::rux::gui::XEnum_WindowState_Normal;
											rux::gui::WindowNormalStateEvent xevent( window );
											window->raise_event( xevent );
										}
									}
								}
							}							
							if( window->_width != width 
								|| window->_height != height )
							{
								window->_width = width;
								window->_height = height;
								if( window->_render_context )
									window->_render_context->Resize( window->_width , window->_height );
								if( window->_both_size_and_move_msg == 1 )
								{
									rux::gui::WindowMoveEvent xevent( window );
									window->raise_event( xevent );
								}
								rux::gui::WindowSizeEvent xevent( window , window->_width , window->_height );
								window->raise_event( xevent );
							}							
						}
						if( window && need_release )
							window->Release();
						return 0;
					}
				case WM_MOVE:
					{
						if( window )
						{
							rux::int32 left = (int)(short)LOWORD( lparam );
							rux::int32 top = (int)(short)HIWORD( lparam );
							if( window->_left != left 
								|| window->_top != top )
							{
								window->_left = left;
								window->_top = top;
								if( window->_both_size_and_move_msg == 0 )
								{
									rux::gui::WindowMoveEvent xevent( window );
									window->raise_event( xevent );
								}
							}
						}
						if( window && need_release )
							window->Release();
						return 0;
					}
				case WM_ERASEBKGND:		
					{
						if( window && need_release )
							window->Release();
						return TRUE;
					}
				case WM_NCPAINT:
					{
						if( window && need_release )
							window->Release();
						return 0;
					}
				case WM_PAINT:
					{
						/*if( rux::gui::engine::_opengl_one_thread_swap_buffers == 1 )
						{
							if( window )
							{
								HDC hdc = NULL;
								PAINTSTRUCT paint_struct;
								hdc = BeginPaint( window->_hwnd , &paint_struct );
								window->SwapBuffers();
								EndPaint( window->_hwnd , &paint_struct );
							}
						}*/
						if( window && need_release )
							window->Release();
						return 0;
					}
				case WM_MBUTTONDBLCLK:
					{
						rux::uint8 shift = ( ( HIWORD( GetKeyState( VK_LSHIFT ) ) ) & 128 ) == 128 ? 1 : 0;
						if( shift == 0 )
							shift = (  ( HIWORD( GetKeyState( VK_RSHIFT ) ) ) & 128 ) == 128 ? 1 : 0;
						rux::uint8 control = ( ( HIWORD( GetKeyState( VK_LCONTROL ) ) ) & 128 ) == 128 ? 1 : 0;
						if( control == 0 )
							control = ( ( HIWORD( GetKeyState( VK_RCONTROL ) ) ) & 128 ) == 128 ? 1 : 0;
						rux::uint8 alt = ( ( HIWORD( GetKeyState( VK_LMENU ) ) ) & 128 ) == 128 ? 1 : 0;
						if( alt == 0 )
							alt = ( ( HIWORD( GetKeyState( VK_RMENU ) ) ) & 128 ) == 128 ? 1 : 0;
						rux::uint8 left_mouse_button_pressed = wparam & MK_LBUTTON;
						rux::uint8 middle_mouse_button_pressed = wparam & MK_MBUTTON;
						rux::uint8 right_mouse_button_pressed = wparam & MK_RBUTTON;
						rux::uint8 x_button1_pressed = wparam & MK_XBUTTON1;
						rux::uint8 x_button2_pressed = wparam & MK_XBUTTON2;
						rux::int16 x = LOWORD( lparam );
						rux::int16 y = HIWORD( lparam );
						rux::gui::WindowMouseEvent xevent(::rux::gui::XEnum_EventType_WindowMouseWheelDoubleClick, window
							, 0, x, y, alt, control, shift, left_mouse_button_pressed, middle_mouse_button_pressed
							, right_mouse_button_pressed, x_button1_pressed, x_button2_pressed);
						window->raise_event(xevent);
						if(window && need_release)
							window->Release();
						return 0;
					}
				case WM_MBUTTONUP:
					{
						rux::uint8 shift = ( ( HIWORD( GetKeyState( VK_LSHIFT ) ) ) & 128 ) == 128 ? 1 : 0;
						if( shift == 0 )
							shift = (  ( HIWORD( GetKeyState( VK_RSHIFT ) ) ) & 128 ) == 128 ? 1 : 0;
						rux::uint8 control = ( ( HIWORD( GetKeyState( VK_LCONTROL ) ) ) & 128 ) == 128 ? 1 : 0;
						if( control == 0 )
							control = ( ( HIWORD( GetKeyState( VK_RCONTROL ) ) ) & 128 ) == 128 ? 1 : 0;
						rux::uint8 alt = ( ( HIWORD( GetKeyState( VK_LMENU ) ) ) & 128 ) == 128 ? 1 : 0;
						if( alt == 0 )
							alt = ( ( HIWORD( GetKeyState( VK_RMENU ) ) ) & 128 ) == 128 ? 1 : 0;
						rux::uint8 left_mouse_button_pressed = wparam & MK_LBUTTON;
						rux::uint8 middle_mouse_button_pressed = wparam & MK_MBUTTON;
						rux::uint8 right_mouse_button_pressed = wparam & MK_RBUTTON;
						rux::uint8 x_button1_pressed = wparam & MK_XBUTTON1;
						rux::uint8 x_button2_pressed = wparam & MK_XBUTTON2;
						rux::int16 x = LOWORD( lparam );
						rux::int16 y = HIWORD( lparam );
						rux::gui::WindowMouseEvent xevent(::rux::gui::XEnum_EventType_WindowMouseWheelUp, window, 0, x, y
							, alt, control, shift, left_mouse_button_pressed, middle_mouse_button_pressed
							, right_mouse_button_pressed, x_button1_pressed, x_button2_pressed);
						window->raise_event( xevent );
						if( window && need_release )
							window->Release();
						return 0;
					}
				case WM_MBUTTONDOWN:
					{
						rux::uint8 shift = ( ( HIWORD( GetKeyState( VK_LSHIFT ) ) ) & 128 ) == 128 ? 1 : 0;
						if( shift == 0 )
							shift = (  ( HIWORD( GetKeyState( VK_RSHIFT ) ) ) & 128 ) == 128 ? 1 : 0;
						rux::uint8 control = ( ( HIWORD( GetKeyState( VK_LCONTROL ) ) ) & 128 ) == 128 ? 1 : 0;
						if( control == 0 )
							control = ( ( HIWORD( GetKeyState( VK_RCONTROL ) ) ) & 128 ) == 128 ? 1 : 0;
						rux::uint8 alt = ( ( HIWORD( GetKeyState( VK_LMENU ) ) ) & 128 ) == 128 ? 1 : 0;
						if( alt == 0 )
							alt = ( ( HIWORD( GetKeyState( VK_RMENU ) ) ) & 128 ) == 128 ? 1 : 0;
						rux::uint8 left_mouse_button_pressed = wparam & MK_LBUTTON;
						rux::uint8 middle_mouse_button_pressed = wparam & MK_MBUTTON;
						rux::uint8 right_mouse_button_pressed = wparam & MK_RBUTTON;
						rux::uint8 x_button1_pressed = wparam & MK_XBUTTON1;
						rux::uint8 x_button2_pressed = wparam & MK_XBUTTON2;
						rux::int16 x = LOWORD( lparam );
						rux::int16 y = HIWORD( lparam );
						rux::gui::WindowMouseEvent xevent(::rux::gui::XEnum_EventType_WindowMouseWheelDown, window, 0, x, y
							, alt, control, shift, left_mouse_button_pressed, middle_mouse_button_pressed
							, right_mouse_button_pressed, x_button1_pressed, x_button2_pressed);
						window->raise_event( xevent );
						if( window && need_release )
							window->Release();
						return 0;
					}
				case WM_MOUSEWHEEL:
					{
						rux::uint8 shift = ( ( HIWORD( GetKeyState( VK_LSHIFT ) ) ) & 128 ) == 128 ? 1 : 0;
						if( shift == 0 )
							shift = (  ( HIWORD( GetKeyState( VK_RSHIFT ) ) ) & 128 ) == 128 ? 1 : 0;
						rux::uint8 control = ( ( HIWORD( GetKeyState( VK_LCONTROL ) ) ) & 128 ) == 128 ? 1 : 0;
						if( control == 0 )
							control = ( ( HIWORD( GetKeyState( VK_RCONTROL ) ) ) & 128 ) == 128 ? 1 : 0;
						rux::uint8 alt = ( ( HIWORD( GetKeyState( VK_LMENU ) ) ) & 128 ) == 128 ? 1 : 0;
						if( alt == 0 )
							alt = ( ( HIWORD( GetKeyState( VK_RMENU ) ) ) & 128 ) == 128 ? 1 : 0;
						rux::int16 delta = GET_WHEEL_DELTA_WPARAM( wparam );
						WPARAM keys = GET_KEYSTATE_WPARAM( wparam );
						rux::uint8 left_mouse_button_pressed = keys & MK_LBUTTON;
						rux::uint8 middle_mouse_button_pressed = keys & MK_MBUTTON;
						rux::uint8 right_mouse_button_pressed = keys & MK_RBUTTON;
						rux::uint8 x_button1_pressed = keys & MK_XBUTTON1;
						rux::uint8 x_button2_pressed = keys & MK_XBUTTON2;

						rux::int16 x = LOWORD( lparam );
						rux::int16 y = HIWORD( lparam );
						if( window->_left <= x
							&& window->_left + window->_width >= x
							&& window->_top <= y
							&& window->_top + window->_height >= y )
						{
							rux::gui::WindowMouseEvent xevent( ::rux::gui::XEnum_EventType_WindowMouseWheel , window , delta , x - window->_left , y - window->_top , alt , control , shift , 
								left_mouse_button_pressed , middle_mouse_button_pressed , 
								right_mouse_button_pressed , x_button1_pressed , x_button2_pressed );
							window->raise_event( xevent );
						}
						if( window && need_release )
							window->Release();
						return 0;
					}
				case WM_MOUSEMOVE:
					{								
						rux::uint8 shift = ( ( HIWORD( GetKeyState( VK_LSHIFT ) ) ) & 128 ) == 128 ? 1 : 0;
						if( shift == 0 )
							shift = (  ( HIWORD( GetKeyState( VK_RSHIFT ) ) ) & 128 ) == 128 ? 1 : 0;
						rux::uint8 control = ( ( HIWORD( GetKeyState( VK_LCONTROL ) ) ) & 128 ) == 128 ? 1 : 0;
						if( control == 0 )
							control = ( ( HIWORD( GetKeyState( VK_RCONTROL ) ) ) & 128 ) == 128 ? 1 : 0;
						rux::uint8 alt = ( ( HIWORD( GetKeyState( VK_LMENU ) ) ) & 128 ) == 128 ? 1 : 0;
						if( alt == 0 )
							alt = ( ( HIWORD( GetKeyState( VK_RMENU ) ) ) & 128 ) == 128 ? 1 : 0;
						rux::uint8 left_mouse_button_pressed = wparam & MK_LBUTTON;
						rux::uint8 middle_mouse_button_pressed = wparam & MK_MBUTTON;
						rux::uint8 right_mouse_button_pressed = wparam & MK_RBUTTON;
						rux::uint8 x_button1_pressed = wparam & MK_XBUTTON1;
						rux::uint8 x_button2_pressed = wparam & MK_XBUTTON2;
						rux::int16 x = LOWORD( lparam );
						rux::int16 y = HIWORD( lparam );
						rux::gui::WindowMouseEvent xevent( ::rux::gui::XEnum_EventType_WindowMouseMove , window , 0 , x , y , alt , control , shift , 
							left_mouse_button_pressed , middle_mouse_button_pressed , 
							right_mouse_button_pressed , x_button1_pressed , x_button2_pressed );
						window->raise_event( xevent );
						if( window && need_release )
							window->Release();
						return 0;
					}
				case WM_LBUTTONDOWN:
					{	
						rux::uint8 shift = ( ( HIWORD( GetKeyState( VK_LSHIFT ) ) ) & 128 ) == 128 ? 1 : 0;
						if( shift == 0 )
							shift = (  ( HIWORD( GetKeyState( VK_RSHIFT ) ) ) & 128 ) == 128 ? 1 : 0;
						rux::uint8 control = ( ( HIWORD( GetKeyState( VK_LCONTROL ) ) ) & 128 ) == 128 ? 1 : 0;
						if( control == 0 )
							control = ( ( HIWORD( GetKeyState( VK_RCONTROL ) ) ) & 128 ) == 128 ? 1 : 0;
						rux::uint8 alt = ( ( HIWORD( GetKeyState( VK_LMENU ) ) ) & 128 ) == 128 ? 1 : 0;
						if( alt == 0 )
							alt = ( ( HIWORD( GetKeyState( VK_RMENU ) ) ) & 128 ) == 128 ? 1 : 0;
						//rux::uint8 left_mouse_button_pressed = wparam & MK_LBUTTON;
						rux::uint8 middle_mouse_button_pressed = wparam & MK_MBUTTON;
						rux::uint8 right_mouse_button_pressed = wparam & MK_RBUTTON;
						rux::uint8 x_button1_pressed = wparam & MK_XBUTTON1;
						rux::uint8 x_button2_pressed = wparam & MK_XBUTTON2;
						rux::int16 x = LOWORD( lparam );
						rux::int16 y = HIWORD( lparam );
						rux::gui::WindowMouseEvent xevent( ::rux::gui::XEnum_EventType_WindowLeftMouseButtonDown , window , 0 , x , y , alt , control , shift , 
							1 , middle_mouse_button_pressed , 
							right_mouse_button_pressed , x_button1_pressed , x_button2_pressed );
						window->CaptureMouse();
						window->raise_event( xevent );
						if( window && need_release )
							window->Release();
						return 0;
					}
				case WM_LBUTTONUP:
					{	
						rux::uint8 shift = ( ( HIWORD( GetKeyState( VK_LSHIFT ) ) ) & 128 ) == 128 ? 1 : 0;
						if( shift == 0 )
							shift = (  ( HIWORD( GetKeyState( VK_RSHIFT ) ) ) & 128 ) == 128 ? 1 : 0;
						rux::uint8 control = ( ( HIWORD( GetKeyState( VK_LCONTROL ) ) ) & 128 ) == 128 ? 1 : 0;
						if( control == 0 )
							control = ( ( HIWORD( GetKeyState( VK_RCONTROL ) ) ) & 128 ) == 128 ? 1 : 0;
						rux::uint8 alt = ( ( HIWORD( GetKeyState( VK_LMENU ) ) ) & 128 ) == 128 ? 1 : 0;
						if( alt == 0 )
							alt = ( ( HIWORD( GetKeyState( VK_RMENU ) ) ) & 128 ) == 128 ? 1 : 0;
						rux::uint8 left_mouse_button_pressed = wparam & MK_LBUTTON;
						rux::uint8 middle_mouse_button_pressed = wparam & MK_MBUTTON;
						rux::uint8 right_mouse_button_pressed = wparam & MK_RBUTTON;
						rux::uint8 x_button1_pressed = wparam & MK_XBUTTON1;
						rux::uint8 x_button2_pressed = wparam & MK_XBUTTON2;
						rux::int16 x = LOWORD( lparam );
						rux::int16 y = HIWORD( lparam );
						rux::gui::WindowMouseEvent xevent( ::rux::gui::XEnum_EventType_WindowLeftMouseButtonUp , window , 0 , x , y , alt , control , shift , 
							left_mouse_button_pressed , middle_mouse_button_pressed , 
							right_mouse_button_pressed , x_button1_pressed , x_button2_pressed );
						window->ReleaseMouse();
						window->raise_event( xevent );
						if( window && need_release )
							window->Release();
						return 0;
					}
				case WM_LBUTTONDBLCLK:
					{	
						rux::uint8 shift = ( ( HIWORD( GetKeyState( VK_LSHIFT ) ) ) & 128 ) == 128 ? 1 : 0;
						if( shift == 0 )
							shift = (  ( HIWORD( GetKeyState( VK_RSHIFT ) ) ) & 128 ) == 128 ? 1 : 0;
						rux::uint8 control = ( ( HIWORD( GetKeyState( VK_LCONTROL ) ) ) & 128 ) == 128 ? 1 : 0;
						if( control == 0 )
							control = ( ( HIWORD( GetKeyState( VK_RCONTROL ) ) ) & 128 ) == 128 ? 1 : 0;
						rux::uint8 alt = ( ( HIWORD( GetKeyState( VK_LMENU ) ) ) & 128 ) == 128 ? 1 : 0;
						if( alt == 0 )
							alt = ( ( HIWORD( GetKeyState( VK_RMENU ) ) ) & 128 ) == 128 ? 1 : 0;
						rux::uint8 left_mouse_button_pressed = wparam & MK_LBUTTON;
						rux::uint8 middle_mouse_button_pressed = wparam & MK_MBUTTON;
						rux::uint8 right_mouse_button_pressed = wparam & MK_RBUTTON;
						rux::uint8 x_button1_pressed = wparam & MK_XBUTTON1;
						rux::uint8 x_button2_pressed = wparam & MK_XBUTTON2;
						rux::int16 x = LOWORD( lparam );
						rux::int16 y = HIWORD( lparam );
						rux::gui::WindowMouseEvent xevent( ::rux::gui::XEnum_EventType_WindowLeftMouseButtonDoubleClick , window , 0 , x , y , alt , control , shift , 
							left_mouse_button_pressed , middle_mouse_button_pressed , 
							right_mouse_button_pressed , x_button1_pressed , x_button2_pressed );
						window->raise_event( xevent );
						if( window && need_release )
							window->Release();
						return 0;
					}
				case WM_RBUTTONDOWN:
					{	
						rux::uint8 shift = ( ( HIWORD( GetKeyState( VK_LSHIFT ) ) ) & 128 ) == 128 ? 1 : 0;
						if( shift == 0 )
							shift = (  ( HIWORD( GetKeyState( VK_RSHIFT ) ) ) & 128 ) == 128 ? 1 : 0;
						rux::uint8 control = ( ( HIWORD( GetKeyState( VK_LCONTROL ) ) ) & 128 ) == 128 ? 1 : 0;
						if( control == 0 )
							control = ( ( HIWORD( GetKeyState( VK_RCONTROL ) ) ) & 128 ) == 128 ? 1 : 0;
						rux::uint8 alt = ( ( HIWORD( GetKeyState( VK_LMENU ) ) ) & 128 ) == 128 ? 1 : 0;
						if( alt == 0 )
							alt = ( ( HIWORD( GetKeyState( VK_RMENU ) ) ) & 128 ) == 128 ? 1 : 0;
						rux::uint8 left_mouse_button_pressed = wparam & MK_LBUTTON;
						rux::uint8 middle_mouse_button_pressed = wparam & MK_MBUTTON;
						//rux::uint8 right_mouse_button_pressed = wparam & MK_RBUTTON;
						rux::uint8 x_button1_pressed = wparam & MK_XBUTTON1;
						rux::uint8 x_button2_pressed = wparam & MK_XBUTTON2;
						rux::int16 x = LOWORD( lparam );
						rux::int16 y = HIWORD( lparam );
						rux::gui::WindowMouseEvent xevent( ::rux::gui::XEnum_EventType_WindowRightMouseButtonDown , window , 0 , x , y , alt , control , shift , 
							left_mouse_button_pressed , middle_mouse_button_pressed , 
							1 , x_button1_pressed , x_button2_pressed );
						window->CaptureMouse();
						window->raise_event( xevent );
						if( window && need_release )
							window->Release();
						return 0;
					}
				case WM_RBUTTONUP:
					{	
						rux::uint8 shift = ( ( HIWORD( GetKeyState( VK_LSHIFT ) ) ) & 128 ) == 128 ? 1 : 0;
						if( shift == 0 )
							shift = (  ( HIWORD( GetKeyState( VK_RSHIFT ) ) ) & 128 ) == 128 ? 1 : 0;
						rux::uint8 control = ( ( HIWORD( GetKeyState( VK_LCONTROL ) ) ) & 128 ) == 128 ? 1 : 0;
						if( control == 0 )
							control = ( ( HIWORD( GetKeyState( VK_RCONTROL ) ) ) & 128 ) == 128 ? 1 : 0;
						rux::uint8 alt = ( ( HIWORD( GetKeyState( VK_LMENU ) ) ) & 128 ) == 128 ? 1 : 0;
						if( alt == 0 )
							alt = ( ( HIWORD( GetKeyState( VK_RMENU ) ) ) & 128 ) == 128 ? 1 : 0;
						rux::uint8 left_mouse_button_pressed = wparam & MK_LBUTTON;
						rux::uint8 middle_mouse_button_pressed = wparam & MK_MBUTTON;
						rux::uint8 right_mouse_button_pressed = wparam & MK_RBUTTON;
						rux::uint8 x_button1_pressed = wparam & MK_XBUTTON1;
						rux::uint8 x_button2_pressed = wparam & MK_XBUTTON2;
						rux::int16 x = LOWORD( lparam );
						rux::int16 y = HIWORD( lparam );
						rux::gui::WindowMouseEvent xevent( ::rux::gui::XEnum_EventType_WindowRightMouseButtonUp , window , 0 , x , y , alt , control , shift , 
							left_mouse_button_pressed , middle_mouse_button_pressed , 
							right_mouse_button_pressed , x_button1_pressed , x_button2_pressed );
						window->ReleaseMouse();
						window->raise_event( xevent );
						if( window && need_release )
							window->Release();
						return 0;
					}
				case WM_RBUTTONDBLCLK:
					{	
						rux::uint8 shift = ( ( HIWORD( GetKeyState( VK_LSHIFT ) ) ) & 128 ) == 128 ? 1 : 0;
						if( shift == 0 )
							shift = (  ( HIWORD( GetKeyState( VK_RSHIFT ) ) ) & 128 ) == 128 ? 1 : 0;
						rux::uint8 control = ( ( HIWORD( GetKeyState( VK_LCONTROL ) ) ) & 128 ) == 128 ? 1 : 0;
						if( control == 0 )
							control = ( ( HIWORD( GetKeyState( VK_RCONTROL ) ) ) & 128 ) == 128 ? 1 : 0;
						rux::uint8 alt = ( ( HIWORD( GetKeyState( VK_LMENU ) ) ) & 128 ) == 128 ? 1 : 0;
						if( alt == 0 )
							alt = ( ( HIWORD( GetKeyState( VK_RMENU ) ) ) & 128 ) == 128 ? 1 : 0;
						rux::uint8 left_mouse_button_pressed = wparam & MK_LBUTTON;
						rux::uint8 middle_mouse_button_pressed = wparam & MK_MBUTTON;
						rux::uint8 right_mouse_button_pressed = wparam & MK_RBUTTON;
						rux::uint8 x_button1_pressed = wparam & MK_XBUTTON1;
						rux::uint8 x_button2_pressed = wparam & MK_XBUTTON2;
						rux::int16 x = LOWORD( lparam );
						rux::int16 y = HIWORD( lparam );
						rux::gui::WindowMouseEvent xevent( ::rux::gui::XEnum_EventType_WindowRightMouseButtonDoubleClick , window , 0 , x , y , alt , control , shift , 
							left_mouse_button_pressed , middle_mouse_button_pressed , 
							right_mouse_button_pressed , x_button1_pressed , x_button2_pressed );
						window->raise_event( xevent );
						if( window && need_release )
							window->Release();
						return 0;
					}
				case WM_SETCURSOR:
					{
						if( window )
						{
							window->set_Cursor( window->_cursor );
							if( need_release )
								window->Release();
						}
						return 0;
					}
				default:
					{
						break;
					}
				}	
				if( window && need_release )
					window->Release();
				return DefWindowProc( hwnd, msg, wparam, lparam );
			};
#elif defined( __UNIX__ )
#ifdef __ANDROID__
#else
			dll_internal ::rux::int32 display_io_error_handler( Display* display )
			{
				printf( "%s, %s:%d\n" , __FUNCTION__ , __FILE__ , __LINE__ );
				::rux::log::WriteError( "%s, %s:%d" , __FUNCTION__ , __FILE__ , __LINE__ );
				return 0;
			};
			dll_internal ::rux::int32 display_error_handler( Display* display , XErrorEvent* error_event )
			{
				char error_string[ 256 ] = {0};
				::rux::engine::_globals->_x11_module.XGetErrorText( error_event->display , error_event->error_code , error_string , 256 );
				printf( "%s, %s:%d, error '%s'\n" , __FUNCTION__ , __FILE__ , __LINE__ , error_string );
				::rux::log::WriteError( "%s, %s:%d, error '%s'" , __FUNCTION__ , __FILE__ , __LINE__ , error_string );
				return 0;
			};
			dll_internal bool lock_display( ::rux::gui::WindowBase* window_base )
			{				
				rux::gui::engine::Window* window = (rux::gui::engine::Window*)window_base;
				bool res = false;
				while( ( res = ::rux::gui::engine::_cs_x_display_main->TryLock() ) == false )
				{
					if( window && window->_is_closing == 1 )
						break;
					rux::threading::XThread::Sleep( 1 );
				}
				return res;
			};
			dll_internal void unlock_display( void )
			{
				rux::gui::engine::_cs_x_display_main->UnLock();
			};
			dll_internal ::rux::uint8 is_allowed_atom( Display* display , ::Window xwindow , Atom atom )
			{				
				Atom type = None;
				int form = 0;
				unsigned long remain = 0;
				unsigned long len = 0;
				Atom* list = NULL;
				rux::uint8 is_allowed = 0;
				if( ::rux::engine::_globals->_x11_module.XGetWindowProperty( display , xwindow , ::rux::gui::engine::_net_wm_allowed_actions , 0 , 1024 , False , XA_ATOM , &type , &form , &len , &remain , (rux::uint8**)&list ) == Success )
				{
					//printf( "list = '%s'\n" , "Success" );
					for( size_t index0 = 0 ; index0 < len ; ++index0 )
					{
						//printf( "list = '%s'\n" , XGetAtomName( display , list[ index0 ] ) );
						if( list[ index0 ] == atom )
						{
							is_allowed = 1;
							break;
						}
					}
					::rux::engine::_globals->_x11_module.XFree( list );
				}
				return is_allowed;
			};
			dll_internal ::rux::gui::engine::Window* find_window( ::Window& xwindow )
			{
				rux::gui::engine::Window* window = NULL;
				CS_PTR_LOCK( ::rux::gui::engine::_cs_windows );
				for( size_t index0 = 0 ; index0 < ::rux::gui::engine::_windows.Count() ; index0++ )
				{
					window = ::rux::gui::engine::_windows[ index0 ];
					if( window && window->_x_window == xwindow )
					{
						window->AddRef();
						break;
					}
					else
						window = NULL;
				}
				CS_PTR_UNLOCK( ::rux::gui::engine::_cs_windows );
				return window;
			};
			dll_internal void WndProc( XEvent& event )
			{
				rux::gui::engine::Window* window = NULL;				
				switch( event.type )
				{
				case ClientMessage:
					{
						window = ::rux::gui::engine::find_window( event.xclient.window );
						if( window )
						{
							//printf( "ClientMessage\n" );
							if( event.xclient.message_type == ::rux::gui::engine::_net_close_window )
							{
								//printf( "_net_close_window\n" );
								if( window->_windows_index != 0xffffffff )
								{
									window->_is_closing = 1;
									if( window->_render_context )
										window->_render_context->DestroyRenderThread();
									CS_PTR_LOCK( ::rux::gui::engine::_cs_windows );
									rux::gui::engine::_windows.set_EmptyAt( window->_windows_index );
									window->_windows_index = 0xffffffff;									
									CS_PTR_UNLOCK( ::rux::gui::engine::_cs_windows );
									rux::gui::WindowClosedEvent xevent( window );
									window->raise_event( xevent );		
									window->free_resources();
									window->Release();									
								}
							}							
							else if( event.xclient.message_type == ::rux::gui::engine::_rux_gui_initialize )
							{		
								//printf( "_rux_gui_initialize\n" );
							}
							else if( event.xclient.message_type == ::rux::gui::engine::_rux_delete_window )
							{		
								//printf( "RUX_DELETE_WINDOW\n" );
								if( window->_windows_index != 0xffffffff )
								{
									window->_is_closing = 1;
									if( window->_render_context )
										window->_render_context->DestroyRenderThread();
									CS_PTR_LOCK( ::rux::gui::engine::_cs_windows );
									rux::gui::engine::_windows.set_EmptyAt( window->_windows_index );
									window->_windows_index = 0xffffffff;
									CS_PTR_UNLOCK( ::rux::gui::engine::_cs_windows );
									rux::gui::WindowClosedEvent xevent( window );
									window->raise_event( xevent );
									window->free_resources();
									window->Release();									
								}							
							}
							else if( event.xclient.message_type == ::rux::gui::engine::_wm_protocols
								&& event.xclient.data.l[ 0 ] == ::rux::gui::engine::_wm_delete_window )
							{
								//printf( "wm_delete_window\n" );
								if( window->_windows_index != 0xffffffff )
								{
									window->_is_closing = 1;
									if( window->_render_context )
										window->_render_context->DestroyRenderThread();
									CS_PTR_LOCK( ::rux::gui::engine::_cs_windows );
									rux::gui::engine::_windows.set_EmptyAt( window->_windows_index );
									window->_windows_index = 0xffffffff;
									CS_PTR_UNLOCK( ::rux::gui::engine::_cs_windows );
									rux::gui::WindowClosedEvent xevent( window );
									window->raise_event( xevent );
									window->free_resources();
									window->Release();
								}		
							}
							else 
							{
								if( rux::gui::engine::_is_x_if_event == 0 )
								{
									char* atom_name = ::rux::engine::_globals->_x11_module.XGetAtomName( ::rux::gui::engine::_x_display_main , event.xclient.message_type );
									printf( "Atom name = '%s'\n" , atom_name );
									::rux::engine::_globals->_x11_module.XFree( atom_name );	
								}
							}
							window->Release();
						}
						else
						{
							if( rux::gui::engine::_is_x_if_event == 0 )
							{
								char* atom_name = ::rux::engine::_globals->_x11_module.XGetAtomName( ::rux::gui::engine::_x_display_main , event.xclient.message_type );
								printf( "%s, atom name '%s'\n" , "ClientMessage" , atom_name );
								::rux::engine::_globals->_x11_module.XFree( atom_name );
							}
						}
						break;
					}
					case CreateNotify:
					{		
						window = ::rux::gui::engine::find_window( event.xcreatewindow.window );
						if( window )		
						{
							::rux::log::WriteTrace( "CreateNotify, %p" , window );
							rux::gui::WindowLoadedEvent xevent( window );
							window->raise_event( xevent );
							//printf( "%s\n" , "CreateNotify" );
							window->Release();
						}
						break;
					}
					case DestroyNotify:
						{
							window = ::rux::gui::engine::find_window( event.xdestroywindow.window );
							if( window )
							{
								//printf( "%s\n" , "DestroyNotify" );
								window->Release();
							}
							break;
						}
					case NoExpose :
						{
							//NO WINDOW
							//printf( "%s\n" , "NoExpose" );
							break;
						}
					case GraphicsExpose:
						{
							//NO WINDOW
							//printf( "%s\n" , "GraphicsExpose" );
							break;
						}
					case FocusOut:
						{
							window = ::rux::gui::engine::find_window( event.xfocus.window );
							if( window )
							{								
								//printf( "%s\n" , "FocusOut" );
								window->Release();
							}	
							break;
						}
					case FocusIn:
						{
							window = ::rux::gui::engine::find_window( event.xfocus.window );
							if( window )
							{								
								//printf( "%s\n" , "FocusIn" );
								window->Release();
							}	
							break;
						}
					case Expose:
					{	
						window = ::rux::gui::engine::find_window( event.xexpose.window );
						if( window )
						{		
							if ( event.xexpose.count != 0 )
								break;					
							if( event.xexpose.send_event == false )
							{	
							}		
							window->Release();
						}			
						break; // Redraw event
					}
					case MappingNotify:
						{
							window = ::rux::gui::engine::find_window( event.xmapping.window );
							if( window )
							{
								//printf( "%s\n" , "MappingNotify" );
								window->Release();
							}	
							break;
						}
					case MapRequest:
						{
							window = ::rux::gui::engine::find_window( event.xmaprequest.window );
							if( window )
							{								
								//printf( "%s\n" , "MapRequest" );
								window->Release();
							}						
							break;
						}
					case UnmapNotify:
					{	
						window = ::rux::gui::engine::find_window( event.xunmap.window );
						if( window )
						{
							::rux::log::WriteTrace( "UnmapNotify, %p" , window );
							window->_is_visible = 0;
							rux::gui::WindowHidedEvent xevent( window );
							window->raise_event( xevent );
							//printf( "%s\n" , "UnmapNotify" );
							window->Release();
						}						
						break;
					}
					case MapNotify:
					{	
						window = ::rux::gui::engine::find_window( event.xmap.window );
						if( window )
						{	
							::rux::log::WriteTrace( "MapNotify, %p" , window );
							window->_is_visible = 1;
							rux::gui::WindowShownEvent xevent( window );
							window->raise_event( xevent );
							//printf( "%s\n" , "MapNotify" );
							window->Release();
						}
						//else 
						//	printf( "cannot find window %s\n" , "MapNotify" );
						break;
					}
					case GravityNotify:
					{		
						window = ::rux::gui::engine::find_window( event.xgravity.window );
						if( window )
						{
							//printf( "%s\n" , "GravityNotify" );
							window->Release();
						}
						break;
					}
					case CirculateNotify:
						{
							window = ::rux::gui::engine::find_window( event.xcirculate.window );
							if( window )
							{
								//printf( "%s\n" , "CirculateNotify" );
								window->Release();
							}
							break;
						}
					case CirculateRequest:
						{
							window = ::rux::gui::engine::find_window( event.xcirculaterequest.window );
							if( window )
							{
								//printf( "%s\n" , "CirculateRequest" );
								window->Release();
							}
							break;
						}
					case ColormapNotify:
						{
							window = ::rux::gui::engine::find_window( event.xcolormap.window );
							if( window )
							{
								//printf( "%s\n" , "ColormapNotify" );
								window->Release();
							}
							break;
						}
					case VisibilityNotify:
						{
							window = ::rux::gui::engine::find_window( event.xvisibility.window );
							if( window )
							{
								//printf( "%s\n" , "VisibilityNotify" );
								window->Release();
							}
							//else
							//	printf( "cannot find window %s\n" , "VisibilityNotify" );
							break;
						}
					case SelectionRequest:
						{
							//NO WINDOW
							//printf( "%s\n" , "SelectionRequest" );
							break;
						}
					case SelectionNotify:
						{
							//NO WINDOW
							//printf( "%s\n" , "SelectionNotify" );
							break;
						}
					case SelectionClear:
						{
							window = ::rux::gui::engine::find_window( event.xselectionclear.window );
							if( window )
							{
								//printf( "%s\n" , "SelectionClear" );
								window->Release();
							}
							break;
						}
					case ResizeRequest:
						{
							window = ::rux::gui::engine::find_window( event.xresizerequest.window );
							if( window )
							{
								//printf( "%s\n" , "ResizeRequest" );
								window->Release();
							}
							break;
						}
					case ReparentNotify:
						{
							window = ::rux::gui::engine::find_window( event.xreparent.window );
							if( window )
							{
								//printf( "%s\n" , "ReparentNotify" );
								window->Release();
							}
							break;
						}
					case PropertyNotify:
						{
							window = ::rux::gui::engine::find_window( event.xproperty.window );
							if( window )
							{
								if( rux::gui::engine::_is_x_if_event == 0 )
								{									
									if( event.xproperty.atom == ::rux::gui::engine::_net_wm_state )
									{
										Atom type = None;
										::rux::int32 format = 0;
										unsigned long item_count = 0 , bytes_after = 0;
										Atom* atoms = NULL;
										if( ::rux::engine::_globals->_x11_module.XGetWindowProperty( ::rux::gui::engine::_x_display_main , event.xproperty.window , ::rux::gui::engine::_net_wm_state , 0 , 1024 , False , XA_ATOM , &type , &format , &item_count , &bytes_after , (::rux::byte**)&atoms ) == Success )
										{
											::rux::byte net_wm_state_max_horz = 0 , net_wm_state_hidden = 0 , net_wm_state_fullscreen = 0 , net_wm_state_max_vert = 0;
											for( size_t item_index = 0 ; item_index < item_count ; ++item_index )
											{
												if( atoms[ item_index ] == ::rux::gui::engine::_net_wm_state_max_horz )
													net_wm_state_max_horz = 1;
												else if( atoms[ item_index ] == ::rux::gui::engine::_net_wm_state_hidden )
													net_wm_state_hidden = 1;
												else if( atoms[ item_index ] == ::rux::gui::engine::_net_wm_state_fullscreen )
													net_wm_state_fullscreen = 1;
												else if( atoms[ item_index ] == ::rux::gui::engine::_net_wm_state_max_vert )
													net_wm_state_max_vert = 1;
											}
											::rux::engine::_globals->_x11_module.XFree( atoms );
											if( ( net_wm_state_max_horz && net_wm_state_max_vert ) || net_wm_state_fullscreen )
											{
												if( window->_window_state != ::rux::gui::XEnum_WindowState_Maximized )
												{
													window->_prev_window_state = window->_window_state;
													window->_window_state = ::rux::gui::XEnum_WindowState_Maximized;
													rux::gui::WindowMaximizedStateEvent xevent( window );
													window->raise_event( xevent );

													//printf( "%s\n" , "WindowMaximizedStateEvent" );
												}
											}
											else if( net_wm_state_hidden )
											{
												if( window->_window_state != ::rux::gui::XEnum_WindowState_Minimized )
												{
													window->_prev_window_state = window->_window_state;
													window->_window_state = ::rux::gui::XEnum_WindowState_Minimized;
													rux::gui::WindowMinimizedStateEvent xevent( window );
													window->raise_event( xevent );

													//printf( "%s\n" , "WindowMinimizedStateEvent" );
												}
											}
											else
											{
												if( window->_window_state != ::rux::gui::XEnum_WindowState_Normal )
												{
													window->_prev_window_state = window->_window_state;
													window->_window_state = ::rux::gui::XEnum_WindowState_Normal;
													rux::gui::WindowNormalStateEvent xevent( window );
													window->raise_event( xevent );

													//printf( "%s\n" , "XEnum_WindowState_Normal" );
												}												
											}
										}
									}
									else
									{
										if( rux::gui::engine::_is_x_if_event == 0 )
										{
											char* atom_name = ::rux::engine::_globals->_x11_module.XGetAtomName( ::rux::gui::engine::_x_display_main , event.xproperty.atom );
											printf( "%s, atom name '%s'\n" , "PropertyNotify" , atom_name );
											::rux::engine::_globals->_x11_module.XFree( atom_name );
										}
									}
								}
								//else
								//	printf( "%s\n" , "PropertyNotify" );
								window->Release();
							}
							//else
							//	printf( "cannot find window %s\n" , "PropertyNotify" );
							break;
						}
					case ConfigureRequest:
						{
							window = ::rux::gui::engine::find_window( event.xconfigurerequest.window );
							if( window )
							{
								//printf( "%s\n" , "ConfigureRequest" );
								window->Release();
							}
							break;
						}
					case ConfigureNotify:
					{		
						window = ::rux::gui::engine::find_window( event.xconfigure.window );
						if( window )
						{
							::rux::log::WriteTrace( "ConfigureNotify, %p, x=%d, y=%d, width=%d, height=%d" , window , event.xconfigure.x , event.xconfigure.y , event.xconfigure.width , event.xconfigure.height );
							if( window->_width != event.xconfigure.width
								|| 	window->_height != event.xconfigure.height )
							{
								window->_width = event.xconfigure.width;
								window->_height = event.xconfigure.height;
								if( window->_render_context )
									window->_render_context->Resize( window->_width , window->_height );
								rux::gui::WindowSizeEvent xevent( window , window->_width , window->_height );
								window->raise_event( xevent );
							}
							if( window->_left != event.xconfigure.x
								||	window->_top != event.xconfigure.y )
							{
								window->_left = event.xconfigure.x;
								window->_top = event.xconfigure.y;
								rux::gui::WindowMoveEvent xevent( window );
								window->raise_event( xevent );
							}
							//printf( "%s\n" , "ConfigureNotify" );
							window->Release();
						}
						//else
							//printf( "cannot find window %s\n" , "ConfigureNotify" );
						break; // Window resize event
					}
					case MotionNotify:
					{		
						window = ::rux::gui::engine::find_window( event.xmotion.window );
						if( window )		
						{							
							rux::uint8 control = event.xmotion.state & ControlMask ? 1 : 0;
							rux::uint8 left_mouse_button_pressed = event.xmotion.state & Button1Mask ? 1 : 0;
							rux::uint8 middle_mouse_button_pressed = event.xmotion.state & Button2Mask ? 1 : 0;
							rux::uint8 right_mouse_button_pressed = event.xmotion.state & Button3Mask ? 1 : 0;
							rux::uint8 shift = event.xmotion.state & ShiftMask ? 1 : 0;
							rux::uint8 alt = event.xmotion.state & LockMask ? 1 : 0;							
							rux::uint8 x_button1_pressed = event.xmotion.state & Button4Mask ? 1 : 0;
							rux::uint8 x_button2_pressed = event.xmotion.state & Button5Mask ? 1 : 0;						
							rux::gui::WindowMouseEvent xevent( ::rux::gui::XEnum_EventType_WindowMouseMove , window , 0 , event.xmotion.x , event.xmotion.y , alt , control , shift , 
								left_mouse_button_pressed , middle_mouse_button_pressed , 
								right_mouse_button_pressed , x_button1_pressed , x_button2_pressed );
							window->raise_event( xevent );
							//printf( "%s\n" , "MotionNotify" );
							window->Release();
						}
						break;
					}
					case ButtonPress:
					{
						window = ::rux::gui::engine::find_window( event.xbutton.window );
						if( window )
						{							
							rux::uint8 control = event.xbutton.state & ControlMask ? 1 : 0;
							rux::uint8 left_mouse_button_pressed = event.xbutton.state & Button1Mask ? 1 : 0;
							rux::uint8 middle_mouse_button_pressed = event.xbutton.state & Button2Mask ? 1 : 0;
							rux::uint8 right_mouse_button_pressed = event.xbutton.state & Button3Mask ? 1 : 0;
							rux::uint8 shift = event.xbutton.state & ShiftMask ? 1 : 0;
							rux::uint8 alt = event.xbutton.state & LockMask ? 1 : 0;							
							rux::uint8 x_button1_pressed = event.xbutton.state & Button4Mask ? 1 : 0;
							rux::uint8 x_button2_pressed = event.xbutton.state & Button5Mask ? 1 : 0;						
							if( event.xbutton.button == Button1 )
							{
								left_mouse_button_pressed = 1;
								if( event.xbutton.time - ::rux::gui::engine::_mouse_down_time < 250 )
								{
									rux::gui::WindowMouseEvent xevent( ::rux::gui::XEnum_EventType_WindowLeftMouseButtonDoubleClick , window , 0 , event.xbutton.x , event.xbutton.y , alt , control , shift , 
										left_mouse_button_pressed , middle_mouse_button_pressed , 
										right_mouse_button_pressed , x_button1_pressed , x_button2_pressed );
									window->raise_event( xevent );
									rux::gui::engine::_is_mouse_down = 0;
								}
								else
								{
									rux::gui::WindowMouseEvent xevent( ::rux::gui::XEnum_EventType_WindowLeftMouseButtonDown , window , 0 , event.xbutton.x , event.xbutton.y , alt , control , shift , 
										1 , middle_mouse_button_pressed , 
										right_mouse_button_pressed , x_button1_pressed , x_button2_pressed );
									window->CaptureMouse();
									window->raise_event( xevent );
									rux::gui::engine::_is_mouse_down = 1;
									rux::gui::engine::_mouse_down_time = event.xbutton.time;
								}
							}
							else if(event.xbutton.button == Button2)
							{
								middle_mouse_button_pressed = 1;
								if(event.xbutton.time - ::rux::gui::engine::_mouse_down_time < 250)
								{
									rux::gui::WindowMouseEvent xevent(
										::rux::gui::XEnum_EventType_WindowMouseWheelDoubleClick, window, 0										
										, event.xbutton.x, event.xbutton.y, alt, control, shift
										, left_mouse_button_pressed, middle_mouse_button_pressed
										, right_mouse_button_pressed, x_button1_pressed, x_button2_pressed);
									window->raise_event(xevent);
									rux::gui::engine::_is_middle_mouse_down = 0;
								}
								else
								{
									rux::gui::WindowMouseEvent xevent(::rux::gui::XEnum_EventType_WindowMouseWheelDown 
										, window, 0, event.xbutton.x, event.xbutton.y, alt, control, shift, 1 
										, middle_mouse_button_pressed, right_mouse_button_pressed, x_button1_pressed
										, x_button2_pressed);
									window->CaptureMouse();
									window->raise_event( xevent );
									rux::gui::engine::_is_middle_mouse_down = 1;
									rux::gui::engine::_middle_mouse_down_time = event.xbutton.time;
								}
							}
							else if( event.xbutton.button == Button3 )
							{
								right_mouse_button_pressed = 1;
								if( event.xbutton.time - ::rux::gui::engine::_right_mouse_down_time < 250 )
								{
									rux::gui::WindowMouseEvent xevent( ::rux::gui::XEnum_EventType_WindowRightMouseButtonDoubleClick , window , 0 , event.xbutton.x , event.xbutton.y , alt , control , shift , 
										left_mouse_button_pressed , middle_mouse_button_pressed , 
										right_mouse_button_pressed , x_button1_pressed , x_button2_pressed );
									window->raise_event( xevent );
									rux::gui::engine::_is_right_mouse_down = 0;
								}
								else
								{
									rux::gui::WindowMouseEvent xevent( ::rux::gui::XEnum_EventType_WindowRightMouseButtonDown , window , 0 , event.xbutton.x , event.xbutton.y , alt , control , shift , 
										left_mouse_button_pressed , middle_mouse_button_pressed , 
										1 , x_button1_pressed , x_button2_pressed );
									window->CaptureMouse();
									window->raise_event( xevent );
									rux::gui::engine::_is_right_mouse_down = 1;
									rux::gui::engine::_right_mouse_down_time = event.xbutton.time;
								}
							}
							else if( event.xbutton.button == Button4 )
							{
								rux::gui::WindowMouseEvent xevent( ::rux::gui::XEnum_EventType_WindowMouseWheel , window , 120 , event.xbutton.x , event.xbutton.y , alt , control , shift , 
									left_mouse_button_pressed , 1 , 
									right_mouse_button_pressed , x_button1_pressed , x_button2_pressed );
								window->raise_event( xevent );
							}
							else if( event.xbutton.button == Button5 )
							{
								rux::gui::WindowMouseEvent xevent( ::rux::gui::XEnum_EventType_WindowMouseWheel , window , -120 , event.xbutton.x , event.xbutton.y , alt , control , shift , 
									left_mouse_button_pressed , 1 , 
									right_mouse_button_pressed , x_button1_pressed , x_button2_pressed );
								window->raise_event( xevent );
							}							
							//printf( "%s\n" , "ButtonPress" );
							window->Release();
						}
						break;
					}
					case ButtonRelease:
					{
						window = ::rux::gui::engine::find_window( event.xbutton.window );
						if( window )
						{
							rux::uint8 control = event.xbutton.state & ControlMask ? 1 : 0;
							rux::uint8 left_mouse_button_pressed = event.xbutton.state & Button1Mask ? 1 : 0;
							rux::uint8 middle_mouse_button_pressed = event.xbutton.state & Button2Mask ? 1 : 0;
							rux::uint8 right_mouse_button_pressed = event.xbutton.state & Button3Mask ? 1 : 0;
							rux::uint8 shift = event.xbutton.state & ShiftMask ? 1 : 0;
							rux::uint8 alt = event.xbutton.state & LockMask ? 1 : 0;							
							rux::uint8 x_button1_pressed = event.xbutton.state & Button4Mask ? 1 : 0;
							rux::uint8 x_button2_pressed = event.xbutton.state & Button5Mask ? 1 : 0;						
							if( event.xbutton.button == Button1 )
							{
								left_mouse_button_pressed = 0;
								if( ::rux::gui::engine::_is_mouse_down == 1 )
								{
									rux::gui::WindowMouseEvent xevent( ::rux::gui::XEnum_EventType_WindowLeftMouseButtonUp , window , 0 , event.xbutton.x , event.xbutton.y , alt , control , shift , 
										left_mouse_button_pressed , middle_mouse_button_pressed , 
										right_mouse_button_pressed , x_button1_pressed , x_button2_pressed );
									window->ReleaseMouse();
									window->raise_event( xevent );
									rux::gui::engine::_is_mouse_down = 0;
								}
							}
							else if(event.xbutton.button == Button2)
							{
								middle_mouse_button_pressed = 0;
								if(::rux::gui::engine::_is_middle_mouse_down == 1)
								{
									rux::gui::WindowMouseEvent xevent(::rux::gui::XEnum_EventType_WindowWheelMouseButtonUp 
										, window, 0, event.xbutton.x, event.xbutton.y, alt, control, shift
										, left_mouse_button_pressed, middle_mouse_button_pressed
										, right_mouse_button_pressed, x_button1_pressed, x_button2_pressed);
									window->ReleaseMouse();
									window->raise_event(xevent);
									rux::gui::engine::_is_middle_mouse_down = 0;
								}
							}	
							else if( event.xbutton.button == Button3 )
							{
								right_mouse_button_pressed = 0;
								if( ::rux::gui::engine::_is_right_mouse_down == 1 )
								{
									rux::gui::WindowMouseEvent xevent( ::rux::gui::XEnum_EventType_WindowRightMouseButtonUp , window , 0 , event.xbutton.x , event.xbutton.y , alt , control , shift , 
										left_mouse_button_pressed , middle_mouse_button_pressed , 
										right_mouse_button_pressed , x_button1_pressed , x_button2_pressed );
									window->ReleaseMouse();
									window->raise_event( xevent );
									rux::gui::engine::_is_right_mouse_down = 0;
								}
							}	
							//printf( "%s\n" , "ButtonRelease" );		
							window->Release();
						}
						break;
					}
					case KeyPress:
					{
						window = ::rux::gui::engine::find_window( event.xkey.window );
						if( window )
						{							
							declare_stack_variable( char , key_string , 20 );
							KeySym keysym;
							XComposeStatus composestatus;
							size_t length = ::rux::engine::_globals->_x11_module.XLookupString( &event.xkey , key_string , 20 , &keysym , &composestatus );
							key_string[ length ] = '\0';
							rux::gui::XEnum_Keys key = XEnum_Keys_Unknown;
							switch( keysym )
							{
							case XK_Left:
								{
									key = XEnum_Keys_Left;	
									break;
								}
							case XK_Tab:
								{
									key = XEnum_Keys_Tab;	
									break;
								}
							case XK_Right:
								{
									key = 	XEnum_Keys_Right;
									break;
								}
							case XK_Up:
								{
									key = XEnum_Keys_Up;
									break;
								}
							case XK_Down:
								{
									key = XEnum_Keys_Down;
									break;
								}
							case XK_Home:
								{
									key = XEnum_Keys_Home;
									break;
								}
							case XK_End:
								{
									key = XEnum_Keys_End;
									break;
								}
							case XK_Insert:
								{								
									break;
								}
							case XK_BackSpace:
								{
									key = XEnum_Keys_Back;
									break;
								}
							case XK_Delete:
								{
									key = XEnum_Keys_Delete;
									break;
								}
							case XK_F2:
								{								
									break;
								}
							case XK_Return:
								{
									key = XEnum_Keys_Enter;
									break;
								}
							case 0x56:
								{
									//key = XEnum_Keys_V;
									break;
								}
							default:
								{
									if( key_string[ 0 ] != 0 && ( keysym!= XK_Return && keysym != XK_Escape && keysym != XK_Delete && keysym != XK_Shift_L ) )
									{
										rux::uint8 control = event.xkey.state & ControlMask ? 1 : 0;
										rux::uint8 left_mouse_button_pressed = event.xkey.state & Button1Mask ? 1 : 0;
										rux::uint8 middle_mouse_button_pressed = event.xkey.state & Button2Mask ? 1 : 0;
										rux::uint8 right_mouse_button_pressed = event.xkey.state & Button3Mask ? 1 : 0;
										rux::uint8 shift = event.xkey.state & ShiftMask ? 1 : 0;
										rux::uint8 alt = event.xkey.state & LockMask ? 1 : 0;							
										rux::uint8 x_button1_pressed = event.xkey.state & Button4Mask ? 1 : 0;
										rux::uint8 x_button2_pressed = event.xkey.state & Button5Mask ? 1 : 0;	
										declare_variable( rux::XString , symbol );
										symbol = key_string;
										symbol.set_ByRef( symbol.ConvertToUTF8() );
										rux::gui::WindowKeyCharEvent xevent( window , symbol.str() , symbol.Size() , alt , control , shift );
										window->raise_event( xevent );
									}
									break;
								}
							}
							rux::uint8 control = event.xkey.state & ControlMask ? 1 : 0;
							rux::uint8 left_mouse_button_pressed = event.xkey.state & Button1Mask ? 1 : 0;
							rux::uint8 middle_mouse_button_pressed = event.xkey.state & Button2Mask ? 1 : 0;
							rux::uint8 right_mouse_button_pressed = event.xkey.state & Button3Mask ? 1 : 0;
							rux::uint8 shift = event.xkey.state & ShiftMask ? 1 : 0;
							rux::uint8 alt = event.xkey.state & LockMask ? 1 : 0;							
							rux::uint8 x_button1_pressed = event.xkey.state & Button4Mask ? 1 : 0;
							rux::uint8 x_button2_pressed = event.xkey.state & Button5Mask ? 1 : 0;							
							rux::gui::WindowKeyDownEvent xevent( window , key , alt , control , shift , ::rux::gui::Keyboard::XlibToWinVirtualKey( keysym ) );
							window->raise_event( xevent );
							::rux::gui::engine::_is_x_key_pressed = 1;
							::rux::log::WriteTrace( "Pressed key '%s', keycode %u\n" , key_string , event.xkey.keycode );
							window->Release();
						}
						else
							::rux::log::WriteTrace( "Pressed key null window, keycode %u\n" , event.xkey.keycode );
						break;
					}
					case KeymapNotify:
						{
							window = ::rux::gui::engine::find_window( event.xkeymap.window );
							if( window )
							{
								//printf( "%s\n" , "KeymapNotify" );
								window->Release();
							}
							break;
						}
					case KeyRelease:
					{
						window = ::rux::gui::engine::find_window( event.xkey.window );
						if( window )
						{
							::rux::byte is_retriggered = 0;
							if( ::rux::gui::engine::_is_x_key_pressed == 1 )
							{
								rux::gui::engine::lock_display();
								if( ::rux::gui::engine::_is_x_if_event == 0
									&& ::rux::engine::_globals->_x11_module.XPending( ::rux::gui::engine::_x_display_main ) > 0 )
								{
									XEvent nev;
									::rux::engine::_globals->_x11_module.XPeekEvent( ::rux::gui::engine::_x_display_main , &nev );
									if( nev.type == KeyPress && nev.xkey.time == event.xkey.time && nev.xkey.keycode == event.xkey.keycode )
									{
										::rux::engine::_globals->_x11_module.XNextEvent( ::rux::gui::engine::_x_display_main , &nev );
										is_retriggered = 1;
									}
								}
								rux::gui::engine::unlock_display();
							}
							if( is_retriggered == 0 )
							{	
								declare_stack_variable( char , key_string , 20 );
								KeySym keysym;
								XComposeStatus composestatus;
								size_t length = ::rux::engine::_globals->_x11_module.XLookupString( &event.xkey , key_string , 20 , &keysym , &composestatus );
								key_string[ length ] = '\0';
								rux::gui::XEnum_Keys key = XEnum_Keys_Unknown;
								switch( keysym )
								{
								case XK_Left:
									{
										key = XEnum_Keys_Left;	
										break;
									}
								case XK_Tab:
									{
										key = XEnum_Keys_Tab;	
										break;
									}
								case XK_Right:
									{
										key = 	XEnum_Keys_Right;
										break;
									}
								case XK_Up:
									{
										key = XEnum_Keys_Up;
										break;
									}
								case XK_Down:
									{
										key = XEnum_Keys_Down;
										break;
									}
								case XK_Home:
									{
										key = XEnum_Keys_Home;
										break;
									}
								case XK_End:
									{
										key = XEnum_Keys_End;
										break;
									}
								case XK_Insert:
									{								
										break;
									}
								case XK_BackSpace:
									{
										key = XEnum_Keys_Back;
										break;
									}
								case XK_Delete:
									{
										key = XEnum_Keys_Delete;
										break;
									}
								case XK_F2:
									{								
										break;
									}
								case XK_Return:
									{
										key = XEnum_Keys_Enter;
										break;
									}
								case 0x56:
									{
										//key = XEnum_Keys_V;
										break;
									}
								default:
									{
										if( key_string[ 0 ] != 0 && ( keysym!= XK_Return && keysym != XK_Escape && keysym != XK_Delete && keysym != XK_Shift_L ) )
										{								
										}
										break;
									}
								}
								rux::uint8 control = event.xkey.state & ControlMask ? 1 : 0;
								rux::uint8 left_mouse_button_pressed = event.xkey.state & Button1Mask ? 1 : 0;
								rux::uint8 middle_mouse_button_pressed = event.xkey.state & Button2Mask ? 1 : 0;
								rux::uint8 right_mouse_button_pressed = event.xkey.state & Button3Mask ? 1 : 0;
								rux::uint8 shift = event.xkey.state & ShiftMask ? 1 : 0;
								rux::uint8 alt = event.xkey.state & LockMask ? 1 : 0;							
								rux::uint8 x_button1_pressed = event.xkey.state & Button4Mask ? 1 : 0;
								rux::uint8 x_button2_pressed = event.xkey.state & Button5Mask ? 1 : 0;							
								rux::gui::WindowKeyUpEvent xevent( window , key , alt , control , shift , ::rux::gui::Keyboard::XlibToWinVirtualKey( keysym ) );
								window->raise_event( xevent );
								::rux::gui::engine::_is_x_key_pressed = 0;
								::rux::log::WriteTrace( "Released key '%s', keycode %u\n" , key_string , event.xkey.keycode );
							}
							window->Release();
						}
						break;
					}
					case EnterNotify:
					{
						window = ::rux::gui::engine::find_window( event.xcrossing.window );
						if( window )
						{
							//printf( "%s\n" , "LeaveNotify" );
						//event.xcrossing.
						//printf( "LEAVE\n" );
							window->Release();
						}
						break;
					}
					case LeaveNotify:
					{
						window = ::rux::gui::engine::find_window( event.xcrossing.window );
						if( window )
						{
							//printf( "%s\n" , "LeaveNotify" );
						//event.xcrossing.
						//printf( "LEAVE\n" );
							window->Release();
						}
						break;
					}
					default:
					{
						printf( "Unknown event: %d (discarding)\n" , event.type );
						break;
					}
				}
			};
#endif
#endif			
		};
	};
};
