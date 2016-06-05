#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_user32_module.h>
namespace rux
{
	namespace system
	{
#ifdef __WINDOWS__
		user32_module::user32_module( void )
			: rux::system::module( "User32.dll" )
		{
		};
		void user32_module::on_loaded( void )
		{
		};
#endif
	};
};