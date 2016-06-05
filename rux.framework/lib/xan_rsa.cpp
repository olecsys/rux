#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_rsa.h>
#include <xan_keyvaluepair.h>
#include <xan_file.h>
#include <xan_asn_defines.h>
#include <xan_base64.h>
#include <xan_error.h>
#include <xan_sha1.h>
#define RUX_RSA_PADDING 1
#include <xan_uint64.h>
#include <xan_boolean.h>
begin_implement_rux_class_ns( RSA , rux::cryptography );
	_encryption_scheme = XEnum_RSA_EncryptionScheme_PKCS1v15;
	_hash_function = XEnum_Hash_Function_sha1;
	_EM = NULL;
	_EM_size = 0;
end_implement_rux_class();
namespace rux
{
	namespace cryptography
	{
		RSA::~RSA()
		{
			if( _EM )
				rux::engine::free_mem( _EM );
		};
		void XRSA::set_PublicKey( XRsaPublicKey& rsa_public_key )
		{
			(*this)()->_public_exponent.Copy( rsa_public_key()->_public_exponent );	
			(*this)()->_modulus.Copy( rsa_public_key()->_modulus );		
			(*this)()->_modulus.ResetForceSize();
		};
		void XRSA::set_Parameters( const rux::XRawData& prime1 , const rux::XRawData& prime2 , const rux::XRawData& public_exponent , rux::XString& error )
		{
			//if( prime1.get_IsPrimeNumber() == true )
			{
				//if( prime2.get_IsPrimeNumber() == true )
				{
					rux::XRawData temp = ( prime1 - 1 ) * ( prime2 - 1 );
					if( rux::XRawData::GCD( temp , public_exponent ) == 1 )
					{
						(*this)()->_prime1.Copy( prime1 );
						(*this)()->_prime2.Copy( prime2 );
						(*this)()->_public_exponent.Copy( public_exponent );
						(*this)()->_modulus = prime1 * prime2;
						(*this)()->_modulus.ResetForceSize();
						(*this)()->_private_exponent = public_exponent.ModularInverse( temp );
						(*this)()->_inverse_prime = prime1.ModularInverse( prime2 );
						(*this)()->_prime_exponent1 = (*this)()->_private_exponent % ( prime1 - 1 );
						(*this)()->_prime_exponent2 = (*this)()->_private_exponent % ( prime2 - 1 );
					}
					else
						error = "exponent and ( prime1 - 1 ) * ( prime2 - 1 ) are not coprime";
				}
				//else
				//	error = "prime2 is not prime number";
			}
			//else 
			//	error = "prime1 is not prime number";
		};
		void XRSA::set_Seed( const rux::XString& seed , rux::XString& error )
		{
			(*this)()->_seed = rux::XRawData::OS2IP( seed , error );
			if( error.Length() > 0 )
				(*this)()->_seed = (::rux::uint32)0;
		};
		void XRSA::set_SeedFromOctetString( const rux::XString& seed , rux::XString& error )
		{
			(*this)()->_seed = rux::XRawData::FromOctetString( seed , error );
			if( error.Length() > 0 )
				(*this)()->_seed = (::rux::uint32)0;
		};
		void RSA::No_padding_encoding( rux::uint8* input_ptr , size_t input_ptr_length , size_t input_position , size_t& block_size )
		{
			size_t EM_length = _modulus.get_Size();
			if( _EM && _EM_size < EM_length )
			{
				rux::engine::free_mem( _EM );
				_EM = NULL;
			}
			if( _EM == NULL )
			{
				_EM = alloc_array_macros( rux::uint8 , EM_length );
				_EM_size = EM_length;
			}
			block_size = block_size > ( input_ptr_length - input_position ) ? ( input_ptr_length - input_position ) : block_size;
			memset( _EM , 0 , _modulus.get_Size() );
			memcpy( _EM , &input_ptr[ input_position ] , block_size );
		};
		void RSA::EME_PKCS1_v1_5_encoding( rux::uint8* input_ptr , size_t input_ptr_length , size_t input_position , size_t& block_size )
		{
			size_t EM_length = _modulus.get_Size();
			if( _EM && _EM_size < EM_length )
			{
				rux::engine::free_mem( _EM );
				_EM = NULL;
			}
			if( _EM == NULL )
			{
				_EM = alloc_array_macros( rux::uint8 , EM_length );
				_EM_size = EM_length;
			}
			_EM[ 0 ] = 0x00;
			_EM[ 1 ] = 0x02;
			block_size = block_size > ( input_ptr_length - input_position ) ? ( input_ptr_length - input_position ) : block_size;
			rux::uint32 seed_length = _modulus.get_Size() - block_size - 3;	
			rux::uint32 index1 = 2;
			if( _seed > 0 && _seed.get_Size() == seed_length )
			{
				memcpy( &_EM[ 2 ] , _seed.get_RawData() , seed_length );
				index1 += seed_length;
			}
			else
			{
				rux::uint8 random[ 128 ] = {0};
				while( seed_length >= 0 )
				{
					_random.Generate( random );
					random[ 0 ] = 127;
					while( random[ 0 ] >= 0 )
					{
						if( random[ random[ 0 ] ] != 0 )
						{			
							_EM[ index1++ ] = random[ random[ 0 ] ];
							seed_length--;
						}
						if( random[ 0 ] == 0 
							|| seed_length == 0 )
							break;
						random[ 0 ]--;
					}
					if( seed_length == 0 )
						break;
				}
			}	
			_EM[ index1++ ] = 0x00;
			memcpy( &_EM[ index1 ] , &input_ptr[ input_position ] , block_size );
		};
		void XRSA::Encrypt( rux::uint8* input_ptr , rux::uint32 input_ptr_length , rux::uint8*& output_ptr , rux::uint32& output_ptr_size , rux::uint32& output_ptr_length , rux::XString& error )
		{
			(*this)()->Encrypt( input_ptr , input_ptr_length , output_ptr , output_ptr_size , output_ptr_length , error );
		};
		void RSA::Encrypt( rux::uint8* input_ptr , rux::uint32 input_ptr_length , rux::uint8*& output_ptr , rux::uint32& output_ptr_size , rux::uint32& output_ptr_length , rux::XString& error )
		{
			CODE_LABELS_INITIALIZE();
			CODE_LABEL( NULL , NULL , 100 );
			error.Clear();
			if( input_ptr_length > 0 && input_ptr == NULL )
				error = "input pointer length is greater 0 , but input pointer is NULL";
			if( error.Length() == 0 )
			{		
				rux::XRawData message;
				size_t input_position = 0;			
				rux::uint8* raw_data_pointer = NULL;
				output_ptr_length = 0;
				size_t block_size = 0;
				if( _encryption_scheme == XEnum_RSA_EncryptionScheme_PKCS1v15 )
					block_size = _modulus.get_Size() - 11;			
				else if( _encryption_scheme == XEnum_RSA_EncryptionScheme_NoPadding )
					block_size = _modulus.get_Size();
				while( input_position < input_ptr_length )
				{
					if( _encryption_scheme == XEnum_RSA_EncryptionScheme_PKCS1v15 )
						EME_PKCS1_v1_5_encoding( input_ptr , input_ptr_length , input_position , block_size );							
					else if( _encryption_scheme == XEnum_RSA_EncryptionScheme_NoPadding )
						No_padding_encoding( input_ptr , input_ptr_length , input_position , block_size );							
					if( error.Length() == 0 )
					{
						message.set_ByRef( rux::XRawData::OS2IP( _EM , _modulus.get_Size() , error ) );
						if( error.Length() == 0 )
						{
							RSAEP( message , _modulus , _public_exponent , error );
							if( error.Length() == 0 )
							{							
								message.set_ByRef( message.I2OSP( _modulus.get_Size() , error ) );
								if( error.Length() > 0 )
									break;
								else
								{
									output_ptr_length += _modulus.get_Size();
									if( output_ptr_length > output_ptr_size )
									{
										raw_data_pointer = alloc_array_macros( rux::uint8 , output_ptr_length );
										memcpy( raw_data_pointer , output_ptr , output_ptr_size );
										output_ptr_size = output_ptr_length;
										if( output_ptr )
											rux::engine::free_mem( output_ptr );
										output_ptr = raw_data_pointer;
									}			
									memcpy( &output_ptr[ output_ptr_length - _modulus.get_Size() ] , message.get_RawData() , _modulus.get_Size() );
								}
							}
							else
								break;
						}
						else
							break;
					}
					else
						break;
					input_position += block_size;
				}
			}
		};
		void XRSA::Sign( rux::uint8* input_ptr , rux::uint32 input_ptr_length , rux::uint8*& output_ptr , rux::uint32& output_ptr_size , rux::uint32& output_ptr_length , rux::XString& error )
		{
			(*this)()->Sign( input_ptr , input_ptr_length , output_ptr , output_ptr_size , output_ptr_length , error );
		};
		rux::uint8 XRSA::Verify( rux::uint8* input_ptr , rux::uint32 input_ptr_length , rux::uint8* signature , size_t signature_size , rux::XString& error )
		{
			return (*this)()->Verify( input_ptr , input_ptr_length , signature , signature_size , error );
		};
		rux::uint8 RSA::Verify( rux::uint8* input_ptr , rux::uint32 input_ptr_length , rux::uint8* signature , size_t signature_size , rux::XString& error )
		{
			CODE_LABELS_INITIALIZE();
			CODE_LABEL( NULL , NULL , 100 );	
			rux::uint8 result = 0;
			error.Clear();
			if( input_ptr_length > 0 && input_ptr == NULL )
				error = "input pointer length is greater 0 , but input pointer is NULL";
			if( signature == NULL )
				error = "signature pointer is NULL";
			else if( signature_size == 0 )
				error = "signature pointer size is equal 0";
			if( error.Length() == 0 )
			{	
				if( _encryption_scheme == XEnum_RSA_EncryptionScheme_PKCS1v15 
					&& _hash_function == XEnum_Hash_Function_sha1 )
				{	
					size_t k = _modulus.get_Size();
					if( k != signature_size )
						error = "invalid signature";
					else
					{
						rux::XRawData s;
						s.set_ByRef( rux::XRawData::OS2IP( signature , k , error ) );
						if( error.Length() == 0 )
						{
							RSAVP1( s , _modulus , _public_exponent , error );
							if( error.Length() == 0 )
							{						
								s.set_ByRef( s.I2OSP( k , error ) );
								if( error.Length() == 0 )
								{
									EMSA_PKCS1_v1_5_encoding( input_ptr , input_ptr_length , k , error );
									if( error.Length() == 0 )
									{
										if( s.get_Size() == k
											&& memcmp( s.get_RawData() , _EM , k ) == 0 )
											result = 1;
									}
								}
							}
						}
					}
				}
			}
			return result;
		};
		void RSA::Sign( rux::uint8* input_ptr , rux::uint32 input_ptr_length , rux::uint8*& output_ptr , rux::uint32& output_ptr_size , rux::uint32& output_ptr_length , rux::XString& error )
		{
			CODE_LABELS_INITIALIZE();
			CODE_LABEL( NULL , NULL , 100 );
			output_ptr_length = 0;
			error.Clear();
			if( input_ptr_length > 0 && input_ptr == NULL )
				error = "input pointer length is greater 0 , but input pointer is NULL";
			if( error.Length() == 0 )
			{	
				if( _encryption_scheme == XEnum_RSA_EncryptionScheme_PKCS1v15 
					&& _hash_function == XEnum_Hash_Function_sha1 )
				{	
					size_t k = _modulus.get_Size();
					EMSA_PKCS1_v1_5_encoding( input_ptr , input_ptr_length , k , error );
					if( error.Length() == 0 )
					{
						rux::XRawData m;
						m.set_ByRef( rux::XRawData::OS2IP( _EM , k , error ) );
						if( error.Length() == 0 )
						{
							RSASP1( m , _modulus , _private_exponent , error );
							if( error.Length() == 0 )
							{
								m.set_ByRef( m.I2OSP( k , error ) );
								if( error.Length() == 0 )
								{
									output_ptr_length += k;
									if( output_ptr_length > output_ptr_size )
									{
										rux::uint8* raw_data_pointer = alloc_array_macros( rux::uint8 , output_ptr_length );							
										if( output_ptr )
										{
											memcpy( raw_data_pointer , output_ptr , output_ptr_size );
											rux::engine::free_mem( output_ptr );
										}
										output_ptr_size = output_ptr_length;
										output_ptr = raw_data_pointer;
									}			
									memcpy( &output_ptr[ output_ptr_length - m.get_Size() ] , m.get_RawData() , m.get_Size() );
								}
							}
						}
					}
				}
			}
		};
		void RSA::RSAEP( rux::XRawData& message , rux::XRawData& modulus , rux::XRawData& public_exponent , rux::XString& error )
		{
			error.Clear();
			if( message >= 0 && message < modulus )
				message()->modExp( *public_exponent() , *modulus() );
			else
				error = "message representative out of range";
		};
		void RSA::RSADP( rux::XRawData& message , rux::XRawData& modulus , rux::XRawData& private_exponent , rux::XString& error )
		{
			error.Clear();
			if( message >= 0 && message < modulus )
				message()->modExp( *private_exponent() , *modulus() );
			else
				error = "ciphertext representative out of range";
		};
		void XRSA::Decrypt( rux::uint8* input_ptr , rux::uint32 input_ptr_length , rux::uint8*& output_ptr , rux::uint32& output_ptr_size , rux::uint32& output_ptr_length , rux::XString& error )
		{
			(*this)()->Decrypt( input_ptr , input_ptr_length , output_ptr , output_ptr_size , output_ptr_length , error );
		};
		void RSA::Decrypt( rux::uint8* input_ptr , rux::uint32 input_ptr_length , rux::uint8*& output_ptr , rux::uint32& output_ptr_size , rux::uint32& output_ptr_length , rux::XString& error )
		{
			CODE_LABELS_INITIALIZE();
			CODE_LABEL( NULL , NULL , 100 );
			error.Clear();
			if( input_ptr )
			{
				if( input_ptr_length > 0 )
				{		
					output_ptr_length = 0;
					size_t k = _modulus.get_Size();
					if( k >= 11 )
					{
						rux::XRawData em , m;
						for( size_t index0 = 0 ; index0 < input_ptr_length ; index0 += k )
						{
							m.set_ByRef( rux::XRawData::OS2IP( &input_ptr[ index0 ] , k , error ) );
							if( error.Length() == 0 )
							{
								if( m.get_Size() != k )
								{
									error = "decryption error";
									break;
								}
								RSADP( m , _modulus , _private_exponent , error );
								if( error.Length() == 0 )
								{
									em.set_ByRef( m.I2OSP( k , error ) );
									if( error.Length() > 0 )
										break;
									if( _encryption_scheme == XEnum_RSA_EncryptionScheme_PKCS1v15 )
									{
										if( em.get_RawData()[ 0 ] == 0x00
											&& em.get_RawData()[ 1 ] == 0x02 )
										{
											size_t index1 = 2;
											for( ; index1 < em.get_Size() ; index1++ )
											{
												if( em.get_RawData()[ index1 ] == 0x00 )
													break;
											}
											if( index1 + 1 < em.get_Size() )
											{
												size_t M_size = em.get_Size() - index1 - 1;											
												if( output_ptr_length + M_size > output_ptr_size )
												{
													output_ptr_size = output_ptr_length + M_size;
													rux::uint8* output_ptr0 = alloc_array_macros( rux::uint8 , output_ptr_size );
													if( output_ptr )
													{
														if( output_ptr_length > 0 )
															memcpy( output_ptr0 , output_ptr , output_ptr_length );
														rux::engine::free_mem( output_ptr );
													}
													output_ptr = output_ptr0;
												}			
												memcpy( &output_ptr[ output_ptr_length ] , &em.get_RawData()[ index1 + 1 ] , M_size );
												output_ptr_length += M_size;											
											}
											else
											{
												error = "decryption error";
												break;
											}								
										}
									}
									else if( _encryption_scheme == XEnum_RSA_EncryptionScheme_NoPadding )
									{
										size_t M_size = em.get_Size();
										if( output_ptr_length + M_size > output_ptr_size )
										{
											output_ptr_size = output_ptr_length + M_size;
											rux::uint8* output_ptr0 = alloc_array_macros( rux::uint8 , output_ptr_size );
											if( output_ptr )
											{
												if( output_ptr_length > 0 )
													memcpy( output_ptr0 , output_ptr , output_ptr_length );
												rux::engine::free_mem( output_ptr );
											}
											output_ptr = output_ptr0;
										}			
										memcpy( &output_ptr[ output_ptr_length ] , em.get_RawData() , M_size );
										output_ptr_length += M_size;		
									}
								}
								else
									break;
							}
							else
								break;
						}			
					}
					else
						error = "decryption error";
				}
				else
					error = "input pointer length is equal 0";
			}
			else
				error = "input pointer is NULL";
		};
		void XRSA::set_EncryptionScheme( XEnum_RSA_EncryptionScheme encryption_scheme )
		{
			(*this)()->_encryption_scheme = encryption_scheme;
		};
		void XRSA::FromPEMFile( XRSA& rsa , const rux::XString& pem_filename , rux::XString& error )
		{
			error.Clear();
			rux::io::XFile pem = rux::io::XFile::Open( pem_filename , error );
			if( pem.Exists() == 1 )
			{
				rux::XString pem_text;
				pem_text.set_ByRef( pem.ReadTextAll( error ) );
				pem.Close();
				if( error.Length() == 0 )
				{
					rux::XArray< rux::XString > lines;
					lines.set_ByRef( pem_text.Split( "\n" ) );
					if( lines[ 0 ] == "-----BEGIN PUBLIC KEY-----" )
					{
						lines.RemoveAt( 0 );
						if( lines[ lines.Count() - 1 ] == "-----END PUBLIC KEY-----" )
						{
							lines.RemoveAt( lines.Count() - 1 );					
							pem_text.set_ByRef( rux::XString::Join( "" , lines ) );
							rux::uint8* base64_data = NULL;
							size_t base64_data_size = 0;
							rux::data::XBase64::Decode( pem_text.str() , base64_data , base64_data_size , error );
							if( error.Length() == 0 )
							{						
								rux::network::AsnPacket asn_packet;
								rux::network::AsnPacket::parse( asn_packet , base64_data , base64_data_size , error );
								if( error.Length() == 0 )
								{
									XMallocArray< rux::network::AsnPacket* > asn_packets;
									rux::network::AsnPacket::parse( asn_packet , asn_packets , error );
									if( error.Length() == 0 )
									{
										if( asn_packets.Count() == 2 )
										{
											rux::engine::free_object< rux::network::AsnPacket >( asn_packets[ 0 ] );									
											rux::network::AsnPacket* bitstring_asn_packet = asn_packets[ 1 ];
											asn_packets.Clear();									
											rux::network::AsnPacket::parse( asn_packet , (rux::uint8*)&bitstring_asn_packet->_buffer_ptr[ 1 ] , bitstring_asn_packet->_buffer_ptr_length - 1 , error );
											rux::engine::free_object< rux::network::AsnPacket >( bitstring_asn_packet );									
											if( error.Length() == 0 )
											{
												rux::network::AsnPacket::parse( asn_packet , asn_packets , error );
												if( error.Length() == 0 )
												{
													for( rux::uint32 index0 = 0 ; index0 < asn_packets.Count() ; index0++ )
													{
														if( index0 == 0 )
														{
															rsa()->_modulus.set_ByRef( rux::XRawData::FromReverseOrder( (rux::uint8*)asn_packets[ index0 ]->_buffer_ptr , asn_packets[ index0 ]->_buffer_ptr_length ) );
															rsa()->_modulus.ResetForceSize();
														}
														else if( index0 == 1 )
															rsa()->_public_exponent.set_ByRef( rux::XRawData::FromReverseOrder( (rux::uint8*)asn_packets[ index0 ]->_buffer_ptr , asn_packets[ index0 ]->_buffer_ptr_length ) );
														rsa()->_seed = (::rux::uint32)0;
														rux::engine::free_object< rux::network::AsnPacket >( asn_packets[ index0 ] );
													}
												}
											}
										}
										else
										{
											rux_write_error( "Private key PEM file format is incorrect" , error );
										}
									}
								}
							}
						}
					}
					else if( lines[ 0 ] == "-----BEGIN RSA PRIVATE KEY-----" )
					{
						lines.RemoveAt( 0 );
						if( lines[ lines.Count() - 1 ] == "-----END RSA PRIVATE KEY-----" )
						{
							lines.RemoveAt( lines.Count() - 1 );
							pem_text.set_ByRef( rux::XString::Join( "" , lines ) );
							rux::uint8* base64_data = NULL;
							size_t base64_data_size = 0;
							rux::data::XBase64::Decode( pem_text.str() , base64_data , base64_data_size , error );
							if( error.Length() == 0 )
							{						
								rux::network::AsnPacket asn_packet;
								rux::network::AsnPacket::parse( asn_packet , base64_data , base64_data_size , error );
								if( error.Length() == 0 )
								{
									XMallocArray< rux::network::AsnPacket* > asn_packets;
									rux::network::AsnPacket::parse( asn_packet , asn_packets , error );
									if( error.Length() == 0 )
									{
										if( asn_packets.Count() == 9 )
										{
											for( rux::uint32 index0 = 0 ; index0 < asn_packets.Count() ; index0++ )
											{
												if( index0 == 1 )
												{
													rsa()->_modulus.set_ByRef( rux::XRawData::FromReverseOrder( (rux::uint8*)asn_packets[ index0 ]->_buffer_ptr , asn_packets[ index0 ]->_buffer_ptr_length ) );
													rsa()->_modulus.ResetForceSize();
												}
												else if( index0 == 2 )
													rsa()->_public_exponent.set_ByRef( rux::XRawData::FromReverseOrder( (rux::uint8*)asn_packets[ index0 ]->_buffer_ptr , asn_packets[ index0 ]->_buffer_ptr_length ) );
												else if( index0 == 3 )
													rsa()->_private_exponent.set_ByRef( rux::XRawData::FromReverseOrder( (rux::uint8*)asn_packets[ index0 ]->_buffer_ptr , asn_packets[ index0 ]->_buffer_ptr_length ) );
												else if( index0 == 4 )
													rsa()->_prime1.set_ByRef( rux::XRawData::FromReverseOrder( (rux::uint8*)asn_packets[ index0 ]->_buffer_ptr , asn_packets[ index0 ]->_buffer_ptr_length ) );
												else if( index0 == 5 )
													rsa()->_prime2.set_ByRef( rux::XRawData::FromReverseOrder( (rux::uint8*)asn_packets[ index0 ]->_buffer_ptr , asn_packets[ index0 ]->_buffer_ptr_length ) );
												else if( index0 == 6 )
													rsa()->_prime_exponent1.set_ByRef( rux::XRawData::FromReverseOrder( (rux::uint8*)asn_packets[ index0 ]->_buffer_ptr , asn_packets[ index0 ]->_buffer_ptr_length ) );
												else if( index0 == 7 )
													rsa()->_prime_exponent2.set_ByRef( rux::XRawData::FromReverseOrder( (rux::uint8*)asn_packets[ index0 ]->_buffer_ptr , asn_packets[ index0 ]->_buffer_ptr_length ) );
												else if( index0 == 8 )
													rsa()->_inverse_prime.set_ByRef( rux::XRawData::FromReverseOrder( (rux::uint8*)asn_packets[ index0 ]->_buffer_ptr , asn_packets[ index0 ]->_buffer_ptr_length ) );
												rux::engine::free_object< rux::network::AsnPacket >( asn_packets[ index0 ] );
											}
										}
										else
										{
											rux_write_error( "Private key PEM file format is incorrect" , error );
										}
									}
								}
							}
						}
					}
				}
			}
		};
		rux::String& XRSA::get_ModulusAsOctetString( void )
		{
			return (*this)()->_modulus.ToOctetString();
		};
		rux::String& XRSA::get_PublicExponentAsOctetString( void )
		{
			return (*this)()->_public_exponent.ToOctetString();
		};
		rux::String& XRSA::get_PrivateExponentAsOctetString( void )
		{
			return (*this)()->_private_exponent.ToOctetString();
		};
		rux::String& XRSA::get_Prime1AsOctetString( void )
		{
			return (*this)()->_prime1.ToOctetString();
		};
		rux::String& XRSA::get_Prime2AsOctetString( void )
		{
			return (*this)()->_prime2.ToOctetString();
		};
		rux::String& XRSA::get_PrimeExponent1AsOctetString( void )
		{
			return (*this)()->_prime_exponent1.ToOctetString();
		};
		rux::String& XRSA::get_PrimeExponent2AsOctetString( void )
		{
			return (*this)()->_prime_exponent2.ToOctetString();
		};
		rux::String& XRSA::get_CoefficientAsOctetString( void )
		{
			return (*this)()->_inverse_prime.ToOctetString();
		};
		rux::String& XRSA::get_ModulusAsReverseOctetString( void )
		{
			return (*this)()->_modulus.ToReverseOctetString();
		};
		rux::String& XRSA::get_PublicExponentAsReverseOctetString( void )
		{
			return (*this)()->_public_exponent.ToReverseOctetString();
		};
		rux::String& XRSA::get_PrivateExponentAsReverseOctetString( void )
		{
			return (*this)()->_private_exponent.ToReverseOctetString();
		};
		rux::String& XRSA::get_Prime1AsReverseOctetString( void )
		{
			return (*this)()->_prime1.ToReverseOctetString();
		};
		rux::String& XRSA::get_Prime2AsReverseOctetString( void )
		{
			return (*this)()->_prime2.ToReverseOctetString();
		};
		rux::String& XRSA::get_PrimeExponent1AsReverseOctetString( void )
		{
			return (*this)()->_prime_exponent1.ToReverseOctetString();
		};
		rux::String& XRSA::get_PrimeExponent2AsReverseOctetString( void )
		{
			return (*this)()->_prime_exponent2.ToReverseOctetString();
		};
		rux::String& XRSA::get_CoefficientAsReverseOctetString( void )
		{
			return (*this)()->_inverse_prime.ToReverseOctetString();
		};
		void XRSA::set_Parameters( const rux::XRawData& prime1 , const rux::XRawData& prime2 , const rux::XRawData& modulus , const rux::XRawData& public_exponent , const rux::XRawData& private_exponent , const rux::XRawData& prime_exponent1 , const rux::XRawData&  prime_exponent2 , const rux::XRawData& coefficient )
		{
			(*this)()->set_Parameters( prime1 , prime2 , modulus , public_exponent , private_exponent , prime_exponent1 , prime_exponent2 , coefficient );
		};
		void RSA::set_Parameters( const rux::XRawData& prime1 , const rux::XRawData& prime2 , const rux::XRawData& modulus , const rux::XRawData& public_exponent , const rux::XRawData& private_exponent , const rux::XRawData& prime_exponent1 , const rux::XRawData&  prime_exponent2 , const rux::XRawData& coefficient )
		{
			_prime1.Copy( prime1 );
			_prime2.Copy( prime2 );
			_modulus.Copy( modulus );
			_modulus.ResetForceSize();
			_public_exponent.Copy( public_exponent );
			_private_exponent.Copy( private_exponent );
			_prime_exponent1.Copy( prime_exponent1 );
			_prime_exponent2.Copy( prime_exponent2 );
			_inverse_prime.Copy( coefficient );
		};
		void RSA::set_ParametersFromReverseOrder( rux::XRawData& prime1 , rux::XRawData& prime2 , rux::XRawData& modulus , rux::XRawData& public_exponent , rux::XRawData& private_exponent , rux::XRawData& prime_exponent1 , rux::XRawData&  prime_exponent2 , rux::XRawData& coefficient )
		{
			_prime1.set_ByRef( rux::XRawData::FromReverseOrder( prime1.get_RawData() , prime1.get_Size() ) );
			_prime2.set_ByRef( rux::XRawData::FromReverseOrder( prime2.get_RawData() , prime2.get_Size() ) );
			_modulus.set_ByRef( rux::XRawData::FromReverseOrder( modulus.get_RawData() , modulus.get_Size() ) );
			_modulus.ResetForceSize();
			_public_exponent.set_ByRef( rux::XRawData::FromReverseOrder( public_exponent.get_RawData() , public_exponent.get_Size() ) );
			_private_exponent.set_ByRef( rux::XRawData::FromReverseOrder( private_exponent.get_RawData() , private_exponent.get_Size() ) );
			_prime_exponent1.set_ByRef( rux::XRawData::FromReverseOrder( prime_exponent1.get_RawData() , prime_exponent1.get_Size() ) );
			_prime_exponent2.set_ByRef( rux::XRawData::FromReverseOrder( prime_exponent2.get_RawData() , prime_exponent2.get_Size() ) );
			_inverse_prime.set_ByRef( rux::XRawData::FromReverseOrder( coefficient.get_RawData() , coefficient.get_Size() ) );
		};
		void XRSA::set_ParametersFromReverseOrder( rux::XRawData& prime1 , rux::XRawData& prime2 , rux::XRawData& modulus , rux::XRawData& public_exponent , rux::XRawData& private_exponent , rux::XRawData& prime_exponent1 , rux::XRawData&  prime_exponent2 , rux::XRawData& coefficient )
		{
			(*this)()->set_ParametersFromReverseOrder( prime1 , prime2 , modulus , public_exponent , private_exponent , prime_exponent1 , prime_exponent2 , coefficient );
		};
		void RSA::RSAVP1( rux::XRawData& signature , rux::XRawData& modulus , rux::XRawData& public_exponent , rux::XString& error )
		{
			error.Clear();
			if( signature >= 0 && signature < modulus )
				signature()->modExp( *public_exponent() , *modulus() );
			else
				error = "signature representative out of range";
		};
		void RSA::RSASP1( rux::XRawData& message , rux::XRawData& modulus , rux::XRawData& private_exponent , rux::XString& error )
		{
			error.Clear();
			if( message >= 0 && message < modulus )
				message()->modExp( *private_exponent() , *modulus() );//TODO b
			else
				error = "message representative out of range";
		};
		void RSA::EMSA_PKCS1_v1_5_encoding( rux::io::file& signed_file , size_t k , rux::XString& error )
		{
			error.Clear();
			if( _EM && _EM_size < k )
			{
				rux::engine::free_mem( _EM );
				_EM = NULL;
			}
			if( _EM == NULL && k > 0 )
			{
				_EM = alloc_array_macros( rux::uint8 , k );
				_EM_size = k;
			}
			if( _EM )
			{
				_EM[ 0 ] = 0x00;
				_EM[ 1 ] = 0x01;
				if( _hash_function == XEnum_Hash_Function_sha1 )
				{
					if( k < 46 )
						error = "intended encoded message length too short";
					else
					{
						memset( &_EM[ 2 ] , 0xff , k - 38 );
						_EM[ k - 36 ] = 0x00;
						rux::uint8 sha1_hash[ 20 ] = {0};
						XSha1::Hash( signed_file , sha1_hash );
						XMallocArray< rux::network::AsnPacket* > digest_algorithm_asn_packets , digest_info_asn_packets;
						rux::network::AsnPacket oid , parameters , digest_algorithm , digest_info , digest;
						oid.fill_oid( "1.3.14.3.2.26" );//1.3.14.3.2.26 - SHA1 hash function
						parameters.fill( XEnum_AsnClass_Universal , 0 , XEnum_AsnUniversalClassTags_NULL );
						digest_algorithm_asn_packets.Add( &oid );
						digest_algorithm_asn_packets.Add( &parameters );	
						digest_algorithm.fill( XEnum_AsnClass_Universal , 1 , XEnum_AsnUniversalClassTags_SEQUENCE , digest_algorithm_asn_packets );		
						digest.fill( XEnum_AsnClass_Universal , 0 , XEnum_AsnUniversalClassTags_OCTETSTRING , (char*)sha1_hash , 20 );
						digest_info_asn_packets.Add( &digest_algorithm );
						digest_info_asn_packets.Add( &digest );
						digest_info.fill( XEnum_AsnClass_Universal , 1 , XEnum_AsnUniversalClassTags_SEQUENCE , digest_info_asn_packets );
						rux::network::AsnPacket::format( digest_info , &_EM[ k - 35 ] );
					}
				}
			}
		};
		void RSA::EMSA_PKCS1_v1_5_encoding( rux::uint8* input_ptr , size_t input_ptr_length , size_t k , rux::XString& error )
		{
			error.Clear();
			if( _EM && _EM_size < k )
			{
				rux::engine::free_mem( _EM );
				_EM = NULL;
			}
			if( _EM == NULL && k > 0 )
			{
				_EM = alloc_array_macros( rux::uint8 , k );
				_EM_size = k;
			}
			if( _EM )
			{
				_EM[ 0 ] = 0x00;
				_EM[ 1 ] = 0x01;
				if( _hash_function == XEnum_Hash_Function_sha1 )
				{
					if( k < 46 )
						error = "intended encoded message length too short";
					else
					{
						memset( &_EM[ 2 ] , 0xff , k - 38 );
						_EM[ k - 36 ] = 0x00;
						rux::uint8 sha1_hash[ 20 ] = {0};
						XSha1::Hash( input_ptr , input_ptr_length , sha1_hash );
						XMallocArray< rux::network::AsnPacket* > digest_algorithm_asn_packets , digest_info_asn_packets;
						rux::network::AsnPacket oid , parameters , digest_algorithm , digest_info , digest;
						oid.fill_oid( "1.3.14.3.2.26" );//1.3.14.3.2.26 - SHA1 hash function
						parameters.fill( XEnum_AsnClass_Universal , 0 , XEnum_AsnUniversalClassTags_NULL );
						digest_algorithm_asn_packets.Add( &oid );
						digest_algorithm_asn_packets.Add( &parameters );	
						digest_algorithm.fill( XEnum_AsnClass_Universal , 1 , XEnum_AsnUniversalClassTags_SEQUENCE , digest_algorithm_asn_packets );		
						digest.fill( XEnum_AsnClass_Universal , 0 , XEnum_AsnUniversalClassTags_OCTETSTRING , (char*)sha1_hash , 20 );
						digest_info_asn_packets.Add( &digest_algorithm );
						digest_info_asn_packets.Add( &digest );
						digest_info.fill( XEnum_AsnClass_Universal , 1 , XEnum_AsnUniversalClassTags_SEQUENCE , digest_info_asn_packets );
						rux::network::AsnPacket::format( digest_info , &_EM[ k - 35 ] );
					}
				}
			}
		};
		implement_duplicate_internal_result_function_3( rux::uint8 , RSA , Verify , rux::io::file& , rux::io::file& , rux::XString& );
		rux::uint8 RSA::Verify( rux::io::file& signed_file , rux::io::file& signature_file , rux::XString& error )
		{
			CODE_LABELS_INITIALIZE();
			CODE_LABEL( NULL , NULL , 100 );	
			rux::uint8 result = 0;
			error.Clear();
			if( error.Length() == 0 )
			{	
				if( _encryption_scheme == XEnum_RSA_EncryptionScheme_PKCS1v15 
					&& _hash_function == XEnum_Hash_Function_sha1 )
				{	
					size_t k = _modulus.get_Size();
					if( k != signature_file.size() )
						error = "invalid signature";
					else
					{
						rux::XRawData s;
						s.set_ByRef( rux::XRawData::OS2IP( signature_file , error ) );
						if( error.Length() == 0 )
						{
							RSAVP1( s , _modulus , _public_exponent , error );
							if( error.Length() == 0 )
							{						
								s.set_ByRef( s.I2OSP( k , error ) );
								if( error.Length() == 0 )
								{
									EMSA_PKCS1_v1_5_encoding( signed_file , k , error );
									if( error.Length() == 0 )
									{
										if( s.get_Size() == k
											&& memcmp( s.get_RawData() , _EM , k ) == 0 )
											result = 1;
									}
								}
							}
						}
					}
				}
			}
			return result;
		};
		rux::uint8 RSA::Verify( rux::io::file& signed_file , rux::uint8* signature , size_t signature_size , rux::XString& error )
		{
			CODE_LABELS_INITIALIZE();
			CODE_LABEL( NULL , NULL , 100 );	
			rux::uint8 result = 0;
			error.Clear();
			if( signature == NULL )
				error = "signature pointer is NULL";
			else if( signature_size == 0 )
				error = "signature pointer size is equal 0";
			if( error.Length() == 0 )
			{	
				if( _encryption_scheme == XEnum_RSA_EncryptionScheme_PKCS1v15 
					&& _hash_function == XEnum_Hash_Function_sha1 )
				{	
					size_t k = _modulus.get_Size();
					if( k != signature_size )
						error = "invalid signature";
					else
					{
						rux::XRawData s;
						s.set_ByRef( rux::XRawData::OS2IP( signature , k , error ) );
						if( error.Length() == 0 )
						{
							RSAVP1( s , _modulus , _public_exponent , error );
							if( error.Length() == 0 )
							{						
								s.set_ByRef( s.I2OSP( k , error ) );
								if( error.Length() == 0 )
								{
									EMSA_PKCS1_v1_5_encoding( signed_file , k , error );
									if( error.Length() == 0 )
									{
										if( s.get_Size() == k
											&& memcmp( s.get_RawData() , _EM , k ) == 0 )
											result = 1;
									}
								}
							}
						}
					}
				}
			}
			return result;
		};
		rux::uint8 XRSA::Verify( rux::io::file& signed_file , rux::uint8* signature , size_t signature_size , rux::XString& error )
		{
			return (*this)()->Verify( signed_file , signature , signature_size , error );
		};
		rux::uint8 XRSA::Verify( const rux::XString& signed_filename , const rux::XString& digest_filename , const rux::XString& pem_filename , rux::XString& error )
		{
			rux::uint8 result = 0;
			error.Clear();
			rux::XString cp_signed_filename;
			cp_signed_filename.set_ByRef( signed_filename.ConvertToAnsi() );
			rux::io::file signed_file( cp_signed_filename.str() );		
			if( signed_file.opened() )
			{
				rux::io::XFile file = rux::io::XFile::Open( digest_filename , error );
				if( file.Exists() == 1 )
				{
					size_t signature_size = 0;
					char* signature = file.ReadBytes( file.get_FileSize( error ) , signature_size , error );
					file.Close();
					if( error.Length() == 0 )
					{
						XRSA rsa_provider;
						XRSA::FromPEMFile( rsa_provider , pem_filename , error );
						if( error.Length() == 0 )
							result = rsa_provider.Verify( signed_file , (rux::uint8*)signature , signature_size , error );
					}
					if( signature )
						rux::engine::free_mem( signature );
				}
			}
			else
				error = "cannot open signed file '" + signed_filename + "'";
			return result;
		};
	};
};