#ifndef XAN_PROCESS_H
#define XAN_PROCESS_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_environment.h>
#include <xan_loader_defines.h>
enum XEnumProcessStartMethod
{
	XEnumProcessStartMethod_CreateProcessAsUser , 
	XEnumProcessStartMethod_CreateProcess , 
	XEnumProcessStartMethod_ShellExecute
};
namespace rux
{	
	namespace diagnostics
	{	
		begin_declare_rux_class( Process );
		begin_declare_rux_class_members( Process );
			rux::XString _module_name;
		end_declare_rux_class_members( Process );
			uint8 Start( rux::XString& error );
			uint8 Start( const rux::XString& arguments , rux::XString& error );
			uint8 StartDetached( const rux::XString& arguments , uint8 show , rux::XString& error );
			uint8 StartWithoutWait( XEnumProcessStartMethod process_start_method , const XArray< rux::XString >& arguments , rux::XString& error , char* user_name = 0 , char** env = 0 , size_t env_size = 0 );
			static void Stop( void );
			void set_ModuleName( const rux::XString& module_name );
			rux::int64 get_WorkingSet( rux::XString& error );
			rux::int64 get_VirtualBytes( rux::XString& error );
			rux::int64 get_PrivateBytes( rux::XString& error );	
			static uint8 IsAdminUserProcess( void );
			static void StartAndKillParent( char* executable_path , const char** arguments , uint32 arguments_count );
			static uint8 IsExecuted( const rux::XString& process_name , rux::XString& error );
			static void Kill( const rux::XString& process_name , rux::XString& error );
			static void KillZombies( const rux::XString& process_name );
		#ifdef __WINDOWS__
			static DWORD get_ParentProcessId( DWORD pid );
			static DWORD get_CurrentProcessId( void );
			static void Kill( const rux::XString& process_name , DWORD pid , rux::XString& error );
			static uint8 CtrlC( const rux::XString& process_name , DWORD pid , rux::XString& error );
		#elif defined( __UNIX__ )
			static pid_t get_ParentProcessId( pid_t pid );
			static pid_t get_CurrentProcessId( void );
			static void Kill( const rux::XString& process_name , pid_t pid , rux::XString& error );
			static uint8 CtrlC( const rux::XString& process_name , pid_t pid , rux::XString& error );
		#endif
		private:
		#ifdef __WINDOWS__
			uint32 private_WaitForProcess( HANDLE handle );
		#endif	
		end_declare_rux_class();
	};
};
#endif
