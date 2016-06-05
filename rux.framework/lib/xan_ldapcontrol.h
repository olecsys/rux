#ifndef XAN_LDAPCONTROL_H
#define XAN_LDAPCONTROL_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_string.h>
namespace rux
{
	namespace network
	{
		begin_declare_rux_class( LdapControl );
			friend class XLdapPacket;
			friend class XLdapSearchRequest;
		begin_declare_rux_class_members( LdapControl );
			rux::XString _control_type;
			rux::uint8 _criticality;
			rux::XString _control_value;
		end_declare_rux_class_members( LdapControl );
			XLdapControl( const rux::XString& control_type );
			XLdapControl( const rux::XString& control_type , rux::uint8 criticality );
			XLdapControl( const rux::XString& control_type , rux::uint8 criticality , const rux::XString& control_value );
			void set_ControlType( const rux::XString& control_type );
			void set_Criticality( rux::uint8 criticality );
			void set_ControlValue( const rux::XString& control_value );
		end_declare_rux_class();
	};
};
#endif
