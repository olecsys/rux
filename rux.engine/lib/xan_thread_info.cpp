#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_thread_info.h>
#include <xan_memorymanager_defines.h>
#include <xan_memorymanager.h>
#include <xan_io_defines.h>
#include <xan_time.h>
#include <xan_log.h>
XThreadInfo::XThreadInfo()
{
	null();
};
XThreadInfo::~XThreadInfo( void )
{
	lock();
	unregister();
	unlock();
};
void XThreadInfo::null( void )
{
	_force_cpu = 0;
	_activity_time = ::rux::XTime::GetTickCount();
	_is_unknown = 0;
	_registered = 0;
	_locked_crt_sects_count = 0;
	_write_locked_rdwrlock_count = 0;
	_read_locked_rdwrlock_count = 0;
	memset( _write_locked_rdwrlocks , 0 , sizeof( XRdWrLockInfo ) * RUX_RDWRLOCK_INFO_COUNT );
	memset( _read_locked_rdwrlocks , 0 , sizeof( XRdWrLockInfo ) * RUX_RDWRLOCK_INFO_COUNT );
	memset( _locked_crt_sects , 0 , sizeof( XCrtSectInfo ) * RUX_CRTSECT_INFO_COUNT );
	memset( _gcrefs_to_clean , 0 , sizeof( XGCRef* ) * RUX_MAX_GC_COUNT );
	_gcrefs_to_clean_count = 0;
	_gcrefs_to_clean_index = 0;
	_gcref = NULL;
	_tid = 0;
	_thread_name[ 0 ] = 0;
	_count = 0;
	_gcrefs_to_clean_locked = 0;
	_module_index = 0;
	_count = 0;
};
void XThreadInfo::unregister( void )
{
	CollectGCRefs( 1 , (::rux::byte)0 );
	CollectModuleGCRefs( 0 );
	RemoveModuleLabels( 0 );
	if( _locked_crt_sects_count > 0 )
	{
		for( size_t index0 = 0 ; index0 < RUX_CRTSECT_INFO_COUNT ; index0++ )
		{
			if( _locked_crt_sects[ index0 ]._crt_sect )
			{
				_locked_crt_sects[ index0 ]._crt_sect = 0;
				_locked_crt_sects[ index0 ]._recursion = 0;
			}
		}
		_locked_crt_sects_count = 0;
	}
	if( _write_locked_rdwrlock_count > 0 )
	{
		for( size_t index0 = 0 ; index0 < RUX_RDWRLOCK_INFO_COUNT ; index0++ )
		{
			if( _write_locked_rdwrlocks[ index0 ]._rdwrlock )
			{
				_write_locked_rdwrlocks[ index0 ]._rdwrlock = 0;
				_write_locked_rdwrlocks[ index0 ]._recursion = 0;
			}
		}
		_write_locked_rdwrlock_count = 0;
	}
	if( _read_locked_rdwrlock_count > 0 )
	{
		for( size_t index0 = 0 ; index0 < RUX_RDWRLOCK_INFO_COUNT ; index0++ )
		{
			if( _read_locked_rdwrlocks[ index0 ]._rdwrlock )
			{
				_read_locked_rdwrlocks[ index0 ]._rdwrlock = 0;
				_read_locked_rdwrlocks[ index0 ]._recursion = 0;
			}
		}
		_read_locked_rdwrlock_count = 0;
	}
};
void XThreadInfo::lock( void )
{
	_cs.WriteLock( __FILE__ , __FUNCTION__ , __LINE__ , 0 );
};
void XThreadInfo::unlock( void )
{
	_cs.WriteUnlock();
};
void XThreadInfo::Add( const char* filename , const char* function , ::rux::int32 line , const char* other_module , const char* other_function , ::rux::int32 module_index , ::rux::uint8 check_delay , ::rux::uint32 max_delay , ::rux::pid_t* owner_tid )
{
	if( _count < RUX_MAX_LABELS_COUNT )
	{
		_filenames[ _count ] = filename;
		_functions[ _count ] = function;
		_lines[ _count ] = line;
		_modules[ _count ] = module_index;	
		_other_modules[ _count ] = other_module;
		_other_functions[ _count ] = other_function;
		_check_delay[ _count ] = check_delay;
		_max_delay[ _count ] = max_delay * 1000000ULL;
		_delta[ _count ] = 0ULL;
#ifdef __WINDOWS__
		LARGE_INTEGER qpc;
		if( QueryPerformanceCounter( &qpc ) )
			_performance[ _count ] = qpc.QuadPart;
#elif defined( __UNIX__ )
		struct timespec ts = { 0 };
		clock_gettime( CLOCK_MONOTONIC , &ts );
		_performance[ _count ] = ts.tv_sec * 1000000000ULL + ts.tv_nsec;
#endif
		_count++;
	}
};
void XThreadInfo::RemoveModuleLabels( ::rux::int32 module_index )
{
	while( _count > 0 
		&& _count <= RUX_MAX_LABELS_COUNT
		&& _modules[ _count - 1 ] == module_index )
		Remove();
};
void XThreadInfo::CollectCrtSectInfos( ::rux::int32 module_index )
{
	for( size_t index0 = RUX_CRTSECT_INFO_COUNT - 1 ; ; index0-- )
	{
		if( _locked_crt_sects[ index0 ]._crt_sect && _locked_crt_sects[ index0 ]._module_index == module_index )
		{
			_locked_crt_sects[ index0 ]._crt_sect = 0;
			_locked_crt_sects[ index0 ]._recursion = 0;
			_locked_crt_sects_count--;
		}
		if( index0 == 0 )
			break;
	}
	for( size_t index0 = RUX_RDWRLOCK_INFO_COUNT - 1 ; ; index0-- )
	{
		if( _write_locked_rdwrlocks[ index0 ]._rdwrlock && _write_locked_rdwrlocks[ index0 ]._module_index == module_index )
		{
			_write_locked_rdwrlocks[ index0 ]._rdwrlock = 0;
			_write_locked_rdwrlocks[ index0 ]._recursion = 0;
			_write_locked_rdwrlock_count--;
		}
		if( index0 == 0 )
			break;
	}
	for( size_t index0 = RUX_RDWRLOCK_INFO_COUNT - 1 ; ; index0-- )
	{
		if( _read_locked_rdwrlocks[ index0 ]._rdwrlock && _read_locked_rdwrlocks[ index0 ]._module_index == module_index )
		{
			_read_locked_rdwrlocks[ index0 ]._rdwrlock = 0;
			_read_locked_rdwrlocks[ index0 ]._recursion = 0;
			_read_locked_rdwrlock_count--;
		}
		if( index0 == 0 )
			break;
	}
};
void XThreadInfo::CollectModuleGCRefs( ::rux::int32 module_index )
{
	while( XInterlocked::CompareExchange( &_gcrefs_to_clean_locked , 1 , 0 ) != 0 )
	{
	}
	if( _gcrefs_to_clean_count > 0 )
	{
		for( size_t index0 = 0 ; index0 < RUX_MAX_GC_COUNT ; index0++ )
		{
			XGCRef* gcref = _gcrefs_to_clean[ index0 ];
			if( gcref && XMemoryManager::get_memory_module_index( gcref ) == module_index )
			{
				gcref->Release( __FILE__ , __LINE__ );
				_gcrefs_to_clean[ index0 ] = NULL;
				_gcrefs_to_clean_count--;
			}
		}
	}
	XInterlocked::CompareExchange( &_gcrefs_to_clean_locked , 0 , 1 );
};
void XThreadInfo::CollectGCRefs( ::rux::byte force , ::rux::byte lock )
{	
	if( lock )
	{
		while( XInterlocked::CompareExchange( &_gcrefs_to_clean_locked , 1 , 0 ) != 0 )
		{
		}
	}
	if( _gcrefs_to_clean_count > 0 )
	{
		if( force )
		{
			for( size_t index0 = 0 ; index0 < RUX_MAX_GC_COUNT ; index0++ )
			{
				XGCRef* gcref = _gcrefs_to_clean[ index0 ];
				if( gcref )
				{
					gcref->Release( __FILE__ , __LINE__ );
					_gcrefs_to_clean[ index0 ] = NULL;
					_gcrefs_to_clean_count--;
				}
			}
			_gcrefs_to_clean_index = 0;
		}
		else if( _gcrefs_to_clean_count == RUX_MAX_GC_COUNT )
		{	
			if( _gcrefs_to_clean_count )
			{
				for( ; ; )
				{
					XGCRef* gcref = _gcrefs_to_clean[ _gcrefs_to_clean_index ];
					if( gcref )
					{
						gcref->Release( __FILE__ , __LINE__ );
						_gcrefs_to_clean[ _gcrefs_to_clean_index ] = NULL;
						_gcrefs_to_clean_count--;
						break;
					}
					if( _gcrefs_to_clean_index == RUX_MAX_GC_COUNT - 1 )
						_gcrefs_to_clean_index = 0;
					else
						_gcrefs_to_clean_index++;
				}
			}
		}
	}
	if( lock )
		XInterlocked::CompareExchange( &_gcrefs_to_clean_locked , 0 , 1 );
};
void XThreadInfo::AddGCRef( const XGCRef* gc_ref_ptr )
{
	while( XInterlocked::CompareExchange( &_gcrefs_to_clean_locked , 1 , 0 ) != 0 )
	{
	}
	CollectGCRefs( 0 , (::rux::byte)0 );
	gc_ref_ptr->AddRef( __FILE__ , __LINE__ );
	for( ; ; )
	{
		if( _gcrefs_to_clean[ _gcrefs_to_clean_index ] )
		{
			if( _gcrefs_to_clean_index == RUX_MAX_GC_COUNT - 1 )
				_gcrefs_to_clean_index = 0;
			else
				_gcrefs_to_clean_index++;
		}
		else
			break;
	}
	_gcrefs_to_clean[ _gcrefs_to_clean_index ] = const_cast< XGCRef* >( gc_ref_ptr );
	_gcrefs_to_clean_count++;
	if( _gcrefs_to_clean_index == RUX_MAX_GC_COUNT - 1 )
		_gcrefs_to_clean_index = 0;
	else
		_gcrefs_to_clean_index++;
	XInterlocked::CompareExchange( &_gcrefs_to_clean_locked , 0 , 1 );	
};
void XThreadInfo::Remove( void )
{
	if( _count > 0 )
	{		
		_count--;
		if( _force_cpu || rux::engine::_diagnostics_cpu )
		{
			::rux::uint8 write_to_file = 0;
#ifdef __WINDOWS__
			LARGE_INTEGER qpc;
			if( QueryPerformanceCounter( &qpc ) )
				_performance[ _count ] = qpc.QuadPart - _performance[ _count ];
			if( ::rux::engine::_globals->_query_performance_frequency > 0 )
			{		
				if( 1000000000ULL >= ::rux::engine::_globals->_query_performance_frequency )
					_performance[ _count ] *= 1000000000ULL / ::rux::engine::_globals->_query_performance_frequency;
				else
					_performance[ _count ] *= (double)1000000000ULL / (double)::rux::engine::_globals->_query_performance_frequency;
			}
#elif defined( __UNIX__ )
			struct timespec ts = { 0 };
			clock_gettime( CLOCK_MONOTONIC , &ts );
			_performance[ _count ] = ts.tv_sec * 1000000000ULL + ts.tv_nsec - _performance[ _count ];		
#endif		
			if( _count > 0 )
				_delta[ _count - 1 ] += _performance[ _count ];
			if( _performance[ _count ] > _max_delay[ _count ] )
				write_to_file = 1;
			if( _check_delay[ _count ] == 1 && write_to_file == 1 )
			{			
				declare_stack_variable( char , line , 8192 );
				::rux::uint64 now = rux_get_now_as_local_unix_time();
				declare_stack_variable( char , now_string , 64 );
				rux_unix_time_to_string( now , now_string , 64 );
				_cs_rux_modules_name->ReadLock();
				if( _other_modules[ _count ] && _other_functions[ _count ] )
					::rux::safe_sprintf( line , 8192 , "%s [%u] module '%s', thread '%s', file '%s', function '%s', line %d, other module '%s', other function '%s', timeout " I64u " ms., cpu " I64u "%%.\n" , now_string , _tid , _rux_modules_name[ _modules[ _count ] ] , _thread_name , _filenames[ _count ] , _functions[ _count ] , _lines[ _count ] , _other_modules[ _count ] , _other_functions[ _count ] , _performance[ _count ] / 1000000ULL , ( 100ULL * ( _performance[ _count ] - _delta[ _count ] ) ) / _performance[ _count ] );
				else if( _other_functions[ _count ] )
					::rux::safe_sprintf( line , 8192 , "%s [%u] module '%s', thread '%s', file '%s', function '%s', line %d, other function '%s', timeout " I64u " ms., cpu " I64u "%%\n" , now_string , _tid , _rux_modules_name[ _modules[ _count ] ] , _thread_name , _filenames[ _count ] , _functions[ _count ] , _lines[ _count ] , _other_functions[ _count ] , _performance[ _count ] / 1000000ULL , ( 100ULL * ( _performance[ _count ] - _delta[ _count ] ) ) / _performance[ _count ] );
				else if( _other_modules[ _count ] )
					::rux::safe_sprintf( line , 8192 , "%s [%u] module '%s', thread '%s', file '%s', function '%s', line %d, other module '%s', timeout " I64u " ms., cpu " I64u "%%\n" , now_string , _tid , _rux_modules_name[ _modules[ _count ] ] , _thread_name , _filenames[ _count ] , _functions[ _count ] , _lines[ _count ] , _other_modules[ _count ] , _performance[ _count ] / 1000000ULL , ( 100ULL * ( _performance[ _count ] - _delta[ _count ] ) ) / _performance[ _count ] );
				else
					::rux::safe_sprintf( line , 8192 , "%s [%u] module '%s', thread '%s', file '%s', function '%s', line %d, timeout " I64u " ms., cpu " I64u "%%\n" , now_string , _tid , _rux_modules_name[ _modules[ _count ] ] , _thread_name , _filenames[ _count ] , _functions[ _count ] , _lines[ _count ] , _performance[ _count ] / 1000000ULL , ( 100ULL * ( _performance[ _count ] - _delta[ _count ] ) ) / _performance[ _count ] );
				_cs_rux_modules_name->ReadUnlock();
				_cs_cpu_logs->WriteLock( NULL , NULL , 0 , 0 );
				::memcpy( _cpu_logs[ _cpu_logs_write_index ] , line , 1024 );
				_cpu_logs[ _cpu_logs_write_index ][ 1023 ] = 0;
				_cpu_logs_write_index++;
				if( _cpu_logs_write_index == RUX_CPU_LOGS_COUNT )
					_cpu_logs_write_index = 0;
				_cs_cpu_logs->WriteUnlock();
			}
		}
	}
};
void XThreadInfo::add_label( size_t thread_index , const char* filename , const char* function , ::rux::int32 line , const char* other_module , const char* other_function , ::rux::int32 module_index , ::rux::uint8 check_delay , ::rux::uint32 max_delay )
{	
	::rux::pid_t tid = ::rux::threading::XThread::get_CurrentThreadId();
	XThreadInfo* ti = 0;
	_cs_rux_threads->ReadLock();
	if( _rux_threads && thread_index < _rux_threads_count )
	{
		ti = _rux_threads[ thread_index ];
		if( ti == 0 || ti->_tid != tid || ::XInterlocked::CompareExchange( &ti->_registered , 0 , 0 ) == 0 )
			ti = 0;
	}
	if( ti == 0 )
		ti = find_thread( tid );
	if( ti )
	{
		ti->lock();
		if( ti->_is_unknown )
			ti->_activity_time = ::rux::XTime::GetTickCount();
		_cs_rux_threads->ReadUnlock();
		ti->Add( filename , function , line , other_module , other_function , module_index , check_delay , max_delay , NULL );
		ti->unlock();
	}
	else
	{
		_cs_rux_threads->ReadUnlock();
		thread_index = add_thread_id_to_global( tid , "unknown thread" , module_index , 1 );
		add_label( thread_index , filename , function , line , other_module , other_function , module_index , check_delay , max_delay );
	}
};
void XThreadInfo::remove_label( size_t thread_index )
{
	::rux::pid_t tid = ::rux::threading::XThread::get_CurrentThreadId();
	XThreadInfo* ti = 0;
	_cs_rux_threads->ReadLock();
	if( _rux_threads && thread_index < _rux_threads_count )
	{
		ti = _rux_threads[ thread_index ];
		if( ti == 0 || ti->_tid != tid || ::XInterlocked::CompareExchange( &ti->_registered , 0 , 0 ) == 0 )
			ti = 0;
	}
	if( ti == 0 )
		ti = find_thread( tid );
	if( ti )
	{
		ti->lock();
		if( ti->_is_unknown )
			ti->_activity_time = ::rux::XTime::GetTickCount();
		_cs_rux_threads->ReadUnlock();
		ti->Remove();
		ti->unlock();
	}
	else
		_cs_rux_threads->ReadUnlock();
};
size_t XThreadInfo::add_thread_id_to_global( ::rux::pid_t thread_id , const char* thread_name , ::rux::int32 module_index , ::rux::byte is_unknown )
{
	size_t thread_index = 0;
	size_t unregistered_index = SIZE_MAX;
	size_t empty_cell_index = SIZE_MAX;
	XThreadInfo* ti = 0;
	_cs_rux_threads->WriteLock( __FILE__ , __FUNCTION__ , __LINE__ , 1 );
	if( _rux_threads )
	{
		for( ; thread_index < _rux_threads_count ; thread_index++ )
		{
			ti = _rux_threads[ thread_index ];
			if( ti )
			{
				if( ::XInterlocked::CompareExchange( &ti->_registered , 0 , 0 ) )
				{
					if( ti->_tid == thread_id )
						break;
				}
				else
					unregistered_index = thread_index;
			}
			else
				empty_cell_index = thread_index;
			ti = 0;
		}
	}
	if( ti == 0 )
	{
		if( unregistered_index != SIZE_MAX )
		{
			ti = _rux_threads[ unregistered_index ];
			thread_index = unregistered_index;
		}
		else if( empty_cell_index != SIZE_MAX )
		{
			ti = alloc_object_macros( XThreadInfo );
			_rux_threads[ empty_cell_index ] = ti;
			thread_index = empty_cell_index;
		}
		else
		{
			size_t rux_threads_count = _rux_threads_count;
			::rux::memory::expand< XThreadInfo* , 5 >( _rux_threads , _rux_threads_count , rux_threads_count + 1 , 1 );
			ti = alloc_object_macros( XThreadInfo );
			_rux_threads[ rux_threads_count ] = ti;
			thread_index = rux_threads_count;
		}
		ti->null();
		ti->_tid = thread_id;
		ti->_module_index = module_index;
		if( thread_name )
			::rux::safe_strncpy( ti->_thread_name , thread_name , 16 );
		if( ::rux::engine::_globals )
			::rux::engine::_globals->_on_thread_registered.raise< ::rux::pid_t , ::rux::threading::Thread* , size_t >( ti->_tid , 0 , thread_index );
	}
	ti->_is_unknown = is_unknown;
	::XInterlocked::Increment( &ti->_registered );
	_cs_rux_threads->WriteUnlock();
	return thread_index;
};
size_t XThreadInfo::add_thread_to_global( XGCRef* memory , ::rux::int32 module_index )
{
	::rux::pid_t thread_id = ( (::rux::threading::Thread*)memory )->get_ThreadId();
	size_t thread_index = 0;
	size_t unregistered_index = SIZE_MAX;
	size_t empty_cell_index = SIZE_MAX;
	XThreadInfo* ti = 0;
	_cs_rux_threads->WriteLock( __FILE__ , __FUNCTION__ , __LINE__ , 1 );
	if( _rux_threads )
	{
		for( ; thread_index < _rux_threads_count ; thread_index++ )
		{
			ti = _rux_threads[ thread_index ];
			if( ti )
			{
				if( ::XInterlocked::CompareExchange( &ti->_registered , 0 , 0 ) )
				{
					if( ti->_tid == thread_id )
						break;
				}
				else
					unregistered_index = thread_index;
			}
			else
				empty_cell_index = thread_index;
			ti = 0;
		}
	}
	if( ti == 0 )
	{
		if( unregistered_index != SIZE_MAX )
		{
			ti = _rux_threads[ unregistered_index ];
			thread_index = unregistered_index;
		}
		else if( empty_cell_index != SIZE_MAX )
		{
			ti = alloc_object_macros( XThreadInfo );
			_rux_threads[ empty_cell_index ] = ti;
			thread_index = empty_cell_index;
		}
		else
		{
			size_t rux_threads_count = _rux_threads_count;
			::rux::memory::expand< XThreadInfo* , 5 >( _rux_threads , _rux_threads_count , rux_threads_count + 1 , 1 );
			ti = alloc_object_macros( XThreadInfo );
			_rux_threads[ rux_threads_count ] = ti;
			thread_index = rux_threads_count;
		}
		ti->null();
		memory->AddRef( __FILE__ , __LINE__ );
		ti->_gcref = memory;
		ti->_tid = thread_id;
		ti->_module_index = module_index;
		char* thread_name = ( (::rux::threading::Thread*)memory )->get_ThreadName();
		if( thread_name )
			::rux::safe_strncpy( ti->_thread_name , thread_name , 16 );
		if( ::rux::engine::_globals )
			::rux::engine::_globals->_on_thread_registered.raise< ::rux::pid_t , ::rux::threading::Thread* , size_t >( ti->_tid , (::rux::threading::Thread*)memory , thread_index );
	}
	::XInterlocked::Increment( &ti->_registered );
	_cs_rux_threads->WriteUnlock();
	return thread_index;
};
void XThreadInfo::remove_thread_from_global( size_t index )
{
	rux_interlocked_return res = 0;
	_cs_rux_threads->ReadLock();
	if( index < _rux_threads_count && _rux_threads[ index ] && ( res = ::XInterlocked::CompareExchange( &_rux_threads[ index ]->_registered , 0 , 0 ) ) )
	{
		XThreadInfo* ti = _rux_threads[ index ];
		ti->lock();
		_cs_rux_threads->ReadUnlock();
		XGCRef* gcref = ti->_gcref;
		if( res == 1 )
		{
			ti->unregister();
			::XInterlocked::Exchange( &ti->_registered , 0 );
			if( ::rux::engine::_globals )
				::rux::engine::_globals->_on_thread_unregistered.raise< ::rux::pid_t , ::rux::threading::Thread* , size_t >( ti->_tid , (::rux::threading::Thread*)gcref , index );
			if( gcref )
				gcref->Release( __FILE__ , __LINE__ );
		}
		else
			::XInterlocked::Decrement( &ti->_registered );
		ti->unlock();
	}
	else
		_cs_rux_threads->ReadUnlock();
};
size_t XThreadInfo::get_threads_count( void )
{
	size_t count = 0;
	if( _rux_threads && _cs_rux_threads )
	{
		_cs_rux_threads->ReadLock();
		for( size_t index = 0 ; index < _rux_threads_count ; index++ )
		{
			XThreadInfo* ti = _rux_threads[ index ];
			if( ti )
			{
				ti->lock();
				_cs_rux_threads->ReadUnlock();
				if( ::XInterlocked::CompareExchange( &ti->_registered , 0 , 0 ) && ti->_gcref )
					count++;
				ti->unlock();
				_cs_rux_threads->ReadLock();
			}
		}
		_cs_rux_threads->ReadUnlock();
	}
	return count;
};
XMallocArray< XGCRef* >* XThreadInfo::get_all_threads( void )
{
	void* ptr = alloc_array_macros( char , sizeof( XMallocArray< XGCRef* > ) );
	XMallocArray< XGCRef* >* threads = new (ptr)XMallocArray< XGCRef* >();		
	_cs_rux_threads->ReadLock();
	for( size_t index = 0 ; index < _rux_threads_count ; index++ )
	{
		XThreadInfo* ti = _rux_threads[ index ];
		if( ti )
		{
			ti->lock();
			if( ::XInterlocked::CompareExchange( &ti->_registered , 0 , 0 ) )
			{
				if( ti->_gcref )
				{
					ti->_gcref->AddRef( __FILE__ , __LINE__ );
					threads->FillEmptyOrAdd( ti->_gcref );
				}
			}
			ti->unlock();
		}
	}
	_cs_rux_threads->ReadUnlock();
	return threads;
};
void XThreadInfo::AddGCRef( rux::int32 module_index , const XGCRef* gc_ref_ptr )
{
	::rux::pid_t tid = ::rux::threading::XThread::get_CurrentThreadId();
	_cs_rux_threads->ReadLock();
	XThreadInfo* ti = find_thread( tid );
	if( ti )
	{
		ti->lock();
		if( ti->_is_unknown )
			ti->_activity_time = ::rux::XTime::GetTickCount();
		_cs_rux_threads->ReadUnlock();
		ti->AddGCRef( gc_ref_ptr );
		ti->unlock();
	}
	else 
	{
		_cs_rux_threads->ReadUnlock();
		add_thread_id_to_global( tid , "unknown thread" , module_index , 1 );
		AddGCRef( module_index , gc_ref_ptr );
	}
};
size_t XThreadInfo::get_thread_index( ::rux::int32 module_index )
{
	::rux::pid_t tid = ::rux::threading::XThread::get_CurrentThreadId();
	_cs_rux_threads->ReadLock();
	size_t thread_index = find_thread_index( tid );
	if( thread_index >= _rux_threads_count )
	{
		_cs_rux_threads->ReadUnlock();
		thread_index = add_thread_id_to_global( tid , "unknown thread" , module_index , 1 );
	}
	else
		_cs_rux_threads->ReadUnlock();
	return thread_index;
};
XThreadInfo* XThreadInfo::find_thread( ::rux::pid_t tid )
{
	XThreadInfo* ti = 0;
	if( _rux_threads )
	{
		for( size_t thread_index = 0 ; thread_index < _rux_threads_count ; thread_index++ )
		{
			ti = _rux_threads[ thread_index ];
			if( ti && ti->_tid == tid && ::XInterlocked::CompareExchange( &ti->_registered , 0 , 0 ) )
				break;
			ti = 0;
		}
	}
	return ti;
};
size_t XThreadInfo::find_thread_index( ::rux::pid_t tid )
{
	size_t thread_index = SIZE_MAX;
	if( _rux_threads )
	{
		XThreadInfo* ti = 0;
		for( thread_index = 0 ; thread_index < _rux_threads_count ; thread_index++ )
		{
			ti = _rux_threads[ thread_index ];
			if( ti && ti->_tid == tid && ::XInterlocked::CompareExchange( &ti->_registered , 0 , 0 ) )
				break;
		}
	}
	return thread_index;
};
void XThreadInfo::CollectGCRefs( ::rux::byte force , size_t thread_index )
{
	XThreadInfo* ti = 0;
	::rux::pid_t tid = ::rux::threading::XThread::get_CurrentThreadId();
	_cs_rux_threads->ReadLock( __FILE__ , __FUNCTION__ , __LINE__ , 1 );
	if( _rux_threads && thread_index < _rux_threads_count )
	{
		ti = _rux_threads[ thread_index ];
		if( ti == 0 || ti->_tid != tid || ::XInterlocked::CompareExchange( &ti->_registered , 0 , 0 ) == 0 )
			ti = 0;
	}
	if( ti == 0 )
		ti = find_thread( tid );
	if( ti )
	{
		ti->lock();
		if( ti->_is_unknown )
			ti->_activity_time = ::rux::XTime::GetTickCount();
		_cs_rux_threads->ReadUnlock();
		ti->CollectGCRefs( force , (::rux::byte)1 );
		ti->unlock();
	}
	else
		_cs_rux_threads->ReadUnlock();
};
void XThreadInfo::free_all_threads( XMallocArray< XGCRef* >* threads )
{
	for( size_t index = 0 ; index < threads->Count() ; index++ )
		threads->operator[]( index )->Release( __FILE__ , __LINE__ );
	threads->~XMallocArray();
	::rux::engine::free_mem( threads );
};
void XThreadInfo::set_thread_force_cpu( size_t thread_index , ::rux::byte force_cpu )
{
	XThreadInfo* ti = 0;
	::rux::pid_t tid = ::rux::threading::XThread::get_CurrentThreadId();
	_cs_rux_threads->ReadLock( __FILE__ , __FUNCTION__ , __LINE__ , 1 );
	if( _rux_threads && thread_index < _rux_threads_count )
	{
		ti = _rux_threads[ thread_index ];
		if( ti == 0 || ti->_tid != tid || ::XInterlocked::CompareExchange( &ti->_registered , 0 , 0 ) == 0 )
			ti = 0;
	}
	if( ti == 0 )
		ti = find_thread( tid );
	if( ti )
	{
		ti->lock();
		if( ti->_is_unknown )
			ti->_activity_time = ::rux::XTime::GetTickCount();
		_cs_rux_threads->ReadUnlock();
		if( force_cpu )
			ti->_force_cpu++;
		else
			ti->_force_cpu--;
		ti->unlock();
	}
	else
		_cs_rux_threads->ReadUnlock();
};
void XThreadInfo::add_write_locked_rdwrlock( size_t thread_index , ::rux::threading::RdWrLock* rdwrlock , size_t& dst_index , const char* file , ::rux::uint32 line , ::rux::int32 module_index )
{
	size_t index = dst_index;
	XThreadInfo* ti = 0;
	::rux::pid_t tid = ::rux::threading::XThread::get_CurrentThreadId();
	_cs_rux_threads->ReadLock( __FILE__ , __FUNCTION__ , __LINE__ , 1 );
	if( _rux_threads && thread_index < _rux_threads_count )
	{
		ti = _rux_threads[ thread_index ];
		if( ti == 0 || ti->_tid != tid || ::XInterlocked::CompareExchange( &ti->_registered , 0 , 0 ) == 0 )
			ti = 0;
	}
	if( ti == 0 )
		ti = find_thread( tid );
	if( ti )
	{
		ti->lock();
		if( ti->_is_unknown )
			ti->_activity_time = ::rux::XTime::GetTickCount();
		_cs_rux_threads->ReadUnlock();
		if( index == SIZE_MAX || index >= ti->_write_locked_rdwrlock_count || ti->_write_locked_rdwrlocks[ index ]._rdwrlock != rdwrlock )
		{
			for( index = 0 ; index < RUX_RDWRLOCK_INFO_COUNT ; index++ )
			{
				if( ti->_write_locked_rdwrlocks[ index ]._rdwrlock == rdwrlock )
				{
					ti->_write_locked_rdwrlocks[ index ]._recursion++;
					break;
				}
			}
			if( index >= RUX_RDWRLOCK_INFO_COUNT )
			{
				if( ti->_write_locked_rdwrlock_count < RUX_RDWRLOCK_INFO_COUNT )
				{
					ti->_write_locked_rdwrlock_count++;
					for( index = 0 ; index < RUX_RDWRLOCK_INFO_COUNT ; index++ )
					{
						if( ti->_write_locked_rdwrlocks[ index ]._rdwrlock == 0 )
						{
							ti->_write_locked_rdwrlocks[ index ]._recursion = 1;
							ti->_write_locked_rdwrlocks[ index ]._rdwrlock = rdwrlock;
							ti->_write_locked_rdwrlocks[ index ]._file = file;
							ti->_write_locked_rdwrlocks[ index ]._line = line;
							ti->_write_locked_rdwrlocks[ index ]._module_index = module_index;
							break;
						}
					}
				}
			}
		}
		else
			ti->_write_locked_rdwrlocks[ index ]._recursion++;
		ti->unlock();
	}
	else 
	{
		_cs_rux_threads->ReadUnlock();
		thread_index = add_thread_id_to_global( tid , "unknown thread" , module_index , 1 );
		add_write_locked_rdwrlock( thread_index , rdwrlock , index , file , line , module_index );
	}
	dst_index = index;
};
::rux::byte XThreadInfo::remove_write_locked_rdwrlock( ::rux::threading::RdWrLock* rdwrlock , size_t& dst_index , bool all )
{
	size_t index = dst_index;
	::rux::byte res = 0;
	if( index < RUX_RDWRLOCK_INFO_COUNT && _write_locked_rdwrlocks[ index ]._rdwrlock == rdwrlock )
	{
		if( all )
			_write_locked_rdwrlocks[ index ]._recursion = 0;
		else
			_write_locked_rdwrlocks[ index ]._recursion--;
		if( _write_locked_rdwrlocks[ index ]._recursion == 0 )
		{
			_write_locked_rdwrlocks[ index ]._rdwrlock = 0;
			_write_locked_rdwrlock_count--;
			index = SIZE_MAX;
			dst_index = index;
		}
		res = 1;
	}
	else
	{
		for( size_t index0 = 0 ; index0 < RUX_RDWRLOCK_INFO_COUNT ; index0++ )
		{
			if( _write_locked_rdwrlocks[ index0 ]._rdwrlock == rdwrlock )
			{
				if( all )
					_write_locked_rdwrlocks[ index0 ]._recursion = 0;
				else
					_write_locked_rdwrlocks[ index0 ]._recursion--;
				if( _write_locked_rdwrlocks[ index0 ]._recursion == 0 )
				{
					_write_locked_rdwrlocks[ index0 ]._rdwrlock = 0;
					_write_locked_rdwrlock_count--;
					index = SIZE_MAX;
				}
				else
					index = index0;
				res = 1;
				dst_index = index;
				break;
			}
		}
	}
	return res;
};
void XThreadInfo::remove_write_locked_rdwrlock( size_t thread_index , ::rux::threading::RdWrLock* rdwrlock , size_t& dst_index , rux::byte release )
{
	size_t index = dst_index;
	if( release )
	{
		XThreadInfo* ti = NULL;
		_cs_rux_threads->ReadLock( __FILE__ , __FUNCTION__ , __LINE__ , 1 );
		if( _rux_threads )
		{
			for( size_t ti_index = 0 ; ti_index < _rux_threads_count ; ti_index++ )
			{
				ti = _rux_threads[ ti_index ];
				if( ti )
				{	
					ti->lock();
					_cs_rux_threads->ReadUnlock();
					if( ::XInterlocked::CompareExchange( &ti->_registered , 0 , 0 ) )
					{
						if( ti->_is_unknown )
							ti->_activity_time = ::rux::XTime::GetTickCount();
						if( ti->remove_write_locked_rdwrlock( rdwrlock , index , true ) )
						{
							ti->unlock();
							_cs_rux_threads->ReadLock();
							break;
						}
						else
						{
							ti->unlock();
							_cs_rux_threads->ReadLock();
						}
					}
					else
					{
						ti->unlock();
						_cs_rux_threads->ReadLock();
					}
				}
			}
		}
		_cs_rux_threads->ReadUnlock();
	}
	else
	{
		XThreadInfo* ti = 0;
		::rux::pid_t tid = ::rux::threading::XThread::get_CurrentThreadId();
		_cs_rux_threads->ReadLock( __FILE__ , __FUNCTION__ , __LINE__ , 1 );
		if( _rux_threads && thread_index < _rux_threads_count )
		{
			ti = _rux_threads[ thread_index ];
			if( ti == 0 || ti->_tid != tid || ::XInterlocked::CompareExchange( &ti->_registered , 0 , 0 ) == 0 )
				ti = 0;
		}
		if( ti == 0 )
			ti = find_thread( tid );
		if( ti )
		{	
			ti->lock();
			if( ti->_is_unknown )
				ti->_activity_time = ::rux::XTime::GetTickCount();
			_cs_rux_threads->ReadUnlock();
			ti->remove_write_locked_rdwrlock( rdwrlock , index , false );
			ti->unlock();
		}
		else
			_cs_rux_threads->ReadUnlock();
	}
	dst_index = index;
};
void XThreadInfo::add_read_locked_rdwrlock( size_t thread_index , ::rux::threading::RdWrLock* rdwrlock , size_t& dst_index , const char* file , ::rux::uint32 line , ::rux::int32 module_index )
{
	size_t index = dst_index;
	XThreadInfo* ti = 0;
	::rux::pid_t tid = ::rux::threading::XThread::get_CurrentThreadId();
	_cs_rux_threads->ReadLock( __FILE__ , __FUNCTION__ , __LINE__ , 1 );
	if( _rux_threads && thread_index < _rux_threads_count )
	{
		ti = _rux_threads[ thread_index ];
		if( ti == 0 || ti->_tid != tid || ::XInterlocked::CompareExchange( &ti->_registered , 0 , 0 ) == 0 )
			ti = 0;
	}
	if( ti == 0 )
		ti = find_thread( tid );
	if( ti )
	{
		ti->lock();
		if( ti->_is_unknown )
			ti->_activity_time = ::rux::XTime::GetTickCount();
		_cs_rux_threads->ReadUnlock();
		if( index == SIZE_MAX || index >= ti->_read_locked_rdwrlock_count || ti->_read_locked_rdwrlocks[ index ]._rdwrlock != rdwrlock )
		{
			for( index = 0 ; index < RUX_RDWRLOCK_INFO_COUNT ; index++ )
			{
				if( ti->_read_locked_rdwrlocks[ index ]._rdwrlock == rdwrlock )
				{
					ti->_read_locked_rdwrlocks[ index ]._recursion++;
					break;
				}
			}
			if( index >= RUX_RDWRLOCK_INFO_COUNT )
			{
				if( ti->_read_locked_rdwrlock_count < RUX_RDWRLOCK_INFO_COUNT )
				{
					ti->_read_locked_rdwrlock_count++;
					for( index = 0 ; index < RUX_RDWRLOCK_INFO_COUNT ; index++ )
					{
						if( ti->_read_locked_rdwrlocks[ index ]._rdwrlock == 0 )
						{
							ti->_read_locked_rdwrlocks[ index ]._recursion = 1;
							ti->_read_locked_rdwrlocks[ index ]._rdwrlock = rdwrlock;
							ti->_read_locked_rdwrlocks[ index ]._file = file;
							ti->_read_locked_rdwrlocks[ index ]._line = line;
							ti->_read_locked_rdwrlocks[ index ]._module_index = module_index;
							break;
						}
					}
				}
			}
		}
		else
			ti->_read_locked_rdwrlocks[ index ]._recursion++;
		ti->unlock();
	}
	else 
	{
		_cs_rux_threads->ReadUnlock();
		thread_index = add_thread_id_to_global( tid , "unknown thread" , module_index , 1 );
		add_read_locked_rdwrlock( thread_index , rdwrlock , index , file , line , module_index );
	}
	dst_index = index;
};
void XThreadInfo::remove_read_locked_rdwrlock( ::rux::threading::RdWrLock* rdwrlock , size_t& dst_index , bool all )
{
	size_t index = dst_index;
	if( index < RUX_RDWRLOCK_INFO_COUNT && _read_locked_rdwrlocks[ index ]._rdwrlock == rdwrlock )
	{
		if( all )
			_read_locked_rdwrlocks[ index ]._recursion = 0;
		else
			_read_locked_rdwrlocks[ index ]._recursion--;
		if( _read_locked_rdwrlocks[ index ]._recursion == 0 )
		{
			_read_locked_rdwrlocks[ index ]._rdwrlock = 0;
			_read_locked_rdwrlock_count--;
			index = SIZE_MAX;
			dst_index = index;
		}
	}
	else
	{
		for( size_t index0 = 0 ; index0 < RUX_RDWRLOCK_INFO_COUNT ; index0++ )
		{
			if( _read_locked_rdwrlocks[ index0 ]._rdwrlock == rdwrlock )
			{
				if( all )
					_read_locked_rdwrlocks[ index0 ]._recursion = 0;
				else
					_read_locked_rdwrlocks[ index0 ]._recursion--;
				if( _read_locked_rdwrlocks[ index0 ]._recursion == 0 )
				{
					_read_locked_rdwrlocks[ index0 ]._rdwrlock = 0;
					_read_locked_rdwrlock_count--;
					index = SIZE_MAX;
				}
				else
					index = index0;
				dst_index = index;
				break;
			}
		}
	}
};
void XThreadInfo::remove_read_locked_rdwrlock( size_t thread_index , ::rux::threading::RdWrLock* rdwrlock , size_t& dst_index , ::rux::byte release )
{
	size_t index = dst_index;
	if( release )
	{
		XThreadInfo* ti = NULL;
		_cs_rux_threads->ReadLock( __FILE__ , __FUNCTION__ , __LINE__ , 1 );
		if( _rux_threads )
		{
			for( size_t ti_index = 0 ; ti_index < _rux_threads_count ; ti_index++ )
			{
				ti = _rux_threads[ ti_index ];
				if( ti )
				{	
					ti->lock();
					if( ::XInterlocked::CompareExchange( &ti->_registered , 0 , 0 ) )
					{
						if( ti->_is_unknown )
							ti->_activity_time = ::rux::XTime::GetTickCount();					
						ti->remove_read_locked_rdwrlock( rdwrlock , index , true );
					}
					ti->unlock();
				}
			}
		}
		_cs_rux_threads->ReadUnlock();
	}
	else
	{
		XThreadInfo* ti = 0;
		::rux::pid_t tid = ::rux::threading::XThread::get_CurrentThreadId();
		_cs_rux_threads->ReadLock( __FILE__ , __FUNCTION__ , __LINE__ , 1 );
		if( _rux_threads && thread_index < _rux_threads_count )
		{
			ti = _rux_threads[ thread_index ];
			if( ti == 0 || ti->_tid != tid || ::XInterlocked::CompareExchange( &ti->_registered , 0 , 0 ) == 0 )
				ti = 0;
		}
		if( ti == 0 )
			ti = find_thread( tid );
		if( ti )
		{	
			ti->lock();
			if( ti->_is_unknown )
				ti->_activity_time = ::rux::XTime::GetTickCount();
			_cs_rux_threads->ReadUnlock();
			ti->remove_read_locked_rdwrlock( rdwrlock , index , false );
			ti->unlock();
		}
		else
			_cs_rux_threads->ReadUnlock();
	}
	dst_index = index;
};
size_t XThreadInfo::write_locked_rdwrlock_count( size_t thread_index )
{
	size_t count = 0;
	XThreadInfo* ti = 0;
	::rux::pid_t tid = ::rux::threading::XThread::get_CurrentThreadId();
	_cs_rux_threads->ReadLock( __FILE__ , __FUNCTION__ , __LINE__ , 1 );
	if( _rux_threads && thread_index < _rux_threads_count )
	{
		ti = _rux_threads[ thread_index ];
		if( ti == 0 || ti->_tid != tid || ::XInterlocked::CompareExchange( &ti->_registered , 0 , 0 ) == 0 )
			ti = 0;
	}
	if( ti == 0 )
		ti = find_thread( tid );
	if( ti )
	{
		ti->lock();
		if( ti->_is_unknown )
			ti->_activity_time = ::rux::XTime::GetTickCount();		
		_cs_rux_threads->ReadUnlock();
		count = ti->_write_locked_rdwrlock_count;
		ti->unlock();
	}
	else
		_cs_rux_threads->ReadUnlock();
	return count;
};
size_t XThreadInfo::read_locked_rdwrlock_count( size_t thread_index )
{
	size_t count = 0;
	XThreadInfo* ti = 0;
	::rux::pid_t tid = ::rux::threading::XThread::get_CurrentThreadId();
	_cs_rux_threads->ReadLock( __FILE__ , __FUNCTION__ , __LINE__ , 1 );
	if( _rux_threads && thread_index < _rux_threads_count )
	{
		ti = _rux_threads[ thread_index ];
		if( ti == 0 || ti->_tid != tid || ::XInterlocked::CompareExchange( &ti->_registered , 0 , 0 ) == 0 )
			ti = 0;
	}
	if( ti == 0 )
		ti = find_thread( tid );
	if( ti )
	{
		ti->lock();
		if( ti->_is_unknown )
			ti->_activity_time = ::rux::XTime::GetTickCount();
		_cs_rux_threads->ReadUnlock();
		count = ti->_read_locked_rdwrlock_count;
		ti->unlock();
	}
	else
		_cs_rux_threads->ReadUnlock();
	return count;
};
void XThreadInfo::add_locked_crt_sect( size_t thread_index , XCrtSect* crt_sect , size_t& dst_index , const char* file , ::rux::uint32 line , ::rux::int32 module_index )
{
	size_t index = dst_index;
	XThreadInfo* ti = 0;
	::rux::pid_t tid = ::rux::threading::XThread::get_CurrentThreadId();
	_cs_rux_threads->ReadLock( __FILE__ , __FUNCTION__ , __LINE__ , 1 );
	if( _rux_threads && thread_index < _rux_threads_count )
	{
		ti = _rux_threads[ thread_index ];
		if( ti == 0 || ti->_tid != tid || ::XInterlocked::CompareExchange( &ti->_registered , 0 , 0 ) == 0 )
			ti = 0;
	}
	if( ti == 0 )
		ti = find_thread( tid );
	if( ti )
	{
		ti->lock();
		if( ti->_is_unknown )
			ti->_activity_time = ::rux::XTime::GetTickCount();
		_cs_rux_threads->ReadUnlock();
		if( index == SIZE_MAX || index >= ti->_locked_crt_sects_count || ti->_locked_crt_sects[ index ]._crt_sect != crt_sect )
		{
			for( index = 0 ; index < RUX_CRTSECT_INFO_COUNT ; index++ )
			{
				if( ti->_locked_crt_sects[ index ]._crt_sect == crt_sect )
				{
					ti->_locked_crt_sects[ index ]._recursion++;
					break;
				}
			}
			if( index >= RUX_CRTSECT_INFO_COUNT )
			{
				if( ti->_locked_crt_sects_count < RUX_CRTSECT_INFO_COUNT )
				{
					ti->_locked_crt_sects_count++;
					for( index = 0 ; index < RUX_CRTSECT_INFO_COUNT ; index++ )
					{
						if( ti->_locked_crt_sects[ index ]._crt_sect == 0 )
						{
							ti->_locked_crt_sects[ index ]._recursion = 1;
							ti->_locked_crt_sects[ index ]._crt_sect = crt_sect;
							ti->_locked_crt_sects[ index ]._file = file;
							ti->_locked_crt_sects[ index ]._line = line;
							ti->_locked_crt_sects[ index ]._module_index = module_index;
							break;
						}
					}
				}
			}
		}
		else
			ti->_locked_crt_sects[ index ]._recursion++;
		ti->unlock();
	}
	else 
	{
		_cs_rux_threads->ReadUnlock();
		thread_index = add_thread_id_to_global( tid , "unknown thread" , module_index , 1 );
		add_locked_crt_sect( thread_index , crt_sect , index , file , line , module_index );
	}
	dst_index = index;
};
::rux::byte XThreadInfo::remove_locked_crt_sect( XCrtSect* crt_sect , size_t& dst_index , bool all )
{
	size_t index = dst_index;
	::rux::byte res = 0;
	if( index < RUX_CRTSECT_INFO_COUNT && _locked_crt_sects[ index ]._crt_sect == crt_sect )
	{
		if( all )
			_locked_crt_sects[ index ]._recursion = 0;
		else
			_locked_crt_sects[ index ]._recursion--;
		if( _locked_crt_sects[ index ]._recursion == 0 )
		{
			_locked_crt_sects[ index ]._crt_sect = 0;
			_locked_crt_sects_count--;
			index = SIZE_MAX;
			dst_index = index;
		}
		res = 1;
	}
	else
	{
		for( size_t index0 = 0 ; index0 < RUX_CRTSECT_INFO_COUNT ; index0++ )
		{
			if( _locked_crt_sects[ index0 ]._crt_sect == crt_sect )
			{
				if( all )
					_locked_crt_sects[ index0 ]._recursion = 0;
				else
					_locked_crt_sects[ index0 ]._recursion--;
				if( _locked_crt_sects[ index0 ]._recursion == 0 )
				{
					_locked_crt_sects[ index0 ]._crt_sect = 0;
					_locked_crt_sects_count--;
					index = SIZE_MAX;
				}
				else
					index = index0;
				res = 1;
				dst_index = index;
				break;
			}
		}
	}
	return res;
};
void XThreadInfo::remove_locked_crt_sect( size_t thread_index , XCrtSect* crt_sect , size_t& dst_index , ::rux::byte release )
{
	size_t index = dst_index;
	if( release )
	{
		XThreadInfo* ti = NULL;
		_cs_rux_threads->ReadLock( __FILE__ , __FUNCTION__ , __LINE__ , 1 );
		if( _rux_threads )
		{
			for( size_t ti_index = 0 ; ti_index < _rux_threads_count ; ti_index++ )
			{
				ti = _rux_threads[ ti_index ];
				if( ti )
				{	
					ti->lock();
					if( ::XInterlocked::CompareExchange( &ti->_registered , 0 , 0 ) )
					{
						if( ti->_is_unknown )
							ti->_activity_time = ::rux::XTime::GetTickCount();
						if( ti->remove_locked_crt_sect( crt_sect , index , true ) )
						{
							ti->unlock();
							break;
						}
						else
							ti->unlock();
					}
					else
						ti->unlock();
				}
			}
		}
		_cs_rux_threads->ReadUnlock();
	}
	else
	{
		::rux::pid_t tid = ::rux::threading::XThread::get_CurrentThreadId();
		XThreadInfo* ti = 0;
		_cs_rux_threads->ReadLock();
		if( _rux_threads && thread_index < _rux_threads_count )
		{
			ti = _rux_threads[ thread_index ];
			if( ti == 0 || ti->_tid != tid || ::XInterlocked::CompareExchange( &ti->_registered , 0 , 0 ) == 0 )
				ti = 0;
		}
		if( ti == 0 )
			ti = find_thread( tid );
		if( ti )
		{	
			ti->lock();
			if( ti->_is_unknown )
				ti->_activity_time = ::rux::XTime::GetTickCount();
			_cs_rux_threads->ReadUnlock();			
			ti->remove_locked_crt_sect( crt_sect , index , false );
			ti->unlock();
		}
		else
			_cs_rux_threads->ReadUnlock();
	}
	dst_index = index;
};
size_t XThreadInfo::locked_crt_sect_count( size_t thread_index )
{
	size_t count = 0;
	XThreadInfo* ti = 0;
	::rux::pid_t tid = ::rux::threading::XThread::get_CurrentThreadId();
	_cs_rux_threads->ReadLock( __FILE__ , __FUNCTION__ , __LINE__ , 1 );
	if( _rux_threads && thread_index < _rux_threads_count )
	{
		ti = _rux_threads[ thread_index ];
		if( ti == 0 || ti->_tid != tid || ::XInterlocked::CompareExchange( &ti->_registered , 0 , 0 ) == 0 )
			ti = 0;
	}
	if( ti == 0 )
		ti = find_thread( tid );
	if( ti )
	{
		ti->lock();
		if( ti->_is_unknown )
			ti->_activity_time = ::rux::XTime::GetTickCount();
		_cs_rux_threads->ReadUnlock();
		count = ti->_locked_crt_sects_count;
		ti->unlock();
	}
	else
		_cs_rux_threads->ReadUnlock();
	return count;
};
void XThreadInfo::write_diagnostics( XMemoryManager* _this , char* now_string , ::rux::uint8& is_interrupt )
{
	if( is_interrupt == 0 )
	{
		if( rux_is_exists_file( _this->_stable_info_filename._str ) )
			chmod( _this->_stable_info_filename._str , 0777 );
		::rux::uint64 size = rux_get_file_size( _this->_stable_info_filename._str );
		if( size >= 1024ULL * 1024ULL )
			rux_remove_file( _this->_stable_info_filename._str );
	}
	::rux::int32 line = 0;
	::rux::uint8 written = 0;
	::rux::uint32 pid = (::rux::uint32)::rux::diagnostics::process::get_process_id();
	{
		::rux::io::file iofile( _this->_stable_info_filename._str , XOpenWriteText );
		if( iofile.opened() == false )
			iofile.open( _this->_stable_info_filename._str , XCreateWriteText );
		if( iofile.opened() )
		{
			iofile.seek( iofile.size() );
			_cs_rux_threads->ReadLock();
			if( _rux_threads )
			{
				for( size_t index0 = 0 ; index0 < _rux_threads_count ; index0++ )
				{
					if( is_interrupt == 1 )
						break;				
					XThreadInfo* ti = _rux_threads[ index0 ];
					if( ti )
					{
						ti->lock();
						_cs_rux_threads->ReadUnlock();
						if( ::XInterlocked::CompareExchange( &ti->_registered , 0 , 0 ) )
						{
							_this->_rux_threads_name.clear();
							::rux::uint64 now = 0;
							::rux::uint32 tid = (::rux::uint32)ti->_tid;
							size_t labels_count = ti->_count;
							if( labels_count > 0 && labels_count <= 2048 )
							{							
								now = ti->get_Timeout( labels_count - 1 );
								_cs_rux_modules_name->ReadLock();
								_this->_thread_info_rux_modules_name.clear();
								_this->_thread_info_rux_modules_name += _rux_modules_name[ ti->_module_index ];
								_cs_rux_modules_name->ReadUnlock();
								_this->_file.clear();
								if( ti->_filenames[ labels_count - 1 ] )
									_this->_file += ti->_filenames[ labels_count - 1 ];
								_this->_function.clear();
								if( ti->_functions[ labels_count - 1 ] )
									_this->_function += ti->_functions[ labels_count - 1 ];
								_this->_other_module.clear();
								if( ti->_other_modules[ labels_count - 1 ] )
									_this->_other_module += ti->_other_modules[ labels_count - 1 ];
								_this->_other_function.clear();
								if( ti->_other_functions[ labels_count - 1 ] )
									_this->_other_function += ti->_other_functions[ labels_count - 1 ];
								line = ti->_lines[ labels_count - 1 ];
							}
							_this->_read_locked_rdwrlock_string.clear();
							size_t read_locked_rdwrlock_count = ti->_read_locked_rdwrlock_count;;
							if( read_locked_rdwrlock_count )
							{
								for( size_t index1 = 0 ; index1 < read_locked_rdwrlock_count ; index1++ )
								{
									XRdWrLockInfo* rdwrlock_info = &ti->_read_locked_rdwrlocks[ index1 ];
									if( rdwrlock_info->_rdwrlock )
									{
										_this->_read_locked_rdwrlock_string += rdwrlock_info->_rdwrlock->_name;
										if( rdwrlock_info->_file )
										{
											_this->_read_locked_rdwrlock_string += " locked by ";
											_this->_read_locked_rdwrlock_string += rdwrlock_info->_file;
											if( rdwrlock_info->_line > 0 )
											{
												_this->_read_locked_rdwrlock_string += ":";
												char line_string[ 128 ] = {0};
												rux::string::uint32_to_string( rdwrlock_info->_line , line_string );
												_this->_read_locked_rdwrlock_string += line_string;
											}
										}	
										_this->_read_locked_rdwrlock_string += "\n";
									}
								}
							}
							_this->_write_locked_rdwrlock_string.clear();
							size_t write_locked_rdwrlock_count = ti->_write_locked_rdwrlock_count;;
							if( write_locked_rdwrlock_count )
							{
								_this->_rux_threads_name.clear();
								_this->_rux_threads_name += ti->_thread_name;
								for( size_t index1 = 0 ; index1 < write_locked_rdwrlock_count ; index1++ )
								{
									XRdWrLockInfo* rdwrlock_info = &ti->_write_locked_rdwrlocks[ index1 ];
									if( rdwrlock_info->_rdwrlock )
									{
										_this->_write_locked_rdwrlock_string += rdwrlock_info->_rdwrlock->_name;
										if( rdwrlock_info->_file )
										{
											_this->_write_locked_rdwrlock_string += " locked by ";
											_this->_write_locked_rdwrlock_string += rdwrlock_info->_file;
											if( rdwrlock_info->_line > 0 )
											{
												_this->_write_locked_rdwrlock_string += ":";
												char line_string[ 128 ] = {0};
												rux::string::uint32_to_string( rdwrlock_info->_line , line_string );
												_this->_write_locked_rdwrlock_string += line_string;
											}
										}	
										_this->_write_locked_rdwrlock_string += "\n";
									}
								}
							}
							_this->_crtsect_string.clear();
							size_t locked_crt_sects_count = ti->_locked_crt_sects_count;;
							if( locked_crt_sects_count )
							{
								for( size_t index1 = 0 ; index1 < locked_crt_sects_count ; index1++ )
								{
									XCrtSectInfo* crt_sect_info = &ti->_locked_crt_sects[ index1 ];
									if( crt_sect_info->_crt_sect )
									{
										_this->_crtsect_string += crt_sect_info->_crt_sect->_name;
										if( crt_sect_info->_file )
										{
											_this->_crtsect_string += " locked by ";
											_this->_crtsect_string += crt_sect_info->_file;
											if( crt_sect_info->_line > 0 )
											{
												_this->_crtsect_string += ":";
												char line_string[ 128 ] = {0};
												rux::string::uint32_to_string( crt_sect_info->_line , line_string );
												_this->_crtsect_string += line_string;
											}
										}	
										_this->_crtsect_string += "\n";
									}
								}
							}
							::rux::byte next = write_locked_rdwrlock_count || read_locked_rdwrlock_count || locked_crt_sects_count || ( labels_count > 0 && labels_count <= 2048 ) ? 1 : 0;
							if( next )
							{
								_this->_rux_threads_name.clear();
								_this->_rux_threads_name += ti->_thread_name;
							}
							ti->unlock();
							if( next )
							{
								if( written == 0 )
									iofile.write_text( "\n%s(process %u)\n=============================\n\n" , now_string , pid );
								iofile.write_text( "[%u][0x%x] %s\n-----------------------------\n" , tid , tid ,
									_this->_rux_threads_name.length() ? _this->_rux_threads_name._str : "thread" );
								if( labels_count > 0 && labels_count <= 2048 )
								{
									iofile.write_text( "Code label:\n%s" , _this->_thread_info_rux_modules_name._str );
									if( _this->_file.length() )
									{
										iofile.write_text( ", %s" , _this->_file._str );
										if( line > 0 )
											iofile.write_text( ":%d" , line );
									}
									if( _this->_function.length() )
										iofile.write_text( ", %s" , _this->_function._str );
									if( _this->_other_module.length() )
									{
										if( _this->_other_function.length() == 0 )
											iofile.write_text( "(%s), " I64u " ms.\n" , _this->_other_module._str , now / 1000000ULL );
										else
											iofile.write_text( "(%s %s), " I64u " ms.\n" , _this->_other_module._str , _this->_other_function._str , now / 1000000ULL );
									}
									else if( _this->_other_function.length() )
										iofile.write_text( "(%s), " I64u " ms.\n" , _this->_other_function._str , now / 1000000ULL );
									else
										iofile.write_text( ", " I64u " ms.\n" , now / 1000000ULL );
								}
								if( _this->_crtsect_string.length() )
									iofile.write_text( "Locked critical sections:\n%s" , _this->_crtsect_string._str );
								if( _this->_write_locked_rdwrlock_string.length() )
									iofile.write_text( "Write locked RdWrLocks:\n%s" , _this->_write_locked_rdwrlock_string._str );
								if( _this->_read_locked_rdwrlock_string.length() )
									iofile.write_text( "Read locked RdWrLocks:\n%s" , _this->_read_locked_rdwrlock_string._str );
								iofile.write_text( "-----------------------------\n\n" );
								written = 1;
							}
						}
						else
							ti->unlock();
						_cs_rux_threads->ReadLock();
					}
				}
			}
			_cs_rux_threads->ReadUnlock();
			if( written == 1 )
				iofile.write_text( "=============================\n\n" );
			iofile.close();
		}
		::chmod( _this->_stable_info_filename._str , 0777 );
	}
};
void XThreadInfo::check_corrupted_memory( void )
{
	while( XInterlocked::CompareExchange( &_gcrefs_to_clean_locked , 1 , 0 ) != 0 )
		::rux::threading::XThread::Sleep( 1 );
	if( _gcrefs_to_clean_count > 0 )
	{
		for( size_t index0 = 0 ; index0 < RUX_MAX_GC_COUNT ; index0++ )
		{
			XGCRef* gcref = _gcrefs_to_clean[ index0 ];
			if( gcref )
				::rux::memory::check_memory( gcref , 0 , sizeof( XGCRef ) );
		}
	}
	XInterlocked::CompareExchange( &_gcrefs_to_clean_locked , 0 , 1 );
};
void XThreadInfo::check_unknown_threads( void )
{
	::rux::uint32 now = ::rux::XTime::GetTickCount();
	_cs_rux_threads->ReadLock();
	if( _rux_threads )
	{
		for( size_t index0 = 0 ; index0 < _rux_threads_count ; index0++ )
		{			
			XThreadInfo* ti = _rux_threads[ index0 ];
			if( ti )
			{
				ti->lock();
				if( ti->_is_unknown && ::XInterlocked::CompareExchange( &ti->_registered , 0 , 0 ) )
				{
					if( now < ti->_activity_time )
						ti->_activity_time = now;
					if( now - ti->_activity_time >= 1000 )
					{
						if( ti->_locked_crt_sects_count == 0 && ti->_write_locked_rdwrlock_count == 0 && ti->_read_locked_rdwrlock_count == 0 )
						{
							_cs_rux_threads->ReadUnlock();
							remove_thread_from_global( index0 );							
							ti->unlock();
							_cs_rux_threads->ReadLock();
						}
						else
							ti->unlock();
					}
					else
						ti->unlock();
				}
				else
					ti->unlock();
			}
		}
	}
	_cs_rux_threads->ReadUnlock();
};