#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_randomnumbergenerator.h>
#include <xan_rawdata.h>
#include <xan_time.h>
#include <xan_keyvaluepair.h>
#include <xan_uint64.h>
#include <xan_boolean.h>
begin_implement_rux_class_ns( RandomNumberGenerator , rux );
end_implement_rux_class();
namespace rux
{	
	void XRandomNumberGenerator::Generate( ::rux::uint8 number[ 128 ] )
	{
	#ifdef __WINDOWS__
		::rux::uint32 i0 = GetTickCount();
		::rux::int64 i1 = 0;
		GetSystemTimeAsFileTime( (LPFILETIME)&i1 );
		GetLocalTime( (LPSYSTEMTIME)number );		
		::rux::int32 cpu_info[ 4 ];
		__cpuid( cpu_info , 1 );
		::rux::uint32 i2 = *(::rux::uint32*)&cpu_info[ 0 ];
		::rux::uint32 i3 = *(::rux::uint32*)&cpu_info[ 2 ];
	#endif
	#ifdef __UNIX__
	#ifdef __LINUX__
		struct sysinfo s_info;
			int error;
			error = sysinfo(&s_info);
		::rux::uint32 i0 = s_info.uptime;
	#elif defined( __SOLARIS__ )
			::rux::uint32 i0 = 0;
	#endif
		::rux::int64 i1 = ::rux::XTime::get_NowAsUnixTime();
		*(::rux::uint64*)&number = ::rux::XTime::get_LocalNowAsWindowsTime();
	#ifdef __LINUX__
		::rux::uint32 i2 = s_info.totalhigh;
		::rux::uint32 i3 = s_info.totalram;
	#elif defined( __SOLARIS__ )
			::rux::uint32 i2 = 0;
		::rux::uint32 i3 = 0;
	#endif
	#endif
		*(::rux::int64*)&number[ i1 % 32 ] |= ~ *(::rux::int64*)&(*this)()->_feedback[ 8 ];
		*(::rux::int64*)&number[ 0 ] ^= *(::rux::int64*)&(*this)()->_feedback[ i1 % 120 ];
		i2 += i3 << 2;	
		i3 -= static_cast< ::rux::uint32>( i1 << 7 ^ 0x8891 * i2 );
		i2 *= i3 + ((::rux::uint8*)&i1)[ 3 ];
		i0 -= i3 | i2 ^ ((::rux::uint8*)&i1)[ i2 % 8 ];
		i1 = i3 % 3;
		if( i1 == 0 )
			i1 = i3 % 5;
		if( i1 == 0 )
			i1 = i3 % 4;
		::rux::uint32 remainder = 0;
		::rux::uint32 padding_length = 0;
		declare_stack_variable( char , message_length_ptr , 8 );
		::rux::uint32 a = 0x67452301;
		::rux::uint32 b = 0xefcdab89;
		::rux::uint32 c = 0x98badcfe;
		::rux::uint32 d = 0x10325476;
		::rux::uint32 aa = 0x67452301;
		::rux::uint32 bb = 0xefcdab89;
		::rux::uint32 cc = 0x98badcfe;
		::rux::uint32 dd = 0x10325476;
		::rux::uint32 index1 = 0;
		::rux::uint32 x = 0;
		char* message_ptr = NULL;
		::rux::uint64 message_ptr_size = 0;	
		while( i1 > 0 )
		{		
			if( ( i2 | 0x28a6 ) > ( (::rux::uint32)((::rux::uint8*)&i3)[ i3 % 4 ] ^ 0x1986 ) )
			{
				i0 *= i2;
				message_ptr = (char*)&number[ ( aa * bb ^ c ) % 77 ];
				message_ptr_size = 128 - ( ( aa * bb ^ c ) % 111 );
				i2 += static_cast< ::rux::uint32>( i1 * i3 );
			}
			else
			{
				i0 ^= i3;
				message_ptr = (char*)&number[ ( cc * a | d ) % 127 ];
				message_ptr_size = 128 - ( ( cc * a | d ) % 33 );
				i3 += static_cast< ::rux::uint32>( i1 * i2 | dd );
			}
			padding_length = 0;			
			if( message_ptr_size < 56 )		//448 bits
				padding_length = 56 - static_cast< ::rux::uint32>( message_ptr_size );
			else if( message_ptr_size < 64 )		
				padding_length = 120 - static_cast< ::rux::uint32>( message_ptr_size );//960 bits		
			else
			{
				remainder = message_ptr_size % 64;//512 bits			
				if( remainder > 55 )//440 bits			
					padding_length = 120 - remainder;
				else			
					padding_length = 56 - remainder;
			}
			message_ptr_size *= 8;
			message_length_ptr[ 0 ] = (::rux::uint8)( message_ptr_size & 0xff );
			message_length_ptr[ 1 ] = (::rux::uint8)( ( message_ptr_size >> 8 ) & 0xff );
			message_length_ptr[ 2 ] = (::rux::uint8)( ( message_ptr_size >> 16 ) & 0xff );
			message_length_ptr[ 3 ] = (::rux::uint8)( ( message_ptr_size >> 24 ) & 0xff );
			message_length_ptr[ 4 ] = (::rux::uint8)( ( message_ptr_size >> 32 ) & 0xff );
			message_length_ptr[ 5 ] = (::rux::uint8)( ( message_ptr_size >> 40 ) & 0xff );
			message_length_ptr[ 6 ] = (::rux::uint8)( ( message_ptr_size >> 48 ) & 0xff );
			message_length_ptr[ 7 ] = (::rux::uint8)( ( message_ptr_size >> 56 ) & 0xff );
			message_ptr_size /= 8;
			a = 0x67452301;
			b = 0xefcdab89;
			c = 0x98badcfe;
			d = 0x10325476;
			aa = 0x67452301;
			bb = 0xefcdab89;
			cc = 0x98badcfe;
			dd = 0x10325476;
			index1 = 0;
			x = 0;
			for( ::rux::uint32 index0 = 0 ; index0 < message_ptr_size + padding_length + 8 ; index0 += 64 )
			{			
				a = aa;
				b = bb;
				c = cc;
				d = dd;
				x = index0;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				a += ( ( b & c ) | ( ( ~b ) & d ) ) + x + 0xd76aa478;
				a = ( a << 7 ) | ( a >> ( 25 ) );
				a += b;		
				x = index0 + 4;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];				  
				d += ( ( a & b ) | ( ( ~a ) & c ) ) + x + 0xe8c7b756;
				d = ( d << 12 ) | ( d >> 20 );
				d += a;
				x = index0 + 8;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];				  
				c += ( ( d & a ) | ( ( ~d ) & b ) ) + x + 0x242070db;
				c = ( c << 17 ) | ( c >> 15 );
				c += d;
				x = index0 + 12;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];				
				b += ( ( c & d ) | ( ( ~c ) & a ) ) + x + 0xc1bdceee;
				b = ( b << 22 ) | ( b >> 10 );
				b += c;
				x = index0 + 16;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];				
				a += ( ( b & c ) | ( ( ~b ) & d ) ) + x + 0xf57c0faf;
				a = ( a << 7 ) | ( a >> ( 25 ) );
				a += b;
				x = index0 + 20;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				d += ( ( a & b ) | ( ( ~a ) & c ) ) + x + 0x4787c62a;
				d = ( d << 12 ) | ( d >> 20 );
				d += a;
				x = index0 + 24;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				c += ( ( d & a ) | ( ( ~d ) & b ) ) + x + 0xa8304613;
				c = ( c << 17 ) | ( c >> 15 );
				c += d;
				x = index0 + 28;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				b += ( ( c & d ) | ( ( ~c ) & a ) ) + x + 0xfd469501;
				b = ( b << 22 ) | ( b >> 10 );
				b += c;
				x = index0 + 32;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				a += ( ( b & c ) | ( ( ~b ) & d ) ) + x + 0x698098d8;
				a = ( a << 7 ) | ( a >> ( 25 ) );
				a += b;
				x = index0 + 36;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				d += ( ( a & b ) | ( ( ~a ) & c ) ) + x + 0x8b44f7af;
				d = ( d << 12 ) | ( d >> 20 );
				d += a;
				x = index0 + 40;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				c += ( ( d & a ) | ( ( ~d ) & b ) ) + x + 0xffff5bb1;
				c = ( c << 17 ) | ( c >> 15 );
				c += d;
				x = index0 + 44;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				b += ( ( c & d ) | ( ( ~c ) & a ) ) + x + 0x895cd7be;
				b = ( b << 22 ) | ( b >> 10 );
				b += c;
				x = index0 + 48;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				a += ( ( b & c ) | ( ( ~b ) & d ) ) + x + 0x6b901122;
				a = ( a << 7 ) | ( a >> ( 25 ) );
				a += b;
				x = index0 + 52;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				d += ( ( a & b ) | ( ( ~a ) & c ) ) + x + 0xfd987193;
				d = ( d << 12 ) | ( d >> 20 );
				d += a;
				x = index0 + 56;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				c += ( ( d & a ) | ( ( ~d ) & b ) ) + x + 0xa679438e;
				c = ( c << 17 ) | ( c >> 15 );
				c += d;
				x = index0 + 60;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				b += ( ( c & d ) | ( ( ~c ) & a ) ) + x + 0x49b40821;
				b = ( b << 22 ) | ( b >> 10 );
				b += c;			
				x = index0 + 4;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				a += ( ( b & d ) | ( c & ( ~d ) ) ) + x + 0xf61e2562;
				a = ( a << 5 ) | ( a >> ( 27 ) );
				a += b;
				x = index0 + 24;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				d += ( ( a & c ) | ( b & ( ~c ) ) ) + x + 0xc040b340;
				d = ( d << 9) | ( d >> 23 );
				d += a;
				x = index0 + 44;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				c += ( ( d & b ) | ( a & ( ~b ) ) ) + x + 0x265e5a51;
				c = ( c << 14 ) | ( c >> 18 );
				c += d;
				x = index0;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				b += ( ( c & a ) | ( d & ( ~a ) ) ) + x + 0xe9b6c7aa;
				b = ( b << 20 ) | ( b >> 12 );
				b += c;
				x = index0 + 20;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				a += ( ( b & d ) | ( c & ( ~d ) ) ) + x + 0xd62f105d;
				a = ( a << 5 ) | ( a >> ( 27 ) );
				a += b;
				x = index0 + 40;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				d += ( ( a & c ) | ( b & ( ~c ) ) ) + x + 0x2441453;
				d = ( d << 9 ) | ( d >> 23 );
				d += a;
				x = index0 + 60;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				c += ( ( d & b ) | ( a & ( ~b ) ) ) + x + 0xd8a1e681;
				c = ( c << 14 ) | ( c >> 18 );
				c += d;
				x = index0 + 16;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				b += ( ( c & a ) | ( d & ( ~a ) ) ) + x + 0xe7d3fbc8;
				b = ( b << 20 ) | ( b >> 12 );
				b += c;
				x = index0 + 36;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				a += ( ( b & d ) | ( c & ( ~d ) ) ) + x + 0x21e1cde6;
				a = ( a << 5 ) | ( a >> ( 27 ) );
				a += b;
				x = index0 + 56;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				d += ( ( a & c ) | ( b & ( ~c ) ) ) + x + 0xc33707d6;
				d = ( d << 9 ) | ( d >> 23 );
				d += a;
				x = index0 + 12;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				c += ( ( d & b ) | ( a & ( ~b ) ) ) + x + 0xf4d50d87;
				c = ( c << 14 ) | ( c >> 18 );
				c += d;
				x = index0 + 32;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				b += ( ( c & a ) | ( d & ( ~a ) ) ) + x + 0x455a14ed;
				b = ( b << 20 ) | ( b >> 12 );
				b += c;
				x = index0 + 52;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				a += ( ( b & d ) | ( c & ( ~d ) ) ) + x + 0xa9e3e905;
				a = ( a << 5 ) | ( a >> ( 27 ) );
				a += b;
				x = index0 + 8;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				d += ( ( a & c ) | ( b & ( ~c ) ) ) + x + 0xfcefa3f8;
				d = ( d << 9 ) | ( d >> 23 );
				d += a;				
				x = index0 + 28;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				c += ( ( d & b ) | ( a & ( ~b ) ) ) + x + 0x676f02d9;
				c = ( c << 14 ) | ( c >> 18 );
				c += d;				
				x = index0 + 48;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				b += ( ( c & a ) | ( d & ( ~a ) ) ) + x + 0x8d2a4c8a;
				b = ( b << 20 ) | ( b >> 12 );
				b += c;							
				x = index0 + 20;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				a += ( b ^ c ^ d ) + x + 0xfffa3942;
				a = ( a << 4 ) | ( a >> ( 28 ) );
				a += b;
				x = index0 + 32;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				d += ( a ^ b ^ c ) + x + 0x8771f681;
				d = ( d << 11 ) | ( d >> 21 );
				d += a;
				x = index0 + 44;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				c += ( d ^ a ^ b ) + x + 0x6d9d6122;
				c = ( c << 16 ) | ( c >> 16 );
				c += d;
				x = index0 + 56;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				b += ( c ^ d ^ a ) + x + 0xfde5380c;
				b = ( b << 23 ) | ( b >> 9 );
				b += c;  
				x = index0 + 4;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				a += ( b ^ c ^ d ) + x + 0xa4beea44;
				a = ( a << 4 ) | ( a >> ( 28 ) );
				a += b;  
				x = index0 + 16;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				d += ( a ^ b ^ c ) + x + 0x4bdecfa9;
				d = ( d << 11 ) | ( d >> 21 );
				d += a;  
				x = index0 + 28;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				c += ( d ^ a ^ b ) + x + 0xf6bb4b60;
				c = ( c << 16 ) | ( c >> 16 );
				c += d;  
				x = index0 + 40;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				b += ( c ^ d ^ a ) + x + 0xbebfbc70;
				b = ( b << 23 ) | ( b >> 9 );
				b += c;  
				x = index0 + 52;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				a += ( b ^ c ^ d ) + x + 0x289b7ec6;
				a = ( a << 4 ) | ( a >> ( 28 ) );
				a += b;  
				x = index0;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				d += ( a ^ b ^ c ) + x + 0xeaa127fa;
				d = ( d << 11 ) | ( d >> 21 );
				d += a;  
				x = index0 + 12;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				c += ( d ^ a ^ b ) + x + 0xd4ef3085;
				c = ( c << 16 ) | ( c >> 16 );
				c += d;  
				x = index0 + 24;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				b += ( c ^ d ^ a ) + x + 0x4881d05;
				b = ( b << 23 ) | ( b >> 9 );
				b += c;  
				x = index0 + 36;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				a += ( b ^ c ^ d ) + x + 0xd9d4d039;
				a = ( a << 4 ) | ( a >> ( 28 ) );
				a += b;    
				x = index0 + 48;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				d += ( a ^ b ^ c ) + x + 0xe6db99e5;
				d = ( d << 11 ) | ( d >> 21 );
				d += a;    
				x = index0 + 60;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				c += ( d ^ a ^ b ) + x + 0x1fa27cf8;
				c = ( c << 16 ) | ( c >> 16 );
				c += d;    
				x = index0 + 8;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				b += ( c ^ d ^ a ) + x + 0xc4ac5665;
				b = ( b << 23 ) | ( b >> 9 );
				b += c;       			
				x = index0;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				a += ( c ^ ( b | (~d) ) ) + x + 0xf4292244;
				a = ( a << 6 ) | ( a >> ( 26 ) );
				a += b;
				x = index0 + 28;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				d += ( b ^ ( a | (~c) ) ) + x + 0x432aff97;
				d = ( d << 10 ) | ( d >> 22 );
				d += a;
				x = index0 + 56;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				c += ( a ^ ( d | (~b) ) ) + x + 0xab9423a7;
				c = ( c << 15 ) | ( c >> 17 );
				c += d;
				x = index0 + 20;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				b += ( d ^ ( c | (~a) ) ) + x + 0xfc93a039;
				b = ( b << 21 ) | ( b >> 11 );
				b += c;
				x = index0 + 48;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				a += ( c ^ ( b | (~d) ) ) + x + 0x655b59c3;
				a = ( a << 6 ) | ( a >> ( 26 ) );
				a += b;
				x = index0 + 12;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				d += ( b ^ ( a | (~c) ) ) + x + 0x8f0ccc92;
				d = ( d << 10 ) | ( d >> 22 );
				d += a;
				x = index0 + 40;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				c += ( a ^ ( d | (~b) ) ) + x + 0xffeff47d;
				c = ( c << 15 ) | ( c >> 17 );
				c += d;
				x = index0 + 4;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				b += ( d ^ ( c | (~a) ) ) + x + 0x85845dd1;
				b = ( b << 21 ) | ( b >> 11 );
				b += c;
				x = index0 + 32;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				a += ( c ^ ( b | (~d) ) ) + x + 0x6fa87e4f;
				a = ( a << 6 ) | ( a >> ( 26 ) );
				a += b;
				x = index0 + 60;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				number[ 38 ] += ((::rux::uint8*)&i3)[ 3 ];
				number[ 39 ] *= ((::rux::uint8*)&i3)[ 0 ];
				number[ 40 ] -= ((::rux::uint8*)&i3)[ 2 ];
				number[ 41 ] ^= ((::rux::uint8*)&i3)[ 1 ];
				number[ 42 ] &= ((::rux::uint8*)&i0)[ 3 ];
				number[ 43 ] |= ((::rux::uint8*)&i0)[ 0 ];
				number[ 44 ] *= ((::rux::uint8*)&i0)[ 2 ];
				number[ 45 ] *= ((::rux::uint8*)&i0)[ 1 ];
				number[ 46 ] += ((::rux::uint8*)&i0)[ 3 ];
				number[ 47 ] |= ((::rux::uint8*)&i3)[ 0 ];
				number[ 48 ] &= ((::rux::uint8*)&i3)[ 2 ];
				number[ 49 ] += ((::rux::uint8*)&i3)[ 1 ];
				number[ 50 ] *= ((::rux::uint8*)&i1)[ 3 ];
				number[ 51 ] -= ((::rux::uint8*)&i1)[ 0 ];
				number[ 52 ] ^= ((::rux::uint8*)&i1)[ 2 ];
				number[ 53 ] += ((::rux::uint8*)&i1)[ 1 ];
				number[ 54 ] += ((::rux::uint8*)&bb)[ 3 ];
				number[ 55 ] |= ((::rux::uint8*)&bb)[ 0 ];
				number[ 56 ] &= ((::rux::uint8*)&x)[ 2 ];
				number[ 57 ] *= ((::rux::uint8*)&dd)[ 1 ];
				number[ 58 ] ^= ((::rux::uint8*)&cc)[ 3 ];
				number[ 59 ] *= ((::rux::uint8*)&d)[ 0 ];
				number[ 60 ] &= ((::rux::uint8*)&i0)[ 2 ];
				number[ 61 ] |= ((::rux::uint8*)&aa)[ 1 ];
				number[ 62 ] += ((::rux::uint8*)&i3)[ 1 ];
				number[ 63 ] -= ((::rux::uint8*)&d)[ 3 ];
				number[ 64 ] &= ((::rux::uint8*)&bb)[ 0 ];
				number[ 65 ] += ((::rux::uint8*)&i3)[ 3 ];
				number[ 66 ] += ((::rux::uint8*)&i3)[ 0 ];
				number[ 67 ] += ((::rux::uint8*)&i3)[ 2 ];
				number[ 68 ] += ((::rux::uint8*)&i3)[ 1 ];
				number[ 69 ] *= ((::rux::uint8*)&i0)[ 3 ];
				number[ 70 ] += ((::rux::uint8*)&i0)[ 0 ];
				number[ 71 ] *= ((::rux::uint8*)&i0)[ 2 ];
				number[ 72 ] += ((::rux::uint8*)&i0)[ 1 ];
				number[ 37 ] -= ((::rux::uint8*)&i0)[ 3 ];
				number[ 36 ] += ((::rux::uint8*)&i3)[ 0 ];
				number[ 35 ] *= ((::rux::uint8*)&i3)[ 2 ];
				number[ 34 ] -= ((::rux::uint8*)&i3)[ 1 ];
				number[ 33 ] -= ((::rux::uint8*)&i1)[ 3 ];
				number[ 32 ] += ((::rux::uint8*)&i1)[ 0 ];
				number[ 31 ] *= ((::rux::uint8*)&i1)[ 2 ];
				number[ 30 ] -= ((::rux::uint8*)&i1)[ 1 ];
				number[ 29 ] -= ((::rux::uint8*)&bb)[ 3 ];
				number[ 28 ] *= ((::rux::uint8*)&bb)[ 0 ];
				number[ 27 ] += ((::rux::uint8*)&x)[ 2 ];
				number[ 26 ] -= ((::rux::uint8*)&dd)[ 1 ];
				number[ 25 ] *= ((::rux::uint8*)&cc)[ 3 ];			
				d += ( b ^ ( a | (~c) ) ) + x + 0xfe2ce6e0;
				d = ( d << 10 ) | ( d >> 22 );
				d += a;
				x = index0 + 24;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				c += ( a ^ ( d | (~b) ) ) + x + 0xa3014314;
				c = ( c << 15 ) | ( c >> 17 );
				c += d;  
				x = index0 + 52;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )			
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				b += ( d ^ ( c | (~a) ) ) + x + 0x4e0811a1;
				b = ( b << 21 ) | ( b >> 11 );
				b += c;  
				x = index0 + 16;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				a += ( c ^ ( b | (~d) ) ) + x + 0xf7537e82;
				a = ( a << 6 ) | ( a >> ( 26 ) );
				a += b;  
				x = index0 + 44;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				d += ( b ^ ( a | (~c) ) ) + x + 0xbd3af235;
				d = ( d << 10 ) | ( d >> 22 );
				d += a;  
				x = index0 + 8;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				c += ( a ^ ( d | (~b) ) ) + x + 0x2ad7d2bb;
				c = ( c << 15 ) | ( c >> 17 );
				c += d;
				x = index0 + 36;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				b += ( d ^ ( c | (~a) ) ) + x + 0xeb86d391;
				b = ( b << 21 ) | ( b >> 11 );
				b += c;
				aa += a;
				bb += b;
				cc += c;
				dd += d;
			}
			if( i1 == 2 )
				::rux::engine::memcpy( (*this)()->_feedback , 0 , number , ( i3 ^ i2 ) % 88 );
			number[ ( x | i3 ) %29 ] = (::rux::uint8)( aa & 0xff );
			number[ ( dd ^ i2 ) %17 ] = (::rux::uint8)( ( aa >> 8 ) & 0xff );
			number[ ( 1986 * x ^ i2 ) %125 ] = (::rux::uint8)( ( aa >> 16 ) & 0xff );
			number[ ( i3 | ~ aa ) %114 ] = (::rux::uint8)( ( aa >> 24 ) & 0xff );
			number[ 4 ] = (::rux::uint8)( bb & 0xff );
			number[ ( i2 ^ i3 * bb ) %128 ] = (::rux::uint8)( ( bb >> 8 ) & 0xff );
			number[ ( i3 ^ i2 * d ) %18 ] = (::rux::uint8)( ( bb >> 16 ) & 0xff );
			number[ ( c * i3 + number[ 51 ] ) %64 ] = (::rux::uint8)( ( bb >> 24 ) & 0xff );
			number[ ( i3 * number[ 86 ] ^ i2 ) %97 ] = (::rux::uint8)( cc & 0xff );
			number[ ( number[ i2 % 128 ] - d | b ) %43 ] = (::rux::uint8)( ( cc >> 8 ) & 0xff );
			number[ ( i1 * i3 | ~ cc ) %99 ] = (::rux::uint8)( ( cc >> 16 ) & 0xff );
			number[ ( i2 ^ c * bb ) %126 ] = (::rux::uint8)( ( cc >> 24 ) & 0xff );
			number[ ( x + i2 * b ) %98 ] = (::rux::uint8)( dd & 0xff );
			number[ ( i2 | ~ (x | cc ) ) %89 ] = (::rux::uint8)( ( dd >> 8 ) & 0xff );
			number[ ( i1 * i3 | ~ cc ) %101 ] ^= (::rux::uint8)( ( dd >> 16 ) & 0xff );
			number[ ( i2 * i3 - a ) %113 ] = (::rux::uint8)( ( dd >> 24 ) & 0xff );		
			number[ 72 ] += ((::rux::uint8*)&i3)[ 3 ];
			number[ 73 ] *= ((::rux::uint8*)&i3)[ 0 ];
			number[ 74 ] -= ((::rux::uint8*)&i3)[ 2 ];
			number[ 75 ] ^= ((::rux::uint8*)&i3)[ 1 ];
			number[ 76 ] &= ((::rux::uint8*)&i0)[ 3 ];
			number[ 77 ] |= ((::rux::uint8*)&i0)[ 0 ];
			number[ 78 ] -= ((::rux::uint8*)&i0)[ 2 ];
			number[ 79 ] *= ((::rux::uint8*)&i0)[ 1 ];
			number[ 80 ] += ((::rux::uint8*)&i0)[ 3 ];
			number[ 81 ] |= ((::rux::uint8*)&i3)[ 0 ];
			number[ 82 ] &= ((::rux::uint8*)&i3)[ 2 ];
			number[ 83 ] += ((::rux::uint8*)&i3)[ 1 ];
			number[ 84 ] *= ((::rux::uint8*)&i1)[ 3 ];
			number[ 85 ] -= ((::rux::uint8*)&i1)[ 0 ];
			number[ 86 ] ^= ((::rux::uint8*)&i1)[ 2 ];
			number[ 87 ] += ((::rux::uint8*)&i1)[ 1 ];
			number[ 88 ] += ((::rux::uint8*)&bb)[ 3 ];
			number[ 89 ] |= ((::rux::uint8*)&bb)[ 0 ];
			number[ 90 ] &= ((::rux::uint8*)&x)[ 2 ];
			number[ 91 ] *= ((::rux::uint8*)&dd)[ 1 ];
			number[ 92 ] ^= ((::rux::uint8*)&cc)[ 3 ];
			number[ 93 ] *= ((::rux::uint8*)&d)[ 0 ];
			number[ 94 ] &= ((::rux::uint8*)&i0)[ 2 ];
			number[ 95 ] |= ((::rux::uint8*)&aa)[ 1 ];
			number[ 96 ] += ((::rux::uint8*)&i3)[ 1 ];
			number[ 97 ] -= ((::rux::uint8*)&d)[ 3 ];
			number[ 98 ] &= ((::rux::uint8*)&bb)[ 0 ];
			number[ 100 ] += ((::rux::uint8*)&i3)[ 3 ];
			number[ 101 ] += ((::rux::uint8*)&i3)[ 0 ];
			number[ 123 ] += ((::rux::uint8*)&i3)[ 2 ];
			number[ 119 ] += ((::rux::uint8*)&i3)[ 1 ];
			number[ 103 ] *= ((::rux::uint8*)&i0)[ 3 ];
			number[ 127 ] += ((::rux::uint8*)&i0)[ 0 ];
			number[ 122 ] *= ((::rux::uint8*)&i0)[ 2 ];
			number[ 107 ] += ((::rux::uint8*)&i0)[ 1 ];
			number[ 102 ] -= ((::rux::uint8*)&i0)[ 3 ];
			number[ 106 ] += ((::rux::uint8*)&i3)[ 0 ];
			number[ 124 ] *= ((::rux::uint8*)&i3)[ 2 ];
			number[ 118 ] -= ((::rux::uint8*)&i3)[ 1 ];
			number[ 105 ] -= ((::rux::uint8*)&i1)[ 3 ];
			number[ 116 ] += ((::rux::uint8*)&i1)[ 0 ];
			number[ 126 ] *= ((::rux::uint8*)&i1)[ 2 ];
			number[ 115 ] -= ((::rux::uint8*)&i1)[ 1 ];
			number[ 14 ] -= ((::rux::uint8*)&bb)[ 3 ];
			number[ 111 ] *= ((::rux::uint8*)&bb)[ 0 ];
			number[ 112 ] += ((::rux::uint8*)&x)[ 2 ];
			number[ 113 ] -= ((::rux::uint8*)&dd)[ 1 ];
			number[ 19 ] *= ((::rux::uint8*)&cc)[ 3 ];
			number[ 117 ] += ((::rux::uint8*)&d)[ 0 ];
			number[ 121 ] += ((::rux::uint8*)&i0)[ 2 ];
			number[ 108 ] ^= ((::rux::uint8*)&aa)[ 1 ];
			number[ 114 ] -= ((::rux::uint8*)&i3)[ 1 ];
			number[ 120 ] += ((::rux::uint8*)&d)[ 3 ];
			number[ 125 ] += ((::rux::uint8*)&bb)[ 0 ];		
			i1--;
		}	
		message_ptr = (char*)number;
		message_ptr_size = 128;
		padding_length = 56;
		message_ptr_size *= 8;
		message_length_ptr[ 0 ] = (::rux::uint8)( message_ptr_size & 0xff );
		message_length_ptr[ 1 ] = (::rux::uint8)( ( message_ptr_size >> 8 ) & 0xff );
		message_length_ptr[ 2 ] = (::rux::uint8)( ( message_ptr_size >> 16 ) & 0xff );
		message_length_ptr[ 3 ] = (::rux::uint8)( ( message_ptr_size >> 24 ) & 0xff );
		message_length_ptr[ 4 ] = (::rux::uint8)( ( message_ptr_size >> 32 ) & 0xff );
		message_length_ptr[ 5 ] = (::rux::uint8)( ( message_ptr_size >> 40 ) & 0xff );
		message_length_ptr[ 6 ] = (::rux::uint8)( ( message_ptr_size >> 48 ) & 0xff );
		message_length_ptr[ 7 ] = (::rux::uint8)( ( message_ptr_size >> 56 ) & 0xff );
		message_ptr_size /= 8;
		for( i1 = 0 ; i1 < 128 ; i1 += 16 )
		{		
			a = 0x67452301;
			b = 0xefcdab89;
			c = 0x98badcfe;
			d = 0x10325476;
			aa = 0x67452301;
			bb = 0xefcdab89;
			cc = 0x98badcfe;
			dd = 0x10325476;
			index1 = 0;
			x = 0;
			for( ::rux::uint32 index0 = 0 ; index0 < message_ptr_size + padding_length + 8 ; index0 += 64 )
			{			
				a = aa;
				b = bb;
				c = cc;
				d = dd;
				x = index0;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				a += ( ( b & c ) | ( ( ~b ) & d ) ) + x + 0xd76aa478;
				a = ( a << 7 ) | ( a >> ( 25 ) );
				a += b;		
				x = index0 + 4;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];				  
				d += ( ( a & b ) | ( ( ~a ) & c ) ) + x + 0xe8c7b756;
				d = ( d << 12 ) | ( d >> 20 );
				d += a;
				x = index0 + 8;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];				  
				c += ( ( d & a ) | ( ( ~d ) & b ) ) + x + 0x242070db;
				c = ( c << 17 ) | ( c >> 15 );
				c += d;
				x = index0 + 12;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];				
				b += ( ( c & d ) | ( ( ~c ) & a ) ) + x + 0xc1bdceee;
				b = ( b << 22 ) | ( b >> 10 );
				b += c;
				x = index0 + 16;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];				
				a += ( ( b & c ) | ( ( ~b ) & d ) ) + x + 0xf57c0faf;
				a = ( a << 7 ) | ( a >> ( 25 ) );
				a += b;
				x = index0 + 20;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				d += ( ( a & b ) | ( ( ~a ) & c ) ) + x + 0x4787c62a;
				d = ( d << 12 ) | ( d >> 20 );
				d += a;
				x = index0 + 24;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				c += ( ( d & a ) | ( ( ~d ) & b ) ) + x + 0xa8304613;
				c = ( c << 17 ) | ( c >> 15 );
				c += d;
				x = index0 + 28;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				b += ( ( c & d ) | ( ( ~c ) & a ) ) + x + 0xfd469501;
				b = ( b << 22 ) | ( b >> 10 );
				b += c;
				x = index0 + 32;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				a += ( ( b & c ) | ( ( ~b ) & d ) ) + x + 0x698098d8;
				a = ( a << 7 ) | ( a >> ( 25 ) );
				a += b;
				x = index0 + 36;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				d += ( ( a & b ) | ( ( ~a ) & c ) ) + x + 0x8b44f7af;
				d = ( d << 12 ) | ( d >> 20 );
				d += a;
				x = index0 + 40;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				c += ( ( d & a ) | ( ( ~d ) & b ) ) + x + 0xffff5bb1;
				c = ( c << 17 ) | ( c >> 15 );
				c += d;
				x = index0 + 44;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				b += ( ( c & d ) | ( ( ~c ) & a ) ) + x + 0x895cd7be;
				b = ( b << 22 ) | ( b >> 10 );
				b += c;
				x = index0 + 48;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				a += ( ( b & c ) | ( ( ~b ) & d ) ) + x + 0x6b901122;
				a = ( a << 7 ) | ( a >> ( 25 ) );
				a += b;
				x = index0 + 52;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				d += ( ( a & b ) | ( ( ~a ) & c ) ) + x + 0xfd987193;
				d = ( d << 12 ) | ( d >> 20 );
				d += a;
				x = index0 + 56;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				c += ( ( d & a ) | ( ( ~d ) & b ) ) + x + 0xa679438e;
				c = ( c << 17 ) | ( c >> 15 );
				c += d;
				x = index0 + 60;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				b += ( ( c & d ) | ( ( ~c ) & a ) ) + x + 0x49b40821;
				b = ( b << 22 ) | ( b >> 10 );
				b += c;			
				x = index0 + 4;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				a += ( ( b & d ) | ( c & ( ~d ) ) ) + x + 0xf61e2562;
				a = ( a << 5 ) | ( a >> ( 27 ) );
				a += b;
				x = index0 + 24;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				d += ( ( a & c ) | ( b & ( ~c ) ) ) + x + 0xc040b340;
				d = ( d << 9) | ( d >> 23 );
				d += a;
				x = index0 + 44;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				c += ( ( d & b ) | ( a & ( ~b ) ) ) + x + 0x265e5a51;
				c = ( c << 14 ) | ( c >> 18 );
				c += d;
				x = index0;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				b += ( ( c & a ) | ( d & ( ~a ) ) ) + x + 0xe9b6c7aa;
				b = ( b << 20 ) | ( b >> 12 );
				b += c;
				x = index0 + 20;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				a += ( ( b & d ) | ( c & ( ~d ) ) ) + x + 0xd62f105d;
				a = ( a << 5 ) | ( a >> ( 27 ) );
				a += b;
				x = index0 + 40;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				d += ( ( a & c ) | ( b & ( ~c ) ) ) + x + 0x2441453;
				d = ( d << 9 ) | ( d >> 23 );
				d += a;
				x = index0 + 60;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				c += ( ( d & b ) | ( a & ( ~b ) ) ) + x + 0xd8a1e681;
				c = ( c << 14 ) | ( c >> 18 );
				c += d;
				x = index0 + 16;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				b += ( ( c & a ) | ( d & ( ~a ) ) ) + x + 0xe7d3fbc8;
				b = ( b << 20 ) | ( b >> 12 );
				b += c;
				x = index0 + 36;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				a += ( ( b & d ) | ( c & ( ~d ) ) ) + x + 0x21e1cde6;
				a = ( a << 5 ) | ( a >> ( 27 ) );
				a += b;
				x = index0 + 56;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				d += ( ( a & c ) | ( b & ( ~c ) ) ) + x + 0xc33707d6;
				d = ( d << 9 ) | ( d >> 23 );
				d += a;
				x = index0 + 12;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				c += ( ( d & b ) | ( a & ( ~b ) ) ) + x + 0xf4d50d87;
				c = ( c << 14 ) | ( c >> 18 );
				c += d;
				x = index0 + 32;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				b += ( ( c & a ) | ( d & ( ~a ) ) ) + x + 0x455a14ed;
				b = ( b << 20 ) | ( b >> 12 );
				b += c;
				x = index0 + 52;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				a += ( ( b & d ) | ( c & ( ~d ) ) ) + x + 0xa9e3e905;
				a = ( a << 5 ) | ( a >> ( 27 ) );
				a += b;
				x = index0 + 8;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				d += ( ( a & c ) | ( b & ( ~c ) ) ) + x + 0xfcefa3f8;
				d = ( d << 9 ) | ( d >> 23 );
				d += a;				
				x = index0 + 28;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				c += ( ( d & b ) | ( a & ( ~b ) ) ) + x + 0x676f02d9;
				c = ( c << 14 ) | ( c >> 18 );
				c += d;				
				x = index0 + 48;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				b += ( ( c & a ) | ( d & ( ~a ) ) ) + x + 0x8d2a4c8a;
				b = ( b << 20 ) | ( b >> 12 );
				b += c;							
				x = index0 + 20;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				a += ( b ^ c ^ d ) + x + 0xfffa3942;
				a = ( a << 4 ) | ( a >> ( 28 ) );
				a += b;
				x = index0 + 32;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				d += ( a ^ b ^ c ) + x + 0x8771f681;
				d = ( d << 11 ) | ( d >> 21 );
				d += a;
				x = index0 + 44;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				c += ( d ^ a ^ b ) + x + 0x6d9d6122;
				c = ( c << 16 ) | ( c >> 16 );
				c += d;
				x = index0 + 56;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				b += ( c ^ d ^ a ) + x + 0xfde5380c;
				b = ( b << 23 ) | ( b >> 9 );
				b += c;  
				x = index0 + 4;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				a += ( b ^ c ^ d ) + x + 0xa4beea44;
				a = ( a << 4 ) | ( a >> ( 28 ) );
				a += b;  
				x = index0 + 16;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				d += ( a ^ b ^ c ) + x + 0x4bdecfa9;
				d = ( d << 11 ) | ( d >> 21 );
				d += a;  
				x = index0 + 28;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				c += ( d ^ a ^ b ) + x + 0xf6bb4b60;
				c = ( c << 16 ) | ( c >> 16 );
				c += d;  
				x = index0 + 40;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				b += ( c ^ d ^ a ) + x + 0xbebfbc70;
				b = ( b << 23 ) | ( b >> 9 );
				b += c;  
				x = index0 + 52;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				a += ( b ^ c ^ d ) + x + 0x289b7ec6;
				a = ( a << 4 ) | ( a >> ( 28 ) );
				a += b;  
				x = index0;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				d += ( a ^ b ^ c ) + x + 0xeaa127fa;
				d = ( d << 11 ) | ( d >> 21 );
				d += a;  
				x = index0 + 12;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				c += ( d ^ a ^ b ) + x + 0xd4ef3085;
				c = ( c << 16 ) | ( c >> 16 );
				c += d;  
				x = index0 + 24;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				b += ( c ^ d ^ a ) + x + 0x4881d05;
				b = ( b << 23 ) | ( b >> 9 );
				b += c;  
				x = index0 + 36;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				a += ( b ^ c ^ d ) + x + 0xd9d4d039;
				a = ( a << 4 ) | ( a >> ( 28 ) );
				a += b;    
				x = index0 + 48;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				d += ( a ^ b ^ c ) + x + 0xe6db99e5;
				d = ( d << 11 ) | ( d >> 21 );
				d += a;    
				x = index0 + 60;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				c += ( d ^ a ^ b ) + x + 0x1fa27cf8;
				c = ( c << 16 ) | ( c >> 16 );
				c += d;    
				x = index0 + 8;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				b += ( c ^ d ^ a ) + x + 0xc4ac5665;
				b = ( b << 23 ) | ( b >> 9 );
				b += c;       			
				x = index0;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				a += ( c ^ ( b | (~d) ) ) + x + 0xf4292244;
				a = ( a << 6 ) | ( a >> ( 26 ) );
				a += b;
				x = index0 + 28;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				d += ( b ^ ( a | (~c) ) ) + x + 0x432aff97;
				d = ( d << 10 ) | ( d >> 22 );
				d += a;
				x = index0 + 56;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				c += ( a ^ ( d | (~b) ) ) + x + 0xab9423a7;
				c = ( c << 15 ) | ( c >> 17 );
				c += d;
				x = index0 + 20;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				b += ( d ^ ( c | (~a) ) ) + x + 0xfc93a039;
				b = ( b << 21 ) | ( b >> 11 );
				b += c;
				x = index0 + 48;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				a += ( c ^ ( b | (~d) ) ) + x + 0x655b59c3;
				a = ( a << 6 ) | ( a >> ( 26 ) );
				a += b;
				x = index0 + 12;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				d += ( b ^ ( a | (~c) ) ) + x + 0x8f0ccc92;
				d = ( d << 10 ) | ( d >> 22 );
				d += a;
				x = index0 + 40;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				c += ( a ^ ( d | (~b) ) ) + x + 0xffeff47d;
				c = ( c << 15 ) | ( c >> 17 );
				c += d;
				x = index0 + 4;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				b += ( d ^ ( c | (~a) ) ) + x + 0x85845dd1;
				b = ( b << 21 ) | ( b >> 11 );
				b += c;
				x = index0 + 32;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				a += ( c ^ ( b | (~d) ) ) + x + 0x6fa87e4f;
				a = ( a << 6 ) | ( a >> ( 26 ) );
				a += b;
				x = index0 + 60;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];				
				d += ( b ^ ( a | (~c) ) ) + x + 0xfe2ce6e0;
				d = ( d << 10 ) | ( d >> 22 );
				d += a;
				x = index0 + 24;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				c += ( a ^ ( d | (~b) ) ) + x + 0xa3014314;
				c = ( c << 15 ) | ( c >> 17 );
				c += d;  
				x = index0 + 52;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )			
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				b += ( d ^ ( c | (~a) ) ) + x + 0x4e0811a1;
				b = ( b << 21 ) | ( b >> 11 );
				b += c;  
				x = index0 + 16;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				a += ( c ^ ( b | (~d) ) ) + x + 0xf7537e82;
				a = ( a << 6 ) | ( a >> ( 26 ) );
				a += b;  
				x = index0 + 44;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				d += ( b ^ ( a | (~c) ) ) + x + 0xbd3af235;
				d = ( d << 10 ) | ( d >> 22 );
				d += a;  
				x = index0 + 8;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				c += ( a ^ ( d | (~b) ) ) + x + 0x2ad7d2bb;
				c = ( c << 15 ) | ( c >> 17 );
				c += d;
				x = index0 + 36;
				if( x < message_ptr_size )
				{
					index1 = static_cast< ::rux::uint32>( message_ptr_size ) - x;
					if( index1 >= sizeof( ::rux::uint32 ) )
						x = *(::rux::uint32*)&message_ptr[ x ];
					else if( index1 == 3 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( ( (::rux::uint32)message_ptr[ x + 2 ]) << 16) | ( 0x00000080 << 24 );
					else if( index1 == 2 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( ( (::rux::uint32)message_ptr[ x + 1 ] ) << 8 ) | ( 0x00000080 << 16) | ( 0x00000000 << 24 );
					else if( index1 == 1 )
						x = ( (::rux::uint32)message_ptr[ x ] ) | ( 0x00000080 << 8 ) | ( 0x00000000 << 16 ) | ( 0x00000000 << 24 );
				}
				else if( x == message_ptr_size )
					x = 0x00000080;
				else if( x < message_ptr_size + padding_length )
					x = 0x00000000;
				else 
					x = *(::rux::uint32*)&message_length_ptr[ x - index0 - 56 ];	
				b += ( d ^ ( c | (~a) ) ) + x + 0xeb86d391;
				b = ( b << 21 ) | ( b >> 11 );
				b += c;
				aa += a;
				bb += b;
				cc += c;
				dd += d;
			}		
			number[ i1 ] = (::rux::uint8)( aa & 0xff );
			number[ i1 + 1 ] = (::rux::uint8)( ( aa >> 8 ) & 0xff );
			number[ i1 + 2 ] = (::rux::uint8)( ( aa >> 16 ) & 0xff );
			number[ i1 + 3 ] = (::rux::uint8)( ( aa >> 24 ) & 0xff );
			number[ i1 + 4 ] = (::rux::uint8)( bb & 0xff );
			number[ i1 + 5 ] = (::rux::uint8)( ( bb >> 8 ) & 0xff );
			number[ i1 + 6 ] = (::rux::uint8)( ( bb >> 16 ) & 0xff );
			number[ i1 + 7 ] = (::rux::uint8)( ( bb >> 24 ) & 0xff );
			number[ i1 + 8 ] = (::rux::uint8)( cc & 0xff );
			number[ i1 + 9 ] = (::rux::uint8)( ( cc >> 8 ) & 0xff );
			number[ i1 + 10 ] = (::rux::uint8)( ( cc >> 16 ) & 0xff );
			number[ i1 + 11 ] = (::rux::uint8)( ( cc >> 24 ) & 0xff );
			number[ i1 + 12 ] = (::rux::uint8)( dd & 0xff );
			number[ i1 + 13 ] = (::rux::uint8)( ( dd >> 8 ) & 0xff );
			number[ i1 + 14 ] = (::rux::uint8)( ( dd >> 16 ) & 0xff );
			number[ i1 + 15 ] = (::rux::uint8)( ( dd >> 24 ) & 0xff );
		}
	};
};
