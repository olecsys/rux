#ifndef XAN_UIBUTTON_H
#define XAN_UIBUTTON_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_ui_enums.h>
#include <xan_window.h>
#include <xan_crtsect.h>
#include <xan_float.h>
namespace rux
{
	namespace gui
	{
		class Window;
		namespace controls
		{
			class Group;			
			class Button : public ::rux::gui::ParentBase
			{
				declare_rux_base_internal_methods();
				friend class XButton;
				friend class ::rux::gui::controls::Group;
				friend class ::rux::gui::Window;
			private:
				DECLARE_BASE_UI_MEMBERS();
				rux::gui::ColorBase* _background;
				rux::gui::ColorBase* _over_background;
				rux::gui::ColorBase* _pressed_background;
				rux::gui::ColorBase* _disabled_background;
				rux::gui::ColorBase* _active_state_background;
				rux::gui::Color* _border_color;
				XObject _content;
				rux::threading::RdWrLock _cs_content;
				rux::gui::RenderCacheBase* _ui_cache;
				rux::gui::RenderCacheBase* _ui_border_cache;
				rux::gui::FontBase* _ui_font;
				rux::gui::Color* _foreground;
				::rux::EventHandler< ::rux::gui::events::on_event_t > _on_click_callback;
				rux::gui::Thickness _corner;
				::rux::uint8 _is_repeat_until_mouse_up;
				rux::threading::XThread _repeat_thread;	
				size_t _current_render_index;
				::rux::gui::Rectangle _clip;
				DECLARE_RUX_BASE_UI_FUNCTIONS( Button );
				declare_rux_property_without_member( XColorWrap , Background );
				declare_rux_property_without_member( XColorWrap , OverBackground );
				declare_rux_property_without_member( XColorWrap , PressedBackground );
				declare_rux_property_without_member( XColorWrap , DisabledBackground );
			private:
				static void repeat_thread( void* param , size_t ___rux__thread_index1986 );
			public:
			private:
				void private_ResetChildLocationAndSizeCache( ::rux::uint8 reset , ::rux::gui::ControlBase* control , ::rux::uint8 left , ::rux::uint8 top , ::rux::uint8 width , ::rux::uint8 height , size_t controls_start_index = 0 );
			public:
				explicit Button( const char* variable_name = 0 , const char* __file__ = 0 , ::rux::int32 __line__ = 0 );
				~Button();
				void set_Content( const XObject& content );
				void set_Content( const Object& content );
				void set_Content( const XGCRef& content );
				virtual void get_Clip( ::rux::gui::Rectangle*& clip , ::rux::uint8 scrollbar = 0 );
				void set_ContentOpacity( float opacity );
				void set_ContentOpacity( const ::rux::XString& control_name , float opacity );
				void set_Background( ::rux::gui::ColorBase* background );	
				void set_ContentBackground( ::rux::gui::ColorBase* background );
				void set_ContentBackground( const ::rux::XString& control_name , ::rux::gui::ColorBase* background );
				void set_ContentBorderColor( ::rux::gui::Color* border_color );
				void set_OverBackground( ::rux::gui::ColorBase* over_background );
				void set_PressedBackground( ::rux::gui::ColorBase* pressed_background );
				void set_DisabledBackground( ::rux::gui::ColorBase* disabled_background );
				::rux::gui::ColorBase* get_Background( void );
				declare_content_size();
				::rux::gui::ColorBase* get_DisabledBackground( void );
				::rux::gui::ColorBase* get_OverBackground( void );
				::rux::gui::ColorBase* get_PressedBackground( void );
				void set_Corner( const ::rux::gui::Thickness& corner );
				void set_OnClick( ::rux::gui::events::on_event_t on_click_callback );
			private:
				virtual void RemoveControl( XGCRef* control );
				virtual void RemoveTabControls( void );
				virtual void AddTabControls( void );
				virtual ::rux::byte Exists( ::rux::gui::CommonBase* control );
				virtual void set_ScrollXOffset( float scroll_x_offset );
				virtual void set_ScrollYOffset( float scroll_y_offset );
				virtual float get_ScrollXOffset( void );
				virtual float get_ScrollYOffset( void );
				virtual ::rux::int32 IndexOf( XGCRef* control );
				virtual void set_ZIndex( XGCRef* control , size_t index );
			};
			class XButton : public XGCHandle<Button>
			{
				declare_rux_base_methods(Button);				
				friend class XGroup;
				friend class Group;
			private:
			public:
				declare_rux_set_operators( Button );
				XButton();
				XButton( const Button& uibutton );
				XButton( const XButton& uibutton );

				explicit XButton( const XObject& object );

				explicit XButton( XGCRef* object );




				DECLARE_BASE_UI_FUNCTIONS();
			public:
				void set_Background( ::rux::gui::ColorBase* background );	
				void set_ContentOpacity( float opacity );
				void set_ContentOpacity( const ::rux::XString& control_name , float opacity );
				void set_ContentBackground( ::rux::gui::ColorBase* background );
				void set_ContentBackground( const ::rux::XString& control_name , ::rux::gui::ColorBase* background );
				void set_ContentBorderColor( ::rux::gui::Color* border_color );
				void set_OverBackground( ::rux::gui::ColorBase* over_background );
				void set_PressedBackground( ::rux::gui::ColorBase* pressed_background );
				void set_DisabledBackground( ::rux::gui::ColorBase* disabled_background );
				void set_Content( const XObject& content );
				void set_Content( const Object& content );
				void set_Content( const XGCRef& content );
				void set_Font( const char* font_file_name ,
					::rux::uint32 font_size_height ,
					::rux::uint32 font_size_width );
				void set_Foreground( ::rux::gui::Color* foreground );
				void set_OnClick( ::rux::gui::events::on_event_t on_click_callback );
				void set_Corner( const ::rux::gui::Thickness& corner );
				void get_Clip( ::rux::gui::Rectangle*& clip );
				void set_IsRepeatUntilMouseUp( ::rux::uint8 is_repeat_until_mouse_up );
			};
		};
	};
};
#endif
