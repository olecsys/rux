#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_uibutton.h>
#include <xan_window.h>
#include <xan_uigroup.h>
#include <xan_uiprimitives.h>
#include <xan_uitextblock.h>
#include <xan_uirectangle.h>
#include <xan_uiinlines.h>
#include <xan_keyvaluepair.h>
#include <xan_uitimepicker.h>
#include <xan_uiimage.h>
begin_implement_rux_class_with_properties_ns_base_class( Button , rux::gui::controls , rux::gui::ParentBase )
	_content.set_Info( "content" , __file__ , __line__ );
	_tag.set_Info( "tag" , __file__ , __line__ );
	_private_tag.set_Info( "private_tag" , __file__ , __line__ );
	IMPLEMENT_BASE_UI_MEMBERS();
	_corner = ::rux::gui::Thickness( 0.2f , 0.2f , 0.2f , 0.2f );
	copy_color( _foreground , create_color( 255 , 255 , 255 , 255 ) );
	copy_color( _background , ::rux::gui::Colors::ButtonNormalColor() );
	copy_color( _over_background , ::rux::gui::Colors::ButtonOverColor() );
	copy_color( _pressed_background , ::rux::gui::Colors::ButtonDownColor() );
	copy_color( _disabled_background , ::rux::gui::Colors::ButtonDisabledColor() );
	_active_state_background = _background;
	_on_click_callback = NULL;	
	_ui_font = ::rux::engine::_globals->_gui_globals->_rux_gui_create_font( "tahoma.ttf" , 0 , 11 );	
	_is_repeat_until_mouse_up = 0;
	_current_render_index = SIZE_MAX;
end_implement_rux_class();
namespace rux
{
	namespace gui
	{
		namespace controls
		{
			Button::~Button()
			{
				FreeResources( 0 );
				_repeat_thread.Stop();	
				if( _background )
					_background->Release();
				if( _over_background )
					_over_background->Release();
				if( _pressed_background )
					_pressed_background->Release();
				if( _disabled_background )
					_disabled_background->Release();
				if( _border_color )
					_border_color->Release();
				if( _foreground )
					_foreground->Release();
			};
			::rux::gui::ColorBase* Button::get_Background( void )
			{
				return _background;
			};
			::rux::gui::ColorBase* Button::get_DisabledBackground( void )
			{
				return _disabled_background;
			};
			::rux::gui::ColorBase* Button::get_OverBackground( void )
			{
				return _over_background;
			};
			void Button::set_ScrollXOffset( float )
			{
			};
			void Button::set_ScrollYOffset( float )
			{
			};
			float Button::get_ScrollXOffset( void )
			{
				return 0;
			};
			::rux::int32 Button::IndexOf( XGCRef* control )
			{
				return -1;
			};
			void Button::set_ZIndex( XGCRef* , size_t )
			{
			};
			float Button::get_ScrollYOffset( void )
			{
				return 0;
			};
			::rux::gui::ColorBase* Button::get_PressedBackground( void )
			{
				return _pressed_background;
			};
			begin_implement_rux_base_ui_functions( Button )
				register_rux_property( XColorWrap , Background );
				register_rux_property( XColorWrap , OverBackground );
				register_rux_property( XColorWrap , PressedBackground );
				register_rux_property( XColorWrap , DisabledBackground );
			end_implement_rux_base_ui_functions();

			implement_rux_color_property( Button , Background );
			implement_rux_color_property( Button , OverBackground );
			implement_rux_color_property( Button , PressedBackground );
			implement_rux_color_property( Button , DisabledBackground );

			implement_duplicate_internal_function_1( Button , set_Background , ::rux::gui::ColorBase* );	
			implement_duplicate_internal_function_1( Button , set_ContentOpacity , float );
			implement_duplicate_internal_function_2( Button , set_ContentOpacity , const ::rux::XString& , float );
			implement_duplicate_internal_function_1( Button , set_ContentBackground , ::rux::gui::ColorBase* );
			implement_duplicate_internal_function_2( Button , set_ContentBackground , const ::rux::XString& , ::rux::gui::ColorBase* );
			implement_duplicate_internal_function_1( Button , set_ContentBorderColor , ::rux::gui::Color* );
			implement_duplicate_internal_function_1( Button , set_OverBackground , ::rux::gui::ColorBase* );
			implement_duplicate_internal_function_1( Button , set_PressedBackground , ::rux::gui::ColorBase* );
			implement_duplicate_internal_function_1( Button , set_DisabledBackground , ::rux::gui::ColorBase* );
			implement_duplicate_internal_function_1( Button , set_Content , const XObject& );
			implement_duplicate_internal_function_1( Button , set_Content , const Object& );
			implement_duplicate_internal_function_1( Button , set_Content , const XGCRef& );
			void Button::raise_OnActivate( void )
			{		
			};
			void Button::raise_OnDeactivate( ::rux::gui::CommonBase* activated_control )
			{		
				XObject activated;
				if( activated_control )
					activated.set_ByRef( XObject( activated_control , 1 ) );
				_on_deactivated.raise< const XObject& , const XObject& >( XObject( *this ) , activated );
			};
			::rux::uint8 Button::get_IsTransparent( void )
			{
				return 0;
			};
			void Button::Invalidate( void )
			{
				private_ResetCache();
			};
			void Button::private_ResetCache( void )
			{
				READ_LOCK( _cs_content );
				::rux::gui::ControlBase* common_base = (::rux::gui::ControlBase*)_content.get_GCRef()->DynamicCast( ::rux::gui::XEnum_ClassType_ControlBase );
				if( common_base )
				{		
					common_base->GCRefAddRef( __FILE__ , __LINE__ );
					_cs_content.ReadUnlock();
					common_base->private_ResetCache();
					common_base->GCRefRelease( __FILE__ , __LINE__ );
				}
				else
					_cs_content.ReadUnlock();
			};
			void Button::set_Background( ::rux::gui::ColorBase* background )
			{	
				_cs_drawing_elements.wlock( debuginfo_macros );
				if( _active_state_background == _background )
				{
					_active_state_background = background;					
					Invalidate();
				}
				::rux::gui::copy_color( _background , background );
				_cs_drawing_elements.wunlock( debuginfo_macros );
			};
			void Button::set_ContentOpacity( float opacity )
			{
				READ_LOCK( _cs_content );
				if( rux_is_object( _content , ::rux::gui::controls::XGroup ) )
				{
					declare_variable_param( ::rux::gui::controls::XGroup , group , _content );
					group.set_Opacity( opacity );
					_cs_content.ReadUnlock();
					Invalidate();
				}
				else if( rux_is_object( _content , ::rux::gui::controls::XTextBlock ) )
				{
					::rux::gui::controls::XTextBlock textblock( _content );
					textblock.set_Opacity( opacity );
					_cs_content.ReadUnlock();
					Invalidate();
				}
				else
					_cs_content.ReadUnlock();
			};
			void Button::set_ContentOpacity( const ::rux::XString& control_name , float opacity )
			{
				READ_LOCK( _cs_content );
				if( rux_is_object( _content , ::rux::gui::controls::XGroup ) )
				{
					declare_variable_param( ::rux::gui::controls::XGroup , group , _content );
					_cs_content.ReadUnlock();
					XArray< ::rux::gui::controls::XRectangle > rects;
					rects.set_ByRef( group.FindAll< ::rux::gui::controls::XRectangle >( control_name ) );
					::rux::uint32 index0 = 0;
					for( index0 = 0 ; index0 < rects.Count() ; index0++ )
						rects[ index0 ].set_Opacity( opacity );
					XArray< XPath > paths;
					paths.set_ByRef( group.FindAll< XPath >( control_name ) );
					for( index0 = 0 ; index0 < paths.Count() ; index0++ )
						paths[ index0 ].set_Opacity( opacity );		
					XArray< XLine > lines;
					lines.set_ByRef( group.FindAll< XLine >( control_name ) );
					for( index0 = 0 ; index0 < lines.Count() ; index0++ )
						lines[ index0 ].set_Opacity( opacity );
					Invalidate();
				}
				else
					_cs_content.ReadUnlock();
			};
			void Button::set_ContentBackground( const ::rux::XString& control_name , ::rux::gui::ColorBase* background )
			{
				READ_LOCK( _cs_content );
				if( rux_is_object( _content , ::rux::gui::controls::XGroup ) )
				{
					declare_variable_param( ::rux::gui::controls::XGroup , group , _content );
					_cs_content.ReadUnlock();
					XArray< ::rux::gui::controls::XRectangle > rects;
					rects.set_ByRef( group.FindAll< ::rux::gui::controls::XRectangle >( control_name ) );
					::rux::uint32 index0 = 0;
					for( index0 = 0 ; index0 < rects.Count() ; index0++ )
					{
						rects[ index0 ].set_Background( background );
					}
					XArray< XPath > paths;
					paths.set_ByRef( group.FindAll< XPath >( control_name ) );
					for( index0 = 0 ; index0 < paths.Count() ; index0++ )
						paths[ index0 ].set_Background( background );
					if( background->_color_type == 0 )
					{
						XArray< XLine > lines;
						lines.set_ByRef( group.FindAll< XLine >( control_name ) );
						for( index0 = 0 ; index0 < lines.Count() ; index0++ )
							lines[ index0 ].set_Color( (rux::gui::Color*)background );
					}
					Invalidate();
				}
				else
					_cs_content.ReadUnlock();
			};
			void Button::set_ContentBackground( ::rux::gui::ColorBase* background )
			{
				READ_LOCK( _cs_content );
				if( rux_is_object( _content , ::rux::gui::controls::XGroup ) )
				{
					declare_variable_param( ::rux::gui::controls::XGroup , group , _content );
					_cs_content.ReadUnlock();
					XArray< ::rux::gui::controls::XRectangle > rects;
					rects.set_ByRef( group.FindAll< ::rux::gui::controls::XRectangle >() );
					::rux::uint32 index0 = 0;
					for( index0 = 0 ; index0 < rects.Count() ; index0++ )
					{
						rects[ index0 ].set_Background( background );
					}
					XArray< XPath > paths;
					paths.set_ByRef( group.FindAll< XPath >() );
					for( index0 = 0 ; index0 < paths.Count() ; index0++ )
					{
						paths[ index0 ].set_Background( background );
					}
					if( background->_color_type == 0 )
					{
						XArray< XLine > lines;
						lines.set_ByRef( group.FindAll< XLine >() );
						for( index0 = 0 ; index0 < lines.Count() ; index0++ )
							lines[ index0 ].set_Color( (rux::gui::Color*)background );
					}
					Invalidate();
				}
				else
					_cs_content.ReadUnlock();
			};
			void Button::set_ContentBorderColor( ::rux::gui::Color* border_color )
			{
				READ_LOCK( _cs_content );
				if( rux_is_object( _content , ::rux::gui::controls::XGroup ) )
				{
					declare_variable_param( ::rux::gui::controls::XGroup , group , _content );
					_cs_content.ReadUnlock();
					XArray< ::rux::gui::controls::XRectangle > rects;
					rects.set_ByRef( group.FindAll< ::rux::gui::controls::XRectangle >() );
					::rux::uint32 index0 = 0;
					for( index0 = 0 ; index0 < rects.Count() ; index0++ )
					{
						rects[ index0 ].set_BorderColor( border_color );
					}		
					Invalidate();
				}
				else
					_cs_content.ReadUnlock();
			};
			void Button::set_OverBackground( ::rux::gui::ColorBase* over_background )
			{	
				_cs_drawing_elements.wlock( debuginfo_macros );
				if( _active_state_background == _over_background )
				{
					_active_state_background = over_background;
					Invalidate();
				}
				rux::gui::copy_color( _over_background , over_background );
				_cs_drawing_elements.wunlock( debuginfo_macros );
			};
			void Button::set_PressedBackground( ::rux::gui::ColorBase* pressed_background )
			{	
				_cs_drawing_elements.wlock( debuginfo_macros );
				if( _active_state_background == _pressed_background )
				{
					_active_state_background = pressed_background;
					Invalidate();
				}
				rux::gui::copy_color( _pressed_background , pressed_background );
				_cs_drawing_elements.wunlock( debuginfo_macros );
			};
			::rux::uint8 Button::get_IsSupportContentSize( void )
			{
				return 1;
			};
			float Button::get_ContentWidth( void )
			{
				float value = 20.f;
				READ_LOCK( _cs_content );
				if( rux_is_object( _content, XTextBlock ) )
				{					
					XTextBlock textblock( _content );
					value = textblock.get_ContentWidth();
				}
				_cs_content.ReadUnlock();
				return value;
			};
			float Button::get_ContentHeight( void )
			{
				float value = 20.f;
				READ_LOCK( _cs_content );
				if( rux_is_object( _content, XTextBlock ) )
				{					
					XTextBlock textblock( _content );
					value = textblock.get_ContentHeight();
				}
				_cs_content.ReadUnlock();
				return value;
			};
			void* Button::DynamicCast( ::rux::int32 class_type ) const
			{
				if( class_type == ::rux::gui::XEnum_ClassType_ControlBase )
				{
					::rux::gui::ControlBase* ptr = const_cast< Button* >( this );
					return ptr;
				}
				else if( class_type == ::rux::gui::XEnum_ClassType_ParentBase )
				{
					::rux::gui::ParentBase* ptr = const_cast< Button* >( this );
					return ptr;
				}
				return 0;
			};
			::rux::uint8 Button::get_IsGroup( void )
			{
				return 0;
			};
			::rux::uint8 Button::get_IsTimePicker( void )
			{
				return 0;
			};
			::rux::uint8 Button::get_IsButton( void )
			{
				return 1;
			};
			void Button::set_DisabledBackground( ::rux::gui::ColorBase* disabled_background )
			{	
				_cs_drawing_elements.wlock( debuginfo_macros );
				if( _active_state_background == _disabled_background )
				{
					_active_state_background = disabled_background;
					Invalidate();
				}
				rux::gui::copy_color( _disabled_background , disabled_background );
				_cs_drawing_elements.wunlock( debuginfo_macros );
			};
			void Button::private_ResetChildLocationAndSizeCache( ::rux::uint8 reset , ::rux::gui::ControlBase* control , ::rux::uint8 left , ::rux::uint8 top , ::rux::uint8 width , ::rux::uint8 height , size_t )
			{	
				READ_LOCK( _cs_content );
				if( rux_is_object( _content, XTextBlock ) )
				{					
					XTextBlock ui_textblock( _content );
					WRITE_LOCK( ui_textblock()->_cs_parent_control );
					ui_textblock()->_parent_control = this;
					ui_textblock()->_cs_parent_control.WriteUnlock();
					ui_textblock()->_parent_window = get_ParentWindow();
					ui_textblock()->private_ResetLocationAndSizeCache( 1 , 1 , 1 , 1 , 0 );
					_cs_content.ReadUnlock();
				}
				else if( rux_is_object( _content, ::rux::gui::controls::XGroup ) )
				{
					declare_variable_param( ::rux::gui::controls::XGroup , ui_group , _content );
					ui_group.set_Left( 0 );
					ui_group.set_Top( 0 );
					ui_group.set_Width( get_Width() );
					ui_group.set_Height( get_Height() );
					WRITE_LOCK( ui_group()->_cs_parent_control );
					ui_group()->_parent_control = this;
					ui_group()->_cs_parent_control.WriteUnlock();
					ui_group()->_parent_window = get_ParentWindow();
					ui_group()->private_ResetLocationAndSizeCache( 1 , 1 , 1 , 1 , 0 );
					_cs_content.ReadUnlock();
				}
				else
					_cs_content.ReadUnlock();
				Invalidate();
			};
			void Button::render( ::rux::gui::RenderContextBase* render_context , float opacity , float& _selected_z_index , size_t ___rux__thread_index1986 )
			{	
				float left = get_Left();
				float top = get_Top();
				float width = get_Width();
				float height = get_Height();
				_cs_drawing_elements.wlock( debuginfo_macros );
				render_context->DrawRectangle( left , top , width , height , 0 , _active_state_background , _border_color , _corner , opacity * _opacity , &_ui_border_cache , &_ui_cache , _selected_z_index , 1 , __FILE__ , __LINE__ , ___rux__thread_index1986 );
				_cs_drawing_elements.wunlock( debuginfo_macros );
				READ_LOCK( _cs_content );
				if( rux_is_object( _content, XTextBlock ) )
				{
					_selected_z_index += 1.f;//0.001f
					XTextBlock ui_textblock( _content );		
					WRITE_LOCK( ui_textblock()->_cs_parent_control );
					ui_textblock()->_parent_control = this;
					ui_textblock()->_cs_parent_control.WriteUnlock();
					ui_textblock()->_parent_window = get_ParentWindow();
					_cs_content.ReadUnlock();
					ui_textblock()->_left = 0;
					ui_textblock()->_top = 0;
					ui_textblock()->_width = width;
					ui_textblock()->_height = height;
					ui_textblock()->_cs_drawing_elements.wlock( debuginfo_macros );
					rux::gui::copy_color( ui_textblock()->_foreground , _foreground );		
					ui_textblock()->_cs_drawing_elements.wunlock( debuginfo_macros );
					ui_textblock()->render( render_context , opacity * _opacity , _selected_z_index , ___rux__thread_index1986 );
				}
				else if( rux_is_object( _content, ::rux::gui::controls::XGroup ) )
				{
					_selected_z_index += 1.f;//0.001f
					declare_variable_param( ::rux::gui::controls::XGroup , ui_group , _content );
					WRITE_LOCK( ui_group()->_cs_parent_control );
					ui_group()->_parent_control = this;
					ui_group()->_cs_parent_control.WriteUnlock();
					ui_group()->_parent_window = get_ParentWindow();
					_cs_content.ReadUnlock();					
					ui_group()->render( render_context , opacity * _opacity , _selected_z_index , ___rux__thread_index1986 );		
				}
				else
					_cs_content.ReadUnlock();
			};
			void XButton::get_Clip( ::rux::gui::Rectangle*& clip )
			{
				(*this)()->get_Clip( clip );
			};
			void Button::RemoveControl( XGCRef* )
			{
			};
			void Button::RemoveTabControls( void )
			{
			};
			void Button::AddTabControls( void )
			{
			};
			void Button::get_Clip( ::rux::gui::Rectangle*& clip , ::rux::uint8 )
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
			::rux::byte Button::Exists( ::rux::gui::CommonBase* control )
			{
				return 0;
			};
			::rux::byte Button::raise_OnMouseEnter( ::rux::gui::WindowMouseEvent* , ::rux::byte& raised_event , ::rux::byte explicit_event )
			{
				_cs_drawing_elements.wlock( debuginfo_macros );
				_active_state_background = _over_background;
				_cs_drawing_elements.wunlock( debuginfo_macros );
				_on_mouse_enter.raise< const ::rux::gui::events::Event& >( ::rux::gui::events::Event( *this , explicit_event ) );				
				Invalidate();
				return 1;
			};
			void Button::raise_OnMouseLeave( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_cs_drawing_elements.wlock( debuginfo_macros );
				_active_state_background = _background;
				_cs_drawing_elements.wunlock( debuginfo_macros );
				_on_mouse_leave_callback.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
				Invalidate();
			};
			void Button::raise_OnKeyChar( const ::rux::XString& , ::rux::uint8 , ::rux::uint8 , ::rux::uint8 )
			{		
			};
			void Button::raise_OnKeyDown( ::rux::gui::XEnum_Keys , ::rux::uint8 , ::rux::uint8 , ::rux::uint8 , ::rux::uint32 , ::rux::byte explicit_event )
			{
			};
			void Button::raise_OnRightMouseButtonDown( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_right_mouse_button_down.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void Button::raise_OnRightMouseButtonDoubleClick( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_right_mouse_button_double_click.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void Button::raise_OnRightMouseButtonUp( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_right_mouse_button_up.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void Button::raise_OnMouseWheelDown(::rux::gui::WindowMouseEvent* window_event, ::rux::byte explicit_event)
			{
				_on_mouse_wheel_down.raise< const ::rux::gui::events::MouseEvent& >(
					::rux::gui::events::MouseEvent(*this, window_event, explicit_event));
			};
			void Button::raise_OnMouseWheelDoubleClick(::rux::gui::WindowMouseEvent* window_event
				, ::rux::byte explicit_event)
			{
				_on_mouse_wheel_double_click.raise< const ::rux::gui::events::MouseEvent& >(
					::rux::gui::events::MouseEvent(*this, window_event, explicit_event));
			};
			void Button::raise_OnMouseWheelUp(::rux::gui::WindowMouseEvent* window_event, ::rux::byte explicit_event)
			{
				_on_mouse_wheel_up.raise< const ::rux::gui::events::MouseEvent& >(
					::rux::gui::events::MouseEvent(*this, window_event, explicit_event));
			};
			void Button::raise_OnLeftMouseButtonDown( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_left_mouse_button_down.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , 1 ) );
				_cs_drawing_elements.wlock( debuginfo_macros );
				_active_state_background = _pressed_background;
				_cs_drawing_elements.wunlock( debuginfo_macros );
				Invalidate();	
				if( _is_repeat_until_mouse_up == 1 )
				{
					_repeat_thread.set_ThreadName( "button repeat" );
					_repeat_thread.set_ThreadMode( XThreadMode_Repeat );
					_repeat_thread.set_Timeout( 200 );
					_repeat_thread.Start();
					_repeat_thread.Push( repeat_thread , this );
				}
				else
					_on_click_callback.raise< const ::rux::gui::events::Event& >( ::rux::gui::events::Event( *this , 1 ) );
			};
			void Button::repeat_thread( void* param , size_t ___rux__thread_index1986 )
			{
				UNREFERENCED_PARAMETER( ___rux__thread_index1986 );
				Button* button = (Button*)param;
				button->_on_click_callback.raise< const ::rux::gui::events::Event& >( ::rux::gui::events::Event( *button , 1 ) );
			};
			void Button::raise_OnLeftMouseButtonDoubleClick( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_cs_drawing_elements.wlock( debuginfo_macros );
				_active_state_background = _pressed_background;
				_cs_drawing_elements.wunlock( debuginfo_macros );
				Invalidate();
				_on_click_callback.raise< const ::rux::gui::events::Event& >( ::rux::gui::events::Event( *this , 1 ) );
			};
			void Button::raise_OnLeftMouseButtonUp( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_left_mouse_button_up.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , 1 ) );
				_repeat_thread.Stop();
				if( get_Left() <= window_event->_x && get_Left() + get_Width() >= window_event->_x
					&& get_Top() <= window_event->_y && get_Top() + get_Height() >= window_event->_y )
				{
					_cs_drawing_elements.wlock( debuginfo_macros );
					_active_state_background = _over_background;
					_cs_drawing_elements.wunlock( debuginfo_macros );
				}
				else
				{
					_cs_drawing_elements.wlock( debuginfo_macros );
					_active_state_background = _background;
					_cs_drawing_elements.wunlock( debuginfo_macros );
				}	
				Invalidate();
			};
			void Button::set_Content( const XObject& content )
			{
				set_Content( *content.get_GCRef() );
			};
			void Button::set_Content( const Object& content )
			{
				content.AddRef();
				if( rux_is_object( content , ::rux::XString ) )
				{
					::rux::XString content_string;
					content_string.set_ByRef( content );
					WRITE_LOCK( _cs_content );
					if( rux_is_object( _content, XTextBlock ) )
					{
						XTextBlock ui_textblock( _content );
						WRITE_LOCK( ui_textblock()->_cs_parent_control );
						ui_textblock()->_parent_control = this;
						ui_textblock()->_cs_parent_control.WriteUnlock();
						ui_textblock()->_parent_window = get_ParentWindow();
						_cs_content.WriteUnlock();
						ui_textblock.set_Text( content_string );
					}
					else
					{
						XTextBlock ui_textblock;
						WRITE_LOCK( ui_textblock()->_cs_parent_control );
						ui_textblock()->_parent_control = this;
						ui_textblock()->_cs_parent_control.WriteUnlock();
						ui_textblock()->_parent_window = get_ParentWindow();						
						ui_textblock.set_Text( content_string );
						_content = ui_textblock;
						_cs_content.WriteUnlock();
					}		
				}
				else if( rux_is_object( content , ::rux::gui::controls::XGroup ) )
				{
					rux::gui::controls::XGroup group( content );
					group.set_Left( 0 );
					group.set_Top( 0 );
					group.set_Width( get_Width() );
					group.set_Height( get_Height() );
					WRITE_LOCK( _cs_content );
					WRITE_LOCK( group()->_cs_parent_control );
					group()->_parent_control = this;
					group()->_cs_parent_control.WriteUnlock();
					group()->_parent_window = get_ParentWindow();
					_content = content;
					_cs_content.WriteUnlock();
				}	
				else if( rux_is_object( content , ::rux::media::XFrame ) )
				{
					rux::media::XFrame frame;
					frame.set_ByRef( content );
					XImage image;
					image.set_IsAlpha( 1 );
					image.set_Frame( frame );
					image.set_HorizontalFilling( XEnum_Filling_Auto );
					image.set_VerticalFilling( XEnum_Filling_Auto );
					declare_variable( ::rux::gui::controls::XGroup , group );
					group.set_Left( 0 );
					group.set_Top( 0 );
					group.set_Width( get_Width() );
					group.set_Height( get_Height() );
					WRITE_LOCK( group()->_cs_parent_control );
					group()->_parent_control = this;
					group()->_cs_parent_control.WriteUnlock();
					group()->_parent_window = get_ParentWindow();
					rux::XString error;
					group.AddControl( image , error );
					WRITE_LOCK( _cs_content );
					_content = group;
					_cs_content.WriteUnlock();
				}
				content.Release();
				if( _horizontal_filling == ::rux::gui::XEnum_Filling_FromContent )
					private_ResetLocationAndSizeCache( 0 , 0 , 1 , 0 );
				if( _vertical_filling == ::rux::gui::XEnum_Filling_FromContent )
					private_ResetLocationAndSizeCache( 0 , 0 , 0 , 1 );
			};
			void Button::set_Content( const XGCRef& content )
			{
				content.AddRef();
				if( rux_is_object( content , ::rux::XString ) )
				{
					rux::XString content_string;
					content_string.set_ByRef( content );
					WRITE_LOCK( _cs_content );
					if( rux_is_object( _content, XTextBlock ) )
					{
						XTextBlock ui_textblock( _content );			
						_cs_content.WriteUnlock();
						WRITE_LOCK( ui_textblock()->_cs_parent_control );
						ui_textblock()->_parent_control = this;
						ui_textblock()->_cs_parent_control.WriteUnlock();
						ui_textblock()->_parent_window = get_ParentWindow();
						ui_textblock.set_Text( content_string );
					}
					else
					{
						XTextBlock ui_textblock;
						WRITE_LOCK( ui_textblock()->_cs_parent_control );
						ui_textblock()->_parent_control = this;
						ui_textblock()->_cs_parent_control.WriteUnlock();
						ui_textblock()->_parent_window = get_ParentWindow();
						ui_textblock.set_Text( content_string );
						_content = ui_textblock;
						_cs_content.WriteUnlock();
					}		
				}
				else if( rux_is_object( content , ::rux::gui::controls::XGroup ) )
				{
					rux::gui::controls::XGroup group( content );
					group.set_Left( 0 );
					group.set_Top( 0 );
					group.set_Width( get_Width() );
					group.set_Height( get_Height() );
					WRITE_LOCK( _cs_content );
					WRITE_LOCK( group()->_cs_parent_control );
					group()->_parent_control = this;
					group()->_cs_parent_control.WriteUnlock();
					group()->_parent_window = get_ParentWindow();
					_content = content;
					_cs_content.WriteUnlock();
				}	
				else if( rux_is_object( content , ::rux::media::XFrame ) )
				{
					rux::media::XFrame frame;
					frame.set_ByRef( content );
					XImage image;
					image.set_IsAlpha( 1 );
					image.set_Frame( frame );
					image.set_HorizontalFilling( XEnum_Filling_Auto );
					image.set_VerticalFilling( XEnum_Filling_Auto );
					declare_variable( ::rux::gui::controls::XGroup , group );
					group.set_Left( 0 );
					group.set_Top( 0 );
					group.set_Width( get_Width() );
					group.set_Height( get_Height() );
					WRITE_LOCK( group()->_cs_parent_control );
					group()->_parent_control = this;
					group()->_cs_parent_control.WriteUnlock();
					group()->_parent_window = get_ParentWindow();
					rux::XString error;
					group.AddControl( image , error );
					WRITE_LOCK( _cs_content );
					_content = group;
					_cs_content.WriteUnlock();
				}
				content.Release();
				if( _horizontal_filling == ::rux::gui::XEnum_Filling_FromContent )
					private_ResetLocationAndSizeCache( 0 , 0 , 1 , 0 );
				if( _vertical_filling == ::rux::gui::XEnum_Filling_FromContent )
					private_ResetLocationAndSizeCache( 0 , 0 , 0 , 1 );
			};
			void XButton::set_Font( const char* font_file_name ,
				::rux::uint32 font_size_height ,
				::rux::uint32 font_size_width )
			{
				(*this)()->_ui_font = ::rux::engine::_globals->_gui_globals->_rux_gui_create_font( font_file_name ,
					font_size_height ,
					font_size_width );
				(*this)()->Invalidate();
			};
			void XButton::set_Foreground( ::rux::gui::Color* foreground )
			{
				(*this)()->_cs_drawing_elements.wlock( debuginfo_macros );
				rux::gui::copy_color( (*this)()->_foreground , foreground );
				(*this)()->_cs_drawing_elements.wunlock( debuginfo_macros );
			};
			implement_duplicate_internal_function_1( Button , set_OnClick , ::rux::gui::events::on_event_t );
			void Button::set_OnClick( ::rux::gui::events::on_event_t on_click_callback )
			{
				_on_click_callback = on_click_callback;
			};
			implement_duplicate_internal_function_1( Button , set_Corner , const ::rux::gui::Thickness& );
			void Button::set_Corner( const ::rux::gui::Thickness& corner )
			{
				_cs_drawing_elements.wlock( debuginfo_macros );
				if( _corner != corner )
					_corner = corner;
				_cs_drawing_elements.wunlock( debuginfo_macros );
			};
			void Button::UpdateParentWindow( void )
			{
				_parent_window = get_ParentWindow();
			};
			void Button::FreeResources( ::rux::byte from_removed_controls_schedule )
			{
				WRITE_LOCK( _cs_content );
				if( rux_is_object( _content, XTextBlock ) )
				{		
					XTextBlock ui( _content );
					_cs_content.WriteUnlock();
					ui()->BeforeFreeSystemResources();
					ui()->FreeResources( from_removed_controls_schedule );
					ui()->FreeSystemResources();
				}
				else if( rux_is_object( _content , ::rux::gui::controls::XGroup ) )
				{
					declare_variable_param( ::rux::gui::controls::XGroup , ui , _content );
					_cs_content.WriteUnlock();
					ui()->BeforeFreeSystemResources();
					ui()->FreeResources( from_removed_controls_schedule );
					ui()->FreeSystemResources();
				}
				else
					_cs_content.WriteUnlock();
				::rux::engine::_globals->_gui_globals->_rux_gui_add_cache( _ui_cache );
				_ui_cache = NULL;
				::rux::engine::_globals->_gui_globals->_rux_gui_add_cache( _ui_border_cache );
				_ui_border_cache = NULL;
				_current_render_index = SIZE_MAX;
			};
			::rux::uint8 XButton::get_IsTransparent( void )
			{
				return 0;
			};
			void Button::raise_OnMouseMove( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{	
				_on_mouse_move.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void Button::raise_OnMouseWheel( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_mouse_wheel.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void XButton::set_IsRepeatUntilMouseUp( ::rux::uint8 is_repeat_until_mouse_up )
			{
				(*this)()->_is_repeat_until_mouse_up = is_repeat_until_mouse_up;
			};
			IMPLEMENT_BASE_UI_FUNCTIONS( XButton );
		};
	};
};