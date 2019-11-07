#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_int32.h"
#include "xan_keyvaluepair.h"
#include "xan_uint64.h"
#include "xan_boolean.h"
begin_implement_rux_class_without_operators_ns( Int32 , rux )
	_value = 0;
end_implement_rux_class();
namespace rux
{		
	XInt32::XInt32( ::rux::int32 value )
	{
		(*this) = value;
	};
	::rux::int32 operator +(XInt32& value0, XInt32& value1)
	{
		return value0() + value1();
	};
	::rux::int32 operator *(XInt32& value0, XInt32& value1)
	{
		return value0() * value1();
	};
	::rux::int32 operator /(XInt32& value0, XInt32& value1)
	{
		return value0() / value1();
	};
	::rux::int32 operator -(XInt32& value0, XInt32& value1)
	{
		return value0() - value1();
	};
	::rux::int32 operator +(::rux::int32 value0, XInt32& value1)
	{
		return value0 + value1();
	};
	::rux::int32 operator *(::rux::int32 value0, XInt32& value1)
	{
		return value0 * value1();
	};
	::rux::int32 operator /(::rux::int32 value0, XInt32& value1)
	{
		return value0 / value1();
	};
	::rux::int32 operator -(::rux::int32 value0, XInt32& value1)
	{
		return value0 - value1();
	};
	::rux::int32 operator +(XInt32& value0, ::rux::int32 value1)
	{
		return value0() + value1;
	};
	::rux::int32 operator *(XInt32& value0, ::rux::int32 value1)
	{
		return value0() * value1;
	};
	::rux::int32 operator /(XInt32& value0, ::rux::int32 value1)
	{
		return value0() / value1;
	};
	::rux::int32 operator -(XInt32& value0, ::rux::int32 value1)
	{
		return value0() - value1;
	};
	Int32& Int32::operator =(const Int32& value)
	{
		_value = value._value;
		return *this;
	};
	Int32& Int32::operator =(::rux::int32 value)
	{
		_value = value;
		return *this;
	};
	bool Int32::operator ==(const Int32& value)
	{
		return _value == value._value;
	};
	bool Int32::operator ==(::rux::int32 value)
	{
		return _value == value;
	};
	bool Int32::operator !=(const Int32& value)
	{
		return _value != value._value;
	};
	bool Int32::operator !=(::rux::int32 value)
	{
		return _value != value;
	};
	Int32& Int32::operator +=(const Int32& value)
	{
		_value += value._value;
		return *this;
	};
	Int32& Int32::operator +=(::rux::int32 value)
	{
		_value += value;
		return *this;
	};
	Int32& Int32::operator -=(const Int32& value)
	{
		_value -= value._value;
		return *this;
	};
	Int32& Int32::operator -=(::rux::int32 value)
	{
		_value -= value;
		return *this;
	}
	XInt32& XInt32::operator =(const XInt32& value)
	{
		(*static_cast< XGCHandle<Int32>*>(this))()->_value = (*static_cast<const XGCHandle<Int32>*>(&value))()->_value;
		return *this;
	};
	XInt32& XInt32::operator =(::rux::int32 value)
	{
		(*static_cast<XGCHandle<Int32>*>(this))()->_value = value;
		return *this;
	};
	XInt32& XInt32::operator =(const Int32& value)
	{
		(*static_cast<XGCHandle<Int32>*>(this))()->_value = value._value;
		return *this;
	};
	XInt32& XInt32::operator=( XObject& object )
	{
		*static_cast<XGCHandle<Int32>*>(this) = object;
		return *this;
	};
	bool XInt32::operator ==(const XInt32& value)
	{
		return (*static_cast<XGCHandle<Int32>*>(this))()->_value == (*static_cast<const XGCHandle<Int32>*>(&value))()->_value;
	};
	bool XInt32::operator ==(::rux::int32 value)
	{
		return (*static_cast<XGCHandle<Int32>*>(this))()->_value == value;
	};
	bool XInt32::operator !=(const XInt32& value)
	{
		return (*static_cast<XGCHandle<Int32>*>(this))()->_value != (*static_cast<const XGCHandle<Int32>*>(&value))()->_value;
	};
	bool XInt32::operator !=(::rux::int32 value)
	{
		return (*static_cast<XGCHandle<Int32>*>(this))()->_value != value;
	};
	XInt32& XInt32::operator +=(const XInt32& value)
	{
		(*static_cast<XGCHandle<Int32>*>(this))()->_value += (*static_cast<const XGCHandle<Int32>*>(&value))()->_value;
		return *this;
	};
	XInt32& XInt32::operator +=(::rux::int32 value)
	{
		(*static_cast<XGCHandle<Int32>*>(this))()->_value += value;
		return *this;
	};
	XInt32& XInt32::operator -=(const XInt32& value)
	{
		(*static_cast<XGCHandle<Int32>*>(this))()->_value -= (*static_cast<const XGCHandle<Int32>*>(&value))()->_value;
		return *this;
	};
	XInt32& XInt32::operator -=(::rux::int32 value)
	{
		(*static_cast<XGCHandle<Int32>*>(this))()->_value -= value;
		return *this;
	}
	::rux::int32 XInt32::operator()() const
	{
		return (*static_cast<XGCHandle<Int32>*>( const_cast<XInt32*>( this ) ))()->_value;
	};
	::rux::int32 Int32::operator()() const
	{
		return _value;
	};
	::rux::String* Int32::ObjectToString( void )
	{		
		XString res( ToString() );
		return &res++;
	};
	implement_duplicate_internal_result_function( ::rux::String& , Int32 , ToString );
	::rux::String& Int32::ToString( void )
	{
		return XString::Format( "%d" , _value );
	};
	bool Int32::Equals( XGCRef* gcref )
	{
		if( gcref == this )
			return true;
		else if( rux_is_object_ptr( gcref , ::rux::XInt32 ) )
		{
			::rux::XInt32 val( *gcref );
			return val == *this;
		}
		else
			return false;
	};
};