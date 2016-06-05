#ifndef XAN_STD_ALLOCATOR_H
#define XAN_STD_ALLOCATOR_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_includes.h>
namespace rux
{
	namespace memory
	{
		template< typename T >
		class allocator : public ::std::allocator< T >
		{
		public:
			typedef size_t size_type;
			typedef T* pointer;
			typedef const T* const_pointer;
			template< typename _Tp1 >
			struct rebind
			{
					typedef ::rux::memory::allocator< _Tp1 > other;
			};
			pointer allocate( size_type n , const void* hint = 0 )
			{
				return ::rux::engine::alloc_array< T >( n , __FILE__ , __LINE__ , 0 );
			}
			void deallocate( pointer p , size_type n )
			{
				return ::rux::engine::free_mem( p );
			}
			allocator()
				throw(): ::std::allocator<T>() 
			{
			};
			allocator( const allocator& a )
				throw(): ::std::allocator<T>( a )
			{
			};
			template <class U>                    
			allocator( const allocator< U >& a )
				throw(): ::std::allocator<T>( a )
			{
			};
			~allocator() 
				throw() 
			{
			};
		};
	};
};
#endif