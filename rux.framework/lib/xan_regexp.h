#ifndef XAN_REGEXP_H
#define XAN_REGEXP_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_string.h>
namespace rux
{	
	begin_declare_rux_class( RegexpChar );		
		friend class XRegexp;
		friend class XRegexpPattern;
		::rux::uint8 IsHardValue( void );
		bool Equals( const ::rux::XString& string , ::rux::uint32& index , ::rux::uint8* is_hard_value , ::rux::uint32 symbol_index , ::rux::uint32& entries );
	begin_declare_rux_class_members( RegexpChar );
		XMallocArray< ::rux::uint32 > _utf8_characters;
		::rux::uint32 _min_entries;
		::rux::uint32 _max_entries;
		::rux::uint8 _is_equal;
		::rux::uint8 _is_letter;
		::rux::uint8 _is_any;
	end_declare_rux_class_members( RegexpChar );
		bool Equals( const ::rux::XString& string , ::rux::uint32& index , ::rux::uint8* is_hard_value , ::rux::uint32 symbol_index , ::rux::uint32& entries );
		void Correct( ::rux::XString& string , ::rux::uint32& index , ::rux::uint32& entries );
		::rux::uint8 IsHardValue( void );
	end_declare_rux_class();

	begin_declare_rux_class( RegexpGroup );
		friend class XRegexp;
		friend class XRegexpPattern;
		bool Equals( const ::rux::XString& string , ::rux::uint32& index , ::rux::uint32& most_matches_index , ::rux::uint32& matches , ::rux::uint8* is_hard_value , ::rux::uint32 symbol_index );
	begin_declare_rux_class_members( RegexpGroup );
		XArray< XObject > _objects;
		::rux::uint32 _min_entries;
		::rux::uint32 _max_entries;
	end_declare_rux_class_members( RegexpGroup );
		bool Equals( const ::rux::XString& string , ::rux::uint32& index , ::rux::uint32& most_matches_index , ::rux::uint32& matches , ::rux::uint8* is_hard_value , ::rux::uint32 symbol_index );
		void Correct( ::rux::XString& string , ::rux::uint32& index , ::rux::uint32 most_matches_index , ::rux::uint32& entries );
	end_declare_rux_class();

	begin_declare_rux_class( RegexpPattern );
		friend class XRegexp;
	begin_declare_rux_class_members( RegexpPattern );
		XArray< XObject > _objects;
	end_declare_rux_class_members( RegexpPattern );
		bool Equals( const ::rux::XString& string , ::rux::uint32& index , ::rux::uint32& matches , ::rux::uint8* is_hard_value , ::rux::uint32 symbol_index );
		void Correct( ::rux::XString& string , ::rux::uint32& index);
		::rux::uint8 IsHardValue( const ::rux::XString& string , size_t index );
	end_declare_rux_class();

	class XRegexp
	{
	public:
		static ::rux::Array< ::rux::XString >& FindAll( const ::rux::XString& string , const XRegexpPattern& regex_pattern );
		static RegexpPattern& get_RegexpPattern( const ::rux::XString& regular_expression , ::rux::XString& error );
		static bool IndexOf( const ::rux::XString& string , ::rux::uint32& index , ::rux::uint32& count , const XRegexpPattern& regex_pattern , ::rux::uint8* is_hard_value = NULL , ::rux::uint32 symbol_index = 0 );
		static bool LastIndexOf( const ::rux::XString& string , ::rux::uint32& index , ::rux::uint32& count , const XRegexpPattern& regex_pattern );	
		static String& Correct( const ::rux::XString& string , const XRegexpPattern& regex_pattern );
		static String& TimeMask( void );
		static String& DateMask( void );
		static String& Ipv4Mask( void );
		static String& IpPortMask( void );
		static String& NumberMask( void );
		static ::rux::uint8 IsHardValue( const ::rux::XString& string , const XRegexpPattern& regex_pattern , size_t symbol_index );
	private:
		static RegexpGroup& get_RegexpGroup( const ::rux::XString& regular_expression , ::rux::uint32& utf8_index , ::rux::uint32& utf8 , XObject& last_object , ::rux::XString& error );
	};
};
#endif
