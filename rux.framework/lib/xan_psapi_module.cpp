#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_psapi_module.h"
namespace rux
{
	namespace system
	{
#ifdef __WINDOWS__
		psapi_module::psapi_module( void )
			: rux::system::module( "psapi.dll" )
		{
			_EnumProcesses = 0;
			_EnumProcessModules = 0;
			_GetModuleBaseNameA = 0;
			_GetModuleFileNameExA = 0;
			_GetProcessImageFileNameA = 0;
		};
		void psapi_module::on_loaded( void )
		{
			system_module_dlsym( EnumProcesses );
			system_module_dlsym( EnumProcessModules );
			system_module_dlsym( GetModuleBaseNameA );
			system_module_dlsym( GetModuleFileNameExA );
			system_module_dlsym( GetProcessImageFileNameA );
		};
		BOOL psapi_module::EnumProcessesF( DWORD* pProcessIds , DWORD cb , DWORD* pBytesReturned )
		{
			load();
			return _EnumProcesses( pProcessIds , cb , pBytesReturned );
		};
		BOOL psapi_module::EnumProcessModulesF( HANDLE hProcess , HMODULE* lphModule , DWORD cb , LPDWORD lpcbNeeded )
		{
			load();
			return _EnumProcessModules( hProcess , lphModule , cb , lpcbNeeded );
		};
		DWORD psapi_module::GetModuleBaseNameAF( HANDLE hProcess , HMODULE hModule , char* lpBaseName , DWORD nSize )
		{
			load();
			return _GetModuleBaseNameA( hProcess , hModule , lpBaseName , nSize );
		};
		DWORD psapi_module::GetModuleFileNameExAF( HANDLE hProcess , HMODULE hModule , char* lpFilename , DWORD nSize )
		{
			load();
			return _GetModuleFileNameExA( hProcess , hModule , lpFilename , nSize );
		};
		DWORD psapi_module::GetProcessImageFileNameAF( HANDLE hProcess , char* lpImageFileName , DWORD nSize )
		{
			load();
			return _GetProcessImageFileNameA( hProcess , lpImageFileName , nSize );
		};
#endif
	};
};