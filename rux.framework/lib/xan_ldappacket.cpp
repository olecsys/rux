#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_ldappacket.h>
#include <xan_keyvaluepair.h>
#include <xan_uint64.h>
#include <xan_boolean.h>
begin_implement_rux_class_ns( LdapPacket , rux::network );
end_implement_rux_class();
namespace rux
{
	namespace network
	{	
		void XLdapPacket::set_LdapContent( const XObject& content )
		{
			(*this)()->_ldap_content = content;
		};
		Object& XLdapPacket::get_LdapContent( void )
		{
			return (*this)()->_ldap_content++;
		};
		void XLdapPacket::NetworkFormat( const XObject& send_data , rux::uint8*& network_buffer_ptr , rux::uint32& network_buffer_size , rux::XString& error )
		{
			if( rux_is_object( send_data , XLdapPacket ) )
			{
				rux::network::XLdapPacket tls_handshake_packet( send_data );
				if( rux_is_object( tls_handshake_packet()->_ldap_content , XLdapUnbindRequest ) )
				{
					XLdapUnbindRequest ldap_unbind_request( tls_handshake_packet()->_ldap_content );
					rux::uint32 total_asn_length = 0;
					//MessageId
					total_asn_length += 2 * sizeof( char );		
					if( ldap_unbind_request()->_message_id <= 0xff )
						total_asn_length += 1;
					else if( ldap_unbind_request()->_message_id <= 0xffff )
						total_asn_length += 2;
					else if( ldap_unbind_request()->_message_id <= 0xffffff )
						total_asn_length += 3;
					else if( ldap_unbind_request()->_message_id <= 0xffffffff )
						total_asn_length += 4;
					//Application
					total_asn_length += 2;
						
					rux::uint8 length_of_length = 0;
					if( total_asn_length > 0x7f )
					{
						if( total_asn_length <= 0xff )
							length_of_length = 1;
						else if( total_asn_length <= 0xffff )
							length_of_length = 2;
						else if( total_asn_length <= 0xffffff )
							length_of_length = 3;
						else if( total_asn_length <= 0xffffffff )
							length_of_length = 4;
					}		
					if( network_buffer_size < 2 * sizeof( char ) + total_asn_length + length_of_length )
					{
						if( network_buffer_ptr )
							rux::engine::free_mem( network_buffer_ptr );
						network_buffer_size = 2 * sizeof( char ) + total_asn_length + length_of_length;
						network_buffer_ptr = alloc_array_macros( rux::uint8 , network_buffer_size );
					}
					else
						network_buffer_size = 2 * sizeof( char ) + total_asn_length + length_of_length;

					rux::uint32 written_bytes = 0;
					network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ 192 ) | ( 0 );//Universal
					network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ ( 1 << 5 ) ) | ( 1 << 5 );//Constructed
					network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ 0 ] & ~ 31 ) | ( (char)XEnum_AsnUniversalClassTags_SEQUENCE );			
					rux::uint8 decrementer = 0;
					rux::uint8 incrementer = 0;
					if( length_of_length == 0 )
						network_buffer_ptr[ written_bytes + 1 ] = (rux::uint8)total_asn_length;
					else
					{
						network_buffer_ptr[ written_bytes + 1 ] = 0x80 + length_of_length;
						decrementer = length_of_length - 1;
						for( incrementer = 0 ; incrementer < length_of_length ; incrementer++ )
						{
							network_buffer_ptr[ written_bytes + 2 + incrementer ] = ( (char*)&total_asn_length )[ decrementer ];							 
							decrementer--;
						}
					}
					written_bytes += 2 * sizeof( char ) + length_of_length;

					length_of_length = 0;
					if( ldap_unbind_request()->_message_id <= 0xff )
						length_of_length = 1;
					else if( ldap_unbind_request()->_message_id <= 0xffff )
						length_of_length = 2;
					else if( ldap_unbind_request()->_message_id <= 0xffffff )
						length_of_length = 3;
					else if( ldap_unbind_request()->_message_id <= 0xffffffff )
						length_of_length = 4;		
					network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ 192 ) | ( 0 );//Universal
					network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ ( 1 << 5 ) ) | ( 0 << 5 );//Primitive
					network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ 31 ) | ( (char)XEnum_AsnUniversalClassTags_INTEGER );					
					network_buffer_ptr[ written_bytes + 1 ] = (rux::uint8)length_of_length;
					decrementer = length_of_length - 1;
					for( incrementer = 0 ; incrementer < length_of_length ; incrementer++ )
					{
						network_buffer_ptr[ written_bytes + 2 + incrementer ] = ( (char*)&ldap_unbind_request()->_message_id )[ decrementer ];							 
						decrementer--;
					}		
					written_bytes += 2 * sizeof( char ) + length_of_length;

					network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ 192 ) | ( 64 );//Application
					network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ ( 1 << 5 ) ) | ( 0 << 5 );//Constructed
					network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ 31 ) | ( (char)2 );					
					network_buffer_ptr[ written_bytes + 1 ] = (rux::uint8)0;			
				}
				else if( rux_is_object( tls_handshake_packet()->_ldap_content , XLdapBindRequest ) )
				{
					XLdapBindRequest ldap_bind_request( tls_handshake_packet()->_ldap_content );
					rux::uint32 total_asn_length = 0;
					rux::uint32 operation_header_length = 0;		
					rux::uint32 sasl_length = 0;			
					//MessageId
					total_asn_length += 2 * sizeof( char );		
					if( ldap_bind_request()->_message_id <= 0xff )
						total_asn_length += 1;
					else if( ldap_bind_request()->_message_id <= 0xffff )
						total_asn_length += 2;
					else if( ldap_bind_request()->_message_id <= 0xffffff )
						total_asn_length += 3;
					else if( ldap_bind_request()->_message_id <= 0xffffffff )
						total_asn_length += 4;
					//OperationHeader		
					//Version
					operation_header_length += 2 * sizeof( char ) + 1;	
					//Name
					rux::uint32 size = ldap_bind_request()->_name.Size() - 1;
					if( size > 0x7f )
					{
						if( size <= 0xff )
							operation_header_length += 1;
						else if( size <= 0xffff )
							operation_header_length += 2;
						else if( size <= 0xffffff )
							operation_header_length += 3;
						else if( size <= 0xffffffff )
							operation_header_length += 4;
					}
					operation_header_length += 2 * sizeof( char ) + size;
					if( ldap_bind_request()->_authentication_type == XEnum_LdapAuthenticationType_simple )
					{
						size = ldap_bind_request()->_password.Size() - 1;
						if( size > 0x7f )
						{
							if( size <= 0xff )
								operation_header_length += 1;
							else if( size <= 0xffff )
								operation_header_length += 2;
							else if( size <= 0xffffff )
								operation_header_length += 3;
							else if( size <= 0xffffffff )
								operation_header_length += 4;
						}
						operation_header_length += 2 * sizeof( char ) + size;
					}
					else if( ldap_bind_request()->_authentication_type == XEnum_LdapAuthenticationType_sasl )
					{
						size = ldap_bind_request()->_sasl_mechanism.Size() - 1;
						if( size > 0x7f )
						{
							if( size <= 0xff )
								sasl_length += 1;
							else if( size <= 0xffff )
								sasl_length += 2;
							else if( size <= 0xffffff )
								sasl_length += 3;
							else if( size <= 0xffffffff )
								sasl_length += 4;
						}
						sasl_length += 2 * sizeof( char ) + size;
						if( ldap_bind_request()->_sasl_credentials.Length() > 0 )
						{
							size = ldap_bind_request()->_sasl_credentials.Size() - 1;
							if( size > 0x7f )
							{
								if( size <= 0xff )
									sasl_length += 1;
								else if( size <= 0xffff )
									sasl_length += 2;
								else if( size <= 0xffffff )
									sasl_length += 3;
								else if( size <= 0xffffffff )
									sasl_length += 4;
							}
							sasl_length += 2 * sizeof( char ) + size;
						}
						if( sasl_length > 0x7f )
						{
							if( sasl_length <= 0xff )
								operation_header_length += 1;
							else if( sasl_length <= 0xffff )
								operation_header_length += 2;
							else if( sasl_length <= 0xffffff )
								operation_header_length += 3;
							else if( sasl_length <= 0xffffffff )
								operation_header_length += 4;
						}	
						operation_header_length += 2 * sizeof( char ) + sasl_length;
					}

					if( operation_header_length > 0x7f )
					{
						if( operation_header_length <= 0xff )
							total_asn_length += 1;
						else if( operation_header_length <= 0xffff )
							total_asn_length += 2;
						else if( operation_header_length <= 0xffffff )
							total_asn_length += 3;
						else if( operation_header_length <= 0xffffffff )
							total_asn_length += 4;
					}	
					total_asn_length += 2 * sizeof( char ) + operation_header_length;

					rux::uint8 length_of_length = 0;
					if( total_asn_length > 0x7f )
					{
						if( total_asn_length <= 0xff )
							length_of_length = 1;
						else if( total_asn_length <= 0xffff )
							length_of_length = 2;
						else if( total_asn_length <= 0xffffff )
							length_of_length = 3;
						else if( total_asn_length <= 0xffffffff )
							length_of_length = 4;
					}		
					if( network_buffer_size < 2 * sizeof( char ) + total_asn_length + length_of_length )
					{
						if( network_buffer_ptr )
							rux::engine::free_mem( network_buffer_ptr );
						network_buffer_size = 2 * sizeof( char ) + total_asn_length + length_of_length;
						network_buffer_ptr = alloc_array_macros( rux::uint8 , network_buffer_size );
					}
					else
						network_buffer_size = 2 * sizeof( char ) + total_asn_length + length_of_length;

					rux::uint32 written_bytes = 0;
					network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ 192 ) | ( 0 );//Universal
					network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ ( 1 << 5 ) ) | ( 1 << 5 );//Constructed
					network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ 0 ] & ~ 31 ) | ( (char)XEnum_AsnUniversalClassTags_SEQUENCE );			
					rux::uint8 decrementer = 0;
					rux::uint8 incrementer = 0;
					if( length_of_length == 0 )
						network_buffer_ptr[ written_bytes + 1 ] = (rux::uint8)total_asn_length;
					else
					{
						network_buffer_ptr[ written_bytes + 1 ] = 0x80 + length_of_length;
						decrementer = length_of_length - 1;
						for( incrementer = 0 ; incrementer < length_of_length ; incrementer++ )
						{
							network_buffer_ptr[ written_bytes + 2 + incrementer ] = ( (char*)&total_asn_length )[ decrementer ];							 
							decrementer--;
						}
					}
					written_bytes += 2 * sizeof( char ) + length_of_length;

					size = 0;
					if( ldap_bind_request()->_message_id <= 0xff )
						size = 1;
					else if( ldap_bind_request()->_message_id <= 0xffff )
						size = 2;
					else if( ldap_bind_request()->_message_id <= 0xffffff )
						size = 3;
					else if( ldap_bind_request()->_message_id <= 0xffffffff )
						size = 4;		
					network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ 192 ) | ( 0 );//Universal
					network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ ( 1 << 5 ) ) | ( 0 << 5 );//Primitive
					network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ 31 ) | ( (char)XEnum_AsnUniversalClassTags_INTEGER );					
					network_buffer_ptr[ written_bytes + 1 ] = (rux::uint8)size;
					decrementer = static_cast<rux::uint8>( size - 1 );
					for( incrementer = 0 ; incrementer < size ; incrementer++ )
					{
						network_buffer_ptr[ written_bytes + 2 + incrementer ] = ( (char*)&ldap_bind_request()->_message_id )[ decrementer ];							 
						decrementer--;
					}		
					written_bytes += 2 * sizeof( char ) + size;

					length_of_length = 0;
					if( operation_header_length > 0x7f )
					{
						if( operation_header_length <= 0xff )
							length_of_length = 1;
						else if( operation_header_length <= 0xffff )
							length_of_length = 2;
						else if( operation_header_length <= 0xffffff )
							length_of_length = 3;
						else if( operation_header_length <= 0xffffffff )
							length_of_length = 4;
					}		
					network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ 192 ) | ( 64 );//Application
					network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ ( 1 << 5 ) ) | ( 1 << 5 );//Constructed
					network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ 31 ) | ( (char)0 );					
					if( length_of_length == 0 )
						network_buffer_ptr[ written_bytes + 1 ] = (rux::uint8)operation_header_length;
					else
					{
						network_buffer_ptr[ written_bytes + 1 ] = 0x80 + length_of_length;
						decrementer = length_of_length - 1;
						for( incrementer = 0 ; incrementer < length_of_length ; incrementer++ )
						{
							network_buffer_ptr[ written_bytes + 2 + incrementer ] = ( (char*)&operation_header_length )[ decrementer ];							 
							decrementer--;
						}
					}
					written_bytes += 2 * sizeof( char ) + length_of_length;
			
					network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ 192 ) | ( 0 );//Universal
					network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ ( 1 << 5 ) ) | ( 0 << 5 );//Primitive
					network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ 31 ) | ( (char)XEnum_AsnUniversalClassTags_INTEGER );					
					network_buffer_ptr[ written_bytes + 1 ] = (rux::uint8)1;
					network_buffer_ptr[ written_bytes + 2 ] = ldap_bind_request()->_version;
					written_bytes += 2 * sizeof( char ) + 1;

					size = ldap_bind_request()->_name.Size() - 1;	
					length_of_length = 0;
					if( size > 0x7f )
					{
						if( size <= 0xff )
							length_of_length = 1;
						else if( size <= 0xffff )
							length_of_length = 2;
						else if( size <= 0xffffff )
							length_of_length = 3;
						else if( size <= 0xffffffff )
							length_of_length = 4;
					}	
					network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ 192 ) | ( 0 );//Universal
					network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ ( 1 << 5 ) ) | ( 0 << 5 );//Primitive
					network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ 31 ) | ( (char)XEnum_AsnUniversalClassTags_OCTETSTRING );					
					if( length_of_length == 0 )
						network_buffer_ptr[ written_bytes + 1 ] = (rux::uint8)size;
					else
					{
						network_buffer_ptr[ written_bytes + 1 ] = 0x80 + length_of_length;
						decrementer = length_of_length - 1;
						for( incrementer = 0 ; incrementer < length_of_length ; incrementer++ )
						{
							network_buffer_ptr[ written_bytes + 2 + incrementer ] = ( (char*)&size )[ decrementer ];							 
							decrementer--;
						}
					}
					written_bytes += 2 * sizeof( char ) + length_of_length;
					if( size > 0 )
						memcpy( &network_buffer_ptr[ written_bytes ] , ldap_bind_request()->_name.str() , size );
					written_bytes += size;

					if( ldap_bind_request()->_authentication_type == XEnum_LdapAuthenticationType_simple )
					{
						size = ldap_bind_request()->_password.Size() - 1;	
						length_of_length = 0;
						if( size > 0x7f )
						{
							if( size <= 0xff )
								length_of_length = 1;
							else if( size <= 0xffff )
								length_of_length = 2;
							else if( size <= 0xffffff )
								length_of_length = 3;
							else if( size <= 0xffffffff )
								length_of_length = 4;
						}	
						network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ 192 ) | ( 128 );//Universal
						network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ ( 1 << 5 ) ) | ( 0 << 5 );//Primitive
						network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ 31 ) | ( (char)ldap_bind_request()->_authentication_type );					
						if( length_of_length == 0 )
							network_buffer_ptr[ written_bytes + 1 ] = (rux::uint8)size;
						else
						{
							network_buffer_ptr[ written_bytes + 1 ] = 0x80 + length_of_length;
							decrementer = length_of_length - 1;
							for( incrementer = 0 ; incrementer < length_of_length ; incrementer++ )
							{
								network_buffer_ptr[ written_bytes + 2 + incrementer ] = ( (char*)&size )[ decrementer ];							 
								decrementer--;
							}
						}
						written_bytes += 2 * sizeof( char ) + length_of_length;
						if( size > 0 )
							memcpy( &network_buffer_ptr[ written_bytes ] , ldap_bind_request()->_password.str() , size );
						written_bytes += size;
					}
					else if( ldap_bind_request()->_authentication_type == XEnum_LdapAuthenticationType_sasl )
					{
						length_of_length = 0;
						if( sasl_length > 0x7f )
						{
							if( sasl_length <= 0xff )
								length_of_length = 1;
							else if( sasl_length <= 0xffff )
								length_of_length = 2;
							else if( sasl_length <= 0xffffff )
								length_of_length = 3;
							else if( sasl_length <= 0xffffffff )
								length_of_length = 4;
						}		
						network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ 192 ) | ( 128 );
						network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ ( 1 << 5 ) ) | ( 0 << 5 );
						network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ 31 ) | ( (char)ldap_bind_request()->_authentication_type );					
						if( length_of_length == 0 )
							network_buffer_ptr[ written_bytes + 1 ] = (rux::uint8)sasl_length;
						else
						{
							network_buffer_ptr[ written_bytes + 1 ] = 0x80 + length_of_length;
							decrementer = length_of_length - 1;
							for( incrementer = 0 ; incrementer < length_of_length ; incrementer++ )
							{
								network_buffer_ptr[ written_bytes + 2 + incrementer ] = ( (char*)&sasl_length )[ decrementer ];							 
								decrementer--;
							}
						}
						written_bytes += 2 * sizeof( char ) + length_of_length;
						size = ldap_bind_request()->_sasl_mechanism.Size() - 1;	
						length_of_length = 0;
						if( size > 0x7f )
						{
							if( size <= 0xff )
								length_of_length = 1;
							else if( size <= 0xffff )
								length_of_length = 2;
							else if( size <= 0xffffff )
								length_of_length = 3;
							else if( size <= 0xffffffff )
								length_of_length = 4;
						}	
						network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ 192 ) | ( 0 );//Universal
						network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ ( 1 << 5 ) ) | ( 0 << 5 );//Primitive
						network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ 31 ) | ( (char)XEnum_AsnUniversalClassTags_OCTETSTRING );					
						if( length_of_length == 0 )
							network_buffer_ptr[ written_bytes + 1 ] = (rux::uint8)size;
						else
						{
							network_buffer_ptr[ written_bytes + 1 ] = 0x80 + length_of_length;
							decrementer = length_of_length - 1;
							for( incrementer = 0 ; incrementer < length_of_length ; incrementer++ )
							{
								network_buffer_ptr[ written_bytes + 2 + incrementer ] = ( (char*)&size )[ decrementer ];							 
								decrementer--;
							}
						}
						written_bytes += 2 * sizeof( char ) + length_of_length;
						if( size > 0 )
							memcpy( &network_buffer_ptr[ written_bytes ] , ldap_bind_request()->_sasl_mechanism.str() , size );
						written_bytes += size;			
						if( ldap_bind_request()->_sasl_credentials.Length() > 0 )
						{
							size = ldap_bind_request()->_sasl_credentials.Size() - 1;	
							length_of_length = 0;
							if( size > 0x7f )
							{
								if( size <= 0xff )
									length_of_length = 1;
								else if( size <= 0xffff )
									length_of_length = 2;
								else if( size <= 0xffffff )
									length_of_length = 3;
								else if( size <= 0xffffffff )
									length_of_length = 4;
							}	
							network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ 192 ) | ( 0 );//Universal
							network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ ( 1 << 5 ) ) | ( 0 << 5 );//Primitive
							network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ 31 ) | ( (char)XEnum_AsnUniversalClassTags_OCTETSTRING );					
							if( length_of_length == 0 )
								network_buffer_ptr[ written_bytes + 1 ] = (rux::uint8)size;
							else
							{
								network_buffer_ptr[ written_bytes + 1 ] = 0x80 + length_of_length;
								decrementer = length_of_length - 1;
								for( incrementer = 0 ; incrementer < length_of_length ; incrementer++ )
								{
									network_buffer_ptr[ written_bytes + 2 + incrementer ] = ( (char*)&size )[ decrementer ];							 
									decrementer--;
								}
							}
							written_bytes += 2 * sizeof( char ) + length_of_length;
							if( size > 0 )
								memcpy( &network_buffer_ptr[ written_bytes ] , ldap_bind_request()->_sasl_credentials.str() , size );
							written_bytes += size;
						}			
					}					
				}
				else if( rux_is_object( tls_handshake_packet()->_ldap_content , rux::network::XLdapSearchRequest ) )
				{
					rux::network::XLdapSearchRequest ldap_search_request( tls_handshake_packet()->_ldap_content );

					rux::uint32 total_asn_length = 0;
					rux::uint32 operation_header_length = 0;		
			
					//MessageId
					total_asn_length += 2 * sizeof( char );		
					if( ldap_search_request()->_message_id <= 0xff )
						total_asn_length += 1;
					else if( ldap_search_request()->_message_id <= 0xffff )
						total_asn_length += 2;
					else if( ldap_search_request()->_message_id <= 0xffffff )
						total_asn_length += 3;
					else if( ldap_search_request()->_message_id <= 0xffffffff )
						total_asn_length += 4;

					//OperationHeader

					//BaseObject
					operation_header_length += 2 * sizeof( char );
					rux::uint32 size = ldap_search_request()->_base_object.Size() - 1;
					rux::uint8 base_object_length_of_length = 0;
					if( size > 0x7f )
					{
						if( size <= 0xff )
						{
							operation_header_length += 1;
							base_object_length_of_length = 1;
						}
						else if( size <= 0xffff )
						{
							operation_header_length += 2;
							base_object_length_of_length = 2;
						}
						else if( size <= 0xffffff )
						{
							operation_header_length += 3;
							base_object_length_of_length = 3;
						}
						else if( size <= 0xffffffff )
						{
							operation_header_length += 4;
							base_object_length_of_length = 4;
						}
					}
					operation_header_length += size;
					//Scope
					operation_header_length += 2 * sizeof( char );		
					if( ldap_search_request()->_scope <= 0xff )
						operation_header_length += 1;
					else if( ldap_search_request()->_scope <= 0xffff )
						operation_header_length += 2;
					else if( ldap_search_request()->_scope <= 0xffffff )
						operation_header_length += 3;
					else if( ldap_search_request()->_scope <= 0xffffffff )
						operation_header_length += 4;
					//DerefAliases
					operation_header_length += 2 * sizeof( char );		
					if( ldap_search_request()->_deref_aliases <= 0xff )
						operation_header_length += 1;
					else if( ldap_search_request()->_deref_aliases <= 0xffff )
						operation_header_length += 2;
					else if( ldap_search_request()->_deref_aliases <= 0xffffff )
						operation_header_length += 3;
					else if( ldap_search_request()->_deref_aliases <= 0xffffffff )
						operation_header_length += 4;
					//SizeLimit
					operation_header_length += 2 * sizeof( char );		
					if( ldap_search_request()->_size_limit <= 0xff )
						operation_header_length += 1;
					else if( ldap_search_request()->_size_limit <= 0xffff )
						operation_header_length += 2;
					else if( ldap_search_request()->_size_limit <= 0xffffff )
						operation_header_length += 3;
					else if( ldap_search_request()->_size_limit <= 0xffffffff )
						operation_header_length += 4;
					//TimeLimit
					operation_header_length += 2 * sizeof( char );		
					if( ldap_search_request()->_time_limit <= 0xff )
						operation_header_length += 1;
					else if( ldap_search_request()->_time_limit <= 0xffff )
						operation_header_length += 2;
					else if( ldap_search_request()->_time_limit <= 0xffffff )
						operation_header_length += 3;
					else if( ldap_search_request()->_time_limit <= 0xffffffff )
						operation_header_length += 4;
					//TypesOnly
					operation_header_length += 2 * sizeof( char ) + 1;		

					//Filter
					rux::uint32 index0 = 0;
					rux::uint32 filter_length = 0;
					rux::XString utf8;
					if( ldap_search_request()->_filter()->_filter_type == XEnum_LdapFilterType_and )
					{
					}
					else if( ldap_search_request()->_filter()->_filter_type == XEnum_LdapFilterType_or )
					{
					}
					else if( ldap_search_request()->_filter()->_filter_type == XEnum_LdapFilterType_not )
					{
					}
					else if( ldap_search_request()->_filter()->_filter_type == XEnum_LdapFilterType_equalityMatch
						|| ldap_search_request()->_filter()->_filter_type == XEnum_LdapFilterType_greaterOrEqual 
						|| ldap_search_request()->_filter()->_filter_type == XEnum_LdapFilterType_lessOrEqual
						|| ldap_search_request()->_filter()->_filter_type == XEnum_LdapFilterType_aproxMatch )
					{
						if( ldap_search_request()->_filter()->_matches.Count() > 0 )
						{
							if( rux_is_object( ldap_search_request()->_filter()->_matches[ 0 ] , XLdapAttributeValueAssertion ) )
							{						
								XLdapAttributeValueAssertion ldap_attribute_value_assertion( ldap_search_request()->_filter()->_matches[ 0 ] );						
								rux::uint32 size1 = 0;
								size = ldap_attribute_value_assertion()->_attribute_description.Size() - 1;
								if( size > 0x7f )
								{
									if( size <= 0xff )
										size1 += 1;
									else if( size <= 0xffff )
										size1 += 2;
									else if( size <= 0xffffff )
										size1 += 3;
									else if( size <= 0xffffffff )
										size1 += 4;
								}
								filter_length += 2 * sizeof( char ) + size1 + size;
								size1 = 0;
								size = ldap_attribute_value_assertion()->_attribute_value.Size() - 1;
								if( size > 0x7f )
								{
									if( size <= 0xff )
										size1 += 1;
									else if( size <= 0xffff )
										size1 += 2;
									else if( size <= 0xffffff )
										size1 += 3;
									else if( size <= 0xffffffff )
										size1 += 4;
								}
								filter_length += 2 * sizeof( char ) + size1 + size;
							}
						}
					}
					else if( ldap_search_request()->_filter()->_filter_type == XEnum_LdapFilterType_substrings )
					{
					}			
					else if( ldap_search_request()->_filter()->_filter_type == XEnum_LdapFilterType_present )
					{
						if( ldap_search_request()->_filter()->_matches.Count() > 0 )
						{
							if( rux_is_object( ldap_search_request()->_filter()->_matches[ 0 ] , rux::XString ) )
							{
								utf8 = ldap_search_request()->_filter()->_matches[ 0 ];
								size = utf8.Size() - 1;
								if( size > 0x7f )
								{
									if( size <= 0xff )
										filter_length += 1;
									else if( size <= 0xffff )
										filter_length += 2;
									else if( size <= 0xffffff )
										filter_length += 3;
									else if( size <= 0xffffffff )
										filter_length += 4;
								}
								filter_length += size;
							}
						}
					}			
					else if( ldap_search_request()->_filter()->_filter_type == XEnum_LdapFilterType_extensibleMatch )
					{
					}


					if( filter_length > 0x7f )
					{
						if( filter_length <= 0xff )
							operation_header_length += 1;
						else if( filter_length <= 0xffff )
							operation_header_length += 2;
						else if( filter_length <= 0xffffff )
							operation_header_length += 3;
						else if( filter_length <= 0xffffffff )
							operation_header_length += 4;
					}	
					operation_header_length += 2 * sizeof( char ) + filter_length;


					//Attributes
					rux::uint32 attribute_selection_header_length = 0; 
					for( index0 = 0 ; index0 < ldap_search_request()->_attributes.Count() ; index0++ )
					{
						size = ldap_search_request()->_attributes[ index0 ].Size() - 1;
						if( size > 0x7f )
						{
							if( size <= 0xff )
								attribute_selection_header_length += 1;
							else if( size <= 0xffff )
								attribute_selection_header_length += 2;
							else if( size <= 0xffffff )
								attribute_selection_header_length += 3;
							else if( size <= 0xffffffff )
								attribute_selection_header_length += 4;
						}
						attribute_selection_header_length += 2 * sizeof( char ) + size;
					}

					if( attribute_selection_header_length > 0x7f )
					{
						if( attribute_selection_header_length <= 0xff )
							operation_header_length += 1;
						else if( attribute_selection_header_length <= 0xffff )
							operation_header_length += 2;
						else if( attribute_selection_header_length <= 0xffffff )
							operation_header_length += 3;
						else if( attribute_selection_header_length <= 0xffffffff )
							operation_header_length += 4;
					}	
					operation_header_length += 2 * sizeof( char ) + attribute_selection_header_length;

					if( operation_header_length > 0x7f )
					{
						if( operation_header_length <= 0xff )
							total_asn_length += 1;
						else if( operation_header_length <= 0xffff )
							total_asn_length += 2;
						else if( operation_header_length <= 0xffffff )
							total_asn_length += 3;
						else if( operation_header_length <= 0xffffffff )
							total_asn_length += 4;
					}	
					total_asn_length += 2 * sizeof( char ) + operation_header_length;

					//Controls
					rux::uint32 controls_length = 0;
					rux::uint32 control_header_length = 0;
					for( index0 = 0 ; index0 < ldap_search_request()->_controls.Count() ; index0++ )
					{
						control_header_length = 0;
						size = ldap_search_request()->_controls[ index0 ]._control_type.Size() - 1;			
						if( size > 0x7f )
						{
							if( size <= 0xff )
								control_header_length += 1;
							else if( size <= 0xffff )
								control_header_length += 2;
							else if( size <= 0xffffff )
								control_header_length += 3;
							else if( size <= 0xffffffff )
								control_header_length += 4;
						}
						control_header_length += 2 * sizeof( char ) + size;
						if( ldap_search_request()->_controls[ index0 ]._criticality == 1 )			
							control_header_length += 2 * sizeof( char ) + 1;
						if( ldap_search_request()->_controls[ index0 ]._control_value.Size() - 1 > 0 )
						{
							size = ldap_search_request()->_controls[ index0 ]._control_value.Size() - 1;			
							if( size > 0x7f )
							{
								if( size <= 0xff )
									control_header_length += 1;
								else if( size <= 0xffff )
									control_header_length += 2;
								else if( size <= 0xffffff )
									control_header_length += 3;
								else if( size <= 0xffffffff )
									control_header_length += 4;
							}
							control_header_length += 2 * sizeof( char ) + size;
						}
						if( control_header_length > 0x7f )
						{
							if( control_header_length <= 0xff )
								controls_length += 1;
							else if( control_header_length <= 0xffff )
								controls_length += 2;
							else if( control_header_length <= 0xffffff )
								controls_length += 3;
							else if( control_header_length <= 0xffffffff )
								controls_length += 4;
						}	
						controls_length += 2 * sizeof( char ) + control_header_length;
					}	

					if( controls_length > 0x7f )
					{
						if( controls_length <= 0xff )
							total_asn_length += 1;
						else if( controls_length <= 0xffff )
							total_asn_length += 2;
						else if( controls_length <= 0xffffff )
							total_asn_length += 3;
						else if( controls_length <= 0xffffffff )
							total_asn_length += 4;
					}	
					total_asn_length += 2 * sizeof( char ) + controls_length;


					rux::uint8 length_of_length = 0;
					if( total_asn_length > 0x7f )
					{
						if( total_asn_length <= 0xff )
							length_of_length = 1;
						else if( total_asn_length <= 0xffff )
							length_of_length = 2;
						else if( total_asn_length <= 0xffffff )
							length_of_length = 3;
						else if( total_asn_length <= 0xffffffff )
							length_of_length = 4;
					}		
					if( network_buffer_size < 2 * sizeof( char ) + total_asn_length + length_of_length )
					{
						if( network_buffer_ptr )
							rux::engine::free_mem( network_buffer_ptr );
						network_buffer_size = 2 * sizeof( char ) + total_asn_length + length_of_length;
						network_buffer_ptr = alloc_array_macros( rux::uint8 , network_buffer_size );
					}
					else
						network_buffer_size = 2 * sizeof( char ) + total_asn_length + length_of_length;

					rux::uint32 written_bytes = 0;
					network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ 192 ) | ( 0 );//Universal
					network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ ( 1 << 5 ) ) | ( 1 << 5 );//Constructed
					network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ 0 ] & ~ 31 ) | ( (char)XEnum_AsnUniversalClassTags_SEQUENCE );			
					rux::uint8 decrementer = 0;
					rux::uint8 incrementer = 0;
					if( length_of_length == 0 )
						network_buffer_ptr[ written_bytes + 1 ] = (rux::uint8)total_asn_length;
					else
					{
						network_buffer_ptr[ written_bytes + 1 ] = 0x80 + length_of_length;
						decrementer = length_of_length - 1;
						for( incrementer = 0 ; incrementer < length_of_length ; incrementer++ )
						{
							network_buffer_ptr[ written_bytes + 2 + incrementer ] = ( (char*)&total_asn_length )[ decrementer ];							 
							decrementer--;
						}
					}
					written_bytes += 2 * sizeof( char ) + length_of_length;

					size = 0;
					if( ldap_search_request()->_message_id <= 0xff )
						size = 1;
					else if( ldap_search_request()->_message_id <= 0xffff )
						size = 2;
					else if( ldap_search_request()->_message_id <= 0xffffff )
						size = 3;
					else if( ldap_search_request()->_message_id <= 0xffffffff )
						size = 4;		
					network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ 192 ) | ( 0 );//Universal
					network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ ( 1 << 5 ) ) | ( 0 << 5 );//Primitive
					network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ 31 ) | ( (char)XEnum_AsnUniversalClassTags_INTEGER );					
					network_buffer_ptr[ written_bytes + 1 ] = (rux::uint8)size;
					decrementer = static_cast<rux::uint8>( size - 1 );
					for( incrementer = 0 ; incrementer < size ; incrementer++ )
					{
						network_buffer_ptr[ written_bytes + 2 + incrementer ] = ( (char*)&ldap_search_request()->_message_id )[ decrementer ];							 
						decrementer--;
					}		
					written_bytes += 2 * sizeof( char ) + size;

					length_of_length = 0;
					if( operation_header_length > 0x7f )
					{
						if( operation_header_length <= 0xff )
							length_of_length = 1;
						else if( operation_header_length <= 0xffff )
							length_of_length = 2;
						else if( operation_header_length <= 0xffffff )
							length_of_length = 3;
						else if( operation_header_length <= 0xffffffff )
							length_of_length = 4;
					}		
					network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ 192 ) | ( 64 );//Application
					network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ ( 1 << 5 ) ) | ( 1 << 5 );//Constructed
					network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ 31 ) | ( (char)3 );					
					if( length_of_length == 0 )
						network_buffer_ptr[ written_bytes + 1 ] = (rux::uint8)operation_header_length;
					else
					{
						network_buffer_ptr[ written_bytes + 1 ] = 0x80 + length_of_length;
						decrementer = length_of_length - 1;
						for( incrementer = 0 ; incrementer < length_of_length ; incrementer++ )
						{
							network_buffer_ptr[ written_bytes + 2 + incrementer ] = ( (char*)&operation_header_length )[ decrementer ];							 
							decrementer--;
						}
					}
					written_bytes += 2 * sizeof( char ) + length_of_length;

					size = ldap_search_request()->_base_object.Size() - 1;		
					network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ 192 ) | ( 0 );//Universal
					network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ ( 1 << 5 ) ) | ( 0 << 5 );//Primitive
					network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ 31 ) | ( (char)XEnum_AsnUniversalClassTags_OCTETSTRING );					
					if( base_object_length_of_length == 0 )
						network_buffer_ptr[ written_bytes + 1 ] = (rux::uint8)size;
					else
					{
						network_buffer_ptr[ written_bytes + 1 ] = 0x80 + base_object_length_of_length;
						decrementer = base_object_length_of_length - 1;
						for( incrementer = 0 ; incrementer < base_object_length_of_length ; incrementer++ )
						{
							network_buffer_ptr[ written_bytes + 2 + incrementer ] = ( (char*)&size )[ decrementer ];							 
							decrementer--;
						}
					}
					written_bytes += 2 * sizeof( char ) + base_object_length_of_length;
					if( size > 0 )
						memcpy( &network_buffer_ptr[ written_bytes ] , ldap_search_request()->_base_object.str() , size );
					written_bytes += size;

					size = 0;
					if( ldap_search_request()->_scope <= 0xff )
						size = 1;
					else if( ldap_search_request()->_scope <= 0xffff )
						size = 2;
					else if( ldap_search_request()->_scope <= 0xffffff )
						size = 3;
					else if( ldap_search_request()->_scope <= 0xffffffff )
						size = 4;		
					network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ 192 ) | ( 0 );//Universal
					network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ ( 1 << 5 ) ) | ( 0 << 5 );//Primitive
					network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ 31 ) | ( (char)XEnum_AsnUniversalClassTags_ENUMERATED );					
					network_buffer_ptr[ written_bytes + 1 ] = (rux::uint8)size;
					decrementer = static_cast<rux::uint8>( size - 1 );
					for( incrementer = 0 ; incrementer < size ; incrementer++ )
					{
						network_buffer_ptr[ written_bytes + 2 + incrementer ] = ( (char*)&ldap_search_request()->_scope )[ decrementer ];							 
						decrementer--;
					}
					written_bytes += 2 * sizeof( char ) + size;

					size = 0;
					if( ldap_search_request()->_deref_aliases <= 0xff )
						size = 1;
					else if( ldap_search_request()->_deref_aliases <= 0xffff )
						size = 2;
					else if( ldap_search_request()->_deref_aliases <= 0xffffff )
						size = 3;
					else if( ldap_search_request()->_deref_aliases <= 0xffffffff )
						size = 4;		
					network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ 192 ) | ( 0 );//Universal
					network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ ( 1 << 5 ) ) | ( 0 << 5 );//Primitive
					network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ 31 ) | ( (char)XEnum_AsnUniversalClassTags_ENUMERATED );					
					network_buffer_ptr[ written_bytes + 1 ] = (rux::uint8)size;
					decrementer = static_cast<rux::uint8>( size - 1 );
					for( incrementer = 0 ; incrementer < size ; incrementer++ )
					{
						network_buffer_ptr[ written_bytes + 2 + incrementer ] = ( (char*)&ldap_search_request()->_deref_aliases )[ decrementer ];							 
						decrementer--;
					}		
					written_bytes += 2 * sizeof( char ) + size;

					size = 0;
					if( ldap_search_request()->_size_limit <= 0xff )
						size = 1;
					else if( ldap_search_request()->_size_limit <= 0xffff )
						size = 2;
					else if( ldap_search_request()->_size_limit <= 0xffffff )
						size = 3;
					else if( ldap_search_request()->_size_limit <= 0xffffffff )
						size = 4;		
					network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ 192 ) | ( 0 );//Universal
					network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ ( 1 << 5 ) ) | ( 0 << 5 );//Primitive
					network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ 31 ) | ( (char)XEnum_AsnUniversalClassTags_INTEGER );					
					network_buffer_ptr[ written_bytes + 1 ] = (rux::uint8)size;
					decrementer = static_cast<rux::uint8>( size - 1 );
					for( incrementer = 0 ; incrementer < size ; incrementer++ )
					{
						network_buffer_ptr[ written_bytes + 2 + incrementer ] = ( (char*)&ldap_search_request()->_size_limit )[ decrementer ];							 
						decrementer--;
					}		
					written_bytes += 2 * sizeof( char ) + size;

					size = 0;
					if( ldap_search_request()->_time_limit <= 0xff )
						size = 1;
					else if( ldap_search_request()->_time_limit <= 0xffff )
						size = 2;
					else if( ldap_search_request()->_time_limit <= 0xffffff )
						size = 3;
					else if( ldap_search_request()->_time_limit <= 0xffffffff )
						size = 4;		
					network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ 192 ) | ( 0 );//Universal
					network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ ( 1 << 5 ) ) | ( 0 << 5 );//Primitive
					network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ 31 ) | ( (char)XEnum_AsnUniversalClassTags_INTEGER );					
					network_buffer_ptr[ written_bytes + 1 ] = (rux::uint8)size;
					decrementer = static_cast<rux::uint8>( size - 1 );
					for( incrementer = 0 ; incrementer < size ; incrementer++ )
					{
						network_buffer_ptr[ written_bytes + 2 + incrementer ] = ( (char*)&ldap_search_request()->_time_limit )[ decrementer ];							 
						decrementer--;
					}				
					written_bytes += 2 * sizeof( char ) + size;

					network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ 192 ) | ( 0 );//Universal
					network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ ( 1 << 5 ) ) | ( 0 << 5 );//Primitive
					network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ 31 ) | ( (char)XEnum_AsnUniversalClassTags_BOOLEAN );					
					network_buffer_ptr[ written_bytes + 1 ] = (rux::uint8)1;
					if( ldap_search_request()->_types_only == 1 )
						network_buffer_ptr[ written_bytes + 2 ] = 0xff;
					else
						network_buffer_ptr[ written_bytes + 2 ] = 0;		
					written_bytes += 2 * sizeof( char ) + 1;

					length_of_length = 0;
					if( filter_length > 0x7f )
					{
						if( filter_length <= 0xff )
							length_of_length = 1;
						else if( filter_length <= 0xffff )
							length_of_length = 2;
						else if( filter_length <= 0xffffff )
							length_of_length = 3;
						else if( filter_length <= 0xffffffff )
							length_of_length = 4;
					}		
					network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ 192 ) | ( 128 );//Context Specific
					if( ldap_search_request()->_filter()->_filter_type == XEnum_LdapFilterType_present )
						network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ ( 1 << 5 ) ) | ( 0 << 5 );//Primitive
					else
						network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ ( 1 << 5 ) ) | ( 1 << 5 );//Constructed
					network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ 31 ) | ( (char)ldap_search_request()->_filter()->_filter_type );					
					if( length_of_length == 0 )
						network_buffer_ptr[ written_bytes + 1 ] = (rux::uint8)filter_length;
					else
					{
						network_buffer_ptr[ written_bytes + 1 ] = 0x80 + length_of_length;
						decrementer = length_of_length - 1;
						for( incrementer = 0 ; incrementer < length_of_length ; incrementer++ )
						{
							network_buffer_ptr[ written_bytes + 2 + incrementer ] = ( (char*)&filter_length )[ decrementer ];							 
							decrementer--;
						}
					}
					written_bytes += 2 * sizeof( char ) + length_of_length;


					if( ldap_search_request()->_filter()->_filter_type == XEnum_LdapFilterType_and )
					{
					}
					else if( ldap_search_request()->_filter()->_filter_type == XEnum_LdapFilterType_or )
					{
					}
					else if( ldap_search_request()->_filter()->_filter_type == XEnum_LdapFilterType_not )
					{
					}
					else if( ldap_search_request()->_filter()->_filter_type == XEnum_LdapFilterType_substrings )
					{
					}
					else if( ldap_search_request()->_filter()->_filter_type == XEnum_LdapFilterType_equalityMatch 
						|| ldap_search_request()->_filter()->_filter_type == XEnum_LdapFilterType_greaterOrEqual
						|| ldap_search_request()->_filter()->_filter_type == XEnum_LdapFilterType_lessOrEqual
						|| ldap_search_request()->_filter()->_filter_type == XEnum_LdapFilterType_aproxMatch )			
					{
						if( ldap_search_request()->_filter()->_matches.Count() > 0 )
						{
							if( rux_is_object( ldap_search_request()->_filter()->_matches[ 0 ] , XLdapAttributeValueAssertion ) )
							{						
								XLdapAttributeValueAssertion ldap_attribute_value_assertion( ldap_search_request()->_filter()->_matches[ 0 ] );						
								length_of_length = 0;
								size = ldap_attribute_value_assertion()->_attribute_description.Size() - 1;
								if( size > 0x7f )
								{
									if( size <= 0xff )
										length_of_length += 1;
									else if( size <= 0xffff )
										length_of_length += 2;
									else if( size <= 0xffffff )
										length_of_length += 3;
									else if( size <= 0xffffffff )
										length_of_length += 4;
								}
								network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ 192 ) | ( 0 );//Universal
								network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ ( 1 << 5 ) ) | ( 0 << 5 );//Primitive
								network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ 31 ) | ( (char)XEnum_AsnUniversalClassTags_OCTETSTRING );
								if( length_of_length == 0 )
									network_buffer_ptr[ written_bytes + 1 ] = (rux::uint8)size;
								else
								{
									network_buffer_ptr[ written_bytes + 1 ] = 0x80 + length_of_length;
									decrementer = length_of_length - 1;
									for( incrementer = 0 ; incrementer < length_of_length ; incrementer++ )
									{
										network_buffer_ptr[ written_bytes + 2 + incrementer ] = ( (char*)&size )[ decrementer ];							 
										decrementer--;
									}
								}
								written_bytes += 2 * sizeof( char ) + length_of_length;
								memcpy( &network_buffer_ptr[ written_bytes ] , ldap_attribute_value_assertion()->_attribute_description.str() , size );
								written_bytes += size;

								length_of_length = 0;
								size = ldap_attribute_value_assertion()->_attribute_value.Size() - 1;
								if( size > 0x7f )
								{
									if( size <= 0xff )
										length_of_length += 1;
									else if( size <= 0xffff )
										length_of_length += 2;
									else if( size <= 0xffffff )
										length_of_length += 3;
									else if( size <= 0xffffffff )
										length_of_length += 4;
								}
								network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ 192 ) | ( 0 );//Universal
								network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ ( 1 << 5 ) ) | ( 0 << 5 );//Primitive
								network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ 31 ) | ( (char)XEnum_AsnUniversalClassTags_OCTETSTRING );
								if( length_of_length == 0 )
									network_buffer_ptr[ written_bytes + 1 ] = (rux::uint8)size;
								else
								{
									network_buffer_ptr[ written_bytes + 1 ] = 0x80 + length_of_length;
									decrementer = length_of_length - 1;
									for( incrementer = 0 ; incrementer < length_of_length ; incrementer++ )
									{
										network_buffer_ptr[ written_bytes + 2 + incrementer ] = ( (char*)&size )[ decrementer ];							 
										decrementer--;
									}
								}
								written_bytes += 2 * sizeof( char ) + length_of_length;
								memcpy( &network_buffer_ptr[ written_bytes ] , ldap_attribute_value_assertion()->_attribute_value.str() , size );
								written_bytes += size;						
							}
						}
					}			
					else if( ldap_search_request()->_filter()->_filter_type == XEnum_LdapFilterType_present )
					{
						if( ldap_search_request()->_filter()->_matches.Count() > 0 )
						{
							if( rux_is_object( ldap_search_request()->_filter()->_matches[ 0 ] , rux::XString ) )
							{
								utf8 = ldap_search_request()->_filter()->_matches[ 0 ];
								size = utf8.Size() - 1;
								memcpy( &network_buffer_ptr[ written_bytes ] , utf8.str() , size );
								written_bytes += size;
							}
						}
					}
					else if( ldap_search_request()->_filter()->_filter_type == XEnum_LdapFilterType_extensibleMatch )
					{
					}

					length_of_length = 0;
					if( attribute_selection_header_length > 0x7f )
					{
						if( attribute_selection_header_length <= 0xff )
							length_of_length = 1;
						else if( attribute_selection_header_length <= 0xffff )
							length_of_length = 2;
						else if( attribute_selection_header_length <= 0xffffff )
							length_of_length = 3;
						else if( attribute_selection_header_length <= 0xffffffff )
							length_of_length = 4;
					}		
					network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ 192 ) | ( 0 );//Universal
					network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ ( 1 << 5 ) ) | ( 1 << 5 );//Constructed
					network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ 31 ) | ( (char)XEnum_AsnUniversalClassTags_SEQUENCE );					
					if( length_of_length == 0 )
						network_buffer_ptr[ written_bytes + 1 ] = (rux::uint8)attribute_selection_header_length;
					else
					{
						network_buffer_ptr[ written_bytes + 1 ] = 0x80 + length_of_length;
						decrementer = length_of_length - 1;
						for( incrementer = 0 ; incrementer < length_of_length ; incrementer++ )
						{
							network_buffer_ptr[ written_bytes + 2 + incrementer ] = ( (char*)&attribute_selection_header_length )[ decrementer ];							 
							decrementer--;
						}
					}
					written_bytes += 2 * sizeof( char ) + length_of_length;

					for( index0 = 0 ; index0 < ldap_search_request()->_attributes.Count() ; index0++ )
					{
						size = ldap_search_request()->_attributes[ index0 ].Size() - 1;	
						length_of_length = 0;
						if( size > 0x7f )
						{
							if( size <= 0xff )
								length_of_length = 1;
							else if( size <= 0xffff )
								length_of_length = 2;
							else if( size <= 0xffffff )
								length_of_length = 3;
							else if( size <= 0xffffffff )
								length_of_length = 4;
						}	
						network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ 192 ) | ( 0 );//Universal
						network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ ( 1 << 5 ) ) | ( 0 << 5 );//Primitive
						network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ 31 ) | ( (char)XEnum_AsnUniversalClassTags_OCTETSTRING );					
						if( length_of_length == 0 )
							network_buffer_ptr[ written_bytes + 1 ] = (rux::uint8)size;
						else
						{
							network_buffer_ptr[ written_bytes + 1 ] = 0x80 + length_of_length;
							decrementer = length_of_length - 1;
							for( incrementer = 0 ; incrementer < length_of_length ; incrementer++ )
							{
								network_buffer_ptr[ written_bytes + 2 + incrementer ] = ( (char*)&size )[ decrementer ];							 
								decrementer--;
							}
						}
						written_bytes += 2 * sizeof( char ) + length_of_length;
						if( size > 0 )
							memcpy( &network_buffer_ptr[ written_bytes ] , ldap_search_request()->_attributes[ index0 ].str() , size );			
						written_bytes	+= size;
					}

					length_of_length = 0;
					if( controls_length > 0x7f )
					{
						if( controls_length <= 0xff )
							length_of_length = 1;
						else if( controls_length <= 0xffff )
							length_of_length = 2;
						else if( controls_length <= 0xffffff )
							length_of_length = 3;
						else if( controls_length <= 0xffffffff )
							length_of_length = 4;
					}		
					network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ 192 ) | ( 128 );//Universal
					network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ ( 1 << 5 ) ) | ( 1 << 5 );//Constructed
					network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ 31 ) | ( (char)0 );					
					if( length_of_length == 0 )
						network_buffer_ptr[ written_bytes + 1 ] = (rux::uint8)controls_length;
					else
					{
						network_buffer_ptr[ written_bytes + 1 ] = 0x80 + length_of_length;
						decrementer = length_of_length - 1;
						for( incrementer = 0 ; incrementer < length_of_length ; incrementer++ )
						{
							network_buffer_ptr[ written_bytes + 2 + incrementer ] = ( (char*)&controls_length )[ decrementer ];							 
							decrementer--;
						}
					}
					written_bytes += 2 * sizeof( char ) + length_of_length;
					for( index0 = 0 ; index0 < ldap_search_request()->_controls.Count() ; index0++ )
					{
						control_header_length = 0;
						size = ldap_search_request()->_controls[ index0 ]._control_type.Size() - 1;			
						if( size > 0x7f )
						{
							if( size <= 0xff )
								control_header_length += 1;
							else if( size <= 0xffff )
								control_header_length += 2;
							else if( size <= 0xffffff )
								control_header_length += 3;
							else if( size <= 0xffffffff )
								control_header_length += 4;
						}
						control_header_length += 2 * sizeof( char ) + size;
						if( ldap_search_request()->_controls[ index0 ]._criticality == 1 )			
							control_header_length += 2 * sizeof( char ) + 1;
						if( ldap_search_request()->_controls[ index0 ]._control_value.Size() - 1 > 0 )
						{
							size = ldap_search_request()->_controls[ index0 ]._control_value.Size() - 1;			
							if( size > 0x7f )
							{
								if( size <= 0xff )
									control_header_length += 1;
								else if( size <= 0xffff )
									control_header_length += 2;
								else if( size <= 0xffffff )
									control_header_length += 3;
								else if( size <= 0xffffffff )
									control_header_length += 4;
							}
							control_header_length += 2 * sizeof( char ) + size;
						}
						length_of_length = 0;
						if( control_header_length > 0x7f )
						{
							if( control_header_length <= 0xff )
								length_of_length = 1;
							else if( control_header_length <= 0xffff )
								length_of_length = 2;
							else if( control_header_length <= 0xffffff )
								length_of_length = 3;
							else if( control_header_length <= 0xffffffff )
								length_of_length = 4;
						}		
						network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ 192 ) | ( 0 );//Universal
						network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ ( 1 << 5 ) ) | ( 1 << 5 );//Constructed
						network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ 31 ) | ( (char)XEnum_AsnUniversalClassTags_SEQUENCE );					
						if( length_of_length == 0 )
							network_buffer_ptr[ written_bytes + 1 ] = (rux::uint8)control_header_length;
						else
						{
							network_buffer_ptr[ written_bytes + 1 ] = 0x80 + length_of_length;
							decrementer = length_of_length - 1;
							for( incrementer = 0 ; incrementer < length_of_length ; incrementer++ )
							{
								network_buffer_ptr[ written_bytes + 2 + incrementer ] = ( (char*)&control_header_length )[ decrementer ];							 
								decrementer--;
							}
						}
						written_bytes += 2 * sizeof( char ) + length_of_length;
						size = ldap_search_request()->_controls[ index0 ]._control_type.Size() - 1;			
						if( size > 0x7f )
						{
							if( size <= 0xff )
								length_of_length = 1;
							else if( size <= 0xffff )
								length_of_length = 2;
							else if( size <= 0xffffff )
								length_of_length = 3;
							else if( size <= 0xffffffff )
								length_of_length = 4;
						}	
						network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ 192 ) | ( 0 );//Universal
						network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ ( 1 << 5 ) ) | ( 0 << 5 );//Primitive
						network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ 31 ) | ( (char)XEnum_AsnUniversalClassTags_OCTETSTRING );					
						if( length_of_length == 0 )
							network_buffer_ptr[ written_bytes + 1 ] = (rux::uint8)size;
						else
						{
							network_buffer_ptr[ written_bytes + 1 ] = 0x80 + length_of_length;
							decrementer = length_of_length - 1;
							for( incrementer = 0 ; incrementer < length_of_length ; incrementer++ )
							{
								network_buffer_ptr[ written_bytes + 2 + incrementer ] = ( (char*)&size )[ decrementer ];							 
								decrementer--;
							}
						}
						written_bytes += 2 * sizeof( char ) + length_of_length;
						if( size > 0 )
							memcpy( &network_buffer_ptr[ written_bytes ] , ldap_search_request()->_controls[ index0 ]._control_type.str() , size );			
						written_bytes	+= size;
						if( ldap_search_request()->_controls[ index0 ]._criticality == 1 )			
						{
							network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ 192 ) | ( 0 );//Universal
							network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ ( 1 << 5 ) ) | ( 0 << 5 );//Primitive
							network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ 31 ) | ( (char)XEnum_AsnUniversalClassTags_BOOLEAN );					
							network_buffer_ptr[ written_bytes + 1 ] = (rux::uint8)1;
							if( ldap_search_request()->_types_only == 1 )
								network_buffer_ptr[ written_bytes + 2 ] = 0xff;
							else
								network_buffer_ptr[ written_bytes + 2 ] = 0;		
							written_bytes += 2 * sizeof( char ) + 1;
						}
						if( ldap_search_request()->_controls[ index0 ]._control_value.Size() - 1 > 0 )
						{
							size = ldap_search_request()->_controls[ index0 ]._control_value.Size() - 1;			
							if( size > 0x7f )
							{
								if( size <= 0xff )
									length_of_length = 1;
								else if( size <= 0xffff )
									length_of_length = 2;
								else if( size <= 0xffffff )
									length_of_length = 3;
								else if( size <= 0xffffffff )
									length_of_length = 4;
							}	
							network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ 192 ) | ( 0 );//Universal
							network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ ( 1 << 5 ) ) | ( 0 << 5 );//Primitive
							network_buffer_ptr[ written_bytes ] = ( network_buffer_ptr[ written_bytes ] & ~ 31 ) | ( (char)XEnum_AsnUniversalClassTags_OCTETSTRING );					
							if( length_of_length == 0 )
								network_buffer_ptr[ written_bytes + 1 ] = (rux::uint8)size;
							else
							{
								network_buffer_ptr[ written_bytes + 1 ] = 0x80 + length_of_length;
								decrementer = length_of_length - 1;
								for( incrementer = 0 ; incrementer < length_of_length ; incrementer++ )
								{
									network_buffer_ptr[ written_bytes + 2 + incrementer ] = ( (char*)&size )[ decrementer ];							 
									decrementer--;
								}
							}
							written_bytes += 2 * sizeof( char ) + length_of_length;
							if( size > 0 )
								memcpy( &network_buffer_ptr[ written_bytes ] , ldap_search_request()->_controls[ index0 ]._control_value.str() , size );
							written_bytes	+= size;
						}
					}	
				}		
				else
					error = "LDAP type is not implemented";		
			}
			else
				error = "Send data object has not XLdapPacket type";
		};
		rux::uint32 XLdapPacket::NetworkGetLength( rux::uint8* network_buffer_ptr , rux::uint32 network_buffer_size , rux::XString& error )
		{
			rux::uint32 length = 0;
			if( network_buffer_size >= 2 )
				{
					if( network_buffer_ptr )
					{				
						if( (rux::uint8)network_buffer_ptr[ 1 ] > (rux::uint8)0x7f )
						{
							rux::uint8 length_of_length = network_buffer_ptr[ 1 ] - 0x80;
							if( network_buffer_size >= 2 * sizeof( char ) + length_of_length )
							{
								rux::uint8 decrementer = length_of_length - 1;
								for( rux::uint8 incrementer = 0 ; incrementer < length_of_length ; incrementer++ )
								{
									( (char*)&length )[ incrementer ] = network_buffer_ptr[ 2 + decrementer];									
									decrementer--;
								}
								length += 2 * sizeof( char ) + length_of_length;
							}
							else
								error = "Length of LDAP packet length is higher received buffer size";				
						}
						else			
							length = 2 * sizeof( char ) + network_buffer_ptr[ 1 ];				
					}
					else
						error = "Receive network buffer pointer is NULL";
				}
				else
					error = "Receive network buffer size is less 2";
			return length;
		};
		rux::uint32 XLdapPacket::NetworkParse( const XObject& receive_data , rux::uint8* network_buffer_ptr , rux::uint32 network_buffer_size , rux::XString& error )
		{
			rux::uint32 readen_bytes = 0;
			if( rux_is_object( receive_data , XLdapPacket ) )
			{
				XLdapPacket ldap_packet( receive_data );		
				rux::uint32 message_id = 0;
				if( network_buffer_size >= 2 )
				{
					if( network_buffer_ptr )
					{
						rux::uint8 is_set_8bit = ( network_buffer_ptr[ 0 ] & 128 ) > 0 ? 1 : 0;
						rux::uint8 is_set_7bit = ( network_buffer_ptr[ 0 ] & 64 ) > 0 ? 1 : 0;
						XEnum_AsnClass asn_class = XEnum_AsnClass_Private;
						if( is_set_8bit && is_set_7bit )
							asn_class = XEnum_AsnClass_Private;
						else if( is_set_8bit )
							asn_class = XEnum_AsnClass_ContextSpecific;
						else if( is_set_7bit )
							asn_class = XEnum_AsnClass_Application;
						else
							asn_class = XEnum_AsnClass_Universal;
						rux::uint8 is_constructed = ( network_buffer_ptr[ 0 ] & 32 ) > 0 ? 1 : 0;
						XEnum_AsnUniversalClassTags asn_universal_class_tags = (XEnum_AsnUniversalClassTags)( network_buffer_ptr[ 0 ] & ~ 224 );
						if( asn_class == XEnum_AsnClass_Universal )
						{
							if( is_constructed == 1 )
							{
								if( asn_universal_class_tags == XEnum_AsnUniversalClassTags_SEQUENCE )
								{
									rux::uint8 total_asn_length_of_length = 0;				
									rux::uint8 decrementer = 0;							
									rux::uint32 total_asn_length = 0;
									if( (rux::uint8)network_buffer_ptr[ 1 ] > (rux::uint8)0x7f )
									{
										total_asn_length_of_length = network_buffer_ptr[ 1 ] - 0x80;
										if( network_buffer_size >= 2 * sizeof( char ) + total_asn_length_of_length )
										{
											decrementer = total_asn_length_of_length - 1;
											for( is_constructed = 0 ; is_constructed < total_asn_length_of_length ; is_constructed++ )
											{
												( (char*)&total_asn_length )[ is_constructed ] = network_buffer_ptr[ 2 + decrementer];									
												decrementer--;
											}
										}
										else
											error = "Incorrect LDAP Bind Response buffer,length of ASN length is higher receive buffer size";				
									}
									else			
										total_asn_length = network_buffer_ptr[ 1 ];
									if( error.Length() == 0 )
									{
										if( total_asn_length > 0 )
										{
											total_asn_length += 2 * sizeof( char ) + total_asn_length_of_length;
											if( total_asn_length <= network_buffer_size )
											{
												readen_bytes = 2 * sizeof( char ) + total_asn_length_of_length;
												if( readen_bytes + 2 * sizeof( char ) <= network_buffer_size )
												{
													is_set_8bit = ( network_buffer_ptr[ readen_bytes ] & 128 ) > 0 ? 1 : 0;
													is_set_7bit = ( network_buffer_ptr[ readen_bytes ] & 64 ) > 0 ? 1 : 0;
													if( is_set_8bit && is_set_7bit )
														asn_class = XEnum_AsnClass_Private;
													else if( is_set_8bit )
														asn_class = XEnum_AsnClass_ContextSpecific;
													else if( is_set_7bit )
														asn_class = XEnum_AsnClass_Application;
													else
														asn_class = XEnum_AsnClass_Universal;
													is_constructed = ( network_buffer_ptr[ readen_bytes ] & 32 ) > 0 ? 1 : 0;
													asn_universal_class_tags = (XEnum_AsnUniversalClassTags)( network_buffer_ptr[ readen_bytes ] & ~ 224 );
													if( asn_class == XEnum_AsnClass_Universal )
													{
														if( is_constructed == 0 )
														{
															if( asn_universal_class_tags == XEnum_AsnUniversalClassTags_INTEGER )
															{
																rux::uint32 asn_length = 0;
																rux::uint8 asn_length_of_length = 0;
																if( (rux::uint8)network_buffer_ptr[ readen_bytes + 1 ] > (rux::uint8)0x7f )
																{
																	asn_length_of_length = network_buffer_ptr[ readen_bytes + 1 ] - 0x80;
																	if( network_buffer_size >= readen_bytes + 2 * sizeof( char ) + asn_length_of_length )
																	{
																		decrementer = asn_length_of_length - 1;
																		for( is_constructed = 0 ; is_constructed < asn_length_of_length ; is_constructed++ )
																		{
																			( (char*)&asn_length )[ is_constructed ] = network_buffer_ptr[ readen_bytes + 2 + decrementer];									
																			decrementer--;
																		}
																	}
																	else
																		error = "Incorrect LDAP Bind Response buffer,length of ASN length is higher receive buffer size";				
																}
																else	
																	asn_length = network_buffer_ptr[ readen_bytes + 1 ];
																if( error.Length() == 0 )
																{
																	if( asn_length <= network_buffer_size - readen_bytes - 2 * sizeof( char ) - asn_length_of_length )
																	{
																		if( asn_length <= sizeof( message_id ) )
																		{
																			readen_bytes += 2 * sizeof( char ) + asn_length_of_length;
																			message_id = 0;																	
																			if( asn_length > 0 )
																			{
																				decrementer = static_cast<rux::uint8>( asn_length - 1 );
																				for( is_constructed = 0 ; is_constructed < asn_length ; is_constructed++ )
																				{
																					((char*)&message_id)[ is_constructed ] = network_buffer_ptr[ readen_bytes + decrementer];
																					decrementer--;
																				}
																			}
																			readen_bytes += asn_length;
																			if( readen_bytes < total_asn_length )
																			{
																				if( readen_bytes + 2 * sizeof( char ) <= network_buffer_size )
																				{
																					is_set_8bit = ( network_buffer_ptr[ readen_bytes ] & 128 ) > 0 ? 1 : 0;
																					is_set_7bit = ( network_buffer_ptr[ readen_bytes ] & 64 ) > 0 ? 1 : 0;
																					if( is_set_8bit && is_set_7bit )
																						asn_class = XEnum_AsnClass_Private;
																					else if( is_set_8bit )
																						asn_class = XEnum_AsnClass_ContextSpecific;
																					else if( is_set_7bit )
																						asn_class = XEnum_AsnClass_Application;
																					else
																						asn_class = XEnum_AsnClass_Universal;
																					is_constructed = ( network_buffer_ptr[ readen_bytes ] & 32 ) > 0 ? 1 : 0;
																					asn_universal_class_tags = (XEnum_AsnUniversalClassTags)( network_buffer_ptr[ readen_bytes ] & ~ 224 );
																					if( asn_class == XEnum_AsnClass_Application )
																					{
																						if( is_constructed == 1 )
																						{																					
																							if( asn_universal_class_tags == 1 )
																							{
																								XLdapBindResponse ldap_bind_response;
																								ldap_bind_response()->_message_id = message_id;
																								asn_length = 0;
																								asn_length_of_length = 0;
																								if( (rux::uint8)network_buffer_ptr[ readen_bytes + 1 ] > (rux::uint8)0x7f )
																								{
																									asn_length_of_length = network_buffer_ptr[ readen_bytes + 1 ] - 0x80;
																									if( network_buffer_size >= readen_bytes + 2 * sizeof( char ) + asn_length_of_length )
																									{
																										decrementer = asn_length_of_length - 1;
																										for( is_constructed = 0 ; is_constructed < asn_length_of_length ; is_constructed++ )
																										{
																											( (char*)&asn_length )[ is_constructed ] = network_buffer_ptr[ readen_bytes + 2 + decrementer];									
																											decrementer--;
																										}
																									}
																									else
																										error = "Incorrect LDAP Bind Response buffer,length of ASN length is higher receive buffer size";				
																								}
																								else	
																									asn_length = network_buffer_ptr[ readen_bytes + 1 ];
																								if( error.Length() == 0 )
																								{
																									if( asn_length <= network_buffer_size - readen_bytes - 2 * sizeof( char ) - asn_length_of_length )
																									{																						
																										if( readen_bytes + asn_length == total_asn_length - 2 * sizeof( char ) - total_asn_length_of_length )
																										{
																											readen_bytes += 2 * sizeof( char ) + asn_length_of_length;				
																											if( readen_bytes < total_asn_length )
																											{
																												if( readen_bytes + 2 * sizeof( char ) <= network_buffer_size )
																												{
																													is_set_8bit = ( network_buffer_ptr[ readen_bytes ] & 128 ) > 0 ? 1 : 0;
																													is_set_7bit = ( network_buffer_ptr[ readen_bytes ] & 64 ) > 0 ? 1 : 0;
																													if( is_set_8bit && is_set_7bit )
																														asn_class = XEnum_AsnClass_Private;
																													else if( is_set_8bit )
																														asn_class = XEnum_AsnClass_ContextSpecific;
																													else if( is_set_7bit )
																														asn_class = XEnum_AsnClass_Application;
																													else
																														asn_class = XEnum_AsnClass_Universal;
																													is_constructed = ( network_buffer_ptr[ readen_bytes ] & 32 ) > 0 ? 1 : 0;
																													asn_universal_class_tags = (XEnum_AsnUniversalClassTags)( network_buffer_ptr[ readen_bytes ] & ~ 224 );
																													if( asn_class == XEnum_AsnClass_Universal )
																													{
																														if( is_constructed == 0 )
																														{
																															if( asn_universal_class_tags == XEnum_AsnUniversalClassTags_ENUMERATED )
																															{
																																asn_length = 0;
																																asn_length_of_length = 0;
																																if( (rux::uint8)network_buffer_ptr[ readen_bytes + 1 ] > (rux::uint8)0x7f )
																																{
																																	asn_length_of_length = network_buffer_ptr[ readen_bytes + 1 ] - 0x80;
																																	if( network_buffer_size >= readen_bytes + 2 * sizeof( char ) + asn_length_of_length )
																																	{
																																		decrementer = asn_length_of_length - 1;
																																		for( is_constructed = 0 ; is_constructed < asn_length_of_length ; is_constructed++ )
																																		{
																																			( (char*)&asn_length )[ is_constructed ] = network_buffer_ptr[ readen_bytes + 2 + decrementer];									
																																			decrementer--;
																																		}
																																	}
																																	else
																																		error = "Incorrect LDAP Bind Response buffer,length of ASN length is higher receive buffer size";				
																																}
																																else	
																																	asn_length = network_buffer_ptr[ readen_bytes + 1 ];
																																if( error.Length() == 0 )
																																{
																																	if( asn_length <= network_buffer_size - readen_bytes - 2 * sizeof( char ) - asn_length_of_length )
																																	{
																																		if( asn_length <= sizeof( ldap_bind_response()->_result_code ) )
																																		{
																																			readen_bytes += 2 * sizeof( char ) + asn_length_of_length;
																																			ldap_bind_response()->_result_code = XEnum_LdapResultCode_success;
																																			if( asn_length > 0 )
																																			{
																																				decrementer = static_cast<rux::uint8>( asn_length - 1 );
																																				for( is_constructed = 0 ; is_constructed < asn_length ; is_constructed++ )
																																				{
																																					((char*)&ldap_bind_response()->_result_code)[ is_constructed ] = network_buffer_ptr[ readen_bytes + decrementer];
																																					decrementer--;
																																				}
																																			}
																																			readen_bytes += asn_length;
																																			if( readen_bytes < total_asn_length )
																																			{
																																				if( readen_bytes + 2 * sizeof( char ) <= network_buffer_size )
																																				{
																																					is_set_8bit = ( network_buffer_ptr[ readen_bytes ] & 128 ) > 0 ? 1 : 0;
																																					is_set_7bit = ( network_buffer_ptr[ readen_bytes ] & 64 ) > 0 ? 1 : 0;
																																					if( is_set_8bit && is_set_7bit )
																																						asn_class = XEnum_AsnClass_Private;
																																					else if( is_set_8bit )
																																						asn_class = XEnum_AsnClass_ContextSpecific;
																																					else if( is_set_7bit )
																																						asn_class = XEnum_AsnClass_Application;
																																					else
																																						asn_class = XEnum_AsnClass_Universal;
																																					is_constructed = ( network_buffer_ptr[ readen_bytes ] & 32 ) > 0 ? 1 : 0;
																																					asn_universal_class_tags = (XEnum_AsnUniversalClassTags)( network_buffer_ptr[ readen_bytes ] & ~ 224 );
																																					if( asn_class == XEnum_AsnClass_Universal )
																																					{
																																						if( is_constructed == 0 )
																																						{
																																							if( asn_universal_class_tags == XEnum_AsnUniversalClassTags_OCTETSTRING )
																																							{
																																								asn_length = 0;
																																								asn_length_of_length = 0;
																																								if( (rux::uint8)network_buffer_ptr[ readen_bytes + 1 ] > (rux::uint8)0x7f )
																																								{
																																									asn_length_of_length = network_buffer_ptr[ readen_bytes + 1 ] - 0x80;
																																									if( network_buffer_size >= readen_bytes + 2 * sizeof( char ) + asn_length_of_length )
																																									{
																																										decrementer = asn_length_of_length - 1;
																																										for( is_constructed = 0 ; is_constructed < asn_length_of_length ; is_constructed++ )
																																										{
																																											( (char*)&asn_length )[ is_constructed ] = network_buffer_ptr[ readen_bytes + 2 + decrementer];									
																																											decrementer--;
																																										}
																																									}
																																									else
																																										error = "Incorrect LDAP Bind Response buffer,length of ASN length is higher receive buffer size";				
																																								}
																																								else	
																																									asn_length = network_buffer_ptr[ readen_bytes + 1 ];
																																								if( error.Length() == 0 )
																																								{
																																									if( asn_length <= network_buffer_size - readen_bytes - 2 * sizeof( char ) - asn_length_of_length )
																																									{																														
																																										readen_bytes += 2 * sizeof( char ) + asn_length_of_length;																							
																																										if( asn_length > 0 )
																																											ldap_bind_response()->_matched_dn = rux::XString( (char*)&network_buffer_ptr[ readen_bytes ] , 0 , asn_length , XEnumCodePage_UTF8 );
																																										else 
																																											ldap_bind_response()->_matched_dn.Clear();
																																										readen_bytes += asn_length;
																																										if( readen_bytes < total_asn_length )
																																										{
																																											if( readen_bytes + 2 * sizeof( char ) <= network_buffer_size )
																																											{
																																												is_set_8bit = ( network_buffer_ptr[ readen_bytes ] & 128 ) > 0 ? 1 : 0;
																																												is_set_7bit = ( network_buffer_ptr[ readen_bytes ] & 64 ) > 0 ? 1 : 0;
																																												if( is_set_8bit && is_set_7bit )
																																													asn_class = XEnum_AsnClass_Private;
																																												else if( is_set_8bit )
																																													asn_class = XEnum_AsnClass_ContextSpecific;
																																												else if( is_set_7bit )
																																													asn_class = XEnum_AsnClass_Application;
																																												else
																																													asn_class = XEnum_AsnClass_Universal;
																																												is_constructed = ( network_buffer_ptr[ readen_bytes ] & 32 ) > 0 ? 1 : 0;
																																												asn_universal_class_tags = (XEnum_AsnUniversalClassTags)( network_buffer_ptr[ readen_bytes ] & ~ 224 );
																																												if( asn_class == XEnum_AsnClass_Universal )
																																												{
																																													if( is_constructed == 0 )
																																													{
																																														if( asn_universal_class_tags == XEnum_AsnUniversalClassTags_OCTETSTRING )
																																														{
																																															asn_length = 0;
																																															asn_length_of_length = 0;
																																															if( (rux::uint8)network_buffer_ptr[ readen_bytes + 1 ] > (rux::uint8)0x7f )
																																															{
																																																asn_length_of_length = network_buffer_ptr[ readen_bytes + 1 ] - 0x80;
																																																if( network_buffer_size >= readen_bytes + 2 * sizeof( char ) + asn_length_of_length )
																																																{
																																																	decrementer = asn_length_of_length - 1;
																																																	for( is_constructed = 0 ; is_constructed < asn_length_of_length ; is_constructed++ )
																																																	{
																																																		( (char*)&asn_length )[ is_constructed ] = network_buffer_ptr[ readen_bytes + 2 + decrementer];									
																																																		decrementer--;
																																																	}
																																																}
																																																else
																																																	error = "Incorrect LDAP Bind Response buffer,length of ASN length is higher receive buffer size";				
																																															}
																																															else	
																																																asn_length = network_buffer_ptr[ readen_bytes + 1 ];
																																															if( error.Length() == 0 )
																																															{
																																																if( asn_length <= network_buffer_size - readen_bytes - 2 * sizeof( char ) - asn_length_of_length )
																																																{																														
																																																	readen_bytes += 2 * sizeof( char ) + asn_length_of_length;																							
																																																	if( asn_length > 0 )
																																																		ldap_bind_response()->_error_message = rux::XString( (char*)&network_buffer_ptr[ readen_bytes ] , 0 , asn_length , XEnumCodePage_UTF8 );
																																																	else 
																																																		ldap_bind_response()->_error_message.Clear();
																																																	readen_bytes += asn_length;
																																																	if( readen_bytes < total_asn_length )
																																																	{
																																																
																																																		if( readen_bytes + 2 * sizeof( char ) <= network_buffer_size )
																																																		{
																																																			is_set_8bit = ( network_buffer_ptr[ readen_bytes ] & 128 ) > 0 ? 1 : 0;
																																																			is_set_7bit = ( network_buffer_ptr[ readen_bytes ] & 64 ) > 0 ? 1 : 0;
																																																			if( is_set_8bit && is_set_7bit )
																																																				asn_class = XEnum_AsnClass_Private;
																																																			else if( is_set_8bit )
																																																				asn_class = XEnum_AsnClass_ContextSpecific;
																																																			else if( is_set_7bit )
																																																				asn_class = XEnum_AsnClass_Application;
																																																			else
																																																				asn_class = XEnum_AsnClass_Universal;
																																																			is_constructed = ( network_buffer_ptr[ readen_bytes ] & 32 ) > 0 ? 1 : 0;
																																																			asn_universal_class_tags = (XEnum_AsnUniversalClassTags)( network_buffer_ptr[ readen_bytes ] & ~ 224 );
																																																			if( asn_class == XEnum_AsnClass_Universal )
																																																			{
																																																				if( is_constructed == 0 )
																																																				{
																																																					if( asn_universal_class_tags == XEnum_AsnUniversalClassTags_OCTETSTRING )
																																																					{
																																																						asn_length = 0;
																																																						asn_length_of_length = 0;
																																																						if( (rux::uint8)network_buffer_ptr[ readen_bytes + 1 ] > (rux::uint8)0x7f )
																																																						{
																																																							asn_length_of_length = network_buffer_ptr[ readen_bytes + 1 ] - 0x80;
																																																							if( network_buffer_size >= readen_bytes + 2 * sizeof( char ) + asn_length_of_length )
																																																							{
																																																								decrementer = asn_length_of_length - 1;
																																																								for( is_constructed = 0 ; is_constructed < asn_length_of_length ; is_constructed++ )
																																																								{
																																																									( (char*)&asn_length )[ is_constructed ] = network_buffer_ptr[ readen_bytes + 2 + decrementer];									
																																																									decrementer--;
																																																								}
																																																							}
																																																							else
																																																								error = "Incorrect LDAP Bind Response buffer,length of ASN length is higher receive buffer size";				
																																																						}
																																																						else	
																																																							asn_length = network_buffer_ptr[ readen_bytes + 1 ];
																																																						if( error.Length() == 0 )
																																																						{
																																																							if( asn_length <= network_buffer_size - readen_bytes - 2 * sizeof( char ) - asn_length_of_length )
																																																							{																														
																																																								readen_bytes += 2 * sizeof( char ) + asn_length_of_length;																							
																																																								if( asn_length > 0 )
																																																									ldap_bind_response()->_server_sasl_creds = rux::XString( (char*)&network_buffer_ptr[ readen_bytes ] , 0 , asn_length , XEnumCodePage_UTF8 );
																																																								else 
																																																									ldap_bind_response()->_server_sasl_creds.Clear();
																																																								readen_bytes += asn_length;
																																																								if( readen_bytes != total_asn_length )
																																																									error = "Incorrect LDAP Bind Response buffer,readen ASN bytes is not equal ASN total length";																																																								
																																																							}
																																																							else 
																																																								error = "Incorrect LDAP Bind Response buffer,received network buffer length less ASN total length";
																																																						}
																																																					}
																																																					else
																																																						error = "Incorrect LDAP Bind Response buffer,incorrect ASN universal class tag";
																																																				}
																																																				else
																																																					error = "Incorrect LDAP Bind Response buffer,incorrect ASN Primitive/Constructed";																													
																																																			}
																																																			else
																																																				error = "Incorrect LDAP Bind Response buffer,incorrect ASN class";
																																																		}
																																																		else 
																																																			error = "Incorrect LDAP Bind Response buffer,received network buffer length less ASN total length";
																																																	}
																																																	else if( readen_bytes != total_asn_length )
																																																		error = "Incorrect LDAP Bind Response buffer,readen ASN bytes is not equal ASN total length";																																														
																																																}
																																																else 
																																																	error = "Incorrect LDAP Bind Response buffer,received network buffer length less ASN total length";
																																															}
																																														}
																																														else
																																															error = "Incorrect LDAP Bind Response buffer,incorrect ASN universal class tag";
																																													}
																																													else
																																														error = "Incorrect LDAP Bind Response buffer,incorrect ASN Primitive/Constructed";
																																												}
																																												else
																																													error = "Incorrect LDAP Bind Response buffer,incorrect ASN class";
																																											}
																																											else 
																																												error = "Incorrect LDAP Bind Response buffer,received network buffer length less ASN total length";
																																										}
																																										else if( readen_bytes > total_asn_length )
																																											error = "Incorrect LDAP Bind Response buffer,readen ASN bytes length more ASN total length";
																																										else
																																											error = "Incorrect LDAP Bind Response buffer,ASN total length is too small ";
																																									}
																																									else 
																																										error = "Incorrect LDAP Bind Response buffer,received network buffer length less ASN total length";
																																								}
																																							}
																																							else
																																								error = "Incorrect LDAP Bind Response buffer,incorrect ASN universal class tag";
																																						}
																																						else
																																							error = "Incorrect LDAP Bind Response buffer,incorrect ASN Primitive/Constructed";
																																					}
																																					else
																																						error = "Incorrect LDAP Bind Response buffer,incorrect ASN class";
																																				}
																																				else 
																																					error = "Incorrect LDAP Bind Response buffer,received network buffer length less ASN total length";
																																			}
																																			else if( readen_bytes > total_asn_length )
																																				error = "Incorrect LDAP Bind Response buffer,readen ASN bytes length more ASN total length";
																																			else
																																				error = "Incorrect LDAP Bind Response buffer,ASN total length is too small ";
																																		}
																																		else
																																			error = "Incorrect LDAP Bind Response buffer,ASN length more size of Result code";
																																	}
																																	else 
																																		error = "Incorrect LDAP Bind Response buffer,received network buffer length less ASN total length";
																																}
																															}
																															else
																																error = "Incorrect LDAP Bind Response buffer,incorrect ASN universal class tag";																																																											
																														}
																														else
																															error = "Incorrect LDAP Bind Response buffer,incorrect ASN Primitive/Constructed";																													
																													}
																													else
																														error = "Incorrect LDAP Bind Response buffer,incorrect ASN class";
																												}
																												else 
																													error = "Incorrect LDAP Bind Response buffer,received network buffer length less ASN total length";
																											}
																											else if( readen_bytes > total_asn_length )
																												error = "Incorrect LDAP Bind Response buffer,readen ASN bytes length more ASN total length";
																											else
																												error = "Incorrect LDAP Bind Response buffer,ASN total length is too small ";
																										}
																										else
																											error = "Incorrect LDAP Bind Response buffer,internal ASN length is not equal total ASN length";																						
																									}
																									else 
																										error = "Incorrect LDAP Bind Response buffer,received network buffer length less ASN total length";
																								}
																								if( error.Length() == 0 )
																									ldap_packet()->_ldap_content = ldap_bind_response;
																							}
																							else if( asn_universal_class_tags == 4 )
																							{
																								XLdapSearchResultEntry ldap_search_result_entry;
																								ldap_search_result_entry()->_message_id = message_id;
																								asn_length = 0;
																								asn_length_of_length = 0;
																								if( (rux::uint8)network_buffer_ptr[ readen_bytes + 1 ] > (rux::uint8)0x7f )
																								{
																									asn_length_of_length = network_buffer_ptr[ readen_bytes + 1 ] - 0x80;
																									if( network_buffer_size >= readen_bytes + 2 * sizeof( char ) + asn_length_of_length )
																									{
																										decrementer = asn_length_of_length - 1;
																										for( is_constructed = 0 ; is_constructed < asn_length_of_length ; is_constructed++ )
																										{
																											( (char*)&asn_length )[ is_constructed ] = network_buffer_ptr[ readen_bytes + 2 + decrementer];									
																											decrementer--;
																										}
																									}
																									else
																										error = "Incorrect LDAP Search Result Entry buffer,length of ASN length is higher receive buffer size";				
																								}
																								else	
																									asn_length = network_buffer_ptr[ readen_bytes + 1 ];
																								if( error.Length() == 0 )
																								{
																									if( asn_length <= network_buffer_size - readen_bytes - 2 * sizeof( char ) - asn_length_of_length )
																									{																						
																										if( readen_bytes + asn_length == total_asn_length - 2 * sizeof( char ) - total_asn_length_of_length )
																										{
																											readen_bytes += 2 * sizeof( char ) + asn_length_of_length;				
																											if( readen_bytes < total_asn_length )
																											{
																												if( readen_bytes + 2 * sizeof( char ) <= network_buffer_size )
																												{
																													is_set_8bit = ( network_buffer_ptr[ readen_bytes ] & 128 ) > 0 ? 1 : 0;
																													is_set_7bit = ( network_buffer_ptr[ readen_bytes ] & 64 ) > 0 ? 1 : 0;
																													if( is_set_8bit && is_set_7bit )
																														asn_class = XEnum_AsnClass_Private;
																													else if( is_set_8bit )
																														asn_class = XEnum_AsnClass_ContextSpecific;
																													else if( is_set_7bit )
																														asn_class = XEnum_AsnClass_Application;
																													else
																														asn_class = XEnum_AsnClass_Universal;
																													is_constructed = ( network_buffer_ptr[ readen_bytes ] & 32 ) > 0 ? 1 : 0;
																													asn_universal_class_tags = (XEnum_AsnUniversalClassTags)( network_buffer_ptr[ readen_bytes ] & ~ 224 );
																													if( asn_class == XEnum_AsnClass_Universal )
																													{
																														if( is_constructed == 0 )
																														{
																															if( asn_universal_class_tags == XEnum_AsnUniversalClassTags_OCTETSTRING )
																															{
																																asn_length = 0;
																																asn_length_of_length = 0;
																																if( (rux::uint8)network_buffer_ptr[ readen_bytes + 1 ] > (rux::uint8)0x7f )
																																{
																																	asn_length_of_length = network_buffer_ptr[ readen_bytes + 1 ] - 0x80;
																																	if( network_buffer_size >= readen_bytes + 2 * sizeof( char ) + asn_length_of_length )
																																	{
																																		decrementer = asn_length_of_length - 1;
																																		for( is_constructed = 0 ; is_constructed < asn_length_of_length ; is_constructed++ )
																																		{
																																			( (char*)&asn_length )[ is_constructed ] = network_buffer_ptr[ readen_bytes + 2 + decrementer];									
																																			decrementer--;
																																		}
																																	}
																																	else
																																		error = "Incorrect LDAP Search Result Entry buffer,length of ASN length is higher receive buffer size";				
																																}
																																else	
																																	asn_length = network_buffer_ptr[ readen_bytes + 1 ];
																																if( error.Length() == 0 )
																																{
																																	if( asn_length <= network_buffer_size - readen_bytes - 2 * sizeof( char ) - asn_length_of_length )
																																	{																														
																																		readen_bytes += 2 * sizeof( char ) + asn_length_of_length;																							
																																		if( asn_length > 0 )
																																			ldap_search_result_entry()->_object_name = rux::XString( (char*)&network_buffer_ptr[ readen_bytes ] , 0 , asn_length , XEnumCodePage_UTF8 );
																																		else 
																																			ldap_search_result_entry()->_object_name.Clear();
																																		readen_bytes += asn_length;
																																		if( readen_bytes < total_asn_length )
																																		{
																																			if( readen_bytes + 2 * sizeof( char ) <= network_buffer_size )
																																			{
																																				is_set_8bit = ( network_buffer_ptr[ readen_bytes ] & 128 ) > 0 ? 1 : 0;
																																				is_set_7bit = ( network_buffer_ptr[ readen_bytes ] & 64 ) > 0 ? 1 : 0;
																																				if( is_set_8bit && is_set_7bit )
																																					asn_class = XEnum_AsnClass_Private;
																																				else if( is_set_8bit )
																																					asn_class = XEnum_AsnClass_ContextSpecific;
																																				else if( is_set_7bit )
																																					asn_class = XEnum_AsnClass_Application;
																																				else
																																					asn_class = XEnum_AsnClass_Universal;
																																				is_constructed = ( network_buffer_ptr[ readen_bytes ] & 32 ) > 0 ? 1 : 0;
																																				asn_universal_class_tags = (XEnum_AsnUniversalClassTags)( network_buffer_ptr[ readen_bytes ] & ~ 224 );
																																				if( asn_class == XEnum_AsnClass_Universal )
																																				{
																																					if( is_constructed == 1 )
																																					{
																																						if( asn_universal_class_tags == XEnum_AsnUniversalClassTags_SEQUENCE )
																																						{
																																							rux::uint32 attributes_asn_length = 0;
																																							asn_length_of_length = 0;
																																							if( (rux::uint8)network_buffer_ptr[ readen_bytes + 1 ] > (rux::uint8)0x7f )
																																							{
																																								asn_length_of_length = network_buffer_ptr[ readen_bytes + 1 ] - 0x80;
																																								if( network_buffer_size >= readen_bytes + 2 * sizeof( char ) + asn_length_of_length )
																																								{
																																									decrementer = asn_length_of_length - 1;
																																									for( is_constructed = 0 ; is_constructed < asn_length_of_length ; is_constructed++ )
																																									{
																																										( (char*)&attributes_asn_length )[ is_constructed ] = network_buffer_ptr[ readen_bytes + 2 + decrementer];									
																																										decrementer--;
																																									}
																																								}
																																								else
																																									error = "Incorrect LDAP Search Result Entry buffer,length of ASN length is higher receive buffer size";				
																																							}
																																							else	
																																								attributes_asn_length = network_buffer_ptr[ readen_bytes + 1 ];
																																							if( error.Length() == 0 )
																																							{
																																								if( attributes_asn_length <= network_buffer_size - readen_bytes - 2 * sizeof( char ) - asn_length_of_length )
																																								{																														
																																									readen_bytes += 2 * sizeof( char ) + asn_length_of_length;																							
																																									if( attributes_asn_length > 0 )
																																									{
																																										rux::uint32 partial_asn_length = 0;
																																										XLdapPartialAttribute ldap_partial_attribute;
																																										rux::XString attribute;
																																										while( attributes_asn_length > 0 )
																																										{
																																											if( readen_bytes < total_asn_length )
																																											{
																																												if( readen_bytes + 2 * sizeof( char ) <= network_buffer_size )
																																												{
																																													is_set_8bit = ( network_buffer_ptr[ readen_bytes ] & 128 ) > 0 ? 1 : 0;
																																													is_set_7bit = ( network_buffer_ptr[ readen_bytes ] & 64 ) > 0 ? 1 : 0;
																																													if( is_set_8bit && is_set_7bit )
																																														asn_class = XEnum_AsnClass_Private;
																																													else if( is_set_8bit )
																																														asn_class = XEnum_AsnClass_ContextSpecific;
																																													else if( is_set_7bit )
																																														asn_class = XEnum_AsnClass_Application;
																																													else
																																														asn_class = XEnum_AsnClass_Universal;
																																													is_constructed = ( network_buffer_ptr[ readen_bytes ] & 32 ) > 0 ? 1 : 0;
																																													asn_universal_class_tags = (XEnum_AsnUniversalClassTags)( network_buffer_ptr[ readen_bytes ] & ~ 224 );
																																													if( asn_class == XEnum_AsnClass_Universal )
																																													{
																																														if( is_constructed == 1 )
																																														{
																																															if( asn_universal_class_tags == XEnum_AsnUniversalClassTags_SEQUENCE )
																																															{
																																																partial_asn_length = 0;
																																																asn_length_of_length = 0;
																																																if( (rux::uint8)network_buffer_ptr[ readen_bytes + 1 ] > (rux::uint8)0x7f )
																																																{
																																																	asn_length_of_length = network_buffer_ptr[ readen_bytes + 1 ] - 0x80;
																																																	if( network_buffer_size >= readen_bytes + 2 * sizeof( char ) + asn_length_of_length )
																																																	{
																																																		decrementer = asn_length_of_length - 1;
																																																		for( is_constructed = 0 ; is_constructed < asn_length_of_length ; is_constructed++ )
																																																		{
																																																			( (char*)&partial_asn_length )[ is_constructed ] = network_buffer_ptr[ readen_bytes + 2 + decrementer];									
																																																			decrementer--;
																																																		}
																																																	}
																																																	else
																																																		error = "Incorrect LDAP Search Result Entry buffer,length of ASN length is higher receive buffer size";				
																																																}
																																																else	
																																																	partial_asn_length = network_buffer_ptr[ readen_bytes + 1 ];
																																																if( error.Length() == 0 )
																																																{
																																																	if( partial_asn_length <= network_buffer_size - readen_bytes - 2 * sizeof( char ) - asn_length_of_length )
																																																	{																														
																																																		readen_bytes += 2 * sizeof( char ) + asn_length_of_length;			
																																																		if( attributes_asn_length >= 2 * sizeof( char ) + asn_length_of_length )
																																																			attributes_asn_length -= 2 * sizeof( char ) + asn_length_of_length;
																																																		else
																																																			error = "Incorrect LDAP Search Result Entry buffer,readen ASN bytes length more ASN total length";
																																																		if( error.Length() == 0 )
																																																		{
																																																			if( readen_bytes < total_asn_length )
																																																			{
																																																				if( readen_bytes + 2 * sizeof( char ) <= network_buffer_size )
																																																				{
																																																					is_set_8bit = ( network_buffer_ptr[ readen_bytes ] & 128 ) > 0 ? 1 : 0;
																																																					is_set_7bit = ( network_buffer_ptr[ readen_bytes ] & 64 ) > 0 ? 1 : 0;
																																																					if( is_set_8bit && is_set_7bit )
																																																						asn_class = XEnum_AsnClass_Private;
																																																					else if( is_set_8bit )
																																																						asn_class = XEnum_AsnClass_ContextSpecific;
																																																					else if( is_set_7bit )
																																																						asn_class = XEnum_AsnClass_Application;
																																																					else
																																																						asn_class = XEnum_AsnClass_Universal;
																																																					is_constructed = ( network_buffer_ptr[ readen_bytes ] & 32 ) > 0 ? 1 : 0;
																																																					asn_universal_class_tags = (XEnum_AsnUniversalClassTags)( network_buffer_ptr[ readen_bytes ] & ~ 224 );
																																																					if( asn_class == XEnum_AsnClass_Universal )
																																																					{
																																																						if( is_constructed == 0 )
																																																						{
																																																							if( asn_universal_class_tags == XEnum_AsnUniversalClassTags_OCTETSTRING )
																																																							{
																																																								asn_length = 0;
																																																								asn_length_of_length = 0;
																																																								if( (rux::uint8)network_buffer_ptr[ readen_bytes + 1 ] > (rux::uint8)0x7f )
																																																								{
																																																									asn_length_of_length = network_buffer_ptr[ readen_bytes + 1 ] - 0x80;
																																																									if( network_buffer_size >= readen_bytes + 2 * sizeof( char ) + asn_length_of_length )
																																																									{
																																																										decrementer = asn_length_of_length - 1;
																																																										for( is_constructed = 0 ; is_constructed < asn_length_of_length ; is_constructed++ )
																																																										{
																																																											( (char*)&asn_length )[ is_constructed ] = network_buffer_ptr[ readen_bytes + 2 + decrementer];									
																																																											decrementer--;
																																																										}																																																		
																																																									}
																																																									else
																																																										error = "Incorrect LDAP Search Result Entry buffer,length of ASN length is higher receive buffer size";				
																																																								}
																																																								else	
																																																									asn_length = network_buffer_ptr[ readen_bytes + 1 ];
																																																								if( error.Length() == 0 )
																																																								{
																																																									if( asn_length <= network_buffer_size - readen_bytes - 2 * sizeof( char ) - asn_length_of_length )
																																																									{																														
																																																										readen_bytes += 2 * sizeof( char ) + asn_length_of_length;			
																																																										ldap_partial_attribute = XLdapPartialAttribute();
																																																										if( asn_length > 0 )
																																																											ldap_partial_attribute()->_attribute_type = rux::XString( (char*)&network_buffer_ptr[ readen_bytes ] , 0 , asn_length , XEnumCodePage_UTF8 );
																																																										else 
																																																											ldap_partial_attribute()->_attribute_type.Clear();
																																																										readen_bytes += asn_length;
																																																										if( attributes_asn_length >= 2 * sizeof( char ) + asn_length_of_length + asn_length )
																																																											attributes_asn_length -= 2 * sizeof( char ) + asn_length_of_length + asn_length;
																																																										else
																																																											error = "Incorrect LDAP Search Result Entry buffer,readen ASN bytes length more ASN total length";
																																																										if( error.Length() == 0 )
																																																										{
																																																											if( readen_bytes < total_asn_length )
																																																											{
																																																												if( readen_bytes + 2 * sizeof( char ) <= network_buffer_size )
																																																												{
																																																													is_set_8bit = ( network_buffer_ptr[ readen_bytes ] & 128 ) > 0 ? 1 : 0;
																																																													is_set_7bit = ( network_buffer_ptr[ readen_bytes ] & 64 ) > 0 ? 1 : 0;
																																																													if( is_set_8bit && is_set_7bit )
																																																														asn_class = XEnum_AsnClass_Private;
																																																													else if( is_set_8bit )
																																																														asn_class = XEnum_AsnClass_ContextSpecific;
																																																													else if( is_set_7bit )
																																																														asn_class = XEnum_AsnClass_Application;
																																																													else
																																																														asn_class = XEnum_AsnClass_Universal;
																																																													is_constructed = ( network_buffer_ptr[ readen_bytes ] & 32 ) > 0 ? 1 : 0;
																																																													asn_universal_class_tags = (XEnum_AsnUniversalClassTags)( network_buffer_ptr[ readen_bytes ] & ~ 224 );
																																																													if( asn_class == XEnum_AsnClass_Universal )
																																																													{
																																																														if( is_constructed == 1 )
																																																														{
																																																															if( asn_universal_class_tags == XEnum_AsnUniversalClassTags_SET )
																																																															{
																																																																partial_asn_length = 0;
																																																																asn_length_of_length = 0;
																																																																if( (rux::uint8)network_buffer_ptr[ readen_bytes + 1 ] > (rux::uint8)0x7f )
																																																																{
																																																																	asn_length_of_length = network_buffer_ptr[ readen_bytes + 1 ] - 0x80;
																																																																	if( network_buffer_size >= readen_bytes + 2 * sizeof( char ) + asn_length_of_length )
																																																																	{
																																																																		decrementer = asn_length_of_length - 1;
																																																																		for( is_constructed = 0 ; is_constructed < asn_length_of_length ; is_constructed++ )
																																																																		{
																																																																			( (char*)&partial_asn_length )[ is_constructed ] = network_buffer_ptr[ readen_bytes + 2 + decrementer];									
																																																																			decrementer--;
																																																																		}																																																										
																																																																	}
																																																																	else
																																																																		error = "Incorrect LDAP Search Result Entry buffer,length of ASN length is higher receive buffer size";				
																																																																}
																																																																else	
																																																																	partial_asn_length = network_buffer_ptr[ readen_bytes + 1 ];
																																																																if( error.Length() == 0 )
																																																																{
																																																																	if( partial_asn_length <= network_buffer_size - readen_bytes - 2 * sizeof( char ) - asn_length_of_length )
																																																																	{																														
																																																																		readen_bytes += 2 * sizeof( char ) + asn_length_of_length;																																																										
																																																																		if( attributes_asn_length >= 2 * sizeof( char ) + asn_length_of_length )
																																																																			attributes_asn_length -= 2 * sizeof( char ) + asn_length_of_length;
																																																																		else
																																																																			error = "Incorrect LDAP Search Result Entry buffer,readen ASN bytes length more ASN total length";
																																																																		if( error.Length() == 0 )
																																																																		{
																																																																			while( partial_asn_length > 0 )
																																																																			{
																																																																				if( readen_bytes < total_asn_length )
																																																																				{
																																																																					if( readen_bytes + 2 * sizeof( char ) <= network_buffer_size )
																																																																					{
																																																																						is_set_8bit = ( network_buffer_ptr[ readen_bytes ] & 128 ) > 0 ? 1 : 0;
																																																																						is_set_7bit = ( network_buffer_ptr[ readen_bytes ] & 64 ) > 0 ? 1 : 0;
																																																																						if( is_set_8bit && is_set_7bit )
																																																																							asn_class = XEnum_AsnClass_Private;
																																																																						else if( is_set_8bit )
																																																																							asn_class = XEnum_AsnClass_ContextSpecific;
																																																																						else if( is_set_7bit )
																																																																							asn_class = XEnum_AsnClass_Application;
																																																																						else
																																																																							asn_class = XEnum_AsnClass_Universal;
																																																																						is_constructed = ( network_buffer_ptr[ readen_bytes ] & 32 ) > 0 ? 1 : 0;
																																																																						asn_universal_class_tags = (XEnum_AsnUniversalClassTags)( network_buffer_ptr[ readen_bytes ] & ~ 224 );
																																																																						if( asn_class == XEnum_AsnClass_Universal )
																																																																						{
																																																																							if( is_constructed == 0 )
																																																																							{
																																																																								if( asn_universal_class_tags == XEnum_AsnUniversalClassTags_OCTETSTRING )
																																																																								{
																																																																									asn_length = 0;
																																																																									asn_length_of_length = 0;
																																																																									if( (rux::uint8)network_buffer_ptr[ readen_bytes + 1 ] > (rux::uint8)0x7f )
																																																																									{
																																																																										asn_length_of_length = network_buffer_ptr[ readen_bytes + 1 ] - 0x80;
																																																																										if( network_buffer_size >= readen_bytes + 2 * sizeof( char ) + asn_length_of_length )
																																																																										{
																																																																											decrementer = asn_length_of_length - 1;
																																																																											for( is_constructed = 0 ; is_constructed < asn_length_of_length ; is_constructed++ )
																																																																											{
																																																																												( (char*)&asn_length )[ is_constructed ] = network_buffer_ptr[ readen_bytes + 2 + decrementer];									
																																																																												decrementer--;
																																																																											}																																																																																																																													
																																																																										}
																																																																										else
																																																																											error = "Incorrect LDAP Search Result Entry buffer,length of ASN length more receive buffer size";				
																																																																									}
																																																																									else	
																																																																										asn_length = network_buffer_ptr[ readen_bytes + 1 ];
																																																																									if( error.Length() == 0 )
																																																																									{
																																																																										if( asn_length <= network_buffer_size - readen_bytes - 2 * sizeof( char ) - asn_length_of_length )
																																																																										{																														
																																																																											readen_bytes += 2 * sizeof( char ) + asn_length_of_length;		
																																																																											if( asn_length > 0 )
																																																																												ldap_partial_attribute()->_attributes.Add( rux::XString( (char*)&network_buffer_ptr[ readen_bytes ] , 0 , asn_length , XEnumCodePage_UTF8 ) );
																																																																											else 
																																																																												ldap_partial_attribute()->_attributes.Add( rux::XString() );
																																																																											readen_bytes += asn_length;
																																																																											if( attributes_asn_length >= 2 * sizeof( char ) + asn_length_of_length + asn_length )
																																																																												attributes_asn_length -= 2 * sizeof( char ) + asn_length_of_length + asn_length;
																																																																											else
																																																																												error = "Incorrect LDAP Search Result Entry buffer,readen ASN bytes length more ASN total length";
																																																																											if( partial_asn_length >= 2 * sizeof( char ) + asn_length_of_length + asn_length )
																																																																												partial_asn_length -= 2 * sizeof( char ) + asn_length_of_length + asn_length;
																																																																											else
																																																																												error = "Incorrect LDAP Search Result Entry buffer,readen ASN bytes length more ASN total length";																																																																										
																																																																										}
																																																																										else
																																																																											error = "Incorrect LDAP Search Result Entry buffer,received network buffer length less ASN total length";
																																																																									}
																																																																								}
																																																																								else
																																																																									error = "Incorrect LDAP Search Result Entry buffer,incorrect ASN universal class tag";
																																																																							}
																																																																							else
																																																																								error = "Incorrect LDAP Search Result Entry buffer,incorrect ASN Primitive/Constructed";
																																																																						}
																																																																						else
																																																																							error = "Incorrect LDAP Search Result Entry buffer,incorrect ASN class";
																																																																					}
																																																																					else
																																																																						error = "Incorrect LDAP Search Result Entry buffer,received network buffer length less ASN total length";
																																																																				}
																																																																				else if( readen_bytes > total_asn_length )
																																																																					error = "Incorrect LDAP Search Result Entry buffer,readen ASN bytes length more ASN total length";
																																																																				if( error.Length() > 0 )
																																																																					break;
																																																																			}			
																																																																			if( error.Length() == 0 )
																																																																				ldap_search_result_entry()->_attributes.Add( ldap_partial_attribute );
																																																																		}
																																																																	}
																																																																	else
																																																																		error = "Incorrect LDAP Search Result Entry buffer,received network buffer length less ASN total length";
																																																																}
																																																															}
																																																															else
																																																																error = "Incorrect LDAP Search Result Entry buffer,incorrect ASN universal class tag";
																																																														}
																																																														else
																																																															error = "Incorrect LDAP Search Result Entry buffer,incorrect ASN Primitive/Constructed";
																																																													}
																																																													else
																																																														error = "Incorrect LDAP Search Result Entry buffer,incorrect ASN class";
																																																												}
																																																												else
																																																													error = "Incorrect LDAP Search Result Entry buffer,received network buffer length less ASN total length";
																																																											}
																																																											else if( readen_bytes > total_asn_length )
																																																												error = "Incorrect LDAP Search Result Entry buffer,readen ASN bytes length more ASN total length";
																																																										}
																																																									}
																																																									else
																																																										error = "Incorrect LDAP Search Result Entry buffer,received network buffer length less ASN total length";
																																																								}
																																																							}
																																																							else
																																																								error = "Incorrect LDAP Search Result Entry buffer,incorrect ASN universal class tag";
																																																						}
																																																						else
																																																							error = "Incorrect LDAP Search Result Entry buffer,incorrect ASN Primitive/Constructed";
																																																					}
																																																					else
																																																						error = "Incorrect LDAP Search Result Entry buffer,incorrect ASN class";
																																																				}																																																		
																																																				else
																																																					error = "Incorrect LDAP Search Result Entry buffer,received network buffer length less ASN total length";
																																																			}
																																																			else if( readen_bytes > total_asn_length )
																																																				error = "Incorrect LDAP Search Result Entry buffer,readen ASN bytes length more ASN total length";
																																																		}
																																																	}
																																																	else
																																																		error = "Incorrect LDAP Search Result Entry buffer,received network buffer length less ASN total length";
																																																}
																																															}
																																															else
																																																error = "Incorrect LDAP Search Result Entry buffer,incorrect ASN universal class tag";
																																														}
																																														else
																																															error = "Incorrect LDAP Search Result Entry buffer,incorrect ASN Primitive/Constructed";
																																													}
																																													else
																																														error = "Incorrect LDAP Search Result Entry buffer,incorrect ASN class";
																																												}
																																												else
																																													error = "Incorrect LDAP Search Result Entry buffer,received network buffer length less ASN total length";
																																											}
																																											else if( readen_bytes > total_asn_length )
																																												error = "Incorrect LDAP Search Result Entry buffer,readen ASN bytes length more ASN total length";
																																											if( error.Length() > 0 )
																																												break;
																																										}
																																									}
																																								}
																																								else 
																																									error = "Incorrect LDAP Search Result Entry buffer,received network buffer length less ASN total length";
																																							}
																																						}
																																						else
																																							error = "Incorrect LDAP Search Result Entry buffer,incorrect ASN universal class tag";
																																					}
																																					else
																																						error = "Incorrect LDAP Search Result Entry buffer,incorrect ASN Primitive/Constructed";
																																				}
																																				else
																																					error = "Incorrect LDAP Search Result Entry buffer,incorrect ASN class";
																																			}
																																			else 
																																				error = "Incorrect LDAP Search Result Entry buffer,received network buffer length less ASN total length";
																																		}
																																		else if( readen_bytes > total_asn_length )
																																			error = "Incorrect LDAP Search Result Entry buffer,readen ASN bytes length more ASN total length";
																																	}
																																	else 
																																		error = "Incorrect LDAP Search Result Entry buffer,received network buffer length less ASN total length";
																																}
																															}
																															else
																																error = "Incorrect LDAP Search Result Entry buffer,incorrect ASN universal class tag";
																														}
																														else
																															error = "Incorrect LDAP Search Result Entry buffer,incorrect ASN Primitive/Constructed";
																													}
																													else
																														error = "Incorrect LDAP Search Result Entry buffer,incorrect ASN class";
																												}
																												else
																													error = "Incorrect LDAP Search Result Entry buffer,received network buffer length less ASN total length";
																											}
																											else if( readen_bytes > total_asn_length )
																												error = "Incorrect LDAP Search Result Entry buffer,readen ASN bytes length more ASN total length";
																										}
																										else
																											error = "Incorrect LDAP Search Result Entry buffer,internal ASN length is not equal total ASN length";																						
																									}
																									else 
																										error = "Incorrect LDAP Search Result Entry buffer,received network buffer length less ASN total length";
																								}
																								if( error.Length() == 0 )
																									ldap_packet()->_ldap_content = ldap_search_result_entry;
																							}
																							else if( asn_universal_class_tags == 5 )
																							{
																								XLdapSearchResultDone ldap_search_result_done;
																								ldap_search_result_done()->_message_id = message_id;																						
																								asn_length = 0;
																								asn_length_of_length = 0;
																								if( (rux::uint8)network_buffer_ptr[ readen_bytes + 1 ] > (rux::uint8)0x7f )
																								{
																									asn_length_of_length = network_buffer_ptr[ readen_bytes + 1 ] - 0x80;
																									if( network_buffer_size >= readen_bytes + 2 * sizeof( char ) + asn_length_of_length )
																									{
																										decrementer = asn_length_of_length - 1;
																										for( is_constructed = 0 ; is_constructed < asn_length_of_length ; is_constructed++ )
																										{
																											( (char*)&asn_length )[ is_constructed ] = network_buffer_ptr[ readen_bytes + 2 + decrementer];									
																											decrementer--;
																										}
																									}
																									else
																										error = "Incorrect LDAP Search Result Done buffer,length of ASN length is higher receive buffer size";				
																								}
																								else	
																									asn_length = network_buffer_ptr[ readen_bytes + 1 ];
																								if( error.Length() == 0 )
																								{
																									if( asn_length <= network_buffer_size - readen_bytes - 2 * sizeof( char ) - asn_length_of_length )
																									{																						
																										if( readen_bytes + asn_length == total_asn_length - 2 * sizeof( char ) - total_asn_length_of_length )
																										{
																											readen_bytes += 2 * sizeof( char ) + asn_length_of_length;				
																											if( readen_bytes < total_asn_length )
																											{
																												if( readen_bytes + 2 * sizeof( char ) <= network_buffer_size )
																												{
																													is_set_8bit = ( network_buffer_ptr[ readen_bytes ] & 128 ) > 0 ? 1 : 0;
																													is_set_7bit = ( network_buffer_ptr[ readen_bytes ] & 64 ) > 0 ? 1 : 0;
																													if( is_set_8bit && is_set_7bit )
																														asn_class = XEnum_AsnClass_Private;
																													else if( is_set_8bit )
																														asn_class = XEnum_AsnClass_ContextSpecific;
																													else if( is_set_7bit )
																														asn_class = XEnum_AsnClass_Application;
																													else
																														asn_class = XEnum_AsnClass_Universal;
																													is_constructed = ( network_buffer_ptr[ readen_bytes ] & 32 ) > 0 ? 1 : 0;
																													asn_universal_class_tags = (XEnum_AsnUniversalClassTags)( network_buffer_ptr[ readen_bytes ] & ~ 224 );
																													if( asn_class == XEnum_AsnClass_Universal )
																													{
																														if( is_constructed == 0 )
																														{
																															if( asn_universal_class_tags == XEnum_AsnUniversalClassTags_ENUMERATED )
																															{
																																asn_length = 0;
																																asn_length_of_length = 0;
																																if( (rux::uint8)network_buffer_ptr[ readen_bytes + 1 ] > (rux::uint8)0x7f )
																																{
																																	asn_length_of_length = network_buffer_ptr[ readen_bytes + 1 ] - 0x80;
																																	if( network_buffer_size >= readen_bytes + 2 * sizeof( char ) + asn_length_of_length )
																																	{
																																		decrementer = asn_length_of_length - 1;
																																		for( is_constructed = 0 ; is_constructed < asn_length_of_length ; is_constructed++ )
																																		{
																																			( (char*)&asn_length )[ is_constructed ] = network_buffer_ptr[ readen_bytes + 2 + decrementer];									
																																			decrementer--;
																																		}
																																	}
																																	else
																																		error = "Incorrect LDAP Search Result Done buffer,length of ASN length is higher receive buffer size";				
																																}
																																else	
																																	asn_length = network_buffer_ptr[ readen_bytes + 1 ];
																																if( error.Length() == 0 )
																																{
																																	if( asn_length <= network_buffer_size - readen_bytes - 2 * sizeof( char ) - asn_length_of_length )
																																	{
																																		if( asn_length <= sizeof( ldap_search_result_done()->_result_code ) )
																																		{
																																			readen_bytes += 2 * sizeof( char ) + asn_length_of_length;
																																			ldap_search_result_done()->_result_code = XEnum_LdapResultCode_success;
																																			if( asn_length > 0 )
																																			{
																																				decrementer = static_cast<rux::uint8>( asn_length - 1 );
																																				for( is_constructed = 0 ; is_constructed < asn_length ; is_constructed++ )
																																				{
																																					((char*)&ldap_search_result_done()->_result_code)[ is_constructed ] = network_buffer_ptr[ readen_bytes + decrementer];
																																					decrementer--;
																																				}
																																			}
																																			readen_bytes += asn_length;
																																			if( readen_bytes < total_asn_length )
																																			{
																																				if( readen_bytes + 2 * sizeof( char ) <= network_buffer_size )
																																				{
																																					is_set_8bit = ( network_buffer_ptr[ readen_bytes ] & 128 ) > 0 ? 1 : 0;
																																					is_set_7bit = ( network_buffer_ptr[ readen_bytes ] & 64 ) > 0 ? 1 : 0;
																																					if( is_set_8bit && is_set_7bit )
																																						asn_class = XEnum_AsnClass_Private;
																																					else if( is_set_8bit )
																																						asn_class = XEnum_AsnClass_ContextSpecific;
																																					else if( is_set_7bit )
																																						asn_class = XEnum_AsnClass_Application;
																																					else
																																						asn_class = XEnum_AsnClass_Universal;
																																					is_constructed = ( network_buffer_ptr[ readen_bytes ] & 32 ) > 0 ? 1 : 0;
																																					asn_universal_class_tags = (XEnum_AsnUniversalClassTags)( network_buffer_ptr[ readen_bytes ] & ~ 224 );
																																					if( asn_class == XEnum_AsnClass_Universal )
																																					{
																																						if( is_constructed == 0 )
																																						{
																																							if( asn_universal_class_tags == XEnum_AsnUniversalClassTags_OCTETSTRING )
																																							{
																																								asn_length = 0;
																																								asn_length_of_length = 0;
																																								if( (rux::uint8)network_buffer_ptr[ readen_bytes + 1 ] > (rux::uint8)0x7f )
																																								{
																																									asn_length_of_length = network_buffer_ptr[ readen_bytes + 1 ] - 0x80;
																																									if( network_buffer_size >= readen_bytes + 2 * sizeof( char ) + asn_length_of_length )
																																									{
																																										decrementer = asn_length_of_length - 1;
																																										for( is_constructed = 0 ; is_constructed < asn_length_of_length ; is_constructed++ )
																																										{
																																											( (char*)&asn_length )[ is_constructed ] = network_buffer_ptr[ readen_bytes + 2 + decrementer];									
																																											decrementer--;
																																										}
																																									}
																																									else
																																										error = "Incorrect LDAP Search Result Done buffer,length of ASN length is higher receive buffer size";				
																																								}
																																								else	
																																									asn_length = network_buffer_ptr[ readen_bytes + 1 ];
																																								if( error.Length() == 0 )
																																								{
																																									if( asn_length <= network_buffer_size - readen_bytes - 2 * sizeof( char ) - asn_length_of_length )
																																									{																														
																																										readen_bytes += 2 * sizeof( char ) + asn_length_of_length;																							
																																										if( asn_length > 0 )
																																											ldap_search_result_done()->_matched_dn = rux::XString( (char*)&network_buffer_ptr[ readen_bytes ] , 0 , asn_length , XEnumCodePage_UTF8 );
																																										else 
																																											ldap_search_result_done()->_matched_dn.Clear();
																																										readen_bytes += asn_length;
																																										if( readen_bytes < total_asn_length )
																																										{
																																											if( readen_bytes + 2 * sizeof( char ) <= network_buffer_size )
																																											{
																																												is_set_8bit = ( network_buffer_ptr[ readen_bytes ] & 128 ) > 0 ? 1 : 0;
																																												is_set_7bit = ( network_buffer_ptr[ readen_bytes ] & 64 ) > 0 ? 1 : 0;
																																												if( is_set_8bit && is_set_7bit )
																																													asn_class = XEnum_AsnClass_Private;
																																												else if( is_set_8bit )
																																													asn_class = XEnum_AsnClass_ContextSpecific;
																																												else if( is_set_7bit )
																																													asn_class = XEnum_AsnClass_Application;
																																												else
																																													asn_class = XEnum_AsnClass_Universal;
																																												is_constructed = ( network_buffer_ptr[ readen_bytes ] & 32 ) > 0 ? 1 : 0;
																																												asn_universal_class_tags = (XEnum_AsnUniversalClassTags)( network_buffer_ptr[ readen_bytes ] & ~ 224 );
																																												if( asn_class == XEnum_AsnClass_Universal )
																																												{
																																													if( is_constructed == 0 )
																																													{
																																														if( asn_universal_class_tags == XEnum_AsnUniversalClassTags_OCTETSTRING )
																																														{
																																															asn_length = 0;
																																															asn_length_of_length = 0;
																																															if( (rux::uint8)network_buffer_ptr[ readen_bytes + 1 ] > (rux::uint8)0x7f )
																																															{
																																																asn_length_of_length = network_buffer_ptr[ readen_bytes + 1 ] - 0x80;
																																																if( network_buffer_size >= readen_bytes + 2 * sizeof( char ) + asn_length_of_length )
																																																{
																																																	decrementer = asn_length_of_length - 1;
																																																	for( is_constructed = 0 ; is_constructed < asn_length_of_length ; is_constructed++ )
																																																	{
																																																		( (char*)&asn_length )[ is_constructed ] = network_buffer_ptr[ readen_bytes + 2 + decrementer];									
																																																		decrementer--;
																																																	}
																																																}
																																																else
																																																	error = "Incorrect LDAP Search Result Done buffer,length of ASN length is higher receive buffer size";				
																																															}
																																															else	
																																																asn_length = network_buffer_ptr[ readen_bytes + 1 ];
																																															if( error.Length() == 0 )
																																															{
																																																if( asn_length <= network_buffer_size - readen_bytes - 2 * sizeof( char ) - asn_length_of_length )
																																																{																														
																																																	readen_bytes += 2 * sizeof( char ) + asn_length_of_length;																							
																																																	if( asn_length > 0 )
																																																		ldap_search_result_done()->_error_message = rux::XString( (char*)&network_buffer_ptr[ readen_bytes ] , 0 , asn_length , XEnumCodePage_UTF8 );
																																																	else 
																																																		ldap_search_result_done()->_error_message.Clear();
																																																	readen_bytes += asn_length;
																																																	if( readen_bytes != total_asn_length )
																																																		error = "Incorrect LDAP Search Result Done buffer,readen ASN bytes is not equal ASN total length";																																														
																																																}
																																																else 
																																																	error = "Incorrect LDAP Search Result Done buffer,received network buffer length less ASN total length";
																																															}
																																														}
																																														else
																																															error = "Incorrect LDAP Search Result Done buffer,incorrect ASN universal class tag";
																																													}
																																													else
																																														error = "Incorrect LDAP Search Result Done buffer,incorrect ASN Primitive/Constructed";
																																												}
																																												else
																																													error = "Incorrect LDAP Search Result Done buffer,incorrect ASN class";
																																											}
																																											else 
																																												error = "Incorrect LDAP Search Result Done buffer,received network buffer length less ASN total length";
																																										}
																																										else if( readen_bytes > total_asn_length )
																																											error = "Incorrect LDAP Search Result Done buffer,readen ASN bytes length more ASN total length";
																																										else
																																											error = "Incorrect LDAP Search Result Done buffer,ASN total length is too small ";
																																									}
																																									else 
																																										error = "Incorrect LDAP Search Result Done buffer,received network buffer length less ASN total length";
																																								}
																																							}
																																							else
																																								error = "Incorrect LDAP Search Result Done buffer,incorrect ASN universal class tag";
																																						}
																																						else
																																							error = "Incorrect LDAP Search Result Done buffer,incorrect ASN Primitive/Constructed";
																																					}
																																					else
																																						error = "Incorrect LDAP Search Result Done buffer,incorrect ASN class";
																																				}
																																				else 
																																					error = "Incorrect LDAP Search Result Done buffer,received network buffer length less ASN total length";
																																			}
																																			else if( readen_bytes > total_asn_length )
																																				error = "Incorrect LDAP Search Result Done buffer,readen ASN bytes length more ASN total length";
																																			else
																																				error = "Incorrect LDAP Search Result Done buffer,ASN total length is too small ";
																																		}
																																		else
																																			error = "Incorrect LDAP Search Result Done buffer,ASN length more size of Result code";
																																	}
																																	else 
																																		error = "Incorrect LDAP Search Result Done buffer,received network buffer length less ASN total length";
																																}
																															}
																															else
																																error = "Incorrect LDAP Search Result Done buffer,incorrect ASN universal class tag";																																																											
																														}
																														else
																															error = "Incorrect LDAP Search Result Done buffer,incorrect ASN Primitive/Constructed";																													
																													}
																													else
																														error = "Incorrect LDAP Search Result Done buffer,incorrect ASN class";
																												}
																												else 
																													error = "Incorrect LDAP Search Result Done buffer,received network buffer length less ASN total length";
																											}
																											else if( readen_bytes > total_asn_length )
																												error = "Incorrect LDAP Search Result Done buffer,readen ASN bytes length more ASN total length";
																											else
																												error = "Incorrect LDAP Search Result Done buffer,ASN total length is too small ";
																										}
																										else
																											error = "Incorrect LDAP Search Result Done buffer,internal ASN length is not equal total ASN length";																						
																									}
																									else 
																										error = "Incorrect LDAP Search Result Done buffer,received network buffer length less ASN total length";
																								}
																								if( error.Length() == 0 )
																									ldap_packet()->_ldap_content = ldap_search_result_done;
																							}



																							else
																								error = "Incorrect LDAP buffer,incorrect ASN universal class tag";
																						}
																						else
																							error = "Incorrect LDAP buffer,incorrect ASN Primitive/Constructed";
																					}
																					else
																						error = "Incorrect LDAP buffer,incorrect ASN class";
																				}																	
																			}
																			else if( readen_bytes > total_asn_length )
																				error = "Incorrect LDAP buffer,readen ASN bytes length more ASN total length";
																			else
																				error = "Incorrect LDAP buffer,ASN total length is too small ";
																		}
																		else
																			error = "Incorrect LDAP buffer,ASN length more size of MessageId";
																	}
																	else 
																		error = "Incorrect LDAP buffer,received network buffer length less ASN total length";
																}
															}
															else
																error = "Incorrect LDAP buffer,incorrect ASN universal class tag";
														}
														else
															error = "Incorrect LDAP buffer,incorrect ASN Primitive/Constructed";
													}
													else
														error = "Incorrect LDAP buffer,incorrect ASN class";
												}
												else 
													error = "Incorrect LDAP buffer,received network buffer length less ASN total length";
											}
											else
												error = "Incorrect LDAP buffer,total ASN length is higher receive buffer size";				
										}
										else 
											error = "Incorrect LDAP buffer,total ASN length is equal 0";
									}
								}
								else
									error = "Incorrect LDAP buffer,incorrect ASN universal class tag";
							}
							else
								error = "Incorrect LDAP buffer,incorrect ASN Primitive/Constructed";
						}
						else
							error = "Incorrect LDAP buffer,incorrect ASN class";
					}
					else
						error = "Receive network buffer pointer is NULL";
				}
				else
					error = "Receive network buffer size is less 2";		
			}
			return readen_bytes;
		};
	};
};