#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_uitextblock.h>
#include <xan_uiinlines.h>
#include <xan_uibutton.h>
#include <xan_keyvaluepair.h>
begin_implement_rux_class_with_properties_ns_base_class( TextBlock , rux::gui::controls , rux::gui::ControlBase )
	_tag.set_Info( "tag" , __file__ , __line__ );
	_private_tag.set_Info( "private_tag" , __file__ , __line__ );
	IMPLEMENT_BASE_UI_MEMBERS();
	_text_need_refresh = 1;
	copy_color( _border_object , create_color( 0 , 0 , 255 , 0 ) );
	copy_color( _background_object , create_color( 255 , 255 , 255 , 0 ) );
	_font = ::rux::engine::_globals->_gui_globals->_rux_gui_create_font( "tahoma.ttf" , 0 , 12 );
	copy_color( _foreground , create_color( 255 , 255 , 255 , 255 ) );
	_vertical_text_alignment = XEnum_Alignment_Center;
	_horizontal_text_alignment = XEnum_Alignment_Center;
	_border_width = 0.f;
	_text_left_margin = 0.f;
	_text_top_margin = 0.f;
	_text_left = 0.f;
	_text_top = 0.f;
	_text_width = 0.f;
	_text_height = 0.f;
	_render_text_width = 0.f;
	_render_text_height = 0.f;
	_text_hash = ::rux::cryptography::hash::times33_hash( _utf8text.str() , _utf8text.Size() - 1 );
	_reset_cache = 1;
end_implement_rux_class();
namespace rux
{
	namespace gui
	{
		namespace controls
		{
			implement_duplicate_internal_result_function( float , TextBlock , get_TextWidth );
			implement_duplicate_internal_result_function( float , TextBlock , get_TextHeight );
			TextBlock::~TextBlock()
			{		
				FreeResources( 0 );
				if( _foreground )
					_foreground->Release();
				if( _background_object )
					_background_object->Release();
				if( _border_object )
					_border_object->Release();
			};
			begin_implement_rux_base_ui_functions( TextBlock )
				register_rux_property( XColorWrap , Foreground );
			end_implement_rux_base_ui_functions();
			implement_rux_color_property( TextBlock , Foreground );
			::rux::gui::ColorBase* TextBlock::get_Foreground( void )
			{
				return _foreground;
			};
			void* TextBlock::DynamicCast( ::rux::int32 class_type ) const
			{
				if( class_type == ::rux::gui::XEnum_ClassType_ControlBase )
				{
					::rux::gui::ControlBase* ptr = const_cast< TextBlock* >( this );
					return ptr;
				}
				return 0;
			};
			implement_duplicate_internal_function_1( TextBlock , set_BorderColor , ::rux::gui::ColorBase* );
			void TextBlock::set_BorderColor( ::rux::gui::ColorBase* border_color )
			{
				_cs_drawing_elements.wlock( debuginfo_macros );
				rux::gui::copy_color( _border_object , border_color );
				_cs_drawing_elements.wunlock( debuginfo_macros );
			};
			implement_duplicate_internal_function_1( TextBlock , set_BorderWidth , float );
			void TextBlock::set_BorderWidth( float border_width )
			{
				if( _border_width != border_width )
					_border_width = ::rux::math::round( border_width , 0 );
			};
			::rux::uint8 TextBlock::get_IsGroup( void )
			{
				return 0;
			};
			::rux::uint8 TextBlock::get_IsTimePicker( void )
			{
				return 0;
			};
			::rux::uint8 TextBlock::get_IsButton( void )
			{
				return 0;
			};
			void XTextBlock::set_Corner( const ::rux::gui::Thickness& corner )
			{
				(*this)()->_cs_drawing_elements.wlock( debuginfo_macros );
				(*this)()->_corner = corner;
				(*this)()->_cs_drawing_elements.wunlock( debuginfo_macros );
			};
			implement_duplicate_internal_function_2( TextBlock , set_TextMargin , float , float );
			void TextBlock::set_TextMargin( float left , float top )
			{
				_cs_drawing_elements.wlock( debuginfo_macros );
				if( _text_left_margin != left 
					|| _text_top_margin != top )
				{
					_text_left_margin = left;
					_text_top_margin = top;					
					_cs_drawing_elements.wunlock( debuginfo_macros );
					private_ResetCache();
				}
				else
					_cs_drawing_elements.wunlock( debuginfo_macros );
			};
			::rux::uint8 TextBlock::get_IsTransparent( void )
			{
				return 0;
			};
			void TextBlock::raise_OnMouseWheel( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
			};
			XTextBlock::XTextBlock( const TextBlock& textblock , const char* variable_name , const char* __file__ , ::rux::int32 __line__ )
				: XGCHandle< TextBlock >( variable_name , __file__ , __line__ )
			{
				(*this) = textblock;
			};
			XTextBlock::XTextBlock( const XTextBlock& textblock , const char* variable_name , const char* __file__ , ::rux::int32 __line__ )
				: XGCHandle< TextBlock >( variable_name , __file__ , __line__ )
			{
				(*(static_cast< XGCHandle< TextBlock >* >( this ))) = textblock;
			};
			implement_duplicate_internal_function_3( TextBlock , set_Font , const char* , ::rux::uint32 , ::rux::uint32 );
			void TextBlock::set_Font( const char* font_file_name ,
				::rux::uint32 font_size_height ,
				::rux::uint32 font_size_width )
			{
				_cs_drawing_elements.wlock( debuginfo_macros );
				_font = ::rux::engine::_globals->_gui_globals->_rux_gui_create_font( font_file_name ,
					font_size_height ,
					font_size_width );
				_cs_drawing_elements.wunlock( debuginfo_macros );
				private_ResetLocationAndSizeCache( _horizontal_alignment == XEnum_Alignment_Stick || _horizontal_alignment == XEnum_Alignment_RightOrBottom || _horizontal_alignment == XEnum_Alignment_Center ? 1 : 0 , _vertical_alignment == XEnum_Alignment_Stick || _vertical_alignment == XEnum_Alignment_RightOrBottom || _vertical_alignment == XEnum_Alignment_Center ? 1 : 0 , _horizontal_filling == XEnum_Filling_FromContent ? 1 : 0 , _vertical_filling == XEnum_Filling_FromContent ? 1 : 0 );
				private_ResetCache();
			};
			rux::String& XTextBlock::get_Text( void )
			{
				return (*this)()->get_Text();
			};
			float TextBlock::get_TextWidth( void )
			{
				return _font->get_TextWidth( _utf8text.str() , SIZE_MAX );
			};
			float TextBlock::get_TextHeight( void )
			{
				return _font->get_Height( _utf8text.str() , SIZE_MAX );
			};
			rux::String& TextBlock::get_Text( void )
			{
				return _utf8text++;
			};
			void XTextBlock::set_Text( const ::rux::XString& text )
			{	
				(*this)()->set_Text( text );
			};
			void TextBlock::set_Text( const ::rux::XString& text )
			{
				::rux::XString utf8_text;
				utf8_text.set_ByRef( text.ConvertToUTF8() );
				set_Text( utf8_text.str() , utf8_text.Size() - 1 );
			};
			void XTextBlock::set_Text( const char* text , size_t utf8bytes )
			{	
				(*this)()->set_Text( text , utf8bytes );
			};
			void TextBlock::set_Text( const char* utf8text , size_t utf8bytes )
			{
				::rux::uint32 text_hash = ::rux::cryptography::hash::times33_hash( utf8text , utf8bytes );
				if( _text_hash != text_hash )
				{
					_cs_drawing_elements.wlock( debuginfo_macros );
					_text_hash = text_hash;
					_utf8text()->set( utf8text , XEnumCodePage_UTF8 , 1 );
					_cs_drawing_elements.wunlock( debuginfo_macros );
					private_ResetLocationAndSizeCache( _horizontal_alignment == XEnum_Alignment_Stick || _horizontal_alignment == XEnum_Alignment_RightOrBottom || _horizontal_alignment == XEnum_Alignment_Center ? 1 : 0 , _vertical_alignment == XEnum_Alignment_Stick || _vertical_alignment == XEnum_Alignment_RightOrBottom || _vertical_alignment == XEnum_Alignment_Center ? 1 : 0 , _horizontal_filling == XEnum_Filling_FromContent ? 1 : 0 , _vertical_filling == XEnum_Filling_FromContent ? 1 : 0 );
					private_ResetCache();
				}
			};
			::rux::uint8 TextBlock::get_IsSupportContentSize( void )
			{
				return 1;
			};
			implement_duplicate_internal_result_function( float , TextBlock , get_ContentWidth );
			float TextBlock::get_ContentWidth( void )
			{
				float val = 0.f;
				_cs_drawing_elements.wlock( debuginfo_macros );
				if( ::XInterlocked::CompareExchange( &_reset_cache , 0 , 1 ) == 1 )
				{
					_text_need_refresh = 1;
					_text_width = _font->get_TextWidth( _utf8text.str() , SIZE_MAX );
					_text_height = _font->get_Height( _utf8text.str() , SIZE_MAX );
				}
				val = _text_width;
				_cs_drawing_elements.wunlock( debuginfo_macros );
				return val;
			};
			implement_duplicate_internal_result_function( float , TextBlock , get_ContentHeight );
			float TextBlock::get_ContentHeight( void )
			{			
				float val = 0.f;
				_cs_drawing_elements.wlock( debuginfo_macros );
				if( ::XInterlocked::CompareExchange( &_reset_cache , 0 , 1 ) == 1 )
				{
					_text_need_refresh = 1;
					_text_width = _font->get_TextWidth( _utf8text.str() , SIZE_MAX );
					_text_height = _font->get_Height( _utf8text.str() , SIZE_MAX );
				}
				val = _text_height;
				_cs_drawing_elements.wunlock( debuginfo_macros );
				return val;
			};
			void TextBlock::set_Foreground( ::rux::gui::ColorBase* foreground )
			{
				_cs_drawing_elements.wlock( debuginfo_macros );
				rux::gui::copy_color( _foreground , foreground );
				_cs_drawing_elements.wunlock( debuginfo_macros );
			};
			void TextBlock::set_Background( ::rux::gui::ColorBase* background )
			{
				_cs_drawing_elements.wlock( debuginfo_macros );
				rux::gui::copy_color( _background_object , background );
				_cs_drawing_elements.wunlock( debuginfo_macros );
				private_ResetCache();
			};
			void TextBlock::render( ::rux::gui::RenderContextBase* render_context , float opacity , float& _selected_z_index , size_t ___rux__thread_index1986 )
			{	
				if( ::XInterlocked::CompareExchange( &_reset_cache , 0 , 1 ) == 1 )
				{
					_cs_drawing_elements.wlock( debuginfo_macros );
					_text_need_refresh = 1;
					_text_width = _font->get_TextWidth( _utf8text.str() , ___rux__thread_index1986 );
					_text_height = _font->get_Height( _utf8text.str() , ___rux__thread_index1986 );
					_cs_drawing_elements.wunlock( debuginfo_macros );
				}
				float __left = get_Left();
				float __top = get_Top();
				float __width = get_Width();
				float __height = get_Height();
				_cs_drawing_elements.wlock( debuginfo_macros );
				render_context->DrawRectangle( __left , __top , __width , __height , _border_width , _background_object , _border_object , _corner , _opacity * opacity , &_ui_border_cache , &_ui_cache , _selected_z_index , 1 , __FILE__ , __LINE__ , ___rux__thread_index1986 );
				if( _text_need_refresh )
				{
					_text_need_refresh = 0;
					_render_text_width = __width;
					_text_left = __left;
					_render_text_height = __height;
					_text_top = __top;
					float offset = 0.f;
					if( _horizontal_text_alignment == XEnum_Alignment_Center )
					{
						if( _text_width < _render_text_width )
						{
							offset = ( _render_text_width - _text_width ) / 2;
							_text_left += offset;
							_render_text_width -= offset;
						}
					}
					else if( _horizontal_text_alignment == XEnum_Alignment_RightOrBottom )
					{
						if( _text_width < _render_text_width )
						{
							offset = _render_text_width - _text_width;
							_text_left += offset;
							_render_text_width -= offset;
						}		
					}
					if( _vertical_text_alignment == XEnum_Alignment_Center )
					{		
						if( _text_height < _render_text_height )
						{
							offset = ( _render_text_height - _text_height ) / 2;
							_text_top += offset;
							_render_text_height -= offset;
						}
					}
					else if( _vertical_text_alignment == XEnum_Alignment_RightOrBottom )
					{		
						if( _text_height < _render_text_height )
						{
							offset = ( _render_text_height - _text_height ) / 2;
							_text_top += offset;
							_render_text_height -= offset;
						}
					}
					_text_left += _text_left_margin;
					_text_top += _text_top_margin;
				}
				render_context->DrawText( _text_left , _text_top , _render_text_width , _render_text_height , _utf8text.str() , _font , _foreground , &_text_cache , _opacity * opacity , _selected_z_index , __FILE__ , __LINE__ , ___rux__thread_index1986 );
				_cs_drawing_elements.wunlock( debuginfo_macros );
			};
			void TextBlock::Invalidate( void )
			{
				private_ResetCache();
			};
			void TextBlock::private_ResetCache( void )
			{
				::XInterlocked::Exchange( &_reset_cache , 1 );
			};
			void TextBlock::raise_OnActivate( void )
			{		
			};
			void TextBlock::raise_OnDeactivate( ::rux::gui::CommonBase* activated_control )
			{		
				XObject activated;
				if( activated_control )
					activated.set_ByRef( XObject( activated_control , 1 ) );
				_on_deactivated.raise< const XObject& , const XObject& >( XObject( *this ) , activated );
			};
			void TextBlock::set_TextVerticalAlignment( ::rux::gui::XEnum_Alignment text_alignment )
			{
				_cs_drawing_elements.wlock( debuginfo_macros );
				if( _vertical_text_alignment != text_alignment )
				{
					_vertical_text_alignment = text_alignment;					
					_cs_drawing_elements.wunlock( debuginfo_macros );
					private_ResetCache();
				}
				else
					_cs_drawing_elements.wunlock( debuginfo_macros );
			};
			void TextBlock::set_TextHorizontalAlignment( ::rux::gui::XEnum_Alignment text_alignment )
			{
				_cs_drawing_elements.wlock( debuginfo_macros );
				if( _horizontal_text_alignment != text_alignment )
				{
					_horizontal_text_alignment = text_alignment;										
					_cs_drawing_elements.wunlock( debuginfo_macros );
					private_ResetCache();
				}
				else
					_cs_drawing_elements.wunlock( debuginfo_macros );
			};
			void TextBlock::UpdateParentWindow( void )
			{
				_parent_window = get_ParentWindow();
			};
			void TextBlock::FreeResources( ::rux::byte )
			{
				::rux::engine::_globals->_gui_globals->_rux_gui_add_cache( _ui_cache );
				_ui_cache = NULL;
				::rux::engine::_globals->_gui_globals->_rux_gui_add_cache( _ui_border_cache );
				_ui_border_cache = NULL;
				::rux::engine::_globals->_gui_globals->_rux_gui_add_cache( _text_cache );
				_text_cache = NULL;
			};
			void TextBlock::raise_OnLeftMouseButtonDoubleClick( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_mouse_double_click.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void TextBlock::raise_OnLeftMouseButtonUp( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_left_mouse_button_up.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void TextBlock::raise_OnKeyChar( const ::rux::XString& , ::rux::uint8 , ::rux::uint8 , ::rux::uint8 )
			{		
			};
			void TextBlock::raise_OnKeyDown( ::rux::gui::XEnum_Keys , ::rux::uint8 , ::rux::uint8 , ::rux::uint8 , ::rux::uint32 , ::rux::byte explicit_event )
			{
			};
			void TextBlock::raise_OnRightMouseButtonDown( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_right_mouse_button_down.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void TextBlock::raise_OnRightMouseButtonDoubleClick( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_right_mouse_button_double_click.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void TextBlock::raise_OnRightMouseButtonUp( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_right_mouse_button_up.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void TextBlock::raise_OnMouseWheelDown(::rux::gui::WindowMouseEvent* window_event, ::rux::byte explicit_event)
			{
				_on_mouse_wheel_down.raise< const ::rux::gui::events::MouseEvent& >(
					::rux::gui::events::MouseEvent(*this, window_event, explicit_event));
			};
			void TextBlock::raise_OnMouseWheelDoubleClick(::rux::gui::WindowMouseEvent* window_event
				, ::rux::byte explicit_event)
			{
				_on_mouse_wheel_double_click.raise< const ::rux::gui::events::MouseEvent& >(
					::rux::gui::events::MouseEvent(*this, window_event, explicit_event));
			};
			void TextBlock::raise_OnMouseWheelUp(::rux::gui::WindowMouseEvent* window_event, ::rux::byte explicit_event)
			{
				_on_mouse_wheel_up.raise< const ::rux::gui::events::MouseEvent& >(
					::rux::gui::events::MouseEvent(*this, window_event, explicit_event));
			};
			void TextBlock::raise_OnLeftMouseButtonDown( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_left_mouse_button_down.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , 1 ) );
			};
			::rux::byte TextBlock::raise_OnMouseEnter( ::rux::gui::WindowMouseEvent* , ::rux::byte& raised_event , ::rux::byte explicit_event )
			{
				_on_mouse_enter.raise< const ::rux::gui::events::Event& >( ::rux::gui::events::Event( *this , explicit_event ) );
				return 1;
			};
			void TextBlock::raise_OnMouseLeave( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_mouse_leave_callback.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void TextBlock::raise_OnMouseMove( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
			};
			implement_duplicate_internal_function_1( TextBlock , set_Foreground , ::rux::gui::ColorBase* );
			implement_duplicate_internal_function_1( TextBlock ,set_TextVerticalAlignment , ::rux::gui::XEnum_Alignment );
			implement_duplicate_internal_function_1( TextBlock , set_TextHorizontalAlignment , ::rux::gui::XEnum_Alignment );
			implement_duplicate_internal_function_1( TextBlock , set_Background , ::rux::gui::ColorBase* );
			IMPLEMENT_BASE_UI_FUNCTIONS( XTextBlock );
		};
	};
};