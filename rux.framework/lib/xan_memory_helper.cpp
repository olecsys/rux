#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_memory_helper.h"
#include "xan_memorymanager_defines.h"
#include "xan_gc_defines.h"
namespace rux
{
	dll_internal char* safe_strncpy( char* dest , const char* src , size_t dest_size , size_t src_size )
	{
		char* dest_ptr = dest;
		char* src_ptr = (char*)src;
		for( ; ; )
		{
			if( src_size <= (size_t)( src_ptr - src ) )
			{
				if( dest_size <= (size_t)( dest_ptr - dest ) )
					dest[ dest_size - 1 ] = 0;
				else
					*dest_ptr = 0;
				break;
			}
			if( dest_size <= (size_t)( dest_ptr - dest ) )
			{
				dest[ dest_size - 1 ] = 0;
				break;
			}
			*dest_ptr = *src_ptr;
			if( *src_ptr == 0 )
				break;
			dest_ptr++;
			src_ptr++;
		}
		return dest;
	};
	dll_internal char* safe_strncat( char* dest , const char* src , size_t dest_size , size_t src_size )
	{
		char* dest_ptr = dest;
		while( *dest_ptr != 0 )
			dest_ptr++;
		::rux::safe_strncpy( dest_ptr , src , dest_size - (size_t)( dest_ptr - dest ) , src_size );
		return dest;
	};
	namespace memory
	{		
		dll_internal size_t get_page_size( void )
		{
#ifdef __WINDOWS__
			SYSTEM_INFO si;
			GetSystemInfo( &si );
			return si.dwPageSize;
#else
			return sysconf( _SC_PAGESIZE );
#endif
		};
		dll_internal void write_corruption_error( const char* memory )
		{
			char one_line[ 8192 ] = {0};
			char info_filename[ 1024 ] = {0};
			::rux::safe_strncat( info_filename , ::rux::engine::_globals->_executable_directory , 1024 );
			::rux::safe_strncat( info_filename , "/" , 1024 );
			::rux::safe_strncat( info_filename , g_current_module_name , 1024 );
			::rux::safe_strncat( info_filename , ".corruption" , 1024 );
			::rux::uint64 now = rux_get_now_as_local_unix_time();
			char now_string[ 64 ] = {0};
			rux_unix_time_to_string( now , now_string , 64 );
			::rux::safe_sprintf( one_line , 8192 , "%s - %s\n" , now_string , memory );			
			::rux::uint64 size = rux_get_file_size( info_filename );
			if( size >= 1024ULL * 1024ULL )
				rux_remove_file( info_filename );
			rux_append_to_file( info_filename , one_line );
#ifdef __WINDOWS__
			_chmod( info_filename , 0777 );
#else				
			chmod( info_filename , 0777 );
#endif			
		};
		dll_internal void check_memory( ::rux::memory::begin_memory_chunk* bmc )
		{
#if RUX_STACK_AND_GLOBAL_DIAGNOSTIC
			::rux::uint8 raise_exception = 0;
			if( bmc )
			{	
				::rux::uint8* dst_begin = ((::rux::uint8*)bmc) + sizeof( ::rux::memory::begin_memory_chunk );
				size_t bsize = bmc->_size - sizeof( ::rux::memory::begin_memory_chunk ) - sizeof( ::rux::memory::end_memory_chunk );
				if( bmc->_extended_info )
					bsize -= sizeof( extended_memory_info );
				::rux::uint8* ptr_end = dst_begin + bsize;
				end_memory_chunk* emc = (end_memory_chunk*)ptr_end;
				if( bmc->_marker != begin_memory_marker
					|| emc->_marker != end_memory_marker )
				{
					char one_line[ 256 ] = {0};
					if( bmc->_marker != begin_memory_marker )
						::rux::safe_sprintf( one_line , 256 , "bottom of the %p is corrupted" , dst_begin );
					else
						::rux::safe_sprintf( one_line , 256 , "top of the %p is corrupted" , dst_begin );
					write_corruption_error( one_line );
					raise_exception = 1;
				}
				else if( bmc->_deleted == 1 )
				{
					char one_line[ 256 ] = {0};
					::rux::safe_sprintf( one_line , 256 , "%p is already deleted" , dst_begin );
					write_corruption_error( one_line );
					raise_exception = 1;
				}
			}
			else
			{				
				write_corruption_error( "unknown memory" );
				raise_exception = 1;
			}
			if( raise_exception ) 
			{
				::rux::int32 exception = 0;
				exception = 2 / exception;
			}
#endif
		};
		dll_internal void check_memory( const void* dst )
		{
#if RUX_STACK_AND_GLOBAL_DIAGNOSTIC
			::rux::uint8 res = is_diagnostics_memory( dst );
			if( res != 0 )
			{
				::rux::uint8* dst_begin = (::rux::uint8*)dst , raise_exception = 0;
				::rux::memory::begin_memory_chunk* bmc = NULL;
				if( res == 1 )
					bmc = (begin_memory_chunk*)( dst_begin - sizeof( begin_memory_chunk ) );
				else
				{
					bmc = ::rux::engine::_globals->_find_begin_memory_chunk( dst );
					if( bmc )
						dst_begin = ((::rux::uint8*)bmc) + sizeof( begin_memory_chunk );
				}
				if( bmc )
				{	
					size_t bsize = bmc->_size - sizeof( ::rux::memory::begin_memory_chunk ) - sizeof( end_memory_chunk );
					if( bmc->_extended_info )
						bsize -= sizeof( extended_memory_info );
					::rux::uint8* ptr_end = dst_begin + bsize;
					end_memory_chunk* emc = (end_memory_chunk*)ptr_end;
					if( bmc->_marker != begin_memory_marker
						|| emc->_marker != end_memory_marker )
					{
						char one_line[ 256 ] = {0};
						if( bmc->_marker != begin_memory_marker )
							::rux::safe_sprintf( one_line , 256 , "bottom of the %p is corrupted" , dst_begin );
						else
							::rux::safe_sprintf( one_line , 256 , "top of the %p is corrupted" , dst_begin );
						write_corruption_error( one_line );
						raise_exception = 1;
					}
					else if( bmc->_deleted == 1 )
					{
						char one_line[ 256 ] = {0};
						::rux::safe_sprintf( one_line , 256 , "%p is already deleted" , dst_begin );
						write_corruption_error( one_line );
						raise_exception = 1;
					}
				}
				else
				{
					declare_stack_variable( char , one_line , 256 );
					::rux::safe_sprintf( one_line , 256 , "%p is unknown memory" , dst );
					write_corruption_error( one_line );
					raise_exception = 1;
				}
				if( raise_exception ) 
				{
					::rux::int32 exception = 0;
					exception = 2 / exception;
				}
			}
#endif
		};
		dll_internal void check_memory( const void* dst , size_t dst_offset , size_t size )
		{	
#if RUX_STACK_AND_GLOBAL_DIAGNOSTIC
			::rux::uint8 res = is_diagnostics_memory( dst );
			if( res != 0 )
			{
				::rux::uint8* dst_begin = (::rux::uint8*)dst , raise_exception = 0;				
				::rux::memory::begin_memory_chunk* bmc = NULL;
				if( res == 1 )
					bmc = (begin_memory_chunk*)( dst_begin - sizeof( begin_memory_chunk ) );
				else
				{
					if( ::rux::engine::_globals )
						bmc = ::rux::engine::_globals->_find_begin_memory_chunk( dst );
					if( bmc )
						dst_begin = ((::rux::uint8*)bmc) + sizeof( begin_memory_chunk );
				}
				if( ::rux::engine::_globals )
				{
					if( bmc )
					{	
						::rux::uint8* dst_end = dst_begin + dst_offset + size;
						size_t bsize = bmc->_size - sizeof( ::rux::memory::begin_memory_chunk ) - sizeof( end_memory_chunk );
						if( bmc->_extended_info )
							bsize -= sizeof( extended_memory_info );
						::rux::uint8* ptr_end = dst_begin + bsize;
						end_memory_chunk* emc = (end_memory_chunk*)ptr_end;
						if( bmc->_marker != begin_memory_marker
							|| emc->_marker != end_memory_marker
							|| dst_end > ptr_end )
						{
							char one_line[ 256 ] = {0};
							if( bmc->_marker != begin_memory_marker )
								::rux::safe_sprintf( one_line , 256 , "bottom of the %p is corrupted" , dst_begin );
							else if( emc->_marker != end_memory_marker )
								::rux::safe_sprintf( one_line , 256 , "top of the %p is corrupted" , dst_begin );
							else
								::rux::safe_sprintf( one_line , 256 , "memory from %p to %p is out of the range" , dst_begin , dst_end );
							write_corruption_error( one_line );
							raise_exception = 1;
						}
						else if( bmc->_deleted == 1 )
						{
							char one_line[ 256 ] = {0};
							::rux::safe_sprintf( one_line , 256 , "%p is already deleted" , dst_begin );
							write_corruption_error( one_line );
							raise_exception = 1;
						}
					}
					else
					{
						declare_stack_variable( char , one_line , 256 );
						::rux::safe_sprintf( one_line , 256 , "%p is unknown memory" , dst );
						write_corruption_error( one_line );
						raise_exception = 1;
					}
				}
				if( raise_exception ) 
				{
					::rux::int32 exception = 172;
					exception = 2 / ( exception - 172 );
					printf( "exception %d" , exception );
				}
			}
#endif
		};
		dll_internal ::rux::uint8 is_diagnostics_memory( const void* dst )
		{
			::rux::uint8* dst_begin = (::rux::uint8*)dst;		
			::rux::memory::begin_memory_chunk* bmc = (begin_memory_chunk*)( dst_begin - sizeof( begin_memory_chunk ) );
			if( bmc->_marker == begin_memory_marker )
			{
				if( bmc->_diagnostics == 1 )
					return 1;
				else if( bmc->_diagnostics == 0 )
					return 0;
			}
			return 2;
		};
	};
};