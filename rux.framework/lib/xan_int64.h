#ifndef XAN_INT64_H
#define XAN_INT64_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_string.h"
namespace rux
{
	class Numeric;
	class XNumeric;
	namespace data
	{
		class XJsonSerializer;
		struct key_value_pair_t;
	};
	begin_declare_rux_class( Int64 );
		friend class ::rux::data::XJsonSerializer;
		friend struct ::rux::data::key_value_pair_t;

		friend class ::rux::Numeric;
		friend class ::rux::XNumeric;
		virtual ::rux::String* ObjectToString( void );
		::rux::String& ToString( void );
		::rux::int64 operator()() const;
		Int64& operator =(::rux::int64 value);	
		bool operator ==(const Int64& value);
		bool operator ==(::rux::int64 value);	
		bool operator !=(const Int64& value);
		bool operator !=(::rux::int64 value);	
		Int64& operator +=(const Int64& value);
		Int64& operator +=(::rux::int64 value);
		Int64& operator -=(const Int64& value);
		Int64& operator -=(::rux::int64 value);
		Int64& operator =(const Int64& value);
		virtual bool Equals( XGCRef* gcref );
	begin_declare_rux_class_members( Int64 );
		::rux::int64 _value;
	end_declare_rux_class_members( Int64 );
		XInt64( ::rux::int64 value );
		friend ::rux::int64 operator +(const XInt64& value0, const XInt64& value1);	
		friend ::rux::int64 operator *(const XInt64& value0, const XInt64& value1);	
		friend ::rux::int64 operator /(const XInt64& value0, const XInt64& value1);	
		friend ::rux::int64 operator -(const XInt64& value0, const XInt64& value1);	
		friend ::rux::int64 operator +(::rux::int64 value0, const XInt64& value1);	
		friend ::rux::int64 operator *(::rux::int64 value0, const XInt64& value1);	
		friend ::rux::int64 operator /(::rux::int64 value0, const XInt64& value1);	
		friend ::rux::int64 operator -(::rux::int64 value0, const XInt64& value1);	
		friend ::rux::int64 operator +(const XInt64& value0, ::rux::int64 value1);	
		friend ::rux::int64 operator *(const XInt64& value0, ::rux::int64 value1);	
		friend ::rux::int64 operator /(const XInt64& value0, ::rux::int64 value1);	
		friend ::rux::int64 operator -(const XInt64& value0, ::rux::int64 value1);	
		XInt64& operator =(::rux::int64 value);	
		XInt64& operator=( XObject& object );
		bool operator ==(const XInt64& value);
		bool operator ==(::rux::int64 value);	
		bool operator !=(const XInt64& value);
		bool operator !=(::rux::int64 value);	
		XInt64& operator +=(const XInt64& value);
		XInt64& operator +=(::rux::int64 value);
		XInt64& operator -=(const XInt64& value);
		XInt64& operator -=(::rux::int64 value);
		::rux::int64 operator()() const;
		::rux::String& ToString( void );
		void ToString( char destination[ 512 ] );
	end_declare_rux_class();
};
#endif
