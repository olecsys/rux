#ifndef XAN_TYPES_H
#define XAN_TYPES_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_platform.h>
#ifdef __WINDOWS__
#define dll_internal
#define rux_inline __forceinline
#elif defined( __UNIX__ )
#ifdef __LINUX__
#if __GNUC__ >= 4
#define dll_internal  __attribute__ ((visibility ("hidden")))
#else
#define dll_internal
#endif
#elif defined( __SOLARIS__ )
#define dll_internal __hidden
#endif
#define rux_inline inline
#endif
#ifdef __WINDOWS__
#define RTLD_LAZY 0
#define memalign( alignment , size ) _aligned_malloc( size , alignment )
#define dlsym( handle , name ) GetProcAddress( handle , name )
#define dlopen( name , flag ) LoadLibraryA( name )
#define dlclose( handle ) FreeLibrary( handle )
#elif defined( __UNIX__ )
#define _aligned_malloc( size , alignment ) memalign( alignment , size )
//#define GetProcAddress( handle , name ) dlsym( handle , name )
//#define LoadLibraryA( name ) (void*)dlopen( name , RTLD_LAZY )
#endif
namespace rux
{	
#ifdef __WINDOWS__
	typedef signed char int8;
	typedef unsigned char uint8;
	typedef short int16;
	typedef unsigned short uint16;
	typedef __int64 int64;
	typedef unsigned __int64 uint64;
	typedef int int32;
	typedef unsigned int uint32;
	typedef unsigned char byte;
#else
	typedef signed char int8;
	typedef unsigned char uint8;
	typedef short int16;
	typedef unsigned short uint16;
	typedef long long int int64;
	typedef unsigned long long int uint64;
	typedef int int32;
	typedef unsigned int uint32;
	typedef unsigned char byte;
#endif
	typedef uint32 pid_t;
};
#ifdef __SOLARIS__
typedef volatile ::rux::uint32 rux_volatile;
typedef ::rux::uint32 rux_interlocked_return;
#define rux_volatile_max 0xffffffff
#else
typedef volatile long rux_volatile;
typedef long rux_interlocked_return;
#ifdef __SIZEOF_LONG__
	#if __SIZEOF_LONG__ == 8
		#define rux_volatile_max 0xffffffffffffffff
	#else
		#define rux_volatile_max 0xffffffff
	#endif
#else
	#define rux_volatile_max 0xffffffff
#endif
#endif
#ifdef __WINDOWS__
#define begin_struct_pack( bytes ) __pragma( pack( push ) )\
	__pragma( pack( bytes ) )\
	__declspec( align( bytes ) )
#define end_struct_pack( bytes ) ;\
	__pragma( pack( pop ) )
#else
#define begin_struct_pack( bytes )
#define end_struct_pack( bytes ) __attribute__( ( packed , aligned( 1 ) ) );
#endif
#define RUX_STACK_AND_GLOBAL_DIAGNOSTIC 1
#if RUX_STACK_AND_GLOBAL_DIAGNOSTIC
#define declare_stack_variable( type , var , size ) class __1986_memory_##var\
	{\
	public:\
		::rux::memory::begin_memory_chunk _begin_memory_chunk;\
		type _ptr[ size ];\
		::rux::memory::end_memory_chunk _end_memory_chunk;\
		__1986_memory_##var()\
		{\
			if( _rux_is_memory_manager == 0 )\
				::rux::engine::load_engine();\
			memset( _ptr , 0 , size * sizeof( type ) );\
			_begin_memory_chunk._marker = begin_memory_marker;\
			_begin_memory_chunk._deleted = 0;\
			_begin_memory_chunk._prev = NULL;\
			_begin_memory_chunk._next = NULL;\
			_begin_memory_chunk._module_index = _rux_current_module_index;\
			_begin_memory_chunk._extended_info = NULL;\
			_begin_memory_chunk._stack = 1;\
			_begin_memory_chunk._size = cpp_offsetof( __1986_memory_##var , _end_memory_chunk ) + sizeof( ::rux::memory::end_memory_chunk );\
			_end_memory_chunk._marker = end_memory_marker;\
			_begin_memory_chunk._diagnostics = ::rux::engine::add_stack_var( &_begin_memory_chunk , __FILE__ , __LINE__ , (char*)#var , #type );\
		};\
		~__1986_memory_##var()\
		{\
			if( _begin_memory_chunk._diagnostics )\
				::rux::engine::remove_stack_var( &_begin_memory_chunk );\
		};\
	} __1986_memory_##var##_obj;\
	type ( &var )[ size ] = __1986_memory_##var##_obj._ptr;
#define _declare_stackvar( type , var , need_add_stack_var ) class __1986_memory_##var\
	{\
	public:\
		::rux::memory::begin_memory_chunk _begin_memory_chunk;\
		type _ptr;\
		::rux::memory::end_memory_chunk _end_memory_chunk;\
		__1986_memory_##var()\
		{\
			if( ::rux::utils::If< need_add_stack_var != 0 >::test() && _rux_is_memory_manager == 0 )\
				::rux::engine::load_engine();\
			_begin_memory_chunk._marker = begin_memory_marker;\
			_begin_memory_chunk._deleted = 0;\
			_begin_memory_chunk._prev = NULL;\
			_begin_memory_chunk._next = NULL;\
			_begin_memory_chunk._module_index = _rux_current_module_index;\
			_begin_memory_chunk._extended_info = NULL;\
			_begin_memory_chunk._stack = 1;\
			_begin_memory_chunk._size = cpp_offsetof( __1986_memory_##var , _end_memory_chunk ) + sizeof( ::rux::memory::end_memory_chunk );\
			_end_memory_chunk._marker = end_memory_marker;\
			if( ::rux::utils::If< need_add_stack_var != 0 >::test() )\
				_begin_memory_chunk._diagnostics = ::rux::engine::add_stack_var( &_begin_memory_chunk , __FILE__ , __LINE__ , (char*)#var , #type );\
			else\
				_begin_memory_chunk._diagnostics = 0;\
		};\
		~__1986_memory_##var()\
		{\
			if( _begin_memory_chunk._diagnostics )\
				::rux::engine::remove_stack_var( &_begin_memory_chunk );\
		};\
	} __1986_memory_##var##_obj;\
	type& var = __1986_memory_##var##_obj._ptr;
#define declare_global_variable( type , var , size ) class __glob__1986_memory_##var##_struct\
			{\
			public:\
				::rux::memory::begin_memory_chunk _begin_memory_chunk;\
				type _ptr[ size ];\
				::rux::memory::end_memory_chunk _end_memory_chunk;\
				__glob__1986_memory_##var##_struct();\
				~__glob__1986_memory_##var##_struct();\
			};\
			extern dll_internal __glob__1986_memory_##var##_struct __glob__1986_memory_##var##_obj;\
			extern dll_internal type ( &var )[ size ]
#define implement_global_variable( type , var , size )	dll_internal __glob__1986_memory_##var##_struct __glob__1986_memory_##var##_obj;\
			dll_internal type ( &var )[ size ] = __glob__1986_memory_##var##_obj._ptr;\
			__glob__1986_memory_##var##_struct::__glob__1986_memory_##var##_struct()\
			{\
				if( _rux_is_memory_manager == 0 )\
					::rux::engine::load_engine();\
				memset( _ptr , 0 , size * sizeof( type ) );\
				_begin_memory_chunk._marker = begin_memory_marker;\
				_begin_memory_chunk._deleted = 0;\
				_begin_memory_chunk._prev = NULL;\
				_begin_memory_chunk._next = NULL;\
				_begin_memory_chunk._module_index = _rux_current_module_index;\
				_begin_memory_chunk._extended_info = NULL;\
				_begin_memory_chunk._stack = 1;\
				_begin_memory_chunk._size = cpp_offsetof( __glob__1986_memory_##var##_struct , _end_memory_chunk ) + sizeof( ::rux::memory::end_memory_chunk );\
				_end_memory_chunk._marker = end_memory_marker;\
				_begin_memory_chunk._diagnostics = ::rux::engine::add_stack_var( &_begin_memory_chunk , __FILE__ , __LINE__ , (char*)#var , #type );\
			};\
			__glob__1986_memory_##var##_struct::~__glob__1986_memory_##var##_struct()\
			{\
				if( _begin_memory_chunk._diagnostics )\
					::rux::engine::remove_stack_var( &_begin_memory_chunk );\
			}
#else
#define declare_stack_variable( type , var , size ) type var[ size ] = { 0 }
#define declare_global_variable( type , var , size ) extern dll_internal type var[ size ]
#define implement_global_variable( type , var , size ) dll_internal type var[ size ] = { 0 }
#endif
#ifdef __WINDOWS__
typedef ::rux::int32 socklen_t;
#define I64d "%I64d"
#define I64x "%I64x"
#define I64u "%I64u"
#define lld "%I64d"
#define llx "%I64x"
#define llu "%I64u"
#else
typedef ::rux::int32 SOCKET;
#define I64d "%lld"
#define I64x "%llx"
#define I64u "%llu"
#define lld "%lld"
#define llx "%llx"
#define llu "%llu"
#endif
#endif
