#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_boolean.h"
#include "xan_uint64.h"
begin_implement_rux_class_without_operators_ns( Boolean , rux )
	_value = 0;
end_implement_rux_class();
namespace rux
{	
	XBoolean::XBoolean( ::rux::uint8 value )
	{
		(*this) = value;
	};
	XBoolean::XBoolean( bool value )
	{
		(*this) = value == true ? (::rux::uint8)1 : (::rux::uint8)0;
	};
	XBoolean& XBoolean::operator =(const XBoolean& value)
	{
		(*static_cast<XGCHandle<Boolean>*>(this))()->_value = (*static_cast<const XGCHandle<Boolean>*>(&value))()->_value;
		return *this;
	};
	XBoolean& XBoolean::operator =(::rux::uint8 value)
	{
		(*static_cast<XGCHandle<Boolean>*>(this))()->_value = value;
		return *this;
	};
	XBoolean& XBoolean::operator =(bool value)
	{
		(*static_cast<XGCHandle<Boolean>*>(this))()->_value = value == true ? 1 : 0;
		return *this;
	};
	XBoolean& XBoolean::operator =(const Boolean& value)
	{
		(*static_cast<XGCHandle<Boolean>*>(this))()->_value = value._value;
		return *this;
	};
	XBoolean& XBoolean::operator=( XObject& object )
	{
		*static_cast<XGCHandle<Boolean>*>(this) = object;
		return *this;
	};
	bool XBoolean::operator ==(const XBoolean& value)
	{
		return (*static_cast<XGCHandle<Boolean>*>(this))()->_value == (*static_cast<const XGCHandle<Boolean>*>(&value))()->_value;
	};
	bool XBoolean::operator ==(::rux::uint8 value)
	{
		return (*static_cast<XGCHandle<Boolean>*>(this))()->_value == value;
	};
	bool XBoolean::operator ==(bool value)
	{
	::rux::uint8 uint8_value = value == true ? 1 : 0;
		return (*static_cast<XGCHandle<Boolean>*>(this))()->_value == uint8_value;
	};
	bool XBoolean::operator !=(const XBoolean& value)
	{
		return (*static_cast<XGCHandle<Boolean>*>(this))()->_value != (*static_cast<const XGCHandle<Boolean>*>(&value))()->_value;
	};
	bool XBoolean::operator !=(::rux::uint8 value)
	{
		return (*static_cast<XGCHandle<Boolean>*>(this))()->_value != value;
	};
	bool XBoolean::operator !=(bool value)
	{
	::rux::uint8 uint8_value = value == true ? 1 : 0;
		return (*static_cast<XGCHandle<Boolean>*>(this))()->_value != uint8_value;
	};
	Boolean& Boolean::operator =(::rux::uint8 value)
	{
		_value = value;
		return *this;
	};
	Boolean& Boolean::operator =(bool value)
	{
		_value = value == true ? 1 : 0;
		return *this;
	};
	Boolean& Boolean::operator =(const Boolean& value)
	{
		_value = value._value;
		return *this;
	};
	bool Boolean::operator ==(::rux::uint8 value)
	{
		return _value == value;
	};
	bool Boolean::operator ==(bool value)
	{
	::rux::uint8 uint8_value = value == true ? 1 : 0;
		return _value == uint8_value;
	};
	bool Boolean::operator !=(::rux::uint8 value)
	{
		return _value != value;
	};
	bool Boolean::operator !=(bool value)
	{
		::rux::uint8 uint8_value = value == true ? 1 : 0;
		return _value != uint8_value;
	};
	::rux::uint8 XBoolean::operator()() const
	{
		return (*static_cast<XGCHandle<Boolean>*>( const_cast<XBoolean*>( this ) ))()->_value;
	};
	::rux::uint8 Boolean::operator()() const
	{
		return _value;
	};
	::rux::String* Boolean::ObjectToString( void )
	{		
		XString res( ToString() );
		return &res++;
	};
	implement_duplicate_internal_result_function( ::rux::String& , Boolean , ToString );
	::rux::String& Boolean::ToString( void )
	{
		if( _value == 1 )
			return XString( "true" )++;
		else
			return XString( "false" )++;
	};
	bool Boolean::Equals( XGCRef* gcref )
	{
		if( gcref == this )
			return true;
		else if( rux_is_object_ptr( gcref , ::rux::XBoolean ) )
		{
			::rux::XBoolean val( *gcref );
			return val == *this;
		}
		else
			return false;
	};
};