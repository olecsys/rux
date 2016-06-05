#ifndef XAN_ENVIRONMENT_H
#define XAN_ENVIRONMENT_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_string.h>
namespace rux
{	
	namespace diagnostics
	{	
		class XEnvironment
		{
		public:
			static rux::String& get_Variable( const rux::XString& variable_name );
			static void set_Variable( const rux::XString& variable_name , const rux::XString& variable_value , rux::XString& error );
		};
	};
};
#endif
