#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_gui_listbox.h>
#include <xan_keyvaluepair.h>
#include <xan_uirectangle.h>
#include <xan_uibutton.h>
#include <xan_uitextblock.h>
#include <xan_uitextbox.h>
#include <xan_int64.h>
#include <xan_uiprimitives.h>
#include <xan_uint64.h>
#include <xan_uiimage.h>
#include <xan_boolean.h>
begin_implement_rux_class_ns( ColumnInfo , rux::gui::controls );

end_implement_rux_class();

begin_rux_gui_user_control_constructor_ns( ListBox , rux::gui::controls )
	_update_threads_count = 0;
	_group_pages.set_ChildLocationType( XEnum_Child_Location_Type_HorizontalStack );
	_group_all.set_ChildLocationType( XEnum_Child_Location_Type_VerticalStack );
	_group_all.set_HorizontalFilling( ::rux::gui::XEnum_Filling_Auto );
	_group_all.set_VerticalFilling( ::rux::gui::XEnum_Filling_Auto );
	_group_content.set_TagPtr( this );
	_group_content.set_ChildLocationType( XEnum_Child_Location_Type_HorizontalStack );
	_item_height = 24.f;
	_old_mouse_x = 0.f;
	_column_header_height = 20.f;
	_visible_rows_count = 0;
	_visible_rows_index = 0;
	_items_virtualization = NULL;
	_can_remove = 0;
	_current_page = 0;
end_rux_gui_user_control_constructor();

begin_rux_gui_user_control_initialize_ns( ListBox , rux::gui::controls )
	::rux::gui::controls::XRectangle rect;
	rect.set_HorizontalFilling( ::rux::gui::XEnum_Filling_Auto );
	rect.set_VerticalFilling( ::rux::gui::XEnum_Filling_Auto );
	rect.set_Background( create_color( 0 , 0 , 0 , (::rux::byte)( 255 * 0.8f ) ) );
	::rux::XString error;
	AddControl( rect , error );

	_group_pages.set_Height( 40 );
	_group_pages.set_VerticalAlignment( ::rux::gui::XEnum_Alignment_LeftOrTop );
	_group_pages.set_HorizontalFilling( ::rux::gui::XEnum_Filling_Auto );
		
	_textblock_pages.set_Margin( ::rux::gui::Margin( 2 , 2 , 0 , 0 ) );
	_textblock_pages.set_HorizontalFilling( ::rux::gui::XEnum_Filling_FromContent );
	_textblock_pages.set_Height( 20 );
	_textblock_pages.set_HorizontalAlignment( ::rux::gui::XEnum_Alignment_LeftOrTop );
	_textblock_pages.set_VerticalAlignment( ::rux::gui::XEnum_Alignment_Center );
	_group_pages.AddControl( _textblock_pages , error );

	_textbox_pages_index.set_TagPtr( this );
	_textbox_pages_index.set_RegexpMask( "[1-9]+[0-9]*" , error );
	_textbox_pages_index.set_Margin( ::rux::gui::Margin( 2 , 2 , 0 , 0 ) );
	_textbox_pages_index.set_Width( 40 );
	_textbox_pages_index.set_Height( 20 );
	_textbox_pages_index.set_HorizontalAlignment( ::rux::gui::XEnum_Alignment_LeftOrTop );
	_textbox_pages_index.set_VerticalAlignment( ::rux::gui::XEnum_Alignment_Center );		
	_textbox_pages_index.set_OnTextChanged( ListBox::on_textbox_page_text_changed );
	_group_pages.AddControl( _textbox_pages_index , error );

	_textblock_pages_count.set_Margin( ::rux::gui::Margin( 2 , 2 , 0 , 0 ) );
	_textblock_pages_count.set_HorizontalFilling( ::rux::gui::XEnum_Filling_FromContent );
	_textblock_pages_count.set_Height( 20 );
	_textblock_pages_count.set_HorizontalAlignment( ::rux::gui::XEnum_Alignment_LeftOrTop );
	_textblock_pages_count.set_VerticalAlignment( ::rux::gui::XEnum_Alignment_Center );
	_group_pages.AddControl( _textblock_pages_count , error );
		
	_group_all.AddControl( _group_pages , error );

	_group_content.set_VerticalAlignment( ::rux::gui::XEnum_Alignment_LeftOrTop );
	_group_content.set_OnMouseWheel( ListBox::on_group_content_mouse_wheel );
	_group_content.set_HorizontalFilling( ::rux::gui::XEnum_Filling_Auto );
	_group_content.set_VerticalFilling( ::rux::gui::XEnum_Filling_Auto );
	_group_content.set_OnResized( ListBox::on_group_content_resized );
	_group_all.AddControl( _group_content , error );

	AddControl( _group_all , error );

	_rectangle_loader.set_Opacity( 0.7f );
	_rectangle_loader.set_Background( ::rux::gui::Colors::VerticalSliderPanelColor() );
	_rectangle_loader.set_HorizontalFilling( ::rux::gui::XEnum_Filling_Auto );
	_rectangle_loader.set_VerticalFilling( ::rux::gui::XEnum_Filling_Auto );
	_rectangle_loader.set_IsVisible( 0 );
	AddControl( _rectangle_loader , error );	

	_group_loader = ::rux::gui::XPrimitives::Loader();
	_group_loader.set_Width( 25 );
	_group_loader.set_Height( 25 );
	_group_loader.set_HorizontalAlignment( ::rux::gui::XEnum_Alignment_Center );
	_group_loader.set_VerticalAlignment( ::rux::gui::XEnum_Alignment_Center );
	::rux::gui::XPrimitives::LoaderAnimation( _group_loader , _group_animation , 100000ULL );
	_group_loader.set_IsVisible( 0 );
	AddControl( _group_loader , error );
end_rux_gui_user_control_initialize();
namespace rux
{
	namespace gui
	{
		namespace controls
		{
			implement_duplicate_internal_function_1( ColumnInfo , set_ColumnName , const ::rux::XString& );
			void ColumnInfo::set_ColumnName( const ::rux::XString&  column_name )
			{
				_column_name = column_name;
			};			
			ListBox::~ListBox( void )
			{
				while( XInterlocked::CompareExchange( &_update_threads_count , 0 , 0 ) > 0 )
					::rux::threading::XThread::Sleep( 1 );
				FreeResources( 0 );				
				if( _items_virtualization )
					_items_virtualization->Release();
			};
			implement_duplicate_internal_function_1( ListBox , set_OnLanguage , ::rux::gui::events::on_language_t );
			void ListBox::set_OnLanguage( ::rux::gui::events::on_language_t on_language )
			{
				_on_language = on_language;
			};
			implement_duplicate_internal_function_1( ListBox , set_OnItemRemoved , ::rux::gui::events::on_object_event_t );
			void ListBox::set_OnItemRemoved( ::rux::gui::events::on_object_event_t on_item_removed )
			{
				_on_item_removed = on_item_removed;
			};
			implement_duplicate_internal_function_1( ListBox , set_OnItemLeftMouseButtonDoubleClick , ::rux::gui::events::on_object_event_t );
			void ListBox::set_OnItemLeftMouseButtonDoubleClick( ::rux::gui::events::on_object_event_t on_item_left_mouse_button_double_click )
			{
				_on_item_left_mouse_button_double_click = on_item_left_mouse_button_double_click;
			};
			implement_duplicate_internal_function_1( ListBox , set_CanRemove , ::rux::byte );
			void ListBox::set_CanRemove( ::rux::byte can_remove )
			{
				_can_remove = can_remove;
				check_button_remove();
			};
			implement_duplicate_internal_function_2( ListBox , AddAdditionalControl , const XObject& , ::rux::XString& );
			void ListBox::AddAdditionalControl( const XObject& control , ::rux::XString& error )
			{
				UNREFERENCED_PARAMETER( control );
				UNREFERENCED_PARAMETER( error );
			};
			void ListBox::on_group_content_mouse_wheel( const ::rux::gui::events::MouseEvent& event )
			{
				::rux::gui::controls::XGroup group_content( event._sender );
				ListBox* listbox = (ListBox*)group_content.get_TagPtr();
				if( listbox )
				{					
					size_t visible_items = listbox->get_MaxVisibleItems();
					size_t count = listbox->_items_virtualization ? listbox->_items_virtualization->Count() : listbox->_rows.Count();
					size_t old = XInterlocked::CompareExchange( &listbox->_visible_rows_index , 0 , 0 );
					size_t new_index = old;
					if( event._window_event->_delta > 0 )
					{
						if( XInterlocked::CompareExchange( &listbox->_visible_rows_index , 0 , 0 ) >= visible_items )
							new_index -= visible_items;
						else
							new_index = 0;
					}
					else
					{
						new_index = XInterlocked::CompareExchange( &listbox->_visible_rows_index , 0 , 0 ) + visible_items;
						if( new_index + visible_items >= count )
						{
							if( count >= visible_items )
								new_index = count - visible_items;
							else
								new_index = 0;
						}				
					}
					if( old != new_index )
						listbox->UpdateRowsUI( new_index );
				}
			};
			void ListBox::on_item_left_mouse_double_click( const ::rux::gui::events::MouseEvent& event )
			{
				if( event._explicit_event )
				{
					declare_variable_param( ::rux::gui::controls::XGroup , group_cell , event._sender );
					declare_variable_param( ::rux::gui::XRow , row , group_cell.get_Tag() );
					declare_variable_param( XObject , item_object , row.get_ItemObject() );
					ListBox* listbox = (ListBox*)group_cell.get_TagPtr();
					listbox->_on_item_left_mouse_button_double_click.raise< const XObject& , const XObject& >( XObject( listbox , 1 ) , item_object );
				}
			};
			void ListBox::on_column_line_left_mouse_double_click( const ::rux::gui::events::MouseEvent& event )
			{
				::rux::gui::controls::XRectangle column_line( event._sender );
				::rux::gui::controls::XGroup parent( *column_line.get_ParentControl() );
				::rux::int32 index = parent.IndexOf( column_line );
				if( index > 0 )
				{
					float auto_width = 5.f;
					::rux::gui::controls::XGroup group_column( parent.get_ControlAt( index - 1 ) ) , group_cell;
					for( size_t index1 = 1 ; index1 < group_column.get_ControlsCount() ; index1++ )
					{
						group_cell.set_ByRef( group_column.get_ControlAt( index1 ) );
						::rux::XArray< ::rux::gui::controls::XTextBlock > textblocks;
						textblocks.set_ByRef( group_cell.Find< ::rux::gui::controls::XTextBlock >() );
						if( textblocks.Count() > 0 )
						{
							float content_width = textblocks[ 0 ].get_ContentWidth() + 5.f;
							if( content_width > auto_width )
								auto_width = content_width;
						}
					}
					group_column.set_Width( auto_width );
				}
			};
			void ListBox::on_column_line_mouse_move( const ::rux::gui::events::MouseEvent& event )
			{
				if( event._explicit_event )
				{
					if( event._window_event->_left_mouse_button_pressed )
					{
						::rux::gui::controls::XRectangle column_line( event._sender );
						ListBox* listbox = (ListBox*)column_line.get_TagPtr();
						if( listbox )
						{
							::rux::gui::controls::XGroup parent( *column_line.get_ParentControl() );
							::rux::int32 index = parent.IndexOf( column_line );
							if( index > 0 )
							{
								::rux::gui::controls::XGroup group_column( parent.get_ControlAt( index - 1 ) );
								float group_column_offset = (float)event._window_event->_x - listbox->_old_mouse_x;
								listbox->_old_mouse_x = event._window_event->_x;
								float group_column_width = group_column.get_Width();
								if( group_column_offset < 0.f )
								{
									if( abs( group_column_offset ) < group_column_width )
										group_column.set_Width( group_column_width + group_column_offset );
								}
								else
								{
									float column_line_width = column_line.get_Width();
									float group_column_left = group_column.get_Left();
									float parent_width = parent.get_Width();
									float parent_left = parent.get_Left();
									if( parent_left + parent_width > group_column_left + group_column_width + group_column_offset + column_line_width )
										group_column.set_Width( group_column_width + group_column_offset );
								}
							}
						}
					}
				}
			};
			void ListBox::on_column_line_left_mouse_button_down( const ::rux::gui::events::MouseEvent& event )
			{
				::rux::gui::controls::XRectangle column_line( event._sender );
				ListBox* listbox = (ListBox*)column_line.get_TagPtr();
				listbox->_old_mouse_x = event._window_event->_x;
			};
			void ListBox::UpdateColumnUI( const ::rux::XString& old_column_name , const ::rux::gui::controls::XColumnInfo& column_info )
			{
				::rux::byte found = 0;
				declare_variable( ::rux::gui::controls::XGroup , group_column );
				group_column.set_ByRef( _group_content.FindOne< ::rux::gui::controls::XGroup >( old_column_name , found ) );
				if( found )
				{
					group_column.set_ControlName( column_info()->_column_name );
					declare_variable( ::rux::XArray< ::rux::gui::controls::XButton > , button_columns );
					button_columns.set_ByRef( group_column.Find< ::rux::gui::controls::XButton >() );
					if( button_columns.Count() )
						button_columns[ 0 ].set_Content( column_info()->_column_name );
				}
			};
			void ListBox::AddColumnUI( const ::rux::gui::controls::XColumnInfo& column_info )
			{
				::rux::byte found = 0;
				declare_variable( ::rux::gui::controls::XGroup , group_column );
				group_column.set_ByRef( _group_content.FindOne< ::rux::gui::controls::XGroup >( column_info()->_column_name , found ) );
				if( found == 0 )
				{				
					group_column.set_ControlName( column_info()->_column_name );

					group_column.set_VerticalFilling( ::rux::gui::XEnum_Filling_Auto );
					group_column.set_HorizontalAlignment( ::rux::gui::XEnum_Alignment_LeftOrTop );
					group_column.set_HorizontalFilling( ::rux::gui::XEnum_Filling_Auto );

					::rux::gui::controls::XRectangle rectangle_column;
					rectangle_column.set_TagPtr( this );
					rectangle_column.set_Width( 2.f );
					rectangle_column.set_VerticalFilling( ::rux::gui::XEnum_Filling_Auto );
					rectangle_column.set_HorizontalAlignment( ::rux::gui::XEnum_Alignment_LeftOrTop );				
					rectangle_column.set_Background( create_color( 0x88 , 0x88 , 0x88 , (uint8)( (double)0xff * 0.4 ) ) );
					if( _group_content.get_ControlsCount() > 0 )
					{
						rectangle_column.set_Cursor( ::rux::gui::XEnum_Cursors_SizeWE );
						rectangle_column.set_OnMouseDoubleClick( ListBox::on_column_line_left_mouse_double_click );
						rectangle_column.set_OnMouseMove( ListBox::on_column_line_mouse_move );
						rectangle_column.set_OnLeftMouseButtonDown( ListBox::on_column_line_left_mouse_button_down );

						::rux::gui::controls::XGroup prev_group_column;					
						prev_group_column.set_ByRef( _group_content.get_ControlAt( _group_content.get_ControlsCount() - 1 ) );
						prev_group_column.set_HorizontalFilling( ::rux::gui::XEnum_Filling_Manual );
						prev_group_column.set_Width( 100 );					
					}
					group_column.set_ChildLocationType( XEnum_Child_Location_Type_VerticalStack );

					::rux::gui::controls::XButton button_column;
					button_column.set_Corner( ::rux::gui::Thickness( 0 , 0 , 0 , 0 ) );
					button_column.set_Height( _column_header_height );
					button_column.set_VerticalAlignment( ::rux::gui::XEnum_Alignment_LeftOrTop );
					button_column.set_HorizontalFilling( ::rux::gui::XEnum_Filling_Auto );
					button_column.set_Content( column_info()->_column_name );
				
					::rux::XString error;
					group_column.AddControl( button_column , error );				

					_group_content.AddControl( rectangle_column , error );
					_group_content.AddControl( group_column , error );
				}
			};
			implement_duplicate_internal_function_2( ListBox , UpdateColumn , size_t , const ::rux::XString& );
			void ListBox::UpdateColumn( size_t index , const ::rux::XString& new_column_name )
			{
				_cs_items.WriteLock( __FILE__ , __FUNCTION__ , __LINE__ , 0 );
				if( index < _columns.Count() )
				{
					declare_variable( ::rux::XString , old_column_name );
					old_column_name = _columns[ index ]._column_name;
					_columns[ index ]._column_name = new_column_name;					
					UpdateColumnUI( old_column_name , _columns[ index ] );
				}
				_cs_items.WriteUnlock();
			};
			implement_duplicate_internal_function_1( ListBox , AddColumn , const ::rux::XString& );
			void ListBox::AddColumn( const ::rux::XString& column_name )
			{
				::rux::byte found = 0;
				declare_variable( ::rux::gui::controls::XColumnInfo , column_info );
				_cs_items.WriteLock( __FILE__ , __FUNCTION__ , __LINE__ , 0 );
				for( size_t index0 = 0 ; index0 < _columns.Count() ; index0++ )
				{
					if( _columns[ index0 ]._column_name == column_name )
					{
						found = 1;
						column_info.set_ByRef( _columns[ index0 ] );
						break;
					}
				}
				if( found == 0 )
				{
					column_info.set_ColumnName( column_name );
					_columns.Add( column_info );
				}
				AddColumnUI( column_info );							
				_cs_items.WriteUnlock();
			};
			implement_duplicate_internal_function_1( ListBox , AddRow , const ::rux::gui::XRow& );
			void ListBox::AddRow( const ::rux::gui::XRow& row )
			{
				_rows.Add( row );
				float using_row_space_height = _column_header_height + _visible_rows_count * _item_height;
				if( using_row_space_height + _item_height  <= _group_content.get_Height() )
					InsertRowUI( _rows.Count() , row() );				
			};
			void ListBox::on_group_content_resized( const ::rux::gui::events::Event& event )
			{
				::rux::gui::controls::XGroup group_content( event._sender );
				ListBox* listbox = (ListBox*)group_content.get_TagPtr();
				listbox->UpdateRowsUI( XInterlocked::CompareExchange( &listbox->_visible_rows_index , 0 , 0 ) );
			};
			implement_duplicate_internal_result_function( size_t , ListBox , get_MaxVisibleItems );
			size_t ListBox::get_MaxVisibleItems( void )
			{
				return (size_t)( ( _group_content.get_Height() - _column_header_height ) / _item_height );
			};
			void ListBox::on_textbox_page_text_changed( const ::rux::gui::events::Event& event )
			{
				::rux::gui::controls::XTextBox textbox( event._sender );
				::rux::gui::controls::ListBox* listbox = (::rux::gui::controls::ListBox*)textbox.get_TagPtr();
				listbox->set_Page( (size_t)textbox.get_Text().ToInt64() - 1 );
			};
			implement_duplicate_internal_result_function( size_t , ListBox , get_Page );
			size_t ListBox::get_Page( void )
			{
				return _current_page;
			};
			implement_duplicate_internal_function_1( ListBox , set_Page , size_t );
			void ListBox::set_Page( size_t page )
			{	
				_rectangle_loader.set_IsVisible( 1 );
				_group_loader.set_IsVisible( 1 );
				_group_animation.Start();
				size_t visible_items = get_MaxVisibleItems();
				size_t count = _items_virtualization->Count();
				size_t pages = count / visible_items + ( ( count % visible_items ) > 0 ? 1 : 0 );
				if( page >= pages )
					page = pages - 1;
				UpdateRowsUI( page * visible_items );
			};
			void ListBox::on_button_click( const ::rux::gui::events::Event& event )
			{
				::rux::gui::controls::XButton button( event._sender );
				ListBox* listbox = (ListBox*)button.get_TagPtr();
				declare_variable( ::rux::XString , name );
				name.set_ByRef( button.get_ControlName() );
				if( name.Equals( "remove" , 6 ) )
				{
					if( listbox->_gui_selected_items.Count() > 0 )
					{
						for( size_t index0 = 0 ; index0 < listbox->_gui_selected_items.Count() ; index0++ )
						{
							if( listbox->_gui_selected_items[ index0 ].Count() > 0 )
							{
								declare_variable_param( ::rux::gui::XRow , row , listbox->_gui_selected_items[ index0 ].operator[]( 0 ).get_Tag() );
								declare_variable_param( XObject , item_object , row.get_ItemObject() );
								listbox->_on_item_removed.raise< const XObject& , const XObject& >( XObject( listbox , 1 ) , item_object );
							}
						}
						listbox->start_update_thread( XInterlocked::CompareExchange( &listbox->_visible_rows_index , 0 , 0 ) );
					}
				}
				else
				{
					::rux::XInt64 page( button.get_Tag() );
					listbox->set_Page( (size_t)page() - 1 );
				}
			};
			void ListBox::on_button_mouse_enter( const ::rux::gui::events::Event& event )
			{
				::rux::gui::controls::XButton button( event._sender );
				button.set_ContentOpacity( 1.f );
			};
			void ListBox::on_button_mouse_leave( const ::rux::gui::events::MouseEvent& event )
			{
				::rux::gui::controls::XButton button( event._sender );
				button.set_ContentOpacity( 0.7f );
			};
			void ListBox::update_thread( void* param , size_t ___rux__thread_index1986 )
			{
				UNREFERENCED_PARAMETER( ___rux__thread_index1986 );
				::rux::gui::controls::ListBox* listbox = (::rux::gui::controls::ListBox*)param;
				while( XInterlocked::Exchange( &listbox->_update_threads_count , 0 ) != 0 )
				{
					listbox->_cs_items.WriteLock( __FILE__ , __FUNCTION__ , __LINE__ , 0 );
					size_t visible_items = listbox->get_MaxVisibleItems();				
					if( listbox->_items_virtualization )
					{
						if( visible_items > 0 )
						{
							declare_variable( ::rux::XString , error );														
							size_t count = listbox->_items_virtualization->Count();
							size_t pages = count / visible_items + ( ( count % visible_items ) > 0 ? 1 : 0 );
							::rux::XArray< ::rux::gui::controls::XButton > buttons;
							buttons.set_ByRef( listbox->_group_pages.Find< ::rux::gui::controls::XButton >() );						
							if( listbox->_can_remove == 0 && pages <= 1 )
							{
								listbox->_group_pages.set_IsVisible( 0 );
								listbox->_textbox_pages_index.set_IsVisible( 0 );
								listbox->_textblock_pages_count.set_IsVisible( 0 );
								listbox->_textblock_pages.set_IsVisible( 0 );
							}
							else
							{
								listbox->_textbox_pages_index.set_IsVisible( 1 );
								listbox->_textblock_pages_count.set_IsVisible( 1 );
								listbox->_textblock_pages.set_IsVisible( 1 );
								size_t current_page = ( XInterlocked::CompareExchange( &listbox->_visible_rows_index , 0 , 0 ) + 1 ) / visible_items + ( ( ( listbox->_visible_rows_index + 1 ) % visible_items ) > 0 ? 1 : 0 );							
								listbox->_current_page = current_page - 1;
								listbox->_textbox_pages_index.set_Text( ::rux::XInt64( current_page ).ToString() );
								if( listbox->_on_language )
								{
									listbox->_textblock_pages.set_Text( listbox->_on_language(  "Page:" ) );
									listbox->_textblock_pages_count.set_Text( listbox->_on_language( "of" ) + " " + ::rux::XInt64( pages ).ToString() );
								}
								else
								{
									listbox->_textblock_pages.set_Text( "Page:" );
									listbox->_textblock_pages_count.set_Text( "of " + ::rux::XInt64( pages ).ToString() );
								}
								size_t begin_index0 = current_page;
								if( begin_index0 <= 5 )
									begin_index0 = 1;
								else
									begin_index0 -= 5;
								if( pages - begin_index0 < 10 )
								{
									if( pages > 10 )
										begin_index0 = pages - 10;
									else
										begin_index0 = 1;
								}
								for( size_t index0 = begin_index0 ; index0 <= pages ; index0++ )
								{
									if( index0 - begin_index0 > 10 )
										break;
									::rux::gui::controls::XButton button;
									if( buttons.Count() > 0 )
										button.set_ByRef( buttons[ 0 ] );
									button.set_TagPtr( listbox );
									button.set_Font( "tahoma.ttf" , 0 , 14 );
									button.set_Content( ::rux::XInt64( index0 ).ToString() );
									if( index0 == current_page )
										button.set_Background( ::rux::gui::Colors::ButtonDownColor() );
									else
									{
										button.set_Background( ::rux::gui::Colors::Transparent() );
										button.set_ContentOpacity( 0.7f );
									}
									button.set_OverBackground( ::rux::gui::Colors::Transparent() );
									button.set_Tag( ::rux::XInt64( index0 ) );
									button.set_OnClick( ListBox::on_button_click );
									button.set_OnMouseEnter( ListBox::on_button_mouse_enter );
									button.set_OnMouseLeave( ListBox::on_button_mouse_leave );
									button.set_Margin( ::rux::gui::Margin( 2 , 2 , 0 , 0 ) );
									button.set_HorizontalFilling( ::rux::gui::XEnum_Filling_FromContent );
									button.set_Height( 15 );
									button.set_HorizontalAlignment( ::rux::gui::XEnum_Alignment_LeftOrTop );
									button.set_VerticalAlignment( ::rux::gui::XEnum_Alignment_Center );
									if( buttons.Count() > 0 )
										buttons.RemoveAt( 0 );
									else
										listbox->_group_pages.AddControl( button , error );
								}							
								listbox->_group_pages.set_IsVisible( 1 );
							}
							for( size_t index0 = 0 ; index0 < buttons.Count() ; index0++ )
								buttons[ index0 ].RemoveControl();
							listbox->_rows.set_ByRef( listbox->_items_virtualization->Get( XInterlocked::CompareExchange( &listbox->_visible_rows_index , 0 , 0 ) , visible_items ) );
							listbox->ClearRowsUI();
							listbox->AddRowsUI();
						}
					}
					else					
					{
						listbox->ClearRowsUI();
						listbox->AddRowsUI();	
					}
					listbox->_cs_items.WriteUnlock();
					listbox->_group_loader.set_IsVisible( 0 );
					listbox->_rectangle_loader.set_IsVisible( 0 );
					listbox->_group_animation.Stop();
				}
				::rux::threading::XThread thread( ::rux::threading::XThread::get_CurrentThread() );
				thread.Stop();
			};
			void ListBox::start_update_thread( size_t visible_rows_index )
			{
				_rectangle_loader.set_IsVisible( 1 );
				_group_loader.set_IsVisible( 1 );
				_group_animation.Start();
				XInterlocked::Exchange( &_visible_rows_index , visible_rows_index );
				if( XInterlocked::Increment( &_update_threads_count ) == 1 )
				{
					::rux::threading::XThread thread;
					thread.set_ThreadMode( XThreadMode_OneTime );
					thread.set_ThreadName( "listbox update" );
					thread.Start();
					thread.Push( ListBox::update_thread , this );
				}
			};
			void ListBox::UpdateRowsUI( size_t visible_rows_index )
			{
				size_t old_visible_items = 0;
				if( _columns.Count() > 1 )
				{
					::rux::gui::controls::XGroup group_column;
					group_column.set_ByRef( _group_content.get_ControlAt( 1 ) );
					old_visible_items = group_column.get_ControlsCount() - 1;
				}
				size_t visible_items = get_MaxVisibleItems();
				if( XInterlocked::CompareExchange( &_visible_rows_index , 0 , 0 ) != visible_rows_index || old_visible_items != visible_items )
					start_update_thread( visible_rows_index );
				else
				{
					_group_loader.set_IsVisible( 0 );
					_rectangle_loader.set_IsVisible( 0 );
					_group_animation.Stop();
				}
			};
			void ListBox::AddRowsUI( void )
			{
				_cs_items.WriteLock( __FILE__ , __FUNCTION__ , __LINE__ , 0 );
				declare_variable( ::rux::gui::controls::XGroup , group_column );
				for( size_t index0 = 0 ; index0 < _columns.Count() ; index0++ )
				{
					group_column.set_ByRef( _group_content.get_ControlAt( 2 * index0 + 1 ) );
					group_column.BeginMultipleAddOrRemoveControls();
				}
				size_t visible_rows_index = XInterlocked::CompareExchange( &_visible_rows_index , 0 , 0 );
				if( visible_rows_index >= _rows.Count() )
					visible_rows_index = 0;
				for( size_t index0 = visible_rows_index ; index0 < _rows.Count() ; index0++ )
				{
					float using_row_space_height = _column_header_height + _visible_rows_count * _item_height;
					if( using_row_space_height + _item_height  <= _group_content.get_Height() )
						InsertRowUI( _rows.Count() , &_rows[ index0 ] );	
					else
						break;
				}
				for( size_t index0 = 0 ; index0 < _columns.Count() ; index0++ )
				{
					group_column.set_ByRef( _group_content.get_ControlAt( 2 * index0 + 1 ) );
					group_column.EndMultipleAddOrRemoveControls();
				}
				_cs_items.WriteUnlock();
			};
			void ListBox::check_button_remove( void )
			{
				WRITE_LOCK( _cs_items );
				::rux::byte found = 0;
				declare_variable( ::rux::gui::controls::XButton , button_remove );
				button_remove.set_ByRef( _group_pages.FindOne< ::rux::gui::controls::XButton >( "remove" , found ) );
				if( _can_remove && _gui_selected_items.Count() > 0 )
				{
					if( found == 0 )
					{
						declare_variable( ::rux::gui::controls::XButton , button_remove );
						button_remove.set_ControlName( "remove" );
						button_remove.set_HorizontalAlignment( ::rux::gui::XEnum_Alignment_LeftOrTop );
						button_remove.set_Background( ::rux::gui::Colors::Transparent() );
						button_remove.set_OverBackground( ::rux::gui::Colors::Transparent() );
						button_remove.set_Corner( ::rux::gui::Thickness( 0 , 0 , 0 , 0 ) );	
						button_remove.set_OnMouseEnter( on_button_mouse_enter );
						button_remove.set_OnMouseLeave( on_button_mouse_leave );	
						button_remove.set_VerticalAlignment( ::rux::gui::XEnum_Alignment_Center );
						button_remove.set_Width( 32 );
						button_remove.set_Height( 32 );
						button_remove.set_OnClick( on_button_click );
						button_remove.set_TagPtr( this );
						button_remove.set_Margin( ::rux::gui::Margin( 20 , 0 , 0 , 0 ) );
						declare_variable( ::rux::XString , error );
						if( ::rux::media::XFrame::Exists( "trash32x32.png" ) == 1 )
						{
							::rux::media::XFrame frame;
							frame.set_ImageData( "trash32x32.png" , error );
							::rux::gui::controls::XImage img;
							img.set_HorizontalAlignment( ::rux::gui::XEnum_Alignment_Center );
							img.set_VerticalAlignment( ::rux::gui::XEnum_Alignment_Center );
							img.set_HorizontalFilling( ::rux::gui::XEnum_Filling_Auto );
							img.set_VerticalFilling( ::rux::gui::XEnum_Filling_Auto );
							img.set_IsAlpha( 1 );
							img.set_Frame( frame );
							::rux::gui::controls::XGroup group;
							group.set_Opacity( .7f );
							group.AddControl( img , error );
							button_remove.set_Content( group );
						}
						else
						{
							if( _on_language )
								button_remove.set_Content( _on_language( "Remove" ) );
							else
								button_remove.set_Content( ::rux::XString( "Remove" ) );
						}
						_group_pages.AddControl( button_remove , error );
					}
					_group_pages.set_IsVisible( 1 );
				}
				else
				{
					if( found )
						button_remove.RemoveControl();
					if( _textblock_pages.get_IsVisible() == 0 )
						_group_pages.set_IsVisible( 0 );
				}
				_cs_items.WriteUnlock();
			};
			void ListBox::ClearRowsUI( void )
			{
				declare_variable( ::rux::XArray< ::rux::gui::controls::XGroup > , controls );
				_cs_items.WriteLock( __FILE__ , __FUNCTION__ , __LINE__ , 0 );
				declare_variable( ::rux::gui::controls::XGroup , group_column );
				for( size_t index0 = 0 ; index0 < _columns.Count() ; index0++ )
				{
					group_column.set_ByRef( _group_content.get_ControlAt( 2 * index0 + 1 ) );
					group_column.BeginMultipleAddOrRemoveControls();
				}
				for( size_t index0 = 0 ; index0 < _gui_items.Count() ; index0++ )
				{
					controls.set_ByRef( _gui_items[ index0 ] );
					if( controls.Count() > 0 )
					{
						for( size_t index1 = controls.Count() - 1 ; ; index1-- )
						{
							controls[ index1 ].RemoveControl();
							if( index1 == 0 )
								break;
						}
					}
				}
				_gui_selected_items.Clear();
				_gui_items.Clear();
				_visible_rows_count = 0;
				check_button_remove();
				for( size_t index0 = 0 ; index0 < _columns.Count() ; index0++ )
				{
					group_column.set_ByRef( _group_content.get_ControlAt( 2 * index0 + 1 ) );
					group_column.EndMultipleAddOrRemoveControls();
				}
				_cs_items.WriteUnlock();
			};
			implement_duplicate_internal_function_1( ListBox , set_ItemsVirtualization , ::rux::gui::ItemsVirtualization* );
			void ListBox::set_ItemsVirtualization( ::rux::gui::ItemsVirtualization* items_virtualization )
			{
				if( _items_virtualization )
				{
					_items_virtualization->Release();
					ClearRowsUI();
					_rows.Clear();
				}
				_items_virtualization = items_virtualization;
				if( _items_virtualization )
				{
					_items_virtualization->AddRef();
					UpdateRowsUI( 0 );
				}
			};
			void ListBox::on_group_cell_left_mouse_button_down( const ::rux::gui::events::MouseEvent& event )
			{
				if( event._explicit_event )
				{
					declare_variable_param( ::rux::gui::controls::XGroup , group_cell , event._sender );
					declare_variable_param( ::rux::gui::XRow , row , group_cell.get_Tag() );
					::rux::XArray< ::rux::gui::controls::XGroup > group_cells;
					group_cells.set_ByRef( row.get_Tag() );
					ListBox* listbox = (ListBox*)group_cell.get_TagPtr();
					if( event._window_event->_control )
					{
						::rux::byte found = 0;
						for( size_t index0 = 0 ; index0 < listbox->_gui_selected_items.Count() ; index0++ )
						{
							if( &listbox->_gui_selected_items[ index0 ] == group_cells() )
							{
								found = 1;
								for( size_t index1 = 0 ; index1 < listbox->_gui_selected_items[ index0 ].Count() ; index1++ )
									listbox->_gui_selected_items[ index0 ].operator[]( index1 ).set_Background( ::rux::gui::Colors::Transparent() );
								listbox->_gui_selected_items.RemoveAt( index0 );
								break;
							}
						}
						if( found == 0 )
						{
							for( size_t index0 = 0 ; index0 < group_cells.Count() ; index0++ )
								group_cells[ index0 ].set_Background( create_color( 255 , 255 , 255 , 0x66 ) );
							listbox->_gui_selected_items.AddByRef( group_cells );
						}
					}
					else if( event._window_event->_shift )
					{
						if( listbox->_gui_selected_items.Count() == 0 )
						{
							WRITE_LOCK( listbox->_cs_items );
							for( size_t index0 = 1 ; index0 < listbox->_gui_items.Count() ; index0++ )
							{
								for( size_t index1 = 0 ; index1 < listbox->_gui_items[ index0 ].Count() ; index1++ )
									listbox->_gui_items[ index0 ].operator[]( index1 ).set_Background( create_color( 255 , 255 , 255 , 0x66 ) );
								listbox->_gui_selected_items.AddByRef( listbox->_gui_items[ index0 ] );
								if( &listbox->_gui_items[ index0 ] == group_cells() )
									break;				
							}
							for( size_t index1 = 0 ; index1 < listbox->_gui_items[ 0 ].Count() ; index1++ )
								listbox->_gui_items[ 0 ].operator[]( index1 ).set_Background( create_color( 255 , 255 , 255 , 0x66 ) );
							listbox->_gui_selected_items.AddByRef( listbox->_gui_items[ 0 ] );
							listbox->_cs_items.WriteUnlock();
						}
						else
						{
							size_t group_cells_index = SIZE_MAX , first_cells_index = SIZE_MAX;
							WRITE_LOCK( listbox->_cs_items );
							for( size_t index0 = 0 ; index0 < listbox->_gui_items.Count() ; index0++ )
							{
								if( &listbox->_gui_items[ index0 ] == group_cells() )
									group_cells_index = index0;
								if( &listbox->_gui_selected_items[ listbox->_gui_selected_items.Count() - 1 ] == &listbox->_gui_items[ index0 ] )
									first_cells_index = index0;
								if( group_cells_index != SIZE_MAX && first_cells_index != SIZE_MAX )
									break;
							}
							for( size_t index0 = 0 ; index0 < listbox->_gui_selected_items.Count() ; index0++ )
							{
								for( size_t index1 = 0 ; index1 < listbox->_gui_selected_items[ index0 ].Count() ; index1++ )
									listbox->_gui_selected_items[ index0 ].operator[]( index1 ).set_Background( ::rux::gui::Colors::Transparent() );
							}
							listbox->_gui_selected_items.Clear();
							if( group_cells_index != SIZE_MAX && first_cells_index != SIZE_MAX )
							{
								if( group_cells_index > first_cells_index )
								{
									for( size_t index0 = first_cells_index + 1 ; index0 <= group_cells_index ; index0++ )
									{
										for( size_t index1 = 0 ; index1 < listbox->_gui_items[ index0 ].Count() ; index1++ )
											listbox->_gui_items[ index0 ].operator[]( index1 ).set_Background( create_color( 255 , 255 , 255 , 0x66 ) );
										listbox->_gui_selected_items.AddByRef( listbox->_gui_items[ index0 ] );
									}
									for( size_t index1 = 0 ; index1 < listbox->_gui_items[ first_cells_index ].Count() ; index1++ )
										listbox->_gui_items[ first_cells_index ].operator[]( index1 ).set_Background( create_color( 255 , 255 , 255 , 0x66 ) );
									listbox->_gui_selected_items.AddByRef( listbox->_gui_items[ first_cells_index ] );
								}
								else
								{
									for( size_t index0 = group_cells_index + 1 ; index0 <= first_cells_index ; index0++ )
									{
										for( size_t index1 = 0 ; index1 < listbox->_gui_items[ index0 ].Count() ; index1++ )
											listbox->_gui_items[ index0 ].operator[]( index1 ).set_Background( create_color( 255 , 255 , 255 , 0x66 ) );
										listbox->_gui_selected_items.AddByRef( listbox->_gui_items[ index0 ] );
									}
									for( size_t index1 = 0 ; index1 < listbox->_gui_items[ group_cells_index ].Count() ; index1++ )
										listbox->_gui_items[ group_cells_index ].operator[]( index1 ).set_Background( create_color( 255 , 255 , 255 , 0x66 ) );
									listbox->_gui_selected_items.AddByRef( listbox->_gui_items[ group_cells_index ] );
								}
							}
							listbox->_cs_items.WriteUnlock();
						}
					}
					else
					{
						for( size_t index0 = 0 ; index0 < listbox->_gui_selected_items.Count() ; index0++ )
						{
							for( size_t index1 = 0 ; index1 < listbox->_gui_selected_items[ index0 ].Count() ; index1++ )
								listbox->_gui_selected_items[ index0 ].operator[]( index1 ).set_Background( ::rux::gui::Colors::Transparent() );
						}
						listbox->_gui_selected_items.Clear();
						for( size_t index0 = 0 ; index0 < group_cells.Count() ; index0++ )
							group_cells[ index0 ].set_Background( create_color( 255 , 255 , 255 , 0x66 ) );
						listbox->_gui_selected_items.AddByRef( group_cells );
					}
					listbox->check_button_remove();
				}
			};
			void ListBox::InsertRowUI( size_t insert_index , const ::rux::gui::XRow& row )
			{
				InsertRowUI( insert_index , row() );
			};
			void ListBox::InsertRowUI( size_t insert_index , ::rux::gui::Row* row )
			{
				_cs_items.WriteLock( __FILE__ , __FUNCTION__ , __LINE__ , 0 );
				if( insert_index == 0 )
					insert_index++;
				::rux::XArray< ::rux::gui::controls::XGroup > group_cells;
				for( size_t index0 = 0 ; index0 < _columns.Count() ; index0++ )
				{
					::rux::gui::controls::XGroup group_column , group_cell;
					group_column.set_ByRef( _group_content.get_ControlAt( 2 * index0 + 1 ) );

					group_cell.set_TagPtr( this );
					group_cell.set_Tag( XObject( row , 1 ) );
					group_cell.set_OnMouseDoubleClick( on_item_left_mouse_double_click );
					group_cell.set_OnLeftMouseButtonDown( on_group_cell_left_mouse_button_down );
					group_cell.set_Height( _item_height );
					group_cell.set_ChildLocationType( XEnum_Child_Location_Type_VerticalStack );
					group_cell.set_VerticalAlignment( ::rux::gui::XEnum_Alignment_LeftOrTop );
					group_cell.set_HorizontalFilling( ::rux::gui::XEnum_Filling_Auto );

					::rux::XString error;
					::rux::gui::controls::XRectangle rectangle_top;	
					rectangle_top.set_Background( create_color( 0x88 , 0x88 , 0x88 , (uint8)( (double)0xff * 0.4 ) ) );
					rectangle_top.set_HorizontalFilling( ::rux::gui::XEnum_Filling_Auto );
					rectangle_top.set_Height( 1.f );
					rectangle_top.set_VerticalAlignment( ::rux::gui::XEnum_Alignment_LeftOrTop );
					group_cell.AddControl( rectangle_top , error );

					::rux::gui::controls::XRectangle rectangle_bottom;		
					rectangle_bottom.set_Background( create_color( 0x88 , 0x88 , 0x88 , (uint8)( (double)0xff * 0.4 ) ) );
					rectangle_bottom.set_HorizontalFilling( ::rux::gui::XEnum_Filling_Auto );
					rectangle_bottom.set_Height( 1.f );
					rectangle_bottom.set_VerticalAlignment( ::rux::gui::XEnum_Alignment_RightOrBottom );
					group_cell.AddControl( rectangle_bottom , error );

					::rux::XString column_name;
					column_name.set_ByRef( group_column.get_ControlName() );
					::rux::uint8 found = 0;
					XRowCell cell;
					cell.set_ByRef( row->FindCell( column_name , found ) );
					if( found )
					{
						if( rux_is_object( cell()->_column_value , ::rux::XString ) )
						{
							::rux::XString value , error;
							value.set_ByRef( cell()->_column_value );

							::rux::gui::controls::XTextBlock textblock_cell;
							textblock_cell.set_IsForwardEvents( 1 );
							textblock_cell.set_VerticalAlignment( ::rux::gui::XEnum_Alignment_LeftOrTop );
							textblock_cell.set_HorizontalFilling( ::rux::gui::XEnum_Filling_Auto );
							textblock_cell.set_VerticalFilling( ::rux::gui::XEnum_Filling_Auto );
							textblock_cell.set_Text( value );

							group_cell.AddControl( textblock_cell , error );
						}
					}
					group_cells.AddByRef( group_cell );
					group_column.InsertControl( insert_index , group_cell , error ); 
				}
				row->set_Tag( group_cells );
				_gui_items.AddByRef( group_cells );
				_visible_rows_count++;
				_cs_items.WriteUnlock();
			};
		};
	};
};
