#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_firewall.h>
#include <xan_path.h>
#include <xan_keyvaluepair.h>
namespace rux
{
	namespace network
	{
		rux::uint8 XFirewall::Allow( const rux::XString& program_path , rux::XString& error )
		{
			error.Clear();
			rux::uint8 result = 0;
		#ifdef __WINDOWS__
			rux::uint8 is_initialized = 0;
			if( FAILED( CoInitialize( NULL ) ) )
				is_initialized = 1;
			INetFwMgr* net_fw_mgr = NULL;
			INetFwPolicy* net_fw_policy = NULL;
			INetFwProfile* net_fw_profile = NULL;
			INetFwAuthorizedApplications* net_fw_authorized_applications = NULL;
			INetFwAuthorizedApplication* net_fw_authorized_application = NULL;		
			wchar_t* name_ptr = NULL;
			BSTR bstr_program_path = NULL;
			BSTR bstr_name = NULL;
			__try
			{		
				if( rux::io::XFile::Exists( program_path ) == 1 )
				{			
					rux::io::XPath::GetFileNameWithoutExtension( program_path , malloc , &name_ptr );			
					HRESULT hr = CoCreateInstance( __uuidof(NetFwMgr), NULL, CLSCTX_INPROC_SERVER, __uuidof(INetFwMgr), (void**)&net_fw_mgr );		
					if( SUCCEEDED( hr ) )
					{
						hr = net_fw_mgr->get_LocalPolicy( &net_fw_policy );			
						if( SUCCEEDED( hr ) )
						{
							hr = net_fw_policy->get_CurrentProfile( &net_fw_profile );				
							if( SUCCEEDED( hr ) )
							{
								hr = net_fw_profile->get_AuthorizedApplications( &net_fw_authorized_applications );					
								if( SUCCEEDED( hr ) )
								{
									hr = CoCreateInstance( __uuidof( NetFwAuthorizedApplication ) , NULL , CLSCTX_INPROC_SERVER , __uuidof( INetFwAuthorizedApplication ) , (void**)&net_fw_authorized_application );						
									if( SUCCEEDED( hr ) )
									{
										bstr_program_path = program_path.Convert();
										bstr_name = ::SysAllocString( name_ptr );
										net_fw_authorized_application->put_ProcessImageFileName( bstr_program_path );
										::SysFreeString( bstr_program_path );
										net_fw_authorized_application->put_Name( bstr_name );
										::SysFreeString( bstr_name );
										net_fw_authorized_application->put_Enabled( VARIANT_TRUE );
										net_fw_authorized_application->put_Scope( NET_FW_SCOPE_ALL );
										net_fw_authorized_application->put_IpVersion( NET_FW_IP_VERSION_ANY );													
										hr = net_fw_authorized_applications->Add( net_fw_authorized_application );
										if( SUCCEEDED( hr ) )
											result = 1;
										else
										{
											error = "Error : cannot add program '";
											error += program_path;
											error += rux::XString::Format( "' to firewall exceptions , result = %d" , hr );
										}
									}
									else
										error = rux::XString::Format( "Error : cannot create NetFwAuthorizedApplication , result = %d" , hr );							
								}
								else
									error = rux::XString::Format( "Error : cannot get authorized applications , result = %d" , hr );						
							}
							else					
								error = rux::XString::Format( "Error : cannot get current policy , result = %d" , hr );					
						}
						else				
							error = rux::XString::Format( "Error : cannot get local policy , result = %d" , hr );				
					}
					else			
						error = rux::XString::Format( "Error : cannot create NetFwMgr , result = %d" , hr );					
				}
				else
					error = "Error : cannot find file '" + program_path + "'";
			}	
			__except( EXCEPTION_CONTINUE_EXECUTION )
			{
			}
			if( net_fw_authorized_application )
			{
				net_fw_authorized_application->Release();
				net_fw_authorized_application = NULL;
			}
			if( net_fw_authorized_applications )
			{
				net_fw_authorized_applications->Release();
				net_fw_authorized_applications = NULL;
			}
			if( net_fw_profile )
			{
				net_fw_profile->Release();
				net_fw_profile = NULL;
			}				
			if( net_fw_policy )
			{
				net_fw_policy->Release();
				net_fw_policy = NULL;
			}		
			if( net_fw_mgr )
			{
				net_fw_mgr->Release();
				net_fw_mgr = NULL;
			}
			if( is_initialized == 0 )
				CoUninitialize();
			if( name_ptr )
				free( name_ptr );
		#endif
		#ifdef __UNIX__
			error = "Error : this function was not implement under LINUX";
		#endif
			return result;
		};
		rux::uint8 XFirewall::Allow( const rux::String& program_path , rux::XString& error )
		{
			program_path.AddRef();	
			wchar_t* local_program_path_ptr = NULL;
			program_path.ConvertToUTF16( malloc , (rux::uint16**)&local_program_path_ptr );	
			wchar_t* name_ptr = NULL;
			error.Clear();
			rux::uint8 result = 0;
		#ifdef __WINDOWS__	
			rux::uint8 is_initialized = 0;
			if( FAILED( CoInitialize( NULL ) ) )
				is_initialized = 1;			
			INetFwMgr* net_fw_mgr = NULL;
			INetFwPolicy* net_fw_policy = NULL;
			INetFwProfile* net_fw_profile = NULL;
			INetFwAuthorizedApplications* net_fw_authorized_applications = NULL;
			INetFwAuthorizedApplication* net_fw_authorized_application = NULL;			
			__try
			{
				if( rux::io::XFile::Exists( local_program_path_ptr ) == 1 )
				{			
					rux::io::XPath::GetFileNameWithoutExtension( local_program_path_ptr , malloc , &name_ptr );			
					HRESULT hr = CoCreateInstance( __uuidof(NetFwMgr), NULL, CLSCTX_INPROC_SERVER, __uuidof(INetFwMgr), (void**)&net_fw_mgr );		
					if( SUCCEEDED( hr ) )
					{
						hr = net_fw_mgr->get_LocalPolicy( &net_fw_policy );			
						if( SUCCEEDED( hr ) )
						{
							hr = net_fw_policy->get_CurrentProfile( &net_fw_profile );				
							if( SUCCEEDED( hr ) )
							{
								hr = net_fw_profile->get_AuthorizedApplications( &net_fw_authorized_applications );					
								if( SUCCEEDED( hr ) )
								{
									hr = CoCreateInstance( __uuidof( NetFwAuthorizedApplication ) , NULL , CLSCTX_INPROC_SERVER , __uuidof( INetFwAuthorizedApplication ) , (void**)&net_fw_authorized_application );						
									if( SUCCEEDED( hr ) )
									{
										BSTR bstr_program_path = ::SysAllocString( local_program_path_ptr );
										BSTR bstr_name = ::SysAllocString( name_ptr );
										net_fw_authorized_application->put_ProcessImageFileName( bstr_program_path );
										::SysFreeString( bstr_program_path );
										net_fw_authorized_application->put_Name( bstr_name );
										::SysFreeString( bstr_name );
										net_fw_authorized_application->put_Enabled( VARIANT_TRUE );
										net_fw_authorized_application->put_Scope( NET_FW_SCOPE_ALL );
										net_fw_authorized_application->put_IpVersion( NET_FW_IP_VERSION_ANY );													
										hr = net_fw_authorized_applications->Add( net_fw_authorized_application );								
										if( SUCCEEDED( hr ) )
											result = 1;
										else
										{
											error = "Error : cannot add program '";
											error += local_program_path_ptr;
											error += rux::XString::Format( "' to firewall exceptions , result = %d" , hr );
										}
									}
									else
										error = rux::XString::Format( "Error : cannot create NetFwAuthorizedApplication , result = %d" , hr );							
								}
								else
									error = rux::XString::Format( "Error : cannot get authorized applications , result = %d" , hr );						
							}
							else					
								error = rux::XString::Format( "Error : cannot get current profile , result = %d" , hr );					
						}
						else				
							error = rux::XString::Format( "Error : cannot get local policy , result = %d" , hr );				
					}
					else			
						error = rux::XString::Format( "Error : cannot create NetFwMgr , result = %d" , hr );						
				}
				else
				{
					error = "Error : cannot find file '";
					error += local_program_path_ptr;
					error += "'";
				}
			}	
			__except( EXCEPTION_CONTINUE_EXECUTION )
			{
			}
			if( net_fw_authorized_application )
			{
				net_fw_authorized_application->Release();
				net_fw_authorized_application = NULL;
			}
			if( net_fw_authorized_applications )
			{
				net_fw_authorized_applications->Release();
				net_fw_authorized_applications = NULL;
			}
			if( net_fw_profile )
			{
				net_fw_profile->Release();
				net_fw_profile = NULL;
			}				
			if( net_fw_policy )
			{
				net_fw_policy->Release();
				net_fw_policy = NULL;
			}		
			if( net_fw_mgr )
			{
				net_fw_mgr->Release();
				net_fw_mgr = NULL;
			}
			if( is_initialized == 0 )
				CoUninitialize();
			if( local_program_path_ptr )
				free( local_program_path_ptr );
			if( name_ptr )
				free( name_ptr );
		#endif
		#ifdef __UNIX__
			error = "Error : this function was not implement under LINUX";
		#endif
			program_path.Release();
			return result;
		};
		rux::uint8 XFirewall::Deny( const rux::XString& program_path , rux::XString& error )
		{
			rux::uint8 result = 0;
		#ifdef __WINDOWS__
			rux::uint8 is_initialized = 0;
			if( FAILED( CoInitialize( NULL ) ) )
				is_initialized = 1;		
			INetFwMgr* net_fw_mgr = NULL;
			INetFwPolicy* net_fw_policy = NULL;
			INetFwProfile* net_fw_profile = NULL;
			INetFwAuthorizedApplications* net_fw_authorized_applications = NULL;	
			__try
			{		
				HRESULT hr = CoCreateInstance( __uuidof(NetFwMgr), NULL, CLSCTX_INPROC_SERVER, __uuidof(INetFwMgr), (void**)&net_fw_mgr );		
				if( SUCCEEDED( hr ) )
				{			
					net_fw_mgr->get_LocalPolicy( &net_fw_policy );			
					if( SUCCEEDED( hr ) )
					{
						hr = net_fw_policy->get_CurrentProfile( &net_fw_profile );				
						if( SUCCEEDED( hr ) )
						{					
							hr = net_fw_profile->get_AuthorizedApplications( &net_fw_authorized_applications );
							if( SUCCEEDED( hr ) )
							{						
								BSTR bstr_program_path = program_path.Convert();						
								hr = net_fw_authorized_applications->Remove( bstr_program_path );	
								::SysFreeString( bstr_program_path );
								if( SUCCEEDED( hr ) )						
									result = 1;
								else
								{
									error = "Error : cannot remove program '";
									error += program_path;
									error += rux::XString::Format( "' from firewall exceptions , result = %d" , hr );
								}
							}					
							else
								error = rux::XString::Format( "Error : cannot get authorized applications , result = %d" , hr );
						}
						else					
							error = rux::XString::Format( "Error : cannot get current profile , result = %d" , hr );
					}
					else
						error = rux::XString::Format( "Error : cannot get local policy , result = %d" , hr );
				}		
				else
					error = rux::XString::Format( "Error : cannot create NetFwMgr , result = %d" , hr );		
			}
			__except( EXCEPTION_CONTINUE_EXECUTION )
			{		
			}	
			if( net_fw_authorized_applications )
			{
				net_fw_authorized_applications->Release();
				net_fw_authorized_applications = NULL;
			}
			if( net_fw_profile )
			{
				net_fw_profile->Release();
				net_fw_profile = NULL;
			}				
			if( net_fw_policy )
			{
				net_fw_policy->Release();
				net_fw_policy = NULL;
			}		
			if( net_fw_mgr )
			{
				net_fw_mgr->Release();
				net_fw_mgr = NULL;
			}
			if( is_initialized == 0 )
				CoUninitialize();
		#endif
		#ifdef __UNIX__
			error = "Error : this function was not implement under LINUX";
		#endif
			return result;
		};
		rux::uint8 XFirewall::Deny( const rux::String& program_path , rux::XString& error )
		{
			program_path.AddRef();
			rux::uint8 result = 0;
		#ifdef __WINDOWS__
			rux::uint8 is_initialized = 0;
			if( FAILED( CoInitialize( NULL ) ) )
				is_initialized = 1;	
			INetFwMgr* net_fw_mgr = NULL;
			INetFwPolicy* net_fw_policy = NULL;
			INetFwProfile* net_fw_profile = NULL;
			INetFwAuthorizedApplications* net_fw_authorized_applications = NULL;	
			__try
			{		
				HRESULT hr = CoCreateInstance( __uuidof(NetFwMgr), NULL, CLSCTX_INPROC_SERVER, __uuidof(INetFwMgr), (void**)&net_fw_mgr );		
				if( SUCCEEDED( hr ) )
				{
					net_fw_mgr->get_LocalPolicy( &net_fw_policy );			
					if( SUCCEEDED( hr ) )
					{
						hr = net_fw_policy->get_CurrentProfile( &net_fw_profile );				
						if( SUCCEEDED( hr ) )
						{				
							hr = net_fw_profile->get_AuthorizedApplications( &net_fw_authorized_applications );
							if( SUCCEEDED( hr ) )
							{
								BSTR bstr_program_path = program_path.Convert();
								hr = net_fw_authorized_applications->Remove( bstr_program_path );	
								::SysFreeString( bstr_program_path );
								if( SUCCEEDED( hr ) )						
									result = 1;
								else
								{
									error = "Error : cannot remove program '";
									error += program_path;
									error += rux::XString::Format( "' from firewall exceptions , result = %d" , hr );
								}
							}					
							else
								error = rux::XString::Format( "Error : cannot get authorized applications , result = %d" , hr );
						}
						else					
							error = rux::XString::Format( "Error : cannot get current profile , result = %d" , hr );
						}
					else
						error = rux::XString::Format( "Error : cannot get local policy , result = %d" , hr );			
				}		
				else
					error = rux::XString::Format( "Error : cannot create NetFwMgr , result = %d" , hr );				
			}
			__except( EXCEPTION_CONTINUE_EXECUTION )
			{
			}	
			if( net_fw_authorized_applications )
			{
				net_fw_authorized_applications->Release();
				net_fw_authorized_applications = NULL;
			}
			if( net_fw_profile )
			{
				net_fw_profile->Release();
				net_fw_profile = NULL;
			}				
			if( net_fw_policy )
			{
				net_fw_policy->Release();
				net_fw_policy = NULL;
			}		
			if( net_fw_mgr )
			{
				net_fw_mgr->Release();
				net_fw_mgr = NULL;
			}
			if( is_initialized == 0 )
				CoUninitialize();
		#endif
		#ifdef __UNIX__
			error = "Error : this function was not implement under LINUX";
		#endif
			program_path.Release();
			return result;
		};
	};
};