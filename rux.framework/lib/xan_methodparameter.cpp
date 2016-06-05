#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_methodparameter.h>
#include <xan_keyvaluepair.h>
#include <xan_uint64.h>
#include <xan_boolean.h>
begin_implement_rux_class_ns( MethodParameter , rux );
	_method_parameter_type = EMethodParameterType_Ptr;
	_value_ptr = NULL;
end_implement_rux_class();
namespace rux
{	
	MethodParameter::~MethodParameter()
	{
		if( _value_ptr )
		{
			::rux::engine::free_mem( _value_ptr );
		}
	};
	XMethodParameter::XMethodParameter( ::rux::int32 value )
	{
		(*this)()->_method_parameter_type = EMethodParameterType_Int32;
		(*this)()->_value_ptr = alloc_array_macros( char , sizeof( ::rux::int32 ) );
		::memcpy( (*this)()->_value_ptr , &value , sizeof( ::rux::int32 ) );
	};
	XMethodParameter::XMethodParameter( ::rux::uint32 value )
	{
		(*this)()->_method_parameter_type = EMethodParameterType_UInt32;
		(*this)()->_value_ptr = alloc_array_macros( char , sizeof( ::rux::uint32 ) );
		::memcpy( (*this)()->_value_ptr , &value , sizeof( ::rux::uint32 ) );
	};
	XMethodParameter::XMethodParameter( ::rux::int64 value )
	{
		(*this)()->_method_parameter_type = EMethodParameterType_Int64;
		(*this)()->_value_ptr = alloc_array_macros( char , sizeof( ::rux::int64 ) );
		::memcpy( (*this)()->_value_ptr , &value , sizeof( ::rux::int64 ) );
	};
	XMethodParameter::XMethodParameter( ::rux::uint64 value )
	{
		(*this)()->_method_parameter_type = EMethodParameterType_UInt64;
		(*this)()->_value_ptr = alloc_array_macros( char , sizeof( ::rux::uint64 ) );
		::memcpy( (*this)()->_value_ptr , &value , sizeof( ::rux::uint64 ) );
	};
	XMethodParameter::XMethodParameter( float value )
	{
		(*this)()->_method_parameter_type = EMethodParameterType_Float;
		(*this)()->_value_ptr = alloc_array_macros( char , sizeof( float ) );
		::memcpy( (*this)()->_value_ptr , &value , sizeof( float ) );
	};
	XMethodParameter::XMethodParameter( double value )
	{
		(*this)()->_method_parameter_type = EMethodParameterType_Double;
		(*this)()->_value_ptr = alloc_array_macros( char , sizeof( double ) );
		::memcpy( (*this)()->_value_ptr , &value , sizeof( double ) );
	};
	XMethodParameter::XMethodParameter( void* value_ptr )
	{
		(*this)()->_method_parameter_type = EMethodParameterType_Ptr;
		(*this)()->_value_ptr = alloc_array_macros( char , sizeof( void* ) );
		::memcpy( (*this)()->_value_ptr , &value_ptr , sizeof( void* ) );
	};
	XMethodParameter::XMethodParameter( const char* value_ptr )
	{
		(*this)()->_method_parameter_type = EMethodParameterType_String;
		if( value_ptr )
		{
			::rux::uint32 length = ::strlen( value_ptr );
			if( length > 0 )
			{
				(*this)()->_value_ptr = alloc_array_macros( char , length + 1 );
				::memcpy( (*this)()->_value_ptr , value_ptr , length + 1 );
			}
			else
			{
				(*this)()->_value_ptr = alloc_array_macros( char , 1 );
				((char*)(*this)()->_value_ptr)[0] = '\0';
			}
		}	
	};
	XMethodParameter::XMethodParameter( const wchar_t* value_ptr )
	{
		(*this)()->_method_parameter_type = EMethodParameterType_String;
		if( value_ptr )
		{
			size_t length = wcslen( value_ptr );
			if( length > 0 )
			{
				(*this)()->_value_ptr = alloc_array_macros( wchar_t , length + 1 );
				::memcpy( (*this)()->_value_ptr , value_ptr , ( length + 1 ) * sizeof( wchar_t ) );
			}		
			else
			{
				(*this)()->_value_ptr = alloc_array_macros( char , sizeof( wchar_t ) );
				((wchar_t*)(*this)()->_value_ptr)[0] = '\0';
			}
		}
	};
};