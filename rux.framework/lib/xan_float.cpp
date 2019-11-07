#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_float.h"
#include "xan_keyvaluepair.h"
#include "xan_uint64.h"
#include "xan_boolean.h"
begin_implement_rux_class_without_operators_ns( Float , rux )
	_value = 0.f;
end_implement_rux_class();
namespace rux
{		
	XFloat::XFloat( float value )
	{
		(*this) = value;
	};
	float operator +(XFloat& value0, XFloat& value1)
	{
		return value0() + value1();
	};
	float operator *(XFloat& value0, XFloat& value1)
	{
		return value0() * value1();
	};
	float operator /(XFloat& value0, XFloat& value1)
	{
		return value0() / value1();
	};
	float operator -(XFloat& value0, XFloat& value1)
	{
		return value0() - value1();
	};
	float operator +(float value0, XFloat& value1)
	{
		return value0 + value1();
	};
	float operator *(float value0, XFloat& value1)
	{
		return value0 * value1();
	};
	float operator /(float value0, XFloat& value1)
	{
		return value0 / value1();
	};
	float operator -(float value0, XFloat& value1)
	{
		return value0 - value1();
	};
	float operator +(XFloat& value0, float value1)
	{
		return value0() + value1;
	};
	float operator *(XFloat& value0, float value1)
	{
		return value0() * value1;
	};
	float operator /(XFloat& value0, float value1)
	{
		return value0() / value1;
	};
	float operator -(XFloat& value0, float value1)
	{
		return value0() - value1;
	};
	XFloat& XFloat::operator =(const XFloat& value)
	{
		(*static_cast<XGCHandle<Float>*>(this))()->_value = (*static_cast<const XGCHandle<Float>*>(&value))()->_value;
		return *this;
	};
	XFloat& XFloat::operator =(float value)
	{
		(*static_cast<XGCHandle<Float>*>(this))()->_value = value;
		return *this;
	};
	XFloat& XFloat::operator =(const Float& value)
	{
		(*static_cast<XGCHandle<Float>*>(this))()->_value = value._value;
		return *this;
	};
	XFloat& XFloat::operator=( XObject& object )
	{
		*static_cast<XGCHandle<Float>*>(this) = object;
		return *this;
	};
	bool XFloat::operator ==(const XFloat& value)
	{
		return (*static_cast<XGCHandle<Float>*>(this))()->_value == (*static_cast<const XGCHandle<Float>*>(&value))()->_value;
	};
	bool XFloat::operator ==(float value)
	{
		return (*static_cast<XGCHandle<Float>*>(this))()->_value == value;
	};
	bool XFloat::operator !=(const XFloat& value)
	{
		return (*static_cast<XGCHandle<Float>*>(this))()->_value != (*static_cast<const XGCHandle<Float>*>(&value))()->_value;
	};
	bool XFloat::operator !=(float value)
	{
		return (*static_cast<XGCHandle<Float>*>(this))()->_value != value;
	};
	XFloat& XFloat::operator +=(const XFloat& value)
	{
		(*static_cast<XGCHandle<Float>*>(this))()->_value += (*static_cast<const XGCHandle<Float>*>(&value))()->_value;
		return *this;
	};
	XFloat& XFloat::operator +=(float value)
	{
		(*static_cast<XGCHandle<Float>*>(this))()->_value += value;
		return *this;
	};
	XFloat& XFloat::operator -=(const XFloat& value)
	{
		(*static_cast<XGCHandle<Float>*>(this))()->_value -= (*static_cast<const XGCHandle<Float>*>(&value))()->_value;
		return *this;
	};
	XFloat& XFloat::operator -=(float value)
	{
		(*static_cast<XGCHandle<Float>*>(this))()->_value -= value;
		return *this;
	}
	Float& Float::operator =(const Float& value)
	{
		_value = value._value;
		return *this;
	};
	Float& Float::operator =(float value)
	{
		_value = value;
		return *this;
	};
	bool Float::operator ==(const Float& value)
	{
		return _value == value._value;
	};
	bool Float::operator ==(float value)
	{
		return _value == value;
	};
	bool Float::operator !=(const Float& value)
	{
		return _value != value._value;
	};
	bool Float::operator !=(float value)
	{
		return _value != value;
	};
	Float& Float::operator +=(const Float& value)
	{
		_value += value._value;
		return *this;
	};
	Float& Float::operator +=(float value)
	{
		_value += value;
		return *this;
	};
	Float& Float::operator -=(const Float& value)
	{
		_value -= value._value;
		return *this;
	};
	Float& Float::operator -=(float value)
	{
		_value -= value;
		return *this;
	}
	float XFloat::operator()() const
	{
		return (*static_cast<XGCHandle<Float>*>( const_cast<XFloat*>( this ) ))()->_value;
	};
	float Float::operator()() const
	{
		return _value;
	};
	::rux::String* Float::ObjectToString( void )
	{		
		XString res( ToString() );
		return &res++;
	};
	implement_duplicate_internal_result_function( ::rux::String& , Float , ToString );
	::rux::String& Float::ToString( void )
	{
		declare_stack_variable( char , text , 64 );
		::rux::safe_sprintf( text , 64 , "%.2f" , _value );
		return XString( text , XEnumCodePage_UTF8 , 1 , NULL , __FILE__ , __LINE__ )++;
	};
	bool Float::Equals( XGCRef* gcref )
	{
		if( gcref == this )
			return true;
		else if( rux_is_object_ptr( gcref , ::rux::XFloat ) )
		{
			::rux::XFloat val( *gcref );
			return val == *this;
		}
		else
			return false;
	};
};