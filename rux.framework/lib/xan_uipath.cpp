#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_uipath.h>
#include <xan_uibutton.h>
#include <xan_uiinlines.h>
#include <xan_keyvaluepair.h>
#include <xan_math.h>
begin_implement_rux_class_with_properties_ns_base_class( Path , rux::gui::controls , rux::gui::ControlBase )
	_tag.set_Info( "tag" , __file__ , __line__ );
	_private_tag.set_Info( "private_tag" , __file__ , __line__ );
	IMPLEMENT_BASE_UI_MEMBERS();
	_is_smooth = 1;
end_implement_rux_class();
namespace rux
{
	namespace gui
	{
		namespace controls
		{
			Path::~Path()
			{
				FreeResources( 0 );
				CRITICAL_SECTION_LOCK( _cs_points );
				for( ; _points.Count() > 0 ; )
				{
					rux::engine::free_object< ::rux::gui::Point >( _points[ _points.Count() - 1 ] );
					_points.RemoveAt( _points.Count() - 1 );
				}
				CRITICAL_SECTION_UNLOCK( _cs_points );	
				if( _background )
					_background->Release();
			};
			begin_implement_rux_base_ui_functions( Path )
				register_rux_property( XColorWrap , Background );
			end_implement_rux_base_ui_functions();
			implement_rux_color_property( Path , Background );
			::rux::gui::ColorBase* Path::get_Background( void )
			{
				return _background;
			};
			void* Path::DynamicCast( ::rux::int32 class_type ) const
			{
				if( class_type == ::rux::gui::XEnum_ClassType_ControlBase )
				{
					::rux::gui::ControlBase* ptr = const_cast< Path* >( this );
					return ptr;
				}
				return 0;
			};
			::rux::uint8 Path::get_IsGroup( void )
			{
				return 0;
			};
			::rux::uint8 Path::get_IsTimePicker( void )
			{
				return 0;
			};
			::rux::uint8 Path::get_IsButton( void )
			{
				return 0;
			};
			implement_duplicate_internal_function_1( Path , set_Background , ::rux::gui::ColorBase* );
			void Path::raise_OnActivate( void )
			{		
			};
			void Path::raise_OnDeactivate( ::rux::gui::CommonBase* activated_control )
			{		
				XObject activated;
				if( activated_control )
					activated.set_ByRef( XObject( activated_control , 1 ) );
				_on_deactivated.raise< const XObject& , const XObject& >( XObject( *this ) , activated );
			};
			::rux::uint8 Path::get_IsTransparent( void )
			{
				return 0;
			};
			void Path::raise_OnMouseWheel( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
			};
			void Path::render( ::rux::gui::RenderContextBase* render_context , float opacity , float& _selected_z_index , size_t ___rux__thread_index1986 )
			{	
				float __left = get_Left();
				float __top = get_Top();
				float __width = get_Width();
				float __height = get_Height();
				CRITICAL_SECTION_LOCK( _cs_points );
				_cs_drawing_elements.wlock( debuginfo_macros );
				render_context->DrawPath( __left , __top , __width , __height , _points , _opacity * opacity , _background , &_ui_cache , _selected_z_index , _is_smooth , ___rux__thread_index1986 );
				_cs_drawing_elements.wunlock( debuginfo_macros );
				CRITICAL_SECTION_UNLOCK( _cs_points );
			};
			void Path::set_Background( ::rux::gui::ColorBase* background )
			{
				_cs_drawing_elements.wlock( debuginfo_macros );
				rux::gui::copy_color( _background , background );
				_cs_drawing_elements.wunlock( debuginfo_macros );
			};
			rux::gui::ColorBase* XPath::get_Background( void )
			{
				return (*this)()->_background;
			};
			void XPath::AddPoint( const ::rux::gui::Point& point )
			{
				rux::gui::Point* p = const_cast< ::rux::gui::Point* >( &point );
				CRITICAL_SECTION_LOCK( (*this)()->_cs_points );
				(*this)()->_points.Add( alloc_object_3_params_macros( ::rux::gui::Point , p->_x , p->_y , p->_point_unit ) );
				CRITICAL_SECTION_UNLOCK( (*this)()->_cs_points );
			};
			void Path::Invalidate( void )
			{
			};
			void Path::private_ResetCache( void )
			{
			};
			void Path::UpdateParentWindow( void )
			{
				_parent_window = get_ParentWindow();
			};
			void Path::FreeResources( ::rux::byte )
			{
				::rux::engine::_globals->_gui_globals->_rux_gui_add_cache( _ui_cache );
				_ui_cache = NULL;
			};
			void Path::raise_OnLeftMouseButtonDoubleClick( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
			};
			void Path::raise_OnLeftMouseButtonUp( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_left_mouse_button_up.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void Path::raise_OnKeyChar( const ::rux::XString& , ::rux::uint8 , ::rux::uint8 , ::rux::uint8 )
			{		
			};
			void Path::raise_OnKeyDown( ::rux::gui::XEnum_Keys , ::rux::uint8 , ::rux::uint8 , ::rux::uint8 , ::rux::uint32 , ::rux::byte explicit_event )
			{
			};
			void Path::raise_OnRightMouseButtonDown( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_right_mouse_button_down.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void Path::raise_OnRightMouseButtonDoubleClick( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_right_mouse_button_double_click.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void Path::raise_OnRightMouseButtonUp( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_right_mouse_button_up.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void Path::raise_OnMouseWheelDown(::rux::gui::WindowMouseEvent* window_event, ::rux::byte explicit_event)
			{
				_on_mouse_wheel_down.raise< const ::rux::gui::events::MouseEvent& >(
					::rux::gui::events::MouseEvent(*this, window_event, explicit_event));
			};
			void Path::raise_OnMouseWheelDoubleClick(::rux::gui::WindowMouseEvent* window_event
				, ::rux::byte explicit_event)
			{
				_on_mouse_wheel_double_click.raise< const ::rux::gui::events::MouseEvent& >(
					::rux::gui::events::MouseEvent(*this, window_event, explicit_event));
			};
			void Path::raise_OnMouseWheelUp(::rux::gui::WindowMouseEvent* window_event, ::rux::byte explicit_event)
			{
				_on_mouse_wheel_up.raise< const ::rux::gui::events::MouseEvent& >(
					::rux::gui::events::MouseEvent(*this, window_event, explicit_event));
			};
			void Path::raise_OnLeftMouseButtonDown( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_left_mouse_button_down.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , 1 ) );
			};
			::rux::byte Path::raise_OnMouseEnter( ::rux::gui::WindowMouseEvent* , ::rux::byte& raised_event , ::rux::byte explicit_event )
			{
				_on_mouse_enter.raise< const ::rux::gui::events::Event& >( ::rux::gui::events::Event( *this , explicit_event ) );
				return 1;
			};
			implement_empty_content_size( Path );
			void Path::raise_OnMouseLeave( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_mouse_leave_callback.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void Path::raise_OnMouseMove( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
			};
			void XPath::set_IsSmooth( ::rux::uint8 smooth )
			{
				(*this)()->_is_smooth = smooth;
			};
			IMPLEMENT_BASE_UI_FUNCTIONS( XPath );
		};
	};
};