#ifndef XAN_CORE_H
#define XAN_CORE_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#ifdef RUX_USE_HEADER_PREFIX
#include "rux_extern_includes/xan_types.h"
#else
#include "xan_types.h"
#endif
namespace rux
{
#ifdef IAM_EXPORT
	//externc __declspec( dllexport ) ::rux::int64 core;
#else
	//externc __declspec( dllimport ) ::rux::int64 core;
#endif
};
#endif
