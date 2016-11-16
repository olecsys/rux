#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_memorymanager.h>
#include <xan_gcref.h>
#include <xan_interlocked.h>
#include <xan_type.h>
#include <xan_console.h>
#include <xan_thread.h>
#include <xan_time.h>
#include <xan_io_defines.h>
#include <xan_native_diagnostics.h>
#include <xan_hash.h>
#include <xan_logger.h>
#include <xan_gcref.h>
#include <xan_typemanager.h>
#include <xan_thread_info.h>
#include <xan_task.h>
implement_global_variable( char , _leak_watch_to_class , 32 );
dll_internal ::rux::uint8 _rux_is_memory_leaks = 0;
namespace rux
{
	namespace engine
	{
		dll_internal ::rux::byte _diagnostics_cpu = 0;
		dll_internal ::rux::uint32 _diagnostics_timeout = 60;
		dll_internal ::rux::byte _diagnostic = 1;
		dll_internal size_t _page_size = 0;
	};
};
dll_internal ::rux::uint8 _rux_is_label_delay = 0;
dll_internal ::rux::uint8 _rux_catch_exception = 1;
dll_internal char _rux_search_directories[ 16 ][ 512 ] = { 0 , 0 };
dll_internal ::rux::uint32 _rux_search_directories_count = 0;
dll_internal rux_volatile _rux_old_total_memory_size[ RUX_MAX_MODULES_SIZE ] = { 0 };
dll_internal rux_volatile _rux_total_memory_size[ RUX_MAX_MODULES_SIZE ] = { 0 };
dll_internal rux_volatile _rux_old_total_descriptors_count[ RUX_MAX_MODULES_SIZE ] = { 0 };
dll_internal rux_volatile _rux_total_descriptors_count[ RUX_MAX_MODULES_SIZE ] = { 0 };
dll_internal XMemoryManager* g_rux_memory_manager = NULL;
dll_internal ::rux::rux_exception_event_t _rux_exception_events[ 1024 ];
dll_internal char _rux_exception_events_module_name[ 1024 ][ 1024 ];
dll_internal char _rux_modules_name[ RUX_MAX_MODULES_SIZE ][ 64 ];
implement_global_variable( char , _rux_cpu_filename , 1024 );
dll_internal ::rux::memory::begin_memory_chunk* _first_last_memory = NULL;
dll_internal XMallocArray< ::rux::memory::summary_memory_info* > _detailed_memory_infos;
dll_internal ::rux::int64 _working_set = 0;
dll_internal ::rux::int64 _process_threads_count = 0;
dll_internal ::rux::int64 _process_descriptors_count = 0;
dll_internal ::rux::int64 _virtual_bytes = 0;
dll_internal XThreadInfo** _rux_threads = 0;
dll_internal size_t _rux_threads_count = 0;
dll_internal size_t _cpu_logs_write_index = 0;
dll_internal size_t _cpu_logs_read_index = 0;
dll_internal char _cpu_logs[ RUX_CPU_LOGS_COUNT ][ 1024 ] = { 0 , 0 };
void XMemoryManager::write_cpu_logs( void )
{
	::rux::uint64 size = rux_get_file_size( _rux_cpu_filename );
	if( size >= 1024ULL * 1024ULL )
		rux_remove_file( _rux_cpu_filename );
	::rux::io::file rux_cpu_file( _rux_cpu_filename , XOpenWriteText );
	if( rux_cpu_file.opened() == false )
		rux_cpu_file.open( _rux_cpu_filename , XCreateWriteText );
	if( rux_cpu_file.opened() )
	{
		rux_cpu_file.seek( rux_cpu_file.size() );
		_cs_cpu_logs->WriteLock( NULL , NULL , 0 , 0 );
		while( _cpu_logs_write_index != _cpu_logs_read_index )
		{
			rux_cpu_file.write_text( _cpu_logs[ _cpu_logs_read_index ] );		
			_cpu_logs_read_index++;
			if( _cpu_logs_read_index == RUX_CPU_LOGS_COUNT )
				_cpu_logs_read_index = 0;
		}
		_cs_cpu_logs->WriteUnlock();
		rux_cpu_file.close();
	}
	::chmod( _rux_cpu_filename , 0777 );
};
rux::uint64 XThreadInfo::get_Timeout( ::rux::uint32 index )
{
	::rux::uint64 timeout = 0;
	if( _count > index && index < RUX_MAX_LABELS_COUNT )
	{		
#ifdef __WINDOWS__
		LARGE_INTEGER qpc;
		if( QueryPerformanceCounter( &qpc ) )
			timeout = qpc.QuadPart - _performance[ index ];
		if( QueryPerformanceFrequency( &qpc ) )
		{		
			if( 1000000000ULL >= qpc.QuadPart )
				timeout *= 1000000000ULL / qpc.QuadPart;
			else
				timeout *= (double)1000000000ULL / (double)qpc.QuadPart;
		}
#elif defined( __UNIX__ )
		struct timespec ts = { 0 };
		clock_gettime( CLOCK_MONOTONIC , &ts );
		timeout = ts.tv_sec * 1000000000ULL + ts.tv_nsec - _performance[ index ];
#endif
	}
	return timeout;
};
XMemoryManager::XMemoryManager()
	: _write_locked_rdwrlock_string( 0 , 0 , 0 ) ,
	_thread_info_rux_modules_name( 0 , 0 , 0 ) ,
	_rux_threads_name( 0 , 0 , 0 ) ,
	_file( 0 , 0 , 0 ) ,
	_function( 0 , 0 , 0 ) ,
	_other_module( 0 , 0 , 0 ) ,
	_other_function( 0 , 0 , 0 ) ,
	_crtsect_string( 0 , 0 , 0 ) ,
	_read_locked_rdwrlock_string( 0 , 0 , 0 ) ,
	_stable_info_filename( 0 , 0 , 0 )
{
	_stable_info_filename += ::rux::engine::_globals->_executable_directory;
	_stable_info_filename += "/stable/";
	_stable_info_filename += ::rux::engine::_globals->_executable;
	_stable_info_filename += ".snapshot";
	_info_task_index = SIZE_MAX;
	_last_diagnostics_time = 0ULL;
	_delete_pidfile = 0;
#ifdef __UNIX__
	stack_t signal_stack;
	signal_stack.ss_sp = valloc( 1024 * 1024 );
	_signal_stack = signal_stack.ss_sp;
	signal_stack.ss_flags = 0;
	signal_stack.ss_size = 1024 * 1024;
	sigaltstack( &signal_stack , NULL );
#endif
	_is_network_inited = 0;
	//if( _rux_catch_exception == 1 )
	//	rux::debug::StartCatchException( exception_event );
#ifdef __UNIX__	
	if( ::rux::engine::_globals->_is_core_dump == 1 )
	{
		struct rlimit core_limits;
		core_limits.rlim_cur = core_limits.rlim_max = RLIM_INFINITY;
		setrlimit( RLIMIT_CORE , &core_limits );
	}
#endif	
	char info_filename[ 1024 ] = {0};
	::rux::safe_strncpy( info_filename , ::rux::engine::_globals->_executable_directory , 1023 );
	::rux::safe_strncat( info_filename , "/stable/" , 1023 );

	if(::rux::engine::_diagnostic)
	{
		::rux::io::directory::create(info_filename);
		::chmod(info_filename, 0777);
	}

	::rux::safe_strncat( info_filename , ::rux::engine::_globals->_executable , 1023 );
	::rux::safe_strncat( info_filename , ".snapshot" , 1023 );
	if( rux_is_exists_file( info_filename ) == 1 )
		rux_remove_file( info_filename );

	::rux::safe_strncpy( info_filename , ::rux::engine::_globals->_executable_directory , 1023 );
	::rux::safe_strncat( info_filename , "/memory/" , 1023 );

	if(::rux::engine::_diagnostic)
	{
		::rux::io::directory::create(info_filename);
		::chmod(info_filename, 0777);
	}

	::rux::safe_strncat( info_filename , ::rux::engine::_globals->_executable , 1023 );
	::rux::safe_strncat( info_filename , ".snapshot" , 1023 );
	if( rux_is_exists_file( info_filename ) == 1 )
		rux_remove_file( info_filename );

	::rux::safe_strncpy( info_filename , ::rux::engine::_globals->_executable_directory , 1023 );
	::rux::safe_strncat( info_filename , "/memory/total." , 1023 );
	::rux::safe_strncat( info_filename , ::rux::engine::_globals->_executable , 1023 );
	::rux::safe_strncat( info_filename , ".snapshot" , 1023 );
	if( rux_is_exists_file( info_filename ) == 1 )
		rux_remove_file( info_filename );	
};
void XMemoryManager::start_info_thread( void )
{
	//if( ::rux::engine::_diagnostic )
	//	::rux::task::add( _info_task_index , private_info_thread , this , 1000 , XThreadMode_Repeat );
};
rux::uint8 XMemoryManager::already_executing_in_current_path( void )
{	
	::rux::uint8 executing = 1;
	declare_stack_variable( char , filename , 1024 );
	declare_stack_variable( char , modulename , 1024 );
	::rux::safe_strncpy( filename , ::rux::engine::_globals->_executable_directory , 1023 );
	rux_get_executable_filename_only( modulename );
	rux_concatenation( filename , "/" );
	rux_concatenation( filename , modulename );
	rux_concatenation( filename , ".pid" );	
	::rux::io::file file( filename );
	while( file.opened() == false )
	{
		if( ::rux::io::file::exists( filename ) == 0 )
		{
			executing = 0;
			file.open( filename , XCreateWriteText );
		}
		else
		{
			executing = 1;
			file.open( filename );
		}
	};
	::rux::pid_t cur_pid = ::rux::diagnostics::process::get_process_id();
	if( executing == 1 )
	{
		executing = 0;
		declare_stack_variable( char , pid_string , 64 );
		size_t readen = 0;
		if( file.read_text( pid_string , 64 , readen ) )
		{
			if( pid_string[ readen - 1 ] == 0 )
			{
				::rux::uint64 pid = ::rux::string::to_uint64( pid_string );
				rux_get_executable_filename( modulename );				
				if( ::rux::diagnostics::process::executing( modulename , pid ) )
				{
					if( cur_pid != pid )
						executing = 1;
				}
			}
		}
	}
	if( executing == 0 )
	{		
		_delete_pidfile = 1;
		declare_stack_variable( char , pid_string , 64 );
		::rux::safe_sprintf( pid_string , 64 , I64u , (::rux::uint64)cur_pid );
		file.clear();
		file.write_text( pid_string );
	}
	if( file.opened() )
		file.close();
	::chmod( filename , 0777 );
	return executing;
};
void XMemoryManager::StopThreads( void )
{
	if( ::rux::engine::_diagnostic )
	{
		if( _info_task_index != SIZE_MAX )
			::rux::task::remove( _info_task_index );
	}
};
XMemoryManager::~XMemoryManager()
{	
	StopThreads();
	if( _delete_pidfile == 1 )
	{
		declare_stack_variable( char , filename , 1024 );
		declare_stack_variable( char , modulename , 1024 );
		::rux::safe_strncpy( filename , ::rux::engine::_globals->_executable_directory , 1023 );
		rux_get_executable_filename_only( modulename );
		rux_concatenation( filename , "/" );
		rux_concatenation( filename , modulename );
		rux_concatenation( filename , ".pid" );	
		::rux::io::file::remove( filename );
	}
	network_deinit();
#ifdef __UNIX__
	if( _signal_stack )
		::free( _signal_stack );
#endif	
};
::rux::memory::begin_memory_chunk* XMemoryManager::find_begin_memory_chunk( const void* mem )
{
	_cs_first_last_memory->ReadLock( 0 , 0 , 0 , 0 );
	::rux::memory::begin_memory_chunk* bmc = _first_last_memory;
	while( bmc )
	{
		void* bmc_end = NULL;
		if( bmc->_extended_info )
			bmc_end = ((::rux::uint8*)bmc) + bmc->_size - sizeof( ::rux::memory::extended_memory_info );
		else
			bmc_end = ((::rux::uint8*)bmc) + bmc->_size;		
		if( bmc <= mem && bmc_end >= mem )
			break;
		bmc = bmc->_prev;
	}
	_cs_first_last_memory->ReadUnlock();
	return bmc;
};
void XMemoryManager::add_stack_var( ::rux::memory::begin_memory_chunk* bmc , const char* file , ::rux::uint32 line , char* variable_name , const char* type_name )
{
	if( _rux_is_memory_leaks == 1 )
	{
		bmc->_extended_info = (::rux::memory::extended_memory_info*)_aligned_malloc( sizeof( ::rux::memory::extended_memory_info ) , 32 );
		bmc->_extended_info->_type_name = type_name;
		bmc->_extended_info->_file_name = file;
		if( variable_name )
		{
			if( strlen( variable_name ) > 31 )
			{
				::memcpy( bmc->_extended_info->_variable_name , variable_name , 31 );
				bmc->_extended_info->_variable_name[ 31 ] = 0;
			}
			else
				strcpy( bmc->_extended_info->_variable_name , variable_name );
		}
		else
			bmc->_extended_info->_variable_name[ 0 ] = 0;
		bmc->_extended_info->_line = line;
		bmc->_size += sizeof( ::rux::memory::extended_memory_info );
	}
	else
		bmc->_extended_info = NULL;
	_cs_first_last_memory->WriteLock();
	if( _first_last_memory )		
		_first_last_memory->_next = bmc;
	bmc->_prev = _first_last_memory;
	_first_last_memory = bmc;		
	_cs_first_last_memory->WriteUnlock();
};
void XMemoryManager::remove_stack_var( ::rux::memory::begin_memory_chunk* bmc )
{
	if( _cs_first_last_memory )
		_cs_first_last_memory->WriteLock();
	if( bmc->_next )
	{
		::rux::memory::check_memory( bmc->_next );
		bmc->_next->_prev = bmc->_prev;
	}
	else
		_first_last_memory = bmc->_prev;
	if( bmc->_prev )
	{
		::rux::memory::check_memory( bmc->_prev );
		bmc->_prev->_next = bmc->_next;
	}
	if( _first_last_memory )
		::rux::memory::check_memory( _first_last_memory );
	bmc->_prev = 0;
	bmc->_next = 0;
	::rux::byte wait_for_delete = 0;
	if( bmc->_deleted != 0 )
		wait_for_delete = 1;
	if( _cs_first_last_memory )
		_cs_first_last_memory->WriteUnlock();
	if( wait_for_delete )
	{
		for( ; ; )
		{
			if( _cs_first_last_memory )
				_cs_first_last_memory->WriteLock();
			if( bmc->_deleted == 0 )
			{
				if( _cs_first_last_memory )
					_cs_first_last_memory->WriteUnlock();
				break;
			}
			else
			{
				if( _cs_first_last_memory )
					_cs_first_last_memory->WriteUnlock();
				::rux::threading::XThread::Sleep( 1 );
			}			
		}
	}
	if( bmc->_extended_info )
		::rux::engine::rux_free_dummy( bmc->_extended_info );
};
::rux::int32 XMemoryManager::get_memory_module_index( void* memory )
{
	::rux::uint8* ptr = (::rux::uint8*)memory;
	::rux::memory::begin_memory_chunk* bmc = (::rux::memory::begin_memory_chunk*)( ptr - sizeof( ::rux::memory::begin_memory_chunk ) );
	if( bmc->_diagnostics )
		return bmc->_module_index;
	else
	{
		size_t diagnostics_size = sizeof( ::rux::memory::begin_memory_chunk ) - cpp_offsetof( ::rux::memory::begin_memory_chunk , _diagnostics );
		ptr -= ( sizeof( ::rux::int32 ) + sizeof( size_t ) + diagnostics_size );
		return *((::rux::int32*)ptr);
	}
};
size_t XMemoryManager::get_memory_size( void* memory )
{
	::rux::uint8* ptr = (::rux::uint8*)memory;
	::rux::memory::begin_memory_chunk* bmc = (::rux::memory::begin_memory_chunk*)( ptr - sizeof( ::rux::memory::begin_memory_chunk ) );
	if( bmc->_diagnostics == 1 )
	{
		size_t size = bmc->_size - sizeof( ::rux::memory::begin_memory_chunk ) - sizeof( ::rux::memory::end_memory_chunk );
		if( bmc->_extended_info )
			size -= sizeof( ::rux::memory::extended_memory_info );
		return size;
	}
	else
	{
		size_t diagnostics_size = sizeof( ::rux::memory::begin_memory_chunk ) - cpp_offsetof( ::rux::memory::begin_memory_chunk , _diagnostics );
		ptr -= ( sizeof( size_t ) + diagnostics_size );
		return *((size_t*)ptr);
	}
};
void XMemoryManager::get_memory_extended_info( void* memory , ::rux::memory::extended_memory_info*& extended_memory_info )
{
	::rux::uint8* ptr = (::rux::uint8*)memory;
	::rux::memory::begin_memory_chunk* bmc = (::rux::memory::begin_memory_chunk*)( ptr - sizeof( ::rux::memory::begin_memory_chunk ) );
	if( bmc->_diagnostics == 1 )
		extended_memory_info = bmc->_extended_info;
	else
		extended_memory_info = 0;
};
void* XMemoryManager::Alloc( size_t alloc_size , const char* type_name , const char* file_name , ::rux::int32 line , ::rux::int32 module_index , const char* variable_name )
{
	void* mem_ptr = NULL;
	if( ::rux::engine::_diagnostic )
	{
		size_t size = sizeof( ::rux::memory::begin_memory_chunk ) + alloc_size + sizeof( ::rux::memory::end_memory_chunk );						
		::rux::byte use_mmap = 0;
		if( ::rux::engine::_globals && ::rux::engine::_globals->_protect_markers_files
			&& ::rux::engine::_globals->_protect_markers_lines && file_name && ::rux::engine::_globals->_protect_markers_files->Count() > 0 
			&& ::rux::engine::_globals->_protect_markers_files->Count() == ::rux::engine::_globals->_protect_markers_lines->Count() 
			&& ::rux::engine::_globals->_protect_markers_lines->_array_ptr )
		{
			::rux::int64 index = ::rux::string::last_index_of( file_name , "/" );
			if( index == -1 )
				index = ::rux::string::last_index_of( file_name , "\\" );
			if( index == -1 )
				index = 0;
			else
				index++;
			for( size_t index0 = 0 ; index0 < ::rux::engine::_globals->_protect_markers_files->Count() ; index0++ )
			{
				if( ::rux::engine::_globals->_protect_markers_lines->operator[]( index0 ) == line 
					&& strcmp( &file_name[ index ] , ::rux::engine::_globals->_protect_markers_files->operator[]( index0 ) ) == 0 )
				{
					use_mmap = 1;
					break;
				}
			}
		}
		char* ptr = 0 , * map_guard = 0;
		if( use_mmap )
		{
			size_t page_size = ::rux::engine::_page_size;
			for( ; ; )
			{
				if( size > page_size )
					page_size += ::rux::engine::_page_size;
				else
					break;
			}
			page_size += 2 * ::rux::engine::_page_size;
#ifdef __WINDOWS__
			ptr = (char*)VirtualAlloc( NULL , page_size , MEM_COMMIT, PAGE_READWRITE );
			if( ptr )
#elif defined( __LINUX__ )
			ptr = (char*)::_aligned_malloc( page_size , ::rux::engine::_page_size );
			if( ptr )
			//ptr = (char*)mmap( 0 , page_size , PROT_READ | PROT_WRITE , MAP_PRIVATE | MAP_ANONYMOUS , -1 , 0 );
			//if( ptr && ptr != (void*)-1 )
#endif	
			{
				map_guard = ptr;
#ifdef __WINDOWS__
				DWORD old_protect = 0;
				if( VirtualProtect( ptr , ::rux::engine::_page_size , PAGE_READONLY , &old_protect ) == 0 )
				{
					VirtualFree( ptr , 0 , MEM_RELEASE );
					char error_string[ 1024 ] = {0};
					//XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError , error_string );
#elif defined( __LINUX__ )
				if( mprotect( ptr , ::rux::engine::_page_size , PROT_READ ) == -1 )
				{
					::rux::engine::rux_free_dummy( ptr );
					//munmap( ptr , page_size );					
#endif		
					printf( "%s\n" , "cannot mprotect(1)" );
					ptr = 0;
					map_guard = 0;
				}
				else
				{
#ifdef __WINDOWS__
					if( VirtualProtect( ptr , ::rux::engine::_page_size , PAGE_READWRITE , &old_protect ) == 0 )
					{
						VirtualFree( ptr , 0 , MEM_RELEASE );
						char error_string[ 1024 ] = {0};
						//XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError , error_string );
#elif defined( __LINUX__ )
					if( mprotect( ptr , ::rux::engine::_page_size , PROT_READ | PROT_WRITE ) == -1 )
					{
						::rux::engine::rux_free_dummy( ptr );
						//munmap( ptr , page_size );					
#endif		
						printf( "%s\n" , "cannot mprotect(2)" );
						ptr = 0;
						map_guard = 0;
					}
					else
					{
						ptr += ( page_size - ::rux::engine::_page_size );
#ifdef __WINDOWS__
						if( VirtualProtect( ptr , ::rux::engine::_page_size , PAGE_NOACCESS , &old_protect ) == 0 )
#elif defined( __LINUX__ )
						if( mprotect( ptr , ::rux::engine::_page_size , PROT_NONE ) == -1 )
#endif		
						{
							ptr -= ( page_size - ::rux::engine::_page_size );
#ifdef __WINDOWS__
							VirtualFree( ptr , 0 , MEM_RELEASE );
							char error_string[ 1024 ] = {0};
							//XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError , error_string );
#elif defined( __LINUX__ )
							::rux::engine::rux_free_dummy( ptr );
							//munmap( ptr , page_size );
#endif
							printf( "%s\n" , "cannot mprotect(3)" );
							ptr = 0;
							map_guard = 0;
						}
						else
							ptr = map_guard + ( ::rux::engine::_page_size - cpp_offsetof( ::rux::memory::begin_memory_chunk , _prev ) );
					}
				}
			}
			else
				ptr = 0;
		}
		if( ptr == 0 )
		{
			use_mmap = 0;
			ptr = (char*)::_aligned_malloc( size , 32 );
		}
		if( ptr == 0 )
		{
			write_heap_is_not_enough( module_index );
			rux::int32 exception = 172;
			exception = 2 / ( exception - 172 );
			printf( "exception %d" , exception );
		}
		::memset( ptr , 0 , size );
		::rux::memory::begin_memory_chunk* bmc = (::rux::memory::begin_memory_chunk*)ptr;
		//if( ::rux::engine::_globals && ::rux::engine::_globals->_log_level == ::rux::log::XEnum_LogLevel_Verbose )
		//	::rux::log::WriteTrace( "%s, %s:%d, %p" , __FUNCTION__ , __FILE__ , __LINE__ , bmc );
		bmc->_mmap = use_mmap;
		bmc->_stack = 0;
		bmc->_deleted = 0;
		bmc->_diagnostics = 1;
		if( _rux_is_memory_leaks == 1 )
		{
			bmc->_extended_info = (::rux::memory::extended_memory_info*)_aligned_malloc( sizeof( ::rux::memory::extended_memory_info ) , 32 );
			bmc->_extended_info->_type_name = type_name;
			bmc->_extended_info->_file_name = file_name;
			if( variable_name )
			{
				if( strlen( variable_name ) > 31 )
				{
					::memcpy( bmc->_extended_info->_variable_name , variable_name , 31 );
					bmc->_extended_info->_variable_name[ 31 ] = 0;
				}
				else
					strcpy( bmc->_extended_info->_variable_name , variable_name );
			}
			else
				bmc->_extended_info->_variable_name[ 0 ] = 0;
			bmc->_extended_info->_line = line;
			size += sizeof( ::rux::memory::extended_memory_info );
		}
		else
			bmc->_extended_info = NULL;
		XInterlocked::ExchangeAdd( &_rux_total_memory_size[ module_index ] , size );
		bmc->_marker = begin_memory_marker;		
		bmc->_size = size;
		bmc->_module_index = module_index;
		bmc->_next = NULL;
		ptr += sizeof( ::rux::memory::begin_memory_chunk );
		mem_ptr = ptr;
		ptr += alloc_size;
		::rux::memory::end_memory_chunk* emc = (::rux::memory::end_memory_chunk*)ptr;
		emc->_marker = end_memory_marker;		
		
		_cs_first_last_memory->WriteLock( __FILE__ , __FUNCTION__ , __LINE__ , 1 );
		if( _first_last_memory )		
		{
			::rux::memory::check_memory( _first_last_memory );
			_first_last_memory->_next = bmc;
		}
		bmc->_prev = _first_last_memory;
		_first_last_memory = bmc;		
		_cs_first_last_memory->WriteUnlock();
		if( map_guard )
		{
#ifdef __WINDOWS__
			DWORD old_protect = 0;
			if( VirtualProtect( map_guard , ::rux::engine::_page_size , PAGE_READONLY , &old_protect ) == 0 )
#elif defined( __LINUX__ )
			if( mprotect( map_guard , ::rux::engine::_page_size , PROT_READ ) == -1 )
#endif
				printf( "%s\n" , "cannot mprotect(4)" );
			else
				printf( "%s\n" , "we use mmap/VirtualAlloc" );
		}
	}
	else
	{
		size_t diagnostics_size = sizeof( ::rux::memory::begin_memory_chunk ) - cpp_offsetof( ::rux::memory::begin_memory_chunk , _diagnostics );
		size_t size = alloc_size + sizeof( ::rux::int32 ) + sizeof( size_t ) + diagnostics_size;
		char* ptr = NULL;
		for( ; ; )
		{
			ptr = (char*)::calloc( size , 1 );
			if( ptr )
				break;
			else
				::rux::threading::XThread::Sleep( 10 );
		}
		::memset( ptr , 0 , size );
		memcpy( ptr , &module_index , sizeof( ::rux::int32 ) );
		ptr += sizeof( ::rux::int32 );
		memcpy( ptr , &alloc_size , sizeof( size_t ) );
		mem_ptr = ptr + sizeof( size_t ) + diagnostics_size;
	}
	//if( ::rux::engine::_globals && ::rux::engine::_globals->_log_level == ::rux::log::XEnum_LogLevel_Verbose )
	//	::rux::log::WriteTrace( "%s, %s:%d, %p" , __FUNCTION__ , __FILE__ , __LINE__ , mem_ptr );
	return mem_ptr;
};
void XMemoryManager::Free( void* mem_ptr )
{		
	::rux::memory::check_memory( mem_ptr );
	//if( ::rux::engine::_globals && ::rux::engine::_globals->_log_level == ::rux::log::XEnum_LogLevel_Verbose )
	//	::rux::log::WriteTrace( "%s, %s:%d, %p" , __FUNCTION__ , __FILE__ , __LINE__ , mem_ptr );
	::rux::uint8* ptr = (::rux::uint8*)mem_ptr;
	::rux::memory::begin_memory_chunk* bmc = (::rux::memory::begin_memory_chunk*)( ptr - sizeof( ::rux::memory::begin_memory_chunk ) );
	if( bmc->_diagnostics == 1 )
	{
		//if( ::rux::engine::_globals && ::rux::engine::_globals->_log_level == ::rux::log::XEnum_LogLevel_Verbose )
		//	::rux::log::WriteTrace( "%s, %s:%d, %p" , __FUNCTION__ , __FILE__ , __LINE__ , bmc );
		ptr = (::rux::byte*)bmc;
		XInterlocked::ExchangeSubtract( &_rux_total_memory_size[ bmc->_module_index ] , bmc->_size );
		_cs_first_last_memory->WriteLock( __FILE__ , __FUNCTION__ , __LINE__ , 1 );
		if( bmc->_next )
		{
			::rux::memory::check_memory( bmc->_next );
			bmc->_next->_prev = bmc->_prev;
		}
		else
			_first_last_memory = bmc->_prev;
		if( bmc->_prev )
		{
			::rux::memory::check_memory( bmc->_prev );
			bmc->_prev->_next = bmc->_next;
		}
		if( _first_last_memory )
			::rux::memory::check_memory( _first_last_memory );
		bmc->_prev = 0;
		bmc->_next = 0;
		::rux::byte wait_for_delete = 0;
		if( bmc->_deleted != 0 )
			wait_for_delete = 1;
		_cs_first_last_memory->WriteUnlock();
		if( wait_for_delete )
		{
			for( ; ; )
			{
				if( _cs_first_last_memory )
					_cs_first_last_memory->WriteLock();
				if( bmc->_deleted == 0 )
				{
					if( _cs_first_last_memory )
						_cs_first_last_memory->WriteUnlock();
					break;
				}
				else
				{
					if( _cs_first_last_memory )
						_cs_first_last_memory->WriteUnlock();
					::rux::threading::XThread::Sleep( 1 );
				}			
			}
		}
		size_t size = bmc->_size;
		if( bmc->_extended_info )
		{
			::rux::engine::rux_free_dummy( bmc->_extended_info );
			size -= sizeof( ::rux::memory::extended_memory_info );
		}
		if( bmc->_mmap )
		{
			size_t page_size = ::rux::engine::_page_size;
			for( ; ; )
			{
				if( bmc->_size > page_size )
					page_size += ::rux::engine::_page_size;
				else
					break;
			}
			page_size += 2 * ::rux::engine::_page_size;
			ptr -= ( ::rux::engine::_page_size - cpp_offsetof( ::rux::memory::begin_memory_chunk , _prev ) );
#ifdef __WINDOWS__
			DWORD old_protect = 0;
			if( VirtualProtect( ptr , ::rux::engine::_page_size , PAGE_READWRITE , &old_protect ) != 0 )
#elif defined( __LINUX__ )
			if( mprotect( ptr , ::rux::engine::_page_size , PROT_READ | PROT_WRITE ) != -1 )
#endif
			{
				ptr += ( page_size - ::rux::engine::_page_size );
#ifdef __WINDOWS__
				if( VirtualProtect( ptr , ::rux::engine::_page_size , PAGE_READWRITE , &old_protect ) != 0 )
#elif defined( __LINUX__ )
				if( mprotect( ptr , ::rux::engine::_page_size , PROT_READ | PROT_WRITE ) != -1 )
#endif
				{
					ptr -= ( page_size - ::rux::engine::_page_size );
#ifdef __WINDOWS__
					VirtualFree( ptr , 0 , MEM_RELEASE );
#elif defined( __LINUX__ )
					//munmap( ptr , page_size );
					::rux::engine::rux_free_dummy( ptr );
#endif
				}
			}
		}
		else
			::rux::engine::rux_free_dummy( ptr );
	}
	else
	{
		size_t diagnostics_size = sizeof( ::rux::memory::begin_memory_chunk ) - cpp_offsetof( ::rux::memory::begin_memory_chunk , _diagnostics );
		ptr = (::rux::uint8*)mem_ptr;
		ptr -= ( sizeof( ::rux::int32 ) + sizeof( size_t ) + diagnostics_size );
		::rux::engine::rux_free_dummy( ptr );
	}
};
void XMemoryManager::memory_unload_module( ::rux::int32 module_index )
{
	if( g_type_manager )
		g_type_manager->FreeTypes( module_index );
	XThreadInfo::CollectGCRefs( 0 , SIZE_MAX );	
	_cs_rux_threads->ReadLock();
	if( _rux_threads )
	{
		for( size_t index0 = 0 ; index0 < _rux_threads_count ; index0++ )
		{
			XThreadInfo* ti = _rux_threads[ index0 ];
			if( ti )
			{
				ti->lock();
				_cs_rux_threads->ReadUnlock();
				if( ::XInterlocked::CompareExchange( &ti->_registered , 0 , 0 ) )
				{
					ti->CollectModuleGCRefs( module_index );
					ti->CollectCrtSectInfos( module_index );
				}
				ti->unlock();
				_cs_rux_threads->ReadLock();
			}
		}
	}
	_cs_rux_threads->ReadUnlock();
	declare_stack_variable( char , executable , 1024 );
	rux_get_executable_directory( executable );
	declare_stack_variable( char , filename , 2048 );
	rux_concatenation( filename , executable , "/memory/" );		
	_cs_rux_modules_name->ReadLock();
	rux_concatenation( filename , _rux_modules_name[ module_index ] );			
	_cs_rux_modules_name->ReadUnlock();
	rux_concatenation( filename , ".leaks" );	
	::rux::pid_t tid = ::rux::threading::XThread::get_CurrentThreadId();
	{
		_cs_rux_threads->ReadLock();
		XThreadInfo* ti = XThreadInfo::find_thread( tid );
		if( ti )
		{
			ti->lock();
			ti->RemoveModuleLabels( module_index );
			ti->unlock();
		}
		_cs_rux_threads->ReadUnlock();
	}
	if( ::rux::engine::_diagnostic )
	{
		::rux::uint64 now = rux_get_now_as_local_unix_time();
		declare_stack_variable( char , now_string , 64 );
		rux_unix_time_to_string( now , now_string , 64 );		
		::rux::uint64 total_memory_size = XInterlocked::CompareExchange( &_rux_total_memory_size[ module_index ] , 0 , 0 );
		::rux::uint64 total_descriptors_count = XInterlocked::CompareExchange( &_rux_total_descriptors_count[ module_index ] , 0 , 0 );
		::rux::memory::begin_memory_chunk* detailed_memory_infos_bmc = NULL;
		if( module_index == 0 )
		{
			if( _detailed_memory_infos._array_ptr )
				detailed_memory_infos_bmc = XMemoryManager::find_begin_memory_chunk( _detailed_memory_infos._array_ptr );
			if( detailed_memory_infos_bmc )
				total_memory_size -= detailed_memory_infos_bmc->_size;
		}
		if( total_memory_size > 0ULL || total_descriptors_count > 0ULL )
		{
			::rux::io::file file( filename , XOpenWriteText );
			if( file.opened() == false )
				file.open( filename , XCreateWriteText );
			if( file.opened() )
			{
				file.seek( file.size() );
				::rux::uint32 pid = (::rux::uint32)::rux::diagnostics::process::get_process_id();
				file.write_text( "\n%s(process %u)\n=============================\n\n" , now_string , pid );
				if( _rux_is_memory_leaks == 1 )
				{			
					::rux::memory::summary_memory_info* sum = NULL;				
					XMallocArray< ::rux::memory::summary_memory_info* > summary_memory_infos;
					_cs_first_last_memory->WriteLock();
					::rux::memory::begin_memory_chunk* bmc = _first_last_memory;
					while( bmc )
					{
						if( bmc->_module_index == module_index )
						{
							if( bmc->_stack == 0 )
							{
								::rux::memory::begin_memory_chunk* summary_memory_infos_bmc = NULL;
								if( module_index == 0 && summary_memory_infos._array_ptr )
									summary_memory_infos_bmc = XMemoryManager::find_begin_memory_chunk( summary_memory_infos._array_ptr );
								if( summary_memory_infos_bmc != bmc && detailed_memory_infos_bmc != bmc )
								{
									if( bmc->_extended_info )
									{
										sum = NULL;
										for( size_t index0 = 0 ; index0 < summary_memory_infos.Count() ; index0++ )
										{
											if( summary_memory_infos[ index0 ]->_file_name == bmc->_extended_info->_file_name
												&& summary_memory_infos[ index0 ]->_type_name == bmc->_extended_info->_type_name
												&& summary_memory_infos[ index0 ]->_line == bmc->_extended_info->_line
												&& strcmp( summary_memory_infos[ index0 ]->_variable_name , bmc->_extended_info->_variable_name ) == 0 )
											{
												sum = summary_memory_infos[ index0 ];
												break;
											}
										}
										if( sum == NULL )
										{
											sum = (::rux::memory::summary_memory_info*)_aligned_malloc( sizeof( ::rux::memory::summary_memory_info ) ,32 );
											sum->_file_name = bmc->_extended_info->_file_name;
											sum->_type_name = bmc->_extended_info->_type_name;
											sum->_line = bmc->_extended_info->_line;
											sum->_size = 0;
											sum->_count = 0;
											sum->_addresses = 0;
											sum->_addresses_count = 0;
											strcpy( sum->_variable_name , bmc->_extended_info->_variable_name );
											summary_memory_infos.Add( sum );
										}
										void* ptr = ((char*)bmc) + sizeof( ::rux::memory::begin_memory_chunk );
										::rux::memory::copy< void* , 1 >( sum->_addresses , sum->_addresses_count , sum->_addresses_count , &ptr , 1 );
										sum->_size += bmc->_size;
										sum->_count++;
									}
								}
							}
							if( module_index != 0 )
							{
								::rux::memory::begin_memory_chunk* temp_bmc = bmc;
								bmc = bmc->_prev;
								if( temp_bmc->_next )
									temp_bmc->_next->_prev = temp_bmc->_prev;
								else
									_first_last_memory = temp_bmc->_prev;
								if( temp_bmc->_prev )
									temp_bmc->_prev->_next = temp_bmc->_next;
								temp_bmc->_prev = 0;
								temp_bmc->_next = 0;
							}
							else
								bmc = bmc->_prev;
						}
						else
							bmc = bmc->_prev;
					}	
					for( size_t index0 = 0 ; index0 < _detailed_memory_infos.Count() ; index0++ )
					{
						if( _detailed_memory_infos[ index0 ]->_module_index == module_index )
						{
							::rux::engine::rux_free_dummy( _detailed_memory_infos[ index0 ] );
							_detailed_memory_infos.RemoveAt( index0 );
							index0--;
						}
					}
					_cs_first_last_memory->WriteUnlock();				
					for( size_t index0 = 0 ; index0 < summary_memory_infos.Count() ; index0++ )
					{
						file.write_text( "%s %s, %s:%d, " I64u " allocs, " I64u " bytes\n-----------------------------\n" ,
							summary_memory_infos[ index0 ]->_type_name , 
							summary_memory_infos[ index0 ]->_variable_name ,
							summary_memory_infos[ index0 ]->_file_name ,
							summary_memory_infos[ index0 ]->_line ,
							(::rux::uint64)summary_memory_infos[ index0 ]->_count ,
							(::rux::uint64)summary_memory_infos[ index0 ]->_size );
						for( size_t index1 = 0 ; index1 < summary_memory_infos[ index0 ]->_addresses_count ; index1++ )
							file.write_text( "%u %p\n" , (::rux::uint32)( index1 + 1 ) , summary_memory_infos[ index0 ]->_addresses[ index1 ] );
						file.write_text( "\n-----------------------------\n" );
						::rux::engine::free_mem( summary_memory_infos[ index0 ]->_addresses );
						::rux::engine::rux_free_dummy( summary_memory_infos[ index0 ] );
					}
				}
				file.write_text( "Total memory:" I64u " bytes\n\nTotal descriptors:" I64u "\n\n=============================\n" , total_memory_size , total_descriptors_count );
				file.close();
			}
			::chmod( filename , 0777 );
		}
	}
	_cs_first_last_memory->WriteLock();
	::rux::memory::begin_memory_chunk* bmc = _first_last_memory;
	while( bmc )
	{
		if( bmc->_module_index == module_index )
		{
			::rux::memory::begin_memory_chunk* temp_bmc = bmc;
			bmc = bmc->_prev;
			if( temp_bmc->_next )
				temp_bmc->_next->_prev = temp_bmc->_prev;
			else
				_first_last_memory = temp_bmc->_prev;
			if( temp_bmc->_prev )
				temp_bmc->_prev->_next = temp_bmc->_next;
		}
		else
			bmc = bmc->_prev;
	}
	_cs_first_last_memory->WriteUnlock();
	_cs_rux_modules_name->WriteLock();
	/*if( ::rux::log::_logger )
		::rux::log::_logger->unregister_module( module_index );*/
	_rux_modules_name[ module_index ][ 0 ] = 0;
	_cs_rux_modules_name->WriteUnlock();
	if( _cs_rux_threads && _rux_threads )
	{
		for( ; ; )
		{
			::rux::uint8 wait = 0;
			_cs_rux_threads->ReadLock();
			for( size_t index0 = 0 ; index0 < _rux_threads_count ; index0++ )
			{
				XThreadInfo* ti = _rux_threads[ index0 ];
				if( ti )
				{
					ti->lock();
					if( ::XInterlocked::CompareExchange( &ti->_registered , 0 , 0 ) )
					{
						XGCRef* gcref = ti->_gcref;
						_cs_rux_threads->ReadUnlock();
						if( gcref && module_index == XMemoryManager::get_memory_module_index( gcref ) )
						{
							wait = 1;
							ti->unlock();
							_cs_rux_threads->ReadLock();
							break;
						}
						ti->unlock();
						_cs_rux_threads->ReadLock();
					}
					else
						ti->unlock();
				}
			}
			_cs_rux_threads->ReadUnlock();
			if( wait == 0 )
				break;
			::rux::threading::XThread::Sleep( 1 );
		}
	}
};
void XMemoryManager::register_exception_event( const char* module_name , ::rux::rux_exception_event_t exception_event )
{
	_rux_cs_exception_events->WriteLock();
	for( ::rux::uint32 index0 = 0 ; index0 < 1024 ; index0++ )
	{
		if( _rux_exception_events[ index0 ] == NULL )
		{
			_rux_exception_events[ index0 ] = exception_event;
			size_t len = strlen( module_name );
			if( len > 0 )
			{
				if( len >= 1024 )
				{
					::memcpy( _rux_exception_events_module_name[ index0 ] , module_name , 1023 );
					_rux_exception_events_module_name[ index0 ][ 1023 ] = '\0';
				}
				else
					::memcpy( _rux_exception_events_module_name[ index0 ] , module_name , len + 1 );
			}
			break;
		}
	}
	_rux_cs_exception_events->WriteUnlock();
};
void XMemoryManager::unregister_exception_event( ::rux::rux_exception_event_t exception_event )
{
	_rux_cs_exception_events->WriteLock();
	for( ::rux::uint32 index0 = 0 ; index0 < 1024 ; index0++ )
	{
		if( _rux_exception_events[ index0 ] == exception_event )
		{
			_rux_exception_events[ index0 ] = NULL;
			break;
		}
	}
	_rux_cs_exception_events->WriteUnlock();
};
void XMemoryManager::exception_event( ::rux::int32 tid )
{
	_cs_rux_threads->ReadLock();
	if( _rux_threads )
	{
		for( size_t thread_index = 0 ; thread_index < _rux_threads_count ; thread_index++ )
		{
			XThreadInfo* ti = _rux_threads[ thread_index ];
			if( ti && ti->_tid == tid && ::XInterlocked::CompareExchange( &ti->_registered , 0 , 0 ) )
			{
				ti->lock();
				_cs_rux_threads->ReadUnlock();
				char tid_string[ 64 ] = {0};
				rux::string::uint32_to_string( tid , tid_string );
				char filename[ 4096 ] = {0};
				::rux::safe_strncpy( filename , ::rux::engine::_globals->_executable_directory , 4095 );
				::rux::safe_strncat( filename , "/rux." , 4095 );	
				::rux::safe_strncat( filename , ::rux::engine::_globals->_executable , 4095 );	
				::rux::safe_strncat( filename , ".exception" , 4095 );	
				char except_lines[ 32768 ] = {0};
				::rux::uint64 now = rux_get_now_as_local_unix_time();
				char now_string[ 64 ] = {0};
				rux_unix_time_to_string( now , now_string , 64 );
				::rux::safe_strncpy( except_lines , "======= " , 32767 );
				::rux::safe_strncat( except_lines , now_string , 32767 );
				::rux::safe_strncat( except_lines , "	thread " , 32767 );
				::rux::safe_strncat( except_lines , tid_string , 32767 );
				if( ti->_thread_name[ 0 ] != 0 )
				{
					::rux::safe_strncat( except_lines , " '" , 32767 );
					::rux::safe_strncat( except_lines , ti->_thread_name , 32767 );
					::rux::safe_strncat( except_lines , "'" , 32767 );
				}
				::rux::safe_strncat( except_lines , " =======\nLabels:\n" , 32767 );
				if( ti->_count > 0 )
				{
					::rux::uint32 index2 = 0;
					for( ::rux::uint32 index1 = ti->_count - 1 ; index1 >= 0 ; index1-- )
					{
						rux::string::uint32_to_string( index2 , tid_string );
						index2++;
						::rux::safe_strncat( except_lines , tid_string , 32767 );
						::rux::safe_strncat( except_lines , " : file '" , 32767 );
						if( ti->_filenames[ index1 ] )
							::rux::safe_strncat( except_lines , ti->_filenames[ index1 ] , 32767 );
						else
							::rux::safe_strncat( except_lines , "?????" , 32767 );
						::rux::safe_strncat( except_lines , "', function '" , 32767 );
						if( ti->_functions[ index1 ] )
							::rux::safe_strncat( except_lines , ti->_functions[ index1 ] , 32767 );
						else
							::rux::safe_strncat( except_lines , "?????" , 32767 );
						::rux::safe_strncat( except_lines , "', " , 32767 );
						if( ti->_other_modules[ index1 ] )				
						{
							::rux::safe_strncat( except_lines , "other module '" , 32767 );
							::rux::safe_strncat( except_lines , ti->_other_modules[ index1 ] , 32767 );
							::rux::safe_strncat( except_lines , "', " , 32767 );
						}
						if( ti->_other_functions[ index1 ] )				
						{
							::rux::safe_strncat( except_lines , "other function '" , 32767 );
							::rux::safe_strncat( except_lines , ti->_other_functions[ index1 ] , 32767 );
							::rux::safe_strncat( except_lines , "', " , 32767 );
						}
						::rux::safe_strncat( except_lines , " line " , 32767 );
						rux::string::uint32_to_string( ti->_lines[ index1 ] , tid_string );
						::rux::safe_strncat( except_lines , tid_string , 32767 );
						::rux::safe_strncat( except_lines , ", module '" , 32767 );		
						::rux::safe_strncat( except_lines , _rux_modules_name[ ti->_module_index ] , 32767 );
						::rux::safe_strncat( except_lines , "'\n" , 32767 );
						if( index1 == 0 )
							break;
					}
				}
				rux_clear_and_write_to_file( filename , except_lines );
				ti->unlock();
				_cs_rux_threads->ReadLock();
				break;
			}
		}
	}
	_cs_rux_threads->ReadUnlock();
	for( size_t index0 = 0 ; index0 < 1024 ; index0++ )
	{
		if( _rux_exception_events[ index0 ] )
		{
			if( _rux_exception_events_module_name[ index0 ][ 0 ] != 0 )
			{
				::rux::XConsole::WriteToStdOut( "before call " );
				::rux::XConsole::WriteToStdOut( _rux_exception_events_module_name[ index0 ] );
				::rux::XConsole::WriteToStdOut( "\n" );
				_rux_exception_events[ index0 ]( tid );
				::rux::XConsole::WriteToStdOut( "after call " );
				::rux::XConsole::WriteToStdOut( _rux_exception_events_module_name[ index0 ] );
				::rux::XConsole::WriteToStdOut( "\n" );
			}
		}
	}
};
void XMemoryManager::network_init( void )
{
	if( _is_network_inited == 0 )
	{
#ifdef __WINDOWS__
		::rux::uint16 version_requested = MAKEWORD( 2 , 2 );
		WSADATA wsa_data;			
		::rux::int32 res = ::rux::engine::_globals->_ws2_32_module.WSAStartup( version_requested , &wsa_data );
		if( res == 0 )
#endif
			_is_network_inited = 1;
	}
};
void XMemoryManager::network_deinit( void )
{
	if( _is_network_inited == 1 )
	{
#ifdef __WINDOWS__
		::rux::engine::_globals->_ws2_32_module.WSACleanup();
#endif
		_is_network_inited = 0;
	}
};
void XMemoryManager::reregister_handler( void )
{
	//if( _rux_catch_exception == 1 )
	//	::rux::debug::StartCatchException( exception_event );
};
#ifdef __WINDOWS__
void XMemoryManager::add_descriptor( ::rux::int32 module_index , const char* file_name_ptr , HANDLE handle , ::rux::int32 line )
#elif defined( __UNIX__ )
void XMemoryManager::add_descriptor( ::rux::int32 module_index , const char* file_name_ptr , ::rux::int32 handle , ::rux::int32 line )
#endif
{
	if( ::rux::engine::_diagnostic )
	{
		if( _rux_is_memory_leaks == 1 )
		{
		}
		//XInterlocked::Increment( &_rux_total_memory_size[ module_index ] );
	}
};
#ifdef __WINDOWS__
void XMemoryManager::remove_descriptor( HANDLE handle , ::rux::int32 module_index )
#elif defined( __UNIX__ )
void XMemoryManager::remove_descriptor( ::rux::int32 handle , ::rux::int32 module_index )
#endif
{
	if( ::rux::engine::_diagnostic )
	{
		if( _rux_is_memory_leaks == 1 )
		{			
		}
		//else
		//XInterlocked::Decrement( &_rux_total_memory_size[ module_index ] );
	}
};
void XMemoryManager::add_memory( void* ptr , size_t size , ::rux::int32 module_index , const char* file_name_ptr , ::rux::int32 line )
{
	if( ::rux::engine::_diagnostic && module_index < RUX_MAX_MODULES_SIZE )
	{
		if( _rux_is_memory_leaks == 1 )
		{
		}
		else
			XInterlocked::ExchangeAdd( &_rux_total_memory_size[ module_index ] , size );
	}
};
void XMemoryManager::remove_memory( void* ptr , size_t size , ::rux::int32 module_index )
{
	if( ::rux::engine::_diagnostic && module_index < RUX_MAX_MODULES_SIZE )
	{
		if( _rux_is_memory_leaks == 1 )
		{
		}
		else
			XInterlocked::ExchangeSubtract( &_rux_total_memory_size[ module_index ] , size );
	}
};
void XMemoryManager::write_heap_is_not_enough( rux::int32 module_index )
{
	_cs_rux_modules_name->ReadLock();
	if( ::_rux_modules_name[ module_index ][ 0 ] != '\0' )
	{
		declare_stack_variable( char , info_filename , 1024 );
		declare_stack_variable( char , one_line , 1024 );
		::rux::safe_strncpy( info_filename , ::rux::engine::_globals->_executable_directory , 1023 );
		rux_concatenation( info_filename , "/memory/" );
		rux_concatenation( info_filename , _rux_modules_name[ 0 ] );
		rux_concatenation( info_filename , ".snapshot" );
		_cs_rux_modules_name->ReadUnlock();
		::rux::uint64 size = rux_get_file_size( info_filename );
		if( size >= 1024ULL * 1024ULL )
			rux_remove_file( info_filename );
		::rux::uint64 now = rux_get_now_as_local_unix_time();
		declare_stack_variable( char , now_string , 64 );
		rux_unix_time_to_string( now , now_string , 64 );
		::rux::safe_sprintf( one_line , 64 , "%s - (ERROR) cannot alloc memory , possible memory leak\n" , now_string );
		rux_append_to_file( info_filename , one_line );	
		::chmod( info_filename , 0777 );
	}
	else
		_cs_rux_modules_name->ReadUnlock();
}
void XMemoryManager::check_corrupted_memory( void )
{
	_cs_first_last_memory->WriteLock();
	::rux::memory::begin_memory_chunk* bmc = _first_last_memory;
	while( bmc )
	{
		if( bmc->_mmap == 0 )
		{
			if( bmc->_deleted == 1 )
			{
				char one_line[ 256 ] = {0};
				::rux::safe_sprintf( one_line , 256 , "%p is already deleted" , bmc );
				::rux::memory::write_corruption_error( one_line );
				::rux::int32 exception = 0;
				exception = 2 / exception;
			}
			bmc->_deleted = 2;
			_cs_first_last_memory->WriteUnlock();
			::rux::memory::check_memory( bmc );
			_cs_first_last_memory->WriteLock();
			bmc->_deleted = 0;
		}
		bmc = bmc->_prev;
	}
	_cs_first_last_memory->WriteUnlock();
	
	/*_cs_rux_threads->ReadLock();
	if( _rux_threads )
	{
		for( size_t index0 = 0 ; index0 < _rux_threads_count ; index0++ )
		{
			XThreadInfo* ti = _rux_threads[ index0 ];
			if( ti )
			{
				ti->lock();
				_cs_rux_threads->ReadUnlock();
				if( ::XInterlocked::CompareExchange( &ti->_registered , 0 , 0 ) )
					ti->check_corrupted_memory();
				ti->unlock();
				_cs_rux_threads->ReadLock();
			}
		}
	}
	_cs_rux_threads->ReadUnlock();*/
};
void XMemoryManager::check_is_heap_ehough( void )
{
	void* mem = _aligned_malloc( 10 * 1024 * 1024 , 32 );
	if( mem )
		::rux::engine::rux_free_dummy( mem );
	else
		write_heap_is_not_enough( 0 );
};
void XMemoryManager::write_diagnostics( char* now_string , ::rux::uint8& is_interrupt )
{
	char rux_modules_name[ 64 ] = {0};
	declare_stack_variable( char , info_filename , 1024 );
	::rux::safe_strncpy( info_filename , ::rux::engine::_globals->_executable_directory , 1023 );
	::rux::safe_strncat( info_filename , "/memory/" , 1023 );
	::rux::safe_strncat( info_filename , ::rux::engine::_globals->_executable , 1023 );
	::rux::safe_strncat( info_filename , ".snapshot" , 1023 );
	if( rux_is_exists_file( info_filename ) )
		chmod( info_filename , 0777 );
	::rux::uint64 size = rux_get_file_size( info_filename );
	if( size >= 1024ULL * 1024ULL )
		rux_remove_file( info_filename );		
	::rux::io::file file( info_filename , XOpenWriteText );
	if( file.opened() == false )
		file.open( info_filename , XCreateWriteText );
	if( file.opened() )
	{
		file.seek( file.size() );
		::rux::uint32 pid = (::rux::uint32)::rux::diagnostics::process::get_process_id();
		file.write_text( "\n%s(process %u)\n=============================\n\n" , now_string , pid );

		::rux::uint64 rux_total_memory_size = 0ULL , rux_old_total_memory_size = 0ULL , rux_total_descriptors_count = 0ULL , rux_old_total_descriptors_count = 0ULL;
		::rux::uint64 total_memory_size = 0ULL , old_total_memory_size = 0ULL , total_descriptors_count = 0ULL , old_total_descriptors_count = 0ULL;
		for( size_t index4 = 0 ; index4 < RUX_MAX_MODULES_SIZE ; index4++ )
		{
			if( is_interrupt == 1 )
				break;
			_cs_rux_modules_name->ReadLock();
			::rux::safe_strncpy( rux_modules_name , _rux_modules_name[ index4 ] , 63 );
			_cs_rux_modules_name->ReadUnlock();
			if( rux_modules_name[ 0 ] != 0 )
			{	
				file.write_text( "%s module\n-----------------------------\n" , rux_modules_name );
				rux_total_memory_size = XInterlocked::CompareExchange( &_rux_total_memory_size[ index4 ] , 0 , 0 );
				total_memory_size += rux_total_memory_size;
				rux_old_total_memory_size = XInterlocked::Exchange( &_rux_old_total_memory_size[ index4 ] , rux_total_memory_size );
				old_total_memory_size += rux_old_total_memory_size;
				rux_total_descriptors_count = XInterlocked::CompareExchange( &_rux_total_descriptors_count[ index4 ] , 0 , 0 );
				total_descriptors_count += rux_total_descriptors_count;
				rux_old_total_descriptors_count = XInterlocked::Exchange( &_rux_old_total_descriptors_count[ index4 ] , rux_total_descriptors_count );
				old_total_descriptors_count += rux_old_total_descriptors_count;
				if( _rux_is_memory_leaks == 1 )
				{		
					::rux::memory::summary_memory_info* sum = NULL;
					XMallocArray< ::rux::memory::summary_memory_info* > summary_memory_infos;
					_cs_first_last_memory->WriteLock();
					::rux::memory::begin_memory_chunk* bmc = _first_last_memory;
					while( bmc )
					{
						if( bmc->_stack == 0 && bmc->_module_index == index4 )
						{
							if( bmc->_extended_info )
							{
								sum = NULL;
								for( size_t index0 = 0 ; index0 < summary_memory_infos.Count() ; index0++ )
								{
									if( summary_memory_infos[ index0 ]->_file_name == bmc->_extended_info->_file_name
										&& summary_memory_infos[ index0 ]->_type_name == bmc->_extended_info->_type_name
										&& summary_memory_infos[ index0 ]->_line == bmc->_extended_info->_line
										&& strcmp( summary_memory_infos[ index0 ]->_variable_name , bmc->_extended_info->_variable_name ) == 0 )
									{
										sum = summary_memory_infos[ index0 ];
										break;
									}
								}
								if( sum == NULL )
								{
									sum = (::rux::memory::summary_memory_info*)_aligned_malloc( sizeof( ::rux::memory::summary_memory_info ) , 32 );
									sum->_file_name = bmc->_extended_info->_file_name;
									sum->_type_name = bmc->_extended_info->_type_name;
									sum->_line = bmc->_extended_info->_line;
									sum->_size = 0;
									sum->_count = 0;
									sum->_module_index = bmc->_module_index;
									::rux::safe_strncpy( sum->_variable_name , bmc->_extended_info->_variable_name , 31 );
									summary_memory_infos.Add( sum );
								}
								sum->_size += bmc->_size;
								sum->_count++;							
							}
						}
						bmc = bmc->_prev;
					}
					for( size_t index0 = 0 ; index0 < _detailed_memory_infos.Count() ; index0++ )
					{
						if( index4 == _detailed_memory_infos[ index0 ]->_module_index )
						{
							::rux::uint8 found = 0;
							for( size_t index1 = 0 ; index1 < summary_memory_infos.Count() ; index1++ )
							{
								if( summary_memory_infos[ index1 ]->_file_name == _detailed_memory_infos[ index0 ]->_file_name
									&& summary_memory_infos[ index1 ]->_type_name == _detailed_memory_infos[ index0 ]->_type_name
									&& summary_memory_infos[ index1 ]->_line == _detailed_memory_infos[ index0 ]->_line
									&& strcmp( summary_memory_infos[ index1 ]->_variable_name , _detailed_memory_infos[ index0 ]->_variable_name ) == 0 )
								{
									if( summary_memory_infos[ index1 ]->_size > _detailed_memory_infos[ index0 ]->_size )
									{
										if( summary_memory_infos[ index1 ]->_count > _detailed_memory_infos[ index0 ]->_count )
										{
											file.write_text( "%s %s, %s:%d, " I64u "(+" I64u ") allocs, " I64u "(+" I64u ") bytes\n\n" ,
												summary_memory_infos[ index1 ]->_type_name , 
												summary_memory_infos[ index1 ]->_variable_name ,
												summary_memory_infos[ index1 ]->_file_name ,
												summary_memory_infos[ index1 ]->_line ,
												(::rux::uint64)summary_memory_infos[ index1 ]->_count ,
												(::rux::uint64)summary_memory_infos[ index1 ]->_count - _detailed_memory_infos[ index0 ]->_count ,
												(::rux::uint64)summary_memory_infos[ index1 ]->_size , 
												(::rux::uint64)summary_memory_infos[ index1 ]->_size - _detailed_memory_infos[ index0 ]->_size );
										}
										else if( summary_memory_infos[ index1 ]->_count < _detailed_memory_infos[ index0 ]->_count )
										{
											file.write_text( "%s %s, %s:%d, " I64u "(-" I64u ") allocs, " I64u "(+" I64u ") bytes\n\n" ,
												summary_memory_infos[ index1 ]->_type_name , 
												summary_memory_infos[ index1 ]->_variable_name ,
												summary_memory_infos[ index1 ]->_file_name ,
												summary_memory_infos[ index1 ]->_line ,
												(::rux::uint64)summary_memory_infos[ index1 ]->_count ,
												(::rux::uint64)_detailed_memory_infos[ index0 ]->_count - summary_memory_infos[ index1 ]->_count ,
												(::rux::uint64)summary_memory_infos[ index1 ]->_size , 
												(::rux::uint64)summary_memory_infos[ index1 ]->_size - _detailed_memory_infos[ index0 ]->_size );
										}
										else
										{
											file.write_text( "%s %s, %s:%d, " I64u " allocs, " I64u "(+" I64u ") bytes\n\n" ,
												summary_memory_infos[ index1 ]->_type_name , 
												summary_memory_infos[ index1 ]->_variable_name ,
												summary_memory_infos[ index1 ]->_file_name ,
												summary_memory_infos[ index1 ]->_line ,
												(::rux::uint64)summary_memory_infos[ index1 ]->_count ,
												(::rux::uint64)summary_memory_infos[ index1 ]->_size , 
												(::rux::uint64)summary_memory_infos[ index1 ]->_size - _detailed_memory_infos[ index0 ]->_size );
										}
									}
									else if( summary_memory_infos[ index1 ]->_size < _detailed_memory_infos[ index0 ]->_size )
									{
										if( summary_memory_infos[ index1 ]->_count > _detailed_memory_infos[ index0 ]->_count )
										{
											file.write_text( "%s %s, %s:%d, " I64u "(+" I64u ") allocs, " I64u "(-" I64u ") bytes\n\n" ,
												summary_memory_infos[ index1 ]->_type_name , 
												summary_memory_infos[ index1 ]->_variable_name ,
												summary_memory_infos[ index1 ]->_file_name ,
												summary_memory_infos[ index1 ]->_line ,
												(::rux::uint64)summary_memory_infos[ index1 ]->_count ,
												(::rux::uint64)summary_memory_infos[ index1 ]->_count - _detailed_memory_infos[ index0 ]->_count ,
												(::rux::uint64)summary_memory_infos[ index1 ]->_size , 
												(::rux::uint64)_detailed_memory_infos[ index0 ]->_size - summary_memory_infos[ index1 ]->_size );
										}
										else if( summary_memory_infos[ index1 ]->_count < _detailed_memory_infos[ index0 ]->_count )
										{
											file.write_text( "%s %s, %s:%d, " I64u "(-" I64u ") allocs, " I64u "(-" I64u ") bytes\n\n" ,
												summary_memory_infos[ index1 ]->_type_name , 
												summary_memory_infos[ index1 ]->_variable_name ,
												summary_memory_infos[ index1 ]->_file_name ,
												summary_memory_infos[ index1 ]->_line ,
												(::rux::uint64)summary_memory_infos[ index1 ]->_count ,
												(::rux::uint64)_detailed_memory_infos[ index0 ]->_count - summary_memory_infos[ index1 ]->_count ,
												(::rux::uint64)summary_memory_infos[ index1 ]->_size , 
												(::rux::uint64)_detailed_memory_infos[ index0 ]->_size - summary_memory_infos[ index1 ]->_size );
										}
										else
										{
											file.write_text( "%s %s, %s:%d, " I64u " allocs, " I64u "(-" I64u ") bytes\n\n" ,
												summary_memory_infos[ index1 ]->_type_name , 
												summary_memory_infos[ index1 ]->_variable_name ,
												summary_memory_infos[ index1 ]->_file_name ,
												summary_memory_infos[ index1 ]->_line ,
												(::rux::uint64)summary_memory_infos[ index1 ]->_count ,
												(::rux::uint64)summary_memory_infos[ index1 ]->_size , 
												(::rux::uint64)_detailed_memory_infos[ index0 ]->_size - summary_memory_infos[ index1 ]->_size );
										}
									}
									_detailed_memory_infos[ index0 ]->_size = summary_memory_infos[ index1 ]->_size;
									_detailed_memory_infos[ index0 ]->_count = summary_memory_infos[ index1 ]->_count;
									found = 1;
									::rux::engine::rux_free_dummy( summary_memory_infos[ index1 ] );
									summary_memory_infos.RemoveAt( index1 );
									break;
								}
							}
							if( found == 0 )
							{
								file.write_text( "%s %s, %s:%d, 0(-" I64u ") allocs, 0(-" I64u ") bytes\n\n" ,
									_detailed_memory_infos[ index0 ]->_type_name , 
									_detailed_memory_infos[ index0 ]->_variable_name ,
									_detailed_memory_infos[ index0 ]->_file_name ,
									_detailed_memory_infos[ index0 ]->_line ,
									(::rux::uint64)_detailed_memory_infos[ index0 ]->_count ,
									(::rux::uint64)_detailed_memory_infos[ index0 ]->_size );
								::rux::engine::rux_free_dummy( _detailed_memory_infos[ index0 ] );
								_detailed_memory_infos.RemoveAt( index0 );
								index0--;
							}		
						}
					}
					for( size_t index0 = 0 ; index0 < summary_memory_infos.Count() ; index0++ )
					{
						file.write_text( "%s %s, %s:%d, " I64u "(+" I64u ") allocs, " I64u "(+" I64u ") bytes\n\n" ,
							summary_memory_infos[ index0 ]->_type_name , 
							summary_memory_infos[ index0 ]->_variable_name ,
							summary_memory_infos[ index0 ]->_file_name ,
							summary_memory_infos[ index0 ]->_line ,
							(::rux::uint64)summary_memory_infos[ index0 ]->_count ,
							(::rux::uint64)summary_memory_infos[ index0 ]->_count ,
							(::rux::uint64)summary_memory_infos[ index0 ]->_size ,
							(::rux::uint64)summary_memory_infos[ index0 ]->_size );

						_detailed_memory_infos.Add( summary_memory_infos[ index0 ] );
					}
					_cs_first_last_memory->WriteUnlock();
				}
				if( rux_total_memory_size > rux_old_total_memory_size )
					file.write_text( "Total memory:" I64u "(+" I64u ") bytes\n\n" , rux_total_memory_size , rux_total_memory_size - rux_old_total_memory_size );
				else if( rux_total_memory_size < rux_old_total_memory_size )
					file.write_text( "Total memory:" I64u "(-" I64u ") bytes\n\n" , rux_total_memory_size , rux_old_total_memory_size - rux_total_memory_size );
				else
					file.write_text( "Total memory:" I64u " bytes\n\n" , rux_total_memory_size );	
				if( rux_total_descriptors_count > rux_old_total_descriptors_count )
					file.write_text( "Total descriptors:" I64u "(+" I64u ")\n\n" , rux_total_descriptors_count , rux_total_descriptors_count - rux_old_total_descriptors_count );
				else if( rux_total_descriptors_count < rux_old_total_descriptors_count )
					file.write_text( "Total descriptors:" I64u "(-" I64u ")\n\n" , rux_total_descriptors_count , rux_old_total_descriptors_count - rux_total_descriptors_count );
				else
					file.write_text( "Total descriptors:" I64u "\n\n" , rux_total_descriptors_count );
				file.write_text( "-----------------------------\n\n" );
			}	
		}

		if( total_memory_size > old_total_memory_size )
			file.write_text( "Total memory:" I64u "(+" I64u ") bytes\n\n" , total_memory_size , total_memory_size - old_total_memory_size );
		else if( total_memory_size < old_total_memory_size )
			file.write_text( "Total memory:" I64u "(-" I64u ") bytes\n\n" , total_memory_size , old_total_memory_size - total_memory_size );
		else
			file.write_text( "Total memory:" I64u " bytes\n\n" , total_memory_size );	
		if( total_descriptors_count > old_total_descriptors_count )
			file.write_text( "Total descriptors:" I64u "(+" I64u ")\n\n" , total_descriptors_count , total_descriptors_count - old_total_descriptors_count );
		else if( total_descriptors_count < rux_old_total_descriptors_count )
			file.write_text( "Total descriptors:" I64u "(-" I64u ")\n\n" , total_descriptors_count , old_total_descriptors_count - total_descriptors_count );
		else
			file.write_text( "Total descriptors:" I64u "\n\n" , total_descriptors_count );
		file.write_text( "Total threads:" I64u "\n\n=============================\n" , (::rux::uint64)XThreadInfo::get_threads_count() );
		file.close();
		::chmod( info_filename , 0777 );
	}
};
void XMemoryManager::private_info_thread(void* param, size_t ___rux__thread_index1986, lineid_t _lineid
	, statement_t _statement, void* udata)
{	
	rux_debuginfo_macros_statement(999);
	XMemoryManager* _this = cpp_ptr_cast(XMemoryManager* , param );
	::rux::uint64 now = rux_get_now_as_local_unix_time();
	if( now - _this->_last_diagnostics_time >= ::rux::engine::_diagnostics_timeout * 1000000 )
	{
		rux_debuginfo_macros_statement(998);
		_this->_last_diagnostics_time = now;
		{
			rux_debuginfo_macros_statement(997);
			::rux::uint8 is_interrupt = 0;
			declare_stack_variable( char , now_string , 64 );
			rux_unix_time_to_string( now , now_string , 64 );
			rux_debuginfo_macros_statement(996);
			_this->write_diagnostics( now_string , is_interrupt );
			rux_debuginfo_macros_statement(995);
			XThreadInfo::write_diagnostics( _this , now_string , is_interrupt );
			rux_debuginfo_macros_statement(994);
			if( is_interrupt == 0 )
			{
				rux_debuginfo_macros_statement(993);
				declare_stack_variable( char , info_filename , 1024 );
				::rux::safe_strncpy( info_filename , ::rux::engine::_globals->_executable_directory , 1023 );
				::rux::safe_strncat( info_filename , "/memory/total." , 1023 );
				::rux::safe_strncat( info_filename , ::rux::engine::_globals->_executable , 1023 );
				::rux::safe_strncat( info_filename , ".snapshot" , 1023 );
				rux_debuginfo_macros_statement(992);
				if( rux_is_exists_file( info_filename ) )
					chmod( info_filename , 0777 );
				rux_debuginfo_macros_statement(991);
				::rux::uint64 size = rux_get_file_size( info_filename );
				if( size >= 1024ULL * 1024ULL )
					rux_remove_file( info_filename );
				rux_debuginfo_macros_statement(990);
				::rux::io::file file( info_filename , XOpenWriteText );
				if( file.opened() == false )
					file.open( info_filename , XCreateWriteText );
				rux_debuginfo_macros_statement(989);
				if( file.opened() )
				{
					rux_debuginfo_macros_statement(988);
					file.seek( file.size() );
					rux_debuginfo_macros_statement(987);
					::rux::uint32 pid = (::rux::uint32)::rux::diagnostics::process::get_process_id();
					file.write_text( "\n%s(process %u)\n=============================\n\n" , now_string , pid );

					rux_debuginfo_macros_statement(986);
#ifdef __WINDOWS__
					::rux::int64 temp = ::rux::diagnostics::process_info::working_set( ::rux::engine::_globals->_executable , 0 );
#else
					::rux::int64 temp = ::rux::diagnostics::process_info::working_set(pid, 0);
#endif
					if( temp > _working_set )
						file.write_text( "Working set " I64u "(+" I64u ") bytes\n" , temp , temp - _working_set );
					else if( temp < _working_set )
						file.write_text( "Working set " I64u "(-" I64u ") bytes\n" , temp , _working_set - temp );
					else
						file.write_text( "Working set " I64u " bytes\n" , temp );

					rux_debuginfo_macros_statement(985);

					_working_set = temp;
#ifdef __WINDOWS__
					temp = ::rux::diagnostics::process_info::virtual_bytes( ::rux::engine::_globals->_executable , 0 );
#else
					temp = ::rux::diagnostics::process_info::virtual_bytes(pid, 0);
#endif
					if( temp > _virtual_bytes )
						file.write_text( "Virtual bytes " I64u "(+" I64u ") bytes\n" , temp , temp - _virtual_bytes );
					else if( temp < _virtual_bytes )
						file.write_text( "Virtual bytes " I64u "(-" I64u ") bytes\n" , temp , _virtual_bytes - temp );
					else
						file.write_text( "Virtual bytes " I64u " bytes\n" , temp );
					_virtual_bytes = temp;

					rux_debuginfo_macros_statement(984);

					temp = ::rux::diagnostics::process_info::threads_count( pid , 0 );
					if( temp > _process_threads_count )
						file.write_text( "Threads " I64u "(+" I64u ")\n" , temp , temp - _process_threads_count );
					else if( temp < _process_threads_count )
						file.write_text( "Threads " I64u "(-" I64u ")\n" , temp , _process_threads_count - temp );
					else
						file.write_text( "Threads " I64u "\n" , temp );
					_process_threads_count = temp;		

					rux_debuginfo_macros_statement(983);
#ifdef __UNIX__
					struct rlimit lim;
					int rlim_cur = 0, rlim_max = 0;
					lim.rlim_cur = 0;
					lim.rlim_max = 0;
					if(getrlimit(RLIMIT_NOFILE, &lim) == 0)
					{
						rlim_cur = (int)lim.rlim_cur, rlim_max = (int)lim.rlim_max;
					}
					rux_debuginfo_macros_statement(982);

					temp = ::rux::diagnostics::process_info::descriptors_count(pid, 0);
					if(temp > _process_descriptors_count)
						file.write_text("Descriptors " I64u "(+" I64u ")\n", temp, temp - _process_descriptors_count);
					else if(temp < _process_descriptors_count)
						file.write_text("Descriptors " I64u "(-" I64u ")\n", temp, _process_descriptors_count - temp);
					else
						file.write_text("Descriptors " I64u "\n", temp);
					file.write_text("Descriptors soft limit %d\n", rlim_cur);
					file.write_text("Descriptors hard limit %d\n", rlim_max);
					_process_descriptors_count = temp;

					rux_debuginfo_macros_statement(981);
#endif
					rux_debuginfo_macros_statement(980);

					file.write_text( "\n=============================\n" );
					file.close();
					::chmod( info_filename , 0777 );

					rux_debuginfo_macros_statement(979);
				}
			}
		}
		rux_debuginfo_macros_statement(978);
		XMemoryManager::check_corrupted_memory();
	}
	rux_debuginfo_macros_statement(977);
	XMemoryManager::write_cpu_logs();
	rux_debuginfo_macros_statement(976);
	XMemoryManager::check_is_heap_ehough();
	rux_debuginfo_macros_statement(975);
	XThreadInfo::check_unknown_threads();
	rux_debuginfo_macros_statement(974);
};
rux::int32 XMemoryManager::register_module( const char* module_name )
{
	::rux::int32 index = 0;
	_cs_rux_modules_name->WriteLock();
	for( ; index < RUX_MAX_MODULES_SIZE - 1 ; index++ )
	{
		if( _rux_modules_name[ index ][ 0 ] == 0 )
		{
			size_t size = strlen( module_name ) + 1;
			if( size > 64 )
			{
				::memcpy( _rux_modules_name[ index ] , module_name , 64 );
				_rux_modules_name[ index ][ 63 ] = 0;
			}
			else
				::memcpy( _rux_modules_name[ index ] , module_name , size );
			break;
		}
	}
	if( index == RUX_MAX_MODULES_SIZE - 1 )
		::memcpy( _rux_modules_name[ index ] , "many.modules" , 13 );
	/*if( ::rux::log::_logger )
		::rux::log::_logger->register_module( index , _rux_modules_name[ index ] );*/
	_cs_rux_modules_name->WriteUnlock();
	return index;
};
void XMemoryManager::add_search_directory( const char* directory )
{
	_cs_rux_search_directories->WriteLock();
	if( _rux_search_directories_count < 16 )
	{
		size_t length = directory ? strlen( directory ) : 0;
		if( length > 0 && length < 512 )
		{
			::memcpy( _rux_search_directories[ _rux_search_directories_count ] , directory , length + 1 );
			rux_replace( _rux_search_directories[ _rux_search_directories_count ] , '\\' , '/' );
			if( _rux_search_directories[ _rux_search_directories_count ][ length - 1 ] == '/' )
				_rux_search_directories[ _rux_search_directories_count ][ length - 1 ] = '\0';	
			_rux_search_directories_count++;
		}
	}
	_cs_rux_search_directories->WriteUnlock();
};
rux::uint32 XMemoryManager::get_search_directory_count( void )
{
	_cs_rux_search_directories->ReadLock();
	::rux::uint32 count = _rux_search_directories_count;
	_cs_rux_search_directories->ReadUnlock();
	return count;
};
void XMemoryManager::get_search_directory( ::rux::uint32 index , char directory[ 512 ] )
{
	_cs_rux_search_directories->ReadLock();
	if( index < _rux_search_directories_count )
		::memcpy( directory , _rux_search_directories[ index ] , strlen( _rux_search_directories[ index ] ) + 1 );
	_cs_rux_search_directories->ReadUnlock();
};
void XMemoryManager::add_or_remove_thread_event( ::rux::byte event_type , ::rux::byte add , ::rux::on_thread_registered_or_unregistered thread_event )
{
	if( event_type == ::rux::engine::XEnum_EngineEvent_RegisteredThread )
	{
		if( add )
			::rux::engine::_globals->_on_thread_registered += thread_event;
		else
			::rux::engine::_globals->_on_thread_registered -= thread_event;
	}
	else if( event_type == ::rux::engine::XEnum_EngineEvent_UnregisteredThread )
	{
		if( add )
			::rux::engine::_globals->_on_thread_unregistered += thread_event;
		else
			::rux::engine::_globals->_on_thread_unregistered -= thread_event;
	}
	else if( event_type == ::rux::engine::XEnum_EngineEvent_BeforeLoadModule )
	{
		if( add )
			::rux::engine::_globals->_on_before_load_module += thread_event;
		else
			::rux::engine::_globals->_on_before_load_module -= thread_event;
	}
	else if( event_type == ::rux::engine::XEnum_EngineEvent_AfterLoadModule )
	{
		if( add )
			::rux::engine::_globals->_on_after_load_module += thread_event;
		else
			::rux::engine::_globals->_on_after_load_module -= thread_event;
	}
	else if( event_type == ::rux::engine::XEnum_EngineEvent_BeforeModuleDllInit )
	{
		if( add )
			::rux::engine::_globals->_on_before_module_dll_init += thread_event;
		else
			::rux::engine::_globals->_on_before_module_dll_init -= thread_event;
	}
	else if( event_type == ::rux::engine::XEnum_EngineEvent_AfterModuleDllInit )
	{
		if( add )
			::rux::engine::_globals->_on_after_module_dll_init += thread_event;
		else
			::rux::engine::_globals->_on_after_module_dll_init -= thread_event;
	}
};
::rux::uint32 XMemoryManager::add_times33_hash( const char* value_string , size_t value_string_length )
{
	::rux::byte exists = 0;
	::rux::uint32 times33_hash = ::rux::cryptography::hash::times33_hash( value_string , value_string_length );
	READ_LOCK( ::rux::engine::_globals->_cs_times33_hashes );
	std::map< ::rux::uint32 , char* >::iterator i = ::rux::engine::_globals->_times33_hashes.find( times33_hash );
	if( i != ::rux::engine::_globals->_times33_hashes.end() )
		exists = 1;
	::rux::engine::_globals->_cs_times33_hashes.ReadUnlock();
	if( exists == 0 )
	{
		WRITE_LOCK( ::rux::engine::_globals->_cs_times33_hashes );
		if( ::rux::engine::_globals->_times33_hashes.find( times33_hash ) == ::rux::engine::_globals->_times33_hashes.end() )
		{
			size_t len = value_string_length;
			if( len == SIZE_MAX )
				len = strlen( value_string );
			char* value = alloc_array_macros( char , len + 1 );
			::memcpy( value , value_string , len );
			value[ len ] = 0;
			::rux::engine::_globals->_times33_hashes[ times33_hash ] = value;
		}
		::rux::engine::_globals->_cs_times33_hashes.WriteUnlock();
	}
	return times33_hash;
};