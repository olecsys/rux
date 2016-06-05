#ifndef XAN_LDAPSEARCHRESULTENTRYPACKET_H
#define XAN_LDAPSEARCHRESULTENTRYPACKET_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_ldappartialattribute.h>
#include <xan_asn_defines.h>
namespace rux
{
	namespace network
	{
		begin_declare_rux_class( LdapSearchResultEntry );
			friend class XLdapPacket;
			friend class XLdap;
		begin_declare_rux_class_members( LdapSearchResultEntry );
			rux::uint32 _message_id;
			rux::XString _object_name;
			rux::XArray<XLdapPartialAttribute> _attributes;
		end_declare_rux_class_members( LdapSearchResultEntry );
			rux::Array<XLdapPartialAttribute>& get_PartialAttributes( void );	
		end_declare_rux_class();
	};
};
#endif
