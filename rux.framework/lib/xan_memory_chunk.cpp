#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_memory_chunk.h>
#include <xan_memorymanager_defines.h>
namespace rux
{
	namespace memory
	{
		chunk_t::chunk_t( ::rux::uint32 type )
		{
			_actual_size = 0;
			_allocated_size = 0;
			_data = 0;
			_type = type;
		};
		chunk_t::~chunk_t()
		{
		};
		size_t chunk_t::actual_size( void )
		{
			return _actual_size;
		};
		void chunk_t::clear( void )
		{
			_actual_size = 0;
		};
		void chunk_t::reduce_actual_size_of( size_t size )
		{
			if( size > _actual_size )
				_actual_size = 0;
			else
				_actual_size -= size;
			if( _type == ::rux::memory::string_type )
				_data[ _actual_size ] = 0;
		};
		heap_chunk_t::heap_chunk_t( ::rux::uint32 type , size_t step )
			: chunk_t( type )
		{			
			_step = step;
			if( _step == 0 )
				_step = 64;
		};
		heap_chunk_t::~heap_chunk_t()
		{
			if( _data )
				::rux::engine::free_mem( _data );
		};		
		void heap_chunk_t::concatenate( void* ptr , size_t size )
		{
			if( _type == ::rux::memory::string_type )
			{
				if( _actual_size + size + 1 > _allocated_size )
				{
					_allocated_size = _actual_size + size + _step;
					_data = ::rux::engine::realloc_array< ::rux::byte >( _data , _allocated_size );
				}
			}
			else if( _type == ::rux::memory::pointer_type )
			{
				if( _actual_size + size > _allocated_size )
				{
					_allocated_size = _actual_size + size + _step;
					_data = ::rux::engine::realloc_array< ::rux::byte >( _data , _allocated_size );
				}
			}		
			::rux::engine::memcpy( _data , _actual_size , ptr , size );
			_actual_size += size;
			if( _type == ::rux::memory::string_type )
				_data[ _actual_size ] = 0;
		};
		stack_chunk_t::stack_chunk_t( ::rux::byte* ptr , size_t allocated_size , ::rux::uint32 type , ::rux::byte check_memory )
			: chunk_t( type )
		{			
			_allocated_size = allocated_size;
			_data = ptr;			
			_check_memory = check_memory;
		};
		stack_chunk_t::~stack_chunk_t()
		{			
		};
		void stack_chunk_t::concatenate( void* ptr , size_t size )
		{
			if( _type == ::rux::memory::string_type )
			{
				if( _actual_size + size + 1 > _allocated_size )
					size = _allocated_size - _actual_size - 1;
			}
			else if( _type == ::rux::memory::pointer_type )
			{
				if( _actual_size + size > _allocated_size )
					size = _allocated_size - _actual_size;
			}		
			if( size > 0 )
			{
				if( _check_memory )
					::rux::engine::memcpy( _data , _actual_size , ptr , size );
				else
					::memcpy( &_data[ _actual_size ] , ptr , size );
				_actual_size += size;
				if( _type == ::rux::memory::string_type )
					_data[ _actual_size ] = 0;
			}			
		};
	};
};