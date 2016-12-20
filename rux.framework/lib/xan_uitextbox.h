#ifndef XAN_UITEXTBOX_H
#define XAN_UITEXTBOX_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_ui_enums.h>
#include <xan_window.h>
#include <xan_regexp.h>
#include <xan_math.h>
namespace rux
{
	namespace gui
	{
		namespace controls
		{
			class TextBox : public ::rux::gui::ControlBase
			{
				declare_rux_base_internal_methods();
				friend class XTextBox;
				friend class TimePicker;
			private:
				DECLARE_BASE_UI_MEMBERS();
				float _border_thickness;
				rux::gui::FontBase* _font;	
				XArray< ::rux::XString > _lines;		
				rux::gui::Color* _foreground;
				rux::gui::Color* _background;
				rux::gui::Color* _border_color;	
				rux::gui::ColorBase* _selection_background;
				rux::gui::RenderCacheBase* _ui_border_cache;
				rux::gui::RenderCacheBase* _ui_cache;	
				XMallocArray< ::rux::gui::RenderCacheBase* > _text_caches;	
				XMallocArray< ::rux::gui::RenderCacheBase* > _mask_caches;	
				size_t _selection_char_index;
				::rux::uint32 _old_selection_char_index;
				float _scrollbar_left_offset;
				float _scrollbar_top_offset;	
				XArray< ::rux::XRectangleF > _selection_rects;
				XMallocArray< ::rux::gui::RenderCacheBase* > _selection_rects_caches;
				::rux::uint8 _is_multi_lines;
				DECLARE_RUX_BASE_UI_FUNCTIONS( TextBox );	
				void calculate_caret_and_selection( void );
				size_t get_line( size_t char_index , size_t& char_index_in_line );
				size_t get_first_char_index( size_t line_index );
				rux::XRegexpPattern _regexp_pattern;
				::rux::uint8 _is_regexp_pattern;
				rux::XRegexpPattern _left_mouse_down_selection_regexp_pattern;
				::rux::uint8 _is_left_mouse_down_selection_regexp_pattern;
				::rux::EventHandler< ::rux::gui::events::on_event_t > _on_text_changed_event_callback;
				::rux::gui::XEnum_Alignment _horizontal_text_alignment;	
				::rux::uint8 _is_masked;
				rux::gui::Thickness _mask_corner;
				rux::gui::RenderCacheBase* _pseudo_cache;
				declare_rux_property_without_member( XColorWrap , Foreground );
			public:
				explicit TextBox( const char* variable_name = 0 , const char* __file__ = 0 , ::rux::int32 __line__ = 0 );
				~TextBox();
				declare_content_size();
				::rux::gui::ColorBase* get_Foreground( void );
				void set_Foreground( ::rux::gui::ColorBase* foreground );
				void set_Background(::rux::gui::ColorBase* background);
			};
			class XTextBox : public XGCHandle<TextBox>
			{
				declare_rux_base_methods(TextBox);
				friend class XGroup;
				friend class Group;
			private:
			public:
				declare_rux_set_operators( TextBox );
				XTextBox();	
				XTextBox( const TextBox& uitextbox );
				XTextBox( const XTextBox& uitextbox );
				explicit XTextBox( const XObject& object );
				explicit XTextBox( XGCRef* object );
				void set_Foreground( ::rux::gui::ColorBase* foreground );
				void set_Background(::rux::gui::ColorBase* background);
				void set_Font( const char* font_file_name ,
					::rux::uint32 font_size_height ,
					::rux::uint32 font_size_width );
				void set_Text( const ::rux::XString& text );
				void set_IsMasked( ::rux::uint8 is_masked );
				void set_RegexpMask( const ::rux::XString& mask , ::rux::XString& error );
				void set_LeftMouseDownSelectionRegexpMask( const ::rux::XString& mask , ::rux::XString& error );
				void set_IsMultiLines( ::rux::uint8 is_multi_lines );
				rux::String& get_Text( void );
				void set_BorderColor( ::rux::gui::Color* border_color );
				void set_BorderThickness( float border_thickness );	
				float get_BorderThickness( void );	
				void set_OnTextChanged( ::rux::gui::events::on_event_t on_text_changed_event_callback );	
				DECLARE_BASE_UI_FUNCTIONS();	
			};
		};
	};
};
#endif
