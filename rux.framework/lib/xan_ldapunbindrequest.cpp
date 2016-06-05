#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_ldapunbindrequest.h>
#include <xan_keyvaluepair.h>
#include <xan_uint64.h>
#include <xan_boolean.h>
begin_implement_rux_class_ns( LdapUnbindRequest , rux::network );
	_message_id = 0;		
end_implement_rux_class();
namespace rux
{
	namespace network
	{
		void XLdapUnbindRequest::set_MessageId( rux::uint32 message_id )
		{
			(*this)()->_message_id = message_id;
		};
	};
};