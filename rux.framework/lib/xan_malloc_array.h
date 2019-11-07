#ifndef XAN_MALLOC_ARRAY_H
#define XAN_MALLOC_ARRAY_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_includes.h"
#ifndef RUX_ARRAY_MAX_STEP
#define RUX_ARRAY_MAX_STEP (size_t)524288
#endif
template< class T , size_t step = 30 , size_t empty_start_index_count = 1 >
class XMallocArray
{	
	friend class XMemoryManager;
private:	
	size_t _size;
	size_t _empty_count;
	size_t _empty_start_indexes[ empty_start_index_count ];
public:		
	size_t _count;
	size_t _step;
	T* _array_ptr;
	XMallocArray()
	{
		_array_ptr = NULL;
		_size = 0;
		_count = 0;
		_empty_count = 0;
		for( size_t index0 = 0 ; index0 < empty_start_index_count ; index0++ )
			_empty_start_indexes[ index0 ] = SIZE_MAX;
		_step = step;
	};
	XMallocArray( XMallocArray& Array )
	{		
		_array_ptr = NULL;
		_size = 0;
		_count = 0;
		_empty_count = 0;
		for( size_t index0 = 0 ; index0 < empty_start_index_count ; index0++ )
			_empty_start_indexes[ index0 ] = SIZE_MAX;
		if( Array._count > 0 )
		{
			_size = Array._count;
			_count = Array._count;			
			_array_ptr = alloc_array_macros( T , _size );
			::rux::engine::memcpy( _array_ptr , 0 , Array._array_ptr , _count * sizeof( T ) );		
		}
	}
	XMallocArray( T* pArray , size_t ulCount )
	{		
		_array_ptr = NULL;
		_size = 0;
		_count = 0;
		_empty_count = 0;
		for( size_t index0 = 0 ; index0 < empty_start_index_count ; index0++ )
			_empty_start_indexes[ index0 ] = SIZE_MAX;
		if( pArray != NULL
			&& ulCount > 0 )
		{
			_size = ulCount;
			_count = ulCount;
			_array_ptr = alloc_array_macros( T , ulCount );
			::rux::engine::memcpy( _array_ptr , 0 , pArray , ulCount * sizeof( T ) );
		}
	}	
	virtual ~XMallocArray()
	{
		FreeMemory();
	};
	void FreeMemory( void )
	{
		if( _array_ptr )
		{
			::rux::engine::free_mem( _array_ptr );
			_array_ptr = NULL;
			_size = 0;
			_count = 0;
		}
	};
	::rux::uint64 get_ActualSize( void )
	{
		return _size * sizeof( T );
	}
	void DecreaseMemory( void )
	{
		if( _size - _count  > _step )
		{
			_size = _count + _step;
			T* new_array_ptr = alloc_array_macros( T , _size );
			memset( new_array_ptr , 0 , _size * sizeof( T ) );
			::rux::engine::memcpy( new_array_ptr , 0 , _array_ptr , _count * sizeof( T ) );
			if( _array_ptr )
				::rux::engine::free_mem( _array_ptr );
			_array_ptr = new_array_ptr;
		}
	};
	size_t Add( const T& item )
	{
		Insert( _count , item );
		return _count - 1;
	}
	void set_Item( size_t index , const T& item )
	{
		if( index > _count )
			index = _count;
		_array_ptr[ index ] = item;
	};
	void AddRange( XMallocArray<T>& Range )
	{
		InsertRange( _count , Range );
	}
	void AddRange( T* pRange , size_t ulCount )
	{
		InsertRange( _count , pRange , ulCount );
	}
	void Insert( size_t index , const T& item )
	{
		if( index > _count )
			index = _count;
		_count++;
		if( _count > _size )
		{
			_size += get_step();
			T* new_array = alloc_array_macros( T , _size );
			memset( new_array , 0 , _size * sizeof( T ) );
			if( index > 0 )
				::rux::engine::memcpy( new_array , 0 , _array_ptr , sizeof( T ) * index );			
			if( _count - 1 > index )
				::rux::engine::memcpy( new_array , ( index + 1 ) * sizeof( T ) , &_array_ptr[ index ] , sizeof( T ) * ( _count - index - 1 ) );			
			if( _array_ptr )
				::rux::engine::free_mem( _array_ptr );
			_array_ptr = new_array;
		}		
		else if( _count - 1 > index )
			::rux::engine::memmove( _array_ptr , ( index + 1 ) * sizeof( T ) , _array_ptr , index * sizeof( T ) , sizeof( T ) * ( _count - index - 1 ) );
		_array_ptr[ index ] = item;
	};
	void InsertRange( size_t index , XMallocArray<T>& range )
	{		
		InsertRange( index , range._array_ptr , range._count );
	};
	void InsertRange( size_t index , T* range , size_t count )
	{
		if( count > 0 )
		{
			if( index > _count )
				index = _count;
			_count += count;
			if( _count > _size )
			{
				if( count > _step )
					_size += count;
				else
					_size += get_step();
				T* new_array = alloc_array_macros( T , _size );
				memset( new_array , 0 , _size * sizeof( T ) );
				if( index > 0 )
					::rux::engine::memcpy( new_array , 0 , _array_ptr , sizeof( T ) * index );
				if( _count - count - index > 0 )
					::rux::engine::memcpy( new_array , ( index + count ) * sizeof( T ) , &_array_ptr[ index ] , sizeof( T ) * ( _count - index - count ) );
				if( _array_ptr )
					::rux::engine::free_mem( _array_ptr );
				_array_ptr = new_array;			
			}
			else if( _count - count - index > 0 )
				::rux::engine::memmove( _array_ptr , ( index + count ) * sizeof( T ) , _array_ptr , index * sizeof( T ) , sizeof( T ) * ( _count - index - count ) );
			::rux::engine::memcpy( _array_ptr , sizeof( T ) * index , range , sizeof( T ) * count );
		}
	};
	void RemoveAt( size_t index )
	{
		if( index < _count )
		{			
			if( _count - 1 > index )
				::rux::engine::memmove( _array_ptr , index * sizeof( T ) , _array_ptr , ( index + 1 ) * sizeof( T ) , sizeof( T ) * ( _count - index - 1 ) );
			_count--;
			_array_ptr[ _count ] = 0;
		}
	};	
	size_t FillEmptyOrAdd( const T& item )
	{
		size_t position = 0;
		if( _empty_count > 0 )
		{	
			T* ptr = NULL;
			size_t index0 = empty_start_index_count - 1;
			for( ; ; index0-- )
			{
				if( _empty_start_indexes[ index0 ] != SIZE_MAX )
				{
					position = _empty_start_indexes[ index0 ];
					ptr = &_array_ptr[ position ];
					_empty_count--;
					::memcpy( ptr , &item , sizeof( T ) );					
					_empty_start_indexes[ index0 ] = SIZE_MAX;
					break;
				}
				if( index0 == 0 )
					break;
			}
			if( _empty_count > 0 )
			{					
				if( index0 == 0 )
				{				
					T* end = _array_ptr + _count;
					while( *ptr != 0 )
					{
						ptr++;
						if( ptr >= end )
							ptr = _array_ptr;
					}
					_empty_start_indexes[ 0 ] = ptr - _array_ptr;
				}
			}
		}
		else
		{			
			_count++;			
			if( _count > _size )
			{
				_size += get_step();
				T* new_array = alloc_array_macros( T , _size );
				memset( new_array , 0 , _size * sizeof( T ) );
				if( _count - 1 > 0 )
					::rux::engine::memcpy( new_array , 0 , _array_ptr , sizeof( T ) * ( _count - 1 ) );
				if( _array_ptr )
					::rux::engine::free_mem( _array_ptr );
				_array_ptr = new_array;
			}				
			_array_ptr[ _count - 1 ] = item;
			position = _count - 1;
		}
		return position;
	};
	void set_EmptyAt( size_t index )
	{
		if( index < _count )
		{
			if( _array_ptr[ index ] != 0 )
			{
				_array_ptr[ index ] = 0;
				size_t index0 = 0;
				for( ; index0 < empty_start_index_count ; index0++ )
				{
					if( _empty_start_indexes[ index0 ] > index )
					{
						if( index0 + 1 < empty_start_index_count )
							::memmove( &_empty_start_indexes[ index0 + 1 ] , &_empty_start_indexes[ index0 ] , ( empty_start_index_count - index0 - 1 ) * sizeof( size_t ) );
						_empty_start_indexes[ index0 ] = index;
						break;
					}
				}
				_empty_count++;
			}
		}
	};
	void ChangePosition( size_t old_index , size_t new_index )
	{
		if( old_index < _count
			&& old_index != new_index )
		{
			T item = _array_ptr[ old_index ];
			RemoveAt( old_index );
			Insert( new_index , item );
		}
	};
	void Swap( size_t old_index , size_t new_index )
	{
		ChangePosition( old_index , new_index );
	};
	void ChangeRangePosition( size_t old_index , size_t new_index , size_t count )
	{
		if( old_index != new_index 
			&& old_index < _count
			&& count > 0 )
		{
			if( old_index + count > _count )
				count = _count - old_index;
			if( count > 0 )
			{
				T* temp_array = alloc_array_macros( T , count );
				::rux::engine::memcpy( temp_array , 0 , &_array_ptr[ old_index ] , count * sizeof( T ) );
				RemoveRange( old_index , count );
				InsertRange( new_index , temp_array , count );
				::rux::engine::free_mem( temp_array );
			}
		}
	};
	size_t RemoveRange( size_t index , size_t count )
	{
		if( index < _count
			&& count > 0 )
		{
			if( index + count > _count )
				count = _count - index;						
			if( _count - count > index )
				::rux::engine::memmove( _array_ptr , index * sizeof( T ) , _array_ptr , ( index + count ) * sizeof( T ) , sizeof( T ) * ( _count - index - count ) );			
			_count -= count;
			memset( &_array_ptr[ _count ] , 0 , count );
		}
		else
			count = 0;
		return count;
	};	
	void Clear()
	{
		_empty_count = 0;
		for( size_t index0 = 0 ; index0 < empty_start_index_count ; index0++ )
			_empty_start_indexes[ index0 ] = SIZE_MAX;
		//memset( _array_ptr , 0 , _count * sizeof( T ) ) ;
		_count = 0;
	};
	::rux::int32 IndexOf( T& item )
	{
		return IndexOf( item , 0 );
	};
	::rux::int32 IndexOf( T& item , size_t index )
	{
		::rux::int32 local_index = -1;	
		for( index ; index < _count ; index++ )
		{
			if( (*this)[ index ] == item )
			{
				local_index = index;
				break;
			}
		}
		return local_index;	
	};
	::rux::int32 LastIndexOf( T& item )
	{
		return LastIndexOf( item , 0 );
	};
	::rux::int32 LastIndexOf( T& item , size_t index )
	{
		::rux::int32 local_index = -1;	
		if( index >= _count )
			index = _count - 1;		
		for( index ; index >= 0 ; index-- )
		{
			if( (*this)[ index ] == item )
			{
				local_index = index;
				break;
			}
		}
		return local_index;	
	};
	void ReplaceAll( T& item1 , T& item2 )
	{
		ReplaceAll( item1 , item2 , 0 );
	};
	void ReplaceAll( T& item1 , T& item2 , size_t index )
	{
		for( index ; index < _count ; index++ )
		{
			if( (*this)[ index ] == item1 )
			{
				Insert( index , item2 );
				RemoveAt( index + 1 );				
			}
		}
	};
	void ReplaceFirst( T& item1 , T& item2 )
	{
		ReplaceFirst( item1 , item2 , 0 );
	};
	void ReplaceFirst( T& item1 , T& item2 , size_t index )
	{
		for( index ; index < _count ; index++ )
		{
			if( (*this)[ index ] == item1 )
			{
				Insert( index , item2 );
				RemoveAt( index + 1 );				
				break;
			}
		}
	};
	void ReplaceLast( T& item1 , T& item2 )
	{
		ReplaceLast( item1 , item2 , 0 );
	};
	void ReplaceLast( T& item1 , T& item2 , size_t index )
	{
		if( index >= _count )
			index = _count - 1;
		for( index ; index >= 0 ; index-- )
		{
			if( (*this)[ index ] == item1 )
			{
				Insert( index , item2 );
				RemoveAt( index + 1 );				
				break;
			}
		}
	};
	size_t Count()
	{
		return _count;
	};	
	size_t ItemsCount() 
	{
		return _count - _empty_count;
	};
	T& operator[] ( size_t index )
	{
		return _array_ptr[ index ];
	};
	T* operator()()
	{
		return _array_ptr;
	};
	XMallocArray& operator=( XMallocArray<T>& Array )
	{
		Clear();
		AddRange( Array );
		return *this;
	};
	rux_inline size_t get_step( void )
	{
		if( _step < RUX_ARRAY_MAX_STEP )
			return _step *= 2;
		else
			return _step;
	};
};
#endif
