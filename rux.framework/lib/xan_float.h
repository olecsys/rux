#ifndef XAN_FLOAT_H
#define XAN_FLOAT_H
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
	begin_declare_rux_class( Float );
		friend class ::rux::data::XJsonSerializer;
		friend struct ::rux::data::key_value_pair_t;

		friend class ::rux::Numeric;
		friend class ::rux::XNumeric;
		friend dll_internal float gcref_to_float( const XGCRef* object );
		virtual ::rux::String* ObjectToString( void );
		::rux::String& ToString( void );
		float operator()() const;
		Float& operator =(float value);	
		Float& operator=( XObject& object );
		bool operator ==(const Float& value);
		bool operator ==(float value);	
		bool operator !=(const Float& value);
		bool operator !=(float value);	
		Float& operator +=(const Float& value);
		Float& operator +=(float value);
		Float& operator -=(const Float& value);
		Float& operator -=(float value);
		Float& operator =(const Float& value);
		virtual bool Equals( XGCRef* gcref );
	begin_declare_rux_class_members( Float );
		float _value;
	end_declare_rux_class_members( Float );
		XFloat( float value );
		friend float operator +(const XFloat& value0, const XFloat& value1);	
		friend float operator *(const XFloat& value0, XFloat& value1);	
		friend float operator /(const XFloat& value0, const XFloat& value1);	
		friend float operator -(const XFloat& value0, const XFloat& value1);	
		friend float operator +(float value0, const XFloat& value1);	
		friend float operator *(float value0, const XFloat& value1);	
		friend float operator /(float value0, const XFloat& value1);	
		friend float operator -(float value0, const XFloat& value1);	
		friend float operator +(const XFloat& value0, float value1);	
		friend float operator *(const XFloat& value0, float value1);	
		friend float operator /(const XFloat& value0, float value1);	
		friend float operator -(const XFloat& value0, float value1);	
		XFloat& operator =(float value);	
		XFloat& operator=( XObject& object );
		bool operator ==(const XFloat& value);
		bool operator ==(float value);	
		bool operator !=(const XFloat& value);
		bool operator !=(float value);	
		XFloat& operator +=(const XFloat& value);
		XFloat& operator +=(float value);
		XFloat& operator -=(const XFloat& value);
		XFloat& operator -=(float value);
		float operator()() const;		
		::rux::String& ToString();
	end_declare_rux_class();
};
#endif
