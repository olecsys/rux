#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_function_info.h"
#include "xan_keyvaluepair.h"
#include "xan_uint64.h"
#include "xan_boolean.h"
begin_implement_rux_class_ns( FunctionInfo , rux );
	_object_type = NULL;
	_with_return = 0;
	_function_address = NULL;
end_implement_rux_class();
namespace rux
{
	implement_duplicate_internal_function_2( FunctionInfo , Invoke , XObject& , const XObject& );
	implement_duplicate_internal_function_2( FunctionInfo , Invoke , const XGCRef* , const XGCRef* );
	void FunctionInfo::Invoke( XObject& object , const XObject& param )
	{		
		Invoke( object.get_GCRef() , param.get_GCRef() );
	};
	void FunctionInfo::Invoke( const XGCRef* object , const XGCRef* param )
	{		
		if( _function_address )
		{
			if( _object_type && _object_type->_type_index == object->get_RuxTypeIndex() )
			{
				if( _param_types.Count() == 1 && _param_types[ 0 ]->_type_index == param->get_RuxTypeIndex() )
				{
					if( _with_return == 0 )
						((::rux::type::function_1_t)_function_address)( object , param );
				}
			}
		}
	};
	implement_duplicate_internal_result_function_2( XGCRef& , FunctionInfo , InvokeResult , const XObject& , const XObject& );
	implement_duplicate_internal_result_function_2( XGCRef& , FunctionInfo , InvokeResult , const XGCRef& , const XObject& );
	implement_duplicate_internal_result_function_2( XGCRef& , FunctionInfo , InvokeResult , const XGCRef* , const XGCRef* );
	implement_duplicate_internal_result_function_2( XGCRef& , FunctionInfo , InvokeResult , const XObject& , const XGCRef* );
	implement_duplicate_internal_result_function_2( XGCRef& , FunctionInfo , InvokeResult , const XGCRef& , const XGCRef* );
	XGCRef& FunctionInfo::InvokeResult( const XObject& object , const XObject& param )
	{
		return InvokeResult( object.get_GCRef() , param.get_GCRef() );
	};
	XGCRef& FunctionInfo::InvokeResult( const XObject& object , const XGCRef* param )
	{
		return InvokeResult( object.get_GCRef() , param );
	};
	XGCRef& FunctionInfo::InvokeResult( const XGCRef& object , const XObject& param )
	{
		return InvokeResult( &object , param.get_GCRef() );
	};
	XGCRef& FunctionInfo::InvokeResult( const XGCRef& object , const XGCRef* param )
	{
		return InvokeResult( &object , param );
	};
	XGCRef& FunctionInfo::InvokeResult( const XGCRef* object , const XGCRef* param )
	{
		if( _function_address )
		{
			if( _object_type && _object_type->_type_index == object->get_RuxTypeIndex() )
			{
				if( _param_types.Count() == 1 && _param_types[ 0 ]->_type_index == param->get_RuxTypeIndex() )
				{
					if( _with_return == 1 )
						return ((::rux::type::return_function_1_t)_function_address)( object , param );
				}
			}
		}
		return XObject( "" , __FILE__ , __LINE__ )++;
	};
};