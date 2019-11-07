#ifndef XAN_NUMERIC_H
#define XAN_NUMERIC_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_float.h"
#include "xan_double.h"
#include "xan_int32.h"
#include "xan_uint32.h"
#include "xan_int64.h"
#include "xan_uint64.h"
namespace rux
{
	class Expr;
	namespace data
	{
		class XJsonSerializer;
		struct key_value_pair_t;
	};
	begin_declare_rux_class( Numeric );
		~Numeric( void );
		friend class ::rux::Expr;
		friend class ::rux::data::XJsonSerializer;
		friend struct ::rux::data::key_value_pair_t;

		Numeric& operator =( double value );
		Numeric& operator =( const Double& value );
		Numeric& operator =( float value );
		Numeric& operator =( const Float& value );
		Numeric& operator =( ::rux::int32 value );
		Numeric& operator =( const ::rux::Int32& value );
		Numeric& operator =( ::rux::uint32 value );
		Numeric& operator =( const ::rux::UInt32& value );
		Numeric& operator =( ::rux::int64 value );
		Numeric& operator =( const ::rux::Int64& value );
		Numeric& operator =( ::rux::uint64 value );
		Numeric& operator =( const ::rux::UInt64& value );
		Numeric& operator=( const XObject& object );
		Numeric& operator=( const XGCRef* object );
		Numeric& operator =( const Numeric* value );
		virtual ::rux::String* ObjectToString( void );
		::rux::String& ToString( void );
		virtual bool Equals( XGCRef* gcref );
		float to_float( void );
		double to_double( void );
		::rux::int32 to_int32( void );
		::rux::int64 to_int64( void );
		::rux::uint32 to_uint32( void );
		::rux::uint64 to_uint64( void );
		XGCRef* _value_gcref;
	begin_declare_rux_class_members( Numeric );
	end_declare_rux_class_members( Numeric );
		XNumeric( double value );
		XNumeric( float value );
		XNumeric( ::rux::int32 value );
		XNumeric( ::rux::int64 value );
		XNumeric( ::rux::uint32 value );
		XNumeric( ::rux::uint64 value );
		XNumeric& operator =( double value );
		XNumeric& operator =( const ::rux::Double& value );
		XNumeric& operator =( float value );
		XNumeric& operator =( const ::rux::Float& value );
		XNumeric& operator =( ::rux::int32 value );
		XNumeric& operator =( const ::rux::Int32& value );
		XNumeric& operator =( ::rux::uint32 value );
		XNumeric& operator =( const ::rux::UInt32& value );
		XNumeric& operator =( ::rux::int64 value );
		XNumeric& operator =( const ::rux::Int64& value );
		XNumeric& operator =( ::rux::uint64 value );
		XNumeric& operator =( const ::rux::UInt64& value );
		XNumeric& operator =( const ::rux::XDouble& value );
		XNumeric& operator =( const ::rux::XFloat& value );
		XNumeric& operator =( const ::rux::XInt32& value );
		XNumeric& operator =( const ::rux::XUInt32& value );
		XNumeric& operator =( const ::rux::XInt64& value );
		XNumeric& operator =( const ::rux::XUInt64& value );
		::rux::String& ToString( void );
		float to_float( void );
		double to_double( void );
		::rux::int32 to_int32( void );
		::rux::int64 to_int64( void );
		::rux::uint32 to_uint32( void );
		::rux::uint64 to_uint64( void );
		void set_ByRef( const XObject& type ) const;
		void set_ByRef( const Object& type ) const;
		void set_ByRef( const XGCRef& type ) const;
		void set_ByRef( XGCRef* type ) const;
	end_declare_rux_class();
};
#endif
