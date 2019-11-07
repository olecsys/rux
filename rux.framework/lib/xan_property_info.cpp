#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_property_info.h"
#include "xan_keyvaluepair.h"
#include "xan_uint64.h"
#include "xan_boolean.h"
begin_implement_rux_class_ns( PropertyInfo , rux );
end_implement_rux_class();
namespace rux
{
	implement_duplicate_internal_function_2( PropertyInfo , Set , const XObject& , const XObject& );
	implement_duplicate_internal_function_2( PropertyInfo , Set , const XGCRef* , const XGCRef* );
	void PropertyInfo::Set( const XObject& object , const XObject& param )
	{
		Set( object.get_GCRef() , param.get_GCRef() );
	};
	void PropertyInfo::Set( const XGCRef* object , const XGCRef* param )
	{
		if( _set_property_address )
		{
			if( _object_type && _object_type->_type_index == object->get_RuxTypeIndex() )
			{
				if( param )
				{				
					::rux::uint32 type_index = param->get_RuxTypeIndex();
					if( _param_type == 0 || ( type_index == _param_type->_type_index
						|| ( _param_type->get_IsArray() && param->get_Type().get_IsArray()
						&& ( _param_type->get_TypeIndex() == ::rux::get_array_object_rux_type_index()
						|| ::rux::is_boolean_and_true( &param->get_Type().get_FunctionInfo( ::rux::compile::times33_hash< 'C','h','e','c','k','A','l','l','I','t','e','m','T','y','p','e' >::value )->InvokeResult( object , &_param_type->get_ArrayItemType() ) ) ) )
						|| ( _param_type->get_TypeIndex() == ::rux::get_numeric_rux_type_index()
						&& ( type_index == ::rux::get_float_rux_type_index()
						|| type_index == ::rux::get_double_rux_type_index()
						|| type_index == ::rux::get_int32_rux_type_index()
						|| type_index == ::rux::get_int64_rux_type_index()
						|| type_index == ::rux::get_uint32_rux_type_index()
						|| type_index == ::rux::get_uint64_rux_type_index() ) ) ) )
						_set_property_address( object , param );
				}
			}
		}
	};
	implement_duplicate_internal_result_function_1( XGCRef& , PropertyInfo , Get , const XObject& );
	implement_duplicate_internal_result_function_1( XGCRef& , PropertyInfo , Get , const XGCRef& );
	implement_duplicate_internal_result_function_1( XGCRef& , PropertyInfo , Get , const XGCRef* );
	XGCRef& PropertyInfo::Get( const XObject& object )
	{		
		return Get( object.get_GCRef() );
	};
	XGCRef& PropertyInfo::Get( const XGCRef& object )
	{
		return Get( &object );
	};
	XGCRef& PropertyInfo::Get( const XGCRef* object )
	{
		if( _get_property_address )
		{
			if( _object_type && _object_type->_type_index == object->get_RuxTypeIndex() )
				return _get_property_address( object );
		}
		return XObject( "" , __FILE__ , __LINE__ )++;
	};
	implement_duplicate_internal_result_function( const char* , PropertyInfo , get_Name );
	const char* PropertyInfo::get_Name( void )
	{
		return ::rux::engine::_globals->get_time33_hash_value( _name_hash );
	};
};