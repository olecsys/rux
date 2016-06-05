#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_stdredirect.h>
#include <xan_error.h>
#include <xan_keyvaluepair.h>
#include <xan_uint64.h>
#include <xan_file_descriptor_waiter.h>
#include <xan_boolean.h>
begin_implement_rux_class_ns( StdRedirect , rux );
	_buffer_size = 16384;
	_on_changed = NULL;
end_implement_rux_class();
namespace rux
{	
	StdRedirect::~StdRedirect()
	{	
		CRITICAL_SECTION_LOCK( _cs_redirect );
		if( _redirect.get_IsStarted() == 1 )
			_redirect.Stop();
		CRITICAL_SECTION_UNLOCK( _cs_redirect );
	};
	void XStdRedirect::Start( void )
	{
		CRITICAL_SECTION_LOCK( (*this)()->_cs_redirect );	
		if( (*this)()->_redirect.get_IsStarted() == 0 )
		{
			(*this)()->_redirect.set_ThreadName( "std redirect" );
			(*this)()->_redirect.set_ThreadMode( XThreadMode_Repeat );
			(*this)()->_redirect.set_Timeout( 100 );
			(*this)()->_redirect.Start();
			(*this)()->_redirect.Push( redirect_thread , (*this)() );
		}
		CRITICAL_SECTION_UNLOCK( (*this)()->_cs_redirect );	
	};
	void XStdRedirect::set_OnChanged( rux_simple_t on_changed )
	{
		CRITICAL_SECTION_LOCK( (*this)()->_cs_buffer );
		(*this)()->_on_changed = on_changed;
		CRITICAL_SECTION_UNLOCK( (*this)()->_cs_buffer );
	};
	void XStdRedirect::Stop( void )
	{
		CRITICAL_SECTION_LOCK( (*this)()->_cs_redirect );
		if( (*this)()->_redirect.get_IsStarted() == 1 )
			(*this)()->_redirect.Stop();
		CRITICAL_SECTION_UNLOCK( (*this)()->_cs_redirect );	
	};
	void XStdRedirect::redirect_thread( void* param , size_t ___rux__thread_index1986 )
	{
		UNREFERENCED_PARAMETER( ___rux__thread_index1986 );
		StdRedirect* std = (StdRedirect*)param;
		::rux::threading::XThread current_thread( ::rux::threading::XThread::get_CurrentThread() );	
		char* buffer = alloc_array_macros( char , std->_buffer_size );
	#ifdef __WINDOWS__	
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
		HANDLE read_stdout_pipe = NULL;
		HANDLE write_stdout_pipe = NULL;
		::rux::XString error;
		if( CreatePipe( &read_stdout_pipe , &write_stdout_pipe , &sa , 0 ) )
		{
			HANDLE read_stderr_pipe = NULL;
			HANDLE write_stderr_pipe = NULL;
			if( CreatePipe( &read_stderr_pipe , &write_stderr_pipe , &sa , 0 ) )
			{		
				setvbuf( stderr , NULL , _IONBF , 0 );
				setvbuf( stdout , NULL , _IONBF , 0 );

				::rux::int32 old_stdout = _dup( 1 );
				::rux::int32 old_stderr = _dup( 2 );
				HANDLE old_stdout_handle = GetStdHandle( STD_OUTPUT_HANDLE );
				HANDLE old_stderr_handle = GetStdHandle( STD_ERROR_HANDLE );
				fflush( stdout );
				fflush( stderr );
				SetStdHandle( STD_OUTPUT_HANDLE , write_stdout_pipe );
				::rux::int32 new_stdout = _open_osfhandle( (intptr_t)write_stdout_pipe , O_WRONLY | O_TEXT );		
				_dup2( new_stdout , 1 );

				SetStdHandle( STD_ERROR_HANDLE , write_stderr_pipe );		
				::rux::int32 new_stderr = _open_osfhandle( (intptr_t)write_stderr_pipe , O_WRONLY | O_TEXT );		
				_dup2( new_stderr , 2 );

				std::ios::sync_with_stdio();
				DWORD bytes_total = 0 , bytes_read = 0;
				while( current_thread.get_IsWaitForExit() == 0 )
				{
					::rux::uint8 readed = 0;
					bytes_total = 0;
					if( PeekNamedPipe( read_stdout_pipe , NULL , 0 , NULL , &bytes_total , NULL ) && bytes_total > 0 )
					{	
						while( bytes_total > 0 )								
						{
							bytes_read = 0;
							ReadFile( read_stdout_pipe , buffer , std->_buffer_size - 1 , &bytes_read , NULL );
							if( bytes_read > 0 )
							{
								readed = 1;
								buffer[ bytes_read ] = '\0';
								CRITICAL_SECTION_LOCK( std->_cs_buffer );								
								if( std->_buffer.Length() > 65536 )
									std->_buffer.RemoveRange( 0 , std->_buffer.Length() - 65536 );							
								std->_buffer += buffer;
								CRITICAL_SECTION_UNLOCK( std->_cs_buffer );
								if( bytes_total >= bytes_read )
									bytes_total -= bytes_read;
								else
									bytes_total = 0;
							}
						}						
					}
					bytes_total = 0;
					if( PeekNamedPipe( read_stderr_pipe , NULL , 0 , NULL , &bytes_total , NULL ) && bytes_total > 0 )
					{	
						while( bytes_total > 0 )								
						{
							bytes_read = 0;
							ReadFile( read_stderr_pipe , buffer , std->_buffer_size - 1 , &bytes_read , NULL );
							if( bytes_read > 0 )
							{
								readed = 1;
								buffer[ bytes_read ] = '\0';
								CRITICAL_SECTION_LOCK( std->_cs_buffer );								
								if( std->_buffer.Length() > 65536 )
									std->_buffer.RemoveRange( 0 , std->_buffer.Length() - 65536 );							
								std->_buffer += buffer;
								CRITICAL_SECTION_UNLOCK( std->_cs_buffer );
								if( bytes_total >= bytes_read )
									bytes_total -= bytes_read;
								else
									bytes_total = 0;
							}
						}						
					}
					if( readed )
					{
						CRITICAL_SECTION_LOCK( std->_cs_buffer );
						if( std->_on_changed )
							std->_on_changed();
						CRITICAL_SECTION_UNLOCK( std->_cs_buffer );
					}
					else if( current_thread.get_IsWaitForExit() == 0 )
						::rux::threading::XThread::Sleep( 10 );
				}
				_dup2( old_stdout , 1 );
				SetStdHandle( STD_OUTPUT_HANDLE , old_stdout_handle );
				_dup2( old_stderr , 2 );
				SetStdHandle( STD_ERROR_HANDLE , old_stderr_handle );

				fflush( stdout );
				fflush( stderr );
			
				CloseHandle( write_stderr_pipe );
				CloseHandle( read_stderr_pipe );
			}
			CloseHandle( write_stdout_pipe );
			CloseHandle( read_stdout_pipe );
		} 
		else
		{
			rux_write_error( XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError ) , error );
		}
	#else
		::rux::int32 pipes[ 2 ] = {0};
		::rux::int32 old_stdout = dup( 1 );
		::rux::int32 old_stderr = dup( 2 );	
		if( pipe( pipes ) == 0 )
		{
			dup2( pipes[ 1 ] , 1 );
			dup2( pipes[ 1 ] , 2 );
			::rux::io::file_descriptor_waiter fd_waiter;
			fd_waiter.add( pipes[ 0 ] );
			::rux::int32 readen_size = 0;
			while( current_thread.get_IsWaitForExit() == 0 )
			{
				if( fd_waiter.wait( 1000 , 1 , 0 ) )
				{				
					if( fd_waiter.get_read_fd() != -1 )
					{
						readen_size = read( pipes[ 0 ] , buffer , std->_buffer_size - 1 );
						if( readen_size > 0 )
						{
							if( std->_buffer.Length() > 65536 )
								std->_buffer.RemoveRange( 0 , std->_buffer.Length() - 65536 );						
							buffer[ readen_size] = '\0';
							CRITICAL_SECTION_LOCK( std->_cs_buffer );
							std->_buffer += buffer;							
							if( std->_on_changed )
								std->_on_changed();
							CRITICAL_SECTION_UNLOCK( std->_cs_buffer );
						}
					}
				}
				else
					break;
			}
			dup2( old_stdout , 1 );
			dup2( old_stderr , 2 );
			close( pipes[ 0 ] );
			close( pipes[ 1 ] );
		}
	#endif
		::rux::engine::free_mem( buffer );
	};
	::rux::String& XStdRedirect::get_Buffer( void )
	{
		::rux::XString buffer;
		CRITICAL_SECTION_LOCK( (*this)()->_cs_buffer );
		buffer = (*this)()->_buffer;
		CRITICAL_SECTION_UNLOCK( (*this)()->_cs_buffer );
		return buffer++;
	};
};