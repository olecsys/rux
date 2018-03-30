#ifndef IV_DEFINES_H
#define IV_DEFINES_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "iv_typeOS.h"

#ifdef __cplusplus
#define ivexternc extern "C"
#else
#define ivexternc
#endif
#if defined(windows_os)
#define ivexport ivexternc __declspec(dllexport)
#define ivinternal
#define ivinline inline
#elif defined(linux_os)
#if __GNUC__ >= 4
#define ivexport ivexternc __attribute__ ((visibility("default")))
#define ivinternal  __attribute__ ((visibility("hidden")))
#else
#define ivexport ivexternc __declspec(dllexport)
#define ivinternal
#endif
#define ivinline inline
#elif defined(sun_os)
#define ivexport ivexternc __symbolic
#define ivinternal __hidden
#define ivinline inline
#endif

#ifndef booinline
#define booinline ivinline
#endif
#ifndef booexport
#define booexport ivexport
#endif
#ifndef boointernal
#define boointernal ivinternal
#endif

#ifdef windows_os	
typedef HMODULE ivmodule_handle;
#else
typedef void* ivmodule_handle;
#endif
#endif
