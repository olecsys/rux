#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_native_diagnostics.h>
#include <xan_pdh.h>
#include <xan_io_defines.h>
#ifdef __WINDOWS__
#include <Psapi.h>
#include <TlHelp32.h>
#endif 
#include <xan_log.h>
#include <xan_error.h>
#include <xan_path.h>
namespace rux
{
	namespace diagnostics
	{
		::rux::int64 process_info::threads_count( ::rux::pid_t pid , ::rux::XString* error )
		{
			::rux::int64 threads = 0;
#ifdef __WINDOWS__
			HANDLE toolhelp32snapshot_handle = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS , 0 );
			if( toolhelp32snapshot_handle )
			{
				PROCESSENTRY32 ProcessEntry;
				ProcessEntry.dwSize = sizeof( ProcessEntry );
				BOOL res = Process32First( toolhelp32snapshot_handle , &ProcessEntry );
				while( res )
				{
					if( ProcessEntry.th32ProcessID != 0 && ProcessEntry.th32ProcessID == pid )
					{			
						threads = ProcessEntry.cntThreads;
						break;
					}
					res = Process32Next( toolhelp32snapshot_handle , &ProcessEntry );
				}
				CloseHandle( toolhelp32snapshot_handle );
			}
#else
			declare_stack_variable( char , pid_string , 64 );
			rux::string::uint32_to_string( pid , pid_string );
			const char* ps_path = "/usr/bin/ps";
			::rux::byte exists = rux_is_exists_file_or_link( ps_path );
			if( exists == 0 )
			{
				ps_path = "/bin/ps";
				exists = rux_is_exists_file_or_link( ps_path );
			}
			if( exists )
			{
				char* args[ 5 ] = {0};
				args[ 0 ] = const_cast< char* >( "-L" );
				args[ 1 ] = const_cast< char* >( "-o" );
				args[ 2 ] = const_cast< char* >( "pid=" );
				args[ 3 ] = const_cast< char* >( "-p" );
				args[ 4 ] = pid_string;
				::rux::diagnostics::process::start_with_redirect( ps_path , args , 5 , ::rux::diagnostics::on_lines_count_redirect_stdout_or_stderr_handler , 0 , &threads , 0 , 0 , NULL );
			}
#endif
			return threads;
		};
		::rux::int64 process_info::working_set( const char* instance_name , ::rux::XString* error )
		{
			::rux::int64 value = 0LL;
#ifdef __WINDOWS__
			size_t len = strlen( instance_name );
			if( len > 4 && strcmp( &instance_name[ len - 4 ] , ".exe" ) == 0 )
			{
				len -= 3;
				char* temp = alloc_array_macros( char , len );
				::memcpy( temp , instance_name , len );
				temp[ len - 1 ] = 0;
				::rux::pdh::counter( temp , 230 , 180 , PDH_FMT_LARGE , &value , error );
				::rux::engine::free_mem( temp );
			}
			else
				::rux::pdh::counter( instance_name , 230 , 180 , PDH_FMT_LARGE , &value , error );
#else
			pid_t pid = ::rux::diagnostics::process_info::pidof( instance_name );
			declare_stack_variable( char , filename , 2048 );
			::rux::safe_sprintf( filename , 2048 , "/proc/%u/status" , (::rux::uint32)pid );
			FILE* file = fopen( filename , "r" );
			if( file )
			{
				while( fgets( filename , 2048 , file ) != NULL )
				{
					if( strncmp( filename , "VmRSS:" , 6 ) == 0 )
					{
						char* begin = filename;
						while( *begin != 0 && ( *begin < '0' || *begin > '9' ) )
							begin++;
						char* end = begin;
						while( *end != 0 && *end >= '0' && *end <= '9' )
							end++;
						*end = 0;
						value = ::rux::string::to_int64( begin ) * 1024LL;
						break;
					}
				}
				fclose(file);
			}
#endif			
			return value;
		};
		pid_t process_info::pidof( const char* process_name , ::rux::pid_t include_pid , ::rux::pid_t except_pid 
			, bool check_only_fullname )
		{
			pid_t pid = 0;
			::rux::string process_name_string( process_name );
			::rux::string process_name_only_filename_string( process_name );
#ifdef __WINDOWS__
			process_name_string.replace_all( '/' , '\\' );
			::rux::io::path::get_filename_without_extension( process_name_only_filename_string );
			::rux::diagnostics::process::adjust_token_privileges();
			size_t drives_count = 0;
			char* drives[ 256 ] = {0};
			char dos_devices[ 256 ][ 64 ] = {0,0};
			char drives_data[ 4096 ] = {0};
			char* drives_data_ptr = drives_data;
			::rux::byte is_absolute = ::rux::io::XPath::IsAbsolutePath( process_name , SIZE_MAX );
			{				
				size_t drives_size = GetLogicalDriveStringsA( 4096 , drives_data_ptr );
				if( drives_size > 4096 )
				{
					drives_data_ptr = alloc_array_macros( char , drives_size );
					drives_size = GetLogicalDriveStringsA( 4096 , drives_data_ptr );
				}
				if( drives_size > 0 )
				{
					size_t index0 = 0;
					do
					{
						drives[ drives_count ] = &drives_data[ index0 ];
						drives_count++;
						size_t length = strlen( &drives_data[ index0 ] );
						index0 += length + 1;
					}
					while( index0 < drives_size );
				}
				for( size_t index0 = 0 ; index0 < drives_count ; index0++ )
				{
					::rux::string::replace_all( drives[ index0 ] , '\\' , 0 );
					QueryDosDeviceA( drives[ index0 ] , dos_devices[ index0 ] , 64 );
				}
			}
			HANDLE toolhelp32snapshot_handle = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS , 0 );
			if( toolhelp32snapshot_handle )
			{
				declare_stack_variable( char , process_name_ptr , 4096 );
				HANDLE process_handle = NULL;
				PROCESSENTRY32 ProcessEntry;
				ProcessEntry.dwSize = sizeof( ProcessEntry );
				BOOL res = Process32First( toolhelp32snapshot_handle , &ProcessEntry );
				while( res )
				{
					if( ProcessEntry.th32ProcessID != 0 && ProcessEntry.th32ProcessID != except_pid && ( include_pid == 0 || include_pid == ProcessEntry.th32ProcessID ) )
					{						
						::rux::string list_process_name_only_filename_string( ProcessEntry.szExeFile );
						::rux::io::path::get_filename_without_extension( list_process_name_only_filename_string );
						if( is_absolute )
						{
							process_handle = OpenProcess( PROCESS_QUERY_LIMITED_INFORMATION , FALSE , ProcessEntry.th32ProcessID );
							if( process_handle == NULL )
								process_handle = OpenProcess( PROCESS_QUERY_INFORMATION , FALSE , ProcessEntry.th32ProcessID );
							if( process_handle != NULL )
							{
								if( ::rux::engine::_globals->_psapi_module.GetProcessImageFileNameA( process_handle , process_name_ptr , 4096 ) )
								{
									::rux::string list_process_name_string( process_name_ptr );
									for( size_t index0 = 0 ; index0 < drives_count ; index0++ )
										list_process_name_string.replace_all( dos_devices[ index0 ] , drives[ index0 ] );
									if( process_name_string == list_process_name_string )
									{
										pid = ProcessEntry.th32ProcessID;
										CloseHandle( process_handle );
										break;
									}
								}
								CloseHandle( process_handle );
							}
						}
						if( check_only_fullname == false )
						{
							if( process_name_only_filename_string == list_process_name_only_filename_string )
							{
								pid = ProcessEntry.th32ProcessID;
								break;
							}
						}
					}
					res = Process32Next( toolhelp32snapshot_handle , &ProcessEntry );
				}
				CloseHandle( toolhelp32snapshot_handle );
			}
			if( drives_data_ptr != drives_data )
				::rux::engine::free_mem( drives_data_ptr );
#else
			process_name_string.replace_all( '\\' , '/' );
			::rux::io::path::get_filename( process_name_only_filename_string );
			XMallocArray< ::rux::io::directory* > dirs;
			::rux::io::directory::get_directories( "/proc" , dirs );
			for( size_t index0 = 0 ; index0 < dirs.Count() ; index0++ )
			{
				::rux::string list_pid_string( dirs[ index0 ]->get_directory_path() , 1 , 0 );
				list_pid_string.substring( 6 );
				::rux::pid_t list_pid = list_pid_string.to_uint32();
				if( list_pid != except_pid && ( include_pid == 0 || include_pid == list_pid ) )
				{
					::rux::string dir( dirs[ index0 ]->get_directory_path() , 1 , 0 );
					dir += "/exe";				
					declare_stack_variable( char , fullpath , 1024 );
					if( readlink( dir._str , fullpath , 1024 ) != -1 )
					{
						::rux::string only_filename( fullpath , 1 , 0 );
						::rux::io::path::get_filename( only_filename );
						if( process_name_string == fullpath
							|| ( check_only_fullname == false && process_name_only_filename_string == only_filename ) )
						{
							pid = list_pid;
							break;
						}
					}
					else
					{
						::rux::string dir( dirs[ index0 ]->get_directory_path() , 1 , 0 );
						dir += "/status";
						FILE* file = fopen( dir._str , "r" );
						if( file )
						{
							::rux::byte found = 0;
							while( fgets( fullpath , 1024 , file ) != NULL )
							{
								if( strncmp( fullpath , "Name:" , 5 ) == 0 )
								{
									char* begin = &fullpath[ 5 ];
									while( *begin == ' ' || *begin == '\t' )
										begin++;
									char* end = begin;
									while( *end != 0 )
									{
										if( *end == '\n' || *end == '\r' )
										{
											*end = 0;
											break;
										}
										end++;
									}
									size_t begin_size = strlen( begin );
									if( process_name_string == begin || ( check_only_fullname == false 
										&& ( process_name_only_filename_string == begin
										|| ( begin_size == 15 && process_name_only_filename_string.length() > 0 
										&& strncmp( process_name_only_filename_string._str , begin , 15 ) == 0 ) ) ) )
									{
										pid = list_pid;
										found = 1;
									}
									break;
								}
							}
							fclose( file );
							if( found )
								break;
						}
					}
				}
			}
			for( size_t index0 = 0 ; index0 < dirs.Count() ; index0++ )
				::rux::engine::free_object< ::rux::io::directory >( dirs[ index0 ] , ::rux::engine::rux_free_dummy );
#endif
			return pid;
		};
		::rux::int64 process_info::virtual_bytes( const char* instance_name , ::rux::XString* error )
		{
			::rux::int64 value = 0LL;
#ifdef __WINDOWS__
			size_t len = strlen( instance_name );
			if( len > 4 && strcmp( &instance_name[ len - 4 ] , ".exe" ) == 0 )
			{
				len -= 3;
				char* temp = alloc_array_macros( char , len );
				::memcpy( temp , instance_name , len );
				temp[ len - 1 ] = 0;
				::rux::pdh::counter( temp , 230 , 186 , PDH_FMT_LARGE , &value , error );
				::rux::engine::free_mem( temp );
			}
			else
				::rux::pdh::counter( instance_name , 230 , 186 , PDH_FMT_LARGE , &value , error );
#else
			pid_t pid = ::rux::diagnostics::process_info::pidof( instance_name );
			declare_stack_variable( char , filename , 2048 );
			::rux::safe_sprintf( filename , 2048 , "/proc/%u/status" , (::rux::uint32)pid );
			FILE* file = fopen( filename , "r" );
			if( file )
			{
				while( fgets( filename , 2048 , file ) != NULL )
				{
					if( strncmp( filename , "VmSize:" , 7 ) == 0 )
					{
						char* begin = filename;
						while( *begin != 0 && ( *begin < '0' || *begin > '9' ) )
							begin++;
						char* end = begin;
						while( *end != 0 && *end >= '0' && *end <= '9' )
							end++;
						*end = 0;
						value = ::rux::string::to_int64( begin ) * 1024LL;
						break;
					}
				}
				fclose(file);
			}
#endif
			return value;
		};
	};
};