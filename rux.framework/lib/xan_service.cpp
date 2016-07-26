#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_service.h>
#include <xan_error.h>
#include <xan_thread.h>
#include <xan_file.h>
#include <xan_security.h>
#include <xan_registry.h>
#include <xan_keyvaluepair.h>

#ifndef BOOLDOG_NETWORK
#define BOOLDOG_NETWORK
#endif
#ifndef BOOLDOG_HEADER
#define BOOLDOG_HEADER( header ) <booldog/header>
#endif
#include BOOLDOG_HEADER(boo_string_utils.h)
#include BOOLDOG_HEADER(boo_heap_allocator.h)
#include BOOLDOG_HEADER(boo_mixed_allocator.h)
#include BOOLDOG_HEADER(boo_io_file.h)
#include BOOLDOG_HEADER(boo_time_utils.h)

const char* _init_d_daemon_script = 
"#!/bin/sh\n\
# %s initscript\n\
#\n\
#\n\
### BEGIN INIT INFO\n\
# Provides:          %s\n\
# Required-Start:    $local_fs $network $remote_fs $syslog\n\
# Required-Stop:    $local_fs $network $remote_fs $syslog\n\
# Should-Start:      $local_fs $network $remote_fs $syslog\n\
# Should-Stop:       $local_fs $network $remote_fs $syslog\n\
# Default-Start:     2 3 4 5\n\
# Default-Stop:      0 1 6\n\
# Short-Description: %s\n\
# Description: %s\n\
### END INIT INFO\n\
\n\
unset USER\n\
# PATH should only include /usr/* if it runs after the mountnfs.sh script\n\
PATH=/sbin:/usr/sbin:/bin:/usr/bin\n\
DESC=\"%s\"\n\
NAME=\"%s\"\n\
DAEMON=\"%s\"\n\
PIDFILE_OPT=%s\n\
PIDFILE=%s\n\
SCRIPTNAME=/etc/init.d/$NAME\n\
START_DAEMON=yes\n\
# Exit if the package is not installed\n\
[ -x \"$DAEMON\" ] || exit 0\n\
\n\
# Load the VERBOSE setting and other rcS variables\n\
if [ -e /lib/init/vars.sh ]; then\n\
	. /lib/init/vars.sh\n\
fi\n\
if [ -e /etc/rc.d/init.d/functions ]; then\n\
	. /etc/rc.d/init.d/functions\n\
fi\n\
# Define LSB log_* functions.\n\
# To be replaced by LSB functions\n\
# Defined here for distributions that don't define\n\
# log_daemon_msg\n\
log_daemon_msg () {\n\
    echo $@\n\
}\n\
# To be replaced by LSB functions\n\
# Defined here for distributions that don't define\n\
# log_end_msg\n\
log_end_msg () {\n\
    retval=$1\n\
    if [ $retval -eq 0 ]; then\n\
        echo \".\"\n\
    else\n\
        echo \" failed!\"\n\
    fi\n\
    return $retval\n\
}\n\
fn_exists() {\n\
  # appended double quote is an ugly trick to make sure we do get a string -- if $1 is not a known command, type does not output anything\n\
  [ `type -t $1`\"\" == 'function' ]\n\
}\n\
# Depend on lsb-base (>= 3.0-6) to ensure that this file is present.\n\
. /lib/lsb/init-functions\n\
# Do not start the daemon if NO_START is enabled in DEFAULT\n\
if [ \"$START_DAEMON\" != \"yes\" ] && [ \"$1\" != \"stop\" ]; then\n\
	log_warning_msg \"$NAME: Not starting $DESC.\"\n\
	log_warning_msg \"$NAME: Disabled in $DEFAULT.\"\n\
	exit 0\n\
fi\n\
\n\
# Set the default log file\n\
if [ -z $LOGFILE ]; then\n\
	LOGFILE=/var/log/%s.log\n\
fi\n\
# Run as cur user if USER is not specified or is root\n\
if [ -z $USER ]; then\n\
	USER=%s\n\
fi\n\
\n\
# If no group is specified, use USER\n\
if [ -z $GROUP ]; then\n\
	GROUP=$USER\n\
fi\n\
\n\
if [ -n $PIDFILE_OPT ]; then\n\
	if [ ! -d \"/var/run/$NAME\" ]; then\n\
		mkdir \"/var/run/$NAME\"\n\
	fi\n\
fi\n\
\n\
if [ -d \"/var/run/$NAME\" ]; then\n\
	chown -R -- $USER:$GROUP \"/var/run/$NAME\"\n\
fi\n\
\n\
DAEMON_ARGS=\"-d --logfile $LOGFILE --name $NAME --pidfile %s --autorecovery %s $DAEMON_OPTS\"\n\
#\n\
# Function that starts the daemon/service\n\
#\n\
do_start()\n\
{\n\
	# Return\n\
	#   0 if daemon has been started\n\
	#   1 if daemon was already running\n\
	#   2 if daemon could not be started\n\
	if hash ulimit 2>/dev/null; then\n\
		ulimit -n 16384\n\
	fi\n\
	if hash start-stop-daemon 2>/dev/null; then\n\
		start-stop-daemon --start --quiet $PIDFILE_OPT $PIDFILE \\\n\
			--chuid $USER:$GROUP --user $USER --exec \"$DAEMON\" --test > /dev/null \\\n\
			|| return 1\n\
		start-stop-daemon --start --quiet $PIDFILE_OPT $PIDFILE \\\n\
			--chuid $USER:$GROUP --user $USER --exec \"$DAEMON\" -- \\\n\
			$DAEMON_ARGS \\\n\
			|| return 2\n\
	else\n\
		if [ -n $PIDFILE_OPT ]; then\n\
			daemon --user=\"$USER\" --pidfile=\"$PIDFILE\" \"$DAEMON $DAEMON_ARGS\"\n\
			RETVAL=\"$?\"\n\
		else\n\
			daemon --user=\"$USER\" \"$DAEMON $DAEMON_ARGS\"\n\
			RETVAL=\"$?\"\n\
		fi\n\
		return \"$RETVAL\"\n\
	fi\n\
}\n\
#\n\
# Function that stops the daemon/service\n\
#\n\
do_stop()\n\
{\n\
	# Return\n\
	#   0 if daemon has been stopped\n\
	#   1 if daemon was already stopped\n\
	#   2 if daemon could not be stopped\n\
	#   other if a failure occurred\n\
	if hash start-stop-daemon 2>/dev/null; then\n\
		start-stop-daemon --stop --quiet --retry=TERM/30/KILL/5 $PIDFILE_OPT $PIDFILE --exec \"$DAEMON\"\n\
		RETVAL=\"$?\"\n\
		[ \"$RETVAL\" = 2 ] && return 2\n\
		# Wait for children to finish too if this is a daemon that forks\n\
		# and if the daemon is only ever run from this initscript.\n\
		#start-stop-daemon --stop --quiet --oknodo --retry=0/30/KILL/5 --exec \"$DAEMON\"\n\
		#[ \"$?\" = 2 ] && return 2\n\
		return \"$RETVAL\"\n\
	else\n\
		if [ -n $PIDFILE_OPT ]; then\n\
			killproc -p \"$PIDFILE\" -d 30 \"$DAEMON\"\n\
			RETVAL=\"$?\"\n\
		else\n\
			killproc -d 30 \"$DAEMON\"\n\
			RETVAL=\"$?\"\n\
		fi\n\
		return \"$RETVAL\"\n\
	fi\n\
}\n\
case \"$1\" in\n\
  start)\n\
    [ \"$VERBOSE\" != no ] && log_daemon_msg \"Starting $DESC \" \"$NAME\"\n\
    do_start\n\
    case \"$?\" in\n\
		0|1) [ \"$VERBOSE\" != no ] && log_end_msg 0 ;;\n\
		2) [ \"$VERBOSE\" != no ] && log_end_msg 1 ;;\n\
	esac\n\
  ;;\n\
  stop)\n\
	[ \"$VERBOSE\" != no ] && log_daemon_msg \"Stopping $DESC\" \"$NAME\"\n\
	do_stop\n\
	case \"$?\" in\n\
		0|1) [ \"$VERBOSE\" != no ] && log_end_msg 0 ;;\n\
		2) [ \"$VERBOSE\" != no ] && log_end_msg 1 ;;\n\
	esac\n\
	;;\n\
  status)\n\
		if  fn_exists status_of_proc; then\n\
			status_of_proc \"$DAEMON\" \"$NAME\" && exit 0 || exit $?\n\
		else\n\
			if [ -n $PIDFILE_OPT ]; then\n\
				status -p \"$PIDFILE\" \"$DAEMON\" && exit 0 || exit $?\n\
			else\n\
				status \"$DAEMON\" && exit 0 || exit $?\n\
			fi\n\
		fi\n\
       ;;\n\
  restart|force-reload)\n\
	log_daemon_msg \"Restarting $DESC\" \"$NAME\"\n\
	do_stop\n\
	case \"$?\" in\n\
	  0|1)\n\
		if [ \"$1\" = \"force-reload\" ]; then\n\
			# Rescan the collection\n\
			DAEMON_ARGS=\"$DAEMON_ARGS -R\"\n\
		fi\n\
		do_start\n\
		case \"$?\" in\n\
			0) log_end_msg 0 ;;\n\
			1) log_end_msg 1 ;; # Old process is still running\n\
			*) log_end_msg 1 ;; # Failed to start\n\
		esac\n\
		;;\n\
	  *)\n\
	  	# Failed to stop\n\
		log_end_msg 1\n\
		;;\n\
	esac\n\
	;;\n\
  *)\n\
	echo \"Usage: $SCRIPTNAME {start|stop|status|restart|force-reload}\" >&2\n\
	exit 3\n\
	;;\n\
esac\n\
\n\
:";
namespace rux
{
	namespace service
	{
		namespace engine
		{
			globals::globals( void )
			{
				_with_pid_file = 1;
				_is_interactive = 0;
				_is_autorecovery = 0;
				_service_task = NULL;
				_service_stop = NULL;
				_service_after_stop = NULL;
#if defined( __WINDOWS__ )
				_service_status_handle = NULL;
				_service_stop_handle = NULL;
#else
				_signo = -1;
				_sigpid = 0;
#endif
				_service_name[ 0 ] = 0;
				_service_display_name[ 0 ] = 0;
				_user[ 0 ] = 0;
				_user_group[ 0 ] = 0;
			};
		};
		void initialize( rux_service_task_t service_task , rux_service_stop_t service_stop , rux_service_stop_t service_after_stop )
		{
			rux::engine::_globals->_service_globals->_service_task = service_task;
			rux::engine::_globals->_service_globals->_service_stop = service_stop;
			rux::engine::_globals->_service_globals->_service_after_stop = service_after_stop;
		};
		void set_WithPidFile( rux::uint8 with_pid_file )
		{
			rux::engine::_globals->_service_globals->_with_pid_file = with_pid_file;
		};
		void set_ServiceName( const char* service_name )
		{
			if( service_name && strlen( service_name ) > 0 )
				memcpy( rux::engine::_globals->_service_globals->_service_name , service_name , strlen( service_name ) + 1 );
		};
		void set_ServiceDisplayName( const char* service_display_name )
		{
			if( service_display_name && strlen( service_display_name ) > 0 )
				memcpy( rux::engine::_globals->_service_globals->_service_display_name , service_display_name , strlen( service_display_name ) + 1 );
		};
		void set_User( const char* user )
		{
			if( user && strlen( user ) > 0 )
				memcpy( rux::engine::_globals->_service_globals->_user , user , strlen( user ) + 1 );
		};
		void set_UserGroup( const char* user_group )
		{
			if( user_group && strlen( user_group ) > 0 )
				memcpy( rux::engine::_globals->_service_globals->_user_group , user_group , strlen( user_group ) + 1 );
		};
		void set_Interactive( rux::uint8 is_interactive )
		{
			rux::engine::_globals->_service_globals->_is_interactive = is_interactive;
		};
		void set_Autorecovery( rux::uint8 is_autorecovery )
		{
			rux::engine::_globals->_service_globals->_is_autorecovery = is_autorecovery;
		};
		rux::uint8 Stop( char error[ 1024 ] )
		{
			return rux::service::Stop( rux::engine::_globals->_service_globals->_service_name , error );
		};
		rux::uint8 Stop( const char* service_name , char error[ 1024 ] )
		{
			rux::uint8 is_success = 0;
			error[ 0 ] = 0;
			if( XSystemSecurity::IsAdminUserProcess() == 1 )
			{
#ifdef __WINDOWS__	
				rux::uint32 start_time = GetTickCount();
				rux::uint32 timeout = 30000;
				SC_HANDLE sc_manager_handle = NULL;
				sc_manager_handle = OpenSCManager( NULL , NULL , SC_MANAGER_ALL_ACCESS );
				if( sc_manager_handle )
				{
					SC_HANDLE sc_service_handle = NULL;
					sc_service_handle = OpenServiceA( sc_manager_handle , service_name , SERVICE_STOP | SERVICE_QUERY_STATUS | SERVICE_ENUMERATE_DEPENDENTS );
					if( sc_service_handle )
					{
						SERVICE_STATUS_PROCESS service_status_process;
						unsigned long bytes_needed = 0;
						if( QueryServiceStatusEx( sc_service_handle , SC_STATUS_PROCESS_INFO , (rux::uint8*)&service_status_process , sizeof( SERVICE_STATUS_PROCESS ) , &bytes_needed ) )
						{
							if ( service_status_process.dwCurrentState != SERVICE_STOPPED )
							{
								while ( service_status_process.dwCurrentState == SERVICE_STOP_PENDING ) 
								{						
									rux::threading::XThread::Sleep( service_status_process.dwWaitHint );
									if( QueryServiceStatusEx( sc_service_handle , SC_STATUS_PROCESS_INFO , (rux::uint8*)&service_status_process , sizeof( SERVICE_STATUS_PROCESS ) , &bytes_needed ) == 0 )						
									{
										XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError , error );
										break;
									}
									if( service_status_process.dwCurrentState == SERVICE_STOPPED )
									{
										is_success = 1;
										break;
									}
									if ( GetTickCount() - start_time > timeout )
									{
										rux_concatenation( error , "Timeout waiting for service to stop" );
										break;
									}
								}
								if( is_success == 0 && error[ 0 ] == 0 )
								{
									rux::uint32 index0 = 0;
									unsigned long count = 0;
									LPENUM_SERVICE_STATUS lpenum_service_status = NULL;
									ENUM_SERVICE_STATUS enum_service_status;
									start_time = GetTickCount();
									timeout = 30000;    
									if( EnumDependentServices( sc_service_handle , SERVICE_ACTIVE , lpenum_service_status , 0 , &bytes_needed , &count ) ) 
									{         
									} 
									else 
									{
										if( GetLastError() == ERROR_MORE_DATA )
										{
											lpenum_service_status = (LPENUM_SERVICE_STATUS)HeapAlloc( GetProcessHeap() , HEAP_ZERO_MEMORY , bytes_needed );  
											if( lpenum_service_status )
											{
												try
												{
													if( EnumDependentServices( sc_service_handle , SERVICE_ACTIVE , lpenum_service_status , bytes_needed , &bytes_needed , &count ) )
													{
														for ( index0 = 0; index0 < count; index0++ ) 
														{
															enum_service_status = *( lpenum_service_status + index0 );
															SC_HANDLE sc_dependency_handle = OpenService( sc_manager_handle , enum_service_status.lpServiceName , SERVICE_STOP | SERVICE_QUERY_STATUS );
															if( sc_dependency_handle )
															{
																try 
																{
																	if( ControlService( sc_dependency_handle , SERVICE_CONTROL_STOP , (LPSERVICE_STATUS)&service_status_process ) )
																	{											
																		while ( service_status_process.dwCurrentState != SERVICE_STOPPED ) 
																		{
																			rux::threading::XThread::Sleep( service_status_process.dwWaitHint );
																			if( QueryServiceStatusEx( sc_dependency_handle , SC_STATUS_PROCESS_INFO , (LPBYTE)&service_status_process , sizeof( SERVICE_STATUS_PROCESS ) , &bytes_needed ) )
																			{           
																				if( service_status_process.dwCurrentState == SERVICE_STOPPED )
																					break;
																				if( GetTickCount() - start_time > timeout )
																				{
																					index0 = count;
																					break;
																				}
																			}
																			else
																			{
																				index0 = count;
																				break;
																			}
																		}
																	}
																	else
																		break;
																}
																catch( ... ) 
																{														
																}
																CloseServiceHandle( sc_dependency_handle );
															}
															else
																break;
														}
													}
												}
												catch( ... )
												{										
												}
												HeapFree( GetProcessHeap() , 0 , lpenum_service_status );
											}
										}
									}
									if( ControlService( sc_service_handle , SERVICE_CONTROL_STOP , (LPSERVICE_STATUS)&service_status_process ) )
									{
										while( service_status_process.dwCurrentState != SERVICE_STOPPED ) 
										{
											rux::threading::XThread::Sleep( service_status_process.dwWaitHint );
											if( QueryServiceStatusEx( sc_service_handle , SC_STATUS_PROCESS_INFO , (LPBYTE)&service_status_process , sizeof( SERVICE_STATUS_PROCESS ) , &bytes_needed ) == 0 )
											{																	
												XError::ConvertErrorCodeToString( GetLastError() , XEnum_ErrorType_GetLastError , error );
												break;
											}
											if( service_status_process.dwCurrentState == SERVICE_STOPPED )
												break;
											if ( GetTickCount() - start_time > timeout )
											{
												rux_concatenation( error , "Timeout waiting for service to stop" );
												break;
											}
										}
										if( service_status_process.dwCurrentState == SERVICE_STOPPED ) 				
										{
											error[ 0 ] = 0;
											is_success = 1;
										}
										else
										{
											rux_concatenation( error , "Service " , service_name );
											rux_concatenation( error , " is not stopped" );
										}
									}
									else
										XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError , error );
								}
							}
							else
								is_success = 1;				
						}
						else
							XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError , error );
						CloseServiceHandle( sc_service_handle );
					}
					else
						XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError , error );
					CloseServiceHandle( sc_manager_handle );
				}
				else
					XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError , error );
#elif defined( __LINUX__ )
				declare_stack_variable( char , init_script_filename , 1024 );
				rux_concatenation( init_script_filename , "/etc/init.d/" , service_name );
				if( rux_is_exists_file_or_link( init_script_filename ) == 1 )
				{
					pid_t pid = fork();
					if( pid < 0 )
					{
					}
					else if( pid != 0 )
					{
						rux::int32 status = 0;
						waitpid( pid , &status , 0 );
						if( WIFEXITED( status ) )
							status = WEXITSTATUS( status );
						else
							status = 1;
						if( status != 0 )
						{
							rux_concatenation( error , "process '" );
							rux_concatenation( error , init_script_filename );
							rux_concatenation( error , "' returned a failure" );
						}
						else 
							is_success = 1;
					}
					else
					{
						execl( init_script_filename , service_name , "stop" , (char*)0 );
						_exit( 1 );
					}
				}
				else
				{
					rux_concatenation( error , "file or link '" );
					rux_concatenation( error , init_script_filename );
					rux_concatenation( error , "' does not exist" );
				}
#endif		
			}
			else
				rux_concatenation( error , "process is not executed as root" );
			if( error[ 0 ] != 0 )
			{
				declare_stack_variable( char , error0 , 1024 );
				size_t len = strlen( error );				
				memcpy( error0 , error , len + 1 );
				rux_concatenation( error , __FUNCTION__ , " , " );
				rux_concatenation( error , service_name );
				rux_concatenation( error , " , " );
				rux_concatenation( error , error0 );
			}
			return is_success;
		};
		rux::uint8 DeInstall( char error[ 1024 ] )
		{
			return DeInstall( rux::engine::_globals->_service_globals->_service_name , error );
		};
		rux::uint8 DeInstall( const char* service_name , char error[ 1024 ] )
		{
			error[ 0 ] = 0;
			rux::uint8 is_deinstalled = 0;
			if( XSystemSecurity::IsAdminUserProcess() == 1 )
			{
#ifdef __WINDOWS__
				::rux::diagnostics::process::killall( "mmc" );
				Stop( error );
				error[ 0 ] = 0;
				SC_HANDLE sc_manager_ptr = NULL;	
				sc_manager_ptr = OpenSCManager(	NULL , NULL , SC_MANAGER_ALL_ACCESS );
				if( sc_manager_ptr )
				{
					SC_HANDLE service_ptr = NULL;					
					service_ptr = OpenServiceA( sc_manager_ptr , service_name , SERVICE_ALL_ACCESS );
					if( service_ptr )
					{
						if( DeleteService( service_ptr ) )
							is_deinstalled = 1;
						CloseServiceHandle( service_ptr ); 
					}			
					CloseServiceHandle( sc_manager_ptr );
				}
				if( is_deinstalled == 0 )
					XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError , error );
#elif defined( __LINUX__ )
				declare_stack_variable( char , init_script_filename , 1024 );
				rux_concatenation( init_script_filename , "/etc/init.d/" , service_name );
				if( rux_is_exists_file_or_link( init_script_filename ) == 1 )
				{
					pid_t pid = fork();
					if( pid < 0 )
					{
					}
					else if( pid != 0 )
					{
						rux::int32 status = 0;
						waitpid( pid , &status , 0 );
						if( WIFEXITED( status ) )
							status = WEXITSTATUS( status );
						pid = fork();
						if( pid < 0 )
						{
						}
						else if( pid != 0 )
						{
							status = 0;
							waitpid( pid , &status , 0 );
							if( WIFEXITED( status ) )
								status = WEXITSTATUS( status );
							else
								status = 1;
							if( status == 0 )
								is_deinstalled = 1;
							rux_remove_file( init_script_filename );
						}
						else
						{
							if( rux_is_exists_file_or_link( "/usr/sbin/update-rc.d" ) )
								execlp( "/usr/sbin/update-rc.d" , "/usr/sbin/update-rc.d" , "-f" , service_name , "remove" , (char*)0 );
							else if( rux_is_exists_file_or_link( "/sbin/chkconfig" ) )
								execlp( "/sbin/chkconfig" , "/sbin/chkconfig" , "--del" , service_name , (char*)0 );
							_exit( 1 );
						}
					}
					else
					{
						execl( init_script_filename , service_name , "stop" , (char*)0 );
						_exit( 1 );
					}
				}
				else
				{
					rux_concatenation( error , "file or link '" );
					rux_concatenation( error , init_script_filename );
					rux_concatenation( error , "' does not exist" );
				}
#endif
			}
			else
				rux_concatenation( error , "process is not executed as root" );
			if( error[ 0 ] != 0 )
			{
				declare_stack_variable( char , error0 , 1024 );
				size_t len = strlen( error );				
				memcpy( error0 , error , len + 1 );
				rux_concatenation( error , __FUNCTION__ , " , " );
				rux_concatenation( error , service_name );
				rux_concatenation( error , " , " );
				rux_concatenation( error , error0 );
			}
			return is_deinstalled;
		};
		rux::uint8 StartByName( const char* service_name , char error[ 1024 ] )
		{
			error[ 0 ] = 0;
			rux::uint8 is_success = 0;
			if( XSystemSecurity::IsAdminUserProcess() == 1 )
			{
#ifdef __WINDOWS__
				SC_HANDLE sc_manager_handle = NULL;
				sc_manager_handle = OpenSCManager( NULL , NULL , SC_MANAGER_ALL_ACCESS );
				if( sc_manager_handle )
				{
					SC_HANDLE sc_service_handle = NULL;
					sc_service_handle = OpenServiceA( sc_manager_handle , service_name , SERVICE_ALL_ACCESS );
					if( sc_service_handle )
					{
						SERVICE_STATUS_PROCESS service_status_process;
						unsigned long bytes_needed = 0;
						if( QueryServiceStatusEx( sc_service_handle , SC_STATUS_PROCESS_INFO , (rux::uint8*)&service_status_process , sizeof( SERVICE_STATUS_PROCESS ) , &bytes_needed ) )
						{
							if( service_status_process.dwCurrentState == SERVICE_STOPPED 
								|| service_status_process.dwCurrentState == SERVICE_STOP_PENDING)
							{
								rux::uint32 start_tick_count = 0;
								rux::uint32 old_check_point = 0;
								rux::uint32 wait_time = 0;
								while( service_status_process.dwCurrentState == SERVICE_STOP_PENDING )
								{
									start_tick_count = GetTickCount();
									old_check_point = service_status_process.dwCheckPoint;
									wait_time = service_status_process.dwWaitHint / 10;
									if( wait_time < 1000 )
										wait_time = 1000;
									else if ( wait_time > 10000 )
										wait_time = 10000;
									rux::threading::XThread::Sleep( wait_time );
									if( QueryServiceStatusEx( sc_service_handle , SC_STATUS_PROCESS_INFO , (rux::uint8*)&service_status_process , sizeof( SERVICE_STATUS_PROCESS ) , &bytes_needed ) == 0 )
									{
										XError::ConvertErrorCodeToString( GetLastError() , XEnum_ErrorType_GetLastError , error );
										break;
									}
									if( service_status_process.dwCheckPoint > old_check_point )        
									{
										start_tick_count = GetTickCount();
										old_check_point = service_status_process.dwCheckPoint;
									}
									else
									{
										if( GetTickCount() - start_tick_count > service_status_process.dwWaitHint )
										{
											rux_concatenation( error , "Timeout waiting for service to stop" );
											break;
										}
									}
								}
								if( error[ 0 ] == 0 )
								{
									 if( StartService( sc_service_handle , 0 , NULL) )
									 {
										 if( QueryServiceStatusEx( sc_service_handle , SC_STATUS_PROCESS_INFO , (rux::uint8*)&service_status_process , sizeof( SERVICE_STATUS_PROCESS ) , &bytes_needed ) )
										 {
											 start_tick_count = GetTickCount();
											 old_check_point = service_status_process.dwCheckPoint;
											 while( service_status_process.dwCurrentState == SERVICE_START_PENDING ) 
											 { 
												 wait_time = service_status_process.dwWaitHint / 10;
												 if( wait_time < 1000 )
													 wait_time = 1000;
												 else if ( wait_time > 10000 )
													 wait_time = 10000;
												 rux::threading::XThread::Sleep( wait_time );
												 if( QueryServiceStatusEx( sc_service_handle , SC_STATUS_PROCESS_INFO , (rux::uint8*)&service_status_process , sizeof( SERVICE_STATUS_PROCESS ) , &bytes_needed ) == 0 )
												 {
													 XError::ConvertErrorCodeToString( GetLastError() , XEnum_ErrorType_GetLastError , error );
													 break;
												 }
												 if( service_status_process.dwCheckPoint > old_check_point )        
												 {
													 start_tick_count = GetTickCount();
													 old_check_point = service_status_process.dwCheckPoint;
												 }
												 else
												 {
													 if( GetTickCount() - start_tick_count > service_status_process.dwWaitHint )
														 break;
												 }
											 } 
											 if( error[ 0 ] == 0 )
											 {
												 if( service_status_process.dwCurrentState == SERVICE_RUNNING ) 				
												 {
													 error[ 0 ] = 0;
													 is_success = 1;
												 }
												 else
												 {	
													 rux::threading::XThread::Sleep( 10000 );
													 if( service_status_process.dwCurrentState == SERVICE_RUNNING ) 				
													 {
														 error[ 0 ] = 0;
														 is_success = 1;
													 }
													 else
													 {
														 rux_concatenation( error , "Service " , service_name );
														 rux_concatenation( error , " is not started" );
														  if( service_status_process.dwCurrentState == SERVICE_STOPPED )
															 rux_concatenation( error , " , state stopped" );
														 else if( service_status_process.dwCurrentState == SERVICE_START_PENDING )
															 rux_concatenation( error , " , state start pending" );
														 else if( service_status_process.dwCurrentState == SERVICE_STOP_PENDING )
															 rux_concatenation( error , " , state stop pending" );
														 else if( service_status_process.dwCurrentState == SERVICE_CONTINUE_PENDING )
															 rux_concatenation( error , " , state continue pending" );
														 else if( service_status_process.dwCurrentState == SERVICE_PAUSE_PENDING )
															 rux_concatenation( error , " , state pause pending" );
														 else if( service_status_process.dwCurrentState == SERVICE_PAUSED )
															 rux_concatenation( error , " , state paused" );
													 }
												 }
											 }
										 }
										 else
											 XError::ConvertErrorCodeToString( GetLastError() , XEnum_ErrorType_GetLastError , error );							 
									 }
									 else
										 XError::ConvertErrorCodeToString( GetLastError() , XEnum_ErrorType_GetLastError , error );							 
								}
							}
							else
								is_success = 1;				
						}
						else
							XError::ConvertErrorCodeToString( GetLastError() , XEnum_ErrorType_GetLastError , error );							 
						CloseServiceHandle( sc_service_handle );
					}
					else
						XError::ConvertErrorCodeToString( GetLastError() , XEnum_ErrorType_GetLastError , error );
					CloseServiceHandle( sc_manager_handle );
				}
				else	
					XError::ConvertErrorCodeToString( GetLastError() , XEnum_ErrorType_GetLastError , error );
#elif defined( __LINUX__ )
				declare_stack_variable( char , init_script_filename , 1024 );
				rux_concatenation( init_script_filename , "/etc/init.d/" , service_name );
				if( rux_is_exists_file_or_link( init_script_filename ) == 1 )
				{
					pid_t pid = fork();
					if( pid < 0 )
					{
					}
					else if( pid != 0 )
					{
						rux::int32 status = 0;
						waitpid( pid , &status , 0 );
						if( WIFEXITED( status ) )
							status = WEXITSTATUS( status );
						else
							status = 1;
						if( status != 0 )
						{
							rux_concatenation( error , "process '" );
							rux_concatenation( error , init_script_filename );
							rux_concatenation( error , "' returned a failure" );
						}
						else
							is_success = 1;
					}
					else
					{
						execl( init_script_filename , service_name , "start" , (char*)0 );
						_exit( 1 );
					}
				}
				else
				{
					rux_concatenation( error , "file or link '" );
					rux_concatenation( error , init_script_filename );
					rux_concatenation( error , "' does not exist" );
				}
#endif
			}
			else
				rux_concatenation( error , "process is not executed as root" );
			if( error[ 0 ] != 0 )
			{
				declare_stack_variable( char , error0 , 1024 );
				size_t len = strlen( error );				
				memcpy( error0 , error , len + 1 );
				rux_concatenation( error , __FUNCTION__ , " , " );
				rux_concatenation( error , service_name );
				rux_concatenation( error , " , " );
				rux_concatenation( error , error0 );
			}
			return is_success;
		};
		rux::uint8 CanStart( void )
		{
			rux::uint8 can_start = 1;
			declare_stack_variable( char , filename , 1024 );
			rux_concatenation( filename , "/var/run/" , rux::engine::_globals->_service_globals->_service_name );
			rux_concatenation( filename , "/" );
			rux_concatenation( filename , rux::engine::_globals->_service_globals->_service_name );
			rux_concatenation( filename , ".pid" );	
			rux::io::file file( filename );
			if( file.opened() )
			{				
				declare_stack_variable( char , pid_string , 64 );
				size_t readen = 0;
				if( file.read_text( pid_string , 64 , readen ) )
				{
					if( pid_string[ readen - 1 ] == 0 )
					{
						rux::uint64 pid = rux::string::to_uint64( pid_string );
						declare_stack_variable( char , modulename , 1024 );
						rux_get_executable_filename( modulename );				
						if( rux::diagnostics::process::executing( modulename , pid ) )
							can_start = 0;
					}
				}
			}			
			return can_start;
		};
		void writepid_to_var(::booldog::result_mbchar* pidfilembchar, ::booldog::result_mbchar* mbchar0)
		{
			if(rux::engine::_globals->_service_globals->_service_name
				&& rux::engine::_globals->_service_globals->_service_name[ 0 ] != 0
				&& rux::engine::_globals->_service_globals->_with_pid_file == 1)
			{
				::booldog::result_file resfile;
				::booldog::utils::string::mbs::assign<16>(0, pidfilembchar->mballocator, false, 0, pidfilembchar->mbchar
					, pidfilembchar->mblen, pidfilembchar->mbsize, "/var/run/", 0, SIZE_MAX);
				::booldog::utils::string::mbs::assign<16>(0, pidfilembchar->mballocator, false, pidfilembchar->mblen
					, pidfilembchar->mbchar, pidfilembchar->mblen, pidfilembchar->mbsize
					, rux::engine::_globals->_service_globals->_service_name, 0, SIZE_MAX);
				::booldog::utils::string::mbs::assign<16>(0, pidfilembchar->mballocator, false, pidfilembchar->mblen, pidfilembchar->mbchar
					, pidfilembchar->mblen, pidfilembchar->mbsize, "/", 0, SIZE_MAX);
				::booldog::utils::string::mbs::assign<16>(0, pidfilembchar->mballocator, false, pidfilembchar->mblen, pidfilembchar->mbchar
					, pidfilembchar->mblen, pidfilembchar->mbsize, rux::engine::_globals->_service_globals->_service_name, 0, SIZE_MAX);
				::booldog::utils::string::mbs::assign<16>(0, pidfilembchar->mballocator, false, pidfilembchar->mblen, pidfilembchar->mbchar
					, pidfilembchar->mblen, pidfilembchar->mbsize, ".pid", 0, SIZE_MAX);

				if(::booldog::io::file::mbsopen(&resfile, pidfilembchar->mballocator, pidfilembchar->mbchar
					, ::booldog::enums::io::file_mode_truncate|::booldog::enums::io::file_mode_write
					, 0) == false)
				{
					::booldog::io::file::mbsopen(&resfile, pidfilembchar->mballocator, pidfilembchar->mbchar
						, ::booldog::enums::io::file_mode_create|::booldog::enums::io::file_mode_truncate|::booldog::enums::io::file_mode_write
						, 0);
				}
				if(resfile.succeeded())
				{					
					::booldog::utils::string::mbs::sprintf(mbchar0, mbchar0->mballocator, debuginfo_macros, "%u"
						, (::booldog::uint32)getpid());

					size_t written = 0;
					resfile.file->write(0, (::booldog::byte*)mbchar0->mbchar, mbchar0->mblen, SIZE_MAX, written);
					resfile.file->close( &resfile );
				}
			}
		};
		void remove_pid_to_var(::booldog::result_mbchar* pidfilembchar)
		{
			::booldog::result_file resfile;
			if(::booldog::io::file::mbsopen(&resfile, pidfilembchar->mballocator, pidfilembchar->mbchar
				, ::booldog::enums::io::file_mode_read, 0))
			{	
				::booldog::result_buffer resbuf(pidfilembchar->mballocator);
				if(resfile.file->readall<16>(&resbuf, resbuf.allocator))
				{
					resfile.file->close(0);
					const ::booldog::byte* ptr = resbuf.buf;
					::booldog::pid_t pid = ::booldog::utils::decimal_string_to_number< ::booldog::pid_t >(ptr);
					if(getpid() == pid)
						::booldog::utils::io::mbs::remove(0, pidfilembchar->mbchar);
				}
				else
					resfile.file->close(0);
			}
		};
		void boowritelog(::booldog::result_mbchar* dst, ::booldog::result_mbchar* mbchar, const char* format, ...)
		{
			::booldog::uint64 now = ::booldog::utils::time::posix::now_as_utc();

			::booldog::result_file resfile;
			::booldog::param filesearch_paths_params[] =
			{
				BOOPARAM_PCHAR(""),
				BOOPARAM_NONE
			};
			::booldog::named_param fileload_params[] =
			{
				BOONAMED_PARAM_PPARAM("search_paths", filesearch_paths_params),
				BOONAMED_PARAM_BOOL("exedir_as_root_path", true),
				BOONAMED_PARAM_NONE
			};

			if(::booldog::io::file::mbsopen(&resfile, dst->mballocator, "service.daemon.logs.txt"
				, ::booldog::enums::io::file_mode_append|::booldog::enums::io::file_mode_write
				, fileload_params) == false)
			{
				::booldog::io::file::mbsopen(&resfile, dst->mballocator, "service.daemon.logs.txt"
					, ::booldog::enums::io::file_mode_create|::booldog::enums::io::file_mode_append|::booldog::enums::io::file_mode_write
					, fileload_params);
			}
			if(resfile.succeeded())
			{					
				now = ::booldog::utils::time::posix::tolocal(now);

				va_list ap;
				va_start(ap, format);
				::booldog::utils::string::mbs::sprintf(mbchar, mbchar->mballocator, format, ap, debuginfo_macros);
				va_end(ap);				
						
				::booldog::utils::time::posix::mbs::tostring<16>(dst, dst->mballocator, "%Y%m%d %H:%M:%S,%MS - ", now);

				::booldog::utils::string::mbs::assign<16>(0, dst->mballocator, false, dst->mblen, dst->mbchar, dst->mblen, dst->mbsize
					, mbchar->mbchar, 0, SIZE_MAX);

				::booldog::utils::string::mbs::assign<16>(0, dst->mballocator, false, dst->mblen, dst->mbchar, dst->mblen, dst->mbsize
					, "\n", 0, SIZE_MAX);

				size_t written = 0;
				resfile.file->write(0, (::booldog::byte*)dst->mbchar, dst->mblen, SIZE_MAX, written);
				resfile.file->close( &resfile );
			}
		};
		rux::uint8 Start( const char* logfile , char error[ 1024 ] , rux::uint8 check_rux_executing_in_current_path )
		{
			::booldog::allocators::easy::heap heap;
			::booldog::allocators::single_threaded::mixed<1024> mixed(&heap);
			{
				::booldog::result_mbchar mbchar0(&mixed), mbchar1(&mixed), pidfilembchar(&mixed);
				
				boowritelog(&mbchar0, &mbchar1, "Start service/daemon %s", ::rux::engine::_globals->_service_globals->_service_name);

				error[ 0 ] = 0;
				rux::uint8 is_started = 0;
	#ifdef	__UNIX__
				const char* termsignal = 0;
				if(rux::engine::_globals->_service_globals->_is_autorecovery == 1)
				{
					pid_t parent_pid = fork();
					if(parent_pid < 0)
						exit(1);
					else if(parent_pid != 0)
						exit(0);
					else
					{
						umask(0);
						pid_t sid = setsid();
						if(sid < 0)
							exit(1);	
						if(chdir("/") < 0)
							exit(1);
						close(STDIN_FILENO);
						close(STDOUT_FILENO);
						close(STDERR_FILENO);
						rux::int32 stdio_fd = open("/dev/null", O_RDWR);
						dup( stdio_fd );
						dup( stdio_fd );

						writepid_to_var(&pidfilembchar, &mbchar0);						
					}
				}
				::rux::byte restart = 1, stop_service_write_log = 0;
				while( restart )
				{
					pid_t sid = -1;		
					pid_t parent_pid = fork();
					if(parent_pid < 0)
						exit(1);
					else if(parent_pid != 0)
					{
						if( rux::engine::_globals->_service_globals->_is_autorecovery == 1 )
						{
							boowritelog(&mbchar0, &mbchar1, "Child process(%u) started", parent_pid);

							sigset_t sigset;
							sigemptyset(&sigset);
							sigaddset(&sigset, SIGQUIT);
							sigaddset(&sigset, SIGINT);
							sigaddset(&sigset, SIGTERM);
							sigprocmask(SIG_BLOCK, &sigset, NULL);

							::booldog::pid_t sigpid = 0;
							int signo = -1;
							rux::int32 status = 0, tries = 0;
							for(;;)
							{
								if(signo == -1)
								{
									struct timespec ts = {0};
									ts.tv_sec = 0;
									ts.tv_nsec = 100000000;
									siginfo_t siginfo;
									signo = sigtimedwait(&sigset, &siginfo, &ts);
									if(signo != -1)
									{
										sigpid = siginfo.si_pid;

										termsignal = "unknown";
										switch(WTERMSIG(status))
										{
										case SIGSEGV: termsignal = "SIGSEGV";break;
										case SIGILL: termsignal = "SIGILL";break;
										case SIGFPE: termsignal = "SIGFPE";break;
										case SIGABRT: termsignal = "SIGABRT";break;
										case SIGBUS: termsignal = "SIGBUS";break;
										case SIGHUP: termsignal = "SIGHUP";break;
										case SIGPIPE: termsignal = "SIGPIPE";break;
										case SIGTSTP: termsignal = "SIGTSTP";break;
										case SIGTTIN: termsignal = "SIGTTIN";break;
										case SIGTTOU: termsignal = "SIGTTOU";break;
										case SIGUSR1: termsignal = "SIGUSR1";break;
										case SIGUSR2: termsignal = "SIGUSR2";break;
										case SIGPROF: termsignal = "SIGPROF";break;
										case SIGSYS: termsignal = "SIGSYS";break;
										case SIGVTALRM: termsignal = "SIGVTALRM";break;
										case SIGXCPU: termsignal = "SIGXCPU";break;
										case SIGXFSZ: termsignal = "SIGXFSZ";break;
										case SIGKILL: termsignal = "SIGKILL";break;
										}
										boowritelog(&mbchar0, &mbchar1, "Parent process(%u) terminated by process(%u), signal %s"
										, (::booldog::uint32)getpid(), (::booldog::uint32)sigpid, termsignal);
										
										kill(parent_pid, SIGTERM);

										boowritelog(&mbchar0, &mbchar1, "Parent process(%u) send SIGTERM to child process(%u)"
											, (::booldog::uint32)getpid(), parent_pid);
									}
								}
								else
								{
									::booldog::threading::sleep(1000);
									++tries;
									if(tries >= 25)
									{
										kill(parent_pid, SIGKILL);
										boowritelog(&mbchar0, &mbchar1, "Parent process(%u) send SIGKILL to child process(%u)"
											, (::booldog::uint32)getpid(), parent_pid);
									}
								}
								int waitpidres = waitpid(parent_pid, &status, WUNTRACED|WCONTINUED|WNOHANG);
								if(waitpidres == parent_pid)
								{
									if(WIFSTOPPED(status))
									{
										boowritelog(&mbchar0, &mbchar1, "Child process(%u) stopped by signal SIGSTOP"
											, (::booldog::uint32)parent_pid);
										continue;
									}
									else if(WIFCONTINUED(status))
									{
										boowritelog(&mbchar0, &mbchar1, "Child process(%u) continued by signal SIGCONT"
											, (::booldog::uint32)parent_pid);
										continue;
									}
									else
									{
										boowritelog(&mbchar0, &mbchar1, "Child process(%u) terminated", parent_pid);
										break;
									}
								}
							}
							if(signo != -1)
								restart = 0;
							else if(WIFSIGNALED(status))
							{
								termsignal = "unknown";
								switch(WTERMSIG(status))
								{
								case SIGSEGV: termsignal = "SIGSEGV";break;
								case SIGILL: termsignal = "SIGILL";break;
								case SIGFPE: termsignal = "SIGFPE";break;
								case SIGABRT: termsignal = "SIGABRT";break;
								case SIGBUS: termsignal = "SIGBUS";break;
								case SIGHUP: termsignal = "SIGHUP";break;
								case SIGPIPE: termsignal = "SIGPIPE";break;
								case SIGTSTP: termsignal = "SIGTSTP";break;
								case SIGTTIN: termsignal = "SIGTTIN";break;
								case SIGTTOU: termsignal = "SIGTTOU";break;
								case SIGUSR1: termsignal = "SIGUSR1";break;
								case SIGUSR2: termsignal = "SIGUSR2";break;
								case SIGPROF: termsignal = "SIGPROF";break;
								case SIGSYS: termsignal = "SIGSYS";break;
								case SIGVTALRM: termsignal = "SIGVTALRM";break;
								case SIGXCPU: termsignal = "SIGXCPU";break;
								case SIGXFSZ: termsignal = "SIGXFSZ";break;
								case SIGKILL: termsignal = "SIGKILL";break;
								}
								boowritelog(&mbchar0, &mbchar1, "Child process(%u) terminated by signal %s"
								, (::booldog::uint32)parent_pid, termsignal);
							}
						}
						else
							exit( 0 );
					}
					else
					{
						if(rux::engine::_globals->_service_globals->_is_autorecovery == 1)
							stop_service_write_log = 1;
						restart = 0;
						::rux::engine::initialize();
						if(rux::engine::_globals->_service_globals->_is_autorecovery == 1
							|| rux::service::CanStart() == 1)
						{
							if( check_rux_executing_in_current_path == 0 
								|| rux_is_already_executing_in_current_path() == 0 )
							{
								rux::engine::_globals->_service_globals->_signo = -1;
								is_started = 1;

								umask( 0 );
								sid = setsid();
								if( sid < 0 )
									exit( 1 );	
								if( chdir( "/" ) < 0 )
									exit( 1 );
								close( STDIN_FILENO );
								close( STDOUT_FILENO );
								close( STDERR_FILENO );
								rux::int32 stdio_fd = open( "/dev/null" , O_RDWR );
								dup( stdio_fd );
								dup( stdio_fd );

								struct rlimit lim;
								lim.rlim_cur = 16384;
								lim.rlim_max = 16384;
								int setrlimit_res = setrlimit( RLIMIT_NOFILE , &lim );
								if(setrlimit_res == -1)
									boowritelog(&mbchar0, &mbchar1, "setrlimit, error(%d)", (int)errno);
								sigemptyset( &rux::engine::_globals->_service_globals->_sigset );
								sigaddset( &rux::engine::_globals->_service_globals->_sigset , SIGQUIT );
								sigaddset( &rux::engine::_globals->_service_globals->_sigset , SIGINT );
								sigaddset( &rux::engine::_globals->_service_globals->_sigset , SIGTERM );
								sigprocmask( SIG_BLOCK , &rux::engine::_globals->_service_globals->_sigset , NULL );
								
								if(rux::engine::_globals->_service_globals->_is_autorecovery == 0)
									writepid_to_var(&pidfilembchar, &mbchar0);

								THREAD_REGISTER( "main" );
								if( rux::engine::_globals->_service_globals->_service_task )
								{
									CODE_LABEL( NULL , NULL , 100 );
									rux::int32 argc = 0;
									char** args = NULL;
									rux::engine::_globals->_service_globals->_service_task( argc , args );
								}
								if( rux::engine::_globals->_service_globals->_signo == -1 )
								{
									siginfo_t siginfo;
									rux::engine::_globals->_service_globals->_signo = sigwaitinfo( 
										&rux::engine::_globals->_service_globals->_sigset , &siginfo );
									rux::engine::_globals->_service_globals->_sigpid = siginfo.si_pid;
								}
								{
									const char* termsignal = "unknown";
									switch(rux::engine::_globals->_service_globals->_signo)
									{
									case SIGSEGV: termsignal = "SIGSEGV";break;
									case SIGILL: termsignal = "SIGILL";break;
									case SIGFPE: termsignal = "SIGFPE";break;
									case SIGABRT: termsignal = "SIGABRT";break;
									case SIGBUS: termsignal = "SIGBUS";break;
									case SIGHUP: termsignal = "SIGHUP";break;
									case SIGPIPE: termsignal = "SIGPIPE";break;
									case SIGTSTP: termsignal = "SIGTSTP";break;
									case SIGTTIN: termsignal = "SIGTTIN";break;
									case SIGTTOU: termsignal = "SIGTTOU";break;
									case SIGUSR1: termsignal = "SIGUSR1";break;
									case SIGUSR2: termsignal = "SIGUSR2";break;
									case SIGPROF: termsignal = "SIGPROF";break;
									case SIGSYS: termsignal = "SIGSYS";break;
									case SIGVTALRM: termsignal = "SIGVTALRM";break;
									case SIGXCPU: termsignal = "SIGXCPU";break;
									case SIGXFSZ: termsignal = "SIGXFSZ";break;
									case SIGKILL: termsignal = "SIGKILL";break;
									}
									boowritelog(&mbchar0, &mbchar1, "Child process(%u) terminated by process(%u), signal %s"
										, (::booldog::uint32)getpid(), (::rux::uint32)rux::engine::_globals->_service_globals->_sigpid
										, termsignal);
								}
								if( rux::engine::_globals->_service_globals->_service_stop )
									rux::engine::_globals->_service_globals->_service_stop();	
								if(rux::engine::_globals->_service_globals->_is_autorecovery == 0
									&& pidfilembchar.mbchar)
									remove_pid_to_var(&pidfilembchar);
							}
							else
								boowritelog(&mbchar0, &mbchar1, "Service/daemon %s already executing in current path"
								, ::rux::engine::_globals->_service_globals->_service_name);
						}
						if( ::rux::engine::_globals && ::rux::engine::_globals->_rux_stop_threads )
							::rux::engine::_globals->_rux_stop_threads();
						if( rux::engine::_globals->_service_globals->_service_after_stop )
							rux::engine::_globals->_service_globals->_service_after_stop();
					}
				}
				if(rux::engine::_globals->_service_globals->_is_autorecovery == 1
					&& pidfilembchar.mbchar)
					remove_pid_to_var(&pidfilembchar);
				if(stop_service_write_log == 0)
					boowritelog(&mbchar0, &mbchar1, "Stop service/daemon %s", ::rux::engine::_globals->_service_globals->_service_name);				
	#endif
	#ifdef __WINDOWS__
				::rux::engine::initialize();
				if( check_rux_executing_in_current_path == 0 
					|| rux_is_already_executing_in_current_path() == 0 )
				{
					signal( SIGTERM , rux::service::posix_signal );		
					signal( SIGINT , rux::service::posix_signal );
					signal( SIGBREAK , rux::service::posix_signal );
					SERVICE_TABLE_ENTRYA DispatchTable[] = 
					{ 
						{ rux::engine::_globals->_service_globals->_service_name , (LPSERVICE_MAIN_FUNCTIONA)rux::service::service_main } , 
						{ NULL , NULL }
					};			
					if( StartServiceCtrlDispatcherA( DispatchTable ) == 0 )
						rux::service::private_report_event();
					else
						is_started = 1;
				}
				else
				{
					boowritelog(&mbchar0, &mbchar1, "Service/daemon %s already executing in current path", ::rux::engine::_globals->_service_globals->_service_name);
					::rux::service::private_report_error_event( ::rux::engine::_globals->_service_globals->_service_name
					, "already executing in current path" );
				}
	#endif
				return is_started;
			}
		};
		bool WaitForExit( void )
		{
#ifdef __WINDOWS__
			return WaitForSingleObject( rux::engine::_globals->_service_globals->_service_stop_handle , 1 ) == WAIT_OBJECT_0;
#else
			if( rux::engine::_globals->_service_globals->_signo == -1 )
			{
				struct timespec ts = {0};
				ts.tv_sec = 0;
				ts.tv_nsec = 1000000;
				siginfo_t siginfo;
				rux::engine::_globals->_service_globals->_signo = sigtimedwait( &rux::engine::_globals->_service_globals->_sigset
					, &siginfo , &ts );
				if( rux::engine::_globals->_service_globals->_signo != -1 )
					rux::engine::_globals->_service_globals->_sigpid = siginfo.si_pid;
			}
			return rux::engine::_globals->_service_globals->_signo != -1;
#endif
		};
		void private_report_info_event( const char* app , const char* information )
		{
#ifdef __WINDOWS__
			HANDLE event_source = NULL;			
			event_source = RegisterEventSourceA( NULL , app ? app : "RUX" );
			if( event_source )
			{
				const char* strings[ 2 ] = {0};
				strings[ 0 ] = app ? app : "RUX";
				strings[ 1 ] = information;
				ReportEventA( event_source , EVENTLOG_INFORMATION_TYPE , 0 , 1 , NULL , 2 , 0 , strings , NULL );
				DeregisterEventSource( event_source );
			}
#else
			information = information;
#endif
		};
		void private_report_error_event( const char* app , const char* information )
		{
#ifdef __WINDOWS__
			HANDLE event_source = NULL;			
			event_source = RegisterEventSourceA( NULL , app ? app : "RUX" );
			if( event_source )
			{
				const char* strings[ 2 ] = {0};
				strings[ 0 ] = app ? app : "RUX";
				strings[ 1 ] = information;
				ReportEventA( event_source , EVENTLOG_ERROR_TYPE , 0 , 1 , NULL , 2 , 0 , strings , NULL );
				DeregisterEventSource( event_source );
			}
#else
			information = information;
#endif
		};
#ifdef __WINDOWS__
		void posix_signal( ::rux::int32 signum )
		{
			rux::service::private_report_status( SERVICE_STOP_PENDING , NO_ERROR , 10000 );
			if( rux::engine::_globals->_service_globals->_service_stop )
				rux::engine::_globals->_service_globals->_service_stop();
			SetEvent( rux::engine::_globals->_service_globals->_service_stop_handle );
		};
		void private_report_event( void ) 
		{
			HANDLE event_source = NULL;			
			event_source = RegisterEventSourceA( NULL , rux::engine::_globals->_service_globals->_service_name );
			if( event_source )
			{
				declare_stack_variable( char , error , 1024 );
				XError::ConvertErrorCodeToString( GetLastError() , XEnum_ErrorType_GetLastError , error );
				const char* strings[ 2 ] = {0};
				strings[ 0 ] = rux::engine::_globals->_service_globals->_service_name;
				strings[ 1 ] = error;
				ReportEventA( event_source , EVENTLOG_ERROR_TYPE , 0 , 1 , NULL , 2 , 0 , strings , NULL );
				DeregisterEventSource( event_source );
			}
		};
		void WINAPI service_main( DWORD , char** )
		{	
			::booldog::allocators::easy::heap heap;
			::booldog::allocators::single_threaded::mixed<1024> mixed(&heap);
			{
				::booldog::result_mbchar mbchar0(&mixed), mbchar1(&mixed);

				::rux::service::private_report_info_event( ::rux::engine::_globals->_service_globals->_service_name
					, "service_main" );
				THREAD_REGISTER( "main" );
				rux::engine::_globals->_service_globals->_service_status_handle = RegisterServiceCtrlHandlerA( rux::engine::_globals->_service_globals->_service_name , rux::service::service_ctrl_handler );
				if( rux::engine::_globals->_service_globals->_service_status_handle == NULL )
				{ 
					rux::service::private_report_event();
					return; 
				}
				else
				{
					rux::engine::_globals->_service_globals->_service_status.dwServiceType = SERVICE_WIN32_OWN_PROCESS; 
					rux::engine::_globals->_service_globals->_service_status.dwServiceSpecificExitCode = 0;    
					rux::service::private_report_status( SERVICE_START_PENDING, NO_ERROR , 10000 );		
					rux::engine::_globals->_service_globals->_service_stop_handle = CreateEvent( NULL , TRUE , FALSE , NULL );
					if( rux::engine::_globals->_service_globals->_service_stop_handle == NULL )
					{
						rux::service::private_report_status( SERVICE_STOPPED , NO_ERROR , 0 );
						return;
					}
					rux::service::private_report_status( SERVICE_RUNNING , NO_ERROR , 0 );
					if( rux::engine::_globals->_service_globals->_service_task )
					{
						CODE_LABELS_INITIALIZE();
						CODE_LABEL( NULL , NULL , 100 );
						rux::int32 argc = 0;
						char** args = NULL;
						rux::engine::_globals->_service_globals->_service_task( argc , args );
					}
					WaitForSingleObject( rux::engine::_globals->_service_globals->_service_stop_handle , ::rux::threading::infinite );				
					rux::service::private_report_status( SERVICE_STOPPED, NO_ERROR, 0 );
					CloseHandle( rux::engine::_globals->_service_globals->_service_stop_handle );
					rux::engine::_globals->_service_globals->_service_stop_handle = NULL;
				}			
				if( ::rux::engine::_globals && ::rux::engine::_globals->_rux_stop_threads )
					::rux::engine::_globals->_rux_stop_threads();
				if( rux::engine::_globals->_service_globals->_service_after_stop )
					rux::engine::_globals->_service_globals->_service_after_stop();

				boowritelog(&mbchar0, &mbchar1, "Stop service/daemon %s", ::rux::engine::_globals->_service_globals->_service_name);
			}
		};
		void private_report_status( rux::uint32 current_state , rux::uint32 exit_code , rux::uint32 wait_hint )
		{			
			rux::engine::_globals->_service_globals->_service_status.dwCurrentState = current_state;
			rux::engine::_globals->_service_globals->_service_status.dwWin32ExitCode = exit_code;
			rux::engine::_globals->_service_globals->_service_status.dwWaitHint = wait_hint;
			if( current_state == SERVICE_START_PENDING )
				rux::engine::_globals->_service_globals->_service_status.dwControlsAccepted = 0;
			else
				rux::engine::_globals->_service_globals->_service_status.dwControlsAccepted = SERVICE_ACCEPT_STOP;
			if( ( current_state == SERVICE_RUNNING ) || ( current_state == SERVICE_STOPPED ) )
				rux::engine::_globals->_service_globals->_service_status.dwCheckPoint = 0;
			else
			{
				static DWORD dwCheckPoint = 1;
				rux::engine::_globals->_service_globals->_service_status.dwCheckPoint = dwCheckPoint++;
			}
			if( SetServiceStatus( rux::engine::_globals->_service_globals->_service_status_handle 
				, &rux::engine::_globals->_service_globals->_service_status ) == 0 )
				::rux::service::private_report_event();
		};
		void WINAPI service_ctrl_handler( DWORD ctrl )
		{
			switch( ctrl ) 
			{  
			case SERVICE_CONTROL_STOP: 
				{
					rux::service::private_report_status( SERVICE_STOP_PENDING , NO_ERROR , 10000 );
					if( rux::engine::_globals->_service_globals->_service_stop )
						rux::engine::_globals->_service_globals->_service_stop();
					SetEvent( rux::engine::_globals->_service_globals->_service_stop_handle );
					return;
				}
			case SERVICE_CONTROL_INTERROGATE: 
				{
					break; 
				}
			default: 
				{
					break;
				}
			} 
			rux::service::private_report_status( rux::engine::_globals->_service_globals->_service_status.dwCurrentState , NO_ERROR , 0 );
		};
#endif
		rux::uint8 Install( char error[ 1024 ] )
		{			
			error[ 0 ] = 0;
			rux::uint8 is_installed = 0;
			if( XSystemSecurity::IsAdminUserProcess() == 1 )
			{
#ifdef __WINDOWS__
				::rux::diagnostics::process::killall( "mmc" );
				declare_stack_variable( char , path_ptr , MAX_PATH );
				if( GetModuleFileNameA( NULL , path_ptr , MAX_PATH ) )
				{
					SC_HANDLE sc_manager_ptr = OpenSCManager(	NULL , NULL , SC_MANAGER_ALL_ACCESS );
					if( sc_manager_ptr )
					{
						SC_HANDLE service_ptr = NULL;						
						service_ptr = CreateServiceA( sc_manager_ptr , rux::engine::_globals->_service_globals->_service_name , rux::engine::_globals->_service_globals->_service_display_name , SERVICE_ALL_ACCESS , SERVICE_WIN32_OWN_PROCESS , SERVICE_AUTO_START , SERVICE_ERROR_NORMAL , path_ptr , NULL , NULL , NULL , NULL , NULL );
						if( service_ptr )
						{
							is_installed = 1;
							declare_stack_variable( char , image_path , 1024 );
							declare_stack_variable( char , registry_name , 1024 );
							rux_concatenation( registry_name , "SYSTEM\\CurrentControlSet\\Services\\" , rux::engine::_globals->_service_globals->_service_name );
							rux::registry::get_RegValueAsString( XEnum_PredefinedRegKey_HKEY_LOCAL_MACHINE , registry_name , "ImagePath" , image_path , error );
							if( image_path[ 0 ] == '"'
								&& image_path[ strlen( image_path ) - 1 ] == '"' )
							{								
								rux_concatenation( image_path , " -d --name " );
								rux_concatenation( image_path , rux::engine::_globals->_service_globals->_service_name );
							}
							else
							{
								declare_stack_variable( char , local_image_path , 1024 );
								rux_concatenation( local_image_path , "\"" );
								rux_concatenation( local_image_path , image_path );
								rux_concatenation( local_image_path , "\"" );
								rux_concatenation( local_image_path , " -d --name " );
								rux_concatenation( local_image_path , rux::engine::_globals->_service_globals->_service_name );
								memcpy( image_path , local_image_path , strlen( local_image_path ) + 1 );
							}
							rux::registry::set_RegValueAsExpandString( XEnum_PredefinedRegKey_HKEY_LOCAL_MACHINE , registry_name , "ImagePath" , image_path , error );
							if( rux::engine::_globals->_service_globals->_is_interactive == 1 )
							{
								DWORD service_type = rux::registry::get_RegValueAsDWORD( XEnum_PredefinedRegKey_HKEY_LOCAL_MACHINE , registry_name , "Type" , error );						
								rux::registry::set_RegValueAsDWORD( XEnum_PredefinedRegKey_HKEY_LOCAL_MACHINE , registry_name , "Type" , ( service_type | 256 ) , error );						
							}
							if( rux::engine::_globals->_service_globals->_is_autorecovery == 1 )
							{
								rux::registry::set_RegValueAsDWORD( XEnum_PredefinedRegKey_HKEY_LOCAL_MACHINE , registry_name , "ErrorControl" , 1 , error );						
								rux::uint8 reg_value[] =
								{
									60 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0
								};
								rux::registry::set_RegValueAsBinary( XEnum_PredefinedRegKey_HKEY_LOCAL_MACHINE , registry_name , "FailureActions" , reg_value , sizeof( reg_value ) , error );						
							}
							CloseServiceHandle( service_ptr ); 
						}			
						CloseServiceHandle( sc_manager_ptr );
					}
				}
#elif defined( __LINUX__ )		
				declare_stack_variable( char , executable_file , 1024 );
				rux_get_executable_filename( executable_file );	
				declare_stack_variable( char , init_script_filename , 1024 );
				declare_stack_variable( char , utf8_daemon , 1024 );
				declare_stack_variable( char , utf8_daemon_desc , 1024 );
				declare_stack_variable( char , utf8_user , 1024 );
				if( rux::engine::_globals->_service_globals->_user[ 0 ] != 0 )
					rux_concatenation( utf8_user , rux::engine::_globals->_service_globals->_user );
				else
					rux_concatenation( utf8_user , "root" );
				rux_concatenation( init_script_filename , "/etc/init.d/" , rux::engine::_globals->_service_globals->_service_name );
				declare_stack_variable( char , pid_file_dir , 1024 );
				rux_concatenation( pid_file_dir , "/var/run/" , rux::engine::_globals->_service_globals->_service_name );
				rux::io::directory::create( pid_file_dir );
				uid_t uid = 0;
				gid_t gid = 0;
				if( rux::security::get_user_id( utf8_user , uid , gid ) )
					chown( pid_file_dir , uid , gid );
				if( rux::engine::_globals->_service_globals->_service_name[ 0 ] != 0 && rux::engine::_globals->_service_globals->_service_name[ 0 ] == '"' )
					rux_concatenation( utf8_daemon , &rux::engine::_globals->_service_globals->_service_name[ 1 ] );
				else
					rux_concatenation( utf8_daemon , rux::engine::_globals->_service_globals->_service_name );
				if( utf8_daemon[ 0 ] != 0 && utf8_daemon[ strlen( utf8_daemon ) - 1 ] == '"' )
					utf8_daemon[ strlen( utf8_daemon ) - 1 ] = 0;
				if( rux::engine::_globals->_service_globals->_service_display_name[ 0 ] != 0 )
				{
					if( rux::engine::_globals->_service_globals->_service_display_name[ 0 ] != 0 && rux::engine::_globals->_service_globals->_service_display_name[ 0 ] == '"' )
						rux_concatenation( utf8_daemon_desc , &rux::engine::_globals->_service_globals->_service_display_name[ 1 ] );
					else
						rux_concatenation( utf8_daemon_desc , rux::engine::_globals->_service_globals->_service_display_name );
				}
				else
				{
					if( rux::engine::_globals->_service_globals->_service_name[ 0 ] != 0 && rux::engine::_globals->_service_globals->_service_name[ 0 ] == '"' )
						rux_concatenation( utf8_daemon_desc , &rux::engine::_globals->_service_globals->_service_name[ 1 ] );
					else
						rux_concatenation( utf8_daemon_desc , rux::engine::_globals->_service_globals->_service_name );
				}
				if( utf8_daemon_desc[ 0 ] != 0 && utf8_daemon_desc[ strlen( utf8_daemon_desc ) - 1 ] == '"' )
					utf8_daemon_desc[ strlen( utf8_daemon_desc ) - 1 ] = 0;
				declare_stack_variable( char , init_script_text , 8192 );
				::rux::safe_sprintf( init_script_text , 8192 , _init_d_daemon_script , utf8_daemon , utf8_daemon , utf8_daemon_desc , utf8_daemon_desc , utf8_daemon_desc , utf8_daemon , executable_file , rux::engine::_globals->_service_globals->_with_pid_file == 1 ? "--pidfile" : "" , rux::engine::_globals->_service_globals->_with_pid_file == 1 ? "/var/run/$NAME/$NAME.pid" : "" , utf8_daemon , utf8_user , rux::engine::_globals->_service_globals->_with_pid_file == 1 ? "true" : "false" , rux::engine::_globals->_service_globals->_is_autorecovery ? "true" : "false" );
				if( rux_is_exists_file_or_link( init_script_filename ) == 0 )
				{
					rux_clear_and_write_to_file( init_script_filename , init_script_text );					
					chmod( init_script_filename , S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH );
					pid_t pid = fork();
					if( pid < 0 )
					{
					}
					else if( pid != 0 )
					{
						rux::int32 status = 0;
						waitpid( pid , &status , 0 );
						if( WIFEXITED( status ) )
							status = WEXITSTATUS( status );
						else
							status = 1;
						if( status != 0 )
						{
							if( rux_is_exists_file_or_link( "/usr/sbin/update-rc.d" ) )
								rux_concatenation( error , "process 'update-rc.d' returned a failure" );
							else if( rux_is_exists_file_or_link( "/sbin/chkconfig" ) )
								rux_concatenation( error , "process 'chkconfig' returned a failure" );
							else
								rux_concatenation( error , "install returned a failure" );
						}
						else
							is_installed = 1;
					}
					else
					{
						if( rux_is_exists_file_or_link( "/usr/sbin/update-rc.d" ) )
							execlp( "/usr/sbin/update-rc.d" , "/usr/sbin/update-rc.d" , utf8_daemon , "defaults" , "99" , (char*)0 );
						else if( rux_is_exists_file_or_link( "/sbin/chkconfig" ) )
							execlp( "/sbin/chkconfig" , "/sbin/chkconfig" , "--add" , utf8_daemon , (char*)0 );
						_exit( 1 );
					}
				}
				else
				{
					rux_concatenation( error , "file or link '" );
					rux_concatenation( error , init_script_filename );
					rux_concatenation( error , "' already exists" );
				}
#endif
			}
			else
				rux_concatenation( error , "process is not executed as root" );
			if( error[ 0 ] != 0 )
			{
				declare_stack_variable( char , error0 , 1024 );
				size_t len = strlen( error );				
				memcpy( error0 , error , len + 1 );
				rux_concatenation( error , __FUNCTION__ , " , " );
				rux_concatenation( error , rux::engine::_globals->_service_globals->_service_name );
				rux_concatenation( error , " , " );
				rux_concatenation( error , error0 );
			}
			return is_installed;
		};
	};
};
