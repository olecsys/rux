#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_ldapbindresponse.h>
#include <xan_keyvaluepair.h>
#include <xan_uint64.h>
#include <xan_boolean.h>
begin_implement_rux_class_ns( LdapBindResponse , rux::network );
	_message_id = 0;
	_result_code = XEnum_LdapResultCode_success;	
end_implement_rux_class();
namespace rux
{
	namespace network
	{		
		XEnum_LdapResultCode XLdapBindResponse::get_ResultCode( void )
		{
			return (*this)()->_result_code;
		};
	};
};