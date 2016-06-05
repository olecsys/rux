#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_directory.h>
#include <xan_directxversion.h>
#include <xan_loader_defines.h>
#ifdef __WINDOWS__
#include <dxdiag.h>
#endif
#include <xan_fileversion.h>
#include <xan_keyvaluepair.h>
#include <xan_uint64.h>
#include <xan_boolean.h>
begin_implement_rux_class_ns( DirectXVersion , rux::diagnostics );
	_version_major = 0;
	_version_minor = 0;	
end_implement_rux_class();
namespace rux
{
	namespace diagnostics
	{
		rux::String& XDirectXVersion::ToString( void )
		{
			if( (*this)()->_version_major > 0 )
			{
				if( (*this)()->_version_letter.Length() == 0 || (*this)()->_version_letter == " " )
					return rux::XString::Format( "%ld.%ld" , (*this)()->_version_major , (*this)()->_version_minor );
				else
					return rux::XString::Format( "%ld.%ld%s" , (*this)()->_version_major , (*this)()->_version_minor , (*this)()->_version_letter() );
			}
			else
				return rux::XString( "Unknown" )++;
		};
		DirectXVersion& XDirectXVersion::get_DirectXVersion( rux::XString& error )
		{
			XDirectXVersion directx_version;
		#ifdef __WINDOWS__	
			error.Clear();
			uint8 is_found_directx_version = 0;		
			uint8 is_cleanup_com = SUCCEEDED( CoInitialize(NULL) ) ? 1 : 0;
			uint8 is_get_major = 0;
			uint8 is_get_minor = 0;
			uint8 is_get_letter = 0;
			IDxDiagProvider* dx_diag_provider_ptr = NULL;
			HRESULT hr = CoCreateInstance( CLSID_DxDiagProvider, NULL, CLSCTX_INPROC_SERVER, IID_IDxDiagProvider, (void**) &dx_diag_provider_ptr );
			if( SUCCEEDED( hr ) )
			{
				DXDIAG_INIT_PARAMS dx_diag_init_param;
				memset( &dx_diag_init_param, 0 , sizeof( DXDIAG_INIT_PARAMS ) );
				dx_diag_init_param.dwSize = sizeof(DXDIAG_INIT_PARAMS);
				dx_diag_init_param.dwDxDiagHeaderVersion = DXDIAG_DX9_SDK_VERSION;
				dx_diag_init_param.bAllowWHQLChecks = false;
				dx_diag_init_param.pReserved = NULL;
				hr = dx_diag_provider_ptr->Initialize( &dx_diag_init_param );
				if( SUCCEEDED(hr) )
				{
					IDxDiagContainer* dx_diag_root_ptr = NULL;
					IDxDiagContainer* dx_diag_system_info_ptr = NULL;
					hr = dx_diag_provider_ptr->GetRootContainer( &dx_diag_root_ptr );
					if( SUCCEEDED(hr) )
					{
						hr = dx_diag_root_ptr->GetChildContainer( L"DxDiag_SystemInfo", &dx_diag_system_info_ptr );					if( SUCCEEDED(hr) )
						{
							VARIANT var;
							VariantInit( &var );
							hr = dx_diag_system_info_ptr->GetProp( L"dwDirectXVersionMajor", &var );
							if( SUCCEEDED(hr) && var.vt == VT_UI4 )
							{
								directx_version()->_version_major = var.ulVal;
								is_get_major = 1;
							}
							VariantClear( &var );
							hr = dx_diag_system_info_ptr->GetProp( L"dwDirectXVersionMinor", &var );
							if( SUCCEEDED(hr) && var.vt == VT_UI4 )
							{
								directx_version()->_version_minor = var.ulVal;
								is_get_minor = 1;
							}
							VariantClear( &var );
							hr = dx_diag_system_info_ptr->GetProp( L"szDirectXVersionLetter", &var );
							if( SUCCEEDED(hr) && var.vt == VT_BSTR && SysStringLen( var.bstrVal ) != 0 )
							{
								directx_version()->_version_letter = rux::XString( (wchar_t*)var.bstrVal ).ToLower();
								is_get_letter = 1;
							}
							VariantClear( &var );
							is_found_directx_version = is_get_major == 1 && is_get_minor == 1 && is_get_letter == 1 ? 1 : 0;
							dx_diag_system_info_ptr->Release();
						}
						dx_diag_root_ptr->Release();
					}
				}
				dx_diag_provider_ptr->Release();
			}
			if( is_cleanup_com == 1 )
				CoUninitialize();
			if( is_found_directx_version == 0 )
			{
				rux::XString system_path = rux::io::XDirectory::get_SpecialFolderPath( "SYSTEM" , error );
				if( error.Length() == 0 )
				{			
					rux::XString file_name = system_path + "/ddraw.dll";
					XFileVersion file_version = XFileVersion::get_FileVersion( file_name , error );
					if( error.Length() == 0 )
					{
						if( ( file_version()->_major > 4 || file_version()->_minor > 2 
							|| file_version()->_revision > 0 || file_version()->_build > 95 )
							|| ( file_version()->_major == 4 && file_version()->_minor == 2 
							&& file_version()->_revision == 0 && file_version()->_build == 95 ) )
						{
							directx_version()->_version_major = 1;
							directx_version()->_version_minor = 0;
							directx_version()->_version_letter = " ";
							is_found_directx_version = 1;
						}
						if( ( file_version()->_major > 4 || file_version()->_minor > 3 
							|| file_version()->_revision > 0 || file_version()->_build > 1096 )
							|| ( file_version()->_major == 4 && file_version()->_minor == 3 
							&& file_version()->_revision == 0 && file_version()->_build == 1096 ) )
						{
							directx_version()->_version_major = 2;
							directx_version()->_version_minor = 0;
							directx_version()->_version_letter = " ";
							is_found_directx_version = 1;
						}
						if( ( file_version()->_major > 4 || file_version()->_minor > 4 
							|| file_version()->_revision > 0 || file_version()->_build > 68 )
							|| ( file_version()->_major == 4 && file_version()->_minor == 4 
							&& file_version()->_revision == 0 && file_version()->_build == 68 ) )					
						{
							directx_version()->_version_major = 3;
							directx_version()->_version_minor = 0;
							directx_version()->_version_letter = " ";
							is_found_directx_version = 1;
						}
					}
					file_name = system_path + "/d3drg8x.dll";
					file_version = XFileVersion::get_FileVersion( file_name , error );
					if( error.Length() == 0 )
					{
						if( ( file_version()->_major > 4 || file_version()->_minor > 4 
							|| file_version()->_revision > 0 || file_version()->_build > 70 )
							|| ( file_version()->_major == 4 && file_version()->_minor == 4 
							&& file_version()->_revision == 0 && file_version()->_build == 70 ) )					
						{
							directx_version()->_version_major = 3;
							directx_version()->_version_minor = 0;
							directx_version()->_version_letter = "a";
							is_found_directx_version = 1;
						}							
					}
					file_name = system_path + "/ddraw.dll";
					file_version = XFileVersion::get_FileVersion( file_name , error );
					if( error.Length() == 0 )
					{
						if( ( file_version()->_major > 4 || file_version()->_minor > 5 
							|| file_version()->_revision > 0 || file_version()->_build > 155 )
							|| ( file_version()->_major == 4 && file_version()->_minor == 5 
							&& file_version()->_revision == 0 && file_version()->_build == 155 ) )					
						{
							directx_version()->_version_major = 5;
							directx_version()->_version_minor = 0;
							directx_version()->_version_letter = " ";
							is_found_directx_version = 1;
						}		
						if( ( file_version()->_major > 4 || file_version()->_minor > 6 
							|| file_version()->_revision > 0 || file_version()->_build > 318 )
							|| ( file_version()->_major == 4 && file_version()->_minor == 6 
							&& file_version()->_revision == 0 && file_version()->_build == 318 ) )					
						{
							directx_version()->_version_major = 6;
							directx_version()->_version_minor = 0;
							directx_version()->_version_letter = " ";
							is_found_directx_version = 1;
						}	
						if( ( file_version()->_major > 4 || file_version()->_minor > 6 
							|| file_version()->_revision > 0 || file_version()->_build > 436 )
							|| ( file_version()->_major == 4 && file_version()->_minor == 6 
							&& file_version()->_revision == 0 && file_version()->_build == 436 ) )					
						{
							directx_version()->_version_major = 6;
							directx_version()->_version_minor = 1;
							directx_version()->_version_letter = " ";
							is_found_directx_version = 1;
						}
						if( ( file_version()->_major > 4 || file_version()->_minor > 7 
							|| file_version()->_revision > 0 || file_version()->_build > 700 )
							|| ( file_version()->_major == 4 && file_version()->_minor == 7 
							&& file_version()->_revision == 0 && file_version()->_build == 700 ) )					
						{
							directx_version()->_version_major = 7;
							directx_version()->_version_minor = 0;
							directx_version()->_version_letter = " ";
							is_found_directx_version = 1;
						}
						if( ( file_version()->_major == 4 && ( ( file_version()->_minor > 8 
							|| file_version()->_revision > 0 || file_version()->_build > 400 )
							|| ( file_version()->_minor == 8 
							&& file_version()->_revision == 0 && file_version()->_build == 400 ) ) )
							|| ( file_version()->_major == 5 && ( ( file_version()->_minor > 1 
							|| file_version()->_revision > 2258 || file_version()->_build > 400 )
							|| ( file_version()->_minor == 1 
							&& file_version()->_revision == 2258 && file_version()->_build == 400 ) ) ) )
						{
							directx_version()->_version_major = 8;
							directx_version()->_version_minor = 0;
							directx_version()->_version_letter = " ";
							is_found_directx_version = 1;
						}
					}
					if( directx_version()->_version_major <= 6 )
					{
						file_name = system_path + "/dplayx.dll";
						file_version = XFileVersion::get_FileVersion( file_name , error );
						if( error.Length() == 0 )
						{
							if( ( file_version()->_major > 4 || file_version()->_minor > 6 
								|| file_version()->_revision > 3 || file_version()->_build > 518 )
								|| ( file_version()->_major == 4 && file_version()->_minor == 6 
								&& file_version()->_revision == 3 && file_version()->_build == 518 ) )					
							{
								directx_version()->_version_major = 6;
								directx_version()->_version_minor = 1;
								directx_version()->_version_letter = "a";
								is_found_directx_version = 1;
							}							
						}
					}
					if( directx_version()->_version_major <= 7 )
					{
						file_name = system_path + "/dinput.dll";
						file_version = XFileVersion::get_FileVersion( file_name , error );
						if( error.Length() == 0 )
						{
							if( ( file_version()->_major > 4 || file_version()->_minor > 7 
								|| file_version()->_revision > 0 || file_version()->_build > 716 )
								|| ( file_version()->_major == 4 && file_version()->_minor == 7 
								&& file_version()->_revision == 0 && file_version()->_build == 716 ) ) 
							{
								directx_version()->_version_major = 7;
								directx_version()->_version_minor = 0;
								directx_version()->_version_letter = "a";
								is_found_directx_version = 1;
							}							
						}
					}
					file_name = system_path + "/d3d8.dll";
					file_version = XFileVersion::get_FileVersion( file_name , error );
					if( error.Length() == 0 )
					{
						if( ( file_version()->_major == 4 && ( ( file_version()->_minor > 8 
							|| file_version()->_revision > 1 || file_version()->_build > 881 )
							|| ( file_version()->_minor == 8 
							&& file_version()->_revision == 1 && file_version()->_build == 881 ) ) )
							|| ( file_version()->_major == 5 && ( ( file_version()->_minor > 1 
							|| file_version()->_revision > 2600 || file_version()->_build > 881 )
							|| ( file_version()->_minor == 1 
							&& file_version()->_revision == 2600 && file_version()->_build == 881 ) ) ) )
						{
							directx_version()->_version_major = 8;
							directx_version()->_version_minor = 1;
							directx_version()->_version_letter = " ";
							is_found_directx_version = 1;
						}							
						if( ( file_version()->_major == 4 && ( ( file_version()->_minor > 8 
							|| file_version()->_revision > 1 || file_version()->_build > 901 )
							|| ( file_version()->_minor == 8 
							&& file_version()->_revision == 1 && file_version()->_build == 901 ) ) )
							|| ( file_version()->_major == 5 && ( ( file_version()->_minor > 1 
							|| file_version()->_revision > 2600 || file_version()->_build > 901 )
							|| ( file_version()->_minor == 1 
							&& file_version()->_revision == 2600 && file_version()->_build == 901 ) ) ) )
						{
							directx_version()->_version_major = 8;
							directx_version()->_version_minor = 1;
							directx_version()->_version_letter = "a";
							is_found_directx_version = 1;
						}							
					}
					file_name = system_path + "/mpg2splt.ax";
					file_version = XFileVersion::get_FileVersion( file_name , error );
					if( error.Length() == 0 )
					{
						if( ( file_version()->_major > 6 || file_version()->_minor > 3 
							|| file_version()->_revision > 1 || file_version()->_build > 885 )
							|| ( file_version()->_major == 6 && file_version()->_minor == 3 
							&& file_version()->_revision == 1 && file_version()->_build == 885 ) ) 
						{
							directx_version()->_version_major = 8;
							directx_version()->_version_minor = 1;
							directx_version()->_version_letter = "b";
							is_found_directx_version = 1;
						}							
					}
					file_name = system_path + "/dpnet.dll";
					file_version = XFileVersion::get_FileVersion( file_name , error );
					if( error.Length() == 0 )
					{
						if( ( file_version()->_major == 4 && ( ( file_version()->_minor > 9 
							|| file_version()->_revision > 0 || file_version()->_build > 134 )
							|| ( file_version()->_minor == 9 
							&& file_version()->_revision == 0 && file_version()->_build == 134 ) ) )
							|| ( file_version()->_major == 5 && ( ( file_version()->_minor > 2 
							|| file_version()->_revision > 3677 || file_version()->_build > 134 )
							|| ( file_version()->_minor == 2 
							&& file_version()->_revision == 3677 && file_version()->_build == 134 ) ) ) )
						{
							directx_version()->_version_major = 8;
							directx_version()->_version_minor = 2;
							directx_version()->_version_letter = " ";
							is_found_directx_version = 1;
						}																
					}
					file_name = system_path + "/d3d9.dll";
					file_version = XFileVersion::get_FileVersion( file_name , error );
					if( error.Length() == 0 )
					{
						directx_version()->_version_major = 9;
						directx_version()->_version_minor = 0;
						directx_version()->_version_letter = " ";
						is_found_directx_version = 1;
					}
				}
			}		
			if( is_found_directx_version == 0 && error.Length() == 0 )
				error = "DirectX is not installed";
		#endif
			return directx_version++;
		};
	};
};