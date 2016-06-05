#ifndef XAN_MEMORY_HELPER_H
#define XAN_MEMORY_HELPER_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_includes.h>
#include <xan_memorymanager_defines.h>
#define begin_memory_marker 25071986
#define end_memory_marker 18121963
namespace rux
{
	dll_internal char* safe_strncat( char* dest , const char* src , size_t dest_size , size_t src_size = SIZE_MAX );
	dll_internal char* safe_strncpy( char* dest , const char* src , size_t dest_size , size_t src_size = SIZE_MAX );
	namespace memory
	{
		struct summary_memory_info
		{
			const char* _type_name;
			const char* _file_name;
			::rux::int32 _line;
			char _variable_name[ 32 ];
			size_t _size;
			size_t _count;
			::rux::int32 _module_index;
			void** _addresses;
			size_t _addresses_count;
		};
		struct extended_memory_info
		{
			const char* _type_name;
			const char* _file_name;
			::rux::int32 _line;
			char _variable_name[ 32 ];
		};
		class end_memory_chunk;
		class begin_memory_chunk
		{
		public:
			::rux::int32 _marker;
			::rux::byte _deleted;
			size_t _size;
			::rux::int32 _module_index;	
			extended_memory_info* _extended_info;
			::rux::byte _stack;
			::rux::byte _mmap;
			begin_memory_chunk* _prev;
			begin_memory_chunk* _next;
			::rux::byte _diagnostics;
		};
		class end_memory_chunk
		{
		public:
			::rux::int32 _marker;
		};
		dll_internal void check_memory( ::rux::memory::begin_memory_chunk* bmc );
		dll_internal void check_memory( const void* dst );
		dll_internal void check_memory( const void* dst , size_t dst_offset , size_t size );
		dll_internal ::rux::uint8 is_diagnostics_memory( const void* dst );
		dll_internal void write_corruption_error( const char* memory );
		template< class T >
		dll_internal void release( T*& memory )
		{
			if( memory )
			{
				::rux::engine::free_object< T >( memory );
				memory = NULL;
			}
		};
		template< class T , size_t step >
		dll_internal void copy( T*& ptr , size_t& ptr_items_count , size_t index , const T* src , size_t src_items_count )
		{
			if( index + src_items_count > ptr_items_count )
			{
				size_t new_count = ptr_items_count + step;
				if( index + src_items_count > new_count )
					new_count = index + src_items_count + step;
				T* new_ptr = alloc_array_macros( T , new_count );
				if( ptr )
				{
					::rux::engine::memcpy( new_ptr , 0 , ptr , sizeof( T ) * ptr_items_count );
					::rux::engine::free_mem( ptr );
				}
				ptr = new_ptr;
				ptr_items_count = new_count;
			}
			::rux::engine::memcpy( ptr , sizeof( T ) * index , src , sizeof( T ) * src_items_count );
		};
		template< class T , size_t step >
		dll_internal void expand( T*& ptr , size_t& ptr_items_count , size_t index , size_t src_items_count )
		{
			if( index + src_items_count > ptr_items_count )
			{
				size_t new_count = ptr_items_count + step;
				if( index + src_items_count > new_count )
					new_count = index + src_items_count + step;
				T* new_ptr = alloc_array_macros( T , new_count );
				if( ptr )
				{
					::rux::engine::memcpy( new_ptr , 0 , ptr , sizeof( T ) * ptr_items_count );
					::rux::engine::free_mem( ptr );
				}
				ptr = new_ptr;
				ptr_items_count = new_count;
			}
		};
	};
};
#endif