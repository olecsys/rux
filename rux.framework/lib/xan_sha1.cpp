#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_sha1.h>
#include <xan_keyvaluepair.h>
#include <xan_io_defines.h>
namespace rux
{	
	namespace cryptography
	{
		void XSha1::HashStep( const rux::uint8* message_part , rux::uint64 message_offset , rux::uint64 message_size , rux::uint64 message_padding , rux::uint32 digest[ 5 ] )
		{
			rux::uint32 a = digest[ 0 ] , b = digest[ 1 ] , c = digest[ 2 ] , 
				d = digest[ 3 ] , e = digest[ 4 ];
			rux::uint32 w[ 80 ] = {0} , temp_uint32 = 0 , * w_ptr = NULL;
			rux::uint64 index1 = 0 , k = 0 , index2 = 0;
			for( index1 = 0 ; index1 < 16 ; index1++ )
			{		
				if( message_offset + k < message_size )
				{
					index2 = message_size - k - message_offset;
					if( index2 >= sizeof( rux::uint32 ) )
					{
						temp_uint32 = *message_part++ << 24;
						temp_uint32 |= *message_part++ << 16;
						temp_uint32 |= *message_part++ << 8;
						temp_uint32 |= *message_part++;
					}
					else if( index2 == 3 )
					{
						temp_uint32 = *message_part++ << 24;
						temp_uint32 |= *message_part++ << 16;
						temp_uint32 |= *message_part++ << 8;
						temp_uint32 |= 0x80;
					}
					else if( index2 == 2 )
					{
						temp_uint32 = *message_part++ << 24;
						temp_uint32 |= *message_part++ << 16;
						temp_uint32 |= 0x80 << 8;
						temp_uint32 |= 0x00;
					}
					else if( index2 == 1 )
					{
						temp_uint32 = *message_part++ << 24;
						temp_uint32 |= 0x80 << 16;
						temp_uint32 |= 0x00 << 8;
						temp_uint32 |= 0x00;
					}
				}
				else if( message_offset + k == message_size )
				{
					temp_uint32 = static_cast<rux::uint32>( 0x80 << 24 );
					temp_uint32 |= 0x00 << 16;
					temp_uint32 |= 0x00 << 8;
					temp_uint32 |= 0x00;
				}
				else if( message_offset + k < message_size + message_padding )
					temp_uint32 = 0x00000000;
				else 
				{
					rux::uint64 message_bits_size = message_size * 8;
					rux::uint8* message_bits_ptr = (rux::uint8*)&message_bits_size;
					message_bits_ptr = &message_bits_ptr[ 7 - ( k - 56 ) ];
					temp_uint32 = *message_bits_ptr-- << 24;
					temp_uint32 |= *message_bits_ptr-- << 16;
					temp_uint32 |= *message_bits_ptr-- << 8;
					temp_uint32 |= *message_bits_ptr--;
				}
				w[ index1 ] = temp_uint32;
				k += sizeof( rux::uint32 );
			}
			w_ptr = &w[ 16 ];
			for( index1 = 16 ; index1 < 80 ; index1++ )
			{			
				*w_ptr = ( w[ index1 - 3 ] ^ w[ index1 - 8 ] ^ w[ index1 - 14 ] ^ w[ index1 - 16 ] );
				*w_ptr++ = ( ( *w_ptr ) << 1 ) | ( ( *w_ptr ) >> 31 );//циклический сдвиг
			}
			w_ptr = w;
			for( index1 = 0 ; index1 < 20 ; index1++ )
			{			
				k = (rux::uint32)( ( ( a << 5 ) | ( a >> 27 ) ) + ( ( b & c ) | ( (~ b) & d ) ) + e + 0x5A827999 + *w_ptr++ ); //a = ( a << 5 ) | ( a >> 27 );
				e = d;
				d = c;
				c = ( b << 30 ) | ( b >> 2 );
				b = a;
				a = static_cast<rux::uint32>( k );
			}
			for( ; index1 < 40 ; index1++ )
			{			
				k = (rux::uint32)( ( a << 5 ) | ( a >> 27 ) ) + ( b ^ c ^ d ) + e + 0x6ED9EBA1 + *w_ptr++;
				e = d;
				d = c;
				c = ( b << 30 ) | ( b >> 2 );
				b = a;
				a = static_cast<rux::uint32>( k );
			}
			for( ; index1 < 60 ; index1++ )
			{			
				k = (rux::uint32)( ( a << 5 ) | ( a >> 27 ) ) + ( (b & c) | (b & d) | (c & d) ) + e + 0x8F1BBCDC + *w_ptr++;
				e = d;
				d = c;
				c = ( b << 30 ) | ( b >> 2 );
				b = a;
				a = static_cast<rux::uint32>( k );
			}
			for( ; index1 < 80 ; index1++ )
			{			
				k = (rux::uint32)( ( a << 5 ) | ( a >> 27 ) ) + ( b ^ c ^ d ) + e + 0xCA62C1D6 + *w_ptr++;
				e = d;
				d = c;
				c = ( b << 30 ) | ( b >> 2 );
				b = a;
				a = static_cast<rux::uint32>( k );
			}			
			digest[ 0 ] += a;
			digest[ 1 ] += b; 
			digest[ 2 ] += c;
			digest[ 3 ] += d;
			digest[ 4 ] += e;
		};
		void XSha1::Hash( const rux::uint8* message_ptr , rux::uint64 message_size , rux::uint8 hash[ 20 ] )
		{		
			rux::uint64 message_padding = 0;	
			if( message_size < 56 )		//448 bits
				message_padding = 56 - message_size;
			else if( message_size < 64 )		
				message_padding = 120 - message_size;//960 bits		
			else
			{
				rux::uint32 remainder = message_size % 64;//512 bits			
				if( remainder > 55 )//440 bits			
					message_padding = 120 - remainder;
				else			
					message_padding = 56 - remainder;
			}
			rux::uint32 digest[ 5 ] = {0};
			digest[ 0 ] = 0x67452301;
			digest[ 1 ] = 0xEFCDAB89;
			digest[ 2 ] = 0x98BADCFE;
			digest[ 3 ] = 0x10325476;
			digest[ 4 ] = 0xC3D2E1F0;
			for( rux::uint64 index0 = 0 ; index0 < message_size + message_padding + 8 ; index0 += 64 )
				HashStep( &message_ptr[ index0 ] , index0 , message_size , message_padding , digest );
			for( rux::int32 index0 = 0; index0 < 20; ++index0 )		
				hash[ index0 ] = static_cast<rux::uint8>( digest[ index0 >> 2 ] >> 8 * ( 3 - ( index0 & 0x03 ) ) );
		};
		void XSha1::Hash( rux::io::file& file , rux::uint8 hash[ 20 ] )
		{
			if( file.opened() )
			{
				rux::uint8* message = (rux::uint8*)::rux::engine::rux_malloc_dummy( 1024 * 1024 );
				rux::uint64 message_size = file.size();
				if( message_size <= 1024 * 1024 )
				{
					file.seek( 0 );
					size_t readen = 0;
					if( file.read_bytes( (char*)message , 1024 * 1024 , readen ) )
						Hash( message , readen , hash );
					else
						memset( hash , 0 , 20 );
				}
				else
				{
					file.seek( 0 );
					size_t readen = 0;
					rux::uint64 message_padding = 0;	
					if( message_size < 56 )		//448 bits
						message_padding = 56 - message_size;
					else if( message_size < 64 )		
						message_padding = 120 - message_size;//960 bits		
					else
					{
						rux::uint32 remainder = message_size % 64;//512 bits			
						if( remainder > 55 )//440 bits			
							message_padding = 120 - remainder;
						else			
							message_padding = 56 - remainder;
					}
					rux::uint32 digest[ 5 ] = {0};
					digest[ 0 ] = 0x67452301;
					digest[ 1 ] = 0xEFCDAB89;
					digest[ 2 ] = 0x98BADCFE;
					digest[ 3 ] = 0x10325476;
					digest[ 4 ] = 0xC3D2E1F0;
					rux::uint8* ptr = NULL;
					for( rux::uint64 index0 = 0 ; index0 < message_size + message_padding + 8 ; index0 += 64 )
					{		
						if( readen == 0 )
						{
							if( file.eof() == false )
							{
								if( file.read_bytes( (char*)message , 1024 * 1024 , readen ) == false )
								{
									memset( hash , 0 , 20 );
									break;
								}
							}
							ptr = message;
						}
						HashStep( ptr , index0 , message_size , message_padding , digest );
						ptr += 64;
						if( readen < 64 )
							readen = 0;
						else
							readen -= 64;
					}
					for( rux::int32 index0 = 0; index0 < 20; ++index0 )		
						hash[ index0 ] = static_cast<rux::uint8>( digest[ index0 >> 2 ] >> 8 * ( 3 - ( index0 & 0x03 ) ) );
				}
				::rux::engine::rux_free_dummy( message );
			}
			else
				memset( hash , 0 , 20 );
		};
		void XSha1::Hash( const char* filename , rux::uint8 hash[ 20 ] )
		{
			rux::io::file file( filename );
			Hash( file , hash );
		};
	};
};