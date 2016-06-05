#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_ldapsearchrequest.h>
#include <xan_keyvaluepair.h>
#include <xan_uint64.h>
#include <xan_boolean.h>
begin_implement_rux_class_ns( LdapSearchRequest , rux::network );
	_message_id = 0;	
	_scope = XEnum_LdapSearchScope_baseObject;
	_deref_aliases = XEnum_LdapSearchAlias_derefInSearching;
	_size_limit = 0;
	_time_limit = 0;
	_types_only = 0;	
end_implement_rux_class();
namespace rux
{
	namespace network
	{
		void XLdapSearchRequest::set_MessageId( rux::uint32 message_id )
		{
			(*this)()->_message_id = message_id;
		};
		void XLdapSearchRequest::set_BaseObject( const rux::XString& base_object )
		{
			(*this)()->_base_object = base_object;
		};
		void XLdapSearchRequest::set_Scope( XEnum_LdapSearchScope scope )
		{
			(*this)()->_scope = scope;
		};
		void XLdapSearchRequest::set_DerefAliases( XEnum_LdapSearchAlias deref_aliases )
		{
			(*this)()->_deref_aliases = deref_aliases;
		};
		void XLdapSearchRequest::set_SizeLimit( rux::uint32 size_limit )
		{
			(*this)()->_size_limit = size_limit;
		};
		void XLdapSearchRequest::set_TimeLimit( rux::uint32 time_limit )
		{
			(*this)()->_time_limit = time_limit;
		};
		void XLdapSearchRequest::set_TypesOnly( rux::uint8 types_only )
		{
			if( types_only > 0 )
				(*this)()->_types_only = 1;
			else
				(*this)()->_types_only = 0;
		};
		void XLdapSearchRequest::AddAttribute( const rux::XString& attribute )
		{
			(*this)()->_attributes.Add( attribute );
		};
		void XLdapSearchRequest::AddControl( const XLdapControl& control )
		{
			(*this)()->_controls.Add( control );
		};
		void XLdapSearchRequest::set_Filter( const XLdapFilter& filter )
		{
			(*this)()->_filter = filter;
		};
		void XLdapSearchRequest::Clear( void )
		{
			(*this)()->_attributes.Clear();
			(*this)()->_controls.Clear();
		};
		void XLdapSearchRequest::ClearAttributes( void )
		{
			(*this)()->_attributes.Clear();
		};
		void XLdapSearchRequest::ClearControls( void )
		{
			(*this)()->_controls.Clear();
		};
	};
};