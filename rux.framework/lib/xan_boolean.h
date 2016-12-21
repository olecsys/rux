#ifndef XAN_BOOLEAN_H
#define XAN_BOOLEAN_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_keyvaluepair.h>
namespace rux
{
	namespace data
	{
		class XJsonSerializer;
		struct key_value_pair_t;
	};
	begin_declare_rux_class( Boolean );
		friend class ::rux::data::XJsonSerializer;
		friend struct ::rux::data::key_value_pair_t;
		friend dll_internal ::rux::byte gcref_to_byte( const XGCRef* object );
		friend dll_internal bool is_boolean_and_true( XGCRef* object );
		virtual ::rux::String* ObjectToString( void );
		::rux::String& ToString( void );
		Boolean& operator =(const Boolean& value);
		Boolean( ::rux::uint8 value );
		Boolean( bool value );
		Boolean& operator =(::rux::uint8 value);	
		Boolean& operator =(bool value);
		bool operator ==(::rux::uint8 value);	
		bool operator ==(bool value);
		bool operator !=(::rux::uint8 value);		
		bool operator !=(bool value);		
		::rux::uint8 operator()() const;		
		virtual bool Equals( XGCRef* gcref );
	begin_declare_rux_class_members( Boolean );
		::rux::uint8 _value;
	end_declare_rux_class_members( Boolean );
		XBoolean( ::rux::uint8 value );
		XBoolean( bool value );
		XBoolean& operator =(::rux::uint8 value);	
		XBoolean& operator =(bool value);
		XBoolean& operator=( XObject& object );
		bool operator ==(const XBoolean& value);
		bool operator ==(::rux::uint8 value);	
		bool operator ==(bool value);	
		bool operator !=(const XBoolean& value);
		bool operator !=(::rux::uint8 value);		
		bool operator !=(bool value);		
		::rux::uint8 operator()() const;		
		::rux::String& ToString( void );
	end_declare_rux_class();
};
#endif
