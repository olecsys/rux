#ifndef XAN_THREAD_H
#define XAN_THREAD_H
#include "xan_defines.h"
#include "xan_crtsect.h"
#include "xan_string.h"
#include "xan_malloc_array.h"
#include "xan_event.h"
namespace rux
{
	namespace threading
	{
		typedef void (*thread_before_exit_t)( void* );
#ifdef __SOLARIS__
extern "C" void* rux_thread_function( void* param );
#endif
		begin_declare_rux_class( Thread );
			~Thread();
			void Start( void );
			void Stop( void );	
			void Push( XTHREADFUNC func , void* param );
			void set_ThreadMode( XThreadMode xtmMode );
			void set_Timeout( ::rux::uint32 timeout_in_milliseconds );
			::rux::uint8 get_IsWaitForExit( void );
			::rux::uint8 get_IsStarted( void );
			bool get_IsBusy( void );
			void set_StackSize( size_t stack_size );
			void set_ThreadName( const char* thread_name_ptr );
			void Wait( void );
			::rux::pid_t get_ThreadId( void );
			char* get_ThreadName( void );
			void set_BeforeExit( ::rux::threading::thread_before_exit_t thread_before_exit , void* user_data );
		private:
#ifdef __WINDOWS__
			static ::rux::uint32 __stdcall rux_thread_function( void* param );
#ifdef __x64__
			static void __stdcall queue_user_apc_function( ::rux::uint64 param );
			static void __stdcall EmptyFunction( ::rux::uint64 param );
#elif defined( __x86__ )
			static void __stdcall queue_user_apc_function( unsigned long param );
			static void __stdcall EmptyFunction( unsigned long param );
#endif
#endif
#ifdef __LINUX__
			static void* rux_thread_function( void* param );
#endif	
		begin_declare_rux_class_members( Thread );
			rux_volatile _is_started;
			::rux::pid_t _thread_id;
			::rux::threading::thread_before_exit_t _thread_before_exit;
			void* _thread_before_exit_user_data;
			XThreadMode _mode;
			::rux::uint32 _repeat_timeout;
			::rux::threading::Event* _event;
			char _thread_name[ 16 ];
			::rux::threading::RdWrLock _cs_functions;
#ifdef __WINDOWS__	
			HANDLE _thread_handle;
			XTHREADFUNC _repeat_function;
			void* _repeat_param;
#elif defined( __UNIX__ )
			pthread_t _pthread;			
			XMallocArray< void* > _functions;
			XMallocArray< void* > _functions_params;
			::rux::threading::Event* _wait_event;
#endif
			::rux::uint32 _function_execution_last_time;
			size_t _stack_size;
			rux_volatile _is_busy;
			size_t _thread_index;
		end_declare_rux_class_members( Thread );
			void Start( void );
			void Stop( void );	
			void Push( XTHREADFUNC func , void* param );
			void set_ThreadMode( XThreadMode xtmMode );
			void set_Timeout( ::rux::uint32 timeout_in_milliseconds );
			::rux::uint8 get_IsStarted( void );
			static void Sleep( ::rux::uint32 milliseconds );
			void set_ThreadName( const char* thread_name_ptr );
			::rux::uint8 get_IsWaitForExit( void );
			static ::rux::pid_t get_CurrentThreadId( void );
			void Wait( void );
			static Thread& get_CurrentThread( void );
			static ::rux::Array< XThread >& get_Threads( void );	
			void set_StackSize( size_t stack_size );
			bool get_IsBusy( void );
			void set_BeforeExit( ::rux::threading::thread_before_exit_t thread_before_exit , void* user_data );
		end_declare_rux_class();
	};
};
#endif
