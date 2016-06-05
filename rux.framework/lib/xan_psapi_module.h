#ifndef XAN_PSAPI_MODULE_H
#define XAN_PSAPI_MODULE_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_system_module.h>
namespace rux
{
	namespace system
	{
#ifdef __WINDOWS__
		typedef BOOL (WINAPI *EnumProcesses_t)( DWORD* pProcessIds , DWORD cb , DWORD* pBytesReturned );
		typedef BOOL (WINAPI *EnumProcessModules_t)( HANDLE hProcess , HMODULE* lphModule , DWORD cb , LPDWORD lpcbNeeded );
		typedef DWORD (WINAPI *GetModuleBaseNameA_t)( HANDLE hProcess , HMODULE hModule , char* lpBaseName , DWORD nSize );
		typedef DWORD (WINAPI *GetModuleFileNameExA_t)( HANDLE hProcess , HMODULE hModule , char* lpFilename , DWORD nSize );
		typedef DWORD (WINAPI *GetProcessImageFileNameA_t)( HANDLE hProcess , char* lpImageFileName , DWORD nSize );
		class psapi_module : public rux::system::module
		{
		protected:
			EnumProcesses_t _EnumProcesses;
			EnumProcessModules_t _EnumProcessModules;
			GetModuleBaseNameA_t _GetModuleBaseNameA;
			GetModuleFileNameExA_t _GetModuleFileNameExA;
			GetProcessImageFileNameA_t _GetProcessImageFileNameA;
		public:
			psapi_module( void );
			virtual void on_loaded( void );
			BOOL WINAPI EnumProcesses( DWORD* pProcessIds , DWORD cb , DWORD* pBytesReturned );
			BOOL WINAPI EnumProcessModules( HANDLE hProcess , HMODULE* lphModule , DWORD cb , LPDWORD lpcbNeeded );
			DWORD WINAPI GetModuleBaseNameA( HANDLE hProcess , HMODULE hModule , char* lpBaseName , DWORD nSize );
			DWORD WINAPI GetModuleFileNameExA( HANDLE hProcess , HMODULE hModule , char* lpFilename , DWORD nSize );
			DWORD WINAPI GetProcessImageFileNameA( HANDLE hProcess , char* lpImageFileName , DWORD nSize );
		};
#endif
	};
};
#endif
