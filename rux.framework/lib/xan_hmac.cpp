#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_hmac.h>
#include <xan_rawdata.h>
#include <xan_sha1.h>
#include <xan_md5.h>
#include <xan_keyvaluepair.h>
namespace rux
{
	namespace cryptography
	{
		void XHMAC::ComputeMD5( const rux::uint8* secret_key_ptr , rux::uint32 secret_key_ptr_length , const rux::uint8* data_ptr , rux::uint32 data_ptr_length , rux::uint8 hmac[ 16 ] )
		{
			rux::uint8 block_ptr[ 64 ] = {0};
			memset( block_ptr , 0 , 64 );
			rux::uint8 index0 = 0;
			if( secret_key_ptr && secret_key_ptr_length > 64 )
				XMD5::Hash( secret_key_ptr , secret_key_ptr_length , block_ptr );
			else if( secret_key_ptr && secret_key_ptr_length > 0 )	
				memcpy( block_ptr , secret_key_ptr , secret_key_ptr_length );
			rux::uint8* s_ptr = alloc_array_macros( rux::uint8 , 64 + data_ptr_length );		
			for( index0 = 0 ; index0 < 64 ; index0++ )	
				s_ptr[ index0 ] = block_ptr[ index0 ] ^ 0x36;	
			if( data_ptr && data_ptr_length > 0 )	
				memcpy( &s_ptr[ 64 ] , data_ptr , data_ptr_length );
			XMD5::Hash( s_ptr , 64 + data_ptr_length , hmac );
			if( 64 + data_ptr_length < 80 )
			{
				rux::engine::free_mem( s_ptr );
				s_ptr = alloc_array_macros( rux::uint8 , 80 );	
			}	
			memcpy( &s_ptr[ 64 ] , hmac , 16 );
			for( index0 = 0 ; index0 < 64 ; index0++ )	
				s_ptr[ index0 ] = block_ptr[ index0 ] ^ 0x5c;	
			XMD5::Hash( s_ptr , 80 , hmac );	
			rux::engine::free_mem( s_ptr );
		};
		void XHMAC::ComputeSHA1( const rux::uint8* secret_key_ptr , rux::uint32 secret_key_ptr_length , const rux::uint8* data_ptr , rux::uint32 data_ptr_length , rux::uint8 hmac[ 20 ] )
		{
			rux::uint8 block_ptr[ 64 ] = {0};
			memset( block_ptr , 0 , 64 );
			rux::uint8 index0 = 0;
			if( secret_key_ptr && secret_key_ptr_length > 64 )
				XSha1::Hash( secret_key_ptr , secret_key_ptr_length , block_ptr );
			else if( secret_key_ptr && secret_key_ptr_length > 0 )
			{		
				memcpy( &block_ptr[ 0 ] , secret_key_ptr , secret_key_ptr_length );
			}
			rux::uint8* s_ptr = alloc_array_macros( rux::uint8 , 64 + data_ptr_length );		
			for( index0 = 0 ; index0 < 64 ; index0++ )	
				s_ptr[ index0 ] = block_ptr[ index0 ] ^ 0x36;	
			if( data_ptr && data_ptr_length > 0 )	
				memcpy( &s_ptr[ 64 ] , data_ptr , data_ptr_length );
			XSha1::Hash( s_ptr , 64 + data_ptr_length , hmac );	
			if( 64 + data_ptr_length < 84 )
			{
				rux::engine::free_mem( s_ptr );
				s_ptr = alloc_array_macros( rux::uint8 , 84 );	
			}
			memcpy( &s_ptr[ 64 ] , hmac , 20 );
			for( index0 = 0 ; index0 < 64 ; index0++ )	
				s_ptr[ index0 ] = block_ptr[ index0 ] ^ 0x5c;	
			XSha1::Hash( s_ptr , 84 , hmac );		
			rux::engine::free_mem( s_ptr );
		};
	};
};