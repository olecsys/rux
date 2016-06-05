#ifndef XAN_DEBUG_H
#define XAN_DEBUG_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_gchandle.h>
#include <xan_crtsect.h>
#ifdef __WINDOWS__
#include <DbgHelp.h>
#endif
namespace rux
{
	namespace debug
	{
#ifdef __WINDOWS__
		dll_internal void get_CallStack( char stack_ptr[] , size_t stack_ptr_size , _EXCEPTION_POINTERS *ExceptionInfo , size_t offset = 0 );
#endif	
		dll_internal ::rux::uint16 get_CallStack( void** stack_frames , size_t offset = 0 );
		dll_internal void get_CallStack( char stack_ptr[] , size_t stack_ptr_size , void** stack_frames , ::rux::uint16 stack_frames_count , size_t offset = 0 );
		dll_internal void initialize( void );
		dll_internal void deinitialize( void );
		dll_internal void StartCatchException( ::rux::rux_exception_event_t rux_exception_event );
#if defined( __LINUX__ )	
		dll_internal void debug_posix_death_signal2( ::rux::int32 signum , siginfo_t* siginfo , void* context );
		dll_internal void debug_posix_alarm_signal2( ::rux::int32 signum , siginfo_t* siginfo , void* context );
#endif
#ifdef __WINDOWS__
		dll_internal LONG __stdcall exception_filter( _EXCEPTION_POINTERS *ExceptionInfo );
		dll_internal LONG __stdcall vectored_exception_handler( _EXCEPTION_POINTERS *ExceptionInfo );
		dll_internal void create_mini_dump( const char* filename , _EXCEPTION_POINTERS *ExceptionInfo );
#endif
		dll_internal void RegisterExceptionEvent( const char* module_name , ::rux::rux_exception_event_t exception_event );
		dll_internal void UnRegisterExceptionEvent( ::rux::rux_exception_event_t exception_event );
		dll_internal void get_CallStack( char stack_ptr[] , size_t stack_ptr_size , size_t offset = 0 );
		dll_internal void send_to_exception_server( void );
#ifdef __WINDOWS__
		dll_internal void ExceptionToFile( const char* exception_code_string , _EXCEPTION_POINTERS* info );
#else
		dll_internal void ExceptionToFile( const char* exception_code_string );
#endif
		dll_internal void CheckFileDescriptors( const char* exception_filename , char* exception_text );
		dll_internal void on_lsof_stdout_or_stderr_handler( void* udata , const char* data , size_t data_size );
#ifdef __SOLARIS__
		extern "C" void debug_posix_alarm_signal2( ::rux::int32 signum , siginfo_t* siginfo , void* context );
		extern "C" void debug_posix_death_signal2( ::rux::int32 signum , siginfo_t* siginfo , void* context );
#endif
	};
};
#endif
