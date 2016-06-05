#ifndef XAN_AES_H
#define XAN_AES_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_string.h>
enum XEnum_AES_KeyType
{
	XEnum_AES_KeyType_128bits , 
	XEnum_AES_KeyType_192bits ,
	XEnum_AES_KeyType_256bits
};
enum XEnum_SymmetricCryptAlgorithm_Padding
{
	XEnum_SymmetricCryptAlgorithm_Padding_Zeroes , 
	XEnum_SymmetricCryptAlgorithm_Padding_PKCS7
};
enum XEnum_SymmetricCryptAlgorithm_CipherMode
{
	XEnum_SymmetricCryptAlgorithm_CipherMode_ElectronicCodebook , 
	XEnum_SymmetricCryptAlgorithm_CipherMode_BlockChaining
};
namespace rux
{	
	namespace cryptography
	{			
		begin_declare_rux_class( AES );
			void Encrypt( rux::uint8 input[ 16 ] , rux::uint8 output[ 16 ] );
			void Decrypt( rux::uint8 input[ 16 ] , rux::uint8 output[ 16 ] );
			void Encrypt( rux::uint8* input_ptr , rux::uint32 input_ptr_length , rux::uint8*& output_ptr , rux::uint32& output_ptr_size , rux::uint32& output_ptr_length );
			void Decrypt( rux::uint8* input_ptr , rux::uint32 input_ptr_length , rux::uint8*& output_ptr , rux::uint32& output_ptr_size , rux::uint32& output_ptr_length );
			void set_InitializationVector( const rux::uint8 vector[ 16 ] );
			void set_CipherMode( XEnum_SymmetricCryptAlgorithm_CipherMode cipher_mode );
			void set_CipherKey( const rux::uint8 key[ 32 ] , XEnum_AES_KeyType key_type );
		begin_declare_rux_class_members( AES );
			rux::uint32 _encryption_round_key[ 64 ];
			rux::uint32 _decryption_round_key[ 64 ];
			XEnum_AES_KeyType _key_type;
			rux::uint32 _round_count;
			rux::uint8 _key[ 32 ];
			rux::uint8 _initialization_vector[ 16 ];
			rux::uint8 _encryption_feedback[ 16 ];
			rux::uint8 _decryption_feedback[ 16 ];
			XEnum_SymmetricCryptAlgorithm_Padding _padding;
			XEnum_SymmetricCryptAlgorithm_CipherMode _cipher_mode;
		end_declare_rux_class_members( AES );
			void Encrypt( rux::uint8 input[ 16 ] , rux::uint8 output[ 16 ] );
			void Decrypt( rux::uint8 input[ 16 ] , rux::uint8 output[ 16 ] );
			void Encrypt( rux::uint8* input_ptr , rux::uint32 input_ptr_length , rux::uint8*& output_ptr , rux::uint32& output_ptr_size , rux::uint32& output_ptr_length );
			void Decrypt( rux::uint8* input_ptr , rux::uint32 input_ptr_length , rux::uint8*& output_ptr , rux::uint32& output_ptr_size , rux::uint32& output_ptr_length );
			void set_InitializationVector( const rux::uint8 vector[ 16 ] );
			void set_CipherMode( XEnum_SymmetricCryptAlgorithm_CipherMode cipher_mode );
			void set_CipherKey( const rux::uint8 key[ 32 ] , XEnum_AES_KeyType key_type );
		end_declare_rux_class();
	};
};
#endif
