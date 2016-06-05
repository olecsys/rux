#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_ldapbindrequest.h>
#include <xan_keyvaluepair.h>
#include <xan_uint64.h>
#include <xan_boolean.h>
begin_implement_rux_class_ns( LdapBindRequest , rux::network );
	_message_id = 0;
	_version = 3;	
	_authentication_type = XEnum_LdapAuthenticationType_simple;	
end_implement_rux_class();
namespace rux
{
	namespace network
	{
		void XLdapBindRequest::set_MessageId( rux::uint32 message_id )
		{
			(*this)()->_message_id = message_id;
		};
		void XLdapBindRequest::set_Version( rux::uint8 version )
		{
			(*this)()->_version = version;
		};
		void XLdapBindRequest::set_Name( const rux::XString& name )
		{
			(*this)()->_name = name.ConvertToUTF8();
		};
		void XLdapBindRequest::set_AuthenticationType( XEnum_LdapAuthenticationType authentication_type )
		{
			(*this)()->_authentication_type = authentication_type;
		};
		void XLdapBindRequest::set_SaslMechanism( const rux::XString& sasl_mechanism )
		{
			(*this)()->_sasl_mechanism = sasl_mechanism.ConvertToUTF8();
		};
		void XLdapBindRequest::set_SaslCredentials( const rux::XString& sasl_credentials )
		{
			(*this)()->_sasl_credentials = sasl_credentials.ConvertToUTF8();
		};
		void XLdapBindRequest::set_Password( const rux::XString& password )
		{
			(*this)()->_password = password.ConvertToUTF8();
		};
	};
};