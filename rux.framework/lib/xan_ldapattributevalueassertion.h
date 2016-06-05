#ifndef XAN_LDAPATTRIBUTEVALUEASSERTION_H
#define XAN_LDAPATTRIBUTEVALUEASSERTION_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_string.h>
namespace rux
{
	namespace network
	{
		begin_declare_rux_class( LdapAttributeValueAssertion );
			friend class XLdapPacket;
		begin_declare_rux_class_members( LdapAttributeValueAssertion );
			rux::XString _attribute_description;
			rux::XString _attribute_value;
		end_declare_rux_class_members( LdapAttributeValueAssertion );
			void set_AttributeDescription( const rux::XString& description );
			void set_AttributeValue( const rux::XString& value );
		end_declare_rux_class();
	};
};
#endif
