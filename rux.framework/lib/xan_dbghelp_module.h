#ifndef XAN_DBGHELP_MODULE_H
#define XAN_DBGHELP_MODULE_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_system_module.h"
#ifdef __WINDOWS__
#include <DbgHelp.h>
typedef struct IMAGEHLP_LINEA64
{
	::rux::uint32 SizeOfStruct;
	void* Key;
	::rux::uint32 LineNumber; 
	char* FileName; 
	::rux::uint64 Address;
}*PIMAGEHLP_LINEA64;
#endif
namespace rux
{
	namespace system
	{
#ifdef __WINDOWS__
		typedef DWORD (WINAPI *SymSetOptions_t)( DWORD SymOptions );
		typedef BOOL (WINAPI *SymInitialize_t)( HANDLE hProcess , PCTSTR UserSearchPath , BOOL fInvadeProcess );
		typedef BOOL (WINAPI *SymFromAddr_t)( HANDLE hProcess , DWORD64 Address , PDWORD64 Displacement , PSYMBOL_INFO Symbol );
		typedef BOOL (WINAPI *SymGetSymFromAddr64_t)( HANDLE hProcess , DWORD64 Address , PDWORD64 Displacement , PIMAGEHLP_SYMBOL64 Symbol );
		typedef BOOL (WINAPI *SymCleanup_t)( HANDLE hProcess );
		typedef BOOL (WINAPI *SymGetLineFromAddr64_t)( HANDLE hProcess , DWORD64 dwAddr , PDWORD pdwDisplacement , PIMAGEHLP_LINEA64 Line );
		typedef DWORD64 (WINAPI *SymLoadModule64_t)( HANDLE hProcess , HANDLE hFile , PCSTR ImageName , PCSTR ModuleName , DWORD64 BaseOfDll , DWORD SizeOfDll );
		typedef PVOID (WINAPI *SymFunctionTableAccess64_t)( HANDLE hProcess , DWORD64 AddrBase );
		typedef DWORD64 (WINAPI *SymGetModuleBase64_t)( HANDLE hProcess , DWORD64 dwAddr );
		typedef BOOL (WINAPI *StackWalk64_t)( DWORD MachineType , HANDLE hProcess , HANDLE hThread , LPSTACKFRAME64 StackFrame , PVOID ContextRecord , PREAD_PROCESS_MEMORY_ROUTINE64 ReadMemoryRoutine , PFUNCTION_TABLE_ACCESS_ROUTINE64 FunctionTableAccessRoutine , PGET_MODULE_BASE_ROUTINE64 GetModuleBaseRoutine , PTRANSLATE_ADDRESS_ROUTINE64 TranslateAddress );
		typedef BOOL (WINAPI *MiniDumpWriteDump_t)( HANDLE hProcess , DWORD ProcessId , HANDLE hFile , MINIDUMP_TYPE DumpType , PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam , PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam , PMINIDUMP_CALLBACK_INFORMATION CallbackParam );
		typedef BOOL (WINAPI *SymGetSearchPath_t)( HANDLE hProcess , PTSTR SearchPath , DWORD SearchPathLength );
		typedef BOOL (WINAPI *SymSetSearchPath_t)( HANDLE hProcess , PCTSTR SearchPath );
		typedef BOOL (WINAPI *SymRefreshModuleList_t)( HANDLE hProcess );
		typedef DWORD64 (WINAPI *SymLoadModuleEx_t)( HANDLE hProcess , HANDLE hFile , PCTSTR ImageName , PCTSTR ModuleName , DWORD64 BaseOfDll , DWORD DllSize , PMODLOAD_DATA Data , DWORD Flags );
		typedef BOOL (WINAPI *SymGetModuleInfo64_t)( HANDLE hProcess , DWORD64 dwAddr , PIMAGEHLP_MODULE64 ModuleInfo );
		class dbghelp_module : public rux::system::module
		{
		public:
			SymSetOptions_t _SymSetOptions;
			SymInitialize_t _SymInitialize;
			SymFromAddr_t _SymFromAddr;
			SymGetSymFromAddr64_t _SymGetSymFromAddr64;
			SymCleanup_t _SymCleanup;
			SymGetLineFromAddr64_t _SymGetLineFromAddr64;
			SymLoadModule64_t _SymLoadModule64;
			SymFunctionTableAccess64_t _SymFunctionTableAccess64;
			SymGetModuleBase64_t _SymGetModuleBase64;
			StackWalk64_t _StackWalk64;
			SymGetSearchPath_t _SymGetSearchPath;
			SymSetSearchPath_t _SymSetSearchPath;
			MiniDumpWriteDump_t _MiniDumpWriteDump;
			SymRefreshModuleList_t _SymRefreshModuleList;
			SymLoadModuleEx_t _SymLoadModuleEx;
			SymGetModuleInfo64_t _SymGetModuleInfo64;
		public:
			dbghelp_module( void );
			DWORD WINAPI SymSetOptions( DWORD SymOptions );
			BOOL WINAPI SymInitialize( HANDLE hProcess , PCTSTR UserSearchPath , BOOL fInvadeProcess );
			BOOL WINAPI SymFromAddr( HANDLE hProcess , DWORD64 Address , PDWORD64 Displacement , PSYMBOL_INFO Symbol );
			BOOL WINAPI SymGetSymFromAddr64( HANDLE hProcess , DWORD64 Address , PDWORD64 Displacement , PIMAGEHLP_SYMBOL64 Symbol );
			BOOL WINAPI SymCleanup( HANDLE hProcess );
			BOOL WINAPI SymGetLineFromAddr64( HANDLE hProcess , DWORD64 dwAddr , PDWORD pdwDisplacement , PIMAGEHLP_LINEA64 Line );
			DWORD64 WINAPI SymLoadModule64( HANDLE hProcess , HANDLE hFile , PCSTR ImageName , PCSTR ModuleName , DWORD64 BaseOfDll , DWORD SizeOfDll );
			PVOID WINAPI SymFunctionTableAccess64( HANDLE hProcess , DWORD64 AddrBase );
			DWORD64 WINAPI SymGetModuleBase64( HANDLE hProcess , DWORD64 dwAddr );
			BOOL WINAPI StackWalk64( DWORD MachineType , HANDLE hProcess , HANDLE hThread , LPSTACKFRAME64 StackFrame , PVOID ContextRecord , PREAD_PROCESS_MEMORY_ROUTINE64 ReadMemoryRoutine , PFUNCTION_TABLE_ACCESS_ROUTINE64 FunctionTableAccessRoutine , PGET_MODULE_BASE_ROUTINE64 GetModuleBaseRoutine , PTRANSLATE_ADDRESS_ROUTINE64 TranslateAddress );
			BOOL WINAPI MiniDumpWriteDump( HANDLE hProcess , DWORD ProcessId , HANDLE hFile , MINIDUMP_TYPE DumpType , PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam , PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam , PMINIDUMP_CALLBACK_INFORMATION CallbackParam );
			BOOL WINAPI SymGetSearchPath( HANDLE hProcess , PTSTR SearchPath , DWORD SearchPathLength );
			BOOL WINAPI SymSetSearchPath( HANDLE hProcess , PCTSTR SearchPath );
			BOOL WINAPI SymRefreshModuleList( HANDLE hProcess );
			DWORD64 WINAPI SymLoadModuleEx( HANDLE hProcess , HANDLE hFile , PCTSTR ImageName , PCTSTR ModuleName , DWORD64 BaseOfDll , DWORD DllSize , PMODLOAD_DATA Data , DWORD Flags );
			BOOL WINAPI SymGetModuleInfo64( HANDLE hProcess , DWORD64 dwAddr , PIMAGEHLP_MODULE64 ModuleInfo );
			virtual void on_loaded( void );
		};
#endif
	};
};
#endif
