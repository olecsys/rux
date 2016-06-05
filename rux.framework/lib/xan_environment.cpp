#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_environment.h>
#include <xan_error.h>
#include <xan_int32.h>
#include <xan_keyvaluepair.h>
namespace rux
{	
	namespace diagnostics
	{	
		rux::String& XEnvironment::get_Variable( const rux::XString& variable_name )
		{
			rux::XString variable_value;
			rux::XString current_cp;
			current_cp.set_ByRef( variable_name.ConvertToAnsi() );
			variable_value = getenv( current_cp.str() );
			return variable_value++;
		};
		void XEnvironment::set_Variable( const rux::XString& variable_name , const rux::XString& variable_value , rux::XString& error )
		{
			if( variable_name.Length() > 0 )
			{
				rux::XString current_cp;
				current_cp.set_ByRef( variable_value.ConvertToAnsi() + "=" + variable_value.ConvertToAnsi() );
				int32 result = rux_putenv( current_cp.str() );
				if( result != 0 )
				{
					rux_write_error( "putenv return " + XInt32( result ).ToString() , error );
				}
			}
			else
			{
				rux_write_error( "variable name is empty" , error );
			}
		};
	};
};