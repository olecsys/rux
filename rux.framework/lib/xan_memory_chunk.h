#ifndef XAN_MEMORY_CHUNK_H
#define XAN_MEMORY_CHUNK_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_includes.h>
#endif
namespace rux
{
	namespace memory
	{
		class heap_chunk_t;
		class stack_chunk_t;
		const ::rux::uint32 string_type = 0;
		const ::rux::uint32 pointer_type = 1;
		class chunk_t
		{
			friend class heap_chunk_t;
			friend class stack_chunk_t;
			::rux::uint32 _type;
			size_t _actual_size;
			size_t _allocated_size;
		public:
			::rux::byte* _data;
			chunk_t( ::rux::uint32 type = ::rux::memory::string_type );
			virtual ~chunk_t();
			virtual void concatenate( void* ptr , size_t size ) = 0;
			virtual size_t actual_size( void );
			virtual void clear( void );
			virtual void reduce_actual_size_of( size_t size );
		};
		class heap_chunk_t : public ::rux::memory::chunk_t
		{	
			size_t _step;
		public:
			heap_chunk_t( ::rux::uint32 type = ::rux::memory::string_type , size_t step = 64 );
			~heap_chunk_t();
			virtual void concatenate( void* ptr , size_t size );
		};
		class stack_chunk_t : public ::rux::memory::chunk_t
		{	
			::rux::byte _check_memory;
		public:
			stack_chunk_t( ::rux::byte* ptr , size_t allocated_size , ::rux::uint32 type = ::rux::memory::string_type , ::rux::byte check_memory = 1 );
			~stack_chunk_t();
			virtual void concatenate( void* ptr , size_t size );
		};
	}
};
