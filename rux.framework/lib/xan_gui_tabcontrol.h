#ifndef XAN_GUI_TABCONTROL_H
#define XAN_GUI_TABCONTROL_H
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
#include <xan_uigroup.h>
namespace rux
{
	namespace gui
	{
		namespace controls
		{
			class TabControl;
			begin_declare_rux_gui_user_control( TabControl );
				~TabControl( void );
				void AddPage( const ::rux::XString& name , ::rux::gui::controls::XGroup& page );
				void set_SelectedPage( size_t page_index );
				void set_SelectedPage( ::rux::gui::controls::XGroup& page );
				static void on_group_button_enter( const ::rux::gui::events::Event& event );
				static void on_group_button_leave( const ::rux::gui::events::MouseEvent& event );
				static void on_group_button_left_mouse_button_down( const ::rux::gui::events::MouseEvent& event );
				void set_IsPageVisible( const ::rux::XString& name , ::rux::byte visible );
			begin_declare_rux_gui_user_control_members( TabControl );
				::rux::gui::controls::XGroup _group_pages;
				::rux::gui::controls::XGroup _group_page;
				size_t _selected_index;

			end_declare_rux_gui_user_control_members( TabControl );
				void AddPage( const ::rux::XString& name , ::rux::gui::controls::XGroup& page );
				void set_SelectedPage( size_t page_index );
				void set_SelectedPage( ::rux::gui::controls::XGroup& page );
				void set_IsPageVisible( const ::rux::XString& name , ::rux::byte visible );
			end_declare_rux_gui_user_control();	
		};
	};
};
#endif