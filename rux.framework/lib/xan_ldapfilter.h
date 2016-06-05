#ifndef XAN_LDAPFILTER_H
#define XAN_LDAPFILTER_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_string.h>
#include <xan_ldapattributevalueassertion.h>
enum XEnum_LdapFilterType
{
	XEnum_LdapFilterType_and = 0 ,
	XEnum_LdapFilterType_or = 1 ,
	XEnum_LdapFilterType_not = 2 ,
	XEnum_LdapFilterType_equalityMatch = 3 ,
	XEnum_LdapFilterType_substrings = 4 ,
	XEnum_LdapFilterType_greaterOrEqual = 5 ,
	XEnum_LdapFilterType_lessOrEqual = 6 ,
	XEnum_LdapFilterType_present = 7 ,
	XEnum_LdapFilterType_aproxMatch = 8 ,
	XEnum_LdapFilterType_extensibleMatch = 9
};
namespace rux
{
	namespace network
	{
		begin_declare_rux_class( LdapFilter );
			friend class XLdapPacket;
			friend class XLdapSearchRequest;
		begin_declare_rux_class_members( LdapFilter );
			XEnum_LdapFilterType _filter_type;
			rux::XArray<XObject> _matches;
		end_declare_rux_class_members( LdapFilter );
			void set_FilterType( XEnum_LdapFilterType filter_type );
			void AddMatch( const XObject& match );
			void ClearMatches();
		end_declare_rux_class();
	};
};
#endif
