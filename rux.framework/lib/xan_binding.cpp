#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_binding.h>
#include <xan_type.h>
#include <xan_keyvaluepair.h>
#include <xan_uint64.h>
#include <xan_boolean.h>
#include <xan_property_info.h>
#include <xan_string.h>
begin_implement_rux_class_ns( Binding , rux );
	_object = 0;
	_property_name = 0;
end_implement_rux_class();
namespace rux
{	
	implement_duplicate_internal_result_function( Object& , Binding , GetValue );
	implement_duplicate_internal_function_1( Binding , set_Object , const XObject& );
	implement_duplicate_internal_function_1( Binding , set_Object , XGCRef* );
	implement_duplicate_internal_function_1( Binding , set_PropertyName , const char* );
	Binding::~Binding( void )
	{
		if( _object )
			_object->Release();
		if( _property_name )
			::rux::engine::free_mem( _property_name );
	};
	Object& Binding::GetValue( void )
	{
		if( _object && _property_name )
		{
			Type* type = &_object->get_Type();
			::rux::PropertyInfo* property_info = type->get_PropertyInfo( _property_name );
			if( property_info )
			{
				XObject property_obj( property_info->Get( _object ) , "obj" , __FILE__ , __LINE__ );
				return property_obj++;
			}
		}
		return XObject( "" , __FILE__ , __LINE__ )++;
	};
	void Binding::set_Object( const XObject& object )
	{
		set_Object( object.get_GCRef() );
	};
	void Binding::set_Object( XGCRef* object )
	{
		if( _object )
			_object->Release();
		_object = object;
		_object->AddRef();
	};
	void Binding::set_PropertyName( const char* utf8propertyname )
	{
		if( _property_name )
			::rux::engine::free_mem( _property_name );
		::rux::XString utf8( utf8propertyname , XEnumCodePage_UTF8 );
		_property_name = utf8()->detach();
	};
};