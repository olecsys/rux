#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_int64.h"
#include <xan_keyvaluepair.h>
#include <xan_uint64.h>
#include <xan_boolean.h>
begin_implement_rux_class_without_operators_ns( Int64 , rux )
	_value = 0;
end_implement_rux_class();
namespace rux
{		
	XInt64::XInt64( ::rux::int64 value )
	{
		(*this) = value;
	};
	::rux::int64 operator +(XInt64& value0, XInt64& value1)
	{
		return value0() + value1();
	};
	::rux::int64 operator *(XInt64& value0, XInt64& value1)
	{
		return value0() * value1();
	};
	::rux::int64 operator /(XInt64& value0, XInt64& value1)
	{
		return value0() / value1();
	};
	::rux::int64 operator -(XInt64& value0, XInt64& value1)
	{
		return value0() - value1();
	};
	::rux::int64 operator +(::rux::int64 value0, XInt64& value1)
	{
		return value0 + value1();
	};
	::rux::int64 operator *(::rux::int64 value0, XInt64& value1)
	{
		return value0 * value1();
	};
	::rux::int64 operator /(::rux::int64 value0, XInt64& value1)
	{
		return value0 / value1();
	};
	::rux::int64 operator -(::rux::int64 value0, XInt64& value1)
	{
		return value0 - value1();
	};
	::rux::int64 operator +(XInt64& value0, ::rux::int64 value1)
	{
		return value0() + value1;
	};
	::rux::int64 operator *(XInt64& value0, ::rux::int64 value1)
	{
		return value0() * value1;
	};
	::rux::int64 operator /(XInt64& value0, ::rux::int64 value1)
	{
		return value0() / value1;
	};
	::rux::int64 operator -(XInt64& value0, ::rux::int64 value1)
	{
		return value0() - value1;
	};
	Int64& Int64::operator =(const Int64& value)
	{
		_value = value._value;
		return *this;
	};
	Int64& Int64::operator =(::rux::int64 value)
	{
		_value = value;
		return *this;
	};
	bool Int64::operator ==(const Int64& value)
	{
		return _value == value._value;
	};
	bool Int64::operator ==(::rux::int64 value)
	{
		return _value == value;
	};
	bool Int64::operator !=(const Int64& value)
	{
		return _value != value._value;
	};
	bool Int64::operator !=(::rux::int64 value)
	{
		return _value != value;
	};
	Int64& Int64::operator +=(const Int64& value)
	{
		_value += value._value;
		return *this;
	};
	Int64& Int64::operator +=(::rux::int64 value)
	{
		_value += value;
		return *this;
	};
	Int64& Int64::operator -=(const Int64& value)
	{
		_value -= value._value;
		return *this;
	};
	Int64& Int64::operator -=(::rux::int64 value)
	{
		_value -= value;
		return *this;
	}
	XInt64& XInt64::operator =(const XInt64& value)
	{
		(*static_cast<XGCHandle<Int64>*>(this))()->_value = (*static_cast<const XGCHandle<Int64>*>(&value))()->_value;
		return *this;
	};
	XInt64& XInt64::operator =(::rux::int64 value)
	{
		(*static_cast<XGCHandle<Int64>*>(this))()->_value = value;
		return *this;
	};
	XInt64& XInt64::operator =(const Int64& value)
	{
		(*static_cast<XGCHandle<Int64>*>(this))()->_value = value._value;
		return *this;
	};
	XInt64& XInt64::operator=( XObject& object )
	{
		*static_cast<XGCHandle<Int64>*>(this) = object;
		return *this;
	};
	bool XInt64::operator ==(const XInt64& value)
	{
		return (*static_cast<XGCHandle<Int64>*>(this))()->_value == (*static_cast<const XGCHandle<Int64>*>(&value))()->_value;
	};
	bool XInt64::operator ==(::rux::int64 value)
	{
		return (*static_cast<XGCHandle<Int64>*>(this))()->_value == value;
	};
	bool XInt64::operator !=(const XInt64& value)
	{
		return (*static_cast<XGCHandle<Int64>*>(this))()->_value != (*static_cast<const XGCHandle<Int64>*>(&value))()->_value;
	};
	bool XInt64::operator !=(::rux::int64 value)
	{
		return (*static_cast<XGCHandle<Int64>*>(this))()->_value != value;
	};
	XInt64& XInt64::operator +=(const XInt64& value)
	{
		(*static_cast<XGCHandle<Int64>*>(this))()->_value += (*static_cast<const XGCHandle<Int64>*>(&value))()->_value;
		return *this;
	};
	XInt64& XInt64::operator +=(::rux::int64 value)
	{
		(*static_cast<XGCHandle<Int64>*>(this))()->_value += value;
		return *this;
	};
	XInt64& XInt64::operator -=(const XInt64& value)
	{
		(*static_cast<XGCHandle<Int64>*>(this))()->_value -= (*static_cast<const XGCHandle<Int64>*>(&value))()->_value;
		return *this;
	};
	XInt64& XInt64::operator -=(::rux::int64 value)
	{
		(*static_cast<XGCHandle<Int64>*>(this))()->_value -= value;
		return *this;
	}
	::rux::int64 XInt64::operator()() const
	{
		return (*static_cast<XGCHandle<Int64>*>( const_cast<XInt64*>( this ) ))()->_value;
	};
	::rux::int64 Int64::operator()() const
	{
		return _value;
	};
	::rux::String* Int64::ObjectToString( void )
	{		
		XString res( ToString() );
		return &res++;
	};
	implement_duplicate_internal_result_function( ::rux::String& , Int64 , ToString );
	::rux::String& Int64::ToString( void )
	{
		return XString::Format( I64d , _value );
	};
	void XInt64::ToString( char destination[ 512 ] )
	{
		::rux::safe_sprintf( destination , 512 , I64d , (*this)() );
	};
	bool Int64::Equals( XGCRef* gcref )
	{
		if( gcref == this )
			return true;
		else if( rux_is_object_ptr( gcref , ::rux::XInt64 ) )
		{
			::rux::XInt64 val( *gcref );
			return val == *this;
		}
		else
			return false;
	};
};