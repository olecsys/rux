#ifndef XAN_UISELECT_H
#define XAN_UISELECT_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_ui_enums.h>
#include <xan_uigroup.h>
#include <xan_uigroupanimation.h>
#include <xan_jsonserializer.h>
namespace rux
{
	namespace gui
	{
		namespace controls
		{
			class Select : public ::rux::gui::ControlBase
			{
				declare_rux_base_internal_methods();
				friend class XSelect;
			private:
				DECLARE_BASE_UI_MEMBERS();
				DECLARE_RUX_BASE_UI_FUNCTIONS( Select );

				::rux::gui::ColorBase* _item_background;
				::rux::gui::ColorBase* _item_over_background;
				::rux::gui::ColorBase* _item_pressed_background;
				::rux::gui::Color* _item_foreground;
				::rux::gui::Color* _item_over_foreground;
				::rux::gui::Color* _item_pressed_foreground;
				::rux::gui::ColorBase* _selected_item_background;
				::rux::gui::ColorBase* _selected_item_over_background;

				rux::gui::RenderCacheBase* _ui_border_cache;
				rux::gui::RenderCacheBase* _ui_cache;
				rux::gui::ColorBase* _combobox_background;
				rux::gui::Color* _combobox_border;
				rux::gui::Thickness _combobox_thickness;	
				rux::gui::ColorBase* _content_background;
				rux::gui::ColorBase* _content_over_background;
				rux::gui::ColorBase* _content_active_background;
				float _items_container_height;
				float _items_container_width;
				XGroup _items_container;
				rux::gui::ColorBase* _items_container_background;
				rux::gui::Color* _items_container_border;
				rux::gui::Thickness _items_container_thickness;	
				float _items_container_border_width;	
				XGroupAnimation _items_container_group_animation;
				::rux::uint32 _selected_index;
				rux::XString _selected_text;
				rux::gui::Color* _selected_text_foreground;
				rux::gui::RenderCacheBase* _selected_text_cache;
				rux::gui::FontBase* _selected_text_font;
				::rux::uint8 _is_editable;
				XObject _binding_source;
				rux::XString _binding_source_path;
				rux::XString _binding_source_display_property_name;
				XArray< XObject > _items;
				::rux::XArray< XObject > _items_tags;
				XCrtSect _cs_items;
				::rux::EventHandler< ::rux::gui::events::on_event_t > _on_item_selected;
	
				size_t _max_visible_items;
				size_t _start_visible_items;
				float _item_height;
				float _border_width;
				::rux::gui::XEnum_Alignment _horizontal_text_alignment;	
				::rux::gui::XEnum_Alignment _vertical_text_alignment;
				float _text_left_margin;
				float _text_top_margin;
				rux_volatile _reset_cache;
				::rux::byte _text_need_refresh;
				float _text_left;
				float _text_top;
				float _text_width;
				float _text_height;
				float _render_text_width;
				float _render_text_height;
			public:
				explicit Select( const char* variable_name = 0 , const char* __file__ = 0 , ::rux::int32 __line__ = 0 );
				~Select();
				declare_content_size();
			private:
				void private_set_ActiveContentBackground( ::rux::gui::ColorBase* background );
				static void on_completed_animation( const ::rux::gui::events::Event& sender );
				static void on_item_button_click( const ::rux::gui::events::Event& sender );
				static void on_items_container_mouse_wheel( const ::rux::gui::events::MouseEvent& event );
			public:
				rux::uint32 get_SelectedIndex( void );
				void set_SelectedIndex( ::rux::uint32 index , ::rux::byte raise_event = 1 );
				void ItemsClear( void );
				void ItemsAdd( const XObject& item , ::rux::uint8 selected = 0 );
				void ItemsRemove(size_t index);
				size_t ItemsCount( void );
				void set_ItemTag( size_t index , const XObject& tag );
				Object& get_ItemTag( size_t index );
				void set_BindingSource( const XObject& binding_source );
				void set_BindingSourcePath( const ::rux::XString& binding_source_path );
				void set_BindingSourceDisplayPropertyName( const ::rux::XString& binding_source_display_property_name );
				void ItemsRefresh( void );
				void set_OnItemSelected( ::rux::gui::events::on_event_t on_item_selected_callback );	
				void create( ::rux::uint8 show );
				void destroy( void );
				void update_main_text( void );
				Object& get_SelectedItem( void );
				void set_MaxVisibleItems( size_t max_visible_items );
				void set_StartVisibleIndex( size_t start_visible_index );
				
				void set_ItemForeground(::rux::gui::Color* foreground);
				void set_ItemOverForeground(::rux::gui::Color* foreground);
				void set_ItemPressedForeground(::rux::gui::Color* foreground);
				void set_ItemBackground(::rux::gui::ColorBase* background);
				void set_ItemOverBackground(::rux::gui::ColorBase* background);
				void set_ItemPressedBackground(::rux::gui::ColorBase* background);

				void set_SelectedItemBackground(::rux::gui::ColorBase* background);
				void set_SelectedItemOverBackground(::rux::gui::ColorBase* background);

				void set_Foreground( ::rux::gui::ColorBase* foreground );
				void set_TextVerticalAlignment( ::rux::gui::XEnum_Alignment text_alignment );
				void set_TextHorizontalAlignment( ::rux::gui::XEnum_Alignment text_alignment );
				void set_Background( ::rux::gui::ColorBase* background );
				float get_TextWidth( void );
				float get_TextHeight( void );
				void set_TextMargin( float left , float top );
				::rux::gui::ColorBase* get_Foreground( void );
				void set_BorderColor( ::rux::gui::ColorBase* border_color );
				void set_BorderWidth( float border_width );
				void set_Font( const char* font_file_name ,
					::rux::uint32 font_size_height ,
					::rux::uint32 font_size_width );
				void set_ItemContainerBackground(::rux::gui::ColorBase* background);
			};
			class XSelect : public XGCHandle<Select>
			{
				declare_rux_base_methods(Select);
			private:
			public:
				declare_rux_set_operators( Select );
				XSelect();
				XSelect( const Select& uiselect );
				XSelect( const XSelect& uiselect );
				explicit XSelect( const XObject& object );
				explicit XSelect( XGCRef* object );
				void ItemsClear( void );
				void ItemsAdd( const XObject& item , ::rux::uint8 selected = 0 );
				void ItemsRemove(size_t index);
				size_t ItemsCount( void );
				void set_SelectedIndex( ::rux::uint32 index , ::rux::byte raise_event = 1 );
				void set_BindingSource( const XObject& binding_source );
				void set_BindingSourcePath( const ::rux::XString& binding_source_path );
				void set_BindingSourceDisplayPropertyName( const ::rux::XString& binding_source_display_property_name );
				void set_OnItemSelected( ::rux::gui::events::on_event_t on_item_selected_callback );
				void set_ItemsContainerWidth( float items_container_width );
				Object& get_SelectedItem( void );
				void set_MaxVisibleItems( size_t max_visible_items );
				void set_StartVisibleIndex( size_t start_visible_index );
				rux::uint32 get_SelectedIndex( void );
				void set_ItemTag( size_t index , const XObject& tag );
				Object& get_ItemTag( size_t index );
				
				void set_Foreground( ::rux::gui::ColorBase* foreground );
				void set_TextVerticalAlignment( ::rux::gui::XEnum_Alignment text_alignment );
				void set_TextHorizontalAlignment( ::rux::gui::XEnum_Alignment text_alignment );
				void set_Background( ::rux::gui::ColorBase* background );
				float get_TextWidth( void );
				float get_TextHeight( void );
				void set_TextMargin( float left , float top );
				::rux::gui::ColorBase* get_Foreground( void );
				void set_BorderColor( ::rux::gui::ColorBase* border_color );
				void set_BorderWidth( float border_width );
				void set_Font( const char* font_file_name ,
					::rux::uint32 font_size_height ,
					::rux::uint32 font_size_width );
				void set_ItemContainerBackground(::rux::gui::ColorBase* background);
				void set_ItemForeground(::rux::gui::Color* foreground);
				void set_ItemOverForeground(::rux::gui::Color* foreground);
				void set_ItemPressedForeground(::rux::gui::Color* foreground);
				void set_ItemBackground(::rux::gui::ColorBase* background);
				void set_ItemOverBackground(::rux::gui::ColorBase* background);
				void set_ItemPressedBackground(::rux::gui::ColorBase* background);
				void set_SelectedItemBackground(::rux::gui::ColorBase* background);
				void set_SelectedItemOverBackground(::rux::gui::ColorBase* background);
				DECLARE_BASE_UI_FUNCTIONS();
			public:
			};
		};
	};
};
#endif
