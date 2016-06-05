#ifndef XAN_GUI_CHECKBOX_H
#define XAN_GUI_CHECKBOX_H
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
			class CheckBox;
			begin_declare_rux_gui_user_control( CheckBox );
				~CheckBox( void );
				static void on_group_main_enter( const ::rux::gui::events::Event& event );
				static void on_group_main_leave( const ::rux::gui::events::MouseEvent& event );
				static void on_group_main_left_mouse_button_down( const ::rux::gui::events::MouseEvent& event );
				void set_Checked( ::rux::byte checked , ::rux::byte raise_event = 1 );
				::rux::byte get_Checked( void );
				void set_Text( const ::rux::XString& text );
				void set_Font( const char* font_file_name , ::rux::uint32 font_size_height , ::rux::uint32 font_size_width );
				void set_OnCheckedChanged( ::rux::gui::events::on_event_t on_checked_changed );
				virtual void set_TagPtr( void* tag_ptr );
				virtual void* get_TagPtr( void ) const;
			begin_declare_rux_gui_user_control_members( CheckBox );
				void* _tag_ptr;
				::rux::gui::controls::XRectangle _rectangle_symbol;
				::rux::gui::controls::XGroup _group_symbol;
				::rux::gui::controls::XGroup _group_text;
				::rux::gui::controls::XTextBlock _textblock_text;
				::rux::gui::controls::XLine _line_checked0;
				::rux::gui::controls::XLine _line_checked1;
				::rux::EventHandler< ::rux::gui::events::on_event_t > _on_checked_changed;
				::rux::byte _checked;
			end_declare_rux_gui_user_control_members( CheckBox );
				void set_Checked( ::rux::byte checked , ::rux::byte raise_event = 1 );
				::rux::byte get_Checked( void );
				void set_Text( const ::rux::XString& text );
				void set_Font( const char* font_file_name , ::rux::uint32 font_size_height , ::rux::uint32 font_size_width );
				void set_OnCheckedChanged( ::rux::gui::events::on_event_t on_checked_changed );
			end_declare_rux_gui_user_control();	
		};
	};
};
#endif