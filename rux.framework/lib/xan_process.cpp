#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_process.h>
#include <xan_file.h>
#include <xan_error.h>
#include <xan_uint32.h>
#ifdef __WINDOWS__ 
#include <aclapi.h>
#include <WtsApi32.h>
#include <UserEnv.h>
#include <Psapi.h>
#define GENERIC_ALL_ACCESS 0x10000000
#endif
#include <xan_keyvaluepair.h>
#include <xan_path.h>
#include <xan_directory.h>
#include <xan_pdh.h>
#include <xan_uint64.h>
#include <xan_boolean.h>
begin_implement_rux_class_ns( Process , rux::diagnostics );
end_implement_rux_class();
namespace rux
{
	namespace diagnostics
	{		
		void XProcess::set_ModuleName( const rux::XString& module_name )
		{
			(*this)()->_module_name = module_name;
		};
		uint8 XProcess::StartWithoutWait( XEnumProcessStartMethod process_start_method , const XArray< rux::XString >& arguments , rux::XString& error , char* user_name , char** env , size_t env_size )
		{
			error.Clear();
			uint32 result = 0;
			if( (*this)()->_module_name.Length() > 0 )
			{
		#ifdef __WINDOWS__
				if( process_start_method == XEnumProcessStartMethod_CreateProcessAsUser )
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
												int32 SessionId = WTSGetActiveConsoleSessionId();
												if (SessionId != 0xffffffff)
												{                        
													if ( ::rux::engine::_globals->_wtsapi32_module.WTSQueryUserToken(SessionId, &Token))
													{                          
														SECURITY_ATTRIBUTES sa;
														memset( &sa , 0 , sizeof( sa ) );
														sa.nLength = sizeof( sa );
														if (DuplicateTokenEx(                          Token,GENERIC_ALL_ACCESS,&sa,														SecurityIdentification,                            TokenPrimary,&DupedToken                            ) )
														{                            
															if (SetTokenInformation(DupedToken, TokenSessionId, &SessionId, sizeof(uint32)))
															{                              
																if ( ::rux::engine::_globals->_userenv_module.CreateEnvironmentBlock(&environment, DupedToken, FALSE))
																{
																	try
																	{
																		STARTUPINFOW si;
																		memset( &si , 0 , sizeof( si ) );
																		si.cb = sizeof( si );
																		si.lpReserved = NULL;
																		si.lpDesktop = L"winsta0\\default";
																		si.lpTitle = NULL;
																		si.cbReserved2 = 0;
																		si.lpReserved2 = NULL;
																		rux::XString utf16_argument = (*this)()->_module_name;
																		for( uint32 index0 = 0 ; index0 < arguments.Count() ; index0++ )
																		{
																			utf16_argument += " " + arguments[ index0 ];
																		}
																		utf16_argument.set_ByRef( utf16_argument.ConvertToUTF16() );
																		int32 result =CreateProcessAsUserW( DupedToken , NULL , (wchar_t*)utf16_argument.str() , &sa , &sa , false , CREATE_DEFAULT_ERROR_MODE | CREATE_UNICODE_ENVIRONMENT, environment, NULL , &si , &pi );
																		if( result )
																		{
																		}
																		else
																		{
																			//uint32 error_uint32 = GetLastError();                                      
																		}
																	}
																	catch ( ... )                                 
																	{                                    
																	}
																	RevertToSelf();															                       
																}
																else
																{
																	uint32 error_uint32 = GetLastError();
																	rux_write_error( "CreateEnvironmentBlock failed,error = " + XUInt32( error_uint32 ).ToString() , error );
																}
															}
															else
															{
																uint32 error_uint32 =  GetLastError();
																rux_write_error( "SetTokenInformation failed,error = " + XUInt32( error_uint32 ).ToString() , error );
															}
														}
														else
														{
															uint32 error_uint32 =  GetLastError();
															rux_write_error( "DuplicateTokenEx failed,error = " + XUInt32( error_uint32 ).ToString() , error );
														}
													}
													else
													{
														uint32 error_uint32 = GetLastError();												
														rux_write_error( "WTSQueryUserToken failed,error = " + XUInt32( error_uint32 ).ToString() , error );
													}
												}
												else
												{
													uint32 error_uint32 =  GetLastError();
													rux_write_error( "WTSGetActiveConsoleSessionId failed,error = " + XUInt32( error_uint32 ).ToString() , error );
												}
											}
											else
											{
												uint32 error_uint32 =  GetLastError();
												rux_write_error( "SetSecurityInfo failed,error = " + XUInt32( error_uint32 ).ToString() , error );
											}
										}
										else
										{
											uint32 error_uint32 = GetLastError();
											rux_write_error( "GetSecurityInfo failed,error = " + XUInt32( error_uint32 ).ToString() , error );
										}
									}
									else
									{
										uint32 error_uint32 = GetLastError();
										rux_write_error( "OpenDesktop failed,error = " + XUInt32( error_uint32 ).ToString() , error );
									}
								}
								else
								{
									uint32 error_uint32 = GetLastError();
									rux_write_error( "SetProcessWindowStation failed,error = " + XUInt32( error_uint32 ).ToString() , error );
								}
							}
							else
							{
								uint32 error_uint32 = GetLastError();
								rux_write_error( "OpenWindowStation failed,error = " + XUInt32( error_uint32 ).ToString() , error );
							}
						}
						else
						{
							uint32 error_uint32 = GetLastError();
							rux_write_error( "GetProcessWindowStation failed,error = " + XUInt32( error_uint32 ).ToString() , error );
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
				else if( process_start_method == XEnumProcessStartMethod_CreateProcess )
				{
					rux::XString cmd_line = (*this)()->_module_name;
					for( uint32 index0 = 0 ; index0 < arguments.Count() ; index0++ )
					{
						cmd_line += " " + arguments[ index0 ];
					}			
					PROCESS_INFORMATION  process_information;
					memset( (void*)&process_information , 0 , sizeof( PROCESS_INFORMATION ) );			
					if( cmd_line()->_code_page == XEnumCodePage_UTF16 )
					{
						rux::XString utf16_module_name;
						utf16_module_name.set_ByRef( (*this)()->_module_name.ConvertToUTF16() );
						STARTUPINFOW startup_info;
						memset( (void*)&startup_info , 0 , sizeof( STARTUPINFOW ) );			
						startup_info.cb = sizeof( STARTUPINFOW );
						startup_info.dwFlags = STARTF_USESHOWWINDOW;
						startup_info.wShowWindow = SW_SHOW;
						if( CreateProcessW( (wchar_t*)utf16_module_name.str() , (wchar_t*)cmd_line.str() , NULL , NULL , FALSE , CREATE_DEFAULT_ERROR_MODE , NULL , NULL , &startup_info , &process_information ) == 0 )				
						{
							result = GetLastError();
							error = XError::ConvertErrorCodeToString( result , XEnum_ErrorType_GetLastError );
						}				
					}
					else if( cmd_line()->_code_page == XEnumCodePage_CP1251 )
					{
						rux::XString ansi_module_name;
						ansi_module_name.set_ByRef( (*this)()->_module_name.ConvertToAnsi() );
						STARTUPINFOA startup_info;
						memset( (void*)&startup_info , 0 , sizeof( STARTUPINFOA ) );			
						startup_info.cb = sizeof( STARTUPINFOA );
						startup_info.dwFlags = STARTF_USESHOWWINDOW;
						startup_info.wShowWindow = SW_SHOW;
						if( CreateProcessA( ansi_module_name.str() , cmd_line.str() , NULL , NULL , FALSE , CREATE_DEFAULT_ERROR_MODE , NULL , NULL , &startup_info , &process_information ) == 0 )				
						{
							result = GetLastError();
							error = XError::ConvertErrorCodeToString( result , XEnum_ErrorType_GetLastError );
						}
					}
					else
					{
						rux::XString utf16_module_name;
						utf16_module_name.set_ByRef( (*this)()->_module_name.ConvertToUTF16() );
						rux::XString utf16_cmd_line = cmd_line.ConvertToUTF16();
						STARTUPINFOW startup_info;
						memset( (void*)&startup_info , 0 , sizeof( STARTUPINFOW ) );			
						startup_info.cb = sizeof( STARTUPINFOW );
						startup_info.dwFlags = STARTF_USESHOWWINDOW;
						startup_info.wShowWindow = SW_SHOW;
						if( CreateProcessW( (wchar_t*)utf16_module_name.str() , (wchar_t*)utf16_cmd_line.str() , NULL , NULL , FALSE , CREATE_DEFAULT_ERROR_MODE , NULL , NULL , &startup_info , &process_information ) == 0 )				
						{
							result = GetLastError();
							error = XError::ConvertErrorCodeToString( result , XEnum_ErrorType_GetLastError );
						}				
					}
					if( process_information.hProcess )
						CloseHandle( process_information.hProcess );
					if( process_information.hThread )
						CloseHandle( process_information.hThread );
				}
				else if( process_start_method == XEnumProcessStartMethod_ShellExecute )
				{
					rux::XString utf16_filename;				
					rux::XString cmd_line;
					for( uint32 index0 = 0 ; index0 < arguments.Count() ; index0++ )
					{
						cmd_line += " " + arguments[ index0 ];
					}	
					cmd_line.set_ByRef( cmd_line.ConvertToUTF16() );
					utf16_filename.set_ByRef( (*this)()->_module_name.ConvertToUTF16() );
					ShellExecuteW( NULL , L"open" , (wchar_t*)utf16_filename.str() , (wchar_t*)cmd_line.str() , NULL , SW_SHOW );
				}
		#endif
		#ifdef __UNIX__
				rux::XString current_cp;
				current_cp.set_ByRef( (*this)()->_module_name.ConvertToAnsi() );
				declare_stack_variable( char , module_name , 4096 );
				if( ::rux::io::file::exists( current_cp.str() , 1 ) == 0 )
				{
					for( size_t index0 = 0 ; index0 < ::rux::engine::_globals->_env_variables_count ; index0++ )
					{
						::rux::safe_strncpy( module_name , ::rux::engine::_globals->_env_variables[ index0 ] , 4096 );
						::rux::string::replace_all( module_name , '\\' , '/' );
						size_t len = strlen( module_name );
						module_name[ len ] = '/';
						module_name[ len + 1 ] = 0;
						::rux::safe_strncat( module_name , current_cp.str() , 4096 );
						if( ::rux::io::file::exists( module_name , 1 ) == 1 )
							break;
					}
				}
				else
					::rux::safe_strncpy( module_name , current_cp.str() , 4096 );
				if( ::rux::io::file::exists( module_name , 1 ) == 1 )
				{
					pid_t pid = fork();
					if( pid < 0 )
					{
					}
					else if( pid != 0 )
					{	
						int status = 0;
						waitpid( pid , &status , 0 );
					}
					else
					{	
						pid_t fork_rv = fork();
						if( fork_rv == 0 )
						{
							char* process_env[ 512 ] = {0};
							process_env[ 0 ] = 0;
							if( env != 0 && env_size != 0 )
							{
								for( size_t process_env_index = 0 ; process_env_index < env_size ; process_env_index++ )
									process_env[ process_env_index ] = env[ process_env_index ];
								process_env[ env_size ] = 0;
							}
							if( user_name != 0 )
							{
								::rux::uint32 uid = 0 , gid = 0;
								if( ::rux::security::get_user_id( user_name , uid , gid ) )
								{
									setuid( uid );
									setgid( gid );
								}
							}
							char letter = 0;
							::rux::int32 module_name_last_index = ::rux::string::last_index_of( module_name , "/" );
							if( module_name_last_index != -1 )
							{
								letter = module_name[ module_name_last_index + 1 ];
								module_name[ module_name_last_index + 1 ] = 0;
							}
							umask( 0 );
							::rux::pid_t sid = setsid();
							if( sid < 0 )
								_exit( 1 );	
							if( module_name_last_index == -1 )
							{
								if( chdir( module_name ) < 0 )
									_exit( 1 );
							}
							else
							{
								if( chdir( "/" ) < 0 )
									_exit( 1 );
							}
							if( module_name_last_index != -1 )
								module_name[ module_name_last_index + 1 ] = letter;
							close( STDIN_FILENO );
							close( STDOUT_FILENO );
							close( STDERR_FILENO );
							::rux::int32 stdio_fd = open( "/dev/null" , O_RDWR );
							dup( stdio_fd );
							dup( stdio_fd );
							if( process_start_method == XEnumProcessStartMethod_ShellExecute )
							{
								declare_variable( ::rux::XString , cmd_args );
								cmd_args += module_name;
								for( size_t index0 = 0 ; index0 < arguments.Count() ; index0++ )
									cmd_args += " " + arguments[ index0 ];
								if( process_env[ 0 ] == 0 )
									execlp( "/bin/sh" , "/bin/sh" , "-c" , cmd_args.str() , (char*)0 );
								else
									execle( "/bin/sh" , "/bin/sh" , "-c" , cmd_args.str() , (char*)0 , process_env );
							}
							else
							{
								if( arguments.Count() == 0 )
								{
									if( process_env[ 0 ] == 0 )
										execlp( module_name , module_name , (char*)0 );
									else
										execle( module_name , module_name , (char*)0 , process_env );
								}
								else if( arguments.Count() == 1 )
								{
									if( process_env[ 0 ] == 0 )
										execlp( module_name , module_name , arguments[ 0 ].str() , (char*)0 );
									else
										execle( module_name , module_name , arguments[ 0 ].str() , (char*)0 , process_env );
								}
								else if( arguments.Count() == 2 )
								{
									if( process_env[ 0 ] == 0 )
										execlp( module_name , module_name , arguments[ 0 ].str() , arguments[ 1 ].str() , (char*)0 );
									else
										execle( module_name , module_name , arguments[ 0 ].str() , arguments[ 1 ].str() , (char*)0 , process_env );
								}
								else if( arguments.Count() == 3 )
								{
									if( process_env[ 0 ] == 0 )
										execlp( module_name , module_name , arguments[ 0 ].str() , arguments[ 1 ].str() , arguments[ 2 ].str() , (char*)0 );
									else
										execle( module_name , module_name , arguments[ 0 ].str() , arguments[ 1 ].str() , arguments[ 2 ].str() , (char*)0 , process_env );
								}
								else if( arguments.Count() == 4 )
								{
									if( process_env[ 0 ] == 0 )
										execlp( module_name , module_name , arguments[ 0 ].str() , arguments[ 1 ].str() , arguments[ 2 ].str() , arguments[ 3 ].str() , (char*)0 );
									else
										execle( module_name , module_name , arguments[ 0 ].str() , arguments[ 1 ].str() , arguments[ 2 ].str() , arguments[ 3 ].str() , (char*)0 , process_env );
								}
								else if( arguments.Count() == 5 )
								{
									if( process_env[ 0 ] == 0 )
										execlp( module_name , module_name , arguments[ 0 ].str() , arguments[ 1 ].str() , arguments[ 2 ].str() , arguments[ 3 ].str() , arguments[ 4 ].str() , (char*)0 );
									else
										execle( module_name , module_name , arguments[ 0 ].str() , arguments[ 1 ].str() , arguments[ 2 ].str() , arguments[ 3 ].str() , arguments[ 4 ].str() , (char*)0 , process_env );
								}
								else if( arguments.Count() == 6 )
								{
									if( process_env[ 0 ] == 0 )
										execlp( module_name , module_name , arguments[ 0 ].str() , arguments[ 1 ].str() , arguments[ 2 ].str() , arguments[ 3 ].str() , arguments[ 4 ].str() , arguments[ 5 ].str() , (char*)0 );
									else
										execle( module_name , module_name , arguments[ 0 ].str() , arguments[ 1 ].str() , arguments[ 2 ].str() , arguments[ 3 ].str() , arguments[ 4 ].str() , arguments[ 5 ].str() , (char*)0 , process_env );
								}
								else if( arguments.Count() == 7 )
								{
									if( process_env[ 0 ] == 0 )
										execlp( module_name , module_name , arguments[ 0 ].str() , arguments[ 1 ].str() , arguments[ 2 ].str() , arguments[ 3 ].str() , arguments[ 4 ].str() , arguments[ 5 ].str() , arguments[ 6 ].str() , (char*)0 );
									else
										execle( module_name , module_name , arguments[ 0 ].str() , arguments[ 1 ].str() , arguments[ 2 ].str() , arguments[ 3 ].str() , arguments[ 4 ].str() , arguments[ 5 ].str() , arguments[ 6 ].str() , (char*)0 , process_env );
								}
								else if( arguments.Count() == 8 )
								{
									if( process_env[ 0 ] == 0 )
										execlp( module_name , module_name , arguments[ 0 ].str() , arguments[ 1 ].str() , arguments[ 2 ].str() , arguments[ 3 ].str() , arguments[ 4 ].str() , arguments[ 5 ].str() , arguments[ 6 ].str() , arguments[ 7 ].str() , (char*)0 );
									else
										execle( module_name , module_name , arguments[ 0 ].str() , arguments[ 1 ].str() , arguments[ 2 ].str() , arguments[ 3 ].str() , arguments[ 4 ].str() , arguments[ 5 ].str() , arguments[ 6 ].str() , arguments[ 7 ].str() , (char*)0 , process_env );
								}
								else if( arguments.Count() == 9 )
								{
									if( process_env[ 0 ] == 0 )
										execlp( module_name , module_name , arguments[ 0 ].str() , arguments[ 1 ].str() , arguments[ 2 ].str() , arguments[ 3 ].str() , arguments[ 4 ].str() , arguments[ 5 ].str() , arguments[ 6 ].str() , arguments[ 7 ].str() , arguments[ 8 ].str() , (char*)0 );
									else
										execle( module_name , module_name , arguments[ 0 ].str() , arguments[ 1 ].str() , arguments[ 2 ].str() , arguments[ 3 ].str() , arguments[ 4 ].str() , arguments[ 5 ].str() , arguments[ 6 ].str() , arguments[ 7 ].str() , arguments[ 8 ].str() , (char*)0 , process_env );
								}
								else if( arguments.Count() == 10 )
								{
									if( process_env[ 0 ] == 0 )
										execlp( module_name , module_name , arguments[ 0 ].str() , arguments[ 1 ].str() , arguments[ 2 ].str() , arguments[ 3 ].str() , arguments[ 4 ].str() , arguments[ 5 ].str() , arguments[ 6 ].str() , arguments[ 7 ].str() , arguments[ 8 ].str() , arguments[ 9 ].str() , (char*)0 );
									else
										execle( module_name , module_name , arguments[ 0 ].str() , arguments[ 1 ].str() , arguments[ 2 ].str() , arguments[ 3 ].str() , arguments[ 4 ].str() , arguments[ 5 ].str() , arguments[ 6 ].str() , arguments[ 7 ].str() , arguments[ 8 ].str() , arguments[ 9 ].str() , (char*)0 , process_env );
								}
							}
							_exit( 1 );
						}
						else if (fork_rv == -1)
							_exit( 2 );
						_exit( 0 );
					}
				}
				else
					error = "executable '" + current_cp + "' is not found";
		#endif		
			}
			else
				error = "cannot start process,module name is empty";
			return result == 0 ? 1 : 0;
		};
		void XProcess::StartAndKillParent( char* executable_path , const char** arguments , uint32 arguments_count )
		{
		#ifdef __UNIX__
			pid_t pid = fork();
			if( pid < 0 )
			{
			}
			else if( pid != 0 )
			{				
				wait( 0 );
			}
			else
			{	
				kill( getppid() , 9 );		
				setsid();			
				declare_stack_variable( char , executable_file , 1024 );
				rux_get_executable_filename_only( executable_path , executable_file );	
				if( arguments_count == 0 )
				{
					execl( executable_path , executable_file , (char*)0 );
				}
				else if( arguments_count == 1 )
				{
					execl( executable_path , executable_file , arguments[ 0 ] , (char*)0 );
				}
				else if( arguments_count == 2 )
				{
					execl( executable_path , executable_file , arguments[ 0 ] , arguments[ 1 ] , (char*)0 );
				}
				else if( arguments_count == 3 )
				{
					execl( executable_path , executable_file , arguments[ 0 ] , arguments[ 1 ] , arguments[ 2 ] , (char*)0 );
				}
				else if( arguments_count == 4 )
				{
					execl( executable_path , executable_file , arguments[ 0 ] , arguments[ 1 ] , arguments[ 2 ] , arguments[ 3 ] , (char*)0 );
				}
				else if( arguments_count == 5 )
				{
					execl( executable_path , executable_file , arguments[ 0 ] , arguments[ 1 ] , arguments[ 2 ] , arguments[ 3 ] , arguments[ 4 ] , (char*)0 );
				}
				else if( arguments_count == 6 )
				{
					execl( executable_path , executable_file , arguments[ 0 ] , arguments[ 1 ] , arguments[ 2 ] , arguments[ 3 ] , arguments[ 4 ] , arguments[ 5 ] , (char*)0 );
				}
				else if( arguments_count == 7 )
				{
					execl( executable_path , executable_file , arguments[ 0 ] , arguments[ 1 ] , arguments[ 2 ] , arguments[ 3 ] , arguments[ 4 ] , arguments[ 5 ] , arguments[ 6 ] , (char*)0 );
				}
				else if( arguments_count == 8 )
				{
					execl( executable_path , executable_file , arguments[ 0 ] , arguments[ 1 ] , arguments[ 2 ] , arguments[ 3 ] , arguments[ 4 ] , arguments[ 5 ] , arguments[ 6 ] , arguments[ 7 ] , (char*)0 );
				}
				else if( arguments_count == 9 )
				{
					execl( executable_path , executable_file , arguments[ 0 ] , arguments[ 1 ] , arguments[ 2 ] , arguments[ 3 ] , arguments[ 4 ] , arguments[ 5 ] , arguments[ 6 ] , arguments[ 7 ] , arguments[ 8 ] , (char*)0 );
				}
				else if( arguments_count == 10 )
				{
					execl( executable_path , executable_file , arguments[ 0 ] , arguments[ 1 ] , arguments[ 2 ] , arguments[ 3 ] , arguments[ 4 ] , arguments[ 5 ] , arguments[ 6 ] , arguments[ 7 ] , arguments[ 8 ] , arguments[ 9 ] , (char*)0 );
				}
				else
				{
					char** zero_arguments = alloc_array_macros( char* , arguments_count + 2 );
					zero_arguments[ 0 ] = executable_file;
					for( size_t index0 = 0 ; index0 < arguments_count ; index0++ )
						zero_arguments[ index0 + 1 ] = const_cast< char* >( arguments[ index0 ] );
					zero_arguments[ arguments_count + 1 ] = 0;
					execv( executable_path , zero_arguments );
					::rux::engine::free_mem( zero_arguments );
				}
				_exit( 1 );
			}
		#endif		
		};
		uint8 XProcess::StartDetached( const rux::XString& arguments , uint8 show , rux::XString& error )
		{
			error.Clear();
			unsigned long result = 0;
			if( (*this)()->_module_name.Length() > 0 )
			{
				rux::XString module_name;
				if( rux::io::XFile::Exists( (*this)()->_module_name ) == 0 )
				{
					rux::XString string( getenv( "Path" ) );
					if( string.Length() )
					{
						XArray< rux::XString > paths;
						paths.set_ByRef( string.Split( ';' ) );
						for( uint32 index0 = 0 ; index0 < paths.Count() ; index0++ )			
						{
							string.set_ByRef( paths[ index0 ] );
							string.set_ByRef( string.ReplaceAll( '\\' , '/' ) );
							if( string.get_UTF8Char( string.Length() - 1 ) == '/' )
								string.RemoveAt( string.Length() - 1 );
							string += "/" + (*this)()->_module_name;
							if( rux::io::XFile::Exists( string ) == 1 )
							{
								module_name.set_ByRef( string );
								break;
							}
						}
					}
				}
				else
					module_name.set_ByRef( (*this)()->_module_name );
				if( module_name.Length() > 0 )
				{			
		#ifdef __WINDOWS__						
					rux::XString cmd_line = module_name + " " + arguments;
					PROCESS_INFORMATION  process_information;
					memset( (void*)&process_information , 0 , sizeof( PROCESS_INFORMATION ) );			
					if( cmd_line()->_code_page == XEnumCodePage_UTF16 )
					{
						STARTUPINFOW startup_info;
						memset( (void*)&startup_info , 0 , sizeof( STARTUPINFOW ) );			
						startup_info.cb = sizeof( STARTUPINFOW );
						startup_info.dwFlags = STARTF_USESHOWWINDOW;
						if( show == 1 )
							startup_info.wShowWindow = SW_SHOW;
						else
							startup_info.wShowWindow = SW_HIDE;
						if( CreateProcessW( NULL , (wchar_t*)cmd_line.str() , NULL , NULL , FALSE , CREATE_DEFAULT_ERROR_MODE | IDLE_PRIORITY_CLASS | DETACHED_PROCESS, NULL , NULL , &startup_info , &process_information ) == 0 )				
						{
							result = GetLastError();
							error = XError::ConvertErrorCodeToString( result , XEnum_ErrorType_GetLastError );
						}
					}
					else if( cmd_line()->_code_page == XEnumCodePage_CP1251 )
					{
						STARTUPINFOA startup_info;
						memset( (void*)&startup_info , 0 , sizeof( STARTUPINFOA ) );			
						startup_info.cb = sizeof( STARTUPINFOA );
						startup_info.dwFlags = STARTF_USESHOWWINDOW;
						if( show == 1 )
							startup_info.wShowWindow = SW_SHOW;
						else
							startup_info.wShowWindow = SW_HIDE;
						if( CreateProcessA( NULL , cmd_line.str() , NULL , NULL , FALSE , CREATE_DEFAULT_ERROR_MODE | IDLE_PRIORITY_CLASS | DETACHED_PROCESS , NULL , NULL , &startup_info , &process_information ) == 0 )				
						{
							result = GetLastError();
							error = XError::ConvertErrorCodeToString( result , XEnum_ErrorType_GetLastError );
						}
					}
					else
					{
						rux::XString utf16_cmd_line = cmd_line.ConvertToUTF16();
						STARTUPINFOW startup_info;
						memset( (void*)&startup_info , 0 , sizeof( STARTUPINFOW ) );			
						startup_info.cb = sizeof( STARTUPINFOW );
						startup_info.dwFlags = STARTF_USESHOWWINDOW;
						if( show == 1 )
							startup_info.wShowWindow = SW_SHOW;
						else
							startup_info.wShowWindow = SW_HIDE;
						if( CreateProcessW( NULL , (wchar_t*)utf16_cmd_line.str() , NULL , NULL , FALSE , CREATE_DEFAULT_ERROR_MODE | IDLE_PRIORITY_CLASS | DETACHED_PROCESS , NULL , NULL , &startup_info , &process_information ) == 0 )				
						{
							result = GetLastError();
							error = XError::ConvertErrorCodeToString( result , XEnum_ErrorType_GetLastError );
						}
					}
					if( process_information.hProcess )
						CloseHandle( process_information.hProcess );
					if( process_information.hThread )
						CloseHandle( process_information.hThread );
		#endif
				}
				else
					error = "cannot start process,module '" + (*this)()->_module_name + "' does not exists";
			}
			else
				error = "cannot start process,module name is empty";
			return result == 0 ? 1 : 0;
		};
		uint8 XProcess::Start( const rux::XString& arguments , rux::XString& error )
		{
			error.Clear();
			unsigned long result = 0;
			if( (*this)()->_module_name.Length() > 0 )
			{
				rux::XString module_name;
				if( rux::io::XFile::Exists( (*this)()->_module_name ) == 0 )
				{
					rux::XString string( getenv( "Path" ) );
					if( string.Length() )
					{
						XArray< rux::XString > paths;
						paths.set_ByRef( string.Split( ';' ) );
						for( uint32 index0 = 0 ; index0 < paths.Count() ; index0++ )			
						{
							string.set_ByRef( paths[ index0 ] );
							string.set_ByRef( string.ReplaceAll( '\\' , '/' ) );
							if( string.get_UTF8Char( string.Length() - 1 ) == '/' )
								string.RemoveAt( string.Length() - 1 );
							string += "/" + (*this)()->_module_name;
							if( rux::io::XFile::Exists( string ) == 1 )
							{
								module_name.set_ByRef( string );
								break;
							}
						}
					}
				}
				else
					module_name.set_ByRef( (*this)()->_module_name );
				if( module_name.Length() > 0 )
				{			
		#ifdef __WINDOWS__						
					rux::XString cmd_line = module_name + " " + arguments;
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
						{
							result = GetLastError();
							error = XError::ConvertErrorCodeToString( result , XEnum_ErrorType_GetLastError );
						}
						else
						{
							result = private_WaitForProcess( process_information.hProcess );
							if( ERROR_SUCCESS == result )
								GetExitCodeProcess( process_information.hProcess, &result );
							else
								error = XError::ConvertErrorCodeToString( result , XEnum_ErrorType_GetLastError );
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
						{
							result = GetLastError();
							error = XError::ConvertErrorCodeToString( result , XEnum_ErrorType_GetLastError );
						}
						else
						{
							result = private_WaitForProcess( process_information.hProcess );
							if( ERROR_SUCCESS == result )
								GetExitCodeProcess( process_information.hProcess, &result );
							else
								error = XError::ConvertErrorCodeToString( result , XEnum_ErrorType_GetLastError );
						}
					}
					else
					{
						rux::XString utf16_cmd_line = cmd_line.ConvertToUTF16();
						STARTUPINFOW startup_info;
						memset( (void*)&startup_info , 0 , sizeof( STARTUPINFOW ) );			
						startup_info.cb = sizeof( STARTUPINFOW );
						startup_info.dwFlags = STARTF_USESHOWWINDOW;
						startup_info.wShowWindow = SW_SHOW;
						if( CreateProcessW( NULL , (wchar_t*)utf16_cmd_line.str() , NULL , NULL , FALSE , CREATE_DEFAULT_ERROR_MODE , NULL , NULL , &startup_info , &process_information ) == 0 )				
						{
							result = GetLastError();
							error = XError::ConvertErrorCodeToString( result , XEnum_ErrorType_GetLastError );
						}
						else
						{
							result = private_WaitForProcess( process_information.hProcess );
							if( ERROR_SUCCESS == result )
								GetExitCodeProcess( process_information.hProcess, &result );
							else
								error = XError::ConvertErrorCodeToString( result , XEnum_ErrorType_GetLastError );
						}
					}
					if( process_information.hProcess )
						CloseHandle( process_information.hProcess );
					if( process_information.hThread )
						CloseHandle( process_information.hThread );
		#endif
				}
				else
					error = "cannot start process,module '" + (*this)()->_module_name + "' does not exists";
			}
			else
				error = "cannot start process,module name is empty";
			return result == 0 ? 1 : 0;
		};
		uint8 XProcess::Start( rux::XString& error )
		{
			error.Clear();
			unsigned long result = 0;
			if( (*this)()->_module_name.Length() > 0 )
			{
				rux::XString module_name;
				if( rux::io::XFile::Exists( (*this)()->_module_name ) == 0 )
				{
					rux::XString string( getenv( "Path" ) );
					if( string.Length() )
					{
						XArray< rux::XString > paths;
						paths.set_ByRef( string.Split( ';' ) );
						for( uint32 index0 = 0 ; index0 < paths.Count() ; index0++ )			
						{
							string.set_ByRef( paths[ index0 ] );
							string.set_ByRef( string.ReplaceAll( '\\' , '/' ) );
							if( string.get_UTF8Char( string.Length() - 1 ) == '/' )
								string.RemoveAt( string.Length() - 1 );
							string += "/" + (*this)()->_module_name;
							if( rux::io::XFile::Exists( string ) == 1 )
							{
								module_name.set_ByRef( string );
								break;
							}
						}
					}
				}
				else
					module_name.set_ByRef( (*this)()->_module_name );
				if( module_name.Length() > 0 )
				{
		#ifdef __WINDOWS__						
					PROCESS_INFORMATION  process_information;
					memset( (void*)&process_information , 0 , sizeof( PROCESS_INFORMATION ) );			
					if( module_name()->_code_page == XEnumCodePage_UTF16 )
					{
						STARTUPINFOW startup_info;
						memset( (void*)&startup_info , 0 , sizeof( STARTUPINFOW ) );			
						startup_info.cb = sizeof( STARTUPINFOW );
						startup_info.dwFlags = STARTF_USESHOWWINDOW;
						startup_info.wShowWindow = SW_SHOW;
						if( CreateProcessW( NULL , (wchar_t*)module_name.str() , NULL , NULL , FALSE , CREATE_DEFAULT_ERROR_MODE , NULL , NULL , &startup_info , &process_information ) == 0 )				
						{
							result = GetLastError();
							error = XError::ConvertErrorCodeToString( result , XEnum_ErrorType_GetLastError );
						}
						else
						{
							result = private_WaitForProcess( process_information.hProcess );
							if( ERROR_SUCCESS == result )
								GetExitCodeProcess( process_information.hProcess, &result );
							else
								error = XError::ConvertErrorCodeToString( result , XEnum_ErrorType_GetLastError );
						}
					}
					else if( module_name()->_code_page == XEnumCodePage_CP1251 )
					{
						STARTUPINFOA startup_info;
						memset( (void*)&startup_info , 0 , sizeof( STARTUPINFOA ) );			
						startup_info.cb = sizeof( STARTUPINFOA );
						startup_info.dwFlags = STARTF_USESHOWWINDOW;
						startup_info.wShowWindow = SW_SHOW;
						if( CreateProcessA( NULL , module_name.str() , NULL , NULL , FALSE , CREATE_DEFAULT_ERROR_MODE , NULL , NULL , &startup_info , &process_information ) == 0 )				
						{
							result = GetLastError();
							error = XError::ConvertErrorCodeToString( result , XEnum_ErrorType_GetLastError );
						}
						else
						{
							result = private_WaitForProcess( process_information.hProcess );
							if( ERROR_SUCCESS == result )
								GetExitCodeProcess( process_information.hProcess, &result );
							else
								error = XError::ConvertErrorCodeToString( result , XEnum_ErrorType_GetLastError );
						}
					}
					else
					{
						rux::XString utf16_cmd_line = module_name.ConvertToUTF16();
						STARTUPINFOW startup_info;
						memset( (void*)&startup_info , 0 , sizeof( STARTUPINFOW ) );			
						startup_info.cb = sizeof( STARTUPINFOW );
						startup_info.dwFlags = STARTF_USESHOWWINDOW;
						startup_info.wShowWindow = SW_SHOW;
						if( CreateProcessW( NULL , (wchar_t*)utf16_cmd_line.str() , NULL , NULL , FALSE , CREATE_DEFAULT_ERROR_MODE , NULL , NULL , &startup_info , &process_information ) == 0 )				
						{
							result = GetLastError();
							error = XError::ConvertErrorCodeToString( result , XEnum_ErrorType_GetLastError );
						}
						else
						{
							result = private_WaitForProcess( process_information.hProcess );
							if( ERROR_SUCCESS == result )
								GetExitCodeProcess( process_information.hProcess, &result );
							else
								error = XError::ConvertErrorCodeToString( result , XEnum_ErrorType_GetLastError );
						}
					}
					if( process_information.hProcess )
						CloseHandle( process_information.hProcess );
					if( process_information.hThread )
						CloseHandle( process_information.hThread );
		#endif
				}
				else
					error = "cannot start process,module '" + (*this)()->_module_name + "' does not exists";
			}
			else
				error = "cannot start process,module name is empty";
			return result == 0 ? 1 : 0;
		};
		#ifdef __WINDOWS__
		uint32 XProcess::private_WaitForProcess( HANDLE handle )
		{
			uint32 result = NOERROR;
			MSG msg;
			memset( (void*)&msg , 0 , sizeof( MSG ) );
				uint8 is_exit = 0;
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
		void XProcess::Stop( void )
		{
			if( raise( SIGINT ) == 0 )
			{
			}
			else
			{
			}
		};
		::rux::int64 XProcess::get_WorkingSet( rux::XString& error )
		{
			error.Clear();
			::rux::XString module_name;
			module_name.set_ByRef( (*this)()->_module_name.ConvertToAnsi() );
			::rux::int64 value = 0;
			if( module_name.Length() > 0 )
			{
		#ifdef __WINDOWS__
				rux::pdh::counter( module_name.str() , 230 , 180 , PDH_FMT_LARGE , &value , &error );
		#endif
			}
			return value;
		};
		::rux::int64 XProcess::get_VirtualBytes( rux::XString& error )
		{
			error.Clear();
			::rux::XString module_name;
			module_name.set_ByRef( (*this)()->_module_name.ConvertToAnsi() );
			::rux::int64 value = 0;
			if( module_name.Length() > 0 )
			{
		#ifdef __WINDOWS__
				rux::pdh::counter( module_name.str() , 230 , 174 , PDH_FMT_LARGE , &value , &error );
		#endif
			}
			return value;
		};
		::rux::int64 XProcess::get_PrivateBytes( rux::XString& error )
		{
			error.Clear();
			::rux::XString module_name;
			module_name.set_ByRef( (*this)()->_module_name.ConvertToAnsi() );
			::rux::int64 value = 0;
			if( module_name.Length() > 0 )
			{
		#ifdef __WINDOWS__
				::rux::pdh::counter( module_name.str() , 230 , 186 , PDH_FMT_LARGE , &value , &error );
		#endif
			}
			return value;
		};
		uint8 XProcess::IsAdminUserProcess( void )
		{
			uint8 is_admin_user = 0;
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
		uint8 XProcess::IsExecuted( const rux::XString& process_name , rux::XString& error )
		{
			error.Clear();
			::rux::XString cp_process_name;
			cp_process_name.set_ByRef( process_name.ConvertToAnsi() );
			::rux::pid_t pid = 0;
			if( ::rux::diagnostics::process::executing( cp_process_name.str() , &pid ) )
				return 1;
			else
				return 0;
		};
		void XProcess::KillZombies( const rux::XString& process_name )
		{
		#ifdef __WINDOWS__
		#else
			pid_t ppid = 0 , pid = 0;
			rux::XString directory_name , file_status_string , Name , PPid , Pid , State , error;
			rux::io::XFile file_status;
			XArray< rux::XString > file_status_lines , file_status_pair;
			XArray< rux::io::XDirectory > dirs;
			dirs.set_ByRef( rux::io::XDirectory::GetDirectories( "/proc" , XTopDirectoryOnly ) );
			uint32 index1 = 0;	
			for( uint32 index0 = 0 ; index0 < dirs.Count() ; index0++ )
			{
				directory_name.set_ByRef( dirs[ index0 ].get_DirectoryName() );
				file_status_string = directory_name + "/status";
				if( rux::io::XFile::Exists( file_status_string ) == 1 )
				{
					file_status = rux::io::XFile::Open( file_status_string , error );
					if( file_status.Exists() == 1 )
					{
						file_status_string.set_ByRef( file_status.ReadTextAll( error ) ); 
						file_status.Close();
						file_status_lines.set_ByRef( file_status_string.Split( '\n' ) );
						rux_get_argument_by_separator( Name , file_status_lines , ":" );
						Name.set_ByRef( Name.Trim() );
						if( Name == process_name )
						{
							rux_get_argument_by_separator( State , file_status_lines , ":" );
							State.set_ByRef( State.Trim() );
							if( State.get_UTF8Char( 0 ) == 'Z' )
							{
								rux_get_argument_by_separator( Pid , file_status_lines , ":" );
								pid = Pid.ToUInt32();
								if( pid != 0 )
								{
									rux_get_argument_by_separator( PPid , file_status_lines , ":" );
									ppid = PPid.ToUInt32();
									XArray< XUInt32 > ppids;
									while( ppid != 0 )
									{
										ppids.Add( XUInt32( ppid ) );
										ppid = get_ParentProcessId( ppid );
									}
									while( ppids.Count() > 0 )
									{
										ppid = ppids[ ppids.Count() - 1 ]();
										ppids.RemoveAt( ppids.Count() - 1 );
										if( ppid != 1 && kill( ppid , 9 ) != -1 )
											waitpid( ppid , NULL , 0 );
									}
									if( kill( pid , 9 ) != -1 )
										waitpid( pid , NULL , 0 );
								}
							}
						}								
					}
				}
			}
		#endif		
		};
		void XProcess::Kill( const rux::XString& process_name , rux::XString& error )
		{
			error.Clear();
			::rux::XString cp_process_name;
			cp_process_name.set_ByRef( process_name.ConvertToAnsi() );
			if( ::rux::diagnostics::process::kill( cp_process_name.str() ) == 0 )
			{
				rux_write_error( "cannot kill process '" + process_name + "'" , error );
			}
		};
		#ifdef __WINDOWS__
		DWORD XProcess::get_ParentProcessId( DWORD pid )
		{
			DWORD ppid = 0;
		#elif defined( __UNIX__ )
		pid_t XProcess::get_ParentProcessId( pid_t pid )
		{
			pid_t ppid = 0;
			rux::XString file_status_string , PPid , error;
			rux::io::XFile file_status;
			XArray< rux::XString > file_status_lines;
			uint32 index1 = 0;			
			file_status_string = "/proc/" + XUInt32( pid ).ToString() + "/status";
			if( rux::io::XFile::Exists( file_status_string ) == 1 )
			{
				file_status = rux::io::XFile::Open( file_status_string , error );
				if( file_status.Exists() == 1 )
				{
					file_status_string.set_ByRef( file_status.ReadTextAll( error ) ); 
					file_status.Close();
					file_status_lines.set_ByRef( file_status_string.Split( '\n' ) );
					rux_get_argument_by_separator( PPid , file_status_lines , ":" );
					ppid = PPid.ToUInt32();			
				}
			}
		#endif
			return ppid;
		};
		#ifdef __WINDOWS__
		uint8 XProcess::CtrlC( const rux::XString& process_name , DWORD pid , rux::XString& error )
		#elif defined( __UNIX__ )
		uint8 XProcess::CtrlC( const rux::XString& process_name , pid_t pid , rux::XString& error )
		#endif
		{
			error.Clear();
			uint8 is_killed = 0;
		#ifdef __WINDOWS__
			::rux::diagnostics::process::adjust_token_privileges();
			DWORD processes[ 1024 ] = {0} , size = 0 , processes_count = 0;
			rux::XString alter_process_name;
			alter_process_name.set_ByRef( process_name.ReplaceAll( '/' , '\\' ) );
			if( ::rux::engine::_globals->_psapi_module.EnumProcesses( processes , sizeof( processes ) , &size ) )
			{
				processes_count = size / sizeof( DWORD );
				declare_stack_variable( char , process_name_ptr , 1024 );
				rux::XString process_name_without_extension;
				HANDLE process_handle = NULL;
				HMODULE module_handle = NULL;
				for( uint32 index0 = 0 ; index0 < processes_count ; index0++ )
				{
					if( processes[ index0 ] != 0 && processes[ index0 ] == pid )
					{
						process_handle = OpenProcess( SYNCHRONIZE | PROCESS_TERMINATE | PROCESS_QUERY_INFORMATION | PROCESS_VM_READ , FALSE , processes[ index0 ] );
						if( process_handle != NULL )
						{
							if( ::rux::engine::_globals->_psapi_module.EnumProcessModules( process_handle , &module_handle , sizeof( module_handle ) , &size ) )
							{						
								if( ::rux::engine::_globals->_psapi_module.GetModuleFileNameExA( process_handle , module_handle , process_name_ptr , 1024 ) > 0 )
								{
									if( process_name == process_name_ptr 
										|| alter_process_name == process_name_ptr )
									{
										if( AttachConsole( pid ) )
										{
											SetConsoleCtrlHandler( NULL , true );
											if( GenerateConsoleCtrlEvent( CTRL_C_EVENT , 0 ) )
												is_killed = 1;
											else
											{
												rux_write_error( XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError ) , error );
												CloseHandle( process_handle );
												break;
											}
											FreeConsole();									
										}
										else
										{
											rux_write_error( XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError ) , error );
											CloseHandle( process_handle );
											break;
										}
									}	
								}
								if( ::rux::engine::_globals->_psapi_module.GetModuleBaseNameA( process_handle , module_handle , process_name_ptr , sizeof( process_name_ptr ) / sizeof( char ) ) > 0 )
								{
									process_name_without_extension.set_ByRef( rux::io::XPath::GetFileNameWithoutExtension( process_name_ptr ) );
									if( process_name == process_name_ptr
										|| process_name_without_extension == process_name )
									{
										if( AttachConsole( pid ) )
										{
											SetConsoleCtrlHandler( NULL , true );
											if( GenerateConsoleCtrlEvent( CTRL_C_EVENT , 0 ) )
												is_killed = 1;
											else
											{
												rux_write_error( XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError ) , error );
												CloseHandle( process_handle );
												break;
											}				
											FreeConsole();									
										}
										else
										{
											rux_write_error( XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError ) , error );
											CloseHandle( process_handle );
											break;
										}
									}							
								}
								else
								{
									rux_write_error( XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError ) , error );
									CloseHandle( process_handle );
									break;
								}
							}
							else
							{
								rux_write_error( XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError ) , error );
								CloseHandle( process_handle );
								break;
							}
							CloseHandle( process_handle );
						}
						else
						{
							rux_write_error( XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError ) , error );
							break;
						}
					}
					if( is_killed == 1 )
						break;
				}
			}
			else
			{
				rux_write_error( XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError ) , error );
			}	
		#else
			rux::XString file_status_string;
			rux::io::XFile file_status;
			XArray< rux::XString > file_status_lines , file_status_pair;	
			uint32 index1 = 0;	
			declare_stack_variable( char , fullpath , 1024 );
			file_status_string = "/proc/" + XUInt32( pid ).ToString() + "/exe";
			memset( fullpath , 0 , 1024 );
			readlink( file_status_string.str() , fullpath , 1024 );
			file_status_string = "/proc/" + XUInt32( pid ).ToString() + "/status";
			rux::XString alter_process_name;
			alter_process_name.set_ByRef( process_name.ReplaceAll( '\\' , '/' ) );
			if( process_name == fullpath
				|| alter_process_name == fullpath )
			{		
				if( kill( pid , 2 ) == -1 )
				{
					is_killed = 0;
					rux_write_error( XError::ConvertErrorCodeToString( errno , XEnum_ErrorType_Errno ) , error );
				}
				else
					is_killed = 1;
			}
			else if( rux::io::XFile::Exists( file_status_string ) == 1 )
			{
				file_status = rux::io::XFile::Open( file_status_string , error );
				if( file_status.Exists() == 1 )
				{
					file_status_string.set_ByRef( file_status.ReadTextAll( error ) ); 
					file_status.Close();
					file_status_lines.set_ByRef( file_status_string.Split( '\n' ) );
					for( index1 = 0 ; index1 < file_status_lines.Count() ; index1++ )
					{
						file_status_pair.set_ByRef( file_status_lines[ index1 ].Split( ':' ) );
						if( is_killed == 0 && file_status_pair.Count() == 2 && file_status_pair[ 0 ].Equals( "Name" , 4 ) )
						{
							file_status_pair.set_ItemByRef( 1 , file_status_pair[ 1 ].Trim() );
							if( file_status_pair[ 1 ] == process_name )
								is_killed = 1;
							else
								break;
						}
						else if( is_killed == 1 && file_status_pair.Count() == 2 && file_status_pair[ 0 ].Equals( "Pid" , 3 ) )
						{
							file_status_pair.set_ItemByRef( 1 , file_status_pair[ 1 ].Trim() );
							pid_t local_pid = file_status_pair[ 1 ].ToUInt32();
							if( kill( local_pid , 2 ) == -1 )
							{
								rux_write_error( XError::ConvertErrorCodeToString( errno , XEnum_ErrorType_Errno ) , error );
							}
							break;
						}
					}			
				}
			}		
		#endif	
			if( error.Length() == 0 && is_killed == 0 )
			{
				rux_write_error( "cannot find process '" + process_name + "'" , error );
			}
			return is_killed;
		};
		#ifdef __WINDOWS__
		void XProcess::Kill( const rux::XString& process_name , DWORD pid , rux::XString& error )
		#elif defined( __UNIX__ )
		void XProcess::Kill( const rux::XString& process_name , pid_t pid , rux::XString& error )
		#endif
		{
			error.Clear();
			uint8 is_killed = 0;
		#ifdef __WINDOWS__
			::rux::diagnostics::process::adjust_token_privileges();
			DWORD processes[ 1024 ] = {0} , size = 0 , processes_count = 0;
			rux::XString alter_process_name;
			alter_process_name.set_ByRef( process_name.ReplaceAll( '/' , '\\' ) );
			if( ::rux::engine::_globals->_psapi_module.EnumProcesses( processes , sizeof( processes ) , &size ) )
			{
			processes_count = size / sizeof( DWORD );
				declare_stack_variable( char , process_name_ptr , 1024 );
				rux::XString process_name_without_extension;
				HANDLE process_handle = NULL;
				HMODULE module_handle = NULL;
			for( uint32 index0 = 0 ; index0 < processes_count ; index0++ )
				{
					if( processes[ index0 ] != 0 && processes[ index0 ] == pid )
					{
						process_handle = OpenProcess( SYNCHRONIZE | PROCESS_TERMINATE | PROCESS_QUERY_INFORMATION | PROCESS_VM_READ , FALSE , processes[ index0 ] );
						if( process_handle != NULL )
						{
							if( ::rux::engine::_globals->_psapi_module.EnumProcessModules( process_handle , &module_handle , sizeof( module_handle ) , &size ) )
							{						
								if( ::rux::engine::_globals->_psapi_module.GetModuleFileNameExA( process_handle , module_handle , process_name_ptr , 1024 ) > 0 )
								{
									if( process_name == process_name_ptr 
										|| alter_process_name == process_name_ptr )
									{
										if( TerminateProcess( process_handle , 0 ) )
										{
											WaitForSingleObject( process_handle , ::rux::threading::infinite );
											is_killed = 1;
										}
										else
										{
											rux_write_error( XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError ) , error );
											CloseHandle( process_handle );
											break;
										}
									}	
								}
								if( ::rux::engine::_globals->_psapi_module.GetModuleBaseNameA( process_handle , module_handle , process_name_ptr , sizeof( process_name_ptr ) / sizeof( char ) ) > 0 )
								{
									process_name_without_extension.set_ByRef( rux::io::XPath::GetFileNameWithoutExtension( process_name_ptr ) );
									if( process_name == process_name_ptr
										|| process_name_without_extension == process_name )
									{
										if( TerminateProcess( process_handle , 0 ) )
										{
											WaitForSingleObject( process_handle , ::rux::threading::infinite );
											is_killed = 1;
										}
										else
										{
											rux_write_error( XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError ) , error );
											CloseHandle( process_handle );
											break;
										}
									}							
								}
								else
								{
									rux_write_error( XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError ) , error );
									CloseHandle( process_handle );
									break;
								}
							}
							else
							{
								rux_write_error( XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError ) , error );
								CloseHandle( process_handle );
								break;
							}
							CloseHandle( process_handle );
						}
						else
						{
							rux_write_error( XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError ) , error );
							break;
						}
					}
					if( is_killed == 1 )
						break;
				}
			}
			else
			{
				rux_write_error( XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError ) , error );
			}	
		#else
			rux::XString file_status_string;
			rux::io::XFile file_status;
			XArray< rux::XString > file_status_lines , file_status_pair;	
			uint32 index1 = 0;	
			declare_stack_variable( char , fullpath , 1024 );
			file_status_string = "/proc/" + XUInt32( pid ).ToString() + "/exe";
			memset( fullpath , 0 , 1024 );
			readlink( file_status_string.str() , fullpath , 1024 );
			file_status_string = "/proc/" + XUInt32( pid ).ToString() + "/status";
			rux::XString alter_process_name;
			alter_process_name.set_ByRef( process_name.ReplaceAll( '\\' , '/' ) );
			if( process_name == fullpath
				|| alter_process_name == fullpath )
			{		
				if( kill( pid , 9 ) == -1 )
				{
					is_killed = 0;
					rux_write_error( XError::ConvertErrorCodeToString( errno , XEnum_ErrorType_Errno ) , error );
				}
				else
				{
					waitpid( pid , NULL , 0 );
					is_killed = 1;
				}
			}
			else if( rux::io::XFile::Exists( file_status_string ) == 1 )
			{
				file_status = rux::io::XFile::Open( file_status_string , error );
				if( file_status.Exists() == 1 )
				{
					file_status_string.set_ByRef( file_status.ReadTextAll( error ) ); 
					file_status.Close();
					file_status_lines.set_ByRef( file_status_string.Split( '\n' ) );
					for( index1 = 0 ; index1 < file_status_lines.Count() ; index1++ )
					{
						file_status_pair.set_ByRef( file_status_lines[ index1 ].Split( ':' ) );
						if( is_killed == 0 && file_status_pair.Count() == 2 && file_status_pair[ 0 ].Equals( "Name" , 4 ) )
						{
							file_status_pair.set_ItemByRef( 1 , file_status_pair[ 1 ].Trim() );
							if( file_status_pair[ 1 ] == process_name )
							{
								is_killed = 1;
							}
							else
								break;
						}
						else if( is_killed == 1 && file_status_pair.Count() == 2 && file_status_pair[ 0 ].Equals( "Pid" , 3 ) )
						{
							file_status_pair.set_ItemByRef( 1 , file_status_pair[ 1 ].Trim() );
							pid_t local_pid = file_status_pair[ 1 ].ToUInt32();
							if( kill( local_pid , 9 ) == -1 )
							{
								rux_write_error( XError::ConvertErrorCodeToString( errno , XEnum_ErrorType_Errno ) , error );
							}
							else
								waitpid( local_pid , NULL , 0 );
							break;
						}
					}			
				}
			}		
		#endif	
			if( error.Length() == 0 && is_killed == 0 )
			{
				rux_write_error( "cannot find process '" + process_name + "'" , error );
			}
		};
		#ifdef __WINDOWS__
		DWORD XProcess::get_CurrentProcessId( void )
		{
			return GetCurrentProcessId();
		};
		#elif defined( __UNIX__ )
		pid_t XProcess::get_CurrentProcessId( void )
		{
			return getpid();
		};
		#endif
	};
};
