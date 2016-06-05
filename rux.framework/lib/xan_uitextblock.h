#ifndef XAN_UITEXTBLOCK_H
#define XAN_UITEXTBLOCK_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_gui_defines.h>
#include <xan_ui_enums.h>
#include <xan_window.h>
namespace rux
{
	namespace gui
	{
		namespace controls
		{
			class TextBlock : public ::rux::gui::ControlBase
			{
				declare_rux_base_internal_methods();
				friend class XTextBlock;
				friend class Button;
				friend class XButton;
			private:
				DECLARE_BASE_UI_MEMBERS();
				rux::gui::FontBase* _font;
				rux::XString _utf8text;
				::rux::uint32 _text_hash;
				rux::gui::Color* _foreground;	
				rux::gui::ColorBase* _background_object;
				rux::gui::Color* _border_object;
				rux::gui::RenderCacheBase* _ui_border_cache;
				rux::gui::RenderCacheBase* _ui_cache;	
				rux::gui::RenderCacheBase* _text_cache;
				::rux::gui::XEnum_Alignment _horizontal_text_alignment;	
				::rux::gui::XEnum_Alignment _vertical_text_alignment;	
				float _border_width;
				float _text_left_margin;
				float _text_top_margin;
				rux::gui::Thickness _corner;
				float _text_left;
				float _text_top;
				float _text_width;
				float _text_height;
				float _render_text_width;
				float _render_text_height;
				::rux::byte _text_need_refresh;
				rux_volatile _reset_cache;
				DECLARE_RUX_BASE_UI_FUNCTIONS( TextBlock );
				declare_rux_property_without_member( XColorWrap , Foreground );
			public:
				explicit TextBlock( const char* variable_name = 0 , const char* __file__ = 0 , ::rux::int32 __line__ = 0 );
				~TextBlock();
				void set_Text( const ::rux::XString& text );
				void set_Text( const char* text , size_t utf8bytes );
				rux::String& get_Text( void );
				void set_Foreground( ::rux::gui::ColorBase* foreground );
				void set_TextVerticalAlignment( ::rux::gui::XEnum_Alignment text_alignment );
				void set_TextHorizontalAlignment( ::rux::gui::XEnum_Alignment text_alignment );
				void set_Background( ::rux::gui::ColorBase* background );
				float get_TextWidth( void );
				float get_TextHeight( void );
				void set_TextMargin( float left , float top );
				declare_content_size();
				::rux::gui::ColorBase* get_Foreground( void );
				void set_BorderColor( ::rux::gui::ColorBase* border_color );
				void set_BorderWidth( float border_width );
				void set_Font( const char* font_file_name ,
					::rux::uint32 font_size_height ,
					::rux::uint32 font_size_width );
			};
			class XTextBlock : public XGCHandle<TextBlock>
			{
				declare_rux_base_methods(TextBlock);
				friend class XGroup;
				friend class Group;				
				friend class XButton;
			private:
			public:
				declare_rux_set_operators( TextBlock );
				XTextBlock();
				XTextBlock( const TextBlock& uigroup , const char* variable_name , const char* __file__ , ::rux::int32 __line__ );
				XTextBlock( const XTextBlock& uigroup , const char* variable_name , const char* __file__ , ::rux::int32 __line__ );
				XTextBlock( const TextBlock& uitextblock );
				XTextBlock( const XTextBlock& uitextblock );
				explicit XTextBlock( const XObject& object );
				explicit XTextBlock( XGCRef* object );


				void set_Font( const char* font_file_name ,
					::rux::uint32 font_size_height ,
					::rux::uint32 font_size_width );
				void set_Text( const ::rux::XString& text );
				void set_Text( const char* text , size_t utf8bytes );
				void set_Foreground( ::rux::gui::ColorBase* foreground );
				void set_TextVerticalAlignment( ::rux::gui::XEnum_Alignment text_alignment );
				void set_TextHorizontalAlignment( ::rux::gui::XEnum_Alignment text_alignment );
				void set_Background( ::rux::gui::ColorBase* background );
				void set_TextMargin( float left , float top );
				void set_Corner( const ::rux::gui::Thickness& corner );
				rux::String& get_Text( void );
				float get_TextWidth( void );
				float get_TextHeight( void );
				void set_BorderColor( ::rux::gui::ColorBase* border_color );
				void set_BorderWidth( float border_width );
				DECLARE_BASE_UI_FUNCTIONS();
			};
		};
	};
};
#endif
