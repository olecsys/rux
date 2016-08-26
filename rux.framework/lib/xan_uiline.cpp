#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_uiline.h>
#include <xan_window.h>
#include <xan_uigroup.h>
#include <xan_uibutton.h>
#include <xan_keyvaluepair.h>
#include <xan_uiinlines.h>
#include <xan_math.h>
begin_implement_rux_class_with_properties_ns_base_class( Line , rux::gui::controls , rux::gui::ControlBase )
	_tag.set_Info( "tag" , __file__ , __line__ );
	_private_tag.set_Info( "private_tag" , __file__ , __line__ );
	_render_thread_id = 0;
	memset( _utf8_control_name , 0 , 128 );
	_utf8_control_name_hash = 0;
	_parent_control = NULL;
	_parent_window = 0;
	_free_system_resources_accomplished = 1;
	_free_system_resources_thread_id = 0;
	_tag_ptr = NULL;
	_is_forward_events = 0;
	_start_left = 20;
	_start_top = 20;
	_end_left = 20;
	_end_top = 20;
	_thickness = 0.5;
	_point_unit = ::rux::gui::XEnum_PointUnit_Pixel;
	_opacity = 1.0;
	_is_visible = 1;
	_is_mouse_entered = 0;
	_bottom_sticker = FLT_MAX;
	_right_sticker = FLT_MAX;
	_tab_index = -1;				
	_cursor = ::rux::gui::XEnum_Cursors_Arrow;
	_vertical_filling = XEnum_Filling_Manual;
	_horizontal_filling = XEnum_Filling_Manual;
	_ui_line_cache = NULL;
	_ui_cap1_cache = NULL;
	_ui_cap2_cache = NULL;
	_already_dispatch_on_resized = 0;
	_already_dispatch_on_relocated = 0;
	_update_polygon = 0;
end_implement_rux_class();
namespace rux
{
	namespace gui
	{
		namespace controls
		{
			Line::~Line()
			{	
				FreeResources( 0 );
				if( _color )
					_color->Release();
			};
			float Line::get_StartLeft( void )
			{
				return _start_left;
			};
			float Line::get_StartTop( void )
			{
				return _start_top;
			};
			float Line::get_EndLeft( void )
			{
				return _end_left;
			};
			float Line::get_EndTop( void )
			{
				return _end_top;
			};
			XGCRef& Line::static_get_ZIndex( const XGCRef* object )
			{
				Line* obj = (Line*)object->get_ObjectAddress();
				return ::rux::XInt32( obj->get_ZIndex() )++;
			};
			void Line::static_set_ZIndex( const XGCRef* object , const XGCRef* value )
			{
				Line* obj = (Line*)object->get_ObjectAddress();
				obj->set_ZIndex( ::rux::gcref_to_int32( value ) );
			};
			XGCRef& Line::static_get_StartLeft( const XGCRef* object )
			{
				Line* obj = (Line*)object->get_ObjectAddress();
				return ::rux::XFloat( obj->get_StartLeft() )++;
			};
			void Line::static_set_StartLeft( const XGCRef* object , const XGCRef* value )
			{
				Line* obj = (Line*)object->get_ObjectAddress();
				obj->set_StartLeft( ::rux::gcref_to_float( value ) );
			};
			XGCRef& Line::static_get_StartTop( const XGCRef* object )
			{
				Line* obj = (Line*)object->get_ObjectAddress();
				return ::rux::XFloat( obj->get_StartTop() )++;
			};
			void Line::static_set_StartTop( const XGCRef* object , const XGCRef* value )
			{
				Line* obj = (Line*)object->get_ObjectAddress();
				obj->set_StartTop( ::rux::gcref_to_float( value ) );
			};
			XGCRef& Line::static_get_EndLeft( const XGCRef* object )
			{
				Line* obj = (Line*)object->get_ObjectAddress();
				return ::rux::XFloat( obj->get_EndLeft() )++;
			};
			void Line::static_set_EndLeft( const XGCRef* object , const XGCRef* value )
			{
				Line* obj = (Line*)object->get_ObjectAddress();
				obj->set_EndLeft( ::rux::gcref_to_float( value ) );
			};
			XGCRef& Line::static_get_EndTop( const XGCRef* object )
			{
				Line* obj = (Line*)object->get_ObjectAddress();
				return ::rux::XFloat( obj->get_EndTop() )++;
			};
			void Line::static_set_EndTop( const XGCRef* object , const XGCRef* value )
			{
				Line* obj = (Line*)object->get_ObjectAddress();
				obj->set_EndTop( ::rux::gcref_to_float( value ) );
			};
			implement_duplicate_internal_function_1( Line , set_Color , ::rux::gui::ColorBase* );
			void* Line::DynamicCast( ::rux::int32 class_type ) const
			{
				if( class_type == ::rux::gui::XEnum_ClassType_ControlBase )
				{
					::rux::gui::ControlBase* ptr = const_cast< Line* >( this );
					return ptr;
				}
				return 0;
			};
			::rux::uint8 Line::get_IsGroup( void )
			{
				return 0;
			};
			::rux::uint8 Line::get_IsTimePicker( void )
			{
				return 0;
			};
			::rux::uint8 Line::get_IsButton( void )
			{
				return 0;
			};
			float Line::get_ClipLeft( void )
			{
				if( _render_thread_id == 0 ||_render_thread_id == ::rux::threading::XThread::get_CurrentThreadId() )
					update_polygon();
				if( _cache_start_left > _cache_end_left )
					return _cache_end_left - 1;
				else
					return _cache_start_left - 1;
			};
			float Line::get_ClipTop( void )
			{
				if( _render_thread_id == 0 ||_render_thread_id == ::rux::threading::XThread::get_CurrentThreadId() )
					update_polygon();
				if( _cache_start_top > _cache_end_top )
					return _cache_end_top - 1;
				else
					return _cache_start_top - 1;
			};
			float Line::get_ClipWidth( void )
			{
				if( _render_thread_id == 0 ||_render_thread_id == ::rux::threading::XThread::get_CurrentThreadId() )
					update_polygon();
				if( _cache_start_left > _cache_end_left )
					return _cache_start_left - _cache_end_left + 2;
				else
					return _cache_end_left - _cache_start_left + 2;
			};
			float Line::get_ClipHeight( void )
			{		
				if( _render_thread_id == 0 ||_render_thread_id == ::rux::threading::XThread::get_CurrentThreadId() )
					update_polygon();
				if( _cache_start_top > _cache_end_top )
					return _cache_start_top - _cache_end_top + 2;
				else
					return _cache_end_top - _cache_start_top + 2;
			};
			begin_implement_rux_base_ui_min_functions( Line )
				register_rux_property( ::rux::XFloat , StartLeft );
				register_rux_property( ::rux::XFloat , StartTop );
				register_rux_property( ::rux::XFloat , EndLeft );
				register_rux_property( ::rux::XFloat , EndTop );
				register_rux_property( XColorWrap , Color );
				register_rux_property( ::rux::XInt32 , ZIndex );
			end_implement_rux_base_ui_min_functions();
			implement_rux_color_property( Line , Color );
			::rux::uint8 Line::get_IsTransparent( void )
			{
				return 0;
			};
			void Line::raise_OnMouseWheel( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
			};
			void Line::render( ::rux::gui::RenderContextBase* render_context , float opacity , float& _selected_z_index , size_t ___rux__thread_index1986 )
			{
				_render_thread_id = ::rux::threading::XThread::get_CurrentThreadId();
				update_polygon();
				_cs_drawing_elements.wlock( debuginfo_macros );
				render_context->DrawLine( _cache_start_left , _cache_start_top , _cache_end_left , _cache_end_top , _thickness , _opacity * opacity , _color , &_ui_line_cache , &_ui_cap1_cache , &_ui_cap2_cache , _selected_z_index , ___rux__thread_index1986 );
				_cs_drawing_elements.wunlock( debuginfo_macros );
			};
			void XLine::set_EndLeft( float width )
			{
				(*this)()->set_EndLeft( width );
			};
			void Line::set_EndLeft( float width )
			{
				if( _end_left != width )
				{
					_cs_drawing_elements.wlock( debuginfo_macros );
					_end_left = width;
					_cs_drawing_elements.wunlock( debuginfo_macros );
					::XInterlocked::Exchange( &_update_polygon , 1 );
					private_parent_reset_cache();
					private_ResetCache();
				}
			};
			void XLine::set_EndTop( float height )
			{
				(*this)()->set_EndTop( height );
			};
			void Line::set_EndTop( float height )
			{
				if( _end_top != height )
				{
					_cs_drawing_elements.wlock( debuginfo_macros );
					_end_top = height;
					_cs_drawing_elements.wunlock( debuginfo_macros );
					::XInterlocked::Exchange( &_update_polygon , 1 );
					private_parent_reset_cache();
					private_ResetCache();
				}
			};
			void XLine::set_StartLeft( float left )
			{
				(*this)()->set_StartLeft( left );
			};
			void Line::set_StartLeft( float left )
			{
				if( _start_left != left )
				{
					_cs_drawing_elements.wlock( debuginfo_macros );
					_start_left = left;	
					_cs_drawing_elements.wunlock( debuginfo_macros );
					::XInterlocked::Exchange( &_update_polygon , 1 );
					private_parent_reset_cache();
					private_ResetCache();
				}
			};
			void XLine::set_StartTop( float top )
			{
				(*this)()->set_StartTop( top );
			};
			void Line::set_StartTop( float top )
			{
				if( _start_top != top )
				{
					_cs_drawing_elements.wlock( debuginfo_macros );
					_start_top = top;
					_cs_drawing_elements.wunlock( debuginfo_macros );
					::XInterlocked::Exchange( &_update_polygon , 1 );
					private_parent_reset_cache();
					private_ResetCache();
				}
			};
			void Line::set_Color( ::rux::gui::ColorBase* color )
			{
				_cs_drawing_elements.wlock( debuginfo_macros );
				::rux::gui::copy_color( _color , color );
				_cs_drawing_elements.wunlock( debuginfo_macros );
				private_ResetCache();
			};
			::rux::gui::ColorBase* Line::get_Color( void )
			{
				return _color;
			};
			void Line::update_polygon( void )
			{	
				if( ::XInterlocked::CompareExchange( &_update_polygon , 0 , 1 ) == 1 )
				{
					float cache_start_left = _start_left;
					float cache_start_top = _start_top;
					float cache_end_left = _end_left;
					float cache_end_top = _end_top;
					READ_LOCK( _cs_parent_control );
					::rux::gui::ParentBase* parent_copy = _parent_control;
					_cs_parent_control.ReadUnlock();
					if( parent_copy )
					{
						if( parent_copy->get_IsGroup() )
						{
							rux::gui::ControlBase* ui_base = parent_copy;
							if( _point_unit == ::rux::gui::XEnum_PointUnit_Percent
								|| _point_unit == ::rux::gui::XEnum_PointUnit_PercentOnlyLocation )
							{
								cache_start_left *= ui_base->get_Width();
								cache_end_left *= ui_base->get_Width();
								cache_start_top *= ui_base->get_Height();
								cache_end_top *= ui_base->get_Height();
							}
							cache_start_left += ui_base->get_Left();								
							cache_end_left += ui_base->get_Left();								
							cache_start_top += ui_base->get_Top();								
							cache_end_top += ui_base->get_Top();
						}
						else if( parent_copy->get_IsWindow() )
						{
							if( _point_unit == ::rux::gui::XEnum_PointUnit_Percent
								|| _point_unit == ::rux::gui::XEnum_PointUnit_PercentOnlyLocation )
							{
								rux::gui::Window* window = (rux::gui::Window*)parent_copy;
								cache_start_left *= window->get_Width();
								cache_end_left *= window->get_Width();
								cache_start_top *= window->get_Height();
								cache_end_top *= window->get_Height();
							}
						}
					}
					cache_start_left = ::rux::math::round( cache_start_left , 0 );
					cache_end_left = ::rux::math::round( cache_end_left , 0 );
					cache_start_top = ::rux::math::round( cache_start_top , 0 );
					cache_end_top = ::rux::math::round( cache_end_top , 0 );
					_cs_drawing_elements.wlock( debuginfo_macros );
					_cache_start_left = cache_start_left;
					_cache_start_top = cache_start_top;
					_cache_end_left = cache_end_left;
					_cache_end_top = cache_end_top;
					_cs_drawing_elements.wunlock( debuginfo_macros );
				}
			};
			implement_duplicate_internal_function_1( Line , set_Thickness , float );
			void Line::set_Thickness( float thickness )
			{
				if( _thickness != thickness )
				{
					_cs_drawing_elements.wlock( debuginfo_macros );
					_thickness = thickness;
					_cs_drawing_elements.wunlock( debuginfo_macros );
					::XInterlocked::Exchange( &_update_polygon , 1 );
					private_ResetCache();
				}
			};
			rux::gui::ColorBase* XLine::get_Color( void )
			{
				return (*this)()->_color;
			};
			void Line::private_ResetCache( void )
			{	
				::XInterlocked::Exchange( &_update_polygon , 1 );
			};
			void XLine::set_Opacity( float opacity )
			{
				if( (*this)()->_opacity != opacity )
				{
					(*this)()->_opacity = opacity;
					(*this)()->private_ResetCache();
				}
			};
			implement_duplicate_internal_function_1( Line , set_ZIndex , size_t );
			implement_duplicate_internal_result_function( ::rux::int32 , Line , get_ZIndex );
			implement_duplicate_internal_result_function( float , Line , get_Opacity );
			::rux::uint8 XLine::get_IsVisible( void )
			{
				return (*this)()->_is_visible;
			};
			void XLine::set_IsVisible( ::rux::uint8 is_visible )
			{
				if( (*this)()->_is_visible != is_visible )
				{
					(*this)()->_is_visible = is_visible;
					(*this)()->private_parent_reset_cache();
				}
			};
			void Line::set_FreeSystemResourcesThreadId( void )
			{
				_free_system_resources_thread_id = ::rux::threading::XThread::get_CurrentThreadId();
			};
			::rux::byte Line::IsFreeSystemResourcesThread( void )
			{
				return _free_system_resources_thread_id == ::rux::threading::XThread::get_CurrentThreadId() ? 1 : 0;
			};
			void Line::BeforeFreeSystemResources( void )
			{
				::rux::gui::controls::before_free_system_resources< Line >( this );
			};
			void Line::WaitForFreeSystemResources( void )
			{
				::rux::gui::controls::wait_for_free_system_resources< Line >( this );
			};
			void Line::FreeSystemResources( void )
			{
				_tag = 0;
				WRITE_LOCK( _cs_parent_control );
				_parent_control = NULL;
				_cs_parent_control.WriteUnlock();
				_parent_window = 0;
				_free_system_resources_thread_id = 0;
				::XInterlocked::Exchange( &_free_system_resources_accomplished , 1 );
			};
			void Line::UpdateParentWindow( void )
			{
				_parent_window = get_ParentWindow();
			};
			void Line::FreeResources( ::rux::byte )
			{
				_render_thread_id = 0;
				::rux::engine::_globals->_gui_globals->_rux_gui_add_cache( _ui_line_cache );
				_ui_line_cache = NULL;
				::rux::engine::_globals->_gui_globals->_rux_gui_add_cache( _ui_cap1_cache );
				_ui_cap1_cache = NULL;
				::rux::engine::_globals->_gui_globals->_rux_gui_add_cache( _ui_cap2_cache );
				_ui_cap2_cache = NULL;
			};
			void Line::raise_OnActivate( void )
			{		
			};
			void Line::raise_OnDeactivate( ::rux::gui::CommonBase* activated_control )
			{		
				XObject activated;
				if( activated_control )
					activated.set_ByRef( XObject( activated_control , 1 ) );
				_on_deactivated.raise< const XObject& , const XObject& >( XObject( *this ) , activated );
			};
			implement_duplicate_internal_function_1( Line , set_PointUnit , ::rux::gui::XEnum_PointUnit );
			void Line::set_PointUnit( ::rux::gui::XEnum_PointUnit point_unit )
			{
				if( _point_unit != point_unit )
				{
					_cs_drawing_elements.wlock( debuginfo_macros );
					_point_unit = point_unit;
					_cs_drawing_elements.wunlock( debuginfo_macros );
					private_ResetLocationAndSizeCache( 1 , 1 , 1 , 1 );
					::XInterlocked::Exchange( &_update_polygon , 1 );
				}
			};
			void XLine::set_ControlName( const ::rux::XString& name )
			{
				::rux::XString utf8_name;
				utf8_name.set_ByRef( name.ConvertToUTF8() );				
				if( utf8_name.Size() <= 128 )
					::memcpy( (*this)()->_utf8_control_name , utf8_name.str() , utf8_name.Size() );
				else
				{
					::memcpy( (*this)()->_utf8_control_name , utf8_name.str() , 127 );
					(*this)()->_utf8_control_name[ 127 ] = 0;
				}
				(*this)()->_utf8_control_name_hash = ::rux::cryptography::hash::times33_hash( (*this)()->_utf8_control_name , SIZE_MAX );
			};
			rux::String& XLine::get_ControlName( void )
			{
				return ::rux::XString( (*this)()->_utf8_control_name , XEnumCodePage_UTF8 )++;
			};
			rux::uint32 XLine::get_ControlNameHash( void )
			{
				return (*this)()->_utf8_control_name_hash;
			};
			void Line::raise_OnLeftMouseButtonDoubleClick( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
			};
			void Line::raise_OnLeftMouseButtonUp( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_left_mouse_button_up.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void Line::raise_OnKeyChar( const ::rux::XString& , ::rux::uint8 , ::rux::uint8 , ::rux::uint8 )
			{		
			};
			void Line::raise_OnKeyDown( ::rux::gui::XEnum_Keys , ::rux::uint8 , ::rux::uint8 , ::rux::uint8 , ::rux::uint32 , ::rux::byte explicit_event )
			{
			};
			void Line::raise_OnRightMouseButtonDown( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_right_mouse_button_down.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void Line::raise_OnRightMouseButtonDoubleClick( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_right_mouse_button_double_click.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void Line::raise_OnRightMouseButtonUp( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_right_mouse_button_up.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void Line::raise_OnMouseWheelDown(::rux::gui::WindowMouseEvent* window_event, ::rux::byte explicit_event)
			{
				_on_mouse_wheel_down.raise< const ::rux::gui::events::MouseEvent& >(
					::rux::gui::events::MouseEvent(*this, window_event, explicit_event));
			};
			void Line::raise_OnMouseWheelDoubleClick(::rux::gui::WindowMouseEvent* window_event
				, ::rux::byte explicit_event)
			{
				_on_mouse_wheel_double_click.raise< const ::rux::gui::events::MouseEvent& >(
					::rux::gui::events::MouseEvent(*this, window_event, explicit_event));
			};
			void Line::raise_OnMouseWheelUp(::rux::gui::WindowMouseEvent* window_event, ::rux::byte explicit_event)
			{
				_on_mouse_wheel_up.raise< const ::rux::gui::events::MouseEvent& >(
					::rux::gui::events::MouseEvent(*this, window_event, explicit_event));
			};
			void Line::raise_OnLeftMouseButtonDown( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_left_mouse_button_down.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , 1 ) );
			};
			void XLine::set_OnLeftMouseButtonDown( ::rux::gui::events::on_mouse_event_t on_left_mouse_button_down )
			{
				(*this)()->set_OnLeftMouseButtonDown( on_left_mouse_button_down );
			};
			void XLine::set_OnRightMouseButtonDown( ::rux::gui::events::on_mouse_event_t on_left_mouse_button_down )
			{
				(*this)()->set_OnRightMouseButtonDown( on_left_mouse_button_down );
			};
			void XLine::set_OnMouseWheelDown(::rux::gui::events::on_mouse_event_t on_left_mouse_button_down)
			{
				(*this)()->set_OnMouseWheelDown(on_left_mouse_button_down);
			};
			void XLine::set_OnMouseEnter( ::rux::gui::events::on_event_t on_mouse_enter_callback )
			{
				(*this)()->set_OnMouseEnter( on_mouse_enter_callback );
			};
			void XLine::set_OnMouseLeave( ::rux::gui::events::on_mouse_event_t on_mouse_leave_callback )
			{
				(*this)()->set_OnMouseLeave( on_mouse_leave_callback );
			};
			implement_empty_content_size( Line );
			::rux::byte Line::raise_OnMouseEnter( ::rux::gui::WindowMouseEvent* , ::rux::byte& raised_event , ::rux::byte explicit_event )
			{
				_on_mouse_enter.raise< const ::rux::gui::events::Event& >( ::rux::gui::events::Event( *this , explicit_event ) );
				return 1;
			};
			void Line::raise_OnMouseLeave( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_mouse_leave_callback.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void Line::raise_OnMouseMove( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
			};
			implement_duplicate_internal_function_1( Line , set_TagPtr , void* );
			implement_duplicate_internal_result_function_with_const( void* , Line , get_TagPtr );
			::rux::uint8 Line::IsPointInControl( ::rux::int32 x , ::rux::int32 y )
			{	
				if( _render_thread_id == 0 ||_render_thread_id == ::rux::threading::XThread::get_CurrentThreadId() )
					update_polygon();
				_cs_drawing_elements.wlock( debuginfo_macros );
				float thickness = _thickness;
				float cache_start_left = _cache_start_left;
				float cache_start_top = _cache_start_top;
				float cache_end_left = _cache_end_left;
				float cache_end_top = _cache_end_top;
				_cs_drawing_elements.wunlock( debuginfo_macros );
				if( thickness > 1 )
				{
					float cathetus = ::rux::math::round( thickness / 2.f , 0 ) , x0 = 0 , y0 = 0 , x1 = 0 , y1 = 0 , x2 = 0 , y2 = 0 , x3 = 0 , y3 = 0;
					::rux::math::calculate_third_point_of_right_triangle( cache_start_left , cache_start_top , cache_end_left , cache_end_top , cathetus , x0 , y0 , x1 , y1 );
					::rux::math::calculate_third_point_of_right_triangle( cache_end_left , cache_end_top , cache_start_left , cache_start_top , cathetus , x2 , y2 , x3 , y3 );
					if( x0 > cache_start_left )
					{
						if( x2 > cache_end_left )
							return ::rux::math::is_point_belongs_to_rectangle( x0 , y0 , x2 , y2 , x3 , y3 , x1 , y1 , x , y , 1 );
						else
							return ::rux::math::is_point_belongs_to_rectangle( x0 , y0 , x3 , y3 , x2 , y2 , x1 , y1 , x , y , 1 );
					}
					else if( x0 < cache_start_left )
					{
						if( x2 < cache_end_left )
							return ::rux::math::is_point_belongs_to_rectangle( x0 , y0 , x2 , y2 , x3 , y3 , x1 , y1 , x , y , 1 );
						else
							return ::rux::math::is_point_belongs_to_rectangle( x0 , y0 , x3 , y3 , x2 , y2 , x1 , y1 , x , y , 1 );
					}
					else if( y0 > cache_start_top )
					{
						if( y2 > cache_end_top )
							return ::rux::math::is_point_belongs_to_rectangle( x0 , y0 , x2 , y2 , x3 , y3 , x1 , y1 , x , y , 1 );
						else
							return ::rux::math::is_point_belongs_to_rectangle( x0 , y0 , x3 , y3 , x2 , y2 , x1 , y1 , x , y , 1 );
					}
					else
					{
						if( y2 < cache_end_top )
							return ::rux::math::is_point_belongs_to_rectangle( x0 , y0 , x2 , y2 , x3 , y3 , x1 , y1 , x , y , 1 );
						else
							return ::rux::math::is_point_belongs_to_rectangle( x0 , y0 , x3 , y3 , x2 , y2 , x1 , y1 , x , y , 1 );
					}
				}
				else
					return ::rux::XMath::is_point_belongs_to_segment( cache_start_left , cache_start_top , cache_end_left , cache_end_top , x , y );
			};
			void Line::Invalidate( void )
			{
				private_ResetCache();
			};
			::rux::gui::ParentBase* XLine::get_ParentControl( ::rux::byte lock , ::rux::byte real_parent )
			{
				return (*this)()->get_ParentControl( lock , real_parent );
			};
			void XLine::set_Tag( const XObject& tag )
			{
				(*this)()->set_Tag( tag );
			};
			Object& XLine::get_Tag( void )
			{
				return (*this)()->get_Tag();
			};			
			rux::gui::XEnum_Cursors Line::get_Cursor( void )
			{
				return _cursor;
			};
			void Line::set_Cursor( ::rux::gui::XEnum_Cursors cursor )
			{
				if( _cursor != cursor )
					_cursor = cursor;
			};
			rux::gui::XEnum_Cursors XLine::get_Cursor( void )
			{
				return (*this)()->get_Cursor();
			};
			void XLine::set_Cursor( ::rux::gui::XEnum_Cursors cursor )
			{
				(*this)()->set_Cursor( cursor );
			};
			float Line::get_Width( float* )
			{
				return get_ClipWidth();
			};
			float Line::get_Height( float* )
			{
				return get_ClipHeight();
			};
			float Line::get_Left( ::rux::uint8 , float* )
			{
				return get_ClipLeft();
			};
			float Line::get_Top( ::rux::uint8 , float* )
			{
				return get_ClipTop();
			};
			::rux::gui::XEnum_Filling Line::get_VerticalFilling( void )
			{
				return _vertical_filling;
			};
			::rux::gui::XEnum_Filling Line::get_HorizontalFilling( void )
			{
				return _horizontal_filling;
			};
			::rux::gui::XEnum_Alignment Line::get_VerticalAlignment( void )
			{
				return XEnum_Alignment_Manual;
			};
			::rux::gui::XEnum_Alignment Line::get_HorizontalAlignment( void )
			{
				return XEnum_Alignment_Manual;
			};
			rux::gui::Margin& Line::get_Margin( void )
			{
				return _margin;
			};
			void Line::set_Left( float ) const
			{
			};
			void Line::set_Top( float ) const
			{
			};
		};
	};
};
