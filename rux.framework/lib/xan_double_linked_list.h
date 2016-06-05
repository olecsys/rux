#ifndef XAN_DOUBLE_LINKED_LIST_H
#define XAN_DOUBLE_LINKED_LIST_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_includes.h>
namespace rux
{
	namespace data
	{
		template< typename T >
		class DoubleLinkedListItem
		{
			template< typename T0 >
			friend class DoubleLinkedList;
		private:
			char _data[ sizeof( T ) ];
			DoubleLinkedListItem< T >* _prev;
			DoubleLinkedListItem< T >* _next;
		public:
			DoubleLinkedListItem( void )
			{
				new ( (void*)_data )T();
				_prev = 0;
				_next = 0;
			};
			~DoubleLinkedListItem( void )
			{
				T* item = reinterpret_cast< T*>( _data );
				item->~T();
			};
			T* operator-> ( void )
			{			
				return reinterpret_cast< T*>( _data );
			};
			DoubleLinkedListItem< T >* Next( void )
			{
				return _next;
			};
		};
		template< typename T >
		class DoubleLinkedList
		{
		private:
			size_t _count;
			DoubleLinkedListItem< T >* _begin;
			DoubleLinkedListItem< T >* _end;
		public:
			DoubleLinkedList( void )
			{
				Clear();
			};
			void Add( DoubleLinkedListItem< T >* item )
			{
				if( _begin == 0 )
				{
					_begin = item;
					_begin->_prev = 0;
					_begin->_next = 0;
				}
				else
				{
					if( _end == 0 )
					{
						_begin->_next = item;
						item->_prev = _begin;
					}
					else
					{
						_end->_next = item;
						item->_prev = _end;
					}
					_end = item;
					_end->_next = 0;
				}
				_count++;
			};
			void Remove( DoubleLinkedListItem< T >* item )
			{
				if( _begin == item )
					_begin = item->_next;
				if( item->_next )
					item->_next->_prev = item->_prev;
				else
					_end = item->_prev;
				if( item->_prev )
					item->_prev->_next = item->_next;
				_count--;
			};
			void Clear( void )
			{
				_begin = 0;
				_end = 0;
				_count = 0;
			};
			DoubleLinkedListItem< T >* operator[] ( size_t index )
			{		
				if( index == 0 )
					return _begin;
				else
				{
					size_t index0 = 0;
					DoubleLinkedListItem< T >* item = _begin;
					for( ; ; )
					{
						item = item->_next;
						index0++;
						if( index0 == index || item == 0 )
							break;
					}
					return item;
				}
			};
			size_t Count( void )
			{
				return _count;
			};
			DoubleLinkedListItem< T >* Begin( void )
			{
				return _begin;
			};
		};
	};
};
#endif