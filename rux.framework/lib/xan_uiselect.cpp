#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_uiselect.h>
#include <xan_uiinlines.h>
#include <xan_uiprimitives.h>
#include <xan_uirectangle.h>
#include <xan_float.h>
#include <xan_uitextblock.h>
#include <xan_uiprimitives.h>
#include <xan_keyvaluepair.h>
#include <xan_image.h>
#include <xan_uiimage.h>
begin_implement_rux_class_with_properties_ns_base_class( Select , rux::gui::controls , rux::gui::ControlBase )
	_binding_source.set_Info( "binding_source" , __file__ , __line__ );
	_tag.set_Info( "tag" , __file__ , __line__ );
	_private_tag.set_Info( "private_tag" , __file__ , __line__ );
	IMPLEMENT_BASE_UI_MEMBERS();
	_selected_text_font = ::rux::engine::_globals->_gui_globals->_rux_gui_create_font( "tahoma.ttf" , 0 , 12 );	
	copy_color( _selected_text_foreground , ::rux::gui::Colors::White() );
	_combobox_thickness = ::rux::gui::Thickness( 0.f , 0.f , 0.f , 0.f );
	_items_container_thickness = ::rux::gui::Thickness( 0.f , 0.f , 0.f , 0.f ); 
	copy_color( _combobox_background , ::rux::gui::Colors::ButtonNormalColor() );
	copy_color( _combobox_border , ::rux::gui::Colors::Transparent() );
	copy_color( _content_background , create_color( 255 , 255 , 255 , (uint8)( 255.0 * 0.7 ) ) );
	copy_color( _content_over_background , ::rux::gui::Colors::White() );
	rux::gui::copy_color( _content_active_background , _content_background );
	copy_color( _items_container_background , create_color( 0x33 , 0x33 , 0x33 , 128 ) );
	copy_color( _items_container_border , ::rux::gui::Colors::Transparent() );
	_is_editable = 0;
	_on_item_selected = NULL;
	_items_container_height = 100.f;
	_items_container_width = 100.f;
	_items_container_border_width = 1.f;
	_items_container.set_OnMouseWheel( on_items_container_mouse_wheel );

	_max_visible_items = 10;
	_start_visible_items = 0;
	_item_height = 30;

	_border_width = 1;
	_vertical_text_alignment = XEnum_Alignment_Center;
	_horizontal_text_alignment = XEnum_Alignment_LeftOrTop;
	_text_left_margin = 5.f;
	_text_top_margin = 0.f;
	_reset_cache = 1;
	_text_need_refresh = 1;
	_text_left = 0.f;
	_text_top = 0.f;
	_text_width = 0.f;
	_text_height = 0.f;
	_render_text_width = 0.f;
	_render_text_height = 0.f;
end_implement_rux_class();
namespace rux
{
	namespace gui
	{
		namespace controls
		{
			implement_duplicate_internal_result_function( Object& , Select , get_SelectedItem );
			implement_duplicate_internal_function_1( Select , set_MaxVisibleItems , size_t );
			implement_duplicate_internal_function_1( Select , set_StartVisibleIndex , size_t );		

			::rux::gui::ColorBase* Select::get_Foreground( void )
			{
				return _selected_text_foreground;
			};
			void* Select::DynamicCast( ::rux::int32 class_type ) const
			{
				if( class_type == ::rux::gui::XEnum_ClassType_ControlBase )
				{
					::rux::gui::ControlBase* ptr = const_cast< Select* >( this );
					return ptr;
				}
				return 0;
			};
			implement_duplicate_internal_function_1( Select , set_BorderColor , ::rux::gui::ColorBase* );
			void Select::set_BorderColor( ::rux::gui::ColorBase* border_color )
			{
				_cs_drawing_elements.wlock( debuginfo_macros );
				rux::gui::copy_color( _combobox_border , border_color );
				_cs_drawing_elements.wunlock( debuginfo_macros );
			};
			implement_duplicate_internal_function_1( Select , set_BorderWidth , float );
			void Select::set_BorderWidth( float border_width )
			{
				if( _border_width != border_width )
					_border_width = ::rux::math::round( border_width , 0 );
			};
			implement_duplicate_internal_function_2( Select , set_TextMargin , float , float );
			void Select::set_TextMargin( float left , float top )
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
			implement_duplicate_internal_function_3( Select , set_Font , const char* , ::rux::uint32 , ::rux::uint32 );
			void Select::set_Font( const char* font_file_name ,
				::rux::uint32 font_size_height ,
				::rux::uint32 font_size_width )
			{
				_cs_drawing_elements.wlock( debuginfo_macros );
				_selected_text_font = ::rux::engine::_globals->_gui_globals->_rux_gui_create_font( font_file_name ,
					font_size_height ,
					font_size_width );
				_cs_drawing_elements.wunlock( debuginfo_macros );
				private_ResetLocationAndSizeCache( _horizontal_alignment == XEnum_Alignment_Stick || _horizontal_alignment == XEnum_Alignment_RightOrBottom || _horizontal_alignment == XEnum_Alignment_Center ? 1 : 0 , _vertical_alignment == XEnum_Alignment_Stick || _vertical_alignment == XEnum_Alignment_RightOrBottom || _vertical_alignment == XEnum_Alignment_Center ? 1 : 0 , _horizontal_filling == XEnum_Filling_FromContent ? 1 : 0 , _vertical_filling == XEnum_Filling_FromContent ? 1 : 0 );
				private_ResetCache();
			};
			float Select::get_TextWidth( void )
			{
				return _selected_text_font->get_TextWidth( _selected_text.str() , SIZE_MAX );
			};
			float Select::get_TextHeight( void )
			{
				return _selected_text_font->get_Height( _selected_text.str() , SIZE_MAX );
			};
			::rux::uint8 Select::get_IsSupportContentSize( void )
			{
				return 1;
			};
			implement_duplicate_internal_result_function( float , Select , get_ContentWidth );
			float Select::get_ContentWidth( void )
			{
				float val = 0.f;
				_cs_drawing_elements.wlock( debuginfo_macros );
				if( ::XInterlocked::CompareExchange( &_reset_cache , 0 , 1 ) == 1 )
				{
					_text_need_refresh = 1;
					_text_width = _selected_text_font->get_TextWidth( _selected_text.str() , SIZE_MAX );
					_text_height = _selected_text_font->get_Height( _selected_text.str() , SIZE_MAX );
				}
				val = _text_width + _text_left_margin;
				_cs_drawing_elements.wunlock( debuginfo_macros );
				return val;
			};
			implement_duplicate_internal_result_function( float , Select , get_ContentHeight );
			float Select::get_ContentHeight( void )
			{			
				float val = 0.f;
				_cs_drawing_elements.wlock( debuginfo_macros );
				if( ::XInterlocked::CompareExchange( &_reset_cache , 0 , 1 ) == 1 )
				{
					_text_need_refresh = 1;
					_text_width = _selected_text_font->get_TextWidth( _selected_text.str() , SIZE_MAX );
					_text_height = _selected_text_font->get_Height( _selected_text.str() , SIZE_MAX );
				}
				val = _text_height + _text_top_margin;
				_cs_drawing_elements.wunlock( debuginfo_macros );
				return val;
			};
			void Select::set_Foreground( ::rux::gui::ColorBase* foreground )
			{
				_cs_drawing_elements.wlock( debuginfo_macros );
				rux::gui::copy_color( _selected_text_foreground , foreground );
				_cs_drawing_elements.wunlock( debuginfo_macros );
			};
			void Select::set_Background( ::rux::gui::ColorBase* background )
			{
				_cs_drawing_elements.wlock( debuginfo_macros );
				rux::gui::copy_color( _combobox_background , background );
				_cs_drawing_elements.wunlock( debuginfo_macros );
				private_ResetCache();
			};
			void Select::render( ::rux::gui::RenderContextBase* render_context , float opacity , float& _selected_z_index , size_t ___rux__thread_index1986 )
			{	
				float __left = get_Left();
				float __top = get_Top();
				float __width = get_Width();
				float __height = get_Height();
				if( ::XInterlocked::CompareExchange( &_reset_cache , 0 , 1 ) == 1 )
				{
					_cs_drawing_elements.wlock( debuginfo_macros );
					_text_need_refresh = 1;
					_text_width = _selected_text_font->get_TextWidth( _selected_text.str() , ___rux__thread_index1986 );
					_text_height = _selected_text_font->get_Height( _selected_text.str() , ___rux__thread_index1986 );
					_cs_drawing_elements.wunlock( debuginfo_macros );
				}
				_cs_drawing_elements.wlock( debuginfo_macros );
				render_context->DrawRectangle( __left , __top , __width , __height , _border_width , _combobox_background , _combobox_border , _combobox_thickness , _opacity * opacity , &_ui_border_cache , &_ui_cache , _selected_z_index , 1 , __FILE__ , __LINE__ , ___rux__thread_index1986 );
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
				render_context->DrawText( _text_left , _text_top , _render_text_width , _render_text_height , _selected_text.str() , _selected_text_font , _selected_text_foreground , &_selected_text_cache , _opacity * opacity , _selected_z_index , __FILE__ , __LINE__ , ___rux__thread_index1986 );
				_cs_drawing_elements.wunlock( debuginfo_macros );
			};
			void Select::Invalidate( void )
			{
				private_ResetCache();
			};
			void Select::private_ResetCache( void )
			{
				::XInterlocked::Exchange( &_reset_cache , 1 );
			};
			void Select::set_TextVerticalAlignment( ::rux::gui::XEnum_Alignment text_alignment )
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
			void Select::set_TextHorizontalAlignment( ::rux::gui::XEnum_Alignment text_alignment )
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
			implement_duplicate_internal_function_1( Select , set_Foreground , ::rux::gui::ColorBase* );
			implement_duplicate_internal_function_1( Select ,set_TextVerticalAlignment , ::rux::gui::XEnum_Alignment );
			implement_duplicate_internal_function_1( Select , set_TextHorizontalAlignment , ::rux::gui::XEnum_Alignment );
			implement_duplicate_internal_function_1( Select , set_Background , ::rux::gui::ColorBase* );




			void Select::set_MaxVisibleItems( size_t max_visible_items )
			{
				if( max_visible_items > 0 )
				{
					if( _max_visible_items != max_visible_items )
					{
						_max_visible_items = max_visible_items;
						if( _items_container.get_ParentControl() )
						{
							if( _items_container.get_ControlsCount() > 1 )
							{
								declare_variable_param( ::rux::gui::controls::XGroup , items_group , _items_container.get_ControlAt( 1 ) );		
								ItemsRefresh();
								_items_container.set_Height( items_group.get_Height() );
								Activate();
							}
						}
					}
				}
			};
			void Select::set_StartVisibleIndex( size_t start_visible_index )
			{
				size_t old = _start_visible_items;
				size_t new_index = start_visible_index;
				if( new_index + _max_visible_items >= _items.Count() )
				{
					if( _items.Count() >= _max_visible_items )
						new_index = _items.Count() - _max_visible_items;
					else
						new_index = 0;
				}
				_start_visible_items = new_index;			
				if( old != _start_visible_items )
				{
					if( _items_container.get_ParentControl() )
					{
						if( _items_container.get_ControlsCount() > 1 )
						{
							declare_variable_param( ::rux::gui::controls::XGroup , items_group , _items_container.get_ControlAt( 1 ) );		
							ItemsRefresh();
							_items_container.set_Height( items_group.get_Height() );
							Activate();
						}
					}
				}
			};
			void Select::set_OnItemSelected( ::rux::gui::events::on_event_t on_item_selected_callback )
			{
				_on_item_selected = on_item_selected_callback;
			};
			void XSelect::set_ItemsContainerWidth( float items_container_width )
			{
				if( (*this)()->_items_container_width != items_container_width )
				{
					(*this)()->_cs_drawing_elements.wlock( debuginfo_macros );
					(*this)()->_items_container_width = items_container_width;
					(*this)()->_cs_drawing_elements.wunlock( debuginfo_macros );		
				}
			};
			void XSelect::set_OnItemSelected( ::rux::gui::events::on_event_t on_item_selected_callback )
			{
				(*this)()->set_OnItemSelected( on_item_selected_callback );
			};
			void Select::ItemsRefresh( void )
			{
				if( rux_is_object( _binding_source , XObject ) == false )
					ItemsClear();
				XArray< XObject > items , items_tags;
				CRITICAL_SECTION_LOCK( _cs_items );
				items_tags = _items_tags;
				if( rux_is_object( _binding_source , ::rux::data::XDataObject ) )
				{
					XArray< ::rux::XString > paths;
					paths.set_ByRef( _binding_source_path.Split( '.' ) );
					if( paths.Count() > 0 )
					{
						::rux::uint8 found = 0;
						rux::data::XDataObject json_object( _binding_source );
						if( paths.Count() > 1 )
						{
							for( ::rux::uint32 index0 = 0 ; index0 < paths.Count() - 1 ; index0++ )
								json_object = json_object.GetValue< ::rux::data::XDataObject >( paths[ index0 ] , found , SIZE_MAX , "->" );
						}
						items.set_ByRef( json_object.GetValue< XArray< XObject > >( paths[ paths.Count() - 1 ] , found , SIZE_MAX , "->" ) );
					}
				}
				else if( rux_is_object( _binding_source , XArray< XObject > ) )
					items.set_ByRef( _binding_source );
				else if( rux_is_object( _binding_source , XObject ) )
				{
					items = _items;
					ItemsClear();
				}
				CRITICAL_SECTION_UNLOCK( _cs_items );
				rux::XString error;
				declare_variable_param( ::rux::gui::controls::XGroup , items_group , _items_container.get_ControlAt( 1 ) );		
				if( _start_visible_items != 0 )
				{
					rux::gui::controls::XButton back_button;
					back_button = XPrimitives::TransparentButton();
					back_button.set_OnClick( Select::on_item_button_click );
					back_button.set_VerticalAlignment( XEnum_Alignment_LeftOrTop );
					back_button.set_HorizontalFilling( XEnum_Filling_Auto );
					back_button.set_Height( _item_height );
					if( ::rux::media::XFrame::Exists( "triangle_up32x32.png" ) == 1 )
					{
						::rux::media::XFrame frame;
						frame.set_ImageData( "triangle_up32x32.png" , error );
						::rux::gui::controls::XImage img;
						img.set_HorizontalAlignment( ::rux::gui::XEnum_Alignment_Center );
						img.set_VerticalAlignment( ::rux::gui::XEnum_Alignment_Center );
						img.set_Width( 8 );
						img.set_Height( 8 );
						img.set_IsAlpha( 1 );
						img.set_Frame( frame );
						::rux::gui::controls::XGroup group;
						group.set_Opacity( .7f );
						group.AddControl( img , error );
						back_button.set_Content( group );
					}
					else
						back_button.set_Content( ::rux::XString( "Вверх" ) );
					back_button.set_Margin( ::rux::gui::Margin( 3.f , 0.f , 1.f , 2.f ) );				
					items_group.AddControl( back_button , error );
				}
				for( size_t index0 = 0 ; index0 < items.Count() ; index0++ )
				{
					if( index0 >= _start_visible_items && index0 < _start_visible_items + _max_visible_items )
					{
						if( items.Count() > index0 )
						{
							::rux::uint8 selected = _selected_index == index0 ? 1 : 0;
							ItemsAdd( XObject( items[ index0 ] ) , selected );
							if( items_tags.Count() > index0 )
								_items_tags.set_ItemByRef( index0 , items_tags[ index0 ] );
						}
					}
					else
					{
						CRITICAL_SECTION_LOCK( _cs_items );
						_items.AddByRef( items[ index0 ] );
						if( items_tags.Count() > index0 )
							_items_tags.AddByRef( items_tags[ index0 ] );
						else
							_items_tags.AddByRef( XObject( "" , __FILE__ , __LINE__ ) );
						CRITICAL_SECTION_UNLOCK( _cs_items );
					}
				}
				if( _start_visible_items + _max_visible_items < _items.Count() )
				{
					rux::gui::controls::XButton forward_button;
					forward_button = XPrimitives::TransparentButton();
					forward_button.set_OnClick( Select::on_item_button_click );
					forward_button.set_VerticalAlignment( XEnum_Alignment_LeftOrTop );
					forward_button.set_HorizontalFilling( XEnum_Filling_Auto );
					forward_button.set_Height( _item_height );
					if( ::rux::media::XFrame::Exists( "triangle_down32x32.png" ) == 1 )
					{
						::rux::media::XFrame frame;
						frame.set_ImageData( "triangle_down32x32.png" , error );
						::rux::gui::controls::XImage img;
						img.set_HorizontalAlignment( ::rux::gui::XEnum_Alignment_Center );
						img.set_VerticalAlignment( ::rux::gui::XEnum_Alignment_Center );
						img.set_Width( 8 );
						img.set_Height( 8 );
						img.set_IsAlpha( 1 );
						img.set_Frame( frame );
						::rux::gui::controls::XGroup group;
						group.set_Opacity( .7f );
						group.AddControl( img , error );
						forward_button.set_Content( group );
					}
					else
						forward_button.set_Content( ::rux::XString( "Вниз" ) );
					forward_button.set_Margin( ::rux::gui::Margin( 3.f , 0.f , 1.f , 2.f ) );
					items_group.AddControl( forward_button , error );
				}
				update_main_text();
			};
			void Select::set_BindingSource( const XObject& binding_source )
			{
				_binding_source = binding_source;
				ItemsRefresh();
				update_main_text();
			};
			void Select::set_BindingSourcePath( const ::rux::XString& binding_source_path )
			{
				if( _binding_source_path != binding_source_path )
				{
					_binding_source_path = binding_source_path;
					ItemsRefresh();
					update_main_text();
				}
			};
			void Select::set_BindingSourceDisplayPropertyName( const ::rux::XString& binding_source_display_property_name )
			{
				if( _binding_source_display_property_name != binding_source_display_property_name )
				{
					_binding_source_display_property_name = binding_source_display_property_name;
					ItemsRefresh();
					update_main_text();
				}
			};
			void XSelect::set_BindingSource( const XObject& binding_source )
			{
				(*this)()->set_BindingSource( binding_source );
			};
			void XSelect::set_BindingSourcePath( const ::rux::XString& binding_source_path )
			{
				(*this)()->set_BindingSourcePath( binding_source_path );
			};
			void XSelect::set_BindingSourceDisplayPropertyName( const ::rux::XString& binding_source_display_property_name )
			{
				(*this)()->set_BindingSourceDisplayPropertyName( binding_source_display_property_name );
			};
			implement_duplicate_internal_function( Select , ItemsClear );
			void Select::ItemsClear( void )
			{
				CRITICAL_SECTION_LOCK( _cs_items );
				_items.Clear();
				_items_tags.Clear();
				CRITICAL_SECTION_UNLOCK( _cs_items );
				declare_variable_param( ::rux::gui::controls::XGroup , items , _items_container.get_ControlAt( 1 ) );		
				_items_container.set_Height( 0.f );
				items.ClearControls();
				update_main_text();
			};
			implement_duplicate_internal_result_function( size_t , Select , ItemsCount );
			size_t Select::ItemsCount( void )
			{
				size_t count = 0;
				CRITICAL_SECTION_LOCK( _cs_items );
				count = _items.Count();
				CRITICAL_SECTION_UNLOCK( _cs_items );
				return count;
			};
			implement_duplicate_internal_function_2( Select , set_ItemTag , size_t , const XObject& );
			void Select::set_ItemTag( size_t index , const XObject& tag )
			{
				CRITICAL_SECTION_LOCK( _cs_items );
				if( _items_tags.Count() > index )
					_items_tags.set_Item( index , tag );
				CRITICAL_SECTION_UNLOCK( _cs_items );
			};
			implement_duplicate_internal_result_function_1( Object& , Select , get_ItemTag , size_t );
			Object& Select::get_ItemTag( size_t index )
			{
				XObject tag;
				CRITICAL_SECTION_LOCK( _cs_items );
				if( _items_tags.Count() > index )
					tag.set_ByRef( _items_tags[ index ] );
				CRITICAL_SECTION_UNLOCK( _cs_items );
				return tag++;
			};
			void Select::ItemsRemove(size_t index)
			{
				CRITICAL_SECTION_LOCK( _cs_items );
				_items.RemoveAt(index);
				_items_tags.RemoveAt(index);
				CRITICAL_SECTION_UNLOCK( _cs_items );
				declare_variable_param( ::rux::gui::controls::XGroup , items , _items_container.get_ControlAt( 1 ) );
				items.RemoveControl(items.get_ControlAt(index));
				if(_selected_index == index)
				{
					_selected_index = 0;
					update_main_text();
					_on_item_selected.raise<const ::rux::gui::events::Event&>(::rux::gui::events::Event(*this, 1));
				}
			};
			void Select::ItemsAdd( const XObject& item , ::rux::uint8 selected )
			{
				if( _items_container.get_ControlsCount() == 0 )
					create( 0 );
				rux::XString item_string;
				if( rux_is_object( item , ::rux::XString ) )
				{
					CRITICAL_SECTION_LOCK( _cs_items );
					if( selected == 1 )
						_selected_index = _items.Count();
					_items.AddByRef( item );
					_items_tags.AddByRef( XObject( "" , __FILE__ , __LINE__ ) );
					CRITICAL_SECTION_UNLOCK( _cs_items );		
					item_string.set_ByRef( item );
				}
				else if( rux_is_object( item , ::rux::data::XDataObject ) )
				{
					rux::data::XDataObject json_object( item );
					::rux::uint8 found = 0;
					item_string.set_ByRef( json_object.GetValue< ::rux::XString >( _binding_source_display_property_name , found , SIZE_MAX , "->" ) );
					CRITICAL_SECTION_LOCK( _cs_items );
					if( selected == 1 )
						_selected_index = _items.Count();
					_items.AddByRef( item );
					_items_tags.AddByRef( XObject( "" , __FILE__ , __LINE__ ) );
					CRITICAL_SECTION_UNLOCK( _cs_items );
				}
				rux::gui::controls::XButton item_button;
				item_button = XPrimitives::TransparentButton();
				item_button.set_OnClick( Select::on_item_button_click );
				item_button.set_VerticalAlignment( XEnum_Alignment_LeftOrTop );
				item_button.set_HorizontalFilling( XEnum_Filling_Auto );
				item_button.set_Height( _item_height );
				item_button.set_Content( item_string );
				item_button.set_Margin( ::rux::gui::Margin( 3.f , 0.f , 1.f , 2.f ) );	
				if( selected == 1 )
				{
					item_button.set_Background( create_color( 0x42 , 0xB6 , 0xE4 , 0xff ) );
					item_button.set_OverBackground( create_color( 0x42 , 0xB6 , 0xE4 , 0xff ) );
					_cs_drawing_elements.wlock( debuginfo_macros );
					_selected_text.set_ByRef( item_string.ConvertToUTF8() );
					_cs_drawing_elements.wunlock( debuginfo_macros );
				}
				rux::XString error;
				declare_variable_param( ::rux::gui::controls::XGroup , items , _items_container.get_ControlAt( 1 ) );		
				items.AddControl( item_button , error );
				_cs_drawing_elements.wlock( debuginfo_macros );
				if( _selected_text.Length() == 0 )
					update_main_text();
				_cs_drawing_elements.wunlock( debuginfo_macros );
			};
			void Select::on_completed_animation( const ::rux::gui::events::Event& event )
			{
				XGroupAnimation group_animation( event._sender );
				XAnimation animation( group_animation.get_Animation( 0 ) );
				declare_variable_param( ::rux::gui::controls::XGroup , group , animation.get_Control() );
				declare_variable_param( ::rux::gui::controls::XGroup , items , group.get_ControlAt( 1 ) );
				Select* combobox = (Select*)group.get_TagPtr();
				if( items.get_ControlsCount() > combobox->_selected_index )
				{
					XObject object( items.get_ControlAt( combobox->_selected_index ) );
					if( rux_is_object( object , XTextBlock ) )
					{
						XTextBlock textblock( object );
						textblock.set_Background( ::rux::gui::Colors::ButtonDownColor() );
					}
				}
			};
			Select::~Select()
			{
				FreeResources( 0 );
				_items_container_group_animation.Stop();
				if( _combobox_background )
					_combobox_background->Release();
				if( _combobox_border )
					_combobox_border->Release();
				if( _content_background )
					_content_background->Release();
				if( _content_over_background )
					_content_over_background->Release();
				if( _content_active_background )
					_content_active_background->Release();
				if( _items_container_background )
					_items_container_background->Release();
				if( _items_container_border )
					_items_container_border->Release();
				if( _selected_text_foreground )
					_selected_text_foreground->Release();
			};
			void Select::UpdateParentWindow( void )
			{
				_parent_window = get_ParentWindow();
			};
			void Select::FreeResources( ::rux::byte )
			{
				::rux::engine::_globals->_gui_globals->_rux_gui_add_cache( _ui_cache );
				_ui_cache = NULL;
				::rux::engine::_globals->_gui_globals->_rux_gui_add_cache( _ui_border_cache );
				_ui_border_cache = NULL;
				::rux::engine::_globals->_gui_globals->_rux_gui_add_cache( _selected_text_cache );
				_selected_text_cache = NULL;
			};
			void Select::raise_OnRightMouseButtonDown( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_right_mouse_button_down.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void Select::raise_OnRightMouseButtonDoubleClick( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_right_mouse_button_double_click.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void Select::raise_OnRightMouseButtonUp( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_right_mouse_button_up.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void Select::raise_OnLeftMouseButtonDoubleClick( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				raise_OnLeftMouseButtonDown( window_event , 1 );
			};
			::rux::byte Select::raise_OnMouseEnter( ::rux::gui::WindowMouseEvent* , ::rux::byte& raised_event , ::rux::byte explicit_event )
			{
				_on_mouse_enter.raise< const ::rux::gui::events::Event& >( ::rux::gui::events::Event( *this , explicit_event ) );
				return 1;
			};
			void Select::raise_OnMouseLeave( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_mouse_leave_callback.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void Select::raise_OnKeyChar( const ::rux::XString& , ::rux::uint8 , ::rux::uint8 , ::rux::uint8 )
			{		
			};
			void Select::raise_OnKeyDown( ::rux::gui::XEnum_Keys , ::rux::uint8 , ::rux::uint8 , ::rux::uint8 , ::rux::uint32 , ::rux::byte explicit_event )
			{
			};
			void Select::create( ::rux::uint8 show )
			{
				if( _items_container.get_ControlsCount() == 0 )
				{
					rux::gui::controls::XRectangle items_container_back;
					items_container_back.set_Left( 0 );
					items_container_back.set_Top( 0 );
					items_container_back.set_HorizontalFilling( XEnum_Filling_Auto );
					items_container_back.set_VerticalFilling( XEnum_Filling_Auto );
					items_container_back.set_Background( _items_container_background );	
					items_container_back.set_BorderColor( _items_container_border );
					items_container_back.set_BorderWidth( _items_container_border_width );
					items_container_back.set_Corner( _items_container_thickness );
					rux::XString error;
					_items_container.AddControl( items_container_back , error );
					declare_variable( ::rux::gui::controls::XGroup , items );
					items.set_ChildLocationType( XEnum_Child_Location_Type_VerticalStack );	
					items.set_HorizontalFilling( XEnum_Filling_Auto );
					items.set_VerticalFilling( XEnum_Filling_FromContent );	
					_items_container.AddControl( items , error );
					_items_container.set_IsVisible( 0 );	
					_items_container.set_TagPtr( this );
					items.set_TagPtr( this );
				}
				ItemsRefresh();
				if( show == 1 )
				{
					private_set_ActiveContentBackground( _content_over_background );	
					declare_variable_param( ::rux::gui::controls::XGroup , items , _items_container.get_ControlAt( 1 ) );
					float height = items.get_Height();
					rux::gui::Window* window = get_ParentWindow();
					if( window && ( get_Top() + get_Height() + height > window->get_Height() && window->get_Height() - get_Top() - get_Height() < get_Top() ) )
					{				
						_items_container_group_animation.Clear();
						_items_container_group_animation.set_OnCompleted( on_completed_animation );
						_items_container.set_Width( _items_container_width );
						_items_container.set_Height( 0 );
						_items_container.set_BottomSticker( get_Top() );
						_items_container.set_Left( get_Left() );
						XAnimation animation;	
						animation.set_Control( _items_container );
						animation.set_AnimationSetValueType( XEnum_AnimationSetValueType_Gradual );
						animation.set_StartValue( XFloat( 0.f ) );
						animation.set_PropertyName( "Height" );
						animation.AddTimeline( 100000ULL , XFloat( height ) );				
						_items_container_group_animation.set_AnimationType( XEnum_AnimationType_OneTime );
						_items_container_group_animation.AddAnimation( animation );						
						rux::XString error;
						window->AddControl( _items_container , error );
						_items_container.set_IsVisible( 1 );
						_items_container_group_animation.Start();
					}
					else
					{
						_items_container_group_animation.Clear();
						_items_container_group_animation.set_OnCompleted( on_completed_animation );
						_items_container.ResetBottomSticker();
						_items_container.set_Width( _items_container_width );
						_items_container.set_Top( get_Top() + get_Height() );
						_items_container.set_Left( get_Left() );
						_items_container.set_Height( 0 );				
						XAnimation animation;	
						animation.set_Control( _items_container );
						animation.set_AnimationSetValueType( XEnum_AnimationSetValueType_Gradual );
						animation.set_StartValue( XFloat( 0.f ) );
						animation.set_PropertyName( "Height" );
						animation.AddTimeline( 100000ULL , XFloat( height ) );				
						_items_container_group_animation.set_AnimationType( XEnum_AnimationType_OneTime );
						_items_container_group_animation.AddAnimation( animation );						
						rux::XString error;
						if( window )
							window->AddControl( _items_container , error );
						_items_container.set_IsVisible( 1 );
						_items_container_group_animation.Start();
					}			
				}
			};
			void Select::destroy( void )
			{
				_items_container_group_animation.Clear();		
				private_set_ActiveContentBackground( _content_background );
				rux::gui::Window* window = get_ParentWindow();
				if( window )
					window->RemoveControl( _items_container );
				_items_container.set_IsVisible( 0 );
			};
			void Select::raise_OnLeftMouseButtonDown( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_left_mouse_button_down.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , 1 ) );
				float button_height = get_Height() - 4.f;
				float button_top = get_Top() + 2.f;
				float button_left = get_Left() + get_Width() - button_height - 2.f;
				if( _is_editable == 0 
					|| (button_left <= window_event->_x && button_top <= window_event->_y 
					&& button_left + button_height >= window_event->_x
					&& button_top + button_height >= window_event->_y ) )
				{
					if( _items_container.get_ParentControl() )
						destroy();
					else
						create( 1 );
				}
			};
			void Select::raise_OnLeftMouseButtonUp( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{	
				_on_left_mouse_button_up.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void Select::private_set_ActiveContentBackground( ::rux::gui::ColorBase* background )
			{				
				_cs_drawing_elements.wlock( debuginfo_macros );
				rux::gui::copy_color( _content_active_background , background );
				_cs_drawing_elements.wunlock( debuginfo_macros );
			};
			void Select::raise_OnMouseMove( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{	
				if( _items_container.get_ParentControl() )
				{
					private_set_ActiveContentBackground( _content_over_background );
				}
				else
				{
					float button_height = get_Height() - 4.f;
					float button_top = get_Top() + 2.f;
					float button_left = get_Left() + get_Width() - button_height - 2.f;
					if( button_left <= window_event->_x && button_top <= window_event->_y 
						&& button_left + button_height >= window_event->_x
						&& button_top + button_height >= window_event->_y )
						private_set_ActiveContentBackground( _content_over_background );
					else
						private_set_ActiveContentBackground( _content_background );
				}
			};
			void Select::raise_OnActivate( void )
			{		
			};
			void Select::raise_OnDeactivate( ::rux::gui::CommonBase* activated_control )
			{		
				if( _items_container.get_ParentControl() )
				{
					if( activated_control )
					{
						XGCRef* gcref = activated_control;
						if( gcref != this )
						{
							declare_variable_param( ::rux::gui::controls::XGroup , items , _items_container.get_ControlAt( 1 ) );
							if( items.ExistsControl( *gcref ) == 0 )
								destroy();
						}
						else
							destroy();
					}
					else 
						destroy();		
				}
				XObject activated;
				if( activated_control )
					activated.set_ByRef( XObject( activated_control , 1 ) );
				_on_deactivated.raise< const XObject& , const XObject& >( XObject( *this ) , activated );
			};
			void Select::on_item_button_click( const ::rux::gui::events::Event& event )
			{
				if( rux_is_object( event._sender , XButton ) )
				{
					rux::gui::controls::XButton button( event._sender );
					declare_variable_param( ::rux::gui::controls::XGroup , items , *button.get_ParentControl() );
					Select* combobox = (Select*)items.get_TagPtr();
					::rux::int32 index = items.IndexOf( button );
					if( combobox->_start_visible_items != 0 && index == 0 )
					{
						size_t old = combobox->_start_visible_items;
						if( combobox->_start_visible_items >= combobox->_max_visible_items )
							combobox->_start_visible_items -= combobox->_max_visible_items;
						else
							combobox->_start_visible_items = 0;
						if( old != combobox->_start_visible_items )
						{
							combobox->ItemsRefresh();
							combobox->_items_container.set_Height( items.get_Height() );							
						}
						combobox->Activate();
					}
					else if( combobox->_start_visible_items + combobox->_max_visible_items < combobox->_items.Count()
						&& ( index > combobox->_items.Count()
						|| index == combobox->_max_visible_items + ( combobox->_start_visible_items == 0 ? 0 : 1 ) ) )
					{
						size_t old = combobox->_start_visible_items;
						size_t new_index = combobox->_start_visible_items + combobox->_max_visible_items;
						if( new_index + combobox->_max_visible_items >= combobox->_items.Count() )
						{
							if( combobox->_items.Count() >= combobox->_max_visible_items )
								new_index = combobox->_items.Count() - combobox->_max_visible_items;
							else
								new_index = 0;
						}
						combobox->_start_visible_items = new_index;
						if( old != combobox->_start_visible_items )
						{
							combobox->ItemsRefresh();
							combobox->_items_container.set_Height( items.get_Height() );							
						}
						combobox->Activate();
					}
					else
					{	
						if( combobox->_start_visible_items != 0 )
							index -= 1;
						combobox->set_SelectedIndex( combobox->_start_visible_items + index );
						combobox->destroy();
					}
				}
			};
			::rux::uint8 Select::get_IsGroup( void )
			{
				return 0;
			};
			::rux::uint8 Select::get_IsTimePicker( void )
			{
				return 0;
			};
			::rux::uint8 Select::get_IsButton( void )
			{
				return 0;
			};
			Object& Select::get_SelectedItem( void )
			{
				XObject item;
				CRITICAL_SECTION_LOCK( _cs_items );
				if( _selected_index < _items.Count() )
					item = _items[ _selected_index ];
				CRITICAL_SECTION_UNLOCK( _cs_items );
				return item++;
			};
			void XSelect::set_SelectedIndex( ::rux::uint32 index , ::rux::byte raise_event )
			{
				(*this)()->set_SelectedIndex( index , raise_event );
			};
			implement_duplicate_internal_result_function( ::rux::uint32 , Select , get_SelectedIndex );
			rux::uint32 Select::get_SelectedIndex( void )
			{
				return _selected_index;
			};
			void Select::set_SelectedIndex( ::rux::uint32 index , ::rux::byte raise_event )
			{		
				CRITICAL_SECTION_LOCK( _cs_items );
				if( _selected_index != index
					&& _items.Count() )
				{		
					_selected_index = index;
					update_main_text();
					if( raise_event )
						_on_item_selected.raise< const ::rux::gui::events::Event& >( ::rux::gui::events::Event( *this , 1 ) );
				}
				_cs_drawing_elements.wlock( debuginfo_macros );
				if( _selected_text.Length() == 0 )
					update_main_text();
				_cs_drawing_elements.wunlock( debuginfo_macros );
				_cs_items.UnLock();
			};
			void Select::update_main_text( void )
			{
				CRITICAL_SECTION_LOCK( _cs_items );
				if( _items.Count() > _selected_index )
				{
					rux::XString item_string;
					if( rux_is_object( _items[ _selected_index ] , ::rux::XString ) )
						item_string.set_ByRef( _items[ _selected_index ] );
					else if( rux_is_object( _items[ _selected_index ] , ::rux::data::XDataObject ) )
					{
						rux::data::XDataObject json_object( _items[ _selected_index ] );
						::rux::uint8 found = 0;
						item_string.set_ByRef( json_object.GetValue< ::rux::XString >( _binding_source_display_property_name , found , SIZE_MAX , "->" ) );
					}
					_cs_drawing_elements.wlock( debuginfo_macros );
					_selected_text.set_ByRef( item_string.ConvertToUTF8() );
					_cs_drawing_elements.wunlock( debuginfo_macros );
				}
				else
				{
					_cs_drawing_elements.wlock( debuginfo_macros );
					_selected_text.Clear();
					_cs_drawing_elements.wunlock( debuginfo_macros );
				}
				CRITICAL_SECTION_UNLOCK( _cs_items );
				private_ResetLocationAndSizeCache( _horizontal_alignment == XEnum_Alignment_Stick || _horizontal_alignment == XEnum_Alignment_RightOrBottom || _horizontal_alignment == XEnum_Alignment_Center ? 1 : 0 , _vertical_alignment == XEnum_Alignment_Stick || _vertical_alignment == XEnum_Alignment_RightOrBottom || _vertical_alignment == XEnum_Alignment_Center ? 1 : 0 , _horizontal_filling == XEnum_Filling_FromContent ? 1 : 0 , _vertical_filling == XEnum_Filling_FromContent ? 1 : 0 );
				private_ResetCache();
			};
			void XSelect::ItemsRemove(size_t index)
			{
				operator()()->ItemsRemove(index);
			};
			void XSelect::ItemsAdd( const XObject& item , ::rux::uint8 selected )
			{
				(*this)()->ItemsAdd( item , selected );
			};
			void Select::on_items_container_mouse_wheel( const ::rux::gui::events::MouseEvent& event )
			{
				if( event._explicit_event == 1 )
				{
					if( rux_is_object( event._sender , ::rux::gui::controls::XGroup ) )
					{
						XGroup items_container( event._sender );
						declare_variable_param( ::rux::gui::controls::XGroup , items , items_container.get_ControlAt( 1 ) );
						Select* combobox = (Select*)items_container.get_TagPtr();
						size_t old = combobox->_start_visible_items;
						if( event._window_event->_delta > 0 )
						{
							if( combobox->_start_visible_items >= combobox->_max_visible_items )
								combobox->_start_visible_items -= combobox->_max_visible_items;
							else
								combobox->_start_visible_items = 0;
						}
						else
						{
							size_t new_index = combobox->_start_visible_items + combobox->_max_visible_items;
							if( new_index + combobox->_max_visible_items >= combobox->_items.Count() )
							{
								if( combobox->_items.Count() >= combobox->_max_visible_items )
									new_index = combobox->_items.Count() - combobox->_max_visible_items;
								else
									new_index = 0;
							}
							combobox->_start_visible_items = new_index;						
						}
						if( old != combobox->_start_visible_items )
						{
							combobox->ItemsRefresh();
							combobox->_items_container.set_Height( items.get_Height() );						
						}
						combobox->Activate();
					}
				}
			};
			void Select::raise_OnMouseWheel( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{	
	
			};
			//void Select::render( ::rux::gui::RenderContextBase* render_context , float opacity , float& _selected_z_index , size_t ___rux__thread_index1986 )
			//{
			//	CODE_LABEL( NULL , _utf8_control_name , 5 );	
			//	_cs_drawing_elements.rlock( debuginfo_macros );
			//	render_context->DrawRectangle( get_Left() , get_Top() , get_Width() , get_Height() , 1.f , _combobox_background , _combobox_border , _combobox_thickness , _opacity * opacity , &_ui_border_cache , &_ui_cache , _selected_z_index , 1 , __FILE__ , __LINE__ , ___rux__thread_index1986 );
			//	float button_height = get_Height() - 4.f;
			//	_selected_z_index += 1.f;//0.001f
			//	if( _selected_text.Length() > 0 )
			//	{
			//		float text_height = _selected_text_font->get_Height();
			//		float text_top = get_Top() + ( get_Height() - text_height ) / 2.f;
			//		float text_left = get_Left() + 5.f;
			//		float text_width = get_Width() - button_height - 10.f;
			//		render_context->DrawText( text_left , text_top , text_width , text_height , _selected_text.str() , _selected_text_font , _selected_text_foreground , &_selected_text_cache , _opacity * opacity , _selected_z_index , __FILE__ , __LINE__ , ___rux__thread_index1986 );
			//	}
			//	_cs_drawing_elements.wunlock( debuginfo_macros );
			//};
			//void Select::private_ResetCache( void )
			//{
			//};
			::rux::uint8 Select::get_IsTransparent( void )
			{
				return 0;
			};
			begin_implement_rux_base_ui_functions( Select )
			end_implement_rux_base_ui_functions();
			IMPLEMENT_BASE_UI_FUNCTIONS( XSelect );
		};
	};
};