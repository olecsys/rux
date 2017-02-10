#ifndef XAN_GUI_WINDOW_H
#define XAN_GUI_WINDOW_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include<xan_gui_defines.h>
#include<xan_render_context.h>
#include <xan_malloc_array.h>
namespace rux
{
	namespace gui
	{
		namespace engine
		{			
			class RenderThread;
			class RenderContext;
			class Screen
			{
				friend class ::rux::gui::engine::RenderContext;
			private:
				rux::int32 _left;
				rux::int32 _top;
				rux::int32 _width;
				rux::int32 _height;
#ifdef __WINDOWS__
				HDC _hdc;
#endif
			public:
				Screen();
				Screen( rux::int32 left , rux::int32 top , rux::int32 width , rux::int32 height );
				Screen( const Screen& screen );
				Screen& operator =( const Screen& screen );
#ifdef __WINDOWS__
				Screen( rux::int32 left , rux::int32 top , rux::int32 width , rux::int32 height , HDC hdc );
				static BOOL CALLBACK monitor_enum( HMONITOR , HDC , LPRECT , LPARAM );
#endif
				static XMallocArray< rux::gui::ScreenBase* >* get_Screens( void );
				virtual rux::int32 get_Left( void );
				virtual rux::int32 get_Top( void );
				virtual rux::int32 get_Width( void );
				virtual rux::int32 get_Height( void );
				static void free( XMallocArray< rux::gui::ScreenBase* >* screens );
			};
			class RenderContext;
			class Window;
			class RenderUniversalCache;
			extern dll_internal XCrtSect* _cs_windows;
			extern dll_internal XMallocArray< rux::gui::engine::Window* > _windows;
			class Window : public rux::gui::WindowBase
			{
				friend class ::rux::gui::engine::RenderContext;
				friend class ::rux::gui::engine::GLContext;
				friend class ::rux::gui::engine::RenderThread;
				friend class ::rux::gui::engine::RenderUniversalCache;
#ifdef __WINDOWS__
				friend LRESULT CALLBACK WndProc( HWND hwnd , rux::uint32 msg , WPARAM wparam , LPARAM lparam );
				friend dll_internal rux::gui::engine::Window* find_window( HWND hwnd );
#elif defined( __UNIX__ )
#ifdef __ANDROID__
#else
				friend dll_internal void WndProc( XEvent& event );
				friend dll_internal rux::gui::engine::Window* find_window( ::Window& xwindow );
				friend dll_internal void start( rux::gui::on_event_t on_event , void* owner );
#endif
#endif
			private:
				::rux::byte _both_size_and_move_msg;
				rux::uint32 _windows_index;
				rux::uint8 _is_top_most;
				rux::uint8 _is_visible;
				rux::int32 _left;
				rux::int32 _top;
				rux::int32 _height;
				rux::int32 _width;
				rux::int32 _min_width;
				rux::int32 _min_height;
				rux::uint8 _is_transparent_window;
#ifdef __WINDOWS__
			public:
				HWND _hwnd;
			private:
				HDC _hdc;				
				HBITMAP _transparent_window_bitmap;
#elif defined( __UNIX__ )
#ifdef __ANDROID__
#else
				::Window _x_window;
				Display* _x_display;
#endif
#endif
			public:
				RenderContext* _render_context;
			private:
				rux::gui::XEnum_WindowState _prev_window_state;
				rux::gui::XEnum_WindowState _window_state;
				void* _owner;
				rux::gui::on_event_t _on_event;		
				rux::gui::XEnum_Cursors _cursor;
				rux_volatile _ref;
				::rux::byte _is_allow_resize;
				::rux::byte _is_allow_relocate;
				::rux::byte _is_allow_close;
			public:
				rux::uint8 _is_closing;
				Window();
				~Window();
				void free_resources( void );
				void AddRef( void ) const;
				void Release( void ) const;	
				void raise_event( rux::gui::Event& xevent );
				virtual void Show( void );
				virtual void Hide( void );
				virtual void Close( void );
				friend dll_internal rux::uint8 dispatch( rux::gui::rux_dispatcher_t rux_dispatcher , void* param , rux::gui::WindowBase* window_base , rux::uint8 stop_render , rux::uint8 no_send_from_render , rux::uint8 wait );
				friend void dispatch_create_window( void* param );
				friend dll_internal rux::gui::WindowBase* create_window( rux::gui::on_event_t on_event );
				friend dll_internal void destroy_window( rux::gui::WindowBase* window );
				void Minimize( ::rux::byte force );
				void Maximize( ::rux::byte force );
				void Restore( ::rux::byte force );
				virtual void Minimize( void );
				virtual void Maximize( void );
				virtual void Restore( void );
				virtual void set_Owner( void* owner );
				virtual void* get_Owner( void );
				virtual void set_TopMost( rux::uint8 top_most );
				virtual rux::uint8 get_TopMost( void );
				virtual rux::uint8 get_IsVisible( void );
				virtual void set_Width( rux::int32 width );
				virtual void set_Height( rux::int32 width );
				virtual void set_Left( rux::int32 width );
				virtual void set_Top( rux::int32 width );
				virtual void set_LocationAndSize( ::rux::int32 x , ::rux::int32 y , ::rux::int32 width , ::rux::int32 height );
				virtual void set_IsAllowResize( ::rux::byte is_allow_resize );
				virtual void set_IsAllowClose( ::rux::byte is_allow_close );
				virtual void set_IsAllowRelocate( ::rux::byte is_allow_relocate );
				virtual rux::int32 get_Left( void );
				virtual rux::int32 get_Top( void );
				virtual rux::int32 get_Width( void );
				virtual rux::int32 get_Height( void );
				virtual void set_MinWidth( rux::int32 width );
				virtual void set_MinHeight( rux::int32 height );
				virtual rux::gui::XEnum_WindowState get_WindowState( void );
				virtual void CaptureMouse( void );
				virtual void ReleaseMouse( void );
				virtual void set_Cursor( rux::gui::XEnum_Cursors cursor );
				virtual void get_CursorPosition( rux::gui::Point& position );
				void check( const char* __file__ , ::rux::int32 __line__ );
				void restore( void );
				static void dispatch_create_context( void* param );
				static void dispatch_close_window( void* param );
#ifdef __UNIX__
#ifdef __ANDROID__
#else
				static void wait_for_map_notify( Window* window );
#endif
#endif
				virtual void RestartRender();
			};
		};
	};
};
#endif
