#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_uianimation.h>
#include <xan_float.h>
#include <xan_window.h>
#include <xan_uitextblock.h>
#include <xan_uitextbox.h>
#include <xan_uiprogressbar.h>
#include <xan_uiimage.h>
#include <xan_uigroup.h>
#include <xan_uirectangle.h>
#define clip( x ) x > 0 ? x < 255 ? x : 255 : 0
#include <xan_keyvaluepair.h>
#include <xan_uint64.h>
#include <xan_property_info.h>
#include <xan_boolean.h>
#include <xan_math.h>
#include <xan_expr.h>
#include <xan_binding.h>
begin_implement_rux_class_ns( ColorWrap , rux::gui );
	_color = NULL;
end_implement_rux_class();
implement_rux_base_methods_ns( Animation , rux::gui );
implement_rux_set_operators_ns_( Animation , rux::gui );
namespace rux
{
	namespace gui
	{
		ColorWrap::~ColorWrap()
		{
			if( _color )
				_color->Release();
		};
		XColorWrap::XColorWrap( ::rux::gui::ColorBase* color )
		{
			rux::gui::copy_color( (*this)()->_color , color );
		};		
		implement_ctor_and_register_type( Animation );
		Animation::Animation( const char* , const char* __file__ , ::rux::int32 __line__ ) 
			: XGCRef( get_static_Type )
		{
			_last_set_value.set_Info( "" , __file__ , __line__ );
			_start_value.set_Info( "" , __file__ , __line__ );
			_control_gcref = 0;
			_animation_set_value_type = XEnum_AnimationSetValueType_Gradual;
			_duration = 0;
			_animation_index = 0;
			_clear_after_completed = 0;
		};
		Animation::~Animation( void )
		{
			if( _control_gcref )
				_control_gcref->Release();
		};
		XAnimation::XAnimation()
		{	
		};
		void XAnimation::x_initialize( void )
		{
		};
		implement_duplicate_internal_function_1( Animation , set_IsClearAfterCompleted , ::rux::byte );
		void Animation::set_IsClearAfterCompleted( ::rux::byte clear_after_completed )
		{
			_clear_after_completed = clear_after_completed;
		};
		XAnimation::XAnimation( const Animation& uianimation )
		{
			(*this) = uianimation;
		};
		XAnimation::XAnimation( const XObject& object )
		{
			*static_cast<XGCHandle<Animation>*>(this) = object;
		};
		XAnimation::XAnimation( const XAnimation& uianimation )
		{
			(*(static_cast<XGCHandle<Animation>*>( this ))) = uianimation;
		};
		XAnimation& XAnimation::operator =( const Animation& uianimation )
		{
			(*(static_cast<XGCHandle<Animation>*>( this ))) = uianimation;
			return *this;
		};
		XAnimation& XAnimation::operator =( const XAnimation& uianimation )
		{
			(*(static_cast<XGCHandle<Animation>*>( this ))) = uianimation;
			return *this;
		};
		implement_duplicate_internal_result_function( uint32 , Animation , get_Duration );
		implement_duplicate_internal_function_1( Animation , Animate , uint32 );
		implement_duplicate_internal_function( Animation , Start );
		implement_duplicate_internal_function( Animation , Stop );
		implement_duplicate_internal_function_1( Animation , set_AnimationSetValueType , XEnum_AnimationSetValueType );
		void XAnimation::set_PropertyName( const ::rux::XString& property_name )
		{
			(*this)()->_property_name = property_name;
		};
		void XAnimation::set_StartValue( const XObject& start_value )
		{
			(*this)()->_start_value = start_value;
		};
		void XAnimation::set_StartValue( ::rux::gui::ColorBase* start_value )
		{
			XColorWrap color_wrap( start_value );
			(*this)()->_start_value = color_wrap;
		};
		implement_duplicate_internal_function_1( Animation , set_Control , const XObject& );
		void Animation::set_Control( const XObject& control )
		{
			if( _control_gcref )
				_control_gcref->Release();
			_control_gcref = control.get_GCRef();
			_control_gcref->AddRef();
		};
		implement_duplicate_internal_function_3( Animation , set_Timeline , ::rux::uint32 , uint64 , const XObject& );
		void Animation::set_Timeline( ::rux::uint32 index , uint64 duration_in_microseconds , const XObject& value )
		{
			WRITE_LOCK( _cs_timeline );
			_duration = 0;
			if( _timeline.Count() > index )
			{
				_timeline.RemoveAt( index );
				_timeline.Insert( index , (rux::uint32)( duration_in_microseconds / 1000ULL ) );
				_values.set_Item( index , value );
			}
			_cs_timeline.WriteUnlock();
		};
		implement_duplicate_internal_function_3( Animation , set_Timeline , ::rux::uint32 , uint64 , ::rux::gui::ColorBase* );
		void Animation::set_Timeline( ::rux::uint32 index , uint64 duration_in_microseconds , ::rux::gui::ColorBase* value )
		{
			XColorWrap color_wrap( value );
			WRITE_LOCK( _cs_timeline );
			_duration = 0;
			if( _timeline.Count() > index )
			{
				_timeline.RemoveAt( index );
				_timeline.Insert( index , (::rux::uint32)( duration_in_microseconds / 1000ULL ) );
				_values.set_Item( index , color_wrap );
			}
			_cs_timeline.WriteUnlock();
		};
		implement_duplicate_internal_function_2( Animation , AddTimeline , uint64 , const XObject& );
		void Animation::AddTimeline( uint64 duration_in_microseconds , const XObject& value )
		{	
			WRITE_LOCK( _cs_timeline );
			_duration = 0;
			_timeline.Add( (::rux::uint32)( duration_in_microseconds / 1000ULL ) );
			_values.Add( value , __FILE__ , __LINE__  );
			_cs_timeline.WriteUnlock();
		};
		implement_duplicate_internal_function_2( Animation , AddTimeline , uint64 , ::rux::gui::ColorBase* );
		void Animation::AddTimeline( uint64 duration_in_microseconds , ::rux::gui::ColorBase* value )
		{
			XColorWrap color_wrap( value );
			WRITE_LOCK( _cs_timeline );
			_duration = 0ULL;
			_timeline.Add( (rux::uint32)( duration_in_microseconds / 1000ULL ) );
			_values.Add( color_wrap , __FILE__ , __LINE__ );
			_cs_timeline.WriteUnlock();
		};
		::rux::uint32 Animation::get_Duration( void )
		{
			if( _duration == 0 )
			{		
				READ_LOCK( _cs_timeline );
				for( size_t index0 = 0 ; index0 < _timeline.Count() ; index0++ )
					_duration += _timeline[ index0 ];
				_cs_timeline.ReadUnlock();
			}
			return _duration;
		};
		rux_inline Object& process_value( Object* value )
		{
			declare_variable( XObject , result );
			if( rux_is_object_ptr( value , ::rux::XExpr ) )
			{
				XGCRef* gcref = value->get_GCRefObj();
				if( gcref )
				{
					::rux::Expr* expr = (::rux::Expr*)gcref->get_ObjectAddress();
					result = expr->Calculate();
				}
			}
			else if( rux_is_object_ptr( value , ::rux::XBinding ) )
			{
				XGCRef* gcref = value->get_GCRefObj();
				if( gcref )
				{
					::rux::Binding* expr = (::rux::Binding*)gcref->get_ObjectAddress();
					result = expr->GetValue();
				}
			}
			else
				result.set_ByRef( value );
			return result++;
		};
		void Animation::Animate( uint32 animation_time )
		{	
			if( animation_time < get_Duration() )
			{
				READ_LOCK( _cs_timeline );
				if( _timeline.Count() > 0 )
				{
					::rux::uint32 timeline_duration = 0;
					declare_variable( XObject , start_value );
					declare_variable( XObject , end_value );
					float percent = 0.0f;
					for( size_t index0 = 0 ; index0 < _animation_index ; index0++ )
						timeline_duration += _timeline[ index0 ];
					for( ; _animation_index < _timeline.Count() ; _animation_index++ )
					{
						timeline_duration += _timeline[ _animation_index ];
						if( animation_time < timeline_duration )
						{
							timeline_duration -= _timeline[ _animation_index ];
							timeline_duration = animation_time - timeline_duration;
							percent = (float)timeline_duration / (float)_timeline[ _animation_index ];
							end_value = _values[ _animation_index ];
							end_value.set_ByRef( process_value( end_value() ) );
							
							if( _animation_index == 0 )
								start_value = _start_value;
							else
								start_value = _values[ _animation_index - 1 ];
							start_value.set_ByRef( process_value( start_value() ) );
							break;
						}
					}
					::rux::uint8 is_set = 1;
					declare_variable( XObject , new_value );
					if( _animation_set_value_type == XEnum_AnimationSetValueType_Gradual )
					{
						if( rux_is_object( start_value , XFloat ) && rux_is_object( end_value , XFloat ) )
						{
							XFloat start_value_double( start_value ) , end_value_double( end_value );
							new_value = XFloat( ::rux::math::round( start_value_double() + percent * ( end_value_double() - start_value_double() ) , 2 ) );					
							if( rux_is_object( _last_set_value , XFloat ) )
							{
								XFloat last_set_value( _last_set_value );
								XFloat new_value_double( new_value );
								is_set = last_set_value() == new_value_double() ? 0 : 1;
							}
						}
						else if( rux_is_object( start_value , XColorWrap ) && rux_is_object( end_value , XColorWrap ) )
						{
							XColorWrap start_value_color( start_value ) , end_value_color( end_value );
							if( start_value_color()->_color->_color_type == 0 && end_value_color()->_color->_color_type == 0 )
							{
								rux::gui::Color* sc = (rux::gui::Color*)start_value_color()->_color;
								rux::gui::Color* ec = (rux::gui::Color*)end_value_color()->_color;
								rux::gui::Color* new_value_color = create_color( (uint8)( clip( (int32)sc->get_Red() + percent * ( (int32)ec->get_Red() - sc->get_Red() ) ) ) ,
									(uint8)( clip( (int32)sc->get_Green() + percent * ( (int32)ec->get_Green() - (int32)sc->get_Green() ) ) ) ,
									(uint8)( clip( (int32)sc->get_Blue() + percent * ( (int32)ec->get_Blue() - (int32)sc->get_Blue() ) ) ) ,
									(uint8)( clip( (int32)sc->get_Alpha() + percent * ( (int32)ec->get_Alpha() - (int32)sc->get_Alpha() ) ) ) );
								new_value = XColorWrap( new_value_color );
								if( rux_is_object( _last_set_value , XColorWrap ) )
								{
									XColorWrap last_set_value( _last_set_value );						
									if( last_set_value()->_color->_color_type == 0 )
										is_set = *((rux::gui::Color*)last_set_value()->_color) == *new_value_color ? 0 : 1;
								}
							}
						}
					}
					else
					{
						new_value = start_value;
						if( rux_is_object( _last_set_value , XFloat ) 
							&& rux_is_object( new_value , XFloat ) )
						{
							XFloat last_set_value( _last_set_value );						
							XFloat new_set_value( new_value );
							is_set = last_set_value() == new_set_value() ? 0 : 1;
						}
						else if( rux_is_object( _last_set_value , XColorWrap )
							&& rux_is_object( new_value , XColorWrap ) )
						{
							XColorWrap last_set_value( _last_set_value );
							XColorWrap new_set_value( new_value );
							if( last_set_value()->_color->_color_type == 0 
								&& new_set_value()->_color->_color_type == 0 )
								is_set = *((rux::gui::Color*)last_set_value()->_color) == *((rux::gui::Color*)new_set_value()->_color) ? 0 : 1;
						}
					}
					if( is_set == 1 )
					{
						private_set_Value( new_value() );
						_last_set_value = new_value;
					}
				}		
				_cs_timeline.ReadUnlock();
			}
			else
				Stop();
		};
		void Animation::Start( void )
		{
			READ_LOCK( _cs_timeline );
			private_set_Value( _start_value() );
			_last_set_value = _start_value;
			_animation_index = 0;
			_cs_timeline.ReadUnlock();
		};
		void Animation::Stop( void )
		{
			READ_LOCK( _cs_timeline );
			if( _values.Count() > 0 )
			{
				Object* val = &_values[ _values.Count() - 1 ];
				private_set_Value( &process_value( val ) );
				_last_set_value = val;
			}
			_cs_timeline.ReadUnlock();
		};
		implement_duplicate_internal_function_1( Animation , private_set_Value , Object* );
		void Animation::private_set_Value( Object* value )
		{	
			if( _control_gcref )
			{
				Type* type = &_control_gcref->get_Type();
				::rux::PropertyInfo* property_info = type->get_PropertyInfo( _property_name.str() );
				if( property_info && value->get_GCRefObj() )
					property_info->Set( _control_gcref , value->get_GCRefObj() );
			}
		};
		void Animation::set_AnimationSetValueType( XEnum_AnimationSetValueType animation_set_value_type )
		{
			_animation_set_value_type = animation_set_value_type;
		};
		Object& XAnimation::get_Control( void )
		{
			return XObject( (*this)()->_control_gcref , 1 , "" , __FILE__ , __LINE__ )++;
		};		
	};
};