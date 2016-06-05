#ifndef XAN_DES_H
#define XAN_DES_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_aes.h>
#include <xan_cryptography.h>
namespace rux
{	
	namespace cryptography
	{			
		begin_declare_rux_class( DES );
			void Encrypt( rux::uint8 input[ 8 ] , rux::uint8 output[ 8 ] );
			void Decrypt( rux::uint8 input[ 8 ] , rux::uint8 output[ 8 ] );
			void Encrypt( rux::uint8* input_ptr , rux::uint32 input_ptr_length , rux::uint8*& output_ptr , rux::uint32& output_ptr_size , rux::uint32& output_ptr_length );
			void Decrypt( rux::uint8* input_ptr , rux::uint32 input_ptr_length , rux::uint8*& output_ptr , rux::uint32& output_ptr_size , rux::uint32& output_ptr_length );
			rux::uint8 set_CipherKey( const rux::uint8 key[ 8 ] );
		begin_declare_rux_class_members( DES );
			XEnum_SymmetricCryptAlgorithm_Padding _padding;	
			XEnum_SymmetricCryptAlgorithm_CipherMode _cipher_mode;
			DES_key_schedule _key_schedule;
		end_declare_rux_class_members( DES );
			void Encrypt( rux::uint8 input[ 8 ] , rux::uint8 output[ 8 ] );
			void Decrypt( rux::uint8 input[ 8 ] , rux::uint8 output[ 8 ] );
			void Encrypt( rux::uint8* input_ptr , rux::uint32 input_ptr_length , rux::uint8*& output_ptr , rux::uint32& output_ptr_size , rux::uint32& output_ptr_length );
			void Decrypt( rux::uint8* input_ptr , rux::uint32 input_ptr_length , rux::uint8*& output_ptr , rux::uint32& output_ptr_size , rux::uint32& output_ptr_length );
			void set_CipherMode( XEnum_SymmetricCryptAlgorithm_CipherMode cipher_mode );
			rux::uint8 set_CipherKey( const rux::uint8 key[ 8 ] );
			static void make_key( rux::uint8 key[ 7 ] , rux::uint8 des_key[ 8 ] );
			static rux::uint8 get_7bits( rux::uint8* input , rux::int32 start_bit );
		end_declare_rux_class();
	};
};
#endif
