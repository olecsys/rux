#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_ldapsearchresultentry.h>
#include <xan_keyvaluepair.h>
#include <xan_uint64.h>
#include <xan_boolean.h>
begin_implement_rux_class_ns( LdapSearchResultEntry , rux::network );
	_message_id = 0;	
end_implement_rux_class();
namespace rux
{
	namespace network
	{
		rux::Array<XLdapPartialAttribute>& XLdapSearchResultEntry::get_PartialAttributes( void )
		{
			return (*this)()->_attributes++;
		};
	};
};