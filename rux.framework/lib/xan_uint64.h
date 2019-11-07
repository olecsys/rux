#ifndef XAN_UINT64_H
#define XAN_UINT64_H
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
	begin_declare_rux_class( UInt64 );
		friend class ::rux::data::XJsonSerializer;
		friend struct ::rux::data::key_value_pair_t;

		friend class ::rux::Numeric;
		friend class ::rux::XNumeric;
		friend dll_internal ::rux::uint64 object_to_uint64( Object* object );
		friend dll_internal ::rux::uint64 gcref_to_uint64( const XGCRef* object );
		virtual ::rux::String* ObjectToString( void );
		::rux::String& ToString( void );
		::rux::uint64 operator()() const;
		UInt64& operator =(::rux::uint64 value);
		bool operator ==(const UInt64& value);
		bool operator ==(::rux::uint64 value);	
		bool operator !=(const UInt64& value);
		bool operator !=(::rux::uint64 value);	
		UInt64& operator +=(const UInt64& value);
		UInt64& operator +=(::rux::uint64 value);
		UInt64& operator -=(const UInt64& value);
		UInt64& operator -=(::rux::uint64 value);
		UInt64& operator =(const UInt64& value);
		virtual bool Equals( XGCRef* gcref );
	begin_declare_rux_class_members( UInt64 );
		::rux::uint64 _value;
	end_declare_rux_class_members( UInt64 );
		XUInt64( ::rux::uint64 value );
		friend ::rux::uint64 operator +(const XUInt64& value0, const XUInt64& value1);	
		friend ::rux::uint64 operator *(const XUInt64& value0, const XUInt64& value1);	
		friend ::rux::uint64 operator /(const XUInt64& value0, const XUInt64& value1);	
		friend ::rux::uint64 operator -(const XUInt64& value0, const XUInt64& value1);	
		friend ::rux::uint64 operator +(::rux::uint64 value0, const XUInt64& value1);	
		friend ::rux::uint64 operator *(::rux::uint64 value0, const XUInt64& value1);	
		friend ::rux::uint64 operator /(::rux::uint64 value0, const XUInt64& value1);	
		friend ::rux::uint64 operator -(::rux::uint64 value0, const XUInt64& value1);	
		friend ::rux::uint64 operator +(const XUInt64& value0, ::rux::uint64 value1);	
		friend ::rux::uint64 operator *(const XUInt64& value0, ::rux::uint64 value1);	
		friend ::rux::uint64 operator /(const XUInt64& value0, ::rux::uint64 value1);	
		friend ::rux::uint64 operator -(const XUInt64& value0, ::rux::uint64 value1);	
		XUInt64& operator =(::rux::uint64 value);	
		XUInt64& operator=( XObject& object );
		bool operator ==(const XUInt64& value);
		bool operator ==(::rux::uint64 value);	
		bool operator !=(const XUInt64& value);
		bool operator !=(::rux::uint64 value);	
		XUInt64& operator +=(const XUInt64& value);
		XUInt64& operator +=(::rux::uint64 value);
		XUInt64& operator -=(const XUInt64& value);
		XUInt64& operator -=(::rux::uint64 value);
		::rux::uint64 operator()() const;
		::rux::String& ToString( void );
	end_declare_rux_class();
};
#endif
