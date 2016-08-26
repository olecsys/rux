#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_uitextbox.h>
#include <xan_uiinlines.h>
#include <xan_uibutton.h>
#include <xan_uiclipboard.h>
#include <xan_keyvaluepair.h>
#include <xan_math.h>
begin_implement_rux_class_with_properties_ns_base_class( TextBox , rux::gui::controls , rux::gui::ControlBase )
	_tag.set_Info( "tag" , __file__ , __line__ );
	_private_tag.set_Info( "private_tag" , __file__ , __line__ );
	IMPLEMENT_BASE_UI_MEMBERS();
	_mask_corner = ::rux::gui::Thickness( 1.f , 1.f , 1.f , 1.f );
	copy_color( _selection_background , create_color( 153 , 204 , 255 , 255 ) );
	_border_thickness = 1.f;
	copy_color( _border_color , create_color( 153 , 153 , 153 , 255 ) );
	copy_color( _background , create_color( 255 , 255 , 255 , 255 ) );
	copy_color( _foreground , create_color( 0 , 0 , 0 , 255 ) );
	_lines.Add( ::rux::XString() );
	_font = ::rux::engine::_globals->_gui_globals->_rux_gui_create_font( "tahoma.ttf" , 0 , 11 );			
	_selection_char_index = 0;
	_old_selection_char_index = 0;
	_scrollbar_left_offset = 0.f;
	_scrollbar_top_offset = 0.f;
	_is_multi_lines = 0;
	_is_regexp_pattern = 0;
	_is_left_mouse_down_selection_regexp_pattern = 0;
	_on_text_changed_event_callback = NULL;
	_horizontal_text_alignment = XEnum_Alignment_Manual;
	_is_masked = 0;
end_implement_rux_class();
namespace rux
{
	namespace gui
	{
		namespace controls
		{
			TextBox::~TextBox()
			{
				FreeResources( 0 );
				if( _foreground )
					_foreground->Release();
				if( _background )
					_background->Release();
				if( _border_color )
					_border_color->Release();
				if( _selection_background )
					_selection_background->Release();
			};
			begin_implement_rux_base_ui_functions( TextBox )
				register_rux_property( XColorWrap , Foreground );
			end_implement_rux_base_ui_functions();
			implement_rux_color_property( TextBox , Foreground );
			void* TextBox::DynamicCast( ::rux::int32 class_type ) const
			{
				if( class_type == ::rux::gui::XEnum_ClassType_ControlBase )
				{
					::rux::gui::ControlBase* ptr = const_cast< TextBox* >( this );
					return ptr;
				}
				return 0;
			};
			::rux::uint8 TextBox::get_IsGroup( void )
			{
				return 0;
			};
			::rux::uint8 TextBox::get_IsTimePicker( void )
			{
				return 0;
			};
			::rux::uint8 TextBox::get_IsButton( void )
			{
				return 0;
			};
			::rux::uint8 TextBox::get_IsTransparent( void )
			{
				return 0;
			};
			void TextBox::raise_OnMouseWheel( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
			};
			void XTextBox::set_OnTextChanged( ::rux::gui::events::on_event_t on_text_changed_event_callback )
			{
				(*this)()->_on_text_changed_event_callback = on_text_changed_event_callback;
			};
			implement_duplicate_internal_function_1( TextBox , set_Foreground , ::rux::gui::ColorBase* );
			void TextBox::set_Foreground( ::rux::gui::ColorBase* foreground )
			{				
				_cs_drawing_elements.wlock( debuginfo_macros );
				rux::gui::copy_color( _foreground , foreground );
				_cs_drawing_elements.wunlock( debuginfo_macros );
			};
			::rux::gui::ColorBase* TextBox::get_Foreground( void )
			{
				return _foreground;
			};
			void XTextBox::set_Font( const char* font_file_name ,
				::rux::uint32 font_size_height ,
				::rux::uint32 font_size_width )
			{
				(*this)()->_cs_drawing_elements.wlock( debuginfo_macros );
				(*this)()->_font = ::rux::engine::_globals->_gui_globals->_rux_gui_create_font( font_file_name ,
					font_size_height ,
					font_size_width );
				(*this)()->_cs_drawing_elements.wunlock( debuginfo_macros );	
			};
			rux::String& XTextBox::get_Text( void )
			{
				rux::XString string;
				if( (*this)()->_is_multi_lines == 1 )
				{
					(*this)()->_cs_drawing_elements.wlock( debuginfo_macros );
					if( (*this)()->_lines.Count() > 0 )
						string.set_ByRef( ::rux::XString::Join( "\n" , (*this)()->_lines ) );
					(*this)()->_cs_drawing_elements.wunlock( debuginfo_macros );
				}
				else
				{
					(*this)()->_cs_drawing_elements.wlock( debuginfo_macros );
					if( (*this)()->_lines.Count() > 0 )
					{
						string = (*this)()->_lines[ 0 ];
						string.set_ByRef( string.ReplaceAll( '\r' , '\n' ) );
					}
					(*this)()->_cs_drawing_elements.wunlock( debuginfo_macros );
				}
				return string++;
			};
			void XTextBox::set_IsMultiLines( ::rux::uint8 is_multi_lines )
			{
				if( (*this)()->_is_multi_lines != is_multi_lines )
				{
					(*this)()->_is_multi_lines = is_multi_lines;
					set_Text( get_Text() );
					(*this)()->Invalidate();
				}
			};
			void XTextBox::set_LeftMouseDownSelectionRegexpMask( const ::rux::XString& mask , ::rux::XString& error )
			{
				error.Clear();
				(*this)()->_left_mouse_down_selection_regexp_pattern = XRegexp::get_RegexpPattern( mask , error );
				if( error.Length() == 0 )
					(*this)()->_is_left_mouse_down_selection_regexp_pattern = 1;
				else
					(*this)()->_is_left_mouse_down_selection_regexp_pattern = 0;
			}
			void XTextBox::set_RegexpMask( const ::rux::XString& mask , ::rux::XString& error )
			{
				error.Clear();
				(*this)()->_regexp_pattern = XRegexp::get_RegexpPattern( mask , error );
				if( error.Length() == 0 )
				{
					(*this)()->_is_regexp_pattern = 1;
					set_Text( XRegexp::Correct( get_Text() , (*this)()->_regexp_pattern ) );
					(*this)()->Invalidate();
				}
				else
					(*this)()->_is_regexp_pattern = 0;
			};
			void XTextBox::set_Text( const ::rux::XString& text )
			{	
				rux::XString string;
				if( (*this)()->_is_multi_lines == 1 )
				{
					XArray< ::rux::XString > line_separators;
					line_separators.Add( ::rux::XString( "\n" ) );
					line_separators.Add( ::rux::XString( "\r" ) );		
					declare_variable( ::rux::XArray< ::rux::XString > , lines );
					lines.set_ByRef( text.Split( line_separators , EStringSplitOptions_None ) );
					(*this)()->_cs_drawing_elements.wlock( debuginfo_macros );
					(*this)()->_lines.Clear();
					for( size_t index0 = 0 ; index0 < lines.Count() ; index0++ )
						(*this)()->_lines.AddByRef( lines[ index0 ].ConvertToUTF8() );
					(*this)()->_cs_drawing_elements.wunlock( debuginfo_macros );
				}
				else
				{
					string.set_ByRef( text.ReplaceAll( "\r" , "" ) );
					string.set_ByRef( text.ReplaceAll( '\n' , '\r' ) );
					if( (*this)()->_is_regexp_pattern == 1 )
						string.set_ByRef( XRegexp::Correct( string , (*this)()->_regexp_pattern ) );
					(*this)()->_cs_drawing_elements.wlock( debuginfo_macros );
					(*this)()->_lines.Clear();
					(*this)()->_lines.AddByRef( string.ConvertToUTF8() );
					(*this)()->_cs_drawing_elements.wunlock( debuginfo_macros );
				}				
				(*this)()->_cs_drawing_elements.wlock( debuginfo_macros );
				if( text.Length() < (*this)()->_selection_char_index )
				{
					(*this)()->_selection_char_index = 0;
					(*this)()->_old_selection_char_index = 0;
					(*this)()->_scrollbar_left_offset = 0.f;
					(*this)()->_scrollbar_top_offset = 0.f;
				}
				(*this)()->_cs_drawing_elements.wunlock( debuginfo_macros );
				(*this)()->calculate_caret_and_selection();
			};
			void XTextBox::set_IsMasked( ::rux::uint8 is_masked )
			{
				if( (*this)()->_is_masked != is_masked )
				{
					(*this)()->_is_masked = is_masked;					
					(*this)()->_cs_drawing_elements.wlock( debuginfo_macros );
					(*this)()->_selection_char_index = 0;
					(*this)()->_old_selection_char_index = 0;
					(*this)()->_scrollbar_left_offset = 0.f;
					(*this)()->_scrollbar_top_offset = 0.f;
					(*this)()->_cs_drawing_elements.wunlock( debuginfo_macros );
					(*this)()->calculate_caret_and_selection();
				}
			};
			void TextBox::render( ::rux::gui::RenderContextBase* render_context , float opacity , float& _selected_z_index , size_t ___rux__thread_index1986 )
			{	
				float left = get_Left();
				float top = get_Top();
				float width = get_Width();
				float height = get_Height();
				float top_border = top + height;
				_cs_drawing_elements.wlock( debuginfo_macros );
				render_context->DrawRectangle( left , top , width , height , _border_thickness , _background , _border_color , ::rux::gui::Thickness() ,  _opacity * opacity , &_ui_border_cache , &_ui_cache , _selected_z_index , 1 , __FILE__ , __LINE__ , ___rux__thread_index1986 );
				_selected_z_index += 1.f;//0.001f
				left += _border_thickness;
				top += _border_thickness;
				width -= 2 * _border_thickness;
				height -= 2 * _border_thickness;
				rux::gui::Rectangle old_clip( render_context->get_Clip() );
				render_context->set_Clip( ::rux::gui::Rectangle( left , top , width , height ) );
				rux::XPointF point;
				for( ::rux::uint32 index0 = 0 ; index0 < _selection_rects.Count() ; index0++ )
				{
					rux::gui::RenderCacheBase* rcb = NULL;
					if( index0 < _selection_rects_caches.Count() )
						rcb = _selection_rects_caches[ index0 ];
					point = _selection_rects[ index0 ].get_Point();
					render_context->DrawRectangle( ::rux::math::round( point.get_X() , 0 ) , ::rux::math::round( point.get_Y() , 0 ) , ::rux::math::round( _selection_rects[ index0 ].get_Width() , 0 ) , ::rux::math::round( _selection_rects[ index0 ].get_Height() , 0 ) , 0.f , _selection_background , ::rux::gui::Colors::Transparent() , ::rux::gui::Thickness() , _opacity * opacity , &_pseudo_cache , &rcb , _selected_z_index , 1 , __FILE__ , __LINE__ , ___rux__thread_index1986 );
					if( index0 >= _selection_rects_caches.Count() )
						_selection_rects_caches.Add( rcb );
				}
				top -= _scrollbar_top_offset;
				float text_height = _font->get_Height();
				if( _is_multi_lines == 0 )
				{
					if( text_height < height )
						top += ( height - text_height ) / 2.f;
				}
				float offset = 0.f , text_width = 0.f , mask_offset = 0.f , mask_size = text_height;
				if( mask_size >= 5.f )
					mask_size -= 3.f;	
				::rux::uint32 index1 = 0 , caches_index = 0;
				for( ::rux::uint32 index0 = 0 ; index0 < _lines.Count() ; index0++ )
				{
					rux::gui::RenderCacheBase* txt_cache = NULL;
					if( caches_index < _text_caches.Count() )
						txt_cache = _text_caches[ caches_index ];
					if( _horizontal_text_alignment == XEnum_Alignment_Center )
					{
						if( _is_masked == 1 )
							text_width = text_height * _lines[ index0 ].Length();
						else
							text_width = _font->get_TextWidth( _lines[ index0 ].str() , ___rux__thread_index1986 );
						if( text_width < width )
						{
							offset = ( width - text_width ) / 2;
							left += offset;
							width -= offset;
						}
					}
					else if( _horizontal_text_alignment == XEnum_Alignment_RightOrBottom )
					{
						if( _is_masked == 1 )
							text_width = text_height * _lines[ index0 ].Length();
						else
							text_width = _font->get_TextWidth( _lines[ index0 ].str() , ___rux__thread_index1986 );
						if( text_width < width )
						{
							offset = width - text_width;
							left += offset;
							width -= offset;
						}		
					}
					if( _is_masked == 1 )
					{
						mask_offset = left - _scrollbar_left_offset + ( text_height - mask_size ) / 2.f;
						for( index1 = 0 ; index1 < _lines[ 0 ].Length() ; index1++ )
						{
							txt_cache = NULL;
							if( caches_index < _mask_caches.Count() )
								txt_cache = _mask_caches[ caches_index ];
							render_context->DrawRectangle( ::rux::math::round( mask_offset , 0 ) , top , ::rux::math::round( mask_size , 0 ) , ::rux::math::round( mask_size , 0 ) , 0.f , _foreground , ::rux::gui::Colors::Transparent() , _mask_corner , _opacity * opacity , &_pseudo_cache , &txt_cache , _selected_z_index , 1 , __FILE__ , __LINE__ , ___rux__thread_index1986 );
							if( caches_index >= _mask_caches.Count() )
								_mask_caches.Add( txt_cache );
							mask_offset += text_height;
							caches_index++;
						}
					}
					else
					{
						render_context->DrawText( left - _scrollbar_left_offset , top , width + _scrollbar_left_offset , text_height , _lines[ index0 ].str() , _font , _foreground , &txt_cache , _opacity * opacity , _selected_z_index , __FILE__ , __LINE__ , ___rux__thread_index1986 );
						if( caches_index >= _text_caches.Count() )
							_text_caches.Add( txt_cache );
					}
					top += text_height;
					if( top_border < top )
						break;
					caches_index++;
				}
				render_context->set_Clip( old_clip );
				_cs_drawing_elements.wunlock( debuginfo_macros );
			};
			void XTextBox::set_BorderColor( ::rux::gui::Color* border_color )
			{
				(*this)()->_cs_drawing_elements.wlock( debuginfo_macros );
				rux::gui::copy_color( (*this)()->_border_color , border_color );
				(*this)()->_cs_drawing_elements.wunlock( debuginfo_macros );
			};
			float XTextBox::get_BorderThickness( void )
			{
				return (*this)()->_border_thickness;
			};
			void XTextBox::set_BorderThickness( float border_thickness )
			{
				if( (*this)()->_border_thickness != border_thickness )
				{
					(*this)()->_border_thickness = ::rux::math::round( border_thickness , 0 );
				}
			};
			void TextBox::calculate_caret_and_selection( void )
			{
				if( get_ParentControl() != NULL )
				{
					_cs_drawing_elements.wlock( debuginfo_macros );
					float caret_left = 0.f , caret_top = 0.f;
					float text_height = _font->get_Height();
					size_t selection_char_index = 0 , line_index = 0;
					size_t char_count = 0;
					
					float left = get_Left() + _border_thickness;
					float top = get_Top() + _border_thickness;
					float width = get_Width() - 2 * _border_thickness;
					float height = get_Height() - 2 * _border_thickness;
					if( width > 0 && height > 0 )
					{
						::rux::uint8 recalculate = 0;
						do
						{
							recalculate = 0;
							selection_char_index = 0 , line_index = 0;
							char_count = 0;
							caret_left = get_Left() - _scrollbar_left_offset + _border_thickness , caret_top = get_Top() - _scrollbar_top_offset + _border_thickness;
							for( ; line_index < _lines.Count() ; line_index++ )
							{
								selection_char_index += _lines[ line_index ].Length() + 1;			
								if( selection_char_index > _selection_char_index )
								{
									selection_char_index -= _lines[ line_index ].Length() + 1;			
									break;
								}	
								else if( line_index == _lines.Count() - 1 )
								{
									if( _old_selection_char_index == _selection_char_index )
										_old_selection_char_index = selection_char_index - 1;
									_selection_char_index = selection_char_index - 1;
									recalculate = 1;
								}
								caret_top += text_height;
							}				
							if( recalculate == 1 )
								continue;
							char_count = _selection_char_index - selection_char_index;		
							if( _horizontal_text_alignment == XEnum_Alignment_Center )
							{
								if( _is_masked == 1 )
								{
									float text_width = text_height * _lines[ line_index ].Length();
									if( text_width < width )
										caret_left += ( width - text_width ) / 2;
								}
								else
								{
									float text_width = _font->get_TextWidth( _lines[ line_index ].str() , SIZE_MAX );
									if( text_width < width )
										caret_left += ( width - text_width ) / 2;
								}
							}
							else if( _horizontal_text_alignment == XEnum_Alignment_RightOrBottom )
							{
								if( _is_masked == 1 )
								{
									float text_width = text_height * _lines[ line_index ].Length();
									if( text_width < width )
										caret_left += width - text_width;
								}
								else
								{
									float text_width = _font->get_TextWidth( _lines[ line_index ].str() , SIZE_MAX );
									if( text_width < width )
										caret_left += width - text_width;
								}
							}
							if( _is_masked == 1 )
							{
								for( ::rux::uint32 index0 = 0 ; index0 < char_count ; index0++ )
									caret_left += text_height;
							}
							else
							{
								for( ::rux::uint32 index0 = 0 ; index0 < char_count ; index0++ )
									caret_left += _font->get_CharacterWidth( _lines[ line_index ].get_UTF8Char( index0 ) , SIZE_MAX );
							}
							if( caret_left > left + width )
							{
								if( char_count < _lines[ line_index ].Length() )
								{
									if( _is_masked == 1 )
										_scrollbar_left_offset += caret_left - left - width + text_height + 5.f;
									else
										_scrollbar_left_offset += caret_left - left - width + _font->get_CharacterWidth( _lines[ line_index ].get_UTF8Char( char_count ) , SIZE_MAX ) + 5.f;
								}
								else
									_scrollbar_left_offset += caret_left - left - width + 5.f;
								recalculate = 1;
							}
							else if( caret_left < left && _scrollbar_left_offset >= left - caret_left )
							{			
								_scrollbar_left_offset -= left - caret_left;
								if( _scrollbar_left_offset >= 40.f )
									_scrollbar_left_offset -= 40.f;
								else
									_scrollbar_left_offset = 0.f;
								recalculate = 1;							
							}
							if( text_height <= height && caret_top + text_height > top + height )
							{
								_scrollbar_top_offset += caret_top + text_height - top - height + 5.f;
								recalculate = 1;							
							}
							else if( text_height <= height && caret_top < top && _scrollbar_top_offset >= top - caret_top )
							{			
								_scrollbar_top_offset -= top - caret_top;
								if( _scrollbar_top_offset >= 5.f )
									_scrollbar_top_offset -= 5.f;
								recalculate = 1;
							}
						}
						while( recalculate == 1 );
					}
					else
					{
						caret_left = get_Left() - _scrollbar_left_offset + _border_thickness , caret_top = get_Top() - _scrollbar_top_offset + _border_thickness;
						_scrollbar_left_offset = 0;
						_scrollbar_top_offset = 0;
						_selection_char_index = 0;
					}
					_selection_rects.Clear();					
					if( _old_selection_char_index < _selection_char_index )
					{		
						::rux::uint32 old_line_index = line_index;
						float rect_left = get_Left() - _scrollbar_left_offset + _border_thickness , rect_top = caret_top , rect_width = 0.f;
						for( ; old_line_index < _lines.Count() ; old_line_index-- )
						{			
							if( _horizontal_text_alignment == XEnum_Alignment_Center )
							{
								if( _is_masked == 1 )
								{
									float text_width = text_height * _lines[ old_line_index ].Length();
									if( text_width < width )
										rect_left = get_Left() - _scrollbar_left_offset + _border_thickness + ( width - text_width ) / 2;
								}
								else
								{
									float text_width = _font->get_TextWidth( _lines[ old_line_index ].str() , SIZE_MAX );
									if( text_width < width )
										rect_left = get_Left() - _scrollbar_left_offset + _border_thickness + ( width - text_width ) / 2;
								}
							}
							else if( _horizontal_text_alignment == XEnum_Alignment_RightOrBottom )
							{
								if( _is_masked == 1 )
								{
									float text_width = text_height * _lines[ old_line_index ].Length();
									if( text_width < width )
										rect_left = get_Left() - _scrollbar_left_offset + _border_thickness + width - text_width;
								}
								else
								{
									float text_width = _font->get_TextWidth( _lines[ old_line_index ].str() , SIZE_MAX );
									if( text_width < width )
										rect_left = get_Left() - _scrollbar_left_offset + _border_thickness + width - text_width;
								}
							}
							if( old_line_index != line_index )
							{
								char_count = _lines[ old_line_index ].Length() + 1;
								selection_char_index -= char_count;			
								char_count--;
							}
							if( selection_char_index <= _old_selection_char_index )
							{				
								selection_char_index = _old_selection_char_index - selection_char_index;
								if( _is_masked == 1 )
								{
									for( ::rux::uint32 index0 = 0 ; index0 < char_count ; index0++ )
									{
										if( index0 < selection_char_index )
											rect_left += text_height;
										else
											rect_width += text_height;
									}
								}
								else
								{
									for( ::rux::uint32 index0 = 0 ; index0 < char_count ; index0++ )
									{
										if( index0 < selection_char_index )
											rect_left += _font->get_CharacterWidth( _lines[ old_line_index ].get_UTF8Char( index0 ) , SIZE_MAX );			
										else
											rect_width += _font->get_CharacterWidth( _lines[ old_line_index ].get_UTF8Char( index0 ) , SIZE_MAX );			
									}
								}
								_selection_rects.Add( ::rux::XRectangleF( ::rux::XPointF( rect_left , rect_top ) , rect_width , text_height ) );
								break;
							}
							if( _is_masked == 1 )
							{
								for( ::rux::uint32 index0 = 0 ; index0 < char_count ; index0++ )
									rect_width += text_height;
							}
							else
							{
								for( ::rux::uint32 index0 = 0 ; index0 < char_count ; index0++ )
									rect_width += _font->get_CharacterWidth( _lines[ old_line_index ].get_UTF8Char( index0 ) , SIZE_MAX );			
							}
							_selection_rects.Add( ::rux::XRectangleF( ::rux::XPointF( rect_left , rect_top ) , rect_width , text_height ) );
							rect_top -= text_height;						
							rect_width = 0.f;
							if( old_line_index == 0 )
								break;
						}
					}
					else if( _old_selection_char_index > _selection_char_index )
					{
						::rux::uint32 old_line_index = line_index;
						float rect_left = caret_left , rect_top = caret_top , rect_width = 0.f;
						for( ; old_line_index < _lines.Count() ; old_line_index++ )
						{
							if( _horizontal_text_alignment == XEnum_Alignment_Center )
							{
								if( _is_masked == 1 )
								{
									float text_width = text_height * _lines[ old_line_index ].Length();
									if( text_width < width )
										rect_left = caret_left + ( width - text_width ) / 2;
								}
								else
								{
									float text_width = _font->get_TextWidth( _lines[ old_line_index ].str() , SIZE_MAX );
									if( text_width < width )
										rect_left = caret_left + ( width - text_width ) / 2;
								}
							}
							else if( _horizontal_text_alignment == XEnum_Alignment_RightOrBottom )
							{
								if( _is_masked == 1 )
								{
									float text_width = text_height * _lines[ old_line_index ].Length();
									if( text_width < width )
										rect_left = caret_left + width - text_width;
								}
								else
								{
									float text_width = _font->get_TextWidth( _lines[ old_line_index ].str() , SIZE_MAX );
									if( text_width < width )
										rect_left = caret_left + width - text_width;
								}
							}
							selection_char_index += _lines[ old_line_index ].Length() + 1;
							if( selection_char_index > _old_selection_char_index )
							{
								selection_char_index -= _lines[ old_line_index ].Length() + 1;
								selection_char_index = _old_selection_char_index - selection_char_index;
								if( _is_masked == 1 )
								{
									for( ; char_count < selection_char_index ; char_count++ )
										rect_width += text_height;
								}
								else
								{
									for( ; char_count < selection_char_index ; char_count++ )
										rect_width += _font->get_CharacterWidth( _lines[ old_line_index ].get_UTF8Char( char_count ) , SIZE_MAX );			
								}
								_selection_rects.Add( ::rux::XRectangleF( ::rux::XPointF( rect_left , rect_top ) , rect_width , text_height ) );
								break;
							}
							if( _is_masked == 1 )
							{
								for( ; char_count < _lines[ old_line_index ].Length() ; char_count++ )
									rect_width += text_height;
							}
							else
							{
								for( ; char_count < _lines[ old_line_index ].Length() ; char_count++ )
									rect_width += _font->get_CharacterWidth( _lines[ old_line_index ].get_UTF8Char( char_count ) , SIZE_MAX );			
							}
							_selection_rects.Add( ::rux::XRectangleF( ::rux::XPointF( rect_left , rect_top ) , rect_width , text_height ) );
							rect_top += text_height;
							char_count = 0;
							rect_left = get_Left() - _scrollbar_left_offset + _border_thickness;
							rect_width = 0.f;
						}
					}
					_cs_drawing_elements.wunlock( debuginfo_macros );
					rux::gui::Window* window = get_ParentWindow();
					if( window )
						window->StartCaret( caret_left , caret_top , text_height , ::rux::gui::Colors::Black( 1 , __FILE__ , __LINE__ ) );
				}
			};
			void TextBox::raise_OnRightMouseButtonDown( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_right_mouse_button_down.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void TextBox::raise_OnRightMouseButtonDoubleClick( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_right_mouse_button_double_click.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void TextBox::raise_OnRightMouseButtonUp( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_right_mouse_button_up.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void TextBox::raise_OnMouseWheelDown(::rux::gui::WindowMouseEvent* window_event, ::rux::byte explicit_event)
			{
				_on_mouse_wheel_down.raise< const ::rux::gui::events::MouseEvent& >(
					::rux::gui::events::MouseEvent(*this, window_event, explicit_event));
			};
			void TextBox::raise_OnMouseWheelDoubleClick(::rux::gui::WindowMouseEvent* window_event
				, ::rux::byte explicit_event)
			{
				_on_mouse_wheel_double_click.raise< const ::rux::gui::events::MouseEvent& >(
					::rux::gui::events::MouseEvent(*this, window_event, explicit_event));
			};
			void TextBox::raise_OnMouseWheelUp(::rux::gui::WindowMouseEvent* window_event, ::rux::byte explicit_event)
			{
				_on_mouse_wheel_up.raise< const ::rux::gui::events::MouseEvent& >(
					::rux::gui::events::MouseEvent(*this, window_event, explicit_event));
			};
			void TextBox::raise_OnLeftMouseButtonDown( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{	
				_on_left_mouse_button_down.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , 1 ) );
				float text_height = _font->get_Height();
				::rux::uint32 line_index = (uint32)( ( window_event->_y - get_Top() - _border_thickness + _scrollbar_top_offset ) / text_height );
				::rux::uint32 char_index = 0;
				if( line_index < _lines.Count() )
				{
					float caret_left = get_Left() - _scrollbar_left_offset + _border_thickness;
					if( _horizontal_text_alignment == XEnum_Alignment_Center )
					{
						float width = get_Width() - 2 * _border_thickness;
						if( _is_masked == 1 )
						{
							float text_width = text_height * _lines[ line_index ].Length();
							if( text_width < width )
								caret_left += ( width - text_width ) / 2;
						}
						else
						{
							float text_width = _font->get_TextWidth( _lines[ line_index ].str() , SIZE_MAX );
							if( text_width < width )
								caret_left += ( width - text_width ) / 2;
						}
					}
					else if( _horizontal_text_alignment == XEnum_Alignment_RightOrBottom )
					{
						float width = get_Width() - 2 * _border_thickness;
						if( _is_masked == 1 )
						{
							float text_width = text_height * _lines[ line_index ].Length();
							if( text_width < width )
								caret_left +=  width - text_width;
						}
						else
						{
							float text_width = _font->get_TextWidth( _lines[ line_index ].str() , SIZE_MAX );
							if( text_width < width )
								caret_left +=  width - text_width;
						}
					}
					float character_width = 0.f;
					for( ; char_index < _lines[ line_index ].Length() ; char_index++ )
					{
						if( _is_masked == 1 )
							character_width = text_height / 2.f;
						else
							character_width = ( _font->get_CharacterWidth( _lines[ line_index ].get_UTF8Char( char_index ) , SIZE_MAX ) ) / 2.f;
						caret_left += character_width;
						if( caret_left > window_event->_x )
						{
							caret_left -= character_width;
							break;
						}
						else
						{
							caret_left += character_width;
							if( caret_left > window_event->_x )
							{
								char_index++;
								break;
							}
						}
					}
				}
				else if( _lines.Count() > 0 )
				{
					line_index = (uint32)_lines.Count() - 1;		
					char_index = _lines[ line_index ].Length();
				}
				else
					line_index = 0;	
				_cs_drawing_elements.wlock( debuginfo_macros );
				if( window_event->_shift == 1 && _old_selection_char_index == _selection_char_index )
					_old_selection_char_index = _selection_char_index;
				_selection_char_index = 0;
				for( size_t index0 = 0 ; index0 < line_index ; index0++ )
					_selection_char_index += _lines[ index0 ].Length() + 1;
				_selection_char_index += char_index;		
				if( window_event->_shift == 0 )
				{
					_old_selection_char_index = _selection_char_index;
					if( _lines.Count() > 0 && _is_left_mouse_down_selection_regexp_pattern == 1 )
					{
						size_t char_index_in_line = 0;
						get_line( _selection_char_index , char_index_in_line );
						::rux::uint32 index = char_index_in_line , count = 0;
						if( XRegexp::LastIndexOf( _lines[ line_index ] , index , count , _left_mouse_down_selection_regexp_pattern ) )
						{				
							if( index <= char_index_in_line && index + count >= char_index_in_line )
							{
								char_index_in_line = get_first_char_index( line_index );
								_old_selection_char_index = char_index_in_line + index;
								_selection_char_index = _old_selection_char_index + count;
							}
						}
					}
				}
				_cs_drawing_elements.wunlock( debuginfo_macros );
				calculate_caret_and_selection();
			};
			void TextBox::raise_OnDeactivate( ::rux::gui::CommonBase* activated_control )
			{		
				XObject activated;
				if( activated_control )
					activated.set_ByRef( XObject( activated_control , 1 ) );
				_on_deactivated.raise< const XObject& , const XObject& >( XObject( *this ) , activated );
			};
			void TextBox::raise_OnActivate( void )
			{	
				calculate_caret_and_selection();
				_on_activated.raise< const ::rux::gui::events::Event& >( ::rux::gui::events::Event( *this , 1 ) );
			};
			void TextBox::raise_OnKeyChar( const ::rux::XString& symbol , ::rux::uint8 , ::rux::uint8 , ::rux::uint8 )
			{	
				_cs_drawing_elements.wlock( debuginfo_macros );
				size_t char_index_in_line = 0;
				::rux::uint32 line_index = get_line( _selection_char_index , char_index_in_line );
				rux::XString regexp_text;
				::rux::uint32 regexp_index = 0;
				if( _selection_char_index == _old_selection_char_index )
				{		
					if( _is_multi_lines == 0 && _is_regexp_pattern == 1 )
						_lines[ line_index ].RemoveAt( char_index_in_line );
					_lines[ line_index ].InsertRange( char_index_in_line , symbol );
					_selection_char_index++;	
				}
				else if( _old_selection_char_index > _selection_char_index )
				{
					size_t old_char_index_in_line = 0;
					::rux::uint32 old_line_index = get_line( _old_selection_char_index , old_char_index_in_line );
					if( old_line_index == line_index )
					{
						if( _is_multi_lines == 0 && _is_regexp_pattern == 1 )
						{
							regexp_index = old_char_index_in_line;
							regexp_text.set_ByRef( _lines[ line_index ].Substring( old_char_index_in_line ) );
						}
						_lines[ line_index ].RemoveRange( char_index_in_line , old_char_index_in_line - char_index_in_line );
					}
					else
					{
						_lines[ line_index ].RemoveRange( char_index_in_line , _lines[ line_index ].Length() - char_index_in_line );
						_lines[ old_line_index ].RemoveRange( 0 , old_char_index_in_line );
						for( ::rux::uint32 index0 = line_index + 1 ; index0 < old_line_index ; index0++ )
							_lines.RemoveAt( line_index + 1 );					
						_lines.set_ItemByRef( line_index , ( _lines[ line_index ] + _lines[ line_index + 1 ] ).ConvertToUTF8() );
						_lines.RemoveAt( line_index + 1 );				
					}
					_lines[ line_index ].InsertRange( char_index_in_line , symbol );
					_selection_char_index++;	
				}
				else if( _old_selection_char_index < _selection_char_index )
				{
					size_t old_char_index_in_line = 0;
					::rux::uint32 old_line_index = get_line( _old_selection_char_index , old_char_index_in_line );
					if( old_line_index == line_index )
					{
						if( _is_multi_lines == 0 && _is_regexp_pattern == 1 )
						{
							regexp_index = char_index_in_line;
							regexp_text.set_ByRef( _lines[ line_index ].Substring( char_index_in_line ) );
						}
						_lines[ line_index ].RemoveRange( old_char_index_in_line , char_index_in_line - old_char_index_in_line );
					}
					else
					{
						_lines[ old_line_index ].RemoveRange( old_char_index_in_line , _lines[ old_line_index ].Length() - old_char_index_in_line );
						_lines[ line_index ].RemoveRange( 0 , char_index_in_line );
						for( ::rux::uint32 index0 = old_line_index + 1 ; index0 < line_index ; index0++ )
							_lines.RemoveAt( old_line_index + 1 );					
						_lines.set_ItemByRef( old_line_index , ( _lines[ old_line_index ] + _lines[ old_line_index + 1 ] ).ConvertToUTF8() );
						_lines.RemoveAt( old_line_index + 1 );				
					}
					_lines[ old_line_index ].InsertRange( old_char_index_in_line , symbol );		
					_selection_char_index = _old_selection_char_index + 1;
				}				
				_old_selection_char_index = _selection_char_index;
				if( _is_multi_lines == 0 && _is_regexp_pattern == 1 )
				{
					_lines.set_ItemByRef( 0 , XRegexp::Correct( _lines[ 0 ] , _regexp_pattern ).ConvertToUTF8() );
					if( regexp_text.Length() > 0 )
					{
						_lines[ 0 ].RemoveRange( regexp_index , _lines[ 0 ].Length() - regexp_index );
						_lines.set_ItemByRef( 0 , _lines[ 0 ] + regexp_text );
						_lines.set_ItemByRef( 0 , XRegexp::Correct( _lines[ 0 ] , _regexp_pattern ).ConvertToUTF8() );
					}
				}
				_cs_drawing_elements.wunlock( debuginfo_macros );
				calculate_caret_and_selection();	
				_on_text_changed_event_callback.raise< const ::rux::gui::events::Event& >( ::rux::gui::events::Event( *this , 1 ) );
			};
			size_t TextBox::get_first_char_index( size_t line_index )
			{
				size_t selection_char_index = 0;
				for( ::rux::uint32 index0 = 0 ; index0 < line_index ; index0++ )
					selection_char_index += _lines[ index0 ].Length() + 1;
				return selection_char_index;
			};
			size_t TextBox::get_line( size_t char_index , size_t& char_index_in_line )
			{
				size_t selection_char_index = 0 , line_index = 0;
				for( ; line_index < _lines.Count() ; line_index++ )
				{
					selection_char_index += _lines[ line_index ].Length() + 1;
					if( selection_char_index > char_index )
					{
						selection_char_index -= _lines[ line_index ].Length() + 1;			
						break;
					}
				}
				char_index_in_line = char_index - selection_char_index;
				return line_index;
			};
			void TextBox::raise_OnKeyDown( ::rux::gui::XEnum_Keys key , ::rux::uint8 shift , ::rux::uint8 , ::rux::uint8 , ::rux::uint32 , ::rux::byte explicit_event )
			{	
				_cs_drawing_elements.wlock( debuginfo_macros );
				switch( key )
				{
				case ::rux::gui::XEnum_Keys_Control_A:
					{
						_old_selection_char_index = 0;
						_selection_char_index = 0;
						for( ::rux::uint32 index0 = 0 ; index0 < _lines.Count() ; index0++ )
						{
							if( index0 > 0 )
								_selection_char_index += 1;
							_selection_char_index += _lines[ index0 ].Length();
						}
						_cs_drawing_elements.wunlock( debuginfo_macros );
						calculate_caret_and_selection();
						_cs_drawing_elements.wlock( debuginfo_macros );
						break;
					}
				case ::rux::gui::XEnum_Keys_Control_V:
					{
						rux::XString regexp_text;
						::rux::uint32 regexp_index = 0;
						rux::XString error , clipboard_data;
						clipboard_data.set_ByRef( XClipboard::GetStringData( error ) );
						if( _is_multi_lines == 0 )
						{
							clipboard_data.set_ByRef( clipboard_data.ReplaceAll( "\r" , "" ) );
							clipboard_data.set_ByRef( clipboard_data.ReplaceAll( '\n' , '\r' ) );
						}
						if( clipboard_data.Length() > 0 )
						{
							XArray< ::rux::XString > lines;
							clipboard_data.set_ByRef( clipboard_data.ReplaceAll( "\r\n" , "\n" ) );				
							lines.set_ByRef( clipboard_data.Split( "\n" , EStringSplitOptions_None ) );
							size_t char_index_in_line = 0;
							::rux::uint32 line_index = get_line( _selection_char_index , char_index_in_line );
							if( _selection_char_index == _old_selection_char_index )
							{		
								if( lines.Count() == 0 )
									_lines[ line_index ].InsertRange( char_index_in_line , lines[ 0 ] );
								else
								{
									rux::XString string;
									if( char_index_in_line < _lines[ line_index ].Length() )
									{
										string.set_ByRef( _lines[ line_index ].Substring( char_index_in_line ) );
										_lines[ line_index ].RemoveRange( char_index_in_line , _lines[ line_index ].Length() - char_index_in_line );
									}
									_lines[ line_index ].InsertRange( char_index_in_line , lines[ 0 ] );
									for( ::rux::uint32 index0 = 1 ; index0 < lines.Count() ; index0++ )
										_lines.Insert( line_index + index0 , lines[ index0 ] );
									_lines.set_ItemByRef( line_index + lines.Count() - 1 , ( _lines[ line_index + lines.Count() - 1 ] + string ).ConvertToUTF8() );
								}
								_selection_char_index += clipboard_data.Length();	
							}
							else if( _old_selection_char_index > _selection_char_index )
							{
								size_t old_char_index_in_line = 0;
								::rux::uint32 old_line_index = get_line( _old_selection_char_index , old_char_index_in_line );
								if( old_line_index == line_index )
								{
									if( _is_multi_lines == 0 && _is_regexp_pattern == 1 )
									{
										regexp_index = old_char_index_in_line;
										regexp_text.set_ByRef( _lines[ line_index ].Substring( old_char_index_in_line ) );
									}
									_lines[ line_index ].RemoveRange( char_index_in_line , old_char_index_in_line - char_index_in_line );
								}
								else
								{
									_lines[ line_index ].RemoveRange( char_index_in_line , _lines[ line_index ].Length() - char_index_in_line );
									_lines[ old_line_index ].RemoveRange( 0 , old_char_index_in_line );
									for( ::rux::uint32 index0 = line_index + 1 ; index0 < old_line_index ; index0++ )
										_lines.RemoveAt( line_index + 1 );					
									_lines.set_ItemByRef( line_index , ( _lines[ line_index ] + _lines[ line_index + 1 ] ).ConvertToUTF8() );
									_lines.RemoveAt( line_index + 1 );				
								}
								if( lines.Count() == 0 )
									_lines[ line_index ].InsertRange( char_index_in_line , lines[ 0 ] );
								else
								{
									rux::XString string;
									if( char_index_in_line < _lines[ line_index ].Length() )
									{
										string.set_ByRef( _lines[ line_index ].Substring( char_index_in_line ) );
										_lines[ line_index ].RemoveRange( char_index_in_line , _lines[ line_index ].Length() - char_index_in_line );
									}
									_lines[ line_index ].InsertRange( char_index_in_line , lines[ 0 ] );
									for( ::rux::uint32 index0 = 1 ; index0 < lines.Count() ; index0++ )
										_lines.Insert( line_index + index0 , lines[ index0 ] );
									_lines.set_ItemByRef( line_index + lines.Count() - 1 , ( _lines[ line_index + lines.Count() - 1 ] + string ).ConvertToUTF8() );
								}
								_selection_char_index += clipboard_data.Length();	
							}
							else if( _old_selection_char_index < _selection_char_index )
							{
								size_t old_char_index_in_line = 0;
								::rux::uint32 old_line_index = get_line( _old_selection_char_index , old_char_index_in_line );
								if( old_line_index == line_index )
								{
									if( _is_multi_lines == 0 && _is_regexp_pattern == 1 )
									{
										regexp_index = char_index_in_line;
										regexp_text.set_ByRef( _lines[ line_index ].Substring( char_index_in_line ) );
									}
									_lines[ line_index ].RemoveRange( old_char_index_in_line , char_index_in_line - old_char_index_in_line );
								}
								else
								{
									_lines[ old_line_index ].RemoveRange( old_char_index_in_line , _lines[ old_line_index ].Length() - old_char_index_in_line );
									_lines[ line_index ].RemoveRange( 0 , char_index_in_line );
									for( ::rux::uint32 index0 = old_line_index + 1 ; index0 < line_index ; index0++ )
										_lines.RemoveAt( old_line_index + 1 );					
									_lines.set_ItemByRef( old_line_index , ( _lines[ old_line_index ] + _lines[ old_line_index + 1 ] ).ConvertToUTF8() );
									_lines.RemoveAt( old_line_index + 1 );				
								}
								if( lines.Count() == 0 )
									_lines[ old_line_index ].InsertRange( old_char_index_in_line , lines[ 0 ] );
								else
								{
									rux::XString string;
									if( old_char_index_in_line < _lines[ old_line_index ].Length() )
									{
										string.set_ByRef( _lines[ old_line_index ].Substring( old_char_index_in_line ) );
										_lines[ old_line_index ].RemoveRange( old_char_index_in_line , _lines[ old_line_index ].Length() - old_char_index_in_line );
									}
									_lines[ old_line_index ].InsertRange( old_char_index_in_line , lines[ 0 ] );
									for( ::rux::uint32 index0 = 1 ; index0 < lines.Count() ; index0++ )
										_lines.Insert( old_line_index + index0 , lines[ index0 ] );
									_lines.set_ItemByRef( old_line_index + lines.Count() - 1 , ( _lines[ old_line_index + lines.Count() - 1 ] + string ).ConvertToUTF8() );
								}
								_selection_char_index = _old_selection_char_index + clipboard_data.Length();
							}							
							_old_selection_char_index = _selection_char_index;
							if( _is_multi_lines == 0 && _is_regexp_pattern == 1 )
							{
								_lines.set_ItemByRef( 0 , XRegexp::Correct( _lines[ 0 ] , _regexp_pattern ).ConvertToUTF8() );
								if( regexp_text.Length() > 0 )
								{
									_lines[ 0 ].RemoveRange( regexp_index , _lines[ 0 ].Length() - regexp_index );
									_lines.set_ItemByRef( 0 , _lines[ 0 ] + regexp_text );
									_lines.set_ItemByRef( 0 , XRegexp::Correct( _lines[ 0 ] , _regexp_pattern ).ConvertToUTF8() );
								}
							}
							_cs_drawing_elements.wunlock( debuginfo_macros );
							calculate_caret_and_selection();
							_on_text_changed_event_callback.raise< const ::rux::gui::events::Event& >( ::rux::gui::events::Event( *this , 1 ) );
							_cs_drawing_elements.wlock( debuginfo_macros );
						}			
						break;
					}
				case ::rux::gui::XEnum_Keys_Control_X:
					{
						rux::XString regexp_text;
						::rux::uint32 regexp_index = 0;
						size_t char_index_in_line = 0;
						::rux::uint32 line_index = get_line( _selection_char_index , char_index_in_line );
						if( _selection_char_index > _old_selection_char_index )
						{
							rux::XString clipboard_data;
							size_t old_char_index_in_line = 0;
							::rux::uint32 old_line_index = get_line( _old_selection_char_index , old_char_index_in_line );
							if( old_line_index == line_index )
							{
								clipboard_data += _lines[ line_index ].Substring( old_char_index_in_line , char_index_in_line - old_char_index_in_line );
								if( _is_multi_lines == 0 && _is_regexp_pattern == 1 )
								{
									regexp_index = char_index_in_line;
									regexp_text.set_ByRef( _lines[ line_index ].Substring( char_index_in_line ) );
								}
								_lines[ line_index ].RemoveRange( old_char_index_in_line , char_index_in_line - old_char_index_in_line );
							}
							else
							{
								clipboard_data += _lines[ old_line_index ].Substring( old_char_index_in_line );					
								_lines[ old_line_index ].RemoveRange( old_char_index_in_line , _lines[ old_line_index ].Length() - old_char_index_in_line );
								for( ::rux::uint32 index0 = old_line_index + 1 ; index0 < line_index ; index0++ )
								{
									clipboard_data += "\n" + _lines[ old_line_index + 1 ];
									_lines.RemoveAt( old_line_index + 1 );
								}
								clipboard_data += "\n" + _lines[ old_line_index + 1 ].Substring( 0 , char_index_in_line );
								_lines[ old_line_index + 1 ].RemoveRange( 0 , char_index_in_line );
								_lines.set_ItemByRef( old_line_index , ( _lines[ old_line_index ] + _lines[ old_line_index + 1 ] ).ConvertToUTF8() );
								_lines.RemoveAt( old_line_index + 1 );	
							}				
							rux::XString error;
							if( _is_multi_lines == 0 )
								clipboard_data.set_ByRef( clipboard_data.ReplaceAll( '\r' , '\n' ) );
							XClipboard::SetStringData( clipboard_data , error );
							_selection_char_index = _old_selection_char_index;
						}
						else if( _selection_char_index < _old_selection_char_index )
						{
							rux::XString clipboard_data;
							size_t old_char_index_in_line = 0;
							::rux::uint32 old_line_index = get_line( _old_selection_char_index , old_char_index_in_line );
							if( old_line_index == line_index )
							{
								clipboard_data += _lines[ line_index ].Substring( char_index_in_line , old_char_index_in_line - char_index_in_line );
								if( _is_multi_lines == 0 && _is_regexp_pattern == 1 )
								{
									regexp_index = old_char_index_in_line;
									regexp_text.set_ByRef( _lines[ line_index ].Substring( old_char_index_in_line ) );
								}
								_lines[ line_index ].RemoveRange( char_index_in_line , old_char_index_in_line - char_index_in_line );
							}
							else
							{
								clipboard_data += _lines[ line_index ].Substring( char_index_in_line );					
								_lines[ line_index ].RemoveRange( char_index_in_line , _lines[ line_index ].Length() - char_index_in_line );					
								for( ::rux::uint32 index0 = line_index + 1 ; index0 < old_line_index ; index0++ )
								{
									clipboard_data += "\n" + _lines[ line_index + 1 ];
									_lines.RemoveAt( line_index + 1 );
								}
								clipboard_data += "\n" + _lines[ line_index + 1 ].Substring( 0 , old_char_index_in_line );
								_lines[ line_index + 1 ].RemoveRange( 0 , old_char_index_in_line );
								_lines.set_ItemByRef( line_index , ( _lines[ line_index ] + _lines[ line_index + 1 ] ).ConvertToUTF8() );
								_lines.RemoveAt( line_index + 1 );	
							}				
							rux::XString error;
							if( _is_multi_lines == 0 )
								clipboard_data.set_ByRef( clipboard_data.ReplaceAll( '\r' , '\n' ) );
							XClipboard::SetStringData( clipboard_data , error );				
						}						
						_old_selection_char_index = _selection_char_index;
						if( _is_multi_lines == 0 && _is_regexp_pattern == 1 )
						{
							_lines.set_ItemByRef( 0 , XRegexp::Correct( _lines[ 0 ] , _regexp_pattern ).ConvertToUTF8() );
							if( regexp_text.Length() > 0 )
							{
								_lines[ 0 ].RemoveRange( regexp_index , _lines[ 0 ].Length() - regexp_index );
								_lines.set_ItemByRef( 0 , _lines[ 0 ] + regexp_text );
								_lines.set_ItemByRef( 0 , XRegexp::Correct( _lines[ 0 ] , _regexp_pattern ).ConvertToUTF8() );
							}
						}
						_cs_drawing_elements.wunlock( debuginfo_macros );
						calculate_caret_and_selection();
						_on_text_changed_event_callback.raise< const ::rux::gui::events::Event& >( ::rux::gui::events::Event( *this , 1 ) );
						_cs_drawing_elements.wlock( debuginfo_macros );
						break;
					}
				case ::rux::gui::XEnum_Keys_Control_C:
					{
						size_t char_index_in_line = 0;
						::rux::uint32 line_index = get_line( _selection_char_index , char_index_in_line );
						if( _selection_char_index > _old_selection_char_index )
						{
							rux::XString clipboard_data;
							size_t old_char_index_in_line = 0;
							::rux::uint32 old_line_index = get_line( _old_selection_char_index , old_char_index_in_line );
							if( old_line_index == line_index )
								clipboard_data += _lines[ line_index ].Substring( old_char_index_in_line , char_index_in_line - old_char_index_in_line );
							else
							{
								clipboard_data += _lines[ old_line_index ].Substring( old_char_index_in_line );					
								for( ::rux::uint32 index0 = old_line_index + 1 ; index0 < line_index ; index0++ )
									clipboard_data += "\n" + _lines[ index0 ];
								clipboard_data += "\n" + _lines[ line_index ].Substring( 0 , char_index_in_line );
							}				
							rux::XString error;
							if( _is_multi_lines == 0 )
								clipboard_data.set_ByRef( clipboard_data.ReplaceAll( '\r' , '\n' ) );
							XClipboard::SetStringData( clipboard_data , error );
						}
						else if( _selection_char_index < _old_selection_char_index )
						{
							rux::XString clipboard_data;
							size_t old_char_index_in_line = 0;
							::rux::uint32 old_line_index = get_line( _old_selection_char_index , old_char_index_in_line );
							if( old_line_index == line_index )
								clipboard_data += _lines[ line_index ].Substring( char_index_in_line , old_char_index_in_line - char_index_in_line );
							else
							{
								clipboard_data += _lines[ line_index ].Substring( char_index_in_line );					
								for( ::rux::uint32 index0 = line_index + 1 ; index0 < old_line_index ; index0++ )
									clipboard_data += "\n" + _lines[ index0 ];
								clipboard_data += "\n" + _lines[ old_line_index ].Substring( 0 , old_char_index_in_line );
							}				
							rux::XString error;
							if( _is_multi_lines == 0 )
								clipboard_data.set_ByRef( clipboard_data.ReplaceAll( '\r' , '\n' ) );
							XClipboard::SetStringData( clipboard_data , error );
						}
						break;
					}
				case ::rux::gui::XEnum_Keys_Enter:
					{
						if( _is_multi_lines == 1 )
						{
							size_t char_index_in_line = 0;
							::rux::uint32 line_index = get_line( _selection_char_index , char_index_in_line );
							if( _old_selection_char_index == _selection_char_index )
							{
								if( char_index_in_line == _lines[ line_index ].Length() )
								{		
									_lines.Insert( line_index + 1 , ::rux::XString() );
									_selection_char_index++;
									_old_selection_char_index = _selection_char_index;
								}
								else
								{					
									_lines.Insert( line_index + 1 , _lines[ line_index ].Substring( char_index_in_line ) );
									_lines[ line_index ].RemoveRange( char_index_in_line , _lines[ line_index ].Length() - char_index_in_line );
									_selection_char_index++;
									_old_selection_char_index = _selection_char_index;
								}
							}			
							else if( _old_selection_char_index > _selection_char_index )
							{
								size_t old_char_index_in_line = 0;
								::rux::uint32 old_line_index = get_line( _old_selection_char_index , old_char_index_in_line );
								if( old_line_index == line_index )
								{
									_lines[ line_index ].RemoveRange( char_index_in_line , old_char_index_in_line - char_index_in_line );
									if( char_index_in_line < _lines[ line_index ].Length() )
									{
										_lines.Insert( line_index + 1 , _lines[ line_index ].Substring( char_index_in_line ) );
										_lines[ line_index ].RemoveRange( char_index_in_line , _lines[ line_index ].Length() - char_index_in_line );
									}
									else
										_lines.Insert( line_index + 1 , ::rux::XString() );															
								}
								else
								{
									_lines[ line_index ].RemoveRange( char_index_in_line , _lines[ line_index ].Length() - char_index_in_line );
									_lines[ old_line_index ].RemoveRange( 0 , old_char_index_in_line );
									for( ::rux::uint32 index0 = line_index + 1 ; index0 < old_line_index ; index0++ )
										_lines.RemoveAt( line_index + 1 );
								}
								_selection_char_index++;
								_old_selection_char_index = _selection_char_index;
							}
							else if( _old_selection_char_index < _selection_char_index )
							{
								size_t old_char_index_in_line = 0;
								::rux::uint32 old_line_index = get_line( _old_selection_char_index , old_char_index_in_line );
								if( old_line_index == line_index )
								{
									_lines[ line_index ].RemoveRange( old_char_index_in_line , char_index_in_line - old_char_index_in_line );
									if( old_char_index_in_line < _lines[ line_index ].Length() )
									{
										_lines.Insert( line_index + 1 , _lines[ line_index ].Substring( old_char_index_in_line ) );
										_lines[ line_index ].RemoveRange( old_char_index_in_line , _lines[ line_index ].Length() - old_char_index_in_line );
									}
									else
										_lines.Insert( line_index + 1 , ::rux::XString() );															
								}
								else
								{
									_lines[ old_line_index ].RemoveRange( old_char_index_in_line , _lines[ old_line_index ].Length() - old_char_index_in_line );
									_lines[ line_index ].RemoveRange( 0 , char_index_in_line );
									for( ::rux::uint32 index0 = old_line_index + 1 ; index0 < line_index ; index0++ )
										_lines.RemoveAt( old_line_index + 1 );
								}
								_old_selection_char_index++;
								_selection_char_index = _old_selection_char_index;
							}							
							_cs_drawing_elements.wunlock( debuginfo_macros );
							calculate_caret_and_selection();
							_on_text_changed_event_callback.raise< const ::rux::gui::events::Event& >( ::rux::gui::events::Event( *this , 1 ) );
							_cs_drawing_elements.wlock( debuginfo_macros );
						}
						break;
					}
				case ::rux::gui::XEnum_Keys_Delete:
					{
						rux::XString regexp_text;
						::rux::uint32 regexp_index = 0;
						size_t char_index_in_line = 0;
						::rux::uint32 line_index = get_line( _selection_char_index , char_index_in_line );
						if( _old_selection_char_index == _selection_char_index )
						{
							if( _lines.Count() > line_index + 1 
								&& char_index_in_line == _lines[ line_index ].Length() )
							{					
								_lines.set_ItemByRef( line_index , ( _lines[ line_index ] + _lines[ line_index + 1 ] ).ConvertToUTF8() );
								_lines.RemoveAt( line_index + 1 );
								_old_selection_char_index = _selection_char_index;
							}
							else
							{					
								_lines[ line_index ].RemoveAt( char_index_in_line );								
								if( _is_multi_lines == 0 && _is_regexp_pattern == 1 )
								{
									char letter0 = '0';
									::rux::uint32 temp_index0 = 0 , temp_count0 = 0;
									for( ; ; )
									{
										_lines[ line_index ].Insert( char_index_in_line - 1 , letter0 );
										if( ::rux::XRegexp::IndexOf( _lines[ line_index ] , temp_index0 , temp_count0 , _regexp_pattern ) == false
											|| temp_index0 > 0 
											|| temp_count0 != _lines[ line_index ].Length() )
											break;
										_lines[ line_index ].RemoveAt( char_index_in_line - 1 );
										letter0++;
									}
									_selection_char_index++;
								}
								_old_selection_char_index = _selection_char_index;
							}
						}			
						else if( _old_selection_char_index > _selection_char_index )
						{
							size_t old_char_index_in_line = 0;
							::rux::uint32 old_line_index = get_line( _old_selection_char_index , old_char_index_in_line );
							if( old_line_index == line_index )
							{
								if( _is_multi_lines == 0 && _is_regexp_pattern == 1 )
								{
									regexp_index = old_char_index_in_line;
									regexp_text.set_ByRef( _lines[ line_index ].Substring( old_char_index_in_line ) );
								}
								_lines[ line_index ].RemoveRange( char_index_in_line , old_char_index_in_line - char_index_in_line );
							}
							else
							{
								_lines[ line_index ].RemoveRange( char_index_in_line , _lines[ line_index ].Length() - char_index_in_line );
								_lines[ old_line_index ].RemoveRange( 0 , old_char_index_in_line );
								for( ::rux::uint32 index0 = line_index + 1 ; index0 < old_line_index ; index0++ )
									_lines.RemoveAt( line_index + 1 );					
								_lines.set_ItemByRef( line_index , ( _lines[ line_index ] + _lines[ line_index + 1 ] ).ConvertToUTF8() );
								_lines.RemoveAt( line_index + 1 );				
							}
							_old_selection_char_index = _selection_char_index;
						}
						else if( _old_selection_char_index < _selection_char_index )
						{
							size_t old_char_index_in_line = 0;
							::rux::uint32 old_line_index = get_line( _old_selection_char_index , old_char_index_in_line );
							if( old_line_index == line_index )
							{
								if( _is_multi_lines == 0 && _is_regexp_pattern == 1 )
								{
									regexp_index = char_index_in_line;
									regexp_text.set_ByRef( _lines[ line_index ].Substring( char_index_in_line ) );
								}
								_lines[ line_index ].RemoveRange( old_char_index_in_line , char_index_in_line - old_char_index_in_line );
							}
							else
							{
								_lines[ old_line_index ].RemoveRange( old_char_index_in_line , _lines[ old_line_index ].Length() - old_char_index_in_line );
								_lines[ line_index ].RemoveRange( 0 , char_index_in_line );
								for( ::rux::uint32 index0 = old_line_index + 1 ; index0 < line_index ; index0++ )
									_lines.RemoveAt( old_line_index + 1 );					
								_lines.set_ItemByRef( old_line_index , ( _lines[ old_line_index ] + _lines[ old_line_index + 1 ] ).ConvertToUTF8() );
								_lines.RemoveAt( old_line_index + 1 );				
							}
							_selection_char_index = _old_selection_char_index;
						}
						if( _is_multi_lines == 0 && _is_regexp_pattern == 1 )
						{
							_lines.set_ItemByRef( 0 , XRegexp::Correct( _lines[ 0 ] , _regexp_pattern ).ConvertToUTF8() );
							if( regexp_text.Length() > 0 )
							{
								_lines[ 0 ].RemoveRange( regexp_index , _lines[ 0 ].Length() - regexp_index );
								_lines.set_ItemByRef( 0 , _lines[ 0 ] + regexp_text );
								_lines.set_ItemByRef( 0 , XRegexp::Correct( _lines[ 0 ] , _regexp_pattern ).ConvertToUTF8() );
							}
						}
						_cs_drawing_elements.wunlock( debuginfo_macros );
						calculate_caret_and_selection();
						_on_text_changed_event_callback.raise< const ::rux::gui::events::Event& >( ::rux::gui::events::Event( *this , 1 ) );
						_cs_drawing_elements.wlock( debuginfo_macros );
						break;
					}
				case ::rux::gui::XEnum_Keys_Back:
					{
						rux::XString regexp_text;
						::rux::uint32 regexp_index = 0;
						size_t char_index_in_line = 0;
						::rux::uint32 line_index = get_line( _selection_char_index , char_index_in_line );
						if( _old_selection_char_index == _selection_char_index 
							&& _selection_char_index > 0 )
						{
							if( char_index_in_line == 0 )
							{
								line_index--;
								_selection_char_index = get_first_char_index( line_index ) + _lines[ line_index ].Length();
								_lines.set_ItemByRef( line_index , ( _lines[ line_index ] + _lines[ line_index + 1 ] ).ConvertToUTF8() );
								_lines.RemoveAt( line_index + 1 );					
								_old_selection_char_index = _selection_char_index;
							}
							else
							{					
								_selection_char_index--;					
								_lines[ line_index ].RemoveAt( char_index_in_line - 1 );
								if( _is_multi_lines == 0 && _is_regexp_pattern == 1 )
								{
									char letter0 = '0';
									::rux::uint32 temp_index0 = 0 , temp_count0 = 0;
									for( ; ; )
									{
										_lines[ line_index ].Insert( char_index_in_line - 1 , letter0 );
										if( ::rux::XRegexp::IndexOf( _lines[ line_index ] , temp_index0 , temp_count0 , _regexp_pattern ) == false
											|| temp_index0 > 0 
											|| temp_count0 != _lines[ line_index ].Length() )
											break;
										_lines[ line_index ].RemoveAt( char_index_in_line - 1 );
										letter0++;
									}									
								}
								_old_selection_char_index = _selection_char_index;
							}
						}			
						else if( _old_selection_char_index > _selection_char_index )
						{
							size_t old_char_index_in_line = 0;
							::rux::uint32 old_line_index = get_line( _old_selection_char_index , old_char_index_in_line );
							if( old_line_index == line_index )
							{
								if( _is_multi_lines == 0 && _is_regexp_pattern == 1 )
								{
									regexp_index = old_char_index_in_line;
									regexp_text.set_ByRef( _lines[ line_index ].Substring( old_char_index_in_line ) );
								}
								_lines[ line_index ].RemoveRange( char_index_in_line , old_char_index_in_line - char_index_in_line );
							}
							else
							{
								_lines[ line_index ].RemoveRange( char_index_in_line , _lines[ line_index ].Length() - char_index_in_line );
								_lines[ old_line_index ].RemoveRange( 0 , old_char_index_in_line );
								for( ::rux::uint32 index0 = line_index + 1 ; index0 < old_line_index ; index0++ )
									_lines.RemoveAt( line_index + 1 );					
								_lines.set_ItemByRef( line_index , ( _lines[ line_index ] + _lines[ line_index + 1 ] ).ConvertToUTF8() );
								_lines.RemoveAt( line_index + 1 );				
							}
							_old_selection_char_index = _selection_char_index;
						}
						else if( _old_selection_char_index < _selection_char_index )
						{
							size_t old_char_index_in_line = 0;
							::rux::uint32 old_line_index = get_line( _old_selection_char_index , old_char_index_in_line );
							if( old_line_index == line_index )
							{
								if( _is_multi_lines == 0 && _is_regexp_pattern == 1 )
								{
									regexp_index = char_index_in_line;
									regexp_text.set_ByRef( _lines[ line_index ].Substring( char_index_in_line ) );
								}
								_lines[ line_index ].RemoveRange( old_char_index_in_line , char_index_in_line - old_char_index_in_line );
							}
							else
							{
								_lines[ old_line_index ].RemoveRange( old_char_index_in_line , _lines[ old_line_index ].Length() - old_char_index_in_line );
								_lines[ line_index ].RemoveRange( 0 , char_index_in_line );
								for( ::rux::uint32 index0 = old_line_index + 1 ; index0 < line_index ; index0++ )
									_lines.RemoveAt( old_line_index + 1 );					
								_lines.set_ItemByRef( old_line_index , ( _lines[ old_line_index ] + _lines[ old_line_index + 1 ] ).ConvertToUTF8() );
								_lines.RemoveAt( old_line_index + 1 );				
							}
							_selection_char_index = _old_selection_char_index;
						}
						if( _is_multi_lines == 0 && _is_regexp_pattern == 1 )
						{
							_lines.set_ItemByRef( 0 , XRegexp::Correct( _lines[ 0 ] , _regexp_pattern ).ConvertToUTF8() );
							if( regexp_text.Length() > 0 )
							{
								_lines[ 0 ].RemoveRange( regexp_index , _lines[ 0 ].Length() - regexp_index );
								_lines.set_ItemByRef( 0 , _lines[ 0 ] + regexp_text );
								_lines.set_ItemByRef( 0 , XRegexp::Correct( _lines[ 0 ] , _regexp_pattern ).ConvertToUTF8() );
							}
						}
						_cs_drawing_elements.wunlock( debuginfo_macros );
						calculate_caret_and_selection();
						_on_text_changed_event_callback.raise< const ::rux::gui::events::Event& >( ::rux::gui::events::Event( *this , 1 ) );
						_cs_drawing_elements.wlock( debuginfo_macros );
						break;
					}
				case ::rux::gui::XEnum_Keys_Home:
					{
						size_t char_index_in_line = 0;
						::rux::uint32 line_index = get_line( _selection_char_index , char_index_in_line );
						if( shift == 1 && _old_selection_char_index == _selection_char_index )
							_old_selection_char_index = _selection_char_index;
						_selection_char_index = get_first_char_index( line_index );
						if( shift == 0 )
							_old_selection_char_index = _selection_char_index;
						_cs_drawing_elements.wunlock( debuginfo_macros );
						calculate_caret_and_selection();
						_cs_drawing_elements.wlock( debuginfo_macros );
						break;
					}
				case ::rux::gui::XEnum_Keys_End:
					{
						size_t char_index_in_line = 0;
						::rux::uint32 line_index = get_line( _selection_char_index , char_index_in_line );
						if( shift == 1 && _old_selection_char_index == _selection_char_index )
							_old_selection_char_index = _selection_char_index;
						_selection_char_index = get_first_char_index( line_index ) + _lines[ line_index ].Length();
						if( shift == 0 )
							_old_selection_char_index = _selection_char_index;
						_cs_drawing_elements.wunlock( debuginfo_macros );
						calculate_caret_and_selection();
						_cs_drawing_elements.wlock( debuginfo_macros );
						break;
					}
				case ::rux::gui::XEnum_Keys_Down:
					{
						size_t char_index_in_line = 0;
						::rux::uint32 line_index = get_line( _selection_char_index , char_index_in_line );
						if( shift == 1 && _old_selection_char_index == _selection_char_index )
							_old_selection_char_index = _selection_char_index;
						if( line_index + 1 < _lines.Count() )
						{				
							if( char_index_in_line < _lines[ line_index + 1 ].Length() + 1 )
								_selection_char_index = get_first_char_index( line_index + 1 ) + char_index_in_line;
							else
								_selection_char_index = get_first_char_index( line_index + 1 ) + _lines[ line_index + 1 ].Length();
							if( shift == 0 )
								_old_selection_char_index = _selection_char_index;
						}
						_cs_drawing_elements.wunlock( debuginfo_macros );
						calculate_caret_and_selection();
						_cs_drawing_elements.wlock( debuginfo_macros );
						break;
					}
				case ::rux::gui::XEnum_Keys_Up:
					{
						size_t char_index_in_line = 0;
						::rux::uint32 line_index = get_line( _selection_char_index , char_index_in_line );
						if( shift == 1 && _old_selection_char_index == _selection_char_index )
							_old_selection_char_index = _selection_char_index;
						if( line_index > 0 )
						{				
							if( char_index_in_line < _lines[ line_index - 1 ].Length() + 1 )
								_selection_char_index = get_first_char_index( line_index - 1 ) + char_index_in_line;
							else
								_selection_char_index = get_first_char_index( line_index - 1 ) + _lines[ line_index - 1 ].Length();
							if( shift == 0 )
								_old_selection_char_index = _selection_char_index;
						}
						_cs_drawing_elements.wunlock( debuginfo_macros );
						calculate_caret_and_selection();
						_cs_drawing_elements.wlock( debuginfo_macros );
						break;
					}
				case ::rux::gui::XEnum_Keys_Right:
					{
						size_t char_index_in_line = 0;
						::rux::uint32 line_index = get_line( _selection_char_index , char_index_in_line );
						if( shift == 1 && _old_selection_char_index == _selection_char_index )
							_old_selection_char_index = _selection_char_index;
						if( char_index_in_line + 1 < _lines[ line_index ].Length() + 1
							|| line_index + 1 < _lines.Count() )
						{
							_selection_char_index++;
							if( shift == 0 )
								_old_selection_char_index = _selection_char_index;
						}
						_cs_drawing_elements.wunlock( debuginfo_macros );
						calculate_caret_and_selection();
						_cs_drawing_elements.wlock( debuginfo_macros );
						break;
					}
				case ::rux::gui::XEnum_Keys_Left:
					{			
						if( shift == 1 && _old_selection_char_index == _selection_char_index )
							_old_selection_char_index = _selection_char_index;
						if( _selection_char_index > 0 )
						{
							_selection_char_index--;
							if( shift == 0 )
								_old_selection_char_index = _selection_char_index;
						}
						_cs_drawing_elements.wunlock( debuginfo_macros );
						calculate_caret_and_selection();
						_cs_drawing_elements.wlock( debuginfo_macros );
						break;
					}
				}
				_cs_drawing_elements.wunlock( debuginfo_macros );
			};
			void TextBox::Invalidate( void )
			{
			};
			void TextBox::private_ResetCache( void )
			{				
			};
			void TextBox::UpdateParentWindow( void )
			{
				_parent_window = get_ParentWindow();
			};
			void TextBox::FreeResources( ::rux::byte )
			{
				::rux::engine::_globals->_gui_globals->_rux_gui_add_cache( _ui_cache );
				_ui_cache = NULL;
				::rux::engine::_globals->_gui_globals->_rux_gui_add_cache( _ui_border_cache );
				_ui_border_cache = NULL;
				for( ::rux::uint32 index0 = 0 ; index0 < _mask_caches.Count() ; index0++ )
						::rux::engine::_globals->_gui_globals->_rux_gui_add_cache( _mask_caches[ index0 ] );
				_mask_caches.Clear();
				for( ::rux::uint32 index0 = 0 ; index0 < _text_caches.Count() ; index0++ )
						::rux::engine::_globals->_gui_globals->_rux_gui_add_cache( _text_caches[ index0 ] );
				_text_caches.Clear();
				for( ::rux::uint32 index0 = 0 ; index0 < _selection_rects_caches.Count() ; index0++ )
					::rux::engine::_globals->_gui_globals->_rux_gui_add_cache( _selection_rects_caches[ index0 ] );
				_selection_rects_caches.Clear();
			};
			void TextBox::raise_OnLeftMouseButtonDoubleClick( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
			};
			void TextBox::raise_OnLeftMouseButtonUp( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_left_mouse_button_up.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			::rux::byte TextBox::raise_OnMouseEnter( ::rux::gui::WindowMouseEvent* , ::rux::byte& raised_event , ::rux::byte explicit_event )
			{
				_on_mouse_enter.raise< const ::rux::gui::events::Event& >( ::rux::gui::events::Event( *this , explicit_event ) );
				return 1;
			};
			implement_empty_content_size( TextBox );
			void TextBox::raise_OnMouseLeave( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_mouse_leave_callback.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void TextBox::raise_OnMouseMove( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
			};
			IMPLEMENT_BASE_UI_FUNCTIONS( XTextBox );
		};
	};
};