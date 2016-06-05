#ifndef XAN_UISLIDER_H
#define XAN_UISLIDER_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_string.h>
#include <xan_ui_enums.h>
#include <xan_window.h>
enum XEnum_SliderOrientation
{
	XEnum_SliderOrientation_Vertical , 
	XEnum_SliderOrientation_Horizontal
};
enum XEnum_SliderMouseClickValueChange
{
	XEnum_SliderMouseClickValueChange_Gradual , 
	XEnum_SliderMouseClickValueChange_Instant
};
namespace rux
{
	namespace gui
	{
		namespace controls
		{
			class Slider : public ::rux::gui::ControlBase
			{
				declare_rux_base_internal_methods();
				friend class XSlider;
			private:
				DECLARE_BASE_UI_MEMBERS();
				DECLARE_RUX_BASE_UI_FUNCTIONS( Slider );
				float _value;
				float _value_step;
				float _min_value;
				float _max_value;
				XEnum_SliderOrientation _orientation;	
				float _track_width;
				float _thumb_height;
				float _thumb_width;
				rux::gui::ColorBase* _track_background;
				rux::gui::Color* _transparent_color;
				rux::gui::RenderCacheBase* _ui_border_cache;
				rux::gui::RenderCacheBase* _ui_cache;
				rux::gui::RenderCacheBase* _ui_thumb_border_cache;
				rux::gui::RenderCacheBase* _ui_thumb_cache;
				rux::gui::ColorBase* _thumb_background;
				rux::gui::ColorBase* _thumb_over_background;
				rux::gui::ColorBase* _thumb_active_background;
				rux::gui::ColorBase* _thumb_dragging_background;
				rux::gui::Thickness _thumb_thickness;
				::rux::EventHandler< ::rux::gui::events::on_value_changed_t > _on_ui_value_changed_callback;
				XCrtSect _cs_value_change_thread;
				rux::threading::XThread _value_change_thread;
				::rux::uint8 _is_raise;
				::rux::uint8 _is_value_change_thread_inited;
				::rux::uint8 _is_thumb_dragging;
				XEnum_SliderMouseClickValueChange _slider_mouse_click_value_change;
			public:
				explicit Slider( const char* variable_name = 0 , const char* __file__ = 0 , ::rux::int32 __line__ = 0 );
				~Slider();
				::rux::uint8 IsPointInThumb( ::rux::int16 x , ::rux::int16 y );
				::rux::uint8 IsPointInThumb( ::rux::int16 x , ::rux::int16 y , ::rux::uint8& left , ::rux::uint8& above );
				::rux::uint8 IsPointInTrack( ::rux::int16 x , ::rux::int16 y , ::rux::uint8& left , ::rux::uint8& above , float& x_offset , float& y_offset );
				void set_Value( float value );
				void set_ValueWithoutEvent( float value );
				float get_MaxValue( void );
				float get_MinValue( void );
				declare_content_size();
			private:
				static void value_change( void* param , size_t ___rux__thread_index1986 );
				void private_set_ActiveThumbBackground( ::rux::gui::ColorBase* background );
			};
			class XSlider : public XGCHandle<Slider>
			{
				declare_rux_base_methods(Slider);
				friend class XGroup;				
			private:
			public:
				declare_rux_set_operators( Slider );
				XSlider();
				XSlider( const Slider& uislider );
				XSlider( const XSlider& uislider );
				explicit XSlider( const XObject& object );
				explicit XSlider( XGCRef* object );
				void set_OnValueChanged( ::rux::gui::events::on_value_changed_t _on_ui_value_changed_callback );
				void set_Value( float value );
				void set_ValueWithoutEvent( float value );
				void set_ValueStep( float value );
				void set_TrackWidth( float track_width );	
				void set_ThumbWidth( float thumb_width );
				void set_ThumbHeight( float thumb_height );
				void set_MaxValue( float max_value );
				void set_MinValue( float min_value );
				void set_Orientation( XEnum_SliderOrientation orientation );
				void set_TrackBackground( ::rux::gui::ColorBase* track_background );
				void set_ThumbBackground( ::rux::gui::ColorBase* thumb_background );
				void set_ThumbOverBackground( ::rux::gui::ColorBase* thumb_over_background );
				void set_ThumbActiveBackground( ::rux::gui::ColorBase* thumb_active_background );
				void set_ThumbDraggingBackground( ::rux::gui::ColorBase* thumb_dragging_background );
				void set_ThumbThickness( const ::rux::gui::Thickness& thumb_thickness );
				void set_MouseClickValueChangeType( XEnum_SliderMouseClickValueChange slider_mouse_click_value_change );
				float get_Value( void );
				DECLARE_BASE_UI_FUNCTIONS();	
			};
		};
	};
};
#endif
