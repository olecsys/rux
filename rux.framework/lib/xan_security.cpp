#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_security.h"
#include "xan_error.h"
#include "xan_uint32.h"
#ifdef __WINDOWS__
#include <AclAPI.h>
#include <WtsApi32.h>
#include <UserEnv.h>
#define GENERIC_ALL_ACCESS 0x10000000
#endif
#ifdef __WINDOWS__
PSECURITY_DESCRIPTOR XSystemSecurity::CreateSecurityDescriptor( ::rux::XString& error )
{
	PSECURITY_DESCRIPTOR security_descriptor = (PSECURITY_DESCRIPTOR)LocalAlloc( LPTR , SECURITY_DESCRIPTOR_MIN_LENGTH ); 
	if( InitializeSecurityDescriptor( security_descriptor , SECURITY_DESCRIPTOR_REVISION ) == FALSE )
	{
		rux_write_error( XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError ) , error );
		LocalFree( security_descriptor );
		security_descriptor = NULL;
	}
	return security_descriptor;
};
void XSystemSecurity::AddAclToSecurityDescriptor( ::rux::XArray< ::rux::XString >& user_or_group_array , ::rux::uint8* is_allowed , DWORD* access_mask_array , XSecurityData& security_data , ::rux::XString& error )
{
	error.Clear();
	if( security_data._security_attributes == NULL )
	{
		security_data._security_attributes = (SECURITY_ATTRIBUTES*)alloc_array_macros( ::rux::uint8 , sizeof( SECURITY_ATTRIBUTES ) );
		security_data._security_attributes->nLength = sizeof( SECURITY_ATTRIBUTES );
		security_data._security_attributes->bInheritHandle = FALSE;
	}
	if( security_data._security_attributes->lpSecurityDescriptor == NULL )
		security_data._security_attributes->lpSecurityDescriptor = CreateSecurityDescriptor( error );
	if( security_data._security_attributes->lpSecurityDescriptor )
	{
		WELL_KNOWN_SID_TYPE well_known_sid_type = WinNullSid;
		::rux::XString lower_user_or_group;
		::rux::uint32 acl_size = sizeof( ACL );
		::rux::uint32 index0 = 0;
		unsigned long sid_size = SECURITY_MAX_SID_SIZE;
		PSID sid_ptr = NULL;
		for( index0 = 0 ; index0 < user_or_group_array.Count() ; index0++ )
		{
			if( user_or_group_array[ index0 ].Length() > 0 )
			{
				lower_user_or_group.set_ByRef( user_or_group_array[ index0 ].ToLower() );
				well_known_sid_type = WinNullSid;
				if( lower_user_or_group == "network service" )
					well_known_sid_type = WinNetworkServiceSid;	
				else if( lower_user_or_group == "everyone" )
					well_known_sid_type = WinWorldSid;	
				else if( lower_user_or_group == "network" )
					well_known_sid_type = WinNetworkSid;	
				sid_size = SECURITY_MAX_SID_SIZE;
				sid_ptr = LocalAlloc( LMEM_FIXED , sid_size );
				if( sid_ptr )
				{
					if( CreateWellKnownSid( well_known_sid_type , NULL , sid_ptr , &sid_size ) )
					{					
						if( is_allowed[ index0 ] == 1 )
							acl_size += sizeof( ACCESS_ALLOWED_ACE ) - sizeof( DWORD );
						else if( is_allowed[ index0 ] == 0 )
							acl_size += sizeof( ACCESS_DENIED_ACE ) - sizeof( DWORD );
						acl_size += GetLengthSid( sid_ptr );							
					}
					else
					{
						rux_write_error( XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError ) , error );
						acl_size = 0;
						break;
					}
					LocalFree( sid_ptr );
				}
			}
		}
		if( acl_size > 0 )
			security_data._pacl = (PACL)LocalAlloc( LPTR , acl_size );
		if( security_data._pacl && InitializeAcl( security_data._pacl , acl_size , ACL_REVISION ) )
		{
			for( index0 = 0 ; index0 < user_or_group_array.Count() ; index0++ )
			{
				if( user_or_group_array[ index0 ].Length() > 0 )
				{
					lower_user_or_group.set_ByRef( user_or_group_array[ index0 ].ToLower() );
					well_known_sid_type = WinNullSid;
					if( lower_user_or_group == "network service" )
						well_known_sid_type = WinNetworkServiceSid;	
					else if( lower_user_or_group == "everyone" )
						well_known_sid_type = WinWorldSid;	
					else if( lower_user_or_group == "network" )
						well_known_sid_type = WinNetworkSid;	
					sid_size = SECURITY_MAX_SID_SIZE;
					sid_ptr = LocalAlloc( LMEM_FIXED , sid_size );
					if( sid_ptr )
					{
						if( CreateWellKnownSid( well_known_sid_type , NULL , sid_ptr , &sid_size ) )
						{
							if( ( is_allowed[ index0 ] == 1 && AddAccessAllowedAce( security_data._pacl , ACL_REVISION , access_mask_array[ index0 ] , sid_ptr ) )
								|| ( is_allowed[ index0 ] == 0 && AddAccessDeniedAce( security_data._pacl , ACL_REVISION , access_mask_array[ index0 ] , sid_ptr ) ) )
							{
							}
							else
							{
								rux_write_error( XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError ) , error );								
								break;
							}													
						}
						else
						{
							rux_write_error( XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError ) , error );							
							break;
						}
						LocalFree( sid_ptr );
					}
				}
			}
		}
		else
		{
			LocalFree( security_data._pacl );
			security_data._pacl = NULL;
		}
		if( security_data._pacl )
		{
			if( SetSecurityDescriptorDacl( security_data._security_attributes->lpSecurityDescriptor , TRUE, security_data._pacl , TRUE ) == FALSE )
			{
				rux_write_error( XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError ) , error );				
			}
		}
	}
	else
	{
		::rux::engine::free_mem( security_data._security_attributes );
		security_data._security_attributes = NULL;
	}
	if( error.Length() > 0 )
	{
		if( security_data._pacl )
		{
			LocalFree( security_data._pacl );
			security_data._pacl = NULL;
		}
		if( security_data._security_attributes )
		{
			if( security_data._security_attributes->lpSecurityDescriptor )
			{
				LocalFree( security_data._security_attributes->lpSecurityDescriptor );
				security_data._security_attributes->lpSecurityDescriptor = NULL;
			}
			::rux::engine::free_mem( security_data._security_attributes );
			security_data._security_attributes = NULL;
		}
	}
};
void XSystemSecurity::ExecuteByUser( const ::rux::XString& login , const ::rux::XString& password , rux_simple_t function , ::rux::XString& error )
{
	error.Clear();
	#ifdef __WINDOWS__
	HANDLE token = NULL;
	::rux::XString login_utf16 , password_utf16;
	login_utf16.set_ByRef( login.ConvertToUTF16() );
	password_utf16.set_ByRef( password.ConvertToUTF16() );	
	if( LogonUserW( (wchar_t*)login_utf16.str() , L"." , (wchar_t*)password_utf16.str() , LOGON32_LOGON_INTERACTIVE , LOGON32_PROVIDER_DEFAULT , &token ) )
	{
		::rux::engine::_globals->_rux_memory_add_descriptor( _rux_current_module_index , __FILE__ , token );
		if( ImpersonateLoggedOnUser( token ) )
		{
			if( function )
				function();
			RevertToSelf();
		}
		else
		{
			rux_write_error( XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError ) , error );
		}
		CloseHandle( token );
		::rux::engine::_globals->_rux_memory_remove_descriptor( token , _rux_current_module_index );
	}
	else
	{
		rux_write_error( XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError ) , error );
	}
#endif
};
#endif
void XSystemSecurity::ExecuteByUser( const ::rux::XString& login , const ::rux::XString& password , const ::rux::XString& executable , ::rux::XArray< ::rux::XString >& arguments , ::rux::XString& error )
{
	error.Clear();
#ifdef __WINDOWS__
	HANDLE token = NULL;
	::rux::XString login_utf16 , password_utf16;
	login_utf16.set_ByRef( login.ConvertToUTF16() );
	password_utf16.set_ByRef( password.ConvertToUTF16() );	
	if( LogonUserW( (wchar_t*)login_utf16.str() , L"." , (wchar_t*)password_utf16.str() , LOGON32_LOGON_INTERACTIVE , LOGON32_PROVIDER_DEFAULT , &token ) )
	{
		::rux::engine::_globals->_rux_memory_add_descriptor( _rux_current_module_index , __FILE__ , token );
		if( ImpersonateLoggedOnUser( token ) )
		{
			void* environment = NULL;        
			if( ::rux::engine::_globals->_userenv_module.CreateEnvironmentBlock( &environment , token , FALSE ) )
			{
				PROCESS_INFORMATION pi = {0};
				STARTUPINFOW si = {0};
				::rux::XString utf16_argument( executable );
				for( ::rux::uint32 index0 = 0 ; index0 < arguments.Count() ; index0++ )
				{
					utf16_argument += " " + arguments[ index0 ];
				}
				utf16_argument.set_ByRef( utf16_argument.ConvertToUTF16() );
				wchar_t user_profile[ 256 ] = L"";
				DWORD size = sizeof( user_profile ) / sizeof( wchar_t );
				if( ::rux::engine::_globals->_userenv_module.GetUserProfileDirectoryW( token , user_profile , &size ) )
				{
					if( CreateProcessWithLogonW( (wchar_t*)login_utf16.str() , L"." , (wchar_t*)password_utf16.str() , LOGON_WITH_PROFILE , NULL , (wchar_t*)utf16_argument.str() , CREATE_UNICODE_ENVIRONMENT , environment , user_profile , &si , &pi ) == 0 )
					{
						rux_write_error( XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError ) , error );
					}
					else
					{
						if( pi.hProcess != NULL ) 
							::rux::engine::_globals->_rux_memory_add_descriptor( _rux_current_module_index , __FILE__ , pi.hProcess );
						if( pi.hThread != NULL ) 
							::rux::engine::_globals->_rux_memory_add_descriptor( _rux_current_module_index , __FILE__ , pi.hThread );
					}
				}
				else
				{
					rux_write_error( XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError ) , error );
				}
				if( pi.hProcess != NULL ) 
				{
					CloseHandle( pi.hProcess );
					::rux::engine::_globals->_rux_memory_remove_descriptor( pi.hProcess , _rux_current_module_index );
				}
				if( pi.hThread != NULL ) 
				{
					CloseHandle( pi.hThread );
					::rux::engine::_globals->_rux_memory_remove_descriptor( pi.hThread , _rux_current_module_index );
				}
				::rux::engine::_globals->_userenv_module.DestroyEnvironmentBlock( environment );
			}		
			else
			{
				rux_write_error( XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError ) , error );
			}
			RevertToSelf();
		}
		else
		{
			rux_write_error( XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError ) , error );
		}
		CloseHandle( token );
		::rux::engine::_globals->_rux_memory_remove_descriptor( token , _rux_current_module_index );
	}
	else
	{
		rux_write_error( XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError ) , error );
	}
#endif
};
rux::uint8 XSystemSecurity::IsAdminUserProcess( void )
{
	::rux::uint8 is_admin_user = 0;
#ifdef __WINDOWS__
	SID_IDENTIFIER_AUTHORITY nt_authority = SECURITY_NT_AUTHORITY;
	PSID administrators_group = NULL; 
	BOOL is_admin = AllocateAndInitializeSid( &nt_authority , 2 , SECURITY_BUILTIN_DOMAIN_RID , DOMAIN_ALIAS_RID_ADMINS , 0 , 0 , 0 , 0 , 0 , 0 , &administrators_group ); 
	if( is_admin ) 
	{
		if( CheckTokenMembership( NULL , administrators_group , &is_admin ) == FALSE )
			is_admin = FALSE;
		FreeSid( administrators_group ); 
	}
	is_admin_user = is_admin ? 1 : 0;
#else
	is_admin_user = geteuid() == 0 ? 1 : 0;
#endif
	return is_admin_user;
};
