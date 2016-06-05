#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_error.h>
rux::String& XError::ConvertErrorCodeToString( ::rux::int32 error_code , XEnum_ErrorType error_type )
{	
	::rux::XString error;		
	if( error_type == XEnum_ErrorType_Errno )
	{
		if( error_code == 0 )
			error.Clear();
		else
		{
			char err_str[ 1024 ] = {0};
			strerror_r( error_code , err_str , 1024 );
			error = err_str;
			if( error.Length() == 0 )
				error = "Unknown error";
		}
	}
	else if( error_type == XEnum_ErrorType_GetLastError )
	{
#ifdef __WINDOWS__
		::rux::uint32 last_error = error_code == 0 ? GetLastError() : error_code;
		if( last_error != 0 )
		{
			void* last_error_void = NULL;
			FormatMessageA( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS , NULL , last_error , MAKELANGID( LANG_NEUTRAL , SUBLANG_DEFAULT ) , (char*)&last_error_void , 0 , NULL );
			if( last_error_void )
			{
				error = (char*)last_error_void;
				LocalFree( last_error_void );
			}
			if( error.Length() == 0 )
				error = "Unknown error";
		}
#endif
	}	
	return error++;
};
void XError::ConvertErrorCodeToString( ::rux::int32 error_code , XEnum_ErrorType error_type , char error[ 1024 ] )
{	
	if( error )
	{
		error[ 0 ] = 0;		
		if( error_type == XEnum_ErrorType_Errno )
		{
			if( error_code == 0 )
				error[ 0 ] = 0;
			else
			{
				strerror_r( error_code , error , 1024 );
				if( error[ 0 ] == 0 )
					rux_concatenation( error , "Unknown error" );
			}
		}
		else if( error_type == XEnum_ErrorType_GetLastError )
		{
	#ifdef __WINDOWS__
			::rux::uint32 last_error = error_code == 0 ? GetLastError() : error_code;
			if( last_error != 0 )
			{
				void* last_error_void = NULL;
				FormatMessageA( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS , NULL , last_error , MAKELANGID( LANG_NEUTRAL , SUBLANG_DEFAULT ) , (char*)&last_error_void , 0 , NULL );
				if( last_error_void )
				{
					::rux::engine::memcpy( error , 0 , (char*)last_error_void , strlen( (char*)last_error_void ) + 1 );
					LocalFree( last_error_void );
				}
				if( error[ 0 ] == 0 )
					rux_concatenation( error , "Unknown error" );
			}
	#endif
		}
	}
};