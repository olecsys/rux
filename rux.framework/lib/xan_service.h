#ifndef XAN_SERVICE_H
#define XAN_SERVICE_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_string.h>
#include <xan_keyvaluepair.h>

#ifndef BOOLDOG_NETWORK
#define BOOLDOG_NETWORK
#endif
#ifndef BOOLDOG_HEADER
#define BOOLDOG_HEADER( header ) <booldog/header>
#endif
#include BOOLDOG_HEADER(boo_string_utils.h)

namespace rux
{
	namespace service
	{
		namespace engine
		{
			class globals
			{
			public:
				globals( void );
				rux::uint8 _is_interactive;
				rux::uint8 _is_autorecovery;
				rux_service_task_t _service_task;
				rux_service_stop_t _service_stop;
				rux_service_stop_t _service_after_stop;
				rux::uint8 _with_pid_file;
				rux::byte _core_dump;
#if defined( __WINDOWS__ )
				SERVICE_STATUS _service_status;
				SERVICE_STATUS_HANDLE   _service_status_handle;
				HANDLE _service_stop_handle;
#else
				sigset_t _sigset;
				rux::int32 _signo;
				::rux::pid_t _sigpid;
#endif
				char _service_name[ 1024 ];
				char _service_display_name[ 1024 ];
				char _user[ 1024 ];
				char _user_group[ 1024 ];
			};
		};
		void initialize( rux_service_task_t service_task , rux_service_stop_t service_stop , rux_service_stop_t service_after_stop = NULL );
		void set_Autorecovery( rux::uint8 is_autorecovery );
		void set_Interactive( rux::uint8 is_interactive );
		void set_UserGroup( const char* user_group );
		void set_User( const char* user );
		void set_ServiceDisplayName( const char* service_display_name );
		void set_ServiceName( const char* service_name );
		void set_WithPidFile( rux::uint8 with_pid_file );
		void set_CoreDump(rux::uint8 core_dump);
		rux::uint8 Install( char error[ 1024 ] );
		rux::uint8 Stop( char error[ 1024 ] );
		rux::uint8 Stop( const char* service_name , char error[ 1024 ] );
		rux::uint8 DeInstall( char error[ 1024 ] );
		rux::uint8 DeInstall( const char* service_name , char error[ 1024 ] );
		rux::uint8 Start( const char* logfile , char error[ 1024 ] , rux::uint8 check_rux_executing_in_current_path = 0 );
		rux::uint8 StartByName( const char* service_name , char error[ 1024 ] );
		rux::uint8 CanStart( void );
		bool WaitForExit( void );
		void private_report_info_event( const char* app , const char* information );
		void private_report_error_event( const char* app , const char* information );
#if defined( __WINDOWS__ )
		void private_report_event( void );
		void WINAPI service_main( DWORD , char** );
		void private_report_status( uint32 current_state , uint32 exit_code , uint32 wait_hint );
		void WINAPI service_ctrl_handler( DWORD ctrl );
		void posix_signal( ::rux::int32 signum );
#endif
		void rename_stable_and_memory(::booldog::result_mbchar* mbchar0, ::booldog::result_mbchar* mbchar1);
	};
};
#endif
