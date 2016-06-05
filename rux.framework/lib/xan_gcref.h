#ifndef XAN_GCREF_H
#define XAN_GCREF_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_interlocked.h>
#include <xan_memorymanager_defines.h>
#include <xan_gc_defines.h>
#include <xan_malloc_array.h>
#include <xan_memory_helper.h>
class Type;
class XType;
namespace rux
{
	template< class T0 , ::rux::int32 step >
	class Array;
	class String;
	typedef Type* (*get_static_Type_t)( rux::uint32* rux_type_index );

	class RefHandle
	{
	private:
		::rux::get_static_Type_t _get_static_Type;
		rux_volatile _ref;
	};
};
class XGCRef
{	
	template< class T0 , ::rux::int32 step >
	friend class ::rux::Array;
	friend class XType;
	friend class XObject;	
	friend class Object;
	template< class T >
	friend class XGCHandle;
	friend class XMemoryManager;
	friend class XTypeManager;
	friend class XThreadInfo;
private:
	::rux::get_static_Type_t _get_static_Type;
	rux_volatile _ref;
// copy and assignment not allowed
	XGCRef( const XGCRef& );
	void operator =( const XGCRef& );
public:
	XGCRef( ::rux::get_static_Type_t get_static_Type );
	virtual ~XGCRef();	
	void AddRef( const char* file = __FILE__ , ::rux::int32 line = __LINE__ ) const;
	void Release( const char* file = __FILE__ , ::rux::int32 line = __LINE__ ) const;
	void set_Auto();
	Type& get_Type( void ) const;
	virtual ::rux::uint32 get_RuxTypeIndex( void ) const;
	virtual ::rux::String* ObjectToString( void );
	virtual ::rux::byte IsObjectType( void ) const;
	virtual const void* get_ObjectAddress( void ) const = 0;
	virtual void* DynamicCast( ::rux::int32 class_type ) const;
	virtual bool Equals( XGCRef* gcref );
};
class GCContainer
{
	friend class XTypeManager;
	template< class T >
	friend class XGCHandle;
	template< class T0 , ::rux::int32 step >
	friend class ::rux::Array;
	friend class XType;
	friend class XObject;
private:
	XGCRef* _gc_ref;
	GCContainer( void );
};
#endif
