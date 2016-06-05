#ifndef XAN_LDAPUNBINDREQUEST_H
#define XAN_LDAPUNBINDREQUEST_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_ldapcontrol.h>
namespace rux
{
	namespace network
	{
		begin_declare_rux_class( LdapUnbindRequest );
			friend class XLdapPacket;
		begin_declare_rux_class_members( LdapUnbindRequest );
			rux::uint32 _message_id;
		end_declare_rux_class_members( LdapUnbindRequest );
			void set_MessageId( rux::uint32 message_id );	
		end_declare_rux_class();
	};
};
#endif
