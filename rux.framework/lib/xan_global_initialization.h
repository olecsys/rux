#ifndef XAN_GLOBAL_INITIALIZATION_H
#define XAN_GLOBAL_INITIALIZATION_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_includes.h>
namespace rux
{
	template< class T >
	dll_internal T* new_object( void )
	{	
		return new ( _aligned_malloc( sizeof( T ) , 32 ) )T();
	};
	template< class T >
	dll_internal void delete_object( T* object )
	{
		object->~T();
#ifdef __WINDOWS__
		_aligned_free( object );
#else
		::free( object );
#endif
	};		
	class global_initialization
	{
	public:
		global_initialization();
		~global_initialization();
	};
};
#endif