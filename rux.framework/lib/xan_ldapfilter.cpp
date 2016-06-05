#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_ldapfilter.h>
#include <xan_keyvaluepair.h>
#include <xan_uint64.h>
#include <xan_boolean.h>
begin_implement_rux_class_ns( LdapFilter , rux::network );
	_filter_type = XEnum_LdapFilterType_present;	
end_implement_rux_class();
namespace rux
{
	namespace network
	{			
		void XLdapFilter::set_FilterType( XEnum_LdapFilterType filter_type )
		{
			if( (*this)()->_filter_type != filter_type )
				(*this)()->_matches.Clear();
			(*this)()->_filter_type = filter_type;
		};
		void XLdapFilter::AddMatch( const XObject& match )
		{
			if( (*this)()->_filter_type == XEnum_LdapFilterType_present )
			{
				if( rux_is_object( match , rux::XString ) )
				{
					rux::XString utf8( match );
					utf8 = utf8.ConvertToUTF8();
					(*this)()->_matches.Add( utf8 );
				}
			}
			else if( (*this)()->_filter_type == XEnum_LdapFilterType_equalityMatch
				|| (*this)()->_filter_type == XEnum_LdapFilterType_greaterOrEqual
				|| (*this)()->_filter_type == XEnum_LdapFilterType_lessOrEqual		
				|| (*this)()->_filter_type == XEnum_LdapFilterType_aproxMatch )
			{
				if( rux_is_object( match , XLdapAttributeValueAssertion ) )		
					(*this)()->_matches.Add( match );
			}
		};
		void XLdapFilter::ClearMatches()
		{
			(*this)()->_matches.Clear();
		};
	};
};