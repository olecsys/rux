#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_uigroupanimation.h>
#include <xan_time.h>
#include <xan_window.h>
#include <xan_keyvaluepair.h>
#include <xan_uint64.h>
#include <xan_uiapplication.h>
#include <xan_boolean.h>
implement_rux_base_methods_ns( GroupAnimation , rux::gui );
implement_rux_set_operators_ns_( GroupAnimation , rux::gui );
namespace rux
{
	namespace gui
	{
		implement_ctor_and_register_type( GroupAnimation );
		GroupAnimation::GroupAnimation( const char* , const char* , ::rux::int32 )
			: XGCRef( get_static_Type )
		{
			_start_time = 0;
			_animation_type = XEnum_AnimationType_Repeat;
			_parent_window_ptr = NULL;
			_duration = 0;
			_on_completed = NULL;
			_stop_is_executed = 0;
			_animation_schedule_index = SIZE_MAX;
		};
		GroupAnimation::~GroupAnimation()
		{
			Stop();
		};
		XGroupAnimation::XGroupAnimation()
		{	
		};
		void XGroupAnimation::x_initialize( void )
		{
		};
		XGroupAnimation::XGroupAnimation( XWindow* window_ptr )
		{
			(*this)()->_parent_window_ptr = (*window_ptr)();
		};
		XGroupAnimation::XGroupAnimation( const GroupAnimation& uigroupanimation )
		{
			(*this) = uigroupanimation;
		};
		XGroupAnimation::XGroupAnimation( const XObject& object )
		{
			*static_cast<XGCHandle<GroupAnimation>*>(this) = object;
		};
		XGroupAnimation::XGroupAnimation( const XGroupAnimation& uigroupanimation )
		{
			(*(static_cast<XGCHandle<GroupAnimation>*>( this ))) = uigroupanimation;
		};
		XGroupAnimation& XGroupAnimation::operator =( const GroupAnimation& uigroupanimation )
		{
			(*(static_cast<XGCHandle<GroupAnimation>*>( this ))) = uigroupanimation;
			return *this;
		};
		XGroupAnimation& XGroupAnimation::operator =( const XGroupAnimation& uigroupanimation )
		{
			(*(static_cast<XGCHandle<GroupAnimation>*>( this ))) = uigroupanimation;
			return *this;
		};
		Animation& XGroupAnimation::get_Animation( ::rux::uint32 index )
		{
			XAnimation animation;
			READ_LOCK( (*this)()->_cs_animations );
			if( index < (*this)()->_animations.Count() )
				animation = (*this)()->_animations[ index ];
			(*this)()->_cs_animations.ReadUnlock();
			return animation++;
		};
		rux::uint8 GroupAnimation::get_IsStarted( void )
		{
			WRITE_LOCK( _cs_animation_schedule_index );
			rux::uint8 res = _animation_schedule_index != SIZE_MAX ? 1 : 0;
			_cs_animation_schedule_index.WriteUnlock();
			return res;
		};
		rux::uint8 XGroupAnimation::get_IsStarted( void )
		{
			return (*this)()->get_IsStarted();
		};
		implement_duplicate_internal_function_1( GroupAnimation , Start , ::rux::byte );
		void GroupAnimation::Start( ::rux::byte anew )
		{
			WRITE_LOCK( _cs_animation_schedule_index );
			if( _animation_schedule_index == SIZE_MAX )
			{
				_stop_is_executed = 0;
				_start_time = ::rux::XTime::GetTickCount();	
				READ_LOCK( _cs_animations );
				for( size_t index0 = 0 ; index0 < _animations.Count() ; index0++ )
					_animations[ index0 ].Start();
				_cs_animations.ReadUnlock();
				_animation_schedule_index = ::rux::gui::application::add_schedule( XGroupAnimation::private_Animation , this , 20000ULL );
			}
			else if( anew )
			{
				_stop_is_executed = 0;
				_start_time = ::rux::XTime::GetTickCount();	
			}
			_cs_animation_schedule_index.WriteUnlock();
		};
		implement_duplicate_internal_function( GroupAnimation , Stop );
		void GroupAnimation::Stop( void )
		{
			WRITE_LOCK( _cs_animation_schedule_index );
			size_t animation_schedule_index = _animation_schedule_index;
			_animation_schedule_index = SIZE_MAX;
			_cs_animation_schedule_index.WriteUnlock();
			if( animation_schedule_index != SIZE_MAX )
				::rux::gui::application::remove_schedule( animation_schedule_index );
		};
		implement_duplicate_internal_function_1( GroupAnimation , AddAnimationsAfterCompleted , const ::rux::XArray< ::rux::gui::XAnimation >& );
		void GroupAnimation::AddAnimationsAfterCompleted( const ::rux::XArray< ::rux::gui::XAnimation >& animations )
		{			
			WRITE_LOCK( _cs_animations );
			if( _animations.Count() > 0 )
				_after_completed_animations.Add( animations );
			else
				_animations.AddRange( animations );
			_cs_animations.WriteUnlock();
		};
		void XGroupAnimation::set_OnCompleted( ::rux::gui::events::on_event_t on_completed_callback )
		{
			(*this)()->_on_completed = on_completed_callback;
		};
		implement_duplicate_internal_function( GroupAnimation , Clear );
		void GroupAnimation::Clear( void )
		{			
			Stop();
			WRITE_LOCK( _cs_animations );
			_duration = 0;
			_on_completed = NULL;
			_animations.Clear();
			_cs_animations.WriteUnlock();
		};
		rux::uint64 GroupAnimation::get_Duration( void )
		{
			if( _duration == 0 )
			{
				READ_LOCK( _cs_animations );
				for( size_t index0 = 0 ; index0 < _animations.Count() ; index0++ )
				{
					if( _animations[ index0 ].get_Duration() > _duration )
						_duration = _animations[ index0 ].get_Duration();
				}
				_cs_animations.ReadUnlock();
			}
			return _duration;
		};
		rux::uint64 XGroupAnimation::get_Duration( void )
		{	
			return (*this)()->get_Duration();
		};
		void XGroupAnimation::private_Animation( void* param )
		{
			rux::uint8 is_stop = 0;
			GroupAnimation* group_animation_ptr = (GroupAnimation*)param;	
			group_animation_ptr->AddRef();
			rux::uint32 now = ::rux::XTime::GetTickCount();
			if( now < group_animation_ptr->_start_time )
				group_animation_ptr->_start_time = now;
			rux::uint32 animation_time = now - group_animation_ptr->_start_time;
			if( group_animation_ptr->get_Duration() <= animation_time )
			{		
				if( group_animation_ptr->_animation_type == XEnum_AnimationType_OneTime )
				{
					READ_LOCK( group_animation_ptr->_cs_animations );
					for( size_t index0 = 0 ; index0 < group_animation_ptr->_animations.Count() ; index0++ )
						group_animation_ptr->_animations[ index0 ].Stop();
					group_animation_ptr->_cs_animations.ReadUnlock();
					is_stop = 1;
				}
				else
				{
					if( group_animation_ptr->_stop_is_executed == 0 )
					{
						group_animation_ptr->_stop_is_executed = 1;
						READ_LOCK( group_animation_ptr->_cs_animations );
						for( size_t index0 = 0 ; index0 < group_animation_ptr->_animations.Count() ; index0++ )
							group_animation_ptr->_animations[ index0 ].Stop();
						group_animation_ptr->_cs_animations.ReadUnlock();
					}
					else
					{
						group_animation_ptr->_start_time = now;
						group_animation_ptr->_stop_is_executed = 0;
						READ_LOCK( group_animation_ptr->_cs_animations );
						for( size_t index0 = 0 ; index0 < group_animation_ptr->_animations.Count() ; index0++ )
							group_animation_ptr->_animations[ index0 ].Start();
						group_animation_ptr->_cs_animations.ReadUnlock();
					}
				}
			}	
			else
			{
				READ_LOCK( group_animation_ptr->_cs_animations );
				for( size_t index0 = 0 ; index0 < group_animation_ptr->_animations.Count() ; index0++ )
					group_animation_ptr->_animations[ index0 ].Animate( animation_time );
				group_animation_ptr->_cs_animations.ReadUnlock();
			}
			if( group_animation_ptr->_parent_window_ptr )
					group_animation_ptr->_parent_window_ptr->Invalidate();
			if( is_stop == 1 )
			{		
				::rux::byte need_start = 0;
				group_animation_ptr->_on_completed.raise< const ::rux::gui::events::Event >( ::rux::gui::events::Event( *group_animation_ptr , 1 ) );
				WRITE_LOCK( group_animation_ptr->_cs_animations );
				if( group_animation_ptr->_after_completed_animations.Count() > 0 )
				{
					group_animation_ptr->_animations.Clear();
					group_animation_ptr->_animations.AddRange( group_animation_ptr->_after_completed_animations[ 0 ] );
					group_animation_ptr->_after_completed_animations.RemoveAt( 0 );
					group_animation_ptr->_duration = 0;
					for( size_t index0 = group_animation_ptr->_animations.Count() - 1 ; ; index0-- )
					{
						group_animation_ptr->_animations[ index0 ]._clear_after_completed = 1;
						if( index0 == 0 )
							break;
					}
					need_start = 1;
				}
				else
				{
					if( group_animation_ptr->_animations.Count() > 0 )
					{
						for( size_t index0 = group_animation_ptr->_animations.Count() - 1 ; ; index0-- )
						{
							if( group_animation_ptr->_animations[ index0 ]._clear_after_completed == 1 )
								group_animation_ptr->_animations.RemoveAt( index0 );
							if( index0 == 0 )
								break;
						}
					}
				}
				group_animation_ptr->_cs_animations.WriteUnlock();
				group_animation_ptr->Stop();
				if( need_start )
					group_animation_ptr->Start();
			}
			group_animation_ptr->Release();
		};
		void XGroupAnimation::set_AnimationType( XEnum_AnimationType animation_type )
		{
			(*this)()->_animation_type = animation_type;
		};
		implement_duplicate_internal_function_1( GroupAnimation , AddAnimation , const XAnimation& );
		void GroupAnimation::AddAnimation( const XAnimation& animation )
		{
			WRITE_LOCK( _cs_animations );
			_duration = 0;
			_animations.Add( animation );
			_cs_animations.WriteUnlock();
		};
		void XGroupAnimation::set_AnimationName( const ::rux::XString& animation_name )
		{
			(*this)()->_animation_name = animation_name;
		};
		rux::String& XGroupAnimation::get_AnimationName( void )
		{
			return (*this)()->_animation_name++;
		};
	};
};