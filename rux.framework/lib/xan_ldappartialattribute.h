#ifndef XAN_LDAPPARTIALATTRIBUTE_H
#define XAN_LDAPPARTIALATTRIBUTE_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_string.h>
namespace rux
{
	namespace network
	{
		begin_declare_rux_class( LdapPartialAttribute );
			friend class XLdapPacket;
			friend class XLdap;
		begin_declare_rux_class_members( LdapPartialAttribute );
			rux::XString _attribute_type;
			rux::XArray<rux::XString> _attributes;
		end_declare_rux_class_members( LdapPartialAttribute );
			rux::Array<rux::XString>& get_Attributes( void );
			rux::String& get_AttributeType( void );
		end_declare_rux_class();
	};
};
#endif
