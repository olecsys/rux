#ifndef XAN_GC_DEFINES_H
#define XAN_GC_DEFINES_H
#include <xan_defines.h>
namespace rux
{
	namespace memory
	{
		class begin_memory_chunk;
	};
};
namespace rux
{	
	namespace engine
	{
		dll_internal ::rux::uint8 add_stack_var( ::rux::memory::begin_memory_chunk* _begin_memory_chunk , const char* file , ::rux::uint32 line , char* variable_name , const char* type_name );
		dll_internal void remove_stack_var( ::rux::memory::begin_memory_chunk* _begin_memory_chunk );		
		dll_internal void add_gc_ref( XGCRef* gc_ref_ptr );
		dll_internal void* alloc_array_malloc_dummy( size_t size );
		dll_internal void free_mem_free_dummy( void* ptr );		
	};
};
#endif
