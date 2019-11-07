#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_uint64.h"
#include "xan_keyvaluepair.h"
#include "xan_boolean.h"
begin_implement_rux_class_without_operators_ns( UInt64 , rux )
	_value = 0;
end_implement_rux_class();
namespace rux
{		
	XUInt64::XUInt64( ::rux::uint64 value )
	{
		(*this) = value;
	};
	::rux::uint64 operator +(XUInt64& value0, XUInt64& value1)
	{
		return value0() + value1();
	};
	::rux::uint64 operator *(XUInt64& value0, XUInt64& value1)
	{
		return value0() * value1();
	};
	::rux::uint64 operator /(XUInt64& value0, XUInt64& value1)
	{
		return value0() / value1();
	};
	::rux::uint64 operator -(XUInt64& value0, XUInt64& value1)
	{
		return value0() - value1();
	};
	::rux::uint64 operator +(::rux::uint64 value0, XUInt64& value1)
	{
		return value0 + value1();
	};
	::rux::uint64 operator *(::rux::uint64 value0, XUInt64& value1)
	{
		return value0 * value1();
	};
	::rux::uint64 operator /(::rux::uint64 value0, XUInt64& value1)
	{
		return value0 / value1();
	};
	::rux::uint64 operator -(::rux::uint64 value0, XUInt64& value1)
	{
		return value0 - value1();
	};
	::rux::uint64 operator +(XUInt64& value0, ::rux::uint64 value1)
	{
		return value0() + value1;
	};
	::rux::uint64 operator *(XUInt64& value0, ::rux::uint64 value1)
	{
		return value0() * value1;
	};
	::rux::uint64 operator /(XUInt64& value0, ::rux::uint64 value1)
	{
		return value0() / value1;
	};
	::rux::uint64 operator -(XUInt64& value0, ::rux::uint64 value1)
	{
		return value0() - value1;
	};
	UInt64& UInt64::operator =(const UInt64& value)
	{
		_value = value._value;
		return *this;
	};
	UInt64& UInt64::operator =(::rux::uint64 value)
	{
		_value = value;
		return *this;
	};
	bool UInt64::operator ==(const UInt64& value)
	{
		return _value == value._value;
	};
	bool UInt64::operator ==(::rux::uint64 value)
	{
		return _value == value;
	};
	bool UInt64::operator !=(const UInt64& value)
	{
		return _value != value._value;
	};
	bool UInt64::operator !=(::rux::uint64 value)
	{
		return _value != value;
	};
	UInt64& UInt64::operator +=(const UInt64& value)
	{
		_value += value._value;
		return *this;
	};
	UInt64& UInt64::operator +=(::rux::uint64 value)
	{
		_value += value;
		return *this;
	};
	UInt64& UInt64::operator -=(const UInt64& value)
	{
		_value -= value._value;
		return *this;
	};
	UInt64& UInt64::operator -=(::rux::uint64 value)
	{
		_value -= value;
		return *this;
	}
	XUInt64& XUInt64::operator =(const XUInt64& value)
	{
		(*static_cast< XGCHandle<UInt64>*>(this))()->_value = (*static_cast<const XGCHandle<UInt64>*>(&value))()->_value;
		return *this;
	};
	XUInt64& XUInt64::operator =(::rux::uint64 value)
	{
		(*static_cast<XGCHandle<UInt64>*>(this))()->_value = value;
		return *this;
	};
	XUInt64& XUInt64::operator =(const UInt64& value)
	{
		(*static_cast<XGCHandle<UInt64>*>(this))()->_value = value._value;
		return *this;
	};
	XUInt64& XUInt64::operator=( XObject& object )
	{
		*static_cast<XGCHandle<UInt64>*>(this) = object;
		return *this;
	};
	bool XUInt64::operator ==(const XUInt64& value)
	{
		return (*static_cast<XGCHandle<UInt64>*>(this))()->_value == (*static_cast<const XGCHandle<UInt64>*>(&value))()->_value;
	};
	bool XUInt64::operator ==(::rux::uint64 value)
	{
		return (*static_cast<XGCHandle<UInt64>*>(this))()->_value == value;
	};
	bool XUInt64::operator !=(const XUInt64& value)
	{
		return (*static_cast<XGCHandle<UInt64>*>(this))()->_value != (*static_cast<const XGCHandle<UInt64>*>(&value))()->_value;
	};
	bool XUInt64::operator !=(::rux::uint64 value)
	{
		return (*static_cast<XGCHandle<UInt64>*>(this))()->_value != value;
	};
	XUInt64& XUInt64::operator +=(const XUInt64& value)
	{
		(*static_cast<XGCHandle<UInt64>*>(this))()->_value += (*static_cast<const XGCHandle<UInt64>*>(&value))()->_value;
		return *this;
	};
	XUInt64& XUInt64::operator +=(::rux::uint64 value)
	{
		(*static_cast<XGCHandle<UInt64>*>(this))()->_value += value;
		return *this;
	};
	XUInt64& XUInt64::operator -=(const XUInt64& value)
	{
		(*static_cast<XGCHandle<UInt64>*>(this))()->_value -= (*static_cast<const XGCHandle<UInt64>*>(&value))()->_value;
		return *this;
	};
	XUInt64& XUInt64::operator -=(::rux::uint64 value)
	{
		(*static_cast<XGCHandle<UInt64>*>(this))()->_value -= value;
		return *this;
	}
	::rux::uint64 XUInt64::operator()() const
	{
		return (*static_cast<XGCHandle<UInt64>*>( const_cast<XUInt64*>( this ) ))()->_value;
	};
	::rux::uint64 UInt64::operator()() const
	{
		return _value;
	};
	::rux::String* UInt64::ObjectToString( void )
	{		
		XString res( ToString() );
		return &res++;
	};
	implement_duplicate_internal_result_function( ::rux::String& , UInt64 , ToString );
	String& UInt64::ToString( void )
	{
		return XString::Format( I64u , _value );
	};
	bool UInt64::Equals( XGCRef* gcref )
	{
		if( gcref == this )
			return true;
		else if( rux_is_object_ptr( gcref , ::rux::XUInt64 ) )
		{
			::rux::XUInt64 val( *gcref );
			return val == *this;
		}
		else
			return false;
	};
};