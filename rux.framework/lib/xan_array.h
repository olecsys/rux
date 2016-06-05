#ifndef XAN_ARRAY_H
#define XAN_ARRAY_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_diagnostic_defines.h>
#include <xan_defines.h>
#include <xan_function_info.h>
#include <xan_property_info.h>
#ifndef RUX_ARRAY_MAX_STEP
#define RUX_ARRAY_MAX_STEP (::rux::uint32)524288
#endif
namespace rux
{
	template < class T > class XArray;
	template< class T , ::rux::int32 step = 5 >
	class Array : public XGCRef
	{		
		template < class T1 >
		friend class XArray;
		static void register_methods_and_properties( Type* type );
		static XGCRef& static_get_Item( const XGCRef* object , const XGCRef* param0 );
		static XGCRef& static_CheckAllItemType( const XGCRef* object , const XGCRef* param0 );
		static XGCRef& static_Count( const XGCRef* object );
		declare_rux_base_internal_methods()
		{
			XArray< T > obj;
			return obj++;
		};
	private:
		XGCRef** _array_ptr;
		size_t _size;
		size_t _count;
		size_t _step;
	public:		
		explicit Array( const char* variable_name = 0 , const char* __file__ = 0 , ::rux::int32 __line__ = 0 ) 
			: XGCRef( get_static_Type )
		{
			UNREFERENCED_PARAMETER( variable_name );
			UNREFERENCED_PARAMETER( __file__ );
			UNREFERENCED_PARAMETER( __line__ );
			_array_ptr = NULL;
			_size = 0;
			_count = 0;
			_step = step;
		};
		virtual ~Array()
		{		
			if( _array_ptr )
			{				
				for( size_t index0 = 0 ; index0 < _count ; index0++ )
					_array_ptr[ index0 ]->Release();
				::rux::engine::free_mem( _array_ptr );
			}
		};	
		static void static_Add( const XGCRef* object , const XGCRef* param0 )
		{
			::rux::Array< T >* arr = (::rux::Array< T >*)object->get_ObjectAddress();
			arr->Add( *param0 );
		};
		static void static_AddByRef( const XGCRef* object , const XGCRef* param0 )
		{
			::rux::Array< T >* arr = (::rux::Array< T >*)object->get_ObjectAddress();
			arr->AddByRef( *param0 );
		};
		void Add( const T& item , const char* __file__ = 0 , ::rux::int32 __line__ = 0 )
		{	
			if( __file__ == 0 )
			{
				__file__ = __FILE__;
				__line__ = __LINE__;
			}
			Insert( _count , item.get_GCRef( 1 , 0 ) , __file__ , __line__ );
		}	
		void Add( const typename T::InternalClass& item , const char* __file__ = 0 , ::rux::int32 __line__ = 0 )
		{	
			if( __file__ == 0 )
			{
				__file__ = __FILE__;
				__line__ = __LINE__;
			}
			Insert( _count , static_cast< XGCRef* >( const_cast< typename T::InternalClass* >( &item ) ) , __file__ , __line__ );
		}	
		void Add( const XGCRef& item , const char* __file__ = 0 , ::rux::int32 __line__ = 0 )
		{	
			if( __file__ == 0 )
			{
				__file__ = __FILE__;
				__line__ = __LINE__;
			}
			Insert( _count , const_cast< XGCRef* >( &item ) , __file__ , __line__ );
		};	
		void Add( const XGCRef* item , const char* __file__ = 0 , ::rux::int32 __line__ = 0 )
		{	
			if( __file__ == 0 )
			{
				__file__ = __FILE__;
				__line__ = __LINE__;
			}
			Insert( _count , const_cast< XGCRef* >( item ) , __file__ , __line__ );
		};
		void AddByRef( const T& item )
		{			
			InsertByRef( _count , item.get_GCRef( 1 , 0 ) );
		};	
		void AddByRef( const typename T::InternalClass& item )
		{
			InsertByRef( _count , static_cast< XGCRef* >( const_cast< typename T::InternalClass* >( &item ) ) );
		};
		void AddByRef( const XGCRef& item )
		{
			InsertByRef( _count , const_cast< XGCRef* >( &item ) );
		};
		void AddRange( const Array< T >& range )
		{
			InsertRange( _count , range );
		}
		void AddRange( typename T::InternalClass** range_ptr , size_t count )
		{
			InsertRange( _count , range_ptr , count );
		}
		void AddRange( const typename T::InternalClass** range_ptr , size_t count )
		{
			InsertRange( _count , range_ptr , count );
		}
		void IncreaseMemory( size_t items_count )
		{
			_size += items_count;
			XGCRef** new_array = alloc_array_macros( XGCRef* , _size );
			if( _array_ptr )
			{
				::rux::engine::memcpy( new_array , 0 , _array_ptr , _count * sizeof( XGCRef* ) );
				::rux::engine::free_mem( _array_ptr );
			}
			_array_ptr = new_array;
		};
		void Insert( size_t index , const T& item , const char* __file__ = 0 , ::rux::int32 __line__ = 0 )
		{	
			if( __file__ == 0 )
			{
				__file__ = __FILE__;
				__line__ = __LINE__;
			}
			Insert( index , item.get_GCRef( 1 , 0 ) , __file__ , __line__ );
		};
		void Insert( size_t index , const typename T::InternalClass& item , const char* __file__ = 0 , ::rux::int32 __line__ = 0 )
		{	
			if( __file__ == 0 )
			{
				__file__ = __FILE__;
				__line__ = __LINE__;
			}
			Insert( index , static_cast< XGCRef* >( const_cast< typename T::InternalClass* >( &item ) ) , __file__ , __line__ );
		};
		void Insert( size_t index , const XGCRef& item , const char* __file__ = 0 , ::rux::int32 __line__ = 0 )
		{	
			if( __file__ == 0 )
			{
				__file__ = __FILE__;
				__line__ = __LINE__;
			}
			Insert( index , const_cast< XGCRef* >( &item ) , __file__ , __line__ );
		};
		void Insert( size_t index , XGCRef* item , const char* __file__ = 0 , ::rux::int32 __line__ = 0 )
		{	
			if( __file__ == 0 )
			{
				__file__ = __FILE__;
				__line__ = __LINE__;
			}
			if( index > _count )
				index = _count;
			_count++;
			if( _count > _size )
			{
				_size += get_step();
				XGCRef** new_array_ptr = ::rux::engine::alloc_array< XGCRef* >( _size , __file__ , __line__ , (char*)0 );
				if( index > 0 )
					::rux::engine::memcpy( new_array_ptr , 0 , _array_ptr , index * sizeof( XGCRef* ) );
				if( _count - index - 1 > 0 )
					::rux::engine::memcpy( new_array_ptr , ( index + 1 ) * sizeof( XGCRef* ) , &_array_ptr[ index ] , ( _count - index - 1 ) * sizeof( XGCRef* ) );
				if( _array_ptr )
					::rux::engine::free_mem( _array_ptr );
				_array_ptr = new_array_ptr;
			}		
			else
			{
				for( size_t index1 = _count - 1 ; index1 > index ; index1-- )
					_array_ptr[ index1 ] = _array_ptr[ index1 - 1 ];
			}			
			T new_item( "obj" , __file__ , __line__ );
			new_item = *item;			
			XGCRef* gcref = new_item.get_GCRef( 1 , 0 );
			gcref->AddRef();
			_array_ptr[ index ] = gcref;
		};
		void InsertByRef( size_t index , XGCRef* item )
		{			
			if( index > _count )
				index = _count;
			_count++;
			if( _count > _size )
			{
				_size += get_step();
				XGCRef** new_array_ptr = alloc_array_macros( XGCRef* , _size );
				if( index > 0 )
					::rux::engine::memcpy( new_array_ptr , 0 , _array_ptr , index * sizeof( XGCRef* ) );
				if( _count - index - 1 > 0 )
					::rux::engine::memcpy( new_array_ptr , ( index + 1 ) * sizeof( XGCRef* ) , &_array_ptr[ index ] , ( _count - index - 1 ) * sizeof( XGCRef* ) );
				if( _array_ptr )
					::rux::engine::free_mem( _array_ptr );
				_array_ptr = new_array_ptr;
			}		
			else
			{
				for( size_t index1 = _count - 1 ; index1 > index ; index1-- )
					_array_ptr[ index1 ] = _array_ptr[ index1 - 1 ];
			}
			item->AddRef();
			_array_ptr[ index ] = item;
		};
		void set_Item( size_t index , const T& item )
		{			
			set_Item( index , item.get_GCRef( 1 , 0 ) );
		};
		void set_Item( size_t index , const typename T::InternalClass& item )
		{
			set_Item( index , static_cast< XGCRef* >( const_cast< typename T::InternalClass* >( &item ) ) );
		};
		void set_Item( size_t index , XGCRef* item , const char* __file__ = 0 , ::rux::int32 __line__ = 0 )
		{	
			if( __file__ == 0 )
			{
				__file__ = __FILE__;
				__line__ = __LINE__;
			}
			if( index < _count )
			{
				_array_ptr[ index ]->Release();				
				T new_item( "obj" , __file__ , __line__ );
				new_item = *item;			
				XGCRef* gcref = new_item.get_GCRef( 1 , 0 );
				gcref->AddRef();
				_array_ptr[ index ] = gcref;
			}
		};
		void set_ItemByRef( size_t index , const T& item )
		{			
			set_ItemByRef( index , item.get_GCRef( 1 , 1 ) );
		};
		void set_ItemByRef( size_t index , const typename T::InternalClass& item )
		{
			set_ItemByRef( index , static_cast< XGCRef* >( const_cast< typename T::InternalClass* >( &item ) ) );
		};
		void set_ItemByRef( size_t index , XGCRef* item )
		{
			if( index < _count )
			{
				_array_ptr[ index ]->Release();
				item->AddRef();
				_array_ptr[ index ] = item;
			}
		};
		void InsertRange( size_t index , const Array< T >& range )
		{		
			InsertRange( index , range._array_ptr , range._count );
		};
		void InsertRange( size_t index , XGCRef** range_ptr , size_t count , const char* __file__ = 0 , ::rux::int32 __line__ = 0 )
		{	
			if( __file__ == 0 )
			{
				__file__ = __FILE__;
				__line__ = __LINE__;
			}
			if( count > 0 )
			{
				::rux::uint8 equal = _array_ptr == range_ptr ? 1 : 0;		
				if( index > _count )
					index = _count;
				_count += count;
				if( _count > _size 
					|| equal == 1 )
				{
					if( _count > _size )
					{
						if( count > _step )
							_size += count;
						else
							_size += get_step();
					}
					XGCRef** new_array_ptr = alloc_array_macros( XGCRef* , _size );
					if( index > 0 )
						::rux::engine::memcpy( new_array_ptr , 0 , _array_ptr , index * sizeof( XGCRef* ) );
					if( _count - index - count > 0 )
						::rux::engine::memcpy( new_array_ptr , ( index + count ) * sizeof( XGCRef* ) , &_array_ptr[ index ] , ( _count - index - count ) * sizeof( XGCRef* ) );
					for( size_t index1 = index ; index1 < index + count ; index1++ )
					{		
						T new_item( "obj" , __file__ , __line__ );
						new_item = *range_ptr[ index1 - index ];			
						XGCRef* gcref = new_item.get_GCRef( 1 , 0 );
						gcref->AddRef();
						new_array_ptr[ index1 ] = gcref;
					}
					if( _array_ptr )
						::rux::engine::free_mem( _array_ptr );
					_array_ptr = new_array_ptr;
				}	
				else
				{
					for( size_t index1 = _count - 1 ; index1 > index + count - 1 ; index1-- )
						_array_ptr[ index1 ] = _array_ptr[ index1 - count ];
					for( size_t index1 = index ; index1 < index + count ; index1++ )
					{				
						T new_item( "obj" , __file__ , __line__ );
						new_item = *range_ptr[ index1 - index ];
						XGCRef* gcref = new_item.get_GCRef( 1 , 0 );
						gcref->AddRef();
						_array_ptr[ index1 ] = gcref;
					}
				}		
			}
		};
		void InsertRange( size_t index , const XGCRef** range_ptr , size_t count )
		{
			InsertRange( index , (XGCRef**)range_ptr , count );
		};
		void RemoveAt( size_t index )
		{
			if( index < _count )
			{
				_array_ptr[ index ]->Release();				
				for( ; index < _count - 1 ; index++ )
					_array_ptr[ index ] = _array_ptr[ index + 1 ];
				_count--;
			}
		};
		void RemoveRange( size_t index , size_t count )
		{
			if( index < _count
				&& count > 0 )
			{
				if( index + count > _count )
					count = _count - index;
				for( size_t index0 = index ; index0 < _count ; index0++ )
				{
					if( index0 < index + count )
						_array_ptr[ index0 ]->Release();
					if( index0 + count < _count )
						_array_ptr[ index0 ] = _array_ptr[ index0 + count ];
				}			
				_count -= count;
			}
		};	
		void Clear()
		{
			RemoveRange( 0 , _count );
		}
		::rux::int32 IndexOf( const T& item )
		{
			return IndexOf( item , 0 );
		};
		::rux::int32 IndexOf( const T& item , size_t index , const char* __file__ = 0 , ::rux::int32 __line__ = 0 )
		{	
			if( __file__ == 0 )
			{
				__file__ = __FILE__;
				__line__ = __LINE__;
			}
			::rux::int32 local_index = -1;	
			for( ; index < _count ; index++ )
			{
				T local_item( *_array_ptr[ index ] , "obj" , __file__ , __line__ );
				if( local_item == item )
				{
					local_index = (::rux::int32)index;
					break;
				}
			}
			return local_index;	
		};
		::rux::int32 LastIndexOf( const T& item )
		{
			return LastIndexOf( item , _count - 1 );
		};
		::rux::int32 LastIndexOf( const T& item , size_t index , const char* __file__ = 0 , ::rux::int32 __line__ = 0 )
		{	
			if( __file__ == 0 )
			{
				__file__ = __FILE__;
				__line__ = __LINE__;
			}
			::rux::int32 local_index = -1;	
			if( _count > 0 )
			{
				if( index >= _count )
					index = _count - 1;		
				for( ; index >= 0 ; index-- )
				{
					T local_item( *_array_ptr[ index ] , "obj" , __file__ , __line__ );
					if( local_item == item )
					{
						local_index = (::rux::int32)index;
						break;
					}
					if( index == 0 )
						break;
				}
			}
			return local_index;	
		};
		void ReplaceAll( const T& item1 , const T& item2 )
		{
			ReplaceAll( item1 , item2 , 0 );
		};
		void ReplaceAll( const T& item1 , const T& item2 , size_t index , const char* __file__ = 0 , ::rux::int32 __line__ = 0 )
		{	
			if( __file__ == 0 )
			{
				__file__ = __FILE__;
				__line__ = __LINE__;
			}
			for( ; index < _count ; index++ )
			{
				T local_item( *_array_ptr[ index ] , "obj" , __file__ , __line__ );
				if( local_item == item1 )
				{
					Insert( index , item2 , __file__ , __line__ );
					RemoveAt( index + 1 );				
				}
			}
		};
		void ReplaceFirst( const T& item1 , const T& item2 )
		{
			ReplaceFirst( item1 , item2 , 0 );
		};
		void ReplaceFirst( const T& item1 , const T& item2 , size_t index , const char* __file__ = 0 , ::rux::int32 __line__ = 0 )
		{	
			if( __file__ == 0 )
			{
				__file__ = __FILE__;
				__line__ = __LINE__;
			}
			for( ; index < _count ; index++ )
			{
				T local_item( *_array_ptr[ index ] , "obj" , __file__ , __line__ );
				if( local_item == item1 )
				{
					Insert( index , item2 , __file__ , __line__ );
					RemoveAt( index + 1 );				
					break;
				}
			}
		};
		void ReplaceLast( const T& item1 , const T& item2 )
		{
			ReplaceLast( item1 , item2 , 0 );
		};
		void ReplaceLast( const T& item1 , const T& item2 , size_t index , const char* __file__ = 0 , ::rux::int32 __line__ = 0 )
		{	
			if( __file__ == 0 )
			{
				__file__ = __FILE__;
				__line__ = __LINE__;
			}
			if( index >= _count )
				index = _count - 1;
			for( ; index >= 0 ; index-- )
			{
				T local_item( *_array_ptr[ index ] , "obj" , __file__ , __line__ );
				if( local_item == item1 )
				{
					Insert( index , item2 , __file__ , __line__ );
					RemoveAt( index + 1 );				
					break;
				}
			}
		};
		void Swap( size_t index1 , size_t index2 )
		{
			if( index1 < _count 
				&& index2 < _count 
				&& index1 != index2 )
			{
				XGCRef* item = _array_ptr[ index1 ];
				_array_ptr[ index1 ] = _array_ptr[ index2 ];
				_array_ptr[ index2 ] = item;
			}
		};
		size_t Count()
		{
			return _count;
		};
		typename T::InternalClass& operator[] ( size_t index )
		{			
			if( T::get_static_RuxTypeIndex() != _array_ptr[ index ]->get_RuxTypeIndex() )
			{
				T obj( *_array_ptr[ index ] , "obj" , __FILE__ , __LINE__ );
				return obj++;
			}
			else
				return *(typename T::InternalClass*)_array_ptr[ index ];
		};
		size_t get_step( void )
		{
			if( _step < RUX_ARRAY_MAX_STEP )
				return _step *= 2;
			else
				return _step;
		};
	};
	template < class T >
	class XArray : public XGCHandle< Array< T > >
	{	
	public:
		static ::rux::uint32 get_static_RuxTypeIndex( void );
		static Type* get_static_Type( rux::uint32* rux_type_index );
		XArray( const char* variable_name = 0 , const char* __file__ = 0 , ::rux::int32 __line__ = 0 )
			: XGCHandle< Array< T > >( variable_name , __file__ , __line__ )
		{
		};
		XArray( const XArray<T>& Array )
		{		
			AddRange( Array );
		};
		XArray( const T& item0 , const T& item1 , const char* __file__ = 0 , ::rux::int32 __line__ = 0 )
		{	
			if( __file__ == 0 )
			{
				__file__ = __FILE__;
				__line__ = __LINE__;
			}
			Add( item0 , __file__ , __line__ );
			Add( item1 , __file__ , __line__ );
		};
		XArray( const T& item0 , const T& item1 , const T& item2 , const char* __file__ = 0 , ::rux::int32 __line__ = 0 )
		{	
			if( __file__ == 0 )
			{
				__file__ = __FILE__;
				__line__ = __LINE__;
			}
			Add( item0 , __file__ , __line__ );
			Add( item1 , __file__ , __line__ );
			Add( item2 , __file__ , __line__ );
		};
		XArray( const Array<T>& array , const char* variable_name = 0 , const char* __file__ = 0 , ::rux::int32 __line__ = 0 )
			: XGCHandle< Array< T > >( variable_name , __file__ , __line__ )
		{		
			AddRange( array );
		};	
		XArray( const XGCRef& gcref , const char* variable_name = 0 , const char* __file__ = 0 , ::rux::int32 __line__ = 0 )
			: XGCHandle< Array< T > >( variable_name , __file__ , __line__ )
		{		
			(*this) = gcref;
		};	
		explicit XArray( const XObject& object , const char* __file__ = 0 , ::rux::int32 __line__ = 0 )
		{	
			if( __file__ == 0 )
			{
				__file__ = __FILE__;
				__line__ = __LINE__;
			}
			if( rux_is_object( object , XArray< T > ) )
				set_ByRef( object.get_GCRef() );
			else if( rux_is_object( object , T ) )
			{
				T item( "obj" , __file__ , __line__ );
				item.set_ByRef( *object.get_GCRef() );
				Add( item , __file__ , __line__ );
			}
		};
		virtual ~XArray()
		{
		};	
		void IncreaseMemory( size_t items_count )
		{
			(*this)()->IncreaseMemory( items_count );
		};
		void AddByRef( const T& item ) const
		{
			(*this)()->AddByRef( item );				
		};
		void AddByRef( const XGCRef& item ) const
		{
			(*this)()->AddByRef( item );
		};
		void AddByRef( const typename T::InternalClass& item ) const
		{
			(*this)()->AddByRef( item );
		};
		void Add( const T& item , const char* __file__ = 0 , ::rux::int32 __line__ = 0 ) const
		{	
			if( __file__ == 0 )
			{
				__file__ = __FILE__;
				__line__ = __LINE__;
			}
			(*this)()->Add( item , __file__ , __line__ );
		};
		void Add( const typename T::InternalClass& item , const char* __file__ = 0 , ::rux::int32 __line__ = 0 ) const
		{	
			if( __file__ == 0 )
			{
				__file__ = __FILE__;
				__line__ = __LINE__;
			}
			(*this)()->Add( item , __file__ , __line__ );
		};
		void Add( const XGCRef& item , const char* __file__ = 0 , ::rux::int32 __line__ = 0 ) const
		{	
			if( __file__ == 0 )
			{
				__file__ = __FILE__;
				__line__ = __LINE__;
			}
			(*this)()->Add( item , __file__ , __line__ );
		};
		void Add( const XGCRef* item , const char* __file__ = 0 , ::rux::int32 __line__ = 0 ) const
		{	
			if( __file__ == 0 )
			{
				__file__ = __FILE__;
				__line__ = __LINE__;
			}
			(*this)()->Add( item , __file__ , __line__ );
		};
		void AddRange( const Array<T>& range )
		{		
			(*this)()->AddRange( range );
		};
		void AddRange( const XArray<T>& range )
		{		
			(*this)()->AddRange( *range() );
		}	
		void Insert( size_t index , const T& item , const char* __file__ = 0 , ::rux::int32 __line__ = 0 )
		{	
			if( __file__ == 0 )
			{
				__file__ = __FILE__;
				__line__ = __LINE__;
			}
			(*this)()->Insert( index , item , __file__ , __line__ );
		};
		void Insert( size_t index , const typename T::InternalClass& item , const char* __file__ = 0 , ::rux::int32 __line__ = 0 )
		{	
			if( __file__ == 0 )
			{
				__file__ = __FILE__;
				__line__ = __LINE__;
			}
			(*this)()->Insert( index , item , __file__ , __line__ );
		};
		void Insert( size_t index , const XGCRef& item , const char* __file__ = 0 , ::rux::int32 __line__ = 0 )
		{	
			if( __file__ == 0 )
			{
				__file__ = __FILE__;
				__line__ = __LINE__;
			}
			(*this)()->Insert( index , item , __file__ , __line__ );
		};
		void InsertRange( size_t index , const Array<T>& range )
		{		
			(*this)()->InsertRange( index , range );
		};
		void InsertRange( size_t index , const XArray<T>& range )
		{	
			(*this)()->InsertRange( index , *range() );
		};	
		void RemoveAt( size_t index )
		{		
			(*this)()->RemoveAt( index );
		};
		void RemoveRange( size_t index , size_t count )
		{		
			(*this)()->RemoveRange( index , count );
		};	
		void Clear() const
		{
			(*this)()->Clear();
		}
		::rux::int32 IndexOf( const T& item )
		{
			return IndexOf( item , 0 );
		};
		::rux::int32 IndexOf( const T& item , size_t index )
		{				
			return (*this)()->IndexOf( item , index );	
		};
		::rux::int32 LastIndexOf( const T& item )
		{
			return LastIndexOf( item , 0 );
		};
		::rux::int32 LastIndexOf( const T& item , size_t index )
		{				
			return (*this)()->LastIndexOf( item , index );	
		};
		void ReplaceAll( const T& item1 , const T& item2 )
		{
			ReplaceAll( item1 , item2 , 0 );
		};
		void ReplaceAll( const T& item1 , const T& item2 , size_t index )
		{		
			(*this)()->ReplaceAll( item1 , item2 , index );
		};
		void ReplaceFirst( const T& item1 , const T& item2 )
		{
			ReplaceFirst( item1 , item2 , 0 );
		};
		void ReplaceFirst( const T& item1 , const T& item2 , size_t index )
		{		
			(*this)()->ReplaceFirst( item1 , item2 , index );
		};
		void ReplaceLast( const T& item1 , const T& item2 )
		{
			ReplaceLast( item1 , item2 , 0 );
		};
		void ReplaceLast( const T& item1 , const T& item2 , size_t index )
		{		
			(*this)()->ReplaceLast( item1 , item2 , index );
		};
		void Swap( size_t index1 , size_t index2 )
		{
			(*this)()->Swap( index1 , index2 );
		};
		size_t Count() const
		{		
			return (*this)()->_count;
		};
		typename T::InternalClass& operator[] ( size_t index ) const
		{				
			return (*const_cast< XArray< T >* >( this ))()->operator[]( index );
		};
		XArray& operator=( const XArray<T>& array )
		{		
			Clear();
			AddRange( array );
			return *this;
		}
		XArray& operator=( const Array<T>& array )
		{		
			Clear();
			AddRange( array );
			return *this;
		}
		void set_Item( size_t index , const T& item )
		{
			(*this)()->set_Item( index , item );
		};
		void set_Item( size_t index , const typename T::InternalClass& item )
		{
			(*this)()->set_Item( index , item );
		};
		void set_ItemByRef( size_t index , const T& item )
		{
			(*this)()->set_ItemByRef( index , item );
		};
		void set_ItemByRef( size_t index , const typename T::InternalClass& item )
		{
			(*this)()->set_ItemByRef( index , item );
		};
		XArray& operator =( const XObject& object )
		{
			set_ByRef( object.get_GCRef() );
			return *this;
		};	
		XArray& operator =( const XGCRef& gcref )
		{
			set_ByRef( &gcref );
			return *this;
		};
		XArray& operator =( const XGCRef* gcref )
		{
			set_ByRef( gcref );
			return *this;
		};
		void set_ByRef( const XObject& object ) const
		{
			set_ByRef( object.get_GCRef() );
		};
		void set_ByRef( const Object& object ) const
		{
			set_ByRef( object.get_GCRefObj() );
		};
		void set_ByRef( const XGCRef* object ) const
		{
			if( object )
			{
				if( XArray< T >::get_static_RuxTypeIndex() == object->get_RuxTypeIndex() )
					*static_cast<const XObject*>( this ) = *object;
				else if( rux_is_object_ptr( object , XArray< T > ) )
				{
					Clear();
					Type* type = &object->get_Type();
					if( type->get_IsArray() == 1 )
					{
						::rux::PropertyInfo* property_info = type->get_PropertyInfo( ::rux::compile::times33_hash< 'C','o','u','n','t' >::value );
						if( property_info )
						{
							::rux::FunctionInfo* function_info = type->get_FunctionInfo( ::rux::compile::times33_hash< 'g','e','t','_','I','t','e','m' >::value );
							if( function_info )
							{
								size_t count = (size_t)::rux::gcref_to_uint64( &property_info->Get( object ) );
								for( size_t index0 = 0 ; index0 < count ; index0++ )
								{
									XGCRef* item = &function_info->InvokeResult( object , ::rux::uint64_to_gcref( index0 ) );
									XGCRef* gcref = item;
									if( item->IsObjectType() )
									{
										gcref = ((Object*)item->get_ObjectAddress())->get_GCRefObj();
										if( gcref == NULL )
											gcref = item;
									}
									if( T::get_static_RuxTypeIndex() == XObject::get_static_RuxTypeIndex() 
										|| T::get_static_RuxTypeIndex() == gcref->get_RuxTypeIndex() )
										Add( gcref );
								}
							}
						}
					}
				}
				else
					Clear();
			}
			else
				Clear();
		};
		void set_ByRef( const XGCRef& object ) const
		{
			set_ByRef( &object );
		};
	};
};
#endif
