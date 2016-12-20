#ifndef XAN_INT32_H
#define XAN_INT32_H
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
	begin_declare_rux_class( Int32 );
		friend class ::rux::data::XJsonSerializer;
		friend struct ::rux::data::key_value_pair_t;

		friend class ::rux::Numeric;
		friend class ::rux::XNumeric;
		friend ::rux::int32 rux::gcref_to_int32( const XGCRef* object );
		virtual ::rux::String* ObjectToString( void );
		::rux::String& ToString( void );
		::rux::int32 operator()() const;
		Int32& operator =(::rux::int32 value);	
		bool operator ==(const Int32& value);
		bool operator ==(::rux::int32 value);	
		bool operator !=(const Int32& value);
		bool operator !=(::rux::int32 value);	
		Int32& operator +=(const Int32& value);
		Int32& operator +=(::rux::int32 value);
		Int32& operator -=(const Int32& value);
		Int32& operator -=(::rux::int32 value);
		Int32& operator =(const Int32& value);	
		virtual bool Equals( XGCRef* gcref );
	begin_declare_rux_class_members( Int32 );
		::rux::int32 _value;
	end_declare_rux_class_members( Int32 );
		XInt32( ::rux::int32 value );
		friend ::rux::int32 operator +(const XInt32& value0, const XInt32& value1);	
		friend ::rux::int32 operator *(const XInt32& value0, const XInt32& value1);	
		friend ::rux::int32 operator /(const XInt32& value0, const XInt32& value1);	
		friend ::rux::int32 operator -(const XInt32& value0, const XInt32& value1);	
		friend ::rux::int32 operator +(::rux::int32 value0, const XInt32& value1);	
		friend ::rux::int32 operator *(::rux::int32 value0, const XInt32& value1);	
		friend ::rux::int32 operator /(::rux::int32 value0, const XInt32& value1);	
		friend ::rux::int32 operator -(::rux::int32 value0, const XInt32& value1);	
		friend ::rux::int32 operator +(const XInt32& value0, ::rux::int32 value1);	
		friend ::rux::int32 operator *(const XInt32& value0, ::rux::int32 value1);	
		friend ::rux::int32 operator /(const XInt32& value0, ::rux::int32 value1);	
		friend ::rux::int32 operator -(const XInt32& value0, ::rux::int32 value1);	
		XInt32& operator =(::rux::int32 value);	
		XInt32& operator=( XObject& object );
		bool operator ==(const XInt32& value);
		bool operator ==(::rux::int32 value);	
		bool operator !=(const XInt32& value);
		bool operator !=(::rux::int32 value);	
		XInt32& operator +=(const XInt32& value);
		XInt32& operator +=(::rux::int32 value);
		XInt32& operator -=(const XInt32& value);
		XInt32& operator -=(::rux::int32 value);
		::rux::int32 operator()() const;		
		::rux::String& ToString( void );
	end_declare_rux_class();
};
#endif
