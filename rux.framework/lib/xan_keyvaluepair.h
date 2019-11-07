#ifndef XAN_KEYVALUEPAIR_H
#define XAN_KEYVALUEPAIR_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_string.h"
#include "xan_hash.h"
namespace rux
{
	template < class T > class XKeyValuePair;
	template< class T >
	class KeyValuePair : public XGCRef
	{
		template< class T0 >
		friend class XKeyValuePair;
		friend class XJsonSerializer;
		friend class XXmlSerializer;
		declare_rux_base_internal_methods()
		{
			XKeyValuePair< T > obj;
			return obj++;
		};
	private:
		::rux::uint32 _name_hash;
		::rux::XString _name;
		T _value;
	public:	
		explicit KeyValuePair( const char* variable_name = 0 , const char* __file__ = 0 , ::rux::int32 __line__ = 0 )
			: XGCRef( get_static_Type )
		{		
			UNREFERENCED_PARAMETER( variable_name );
			UNREFERENCED_PARAMETER( __file__ );
			UNREFERENCED_PARAMETER( __line__ );
			_name_hash = 0ULL;
		};
		::rux::String& get_Name( void )
		{
			return _name++;
		};
		::rux::uint64 get_NameHash( void )
		{
			if( _name_hash == 0 )
			{
				_name.set_ByRef( _name.ConvertToUTF8() );
				_name_hash = ::rux::cryptography::hash::times33_hash( _name.str() , _name.Size() - 1 );
			}
			return _name_hash;
		};
		typename T::InternalClass& get_Value( void )
		{
			return _value++;
		};
		void set_Name( const ::rux::XString& name )
		{
			_name = name;
			_name.set_ByRef( _name.ConvertToUTF8() );
			_name_hash = ::rux::cryptography::hash::times33_hash( _name.str() , _name.Size() - 1 );
		};
		void set_Value( const T& value )
		{
			_value = value;
		};	
	};
	template< class T = XObject >
	class XKeyValuePair : public XGCHandle< ::rux::KeyValuePair< T > >
	{
	private:
	public:
		static Type* get_static_Type( rux::uint32* rux_type_index );
		static ::rux::uint32 get_static_RuxTypeIndex( void );
		XKeyValuePair( const char* variable_name = 0 , const char* __file__ = 0 , ::rux::int32 __line__ = 0 )
			: XGCHandle< ::rux::KeyValuePair< T > >( variable_name , __file__ , __line__ )
		{
		};
		XKeyValuePair( const ::rux::KeyValuePair< T >& keyvaluepair , const char* variable_name = 0 , const char* __file__ = 0 , ::rux::int32 __line__ = 0 )
			: XGCHandle< ::rux::KeyValuePair< T > >( variable_name , __file__ , __line__ )
		{
			(*this) = keyvaluepair;
		};
		XKeyValuePair( const XGCRef& gcref , const char* variable_name = 0 , const char* __file__ = 0 , ::rux::int32 __line__ = 0 )
			: XGCHandle< ::rux::KeyValuePair< T > >( variable_name , __file__ , __line__ )
		{
			(*this) = gcref;
		};
		explicit XKeyValuePair( const XObject& object )
		{
			*static_cast<XGCHandle< ::rux::KeyValuePair< T > >*>(this) = object;
		};
		XKeyValuePair( const ::rux::XKeyValuePair< T >& keyvaluepair )
		{
			(*(static_cast<XGCHandle<KeyValuePair< T > >*>( this ))) = keyvaluepair;
		};
		XKeyValuePair( const ::rux::XString& name , const T& value )
		{
			(*this)()->_name = name;
			(*this)()->_name.set_ByRef( (*this)()->_name.ConvertToUTF8() );
			(*this)()->_value = value;
			(*this)()->_name_hash = ::rux::cryptography::hash::times33_hash( (*this)()->_name.str() , (*this)()->_name.Size() - 1 );
		};
		XKeyValuePair( const ::rux::XString& name , const typename T::InternalClass& value )
		{
			(*this)()->_name = name;
			(*this)()->_name.set_ByRef( (*this)()->_name.ConvertToUTF8() );
			(*this)()->_value = value;
			(*this)()->_name_hash = ::rux::cryptography::hash::times33_hash( (*this)()->_name.str() , (*this)()->_name.Size() - 1 );
		};
		XKeyValuePair( const ::rux::XString& name , ::rux::uint8 is_name_set_by_ref , const T& value , ::rux::uint8 is_value_set_by_ref )
		{
			if( is_name_set_by_ref == 1 )
				(*this)()->_name.set_ByRef( name );
			else
				(*this)()->_name = name;
			(*this)()->_name.set_ByRef( (*this)()->_name.ConvertToUTF8() );
			if( is_value_set_by_ref == 1 )
				(*this)()->_value.set_ByRef( value );
			else
				(*this)()->_value = value;
			(*this)()->_name_hash = ::rux::cryptography::hash::times33_hash( (*this)()->_name.str() , (*this)()->_name.Size() - 1 );
		};
		XKeyValuePair( const ::rux::XString& name , ::rux::uint8 is_name_set_by_ref , typename T::InternalClass& value , ::rux::uint8 is_value_set_by_ref )
		{
			value.AddRef();
			if( is_name_set_by_ref == 1 )
				(*this)()->_name.set_ByRef( name );
			else
				(*this)()->_name = name;
			(*this)()->_name.set_ByRef( (*this)()->_name.ConvertToUTF8() );
			if( is_value_set_by_ref == 1 )
				(*this)()->_value.set_ByRef( value );
			else
				(*this)()->_value = value;
			(*this)()->_name_hash = ::rux::cryptography::hash::times33_hash( (*this)()->_name.str() , (*this)()->_name.Size() - 1 );
			value.Release();
		};
		XKeyValuePair& operator =( const XObject& object )
		{
			(*(static_cast<XGCHandle< ::rux::KeyValuePair< T > >*>( this ))) = object;
			return *this;
		};
		XKeyValuePair& operator =( const ::rux::KeyValuePair< T >& keyvaluepair )
		{
			(*(static_cast<XGCHandle< ::rux::KeyValuePair< T > >*>( this ))) = keyvaluepair;
			return *this;
		};
		XKeyValuePair& operator =( const ::rux::XKeyValuePair< T >& keyvaluepair )
		{
			(*(static_cast<XGCHandle< ::rux::KeyValuePair< T > >*>( this ))) = keyvaluepair;
			return *this;
		};
		XKeyValuePair& operator=( const XGCRef& gc_ref )
		{
			(*(static_cast<XGCHandle< ::rux::KeyValuePair< T > >*>( this ))) = gc_ref;
			return *this;
		};
		XKeyValuePair& operator=( const Object& object )
		{
			(*(static_cast<XGCHandle< ::rux::KeyValuePair< T > >*>( this ))) = object;
			return *this;
		};
		::rux::String& get_Name( void )
		{
			return (*this)()->_name++;
		};
		::rux::uint64 get_NameHash( void )
		{
			if( (*this)()->_name_hash == 0 )
			{
				(*this)()->_name.set_ByRef( (*this)()->_name.ConvertToUTF8() );
				(*this)()->_name_hash = ::rux::cryptography::hash::times33_hash( (*this)()->_name.str() , (*this)()->_name.Size() - 1 );
			}
			return (*this)()->_name_hash;
		};
		typename T::InternalClass& get_Value( void )
		{
			return (*this)()->_value++;
		};
		void set_Name( const ::rux::XString& name )
		{
			(*this)()->_name = name;
			(*this)()->_name.set_ByRef( (*this)()->_name.ConvertToUTF8() );
			(*this)()->_name_hash = ::rux::cryptography::hash::times33_hash( (*this)()->_name.str() , (*this)()->_name.Size() - 1 );
		};
		void set_Value( const T& value )
		{
			(*this)()->_value = value;
		};	
	};
};
#endif
