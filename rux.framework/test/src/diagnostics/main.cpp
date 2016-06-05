#include <xan_console.h>
#include <xan_process.h>
#include <xan_debug.h>
const char* g_current_module_name = "rux.diagnostics.test.exe";
dll_internal ::rux::uint8 _rux_is_memory_manager = 0;
namespace rux
{
	dll_internal rux_simple_t _first_function = NULL;
	dll_internal rux_simple_t _last_function = NULL;
	dll_internal rux_simple_t _pre_initialization = NULL;
	dll_internal rux_simple_t _post_deinitialization = NULL;
};
void process( char* line_ptr , rux::XConsole* console_ptr )
{	
	rux::XString error;
	rux::XString line( line_ptr );
	line.ToLower();
	if( line.ToLower() == "quit"
		|| line.ToLower() == "exit"
		|| line.ToLower() == "q" )
	{
		console_ptr->Exit();
	}
	else if( line.ToLower() == "except" )
	{
		int i = 0;
		int except = 10986 / i;
	}
	else if( line.ToLower() == "start"
	|| line.ToLower() == "start\n" )
	{
#ifdef __WINDOWS__
		rux::XModule module = rux::XModule::get_Executable( error );
		rux::XArray< rux::XString > arguments;
		rux::diagnostics::XProcess process;
		process.set_ModuleName( module.get_FileName() );
		process.StartWithoutWait( XEnumProcessStartMethod_ShellExecute , arguments , error );
#else
		declare_stack_variable( char , g_terminal , 1024 );
		rux::XString terminal , terminal_path;
		terminal.set_ByRef( XEnvironment::get_Variable( "COLORTERM" ) );
		if( terminal.Length() == 0 )
			terminal.set_ByRef( XEnvironment::get_Variable( "TERM" ) );
		rux::XString path;
		path.set_ByRef( XEnvironment::get_Variable( "PATH" ) );
		rux::XArray< rux::XString > paths;
		paths.set_ByRef( path.Split( ':' ) );
		for( uint32 index0 = 0 ; index0 < paths.Count() ; index0++ )
		{
			terminal_path.set_ByRef( paths[ index0 ] + "/" + terminal );
			if( rux_is_exists_file_or_link( terminal_path()->_array_ptr ) == 1 )
			{
				printf( terminal_path()->_array_ptr );
				break;				
			}
			terminal_path.Clear();
		}
		if( terminal_path.Length() > 0 && terminal_path.Size() <= 1024 )
			memcpy( g_terminal , terminal_path()->_array_ptr , terminal_path.Size() );
		else
			memcpy( g_terminal , "/usr/bin/gnome-terminal" , strlen( "/usr/bin/gnome-terminal" ) + 1 );
		
		
		declare_stack_variable( char , executable_file , 1024 );
		rux_get_executable_filename( executable_file );		
		if( g_terminal[ 0 ] != '\0' )
		{			
			const char* arguments[] =		
			{
				"-e" ,
				executable_file
			};
			rux::diagnostics::XProcess::StartAndKillParent( g_terminal , arguments , 2 );
		}
		else
			rux::diagnostics::XProcess::StartAndKillParent( executable_file , NULL , 0 );		
#endif
	}
	else
	{		
#ifdef __WINDOWS__
		rux::diagnostics::XProcess process;
		process.set_ModuleName( "Video" );
		rux::int64 value = process.get_WorkingSet( error );
		printf( "Working Set = "I64d" Kb\n" , value / 1024L );
		value = process.get_VirtualBytes( error );
		printf( "Virtual Bytes = "I64d" Kb\n" , value / 1024L );
		value = process.get_PrivateBytes( error );
		printf( "Private Bytes = "I64d" Kb\n" , value / 1024L );		
#endif		
	}
}
void exception_event( char* callstack_ptr )
{
#ifdef __WINDOWS__
	MessageBox( NULL , callstack_ptr , NULL , MB_OK );
	rux::XString error;
	rux::XModule module = rux::XModule::get_Executable( error );
	rux::XArray< rux::XString > arguments;
	rux::diagnostics::XProcess process;
	process.set_ModuleName( module.get_FileName() );
	process.StartWithoutWait( XEnumProcessStartMethod_ShellExecute , arguments , error );
#endif	
};
int main(int c, char ** argc)
{
	rux::XString error;
	
	execl( "/etc/init.d/videosrv7" , "videosrv7" , "stop" , NULL );
	
	if( rux::diagnostics::XProcess::IsExecuted( "videosrv7" , error ) == 1 )
		rux::diagnostics::XProcess::Kill( "videosrv7" , error );


	XDebug::set_IsKillChildIfParentKilled( 0 );
	rux::XConsole console;
	console.Process( process , 0 , NULL , NULL );	
	rux::deinitialize();
	return 0;
}
