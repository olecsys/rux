#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_numeric.h"
#include <xan_keyvaluepair.h>
#include <xan_uint64.h>
#include <xan_boolean.h>
begin_implement_rux_class_without_set_operators_ns( Numeric , rux )
	_value_gcref = 0;
end_implement_rux_class();
namespace rux
{		
	Numeric::~Numeric( void )
	{
		if( _value_gcref )
			_value_gcref->Release();
	};
	XNumeric::XNumeric( double value )
	{
		this->operator()()->operator=( value );
	};
	XNumeric& XNumeric::operator =(const XDouble& value)
	{
		this->operator()()->operator=( value() );
		return *this;
	};
	XNumeric& XNumeric::operator =( double value )
	{
		this->operator()()->operator=( value );
		return *this;
	};
	XNumeric& XNumeric::operator =(const Double& value)
	{
		this->operator()()->operator=( value._value );
		return *this;
	};
	Numeric& Numeric::operator =(const Double& value)
	{
		this->operator=( value._value );
		return *this;
	};
	Numeric& Numeric::operator =(double value)
	{
		if( _value_gcref )
			_value_gcref->Release();
		::rux::Double* value_gcref = alloc_object_macros( ::rux::Double );
		value_gcref->_value = value;
		_value_gcref = value_gcref;
		return *this;
	};
	XNumeric::XNumeric( float value )
	{
		this->operator()()->operator=( value );
	};
	XNumeric& XNumeric::operator =(const XFloat& value)
	{
		this->operator()()->operator=( value() );
		return *this;
	};
	XNumeric& XNumeric::operator =( float value )
	{
		this->operator()()->operator=( value );
		return *this;
	};
	XNumeric& XNumeric::operator =(const ::rux::Float& value)
	{
		this->operator()()->operator=( value._value );
		return *this;
	};
	Numeric& Numeric::operator =(const ::rux::Float& value)
	{
		this->operator=( value._value );
		return *this;
	};
	Numeric& Numeric::operator =( float value )
	{
		if( _value_gcref )
			_value_gcref->Release();
		::rux::Float* value_gcref = alloc_object_macros( ::rux::Float );
		value_gcref->_value = value;
		_value_gcref = value_gcref;
		return *this;
	};
	XNumeric::XNumeric( ::rux::int32 value )
	{
		this->operator()()->operator=( value );
	};
	XNumeric& XNumeric::operator =(const ::rux::XInt32& value)
	{
		this->operator()()->operator=( value() );
		return *this;
	};
	XNumeric& XNumeric::operator =( ::rux::int32 value )
	{
		this->operator()()->operator=( value );
		return *this;
	};
	XNumeric& XNumeric::operator =(const ::rux::Int32& value)
	{
		this->operator()()->operator=( value._value );
		return *this;
	};
	Numeric& Numeric::operator =(const ::rux::Int32& value)
	{
		this->operator=( value._value );
		return *this;
	};
	XNumeric& XNumeric::operator =(const XNumeric& value)
	{
		operator()()->operator=( value() );
		return *this;
	};
	XNumeric& XNumeric::operator =(const Numeric& value)
	{
		operator()()->operator=( &value );
		return *this;
	};
	Numeric& Numeric::operator =(const Numeric* value)
	{
		if( _value_gcref )
		{
			_value_gcref->Release();
			_value_gcref = 0;
		}
		_value_gcref = value->_value_gcref;
		if( _value_gcref )
			_value_gcref->AddRef();
		return *this;
	};
	Numeric& Numeric::operator =( ::rux::int32 value )
	{
		if( _value_gcref )
			_value_gcref->Release();
		::rux::Int32* value_gcref = alloc_object_macros( ::rux::Int32 );
		value_gcref->_value = value;
		_value_gcref = value_gcref;
		return *this;
	};
	XNumeric::XNumeric( ::rux::uint32 value )
	{
		this->operator()()->operator=( value );
	};
	XNumeric& XNumeric::operator =(const ::rux::XUInt32& value)
	{
		this->operator()()->operator=( value() );
		return *this;
	};
	XNumeric& XNumeric::operator =( ::rux::uint32 value )
	{
		this->operator()()->operator=( value );
		return *this;
	};
	XNumeric& XNumeric::operator =(const ::rux::UInt32& value)
	{
		this->operator()()->operator=( value._value );
		return *this;
	};
	Numeric& Numeric::operator =(const ::rux::UInt32& value)
	{
		this->operator=( value._value );
		return *this;
	};
	Numeric& Numeric::operator =( ::rux::uint32 value )
	{
		if( _value_gcref )
			_value_gcref->Release();
		::rux::UInt32* value_gcref = alloc_object_macros( ::rux::UInt32 );
		value_gcref->_value = value;
		_value_gcref = value_gcref;
		return *this;
	};
	XNumeric::XNumeric( ::rux::uint64 value )
	{
		this->operator()()->operator=( value );
	};
	XNumeric& XNumeric::operator =(const ::rux::XUInt64& value)
	{
		this->operator()()->operator=( value() );
		return *this;
	};
	XNumeric& XNumeric::operator =( ::rux::uint64 value )
	{
		this->operator()()->operator=( value );
		return *this;
	};
	XNumeric& XNumeric::operator =(const ::rux::UInt64& value)
	{
		this->operator()()->operator=( value._value );
		return *this;
	};
	Numeric& Numeric::operator =(const ::rux::UInt64& value)
	{
		this->operator=( value._value );
		return *this;
	};
	Numeric& Numeric::operator =( ::rux::uint64 value )
	{
		if( _value_gcref )
			_value_gcref->Release();
		::rux::UInt64* value_gcref = alloc_object_macros( ::rux::UInt64 );
		value_gcref->_value = value;
		_value_gcref = value_gcref;
		return *this;
	};
	XNumeric::XNumeric( ::rux::int64 value )
	{
		this->operator()()->operator=( value );
	};
	XNumeric& XNumeric::operator =(const ::rux::XInt64& value)
	{
		this->operator()()->operator=( value() );
		return *this;
	};
	XNumeric& XNumeric::operator =( ::rux::int64 value )
	{
		this->operator()()->operator=( value );
		return *this;
	};
	XNumeric& XNumeric::operator =(const ::rux::Int64& value)
	{
		this->operator()()->operator=( value._value );
		return *this;
	};
	Numeric& Numeric::operator =(const ::rux::Int64& value)
	{
		this->operator=( value._value );
		return *this;
	};
	Numeric& Numeric::operator =( ::rux::int64 value )
	{
		if( _value_gcref )
			_value_gcref->Release();
		::rux::Int64* value_gcref = alloc_object_macros( ::rux::Int64 );
		value_gcref->_value = value;
		_value_gcref = value_gcref;
		return *this;
	};
	XNumeric& XNumeric::operator=( const XObject& object )
	{
		operator()()->operator=( object );
		return *this;
	};
	XNumeric& XNumeric::operator=( const XGCRef& object )
	{
		operator()()->operator=( &object );
		return *this;
	};
	XNumeric& XNumeric::operator=( XGCRef* object )
	{
		operator()()->operator=( object );
		return *this;
	};
	void XNumeric::set_ByRef( const XObject& type ) const
	{
		operator()()->operator=( type );
	};
	void XNumeric::set_ByRef( const Object& type ) const
	{
		operator()()->operator=( type.get_GCRefObj() );
	};
	void XNumeric::set_ByRef( const XGCRef& type ) const
	{
		operator()()->operator=( &type );
	};
	void XNumeric::set_ByRef( XGCRef* type ) const
	{
		operator()()->operator=( type );
	};
	Numeric& Numeric::operator=( const XGCRef* gcref )
	{
		if( _value_gcref )
		{
			_value_gcref->Release();
			_value_gcref = 0;
		}
		if( rux_is_object_ptr( gcref , ::rux::XFloat ) )
		{
			::rux::Float* value = (::rux::Float*)gcref->get_ObjectAddress();
			operator=( value->_value );
		}
		else if( rux_is_object_ptr( gcref , ::rux::XDouble ) )
		{
			::rux::Double* value = (::rux::Double*)gcref->get_ObjectAddress();
			operator=( value->_value );
		}
		else if( rux_is_object_ptr( gcref , ::rux::XInt32 ) )
		{
			::rux::Int32* value = (::rux::Int32*)gcref->get_ObjectAddress();
			operator=( value->_value );
		}
		else if( rux_is_object_ptr( gcref , ::rux::XInt64 ) )
		{
			::rux::Int64* value = (::rux::Int64*)gcref->get_ObjectAddress();
			operator=( value->_value );
		}
		else if( rux_is_object_ptr( gcref , ::rux::XUInt32 ) )
		{
			::rux::UInt32* value = (::rux::UInt32*)gcref->get_ObjectAddress();
			operator=( value->_value );
		}
		else if( rux_is_object_ptr( gcref , ::rux::XUInt64 ) )
		{
			::rux::UInt64* value = (::rux::UInt64*)gcref->get_ObjectAddress();
			operator=( value->_value );
		}
		else if( rux_is_object_ptr( gcref , ::rux::XNumeric ) )
		{
			::rux::Numeric* value = (::rux::Numeric*)gcref->get_ObjectAddress();
			_value_gcref = value->_value_gcref;
			if( _value_gcref )
				_value_gcref->AddRef();
		}
		return *this;
	};
	Numeric& Numeric::operator=( const XObject& object )
	{
		operator=( object.get_GCRef() );
		return *this;
	};
	implement_duplicate_internal_result_function( float , Numeric , to_float );
	float Numeric::to_float( void )
	{		
		if( _value_gcref )
		{
			if( rux_is_object_ptr( _value_gcref , ::rux::XFloat ) )
			{
				::rux::Float* value = (::rux::Float*)_value_gcref->get_ObjectAddress();
				return (float)value->_value;
			}
			else if( rux_is_object_ptr( _value_gcref , ::rux::XDouble ) )
			{
				::rux::Double* value = (::rux::Double*)_value_gcref->get_ObjectAddress();
				return (float)value->_value;
			}
			else if( rux_is_object_ptr( _value_gcref , ::rux::XInt32 ) )
			{
				::rux::Int32* value = (::rux::Int32*)_value_gcref->get_ObjectAddress();
				return (float)value->_value;
			}
			else if( rux_is_object_ptr( _value_gcref , ::rux::XInt64 ) )
			{
				::rux::Int64* value = (::rux::Int64*)_value_gcref->get_ObjectAddress();
				return (float)value->_value;
			}
			else if( rux_is_object_ptr( _value_gcref , ::rux::XUInt32 ) )
			{
				::rux::UInt32* value = (::rux::UInt32*)_value_gcref->get_ObjectAddress();
				return (float)value->_value;
			}
			else if( rux_is_object_ptr( _value_gcref , ::rux::XUInt64 ) )
			{
				::rux::UInt64* value = (::rux::UInt64*)_value_gcref->get_ObjectAddress();
				return (float)value->_value;
			}
		}
		return 0.f;
	};
	implement_duplicate_internal_result_function( double , Numeric , to_double );
	double Numeric::to_double( void )
	{
		if( _value_gcref )
		{
			if( rux_is_object_ptr( _value_gcref , ::rux::XFloat ) )
			{
				::rux::Float* value = (::rux::Float*)_value_gcref->get_ObjectAddress();
				return (double)value->_value;
			}
			else if( rux_is_object_ptr( _value_gcref , ::rux::XDouble ) )
			{
				::rux::Double* value = (::rux::Double*)_value_gcref->get_ObjectAddress();
				return (double)value->_value;
			}
			else if( rux_is_object_ptr( _value_gcref , ::rux::XInt32 ) )
			{
				::rux::Int32* value = (::rux::Int32*)_value_gcref->get_ObjectAddress();
				return (double)value->_value;
			}
			else if( rux_is_object_ptr( _value_gcref , ::rux::XInt64 ) )
			{
				::rux::Int64* value = (::rux::Int64*)_value_gcref->get_ObjectAddress();
				return (double)value->_value;
			}
			else if( rux_is_object_ptr( _value_gcref , ::rux::XUInt32 ) )
			{
				::rux::UInt32* value = (::rux::UInt32*)_value_gcref->get_ObjectAddress();
				return (double)value->_value;
			}
			else if( rux_is_object_ptr( _value_gcref , ::rux::XUInt64 ) )
			{
				::rux::UInt64* value = (::rux::UInt64*)_value_gcref->get_ObjectAddress();
				return (double)value->_value;
			}
		}
		return 0.0;
	};
	implement_duplicate_internal_result_function( ::rux::int32 , Numeric , to_int32 );
	::rux::int32 Numeric::to_int32( void )
	{
		if( _value_gcref )
		{
			if( rux_is_object_ptr( _value_gcref , ::rux::XFloat ) )
			{
				::rux::Float* value = (::rux::Float*)_value_gcref->get_ObjectAddress();
				return (::rux::int32)value->_value;
			}
			else if( rux_is_object_ptr( _value_gcref , ::rux::XDouble ) )
			{
				::rux::Double* value = (::rux::Double*)_value_gcref->get_ObjectAddress();
				return (::rux::int32)value->_value;
			}
			else if( rux_is_object_ptr( _value_gcref , ::rux::XInt32 ) )
			{
				::rux::Int32* value = (::rux::Int32*)_value_gcref->get_ObjectAddress();
				return (::rux::int32)value->_value;
			}
			else if( rux_is_object_ptr( _value_gcref , ::rux::XInt64 ) )
			{
				::rux::Int64* value = (::rux::Int64*)_value_gcref->get_ObjectAddress();
				return (::rux::int32)value->_value;
			}
			else if( rux_is_object_ptr( _value_gcref , ::rux::XUInt32 ) )
			{
				::rux::UInt32* value = (::rux::UInt32*)_value_gcref->get_ObjectAddress();
				return (::rux::int32)value->_value;
			}
			else if( rux_is_object_ptr( _value_gcref , ::rux::XUInt64 ) )
			{
				::rux::UInt64* value = (::rux::UInt64*)_value_gcref->get_ObjectAddress();
				return (::rux::int32)value->_value;
			}
		}
		return 0;
	};
	implement_duplicate_internal_result_function( ::rux::int64 , Numeric , to_int64 );
	::rux::int64 Numeric::to_int64( void )
	{
		if( _value_gcref )
		{
			if( rux_is_object_ptr( _value_gcref , ::rux::XFloat ) )
			{
				::rux::Float* value = (::rux::Float*)_value_gcref->get_ObjectAddress();
				return (::rux::int64)value->_value;
			}
			else if( rux_is_object_ptr( _value_gcref , ::rux::XDouble ) )
			{
				::rux::Double* value = (::rux::Double*)_value_gcref->get_ObjectAddress();
				return (::rux::int64)value->_value;
			}
			else if( rux_is_object_ptr( _value_gcref , ::rux::XInt32 ) )
			{
				::rux::Int32* value = (::rux::Int32*)_value_gcref->get_ObjectAddress();
				return (::rux::int64)value->_value;
			}
			else if( rux_is_object_ptr( _value_gcref , ::rux::XInt64 ) )
			{
				::rux::Int64* value = (::rux::Int64*)_value_gcref->get_ObjectAddress();
				return (::rux::int64)value->_value;
			}
			else if( rux_is_object_ptr( _value_gcref , ::rux::XUInt32 ) )
			{
				::rux::UInt32* value = (::rux::UInt32*)_value_gcref->get_ObjectAddress();
				return (::rux::int64)value->_value;
			}
			else if( rux_is_object_ptr( _value_gcref , ::rux::XUInt64 ) )
			{
				::rux::UInt64* value = (::rux::UInt64*)_value_gcref->get_ObjectAddress();
				return (::rux::int64)value->_value;
			}
		}
		return 0;
	};
	implement_duplicate_internal_result_function( ::rux::uint32 , Numeric , to_uint32 );
	::rux::uint32 Numeric::to_uint32( void )
	{
		if( _value_gcref )
		{
			if( rux_is_object_ptr( _value_gcref , ::rux::XFloat ) )
			{
				::rux::Float* value = (::rux::Float*)_value_gcref->get_ObjectAddress();
				return (::rux::uint32)value->_value;
			}
			else if( rux_is_object_ptr( _value_gcref , ::rux::XDouble ) )
			{
				::rux::Double* value = (::rux::Double*)_value_gcref->get_ObjectAddress();
				return (::rux::uint32)value->_value;
			}
			else if( rux_is_object_ptr( _value_gcref , ::rux::XInt32 ) )
			{
				::rux::Int32* value = (::rux::Int32*)_value_gcref->get_ObjectAddress();
				return (::rux::uint32)value->_value;
			}
			else if( rux_is_object_ptr( _value_gcref , ::rux::XInt64 ) )
			{
				::rux::Int64* value = (::rux::Int64*)_value_gcref->get_ObjectAddress();
				return (::rux::uint32)value->_value;
			}
			else if( rux_is_object_ptr( _value_gcref , ::rux::XUInt32 ) )
			{
				::rux::UInt32* value = (::rux::UInt32*)_value_gcref->get_ObjectAddress();
				return (::rux::uint32)value->_value;
			}
			else if( rux_is_object_ptr( _value_gcref , ::rux::XUInt64 ) )
			{
				::rux::UInt64* value = (::rux::UInt64*)_value_gcref->get_ObjectAddress();
				return (::rux::uint32)value->_value;
			}
		}
		return 0;
	};
	implement_duplicate_internal_result_function( ::rux::uint64 , Numeric , to_uint64 );
	::rux::uint64 Numeric::to_uint64( void )
	{
		if( _value_gcref )
		{
			if( rux_is_object_ptr( _value_gcref , ::rux::XFloat ) )
			{
				::rux::Float* value = (::rux::Float*)_value_gcref->get_ObjectAddress();
				return (::rux::uint64)value->_value;
			}
			else if( rux_is_object_ptr( _value_gcref , ::rux::XDouble ) )
			{
				::rux::Double* value = (::rux::Double*)_value_gcref->get_ObjectAddress();
				return (::rux::uint64)value->_value;
			}
			else if( rux_is_object_ptr( _value_gcref , ::rux::XInt32 ) )
			{
				::rux::Int32* value = (::rux::Int32*)_value_gcref->get_ObjectAddress();
				return (::rux::uint64)value->_value;
			}
			else if( rux_is_object_ptr( _value_gcref , ::rux::XInt64 ) )
			{
				::rux::Int64* value = (::rux::Int64*)_value_gcref->get_ObjectAddress();
				return (::rux::uint64)value->_value;
			}
			else if( rux_is_object_ptr( _value_gcref , ::rux::XUInt32 ) )
			{
				::rux::UInt32* value = (::rux::UInt32*)_value_gcref->get_ObjectAddress();
				return (::rux::uint64)value->_value;
			}
			else if( rux_is_object_ptr( _value_gcref , ::rux::XUInt64 ) )
			{
				::rux::UInt64* value = (::rux::UInt64*)_value_gcref->get_ObjectAddress();
				return (::rux::uint64)value->_value;
			}
		}
		return 0;
	};
	::rux::String* Numeric::ObjectToString( void )
	{		
		XString res( ToString() );
		return &res++;
	};
	implement_duplicate_internal_result_function( ::rux::String& , Numeric , ToString );
	::rux::String& Numeric::ToString( void )
	{
		if( _value_gcref )
		{
			if( rux_is_object_ptr( _value_gcref , ::rux::XDouble ) )
			{
				::rux::Double* val = (::rux::Double*)_value_gcref;
				return val->ToString();
			}
			else if( rux_is_object_ptr( _value_gcref , ::rux::XFloat ) )
			{
				::rux::Float* val = (::rux::Float*)_value_gcref;
				return val->ToString();
			}
			else if( rux_is_object_ptr( _value_gcref , ::rux::XInt32 ) )
			{
				::rux::Int32* val = (::rux::Int32*)_value_gcref;
				return val->ToString();
			}
			else if( rux_is_object_ptr( _value_gcref , ::rux::XUInt32 ) )
			{
				::rux::UInt32* val = (::rux::UInt32*)_value_gcref;
				return val->ToString();
			}
			else if( rux_is_object_ptr( _value_gcref , ::rux::XInt64 ) )
			{
				::rux::Int64* val = (::rux::Int64*)_value_gcref;
				return val->ToString();
			}
			else if( rux_is_object_ptr( _value_gcref , ::rux::XUInt64 ) )
			{
				::rux::UInt64* val = (::rux::UInt64*)_value_gcref;
				return val->ToString();
			}
		}
		return XString()++;
	};
	bool Numeric::Equals( XGCRef* gcref )
	{
		if( gcref == this )
			return true;
		else if( rux_is_object_ptr( gcref , ::rux::XDouble ) )
		{
			::rux::Double* val = (::rux::Double*)gcref;
			return val->_value == to_double();
		}
		else if( rux_is_object_ptr( gcref , ::rux::XFloat ) )
		{
			::rux::Float* val = (::rux::Float*)gcref;
			return val->_value == to_float();
		}
		else if( rux_is_object_ptr( gcref , ::rux::XInt32 ) )
		{
			::rux::Int32* val = (::rux::Int32*)gcref;
			return val->_value == to_int32();
		}
		else if( rux_is_object_ptr( gcref , ::rux::XUInt32 ) )
		{
			::rux::UInt32* val = (::rux::UInt32*)gcref;
			return val->_value == to_uint32();
		}
		else if( rux_is_object_ptr( gcref , ::rux::XInt64 ) )
		{
			::rux::Int64* val = (::rux::Int64*)gcref;
			return val->_value == to_int64();
		}
		else if( rux_is_object_ptr( gcref , ::rux::XUInt64 ) )
		{
			::rux::UInt64* val = (::rux::UInt64*)gcref;
			return val->_value == to_uint64();
		}
		else
			return false;
	};
};