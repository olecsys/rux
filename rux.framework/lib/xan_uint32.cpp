#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_uint32.h"
#include <xan_keyvaluepair.h>
#include <xan_uint64.h>
#include <xan_boolean.h>
begin_implement_rux_class_without_operators_ns( UInt32 , rux )
	_value = 0;
end_implement_rux_class();
namespace rux
{		
	XUInt32::XUInt32( ::rux::uint32 value )
	{
		(*this) = value;
	};
	::rux::uint32 operator +(XUInt32& value0, XUInt32& value1)
	{
		return value0() + value1();
	};
	::rux::uint32 operator *(XUInt32& value0, XUInt32& value1)
	{
		return value0() * value1();
	};
	::rux::uint32 operator /(XUInt32& value0, XUInt32& value1)
	{
		return value0() / value1();
	};
	::rux::uint32 operator -(XUInt32& value0, XUInt32& value1)
	{
		return value0() - value1();
	};
	::rux::uint32 operator +(::rux::uint32 value0, XUInt32& value1)
	{
		return value0 + value1();
	};
	::rux::uint32 operator *(::rux::uint32 value0, XUInt32& value1)
	{
		return value0 * value1();
	};
	::rux::uint32 operator /(::rux::uint32 value0, XUInt32& value1)
	{
		return value0 / value1();
	};
	::rux::uint32 operator -(::rux::uint32 value0, XUInt32& value1)
	{
		return value0 - value1();
	};
	::rux::uint32 operator +(XUInt32& value0, ::rux::uint32 value1)
	{
		return value0() + value1;
	};
	::rux::uint32 operator *(XUInt32& value0, ::rux::uint32 value1)
	{
		return value0() * value1;
	};
	::rux::uint32 operator /(XUInt32& value0, ::rux::uint32 value1)
	{
		return value0() / value1;
	};
	::rux::uint32 operator -(XUInt32& value0, ::rux::uint32 value1)
	{
		return value0() - value1;
	};
	UInt32& UInt32::operator =(const UInt32& value)
	{
		_value = value._value;
		return *this;
	};
	UInt32& UInt32::operator =(::rux::uint32 value)
	{
		_value = value;
		return *this;
	};
	bool UInt32::operator ==(const UInt32& value)
	{
		return _value == value._value;
	};
	bool UInt32::operator ==(::rux::uint32 value)
	{
		return _value == value;
	};
	bool UInt32::operator !=(const UInt32& value)
	{
		return _value != value._value;
	};
	bool UInt32::operator >( ::rux::uint32 value )
	{
		return _value > value;
	};
	bool UInt32::operator >=( ::rux::uint32 value )
	{
		return _value >= value;
	};
	bool UInt32::operator <( ::rux::uint32 value )
	{
		return _value < value;
	};
	bool UInt32::operator <=( ::rux::uint32 value )
	{
		return _value <= value;
	};
	bool UInt32::operator !=(::rux::uint32 value)
	{
		return _value != value;
	};
	UInt32& UInt32::operator +=(const UInt32& value)
	{
		_value += value._value;
		return *this;
	};
	UInt32& UInt32::operator +=(::rux::uint32 value)
	{
		_value += value;
		return *this;
	};
	UInt32& UInt32::operator -=(const UInt32& value)
	{
		_value -= value._value;
		return *this;
	};
	UInt32& UInt32::operator -=(::rux::uint32 value)
	{
		_value -= value;
		return *this;
	}
	XUInt32& XUInt32::operator =(const XUInt32& value)
	{
		(*static_cast< XGCHandle<UInt32>*>(this))()->_value = (*static_cast<const XGCHandle<UInt32>*>(&value))()->_value;
		return *this;
	};
	XUInt32& XUInt32::operator =(::rux::uint32 value)
	{
		(*static_cast<XGCHandle<UInt32>*>(this))()->_value = value;
		return *this;
	};
	XUInt32& XUInt32::operator =(const UInt32& value)
	{
		(*static_cast<XGCHandle<UInt32>*>(this))()->_value = value._value;
		return *this;
	};
	XUInt32& XUInt32::operator=( XObject& object )
	{
		*static_cast<XGCHandle<UInt32>*>(this) = object;
		return *this;
	};
	bool XUInt32::operator ==(const XUInt32& value)
	{
		return (*static_cast<XGCHandle<UInt32>*>(this))()->_value == (*static_cast<const XGCHandle<UInt32>*>(&value))()->_value;
	};
	bool XUInt32::operator ==(::rux::uint32 value)
	{
		return (*static_cast<XGCHandle<UInt32>*>(this))()->_value == value;
	};
	bool XUInt32::operator !=(const XUInt32& value)
	{
		return (*static_cast<XGCHandle<UInt32>*>(this))()->_value != (*static_cast<const XGCHandle<UInt32>*>(&value))()->_value;
	};
	bool XUInt32::operator >( ::rux::uint32 value )
	{
		return (*static_cast<XGCHandle<UInt32>*>(this))()->_value > value;
	};
	bool XUInt32::operator >=( ::rux::uint32 value )
	{
		return (*static_cast<XGCHandle<UInt32>*>(this))()->_value >= value;
	};
	bool XUInt32::operator <( ::rux::uint32 value )
	{
		return (*static_cast<XGCHandle<UInt32>*>(this))()->_value < value;
	};
	bool XUInt32::operator <=( ::rux::uint32 value )
	{
		return (*static_cast<XGCHandle<UInt32>*>(this))()->_value <= value;
	};
	bool XUInt32::operator !=(::rux::uint32 value)
	{
		return (*static_cast<XGCHandle<UInt32>*>(this))()->_value != value;
	};
	XUInt32& XUInt32::operator +=(const XUInt32& value)
	{
		(*static_cast<XGCHandle<UInt32>*>(this))()->_value += (*static_cast<const XGCHandle<UInt32>*>(&value))()->_value;
		return *this;
	};
	XUInt32& XUInt32::operator +=(::rux::uint32 value)
	{
		(*static_cast<XGCHandle<UInt32>*>(this))()->_value += value;
		return *this;
	};
	XUInt32& XUInt32::operator -=(const XUInt32& value)
	{
		(*static_cast<XGCHandle<UInt32>*>(this))()->_value -= (*static_cast<const XGCHandle<UInt32>*>(&value))()->_value;
		return *this;
	};
	XUInt32& XUInt32::operator -=(::rux::uint32 value)
	{
		(*static_cast<XGCHandle<UInt32>*>(this))()->_value -= value;
		return *this;
	}
	::rux::uint32 XUInt32::operator()() const
	{
		return (*static_cast<XGCHandle<UInt32>*>( const_cast<XUInt32*>( this ) ) )()->_value;
	};
	::rux::uint32 UInt32::operator()() const
	{
		return _value;
	};
	::rux::String* UInt32::ObjectToString( void )
	{		
		XString res( ToString() );
		return &res++;
	};
	implement_duplicate_internal_result_function( ::rux::String& , UInt32 , ToString );
	String& UInt32::ToString( void )
	{
		return XString::Format( "%u" , _value );
	};
	bool UInt32::Equals( XGCRef* gcref )
	{
		if( gcref == this )
			return true;
		else if( rux_is_object_ptr( gcref , ::rux::XUInt32 ) )
		{
			::rux::XUInt32 val( *gcref );
			return val == *this;
		}
		else
			return false;
	};
};