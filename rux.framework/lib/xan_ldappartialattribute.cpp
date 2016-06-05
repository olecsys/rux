#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_ldappartialattribute.h>
#include <xan_keyvaluepair.h>
#include <xan_uint64.h>
#include <xan_boolean.h>
begin_implement_rux_class_ns( LdapPartialAttribute , rux::network );
end_implement_rux_class();
namespace rux
{
	namespace network
	{	
		rux::Array<rux::XString>& XLdapPartialAttribute::get_Attributes( void )
		{
			return (*this)()->_attributes++;
		};
		rux::String& XLdapPartialAttribute::get_AttributeType( void )
		{
			return (*this)()->_attribute_type++;
		};
	};
};