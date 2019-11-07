#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_debug.h"
#ifdef __LINUX__
#include <cxxabi.h>
#endif
#ifdef __WINDOWS__
#include <Psapi.h>
#endif
#include "xan_console.h"
#include "xan_thread.h"
#include "xan_io_defines.h"
#include "xan_file_descriptor_waiter.h"
#include "xan_memory_helper.h"
#include "xan_memory_chunk.h"
#include "xan_directory.h"
#include "xan_engine_globals.h"
#include "xan_log.h"
#include "xan_error.h"
#include "xan_native_diagnostics.h"
namespace rux
{
	namespace debug
	{
		dll_internal void get_CallStack( char stack_ptr[] , size_t stack_ptr_size , void** stack_frames , ::rux::uint16 stack_frames_count , size_t stack_frames_offset )
		{	
			::rux::debug::initialize();
		#ifdef __WINDOWS__
			stack_ptr[ 0 ] = 0;
			if( ::rux::engine::_globals->_is_call_stack_initialized )
			{		
				::rux::uint64 displacement = 0;		
				DWORD displacement32 = 0;		
				IMAGEHLP_SYMBOL64* imagehlp_symbol64 = NULL;
				IMAGEHLP_LINEA64 imagehelp_line64;
				::rux::uint8 is_imagehlp_line64 = 0;
				::rux::uint8 is_sym_from_addr = 0;
				::rux::uint8 imagehlp_symbol64_ptr[ sizeof( IMAGEHLP_SYMBOL64 ) + MAX_SYM_NAME ] = {0};
				char one_stack_line[ 2048 ] = "";
				HMODULE hmodule = NULL;
				char module_path[ 4096 ] = "";
				rux::byte sym_refresh_module_list = 0;
				//DWORD64 loaded_modules[ 32 ] = {0};
				size_t loaded_modules_count = 0;
				::rux::string executable_directory_string( ::rux::engine::_globals->_executable_directory );
				executable_directory_string += "\\";
				for( size_t index0 = stack_frames_offset ; index0 < stack_frames_count ; index0++ )
				{	
					module_path[ 0 ] = 0;
					is_imagehlp_line64 = 0;
					is_sym_from_addr = 0;
					if( GetModuleHandleExA( GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT , (LPCTSTR)(DWORD64)stack_frames[ index0 ] , &hmodule ) )
					{
						if( GetModuleFileNameA( hmodule , module_path , 4096 ) == 0 )
							module_path[ 0 ] = 0;
					}
					if( module_path[ 0 ] != 0 )
					{
						DWORD64 mod_base = 0;
						if( sym_refresh_module_list == 0 )
						{
							CS_PTR_LOCK( ::rux::debug::_cs );
							if( ::rux::engine::_globals->_dbghelp_module._SymLoadModuleEx )
								mod_base = ::rux::engine::_globals->_dbghelp_module.SymLoadModuleEx( ::rux::engine::_globals->_current_process_handle , 0 , module_path , 0 , 0 , 0 , 0 , 0 );
							else if( ::rux::engine::_globals->_dbghelp_module._SymLoadModule64 )
								mod_base = ::rux::engine::_globals->_dbghelp_module.SymLoadModule64( ::rux::engine::_globals->_current_process_handle , 0 , module_path , 0 , 0 , 0 );
							if( mod_base != 0 )
							{
								//loaded_modules[ loaded_modules_count ] = mod_base;
								loaded_modules_count++;
								IMAGEHLP_MODULE64 imagehlp_module64;
								memset( &imagehlp_module64 , 0 , sizeof( imagehlp_module64 ) ); 
								imagehlp_module64.SizeOfStruct = sizeof( imagehlp_module64 ); 
								::rux::engine::_globals->_dbghelp_module.SymGetModuleInfo64( ::rux::engine::_globals->_current_process_handle , mod_base , &imagehlp_module64 );
							}
							::rux::debug::_cs->UnLock();
						}
					}
					::rux::string module_path_string( module_path );
					module_path_string.replace_all( executable_directory_string._str , "" );
					module_path_string.replace_all( "C:\\Windows\\SYSTEM32\\" , "" );
					module_path_string.replace_all( "C:\\Windows\\system32\\" , "" );

					memset( imagehlp_symbol64_ptr , 0 , sizeof( IMAGEHLP_SYMBOL64 ) + MAX_SYM_NAME );
					imagehlp_symbol64 = ( IMAGEHLP_SYMBOL64* )imagehlp_symbol64_ptr;
					imagehlp_symbol64->SizeOfStruct = sizeof( IMAGEHLP_SYMBOL64 );
					imagehlp_symbol64->MaxNameLength = MAX_SYM_NAME;
					CS_PTR_LOCK( ::rux::debug::_cs );			
					is_sym_from_addr = ::rux::engine::_globals->_dbghelp_module.SymGetSymFromAddr64( ::rux::engine::_globals->_current_process_handle , (::rux::uint64)stack_frames[ index0 ] , &displacement , imagehlp_symbol64 ) ? 1 : 0;			
					::rux::debug::_cs->UnLock();
					if( is_sym_from_addr == 0 )
					{
						if( sym_refresh_module_list == 0 )
						{
							CS_PTR_LOCK( ::rux::debug::_cs );
							if( ::rux::engine::_globals->_dbghelp_module._SymRefreshModuleList )
								::rux::engine::_globals->_dbghelp_module.SymRefreshModuleList( ::rux::engine::_globals->_current_process_handle );
							else
							{
								::rux::debug::deinitialize();
								::rux::debug::initialize();
							}
							is_sym_from_addr = ::rux::engine::_globals->_dbghelp_module.SymGetSymFromAddr64( ::rux::engine::_globals->_current_process_handle , (::rux::uint64)stack_frames[ index0 ] , &displacement , imagehlp_symbol64 ) ? 1 : 0;
							::rux::debug::_cs->UnLock();
							sym_refresh_module_list = 1;
						}
					}
					if( is_sym_from_addr == 1 )
					{				
						memset( &imagehelp_line64 , 0 , sizeof( IMAGEHLP_LINEA64 ) );				
						imagehelp_line64.SizeOfStruct = sizeof( IMAGEHLP_LINEA64 );
						CS_PTR_LOCK( ::rux::debug::_cs );
						is_imagehlp_line64 = ::rux::engine::_globals->_dbghelp_module.SymGetLineFromAddr64( ::rux::engine::_globals->_current_process_handle , (::rux::uint64)stack_frames[ index0 ] , &displacement32 , &imagehelp_line64 ) ? 1 : 0;																		
						::rux::debug::_cs->UnLock();
					}
					rux::byte need_point = 0;
					::rux::safe_sprintf( one_stack_line , 2048 , "%u: " , (::rux::uint32)index0 );
					if( module_path_string._str[ 0 ] != 0 )
					{
						::rux::safe_strncat( one_stack_line , module_path_string._str , 2048 );
						need_point = 1;
					}
					if( imagehlp_symbol64->Name && imagehlp_symbol64->Name[ 0 ] != 0 )
					{
						if( need_point )
							::rux::safe_strncat( one_stack_line , ", " , 2048 );
						::rux::safe_strncat( one_stack_line , imagehlp_symbol64->Name , 2048 );
						need_point = 1;
					}
					if( imagehlp_symbol64->Address != 0 )
					{
						if( need_point )
							::rux::safe_strncat( one_stack_line , ", " , 2048 );
						::rux::string::uint64_to_hex_string( imagehlp_symbol64->Address , &one_stack_line[ strlen( one_stack_line ) ] );
						need_point = 1;
					}
					if( imagehelp_line64.FileName && imagehelp_line64.FileName[ 0 ] != 0 )
					{
						if( need_point )
							::rux::safe_strncat( one_stack_line , ", " , 2048 );
						::rux::safe_strncat( one_stack_line , imagehelp_line64.FileName , 2048 );
						::rux::safe_strncat( one_stack_line , ":" , 2048 );
						::rux::string::uint32_to_string( imagehelp_line64.LineNumber , &one_stack_line[ strlen( one_stack_line ) ] );
						need_point = 1;
					}
					if( need_point == 0 )
						::rux::safe_strncat( one_stack_line , "?????" , 2048 );
					::rux::safe_strncat( one_stack_line , "\n" , 2048 );
					if( strlen( stack_ptr ) + strlen( one_stack_line ) < stack_ptr_size )
						::rux::safe_sprintf( &stack_ptr[ strlen( stack_ptr ) ] , stack_ptr_size , "%s" , one_stack_line );
					else
						break;
				}
			}
		#endif
		};
		dll_internal rux::uint16 get_CallStack( void** stack_frames , size_t stack_frames_offset )
		{
			::rux::uint16 stack_frames_count0 = 0;
		#ifdef __WINDOWS__
			::rux::engine::_globals->_kernel32_module.load();
			if( ::rux::engine::_globals->_kernel32_module._RtlCaptureStackBackTrace )
				stack_frames_count0 = ::rux::engine::_globals->_kernel32_module.RtlCaptureStackBackTrace( 1 + stack_frames_offset , RUX_MAX_STACK_FRAMES , stack_frames , NULL );
		#endif
			return stack_frames_count0;
		};
		#ifdef __WINDOWS__
		dll_internal void get_CallStack( char stack_ptr[] , size_t stack_ptr_size , _EXCEPTION_POINTERS* info , size_t stack_frames_offset )
		{
			::rux::debug::initialize();
			if( ::rux::engine::_globals->_is_call_stack_initialized )
			{		
				void* stack_frames[ RUX_MAX_STACK_FRAMES ] = {0};
				STACKFRAME64 stack_frame;
				memset( &stack_frame , 0 , sizeof( stack_frame ) );
		#if defined( __x64__ )
				int machine_type = IMAGE_FILE_MACHINE_AMD64;
				stack_frame.AddrPC.Offset = info->ContextRecord->Rip;
				stack_frame.AddrFrame.Offset = info->ContextRecord->Rbp;
				stack_frame.AddrStack.Offset = info->ContextRecord->Rsp;
		#else
				int machine_type = IMAGE_FILE_MACHINE_I386;
				stack_frame.AddrPC.Offset = info->ContextRecord->Eip;
				stack_frame.AddrFrame.Offset = info->ContextRecord->Ebp;
				stack_frame.AddrStack.Offset = info->ContextRecord->Esp;
		#endif
				stack_frame.AddrPC.Mode = AddrModeFlat;
				stack_frame.AddrFrame.Mode = AddrModeFlat;
				stack_frame.AddrStack.Mode = AddrModeFlat;
				::rux::uint16 stack_frames_count = 0;		
				while( ::rux::engine::_globals->_dbghelp_module.StackWalk64( machine_type , ::rux::engine::_globals->_current_process_handle , GetCurrentThread() , &stack_frame , info->ContextRecord , NULL , ::rux::engine::_globals->_dbghelp_module._SymFunctionTableAccess64 , ::rux::engine::_globals->_dbghelp_module._SymGetModuleBase64 , NULL ) 
					&& stack_frames_count < RUX_MAX_STACK_FRAMES )
					stack_frames[ stack_frames_count++ ] = reinterpret_cast< void* >( stack_frame.AddrPC.Offset );
				if( stack_frames_count > 0 )
					get_CallStack( stack_ptr , stack_ptr_size , stack_frames , stack_frames_count , stack_frames_offset );
			}
		};
		#endif
		dll_internal void get_CallStack( char stack_ptr[] , size_t stack_ptr_size , size_t stack_frames_offset )
		{	
		#ifdef __WINDOWS__
			void* stack_frames[ RUX_MAX_STACK_FRAMES ] = {0};	
			::rux::uint16 stack_frames_count = get_CallStack( stack_frames , stack_frames_offset );	
			if( stack_frames_count > 0 )
				get_CallStack( stack_ptr , stack_ptr_size , stack_frames , stack_frames_count );		
		#endif
		#if defined( __LINUX__ ) && !defined( __ANDROID__ )
			void* stack_frames[ RUX_MAX_STACK_FRAMES ] = {0};	
			memset( stack_frames , 0 , RUX_MAX_STACK_FRAMES * sizeof( void* ) );
			::rux::int32 size = backtrace( stack_frames , RUX_MAX_STACK_FRAMES - 1 );
			if( size > 1 )
			{
				::rux::int32 pipes[ 2 ] = {0};
				if( pipe( pipes ) == 0 )
				{	
					::rux::uint8 is_addr2line_exists = rux_is_exists_file_or_link( "/usr/bin/addr2line" );
					::rux::uint8 is_gdb_exists = rux_is_exists_file_or_link( "/usr/bin/gdb" );
					char executable[ 512 ] = {0};
					if( is_addr2line_exists == 1 )
					{
						::rux::safe_strncpy( executable , ::rux::engine::_globals->_executable_directory , 512 );
						::rux::safe_strncat( executable , "/" , 512 );
						::rux::safe_strncat( executable , ::rux::engine::_globals->_executable , 512 );
					}
					char backtrace[ 1024 * 8 ] = {0};
					backtrace_symbols_fd( &stack_frames[ 1 + stack_frames_offset ] , size - ( 1 + stack_frames_offset ) , pipes[ 1 ] ); 			
					::rux::int32 readen_size = 0;
					::rux::int32 offset = 0;
					while( ( readen_size = read( pipes[ 0 ] , &backtrace[ offset ] , 1024 ) ) > 0 )
					{
						offset += readen_size;
						if( readen_size < 1024 || offset + 1024 > 1024 * 8 )
							break;
					}
					backtrace[ offset ] = 0;
					if( backtrace[ 0 ] != 0 )
					{
						stack_ptr[ 0 ] = 0;
						char* args[ 7 ] = {0};
						char callstack_line_str[ 16 ] = {0};
						::rux::int32 callstack_line = 0;
						char gdb_command[ 512 ] = {0};
						char gdb_file_command[ 512 ] = {0};
						char func_address[ 64 ] = {0};
						char func_offset[ 64 ] = {0};
						char module[ 512 ] = {0};
						char cpp_file[ 512 ] = {0};
						char mangled_function[ 512 ] = {0};
						char function[ 512 ] = {0};
						char pipe_res[ 1024 ] = {0};
						char* ptr = backtrace;
						::rux::int32 temp_index1 = -1;
						::rux::int32 temp_index2 = -1;
						::rux::int32 temp_index3 = -1;
						::rux::byte video7_lin_exists = 0;
						rux::int32 gdbcmd_file = -1;
						size_t gdbcmd_file_size = 0;
						if( is_gdb_exists )
						{
							video7_lin_exists = ::rux::io::directory::exists( "/mnt/video7_lin" ) ? 1 : 0;
							if( video7_lin_exists )
							{
								video7_lin_exists = 0;
								char gdb_command_filename[ 1024 ] = {0};
								::rux::safe_strncpy( gdb_command_filename , ::rux::engine::_globals->_executable_directory , 1024 );
								::rux::safe_strncat( gdb_command_filename , "/gdbcmd" , 1024 );
								::rux::uint8 is_exists = rux_is_exists_file( gdb_command_filename );
								if( is_exists )
									rux_remove_file( gdb_command_filename );


								gdbcmd_file = rux_open( gdb_command_filename , O_CREAT | O_WRONLY | O_TRUNC );
								if( gdbcmd_file != -1 )
								{
									char gdb_command_file_text[ 1024 ] = {0};
									::rux::safe_strncpy( gdb_command_file_text , "set debug-file-directory /mnt/video7_lin\nfile " , 1024 );
									gdbcmd_file_size = strlen( gdb_command_file_text );
									rux_write( gdbcmd_file , gdb_command_file_text , strlen( gdb_command_file_text ) );

									gdb_file_command[ 0 ] = 0;
									::rux::safe_strncpy( gdb_file_command , "-x=" , 512 );
									::rux::safe_strncat( gdb_file_command , gdb_command_filename , 512 );
									video7_lin_exists = 1;
								}
							}
						}
						::rux::memory::stack_chunk_t chunk( (::rux::byte*)pipe_res , 1024 , ::rux::memory::string_type , 0 );
						for( ; ; )
						{
							 func_address[ 0 ] = 0;
							 module[ 0 ] = 0;
							 function[ 0 ] = 0;
							 cpp_file[ 0 ] = 0;
							 mangled_function[ 0 ] = 0;

							rux::string::uint32_to_string( (::rux::uint32)callstack_line , callstack_line_str );
							callstack_line++;
							::rux::int32 temp_index0 = ::rux::string::index_of( ptr , "\n" );
							if( temp_index0 > -1 )
								ptr[ temp_index0 ] = 0;
							::rux::safe_strncat( stack_ptr , callstack_line_str , stack_ptr_size );
							::rux::safe_strncat( stack_ptr , ": " , stack_ptr_size );
							
							func_offset[ 0 ] = 0;
							temp_index1 = ::rux::string::index_of( ptr , "(" );
							temp_index2 = ::rux::string::index_of( ptr , ")" );
							if( temp_index1 > -1 && temp_index2 > -1 && temp_index2 - temp_index1 > 1 )
							{
								ptr[ temp_index2 ] = 0;
								temp_index3 = ::rux::string::index_of( ptr , "+" );
								if( temp_index3 > -1 )
								{
									::rux::safe_strncpy( func_offset , &ptr[ temp_index3 + 1 ] , 64 );
									ptr[ temp_index3 ] = 0;
									::rux::safe_strncpy( mangled_function , &ptr[ temp_index1 + 1 ] , 512 );
									ptr[ temp_index3 ] = '+';
								}
								else
									::rux::safe_strncpy( mangled_function , &ptr[ temp_index1 + 1 ] , 512 );
								ptr[ temp_index2 ] = ')';
							}
							temp_index1 = ::rux::string::index_of( ptr , "[" );
							temp_index2 = ::rux::string::index_of( ptr , "]" );
							if( temp_index1 > -1 && temp_index2 > -1 && temp_index2 - temp_index1 > 1 )
							{
								ptr[ temp_index2 ] = 0;
								::rux::safe_strncpy( func_address , &ptr[ temp_index1 + 1 ] , 64 );
								ptr[ temp_index2 ] = ']';
							}
							temp_index2 = ::rux::string::index_of( ptr , "(" );
							if( temp_index2 == -1 )
							{
								temp_index2 = ::rux::string::index_of( ptr , "[" );
								if( temp_index2 > -1 )
								{
									ptr[ temp_index2 ] = 0;
									::rux::safe_strncpy( module , ptr , 512 );
									ptr[ temp_index2 ] = '[';
								}
							}
							else
							{
								ptr[ temp_index2 ] = 0;
								::rux::safe_strncpy( module , ptr , 512 );
								ptr[ temp_index2 ] = '(';
							}
							if( module[ 0 ] == 0 && func_address[ 0 ] != 0 )
							{
								::rux::uint64 addr = 0;
								::rux::string::hex_string_to_uint64( func_address , addr );
								Dl_info info;
								if( dladdr( (void*)addr , &info ) != 0
									&& info.dli_fname && info.dli_fname[ 0 ] != 0 )
									::rux::safe_strncpy( module , info.dli_fname , 512 );
							}
							::rux::byte gdb_success = 0;
							if( is_gdb_exists && module[ 0 ] != 0 && ( func_offset[ 0 ] != 0 || func_address[ 0 ] != 0 ) )
							{
								gdb_command[ 0 ] = 0;
								::rux::safe_strncpy( gdb_command , "-ex=info line *" , 512 );
								if( func_offset[ 0 ] != 0 )
									::rux::safe_strncat( gdb_command , func_offset , 512 );
								else
									::rux::safe_strncat( gdb_command , func_address , 512 );
								pipe_res[ 0 ] = 0;
								chunk.clear();
								args[ 0 ] = const_cast< char* >( "--batch" );
								temp_index1 = 1;
								if( video7_lin_exists )
								{
									if( gdbcmd_file != -1 )
									{
										ftruncate( gdbcmd_file , gdbcmd_file_size );
										lseek( gdbcmd_file , 0 , SEEK_END );
										rux_write( gdbcmd_file , module , strlen( module ) );
										fsync( gdbcmd_file );
									}
									args[ temp_index1 ] = gdb_file_command;
									temp_index1++;
								}
								args[ temp_index1 ] = gdb_command;
								temp_index1++;
								args[ temp_index1 ] = module;
								temp_index1++;
								if( ::rux::engine::_globals->_alarm_is_addr2line )
									*(::rux::engine::_globals->_alarm_is_addr2line) = 1;
								alarm( 2 );
								::rux::diagnostics::process::start_with_redirect( "/usr/bin/gdb" , args , temp_index1 , ::rux::diagnostics::on_default_redirect_stdout_or_stderr_handler , 0 , &chunk , 0 , 0 , NULL );
								if( ::rux::engine::_globals->_exception_raised )
									alarm( 2 );
								else
									alarm( 0 );
								if( ::rux::engine::_globals->_alarm_is_addr2line )
									*(::rux::engine::_globals->_alarm_is_addr2line) = 0;
								if( chunk.actual_size() > 0 )
								{
									temp_index1 = ::rux::string::index_of( pipe_res , "Line " );
									if( temp_index1 != -1 )
									{
										gdb_success = 1;
										temp_index2 = ::rux::string::index_of( pipe_res , " of \"" );
										if( temp_index2 != -1 && temp_index1 < temp_index2 )
										{
											temp_index3 = ::rux::string::index_of( pipe_res , "\"\n" );
											if( temp_index3 != -1 && temp_index3 > temp_index2 )
											{
												pipe_res[ temp_index3 ] = 0;
												::rux::safe_strncpy( cpp_file , &pipe_res[ temp_index2 + 5 ] , 512 );
												pipe_res[ temp_index3 ] = '"';
												::rux::safe_strncat( cpp_file , ":" , 512 );
												pipe_res[ temp_index2 ] = 0;
												::rux::safe_strncat( cpp_file , &pipe_res[ temp_index1 + 5 ] , 512 );
												pipe_res[ temp_index2 ] = ' ';
												temp_index1 = ::rux::string::index_of( pipe_res , " <" );
												if( temp_index1 != -1 )
												{
													temp_index2 = ::rux::string::index_of( pipe_res , ">\n" );
													if( temp_index2 == -1 )
														temp_index2 = ::rux::string::index_of( pipe_res , "> " );
													if( temp_index2 != -1 && temp_index2 > temp_index1 )
													{
														pipe_res[ temp_index2 ] = 0;
														::rux::safe_strncpy( mangled_function , &pipe_res[ temp_index1 + 2 ] , 512 );
														pipe_res[ temp_index2 ] = '>';
														temp_index1 = ::rux::string::index_of( mangled_function , "+" );
														if( temp_index1 > -1 )
															mangled_function[ temp_index1 ] = 0;
													}
												}
											}
										}
									}
									else
									{
										temp_index1 = ::rux::string::index_of( pipe_res , "No line number " );
										if( temp_index1 != -1 )
										{
											gdb_success = 1;
											temp_index1 = ::rux::string::index_of( pipe_res , " <" );
											if( temp_index1 != -1 )
											{
												temp_index2 = ::rux::string::index_of( pipe_res , ">\n" );
												if( temp_index2 == -1 )
													temp_index2 = ::rux::string::index_of( pipe_res , "> " );
												if( temp_index2 != -1 && temp_index2 > temp_index1 )
												{
													pipe_res[ temp_index2 ] = 0;
													::rux::safe_strncpy( mangled_function , &pipe_res[ temp_index1 + 2 ] , 512 );
													pipe_res[ temp_index2 ] = '>';
													temp_index1 = ::rux::string::index_of( mangled_function , "+" );
													if( temp_index1 > -1 )
														mangled_function[ temp_index1 ] = 0;
												}
											}
										}
									}
								}								
							}
							if( gdb_success == 0 && function[ 0 ] == 0 && cpp_file[ 0 ] == 0 )
							{
								if( func_address[ 0 ] != 0 )
								{
									if( is_addr2line_exists == 1 )
									{
										pipe_res[ 0 ] = 0;
										chunk.clear();
										args[ 0 ] = const_cast< char* >( "-s" );
										args[ 1 ] = const_cast< char* >( "-f" );
										args[ 2 ] = const_cast< char* >( "-C" );
										args[ 3 ] = const_cast< char* >( "-i" );
										args[ 4 ] = const_cast< char* >( "-e" );
										args[ 5 ] = module;
										if( func_offset[ 0 ] != 0 )
											args[ 6 ] = func_offset;
										else
											args[ 6 ] = func_address;
										if( ::rux::engine::_globals->_alarm_is_addr2line )
											*(::rux::engine::_globals->_alarm_is_addr2line) = 1;
										alarm( 10 );
										::rux::diagnostics::process::start_with_redirect( "/usr/bin/addr2line" , args , 7 , ::rux::diagnostics::on_default_redirect_stdout_or_stderr_handler , 0 , &chunk , 0 , 0 , NULL );
										if( ::rux::engine::_globals->_exception_raised )
											alarm( 2 );
										else
											alarm( 0 );
										if( ::rux::engine::_globals->_alarm_is_addr2line )
											*(::rux::engine::_globals->_alarm_is_addr2line) = 0;
										if( chunk.actual_size() > 0 )
										{
											if( ::rux::string::index_of( pipe_res , "No such file" ) == -1 )
											{
												if( pipe_res[ 0 ] != '?' )
												{
													temp_index1 = ::rux::string::index_of( pipe_res , "\n" );
													if( temp_index1 > -1 )
													{
														temp_index2 = ::rux::string::index_of( pipe_res , ":" );
														if( temp_index2 > temp_index1 )
														{
															pipe_res[ temp_index1 ] = 0;
															::rux::safe_strncpy( function , pipe_res , 512 );
															if( pipe_res[ temp_index1 + 1 ] != '?' )
															{
																::rux::safe_strncpy( cpp_file , &pipe_res[ temp_index1 + 1 ] , 512 );
																temp_index1 = ::rux::string::index_of( cpp_file , "\n" );
																if( temp_index1 > -1 )
																	cpp_file[ temp_index1 ] = 0;
															}
														}
													}
												}
											}
										}
									}
								}
							}
							if( function[ 0 ] == 0 && mangled_function[ 0 ] != 0 )
							{
								int status = 0;
								size_t function_size = 512;
								abi::__cxa_demangle( mangled_function , function , &function_size , &status );
								if( status != 0 )
									function[ 0 ] = 0;
							}
							if( module[ 0 ] != 0 )
							{
								temp_index1 = ::rux::string::last_index_of( module , "/" );
								if( temp_index1 > -1 )
									::rux::safe_strncat( stack_ptr , &module[ temp_index1 + 1 ] , stack_ptr_size );
								else
									::rux::safe_strncat( stack_ptr , module , stack_ptr_size );
							}
							else
								::rux::safe_strncat( stack_ptr , "?????" , stack_ptr_size );
							if( function[ 0 ] != 0 )
							{
								::rux::safe_strncat( stack_ptr , ", " , stack_ptr_size );
								::rux::safe_strncat( stack_ptr , function , stack_ptr_size );
							}
							else if( mangled_function[ 0 ] != 0 )
							{
								::rux::safe_strncat( stack_ptr , ", " , stack_ptr_size );
								::rux::safe_strncat( stack_ptr , mangled_function , stack_ptr_size );
							}
							else if( func_offset[ 0 ] != 0 )
							{
								::rux::safe_strncat( stack_ptr , ", +" , stack_ptr_size );
								::rux::safe_strncat( stack_ptr , func_offset , stack_ptr_size );
							}
							else if( func_address[ 0 ] != 0 )
							{
								::rux::safe_strncat( stack_ptr , ", " , stack_ptr_size );
								::rux::safe_strncat( stack_ptr , func_address , stack_ptr_size );
							}
							else
								::rux::safe_strncat( stack_ptr , ", ?????" , stack_ptr_size );
							if( cpp_file[ 0 ] != 0 )
							{
								::rux::safe_strncat( stack_ptr , ", " , stack_ptr_size );
								temp_index1 = ::rux::string::last_index_of( cpp_file , "/" );
								if( temp_index1 > -1 )
									::rux::safe_strncat( stack_ptr , &cpp_file[ temp_index1 + 1 ] , stack_ptr_size );
								else
									::rux::safe_strncat( stack_ptr , cpp_file , stack_ptr_size );
							}
							if( temp_index0 == -1 )
								break;
							else
							{
								ptr = &ptr[ temp_index0 + 1 ];
								if( *ptr == 0 )
									break;
								::rux::safe_strncat( stack_ptr , "\n" , stack_ptr_size );
							}
						}
						if( gdbcmd_file != -1 )
						{
							rux_close( gdbcmd_file );
							char gdb_command_filename[ 1024 ] = {0};
							::rux::safe_strncpy( gdb_command_filename , ::rux::engine::_globals->_executable_directory , 1024 );
							::rux::safe_strncat( gdb_command_filename , "/gdbcmd" , 1024 );
							::rux::uint8 is_exists = rux_is_exists_file( gdb_command_filename );
							if( is_exists )
								rux_remove_file( gdb_command_filename );
						}
					}
					close( pipes[ 0 ] );
					close( pipes[ 1 ] );
				}
			}
		#endif
		#ifdef __SOLARIS__	
			::rux::uint8 is_pstack_exists = rux_is_exists_file( "/usr/bin/pstack" );	
			::rux::uint8 is_c_filt_exists = rux_is_exists_file( "/usr/bin/c++filt" );	
			if( is_pstack_exists == 1 )
			{
				::rux::uint32 stack_ptr_size = 0 ;
				const char* arg0 = "/usr/bin/pstack";
				const char* arg1 = "-F";		
				declare_stack_variable( char , arg2 , 256 );
				declare_stack_variable( char , pid_string , 64 );
				rux::string::uint32_to_string( getpid() , pid_string );
				declare_stack_variable( char , tid_string , 64 );
				rux::string::uint32_to_string( rux_gettid() , tid_string );
				const char* const_string = NULL;
				::rux::uint32 offset = 0;			
				::rux::engine::memcpy( arg2 , offset , pid_string , strlen( pid_string ) );
				offset += strlen( pid_string );
				const_string = "/";
				::rux::engine::memcpy( arg2 , offset , const_string , strlen( const_string ) );
				offset += strlen( const_string );
				::rux::engine::memcpy( arg2 , offset , tid_string , strlen( tid_string ) );
				offset += strlen( tid_string );
				arg2[ offset ] = '\0';
				::rux::int32 pstack_pipes[ 2 ] = {0};		
				if( pipe( pstack_pipes ) == 0 )
				{			
					::rux::pid_t child_pid = fork();
					if( child_pid == 0 )
					{				
						close( pstack_pipes[ 0 ] );				
						dup2( pstack_pipes[ 1 ] , STDOUT_FILENO );				
						dup2( pstack_pipes[ 1 ] , STDERR_FILENO );
						close( pstack_pipes[ 1 ] );				
						execl( "/usr/bin/pstack" , arg0 , arg2 , (char*)0 );
						_exit( 1 );
					}			
					else if( child_pid != 0 )
					{
						close( pstack_pipes[ 1 ] );				
						offset = 0;
						::rux::io::file_descriptor_waiter fd_waiter;
						fd_waiter.add( pstack_pipes[ 0 ] );
						if( fd_waiter.wait( 5000 , 1 , 0 ) && fd_waiter.get_read_fd() != -1 )
						{			
							::rux::int32 readen_bytes = 0;			
							while( ( readen_bytes = read( pstack_pipes[ 0 ] , &stack_ptr[ offset ] , 1024 ) ) > 0 )
							{
								offset += readen_bytes;
								if( readen_bytes < 1024 )
									break;
							}			
						}
						stack_ptr[ offset ] = '\0';		
						close( pstack_pipes[ 0 ] );
						waitpid( child_pid , NULL , 0 );
						if( is_c_filt_exists == 1 )
						{				
							if( pipe( pstack_pipes ) == 0 )
							{					
								declare_stack_variable( char , executable_filename_only , 1024 );
								rux_get_executable_filename_only( executable_filename_only );
								const_string = "/var/log/";
								offset = 0;
								::rux::engine::memcpy( arg2 , offset , const_string , strlen( const_string ) );
								offset += strlen( const_string );
								::rux::engine::memcpy( arg2 , offset , executable_filename_only , strlen( executable_filename_only ) );
								offset += strlen( executable_filename_only );
								const_string = ".log";						
								::rux::engine::memcpy( arg2 , offset , const_string , strlen( const_string ) );
								offset += strlen( const_string );
								arg2[ offset ] = '\0';
								rux_append_to_file( arg2 , stack_ptr );
								child_pid = fork();
								if( child_pid == 0 )
								{							
									close( pstack_pipes[ 0 ] );							
									dup2( pstack_pipes[ 1 ] , STDOUT_FILENO );							
									dup2( pstack_pipes[ 1 ] , STDERR_FILENO );
									close( pstack_pipes[ 1 ] );							
									execl( "/usr/bin/c++filt" , "/usr/bin/c++filt" , arg2 , (char*)0 );
									_exit( 1 );
								}						
								else if( child_pid != 0 )
								{
									close( pstack_pipes[ 1 ] );							
									offset = 0;
									::rux::io::file_descriptor_waiter fd_waiter;
									fd_waiter.add( pstack_pipes[ 0 ] );
									if( fd_waiter.wait( 5000 , 1 , 0 ) && fd_waiter.get_read_fd() != -1 )
									{
										::rux::int32 readen_bytes = 0;
										while( ( readen_bytes = read( pstack_pipes[ 0 ] , &stack_ptr[ offset ] , 1024 ) ) > 0 )
										{
											offset += readen_bytes;
											if( readen_bytes < 1024 )
												break;
										}
									}
									stack_ptr[ offset ] = '\0';
									close( pstack_pipes[ 0 ] );
									rux_remove_file( arg2 );
									waitpid( child_pid , NULL , 0 );
								}
							}
						}
					}
				}
			}
		#endif	
		};
		dll_internal void initialize( void )
		{	
			CS_PTR_LOCK( ::rux::debug::_cs );
		#ifdef __WINDOWS__
			::rux::engine::_globals->_kernel32_module.load();
			::rux::engine::_globals->_symsrv_module.load();
			::rux::engine::_globals->_dbghelp_module.load();
			if( ::rux::engine::_globals->_dbghelp_module.loaded() )
			{
				if( ::rux::engine::_globals->_is_call_stack_initialized == 0 )
				{
					::rux::engine::_globals->_current_process_handle = GetCurrentProcess();
					::rux::engine::_globals->_dbghelp_module.SymSetOptions( SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS | SYMOPT_LOAD_LINES );//| SYMOPT_DEBUG );
					if( ::rux::engine::_globals->_dbghelp_module.SymInitialize( ::rux::engine::_globals->_current_process_handle , 0 , TRUE ) == 0 )
					{
						if( GetLastError() == 87 )
							::rux::engine::_globals->_is_call_stack_initialized = 2;
					}
					else
						::rux::engine::_globals->_is_call_stack_initialized = 1;
					if( ::rux::engine::_globals->_is_call_stack_initialized )
					{
						::rux::string symsrv( ::rux::engine::_globals->_symsrv );
						symsrv.replace_all( "$(origin)" , ::rux::engine::_globals->_executable_directory );
						symsrv.replace_all( '/' , '\\' );
						char search_path[ 1024 ] = {0};
						search_path[ 0 ] = 0;
						::rux::engine::_globals->_dbghelp_module.SymGetSearchPath( ::rux::engine::_globals->_current_process_handle , search_path , 1024 );
						if( symsrv._str[ symsrv.length() - 1 ] != ';' )
							symsrv += ";";
						symsrv += search_path;
						if( ::rux::engine::_globals->_dbghelp_module.SymSetSearchPath( ::rux::engine::_globals->_current_process_handle , symsrv._str ) == 0 )
							::rux::log::WriteError( "%s, %s:%d, SymSetSearchPath, error %u" , __FUNCTION__ , __FILE__ , __LINE__ , (::rux::uint32)GetLastError() );
					}
				}				
			}
		#endif
			::rux::debug::_cs->UnLock();
		};
		dll_internal void deinitialize( void )
		{
			CS_PTR_LOCK( ::rux::debug::_cs );
		#ifdef __WINDOWS__
				if( ::rux::engine::_globals->_is_call_stack_initialized == 1 )
					::rux::engine::_globals->_dbghelp_module.SymCleanup( ::rux::engine::_globals->_current_process_handle );
				::rux::engine::_globals->_is_call_stack_initialized = 0;
		#endif
			::rux::debug::_cs->UnLock();
		};
		dll_internal void StartCatchException( ::rux::rux_exception_event_t rux_exception_event )
		{
			::rux::engine::_globals->_exception_event = rux_exception_event;
		#ifdef __WINDOWS__
			AddVectoredExceptionHandler( TRUE , vectored_exception_handler );
			SetUnhandledExceptionFilter( exception_filter );	
		#elif defined( __UNIX__ )
			struct sigaction act;
			memset( &act , 0 , sizeof( act ) );	
			act.sa_sigaction = debug_posix_death_signal2;
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
			sigaction( SIGSEGV , &act , &::rux::engine::_globals->_old_sigactions[ 0 ] );
			sigaction( SIGILL , &act , &::rux::engine::_globals->_old_sigactions[ 1 ] );
			sigaction( SIGFPE , &act , &::rux::engine::_globals->_old_sigactions[ 2 ] );	
			sigaction( SIGABRT , &act , &::rux::engine::_globals->_old_sigactions[ 4 ] );	
			sigaction( SIGBUS , &act , &::rux::engine::_globals->_old_sigactions[ 6 ] );
			sigaction( SIGHUP , &act , &::rux::engine::_globals->_old_sigactions[ 7 ] );
			signal( SIGPIPE , SIG_IGN );
			sigaction( SIGTSTP , &act , &::rux::engine::_globals->_old_sigactions[ 10 ] );
			sigaction( SIGTTIN , &act , &::rux::engine::_globals->_old_sigactions[ 11 ] );
			sigaction( SIGTTOU , &act , &::rux::engine::_globals->_old_sigactions[ 12 ] );
			sigaction( SIGUSR1 , &act , &::rux::engine::_globals->_old_sigactions[ 13 ] );
			sigaction( SIGUSR2 , &act , &::rux::engine::_globals->_old_sigactions[ 14 ] );	
			sigaction( SIGPROF , &act , &::rux::engine::_globals->_old_sigactions[ 16 ] );
			sigaction( SIGSYS , &act , &::rux::engine::_globals->_old_sigactions[ 17 ] );	
			sigaction( SIGVTALRM , &act , &::rux::engine::_globals->_old_sigactions[ 19 ] );
			sigaction( SIGXCPU , &act , &::rux::engine::_globals->_old_sigactions[ 20 ] );
			sigaction( SIGXFSZ , &act , &::rux::engine::_globals->_old_sigactions[ 21 ] );
	
			memset( &act , 0 , sizeof( act ) );	
			act.sa_sigaction = debug_posix_alarm_signal2;
			act.sa_mask = set;
			act.sa_flags = SA_ONSTACK | SA_SIGINFO;
			sigaction( SIGALRM , &act , &::rux::engine::_globals->_old_sigactions[ 5 ] );
		#endif
		};
		dll_internal void send_to_exception_server( void )
		{	
			declare_stack_variable( char , date , 64 );
			::rux::uint64 now = rux_get_now_as_local_unix_time();
			rux_unix_time_to_string( now , date , 64 );
			::rux::string::replace_all( date , ':' , '_' );
			::rux::string::replace_all( date , ' ' , '_' );
			::rux::string::replace_all( date , '.' , '_' );
			declare_stack_variable( char , commands , 4096 );
			rux_concatenation( commands , "--out \"" , ::rux::engine::_globals->_executable_directory );
			rux_concatenation( commands , "/" );
			rux_concatenation( commands , ::rux::engine::_globals->_hostname );
			rux_concatenation( commands , "(" );
			rux_concatenation( commands , date );
			rux_concatenation( commands , ").zip\"" );
			rux_concatenation( commands , " --command args_from_file --in \"" );
			rux_concatenation( commands , ::rux::engine::_globals->_executable_directory );
			rux_concatenation( commands , "/" );
			rux_concatenation( commands , ::rux::engine::_globals->_executable );
			rux_concatenation( commands , ".exception\" \"" );
			rux_concatenation( commands , ::rux::engine::_globals->_executable_directory );
			rux_concatenation( commands , "/rux." );
			rux_concatenation( commands , ::rux::engine::_globals->_executable );
			rux_concatenation( commands , ".exception\" \"" );
			rux_concatenation( commands , ::rux::engine::_globals->_executable_directory );
			rux_concatenation( commands , "/stable\" \"" );
			rux_concatenation( commands , ::rux::engine::_globals->_executable_directory );
			rux_concatenation( commands , "/memory\" \"" );
			rux_concatenation( commands , ::rux::engine::_globals->_executable_directory );
			rux_concatenation( commands , "/cpu\"" );	
			if( ::rux::diagnostics::process::start_with_redirect( ::rux::engine::_globals->_ruxiver_filename , commands , NULL , NULL , NULL , 0 , 0 , 0 ) == 0 )
			{		
				/*commands[ 0 ] = 0;
				rux_concatenation( commands , "--in \"" , ::rux::engine::_globals->_executable_directory );
				rux_concatenation( commands , "/" );
				rux_concatenation( commands , ::rux::engine::_globals->_hostname );
				rux_concatenation( commands , "(" );
				rux_concatenation( commands , date );
				rux_concatenation( commands , ").zip\"" );
				rux_concatenation( commands , " --command args_from_file" );		
				::rux::diagnostics::process::start_with_redirect( ::rux::engine::_globals->_neficl_filename , commands , NULL , NULL , NULL , 0 , 0 , 0 );*/
			}
		};
		#ifdef __WINDOWS__
		dll_internal void create_mini_dump( const char* filename , _EXCEPTION_POINTERS *ExceptionInfo )
		{
			if( ::rux::engine::_globals->_dbghelp_module._MiniDumpWriteDump )
			{
				rux_remove_file( filename );
				if( rux_is_exists_file( filename ) == 0 )
				{
					HANDLE file = CreateFile( filename , GENERIC_READ | GENERIC_WRITE , 0 , NULL , CREATE_ALWAYS , FILE_ATTRIBUTE_NORMAL , NULL ); 
					if( file != NULL && file != INVALID_HANDLE_VALUE ) 
					{
						MINIDUMP_EXCEPTION_INFORMATION mdei; 
						mdei.ThreadId = GetCurrentThreadId(); 
						mdei.ExceptionPointers = ExceptionInfo; 
						mdei.ClientPointers = TRUE; 
						MINIDUMP_TYPE mdt = MiniDumpNormal; 
						::rux::engine::_globals->_dbghelp_module.MiniDumpWriteDump( ::rux::engine::_globals->_current_process_handle , GetCurrentProcessId() , file , mdt , ( ExceptionInfo != 0 ) ? &mdei : 0 , 0 , 0 );
						CloseHandle( file ); 				
					}
				}
			}
		};
#define NO_DEBUG_EXCEPTION_MECHANISM 1
		dll_internal LONG __stdcall vectored_exception_handler( _EXCEPTION_POINTERS *ExceptionInfo )
		{
#if NO_DEBUG_EXCEPTION_MECHANISM
			if( IsDebuggerPresent() )
				return EXCEPTION_CONTINUE_SEARCH;
			else
#endif
			{
				if( ExceptionInfo->ExceptionRecord )
				{
#if NO_DEBUG_EXCEPTION_MECHANISM
					if( ExceptionInfo->ExceptionRecord->ExceptionCode == 0xc0000005
						|| ExceptionInfo->ExceptionRecord->ExceptionCode == 0xc0000374 )
#endif
						return exception_filter( ExceptionInfo );
#if NO_DEBUG_EXCEPTION_MECHANISM
					else
						return EXCEPTION_CONTINUE_SEARCH;
#endif
				}
				else
					return EXCEPTION_CONTINUE_SEARCH;
			}
		};
		dll_internal LONG __stdcall exception_filter( _EXCEPTION_POINTERS * info )
		{	
			if( info->ExceptionRecord->ExceptionCode != 0x406d1388
				&& info->ExceptionRecord->ExceptionCode != 0x40010006 )
			{	
				if( ::rux::engine::_globals->_exception_raised == 0 )
				{
					::rux::engine::_globals->_exception_raised = 1;		
					::rux::engine::_globals->_exception_time = rux_get_now_as_local_unix_time();
					::rux::engine::_globals->_exception_tid = ::rux::threading::XThread::get_CurrentThreadId();
					char exception_code_string[ 64 ] = {0};
					rux::string::uint64_to_hex_string( info->ExceptionRecord->ExceptionCode , exception_code_string );
					ExceptionToFile( exception_code_string , info );
					if( ::rux::engine::_globals->_exception_event )
						::rux::engine::_globals->_exception_event( ::rux::engine::_globals->_exception_tid );
					if( ::rux::engine::_globals->_is_core_dump == 1 )
					{
						char filename[ 4096 ] = {0};
						::rux::safe_strncat( filename , ::rux::engine::_globals->_executable_directory , 4096 );
						::rux::safe_strncat( filename , "/rux." , 4096 );
						::rux::safe_strncat( filename , ::rux::engine::_globals->_executable , 4096 );
						::rux::safe_strncat( filename , ".dmp" , 4096 );
						create_mini_dump( filename , info );
					}
					send_to_exception_server();
					TerminateProcess(::rux::engine::_globals->_current_process_handle, 1112);
					return EXCEPTION_EXECUTE_HANDLER;
				}	
				else
				{
					::rux::uint32 tid = ::rux::threading::XThread::get_CurrentThreadId();
					if( ::rux::engine::_globals->_exception_tid == tid
						|| ( ::rux::engine::_globals->_exception_time != 0ULL 
						&& rux_get_now_as_local_unix_time() - ::rux::engine::_globals->_exception_time >= 10000000ULL ) )
						TerminateProcess(::rux::engine::_globals->_current_process_handle, 1112);
					::rux::threading::XThread::Sleep( ::rux::threading::infinite );
				}
				return EXCEPTION_EXECUTE_HANDLER;
			}
			return EXCEPTION_CONTINUE_EXECUTION;
		};
		#endif
		dll_internal void on_lsof_stdout_or_stderr_handler( void* udata , const char* data , size_t data_size )
		{
			::rux::int32 exception_file = *((::rux::int32*)udata);
			if( data && data_size > 0 )
				rux_write( exception_file , data , data_size );
		};
		dll_internal void CheckFileDescriptors( const char* exception_filename , char* exception_text )
		{
			declare_stack_variable( char , exec_dir , 1024 );
			rux_concatenation( exec_dir , ::rux::engine::_globals->_executable_directory , "/" );
			rux_concatenation( exec_dir , "/" );
			rux_concatenation( exec_dir , ::rux::engine::_globals->_executable );
			::rux::int32 file = rux_open( exec_dir , O_RDONLY );
			if( file == -1 )
			{
				if( errno == EMFILE )
				{
					rux::int32 max_descriptors = ::rux::diagnostics::process::get_max_descriptors();
					for( ::rux::int32 fd = max_descriptors - 10 ; fd < max_descriptors ; fd++ )
						close( fd );

					::rux::uint8 is_exists = rux_is_exists_file( exception_filename );
					::rux::int32 exception_file = -1;
					if( is_exists == 1 )
						exception_file = rux_open( exception_filename , O_WRONLY | O_APPEND );
					else
						exception_file = rux_open( exception_filename , O_CREAT | O_WRONLY | O_TRUNC );
					if( exception_file != -1 )
					{
						if( exception_text[ 0 ] != 0 )
						{
							rux_write( exception_file , exception_text , strlen( exception_text ) );
							::rux::XConsole::WriteToStdErr( exception_text );
							exception_text[ 0 ] = 0;
						}
		#ifdef __UNIX__
						if( rux_is_exists_file_or_link( "/usr/bin/lsof" ) )
						{
							::rux::pid_t pid = ::rux::diagnostics::process::get_process_id();
							declare_stack_variable( char , pid_string , 64 );
							rux::string::uint32_to_string( pid , pid_string );
				
							rux_write( exception_file , "{OPEN_MAX} file descriptors are currently open in the calling process.lsof info:" , 80 );

							char* args[ 2 ] = {0};
							args[ 0 ] = const_cast< char* >( "-p" );
							args[ 1 ] = pid_string;
							if( ::rux::engine::_globals->_alarm_is_addr2line )
								*(::rux::engine::_globals->_alarm_is_addr2line) = 1;
							alarm( 5 );
							::rux::diagnostics::process::start_with_redirect( "/usr/bin/lsof" , args , 2 , on_lsof_stdout_or_stderr_handler , on_lsof_stdout_or_stderr_handler , &exception_file , 0 , 0 , NULL );
							if( ::rux::engine::_globals->_exception_raised )
								alarm( 5 );
							else
								alarm( 0 );
							if( ::rux::engine::_globals->_alarm_is_addr2line )
								*(::rux::engine::_globals->_alarm_is_addr2line) = 0;
						}
		#endif
						close( exception_file );
					}
					for( ::rux::int32 fd = max_descriptors / 2 ; fd < max_descriptors ; fd++ )
						close( fd );
				}
			}
			else
				close( file );
		};
#ifdef __WINDOWS__
		dll_internal void ExceptionToFile( const char* exception_code_string , _EXCEPTION_POINTERS* info )
#else
		dll_internal void ExceptionToFile( const char* exception_code_string )
#endif
		{
			::rux::uint32 tid = ::rux::threading::XThread::get_CurrentThreadId();
			::rux::pid_t pid = ::rux::diagnostics::process::get_process_id();
			char tid_string[ 64 ] = {0};
			char pid_string[ 64 ] = {0};
			rux::string::uint32_to_string( tid , tid_string );
			rux::string::uint32_to_string( pid , pid_string );

			::rux::uint64 now = rux_get_now_as_local_unix_time();
			char now_string[ 64 ] = {0};
			rux_unix_time_to_string( now , now_string , 64 );

			char exception_filename[ 2048 ] = {0};
			::rux::safe_strncpy( exception_filename , ::rux::engine::_globals->_executable_directory , 2048 );
			::rux::safe_strncat( exception_filename , "/" , 2048 );
			::rux::safe_strncat( exception_filename , ::rux::engine::_globals->_executable , 2048 );
			::rux::safe_strncat( exception_filename , ".exception" , 2048 );
	
			char exception_text[ 2048 ] = {0};	
			::rux::safe_strncpy( exception_text , "\n======= EXCEPTION, code '" , 2048 );
			::rux::safe_strncat( exception_text , exception_code_string , 2048 );
			::rux::safe_strncat( exception_text , "', " , 2048 );
			::rux::safe_strncat( exception_text , now_string , 2048 );
			::rux::safe_strncat( exception_text , " =======\nProcess Id:" , 2048 );
			::rux::safe_strncat( exception_text , pid_string , 2048 );
			::rux::safe_strncat( exception_text , "\nThread Id:" , 2048 );
			::rux::safe_strncat( exception_text , tid_string , 2048 );

			::rux::safe_strncat( exception_text , "\n-------------------------\n" , 2048 );
			::rux::safe_strncat( exception_text , "Working set " , 2048 );
			::rux::int64 temp = ::rux::diagnostics::process_info::working_set( ::rux::engine::_globals->_executable , 0 );
			rux::string::uint64_to_string( temp , pid_string );
			::rux::safe_strncat( exception_text , pid_string , 2048 );
			::rux::safe_strncat( exception_text , " bytes" , 2048 );

			::rux::safe_strncat( exception_text , "\nVirtual bytes " , 2048 );
			temp = ::rux::diagnostics::process_info::virtual_bytes( ::rux::engine::_globals->_executable , 0 );
			rux::string::uint64_to_string( temp , pid_string );
			::rux::safe_strncat( exception_text , pid_string , 2048 );
			::rux::safe_strncat( exception_text , " bytes" , 2048 );

			::rux::safe_strncat( exception_text , "\nThreads " , 2048 );
			temp = ::rux::diagnostics::process_info::threads_count( pid , 0 );
			rux::string::uint64_to_string( temp , pid_string );
			::rux::safe_strncat( exception_text , pid_string , 2048 );
			::rux::safe_strncat( exception_text , "\n-------------------------\n" , 2048 );

			::rux::safe_strncat( exception_text , "Stack:\n" , 2048 );
	
			::rux::uint64 size = rux_get_file_size( exception_filename );
			if( size >= 1024ULL * 1024ULL )
				rux_remove_file( exception_filename );
			::rux::uint8 is_exists = rux_is_exists_file( exception_filename );
			::rux::int32 exception_file = -1;
			if( is_exists == 1 )
				exception_file = rux_open( exception_filename , O_WRONLY | O_APPEND );
			else
				exception_file = rux_open( exception_filename , O_CREAT | O_WRONLY | O_TRUNC );
			if( exception_file == -1 )
			{
				CheckFileDescriptors( exception_filename , exception_text );
				is_exists = rux_is_exists_file( exception_filename );
				if( is_exists == 1 )
					exception_file = rux_open( exception_filename , O_WRONLY | O_APPEND );
				else
					exception_file = rux_open( exception_filename , O_CREAT | O_WRONLY | O_TRUNC );
			}	
			if( exception_file != -1 )
			{
				rux_lseek( exception_file , 0L , SEEK_END );
				if( exception_text[ 0 ] != 0 )
				{
					rux_write( exception_file , exception_text , strlen( exception_text ) );
					::rux::XConsole::WriteToStdErr( exception_text );
				}
				exception_text[ 0 ] = 0;
				char callstack_ptr[ 4096 ] = {0};
#ifdef __WINDOWS__
				get_CallStack( callstack_ptr , 4096 , info , 0 );
#else
				get_CallStack( callstack_ptr , 4096 , 1 );
#endif
				if( callstack_ptr[ 0 ] != 0 )
				{			
					rux_write( exception_file , callstack_ptr , strlen( callstack_ptr ) );
					::rux::XConsole::WriteToStdErr( callstack_ptr );
					::rux::safe_strncat( exception_text , "\n" , 2048 );
				}
				else
					::rux::safe_strncat( exception_text , "empty\n" , 2048 );
				::rux::safe_strncat( exception_text , "\n=========================\n" , 2048 );
				rux_write( exception_file , exception_text , strlen( exception_text ) );
				::rux::XConsole::WriteToStdErr( exception_text );
				rux_close( exception_file );
				::chmod( exception_filename , 0777 );
			}
		};
#ifdef __UNIX__
#ifdef __SOLARIS__
		dll_internal void debug_posix_death_signal2( ::rux::int32 signum , siginfo_t* siginfo , void* context )
#else
		dll_internal void debug_posix_death_signal2( ::rux::int32 signum , siginfo_t* siginfo , void* context )
#endif
		{	
			if( ::rux::engine::_globals->_exception_raised == 0 )
			{
				::rux::engine::_globals->_exception_raised = 1;
				alarm( 10 );
				::rux::uint8 is_kill = 1;
				char exception_code_string[ 16 ] = {0};
				switch( signum )
				{
				case SIGSEGV: strcpy( exception_code_string , "SIGSEGV" );break;
				case SIGILL: strcpy( exception_code_string , "SIGILL" );break;
				case SIGFPE: strcpy( exception_code_string , "SIGFPE" );break;
				case SIGABRT: strcpy( exception_code_string , "SIGABRT" );break;
				case SIGBUS: strcpy( exception_code_string , "SIGBUS" );break;
				case SIGHUP: strcpy( exception_code_string , "SIGHUP" );break;
				case SIGPIPE: strcpy( exception_code_string , "SIGPIPE" );break;
				case SIGTSTP: strcpy( exception_code_string , "SIGTSTP" );break;
				case SIGTTIN: strcpy( exception_code_string , "SIGTTIN" );break;
				case SIGTTOU: strcpy( exception_code_string , "SIGTTOU" );break;
				case SIGUSR1: strcpy( exception_code_string , "SIGUSR1" );break;
				case SIGUSR2: strcpy( exception_code_string , "SIGUSR2" );break;
				case SIGPROF: strcpy( exception_code_string , "SIGPROF" );break;
				case SIGSYS: strcpy( exception_code_string , "SIGSYS" );break;
				case SIGVTALRM: strcpy( exception_code_string , "SIGVTALRM" );break;
				case SIGXCPU: strcpy( exception_code_string , "SIGXCPU" );break;
				case SIGXFSZ: strcpy( exception_code_string , "SIGXFSZ" );break;
				}
				ExceptionToFile( exception_code_string );
				if( ::rux::engine::_globals->_exception_event )
					::rux::engine::_globals->_exception_event( ::rux::threading::XThread::get_CurrentThreadId() );
				::rux::uint8 is_raised = 0;
				switch( signum )
				{
				case SIGSEGV: if( ::rux::engine::_globals->_old_sigactions[ 0 ].sa_sigaction && ::rux::engine::_globals->_old_sigactions[ 0 ].sa_sigaction != debug_posix_death_signal2 ) ::rux::engine::_globals->_old_sigactions[ 0 ].sa_sigaction( signum , siginfo , context );is_raised = 1;break;
				case SIGILL: if( ::rux::engine::_globals->_old_sigactions[ 1 ].sa_sigaction && ::rux::engine::_globals->_old_sigactions[ 1 ].sa_sigaction != debug_posix_death_signal2 ) ::rux::engine::_globals->_old_sigactions[ 1 ].sa_sigaction( signum , siginfo , context );is_raised = 1;break;
				case SIGFPE: if( ::rux::engine::_globals->_old_sigactions[ 2 ].sa_sigaction && ::rux::engine::_globals->_old_sigactions[ 2 ].sa_sigaction != debug_posix_death_signal2 ) ::rux::engine::_globals->_old_sigactions[ 2 ].sa_sigaction( signum , siginfo , context );is_raised = 1;break;    
				case SIGABRT: if( ::rux::engine::_globals->_old_sigactions[ 4 ].sa_sigaction && ::rux::engine::_globals->_old_sigactions[ 4 ].sa_sigaction != debug_posix_death_signal2 ) ::rux::engine::_globals->_old_sigactions[ 4 ].sa_sigaction( signum , siginfo , context );is_raised = 1;break;		
				case SIGBUS: if( ::rux::engine::_globals->_old_sigactions[ 6 ].sa_sigaction && ::rux::engine::_globals->_old_sigactions[ 6 ].sa_sigaction != debug_posix_death_signal2 ) ::rux::engine::_globals->_old_sigactions[ 6 ].sa_sigaction( signum , siginfo , context );is_raised = 1;break;
				case SIGHUP: is_kill = 1;if( ::rux::engine::_globals->_old_sigactions[ 7 ].sa_sigaction && ::rux::engine::_globals->_old_sigactions[ 7 ].sa_sigaction != debug_posix_death_signal2 ) ::rux::engine::_globals->_old_sigactions[ 7 ].sa_sigaction( signum , siginfo , context );is_raised = 1;break;
				case SIGPIPE: if( ::rux::engine::_globals->_old_sigactions[ 8 ].sa_sigaction && ::rux::engine::_globals->_old_sigactions[ 8 ].sa_sigaction != debug_posix_death_signal2 ) ::rux::engine::_globals->_old_sigactions[ 8 ].sa_sigaction( signum , siginfo , context );is_raised = 1;break;    
				case SIGTSTP: if( ::rux::engine::_globals->_old_sigactions[ 10 ].sa_sigaction && ::rux::engine::_globals->_old_sigactions[ 10 ].sa_sigaction != debug_posix_death_signal2 ) ::rux::engine::_globals->_old_sigactions[ 10 ].sa_sigaction( signum , siginfo , context );is_raised = 1;break;
				case SIGTTIN: if( ::rux::engine::_globals->_old_sigactions[ 11 ].sa_sigaction && ::rux::engine::_globals->_old_sigactions[ 11 ].sa_sigaction != debug_posix_death_signal2 ) ::rux::engine::_globals->_old_sigactions[ 11 ].sa_sigaction( signum , siginfo , context );is_raised = 1;break;
				case SIGTTOU: if( ::rux::engine::_globals->_old_sigactions[ 12 ].sa_sigaction && ::rux::engine::_globals->_old_sigactions[ 12 ].sa_sigaction != debug_posix_death_signal2 ) ::rux::engine::_globals->_old_sigactions[ 12 ].sa_sigaction( signum , siginfo , context );is_raised = 1;break;
				case SIGUSR1: if( ::rux::engine::_globals->_old_sigactions[ 13 ].sa_sigaction && ::rux::engine::_globals->_old_sigactions[ 13 ].sa_sigaction != debug_posix_death_signal2 ) ::rux::engine::_globals->_old_sigactions[ 13 ].sa_sigaction( signum , siginfo , context );is_raised = 1;break;
				case SIGUSR2: if( ::rux::engine::_globals->_old_sigactions[ 14 ].sa_sigaction && ::rux::engine::_globals->_old_sigactions[ 14 ].sa_sigaction != debug_posix_death_signal2 ) ::rux::engine::_globals->_old_sigactions[ 14 ].sa_sigaction( signum , siginfo , context );is_raised = 1;break;		
				case SIGPROF: if( ::rux::engine::_globals->_old_sigactions[ 16 ].sa_sigaction && ::rux::engine::_globals->_old_sigactions[ 16 ].sa_sigaction != debug_posix_death_signal2 ) ::rux::engine::_globals->_old_sigactions[ 16 ].sa_sigaction( signum , siginfo , context );is_raised = 1;break;
				case SIGSYS: if( ::rux::engine::_globals->_old_sigactions[ 17 ].sa_sigaction && ::rux::engine::_globals->_old_sigactions[ 17 ].sa_sigaction != debug_posix_death_signal2 ) ::rux::engine::_globals->_old_sigactions[ 17 ].sa_sigaction( signum , siginfo , context );is_raised = 1;break;		
				case SIGVTALRM: if( ::rux::engine::_globals->_old_sigactions[ 19 ].sa_sigaction && ::rux::engine::_globals->_old_sigactions[ 19 ].sa_sigaction != debug_posix_death_signal2 ) ::rux::engine::_globals->_old_sigactions[ 19 ].sa_sigaction( signum , siginfo , context );is_raised = 1;break;
				case SIGXCPU: if( ::rux::engine::_globals->_old_sigactions[ 20 ].sa_sigaction && ::rux::engine::_globals->_old_sigactions[ 20 ].sa_sigaction != debug_posix_death_signal2 ) ::rux::engine::_globals->_old_sigactions[ 20 ].sa_sigaction( signum , siginfo , context );is_raised = 1;break;
				case SIGXFSZ: if( ::rux::engine::_globals->_old_sigactions[ 21 ].sa_sigaction && ::rux::engine::_globals->_old_sigactions[ 21 ].sa_sigaction != debug_posix_death_signal2 ) ::rux::engine::_globals->_old_sigactions[ 21 ].sa_sigaction( signum , siginfo , context );is_raised = 1;break;
				}	
				switch( signum )
				{
				case SIGSEGV: if( ::rux::engine::_globals->_old_sigactions[ 0 ].sa_handler ) ::rux::engine::_globals->_old_sigactions[ 0 ].sa_handler( signum );is_raised = 1;break;
				case SIGILL: if( ::rux::engine::_globals->_old_sigactions[ 1 ].sa_handler ) ::rux::engine::_globals->_old_sigactions[ 1 ].sa_handler( signum );is_raised = 1;break;
				case SIGFPE: if( ::rux::engine::_globals->_old_sigactions[ 2 ].sa_handler ) ::rux::engine::_globals->_old_sigactions[ 2 ].sa_handler( signum );is_raised = 1;break;    
				case SIGABRT: if( ::rux::engine::_globals->_old_sigactions[ 4 ].sa_handler ) ::rux::engine::_globals->_old_sigactions[ 4 ].sa_handler( signum );is_raised = 1;break;		
				case SIGBUS: if( ::rux::engine::_globals->_old_sigactions[ 6 ].sa_handler ) ::rux::engine::_globals->_old_sigactions[ 6 ].sa_handler( signum );is_raised = 1;break;
				case SIGHUP: is_kill = 1;if( ::rux::engine::_globals->_old_sigactions[ 7 ].sa_handler ) ::rux::engine::_globals->_old_sigactions[ 7 ].sa_handler( signum );is_raised = 1;break;
				case SIGPIPE: if( ::rux::engine::_globals->_old_sigactions[ 8 ].sa_handler ) ::rux::engine::_globals->_old_sigactions[ 8 ].sa_handler( signum );is_raised = 1;break;    
				case SIGTSTP: if( ::rux::engine::_globals->_old_sigactions[ 10 ].sa_handler ) ::rux::engine::_globals->_old_sigactions[ 10 ].sa_handler( signum );is_raised = 1;break;
				case SIGTTIN: if( ::rux::engine::_globals->_old_sigactions[ 11 ].sa_handler ) ::rux::engine::_globals->_old_sigactions[ 11 ].sa_handler( signum );is_raised = 1;break;
				case SIGTTOU: if( ::rux::engine::_globals->_old_sigactions[ 12 ].sa_handler ) ::rux::engine::_globals->_old_sigactions[ 12 ].sa_handler( signum );is_raised = 1;break;
				case SIGUSR1: if( ::rux::engine::_globals->_old_sigactions[ 13 ].sa_handler ) ::rux::engine::_globals->_old_sigactions[ 13 ].sa_handler( signum );is_raised = 1;break;
				case SIGUSR2: if( ::rux::engine::_globals->_old_sigactions[ 14 ].sa_handler ) ::rux::engine::_globals->_old_sigactions[ 14 ].sa_handler( signum );is_raised = 1;break;		
				case SIGPROF: if( ::rux::engine::_globals->_old_sigactions[ 16 ].sa_handler ) ::rux::engine::_globals->_old_sigactions[ 16 ].sa_handler( signum );is_raised = 1;break;
				case SIGSYS: if( ::rux::engine::_globals->_old_sigactions[ 17 ].sa_handler ) ::rux::engine::_globals->_old_sigactions[ 17 ].sa_handler( signum );is_raised = 1;break;		
				case SIGVTALRM: if( ::rux::engine::_globals->_old_sigactions[ 19 ].sa_handler ) ::rux::engine::_globals->_old_sigactions[ 19 ].sa_handler( signum );is_raised = 1;break;
				case SIGXCPU: if( ::rux::engine::_globals->_old_sigactions[ 20 ].sa_handler ) ::rux::engine::_globals->_old_sigactions[ 20 ].sa_handler( signum );is_raised = 1;break;
				case SIGXFSZ: if( ::rux::engine::_globals->_old_sigactions[ 21 ].sa_handler ) ::rux::engine::_globals->_old_sigactions[ 21 ].sa_handler( signum );is_raised = 1;break;
				}	
				if( is_kill == 1 )
				{
					send_to_exception_server();
					abort();
					kill( getpid() , SIGKILL );
				}
				alarm( 0 );
			}
		};
#endif
#ifdef __UNIX__
#ifdef __SOLARIS__
		dll_internal void debug_posix_alarm_signal2( ::rux::int32 signum , siginfo_t* siginfo , void* context )
#else
		dll_internal void debug_posix_alarm_signal2( ::rux::int32 signum , siginfo_t* siginfo , void* context )
#endif
		{
			if( ::rux::engine::_globals->_alarm_is_addr2line && *(::rux::engine::_globals->_alarm_is_addr2line ) )
				*(::rux::engine::_globals->_alarm_is_addr2line) = 0;
			else if( ::rux::engine::_globals->_exception_raised == 1 )
			{
				abort();
				kill( getpid() , SIGKILL );	
			}
		};
#endif
		dll_internal void RegisterExceptionEvent( const char* module_name , ::rux::rux_exception_event_t exception_event )
		{
			::rux::engine::_globals->_rux_memory_register_exception_event( module_name , exception_event );
		};
		dll_internal void UnRegisterExceptionEvent( ::rux::rux_exception_event_t exception_event )
		{
			::rux::engine::_globals->_rux_memory_unregister_exception_event( exception_event );
		};
	};
};
