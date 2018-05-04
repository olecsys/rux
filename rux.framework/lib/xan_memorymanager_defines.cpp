#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_memorymanager_defines.h>
#include <xan_asm.h>
#include <xan_console.h>
#include <xan_error.h>
#include <xan_io_defines.h>
#ifdef __WINDOWS__
#include <aclapi.h>
#include <WtsApi32.h>
#include <UserEnv.h>
#define GENERIC_ALL_ACCESS 0x10000000
#include <Psapi.h>
dll_internal HMODULE _engine_handle = NULL;
#endif
#ifdef __UNIX__
#include <pwd.h>
dll_internal void* _engine_handle = NULL;
#endif
#include <xan_debug.h>
#include <xan_log.h>
#include <xan_native_diagnostics.h>
#include <xan_file_descriptor_waiter.h>
#include <xan_code_performance_counter.h>
#include <xan_memory_chunk.h>
#include <xan_network_functions.h>
#include <xan_boolean.h>
#include <xan_uint64.h>
#include <xan_float.h>
#include <xan_double.h>
#include <xan_uint32.h>
#include <xan_int64.h>
#include <xan_numeric.h>
#include <xan_event.h>
#include <xan_uigroup.h>
#include <xan_dataobject.h>
#include <xan_data.h>
dll_internal ::rux::pid_t _engine_init_thread_id = 0;
dll_internal ::rux::int32 _rux_current_module_index = 0;
dll_internal ::rux::uint8 _rux_deinitialization_funcs_count = 0;
dll_internal rux_simple_t _rux_deinitialization_funcs[ 16 ] = {0};
namespace rux
{
	dll_internal rux_volatile _current_module_is_locked = 0;
};
XCodeLabel::XCodeLabel( size_t rux__thread_index1986 , const char* filename , const char* function , ::rux::int32 line , const char* other_module , const char* other_function , ::rux::uint32 max_delay , ::rux::byte debug )
{
	___rux__thread_index1986 = rux__thread_index1986;
	if( debug && ::rux::engine::_globals && ::rux::engine::_globals->_rux_memory_add_label )
		::rux::engine::_globals->_rux_memory_add_label( ___rux__thread_index1986 , filename , function , line , other_module , other_function , _rux_current_module_index , 1 , max_delay );
	else
		___rux__thread_index1986 = SIZE_MAX;
};
XCodeLabel::~XCodeLabel()
{
	if( ___rux__thread_index1986 != SIZE_MAX && ::rux::engine::_globals && ::rux::engine::_globals->_rux_memory_remove_label )
		::rux::engine::_globals->_rux_memory_remove_label( ___rux__thread_index1986 );
};
dll_internal ::rux::uint64 rux_get_file_size( char* filename )
{
	::rux::uint64 size = 0ULL;
#ifdef __WINDOWS__
	HANDLE file = CreateFileA( filename , GENERIC_READ | GENERIC_WRITE , 0 , NULL , OPEN_EXISTING , FILE_ATTRIBUTE_NORMAL , NULL ); 
	if( file != NULL && file != INVALID_HANDLE_VALUE ) 
	{
		LARGE_INTEGER large_integer;
		memset( &large_integer , 0 , sizeof( LARGE_INTEGER ) );
		GetFileSizeEx( file , &large_integer );
		size = large_integer.QuadPart;
		CloseHandle( file );
	}
#elif defined( __UNIX__ )
	struct stat st;
	if( stat( filename , &st ) == 0 )
		size = st.st_size;
#endif
	return size;
};
dll_internal void rux_unix_time_to_string( ::rux::uint64 unix_time_in_microseconds , char unix_time_string[] , size_t unix_time_string_size )
{
	::rux::uint32 days = (::rux::uint32)( ( unix_time_in_microseconds / 1000000ULL ) / ( 60ULL * 60ULL * 24ULL ) );
	::rux::uint32 year = 1970;
	::rux::uint32 leap = 2;
	::rux::uint32 days_offset = 0;
	::rux::uint64 time_offset = days * 60ULL * 60ULL * 24ULL * 1000000ULL;
	for( ; ; )
	{
		if( leap == 0 )
		{
			days_offset += 366;
			if( days_offset >= days )
			{
				days_offset -= 366;
				break;
			}
			leap = 3;
		}
		else
		{
			days_offset += 365;
			if( days_offset >= days )
			{
				days_offset -= 365;
				break;
			}
			leap--;
		}
		year++;
	}
	days -= days_offset;
	days++;
	::rux::uint32 month = 1;
	for( ; month < 13 ; month++ )
	{
		days_offset = rux_days_in_month( month , year );
		if( days > days_offset )
			days -= days_offset;
		else
			break;
	}
	time_offset = unix_time_in_microseconds - time_offset;
	::rux::uint32 hour = (::rux::uint32)( time_offset / ( 60ULL * 60ULL * 1000000ULL ) );
	time_offset -= hour * 60ULL * 60ULL * 1000000ULL;
	::rux::uint32 minute = (::rux::uint32)( time_offset / ( 60ULL * 1000000ULL ) );
	time_offset -= minute * 60ULL * 1000000ULL;
	::rux::uint32 seconds = (::rux::uint32)( time_offset / 1000000ULL );
	time_offset -= seconds * 1000000ULL;
	if( unix_time_string_size >= 20 )
	{
		char uint32_string[ 16 ] = {0};
		rux::string::uint32_to_string( year , uint32_string );
		::rux::safe_strncat( unix_time_string , uint32_string , unix_time_string_size );
		::rux::safe_strncat( unix_time_string , "." , unix_time_string_size );
		if( month < 10 )
			::rux::safe_strncat( unix_time_string , "0" , unix_time_string_size );
		rux::string::uint32_to_string( month , uint32_string );
		::rux::safe_strncat( unix_time_string , uint32_string , unix_time_string_size );
		::rux::safe_strncat( unix_time_string , "." , unix_time_string_size );
		if( days < 10 )
			::rux::safe_strncat( unix_time_string , "0" , unix_time_string_size );
		rux::string::uint32_to_string( days , uint32_string );
		::rux::safe_strncat( unix_time_string , uint32_string , unix_time_string_size );
		::rux::safe_strncat( unix_time_string , " " , unix_time_string_size );
		if( hour < 10 )
			::rux::safe_strncat( unix_time_string , "0" , unix_time_string_size );
		rux::string::uint32_to_string( hour , uint32_string );
		::rux::safe_strncat( unix_time_string , uint32_string , unix_time_string_size );
		::rux::safe_strncat( unix_time_string , ":" , unix_time_string_size );
		if( minute < 10 )
			::rux::safe_strncat( unix_time_string , "0" , unix_time_string_size );
		rux::string::uint32_to_string( minute , uint32_string );
		::rux::safe_strncat( unix_time_string , uint32_string , unix_time_string_size );
		::rux::safe_strncat( unix_time_string , ":" , unix_time_string_size );
		if( seconds < 10 )
			::rux::safe_strncat( unix_time_string , "0" , unix_time_string_size );
		rux::string::uint32_to_string( seconds , uint32_string );
		::rux::safe_strncat( unix_time_string , uint32_string , unix_time_string_size );
	}
};
dll_internal void rux_get_executable_filename_only( char module_directory[] )
{	
#ifdef __WINDOWS__
	HMODULE module_handle = GetModuleHandleA( NULL );
	if( module_handle )
	{
		declare_stack_variable( char , module_path , 4096 );
		if( GetModuleFileNameA( module_handle , module_path , 4096 ) != 0 )
		{
			::rux::uint32 index0 = 0 , index1 = 0;					
			for( ; ; )
			{						
				if( module_path[ index0 ] == '\0' ) break;		
				else if( module_path[ index0 ] == '\\' )
					index1 = index0;
				index0++;
			}
			::memcpy( module_directory , &module_path[ index1 + 1 ] , index0 - index1 );
			module_directory[ index0 - index1 ] = '\0';				
		}				
	}	
#endif
#ifdef __UNIX__
	declare_stack_variable( char , module_path , 4096 );
#ifdef __LINUX__
	if( readlink( "/proc/self/exe" , module_path , 4095 ) > 0 )
#elif defined( __SOLARIS__ )
	declare_stack_variable( char , key_ptr , 64 );
	::rux::uint32 pid = getpid() , number = 0 , offset = 0;
	const char* const_string = NULL;
	const_string = "/proc/";
	::rux::engine::memcpy( key_ptr , offset , const_string , 6 );
	offset += 6;
	rux::string::uint32_to_string( pid , &key_ptr[ offset ] );
	offset = strlen( key_ptr );
	const_string = "/path/a.out";
	::rux::engine::memcpy( key_ptr , offset , const_string , 11 );
	offset += 11;
	key_ptr[ offset ] = '\0';	
	if( readlink( key_ptr , module_path , 4095 ) > 0 )
#endif
	{
		::rux::uint32 index0 = 0 , index1 = 0;					
		for( ; ; )
		{						
			if( module_path[ index0 ] == '\0' ) break;		
			else if( module_path[ index0 ] == '/' )
				index1 = index0;
			index0++;
		}		
		::rux::engine::memcpy( module_directory , 0 , &module_path[ index1 + 1 ] , index0 - index1 );
		module_directory[ index0 - index1 ] = '\0';				
	}
#endif
};
dll_internal void rux_get_directory_only( const char* module_path , char module_directory[] )
{	
	if( module_path )
	{		
		::rux::uint32 index0 = 0 , index1 = 0;					
		for( ; ; )
		{						
			if( module_path[ index0 ] == '\0' ) break;		
			else if( module_path[ index0 ] == '/' || module_path[ index0 ] == '\\' )
				index1 = index0;
			index0++;
		}
		if( index1 == 0 && index0 > 0 )
		{
			module_directory[ 0 ] = '/';
			module_directory[ 1 ] = '\0';
		}
		else if( index1 > 0 )
		{
			::rux::engine::memcpy( module_directory , 0 , module_path , index1 );
			module_directory[ index1 ] = '\0';
		}
		else
			module_directory[ 0 ] = '\0';
	}
};
dll_internal void rux_get_executable_filename_only( const char* module_path , char module_directory[] )
{	
	if( module_path )
	{		
		::rux::uint32 index0 = 0 , index1 = 0;					
		for( ; ; )
		{						
			if( module_path[ index0 ] == '\0' ) break;		
			else if( module_path[ index0 ] == '/' || module_path[ index0 ] == '\\' )
				index1 = index0;
			index0++;
		}
		if( index1 == 0 && module_path[ 0 ] != '/' && module_path[ 0 ] != '\\' )
			::rux::engine::memcpy( module_directory , 0 , &module_path[ 0 ] , index0 );
		else if( index0 > 0 )
			::rux::engine::memcpy( module_directory , 0 , &module_path[ index1 + 1 ] , index0 - index1 );
		module_directory[ index0 - index1 ] = '\0';
	}
};
dll_internal ::rux::uint8 rux_is_already_executing_in_current_path( void )
{
	if( ::rux::engine::_globals && ::rux::engine::_globals->_rux_already_executing_in_current_path  )
		return ::rux::engine::_globals->_rux_already_executing_in_current_path();
	return 0;
};
dll_internal void rux_add_search_directory( const char* directory )
{
	if( ::rux::engine::_globals && ::rux::engine::_globals->_rux_add_search_directory )
		::rux::engine::_globals->_rux_add_search_directory( directory );
};
dll_internal void rux_get_search_directories( ::rux::uint32& count , char directories[ 16 ][ 512 ] )
{
	if( ::rux::engine::_globals && ::rux::engine::_globals->_rux_get_search_directory
		&& ::rux::engine::_globals->_rux_get_search_directory_count )
	{
		count = ::rux::engine::_globals->_rux_get_search_directory_count();
		declare_stack_variable( char , directory , 512 );
		for( ::rux::uint32 index0 = 0 ; index0 < count ; index0++ )
		{
			::rux::engine::_globals->_rux_get_search_directory( index0 , directory );
			::memcpy( directories[ index0 ] , directory , strlen( directory ) + 1 );
		}
	}
};
dll_internal void rux_concatenation( char* destination , const char* string1 , const char* string2 )
{
	size_t length1 = strlen( string1 );
	if( length1 > 0 )
		::memcpy( destination , string1 , length1 );
	size_t length2 = strlen( string2 );
	if( length2 > 0 )
		::memcpy( &destination[ length1 ] , string2 , length2 );
	destination[ length1 + length2 ] = 0;
};
dll_internal void rux_concatenation( char* destination , const char* string2 )
{
	size_t length1 = strlen( destination );	
	size_t length2 = strlen( string2 );
	if( length2 > 0 )
		::memcpy( &destination[ length1 ] , string2 , length2 );
	destination[ length1 + length2 ] = 0;
};
dll_internal void rux_replace( char* destination , char old_char , char new_char )
{
	char letter = ' ';
	::rux::uint32 index0 = 0;
	do
	{
		letter = destination[ index0 ];
		if( letter == old_char )
			destination[ index0 ] = new_char;
		index0++;
	}
	while( letter != '\0' );
};
dll_internal ::rux::int32 rux_index_of( const char* destination , char letter )
{
	char letter0 = ' ';
	::rux::int32 index0 = 0 , result = -1;
	do
	{
		letter0 = destination[ index0 ];
		if( letter0 == letter )
		{
			result = index0;
			break;
		}
		index0++;
	}
	while( letter0 != '\0' );
	return result;
};
dll_internal ::rux::int32 rux_index_of( const char* destination , const char* to_find )
{
	::rux::int32 result = -1;
	if(destination && to_find && *destination != '\0' && *to_find != '\0')
	{
		char letter0 = ' ' , letter1 = to_find[ 0 ];
		::rux::int32 index0 = 0 , index1 = 0;
		do
		{
			letter0 = destination[ index0 ];
			if( letter0 == letter1 )
			{
				index1 = 1;
				do
				{
					letter0 = destination[ index0 + index1 ];
					letter1 = to_find[ index1 ];
					if( letter0 != letter1 )
					{
						if( letter1 == '\0' )
							result = index0;
						break;				
					}
					else if( letter0 == '\0' )
					{
						result = index0;
						break;
					}
					index1++;
				}
				while( letter0 != '\0' && letter1 != '\0' );
				if( result != -1 )
					break;
				else
					letter1 = to_find[ 0 ];
			}
			index0++;
		}
		while( letter0 != '\0' );
	}
	return result;
};
dll_internal ::rux::int32 rux_last_index_of( const char* destination , char letter )
{
	char letter0 = ' ';
	::rux::int32 index0 = 0 , result = -1;
	do
	{
		letter0 = destination[ index0 ];
		if( letter0 == letter )
			result = index0;
		index0++;
	}
	while( letter0 != '\0' );
	return result;
};
dll_internal void rux_string_remove_all( char* destination , char letter )
{	
	size_t offset = 0;
	size_t length = strlen( destination );
	while( offset < length )
	{
		if( destination[ offset ] == letter )
		{
			if( offset + 1 < length )
				::memmove( &destination[ offset ] , &destination[ offset + 1 ] , length - offset );
			else
				destination[ offset ] = '\0';
			length--;
		}
		else
			offset++;
	}
};
dll_internal void rux_substring( char* destination , const char* source , ::rux::uint32 start_index , ::rux::uint32 length )
{
	if( length > 0 )
	{
		size_t source_length = strlen( source );
		if( start_index < source_length )
		{
			if( start_index + length > source_length )
				length = source_length - start_index;
			if( length > 0 )
			{
				if( destination == source )
				{
					declare_stack_variable( char , temp , 2048 );
					::rux::engine::memcpy( temp , 0 , &source[ start_index ] , length );
					temp[ length ] = '\0';
					::rux::engine::memcpy( destination , 0 , temp , length + 1 );
				}
				else
				{
					::rux::engine::memcpy( destination , 0 , &source[ start_index ] , length );
					destination[ length ] = '\0';
				}
			}
			else
				destination[ 0 ] = '\0';
		}
	}
};
dll_internal ::rux::uint8 rux_is_exists_fifo( const char* filename )
{
	::rux::uint8 is_file_exists = 0;
	is_file_exists = rux_access( filename , 0 ) != -1 ? 1 : 0;
	if( is_file_exists == 1 )
	{
		struct stat st;
		if( stat( filename , &st ) == 0 )
		{
#ifdef __UNIX__
			if( S_ISFIFO( st.st_mode ) == 0 )
				is_file_exists = 0;
#elif defined( __WINDOWS__ )
			if( ( st.st_mode & _S_IFREG ) == 0 )
				is_file_exists = 0;		
#endif
		}
	}
	return is_file_exists;
};
dll_internal ::rux::uint8 rux_is_exists_file_or_link_or_fifo( const char* filename )
{
	::rux::uint8 is_file_exists = 0;
	is_file_exists = rux_access( filename , 0 ) != -1 ? 1 : 0;
	if( is_file_exists == 1 )
	{
		struct stat st;
		if( stat( filename , &st ) == 0 )
		{
#ifdef __UNIX__
			if( S_ISREG( st.st_mode ) == 0
				&& S_ISLNK( st.st_mode ) == 0
				&& S_ISFIFO( st.st_mode ) == 0 )
				is_file_exists = 0;
#elif defined( __WINDOWS__ )
			if( ( st.st_mode & _S_IFREG ) == 0 )
				is_file_exists = 0;		
#endif
		}
	}
	return is_file_exists;
};
dll_internal ::rux::uint8 rux_is_exists_file_or_link( const char* filename )
{	
	::rux::uint8 is_file_exists = 0;
	is_file_exists = rux_access( filename , 0 ) != -1 ? 1 : 0;
	if( is_file_exists == 1 )
	{
		struct stat st;
		if( stat( filename , &st ) == 0 )
		{
#ifdef __UNIX__
			if( S_ISREG( st.st_mode ) == 0
				&& S_ISLNK( st.st_mode ) == 0 )
				is_file_exists = 0;
#elif defined( __WINDOWS__ )
			if( ( st.st_mode & _S_IFREG ) == 0 )
				is_file_exists = 0;		
#endif
		}
	}
	return is_file_exists;
};
dll_internal ::rux::uint8 rux_is_exists_file( const char* filename )
{	
	::rux::uint8 is_file_exists = 0;
	is_file_exists = rux_access( filename , 0 ) != -1 ? 1 : 0;
	if( is_file_exists == 1 )
	{
		struct stat st;
		if( stat( filename , &st ) == 0 )
		{
#ifdef __UNIX__
			if( S_ISREG( st.st_mode ) == 0 )
				is_file_exists = 0;
#elif defined( __WINDOWS__ )
			if( ( st.st_mode & _S_IFREG ) == 0 )
				is_file_exists = 0;		
#endif
		}
	}
	return is_file_exists;
};
dll_internal void rux_remove_file( const char* filename )
{
	if( rux_is_exists_file( filename ) == 1 )
	{
		remove( filename );
	}
};
dll_internal void rux_clear_and_write_to_file( const char* filename , const char* text )
{
	::rux::int32 file = rux_open( filename , O_CREAT | O_WRONLY | O_TRUNC );
	if( file != -1 )
	{		
		rux_write( file , text , strlen( text ) );
#ifdef __UNIX__
		fsync( file );
#endif
		rux_close( file );
	}
};
dll_internal void rux_split( const char* string , size_t string_size , const char* delimiter , char splits[ 64 ][ 64 ] , size_t& splits_count )
{
	size_t offset = 0 , split_begin = 0;
	size_t delimiter_size = strlen( delimiter );
	splits_count = 0;
	while( offset != string_size && string[ offset ] != '\0' )
	{
		if( offset + delimiter_size < string_size
			&& memcmp( &string[ offset ] , delimiter , delimiter_size ) == 0 )
		{
			if( offset > split_begin )
			{
				::rux::engine::memcpy( splits[ splits_count ] , 0 , &string[ split_begin ] , offset - split_begin );
				splits[ splits_count ][ offset - split_begin ] = '\0';
				splits_count++;
			}
			split_begin = offset + delimiter_size;
		}		
		offset++;
	}
	if( offset > split_begin )
	{
		::rux::engine::memcpy( splits[ splits_count ] , 0 , &string[ split_begin ] , offset - split_begin );		
		splits[ splits_count ][ offset - split_begin ] = '\0';
		splits_count++;
	}
};
template< class T , class char_class >
dll_internal T string_to_number( const char_class* string , size_t string_size = SIZE_MAX , ::rux::byte is_unsigned = 1 )
{	
	T val = 0;
	if( string_size == SIZE_MAX )
	{
		char_class* ptr = const_cast< char_class* >( string );
		T factor = 1;
		if( is_unsigned == 0 && string[ 0 ] == '-' )
			ptr++;
		while( *ptr != 0 )
		{			
			if( *ptr > 47 && *ptr < 58 )
			{
				val *= factor;
				val += *ptr - 48;
			}
			else
				break;
			if( factor == 1 )
				factor = 10;
			ptr++;
		}
		if( is_unsigned == 0 && string[ 0 ] == '-' )
			val *= (T)-1;
	}
	else
	{
		T factor = 1;
		size_t offset = 0;
		if( is_unsigned == 0 && string[ 0 ] == '-' )
			offset = 1;
		while( offset != string_size )
		{	
			if( string[ offset ] > 47 && string[ offset ] < 58 )
			{
				val *= factor;
				val += string[ offset ] - 48;
			}
			else
				break;
			if( factor == 1 )
				factor = 10;
			offset++;
		}
		if( is_unsigned == 0 && string[ 0 ] == '-' )
			val *= (T)-1;
	}
	return val;
};
#ifdef __WINDOWS__
dll_internal DWORD rux_get_pid( void )
{
	return GetCurrentProcessId();
};
dll_internal size_t rux_get_process_virtual_memory( DWORD )
{
	return 0;
};
dll_internal size_t rux_get_process_detailed_virtual_memory( DWORD pid , char modules[ 512 ][ 512 ] , size_t modules_vm[ 512 ] , size_t& modules_count )
{
	modules_count = 0;
	declare_stack_variable( char , filename , 1024 );
	declare_stack_variable( char , pid_string , 32 );
	rux::string::uint32_to_string( pid , pid_string );
	rux_concatenation( filename , "E:/PUBLIC" );
	rux_concatenation( filename , "/smaps" );
	size_t readen_size = 0 , virtual_size = 0;
	char* result = (char*)rux_read_all_from_file( filename , readen_size );
	if( result )
	{
		size_t offset = 0;
		if( readen_size > 3 && memcmp( result , _rux_utf8_header , 3 ) == 0 )
			offset = 3;
		size_t begin = 0 , splits_count = 0 , splits_len = 0 , vm = 0 , modules_index = 0;
		char splits[ 64 ][ 64 ] = { 0 , 0 };
		declare_stack_variable( char , current_module , 64 );
		do
		{
			begin = offset;			
			while( offset != readen_size && result[ offset ] != '\0' && result[ offset ] != '\n' )
				offset++;
			if( offset > begin )
			{				
				rux_split( &result[ begin ] , offset - begin , " " , splits , splits_count );
				if( splits_count > 0 )
				{
					splits_len = strlen( splits[ splits_count - 1 ] );
					if( splits_len >= 2 
						&& memcmp( splits[ splits_count - 1 ] , "kB" , 2 ) == 0 )
					{
						if( memcmp( splits[ 0 ] , "Size:" , 5 ) == 0 )
						{
							if( splits_count > 1 )
							{
								splits_len = strlen( splits[ 1 ] );
								vm = rux::string::to_size_t( splits[ 1 ] , splits_len );
								splits_len = strlen( current_module );
								for( modules_index = 0 ; modules_index < modules_count ; modules_index++ )
								{
									if( strlen( modules[ modules_index ] ) == splits_len
										&& memcmp( modules[ modules_index ] , current_module , splits_len ) == 0 )
									{
										modules_vm[ modules_index ] += vm;
										break;
									}
								}
								if( modules_index >= modules_count )
								{
									::rux::engine::memcpy( modules[ modules_index ] , 0 , current_module , splits_len + 1 );
									modules_vm[ modules_index ] = vm;
									modules_count++;
								}
								virtual_size += vm;
							}
						}
					}
					else if( splits_count == 6 )
						::rux::engine::memcpy( current_module , 0 , splits[ 5 ] , splits_len + 1 );
					else
						::rux::engine::memcpy( current_module , 0 , "anonymous" , 10 );
				}
			}
			if( offset == readen_size || result[ offset ] == '\0' )
				break;
			offset++;
		}
		while( result[ offset ] != '\0' );
		::rux::engine::free_mem( result );
	}
	return virtual_size;
};
#elif defined( __UNIX__ )
dll_internal ::rux::pid_t rux_get_pid( void )
{
	return getpid();
};
dll_internal size_t rux_get_process_virtual_memory( ::rux::pid_t pid )
{
	declare_stack_variable( char , filename , 1024 );
	declare_stack_variable( char , pid_string , 32 );
	rux::string::uint32_to_string( pid , pid_string );
	rux_concatenation( filename , "/proc/" , pid_string );
	rux_concatenation( filename , "/smaps" );
	size_t readen_size = 0 , virtual_size = 0;
	char* result = (char*)rux_read_all_from_file( filename , readen_size );
	if( result )
	{
		size_t offset = 0 , memory_offset = 0 , memory_size = 0;
		if( readen_size > 3 && memcmp( result , _rux_utf8_header , 3 ) == 0 )
			offset = 3;
		do
		{
			if( offset > 0 && result[ offset - 1 ] == '\n' && offset + 4 < readen_size && memcmp( &result[ offset ] , "Size" , 4 ) == 0 )
			{
				do
				{
					offset++;
				}
				while( offset != readen_size 
					&& result[ offset ] != '\n'
					&& result[ offset ] != '\0' 
					&& result[ offset ] != 'k' );
				if( offset == readen_size || result[ offset ] == '\0' )
					offset--;
				if( result[ offset ] == 'k' )
				{
					memory_offset = offset;
					offset++;
					if( result[ offset ] == 'B' )
					{
						offset++;
						if( result[ offset ] == '\n' )
						{
							if( memory_offset > 0 
								&& ( result[ memory_offset - 1 ] == ' ' 
								|| result[ memory_offset - 1 ] == '\t'  ) )
							{
								if( memory_offset > 1)
								{
									memory_offset -= 2;
									memory_size = 0;
									while( memory_offset > 0 
										&& result[ memory_offset ] != ' '
										&& result[ memory_offset ] != '\t' )
									{
										memory_size++;
										memory_offset--;
									}
									if( memory_size > 0 )
									{
										memory_offset++;
										virtual_size += rux::string::to_size_t( &result[ memory_offset ] , memory_size );
									}
								}
							}
						}
					}
				}
			}
			offset++;
		}
		while( offset == readen_size || result[ offset ] != 0 );
		::rux::engine::free_mem( result );
	}
	return virtual_size;
};
dll_internal size_t rux_get_process_detailed_virtual_memory( ::rux::pid_t pid , char modules[ 512 ][ 512 ] , size_t modules_vm[ 512 ] , size_t& modules_count )
{
	modules_count = 0;
	declare_stack_variable( char , filename , 1024 );
	declare_stack_variable( char , pid_string , 32 );
	rux::string::uint32_to_string( pid , pid_string );
	rux_concatenation( filename , "/proc/" , pid_string );
	rux_concatenation( filename , "/smaps" );
	size_t readen_size = 0 , virtual_size = 0;
	char* result = (char*)rux_read_all_from_file( filename , readen_size );
	if( result )
	{
		size_t offset = 0 , memory_offset = 0 , memory_size = 0;
		if( readen_size > 3 && memcmp( result , _rux_utf8_header , 3 ) == 0 )
			offset = 3;
		size_t begin = 0 , splits_count = 0 , splits_len = 0 , vm = 0 , modules_index = 0;
		char splits[ 64 ][ 64 ] = { 0 , 0 };
		declare_stack_variable( char , current_module , 64 );
		do
		{
			begin = offset;			
			while( offset != readen_size && result[ offset ] != '\0' && result[ offset ] != '\n' )
				offset++;
			if( offset > begin )
			{				
				rux_split( &result[ begin ] , offset - begin , " " , splits , splits_count );
				if( splits_count > 0 )
				{
					splits_len = strlen( splits[ splits_count - 1 ] );
					if( splits_len >= 2 
						&& memcmp( splits[ splits_count - 1 ] , "kB" , 2 ) == 0 )
					{
						if( memcmp( splits[ 0 ] , "Size:" , 5 ) == 0 )
						{
							if( splits_count > 1 )
							{
								splits_len = strlen( splits[ 1 ] );
								vm = rux::string::to_size_t( splits[ 1 ] , splits_len );
								splits_len = strlen( current_module );
								for( modules_index = 0 ; modules_index < modules_count ; modules_index++ )
								{
									if( strlen( modules[ modules_index ] ) == splits_len
										&& memcmp( modules[ modules_index ] , current_module , splits_len ) == 0 )
									{
										modules_vm[ modules_index ] += vm;
										break;
									}
								}
								if( modules_index >= modules_count )
								{
									::rux::engine::memcpy( modules[ modules_index ] , 0 , current_module , splits_len + 1 );
									modules_vm[ modules_index ] = vm;
									modules_count++;
								}
								virtual_size += vm;
							}
						}
					}
					else if( splits_count == 6 )
						::rux::engine::memcpy( current_module , 0 , splits[ 5 ] , splits_len + 1 );
					else
						::rux::engine::memcpy( current_module , 0 , "anonymous" , 10 );
				}
			}
			if( offset == readen_size || result[ offset ] == '\0' )
				break;
			offset++;
		}
		while( offset < readen_size && result[ offset ] != '\0' );
		::rux::engine::free_mem( result );
	}
	return virtual_size;
};
#endif
dll_internal void* rux_read_all_from_file( const char* filename , size_t& readen_size )
{
	readen_size = 0;
	::rux::uint8* result = NULL;
	::rux::uint8 is_exists = rux_is_exists_file( filename );
	::rux::int32 file = -1;
	if( is_exists == 1 )
	{
#ifdef __WINDOWS__
		file = rux_open( filename , O_RDONLY | O_BINARY );
#elif defined( __UNIX__ )
		file = rux_open( filename , O_RDONLY );
#endif
		if( file != -1 )
		{			
			size_t size = 1024 , avail_size = 1024 , read_size = 0;
			result = alloc_array_macros( ::rux::uint8 , size );
			do
			{
				readen_size += read_size;
				avail_size -= read_size;
				if( avail_size == 0 )
				{
					size += 1024;
					result = rux::engine::realloc_array< ::rux::uint8 >( result , size );
					avail_size = 1024;
				}			
#ifdef __WINDOWS__
				read_size = _read( file , &result[ readen_size ] , avail_size ); 
#else
				read_size = read( file , &result[ readen_size ] , avail_size ); 
#endif
			}
			while( read_size != 0 && read_size != -1 );
			rux_close( file );
		}
	}
	return result;
};
dll_internal void rux_append_to_file( const char* filename , const char* text )
{
	::rux::uint8 is_exists = rux_is_exists_file( filename );
	::rux::int32 file = -1;
	if( is_exists == 1 )
		file = rux_open( filename , O_WRONLY | O_APPEND );
	else
		file = rux_open( filename , O_CREAT | O_WRONLY | O_TRUNC );
	if( file != -1 )
	{
		rux_lseek( file , 0L , SEEK_END );
		rux_write( file , text , strlen( text ) );
#ifdef __UNIX__
		fsync( file );
#endif
		rux_close( file );
	}
};
dll_internal void rux_append_to_file( const char* filename , const ::rux::uint8* bytes , size_t bytes_size )
{
	::rux::uint8 is_exists = rux_is_exists_file( filename );
	::rux::int32 file = -1;
	if( is_exists == 1 )
		file = rux_open( filename , O_WRONLY | O_APPEND );
	else
		file = rux_open( filename , O_CREAT | O_WRONLY | O_TRUNC );
	if( file != -1 )
	{
		rux_lseek( file , 0L , SEEK_END );
		rux_write( file , bytes , bytes_size );
#ifdef __UNIX__
		fsync( file );
#endif
		rux_close( file );
	}
};
dll_internal void rux_get_executable_filename( char executable_filename[] )
{	
#ifdef __WINDOWS__
	HMODULE module_handle = GetModuleHandleA( NULL );
	if( module_handle )
	{
		declare_stack_variable( char , module_path , 4096 );
		if( GetModuleFileNameA( module_handle , module_path , 4096 ) != 0 )
		{			
			::rux::engine::memcpy( executable_filename , 0 , module_path , strlen( module_path ) );
			executable_filename[ strlen( module_path ) ] = '\0';			
		}				
	}	
#endif
#ifdef __UNIX__
	declare_stack_variable( char , module_path , 4096 );
#ifdef __LINUX__
	if( readlink( "/proc/self/exe" , module_path , 4095 ) > 0 )
#elif defined( __SOLARIS__ )
	declare_stack_variable( char , key_ptr , 64 );
	::rux::uint32 pid = getpid() , number = 0 , offset = 0;
	const char* const_string = NULL;
	const_string = "/proc/";
	::rux::engine::memcpy( key_ptr , offset , const_string , 6 );
	offset += 6;
	rux::string::uint32_to_string( pid , &key_ptr[ offset ] );
	offset = strlen( key_ptr );
	const_string = "/path/a.out";
	::rux::engine::memcpy( key_ptr , offset , const_string , 11 );
	offset += 11;
	key_ptr[ offset ] = '\0';	
	if( readlink( key_ptr , module_path , 4095 ) > 0 )
#endif
	{		
		::rux::engine::memcpy( executable_filename , 0 , module_path , strlen( module_path ) );
		executable_filename[ strlen( module_path ) ] = '\0';			
	}
#endif
};
dll_internal void rux_get_executable_directory( char module_directory[] )
{	
#ifdef __WINDOWS__
	HMODULE module_handle = GetModuleHandleA( NULL );
	if( module_handle )
	{
		declare_stack_variable( char , module_path , 4096 );
		if( GetModuleFileNameA( module_handle , module_path , 4096 ) != 0 )
		{
			::rux::uint32 index0 = 0 , index1 = 0;					
			for( ; ; )
			{						
				if( module_path[ index0 ] == '\0' ) break;		
				else if( module_path[ index0 ] == '\\' )
					index1 = index0;
				index0++;
			}
			::rux::engine::memcpy( module_directory , 0 , module_path , index1 );
			module_directory[ index1 ] = '\0';			
		}				
	}	
#endif
#ifdef __UNIX__
	char module_path[ 4096 ] = {0};
#ifdef __LINUX__
	if( readlink( "/proc/self/exe" , module_path , 4095 ) > 0 )
#elif defined( __SOLARIS__ )
	char key_ptr[ 64 ] = {0};
	::rux::uint32 pid = getpid() , number = 0 , offset = 0;
	const char* const_string = NULL;
	const_string = "/proc/";
	::memcpy( key_ptr , const_string , 6 );
	offset += 6;
	rux::string::uint32_to_string( pid , &key_ptr[ offset ] );
	offset = strlen( key_ptr );
	const_string = "/path/a.out";
	::memcpy( &key_ptr[ offset ] , const_string , 11 );
	offset += 11;
	key_ptr[ offset ] = 0;
	if( readlink( key_ptr , module_path , 4095 ) > 0 )
#endif
	{
		::rux::uint32 index0 = 0 , index1 = 0;					
		for( ; ; )
		{						
			if( module_path[ index0 ] == '\0' ) break;		
			else if( module_path[ index0 ] == '/' )
				index1 = index0;
			index0++;
		}		
		::memcpy( module_directory , module_path , index1 );
		module_directory[ index1 ] = 0;
	}
#endif
};
dll_internal ::rux::uint32 rux_days_in_month( ::rux::uint32 month , ::rux::uint32 year )
{
	return ( 30 + ( ( ( month & 9 ) == 8 ) || ( ( month & 9 ) == 1 ) ) - ( month == 2 ) - ( !( ( ( year % 4 ) == 0 ) && ( ( ( year % 100 ) != 0 ) || ( ( year % 400 ) == 0 ) ) ) && ( month == 2 ) ) );
};
dll_internal ::rux::uint32 rux_days_in_year( ::rux::uint32 year )
{
	::rux::uint32 days = 0;
	for( ::rux::uint32 month = 1 ; month < 13 ; month++ )
	{
		days += rux_days_in_month( month , year );
	}
	return days;
};
dll_internal ::rux::uint64 rux_get_now_as_unix_time( void )
{
	#ifdef __WINDOWS__
	::rux::uint64 time = 0;
	FILETIME now;
	GetSystemTimeAsFileTime( (LPFILETIME)&now );
	time = now.dwHighDateTime;
	time <<= 32;
	time |= now.dwLowDateTime;
	time /= 10;
	time -= DELTA_EPOCH_IN_USEC;
	return time;
#endif
#ifdef __UNIX__
	struct timespec ts;
	clock_gettime( CLOCK_REALTIME , &ts );
	return ts.tv_sec * 1000000LL + ( ts.tv_nsec / 1000ULL );
#endif
};
dll_internal ::rux::uint64 rux_get_now_as_local_unix_time( void )
{
#ifdef __WINDOWS__
	::rux::uint64 time = 0;	
  FILETIME local_file_time;
  GetSystemTimeAsFileTime((LPFILETIME)&time);
  FileTimeToLocalFileTime((LPFILETIME)&time, &local_file_time);	
	time = local_file_time.dwHighDateTime;
	time <<= 32;
	time |= local_file_time.dwLowDateTime;
	time /= 10;
	time -= DELTA_EPOCH_IN_USEC;
	return time;
#endif
#ifdef __UNIX__
	struct timespec ts;
	clock_gettime( CLOCK_REALTIME , &ts );
	return ( ts.tv_sec - timezone ) * 1000000LL + ( ts.tv_nsec / 1000ULL );
#endif
};
dll_internal void rux_from_big_endian_to_little_endian( ::rux::uint8* source_ptr , ::rux::uint8* destination_ptr , ::rux::uint32 length )
{
	::rux::uint32 decrementer = length - 1;
	::rux::uint8 byte = 0x00;
	if( source_ptr == destination_ptr )
	{
		for( ::rux::uint32 incrementer = 0 ; incrementer < length ; incrementer++ )
		{
			byte = source_ptr[ incrementer ];
			source_ptr[ incrementer ] = destination_ptr[ decrementer ];
			destination_ptr[ decrementer ] = byte;
			decrementer--;
		}
	}
	else
	{
		for( ::rux::uint32 incrementer = 0 ; incrementer < length ; incrementer++ )
		{			
			destination_ptr[ decrementer ] = source_ptr[ incrementer ];
			decrementer--;
		}
	}
};
dll_internal ::rux::int64 rux_from_memory_address_to_int64( void* memory_address )
{
	::rux::int64 mem_addr = (::rux::int64)memory_address;
#ifdef __WINDOWS__
#ifdef __x86__
	memset( &((::rux::uint8*)&mem_addr)[ sizeof( void* ) ] , 0 , sizeof( ::rux::int64 ) - sizeof( void* ) );
#endif
#endif
#ifdef __UNIX__
	if( sizeof( void* ) < sizeof( ::rux::int64 ) )
		memset( &((::rux::uint8*)&mem_addr)[ sizeof( void* ) ] , 0 , sizeof( ::rux::int64 ) - sizeof( void* ) );
#endif
	return mem_addr;
};
dll_internal char* rux_get_module_directory( void )
{
	char* module_directory = NULL;
#ifdef __WINDOWS__
	HMODULE module_handle = GetModuleHandleA( g_current_module_name );
	if( module_handle )
	{
		declare_stack_variable( char , module_path , 4096 );
		if( GetModuleFileNameA( module_handle , module_path , 4096 ) != 0 )
		{
			::rux::uint32 index0 = 0 , index1 = 0;					
			for( ; ; )
			{						
				if( module_path[ index0 ] == '\0' ) break;		
				else if( module_path[ index0 ] == '\\' )
					index1 = index0;
				index0++;
			}					
			module_directory = static_cast<char*>( ::rux::engine::_rux_alloc( sizeof( char ) * ( index1 + 1 ) , typeid( char ).name() , __FILE__ , __LINE__ , _rux_current_module_index , NULL ) );
			::rux::engine::memcpy( module_directory , 0 , module_path , index1 );
			module_directory[ index1 ] = '\0';			
		}				
	}	
#endif
#ifdef __UNIX__
	Dl_info info;
	declare_stack_variable( char , module_path , 4096 );
	if( dladdr( (void*)g_current_module_name , &info ) != 0 )
	{
		::rux::uint32 index0 = 0 , index1 = 0;					
		for( ; ; )
		{						
			if( info.dli_fname[ index0 ] == '\0' ) break;		
			else if( info.dli_fname[ index0 ] == '/' )
				index1 = index0;
			index0++;
		}
		module_directory = static_cast<char*>( ::rux::engine::_rux_alloc( sizeof( char ) * ( index1 + 1 ) , typeid( char ).name() , __FILE__ , __LINE__ , _rux_current_module_index , NULL ) );
		::rux::engine::memcpy( module_directory , 0 , module_path , index1 );
		module_directory[ index1 ] = '\0';			
	}	
	else
	{				
#ifdef __LINUX__
		if( readlink( "/proc/self/exe" , module_path , 4095 ) > 0 )
#elif defined( __SOLARIS__ )
		declare_stack_variable( char , key_ptr , 64 );
		::rux::safe_sprintf( key_ptr , 64 , "/proc/%ld/path/a.out" , getpid() );
		if( readlink( key_ptr , module_path , 4095 ) > 0 )
#endif
		{
			::rux::uint32 index0 = 0 , index1 = 0;					
			for( ; ; )
			{						
				if( module_path[ index0 ] == '\0' ) break;		
				else if( module_path[ index0 ] == '/' )
					index1 = index0;
				index0++;
			}
			module_directory = static_cast<char*>( ::rux::engine::_rux_alloc( sizeof( char ) * ( index1 + 1 ) , typeid( char ).name() , __FILE__ , __LINE__ , _rux_current_module_index , NULL ) );
			::rux::engine::memcpy( module_directory , 0 , module_path , index1 );
			module_directory[ index1 ] = '\0';				
		}	
	}	
#endif
	return module_directory;
};
dll_internal ::rux::uint8 rux_start_detached_process( const char* executable , const char* arguments_string , ::rux::uint8 show , rux_start_process_method start_process_method )
{
	unsigned long result = 0;
	if( executable )
	{					
#ifdef __WINDOWS__					
		declare_stack_variable( char , cmd_line , 2048 );
		if( strlen( executable ) > 0 )
			rux_concatenation( cmd_line , executable , " " );
		if( arguments_string )
			rux_concatenation( cmd_line , arguments_string );
		if( start_process_method == rux_start_process_method_create_process )
		{
			PROCESS_INFORMATION  process_information;
			memset( (void*)&process_information , 0 , sizeof( PROCESS_INFORMATION ) );			
			STARTUPINFOA startup_info;
			memset( (void*)&startup_info , 0 , sizeof( STARTUPINFOA ) );			
			startup_info.cb = sizeof( STARTUPINFOA );
			startup_info.dwFlags = STARTF_USESHOWWINDOW;
			if( show == 1 )
				startup_info.wShowWindow = SW_SHOW;
			else
				startup_info.wShowWindow = SW_HIDE;
			if( CreateProcessA( NULL , cmd_line , NULL , NULL , FALSE , CREATE_DEFAULT_ERROR_MODE | IDLE_PRIORITY_CLASS | DETACHED_PROCESS , NULL , NULL , &startup_info , &process_information ) == 0 )
				result = GetLastError();
			if( process_information.hProcess )
				CloseHandle( process_information.hProcess );
			if( process_information.hThread )
				CloseHandle( process_information.hThread );
		}
		else if( start_process_method == rux_start_process_method_shell_execute )
		{
			::rux::int32 sw = SW_HIDE;
			if( show == 1 )
				sw = SW_SHOWNORMAL;
			ShellExecuteA( NULL , "open" , executable , cmd_line , NULL , sw );
		}
		else if( start_process_method == rux_start_process_method_create_process_as_user )
		{
			HANDLE Token = NULL;
			HANDLE DupedToken = NULL;		
			void* environment = NULL;        
			HWINSTA winsta_original = NULL;
			HWINSTA winsta_interactive = NULL;
			HDESK desktop = NULL;
			PSID* sid_owner = NULL;
			PSID* sid_group = NULL;
			PACL* dacl = NULL;
			PACL* sacl = NULL;
			PSECURITY_DESCRIPTOR security_descriptor = NULL;
			PROCESS_INFORMATION pi;
			memset( &pi , 0 , sizeof( PROCESS_INFORMATION ) );
			try
			{
				winsta_original = GetProcessWindowStation();
				if (winsta_original != NULL)
				{
					winsta_interactive = OpenWindowStationW(L"WinSta0", FALSE, STANDARD_RIGHTS_REQUIRED | WINSTA_ACCESSCLIPBOARD | WINSTA_ACCESSGLOBALATOMS | WINSTA_CREATEDESKTOP | WINSTA_ENUMDESKTOPS | WINSTA_ENUMERATE | WINSTA_EXITWINDOWS | WINSTA_READATTRIBUTES | WINSTA_READSCREEN | WINSTA_WRITEATTRIBUTES);
					if (winsta_interactive != NULL)
					{
						if (SetProcessWindowStation(winsta_interactive))
						{
							desktop = OpenDesktopW(L"Default", 0, FALSE, DESKTOP_CREATEMENU | DESKTOP_CREATEWINDOW | DESKTOP_ENUMERATE | DESKTOP_HOOKCONTROL | DESKTOP_JOURNALPLAYBACK | DESKTOP_JOURNALRECORD | DESKTOP_READOBJECTS | DESKTOP_SWITCHDESKTOP | DESKTOP_WRITEOBJECTS | STANDARD_RIGHTS_REQUIRED);
							SetProcessWindowStation(winsta_original);
							winsta_original = NULL;
							if (desktop != NULL)
							{
								if (GetSecurityInfo(desktop, SE_WINDOW_OBJECT, DACL_SECURITY_INFORMATION, sid_owner, sid_group, dacl, sacl, &security_descriptor) == 0)
								{
									if (SetSecurityInfo(desktop, SE_WINDOW_OBJECT, DACL_SECURITY_INFORMATION, NULL , NULL , NULL, NULL ) == 0)
									{
										::rux::int32 SessionId = WTSGetActiveConsoleSessionId();
										if (SessionId != 0xffffffff)
										{                        
											if( ::rux::engine::_globals->_wtsapi32_module.WTSQueryUserToken(SessionId, &Token) )
											{                          
												SECURITY_ATTRIBUTES sa;
												memset( &sa , 0 , sizeof( sa ) );
												sa.nLength = sizeof( sa );
												if (DuplicateTokenEx(                          Token,GENERIC_ALL_ACCESS,&sa,														SecurityIdentification,                            TokenPrimary,&DupedToken                            ) )
												{                            
													if (SetTokenInformation(DupedToken, TokenSessionId, &SessionId, sizeof(::rux::uint32)))
													{                              
														if ( ::rux::engine::_globals->_userenv_module.CreateEnvironmentBlock(&environment, DupedToken, FALSE))
														{
															try
															{
																STARTUPINFOA si;
																memset( &si , 0 , sizeof( si ) );
																si.cb = sizeof( si );
																si.lpReserved = NULL;
																si.lpDesktop = "winsta0\\default";
																si.lpTitle = NULL;
																si.cbReserved2 = 0;
																si.lpReserved2 = NULL;
																result = CreateProcessAsUserA( DupedToken , NULL , cmd_line , &sa , &sa , false , CREATE_DEFAULT_ERROR_MODE | CREATE_UNICODE_ENVIRONMENT, environment, NULL , &si , &pi );
																if( result )
																{
																}
																else
																{
																	//rux::uint32 error_uint32 = GetLastError();                                      
																}
															}
															catch ( ... )                                 
															{                                    
															}
															RevertToSelf();															                       
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
			catch ( ...)
			{          
			}
			if (pi.hProcess != NULL) CloseHandle(pi.hProcess);
			if (pi.hThread != NULL) CloseHandle(pi.hThread);
			if (environment != NULL) ::rux::engine::_globals->_userenv_module.DestroyEnvironmentBlock(environment);          
			if (desktop != NULL)
			{
				if (sacl != NULL || dacl != NULL || sid_owner != NULL || sid_group != NULL)
				{
					SetSecurityInfo(desktop, SE_WINDOW_OBJECT, DACL_SECURITY_INFORMATION, sid_owner, sid_group, *dacl, *sacl);
				}
			}
			if (winsta_original != NULL)
				SetProcessWindowStation(winsta_original);
			if (winsta_interactive != NULL) CloseWindowStation(winsta_interactive);
			if (desktop != NULL) CloseDesktop(desktop);
			if (DupedToken != NULL) CloseHandle(DupedToken);
			if (Token != NULL) CloseHandle(Token);    
		}
#elif defined( __UNIX__ )
		::rux::pid_t child_pid = fork();
		if( child_pid == 0 )
		{
			::rux::pid_t detached_pid = fork();
			if( detached_pid < 0 )
				_exit( 2 );
			else if( detached_pid != 0 )
				_exit( 0 );
			else
			{
				umask( 0 );
				::rux::pid_t sid = setsid();
				if( sid < 0 )
					_exit( 1 );	
				if( chdir( "/" ) < 0 )
					_exit( 1 );
				close( STDIN_FILENO );
				close( STDOUT_FILENO );
				close( STDERR_FILENO );
				::rux::int32 stdio_fd = open( "/dev/null" , O_RDWR );
				dup( stdio_fd );
				dup( stdio_fd );
				char* arguments[ 1024 ] = {0};
				size_t arguments_size = 0;
				char* copy = ::rux::arguments_string_to_array( arguments_string , arguments , arguments_size );
				if( arguments_size == 0 )
					execl( executable , executable , (char*)0 );
				else if( arguments_size == 1 )
					execl( executable , executable , arguments[ 0 ] , (char*)0 );
				else if( arguments_size == 2 )
					execl( executable , executable , arguments[ 0 ] , arguments[ 1 ] , (char*)0 );
				else if( arguments_size == 3 )
					execl( executable , executable , arguments[ 0 ] , arguments[ 1 ] , arguments[ 2 ] , (char*)0 );
				else if( arguments_size == 4 )
					execl( executable , executable , arguments[ 0 ] , arguments[ 1 ] , arguments[ 2 ] , arguments[ 3 ] , (char*)0 );
				else if( arguments_size == 5 )
					execl( executable , executable , arguments[ 0 ] , arguments[ 1 ] , arguments[ 2 ] , arguments[ 3 ] , arguments[ 4 ] , (char*)0 );
				else if( arguments_size == 6 )
					execl( executable , executable , arguments[ 0 ] , arguments[ 1 ] , arguments[ 2 ] , arguments[ 3 ] , arguments[ 4 ] , arguments[ 5 ] , (char*)0 );
				else if( arguments_size == 7 )
					execl( executable , executable , arguments[ 0 ] , arguments[ 1 ] , arguments[ 2 ] , arguments[ 3 ] , arguments[ 4 ] , arguments[ 5 ] , arguments[ 6 ] , (char*)0 );
				else if( arguments_size == 8 )
					execl( executable , executable , arguments[ 0 ] , arguments[ 1 ] , arguments[ 2 ] , arguments[ 3 ] , arguments[ 4 ] , arguments[ 5 ] , arguments[ 6 ] , arguments[ 7 ] , (char*)0 );
				else if( arguments_size == 9 )
					execl( executable , executable , arguments[ 0 ] , arguments[ 1 ] , arguments[ 2 ] , arguments[ 3 ] , arguments[ 4 ] , arguments[ 5 ] , arguments[ 6 ] , arguments[ 7 ] , arguments[ 8 ] , (char*)0 );
				else if( arguments_size == 10 )
					execl( executable , executable , arguments[ 0 ] , arguments[ 1 ] , arguments[ 2 ] , arguments[ 3 ] , arguments[ 4 ] , arguments[ 5 ] , arguments[ 6 ] , arguments[ 7 ] , arguments[ 8 ] , arguments[ 9 ] , (char*)0 );
				else
				{
					char** zero_arguments = alloc_array_macros( char* , arguments_size + 2 );
					zero_arguments[ 0 ] = const_cast< char* >( executable );
					for( size_t index0 = 0 ; index0 < arguments_size ; index0++ )
						zero_arguments[ index0 + 1 ] = arguments[ index0 ];
					zero_arguments[ arguments_size + 1 ] = 0;
					execv( executable , zero_arguments );
					::rux::engine::free_mem( zero_arguments );
				}
				_exit( 1 );
			}
		}
		else if( child_pid > 0 )
			waitpid( child_pid , NULL , 0 );
#endif
	}
	return result == 0 ? 1 : 0;
};
dll_internal void rux_get_working_directory( char string[] , size_t string_size )
{
#ifdef __WINDOWS__
	_getcwd( string , string_size );
#elif defined( __UNIX__ )
	getcwd( string , string_size );
#endif
};
dll_internal void rux_network_init( void )
{
	::rux::engine::_globals->_rux_memory_network_init();
};
dll_internal void rux_network_deinit( void )
{
	if( ::rux::engine::_globals && ::rux::engine::_globals->_rux_memory_network_deinit )
		::rux::engine::_globals->_rux_memory_network_deinit();
};
dll_internal ::rux::uint8 rux_is_hex_digit( char letter )
{
	::rux::uint8 yes = 0;
	switch( letter )
	{
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
		{
			yes = 1;
			break;
		}
	}
	return yes;
};
dll_internal ::rux::uint8 rux_hex_string_to_uint32( const char* hex , ::rux::uint32& value , size_t length )
{
	::rux::uint8 result = 1;
	value = 0;
	if( length == 0 )
		length = hex ? strlen( hex ) : 0;
	if( length > 0 )
	{
		::rux::uint8 even = 0;
		if( length % 2 == 0 )
			even = 1;
		::rux::uint32 index0 = 0;
		char letter = 0;
		if( hex[ 0 ] == '0'
			&& ( length > 1 && hex[ 1 ] == 'x' ) )
			index0 = 2;		
		if( even == 0 && index0 < length )
		{
			letter = hex[ index0 ];
			switch( letter )
			{
			case '1':
				{
					value = 1;
					break;
				}
			case '2':
				{
					value = 2;
					break;
				}
			case '3':
				{
					value = 3;
					break;
				}
			case '4':
				{
					value = 4;
					break;
				}
			case '5':
				{
					value = 5;
					break;
				}
			case '6':
				{
					value = 6;
					break;
				}
			case '7':
				{
					value = 7;
					break;
				}
			case '8':
				{
					value = 8;
					break;
				}
			case '9':
				{
					value = 9;
					break;
				}
			case '0':
				{
					value = 0;
					break;
				}
			case 'a':
			case 'A':
				{
					value = 10;
					break;
				}
			case 'b':
			case 'B':
				{
					value = 11;
					break;
				}
			case 'c':
			case 'C':
				{
					value = 12;
					break;
				}
			case 'd':
			case 'D':
				{
					value = 13;
					break;
				}
			case 'e':
			case 'E':
				{
					value = 14;
					break;
				}
			case 'f':
			case 'F':
				{
					value = 15;
					break;
				}
			default:
				{
					result = 0;
					break;
				}
			}			
			index0++;
		}
		if( result == 1 )
		{
			::rux::uint8 byte = 0;
			while( index0 < length )
			{
				value <<= 8;			
				letter = hex[ index0 ];
				switch( letter )
				{
				case '1':
					{
						byte = 1 * 16;
						break;
					}
				case '2':
					{
						byte = 2 * 16;
						break;
					}
				case '3':
					{
						byte = 3 * 16;
						break;
					}
				case '4':
					{
						byte = 4 * 16;
						break;
					}
				case '5':
					{
						byte = 5 * 16;
						break;
					}
				case '6':
					{
						byte = 6 * 16;
						break;
					}
				case '7':
					{
						byte = 7 * 16;
						break;
					}
				case '8':
					{
						byte = 8 * 16;
						break;
					}
				case '9':
					{
						byte = 9 * 16;
						break;
					}
				case '0':
					{
						byte = 0;
						break;
					}
				case 'a':
				case 'A':
					{
						byte = 10 * 16;
						break;
					}
				case 'b':
				case 'B':
					{
						byte = 11 * 16;
						break;
					}
				case 'c':
				case 'C':
					{
						byte = 12 * 16;
						break;
					}
				case 'd':
				case 'D':
					{
						byte = 13 * 16;
						break;
					}
				case 'e':
				case 'E':
					{
						byte = 14 * 16;
						break;
					}
				case 'f':
				case 'F':
					{
						byte = 15 * 16;
						break;
					}
				default:
					{
						result = 0;
						break;
					}
				}
				if( result == 0 )
					break;
				index0++;
				letter = hex[ index0 ];
				switch( letter )
				{
				case '1':
					{
						byte += 1;
						break;
					}
				case '2':
					{
						byte += 2;
						break;
					}
				case '3':
					{
						byte += 3;
						break;
					}
				case '4':
					{
						byte += 4;
						break;
					}
				case '5':
					{
						byte += 5;
						break;
					}
				case '6':
					{
						byte += 6;
						break;
					}
				case '7':
					{
						byte += 7;
						break;
					}
				case '8':
					{
						byte += 8;
						break;
					}
				case '9':
					{
						byte += 9;
						break;
					}
				case '0':
					{
						byte += 0;
						break;
					}
				case 'a':
				case 'A':
					{
						byte += 10;
						break;
					}
				case 'b':
				case 'B':
					{
						byte += 11;
						break;
					}
				case 'c':
				case 'C':
					{
						byte += 12;
						break;
					}
				case 'd':
				case 'D':
					{
						byte += 13;
						break;
					}
				case 'e':
				case 'E':
					{
						byte += 14;
						break;
					}
				case 'f':
				case 'F':
					{
						byte += 15;
						break;
					}
				default:
					{
						result = 0;
						break;
					}
				}
				if( result == 0 )
					break;
				value |= byte;
				index0++;
			}
		}
	}
	else
		result = 0;
	return result;
};
dll_internal ::rux::uint8 rux_get_byte( ::rux::uint64 data , ::rux::uint32 index )
{
	return ( data >> ( index * 8 ) ) & 0x000000ff;
};
dll_internal ::rux::uint8 rux_get_bit( ::rux::uint8* bytes , size_t bit_index )
{
	size_t byte_index = bit_index / 8;
	bit_index -= byte_index * 8;
	if( ( ( bytes[ byte_index ] ) & ( 1 << ( bit_index ) ) ) )
		return 1;
	else
		return 0;
};
dll_internal void rux_unset_bit( ::rux::uint8* bytes , size_t bit_index )
{
	size_t byte_index = bit_index / 8;
	bit_index -= byte_index * 8; 
	bytes[ byte_index ] &= ~( 1 << bit_index );
};
dll_internal void rux_set_bit( ::rux::uint8* bytes , size_t bit_index )
{
	size_t byte_index = bit_index / 8;
	bit_index -= byte_index * 8; 
	bytes[ byte_index ] |= 1 << bit_index;
};
dll_internal void ::rux::engine::free_mem( void* mem_ptr )
{
	::rux::engine::_rux_free( mem_ptr );
};
namespace rux
{
	namespace engine
	{
		dll_internal ::rux::int32 this_module_deinitialize( rux_volatile* current_module_init , rux_volatile* current_module_reference , rux_simple_t stop )
		{
			if( XInterlocked::CompareExchange( current_module_init , 2 , 1 ) != 0 )
			{
				_CS_PTR_LOCK_( ::rux::_cs_current_module , 0 );
				if( XInterlocked::CompareExchange( current_module_init , 0 , 0 ) == 2 )
				{
					if( XInterlocked::Decrement( current_module_reference ) == 0 )
					{
						while( XInterlocked::CompareExchange( &::rux::_current_module_is_locked , 0 , 0 ) != 0 )
						{
						}
						CODE_LABELS_INITIALIZE();
						CODE_LABEL_ANOTHER_CONTEXT( NULL , "stop" , stop );
						if( ::rux::engine::_globals && ::rux::engine::_globals->_rux_stop_threads )
							::rux::engine::_globals->_rux_stop_threads();
						XInterlocked::CompareExchange( current_module_init , 0 , 2 );
					}
					else
						XInterlocked::CompareExchange( current_module_init , 1 , 2 );
				}
				::rux::_cs_current_module->UnLock( 0 );
			}
			return 0;
		};
	};
	namespace io
	{
		dll_internal ::rux::int32 get_errno( void )
		{
			if( ::rux::engine::_globals && ::rux::engine::_globals->_rux_errno )
				return ::rux::engine::_globals->_rux_errno();
			else
				return errno;
		};
	};
};
dll_internal FILE* ::rux::io::fopen( const char * _Filename, const char * _Mode )
{
	if( ::rux::engine::_globals && ::rux::engine::_globals->_rux_fopen )
		return ::rux::engine::_globals->_rux_fopen( _Filename , _Mode );
	else
		return ::fopen( _Filename , _Mode );
};
dll_internal ::rux::int64 rux::io::ftell64( FILE * _File )
{
	if( ::rux::engine::_globals && ::rux::engine::_globals->_rux_ftell64 )
		return ::rux::engine::_globals->_rux_ftell64( _File );
	else
#ifdef __WINDOWS__
		return ::_ftelli64( _File );
#else
#ifdef __ANDROID__
		return ::ftell( _File );
#else
		return ::ftello64( _File );
#endif
#endif
};
dll_internal ::rux::int32 rux::io::fseek64( FILE* _File , ::rux::int64 _Offset , ::rux::int32 _Origin )
{
	if( ::rux::engine::_globals && ::rux::engine::_globals->_rux_fseek64 )
		return ::rux::engine::_globals->_rux_fseek64( _File , _Offset , _Origin );
	else
#ifdef __WINDOWS__
		return ::_fseeki64( _File , _Offset , _Origin );
#else
#ifdef __ANDROID__
		return ::lseek64( fileno( _File ) , _Offset , _Origin );
#else
		return ::fseeko64( _File , _Offset , _Origin );
#endif
#endif
};
dll_internal FILE* ::rux::io::freopen( const char * _Filename , const char * _Mode , FILE* _File )
{
	if( ::rux::engine::_globals && ::rux::engine::_globals->_rux_freopen )
		return ::rux::engine::_globals->_rux_freopen( _Filename , _Mode , _File );
	else
		return ::freopen( _Filename , _Mode , _File );
};
dll_internal size_t rux::io::fwrite( const void* _Str , size_t _Size , size_t _Count , FILE* _File )
{
	if( ::rux::engine::_globals && ::rux::engine::_globals->_rux_fwrite )
		return ::rux::engine::_globals->_rux_fwrite( _Str , _Size , _Count , _File );
	else
		return ::fwrite( _Str , _Size , _Count , _File );
};
dll_internal size_t rux::io::fread( void* _DstBuf , size_t _ElementSize , size_t _Count, FILE* _File )
{
	if( ::rux::engine::_globals && ::rux::engine::_globals->_rux_fread )
		return ::rux::engine::_globals->_rux_fread( _DstBuf , _ElementSize , _Count, _File );
	else
		return ::fread( _DstBuf , _ElementSize , _Count, _File );
};
dll_internal ::rux::int32 rux::io::_ferror( FILE* _File )
{
	if( ::rux::engine::_globals && ::rux::engine::_globals->_rux_ferror )
		return ::rux::engine::_globals->_rux_ferror( _File );
	else
#ifdef __ANDROID__
		return ferror( _File );
#else
		return ::ferror( _File );
#endif
};
dll_internal ::rux::int32 rux::io::_feof( FILE* _File )
{
	if( ::rux::engine::_globals && ::rux::engine::_globals->_rux_feof )
		return ::rux::engine::_globals->_rux_feof( _File );
	else
#ifdef __ANDROID__
		return feof( _File );
#else
		return ::feof( _File );
#endif
};
dll_internal int rux::io::vfprintf( FILE* stream , const char* format , va_list arg )
{
	if( ::rux::engine::_globals && ::rux::engine::_globals->_vfprintf )
		return ::rux::engine::_globals->_vfprintf( stream , format , arg );
	else
		return ::vfprintf( stream , format , arg );
};
dll_internal ::rux::int32 rux::io::fputs( const char* _Str, FILE* _File )
{
	if( ::rux::engine::_globals && ::rux::engine::_globals->_rux_fputs )
		return ::rux::engine::_globals->_rux_fputs( _Str , _File );
	else
		return ::fputs( _Str , _File );
};
dll_internal ::rux::int32 rux::io::fflush( FILE* _File )
{
	if( ::rux::engine::_globals && ::rux::engine::_globals->_rux_fflush )
		return ::rux::engine::_globals->_rux_fflush( _File );
	else
		return ::fflush( _File );
};
dll_internal ::rux::int32 rux::io::chsize( ::rux::int32 _FileHandle, long _Size )
{
	if( ::rux::engine::_globals && ::rux::engine::_globals->_rux_chsize )
		return ::rux::engine::_globals->_rux_chsize( _FileHandle , _Size );
	else
#ifdef __WINDOWS__
		return _chsize( _FileHandle , _Size );
#else
		return ftruncate( _FileHandle , _Size );
#endif
};
dll_internal ::rux::int32 rux::io::fclose( FILE * _File )
{
	if( ::rux::engine::_globals && ::rux::engine::_globals->_rux_fclose )
		return ::rux::engine::_globals->_rux_fclose( _File );
	else
		return ::fclose( _File );
};
dll_internal void rux_add_deinitialization_func( rux_simple_t func )
{
	CS_PTR_LOCK( _cs_rux_deinitialization_funcs );
	if( _rux_deinitialization_funcs_count == 0 )
		memset( _rux_deinitialization_funcs , 0 , 16 * sizeof( rux_simple_t ) );
	for( ::rux::uint8 index0 = 0 ; index0 < 16 ; index0++ )
	{
		if( _rux_deinitialization_funcs[ index0 ] ==  NULL )
		{
			_rux_deinitialization_funcs_count++;
			_rux_deinitialization_funcs[ index0 ] = func;
			break;
		}
	}
	_cs_rux_deinitialization_funcs->UnLock();
};
#ifdef __x86__
rux_runtime_param_t::rux_runtime_param_t()
{
	_value0 = 0;
	_value1 = 0;
	_type = XEnum_ReturnValueType_Default;
};
rux_runtime_param_t& rux_runtime_param_t::operator =( const char* value )
{
	_value0 = (::rux::uint32)value;
	_type = XEnum_ReturnValueType_Default;
	return *this;
};
rux_runtime_param_t& rux_runtime_param_t::operator =( void* value )
{
	_value0 = (::rux::uint32)value;
	_type = XEnum_ReturnValueType_Default;
	return *this;
};
rux_runtime_param_t& rux_runtime_param_t::operator =( double value )
{
	_value0 = ( (::rux::uint32*)&value )[ 0 ];
	_value1 = ( (::rux::uint32*)&value )[ 1 ];
	_type = XEnum_ReturnValueType_Double;
	return *this;
};
rux_runtime_param_t& rux_runtime_param_t::operator =( float value )
{
	_value0 = ( (::rux::uint32*)&value )[ 0 ];
	_type = XEnum_ReturnValueType_Float;
	return *this;
};
rux_runtime_param_t& rux_runtime_param_t::operator =( ::rux::int64 value )
{		
	_value0 = ( (::rux::uint32*)&value )[ 0 ];
	_value1 = ( (::rux::uint32*)&value )[ 1 ];
	_type = XEnum_ReturnValueType_64bits;
	return *this;
};
rux_runtime_param_t& rux_runtime_param_t::operator =( ::rux::uint64 value )
{
	_value0 = ( (::rux::uint32*)&value )[ 0 ];
	_value1 = ( (::rux::uint32*)&value )[ 1 ];
	_type = XEnum_ReturnValueType_64bits;
	return *this;
};
rux_runtime_param_t& rux_runtime_param_t::operator =( ::rux::int32 value )
{
	_value0 = value;
	_type = XEnum_ReturnValueType_Default;
	return *this;
};
rux_runtime_param_t& rux_runtime_param_t::operator =( ::rux::uint32 value )
{
	_value0 = value;
	_type = XEnum_ReturnValueType_Default;
	return *this;
};
#else
rux_runtime_param_t::rux_runtime_param_t()
{
	_value0 = 0;
	_just_to_align = 0;
	_type = XEnum_ReturnValueType_Default;
};
rux_runtime_param_t& rux_runtime_param_t::operator =( const char* value )
{	
	::memcpy( &_value0 , &value , sizeof( void* ) );
	_type = XEnum_ReturnValueType_Default;
	return *this;
};
rux_runtime_param_t& rux_runtime_param_t::operator =( void* value )
{
	::memcpy( &_value0 , &value , sizeof( void* ) );
	_type = XEnum_ReturnValueType_Default;
	return *this;
};
rux_runtime_param_t& rux_runtime_param_t::operator =( double value )
{
	::memcpy( &_value0 , &value , sizeof( void* ) );
	_type = XEnum_ReturnValueType_Double;
	return *this;
};
rux_runtime_param_t& rux_runtime_param_t::operator =( float value )
{
	_value0 = 0;
	::memcpy( &_value0 , &value , sizeof( float ) );
	_type = XEnum_ReturnValueType_Float;
	return *this;
};
rux_runtime_param_t& rux_runtime_param_t::operator =( ::rux::int64 value )
{		
	::memcpy( &_value0 , &value , sizeof( void* ) );
	_type = XEnum_ReturnValueType_64bits;
	return *this;
};
rux_runtime_param_t& rux_runtime_param_t::operator =( ::rux::uint64 value )
{
	::memcpy( &_value0 , &value , sizeof( void* ) );
	_type = XEnum_ReturnValueType_64bits;
	return *this;
};
rux_runtime_param_t& rux_runtime_param_t::operator =( ::rux::int32 value )
{
	_value0 = 0;
	::memcpy( &_value0 , &value , sizeof( ::rux::int32 ) );
	_type = XEnum_ReturnValueType_Default;
	return *this;
};
rux_runtime_param_t& rux_runtime_param_t::operator =( ::rux::uint32 value )
{
	_value0 = 0;
	::memcpy( &_value0 , &value , sizeof( ::rux::uint32 ) );
	_type = XEnum_ReturnValueType_Default;
	return *this;
};
#endif
#define rux_gost_hash_init 0xE819F743DE590AB0
dll_internal ::rux::uint8 rux_tz[ 8 ][ 16 ] = {
	0x4, 0x2, 0xF, 0x5, 0x9, 0x1, 0x0, 0x8, 0xE, 0x3, 0xB, 0xC, 0xD, 0x7, 0xA, 0x6,
	0xC, 0x9, 0xF, 0xE, 0x8, 0x1, 0x3, 0xA, 0x2, 0x7, 0x4, 0xD, 0x6, 0x0, 0xB, 0x5,
	0xD, 0xF, 0xE, 0xC, 0x7, 0x3, 0x9, 0xA, 0x1, 0x5, 0x2, 0x4, 0x6, 0x8, 0x0, 0xB,
	0xE, 0x9, 0xB, 0x2, 0x5, 0xF, 0x7, 0x1, 0x0, 0xD, 0xC, 0x6, 0xA, 0x4, 0x3, 0x8,
	0x3, 0xE, 0x5, 0x9, 0x6, 0x8, 0x0, 0xD, 0xA, 0xB, 0x7, 0xC, 0x2, 0x1, 0xF, 0x4,
	0x8, 0xF, 0x6, 0xB, 0x1, 0x9, 0xC, 0x5, 0xD, 0x3, 0x7, 0xA, 0x0, 0xE, 0x2, 0x4,
	0xA, 0xB, 0xC, 0x0, 0x3, 0x6, 0x7, 0x5, 0x4, 0x8, 0xE, 0xF, 0x1, 0x9, 0x2, 0xD,
	0xC, 0x6, 0x5, 0x2, 0xB, 0x0, 0x9, 0xD, 0x3, 0xE, 0x7, 0xA, 0xF, 0x4, 0x1, 0x8
};
dll_internal bool ruxGostBase( ::rux::uint64& data, ::rux::uint8 index )
{
	bool res = true;	
	try
	{
		::rux::uint32 key[] = 
		{
			0xC9C02774, 0x52A2E838, 0x9C04B513, 0x4F155F55,
			0x923A7F1A, 0xE284730F, 0x14AB6F0F, 0xDCF0F7A5
		};
		::rux::int32 i;
		::rux::uint32 n1 = ( (::rux::uint32*)&data )[ 0 ];
		::rux::uint32 n3 = n1;
		n1 += key[ index ];
		::rux::uint32 n2 = ( (::rux::uint32*)&data )[ 1 ];
		::rux::uint32 temp_n1 = n1;
		::rux::uint32 temp_n2 = 0;
		for( i = 7; i >= 0; i-- )
		{
			temp_n2 <<= 4;
			temp_n2 |= rux_tz[ i ][ ( temp_n1 >> 28 ) ];
			temp_n1 <<= 4;
		};
		n1 = ( temp_n2 << 11 ) | ( temp_n2 >> 21 );
		n2 ^= n1;
		( (::rux::uint32*)&data )[ 1 ] = n3;
		( (::rux::uint32*)&data )[ 0 ] = n2;
	}
	catch( ... )
	{
	};
	return res;
};
dll_internal bool ruxGostHashStep( ::rux::uint64 &data )
{
	bool res = true;	
	try
	{
		::rux::uint32 n1;
		::rux::uint32 n2;
		char i;
		for( i = 0; i < 8; i++ )
		{
			ruxGostBase( data, i );
		};
		for( i = 0; i < 8; i++ )
		{
			ruxGostBase( data, i );
		};
		for( i = 0; i < 8; i++ )
		{
			ruxGostBase( data, i );
		};
		for( i = 7; i >= 0; i-- )
		{
			ruxGostBase( data, i );
		};
		n1 = ( (::rux::uint32*)&data )[ 0 ];
		n2 = ( (::rux::uint32*)&data )[ 1 ];
		( (::rux::uint32*)&data )[ 1 ] = n1;
		( (::rux::uint32*)&data )[ 0 ] = n2;
	}
	catch( ... )
	{
	};		
	return res;
};
dll_internal ::rux::uint64 rux_hash( unsigned char* data_ptr, ::rux::int32 data_size )
{
	::rux::uint64 hash = rux_gost_hash_init;
	::rux::int32 i = 0;
	::rux::int32 nsize8 = data_size >> 3;
	::rux::int32 nsizeOst = data_size % 8;	
	try
	{
		::rux::uint64 data = 0;
		ruxGostHashStep( hash );
		for( i=0;i<nsize8;i++ )
		{
			data = ( (::rux::uint64*)data_ptr )[ 0 ];
			data ^= hash;
			ruxGostHashStep( data );
			hash = data;
			data_ptr += sizeof( ::rux::uint64 );
		};
		if( nsizeOst )
		{
			data = 0;
			::memcpy( &data , data_ptr , nsizeOst );
			data ^= hash;
			ruxGostHashStep( data );
			hash = data;
		};
	}
	catch( ... )
	{
	}	
	return hash;
};
void rux_start_process_with_redirect( const char* executable , char** arguments , size_t arguments_size , char* result , size_t result_size , size_t& readen_size )
{
	readen_size = 0;
	if( executable )
	{		
		::rux::int32 index = rux_index_of( executable , '\\' );
		if( index == -1 )
			index = rux_index_of( executable , '/' );
		if( index == -1 || rux_is_exists_file( executable ) == 1 )
		{
#ifdef __UNIX__
			::rux::int32 pipes[ 2 ] = {0};
			if( pipe( pipes ) == 0 )
			{
				::rux::pid_t child_pid = fork();
				if( child_pid < 0 )
					::rux::XConsole::WriteToStdErr( "\fork failed\n" );
				else if( child_pid == 0 )
				{				
					close( pipes[ 0 ] );				
					dup2( pipes[ 1 ] , STDOUT_FILENO );				
					dup2( pipes[ 1 ] , STDERR_FILENO );
					close( pipes[ 1 ] );
					if( arguments_size == 0 )
						execl( executable , executable , (char*)0 );
					else if( arguments_size == 1 )
						execl( executable , executable , arguments[ 0 ] , (char*)0 );
					else if( arguments_size == 2 )
						execl( executable , executable , arguments[ 0 ] , arguments[ 1 ] , (char*)0 );
					else if( arguments_size == 3 )
						execl( executable , executable , arguments[ 0 ] , arguments[ 1 ] , arguments[ 2 ] , (char*)0 );
					else if( arguments_size == 4 )
						execl( executable , executable , arguments[ 0 ] , arguments[ 1 ] , arguments[ 2 ] , arguments[ 3 ] , (char*)0 );
					else if( arguments_size == 5 )
						execl( executable , executable , arguments[ 0 ] , arguments[ 1 ] , arguments[ 2 ] , arguments[ 3 ] , arguments[ 4 ] , (char*)0 );
					else if( arguments_size == 6 )
						execl( executable , executable , arguments[ 0 ] , arguments[ 1 ] , arguments[ 2 ] , arguments[ 3 ] , arguments[ 4 ] , arguments[ 5 ] , (char*)0 );
					else if( arguments_size == 7 )
						execl( executable , executable , arguments[ 0 ] , arguments[ 1 ] , arguments[ 2 ] , arguments[ 3 ] , arguments[ 4 ] , arguments[ 5 ] , arguments[ 6 ] , (char*)0 );
					else if( arguments_size == 8 )
						execl( executable , executable , arguments[ 0 ] , arguments[ 1 ] , arguments[ 2 ] , arguments[ 3 ] , arguments[ 4 ] , arguments[ 5 ] , arguments[ 6 ] , arguments[ 7 ] , (char*)0 );
					else if( arguments_size == 9 )
						execl( executable , executable , arguments[ 0 ] , arguments[ 1 ] , arguments[ 2 ] , arguments[ 3 ] , arguments[ 4 ] , arguments[ 5 ] , arguments[ 6 ] , arguments[ 7 ] , arguments[ 8 ] , (char*)0 );
					else if( arguments_size == 10 )
						execl( executable , executable , arguments[ 0 ] , arguments[ 1 ] , arguments[ 2 ] , arguments[ 3 ] , arguments[ 4 ] , arguments[ 5 ] , arguments[ 6 ] , arguments[ 7 ] , arguments[ 8 ] , arguments[ 9 ] , (char*)0 );
					else
					{
						char** zero_arguments = alloc_array_macros( char* , arguments_size + 2 );
						zero_arguments[ 0 ] = const_cast< char* >( executable );
						for( size_t index0 = 0 ; index0 < arguments_size ; index0++ )
							zero_arguments[ index0 + 1 ] = arguments[ index0 ];
						zero_arguments[ arguments_size + 1 ] = 0;
						execv( executable , zero_arguments );
						::rux::engine::free_mem( zero_arguments );
					}
					_exit( 1 );
				}
				else if( child_pid != 0 )
				{
					close( pipes[ 1 ] );
					::rux::io::file_descriptor_waiter fd_waiter;
					fd_waiter.add( pipes[ 0 ] );
					if( fd_waiter.wait( 5000 , 1 , 0 ) )
					{
						if( fd_waiter.get_read_fd() != -1 )
						{
							::rux::int32 read_size = 0;
							do
							{
								readen_size += read_size;
								result_size -= read_size;
								if( result_size == 0 )
									break;
								read_size = read( pipes[ 0 ] , &result[ readen_size ] , result_size ); 
							}
							while( read_size != 0 && read_size != -1 );
						}
					}
					close( pipes[ 0 ] );
					waitpid( child_pid , NULL , 0 );
				}
			}	
#endif
		}
	}
};
rux::uint8 rux_get_hostname( char* hostname , size_t hostname_size )
{
	::rux::uint8 success = 0;
	if( hostname && hostname_size > 0 )
	{
#ifdef __WINDOWS__
		::rux::uint16 version_requested = MAKEWORD( 2 , 2 );
		WSADATA wsa_data;			
		::rux::engine::_globals->_ws2_32_module.WSAStartup( version_requested , &wsa_data );
#endif
		if( ::rux::network::gethostname( hostname , hostname_size ) == 0 )
			success = 1;	
	}
	return success;
};
dll_internal rux_simple_t _rux_native_on_any_exit = NULL;
#ifdef __WINDOWS__
LONG __stdcall rux_native_exception_filter( _EXCEPTION_POINTERS * )
{	
	if( _rux_native_on_any_exit )
		_rux_native_on_any_exit();	
	_rux_native_on_any_exit = NULL;
	TerminateProcess(GetCurrentProcess(), 1112);
	return EXCEPTION_EXECUTE_HANDLER;
};
void rux_native_ctrl_c_signal( ::rux::int32 )
{
	if( _rux_native_on_any_exit )
		_rux_native_on_any_exit();			
	_rux_native_on_any_exit = NULL;
	TerminateProcess(GetCurrentProcess(), 0);
};
#endif
#ifdef __UNIX__
void rux_native_posix_death_signal2( ::rux::int32 signum , siginfo_t* siginfo , void* context )
{	
	if( _rux_native_on_any_exit )
		_rux_native_on_any_exit();	
	_rux_native_on_any_exit = NULL;
	abort();
	kill( getpid() , SIGKILL );
};
#endif
void rux_native_set_on_any_exit( rux_simple_t on_any_exit )
{
	atexit( on_any_exit );
	_rux_native_on_any_exit = on_any_exit;
#ifdef __WINDOWS__
	SetUnhandledExceptionFilter( rux_native_exception_filter );
	signal( SIGTERM , rux_native_ctrl_c_signal );		
	signal( SIGINT , rux_native_ctrl_c_signal );
	signal( SIGBREAK , rux_native_ctrl_c_signal );
#endif
#ifdef __UNIX__
	struct sigaction act;
	memset( &act , 0 , sizeof( act ) );	
	act.sa_sigaction = rux_native_posix_death_signal2;
	act.sa_flags = SA_ONSTACK | SA_RESTART | SA_SIGINFO;
	sigset_t set; 
	sigemptyset( &set );
	sigaddset( &set , SIGSEGV );
	sigaddset( &set , SIGILL );
	sigaddset( &set , SIGFPE );
	sigaddset( &set , SIGTERM );
	sigaddset( &set , SIGABRT );	
	sigaddset( &set , SIGBUS );
	sigaddset( &set , SIGHUP );
	sigaddset( &set , SIGPIPE );
	sigaddset( &set , SIGQUIT );
	sigaddset( &set , SIGTSTP );
	sigaddset( &set , SIGTTIN );
	sigaddset( &set , SIGTTOU );
	sigaddset( &set , SIGUSR1 );
	sigaddset( &set , SIGUSR2 );
	sigaddset( &set , SIGPOLL );
	sigaddset( &set , SIGPROF );
	sigaddset( &set , SIGSYS );
	sigaddset( &set , SIGTRAP );
	sigaddset( &set , SIGVTALRM );
	sigaddset( &set , SIGXCPU );
	sigaddset( &set , SIGXFSZ );
	sigaddset( &set , SIGINT );
	act.sa_mask = set;
	sigaction( SIGSEGV , &act , 0 );
	sigaction( SIGILL , &act , 0 );
	sigaction( SIGFPE , &act , 0 );	
	sigaction( SIGABRT , &act , 0 );	
	sigaction( SIGBUS , &act , 0 );
	sigaction( SIGHUP , &act , 0 );
	signal( SIGPIPE , SIG_IGN );
	sigaction( SIGTSTP , &act , 0 );
	sigaction( SIGTTIN , &act , 0 );
	sigaction( SIGTTOU , &act , 0 );
	sigaction( SIGUSR1 , &act , 0 );
	sigaction( SIGUSR2 , &act , 0 );	
	sigaction( SIGPROF , &act , 0 );
	sigaction( SIGSYS , &act , 0 );	
	sigaction( SIGVTALRM , &act , 0 );
	sigaction( SIGXCPU , &act , 0 );
	sigaction( SIGXFSZ , &act , 0 );	
	sigaction( SIGINT , &act , 0 );
	sigaction( SIGTERM , &act , 0 );
	sigaction( SIGQUIT , &act , 0 );
#endif
};
namespace rux
{
	namespace log
	{
		dll_internal const char* _log_levels[] = 
		{
			"ERROR" , "WARN" , "DEBUG" , "TRACE" , "VERBOSE"
		};
	};
	dll_internal int safe_sprintf( char * str , size_t str_size , const char* format , ... )
	{
		int res = -1;
		va_list ap;
		va_start( ap , format );
#ifdef __WINDOWS__ 
		res = ::rux::_vscprintf( format , ap ) + 1;
		if( str_size >= res )
			res = vsprintf( str , format , ap );
		else
		{
			char* temp_ptr = alloc_array_macros( char , res );
			vsnprintf( temp_ptr , res , format , ap );
			::memcpy( str , temp_ptr , str_size - 1 );
			::rux::engine::free_mem( temp_ptr );
			res = str_size - 1;
			str[ res ] = 0;			
		}
#else
		res = vsnprintf( str , str_size , format , ap );
#endif
		va_end( ap );
		return res;
	};
	dll_internal int safe_sprintf( char * str , size_t str_size , const char* format )
	{
		return ::rux::safe_sprintf( str , str_size , "%s" , format );
	};
	dll_internal int _scprintf( const char* format , ... )
	{
		va_list ap;
		va_start( ap , format );
		int retval = ::rux::_vscprintf( format , ap );
		va_end( ap );
		return retval;
	};
	dll_internal int _vscprintf( const char* format , va_list pargs )
	{
#ifdef __WINDOWS__
		return ::_vscprintf( format , pargs );
#else
		va_list argcopy;
		va_copy( argcopy , pargs ); 
		int retval = ::vsnprintf( NULL , 0 , format , argcopy ); 
		va_end( argcopy );
		return retval;
#endif
	};
	dll_internal struct tm* gmtime_r( const time_t* timep , struct tm* result )
	{
#ifdef __WINDOWS__
		if( gmtime_s( result , timep ) == 0 )
			return result;
		else
			return 0;
#else
		return ::gmtime_r( timep , result );
#endif
	};
	dll_internal char* arguments_string_to_array( const char* arguments_string , char** arguments_array , size_t& arguments_array_count )
	{
		arguments_array_count = 0;
		if( arguments_string && ( arguments_array_count = strlen( arguments_string ) ) > 0 )
		{
			char* copy = alloc_array_macros( char , arguments_array_count + 1 );
			::rux::engine::memcpy( copy , 0 , arguments_string , arguments_array_count + 1 );
			char* begin = copy , * ptr = NULL , * end = &copy[ arguments_array_count ];
			arguments_array_count = 0;			
			::rux::uint8 quote = 0;
			do
			{
				if( quote == 1 )
					ptr = strstr( begin , "\"" );
				else
					ptr = strpbrk( begin , " \"" );
				if( ptr )
				{
					if( *ptr == '"' )
					{
						quote = quote == 1 ? 0 : 1;
					}
					*ptr = 0;
					if( ptr - begin > 1 )
					{
						arguments_array[ arguments_array_count ] = begin;
						arguments_array_count++;
					}
					ptr++;
					begin = ptr;					
				}
			}
			while( ptr && *ptr != 0 );
			if( end - begin > 0 )
			{
				arguments_array[ arguments_array_count ] = begin;
				arguments_array_count++;
			}			
			return copy;
		}
		return NULL;
	};
	dll_internal XGCRef* uint64_to_gcref( ::rux::uint64 value )
	{
		return &::rux::XUInt64( value )++;
	};
	dll_internal Object* uint64_to_object( ::rux::uint64 value )
	{
		return &XObject( ::rux::XUInt64( value ) )++;
	};
	dll_internal ::rux::byte gcref_to_byte( const XGCRef* object )
	{
		if( rux_is_object_ptr( object , ::rux::XBoolean ) )
		{
			::rux::Boolean* bool_val = (::rux::Boolean*)object->get_ObjectAddress();
			return bool_val->_value;
		}
		return 0;
	};
	dll_internal float gcref_to_float( const XGCRef* object )
	{
		if( rux_is_object_ptr( object , ::rux::XFloat ) )
		{
			::rux::Float* uint64_val = (::rux::Float*)object->get_ObjectAddress();
			return uint64_val->_value;
		}
		return 0;
	};
	dll_internal ::rux::int32 gcref_to_int32( const XGCRef* object )
	{
		if( rux_is_object_ptr( object , ::rux::XInt32 ) )
		{
			::rux::Int32* uint64_val = (::rux::Int32*)object->get_ObjectAddress();
			return uint64_val->_value;
		}
		return 0;
	};
	dll_internal ::rux::uint64 gcref_to_uint64( const XGCRef* object )
	{
		if( rux_is_object_ptr( object , ::rux::XUInt64 ) )
		{
			::rux::UInt64* uint64_val = (::rux::UInt64*)object->get_ObjectAddress();
			return uint64_val->_value;
		}
		return 0;
	};
	dll_internal ::rux::uint64 object_to_uint64( Object* object )
	{
		if( rux_is_object_ptr( object , ::rux::XUInt64 ) )
		{
			::rux::UInt64* uint64_val = (::rux::UInt64*)object->_gc_ref_obj->get_ObjectAddress();
			return uint64_val->_value;
		}
		return 0;
	};
	dll_internal bool is_boolean_and_true( XGCRef* object )
	{
		if( rux_is_object_ptr( object , ::rux::XBoolean ) )
		{
			::rux::Boolean* bool_val = (::rux::Boolean*)object->get_ObjectAddress();
			return bool_val->_value == 1;
		}
		return false;
	};
	dll_internal ::rux::uint32 get_numeric_rux_type_index( void )
	{
		return ::rux::XNumeric::get_static_RuxTypeIndex();
	};
	dll_internal ::rux::uint32 get_float_rux_type_index( void )
	{
		return ::rux::XFloat::get_static_RuxTypeIndex();
	};
	dll_internal ::rux::uint32 get_data_object_rux_type_index( void )
	{
		return ::rux::data::XDataObject::get_static_RuxTypeIndex();
	};
	dll_internal ::rux::uint32 get_array_object_rux_type_index( void )
	{
		return ::rux::XArray< XObject >::get_static_RuxTypeIndex();
	};
	dll_internal ::rux::uint32 get_double_rux_type_index( void )
	{
		return ::rux::XDouble::get_static_RuxTypeIndex();
	};
	dll_internal ::rux::uint32 get_group_rux_type_index( void )
	{
		return ::rux::gui::controls::XGroup::get_static_RuxTypeIndex();
	};
	dll_internal ::rux::uint32 get_int32_rux_type_index( void )
	{
		return ::rux::XInt32::get_static_RuxTypeIndex();
	};
	dll_internal ::rux::uint32 get_uint32_rux_type_index( void )
	{
		return ::rux::XUInt32::get_static_RuxTypeIndex();
	};
	dll_internal ::rux::uint32 get_int64_rux_type_index( void )
	{
		return ::rux::XInt64::get_static_RuxTypeIndex();
	};
	dll_internal ::rux::uint32 get_uint64_rux_type_index( void )
	{
		return ::rux::XUInt64::get_static_RuxTypeIndex();
	};
	dll_internal ::rux::int32 get_NumberPowerOf2( ::rux::int32 number )
	{
		::rux::int32 power_of_2 = 2;
		while( number > power_of_2 )
			power_of_2 *= 2;
		return power_of_2;
	};	
	void string::uint64_to_hex_string( ::rux::uint64 number1 , char string[] )
	{
		string[ 0 ] = '0';
		string[ 1 ] = 'x';
		size_t string_index = 2;
		size_t byte_index = 7;
		::rux::uint8 byte = 0 , value = 0;
		for( ; ; )
		{
			byte = rux_get_byte( number1 , byte_index );
			value = byte / 16;
			if( value < 10 )
				string[ string_index ] = (char)( value + 48 );
			else
				string[ string_index ] = (char)( value + 87 );
			string_index++;
			value = byte % 16;
			if( value < 10 )
				string[ string_index ] = (char)( value + 48 );
			else
				string[ string_index ] = (char)( value + 87 );
			string_index++;
			if( byte_index == 0 )
			{
				string[ string_index ] = 0;
				break;
			}
			byte_index--;		
		}
	};
	::rux::uint64 string::to_uint64( const wchar_t* string , size_t string_size )
	{
		return string_to_number< ::rux::uint64 , wchar_t >( string , string_size , 1 );
	};
	::rux::int64 string::to_int64( const wchar_t* string , size_t string_size )
	{
		return string_to_number< ::rux::int64 , wchar_t >( string , string_size , 0 );
	};
	::rux::uint64 string::to_uint64( const wchar_t* string )
	{
		return string_to_number< ::rux::uint64 , wchar_t >( string , SIZE_MAX , 1 );
	};
	::rux::int64 string::to_int64( const wchar_t* string )
	{
		return string_to_number< ::rux::int64 , wchar_t >( string , SIZE_MAX , 0 );
	};
	::rux::uint64 string::to_uint64( const char* string , size_t string_size )
	{
		return string_to_number< ::rux::uint64 , char >( string , string_size , 1 );
	};
	::rux::int64 string::to_int64(  const char* string , size_t string_size )
	{
		return string_to_number< ::rux::int64 , char >( string , string_size , 0 );
	};
	::rux::uint32 string::to_uint32( const char* string )
	{
		return string_to_number< ::rux::uint32 , char >( string , SIZE_MAX , 1 );
	};
	::rux::uint64 string::to_uint64( const char* string )
	{
		return string_to_number< ::rux::uint64 , char >( string , SIZE_MAX , 1 );
	};
	size_t string::to_size_t( char string[] , size_t string_size )
	{	
		return string_to_number< size_t , char >( string , string_size , 1 );
	};
	::rux::uint8 string::is_utf8( const char* string , size_t length )
	{			
		if( length == 0 )
			return 1;
		if( string == 0 )
			return 0;
		::rux::uint8* ptr = (::rux::uint8*)string;
		while( length-- ) 
		{
			if( *ptr <= 0x7f )
			{
			}
			else if( (*ptr >> 5 ) == 0x6 )
			{
				ptr++;
				if( ( length-- ) == 0 )
					return 0;
				if( !( ( *ptr >> 6 ) == 0x2 ) )
					return 0;
			}
			/* three byte */
			else if( ( *ptr >> 4 ) == 0x0e ) 
			{
				ptr++;
				if( ( length-- ) == 0 )
					return 0;
				if( !( ( *ptr >> 6 ) == 0x2 ) )
					return 0;
				ptr++;
				if( ( length-- ) == 0 )
					return 0;
				if( !( ( *ptr >> 6 ) == 0x2 ) )
					return 0;
			}
			/* four byte */        
			else if( ( *ptr >> 3 ) == 0x1e )
			{
				ptr++;
				if( ( length-- ) == 0 )
					return 0;
				if( !( ( *ptr >> 6 ) == 0x2 ) )
					return 0;
				ptr++;
				if( ( length-- ) == 0 )
					return 0;
				if( !( ( *ptr >> 6 ) == 0x2 ) )
					return 0;
				ptr++;
				if( ( length-- ) == 0 )
					return 0;
				if( !( ( *ptr >> 6 ) == 0x2 ) )
					return 0;
			}
			else
				return 0;        
			ptr++;
		}    
		return 1;
	};
	::rux::int64 string::index_of( const char* string , const char* find )
	{
		const char* ptr = strstr( string , find );
		if( ptr )
			return ptr - string;
		else
			return -1;
	};
	::rux::uint8 string::hex_string_to_uint64( const char* hex , ::rux::uint64& value , size_t length )
	{
		::rux::uint8 result = 1;
		value = 0;
		if( length == 0 )
			length = hex ? strlen( hex ) : 0;
		if( length > 0 )
		{
			::rux::uint8 even = 0;
			if( length % 2 == 0 )
				even = 1;
			::rux::uint32 index0 = 0;
			char letter = 0;
			if( hex[ 0 ] == '0'
				&& ( length > 1 && hex[ 1 ] == 'x' ) )
				index0 = 2;		
			if( even == 0 && index0 < length )
			{
				letter = hex[ index0 ];
				switch( letter )
				{
				case '1':
					{
						value = 1;
						break;
					}
				case '2':
					{
						value = 2;
						break;
					}
				case '3':
					{
						value = 3;
						break;
					}
				case '4':
					{
						value = 4;
						break;
					}
				case '5':
					{
						value = 5;
						break;
					}
				case '6':
					{
						value = 6;
						break;
					}
				case '7':
					{
						value = 7;
						break;
					}
				case '8':
					{
						value = 8;
						break;
					}
				case '9':
					{
						value = 9;
						break;
					}
				case '0':
					{
						value = 0;
						break;
					}
				case 'a':
				case 'A':
					{
						value = 10;
						break;
					}
				case 'b':
				case 'B':
					{
						value = 11;
						break;
					}
				case 'c':
				case 'C':
					{
						value = 12;
						break;
					}
				case 'd':
				case 'D':
					{
						value = 13;
						break;
					}
				case 'e':
				case 'E':
					{
						value = 14;
						break;
					}
				case 'f':
				case 'F':
					{
						value = 15;
						break;
					}
				default:
					{
						result = 0;
						break;
					}
				}			
				index0++;
			}
			if( result == 1 )
			{
				::rux::uint8 byte = 0;
				while( index0 < length )
				{
					value <<= 8;			
					letter = hex[ index0 ];
					switch( letter )
					{
					case '1':
						{
							byte = 1 * 16;
							break;
						}
					case '2':
						{
							byte = 2 * 16;
							break;
						}
					case '3':
						{
							byte = 3 * 16;
							break;
						}
					case '4':
						{
							byte = 4 * 16;
							break;
						}
					case '5':
						{
							byte = 5 * 16;
							break;
						}
					case '6':
						{
							byte = 6 * 16;
							break;
						}
					case '7':
						{
							byte = 7 * 16;
							break;
						}
					case '8':
						{
							byte = 8 * 16;
							break;
						}
					case '9':
						{
							byte = 9 * 16;
							break;
						}
					case '0':
						{
							byte = 0;
							break;
						}
					case 'a':
					case 'A':
						{
							byte = 10 * 16;
							break;
						}
					case 'b':
					case 'B':
						{
							byte = 11 * 16;
							break;
						}
					case 'c':
					case 'C':
						{
							byte = 12 * 16;
							break;
						}
					case 'd':
					case 'D':
						{
							byte = 13 * 16;
							break;
						}
					case 'e':
					case 'E':
						{
							byte = 14 * 16;
							break;
						}
					case 'f':
					case 'F':
						{
							byte = 15 * 16;
							break;
						}
					default:
						{
							result = 0;
							break;
						}
					}
					if( result == 0 )
						break;
					index0++;
					letter = hex[ index0 ];
					switch( letter )
					{
					case '1':
						{
							byte += 1;
							break;
						}
					case '2':
						{
							byte += 2;
							break;
						}
					case '3':
						{
							byte += 3;
							break;
						}
					case '4':
						{
							byte += 4;
							break;
						}
					case '5':
						{
							byte += 5;
							break;
						}
					case '6':
						{
							byte += 6;
							break;
						}
					case '7':
						{
							byte += 7;
							break;
						}
					case '8':
						{
							byte += 8;
							break;
						}
					case '9':
						{
							byte += 9;
							break;
						}
					case '0':
						{
							byte += 0;
							break;
						}
					case 'a':
					case 'A':
						{
							byte += 10;
							break;
						}
					case 'b':
					case 'B':
						{
							byte += 11;
							break;
						}
					case 'c':
					case 'C':
						{
							byte += 12;
							break;
						}
					case 'd':
					case 'D':
						{
							byte += 13;
							break;
						}
					case 'e':
					case 'E':
						{
							byte += 14;
							break;
						}
					case 'f':
					case 'F':
						{
							byte += 15;
							break;
						}
					default:
						{
							result = 0;
							break;
						}
					}
					if( result == 0 )
						break;
					value |= byte;
					index0++;
				}
			}
		}
		else
			result = 0;
		return result;
	};
	::rux::int64 string::last_index_of( const char* string , const char* find )
	{
		char* ptr = const_cast< char* >( string ) , * last_ptr = NULL;
		for( ; ; )
		{
			ptr = strstr( ptr , find );
			if( ptr )
			{
				last_ptr = ptr;
				ptr++;
				if( *ptr == 0 )
					break;
			}
			else
				break;
		}
		if( last_ptr )
			return last_ptr - string;
		else
			return -1;
	};
	::rux::int64 string::to_int64( const char* string )
	{
		return string_to_number< ::rux::int64 , char >( string , SIZE_MAX , 0 );
	};
	void string::uint32_to_string( ::rux::uint32 number1 , char string[] )
	{
		if( number1 == 0 )
		{
			string[ 0 ] = '0';
			string[ 1 ] = 0;
		}
		else
		{
			::rux::uint32 offset = 0 , number = 0;		
			::rux::uint32 max = 1000000000;
			for( ; ; )
			{
				if( number1 > max - 1 )
				{
					number = number1 / max;
					number1 %= max;
					string[ offset ] = (char)( 48 + number );
					offset++;
				}
				else if( offset > 0 )
				{
					string[ offset ] = '0';
					offset++;
				}
				if( max == 1 )
				{
					offset--;
					break;
				}
				max /= 10;
			}
			string[ offset + 1 ] = 0;
		}
	};
	void string::uint64_to_string( ::rux::uint64 number1 , char string[] )
	{	
		if( number1 == 0 )
		{
			string[ 0 ] = '0';
			string[ 1 ] = 0;
		}
		else
		{
			::rux::uint64 offset = 0 , number = 0;		
			::rux::uint64 max = 10000000000000000000ULL;
			for( ; ; )
			{
				if( number1 > max - 1 )
				{
					number = number1 / max;
					number1 %= max;
					string[ offset ] = (char)( 48 + number );
					offset++;
				}
				else if( offset > 0 )
				{
					string[ offset ] = '0';
					offset++;
				}
				if( max == 1 )
				{
					offset--;
					break;
				}
				max /= 10;
			}
			string[ offset + 1 ] = 0;
		}
	};
	void string::uint32_to_hex_string( ::rux::uint32 number1 , char string[] )
	{
		string[ 0 ] = '0';
		string[ 1 ] = 'x';
		size_t string_index = 2;
		size_t byte_index = 7;
		::rux::uint8 byte = 0 , value = 0;
		for( ; ; )
		{
			byte = rux_get_byte( number1 , byte_index );
			value = byte / 16;
			if( value < 10 )
				string[ string_index ] = (char)( value + 48 );
			else
				string[ string_index ] = (char)( value + 87 );
			string_index++;
			value = byte % 16;
			if( value < 10 )
				string[ string_index ] = (char)( value + 48 );
			else
				string[ string_index ] = (char)( value + 87 );
			string_index++;
			if( byte_index == 0 )
			{
				string[ string_index ] = 0;
				break;
			}
			byte_index--;		
		}
	};	
	string::string( ::rux::string* string )
	{
		_length = string ? string->_length : 0;
		_str = ::rux::engine::realloc_array< char >( NULL , _length + 1 );
		if( _length > 0 )
			::memcpy( _str , string->_str , _length );
		_str[ _length ] = 0;
		_need_free = ::rux::string::DeleteStr;
		if( string->_need_free & ::rux::string::DeleteObject )
		{
			string->~string();
			::rux::engine::free_mem( string );
		}
	};
	string::string( const char* str , ::rux::uint8 by_ref , ::rux::uint8 need_free )
	{
		_length = str ? strlen( str ) : 0;
		if( by_ref )
		{
			_str = const_cast< char* >( str );
			_need_free = need_free;
		}
		else
		{
			_str = ::rux::engine::realloc_array< char >( NULL , _length + 1 );
			if( _length > 0 )
				::memcpy( _str , str , _length );
			_str[ _length ] = 0;
			_need_free = ::rux::string::DeleteStr;
		}
	};
	string::~string( void )
	{
		if( _str && ( _need_free & ::rux::string::DeleteStr ) )
			::rux::engine::free_mem( _str );
	};
	void string::clear( void )
	{
		_length = 0;
		_str[ _length ] = 0;
	};
	::rux::string& string::operator +=( const char* str )
	{
		::rux::string string( str , 1 , 0 );
		*this += string;
		return *this;
	};
	::rux::string& string::operator +=( const ::rux::string& string )
	{
		if( string._length > 0 )
		{
			if( _need_free & ::rux::string::DeleteStr )
			{
				_str = ::rux::engine::realloc_array< char >( _str , _length + string._length + 1 );
				::memcpy( &_str[ _length ] , string._str , string._length );
				_length += string._length;
				_str[ _length ] = 0;
				if( _need_free & ::rux::string::DeleteObject )
					_need_free = ::rux::string::DeleteObject | ::rux::string::DeleteStr;
				else
					_need_free = ::rux::string::DeleteStr;
			}
			else
			{
				char* str = _str;
				_str = ::rux::engine::realloc_array< char >( NULL , _length + string._length + 1 );
				::memcpy( _str , str , _length );
				::memcpy( &_str[ _length ] , string._str , string._length );
				_length += string._length;
				_str[ _length ] = 0;
				if( _need_free & ::rux::string::DeleteObject )
					_need_free = ::rux::string::DeleteObject | ::rux::string::DeleteStr;
				else
					_need_free = ::rux::string::DeleteStr;
			}
		}
		return *this;
	};
	::rux::string* operator +( const char* str , const ::rux::string& string )
	{
		void* ptr = alloc_array_macros( char , sizeof( ::rux::string ) );
		::rux::string* res = new (ptr)::rux::string( str , 1 , ::rux::string::DeleteObject );	
		*res += string;
		return res;
	};
	bool operator ==( const ::rux::string& string , const char* str )
	{
		if( string._str && str )
			return strcmp( string._str , str ) == 0;
		else
			return false;
	};
	::rux::int64 string::index_of( const char* find )
	{
		if( _str )
			return ::rux::string::index_of( _str , find );
		else
			return -1;
	};
	::rux::int64 string::last_index_of( const char* find )
	{
		if( _str )
			return ::rux::string::last_index_of( _str , find );
		else
			return -1;
	};
	void string::replace_all( const char* old_string , const char* new_string )
	{
		if( _length > 0 )
		{
			if( ( _need_free & ::rux::string::DeleteStr ) == false )
			{
				char* str = _str;
				_str = ::rux::engine::realloc_array< char >( NULL , _length + 1 );
				::memcpy( _str , str , _length );
				_str[ _length ] = 0;
				if( _need_free & ::rux::string::DeleteObject )
					_need_free = ::rux::string::DeleteObject | ::rux::string::DeleteStr;
				else
					_need_free = ::rux::string::DeleteStr;
			}
			if( _str )
			{
				size_t _str_index = 0;
				size_t old_string_length = strlen( old_string );
				if( old_string_length )
				{
					size_t new_string_length = strlen( new_string );
					if( new_string_length == old_string_length )
					{
						for( ; ; )
						{
							char* ptr = strstr( &_str[ _str_index ] , old_string );
							if( ptr )
							{
								_str_index = ptr - _str;
								::memcpy( &_str[ _str_index ] , new_string , new_string_length );
								_str_index += new_string_length;
							}
							else
								break;
						}				
					}
					else if( new_string_length > old_string_length )
					{
						size_t difference = new_string_length - old_string_length;
						for( ; ; )
						{
							char* ptr = strstr( &_str[ _str_index ] , old_string );
							if( ptr )
							{	
								_str_index = ptr - _str;
								_length += difference;
								_str = ::rux::engine::realloc_array< char >( _str , _length + 1 );
								::memmove( &_str[ _str_index + new_string_length ] , &_str[ _str_index + old_string_length ] , _length - difference - _str_index - old_string_length );
								::memcpy( &_str[ _str_index ] , new_string , new_string_length );
								_str_index += new_string_length;
							}
							else
								break;
						}
					}
					else
					{
						size_t difference = old_string_length - new_string_length;
						for( ; ; )
						{
							char* ptr = strstr( &_str[ _str_index ] , old_string );
							if( ptr )
							{
								_str_index = ptr - _str;
								if( new_string_length > 0 )
									::memcpy( &_str[ _str_index ] , new_string , new_string_length );
								::memmove( &_str[ _str_index + new_string_length ] , &_str[ _str_index + old_string_length ] , _length - _str_index - old_string_length );
								_length -= difference;
								_str_index += new_string_length;
							}
							else
								break;
						}
						_str = ::rux::engine::realloc_array< char >( _str , _length + 1 );
						_str[ _length ] = 0;
					}		
				}
			}
		}
	};
	void string::replace_all( char old_char , char new_char )
	{
		if( _length > 0 )
		{
			if( ( _need_free & ::rux::string::DeleteStr ) == false )
			{
				char* str = _str;
				_str = ::rux::engine::realloc_array< char >( NULL , _length + 1 );
				::memcpy( _str , str , _length );
				_str[ _length ] = 0;
				if( _need_free & ::rux::string::DeleteObject )
					_need_free = ::rux::string::DeleteObject | ::rux::string::DeleteStr;
				else
					_need_free = ::rux::string::DeleteStr;
			}
			if( _str )
				::rux::string::replace_all( _str , old_char , new_char );
		}
	};
	void string::substring( size_t start_index , size_t length )
	{
		if( _length > 0 )
		{
			if( ( _need_free & ::rux::string::DeleteStr ) == false )
			{
				char* str = _str;
				_str = ::rux::engine::realloc_array< char >( NULL , _length + 1 );
				::memcpy( _str , str , _length );
				_str[ _length ] = 0;
				if( _need_free & ::rux::string::DeleteObject )
					_need_free = ::rux::string::DeleteObject | ::rux::string::DeleteStr;
				else
					_need_free = ::rux::string::DeleteStr;
			}
			if( _str )
			{
				::rux::string::substring( _str , _str , start_index , length , _length );
				_length = strlen( _str );
			}
		}
	};
	::rux::uint32 string::to_uint32( void )
	{
		if( _str )
			return ::rux::string::to_uint32( _str );
		return 0;
	};
	void string::substring( size_t start_index )
	{
		if( _length > 0 )
		{
			if( ( _need_free & ::rux::string::DeleteStr ) == false )
			{
				char* str = _str;
				_str = ::rux::engine::realloc_array< char >( NULL , _length + 1 );
				::memcpy( _str , str , _length );
				_str[ _length ] = 0;
				if( _need_free & ::rux::string::DeleteObject )
					_need_free = ::rux::string::DeleteObject | ::rux::string::DeleteStr;
				else
					_need_free = ::rux::string::DeleteStr;
			}
			if( _str )
			{
				::rux::string::substring( _str , _str , start_index , _length , _length );
				_length = strlen( _str );
			}
		}
	};
	bool string::operator ==( const ::rux::string& string )
	{
		if( _length == string._length )
		{
			if( _length == 0 )
				return true;
			else if( string._str && _str )
				return memcmp( string._str , _str , _length ) == 0;
			else
				return false;
		}
		else
			return false;
	};
	size_t string::length( void ) const
	{
		return _length;
	};
	void string::replace_all( char* destination , char old_char , char new_char )
	{
		char olds[] = { old_char , 0 };
		char* ptr = destination;
		for( ; ; )
		{
			if( ptr )
			{
				if( *ptr == 0 )
					break;			
				ptr = strstr( ptr , olds );
			}
			else
				break;
			if( ptr )
			{
				*ptr = new_char;
				ptr++;				
			}
			else
				break;
		}
	};
	void string::substring( char* destination , const char* source , size_t start_index , size_t length , size_t source_length )
	{
		if( length > 0 )
		{
			if( source_length == 0 )
				source_length = strlen( source );
			if( start_index < source_length )
			{				
				if( start_index + length > source_length )
					length = source_length - start_index;
				if( length > 0 )
				{
					if( destination == source )
					{
						::memmove( destination , &source[ start_index ] , length );
						destination[ length ] = 0;
					}
					else
					{
						::memcpy( destination , &source[ start_index ] , length );
						destination[ length ] = 0;
					}
				}
				else
					destination[ 0 ] = 0;
			}
			else
				destination[ 0 ] = 0;
		}
		else
			destination[ 0 ] = 0;
	};
	void string::substring( char* destination , const char* source , size_t start_index , size_t source_length )
	{	
		if( source_length == 0 )
			source_length = strlen( source );
		if( start_index < source_length )
		{
			size_t length = source_length - start_index;
			::rux::string::substring( destination , source , start_index , length , source_length );
		}
		else
			destination[ 0 ] = 0;
	};
	bool bytes::revert( ::rux::uint8* dest , ::rux::uint8* src , size_t data_size )
	{
		if( src && dest && data_size > 0 )
		{
			size_t decrementer = data_size - 1;		
			if( src == dest )
			{
				::rux::uint8 byte = 0x00;
				data_size /= 2;
				for( size_t incrementer = 0 ; incrementer < data_size ; incrementer++ )
				{
					byte = src[ incrementer ];
					src[ incrementer ] = dest[ decrementer ];
					dest[ decrementer ] = byte;
					decrementer--;
				}
			}
			else
			{
				data_size /= 2;
				for( size_t incrementer = 0 ; incrementer < data_size ; incrementer++ )
				{			
					dest[ decrementer ] = src[ incrementer ];
					decrementer--;
				}
			}
			return true;
		}
		else
			return false;
	};
	bool bytes::revert( ::rux::uint8* data , size_t data_size )
	{
		return ::rux::bytes::revert( data , data , data_size );
	};
	bool security::get_current_user_name( char* user_name , size_t user_name_size )
	{
#ifdef __WINDOWS__
		DWORD user_name_dword = user_name_size;
		if( GetUserNameA( user_name , &user_name_dword ) )
			return true;
		else
			return false;
#else
		uid_t uid = geteuid ();
		struct passwd* pw = getpwuid( uid );
		if( pw )
		{
			if( pw->pw_name && pw->pw_name[ 0 ] != 0 )
			{
				::rux::safe_strncpy( user_name , pw->pw_name , user_name_size );
				return true;
			}
			else
				return false;
		}
		else
			return false;
#endif
	};
	bool security::get_current_user_id( ::rux::uint32& uid )
	{
#ifdef __UNIX__
		uid = geteuid();
		return true;
#else
		return false;
#endif
	};
	bool security::get_user_id( const char* username , ::rux::uint32& uid , ::rux::uint32& gid )
	{
		uid = 0 , gid = 0;
		bool res = false;
#ifdef __UNIX__
#ifdef __ANDROID__
		struct passwd* result = getpwnam( username );
		if( result )
		{
			uid = result->pw_uid;
			gid = result->pw_gid;
			res = true;
		}
#else
		struct passwd pwd;
		struct passwd* result = NULL;
		char* buffer = NULL;
		size_t bufsize = 0;
		bufsize = sysconf( _SC_GETPW_R_SIZE_MAX );
		if( bufsize == -1 )
			bufsize = 16384;
		buffer = (char*)alloc_array_macros( char , bufsize );
		::rux::int32 s = getpwnam_r( username , &pwd , buffer , bufsize , &result );
		if( result )
		{
			uid = pwd.pw_uid;
			gid = pwd.pw_gid;
			res = true;
		}
		::rux::engine::free_mem( buffer );
#endif
#endif
		return res;
	};
	namespace io
	{		
		bool directory::get_files( const char* dir , XMallocArray< file* >& files )
		{
			bool res = false;
			if( dir && strlen( dir ) > 0 ) 
			{
				declare_stack_variable( char , local_dir , 2048 );
				strcpy( local_dir , dir );
				::rux::string::replace_all( local_dir , '\\' , '/' );
#ifdef __WINDOWS__				
				if( local_dir[ strlen( local_dir ) - 1 ] == '/' )
					strcat( local_dir , "*" );
				else
					strcat( local_dir , "/*" );
				HANDLE find_handle = NULL;
				WIN32_FIND_DATAA win32_find_data;
				if( ( find_handle = FindFirstFileA( local_dir , &win32_find_data ) ) != INVALID_HANDLE_VALUE )
				{		
					res = true;
					do
					{
						if( !( win32_find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
						{
							declare_stack_variable( char , filename , 2048 );
							strcpy( filename , dir );
							::rux::string::replace_all( filename , '\\' , '/' );
							if( filename[ strlen( filename ) - 1 ] == '/' )
								strcat( filename , win32_find_data.cFileName );
							else
							{
								strcat( filename , "/" );
								strcat( filename , win32_find_data.cFileName );
							}
							files.Add( ::rux::engine::alloc_object< ::rux::io::file >( filename , (const char*)__FILE__ , (::rux::uint32)__LINE__ , (char*)NULL , (::rux::malloc_t)::rux::engine::rux_malloc_dummy ) );
						}
					}
					while( FindNextFileA( find_handle , &win32_find_data ) );
					FindClose( find_handle );
				}
#elif defined( __UNIX__ )
				DIR* dir_ptr = opendir( local_dir );
				if( dir_ptr )
				{
					res = true;
					::rux::int32 name_max = pathconf( local_dir , _PC_NAME_MAX );
					if( name_max == -1 )
						name_max = 255;
					size_t dirent_len = offsetof( struct dirent , d_name ) + name_max + 1;
					dirent* dirent_object_ptr = (dirent*)alloc_array_macros( char* , dirent_len );
					dirent* dirent_result = 0;
					struct stat io_stat;
					while( readdir_r( dir_ptr , dirent_object_ptr , &dirent_result ) == 0 && dirent_result )
					{
						if( strcmp( dirent_object_ptr->d_name , "." ) != 0
							&& strcmp( dirent_object_ptr->d_name , ".." ) != 0 )
						{
							declare_stack_variable( char , filename , 2048 );
							strcpy( filename , dir );
							::rux::string::replace_all( filename , '\\' , '/' );
							if( filename[ strlen( filename ) - 1 ] == '/' )
								strcat( filename , dirent_object_ptr->d_name );
							else
							{
								strcat( filename , "/" );
								strcat( filename , dirent_object_ptr->d_name );
							}
							stat( filename , &io_stat );
#ifdef __LINUX__
							if( dirent_object_ptr->d_type == DT_REG )
#elif defined( __SOLARIS__ )
							if( io_stat.st_mode & S_IFREG )
#endif
								files.Add( ::rux::engine::alloc_object< ::rux::io::file >( filename , (const char*)__FILE__ , (::rux::uint32)__LINE__ , (char*)NULL , (::rux::malloc_t)::rux::engine::rux_malloc_dummy ) );
						}
					}
					closedir( dir_ptr );
					::rux::engine::free_mem( dirent_object_ptr );	
				}
#endif
			}
			return res;
		};
		bool directory::get_all_files( const char* dir , XMallocArray< file* >& files )
		{
			bool res = false;
			if( dir && strlen( dir ) > 0 ) 
			{
				declare_stack_variable( char , local_dir , 2048 );
				strcpy( local_dir , dir );
				::rux::string::replace_all( local_dir , '\\' , '/' );
#ifdef __WINDOWS__				
				if( local_dir[ strlen( local_dir ) - 1 ] == '/' )
					strcat( local_dir , "*" );
				else
					strcat( local_dir , "/*" );
				HANDLE find_handle = NULL;
				WIN32_FIND_DATAA win32_find_data;
				if( ( find_handle = FindFirstFileA( local_dir , &win32_find_data ) ) != INVALID_HANDLE_VALUE )
				{		
					res = true;
					do
					{
						if( !( win32_find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
						{
							declare_stack_variable( char , filename , 2048 );
							strcpy( filename , dir );
							::rux::string::replace_all( filename , '\\' , '/' );
							if( filename[ strlen( filename ) - 1 ] == '/' )
								strcat( filename , win32_find_data.cFileName );
							else
							{
								strcat( filename , "/" );
								strcat( filename , win32_find_data.cFileName );
							}
							files.Add( ::rux::engine::alloc_object< ::rux::io::file >( filename , (const char*)__FILE__ , (::rux::uint32)__LINE__ , (char*)NULL , (::rux::malloc_t)::rux::engine::rux_malloc_dummy ) );
						}
						else
						{
							if( strcmp( win32_find_data.cFileName , "." ) != 0
								&& strcmp( win32_find_data.cFileName , ".." ) != 0 )
							{
								declare_stack_variable( char , filename , 2048 );
								strcpy( filename , dir );
								::rux::string::replace_all( filename , '\\' , '/' );
								if( filename[ strlen( filename ) - 1 ] == '/' )
									strcat( filename , win32_find_data.cFileName );
								else
								{
									strcat( filename , "/" );
									strcat( filename , win32_find_data.cFileName );
								}
								get_all_files( filename , files );
							}
						}
					}
					while( FindNextFileA( find_handle , &win32_find_data ) );
					FindClose( find_handle );
				}
#elif defined( __UNIX__ )
				DIR* dir_ptr = opendir( local_dir );
				if( dir_ptr )
				{
					res = true;
					::rux::int32 name_max = pathconf( local_dir , _PC_NAME_MAX );
					if( name_max == -1 )
						name_max = 255;
					size_t dirent_len = offsetof( struct dirent , d_name ) + name_max + 1;
					dirent* dirent_object_ptr = (dirent*)alloc_array_macros( char* , dirent_len );
					dirent* dirent_result = 0;
					struct stat io_stat;
					while( readdir_r( dir_ptr , dirent_object_ptr , &dirent_result ) == 0 && dirent_result )
					{
						if( strcmp( dirent_object_ptr->d_name , "." ) != 0
							&& strcmp( dirent_object_ptr->d_name , ".." ) != 0 )
						{
							declare_stack_variable( char , filename , 2048 );
							strcpy( filename , dir );
							::rux::string::replace_all( filename , '\\' , '/' );
							if( filename[ strlen( filename ) - 1 ] == '/' )
								strcat( filename , dirent_object_ptr->d_name );
							else
							{
								strcat( filename , "/" );
								strcat( filename , dirent_object_ptr->d_name );
							}
							stat( filename , &io_stat );
#ifdef __LINUX__
							if( dirent_object_ptr->d_type == DT_REG )
#elif defined( __SOLARIS__ )
							if( io_stat.st_mode & S_IFREG )
#endif
								files.Add( ::rux::engine::alloc_object< ::rux::io::file >( filename , (const char*)__FILE__ , (::rux::uint32)__LINE__ , (char*)NULL , (::rux::malloc_t)::rux::engine::rux_malloc_dummy ) );
#ifdef __LINUX__
							else if( dirent_object_ptr->d_type == DT_DIR )
#elif defined( __SOLARIS__ )
							else if( io_stat.st_mode & S_IFDIR )
#endif
								::rux::io::directory::get_all_files( filename , files );
						}
					}
					closedir( dir_ptr );
					::rux::engine::free_mem( dirent_object_ptr );	
				}
#endif
			}
			return res;
		};
		bool directory::get_directories( const char* dir , XMallocArray< directory* >& directories )
		{
			bool res = false;
			if( dir && strlen( dir ) > 0 ) 
			{
				declare_stack_variable( char , local_dir , 2048 );
				strcpy( local_dir , dir );
				::rux::string::replace_all( local_dir , '\\' , '/' );
#ifdef __WINDOWS__				
				if( local_dir[ strlen( local_dir ) - 1 ] == '/' )
					strcat( local_dir , "*" );
				else
					strcat( local_dir , "/*" );
				HANDLE find_handle = NULL;
				WIN32_FIND_DATAA win32_find_data;
				if( ( find_handle = FindFirstFileA( local_dir , &win32_find_data ) ) != INVALID_HANDLE_VALUE )
				{		
					res = true;
					do
					{
						if( ( win32_find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
						{
							if( strcmp( win32_find_data.cFileName , "." ) != 0
								&& strcmp( win32_find_data.cFileName , ".." ) != 0 )
							{
								declare_stack_variable( char , filename , 2048 );
								strcpy( filename , dir );
								::rux::string::replace_all( filename , '\\' , '/' );
								if( filename[ strlen( filename ) - 1 ] == '/' )
									strcat( filename , win32_find_data.cFileName );
								else
								{
									strcat( filename , "/" );
									strcat( filename , win32_find_data.cFileName );
								}
								::rux::string::replace_all( filename , '\\' , '/' );
								directories.Add( ::rux::engine::alloc_object< ::rux::io::directory >( filename , (const char*)__FILE__ , (::rux::uint32)__LINE__ , (char*)NULL , (::rux::malloc_t)::rux::engine::rux_malloc_dummy ) );
							}
						}
					}
					while( FindNextFileA( find_handle , &win32_find_data ) );
					FindClose( find_handle );
				}
#elif defined( __UNIX__ )
				if( local_dir[ strlen( local_dir ) - 1 ] != '/' )
					strcat( local_dir , "/" );
				DIR* dir_ptr = opendir( local_dir );
				if( dir_ptr )
				{
					res = true;
					::rux::int32 name_max = pathconf( local_dir , _PC_NAME_MAX );
					if( name_max == -1 )
						name_max = 255;
					size_t dirent_len = offsetof( struct dirent , d_name ) + name_max + 1;
					dirent* dirent_object_ptr = (dirent*)alloc_array_macros( char* , dirent_len );
					dirent* dirent_result = 0;
					struct stat io_stat;
					while( readdir_r( dir_ptr , dirent_object_ptr , &dirent_result ) == 0 && dirent_result )
					{
						if( strcmp( dirent_object_ptr->d_name , "." ) != 0
							&& strcmp( dirent_object_ptr->d_name , ".." ) != 0 )
						{
							declare_stack_variable( char , filename , 2048 );
							strcpy( filename , dir );
							::rux::string::replace_all( filename , '\\' , '/' );
							if( filename[ strlen( filename ) - 1 ] == '/' )
								strcat( filename , dirent_object_ptr->d_name );
							else
							{
								strcat( filename , "/" );
								strcat( filename , dirent_object_ptr->d_name );
							}
							stat( filename , &io_stat );
#ifdef __LINUX__
							if( dirent_object_ptr->d_type == DT_DIR )
#elif defined( __SOLARIS__ )
							if( io_stat.st_mode & S_IFDIR )
#endif
								directories.Add( ::rux::engine::alloc_object< ::rux::io::directory >( filename , (const char*)__FILE__ , (::rux::uint32)__LINE__ , (char*)NULL , (::rux::malloc_t)::rux::engine::rux_malloc_dummy ) );
						}
					}
					closedir( dir_ptr );
					::rux::engine::free_mem( dirent_object_ptr );	
				}
#endif
			}
			return res;
		};
		bool directory::exists( const char* filename )
		{	
			bool exists = rux_access( filename , 0 ) != -1;
			if( exists )
			{
				struct stat st;
				if( stat( filename , &st ) == 0 )
				{
		#ifdef __UNIX__
					if( S_ISDIR( st.st_mode ) == 0 )
						exists = false;
		#elif defined( __WINDOWS__ )
					if( ( st.st_mode & _S_IFDIR ) == 0 )
						exists = false;
		#endif
				}
				else
					exists = false;
			}
			return exists;
		};
		bool directory::get_all_directories( const char* dir , XMallocArray< directory* >& directories )
		{
			bool res = false;
			if( dir && strlen( dir ) > 0 ) 
			{
				declare_stack_variable( char , local_dir , 2048 );
				strcpy( local_dir , dir );
				::rux::string::replace_all( local_dir , '\\' , '/' );
#ifdef __WINDOWS__				
				if( local_dir[ strlen( local_dir ) - 1 ] == '/' )
					strcat( local_dir , "*" );
				else
					strcat( local_dir , "/*" );
				HANDLE find_handle = NULL;
				WIN32_FIND_DATAA win32_find_data;
				if( ( find_handle = FindFirstFileA( local_dir , &win32_find_data ) ) != INVALID_HANDLE_VALUE )
				{		
					res = true;
					do
					{
						if( ( win32_find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
						{
							if( strcmp( win32_find_data.cFileName , "." ) != 0
								&& strcmp( win32_find_data.cFileName , ".." ) != 0 )
							{
								declare_stack_variable( char , filename , 2048 );
								strcpy( filename , dir );
								::rux::string::replace_all( filename , '\\' , '/' );
								if( filename[ strlen( filename ) - 1 ] == '/' )
									strcat( filename , win32_find_data.cFileName );
								else
								{
									strcat( filename , "/" );
									strcat( filename , win32_find_data.cFileName );
								}
								directories.Add( ::rux::engine::alloc_object< ::rux::io::directory >( filename , (const char*)__FILE__ , (::rux::uint32)__LINE__ , (char*)NULL , (::rux::malloc_t)::rux::engine::rux_malloc_dummy ) );
							}
						}
					}
					while( FindNextFileA( find_handle , &win32_find_data ) );
					FindClose( find_handle );
				}
#elif defined( __UNIX__ )
				DIR* dir_ptr = opendir( local_dir );
				if( dir_ptr )
				{
					res = true;
					::rux::int32 name_max = pathconf( local_dir , _PC_NAME_MAX );
					if( name_max == -1 )
						name_max = 255;
					size_t dirent_len = offsetof( struct dirent , d_name ) + name_max + 1;
					dirent* dirent_object_ptr = (dirent*)alloc_array_macros( char* , dirent_len );
					dirent* dirent_result = 0;
					struct stat io_stat;
					while( readdir_r( dir_ptr , dirent_object_ptr , &dirent_result ) == 0 && dirent_result )
					{
						if( strcmp( dirent_object_ptr->d_name , "." ) != 0
							&& strcmp( dirent_object_ptr->d_name , ".." ) != 0 )
						{
							declare_stack_variable( char , filename , 2048 );
							strcpy( filename , dir );
							::rux::string::replace_all( filename , '\\' , '/' );
							if( filename[ strlen( filename ) - 1 ] == '/' )
								strcat( filename , dirent_object_ptr->d_name );
							else
							{
								strcat( filename , "/" );
								strcat( filename , dirent_object_ptr->d_name );
							}
							stat( filename , &io_stat );
#ifdef __LINUX__
							if( dirent_object_ptr->d_type == DT_DIR )
#elif defined( __SOLARIS__ )
							if( io_stat.st_mode & S_IFDIR )
#endif
								directories.Add( ::rux::engine::alloc_object< ::rux::io::directory >( filename , (const char*)__FILE__ , (::rux::uint32)__LINE__ , (char*)NULL , (::rux::malloc_t)::rux::engine::rux_malloc_dummy ) );
						}
					}
					closedir( dir_ptr );
					::rux::engine::free_mem( dirent_object_ptr );	
				}
#endif
				for( ::rux::uint32 index1 = 0 ; index1 < directories.Count() ; index1++ )
					::rux::io::directory::get_directories( directories[ index1 ]->_dir , directories );				
			}
			return res;
		};
		bool directory::remove_all( const char* dir )
		{
			bool res = false;
			if( dir && strlen( dir ) > 0 ) 
			{
				XMallocArray< ::rux::io::file* > files;
				get_all_files( dir , files );
				for( size_t index0 = 0 ; index0 < files.Count() ; index0++ )
				{
					files[ index0 ]->remove();
					::rux::engine::free_object< ::rux::io::file >( files[ index0 ] , ::rux::engine::rux_free_dummy );
				}
				XMallocArray< ::rux::io::directory* > dirs;
				get_all_directories( dir , dirs );
				if( dirs.Count() > 0 )
				{
					for( size_t index0 = dirs.Count() - 1 ; index0 >= 0 ; index0-- )
					{
						::rux::io::directory::remove( dirs[ index0 ]->_dir );
						::rux::engine::free_object< ::rux::io::directory >( dirs[ index0 ] , ::rux::engine::rux_free_dummy );
						if( index0 == 0 )
							break;
					}
				}
				res = ::rux::io::directory::remove( dir );
			}
			return res;
		};
		bool file::rename( const char* old_file , const char* new_file )
		{
			bool res = false;
			if( old_file && strlen( old_file ) > 0 &&
				new_file && strlen( new_file ) > 0 )
			{
				if( ::rename( old_file , new_file ) == 0 )
					res = true;
			}
			return res;
		};
		bool directory::remove( const char* dir )
		{
			bool res = false;
			if( dir && strlen( dir ) > 0 ) 
			{
#ifdef __WINDOWS__
				if( _access( dir , 0 ) != -1 )
				{
					struct _stat st;
					if( _stat( dir , &st ) == 0 )
					{
						if( ( st.st_mode & _S_IFDIR ) )
						{
							if( _rmdir( dir ) == 0 )
								res = true;
						}
					}
				}
#endif
#ifdef __UNIX__				
				if( access( dir , F_OK ) != -1 )
				{			
					struct stat st;
					if( stat( dir , &st ) == 0 )
					{
						if( S_ISDIR( st.st_mode ) )
						{
							if( rmdir( dir ) == 0 )
								res = true;
						}
					}
				}
#endif		
			}
			return res;
		};
		bool directory::rename( const char* old_dir , const char* new_dir )
		{
			bool res = false;
			if( old_dir && strlen( old_dir ) > 0 &&
				new_dir && strlen( new_dir ) > 0 )
			{
				if( ::rename( old_dir , new_dir ) == 0 )
					res = true;
			}
			return res;
		};
		char* directory::get_directory_path( void )
		{
			return _dir;
		};
		bool directory::create( const char* dir )
		{
			bool res = false;
			if( dir && strlen( dir ) > 0 ) 
			{
#ifdef __WINDOWS__
				if( _access( dir , 0 ) == -1 )
				{
					if( _mkdir( dir ) == 0 )
						res = true;
				}
				else
				{
					struct _stat st;
					if( _stat( dir , &st ) == 0 )
					{
						if( ( st.st_mode & _S_IFDIR ) == 0 )
						{
							if( _mkdir( dir ) == 0 )
								res = true;
						}
						else
							res = true;
					}
				}
#else
				if( access( dir , F_OK ) == -1 )
				{
					if( mkdir( dir , S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH ) != 0 )
						res = true;
				}
				else
				{
					struct stat st;
					if( stat( dir , &st ) == 0 )
					{
						if( S_ISDIR( st.st_mode ) == 0 )
						{
							if( mkdir( dir , S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH ) == 0 )
								res = true;
						}
						else
							res = true;
					}
				}
#endif
			}
			return res;
		};
		void* file::read_all( const char* filename , size_t& readen_size )
		{
			readen_size = 0;
			::rux::uint8* result = NULL;
			::rux::uint8 is_exists = ::rux::io::file::exists( filename );
			::rux::int32 file = -1;
			if( is_exists == 1 )
			{
			#ifdef __WINDOWS__
				file = rux_open( filename , O_RDONLY | O_BINARY );
			#elif defined( __UNIX__ )
				file = rux_open( filename , O_RDONLY );
			#endif
				if( file != -1 )
				{			
					size_t size = 1024 , avail_size = 1024 , read_size = 0;
					result = alloc_array_macros( ::rux::uint8 , size );
					do
					{
						readen_size += read_size;
						avail_size -= read_size;
						if( avail_size == 0 )
						{
							size += 1024;
							result = ::rux::engine::realloc_array< ::rux::uint8 >( result , size );
							avail_size = 1024;
						}			
			#ifdef __WINDOWS__
						read_size = _read( file , &result[ readen_size ] , avail_size ); 
			#else
						read_size = ::read( file , &result[ readen_size ] , avail_size ); 
			#endif
					}
					while( read_size != 0 && read_size != -1 );
					rux_close( file );
				}
			}
			return result;
		};
		void file::remove( const char* filename )
		{
			if( ::rux::io::file::exists( filename ) == 1 )
				::remove( filename );
		};
		::rux::uint64 file::get_size( const char* filename )
		{
			::rux::uint64 size = 0ULL;
#ifdef __WINDOWS__
			HANDLE file = CreateFileA( filename , GENERIC_READ | GENERIC_WRITE , 0 , NULL , OPEN_EXISTING , FILE_ATTRIBUTE_NORMAL , NULL ); 
			if( file != NULL && file != INVALID_HANDLE_VALUE ) 
			{
				LARGE_INTEGER large_integer;
				memset( &large_integer , 0 , sizeof( LARGE_INTEGER ) );
				GetFileSizeEx( file , &large_integer );
				size = large_integer.QuadPart;
				CloseHandle( file );
			}
#elif defined( __UNIX__ )
			struct stat st;
			if( stat( filename , &st ) == 0 )
				size = st.st_size;
#endif
			return size;
		};
		void file::append( const char* filename , const char* text )
		{
			::rux::uint8 is_exists = file::exists( filename );
			::rux::int32 file = -1;
			if( is_exists == 1 )
				file = rux_open( filename , O_WRONLY | O_APPEND );
			else
				file = rux_open( filename , O_CREAT | O_WRONLY | O_TRUNC );
			if( file != -1 )
			{
				rux_lseek( file , 0L , SEEK_END );
				rux_write( file , text , strlen( text ) );
#ifdef __UNIX__
				fsync( file );
#endif
				rux_close( file );
			}
		};
		directory::directory( const char* dir )
		{
			size_t len = strlen( dir );
			_dir = alloc_array_macros( char , len + 1 );
			::memcpy( _dir , dir , len );
			_dir[ len ] = 0;	
		};
		directory::~directory()
		{
			if( _dir )
			{
				::rux::engine::free_mem( _dir );
				_dir = NULL;
			}
		};
		file::file( void )
		{
			_file = NULL;
			_filename = NULL;
			_file_mode = XReadText;
		};
		file::file( const char* filename , XFileMode file_mode )
		{
			_file = NULL;
			_filename = NULL;
			this->open( filename , file_mode );
		};
		file::~file()
		{
			close();
		};
		bool file::opened( void )
		{
			return _file ? true : false;
		};
		bool file::reopen( XFileMode file_mode )
		{
			if( _file )
			{
				if( _file_mode != file_mode )
				{
					::rux::uint64 cur = ::rux::io::ftell64( _file );
					::rux::io::fclose( _file );
					if( file_mode == XReadText )
						_file = ::rux::io::fopen( _filename , "rt" );			
					else if( file_mode == XReadBinary )
						_file = ::rux::io::fopen( _filename , "rb" );			
					else if( file_mode == XCreateWriteText )
						_file = ::rux::io::fopen( _filename , "wt" );			
					else if( file_mode == XOpenWriteText )
						_file = ::rux::io::fopen( _filename , "r+t" );			
					else if( file_mode == XCreateWriteBinary )
						_file = ::rux::io::fopen( _filename , "wb" );			
					else if( file_mode == XOpenWriteBinary )
						_file = ::rux::io::fopen( _filename , "r+b" );
					if( _file )
					{
						_file_mode = file_mode;
						::rux::io::fseek64( _file , cur , SEEK_SET );
						return true;
					}
					else
					{
						if( _filename )
						{
							::rux::engine::free_mem( _filename );
							_filename = NULL;
						}
						return false;
					}
				}
				else
					return true;
			}
			else
				return false;
		};
		bool file::open( const char* filename , XFileMode file_mode )
		{
			close();
			if( file_mode == XReadText )
				_file = ::rux::io::fopen( filename , "rt" );			
			else if( file_mode == XReadBinary )
				_file = ::rux::io::fopen( filename , "rb" );			
			else if( file_mode == XCreateWriteText )
				_file = ::rux::io::fopen( filename , "wt" );			
			else if( file_mode == XOpenWriteText )
				_file = ::rux::io::fopen( filename , "r+t" );			
			else if( file_mode == XCreateWriteBinary )
				_file = ::rux::io::fopen( filename , "wb" );			
			else if( file_mode == XOpenWriteBinary )
				_file = ::rux::io::fopen( filename , "r+b" );
			if( _file )
			{
				_file_mode = file_mode;
				size_t len = strlen( filename );
				_filename = alloc_array_macros( char , len + 1 );
				::memcpy( _filename , filename , len );
				_filename[ len ] = 0;				
			}
			return _file ? true : false;
		};
		bool file::remove( void )
		{
			if( _file )
			{
				size_t len = strlen( _filename );
				char* filename = alloc_array_macros( char , len + 1 );
				::memcpy( filename , _filename , len );
				filename[ len ] = 0;
				close();
				if( ::remove( filename ) == 0 )
				{
					::rux::engine::free_mem( filename );
					return true;
				}
				else
				{
					::rux::engine::free_mem( filename );
					return false;
				}
			}
			else
				return false;
		};
		bool file::write( char* buffer , size_t buffer_size , size_t& written )
		{
			written = 0;
			if( _file )
			{
				size_t write_size = 0;
				while( ( write_size = ::rux::io::fwrite( &buffer[ written ] , sizeof( char ) , buffer_size , _file ) ) > 0 )
				{
					written += write_size;
					if( written == buffer_size )
						break;
				}			
				if( written != buffer_size )
					return false;
				else
				{
					::rux::io::fflush( _file );
					return true;
				}
			}
			else
				return false;
		};
		bool file::read( char* buffer , size_t buffer_size , size_t& readen )
		{
			readen = 0;
			if( _file )
			{
				size_t read_size = 0;
				while( ( read_size = ::rux::io::fread( &buffer[ readen ] , sizeof( char ) , buffer_size , _file ) ) > 0 )
				{
					readen += read_size;
					if( readen == buffer_size )
						break;
				}			
				if( ::rux::io::_ferror( _file ) && ::rux::io::_feof( _file ) == 0 )
					return false;
				else if( readen > 0 )
					return true;
				else
					return false;
			}
			else
				return false;
		};
		bool file::eof( void )
		{
			return _file && ::rux::io::_feof( _file ) != 0;
		};
		bool file::rename( const char* new_file )
		{
			if( _file )
			{
				size_t len = strlen( _filename );
				char* filename = alloc_array_macros( char , len + 1 );
				::rux::engine::memcpy( filename , 0 , _filename , len );
				filename[ len ] = 0;
				close();
				if( file::rename( filename , new_file ) )
				{
					this->open( new_file , _file_mode );
					::rux::engine::free_mem( filename );
					return true;
				}
				else
				{
					this->open( filename , _file_mode );
					::rux::engine::free_mem( filename );
					return false;
				}
			}
			else
				return false;
		};
		bool file::write_bytes( char* buffer , size_t buffer_size , size_t& written )
		{
			written = 0;
			if( reopen( XOpenWriteBinary ) )
			{
				if( write( buffer , buffer_size , written ) )
					return true;
				else
					return false;
			}
		else
				return false;
		};
		bool file::write_text_va_list( const char* format , va_list pargs )
		{
			if( reopen( XOpenWriteText ) )
			{
				if( ::rux::io::vfprintf( _file , format , pargs ) )
					return true;
				else
					return false;
			}
			else
				return false;	
		};
		bool file::write_text( const char* format , ... )
		{
			if( reopen( XOpenWriteText ) )
			{
				va_list ap;
				va_start( ap , format );
				if( ::rux::io::vfprintf( _file , format , ap ) )
				{
					va_end( ap );
					return true;
				}
				else
				{
					va_end( ap );
					return false;
				}
			}
			else
				return false;	
		};
		bool file::read_bytes( char* buffer , size_t buffer_size , size_t& readen )
		{
			readen = 0;
			if( reopen( XReadBinary ) )
			{
				if( read( buffer , buffer_size , readen ) )
					return true;
				else
					return false;
			}
			else
				return false;
		};
		bool file::read_text( char* buffer , size_t buffer_size , size_t& readen )
		{
			readen = 0;
			if( reopen( XReadText ) )
			{
				if( read( buffer , buffer_size , readen ) )
				{
					if( buffer_size == readen )
						buffer[ buffer_size - 1 ] = 0;
					else
					{
						buffer[ readen ] = 0;
						readen++;
					}
					return true;
				}
				else
					return false;
			}
			else
				return false;
		};
		bool file::clear( void )
		{
			if( size( 0ULL ) )
			{
				seek( 0ULL );
				return true;
			}
			else
				return false;
		};
		bool file::size( ::rux::uint64 size )
		{
			if( reopen( XOpenWriteText ) )
			{
#ifdef __WINDOWS__
				if( ::rux::io::chsize( _fileno( _file ) , size ) != 0 )
#else
				if( ::rux::io::chsize( fileno( _file ) , size ) != 0 )
#endif
					return false;
				else
				{
					::rux::io::fflush( _file );
					return true;
				}
			}
			else
				return false;
		};
		::rux::uint64 file::position( void )
		{
			::rux::uint64 cur = 0ULL;
			if( _file )
				cur = ::rux::io::ftell64( _file );
			return cur;
		};
		::rux::uint64 file::size( void )
		{
			::rux::uint64 size = 0ULL;
			if( _file )
			{
				::rux::int64 cur = ::rux::io::ftell64( _file );
				if( cur >= 0 )
				{
					if( ::rux::io::fseek64( _file , 0L , SEEK_END ) == 0 )
					{
						::rux::int64 res = ::rux::io::ftell64( _file );
						if( res >= 0 )
							size = res;
						else
							errno_error;
						if( ::rux::io::fseek64( _file , cur , SEEK_SET ) != 0 )
							errno_error;
					}
					else
						errno_error;
				}
				else
					errno_error;
			}
			else
				::rux::log::WriteError( "%s, %s:%d, error _file is null" , __FUNCTION__ , __FILE__ , __LINE__ );
			if( size == 0ULL && _filename )
			{
				struct stat st;
				if( stat( _filename , &st ) == 0 )
					size = st.st_size;
				else
					errno_error;
			}
			return size;
		};
		bool file::seek( ::rux::uint64 position , XEnum_SetPosition type )
		{
			if( _file )
			{
				if( ::rux::io::fseek64( _file , position , type ) == 0 )
					return true;
				else
					return false;
			}
			else
				return false;
		};
		bool file::copy( const char* dst_filename )
		{
			bool res = false;
			::rux::io::file dst_file( dst_filename , XCreateWriteBinary );
			if( dst_file.opened() )
			{				
				char* buffer = NULL;
				::rux::uint64 dst_size = 0;
				::rux::uint64 src_size = size();
				size_t buffer_size = src_size;
				for( ; ; )
				{
					if( buffer_size == 0 )
						break;
					buffer = (char*)_aligned_malloc( buffer_size , 32 );
					if( buffer )
						break;
					buffer_size /= 2;
				}
				if( buffer )
				{
					size_t readen = 0 , written = 0;
					while( read_bytes( buffer , buffer_size , readen ) )
					{
						if( dst_file.write_bytes( buffer , buffer_size , written ) == false )
							break;
						else
							dst_size += written;
					}
					::rux::engine::rux_free_dummy( buffer );
				}
				if( dst_size == src_size )
					res = true;
				else
					dst_file.remove();
				dst_file.close();
			}
			return res;
		};
		bool file::close( void )
		{
			if( _file )
			{
				::rux::io::fclose( _file );
				_file = NULL;
				if( _filename )
				{
					::rux::engine::free_mem( _filename );
					_filename = NULL;
				}
				return true;
			}
			else
			{
				if( _filename )
				{
					::rux::engine::free_mem( _filename );
					_filename = NULL;
				}
				return false;
			}
		};
		::rux::uint8 file::exists( const char* filename , ::rux::uint8 find_link_too )
		{
			::rux::uint8 is_file_exists = rux_access( filename , 0 ) != -1 ? 1 : 0;
			if( is_file_exists == 1 )
			{
				struct stat st;
				if( stat( filename , &st ) == 0 )
				{
#ifdef __UNIX__
					if( S_ISREG( st.st_mode ) == 0
						&& ( find_link_too == 0 || S_ISLNK( st.st_mode ) == 0 ) )
						is_file_exists = 0;
#elif defined( __WINDOWS__ )
					if( ( st.st_mode & _S_IFREG ) == 0 )
						is_file_exists = 0;		
#endif
				}
			}
			return is_file_exists;
		};		
		void path::get_filename_without_extension( ::rux::string& filename )
		{
			filename.replace_all( '\\' , '/' );
			::rux::int64 index0 = filename.last_index_of( "/" );
			if( index0 > -1 && static_cast< size_t >( index0 + 1 ) < filename.length() )	
				filename.substring( index0 + 1 );
			index0 = filename.last_index_of( "." );
			if( index0 > -1 )
				filename.substring( 0 , index0 );
		};
		void path::get_filename( ::rux::string& filename )
		{
			filename.replace_all( '\\' , '/' );
			::rux::int64 index0 = filename.last_index_of( "/" );
			if( index0 > -1 && static_cast< size_t >( index0 + 1 ) < filename.length() )	
				filename.substring( index0 + 1 );
		};
		void path::get_filename_without_extension( const char* filename )
		{				
			char* filename_ = const_cast< char* >( filename );
			rux_replace( filename_ , '\\' , '/' );
			::rux::int32 index0 = rux_last_index_of( filename_ , '/' );
			if( index0 > -1 && static_cast< size_t >( index0 + 1 ) < strlen( filename_ ) )	
				::rux::string::substring( filename_ , filename_ , index0 + 1 );
			index0 = rux_last_index_of( filename_ , '.' );
			if( index0 > -1 )
				::rux::string::substring( filename_ , filename_ , 0 , index0 , 0 );
		};
		void path::get_filename_without_extension( const char* filename , char destination[ 1024 ] )
		{
			destination[ 0 ] = 0;
			rux_concatenation( destination , filename );
			rux_replace( destination , '\\' , '/' );
			::rux::int32 index0 = rux_last_index_of( destination , '/' );
			if( index0 > -1 && static_cast< size_t >( index0 + 1 ) < strlen( destination ) )	
				::rux::string::substring( destination , destination , index0 + 1 );
			index0 = rux_last_index_of( destination , '.' );
			if( index0 > -1 )
				::rux::string::substring( destination , destination , 0 , index0 , 0 );
		};
		void GetDirectories( const ::rux::XString& directory_name , ::rux::XArray< ::rux::XString >& directories )
		{
			if( directory_name.Length() > 0 )
			{
				::rux::XString local_directory_name( directory_name );	
#ifdef __WINDOWS__
				local_directory_name = local_directory_name.ReplaceAll( '\\' , '/' );	
				::rux::uint32 symbol = local_directory_name.get_UTF8Char( local_directory_name.Length() - 1 );
				if( symbol == '/' )
					local_directory_name += L"*";
				else
					local_directory_name += L"/*";			
				local_directory_name.set_ByRef( local_directory_name.ConvertToUTF16() );
				HANDLE find_handle = NULL;
				WIN32_FIND_DATAW win32_find_data;
				if( ( find_handle = FindFirstFileW( (wchar_t*)local_directory_name.str() , &win32_find_data ) ) != INVALID_HANDLE_VALUE )
				{					
					do
					{
						if( ( win32_find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
						{
							if( XString::IsEqualUTF16( (::rux::uint16*)win32_find_data.cFileName , (::rux::uint16*)L"." ) == 0
								&& XString::IsEqualUTF16( (::rux::uint16*)win32_find_data.cFileName , (::rux::uint16*)L".." ) == 0 )
							{								
								::rux::XString directory( directory_name );
								directory.set_ByRef( directory.ReplaceAll( '\\' , '/' ) );
								symbol = directory.get_UTF8Char( directory.Length() - 1 );
								if( symbol == '/' )
									directory += win32_find_data.cFileName;		
								else
								{
									directory += "/";
									directory += win32_find_data.cFileName;		
								}							
								directories.Add( directory );
							}
						}
					}
					while( FindNextFileW( find_handle , &win32_find_data ) );
					FindClose( find_handle );
				}		
#endif
#ifdef __UNIX__
				local_directory_name.set_ByRef( local_directory_name.ReplaceAll( '\\' , '/' ) );
				local_directory_name.set_ByRef( local_directory_name.ConvertToUTF8() );
				DIR* dir_ptr = opendir( local_directory_name.str() );
				if( dir_ptr )
				{
					::rux::int32 name_max = pathconf( local_directory_name.str() , _PC_NAME_MAX );
					if( name_max == -1 )
						name_max = 255;
					size_t dirent_len = offsetof( struct dirent , d_name ) + name_max + 1;
					dirent* dirent_object_ptr = (dirent*)alloc_array_macros( char* , dirent_len );
					dirent* dirent_result = 0;					
					::rux::uint32 symbol = 0;
#ifdef __SOLARIS__
					struct stat io_stat;
					::rux::XString full_directory;
#endif
					while( readdir_r( dir_ptr , dirent_object_ptr , &dirent_result ) == 0 && dirent_result )
					{
						if( XString::IsEqualA( dirent_object_ptr->d_name , "." ) == 0
							&& XString::IsEqualA( dirent_object_ptr->d_name , ".." ) == 0 )
						{
#ifdef __LINUX__
							if( dirent_object_ptr->d_type == DT_DIR )
#elif defined( __SOLARIS__ )
							full_directory = directory_name;
							full_directory = full_directory.ReplaceAll( '\\' , '/' );
							symbol = full_directory.get_UTF8Char( full_directory.Length() - 1 );
							if( symbol == '/' )
								full_directory += dirent_object_ptr->d_name;
							else
							{
								full_directory += "/";
								full_directory += dirent_object_ptr->d_name;
							}
							stat( full_directory.str() , &io_stat );
							if( io_stat.st_mode & S_IFDIR )
#endif
							{
#ifdef __SOLARIS__
								::rux::XString directory( full_directory );
#elif defined( __LINUX__ )
								::rux::XString directory( directory_name );
								directory.set_ByRef( directory.ReplaceAll( '\\' , '/' ) );
								symbol = directory.get_UTF8Char( directory.Length() - 1 );
								if( symbol == '/' )
									directory += dirent_object_ptr->d_name;		
								else
								{
									directory += "/";
									directory += dirent_object_ptr->d_name;		
								}
#endif
								directories.Add( directory );
							}
						}
					}
					closedir( dir_ptr );
					::rux::engine::free_mem( dirent_object_ptr );					
				}
#endif
			}
		};
		Array< ::rux::XString >& GetAllDirectories( const ::rux::XString& directory_name )
		{
			XArray< XString > directories;
			if( directory_name.Length() > 0 )
			{
				::rux::XString local_directory_name( directory_name );
#ifdef __WINDOWS__			
				local_directory_name = local_directory_name.ReplaceAll( '\\' , '/' );	
				::rux::uint32 symbol = local_directory_name.get_UTF8Char( local_directory_name.Length() - 1 );
				if( symbol == '/' )
					local_directory_name += L"*";
				else
					local_directory_name += L"/*";			
				local_directory_name.set_ByRef( local_directory_name.ConvertToUTF16() );
				HANDLE find_handle = NULL;
				WIN32_FIND_DATAW win32_find_data;
				if( ( find_handle = FindFirstFileW( (wchar_t*)local_directory_name.str() , &win32_find_data ) ) != INVALID_HANDLE_VALUE )
				{					
					do
					{
						if( ( win32_find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
						{
							if( XString::IsEqualUTF16( (::rux::uint16*)win32_find_data.cFileName , (::rux::uint16*)L"." ) == 0
								&& XString::IsEqualUTF16( (::rux::uint16*)win32_find_data.cFileName , (::rux::uint16*)L".." ) == 0 )
							{
								::rux::XString directory( directory_name );
								directory.set_ByRef( directory.ReplaceAll( '\\' , '/' ) );
								symbol = directory.get_UTF8Char( directory.Length() - 1 );
								if( symbol == '/' )
									directory += 	win32_find_data.cFileName;		
								else
								{
									directory += "/";
									directory += win32_find_data.cFileName;		
								}							
								directories.Add( directory );							
							}
						}
					}
					while( FindNextFileW( find_handle , &win32_find_data ) );
					FindClose( find_handle );
					for( ::rux::uint32 index1 = 0 ; index1 < directories.Count() ; index1++ )
						GetDirectories( directories[ index1 ] , directories );				
				}
#endif
#ifdef __UNIX__
				local_directory_name.set_ByRef( local_directory_name.ReplaceAll( '\\' , '/' ) );
				local_directory_name.set_ByRef( local_directory_name.ConvertToUTF8() );
				DIR* dir_ptr = opendir( local_directory_name.str() );
				if( dir_ptr )
				{
					::rux::int32 name_max = pathconf( local_directory_name.str() , _PC_NAME_MAX );
					if( name_max == -1 )
						name_max = 255;
					size_t dirent_len = offsetof( struct dirent , d_name ) + name_max + 1;
					dirent* dirent_object_ptr = (dirent*)alloc_array_macros( char* , dirent_len );
					dirent* dirent_result = 0;					
					::rux::uint32 symbol = 0;
#ifdef __SOLARIS__
					struct stat io_stat;
					::rux::XString full_directory;
#endif
					while( readdir_r( dir_ptr , dirent_object_ptr , &dirent_result ) == 0 && dirent_result )
					{
						if( XString::IsEqualA( dirent_object_ptr->d_name , "." ) == 0
							&& XString::IsEqualA( dirent_object_ptr->d_name , ".." ) == 0 )
						{
#ifdef __LINUX__
							if( dirent_object_ptr->d_type == DT_DIR )
#elif defined( __SOLARIS__ )
							full_directory = directory_name;
							full_directory = full_directory.ReplaceAll( '\\' , '/' );
							symbol = full_directory.get_UTF8Char( full_directory.Length() - 1 );
							if( symbol == '/' )
								full_directory += dirent_object_ptr->d_name;
							else
							{
								full_directory += "/";                                                        
								full_directory += dirent_object_ptr->d_name;
							}
							stat( full_directory.str() , &io_stat );
							if( io_stat.st_mode & S_IFDIR )
#endif
							{								
#ifdef __SOLARIS__
								::rux::XString directory( full_directory );
#elif defined( __LINUX__ )
								::rux::XString directory( directory_name );
								directory.set_ByRef( directory.ReplaceAll( '\\' , '/' ) );
								symbol = directory.get_UTF8Char( directory.Length() - 1 );
								if( symbol == '/' )
									directory += dirent_object_ptr->d_name;		
								else
								{
									directory += "/";
									directory += dirent_object_ptr->d_name;		
								}
#endif
								directories.Add( directory );
							}
						}
					}
					closedir( dir_ptr );
					::rux::engine::free_mem( dirent_object_ptr );
					for( ::rux::uint32 index1 = 0 ; index1 < directories.Count() ; index1++ )
						GetDirectories( directories[ index1 ] , directories );				
				}
#endif
			}	
			return directories++;
		};
	};
	namespace threading
	{
#ifdef __WINDOWS__
		HANDLE get_current_thread( void )
		{
			return GetCurrentThread();
		};
#else
		pthread_t get_current_thread( void )
		{
			return pthread_self();
		};
#endif
	};
	namespace diagnostics
	{		
#ifdef __WINDOWS__
		::rux::uint32 private_WaitForProcess( HANDLE handle )
		{
			::rux::uint32 result = NOERROR;
			MSG msg;
			memset( (void*)&msg , 0 , sizeof( MSG ) );
			::rux::uint8 is_exit = 0;
			for( ; ; )
				{
					switch ( MsgWaitForMultipleObjects( 1 , &handle , false , ::rux::threading::infinite , QS_ALLINPUT ) )
					{
					case WAIT_OBJECT_0:
						{
							is_exit = 1;
							break;
						}
					case (WAIT_OBJECT_0 + 1):
						{
							if (PeekMessage( &msg, NULL, NULL, NULL, PM_REMOVE ) ) 
							{
								TranslateMessage(&msg);
								DispatchMessage(&msg);
							}
							break;
						}
					default:
						{
							result = GetLastError();
							is_exit = 1;
							break;
						}
					}
					if( is_exit == 1 )
						break;
			}
			return result;
		};
#endif
		::rux::uint8 is_admin_user_process( void )
		{
			::rux::uint8 is_admin_user = 0;
#ifdef __WINDOWS__
			SID_IDENTIFIER_AUTHORITY nt_authority = SECURITY_NT_AUTHORITY;
			PSID administrators_group = NULL; 
			BOOL is_admin = AllocateAndInitializeSid( &nt_authority , 2 , SECURITY_BUILTIN_DOMAIN_RID , DOMAIN_ALIAS_RID_ADMINS , 0 , 0 , 0 , 0 , 0 , 0 , &administrators_group ); 
			if( is_admin ) 
			{
				if( CheckTokenMembership( NULL , administrators_group , &is_admin ) == FALSE ) 
					is_admin = FALSE;
				FreeSid( administrators_group ); 
			}
			is_admin_user = is_admin ? 1 : 0;
#else
			is_admin_user = geteuid() == 0 ? 1 : 0;
#endif
			return is_admin_user;
		};
		void process::get_working_directory( char* string , size_t string_size )
		{
#ifdef __WINDOWS__
			_getcwd( string , string_size );
#elif defined( __UNIX__ )
			getcwd( string , string_size );
#endif
		};
#ifdef __WINDOWS__
		::rux::uint8 process::adjust_token_privileges( HANDLE process_handle , char* error_string , size_t error_string_size )
		{
			if( process_handle == NULL )
				process_handle = GetCurrentProcess();
			::rux::byte res = 0;
			HANDLE token = NULL; 			
			if ( OpenProcessToken( process_handle , TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY , &token ) != 0 ) 
			{
				TOKEN_PRIVILEGES token_privileges;
				if( LookupPrivilegeValue( NULL , SE_DEBUG_NAME , &token_privileges.Privileges[ 0 ].Luid ) != 0 )
				{
					token_privileges.PrivilegeCount = 1;
					token_privileges.Privileges[ 0 ].Attributes = SE_PRIVILEGE_ENABLED; 					
					if( AdjustTokenPrivileges( token , FALSE , &token_privileges , 0 , (PTOKEN_PRIVILEGES)NULL , 0 ) != 0 )
						res = 1;
					else
						XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError , error_string );
				}
				else
					XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError , error_string );
				CloseHandle( token );
			}
			else
				XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError , error_string );
			return res;
		};
#endif
		::rux::uint8 process::killall( const char* module_name )
		{			
			::rux::uint8 is_killed = 0;
#ifdef __WINDOWS__
			::rux::diagnostics::process::adjust_token_privileges();
			DWORD processes[ 1024 ] = {0} , size = 0 , processes_count = 0;
			declare_stack_variable( char , alter_process_name , 1024 );
			rux_concatenation( alter_process_name , module_name );
			rux_replace( alter_process_name , '/' , '\\' );
			if( ::rux::engine::_globals->_psapi_module.EnumProcesses( processes , sizeof( processes ) , &size ) )
			{
				processes_count = size / sizeof( DWORD );
				declare_stack_variable( char , process_name_ptr , 1024 );
				declare_stack_variable( char , process_name_without_extension , 1024 );
				HANDLE process_handle = NULL;
				HMODULE module_handle = NULL;
				for( size_t index0 = 0 ; index0 < processes_count ; index0++ )
				{
					if( processes[ index0 ] != 0 )
					{
						process_handle = OpenProcess( SYNCHRONIZE | PROCESS_TERMINATE  | PROCESS_QUERY_INFORMATION | PROCESS_VM_READ , FALSE , processes[ index0 ] );
						if( process_handle != NULL )
						{					
							if( ::rux::engine::_globals->_psapi_module.EnumProcessModules( process_handle , &module_handle , sizeof( module_handle ) , &size ) )
							{						
								if( ::rux::engine::_globals->_psapi_module.GetModuleFileNameExA( process_handle , module_handle , process_name_ptr , 1024 ) > 0 )
								{
									if( strcmp( module_name , process_name_ptr ) == 0
										|| alter_process_name == process_name_ptr )
									{
										if(TerminateProcess(process_handle, 1112))
										{
											WaitForSingleObject( process_handle , ::rux::threading::infinite );
											is_killed = 1;
										}
									}	
								}
								if( ::rux::engine::_globals->_psapi_module.GetModuleBaseNameA( process_handle , module_handle , process_name_ptr , sizeof( process_name_ptr ) / sizeof( char ) ) > 0 )
								{
									::rux::io::path::get_filename_without_extension( process_name_ptr , process_name_without_extension );
									if( strcmp( module_name , process_name_ptr ) == 0
										|| strcmp( process_name_without_extension , module_name ) == 0 )
									{
										if(TerminateProcess(process_handle, 1112))
										{
											WaitForSingleObject( process_handle , ::rux::threading::infinite );
											is_killed = 1;
										}
									}
								}						
							}					
							CloseHandle( process_handle );
						}				
					}
				}
			}
#else
			::rux::XString directory_name , file_status_string;
			XArray< XString > file_status_lines , file_status_pair;
			XArray< XString > dirs;
			::rux::io::GetDirectories( "/proc" , dirs );
			::rux::uint32 index1 = 0;
			declare_stack_variable( char , fullpath , 1024 );
			::rux::XString alter_process_name , process_name( module_name );
			alter_process_name.set_ByRef( process_name.ReplaceAll( '\\' , '/' ) );
			for( ::rux::uint32 index0 = 0 ; index0 < dirs.Count() ; index0++ )
			{
				directory_name.set_ByRef( dirs[ index0 ] );
				file_status_string = directory_name + "/exe";		
				memset( fullpath , 0 , 1024 );
				readlink( file_status_string.str() , fullpath , 1024 );
				if( process_name == fullpath
					|| alter_process_name == fullpath )
				{
					file_status_pair.set_ByRef( directory_name.Split( '/' ) );
					if( file_status_pair.Count() > 1 )
					{
						XString pidstr;
						pidstr.set_ByRef( file_status_pair[ 1 ] );
						::rux::pid_t pid = pidstr.ToUInt32();
						if( ::kill( pid , 9 ) == -1 )
							is_killed = 0;
						else
						{
							waitpid( pid , NULL , 0 );
							is_killed = 1;
						}
					}
				}
				else
				{
					file_status_string = directory_name + "/status";
					file_status_string.set_ByRef( file_status_string.ConvertToUTF8() );
					if( ::rux::io::file::exists( file_status_string.str() ) == 1 )
					{
						size_t readen_size = 0;
						char* lines = (char*)rux::io::file::read_all( file_status_string.str() , readen_size );
						if( lines )
						{
							file_status_string = lines;
							file_status_lines.set_ByRef( file_status_string.Split( '\n' ) );
							for( index1 = 0 ; index1 < file_status_lines.Count() ; index1++ )
							{
								XString pidstr;
								pidstr.set_ByRef( file_status_pair[ 1 ] );
								file_status_pair.set_ByRef( pidstr.Split( ':' ) );
								if( is_killed == 0 && file_status_pair.Count() == 2 && file_status_pair[ 0 ].Equals( "Name" , 4 ) )
								{
									pidstr.set_ByRef( pidstr.Trim() );
									if( pidstr == process_name )
										is_killed = 1;
									else
										break;
								}
								else if( is_killed == 1 && file_status_pair.Count() == 2 && file_status_pair[ 0 ].Equals( "Pid" , 3 ) )
								{
									pidstr.set_ByRef( pidstr.Trim() );
									::rux::pid_t pid = pidstr.ToUInt32();
									if( ::kill( pid , 9 ) == -1 )
										is_killed = 0;
									else
										waitpid( pid , NULL , 0 );
									break;
								}
							}			
							::rux::engine::free_mem( lines );
						}
					}
				}
			}
#endif	
			return is_killed;
		};
		::rux::pid_t process::get_process_id( void )
		{
#ifdef __WINDOWS__
			return GetCurrentProcessId();
#else
			return getpid();
#endif
		};
		size_t process::get_max_descriptors( void )
		{
			size_t max_descriptors = 0;
#ifdef __UNIX__
			struct rlimit rlim;
			getrlimit( RLIMIT_NOFILE , &rlim );
			max_descriptors = rlim.rlim_cur;
#elif defined( __WINDOWS__ )
			max_descriptors = _getmaxstdio();
#endif
			return max_descriptors;
		};
#ifdef __WINDOWS__
		::rux::uint64 process::get_thread_cpu_time( HANDLE thread )
#else
		::rux::uint64 process::get_thread_cpu_time( pthread_t* thread )
#endif
		{
#ifdef __UNIX__
			pthread_t pthread;
#endif
			if( thread == NULL )
#ifdef __WINDOWS__
				thread = GetCurrentThread();
#else
			{
				pthread = pthread_self();
				thread = &pthread;
			}
#endif
			::rux::uint64 cpu = 0ULL;				
#ifdef __WINDOWS__
			FILETIME a , b , c , d;
			if( GetThreadTimes( thread , &a , &b , &c , &d ) )
				cpu = ( d.dwLowDateTime | ( (::rux::uint64)d.dwHighDateTime << 32 ) ) / 10000ULL;
#else
			clockid_t cid;
			if( pthread_getcpuclockid( *thread , &cid ) == 0 )
			{
				struct timespec ts;
				if( clock_gettime( cid , &ts ) == 0 )
					cpu = ts.tv_sec * 1000ULL + ( ts.tv_nsec / 1000000ULL );
			}
#endif
			return cpu;
		};
		float process::get_process_cpu_time( void )
		{
#ifdef __WINDOWS__
			FILETIME a , b , c , d;
			if( GetProcessTimes( GetCurrentProcess() , &a , &b , &c , &d ) != 0 )
				return (float)( d.dwLowDateTime | ( (::rux::uint64)d.dwHighDateTime << 32 ) ) * 0.0000001f;
			else
				return 0;
#else
			return (float)clock() / CLOCKS_PER_SEC;
#endif
		};
		bool process::executing( const char* module_name , ::rux::pid_t pid )
		{
			if( ::rux::diagnostics::process_info::pidof( module_name , pid ) == pid )
				return true;
			else
				return false;
		};
		bool process::executing( const char* module_name , ::rux::pid_t* pid , ::rux::pid_t except_pid )
		{
			*pid = 0;
			bool is_executed = false;
			*pid = ::rux::diagnostics::process_info::pidof( module_name , 0 , except_pid );
			if( *pid != 0 )
				is_executed = true;
			return is_executed;
		};
#ifdef __WINDOWS__
		typedef BOOL (*GetProductInfo_t)(DWORD, DWORD, DWORD, DWORD, PDWORD);
		typedef void (*GetNativeSystemInfo_t)( LPSYSTEM_INFO lpSystemInfo );
		typedef void (*GetSystemInfo_t)( LPSYSTEM_INFO lpSystemInfo );
		typedef BOOL (*GetVersionExA_t)( LPOSVERSIONINFOA lpVersionInformation );
#endif		
		bool os::get_os( char* os_string , size_t os_string_size , char* error_string , size_t error_string_size )
		{
			os_string[ 0 ]  = 0;
			if( error_string )
				error_string[ 0 ] = 0;
			bool res = false;
#ifdef __WINDOWS__
			SYSTEM_INFO system_info;	
			memset( &system_info , 0 , sizeof( SYSTEM_INFO ) );			
			HMODULE module_handle = LoadLibraryA( "kernel32.dll" );
			if( module_handle )
			{
				GetNativeSystemInfo_t get_native_system_info = (GetNativeSystemInfo_t)dlsym( module_handle , "GetNativeSystemInfo" );
				if( get_native_system_info == NULL )
				{					
					GetSystemInfo_t get_system_info = (GetSystemInfo_t)dlsym( module_handle , "GetSystemInfo" );
					if( get_system_info )
					{
						get_system_info( &system_info );
						res = true;
					}
				}
				else
				{
					get_native_system_info( &system_info );
					res = true;
				}
				if( res )
				{
					res = false;
					OSVERSIONINFOEX os_version_info_ex;
					memset( &os_version_info_ex , 0 , sizeof( OSVERSIONINFOEX ) );
					os_version_info_ex.dwOSVersionInfoSize = sizeof( OSVERSIONINFOEX );		
					GetVersionExA_t get_version_exA = (GetVersionExA_t)dlsym( module_handle , "GetVersionExA" );
					if( get_version_exA && get_version_exA( (OSVERSIONINFO*)&os_version_info_ex ) )
					{
						if( VER_PLATFORM_WIN32_NT == os_version_info_ex.dwPlatformId && os_version_info_ex.dwMajorVersion > 4 )
						{
							res = true;
							if ( os_version_info_ex.dwMajorVersion == 6 )
							{
								if( os_version_info_ex.dwMinorVersion == 0 )
								{
									if( os_version_info_ex.wProductType == VER_NT_WORKSTATION )
										::rux::safe_strncat( os_string , "Windows Vista " , os_string_size );
									else 
										::rux::safe_strncat( os_string , "Windows Server 2008 " , os_string_size );
								}
								else if ( os_version_info_ex.dwMinorVersion == 1 || os_version_info_ex.dwMinorVersion == 2 )
								{
									if( os_version_info_ex.wProductType == VER_NT_WORKSTATION && os_version_info_ex.dwMinorVersion == 1 )
										::rux::safe_strncat( os_string , "Windows 7 " , os_string_size );
									else if( os_version_info_ex.wProductType == VER_NT_WORKSTATION && os_version_info_ex.dwMinorVersion == 2 )
										::rux::safe_strncat( os_string , "Windows 8 " , os_string_size );
									else 
										::rux::safe_strncat( os_string , "Windows Server 2008 R2 " , os_string_size );
								}
								GetProductInfo_t get_product_info = (GetProductInfo_t)dlsym( module_handle , "GetProductInfo" );
								DWORD product_type = 0;
								get_product_info( os_version_info_ex.dwMajorVersion , os_version_info_ex.dwMinorVersion, 0 , 0 , &product_type );
								switch( product_type )
								{
								case PRODUCT_ULTIMATE:
									::rux::safe_strncat( os_string , "Ultimate Edition" , os_string_size );
									break;
								case PRODUCT_PROFESSIONAL:
									::rux::safe_strncat( os_string , "Professional" , os_string_size );
									break;
								case PRODUCT_HOME_PREMIUM:
									::rux::safe_strncat( os_string , "Home Premium Edition" , os_string_size );
									break;
								case PRODUCT_HOME_BASIC:
									::rux::safe_strncat( os_string , "Home Basic Edition" , os_string_size );
									break;
								case PRODUCT_ENTERPRISE:
									::rux::safe_strncat( os_string , "Enterprise Edition" , os_string_size );
									break;
								case PRODUCT_BUSINESS:
									::rux::safe_strncat( os_string , "Business Edition" , os_string_size );
									break;
								case PRODUCT_STARTER:
									::rux::safe_strncat( os_string , "Starter Edition" , os_string_size );
									break;
								case PRODUCT_CLUSTER_SERVER:
									::rux::safe_strncat( os_string , "Cluster Server Edition" , os_string_size );
									break;
								case PRODUCT_DATACENTER_SERVER:
									::rux::safe_strncat( os_string , "Datacenter Edition" , os_string_size );
									break;
								case PRODUCT_DATACENTER_SERVER_CORE:
									::rux::safe_strncat( os_string , "Datacenter Edition (core installation)" , os_string_size );
									break;
								case PRODUCT_ENTERPRISE_SERVER:
									::rux::safe_strncat( os_string , "Enterprise Edition" , os_string_size );
									break;
								case PRODUCT_ENTERPRISE_SERVER_CORE:
									::rux::safe_strncat( os_string , "Enterprise Edition (core installation)" , os_string_size );
									break;
								case PRODUCT_ENTERPRISE_SERVER_IA64:
									::rux::safe_strncat( os_string , "Enterprise Edition for Itanium-based Systems" , os_string_size );
									break;
								case PRODUCT_SMALLBUSINESS_SERVER:
									::rux::safe_strncat( os_string , "Small Business Server" , os_string_size );
									break;
								case PRODUCT_SMALLBUSINESS_SERVER_PREMIUM:
									::rux::safe_strncat( os_string , "Small Business Server Premium Edition" , os_string_size );
									break;
								case PRODUCT_STANDARD_SERVER:
									::rux::safe_strncat( os_string , "Standard Edition" , os_string_size );
									break;
								case PRODUCT_STANDARD_SERVER_CORE:
									::rux::safe_strncat( os_string , "Standard Edition (core installation)" , os_string_size );
									break;
								case PRODUCT_WEB_SERVER:
									::rux::safe_strncat( os_string , "Web Server Edition" , os_string_size );
									break;
								}
								if( system_info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL ) 
									::rux::safe_strncat( os_string , ", x86" , os_string_size );
								else if( system_info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64 ) 
									::rux::safe_strncat( os_string , ", x64 IA" , os_string_size );
								else if( system_info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 ) 
									::rux::safe_strncat( os_string , ", x64 AMD" , os_string_size );
							}
							else if( os_version_info_ex.dwMajorVersion == 5 && os_version_info_ex.dwMinorVersion == 2 )
							{
								if( GetSystemMetrics( SM_SERVERR2 ) )
										::rux::safe_strncat( os_string , "Windows Server 2003 R2" , os_string_size );
								 else if ( os_version_info_ex.wSuiteMask & VER_SUITE_STORAGE_SERVER )
									::rux::safe_strncat( os_string , "Windows Storage Server 2003" , os_string_size );
								 else if ( os_version_info_ex.wSuiteMask & VER_SUITE_WH_SERVER )
									::rux::safe_strncat( os_string , "Windows Home Server" , os_string_size );
								else if( os_version_info_ex.wProductType == VER_NT_WORKSTATION && system_info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
								{
									::rux::safe_strncat( os_string , "Windows XP Professional" , os_string_size );				
									::rux::safe_strncat( os_string , ", x64 AMD" , os_string_size );
								}
								else
									::rux::safe_strncat( os_string , "Windows Server 2003" , os_string_size );
								if ( os_version_info_ex.wProductType != VER_NT_WORKSTATION )
								 {									
									if( os_version_info_ex.wSuiteMask & VER_SUITE_COMPUTE_SERVER )
										::rux::safe_strncat( os_string , " Compute Cluster Edition" , os_string_size );
									else if( os_version_info_ex.wSuiteMask & VER_SUITE_DATACENTER )
										::rux::safe_strncat( os_string , " Datacenter Edition" , os_string_size );
									else if( os_version_info_ex.wSuiteMask & VER_SUITE_ENTERPRISE )
										::rux::safe_strncat( os_string , " Enterprise Edition" , os_string_size );
									else if( os_version_info_ex.wSuiteMask & VER_SUITE_BLADE )
										::rux::safe_strncat( os_string , " Web Edition" , os_string_size );
									else 
										::rux::safe_strncat( os_string , " Standard Edition" , os_string_size );						 
									if( system_info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL ) 
										::rux::safe_strncat( os_string , ", x86" , os_string_size );
									else if( system_info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64 ) 
										::rux::safe_strncat( os_string , ", x64 IA" , os_string_size );
									else if( system_info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 ) 
										::rux::safe_strncat( os_string , ", x64 AMD" , os_string_size );
								}
							}
							else if( os_version_info_ex.dwMajorVersion == 5 && os_version_info_ex.dwMinorVersion == 1 )
							{
								::rux::safe_strncat( os_string , "Windows XP" , os_string_size );
								if( os_version_info_ex.wSuiteMask & VER_SUITE_PERSONAL )
									::rux::safe_strncat( os_string , " Home Edition" , os_string_size );
								else 
										::rux::safe_strncat( os_string , " Professional" , os_string_size );
								::rux::safe_strncat( os_string , ", x86" , os_string_size );
							}
							else if( os_version_info_ex.dwMajorVersion == 5 && os_version_info_ex.dwMinorVersion == 0 )
							{
								::rux::safe_strncat( os_string , "Windows 2000" , os_string_size );
								if ( os_version_info_ex.wProductType == VER_NT_WORKSTATION )
									::rux::safe_strncat( os_string , " Professional" , os_string_size );
								else 
								{
									if( os_version_info_ex.wSuiteMask & VER_SUITE_DATACENTER )
										::rux::safe_strncat( os_string , " Datacenter Server" , os_string_size );
									else if( os_version_info_ex.wSuiteMask & VER_SUITE_ENTERPRISE )
										::rux::safe_strncat( os_string , " Advanced Server" , os_string_size );
									else 
										::rux::safe_strncat( os_string , " Server" , os_string_size );
								}
								::rux::safe_strncat( os_string , ", x86" , os_string_size );								
							}
						}
					}
				}
				FreeLibrary( module_handle );
			}
#else
			::rux::memory::stack_chunk_t chunk( (::rux::byte*)os_string , os_string_size , ::rux::memory::string_type );
			char* args[] =
			{
				(char*)"-ds"
			};		
			declare_stack_variable( char , my_error_string , 1024 );
			if( error_string == 0 )
				error_string = my_error_string;
			::rux::diagnostics::process::start_with_redirect( "lsb_release" , args , 1 , on_default_redirect_stdout_or_stderr_handler , on_default_redirect_stdout_or_stderr_handler , &chunk , 0 , 0 , error_string );
			::rux::log::WriteTrace( "%s, %s:%d, os '%s'" , __FUNCTION__ , __FILE__ , __LINE__ , os_string );
			if( error_string[ 0 ] != 0 )
				::rux::log::WriteError( "%s, %s:%d, error '%s'" , __FUNCTION__ , __FILE__ , __LINE__ , error_string );
			else
				res = true;
			if( chunk.actual_size() > 0 && os_string[ chunk.actual_size() - 1 ] == '\n' )
				chunk.reduce_actual_size_of( 1 );
			if( chunk.actual_size() > 1 && os_string[ chunk.actual_size() - 2 ] == '\n' )
				chunk.reduce_actual_size_of( 2 );
			if( res )
			{
				chunk.concatenate( (::rux::byte*)", " , 2 );				
				char* args[] =
				{
					(char*)"-m"
				};		
				error_string[ 0 ] = 0;
				::rux::diagnostics::process::start_with_redirect( "uname" , args , 1 , on_default_redirect_stdout_or_stderr_handler , on_default_redirect_stdout_or_stderr_handler , &chunk , 0 , 0 , error_string );
				if( error_string[ 0 ] != 0 )
					chunk.reduce_actual_size_of( 2 );
				else
				{
					if( chunk.actual_size() > 0 && os_string[ chunk.actual_size() - 1 ] == '\n' )
						chunk.reduce_actual_size_of( 1 );
					if( chunk.actual_size() > 1 && os_string[ chunk.actual_size() - 2 ] == '\n' )
						chunk.reduce_actual_size_of( 2 );
				}
			}
#endif
			if( os_string[ 0 ] == 0 )
				strcpy( os_string , "Unknown OS" );
			return res;
		};		
		void on_lines_count_redirect_stdout_or_stderr_handler( void* udata , const char* data , size_t data_size )
		{
			::rux::int64* lines_count = (::rux::int64*)udata;
			char* ptr = (char*)data;
			for( ; ; )
			{
				ptr = strstr( ptr , "\n" );
				if( ptr )
				{
					(*lines_count)++;
					ptr++;
				}
				else
					break;
			}
		};
		void on_default_redirect_stdout_or_stderr_handler( void* udata , const char* data , size_t data_size )
		{
			::rux::memory::chunk_t* chunk = (::rux::memory::chunk_t*)udata;
			if( data_size > 0 )
				chunk->concatenate( (void*)data , data_size );
		};
		void module::get_current_module_directory( char* module_filename , size_t module_filename_size )
		{
			::rux::diagnostics::module::get_current_module_filename( module_filename , module_filename_size );
			::rux::int64 last_index = ::rux::string::last_index_of( module_filename , "/" );
			if( last_index == -1 )
				last_index = ::rux::string::last_index_of( module_filename , "\\" );
			if( last_index != -1 )
				module_filename[ last_index ] = 0;
			::rux::string::replace_all( module_filename , '\\' , '/' );
		};
		void module::get_current_executable_directory( char* module_filename , size_t module_filename_size )
		{
			::rux::diagnostics::module::get_current_executable_filename( module_filename , module_filename_size );
			::rux::int64 last_index = ::rux::string::last_index_of( module_filename , "/" );
			if( last_index == -1 )
				last_index = ::rux::string::last_index_of( module_filename , "\\" );
			if( last_index != -1 )
				module_filename[ last_index ] = 0;
			::rux::string::replace_all( module_filename , '\\' , '/' );
		};
		void module::get_current_executable_filename( char* module_filename , size_t module_filename_size )
		{
			module_filename[ 0 ] = 0;
#ifdef __WINDOWS__
			HMODULE module_handle = GetModuleHandleA( NULL );
			if( module_handle )
			{
				if( GetModuleFileNameA( module_handle , module_filename , module_filename_size ) != 0 )
				{
				}				
			}
#elif defined( __UNIX__ )
#ifdef __LINUX__
			if( readlink( "/proc/self/exe" , module_filename , module_filename_size ) > 0 )
#elif defined( __SOLARIS__ )
			char key_ptr[ 64 ] = {0};
			::rux::uint32 pid = getpid() , number = 0 , offset = 0;
			const char* const_string = NULL;
			const_string = "/proc/";
			::memcpy( key_ptr , const_string , 6 );
			offset += 6;
			rux::string::uint32_to_string( pid , &key_ptr[ offset ] );
			offset = strlen( key_ptr );
			const_string = "/path/a.out";
			::memcpy( &key_ptr[ offset ] , const_string , 11 );
			offset += 11;
			key_ptr[ offset ] = 0;
			if( readlink( key_ptr , module_filename , module_filename_size ) > 0 )
#endif
			{
			}
#endif
		};
		void module::get_current_module_filename( char* module_filename , size_t module_filename_size )
		{
			module_filename[ 0 ] = 0;
#ifdef __WINDOWS__
			HMODULE module_handle = GetModuleHandleA( g_current_module_name );
			if( module_handle )
			{
				if( GetModuleFileNameA( module_handle , module_filename , module_filename_size ) != 0 )
				{	
				}				
			}	
#elif defined( __UNIX__ )
			Dl_info info;
			if( dladdr( (void*)g_current_module_name , &info ) != 0 )
				::rux::safe_strncat( module_filename , info.dli_fname , module_filename_size );
			else
			{
#ifdef __LINUX__
				if( readlink( "/proc/self/exe" , module_filename , module_filename_size ) > 0 )
#elif defined( __SOLARIS__ )
				declare_stack_variable( char , key_ptr , 64 );
				::rux::safe_sprintf( key_ptr , 64 , "/proc/%ld/path/a.out" , getpid() );
				if( readlink( key_ptr , module_filename , module_filename_size ) > 0 )
#endif
				{			
				}	
			}	
#endif
		};
		bool os::restart( char error_string[ 1024 ] )
		{
			error_string[ 0 ] = 0;
			bool restarted = false;
#ifdef __WINDOWS__
			HANDLE token = NULL; 			
			if ( OpenProcessToken( GetCurrentProcess() , TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY , &token ) != 0 ) 
			{
				TOKEN_PRIVILEGES token_privileges;
				if( LookupPrivilegeValue( NULL , SE_SHUTDOWN_NAME , &token_privileges.Privileges[ 0 ].Luid ) != 0 )
				{
					token_privileges.PrivilegeCount = 1;
					token_privileges.Privileges[ 0 ].Attributes = SE_PRIVILEGE_ENABLED; 					
					if( AdjustTokenPrivileges( token , FALSE , &token_privileges , 0 , (PTOKEN_PRIVILEGES)NULL , 0 ) != 0 )
					{
						if( InitiateSystemShutdownEx( NULL , NULL , 0 , TRUE , TRUE , SHTDN_REASON_MAJOR_APPLICATION | SHTDN_REASON_MINOR_MAINTENANCE | SHTDN_REASON_FLAG_PLANNED ) )
							restarted = true;
						else
							XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError , error_string );
					}
					else
						XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError , error_string );
				}
				else
					XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError , error_string );
				CloseHandle( token );
			}
			else
				XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError , error_string );
#else			
			::rux::memory::heap_chunk_t chunk( ::rux::memory::string_type );
			char* args[] =
			{
				(char*)"-r" ,
				(char*)"now"
			};		
			::rux::int32 res = ::rux::diagnostics::process::start_with_redirect( "shutdown" , args , 2 , ::rux::diagnostics::on_default_redirect_stdout_or_stderr_handler , ::rux::diagnostics::on_default_redirect_stdout_or_stderr_handler , &chunk , 0 , 0 , error_string );
			if( chunk.actual_size() )
			{
				if( res != 0 )
				{
					size_t len = strlen( (char*)chunk._data );
					if( len > 1023 )
					{
						::rux::engine::memcpy( error_string , 0 , chunk._data , 1023 );
						error_string[ 1023 ] = 0;
					}
					else
						strcpy( error_string , (char*)chunk._data );
				}
			}
			if( res != 0 )
			{
				chunk.clear();
				res = ::rux::diagnostics::process::start_with_redirect( "reboot" , NULL , 0 , ::rux::diagnostics::on_default_redirect_stdout_or_stderr_handler , ::rux::diagnostics::on_default_redirect_stdout_or_stderr_handler , &chunk , 0 , 0 , error_string );
				if( chunk.actual_size() )
				{
					if( res != 0 )
					{
						size_t len = strlen( (char*)chunk._data );
						if( len > 1023 )
						{
							::rux::engine::memcpy( error_string , 0 , chunk._data , 1023 );
							error_string[ 1023 ] = 0;
						}
						else
							strcpy( error_string , (char*)chunk._data );
					}
				}
			}
			restarted = res == 0;
#endif
			return restarted;
		};
		::rux::int32 process::start_with_redirect( const char* executable , const char* arguments , process_stdout_or_stderr_t on_process_stdout , process_stdout_or_stderr_t on_process_stderr , void* udata , char** env , size_t env_size , char error_string[ 1024 ] )
		{
			size_t arguments_size = 0;
			char* arguments_array[ 256 ] = {0};
			size_t len = 0;
			if( arguments && ( len = strlen( arguments ) ) > 0 )
			{
				char arguments_copy[ 4096 ] = {0};
				if( len < 4096 )
				{
					strcpy( arguments_copy , arguments );
					char* begin = arguments_copy;
					char* ptr = begin;					
					do
					{
						ptr = strpbrk( ptr , " \n\r\t\"" );
						if( ptr )
						{
							if( ptr - begin > 1 )
							{
								arguments_array[ arguments_size ] = begin;
								arguments_size++;
							}
							if( *ptr == '"' )
							{
								*ptr = 0;
								ptr++;
								begin = ptr;
								ptr = strstr( ptr , "\"" );
								if( ptr )
								{
									if( ptr - begin > 1 )
									{
										arguments_array[ arguments_size ] = begin;
										arguments_size++;
									}
									*ptr = 0;
									ptr++;
									begin = ptr;
								}
							}
							else
							{
								*ptr = 0;
								ptr++;
								begin = ptr;
							}
						}
						else
							break;
					}
					while( ptr );
					if( begin < arguments_copy + len )
					{
						arguments_array[ arguments_size ] = begin;
						arguments_size++;
					}
				}
				else if( error_string )
					strcpy( error_string , "arguments string is too long" );
			}
			return ::rux::diagnostics::process::start_with_redirect( executable , arguments_array , arguments_size , on_process_stdout , on_process_stderr , udata , env , env_size , error_string );
		};		
		::rux::int32 process::start_with_redirect( const char* executable , char** arguments , size_t arguments_size , process_stdout_or_stderr_t on_process_stdout , process_stdout_or_stderr_t on_process_stderr , void* udata , char** env , size_t env_size , char error_string[ 1024 ] )
		{
			if( error_string )
				error_string[ 0 ] = 0;
			::rux::int32 res = 1;
			if( executable && executable[ 0 ] != 0 )
			{
				char module_name[ 4096 ] = {0};
				if( ::rux::io::file::exists( executable , 1 ) == 0 )
				{
					for( size_t index0 = 0 ; index0 < ::rux::engine::_globals->_env_variables_count ; index0++ )
					{
						::rux::safe_strncpy( module_name , ::rux::engine::_globals->_env_variables[ index0 ] , 4096 );
						::rux::string::replace_all( module_name , '\\' , '/' );
						size_t len = strlen( module_name );
						module_name[ len ] = '/';
						module_name[ len + 1 ] = 0;
						::rux::safe_strncat( module_name , executable , 4096 );
						if( ::rux::io::file::exists( module_name , 1 ) == 1 )
							break;
					}
				}
				else
					::rux::safe_strncpy( module_name , executable , 4096 );
				if( ::rux::io::file::exists( module_name , 1 ) == 1 )
				{				
#ifdef __WINDOWS__			
					char cmd_line[ 4096 ] = {0};
					::rux::safe_strncpy( cmd_line , module_name , 4096 );
					if( arguments )
					{
						for( size_t index0 = 0 ; index0 < arguments_size ; index0++ )
						{
							if( strstr( arguments[ index0 ] , " " ) )
							{
								strcat( cmd_line , " \"" );
								strcat( cmd_line , arguments[ index0 ] );							
								strcat( cmd_line , "\"" );
							}
							else
							{
								strcat( cmd_line , " " );
								strcat( cmd_line , arguments[ index0 ] );							
							}
						}
					}
					SECURITY_ATTRIBUTES sa; 
					sa.nLength = sizeof( SECURITY_ATTRIBUTES ); 
					sa.bInheritHandle = TRUE; 
					OSVERSIONINFO osv;
					osv.dwOSVersionInfoSize = sizeof( osv );
					GetVersionEx( &osv );
					if( osv.dwPlatformId == VER_PLATFORM_WIN32_NT )
					{
						SECURITY_DESCRIPTOR sd;
						InitializeSecurityDescriptor( &sd , SECURITY_DESCRIPTOR_REVISION );
						SetSecurityDescriptorDacl( &sd , true , NULL , false );
						sa.lpSecurityDescriptor = &sd;
					}
					else
						sa.lpSecurityDescriptor = NULL; 

					STARTUPINFOA startup_info;
					memset( (void*)&startup_info , 0 , sizeof( STARTUPINFOA ) );			
					startup_info.cb = sizeof( STARTUPINFOA );
					startup_info.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
					startup_info.wShowWindow = SW_HIDE;					

					HANDLE read_stdout_pipe = NULL;
					HANDLE pipe = NULL;
					HANDLE process_handle = GetCurrentProcess();
					if( CreatePipe( &read_stdout_pipe , &pipe , &sa , 0 ) )
					{
						if( DuplicateHandle( process_handle , pipe , process_handle , &startup_info.hStdOutput , 
							0 , TRUE , DUPLICATE_SAME_ACCESS | DUPLICATE_CLOSE_SOURCE ) )
						{
							HANDLE read_stderr_pipe = NULL;
							if( CreatePipe( &read_stderr_pipe , &pipe , &sa , 0 ) )
							{
								if( DuplicateHandle( process_handle , pipe , process_handle , &startup_info.hStdError , 
									0, TRUE, DUPLICATE_SAME_ACCESS | DUPLICATE_CLOSE_SOURCE ) )
								{
									HANDLE write_stdin_pipe = NULL;
									if( CreatePipe( &pipe , &write_stdin_pipe , &sa , 0 ) )
									{
										if( DuplicateHandle( process_handle , pipe , process_handle , &startup_info.hStdInput , 
											0, TRUE, DUPLICATE_SAME_ACCESS | DUPLICATE_CLOSE_SOURCE ) )
										{								
											PROCESS_INFORMATION  process_information;
											memset( (void*)&process_information , 0 , sizeof( PROCESS_INFORMATION ) );															
											unsigned long _result = 1;			

											char* environment = 0;
											XData heap_environment_string;
											if( env != 0 && env_size != 0 )
											{
												for( size_t process_env_index = 0 ; process_env_index < env_size ; process_env_index++ )
													heap_environment_string.Concatenate( env[ process_env_index ] , strlen( env[ process_env_index ] ) + 1 , __FILE__ , __LINE__ );
												char separator = '\0';
												heap_environment_string.Concatenate( &separator , 1 , __FILE__ , __LINE__ );
												environment = cpp_ptr_cast( char* , heap_environment_string._data );
											}
											if( CreateProcessA( NULL , cmd_line , NULL , NULL , TRUE , CREATE_NEW_CONSOLE , environment , NULL , &startup_info , &process_information ) == 0 )				
											{								
												if( error_string )
													XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError , error_string );
											}
											else
											{
												if( startup_info.hStdInput )
												{
													CloseHandle( startup_info.hStdInput );
													startup_info.hStdInput = NULL;
												}
												if( startup_info.hStdOutput )
												{
													CloseHandle( startup_info.hStdOutput );
													startup_info.hStdOutput = NULL;
												}
												if( startup_info.hStdError )
												{
													CloseHandle( startup_info.hStdError );
													startup_info.hStdError = NULL;
												}
												::rux::uint8 exited = 0;
												declare_stack_variable( char , result , 4096 );
												DWORD peek_read = 0 , peek_avail = 0;									
												for( ; ; ) 
												{			
													peek_read = 0;
													::rux::uint8 readed = 0;
													PeekNamedPipe( read_stdout_pipe , result , 4095 , &peek_read , &peek_avail ,NULL );
													if( peek_read != 0 )
														ReadFile( read_stdout_pipe , result , 4095 , &peek_read , NULL );
													if(	peek_read > 0 )
													{
														readed = 1;
														result[ peek_read ] = 0;
														if( on_process_stdout )
															on_process_stdout( udata , result , peek_read );
													}
													peek_read = 0;
													PeekNamedPipe( read_stderr_pipe , result , 4095 , &peek_read , &peek_avail ,NULL );
													if( peek_read != 0 )
														ReadFile( read_stderr_pipe , result , 4095 , &peek_read , NULL );
													if(	peek_read > 0 )
													{
														readed = 1;
														result[ peek_read ] = 0;
														if( on_process_stderr )
															on_process_stderr( udata , result , peek_read );
													}
													if( readed == 0 
														&& exited == 1 )
														break;
													if( WaitForSingleObject( process_information.hProcess , 1 ) == WAIT_OBJECT_0 )
														exited = 1;
												}																	
												_result = private_WaitForProcess( process_information.hProcess );
												if( ERROR_SUCCESS == _result )
												{
													GetExitCodeProcess( process_information.hProcess, &_result );
													res = _result;
												}
											}
											if( process_information.hProcess )
												CloseHandle( process_information.hProcess );
											if( process_information.hThread )
												CloseHandle( process_information.hThread );
											if( startup_info.hStdInput ) 
											{
												CloseHandle( startup_info.hStdInput );
												startup_info.hStdInput = NULL;
											}
										}
										else
										{
											CloseHandle( pipe );
											if( error_string )
												XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError , error_string );
										}
										if( write_stdin_pipe )
										{
											CloseHandle( write_stdin_pipe );
											write_stdin_pipe = NULL;
										}
									}
									else if( error_string )
										XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError , error_string );
									if( startup_info.hStdError ) 
									{
										CloseHandle( startup_info.hStdError );
										startup_info.hStdError = NULL;
									}
								}
								else
								{
									CloseHandle( pipe );
									if( error_string )
										XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError , error_string );
								}
								if( read_stderr_pipe )
								{
									CloseHandle( read_stderr_pipe );
									read_stderr_pipe = NULL;
								}	
							}
							else if( error_string )
								XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError , error_string );
							if( startup_info.hStdOutput ) 
							{
								CloseHandle( startup_info.hStdOutput );
								startup_info.hStdOutput = NULL;
							}
						}
						else
						{
							CloseHandle( pipe );
							if( error_string )
								XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError , error_string );
						}
						if( read_stdout_pipe )
						{
							CloseHandle( read_stdout_pipe );
							read_stdout_pipe = NULL;
						}
					}
					else if( error_string )
						XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError , error_string );
#else
					::rux::int32 stdout_pipes[ 2 ] = {0};
					if( pipe( stdout_pipes ) == 0 )
					{
						::rux::int32 stderr_pipes[ 2 ] = {0};
						if( pipe( stderr_pipes ) == 0 )
						{
							::rux::int32 stdin_pipes[ 2 ] = {0};
							if( pipe( stdin_pipes ) == 0 )
							{
								::rux::pid_t child_pid = fork();
								if( child_pid < 0 )
									::rux::XConsole::WriteToStdErr( "\fork failed\n" );
								else if( child_pid == 0 )
								{				
									close( stdin_pipes[ 1 ] );
									close( stderr_pipes[ 0 ] );
									close( stdout_pipes[ 0 ] );				
									dup2( stdin_pipes[ 0 ] , STDIN_FILENO );
									dup2( stdout_pipes[ 1 ] , STDOUT_FILENO );				
									dup2( stderr_pipes[ 1 ] , STDERR_FILENO );
									close( stdout_pipes[ 1 ] );
									close( stderr_pipes[ 1 ] );
									close( stdin_pipes[ 0 ] );
									char* process_env[ 512 ] = {0};
									process_env[ 0 ] = 0;
									if( env != 0 && env_size != 0 )
									{
										for( size_t process_env_index = 0 ; process_env_index < env_size ; process_env_index++ )
											process_env[ process_env_index ] = env[ process_env_index ];
										process_env[ env_size ] = 0;
									}
									if( arguments_size == 0 )
									{
										if( process_env[ 0 ] == 0 )
											execl( module_name , module_name , (char*)0 );
										else
											execle( module_name , module_name , (char*)0 , process_env );
									}
									else if( arguments_size == 1 )
									{
										if( process_env[ 0 ] == 0 )
											execl( module_name , module_name , arguments[ 0 ] , (char*)0 );
										else
											execle( module_name , module_name , arguments[ 0 ] , (char*)0 , process_env );
									}
									else if( arguments_size == 2 )
									{
										if( process_env[ 0 ] == 0 )
											execl( module_name , module_name , arguments[ 0 ] , arguments[ 1 ] , (char*)0 );
										else
											execle( module_name , module_name , arguments[ 0 ] , arguments[ 1 ] , (char*)0 , process_env );
									}
									else if( arguments_size == 3 )
									{
										if( process_env[ 0 ] == 0 )
											execl( module_name , module_name , arguments[ 0 ] , arguments[ 1 ] , arguments[ 2 ] , (char*)0 );
										else
											execle( module_name , module_name , arguments[ 0 ] , arguments[ 1 ] , arguments[ 2 ] , (char*)0 , process_env );
									}
									else if( arguments_size == 4 )
									{
										if( process_env[ 0 ] == 0 )
											execl( module_name , module_name , arguments[ 0 ] , arguments[ 1 ] , arguments[ 2 ] , arguments[ 3 ] , (char*)0 );
										else
											execle( module_name , module_name , arguments[ 0 ] , arguments[ 1 ] , arguments[ 2 ] , arguments[ 3 ] , (char*)0 , process_env );
									}
									else if( arguments_size == 5 )
									{
										if( process_env[ 0 ] == 0 )
											execl( module_name , module_name , arguments[ 0 ] , arguments[ 1 ] , arguments[ 2 ] , arguments[ 3 ] , arguments[ 4 ] , (char*)0 );
										else
											execle( module_name , module_name , arguments[ 0 ] , arguments[ 1 ] , arguments[ 2 ] , arguments[ 3 ] , arguments[ 4 ] , (char*)0 , process_env );
									}
									else if( arguments_size == 6 )
									{
										if( process_env[ 0 ] == 0 )
											execl( module_name , module_name , arguments[ 0 ] , arguments[ 1 ] , arguments[ 2 ] , arguments[ 3 ] , arguments[ 4 ] , arguments[ 5 ] , (char*)0 );
										else
											execle( module_name , module_name , arguments[ 0 ] , arguments[ 1 ] , arguments[ 2 ] , arguments[ 3 ] , arguments[ 4 ] , arguments[ 5 ] , (char*)0 , process_env );
									}
									else if( arguments_size == 7 )
									{
										if( process_env[ 0 ] == 0 )
											execl( module_name , module_name , arguments[ 0 ] , arguments[ 1 ] , arguments[ 2 ] , arguments[ 3 ] , arguments[ 4 ] , arguments[ 5 ] , arguments[ 6 ] , (char*)0 );
										else
											execle( module_name , module_name , arguments[ 0 ] , arguments[ 1 ] , arguments[ 2 ] , arguments[ 3 ] , arguments[ 4 ] , arguments[ 5 ] , arguments[ 6 ] , (char*)0 , process_env );
									}
									else if( arguments_size == 8 )
									{
										if( process_env[ 0 ] == 0 )
											execl( module_name , module_name , arguments[ 0 ] , arguments[ 1 ] , arguments[ 2 ] , arguments[ 3 ] , arguments[ 4 ] , arguments[ 5 ] , arguments[ 6 ] , arguments[ 7 ] , (char*)0 );
										else
											execle( module_name , module_name , arguments[ 0 ] , arguments[ 1 ] , arguments[ 2 ] , arguments[ 3 ] , arguments[ 4 ] , arguments[ 5 ] , arguments[ 6 ] , arguments[ 7 ] , (char*)0 , process_env );
									}
									else if( arguments_size == 9 )
									{
										if( process_env[ 0 ] == 0 )
											execl( module_name , module_name , arguments[ 0 ] , arguments[ 1 ] , arguments[ 2 ] , arguments[ 3 ] , arguments[ 4 ] , arguments[ 5 ] , arguments[ 6 ] , arguments[ 7 ] , arguments[ 8 ] , (char*)0 );
										else
											execle( module_name , module_name , arguments[ 0 ] , arguments[ 1 ] , arguments[ 2 ] , arguments[ 3 ] , arguments[ 4 ] , arguments[ 5 ] , arguments[ 6 ] , arguments[ 7 ] , arguments[ 8 ] , (char*)0 , process_env );
									}
									else if( arguments_size == 10 )
									{
										if( process_env[ 0 ] == 0 )
											execl( module_name , module_name , arguments[ 0 ] , arguments[ 1 ] , arguments[ 2 ] , arguments[ 3 ] , arguments[ 4 ] , arguments[ 5 ] , arguments[ 6 ] , arguments[ 7 ] , arguments[ 8 ] , arguments[ 9 ] , (char*)0 );
										else
											execle( module_name , module_name , arguments[ 0 ] , arguments[ 1 ] , arguments[ 2 ] , arguments[ 3 ] , arguments[ 4 ] , arguments[ 5 ] , arguments[ 6 ] , arguments[ 7 ] , arguments[ 8 ] , arguments[ 9 ] , (char*)0 , process_env );
									}
									else
									{
										char** zero_arguments = alloc_array_macros( char* , arguments_size + 2 );
										zero_arguments[ 0 ] = module_name;
										for( size_t index0 = 0 ; index0 < arguments_size ; index0++ )
											zero_arguments[ index0 + 1 ] = arguments[ index0 ];
										zero_arguments[ arguments_size + 1 ] = 0;
										if( process_env[ 0 ] == 0 )
											execv( module_name , zero_arguments );
										else
											execve( module_name , zero_arguments , process_env );
										::rux::engine::free_mem( zero_arguments );
									}
									_exit( 1 );
								}
								else if( child_pid != 0 )
								{
									close( stdin_pipes[ 0 ] );
									close( stderr_pipes[ 1 ] );
									close( stdout_pipes[ 1 ] );
									::rux::io::file_descriptor_waiter fd_waiter_stdout;
									fd_waiter_stdout.add( stdout_pipes[ 0 ] );
									::rux::io::file_descriptor_waiter fd_waiter_stderr;
									fd_waiter_stderr.add( stderr_pipes[ 0 ] );
									::rux::uint8 exited = 0 , readed = 0;
									declare_stack_variable( char , result , 4096 );
									for( ; ; )
									{	
										readed = 0;
										if( fd_waiter_stdout.wait( 10 , 1 , 0 ) )
										{
											if( fd_waiter_stdout.get_read_fd() != -1 )
											{
												::rux::int32 read_size = 0;
												do
												{											
													read_size = read( stdout_pipes[ 0 ] , result , 4095 ); 
													if( read_size > 0 )
													{
														readed = 1;
														result[ read_size ] = 0;
														if( on_process_stdout )
															on_process_stdout( udata , result , read_size );
													}
												}
												while( read_size != 0 && read_size != -1 );												
											}
										}
										if( fd_waiter_stderr.wait( 10 , 1 , 0 ) )
										{
											if( fd_waiter_stderr.get_read_fd() != -1 )
											{
												::rux::int32 read_size = 0;
												do
												{											
													read_size = read( stderr_pipes[ 0 ] , result , 4095 ); 
													if( read_size > 0 )
													{
														readed = 1;
														result[ read_size ] = 0;
														if( on_process_stderr )
															on_process_stderr( udata , result , read_size );
													}
												}
												while( read_size != 0 && read_size != -1 );												
											}
										}
										if( exited == 1 )
											break;
										pid_t waitpid_res = -1;
										if( ( waitpid_res = waitpid( child_pid , &res , WNOHANG ) ) == child_pid )
										{
											if( WIFEXITED( res ) || WIFSIGNALED( res ) )
												exited = 1;
										}
										else if( waitpid_res == -1 )
										{
											res = 0;
											exited = 1;
										}
									}
									close( stdin_pipes[ 1 ] );
									close( stderr_pipes[ 0 ] );
									close( stdout_pipes[ 0 ] );
									if( exited && WIFEXITED( res ) )
										res = WEXITSTATUS( res );
									else
										res = 1;
								}
							}
							else if( error_string )
								XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_Errno , error_string );
						}
						else if( error_string )
							XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_Errno , error_string );
					}	
					else if( error_string )
						XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_Errno , error_string );
#endif
				}
				else if( error_string )
					::rux::safe_sprintf( error_string , 1024 , "executable '%s' is not found" , executable );
			}
			else if( error_string )
				strcpy( error_string , "executable is empty" );			
			return res;
		};		
		::rux::uint8 process::kill( const char* module_name )
		{			
			::rux::uint8 is_killed = 0;
#ifdef __WINDOWS__
			::rux::diagnostics::process::adjust_token_privileges();
			DWORD processes[ 1024 ] = {0} , size = 0 , processes_count = 0;
			declare_stack_variable( char , alter_process_name , 1024 );
			rux_concatenation( alter_process_name , module_name );
			rux_replace( alter_process_name , '/' , '\\' );
			if( ::rux::engine::_globals->_psapi_module.EnumProcesses( processes , sizeof( processes ) , &size ) )
			{
				processes_count = size / sizeof( DWORD );
				declare_stack_variable( char , process_name_ptr , 1024 );
				declare_stack_variable( char , process_name_without_extension , 1024 );
				HANDLE process_handle = NULL;
				HMODULE module_handle = NULL;
				for( ::rux::uint32 index0 = 0 ; index0 < processes_count ; index0++ )
				{
					if( processes[ index0 ] != 0 )
					{
						process_handle = OpenProcess( SYNCHRONIZE | PROCESS_TERMINATE  | PROCESS_QUERY_INFORMATION | PROCESS_VM_READ , FALSE , processes[ index0 ] );
						if( process_handle != NULL )
						{					
							if( ::rux::engine::_globals->_psapi_module.EnumProcessModules( process_handle , &module_handle , sizeof( module_handle ) , &size ) )
							{						
								if( ::rux::engine::_globals->_psapi_module.GetModuleFileNameExA( process_handle , module_handle , process_name_ptr , 1024 ) > 0 )
								{
									if( strcmp( module_name , process_name_ptr ) == 0
										|| strcmp( alter_process_name , process_name_ptr ) == 0 )
									{
										if(TerminateProcess(process_handle, 1112))
										{
											WaitForSingleObject( process_handle , ::rux::threading::infinite );
											is_killed = 1;
										}
										else
										{
											CloseHandle( process_handle );
											break;
										}
									}	
								}
								if( ::rux::engine::_globals->_psapi_module.GetModuleBaseNameA( process_handle , module_handle , process_name_ptr , sizeof( process_name_ptr ) / sizeof( char ) ) > 0 )
								{
									::rux::io::path::get_filename_without_extension( process_name_ptr , process_name_without_extension );
									if( strcmp( module_name , process_name_ptr ) == 0
										|| strcmp( process_name_without_extension , module_name ) == 0 )
									{
										if(TerminateProcess(process_handle, 1112))
										{
											WaitForSingleObject( process_handle , ::rux::threading::infinite );
											is_killed = 1;
										}
										else
										{
											CloseHandle( process_handle );
											break;
										}
									}
								}						
							}					
							CloseHandle( process_handle );
						}				
					}
					if( is_killed == 1 )
						break;
				}
			}
#else
			::rux::XString directory_name , file_status_string;
			XArray< XString > file_status_lines , file_status_pair;
			XArray< XString > dirs;
			::rux::io::GetDirectories( "/proc" , dirs );
			::rux::uint32 index1 = 0;
			declare_stack_variable( char , fullpath , 1024 );
			::rux::XString alter_process_name , process_name( module_name );
			alter_process_name.set_ByRef( process_name.ReplaceAll( '\\' , '/' ) );
			for( ::rux::uint32 index0 = 0 ; index0 < dirs.Count() ; index0++ )
			{
				directory_name.set_ByRef( dirs[ index0 ] );
				file_status_string = directory_name + "/exe";		
				memset( fullpath , 0 , 1024 );
				readlink( file_status_string.str() , fullpath , 1024 );
				if( process_name == fullpath
					|| alter_process_name == fullpath )
				{
					file_status_pair.set_ByRef( directory_name.Split( '/' ) );
					if( file_status_pair.Count() > 1 )
					{
						::rux::XString pidstr;
						pidstr.set_ByRef( file_status_pair[ 1 ] );
						::rux::pid_t pid = pidstr.ToUInt32();
						if( ::kill( pid , 9 ) == -1 )
							is_killed = 0;
						else
						{
							waitpid( pid , NULL , 0 );
							is_killed = 1;
						}
					}
				}
				else
				{
					file_status_string = directory_name + "/status";
					file_status_string.set_ByRef( file_status_string.ConvertToUTF8() );
					if( ::rux::io::file::exists( file_status_string.str() ) == 1 )
					{
						size_t readen_size = 0;
						char* lines = (char*)rux::io::file::read_all( file_status_string.str() , readen_size );
						if( lines )
						{
							file_status_string = lines;
							file_status_lines.set_ByRef( file_status_string.Split( '\n' ) );
							for( index1 = 0 ; index1 < file_status_lines.Count() ; index1++ )
							{
								::rux::XString pidstr;
								pidstr.set_ByRef( file_status_pair[ 1 ] );
								file_status_pair.set_ByRef( pidstr.Split( ':' ) );
								if( is_killed == 0 && file_status_pair.Count() == 2 && file_status_pair[ 0 ].Equals( "Name" , 4 ) )
								{
									pidstr.set_ByRef( pidstr.Trim() );
									if( file_status_pair[ 1 ] == process_name )
										is_killed = 1;
									else
										break;
								}
								else if( is_killed == 1 && file_status_pair.Count() == 2 && file_status_pair[ 0 ].Equals( "Pid" , 3 ) )
								{
									pidstr.set_ByRef( pidstr.Trim() );
									::rux::pid_t pid = pidstr.ToUInt32();
									if( ::kill( pid , 9 ) == -1 )
										is_killed = 0;
									else
										waitpid( pid , NULL , 0 );
									break;
								}
							}			
							::rux::engine::free_mem( lines );
						}
					}
				}
				if( is_killed == 1 )
					break;
			}
#endif	
			return is_killed;
		};
		::rux::uint8 start_process( const char* module , const char* arguments )
		{			
			unsigned long result = 1;
			if( module && strlen( module ) > 0 )
			{
				::rux::XString module_name;
				if( rux_is_exists_file( module ) == 0 )
				{
					::rux::XString string( getenv( "Path" ) );
					if( string.Length() )
					{
						XArray< XString > paths;
						paths.set_ByRef( string.Split( ';' ) );
						for( ::rux::uint32 index0 = 0 ; index0 < paths.Count() ; index0++ )			
						{
							string.set_ByRef( paths[ index0 ] );
							string.set_ByRef( string.ReplaceAll( '\\' , '/' ) );
							if( string.get_UTF8Char( string.Length() - 1 ) == '/' )
								string.RemoveAt( string.Length() - 1 );
							string += "/";
							string += module;
							if( rux_is_exists_file( string.str() ) == 1 )
							{
								module_name.set_ByRef( string );
								break;
							}
						}
					}
				}
				else
					module_name = module;
				if( module_name.Length() > 0 )
				{			
#ifdef __WINDOWS__						
					::rux::XString cmd_line = module_name + " " + arguments;
					PROCESS_INFORMATION  process_information;
					memset( (void*)&process_information , 0 , sizeof( PROCESS_INFORMATION ) );			
					if( cmd_line()->_code_page == XEnumCodePage_UTF16 )
					{
						STARTUPINFOW startup_info;
						memset( (void*)&startup_info , 0 , sizeof( STARTUPINFOW ) );			
						startup_info.cb = sizeof( STARTUPINFOW );
						startup_info.dwFlags = STARTF_USESHOWWINDOW;
						startup_info.wShowWindow = SW_SHOW;
						if( CreateProcessW( NULL , (wchar_t*)cmd_line.str() , NULL , NULL , FALSE , CREATE_DEFAULT_ERROR_MODE , NULL , NULL , &startup_info , &process_information ) == 0 )				
							result = GetLastError();
						else
						{
							result = private_WaitForProcess( process_information.hProcess );
							if( ERROR_SUCCESS == result )
								GetExitCodeProcess( process_information.hProcess, &result );
						}
					}
					else if( cmd_line()->_code_page == XEnumCodePage_CP1251 )
					{
						STARTUPINFOA startup_info;
						memset( (void*)&startup_info , 0 , sizeof( STARTUPINFOA ) );			
						startup_info.cb = sizeof( STARTUPINFOA );
						startup_info.dwFlags = STARTF_USESHOWWINDOW;
						startup_info.wShowWindow = SW_SHOW;
						if( CreateProcessA( NULL , cmd_line.str() , NULL , NULL , FALSE , CREATE_DEFAULT_ERROR_MODE , NULL , NULL , &startup_info , &process_information ) == 0 )				
							result = GetLastError();
						else
						{
							result = private_WaitForProcess( process_information.hProcess );
							if( ERROR_SUCCESS == result )
								GetExitCodeProcess( process_information.hProcess, &result );
						}
					}
					else
					{
						::rux::XString utf16_cmd_line = cmd_line.ConvertToUTF16();
						STARTUPINFOW startup_info;
						memset( (void*)&startup_info , 0 , sizeof( STARTUPINFOW ) );			
						startup_info.cb = sizeof( STARTUPINFOW );
						startup_info.dwFlags = STARTF_USESHOWWINDOW;
						startup_info.wShowWindow = SW_SHOW;
						if( CreateProcessW( NULL , (wchar_t*)utf16_cmd_line.str() , NULL , NULL , FALSE , CREATE_DEFAULT_ERROR_MODE , NULL , NULL , &startup_info , &process_information ) == 0 )				
							result = GetLastError();
						else
						{
							result = private_WaitForProcess( process_information.hProcess );
							if( ERROR_SUCCESS == result )
								GetExitCodeProcess( process_information.hProcess, &result );
						}
					}
					if( process_information.hProcess )
						CloseHandle( process_information.hProcess );
					if( process_information.hThread )
						CloseHandle( process_information.hThread );
#endif
				}
			}
			return result == 0 ? 1 : 0;
		};
		XEnum_BinaryType get_binary_type( const char* filename )
		{
			XEnum_BinaryType binary_type = XEnum_BinaryType_Unknown;
#ifdef __WINDOWS__
			DWORD type = 0;
			size_t size = strlen( filename );
			if( size > MAX_PATH )
			{
				size_t ansi_to_utf16_bytes = 0;
				wchar_t* wcs_filename = (wchar_t*)XString::ansi_to_utf16( filename , size , ansi_to_utf16_bytes );
				if( wcs_filename )
				{
					if( GetBinaryTypeW( wcs_filename , &type ) )
					{
						if( type == SCS_32BIT_BINARY )
							binary_type = XEnum_BinaryType_Windows_x86;
						else if( type == SCS_64BIT_BINARY )
							binary_type = XEnum_BinaryType_Windows_x64;
					}
					::rux::engine::free_mem( wcs_filename );
				}
			}
			else
			{
				if( GetBinaryTypeA( filename , &type ) )
				{
					if( type == SCS_32BIT_BINARY )
						binary_type = XEnum_BinaryType_Windows_x86;
					else if( type == SCS_64BIT_BINARY )
						binary_type = XEnum_BinaryType_Windows_x64;
				}
			}
#elif defined __LINUX__
#endif
			return binary_type;
		};
	};
};
