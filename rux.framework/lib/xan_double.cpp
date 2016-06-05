#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_double.h"
#include <xan_keyvaluepair.h>
#include <xan_uint64.h>
#include <xan_boolean.h>
begin_implement_rux_class_without_operators_ns( Double , rux )
	_value = 0.0;
end_implement_rux_class();
namespace rux
{		
	XDouble::XDouble( double value )
	{
		(*this) = value;
	};
	double operator +(XDouble& value0, XDouble& value1)
	{
		return value0() + value1();
	};
	double operator *(XDouble& value0, XDouble& value1)
	{
		return value0() * value1();
	};
	double operator /(XDouble& value0, XDouble& value1)
	{
		return value0() / value1();
	};
	double operator -(XDouble& value0, XDouble& value1)
	{
		return value0() - value1();
	};
	double operator +(double value0, XDouble& value1)
	{
		return value0 + value1();
	};
	double operator *(double value0, XDouble& value1)
	{
		return value0 * value1();
	};
	double operator /(double value0, XDouble& value1)
	{
		return value0 / value1();
	};
	double operator -(double value0, XDouble& value1)
	{
		return value0 - value1();
	};
	double operator +(XDouble& value0, double value1)
	{
		return value0() + value1;
	};
	double operator *(XDouble& value0, double value1)
	{
		return value0() * value1;
	};
	double operator /(XDouble& value0, double value1)
	{
		return value0() / value1;
	};
	double operator -(XDouble& value0, double value1)
	{
		return value0() - value1;
	};
	Double& Double::operator =(const Double& value)
	{
		_value = value._value;
		return *this;
	};
	Double& Double::operator =(double value)
	{
		_value = value;
		return *this;
	};
	bool Double::operator ==(const Double& value)
	{
		return _value == value._value;
	};
	bool Double::operator ==(double value)
	{
		return _value == value;
	};
	bool Double::operator !=(const Double& value)
	{
		return _value != value._value;
	};
	bool Double::operator !=(double value)
	{
		return _value != value;
	};
	Double& Double::operator +=(const Double& value)
	{
		_value += value._value;
		return *this;
	};
	Double& Double::operator +=(double value)
	{
		_value += value;
		return *this;
	};
	Double& Double::operator -=(const Double& value)
	{
		_value -= value._value;
		return *this;
	};
	Double& Double::operator -=(double value)
	{
		_value -= value;
		return *this;
	}
	XDouble& XDouble::operator =(const XDouble& value)
	{
		(*static_cast<XGCHandle<Double>*>(this))()->_value = (*static_cast<const XGCHandle<Double>*>(&value))()->_value;
		return *this;
	};
	XDouble& XDouble::operator =(double value)
	{
		(*static_cast<XGCHandle<Double>*>(this))()->_value = value;
		return *this;
	};
	XDouble& XDouble::operator =(const Double& value)
	{
		(*static_cast<XGCHandle<Double>*>(this))()->_value = value._value;
		return *this;
	};
	XDouble& XDouble::operator=( XObject& object )
	{
		*static_cast<XGCHandle<Double>*>(this) = object;
		return *this;
	};
	bool XDouble::operator ==(const XDouble& value)
	{
		return (*static_cast<XGCHandle<Double>*>(this))()->_value == (*static_cast<const XGCHandle<Double>*>(&value))()->_value;
	};
	bool XDouble::operator ==(double value)
	{
		return (*static_cast<XGCHandle<Double>*>(this))()->_value == value;
	};
	bool XDouble::operator !=(const XDouble& value)
	{
		return (*static_cast<XGCHandle<Double>*>(this))()->_value != (*static_cast<const XGCHandle<Double>*>(&value))()->_value;
	};
	bool XDouble::operator !=(double value)
	{
		return (*static_cast<XGCHandle<Double>*>(this))()->_value != value;
	};
	XDouble& XDouble::operator +=(const XDouble& value)
	{
		(*static_cast<XGCHandle<Double>*>(this))()->_value += (*static_cast<const XGCHandle<Double>*>(&value))()->_value;
		return *this;
	};
	XDouble& XDouble::operator +=(double value)
	{
		(*static_cast<XGCHandle<Double>*>(this))()->_value += value;
		return *this;
	};
	XDouble& XDouble::operator -=(const XDouble& value)
	{
		(*static_cast<XGCHandle<Double>*>(this))()->_value -= (*static_cast<const XGCHandle<Double>*>(&value))()->_value;
		return *this;
	};
	XDouble& XDouble::operator -=(double value)
	{
		(*static_cast<XGCHandle<Double>*>(this))()->_value -= value;
		return *this;
	}
	double XDouble::operator()() const
	{
		return (*static_cast<XGCHandle<Double>*>( const_cast<XDouble*>( this ) ))()->_value;
	};
	double Double::operator()() const
	{
		return _value;
	};
	::rux::String* Double::ObjectToString( void )
	{		
		XString res( ToString() );
		return &res++;
	};
	implement_duplicate_internal_result_function( ::rux::String& , Double , ToString );
	::rux::String& Double::ToString( void )
	{
		declare_stack_variable( char , text , 64 );
		::rux::safe_sprintf( text , 64 , "%.2f" , _value );
		return XString( text , XEnumCodePage_UTF8 , 1 , NULL , __FILE__ , __LINE__ )++;
	};
	bool Double::Equals( XGCRef* gcref )
	{
		if( gcref == this )
			return true;
		else if( rux_is_object_ptr( gcref , ::rux::XDouble ) )
		{
			::rux::XDouble val( *gcref );
			return val == *this;
		}
		else
			return false;
	};
};