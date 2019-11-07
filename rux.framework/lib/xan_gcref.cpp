#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_gcref.h"
#include "xan_typemanager_defines.h"
#include "xan_debug.h"
#include "xan_time.h"
#include "xan_memory_helper.h"
#include "xan_log.h"

GCContainer::GCContainer( void )
{
	_gc_ref = 0;
};
XGCRef::XGCRef( ::rux::get_static_Type_t get_static_Type )
{
	_get_static_Type = get_static_Type;
	_ref = 1;
};
XGCRef::~XGCRef()
{
};
void XGCRef::AddRef( const char* file , ::rux::int32 line ) const
{
	UNREFERENCED_PARAMETER( file );
	UNREFERENCED_PARAMETER( line );
	::rux::memory::check_memory( const_cast< XGCRef* >( this ) , 0 , sizeof( XGCRef ) );
	XInterlocked::Increment( const_cast<rux_volatile*>( &_ref ) );
};
void XGCRef::Release( const char* file , ::rux::int32 line ) const
{	
	UNREFERENCED_PARAMETER( file );
	UNREFERENCED_PARAMETER( line );
	::rux::memory::check_memory( const_cast< XGCRef* >( this ) , 0 , sizeof( XGCRef ) );
	if(	XInterlocked::Decrement( const_cast< rux_volatile* >( &_ref ) ) == 0 )
		::rux::engine::free_object<XGCRef>( const_cast< XGCRef* >( this ) );
};
void XGCRef::set_Auto()
{		
	::rux::memory::check_memory( this , 0 , sizeof( XGCRef ) );
	::rux::engine::add_gc_ref( this );
};
Type& XGCRef::get_Type( void ) const
{
	::rux::memory::check_memory( this , 0 , sizeof( XGCRef ) );
	return *_get_static_Type( 0 );
};
void* XGCRef::DynamicCast( ::rux::int32 ) const
{
	return 0;
};
bool XGCRef::Equals( XGCRef* gcref )
{
	return this == gcref;
};
rux::uint32 XGCRef::get_RuxTypeIndex( void ) const
{	
	::rux::memory::check_memory( const_cast< XGCRef* >( this ) , 0 , sizeof( XGCRef ) );
	::rux::uint32 rux_type_index = 0;
	_get_static_Type( &rux_type_index );
	return rux_type_index;
};
rux::String* XGCRef::ObjectToString( void )
{
	::rux::memory::check_memory( this , 0 , sizeof( XGCRef ) );
	XType type( get_Type() );
	return &rux::XString( type.get_TypeName() )++;
};
::rux::byte XGCRef::IsObjectType( void ) const
{
	return 0;
};
