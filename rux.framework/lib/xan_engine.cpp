#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_gc_defines.h"
#include "xan_loader_defines.h"
#include "xan_gui_defines.h"
#include "xan_io_defines.h"
#include "xan_error.h"
#include "xan_memory_helper.h"
#include "xan_thread.h"
#include "xan_log.h"
namespace rux
{
	dll_internal void deinitialize( void )
	{
		CODE_LABELS_INITIALIZE();
		if( _cs_rux_deinitialization_funcs )
			_CS_PTR_LOCK_( _cs_rux_deinitialization_funcs , 0 );	
		for( ::rux::uint8 index0 = 15 ; index0 >= 0 ; index0-- )
		{
			rux_simple_t rux_deinitialization_func = _rux_deinitialization_funcs[ index0 ];
			if( rux_deinitialization_func )
			{				
				_rux_deinitialization_funcs[ index0 ] = NULL;
				if( rux_deinitialization_func != ::rux::engine::deinitialize )
				{
					CODE_LABEL_ANOTHER_CONTEXT( NULL , NULL , 	rux_deinitialization_func );
				}
				else
					::rux::engine::deinitialize();
			}
			if( index0 == 0 )
				break;
		}
		if( _cs_rux_deinitialization_funcs )
			_cs_rux_deinitialization_funcs->UnLock( 0 );	
	};
	namespace engine
	{
		dll_internal rux_volatile _rux_engine_inited = 0;
		dll_internal rux_volatile _loaded_engine = 0;

		dll_internal void* alloc_array_malloc_dummy( size_t size )
		{
			return alloc_array_macros( ::rux::uint8 , size );
		};
		dll_internal void free_mem_free_dummy( void* ptr )
		{
			::rux::engine::free_mem( ptr );
		};
		dll_internal void* rux_malloc_dummy( size_t size )
		{
			return _aligned_malloc( size , 32 );
		};
		dll_internal void rux_free_dummy( void* ptr )
		{
#ifdef __WINDOWS__
			_aligned_free( ptr );
#else
			::free( ptr );
#endif
		};
		dll_internal ::rux::uint8 add_stack_var( ::rux::memory::begin_memory_chunk* _begin_memory_chunk , const char* file , ::rux::uint32 line , char* variable_name , const char* type_name )
		{
			if( _rux_is_memory_manager == 0 )
				::rux::engine::load_engine();
			if( ::rux::engine::_globals && ::rux::engine::_globals->_add_stack_var )
			{
				::rux::engine::_globals->_add_stack_var( _begin_memory_chunk , file , line , variable_name , type_name );
				return 1;
			}
			else
				return 0;
		};
		dll_internal void remove_stack_var( ::rux::memory::begin_memory_chunk* _begin_memory_chunk )
		{
			if( ::rux::engine::_globals && ::rux::engine::_globals->_remove_stack_var )
				::rux::engine::_globals->_remove_stack_var( _begin_memory_chunk );
		};
		dll_internal void* memcpy( void* dst , size_t dst_offset , const void* src, size_t size )
		{
			::rux::memory::check_memory( dst , dst_offset , size );
			//::rux::memory::check_memory( const_cast< void* >( src ) , 0 , size );
			void* res = NULL;
			if( dst_offset == 0 )
				res = ::memcpy( dst , src , size );
			else
				res = ::memcpy( &((::rux::byte*)dst)[ dst_offset ] , src , size );
			return res;
		};
		dll_internal void* memmove( void* dst , size_t dst_offset , const void* src , size_t src_offset , size_t size )
		{
			::rux::memory::check_memory( dst , dst_offset , size );
			::rux::memory::check_memory( const_cast< void* >( src ) , src_offset , size );
			void* res = NULL;
			if( dst_offset == 0 && src_offset == 0 )
				res = ::memmove( dst , src , size );
			else
				res = ::memmove( &((::rux::byte*)dst)[ dst_offset ] , &((::rux::byte*)src)[ src_offset ] , size );
			return res;
		};
		dll_internal void add_gc_ref( XGCRef* gc_ref_ptr )
		{	
			::rux::engine::_globals->_rux_add_gc_ref( _rux_current_module_index , gc_ref_ptr );
		};
#ifdef __ANDROID__
		dll_internal void load_engine( char* rux_engine_name , ANativeActivity* activity )
#else
		dll_internal void load_engine( char* rux_engine_name )
#endif
		{
			if( XInterlocked::CompareExchange( &::rux::engine::_loaded_engine , 2 , 0 ) != 1 )
			{					
				CS_PTR_LOCK( _cs_engine );
				if( _engine_init_thread_id != ::rux::threading::XThread::get_CurrentThreadId() )
				{
					_engine_init_thread_id = ::rux::threading::XThread::get_CurrentThreadId();
					if( _rux_is_memory_manager == 1 )
					{
						rux_add_deinitialization_func( ::rux::engine::deinitialize );
						XInterlocked::CompareExchange( &::rux::engine::_loaded_engine , 1 , 2 );
					}
					else
					{
						if( _engine_handle == NULL )
						{
							if( XInterlocked::CompareExchange( &::rux::engine::_loaded_engine , 0 , 0 ) == 2 )
							{
								char module_path[ 2048 ] = {0};
								::rux::diagnostics::module::get_current_module_directory( module_path , 2048 );
								::rux::safe_strncat( module_path , "/" , 2048 );
								char library[ 2048 ] = {0};
								if( rux_engine_name )
									strcpy( library , rux_engine_name );
								else
									strcpy( library , dynamic_library_prefix "rux.engine" dynamic_library_extension );
#ifdef __WINDOWS__
								rux_get_engine_globals_t get_engine_globals = (rux_get_engine_globals_t)dlsym( GetModuleHandleA( library ) , "rux_get_engine_globals" );
								::rux::get_time33_hash_value_t get_time33_hash_value = (::rux::get_time33_hash_value_t)dlsym( GetModuleHandleA( library ) , "rux_get_time33_hash_value" );
#else
								rux_get_engine_globals_t get_engine_globals = (rux_get_engine_globals_t)dlsym( NULL , "rux_get_engine_globals" );
								::rux::get_time33_hash_value_t get_time33_hash_value = (::rux::get_time33_hash_value_t)dlsym( NULL , "rux_get_time33_hash_value" );
#endif
								if( get_engine_globals )
								{
									::rux::engine::_globals = get_engine_globals();
									::rux::engine::_get_time33_hash_value = get_time33_hash_value;
									::rux::engine::_rux_alloc = ::rux::engine::_globals->_alloc;
									::rux::engine::_rux_free = ::rux::engine::_globals->_free;
									_engine_handle = dlopen( ::rux::engine::_globals->_engine_full_filename , RTLD_LAZY );
								}
								else
								{
									char main_lib_path[ 2048 ] = {0};
#ifdef __ANDROID__
									if( activity )
									{
										JNIEnv* env = NULL;
										activity->vm->AttachCurrentThread( &env, NULL );
										jclass android_content_context = env->GetObjectClass( activity->clazz );
										jmethodID midGetPackageName = env->GetMethodID( android_content_context , "getPackageName" , "()Ljava/lang/String;" );
										jstring packageName = (jstring)env->CallObjectMethod( activity->clazz , midGetPackageName );
										const char* appname = env->GetStringUTFChars( packageName, NULL );
										size_t appname_len = 0;
										if( appname && ( appname_len = strlen( appname ) ) > 0 )
										{
											if( sizeof( main_lib_path ) > appname_len )
											{
												strcpy( main_lib_path , "/data/data/" );
												strcat( main_lib_path , appname );
												strcat( main_lib_path , "/lib/" );
											}
										}
										env->ReleaseStringUTFChars( packageName, appname );
										activity->vm->DetachCurrentThread();
									}
#endif
									if( main_lib_path[ 0 ] == 0 )
									{
										::rux::diagnostics::module::get_current_executable_directory( main_lib_path , 2048 );
										::rux::safe_strncat( main_lib_path , "/" , 2048 );
									}
#if defined( __UNIX__ )
									const char* dl_error = 0;
#endif
									_engine_handle = dlopen( library , RTLD_LAZY );
									if( _engine_handle == NULL )
									{
#if defined( __UNIX__ )
										dl_error = dlerror();
#endif
										size_t len = strlen( main_lib_path );
										if( len > 0 )
											::memcpy( library , main_lib_path , len );
										size_t name_length = strlen( dynamic_library_prefix "rux.engine" dynamic_library_extension );
										if( rux_engine_name )
										{
											name_length = strlen( rux_engine_name );
											::memcpy( &library[ len ] , rux_engine_name , name_length );
										}
										else
											::memcpy( &library[ len ] , dynamic_library_prefix "rux.engine" dynamic_library_extension , name_length );
										library[ len + name_length ] = 0;
										if( rux_is_exists_file_or_link( library ) )
											_engine_handle = dlopen( library , RTLD_LAZY );
									}			
									if( _engine_handle == NULL )
									{
#if defined( __UNIX__ )
										dl_error = dlerror();
#endif
										size_t len = strlen( module_path );
										if( len > 0 )
											::memcpy( library , module_path , len );
										size_t name_length = strlen( dynamic_library_prefix "rux.engine" dynamic_library_extension );
										if( rux_engine_name )
										{
											name_length = strlen( rux_engine_name );
											::memcpy( &library[ len ] , rux_engine_name , name_length );
										}
										else
											::memcpy( &library[ len ] , dynamic_library_prefix "rux.engine" dynamic_library_extension , name_length );
										library[ len + name_length ] = 0;
										if( rux_is_exists_file_or_link( library ) )
											_engine_handle = dlopen( library , RTLD_LAZY );
									}
									if( _engine_handle == NULL )
									{
#if defined( __UNIX__ )
										dl_error = dlerror();
#endif
										char alter_main_lib_path[ 2048 ] = {0};
										::memcpy( alter_main_lib_path , main_lib_path , 2048 );
										size_t len = strlen( alter_main_lib_path );
										if( len )
										{
											alter_main_lib_path[ len - 1 ] = 0;
											::rux::int64 last_index = ::rux::string::last_index_of( alter_main_lib_path , "/" );
											if( last_index != -1 )
												alter_main_lib_path[ last_index + 1 ] = 0;
											len = strlen( alter_main_lib_path );
											if( len > 0 )
												::memcpy( library , alter_main_lib_path , len );
											size_t name_length = strlen( dynamic_library_prefix "rux.engine" dynamic_library_extension );
											if( rux_engine_name )
											{
												name_length = strlen( rux_engine_name );
												::memcpy( &library[ len ] , rux_engine_name , name_length );
											}
											else
												::memcpy( &library[ len ] , dynamic_library_prefix "rux.engine" dynamic_library_extension , name_length );
											library[ len + name_length ] = 0;
											if( rux_is_exists_file_or_link( library ) )
												_engine_handle = dlopen( library , RTLD_LAZY );
										}
									}
		#ifdef __WINDOWS__
									if( _engine_handle )
										DisableThreadLibraryCalls( _engine_handle );
		#elif defined( __UNIX__ )
									if( _engine_handle == NULL )
									{
										dl_error = dlerror();
										if( dl_error )
											printf( "cannot load module 'librux.engine.so', error = '%s'\n" , dl_error );
										else
											printf( "cannot load module 'librux.engine.so'\n" );
									}
		#endif
									get_engine_globals = (rux_get_engine_globals_t)dlsym( _engine_handle , "rux_get_engine_globals" );
									get_time33_hash_value = (::rux::get_time33_hash_value_t)dlsym( _engine_handle , "rux_get_time33_hash_value" );
									::rux::engine::_globals = get_engine_globals();
									::rux::engine::_get_time33_hash_value = get_time33_hash_value;
									::rux::engine::_rux_alloc = ::rux::engine::_globals->_alloc;
									::rux::engine::_rux_free = ::rux::engine::_globals->_free;
									::rux::safe_strncpy( ::rux::engine::_globals->_main_lib_path , main_lib_path , 2048 );
									::rux::engine::_globals->_main_lib_path[ 2047 ] = 0;
								}
								if( ::rux::engine::_globals && ::rux::engine::_globals->_rux_memory_init )
								{
									rux_add_deinitialization_func( ::rux::engine::deinitialize );
									_rux_current_module_index = ::rux::engine::_globals->_rux_memory_register_module( g_current_module_name );
									XInterlocked::CompareExchange( &::rux::engine::_loaded_engine , 1 , 2 );
								}
								else
									XInterlocked::CompareExchange( &::rux::engine::_loaded_engine , 0 , 2 );
							}					
						}						
					}
					_engine_init_thread_id = 0;
				}
				_cs_engine->UnLock();
			}
		};
#ifdef __ANDROID__
		dll_internal void initialize( char* rux_engine_name , ANativeActivity* activity )
		{
			load_engine( rux_engine_name , activity );
#else
		dll_internal void initialize( char* rux_engine_name )
		{
			load_engine( rux_engine_name );
#endif			
			if( XInterlocked::CompareExchange( &_rux_engine_inited , 2 , 0 ) != 1 )
			{
				CS_PTR_LOCK( _cs_engine );
				if( _engine_init_thread_id != ::rux::threading::XThread::get_CurrentThreadId() )
				{
					_engine_init_thread_id = ::rux::threading::XThread::get_CurrentThreadId();
					if( _rux_is_memory_manager == 1 )
						XInterlocked::CompareExchange( &_rux_engine_inited , 1 , 2 );
					else
					{
						if( _engine_handle )
						{
							if( XInterlocked::CompareExchange( &_rux_engine_inited , 0 , 0 ) == 2 )
							{
								if( ::rux::engine::_globals && ::rux::engine::_globals->_rux_memory_init )
								{
									if( ::rux::engine::_on_thread_registered )
									{
										::rux::engine::_globals->add_thread_registered_event( ::rux::engine::_on_thread_registered );
										::rux::engine::_on_thread_registered = 0;
									}
									if( ::rux::engine::_on_thread_unregistered )
									{
										::rux::engine::_globals->add_thread_unregistered_event( ::rux::engine::_on_thread_unregistered );
										::rux::engine::_on_thread_unregistered = 0;
									}
									::rux::engine::_globals->_rux_memory_init( NULL );
									XInterlocked::CompareExchange( &_rux_engine_inited , 1 , 2 );
								}
								else
									XInterlocked::CompareExchange( &_rux_engine_inited , 0 , 2 );
							}
						}
					}
					_engine_init_thread_id = 0;
				}
				_cs_engine->UnLock();
			}
		};
		dll_internal  void deinitialize( void )
		{	
			CODE_LABELS_INITIALIZE();
			if( XInterlocked::CompareExchange( &_rux_engine_inited , 2 , 1 ) == 1 )
			{
				if( _rux_is_memory_manager == 0 )
				{
					_CS_PTR_LOCK_( _cs_engine , 0 );
					if( _engine_handle )
					{
						::rux::engine::_globals->_loader_globals->_rux_loader_unload_loadable_module( _rux_current_module_index );
						CODE_LABEL_ANOTHER_CONTEXT_PARAMETER( NULL , "rux_memory_unload_module" , ::rux::engine::_globals->_rux_memory_unload_module , _rux_current_module_index );
						::rux::engine::_globals->_rux_memory_free();
						dlclose( _engine_handle );
						_engine_handle = NULL;							
					}
					::rux::engine::_globals = NULL;
					XInterlocked::CompareExchange( &_rux_engine_inited , 0 , 2 );
					_cs_engine->UnLock( 0 );
				}
				else
					XInterlocked::CompareExchange( &_rux_engine_inited , 0 , 2 );
			}
		};
	};
	namespace loader
	{
		Module& LoadModule( const ::rux::XString& module_name , void* init_param , ::rux::XString& error , ::rux::int32 loadable_module_index )
		{
			error.Clear();		
			return ::rux::engine::_globals->_loader_globals->_rux_load_module( module_name , init_param , error , loadable_module_index );
		};
		void UnLoadModule( const Module &module , ::rux::int32 loadable_module_index )
		{
			if( module._loaded_modules_index != 0xffffffff )
				::rux::engine::_globals->_loader_globals->_rux_unload_module( module , loadable_module_index );
		};
		void UnLoadModule( const XModule &module , ::rux::int32 loadable_module_index )
		{
			if( module()->_loaded_modules_index != 0xffffffff )
				::rux::engine::_globals->_loader_globals->_rux_unload_module( module , loadable_module_index );
		};
		void UnLoadModule( const ::rux::XString& module_name , ::rux::int32 loadable_module_index )
		{	
			::rux::engine::_globals->_loader_globals->_rux_unload_module_by_name( module_name , loadable_module_index );
		};
		Array< ::rux::XString >& get_LoadedModules( void )
		{
			XArray< XString > loaded_modules;
			loaded_modules.set_ByRef( ::rux::engine::_globals->_loader_globals->_rux_get_loaded_modules() );
			return loaded_modules++;
		};
		void AddExcludeDirectory( const ::rux::XString& directory )
		{
			::rux::engine::_globals->_loader_globals->_rux_add_exclude_directory( directory );
		};
	};
};
#ifdef __WINDOWS__
#pragma warning( push )
#pragma warning( disable: 4075 )
#pragma init_seg( ".CRT$XCB" )
#pragma warning( pop )
dll_internal ::rux::global_initialization _global_initialization;
#endif