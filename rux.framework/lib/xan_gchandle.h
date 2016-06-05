#ifndef XAN_GCHANDLE_H
#define XAN_GCHANDLE_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_object.h>
template< class T >
class XGCHandle : public XObject
{
	friend class XObject;
private:		
public:		
	typedef T InternalClass;	
	XGCHandle()
	{
	};		
	XGCHandle( const char* variable_name , const char* __file__ , ::rux::int32 __line__ )
		: XObject( variable_name , __file__ , __line__ )
	{
	};
	virtual ~XGCHandle()
	{
	};
	XGCHandle<T>& operator=( const XGCHandle<T>& gc_handle) const
	{
		*static_cast<const XObject*>( this ) = *static_cast<const XObject*>( &gc_handle );		
		return *(const_cast< XGCHandle< T >* >( this ));
	};	
	XGCHandle<T>& operator =( const XObject& object ) const
	{
		if( T::get_static_RuxTypeIndex() == object.get_RuxTypeIndex() )
			*static_cast<const XObject*>( this ) = object;
		else
			*static_cast<const XObject*>( this ) = 0;
		return *(const_cast< XGCHandle< T >* >( this ));
	};	
	XGCHandle<T>& operator =( const Object& object ) const
	{
		if( T::get_static_RuxTypeIndex() == object.get_RuxTypeIndex() )
			*static_cast<const XObject*>( this ) = object;
		else
			*static_cast<const XObject*>( this ) = 0;
		return *(const_cast< XGCHandle< T >* >( this ));
	};
	XGCHandle<T>& operator =( const XGCRef& gc_ref ) const
	{		
		*this = const_cast< XGCRef* >( &gc_ref );
		return *(const_cast< XGCHandle< T >* >( this ));
	};
	XGCHandle<T>& operator =( const XGCRef* gc_ref ) const
	{		
		if( T::get_static_RuxTypeIndex() == gc_ref->get_RuxTypeIndex() )
			*static_cast<const XObject*>( this ) = gc_ref;
		else
			*static_cast<const XObject*>( this ) = 0;
		return *(const_cast< XGCHandle< T >* >( this ));
	};	
	XGCHandle<T>& operator =( const T& object ) const
	{
		*static_cast<const XObject*>( this ) = *static_cast<const XGCRef*>( &object );
		return *(const_cast< XGCHandle< T >* >( this ));
	};	
	XGCHandle<T>& operator =( ::rux::int32 null_ptr )
	{
		*static_cast<XObject*>( this ) = null_ptr;
		return *(const_cast< XGCHandle< T >* >( this ));
	};
	T& operator++(::rux::int32 ) const
	{
		XGCRef* gcref = get_GCRef( 1 , 1 );
		gcref->set_Auto();
		return static_cast<T&>( *gcref );
	};
	T* operator ()() const
	{		
		return &static_cast<T&>( *get_GCRef( 1 , 1 ) );
	}	
	virtual XGCRef* get_GCRef( ::rux::uint8 create , ::rux::uint8 ) const
	{
		XGCHandle<T>* _this = const_cast<XGCHandle<T>* >( this );
		XObject* _this_object = cpp_ptr_cast( XObject* , _this );
		XGCRef* gc_ref = _this_object->_gc_ref;		
		if( gc_ref && gc_ref->IsObjectType() )
		{
			Object* obj_ptr = cpp_ptr_cast( Object* , gc_ref );
			XGCRef* gc = obj_ptr->_gc_ref_obj;
			obj_ptr->_gc_ref_obj = 0;
			gc_ref->Release( __FILE__ , __LINE__ );
			_this_object->_gc_ref = gc;
			gc_ref = gc;
		}
		if( gc_ref == NULL && create == 1 )
		{			
#if gcref_variable_name_on
			gc_ref = cpp_ptr_cast( XGCRef* , ( ::rux::engine::alloc_object< T , const char* , const char* , ::rux::int32 >( _variable_name , _filename , _line , (const char*)_filename , (::rux::uint32)_line , (char*)_variable_name ) ) );
#else
			gc_ref = cpp_ptr_cast( XGCRef* , ( ::rux::engine::alloc_object< T , const char* , const char* , ::rux::int32 >( 0 , _filename , _line , (const char*)_filename , (::rux::uint32)_line ) ) );
#endif
			_this_object->_gc_ref = gc_ref;
		}
		return gc_ref;
	}	
	void set_ByRef( const XGCHandle<T>& gc_handle ) const
	{
		(*(static_cast<XGCHandle< T >*>( const_cast<XGCHandle< T >*>( this ) ))) = gc_handle;
	};
	void set_ByRef( const T& object ) const
	{
		(*(static_cast<XGCHandle< T >*>( const_cast<XGCHandle< T >*>( this ) ))) = object;
	};
	void set_ByRef( const XObject& object ) const
	{
		(*(static_cast<XGCHandle< T >*>( const_cast<XGCHandle< T >*>( this ) ))) = object;
	};
	void set_ByRef( const Object& object ) const
	{
		(*(static_cast<XGCHandle< T >*>( const_cast<XGCHandle< T >*>( this ) ))) = object;
	};
	void set_ByRef( const XGCRef& gcref ) const
	{
		(*(static_cast<XGCHandle< T >*>( const_cast<XGCHandle< T >*>( this ) ))) = gcref;
	};
	void set_ByRef( const XGCRef* gcref ) const
	{
		(*(static_cast<XGCHandle< T >*>( const_cast<XGCHandle< T >*>( this ) ))) = const_cast< XGCRef* >( gcref );
	};
	virtual Type& get_Type( void ) const
	{
		XGCRef* gc_ref = get_GCRef( 1 , 1 );
		if( gc_ref != NULL )
			return gc_ref->get_Type();
		else
			return *_typeof( T::get_Alias() , &T::_rux_type_index , T::register_type , T::_rux_is_array , T::get_rux_array_item_type() );
	};
	::rux::byte IsObjectType( void ) const
	{
		return 0;
	};
};
#endif
