#ifndef XAN_TYPEMANAGER_DEFINES_H
#define XAN_TYPEMANAGER_DEFINES_H
#include "xan_type.h"
dll_internal Type* _typeof( const char* class_name , rux_volatile* rux_type_index , ::rux::register_type_t register_type , ::rux::uint8 is_array , Type* item_type );
#define typeof( xclass ) *_typeof( xclass::get_Alias() , &xclass::InternalClass::_rux_type_index , xclass::InternalClass::register_type , xclass::InternalClass::_rux_is_array , xclass::InternalClass::get_rux_array_item_type() )
#define typeofptr( xclass ) _typeof( xclass::get_Alias() , &xclass::InternalClass::_rux_type_index , xclass::InternalClass::register_type , xclass::InternalClass::_rux_is_array , xclass::InternalClass::get_rux_array_item_type() )
#define rux_is_class( xclass0 , xclass1 ) ( xclass0::get_static_RuxTypeIndex() == xclass1::get_static_RuxTypeIndex() )
#define rux_typeptr_is_class( type , xclass ) ( type->get_TypeIndex() == xclass::get_static_RuxTypeIndex() )
#define rux_same_types_of_objects( xobject0 , xobject1 ) ( xobject0.get_RuxTypeIndex() == xobject1.get_RuxTypeIndex() \
	|| ( xobject1.get_Type().get_IsArray() && xobject0.get_Type().get_IsArray()\
	&& ( xobject1.get_RuxTypeIndex() == ::rux::get_array_object_rux_type_index()\
	|| ::rux::is_boolean_and_true( &xobject0.get_Type().get_FunctionInfo( ::rux::compile::times33_hash< 'C','h','e','c','k','A','l','l','I','t','e','m','T','y','p','e' >::value )->InvokeResult( xobject0 , &xobject1.get_Type().get_ArrayItemType() ) ) ) )\
	|| ( xobject1.get_RuxTypeIndex() == ::rux::get_numeric_rux_type_index()\
	&& ( xobject0.get_RuxTypeIndex() == ::rux::get_float_rux_type_index()\
	|| xobject0.get_RuxTypeIndex() == ::rux::get_double_rux_type_index()\
	|| xobject0.get_RuxTypeIndex() == ::rux::get_int32_rux_type_index()\
	|| xobject0.get_RuxTypeIndex() == ::rux::get_int64_rux_type_index()\
	|| xobject0.get_RuxTypeIndex() == ::rux::get_uint32_rux_type_index()\
	|| xobject0.get_RuxTypeIndex() == ::rux::get_uint64_rux_type_index() ) ) )
#define rux_is_object( object , xclass ) ( object.get_RuxTypeIndex() == xclass::get_static_RuxTypeIndex() \
	|| ( xclass::InternalClass::_rux_is_array && object.get_Type().get_IsArray()\
	&& ( xclass::get_static_RuxTypeIndex() == ::rux::get_array_object_rux_type_index()\
	|| ::rux::is_boolean_and_true( &object.get_Type().get_FunctionInfo( ::rux::compile::times33_hash< 'C','h','e','c','k','A','l','l','I','t','e','m','T','y','p','e' >::value )->InvokeResult( object , xclass::InternalClass::get_rux_array_item_type() ) ) ) )\
	|| ( xclass::get_static_RuxTypeIndex() == ::rux::get_numeric_rux_type_index()\
	&& ( object.get_RuxTypeIndex() == ::rux::get_float_rux_type_index()\
	|| object.get_RuxTypeIndex() == ::rux::get_double_rux_type_index()\
	|| object.get_RuxTypeIndex() == ::rux::get_int32_rux_type_index()\
	|| object.get_RuxTypeIndex() == ::rux::get_int64_rux_type_index()\
	|| object.get_RuxTypeIndex() == ::rux::get_uint32_rux_type_index()\
	|| object.get_RuxTypeIndex() == ::rux::get_uint64_rux_type_index() ) ) )
#define rux_is_object_and_type_equal( object , type ) ( object.get_RuxTypeIndex() == type->get_TypeIndex()\
	|| ( type->get_IsArray() && object.get_Type().get_IsArray()\
	&& ( type->get_TypeIndex() == ::rux::get_array_object_rux_type_index()\
	|| ::rux::is_boolean_and_true( &object.get_Type().get_FunctionInfo( ::rux::compile::times33_hash< 'C','h','e','c','k','A','l','l','I','t','e','m','T','y','p','e' >::value )->InvokeResult( object , &type->get_ArrayItemType() ) ) ) )\
	|| ( type->get_TypeIndex() == ::rux::get_numeric_rux_type_index()\
	&& ( object.get_RuxTypeIndex() == ::rux::get_float_rux_type_index()\
	|| object.get_RuxTypeIndex() == ::rux::get_double_rux_type_index()\
	|| object.get_RuxTypeIndex() == ::rux::get_int32_rux_type_index()\
	|| object.get_RuxTypeIndex() == ::rux::get_int64_rux_type_index()\
	|| object.get_RuxTypeIndex() == ::rux::get_uint32_rux_type_index()\
	|| object.get_RuxTypeIndex() == ::rux::get_uint64_rux_type_index() ) ) )
#define rux_is_object_ptr( object_ptr , xclass ) ( object_ptr\
	? ( object_ptr->get_RuxTypeIndex() == xclass::get_static_RuxTypeIndex()\
	|| ( xclass::InternalClass::_rux_is_array && object_ptr->get_Type().get_IsArray()\
	&& ( xclass::get_static_RuxTypeIndex() == ::rux::get_array_object_rux_type_index()\
	|| ::rux::is_boolean_and_true( &object_ptr->get_Type().get_FunctionInfo( ::rux::compile::times33_hash< 'C','h','e','c','k','A','l','l','I','t','e','m','T','y','p','e' >::value )->InvokeResult( object_ptr , xclass::InternalClass::get_rux_array_item_type() ) ) ) )\
	|| ( xclass::get_static_RuxTypeIndex() == ::rux::get_numeric_rux_type_index()\
	&& ( object_ptr->get_RuxTypeIndex() == ::rux::get_float_rux_type_index()\
	|| object_ptr->get_RuxTypeIndex() == ::rux::get_double_rux_type_index()\
	|| object_ptr->get_RuxTypeIndex() == ::rux::get_int32_rux_type_index()\
	|| object_ptr->get_RuxTypeIndex() == ::rux::get_int64_rux_type_index()\
	|| object_ptr->get_RuxTypeIndex() == ::rux::get_uint32_rux_type_index()\
	|| object_ptr->get_RuxTypeIndex() == ::rux::get_uint64_rux_type_index() ) ) ) : false )
#endif
