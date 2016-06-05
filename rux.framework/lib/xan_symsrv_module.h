#ifndef XAN_SYMSRV_MODULE_H
#define XAN_SYMSRV_MODULE_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_system_module.h>
namespace rux
{
	namespace system
	{
#ifdef __WINDOWS__
		class symsrv_module : public rux::system::module
		{
		public:
		public:
			symsrv_module( void );
			virtual void on_loaded( void );
		};
#endif
	};
};
#endif
