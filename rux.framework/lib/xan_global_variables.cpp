#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_global_variables.h"
#include "xan_engine_globals.h"
#ifdef __UNIX__
extern char** environ;
#endif
namespace rux
{
	namespace engine
	{
		class globals;
		dll_internal ::rux::engine::globals* _globals = NULL;
		dll_internal ::rux::rux_alloc_t _rux_alloc = 0;
		dll_internal ::rux::rux_free_t _rux_free = 0;
		dll_internal ::rux::get_time33_hash_value_t _get_time33_hash_value = NULL;
	};
};