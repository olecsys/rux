#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_uislider.h>
#include <xan_uiinlines.h>
#include <xan_uibutton.h>
#include <xan_keyvaluepair.h>
begin_implement_rux_class_with_properties_ns_base_class( Slider , rux::gui::controls , rux::gui::ControlBase )
	_tag.set_Info( "tag" , __file__ , __line__ );
	_private_tag.set_Info( "private_tag" , __file__ , __line__ );
	IMPLEMENT_BASE_UI_MEMBERS();
	_thumb_thickness = ::rux::gui::Thickness( 0.2f , 0.2f , 0.2f , 0.2f );
	_value_step = 5.f;
	_value = 50.f;
	_min_value = 0.f;
	_max_value = 100.f;
	_track_width = 5.f;
	_thumb_height = 7.f;
	_thumb_width = 15.f;
	_orientation = XEnum_SliderOrientation_Vertical;
	copy_color( _transparent_color , ::rux::gui::Colors::Transparent() );
	copy_color( _track_background , ::rux::gui::Colors::SliderTrackNormalBackground() );
	copy_color( _thumb_background , ::rux::gui::Colors::VerticalSliderThumbColor() );
	copy_color( _thumb_over_background , ::rux::gui::Colors::VerticalSliderOverThumbColor() );
	copy_color( _thumb_dragging_background , ::rux::gui::Colors::VerticalSliderDraggingThumbColor() );
	rux::gui::copy_color( _thumb_active_background , _thumb_background );
	_on_ui_value_changed_callback = NULL;
	_is_raise = 0;
	_is_value_change_thread_inited = 0;
	_is_thumb_dragging = 0;
	_slider_mouse_click_value_change = XEnum_SliderMouseClickValueChange_Gradual;
end_implement_rux_class();
namespace rux
{
	namespace gui
	{
		namespace controls
		{
			Slider::~Slider()
			{
				FreeResources( 0 );
				CRITICAL_SECTION_LOCK( _cs_value_change_thread );
				if( _value_change_thread.get_IsStarted() == 1 )
					_value_change_thread.Stop();
				CRITICAL_SECTION_UNLOCK( _cs_value_change_thread );	
				if( _transparent_color )
					_transparent_color->Release();
				if( _track_background )
					_track_background->Release();
				if( _thumb_background )
					_thumb_background->Release();
				if( _thumb_over_background )
					_thumb_over_background->Release();
				if( _thumb_active_background )
					_thumb_active_background->Release();
				if( _thumb_dragging_background )
					_thumb_dragging_background->Release();
			};
			begin_implement_rux_base_ui_functions( Slider )
			end_implement_rux_base_ui_functions();
			void Slider::UpdateParentWindow( void )
			{
				_parent_window = get_ParentWindow();
			};
			void Slider::FreeResources( ::rux::byte )
			{
				::rux::engine::_globals->_gui_globals->_rux_gui_add_cache( _ui_cache );
				_ui_cache = NULL;
				::rux::engine::_globals->_gui_globals->_rux_gui_add_cache( _ui_border_cache );
				_ui_border_cache = NULL;
				::rux::engine::_globals->_gui_globals->_rux_gui_add_cache( _ui_thumb_border_cache );
				_ui_thumb_border_cache = NULL;
				::rux::engine::_globals->_gui_globals->_rux_gui_add_cache( _ui_thumb_cache );
				_ui_thumb_cache = NULL;
			};
			void Slider::raise_OnLeftMouseButtonDoubleClick( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
			};
			::rux::byte Slider::raise_OnMouseEnter( ::rux::gui::WindowMouseEvent* , ::rux::byte& raised_event , ::rux::byte explicit_event )
			{
				_on_mouse_enter.raise< const ::rux::gui::events::Event& >( ::rux::gui::events::Event( *this , explicit_event ) );
				return 1;
			};
			implement_empty_content_size( Slider );
			void Slider::raise_OnMouseLeave( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_mouse_leave_callback.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			float Slider::get_MaxValue( void )
			{
				return _max_value;
			};
			float Slider::get_MinValue( void )
			{
				return _min_value;
			};
			void Slider::set_ValueWithoutEvent( float value )
			{
				if( value >= _min_value && value <= _max_value )
				{		
					_value = value;
				}
			};
			void Slider::set_Value( float value )
			{
				if( value >= _min_value && value <= _max_value )
				{
					float old_value = _value;
					_value = value;
					_on_ui_value_changed_callback.raise< const XObject& >( XObject( *this ) , old_value , value );
				}
			};
			void Slider::raise_OnKeyChar( const ::rux::XString& , ::rux::uint8 , ::rux::uint8 , ::rux::uint8 )
			{		
			};
			void Slider::raise_OnKeyDown( ::rux::gui::XEnum_Keys , ::rux::uint8 , ::rux::uint8 , ::rux::uint8 , ::rux::uint32 , ::rux::byte explicit_event )
			{
			};
			void Slider::raise_OnRightMouseButtonDown( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_right_mouse_button_down.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void Slider::raise_OnRightMouseButtonDoubleClick( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_right_mouse_button_double_click.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void Slider::raise_OnRightMouseButtonUp( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_right_mouse_button_up.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void Slider::raise_OnMouseWheelDown(::rux::gui::WindowMouseEvent* window_event, ::rux::byte explicit_event)
			{
				_on_mouse_wheel_down.raise< const ::rux::gui::events::MouseEvent& >(
					::rux::gui::events::MouseEvent(*this, window_event, explicit_event));
			};
			void Slider::raise_OnMouseWheelDoubleClick(::rux::gui::WindowMouseEvent* window_event
				, ::rux::byte explicit_event)
			{
				_on_mouse_wheel_double_click.raise< const ::rux::gui::events::MouseEvent& >(
					::rux::gui::events::MouseEvent(*this, window_event, explicit_event));
			};
			void Slider::raise_OnMouseWheelUp(::rux::gui::WindowMouseEvent* window_event, ::rux::byte explicit_event)
			{
				_on_mouse_wheel_up.raise< const ::rux::gui::events::MouseEvent& >(
					::rux::gui::events::MouseEvent(*this, window_event, explicit_event));
			};
			void Slider::raise_OnLeftMouseButtonDown( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_left_mouse_button_down.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , 1 ) );
				::rux::uint8 left = 0 , above = 0;
				if( IsPointInThumb( window_event->_x , window_event->_y , left , above ) == 1 )
				{
					private_set_ActiveThumbBackground( _thumb_dragging_background );
					_is_thumb_dragging = 1;
				}
				else if( _slider_mouse_click_value_change == XEnum_SliderMouseClickValueChange_Gradual )
				{
					if( _orientation == XEnum_SliderOrientation_Vertical )
					{
						if( above == 1 )
						{
							_is_raise = 1;
							float value = _min_value;
							while( value <= _value )
							{
								value += _value_step;
							}
							set_Value( value );
						}
						else
						{
							_is_raise = 0;
							float value = _max_value;
							while( value >= _value )
							{
								value -= _value_step;
							}
							set_Value( value );
						}
					}
					else if( _orientation == XEnum_SliderOrientation_Horizontal )
					{
						if( left == 1 )
						{
							_is_raise = 0;
							float value = _max_value;
							while( value >= _value )
							{
								value -= _value_step;
							}
							set_Value( value );			
						}
						else
						{
							_is_raise = 1;
							float value = _min_value;
							while( value <= _value )
							{
								value += _value_step;
							}
							set_Value( value );
						}
					}
					CRITICAL_SECTION_LOCK( _cs_value_change_thread );		
					if( _value_change_thread.get_IsStarted() == 0 )
					{
						_value_change_thread.set_ThreadMode( XThreadMode_Repeat );
						_value_change_thread.set_Timeout( 200 );
						_is_value_change_thread_inited = 0;			
						_value_change_thread.Start();			
						_value_change_thread.Push( value_change , this );
					}		
					CRITICAL_SECTION_UNLOCK( _cs_value_change_thread );
					private_set_ActiveThumbBackground( _thumb_background );
				}
				else if( _slider_mouse_click_value_change == XEnum_SliderMouseClickValueChange_Instant )
				{
					if( _orientation == XEnum_SliderOrientation_Vertical )
					{			
						float track_height = get_Height() - ( _thumb_height / 2.f + 5.f );			
						float track_top = get_Top() + ( get_Height() - track_height ) / 2.f;
						float value = 0.f;
						if( window_event->_y <= track_top )
							value = _max_value;
						if( window_event->_y >= track_top + track_height )
							value = _min_value;
						else
							value = _min_value + ( ( track_top + track_height - window_event->_y ) / track_height ) * ( _max_value - _min_value );
						set_Value( value );
					}
					else if( _orientation == XEnum_SliderOrientation_Horizontal )
					{			
						float track_width = get_Width() - ( _thumb_width / 2.f + 5.f );
						float track_left = get_Left() + ( get_Width() - track_width ) / 2.f;			
						float value = 0.f;
						if( window_event->_x <= track_left )
							value = _min_value;
						if( window_event->_x >= track_left + track_width )
							value = _max_value;
						else
							value = _min_value + ( ( window_event->_x - track_left ) / track_width ) * ( _max_value - _min_value );
						set_Value( value );
					}		
				}
			};
			void Slider::raise_OnLeftMouseButtonUp( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_left_mouse_button_up.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
				_is_thumb_dragging = 0;
				CRITICAL_SECTION_LOCK( _cs_value_change_thread );
				if( _value_change_thread.get_IsStarted() == 1 )
					_value_change_thread.Stop();
				CRITICAL_SECTION_UNLOCK( _cs_value_change_thread );
				if( IsPointInThumb( window_event->_x , window_event->_y ) == 1 )
					private_set_ActiveThumbBackground( _thumb_over_background );
				else
					private_set_ActiveThumbBackground( _thumb_background );
			};
			void Slider::raise_OnMouseMove( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				if( _is_thumb_dragging == 1 )
				{
					::rux::uint8 left = 0 , above = 0;
					float x_offset = 0.f , y_offset = 0.f;
					if( IsPointInTrack( window_event->_x , window_event->_y , left , above , x_offset , y_offset ) == 1 )
					{
						float value0 = 0.f;
						if( _orientation == XEnum_SliderOrientation_Vertical )
							value0 = _min_value + ( _max_value - _min_value ) * y_offset;
						else if( _orientation == XEnum_SliderOrientation_Horizontal )
							value0 = _min_value + ( _max_value - _min_value ) * x_offset;
						float value1 = _min_value;
						while( value1 < value0 )
						{
							value1 += _value_step;
						}
						set_Value( value1 );
					}
					else if( _orientation == XEnum_SliderOrientation_Vertical )
					{
						if( above == 1 )
							set_Value( _max_value );
						else
							set_Value( _min_value );
					}
					else if( _orientation == XEnum_SliderOrientation_Horizontal )
					{
						if( left == 1 )
							set_Value( _min_value );
						else
							set_Value( _max_value );
					}
					private_set_ActiveThumbBackground( _thumb_dragging_background );		
				}
				else if( IsPointInThumb( window_event->_x , window_event->_y ) == 1 )
					private_set_ActiveThumbBackground( _thumb_over_background );
				else
					private_set_ActiveThumbBackground( _thumb_background );
			};
			void Slider::raise_OnMouseWheel( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				if( window_event->_delta > 0 )
				{
					float value = _min_value;
					while( value <= _value )
					{
						value += _value_step;
					}
					set_Value( value );	
				}
				else if( window_event->_delta < 0 )
				{
					float value = _max_value;
					while( value >= _value )
					{
						value -= _value_step;
					}
					set_Value( value );
				}
			};
			void* Slider::DynamicCast( ::rux::int32 class_type ) const
			{
				if( class_type == ::rux::gui::XEnum_ClassType_ControlBase )
				{
					::rux::gui::ControlBase* ptr = const_cast< Slider* >( this );
					return ptr;
				}
				return 0;
			};
			::rux::uint8 Slider::get_IsGroup( void )
			{
				return 0;
			};
			::rux::uint8 Slider::get_IsTimePicker( void )
			{
				return 0;
			};
			::rux::uint8 Slider::get_IsButton( void )
			{
				return 0;
			};
			void Slider::render( ::rux::gui::RenderContextBase* render_context , float opacity , float& _selected_z_index , size_t ___rux__thread_index1986 )
			{		
				float track_width = 0.f , track_height = 0.f;
				if( _orientation == XEnum_SliderOrientation_Vertical )
				{
					track_width = _track_width;
					if( track_width > get_Width() )
						track_width = get_Width();
					track_height = get_Height() - ( _thumb_height / 2.f + 5.f );
				}
				else if( _orientation == XEnum_SliderOrientation_Horizontal )
				{
					track_height = _track_width;
					if( track_height > get_Height() )
						track_height = get_Height();
					track_width = get_Width() - ( _thumb_width / 2.f + 5.f );
				}
				float track_left = get_Left() + ( get_Width() - track_width ) / 2.f;
				float track_top = get_Top() + ( get_Height() - track_height ) / 2.f;
				_cs_drawing_elements.wlock( debuginfo_macros );				
				render_context->DrawRectangle( ::rux::math::round( track_left , 0 ) , ::rux::math::round( track_top , 0 )
					, ::rux::math::round( track_width , 0 ) , ::rux::math::round( track_height , 0 ) , 0 , _track_background 
					, _transparent_color , ::rux::gui::Thickness( 0 , 0 , 0 , 0 ) , _opacity * opacity , &_ui_border_cache 
					, &_ui_cache , _selected_z_index , 1 , __FILE__ , __LINE__ , ___rux__thread_index1986 );
				_cs_drawing_elements.wunlock( debuginfo_macros );
				float offset = (_value - _min_value ) / ( _max_value - _min_value );
				float thumb_left = 0.f;
				float thumb_top = 0.f;
				if( _orientation == XEnum_SliderOrientation_Vertical )
				{
					offset *= track_height;
					thumb_top = ( track_top + track_height - offset ) - ( _thumb_height / 2.f );
					thumb_left = get_Left() + ( get_Width() - _thumb_width ) / 2.f;
				}
				else if( _orientation == XEnum_SliderOrientation_Horizontal )
				{
					offset *= track_width;
					thumb_left = ( track_left + offset ) - ( _thumb_width / 2.f );
					thumb_top = get_Top() + ( get_Height() - _thumb_height ) / 2.f;
				}
				_selected_z_index += 1.f;//0.001f
				_cs_drawing_elements.wlock( debuginfo_macros );
				render_context->DrawRectangle( ::rux::math::round( thumb_left , 0 ) , ::rux::math::round( thumb_top , 0 ) 
					, _thumb_width , _thumb_height , 0 , _thumb_active_background , _transparent_color , _thumb_thickness 
					, _opacity * opacity , &_ui_thumb_border_cache , &_ui_thumb_cache , _selected_z_index , 1 , __FILE__ 
					, __LINE__ , ___rux__thread_index1986 );
				_cs_drawing_elements.wunlock( debuginfo_macros );
			};
			::rux::uint8 Slider::IsPointInTrack( ::rux::int16 x , ::rux::int16 y , ::rux::uint8& left , ::rux::uint8& above , float& x_offset , float& y_offset )
			{
				float track_width = 0.f , track_height = 0.f;
				if( _orientation == XEnum_SliderOrientation_Vertical )
				{
					track_width = _track_width;
					if( track_width > get_Width() )
						track_width = get_Width();
					track_height = get_Height() - ( _thumb_height / 2.f + 5.f );
				}
				else if( _orientation == XEnum_SliderOrientation_Horizontal )
				{
					track_height = _track_width;
					if( track_height > get_Height() )
						track_height = get_Height();
					track_width = get_Width() - ( _thumb_width / 2.f + 5.f );
				}
				float track_left = get_Left() + ( get_Width() - track_width ) / 2.f;
				float track_top = get_Top() + ( get_Height() - track_height ) / 2.f;		
				if( _orientation == XEnum_SliderOrientation_Vertical )
				{
					if( y >= track_top && y <= track_top + track_height )
					{
						y_offset = ( track_top + track_height - y ) / track_height;
						return 1;
					}
					else
					{
						if( y < track_top )
							above = 1;
						else
							above = 0;
						return 0;
					}
				}
				else if( _orientation == XEnum_SliderOrientation_Horizontal )
				{
					if( x >= track_left && x <= track_left + track_width )
					{
						x_offset = ( x - track_left ) / track_width;
						return 1;
					}
					else
					{
						if( x < track_left )
							left = 1;
						else
							left = 0;
						return 0;
					}
				}
				else
					return 0;
			};
			::rux::uint8 Slider::IsPointInThumb( ::rux::int16 x , ::rux::int16 y , ::rux::uint8& left , ::rux::uint8& above )
			{
				float track_width = 0.f , track_height = 0.f;
				if( _orientation == XEnum_SliderOrientation_Vertical )
				{
					track_width = _track_width;
					if( track_width > get_Width() )
						track_width = get_Width();
					track_height = get_Height() - ( _thumb_height / 2.f + 5.f );
				}
				else if( _orientation == XEnum_SliderOrientation_Horizontal )
				{
					track_height = _track_width;
					if( track_height > get_Height() )
						track_height = get_Height();
					track_width = get_Width() - ( _thumb_width / 2.f + 5.f );
				}
				float track_left = get_Left() + ( get_Width() - track_width ) / 2.f;
				float track_top = get_Top() + ( get_Height() - track_height ) / 2.f;	
				float offset = ( _value - _min_value ) / ( _max_value - _min_value );
				float thumb_left = 0.f;
				float thumb_top = 0.f;
				if( _orientation == XEnum_SliderOrientation_Vertical )
				{
					offset *= track_height;
					thumb_top = ( track_top + track_height - offset ) - ( _thumb_height / 2.f );
					thumb_left = get_Left() + ( get_Width() - _thumb_width ) / 2.f;
				}
				else if( _orientation == XEnum_SliderOrientation_Horizontal )
				{
					offset *= track_width;
					thumb_left = ( track_left + offset ) - ( _thumb_width / 2.f );
					thumb_top = get_Top() + ( get_Height() - _thumb_height ) / 2.f;
				}
				if( x >= thumb_left && x <= thumb_left + _thumb_width
					&& y >= thumb_top && y <= thumb_top + _thumb_height )
					return 1;
				else
				{
					if( x < thumb_left )
						left = 1;
					else
						left = 0;
					if( y < thumb_top )
						above = 1;
					else
						above = 0;
					return 0;
				}
			};
			::rux::uint8 Slider::IsPointInThumb( ::rux::int16 x , ::rux::int16 y )
			{
				float track_width = 0.f , track_height = 0.f;
				if( _orientation == XEnum_SliderOrientation_Vertical )
				{
					track_width = _track_width;
					if( track_width > get_Width() )
						track_width = get_Width();
					track_height = get_Height() -  ( _thumb_height / 2.f + 5.f );
				}
				else if( _orientation == XEnum_SliderOrientation_Horizontal )
				{
					track_height = _track_width;
					if( track_height > get_Height() )
						track_height = get_Height();
					track_width = get_Width() - ( _thumb_width / 2.f + 5.f );
				}
				float track_left = get_Left() + ( get_Width() - track_width ) / 2.f;
				float track_top = get_Top() + ( get_Height() - track_height ) / 2.f;	
				float offset = ( _value - _min_value ) / ( _max_value - _min_value );
				float thumb_left = 0.f;
				float thumb_top = 0.f;
				if( _orientation == XEnum_SliderOrientation_Vertical )
				{
					offset *= track_height;
					thumb_top = ( track_top + track_height - offset ) - ( _thumb_height / 2.f );
					thumb_left = get_Left() + ( get_Width() - _thumb_width ) / 2.f;
				}
				else if( _orientation == XEnum_SliderOrientation_Horizontal )
				{
					offset *= track_width;
					thumb_left = ( track_left + offset ) - ( _thumb_width / 2.f );
					thumb_top = get_Top() + ( get_Height() - _thumb_height ) / 2.f;
				}
				if( x >= thumb_left && x <= thumb_left + _thumb_width
					&& y >= thumb_top && y <= thumb_top + _thumb_height )
					return 1;
				else
					return 0;
			};
			void Slider::private_ResetCache( void )
			{
			};
			::rux::uint8 Slider::get_IsTransparent( void )
			{
				return 0;
			};
			void Slider::Invalidate( void )
			{	
			};
			void XSlider::set_OnValueChanged( ::rux::gui::events::on_value_changed_t on_ui_value_changed_callback )
			{
				(*this)()->_on_ui_value_changed_callback = on_ui_value_changed_callback;
			};
			void Slider::private_set_ActiveThumbBackground( ::rux::gui::ColorBase* background )
			{				
				_cs_drawing_elements.wlock( debuginfo_macros );
				rux::gui::copy_color( _thumb_active_background , background );
				_cs_drawing_elements.wunlock( debuginfo_macros );
			};
			void Slider::raise_OnActivate( void )
			{		
			};
			void Slider::raise_OnDeactivate( ::rux::gui::CommonBase* activated_control )
			{		
				XObject activated;
				if( activated_control )
					activated.set_ByRef( XObject( activated_control , 1 ) );
				_on_deactivated.raise< const XObject& , const XObject& >( XObject( *this ) , activated );
			};
			void Slider::value_change( void* param , size_t ___rux__thread_index1986 )
			{	
				UNREFERENCED_PARAMETER( ___rux__thread_index1986 );
				Slider* slider = (Slider*)param;
				if( slider )
				{		
					if( slider->_value_change_thread.get_IsWaitForExit() == 0 )
					{
						if( slider->_is_value_change_thread_inited == 0 )
							slider->_is_value_change_thread_inited = 1;
						else if( slider->_is_raise == 0 )
						{
							float value = slider->_max_value;
							while( value >= slider->_value )
							{
								value -= slider->_value_step;
							}
							slider->set_Value( value );			
						}
						else
						{
							float value = slider->_min_value;
							while( value <= slider->_value )
							{
								value += slider->_value_step;
							}
							slider->set_Value( value );
						}
					}
				}
			};
			void XSlider::set_ValueWithoutEvent( float value )
			{
				(*this)()->set_ValueWithoutEvent( value );
			};
			void XSlider::set_Value( float value )
			{
				(*this)()->set_Value( value );
			};
			void XSlider::set_ValueStep( float value )
			{
				if( (*this)()->_value_step != value && value < (*this)()->_max_value )
					(*this)()->_value_step = value;
			};
			void XSlider::set_TrackWidth( float track_width )
			{
				if( (*this)()->_track_width != track_width )
				{
					(*this)()->_track_width = track_width;
					(*this)()->Invalidate();
				}
			};
			void XSlider::set_ThumbWidth( float thumb_width )
			{
				if( (*this)()->_thumb_width != thumb_width )
				{
					(*this)()->_thumb_width = ::rux::math::round( thumb_width , 0 );
					(*this)()->Invalidate();
				}
			};
			void XSlider::set_ThumbHeight( float thumb_height )
			{
				if( (*this)()->_thumb_height != thumb_height )
				{
					(*this)()->_thumb_height = ::rux::math::round( thumb_height , 0 );
					(*this)()->Invalidate();
				}
			};
			void XSlider::set_MaxValue( float max_value )
			{
				if( (*this)()->_max_value != max_value && max_value > (*this)()->_min_value )
				{
					(*this)()->_max_value = max_value;
					if( (*this)()->_value > max_value )
						(*this)()->_value = max_value;
					(*this)()->Invalidate();
				}
			};
			void XSlider::set_MinValue( float min_value )
			{
				if( (*this)()->_min_value != min_value && min_value < (*this)()->_max_value )
				{
					(*this)()->_min_value = min_value;
					if( (*this)()->_value < min_value )
						(*this)()->_value = min_value;
					(*this)()->Invalidate();
				}
			};
			void XSlider::set_Orientation( XEnum_SliderOrientation orientation )
			{
				if( (*this)()->_orientation != orientation )
				{
					(*this)()->_orientation = orientation;
					(*this)()->Invalidate();
				}
			};
			void XSlider::set_TrackBackground( ::rux::gui::ColorBase* track_background )
			{
				(*this)()->_cs_drawing_elements.wlock( debuginfo_macros );
				rux::gui::copy_color( (*this)()->_track_background , track_background );
				(*this)()->_cs_drawing_elements.wunlock( debuginfo_macros );
				(*this)()->Invalidate();
			};
			void XSlider::set_ThumbBackground( ::rux::gui::ColorBase* thumb_background )
			{
				rux::gui::copy_color( (*this)()->_thumb_background , thumb_background );
				(*this)()->Invalidate();
			};
			void XSlider::set_ThumbOverBackground( ::rux::gui::ColorBase* thumb_over_background )
			{
				rux::gui::copy_color( (*this)()->_thumb_over_background , thumb_over_background );
				(*this)()->Invalidate();
			};
			void XSlider::set_ThumbActiveBackground( ::rux::gui::ColorBase* thumb_active_background )
			{
				(*this)()->_cs_drawing_elements.wlock( debuginfo_macros );
				rux::gui::copy_color( (*this)()->_thumb_active_background , thumb_active_background );
				(*this)()->_cs_drawing_elements.wunlock( debuginfo_macros );
				(*this)()->Invalidate();
			};
			void XSlider::set_ThumbDraggingBackground( ::rux::gui::ColorBase* thumb_dragging_background )
			{
				rux::gui::copy_color( (*this)()->_thumb_dragging_background , thumb_dragging_background );
				(*this)()->Invalidate();
			};
			void XSlider::set_ThumbThickness( const ::rux::gui::Thickness& thumb_thickness )
			{
				(*this)()->_thumb_thickness = thumb_thickness;
				(*this)()->Invalidate();
			};
			void XSlider::set_MouseClickValueChangeType( XEnum_SliderMouseClickValueChange slider_mouse_click_value_change )
			{
				(*this)()->_slider_mouse_click_value_change = slider_mouse_click_value_change;
			};
			float XSlider::get_Value( void )
			{
				return (*this)()->_value;
			};
			IMPLEMENT_BASE_UI_FUNCTIONS( XSlider );
		};
	};
};