#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_object.h"
#include "xan_typemanager_defines.h"
#include "xan_keyvaluepair.h"
#include "xan_string.h"
#include "xan_uint64.h"
#include "xan_boolean.h"
implement_rux_base_methods_for_object( Object );
Object::Object( XGCRef* gcref )
	: XGCRef( Object::get_static_Type )
{	
	_gc_ref_obj = gcref;
};
Object::~Object()
{
	if( _gc_ref_obj )
		_gc_ref_obj->Release( __FILE__ , __LINE__ );
};
Type* Object::get_rux_array_item_type( void )
{
	return NULL;
};
const char* Object::ToString( void )
{
	if( _gc_ref_obj )
	{
		::rux::String* str = _gc_ref_obj->ObjectToString();
		if( str )
			return str->str();
	}
	return "object";
};
void Object::register_type( Type* )
{
};
::rux::byte Object::IsObjectType( void ) const
{
	return 1;
};
::rux::byte XObject::IsObjectType( void ) const
{
	return 1;
};
XGCRef* Object::get_GCRefObj( void ) const
{
	return _gc_ref_obj;
};
rux::uint32 Object::get_RuxTypeIndex( void ) const
{
	if( _gc_ref_obj )
		return _gc_ref_obj->get_RuxTypeIndex();	
	else	
	{
		if( XInterlocked::CompareExchange( &_rux_type_index , 0 , 0 ) == rux_volatile_max )
			_typeof( "Object" , &_rux_type_index , register_type , Object::_rux_is_array , Object::get_rux_array_item_type() );
		return XInterlocked::CompareExchange( &_rux_type_index , 0 , 0 );
	}
};
XObject::XObject( const char* variable_name , const char* __file__ , ::rux::int32 __line__ )
{
	_gc_ref = NULL;
	set_Info( variable_name , __file__ , __line__ );
};
XObject::XObject( const XObject& object , const char* variable_name , const char* __file__ , ::rux::int32 __line__ )
{
	XGCRef* object_gc_ref = object.get_GCRef( 1 , 1 );
	if( object_gc_ref )
		object_gc_ref->AddRef( __FILE__ , __LINE__ );
	_gc_ref = object_gc_ref;
	set_Info( variable_name , __file__ , __line__ );
};
void XObject::set_Info( const char* variable_name , const char* __file__ , ::rux::int32 __line__ )
{
#if gcref_variable_name_on
	_variable_name[ 0 ] = 0;
	if( variable_name )
	{
		if( strlen( variable_name ) > 31 )
		{
			::rux::engine::memcpy( _variable_name , 0 , variable_name , 31 );
			_variable_name[ 31 ] = 0;
		}
		else
			strcpy( _variable_name , variable_name );	
	}
#endif
	if( __file__ )
	{
		_filename = __file__;
		_line = __line__;
	}
	else
	{
		_filename = __FILE__;
		_line = __LINE__;
	}
};
XObject::XObject( const XGCRef& gc_ref , const char* variable_name , const char* __file__ , ::rux::int32 __line__ )
{
	gc_ref.AddRef( __FILE__ , __LINE__ );
	_gc_ref = const_cast< XGCRef* >( &gc_ref );
	if( _gc_ref->IsObjectType() && IsObjectType() == 0 )
	{
		Object* object = (Object*)_gc_ref->get_ObjectAddress();
		XGCRef* object_gc_ref = object->_gc_ref_obj;
		if( object_gc_ref )
			object_gc_ref->AddRef( __FILE__ , __LINE__ );	
		_gc_ref = object_gc_ref;
		object->Release( __FILE__ , __LINE__ );
	}
	set_Info( variable_name , __file__ , __line__ );
};
XObject::XObject( const Object& object , const char* variable_name , const char* __file__ , ::rux::int32 __line__ )
{	
	XGCRef* object_gc_ref = object._gc_ref_obj;
	if( object_gc_ref )
		object_gc_ref->AddRef( __FILE__ , __LINE__ );	
	_gc_ref = object_gc_ref;
	set_Info( variable_name , __file__ , __line__ );
};
XObject::XObject( const XGCRef* gc_ref , ::rux::byte copy , const char* variable_name , const char* __file__ , ::rux::int32 __line__ )
{
	if( gc_ref )
	{
		if( copy )
			gc_ref->AddRef( __FILE__ , __LINE__ );
		_gc_ref = const_cast< XGCRef* >( gc_ref );
		if( _gc_ref->IsObjectType() && IsObjectType() == 0 )
		{
			Object* object = (Object*)_gc_ref->get_ObjectAddress();
			XGCRef* object_gc_ref = object->_gc_ref_obj;
			if( copy && object_gc_ref )
				object_gc_ref->AddRef( __FILE__ , __LINE__ );	
			_gc_ref = object_gc_ref;
			if( copy )
				object->Release( __FILE__ , __LINE__ );
		}
	}
	else
		_gc_ref = NULL;
	set_Info( variable_name , __file__ , __line__ );
};
XObject::~XObject()
{	
	if( _gc_ref )
		_gc_ref->Release( __FILE__ , __LINE__ );
};
bool XObject::Equals( XObject& obj )
{
	XGCRef* object_gc_ref = obj.get_GCRef( 0 , 1 );
	XGCRef* this_gc_ref = get_GCRef( 0 , 1 );
	if( this_gc_ref )
	{
		if( object_gc_ref )
		{
			bool result = this_gc_ref->Equals( object_gc_ref );
			return result;
		}
		else
			return false;
	}
	else
	{
		if( object_gc_ref )
			return false;
		else
			return false;
	}
};
bool XObject::operator ==( const XObject& object )
{
	XGCRef* object_gc_ref = object.get_GCRef( 0 , 1 );
	XGCRef* this_gc_ref = get_GCRef( 0 , 1 );
	if( this_gc_ref )
	{
		if( object_gc_ref )
		{
			bool result = this_gc_ref == object_gc_ref;
			return result;
		}
		else
			return false;
	}
	else
	{
		if( object_gc_ref )
			return false;
		else
			return false;
	}
};
bool XObject::operator ==( const Object& object ) const
{
	return *this == object._gc_ref_obj;
};
bool XObject::operator ==( const XGCRef& gc_ref ) const
{
	return *this == &gc_ref;
};
bool XObject::operator ==( const XGCRef* gc_ref ) const
{
	XGCRef* this_gc_ref = get_GCRef( 0 , 1 );
	if( this_gc_ref )
	{
		if( gc_ref )
		{
			bool result = this_gc_ref == gc_ref;
			return result;
		}
		else
			return false;
	}
	else
	{
		if( gc_ref )
			return false;
		else
			return false;
	}
};
XObject& XObject::operator=( const XObject& object ) const
{	
	XGCRef* object_gc_ref = object.get_GCRef( 1 , 1 );	
	if( object_gc_ref )
		object_gc_ref->AddRef( __FILE__ , __LINE__ );		
	if( _gc_ref )
		_gc_ref->Release( __FILE__ , __LINE__ );
	XObject* this_obj = const_cast< XObject* >( this );
	this_obj->_gc_ref = object_gc_ref;
	return *this_obj;
};
XObject& XObject::operator=( const XGCRef* gc_ref ) const
{
	XObject* this_obj = const_cast< XObject* >( this );
	gc_ref->AddRef( __FILE__ , __LINE__ );	
	if( _gc_ref )
		_gc_ref->Release( __FILE__ , __LINE__ );
	this_obj->_gc_ref = const_cast< XGCRef* >( gc_ref );
	if( _gc_ref->IsObjectType() && IsObjectType() == 0 )
	{
		Object* object = (Object*)_gc_ref->get_ObjectAddress();
		XGCRef* object_gc_ref = object->_gc_ref_obj;
		if( object_gc_ref )
			object_gc_ref->AddRef( __FILE__ , __LINE__ );	
		this_obj->_gc_ref = object_gc_ref;
		object->Release( __FILE__ , __LINE__ );
	}
	return *this_obj;
};
XObject& XObject::operator=( const XGCRef& gc_ref ) const
{
	*this = const_cast< XGCRef* >( &gc_ref );
	return *( const_cast< XObject* >( this ) );
};
XObject& XObject::operator=( const Object& object ) const
{
	XObject* this_obj = const_cast< XObject* >( this );
	XGCRef* object_gc_ref = object._gc_ref_obj;
	if( object_gc_ref )
		object_gc_ref->AddRef( __FILE__ , __LINE__ );
	if( _gc_ref )
		_gc_ref->Release( __FILE__ , __LINE__ );
	this_obj->_gc_ref = object_gc_ref;
	return *this_obj;
};
XObject& XObject::operator=(::rux::int32 null_ptr) const
{
	XObject* this_obj = const_cast< XObject* >( this );
	if( null_ptr == 0 )
	{		
		if( _gc_ref )
		{
			_gc_ref->Release( __FILE__ , __LINE__ );
			this_obj->_gc_ref = NULL;
		}
	}
	return *this_obj;
};	
Object* XObject::operator ()() const
{
	XGCRef* gc_ref = get_GCRef( 0 , 1 );
	XObject* _this = const_cast< XObject* >( this );
	if( _this->_gc_ref == NULL )
		_this->_gc_ref = ::rux::engine::alloc_object< Object , XGCRef* >( gc_ref , (const char*)_filename , (::rux::uint32)_line );
	else if( _gc_ref->IsObjectType() == 0 )
		_this->_gc_ref = ::rux::engine::alloc_object< Object , XGCRef* >( gc_ref , (const char*)_filename , (::rux::uint32)_line );
	return static_cast< Object* >( _this->_gc_ref );
};	
Type& XObject::get_Type( void ) const
{
	return get_GCRef( 1 , 1 )->get_Type();
};
rux::uint32 XObject::get_RuxTypeIndex( void ) const
{
	return get_GCRef( 1 , 1 )->get_RuxTypeIndex();
};
XGCRef* XObject::get_GCRef( ::rux::uint8 create , ::rux::uint8 transform ) const
{
	if( transform )
	{
		if( create == 1 && _gc_ref == NULL )
		{
			const_cast< XObject* >( this )->_gc_ref = ::rux::engine::alloc_object< Object , XGCRef* >( (XGCRef*)NULL , (const char*)_filename , (::rux::uint32)_line );
			return _gc_ref;
		}
		else if( _gc_ref && _gc_ref->IsObjectType() )
		{
			if( static_cast< Object* >( _gc_ref )->_gc_ref_obj )
				return static_cast< Object* >( _gc_ref )->_gc_ref_obj;
			else
				return _gc_ref;
		}
		else if( _gc_ref )
			return _gc_ref;
		else
			return NULL;
	}
	else
	{
		XGCRef* gc_ref = get_GCRef( 0 , 1 );
		XObject* _this = const_cast< XObject* >( this );
		if( _this->_gc_ref == NULL )
			_this->_gc_ref = ::rux::engine::alloc_object< Object , XGCRef* >( gc_ref , (const char*)_filename , (::rux::uint32)_line );
		else if( _gc_ref->IsObjectType() == 0 )
			_this->_gc_ref = ::rux::engine::alloc_object< Object , XGCRef* >( gc_ref , (const char*)_filename , (::rux::uint32)_line );
		return _gc_ref;
	}
};
Object& XObject::operator++(::rux::int32) const
{
	XGCRef* gc_ref = get_GCRef( 0 , 1 );
	XObject* _this = const_cast< XObject* >( this );
	if( _this->_gc_ref == NULL )
		_this->_gc_ref = ::rux::engine::alloc_object< Object , XGCRef* >( gc_ref , (const char*)_filename , (::rux::uint32)_line );
	else if( _gc_ref->IsObjectType() == 0 )
		_this->_gc_ref = ::rux::engine::alloc_object< Object , XGCRef* >( gc_ref , (const char*)_filename , (::rux::uint32)_line );
	_gc_ref->set_Auto();	
	return ( *static_cast< Object* >( _gc_ref ) );	
};	
void XObject::set_ByRef( const XObject& object ) const
{
	*this = object;
};
void XObject::set_ByRef( const Object& object ) const
{
	*this = object;
};
void XObject::set_ByRef( const XGCRef& gcref ) const
{
	set_ByRef( const_cast< XGCRef* >( &gcref ) );
};
void XObject::set_ByRef( XGCRef* gcref ) const
{
	*this = gcref;
};
rux::String* XObject::ObjectToString( void )
{
	return get_GCRef( 1 , 1 )->ObjectToString();
};