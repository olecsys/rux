#include <xan_console.h>
#include <xan_service.h>
#include <xan_security.h>
rux::int32 start( rux::XArray< rux::XString >& names , rux::XArray< rux::XString >& values )
{	
	if( names.Count() >= 1 && values.Count() >= 1 )
	{
		declare_stack_variable( char , error , 1024 );
		if( names[ 0 ] == "--start-service" )
			rux::service::StartByName( values[ 0 ]._array_ptr , error );
		else if( names[ 0 ] == "--stop-service" )
			rux::service::Stop( values[ 0 ]._array_ptr , error );
		else if( names[ 0 ] == "--deinstall-service" )
			rux::service::DeInstall( values[ 0 ]._array_ptr , error );
		if( error[ 0 ] != 0 )
		{
			rux::XConsole::Print( stderr , XRed , "error : " );
			rux::XConsole::Print( stderr , XRed , rux::XString( error ) );			
		}
		else
			rux::XConsole::Print( stdout , XGreen , "successful" );
		rux::XConsole::Print( stdout , XWhite , "\n\nPress <ENTER>" );
		rux::XConsole console;
		console.Read();
	}
	return 0;
};
rux_simple_console_initialize( "rux.service.test.exe" , start );
