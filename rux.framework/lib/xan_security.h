#ifndef XAN_SECURITY_H
#define XAN_SECURITY_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_string.h"
struct XSecurityData
{
#ifdef __WINDOWS__
	SECURITY_ATTRIBUTES* _security_attributes;
	PACL _pacl;
#endif
	XSecurityData()
	{
#ifdef __WINDOWS__		
		_security_attributes = NULL;
		_pacl = NULL;
#endif
	};
	~XSecurityData()
	{
#ifdef __WINDOWS__		
		if( _pacl )
			LocalFree( _pacl );
		if( _security_attributes )
		{
			if( _security_attributes->lpSecurityDescriptor )
				LocalFree( _security_attributes->lpSecurityDescriptor );
			::rux::engine::free_mem( _security_attributes );
		}		
#endif		
	};
};
class XSystemSecurity
{
public:
#ifdef __WINDOWS__
	static void AddAclToSecurityDescriptor( ::rux::XArray< ::rux::XString >& user_or_group_array , ::rux::uint8* is_allowed , DWORD* access_mask_array , XSecurityData& security_data , ::rux::XString& error );
	static PSECURITY_DESCRIPTOR CreateSecurityDescriptor( ::rux::XString& error );
	static void ExecuteByUser( const ::rux::XString& login , const ::rux::XString& password , rux_simple_t function , ::rux::XString& error );	
#endif
	static void ExecuteByUser( const ::rux::XString& login , const ::rux::XString& password , const ::rux::XString& executable , ::rux::XArray< ::rux::XString >& arguments , ::rux::XString& error );		
	static ::rux::uint8 IsAdminUserProcess( void );
};
#endif