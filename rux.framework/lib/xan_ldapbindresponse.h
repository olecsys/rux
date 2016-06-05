#ifndef XAN_LDAPBINDRESPONSEPACKET_H
#define XAN_LDAPBINDRESPONSEPACKET_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_ldapsearchresultdone.h>
namespace rux
{
	namespace network
	{
		begin_declare_rux_class( LdapBindResponse );
			friend class XLdapPacket;
		begin_declare_rux_class_members( LdapBindResponse );
			rux::uint32 _message_id;
			XEnum_LdapResultCode _result_code;
			rux::XString _matched_dn;
			rux::XString _error_message;
			rux::XString _server_sasl_creds;
		end_declare_rux_class_members( LdapBindResponse );
			XEnum_LdapResultCode get_ResultCode( void );	
		end_declare_rux_class();
	};
};
#endif
