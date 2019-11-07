#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_dbghelp_module.h"
namespace rux
{
	namespace system
	{
#ifdef __WINDOWS__
		dbghelp_module::dbghelp_module( void )
			: rux::system::module( "dbghelp.dll" , 0 , 1 )
		{
			_SymSetOptions = 0;
			_SymInitialize = 0;
			_SymFromAddr = 0;
			_SymGetSymFromAddr64 = 0;
			_SymCleanup = 0;
			_SymGetLineFromAddr64 = 0;
			_SymLoadModule64 = 0;
			_SymFunctionTableAccess64 = 0;
			_SymGetModuleBase64 = 0;
			_StackWalk64 = 0;
			_MiniDumpWriteDump = 0;
			_SymGetSearchPath = 0;
			_SymSetSearchPath = 0;
			_SymRefreshModuleList = 0;
			_SymLoadModuleEx = 0;
			_SymGetModuleInfo64 = 0;
		};
		DWORD WINAPI dbghelp_module::SymSetOptions( DWORD SymOptions )
		{
			load();
			return _SymSetOptions( SymOptions );
		};
		BOOL WINAPI dbghelp_module::SymInitialize( HANDLE hProcess , PCTSTR UserSearchPath , BOOL fInvadeProcess )
		{
			load();
			return _SymInitialize( hProcess , UserSearchPath , fInvadeProcess );
		};
		BOOL WINAPI dbghelp_module::SymFromAddr( HANDLE hProcess , DWORD64 Address , PDWORD64 Displacement , PSYMBOL_INFO Symbol )
		{
			load();
			return _SymFromAddr( hProcess , Address , Displacement , Symbol );
		};
		BOOL WINAPI dbghelp_module::SymGetSymFromAddr64( HANDLE hProcess , DWORD64 Address , PDWORD64 Displacement , PIMAGEHLP_SYMBOL64 Symbol )
		{
			load();
			return _SymGetSymFromAddr64( hProcess , Address , Displacement , Symbol );
		};
		BOOL WINAPI dbghelp_module::SymCleanup( HANDLE hProcess )
		{
			load();
			return _SymCleanup( hProcess );
		};
		BOOL WINAPI dbghelp_module::SymGetLineFromAddr64( HANDLE hProcess , DWORD64 dwAddr , PDWORD pdwDisplacement , PIMAGEHLP_LINEA64 Line )
		{
			load();
			return _SymGetLineFromAddr64( hProcess , dwAddr , pdwDisplacement , Line );
		};
		DWORD64 WINAPI dbghelp_module::SymLoadModule64( HANDLE hProcess , HANDLE hFile , PCSTR ImageName , PCSTR ModuleName , DWORD64 BaseOfDll , DWORD SizeOfDll )
		{
			load();
			return _SymLoadModule64( hProcess , hFile , ImageName , ModuleName , BaseOfDll , SizeOfDll );
		};
		PVOID WINAPI dbghelp_module::SymFunctionTableAccess64( HANDLE hProcess , DWORD64 AddrBase )
		{
			load();
			return _SymFunctionTableAccess64( hProcess , AddrBase );
		};
		DWORD64 WINAPI dbghelp_module::SymGetModuleBase64( HANDLE hProcess , DWORD64 dwAddr )
		{
			load();
			return _SymGetModuleBase64( hProcess , dwAddr );
		};
		BOOL WINAPI dbghelp_module::StackWalk64( DWORD MachineType , HANDLE hProcess , HANDLE hThread , LPSTACKFRAME64 StackFrame , PVOID ContextRecord , PREAD_PROCESS_MEMORY_ROUTINE64 ReadMemoryRoutine , PFUNCTION_TABLE_ACCESS_ROUTINE64 FunctionTableAccessRoutine , PGET_MODULE_BASE_ROUTINE64 GetModuleBaseRoutine , PTRANSLATE_ADDRESS_ROUTINE64 TranslateAddress )
		{
			load();
			return _StackWalk64( MachineType , hProcess , hThread , StackFrame , ContextRecord , ReadMemoryRoutine , FunctionTableAccessRoutine , GetModuleBaseRoutine , TranslateAddress );
		};
		BOOL WINAPI dbghelp_module::MiniDumpWriteDump( HANDLE hProcess , DWORD ProcessId , HANDLE hFile , MINIDUMP_TYPE DumpType , PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam , PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam , PMINIDUMP_CALLBACK_INFORMATION CallbackParam )
		{
			load();
			return _MiniDumpWriteDump( hProcess , ProcessId , hFile , DumpType , ExceptionParam , UserStreamParam , CallbackParam );
		};
		BOOL WINAPI dbghelp_module::SymGetSearchPath( HANDLE hProcess , PTSTR SearchPath , DWORD SearchPathLength )
		{
			load();
			return _SymGetSearchPath( hProcess , SearchPath , SearchPathLength );
		};
		BOOL WINAPI dbghelp_module::SymSetSearchPath( HANDLE hProcess , PCTSTR SearchPath )
		{
			load();
			return _SymSetSearchPath( hProcess , SearchPath );
		};
		BOOL WINAPI dbghelp_module::SymRefreshModuleList( HANDLE hProcess )
		{
			load();
			return _SymRefreshModuleList( hProcess );
		};
		DWORD64 WINAPI dbghelp_module::SymLoadModuleEx( HANDLE hProcess , HANDLE hFile , PCTSTR ImageName , PCTSTR ModuleName , DWORD64 BaseOfDll , DWORD DllSize , PMODLOAD_DATA Data , DWORD Flags )
		{
			load();
			return _SymLoadModuleEx( hProcess , hFile , ImageName , ModuleName , BaseOfDll , DllSize , Data , Flags );
		};
		BOOL WINAPI dbghelp_module::SymGetModuleInfo64( HANDLE hProcess , DWORD64 dwAddr , PIMAGEHLP_MODULE64 ModuleInfo )
		{
			load();
			return _SymGetModuleInfo64( hProcess , dwAddr , ModuleInfo );
		};
		void dbghelp_module::on_loaded( void )
		{
			system_module_dlsym( SymSetOptions );
			system_module_dlsym( SymInitialize );
			system_module_dlsym( SymFromAddr );
			system_module_dlsym( SymGetSymFromAddr64 );
			system_module_dlsym( SymCleanup );
			system_module_dlsym( SymGetLineFromAddr64 );
			system_module_dlsym( SymLoadModule64 );
			system_module_dlsym( SymFunctionTableAccess64 );
			system_module_dlsym( SymGetModuleBase64 );
			system_module_dlsym( StackWalk64 );
			system_module_dlsym( MiniDumpWriteDump );
			system_module_dlsym( SymGetSearchPath );
			system_module_dlsym( SymSetSearchPath );
			system_module_dlsym( SymRefreshModuleList );
			system_module_dlsym( SymLoadModuleEx );
			system_module_dlsym( SymGetModuleInfo64 );
		};
#endif
	};
};