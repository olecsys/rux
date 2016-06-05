#ifndef XAN_SYSTEM_MODULE_H
#define XAN_SYSTEM_MODULE_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_includes.h>
#include <xan_crtsect.h>
#include <xan_interlocked.h>
namespace rux
{
#define system_module_dlsym( func ) {\
		_##func = (func##_t)dlsym( _handle , #func );\
		if( _##func == 0 )\
			printf( "library %s, function %s is not found\n" , _library , #func );\
	}
	namespace system
	{
		class module
		{
		protected:
			::rux::byte _need_lock;
			::rux::byte _begin_from_exe_dir;
			rux_volatile _loaded;
			char _library[ 128 ];
			char _alter_library[ 128 ];
#ifdef __WINDOWS__
			HMODULE _handle;
#elif defined( __UNIX__ )
			void* _handle;
#endif
			XCrtSect _cs;
		public:
			module( const char* library , const char* alter_library = 0 , ::rux::byte begin_from_exe_dir = 0 );
			virtual ~module( void );
			void load( void );
			void unload( void );
			virtual void on_loaded( void );
			bool loaded( void );
		};
	};
};
#endif
