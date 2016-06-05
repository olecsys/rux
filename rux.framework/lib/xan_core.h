#ifndef XAN_CORE_H
#define XAN_CORE_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_types.h>
namespace rux
{
#ifdef IAM_EXPORT
	//externc __declspec( dllexport ) ::rux::int64 core;
#else
	//externc __declspec( dllimport ) ::rux::int64 core;
#endif
};
#endif
