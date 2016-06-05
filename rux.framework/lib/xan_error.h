#ifndef XAN_ERROR_H
#define XAN_ERROR_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_console.h>
#include <xan_time.h>
#include <xan_int32.h>
#include <xan_log.h>
enum XEnum_ErrorType
{	
	XEnum_ErrorType_Errno ,
	XEnum_ErrorType_GetLastError
};
class XError
{
public:	
	static ::rux::String& ConvertErrorCodeToString( ::rux::int32 error_code , XEnum_ErrorType error_type );
	static void ConvertErrorCodeToString( ::rux::int32 error_code , XEnum_ErrorType error_type , char error[ 1024 ] );
};
#define rux_write_error_2( error , output , write_to_console ) {\
		output.set_ByRef( ::rux::XString::Format( "%s, %s, %s:%d, error '" , g_current_module_name , __FUNCTION__ , __FILE__ , __LINE__ ) );\
		output += error;\
		output += "'";\
		::rux::log::WriteError( output.str() );\
		output += "\n";\
		if( ::rux::utils::If< write_to_console != 0 >::test() )\
			::rux::XConsole::Print( stderr , XRed , output );\
	}
#define rux_write_error( error , output ) rux_write_error_2( error , output , 1 )
#define RUX_ERROR( output , error ) {\
		output.set_ByRef( ::rux::XString( "module \"" ) + g_current_module_name + "\"( file " + __FILE__ + ", line " + ::rux::XInt32( __LINE__ ).ToString() + ", function " + __FUNCTION__ + " )," + error );\
	}
#define errno_error {\
		::rux::XString error_string;\
		errno_error_to_string( error_string );\
	}
#ifndef EDQUOT
#define EDQUOT 122
#endif
#define errno_error_to_string( error_string ) errno_error_to_string_2( errno , error_string , 1 )
#define errno_error_to_string_2( err , error_string , write_to_console ) {\
		char err_str[ 1024 ] = {0};\
		strerror_r( err , err_str , 1024 );\
		if( err_str[ 0 ] == 0 )\
		{\
			if( err == EBADF )\
				strcpy( err_str , "EBADF" );\
			else if( err == ENOENT )\
				strcpy( err_str , "ENOENT" );\
			else if( err == ENOTDIR )\
				strcpy( err_str , "ENOTDIR" );\
			else if( err == ELOOP )\
				strcpy( err_str , "ELOOP" );\
			else if( err == EFAULT )\
				strcpy( err_str , "EFAULT" );\
			else if( err == EACCES )\
				strcpy( err_str , "EACCES" );\
			else if( err == ENOMEM )\
				strcpy( err_str , "ENOMEM" );\
			else if( err == ENAMETOOLONG )\
				strcpy( err_str , "ENAMETOOLONG" );\
			else if( err == EINTR )\
				strcpy( err_str , "EINTR" );\
			else if( err == EINVAL )\
				strcpy( err_str , "EINVAL" );\
			else if( err == EAGAIN )\
				strcpy( err_str , "EAGAIN" );\
			else if( err == EIO )\
				strcpy( err_str , "EIO" );\
			else if( err == EISDIR )\
				strcpy( err_str , "EISDIR" );\
			else if( err == EFBIG )\
				strcpy( err_str , "EFBIG" );\
			else if( err == EPIPE )\
				strcpy( err_str , "EPIPE" );\
			else if( err == ENOSPC )\
				strcpy( err_str , "ENOSPC" );\
			else if( err == ENOMEM )\
				strcpy( err_str , "ENOMEM" );\
			else if( err == EEXIST )\
				strcpy( err_str , "EEXIST" );\
			else if( err == ENXIO )\
				strcpy( err_str , "ENXIO" );\
			else if( err == ENODEV )\
				strcpy( err_str , "ENODEV" );\
			else if( err == EROFS )\
				strcpy( err_str , "EROFS" );\
			else if( err == ETXTBSY )\
				strcpy( err_str , "ETXTBSY" );\
			else if( err == EMFILE )\
				strcpy( err_str , "EMFILE" );\
			else if( err == ENFILE )\
				strcpy( err_str , "ENFILE" );\
			else if( err == EDQUOT )\
				strcpy( err_str , "EDQUOT" );\
		}\
		if( err_str[ 0 ] == 0 )\
			error_string.set_ByRef( ::rux::XString::Format( "%s, %s, %s:%d, error %d" , g_current_module_name , __FUNCTION__ , __FILE__ , __LINE__ , err ) );\
		else\
			error_string.set_ByRef( ::rux::XString::Format( "%s, %s, %s:%d, error '%s'" , g_current_module_name , __FUNCTION__ , __FILE__ , __LINE__ , err_str ) );\
		if( ::rux::utils::If< write_to_console != 0 >::test() )\
			::rux::XConsole::Print( stderr , XRed , error_string );\
		::rux::log::WriteError( error_string.str() );\
	}
#endif
