#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_cryptography.h"
#include "xan_keyvaluepair.h"
#include "xan_asn_defines.h"
#include "xan_rawdata.h"
#include "xan_uint32.h"
#include "xan_uint64.h"
#include "xan_boolean.h"
#include "xan_memory_helper.h"
begin_implement_rux_class_ns( Cryptography , rux::cryptography );
	rux::XString openssl_filename , error;
#ifdef __WINDOWS__
	openssl_filename = "libeay32.dll";
#else
	//openssl_filename = "libssl.so.0.9.8";
	openssl_filename = "libssl.so";
#endif
	_openssl_module.set_ByRef( rux::loader::LoadModule( openssl_filename , NULL , error ) );
	if( error.Length() == 0 )
	{
		_DES_set_key = (DES_set_key_t)_openssl_module.get_MethodAddress( "DES_set_key" , error );
		_ERR_clear_error = (ERR_clear_error_t)_openssl_module.get_MethodAddress( "ERR_clear_error" , error );
		_DES_ecb_encrypt = (DES_ecb_encrypt_t)_openssl_module.get_MethodAddress( "DES_ecb_encrypt" , error );
		_MD4 = (MD4_t)_openssl_module.get_MethodAddress( "MD4" , error );
		_DES_set_odd_parity = (DES_set_odd_parity_t)_openssl_module.get_MethodAddress( "DES_set_odd_parity" , error );
	}
	else
	{
		_DES_set_key = NULL;
		_ERR_clear_error = NULL;
		_DES_ecb_encrypt = NULL;
		_MD4 = NULL;
		_DES_set_odd_parity = NULL;
	}
end_implement_rux_class();
namespace rux
{
	namespace network
	{
		AsnPacket::AsnPacket() 
		{
			_asn_class = XEnum_AsnClass_Universal;
			_is_constructed = 0;
			_asn_universal_class_tags = XEnum_AsnUniversalClassTags_INTEGER;
			_buffer_ptr_length = 0;
			_buffer_ptr_size = 0;
			_buffer_ptr = NULL;
			_is_delete = 1;
		};
		AsnPacket::~AsnPacket()
		{
			if( _is_delete == 1 )
			{
				if( _buffer_ptr )
					rux::engine::free_mem( _buffer_ptr );
			}
		};
		void rux::network::AsnPacket::fill( XEnum_AsnClass asn_class , rux::uint8 is_constructed , XEnum_AsnUniversalClassTags asn_universal_class_tags , XMallocArray< rux::network::AsnPacket* >& asn_packets )
		{
			if( is_constructed > 0 )
				_is_constructed = 1;
			else 
				_is_constructed = 0;
			_asn_class = asn_class;
			_asn_universal_class_tags = asn_universal_class_tags;	
			_buffer_ptr_length = 0;
			if( asn_packets.Count() > 0 )
			{
				rux::uint32 index0 = 0;
				rux::uint8 length_of_length = 0;
				for( index0 ; index0 < asn_packets.Count() ; index0++ )
				{
					length_of_length = 0;
					if( asn_packets[ index0 ]->_buffer_ptr_length > 0x7f )
					{
						if( asn_packets[ index0 ]->_buffer_ptr_length <= 0xff )
							length_of_length = 1;
						else if( asn_packets[ index0 ]->_buffer_ptr_length <= 0xffff )
							length_of_length = 2;
						else if( asn_packets[ index0 ]->_buffer_ptr_length <= 0xffffff )
							length_of_length = 3;
						else if( asn_packets[ index0 ]->_buffer_ptr_length <= 0xffffffff )
							length_of_length = 4;
					}
					_buffer_ptr_length += sizeof( char ) + sizeof( char ) + asn_packets[ index0 ]->_buffer_ptr_length + length_of_length;
				}
				if( _buffer_ptr_size < _buffer_ptr_length )	
				{
					if( _buffer_ptr )
						rux::engine::free_mem( _buffer_ptr );
					_buffer_ptr = alloc_array_macros( char , _buffer_ptr_length );
					_buffer_ptr_size = _buffer_ptr_length;
				}
				rux::uint32 written_bytes = 0;
				rux::uint8 incrementer = 0;
				rux::uint8 decrementer = 0;
				for( index0 = 0 ; index0 < asn_packets.Count() ; index0++ )
				{
					length_of_length = 0;
					if( asn_packets[ index0 ]->_buffer_ptr_length > 0x7f )
					{
						if( asn_packets[ index0 ]->_buffer_ptr_length <= 0xff )
							length_of_length = 1;
						else if( asn_packets[ index0 ]->_buffer_ptr_length <= 0xffff )
							length_of_length = 2;
						else if( asn_packets[ index0 ]->_buffer_ptr_length <= 0xffffff )
							length_of_length = 3;
						else if( asn_packets[ index0 ]->_buffer_ptr_length <= 0xffffffff )
							length_of_length = 4;
					}
					if( asn_packets[ index0 ]->_asn_class == XEnum_AsnClass_Universal )		
						_buffer_ptr[ written_bytes ] = ( _buffer_ptr[ written_bytes ] & ~ 192 ) | ( 0 );
					else if( asn_packets[ index0 ]->_asn_class == XEnum_AsnClass_Application )		
						_buffer_ptr[ written_bytes ] = ( _buffer_ptr[ written_bytes ] & ~ 192 ) | ( 64 );
					else if( asn_packets[ index0 ]->_asn_class == XEnum_AsnClass_ContextSpecific )
						_buffer_ptr[ written_bytes ] = ( _buffer_ptr[ written_bytes ] & ~ 192 ) | ( 128 );
					else if( asn_packets[ index0 ]->_asn_class == XEnum_AsnClass_Private )
						_buffer_ptr[ written_bytes ] = ( _buffer_ptr[ written_bytes ] & ~ 192 ) | ( 192 );
					_buffer_ptr[ written_bytes ] = ( _buffer_ptr[ written_bytes ] & ~ ( 1 << 5 ) ) | ( asn_packets[ index0 ]->_is_constructed << 5 );
					_buffer_ptr[ written_bytes ] = ( _buffer_ptr[ written_bytes ] & ~ 31 ) | ( (char)asn_packets[ index0 ]->_asn_universal_class_tags );				
					if( asn_packets[ index0 ]->_buffer_ptr )
					{
						if( asn_packets[ index0 ]->_buffer_ptr_length > 0 )
						{
							if( length_of_length == 0 )
								_buffer_ptr[ written_bytes + 1 ] = (rux::uint8)asn_packets[ index0 ]->_buffer_ptr_length;
							else
							{
								_buffer_ptr[ written_bytes + 1 ] = 0x80 + length_of_length;
								decrementer = length_of_length - 1;
								for( incrementer = 0 ; incrementer < length_of_length ; incrementer++ )
								{
									_buffer_ptr[ written_bytes + 2 + incrementer ] = ( (char*)&asn_packets[ index0 ]->_buffer_ptr_length )[ decrementer ];							 
									decrementer--;
								}
							}						
							memcpy( &_buffer_ptr[ written_bytes + length_of_length + 2 ] , asn_packets[ index0 ]->_buffer_ptr , asn_packets[ index0 ]->_buffer_ptr_length );									
							written_bytes += (unsigned int)(2 * sizeof( char ) + asn_packets[ index0 ]->_buffer_ptr_length + length_of_length);
						}
						else
						{
							_buffer_ptr[ written_bytes + 1 ] = 0;
							written_bytes += 2 * sizeof( char );
						}
					}
					else
					{
						_buffer_ptr[ written_bytes + 1 ] = 0;
						written_bytes += 2 * sizeof( char );
					}			
				}
			}		
		};
		void rux::network::AsnPacket::fill_oid( const char* oid_string )
		{
			if( oid_string && oid_string[ 0 ] != '\0' )
			{		
				rux::uint32 values[ 32 ] = { 0 };
				size_t cur = 0 , begin = 0 , values_count = 0 , buffer_length = 0;		
				declare_stack_variable( char , number_string , 64 );
				for( ; ; )
				{
					if( oid_string[ cur ] == '.'
						|| oid_string[ cur ] == '\0' )
					{
						memcpy( number_string , &oid_string[ begin ] , cur - begin );
						number_string[ cur - begin ] = '\0';
						values[ values_count ] = rux::XString::ToUInt32( number_string );
						if( values_count == 1 )
							buffer_length = 1;
						else if( values_count > 1 )
						{
							if( values[ values_count ] < 128 )
								buffer_length++;
							else
							{						
								if( values[ values_count ] <= 16256 )	
									buffer_length += 2;
								else if( values[ values_count ] <= 2080768 )
									buffer_length += 3;
								else if( values[ values_count ] <= 266338304 )
									buffer_length += 4;
								else
									buffer_length += 5;
							}
						}
						values_count++;
						if( oid_string[ cur ] == '.' )
							begin = cur + 1;
						else
							break;
					}
					cur++;			
				}
				if( values_count >= 2 )
				{
					if( _buffer_ptr_size < buffer_length )
					{
						if( _buffer_ptr )
							rux::engine::free_mem( _buffer_ptr );
						_buffer_ptr_size = buffer_length;
						_buffer_ptr = alloc_array_macros( char , _buffer_ptr_size );
					}
					_buffer_ptr_length = buffer_length;
					_buffer_ptr[ 0 ] = (char)(40 * values[ 0 ] + values[ 1 ]);
					cur = 1;
					for( size_t index0 = 2 ; index0 < values_count ; index0++ )
					{
						if( values[ index0 ] < 128 )
						{
							_buffer_ptr[ cur ] = (rux::uint8)values[ index0 ];
							cur++;
						}
						else
						{
							if( values[ index0 ] <= 16256 )//128
							{
								_buffer_ptr[ cur++ ] = (char)(0x80 + values[ index0 ] / 0x80);
								_buffer_ptr[ cur++ ] = values[ index0 ] % 0x80;
							}
							else if( values[ index0 ] <= 2080768 )//16384
							{
								rux::uint32 value_uint32 = values[ index0 ] / 16384;
								_buffer_ptr[ cur++ ] = (char)(0x80 + value_uint32);
								values[ index0 ] = values[ index0 ] % 16384;
								value_uint32 = values[ index0 ] / 128;
								_buffer_ptr[ cur++ ] = (char)(0x80 + value_uint32);
								_buffer_ptr[ cur++ ] = values[ index0 ] % 128;
							}
							else if( values[ index0 ] <= 266338304 )//2097152
							{
								rux::uint32 value_uint32 = values[ index0 ] / 2097152;
								_buffer_ptr[ cur++ ] = (char)(0x80 + value_uint32);
								values[ index0 ] = values[ index0 ] % 2097152;
								value_uint32 = values[ index0 ] / 16384;
								_buffer_ptr[ cur++ ] = (char)(0x80 + value_uint32);
								values[ index0 ] = values[ index0 ] % 16384;
								value_uint32 = values[ index0 ] / 128;
								_buffer_ptr[ cur++ ] = (char)(0x80 + value_uint32);
								_buffer_ptr[ cur++ ] = values[ index0 ] % 128;
							}
							else//268435456
							{
								rux::uint32 value_uint32 = values[ index0 ] / 268435456;
								_buffer_ptr[ cur++ ] = (char)(0x80 + value_uint32);
								values[ index0 ] = values[ index0 ] % 268435456;
								value_uint32 = values[ index0 ] / 2097152;
								_buffer_ptr[ cur++ ] = (char)(0x80 + value_uint32);
								values[ index0 ] = values[ index0 ] % 2097152;
								value_uint32 = values[ index0 ] / 16384;
								_buffer_ptr[ cur++ ] = (char)(0x80 + value_uint32);
								values[ index0 ] = values[ index0 ] % 16384;
								value_uint32 = values[ index0 ] / 128;
								_buffer_ptr[ cur++ ] = (char)(0x80 + value_uint32);
								_buffer_ptr[ cur++ ] = values[ index0 ] % 128;
							}					
						}
					}
					_is_constructed = 0;
					_asn_class = XEnum_AsnClass_Universal;
					_asn_universal_class_tags = XEnum_AsnUniversalClassTags_OBJECTIDENTIFIER;
				}
			}
		};
		void rux::network::AsnPacket::fill( XEnum_AsnClass asn_class , rux::uint8 is_constructed , XEnum_AsnUniversalClassTags asn_universal_class_tags )
		{
			if( is_constructed > 0 )
				_is_constructed = 1;
			else 
				_is_constructed = 0;
			_asn_class = asn_class;
			_asn_universal_class_tags = asn_universal_class_tags;	
			_buffer_ptr_length = 0;	
		};
		void rux::network::AsnPacket::fill( XEnum_AsnClass asn_class , rux::uint8 is_constructed , XEnum_AsnUniversalClassTags asn_universal_class_tags , rux::uint32 value )
		{
			if( is_constructed > 0 )
				_is_constructed = 1;
			else 
				_is_constructed = 0;
			_asn_class = asn_class;
			_asn_universal_class_tags = asn_universal_class_tags;
			rux::uint8 value_size = 4;
			if( value <= 0xff )	
				value_size = 1;
			else if( value <= 0xffff )
				value_size = 2;
			else if( value <= 0xffffff )
				value_size = 3;	
			if( _buffer_ptr_size < value_size )	
			{
				if( _buffer_ptr )
					rux::engine::free_mem( _buffer_ptr );
				_buffer_ptr = alloc_array_macros( char , value_size );
				_buffer_ptr_size = value_size;
			}
			_buffer_ptr_length = value_size;	
			rux::uint8 decrementer = value_size - 1;
			for( rux::uint8 incrementer = 0 ; incrementer < value_size ; incrementer++ )
			{
				_buffer_ptr[ incrementer ] = ( (char*)&value )[ decrementer ];							 
				decrementer--;
			}
		};
		void rux::network::AsnPacket::fill( XEnum_AsnClass asn_class , rux::uint8 is_constructed , XEnum_AsnUniversalClassTags asn_universal_class_tags , char* value_ptr , rux::uint32 value_ptr_size )
		{
			if( is_constructed > 0 )
				_is_constructed = 1;
			else 
				_is_constructed = 0;
			_asn_class = asn_class;
			_asn_universal_class_tags = asn_universal_class_tags;
			if( _buffer_ptr_size < value_ptr_size )	
			{
				if( _buffer_ptr )
					rux::engine::free_mem( _buffer_ptr );
				_buffer_ptr = alloc_array_macros( char , value_ptr_size );
				_buffer_ptr_size = value_ptr_size;
			}
			if( value_ptr )
			{
				_buffer_ptr_length = value_ptr_size;	
				if( value_ptr_size > 0 )
					memcpy( _buffer_ptr , value_ptr , value_ptr_size );
			}
			else	
				_buffer_ptr_length = 0;
		};
		void rux::network::AsnPacket::format( rux::network::AsnPacket& asn_packet , rux::uint8*& network_buffer_ptr , size_t& network_buffer_size )
		{	
			rux::uint8 length_of_length = 0;
			if( asn_packet._buffer_ptr_length > 0x7f )
			{
				if( asn_packet._buffer_ptr_length <= 0xff )
					length_of_length = 1;
				else if( asn_packet._buffer_ptr_length <= 0xffff )
					length_of_length = 2;
				else if( asn_packet._buffer_ptr_length <= 0xffffff )
					length_of_length = 3;
				else if( asn_packet._buffer_ptr_length <= 0xffffffff )
					length_of_length = 4;
			}		
			if( network_buffer_size < sizeof( char ) + sizeof( char ) + asn_packet._buffer_ptr_length + length_of_length )
			{
				if( network_buffer_ptr )
					rux::engine::free_mem( network_buffer_ptr );
				network_buffer_size = sizeof( char ) + sizeof( char ) + asn_packet._buffer_ptr_length + length_of_length;
				network_buffer_ptr = alloc_array_macros( rux::uint8 , network_buffer_size );
			}
			else
				network_buffer_size = sizeof( char ) + sizeof( char ) + asn_packet._buffer_ptr_length + length_of_length;
			if( asn_packet._asn_class == XEnum_AsnClass_Universal )		
				network_buffer_ptr[ 0 ] = ( network_buffer_ptr[ 0 ] & ~ 192 ) | ( 0 );
			else if( asn_packet._asn_class == XEnum_AsnClass_Application )		
				network_buffer_ptr[ 0 ] = ( network_buffer_ptr[ 0 ] & ~ 192 ) | ( 64 );
			else if( asn_packet._asn_class == XEnum_AsnClass_ContextSpecific )
				network_buffer_ptr[ 0 ] = ( network_buffer_ptr[ 0 ] & ~ 192 ) | ( 128 );
			else if( asn_packet._asn_class == XEnum_AsnClass_Private )
				network_buffer_ptr[ 0 ] = ( network_buffer_ptr[ 0 ] & ~ 192 ) | ( 192 );
			network_buffer_ptr[ 0 ] = ( network_buffer_ptr[ 0 ] & ~ ( 1 << 5 ) ) | ( asn_packet._is_constructed << 5 );
			network_buffer_ptr[ 0 ] = ( network_buffer_ptr[ 0 ] & ~ 31 ) | ( (char)asn_packet._asn_universal_class_tags );			
			if( asn_packet._buffer_ptr )
			{
				if( asn_packet._buffer_ptr_length > 0 )
				{
					if( length_of_length == 0 )
						network_buffer_ptr[ 1 ] = (rux::uint8)asn_packet._buffer_ptr_length;
					else
					{
						network_buffer_ptr[ 1 ] = 0x80 + length_of_length;
						rux::uint8 decrementer = length_of_length - 1;
						for( rux::uint8 incrementer = 0 ; incrementer < length_of_length ; incrementer++ )
						{
							network_buffer_ptr[ 2 + incrementer ] = ( (char*)&asn_packet._buffer_ptr_length )[ decrementer ];							 
							decrementer--;
						}
					}
					memcpy( &network_buffer_ptr[ length_of_length + 2 ] , asn_packet._buffer_ptr , asn_packet._buffer_ptr_length );			
				}
				else
					network_buffer_ptr[ 1 ] = 0;
			}
			else
				network_buffer_ptr[ 1 ] = 0;
		};
		size_t rux::network::AsnPacket::format( rux::network::AsnPacket& asn_packet , rux::uint8* network_buffer_ptr )
		{	
			size_t fill_size = 0;
			rux::uint8 length_of_length = 0;
			if( asn_packet._buffer_ptr_length > 0x7f )
			{
				if( asn_packet._buffer_ptr_length <= 0xff )
					length_of_length = 1;
				else if( asn_packet._buffer_ptr_length <= 0xffff )
					length_of_length = 2;
				else if( asn_packet._buffer_ptr_length <= 0xffffff )
					length_of_length = 3;
				else if( asn_packet._buffer_ptr_length <= 0xffffffff )
					length_of_length = 4;
			}
			if( asn_packet._asn_class == XEnum_AsnClass_Universal )		
				network_buffer_ptr[ 0 ] = ( network_buffer_ptr[ 0 ] & ~ 192 ) | ( 0 );
			else if( asn_packet._asn_class == XEnum_AsnClass_Application )		
				network_buffer_ptr[ 0 ] = ( network_buffer_ptr[ 0 ] & ~ 192 ) | ( 64 );
			else if( asn_packet._asn_class == XEnum_AsnClass_ContextSpecific )
				network_buffer_ptr[ 0 ] = ( network_buffer_ptr[ 0 ] & ~ 192 ) | ( 128 );
			else if( asn_packet._asn_class == XEnum_AsnClass_Private )
				network_buffer_ptr[ 0 ] = ( network_buffer_ptr[ 0 ] & ~ 192 ) | ( 192 );
			network_buffer_ptr[ 0 ] = ( network_buffer_ptr[ 0 ] & ~ ( 1 << 5 ) ) | ( asn_packet._is_constructed << 5 );
			network_buffer_ptr[ 0 ] = ( network_buffer_ptr[ 0 ] & ~ 31 ) | ( (char)asn_packet._asn_universal_class_tags );			
			if( asn_packet._buffer_ptr )
			{
				if( asn_packet._buffer_ptr_length > 0 )
				{
					if( length_of_length == 0 )
						network_buffer_ptr[ 1 ] = (rux::uint8)asn_packet._buffer_ptr_length;
					else
					{
						network_buffer_ptr[ 1 ] = 0x80 + length_of_length;
						rux::uint8 decrementer = length_of_length - 1;
						for( rux::uint8 incrementer = 0 ; incrementer < length_of_length ; incrementer++ )
						{
							network_buffer_ptr[ 2 + incrementer ] = ( (char*)&asn_packet._buffer_ptr_length )[ decrementer ];							 
							decrementer--;
						}
					}
					memcpy( &network_buffer_ptr[ length_of_length + 2 ] , asn_packet._buffer_ptr , asn_packet._buffer_ptr_length );			
					fill_size = length_of_length + 2 + asn_packet._buffer_ptr_length;
				}
				else
					network_buffer_ptr[ 1 ] = 0;
			}
			else
				network_buffer_ptr[ 1 ] = 0;
			return fill_size;
		};
		rux::uint32 rux::network::AsnPacket::get_length( rux::uint8* network_buffer_ptr , size_t network_buffer_size , rux::XString& error )
		{
			rux::uint32 length = 0;
			if( network_buffer_size >= 2 )
				{
					if( network_buffer_ptr )
					{				
						if( (rux::uint8)network_buffer_ptr[ 1 ] > (rux::uint8)0x7f )
						{
							rux::uint8 length_of_length = network_buffer_ptr[ 1 ] - 0x80;
							if( network_buffer_size >= 2 + length_of_length )
							{
								rux::uint8 decrementer = length_of_length - 1;
								for( rux::uint8 incrementer = 0 ; incrementer < length_of_length ; incrementer++ )
								{
									( (char*)&length )[ incrementer ] = network_buffer_ptr[ 2 + decrementer];									
									decrementer--;
								}
								length += 2 + length_of_length;
							}
							else
								error = "Length of ASN length is higher received buffer size";				
						}
						else			
							length = 2 + network_buffer_ptr[ 1 ];				
					}
					else
						error = "Receive network buffer pointer is NULL";
				}
				else
					error = "Receive network buffer size is less 2";
			return length;
		};
		void rux::network::AsnPacket::free_packets( XMallocArray< rux::network::AsnPacket* >& asn_packets )
		{
			for( rux::uint32 index0 = 0 ; index0 < asn_packets.Count() ; index0++ )
			{
				if( asn_packets[ index0 ] )
					rux::engine::free_object< rux::network::AsnPacket >( asn_packets[ index0 ] );
			}
			asn_packets.Clear();
		};
		void rux::network::AsnPacket::parse( rux::network::AsnPacket& asn_packet , XMallocArray< rux::network::AsnPacket* >& asn_packets , rux::XString& error )
		{
			for( rux::uint32 index0 = 0 ; index0 < asn_packets.Count() ; index0++ )
			{
				if( asn_packets[ index0 ] )
					rux::engine::free_object< rux::network::AsnPacket >( asn_packets[ index0 ] );
			}
			asn_packets.Clear();
			size_t readen_size = 0 , offset = 0;
			while( asn_packet._buffer_ptr_length > offset )
			{
				rux::network::AsnPacket* asn_packet_ptr = rux::engine::alloc_object< rux::network::AsnPacket >();
				readen_size = parse( *asn_packet_ptr , (rux::uint8*)&asn_packet._buffer_ptr[ offset ] , asn_packet._buffer_ptr_length - offset , error );
				if( error.Length() == 0 )
				{
					offset += readen_size;
					asn_packets.Add( asn_packet_ptr );
				}
				else
				{
					for( rux::uint32 index0 = 0 ; index0 < asn_packets.Count() ; index0++ )
					{
						if( asn_packets[ index0 ] )
							rux::engine::free_object< rux::network::AsnPacket >( asn_packets[ index0 ] );
					}
					asn_packets.Clear();
					break;
				}
			}
		};
		void rux::network::AsnPacket::oid_to_string( rux::XString& oid , rux::XString& error )
		{
			oid.Clear();
			if( _asn_universal_class_tags == XEnum_AsnUniversalClassTags_OBJECTIDENTIFIER )
			{
				size_t asn_length = _buffer_ptr_length - 1;
				rux::uint32 multiplicator = 0x80 , value = 0;
				rux::XString temp_string;
				for( ; ; )
				{
					if( asn_length == 0 )
					{
						if( (rux::uint8)_buffer_ptr[ asn_length ] >= 0x28 )
						{
							temp_string.set_ByRef( "." + XUInt32( value ).ToString() );
							oid.InsertRange( 0, temp_string );
							value = (rux::uint8)_buffer_ptr[ asn_length ] % 0x28;
							temp_string.set_ByRef( "." + XUInt32( value ).ToString() );
							oid.InsertRange( 0, temp_string );
							value = (rux::uint8)_buffer_ptr[ asn_length ] / 0x28;
							temp_string.set_ByRef( XUInt32( value ).ToString() );
							oid.InsertRange( 0, temp_string );
						}
						else
							error = "Object identifier buffer contains incorrect first byte";
						break;
					}
					else if( (rux::uint8)_buffer_ptr[ asn_length ] < 0x80 )
					{				
						if( asn_length + 1 != _buffer_ptr_length )
						{
							temp_string.set_ByRef( "." + XUInt32( value ).ToString() );
							oid.InsertRange( 0, temp_string );																				
						}
						value = (rux::uint8)_buffer_ptr[ asn_length ];
						multiplicator = 0x80;
					}
					else
					{
						value += ( (rux::uint8)_buffer_ptr[ asn_length ] - 0x80 ) * multiplicator;
						multiplicator *= 0x80;
					}
					asn_length--;
				}
			}
			else
				error = "Asn Universal Class Tag is not OBJECTIDENTIFIER";
		};
		size_t rux::network::AsnPacket::parse( rux::network::AsnPacket& asn_packet , rux::uint8* network_buffer_ptr , size_t network_buffer_size , rux::XString& error )
		{
			size_t readen_bytes = 0;	
			asn_packet._buffer_ptr_length = 0;
			if( network_buffer_size >= 2 )
			{
				if( network_buffer_ptr )
				{
					rux::uint8 is_set_8bit = ( network_buffer_ptr[ 0 ] & 128 ) > 0 ? 1 : 0;
					rux::uint8 is_set_7bit = ( network_buffer_ptr[ 0 ] & 64 ) > 0 ? 1 : 0;
					if( is_set_8bit && is_set_7bit )
						asn_packet._asn_class = XEnum_AsnClass_Private;
					else if( is_set_8bit )
						asn_packet._asn_class = XEnum_AsnClass_ContextSpecific;
					else if( is_set_7bit )
						asn_packet._asn_class = XEnum_AsnClass_Application;
					else
						asn_packet._asn_class = XEnum_AsnClass_Universal;
					asn_packet._is_constructed = ( network_buffer_ptr[ 0 ] & 32 ) > 0 ? 1 : 0;
					asn_packet._asn_universal_class_tags = (XEnum_AsnUniversalClassTags)( network_buffer_ptr[ 0 ] & ~ 224 );
					rux::uint8 length_of_length = 0;				
					if( (rux::uint8)network_buffer_ptr[ 1 ] > (rux::uint8)0x7f )
					{
						length_of_length = network_buffer_ptr[ 1 ] - 0x80;
						if( network_buffer_size >= 2 + length_of_length )
						{
							rux::uint8 decrementer = length_of_length - 1;
							for( rux::uint8 incrementer = 0 ; incrementer < length_of_length ; incrementer++ )
							{
								( (char*)&asn_packet._buffer_ptr_length )[ incrementer ] = network_buffer_ptr[ 2 + decrementer];									
								decrementer--;
							}
						}
						else
							error = "Length of ASN length is higher receive buffer size";				
					}
					else			
						asn_packet._buffer_ptr_length = network_buffer_ptr[ 1 ];
					readen_bytes += 2 + length_of_length + asn_packet._buffer_ptr_length;
					if( error.Length() == 0 )
					{
						if( asn_packet._buffer_ptr_length <= network_buffer_size - 2 - length_of_length )
						{
							if( asn_packet._buffer_ptr_size < asn_packet._buffer_ptr_length )	
							{
								if( asn_packet._buffer_ptr )
									rux::engine::free_mem( asn_packet._buffer_ptr );
								asn_packet._buffer_ptr = alloc_array_macros( char , asn_packet._buffer_ptr_length );
								asn_packet._buffer_ptr_size = asn_packet._buffer_ptr_length;
							}
							if( asn_packet._buffer_ptr_length > 0 )
								memcpy( asn_packet._buffer_ptr , &network_buffer_ptr[ 2 + length_of_length ] , asn_packet._buffer_ptr_length );
						}
						else
							error = "ASN length is higher receive buffer size";				
					}
				}
				else
					error = "Receive network buffer pointer is NULL";
			}
			else
				error = "Receive network buffer size is less 2";
			return readen_bytes;
		};
	};
	namespace cryptography
	{
		#define gost_hash_init 0xE819F743DE590AB0
		rux::uint8 tz[ 8 ][ 16 ] = {
			0x4, 0x2, 0xF, 0x5, 0x9, 0x1, 0x0, 0x8, 0xE, 0x3, 0xB, 0xC, 0xD, 0x7, 0xA, 0x6,
			0xC, 0x9, 0xF, 0xE, 0x8, 0x1, 0x3, 0xA, 0x2, 0x7, 0x4, 0xD, 0x6, 0x0, 0xB, 0x5,
			0xD, 0xF, 0xE, 0xC, 0x7, 0x3, 0x9, 0xA, 0x1, 0x5, 0x2, 0x4, 0x6, 0x8, 0x0, 0xB,
			0xE, 0x9, 0xB, 0x2, 0x5, 0xF, 0x7, 0x1, 0x0, 0xD, 0xC, 0x6, 0xA, 0x4, 0x3, 0x8,
			0x3, 0xE, 0x5, 0x9, 0x6, 0x8, 0x0, 0xD, 0xA, 0xB, 0x7, 0xC, 0x2, 0x1, 0xF, 0x4,
			0x8, 0xF, 0x6, 0xB, 0x1, 0x9, 0xC, 0x5, 0xD, 0x3, 0x7, 0xA, 0x0, 0xE, 0x2, 0x4,
			0xA, 0xB, 0xC, 0x0, 0x3, 0x6, 0x7, 0x5, 0x4, 0x8, 0xE, 0xF, 0x1, 0x9, 0x2, 0xD,
			0xC, 0x6, 0x5, 0x2, 0xB, 0x0, 0x9, 0xD, 0x3, 0xE, 0x7, 0xA, 0xF, 0x4, 0x1, 0x8
		};
		Cryptography::~Cryptography()
		{
			if( _openssl_module.IsLoaded() == 1 )
			{
				rux::loader::UnLoadModule( _openssl_module );
				_DES_set_key = NULL;
				_ERR_clear_error = NULL;
				 _DES_ecb_encrypt = NULL;
				 _MD4 = NULL;
				 _DES_set_odd_parity = NULL;
			}
		};
		rux::uint64 XCryptography::Hash( unsigned char* data_ptr, rux::int32 data_size )
		{
			rux::uint64 hash = gost_hash_init;
			rux::int32 i = 0;
			rux::int32 nsize8 = data_size >> 3;
			rux::int32 nsizeOst = data_size % 8;	
			try
			{
				rux::uint64 data = 0;
				GostHashStep( hash );
				for( i=0;i<nsize8;i++ )
				{
					data = ( (rux::uint64*)data_ptr )[ 0 ];
					data ^= hash;
					GostHashStep( data );
					hash = data;
					data_ptr += sizeof( rux::uint64 );
				};
				if( nsizeOst )
				{
					data = 0;
					memcpy( &data , data_ptr , nsizeOst );
					data ^= hash;
					GostHashStep( data );
					hash = data;
				};
			}
			catch( ... )
			{
			}	
			return hash;
		};
		bool XCryptography::GostHashStep( rux::uint64 &data )
		{
			bool res = true;	
			try
			{
				rux::uint32 n1;
				rux::uint32 n2;
				char i;
				for( i = 0; i < 8; i++ )
				{
					GostBase( data, i );
				};
				for( i = 0; i < 8; i++ )
				{
					GostBase( data, i );
				};
				for( i = 0; i < 8; i++ )
				{
					GostBase( data, i );
				};
				for( i = 7; i >= 0; i-- )
				{
					GostBase( data, i );
				};
				n1 = ( (rux::uint32*)&data )[ 0 ];
				n2 = ( (rux::uint32*)&data )[ 1 ];
				( (rux::uint32*)&data )[ 1 ] = n1;
				( (rux::uint32*)&data )[ 0 ] = n2;
			}
			catch( ... )
			{
			};		
			return res;
		};
		bool XCryptography::GostBase( rux::uint64& data, rux::uint8 index )
		{
			bool res = true;	
			try
			{
				rux::uint32 key[] = 
				{
					0xC9C02774, 0x52A2E838, 0x9C04B513, 0x4F155F55,
					0x923A7F1A, 0xE284730F, 0x14AB6F0F, 0xDCF0F7A5
				};
				rux::int32 i;
				rux::uint32 n1 = ( (rux::uint32*)&data )[ 0 ];
				rux::uint32 n3 = n1;
				n1 += key[ index ];
				rux::uint32 n2 = ( (rux::uint32*)&data )[ 1 ];
				rux::uint32 temp_n1 = n1;
				rux::uint32 temp_n2 = 0;
				for( i = 7; i >= 0; i-- )
				{
					temp_n2 <<= 4;
					temp_n2 |= tz[ i ][ ( temp_n1 >> 28 ) ];
					temp_n1 <<= 4;
				};
				n1 = ( temp_n2 << 11 ) | ( temp_n2 >> 21 );
				n2 ^= n1;
				( (rux::uint32*)&data )[ 1 ] = n3;
				( (rux::uint32*)&data )[ 0 ] = n2;
			}
			catch( ... )
			{
			};
			return res;
		};		
		dll_internal rux_volatile _inited = 0;
		dll_internal XCrtSect* _cs;
		dll_internal XCryptography* _cryptography = NULL;
		void initialize( void )
		{
			if( XInterlocked::CompareExchange( &rux::cryptography::_inited , 2 , 0 ) != 1 )
			{
				CS_PTR_LOCK( rux::cryptography::_cs );
				if( XInterlocked::CompareExchange( &rux::cryptography::_inited , 0 , 0 ) == 2 )
				{
					rux_add_deinitialization_func( rux::cryptography::deinitialize );			
					rux::cryptography::_cryptography = rux::engine::alloc_object< XCryptography >();
				}
				XInterlocked::CompareExchange( &rux::cryptography::_inited , 1 , 2 );		
				rux::cryptography::_cs->UnLock();
			}
		};
		void deinitialize( void )
		{
			if( XInterlocked::CompareExchange( &rux::cryptography::_inited , 2 , 1 ) == 1 )
			{
				CS_PTR_LOCK( rux::cryptography::_cs );
				if( rux::cryptography::_cryptography )
				{
					rux::engine::free_object< XCryptography >( rux::cryptography::_cryptography );
					rux::cryptography::_cryptography = NULL;
				}
				rux::cryptography::_cs->UnLock( 0 );
			}
		};
	};
};