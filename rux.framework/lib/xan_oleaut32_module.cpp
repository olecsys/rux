#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_oleaut32_module.h>
namespace rux
{
	namespace system
	{
#ifdef __WINDOWS__
		oleaut32_module::oleaut32_module( void )
			: rux::system::module( "oleaut32.dll" )
		{
		};
		void oleaut32_module::on_loaded( void )
		{
		};
#endif
	};
};