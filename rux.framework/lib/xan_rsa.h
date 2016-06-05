#ifndef XAN_RSA_H
#define XAN_RSA_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_rawdata.h>
#include <xan_randomnumbergenerator.h>
#include <xan_rsapublickey.h>
#include <xan_io_defines.h>
enum XEnum_Hash_Function
{
	XEnum_Hash_Function_sha1 , 
	XEnum_Hash_Function_sha256 ,
	XEnum_Hash_Function_sha384 ,
	XEnum_Hash_Function_sha512 ,
	XEnum_Hash_Function_md2 ,
	XEnum_Hash_Function_md5
};
enum XEnum_RSA_EncryptionScheme
{	
	XEnum_RSA_EncryptionScheme_PKCS1v15 , 
	XEnum_RSA_EncryptionScheme_NoPadding
};
namespace rux
{	
	namespace cryptography
	{			
		begin_declare_rux_class_with_namespace( RSA , rux::cryptography );
			~RSA();
			void Encrypt( rux::uint8* input_ptr , rux::uint32 input_ptr_length , rux::uint8*& output_ptr , rux::uint32& output_ptr_size , rux::uint32& output_ptr_length , rux::XString& error );
			void Decrypt( rux::uint8* input_ptr , rux::uint32 input_ptr_length , rux::uint8*& output_ptr , rux::uint32& output_ptr_size , rux::uint32& output_ptr_length , rux::XString& error );
			void Sign( rux::uint8* input_ptr , rux::uint32 input_ptr_length , rux::uint8*& output_ptr , rux::uint32& output_ptr_size , rux::uint32& output_ptr_length , rux::XString& error );
			rux::uint8 Verify( rux::uint8* input_ptr , rux::uint32 input_ptr_length , rux::uint8* signature , size_t signature_size , rux::XString& error );
			rux::uint8 Verify( rux::io::file& signed_file , rux::uint8* signature , size_t signature_size , rux::XString& error );
			rux::uint8 Verify( rux::io::file& signed_file , rux::io::file& signature_file , rux::XString& error );
			static void RSAEP( rux::XRawData& message , rux::XRawData& modulus , rux::XRawData& public_exponent , rux::XString& error );	
			static void RSADP( rux::XRawData& message , rux::XRawData& modulus , rux::XRawData& private_exponent , rux::XString& error );
			static void RSAVP1( rux::XRawData& signature , rux::XRawData& modulus , rux::XRawData& private_exponent , rux::XString& error );
			static void RSASP1( rux::XRawData& message , rux::XRawData& modulus , rux::XRawData& private_exponent , rux::XString& error );
			void EME_PKCS1_v1_5_encoding( rux::uint8* input_ptr , size_t input_ptr_length , size_t input_position , size_t& block_size );
			void EMSA_PKCS1_v1_5_encoding( rux::uint8* input_ptr , size_t input_ptr_length , size_t k , rux::XString& error );
			void EMSA_PKCS1_v1_5_encoding( rux::io::file& signed_file , size_t k , rux::XString& error );
			void No_padding_encoding( rux::uint8* input_ptr , size_t input_ptr_length , size_t input_position , size_t& block_size );
			void set_Parameters( const rux::XRawData& prime1 , const rux::XRawData& prime2 , const rux::XRawData& modulus , const rux::XRawData& public_exponent , const rux::XRawData& private_exponent , const rux::XRawData& prime_exponent1 , const rux::XRawData&  prime_exponent2 , const rux::XRawData& coefficient );
			void set_ParametersFromReverseOrder( rux::XRawData& prime1 , rux::XRawData& prime2 , rux::XRawData& modulus , rux::XRawData& public_exponent , rux::XRawData& private_exponent , rux::XRawData& prime_exponent1 , rux::XRawData&  prime_exponent2 , rux::XRawData& coefficient );	
		begin_declare_rux_class_members( RSA );
			XEnum_Hash_Function _hash_function;
			XEnum_RSA_EncryptionScheme _encryption_scheme;
			rux::XRawData _prime1;
			rux::XRawData _prime2;
			rux::XRawData _modulus;
			rux::XRawData _public_exponent;
			rux::XRawData _private_exponent;
			rux::XRawData _prime_exponent1;
			rux::XRawData _prime_exponent2;
			rux::XRawData _inverse_prime;
			rux::XRandomNumberGenerator _random;
			rux::XRawData _seed;
			rux::uint8* _EM;
			size_t _EM_size;
		end_declare_rux_class_members( RSA );
			void Encrypt( rux::uint8* input_ptr , rux::uint32 input_ptr_length , rux::uint8*& output_ptr , rux::uint32& output_ptr_size , rux::uint32& output_ptr_length , rux::XString& error );
			void Decrypt( rux::uint8* input_ptr , rux::uint32 input_ptr_length , rux::uint8*& output_ptr , rux::uint32& output_ptr_size , rux::uint32& output_ptr_length , rux::XString& error );
			void set_Parameters( const rux::XRawData& prime1 , const rux::XRawData& prime2 , const rux::XRawData& public_exponent , rux::XString& error );
			void set_Seed( const rux::XString& seed , rux::XString& error );
			void set_SeedFromOctetString( const rux::XString& seed , rux::XString& error );
			void set_PublicKey( XRsaPublicKey& rsa_public_key );
			void set_Parameters( const rux::XRawData& prime1 , const rux::XRawData& prime2 , const rux::XRawData& modulus , const rux::XRawData& public_exponent , const rux::XRawData& private_exponent , const rux::XRawData& prime_exponent1 , const rux::XRawData&  prime_exponent2 , const rux::XRawData& coefficient );
			void set_EncryptionScheme( XEnum_RSA_EncryptionScheme encryption_scheme );
			static void FromPEMFile( XRSA& rsa , const rux::XString& pem_filename , rux::XString& error );	
			rux::String& get_ModulusAsOctetString( void );
			rux::String& get_PublicExponentAsOctetString( void );
			rux::String& get_PrivateExponentAsOctetString( void );
			rux::String& get_Prime1AsOctetString( void );
			rux::String& get_Prime2AsOctetString( void );
			rux::String& get_PrimeExponent1AsOctetString( void );
			rux::String& get_PrimeExponent2AsOctetString( void );
			rux::String& get_CoefficientAsOctetString( void );
			rux::String& get_ModulusAsReverseOctetString( void );
			rux::String& get_PublicExponentAsReverseOctetString( void );
			rux::String& get_PrivateExponentAsReverseOctetString( void );
			rux::String& get_Prime1AsReverseOctetString( void );
			rux::String& get_Prime2AsReverseOctetString( void );
			rux::String& get_PrimeExponent1AsReverseOctetString( void );
			rux::String& get_PrimeExponent2AsReverseOctetString( void );
			rux::String& get_CoefficientAsReverseOctetString( void );
			void set_ParametersFromReverseOrder( rux::XRawData& prime1 , rux::XRawData& prime2 , rux::XRawData& modulus , rux::XRawData& public_exponent , rux::XRawData& private_exponent , rux::XRawData& prime_exponent1 , rux::XRawData&  prime_exponent2 , rux::XRawData& coefficient );
			void Sign( rux::uint8* input_ptr , rux::uint32 input_ptr_length , rux::uint8*& output_ptr , rux::uint32& output_ptr_size , rux::uint32& output_ptr_length , rux::XString& error );
			rux::uint8 Verify( rux::uint8* input_ptr , rux::uint32 input_ptr_length , rux::uint8* signature , size_t signature_size , rux::XString& error );
			rux::uint8 Verify( rux::io::file& signed_file , rux::uint8* signature , size_t signature_size , rux::XString& error );
			rux::uint8 Verify( rux::io::file& signed_file , rux::io::file& signature_file , rux::XString& error );
			static rux::uint8 Verify( const rux::XString& signed_filename , const rux::XString& digest_filename , const rux::XString& pem_filename , rux::XString& error );
		end_declare_rux_class();
	};
};
#endif
