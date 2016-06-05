#ifndef XAN_LDAP_H
#define XAN_LDAP_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_tcpclient.h>
#include <xan_ldappacket.h>
#include <xan_ldapnode.h>
#include <xan_tls.h>
namespace rux
{
	namespace network
	{
		class XLdap;
		begin_declare_rux_class( Ldap );
		begin_declare_rux_class_members( Ldap );
			//XTcpClient _tcp_client;
			XTls _tls_client;
			rux::network::XLdapPacket _ldap_packet;
			rux::uint32 _message_id;
			rux::uint8 _is_connected;
			rux::XString _create_timestamp;
			rux::XString _modify_timestamp;
			rux::XArray<rux::XString> _naming_contexts;
			rux::XArray<rux::XString> _supported_controls;
			rux::XArray<rux::XString> _supported_extensions;
			rux::XArray<rux::XString> _supported_features;
			rux::XArray<rux::XString> _supported_ldap_versions;
			rux::XArray<rux::XString> _supported_sasl_mechanism;
			rux::XArray<rux::XString> _object_classes;
			rux::XArray<XLdapNode> _subschema_subentries;
			rux::uint8 _is_ssl;
		end_declare_rux_class_members( Ldap );	
			void Connect( const rux::network::XAddress& network_address , rux::XString& user_name , rux::XString& password , rux::uint8 is_ssl , rux::XString& error );
			void Disconnect( void );
			rux::Array<XLdapNode>&  Search( const rux::XString& base_object , XEnum_LdapSearchScope ldap_search_scope ,
				XEnum_LdapSearchAlias ldap_search_alias , rux::uint32 size_limit , rux::uint32 time_limit , rux::uint8 types_only , 
				const XLdapFilter& ldap_filter , const rux::XArray<rux::XString>& attributes , rux::XString& error );
			LdapNode& SearchAll( rux::XString& error );
		private:
			rux::String& GetLDAPError( XEnum_LdapResultCode ldap_result_code );
		end_declare_rux_class();
	};
};
#endif
