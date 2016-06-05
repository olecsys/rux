#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_md5.h>
#include <xan_keyvaluepair.h>
namespace rux
{
	namespace cryptography
	{
		void XMD5::Hash( const rux::uint8* message_ptr , rux::uint64 message_ptr_size , rux::uint8 hash[ 16 ] )
		{
			rux::uint32 padding_length = 0;
			rux::uint8 message_length_ptr[ 8 ] = {0};
			if( message_ptr_size < 56 )		//448 bits
				padding_length = 56 - static_cast<rux::uint32>( message_ptr_size );
			else if( message_ptr_size < 64 )		
				padding_length = 120 - static_cast<rux::uint32>( message_ptr_size );//960 bits		
			else
			{
				rux::uint32 remainder = message_ptr_size % 64;//512 bits			
				if( remainder > 55 )//440 bits			
					padding_length = 120 - remainder;
				else			
					padding_length = 56 - remainder;
			}
			message_ptr_size *= 8;
			message_length_ptr[ 0 ] = (rux::uint8)( message_ptr_size & 0xff );
			message_length_ptr[ 1 ] = (rux::uint8)( ( message_ptr_size >> 8 ) & 0xff );
			message_length_ptr[ 2 ] = (rux::uint8)( ( message_ptr_size >> 16 ) & 0xff );
			message_length_ptr[ 3 ] = (rux::uint8)( ( message_ptr_size >> 24 ) & 0xff );
			message_length_ptr[ 4 ] = (rux::uint8)( ( message_ptr_size >> 32 ) & 0xff );
			message_length_ptr[ 5 ] = (rux::uint8)( ( message_ptr_size >> 40 ) & 0xff );
			message_length_ptr[ 6 ] = (rux::uint8)( ( message_ptr_size >> 48 ) & 0xff );
			message_length_ptr[ 7 ] = (rux::uint8)( ( message_ptr_size >> 56 ) & 0xff );
			message_ptr_size /= 8;
	
			rux::uint32 a = 0x67452301;
			rux::uint32 b = 0xefcdab89;
			rux::uint32 c = 0x98badcfe;
			rux::uint32 d = 0x10325476;
			rux::uint32 aa = 0x67452301;
			rux::uint32 bb = 0xefcdab89;
			rux::uint32 cc = 0x98badcfe;
			rux::uint32 dd = 0x10325476;
			rux::uint32 index1 = 0;
			rux::uint32 x = 0;
			for( rux::uint32 index0 = 0 ; index0 < message_ptr_size + padding_length + 8 ; index0 += 64 )
			{
				/* Round 1 */
				a = aa;
				b = bb;
				c = cc;
				d = dd;
				x = index0;
				if( x < message_ptr_size )
				{
					index1 = static_cast<rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( rux::uint32 ) )
						x = *(rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) & 0xff00 ) | ( ( ( (rux::uint32)message_ptr[ x + 2 ]) << 16 ) & 0xff0000 ) | 0x80000000;
					else if( index1 == 2 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) &0xff00 ) | 0x00800000;
					else if( index1 == 1 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | 0x00008000;
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				a += ( ( b & c ) | ( ( ~b ) & d ) ) + x + 0xd76aa478;
				a = ( a << 7 ) | ( a >> ( 25 ) );
				a += b;		
				x = index0 + 4;
				if( x < message_ptr_size )
				{
					index1 = static_cast<rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( rux::uint32 ) )
						x = *(rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) & 0xff00 ) | ( ( ( (rux::uint32)message_ptr[ x + 2 ]) << 16 ) & 0xff0000 ) | 0x80000000;
					else if( index1 == 2 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) &0xff00 ) | 0x00800000;
					else if( index1 == 1 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | 0x00008000;
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(rux::uint32*)&message_length_ptr[ x - index0 - 56 ];				  
				d += ( ( a & b ) | ( ( ~a ) & c ) ) + x + 0xe8c7b756;
				d = ( d << 12 ) | ( d >> 20 );
				d += a;
				x = index0 + 8;
				if( x < message_ptr_size )
				{
					index1 = static_cast<rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( rux::uint32 ) )
						x = *(rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) & 0xff00 ) | ( ( ( (rux::uint32)message_ptr[ x + 2 ]) << 16 ) & 0xff0000 ) | 0x80000000;
					else if( index1 == 2 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) &0xff00 ) | 0x00800000;
					else if( index1 == 1 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | 0x00008000;
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(rux::uint32*)&message_length_ptr[ x - index0 - 56 ];				  
				c += ( ( d & a ) | ( ( ~d ) & b ) ) + x + 0x242070db;
				c = ( c << 17 ) | ( c >> 15 );
				c += d;
				x = index0 + 12;
				if( x < message_ptr_size )
				{
					index1 = static_cast<rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( rux::uint32 ) )
						x = *(rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) & 0xff00 ) | ( ( ( (rux::uint32)message_ptr[ x + 2 ]) << 16 ) & 0xff0000 ) | 0x80000000;
					else if( index1 == 2 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) &0xff00 ) | 0x00800000;
					else if( index1 == 1 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | 0x00008000;
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(rux::uint32*)&message_length_ptr[ x - index0 - 56 ];				
				b += ( ( c & d ) | ( ( ~c ) & a ) ) + x + 0xc1bdceee;
				b = ( b << 22 ) | ( b >> 10 );
				b += c;
				x = index0 + 16;
				if( x < message_ptr_size )
				{
					index1 = static_cast<rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( rux::uint32 ) )
						x = *(rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) & 0xff00 ) | ( ( ( (rux::uint32)message_ptr[ x + 2 ]) << 16 ) & 0xff0000 ) | 0x80000000;
					else if( index1 == 2 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) &0xff00 ) | 0x00800000;
					else if( index1 == 1 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | 0x00008000;
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(rux::uint32*)&message_length_ptr[ x - index0 - 56 ];				
				a += ( ( b & c ) | ( ( ~b ) & d ) ) + x + 0xf57c0faf;
				a = ( a << 7 ) | ( a >> ( 25 ) );
				a += b;
				x = index0 + 20;
				if( x < message_ptr_size )
				{
					index1 = static_cast<rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( rux::uint32 ) )
						x = *(rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) & 0xff00 ) | ( ( ( (rux::uint32)message_ptr[ x + 2 ]) << 16 ) & 0xff0000 ) | 0x80000000;
					else if( index1 == 2 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) &0xff00 ) | 0x00800000;
					else if( index1 == 1 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | 0x00008000;
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				d += ( ( a & b ) | ( ( ~a ) & c ) ) + x + 0x4787c62a;
				d = ( d << 12 ) | ( d >> 20 );
				d += a;
				x = index0 + 24;
				if( x < message_ptr_size )
				{
					index1 = static_cast<rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( rux::uint32 ) )
						x = *(rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) & 0xff00 ) | ( ( ( (rux::uint32)message_ptr[ x + 2 ]) << 16 ) & 0xff0000 ) | 0x80000000;
					else if( index1 == 2 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) &0xff00 ) | 0x00800000;
					else if( index1 == 1 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | 0x00008000;
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				c += ( ( d & a ) | ( ( ~d ) & b ) ) + x + 0xa8304613;
				c = ( c << 17 ) | ( c >> 15 );
				c += d;
				x = index0 + 28;
				if( x < message_ptr_size )
				{
					index1 = static_cast<rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( rux::uint32 ) )
						x = *(rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) & 0xff00 ) | ( ( ( (rux::uint32)message_ptr[ x + 2 ]) << 16 ) & 0xff0000 ) | 0x80000000;
					else if( index1 == 2 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) &0xff00 ) | 0x00800000;
					else if( index1 == 1 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | 0x00008000;
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				b += ( ( c & d ) | ( ( ~c ) & a ) ) + x + 0xfd469501;
				b = ( b << 22 ) | ( b >> 10 );
				b += c;
				x = index0 + 32;
				if( x < message_ptr_size )
				{
					index1 = static_cast<rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( rux::uint32 ) )
						x = *(rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) & 0xff00 ) | ( ( ( (rux::uint32)message_ptr[ x + 2 ]) << 16 ) & 0xff0000 ) | 0x80000000;
					else if( index1 == 2 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) &0xff00 ) | 0x00800000;
					else if( index1 == 1 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | 0x00008000;
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				a += ( ( b & c ) | ( ( ~b ) & d ) ) + x + 0x698098d8;
				a = ( a << 7 ) | ( a >> ( 25 ) );
				a += b;
				x = index0 + 36;
				if( x < message_ptr_size )
				{
					index1 = static_cast<rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( rux::uint32 ) )
						x = *(rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) & 0xff00 ) | ( ( ( (rux::uint32)message_ptr[ x + 2 ]) << 16 ) & 0xff0000 ) | 0x80000000;
					else if( index1 == 2 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) &0xff00 ) | 0x00800000;
					else if( index1 == 1 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | 0x00008000;
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				d += ( ( a & b ) | ( ( ~a ) & c ) ) + x + 0x8b44f7af;
				d = ( d << 12 ) | ( d >> 20 );
				d += a;
				x = index0 + 40;
				if( x < message_ptr_size )
				{
					index1 = static_cast<rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( rux::uint32 ) )
						x = *(rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) & 0xff00 ) | ( ( ( (rux::uint32)message_ptr[ x + 2 ]) << 16 ) & 0xff0000 ) | 0x80000000;
					else if( index1 == 2 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) &0xff00 ) | 0x00800000;
					else if( index1 == 1 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | 0x00008000;
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				c += ( ( d & a ) | ( ( ~d ) & b ) ) + x + 0xffff5bb1;
				c = ( c << 17 ) | ( c >> 15 );
				c += d;
				x = index0 + 44;
				if( x < message_ptr_size )
				{
					index1 = static_cast<rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( rux::uint32 ) )
						x = *(rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) & 0xff00 ) | ( ( ( (rux::uint32)message_ptr[ x + 2 ]) << 16 ) & 0xff0000 ) | 0x80000000;
					else if( index1 == 2 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) &0xff00 ) | 0x00800000;
					else if( index1 == 1 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | 0x00008000;
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				b += ( ( c & d ) | ( ( ~c ) & a ) ) + x + 0x895cd7be;
				b = ( b << 22 ) | ( b >> 10 );
				b += c;
				x = index0 + 48;
				if( x < message_ptr_size )
				{
					index1 = static_cast<rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( rux::uint32 ) )
						x = *(rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) & 0xff00 ) | ( ( ( (rux::uint32)message_ptr[ x + 2 ]) << 16 ) & 0xff0000 ) | 0x80000000;
					else if( index1 == 2 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) &0xff00 ) | 0x00800000;
					else if( index1 == 1 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | 0x00008000;
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				a += ( ( b & c ) | ( ( ~b ) & d ) ) + x + 0x6b901122;
				a = ( a << 7 ) | ( a >> ( 25 ) );
				a += b;
				x = index0 + 52;
				if( x < message_ptr_size )
				{
					index1 = static_cast<rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( rux::uint32 ) )
						x = *(rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) & 0xff00 ) | ( ( ( (rux::uint32)message_ptr[ x + 2 ]) << 16 ) & 0xff0000 ) | 0x80000000;
					else if( index1 == 2 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) &0xff00 ) | 0x00800000;
					else if( index1 == 1 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | 0x00008000;
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				d += ( ( a & b ) | ( ( ~a ) & c ) ) + x + 0xfd987193;
				d = ( d << 12 ) | ( d >> 20 );
				d += a;
				x = index0 + 56;
				if( x < message_ptr_size )
				{
					index1 = static_cast<rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( rux::uint32 ) )
						x = *(rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) & 0xff00 ) | ( ( ( (rux::uint32)message_ptr[ x + 2 ]) << 16 ) & 0xff0000 ) | 0x80000000;
					else if( index1 == 2 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) &0xff00 ) | 0x00800000;
					else if( index1 == 1 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | 0x00008000;
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				c += ( ( d & a ) | ( ( ~d ) & b ) ) + x + 0xa679438e;
				c = ( c << 17 ) | ( c >> 15 );
				c += d;
				x = index0 + 60;
				if( x < message_ptr_size )
				{
					index1 = static_cast<rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( rux::uint32 ) )
						x = *(rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) & 0xff00 ) | ( ( ( (rux::uint32)message_ptr[ x + 2 ]) << 16 ) & 0xff0000 ) | 0x80000000;
					else if( index1 == 2 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) &0xff00 ) | 0x00800000;
					else if( index1 == 1 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | 0x00008000;
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				b += ( ( c & d ) | ( ( ~c ) & a ) ) + x + 0x49b40821;
				b = ( b << 22 ) | ( b >> 10 );
				b += c;
				/* Round 2 */    
				x = index0 + 4;
				if( x < message_ptr_size )
				{
					index1 = static_cast<rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( rux::uint32 ) )
						x = *(rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) & 0xff00 ) | ( ( ( (rux::uint32)message_ptr[ x + 2 ]) << 16 ) & 0xff0000 ) | 0x80000000;
					else if( index1 == 2 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) &0xff00 ) | 0x00800000;
					else if( index1 == 1 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | 0x00008000;
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				a += ( ( b & d ) | ( c & ( ~d ) ) ) + x + 0xf61e2562;
				a = ( a << 5 ) | ( a >> ( 27 ) );
				a += b;
				x = index0 + 24;
				if( x < message_ptr_size )
				{
					index1 = static_cast<rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( rux::uint32 ) )
						x = *(rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) & 0xff00 ) | ( ( ( (rux::uint32)message_ptr[ x + 2 ]) << 16 ) & 0xff0000 ) | 0x80000000;
					else if( index1 == 2 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) &0xff00 ) | 0x00800000;
					else if( index1 == 1 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | 0x00008000;
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				d += ( ( a & c ) | ( b & ( ~c ) ) ) + x + 0xc040b340;
				d = ( d << 9) | ( d >> 23 );
				d += a;
				x = index0 + 44;
				if( x < message_ptr_size )
				{
					index1 = static_cast<rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( rux::uint32 ) )
						x = *(rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) & 0xff00 ) | ( ( ( (rux::uint32)message_ptr[ x + 2 ]) << 16 ) & 0xff0000 ) | 0x80000000;
					else if( index1 == 2 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) &0xff00 ) | 0x00800000;
					else if( index1 == 1 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | 0x00008000;
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				c += ( ( d & b ) | ( a & ( ~b ) ) ) + x + 0x265e5a51;
				c = ( c << 14 ) | ( c >> 18 );
				c += d;
				x = index0;
				if( x < message_ptr_size )
				{
					index1 = static_cast<rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( rux::uint32 ) )
						x = *(rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) & 0xff00 ) | ( ( ( (rux::uint32)message_ptr[ x + 2 ]) << 16 ) & 0xff0000 ) | 0x80000000;
					else if( index1 == 2 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) &0xff00 ) | 0x00800000;
					else if( index1 == 1 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | 0x00008000;
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				b += ( ( c & a ) | ( d & ( ~a ) ) ) + x + 0xe9b6c7aa;
				b = ( b << 20 ) | ( b >> 12 );
				b += c;
				x = index0 + 20;
				if( x < message_ptr_size )
				{
					index1 = static_cast<rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( rux::uint32 ) )
						x = *(rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) & 0xff00 ) | ( ( ( (rux::uint32)message_ptr[ x + 2 ]) << 16 ) & 0xff0000 ) | 0x80000000;
					else if( index1 == 2 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) &0xff00 ) | 0x00800000;
					else if( index1 == 1 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | 0x00008000;
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				a += ( ( b & d ) | ( c & ( ~d ) ) ) + x + 0xd62f105d;
				a = ( a << 5 ) | ( a >> ( 27 ) );
				a += b;
				x = index0 + 40;
				if( x < message_ptr_size )
				{
					index1 = static_cast<rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( rux::uint32 ) )
						x = *(rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) & 0xff00 ) | ( ( ( (rux::uint32)message_ptr[ x + 2 ]) << 16 ) & 0xff0000 ) | 0x80000000;
					else if( index1 == 2 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) &0xff00 ) | 0x00800000;
					else if( index1 == 1 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | 0x00008000;
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				d += ( ( a & c ) | ( b & ( ~c ) ) ) + x + 0x2441453;
				d = ( d << 9 ) | ( d >> 23 );
				d += a;
				x = index0 + 60;
				if( x < message_ptr_size )
				{
					index1 = static_cast<rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( rux::uint32 ) )
						x = *(rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) & 0xff00 ) | ( ( ( (rux::uint32)message_ptr[ x + 2 ]) << 16 ) & 0xff0000 ) | 0x80000000;
					else if( index1 == 2 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) &0xff00 ) | 0x00800000;
					else if( index1 == 1 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | 0x00008000;
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				c += ( ( d & b ) | ( a & ( ~b ) ) ) + x + 0xd8a1e681;
				c = ( c << 14 ) | ( c >> 18 );
				c += d;
				x = index0 + 16;
				if( x < message_ptr_size )
				{
					index1 = static_cast<rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( rux::uint32 ) )
						x = *(rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) & 0xff00 ) | ( ( ( (rux::uint32)message_ptr[ x + 2 ]) << 16 ) & 0xff0000 ) | 0x80000000;
					else if( index1 == 2 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) &0xff00 ) | 0x00800000;
					else if( index1 == 1 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | 0x00008000;
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				b += ( ( c & a ) | ( d & ( ~a ) ) ) + x + 0xe7d3fbc8;
				b = ( b << 20 ) | ( b >> 12 );
				b += c;
				x = index0 + 36;
				if( x < message_ptr_size )
				{
					index1 = static_cast<rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( rux::uint32 ) )
						x = *(rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) & 0xff00 ) | ( ( ( (rux::uint32)message_ptr[ x + 2 ]) << 16 ) & 0xff0000 ) | 0x80000000;
					else if( index1 == 2 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) &0xff00 ) | 0x00800000;
					else if( index1 == 1 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | 0x00008000;
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				a += ( ( b & d ) | ( c & ( ~d ) ) ) + x + 0x21e1cde6;
				a = ( a << 5 ) | ( a >> ( 27 ) );
				a += b;
				x = index0 + 56;
				if( x < message_ptr_size )
				{
					index1 = static_cast<rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( rux::uint32 ) )
						x = *(rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) & 0xff00 ) | ( ( ( (rux::uint32)message_ptr[ x + 2 ]) << 16 ) & 0xff0000 ) | 0x80000000;
					else if( index1 == 2 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) &0xff00 ) | 0x00800000;
					else if( index1 == 1 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | 0x00008000;
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				d += ( ( a & c ) | ( b & ( ~c ) ) ) + x + 0xc33707d6;
				d = ( d << 9 ) | ( d >> 23 );
				d += a;
				x = index0 + 12;
				if( x < message_ptr_size )
				{
					index1 = static_cast<rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( rux::uint32 ) )
						x = *(rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) & 0xff00 ) | ( ( ( (rux::uint32)message_ptr[ x + 2 ]) << 16 ) & 0xff0000 ) | 0x80000000;
					else if( index1 == 2 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) &0xff00 ) | 0x00800000;
					else if( index1 == 1 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | 0x00008000;
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				c += ( ( d & b ) | ( a & ( ~b ) ) ) + x + 0xf4d50d87;
				c = ( c << 14 ) | ( c >> 18 );
				c += d;
				x = index0 + 32;
				if( x < message_ptr_size )
				{
					index1 = static_cast<rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( rux::uint32 ) )
						x = *(rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) & 0xff00 ) | ( ( ( (rux::uint32)message_ptr[ x + 2 ]) << 16 ) & 0xff0000 ) | 0x80000000;
					else if( index1 == 2 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) &0xff00 ) | 0x00800000;
					else if( index1 == 1 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | 0x00008000;
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				b += ( ( c & a ) | ( d & ( ~a ) ) ) + x + 0x455a14ed;
				b = ( b << 20 ) | ( b >> 12 );
				b += c;
				x = index0 + 52;
				if( x < message_ptr_size )
				{
					index1 = static_cast<rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( rux::uint32 ) )
						x = *(rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) & 0xff00 ) | ( ( ( (rux::uint32)message_ptr[ x + 2 ]) << 16 ) & 0xff0000 ) | 0x80000000;
					else if( index1 == 2 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) &0xff00 ) | 0x00800000;
					else if( index1 == 1 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | 0x00008000;
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				a += ( ( b & d ) | ( c & ( ~d ) ) ) + x + 0xa9e3e905;
				a = ( a << 5 ) | ( a >> ( 27 ) );
				a += b;
				x = index0 + 8;
				if( x < message_ptr_size )
				{
					index1 = static_cast<rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( rux::uint32 ) )
						x = *(rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) & 0xff00 ) | ( ( ( (rux::uint32)message_ptr[ x + 2 ]) << 16 ) & 0xff0000 ) | 0x80000000;
					else if( index1 == 2 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) &0xff00 ) | 0x00800000;
					else if( index1 == 1 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | 0x00008000;
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				d += ( ( a & c ) | ( b & ( ~c ) ) ) + x + 0xfcefa3f8;
				d = ( d << 9 ) | ( d >> 23 );
				d += a;				
				x = index0 + 28;
				if( x < message_ptr_size )
				{
					index1 = static_cast<rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( rux::uint32 ) )
						x = *(rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) & 0xff00 ) | ( ( ( (rux::uint32)message_ptr[ x + 2 ]) << 16 ) & 0xff0000 ) | 0x80000000;
					else if( index1 == 2 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) &0xff00 ) | 0x00800000;
					else if( index1 == 1 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | 0x00008000;
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				c += ( ( d & b ) | ( a & ( ~b ) ) ) + x + 0x676f02d9;
				c = ( c << 14 ) | ( c >> 18 );
				c += d;				
				x = index0 + 48;
				if( x < message_ptr_size )
				{
					index1 = static_cast<rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( rux::uint32 ) )
						x = *(rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) & 0xff00 ) | ( ( ( (rux::uint32)message_ptr[ x + 2 ]) << 16 ) & 0xff0000 ) | 0x80000000;
					else if( index1 == 2 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) &0xff00 ) | 0x00800000;
					else if( index1 == 1 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | 0x00008000;
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				b += ( ( c & a ) | ( d & ( ~a ) ) ) + x + 0x8d2a4c8a;
				b = ( b << 20 ) | ( b >> 12 );
				b += c;				
				/* Round 3 */
				x = index0 + 20;
				if( x < message_ptr_size )
				{
					index1 = static_cast<rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( rux::uint32 ) )
						x = *(rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) & 0xff00 ) | ( ( ( (rux::uint32)message_ptr[ x + 2 ]) << 16 ) & 0xff0000 ) | 0x80000000;
					else if( index1 == 2 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) &0xff00 ) | 0x00800000;
					else if( index1 == 1 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | 0x00008000;
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				a += ( b ^ c ^ d ) + x + 0xfffa3942;
				a = ( a << 4 ) | ( a >> ( 28 ) );
				a += b;
				x = index0 + 32;
				if( x < message_ptr_size )
				{
					index1 = static_cast<rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( rux::uint32 ) )
						x = *(rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) & 0xff00 ) | ( ( ( (rux::uint32)message_ptr[ x + 2 ]) << 16 ) & 0xff0000 ) | 0x80000000;
					else if( index1 == 2 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) &0xff00 ) | 0x00800000;
					else if( index1 == 1 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | 0x00008000;
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				d += ( a ^ b ^ c ) + x + 0x8771f681;
				d = ( d << 11 ) | ( d >> 21 );
				d += a;
				x = index0 + 44;
				if( x < message_ptr_size )
				{
					index1 = static_cast<rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( rux::uint32 ) )
						x = *(rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) & 0xff00 ) | ( ( ( (rux::uint32)message_ptr[ x + 2 ]) << 16 ) & 0xff0000 ) | 0x80000000;
					else if( index1 == 2 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) &0xff00 ) | 0x00800000;
					else if( index1 == 1 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | 0x00008000;
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				c += ( d ^ a ^ b ) + x + 0x6d9d6122;
				c = ( c << 16 ) | ( c >> 16 );
				c += d;
				x = index0 + 56;
				if( x < message_ptr_size )
				{
					index1 = static_cast<rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( rux::uint32 ) )
						x = *(rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) & 0xff00 ) | ( ( ( (rux::uint32)message_ptr[ x + 2 ]) << 16 ) & 0xff0000 ) | 0x80000000;
					else if( index1 == 2 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) &0xff00 ) | 0x00800000;
					else if( index1 == 1 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | 0x00008000;
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				b += ( c ^ d ^ a ) + x + 0xfde5380c;
				b = ( b << 23 ) | ( b >> 9 );
				b += c;  
				x = index0 + 4;
				if( x < message_ptr_size )
				{
					index1 = static_cast<rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( rux::uint32 ) )
						x = *(rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) & 0xff00 ) | ( ( ( (rux::uint32)message_ptr[ x + 2 ]) << 16 ) & 0xff0000 ) | 0x80000000;
					else if( index1 == 2 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) &0xff00 ) | 0x00800000;
					else if( index1 == 1 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | 0x00008000;
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				a += ( b ^ c ^ d ) + x + 0xa4beea44;
				a = ( a << 4 ) | ( a >> ( 28 ) );
				a += b;  
				x = index0 + 16;
				if( x < message_ptr_size )
				{
					index1 = static_cast<rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( rux::uint32 ) )
						x = *(rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) & 0xff00 ) | ( ( ( (rux::uint32)message_ptr[ x + 2 ]) << 16 ) & 0xff0000 ) | 0x80000000;
					else if( index1 == 2 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) &0xff00 ) | 0x00800000;
					else if( index1 == 1 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | 0x00008000;
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				d += ( a ^ b ^ c ) + x + 0x4bdecfa9;
				d = ( d << 11 ) | ( d >> 21 );
				d += a;  
				x = index0 + 28;
				if( x < message_ptr_size )
				{
					index1 = static_cast<rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( rux::uint32 ) )
						x = *(rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) & 0xff00 ) | ( ( ( (rux::uint32)message_ptr[ x + 2 ]) << 16 ) & 0xff0000 ) | 0x80000000;
					else if( index1 == 2 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) &0xff00 ) | 0x00800000;
					else if( index1 == 1 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | 0x00008000;
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				c += ( d ^ a ^ b ) + x + 0xf6bb4b60;
				c = ( c << 16 ) | ( c >> 16 );
				c += d;  
				x = index0 + 40;
				if( x < message_ptr_size )
				{
					index1 = static_cast<rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( rux::uint32 ) )
						x = *(rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) & 0xff00 ) | ( ( ( (rux::uint32)message_ptr[ x + 2 ]) << 16 ) & 0xff0000 ) | 0x80000000;
					else if( index1 == 2 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) &0xff00 ) | 0x00800000;
					else if( index1 == 1 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | 0x00008000;
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				b += ( c ^ d ^ a ) + x + 0xbebfbc70;
				b = ( b << 23 ) | ( b >> 9 );
				b += c;  
				x = index0 + 52;
				if( x < message_ptr_size )
				{
					index1 = static_cast<rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( rux::uint32 ) )
						x = *(rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) & 0xff00 ) | ( ( ( (rux::uint32)message_ptr[ x + 2 ]) << 16 ) & 0xff0000 ) | 0x80000000;
					else if( index1 == 2 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) &0xff00 ) | 0x00800000;
					else if( index1 == 1 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | 0x00008000;
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				a += ( b ^ c ^ d ) + x + 0x289b7ec6;
				a = ( a << 4 ) | ( a >> ( 28 ) );
				a += b;  
				x = index0;
				if( x < message_ptr_size )
				{
					index1 = static_cast<rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( rux::uint32 ) )
						x = *(rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) & 0xff00 ) | ( ( ( (rux::uint32)message_ptr[ x + 2 ]) << 16 ) & 0xff0000 ) | 0x80000000;
					else if( index1 == 2 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) &0xff00 ) | 0x00800000;
					else if( index1 == 1 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | 0x00008000;
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				d += ( a ^ b ^ c ) + x + 0xeaa127fa;
				d = ( d << 11 ) | ( d >> 21 );
				d += a;  
				x = index0 + 12;
				if( x < message_ptr_size )
				{
					index1 = static_cast<rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( rux::uint32 ) )
						x = *(rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) & 0xff00 ) | ( ( ( (rux::uint32)message_ptr[ x + 2 ]) << 16 ) & 0xff0000 ) | 0x80000000;
					else if( index1 == 2 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) &0xff00 ) | 0x00800000;
					else if( index1 == 1 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | 0x00008000;
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				c += ( d ^ a ^ b ) + x + 0xd4ef3085;
				c = ( c << 16 ) | ( c >> 16 );
				c += d;  
				x = index0 + 24;
				if( x < message_ptr_size )
				{
					index1 = static_cast<rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( rux::uint32 ) )
						x = *(rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) & 0xff00 ) | ( ( ( (rux::uint32)message_ptr[ x + 2 ]) << 16 ) & 0xff0000 ) | 0x80000000;
					else if( index1 == 2 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) &0xff00 ) | 0x00800000;
					else if( index1 == 1 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | 0x00008000;
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				b += ( c ^ d ^ a ) + x + 0x4881d05;
				b = ( b << 23 ) | ( b >> 9 );
				b += c;  
				x = index0 + 36;
				if( x < message_ptr_size )
				{
					index1 = static_cast<rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( rux::uint32 ) )
						x = *(rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) & 0xff00 ) | ( ( ( (rux::uint32)message_ptr[ x + 2 ]) << 16 ) & 0xff0000 ) | 0x80000000;
					else if( index1 == 2 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) &0xff00 ) | 0x00800000;
					else if( index1 == 1 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | 0x00008000;
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				a += ( b ^ c ^ d ) + x + 0xd9d4d039;
				a = ( a << 4 ) | ( a >> ( 28 ) );
				a += b;    
				x = index0 + 48;
				if( x < message_ptr_size )
				{
					index1 = static_cast<rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( rux::uint32 ) )
						x = *(rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) & 0xff00 ) | ( ( ( (rux::uint32)message_ptr[ x + 2 ]) << 16 ) & 0xff0000 ) | 0x80000000;
					else if( index1 == 2 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) &0xff00 ) | 0x00800000;
					else if( index1 == 1 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | 0x00008000;
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				d += ( a ^ b ^ c ) + x + 0xe6db99e5;
				d = ( d << 11 ) | ( d >> 21 );
				d += a;    
				x = index0 + 60;
				if( x < message_ptr_size )
				{
					index1 = static_cast<rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( rux::uint32 ) )
						x = *(rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) & 0xff00 ) | ( ( ( (rux::uint32)message_ptr[ x + 2 ]) << 16 ) & 0xff0000 ) | 0x80000000;
					else if( index1 == 2 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) &0xff00 ) | 0x00800000;
					else if( index1 == 1 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | 0x00008000;
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				c += ( d ^ a ^ b ) + x + 0x1fa27cf8;
				c = ( c << 16 ) | ( c >> 16 );
				c += d;    
				x = index0 + 8;
				if( x < message_ptr_size )
				{
					index1 = static_cast<rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( rux::uint32 ) )
						x = *(rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) & 0xff00 ) | ( ( ( (rux::uint32)message_ptr[ x + 2 ]) << 16 ) & 0xff0000 ) | 0x80000000;
					else if( index1 == 2 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) &0xff00 ) | 0x00800000;
					else if( index1 == 1 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | 0x00008000;
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				b += ( c ^ d ^ a ) + x + 0xc4ac5665;
				b = ( b << 23 ) | ( b >> 9 );
				b += c;       
				/* Round 4 */			
				x = index0;
				if( x < message_ptr_size )
				{
					index1 = static_cast<rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( rux::uint32 ) )
						x = *(rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) & 0xff00 ) | ( ( ( (rux::uint32)message_ptr[ x + 2 ]) << 16 ) & 0xff0000 ) | 0x80000000;
					else if( index1 == 2 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) &0xff00 ) | 0x00800000;
					else if( index1 == 1 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | 0x00008000;
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				a += ( c ^ ( b | (~d) ) ) + x + 0xf4292244;
				a = ( a << 6 ) | ( a >> ( 26 ) );
				a += b;
				x = index0 + 28;
				if( x < message_ptr_size )
				{
					index1 = static_cast<rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( rux::uint32 ) )
						x = *(rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) & 0xff00 ) | ( ( ( (rux::uint32)message_ptr[ x + 2 ]) << 16 ) & 0xff0000 ) | 0x80000000;
					else if( index1 == 2 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) &0xff00 ) | 0x00800000;
					else if( index1 == 1 )
						x = ( (rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				d += ( b ^ ( a | (~c) ) ) + x + 0x432aff97;
				d = ( d << 10 ) | ( d >> 22 );
				d += a;
				x = index0 + 56;
				if( x < message_ptr_size )
				{
					index1 = static_cast<rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( rux::uint32 ) )
						x = *(rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) & 0xff00 ) | ( ( ( (rux::uint32)message_ptr[ x + 2 ]) << 16 ) & 0xff0000 ) | 0x80000000;
					else if( index1 == 2 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) &0xff00 ) | 0x00800000;
					else if( index1 == 1 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | 0x00008000;
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				c += ( a ^ ( d | (~b) ) ) + x + 0xab9423a7;
				c = ( c << 15 ) | ( c >> 17 );
				c += d;
				x = index0 + 20;
				if( x < message_ptr_size )
				{
					index1 = static_cast<rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( rux::uint32 ) )
						x = *(rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) & 0xff00 ) | ( ( ( (rux::uint32)message_ptr[ x + 2 ]) << 16 ) & 0xff0000 ) | 0x80000000;
					else if( index1 == 2 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) &0xff00 ) | 0x00800000;
					else if( index1 == 1 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | 0x00008000;
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				b += ( d ^ ( c | (~a) ) ) + x + 0xfc93a039;
				b = ( b << 21 ) | ( b >> 11 );
				b += c;
				x = index0 + 48;
				if( x < message_ptr_size )
				{
					index1 = static_cast<rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( rux::uint32 ) )
						x = *(rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) & 0xff00 ) | ( ( ( (rux::uint32)message_ptr[ x + 2 ]) << 16 ) & 0xff0000 ) | 0x80000000;
					else if( index1 == 2 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) &0xff00 ) | 0x00800000;
					else if( index1 == 1 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | 0x00008000;
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				a += ( c ^ ( b | (~d) ) ) + x + 0x655b59c3;
				a = ( a << 6 ) | ( a >> ( 26 ) );
				a += b;
				x = index0 + 12;
				if( x < message_ptr_size )
				{
					index1 = static_cast<rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( rux::uint32 ) )
						x = *(rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) & 0xff00 ) | ( ( ( (rux::uint32)message_ptr[ x + 2 ]) << 16 ) & 0xff0000 ) | 0x80000000;
					else if( index1 == 2 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) &0xff00 ) | 0x00800000;
					else if( index1 == 1 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | 0x00008000;
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				d += ( b ^ ( a | (~c) ) ) + x + 0x8f0ccc92;
				d = ( d << 10 ) | ( d >> 22 );
				d += a;
				x = index0 + 40;
				if( x < message_ptr_size )
				{
					index1 = static_cast<rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( rux::uint32 ) )
						x = *(rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) & 0xff00 ) | ( ( ( (rux::uint32)message_ptr[ x + 2 ]) << 16 ) & 0xff0000 ) | 0x80000000;
					else if( index1 == 2 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) &0xff00 ) | 0x00800000;
					else if( index1 == 1 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | 0x00008000;
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				c += ( a ^ ( d | (~b) ) ) + x + 0xffeff47d;
				c = ( c << 15 ) | ( c >> 17 );
				c += d;
				x = index0 + 4;
				if( x < message_ptr_size )
				{
					index1 = static_cast<rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( rux::uint32 ) )
						x = *(rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) & 0xff00 ) | ( ( ( (rux::uint32)message_ptr[ x + 2 ]) << 16 ) & 0xff0000 ) | 0x80000000;
					else if( index1 == 2 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) &0xff00 ) | 0x00800000;
					else if( index1 == 1 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | 0x00008000;
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				b += ( d ^ ( c | (~a) ) ) + x + 0x85845dd1;
				b = ( b << 21 ) | ( b >> 11 );
				b += c;
				x = index0 + 32;
				if( x < message_ptr_size )
				{
					index1 = static_cast<rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( rux::uint32 ) )
						x = *(rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) & 0xff00 ) | ( ( ( (rux::uint32)message_ptr[ x + 2 ]) << 16 ) & 0xff0000 ) | 0x80000000;
					else if( index1 == 2 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) &0xff00 ) | 0x00800000;
					else if( index1 == 1 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | 0x00008000;
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				a += ( c ^ ( b | (~d) ) ) + x + 0x6fa87e4f;
				a = ( a << 6 ) | ( a >> ( 26 ) );
				a += b;
				x = index0 + 60;
				if( x < message_ptr_size )
				{
					index1 = static_cast<rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( rux::uint32 ) )
						x = *(rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) & 0xff00 ) | ( ( ( (rux::uint32)message_ptr[ x + 2 ]) << 16 ) & 0xff0000 ) | 0x80000000;
					else if( index1 == 2 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) &0xff00 ) | 0x00800000;
					else if( index1 == 1 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | 0x00008000;
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				d += ( b ^ ( a | (~c) ) ) + x + 0xfe2ce6e0;
				d = ( d << 10 ) | ( d >> 22 );
				d += a;
				x = index0 + 24;
				if( x < message_ptr_size )
				{
					index1 = static_cast<rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( rux::uint32 ) )
						x = *(rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) & 0xff00 ) | ( ( ( (rux::uint32)message_ptr[ x + 2 ]) << 16 ) & 0xff0000 ) | 0x80000000;
					else if( index1 == 2 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) &0xff00 ) | 0x00800000;
					else if( index1 == 1 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | 0x00008000;
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				c += ( a ^ ( d | (~b) ) ) + x + 0xa3014314;
				c = ( c << 15 ) | ( c >> 17 );
				c += d;  
				x = index0 + 52;
				if( x < message_ptr_size )
				{
					index1 = static_cast<rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( rux::uint32 ) )			
						x = *(rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) & 0xff00 ) | ( ( ( (rux::uint32)message_ptr[ x + 2 ]) << 16 ) & 0xff0000 ) | 0x80000000;
					else if( index1 == 2 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) &0xff00 ) | 0x00800000;
					else if( index1 == 1 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | 0x00008000;
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				b += ( d ^ ( c | (~a) ) ) + x + 0x4e0811a1;
				b = ( b << 21 ) | ( b >> 11 );
				b += c;  
				x = index0 + 16;
				if( x < message_ptr_size )
				{
					index1 = static_cast<rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( rux::uint32 ) )
						x = *(rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) & 0xff00 ) | ( ( ( (rux::uint32)message_ptr[ x + 2 ]) << 16 ) & 0xff0000 ) | 0x80000000;
					else if( index1 == 2 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) &0xff00 ) | 0x00800000;
					else if( index1 == 1 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | 0x00008000;
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				a += ( c ^ ( b | (~d) ) ) + x + 0xf7537e82;
				a = ( a << 6 ) | ( a >> ( 26 ) );
				a += b;  
				x = index0 + 44;
				if( x < message_ptr_size )
				{
					index1 = static_cast<rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( rux::uint32 ) )
						x = *(rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) & 0xff00 ) | ( ( ( (rux::uint32)message_ptr[ x + 2 ]) << 16 ) & 0xff0000 ) | 0x80000000;
					else if( index1 == 2 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) &0xff00 ) | 0x00800000;
					else if( index1 == 1 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | 0x00008000;
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				d += ( b ^ ( a | (~c) ) ) + x + 0xbd3af235;
				d = ( d << 10 ) | ( d >> 22 );
				d += a;  
				x = index0 + 8;
				if( x < message_ptr_size )
				{
					index1 = static_cast<rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( rux::uint32 ) )
						x = *(rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) & 0xff00 ) | ( ( ( (rux::uint32)message_ptr[ x + 2 ]) << 16 ) & 0xff0000 ) | 0x80000000;
					else if( index1 == 2 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) &0xff00 ) | 0x00800000;
					else if( index1 == 1 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | 0x00008000;
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				c += ( a ^ ( d | (~b) ) ) + x + 0x2ad7d2bb;
				c = ( c << 15 ) | ( c >> 17 );
				c += d;
				x = index0 + 36;
				if( x < message_ptr_size )
				{
					index1 = static_cast<rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( rux::uint32 ) )
						x = *(rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) & 0xff00 ) | ( ( ( (rux::uint32)message_ptr[ x + 2 ]) << 16 ) & 0xff0000 ) | 0x80000000;
					else if( index1 == 2 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | ( ( ( (rux::uint32)message_ptr[ x + 1 ] ) << 8 ) &0xff00 ) | 0x00800000;
					else if( index1 == 1 )
						x = ( (rux::uint32)message_ptr[ x ] & 0xff ) | 0x00008000;
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				b += ( d ^ ( c | (~a) ) ) + x + 0xeb86d391;
				b = ( b << 21 ) | ( b >> 11 );
				b += c;
				aa += a;
				bb += b;
				cc += c;
				dd += d;
			}		
			hash[ 0 ] = (rux::uint8)( aa & 0xff );
			hash[ 1 ] = (rux::uint8)( ( aa >> 8 ) & 0xff );
			hash[ 2 ] = (rux::uint8)( ( aa >> 16 ) & 0xff );
			hash[ 3 ] = (rux::uint8)( ( aa >> 24 ) & 0xff );
			hash[ 4 ] = (rux::uint8)( bb & 0xff );
			hash[ 5 ] = (rux::uint8)( ( bb >> 8 ) & 0xff );
			hash[ 6 ] = (rux::uint8)( ( bb >> 16 ) & 0xff );
			hash[ 7 ] = (rux::uint8)( ( bb >> 24 ) & 0xff );
			hash[ 8 ] = (rux::uint8)( cc & 0xff );
			hash[ 9 ] = (rux::uint8)( ( cc >> 8 ) & 0xff );
			hash[ 10 ] = (rux::uint8)( ( cc >> 16 ) & 0xff );
			hash[ 11 ] = (rux::uint8)( ( cc >> 24 ) & 0xff );
			hash[ 12 ] = (rux::uint8)( dd & 0xff );
			hash[ 13 ] = (rux::uint8)( ( dd >> 8 ) & 0xff );
			hash[ 14 ] = (rux::uint8)( ( dd >> 16 ) & 0xff );
			hash[ 15 ] = (rux::uint8)( ( dd >> 24 ) & 0xff );
		};
	};
};