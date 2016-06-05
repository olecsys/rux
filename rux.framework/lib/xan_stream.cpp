#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_stream.h>
#include <xan_memorymanager_defines.h>
namespace rux
{
	namespace io
	{
		stream::~stream()
		{
		};
		void stream::release( void ) const
		{
			::rux::engine::free_object< ::rux::io::stream >( const_cast< ::rux::io::stream* >( this ) );
		};
		memory_stream::memory_stream( void )
		{
			_buffer_allocated_size = 0;
			_buffer_size = 0;
			_buffer_offset = 0;
			_buffer = 0;
			_delete_here = 1;
		};
		memory_stream::memory_stream( ::rux::byte* buffer , size_t buffer_size , ::rux::byte copy , ::rux::byte delete_here )
		{
			_buffer_allocated_size = 0;
			_buffer_size = 0;
			_buffer_offset = 0;
			_buffer = 0;
			_delete_here = 1;
			set_buffer( buffer , buffer_size , copy , delete_here );
		};
		void memory_stream::set_buffer( ::rux::byte* buffer , size_t buffer_size , ::rux::byte copy , ::rux::byte delete_here )
		{
			if( buffer && buffer_size )
			{
				_delete_here = delete_here;
				if( copy )
				{
					if( _buffer_allocated_size < buffer_size )
						free_buffer();
					if( _buffer == NULL )
					{
						_buffer = alloc_array_macros( ::rux::byte , buffer_size );
						_buffer_allocated_size = buffer_size;
						_buffer_size = buffer_size;
					}
					::memcpy( _buffer , buffer , buffer_size );
				}
				else
				{
					free_buffer();
					_buffer = buffer;
					_buffer_allocated_size = buffer_size;
					_buffer_size = buffer_size;
				}
			}
			else
				free_buffer();
		};
		void memory_stream::free_buffer( void )
		{
			if( _buffer )
			{
				if( _delete_here )
					::rux::engine::free_mem( _buffer );
				_buffer_allocated_size = 0;
				_buffer_size = 0;
				_buffer_offset = 0;				
				_buffer = NULL;
			}
		};
		memory_stream::~memory_stream( void )
		{
			free_buffer();
		};
		bool memory_stream::opened( void )
		{
			return _buffer ? true : false;
		};
		bool memory_stream::write_bytes( char* buffer , size_t buffer_size , size_t& written )
		{
			if( _buffer )
			{
				if( _buffer_size >= _buffer_offset + buffer_size )
					written = buffer_size;
				else
					written = _buffer_size - _buffer_offset;
				if( written )
				{
					::memcpy( &_buffer[ _buffer_offset ] , buffer , written );
					_buffer_offset += written;
					return true;
				}
				else
					return false;
			}
			else
			{
				written = 0;
				return false;
			}
		};
		bool memory_stream::read_bytes( char* buffer , size_t buffer_size , size_t& readen )
		{
			if( _buffer )
			{
				if( _buffer_size >= _buffer_offset + buffer_size )
					readen = buffer_size;
				else
					readen = _buffer_size - _buffer_offset;
				if( readen )
				{
					::memcpy( buffer , &_buffer[ _buffer_offset ] , readen );
					_buffer_offset += readen;
					return true;
				}
				else
					return false;
			}
			else
			{
				readen = 0;
				return false;
			}
		};
		::rux::uint64 memory_stream::position( void )
		{
			return _buffer_offset;
		};
		bool memory_stream::seek( ::rux::uint64 position , XEnum_SetPosition type )
		{
			if( type == XEnum_SetPosition_FromBegin )
			{
				if( position >= _buffer_size )
					return false;
				else
				{
					_buffer_offset = position;
					return true;
				}
			}
			else if( type == XEnum_SetPosition_FromEnd )
			{
				if( position > _buffer_size )
					return false;
				else
				{
					_buffer_offset = _buffer_size - position;
					return true;
				}
			}
			else if( type == XEnum_SetPosition_FromCurrentPosition )
			{
				if( _buffer_size >= _buffer_offset + position )
				{
					_buffer_offset += position;
					return true;
				}
				else
					return false;
			}
			else
				return false;
		};
	};
};