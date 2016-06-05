#ifndef XAN_UIFLOATANIMATION_H
#define XAN_UIFLOATANIMATION_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_string.h>
#include <xan_crtsect.h>
#include <xan_thread.h>
#include <xan_ui_enums.h>
namespace rux
{
	namespace gui
	{
		class XGroupAnimation;
		begin_declare_rux_class( ColorWrap );	
			friend class XAnimation;
			friend class Animation;
			friend class ::rux::gui::XGroupAnimation;
		begin_declare_rux_class_members( ColorWrap );			
		public:
			rux::gui::ColorBase* _color;
			~ColorWrap();
		end_declare_rux_class_members( ColorWrap );
		public:
			XColorWrap( ::rux::gui::ColorBase* color );
		end_declare_rux_class();
		class Animation : public XGCRef
		{
			declare_rux_base_internal_methods();
			friend class XAnimation;
			friend class ::rux::gui::XGroupAnimation;
		private:
			XGCRef* _control_gcref;
			::rux::threading::RdWrLock _cs_timeline;
			XMallocArray< uint32 , 5 > _timeline;
			XArray< XObject > _values;
			rux::XString _property_name;
			XObject _start_value;	
			XEnum_AnimationSetValueType _animation_set_value_type;
			::rux::uint32 _duration;
			XObject _last_set_value;
			::rux::uint32 _animation_index;
			::rux::byte _clear_after_completed;
		public:
			explicit Animation( const char* variable_name = 0 , const char* __file__ = 0 , ::rux::int32 __line__ = 0 );
			~Animation( void );
			::rux::uint32 get_Duration( void );
			void Animate( uint32 animation_time );
			void Start( void );
			void Stop( void );
			void private_set_Value( Object* value );
			void set_AnimationSetValueType( XEnum_AnimationSetValueType animation_set_value_type );
			void set_Timeline( ::rux::uint32 index , uint64 duration_in_microseconds , const XObject& value );
			void set_Timeline( ::rux::uint32 index , uint64 duration_in_microseconds , ::rux::gui::ColorBase* value );
			void AddTimeline( uint64 duration_in_microseconds , const XObject& value );
			void AddTimeline( uint64 duration_in_microseconds , ::rux::gui::ColorBase* value );
			void set_IsClearAfterCompleted( ::rux::byte clear_after_completed );
			void set_Control( const XObject& control );
		};
		class XAnimation : public XGCHandle<Animation>
		{
			declare_rux_base_methods(Animation);
		private:
		public:
			declare_rux_set_operators( Animation );
			XAnimation();	
			XAnimation( const Animation& uianimation );
			XAnimation( const XAnimation& uianimation );
			explicit XAnimation( const XObject& object );
			void set_Control( const XObject& control );
			void set_PropertyName( const ::rux::XString& property_name );
			void set_StartValue( const XObject& start_value );
			void set_StartValue( ::rux::gui::ColorBase* start_value );
			void AddTimeline( uint64 duration_in_microseconds , const XObject& value );
			void AddTimeline( uint64 duration_in_microseconds , ::rux::gui::ColorBase* value );
			void set_Timeline( ::rux::uint32 index , uint64 duration_in_microseconds , const XObject& value );
			void set_Timeline( ::rux::uint32 index , uint64 duration_in_microseconds , ::rux::gui::ColorBase* value );
			::rux::uint32 get_Duration( void );
			void Animate( uint32 animation_time );
			void Start( void );
			void Stop( void );
			void set_AnimationSetValueType( XEnum_AnimationSetValueType animation_set_value_type );
			Object& get_Control( void );
			void set_IsClearAfterCompleted( ::rux::byte clear_after_completed );
		private:
			void private_set_Value( Object* value );
		};
	};
};
#endif
