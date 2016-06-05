#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_ldapcontrol.h>
#include <xan_keyvaluepair.h>
#include <xan_uint64.h>
#include <xan_boolean.h>
begin_implement_rux_class_ns( LdapControl , rux::network );
	_criticality = 0;	
end_implement_rux_class();
namespace rux
{
	namespace network
	{		
		XLdapControl::XLdapControl( const rux::XString& control_type )
		{
			(*this)()->_control_type = control_type.ConvertToUTF8();
		};
		XLdapControl::XLdapControl( const rux::XString& control_type , rux::uint8 criticality )
		{
			(*this)()->_control_type = control_type.ConvertToUTF8();
			if( (*this)()->_criticality > 0 )
				(*this)()->_criticality = criticality;
			else
				(*this)()->_criticality = 0;
		};
		XLdapControl::XLdapControl( const rux::XString& control_type , rux::uint8 criticality , const rux::XString& control_value )
		{
			(*this)()->_control_type = control_type.ConvertToUTF8();
			if( (*this)()->_criticality > 0 )
				(*this)()->_criticality = criticality;
			else
				(*this)()->_criticality = 0;
			(*this)()->_control_value = control_value.ConvertToUTF8();
		};
		void XLdapControl::set_ControlType( const rux::XString& control_type )
		{
			(*this)()->_control_type = control_type.ConvertToUTF8();
		};
		void XLdapControl::set_Criticality( rux::uint8 criticality )
		{
			if( criticality > 0 )
				(*this)()->_criticality = 1;
			else
				(*this)()->_criticality = 0;
		};
		void XLdapControl::set_ControlValue( const rux::XString& control_value )
		{
			(*this)()->_control_value = control_value.ConvertToUTF8();
		};
	};
};