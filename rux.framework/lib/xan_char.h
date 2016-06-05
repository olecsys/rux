#ifndef XAN_CHAR_H
#define XAN_CHAR_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_array.h>
namespace rux
{
	class String;
	class XString;
	begin_declare_rux_class( Char );
		friend class ::rux::String;
		friend class ::rux::XString;
		Char& operator =(const Char& value);
		Char& operator =(char value);
		bool operator ==(char value);
		bool operator !=(char value);
		bool operator !=(const Char& value);
		char operator()();
	begin_declare_rux_class_members( Char );
		char _char;
	end_declare_rux_class_members( Char );
		XChar( char value );
		XChar& operator =(char value);	
		bool operator ==(const XChar& value);
		bool operator ==(char value);	
		bool operator !=(const XChar& value);
		bool operator !=(char value);		
		char operator()();
	end_declare_rux_class();
};
#endif
