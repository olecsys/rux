#ifndef XAN_UILINE_H
#define XAN_UILINE_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_ui_enums.h>
#include <xan_gui_defines.h>
#include <xan_float.h>
#include <xan_uianimation.h>
namespace rux
{
	namespace gui
	{
		namespace controls
		{
			class Line : public ::rux::gui::ControlBase
			{
				declare_rux_base_internal_methods();
				friend class XLine;
			private:
				::rux::pid_t _render_thread_id;
				rux_volatile _update_polygon;
				rux::gui::Margin _margin;
				::rux::gui::XEnum_Filling _vertical_filling;
				::rux::gui::XEnum_Filling _horizontal_filling;
				float _start_left;
				float _start_top;
				float _end_left;
				float _end_top;
				float _cache_start_left;
				float _cache_start_top;
				float _cache_end_left;
				float _cache_end_top;
				float _thickness;				
				rux::gui::XEnum_Cursors _cursor;
				rux::gui::Color* _color;
				::rux::gui::ParentBase* _parent_control;
				::rux::threading::RdWrLock _cs_parent_control;
				::rux::gui::Window* _parent_window;
			public:
				rux_volatile _free_system_resources_accomplished;
			private:
				::rux::pid_t _free_system_resources_thread_id;
				rux::gui::XEnum_PointUnit _point_unit;
				float _opacity;
				::rux::uint8 _is_visible;	
				::rux::byte _is_mouse_entered;
				char _utf8_control_name[ 128 ];
				::rux::uint32 _utf8_control_name_hash;
				XObject _tag;
				XObject _private_tag;
				void* _tag_ptr;
				::rux::byte _is_forward_events;
				::rux::EventHandler< ::rux::gui::events::on_event_t > _on_mouse_enter;
				::rux::EventHandler< ::rux::gui::events::on_event_t > _on_resized_callback;
				::rux::EventHandler< ::rux::gui::events::on_event_t > _on_relocated_callback;
				::rux::EventHandler< ::rux::gui::events::on_mouse_event_t > _on_mouse_leave_callback;
				float _bottom_sticker;
				float _right_sticker;
				::booldog::threading::rdwrlock _cs_drawing_elements;
				::rux::EventHandler< ::rux::gui::events::on_mouse_event_t > _on_mouse_double_click;
				::rux::EventHandler< ::rux::gui::events::on_mouse_event_t > _on_right_mouse_button_double_click;
				::rux::EventHandler< ::rux::gui::events::on_mouse_event_t > _on_mouse_move;	
				::rux::EventHandler< ::rux::gui::events::on_mouse_event_t > _on_left_mouse_button_down;
				::rux::EventHandler< ::rux::gui::events::on_mouse_event_t > _on_left_mouse_button_up;	
				::rux::EventHandler< ::rux::gui::events::on_mouse_event_t > _on_right_mouse_button_down;
				::rux::EventHandler< ::rux::gui::events::on_mouse_event_t > _on_mouse_wheel_down;
				::rux::EventHandler< ::rux::gui::events::on_mouse_event_t > _on_mouse_wheel_up;
				::rux::EventHandler< ::rux::gui::events::on_mouse_event_t > _on_mouse_wheel_double_click;
				::rux::EventHandler< ::rux::gui::events::on_mouse_event_t > _on_right_mouse_button_up;	
				::rux::EventHandler< ::rux::gui::events::on_event_t > _on_activated;
				::rux::EventHandler< ::rux::gui::events::on_deactivated_event_t > _on_deactivated;
				::rux::int32 _tab_index;
				rux::gui::RenderCacheBase* _ui_line_cache;
				rux::gui::RenderCacheBase* _ui_cap1_cache;
				rux::gui::RenderCacheBase* _ui_cap2_cache;
				rux_volatile _already_dispatch_on_resized;
				rux_volatile _already_dispatch_on_relocated;
				DECLARE_RUX_BASE_UI_MIN_FUNCTIONS();
				void initialize_members( const char* variable_name , const char* __file__ , ::rux::int32 __line__ );
				declare_register_rux_property();
				declare_rux_property_without_member( ::rux::XInt32 , ZIndex );
				declare_rux_property_without_member( ::rux::XFloat , StartLeft );
				declare_rux_property_without_member( ::rux::XFloat , StartTop );
				declare_rux_property_without_member( ::rux::XFloat , EndLeft );
				declare_rux_property_without_member( ::rux::XFloat , EndTop );
				declare_rux_property_without_member( ::rux::XFloat , Opacity );
				declare_rux_property_without_member( ::rux::XBoolean , Visible );
				declare_rux_property_without_member( XColorWrap , Color );
			public:
				explicit Line( const char* variable_name = 0 , const char* __file__ = 0 , ::rux::int32 __line__ = 0 );
				~Line();
				float get_ClipLeft( void );
				float get_ClipTop( void );
				float get_ClipWidth( void );
				float get_ClipHeight( void );
				rux::gui::Margin& get_Margin( void );
				float get_Width( float* cache_height_ptr = NULL );
				float get_Height( float* cache_height_ptr = NULL );
				float get_Left( ::rux::uint8 relative_to_parent = 0 , float* cache_top_ptr = NULL );
				float get_Top( ::rux::uint8 relative_to_parent = 0 , float* cache_top_ptr = NULL );
				declare_content_size();
				float get_ContentWidth( void );
				float get_ContentHeight( void );
				void set_Cursor( ::rux::gui::XEnum_Cursors cursor );
				rux::gui::XEnum_Cursors get_Cursor( void );
				::rux::gui::XEnum_Filling get_VerticalFilling( void );
				::rux::gui::XEnum_Filling get_HorizontalFilling( void );
				::rux::gui::XEnum_Alignment get_VerticalAlignment( void );
				::rux::gui::XEnum_Alignment get_HorizontalAlignment( void );
				void set_StartLeft( float left );
				void set_StartTop( float top );
				void set_EndLeft( float left );
				void set_EndTop( float top );
				void set_Color( ::rux::gui::ColorBase* background );
				void set_Left( float left ) const;
				void set_Top( float top ) const;
				float get_StartLeft( void );
				float get_StartTop( void );
				float get_EndLeft( void );
				float get_EndTop( void );
				void set_Thickness( float thickness );
			private:
				void update_polygon( void );
				::rux::gui::ColorBase* get_Color( void );
			private:
				// copy and assignment not allowed
				Line( const Line& );
				void operator =( const Line& );
			};
			class XLine : public XGCHandle<Line>
			{
				declare_rux_base_methods(Line);
				friend class XGroup;
				friend class Group;				
			private:
			public:
				declare_rux_set_operators( Line );
				XLine();
				XLine( const Line& uiline );
				XLine( const XLine& uiline );
				explicit XLine( const XObject& object );
				explicit XLine( XGCRef* object );
				void set_StartLeft( float left );
				void set_StartTop( float top );
				void set_EndLeft( float left );
				void set_EndTop( float top );
				void set_Color( ::rux::gui::ColorBase* background );
				rux::gui::ColorBase* get_Color( void );	
				void set_Thickness( float thickness );	
				void set_PointUnit( ::rux::gui::XEnum_PointUnit point_unit );
				void set_Opacity( float opacity );
				void set_IsVisible( ::rux::uint8 is_visible );
				::rux::uint8 get_IsVisible( void );	
				void set_ControlName( const ::rux::XString& name );
				rux::String& get_ControlName( void );
				::rux::uint32 get_ControlNameHash( void );
				void set_OnLeftMouseButtonDown( ::rux::gui::events::on_mouse_event_t on_left_mouse_button_down );
				void set_OnRightMouseButtonDown( ::rux::gui::events::on_mouse_event_t on_left_mouse_button_down );
				void set_OnMouseWheelDown( ::rux::gui::events::on_mouse_event_t on_left_mouse_button_down );
				void set_OnMouseEnter( ::rux::gui::events::on_event_t on_mouse_enter_callback );
				void set_OnMouseLeave( ::rux::gui::events::on_mouse_event_t on_mouse_leave_callback );
				::rux::gui::ParentBase* get_ParentControl( ::rux::byte lock = 1 , ::rux::byte real_parent = 0 );
				void set_Tag( const XObject& tag );
				Object& get_Tag( void );
				void set_Cursor( ::rux::gui::XEnum_Cursors cursor );
				rux::gui::XEnum_Cursors get_Cursor( void );
				float get_Opacity( void );
				void set_TagPtr( void* tag_ptr );
				void* get_TagPtr( void ) const;
				::rux::int32 get_ZIndex( void );
				void set_ZIndex( size_t index );
			};
		};
	};
};
#endif
