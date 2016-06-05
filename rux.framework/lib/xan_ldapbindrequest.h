#ifndef XAN_LDAPBINDREQUESTPACKET_H
#define XAN_LDAPBINDREQUESTPACKET_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_asn_defines.h>
#include <xan_string.h>
enum XEnum_LdapAuthenticationType
{
	XEnum_LdapAuthenticationType_simple = 0 ,
	XEnum_LdapAuthenticationType_sasl = 3
};
namespace rux
{
	namespace network
	{
		begin_declare_rux_class( LdapBindRequest );
			friend class XLdapPacket;
		begin_declare_rux_class_members( LdapBindRequest );
			rux::uint32 _message_id;
			rux::uint8 _version;
			rux::XString _name;
			XEnum_LdapAuthenticationType _authentication_type;
			rux::XString _sasl_mechanism;
			rux::XString _sasl_credentials;
			rux::XString _password;
		end_declare_rux_class_members( LdapBindRequest );
			void set_MessageId( rux::uint32 message_id );
			void set_Version( rux::uint8 version );
			void set_Name( const rux::XString& name );
			void set_AuthenticationType( XEnum_LdapAuthenticationType authentication_type );
			void set_SaslMechanism( const rux::XString& sasl_mechanism );
			void set_SaslCredentials( const rux::XString& sasl_credentials );
			void set_Password( const rux::XString& password );	
		end_declare_rux_class();
	};
};
#endif
