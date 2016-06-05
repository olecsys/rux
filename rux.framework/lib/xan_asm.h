#ifndef XAN_INVOKE_H
#define XAN_INVOKE_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_defines.h>
#include <xan_diagnostic_defines.h>
enum XEnum_ReturnValueType
{
	XEnum_ReturnValueType_Default = 0 ,
	XEnum_ReturnValueType_Float = 1 , 
	XEnum_ReturnValueType_Double = 2 ,	
	XEnum_ReturnValueType_64bits = 3
};
#ifdef __x86__
#ifdef __WINDOWS__
#pragma pack( push )
#pragma pack( 1 )
__declspec( align( 1 ) )
#endif
struct rux_runtime_param_t
{
	XEnum_ReturnValueType _type;	
	::rux::uint32 _value0;
	::rux::uint32 _value1;
	rux_runtime_param_t();
	rux_runtime_param_t& operator =( const char* value );
	rux_runtime_param_t& operator =( void* value );
	rux_runtime_param_t& operator =( double value );
	rux_runtime_param_t& operator =( float value );
	rux_runtime_param_t& operator =( ::rux::int64 value );
	rux_runtime_param_t& operator =( ::rux::uint64 value );
	rux_runtime_param_t& operator =( ::rux::int32 value );
	rux_runtime_param_t& operator =( ::rux::uint32 value );
}
#ifdef __UNIX__
__attribute__( ( packed , aligned( 1 ) ) )
#endif
;
#ifdef __WINDOWS__
#pragma pack( pop )
#endif
#elif defined( __x64__ )
#ifdef __WINDOWS__
#pragma pack( push )
#pragma pack( 1 )
__declspec( align( 16 ) )
#endif
struct rux_runtime_param_t
{
	XEnum_ReturnValueType _type;	
	::rux::uint32 _just_to_align;
	::rux::uint64 _value0;	
	rux_runtime_param_t();
	rux_runtime_param_t& operator =( const char* value );
	rux_runtime_param_t& operator =( void* value );
	rux_runtime_param_t& operator =( double value );
	rux_runtime_param_t& operator =( float value );
	rux_runtime_param_t& operator =( ::rux::int64 value );
	rux_runtime_param_t& operator =( ::rux::uint64 value );
	rux_runtime_param_t& operator =( ::rux::int32 value );
	rux_runtime_param_t& operator =( ::rux::uint32 value );
}
#ifdef __UNIX__
__attribute__( ( packed , aligned( 16 ) ) )
#endif
;
#ifdef __WINDOWS__
#pragma pack( pop )
#endif
#endif
#ifdef __x86__
extern "C" ::rux::int64 __cdecl RuntimeInvoke( void* fn_address_ptr , XEnum_ReturnValueType float_type ,  rux_runtime_param_t* params_ptr , ::rux::uint32 params_count );
extern "C" ::rux::int64 __cdecl ObjectInvoke( void* fn_address_ptr , XEnum_ReturnValueType float_type , void* object_ptr , rux_runtime_param_t* params_ptr , ::rux::uint32 params_count );
#elif defined( __x64__ )
extern "C" ::rux::int64 __cdecl RuntimeInvoke( void* fn_address_ptr , XEnum_ReturnValueType float_type ,  rux_runtime_param_t* params_ptr , ::rux::uint64 params_count );
extern "C" ::rux::int64 __cdecl ObjectInvoke( void* fn_address_ptr , XEnum_ReturnValueType float_type , void* object_ptr , rux_runtime_param_t* params_ptr , ::rux::uint64 params_count );
#endif
#ifdef __x86__
extern "C" void __cdecl AsmLeftShift( void* buffer_ptr , ::rux::uint32 length );
extern "C" void __cdecl AsmRightShift( void* buffer_ptr , ::rux::uint32 length );
extern "C" void __cdecl AsmMinus( void* buffer_ptr0 , void* buffer_ptr1 , ::rux::uint32 length );
extern "C" void __cdecl AsmPlus( void* buffer_ptr0 , void* buffer_ptr1 , ::rux::uint32 length );
extern "C" void __cdecl AsmCompare( void* buffer_ptr0 , void* buffer_ptr1 , ::rux::uint32 length , ::rux::int32* result );
#elif defined( __x64__ )
extern "C" void __cdecl AsmLeftShift( void* buffer_ptr , ::rux::uint64 length );
extern "C" void __cdecl AsmRightShift( void* buffer_ptr , ::rux::uint64 length );
extern "C" void __cdecl AsmMinus( void* buffer_ptr0 , void* buffer_ptr1 , ::rux::uint64 length );
extern "C" void __cdecl AsmPlus( void* buffer_ptr0 , void* buffer_ptr1 , ::rux::uint64 length );
extern "C" void __cdecl AsmCompare( void* buffer_ptr0 , void* buffer_ptr1 , ::rux::uint64 length , ::rux::int64* result );
#endif
template< class T >
#ifdef __x86__
T rux_runtime_invoke( void* fn_address_ptr , rux_runtime_param_t* params_ptr , ::rux::uint32 params_count )
#else
T rux_runtime_invoke( void* fn_address_ptr , rux_runtime_param_t* params_ptr , ::rux::uint64 params_count )
#endif
{	
	if( typeid( T ) == typeid( float ) )
	{
		XEnum_ReturnValueType float_type = XEnum_ReturnValueType_Float;
		::rux::int64 result_int64 = RuntimeInvoke( fn_address_ptr , float_type , params_ptr , params_count );
		T result = 0;
		::memcpy( &result , &result_int64 , sizeof( T ) );
		return result;
	}
	else if( typeid( T ) == typeid( double ) )
	{
		XEnum_ReturnValueType float_type = XEnum_ReturnValueType_Double;
		::rux::int64 result_int64 = RuntimeInvoke( fn_address_ptr , float_type , params_ptr , params_count );
		T result = 0;
		::memcpy( &result , &result_int64 , sizeof( T ) );
		return result;
	}
	else if( typeid( T ) == typeid( ::rux::int64 )
		|| typeid( T ) == typeid( ::rux::uint64 ) )
	{
		XEnum_ReturnValueType float_type = XEnum_ReturnValueType_64bits;
		return ( T )RuntimeInvoke( fn_address_ptr , float_type , params_ptr , params_count );
	}
	else
	{
		XEnum_ReturnValueType float_type = XEnum_ReturnValueType_Default;
		::rux::int64 result_int64 = RuntimeInvoke( fn_address_ptr , float_type , params_ptr , params_count );
		T result = 0;
		::memcpy( &result , &result_int64 , sizeof( T ) );
		return result;
	}	
};
#endif