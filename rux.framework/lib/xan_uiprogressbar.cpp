#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_uiprogressbar.h>
#include <xan_uiinlines.h>
#include <xan_uibutton.h>
#include <xan_keyvaluepair.h>
#include <xan_math.h>
begin_implement_rux_class_with_properties_ns_base_class( ProgressBar , rux::gui::controls , rux::gui::ControlBase )
	_tag.set_Info( "tag" , __file__ , __line__ );
	_private_tag.set_Info( "private_tag" , __file__ , __line__ );
	IMPLEMENT_BASE_UI_MEMBERS();
	rux::gui::LinearGradientColor* linear_gradient = alloc_object_macros( ::rux::gui::LinearGradientColor );
	linear_gradient->set_LinearGradientType( ::rux::gui::XEnum_LinearGradientType_Vertical );
	linear_gradient->AddGradientStop( ::rux::gui::GradientStop( ::rux::gui::Color( 0x85 , 0xDE , 0xF9 , 255 ) , 0.0 ) );
	linear_gradient->AddGradientStop( ::rux::gui::GradientStop( ::rux::gui::Color( 0x19 , 0x78 , 0xAF , 255 ) , 1.0 ) );
	copy_color( _progress_color_object , linear_gradient );
	copy_color( _background_object , create_color( 0x33 , 0x31 , 0x2D , 255 ) );
	copy_color( _background_border_color_object ,  create_color( 0x5F , 0x5C , 0x58 , 255 ) );
	_value = 50.0;
	_max_value = 100.0;
	_min_value = 0.0;
end_implement_rux_class();
namespace rux
{
	namespace gui
	{
		namespace controls
		{
			ProgressBar::~ProgressBar()
			{		
				FreeResources( 0 );
				if( _background_object )
					_background_object->Release();
				if( _background_border_color_object )
					_background_border_color_object->Release();
				if( _progress_color_object )
					_progress_color_object->Release();
				if( _progress_border_color_object )
					_progress_border_color_object->Release();
			};
			XGCRef& ProgressBar::static_get_Value( const XGCRef* object )
			{
				ProgressBar* obj = (ProgressBar*)object->get_ObjectAddress();
				return ::rux::XFloat( obj->get_Value() )++;
			};
			void ProgressBar::static_set_Value( const XGCRef* object , const XGCRef* value )
			{
				ProgressBar* obj = (ProgressBar*)object->get_ObjectAddress();
				obj->set_Value( ::rux::gcref_to_float( value ) );
			};
			begin_implement_rux_base_ui_functions( ProgressBar )
				register_rux_property( ::rux::XFloat , Value );
			end_implement_rux_base_ui_functions();
			void* ProgressBar::DynamicCast( ::rux::int32 class_type ) const
			{
				if( class_type == ::rux::gui::XEnum_ClassType_ControlBase )
				{
					::rux::gui::ControlBase* ptr = const_cast< ProgressBar* >( this );
					return ptr;
				}
				return 0;
			};
			::rux::uint8 ProgressBar::get_IsGroup( void )
			{
				return 0;
			};
			::rux::uint8 ProgressBar::get_IsTimePicker( void )
			{
				return 0;
			};
			::rux::uint8 ProgressBar::get_IsButton( void )
			{
				return 0;
			};
			::rux::uint8 ProgressBar::get_IsTransparent( void )
			{
				return 0;
			};
			void ProgressBar::raise_OnMouseWheel( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
			};
			void ProgressBar::raise_OnActivate( void )
			{		
			};
			void ProgressBar::raise_OnDeactivate( ::rux::gui::CommonBase* activated_control )
			{		
				XObject activated;
				if( activated_control )
					activated.set_ByRef( XObject( activated_control , 1 ) );
				_on_deactivated.raise< const XObject& , const XObject& >( XObject( *this ) , activated );
			};
			void ProgressBar::Invalidate( void )
			{
			};
			void ProgressBar::private_ResetCache( void )
			{
			};
			void ProgressBar::render( ::rux::gui::RenderContextBase* render_context , float opacity , float& _selected_z_index , size_t ___rux__thread_index1986 )
			{	
				float __left = get_Left();
				float __top = get_Top();
				float __width = get_Width();
				float __height = get_Height();
				float border_width = 1.0;
				_cs_drawing_elements.wlock( debuginfo_macros );
				render_context->DrawRectangle( __left , __top , __width , __height , border_width , _background_object , _background_border_color_object , ::rux::gui::Thickness() , _opacity * opacity , &_ui_border_cache , &_ui_cache , _selected_z_index , 1 , __FILE__ , __LINE__ , ___rux__thread_index1986 );
				_cs_drawing_elements.wunlock( debuginfo_macros );
				if( _value > 0.0 )
				{
					_selected_z_index += 1.f;//0.001f
					float progress_width = ( __width - 2 * border_width ) * ( _value / _max_value );
					_cs_drawing_elements.wlock( debuginfo_macros );
					render_context->DrawRectangle( ::rux::math::round( __left + border_width , 0 ) 
						, ::rux::math::round( __top + border_width , 0 ) , ::rux::math::round( progress_width , 0 ) 
						, ::rux::math::round( __height - 2 * border_width , 0 ) , 0.0f , _progress_color_object , _progress_border_color_object 
						, ::rux::gui::Thickness() , _opacity * opacity , &_ui_progress_border_cache , &_ui_progress_cache 
						, _selected_z_index , 1 , __FILE__ , __LINE__ , ___rux__thread_index1986 );
					_cs_drawing_elements.wunlock( debuginfo_macros );
				}
			};
			void ProgressBar::set_Value( float value )
			{
				if( value >= _min_value && value <= _max_value )
				{
					_value = value;
					Invalidate();
				}
			};
			float ProgressBar::get_Value( void )
			{
				return _value;
			};
			void XProgressBar::set_Value( float value )
			{
				(*this)()->set_Value( value );
			};
			void XProgressBar::set_MaxValue( float max_value )
			{
				if( (*this)()->_min_value < max_value )
				{
					(*this)()->_max_value = max_value;
					if( (*this)()->_value > max_value )
						(*this)()->_value = max_value;
					(*this)()->Invalidate();
				}
			};
			void XProgressBar::set_MinValue( float min_value )
			{
				if( (*this)()->_max_value > min_value )
				{
					(*this)()->_min_value = min_value;
					if( (*this)()->_value < min_value )
						(*this)()->_value = min_value;
					(*this)()->Invalidate();
				}
			};
			void ProgressBar::UpdateParentWindow( void )
			{
				_parent_window = get_ParentWindow();
			};
			void ProgressBar::FreeResources( ::rux::byte )
			{
				::rux::engine::_globals->_gui_globals->_rux_gui_add_cache( _ui_cache );
				_ui_cache = NULL;
				::rux::engine::_globals->_gui_globals->_rux_gui_add_cache( _ui_border_cache );
				_ui_border_cache = NULL;
				::rux::engine::_globals->_gui_globals->_rux_gui_add_cache( _ui_progress_border_cache );
				_ui_progress_border_cache = NULL;
				::rux::engine::_globals->_gui_globals->_rux_gui_add_cache( _ui_progress_cache );
				_ui_progress_cache = NULL;
			};
			void ProgressBar::raise_OnLeftMouseButtonDoubleClick( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
			};
			void ProgressBar::raise_OnLeftMouseButtonUp( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_left_mouse_button_up.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void ProgressBar::raise_OnKeyChar( const ::rux::XString& , ::rux::uint8 , ::rux::uint8 , ::rux::uint8 )
			{		
			};
			void ProgressBar::raise_OnKeyDown( ::rux::gui::XEnum_Keys , ::rux::uint8 , ::rux::uint8 , ::rux::uint8 , ::rux::uint32 , ::rux::byte explicit_event )
			{
			};
			void ProgressBar::raise_OnRightMouseButtonDown( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_right_mouse_button_down.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void ProgressBar::raise_OnRightMouseButtonDoubleClick( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_right_mouse_button_double_click.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void ProgressBar::raise_OnRightMouseButtonUp( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_right_mouse_button_up.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void ProgressBar::raise_OnLeftMouseButtonDown( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_left_mouse_button_down.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , 1 ) );
			};
			::rux::byte ProgressBar::raise_OnMouseEnter( ::rux::gui::WindowMouseEvent* , ::rux::byte& raised_event , ::rux::byte explicit_event )
			{
				_on_mouse_enter.raise< const ::rux::gui::events::Event& >( ::rux::gui::events::Event( *this , explicit_event ) );
				return 1;
			};
			implement_empty_content_size( ProgressBar );
			void ProgressBar::raise_OnMouseLeave( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_mouse_leave_callback.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void ProgressBar::raise_OnMouseMove( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
			};
			IMPLEMENT_BASE_UI_FUNCTIONS( XProgressBar );
		};
	};
};