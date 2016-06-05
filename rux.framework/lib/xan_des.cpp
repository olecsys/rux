#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_des.h>
#include <xan_keyvaluepair.h>
#include <xan_uint64.h>
#include <xan_boolean.h>
begin_implement_rux_class_ns( DES , rux::cryptography );
	rux::cryptography::initialize();
	_padding = XEnum_SymmetricCryptAlgorithm_Padding_Zeroes;	
	_cipher_mode = XEnum_SymmetricCryptAlgorithm_CipherMode_ElectronicCodebook;
end_implement_rux_class();
namespace rux
{
	namespace cryptography
	{
		rux::uint8 XDES::get_7bits( rux::uint8* input , rux::int32 start_bit )
		{
			rux::int32 word  = (unsigned)input[ start_bit / 8 ] << 8;
			word |= (unsigned)input[ start_bit / 8 + 1 ];
			word >>= 15 - ( start_bit % 8 + 7 );
			return word & 0xFE;
		}
		void XDES::make_key( rux::uint8 key[ 7 ] , rux::uint8 des_key[ 8 ] )
		{
			des_key[ 0 ] = get_7bits( key , 0 );
			des_key[ 1 ] = get_7bits( key , 7 );
			des_key[ 2 ] = get_7bits( key , 14 );
			des_key[ 3 ] = get_7bits( key , 21 );
			des_key[ 4 ] = get_7bits( key , 28 );
			des_key[ 5 ] = get_7bits( key , 35 );
			des_key[ 6 ] = get_7bits( key , 42 );
			des_key[ 7 ] = get_7bits( key , 49 );
			rux::cryptography::initialize();
			if( (*rux::cryptography::_cryptography)()->_DES_set_odd_parity )
				(*rux::cryptography::_cryptography)()->_DES_set_odd_parity( (des_cblock*)des_key );
		}
		void XDES::Encrypt( rux::uint8* input_ptr , rux::uint32 input_ptr_length , rux::uint8*& output_ptr , rux::uint32& output_ptr_size , rux::uint32& output_ptr_length )
		{
			(*this)()->Encrypt( input_ptr , input_ptr_length , output_ptr , output_ptr_size , output_ptr_length );
		}
		void DES::Encrypt( rux::uint8* input_ptr , rux::uint32 input_ptr_length , rux::uint8*& output_ptr , rux::uint32& output_ptr_size , rux::uint32& output_ptr_length )
		{	
			rux::uint32 last_block_length = input_ptr_length % 8;
			if( input_ptr_length == 0 || last_block_length > 0 )
				output_ptr_length = input_ptr_length + ( 8 - last_block_length );
			else
				output_ptr_length = input_ptr_length;
			if( output_ptr_length > output_ptr_size )
			{
				output_ptr_size = output_ptr_length;
				if( output_ptr )
					rux::engine::free_mem( output_ptr );
				output_ptr = alloc_array_macros( rux::uint8 , output_ptr_length );
			}
			if( _cipher_mode == XEnum_SymmetricCryptAlgorithm_CipherMode_ElectronicCodebook )
			{
				rux::uint32 size = 0;
				for( size_t index1 = 0 ; index1 < output_ptr_length ; index1 += 8 )
				{
					if( ( size = input_ptr_length - index1 ) >= 8 )
						Encrypt( &input_ptr[ index1 ] , &output_ptr[ index1 ] );
					else
					{
						rux::uint8 input[ 8 ] = {0};
						if( _padding == XEnum_SymmetricCryptAlgorithm_Padding_Zeroes )
							memset( input , 0 , 8 );
						else if( _padding == XEnum_SymmetricCryptAlgorithm_Padding_PKCS7 )
							memset( input , 8 - size , 8 );
						memcpy( input , &input_ptr[ index1 ] , size );
						Encrypt( input , &output_ptr[ index1 ] );
						break;
					}
				}
			}
			else if( _cipher_mode == XEnum_SymmetricCryptAlgorithm_CipherMode_BlockChaining )
			{
			}
		};
		void XDES::Decrypt( rux::uint8* input_ptr , rux::uint32 input_ptr_length , rux::uint8*& output_ptr , rux::uint32& output_ptr_size , rux::uint32& output_ptr_length )
		{
			(*this)()->Decrypt( input_ptr , input_ptr_length , output_ptr , output_ptr_size , output_ptr_length );
		}
		void DES::Decrypt( rux::uint8* input_ptr , rux::uint32 input_ptr_length , rux::uint8*& output_ptr , rux::uint32& output_ptr_size , rux::uint32& output_ptr_length )
		{
			rux::uint32 last_block_length = input_ptr_length % 16;
			if( input_ptr_length == 0 || last_block_length > 0 )
				output_ptr_length = input_ptr_length + ( 16 - last_block_length );
			else
				output_ptr_length = input_ptr_length;
			if( output_ptr_length > output_ptr_size )
			{
				output_ptr_size = output_ptr_length;
				if( output_ptr )
					rux::engine::free_mem( output_ptr );
				output_ptr = alloc_array_macros( rux::uint8 , output_ptr_length );
			}
			if( _cipher_mode == XEnum_SymmetricCryptAlgorithm_CipherMode_ElectronicCodebook )
			{
				for( size_t index1 = 0 ; index1 < output_ptr_length ; index1 += 8 )
				{
					if( input_ptr_length - index1 > 8 )
						Decrypt( &input_ptr[ index1 ] , &output_ptr[ index1 ] );
					else
					{
						rux::uint8 input[ 8 ] = {0};
						memset( input , 0 , 8 );
						memcpy( input , &input_ptr[ index1 ] , input_ptr_length - index1 );
						Decrypt( input , &output_ptr[ index1 ] );
					}
				}
			}
			else if( _cipher_mode == XEnum_SymmetricCryptAlgorithm_CipherMode_BlockChaining )
			{
			}
		};
		void XDES::Encrypt( rux::uint8 input[ 8 ] , rux::uint8 output[ 8 ] )
		{
			(*this)()->Encrypt( input , output );
		};
		void DES::Encrypt( rux::uint8 input[ 8 ] , rux::uint8 output[ 8 ] )
		{	
			if( (*rux::cryptography::_cryptography)()->_DES_set_key 
				&& (*rux::cryptography::_cryptography)()->_ERR_clear_error 
				&&	(*rux::cryptography::_cryptography)()->_DES_ecb_encrypt )
				(*rux::cryptography::_cryptography)()->_DES_ecb_encrypt( (DES_cblock*)input , (DES_cblock*)output , &_key_schedule , 1 );
		};
		void XDES::Decrypt( rux::uint8 input[ 8 ] , rux::uint8 output[ 8 ] )
		{	
			(*this)()->Decrypt( input , output );
		};
		void DES::Decrypt( rux::uint8 input[ 8 ] , rux::uint8 output[ 8 ] )
		{	
			if( (*rux::cryptography::_cryptography)()->_DES_set_key 
				&& (*rux::cryptography::_cryptography)()->_ERR_clear_error 
				&&	(*rux::cryptography::_cryptography)()->_DES_ecb_encrypt )
				(*rux::cryptography::_cryptography)()->_DES_ecb_encrypt( (DES_cblock*)input , (DES_cblock*)output , &_key_schedule , 0 );
		};
		void XDES::set_CipherMode( XEnum_SymmetricCryptAlgorithm_CipherMode cipher_mode )
		{
			(*this)()->_cipher_mode = cipher_mode;
		};
		rux::uint8 XDES::set_CipherKey( const rux::uint8 key[ 8 ] )
		{
			return (*this)()->set_CipherKey( key );
		};
		rux::uint8 DES::set_CipherKey( const rux::uint8 key[ 8 ] )
		{	
			if( (*rux::cryptography::_cryptography)()->_DES_set_key
				&& (*rux::cryptography::_cryptography)()->_ERR_clear_error 
				&&	(*rux::cryptography::_cryptography)()->_DES_ecb_encrypt )
			{		
				(*rux::cryptography::_cryptography)()->_ERR_clear_error();
				if( (*rux::cryptography::_cryptography)()->_DES_set_key( (DES_cblock*)key , &_key_schedule ) >= 0 )
					return 1;
				else
					return 0;
			}
			return 0;
		};
	};
};