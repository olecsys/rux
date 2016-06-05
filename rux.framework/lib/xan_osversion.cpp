#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_osversion.h>
#include <xan_loader_defines.h>
#include <xan_keyvaluepair.h>
#include <xan_uint64.h>
#include <xan_boolean.h>
begin_implement_rux_class_ns( OSVersion , rux::diagnostics );
	_os_version = XEnum_OSVersion_Unknown;
	_processor_architecture = XEnum_ProcessorArchitecture_X86;
end_implement_rux_class();
namespace rux
{
	namespace diagnostics
	{
		rux::String& XOSVersion::ToString( void )
		{
			rux::XString os_version( "Unknown" );
			switch( (*this)()->_os_version )
			{
			case XEnum_OSVersion_WindowsXP:
				{
					if( (*this)()->_processor_architecture == XEnum_ProcessorArchitecture_X64_AMD )
						os_version = "Windows XP , x64";
					else if( (*this)()->_processor_architecture == XEnum_ProcessorArchitecture_X64_IA )
						os_version = "Windows XP , x64 Itanium";
					else if( (*this)()->_processor_architecture == XEnum_ProcessorArchitecture_X86 )
						os_version = "Windows XP , x86";
					break;
				}
			case 	XEnum_OSVersion_WindowsServer2008:
				{
					if( (*this)()->_processor_architecture == XEnum_ProcessorArchitecture_X64_AMD )
						os_version = "Windows Server 2008 , x64";
					else if( (*this)()->_processor_architecture == XEnum_ProcessorArchitecture_X64_IA )
						os_version = "Windows Server 2008 , x64 Itanium";
					else if( (*this)()->_processor_architecture == XEnum_ProcessorArchitecture_X86 )
						os_version = "Windows Server 2008 , x86";
					break;
				}
			case 	XEnum_OSVersion_WindowsServer2008R2:
				{
					if( (*this)()->_processor_architecture == XEnum_ProcessorArchitecture_X64_AMD )
						os_version = "Windows Server 2008 R2 , x64";
					else if( (*this)()->_processor_architecture == XEnum_ProcessorArchitecture_X64_IA )
						os_version = "Windows Server 2008 R2 , x64 Itanium";
					else if( (*this)()->_processor_architecture == XEnum_ProcessorArchitecture_X86 )
						os_version = "Windows Server 2008 R2 , x86";
					break;
				}
			case 	XEnum_OSVersion_Windows7:
				{
					if( (*this)()->_processor_architecture == XEnum_ProcessorArchitecture_X64_AMD )
						os_version = "Windows 7 , x64";
					else if( (*this)()->_processor_architecture == XEnum_ProcessorArchitecture_X64_IA )
						os_version = "Windows 7 , x64 Itanium";
					else if( (*this)()->_processor_architecture == XEnum_ProcessorArchitecture_X86 )
						os_version = "Windows 7 , x86";
					break;
				}
			case 	XEnum_OSVersion_WindowsVista:
				{
					if( (*this)()->_processor_architecture == XEnum_ProcessorArchitecture_X64_AMD )
						os_version = "Windows 7 , x64";
					else if( (*this)()->_processor_architecture == XEnum_ProcessorArchitecture_X64_IA )
						os_version = "Windows 7 , x64 Itanium";
					else if( (*this)()->_processor_architecture == XEnum_ProcessorArchitecture_X86 )
						os_version = "Windows Vista , x86";
					break;
				}
			case 	XEnum_OSVersion_WindowsServer2003:
				{
					if( (*this)()->_processor_architecture == XEnum_ProcessorArchitecture_X64_AMD )
						os_version = "Windows Server 2003 , x64";
					else if( (*this)()->_processor_architecture == XEnum_ProcessorArchitecture_X64_IA )
						os_version = "Windows Server 2003 , x64 Itanium";
					else if( (*this)()->_processor_architecture == XEnum_ProcessorArchitecture_X86 )
						os_version = "Windows Server 2003 , x86";
					break;
				}
			case 	XEnum_OSVersion_Windows2000:
				{
					if( (*this)()->_processor_architecture == XEnum_ProcessorArchitecture_X64_AMD )
						os_version = "Windows 2000 , x64";
					else if( (*this)()->_processor_architecture == XEnum_ProcessorArchitecture_X64_IA )
						os_version = "Windows 2000 , x64 Itanium";
					else if( (*this)()->_processor_architecture == XEnum_ProcessorArchitecture_X86 )
						os_version = "Windows 2000 , x86";
					break;
				}
			}
			return os_version++;
		};
		#ifdef __WINDOWS__
		typedef void (*__stdcall GetNativeSystemInfo_t)( LPSYSTEM_INFO lpSystemInfo );
		typedef void (*__stdcall GetSystemInfo_t)( LPSYSTEM_INFO lpSystemInfo );
		typedef BOOL (*__stdcall GetVersionExA_t)( LPOSVERSIONINFOA lpVersionInformation );
		#endif
		OSVersion& XOSVersion::get_OSVersion( rux::XString& error )
		{
			XOSVersion os_version;
		#ifdef __WINDOWS__
			SYSTEM_INFO system_info;	
			memset( &system_info , 0 , sizeof( SYSTEM_INFO ) );	
			error.Clear();
			XModule module = rux::loader::LoadModule( rux::XString( "kernel32" ) , NULL , error );
			if( module.IsLoaded() == 1 )
			{
				GetNativeSystemInfo_t get_native_system_info = (GetNativeSystemInfo_t)module.get_MethodAddress( "GetNativeSystemInfo" , error );
				if( error.Length() > 0 )
				{
					GetSystemInfo_t get_system_info = (GetSystemInfo_t)module.get_MethodAddress( "GetSystemInfo" , error );
					if( error.Length() == 0 )
						get_system_info( &system_info );
				}
				else
					get_native_system_info( &system_info );
				if( error.Length() == 0 )
				{
					OSVERSIONINFOEX os_version_info_ex;
					memset( &os_version_info_ex , 0 , sizeof( OSVERSIONINFOEX ) );
					os_version_info_ex.dwOSVersionInfoSize = sizeof( OSVERSIONINFOEX );		
					GetVersionExA_t get_version_exA = (GetVersionExA_t)module.get_MethodAddress( "GetVersionExA" , error );
					if( error.Length() == 0 && get_version_exA( (OSVERSIONINFO*)&os_version_info_ex ) )
					{
						if( VER_PLATFORM_WIN32_NT == os_version_info_ex.dwPlatformId && os_version_info_ex.dwMajorVersion > 4 )
						{
							if( os_version_info_ex.dwMajorVersion == 6 && os_version_info_ex.dwMinorVersion == 1 )
							{
								if( os_version_info_ex.wProductType == VER_NT_WORKSTATION )
									os_version()->_os_version = XEnum_OSVersion_Windows7;
								else
									os_version()->_os_version = XEnum_OSVersion_WindowsServer2008R2;
								if( system_info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 )
									os_version()->_processor_architecture = XEnum_ProcessorArchitecture_X64_AMD;
								else if( system_info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64 )
									os_version()->_processor_architecture = XEnum_ProcessorArchitecture_X64_IA;
								else if(system_info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL )
									os_version()->_processor_architecture = XEnum_ProcessorArchitecture_X86;
							}
							if( os_version_info_ex.dwMajorVersion == 6 && os_version_info_ex.dwMinorVersion == 0 )
							{
								if( os_version_info_ex.wProductType == VER_NT_WORKSTATION )
									os_version()->_os_version = XEnum_OSVersion_WindowsVista;
								else
									os_version()->_os_version = XEnum_OSVersion_WindowsServer2008;
								if( system_info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 ) 
									os_version()->_processor_architecture = XEnum_ProcessorArchitecture_X64_AMD;
								else if( system_info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64 ) 
									os_version()->_processor_architecture = XEnum_ProcessorArchitecture_X64_IA;
								else if( system_info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL ) 
									os_version()->_processor_architecture = XEnum_ProcessorArchitecture_X86;
							}
							if( os_version_info_ex.dwMajorVersion == 5 && os_version_info_ex.dwMinorVersion == 2 )
							{
								if( os_version_info_ex.wProductType == VER_NT_WORKSTATION && system_info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
								{
									os_version()->_os_version = XEnum_OSVersion_WindowsXP;							
									os_version()->_processor_architecture = XEnum_ProcessorArchitecture_X64_AMD;
								}
								else 
								{
									os_version()->_os_version = XEnum_OSVersion_WindowsServer2003;
									if( system_info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL ) 
										os_version()->_processor_architecture = XEnum_ProcessorArchitecture_X86;
									else if( system_info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64 ) 
										os_version()->_processor_architecture = XEnum_ProcessorArchitecture_X64_IA;
									else if( system_info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 ) 
										os_version()->_processor_architecture = XEnum_ProcessorArchitecture_X64_AMD;
								}
							}
							if( os_version_info_ex.dwMajorVersion == 5 && os_version_info_ex.dwMinorVersion == 1 )
							{
								os_version()->_os_version = XEnum_OSVersion_WindowsXP;
								os_version()->_processor_architecture = XEnum_ProcessorArchitecture_X86;
							}
							if( os_version_info_ex.dwMajorVersion == 5 && os_version_info_ex.dwMinorVersion == 0 )
							{
								os_version()->_os_version = XEnum_OSVersion_Windows2000;
								os_version()->_processor_architecture = XEnum_ProcessorArchitecture_X86;
							}
						}
					}
				}
			}
		#endif
			return os_version++;
		};
		uint8 XOSVersion::get_IsWindows( void )
		{
			return (*this)()->_os_version == XEnum_OSVersion_WindowsXP 
				|| (*this)()->_os_version == XEnum_OSVersion_WindowsServer2008
				|| (*this)()->_os_version == XEnum_OSVersion_WindowsServer2008R2
				|| (*this)()->_os_version == XEnum_OSVersion_Windows7
				|| (*this)()->_os_version == XEnum_OSVersion_WindowsVista
				|| (*this)()->_os_version == XEnum_OSVersion_WindowsServer2003
				|| (*this)()->_os_version == XEnum_OSVersion_Windows2000 ? 1 : 0;
		};
		XEnum_ProcessorArchitecture XOSVersion::get_ProcessorArchitecture( void )
		{
			return (*this)()->_processor_architecture ;
		};
	};
};