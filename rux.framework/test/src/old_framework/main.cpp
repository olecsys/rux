#include <xan_console.h>
#include <xan_rawdata.h>
#include <xan_asm.h>
#include <xan_math.h>
#include <xan_int64.h>
#include <xan_time.h>
#include <xan_debug.h>
#include <xan_registry.h>
#include <xan_pipe.h>
#include <xan_security.h>
#include <xan_boolean.h>
#include <xan_regexp.h>
#include <xan_stdredirect.h>
#include <xan_int32.h>
#include <xan_gui_defines.h>
#include <xan_module.h>
#include <xan_io_defines.h>
#include <xan_png.h>
#include <xan_loader_defines.h>
#include <xan_com_object.h>
#include <xan_pdh.h>
#include <xan_native_diagnostics.h>
#include <xan_constructor_info.h>
#include <xan_uint64.h>
#include <xan_memory_helper.h>
#include <xan_error.h>
#include <xan_log.h>
#include <xan_dataobject.h>
#include <xan_jsonserializer.h>
#include <xan_process.h>
#include <xan_code_performance_counter.h>
#include <xan_memory_chunk.h>
#include <xan_wtsapi32_module.h>
#include <xan_Iphlpapi_module.h>
#include <xan_ole32_module.h>
#include <xan_oleaut32_module.h>
#include <xan_psapi_module.h>
#include <xan_user32_module.h>
#include <xan_userenv32_module.h>
#include <xan_winmm_module.h>
#include <xan_ws2_32_module.h>
#include <xan_x11_module.h>
#include <xan_window.h>
#include <xan_uibutton.h>
#include <xan_uitextblock.h>
#include <xan_uiselect.h>
#include <xan_uiimage.h>
#include <xan_hard_disk.h>
#include <xan_network_information.h>
#include <xan_gui_checkbox.h>
#include <xan_gui_tabcontrol.h>
#include <xan_udpclient.h>
#include <xan_uidatepicker.h>
#include <xan_numeric.h>
#include <xan_std_allocator.h>
#include <xan_double_linked_list.h>
#define not_empty_code 1
const char* g_current_module_name = "rux.basis.test.exe";
dll_internal ::rux::uint8 _rux_is_memory_manager = 0;
namespace rux
{
	dll_internal rux_simple_t _first_function = NULL;
	dll_internal rux_simple_t _last_function = NULL;
	dll_internal rux_simple_t _pre_initialization = NULL;
	dll_internal rux_simple_with_result_t _post_deinitialization = NULL;
};
#if not_empty_code
#ifdef __WINDOWS__
#include <crtdbg.h>
void myInvalidParameterHandler(const wchar_t* expression,
   const wchar_t* function, 
   const wchar_t* file, 
   unsigned int line, 
   uintptr_t pReserved)
{
	printf( "%s\n" , __FUNCTION__ );
   wprintf(L"Invalid parameter detected in function %s."
			L" File: %s Line: %d\n", function, file, line);
   wprintf(L"Expression: %s\n", expression);
}
void my_unexpected_handler()
{
	printf( "%s\n" , __FUNCTION__ );
  // Abnormal program termination (terminate() function was called)
  // Do something here
  // Finally, terminate program
  exit(1); 
}
void my_terminate_handler()
{
	printf( "%s\n" , __FUNCTION__ );
  // Abnormal program termination (terminate() function was called)
  // Do something here
  // Finally, terminate program
  exit(1); 
}
void sigabrt_handler(int)
{
	printf( "%s\n" , __FUNCTION__ );
  // Caught SIGABRT C++ signal

  // Terminate program
  exit(1);
}


declare_rux_com_object_clsid( 23 , 23 , 23 , 23 , 23 , 23 , 23 , 23 , 23 , 23 , 23 , 23 , 23 , 23 , 23 , 23 , ComTest );
const char* ComTestInterfaceGuid = declare_rux_com_object_guid_string( F6 , 14 , FB , 00 , 67 , 02 , 11 , d4 , B0 , B7 , 00 , 50 , BA , BF , C9 , 04 );
begin_declare_rux_com_interface( "F614FB00-6702-11d4-B0B7-0050BABFC904" , ComTestInterface )
end_declare_rux_com_interface();

begin_declare_rux_com_object_class( ComTestClass , IComTestInterface );
end_declare_rux_com_object_class( ComTestClass );

begin_implement_rux_com_object_class( ComTestClass )
end_implement_rux_com_object_class();
#endif
::rux::uint32 test_index0 = 0;
void test_thread0( void* param , size_t ___rux__thread_index1986 )
{	
	::rux::gui::XWindow* window = (::rux::gui::XWindow*)param;		
	test_index0++;
	char line[ 256 ] = {0};
	sprintf( line , "try #%u" , test_index0 );
	::rux::byte found = 0;
	::rux::gui::controls::XTextBlock test_textblock0;
	test_textblock0.set_ByRef( window->FindOne< ::rux::gui::controls::XTextBlock >( "test_textblock0" , found ) );
	if( found )
		test_textblock0.set_Text( line );
};
using namespace rux;
begin_declare_rux_class( BASIS_TEST );
begin_declare_rux_class_members( BASIS_TEST );
end_declare_rux_class_members( BASIS_TEST );
end_declare_rux_class();
begin_implement_rux_class( BASIS_TEST );
end_implement_rux_class();
const char* string0 = "hello 0";
const char* string1 = "hello 1";
const char* string2 = "hello 2";
const char* string3 = "hello 3";
const char* string4 = "hello 4";
const char* string5 = "hello 5";
const char* string6 = "hello 6";
const char* string7 = "hello 7";
rux::int32 _stack_level = 0;
void on_pipe_received( XData& recv , XData& send , const char* , const char* )
{
	size_t recv_size = recv._data_size;
	if( recv_size > 0 )
	{
		char* recv_string = (char*)recv.Read( recv_size );
		printf( "%s\n" , recv_string );
		//printf( "%s\n" , "yes" );
	}
	send.Copy( (void*)"I`am alive" , 11 );		
};
int register_in_stability( const char* service_name )
{
	int res = -1;
	declare_stack_variable( char , executable_filename , 4096 );
	rux_get_executable_filename( executable_filename );
	::rux::string::replace_all( executable_filename , '\\' , '/' );
	::rux::data::XDataObject object;
	object.SetValue( "command" , "register" , 1 , 8 , 0 );
	object.SetValue( "type" , "service" , 1 , 7 , 0 );
	object.SetValue( "service" , service_name , 1 , SIZE_MAX , 0 );
	object.SetValue( "filename" , executable_filename , 1 , SIZE_MAX , 0 );
	object.SetValue( "pid" , (::rux::int64)rux::diagnostics::XProcess::get_CurrentProcessId() , 0);
	object.SetValue( "timeout" , 15LL , 0 );
	object.SetValue( "unique" , "total workability" , 1 , 17 , 0 );
	object.SetValue( "mon" , false , 0 );
	object.SetValue( "tue" , false , 0 );
	object.SetValue( "wed" , false , 0 );
	object.SetValue( "thu" , false , 0 );
	object.SetValue( "fri" , false , 0 );
	object.SetValue( "sat" , false , 0 );
	object.SetValue( "sun" , false , 0 );
	object.SetValue( "time" , "00:00:00" , 1 , 8 , 0 );
	object.SetValue( "usb_restart" , false , 0 );
	object.SetValue( "restart_type" , "Program" , 1 , SIZE_MAX , 0 );
	object.SetValue( "day_of_a_month_restart" , 0LL , 0 );
	object.SetValue( "period_restart" , "Every week" , 1 , SIZE_MAX , 0 );
	::rux::data::XDataObject environment;
	environment.set_ByRef( *::rux::engine::_globals->_environment );
	object.SetValue( "environment" , environment , 0 );
	::rux::XString error , js;
	js.set_ByRef( ::rux::data::XJsonSerializer::Serialize( object , 1 , error ) );
	if( error.Length() == 0 )
	{
		res = -2;
		::rux::XPipeClient stability_service_pipe_client;
		XData recv_data;
		::rux::pid_t pid = 0;
		//if( ::rux::diagnostics::process::executing( "stability" , &pid ) )
		{
			res = -3;
			stability_service_pipe_client.Send( "_rux_stability" , js.str() , js.Size() , recv_data , error );
			if( error.Length() == 0 )
			{
				size_t recv_data_size = recv_data._data_size;
				if( recv_data_size > 0 )
				{
					char* recv_string = (char*)recv_data.Read( recv_data_size );
					printf( "recv_string='%s'\n" , recv_string );
				}
				res = 0;
			}
			else
			{
				printf( "error='%s'\n" , error.str() );
				res = -4;
			}
		}
	}
	return res;
};
rux::int64 __cdecl test_stack( const char* module_ptr , const char* module_ptr2 , const char* module_ptr3 , const char* module_ptr4 , const char* module_ptr5 , const char* module_ptr6 , const char* module_ptr7 )
{
	printf( "test_stack %p %p %p %p %p %p %p\n" , module_ptr , module_ptr2 , module_ptr3 , module_ptr4 , module_ptr5 , module_ptr6 , module_ptr7 );	
	printf( "test_stack %s %s %s %s %s %s %s\n" , module_ptr , module_ptr2 , module_ptr3 , module_ptr4 , module_ptr5 , module_ptr6 , module_ptr7 );	
	if( _stack_level < 25 )
	{
		_stack_level++;
		rux_runtime_param_t params[ 7 ];
		params[ 0 ] = string6;
		params[ 1 ] = string5;
		params[ 2 ] = string4;
		params[ 3 ] = string3;
		params[ 4 ] = string2;
		params[ 5 ] = string1;
		params[ 6 ] = string0;	
		printf( "test_stack level=%d , params=%p\n" , _stack_level , params );
		printf( "before test_stack\n" );
		::rux::int64 result = RuntimeInvoke( (void*)test_stack , XEnum_ReturnValueType_Default , params , 7 );		
		printf( "after test_stack result="I64d",params=%p\n" , result , params );		
	}
	printf( "before return result\n" );		
	return _stack_level;
};
rux::int32 __cdecl test0( void )
{	
	printf( "test0\n" );
	return 0;
};
double __cdecl test1_double( double param0 )
{	
	printf( "test1_double %f\n" , param0 );
	return 1.75;
};
double __cdecl test2_mix( double param0 , ::rux::int32 param1 )
{	
	printf( "test1_mix %f %d\n" , param0 , param1 );	
	return 1.7;
};
double __cdecl test7_mix( double param0 , ::rux::int32 param1 , double param2 , ::rux::int32 param3 , double param4 , ::rux::int32 param5 , double param6 )
{	
	printf( "test7_mix %f %d %f %d %f %d %f\n" , param0 , param1 , param2 , param3 , param4 , param5 , param6 );	
	return 1.7;
};
double __cdecl test18_mix( double param0 , ::rux::int32 param1 , double param2 , ::rux::int32 param3 , double param4 , ::rux::int32 param5 , double param6 ,
	::rux::int32 param7 , double param8 , ::rux::int32 param9 , double param10 , ::rux::int32 param11 , double param12 , ::rux::int32 param13 , double param14 , ::rux::int32 param15 , double param16 , ::rux::int32 param17 )
{	
	printf( "test18_mix %f %d %f %d %f %d %f %d %f %d %f %d %f %d %f %d %f %d\n" , param0 , param1 , param2 , param3 , param4 , param5 , param6 , param7 , param8 , param9 , param10 , param11 , param12 , param13 , param14 , param15 , param16 , param17 );	
	return 1.7;
};
double __cdecl test1( char* module_ptr )
{	
	printf( "test1 %p\n" , module_ptr );	
	printf( "test1 %s\n" , module_ptr );	
	return 1.7;
};
float __cdecl test2( char* module_ptr , char* module_ptr2 )
{	
	printf( "test2 %p %p\n" , module_ptr , module_ptr2 );	
	printf( "test2 %s %s\n" , module_ptr , module_ptr2 );	
	return 2.6f;
};
rux::int64 __cdecl test3( char* module_ptr , char* module_ptr2 , char* module_ptr3 )
{	
	printf( "test3 %p %p %p\n" , module_ptr , module_ptr2 , module_ptr3 );	
	printf( "test3 %s %s %s\n" , module_ptr , module_ptr2 , module_ptr3 );	
	return 3;
};
char* __cdecl test4( char* module_ptr , char* module_ptr2 , char* module_ptr3 , char* module_ptr4 )
{	
	printf( "test4 %p %p %p %p\n" , module_ptr , module_ptr2 , module_ptr3 , module_ptr4 );	
	printf( "test4 %s %s %s %s\n" , module_ptr , module_ptr2 , module_ptr3 , module_ptr4 );	
	char* result = alloc_array_macros( char , 5 );
	::rux::engine::memcpy( result , 0 , "JOKE" , 5 );
	return result;
};
rux::uint32 __cdecl test5( char* module_ptr , char* module_ptr2 , char* module_ptr3 , char* module_ptr4 , char* module_ptr5 )
{	
	printf( "test5 %p %p %p %p %p\n" , module_ptr , module_ptr2 , module_ptr3 , module_ptr4 , module_ptr5 );	
	printf( "test5 %s %s %s %s %s\n" , module_ptr , module_ptr2 , module_ptr3 , module_ptr4 , module_ptr5 );	
	return 5;
};
rux::uint64 __cdecl test6( char* module_ptr , char* module_ptr2 , char* module_ptr3 , char* module_ptr4 , char* module_ptr5 , char* module_ptr6 )
{	
	printf( "test6 %p %p %p %p %p %p\n" , module_ptr , module_ptr2 , module_ptr3 , module_ptr4 , module_ptr5 , module_ptr6 );	
	printf( "test6 %s %s %s %s %s %s\n" , module_ptr , module_ptr2 , module_ptr3 , module_ptr4 , module_ptr5 , module_ptr6 );	
	return 6;
};
rux::int32 __cdecl test7( char* module_ptr , char* module_ptr2 , char* module_ptr3 , char* module_ptr4 , char* module_ptr5 , char* module_ptr6 , char* module_ptr7 )
{	
	printf( "test7 %p %p %p %p %p %p %p\n" , module_ptr , module_ptr2 , module_ptr3 , module_ptr4 , module_ptr5 , module_ptr6 , module_ptr7 );	
	printf( "test7 %s %s %s %s %s %s %s\n" , module_ptr , module_ptr2 , module_ptr3 , module_ptr4 , module_ptr5 , module_ptr6 , module_ptr7 );	
	return 7;
};
rux::int32 __cdecl test8( char* module_ptr , char* module_ptr2 , char* module_ptr3 , char* module_ptr4 , char* module_ptr5 , char* module_ptr6 , char* module_ptr7 , char* module_ptr8 )
{	
	printf( "test7 %p %p %p %p %p %p %p %p\n" , module_ptr , module_ptr2 , module_ptr3 , module_ptr4 , module_ptr5 , module_ptr6 , module_ptr7 , module_ptr8 );	
	printf( "test7 %s %s %s %s %s %s %s %s\n" , module_ptr , module_ptr2 , module_ptr3 , module_ptr4 , module_ptr5 , module_ptr6 , module_ptr7 , module_ptr8 );	
	return 8;
};
void process( char* line_ptr , XConsole* console_ptr )
{
	::rux::XString line( line_ptr );
	line = line.ToLower();
	line = line.Trim();
	if( line.Equals( "quit" , 4 )
		|| line.Equals( "exit" , 4 )
		|| line.Equals( "q" , 1 ) )
		console_ptr->Exit();
};
void thread_memory_test( void* , size_t ___rux__thread_index1986 )
{	
	declare_stack_variable( char , log , 1024 );
	declare_stack_variable( char , filename , 1024 );
	::rux::safe_strncpy( filename , ::rux::engine::_globals->_executable_directory , 1024 );
	rux_concatenation( filename , "/memory_test.log" );	
	::rux::uint64 time = 0;
	void* memory = NULL;
	{
		memory = alloc_array_macros( char , 1024 );
	}	
	if( time > 10ULL * 1000000ULL )
	{
		sprintf_s( log , 1024 , "alloc_array_macros , delay is equal "I64u" ms.\n" , time / 1000000ULL );
		rux_append_to_file( filename , log );
	}
	{
		::rux::engine::free_mem( memory );
	}
	if( time > 10ULL * 1000000ULL )
	{
		sprintf_s( log , 1024 , "rux::engine::free_mem , delay is equal "I64u" ms.\n" , time / 1000000ULL );
		rux_append_to_file( filename , log );
	}
};
void onexit( void )
{
	printf( "onexit!!!\n" );
};
void gui_on_event( ::rux::gui::Event& xevent , void* owner );
#ifdef __WINDOWS__
LONG __stdcall rux_structured_exception_handler( _EXCEPTION_POINTERS *ExceptionInfo )
{
	declare_stack_variable( char , callstack_ptr , 4096 );
	::rux::debug::get_CallStack( callstack_ptr , 4096 , ExceptionInfo );

	if( ExceptionInfo->ExceptionRecord )
		printf( "%s!!! %x\n%s" , __FUNCTION__ , ExceptionInfo->ExceptionRecord->ExceptionCode , callstack_ptr );
	return EXCEPTION_EXECUTE_HANDLER;
};
LONG __stdcall rux_vectored_exception_handler( _EXCEPTION_POINTERS *ExceptionInfo )
{
	printf( "%s!!!\n" , __FUNCTION__ );
	if( ExceptionInfo->ExceptionRecord )
	{
		printf( "%s!!! %x\n" , __FUNCTION__ , ExceptionInfo->ExceptionRecord->ExceptionCode );
		if( ExceptionInfo->ExceptionRecord->ExceptionCode == 0xc0000005
			|| ExceptionInfo->ExceptionRecord->ExceptionCode == 0xc0000374 )
			return rux_structured_exception_handler( ExceptionInfo );
		else
			return EXCEPTION_CONTINUE_SEARCH;
	}
	else
		return EXCEPTION_CONTINUE_SEARCH;
};
#endif
rux::gui::WindowBase* _window_base3 = NULL;
void on_process_stdout_stderr( void* udata , const char* data )
{
	printf( "%s" , data );
};
struct ttt
{
	::rux::uint32 _test0;
	::rux::uint8 _test1;
};
rux_volatile _detached_thread_count = 0;
void detached_thread_test( void* , size_t ___rux__thread_index1986 )
{
	::rux::threading::XThread cur( ::rux::threading::XThread::get_CurrentThread() );
	cur.Stop();
	printf( "%s, %d\n" , __FUNCTION__ , (::rux::int32)XInterlocked::Decrement( &_detached_thread_count ) );
};
void on_mouse_enter( const ::rux::gui::events::Event& event )
{
	::rux::gui::controls::XImage img( event._sender );
	img.set_Opacity( 1 );
};
void on_mouse_leave( const ::rux::gui::events::MouseEvent& event )
{
	::rux::gui::controls::XImage img( event._sender );
	img.set_Opacity( 0.5f );
};
#endif
#define MY_PAGE_SIZE 4096
size_t _mmaps_count = 0;
void* _mmaps[ 8192 ] = {0};
void* mmap_alloc( size_t size )
{
	char check[ MY_PAGE_SIZE ] = {0};
	::memset( check , 0x77 , MY_PAGE_SIZE );
	for( size_t index0 = 0 ; index0 < _mmaps_count ; index0++ )
	{
		char* ptr = (char*)_mmaps[ index0 ];
		if( memcmp( ptr , check , MY_PAGE_SIZE ) != 0 )
		{
			printf( "%s\n" , "something is wrong" );
		}
	}
	void* mem = 0;
#ifdef __LINUX__
	_mmaps[ _mmaps_count ] = mmap( 0 , 3 * MY_PAGE_SIZE , PROT_READ | PROT_WRITE , MAP_PRIVATE | MAP_ANONYMOUS , -1 , 0 );
	if( _mmaps[ _mmaps_count ] != (void*)-1 && _mmaps[ _mmaps_count ] != 0 )
	{
		char* ptr = (char*)_mmaps[ _mmaps_count ];
		::memset( ptr , 0x77 , 3 * MY_PAGE_SIZE );
		_mmaps_count++;
		if( mprotect( ptr , MY_PAGE_SIZE , PROT_NONE ) != -1 )
		{
			ptr += 2 * MY_PAGE_SIZE;
			if( mprotect( ptr , MY_PAGE_SIZE , PROT_NONE ) != -1 )
			{
				ptr -= MY_PAGE_SIZE;
				mem = ptr;
			}
			else
				printf( "%s\n" , "cannot mprotect(2)" );
		}
		else
			printf( "%s\n" , "cannot mprotect(1)" );
	}
	else
		printf( "%s\n" , "cannot alloc mem" );
#endif
	return mem;
};
int main(int argc, char ** args)
{	
	for( int index0 = 0 ; index0 < argc ; index0++ )
		printf( "arg%d = %s\n" , index0 , args[ index0 ] );
	{		
		::rux::engine::initialize();
	}
	{
		THREAD_REGISTER( "main" );
	//printf( "os='%s'\n" , ::rux::engine::_globals->_os );
#if not_empty_code
	//rux_native_set_on_any_exit( onexit );
		if( argc == 2 && strcmp( args[ 1 ] , "--start-client-test" ) == 0 )
		{
	#ifdef __LINUX__
			::rux::uint32 uid = 0 , gid = 0;
			if( ::rux::security::get_user_id( "test" , uid , gid ) )
			{
				setuid( uid );
				setgid( gid );
				//seteuid( uid );
				//setegid( gid );
			}
			umask( 0 );
			::rux::pid_t sid = setsid();
			execlp( "/bin/sh" , "/bin/sh" , "-c" , "/home/test/distr/client/video-client-7.0" , (char*)0 );
	#endif
			::rux::XConsole::WaitForEnter();
		}
		else if( argc == 2 && strcmp( args[ 1 ] , "--strncat-test" ) == 0 )
		{
			char test0[ 16 ] = {0};
			const char* src_test0 = "1 2 3 4 5 6 7 8 9 10 11 12 13 14 ";
			::rux::safe_strncat( test0 , src_test0 , 16 , strlen( src_test0 ) );

			test0[ 0 ] = 0;
			::rux::safe_strncat( test0 , src_test0 , 16 , 4 );

			const char* src_test1 = "1 2 3 4 5";
			::rux::safe_strncat( test0 , src_test1 , 16 , SIZE_MAX );
		}
		else if( argc == 2 && strcmp( args[ 1 ] , "--ssdp-cli-test" ) == 0 )
		{
			declare_variable( ::rux::XString , error );
			declare_variable( ::rux::network::XUdpClient , udp_cli );
			udp_cli.set_NetworkAddress( ::rux::network::XAddress( "239.255.255.250" , 1900 , error ) );
			char* send_data = "M-SEARCH * HTTP/1.1\r\nHost:239.255.255.250:1900\r\nST:ssdp:all\r\nMan:\"ssdp:discover\"\r\nMX:10\r\n\r\n";
			if( error.Length() == 0 )
			{
				::rux::uint32 start_tickcount = ::rux::XTime::GetTickCount();
				XData* data = 0;
				for( ; ; )
				{
					udp_cli.Send( send_data , strlen( send_data ) , error );
					if( error.Length() > 0 )
						break;
					udp_cli.Receive( &data , error );
					::rux::uint32 now_tickcount = ::rux::XTime::GetTickCount();
					if( now_tickcount < start_tickcount )
						start_tickcount = now_tickcount;
					if( now_tickcount - start_tickcount >= 10000 )
						break;
				}
			}
		}
		else if( argc == 2 && strcmp( args[ 1 ] , "--mmap-test" ) == 0 )
		{
			for( int pp = 0 ; pp < 8 ; pp++ )
				for( int yyy = 0 ; yyy < 2000 ; yyy++ )
				{
					for( int ww = 1 ; ww < 60 ; ww++ )
					{
						char * tt=(char *)mmap_alloc( ww );
						//memset(tt,0x88,ww);
					};
					::rux::threading::XThread::Sleep( 1 );
				};
		}
		else if( argc == 2 && strcmp( args[ 1 ] , "--protect-test" ) == 0 )
		{
			size_t page_size = ::rux::memory::get_page_size();
			printf( "Page size: %u bytes\n" , (::rux::uint32)page_size );
	#ifdef __WINDOWS__
			size_t alloc_size = 10 * 1024 * 1024;
			size_t size_to_alloc = ( alloc_size / page_size ) * page_size;
			void* mem = VirtualAlloc( NULL , size_to_alloc , MEM_COMMIT, PAGE_READWRITE );
			DWORD old_protect = 0;
			if( VirtualProtect( mem , page_size , PAGE_NOACCESS , &old_protect ) == 0 )
			{
				char error_string[ 1024 ] = {0};
				XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError , error_string );
			}
			else
			{
				char* ptr = (char*)mem;
				ptr += size_to_alloc - page_size;
				if( VirtualProtect( ptr , page_size , PAGE_NOACCESS , &old_protect ) == 0 )
				{
					char error_string[ 1024 ] = {0};
					XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError , error_string );
				}
				else
				{
					//ptr = (char*)mem;

					//char y = ptr[ size_to_alloc - page_size - 1 ];
					//y = ptr[ size_to_alloc - page_size ];
				}
			}
			VirtualFree( mem , 0 , MEM_RELEASE );
	#endif
		}
		else if( argc == 2 && strcmp( args[ 1 ] , "--used-memory-test" ) == 0 )
		{
			::rux::XConsole::WaitForEnter();
		}
		else if( argc == 2 && strcmp( args[ 1 ] , "--get-mac-address-test" ) == 0 )
		{
			declare_variable( ::rux::XString , error );
			declare_variable( ::rux::XArray< ::rux::XString > , interfaces );
			interfaces.set_ByRef( ::rux::network::XNetworkInformation::get_Interfaces( error ) );
			for( size_t index0 = 0 ; index0 < interfaces.Count() ; index0++ )
			{
				declare_variable( ::rux::XString , mac_address );
				mac_address.set_ByRef( ::rux::network::XNetworkInformation::get_MACAddress( interfaces[ index0 ] ) );
				printf( "Interface %s: MAC %s\n" , interfaces[ index0 ].str() , mac_address.str() );
			}		
		}
		else if( argc == 2 && strcmp( args[ 1 ] , "--hard-disk-test" ) == 0 )
		{
			char hard_disk_serial_number[ 1024 ] = {0};
			::rux::io::hard_disk::get_serial_number( 0 , hard_disk_serial_number , 1024 );
			printf( "Serial number: %s\n" , hard_disk_serial_number );
		}
		else if( argc == 2 && strcmp( args[ 1 ] , "--json-test" ) == 0 )
		{
			size_t t0 = cpp_offsetof( ::rux::memory::begin_memory_chunk , _diagnostics );
			size_t t1 = offsetof( ::rux::memory::begin_memory_chunk , _diagnostics );

			char* gg = alloc_array_macros( char , 4 );
			//char* ptr = gg - ( sizeof( ::rux::memory::begin_memory_chunk ) - cpp_offsetof( ::rux::memory::begin_memory_chunk , _prev ) );
			//ptr[ 0 ] = 89;
			::rux::engine::free_mem( gg );

			//CODE_CPU_INIT();
			declare_variable( ::rux::XString , error );
			XObject obj;
			{
				LAZY_PERFORMANCE;
				obj.set_ByRef( ::rux::data::XJsonSerializer::Deserialize( "[{\"name\":\"Joe\"},{\"name\":\"Matthew\"},{\"name\":\"Bill\"}]" , 1 , error , __FILE__ , __LINE__ ) );
			}
			if( rux_is_object( obj , ::rux::XArray< ::rux::data::XDataObject > ) )
			{
				declare_variable( ::rux::XArray< ::rux::data::XDataObject > , arr );
				arr.set_ByRef( obj );

				declare_variable( ::rux::XString , js );
				{
					LAZY_PERFORMANCE;
					js.set_ByRef( ::rux::data::XJsonSerializer::Serialize( arr , 1 , error ) );
				}
			}
			{
				LAZY_PERFORMANCE;
				obj.set_ByRef( ::rux::data::XJsonSerializer::Deserialize( "[23,2.33,1999999999999999]" , 1 , error , __FILE__ , __LINE__ ) );
			}
			if( rux_is_object( obj , ::rux::XArray< ::rux::XNumeric > ) )
			{
				declare_variable( ::rux::XArray< ::rux::XNumeric > , arr );
				arr.set_ByRef( obj );

				declare_variable( ::rux::XString , js );
				{
					LAZY_PERFORMANCE;
					js.set_ByRef( ::rux::data::XJsonSerializer::Serialize( arr , 1 , error ) );
				}
			}
			{
				LAZY_PERFORMANCE;
				obj.set_ByRef( ::rux::data::XJsonSerializer::Deserialize( "{\"1\":23,\"2\":2.33,\"3\":1999999999999999,\"4\":\"TEST\",\"5\":true,\"6\":\"Its larger than 16\",\"7\":{\"0\":24}}" , 1 , error , __FILE__ , __LINE__ ) );
			}
			if( rux_is_object( obj , ::rux::data::XDataObject ) )
			{
				declare_variable( ::rux::data::XDataObject , object );
				object.set_ByRef( obj );

				declare_variable( ::rux::XString , js );
				{
					LAZY_PERFORMANCE;
					js.set_ByRef( ::rux::data::XJsonSerializer::Serialize( object , 1 , error ) );
				}
			}
			{
				LAZY_PERFORMANCE;
				declare_variable( ::rux::XString , name );
				rux::data::XDataObject query;
				query.SetValue( "alias" , "conf" , 1 , SIZE_MAX , 0 );
				query.SetValue( "instruction" , "insert" , 1 , SIZE_MAX , 0 );
				query.SetValue( "table" , "videoserver" , 1 , SIZE_MAX , 0 );
				query.SetValue( "fields->vsrname" , name , "->" );
				query.SetValue( "fields->vsrport" , 0LL , "->" );
				query.SetValue( "fields->vsrcomment" , "" , 1 , SIZE_MAX , "->" );
				query.SetValue( "fields->vsrsort" , 0LL , "->" );
				query.SetValue( "fields->vsrinfo" , "" , 1 , SIZE_MAX , "->" );
				query.SetValue( "fields->vsrdirectaccess" , 1LL , "->" );
				query.SetValue( "fields->vsrip" , "" , 1 , SIZE_MAX , "->" );
				query.SetValue( "fields->vsrisreadevents" , 0LL , "->" );
				rux::XString query_string , error;
				query_string.set_ByRef( rux::data::XJsonSerializer::Serialize( query , 0 , error ) );
			}
			{
				LAZY_PERFORMANCE;
				obj.set_ByRef( ::rux::data::XJsonSerializer::Deserialize( "{\"instruction\":\"profile_data\",\"track_cmd_name\":\"trackSqlCommand\",\"track_data_name\":\"trackSqlData\",\"timeout\":30,\"parameters\":{\"json\":\"{\\\"alias\\\":\\\"conf\\\",\\\"instruction\\\":\\\"select\\\",\\\"table\\\":\\\"videoclient\\\",\\\"fields\\\":[\\\"vclsettinglocation\\\"],\\\"conditions\\\":[{\\\"field\\\":\\\"vclname\\\",\\\"operator\\\":\\\"=\\\",\\\"value\\\":\\\"asus-k53s-rux\\\"}]}\"}}" , error , __FILE__ , __LINE__ ) );
			}
			if( rux_is_object( obj , ::rux::data::XDataObject ) )
			{
				declare_variable( ::rux::data::XDataObject , object );
				object.set_ByRef( obj );

				declare_variable( ::rux::XString , js );
				{
					LAZY_PERFORMANCE;
					js.set_ByRef( ::rux::data::XJsonSerializer::Serialize( object , 1 , error ) );
				}
			}
			::rux::data::XDataObject test_obj;
			test_obj.AddValue( "name" , "JOE" , 1 , 3 , 0 , ___rux__thread_index1986 );
			test_obj.AddValue( "surname" , "JOEVICH" , 1 , 7 , 0 , ___rux__thread_index1986 );
			{
				LAZY_PERFORMANCE;
				::rux::data::json::serializer json( 1 , 1 );
				json.begin_array( 0 , __FILE__ , __LINE__ , ___rux__thread_index1986 );

				json.generate_boolean_value( 0 , true , __FILE__ , __LINE__ , ___rux__thread_index1986 );

				json.generate_int32_value( 0 , 1986 , __FILE__ , __LINE__ , ___rux__thread_index1986 );

				json.generate_string_value( 0 , "string" , __FILE__ , __LINE__ , ___rux__thread_index1986 );

				json.generate_null_value( 0 , __FILE__ , __LINE__ , ___rux__thread_index1986 );

				json.generate_string_value( 0 , "string" , __FILE__ , __LINE__ , ___rux__thread_index1986 );

					json.begin_object( 0 , __FILE__ , __LINE__ , ___rux__thread_index1986 );

						json.generate_boolean_value( "boolean" , false , __FILE__ , __LINE__ , ___rux__thread_index1986 );

						json.generate_int32_value( "int32" , 1986 , __FILE__ , __LINE__ , ___rux__thread_index1986 );

						json.generate_string_value( "str" , "string" , __FILE__ , __LINE__ , ___rux__thread_index1986 );

						json.generate_null_value( "null" , __FILE__ , __LINE__ , ___rux__thread_index1986 );

							json.begin_array( "array" , __FILE__ , __LINE__ , ___rux__thread_index1986 );

							json.end_array( __FILE__ , __LINE__ , ___rux__thread_index1986 );

							json.begin_object( "object" , __FILE__ , __LINE__ , ___rux__thread_index1986 );

							json.end_object( __FILE__ , __LINE__ , ___rux__thread_index1986 );

							json.begin_object( "object2" , __FILE__ , __LINE__ , ___rux__thread_index1986 );

								json.generate_string_value( "str" , "string" , __FILE__ , __LINE__ , ___rux__thread_index1986 );

							json.end_object( __FILE__ , __LINE__ , ___rux__thread_index1986 );

							json.generate_object_value( "obj" , test_obj() , error , __FILE__ , __LINE__ , ___rux__thread_index1986 );

					json.end_object( __FILE__ , __LINE__ , ___rux__thread_index1986 );

				json.end_array( __FILE__ , __LINE__ , ___rux__thread_index1986 );
			}
			//::rux::engine::_globals->_rux_gc_thread_collect( ___rux__thread_index1986 );
			::rux::XConsole::WaitForEnter();
		}
		else if( argc == 2 && strcmp( args[ 1 ] , "--double-linked-list-test" ) == 0 )
		{
			::rux::data::DoubleLinkedList< ::rux::data::key_value_pair_t > list;

			printf( "%u\n" , sizeof( list ) );
			::rux::XConsole::WaitForEnter();
		}
		else if( argc == 2 && strcmp( args[ 1 ] , "--map-test" ) == 0 )
		{
			{
				LAZY_PERFORMANCE;
				XMallocArray< ::rux::uint32 > test_array;
				for( ::rux::uint32 index0 = 0 ; index0 < 10000 ; index0++ )
				{
					test_array.Insert( 0 , index0 );
					//test_array.Add( index0 );
				}
			}

			{
				LAZY_PERFORMANCE;
				//::std::list< ::rux::uint32 , ::rux::memory::allocator< ::rux::uint32 > > test_array;
				//::std::vector< ::rux::uint32 , ::rux::memory::allocator< ::rux::uint32 > > test_array;
				::std::vector< ::rux::uint32 > test_array;
				for( ::rux::uint32 index0 = 0 ; index0 < 10000 ; index0++ )
				{
					test_array.insert( test_array.begin() , index0 );
					//test_array.push_back( index0 );
				}
			}


			{
				LAZY_PERFORMANCE;
				std::map< ::rux::uint32 , char* , std::less< ::rux::uint32 > , ::rux::memory::allocator< ::std::pair< ::rux::uint32 , char* > > > hashes;
				char test[ 32 ] = {0};
				for( size_t index0 = 0 ; index0 < 1000000 ; index0++ )
				{
					size_t test_len = sprintf( test , "test%u" , (rux::uint32)index0 );
			
					::rux::byte exists = 0;
					::rux::uint32 times33_hash = ::rux::cryptography::hash::times33_hash( test , test_len );
					std::map< ::rux::uint32 , char* , std::less< ::rux::uint32 > , ::rux::memory::allocator< ::std::pair< ::rux::uint32 , char* > > >::iterator i = hashes.find( times33_hash );
					if( i != hashes.end() )
						exists = 1;
					if( exists == 0 )
					{
						if( hashes.find( times33_hash ) == hashes.end() )
						{
							size_t len = test_len;
							if( len == SIZE_MAX )
								len = strlen( test );
							char* value = alloc_array_macros( char , len + 1 );
							::memcpy( value , test , len );
							value[ len ] = 0;
							hashes[ times33_hash ] = value;
						}
					}
				}

				{
					LAZY_PERFORMANCE;
					::std::vector< ::std::pair< ::rux::uint32 , char* > , ::rux::memory::allocator< ::std::pair< ::rux::uint32 , char* > > > hashes_vector( hashes.begin() , hashes.end() );
					//::std::vector< ::std::map< ::rux::uint32 , char* , std::less< ::rux::uint32 > , ::rux::memory::allocator< ::std::pair< ::rux::uint32 , char* > > >::value_type , ::rux::memory::allocator< ::std::pair< ::rux::uint32 , char* > > > hashes_vector( hashes.begin() , hashes.end() );
				}


				sprintf( test , "test%u" , (rux::uint32)1000002 );
				::rux::uint32 times33_hash = ::rux::cryptography::hash::times33_hash( test , SIZE_MAX );
				{
					LAZY_PERFORMANCE;
					if( hashes.find( times33_hash ) == hashes.end() )
					{
						printf( "Not found\n" );
					}
					else
						printf( "Found\n" );
				}
			}
			::rux::XConsole::WaitForEnter();
		}
		else if( argc == 2 && strcmp( args[ 1 ] , "--json-large-string-test" ) == 0 )
		{
			declare_stack_variable( char , filename , 1024 );
			::rux::safe_strncpy( filename , ::rux::engine::_globals->_executable_directory , 1024 );
			//rux_concatenation( filename , "/../../addons/json_test.txt" );
			rux_concatenation( filename , "/../../addons/json_mem_leak.txt" );
			::rux::io::file file( filename );
			if( file.opened() )
			{
				size_t size = file.size() + 1 , readen = 0;
				char* json = alloc_array_macros( char , size );
				if( file.read_text( json , size , readen ) )
				{
					char* ptr = json;
					if( memcmp( json , _rux_utf8_header , 3 ) == 0 )
						ptr = &json[ 3 ];
					::rux::XString error;
					{
						LAZY_PERFORMANCE;
						XObject obj( ::rux::data::XJsonSerializer::Deserialize( ptr , 1 , error , __FILE__ , __LINE__ ) );
					}
				}
				::rux::engine::free_mem( json );
			}
			::rux::XConsole::WaitForEnter();
		}
		else if( argc == 2 && strcmp( args[ 1 ] , "--detached-thread-test" ) == 0 )
		{
			//::rux::int32 thread_count;
			::rux::threading::XThread thread;
			XInterlocked::Increment( &_detached_thread_count );
			thread.Start();
			thread.Push( detached_thread_test , NULL );
			while( XInterlocked::CompareExchange( &_detached_thread_count , 0 , 0 ) != 0 )
				::rux::threading::XThread::Sleep( 1 );
		}
		else if( argc == 2 && strcmp( args[ 1 ] , "--gc-test" ) == 0 )
		{
			::rux::XString _stability_service_json , _executable_filename;
			_stability_service_json = "{\"command\":\"%s\" , \"type\":\"console\" , \"filename\":\"" + _executable_filename + "\" , \"pid\" : " + rux::XInt64( 276LL ).ToString() + " , \"timeout\" : %d , \"unique\" : \"%s\",\"mon\":%s,\"tue\":%s,\"wed\":%s,\"thu\":%s,\"fri\":%s,\"sat\":%s,\"sun\":%s,\"time\":\"%s\",\"usb_restart\":%s,\"restart_type\":\"%s\"}";					
		}
		else if( argc == 2 && strcmp( args[ 1 ] , "--cdecl-test" ) == 0 )
		{			
			rux_runtime_param_t empty_params[ 7 ];
			empty_params[ 0 ] = string6;
			empty_params[ 1 ] = (void*)0;
			empty_params[ 2 ] = (void*)0;
			empty_params[ 3 ] = (void*)0;
			empty_params[ 4 ] = (void*)0;
			empty_params[ 5 ] = (void*)0;
			empty_params[ 6 ] = (void*)0;
			printf( "%p , %p , %p , %p , %p , %p , %p\n" , string0 , string1 , string2 , string3 , string4 , string5 , string6 );
			rux_runtime_param_t params[ 8 ];
			params[ 0 ] = string7;
			params[ 1 ] = string6;
			params[ 2 ] = string5;
			params[ 3 ] = string4;
			params[ 4 ] = string3;
			params[ 5 ] = string2;
			params[ 6 ] = string1;
			params[ 7 ] = string0;	
			//printf( "%u , %u , %u , %u , %u , %u , %u\n" , params[ 0 ] , params[ 1 ] , params[ 2 ] , params[ 3 ] , params[ 4 ] , params[ 5 ] , params[ 6 ] );
			printf( "params=%p\n" , params );
			::rux::int64 result = 0;		
			printf( "before test0\n" );		
			::rux::int32 result_int32 = rux_runtime_invoke< ::rux::int32 >( (void*)test0 , params , 0 );		
			assert( result_int32 == 0 );
			printf( "after test0 result=%d,params=%p\n" , result_int32 , params );
			printf( "before test1\n" );
			double result_double = rux_runtime_invoke< double >( (void*)test1 , params , 1 );						
			assert( result_double == 1.7 );
			printf( "after test1 result=%f,params=%p\n" , result_double , params );		
			printf( "before test2\n" );
			float result_float = rux_runtime_invoke< float >( (void*)test2 , params , 2 );		
			assert( result_float == 2.6f );
			printf( "after test2 result=%f,params=%p\n" , result_float , params );
			printf( "before test2 with empty params\n" );
			result_float = rux_runtime_invoke< float >( (void*)test2 , empty_params , 2 );
			assert( result_float == 2.6f );
			printf( "after test2 with empty params result=%f,params=%p\n" , result_float , params );
			printf( "before test3\n" );
			result = rux_runtime_invoke< ::rux::int64 >( (void*)test3 , params , 3 );		
			assert( result == 3 );
			printf( "after test3 result="I64d",params=%p\n" , result , params );
			printf( "before test4\n" );
			char* result_string = rux_runtime_invoke< char* >( (void*)test4 , params , 4 );		
			assert( strcmp( result_string , "JOKE" ) == 0 );
			printf( "after test4 result=%s,params=%p\n" , result_string , params );
			printf( "before test4 with empty params\n" );
			result_string = rux_runtime_invoke< char* >( (void*)test4 , empty_params , 4 );		
			assert( strcmp( result_string , "JOKE" ) == 0 );
			printf( "after test4 with empty params result=%s,params=%p\n" , result_string , params );
			printf( "before test5\n" );
			::rux::uint32 result_uint32 = rux_runtime_invoke< ::rux::uint32 >( (void*)test5 , params , 5 );		
			assert( result_uint32 == 5 );
			printf( "after test5 result=%u,params=%p\n" , result_uint32 , params );
			printf( "before test6\n" );
			::rux::uint64 result_uint64 = rux_runtime_invoke< ::rux::uint64 >( (void*)test6 , params , 6 );		
			assert( result_uint64 == 6 );
			printf( "after test6 result="I64u",params=%p\n" , result_uint64 , params );
			printf( "before test7\n" );
			result_int32 = rux_runtime_invoke< ::rux::int32 >( (void*)test7 , params , 7 );		
			assert( result_int32 == 7 );
			printf( "after test7 result=%d,params=%p\n" , result_int32 , params );
			printf( "before test7 with empty params\n" );
			result_int32 = rux_runtime_invoke< ::rux::int32 >( (void*)test7 , empty_params , 7 );		
			assert( result_int32 == 7 );
			printf( "after test7 with empty params result=%d,params=%p\n" , result_int32 , params );
			printf( "before test8\n" );
			result_int32 = rux_runtime_invoke< ::rux::int32 >( (void*)test8 , params , 8 );		
			assert( result_int32 == 8 );
			printf( "after test8 result=%d,params=%p\n" , result_int32 , params );

			rux_runtime_param_t params_double[ 7 ];
			params_double[ 0 ] = 1986.25;
			printf( "before test1_double\n" );
			result_double = rux_runtime_invoke< double >( (void*)test1_double , params_double , 1 );
			assert( result_double == 1.75 );
			printf( "after test1_double result=%f,params=%p\n" , result_double , params_double );

			rux_runtime_param_t params_mix[ 18 ];
			params_mix[ 0 ] = 1986.25;
			params_mix[ 1 ] = 25;
			params_mix[ 2 ] = 1987.25;
			params_mix[ 3 ] = 26;
			params_mix[ 4 ] = 1988.25;
			params_mix[ 5 ] = 27;
			params_mix[ 6 ] = 1989.25;
			params_mix[ 7 ] = 28;
			params_mix[ 8 ] = 1990.25;
			params_mix[ 9 ] = 29;
			params_mix[ 10 ] = 1991.25;
			params_mix[ 11 ] = 30;
			params_mix[ 12 ] = 1992.25;
			params_mix[ 13 ] = 31;
			params_mix[ 14 ] = 1993.25;
			params_mix[ 15 ] = 32;
			params_mix[ 16 ] = 1994.25;
			params_mix[ 17 ] = 33;
		
			printf( "before test2_mix\n" );
			result_double = rux_runtime_invoke< double >( (void*)test2_mix , params_mix , 2 );
			assert( result_double == 1.7 );
			printf( "after test2_mix result=%f,params=%p\n" , result_double , params_mix );
			printf( "before test7_mix\n" );
			result_double = rux_runtime_invoke< double >( (void*)test7_mix , params_mix , 7 );
			assert( result_double == 1.7 );
			printf( "after test7_mix result=%f,params=%p\n" , result_double , params_mix );

			printf( "before test18_mix\n" );
			result_double = rux_runtime_invoke< double >( (void*)test18_mix , params_mix , 18 );
			assert( result_double == 1.7 );
			printf( "after test18_mix result=%f,params=%p\n" , result_double , params_mix );

			printf( "before test_stack\n" );
			result = rux_runtime_invoke< ::rux::int64 >( (void*)test_stack , params , 7 );		
			assert( result == 25 );
			printf( "after test_stack result="I64d",params=%p\n" , result , params );	
		}
		else if( argc == 2 && strcmp( args[ 1 ] , "--regexp-test" ) == 0 )
		{
			::rux::XString error;
			XRegexpPattern regexp_pattern;
			::rux::XString corrected_string;
			regexp_pattern = XRegexp::get_RegexpPattern( "(0[0-9]|1[0-9]|2[0-3]):[0-5][0-9]:[0-5][0-9]" , error );
			if( error.Length() == 0 )
			{
				::rux::uint32 index = 0 , count = 0;
				if( XRegexp::IndexOf( "16:07:09 year" , index , count , regexp_pattern ) )
				{
				}
				XRegexp::IndexOf( "Hello,my dear friend 16:07:09 year" , index , count , regexp_pattern );
				XRegexp::IndexOf( "16:07:" , index , count , regexp_pattern );			
				corrected_string.set_ByRef( XRegexp::Correct( "16:0R:" , regexp_pattern ) );

				corrected_string.set_ByRef( XRegexp::Correct( "16:0R:U7 io" , regexp_pattern ) );

				corrected_string.set_ByRef( XRegexp::Correct( "" , regexp_pattern ) );

				corrected_string.set_ByRef( XRegexp::Correct( "1116:07:09" , regexp_pattern ) );			
			}
			regexp_pattern = XRegexp::get_RegexpPattern( XRegexp::Ipv4Mask() , error );
			if( error.Length() == 0 )
			{
				corrected_string.set_ByRef( XRegexp::Correct( "127.0.01.01" , regexp_pattern ) );			

				::rux::uint8 res = XRegexp::IsHardValue( "192.168.040.102" , regexp_pattern , 3 );
			}
			regexp_pattern = XRegexp::get_RegexpPattern( "[1-9]+[0-9]*" , error );
			if( error.Length() == 0 )
			{
				corrected_string.set_ByRef( XRegexp::Correct( "124" , regexp_pattern ) );		

				corrected_string.set_ByRef( XRegexp::Correct( "124AS" , regexp_pattern ) );		
			}
		}
		else if( argc == 2 && strcmp( args[ 1 ] , "--std-test" ) == 0 )
		{
			XStdRedirect std;
			std.Start();		
			::rux::int32 index = 0;
			::rux::XString text;
			while( index < 1000 )
			{
				XConsole::Print( stderr , XRed , XString( "XConsole::Print " ) + XInt32( index ).ToString() + "\n" );
				printf( "printf %d\n" , index );
				text.set_ByRef( "write " + XInt32( index ).ToString() + "\n" );
	#ifdef __WINDOWS__
				_write( 1 , text.str() , text.Length() );
	#else
				write( 1 , text.str() , text.Length() );
	#endif
				index++;
			};		
			std.Stop();
			text.set_ByRef( std.get_Buffer() );
			printf( "%s\n", text.str() );
			index = 0;
			while( index < 1000 )
			{
				printf( "%d test message\n" , index );
	#ifdef __WINDOWS__
				_write( 1 , "write test\n" , 11 );
	#else
				write( 1 , "write test\n" , 11 );
	#endif
				index++;
			};
		}
		else if( argc == 2 && strcmp( args[ 1 ] , "--time-test" ) == 0 )
		{
			XTime time( 2013 , 1 , 13 , 16 , 45 , 15 , 5 );
			::rux::XString now_string( time.ToString( "%H:%M:%S.%MS %d.%m.%Y" ) );
			printf( "%s\n" , now_string.str() );

			XTime local_time( time.ConvertToLocalUnixTime() );
			printf( "%u.%u.%u %u:%u:%u.%u\n" , local_time.get_Year() , local_time.get_Month() , local_time.get_DayOfMonth() , local_time.get_Hour() , local_time.get_Minute() , local_time.get_Second() , local_time.get_Millisecond() );

			declare_stack_variable( char , now_uint64_string , 64 );
			::rux::uint64 now_uint64 = rux_get_now_as_local_unix_time();
			rux_unix_time_to_string( now_uint64 , now_uint64_string , 64 );
			printf( "rux_get_now_as_local_unix_time %s\n" , now_uint64_string );

			now_uint64 = rux_get_now_as_unix_time();		
			now_uint64_string[ 0 ] = '\0';
			rux_unix_time_to_string( now_uint64 , now_uint64_string , 64 );
			printf( "%s\n" , now_uint64_string );

			now_uint64 = rux_get_now_as_local_unix_time();	
			now_uint64_string[ 0 ] = '\0';
			rux_unix_time_to_string( now_uint64 , now_uint64_string , 64 );
			printf( "%s\n" , now_uint64_string );

			XTime now( ::rux::XTime::Now() );
			now = now.ConvertToLocalWindowsTime();
			now_string = now.ToString();
			printf( "now.ConvertToLocalWindowsTime %s\n" , now_string.str() );		
	
			now = ::rux::XTime::Now();
			now = now.ConvertToWindowsTime();
			now_string = now.ToString();
			printf( "now.ConvertToWindowsTime %s\n" , now_string.str() );
	
			now = ::rux::XTime::Now();
			now = now.ConvertToLocalUnixTime();
			now_string = now.ToString();
			printf( "now.ConvertToLocalUnixTime %s\n" , now_string.str() );
			now_uint64_string[ 0 ] = '\0';
			rux_unix_time_to_string( now.get_Time() , now_uint64_string , 64 );
			printf( "rux_unix_time_to_string %s\n" , now_uint64_string );
	
			now = ::rux::XTime::Now();
			now = now.ConvertToUnixTime();
			now_string = now.ToString();
			printf( "now.ConvertToUnixTime %s\n" , now_string.str() );
		}
		else if( argc == 2 && strcmp( args[ 1 ] , "--console-test" ) == 0 )
		{
			::rux::XConsole console;
			console.Process( process , 0 , NULL );
		}
		else if( argc == 2 && strcmp( args[ 1 ] , "--memory-test" ) == 0 )
		{
			declare_stack_variable( char , log , 1024 );
			declare_stack_variable( char , filename , 1024 );
			::rux::safe_strncpy( filename , ::rux::engine::_globals->_executable_directory , 1024 );
			rux_concatenation( filename , "/memory_test.log" );
			rux_remove_file( filename );
			XArray< threading::XThread > threads;
			for( ::rux::uint32 index0 = 0 ; index0 < 1000 ; index0++ )
			{
				::rux::threading::XThread thread;
				thread.set_ThreadMode( XThreadMode_Repeat );
				thread.set_Timeout( 1 );
				thread.Start();
				thread.Push( thread_memory_test , NULL );
				threads.Add( thread );
			}
			::rux::threading::XThread::Sleep( 20000 );
			for( ::rux::uint32 index0 = 0 ; index0 < threads.Count() ; index0++ )
				threads[ index0 ].Stop();
		}
		else if( argc == 2 && strcmp( args[ 1 ] , "--already-executing-test" ) == 0 )
		{
			if( rux_is_already_executing_in_current_path() == 0 )
			{
				printf( "Executing!!!\n" );
				::rux::threading::XThread::Sleep( 30000 );			
			}
			else
				printf( "Already executing!!!\n" );
		}
		else if( argc == 2 && strcmp( args[ 1 ] , "--console-ui-test" ) == 0 )
		{
			XConsole console;

			::rux::XString answer;
			do
			{
				XConsole::Print( stdout , XWhite , XString( "'$program' уже установлена.Выберите действие( 1,2 или 3).\n1.Обновить программу\n2.Удалить программу\n3.Отменить установку\n" ) );
				answer.set_ByRef( console.Read() );
			}
			while( answer.Equals( "1" , 1 ) == false
				&& answer.Equals( "2" , 1 ) == false
				&& answer.Equals( "3" , 1 ) == false );

		
			XConsole::Print( stdout , XWhite , "'$program' will be updated.\n" );
			console.Read();
		}
		else if( argc == 2 && strcmp( args[ 1 ] , "--callstack-test" ) == 0 )
		{
			::rux::engine::initialize();
			declare_stack_variable( char , stack , 8192 );
			::rux::debug::get_CallStack( stack , 8192 );
			printf( "Stack:\n%s\n" , stack );
		}
		else if( argc == 2 && strcmp( args[ 1 ] , "--loader-test" ) == 0 )
		{
			declare_stack_variable( char , exec_dir , 1024 );
			::rux::safe_strncpy( exec_dir , ::rux::engine::_globals->_executable_directory , 1024 );
	#ifdef __WINDOWS__
	#ifdef __x86__
			rux_concatenation( exec_dir , "/../../extern_lib/windows/win32" );
	#elif defined( __x64__ )
			rux_concatenation( exec_dir , "/../../extern_lib/windows/win64" );
	#endif
	#else
	#ifdef __x86__
			rux_concatenation( exec_dir , "/../extern_lib/linux/x86" );
	#elif defined( __x64__ )
			rux_concatenation( exec_dir , "/../extern_lib/linux/x64" );
	#endif
	#endif
			rux_add_search_directory( exec_dir );

			::rux::XString error , openssl_filename;
			::rux::XModule _openssl_module;
	#ifdef __WINDOWS__
			openssl_filename = "libeay32.dll";
	#else
			openssl_filename = "libssl.so";
	#endif
			_openssl_module.set_ByRef( ::rux::loader::LoadModule( openssl_filename , NULL , error ) );
			if( error.Length() == 0 )
			{
				_openssl_module.get_MethodAddress( "DES_set_key" , error );
				_openssl_module.get_MethodAddress( "ERR_clear_error" , error );
				_openssl_module.get_MethodAddress( "DES_ecb_encrypt" , error );
				_openssl_module.get_MethodAddress( "MD4" , error );
				_openssl_module.get_MethodAddress( "DES_set_odd_parity" , error );
			}
		}
		else if( argc == 2 && strcmp( args[ 1 ] , "--big-number-test" ) == 0 )
		{		
			::rux::uint8* data1 = NULL;
			::rux::uint32 data_size1 = 0;
			XRawData::FromString( "0x767367137153786786858768778" , data1 , data_size1 );
			XRawData number1 , number2 , result;
			number1.set_RawData( data1 , data_size1 );

			::rux::uint8* data2 = NULL;
			::rux::uint32 data_size2 = 0;
			XRawData::FromString( "0x6768173678213671836781238" , data2 , data_size2 );
			number2.set_RawData( data2 , data_size2 );
			result = number1 - number2;//149 642 172 321 143 659 878 621 787 354 432
			::rux::XString result_string;
			result_string = result.ToString();
			if( result_string != "149642172321143659878621787354432" )
			{
				printf( "incorrect!!!\n" );
			}
			result = number1 + number2;//150 666 261 726 457 196 176 699 361 761 712
			result_string = result.ToString();
			if( result_string != "150666261726457196176699361761712" )
			{
				printf( "incorrect!!!\n" );
			}
			result = number1 * number2;//150 666 261 726 457 196 176 699 361 761 712
			result_string = result.ToString();
			if( result_string != "76885671408611724261985664910978017215953474706911341669782080" )
			{
				printf( "incorrect!!!\n" );
			}
			result = number1 % number2;//150 666 261 726 457 196 176 699 361 761 712
			result_string = result.ToString();
			if( result_string != "125119145367360359295923891552" )
			{
				printf( "incorrect!!!\n" );
			}
			XRawData exp , temp_a , temp_b;
			exp = 3;
			result.Copy( number1 );
			result()->modmult( *exp() , *number2() , *temp_a() , *temp_b() );//375 357 436 102 081 077 887 771 674 656
			result_string = result.ToString();
			if( result_string != "375357436102081077887771674656" )
			{
				printf( "\n%s\n375357436102081077887771674656\nincorrect!!!\n" , result_string.str() );
			}
			result = ( number1 * exp ) % number2;//150 666 261 726 457 196 176 699 361 761 712
			result_string = result.ToString();
			if( result_string != "375357436102081077887771674656" )
			{
				printf( "incorrect!!!\n" );
			}
			result.Copy( number1 );
			result.FastRightShift();
			result_string = result.ToString();
			if( result_string != "75077108511900214013830287279036" )
			{
				printf( "incorrect!!!\n" );
			}
			result.Copy( number2 );
			result.FastRightShift();
			result_string = result.ToString();
			if( result_string != "256022351328384074519393601820" )
			{
				printf( "incorrect!!!\n" );
			}
			result.Copy( number1 );
			result.FastLeftShift();
			result_string = result.ToString();
			if( result_string != "300308434047600856055321149116144" )
			{
				printf( "incorrect!!!\n" );
			}
			result.Copy( number2 );
			result.FastLeftShift();
			result_string = result.ToString();
			if( result_string != "1024089405313536298077574407280" )
			{
				printf( "incorrect!!!\n" );
			}
			if( data1 )
				::rux::engine::free_mem( data1 );
			if( data2 )
				::rux::engine::free_mem( data2 );
		}
		else if( argc == 2 && strcmp( args[ 1 ] , "--semaphore-test" ) == 0 )
		{
			::rux::uint32 iterations = 0;
			::rux::uint64 now = ::rux::XTime::get_NowAsUnixTime();
			do
			{
				iterations++;
			}
			while( ::rux::XTime::get_NowAsUnixTime() - now >= 10000000 );
			printf( "%u\n" , iterations );
		}
		else if( argc == 2 && strcmp( args[ 1 ] , "--descriptors-test" ) == 0 )
		{
			declare_stack_variable( char , exec_dir , 1024 );
			rux_get_executable_filename( exec_dir );
			for( ; ; )
			{
				::rux::int32 file = rux_open( exec_dir , O_RDONLY );
				if( file == -1 )
				{
					if( errno == EMFILE )
					{
						printf( "%s\n" , "OPEN_MAX" );
					}
					break;
				}
			}

			::rux::int32 file = rux_open( exec_dir , O_RDONLY );
			if( file == -1 )
			{
				if( errno == EMFILE )
				{
					rux::int32 max_descriptors = ::rux::diagnostics::process::get_max_descriptors();
					declare_stack_variable( char , function , 8 * 1024 );
					sprintf_s( function , 8 * 1024 , "max_descriptors %d\n\n" , max_descriptors );
					::rux::XConsole::WriteToStdOut( function );
					for( ::rux::int32 fd = max_descriptors - 10 ; fd < max_descriptors ; fd++ )
						close( fd );
				}
			}

			declare_stack_variable( char , function , 2 * 8 * 1024 );
			::rux::pid_t pid = ::rux::diagnostics::process::get_process_id();
			declare_stack_variable( char , pid_string , 64 );
			rux::string::uint32_to_string( pid , pid_string );
			rux_is_exists_file_or_link( "/usr/bin/lsof" );
			char* args[ 5 ] = {0};
			args[ 0 ] = const_cast< char* >( "-p" );
			args[ 1 ] = pid_string;		
			::rux::memory::stack_chunk_t chunk( (::rux::byte*)function , 2 * 8 *1024 , ::rux::memory::string_type );
			::rux::diagnostics::process::start_with_redirect( "/usr/bin/lsof" , args , 2 , ::rux::diagnostics::on_default_redirect_stdout_or_stderr_handler , ::rux::diagnostics::on_default_redirect_stdout_or_stderr_handler , &chunk , NULL );
			if( chunk.actual_size() > 0 )
			{
				::rux::XConsole::WriteToStdOut( function );
			}
		}
		else if( argc == 2 && strcmp( args[ 1 ] , "--exception-test" ) == 0 )
		{
	#ifdef __WINDOWS__
			AddVectoredExceptionHandler( TRUE , rux_vectored_exception_handler );
			SetUnhandledExceptionFilter( rux_structured_exception_handler );
	#endif
			//HeapSetInformation( NULL , HeapEnableTerminationOnCorruption , NULL , 0 );

			/*_invalid_parameter_handler oldHandler, newHandler;
		   newHandler = myInvalidParameterHandler;
		   oldHandler = _set_invalid_parameter_handler(newHandler);
		   _CrtSetReportMode(_CRT_ASSERT, 0);

		   set_terminate( my_terminate_handler );

		   set_unexpected( my_unexpected_handler );

		   signal(SIGABRT, sigabrt_handler);

		   signal(SIGILL, sigabrt_handler);

		   signal(SIGFPE, sigabrt_handler);*/

		  // _set_security_error_handler(

			printf( "EXCEPTION TEST\n" );

		

			//_set_invalid_parameter_handler( rux_invalid_parameter_handler );
		
			//set_unexpected( rux_unexpected );
			//_set_purecall_handler( rux_purecall_handler );
			//SetErrorMode( SEM_FAILCRITICALERRORS | SEM_NOALIGNMENTFAULTEXCEPT | SEM_NOGPFAULTERRORBOX | SEM_NOOPENFILEERRORBOX );
			//_set_abort_behavior( 0 , _WRITE_ABORT_MSG | _CALL_REPORTFAULT );
		
			//rux_start_detached_process( "http://192.168.40.102:1080" , "" , 1 , rux_start_process_method_shell_execute );

			//XString string( "TOLERANCE" );
	#ifdef __UNIX__
			//alarm( 5 );
			//char* line_ptr = NULL;
			//size_t line_size = 0;
			//getline( &line_ptr , &line_size , stdin );
			//alarm( 0 );
	#endif		
			/*char test[ 4 ] = { 'a' , 'b' , 'c' , '\0' };
			test[ 4 ] = 'u';
			printf( "%s\n" , test );*/
			/*void* test = malloc( 11132 );
			free( test );
			free( test );*/
			/*rux::uint8* _array = alloc_array_macros( ::rux::uint8 , 1024 );
			rux::uint8 test = _array[ 4294967293 ];*/

			/*int* crappy( new int );
			if( crappy )
				delete crappy;
			if( crappy )
				delete crappy;*/

			/*rux::int32* p = NULL;
			*p = 0;*/
			/*char text[ 1024 ] = {0};
			sprintf_s( text , 1024 , "ddd %d %s" , "text" , 10 );
			printf( "%s\n" , text );*/
			int y = 1986 - 1800 - 186;
			int x = 167 / y;
			printf( "res %d\n" , x );
		}
		else if( argc == 2 && strcmp( args[ 1 ] , "--gui-engine-test" ) == 0 )
		{
			char* arguments_array[ 1024 ] = {0};
			size_t arguments_array_count = 0;
			char* copy = ::rux::arguments_string_to_array( "\"rivet    i\" --oppa karab \"hjhjhj\" jj" , arguments_array , arguments_array_count );

			::rux::engine::initialize();


			_window_base3 = ::rux::engine::_globals->_gui_globals->_rux_gui_create_window( gui_on_event );
			_window_base3->set_Left( 30 );
			_window_base3->set_Top( 30 );
			_window_base3->set_Width( 60 );
			_window_base3->set_Height( 60 );

			::rux::engine::_globals->_gui_globals->_rux_gui_start( gui_on_event , NULL );
		}
		else if( argc == 2 && strcmp( args[ 1 ] , "--start-detached-process-test" ) == 0 )
		{
			if( rux_is_exists_file_or_link( "/usr/bin/bash" ) == 1 )
				rux_start_detached_process( "/usr/bin/bash" , "\"/home/olecsys/rux/addons/detached_process.sh\" prosto \"privet\"" , 1 );
			else if( rux_is_exists_file_or_link( "/bin/bash" ) == 1 )
				rux_start_detached_process( "/bin/bash" , "\"/home/olecsys/rux/addons/detached_process.sh\" prosto \"privet\"" , 1 );
		}
		else if( argc == 2 && strcmp( args[ 1 ] , "--sizeof-test" ) == 0 )
		{	
			::rux::XModule tt;
			XArray< ::rux::XModule > ar;
			ar.Add( tt );
			tt()->Release();
			ar.RemoveAt( 0 );

			printf( "%s size = '%d'\n" , XString::get_Alias() , sizeof( XString ) );
			printf( "%s size = '%d'\n" , String::get_Alias() , sizeof( String ) );

			printf( "XGCRef size = '%d'\n" , sizeof( XGCRef ) );
			printf( "XObject size = '%d'\n" , sizeof( XObject ) );
			printf( "XKeyValuePair< XObject > size = '%d'\n" , sizeof( ::rux::XKeyValuePair< XObject > ) );
			printf( "off_t size = '%d'\n" , sizeof( off_t ) );

			XConsole::WaitForEnter();
		}
		else if( argc == 2 && strcmp( args[ 1 ] , "--start-redirect-test" ) == 0 )
		{
			::rux::memory::heap_chunk_t chunk;
	#ifdef __WINDOWS__
			char* args0[] =
			{
				"\"D:/core/build\"" ,
				"443"
			};
			::rux::int32 res0 = ::rux::diagnostics::process::start_with_redirect( "D:\\core\\build\\x86\\set_port.exe" , args0 , 2 , ::rux::diagnostics::on_default_redirect_stdout_or_stderr_handler , ::rux::diagnostics::on_default_redirect_stdout_or_stderr_handler , &chunk );		


			char* args[] =
			{
				"info" ,
				"\"D:\\rux\""
			};
			::rux::int32 res = ::rux::diagnostics::process::start_with_redirect( "svn.exe" , args , 2 , ::rux::diagnostics::on_default_redirect_stdout_or_stderr_handler , ::rux::diagnostics::on_default_redirect_stdout_or_stderr_handler , &chunk );		
	#else
			char* args[] =
			{
				"info" ,
				"/home/olecsys/rux"
			};		
			::rux::int32 res = ::rux::diagnostics::process::start_with_redirect( "svn" , args , 2 , ::rux::diagnostics::on_default_redirect_stdout_or_stderr_handler , ::rux::diagnostics::on_default_redirect_stdout_or_stderr_handler , &chunk );
	#endif
			if( chunk.actual_size() )
				printf( "res :\n%d\n%s\n" , res , (char*)chunk._data );
			else
				printf( "%s\n" , "test failed" );
			chunk.clear();
	#ifdef __WINDOWS__
			const char* args_string = "info \"D:\\rux\"";
			res = ::rux::diagnostics::process::start_with_redirect( "svn.exe" , args_string , ::rux::diagnostics::on_default_redirect_stdout_or_stderr_handler , ::rux::diagnostics::on_default_redirect_stdout_or_stderr_handler , &chunk );
	#else
			const char* args_string = "info /home/olecsys/rux";
			res = ::rux::diagnostics::process::start_with_redirect( "svn" , args_string , ::rux::diagnostics::on_default_redirect_stdout_or_stderr_handler , ::rux::diagnostics::on_default_redirect_stdout_or_stderr_handler , &chunk );
	#endif
			if( chunk.actual_size() )
				printf( "res :\n%d\n%s\n" , res , (char*)chunk._data );
			else
				printf( "%s\n" , "test failed" );
			chunk.clear();
			XConsole::WaitForEnter();
		}
		else if( argc == 2 && strcmp( args[ 1 ] , "--send-bat-test" ) == 0 )
		{
			declare_stack_variable( char , filename , 1024 );
			::rux::safe_strncpy( filename , ::rux::engine::_globals->_executable_directory , 1024 );
			rux_concatenation( filename , "/send_to_srv.bat" );
			::rux::string::replace_all( filename , '/' , '\\' );
			::rux::io::file file( filename , XCreateWriteText );
			if( file.opened() )
			{	
				declare_stack_variable( char , hostname , 1024 );
				declare_stack_variable( char , date , 64 );
				::rux::uint64 now = rux_get_now_as_local_unix_time();
				rux_unix_time_to_string( now , date , 64 );
				::rux::string::replace_all( date , ':' , '_' );
				::rux::string::replace_all( date , ' ' , '_' );
				::rux::string::replace_all( date , '.' , '_' );
				const char* srv = "\\\\192.168.40.88\\IntegraVideo7\\EXCEPTIONS";
				rux_get_hostname( hostname , 1024 );
				declare_stack_variable( char , dir , 1024 );
				::rux::safe_strncpy( dir , ::rux::engine::_globals->_executable_directory , 1024 );
				declare_stack_variable( char , modulename , 1024 );
				declare_stack_variable( char , bat_text , 8192 );
				rux_get_executable_filename_only( modulename );
				sprintf_s( bat_text , 8192 , "@set current_directory=\"%s\"\n"
					"@set executable_name=%s\n"
					"@set exception_ip=%s\n"
					"@set exception_date=%s\n"
					"@set exception_srv=%s\n"
					"@if not exist \"%%exception_srv%%\" mkdir \"%%exception_srv%%\"\n"
					"@if not exist \"%%exception_srv%%\\%%exception_ip%%\" mkdir \"%%exception_srv%%\\%%exception_ip%%\"\n"
					"@if not exist \"%%exception_srv%%\\%%exception_ip%%\\%%exception_date%%\" mkdir \"%%exception_srv%%\\%%exception_ip%%\\%%exception_date%%\"\n"
					"@set root_path=\"%%exception_srv%%\\%%exception_ip%%\\%%exception_date%%\\\"\n"
					"@copy \"%%current_directory%%\\%%executable_name%%.exception\" \"%%root_path%%%%executable_name%%.exception\"\n"
					"@copy \"%%current_directory%%\\rux.%%executable_name%%.exception\" \"%%root_path%%rux.%%executable_name%%.exception\"\n"
					"@mkdir \"%%root_path%%stable\"\n"
					"@mkdir \"%%root_path%%memory\"\n"
					"@mkdir \"%%root_path%%cpu\"\n"
					"@xcopy /e /v /q /r /y \"%%current_directory%%\\stable\" \"%%root_path%%stable\"\n"
					"@xcopy /e /v /q /r /y \"%%current_directory%%\\memory\" \"%%root_path%%memory\"\n"
					"@xcopy /e /v /q /r /y \"%%current_directory%%\\cpu\" \"%%root_path%%cpu\"\n"
					"@del \"%s\"" ,
					dir , modulename , hostname , date , srv , filename
					);
				file.clear();
				file.write_text( bat_text );
				file.close();
				rux_start_detached_process( filename , "" , 0 );
			}
		}
		else if( argc == 2 && strcmp( args[ 1 ] , "--io-test" ) == 0 )
		{
			::rux::uint32 chine_symbol = 13019;
			::rux::uint32 chine_symbol_length = XString::utf8_symbol_length( (char*)&chine_symbol , sizeof( ::rux::uint32 ) , 0 );		
			::rux::XString r( "OOO" );
			::rux::uint8 executing = 1;
			declare_stack_variable( char , filename , 1024 );
			declare_stack_variable( char , modulename , 1024 );
			::rux::safe_strncpy( filename , ::rux::engine::_globals->_executable_directory , 1024 );
			rux_get_executable_filename_only( modulename );
			rux_concatenation( filename , "/" );
			rux_concatenation( filename , modulename );
			rux_concatenation( filename , ".pid" );	
			::rux::io::file file( filename );
			if( file.opened() )
			{	
				declare_stack_variable( char , pid_string , 64 );
				size_t readen = 0;
				file.read_text( pid_string , 64 , readen );
				file.clear();
				file.write_text( "TEST" );
			}
		}
		else if( argc == 2 && strcmp( args[ 1 ] , "--os-restart-test" ) == 0 )
		{
			declare_stack_variable( char , error_string , 1024 );
			if( ::rux::diagnostics::os::restart( error_string ) )
				printf( "%s\n" , "os restart succeeded" );
			else
				printf( "%s , error = '%s'\n" , "os restart failed" , error_string );
		}
		else if( argc == 2 && strcmp( args[ 1 ] , "--png-test" ) == 0 )
		{
			declare_stack_variable( char , filename , 1024 );
			::rux::safe_strncpy( filename , ::rux::engine::_globals->_executable_directory , 1024 );
			rux_concatenation( filename , "/../../addons/png_test/warning.png" );
			::rux::media::png png( filename );
			for( ; ; )
			{
				::rux::byte new_image = 0;
				png.next_image( new_image );
				::rux::threading::XThread::Sleep( 1 );
			}
		}
		else if( argc == 2 && strcmp( args[ 1 ] , "--rem-dir-test" ) == 0 )
		{
			declare_stack_variable( char , filename , 1024 );
			::rux::safe_strncpy( filename , ::rux::engine::_globals->_executable_directory , 1024 );
			rux_concatenation( filename , "/addons" );
			::rux::io::directory::remove_all( filename );
		}
		else if( argc == 2 && strcmp( args[ 1 ] , "--thread-handle-test" ) == 0 )
		{
			::rux::uint32 tries = 0;
			while( tries < 100 )
			{		
				::rux::int32 e = 0;
				for( size_t index0 = 0 ; index0 < 10000000 ; index0++ )
				{
					e *= 100;
					e = ::sin( (float)e );
				}				
				printf( "e = %d\n" , e );
				::rux::uint64 cpu = ::rux::diagnostics::process::get_thread_cpu_time();
				printf( "THREAD "I64u" ms.\n" , cpu );
				tries++;
			}
		}
		else if( argc == 2 && strcmp( args[ 1 ] , "--get-os-test" ) == 0 )
		{
			declare_stack_variable( char , os_string , 1024 );
			::rux::diagnostics::os::get_os( os_string , 1024 );
			printf( "OS: %s\n" , os_string );
			printf( "Global OS: %s\n" , rux::engine::_globals->_os );
			XConsole::WaitForEnter();
		}
		else if( argc == 2 && strcmp( args[ 1 ] , "--username-test" ) == 0 )
		{
			char user_name[ 512 ] = {0};
			::rux::security::get_current_user_name( user_name , 512 );
			printf( "%s\n" , user_name );
		}
		else if( argc == 2 && strcmp( args[ 1 ] , "--gui-test" ) == 0 )
		{
			rux::engine::initialize();
			{
				rux_add_search_directory( "../../addons" );
				rux_add_search_directory( "../../addons/png_test" );

				::rux::XString error;
				::rux::gui::XWindow window;		
				window.set_BorderWidth( 5 );
				window.set_Width( 500 );
				window.set_Height( 500 );
				window.set_StartPosition( ::rux::gui::XEnum_StartPosition_Center );
				window.set_Background( ::rux::gui::Colors::ButtonNormalColor() );
				window.set_BorderColor( ::rux::gui::Colors::White() );

				::rux::gui::controls::XSelect test_select0;
				test_select0.set_Left( 30 );
				test_select0.set_Top( 30 );
				test_select0.set_Height( 20 );
				test_select0.set_Width( 100 );
				test_select0.set_MaxVisibleItems( 3 );
				test_select0.ItemsAdd( ::rux::XString( "0" ) );
				test_select0.ItemsAdd( ::rux::XString( "1" ) );
				test_select0.ItemsAdd( ::rux::XString( "2" ) );
				test_select0.ItemsAdd( ::rux::XString( "3" ) );
				test_select0.ItemsAdd( ::rux::XString( "4" ) );
				test_select0.ItemsAdd( ::rux::XString( "5" ) );
				test_select0.ItemsAdd( ::rux::XString( "6" ) );
				//window.AddControl( test_select0 , error );

				::rux::gui::controls::XButton exit;
				exit.set_Width( 40 );
				exit.set_Height( 20 );
				exit.set_HorizontalAlignment( ::rux::gui::XEnum_Alignment_RightOrBottom );
				exit.set_VerticalAlignment( ::rux::gui::XEnum_Alignment_LeftOrTop );
				exit.set_Content( ::rux::XString( "EXIT" ) );
				//window.AddControl( exit , error );

				::rux::gui::controls::XImage test_image0;
				test_image0.set_Width( 100 );
				test_image0.set_Height( 100 );
				test_image0.set_Left( 40 );
				test_image0.set_Top( 40 );
				test_image0.set_Opacity( 0.5f );
				test_image0.set_OnMouseEnter( on_mouse_enter );
				test_image0.set_OnMouseLeave( on_mouse_leave );
				rux::media::XFrame frame;		
				//frame.set_ImageData( "1.yv12" , error );
				//frame.set_ImageData( "brightness.bmp" , error );
				frame.set_ImageData( "usb.png" , error );
				//frame.set_ImageData( "warning.png" , error );
				//frame.set_ImageData( "alarm.png" , error );
				::rux::byte new_image = 0;
				test_image0.set_Frame( frame );									
				window.AddControl( test_image0 , error );

				::rux::gui::controls::XTextBlock test_textblock2;
				test_textblock2.set_Font( "tahoma.ttf" , 0 , 10 );
				test_textblock2.set_ControlName( "test_textblock2" );
				test_textblock2.set_Background( ::rux::gui::Colors::Black() );
				test_textblock2.set_Left( 20 );
				test_textblock2.set_Top( 20 );
				test_textblock2.set_HorizontalFilling( ::rux::gui::XEnum_Filling_FromContent );
				test_textblock2.set_VerticalFilling( ::rux::gui::XEnum_Filling_FromContent );
				test_textblock2.set_Text( "WOWWOWOWOW" );
				//window.AddControl( test_textblock2 , error );

				::rux::gui::controls::XTextBlock test_textblock1;
				test_textblock1.set_Font( "tahoma.ttf" , 0 , 11 );
				test_textblock1.set_ControlName( "test_textblock1" );
				test_textblock1.set_Background( ::rux::gui::Colors::Black() );
				test_textblock1.set_Left( 20 );
				test_textblock1.set_Top( 45 );
				test_textblock1.set_HorizontalFilling( ::rux::gui::XEnum_Filling_FromContent );
				test_textblock1.set_VerticalFilling( ::rux::gui::XEnum_Filling_FromContent );
				test_textblock1.set_Text( "WOWWOWOWOW" );
				//window.AddControl( test_textblock1 , error );

				::rux::gui::controls::XTextBlock test_textblock0;
				test_textblock0.set_Font( "tahoma.ttf" , 0 , 30 );
				test_textblock0.set_ControlName( "test_textblock0" );
				test_textblock0.set_Background( ::rux::gui::Colors::Black() );
				test_textblock0.set_Left( 20 );
				test_textblock0.set_Top( 70 );
				test_textblock0.set_HorizontalFilling( ::rux::gui::XEnum_Filling_FromContent );
				test_textblock0.set_VerticalFilling( ::rux::gui::XEnum_Filling_FromContent );
				test_textblock0.set_Text( "WOWWOWOWOW" );
				//window.AddControl( test_textblock0 , error );		

				::rux::gui::controls::XCheckBox test_checkbox0;
				test_checkbox0.set_ControlName( "test_checkbox0" );
				test_checkbox0.set_Text( "save as..." );
				test_checkbox0.set_HorizontalFilling( ::rux::gui::XEnum_Filling_FromContent );
				test_checkbox0.set_Left( 20 );
				test_checkbox0.set_Top( 170 );
				test_checkbox0.set_Width( 300 );
				test_checkbox0.set_Height( 20 );
				//window.AddControl( test_checkbox0 , error );

				::rux::gui::controls::XDatePicker test_datepicker0;
				test_datepicker0.set_ControlName( "test_datepicker0" );
				test_datepicker0.set_HorizontalFilling( ::rux::gui::XEnum_Filling_FromContent );
				test_datepicker0.set_Left( 20 );
				test_datepicker0.set_Top( 20 );
				test_datepicker0.set_Width( 100 );
				test_datepicker0.set_Height( 20 );
				//window.AddControl( test_datepicker0 , error );

				::rux::gui::controls::XTabControl test_tabcontrol0;
				test_tabcontrol0.set_ControlName( "test_tabcontrol0" );
				//test_tabcontrol0.set_HorizontalFilling( ::rux::gui::XEnum_Filling_FromContent );
				test_tabcontrol0.set_Left( 20 );
				test_tabcontrol0.set_Top( 170 );
				test_tabcontrol0.set_Width( 300 );			
				test_tabcontrol0.set_Height( 320 );
				declare_variable( ::rux::gui::controls::XGroup , page1 );
				test_tabcontrol0.AddPage( "page 1" , page1 );
				declare_variable( ::rux::gui::controls::XGroup , page2 );
				test_tabcontrol0.AddPage( "page 2" , page2 );
				//window.AddControl( test_tabcontrol0 , error );

				declare_variable( ::rux::gui::controls::XGroup , test_group0 );
				test_group0.set_Background( ::rux::gui::Colors::Blue( 0.5f ) );
				test_group0.set_Left( 30 );
				test_group0.set_Top( 40 );
				test_group0.set_ScrollXOffset( 40 );
				test_group0.set_Width( 100 );
				test_group0.set_Height( 100 );
			
				//test_group0.AddControl( test_image0 , error );

				//window.AddControl( test_group0 , error );

				::rux::threading::XThread thread;
				thread.set_Timeout( 1000 );
				thread.set_ThreadMode( XThreadMode_Repeat );
				thread.Start();
				thread.Push( test_thread0 , &window );

				window.Show();
			}
			rux::engine::deinitialize();
		}
		else if( argc == 2 && strcmp( args[ 1 ] , "--symsrv-test" ) == 0 )
		{
		}
		else if( argc == 2 && strcmp( args[ 1 ] , "--string-test" ) == 0 )
		{
			::rux::XString error;
			::rux::XRegexpPattern regexp_pattern( ::rux::XRegexp::get_RegexpPattern( "\\$\\(.*)" , error ) );
			::rux::XRegexp::FindAll( "  Privet $(inter) ioioiopi  $(rtrt) hello" , regexp_pattern );

			::rux::XString utf8_test0( "Привет!!!" );
			::rux::uint8 res = ::rux::string::is_utf8( utf8_test0.str() , utf8_test0.Size() - 1 );
			utf8_test0.set_ByRef( utf8_test0.ConvertToUTF8() );
			res = ::rux::string::is_utf8( utf8_test0.str() , utf8_test0.Size() - 1 );
	#ifdef __UNIX__
			const char* src = "luckys";
			declare_stack_variable( char , dst , 32 );
			iconv_t conv = ::rux::iconv::_iconv_open( "UTF16LE" , "UTF-8" );
			if( conv == (iconv_t)-1 )
				conv = ::rux::iconv::_iconv_open( "UTF-16LE" , "UTF-8" );
	#ifdef __LINUX__
			char* ptr1 = const_cast<char*>( src );
	#elif defined( __SOLARIS__)
			const char* ptr1 = src;
	#endif
			size_t ptr1_size = strlen( src );
			char* ptr2 = (char*)dst;
			size_t ptr2_size = 32;
			::rux::iconv::_iconv( conv , &ptr1 , &ptr1_size , &ptr2 , &ptr2_size );
			::rux::iconv::_iconv_close( conv );		
			for( size_t index0 = 0 ; index0 < 32 ; index0++ )
				printf( "%d %02x " , index0 , (::rux::int32)dst[ index0 ] );
	#endif

			::rux::XString test0( "luckys" );
			test0.set_ByRef( test0.ConvertToUTF16() );
			for( size_t index0 = 0 ; index0 < test0.Size() ; index0++ )
				printf( "%d %02x " , (::rux::int32)index0 , (::rux::int32)test0.str()[ index0 ] );
		}
		else if( argc == 2 && strcmp( args[ 1 ] , "--com-object-test" ) == 0 )
		{
	#ifdef __WINDOWS__
			printf( "%s\n" , ComTestInterfaceGuid );

			ComTestClassFactory* comTestClassFactory = ::rux::engine::alloc_object< ComTestClassFactory >();

			IUnknown* unk_obj = NULL;
			comTestClassFactory->CreateInstance( NULL , IID_IUnknown , (void**)&unk_obj );
			unk_obj->Release();
			unk_obj = NULL;

			IComTestInterface* com_interface = NULL;
			comTestClassFactory->CreateInstance( NULL , __uuidof( IComTestInterface ) , (void**)&com_interface );
			com_interface->Release();
			com_interface = NULL;		
	#endif
		}
		else if( argc == 2 && strcmp( args[ 1 ] , "--reflection-test" ) == 0 )
		{
			XGCHandle< rux::String > string_test2;
			::rux::XString string_test0( "Hello, my friend!!!" );
				

			::rux::XArray< XObject > arr_test0;
			arr_test0.Add( ::rux::XString( "First" ) );
			arr_test0.Add( ::rux::XString( "Second" ) );
			arr_test0.Add( ::rux::XInt32( 1986 ) );

			XType type( typeof( ::rux::XInt32 ) );
		
			::rux::ConstructorInfo* ctor_info = type.get_ConstructorInfo();
			if( ctor_info )
			{
				XObject obj( ctor_info->Create() );
			}
		}
		else if( argc == 2 && strcmp( args[ 1 ] , "--get-directories-test" ) == 0 )
		{
			XMallocArray< ::rux::io::directory* > dirs;
			::rux::io::directory::get_directories( "/proc" , dirs );
			for( size_t index0 = 0 ; index0 < dirs.Count() ; index0++ )
			{
				::rux::string dir( dirs[ index0 ]->get_directory_path() , 1 , 0 );		
				printf( "%s\n" , dir._str );
				::rux::engine::free_object< ::rux::io::directory >( dirs[ index0 ] , ::rux::engine::rux_free_dummy );
			}
		}
		else if( argc == 2 && strcmp( args[ 1 ] , "--pidof-test" ) == 0 )
		{
			declare_stack_variable( char , executable , 1024 );
			rux_get_executable_filename_only( executable );
			::rux::pid_t pid0 = 0 , pid1 = 0;
	#ifdef __WINDOWS__
			::rux::diagnostics::process::executing( "C:\\work\\core\\build\\x64\\guard.exe" , &pid0 );
	#else
			::rux::diagnostics::process::executing( "/home/olecsys/core/build/video-server-7.0" , &pid0 );
	#endif
			printf( "pid = %u\n" , pid0 );
			::rux::diagnostics::process::executing( "stability" , &pid1 );
			printf( "pid = %u\n" , pid1 );
	#ifdef __WINDOWS__
			if( ::rux::diagnostics::process::executing( "C:\\work\\core\\build\\x64\\guard.exe" , pid0 ) )
	#else
			if( ::rux::diagnostics::process::executing( "/home/olecsys/core/build/video-server-7.0" , pid0 ) )
	#endif
				printf( "pid = %u is executing\n" , pid0 );
			else
				printf( "pid = %u is not executing\n" , pid0 );
		}
		else if( argc == 2 && strcmp( args[ 1 ] , "--stability-server-pipe-test" ) == 0 )
		{
			rux::XPipeServer pipe_server;
			pipe_server.set_OnReceived( on_pipe_received );
			pipe_server.AddAllAllowAccessUserOrGroup( "everyone" );
			pipe_server.AddAllDenyAccessUserOrGroup( "network" );
			rux::XString error;
			pipe_server.StartListen( "_rux_stability" , error );
			::rux::threading::XThread::Sleep( 60000 );
			pipe_server.StopListen();
		}
		else if( argc == 2 && strcmp( args[ 1 ] , "--stability-pipe-test" ) == 0 )
		{
			printf( "%s\n" , "start test" );
			::rux::int32 tries = 0;
			while( tries < 200 )
			{
				::rux::int32 res = register_in_stability( "test" );
				printf( "res=%d\n" , res );
				::rux::threading::XThread::Sleep( 5 );
				tries++;
			}
			printf( "%s\n" , "stop test" );
		}
		else if( argc == 2 && strcmp( args[ 1 ] , "--environ-test" ) == 0 )
		{
	#ifdef __WINDOWS__
			wchar_t* envs = GetEnvironmentStringsW();
			if( envs )
			{		
				wchar_t* ptr = envs;
				while( *ptr != 0 )
				{
					size_t len = wcslen( ptr );
					::rux::XString env_string( ptr );
					env_string.set_ByRef( env_string.ConvertToUTF8() );
					::rux::int32 index = env_string.IndexOf( '=' );
					if( index != -1 )
					{
						::rux::XString env_name , env_value;
						env_name.set_ByRef( env_string.Substring( 0 , index ) );
						env_value.set_ByRef( env_string.Substring( index + 1 ) );

						printf( "%s, name %s, value %s\n" , env_string.str() , env_name.str() , env_value.str() );
					}
					ptr += len + 1;
				}
				FreeEnvironmentStringsW( envs );
			}
	#else
			int i = 1;
			char* s = *environ;
			for( ; s ; i++ )
			{
				::rux::XString env_string( s );
				::rux::int32 index = env_string.IndexOf( '=' );
				if( index != -1 )
				{
					::rux::XString env_name , env_value;
					env_name.set_ByRef( env_string.Substring( 0 , index ) );
					env_value.set_ByRef( env_string.Substring( index + 1 ) );

					printf( "%s, name %s, value %s\n" , env_string.str() , env_name.str() , env_value.str() );
				}
				s = *( environ + i );
			}
	#endif
		}
		else if( argc == 2 && strcmp( args[ 1 ] , "--native-string-test" ) == 0 )
		{
			::rux::string string0( "Hello!!!" );
			::rux::string res0 = "Check " + string0;
			res0 += ", my friend!!!";

			res0.replace_all( '!' , ' ' );
		}
		else if( argc == 2 && strcmp( args[ 1 ] , "--process-info-test" ) == 0 )
		{
			declare_stack_variable( char , executable , 1024 );
			rux_get_executable_filename_only( executable );
			::rux::int64 value = ::rux::diagnostics::process_info::working_set( executable , 0 );
			printf( "working set = "I64d"\n" , value );
			value = ::rux::diagnostics::process_info::virtual_bytes( executable , 0 );
			printf( "virtual bytes = "I64d"\n" , value );
			::rux::pid_t pid = ::rux::diagnostics::process::get_process_id();
			value = ::rux::diagnostics::process_info::threads_count( pid , 0 );
			printf( "threads = "I64d"\n" , value );
			::rux::XConsole::WaitForEnter();
		}
		else if( argc == 2 && strcmp( args[ 1 ] , "--pid-from-status-test" ) == 0 )
		{		
			declare_stack_variable( char , filename , 1024 );
			::rux::safe_strncpy( filename , ::rux::engine::_globals->_executable_directory , 1024 );
			rux_concatenation( filename , "/../../addons/status" );
			::rux::io::file file( filename );
			if( file.opened() )
			{	
				::rux::uint64 filesize = file.size();
				if( filesize > 0 )
				{
					char* lines = (char*)alloc_array_macros( char , filesize );
					size_t readen = 0;
					if( file.read_text( lines , filesize , readen ) )
					{
						file.close();
						char* ptr = strstr( lines , "Name:" );
						if( ptr )
						{
							ptr += 5;	
							char* name_string = ptr;
							ptr = strstr( ptr , "\n" );
							if( ptr )
							{
								*ptr = 0;
								ptr++;
								while( *name_string == '\t'
									|| *name_string == '\r' 
									|| *name_string == ' ' )
									name_string++;
								ptr = strstr( ptr , "Pid:" );
								if( ptr == NULL )
									ptr = strstr( lines , "Pid:" );
								if( ptr )
								{
									ptr += 4;
									char* pid_string = ptr;
									ptr = strstr( ptr , "\n" );
									if( ptr )
									{
										*ptr = 0;
										while( *pid_string == '\t'
											|| *pid_string == '\r' 
											|| *pid_string == ' ' )
											pid_string++;
										::rux::pid_t local_pid = ::rux::string::to_uint64( pid_string );
										printf( "%s %u\n" , name_string , local_pid );
									}
								}							
							}
						}
					}
					else
						file.close();
					::rux::engine::free_mem( lines );
				}
			}
		}
	#else
		::rux::XConsole::WaitForEnter();
	#endif
	}

	

	::rux::deinitialize();

	//::rux::engine::_globals->_rux_gc_thread_collect( ___rux__thread_index1986 );

	printf( "rux_deinitialize\n" );	
	return 0;
};
#if not_empty_code
rux::gui::WindowBase* _window_base = NULL;
rux::gui::WindowBase* _window_base2 = NULL;
void window_close_thread( void* )
{	
	if( _window_base )
	{
		_window_base->set_TopMost( 1 );
		::rux::threading::XThread::Sleep( 1000 );
		_window_base->set_TopMost( 0 );
		::rux::threading::XThread::Sleep( 1000 );
		_window_base->Hide();
		::rux::threading::XThread::Sleep( 1000 );
		_window_base->Show();
		::rux::threading::XThread::Sleep( 1000 );
		_window_base->Maximize();
		::rux::threading::XThread::Sleep( 1000 );
		_window_base->Restore();
		::rux::threading::XThread::Sleep( 1000 );
		_window_base->Minimize();
		::rux::threading::XThread::Sleep( 1000 );
		_window_base->Restore();
		::rux::threading::XThread::Sleep( 1000 );
		_window_base->Minimize();
		::rux::threading::XThread::Sleep( 1000 );
		_window_base->Maximize();
		::rux::threading::XThread::Sleep( 1000 );
		_window_base->Maximize();
		::rux::threading::XThread::Sleep( 1000 );
		_window_base->Minimize();
		::rux::threading::XThread::Sleep( 1000 );
		
		::rux::engine::_globals->_gui_globals->_rux_gui_close_all_windows();
		if( ::rux::engine::_globals && ::rux::engine::_globals->_gui_globals->_rux_gui_windows_count() == 0 )
			::rux::engine::_globals->_gui_globals->_rux_gui_stop( gui_on_event , NULL );
	}
}
void gui_on_event( ::rux::gui::Event& xevent , void* )
{
	switch( xevent._type )
	{
	case ::rux::gui::XEnum_EventType_GuiInitialize:
		{
			_window_base3->Show();
			::rux::threading::XThread::Sleep( 1000 );
			_window_base3->Close();

			_window_base = ::rux::engine::_globals->_gui_globals->_rux_gui_create_window( gui_on_event );

			_window_base->Show();

			//_window_base2 = ::rux::engine::_globals->_gui_globals->_rux_gui_create_window();

			//_window_base2->Show();

			break;
		}
	case ::rux::gui::XEnum_EventType_WindowNormalState:
		{
			printf( "%s\n" , "XEnum_EventType_WindowNormalState" );
			break;
		}
	case ::rux::gui::XEnum_EventType_WindowLoaded:
		{
			printf( "%s\n" , "XEnum_EventType_WindowLoaded" );
			break;
		}
	case ::rux::gui::XEnum_EventType_WindowMinimizedState:
		{
			printf( "%s\n" , "XEnum_EventType_WindowMinimizedState" );
			break;
		}
	case ::rux::gui::XEnum_EventType_WindowMaximizedState:
		{
			printf( "%s\n" , "XEnum_EventType_WindowMaximizedState" );
			break;
		}
	case ::rux::gui::XEnum_EventType_WindowShown:
		{
			printf( "%s\n" , "XEnum_EventType_WindowShown" );
			break;
		}
	case ::rux::gui::XEnum_EventType_WindowHided:
		{
			printf( "%s\n" , "XEnum_EventType_WindowHided" );
			break;
		}
	}
};
#endif
#ifdef __UNIX__

#endif
