#ifndef XAN_UIDATEPICKER_H
#define XAN_UIDATEPICKER_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_ui_enums.h>
#include <xan_uigroup.h>
#include <xan_uigroupanimation.h>
#include <xan_jsonserializer.h>
#include <xan_time.h>
#include <xan_uitextblock.h>
#include <xan_uibutton.h>
namespace rux
{
	namespace gui
	{
		namespace controls
		{
			class DatePicker : public ::rux::gui::ControlBase
			{
				declare_rux_base_internal_methods();
				friend class XDatePicker;
			private:
				DECLARE_BASE_UI_MEMBERS();
				DECLARE_RUX_BASE_UI_FUNCTIONS( DatePicker );	
				rux::gui::RenderCacheBase* _ui_border_cache;
				rux::gui::RenderCacheBase* _ui_cache;		
				rux::gui::RenderCacheBase* _ui_date_cache;		
				rux::gui::FontBase* _date_font;
				rux::gui::Color* _date_foreground;
				rux::gui::ColorBase* _datepicker_background;
				rux::gui::Color* _datepicker_border;	
				rux::gui::Thickness _datepicker_thickness;		
				::rux::byte _opened;
				float _date_container_width;
				XGroup _date_container;
				XGroup _group_header;
				XGroup _group_week_days;
				XGroup _group_calendar;
				XGroup _group_calendar_months;
				rux::gui::controls::XButton _button_left;
				rux::gui::controls::XButton _button_right;
				rux::gui::ColorBase* _date_container_background;
				rux::gui::Color* _date_container_border;
				rux::gui::Thickness _date_container_thickness;	
				float _date_container_border_width;	
				XGroupAnimation _date_container_group_animation;	
				::rux::uint8 _is_editable;	
				//rux::XTime _unix_time;
				::rux::uint64 _posix_time;
				::rux::gui::XEnum_Alignment _horizontal_text_alignment;	
				::rux::gui::XEnum_Alignment _vertical_text_alignment;
				::rux::EventHandler< ::rux::gui::events::on_event_t > _on_datetime_changed_event_callback;
				XTextBlock _textblock_month;
				XTextBlock _textblock_year;
			public:
				explicit DatePicker( const char* variable_name = 0 , const char* __file__ = 0 , ::rux::int32 __line__ = 0 );
				~DatePicker();
				void set_DateTime( uint64 unix_time );
				void set_DateTimeWithoutEvent( uint64 unix_time );
				void add_gui( void );
				declare_content_size();
			private:		
				static void on_button_click( const ::rux::gui::events::Event& sender );
				static void on_date_left_mouse_down( const ::rux::gui::events::MouseEvent& event );
				static void on_date_left_mouse_double_click( const ::rux::gui::events::MouseEvent& event );
				static void on_date_enter( const ::rux::gui::events::Event& sender );
				static void on_date_leave( const ::rux::gui::events::MouseEvent& event );	
				void refresh_date_container( ::rux::uint64 time );	
				void accept_date( void );
				static void on_deactivated( const XObject& sender , const XObject& activated_control );
				static void on_mouse_wheel( const ::rux::gui::events::MouseEvent& event );
			};
			class XDatePicker : public XGCHandle<DatePicker>
			{
				declare_rux_base_methods(DatePicker);
			private:
			public:
				declare_rux_set_operators( DatePicker );
				XDatePicker();
				XDatePicker( const DatePicker& uidatepicker );
				XDatePicker( const XDatePicker& uidatepicker );
				explicit XDatePicker( const XObject& object );
				explicit XDatePicker( XGCRef* object );
				DECLARE_BASE_UI_FUNCTIONS();
			public:
				void set_DateTime( uint64 unix_time );
				void set_DateTimeWithoutEvent( uint64 unix_time );
				::rux::uint64 get_DateTime( void );
				void set_OnDatetimeChanged( ::rux::gui::events::on_event_t on_datetime_changed_event_callback );
			};
		};
	};
};
#endif
