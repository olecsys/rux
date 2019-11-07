#ifndef XAN_OLEAUT32_MODULE_H
#define XAN_OLEAUT32_MODULE_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_system_module.h"
namespace rux
{
	namespace system
	{
#ifdef __WINDOWS__
		class oleaut32_module : public rux::system::module
		{
		protected:
		public:
			oleaut32_module( void );
			virtual void on_loaded( void );
		};
#endif
	};
};
#endif
