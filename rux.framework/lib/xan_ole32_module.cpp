#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_ole32_module.h"
namespace rux
{
	namespace system
	{
#ifdef __WINDOWS__
		ole32_module::ole32_module( void )
			: rux::system::module( "ole32.dll" )
		{
		};
		void ole32_module::on_loaded( void )
		{
		};
#endif
	};
};