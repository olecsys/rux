#ifndef XAN_INCLUDES_H
#define XAN_INCLUDES_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_diagnostic_defines.h"
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#ifdef __WINDOWS__
#include <WinSock2.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <ObjBase.h>
#include <process.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <conio.h>
#include <locale>
#include <typeinfo>
#include <shlobj.h>
#include <direct.h>
#include <assert.h>
#include <netfw.h>
#include <sys/stat.h>
#include <Ws2ipdef.h>
#include <intrin.h>
#include <signal.h>
#include <dbghelp.h>
#include <limits.h>
#include <pdh.h>
#endif

#ifdef __UNIX__
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <locale>
#include <typeinfo>
#include <time.h>
#include <dlfcn.h>
#include <stdarg.h>
#ifdef __LINUX__
#include <sys/prctl.h>
#elif defined( __SOLARIS__ )
#include <rctl.h>
#endif
#ifdef __ANDROID__
#include <jni.h>
#include <android_native_app_glue.h>
#include <android/log.h>
#include <pwd.h>
#endif
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <sys/time.h>
#include <wchar.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#ifdef __LINUX__
#ifndef __ANDROID__
#include <execinfo.h>
#endif
#include <asm/types.h>
#include <math.h>
#endif
#ifdef __SOLARIS__
#include <atomic.h>
#endif
#include <signal.h>
#include <sys/mman.h>
#include <limits.h>
#include <setjmp.h>
#include <sys/wait.h>
#include <sys/resource.h>
#endif
#include <memory>
#ifdef __UNIX__
#include <tr1/memory>
#endif
#include <float.h>
#include <algorithm>
#include <map>
#include <vector>

#include <stddef.h>

#include <openssl/des.h>
#include <openssl/md4.h>
enum rux_start_process_method
{
	rux_start_process_method_shell_execute ,
	rux_start_process_method_create_process , 
	rux_start_process_method_create_process_as_user
};
extern dll_internal ::rux::int32 _rux_current_module_index;
namespace rux
{
	typedef void* (* rux_alloc_t)(size_t size,const char* type_name_ptr,const char* file_name_ptr,rux::int32 line , ::rux::int32 module_index , const char* variable_name );
	typedef void (* rux_free_t)(void* mem_ptr );
	namespace engine
	{
		dll_internal void free_mem( void* mem_ptr );
		dll_internal void* memcpy( void* dst , size_t dst_offset , const void* src, size_t size );
		dll_internal void* memmove( void* dst , size_t dst_offset , const void* src , size_t src_offset , size_t size );
		class globals;
		extern dll_internal ::rux::rux_alloc_t _rux_alloc;
		extern dll_internal ::rux::rux_free_t _rux_free;
		extern dll_internal ::rux::engine::globals* _globals;
		template< class T >
		dll_internal T* alloc_array( size_t size , const char* file = __FILE__ , ::rux::uint32 line = __LINE__ , char* variable_name = 0 )
		{
			return static_cast<T*>( ::rux::engine::_rux_alloc( sizeof( T ) * size , typeid( T ).name() , file , line , _rux_current_module_index , variable_name ) );
		};
	};
};
#define alloc_array_macros( object_class , size ) ::rux::engine::alloc_array< object_class >( size , (const char*)__FILE__ , (::rux::uint32)__LINE__ , (char*)0 )
#include "xan_malloc_array.h"
#endif
