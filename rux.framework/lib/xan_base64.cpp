#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_base64.h"
#include <xan_error.h>
#include <xan_keyvaluepair.h>
#include <xan_uint64.h>
#include <xan_boolean.h>
namespace rux
{
	namespace data
	{
		const ::rux::uint8 encode_6_to_8[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";
		const ::rux::uint8 web_encode_6_to_8[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
		rux_inline ::rux::uint8 encode_8_to_6( ::rux::uint8 symbol )
		{
			if( 'A' <= symbol && symbol <= 'Z' )
				return symbol - 'A';
			else if( 'a' <= symbol && symbol <= 'z' )
				return ( 'Z' - 'A' + 1 ) + symbol - 'a';
			else if( '0' <= symbol && symbol <= '9' )
				return 2 * ( 'Z' - 'A' + 1 ) + symbol - '0';
			else if( symbol == '-' )
				return 62;
			else if( symbol == '_' )
				return 63;
			else if( symbol == '+' )
				return 62;
			else if( symbol == '/' )
				return 63;
			return 0;
		}
		void XBase64::Decode( const char* encoded_data , ::rux::uint8*& decoded_data_ptr , size_t& decoded_data_ptr_size , ::rux::XString& error )
		{
			error.Clear();	
			size_t size = 0;
			if( encoded_data && ( size = strlen( encoded_data ) ) > 0 )
			{		
				size_t actual_size = 0;
				size_t decoded_data_ptr_size2 = get_DecodedSize( encoded_data , size , actual_size );
				if( decoded_data_ptr_size2 != SIZE_MAX )
				{
					if( decoded_data_ptr_size2 > 0 )
					{
						if( decoded_data_ptr_size2 != decoded_data_ptr_size )
						{
							if( decoded_data_ptr != NULL )
								::rux::engine::free_mem( decoded_data_ptr );
							decoded_data_ptr = alloc_array_macros( ::rux::uint8 , (size_t)decoded_data_ptr_size2 );
							decoded_data_ptr_size = decoded_data_ptr_size2;
						}
						::rux::uint8* begin_ptr = (::rux::uint8*)encoded_data;
						::rux::uint8* end_ptr = begin_ptr + actual_size;
						::rux::uint8* ptr_ptr = decoded_data_ptr;
						::rux::uint8 in[ 4 ] = { 0 };
						while( end_ptr - begin_ptr >= 4 ) 
						{
							for( size_t index0 = 0 ; index0 < 4 ; index0++ )
								in[ index0 ] = encode_8_to_6( begin_ptr[ index0 ] );
							ptr_ptr[ 0 ] = (::rux::uint8)( in[ 0 ] << 2 | in[ 1 ] >> 4 );
							ptr_ptr[ 1 ] = (::rux::uint8)( in[ 1 ] << 4 | in[ 2 ] >> 2 );
							ptr_ptr[ 2 ] = (::rux::uint8)( ( ( in[ 2 ] << 6 ) & 0xc0 ) | in[ 3 ] );
							begin_ptr += 4;
							ptr_ptr += 3;
						}
						if( end_ptr != begin_ptr )
						{
							size_t length = end_ptr - begin_ptr;
							for( size_t index0 = 0 ; index0 < length ; index0++ )
								in[ index0 ] = encode_8_to_6( begin_ptr[ index0 ] );
							ptr_ptr[ 0 ] = (::rux::uint8)( in[ 0 ] << 2 | in[ 1 ] >> 4 );
							if( length > 2 )
							{
								ptr_ptr[ 1 ] = (::rux::uint8)( in[ 1 ] << 4 | in[ 2 ] >> 2 );
								if( length > 3 )
									ptr_ptr[ 2 ] = (::rux::uint8)( ( ( in[ 2 ] << 6 ) & 0xc0 ) | in[ 3 ] );
							}
						}
					}
					else
					{
						rux_write_error( "decoded length is equal 0" , error );
					}
				}
				else
				{
					rux_write_error( "incorrect encoded length" , error );
				}
			}
			else
			{
				rux_write_error( "encoded data is empty" , error );
			}
		};
		void XBase64::Encode( char*& destination_ptr , size_t& destination_size , size_t& destination_length , ::rux::uint8* decoded_data_ptr , size_t decoded_data_ptr_size , ::rux::XString& error , ::rux::byte web , ::rux::byte padding )
		{
			error.Clear();
			if( decoded_data_ptr )
			{
				if( decoded_data_ptr_size > 0 )
				{
					size_t encoded_data_ptr_size = get_EncodedSize( decoded_data_ptr_size , padding );
					if( encoded_data_ptr_size > 0 )
					{
						::rux::uint8* begin_ptr = decoded_data_ptr;
						::rux::uint8* end_ptr = decoded_data_ptr + decoded_data_ptr_size;
						destination_length = encoded_data_ptr_size + 1;
						if( destination_ptr != NULL && destination_size < destination_length )
						{
							::rux::engine::free_mem( destination_ptr );
							destination_ptr = NULL;
							destination_size = 0;
						}
						if( destination_ptr == NULL )
						{
							destination_ptr = alloc_array_macros( char , (size_t)destination_length );
							destination_size = destination_length;
						}
						::rux::uint8* out_ptr = (::rux::uint8*)destination_ptr;
						const ::rux::byte* encode_6_to_8_ptr = encode_6_to_8;
						if( web )
							encode_6_to_8_ptr = web_encode_6_to_8;
						while( end_ptr - begin_ptr >= 3 )
						{
							out_ptr[0] = encode_6_to_8_ptr[ begin_ptr[0] >> 2];
							out_ptr[1] = encode_6_to_8_ptr[((begin_ptr[0] & 0x03) << 4) | ((begin_ptr[1] & 0xf0) >> 4)];
							out_ptr[2] = encode_6_to_8_ptr[((begin_ptr[1] & 0x0f) << 2) | ((begin_ptr[2] & 0xc0) >> 6)];
							out_ptr[3] = encode_6_to_8_ptr[begin_ptr[2] & 0x3f];
							begin_ptr += 3;
							out_ptr += 4;
						}
						if( end_ptr != begin_ptr )
						{
							size_t length = end_ptr - begin_ptr;		
							out_ptr[ 0 ] = encode_6_to_8_ptr[ begin_ptr[ 0 ] >> 2 ];
							if( length == 1 )
							{
								out_ptr[ 1 ] = encode_6_to_8_ptr[ ( begin_ptr[ 0 ] & 0x03 ) << 4 ];
								if( padding )
								{
									out_ptr[ 2 ] = '=';
									out_ptr[ 3 ] = '=';
								}
							}
							else 
							{
								out_ptr[ 1 ] = encode_6_to_8_ptr[ ( ( begin_ptr[ 0 ] & 0x03 ) << 4 ) | ( ( begin_ptr[ 1 ] & 0xf0 ) >> 4 ) ];
								out_ptr[ 2 ] = encode_6_to_8_ptr[ ( begin_ptr[ 1 ] & 0x0f ) << 2 ];
								if( padding )
									out_ptr[ 3 ] = '=';
							}
						}
						destination_ptr[ destination_length - 1 ] = 0;
					}
					else
					{
						rux_write_error( "encoded data pointer size is equal 0" , error );
					}
				}
				else
				{
					rux_write_error( "decoded data pointer size is equal 0" , error );
				}
			}
			else
			{
				rux_write_error( "decoded data pointer is NULL" , error );
			}	
		};
		::rux::String& XBase64::Encode( ::rux::uint8* decoded_data_ptr , size_t decoded_data_ptr_size , ::rux::XString& error , ::rux::byte web , ::rux::byte padding )
		{
			error.Clear();
			if( decoded_data_ptr )
			{
				if( decoded_data_ptr_size > 0 )
				{
					size_t encoded_data_ptr_size = get_EncodedSize( decoded_data_ptr_size , padding );
					if( encoded_data_ptr_size > 0 )
					{
						::rux::uint8* begin_ptr = decoded_data_ptr;
						::rux::uint8* end_ptr = decoded_data_ptr + decoded_data_ptr_size;
						::rux::XString encoded_data( static_cast< ::rux::uint32 >( encoded_data_ptr_size ) + 1 );
						::rux::uint8* out_ptr = (::rux::uint8*)encoded_data.str();
						const ::rux::byte* encode_6_to_8_ptr = encode_6_to_8;
						if( web )
							encode_6_to_8_ptr = web_encode_6_to_8;
						while( end_ptr - begin_ptr >=3 )
						{
							out_ptr[0] = encode_6_to_8_ptr[ begin_ptr[0] >> 2];
							out_ptr[1] = encode_6_to_8_ptr[((begin_ptr[0] & 0x03) << 4) | ((begin_ptr[1] & 0xf0) >> 4)];
							out_ptr[2] = encode_6_to_8_ptr[((begin_ptr[1] & 0x0f) << 2) | ((begin_ptr[2] & 0xc0) >> 6)];
							out_ptr[3] = encode_6_to_8_ptr[begin_ptr[2] & 0x3f];
							begin_ptr += 3;
							out_ptr += 4;
						}
						if( end_ptr != begin_ptr )
						{
							size_t length = end_ptr - begin_ptr;		
							out_ptr[ 0 ] = encode_6_to_8_ptr[ begin_ptr[ 0 ] >> 2 ];
							if( length == 1 )
							{
								out_ptr[ 1 ] = encode_6_to_8_ptr[ ( begin_ptr[ 0 ] & 0x03 ) << 4 ];
								if( padding )
								{
									out_ptr[ 2 ] = '=';
									out_ptr[ 3 ] = '=';
								}
							}
							else 
							{
								out_ptr[ 1 ] = encode_6_to_8_ptr[ ( ( begin_ptr[ 0 ] & 0x03 ) << 4 ) | ( ( begin_ptr[ 1 ] & 0xf0 ) >> 4 ) ];
								out_ptr[ 2 ] = encode_6_to_8_ptr[ ( begin_ptr[ 1 ] & 0x0f ) << 2 ];
								out_ptr[ 3 ] = '=';
							}
						}
						encoded_data()->_count = static_cast< ::rux::uint32 >( encoded_data_ptr_size ) + 1;
						encoded_data.str()[ encoded_data()->_count - 1 ] = 0;
						encoded_data()->_code_page = XEnumCodePage_UTF8;
						return encoded_data++;
					}
					else
					{
						rux_write_error( "encoded data pointer size is equal 0" , error );
					}
				}
				else
				{
					rux_write_error( "decoded data pointer size is equal 0" , error );
				}
			}
			else
			{
				rux_write_error( "decoded data pointer is NULL" , error );
			}
			::rux::XString empty;
			return empty++;
		};
		size_t XBase64::get_EncodedSize( size_t decoded_data_ptr_size , rux::byte padding )
		{
			if( padding )
			{
				if( decoded_data_ptr_size % 3 )
					return ( ( decoded_data_ptr_size / 3 ) + 1 ) * 4;
				else
					return decoded_data_ptr_size / 3 * 4;
			}
			else
			{
				switch( decoded_data_ptr_size % 3 )
				{
				case 1:
					{
						return decoded_data_ptr_size / 3 * 4 + 2;
					}
				case 2:
					{
						return decoded_data_ptr_size / 3 * 4 + 3;
					}
				default:
					{
						return decoded_data_ptr_size / 3 * 4;
					}
				}
			}
		};
		size_t XBase64::get_DecodedSize( const char* encoded_data , size_t size , size_t& actual_size )
		{
			if( size == 0 )
				size = encoded_data ? strlen( encoded_data ) : 0;
			actual_size = size;
			if( actual_size > 0 && encoded_data[ actual_size - 1 ] == '=' )
				actual_size--;
			if( actual_size > 0 && encoded_data[ actual_size - 1 ] == '=' )
				actual_size--;
			switch( actual_size % 4 )
			{
			case 1: 
				{
					return SIZE_MAX;
				}
			case 2: 
				{
					return actual_size / 4 * 3 + 1;
				}
			case 3:
				{
					return actual_size / 4 * 3 + 2;
				}
			default:
				{
					return actual_size / 4 * 3;
				}
			}
		};
	};
};