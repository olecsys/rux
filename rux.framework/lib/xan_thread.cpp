#include "xan_thread.h"
#include "xan_time.h"
#include "xan_keyvaluepair.h"
#include "xan_uint64.h"
#include "xan_user_data.h"
#include "xan_gui_defines.h"
#include "xan_boolean.h"
#include "xan_log.h"
begin_implement_rux_class_ns( Thread , rux::threading );
	_thread_before_exit = NULL;
	_thread_before_exit_user_data = NULL;
	_event = alloc_object_macros( ::rux::threading::Event );
	_repeat_timeout = ::rux::threading::infinite;
	_mode = XThreadMode_OneTime;	
#ifdef __WINDOWS__
	_repeat_param = 0;
	_repeat_function = 0;
	_thread_handle = 0;
#elif defined( __UNIX__ )
	_wait_event = alloc_object_macros( ::rux::threading::Event );
	memset( &_pthread , 0 , sizeof( pthread_t ) );		
#endif
	_is_started = 0;
	::rux::safe_strncpy( _thread_name , "xan thread" , 16 );
	_thread_id = 0;
	_function_execution_last_time = 0;
	_stack_size = 512 * 1024;
	_is_busy = 0;
	_thread_index = SIZE_MAX;
end_implement_rux_class();
namespace rux
{
	namespace threading
	{
		XThreadRegister::XThreadRegister( const char* thread_name )
		{
			if( ::rux::engine::_globals && ::rux::engine::_globals->_rux_add_thread_id_to_global )
				_thread_index = ::rux::engine::_globals->_rux_add_thread_id_to_global( ::rux::threading::XThread::get_CurrentThreadId() , thread_name , _rux_current_module_index , 0 );
			else
				_thread_index = SIZE_MAX;
		};
		XThreadRegister::~XThreadRegister()
		{	
			if( ::rux::engine::_globals && ::rux::engine::_globals->_rux_remove_thread_from_global )
				::rux::engine::_globals->_rux_remove_thread_from_global( _thread_index );
		};
		Thread::~Thread()
		{
			Stop();
			_event->Release();
#if defined( __UNIX__ )
			_wait_event->Release();
#endif
		};
		char* Thread::get_ThreadName( void )
		{
			return _thread_name;
		};
		::rux::pid_t Thread::get_ThreadId( void )
		{
			return _thread_id;
		};
		implement_duplicate_internal_function_2( Thread , set_BeforeExit , ::rux::threading::thread_before_exit_t , void* );
		void Thread::set_BeforeExit( ::rux::threading::thread_before_exit_t thread_before_exit , void* user_data )
		{
			::rux::memory::check_memory( const_cast< Thread* >( this ) , 0 , sizeof( Thread ) );
			_thread_before_exit = thread_before_exit;
			_thread_before_exit_user_data = user_data;
		};
		void Thread::set_StackSize( size_t stack_size )
		{
			_stack_size = stack_size;
		};
		void XThread::set_StackSize( size_t stack_size )
		{
			(*this)()->set_StackSize( stack_size );
		};
		void Thread::Start( void )
		{
			char name[ 128 ] = {0};
			snprintf( name , 128 , "[%u][0x%x] %s" , (::rux::uint32)_thread_id , (::rux::uint32)_thread_id , _thread_name );
			CODE_LABELS_INITIALIZE();
			CODE_LABEL( "start thread" , name , 10 );
			::rux::memory::check_memory( const_cast< Thread* >( this ) , 0 , sizeof( Thread ) );
			if( XInterlocked::CompareExchange( &_is_started , 1 , 0 ) == 0 )
			{
				_event->Reset( __FILE__ , __LINE__ );
				AddRef();
#ifdef __WINDOWS__
				for( ; ; )
				{
					_thread_handle = ( HANDLE )_beginthreadex( NULL , (unsigned int)_stack_size , rux_thread_function , this , CREATE_SUSPENDED , NULL );
					if( _thread_handle )
						break;
					else
						::rux::threading::XThread::Sleep( 1 );
				}
				if( _thread_handle )
				{
					::rux::engine::_globals->_rux_memory_add_descriptor( _rux_current_module_index , __FILE__ , _thread_handle );
					ResumeThread( _thread_handle );
					QueueUserAPC( EmptyFunction , _thread_handle , (ULONG_PTR)this );
					Sleep( 1 );
				}
#elif defined( __UNIX__ )
				for( ; ; )
				{
					pthread_attr_t pthread_attr;		
					::rux::int32 result = pthread_attr_init( &pthread_attr );		
					if( result == 0 )
					{
						result = pthread_attr_setstacksize( &pthread_attr , _stack_size );
						pthread_attr_setdetachstate( &pthread_attr , PTHREAD_CREATE_DETACHED );
						result = pthread_create( &_pthread , &pthread_attr , rux_thread_function , (void*)this );
					}
					else
						result = pthread_create( &_pthread , NULL , rux_thread_function , (void*)this );
					pthread_attr_destroy( &pthread_attr );
					if( result == 0 )
						break;
				}
#endif
			}
			else
			{
				while( _thread_id == 0 && XInterlocked::CompareExchange( &_is_started , 1 , 1 ) == 1 )
					::rux::threading::XThread::Sleep( 1 );
			}
		};
		void XThread::Wait( void )
		{
			(*this)()->Wait();
		};
		void Thread::Wait( void )
		{
			::rux::pid_t gui_message_processing_thread_id = 0;
			if( ::rux::engine::_globals && ::rux::engine::_globals->_gui_globals->_rux_gui_get_message_processing_thread_id )
				gui_message_processing_thread_id = ::rux::engine::_globals->_gui_globals->_rux_gui_get_message_processing_thread_id();
			if( XInterlocked::CompareExchange( &_is_started , 1 , 1 ) == 1 )
			{
				while( _thread_id == 0 && XInterlocked::CompareExchange( &_is_started , 1 , 1 ) == 1 )
					::rux::threading::XThread::Sleep( 1 );
#ifdef __WINDOWS__
				if( _thread_id != GetCurrentThreadId() )
#elif defined( __UNIX__	)
				if( _pthread != pthread_self() )
#endif
				{
					char name[ 128 ] = {0};
					snprintf( name , 128 , "[%u][0x%x] %s" , (::rux::uint32)_thread_id , (::rux::uint32)_thread_id , _thread_name );
					CODE_LABELS_INITIALIZE();
					CODE_LABEL( "wait for thread" , name , 100 );
					while( XInterlocked::CompareExchange( &_is_started , 0 , 0 ) != 0 )
					{	
						if( gui_message_processing_thread_id == ::rux::threading::XThread::get_CurrentThreadId() )
							::rux::engine::_globals->_gui_globals->_rux_gui_pump_message( ::rux::engine::_globals->_get_thread_index( _rux_current_module_index ) );
						::rux::threading::XThread::Sleep( 1 );
					}
				}
			}
		};
		void Thread::Stop( void )
		{			
			::rux::pid_t gui_message_processing_thread_id = 0;
			if( ::rux::engine::_globals && ::rux::engine::_globals->_gui_globals->_rux_gui_get_message_processing_thread_id )
				gui_message_processing_thread_id = ::rux::engine::_globals->_gui_globals->_rux_gui_get_message_processing_thread_id();
			if( XInterlocked::CompareExchange( &_is_started , 1 , 1 ) == 1 )
			{		
				while( _thread_id == 0 && XInterlocked::CompareExchange( &_is_started , 1 , 1 ) == 1 )
					::rux::threading::XThread::Sleep( 1 );
				_event->Set( __FILE__ , __LINE__ );
#ifdef __WINDOWS__
        if(_thread_handle)
				  QueueUserAPC( EmptyFunction , _thread_handle , (ULONG_PTR)this );
				Sleep( 1 );
				if( _thread_id != GetCurrentThreadId() )
#elif defined( __UNIX__ )
				_wait_event->Set( __FILE__ , __LINE__ );
				if( _pthread != pthread_self() )
#endif
				{
					char name[ 128 ] = {0};
					snprintf( name , 128 , "[%u][0x%x] %s" , (::rux::uint32)_thread_id , (::rux::uint32)_thread_id , _thread_name );
					CODE_LABELS_INITIALIZE();
					CODE_LABEL( "stop thread" , name , 100 );
					while( XInterlocked::CompareExchange( &_is_started , 0 , 0 ) != 0 )
					{						
						if( gui_message_processing_thread_id == ::rux::threading::XThread::get_CurrentThreadId() )
							::rux::engine::_globals->_gui_globals->_rux_gui_pump_message( ::rux::engine::_globals->_get_thread_index( _rux_current_module_index ) );
						::rux::threading::XThread::Sleep( 1 );
					}
				}
				_function_execution_last_time = 0;
			}
		};
		void Thread::set_ThreadMode( XThreadMode xtmMode )
		{
			_mode = xtmMode;
		};
		void Thread::set_Timeout( ::rux::uint32 timeout_in_milliseconds )
		{
			_repeat_timeout = timeout_in_milliseconds;
		};
#ifdef __WINDOWS__
#ifdef __x64__
		void __stdcall Thread::queue_user_apc_function( ::rux::uint64 param )
#elif defined( __x86__ )
		void __stdcall Thread::queue_user_apc_function( unsigned long param )
#endif
		{
			XUserData3* user_data3 = (XUserData3*)param;
			if( user_data3 )
			{
				Thread* thread_ptr = (Thread*)user_data3->_udata2;
				XInterlocked::Exchange( &thread_ptr->_is_busy , 1 );
				if( thread_ptr->_thread_id == 0 )
				{
					thread_ptr->_thread_id = XThread::get_CurrentThreadId();
					thread_ptr->_thread_index = ::rux::engine::_globals->_rux_add_thread_to_global( thread_ptr , _rux_current_module_index );
				}
				thread_ptr->_function_execution_last_time = ::rux::XTime::GetTickCount();
				::rux::engine::_globals->_rux_memory_add_label( thread_ptr->_thread_index , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , _rux_current_module_index , 0 , 100 );
				( (XTHREADFUNC)user_data3->_udata0)( user_data3->_udata1 , thread_ptr->_thread_index );
				::rux::engine::_globals->_rux_gc_thread_collect( thread_ptr->_thread_index );
				::rux::engine::_globals->_rux_memory_remove_label( thread_ptr->_thread_index );				
				XInterlocked::Exchange( &thread_ptr->_is_busy , 0 );
				::rux::engine::free_object< XUserData3 >( user_data3 );
			}
		};
#ifdef __x64__
		void __stdcall Thread::EmptyFunction( ::rux::uint64 param )
#elif defined( __x86__ )
		void __stdcall Thread::EmptyFunction( unsigned long param )
#endif
		{
			Thread* thread_ptr = (Thread*)param;
			if( thread_ptr->_thread_id == 0 )
			{
				thread_ptr->_thread_id = XThread::get_CurrentThreadId();
				thread_ptr->_thread_index = ::rux::engine::_globals->_rux_add_thread_to_global( thread_ptr , _rux_current_module_index );		
			}
		};
#endif
#ifdef __WINDOWS__
		::rux::uint32 __stdcall Thread::rux_thread_function( void* param )
#elif defined( __LINUX__ )
		void* Thread::rux_thread_function( void* param )
#elif defined( __SOLARIS__ )
		void* rux_thread_function( void* param )
#endif
		{	
#ifdef __UNIX__
			pthread_detach( pthread_self() );
#endif
			rux_tzset();
			Thread* thread_ptr = (Thread*)param;
			if( thread_ptr->_thread_id == 0 )
			{
				thread_ptr->_thread_id = XThread::get_CurrentThreadId();
				thread_ptr->_thread_index = ::rux::engine::_globals->_rux_add_thread_to_global( thread_ptr , _rux_current_module_index );
			}
#ifdef __WINDOWS__			
			struct
			{
				::rux::uint32 _type;
				const char* _name;
				::rux::uint32 _thread_id;
				::rux::uint32 _flags;		
			}info = { 0x1000 , thread_ptr->_thread_name , thread_ptr->_thread_id , 0 }; 
			__try
			{
#ifdef __x64__
				RaiseException( 0x406d1388 , 0 , sizeof( info ) / sizeof( ::rux::uint32 ) , (::rux::uint64*)&info );
#elif defined( __x86__ )
				RaiseException( 0x406d1388 , 0 , sizeof( info ) / sizeof( unsigned long ) , (unsigned long*)&info );
#endif
			}
			__except( EXCEPTION_CONTINUE_EXECUTION )
			{
			};
#elif defined( __UNIX__ )
#ifdef __LINUX__	
			prctl( PR_SET_NAME , thread_ptr->_thread_name );
#endif
#endif
			::rux::log::WriteError( "<thread>[%u][%x]'%s' started, %u bytes" 
				, (::rux::uint32)thread_ptr->_thread_id, (::rux::uint32)thread_ptr->_thread_id, thread_ptr->_thread_name 
				, (::rux::uint32)thread_ptr->_stack_size );
			XTHREADFUNC local_func = 0;
			void* local_param = 0;
			while( thread_ptr->_event->IsSet( __FILE__ , __LINE__ ) == 0 )
			{
				rux::uint32 now_tickcount = ::rux::XTime::GetTickCount();
				if( thread_ptr->_function_execution_last_time > now_tickcount )
					thread_ptr->_function_execution_last_time = now_tickcount;
#ifdef __WINDOWS__				
				if( thread_ptr->_event->IsSet( __FILE__ , __LINE__ ) == 0 && now_tickcount - thread_ptr->_function_execution_last_time < thread_ptr->_repeat_timeout )
					SleepEx( thread_ptr->_repeat_timeout - ( now_tickcount - thread_ptr->_function_execution_last_time ) , true );
				if( thread_ptr->_mode == XThreadMode_Repeat )
				{
					WRITE_LOCK( thread_ptr->_cs_functions );
					local_func = thread_ptr->_repeat_function;
					local_param = thread_ptr->_repeat_param;
					thread_ptr->_cs_functions.WriteUnlock();
					if( thread_ptr->_event->IsSet( __FILE__ , __LINE__ ) == 0 && local_func )
					{
						thread_ptr->_function_execution_last_time = now_tickcount;
						XInterlocked::Exchange( &thread_ptr->_is_busy , 1 );
						::rux::engine::_globals->_rux_memory_add_label( thread_ptr->_thread_index , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , _rux_current_module_index , 0 , 100 );
						local_func( local_param , thread_ptr->_thread_index );
						::rux::engine::_globals->_rux_gc_thread_collect( thread_ptr->_thread_index );
						::rux::engine::_globals->_rux_memory_remove_label( thread_ptr->_thread_index );
						XInterlocked::Exchange( &thread_ptr->_is_busy , 0 );
					}
				}		
				if( thread_ptr->_event->IsSet( __FILE__ , __LINE__ ) == 0 )
					SleepEx( thread_ptr->_repeat_timeout , true );
#elif defined( __UNIX__ )
				if( thread_ptr->_mode == XThreadMode_Repeat )
				{
					if( thread_ptr->_event->IsSet( __FILE__ , __LINE__ ) == 0 && now_tickcount - thread_ptr->_function_execution_last_time < thread_ptr->_repeat_timeout )
					{
						thread_ptr->_wait_event->Wait( thread_ptr->_repeat_timeout - ( now_tickcount - thread_ptr->_function_execution_last_time ) , NULL , NULL , 0 , 0 );	
						thread_ptr->_wait_event->Reset( __FILE__ , __LINE__ );			
					}
					WRITE_LOCK( thread_ptr->_cs_functions );
					if( thread_ptr->_event->IsSet( __FILE__ , __LINE__ ) == 0 && thread_ptr->_functions.Count() > 0 )
					{
						local_func = (XTHREADFUNC)thread_ptr->_functions[ 0 ];
						local_param = thread_ptr->_functions_params[ 0 ];
						thread_ptr->_cs_functions.WriteUnlock();
						thread_ptr->_function_execution_last_time = now_tickcount;
						XInterlocked::Exchange( &thread_ptr->_is_busy , 1 );
						::rux::engine::_globals->_rux_memory_add_label( thread_ptr->_thread_index , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , _rux_current_module_index , 0 , 100 );
						local_func( local_param , thread_ptr->_thread_index );
						::rux::engine::_globals->_rux_gc_thread_collect( thread_ptr->_thread_index );
						::rux::engine::_globals->_rux_memory_remove_label( thread_ptr->_thread_index );
						XInterlocked::Exchange( &thread_ptr->_is_busy , 0 );
						WRITE_LOCK( thread_ptr->_cs_functions );
					}
					thread_ptr->_cs_functions.WriteUnlock();
					if( thread_ptr->_event->IsSet( __FILE__ , __LINE__ ) == 0 )
						thread_ptr->_wait_event->Wait( thread_ptr->_repeat_timeout , NULL , NULL , 0 , 0 );
					thread_ptr->_wait_event->Reset( __FILE__ , __LINE__ );			
				}
				else if( thread_ptr->_mode == XThreadMode_OneTime )
				{			
					WRITE_LOCK( thread_ptr->_cs_functions );
					while( thread_ptr->_functions.Count() > 0 )
					{
						local_func = (XTHREADFUNC)thread_ptr->_functions[ 0 ];
						local_param = thread_ptr->_functions_params[ 0 ];
						thread_ptr->_functions.RemoveAt( 0 );
						thread_ptr->_functions_params.RemoveAt( 0 );
						thread_ptr->_cs_functions.WriteUnlock();
						XInterlocked::Exchange( &thread_ptr->_is_busy , 1 );
						::rux::engine::_globals->_rux_memory_add_label( thread_ptr->_thread_index , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , _rux_current_module_index , 0 , 100 );
						local_func( local_param , thread_ptr->_thread_index );
						::rux::engine::_globals->_rux_gc_thread_collect( thread_ptr->_thread_index );
						::rux::engine::_globals->_rux_memory_remove_label( thread_ptr->_thread_index );				
						XInterlocked::Exchange( &thread_ptr->_is_busy , 0 );
						WRITE_LOCK( thread_ptr->_cs_functions );
					}
					thread_ptr->_cs_functions.WriteUnlock();
					if( thread_ptr->_event->IsSet( __FILE__ , __LINE__ ) == 0 )
						thread_ptr->_wait_event->Wait( ::rux::threading::infinite , NULL , NULL , 0 , 0 );			
					thread_ptr->_wait_event->Reset( __FILE__ , __LINE__ );
				}
#endif		
			}
#ifdef __WINDOWS__
			CloseHandle( thread_ptr->_thread_handle );
			::rux::engine::_globals->_rux_memory_remove_descriptor( thread_ptr->_thread_handle , _rux_current_module_index );
			thread_ptr->_thread_handle = NULL;
#endif
			::rux::memory::check_memory( thread_ptr , 0 , sizeof( Thread ) );
			::rux::memory::check_memory( thread_ptr->_event , 0 , sizeof( Event ) );
#ifdef __UNIX__
			::rux::memory::check_memory( thread_ptr->_wait_event , 0 , sizeof( Event ) );
#endif
			::rux::engine::_globals->_rux_remove_thread_from_global( thread_ptr->_thread_index );
			if( thread_ptr->_thread_before_exit )
				thread_ptr->_thread_before_exit( thread_ptr->_thread_before_exit_user_data );

			::rux::log::WriteError("<thread>[%u][%x]'%s' stopped, %u bytes"
				, (::rux::uint32)thread_ptr->_thread_id, (::rux::uint32)thread_ptr->_thread_id, thread_ptr->_thread_name 
				, (::rux::uint32)thread_ptr->_stack_size);

			thread_ptr->_thread_id = 0;
			XInterlocked::CompareExchange( &thread_ptr->_is_started , 0 , 1 );
			thread_ptr->Release();		
			return 0;
		};
		void Thread::Push( XTHREADFUNC func , void* param )
		{
			::rux::memory::check_memory( const_cast< Thread* >( this ) , 0 , sizeof( Thread ) );
			if( XInterlocked::CompareExchange( &_is_started , 1 , 1 ) == 1 )
			{
				while( _thread_id == 0 && XInterlocked::CompareExchange( &_is_started , 1 , 1 ) == 1 )
					::rux::threading::XThread::Sleep( 1 );
#ifdef __WINDOWS__			
				if( _mode == XThreadMode_OneTime )
				{
					XUserData3* user_data3 = ::rux::engine::alloc_object< XUserData3 >();
					user_data3->_udata0 = func;
					user_data3->_udata1 = param;
					user_data3->_udata2 = this;
					QueueUserAPC( queue_user_apc_function , _thread_handle , (ULONG_PTR)user_data3 );
					Sleep( 1 );
				}
				else if( _mode == XThreadMode_Repeat )
				{
					_function_execution_last_time = 0;
					WRITE_LOCK( _cs_functions );
					_repeat_function = func;
					_repeat_param = param;
					_cs_functions.WriteUnlock();
					QueueUserAPC( EmptyFunction , _thread_handle , (ULONG_PTR)this );
					Sleep( 1 );
				}
#elif defined( __UNIX__ )
				if( _mode == XThreadMode_OneTime )
				{
					if( func )
					{
						WRITE_LOCK( _cs_functions );
						_functions.Add( (void*)func );
						_functions_params.Add( param );				
						_cs_functions.WriteUnlock();
					}			
					_wait_event->Set( __FILE__ , __LINE__ );			
				}
				else if( _mode == XThreadMode_Repeat )
				{
					if( func )
					{
						_function_execution_last_time = 0;
						WRITE_LOCK( _cs_functions );
						_functions.Clear();
						_functions_params.Clear();
						_functions.Add( (void*)func );
						_functions_params.Add( param );
						_cs_functions.WriteUnlock();
					}
					_wait_event->Set( __FILE__ , __LINE__ );
				}
#endif
			}
		};
		void XThread::Start( void )
		{
			(*this)()->Start();
		};
		void XThread::Stop( void )
		{
			(*this)()->Stop();
		};
		void XThread::Push( XTHREADFUNC func , void* param )
		{
			(*this)()->Push( func , param );
		};
		void XThread::set_ThreadMode( XThreadMode xtmMode )
		{
			(*this)()->set_ThreadMode( xtmMode );
		};
		void XThread::set_Timeout( ::rux::uint32 timeout_in_milliseconds )
		{
			(*this)()->set_Timeout( timeout_in_milliseconds );
		};	
		void XThread::Sleep( ::rux::uint32 milliseconds )
		{
#ifdef __WINDOWS__
			::Sleep( milliseconds );
#elif defined( __UNIX__ )
			struct timespec ts = { 0 };	
			struct timespec rem = { 0 };
			ts.tv_sec = milliseconds / 1000;
			ts.tv_nsec = ( milliseconds % 1000 ) * 1000000;
			nanosleep( &ts , &rem );
#endif
		};
		::rux::uint8 XThread::get_IsStarted( void )
		{
			return (*this)()->get_IsStarted();
		};
		::rux::uint8 Thread::get_IsStarted( void )
		{
			while( _thread_id == 0 && XInterlocked::CompareExchange( &_is_started , 1 , 1 ) == 1 )
				::rux::threading::XThread::Sleep( 1 );
			return XInterlocked::CompareExchange( &_is_started , 1 , 1 );
		};
		void XThread::set_ThreadName( const char* thread_name_ptr )
		{
			(*this)()->set_ThreadName( thread_name_ptr );
		};
		void Thread::set_ThreadName( const char* thread_name_ptr )
		{
			::rux::safe_strncpy( _thread_name , thread_name_ptr , 16 );
		};
		::rux::pid_t XThread::get_CurrentThreadId( void )
		{
#ifdef __WINDOWS__
			return GetCurrentThreadId();
#else
			return rux_gettid();
#endif
		};
		::rux::uint8 Thread::get_IsWaitForExit( void )
		{
			return _event->IsSet( __FILE__ , __LINE__ );
		};
		::rux::uint8 XThread::get_IsWaitForExit( void )
		{
			return (*this)()->get_IsWaitForExit();
		};
		Array< XThread >& XThread::get_Threads( void )
		{
			XArray< XThread > threads_array;
			XMallocArray< XGCRef* >* threads = ::rux::engine::_globals->_rux_get_all_threads();
			if( threads )
			{
				for( size_t index = 0 ; index < threads->Count() ; index++ )
					threads_array.Add( *( (Thread*)threads->operator[]( index ) ) );
				::rux::engine::_globals->_rux_free_all_threads( threads );
			}
			return threads_array++;
		};
		Thread& XThread::get_CurrentThread( void )
		{
			::rux::uint32 current_thread_id = get_CurrentThreadId();
			::rux::threading::XThread thread;
			XMallocArray< XGCRef* >* threads = ::rux::engine::_globals->_rux_get_all_threads();
			if( threads )
			{
				for( size_t index = 0 ; index < threads->Count() ; index++ )
				{
					Thread* local_thread = (Thread*)threads->operator[]( index );
					if( local_thread->_thread_id == current_thread_id )
					{
						thread = *local_thread;
						break;
					}			
				}
				::rux::engine::_globals->_rux_free_all_threads( threads );
			}
			return thread++;
		};
		bool Thread::get_IsBusy( void )
		{
			return XInterlocked::CompareExchange( &_is_busy , 0 , 0 ) == 1;
		};
		bool XThread::get_IsBusy( void )
		{
			return (*this)()->get_IsBusy();
		};
	};
};