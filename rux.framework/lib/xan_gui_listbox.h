#ifndef XAN_GUI_LISTBOX_H
#define XAN_GUI_LISTBOX_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_string.h>
#include <xan_ui_enums.h>
#include <xan_usercontrol.h>
#include <xan_items_virtualization.h>
#include <xan_uitextblock.h>
#include <xan_uitextbox.h>
#include <xan_uigroupanimation.h>
#include <xan_uirectangle.h>
#include <xan_uibutton.h>
namespace rux
{
	namespace gui
	{
		namespace controls
		{
			class ListBox;
			begin_declare_rux_class( ColumnInfo );
				friend class ListBox;
				void set_ColumnName( const ::rux::XString& column_name );
			begin_declare_rux_class_members( ColumnInfo );
				::rux::XString _column_name;
			end_declare_rux_class_members( ColumnInfo );
				void set_ColumnName( const ::rux::XString& column_name );
			end_declare_rux_class();

			begin_declare_rux_gui_user_control( ListBox );
				~ListBox( void );
				void set_Page( size_t page );
				size_t get_Page( void );
				void AddColumn( const ::rux::XString& column_name );
				void UpdateColumn( size_t index , const ::rux::XString& new_column_name );
				void AddRow( const ::rux::gui::XRow& row );
				size_t get_MaxVisibleItems( void );
				void set_ItemsVirtualization( ::rux::gui::ItemsVirtualization* items_virtualization );
				static void on_column_line_mouse_move( const ::rux::gui::events::MouseEvent& event );
				static void on_column_line_left_mouse_button_down( const ::rux::gui::events::MouseEvent& event );
				static void on_column_line_left_mouse_double_click( const ::rux::gui::events::MouseEvent& event );
				static void on_item_left_mouse_double_click( const ::rux::gui::events::MouseEvent& event );
				static void on_group_cell_left_mouse_button_down( const ::rux::gui::events::MouseEvent& event );
				static void on_group_content_resized( const ::rux::gui::events::Event& sender );
				static void on_group_content_mouse_wheel( const ::rux::gui::events::MouseEvent& event );
				static void on_button_mouse_enter( const ::rux::gui::events::Event& sender );
				static void on_textbox_page_text_changed( const ::rux::gui::events::Event& sender );
				static void on_button_mouse_leave( const ::rux::gui::events::MouseEvent& event );
				static void on_button_click( const ::rux::gui::events::Event& sender );
				static void update_thread( void* param , size_t ___rux__thread_index1986 );
				void start_update_thread( size_t visible_rows_index );
				void set_OnItemRemoved( ::rux::gui::events::on_object_event_t on_item_removed );
				void set_OnLanguage( ::rux::gui::events::on_language_t on_language );
				void set_OnItemLeftMouseButtonDoubleClick( ::rux::gui::events::on_object_event_t on_item_left_mouse_button_double_click );
				void set_CanRemove( ::rux::byte can_remove );
			begin_declare_rux_gui_user_control_members( ListBox );
				size_t _current_page;
				::rux::gui::controls::XRectangle _rectangle_loader;
				::rux::gui::controls::XGroup _group_loader;
				::rux::gui::XGroupAnimation _group_animation;
				rux_volatile _update_threads_count;
				::rux::threading::RdWrLock _cs_items;
				::rux::gui::ItemsVirtualization* _items_virtualization;
				::rux::gui::controls::XGroup _group_all;
				::rux::gui::controls::XTextBlock _textblock_pages;
				::rux::gui::controls::XTextBlock _textblock_pages_count;
				::rux::gui::controls::XTextBox _textbox_pages_index;
				::rux::gui::controls::XGroup _group_pages;
				::rux::gui::controls::XGroup _group_additional;
				::rux::gui::controls::XGroup _group_content;
				::rux::XArray< ::rux::gui::controls::XColumnInfo > _columns;
				::rux::XArray< ::rux::gui::XRow > _rows;
				::rux::XArray< ::rux::XArray< ::rux::gui::controls::XGroup > > _gui_items;
				::rux::XArray< ::rux::XArray< ::rux::gui::controls::XGroup > > _gui_selected_items;
				float _item_height;
				float _column_header_height;
				float _old_mouse_x;
				size_t _visible_rows_count;
				rux_volatile _visible_rows_index;
				::rux::byte _can_remove;
				::rux::EventHandler< ::rux::gui::events::on_object_event_t > _on_item_removed;
				::rux::EventHandler< ::rux::gui::events::on_object_event_t > _on_item_left_mouse_button_double_click;
				::rux::gui::events::on_language_t _on_language;
				void AddColumnUI( const ::rux::gui::controls::XColumnInfo& column );
				void UpdateColumnUI( const ::rux::XString& old_column_name , const ::rux::gui::controls::XColumnInfo& column );
				void InsertRowUI( size_t insert_index , const ::rux::gui::XRow& row );
				void InsertRowUI( size_t insert_index , ::rux::gui::Row* row );
				void ClearRowsUI( void );
				void AddRowsUI( void );
				void UpdateRowsUI( size_t visible_rows_index );
				void check_button_remove( void );
				void AddAdditionalControl( const XObject& control , ::rux::XString& error );
			end_declare_rux_gui_user_control_members( ListBox );
				void set_Page( size_t page );
				size_t get_MaxVisibleItems( void );
				void AddColumn( const ::rux::XString& column_name );
				void UpdateColumn( size_t index , const ::rux::XString& new_column_name );
				void AddRow( const ::rux::gui::XRow& row );
				void set_ItemsVirtualization( ::rux::gui::ItemsVirtualization* items_virtualization );
				void set_OnItemRemoved( ::rux::gui::events::on_object_event_t on_item_removed );
				void set_OnLanguage( ::rux::gui::events::on_language_t on_language );
				void set_OnItemLeftMouseButtonDoubleClick( ::rux::gui::events::on_object_event_t on_item_left_mouse_button_double_click );
				void set_CanRemove( ::rux::byte can_remove );
				void AddAdditionalControl( const XObject& control , ::rux::XString& error );
				size_t get_Page( void );
			end_declare_rux_gui_user_control();	
		};
	};
};
#endif