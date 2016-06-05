#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_ldap.h>
#include <xan_keyvaluepair.h>
#include <xan_uint64.h>
#include <xan_boolean.h>
begin_implement_rux_class_ns( Ldap , rux::network );
	_message_id = 1;
	_is_connected = 0;
	_is_ssl = 0;
end_implement_rux_class();
namespace rux
{
	namespace network
	{		
		void XLdap::Disconnect( void )
		{
			if( (*this)()->_is_connected == 1 )
			{
				rux::XString error;
				XLdapUnbindRequest ldap_unbind_request;
				ldap_unbind_request.set_MessageId( (*this)()->_message_id );
				(*this)()->_ldap_packet.set_LdapContent( ldap_unbind_request );
				if( (*this)()->_is_ssl == 1 )
				{
					//(*this)()->_tls_client.Send( (*this)()->_ldap_packet , XLdapPacket::NetworkFormat , error );
					(*this)()->_tls_client.Disconnect();
				}
				else
				{
					//(*this)()->_tcp_client.Send( (*this)()->_ldap_packet , XLdapPacket::NetworkFormat , error );
					//(*this)()->_tcp_client.Close();
				}
				(*this)()->_is_connected = 0;
				(*this)()->_message_id = 1;
				(*this)()->_is_ssl = 0;
			}
		};
		void XLdap::Connect( const rux::network::XAddress& network_address , rux::XString& user_name , rux::XString& password , rux::uint8 is_ssl , rux::XString& error )
		{
			Disconnect();
			error.Clear();	
			(*this)()->_is_ssl = is_ssl;
			if( is_ssl == 1 )
				(*this)()->_tls_client.Connect( network_address , error );
			else
				//(*this)()->_tcp_client.set_NetworkAddress( network_address );		
			if( error.Length() == 0 )
			{
				XLdapBindRequest ldap_bind_request;
				ldap_bind_request.set_MessageId( (*this)()->_message_id );
				ldap_bind_request.set_Version( 3 );
				ldap_bind_request.set_Name( user_name );
				ldap_bind_request.set_AuthenticationType( XEnum_LdapAuthenticationType_simple );
				ldap_bind_request.set_Password( password );
				(*this)()->_ldap_packet.set_LdapContent( ldap_bind_request );
				//if( is_ssl == 1 )
					//(*this)()->_tls_client.Send( (*this)()->_ldap_packet , (*this)()->_ldap_packet , XLdapPacket::NetworkFormat , XLdapPacket::NetworkParse , XLdapPacket::NetworkGetLength , error );
				//else
					//(*this)()->_tcp_client.Send( (*this)()->_ldap_packet , (*this)()->_ldap_packet , XLdapPacket::NetworkFormat , XLdapPacket::NetworkParse , XLdapPacket::NetworkGetLength , error );
				if( error.Length() == 0 )
				{
					(*this)()->_message_id++;
					XObject ldap_content( (*this)()->_ldap_packet.get_LdapContent() );
					if( rux_is_object( ldap_content , XLdapBindResponse ) )
					{
						XLdapBindResponse ldap_bind_response( ldap_content );
						if( ldap_bind_response.get_ResultCode() == XEnum_LdapResultCode_success )
							(*this)()->_is_connected = 1;
						else
							error = "LDAP bind response buffer contains result code '" + GetLDAPError( ldap_bind_response.get_ResultCode() ) + "'";
					}
					else 
						error = "LDAP received buffer is not LDAP bind response buffer";
				}
			}
		};
		LdapNode& XLdap::SearchAll( rux::XString& error )
		{
			XLdapNode ldap_node , ldap_node1;	
			rux::XArray<XLdapNode> nodes0 , nodes1;
			XLdapFilter ldap_filter;
			ldap_filter.set_FilterType( XEnum_LdapFilterType_present );
			ldap_filter.AddMatch( rux::XString( "objectclass" ) );
			rux::XArray<rux::XString> attributes;
			attributes.Add( rux::XString( "subschemaSubentry" ) );
			nodes0 = Search( rux::XString() , XEnum_LdapSearchScope_baseObject , XEnum_LdapSearchAlias_derefAlways ,
				0 , 0 , 0 , ldap_filter ,	attributes , error );
			if( error.Length() == 0 )
			{
				rux::uint32 index0 = 0 , index1 = 0 , index2 = 0;
				XLdapAttributeValueAssertion ldap_attribute_value_assertion;
				ldap_attribute_value_assertion.set_AttributeDescription( rux::XString( "objectclass" ) );
				ldap_attribute_value_assertion.set_AttributeValue( rux::XString( "subschema" ) );
				ldap_filter.set_FilterType( XEnum_LdapFilterType_equalityMatch );
				ldap_filter.AddMatch( ldap_attribute_value_assertion );
				attributes.Clear();
				attributes.Add( rux::XString( "createTimestamp" ) );
				attributes.Add( rux::XString( "modifyTimestamp" ) );
				for( ; index0 < nodes0.Count() ; index0++ )
				{
					for( index1 = 0 ; index1 < nodes0[ index0 ]._attributes.Count() ; index1++ )
					{
						if( nodes0[ index0 ]._attributes[ index1 ].get_Name() == "subschemaSubentry" )
						{
							ldap_node1 = XLdapNode();
							ldap_node1()->_object_name = nodes0[ index0 ]._attributes[ index1 ].get_Value();
							(*this)()->_subschema_subentries.Add( ldap_node1 );
							nodes1.AddRange( Search( nodes0[ index0 ]._attributes[ index1 ].get_Value() ,
								XEnum_LdapSearchScope_baseObject , XEnum_LdapSearchAlias_derefAlways , 0 , 0 , 0 , ldap_filter , attributes ,
								error ) );
							if( error.Length() > 0 )
								break;					
						}
					}
				}
				if( error.Length() == 0 )
				{
					if( nodes1.Count() > 0 )
					{
						for( index0 = 0 ; index0 < nodes1.Count() ; index0++ )
						{
							for( index1 = 0 ; index1 < nodes1[ index0 ]._attributes.Count() ; index1++ )
							{
								if( nodes1[ index0 ]._attributes[ index1 ].get_Name() == "createTimestamp" )						
									(*this)()->_create_timestamp = nodes1[ index0 ]._attributes[ index1 ].get_Value();
								else if( nodes1[ index0 ]._attributes[ index1 ].get_Name() == "modifyTimestamp" )
									(*this)()->_modify_timestamp = nodes1[ index0 ]._attributes[ index1 ].get_Value();
							}
						}
					}
					ldap_filter.set_FilterType( XEnum_LdapFilterType_present );
					ldap_filter.AddMatch( rux::XString( "objectclass" ) );
					attributes.Clear();
					attributes.Add( rux::XString( "namingContexts" ) );
					attributes.Add( rux::XString( "subschemaSubentry" ) );
					attributes.Add( rux::XString( "supportedLDAPVersion" ) );
					attributes.Add( rux::XString( "supportedSASLMechanisms" ) );
					attributes.Add( rux::XString( "supportedExtension" ) );
					attributes.Add( rux::XString( "supportedControl" ) );
					attributes.Add( rux::XString( "supportedFeatures" ) );
					attributes.Add( rux::XString( "vendorName" ) );
					attributes.Add( rux::XString( "vendorVersion" ) );
					attributes.Add( rux::XString( "+" ) );
					attributes.Add( rux::XString( "objectClass" ) );
					nodes0 = Search( rux::XString() , XEnum_LdapSearchScope_baseObject , XEnum_LdapSearchAlias_neverDerefAliases ,
						0 , 0 , 0 , ldap_filter ,	attributes , error );
					if( error.Length() == 0 )
					{
						(*this)()->_naming_contexts.Clear();
						(*this)()->_supported_controls.Clear();
						(*this)()->_supported_extensions.Clear();
						(*this)()->_supported_features.Clear();
						(*this)()->_supported_sasl_mechanism.Clear();
						(*this)()->_supported_ldap_versions.Clear();
						(*this)()->_object_classes.Clear();
						if( nodes0.Count() > 0 )
						{
							for( index0 = 0 ; index0 < nodes0.Count() ; index0++ )
							{
								for( index1 = 0 ; index1 < nodes0[ index0 ]._attributes.Count() ; index1++ )
								{
									if( nodes0[ index0 ]._attributes[ index1 ].get_Name() == "namingContexts" )
									{
										(*this)()->_naming_contexts.Add( nodes0[ index0 ]._attributes[ index1 ].get_Value() );
									}
									else if( nodes0[ index0 ]._attributes[ index1 ].get_Name() == "supportedControl" )
									{
										(*this)()->_supported_controls.Add( nodes0[ index0 ]._attributes[ index1 ].get_Value() );
									}
									else if( nodes0[ index0 ]._attributes[ index1 ].get_Name() == "supportedExtension" )
									{
										(*this)()->_supported_extensions.Add( nodes0[ index0 ]._attributes[ index1 ].get_Value() );
									}
									else if( nodes0[ index0 ]._attributes[ index1 ].get_Name() == "supportedFeatures" )
									{
										(*this)()->_supported_features.Add( nodes0[ index0 ]._attributes[ index1 ].get_Value() );
									}
									else if( nodes0[ index0 ]._attributes[ index1 ].get_Name() == "supportedLDAPVersion" )
									{
										(*this)()->_supported_ldap_versions.Add( nodes0[ index0 ]._attributes[ index1 ].get_Value() );
									}
									else if( nodes0[ index0 ]._attributes[ index1 ].get_Name() == "supportedSASLMechanisms" )
									{
										(*this)()->_supported_sasl_mechanism.Add( nodes0[ index0 ]._attributes[ index1 ].get_Value() );
									}								
									else if( nodes0[ index0 ]._attributes[ index1 ].get_Name() == "objectClass" )
									{
										(*this)()->_object_classes.Add( nodes0[ index0 ]._attributes[ index1 ].get_Value() );
									}								
								}
							}
						}
					}
					for( index0 = 0 ; index0 < (*this)()->_supported_ldap_versions.Count() ; index0++ )
					{
						if( (*this)()->_supported_ldap_versions[ index0 ] == "3" )
							break;
					}
					if( index0 < (*this)()->_supported_ldap_versions.Count() )
					{
						ldap_filter.set_FilterType( XEnum_LdapFilterType_present );
						ldap_filter.AddMatch( rux::XString( "objectclass" ) );
						attributes.Clear();
						attributes.Add( rux::XString( "*" ) );				
						nodes0 = Search( rux::XString() , XEnum_LdapSearchScope_baseObject , XEnum_LdapSearchAlias_neverDerefAliases ,
							0 , 0 , 0 , ldap_filter ,	attributes , error );
						if( error.Length() == 0 )
						{
							for( index0 = 0 ; index0 < nodes0.Count() ; index0++ )
							{
								for( index1 = 0 ; index1 < nodes0[ index0 ]._attributes.Count() ; index1++ )
								{
									if( nodes0[ index0 ]._attributes[ index1 ].get_Name() == "objectClass" )
									{
										for( index2 = 0 ; index2 < (*this)()->_object_classes.Count() ; index2++ )
										{
											if( (*this)()->_object_classes[ index2 ] == nodes0[ index0 ]._attributes[ index1 ].get_Value() )
												break;
										}
										if( index2 >= (*this)()->_object_classes.Count() )
										{
											error = "Received LDAP object classes are incorrect";
											break;
										}
									}														
								}
							}
							for( index0 = 0 ; index0 < (*this)()->_naming_contexts.Count() ; index0++ )
							{
								ldap_filter.set_FilterType( XEnum_LdapFilterType_present );
								ldap_filter.AddMatch( rux::XString( "objectclass" ) );
								attributes.Clear();
								attributes.Add( rux::XString( "hasSubordinates" ) );				
								attributes.Add( rux::XString( "objectClass" ) );		
								ldap_node()->_nodes.AddRange( Search( (*this)()->_naming_contexts[ index0 ] , XEnum_LdapSearchScope_baseObject , XEnum_LdapSearchAlias_derefAlways ,
									1 , 0 , 0 , ldap_filter ,	attributes , error ) );
								if( error.Length() > 0 )
									break;
							}
							if( error.Length() == 0 )
							{
								for( index0 = 0 ; index0 < (*this)()->_subschema_subentries.Count() ; index0++ )
								{
									(*this)()->_subschema_subentries[ index0 ]._nodes.AddRange( Search( (*this)()->_subschema_subentries[ index0 ]._object_name ,
										XEnum_LdapSearchScope_baseObject , XEnum_LdapSearchAlias_derefAlways , 1 , 0 , 0 , ldap_filter ,	attributes ,
										error ) );
									if( error.Length() > 0 )
										break;
								}
								if( error.Length() == 0 )
								{							
									for( index0 = 0 ; index0 < ldap_node()->_nodes.Count() ; index0++ )
									{
										ldap_node()->_nodes[ index0 ]._nodes.AddRange( Search( ldap_node()->_nodes[ index0 ]._object_name ,
											XEnum_LdapSearchScope_singleLevel , XEnum_LdapSearchAlias_derefAlways , 0 , 0 , 0 , ldap_filter ,	attributes ,
											error ) );
										if( error.Length() > 0 )
											break;
									}
								}
							}
						}
					}
					else
						error = "LDAP server does not support LDAP version 3";
				}
			}
			return ldap_node++;
		};
		rux::Array<XLdapNode>& XLdap::Search( const rux::XString& base_object , XEnum_LdapSearchScope ldap_search_scope ,
				XEnum_LdapSearchAlias ldap_search_alias , rux::uint32 size_limit , rux::uint32 time_limit , rux::uint8 types_only , 
				const XLdapFilter& ldap_filter , const rux::XArray<rux::XString>& attributes , rux::XString& error )
		{
			error.Clear();
			rux::XArray<XLdapNode> ldap_nodes;
			if( (*this)()->_is_connected == 1 )
			{
				rux::network::XLdapSearchRequest ldap_search_request;
				ldap_search_request.set_MessageId( (*this)()->_message_id );
				ldap_search_request.set_BaseObject( base_object );
				ldap_search_request.set_Scope( ldap_search_scope );
				ldap_search_request.set_DerefAliases( ldap_search_alias );
				ldap_search_request.set_SizeLimit( size_limit );
				ldap_search_request.set_TimeLimit( time_limit );
				ldap_search_request.set_TypesOnly( types_only );		
				ldap_search_request.set_Filter( ldap_filter );
				for( rux::uint32 index0 = 0 ; index0 < attributes.Count() ; index0++ )
					ldap_search_request.AddAttribute( attributes[ index0 ] );
				(*this)()->_ldap_packet.set_LdapContent( ldap_search_request );
				//if( (*this)()->_is_ssl == 1 )
				//	(*this)()->_tls_client.Send( (*this)()->_ldap_packet , (*this)()->_ldap_packet , XLdapPacket::NetworkFormat , XLdapPacket::NetworkParse , XLdapPacket::NetworkGetLength , error );
				//else
				//	(*this)()->_tcp_client.Send( (*this)()->_ldap_packet , (*this)()->_ldap_packet , XLdapPacket::NetworkFormat , XLdapPacket::NetworkParse , XLdapPacket::NetworkGetLength , error );
				if( error.Length() == 0 )
				{
					(*this)()->_message_id++;
					XObject ldap_content( (*this)()->_ldap_packet.get_LdapContent() );
					XLdapSearchResultEntry ldap_search_result_entry;
					XLdapNode ldap_node;
					rux::uint32 index0 = 0 , index1 = 0;
					rux::XKeyValuePair<rux::XString> key_value_pair;
					if( rux_is_object( ldap_content , XLdapSearchResultEntry ) )
					{
						do
						{
							ldap_search_result_entry = ldap_content;
							ldap_node = XLdapNode();
							ldap_node()->_object_name = ldap_search_result_entry()->_object_name;
							for( index0 = 0 ; index0 < ldap_search_result_entry()->_attributes.Count() ; index0++ )
							{
								for( index1 = 0 ; index1 < ldap_search_result_entry()->_attributes[ index0 ]._attributes.Count() ; index1++ )
								{
									key_value_pair = rux::XKeyValuePair<rux::XString>();
									key_value_pair.set_Name( ldap_search_result_entry()->_attributes[ index0 ]._attribute_type );
									key_value_pair.set_Value( ldap_search_result_entry()->_attributes[ index0 ]._attributes[ index1 ] );
									ldap_node()->_attributes.Add( key_value_pair );
								}
							}
							ldap_nodes.Add( ldap_node );
							//if( (*this)()->_is_ssl == 1 )
							//	(*this)()->_tls_client.Receive( (*this)()->_ldap_packet , XLdapPacket::NetworkParse , XLdapPacket::NetworkGetLength , error );
							//else
							//	(*this)()->_tcp_client.Receive( (*this)()->_ldap_packet , XLdapPacket::NetworkParse , XLdapPacket::NetworkGetLength , error );
							if( error.Length() == 0 )					
								ldap_content = (*this)()->_ldap_packet.get_LdapContent();
							else 
								break;
						}
						while( rux_is_object( ldap_content , XLdapSearchResultEntry ) );
					}
					if( error.Length() == 0 )
					{
						if( rux_is_object( ldap_content , XLdapSearchResultDone ) )
						{
							XLdapSearchResultDone ldap_search_result_done( ldap_content );
							if( ldap_search_result_done.get_ResultCode() != XEnum_LdapResultCode_success )
								error = "LDAP search result done contains result code '" + GetLDAPError( ldap_search_result_done.get_ResultCode() ) + "'";						
						}
						else 
							error = "LDAP received buffer is not LDAP search result entry or LDAP search result done buffer";
					}
				}
			}
			else
				error = "LDAP client was not connected";
			return ldap_nodes++;
		};
		rux::String& XLdap::GetLDAPError( XEnum_LdapResultCode ldap_result_code )
		{
			rux::XString error;
			switch( ldap_result_code )
			{
			case XEnum_LdapResultCode_operationsError:
				{
					error = "operations error";
					break;
				}
			case 	XEnum_LdapResultCode_protocolError:
				{
					error = "protocol error";
					break;
				}
			case 	XEnum_LdapResultCode_timeLimitExceeded:
				{
					error = "time limit exceeded";
					break;
				}
			case XEnum_LdapResultCode_sizeLimitExceeded:
				{
					error = "size limit exceeded";
					break;
				}
			case XEnum_LdapResultCode_compareFalse:
				{
					error = "compare false";
					break;
				}
			case XEnum_LdapResultCode_compareTrue:
				{
					error = "compare true";
					break;
				}
			case XEnum_LdapResultCode_authMethodNotSupported:
				{
					error = "auth method not supported";
					break;
				}
			case XEnum_LdapResultCode_strongerAuthRequired:
				{
					error = "stronger auth required";
					break;
				}
			case XEnum_LdapResultCode_referral:
				{
					error = "referral";
					break;
				}
			case XEnum_LdapResultCode_adminLimitExceeded:
				{
					error = "admin limit exceeded";
					break;
				}
			case XEnum_LdapResultCode_unavailableCriticalExtension:
				{
					error = "unavailable critical extension";
					break;
				}
			case XEnum_LdapResultCode_confidentialityRequired:
				{
					error = "confidentiality required";
					break;
				}
			case XEnum_LdapResultCode_saslBindInProgress:
				{
					error = "sasl bind in progress";
					break;
				}
			case XEnum_LdapResultCode_noSuchAttribute:
				{
					error = "no such attribute";
					break;
				}
			case XEnum_LdapResultCode_undefinedAttributeType:
				{
					error = "undefined attribute type";
					break;
				}
			case XEnum_LdapResultCode_inappropriateMatching:
				{
					error = "inappropriate matching";
					break;
				}
			case XEnum_LdapResultCode_constraintViolation:
				{
					error = "constraint violation";
					break;
				}
			case XEnum_LdapResultCode_attributeOrValueExists:
				{
					error = "attribute or value exists";
					break;
				}
			case XEnum_LdapResultCode_invalidAttributeSyntax:
				{
					error = "invalid attribute syntax";
					break;
				}
			case XEnum_LdapResultCode_noSuchObject:
				{
					error = "no such object";
					break;
				}
			case XEnum_LdapResultCode_aliasProblem:
				{
					error = "alias problem";
					break;
				}
			case XEnum_LdapResultCode_invalidDNSyntax:
				{
					error = "invalid DN syntax";
					break;
				}
			case XEnum_LdapResultCode_aliasDereferencingProblem:
				{
					error = "alias dereferencing problem";
					break;
				}
			case XEnum_LdapResultCode_inappropriateAuthentication:
				{
					error = "inappropriate authentication";
					break;
				}
			case XEnum_LdapResultCode_invalidCredentials:
				{
					error = "invalid credentials";
					break;
				}
			case XEnum_LdapResultCode_insufficientAccessRights:
				{
					error = "insufficient access rights";
					break;
				}
			case XEnum_LdapResultCode_busy:
				{
					error = "busy";
					break;
				}
			case XEnum_LdapResultCode_unavailable:
				{
					error = "unavailable";
					break;
				}
			case XEnum_LdapResultCode_unwillingToPerform:
				{
					error = "unwilling to perform";
					break;
				}
			case XEnum_LdapResultCode_loopDetect:
				{
					error = "loop detect";
					break;
				}
			case XEnum_LdapResultCode_namingViolation:
				{
					error = "naming violation";
					break;
				}
			case XEnum_LdapResultCode_objectClassViolation:
				{
					error = "object class violation";
					break;
				}
			case XEnum_LdapResultCode_nonAllowedOnNonLeaf:
				{
					error = "non allowed on non leaf";
					break;
				}
			case XEnum_LdapResultCode_nonAllowedOnRDN:
				{
					error = "non allowed on RDN";
					break;
				}
			case XEnum_LdapResultCode_entryAlreadyExists:
				{
					error = "entry already exists";
					break;
				}
			case XEnum_LdapResultCode_objectClassModsProhibited:
				{
					error = "object class mods prohibited";
					break;
				}
			case XEnum_LdapResultCode_affectsMultipleDSAs:
				{
					error = "affects multiple DSAs";
					break;
				}
			case XEnum_LdapResultCode_other:
				{
					error = "other";
					break;
				}
			}
			return error++;
		};
	};
};