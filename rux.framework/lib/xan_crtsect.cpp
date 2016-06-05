#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_crtsect.h>
#include <xan_interlocked.h>
#include <xan_thread.h>
#include <xan_global_initialization.h>
#include <xan_gui_defines.h>
#include <xan_global_variables.h>
XCrtSect::XCrtSect()
{
	_crt_sect_index = SIZE_MAX;
	snprintf( _name , 24 , "%p" , (void*)this );
#ifdef __WINDOWS__
	_is_spin_lock = 1;
	if( _is_spin_lock == 1 )
		InitializeCriticalSectionAndSpinCount( &_cs , 1000 );
	else
		InitializeCriticalSection( &_cs );
#endif
#ifdef __UNIX__
	_is_spin_lock = 0;
#ifndef __ANDROID__
	if( _is_spin_lock == 1 )
		pthread_spin_init( &_pthread_spin , PTHREAD_PROCESS_PRIVATE );
	else
#endif
	{
		pthread_mutexattr_init( &_pthread_mutexattr );
		pthread_mutexattr_settype( &_pthread_mutexattr , PTHREAD_MUTEX_RECURSIVE );
		pthread_mutex_init( &_pthread_mutex , &_pthread_mutexattr );
	}
#endif
 };
XCrtSect::~XCrtSect()
{
	if( _crt_sect_index != SIZE_MAX )
	{
		if( ::rux::engine::_globals && ::rux::engine::_globals->_remove_locked_crt_sect )
			::rux::engine::_globals->_remove_locked_crt_sect( SIZE_MAX , this , _crt_sect_index , 1 );
	}
#ifdef __WINDOWS__	
	DeleteCriticalSection( &_cs );
#endif
#ifdef __UNIX__
#ifndef __ANDROID__
	if( _is_spin_lock == 1 )
		pthread_spin_destroy( &_pthread_spin );
	else
#endif
		pthread_mutex_destroy( &_pthread_mutex );
#endif
};	
void XCrtSect::Lock( ::rux::uint8 is_code_block , const char* file , const char* function , ::rux::int32 line , ::rux::byte rux_gui_pump_message )
{
	size_t ___rux__thread_index1986 = SIZE_MAX;
	if( ::rux::engine::_globals && ::rux::engine::_globals->_get_thread_index )
		___rux__thread_index1986 = ::rux::engine::_globals->_get_thread_index( _rux_current_module_index );
	::rux::byte diagnostic = 0;
	if( ::rux::engine::_globals && ::rux::engine::_globals->_rux_memory_add_label )
		diagnostic = ::rux::engine::_globals->_diagnostic;
	if( is_code_block == 1 && diagnostic )
	{
		if( file == NULL || function == NULL )
			::rux::engine::_globals->_rux_memory_add_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , "XCrtSect:Lock" , _name , _rux_current_module_index , 1 , 100 );	
		else
			::rux::engine::_globals->_rux_memory_add_label( ___rux__thread_index1986 , file , function , line , "XCrtSect:Lock" , _name , _rux_current_module_index , 1 , 100 );	
	}
	else
		diagnostic = 0;
	::rux::pid_t gui_message_processing_thread_id = 0;
	if( rux_gui_pump_message && ::rux::engine::_globals && ::rux::engine::_globals->_gui_globals->_rux_gui_get_message_processing_thread_id )
		gui_message_processing_thread_id = ::rux::engine::_globals->_gui_globals->_rux_gui_get_message_processing_thread_id();
	if( gui_message_processing_thread_id == 0
		|| gui_message_processing_thread_id != ::rux::threading::XThread::get_CurrentThreadId() )
	{
#ifdef __WINDOWS__
		EnterCriticalSection( &_cs );	
#elif defined( __UNIX__ )
#ifndef __ANDROID__
		if( _is_spin_lock == 1 )
			pthread_spin_lock( &_pthread_spin );
		else
#endif
			pthread_mutex_lock( &_pthread_mutex );
#endif
		if( ::rux::engine::_globals && ::rux::engine::_globals->_add_locked_crt_sect )
			::rux::engine::_globals->_add_locked_crt_sect( ___rux__thread_index1986 , this , _crt_sect_index , file , line , _rux_current_module_index );
	}
	else
	{
		while( TryLock( 0 , file , function , line ) == false )
			::rux::engine::_globals->_gui_globals->_rux_gui_pump_message( ___rux__thread_index1986 );
	}
	if( diagnostic )
		::rux::engine::_globals->_rux_memory_remove_label( ___rux__thread_index1986 );
};
void XCrtSect::UnLock( ::rux::uint8 is_code_block , const char* file , const char* function , ::rux::int32 line )
{
	size_t ___rux__thread_index1986 = SIZE_MAX;
	if( ::rux::engine::_globals && ::rux::engine::_globals->_get_thread_index )
		___rux__thread_index1986 = ::rux::engine::_globals->_get_thread_index( _rux_current_module_index );
	::rux::byte diagnostic = 0;
	if( ::rux::engine::_globals && ::rux::engine::_globals->_rux_memory_add_label )
		diagnostic = ::rux::engine::_globals->_diagnostic;
	if( is_code_block == 1 && diagnostic )
	{
		if( file == NULL || function == NULL )
			::rux::engine::_globals->_rux_memory_add_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , "XCrtSect::UnLock" , _name , _rux_current_module_index , 1 , 100 );
		else
			::rux::engine::_globals->_rux_memory_add_label( ___rux__thread_index1986 , file , function , line , "XCrtSect::UnLock" , _name , _rux_current_module_index , 1 , 100 );
	}
	else
		diagnostic = 0;
#ifdef __WINDOWS__
	LeaveCriticalSection( &_cs );	
#elif defined( __UNIX__ )
#ifndef __ANDROID__
	if( _is_spin_lock == 1 )
		pthread_spin_unlock( &_pthread_spin );
	else
#endif
		pthread_mutex_unlock( &_pthread_mutex );
#endif
	if( diagnostic )
		::rux::engine::_globals->_rux_memory_remove_label( ___rux__thread_index1986 );
	if( ::rux::engine::_globals && ::rux::engine::_globals->_remove_locked_crt_sect )
		::rux::engine::_globals->_remove_locked_crt_sect( ___rux__thread_index1986 , this , _crt_sect_index , 0 );
};
bool XCrtSect::TryLock( ::rux::uint8 is_code_block , const char* file , const char* function , ::rux::int32 line )
{
	size_t ___rux__thread_index1986 = SIZE_MAX;
	if( ::rux::engine::_globals && ::rux::engine::_globals->_get_thread_index )
		___rux__thread_index1986 = ::rux::engine::_globals->_get_thread_index( _rux_current_module_index );
	::rux::byte diagnostic = 0;
	if( ::rux::engine::_globals && ::rux::engine::_globals->_rux_memory_add_label )
		diagnostic = ::rux::engine::_globals->_diagnostic;
	if( is_code_block == 1 && diagnostic )
	{
		if( file == NULL || function == NULL )
			::rux::engine::_globals->_rux_memory_add_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , "XCrtSect::TryLock" , _name , _rux_current_module_index , 1 , 100 );
		else
			::rux::engine::_globals->_rux_memory_add_label( ___rux__thread_index1986 , file , function , line , "XCrtSect::TryLock" , _name , _rux_current_module_index , 1 , 100 );
	}
	else
		diagnostic = 0;
#ifdef __WINDOWS__
	bool res = TryEnterCriticalSection( &_cs ) == 0 ? false : true;	
#elif defined( __UNIX__ )
	bool res = false;
#ifndef __ANDROID__
	if( _is_spin_lock == 1 )
		res = pthread_spin_trylock( &_pthread_spin ) == 0;
	else
#endif
		res = pthread_mutex_trylock( &_pthread_mutex ) == 0;
#endif
	if( diagnostic )
		::rux::engine::_globals->_rux_memory_remove_label( ___rux__thread_index1986 );
	if( res )
	{
		if( ::rux::engine::_globals && ::rux::engine::_globals->_add_locked_crt_sect )
			::rux::engine::_globals->_add_locked_crt_sect( ___rux__thread_index1986 , this , _crt_sect_index , file , line , _rux_current_module_index );
	}
	return res;
};
namespace rux
{
	namespace threading
	{
		RdWrLock::RdWrLock()
		: _writer_readers( 0 ) , _writer_recursion( 0 ) , _writer_thread( 0 )
		{		
			snprintf( _name , 24 , "%p" , (void*)this );
			_write_rdwrlock_index = SIZE_MAX;
			_read_rdwrlock_index = SIZE_MAX;
		};
		RdWrLock::~RdWrLock( void )
		{
			if( XInterlocked::CompareExchange( &_writer_readers , 0 , 0 ) != 0 )
			{
				if( _rux_is_memory_manager == 0 && ::rux::engine::_globals && ::rux::engine::_globals->_remove_read_locked_rdwrlock )
					::rux::engine::_globals->_remove_read_locked_rdwrlock( SIZE_MAX , this , _read_rdwrlock_index , 1 );
				if( _rux_is_memory_manager == 0 && ::rux::engine::_globals && ::rux::engine::_globals->_remove_write_locked_rdwrlock )
					::rux::engine::_globals->_remove_write_locked_rdwrlock( SIZE_MAX , this , _write_rdwrlock_index , 1 );
			}
		};
		void RdWrLock::ReadLock( const char* file , const char* function , ::rux::int32 line , ::rux::uint8 debug )
		{
			size_t ___rux__thread_index1986 = SIZE_MAX;
			if( _rux_is_memory_manager == 0 && ::rux::engine::_globals && ::rux::engine::_globals->_get_thread_index )
				___rux__thread_index1986 = ::rux::engine::_globals->_get_thread_index( _rux_current_module_index );
			if( debug )
			{
				::rux::byte diagnostic = 0;
				if( ::rux::engine::_globals && ::rux::engine::_globals->_rux_memory_add_label )
					diagnostic = ::rux::engine::_globals->_diagnostic;
				if( _rux_is_memory_manager == 0 && diagnostic )
				{
					if( file == NULL || function == NULL )
						::rux::engine::_globals->_rux_memory_add_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , "RdWrLock::ReadLock" , _name , _rux_current_module_index , 1 , 100 );	
					else
						::rux::engine::_globals->_rux_memory_add_label( ___rux__thread_index1986 , file , function , line , "RdWrLock::ReadLock" , _name , _rux_current_module_index , 1 , 100 );
				}
				else
					diagnostic = 0;
				if( XInterlocked::Increment( &_writer_readers ) >= WRITER_BIT )
				{
					if( _writer_thread != ::rux::threading::XThread::get_CurrentThreadId() )
					{
						::rux::int32 tries = 0;
						while( XInterlocked::CompareExchange( &_writer_readers , 0 , 0 ) >= WRITER_BIT )
						{
							tries++;
							if( tries == 5 )
							{
								::rux::threading::XThread::Sleep( 1 );
								tries = 0;
							}
						}
					}
				}
				if( diagnostic )
					::rux::engine::_globals->_rux_memory_remove_label( ___rux__thread_index1986 );
			}
			else
			{
				if( XInterlocked::Increment( &_writer_readers ) >= WRITER_BIT )
				{
					if( _writer_thread != ::rux::threading::XThread::get_CurrentThreadId() )
					{
						::rux::int32 tries = 0;
						while( XInterlocked::CompareExchange( &_writer_readers , 0 , 0 ) >= WRITER_BIT )
						{
							tries++;
							if( tries == 5 )
							{
								::rux::threading::XThread::Sleep( 1 );
								tries = 0;
							}
						}
					}
				}
			}
			if( _rux_is_memory_manager == 0 && ::rux::engine::_globals && ::rux::engine::_globals->_add_read_locked_rdwrlock )
				::rux::engine::_globals->_add_read_locked_rdwrlock( ___rux__thread_index1986 , this , _read_rdwrlock_index , file , line , _rux_current_module_index );
		};
		bool RdWrLock::TryWriteLock( const char* file , const char* function , ::rux::int32 line , ::rux::uint8 debug )
		{
			bool res = false;
			size_t ___rux__thread_index1986 = SIZE_MAX;
			if( _rux_is_memory_manager == 0 && ::rux::engine::_globals && ::rux::engine::_globals->_get_thread_index )
				___rux__thread_index1986 = ::rux::engine::_globals->_get_thread_index( _rux_current_module_index );
			if( debug )
			{
				::rux::byte diagnostic = 0;
				if( ::rux::engine::_globals && ::rux::engine::_globals->_rux_memory_add_label )
					diagnostic = ::rux::engine::_globals->_diagnostic;
				if( _rux_is_memory_manager == 0 && diagnostic )
				{
					if( file == NULL || function == NULL )
						::rux::engine::_globals->_rux_memory_add_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , "RdWrLock::TryWriteLock" , _name , _rux_current_module_index , 1 , 100 );	
					else
						::rux::engine::_globals->_rux_memory_add_label( ___rux__thread_index1986 , file , function , line , "RdWrLock::TryWriteLock" , _name , _rux_current_module_index , 1 , 100 );	
				}
				else
					diagnostic = 0;
				if( XInterlocked::CompareExchange( &_writer_readers , WRITER_BIT , 0 ) == 0 )
					res = true;
				if( diagnostic )
					::rux::engine::_globals->_rux_memory_remove_label( ___rux__thread_index1986 );
			}
			else
			{
				if( XInterlocked::CompareExchange( &_writer_readers , WRITER_BIT , 0 ) == 0 )
					res = true;
			}
			if( res )
			{
				XInterlocked::Increment( &_writer_recursion );
				_writer_thread = ::rux::threading::XThread::get_CurrentThreadId();
				if( _rux_is_memory_manager == 0 && ::rux::engine::_globals && ::rux::engine::_globals->_add_write_locked_rdwrlock )
					::rux::engine::_globals->_add_write_locked_rdwrlock( ___rux__thread_index1986 , this , _write_rdwrlock_index , file , line , _rux_current_module_index );
			}
			return res;
		};
		void RdWrLock::WriteLock( const char* file , const char* function , ::rux::int32 line , ::rux::uint8 debug )
		{
			size_t ___rux__thread_index1986 = SIZE_MAX;
			if( _rux_is_memory_manager == 0 && ::rux::engine::_globals && ::rux::engine::_globals->_get_thread_index )
				___rux__thread_index1986 = ::rux::engine::_globals->_get_thread_index( _rux_current_module_index );
			if( debug )
			{
				::rux::byte diagnostic = 0;
				if( ::rux::engine::_globals && ::rux::engine::_globals->_rux_memory_add_label )
					diagnostic = ::rux::engine::_globals->_diagnostic;
				if( _rux_is_memory_manager == 0 && diagnostic )
				{
					if( file == NULL || function == NULL )
						::rux::engine::_globals->_rux_memory_add_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , "RdWrLock::WriteLock" , _name , _rux_current_module_index , 1 , 100 );
					else
						::rux::engine::_globals->_rux_memory_add_label( ___rux__thread_index1986 , file , function , line , "RdWrLock::WriteLock" , _name , _rux_current_module_index , 1 , 100 );	
				}
				else
					diagnostic = 0;
				::rux::int32 tries = 0;
				while( XInterlocked::CompareExchange( &_writer_readers , WRITER_BIT , 0 ) != 0 )
				{
					if( _writer_thread != ::rux::threading::XThread::get_CurrentThreadId() )
					{
						tries++;
						if( tries == 5 )
						{
							::rux::threading::XThread::Sleep( 1 );
							tries = 0;
						}
					}
					else
						break;
				}	
				if( diagnostic )
					::rux::engine::_globals->_rux_memory_remove_label( ___rux__thread_index1986 );
			}
			else
			{
				::rux::int32 tries = 0;
				while( XInterlocked::CompareExchange( &_writer_readers , WRITER_BIT , 0 ) != 0 )
				{
					if( _writer_thread != ::rux::threading::XThread::get_CurrentThreadId() )
					{
						tries++;
						if( tries == 5 )
						{
							::rux::threading::XThread::Sleep( 1 );
							tries = 0;
						}
					}
					else
						break;
				}
			}
			XInterlocked::Increment( &_writer_recursion );
			_writer_thread = ::rux::threading::XThread::get_CurrentThreadId();
			if( _rux_is_memory_manager == 0 && ::rux::engine::_globals && ::rux::engine::_globals->_add_write_locked_rdwrlock )
				::rux::engine::_globals->_add_write_locked_rdwrlock( ___rux__thread_index1986 , this , _write_rdwrlock_index , file , line , _rux_current_module_index );
		};
		void RdWrLock::ReadUnlock( void )
		{
			XInterlocked::Decrement( &_writer_readers );
			if( _rux_is_memory_manager == 0 && ::rux::engine::_globals && ::rux::engine::_globals->_remove_read_locked_rdwrlock )
				::rux::engine::_globals->_remove_read_locked_rdwrlock( SIZE_MAX , this , _read_rdwrlock_index , 0 );
		};
		void RdWrLock::WriteUnlock( void )
		{
			if( _writer_thread == ::rux::threading::XThread::get_CurrentThreadId() )
			{
				if( XInterlocked::Decrement( &_writer_recursion ) == 0 )
				{
					_writer_thread = 0;
					XInterlocked::ExchangeAdd( &_writer_readers , -WRITER_BIT );
				}
				if( _rux_is_memory_manager == 0 && ::rux::engine::_globals && ::rux::engine::_globals->_remove_write_locked_rdwrlock )
					::rux::engine::_globals->_remove_write_locked_rdwrlock( SIZE_MAX , this , _write_rdwrlock_index , 0 );
			}
		};
	};
};