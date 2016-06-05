#ifndef XAN_LDAPPACKET_H
#define XAN_LDAPPACKET_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_string.h>
#include <xan_ldapbindrequest.h>
#include <xan_ldapsearchrequest.h>
#include <xan_ldapbindresponse.h>
#include <xan_ldapsearchresultentry.h>
#include <xan_ldapunbindrequest.h>
namespace rux
{
	namespace network
	{
		begin_declare_rux_class( LdapPacket );
		begin_declare_rux_class_members( LdapPacket );
			XObject _ldap_content;	
		end_declare_rux_class_members( LdapPacket );
			void set_LdapContent( const XObject& content );
			Object& get_LdapContent( void );
			static void NetworkFormat( const XObject& send_data , rux::uint8*& network_buffer_ptr , rux::uint32& network_buffer_size , rux::XString& error );
			static rux::uint32 NetworkParse( const XObject& receive_data , rux::uint8* network_buffer_ptr , rux::uint32 network_buffer_size , rux::XString& error );
			static rux::uint32 NetworkGetLength( rux::uint8* network_buffer_ptr , rux::uint32 network_buffer_size , rux::XString& error );
		end_declare_rux_class();
	};
};
#endif
