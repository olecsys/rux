#ifndef XAN_METHODPARAMETER_H
#define XAN_METHODPARAMETER_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_string.h>
enum EMethodParameterType
{
	EMethodParameterType_Int32,
	EMethodParameterType_UInt32,
	EMethodParameterType_Int64,
	EMethodParameterType_Double,
	EMethodParameterType_Ptr	,
	EMethodParameterType_String ,
	EMethodParameterType_Void ,
	EMethodParameterType_Boolean , 
	EMethodParameterType_Float ,
	EMethodParameterType_UInt64
};
namespace rux
{	
	begin_declare_rux_class( MethodParameter );
		EMethodParameterType _method_parameter_type;
		void* _value_ptr;
		~MethodParameter();
	begin_declare_rux_class_members( MethodParameter );
	end_declare_rux_class_members( MethodParameter );
		XMethodParameter( ::rux::int32 value );
		XMethodParameter( ::rux::uint32 value );	
		XMethodParameter( ::rux::int64 value );
		XMethodParameter( ::rux::uint64 value );
		XMethodParameter( float value );
		XMethodParameter( double value );
		XMethodParameter( const char* value );
		XMethodParameter( const wchar_t* value );
		XMethodParameter( void* value_ptr );
	end_declare_rux_class();
};
#endif
