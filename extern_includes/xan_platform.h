#ifndef XAN_PLATFORM_H
#define XAN_PLATFORM_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#if defined(_WIN64) || defined(WIN64)
	#define __WINDOWS__
	#define __x64__
#elif defined(WIN32) || defined(_WIN32)
	#define __WINDOWS__
	#define __x86__
#elif defined(__GNUC__)
	#ifdef __linux__
		#if defined( __ANDROID__ ) || defined( ANDROID )
			#ifndef __ANDROID__
				#define __ANDROID__
			#endif
		#endif
		#if defined( __arm__ )
			#ifndef __ARM__
				#define __ARM__
			#endif
		#endif
		#define __LINUX__
		#define __USE_EPOLL__
		#ifdef __ARM__
			#define __x86__
		#endif
		#ifdef __aarch64__
			#define __x64__
			#define RUX_NO_ASM
		#else
			#ifdef __ARM__
				#define __x86__
			#endif
		#endif
		#ifdef __i386__
			#define __x86__
		#endif
		#ifdef __i486__
			#define __x86__
		#endif
		#ifdef __i586__
			#define __x86__
		#endif
		#ifdef __i686__	
			#define __x86__
		#endif
		#ifdef __IA64__
			#define __x64__
		#endif
		#ifdef __ia64__
			#define __x64__
		#endif
		#ifdef _IA64
			#define __x64__
		#endif
		#ifdef __amd64__
			#define __x64__
		#endif
		#ifdef __amd64
			#define __x64__
		#endif
		#ifdef __x86_64__
			#define __x64__
		#endif
		#ifdef __x86_64
			#define __x64__
		#endif
		#ifdef __e2k__
			#define __x64__
			#define __ELBRUS__
		#endif
		#define __UNIX__
	#endif
#elif defined( __SUNPRO_CC_COMPAT ) || defined( __sun ) || defined( __sun__ )
	#define __x64__
	#define __SOLARIS__
	#define __UNIX__
#else
#error "Cannot find OS and architecture"
#endif
#endif
