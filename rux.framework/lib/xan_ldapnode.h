#ifndef XAN_LDAPLEAF_H
#define XAN_LDAPLEAF_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_ldappartialattribute.h>
#include <xan_keyvaluepair.h>
namespace rux
{
	namespace network
	{
		class LdapNode;
		class XLdapNode : public XGCHandle<LdapNode>
		{
			declare_rux_base_methods(LdapNode);
		private:
		public:
			declare_rux_set_operators( LdapNode );
			XLdapNode();
			XLdapNode( const LdapNode& ldap_node );
			XLdapNode( const XLdapNode& ldap_node );
			explicit XLdapNode( const XObject& object );
			explicit XLdapNode( XGCRef* object );
		};
		class LdapNode : public XGCRef
		{
			declare_rux_base_internal_methods();
			friend class XLdapNode;
			friend class XLdap;
		private:
			rux::XString _object_name;
			rux::XArray< rux::XKeyValuePair<rux::XString> > _attributes;
			rux::XArray<XLdapNode> _nodes;
			void initialize_members( const char* variable_name , const char* __file__ , ::rux::int32 __line__ );
		public:
			explicit LdapNode( const char* variable_name = 0 , const char* __file__ = 0 , ::rux::int32 __line__ = 0 );
		};
	};
};
#endif
