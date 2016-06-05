#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_char.h"
#include <xan_keyvaluepair.h>
#include <xan_uint64.h>
#include <xan_boolean.h>
begin_implement_rux_class_without_operators_ns( Char , rux )
	_char = '\0';
end_implement_rux_class();
namespace rux
{		
	XChar::XChar( char value )
	{
		(*this) = value;
	};
	XChar& XChar::operator =(const XChar& value)
	{
		(*static_cast<XGCHandle<Char>*>(this))()->_char = (*static_cast<const XGCHandle<Char>*>(&value))()->_char;
		return *this;
	};
	XChar& XChar::operator =(char value)
	{
		(*static_cast<XGCHandle<Char>*>(this))()->_char = value;
		return *this;
	};
	XChar& XChar::operator =(const Char& value)
	{
		(*static_cast<XGCHandle<Char>*>(this))()->_char = value._char;
		return *this;
	};
	bool XChar::operator ==(const XChar& value)
	{
		return (*static_cast<XGCHandle<Char>*>(this))()->_char == (*static_cast<const XGCHandle<Char>*>(&value))()->_char;
	};
	bool XChar::operator ==(char value)
	{
		return (*static_cast<XGCHandle<Char>*>(this))()->_char == value;
	};
	bool XChar::operator !=(const XChar& value)
	{
		return (*static_cast<XGCHandle<Char>*>(this))()->_char != (*static_cast<const XGCHandle<Char>*>(&value))()->_char;
	};
	bool XChar::operator !=(char value)
	{
		return (*static_cast<XGCHandle<Char>*>(this))()->_char != value;
	};
	Char& Char::operator =(const Char& value)
	{
		_char =value._char;
		return *this;
	};
	Char& Char::operator =(char value)
	{
		_char = value;
		return *this;
	};
	bool Char::operator ==(char value)
	{
		return _char == value;
	};
	bool Char::operator !=(const Char& value)
	{
		return _char != value._char;
	};
	bool Char::operator !=(char value)
	{
		return _char != value;
	};
	char Char::operator()()
	{
		return _char;
	};
	char XChar::operator()()
	{
		return (*static_cast<XGCHandle<Char>*>(this))()->_char;
	};
};