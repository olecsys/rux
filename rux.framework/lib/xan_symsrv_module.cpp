#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_symsrv_module.h>
namespace rux
{
	namespace system
	{
#ifdef __WINDOWS__
		symsrv_module::symsrv_module( void )
			: rux::system::module( "symsrv.dll" , 0 , 1 )
		{
		};
		void symsrv_module::on_loaded( void )
		{
		};
#endif
	};
};