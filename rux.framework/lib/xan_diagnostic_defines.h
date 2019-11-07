#ifndef XAN_DIAGNOSTIC_DEFINES_H
#define XAN_DIAGNOSTIC_DEFINES_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_utils.h"
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
#endif
