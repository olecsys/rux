#ifndef XAN_LDAPSEARCHREQUEST_H
#define XAN_LDAPSEARCHREQUEST_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_asn_defines.h>
#include <xan_ldapcontrol.h>
#include <xan_ldapfilter.h>
enum XEnum_LdapSearchScope
{
	XEnum_LdapSearchScope_baseObject,
	XEnum_LdapSearchScope_singleLevel,
	XEnum_LdapSearchScope_wholeSubtree	
};
enum XEnum_LdapSearchAlias
{
	XEnum_LdapSearchAlias_neverDerefAliases,
	XEnum_LdapSearchAlias_derefInSearching,
	XEnum_LdapSearchAlias_derefFindingBaseObj,
	XEnum_LdapSearchAlias_derefAlways
};
namespace rux
{
	namespace network
	{
		begin_declare_rux_class( LdapSearchRequest );
			friend class XLdapPacket;			
		begin_declare_rux_class_members( LdapSearchRequest );
			rux::uint32 _message_id;
			rux::XString _base_object;
			XEnum_LdapSearchScope _scope;
			XEnum_LdapSearchAlias _deref_aliases;
			rux::uint32 _size_limit;
			rux::uint32 _time_limit;
			rux::uint8 _types_only;
			rux::XArray<rux::XString> _attributes;
			rux::XArray<rux::network::XLdapControl> _controls;
			rux::network::XLdapFilter _filter;
		end_declare_rux_class_members( LdapSearchRequest );
			void set_MessageId( rux::uint32 message_id );
			void set_BaseObject( const rux::XString& base_object );
			void set_Scope( XEnum_LdapSearchScope scope );
			void set_DerefAliases( XEnum_LdapSearchAlias deref_aliases );
			void set_SizeLimit( rux::uint32 size_limit );
			void set_TimeLimit( rux::uint32 time_limit );
			void set_TypesOnly( rux::uint8 types_only );
			void AddAttribute( const rux::XString& attribute );
			void AddControl( const rux::network::XLdapControl& control );
			void set_Filter( const rux::network::XLdapFilter& filter );
			void Clear( void );
			void ClearAttributes( void );
			void ClearControls( void );	
		end_declare_rux_class();
	};
};
#endif
