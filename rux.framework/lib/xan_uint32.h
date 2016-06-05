#ifndef XAN_UINT32_H
#define XAN_UINT32_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_string.h>
namespace rux
{
	class Numeric;
	class XNumeric;
	namespace data
	{
		class XJsonSerializer;
		struct key_value_pair_t;
	};
	begin_declare_rux_class( UInt32 );
		friend class ::rux::data::XJsonSerializer;
		friend struct ::rux::data::key_value_pair_t;

		friend class ::rux::Numeric;
		friend class ::rux::XNumeric;
		virtual ::rux::String* ObjectToString( void );
		::rux::String& ToString( void );
		::rux::uint32 operator()() const;
		UInt32& operator =(::rux::uint32 value);	
		bool operator ==(const UInt32& value);
		bool operator ==(::rux::uint32 value);	
		bool operator !=(const UInt32& value);
		bool operator !=(::rux::uint32 value);	
		bool operator >( ::rux::uint32 value );
		bool operator >=( ::rux::uint32 value );
		UInt32& operator =(const UInt32& value);
		bool operator <( ::rux::uint32 value );
		bool operator <=( ::rux::uint32 value );
		UInt32& operator +=(const UInt32& value);
		UInt32& operator +=(::rux::uint32 value);
		UInt32& operator -=(const UInt32& value);
		UInt32& operator -=(::rux::uint32 value);
		virtual bool Equals( XGCRef* gcref );
	begin_declare_rux_class_members( UInt32 );
		::rux::uint32 _value;
	end_declare_rux_class_members( UInt32 );
		XUInt32( ::rux::uint32 value );
		friend ::rux::uint32 operator +(const XUInt32& value0, const XUInt32& value1);	
		friend ::rux::uint32 operator *(const XUInt32& value0, const XUInt32& value1);	
		friend ::rux::uint32 operator /(const XUInt32& value0, const XUInt32& value1);	
		friend ::rux::uint32 operator -(const XUInt32& value0, const XUInt32& value1);	
		friend ::rux::uint32 operator +(::rux::uint32 value0, const XUInt32& value1);	
		friend ::rux::uint32 operator *(::rux::uint32 value0, const XUInt32& value1);	
		friend ::rux::uint32 operator /(::rux::uint32 value0, const XUInt32& value1);	
		friend ::rux::uint32 operator -(::rux::uint32 value0, const XUInt32& value1);	
		friend ::rux::uint32 operator +(const XUInt32& value0, ::rux::uint32 value1);	
		friend ::rux::uint32 operator *(const XUInt32& value0, ::rux::uint32 value1);	
		friend ::rux::uint32 operator /(const XUInt32& value0, ::rux::uint32 value1);	
		friend ::rux::uint32 operator -(const XUInt32& value0, ::rux::uint32 value1);	
		XUInt32& operator =(::rux::uint32 value);	
		XUInt32& operator=( XObject& object );
		bool operator ==(const XUInt32& value);
		bool operator ==(::rux::uint32 value);	
		bool operator !=(const XUInt32& value);
		bool operator !=(::rux::uint32 value);	
		bool operator >( ::rux::uint32 value );
		bool operator >=( ::rux::uint32 value );
		bool operator <( ::rux::uint32 value );
		bool operator <=( ::rux::uint32 value );
		XUInt32& operator +=(const XUInt32& value);
		XUInt32& operator +=(::rux::uint32 value);
		XUInt32& operator -=(const XUInt32& value);
		XUInt32& operator -=(::rux::uint32 value);
		::rux::uint32 operator()() const;
		::rux::String& ToString();
	end_declare_rux_class();
};
#endif
