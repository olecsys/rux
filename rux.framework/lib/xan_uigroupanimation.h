#ifndef XAN_UIGROUPANIMATION_H
#define XAN_UIGROUPANIMATION_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_ui_enums.h>
#include <xan_thread.h>
#include <xan_uianimation.h>
namespace rux
{
	namespace gui
	{
		class GroupAnimation : public XGCRef
		{
			declare_rux_base_internal_methods();
			friend class XGroupAnimation;
			friend class Window;
		private:
			::rux::threading::RdWrLock _cs_animation_schedule_index;
			size_t _animation_schedule_index;
			::rux::uint32 _start_time;
			class Window* _parent_window_ptr;
			XEnum_AnimationType _animation_type;	
			::rux::threading::RdWrLock _cs_animations;
			XArray< ::rux::gui::XAnimation > _animations;	
			::rux::uint32 _duration;
			::rux::EventHandler< ::rux::gui::events::on_event_t > _on_completed;
			rux::XString _animation_name;
			::rux::uint8 _stop_is_executed;
			::rux::XArray< ::rux::XArray< ::rux::gui::XAnimation > > _after_completed_animations;	
		public:
			explicit GroupAnimation( const char* variable_name = 0 , const char* __file__ = 0 , ::rux::int32 __line__ = 0 );
			~GroupAnimation();
			::rux::uint64 get_Duration( void );
			::rux::uint8 get_IsStarted( void );
			void Start( ::rux::byte anew = 1 );
			void Stop( void );
			void AddAnimationsAfterCompleted( const ::rux::XArray< ::rux::gui::XAnimation >& animations );
			void Clear( void );
			void AddAnimation( const XAnimation& animation );
		};
		class XGroupAnimation : public XGCHandle<GroupAnimation>
		{
			declare_rux_base_methods(GroupAnimation);
		private:
		public:
			declare_rux_set_operators( GroupAnimation );
			XGroupAnimation();
			XGroupAnimation( class XWindow* window_ptr );
			XGroupAnimation( const GroupAnimation& uigroupanimation );
			XGroupAnimation( const XGroupAnimation& uigroupanimation );
			explicit XGroupAnimation( const XObject& object );
			void AddAnimation( const ::rux::gui::XAnimation& animation );
			void AddAnimationsAfterCompleted( const ::rux::XArray< ::rux::gui::XAnimation >& animations );
			void Start( ::rux::byte anew = 1 );
			void Stop( void );
			void set_AnimationType( XEnum_AnimationType animation_type );
			::rux::uint64 get_Duration( void );
			void Clear( void );
			void set_OnCompleted( ::rux::gui::events::on_event_t on_completed_callback );
			rux::gui::Animation& get_Animation( ::rux::uint32 index );
			void set_AnimationName( const ::rux::XString& animation_name );
			rux::String& get_AnimationName( void );
			static void private_Animation( void* param );	
			::rux::uint8 get_IsStarted( void );
		};
	};
};
#endif
