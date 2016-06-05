#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_modulemanager.h>
#include <xan_keyvaluepair.h>
#include <xan_io_defines.h>
#include <xan_uint64.h>
dll_internal ModuleManager* g_module_manager = NULL;
ModuleManager::ModuleManager( void )
{
	for( rux::uint32 index0 = 0 ; index0 < 128 ; index0++ )
		_loaded_modules_by_loadable_module[ index0 ] = 0;
};
ModuleManager::~ModuleManager()
{
	CRITICAL_SECTION_LOCK( _cs_loaded_modules );
	if( _loaded_modules.ItemsCount() > 0 )
		printf( "ERROR: loaded modules items count = %u\n" , (::rux::uint32)_loaded_modules.ItemsCount() );		
	CRITICAL_SECTION_UNLOCK( _cs_loaded_modules );
};
void ModuleManager::AddExcludeDirectory( const rux::XString& directory )
{
	if( directory.Length() > 0 )
	{
		declare_variable( rux::XString , local_directory );
		local_directory.set_ByRef( directory.ReplaceAll( '\\' , '/' ) );
		rux::uint8 is_relative = 1;
#ifdef __WINDOWS__
		rux::int32 index0 = local_directory.IndexOf( '/' );
		if( index0 > 0 )
			is_relative = local_directory.get_UTF8Char( index0 - 1 ) == ':' ? 0 : 1;
#elif defined( __UNIX__ )
		is_relative = local_directory.get_UTF8Char( 0 ) == '/' ? 0 : 1;
#endif
		if( is_relative == 1 )
		{
			declare_variable( rux::XString , error );
			rux::XModule module( rux::XModule::get_Executable( error ) );
			if( error.Length() == 0 )
				local_directory.set_ByRef( module.get_Directory() + "/" + local_directory );
			else
				local_directory.Clear();
		}
		if( local_directory.Length() > 0 )
		{
			CRITICAL_SECTION_LOCK( g_module_manager->_cs_search_directories );
			g_module_manager->_exclude_directories.Add( local_directory );				
			rux::XArray< rux::XString > directories;
			directories.set_ByRef( rux::io::GetAllDirectories( local_directory ) );
			for( rux::uint32 index1 = 0 ; index1 < directories.Count() ; index1++ )			
				g_module_manager->_exclude_directories.Add( directories[ index1 ] );	
			CRITICAL_SECTION_UNLOCK( g_module_manager->_cs_search_directories );
		}
	}
};
void ModuleManager::update_search_path( void )
{
	CRITICAL_SECTION_LOCK( g_module_manager->_cs_search_directories );	
	if( g_module_manager->_search_directories.Count() == 0 )
	{
		declare_variable( rux::XString , string );
		rux::XModule module( rux::XModule::get_Executable( string ) );
		if( string.Length() == 0 )
		{				
			string.set_ByRef( module.get_Directory() );
			g_module_manager->_search_directories.AddByRef( string );
			declare_stack_variable( char , module_directory , 2048 );
			::rux::diagnostics::module::get_current_module_directory( module_directory , 2048 );
			g_module_manager->_search_directories.AddByRef( ::rux::XString( module_directory ) );
			rux::XArray< rux::XString> directories;
			directories.set_ByRef( rux::io::GetAllDirectories( string ) );					
			for( size_t index0 = 0 ; index0 < directories.Count() ; index0++ )			
				g_module_manager->_search_directories.Add( directories[ index0 ] );
			rux::uint32 count = 0;
			char search_directories[ 16 ][ 512 ] = { 0 , 0 };
			rux_get_search_directories( count , search_directories );
			for( size_t index0 = 0 ; index0 < count ; index0++ )			
				g_module_manager->_search_directories.Add( ::rux::XString( search_directories[ index0 ] ) );
			for( size_t index0 = 0 ; index0 < ::rux::engine::_globals->_env_variables_count ; index0++ )
			{
				::rux::XString string0( ::rux::engine::_globals->_env_variables[ index0 ] );
				string0.set_ByRef( string0.ReplaceAll( '\\' , '/' ) );
				string0.Shrink();
				g_module_manager->_search_directories.AddByRef( string0 );
			}		
		}
	}
	CRITICAL_SECTION_UNLOCK( g_module_manager->_cs_search_directories );	
};
rux::Module& ModuleManager::LoadModule( const rux::XString& module_name , void* init_param , rux::XString& error , rux::int32 loadable_module_index )
{
	CODE_LABELS_INITIALIZE();
	error.Clear();
	rux::uint8 is_exists = 0 , is_search = 0 , is_load = 0;
	rux::XModule module;
	declare_stack_variable( char , _filename_ptr , 2048 );
	declare_stack_variable( char , local_path_ptr , 2048 );
	module_name.CopyToArrayAsCurrentCodePage( _filename_ptr , 2048 );
	rux_replace( _filename_ptr , '\\' , '/' );
	is_search = rux_index_of( _filename_ptr , '/' ) == -1 ? 1 : 0;
	if( is_search == 1 )
	{
		update_search_path();
		rux::int32 index2 = rux_index_of( _filename_ptr , dynamic_library_extension );
		if( index2 >= 0 )
			_filename_ptr[ index2 ] = '\0';
		size_t length = strlen( _filename_ptr );
		index2 = rux_index_of( _filename_ptr , "lib" );
		if( index2 == 0 )
			rux_substring( _filename_ptr , _filename_ptr , 3 , length - 3 );			
	}
	else
	{
		declare_stack_variable( char , filename_only , 1024 );
		rux_get_executable_filename_only( _filename_ptr , filename_only );
		size_t length = strlen( filename_only );
		memcpy( _filename_ptr , filename_only , length + 1 );
		rux::int32 index2 = rux_index_of( _filename_ptr , dynamic_library_extension );
		if( index2 >= 0 )
			_filename_ptr[ index2 ] = '\0';
		length = strlen( _filename_ptr );
		index2 = rux_index_of( _filename_ptr , "lib" );
		if( index2 == 0 )
		{
			rux_substring( filename_only , _filename_ptr , 3 , length - 3 );		
			length = strlen( filename_only );
			memcpy( _filename_ptr , filename_only , length + 1 );	
		}
	}
	declare_stack_variable( char , _filename_ptr0 , 2048 );
	declare_stack_variable( char , _filename_ptr1 , 2048 );
	declare_stack_variable( char , _filename_ptr2 , 2048 );
	declare_variable( rux::XString ,filename3 );
	rux_concatenation( _filename_ptr0 , "lib" , _filename_ptr );
	rux_concatenation( _filename_ptr1 , _filename_ptr , dynamic_library_extension );
	rux_concatenation( _filename_ptr2 , _filename_ptr0 , dynamic_library_extension );
	rux::uint32 filename_ptr_length = strlen( _filename_ptr ) , filename_ptr0_length = strlen( _filename_ptr0 ) , filename_ptr1_length = strlen( _filename_ptr1 ) , filename_ptr2_length = strlen( _filename_ptr2 );
	rux::uint32 index = 0 , index1 = 0;
	rux::uint32 index_to_empty0 = 0 , index_to_empty1 = 0 , index_to_empty2 = 0 , index_to_empty3 = 0;
	do
	{
		is_exists = 0;
		CRITICAL_SECTION_LOCK( g_module_manager->_cs_busy_modules );
		if( g_module_manager->_busy_modules.ItemsCount() > 0 )
		{
			for( index = 0 ; index < g_module_manager->_busy_modules.Count() ; index++ )
			{
				if( g_module_manager->_busy_modules[ index ] )
				{
					if( strcmp( _filename_ptr , g_module_manager->_busy_modules[ index ] ) == 0 
						|| strcmp( _filename_ptr0 , g_module_manager->_busy_modules[ index ] ) == 0 
						|| strcmp( _filename_ptr1 , g_module_manager->_busy_modules[ index ] ) == 0 
						|| strcmp( _filename_ptr2 , g_module_manager->_busy_modules[ index ] ) == 0 )
					{
						is_exists = 1;
						break;
					}
				}
			}
		}
		if( is_exists == 0 )
		{
			index_to_empty0 = g_module_manager->_busy_modules.FillEmptyOrAdd( _filename_ptr );
			index_to_empty1 = g_module_manager->_busy_modules.FillEmptyOrAdd( _filename_ptr0 );
			index_to_empty2 = g_module_manager->_busy_modules.FillEmptyOrAdd( _filename_ptr1 );
			index_to_empty3 = g_module_manager->_busy_modules.FillEmptyOrAdd( _filename_ptr2 );
		}
		CRITICAL_SECTION_UNLOCK( g_module_manager->_cs_busy_modules );
		if( is_exists == 1 )
			rux::threading::XThread::Sleep( 1 );
	}
	while( is_exists == 1 );
	is_exists = 0;		
	declare_stack_variable( char , filename_ptr0 , 2048 );
	declare_stack_variable( char , filename_ptr , 2048 );
	declare_stack_variable( char , filename_ptr1 , 2048 );
	declare_stack_variable( char , filename_ptr2 , 2048 );
	if( is_search == 0 )
	{
		module_name.CopyToArrayAsCurrentCodePage( filename_ptr , 2048 );
		rux_replace( filename_ptr , '\\' , '/' );
		rux_concatenation( filename_ptr1 , filename_ptr , dynamic_library_extension );			
		declare_stack_variable( char , directory_only , 1024 );
		declare_stack_variable( char , filename_only , 1024 );
		rux_get_directory_only( filename_ptr , directory_only );
		rux_get_executable_filename_only( filename_ptr , filename_only );
		rux_concatenation( filename_ptr0 , directory_only , "/lib" );
		rux_concatenation( filename_ptr0 , filename_only );
		rux_concatenation( filename_ptr2 , filename_ptr0 , dynamic_library_extension );
		filename_ptr_length = strlen( filename_ptr ) , filename_ptr0_length = strlen( filename_ptr0 ) , filename_ptr1_length = strlen( filename_ptr1 ) , filename_ptr2_length = strlen( filename_ptr2 );
	}
	else
	{
		if( filename_ptr_length > 0 )
			memcpy( filename_ptr , _filename_ptr , filename_ptr_length + 1 );
		if( filename_ptr0_length > 0 )
			memcpy( filename_ptr0 , _filename_ptr0 , filename_ptr0_length + 1 );
		if( filename_ptr1_length > 0 )
			memcpy( filename_ptr1 , _filename_ptr1 , filename_ptr1_length + 1 );
		if( filename_ptr2_length > 0 )
			memcpy( filename_ptr2 , _filename_ptr2 , filename_ptr2_length + 1 );
	}
	CRITICAL_SECTION_LOCK( g_module_manager->_cs_loaded_modules );						
	for( index = 0 ; index < g_module_manager->_loaded_modules.Count() ; index++ )
	{				
		if( g_module_manager->_loaded_modules[ index ] )
		{
			if( is_search == 1 )
				filename3.set_ByRef( g_module_manager->_loaded_modules[ index ]->get_FileNameWithoutExtension() );
			else
				filename3.set_ByRef( g_module_manager->_loaded_modules[ index ]->_filename );
			if( filename3.Equals( filename_ptr , filename_ptr_length ) 
				|| filename3.Equals( filename_ptr0 , filename_ptr0_length ) 
				|| filename3.Equals( filename_ptr1 , filename_ptr1_length )
				|| filename3.Equals( filename_ptr2 , filename_ptr2_length ) )
			{					
				module = *( g_module_manager->_loaded_modules[ index ] );
				module.private_AddRef( loadable_module_index );
				is_exists = 1;
				break;					
			}				
		}
	}		
	CRITICAL_SECTION_UNLOCK( g_module_manager->_cs_loaded_modules );
	module_name.CopyToArrayAsCurrentCodePage( filename_ptr , 2048 );
	rux_replace( filename_ptr , '\\' , '/' );
	if( is_exists == 0 )
	{
#ifdef __WINDOWS__
		HMODULE module_handle = NULL;
#endif
#ifdef __UNIX__
		void* module_handle = NULL;
		const char* dl_error = 0;
#endif
		if( is_search == 1 )
		{						
			for( index = 0 ; index < g_module_manager->_search_directories.Count() ; index++ )
			{
				CRITICAL_SECTION_LOCK( g_module_manager->_cs_search_directories );
				for( index1 = 0 ; index1 < g_module_manager->_exclude_directories.Count() ; index1++ )
				{
					if( g_module_manager->_exclude_directories[ index1 ] == g_module_manager->_search_directories[ index ] )
						break;
				}
				if( index1 <  g_module_manager->_exclude_directories.Count() )
				{
					CRITICAL_SECTION_UNLOCK( g_module_manager->_cs_search_directories );
					continue;
				}
				else
					CRITICAL_SECTION_UNLOCK( g_module_manager->_cs_search_directories );
				g_module_manager->_search_directories[ index ].CopyToArrayAsCurrentCodePage( local_path_ptr , 2048 );
				rux_concatenation( local_path_ptr , "/" );
				rux_concatenation( local_path_ptr , filename_ptr );
				if( rux_index_of( local_path_ptr , dynamic_library_extension ) != strlen( local_path_ptr ) - strlen( dynamic_library_extension ) )
					rux_concatenation( local_path_ptr , dynamic_library_extension );
				if( rux_is_exists_file( local_path_ptr ) == 1 )
				{
#ifdef __WINDOWS__				
					::rux::engine::_globals->_on_before_load_module.raise< ::rux::pid_t , ::rux::threading::Thread* , size_t >( 0 , 0 , 0 );
					CODE_LABEL_ANOTHER_CONTEXT_PARAMETER_RESULT( local_path_ptr , "LoadLibraryA" , LoadLibraryA , local_path_ptr , module_handle );
					::rux::engine::_globals->_on_after_load_module.raise< ::rux::pid_t , ::rux::threading::Thread* , size_t >( 0 , 0 , 0 );
					is_load = 1;
					if( module_handle != NULL )
					{
						module()->_filename = local_path_ptr;
						break;
					}						
#endif
#ifdef __UNIX__					
					::rux::engine::_globals->_on_before_load_module.raise< ::rux::pid_t , ::rux::threading::Thread* , size_t >( 0 , 0 , 0 );
					CODE_LABEL_ANOTHER_CONTEXT_2_PARAMETERS_RESULT( local_path_ptr , "dlopen" , dlopen , local_path_ptr , RTLD_LAZY , module_handle ); 
					::rux::engine::_globals->_on_after_load_module.raise< ::rux::pid_t , ::rux::threading::Thread* , size_t >( 0 , 0 , 0 );
					is_load = 1;
					if( module_handle != NULL )
					{
						module()->_filename = local_path_ptr;
						break;
					}
					else
						dl_error = dlerror();
#endif
				}
			}
			if( module_handle == NULL)
			{
				for( index = 0 ; index < g_module_manager->_search_directories.Count() ; index++ )
				{
					CRITICAL_SECTION_LOCK( g_module_manager->_cs_search_directories );
					for( index1 = 0 ; index1 < g_module_manager->_exclude_directories.Count() ; index1++ )
					{
						if( g_module_manager->_exclude_directories[ index1 ] == g_module_manager->_search_directories[ index ] )
							break;
					}
					if( index1 <  g_module_manager->_exclude_directories.Count() )
					{
						CRITICAL_SECTION_UNLOCK( g_module_manager->_cs_search_directories );
						continue;
					}
					else
						CRITICAL_SECTION_UNLOCK( g_module_manager->_cs_search_directories );
					g_module_manager->_search_directories[ index ].CopyToArrayAsCurrentCodePage( local_path_ptr , 2048 );
					rux_concatenation( local_path_ptr , "/lib" );
					rux_concatenation( local_path_ptr , filename_ptr );							
#ifdef __WINDOWS__				
					if( rux_index_of( local_path_ptr , ".dll" ) != strlen( local_path_ptr ) - 4 )
						rux_concatenation( local_path_ptr , ".dll" );
					if( rux_is_exists_file( local_path_ptr ) == 1 )
					{							
						::rux::engine::_globals->_on_before_load_module.raise< ::rux::pid_t , ::rux::threading::Thread* , size_t >( 0 , 0 , 0 );
						CODE_LABEL_ANOTHER_CONTEXT_PARAMETER_RESULT( local_path_ptr , "LoadLibraryA" , LoadLibraryA , local_path_ptr , module_handle );
						::rux::engine::_globals->_on_after_load_module.raise< ::rux::pid_t , ::rux::threading::Thread* , size_t >( 0 , 0 , 0 );
						is_load = 1;
						if( module_handle != NULL )
						{
							module()->_filename = local_path_ptr;
							break;
						}						
					}
#endif
#ifdef __UNIX__
					if( rux_index_of( local_path_ptr , ".so" ) != strlen( local_path_ptr ) - 3 )
						rux_concatenation( local_path_ptr , ".so" );
					if( rux_is_exists_file( local_path_ptr ) == 1 )
					{						
						::rux::engine::_globals->_on_before_load_module.raise< ::rux::pid_t , ::rux::threading::Thread* , size_t >( 0 , 0 , 0 );
						CODE_LABEL_ANOTHER_CONTEXT_2_PARAMETERS_RESULT( local_path_ptr , "dlopen" , dlopen , local_path_ptr , RTLD_LAZY , module_handle );
						::rux::engine::_globals->_on_after_load_module.raise< ::rux::pid_t , ::rux::threading::Thread* , size_t >( 0 , 0 , 0 );
						is_load = 1;
						if( module_handle != NULL )
						{
							module()->_filename = local_path_ptr;
							break;
						}
						else
							dl_error = dlerror();
					}
#endif
				}
			}
		}
		else
		{
#ifdef __WINDOWS__
			if( rux_index_of( filename_ptr , ".dll" ) != strlen( filename_ptr ) - 4 )
				rux_concatenation( filename_ptr , ".dll" );
			if( rux_is_exists_file( filename_ptr ) == 1 )
			{
				::rux::engine::_globals->_on_before_load_module.raise< ::rux::pid_t , ::rux::threading::Thread* , size_t >( 0 , 0 , 0 );
				CODE_LABEL_ANOTHER_CONTEXT_PARAMETER_RESULT( filename_ptr , "LoadLibraryA" , LoadLibraryA , filename_ptr , module_handle );
				::rux::engine::_globals->_on_after_load_module.raise< ::rux::pid_t , ::rux::threading::Thread* , size_t >( 0 , 0 , 0 );
				is_load = 1;
				module()->_filename = filename_ptr;				
			}
			else
			{
				declare_stack_variable( char , directory_only , 1024 );
				declare_stack_variable( char , filename_only , 1024 );
				rux_get_directory_only( filename_ptr , directory_only );
				rux_get_executable_filename_only( filename_ptr , filename_only );
				rux_concatenation( filename_ptr , directory_only , "/lib" );
				rux_concatenation( filename_ptr , filename_only );
				::rux::engine::_globals->_on_before_load_module.raise< ::rux::pid_t , ::rux::threading::Thread* , size_t >( 0 , 0 , 0 );
				CODE_LABEL_ANOTHER_CONTEXT_PARAMETER_RESULT( filename_ptr , "LoadLibraryA" , LoadLibraryA , filename_ptr , module_handle );
				::rux::engine::_globals->_on_after_load_module.raise< ::rux::pid_t , ::rux::threading::Thread* , size_t >( 0 , 0 , 0 );
				is_load = 1;
				module()->_filename = filename_ptr;
			}
#endif
#ifdef __UNIX__
			if( rux_index_of( filename_ptr , ".so" ) != strlen( filename_ptr ) - 3 )
				rux_concatenation( filename_ptr , ".so" );
			if( rux_is_exists_file( filename_ptr ) == 1 )
			{
				::rux::engine::_globals->_on_before_load_module.raise< ::rux::pid_t , ::rux::threading::Thread* , size_t >( 0 , 0 , 0 );
				CODE_LABEL_ANOTHER_CONTEXT_2_PARAMETERS_RESULT( filename_ptr , "dlopen" , dlopen , filename_ptr , RTLD_LAZY , module_handle );
				::rux::engine::_globals->_on_after_load_module.raise< ::rux::pid_t , ::rux::threading::Thread* , size_t >( 0 , 0 , 0 );
				is_load = 1;
				module()->_filename = filename_ptr;
				if( module_handle == 0 )
					dl_error = dlerror();
			}
			else
			{
				declare_stack_variable( char , directory_only , 1024 );
				declare_stack_variable( char , filename_only , 1024 );
				rux_get_directory_only( filename_ptr , directory_only );
				rux_get_executable_filename_only( filename_ptr , filename_only );
				rux_concatenation( filename_ptr , directory_only , "/lib" );
				rux_concatenation( filename_ptr , filename_only );
				::rux::engine::_globals->_on_before_load_module.raise< ::rux::pid_t , ::rux::threading::Thread* , size_t >( 0 , 0 , 0 );
				CODE_LABEL_ANOTHER_CONTEXT_2_PARAMETERS_RESULT( filename_ptr , "dlopen" , dlopen , filename_ptr , RTLD_LAZY , module_handle );
				::rux::engine::_globals->_on_after_load_module.raise< ::rux::pid_t , ::rux::threading::Thread* , size_t >( 0 , 0 , 0 );
				is_load = 1;
				module()->_filename = filename_ptr;
				if( module_handle == 0 )
					dl_error = dlerror();
			}
#endif
		}
		if( module_handle )
		{									
			module()->_module_free = (::rux::rux_module_free_t)dlsym( module_handle , "rux_module_free" );
			module()->_rux_module_init = (::rux::rux_module_init_t)dlsym( module_handle , "rux_module_init" );
			if( module()->_exception_filter == NULL )
			{
				module()->_exception_filter = (rux::rux_exception_event_t)dlsym( module_handle , "exception_filter" );
				if( module()->_exception_filter )
					::rux::debug::RegisterExceptionEvent( module()->_filename.str() , module()->_exception_filter );
			}
			module()->_is_rux_module = (::rux::is_rux_module_t)dlsym( module_handle , "is_rux_module" );
#ifdef __WINDOWS__
			if( module()->_is_rux_module )
				DisableThreadLibraryCalls( module_handle );
#endif
#ifdef __UNIX__
			if( module()->_rux_module_init )
			{
				Dl_info info;
				if( dladdr( (void*)module()->_rux_module_init , &info ) != 0
					&& info.dli_fname && strlen( info.dli_fname ) > 0 )
				{
					declare_stack_variable( char , full_filename0 , 2048 );
					declare_stack_variable( char , filename_only0 , 1024 );
					declare_stack_variable( char , filename_only1 , 1024 );
					module()->_filename.CopyToArrayAsCurrentCodePage( full_filename0 , 2048 );
					rux_get_executable_filename_only( full_filename0 , filename_only0 );
					rux_get_executable_filename_only( info.dli_fname , filename_only1 );
					if( strcmp( filename_only0 , filename_only1 ) != 0 )
						module()->_rux_module_init = NULL;
				}
			}
#endif

			if( module()->_rux_module_init )
			{					
				CRITICAL_SECTION_LOCK( g_module_manager->_cs_loaded_modules );
				module()->AddRef( __FILE__ , __LINE__ );
				module()->_loaded_modules_index = g_module_manager->_loaded_modules.FillEmptyOrAdd( module() );
				CRITICAL_SECTION_UNLOCK( g_module_manager->_cs_loaded_modules );
				::rux::engine::_globals->_on_before_module_dll_init.raise< ::rux::pid_t , ::rux::threading::Thread* , size_t >( 0 , 0 , 0 );
				CODE_LABEL_ANOTHER_CONTEXT_PARAMETER( module()->_filename.str() , "dll_init" , module()->_rux_module_init , init_param );
				::rux::engine::_globals->_on_after_module_dll_init.raise< ::rux::pid_t , ::rux::threading::Thread* , size_t >( 0 , 0 , 0 );
				CRITICAL_SECTION_LOCK( module()->_cs_methods );
				module()->_loadable_module_indexes.Add( loadable_module_index );
				if( loadable_module_index < 128 )
					XInterlocked::Increment( &g_module_manager->_loaded_modules_by_loadable_module[ loadable_module_index ] );
				CRITICAL_SECTION_UNLOCK( module()->_cs_methods );
			}
			else 
			{
				module()->_module_free = (::rux::rux_module_free_t)dlsym( module_handle , "dll_free" );
				if( module()->_exception_filter == NULL )
				{
					module()->_exception_filter = (rux::rux_exception_event_t)dlsym( module_handle , "exception_filter" );
					if( module()->_exception_filter )
						::rux::debug::RegisterExceptionEvent( module()->_filename.str() , module()->_exception_filter );
				}
				module()->_rux_module_init = (::rux::rux_module_init_t)dlsym( module_handle , "dll_init" );						
#ifdef __UNIX__
				if( module()->_rux_module_init )
				{
					Dl_info info;
					if( dladdr( (void*)module()->_rux_module_init , &info ) != 0
						&& info.dli_fname && strlen( info.dli_fname ) > 0 )
					{
						declare_stack_variable( char , full_filename0 , 2048 );
						declare_stack_variable( char , filename_only0 , 1024 );
						declare_stack_variable( char , filename_only1 , 1024 );
						module()->_filename.CopyToArrayAsCurrentCodePage( full_filename0 , 2048 );
						rux_get_executable_filename_only( full_filename0 , filename_only0 );
						rux_get_executable_filename_only( info.dli_fname , filename_only1 );
						if( strcmp( filename_only0 , filename_only1 ) != 0 )
							module()->_rux_module_init = NULL;
					}
				}
#endif
				if( module()->_rux_module_init )
				{						
					CRITICAL_SECTION_LOCK( g_module_manager->_cs_loaded_modules );
					module()->AddRef( __FILE__ , __LINE__ );
					module()->_loaded_modules_index = g_module_manager->_loaded_modules.FillEmptyOrAdd( module() );
					CRITICAL_SECTION_UNLOCK( g_module_manager->_cs_loaded_modules );			
					::rux::engine::_globals->_on_before_module_dll_init.raise< ::rux::pid_t , ::rux::threading::Thread* , size_t >( 0 , 0 , 0 );
					CODE_LABEL_ANOTHER_CONTEXT_PARAMETER( module()->_filename.str() , "dll_init" , module()->_rux_module_init , init_param );
					::rux::engine::_globals->_on_after_module_dll_init.raise< ::rux::pid_t , ::rux::threading::Thread* , size_t >( 0 , 0 , 0 );
					CRITICAL_SECTION_LOCK( module()->_cs_methods );
					module()->_loadable_module_indexes.Add( loadable_module_index );
					if( loadable_module_index < 128 )
						XInterlocked::Increment( &g_module_manager->_loaded_modules_by_loadable_module[ loadable_module_index ] );
					CRITICAL_SECTION_UNLOCK( module()->_cs_methods );
				}
				else 
				{
					module()->_module_free = (::rux::rux_module_free_t)dlsym( module_handle , "core_free" );
					if( module()->_exception_filter == NULL )
					{
						module()->_exception_filter = (rux::rux_exception_event_t)dlsym( module_handle , "exception_filter" );
						if( module()->_exception_filter )
							::rux::debug::RegisterExceptionEvent( module()->_filename.str() , module()->_exception_filter );
					}
					module()->_rux_module_init = (::rux::rux_module_init_t)dlsym( module_handle , "core_init" );
#ifdef __UNIX__
					if( module()->_rux_module_init )
					{
						Dl_info info;
						if( dladdr( (void*)module()->_rux_module_init , &info ) != 0
							&& info.dli_fname && strlen( info.dli_fname ) > 0 )
						{
							declare_stack_variable( char , full_filename0 , 2048 );
							declare_stack_variable( char , filename_only0 , 1024 );
							declare_stack_variable( char , filename_only1 , 1024 );
							module()->_filename.CopyToArrayAsCurrentCodePage( full_filename0 , 2048 );
							rux_get_executable_filename_only( full_filename0 , filename_only0 );
							rux_get_executable_filename_only( info.dli_fname , filename_only1 );
							if( strcmp( filename_only0 , filename_only1 ) != 0 )
								module()->_rux_module_init = NULL;
						}
					}
#endif
					if( module()->_rux_module_init )
					{							
						CRITICAL_SECTION_LOCK( g_module_manager->_cs_loaded_modules );
						module()->AddRef( __FILE__ , __LINE__ );
						module()->_loaded_modules_index = g_module_manager->_loaded_modules.FillEmptyOrAdd( module() );
						CRITICAL_SECTION_UNLOCK( g_module_manager->_cs_loaded_modules );			
						::rux::engine::_globals->_on_before_module_dll_init.raise< ::rux::pid_t , ::rux::threading::Thread* , size_t >( 0 , 0 , 0 );
						CODE_LABEL_ANOTHER_CONTEXT_PARAMETER( module()->_filename.str() , "dll_init" , module()->_rux_module_init , init_param );
						::rux::engine::_globals->_on_after_module_dll_init.raise< ::rux::pid_t , ::rux::threading::Thread* , size_t >( 0 , 0 , 0 );
						CRITICAL_SECTION_LOCK( module()->_cs_methods );
						module()->_loadable_module_indexes.Add( loadable_module_index );
						if( loadable_module_index < 128 )
							XInterlocked::Increment( &g_module_manager->_loaded_modules_by_loadable_module[ loadable_module_index ] );
						CRITICAL_SECTION_UNLOCK( module()->_cs_methods );
					}
				}
			}
			module()->_module_handle = module_handle;
			::rux::log::WriteTrace( "module '%s' is loaded" , module_name.str() );
		}
		else
		{
			if( is_load == 1 )
			{
#ifdef __WINDOWS__				
				rux_write_error(  "cannot load module '" + module_name + "' , " + XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError ) , error );				
#else
				if( dl_error )
				{
					rux_write_error(  "cannot load module '" + module_name + "', " + rux::XString( dl_error ) , error );	
					::rux::log::WriteError( "cannot load module '%s', %s" , module_name.str() , dl_error );
				}
				else
				{
					rux_write_error(  "cannot load module '" + module_name + "'" , error );
					::rux::log::WriteError( "cannot load module '%s'" , module_name.str() );
				}				
#endif
			}
			else
			{
				rux_write_error(  "cannot load module '" + module_name + "' , module does not exist" , error );	
				::rux::log::WriteError( "cannot load module '%s', module does not exist" , module_name.str() );
			}
		}
	}		
	CRITICAL_SECTION_LOCK( g_module_manager->_cs_busy_modules );
	g_module_manager->_busy_modules.set_EmptyAt( index_to_empty0 );
	g_module_manager->_busy_modules.set_EmptyAt( index_to_empty1 );
	g_module_manager->_busy_modules.set_EmptyAt( index_to_empty2 );
	g_module_manager->_busy_modules.set_EmptyAt( index_to_empty3 );
	CRITICAL_SECTION_UNLOCK( g_module_manager->_cs_busy_modules );
	return module++;			
};
void ModuleManager::UnLoadModule(const rux::XModule &module , rux::int32 loadable_module_index )
{
	CODE_LABELS_INITIALIZE();
	if( module()->_loaded_modules_index != 0xffffffff )
	{
#ifdef __WINDOWS__
		HMODULE module_handle = NULL;
#endif
#ifdef __UNIX__
		void* module_handle = NULL;
#endif
		if( g_module_manager )
		{
			rux::uint32 index = 0;
			rux::uint32 index_to_empty0 = 0 , index_to_empty1 = 0;
			rux::uint8 is_exists = 0;		
			declare_variable( rux::XString , filename0 );
			declare_variable( rux::XString , filename1 );
			filename0.set_ByRef( module.get_FileName() );
			filename1.set_ByRef( module.get_FileNameWithoutExtension() );
			do
			{
				is_exists = 0;
				CRITICAL_SECTION_LOCK( g_module_manager->_cs_busy_modules );
				if( g_module_manager->_busy_modules.ItemsCount() > 0 )
				{
					for( index = 0 ; index < g_module_manager->_busy_modules.Count() ; index++ )
					{
						if( g_module_manager->_busy_modules[ index ] )
						{
							if( strcmp( filename0.str() , g_module_manager->_busy_modules[ index ] ) == 0 
								|| strcmp( filename1.str() , g_module_manager->_busy_modules[ index ] ) == 0 )
							{
								is_exists = 1;
								break;
							}
						}
					}
				}
				if( is_exists == 0 )
				{
					index_to_empty0 = g_module_manager->_busy_modules.FillEmptyOrAdd( filename0.str() );
					index_to_empty1 = g_module_manager->_busy_modules.FillEmptyOrAdd( filename1.str() );
				}
				CRITICAL_SECTION_UNLOCK( g_module_manager->_cs_busy_modules );
				if( is_exists == 1 )
					rux::threading::XThread::Sleep( 1 );
			}
			while( is_exists == 1 );
			CRITICAL_SECTION_LOCK( g_module_manager->_cs_loaded_modules );
			if(  g_module_manager->_loaded_modules[ module()->_loaded_modules_index ] == module() )
			{
				module.private_Release( loadable_module_index );
				if( module.private_IsActive() == false )
				{				
					g_module_manager->_loaded_modules.set_EmptyAt( module()->_loaded_modules_index );					
					module()->Release( __FILE__ , __LINE__ );
					CRITICAL_SECTION_UNLOCK( g_module_manager->_cs_loaded_modules );
					if( module()->_module_free )
					{							
						CODE_LABEL_ANOTHER_CONTEXT( module()->_filename.str() , "dll_free" , module()->_module_free );
					}
					if( rux::engine::_globals->_rux_memory_reregister_handler )
						rux::engine::_globals->_rux_memory_reregister_handler();
					if( module()->_exception_filter )
						::rux::debug::UnRegisterExceptionEvent( module()->_exception_filter );
					module_handle = module()->_module_handle;
					module()->_module_handle = NULL;
					CRITICAL_SECTION_LOCK( g_module_manager->_cs_loaded_modules );
				}					
			}
			CRITICAL_SECTION_UNLOCK( g_module_manager->_cs_loaded_modules );
			CRITICAL_SECTION_LOCK( g_module_manager->_cs_busy_modules );
			g_module_manager->_busy_modules.set_EmptyAt( index_to_empty0 );
			g_module_manager->_busy_modules.set_EmptyAt( index_to_empty1 );
			CRITICAL_SECTION_UNLOCK( g_module_manager->_cs_busy_modules );	
		}
		if( module_handle )
#ifdef __WINDOWS__
			FreeLibrary( module_handle );
#endif
#ifdef __UNIX__
#ifndef VALGRIND_DEBUG_INFO
			dlclose( module_handle );
#endif
#endif
	}
	const_cast< rux::XModule* >( &module )->set_ByRef( rux::XModule() );
};
void ModuleManager::UnLoadModule(const rux::Module* module_ptr , rux::int32 loadable_module_index )
{
	CODE_LABELS_INITIALIZE();
	if( module_ptr->_loaded_modules_index != 0xffffffff )
	{
#ifdef __WINDOWS__
		HMODULE module_handle = NULL;
#endif
#ifdef __UNIX__
		void* module_handle = NULL;
#endif
		if( g_module_manager )
		{
			rux::uint32 index = 0;
			rux::uint32 index_to_empty0 = 0 , index_to_empty1 = 0;
			rux::uint8 is_exists = 0;		
			declare_variable( rux::XString , filename0 );
			declare_variable( ::rux::XString , filename1 );
			rux::Module* local_module_ptr = const_cast< rux::Module* >( module_ptr );
			filename0.set_ByRef( local_module_ptr->_filename );
			filename1.set_ByRef( local_module_ptr->get_FileNameWithoutExtension() );
			do
			{
				is_exists = 0;
				CRITICAL_SECTION_LOCK( g_module_manager->_cs_busy_modules );
				if( g_module_manager->_busy_modules.ItemsCount() > 0 )
				{
					for( index = 0 ; index < g_module_manager->_busy_modules.Count() ; index++ )
					{
						if( g_module_manager->_busy_modules[ index ] )
						{
							if( strcmp( filename0.str() , g_module_manager->_busy_modules[ index ] ) == 0 
								|| strcmp( filename1.str() , g_module_manager->_busy_modules[ index ] ) == 0 )
							{
								is_exists = 1;
								break;
							}
						}
					}
				}
				if( is_exists == 0 )
				{
					index_to_empty0 = g_module_manager->_busy_modules.FillEmptyOrAdd( filename0.str() );
					index_to_empty1 = g_module_manager->_busy_modules.FillEmptyOrAdd( filename1.str() );
				}
				CRITICAL_SECTION_UNLOCK( g_module_manager->_cs_busy_modules );
				if( is_exists == 1 )
					rux::threading::XThread::Sleep( 1 );
			}
			while( is_exists == 1 );
			CRITICAL_SECTION_LOCK( g_module_manager->_cs_loaded_modules );
			if(  g_module_manager->_loaded_modules[ module_ptr->_loaded_modules_index ] == module_ptr )
			{
				module_ptr->private_Release( loadable_module_index );
				if( module_ptr->private_IsActive() == false )
				{			
					g_module_manager->_loaded_modules.set_EmptyAt( module_ptr->_loaded_modules_index );					
					module_ptr->Release( __FILE__ , __LINE__ );
					CRITICAL_SECTION_UNLOCK( g_module_manager->_cs_loaded_modules );
					if( module_ptr->_module_free )
					{							
						CODE_LABEL_ANOTHER_CONTEXT( module_ptr->_filename.str() , "dll_free" , module_ptr->_module_free );
					}
					if( rux::engine::_globals->_rux_memory_reregister_handler )
						rux::engine::_globals->_rux_memory_reregister_handler();
					if( module_ptr->_exception_filter )
						::rux::debug::UnRegisterExceptionEvent( module_ptr->_exception_filter );
					module_handle = module_ptr->_module_handle;
					local_module_ptr->_module_handle = NULL;
					CRITICAL_SECTION_LOCK( g_module_manager->_cs_loaded_modules );
				}
			}
			CRITICAL_SECTION_UNLOCK( g_module_manager->_cs_loaded_modules );
			CRITICAL_SECTION_LOCK( g_module_manager->_cs_busy_modules );
			g_module_manager->_busy_modules.set_EmptyAt( index_to_empty0 );
			g_module_manager->_busy_modules.set_EmptyAt( index_to_empty1 );
			CRITICAL_SECTION_UNLOCK( g_module_manager->_cs_busy_modules );
		}
		if( module_handle )
#ifdef __WINDOWS__
			FreeLibrary( module_handle );
#endif
#ifdef __UNIX__
#ifndef VALGRIND_DEBUG_INFO
			dlclose( module_handle );
#endif
#endif
	}
};
void ModuleManager::UnLoadModule(const rux::XString& module_name , rux::int32 loadable_module_index )
{	
	CODE_LABELS_INITIALIZE();
#ifdef __WINDOWS__
	HMODULE module_handle = NULL;
#endif
#ifdef __UNIX__
	void* module_handle = NULL;
#endif
	if( g_module_manager )
	{
		rux::uint8 is_exists = 0 , is_search = 0;
		rux::XModule module;
		declare_stack_variable( char , _filename_ptr , 2048 );
		module_name.CopyToArrayAsCurrentCodePage( _filename_ptr , 2048 );
		rux_replace( _filename_ptr , '\\' , '/' );
		is_search = rux_index_of( _filename_ptr , '/' ) == -1 ? 1 : 0;
		if( is_search == 1 )
		{
			rux::int32 index2 = rux_index_of( _filename_ptr , dynamic_library_extension );
			if( index2 >= 0 )
				_filename_ptr[ index2 ] = '\0';
			size_t length = strlen( _filename_ptr );
			index2 = rux_index_of( _filename_ptr , "lib" );
			if( index2 == 0 )
				rux_substring( _filename_ptr , _filename_ptr , 3 , length - 3 );			
		}
		else
		{
			declare_stack_variable( char , filename_only , 1024 );
			rux_get_executable_filename_only( _filename_ptr , filename_only );
			size_t length = strlen( filename_only );
			memcpy( _filename_ptr , filename_only , length + 1 );
			rux::int32 index2 = rux_index_of( _filename_ptr , dynamic_library_extension );
			if( index2 >= 0 )
				_filename_ptr[ index2 ] = '\0';
			length = strlen( _filename_ptr );
			index2 = rux_index_of( _filename_ptr , "lib" );
			if( index2 == 0 )
			{
				rux_substring( filename_only , _filename_ptr , 3 , length - 3 );		
				length = strlen( filename_only );
				memcpy( _filename_ptr , filename_only , length + 1 );	
			}
		}
		declare_stack_variable( char , _filename_ptr0 , 2048 );
		declare_stack_variable( char , _filename_ptr1 , 2048 );
		declare_stack_variable( char , _filename_ptr2 , 2048 );
		declare_variable( rux::XString , filename3 );
		rux_concatenation( _filename_ptr0 , "lib" , _filename_ptr );
		rux_concatenation( _filename_ptr1 , _filename_ptr , dynamic_library_extension );
		rux_concatenation( _filename_ptr2 , _filename_ptr0 , dynamic_library_extension );
		rux::uint32 filename_ptr_length = strlen( _filename_ptr ) , filename_ptr0_length = strlen( _filename_ptr0 ) , filename_ptr1_length = strlen( _filename_ptr1 ) , filename_ptr2_length = strlen( _filename_ptr2 );		
		rux::uint32 index = 0;
		rux::uint32 index_to_empty0 = 0 , index_to_empty1 = 0 , index_to_empty2 = 0 , index_to_empty3 = 0;
		do
		{
			is_exists = 0;
			CRITICAL_SECTION_LOCK( g_module_manager->_cs_busy_modules );
			for( index = 0 ; index < g_module_manager->_busy_modules.Count() ; index++ )
			{
				if( g_module_manager->_busy_modules[ index ] )
				{
					if( strcmp( _filename_ptr , g_module_manager->_busy_modules[ index ] ) == 0 
						|| strcmp( _filename_ptr0 , g_module_manager->_busy_modules[ index ] ) == 0 
						|| strcmp( _filename_ptr1 , g_module_manager->_busy_modules[ index ] ) == 0 
						|| strcmp( _filename_ptr2 , g_module_manager->_busy_modules[ index ] ) == 0 )
					{
						is_exists = 1;
						break;
					}
				}
			}
			if( is_exists == 0 )
			{
				index_to_empty0 = g_module_manager->_busy_modules.FillEmptyOrAdd( _filename_ptr );
				index_to_empty1 = g_module_manager->_busy_modules.FillEmptyOrAdd( _filename_ptr0 );
				index_to_empty2 = g_module_manager->_busy_modules.FillEmptyOrAdd( _filename_ptr1 );
				index_to_empty3 = g_module_manager->_busy_modules.FillEmptyOrAdd( _filename_ptr2 );
			}
			CRITICAL_SECTION_UNLOCK( g_module_manager->_cs_busy_modules );
			if( is_exists == 1 )
				rux::threading::XThread::Sleep( 1 );
		}
		while( is_exists == 1 );
		is_exists = 0;		
		declare_stack_variable( char , filename_ptr , 2048 );
		declare_stack_variable( char , filename_ptr0 , 2048 );
		declare_stack_variable( char , filename_ptr1 , 2048 );
		declare_stack_variable( char , filename_ptr2 , 2048 );
		if( is_search == 0 )
		{
			module_name.CopyToArrayAsCurrentCodePage( filename_ptr , 2048 );
			rux_replace( filename_ptr , '\\' , '/' );
			rux_concatenation( filename_ptr1 , filename_ptr , dynamic_library_extension );
			module_name.CopyToArrayAsCurrentCodePage( filename_ptr , 2048 );
			rux_replace( filename_ptr , '\\' , '/' );
			rux_concatenation( filename_ptr1 , filename_ptr , dynamic_library_extension );			
			declare_stack_variable( char , directory_only , 1024 );
			declare_stack_variable( char , filename_only , 1024 );
			rux_get_directory_only( filename_ptr , directory_only );
			rux_get_executable_filename_only( filename_ptr , filename_only );
			rux_concatenation( filename_ptr0 , directory_only , "/lib" );
			rux_concatenation( filename_ptr0 , filename_only );
			rux_concatenation( filename_ptr2 , filename_ptr0 , dynamic_library_extension );
			filename_ptr_length = strlen( filename_ptr ) , filename_ptr0_length = strlen( filename_ptr0 ) , filename_ptr1_length = strlen( filename_ptr1 ) , filename_ptr2_length = strlen( filename_ptr2 );
		}
		else
		{
			if( filename_ptr_length > 0 )
				memcpy( filename_ptr , _filename_ptr , filename_ptr_length + 1 );
			if( filename_ptr0_length > 0 )
				memcpy( filename_ptr0 , _filename_ptr0 , filename_ptr0_length + 1 );
			if( filename_ptr1_length > 0 )
				memcpy( filename_ptr1 , _filename_ptr1 , filename_ptr1_length + 1 );
			if( filename_ptr2_length > 0 )
				memcpy( filename_ptr2 , _filename_ptr2 , filename_ptr2_length + 1 );
		}
		CRITICAL_SECTION_LOCK( g_module_manager->_cs_loaded_modules );						
		for( index = 0 ; index < g_module_manager->_loaded_modules.Count() ; index++ )
		{				
			if( g_module_manager->_loaded_modules[ index ] )
			{
				if( is_search == 1 )
					filename3.set_ByRef( g_module_manager->_loaded_modules[ index ]->get_FileNameWithoutExtension() );
				else
					filename3.set_ByRef( g_module_manager->_loaded_modules[ index ]->_filename );
				if( filename3.Equals( filename_ptr , filename_ptr_length ) 
					|| filename3.Equals( filename_ptr0 , filename_ptr0_length ) 
					|| filename3.Equals( filename_ptr1 , filename_ptr1_length )
					|| filename3.Equals( filename_ptr2 , filename_ptr2_length ) )
				{					
					module = *( g_module_manager->_loaded_modules[ index ] );
					is_exists = 1;
					break;					
				}
			}
		}					
		if( is_exists == 1 )
		{
			module.private_Release( loadable_module_index );
			if( module.private_IsActive() == false )
			{				
				g_module_manager->_loaded_modules.set_EmptyAt( index );
				module()->Release( __FILE__ , __LINE__ );
				CRITICAL_SECTION_UNLOCK( g_module_manager->_cs_loaded_modules );
				if( module()->_module_free )
				{						
					CODE_LABEL_ANOTHER_CONTEXT( module()->_filename.str() , "dll_free" , module()->_module_free );
				}
				if( rux::engine::_globals->_rux_memory_reregister_handler )
					rux::engine::_globals->_rux_memory_reregister_handler();
				if( module()->_exception_filter )
					::rux::debug::UnRegisterExceptionEvent( module()->_exception_filter );
				module_handle = module()->_module_handle;
				module()->_module_handle = NULL;		
			}
			else
				CRITICAL_SECTION_UNLOCK( g_module_manager->_cs_loaded_modules );
		}
		else
			CRITICAL_SECTION_UNLOCK( g_module_manager->_cs_loaded_modules );	
		CRITICAL_SECTION_LOCK( g_module_manager->_cs_busy_modules );
		g_module_manager->_busy_modules.set_EmptyAt( index_to_empty0 );
		g_module_manager->_busy_modules.set_EmptyAt( index_to_empty1 );
		g_module_manager->_busy_modules.set_EmptyAt( index_to_empty2 );
		g_module_manager->_busy_modules.set_EmptyAt( index_to_empty3 );
		CRITICAL_SECTION_UNLOCK( g_module_manager->_cs_busy_modules );
	}
	if( module_handle )
#ifdef __WINDOWS__
		FreeLibrary( module_handle );
#endif
#ifdef __UNIX__
#ifndef VALGRIND_DEBUG_INFO
		dlclose( module_handle );
#endif
#endif
};
void ModuleManager::AddModuleMethodInfo( const rux::Module*  module_ptr , const rux::XString& method_name , void* address_ptr )
{
	if( g_module_manager )
	{
		rux::XMethodInfo method_info( method_name , address_ptr );
		const_cast< rux::Module* >( module_ptr )->_methods.Add( method_info );
	}
}
rux::Array< rux::XString >& ModuleManager::get_LoadedModules( void )
{
	rux::XArray< rux::XString > loaded_modules;
	if( g_module_manager )
	{
		CRITICAL_SECTION_LOCK( g_module_manager->_cs_loaded_modules );
		for( rux::uint32 index0 = 0 ; index0 < g_module_manager->_loaded_modules.Count() ; index0++ )
		{
			if( g_module_manager->_loaded_modules[ index0 ] )
				loaded_modules.Add( g_module_manager->_loaded_modules[ index0 ]->_filename );
		}
		CRITICAL_SECTION_UNLOCK( g_module_manager->_cs_loaded_modules );
	}
	return loaded_modules++;
};
void ModuleManager::loader_unload_loadable_module( rux::int32 loadable_module_index )
{
	if( loadable_module_index < 128 )
	{
		rux::Module* module = NULL;
		rux::uint8 exists = 0;
		while( XInterlocked::CompareExchange( &g_module_manager->_loaded_modules_by_loadable_module[ loadable_module_index ] , 0 , 0 ) > 0 )
		{	
			exists = 0;
			CRITICAL_SECTION_LOCK( g_module_manager->_cs_loaded_modules );
			if( g_module_manager->_loaded_modules.Count() > 0 )
			{
				for( rux::uint32 index0 = g_module_manager->_loaded_modules.Count() - 1 ; index0 >= 0  ; index0-- )
				{				
					module = g_module_manager->_loaded_modules[ index0 ];
					if( module )
						module->AddRef( __FILE__ , __LINE__ );
					CRITICAL_SECTION_UNLOCK( g_module_manager->_cs_loaded_modules );
					if( module )
					{					
						CRITICAL_SECTION_LOCK( module->_cs_methods );
						for( rux::uint32 index1 = 0 ; index1 < module->_loadable_module_indexes.Count() ; index1++ )
						{
							if( module->_loadable_module_indexes[ index1 ] == loadable_module_index )
							{
								exists = 1;
								break;
							}
						}
						CRITICAL_SECTION_UNLOCK( module->_cs_methods );
						if( exists == 1 )
							ModuleManager::UnLoadModule( module , loadable_module_index );
						module->Release( __FILE__ , __LINE__ );
					}
					CRITICAL_SECTION_LOCK( g_module_manager->_cs_loaded_modules );
					if( index0 == 0 || exists == 1 )
						break;
				}
			}
			else
			{
				CRITICAL_SECTION_UNLOCK( g_module_manager->_cs_loaded_modules );
				break;
			}
			CRITICAL_SECTION_UNLOCK( g_module_manager->_cs_loaded_modules );
		}
	}
};
