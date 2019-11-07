#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_event.h"
#include "xan_console.h"
#include "xan_keyvaluepair.h"
#include "xan_uint64.h"
#include "xan_boolean.h"
#include "xan_thread.h"
#include "xan_log.h"
#include "xan_time.h"
begin_implement_rux_class_ns( Event , rux::threading );
#if OLD_RUX_EVENT
#ifdef __WINDOWS__
	for( ; ; )
	{
		_event_handle = CreateEventA( NULL , TRUE , FALSE , NULL );
		if( _event_handle )
			break;
		else
			::rux::threading::XThread::Sleep( 1 );
	}
	::rux::engine::_globals->_rux_memory_add_descriptor( _rux_current_module_index , __FILE__ , _event_handle );
#else
	pthread_condattr_t condattr;
	while( pthread_condattr_init( &condattr ) != 0 )
		::rux::threading::XThread::Sleep( 1 );
	_clockid = CLOCK_MONOTONIC;
	if( pthread_condattr_setclock( &condattr , _clockid ) != 0 )
	{
		_clockid = CLOCK_REALTIME;
		pthread_condattr_setclock( &condattr , _clockid );
	}
	::rux::int32 res = 0;
	for( ; ; )
	{
		res = pthread_cond_init( &_pthread_cond , &condattr );
		if( res != 0 )
			res = pthread_cond_init( &_pthread_cond , NULL );
		if( res == 0 )
			break;
		else
			::rux::threading::XThread::Sleep( 1 );
	}
	while( pthread_mutex_init( &_pthread_mutex , NULL ) != 0 )
		::rux::threading::XThread::Sleep( 1 );
	_is_set = 0;
	pthread_condattr_destroy( &condattr );
#endif
#else
	_is_set = 0;
#endif
end_implement_rux_class();
namespace rux
{
	namespace threading
	{		
		Event::~Event()
		{
#if OLD_RUX_EVENT
		#ifdef __WINDOWS__
			CloseHandle( _event_handle );
			::rux::engine::_globals->_rux_memory_remove_descriptor( _event_handle , _rux_current_module_index );
			_event_handle = NULL;
		#else
			pthread_cond_destroy( &_pthread_cond );
			pthread_mutex_destroy( &_pthread_mutex );
		#endif
#endif
		};
		void Event::Set( const char* file , ::rux::int32 line )
		{
			if( _rux_is_memory_manager == 0 && ::rux::engine::_globals && ::rux::engine::_globals->_log_level == ::rux::log::XEnum_LogLevel_Verbose )
				::rux::log::WriteTrace( "%s, %s:%d, set event %p" , __FUNCTION__ , file ? file : __FILE__ , line ? line : __LINE__ , this );
			char name[ 128 ] = {0};
			snprintf( name , 128 , "%p" , this );
			CODE_LABELS_INITIALIZE();
			CODE_LABEL( "set event" , name , 10 );
			::rux::memory::check_memory( const_cast< Event* >( this ) , 0 , sizeof( Event ) );
#if OLD_RUX_EVENT
		#ifdef __WINDOWS__
			SetEvent( _event_handle );
		#else
			pthread_mutex_lock( &_pthread_mutex );
			if( _is_set == 0 )
			{
				pthread_cond_broadcast( &_pthread_cond );
				_is_set = 1;
			}
			pthread_mutex_unlock( &_pthread_mutex );	
		#endif
#else
			XInterlocked::Exchange( &_is_set , 1 );
#endif
		};
		void Event::Reset( const char* file , ::rux::int32 line )
		{
			if( _rux_is_memory_manager == 0 && ::rux::engine::_globals && ::rux::engine::_globals->_log_level == ::rux::log::XEnum_LogLevel_Verbose )
				::rux::log::WriteTrace( "%s, %s:%d, reset event %p" , __FUNCTION__ , file ? file : __FILE__ , line ? line : __LINE__ , this );
			char name[ 128 ] = {0};
			snprintf( name , 128 , "%p" , this );
			CODE_LABELS_INITIALIZE();
			CODE_LABEL( "reset event" , name , 10 );
			::rux::memory::check_memory( const_cast< Event* >( this ) , 0 , sizeof( Event ) );
#if OLD_RUX_EVENT
		#ifdef __WINDOWS__
			ResetEvent( _event_handle );
		#else
			pthread_mutex_lock( &_pthread_mutex );
			_is_set = 0;
			pthread_mutex_unlock( &_pthread_mutex );
		#endif
#else
			XInterlocked::Exchange( &_is_set , 0 );
#endif
		};
		::rux::uint8 Event::IsSet( const char* file , ::rux::int32 line )
		{
			if( _rux_is_memory_manager == 0 && ::rux::engine::_globals && ::rux::engine::_globals->_log_level == ::rux::log::XEnum_LogLevel_Verbose )
				::rux::log::WriteTrace( "%s, %s:%d, is set event %p" , __FUNCTION__ , file ? file : __FILE__ , line ? line : __LINE__ , this );
			char name[ 128 ] = {0};
			snprintf( name , 128 , "%p" , this );
			CODE_LABELS_INITIALIZE();
			CODE_LABEL( "is set event" , name , 10 );
			::rux::memory::check_memory( const_cast< Event* >( this ) , 0 , sizeof( Event ) );
#if OLD_RUX_EVENT
			::rux::uint8 is_set = 0;
		#ifdef __WINDOWS__
			is_set = WaitForSingleObject( _event_handle , 0 ) == WAIT_OBJECT_0 ? 1 : 0;	
		#else
			pthread_mutex_lock( &_pthread_mutex );
			is_set = _is_set;
			pthread_mutex_unlock( &_pthread_mutex );
		#endif
			return is_set;
#else
			return (::rux::uint8)XInterlocked::CompareExchange( &_is_set , 0 , 0 );
#endif
		};
		::rux::uint8 Event::Wait( ::rux::uint32 milliseconds , const char* file , const char* function , ::rux::int32 line , ::rux::uint8 debug )
		{	
			::rux::memory::check_memory( const_cast< Event* >( this ) , 0 , sizeof( Event ) );
			::rux::byte diagnostic = 0;
			if( ::rux::engine::_globals && ::rux::engine::_globals->_rux_memory_add_label )
				diagnostic = ::rux::engine::_globals->_diagnostic;
			size_t ___rux__thread_index1986 = SIZE_MAX;
			if( debug == 1 && diagnostic )
			{
				___rux__thread_index1986 = ::rux::engine::_globals->_get_thread_index( _rux_current_module_index );
				if( file == NULL || function == NULL )
					::rux::engine::_globals->_rux_memory_add_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , file , function , _rux_current_module_index , 1 , 100 );	
				else
					::rux::engine::_globals->_rux_memory_add_label( ___rux__thread_index1986 , file , function , line , __FILE__ , __FUNCTION__ , _rux_current_module_index , 1 , 100 );	
			}
			else
				diagnostic = 0;
			::rux::uint8 is_set = 1;
#if OLD_RUX_EVENT
		#ifdef __WINDOWS__
			is_set = WaitForSingleObject( _event_handle , milliseconds ) == WAIT_OBJECT_0 ? 1 : 0;
		#else
			if( milliseconds == ::rux::threading::infinite )
			{
				pthread_mutex_lock( &_pthread_mutex );
				while( _is_set == 0 )
					pthread_cond_wait( &_pthread_cond , &_pthread_mutex );		
				pthread_mutex_unlock( &_pthread_mutex );
			}
			else
			{		
				pthread_mutex_lock( &_pthread_mutex );
				if( _is_set == 0 )
				{
					struct timespec ts = { 0 };			
					clock_gettime( _clockid , &ts );			
					ts.tv_sec += milliseconds / 1000;
					ts.tv_nsec += ( milliseconds % 1000 ) * 1000000;
					if( ts.tv_nsec > 999999999 )
					{
						ts.tv_sec += 1;
						ts.tv_nsec -= 1000000000;
					}
					::rux::int32 res = pthread_cond_timedwait( &_pthread_cond , &_pthread_mutex , &ts );			
					is_set = res == 0 ? 1 : 0;
				}
				else
					is_set = 1;		
				pthread_mutex_unlock( &_pthread_mutex );		
			}
		#endif
#else
			if( milliseconds == ::rux::threading::infinite )
			{
				while( XInterlocked::CompareExchange( &_is_set , 0 , 0 ) == 0 )
					::rux::threading::XThread::Sleep( 1 );
			}
			else
			{
				rux::uint32 start_time = ::rux::XTime::GetTickCount() , now = 0;
				while( ( is_set = (::rux::uint8)XInterlocked::CompareExchange( &_is_set , 0 , 0 ) ) == 0 )
				{
					::rux::threading::XThread::Sleep( 1 );
					now = ::rux::XTime::GetTickCount();
					if( start_time > now )
					{
						::rux::uint32 difference = ( UINT_MAX - start_time ) + now;
						start_time = now;
						if( milliseconds > difference )
							milliseconds -= difference;
						else
							milliseconds = 0;
					}
					if( now - start_time >= milliseconds )
						break;
				}
			}
#endif
			if( diagnostic )
				::rux::engine::_globals->_rux_memory_remove_label( ___rux__thread_index1986 );	
			return is_set;
		};
		bool XEvent::operator ==( const XEvent& event)
		{
			return (*this)() == event();
		};
		void XEvent::Set( const char* file , ::rux::int32 line )
		{
			(*this)()->Set( file , line );
		};
		void XEvent::Reset( const char* file , ::rux::int32 line )
		{
			(*this)()->Reset( file , line );
		};
		::rux::uint8 XEvent::IsSet( const char* file , ::rux::int32 line )
		{
			return (*this)()->IsSet( file , line );
		};
		::rux::uint8 XEvent::Wait( ::rux::uint32 milliseconds , const char* file , const char* function , ::rux::int32 line , ::rux::uint8 debug )
		{
			return (*this)()->Wait( milliseconds , file , function , line , debug );
		};
	};
};