#ifndef XAN_DOUBLE_H
#define XAN_DOUBLE_H
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
	begin_declare_rux_class( Double );
		friend class ::rux::data::XJsonSerializer;
		friend struct ::rux::data::key_value_pair_t;

		friend class ::rux::Numeric;
		friend class ::rux::XNumeric;
		Double& operator =(double value);
		Double& operator =(const Double& value);
		bool operator ==(double value);
		bool operator !=(double value);
		Double& operator +=(double value);
		Double& operator -=(double value);
		virtual ::rux::String* ObjectToString( void );
		::rux::String& ToString( void );
		double operator()() const;
		bool operator ==(const Double& value);
		bool operator !=(const Double& value);
		Double& operator +=(const Double& value);
		Double& operator -=(const Double& value);
		virtual bool Equals( XGCRef* gcref );
	begin_declare_rux_class_members( Double );
		double _value;
	end_declare_rux_class_members( Double );
		XDouble( double value );
		friend double operator +(const XDouble& value0, const XDouble& value1);	
		friend double operator *(const XDouble& value0, XDouble& value1);	
		friend double operator /(const XDouble& value0, const XDouble& value1);	
		friend double operator -(const XDouble& value0, const XDouble& value1);	
		friend double operator +(double value0, const XDouble& value1);	
		friend double operator *(double value0, const XDouble& value1);	
		friend double operator /(double value0, const XDouble& value1);	
		friend double operator -(double value0, const XDouble& value1);	
		friend double operator +(const XDouble& value0, double value1);	
		friend double operator *(const XDouble& value0, double value1);	
		friend double operator /(const XDouble& value0, double value1);	
		friend double operator -(const XDouble& value0, double value1);	
		XDouble& operator =(double value);	
		XDouble& operator=( XObject& object );
		bool operator ==(const XDouble& value);
		bool operator ==(double value);	
		bool operator !=(const XDouble& value);
		bool operator !=(double value);	
		XDouble& operator +=(const XDouble& value);
		XDouble& operator +=(double value);
		XDouble& operator -=(const XDouble& value);
		XDouble& operator -=(double value);
		double operator()() const;		
		::rux::String& ToString( void );
	end_declare_rux_class();
};
#endif
