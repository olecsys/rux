#ifndef XAN_UITIMEPICKER_H
#define XAN_UITIMEPICKER_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_ui_enums.h>
#include <xan_uigroup.h>
#include <xan_uigroupanimation.h>
#include <xan_jsonserializer.h>
#include <xan_time.h>
#include <xan_uitextbox.h>
namespace rux
{
	namespace gui
	{
		namespace controls
		{
			class TimePicker : public ::rux::gui::ParentBase
			{
				declare_rux_base_internal_methods();
				friend class XTimePicker;
			private:
				DECLARE_BASE_UI_MEMBERS();
				DECLARE_RUX_BASE_UI_FUNCTIONS( TimePicker );	
				rux::gui::controls::XTextBox _time_textbox;
				::rux::uint64 _posix_time;
				rux::gui::FontBase* _time_font;
				::rux::EventHandler< ::rux::gui::events::on_event_t > _on_datetime_changed_event_callback;	
				size_t _current_render_index;
				::rux::gui::Rectangle _clip;
				void private_ResetChildLocationAndSizeCache( ::rux::uint8 reset , ::rux::gui::ControlBase* control , ::rux::uint8 left , ::rux::uint8 top , ::rux::uint8 width , ::rux::uint8 height , size_t controls_start_index = 0 );
			public:
				explicit TimePicker( const char* variable_name = 0 , const char* __file__ = 0 , ::rux::int32 __line__ = 0 );
				~TimePicker();
				void set_DateTime( uint64 unix_time );
				void set_DateTimeWithoutEvent( uint64 unix_time );
				virtual void get_Clip( ::rux::gui::Rectangle*& clip , ::rux::uint8 scrollbar = 0 );
				declare_content_size();
			private:	
				static void on_text_changed( const ::rux::gui::events::Event& sender );
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
			class XTimePicker : public XGCHandle<TimePicker>
			{
				friend class Group;
				friend class Button;
				friend class TimePicker;
				declare_rux_base_methods( TimePicker );
			private:
			public:
				declare_rux_set_operators( TimePicker );
				XTimePicker();
				XTimePicker( const TimePicker& uitimepicker );
				XTimePicker( const XTimePicker& uitimepicker );
				explicit XTimePicker( const XObject& object );
				explicit XTimePicker( XGCRef* object );
				DECLARE_BASE_UI_FUNCTIONS();
			public:
				void set_DateTime( uint64 unix_time );
				void set_DateTimeWithoutEvent( uint64 unix_time );
				::rux::uint64 get_DateTime( void );
				void set_OnDatetimeChanged( ::rux::gui::events::on_event_t on_datetime_changed_event_callback );
			private:
				void get_Clip( ::rux::gui::Rectangle*& clip );
			};
		};
	};
};
#endif
