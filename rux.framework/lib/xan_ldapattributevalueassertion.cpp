#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_ldapattributevalueassertion.h>
#include <xan_keyvaluepair.h>
#include <xan_uint64.h>
#include <xan_boolean.h>
begin_implement_rux_class_ns( LdapAttributeValueAssertion , rux::network );
end_implement_rux_class();
namespace rux
{
	namespace network
	{
		void XLdapAttributeValueAssertion::set_AttributeDescription( const rux::XString& description )
		{
			(*this)()->_attribute_description = description.ConvertToUTF8();
		};
		void XLdapAttributeValueAssertion::set_AttributeValue( const rux::XString& value )
		{
			(*this)()->_attribute_value = value.ConvertToUTF8();
		};
	};
};