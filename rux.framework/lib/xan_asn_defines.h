#ifndef XAN_ASN_DEFINES_H
#define XAN_ASN_DEFINES_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_string.h>
enum XEnum_AsnClass
{
	XEnum_AsnClass_Universal,
	XEnum_AsnClass_Application,
	XEnum_AsnClass_ContextSpecific,
	XEnum_AsnClass_Private
};
enum XEnum_AsnUniversalClassTags
{
	XEnum_AsnUniversalClassTags_EOC = 0 ,
	XEnum_AsnUniversalClassTags_BOOLEAN = 1 ,
	XEnum_AsnUniversalClassTags_INTEGER = 2 ,
	XEnum_AsnUniversalClassTags_BITSTRING = 3 ,
	XEnum_AsnUniversalClassTags_OCTETSTRING = 4 ,
	XEnum_AsnUniversalClassTags_NULL = 5 ,
	XEnum_AsnUniversalClassTags_OBJECTIDENTIFIER = 6 , 
	XEnum_AsnUniversalClassTags_ObjectDescriptor = 7 ,
	XEnum_AsnUniversalClassTags_EXTERNAL = 8 ,
	XEnum_AsnUniversalClassTags_REAL = 9 ,
	XEnum_AsnUniversalClassTags_ENUMERATED = 10 ,
	XEnum_AsnUniversalClassTags_EMBEDDEDPDV = 11 ,
	XEnum_AsnUniversalClassTags_Utf8String = 12 , 
	XEnum_AsnUniversalClassTags_RELATIVEOID = 13 , 	
	XEnum_AsnUniversalClassTags_SEQUENCE = 16 , 
	XEnum_AsnUniversalClassTags_SET = 17 , 
	XEnum_AsnUniversalClassTags_NumericString = 18 , 
	XEnum_AsnUniversalClassTags_PrintableString = 19 ,
	XEnum_AsnUniversalClassTags_T61String = 20 ,
	XEnum_AsnUniversalClassTags_VideotexString = 21 , 
	XEnum_AsnUniversalClassTags_IA5String = 22 , 
	XEnum_AsnUniversalClassTags_UTCTime = 23 ,
	XEnum_AsnUniversalClassTags_GeneralizedTime = 24 , 
	XEnum_AsnUniversalClassTags_GraphicString = 25 ,
	XEnum_AsnUniversalClassTags_VisibleString = 26 , 
	XEnum_AsnUniversalClassTags_GeneralString = 27 ,
	XEnum_AsnUniversalClassTags_UniversalString = 28 , 
	XEnum_AsnUniversalClassTags_CHARACTERSTRING = 29 , 
	XEnum_AsnUniversalClassTags_BMPString	 = 30
};
rux_inline void rux_convert_asn_objectidentifier_to_string( const rux::uint8* data_ptr , size_t& position ,
	size_t& asn_length , size_t& index , ::rux::uint32& value , size_t& multiplicator , rux::XString& oid , rux::XString& error )
{
	oid.Clear();
	index = asn_length;
	asn_length--;
	multiplicator = 0x80;
	while( asn_length >= 0 )
	{
		if( asn_length == 0 )
		{
			if( data_ptr[ position + asn_length ] >= 0x28 )
			{
				oid.InsertRange( 0, rux::XString( "." + rux::XString::Format( "%ld" , value ) ) );
				value = data_ptr[ position + asn_length ] % 0x28;
				oid.InsertRange( 0, rux::XString( "." + rux::XString::Format( "%ld" , value ) ) );
				value = data_ptr[ position + asn_length ] / 0x28;
				oid.InsertRange( 0, rux::XString( rux::XString::Format( "%ld" , value ) ) );
			}
			else
				error = "Object identifier buffer contains incorrect first byte";
			break;
		}
		else if( data_ptr[ position + asn_length ] < 0x80 )
		{
			if( multiplicator == 0x80 )
			{
				if( asn_length + 1 != index )
					oid.InsertRange( 0, rux::XString( "." + rux::XString::Format( "%ld" , value ) ) );																				
				value = data_ptr[ position + asn_length ];
				multiplicator = 0x80;
			}
			else
			{
				value += data_ptr[ position + asn_length ];
				multiplicator = 0x80;
			}			
		}
		else
		{
			value += ( data_ptr[ position + asn_length ] - 0x80 ) * multiplicator;
			multiplicator *= 0x80;
		}
		asn_length--;
	}
};
rux_inline void rux_parse_asn_header( const rux::uint8* data_ptr , rux::uint32& data_ptr_length , rux::uint8& length_of_length , rux::uint8& is_set_8bit , rux::uint8& is_set_7bit , size_t& position , rux::uint8& decrementer , rux::uint8& incrementer ,
																		 XEnum_AsnClass& asn_class , XEnum_AsnUniversalClassTags& asn_universal_class_tags ,
																		 rux::uint8& is_constructed , size_t& asn_length , rux::XString& error )
{
	is_set_8bit = ( data_ptr[ position ] & 128 ) > 0 ? 1 : 0;
	is_set_7bit = ( data_ptr[ position ] & 64 ) > 0 ? 1 : 0;
	if( is_set_8bit && is_set_7bit )
		asn_class = XEnum_AsnClass_Private;
	else if( is_set_8bit )
		asn_class = XEnum_AsnClass_ContextSpecific;
	else if( is_set_7bit )
		asn_class = XEnum_AsnClass_Application;
	else
		asn_class = XEnum_AsnClass_Universal;
	is_constructed = ( data_ptr[ position ] & 32 ) > 0 ? 1 : 0;
	asn_universal_class_tags = (XEnum_AsnUniversalClassTags)( data_ptr[ position ] & ~ 224 );
	position++;
	length_of_length = 0;				
	if( (rux::uint8)data_ptr[ position ] > (rux::uint8)0x7f )
	{
		length_of_length = data_ptr[ position ] - 0x80;
		position++;
		if( data_ptr_length >= position + length_of_length )
		{
			decrementer = length_of_length - 1;
			for( incrementer = 0 ; incrementer < length_of_length ; incrementer++ )
			{
				( (char*)&asn_length )[ incrementer ] = data_ptr[ position + decrementer];									
				decrementer--;
			}
			position += length_of_length;
		}
		else
		{
			error = "Length of ASN length is higher data pointer length";							
		}
	}
	else			
	{
		asn_length = data_ptr[ position ];
		position++;
	}
};
namespace rux
{
	namespace network
	{
		struct AsnPacket
		{
			AsnPacket();
			~AsnPacket();
			XEnum_AsnClass _asn_class;
			rux::uint8 _is_constructed;
			XEnum_AsnUniversalClassTags _asn_universal_class_tags;
			size_t _buffer_ptr_size;
			size_t _buffer_ptr_length;
			char* _buffer_ptr;
			rux::uint8 _is_delete;	
			void fill( XEnum_AsnClass asn_class , rux::uint8 is_constructed , XEnum_AsnUniversalClassTags asn_universal_class_tags , char* value_ptr , rux::uint32 value_length );
			void fill( XEnum_AsnClass asn_class , rux::uint8 is_constructed , XEnum_AsnUniversalClassTags asn_universal_class_tags , XMallocArray< rux::network::AsnPacket* >& asn_packets );
			void fill( XEnum_AsnClass asn_class , rux::uint8 is_constructed , XEnum_AsnUniversalClassTags asn_universal_class_tags , rux::uint32 value );
			void fill( XEnum_AsnClass asn_class , rux::uint8 is_constructed , XEnum_AsnUniversalClassTags asn_universal_class_tags );
			void fill_oid( const char* oid_string );
			static void format( rux::network::AsnPacket& asn_packet , rux::uint8*& network_buffer_ptr , size_t& network_buffer_size );
			static size_t format( rux::network::AsnPacket& asn_packet , rux::uint8* network_buffer_ptr );
			static size_t parse( rux::network::AsnPacket& asn_packet , rux::uint8* network_buffer_ptr , size_t network_buffer_size , rux::XString& error );
			static void parse( rux::network::AsnPacket& asn_packet , XMallocArray< rux::network::AsnPacket* >& asn_packets , rux::XString& error );
			static rux::uint32 get_length( rux::uint8* network_buffer_ptr , size_t network_buffer_size , rux::XString& error );
			static void free_packets( XMallocArray< rux::network::AsnPacket* >& asn_packets );
			void oid_to_string( rux::XString& oid , rux::XString& error );
		};
	};
};
#endif
