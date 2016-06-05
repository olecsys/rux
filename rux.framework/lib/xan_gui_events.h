#ifndef XAN_GUI_EVENTS_H
#define XAN_GUI_EVENTS_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_object.h>
namespace rux
{
	namespace gui
	{
		namespace events
		{
			enum XEnum_EventType
			{
				XEnum_EventType_MouseMove = 1 ,
				XEnum_EventType_MouseEnter = 2 ,
				XEnum_EventType_MouseLeave = 4 ,
				XEnum_EventType_LeftMouseButtonDown = 8 ,
				XEnum_EventType_LeftMouseButtonUp = 16
			};
			struct MouseEvent
			{
				XObject _sender;
				::rux::gui::WindowMouseEvent* _window_event;
				::rux::byte _explicit_event;
				MouseEvent( XGCRef& sender , ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
				{
					_sender = sender;
					_window_event = window_event;
					_explicit_event = explicit_event;
				};
			};
			struct KeyboardEvent
			{
				XObject _sender;
				::rux::int32 _key;
				::rux::uint8 _shift;
				::rux::uint8 _control;
				::rux::uint8 _alt;
				::rux::uint32 _native_value;
				::rux::byte _explicit_event;
				KeyboardEvent( XGCRef& sender , ::rux::int32 key , ::rux::uint8 shift , ::rux::uint8 control , ::rux::uint8 alt , ::rux::uint32 native_value , ::rux::byte explicit_event )
				{
					_sender = sender;
					_key = key;
					_shift = shift;
					_control = control;
					_alt = alt;
					_native_value = native_value;
					_explicit_event = explicit_event;
				};
			};
			struct Event
			{
				XObject _sender;
				::rux::byte _explicit_event;
				Event( XGCRef& sender , ::rux::byte explicit_event )
				{
					_sender = sender;
					_explicit_event = explicit_event;
				};
			};
			typedef void (*on_mouse_event_t)( const MouseEvent& event );
			typedef void (*on_keyboard_event_t)( const KeyboardEvent& event );
			typedef void (*on_event_t)( const Event& event );
			typedef void (*on_object_event_t)( const XObject& sender , const XObject& object );
			typedef void (*on_deactivated_event_t)( const XObject& sender , const XObject& activated_control );
			typedef void (*on_drop_t)( const XObject& control , const XObject& object );
			typedef void (*on_value_changed_t)( const XObject& sender , float old_value , float new_value );
			typedef ::rux::String& (*on_language_t)( const char* english_key );
		}
	};
};
#endif
