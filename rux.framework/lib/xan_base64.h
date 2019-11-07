#ifndef XAN_BASE64_H
#define XAN_BASE64_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_string.h"
namespace rux
{
	namespace data
	{
		enum Base64_Type
		{
			Base64_Type_RFC1421 ,
			Base64_Type_RFC2045 ,
			Base64_Type_RFC3548 ,
			Base64_Type_Radix64 ,
			Base64_Type_UTF7 ,
			Base64_Type_URL ,
			Base64_Type_Y64 ,
			Base64_Type_XMLNameTokens ,
			Base64_Type_XMLIdentifiers ,
			Base64_Type_ProgramIdentifiers1 ,
			Base64_Type_ProgramIdentifiers2 ,
			Base64_Type_Regexp
		};
		class XBase64
		{
		public:
			static size_t get_EncodedSize( size_t decoded_data_ptr_size , rux::byte padding );
			static size_t get_DecodedSize( const char* encoded_data , size_t size , size_t& actual_size );
			static void Decode( const char* encoded_data , ::rux::uint8*& decoded_data_ptr , size_t& decoded_data_ptr_size , ::rux::XString& error );
			static String& Encode( ::rux::uint8* decoded_data_ptr , size_t decoded_data_ptr_size , ::rux::XString& error , ::rux::byte web = 0 , ::rux::byte padding = 0 );
			static void Encode( char*& destination_ptr , size_t& destination_size , size_t& destination_length , ::rux::uint8* decoded_data_ptr , size_t decoded_data_ptr_size , ::rux::XString& error , ::rux::byte web = 0 , ::rux::byte padding = 0 );
		};
	};
};
#endif
