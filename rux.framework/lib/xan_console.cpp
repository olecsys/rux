#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_console.h>
#include <xan_debug.h>
#include <xan_file_descriptor_waiter.h>
#include <xan_memory_helper.h>
namespace rux
{	
	dll_internal rux_start_event_t _rux_console_start_event = NULL;
	dll_internal before_exit_event_t _g_rux_console_before_exit_event = NULL;
	dll_internal XConsole* _rux_console = NULL;
	#if defined(__APPLE__) || defined(__ANDROID__)
	ssize_t getdelim( char** linep , size_t* np , int delim , FILE* stream )
	{
		int ch;
		size_t new_len;
		ssize_t i = 0;
		char *line, *new_line;
		// Invalid input
		if (!linep || !np) 
		{
			errno = EINVAL;
			return -1;
		}
		if( *linep == NULL || *np == 0 )
		{
			*np = 1024;
			*linep = (char*)calloc( 1, *np );
			if (*linep == NULL)
				return -1;
		}
		line = *linep;
		while( ( ch = getc( stream ) ) != EOF )
		{
			if( i > *np )
			{
				// Need more space
				new_len  = *np + 1024;
				new_line = (char*)::realloc( *linep , new_len );
				if( !new_line )
					return -1;
				*np    = new_len;
				*linep = new_line;
			}
			line[ i ] = ch;
			if( ch == delim )
				break;
			i += 1;
		}
		if( i > *np )
		{
			// Need more space
			new_len = i + 2;
			new_line = (char*)::realloc( *linep , new_len );
			if( !new_line )
				return -1;
			*np = new_len;
			*linep = new_line;
		}
		line[ i + 1 ] = '\0';
		return i > 0 ? i : -1;
	};
	ssize_t getline( char** linep , size_t* np , FILE* stream )
	{
		return getdelim( linep , np , '\n' , stream );
	};
	#endif
	XConsole::XConsole()
	{
		_is_exit = 0;
		_rux_console = this;
	};
	void XConsole::Exit( void )
	{
		_is_exit = 1;
	};
	#ifdef __SOLARIS__
	void console_posix_ctrl_c_signal( ::rux::int32 signum )
	#else
	void XConsole::console_posix_ctrl_c_signal( ::rux::int32 signum )
	#endif
	{	
		_rux_console->_is_exit = 1;
		CS_PTR_LOCK( _g_cs_rux_console_before_exit_event );
		if( _g_rux_console_before_exit_event )
			_g_rux_console_before_exit_event();
		_g_rux_console_before_exit_event = NULL;
		_g_cs_rux_console_before_exit_event->UnLock();
		signal( signum , SIG_IGN );
	};
	void XConsole::WaitForEnter( void )
	{	
	#ifdef __WINDOWS__
		wchar_t line_ptr[ 1024 ] = L"";
		size_t line_size = 0;
		_cgetws_s( line_ptr , 256 , &line_size );
	#endif
	#ifdef __LINUX__
		char* line_ptr = NULL;
		size_t line_size = 0;
		getline( &line_ptr , &line_size , stdin );
		if( line_ptr )
			::free( line_ptr );
	#endif
	#ifdef __SOLARIS__
		declare_stack_variable( char , line_ptr , 256 );
		fgets( line_ptr , 256 , stdin );
	#endif
	};
	::rux::String& XConsole::Read( void )
	{	
		::rux::XString line;
	#ifdef __WINDOWS__
			wchar_t line_ptr[ 1024 ] = L"";
			size_t line_size = 0;
			::rux::uint8 tries = 0;
			do
			{
				if( _cgetws_s( line_ptr , 256 , &line_size ) == 0 )
				{
					if( line_size > 0 )
					{
						line = line_ptr;
						line.set_ByRef( line.ConvertToUTF8() );
					}
					else
						tries++;
				}
			}
			while( tries <= 1 && line.Length() == 0 );
	#endif
	#ifdef __LINUX__
			char* line_ptr = NULL;
			size_t line_size = 0;
			getline( &line_ptr , &line_size , stdin );
			if( line_ptr )
			{
				line = line_ptr;
				::free( line_ptr );
				line_ptr = NULL;
			}
	#endif
	#ifdef __SOLARIS__
			declare_stack_variable( char , line_ptr , 256 );
			if( fgets( line_ptr , 256 , stdin ) )
				line = line_ptr;
	#endif
		return line++;
	};
	void XConsole::Process( LPXPROCESSFN process , int argc , char ** args )
	{	
		::rux::uint8 res = 0;
		CS_PTR_LOCK( _g_cs_rux_console_before_exit_event );
		if( _rux_console_start_event )
			res = _rux_console_start_event( argc , args );
		_g_cs_rux_console_before_exit_event->UnLock();
		if( res == 0 )
		{
		#ifdef __WINDOWS__	
			signal( SIGTERM , console_posix_ctrl_c_signal );		
			signal( SIGINT , console_posix_ctrl_c_signal );
			signal( SIGBREAK , console_posix_ctrl_c_signal );
		#else		
			struct rlimit lim;
			lim.rlim_cur = 16384;
			lim.rlim_max = 16384;
			int setrlimit_res = setrlimit( RLIMIT_NOFILE , &lim );
			if(setrlimit_res == -1)
				printf("setrlimit, error(%d)", (int)errno);
			struct sigaction act;
			memset( &act , 0 , sizeof( act ) );
			act.sa_handler = console_posix_ctrl_c_signal;
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
			sigaction( SIGINT , &act , 0 );
			sigaction( SIGTERM , &act , 0 );
			sigaction( SIGQUIT , &act , 0 );
		#endif
		#ifdef __WINDOWS__
			wchar_t line[ 1024 ] = L"";
			declare_stack_variable( char , ansi_line , 1024 );
			size_t line_size = 0;	
			HANDLE handle = GetStdHandle( STD_INPUT_HANDLE );
			INPUT_RECORD input_record[ 1 ]; 
			DWORD input_number_of_events = 0;
			memset( &input_record , 0 , sizeof( INPUT_RECORD ) );
			while( _is_exit == 0 )
			{
				if( PeekConsoleInputA( handle , input_record , 1 , &input_number_of_events ) )
				{		
					if( input_number_of_events > 0 )
					{
						if( input_record[ 0 ].EventType == KEY_EVENT
							&& ( input_record[ 0 ].Event.KeyEvent.uChar.AsciiChar != 0
							|| input_record[ 0 ].Event.KeyEvent.uChar.UnicodeChar != 0 ) )
						{
							do
							{						
								if( _cgetws_s( line , 256 , &line_size ) == 0 )
								{
									if( line_size > 0 )
									{
										line[ 0 ] = input_record[ 0 ].Event.KeyEvent.uChar.AsciiChar;
										size_t result_bytes = 0;
										::rux::XString::utf16_to_ansi( (::rux::uint16*)line , wcslen( line ) , ansi_line , 1024 , result_bytes );
										if( process != NULL )
											process( ansi_line , this );
									}
								}
							}
							while( _is_exit == 0 && input_record[ 0 ].Event.KeyEvent.uChar.AsciiChar != 13 && line_size == 0 );
						}
					}
					FlushConsoleInputBuffer( handle );
					Sleep( 100 );
				}
			}
		#endif
#ifdef __LINUX__
			char* line = NULL;
			size_t line_size = 0;
			::rux::io::file_descriptor_waiter fd_waiter;
			fd_waiter.add( fileno( stdin ) );
			while( _is_exit == 0 )
			{
				if( fd_waiter.wait( 1000 , 1 , 0 ) )
				{
					if( fd_waiter.get_read_fd() != -1 )
					{
						getline( &line , &line_size , stdin );
						if( line )
						{
							if( process != NULL && _is_exit == 0 )
								process( line , this );
							::free( line );
							line = NULL;
						}
					}
				}
			}
		#endif
		#ifdef __SOLARIS__
			declare_stack_variable( char , line , 256 );
			::rux::io::file_descriptor_waiter fd_waiter;
			fd_waiter.add( fileno( stdin ) );
			while( _is_exit == 0 )
			{
				if( fd_waiter.wait( 1000 , 1 , 0 ) )
				{
					if( fd_waiter.get_read_fd() != -1 )
					{
						if( fgets( line , 256 , stdin ) )
						{
							if( process != NULL && _is_exit == 0 )
								process( line , this );
						}
					}
				}
			}
		#endif	
			CS_PTR_LOCK( _g_cs_rux_console_before_exit_event );
			if( _g_rux_console_before_exit_event )
				_g_rux_console_before_exit_event();
			_g_rux_console_before_exit_event = NULL;
			_g_cs_rux_console_before_exit_event->UnLock();
		}
	};
	void XConsole::DeInitialize( void )
	{
		CS_PTR_LOCK( _g_cs_rux_console_before_exit_event );
		_rux_console_start_event = 0;
		_g_rux_console_before_exit_event = 0;	
		_g_cs_rux_console_before_exit_event->UnLock();
	#ifdef __WINDOWS__	
		signal( SIGTERM , SIG_DFL );		
		signal( SIGINT , SIG_DFL );
		signal( SIGBREAK , SIG_DFL );
	#else
		struct rlimit lim;
		lim.rlim_cur = 16384;
		lim.rlim_max = 16384;
		int setrlimit_res = setrlimit( RLIMIT_NOFILE , &lim );
		if(setrlimit_res == -1)
			printf("setrlimit, error(%d)", (int)errno);
		struct sigaction act;
		memset( &act , 0 , sizeof( act ) );
		act.sa_handler = SIG_DFL;
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
		sigaction( SIGINT , &act , 0 );
		sigaction( SIGTERM , &act , 0 );
		sigaction( SIGQUIT , &act , 0 );
		signal( SIGPIPE , SIG_DFL );
	#endif		
	};
	void XConsole::Initialize( rux_start_event_t start_event , before_exit_event_t before_exit_event )
	{
		CS_PTR_LOCK( _g_cs_rux_console_before_exit_event );
		_rux_console_start_event = start_event;
		_g_rux_console_before_exit_event = before_exit_event;	
		_g_cs_rux_console_before_exit_event->UnLock();
	#ifdef __WINDOWS__	
		signal( SIGTERM , console_posix_ctrl_c_signal );		
		signal( SIGINT , console_posix_ctrl_c_signal );
		signal( SIGBREAK , console_posix_ctrl_c_signal );
	#else
		struct rlimit lim;
		lim.rlim_cur = 16384;
		lim.rlim_max = 16384;
		int setrlimit_res = setrlimit( RLIMIT_NOFILE , &lim );
		if(setrlimit_res == -1)
			printf("setrlimit, error(%d)", (int)errno);
		struct sigaction act;
		memset( &act , 0 , sizeof( act ) );
		act.sa_handler = console_posix_ctrl_c_signal;
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
		sigaction( SIGINT , &act , 0 );
		sigaction( SIGTERM , &act , 0 );
		sigaction( SIGQUIT , &act , 0 );
		signal( SIGPIPE , SIG_IGN );
	#endif		
	};
	void XConsole::Print( FILE* file_ptr , XColors color , const char* format )
	{
		XConsole::Print( file_ptr , color , "%s" , format );
	};
	void XConsole::Print( FILE* file_ptr , XColors color , const char* format , ... )
	{
		va_list ap;
		va_start( ap , format );
		::rux::XString str( ::rux::XString::FormatVAList( format , ap ) );
		va_end( ap );
	#ifdef __WINDOWS__
		HANDLE console_ptr = INVALID_HANDLE_VALUE;
		if( file_ptr == stderr )
			console_ptr = GetStdHandle( STD_ERROR_HANDLE );
		else if( file_ptr == stdout )
			console_ptr = GetStdHandle( STD_OUTPUT_HANDLE );
		else if( file_ptr == stdin )
			console_ptr = GetStdHandle( STD_INPUT_HANDLE );
		if( console_ptr != INVALID_HANDLE_VALUE )
		{
			if( color == XRed )
				SetConsoleTextAttribute( console_ptr , FOREGROUND_RED | FOREGROUND_INTENSITY );		
			else if( color == XGreen )
				SetConsoleTextAttribute( console_ptr , FOREGROUND_GREEN | FOREGROUND_INTENSITY );
			else if( color == XWhite )
				SetConsoleTextAttribute( console_ptr , FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY );					
		}
		fprintf( file_ptr , "%s" , str.str() );		
		if( console_ptr != INVALID_HANDLE_VALUE )
		{
			SetConsoleTextAttribute( console_ptr , FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY );		
		}
		OutputDebugStringA(  str.str() );
	#endif
	#ifdef __UNIX__
	#ifdef __LINUX__
		if( color == XRed )
				fprintf( file_ptr , "\e[31m" );
			else if( color == XGreen )
				fprintf( file_ptr , "\e[32m" );
			else if( color == XWhite )
				fprintf( file_ptr , "\e[37m" );
	#endif
		fprintf( file_ptr , "%s" , str.str() );
	#ifdef __LINUX__
		fprintf( file_ptr , "\e[37m" );
	#endif	
	#endif
	};
	void XConsole::Print( FILE* file_ptr , XColors color , const ::rux::XString& string )
	{
		char* string_ptr = NULL;
		::rux::XString utf8;
	#ifdef __WINDOWS__
		utf8.set_ByRef( string.ConvertToAnsi() );
		string_ptr = alloc_array_macros( char , utf8.Length() + 1 );
		if( CharToOemA( utf8.str() , string_ptr ) == FALSE )
			string_ptr[ 0 ] = 0;
	#else	
		utf8.set_ByRef( string.ConvertToUTF8() );
		string_ptr = utf8.str();
	#endif
	#ifdef __WINDOWS__
		HANDLE console_ptr = INVALID_HANDLE_VALUE;
		if( file_ptr == stderr )
			console_ptr = GetStdHandle( STD_ERROR_HANDLE );
		else if( file_ptr == stdout )
			console_ptr = GetStdHandle( STD_OUTPUT_HANDLE );
		else if( file_ptr == stdin )
			console_ptr = GetStdHandle( STD_INPUT_HANDLE );
		if( console_ptr != INVALID_HANDLE_VALUE )
		{
			if( color == XRed )
				SetConsoleTextAttribute( console_ptr , FOREGROUND_RED | FOREGROUND_INTENSITY );		
			else if( color == XGreen )
				SetConsoleTextAttribute( console_ptr , FOREGROUND_GREEN | FOREGROUND_INTENSITY );
			else if( color == XWhite )
				SetConsoleTextAttribute( console_ptr , FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY );					
		}
		fprintf( file_ptr , "%s" , string_ptr );		
		if( console_ptr != INVALID_HANDLE_VALUE )
			SetConsoleTextAttribute( console_ptr , FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY );
		OutputDebugStringA(  utf8.str() );
		if( string_ptr )
			::rux::engine::free_mem( string_ptr );
	#endif
	#ifdef __UNIX__
	#ifdef __LINUX__
		if( color == XRed )
			fprintf( file_ptr , "\e[31m" );
		else if( color == XGreen )
			fprintf( file_ptr , "\e[32m" );
		else if( color == XWhite )
			fprintf( file_ptr , "\e[37m" );
	#endif
		fprintf( file_ptr , "%s" , string_ptr );
	#ifdef __LINUX__
		fprintf( file_ptr , "\e[37m" );
	#endif	
	#endif
	};
	void XConsole::WriteToStdIn( const char* text_ptr )
	{
		rux_write( 0 , text_ptr , strlen( text_ptr ) );
	#ifdef __UNIX__
		fsync( STDIN_FILENO );
	#elif defined( __WINDOWS__ )
		fflush( stdout );
	#endif
	};
	void XConsole::WriteToStdOut( const char* text_ptr )
	{
	#ifdef __UNIX__	
	#ifdef __LINUX__
		rux_write( 1 , "\e[32m" , 5 );
	#endif
	#elif defined( __WINDOWS__ )
		HANDLE console_ptr = GetStdHandle( STD_OUTPUT_HANDLE );
		if( console_ptr != INVALID_HANDLE_VALUE )
				SetConsoleTextAttribute( console_ptr , FOREGROUND_GREEN | FOREGROUND_INTENSITY );	
	#endif
		rux_write( 1 , text_ptr , strlen( text_ptr ) );
	#ifdef __UNIX__
		fsync( STDOUT_FILENO );
	#ifdef __LINUX__
		rux_write( 2 , "\e[37m" , 5 );
	#endif
	#elif defined( __WINDOWS__ )
		fflush( stdout );
		if( console_ptr != INVALID_HANDLE_VALUE )
			SetConsoleTextAttribute( console_ptr , FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY );		
	#endif
	};
	void XConsole::WriteToStdErr( const char* text_ptr )
	{
	#ifdef __UNIX__	
	#ifdef __LINUX__
		rux_write( 2 , "\e[31m" , 5 );
	#endif
	#elif defined( __WINDOWS__ )
		HANDLE console_ptr = GetStdHandle( STD_ERROR_HANDLE );
		if( console_ptr != INVALID_HANDLE_VALUE )
				SetConsoleTextAttribute( console_ptr , FOREGROUND_RED | FOREGROUND_INTENSITY );	
	#endif
		rux_write( 2 , text_ptr , strlen( text_ptr ) );
	#ifdef __UNIX__
		fsync( STDERR_FILENO );
	#ifdef __LINUX__
		rux_write( 2 , "\e[37m" , 5 );
	#endif
	#elif defined( __WINDOWS__ )	
		fflush( stdout );
		if( console_ptr != INVALID_HANDLE_VALUE )
			SetConsoleTextAttribute( console_ptr , FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY );		
	#endif
	};
	void XConsole::ShowWindow( void )
	{
	#ifdef __WINDOWS__
		HWND hwnd = GetConsoleWindow();
		if( hwnd )
			::ShowWindow( hwnd , SW_SHOW );
	#endif
	};
	void XConsole::HideWindow( void )
	{
	#ifdef __WINDOWS__
		HWND hwnd = GetConsoleWindow();
		if( hwnd )
			::ShowWindow( hwnd , SW_HIDE );
	#endif
	};
	::rux::uint8 XConsole::get_IsExit( void )
	{
		return _is_exit;
	};
};