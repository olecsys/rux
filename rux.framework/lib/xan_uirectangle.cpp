#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_uirectangle.h>
#include <xan_uiinlines.h>
#include <xan_uibutton.h>
#include <xan_keyvaluepair.h>
begin_implement_rux_class_with_properties_ns_base_class( Rectangle , rux::gui::controls , rux::gui::ControlBase )
	_tag.set_Info( "tag" , __file__ , __line__ );
	_private_tag.set_Info( "private_tag" , __file__ , __line__ );
	IMPLEMENT_BASE_UI_MEMBERS();
	_border_width = 0;
	_smooth = 1;
	copy_color( _background , ::rux::gui::Colors::White() );
	copy_color( _border_color , ::rux::gui::Colors::Transparent() );
end_implement_rux_class();
namespace rux
{
	namespace gui
	{
		namespace controls
		{
			Rectangle::~Rectangle()
			{		
				FreeResources( 0 );
				if( _background )
					_background->Release();
				if( _border_color )
					_border_color->Release();
			};
			begin_implement_rux_base_ui_functions( Rectangle )
				register_rux_property( XColorWrap , Background );
				register_rux_property( XColorWrap , BorderColor );
			end_implement_rux_base_ui_functions();
			implement_rux_color_property( Rectangle , Background );
			implement_rux_color_property( Rectangle , BorderColor );
			::rux::gui::ColorBase* Rectangle::get_Background( void )
			{
				return _background;
			};
			::rux::gui::ColorBase* Rectangle::get_BorderColor( void )
			{
				return _border_color;
			};
			void* Rectangle::DynamicCast( ::rux::int32 class_type ) const
			{
				if( class_type == ::rux::gui::XEnum_ClassType_ControlBase )
				{
					::rux::gui::ControlBase* ptr = const_cast< Rectangle* >( this );
					return ptr;
				}
				return 0;
			};
			::rux::uint8 Rectangle::get_IsGroup( void )
			{
				return 0;
			};
			::rux::uint8 Rectangle::get_IsTimePicker( void )
			{
				return 0;
			};
			::rux::uint8 Rectangle::get_IsButton( void )
			{
				return 0;
			};
			::rux::uint8 Rectangle::get_IsTransparent( void )
			{
				return 0;
			};
			XRectangle::XRectangle( const Rectangle& rect , const char* variable_name , const char* __file__ , ::rux::int32 __line__ )
				: XGCHandle< Rectangle >( variable_name , __file__ , __line__ )
			{
				rect.AddRef();
				(*this) = rect;
				rect.Release();
			};
			XRectangle::XRectangle( const XRectangle& rect , const char* variable_name , const char* __file__ , ::rux::int32 __line__ )
				: XGCHandle< Rectangle >( variable_name , __file__ , __line__ )
			{
				(*(static_cast< XGCHandle< Rectangle >* >( this ))) = rect;
			};
			void Rectangle::render( ::rux::gui::RenderContextBase* render_context , float opacity , float& _selected_z_index , size_t ___rux__thread_index1986 )
			{	
				float __left = get_Left();
				float __top = get_Top();
				float __width = get_Width();
				float __height = get_Height();
				_cs_drawing_elements.wlock( debuginfo_macros );
				render_context->DrawRectangle( __left , __top , __width , __height , _border_width , _background , _border_color , _corner , _opacity * opacity , &_ui_border_cache , &_ui_cache , _selected_z_index , _smooth , __FILE__ , __LINE__ , ___rux__thread_index1986 );
				_cs_drawing_elements.wunlock( debuginfo_macros );
			};
			void Rectangle::set_Background( ::rux::gui::ColorBase* background )
			{				
				_cs_drawing_elements.wlock( debuginfo_macros );
				rux::gui::copy_color( _background , background );
				_cs_drawing_elements.wunlock( debuginfo_macros );
			};
			void XRectangle::set_Background( ::rux::gui::ColorBase* background )
			{
				(*this)()->set_Background( background );
			};
			void XRectangle::set_BorderColor( ::rux::gui::ColorBase* border_color )
			{
				(*this)()->set_BorderColor( border_color );
			};
			void Rectangle::set_BorderColor( ::rux::gui::ColorBase* border_color )
			{
				_cs_drawing_elements.wlock( debuginfo_macros );
				rux::gui::copy_color( _border_color , border_color );
				_cs_drawing_elements.wunlock( debuginfo_macros );
			};
			void XRectangle::set_BorderWidth( float border_width )
			{
				if( (*this)()->_border_width != border_width )
				{
					(*this)()->_border_width = ::rux::math::round( border_width , 0 );
				}
			};
			rux::gui::ColorBase* XRectangle::get_Background( void )
			{
				return (*this)()->_background;
			};
			rux::gui::Color* XRectangle::get_BorderColor( void )
			{
				return (*this)()->_border_color;
			};
			void XRectangle::set_Corner( const ::rux::gui::Thickness& corner )
			{
				if( (*this)()->_corner != corner )
				{
					(*this)()->_cs_drawing_elements.wlock( debuginfo_macros );
					(*this)()->_corner = corner;
					(*this)()->_cs_drawing_elements.wunlock( debuginfo_macros );
				}
			};
			void Rectangle::Invalidate( void )
			{
			};
			void Rectangle::private_ResetCache( void )
			{
			};
			::rux::byte Rectangle::raise_OnMouseEnter( ::rux::gui::WindowMouseEvent* , ::rux::byte& raised_event , ::rux::byte explicit_event )
			{	
				_on_mouse_enter.raise< const ::rux::gui::events::Event& >( ::rux::gui::events::Event( *this , explicit_event ) );
				return 1;
			};
			void Rectangle::raise_OnActivate( void )
			{		
			};
			void Rectangle::raise_OnDeactivate( ::rux::gui::CommonBase* activated_control )
			{		
				XObject activated;
				if( activated_control )
					activated.set_ByRef( XObject( activated_control , 1 ) );
				_on_deactivated.raise< const XObject& , const XObject& >( XObject( *this ) , activated );
			};
			implement_empty_content_size( Rectangle );
			void Rectangle::raise_OnMouseLeave( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{	
				_on_mouse_leave_callback.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void Rectangle::raise_OnMouseMove( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{	
				_on_mouse_move.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void Rectangle::UpdateParentWindow( void )
			{
				_parent_window = get_ParentWindow();
			};
			void Rectangle::FreeResources( ::rux::byte )
			{
				::rux::engine::_globals->_gui_globals->_rux_gui_add_cache( _ui_cache );
				_ui_cache = NULL;
				::rux::engine::_globals->_gui_globals->_rux_gui_add_cache( _ui_border_cache );
				_ui_border_cache = NULL;
			};
			::rux::uint8 XRectangle::get_IsTransparent( void )
			{
				return ::rux::gui::rux_color_is_visible( (*this)()->_background ) == 1 ? 0 : 1;
			};
			void Rectangle::raise_OnMouseWheel( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_mouse_wheel.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void Rectangle::raise_OnKeyChar( const ::rux::XString& , ::rux::uint8 , ::rux::uint8 , ::rux::uint8 )
			{		
			};
			void Rectangle::raise_OnKeyDown( ::rux::gui::XEnum_Keys , ::rux::uint8 , ::rux::uint8 , ::rux::uint8 , ::rux::uint32 , ::rux::byte explicit_event )
			{
			};
			void Rectangle::raise_OnRightMouseButtonDown( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_right_mouse_button_down.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void Rectangle::raise_OnRightMouseButtonDoubleClick( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_right_mouse_button_double_click.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void Rectangle::raise_OnRightMouseButtonUp( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_right_mouse_button_up.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void Rectangle::raise_OnLeftMouseButtonDown( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{	
				_on_left_mouse_button_down.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , 1 ) );
			};
			void Rectangle::raise_OnLeftMouseButtonUp( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_left_mouse_button_up.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void Rectangle::raise_OnLeftMouseButtonDoubleClick( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_mouse_double_click.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , 1 ) );
			};
			IMPLEMENT_BASE_UI_FUNCTIONS( XRectangle );
		};
	};
};