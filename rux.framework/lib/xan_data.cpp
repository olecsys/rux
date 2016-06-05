#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_data.h>
#include <xan_memorymanager_defines.h>
XData::XData()
{
	_offset = 0;
	_data = NULL;
	_data_size = 0;
	_allocated_size = 0;
};
XData::XData( void* data , size_t data_size )
{
	_offset = 0;
	_data = NULL;
	_data_size = 0;
	_allocated_size = 0;
	if( data_size > 0 )
	{
		_data = alloc_array_macros( char , data_size );
		_allocated_size = data_size;
	}
	if( data && data_size > 0 )
	{
		::rux::engine::memcpy( _data , 0 , data , data_size );
		_data_size = data_size;
	}
};
XData::XData( size_t data_size )
{
	_offset = 0;
	_data = NULL;
	_data_size = 0;
	_allocated_size = 0;
	if( data_size > 0 )
	{
		_data = alloc_array_macros( char , data_size );
		_allocated_size = data_size;
	}
};
XData::~XData()
{
	if( _data )
		::rux::engine::free_mem( _data );
};
void XData::Copy( void* data , size_t data_size , const char* file , ::rux::int32 line )
{		
	if( _data && _allocated_size < data_size )
	{
		::rux::engine::free_mem( _data );
		_data = NULL;
		_allocated_size = 0;
		_data_size = 0;
	}
	if( _data == NULL && data_size > 0 )
	{
		if( file && line )
			_data = ::rux::engine::alloc_array< char >( data_size , file , line );
		else
			_data = alloc_array_macros( char , data_size );
		_allocated_size = data_size;
	}
	if( data && data_size > 0 )
	{
		_offset = 0;
		::rux::engine::memcpy( _data , 0 , data , data_size );
		_data_size = data_size;
	}
};
void XData::ReAlloc( size_t data_size , const char* file , ::rux::int32 line )
{
	if( _data && _allocated_size < data_size )
	{
		_allocated_size += data_size;
		void* new_data = 0;
		if( file && line )
			new_data = ::rux::engine::alloc_array< char >( _allocated_size , file , line );
		else
			new_data = alloc_array_macros( char , _allocated_size );
		if( _data_size > 0 )
			::rux::engine::memcpy( new_data , 0 , _data , _data_size );
		::rux::engine::free_mem( _data );
		_data = new_data;
	}
	if( _data == NULL && data_size > 0 )
	{
		if( file && line )
			_data = ::rux::engine::alloc_array< char >( _allocated_size , file , line );
		else
			_data = alloc_array_macros( char , data_size );
		_allocated_size = data_size;
	}
};
void XData::Concatenate( void* data , size_t data_size , const char* file , ::rux::int32 line )
{
	if( _data && ( _allocated_size - _data_size ) < data_size )
	{
		_allocated_size += data_size;
		void* new_data = 0;
		if( file && line )
			new_data = ::rux::engine::alloc_array< char >( _allocated_size , file , line );
		else
			new_data = alloc_array_macros( char , _allocated_size );
		if( _data_size > 0 )
			::rux::engine::memcpy( new_data , 0 , _data , _data_size );
		::rux::engine::free_mem( _data );
		_data = new_data;
	}
	if( _data == NULL && data_size > 0 )
	{
		_data = alloc_array_macros( char , data_size );
		_allocated_size = data_size;
	}
	if( data && data_size > 0 )
	{
		::rux::engine::memcpy( _data , _data_size , data , data_size );
		_data_size += data_size;
	}
};
void* XData::Read( size_t read_size )
{
	void* read_data = NULL;
	if( _data && _data_size > 0 )
	{
		if( _offset + read_size <= _data_size )
		{
			read_data = &( (::rux::uint8*)_data )[ _offset ];
			_offset += read_size;
			if( _data_size == _offset )
			{
				_data_size = 0 ;
				_offset = 0;
			}
		}
	}
	return read_data;
};