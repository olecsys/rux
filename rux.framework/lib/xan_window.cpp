#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_window.h>
#include <xan_uiapplication.h>
#include <xan_uirectangle.h>
#include <xan_uitextbox.h>
#include <xan_uitextblock.h>
#include <xan_uigroup.h>
#include <xan_uiimage.h>
#include <xan_uiline.h>
#include <xan_uipath.h>
#include <xan_uiprogressbar.h>
#include <xan_uibutton.h>
#include <xan_uislider.h>
#include <xan_uiinlines.h>
#include <xan_error.h>
#include <xan_float.h>
#include <xan_uint64.h>
#include <xan_keyvaluepair.h>
#include <xan_usercontrol.h>
implement_rux_base_methods_ns( Window , rux::gui );
implement_rux_set_operators_ns_( Window , rux::gui );
namespace rux
{
	namespace gui
	{
#ifdef __UNIX__
		struct MWMHints 
		{
			unsigned long flags;
			unsigned long functions;
			unsigned long decorations;
			long input_mode;
			unsigned long status;
		};
		#define MWM_HINTS_DECORATIONS ( 1L << 1 )
		#define PROP_MWM_HINTS_ELEMENTS 5
		enum
		{
			_NET_WM_STATE_REMOVE = 0 ,
			_NET_WM_STATE_ADD = 1 ,
			_NET_WM_STATE_TOGGLE = 2
		};
#endif
		implement_ctor_and_register_type( Window );
		Window::Window( const char* , const char* , ::rux::int32 ) 
			: rux::gui::ParentBase( Window::get_static_Type ) , declare_class_member( _group )
			, _window_base_state(0)
		{	
			_group.set_ControlName( "__WindowGroup" );
			_already_dispatch_on_resized = 0;
			_render_index = 0;
			_cursor = ::rux::gui::XEnum_Cursors_Arrow;
			_current_render_index = SIZE_MAX;
			strcpy( _utf8_control_name , "Window" );
			_utf8_control_name_hash = ::rux::cryptography::hash::times33_hash( _utf8_control_name , SIZE_MAX );

			_selected_z_index = 0.f;
			_window_base = NULL;
			//_closed = 0;
			_pressed_control = NULL;
			_caret.set_IsVisible( 0 );
			_caret.set_Color( create_color( 0 , 0 , 0 , 0 ) );
			_drag_control = NULL;		
			_old_width = 0;
			_old_cursor_left = 0;
			_old_height = 0;
			_old_cursor_top = 0;
			_old_top = 0xffffffff;
			_old_left = 0xffffffff;
			_start_position = XEnum_StartPosition_Manual;
			_is_start_drag = 0;	
			_scrollbar_left_offset = 0.f;
			_scrollbar_top_offset = 0.f;
			_tab_index = -1;
			_group.set_ParentControl( this );
		};
		Window::~Window()
		{
			Close();
			FreeResources( 0 );
			if( _window_base )
				::rux::engine::_globals->_gui_globals->_rux_gui_destroy_window( _window_base );
		};
		float Window::get_ScrollXOffset( void )
		{
			return _scrollbar_left_offset;
		};
		float Window::get_ScrollYOffset( void )
		{
			return _scrollbar_top_offset;
		};
		void Window::set_ScrollXOffset( float scroll_x_offset )
		{
			if( _scrollbar_left_offset != scroll_x_offset )
			{
				_scrollbar_left_offset = scroll_x_offset;
			}
		};
		void Window::set_ScrollYOffset( float scroll_y_offset )
		{
			if( _scrollbar_top_offset != scroll_y_offset )
			{
				_scrollbar_top_offset = scroll_y_offset;
			}
		};
		void Window::GCRefRelease( const char* file , ::rux::int32 line )
		{
			Release( file , line );
		};
		void Window::GCRefAddRef( const char* file , ::rux::int32 line )
		{
			AddRef( file , line );
		};
		::rux::uint8 Window::IsPointInControl( ::rux::int32 x , ::rux::int32 y )
		{
			return 1;
		};
		void Window::set_IsForwardEvents( ::rux::byte )
		{
		};
		::rux::byte Window::get_IsForwardEvents( void )
		{
			return 0;
		};
		void Window::raise_OnMouseLeave( ::rux::gui::WindowMouseEvent* , ::rux::byte )
		{
		};
		::rux::gui::XEnum_Cursors Window::get_Cursor( void )
		{
			return _cursor;
		};
		::rux::uint8 Window::get_IsButton( void )
		{
			return 0;
		};
		implement_duplicate_internal_function_1( Window , BringToFront , const XGCRef& );		
		XWindow::XWindow()
		{	
		};
		XWindow::XWindow( Window* window )
		{
			if( window )
				(*this) = *window;
		};
		XWindow::XWindow( const Window& window )
		{
			(*this) = window;
		};
		XWindow::XWindow( const XObject& object )
		{
				*static_cast<XGCHandle<Window>*>(this) = object;
		};
		XWindow::XWindow( const XWindow& window )
		{
				(*(static_cast<XGCHandle<Window>*>( this ))) = window;
		};
		XWindow& XWindow::operator =( const Window& window )
		{
			(*(static_cast<XGCHandle<Window>*>( this ))) = window;
			return *this;
		};
		XWindow& XWindow::operator =( const XWindow& window )
		{
			(*(static_cast<XGCHandle<Window>*>( this ))) = window;
			return *this;
		};
		void XWindow::set_StartPosition( XEnum_StartPosition start_position )
		{
			(*this)()->set_StartPosition( start_position );
		};
		void Window::set_StartPosition( XEnum_StartPosition start_position )
		{
			_start_position = start_position;
			if( _start_position == XEnum_StartPosition_Center )
			{
				XMallocArray< ::rux::gui::ScreenBase* >* screens = ::rux::engine::_globals->_gui_globals->_rux_gui_get_screens();
				if( screens && screens->Count() > 0 )
				{
					if( get_Width() > screens->operator[]( 0 )->get_Width() )
						set_Width( screens->operator[]( 0 )->get_Width() );
					if( get_Height() > screens->operator[]( 0 )->get_Height() )
						set_Height( screens->operator[]( 0 )->get_Height() );
					set_Left( screens->operator[]( 0 )->get_Left() + ( screens->operator[]( 0 )->get_Width() - get_Width() ) / 2 );
					set_Top( screens->operator[]( 0 )->get_Top() + ( screens->operator[]( 0 )->get_Height() - get_Height() ) / 2 );			
				}
				if( screens )
					::rux::engine::_globals->_gui_globals->_rux_gui_free_screens( screens );
			};
		};
		void Window::private_OnClosed( void )
		{
			::booldog::interlocked::atomic_return res = 0;
			for(;;)
			{
				res = ::booldog::interlocked::compare_exchange(&_window_base_state, 4, 1);
				if(res == 1)
				{
					_on_closed.raise< const ::rux::gui::events::Event& >(::rux::gui::events::Event(*this, 1));
					::booldog::interlocked::compare_exchange(&_window_base_state, 2, 4);
					FreeResources(0);
					_window_base = 0;
					//_closed = 1;
					::booldog::interlocked::compare_exchange(&_window_base_state, 3, 2);
				}
				else if(res == 2)
					continue;
				break;
			}
		};
		void XWindow::set_Width( ::rux::int32 width )
		{
			(*this)()->set_Width( width );
		};
		implement_duplicate_internal_function_1( Window , set_IsAllowResize , ::rux::byte );
		void Window::set_IsAllowResize( ::rux::byte is_allow_resize )
		{
			::rux::gui::WindowBase* window_base = get_WindowBase();
			if( window_base )
				window_base->set_IsAllowResize( is_allow_resize );
		};
		implement_duplicate_internal_function_1( Window , set_IsAllowRelocate , ::rux::byte );
		void Window::set_IsAllowRelocate( ::rux::byte is_allow_relocate )
		{
			::rux::gui::WindowBase* window_base = get_WindowBase();
			if( window_base )
				window_base->set_IsAllowRelocate( is_allow_relocate );
		};
		implement_duplicate_internal_function_1( Window , set_IsAllowClose , ::rux::byte );
		void Window::set_IsAllowClose( ::rux::byte is_allow_close )
		{
			::rux::gui::WindowBase* window_base = get_WindowBase();
			if( window_base )
				window_base->set_IsAllowClose( is_allow_close );
		};
		void Window::set_Width( ::rux::int32 width )
		{	
			::rux::gui::WindowBase* window_base = get_WindowBase();
			if( window_base )
				window_base->set_Width( width );
		};
		void XWindow::set_Height( ::rux::int32 height )
		{
			(*this)()->set_Height( height );
		};
		void Window::set_Height( ::rux::int32 height )
		{
			::rux::gui::WindowBase* window_base = get_WindowBase();
			if( window_base )
				window_base->set_Height( height );
		};
		implement_duplicate_internal_function_4( Window , set_LocationAndSize , ::rux::int32 , ::rux::int32 , ::rux::int32 , ::rux::int32 );
		void Window::set_LocationAndSize( ::rux::int32 x , ::rux::int32 y , ::rux::int32 width , ::rux::int32 height )
		{
			::rux::gui::WindowBase* window_base = get_WindowBase();
			if( window_base )
				window_base->set_LocationAndSize( x , y , width , height );
		};
		void XWindow::set_Left( ::rux::int32 left )
		{
			(*this)()->set_Left( left );
		};
		void Window::set_Left( float left )
		{
			::rux::gui::WindowBase* window_base = get_WindowBase();
			if( window_base )
				window_base->set_Left( left );
		};
		void XWindow::set_Top( ::rux::int32 top )
		{
			(*this)()->set_Top( top );
		};
		void Window::set_Top( float top )
		{
			::rux::gui::WindowBase* window_base = get_WindowBase();
			if( window_base )
				window_base->set_Top( top );
		};
		void XWindow::set_TopMost( ::rux::uint8 is_top_most )
		{
			(*this)()->set_TopMost( is_top_most );
		};
		void Window::set_TopMost( ::rux::uint8 is_top_most )
		{
			::rux::gui::WindowBase* window_base = get_WindowBase();
			if( window_base )
				window_base->set_TopMost( is_top_most );
		};
		rux::uint8 XWindow::get_IsVisible( void )
		{
			return (*this)()->get_IsVisible();
		};
		rux::int32 Window::get_IsVisible( void )
		{
			::rux::gui::WindowBase* window_base = get_WindowBase();
			if( window_base )
				return window_base->get_IsVisible();
			else
				return 0;
		};
		rux::uint8 XWindow::get_TopMost( void )
		{
			return (*this)()->get_TopMost();
		};
		rux::uint8 Window::get_TopMost( void )
		{
			::rux::gui::WindowBase* window_base = get_WindowBase();
			if( window_base )
				return window_base->get_TopMost();
			else
				return 0;
		};
		void XWindow::set_MinHeight( ::rux::int32 height )
		{
			(*this)()->set_MinHeight( height );
		};
		void Window::set_MinHeight( ::rux::int32 height )
		{
			::rux::gui::WindowBase* window_base = get_WindowBase();
			if( window_base )
				window_base->set_MinHeight( height );
		};
		void XWindow::set_MinWidth( ::rux::int32 width )
		{
			(*this)()->set_MinWidth( width );
		};
		void Window::set_MinWidth( ::rux::int32 width )
		{
			::rux::gui::WindowBase* window_base = get_WindowBase();
			if( window_base )
				window_base->set_MinWidth( width );
		};
		void XWindow::Restore( void )
		{
			(*this)()->Restore();
		};
		void Window::Restore( void )
		{
			::rux::gui::WindowBase* window_base = get_WindowBase();
			if( window_base )
				window_base->Restore();
		};
		void Window::Minimize( void )
		{
			::rux::gui::WindowBase* window_base = get_WindowBase();
			if( window_base )
				window_base->Minimize();
		};
		void XWindow::Maximize( void )
		{
			(*this)()->Maximize();
		};
		void XWindow::Minimize( void )
		{
			(*this)()->Minimize();
		};
		void Window::Maximize( void )
		{
			::rux::gui::WindowBase* window_base = get_WindowBase();
			if( window_base )
				window_base->Maximize();
		};
		void Window::raise_OnWindowRestored( void )
		{
			private_ResetChildLocationAndSizeCache( 0 , NULL , 1 , 1 , 1 , 1 );
			_on_restored.raise< const ::rux::gui::events::Event& >( ::rux::gui::events::Event( *this , 1 ) );
		};
		void Window::raise_OnWindowMinimized( void )
		{
			private_ResetChildLocationAndSizeCache( 0 , NULL , 1 , 1 , 1 , 1 );
			_on_minimized.raise< const ::rux::gui::events::Event& >( ::rux::gui::events::Event( *this , 1 ) );
		};
		void Window::raise_OnWindowMaximized( void )
		{
			private_ResetChildLocationAndSizeCache( 0 , NULL , 1 , 1 , 1 , 1 );
			_on_maximized.raise< const ::rux::gui::events::Event& >( ::rux::gui::events::Event( *this , 1 ) );
		};
		void Window::cleanup(void)
		{
			::booldog::interlocked::exchange(&_window_base_state, 0);
		};
		void Window::Show( void )
		{
			if(::rux::gui::application::run( this ) == 1)
			{
				::booldog::interlocked::compare_exchange(&_window_base_state, 0, 3);
				::rux::gui::WindowBase* window_base = get_WindowBase();
				if(window_base)
					window_base->Show();
			}
		};
		void XWindow::Show( void )
		{
			(*this)()->Show();
		};
		void XWindow::Hide( void )
		{
			(*this)()->Hide();
		};
		void Window::Hide( void )
		{
			if( ::rux::gui::application::run( this , 0 ) == 1 )
			{
				::rux::gui::WindowBase* window_base = get_WindowBase();
				if( window_base )
					window_base->Hide();
			}
		};
		void XWindow::Close( void )
		{
			(*this)()->Close();
		};
		void Window::Close( void )
		{
			if( _window_base )
				_window_base->Close();	
		};
		void Window::render( ::rux::gui::RenderContextBase* render_context )
		{	
			_selected_z_index = 0.f;
			_render_index++;
			_group()->render( render_context , 1.f , _selected_z_index , SIZE_MAX );
		};
		void Window::get_Clip( ::rux::gui::Rectangle*& clip , ::rux::uint8 )
		{
			if( _current_render_index != _render_index )
			{				
				_clip._left = 0.f , _clip._top = 0.f , _clip._width = get_Width() , _clip._height = get_Height();
				_current_render_index = _render_index;
			}
			if( clip )
				clip->_left = 0.f , clip->_top = 0.f , clip->_width = get_Width() , clip->_height = get_Height();
			else
				clip = &_clip;
		};
		void Window::private_Relocation( void )
		{
			_on_location.raise< const ::rux::gui::events::Event& >( ::rux::gui::events::Event( *this , 1 ) );
		};
		void Window::raise_OnWindowResize( ::rux::int32 , ::rux::int32 )
		{	
			_group.set_Width( get_Width() );
			_group.set_Height( get_Height() );
			if( XInterlocked::CompareExchange( &_already_dispatch_on_resized , 1 , 0 ) == 0 )
				Dispatch( dispatch_on_resized , this , 0 );
			_on_resize.raise< const ::rux::gui::events::Event& >( ::rux::gui::events::Event( *this , 1 ) );
		};
		rux::gui::XEnum_WindowState Window::get_WindowState( void )
		{
			::rux::gui::WindowBase* window_base = get_WindowBase();
			if( window_base )
				return window_base->get_WindowState();
			else
				return rux::gui::XEnum_WindowState_Normal;
		};
		rux::gui::XEnum_WindowState XWindow::get_WindowState( void )
		{
			return (*this)()->get_WindowState();
		};
		void Window::private_InsertTabControl( ::rux::int32 tab_index , ::rux::gui::ControlBase* control )
		{
			CRITICAL_SECTION_LOCK( _cs_tab_common_bases );
			for( size_t index1 = 0 ; index1 < _tab_common_bases.Count() ; index1++ )
			{
				if( _tab_common_bases[ index1 ] == control )
				{
					if( control->get_IsGroup() == 1 )
						((::rux::gui::controls::Group*)control->get_ObjectAddress())->RemoveTabControls();
					_tab_common_bases.RemoveAt( index1 );
					break;
				}
			}
			if( control->get_IsGroup() == 1 )
				((::rux::gui::controls::Group*)control->get_ObjectAddress())->AddTabControls();
			if( tab_index >= 0 )
				_tab_common_bases.Add( control );
			CRITICAL_SECTION_UNLOCK( _cs_tab_common_bases );	
		};
		void Window::RemoveControl( XObject& control )
		{
			RemoveControl( control.get_GCRef() );
		};
		void XWindow::RemoveControl( XObject& control )
		{
			(*this)()->RemoveControl( control.get_GCRef() );
		};
		void XWindow::BringToFront( const XObject& control )
		{
			(*this)()->BringToFront( control );
		};
		void Window::BringToFront( const XObject& control )
		{
			_group.BringToFront( control );
		};
		void Window::BringToFront( const XGCRef& control )
		{
			control.AddRef();
			_group.BringToFront( control );
			control.Release();
		};
		void XWindow::set_ZIndex( const XObject& control , ::rux::uint32 index )
		{
			(*this)()->set_ZIndex( control , index );
		};
		rux::uint32 XWindow::get_ControlsCount( void )
		{
			return (*this)()->get_ControlsCount();
		};
		struct dispatch_struct
		{
			rux::gui::Window* _window;
			XObject* _control;
			rux::XString* _error;
		};
		void Window::set_ActiveControl( ::rux::gui::ControlBase* control )
		{
			WRITE_LOCK( _cs_active_control );
			if( _active_control != control )
			{
				rux::gui::CommonBase* active_control = _active_control;
				control->GCRefAddRef( __FILE__ , __LINE__ );
				_active_control = control;
				control->GCRefAddRef( __FILE__ , __LINE__ );
				_cs_active_control.WriteUnlock();
				if( active_control )
				{
					active_control->raise_OnDeactivate( control );
					active_control->GCRefRelease( __FILE__ , __LINE__ );
				}
				control->raise_OnActivate();
				control->GCRefRelease( __FILE__ , __LINE__ );
			}
			else
				_cs_active_control.WriteUnlock();
		};
		::rux::gui::CommonBase* Window::get_ActiveControl( void )
		{
			return _active_control;
		};
		rux::gui::WindowBase* Window::get_WindowBase( void )
		{
			::booldog::interlocked::atomic_return res = 0;
			for(;;)
			{
				res = ::booldog::interlocked::compare_exchange(&_window_base_state, 2, 0);
				if(res == 0)
				{
					AddRef();
					_over_control = this;			
					_window_base = ::rux::engine::_globals->_gui_globals->_rux_gui_create_window(
						::rux::gui::application::gui_on_event);
					_window_base->set_Owner(this);
					
					::booldog::interlocked::compare_exchange(&_window_base_state, 1, 2);
				}
				else if(res == 2)
					continue;
				break;
			}
			return _window_base;
		};
		void Window::dispatch( ::rux::gui::rux_dispatcher_t rux_dispatcher , void* param , ::rux::uint8 wait )
		{
			::rux::gui::WindowBase* window_base = get_WindowBase();
			if( window_base )
				::rux::engine::_globals->_gui_globals->_rux_gui_dispatch( rux_dispatcher , param , window_base , wait );	
		};
		void Window::dispatch( ::rux::gui::rux_dispatcher_t rux_dispatcher , const XObject& param , ::rux::uint8 wait )
		{
			::rux::gui::WindowBase* window_base = get_WindowBase();
			if( window_base )
			{
				dispatch_struct _dispatch_struct;
				_dispatch_struct._control = const_cast< XObject* >( &param );
				_dispatch_struct._window = this;
				::rux::engine::_globals->_gui_globals->_rux_gui_dispatch( rux_dispatcher , &_dispatch_struct , window_base , wait );	
			}
		};
		void Window::set_ZIndex( const XObject& control , ::rux::uint32 index )
		{
			_group()->set_ZIndex( control.get_GCRef() , index );
		};
		void Window::set_ZIndex( const XGCRef& control , ::rux::uint32 index )
		{
			_group.set_ZIndex( control , index );
		};
		void Window::set_ZIndex( XGCRef* control , size_t index )
		{
			_group()->set_ZIndex( control , index );
		};
		void Window::InsertControl( ::rux::uint32 index , XObject& control , ::rux::XString& error )
		{
			error.Clear();
			_group.InsertControl( index , control , error );
		};
		rux::uint32 Window::get_ControlsCount( void )
		{
			return _group.get_ControlsCount();
		};
		void Window::AddTabControls( void )
		{	
			_group()->AddTabControls();
		};
		void Window::RemoveTabControls( void )
		{
			_group()->RemoveTabControls();
		};
		void Window::ClearControls( void )
		{	
			_group.ClearControls();
		};
		void Window::RemoveControl( XGCRef* control )
		{
			_group.RemoveControl( *control );
		};
		rux::uint8 Window::ExistsControl( const XObject& control )
		{
			return ExistsControl( *control.get_GCRef( 1 ) );
		}
		rux::uint8 XWindow::ExistsControl( const XGCRef& control )
		{
			return (*this)()->ExistsControl( control );
		};
		rux::uint8 Window::ExistsControl( const XGCRef& control )
		{
			return _group.ExistsControl( control );
		};
		rux::uint8 XWindow::ExistsControl( const XObject& control )
		{	
			return (*this)()->ExistsControl( control );
		};
		void XWindow::AddControl( const XObject& control , ::rux::XString& error )
		{
			(*this)()->AddControl( control , error );
		};
		void Window::AddControl( const XObject& control , ::rux::XString& error )
		{
			error.Clear();
			_group.AddControl( control , error );
		};
		implement_duplicate_internal_function_1( Window , set_Background , ::rux::gui::ColorBase* );
		void Window::set_Background( ::rux::gui::ColorBase* background )
		{	
			_group.set_Background( background );
		};
		implement_duplicate_internal_function_1( Window , set_BorderWidth , float );
		void Window::set_BorderWidth( float border_width )
		{
			_group.set_BorderWidth( border_width );
		};
		implement_duplicate_internal_function_1( Window , set_BorderColor , ::rux::gui::Color* );
		void Window::set_BorderColor( ::rux::gui::Color* border_color )
		{
			_group.set_BorderColor( border_color );
		};
		void Window::private_set_Tag( const XObject& ) const
		{
		};
		Object& Window::private_get_Tag( void )
		{
			return XObject( "" , __FILE__ , __LINE__ )++;
		};
		void Window::raise_OnActivate( void )
		{
		};
		::rux::int32 Window::IndexOf( XGCRef* control )
		{
			return _group()->IndexOf( control );
		};
		void Window::raise_OnDeactivate( ::rux::gui::CommonBase* )
		{
		};
		void Window::raise_OnRightMouseButtonDown( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
		{

			AddRef();
			_drag_and_drop_control = 0;
			_drag_and_drop_object = 0;
			StopCaret();

			WRITE_LOCK( _cs_over_control );
			if( _over_control == 0 || _over_control == (::rux::gui::CommonBase*)this )
			{
				::rux::gui::ControlBase* temp_control = _group();
				::rux::byte enter_raised = 0;
				::rux::gui::controls::rux_try_mouse_move( this , temp_control , window_event , enter_raised );
			}
			::rux::gui::CommonBase* over_control = _over_control;
			over_control->GCRefAddRef( __FILE__ , __LINE__ );
			_cs_over_control.WriteUnlock();
				
			over_control->GCRefAddRef( __FILE__ , __LINE__ );
			WRITE_LOCK( _cs_pressed_control );
			if( _pressed_control )
				_pressed_control->GCRefRelease( __FILE__ , __LINE__ );
			_pressed_control = over_control;
			_cs_pressed_control.WriteUnlock();
			WRITE_LOCK( _cs_active_control );
			if( _active_control != over_control )
			{	
				rux::gui::CommonBase* active_control = _active_control;
				over_control->GCRefAddRef( __FILE__ , __LINE__ );
				_active_control = over_control;
				_cs_active_control.WriteUnlock();
				if( active_control )
				{
					active_control->raise_OnDeactivate( over_control );
					active_control->GCRefRelease( __FILE__ , __LINE__ );
				}
				over_control->raise_OnActivate();
			}
			else
				_cs_active_control.WriteUnlock();
			if( over_control == ((::rux::gui::CommonBase*)this) )
				_on_right_mouse_button_down.raise< const ::rux::gui::events::MouseEvent >( ::rux::gui::events::MouseEvent( *this , window_event , 1 ) );
			else
			{
				over_control->raise_OnRightMouseButtonDown( window_event , 1 );
				::rux::gui::ParentBase* parent = over_control->get_ParentControl();
				if( parent && parent->get_IsWindow() == 0 )
				{
					::rux::gui::ParentBase* new_parent = parent;
					XMallocArray< ::rux::gui::ParentBase* > parents;
					for( ; ; )
					{
						new_parent = new_parent->get_ParentControl();
						if( new_parent && new_parent->get_IsWindow() == 0 )
						{
							new_parent->AddRef( __FILE__ , __LINE__ );
							parents.Add( new_parent );
						}
						else
							break;
					}
					size_t parents_index = 0;
					for( ; ; )
					{
						if( parent->get_IsForwardEvents() == 0 )
							parent->raise_OnRightMouseButtonDown( window_event , 0 );
						if( parents_index > 0 )
							parent->Release( __FILE__ , __LINE__ );
						if( parents_index < parents.Count() )
							parent = parents[ parents_index++ ];
						else
							break;
					}
				}
				_on_right_mouse_button_down.raise< const ::rux::gui::events::MouseEvent >( ::rux::gui::events::MouseEvent( *this , window_event , 0 ) );
			}
			over_control->GCRefRelease( __FILE__ , __LINE__ );
			Release();
		};
		void Window::raise_OnMouseWheelDown(::rux::gui::WindowMouseEvent* window_event, ::rux::byte explicit_event)
		{

			AddRef();
			_drag_and_drop_control = 0;
			_drag_and_drop_object = 0;
			StopCaret();

			WRITE_LOCK( _cs_over_control );
			if( _over_control == 0 || _over_control == (::rux::gui::CommonBase*)this )
			{
				::rux::gui::ControlBase* temp_control = _group();
				::rux::byte enter_raised = 0;
				::rux::gui::controls::rux_try_mouse_move( this , temp_control , window_event , enter_raised );
			}
			::rux::gui::CommonBase* over_control = _over_control;
			over_control->GCRefAddRef( __FILE__ , __LINE__ );
			_cs_over_control.WriteUnlock();
				
			over_control->GCRefAddRef( __FILE__ , __LINE__ );
			WRITE_LOCK( _cs_pressed_control );
			if( _pressed_control )
				_pressed_control->GCRefRelease( __FILE__ , __LINE__ );
			_pressed_control = over_control;
			_cs_pressed_control.WriteUnlock();
			WRITE_LOCK( _cs_active_control );
			if( _active_control != over_control )
			{	
				rux::gui::CommonBase* active_control = _active_control;
				over_control->GCRefAddRef( __FILE__ , __LINE__ );
				_active_control = over_control;
				_cs_active_control.WriteUnlock();
				if( active_control )
				{
					active_control->raise_OnDeactivate( over_control );
					active_control->GCRefRelease( __FILE__ , __LINE__ );
				}
				over_control->raise_OnActivate();
			}
			else
				_cs_active_control.WriteUnlock();
			if( over_control == ((::rux::gui::CommonBase*)this) )
				_on_mouse_wheel_down.raise< const ::rux::gui::events::MouseEvent >( ::rux::gui::events::MouseEvent( *this , window_event , 1 ) );
			else
			{
				over_control->raise_OnMouseWheelDown( window_event , 1 );
				::rux::gui::ParentBase* parent = over_control->get_ParentControl();
				if( parent && parent->get_IsWindow() == 0 )
				{
					::rux::gui::ParentBase* new_parent = parent;
					XMallocArray< ::rux::gui::ParentBase* > parents;
					for( ; ; )
					{
						new_parent = new_parent->get_ParentControl();
						if( new_parent && new_parent->get_IsWindow() == 0 )
						{
							new_parent->AddRef( __FILE__ , __LINE__ );
							parents.Add( new_parent );
						}
						else
							break;
					}
					size_t parents_index = 0;
					for( ; ; )
					{
						if( parent->get_IsForwardEvents() == 0 )
							parent->raise_OnMouseWheelDown( window_event , 0 );
						if( parents_index > 0 )
							parent->Release( __FILE__ , __LINE__ );
						if( parents_index < parents.Count() )
							parent = parents[ parents_index++ ];
						else
							break;
					}
				}
				_on_mouse_wheel_down.raise< const ::rux::gui::events::MouseEvent >( ::rux::gui::events::MouseEvent( *this , window_event , 0 ) );
			}
			over_control->GCRefRelease( __FILE__ , __LINE__ );
			Release();
		};
		void Window::raise_OnRightMouseButtonDoubleClick( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
		{
			StopCaret();

			WRITE_LOCK( _cs_over_control );
			if( _over_control == 0 || _over_control == (::rux::gui::CommonBase*)this )
			{
				::rux::gui::ControlBase* temp_control = _group();
				::rux::byte enter_raised = 0;
				::rux::gui::controls::rux_try_mouse_move( this , temp_control , window_event , enter_raised );
			}
			::rux::gui::CommonBase* over_control = _over_control;
			over_control->GCRefAddRef( __FILE__ , __LINE__ );
			_cs_over_control.WriteUnlock();
				
			if( over_control == ((::rux::gui::CommonBase*)this) )
				_on_right_mouse_button_double_click.raise< const ::rux::gui::events::MouseEvent >( ::rux::gui::events::MouseEvent( *this , window_event , 1 ) );
			else
			{
				over_control->raise_OnRightMouseButtonDoubleClick( window_event , 1 );
				::rux::gui::ParentBase* parent = over_control->get_ParentControl();
				if( parent && parent->get_IsWindow() == 0 )
				{
					::rux::gui::ParentBase* new_parent = parent;
					XMallocArray< ::rux::gui::ParentBase* > parents;
					for( ; ; )
					{
						new_parent = new_parent->get_ParentControl();
						if( new_parent && new_parent->get_IsWindow() == 0 )
						{
							new_parent->AddRef( __FILE__ , __LINE__ );
							parents.Add( new_parent );
						}
						else
							break;
					}
					size_t parents_index = 0;
					for( ; ; )
					{
						if( parent->get_IsForwardEvents() == 0 )
							parent->raise_OnRightMouseButtonDoubleClick( window_event , 0 );
						if( parents_index > 0 )
							parent->Release( __FILE__ , __LINE__ );
						if( parents_index < parents.Count() )
							parent = parents[ parents_index++ ];
						else
							break;
					}
				}
				_on_right_mouse_button_double_click.raise< const ::rux::gui::events::MouseEvent >( ::rux::gui::events::MouseEvent( *this , window_event , 0 ) );
			}
			over_control->GCRefRelease( __FILE__ , __LINE__ );
		};
		void Window::raise_OnMouseWheelDoubleClick(::rux::gui::WindowMouseEvent* window_event, ::rux::byte explicit_event)
		{
			StopCaret();

			WRITE_LOCK( _cs_over_control );
			if( _over_control == 0 || _over_control == (::rux::gui::CommonBase*)this )
			{
				::rux::gui::ControlBase* temp_control = _group();
				::rux::byte enter_raised = 0;
				::rux::gui::controls::rux_try_mouse_move( this , temp_control , window_event , enter_raised );
			}
			::rux::gui::CommonBase* over_control = _over_control;
			over_control->GCRefAddRef( __FILE__ , __LINE__ );
			_cs_over_control.WriteUnlock();
				
			if( over_control == ((::rux::gui::CommonBase*)this) )
				_on_mouse_wheel_double_click.raise< const ::rux::gui::events::MouseEvent >( ::rux::gui::events::MouseEvent( *this , window_event , 1 ) );
			else
			{
				over_control->raise_OnMouseWheelDoubleClick( window_event , 1 );
				::rux::gui::ParentBase* parent = over_control->get_ParentControl();
				if( parent && parent->get_IsWindow() == 0 )
				{
					::rux::gui::ParentBase* new_parent = parent;
					XMallocArray< ::rux::gui::ParentBase* > parents;
					for( ; ; )
					{
						new_parent = new_parent->get_ParentControl();
						if( new_parent && new_parent->get_IsWindow() == 0 )
						{
							new_parent->AddRef( __FILE__ , __LINE__ );
							parents.Add( new_parent );
						}
						else
							break;
					}
					size_t parents_index = 0;
					for( ; ; )
					{
						if( parent->get_IsForwardEvents() == 0 )
							parent->raise_OnMouseWheelDoubleClick( window_event , 0 );
						if( parents_index > 0 )
							parent->Release( __FILE__ , __LINE__ );
						if( parents_index < parents.Count() )
							parent = parents[ parents_index++ ];
						else
							break;
					}
				}
				_on_mouse_wheel_double_click.raise< const ::rux::gui::events::MouseEvent >( ::rux::gui::events::MouseEvent( *this , window_event , 0 ) );
			}
			over_control->GCRefRelease( __FILE__ , __LINE__ );
		};
		
		void Window::raise_OnRightMouseButtonUp( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
		{
			::rux::gui::WindowBase* window_base = get_WindowBase();
			if( window_base )
				window_base->ReleaseMouse();
			_old_top = 0xffffffff;
			_old_left = 0xffffffff;
			_old_width = 0;
			_old_cursor_left = 0;
			_old_height = 0;
			_old_cursor_top = 0;
			WRITE_LOCK( _cs_pressed_control );
			if( _pressed_control )
			{
				rux::gui::CommonBase* pressed_control = _pressed_control;
				_pressed_control = NULL;		
				_cs_pressed_control.WriteUnlock();
				{
					pressed_control->raise_OnRightMouseButtonUp( window_event , 1 );
					::rux::gui::ParentBase* parent = pressed_control->get_ParentControl();
					if( parent && parent->get_IsWindow() == 0 )
					{
						::rux::gui::ParentBase* new_parent = parent;
						XMallocArray< ::rux::gui::ParentBase* > parents;
						for( ; ; )
						{
							new_parent = new_parent->get_ParentControl();
							if( new_parent && new_parent->get_IsWindow() == 0 )
							{
								new_parent->AddRef( __FILE__ , __LINE__ );
								parents.Add( new_parent );
							}
							else
								break;
						}
						size_t parents_index = 0;
						for( ; ; )
						{
							if( parent->get_IsForwardEvents() == 0 )
								parent->raise_OnRightMouseButtonUp( window_event , 0 );
							if( parents_index > 0 )
								parent->Release( __FILE__ , __LINE__ );
							if( parents_index < parents.Count() )
								parent = parents[ parents_index++ ];
							else
								break;
						}
						_on_right_mouse_button_up.raise< const ::rux::gui::events::MouseEvent >( ::rux::gui::events::MouseEvent( *this , window_event , 0 ) );
					}
					else
						_on_right_mouse_button_up.raise< const ::rux::gui::events::MouseEvent >( ::rux::gui::events::MouseEvent( *this , window_event , 1 ) );
				}
				pressed_control->GCRefRelease( __FILE__ , __LINE__ );
			}
			else
				_cs_pressed_control.WriteUnlock();
			if( rux_is_object( _drag_and_drop_control , XObject ) == false
				|| rux_is_object( _drag_and_drop_object , XObject ) == false )
			{
				//DROP
				_drag_and_drop_control = 0;
				_drag_and_drop_object = 0;
			}
		};
		void Window::raise_OnMouseWheelUp(::rux::gui::WindowMouseEvent* window_event, ::rux::byte explicit_event)
		{
			::rux::gui::WindowBase* window_base = get_WindowBase();
			if( window_base )
				window_base->ReleaseMouse();
			_old_top = 0xffffffff;
			_old_left = 0xffffffff;
			_old_width = 0;
			_old_cursor_left = 0;
			_old_height = 0;
			_old_cursor_top = 0;
			WRITE_LOCK( _cs_pressed_control );
			if( _pressed_control )
			{
				rux::gui::CommonBase* pressed_control = _pressed_control;
				_pressed_control = NULL;		
				_cs_pressed_control.WriteUnlock();
				{
					pressed_control->raise_OnMouseWheelUp(window_event, 1);
					::rux::gui::ParentBase* parent = pressed_control->get_ParentControl();
					if( parent && parent->get_IsWindow() == 0 )
					{
						::rux::gui::ParentBase* new_parent = parent;
						XMallocArray< ::rux::gui::ParentBase* > parents;
						for( ; ; )
						{
							new_parent = new_parent->get_ParentControl();
							if( new_parent && new_parent->get_IsWindow() == 0 )
							{
								new_parent->AddRef( __FILE__ , __LINE__ );
								parents.Add( new_parent );
							}
							else
								break;
						}
						size_t parents_index = 0;
						for( ; ; )
						{
							if( parent->get_IsForwardEvents() == 0 )
								parent->raise_OnMouseWheelUp( window_event , 0 );
							if( parents_index > 0 )
								parent->Release( __FILE__ , __LINE__ );
							if( parents_index < parents.Count() )
								parent = parents[ parents_index++ ];
							else
								break;
						}
						_on_mouse_wheel_up.raise< const ::rux::gui::events::MouseEvent >( 
							::rux::gui::events::MouseEvent(*this, window_event, 0));
					}
					else
						_on_mouse_wheel_up.raise< const ::rux::gui::events::MouseEvent >(
						::rux::gui::events::MouseEvent(*this, window_event, 1));
				}
				pressed_control->GCRefRelease( __FILE__ , __LINE__ );
			}
			else
				_cs_pressed_control.WriteUnlock();
			if( rux_is_object( _drag_and_drop_control , XObject ) == false
				|| rux_is_object( _drag_and_drop_object , XObject ) == false )
			{
				//DROP
				_drag_and_drop_control = 0;
				_drag_and_drop_object = 0;
			}
		};
		void Window::raise_OnLeftMouseButtonDown( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
		{	
			AddRef();
			_drag_and_drop_control = 0;
			_drag_and_drop_object = 0;
			StopCaret();
			if( _group()->_border_width == 0 
				|| ( window_event->_x > _group()->_border_width &&
				window_event->_y > _group()->_border_width &&
				window_event->_x < get_Width() - _group()->_border_width &&
				window_event->_y < get_Height() - _group()->_border_width &&
				( window_event->_x < get_Width() - 4.f
				|| window_event->_y < get_Height() - 4.f ) ) )
			{	
				WRITE_LOCK( _cs_over_control );
				if( _over_control == 0 || _over_control == (::rux::gui::CommonBase*)this )
				{
					::rux::gui::ControlBase* temp_control = _group();
					::rux::byte enter_raised = 0;
					::rux::gui::controls::rux_try_mouse_move( this , temp_control , window_event , enter_raised );
				}
				::rux::gui::CommonBase* over_control = _over_control;
				over_control->GCRefAddRef( __FILE__ , __LINE__ );
				_cs_over_control.WriteUnlock();
				
				over_control->GCRefAddRef( __FILE__ , __LINE__ );
				WRITE_LOCK( _cs_pressed_control );
				if( _pressed_control )
					_pressed_control->GCRefRelease( __FILE__ , __LINE__ );
				_pressed_control = over_control;
				_cs_pressed_control.WriteUnlock();
				WRITE_LOCK( _cs_active_control );
				if( _active_control != over_control )
				{	
					rux::gui::CommonBase* active_control = _active_control;
					over_control->GCRefAddRef( __FILE__ , __LINE__ );
					_active_control = over_control;
					_cs_active_control.WriteUnlock();
					if( active_control )
					{
						active_control->raise_OnDeactivate( over_control );
						active_control->GCRefRelease( __FILE__ , __LINE__ );
					}
					over_control->raise_OnActivate();
				}
				else
					_cs_active_control.WriteUnlock();
				if( over_control == ((::rux::gui::CommonBase*)this) )
					_on_left_mouse_button_down.raise< const ::rux::gui::events::MouseEvent >( ::rux::gui::events::MouseEvent( *this , window_event , 1 ) );
				else
				{
					over_control->raise_OnLeftMouseButtonDown( window_event , 1 );
					::rux::gui::ParentBase* parent = over_control->get_ParentControl();
					if( parent && parent->get_IsWindow() == 0 )
					{
						::rux::gui::ParentBase* new_parent = parent;
						XMallocArray< ::rux::gui::ParentBase* > parents;
						for( ; ; )
						{
							new_parent = new_parent->get_ParentControl();
							if( new_parent && new_parent->get_IsWindow() == 0 )
							{
								new_parent->AddRef( __FILE__ , __LINE__ );
								parents.Add( new_parent );
							}
							else
								break;
						}
						size_t parents_index = 0;
						for( ; ; )
						{
							if( parent->get_IsForwardEvents() == 0 )
								parent->raise_OnLeftMouseButtonDown( window_event , 0 );
							if( parents_index > 0 )
								parent->Release( __FILE__ , __LINE__ );
							if( parents_index < parents.Count() )
								parent = parents[ parents_index++ ];
							else
								break;
						}
					}
					_on_left_mouse_button_down.raise< const ::rux::gui::events::MouseEvent >( ::rux::gui::events::MouseEvent( *this , window_event , 0 ) );
				}
				over_control->GCRefRelease( __FILE__ , __LINE__ );
			}
			else if( window_event->_x >= get_Width() - 4.f
				&& window_event->_y >= get_Height() - 4.f )
			{
				rux::gui::controls::rux_try_mouse_deactivate( this , NULL );
				WRITE_LOCK( _cs_over_control );
				if( _over_control != ((::rux::gui::CommonBase*)this) )
				{
					rux::gui::CommonBase* over_control = _over_control;
					GCRefAddRef( __FILE__ , __LINE__ );
					_over_control = this;
					_cs_over_control.WriteUnlock();
					::rux::gui::controls::rux_change_over_control( window_event , over_control , NULL );
					over_control->GCRefRelease( __FILE__ , __LINE__ );
				}
				else
					_cs_over_control.WriteUnlock();
				set_Cursor( ::rux::gui::XEnum_Cursors_SizeNWSE );
				rux::gui::Point pt;
				::rux::gui::WindowBase* window_base = get_WindowBase();
				if( window_base )
				{
					window_base->get_CursorPosition( pt );
					_old_cursor_left = (rux::int32)pt._x;
					_old_cursor_top = (rux::int32)pt._y;
					window_base->CaptureMouse();
					_old_width = get_Width();	
					_old_height = get_Height();
				}
			}
			else if( window_event->_x <= _group()->_border_width )
			{
				rux::gui::controls::rux_try_mouse_deactivate( this , NULL );
				WRITE_LOCK( _cs_over_control );
				if( _over_control != ((::rux::gui::CommonBase*)this) )
				{
					rux::gui::CommonBase* over_control = _over_control;
					GCRefAddRef( __FILE__ , __LINE__ );
					_over_control = this;
					_cs_over_control.WriteUnlock();
					::rux::gui::controls::rux_change_over_control( window_event , over_control , NULL );
					over_control->GCRefRelease( __FILE__ , __LINE__ );
				}
				else
					_cs_over_control.WriteUnlock();
				set_Cursor( ::rux::gui::XEnum_Cursors_SizeWE );
				rux::gui::Point pt;
				::rux::gui::WindowBase* window_base = get_WindowBase();
				if( window_base )
				{
					window_base->get_CursorPosition( pt );
					_old_cursor_left = (rux::int32)pt._x;
					window_base->CaptureMouse();
					_old_width = get_Width();		
					_old_left = get_Left();
				}
			}
			else if( window_event->_y <= _group()->_border_width )
			{
				rux::gui::controls::rux_try_mouse_deactivate( this , NULL );		
				WRITE_LOCK( _cs_over_control );
				if( _over_control != ((::rux::gui::CommonBase*)this) )
				{
					rux::gui::CommonBase* over_control = _over_control;
					GCRefAddRef( __FILE__ , __LINE__ );
					_over_control = this;
					_cs_over_control.WriteUnlock();
					::rux::gui::controls::rux_change_over_control( window_event , over_control , NULL );
					over_control->GCRefRelease( __FILE__ , __LINE__ );
				}
				else
					_cs_over_control.WriteUnlock();
				set_Cursor( ::rux::gui::XEnum_Cursors_SizeNS );
				rux::gui::Point pt;
				::rux::gui::WindowBase* window_base = get_WindowBase();
				if( window_base )
				{
					window_base->get_CursorPosition( pt );
					_old_cursor_top = (rux::int32)pt._y;
					window_base->CaptureMouse();
					_old_height = get_Height();		
					_old_top = get_Top();
				}
			}
			else if( window_event->_x >= get_Width() - _group()->_border_width )
			{
				rux::gui::controls::rux_try_mouse_deactivate( this , NULL );
				WRITE_LOCK( _cs_over_control );
				if( _over_control != ((::rux::gui::CommonBase*)this) )
				{
					rux::gui::CommonBase* over_control = _over_control;
					GCRefAddRef( __FILE__ , __LINE__ );
					_over_control = this;
					_cs_over_control.WriteUnlock();
					::rux::gui::controls::rux_change_over_control( window_event , over_control , NULL );
					over_control->GCRefRelease( __FILE__ , __LINE__ );
				}
				else
					_cs_over_control.WriteUnlock();
				set_Cursor( ::rux::gui::XEnum_Cursors_SizeWE );
				rux::gui::Point pt;
				::rux::gui::WindowBase* window_base = get_WindowBase();
				if( window_base )
				{
					window_base->get_CursorPosition( pt );
					_old_cursor_left = (rux::int32)pt._x;
					window_base->CaptureMouse();
					_old_width = get_Width();
				}
			}
			else if( window_event->_y >= get_Height() - _group()->_border_width )
			{
				rux::gui::controls::rux_try_mouse_deactivate( this , NULL );		
				WRITE_LOCK( _cs_over_control );
				if( _over_control != ((::rux::gui::CommonBase*)this) )
				{
					rux::gui::CommonBase* over_control = _over_control;
					GCRefAddRef( __FILE__ , __LINE__ );
					_over_control = this;
					_cs_over_control.WriteUnlock();
					::rux::gui::controls::rux_change_over_control( window_event , over_control , NULL );
					over_control->GCRefRelease( __FILE__ , __LINE__ );
				}
				else
					_cs_over_control.WriteUnlock();
				set_Cursor( ::rux::gui::XEnum_Cursors_SizeNS );
				rux::gui::Point pt;
				::rux::gui::WindowBase* window_base = get_WindowBase();
				if( window_base )
				{
					window_base->get_CursorPosition( pt );
					_old_cursor_top = (rux::int32)pt._y;
					window_base->CaptureMouse();
					_old_height = get_Height();
				}
			}	
			Release();
		};
		void Window::raise_OnLeftMouseButtonDoubleClick( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
		{	
			StopCaret();
			if( _group()->_border_width == 0 
				|| ( window_event->_x < get_Width() - _group()->_border_width
				&& window_event->_y < get_Height() - _group()->_border_width ) )
			{	
				WRITE_LOCK( _cs_over_control );
				if( _over_control == 0 || _over_control == (::rux::gui::CommonBase*)this )
				{
					::rux::gui::ControlBase* temp_control = _group();
					::rux::byte enter_raised = 0;
					::rux::gui::controls::rux_try_mouse_move( this , temp_control , window_event , enter_raised );
				}
				::rux::gui::CommonBase* over_control = _over_control;
				over_control->GCRefAddRef( __FILE__ , __LINE__ );
				_cs_over_control.WriteUnlock();
				
				if( over_control == ((::rux::gui::CommonBase*)this) )
					_on_left_mouse_button_double_click.raise< const ::rux::gui::events::MouseEvent >( ::rux::gui::events::MouseEvent( *this , window_event , 1 ) );
				else
				{
					over_control->raise_OnLeftMouseButtonDoubleClick( window_event , 1 );
					::rux::gui::ParentBase* parent = over_control->get_ParentControl();
					if( parent && parent->get_IsWindow() == 0 )
					{
						::rux::gui::ParentBase* new_parent = parent;
						XMallocArray< ::rux::gui::ParentBase* > parents;
						for( ; ; )
						{
							new_parent = new_parent->get_ParentControl();
							if( new_parent && new_parent->get_IsWindow() == 0 )
							{
								new_parent->AddRef( __FILE__ , __LINE__ );
								parents.Add( new_parent );
							}
							else
								break;
						}
						size_t parents_index = 0;
						for( ; ; )
						{
							if( parent->get_IsForwardEvents() == 0 )
								parent->raise_OnLeftMouseButtonDoubleClick( window_event , 0 );
							if( parents_index > 0 )
								parent->Release( __FILE__ , __LINE__ );
							if( parents_index < parents.Count() )
								parent = parents[ parents_index++ ];
							else
								break;
						}
					}
					_on_left_mouse_button_double_click.raise< const ::rux::gui::events::MouseEvent >( ::rux::gui::events::MouseEvent( *this , window_event , 0 ) );
				}
				over_control->GCRefRelease( __FILE__ , __LINE__ );
			}	
		};
		void XWindow::StartDrag( void )
		{
			(*this)()->StartDrag();
		};
		void Window::StartDrag( void )
		{
			_is_start_drag = 1;	
			set_Cursor( ::rux::gui::XEnum_Cursors_SizeAll );
			rux::gui::Point pt;
			::rux::gui::WindowBase* window_base = get_WindowBase();
			if( window_base )
			{
				window_base->get_CursorPosition( pt );
				_old_cursor_left = (rux::int32)pt._x;
				_old_cursor_top = (rux::int32)pt._y;
				window_base->CaptureMouse();
				_old_left = get_Left();		
				_old_top = get_Top();
			}
		};
		void XWindow::StartDrag( const XObject& control , ::rux::XString& error )
		{
			(*this)()->StartDrag( control , error );
		};
		void Window::StartDrag( const XObject& control , ::rux::XString& error )
		{
			error.Clear();
			rux::gui::ControlBase* common_base = (::rux::gui::ControlBase*)control.get_GCRef()->DynamicCast( XEnum_ClassType_ControlBase );
			float left = 0.f , top = 0.f;
			if( common_base )
			{		
				left = common_base->get_Left();
				top = common_base->get_Top();
				_drag_control = common_base;
			}
			else
			{
				rux_write_error( "control is not ::rux::gui::ControlBase class" , error );
			}
			if( error.Length() == 0 )
			{
				_is_start_drag = 1;		
				set_Cursor( ::rux::gui::XEnum_Cursors_SizeAll );
				rux::gui::Point pt;
				::rux::gui::WindowBase* window_base = get_WindowBase();
				if( window_base )
				{
					window_base->get_CursorPosition( pt );
					_old_cursor_left = (rux::int32)pt._x;
					_old_cursor_top = (rux::int32)pt._y;
					window_base->CaptureMouse();
					_old_left = (rux::int32)left;		
					_old_top = (rux::int32)top;		
				}
			}
		};
		void XWindow::StopDrag( void )
		{
			(*this)()->StopDrag();
		};
		void Window::StopDrag( void )
		{
			::rux::gui::WindowBase* window_base = get_WindowBase();
			if( window_base )
				window_base->ReleaseMouse();
			_old_left = 0xffffffff;
			_old_cursor_left = 0;
			_old_top = 0xffffffff;
			_old_cursor_top = 0;
			_is_start_drag = 0;
			_drag_control = NULL;
			WRITE_LOCK( _cs_over_control );
			if( _over_control )
				set_Cursor( _over_control->get_Cursor() );
			_cs_over_control.WriteUnlock();
		};
		Window* Window::get_ParentWindow( void ) const
		{
			return const_cast< Window* >( this );
		};
		void Window::raise_OnLeftMouseButtonUp( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
		{
			::rux::gui::WindowBase* window_base = get_WindowBase();
			if( window_base )
				window_base->ReleaseMouse();
			_old_top = 0xffffffff;
			_old_left = 0xffffffff;
			_old_width = 0;
			_old_cursor_left = 0;
			_old_height = 0;
			_old_cursor_top = 0;
			WRITE_LOCK( _cs_pressed_control );
			if( _pressed_control )
			{
				rux::gui::CommonBase* pressed_control = _pressed_control;
				_pressed_control = NULL;		
				_cs_pressed_control.WriteUnlock();
				{
					pressed_control->raise_OnLeftMouseButtonUp( window_event , 1 );
					::rux::gui::ParentBase* parent = pressed_control->get_ParentControl();
					if( parent && parent->get_IsWindow() == 0 )
					{
						::rux::gui::ParentBase* new_parent = parent;
						XMallocArray< ::rux::gui::ParentBase* > parents;
						for( ; ; )
						{
							new_parent = new_parent->get_ParentControl();
							if( new_parent && new_parent->get_IsWindow() == 0 )
							{
								new_parent->AddRef( __FILE__ , __LINE__ );
								parents.Add( new_parent );
							}
							else
								break;
						}
						size_t parents_index = 0;
						for( ; ; )
						{
							if( parent->get_IsForwardEvents() == 0 )
								parent->raise_OnLeftMouseButtonUp( window_event , 0 );
							if( parents_index > 0 )
								parent->Release( __FILE__ , __LINE__ );
							if( parents_index < parents.Count() )
								parent = parents[ parents_index++ ];
							else
								break;
						}
						_on_left_mouse_button_up.raise< const ::rux::gui::events::MouseEvent >( ::rux::gui::events::MouseEvent( *this , window_event , 0 ) );
					}
					else
						_on_left_mouse_button_up.raise< const ::rux::gui::events::MouseEvent >( ::rux::gui::events::MouseEvent( *this , window_event , 1 ) );
				}
				pressed_control->GCRefRelease( __FILE__ , __LINE__ );
			}
			else
				_cs_pressed_control.WriteUnlock();
			if( rux_is_object( _drag_and_drop_control , XObject ) == false
				|| rux_is_object( _drag_and_drop_object , XObject ) == false )
			{
				//DROP
				_drag_and_drop_control = 0;
				_drag_and_drop_object = 0;
			}
		};
		::rux::uint8 Window::get_IsWindow( void )
		{
			return 1;
		};
		void* Window::DynamicCast( ::rux::int32 class_type ) const
		{
			if( class_type == ::rux::gui::XEnum_ClassType_ControlBase )
			{
				::rux::gui::ControlBase* ptr = const_cast< Window* >( this );
				return ptr;
			}
			else if( class_type == ::rux::gui::XEnum_ClassType_ParentBase )
			{
				::rux::gui::ParentBase* ptr = const_cast< Window* >( this );
				return ptr;
			}
			return 0;
		};
		::rux::uint8 Window::get_IsGroup( void )
		{
			return 0;
		};
		::rux::uint8 Window::get_IsTimePicker( void )
		{
			return 0;
		};
		::rux::byte Window::raise_OnMouseEnter( ::rux::gui::WindowMouseEvent* , ::rux::byte& , ::rux::byte )
		{
			return 1;
		};
		::rux::gui::ParentBase* Window::get_ParentControl( ::rux::byte , ::rux::byte )
		{
			return 0;
		};
		void Window::raise_OnMouseWheel( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
		{	
			if( _group()->_border_width == 0 
				|| ( window_event->_x < get_Width() - _group()->_border_width
				&& window_event->_y < get_Height() - _group()->_border_width ) )
			{		
				WRITE_LOCK( _cs_over_control );
				if( _over_control == 0 || _over_control == (::rux::gui::CommonBase*)this )
				{
					::rux::gui::ControlBase* temp_control = _group();
					::rux::byte enter_raised = 0;
					::rux::gui::controls::rux_try_mouse_move( this , temp_control , window_event , enter_raised );
				}
				::rux::gui::CommonBase* over_control = _over_control;
				over_control->GCRefAddRef( __FILE__ , __LINE__ );
				_cs_over_control.WriteUnlock();

				if( over_control == ((::rux::gui::CommonBase*)this) )
					_on_mouse_wheel.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , 1 ) );
				else
				{
					over_control->raise_OnMouseWheel( window_event , 1 );
					::rux::gui::ParentBase* parent = over_control->get_ParentControl();
					if( parent && parent->get_IsWindow() == 0 )
					{
						::rux::gui::ParentBase* new_parent = parent;
						XMallocArray< ::rux::gui::ParentBase* > parents;
						for( ; ; )
						{
							new_parent = new_parent->get_ParentControl();
							if( new_parent && new_parent->get_IsWindow() == 0 )
							{
								new_parent->AddRef( __FILE__ , __LINE__ );
								parents.Add( new_parent );
							}
							else
								break;
						}
						size_t parents_index = 0;
						for( ; ; )
						{
							if( parent->get_IsForwardEvents() == 0 )
								parent->raise_OnMouseWheel( window_event , 0 );
							if( parents_index > 0 )
								parent->Release( __FILE__ , __LINE__ );
							if( parents_index < parents.Count() )
								parent = parents[ parents_index++ ];
							else
								break;
						}
					}
					_on_mouse_wheel.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , 0 ) );
				}
				over_control->GCRefRelease( __FILE__ , __LINE__ );
			}
		};
		void Window::Dispatch( ::rux::gui::rux_dispatcher_t rux_dispatcher , void* param , ::rux::uint8 wait )
		{
			dispatch( rux_dispatcher , param , wait );
		};
		void XWindow::Dispatch( ::rux::gui::rux_dispatcher_t rux_dispatcher , void* param )
		{
			(*this)()->Dispatch( rux_dispatcher , param );
		};
		::rux::byte Window::Exists( ::rux::gui::CommonBase* control )
		{
			if( control == ((::rux::gui::CommonBase*)this) )
				return 1;
			else
			{
				::rux::gui::ParentBase* parent = _group();
				return parent->Exists( control );
			}
		};
		void Window::raise_OnMouseMove( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
		{
			if( _is_start_drag == 1 )
			{		
				rux::gui::Point pt;
				::rux::gui::WindowBase* window_base = get_WindowBase();
				if( window_base )
					window_base->get_CursorPosition( pt );
				if( _drag_control ) 
				{
					::rux::gui::ParentBase* parent_control = _drag_control->get_ParentControl();
					if( parent_control )
					{
						if( parent_control->get_IsWindow() == 0 )
						{
							float new_left = _old_left - ( _old_cursor_left - pt._x );
							float new_top = _old_top - ( _old_cursor_top - pt._y );
							if( _drag_control->get_PointUnit() == ::rux::gui::XEnum_PointUnit_Pixel 
								|| _drag_control->get_PointUnit() == ::rux::gui::XEnum_PointUnit_PercentOnlySize )
							{
								if( new_left >= parent_control->get_Left() 
									&& new_left + _drag_control->get_Width() <= parent_control->get_Left() + parent_control->get_Width() )
									_drag_control->set_Left( new_left - parent_control->get_Left() );
								if( new_top >= parent_control->get_Top() 
									&& new_top + _drag_control->get_Height() <= parent_control->get_Top() + parent_control->get_Height() )
									_drag_control->set_Top( new_top - parent_control->get_Top() );
							}
							else if( _drag_control->get_PointUnit() == ::rux::gui::XEnum_PointUnit_Percent 
								|| _drag_control->get_PointUnit() == ::rux::gui::XEnum_PointUnit_PercentOnlyLocation )
							{
								float parent_left = parent_control->get_Left();
								float parent_width = parent_control->get_Width();
								if( new_left >= parent_left
									&& new_left + _drag_control->get_Width() <= parent_left + parent_width )
									_drag_control->set_Left( ( new_left - parent_left ) / parent_width );
								float parent_top = parent_control->get_Top();
								float parent_height = parent_control->get_Height();
								if( new_top >= parent_top 
									&& new_top + _drag_control->get_Height() <= parent_top + parent_height )
									_drag_control->set_Top( ( new_top - parent_top ) / parent_height );
							}
						}
						else
						{
							float new_left = _old_left - ( _old_cursor_left - pt._x );
							float new_top = _old_top - ( _old_cursor_top - pt._y );
							if( _drag_control->get_PointUnit() == ::rux::gui::XEnum_PointUnit_Pixel 
								|| _drag_control->get_PointUnit() == ::rux::gui::XEnum_PointUnit_PercentOnlySize )
							{
								_drag_control->set_Left( new_left );
								_drag_control->set_Top( new_top );
							}
							else if( _drag_control->get_PointUnit() == ::rux::gui::XEnum_PointUnit_Percent
								|| _drag_control->get_PointUnit() == ::rux::gui::XEnum_PointUnit_PercentOnlyLocation )
							{
								_drag_control->set_Left( new_left / parent_control->get_Width() );
								_drag_control->set_Top( new_top / parent_control->get_Height() );
							}
						}
					}
				}
				else
				{ 
					set_Left( (rux::int32)( _old_left - ( _old_cursor_left - pt._x ) ) );
					set_Top( (rux::int32)( _old_top - ( _old_cursor_top - pt._y ) ) );
				}
			}
			else if( window_event->_left_mouse_button_pressed == 1 
				&& ( _old_width != 0.0
				|| _old_height != 0.0 ) )
			{
				rux::gui::Point pt;
				::rux::gui::WindowBase* window_base = get_WindowBase();
				if( window_base )
					window_base->get_CursorPosition( pt );
				if( _old_width != 0.0 )
				{
					if( _old_left == 0xffffffff )
						set_Width( (rux::int32)( _old_width - ( _old_cursor_left - pt._x ) ) );
					else
					{
						set_Left( (rux::int32)pt._x );
						set_Width( (rux::int32)( _old_width + ( _old_cursor_left - pt._x ) ) );
					}
				}
				if( _old_height != 0.0 )
				{
					if( _old_top == 0xffffffff )
						set_Height( _old_height - ( _old_cursor_top - pt._y ) );	
					else
					{
						set_Top( pt._y );
						set_Height( _old_height + ( _old_cursor_top - pt._y ) );	
					}
				}
			}
			else
			{
				if( _group()->_border_width == 0 
					|| ( window_event->_x > _group()->_border_width &&
					window_event->_y > _group()->_border_width &&
					window_event->_x < get_Width() - _group()->_border_width &&
					window_event->_y < get_Height() - _group()->_border_width &&
					( window_event->_x < get_Width() - 4.f ||
					window_event->_y < get_Height() - 4.f ) ) )
				{
					rux::uint8 temp_res = 0;
					WRITE_LOCK( _cs_pressed_control );
					if( window_event->_left_mouse_button_pressed == 0 )
					{
						_cs_pressed_control.WriteUnlock();
						::rux::gui::ControlBase* temp_control = _group();
						::rux::byte enter_raised = 0;
						temp_res = ::rux::gui::controls::rux_try_mouse_move( this , temp_control , window_event , enter_raised );
					}
					else if( _pressed_control && _pressed_control != ((::rux::gui::CommonBase*)this) )
					{
						::rux::gui::CommonBase* pressed_control = _pressed_control;
						pressed_control->GCRefAddRef( __FILE__ , __LINE__ );
						_cs_pressed_control.WriteUnlock();
						pressed_control->raise_OnMouseMove( window_event , 1 );
						::rux::gui::ParentBase* parent = pressed_control->get_ParentControl();
						if( parent && parent->get_IsWindow() == 0 )
						{
							::rux::gui::ParentBase* new_parent = parent;
							XMallocArray< ::rux::gui::ParentBase* > parents;
							for( ; ; )
							{
								new_parent = new_parent->get_ParentControl();
								if( new_parent && new_parent->get_IsWindow() == 0 )
								{
									new_parent->AddRef( __FILE__ , __LINE__ );
									parents.Add( new_parent );
								}
								else
									break;
							}
							size_t parents_index = 0;
							for( ; ; )
							{
								parent->raise_OnMouseMove( window_event , 0 );
								if( parents_index > 0 )
									parent->Release( __FILE__ , __LINE__ );
								if( parents_index < parents.Count() )
									parent = parents[ parents_index++ ];
								else
									break;
							}
						}
						pressed_control->GCRefRelease( __FILE__ , __LINE__ );
						temp_res = 1;
					}
					else
						_cs_pressed_control.WriteUnlock();
					WRITE_LOCK( _cs_over_control );
					if( temp_res == 0 && _over_control != ((::rux::gui::CommonBase*)this) )
					{
						rux::gui::CommonBase* over_control = _over_control;
						GCRefAddRef( __FILE__ , __LINE__ );
						_over_control = this;
						_cs_over_control.WriteUnlock();
						::rux::gui::controls::rux_change_over_control( window_event , over_control , NULL );
						over_control->GCRefRelease( __FILE__ , __LINE__ );
						set_Cursor( _cursor );
					}
					else
						_cs_over_control.WriteUnlock();
					_on_mouse_move.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , temp_res == 1 ? 0 : 1 ) );
				}
				else if( window_event->_x >= get_Width() - 4.f
					&& window_event->_y >= get_Height() - 4.f )
				{
					WRITE_LOCK( _cs_over_control );
					if( _over_control != ((::rux::gui::CommonBase*)this) )
					{
						rux::gui::CommonBase* over_control = _over_control;
						GCRefAddRef( __FILE__ , __LINE__ );
						_over_control = this;
						_cs_over_control.WriteUnlock();
						::rux::gui::controls::rux_change_over_control( window_event , over_control , NULL );
						if( over_control )
							over_control->GCRefRelease( __FILE__ , __LINE__ );
					}
					else
						_cs_over_control.WriteUnlock();
					set_Cursor( ::rux::gui::XEnum_Cursors_SizeNWSE );
				}
				else if( window_event->_y < _group()->_border_width )
				{
					WRITE_LOCK( _cs_over_control );
					if( _over_control != ((::rux::gui::CommonBase*)this) )
					{
						rux::gui::CommonBase* over_control = _over_control;
						GCRefAddRef( __FILE__ , __LINE__ );
						_over_control = this;
						_cs_over_control.WriteUnlock();
						::rux::gui::controls::rux_change_over_control( window_event , over_control , NULL );
						if( over_control )
							over_control->GCRefRelease( __FILE__ , __LINE__ );
					}
					else
						_cs_over_control.WriteUnlock();
					set_Cursor( ::rux::gui::XEnum_Cursors_SizeNS );
				}
				else if( window_event->_x < _group()->_border_width )
				{
					WRITE_LOCK( _cs_over_control );
					if( _over_control != ((::rux::gui::CommonBase*)this) )
					{
						rux::gui::CommonBase* over_control = _over_control;
						GCRefAddRef( __FILE__ , __LINE__ );
						_over_control = this;
						_cs_over_control.WriteUnlock();
						::rux::gui::controls::rux_change_over_control( window_event , over_control , NULL );
						over_control->GCRefRelease( __FILE__ , __LINE__ );
					}
					else
						_cs_over_control.WriteUnlock();
					set_Cursor( ::rux::gui::XEnum_Cursors_SizeWE );
				}
				else if( window_event->_x >= get_Width() - _group()->_border_width )
				{
					WRITE_LOCK( _cs_over_control );
					if( _over_control != ((::rux::gui::CommonBase*)this) )
					{
						rux::gui::CommonBase* over_control = _over_control;
						GCRefAddRef( __FILE__ , __LINE__ );
						_over_control = this;
						_cs_over_control.WriteUnlock();
						::rux::gui::controls::rux_change_over_control( window_event , over_control , NULL );
						over_control->GCRefRelease( __FILE__ , __LINE__ );
					}
					else
						_cs_over_control.WriteUnlock();
					set_Cursor( ::rux::gui::XEnum_Cursors_SizeWE );
				}
				else if( window_event->_y >= get_Height() - _group()->_border_width )
				{
					WRITE_LOCK( _cs_over_control );
					if( _over_control != ((::rux::gui::CommonBase*)this) )
					{
						rux::gui::CommonBase* over_control = _over_control;
						GCRefAddRef( __FILE__ , __LINE__ );
						_over_control = this;
						_cs_over_control.WriteUnlock();
						::rux::gui::controls::rux_change_over_control( window_event , over_control , NULL );
						over_control->GCRefRelease( __FILE__ , __LINE__ );
					}
					else
						_cs_over_control.WriteUnlock();
					set_Cursor( ::rux::gui::XEnum_Cursors_SizeNS );
				}
			}
		};
		::rux::byte Window::get_IsMouseEntered( void )
		{
			return 0;
		};
		void Window::set_IsMouseEntered( ::rux::byte )
		{
		};
		implement_duplicate_internal_function_1( Window , set_OnLeftMouseButtonDown , ::rux::gui::events::on_mouse_event_t );
		void Window::set_OnLeftMouseButtonDown( ::rux::gui::events::on_mouse_event_t on_left_mouse_button_down_callback )
		{
			_on_left_mouse_button_down = on_left_mouse_button_down_callback;
		};
		implement_duplicate_internal_function_1( Window , set_OnRightMouseButtonDown , ::rux::gui::events::on_mouse_event_t );
		void Window::set_OnRightMouseButtonDown( ::rux::gui::events::on_mouse_event_t on_right_mouse_button_down_callback )
		{
			_on_right_mouse_button_down = on_right_mouse_button_down_callback;
		};
		implement_duplicate_internal_function_1(Window , set_OnMouseWheelDown, ::rux::gui::events::on_mouse_event_t);
		void Window::set_OnMouseWheelDown(::rux::gui::events::on_mouse_event_t on_mouse_wheel_down_callback)
		{
			_on_mouse_wheel_down = on_mouse_wheel_down_callback;
		};
		implement_duplicate_internal_function_1(Window , set_OnMouseWheelUp, ::rux::gui::events::on_mouse_event_t);
		void Window::set_OnMouseWheelUp(::rux::gui::events::on_mouse_event_t on_mouse_wheel_up_callback)
		{
			_on_mouse_wheel_up = on_mouse_wheel_up_callback;
		};
		implement_duplicate_internal_function_1( Window , set_OnLeftMouseButtonUp , ::rux::gui::events::on_mouse_event_t );
		void Window::set_OnLeftMouseButtonUp( ::rux::gui::events::on_mouse_event_t on_left_mouse_button_up_callback )
		{
			_on_left_mouse_button_up = on_left_mouse_button_up_callback;
		};
		implement_duplicate_internal_function_1( Window , set_OnRightMouseButtonUp , ::rux::gui::events::on_mouse_event_t );
		void Window::set_OnRightMouseButtonUp( ::rux::gui::events::on_mouse_event_t on_right_mouse_button_up_callback )
		{
			_on_right_mouse_button_up = on_right_mouse_button_up_callback;
		};
		implement_duplicate_internal_function_1( Window , set_OnMouseMove , ::rux::gui::events::on_mouse_event_t );
		void Window::set_OnMouseMove( ::rux::gui::events::on_mouse_event_t on_mouse_move_callback )
		{
			_on_mouse_move = on_mouse_move_callback;
		};
		implement_duplicate_internal_function_1( Window , set_OnKeyDown , ::rux::gui::events::on_keyboard_event_t );
		void Window::set_OnKeyDown( ::rux::gui::events::on_keyboard_event_t on_key_down_callback )
		{
			_on_key_down = on_key_down_callback;
		};
		implement_duplicate_internal_function_1( Window , set_OnKeyUp , ::rux::gui::events::on_keyboard_event_t );
		void Window::set_OnKeyUp( ::rux::gui::events::on_keyboard_event_t on_key_up_callback )
		{
			_on_key_up = on_key_up_callback;
		};
		void Window::raise_OnKeyChar( const ::rux::XString& symbol , ::rux::uint8 shift , ::rux::uint8 control , ::rux::uint8 alt )
		{	
			WRITE_LOCK( _cs_active_control );
			if( _active_control )
			{
				::rux::gui::CommonBase* active_control = _active_control;
				active_control->GCRefAddRef( __FILE__ , __LINE__ );
				_cs_active_control.WriteUnlock();
				active_control->raise_OnKeyChar( symbol , shift , control , alt );
				active_control->GCRefRelease( __FILE__ , __LINE__ );
			}
			else
				_cs_active_control.WriteUnlock();
		};
		void Window::raise_OnKeyDown( ::rux::gui::XEnum_Keys key , ::rux::uint8 shift , ::rux::uint8 control , ::rux::uint8 alt , ::rux::uint32 value , ::rux::byte explicit_event )
		{	
			if( key == ::rux::gui::XEnum_Keys_Tab && shift == 0 && control == 0 && alt == 0 )
			{		
				CRITICAL_SECTION_LOCK( _cs_tab_common_bases );
				if( _tab_common_bases.Count() > 0 )
				{
					rux::uint8 exists = 0;
					size_t index0 = 0;
					do
					{
						_tab_index++;
						for( index0 = 0 ; index0 < _tab_common_bases.Count() ; index0++ )
						{
							if( _tab_common_bases[ index0 ]->get_TabIndex() == _tab_index )
							{
								WRITE_LOCK( _cs_active_control );
								if( _tab_common_bases[ index0 ] != _active_control )
								{
									::rux::gui::CommonBase* active_control0 = _active_control;
									_active_control = _tab_common_bases[ index0 ];
									_active_control->GCRefAddRef( __FILE__ , __LINE__ );
									::rux::gui::CommonBase* active_control = _active_control;
									active_control->GCRefAddRef( __FILE__ , __LINE__ );
									_cs_active_control.WriteUnlock();
									if( active_control0 )
									{
										active_control0->raise_OnDeactivate( active_control );
										active_control0->GCRefRelease( __FILE__ , __LINE__ );
									}
									active_control->raise_OnActivate();
									active_control->GCRefRelease( __FILE__ , __LINE__ );
									exists = 1;
								}
								else
									_cs_active_control.WriteUnlock();
								break;					
							}
						}
						if( index0 >= _tab_common_bases.Count() )
							_tab_index = -1;
					}
					while( exists == 0 );
				}
				CRITICAL_SECTION_UNLOCK( _cs_tab_common_bases );
			}
			else
			{		
				WRITE_LOCK( _cs_active_control );
				if( _active_control && _active_control != (::rux::gui::CommonBase*)this )
				{
					::rux::gui::CommonBase* active_cmn_ctrl = _active_control;
					active_cmn_ctrl->GCRefAddRef( __FILE__ , __LINE__ );
					_cs_active_control.WriteUnlock();
					active_cmn_ctrl->raise_OnKeyDown( key , shift , control , alt , value , 1 );
					active_cmn_ctrl->GCRefRelease( __FILE__ , __LINE__ );
					_on_key_down.raise< const ::rux::gui::events::KeyboardEvent& >( ::rux::gui::events::KeyboardEvent( *this , key , shift , control , alt , value , 0 ) );
				}
				else
				{
					_cs_active_control.WriteUnlock();
					_on_key_down.raise< const ::rux::gui::events::KeyboardEvent& >( ::rux::gui::events::KeyboardEvent( *this , key , shift , control , alt , value , 1 ) );
				}
			}
		};
		void Window::private_KeyUp( ::rux::gui::XEnum_Keys key , ::rux::uint8 shift , ::rux::uint8 control , ::rux::uint8 alt , ::rux::uint32 value , ::rux::byte explicit_event )
		{
			_on_key_up.raise< const ::rux::gui::events::KeyboardEvent& >( ::rux::gui::events::KeyboardEvent( *this , key , shift , control , alt , value , explicit_event ) );
		};
		Object& Window::private_ActivateControl( void )
		{
			return XObject( "" , __FILE__ , __LINE__ )++;
		};
		void XWindow::StartCaret( float left , float top , float height , ::rux::gui::Color* color )
		{
			(*this)()->StartCaret( left , top , height , color );
		};
		void Window::StartCaret( float left , float top , float height , ::rux::gui::Color* color )
		{
			StopCaret();
			rux::XString error;
			AddControl( _caret , error );
			_caret.set_IsVisible( 1 );
			_caret.set_StartLeft( left );
			_caret.set_StartTop( top );
			_caret.set_EndLeft( left );
			_caret.set_EndTop( top + height );
			_caret.set_Thickness( 1 );		
			_caret_animation.Clear();
			XAnimation animation;
			animation.set_Control( _caret );
			animation.set_PropertyName( "Color" );	
			animation.set_StartValue( color );
			animation.AddTimeline( 500000ULL , create_color( color->get_Red() , color->get_Green() , color->get_Blue() , 0 ) );
			animation.AddTimeline( 500000ULL , create_color( color->get_Red() , color->get_Green() , color->get_Blue() , color->get_Alpha() ) );
			_caret_animation.AddAnimation( animation );
			_caret_animation.Start();
		};
		void Window::StopCaret( void )
		{
			if( _caret_animation.get_IsStarted() == 1 )
				_caret_animation.Stop();
			if( ExistsControl( _caret ) == 1 )
			{
				_caret.set_Color( create_color( 0 , 0 , 0 , 0 ) );
				RemoveControl( _caret );
			}
		};
		void XWindow::StopCaret( void )
		{
			(*this)()->StopCaret();
		};
		rux::int32 XWindow::get_Left( void )
		{
			return (*this)()->get_Left();
		};
		float Window::get_Left( ::rux::uint8 , float* )
		{
			::rux::gui::WindowBase* window_base = get_WindowBase();
			if( window_base )
				return window_base->get_Left();
			else
				return 0;
		};
		rux::int32 XWindow::get_Top( void )
		{
			return (*this)()->get_Top();
		};
		float Window::get_Top( ::rux::uint8 , float* )
		{
			::rux::gui::WindowBase* window_base = get_WindowBase();
			if( window_base )
				return window_base->get_Top();
			else
				return 0;
		};
		rux::int32 XWindow::get_Width( void )
		{
			return (*this)()->get_Width();
		};
		rux::int32 XWindow::get_Height( void )
		{
			return (*this)()->get_Height();
		};
		float Window::get_Width( float* )
		{
			::rux::gui::WindowBase* window_base = get_WindowBase();
			if( window_base )
				return window_base->get_Width();
			else
				return 0;
		};
		float Window::get_Height( float* )
		{
			::rux::gui::WindowBase* window_base = get_WindowBase();
			if( window_base )
				return window_base->get_Height();
			else
				return 0;
		};
		void XWindow::Invalidate( void )
		{
		};
		void Window::Invalidate( void )
		{
		}
		void XWindow::set_Tag( const XObject& tag )
		{
			(*this)()->_tag = tag;
		};
		implement_duplicate_internal_result_function( Object& , Window , get_Tag );
		Object& Window::get_Tag( void )
		{
			return _tag++;
		};
		implement_duplicate_internal_function_1( Window , set_OnResize , ::rux::gui::events::on_event_t );
		void Window::set_OnResize( ::rux::gui::events::on_event_t on_resize_callback )
		{
			_on_resize = on_resize_callback;
		};
		implement_duplicate_internal_function_1( Window , set_OnLocationChanged , ::rux::gui::events::on_event_t );
		void Window::set_OnLocationChanged( ::rux::gui::events::on_event_t on_location_callback )
		{
			_on_location = on_location_callback;
		};
		implement_duplicate_internal_function_1( Window , set_OnShown , ::rux::gui::events::on_event_t );
		void Window::set_OnShown( ::rux::gui::events::on_event_t on_shown_callback )
		{
			_on_shown = on_shown_callback;
		};
		implement_duplicate_internal_function_1( Window , set_OnClosed , ::rux::gui::events::on_event_t );
		void Window::set_OnClosed( ::rux::gui::events::on_event_t on_closed_callback )
		{
			_on_closed = on_closed_callback;
		};
		implement_duplicate_internal_function_1( Window , set_OnRestored , ::rux::gui::events::on_event_t );
		void Window::set_OnRestored( ::rux::gui::events::on_event_t on_restored_callback )	
		{
			_on_restored = on_restored_callback;
		};
		implement_duplicate_internal_function_1( Window , set_OnMinimized , ::rux::gui::events::on_event_t );
		void Window::set_OnMinimized( ::rux::gui::events::on_event_t on_minimized_callback )
		{
			_on_minimized = on_minimized_callback;
		};
		implement_duplicate_internal_function_1( Window , set_OnMaximized , ::rux::gui::events::on_event_t );
		void Window::set_OnMaximized( ::rux::gui::events::on_event_t on_maximized_callback )
		{
			_on_maximized = on_maximized_callback;
		};
		implement_duplicate_internal_function_1( Window , set_OnHided , ::rux::gui::events::on_event_t );
		void Window::set_OnHided( ::rux::gui::events::on_event_t on_hided_callback )
		{
			_on_hided = on_hided_callback;
		};
		void Window::private_Shown( void )
		{
			_group.set_Width( get_Width() );
			_group.set_Height( get_Height() );
			_on_shown.raise< const ::rux::gui::events::Event& >( ::rux::gui::events::Event( *this , 1 ) );
		};
		void Window::private_Hided( void )
		{
			_on_hided.raise< const ::rux::gui::events::Event& >( ::rux::gui::events::Event( *this , 1 ) );
		};
		void Window::UpdateParentWindow( void )
		{
		};
		void Window::FreeResources( ::rux::byte from_removed_controls_schedule )
		{	
			_group()->FreeResources( from_removed_controls_schedule );
			_tab_common_bases.Clear();
			WRITE_LOCK( _cs_active_control );
			if( _active_control )
			{
				_active_control->GCRefRelease( __FILE__ , __LINE__ );
				_active_control = NULL;
			}
			_cs_active_control.WriteUnlock();
			_drag_control = NULL;
			WRITE_LOCK( _cs_over_control );
			if( _over_control )
			{
				_over_control->GCRefRelease( __FILE__ , __LINE__ );
				_over_control = NULL;
			}
			_cs_over_control.WriteUnlock();
			WRITE_LOCK( _cs_pressed_control );
			if( _pressed_control )
			{
				_pressed_control->GCRefRelease( __FILE__ , __LINE__ );
				_pressed_control = NULL;
			}
			_cs_pressed_control.WriteUnlock();
			_drag_and_drop_object = (::rux::int32)0;
			_drag_and_drop_control = (::rux::int32)0;
			_tag = (::rux::int32)0;
			_tab_index = -1;
		};
		void XWindow::set_Cursor( ::rux::gui::XEnum_Cursors cursor )
		{
			(*this)()->set_Cursor( cursor );
		};
		void Window::set_Cursor( ::rux::gui::XEnum_Cursors cursor )
		{
			::rux::gui::WindowBase* window_base = get_WindowBase();
			if( window_base )
				window_base->set_Cursor( cursor );
		};
		void Window::dispatch_on_resized( void* param )
		{
			Window* control = (Window*)param;
			XInterlocked::Exchange( &control->_already_dispatch_on_resized , 0 );
		};
		void Window::private_ResetChildLocationAndSizeCache( ::rux::uint8 reset , ::rux::gui::ControlBase* control , ::rux::uint8 left , ::rux::uint8 top , ::rux::uint8 width , ::rux::uint8 height , size_t )
		{	
			_group()->private_ResetChildLocationAndSizeCache( reset , control , left , top , width , height );
		};
		void XWindow::x_initialize( void )
		{
		};
		void XWindow::ShowTimeoutMessage( const ::rux::XString& message , ::rux::uint64 show_timeout , ::rux::uint64 hide_timeout )
		{
			declare_variable( ::rux::gui::controls::XGroup , message_group );
			message_group.set_Opacity( 0.f );
			message_group.set_Height( 70.f );
			message_group.set_HorizontalAlignment( XEnum_Alignment_RightOrBottom );
			message_group.set_VerticalAlignment( XEnum_Alignment_RightOrBottom );
			message_group.set_Tag( XUInt64( hide_timeout ) );

			rux::gui::controls::XRectangle back;	
			back.set_Corner( ::rux::gui::Thickness( 0.5f , 0.5f , 0.5f , 0.5f ) );
			back.set_HorizontalFilling( XEnum_Filling_Auto );
			back.set_VerticalFilling( XEnum_Filling_Auto );	
			back.set_Background( ::rux::gui::Colors::VerticalSliderPanelColor() );
			rux::XString error;
			message_group.AddControl( back , error );	

			rux::gui::controls::XTextBlock message_textblock;
			message_textblock.set_HorizontalAlignment( XEnum_Alignment_Center );
			message_textblock.set_VerticalAlignment( XEnum_Alignment_Center );
			message_textblock.set_HorizontalFilling( XEnum_Filling_Auto );
			message_textblock.set_Height( 60.f );
			message_textblock.set_Text( message );
			float width = message_textblock.get_TextWidth();
			message_group.set_Width( width + 5.f );	
			message_group.AddControl( message_textblock , error );
			AddControl( message_group , error );
			XAnimation animation;	
			animation.set_Control( message_group );
			animation.set_AnimationSetValueType( XEnum_AnimationSetValueType_Gradual );
			animation.set_StartValue( XFloat( 0.f ) );
			animation.set_PropertyName( "Opacity" );
			animation.AddTimeline( show_timeout , XFloat( 0.8f ) );
			XGroupAnimation group_animation;
			back.set_Tag( group_animation );
			group_animation.set_AnimationType( XEnum_AnimationType_OneTime );
			group_animation.AddAnimation( animation );		
			group_animation.set_OnCompleted( on_show_timeout_message_completed );
			group_animation.Start();
		};
		void XWindow::on_hide_timeout_message_completed( const ::rux::gui::events::Event& event )
		{
			XGroupAnimation group_animation( event._sender );
			XAnimation animation( group_animation.get_Animation( 0 ) );
			declare_variable_param( ::rux::gui::controls::XGroup , group , animation.get_Control() );
			rux::gui::Window* window = group.get_ParentWindow();
			if( window )
				window->RemoveControl( group );
		};
		implement_duplicate_internal_result_function( ::rux::String& , Window , get_ControlName );
		rux::String& Window::get_ControlName( void )
		{
			return ::rux::XString( _utf8_control_name , XEnumCodePage_UTF8 )++;
		};
		implement_duplicate_internal_result_function( ::rux::uint32 , Window , get_ControlNameHash );
		rux::uint32 Window::get_ControlNameHash( void )
		{
			return _utf8_control_name_hash;
		};
		void XWindow::on_show_timeout_message_completed( const ::rux::gui::events::Event& event )
		{
			XGroupAnimation group_animation( event._sender );
			XAnimation animation( group_animation.get_Animation( 0 ) );
			declare_variable_param( ::rux::gui::controls::XGroup , group , animation.get_Control() );
			XUInt64 timeout( group.get_Tag() );
			XAnimation hide_animation;		
			hide_animation.set_Control( group );
			hide_animation.set_AnimationSetValueType( XEnum_AnimationSetValueType_Gradual );
			hide_animation.set_StartValue( XFloat( 0.8f ) );
			hide_animation.set_PropertyName( "Opacity" );
			hide_animation.AddTimeline( 1000000ULL , XFloat( 0.8f ) );
			hide_animation.AddTimeline( 1000000ULL + timeout() , XFloat( 0.f ) );
			XGroupAnimation hide_group_animation;
			group.set_Tag( hide_group_animation );
			hide_group_animation.set_AnimationType( XEnum_AnimationType_OneTime );
			hide_group_animation.AddAnimation( hide_animation );		
			hide_group_animation.set_OnCompleted( on_hide_timeout_message_completed );
			hide_group_animation.Start();
		};
		void Window::render( ::rux::gui::RenderContextBase* , float , float& , size_t )
		{
		};
		void Window::private_ResetLocationAndSizeCache( ::rux::uint8 , ::rux::uint8 , ::rux::uint8 , ::rux::uint8 , ::rux::uint8 )
		{
		};
		::rux::uint8 Window::get_IsSupportContentSize( void )
		{
			return 0;
		};
		::rux::uint8 Window::get_IsTransparent( void )
		{
			return 0;
		};
		void Window::FreeSystemResources( void )
		{
		};
		::rux::byte Window::IsFreeSystemResourcesThread( void )
		{
			return 0;
		};
		void Window::set_FreeSystemResourcesThreadId( void )
		{
		};
		void Window::BeforeFreeSystemResources( void )
		{
		};
		void Window::WaitForFreeSystemResources( void )
		{
		};
		::rux::int32 Window::get_TabIndex( void )
		{
			return 0;
		};
		void Window::set_ParentControl( ::rux::gui::ParentBase* )
		{
		};
		void Window::set_OnDeactivated( ::rux::gui::events::on_deactivated_event_t )
		{
		};
		void Window::private_ResetCache( void )
		{
		};
		::rux::uint32 Window::get_TypeIndex( void )
		{
			return get_RuxTypeIndex();
		};
		::rux::uint8 Window::get_IsUserControl( void )
		{
			return 0;
		};
		void Window::RemoveControl( void )
		{
		};
		void Window::Activate( void )
		{
		};
		::rux::gui::XEnum_Filling Window::get_VerticalFilling( void )
		{
			return ::rux::gui::XEnum_Filling_Manual;
		};
		::rux::gui::XEnum_Filling Window::get_HorizontalFilling( void )
		{
			return ::rux::gui::XEnum_Filling_Manual;
		};
		::rux::gui::XEnum_Alignment Window::get_VerticalAlignment( void )
		{
			return ::rux::gui::XEnum_Alignment_Manual;
		};
		::rux::gui::XEnum_Alignment Window::get_HorizontalAlignment( void )
		{
			return ::rux::gui::XEnum_Alignment_Manual;
		};
		::rux::gui::Margin& Window::get_Margin( void )
		{
			return _margin;
		};
		::rux::gui::XEnum_PointUnit Window::get_PointUnit( void )
		{
			return ::rux::gui::XEnum_PointUnit_Pixel;
		};
		void Window::set_Left( float ) const
		{
		};
		void Window::set_Top( float ) const
		{
		};
		void* Window::get_TagPtr( void ) const
		{
			return 0;
		};
	};
};
