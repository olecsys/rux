#include <xan_rsapublickey.h>
#include <xan_rsa.h>
#include <xan_asn_defines.h>
#include <xan_file.h>
#include <xan_console.h>
#include <xan_sha1.h>
#include <xan_des.h>
#ifdef __WINDOWS__
#define OPENSSL_TEST 0
#endif
#include <openssl/rsa.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/bio.h>
#include <openssl/sha.h>
#include <openssl/des.h>
#if OPENSSL_TEST
#pragma comment( lib , "libeay32.lib" )
#endif
const char* g_current_module_name = "rux.cryptography.test.exe";
dll_internal ::rux::uint8 _rux_is_memory_manager = 0;
namespace rux
{
	dll_internal rux_simple_t _first_function = NULL;
	dll_internal rux_simple_t _last_function = NULL;
	dll_internal rux_simple_t _pre_initialization = NULL;
	dll_internal rux_simple_t _post_deinitialization = NULL;
};
rux::uint32 _test_index = 0;
rux::uint32 _succeeded_tests = 0;
rux::uint32 _failed_tests = 0;
rux::uint8 _pem = 0;
void opennssl_des_cfb_encrypt( char* Key , char *Msg , int size , char* output );
void opennssl_des_ecb_encrypt( DES_cblock Key , DES_cblock Msg , DES_cblock output );
void rsa_test( const rux::XString& modulus_octet_string , const rux::XString& public_exponent_octet_string ,
	const rux::XString& message_octet_string , const rux::XString& seed_octet_string , 
	const rux::XString& encryption_octet_string , const rux::XString& private_exponent_octet_string ,
	const rux::XString& prime_1_octet_string , const rux::XString& prime_2_octet_string , 
	const rux::XString& prime_exponent_1_octet_string , const rux::XString& prime_exponent_2_octet_string , 
	const rux::XString& coefficient_octet_string )
{
	_test_index++;
	rux::XString error;
	rux::XRawData modulus = rux::XRawData::FromOctetString( modulus_octet_string , error );

	rux::XRawData public_exponent = rux::XRawData::FromOctetString( public_exponent_octet_string , error );
	rux::XRawData private_exponent = rux::XRawData::FromOctetString( private_exponent_octet_string , error );	
	rux::XRawData prime1 = rux::XRawData::FromOctetString( prime_1_octet_string , error );
	rux::XRawData prime2 = rux::XRawData::FromOctetString( prime_2_octet_string , error );
	rux::XRawData prime_exponent1 = rux::XRawData::FromOctetString( prime_exponent_1_octet_string , error );
	rux::XRawData prime_exponent2 = rux::XRawData::FromOctetString( prime_exponent_2_octet_string , error );
	rux::XRawData coefficient = rux::XRawData::FromOctetString( coefficient_octet_string , error );
	rux::cryptography::XRSA rsa_provider;
	
	rux::XRawData message = rux::XRawData::FromOctetString( message_octet_string , error );			
#if OPENSSL_TEST			
	rux::XRawData seed = rux::XRawData::FromOctetString( seed_octet_string , error );
#else			
	rsa_provider.set_SeedFromOctetString( seed_octet_string , error );
#endif
	rsa_provider.set_ParametersFromReverseOrder( prime1 , prime2 , modulus , public_exponent , private_exponent , prime_exponent1 , prime_exponent2 , coefficient );
	size_t message_size = message.get_Size();		
#if OPENSSL_TEST
	rsa_provider.set_EncryptionScheme( XEnum_RSA_EncryptionScheme_NoPadding );
	RSA* openssl_rsa = RSA_new();
	if( _pem == 0 )
	{
		openssl_rsa = RSA_new();
		openssl_rsa->n = BN_bin2bn( modulus.get_RawData() , modulus.get_Size() , openssl_rsa->n );
		openssl_rsa->e = BN_bin2bn( public_exponent.get_RawData() , public_exponent.get_Size() , openssl_rsa->e );		
		openssl_rsa->d = BN_bin2bn( private_exponent.get_RawData() , private_exponent.get_Size() , openssl_rsa->d );
	}
	else
	{
		BIO* file_ptr = BIO_new_file( "../addons/private.pem" , "r" );
		if( file_ptr )
		{
			openssl_rsa = PEM_read_bio_RSAPrivateKey( file_ptr , &openssl_rsa , NULL , NULL );
			BIO_free( file_ptr );
		}
	}
	openssl_rsa->flags = RSA_FLAG_NO_BLINDING | RSA_FLAG_NO_CONSTTIME;
	message_size = RSA_size( openssl_rsa );
	char* encrypt = (char*)malloc( message_size );
	char* decrypt = (char*)malloc( message_size );		
	rux::uint8* message_with_seed = rux::engine::alloc_array< rux::uint8 >( message_size );
	message_with_seed[ 0 ] = 0x00;
	message_with_seed[ 1 ] = 0x02;
	size_t seed_size = seed.get_Size();
	if( seed_size == 0 )
	{
		seed_size = message_size - message.get_Size() - 3;	
		memset( &message_with_seed[ 2 ] , 0xaa , seed_size );
	}
	else
		memcpy( &message_with_seed[ 2 ] , seed.get_RawData() , seed_size );
	message_with_seed[ 2 + seed_size ] = 0x00;
	memcpy( &message_with_seed[ 3 + seed_size ] , message.get_RawData() , message.get_Size() );					
	message.set_ByRef( rux::XRawData( message_with_seed , message_size ) );	

	//rux_append_to_file( "../addons/text_to_encode" , message_with_seed , message_size );

	char* err = (char*)malloc( 130 );
	rux::int32 encrypt_len = 0 , decrypt_len = 0;		
	if( ( encrypt_len = RSA_public_encrypt( message_size , message.get_RawData() , (unsigned char*)encrypt , openssl_rsa , RSA_NO_PADDING ) ) == -1 ) 
	{
		ERR_load_crypto_strings();
		ERR_error_string(ERR_get_error(), err);
		fprintf(stderr, "Error encrypting message: %s\n", err);
	}
	else
	{
		if( ( decrypt_len = RSA_private_decrypt( encrypt_len , (rux::uint8*)encrypt , (rux::uint8*)decrypt , openssl_rsa , RSA_NO_PADDING ) ) == -1 ) 
		{
			ERR_load_crypto_strings();
			ERR_error_string(ERR_get_error(), err);
			fprintf(stderr, "Error decrypting message: %s\n", err);
		}
	}
	if( memcmp( decrypt , message_with_seed , message_size ) == 0 )
		printf( "decrypt == message_with_seed\n" );
	else
		printf( "decrypt != message_with_seed\n" );
	rux::engine::free_mem( message_with_seed );
	RSA_free( openssl_rsa );
	free( err );
#endif
	rux::uint8* encrypted_data = NULL;
	rux::uint32 encrypted_size = 0 , encrypted_length = 0;
	rsa_provider.Encrypt( message.get_RawData() , message_size , encrypted_data , encrypted_size , encrypted_length , error );
#if OPENSSL_TEST
	if( encrypted_length != encrypt_len
		|| memcmp( encrypt , encrypted_data , encrypted_length ) != 0 )
	{
		printf( "Test #%u failed!!!\n" , _test_index );
		_failed_tests++;
	}
	else
	{
		rux::uint8* decrypted_data = NULL;
		rux::uint32 decrypted_size = 0 , decrypted_length = 0;
		rsa_provider.Decrypt( encrypted_data , encrypted_length , decrypted_data , decrypted_size , decrypted_length , error ); 
		if( decrypted_length != decrypt_len
			|| memcmp( decrypt , decrypted_data , decrypted_length ) != 0 )
		{
			printf( "Test #%u failed!!!\n" , _test_index );
			_failed_tests++;
		}
		else
		{
			printf( "Test #%u succeeded!!!\n" , _test_index );
			_succeeded_tests++;
		}
	}
	free( decrypt );
	free( encrypt );
#else
	rux::XRawData encryption = rux::XRawData::FromOctetString( encryption_octet_string , error );
	if( encrypted_length != encryption.get_Size()
		|| memcmp( encryption.get_RawData() , encrypted_data , encrypted_length ) != 0 )
	{
		printf( "Test #%u failed!!!\n" , _test_index );
		_failed_tests++;
	}
	else
	{			
		rux::XRawData message = rux::XRawData::FromOctetString( message_octet_string , error );
		rux::uint8* decrypted_data = NULL;
		rux::uint32 decrypted_size = 0 , decrypted_length = 0;
		rsa_provider.Decrypt( encrypted_data , encrypted_length , decrypted_data , decrypted_size , decrypted_length , error ); 
		if( decrypted_length != message.get_Size()
			|| memcmp( message.get_RawData() , decrypted_data , decrypted_length ) != 0 )
		{
			rux::XString t0 ,t1;
			t0.set_ByRef( rux::XRawData::ToHexString( message.get_RawData() , message.get_Size() ) );
			t1.set_ByRef( rux::XRawData::ToHexString( decrypted_data , decrypted_length ) );
			printf( "Test #%u failed!!!\n%s\n\n%s\n\n" , _test_index , t0()->_array_ptr , t1()->_array_ptr );
			_failed_tests++;
		}
		else
		{
			printf( "Test #%u succeeded!!!\n" , _test_index );
			_succeeded_tests++;
		}
	}
#endif
};
void rsa_sign_test( const rux::XString& modulus_octet_string , const rux::XString& public_exponent_octet_string ,
	const rux::XString& message_octet_string , const rux::XString& signature_octet_string , 
	const rux::XString& private_exponent_octet_string ,
	const rux::XString& prime_1_octet_string , const rux::XString& prime_2_octet_string , 
	const rux::XString& prime_exponent_1_octet_string , const rux::XString& prime_exponent_2_octet_string , 
	const rux::XString& coefficient_octet_string )
{
	_test_index++;
	rux::XString error;
	rux::XRawData modulus = rux::XRawData::FromOctetString( modulus_octet_string , error );
	rux::XRawData public_exponent = rux::XRawData::FromOctetString( public_exponent_octet_string , error );
	rux::XRawData private_exponent = rux::XRawData::FromOctetString( private_exponent_octet_string , error );	
	rux::XRawData prime1 = rux::XRawData::FromOctetString( prime_1_octet_string , error );
	rux::XRawData prime2 = rux::XRawData::FromOctetString( prime_2_octet_string , error );
	rux::XRawData prime_exponent1 = rux::XRawData::FromOctetString( prime_exponent_1_octet_string , error );
	rux::XRawData prime_exponent2 = rux::XRawData::FromOctetString( prime_exponent_2_octet_string , error );
	rux::XRawData coefficient = rux::XRawData::FromOctetString( coefficient_octet_string , error );
	rux::cryptography::XRSA rsa_provider;	
	rux::XRawData message = rux::XRawData::FromOctetString( message_octet_string , error );			
	rsa_provider.set_ParametersFromReverseOrder( prime1 , prime2 , modulus , public_exponent , private_exponent , prime_exponent1 , prime_exponent2 , coefficient );
	size_t message_size = message.get_Size();		
#if OPENSSL_TEST	
	RSA* openssl_rsa = RSA_new();
	if( _pem == 0 )
	{
		openssl_rsa = RSA_new();
		openssl_rsa->n = BN_bin2bn( modulus.get_RawData() , modulus.get_Size() , openssl_rsa->n );
		openssl_rsa->e = BN_bin2bn( public_exponent.get_RawData() , public_exponent.get_Size() , openssl_rsa->e );		
		openssl_rsa->d = BN_bin2bn( private_exponent.get_RawData() , private_exponent.get_Size() , openssl_rsa->d );
	}
	else
	{
		BIO* file_ptr = BIO_new_file( "../addons/private.pem" , "r" );
		if( file_ptr )
		{
			openssl_rsa = PEM_read_bio_RSAPrivateKey( file_ptr , &openssl_rsa , NULL , NULL );
			BIO_free( file_ptr );
		}
	}
	openssl_rsa->flags = RSA_PKCS1_PADDING | RSA_FLAG_NO_BLINDING | RSA_FLAG_NO_CONSTTIME;
	message_size = RSA_size( openssl_rsa );
	char* signature = (char*)malloc( message_size );
	unsigned int signature_len = 0;
	char* decrypt = (char*)malloc( message_size );
	char* err = (char*)malloc( 130 );	
	if( RSA_sign( NID_sha1 , message.get_RawData() , message.get_Size() , (unsigned char*)signature , &signature_len , openssl_rsa ) == 0 ) 
	{
		ERR_load_crypto_strings();
		ERR_error_string(ERR_get_error(), err);
		fprintf(stderr, "Error encrypting message: %s\n", err);
	}
	else
	{
		//if( ( decrypt_len = RSA_private_decrypt( encrypt_len , (rux::uint8*)encrypt , (rux::uint8*)decrypt , openssl_rsa , RSA_NO_PADDING ) ) == -1 ) 
		//{
		//	ERR_load_crypto_strings();
		//	ERR_error_string(ERR_get_error(), err);
		//	fprintf(stderr, "Error decrypting message: %s\n", err);
		//}
	}
	/*if( memcmp( decrypt , message_with_seed , message_size ) == 0 )
		printf( "decrypt == message_with_seed\n" );
	else
		printf( "decrypt != message_with_seed\n" );*/
	//free_mem( message_with_seed );
	RSA_free( openssl_rsa );
	free( err );

	rux::XString tt , tt1;
	rux::XRawData signature_raw_data = rux::XRawData::FromOctetString( signature_octet_string , error );
	tt.set_ByRef( rux::XRawData::ToHexString( (rux::uint8*)signature , signature_len ) );
	tt1.set_ByRef( rux::XRawData::ToHexString( (rux::uint8*)signature_raw_data.get_RawData() , signature_raw_data.get_Size() ) );
	printf( "\nopenssl signature = '%s'\n\nreal signature = '%s'\n\n" , tt()->_array_ptr , tt1()->_array_ptr );

#endif
	rux::uint8* signature_data = NULL;
	rux::uint32 signature_size = 0 , signature_length = 0;
	rsa_provider.Sign( message.get_RawData() , message_size , signature_data , signature_size , signature_length , error );
#if OPENSSL_TEST
	if( signature_length != signature_len
		|| memcmp( signature , signature_data , signature_length ) != 0 )
	{
		printf( "Test #%u failed!!!\n" , _test_index );
		_failed_tests++;
	}
	else
	{
		/*rux::uint8* decrypted_data = NULL;
		rux::uint32 decrypted_size = 0 , decrypted_length = 0;
		rsa_provider.Decrypt( encrypted_data , encrypted_length , decrypted_data , decrypted_size , decrypted_length , error ); 
		if( decrypted_length != decrypt_len
			|| memcmp( decrypt , decrypted_data , decrypted_length ) != 0 )
		{
			printf( "Test #%u failed!!!\n" , _test_index );
			_failed_tests++;
		}
		else
		{
			printf( "Test #%u succeeded!!!\n" , _test_index );
			_succeeded_tests++;
		}*/
	}
	free( decrypt );
	free( signature );
#else
	rux::XRawData signature = rux::XRawData::FromOctetString( signature_octet_string , error );
	if( signature_length != signature.get_Size()
		|| memcmp( signature.get_RawData() , signature_data , signature_length ) != 0 )
	{		
		printf( "Test #%u failed!!!\n" , _test_index );
		_failed_tests++;
	}
	else
	{
		if( rsa_provider.Verify( message.get_RawData() , message_size , signature_data , signature_length , error ) == 1 )
		{
			printf( "Test #%u succeeded!!!\n" , _test_index );
			_succeeded_tests++;
		}
		else
		{
			printf( "Test #%u failed!!!\n" , _test_index );
			_failed_tests++;
		}
	}
#endif
};
int main(int argc, char ** args)
{	
	{
		THREAD_REGISTER( "main" );
		if( argc == 2 && strcmp( args[ 1 ] , "--rsa-test" ) == 0 )
		{	
			rux::XString modulus_octet_string , public_exponent_octet_string ,
				message_octet_string , seed_octet_string , encryption_octet_string ,
				prime_1_octet_string , prime_2_octet_string , prime_exponent_1_octet_string , 
				prime_exponent_2_octet_string , coefficient_octet_string , private_exponent_octet_string;
			rux::XString error;
			declare_stack_variable( char , filename , 1024 );
			rux_get_executable_directory( filename );
#ifdef __WINDOWS__
			rux_concatenation( filename , "/../../addons/pkcs1v15crypt-vectors.txt" );
#else
			rux_concatenation( filename , "/../addons/pkcs1v15crypt-vectors.txt" );
#endif			
			rux::io::XFile file( rux::io::XFile::Open( filename , error ) );
			printf( "file = '%s'\n" , file.get_FileName()()->_array_ptr );
			if( error.Length() > 0 )
			{
				printf( "error = '%s'\n" , error()->_array_ptr );
			}
			if( file.Exists() == 1 )
			{
				rux::XString text_all;
				text_all.set_ByRef( file.ReadTextAll( error ) );
				if( error.Length() > 0 )
				{
					printf( "file = '%s' , error = '%s'\n" , file.get_FileName()()->_array_ptr , error()->_array_ptr );
				}
				file.Close();				
				if( error.Length() == 0 )
				{				
					rux::XArray< rux::XString > separators;
					separators.Add( "\n" );
					separators.Add( "\r" );
					separators.Add( "\f" );
					rux::XArray< rux::XString > lines;
					lines.set_ByRef( text_all.Split( separators ) );
					rux::uint8 private_key = 0;
					for( rux::uint32 index0 = 0 ; index0 < lines.Count() ; index0++ )
					{
						if( lines[ index0 ].IndexOf( "# Private key" ) == 0 )
							private_key = 1;
						else if( lines[ index0 ].IndexOf( "# Public key" ) == 0 )
							private_key = 0;
						else if( lines[ index0 ].IndexOf( "# Modulus" ) == 0 )
						{
							modulus_octet_string.Clear();
							index0++;
							while( lines.Count() > index0 
								&& lines[ index0 ].get_UTF8Char( 0 ) != '#' )
							{
								modulus_octet_string += " " + lines[ index0 ];
								index0++;
							}
							index0--;
						}
						else if( lines[ index0 ].IndexOf( "# Public exponent" ) == 0 )
						{
							public_exponent_octet_string.Clear();
							index0++;
							while( lines.Count() > index0 
								&& lines[ index0 ].get_UTF8Char( 0 ) != '#' )
							{
								public_exponent_octet_string += " " + lines[ index0 ];
								index0++;
							}
							index0--;
						}
						else if( lines[ index0 ].IndexOf( "# Prime 1" ) == 0 )
						{
							prime_1_octet_string.Clear();
							index0++;
							while( lines.Count() > index0 
								&& lines[ index0 ].get_UTF8Char( 0 ) != '#' )
							{
								prime_1_octet_string += " " + lines[ index0 ];
								index0++;
							}
							index0--;
						}
						else if( lines[ index0 ].IndexOf( "# Prime 2" ) == 0 )
						{
							prime_2_octet_string.Clear();
							index0++;
							while( lines.Count() > index0 
								&& lines[ index0 ].get_UTF8Char( 0 ) != '#' )
							{
								prime_2_octet_string += " " + lines[ index0 ];
								index0++;
							}
							index0--;
						}
						else if( lines[ index0 ].IndexOf( "# Prime exponent 1" ) == 0 )
						{
							prime_exponent_1_octet_string.Clear();
							index0++;
							while( lines.Count() > index0 
								&& lines[ index0 ].get_UTF8Char( 0 ) != '#' )
							{
								prime_exponent_1_octet_string += " " + lines[ index0 ];
								index0++;
							}
							index0--;
						}
						else if( lines[ index0 ].IndexOf( "# Prime exponent 2" ) == 0 )
						{
							prime_exponent_2_octet_string.Clear();
							index0++;
							while( lines.Count() > index0 
								&& lines[ index0 ].get_UTF8Char( 0 ) != '#' )
							{
								prime_exponent_2_octet_string += " " + lines[ index0 ];
								index0++;
							}
							index0--;
						}
						else if( lines[ index0 ].IndexOf( "# Coefficient" ) == 0 )
						{
							coefficient_octet_string.Clear();
							index0++;
							while( lines.Count() > index0 
								&& lines[ index0 ].get_UTF8Char( 0 ) != '#' )
							{
								coefficient_octet_string += " " + lines[ index0 ];
								index0++;
							}
							index0--;
						} 
						else if( lines[ index0 ].IndexOf( "# Message" ) == 0 )
						{
							message_octet_string.Clear();
							index0++;
							while( lines.Count() > index0 
								&& lines[ index0 ].get_UTF8Char( 0 ) != '#' )
							{
								message_octet_string += " " + lines[ index0 ];
								index0++;
							}
							index0--;
						} 
						else if( lines[ index0 ].IndexOf( "# Seed" ) == 0 )
						{
							seed_octet_string.Clear();
							index0++;
							while( lines.Count() > index0 
								&& lines[ index0 ].get_UTF8Char( 0 ) != '#' )
							{
								seed_octet_string += " " + lines[ index0 ];
								index0++;
							}
							index0--;
						} 
						else if( lines[ index0 ].IndexOf( "# Encryption" ) == 0 )
						{
							encryption_octet_string.Clear();
							index0++;
							while( lines.Count() > index0 
								&& lines[ index0 ].get_UTF8Char( 0 ) != '#' )
							{
								encryption_octet_string += " " + lines[ index0 ];
								index0++;
							}
							index0--;
						} 
						else if( lines[ index0 ].IndexOf( "# Exponent" ) == 0 )
						{
							if( private_key == 1 )
							{
								private_exponent_octet_string.Clear();
								index0++;
								while( lines.Count() > index0 
									&& lines[ index0 ].get_UTF8Char( 0 ) != '#' )
								{
									private_exponent_octet_string += " " + lines[ index0 ];
									index0++;
								}
								index0--;
							}
							else
							{
								public_exponent_octet_string.Clear();
								index0++;
								while( lines.Count() > index0 
									&& lines[ index0 ].get_UTF8Char( 0 ) != '#' )
								{
									public_exponent_octet_string += " " + lines[ index0 ];
									index0++;
								}
								index0--;
							}
						}
						if( message_octet_string.Length() > 0 
							&& encryption_octet_string.Length() > 0 
							&& seed_octet_string.Length() > 0 )
						{
							rsa_test( modulus_octet_string , public_exponent_octet_string , 
								message_octet_string , seed_octet_string , encryption_octet_string ,
								private_exponent_octet_string , prime_1_octet_string , prime_2_octet_string , 
								prime_exponent_1_octet_string , prime_exponent_2_octet_string , 
								coefficient_octet_string );
							message_octet_string.Clear();
							encryption_octet_string.Clear();
							seed_octet_string.Clear();
						}
					}							
				}			
			}				
		}
		else if( argc == 2 && strcmp( args[ 1 ] , "--rsa-sign-test" ) == 0 )
		{	
			rux::XString modulus_octet_string , public_exponent_octet_string ,
				message_octet_string , signature_octet_string ,
				prime_1_octet_string , prime_2_octet_string , prime_exponent_1_octet_string , 
				prime_exponent_2_octet_string , coefficient_octet_string , private_exponent_octet_string;
			rux::XString error;
			declare_stack_variable( char , filename , 1024 );
			rux_get_executable_directory( filename );
#ifdef __WINDOWS__
			rux_concatenation( filename , "/../../addons/pkcs1v15sign-vectors.txt" );
#else
			rux_concatenation( filename , "/../addons/pkcs1v15sign-vectors.txt" );
#endif			
			rux::io::XFile file( rux::io::XFile::Open( filename , error ) );
			printf( "file = '%s'\n" , file.get_FileName()()->_array_ptr );
			if( error.Length() > 0 )
			{
				printf( "error = '%s'\n" , error()->_array_ptr );
			}
			if( file.Exists() == 1 )
			{
				rux::XString text_all;
				text_all.set_ByRef( file.ReadTextAll( error ) );
				if( error.Length() > 0 )
				{
					printf( "file = '%s' , error = '%s'\n" , file.get_FileName()()->_array_ptr , error()->_array_ptr );
				}
				file.Close();				
				if( error.Length() == 0 )
				{				
					rux::XArray< rux::XString > separators;
					separators.Add( "\n" );
					separators.Add( "\r" );
					separators.Add( "\f" );
					rux::XArray< rux::XString > lines;
					lines.set_ByRef( text_all.Split( separators ) );
					rux::uint8 private_key = 0;
					for( rux::uint32 index0 = 0 ; index0 < lines.Count() ; index0++ )
					{
						if( lines[ index0 ].IndexOf( "# Private key" ) == 0 )
							private_key = 1;
						else if( lines[ index0 ].IndexOf( "# Public key" ) == 0 )
							private_key = 0;
						else if( lines[ index0 ].IndexOf( "# Modulus" ) == 0 )
						{
							modulus_octet_string.Clear();
							index0++;
							while( lines.Count() > index0 
								&& lines[ index0 ].get_UTF8Char( 0 ) != '#' )
							{
								modulus_octet_string += " " + lines[ index0 ];
								index0++;
							}
							index0--;
						}
						else if( lines[ index0 ].IndexOf( "# Public exponent" ) == 0 )
						{
							public_exponent_octet_string.Clear();
							index0++;
							while( lines.Count() > index0 
								&& lines[ index0 ].get_UTF8Char( 0 ) != '#' )
							{
								public_exponent_octet_string += " " + lines[ index0 ];
								index0++;
							}
							index0--;
						}
						else if( lines[ index0 ].IndexOf( "# Prime 1" ) == 0 )
						{
							prime_1_octet_string.Clear();
							index0++;
							while( lines.Count() > index0 
								&& lines[ index0 ].get_UTF8Char( 0 ) != '#' )
							{
								prime_1_octet_string += " " + lines[ index0 ];
								index0++;
							}
							index0--;
						}
						else if( lines[ index0 ].IndexOf( "# Prime 2" ) == 0 )
						{
							prime_2_octet_string.Clear();
							index0++;
							while( lines.Count() > index0 
								&& lines[ index0 ].get_UTF8Char( 0 ) != '#' )
							{
								prime_2_octet_string += " " + lines[ index0 ];
								index0++;
							}
							index0--;
						}
						else if( lines[ index0 ].IndexOf( "# Prime exponent 1" ) == 0 )
						{
							prime_exponent_1_octet_string.Clear();
							index0++;
							while( lines.Count() > index0 
								&& lines[ index0 ].get_UTF8Char( 0 ) != '#' )
							{
								prime_exponent_1_octet_string += " " + lines[ index0 ];
								index0++;
							}
							index0--;
						}
						else if( lines[ index0 ].IndexOf( "# Prime exponent 2" ) == 0 )
						{
							prime_exponent_2_octet_string.Clear();
							index0++;
							while( lines.Count() > index0 
								&& lines[ index0 ].get_UTF8Char( 0 ) != '#' )
							{
								prime_exponent_2_octet_string += " " + lines[ index0 ];
								index0++;
							}
							index0--;
						}
						else if( lines[ index0 ].IndexOf( "# Coefficient" ) == 0 )
						{
							coefficient_octet_string.Clear();
							index0++;
							while( lines.Count() > index0 
								&& lines[ index0 ].get_UTF8Char( 0 ) != '#' )
							{
								coefficient_octet_string += " " + lines[ index0 ];
								index0++;
							}
							index0--;
						} 
						else if( lines[ index0 ].IndexOf( "# Message to be signed" ) == 0 )
						{
							message_octet_string.Clear();
							index0++;
							while( lines.Count() > index0 
								&& lines[ index0 ].get_UTF8Char( 0 ) != '#' )
							{
								message_octet_string += " " + lines[ index0 ];
								index0++;
							}
							index0--;
						} 
						else if( lines[ index0 ].IndexOf( "# Signature" ) == 0 )
						{
							signature_octet_string.Clear();
							index0++;
							while( lines.Count() > index0 
								&& lines[ index0 ].get_UTF8Char( 0 ) != '#' )
							{
								signature_octet_string += " " + lines[ index0 ];
								index0++;
							}
							index0--;
						} 						
						else if( lines[ index0 ].IndexOf( "# Exponent" ) == 0 )
						{
							if( private_key == 1 )
							{
								private_exponent_octet_string.Clear();
								index0++;
								while( lines.Count() > index0 
									&& lines[ index0 ].get_UTF8Char( 0 ) != '#' )
								{
									private_exponent_octet_string += " " + lines[ index0 ];
									index0++;
								}
								index0--;
							}
							else
							{
								public_exponent_octet_string.Clear();
								index0++;
								while( lines.Count() > index0 
									&& lines[ index0 ].get_UTF8Char( 0 ) != '#' )
								{
									public_exponent_octet_string += " " + lines[ index0 ];
									index0++;
								}
								index0--;
							}
						}
						if( message_octet_string.Length() > 0
							&& signature_octet_string.Length() > 0 )
						{
							rsa_sign_test( modulus_octet_string , public_exponent_octet_string , 
								message_octet_string , signature_octet_string ,
								private_exponent_octet_string , prime_1_octet_string , prime_2_octet_string , 
								prime_exponent_1_octet_string , prime_exponent_2_octet_string , 
								coefficient_octet_string );
							message_octet_string.Clear();
							signature_octet_string.Clear();
						}
					}							
				}			
			}				
		}
		else if( argc == 2 && strcmp( args[ 1 ] , "--sha1-large-file-test" ) == 0 )
		{
			declare_stack_variable( char , filename , 1024 );
			rux_get_executable_directory( filename );
#ifdef __WINDOWS__
			rux_concatenation( filename , "/../../addons/video-server-7.1.146-x86.exe" );
#else
			rux_concatenation( filename , "/../addons/video-server-7.1.146-x86.exe" );
#endif			
			rux::io::file file( filename );
			if( file.opened() )
			{				
				rux::uint8 checksum[ 20 ] = {0};
				rux::uint64 filesize = file.size();
				rux::uint8* data = (rux::uint8*)::malloc( filesize );
				if( data )
				{
					size_t readen = 0;
					if( file.read_bytes( (char*)data , filesize , readen ) )
					{
#if OPENSSL_TEST
						rux::uint8 openssl_hash[ 20 ] = {0};
						SHA1( data , filesize , openssl_hash );
#endif
						rux::cryptography::XSha1::Hash( data , filesize , checksum );
						_test_index++;
#if OPENSSL_TEST
						if( memcmp( checksum , openssl_hash , 20 ) == 0 )
							printf( "Test #%u succeeded!!!\n" , _test_index );
						else
							printf( "Test #%u failed!!!\n" , _test_index );
#endif
						::free( data );
						rux::cryptography::XSha1::Hash( file , checksum );
						_test_index++;
#if OPENSSL_TEST
						if( memcmp( checksum , openssl_hash , 20 ) == 0 )
							printf( "Test #%u succeeded!!!\n" , _test_index );
						else
							printf( "Test #%u failed!!!\n" , _test_index );
#endif
					}
				}
				file.close();
			}
		}
		else if( argc == 2 && strcmp( args[ 1 ] , "--sha-test" ) == 0 )
		{	
			rux::XString sha1_octet_string , input_message;
			rux::XString error;
			declare_stack_variable( char , filename , 1024 );
			rux_get_executable_directory( filename );
#ifdef __WINDOWS__
			rux_concatenation( filename , "/../../addons/sha-vectors.txt" );
#else
			rux_concatenation( filename , "/../addons/sha-vectors.txt" );
#endif			
			rux::io::file sha_file( filename );
			if( sha_file.opened() )
			{
				rux::uint8 hash0[ 20 ] , hash1[ 20 ];
				rux::uint64 filesize = sha_file.size();
				rux::uint8* message = (rux::uint8*)::malloc( filesize );
				if( message )
				{
					size_t readen = 0;
					if( sha_file.read_bytes( (char*)message , filesize , readen ) 
						&& readen == filesize )
					{
						_test_index++;
						rux::cryptography::XSha1::Hash( message , filesize , hash0 );
						sha_file.close();
						rux::cryptography::XSha1::Hash( filename , hash1 );
						if( memcmp( hash0 , hash1 , 20 ) != 0 )
						{
							printf( "Test #%u failed!!!\n" , _test_index );
							_failed_tests++;
						}
						else
						{
							printf( "Test #%u succeeded!!!\n" , _test_index );
							_succeeded_tests++;
						}
					}
					::free( message );
				}
				sha_file.close();
			}
			rux::io::XFile file( rux::io::XFile::Open( filename , error ) );
			printf( "file = '%s'\n" , file.get_FileName()()->_array_ptr );
			if( error.Length() > 0 )
			{
				printf( "error = '%s'\n" , error()->_array_ptr );
			}
			if( file.Exists() == 1 )
			{
				rux::XString text_all;
				text_all.set_ByRef( file.ReadTextAll( error ) );
				if( error.Length() > 0 )
				{
					printf( "file = '%s' , error = '%s'\n" , file.get_FileName()()->_array_ptr , error()->_array_ptr );
				}
				file.Close();				
				if( error.Length() == 0 )
				{				
					rux::XArray< rux::XString > separators;
					separators.Add( "\n" );
					separators.Add( "\r" );
					separators.Add( "\f" );
					rux::XArray< rux::XString > lines;
					lines.set_ByRef( text_all.Split( separators ) );
					rux::uint8 found = 0;
					for( rux::uint32 index0 = 0 ; index0 < lines.Count() ; index0++ )
					{
						if( lines[ index0 ].IndexOf( "# Input message" ) == 0 )
						{
							index0++;
							input_message = lines[ index0 ].Substring( 1 , lines[ index0 ].Length() - 2 );
							found = 1;
						}
						else if( lines[ index0 ].IndexOf( "# SHA-1" ) == 0 )
						{
							index0++;
							sha1_octet_string = lines[ index0 ];
						}						
						if( found == 1
							&& sha1_octet_string.Length() > 0 )
						{
							_test_index++;
							rux::uint8 hash[ 20 ] = {0};
#if OPENSSL_TEST
							rux::uint8 openssl_hash[ 20 ] = {0};
							SHA1( (rux::uint8*)input_message()->_array_ptr , input_message.Length(), openssl_hash );
#endif
							rux::cryptography::XSha1::Hash( (rux::uint8*)input_message()->_array_ptr , input_message.Length() , hash );							
							rux::XRawData sha1_hash( rux::XRawData::FromOctetString( sha1_octet_string , error ) );
							if( memcmp( sha1_hash.get_RawData() , hash , 20 ) != 0 )
							{								
								rux::cryptography::XSha1::Hash( (rux::uint8*)input_message()->_array_ptr , input_message.Length() , hash );							

								printf( "Test #%u failed!!!\n" , _test_index );
								_failed_tests++;
							}
							else
							{
								printf( "Test #%u succeeded!!!\n" , _test_index );
								_succeeded_tests++;
							}
							found = 0;
							input_message.Clear();
							sha1_octet_string.Clear();
						}
					}							
				}			
			}				
		}
		if( argc == 2 && strcmp( args[ 1 ] , "--rsa-from-pem-test" ) == 0 )
		{	
			rux::XString modulus_octet_string , public_exponent_octet_string ,
				message_octet_string , seed_octet_string , encryption_octet_string ,
				prime_1_octet_string , prime_2_octet_string , prime_exponent_1_octet_string , 
				prime_exponent_2_octet_string , coefficient_octet_string , private_exponent_octet_string;
			rux::XString error;
			rux::cryptography::XRSA rsa_provider;
			rux::cryptography::XRSA::FromPEMFile( rsa_provider , "../addons/public.pem" , error );
			rux::cryptography::XRSA::FromPEMFile( rsa_provider , "../addons/private.pem" , error );

			const char* text = "Hello , people!!!";
			modulus_octet_string.set_ByRef( rsa_provider.get_ModulusAsReverseOctetString() );
			public_exponent_octet_string.set_ByRef( rsa_provider.get_PublicExponentAsReverseOctetString() );
			private_exponent_octet_string.set_ByRef( rsa_provider.get_PrivateExponentAsReverseOctetString() );		
			prime_1_octet_string.set_ByRef( rsa_provider.get_Prime1AsReverseOctetString() );
			prime_2_octet_string.set_ByRef( rsa_provider.get_Prime2AsReverseOctetString() );
			prime_exponent_1_octet_string.set_ByRef( rsa_provider.get_PrimeExponent1AsReverseOctetString() );
			prime_exponent_2_octet_string.set_ByRef( rsa_provider.get_PrimeExponent2AsReverseOctetString() );
			coefficient_octet_string.set_ByRef( rsa_provider.get_CoefficientAsReverseOctetString() );
			message_octet_string.set_ByRef( rux::XRawData::ToReverseOctetString( (rux::uint8*)text , strlen( text ) ) );
		
			rsa_test( modulus_octet_string , public_exponent_octet_string , 
				message_octet_string , "" , "" , private_exponent_octet_string , 
				prime_1_octet_string , prime_2_octet_string , prime_exponent_1_octet_string , 
				prime_exponent_2_octet_string , coefficient_octet_string );
				
			rux::uint8* encrypted_data = NULL;
			rux::uint32 encrypted_size = 0 , encrypted_length = 0;
			rsa_provider.Encrypt( (rux::uint8*)text , strlen( text ) , encrypted_data , encrypted_size , encrypted_length , error );		
			rux::uint8* decrypted_data = NULL;
			rux::uint32 decrypted_size = 0 , decrypted_length = 0;
			rsa_provider.Decrypt( encrypted_data , encrypted_length , decrypted_data , decrypted_size , decrypted_length , error );
		}
		else if( argc == 2 && strcmp( args[ 1 ] , "--aes-test" ) == 0 )
		{
		}
		else if( argc == 2 && strcmp( args[ 1 ] , "--des-test" ) == 0 )
		{
			declare_stack_variable( char , exec_dir , 1024 );
			rux_get_executable_directory( exec_dir );
#ifdef __WINDOWS__
#ifdef __x86__
			rux_concatenation( exec_dir , "/../../extern_lib/windows/win32" );
#elif defined( __x64__ )
			rux_concatenation( exec_dir , "/../../extern_lib/windows/win64" );
#endif
#else
#ifdef __x86__
			rux_concatenation( exec_dir , "/../extern_lib/linux/x86" );
#elif defined( __x64__ )
			rux_concatenation( exec_dir , "/../extern_lib/linux/x64" );
#endif
#endif
			rux_add_search_directory( exec_dir );

			rux::cryptography::XDES des_provider;
			rux::uint8 key[ 8 ] = { 0 };
			//rux::int64 ansi_test_key = 0x0123456789abcdef;
			rux::int64 ansi_test_key = 0xefcdab8967452301;
			memcpy( key , "12345678" , 8 );		
			memcpy( key , &ansi_test_key , 8 );
			des_provider.set_CipherKey( (rux::uint8*)&key );
			rux::uint8* output = NULL;
			rux::uint32 output_size = 0 , output_length = 0;
			des_provider.Encrypt( (rux::uint8*)"Now is the time for all" , strlen( "Now is the time for all" ) , output , output_size , output_length );
			if( output )
			{
				_test_index++;
				rux::uint8 res[] = 
				{
					0x3f , 0xa4 , 0x0e , 0x8a , 0x98 , 0x4d , 0x48 , 0x15 , 0x6a , 0x27 , 0x17 , 
					0x87 , 0xab , 0x88 , 0x83 , 0xf9 , 0xb1 , 0xcb , 0xc8 , 0x07 , 0x56 , 0x55 , 0x70 , 0x58
				};
				if( output_length != sizeof( res )
					|| memcmp( output , res , sizeof( res ) ) != 0 )
				{
					printf( "Test #%u failed!!!\n" , _test_index );
					_failed_tests++;
				}
				else
				{
					printf( "Test #%u succeeded!!!\nResult:\n" , _test_index );					
					for( size_t index0 = 0 ; index0 < output_length ; index0++ )
						printf( "%02x" , (rux::int32)output[ index0 ] );
					printf( "%s" , "\n\n" );
					_succeeded_tests++;
				}
			}
			memcpy( key , "12345678" , 8 );
			des_provider.set_CipherKey( (rux::uint8*)&key );
			des_provider.Encrypt( (rux::uint8*)"It's super thi" , strlen( "It's super thi" ) , output , output_size , output_length );
			if( output )
			{
				_test_index++;
				rux::uint8 res[] = 
				{
					0xab , 0x9d , 0x21 , 0x16 , 0x1d , 0x00 , 0xd9 , 0x2e , 0xed , 0xb0 , 0xb6 , 0xbb , 
					0x54 , 0x16 , 0x92 , 0x6b
				};
				if( output_length != sizeof( res )
					|| memcmp( output , res , sizeof( res ) ) != 0 )
				{
					printf( "Test #%u failed!!!\n" , _test_index );
					_failed_tests++;
				}
				else
				{
					printf( "Test #%u succeeded!!!\nResult:\n" , _test_index );					
					for( size_t index0 = 0 ; index0 < output_length ; index0++ )
						printf( "%02x" , (rux::int32)output[ index0 ] );
					printf( "%s" , "\n\n" );
					_succeeded_tests++;
				}				
			}
			if( output )
				rux::engine::free_mem( output );
		}
		printf( "\nSucceeded tests : %u\nFailed tests : %u\n\n" , _succeeded_tests , _failed_tests );
		printf( "Press <ENTER>" , _succeeded_tests , _failed_tests );
		rux::XConsole console;
		console.Read();
	}	
	rux::deinitialize();
	return 0;
};
void opennssl_des_ecb_encrypt( DES_cblock Key , DES_cblock Msg , DES_cblock output )
{ 
#if OPENSSL_TEST
	DES_key_schedule key;
    ERR_clear_error();
    if (DES_set_key((DES_cblock*)Key, &key) >= 0)
	{
		DES_ecb_encrypt( (DES_cblock*)Msg, (DES_cblock*)output , &key, 1);
		
		//DES_ecb_encrypt( &ciphertext, &buf, &key, 0);
	}
#endif
};
void opennssl_des_cfb_encrypt( char* Key , char *Msg , int size , char* output )
{ 
#if OPENSSL_TEST
        int             n=0;
        DES_cblock      Key2;
        DES_key_schedule schedule; 
        /* Prepare the key for use with DES_cfb64_encrypt */
        memcpy( Key2, Key,8);
        DES_set_odd_parity( &Key2 );
        DES_set_key_checked( &Key2, &schedule ); 
        /* Encryption occurs here */		
        DES_cfb64_encrypt( ( unsigned char * ) Msg, ( unsigned char * ) output,
                          size, &schedule, &Key2, &n, DES_ENCRYPT );
#endif
}
