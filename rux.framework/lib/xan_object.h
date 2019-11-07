#ifndef XAN_OBJECT_H
#define XAN_OBJECT_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_typemanager_defines.h"
namespace rux
{
	class String;
	namespace data
	{
		class XJsonSerializer;
		class DataObject;
		struct key_value_pair_t;
	};
};
#define gcref_variable_name_on 0
class Object : public XGCRef
{
	declare_rux_base_internal_methods();
	template< class T0 >
	friend class XGCHandle;
	friend class XObject;
	friend class ::rux::data::XJsonSerializer;
	friend class ::rux::data::DataObject;
	friend struct ::rux::data::key_value_pair_t;
	friend dll_internal bool ::rux::is_boolean_and_true( XGCRef* object );
	friend dll_internal ::rux::uint64 rux::object_to_uint64( Object* object );
	friend dll_internal ::rux::uint64 gcref_to_uint64( const XGCRef* object );
private:	
	XGCRef* _gc_ref_obj;	
public:
	explicit Object( XGCRef* gcref );	
	~Object();
	virtual ::rux::uint32 get_RuxTypeIndex( void ) const;
	virtual ::rux::byte IsObjectType( void ) const;
	XGCRef* get_GCRefObj( void ) const;
	const char* ToString( void );
};
class XObject : public GCContainer
{
	declare_rux_base_methods_for_object();
	template< class T >
	friend class XGCHandle;
public:
#if gcref_variable_name_on
	char _variable_name[ 32 ];	
#endif
	const char* _filename;
	::rux::int32 _line;
	typedef Object InternalClass;
	void set_Info( const char* variable_name , const char* __file__ , ::rux::int32 __line__ );
	XObject( const char* variable_name = 0 , const char* __file__ = 0 , ::rux::int32 __line__ = 0 );
	explicit XObject( const XObject& object , const char* variable_name = 0 , const char* __file__ = 0 , ::rux::int32 __line__ = 0 );
	explicit XObject( const XGCRef& gc_ref , const char* variable_name = 0 , const char* __file__ = 0 , ::rux::int32 __line__ = 0 );
	explicit XObject( const XGCRef* gc_ref , ::rux::byte copy , const char* variable_name = 0 , const char* __file__ = 0 , ::rux::int32 __line__ = 0 );
	explicit XObject( const Object& object , const char* variable_name = 0 , const char* __file__ = 0 , ::rux::int32 __line__ = 0 );
	virtual ~XObject();
	bool operator ==( const XObject& object );
	bool operator ==( const Object& object ) const;
	bool operator ==( const XGCRef* gc_ref ) const;
	bool operator ==( const XGCRef& gc_ref ) const;
	XObject& operator=( const XObject& object ) const;
	XObject& operator=( const XGCRef& gc_ref ) const;
	XObject& operator=( const XGCRef* gc_ref ) const;
	XObject& operator=( const Object& object ) const;
	XObject& operator=(::rux::int32 null_ptr) const;	
	Object* operator ()() const;
	virtual ::rux::uint32 get_RuxTypeIndex( void ) const;
	virtual Type& get_Type( void ) const;		
	Object& operator++(::rux::int32) const;
	template<class T>
	T* Cast()
	{
		if( get_GCRef( 1 , 1 ) )	
			return static_cast<T*>( get_GCRef( 1 , 1 ) );	
		else	
			return NULL;	
	};
	virtual XGCRef* get_GCRef( ::rux::uint8 create = 1 , ::rux::uint8 transform = 1 ) const;
	void set_ByRef( const XObject& type ) const;
	void set_ByRef( const Object& type ) const;
	void set_ByRef( const XGCRef& type ) const;
	void set_ByRef( XGCRef* type ) const;
	::rux::String* ObjectToString();
	virtual ::rux::byte IsObjectType( void ) const;
	bool Equals( XObject& obj );
};
#endif
