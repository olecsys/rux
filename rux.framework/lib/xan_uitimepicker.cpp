#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_uitimepicker.h>
#include <xan_uiinlines.h>
#include <xan_uiprimitives.h>
#include <xan_uirectangle.h>
#include <xan_float.h>
#include <xan_uitextblock.h>
#include <xan_uiprimitives.h>
#include <xan_keyvaluepair.h>
#ifndef BOOLDOG_NETWORK
#define BOOLDOG_NETWORK
#endif
#ifndef BOOLDOG_HEADER
#define BOOLDOG_HEADER( header ) <booldog/header>
#endif
#include BOOLDOG_HEADER(boo_time_utils.h)
#include BOOLDOG_HEADER(boo_heap_allocator.h)
#include BOOLDOG_HEADER(boo_mixed_allocator.h)
begin_implement_rux_class_with_properties_ns_base_class( TimePicker , rux::gui::controls , rux::gui::ParentBase )
	_tag.set_Info( "tag" , __file__ , __line__ );
	_private_tag.set_Info( "private_tag" , __file__ , __line__ );
	IMPLEMENT_BASE_UI_MEMBERS();	
	_posix_time = ::booldog::utils::time::posix::now_as_utc();

	::booldog::allocators::easy::heap heap;
	::booldog::allocators::single_threaded::mixed< 512 > mixed( &heap );
	{
		::booldog::result_mbchar mbchar( &mixed );

		::booldog::utils::time::posix::mbs::tostring< 16 >( &mbchar , mbchar.mballocator , "%H:%M:%S" 
			, ::booldog::utils::time::posix::tolocal( _posix_time ) , debuginfo_macros );
		_time_textbox.set_Text( mbchar.mbchar );	

		rux::XString error;
		WRITE_LOCK( _time_textbox()->_cs_parent_control );
		_time_textbox()->_parent_control = this;
		_time_textbox()->_cs_parent_control.WriteUnlock();
		_time_textbox.set_TagPtr( this );
		_time_textbox.set_LeftMouseDownSelectionRegexpMask( "(0[0-9]|1[0-9]|2[0-3])|[0-5][0-9]|[0-5][0-9]" , error );
		_time_textbox.set_RegexpMask( XRegexp::TimeMask() , error );				
		_time_textbox.set_IsMultiLines( 0 );
		_time_textbox.set_OnTextChanged( on_text_changed );
		_time_font = ::rux::engine::_globals->_gui_globals->_rux_gui_create_font( "tahoma.ttf" , 0 , 12 );
		_on_datetime_changed_event_callback = NULL;	
		_time_textbox()->_horizontal_text_alignment = XEnum_Alignment_Center;
		_current_render_index = SIZE_MAX;
	}
end_implement_rux_class();
namespace rux
{
	namespace gui
	{
		namespace controls
		{
			TimePicker::~TimePicker()
			{
				FreeResources( 0 );
			};
			void XTimePicker::set_OnDatetimeChanged( ::rux::gui::events::on_event_t on_datetime_changed_event_callback )
			{
				(*this)()->_on_datetime_changed_event_callback = on_datetime_changed_event_callback;
			};
			void TimePicker::set_ScrollXOffset( float )
			{
			};
			void TimePicker::set_ScrollYOffset( float )
			{
			};
			float TimePicker::get_ScrollXOffset( void )
			{
				return 0;
			};
			::rux::int32 TimePicker::IndexOf( XGCRef* control )
			{
				return -1;
			};
			void TimePicker::set_ZIndex( XGCRef* , size_t )
			{
			};
			float TimePicker::get_ScrollYOffset( void )
			{
				return 0;
			};
			void TimePicker::UpdateParentWindow( void )
			{
				_parent_window = get_ParentWindow();
			};
			void TimePicker::FreeResources( ::rux::byte from_removed_controls_schedule )
			{
				_time_textbox()->BeforeFreeSystemResources();
				_time_textbox()->FreeResources( from_removed_controls_schedule );
				_time_textbox()->FreeSystemResources();
				_current_render_index = SIZE_MAX;
			};
			void TimePicker::raise_OnLeftMouseButtonDoubleClick( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_time_textbox()->raise_OnLeftMouseButtonDoubleClick( window_event , explicit_event );
			};
			::rux::byte TimePicker::raise_OnMouseEnter( ::rux::gui::WindowMouseEvent* , ::rux::byte& raised_event , ::rux::byte explicit_event )
			{
				_on_mouse_enter.raise< const ::rux::gui::events::Event& >( ::rux::gui::events::Event( *this , explicit_event ) );
				return 1;
			};
			implement_empty_content_size( TimePicker );
			void TimePicker::raise_OnMouseLeave( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{	
				_on_mouse_leave_callback.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			::rux::byte TimePicker::Exists( ::rux::gui::CommonBase* control )
			{
				return 0;
			};
			::rux::uint64 XTimePicker::get_DateTime( void )
			{
				return (*this)()->_posix_time;
			};
			void TimePicker::raise_OnKeyChar( const ::rux::XString& symbol , ::rux::uint8 shift , ::rux::uint8 control , ::rux::uint8 alt )
			{		
				_time_textbox()->raise_OnKeyChar( symbol , shift , control , alt );
			};
			void TimePicker::raise_OnKeyDown( ::rux::gui::XEnum_Keys key , ::rux::uint8 shift , ::rux::uint8 control , ::rux::uint8 alt , ::rux::uint32 native_value , ::rux::byte explicit_event )
			{
				_time_textbox()->raise_OnKeyDown( key , shift , control , alt , native_value , explicit_event );
			};
			void TimePicker::raise_OnRightMouseButtonDown( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_right_mouse_button_down.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void TimePicker::raise_OnRightMouseButtonDoubleClick( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_right_mouse_button_double_click.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void TimePicker::raise_OnRightMouseButtonUp( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_right_mouse_button_up.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void TimePicker::raise_OnLeftMouseButtonDown( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{	
				_on_left_mouse_button_down.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , 1 ) );
				_time_textbox()->raise_OnLeftMouseButtonDown( window_event , 1 );
			};
			void TimePicker::raise_OnLeftMouseButtonUp( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{	
				_on_left_mouse_button_up.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
				_time_textbox()->raise_OnLeftMouseButtonUp( window_event , explicit_event );
			};
			void TimePicker::Invalidate( void )
			{	
				_time_textbox()->Invalidate();
			};
			void TimePicker::raise_OnMouseMove( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{	
				_time_textbox()->raise_OnMouseMove( window_event , explicit_event );
			};
			void XTimePicker::set_DateTime( uint64 unix_time )
			{
				(*this)()->set_DateTime( unix_time );
			};
			void TimePicker::set_DateTime( uint64 unix_time )
			{
				if( _posix_time != unix_time )
				{
					_posix_time = unix_time;

					::booldog::allocators::easy::heap heap;
					::booldog::allocators::single_threaded::mixed< 512 > mixed( &heap );
					{
						::booldog::result_mbchar mbchar( &mixed );

						::booldog::utils::time::posix::mbs::tostring< 16 >( &mbchar , mbchar.mballocator , "%H:%M:%S" 
							, ::booldog::utils::time::posix::tolocal( _posix_time ) , debuginfo_macros );

						::rux::XString time( mbchar.mbchar );	
						_time_textbox()->_lines.set_ItemByRef( 0 , time );
						_time_textbox()->private_ResetCache();
						_on_datetime_changed_event_callback.raise< const ::rux::gui::events::Event& >( ::rux::gui::events::Event( *this , 1 ) );
					}
				}
			};
			void XTimePicker::set_DateTimeWithoutEvent( uint64 unix_time )
			{
				(*this)()->set_DateTimeWithoutEvent( unix_time );
			};
			void TimePicker::set_DateTimeWithoutEvent( uint64 unix_time )
			{
				if( _posix_time != unix_time )
				{
					_posix_time = unix_time;

					::booldog::allocators::easy::heap heap;
					::booldog::allocators::single_threaded::mixed< 512 > mixed( &heap );
					{
						::booldog::result_mbchar mbchar( &mixed );

						::booldog::utils::time::posix::mbs::tostring< 16 >( &mbchar , mbchar.mballocator , "%H:%M:%S" 
							, ::booldog::utils::time::posix::tolocal( _posix_time ) , debuginfo_macros );

						::rux::XString time( mbchar.mbchar );	
						_time_textbox()->_lines.set_ItemByRef( 0 , time );
						_time_textbox()->private_ResetCache();
					}
				}
			};
			void TimePicker::raise_OnActivate( void )
			{		
				_time_textbox()->raise_OnActivate();
			};
			void TimePicker::raise_OnDeactivate( ::rux::gui::CommonBase* activated_control )
			{	
				_time_textbox()->raise_OnDeactivate( activated_control );
				XObject activated;
				if( activated_control )
					activated.set_ByRef( XObject( activated_control , 1 ) );
				_on_deactivated.raise< const XObject& , const XObject& >( XObject( *this ) , activated );
			};
			void* TimePicker::DynamicCast( ::rux::int32 class_type ) const
			{
				if( class_type == ::rux::gui::XEnum_ClassType_ControlBase )
				{
					::rux::gui::ControlBase* ptr = const_cast< TimePicker* >( this );
					return ptr;
				}
				else if( class_type == ::rux::gui::XEnum_ClassType_ParentBase )
				{
					::rux::gui::ParentBase* ptr = const_cast< TimePicker* >( this );
					return ptr;
				}
				return 0;
			};
			::rux::uint8 TimePicker::get_IsGroup( void )
			{
				return 0;
			};
			void TimePicker::private_ResetChildLocationAndSizeCache( ::rux::uint8 reset , ::rux::gui::ControlBase* control , ::rux::uint8 left , ::rux::uint8 top , ::rux::uint8 width , ::rux::uint8 height , size_t )
			{
				_cs_drawing_elements.wlock( debuginfo_macros );
				WRITE_LOCK( _time_textbox()->_cs_parent_control );
				_time_textbox()->_parent_control = this;
				_time_textbox()->_cs_parent_control.WriteUnlock();
				_time_textbox()->_parent_window = get_ParentWindow();
				_time_textbox()->private_ResetLocationAndSizeCache( 1 , 1 , 1 , 1 , 0 );
				_cs_drawing_elements.wunlock( debuginfo_macros );
				Invalidate();				
			};
			::rux::uint8 TimePicker::get_IsTimePicker( void )
			{
				return 1;
			};
			::rux::uint8 TimePicker::get_IsButton( void )
			{
				return 0;
			};
			void TimePicker::on_text_changed( const ::rux::gui::events::Event& event )
			{
				rux::gui::controls::XTextBox textbox( event._sender );
				TimePicker* timepicker = (TimePicker*)textbox.get_TagPtr();
				rux::XString time_string;
				time_string.set_ByRef( textbox.get_Text() );
				XArray< ::rux::XString > splits;
				splits.set_ByRef( time_string.Split( ':' ) );
				if( splits.Count() == 3 )
				{
					::booldog::uint64 loctime = ::booldog::utils::time::posix::tolocal( timepicker->_posix_time );

					::booldog::uint32 year = 0 , month = 0 , day_of_month = 0;
					::booldog::utils::time::posix::date( loctime , year , month , day_of_month );
					::booldog::uint32 hour = 0 , minute = 0 , second = 0 , millisecond = 0;
					::booldog::utils::time::posix::time( loctime , hour , minute , second , millisecond );
					
					loctime = ::booldog::utils::time::posix::generate( millisecond , splits[ 2 ].ToUInt32() 
						, splits[ 1 ].ToUInt32() , splits[ 0 ].ToUInt32() , day_of_month , month , year );
					timepicker->set_DateTime( ::booldog::utils::time::posix::toutc( loctime ) );
				}
			};
			void TimePicker::raise_OnMouseWheel( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				if( ( _time_textbox()->_old_selection_char_index == 0
					&& _time_textbox()->_selection_char_index == 2 )
					|| ( _time_textbox()->_old_selection_char_index == 2
					&& _time_textbox()->_selection_char_index == 0 ) )
				{		
					if( window_event->_delta > 0 )
						set_DateTime( ::booldog::utils::time::posix::add_hours( _posix_time , 1 ) );
					else if( window_event->_delta < 0 )
						set_DateTime( ::booldog::utils::time::posix::sub_hours( _posix_time , 1 ) );
				}
				else if( ( _time_textbox()->_old_selection_char_index == 3
					&& _time_textbox()->_selection_char_index == 5 )
					|| ( _time_textbox()->_old_selection_char_index == 5
					&& _time_textbox()->_selection_char_index == 3 ) )
				{	
					if( window_event->_delta > 0 )
						set_DateTime( ::booldog::utils::time::posix::add_minutes( _posix_time , 1 ) );
					else if( window_event->_delta < 0 )
						set_DateTime( ::booldog::utils::time::posix::sub_minutes( _posix_time , 1 ) );
				}
				else if( ( _time_textbox()->_old_selection_char_index == 6
					&& _time_textbox()->_selection_char_index == 8 )
					|| ( _time_textbox()->_old_selection_char_index == 8
					&& _time_textbox()->_selection_char_index == 6 ) )
				{		
					if( window_event->_delta > 0 )
						set_DateTime( ::booldog::utils::time::posix::add_seconds( _posix_time , 1 ) );
					else if( window_event->_delta < 0 )
						set_DateTime( ::booldog::utils::time::posix::sub_seconds( _posix_time , 1 ) );
				}
			};
			void XTimePicker::get_Clip( ::rux::gui::Rectangle*& clip )
			{
				(*this)()->get_Clip( clip );
			};
			void TimePicker::RemoveControl( XGCRef* )
			{
			};
			void TimePicker::RemoveTabControls( void )
			{
			};
			void TimePicker::AddTabControls( void )
			{
			};
			void TimePicker::get_Clip( ::rux::gui::Rectangle*& clip , ::rux::uint8 )
			{
				::rux::gui::Window* window = get_ParentWindow();
				if( window && _current_render_index != window->_render_index )
				{					
					float left = get_Left();
					float top = get_Top();
					float width = get_Width();
					float height = get_Height();
					READ_LOCK( _cs_parent_control );
					::rux::gui::ParentBase* parent_copy = _parent_control;
					_cs_parent_control.ReadUnlock();
					if( parent_copy )
					{
						if( parent_copy->get_IsGroup() || parent_copy->get_IsButton() || parent_copy->get_IsTimePicker() )
						{
							::rux::gui::Rectangle* clip_ptr = &_clip;
							parent_copy->get_Clip( clip_ptr );
						}
						else
							_clip = ::rux::gui::Rectangle( left , top , width , height );
					}
					if( left < _clip._left )
					{
						width -= _clip._left - left;
						left = _clip._left;
					}
					if( top < _clip._top )
					{
						height -= _clip._top - top;
						top = _clip._top;
					}	
					if( _clip._left + _clip._width < left + width )
						width = _clip._left + _clip._width - left;	
					if( _clip._top + _clip._height < top + height )
						height = _clip._top + _clip._height - top;
					_clip._left = left , _clip._top = top , _clip._width = width , _clip._height = height;
					_current_render_index = window->_render_index;
				}
				if( clip )
					*clip = _clip;
				else
					clip = &_clip;
			};
			void TimePicker::render( ::rux::gui::RenderContextBase* render_context , float opacity , float& _selected_z_index , size_t ___rux__thread_index1986 )
			{	
				float width = get_Width();
				float height = get_Height();
				_cs_drawing_elements.wlock( debuginfo_macros );
				_time_textbox()->_left = 0;
				_time_textbox()->_top = 0;
				_time_textbox()->_width = width;
				_time_textbox()->_height = height;
				_time_textbox()->_cs_drawing_elements.wlock( debuginfo_macros );
				_time_textbox()->_font = _time_font;
				_time_textbox()->_cs_drawing_elements.wunlock( debuginfo_macros );
				WRITE_LOCK( _time_textbox()->_cs_parent_control );
				_time_textbox()->_parent_control = this;
				_time_textbox()->_cs_parent_control.WriteUnlock();
				_time_textbox()->_parent_window = get_ParentWindow();
				_time_textbox()->render( render_context , opacity * _opacity , _selected_z_index , ___rux__thread_index1986 );
				_cs_drawing_elements.wunlock( debuginfo_macros );
			};
			void TimePicker::private_ResetCache( void )
			{
				_time_textbox()->private_ResetCache();
			};
			::rux::uint8 TimePicker::get_IsTransparent( void )
			{
				return 0;
			};
			begin_implement_rux_base_ui_functions( TimePicker )
			end_implement_rux_base_ui_functions();
			IMPLEMENT_BASE_UI_FUNCTIONS( XTimePicker );
		};
	};
};