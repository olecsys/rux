#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_type.h"
#include "xan_asm.h"
#include "xan_keyvaluepair.h"
#include "xan_property_info.h"
#include "xan_constructor_info.h"
#include "xan_function_info.h"
#include "xan_uint64.h"
#include "xan_boolean.h"
#include "xan_log.h"
implement_rux_base_methods_for_object( Type );
implement_ctor_and_register_type( Type );
Type::Type( void )
	: XGCRef( get_static_Type )
{
	_type_module_index = 0; 
	_type_hash = 0;
	_constructor_info = NULL;
	_is_array = 0;
	_array_item_type = NULL;
	_type_index = 0;
};
Type::~Type()
{
	for( size_t index0 = 0 ; index0 < _function_infos.Count() ; index0++ )
		_function_infos[ index0 ]->Release();
	for( size_t index0 = 0 ; index0 < _property_infos.Count() ; index0++ )
		_property_infos[ index0 ]->Release();
	if( _constructor_info )
		_constructor_info->Release();
};
const char* Type::get_TypeName( void )
{
	return ::rux::engine::_globals->get_time33_hash_value( _type_hash );
};
void Type::RegisterConstructor( void* ctor_address )
{
	if( _rux_is_memory_manager )
	{
		if( ctor_address )
		{
			if( get_ConstructorInfo() == NULL )
			{
				::rux::XConstructorInfo ctor_info;
				_constructor_info = ctor_info();
				_constructor_info->_object_type = this;
				_constructor_info->_ctor = (::rux::ctor_t)ctor_address;
				_constructor_info->AddRef();
			}
		}
	}
	else
		::rux::engine::_globals->_register_constructor( this , ctor_address );	
};
void Type::RegisterFunction( ::rux::uint8 with_return , const char* name ,
		void* function_address ,
		Type* param_type0 )
{
	if( _rux_is_memory_manager )
	{
		if( name && get_FunctionInfo( name ) == NULL )
		{
			::rux::XFunctionInfo function_info;
			::rux::FunctionInfo* function_info_ptr = function_info();
			function_info_ptr->_name_hash = ::rux::engine::_globals->add_times33_hash( name , SIZE_MAX );
			function_info_ptr->_object_type = this;
			function_info_ptr->_param_types.Add( param_type0 );
			function_info_ptr->_with_return = with_return;
			function_info_ptr->_function_address = function_address;
			function_info_ptr->AddRef();
			_function_infos.Add( function_info_ptr );
		}
	}
	else
		::rux::engine::_globals->_register_function_1( this , with_return , name , function_address , param_type0 );
};
void Type::RegisterProperty( const char* name ,
		Type* param_type ,
		void* set_property_address ,
		void* get_property_address )
{
	if( _rux_is_memory_manager )
	{
		if( name && get_PropertyInfo( name ) == NULL )
		{
			::rux::XPropertyInfo property_info;
			::rux::PropertyInfo* property_info_ptr = property_info();
			property_info_ptr->_name_hash = ::rux::engine::_globals->add_times33_hash( name , SIZE_MAX );
			property_info_ptr->_object_type = this;
			property_info_ptr->_param_type = param_type;
			property_info_ptr->_set_property_address = (::rux::set_property_t)set_property_address;
			property_info_ptr->_get_property_address = (::rux::get_property_t)get_property_address;
			property_info_ptr->AddRef();
			_property_infos.Add( property_info_ptr );
		}
	}
	else
		::rux::engine::_globals->_register_property( this , name , param_type , set_property_address , get_property_address );
};
::rux::uint8 Type::get_IsArray( void )
{
	return _is_array;
};
::rux::uint8 XType::get_IsArray( void )
{
	return (*this)()->get_IsArray();
};
Type& XType::get_ArrayItemType( void )
{
	return (*this)()->get_ArrayItemType();
};
void XType::set_ModuleIndex( ::rux::int32 module_index )
{
	(*this)()->set_ModuleIndex( module_index );
};
::rux::int32 XType::get_ModuleIndex( void )
{
	return (*this)()->get_ModuleIndex();
};
Type& XType::get_Type( void )
{
	return *_typeof( Type::get_Alias() , &Type::_rux_type_index , Type::register_type , Type::_rux_is_array , Type::get_rux_array_item_type() );
};
Type& Type::get_ArrayItemType( void )
{
	XType type;
	if( _array_item_type )
		type = *_array_item_type;
	return type++;
};
::rux::FunctionInfo* XType::get_FunctionInfo( const char* function_name )
{
	return (*this)()->get_FunctionInfo( function_name );
};
::rux::FunctionInfo* XType::get_FunctionInfo( ::rux::uint32 function_name_hash )
{
	return (*this)()->get_FunctionInfo( function_name_hash );
};
::rux::FunctionInfo* Type::get_FunctionInfo( ::rux::uint32 function_name_hash )
{
	for( size_t index0 = 0 ; index0 < _function_infos.Count() ; index0++ )
	{
		if( _function_infos[ index0 ]->_name_hash == function_name_hash )
			return _function_infos[ index0 ];
	}
	return 0;
};
::rux::FunctionInfo* Type::get_FunctionInfo( const char* function_name )
{
	if( function_name )
		return get_FunctionInfo( ::rux::cryptography::hash::times33_hash( function_name , SIZE_MAX ) );
	return 0;
};
XMallocArray< ::rux::PropertyInfo* >* XType::get_Properties( void )
{
	return (*this)()->get_Properties();
};
XMallocArray< ::rux::PropertyInfo* >* Type::get_Properties( void )
{
	return &_property_infos;
};
::rux::PropertyInfo* XType::get_PropertyInfo( const char* property_name )
{
	return (*this)()->get_PropertyInfo( property_name );
};
::rux::PropertyInfo* XType::get_PropertyInfo( ::rux::uint32 times33_hash )
{
	return (*this)()->get_PropertyInfo( times33_hash );
};
::rux::PropertyInfo* Type::get_PropertyInfo( ::rux::uint32 times33_hash )
{
	for( size_t index0 = 0 ; index0 < _property_infos.Count() ; index0++ )
	{
		if( _property_infos[ index0 ]->_name_hash == times33_hash )
			return _property_infos[ index0 ];
	}
	return 0;
};
::rux::PropertyInfo* Type::get_PropertyInfo( const char* property_name )
{
	if( property_name )
		return get_PropertyInfo( ::rux::cryptography::hash::times33_hash( property_name , SIZE_MAX ) );
	return 0;
};
::rux::ConstructorInfo* XType::get_ConstructorInfo( void )
{
	return (*this)()->get_ConstructorInfo();
};
::rux::ConstructorInfo* Type::get_ConstructorInfo( void )
{	
	return _constructor_info;
};
XType::XType( const char* , const char* , ::rux::int32 )
{
	_gc_ref = 0;
};
XType::XType( XType& type )
{
	_gc_ref = 0;
	(*this) = type;
};
XType::XType( Type& type , const char* , const char* , ::rux::int32 )
{	
	_gc_ref = 0;
	(*this) = type;	
};
XType::XType( const XGCRef& gc_ref , const char* , const char* , ::rux::int32 )
{
	_gc_ref = 0;
	(*this) = gc_ref;	
};
XType::~XType()
{	
	if( _gc_ref )
		_gc_ref->Release();
};
XType& XType::operator=( const XType& type )
{
	if( _gc_ref )
		_gc_ref->Release();
	if( type._gc_ref )
	{
		type._gc_ref->AddRef();
		_gc_ref = type._gc_ref;		
	}
	else
		_gc_ref = 0;	
	return *this;
};
XType& XType::operator=( const Type& type )
{
	type.AddRef();
	if( _gc_ref )
		_gc_ref->Release();
	_gc_ref = const_cast< Type* >( &type );
	return *this;
};
XType& XType::operator=( const XGCRef& type )
{
	if( type.get_RuxTypeIndex() == XType::get_static_RuxTypeIndex() )
	{
		if( _gc_ref )
			_gc_ref->Release();
		type.AddRef();	
		XGCRef* gcref = const_cast< XGCRef* >( &type );
		_gc_ref = static_cast< Type* >( gcref );
	}
	return *this;
};
XGCRef* XType::get_GCRef( ::rux::uint8 , ::rux::uint8 ) const
{
	XType* _this = const_cast< XType* >( this );
	if( _gc_ref == 0 )
		_this->_gc_ref = ::rux::engine::alloc_object< Type >( (const char*)__FILE__ , (::rux::uint32)__LINE__ );
	return _gc_ref;
};
Type* XType::operator ()() const
{		
	XType* _this = const_cast< XType* >( this );
	if( _gc_ref == 0 )
		_this->_gc_ref = ::rux::engine::alloc_object< Type >( (const char*)__FILE__ , (::rux::uint32)__LINE__ );
	return (Type*)_gc_ref;
};
Type& XType::operator++(::rux::int32) const
{	
	_gc_ref->set_Auto();
	return *(Type*)_gc_ref;
};	
void Type::set_ModuleIndex( ::rux::int32 module_index )
{
	if( _type_module_index == 0 )
		_type_module_index = module_index;	
};
rux::uint32 Type::get_TypeIndex( void )
{
	return _type_index;
};
rux::uint32 XType::get_TypeIndex( void )
{
	return (*this)()->_type_index;
};
const char* XType::get_TypeName( void )
{
	return ::rux::engine::_globals->get_time33_hash_value( (*this)()->_type_hash );
};
rux::int32 Type::get_ModuleIndex( void )
{
	return _type_module_index;
};
bool XType::operator ==( const XType& type )
{
	return (*this)()->_type_hash == type()->_type_hash;
};
bool operator ==(Type& type1,XType& type2)
{
	return type1._type_hash == type2()->_type_hash;
};
bool operator ==(XType& type1,Type& type2)
{
	return type1()->_type_hash == type2._type_hash;
};
bool operator ==(Type& type1,Type& type2)
{
	return type1._type_hash == type2._type_hash;
};
bool operator ==(XType& type1,XType& type2)
{
	return type1()->_type_hash == type2()->_type_hash;
};
bool operator !=(Type& type1,XType& type2)
{
	return type1._type_hash != type2()->_type_hash;
};
bool operator !=(XType& type1,Type& type2)
{
	return type1()->_type_hash != type2._type_hash;
};
bool operator !=(Type& type1,Type& type2)
{
	return type1._type_hash != type2._type_hash;
};
bool operator !=(XType& type1,XType& type2)
{
	return type1()->_type_hash != type2()->_type_hash;
};
void XType::set_ByRef( const XType& type ) const
{
	*const_cast< XType* >( this ) = type;
};
void XType::set_ByRef( const Type& type ) const
{
	* const_cast< XType* >( this ) = type;
};
void XType::set_ByRef( const XGCRef& gcref ) const
{
	*const_cast< XType* >( this ) = gcref;
};