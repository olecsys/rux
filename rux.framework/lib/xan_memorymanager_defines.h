#ifndef XAN_MEMORYMANAGER_DEFINES_H
#define XAN_MEMORYMANAGER_DEFINES_H
#include <xan_defines.h>
#include <xan_diagnostic_defines.h>
namespace rux
{
	namespace engine
	{
		dll_internal void* rux_malloc_dummy( size_t size );
		dll_internal void rux_free_dummy( void* ptr );		
	};
};
namespace rux
{
	namespace memory
	{
		class begin_memory_chunk;
		dll_internal size_t get_page_size( void );
	};
};
namespace rux
{
	namespace engine
	{
#ifdef __ANDROID__
		dll_internal void initialize( char* rux_engine_name = NULL , ANativeActivity* activity = NULL );
		dll_internal void load_engine( char* rux_engine_name = NULL , ANativeActivity* activity = NULL );
#else
		dll_internal void initialize( char* rux_engine_name = NULL );
		dll_internal void load_engine( char* rux_engine_name = NULL );
#endif
	};
};
namespace rux
{	
	namespace engine
	{	
#define alloc_object_6_params_macros( object_class , param0 , param1 , param2 , param3 , param4 , param5 ) ::rux::engine::alloc_object< object_class >( param0 , param1 , param2 , param3 , param4 , param5 , (const char*)__FILE__ , (::rux::uint32)__LINE__ , (char*)NULL , (::rux::malloc_t)NULL )
#define alloc_object_5_params_macros( object_class , param0 , param1 , param2 , param3 , param4 ) ::rux::engine::alloc_object< object_class >( param0 , param1 , param2 , param3 , param4 , (const char*)__FILE__ , (::rux::uint32)__LINE__ , (char*)NULL , (::rux::malloc_t)NULL )
#define alloc_object_4_params_macros( object_class , param0 , param1 , param2 , param3 ) ::rux::engine::alloc_object< object_class >( param0 , param1 , param2 , param3 , (const char*)__FILE__ , (::rux::uint32)__LINE__ , (char*)NULL , (::rux::malloc_t)NULL )
#define alloc_object_3_params_macros( object_class , param0 , param1 , param2 ) ::rux::engine::alloc_object< object_class >( param0 , param1 , param2 , (const char*)__FILE__ , (::rux::uint32)__LINE__ , (char*)NULL , (::rux::malloc_t)NULL )
#define alloc_object_2_params_macros( object_class , param0 , param1 ) ::rux::engine::alloc_object< object_class >( param0 , param1 , (const char*)__FILE__ , (::rux::uint32)__LINE__ , (char*)NULL , (::rux::malloc_t)NULL )
#define alloc_object_1_param_macros( object_class , param0 ) ::rux::engine::alloc_object< object_class >( param0 , (const char*)__FILE__ , (::rux::uint32)__LINE__ , (char*)NULL , (::rux::malloc_t)NULL )
#define alloc_object_macros( object_class ) ::rux::engine::alloc_object< object_class >( (const char*)__FILE__ , (::rux::uint32)__LINE__ , (char*)NULL , (::rux::malloc_t)NULL )
		template< class T , class P0 , class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 , class P11 >
		dll_internal T* alloc_object( P0 param0 , P1 param1 , P2 param2 , P3 param3 , P4 param4 , P5 param5 , P6 param6 , P7 param7 , P8 param8 , P9 param9 , P10 param10 , P11 param11 , const char* file = __FILE__ , ::rux::uint32 line = __LINE__ , char* variable_name = NULL , ::rux::malloc_t mallocfn = NULL )
		{	
			if( mallocfn )
				return new ( mallocfn( sizeof( T ) ) )T( param0 , param1 , param2 , param3 , param4 , param5 , param6 , param7 , param8 , param9 , param10 , param11 );			
			else
				return new ( ::rux::engine::_rux_alloc( sizeof( T ) , typeid( T ).name() , file , line , _rux_current_module_index , variable_name ) )T( param0 , param1 , param2 , param3 , param4 , param5 , param6 , param7 , param8 , param9 , param10 , param11 );
		};
		template< class T , class P0 , class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 >
		dll_internal T* alloc_object( P0 param0 , P1 param1 , P2 param2 , P3 param3 , P4 param4 , P5 param5 , P6 param6 , P7 param7 , P8 param8 , P9 param9 , P10 param10 , const char* file = __FILE__ , ::rux::uint32 line = __LINE__ , char* variable_name = NULL , ::rux::malloc_t mallocfn = NULL )
		{	
			if( mallocfn )
				return new ( mallocfn( sizeof( T ) ) )T( param0 , param1 , param2 , param3 , param4 , param5 , param6 , param7 , param8 , param9 , param10 );
			else
				return new ( ::rux::engine::_rux_alloc( sizeof( T ) , typeid( T ).name() , file , line , _rux_current_module_index , variable_name ) )T( param0 , param1 , param2 , param3 , param4 , param5 , param6 , param7 , param8 , param9 , param10 );
		};
		template< class T , class P0 , class P1 , class P2 , class P3 , class P4 , class P5 , class P6 >
		dll_internal T* alloc_object( P0 param0 , P1 param1 , P2 param2 , P3 param3 , P4 param4 , P5 param5 , P6 param6 , const char* file = __FILE__ , ::rux::uint32 line = __LINE__ , char* variable_name = NULL , ::rux::malloc_t mallocfn = NULL )
		{	
			if( mallocfn )
				return new ( mallocfn( sizeof( T ) ) )T( param0 , param1 , param2 , param3 , param4 , param5 , param6 );
			else
				return new ( ::rux::engine::_rux_alloc( sizeof( T ) , typeid( T ).name() , file , line , _rux_current_module_index , variable_name ) )T( param0 , param1 , param2 , param3 , param4 , param5 , param6 );
		};
		template< class T , class P0 , class P1 , class P2 , class P3 , class P4 , class P5 >
		dll_internal T* alloc_object( P0 param0 , P1 param1 , P2 param2 , P3 param3 , P4 param4 , P5 param5 , const char* file = __FILE__ , ::rux::uint32 line = __LINE__ , char* variable_name = NULL , ::rux::malloc_t mallocfn = NULL )
		{	
			if( mallocfn )
				return new ( mallocfn( sizeof( T ) ) )T( param0 , param1 , param2 , param3 , param4 , param5 );
			else
				return new ( ::rux::engine::_rux_alloc( sizeof( T ) , typeid( T ).name() , file , line , _rux_current_module_index , variable_name ) )T( param0 , param1 , param2 , param3 , param4 , param5 );
		};
		template< class T , class P0 , class P1 , class P2 , class P3 , class P4 >
		dll_internal T* alloc_object( P0 param0 , P1 param1 , P2 param2 , P3 param3 , P4 param4 , const char* file = __FILE__ , ::rux::uint32 line = __LINE__ , char* variable_name = NULL , ::rux::malloc_t mallocfn = NULL )
		{	
			if( mallocfn )
				return new ( mallocfn( sizeof( T ) ) )T( param0 , param1 , param2 , param3 , param4 );
			else
				return new ( ::rux::engine::_rux_alloc( sizeof( T ) , typeid( T ).name() , file , line , _rux_current_module_index , variable_name ) )T( param0 , param1 , param2 , param3 , param4 );
		};
		template< class T , class P0 , class P1 , class P2 , class P3 >
		dll_internal T* alloc_object( P0 param0 , P1 param1 , P2 param2 , P3 param3 , const char* file = __FILE__ , ::rux::uint32 line = __LINE__ , char* variable_name = NULL , ::rux::malloc_t mallocfn = NULL )
		{	
			if( mallocfn )
				return new ( mallocfn( sizeof( T ) ) )T( param0 , param1 , param2 , param3 );
			else
				return new ( ::rux::engine::_rux_alloc( sizeof( T ) , typeid( T ).name() , file , line , _rux_current_module_index , variable_name ) )T( param0 , param1 , param2 , param3 );
		};
		template< class T , class P0 , class P1 , class P2 >
		dll_internal T* alloc_object( P0 param0 , P1 param1 , P2 param2 , const char* file = __FILE__ , ::rux::uint32 line = __LINE__ , char* variable_name = NULL , ::rux::malloc_t mallocfn = NULL )
		{	
			if( mallocfn )
				return new ( mallocfn( sizeof( T ) ) )T( param0 , param1 , param2 );
			else
				return new ( ::rux::engine::_rux_alloc( sizeof( T ) , typeid( T ).name() , file , line , _rux_current_module_index , variable_name ) )T( param0 , param1 , param2 );
		};
		template< class T , class P0 , class P1 >
		dll_internal T* alloc_object( P0 param0 , P1 param1 , const char* file = __FILE__ , ::rux::uint32 line = __LINE__ , char* variable_name = NULL , ::rux::malloc_t mallocfn = NULL )
		{	
			if( mallocfn )
				return new ( mallocfn( sizeof( T ) ) )T( param0 , param1 );
			else
				return new ( ::rux::engine::_rux_alloc( sizeof( T ) , typeid( T ).name() , file , line , _rux_current_module_index , variable_name ) )T( param0 , param1 );
		};
		template< class T , class P0 >
		dll_internal T* alloc_object( P0 param0 , const char* file = __FILE__ , ::rux::uint32 line = __LINE__ , char* variable_name = NULL , ::rux::malloc_t mallocfn = NULL )
		{	
			if( mallocfn )
				return new ( mallocfn( sizeof( T ) ) )T( param0 );
			else
				return new ( ::rux::engine::_rux_alloc( sizeof( T ) , typeid( T ).name() , file , line , _rux_current_module_index , variable_name ) )T( param0 );
		};
		template< class T >
		dll_internal T* alloc_object( T& value , const char* file = __FILE__ , ::rux::uint32 line = __LINE__ , char* variable_name = NULL , ::rux::malloc_t mallocfn = NULL )
		{	
			if( mallocfn )
				return new ( mallocfn( sizeof( T ) ) )T( value );
			else
				return new ( ::rux::engine::_rux_alloc( sizeof( T ) , typeid( T ).name() , file , line , _rux_current_module_index , variable_name ) )T( value );
		};
		template< class T >
		dll_internal T* alloc_object( const char* file = __FILE__ , ::rux::uint32 line = __LINE__ , char* variable_name = NULL , ::rux::malloc_t mallocfn = NULL )
		{	
			if( mallocfn )
				return new ( mallocfn( sizeof( T ) ) )T;
			else
				return new ( ::rux::engine::_rux_alloc( sizeof( T ) , typeid( T ).name() , file , line , _rux_current_module_index , variable_name ) )T;
		};
		template< class T >
		dll_internal void free_object( T* object , ::rux::free_t freefn = NULL )
		{
			object->~T();
			if( freefn )
				freefn( object );
			else
				::rux::engine::_rux_free( object );
		};
		template< class T >
		dll_internal T* realloc_array( T* ptr , size_t size , const char* file = __FILE__ , ::rux::uint32 line = __LINE__ , char* variable_name = NULL )
		{			
			if( ptr )
			{
				size_t old_size = ::rux::engine::_globals->_rux_get_memory_size( ptr );
				if( old_size == size )
					return ptr;
				else
				{
					T* new_ptr = static_cast<T*>( ::rux::engine::_rux_alloc( sizeof( T ) * size , typeid( T ).name() , file , line , _rux_current_module_index , variable_name ) );
					if( old_size > size )
						::rux::engine::memcpy( new_ptr , 0 , ptr , size );
					else
						::rux::engine::memcpy( new_ptr , 0 , ptr , old_size );
					::rux::engine::free_mem( ptr );
					return new_ptr;
				}
			}
			else
				return static_cast<T*>( ::rux::engine::_rux_alloc( sizeof( T ) * size , typeid( T ).name() , file , line , _rux_current_module_index , variable_name ) );
		};
	};
};
#endif
