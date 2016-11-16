#include <xan_defines.h>
#include <xan_memorymanager.h>
#include <xan_typemanager.h>
#include <xan_modulemanager.h>
#include <xan_loader_defines.h>
#include <xan_gui_engine.h>
#include <xan_opengl_functions.h>
#include <xan_gui_window.h>
#include <xan_io_defines.h>
#include <xan_constructor_info.h>
#include <xan_property_info.h>
#include <xan_global_initialization.h>
#include <xan_logger.h>
#include <xan_code_performance_counter.h>
#include <xan_gui_defines.h>
#include <xan_service.h>
#include <xan_thread_info.h>
#include <xan_dataobject.h>
#include <xan_memory_cluster.h>
#include <xan_tasker.h>
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#ifdef __UNIX__
#include <mcheck.h>
#endif

#ifdef __WINDOWS__
exports LONG tempexception_filter( _EXCEPTION_POINTERS *ExceptionInfo )
{
	return ::rux::debug::exception_filter( ExceptionInfo );
};
#else
exports void tempexception_filter( ::rux::byte* palarm_is_addr2line , ::rux::int32 signum , siginfo_t* siginfo , void* context )
{
	::rux::engine::_globals->_alarm_is_addr2line = palarm_is_addr2line;
	::rux::debug::debug_posix_death_signal2( signum , siginfo , context );
};
#endif
exports void tempprivate_info_thread(lineid_t _lineid, statement_t _statement, void* udata)
{
	XMemoryManager::private_info_thread(g_rux_memory_manager, SIZE_MAX, _lineid, _statement, udata);
};
exports void temp_set_ruxlog_write( ::rux::log::ruxlog_write_t ruxlog_write )
{
	if( ruxlog_write )
		::rux::log::_ruxlog_write = ruxlog_write;
	else
		::rux::log::_ruxlog_write = ::rux::log::empties::ruxlog_write;
};

exports ::rux::engine::globals* rux_get_engine_globals( void )
{
	return ::rux::engine::_globals;
};
/*begin loader engine*/
exports void rux_write_log( ::rux::int32 module_index , ::rux::log::XEnum_LogLevel level , const char* format , va_list ap )
{
	::rux::log::_ruxlog_write( level , format , ap );
	/*if( ::rux::log::_logger )
		::rux::log::_logger->write_log( module_index , level , format , ap );	*/
};
exports void rux_set_conf_property( const char* conf_property , const char* value )
{
	if( strcmp( conf_property , "diagnostic" ) == 0 )
	{
		if( strcmp( value , "false" ) == 0 )
		{
			rux::engine::_globals->_diagnostic = 0;
			::rux::engine::_diagnostic = 0;
			if( g_rux_memory_manager )
			{
				if( g_rux_memory_manager->_info_task_index != SIZE_MAX )
					::rux::task::remove( g_rux_memory_manager->_info_task_index );
			}
			declare_stack_variable( char , executable_dir , 1024 );
			::rux::safe_strncpy( executable_dir , ::rux::engine::_globals->_executable_directory , 1023 );
			declare_stack_variable( char , dir , 1024 );
			strcpy( dir , executable_dir );
			strcat( dir , "/stable" );
			rux::io::directory::remove_all( dir );
			strcpy( dir , executable_dir );
			strcat( dir , "/cpu" );
			rux::io::directory::remove_all( dir );
			strcpy( dir , executable_dir );
			strcat( dir , "/memory" );
			rux::io::directory::remove_all( dir );
		}
	}
	else if( strcmp( conf_property , "diagnostics_timeout" ) == 0 )
	{
		::rux::uint32 timeout = ::rux::string::to_uint32( value );
		if( timeout > 0 )
			::rux::engine::_diagnostics_timeout = timeout;
	}
	else if( strcmp( conf_property , "heap_corruption_checker" ) == 0 )
	{
		if( strcmp( value , "true" ) == 0 )
			rux::engine::_globals->_heap_corruption_checker = 1;
	}
	else if( strcmp( conf_property , "catch_exception" ) == 0 )
	{
		if( strcmp( value , "false" ) == 0 )
			_rux_catch_exception = 0;
	}
	else if( strcmp( conf_property , "detail_memory_leak" ) == 0 )
	{			
		if( strcmp( value , "true" ) == 0 )
			_rux_is_memory_leaks = 1;
	}
	else if( strcmp( conf_property , "core_dump" ) == 0 )
	{			
		if( strcmp( value , "true" ) == 0 )
			rux::engine::_globals->_is_core_dump = 1;
	}
	else if( strcmp( conf_property , "diagnostics_cpu" ) == 0 )
	{			
		if( strcmp( value , "false" ) == 0 )
			rux::engine::_diagnostics_cpu = 0;
		else if( strcmp( value , "true" ) == 0 )
			rux::engine::_diagnostics_cpu = 1;
	}
	else if( strcmp( conf_property , "leak_watch_to_class" ) == 0 )
	{
		rux::int32 start_index = rux_index_of( value , '"');
		rux::int32 end_index = rux_last_index_of( value , '"' );
		if( start_index > -1 && end_index > -1 && end_index - start_index > 1 )
			memcpy( _leak_watch_to_class , &value[ start_index + 1 ] , end_index - start_index - 1 );
	}
	else if( strcmp( conf_property , "symsrv" ) == 0 )
	{
		rux::int32 start_index = rux_index_of( value , '"' );
		rux::int32 end_index = rux_last_index_of( value , '"' );
		if( start_index > -1 && end_index > -1 && end_index - start_index > 1 )
			memcpy( rux::engine::_globals->_symsrv , &value[ start_index + 1 ] , end_index - start_index - 1 );
	}
	else if( strcmp( conf_property , "opengl_log" ) == 0 )
	{
		if( strcmp( value , "true" ) == 0 )
			rux::gui::engine::_log_enable = 1;
	}
	else if( strcmp( conf_property , "opengl_one_thread_swap_buffers" ) == 0 )
	{
		if( strcmp( value , "true" ) == 0 )
			rux::gui::engine::_opengl_one_thread_swap_buffers = 1;
	}
	else if( strcmp( conf_property , "opengl_check_support_framebuffer" ) == 0 )
	{
		if( strcmp( value , "false" ) == 0 )
			rux::gui::engine::_check_support_framebuffer = 0;
		else if( strcmp( value , "true" ) == 0 )
			rux::gui::engine::_check_support_framebuffer = 1;
	}
	else if( strcmp( conf_property , "opengl_check_gl_error" ) == 0 )
	{
		if( strcmp( value , "false" ) == 0 )
			rux::gui::engine::_check_gl_error = 0;
		else if( strcmp( value , "true" ) == 0 )
			rux::gui::engine::_check_gl_error = 1;
	}
	else if( strcmp( conf_property , "opengl_check_opengl_functions_timeout" ) == 0 )
	{
		if( strcmp( value , "truee" ) == 0 )
			rux::gui::engine::_check_opengl_functions_timeout = 0;
	}	
	else if( strcmp( conf_property , "opengl_check_support_arb_texture_rectangle" ) == 0 )
	{
		if( strcmp( value , "false" ) == 0 )
			rux::gui::engine::_check_support_arb_texture_rectangle = 0;
	}
	else if( strcmp( conf_property , "opengl_check_support_old_yv12_to_rgb_shader" ) == 0 )
	{
		if( strcmp( value , "false" ) == 0 )
			rux::gui::engine::_check_support_old_yv12_to_rgb_shader = 0;
	}
	else if( strcmp( conf_property , "opengl_check_support_old_yv12_to_rgb_shader2" ) == 0 )
	{
		if( strcmp( value , "false" ) == 0 )
			rux::gui::engine::_check_support_old_yv12_to_rgb_shader2 = 0;
	}
	else if( strcmp( conf_property , "opengl_check_support_old_yuyv_to_rgb_shader" ) == 0 )
	{
		if( strcmp( value , "false" ) == 0 )
			rux::gui::engine::_check_support_old_yuyv_to_rgb_shader = 0;
	}
	else if( strcmp( conf_property , "opengl_check_support_yv12_to_rgb_shader" ) == 0 )
	{
		if( strcmp( value , "false" ) == 0 )
			rux::gui::engine::_check_support_yv12_to_rgb_shader = 0;
	}
	else if( strcmp( conf_property , "opengl_check_support_yuyv_to_rgb_shader" ) == 0 )
	{
		if( strcmp( value , "false" ) == 0 )
			rux::gui::engine::_check_support_yuyv_to_rgb_shader = 0;
	}
	else if( strcmp( conf_property , "opengl_check_support_vbo" ) == 0 )
	{
		if( strcmp( value , "false" ) == 0 )
			rux::gui::engine::_check_support_vbo = 0;
	}
	else if( strcmp( conf_property , "opengl_check_support_fxaa" ) == 0 )
	{
		if( strcmp( value , "false" ) == 0 )
			rux::gui::engine::_check_support_fxaa = 0;
	}		
	else if( strcmp( conf_property , "opengl_vsync" ) == 0 )
	{
		if( strcmp( value , "true" ) == 0 )
			::rux::gui::engine::_vsync = 1;
	}
	else if( strcmp( conf_property , "opengl_use_gl_finish" ) == 0 )
	{
		if( strcmp( value , "true" ) == 0 )
			::rux::gui::engine::_use_gl_finish = 1;
	}
};
namespace rux
{
	exports void my_super_test( void )
	{
		printf( "%s\n" , __FUNCTION__ );
	};
};
exports void rux_add_module_method_info( const rux::Module*  module_ptr , const rux::XString& method_name , void* address_ptr )
{
	g_module_manager->AddModuleMethodInfo( module_ptr , method_name , address_ptr );
};
exports void rux_loadable_increment( rux::int32 loadable_module_index )
{
	if( loadable_module_index < 128 )
		XInterlocked::Increment( &g_module_manager->_loaded_modules_by_loadable_module[ loadable_module_index ] );
};
exports void rux_loadable_decrement( rux::int32 loadable_module_index )
{
	if( loadable_module_index < 128 )
		XInterlocked::Decrement( &g_module_manager->_loaded_modules_by_loadable_module[ loadable_module_index ] );
};
/*end loader engine*/
/*begin gc engine*/
exports void rux_gc_thread_collect( size_t thread_index )
{
	XThreadInfo::CollectGCRefs( 1 , thread_index );
};
/*end gc engine*/
/*begin type engine*/
exports void rux_type_register_constructor( Type* type , void* ctor_address )
{
	type->RegisterConstructor( ctor_address );
};
exports void register_function_1( Type* type , ::rux::uint8 with_return , const char* name , void* function_address , Type* param_type0 )
{
	type->RegisterFunction( with_return , name , function_address , param_type0 );
};
exports void rux_type_register_property( Type* type , const char* name , Type* param_type , void* set_property_address , void* get_property_address )
{
	type->RegisterProperty( name , param_type , set_property_address , get_property_address );
};
exports void rux_type_free_types( rux::int32 module_index )
{
	_CS_PTR_LOCK_( ::rux::_cs_current_module , 0 );
	if( g_type_manager )
		g_type_manager->FreeTypes( module_index );
	::rux::_cs_current_module->UnLock( 0 );
};
/*end type engine*/
/*begin engine*/
exports rux::int32 rux_errno( void )
{
	return errno;
};
exports rux::int64 rux_ftell64( FILE * _File )
{
#ifdef __WINDOWS__
	return ::_ftelli64( _File );
#else
#ifdef __ANDROID__
	return ::ftell( _File );
#else
	return ::ftello64( _File );
#endif
#endif
};
exports rux::int32 rux_fseek64( FILE* _File , rux::int64 _Offset , rux::int32 _Origin )
{
#ifdef __WINDOWS__
	return ::_fseeki64( _File , _Offset , _Origin );
#else
#ifdef __ANDROID__
	return ::lseek64( fileno( _File ) , _Offset , _Origin );
#else
	return ::fseeko64( _File , _Offset , _Origin );
#endif
#endif
};
exports rux::int32 rux_ferror( FILE* _File )
{
#ifdef __ANDROID__
	return ferror( _File );
#else
	return ::ferror( _File );
#endif
};
exports rux::int32 rux_feof( FILE* _File )
{
#ifdef __ANDROID__
	return feof( _File );
#else
	return ::feof( _File );
#endif
};
exports rux::int32 rux_chsize( rux::int32 _FileHandle, long _Size )
{
#ifdef __WINDOWS__
	return ::_chsize( _FileHandle , _Size );
#else
	return ::ftruncate( _FileHandle , _Size );
#endif
};
exports FILE* rux_fopen( const char * _Filename, const char * _Mode )
{
	FILE* file = ::fopen( _Filename , _Mode );
	if( file )
		XInterlocked::Increment( &_rux_total_descriptors_count[ 0 ] );
	return file;
};
exports rux::int32 rux_fclose( FILE * _File )
{
	XInterlocked::Decrement( &_rux_total_descriptors_count[ 0 ] );
	return ::fclose( _File );
};
exports void rux_stop_threads( void )
{
	if( XInterlocked::CompareExchange( &::rux::_current_module_reference , 0 , 0 ) == 1 )
	{
		if( g_rux_memory_manager )
			g_rux_memory_manager->StopThreads();
	}
};
dll_internal ::rux::byte post_deinitialize( void );
void stop( void )
{
	CS_PTR_LOCK( rux::gui::engine::_cs_font_bases );
	for( size_t index0 = 0 ; index0 < rux::gui::engine::_font_bases.Count() ; index0++ )
		rux::engine::free_object< rux::gui::engine::RenderFont >( rux::gui::engine::_font_bases[ index0 ] );
	CS_PTR_UNLOCK( rux::gui::engine::_cs_font_bases );
	CS_PTR_LOCK( rux::gui::engine::_cs_rux_fonts );
	while( rux::gui::engine::_rux_fonts.Count() > 0 )
	{
		rux::engine::free_object< rux::gui::engine::freetype_font >( rux::gui::engine::_rux_fonts[ rux::gui::engine::_rux_fonts.Count() - 1 ] );
		rux::gui::engine::_rux_fonts.RemoveAt( rux::gui::engine::_rux_fonts.Count() - 1 );
	}
	rux::gui::engine::_rux_fonts.FreeMemory();
	CS_PTR_UNLOCK( rux::gui::engine::_cs_rux_fonts );	
	if( rux::gui::engine::_rux_ft_library )
	{
		FT_Done_FreeType( rux::gui::engine::_rux_ft_library );
		rux::gui::engine::_rux_ft_library = NULL;
	}
	::rux::gui::engine::_dispatchers.FreeMemory();
	rux::gui::engine::_windows.FreeMemory();
	rux::gui::engine::_rux_fonts.FreeMemory();
	rux::gui::engine::_font_bases.FreeMemory();
	rux::gui::engine::_on_events.FreeMemory();
	rux::gui::engine::_on_event_owners.FreeMemory();
	rux::gui::engine::OpenGL::deinitialize();
	
	if( g_module_manager )
	{
		rux::engine::free_object< ModuleManager >( g_module_manager );
		g_module_manager = NULL;
	}		
	/*if( ::rux::log::_logger )
	{		
		::rux::log::_logger->unregister_module( _rux_current_module_index );
		rux::engine::free_object< ::rux::log::Logger >( ::rux::log::_logger );		
		::rux::log::_logger = NULL;		
	}*/
	::rux::memory::release< ::rux::task::Tasker >( ::rux::task::_tasker );
	if( g_rux_memory_manager )
	{		
		rux::engine::free_object< XMemoryManager >( g_rux_memory_manager );		
		g_rux_memory_manager = NULL;		
	}
	if( g_type_manager )
	{
		rux::engine::free_object< XTypeManager >( g_type_manager );
		g_type_manager = NULL;
	}
	::rux::engine::_globals->_engine_started = 0;
	if( ::rux::engine::_globals->_engine_post_deinitialize_after_stop == 1 )
	{
		::rux::engine::_globals->_engine_post_deinitialize_after_stop = 0;
		post_deinitialize();
	}
};
exports void rux_memory_register_exception_event( const char* module_name , rux::rux_exception_event_t exception_event )
{
	if( g_rux_memory_manager )
		g_rux_memory_manager->register_exception_event( module_name , exception_event );
};
exports void rux_memory_unregister_exception_event( rux::rux_exception_event_t exception_event )
{
	if( g_rux_memory_manager )
		g_rux_memory_manager->unregister_exception_event( exception_event );
};
exports void rux_memory_network_init( void )
{
	if( g_rux_memory_manager )
		g_rux_memory_manager->network_init();
};
exports void rux_memory_network_deinit( void )
{
	if( g_rux_memory_manager )
		g_rux_memory_manager->network_deinit();
};
exports void rux_memory_reregister_handler( void )
{
	if( g_rux_memory_manager )
		g_rux_memory_manager->reregister_handler();
};
#ifdef __WINDOWS__
exports void rux_memory_add_descriptor( rux::int32 module_index , const char* file_name_ptr , HANDLE handle )
#elif defined( __UNIX__ )
exports void rux_memory_add_descriptor( rux::int32 module_index , const char* file_name_ptr , rux::int32 handle )
#endif
{
	if( ::rux::engine::_diagnostic == 1 )	
		XMemoryManager::add_descriptor( module_index , file_name_ptr , handle , 0 );
};
#ifdef __WINDOWS__
exports void rux_memory_remove_descriptor( HANDLE handle , rux::int32 module_index )
#elif defined( __UNIX__ )
exports void rux_memory_remove_descriptor( rux::int32 handle , rux::int32 module_index )
#endif
{
	if( ::rux::engine::_diagnostic == 1 )
		XMemoryManager::remove_descriptor( handle , module_index );
};
exports rux::uint8 rux_already_executing_in_current_path( void )
{
	rux::uint8 executing = 0;
	if( g_rux_memory_manager )
		executing = g_rux_memory_manager->already_executing_in_current_path();
	return executing;
};
exports rux::pid_t rux_gui_get_message_processing_thread_id( void )
{
	return rux::gui::engine::_message_processing_thread_id;
};
exports void rux_gui_pump_message( size_t thread_index )
{
	if( XThreadInfo::locked_crt_sect_count( thread_index ) == 0 && XThreadInfo::write_locked_rdwrlock_count( thread_index ) == 0 && XThreadInfo::read_locked_rdwrlock_count( thread_index ) == 0 )
		rux::gui::engine::pump_message( 0 );
};
dll_internal ::rux::threading::RdWrLock* _cs_rux_search_directories = NULL;
dll_internal ::rux::threading::RdWrLock* _cs_rux_threads = NULL;
dll_internal ::rux::threading::RdWrLock* _cs_rux_modules_name = NULL;
dll_internal ::rux::threading::RdWrLock* _cs_first_last_memory = NULL;
dll_internal ::rux::threading::RdWrLock* _cs_cpu_logs = NULL;
dll_internal ::rux::threading::RdWrLock* _rux_cs_exception_events = NULL;
namespace rux
{
	namespace gui
	{
		namespace engine
		{		
			dll_internal XCrtSect* _cs_gl_library = NULL;
			dll_internal XCrtSect* _cs_rux_fonts = NULL;
			dll_internal XCrtSect* _cs_font_bases = NULL;
			dll_internal XCrtSect* _cs_engine = NULL;
			dll_internal XCrtSect* _cs_windows = NULL;		
			dll_internal XCrtSect* _cs_dispatchers = NULL;
#if defined( __UNIX__ )
#ifdef __ANDROID__
#else
#endif
			dll_internal XCrtSect* _cs_x_display_main = NULL;			
#endif
		};
	};
};
namespace rux
{
	dll_internal ::rux::int32 module_initialize( void* param );
	dll_internal ::rux::int32 module_deinitialize( void );
};
dll_internal void pre_initialize( void )
{
	_rux_is_memory_manager = 1;
	rux::engine::_rux_alloc = XMemoryManager::Alloc;
	rux::engine::_rux_free = XMemoryManager::Free;
	::rux::engine::_page_size = ::rux::memory::get_page_size();

	std::string debug_utf8_control_names , protect_markers_file_and_line;
	char symsrv[ 128 ] = {0};
	::rux::byte opengl_render = 1 , winapi_trace = 0 , heap_corruption_checker = 0 , is_core_dump = 0;
	::rux::log::XEnum_LogLevel log_level = ::rux::log::XEnum_LogLevel_Error;
	char conf_filename[ 2048 ] = {0};
	::rux::diagnostics::module::get_current_executable_directory( conf_filename , 2048 );
	::rux::safe_strncat( conf_filename , "/rux.conf" , 2047 );
	FILE* file = fopen( conf_filename , "r" );
	if( file )
	{
		while( fgets( conf_filename , 2048 , file ) != NULL )
		{
			char* ptr = conf_filename;
			while( *ptr == ' ' || *ptr == '\t' )
				ptr++;
			char* name = ptr;
			while( *ptr != ' ' && *ptr != '\t' && *ptr != '=' && *ptr != 0 )
				ptr++;
			if( *ptr == 0 )
				continue;
			*ptr = 0;
			ptr++;
			while( *ptr == ' ' || *ptr == '\t' || *ptr == '=' )
				ptr++;
			if( strcmp( name , "diagnostics_timeout" ) == 0 )
			{	
				::rux::uint32 timeout = ::rux::string::to_uint32( ptr );
				if( timeout > 0 )
					::rux::engine::_diagnostics_timeout = timeout;
			}
			else if( strcmp( name , "diagnostic" ) == 0 )
			{
				if( strncmp( ptr , "false" , 5 ) == 0 )
					::rux::engine::_diagnostic = 0;
			}
			else if( strcmp( name , "heap_corruption_checker" ) == 0 )
			{
				if( strncmp( ptr , "true" , 4 ) == 0 )
					heap_corruption_checker = 1;
			}
			else if( strcmp( name , "catch_exception" ) == 0 )
			{
				if( strncmp( ptr , "false" , 5 ) == 0 )
					_rux_catch_exception = 0;
			}
			else if( strcmp( name , "detail_memory_leak" ) == 0 )
			{
				if( strncmp( ptr , "true" , 4 ) == 0 )
					_rux_is_memory_leaks = 1;
			}
			else if( strcmp( name , "core_dump" ) == 0 )
			{
				if( strncmp( ptr , "true" , 4 ) == 0 )
					is_core_dump = 1;
			}
			else if( strcmp( name , "diagnostics_cpu" ) == 0 )
			{
				if( strncmp( ptr , "false" , 5 ) == 0 )
					rux::engine::_diagnostics_cpu = 0;
				else if( strncmp( ptr , "true" , 4 ) == 0 )
					rux::engine::_diagnostics_cpu = 1;
			}
			else if( strcmp( name , "protect_markers_file_and_line" ) == 0 )
			{
				rux::int32 start_index = rux_index_of( ptr , '"');
				rux::int32 end_index = rux_last_index_of( ptr , '"' );
				if( start_index > -1 && end_index > -1 && end_index - start_index > 1 )
				{
					ptr[ end_index ] = 0;
					protect_markers_file_and_line = &ptr[ start_index + 1 ];
					ptr[ end_index ] = '"';
				}
			}
			else if( strcmp( name , "debug_utf8_control_names" ) == 0 )
			{
				rux::int32 start_index = rux_index_of( ptr , '"');
				rux::int32 end_index = rux_last_index_of( ptr , '"' );
				if( start_index > -1 && end_index > -1 && end_index - start_index > 1 )
				{
					ptr[ end_index ] = 0;
					debug_utf8_control_names = &ptr[ start_index + 1 ];
					ptr[ end_index ] = '"';
				}
			}
			else if( strcmp( name , "leak_watch_to_class" ) == 0 )
			{
				rux::int32 start_index = rux_index_of( ptr , '"');
				rux::int32 end_index = rux_last_index_of( ptr , '"' );
				if( start_index > -1 && end_index > -1 && end_index - start_index > 1 )
					memcpy( _leak_watch_to_class , &ptr[ start_index + 1 ] , end_index - start_index - 1 );
			}
			else if( strcmp( name , "log_level" ) == 0 )
			{
				rux::int32 start_index = rux_index_of( ptr , '"');
				rux::int32 end_index = rux_last_index_of( ptr , '"' );
				if( start_index > -1 && end_index > -1 && end_index - start_index > 1 )
				{
					ptr[ end_index ] = 0;
					if( strcmp( &ptr[ start_index + 1 ] , "trace" ) == 0 )
						log_level = ::rux::log::XEnum_LogLevel_Trace;
					else if( strcmp( &ptr[ start_index + 1 ] , "verbose" ) == 0 )
						log_level = ::rux::log::XEnum_LogLevel_Verbose;
					else if( strcmp( &ptr[ start_index + 1 ] , "error" ) == 0 )
						log_level = ::rux::log::XEnum_LogLevel_Error;
					else if( strcmp( &ptr[ start_index + 1 ] , "debug" ) == 0 )
						log_level = ::rux::log::XEnum_LogLevel_Debug;
					else if( strcmp( &ptr[ start_index + 1 ] , "warn" ) == 0 )
						log_level = ::rux::log::XEnum_LogLevel_Warn;
					ptr[ end_index ] = '"';
				}
			}
			else if( strcmp( name , "symsrv" ) == 0 )
			{
				rux::int32 start_index = rux_index_of( ptr , '"' );
				rux::int32 end_index = rux_last_index_of( ptr , '"' );
				if( start_index > -1 && end_index > -1 && end_index - start_index > 1 && end_index - start_index < 128 )
					memcpy( symsrv , &ptr[ start_index + 1 ] , end_index - start_index - 1 );
			}
			else if( strcmp( name , "opengl_log" ) == 0 )
			{
				if( strncmp( ptr , "true" , 4 ) == 0 )
					rux::gui::engine::_log_enable = 1;
			}
			else if( strcmp( name , "opengl_one_thread_swap_buffers" ) == 0 )
			{
				if( strncmp( ptr , "true" , 4 ) == 0 )
					rux::gui::engine::_opengl_one_thread_swap_buffers = 1;
			}
			else if( strcmp( name , "opengl_render" ) == 0 )
			{
				if( strncmp( ptr , "false" , 5 ) == 0 )
					opengl_render = 0;
				else
					opengl_render = 1;
			}
			else if( strcmp( name , "winapi_trace" ) == 0 )
			{
				if( strncmp( ptr , "true" , 4 ) == 0 )
					winapi_trace = 1;
			}
			else if( strcmp( name , "opengl_check_support_framebuffer" ) == 0 )
			{
				if( strncmp( ptr , "false" , 5 ) == 0 )
					rux::gui::engine::_check_support_framebuffer = 0;
				else if( strncmp( ptr , "true" , 4 ) == 0 )
					rux::gui::engine::_check_support_framebuffer = 1;
			}
			else if( strcmp( name , "opengl_check_gl_error" ) == 0 )
			{
				if( strncmp( ptr , "false" , 5 ) == 0 )
					rux::gui::engine::_check_gl_error = 0;
				else if( strncmp( ptr , "true" , 4 ) == 0 )
					rux::gui::engine::_check_gl_error = 1;
			}
			else if( strcmp( name , "opengl_check_opengl_functions_timeout" ) == 0 )
			{
				if( strncmp( ptr , "true" , 4 ) == 0 )
					rux::gui::engine::_check_opengl_functions_timeout = 0;
			}						
			else if( strcmp( name , "opengl_check_support_arb_texture_rectangle" ) == 0 )
			{
				if( strncmp( ptr , "false" , 5 ) == 0 )
					rux::gui::engine::_check_support_arb_texture_rectangle = 0;
			}
			else if( strcmp( name , "opengl_check_support_old_yv12_to_rgb_shader" ) == 0 )
			{
				if( strncmp( ptr , "false" , 5 ) == 0 )
					rux::gui::engine::_check_support_old_yv12_to_rgb_shader = 0;
			}
			else if( strcmp( name , "opengl_check_support_old_yv12_to_rgb_shader2" ) == 0 )
			{
				if( strncmp( ptr , "false" , 5 ) == 0 )
					rux::gui::engine::_check_support_old_yv12_to_rgb_shader2 = 0;
			}
			else if( strcmp( name , "opengl_check_support_old_yuyv_to_rgb_shader" ) == 0 )
			{
				if( strncmp( ptr , "false" , 5 ) == 0 )
					rux::gui::engine::_check_support_old_yuyv_to_rgb_shader = 0;
			}
			else if( strcmp( name , "opengl_check_support_yv12_to_rgb_shader" ) == 0 )
			{
				if( strncmp( ptr , "false" , 5 ) == 0 )
					rux::gui::engine::_check_support_yv12_to_rgb_shader = 0;
			}
			else if( strcmp( name , "opengl_check_support_yuyv_to_rgb_shader" ) == 0 )
			{
				if( strncmp( ptr , "false" , 5 ) == 0 )
					rux::gui::engine::_check_support_yuyv_to_rgb_shader = 0;
			}
			else if( strcmp( name , "opengl_check_support_fxaa" ) == 0 )
			{
				if( strncmp( ptr , "false" , 5 ) == 0 )
					rux::gui::engine::_check_support_fxaa = 0;
			}
			else if( strcmp( name , "opengl_vsync" ) == 0 )
			{
				if( strncmp( ptr , "true" , 4 ) == 0 )
					rux::gui::engine::_vsync = 1;
			}
			else if( strcmp( name , "opengl_use_gl_finish" ) == 0 )
			{
				if( strncmp( ptr , "true" , 4 ) == 0 )
					rux::gui::engine::_use_gl_finish = 1;
			}
			else if( strcmp( name , "opengl_check_support_vbo" ) == 0 )
			{
				if( strncmp( ptr , "false" , 5 ) == 0 )
					rux::gui::engine::_check_support_vbo = 0;
			}
		}
		fclose( file );
	}
	_cs_cpu_logs = ::rux::new_object< ::rux::threading::RdWrLock >();
	_cs_rux_modules_name = ::rux::new_object< ::rux::threading::RdWrLock >();
	_cs_first_last_memory = ::rux::new_object< ::rux::threading::RdWrLock >();
	_cs_rux_search_directories = ::rux::new_object< ::rux::threading::RdWrLock >();
	_cs_rux_threads = ::rux::new_object< ::rux::threading::RdWrLock >();
	_rux_cs_exception_events = ::rux::new_object< ::rux::threading::RdWrLock >();
	::rux::gui::engine::_cs_gl_library = ::rux::new_object< XCrtSect >();
	::rux::gui::engine::_cs_rux_fonts = ::rux::new_object< XCrtSect >();
	::rux::gui::engine::_cs_font_bases = ::rux::new_object< XCrtSect >();
	::rux::gui::engine::_cs_engine = ::rux::new_object< XCrtSect >();
	::rux::gui::engine::_cs_windows = ::rux::new_object< XCrtSect >();
	::rux::gui::engine::_cs_dispatchers = ::rux::new_object< XCrtSect >();
	::rux::task::_cs_tasks = ::rux::new_object< ::rux::threading::RdWrLock >();
	::rux::task::_cs_tasks_event_handlers = ::rux::new_object< ::rux::threading::RdWrLock >();
#if defined( __UNIX__ )
#ifdef __ANDROID__
#else
#endif
	::rux::gui::engine::_cs_x_display_main = ::rux::new_object< XCrtSect >();	
#endif
	memset( ::rux::memory::_memory_clusters , 0 , sizeof( ::rux::memory::XMemoryCluster ) * RUX_MAX_MEMORY_CLUSTERS_COUNT );
	memset( _rux_modules_name , 0 , RUX_MAX_MODULES_SIZE * 64 );
	for( size_t index0 = 0 ; index0 < RUX_MAX_MODULES_SIZE ; index0++ )
	{
		_rux_total_memory_size[ index0 ] = 0;
		_rux_old_total_memory_size[ index0 ] = 0;
		_rux_old_total_descriptors_count[ index0 ] = 0;
		_rux_total_descriptors_count[ index0 ] = 0;
	}
	_rux_cs_exception_events->WriteLock();
	memset( _rux_exception_events , 0 , 1024 * sizeof( ::rux::rux_exception_event_t ) );
	memset( _rux_exception_events_module_name , 0 , 1024 * 1024 );
	_rux_cs_exception_events->WriteUnlock();
	_rux_current_module_index = XMemoryManager::register_module( g_current_module_name );

	rux::engine::_globals = new ( XMemoryManager::Alloc( sizeof( ::rux::engine::globals ) , typeid( ::rux::engine::globals ).name() , __FILE__ , __LINE__ , _rux_current_module_index , "rux::engine::_globals" ) )::rux::engine::globals();
	rux::engine::_globals->_gui_globals = new ( XMemoryManager::Alloc( sizeof( ::rux::gui::engine::globals ) , typeid( ::rux::gui::engine::globals ).name() , __FILE__ , __LINE__ , _rux_current_module_index , "rux::gui::engine::_globals" ) )::rux::gui::engine::globals();
	if( debug_utf8_control_names.length() )
	{
		size_t pos = 0 , begin = 0;
		while( ( pos = debug_utf8_control_names.find_first_of( ';' , pos ) ) != std::string::npos )
		{
			if( pos - begin > 0 )
			{
				std::string utf8_control_name_string;
				utf8_control_name_string = debug_utf8_control_names.substr( begin , pos - begin );
				::rux::uint32 utf8_control_name = ::rux::string::to_uint32( utf8_control_name_string.c_str() );
				if( ::rux::engine::_globals->_gui_globals->_debug_utf8_control_names == 0 )
					::rux::engine::_globals->_gui_globals->_debug_utf8_control_names = alloc_object_macros( XMallocArray< ::rux::uint32 > );
				::rux::engine::_globals->_gui_globals->_debug_utf8_control_names->Add( utf8_control_name );
			}
			pos++;
			begin = pos;
		}
		pos = debug_utf8_control_names.length();
		if( pos - begin > 0 )
		{
			std::string utf8_control_name_string;
			utf8_control_name_string = debug_utf8_control_names.substr( begin , pos - begin );
			::rux::uint32 utf8_control_name = ::rux::string::to_uint32( utf8_control_name_string.c_str() );
			if( ::rux::engine::_globals->_gui_globals->_debug_utf8_control_names == 0 )
				::rux::engine::_globals->_gui_globals->_debug_utf8_control_names = alloc_object_macros( XMallocArray< ::rux::uint32 > );
			::rux::engine::_globals->_gui_globals->_debug_utf8_control_names->Add( utf8_control_name );
		}
	}
	if( protect_markers_file_and_line.length() )
	{
		size_t pos = 0 , begin = 0;
		while( ( pos = protect_markers_file_and_line.find_first_of( ';' , pos ) ) != std::string::npos )
		{
			if( pos - begin > 0 )
			{
				std::string protect_markers_file_and_line_string;
				protect_markers_file_and_line_string = protect_markers_file_and_line.substr( begin , pos - begin );

				size_t line_pos = protect_markers_file_and_line_string.find_first_of( ':' , 0 );
				if( line_pos > 0 && line_pos != std::string::npos && protect_markers_file_and_line.length() - line_pos > 1 )
				{
					std::string line_string;
					line_string = protect_markers_file_and_line_string.substr( line_pos + 1 , protect_markers_file_and_line.length() - line_pos - 1 );

					::rux::uint32 line = ::rux::string::to_uint32( line_string.c_str() );
					char* file = alloc_array_macros( char , line_pos + 1 );
					::memcpy( file , protect_markers_file_and_line_string.c_str() , line_pos );
					file[ line_pos ] = 0;
					if( ::rux::engine::_globals->_protect_markers_files == 0 )
						::rux::engine::_globals->_protect_markers_files = alloc_object_macros( XMallocArray< char* > );
					if( ::rux::engine::_globals->_protect_markers_lines == 0 )
						::rux::engine::_globals->_protect_markers_lines = alloc_object_macros( XMallocArray< ::rux::uint32 > );				
					::rux::engine::_globals->_protect_markers_files->Add( file );
					::rux::engine::_globals->_protect_markers_lines->Add( line );
				}
			}
			pos++;
			begin = pos;
		}
		pos = protect_markers_file_and_line.length();
		if( pos - begin > 0 )
		{
			std::string protect_markers_file_and_line_string;
			protect_markers_file_and_line_string = protect_markers_file_and_line.substr( begin , pos - begin );

			size_t line_pos = protect_markers_file_and_line_string.find_first_of( ':' , 0 );
			if( line_pos > 0 && line_pos != std::string::npos && protect_markers_file_and_line.length() - line_pos > 1 )
			{
				std::string line_string;
				line_string = protect_markers_file_and_line_string.substr( line_pos + 1 , protect_markers_file_and_line.length() - line_pos - 1 );

				::rux::uint32 line = ::rux::string::to_uint32( line_string.c_str() );
				char* file = alloc_array_macros( char , line_pos + 1 );
				::memcpy( file , protect_markers_file_and_line_string.c_str() , line_pos );
				file[ line_pos ] = 0;
				if( ::rux::engine::_globals->_protect_markers_files == 0 )
					::rux::engine::_globals->_protect_markers_files = alloc_object_macros( XMallocArray< char* > );
				if( ::rux::engine::_globals->_protect_markers_lines == 0 )
					::rux::engine::_globals->_protect_markers_lines = alloc_object_macros( XMallocArray< ::rux::uint32 > );				
				::rux::engine::_globals->_protect_markers_files->Add( file );
				::rux::engine::_globals->_protect_markers_lines->Add( line );
			}
		}
	}
	
	rux::engine::_globals->_loader_globals = new ( XMemoryManager::Alloc( sizeof( ::rux::loader::engine::globals ) , typeid( ::rux::loader::engine::globals ).name() , __FILE__ , __LINE__ , _rux_current_module_index , "rux::loader::engine::_globals" ) )::rux::loader::engine::globals();
	rux::engine::_globals->_log_globals = new ( XMemoryManager::Alloc( sizeof( ::rux::log::engine::globals ) , typeid( ::rux::log::engine::globals ).name() , __FILE__ , __LINE__ , _rux_current_module_index , "rux::log::engine::_globals" ) )::rux::log::engine::globals();
	rux::engine::_globals->_service_globals = new ( XMemoryManager::Alloc( sizeof( ::rux::service::engine::globals ) , typeid( ::rux::service::engine::globals ).name() , __FILE__ , __LINE__ , _rux_current_module_index , "rux::service::engine::_globals" ) )::rux::service::engine::globals();
	
	::rux::safe_strncpy( rux::engine::_globals->_symsrv , symsrv , 127 );
	rux::engine::_globals->_opengl_render = opengl_render;
	rux::gui::engine::_winapi_trace = winapi_trace;
	::rux::engine::_globals->_log_level = log_level;
	rux::engine::_globals->_diagnostic = ::rux::engine::_diagnostic;
	rux::engine::_globals->_heap_corruption_checker = heap_corruption_checker;
	rux::engine::_globals->_is_core_dump = is_core_dump;

	rux::engine::_globals->_alloc = ::rux::engine::_rux_alloc;
	rux::engine::_globals->_free = ::rux::engine::_rux_free;
	rux::engine::_globals->_add_stack_var = XMemoryManager::add_stack_var;
	rux::engine::_globals->_remove_stack_var = XMemoryManager::remove_stack_var;
	rux::engine::_globals->_find_begin_memory_chunk = XMemoryManager::find_begin_memory_chunk;
	rux::engine::_globals->_rux_get_memory_size = XMemoryManager::get_memory_size;
	rux::engine::_globals->_rux_memory_init = ::rux::module_initialize;
	rux::engine::_globals->_rux_memory_free = ::rux::module_deinitialize;
	rux::engine::_globals->_rux_memory_register_module = XMemoryManager::register_module;
	rux::engine::_globals->_rux_fclose = rux_fclose;
	rux::engine::_globals->_rux_fopen = rux_fopen;
	rux::engine::_globals->_rux_ftell64 = rux_ftell64;
	rux::engine::_globals->_rux_fseek64 = rux_fseek64;
	rux::engine::_globals->_rux_freopen = ::freopen;
	rux::engine::_globals->_rux_fwrite = ::fwrite;
	rux::engine::_globals->_rux_fread = ::fread;
	rux::engine::_globals->_rux_ferror = rux_ferror;
	rux::engine::_globals->_rux_feof = rux_feof;
	rux::engine::_globals->_rux_fputs = ::fputs;
	rux::engine::_globals->_vfprintf = ::vfprintf;
	rux::engine::_globals->_rux_fflush = ::fflush;
	rux::engine::_globals->_rux_chsize = rux_chsize;
	rux::engine::_globals->_rux_errno = rux_errno;
	rux::engine::_globals->_log_globals->_write_log = rux_write_log;
	rux::engine::_globals->_add_task = ::rux::task::Tasker::add;
	rux::engine::_globals->_add_times33_hash = XMemoryManager::add_times33_hash;
	rux::engine::_globals->_add_or_remove_thread_event = XMemoryManager::add_or_remove_thread_event;
	rux::engine::_globals->_remove_task = ::rux::task::Tasker::remove;
	rux::engine::_globals->_force_execute_task = ::rux::task::Tasker::force_execute_task;
	rux::engine::_globals->_task_add_event_handler = ::rux::task::Tasker::add_event_handler;
	rux::engine::_globals->_task_remove_event_handler = ::rux::task::Tasker::remove_event_handler;
	rux::engine::_globals->_rux_already_executing_in_current_path = rux_already_executing_in_current_path;
#ifdef __WINDOWS__
	const char* env = getenv( "Path" );
#else
	const char* env = getenv( "PATH" );
	::rux::byte usr_bin_found = 0;
	const char* ptr = env;
	for( ; ; )
	{
		ptr = strstr( ptr , "/usr/bin" );
		if( ptr )
		{
			if( ptr[ 8 ] == ':' || ptr[ 8 ] == 0 )
			{
				usr_bin_found = 1;
				break;
			}
			ptr++;
		}
		else
			break;
	}
	if( usr_bin_found == 0 )
	{
		size_t length = strlen( env );
		char* set_env = alloc_array_macros( char , length + 10 );
		::memcpy( set_env , env , length );
		::memcpy( &set_env[ length ] , ":/usr/bin" , 10 );
		if( setenv( "PATH" , set_env , 1 ) == 0 )
			env = getenv( "PATH" );
		::rux::engine::free_mem( set_env );
	}
#endif
	if( env )
	{
		size_t length = strlen( env );
		::rux::engine::_globals->_env = alloc_array_macros( char , length + 1 );
		::memcpy( ::rux::engine::_globals->_env , env , length + 1 );
		char* ptr = ::rux::engine::_globals->_env , * begin = 0;
		do
		{
#ifdef __WINDOWS__
			if( *ptr == ';' )
#else
			if( *ptr == ':' )
#endif
			{
				if( begin && ptr - begin > 1 )
				{
					ptr--;
					if( *ptr == '/' || *ptr == '\\' )
						*ptr = 0;
					ptr++;
					*ptr = 0;
					::rux::memory::copy< char* , 1 >( ::rux::engine::_globals->_env_variables , ::rux::engine::_globals->_env_variables_count , ::rux::engine::_globals->_env_variables_count , &begin , 1 );
				}
				ptr++;
				begin = ptr;
			}
			else
				begin = ptr;
#ifdef __WINDOWS__
			ptr = strstr( ptr , ";" );
#else
			ptr = strstr( ptr , ":" );
#endif	
		}
		while( ptr );
		if( begin && begin[ 0 ] != 0 )
		{
			length = strlen( begin );
			if( begin[ length - 1 ] == '/' || begin[ length - 1 ] == '\\' )
				begin[ length - 1 ] = 0;
			::rux::memory::copy< char* , 1 >( ::rux::engine::_globals->_env_variables , ::rux::engine::_globals->_env_variables_count , ::rux::engine::_globals->_env_variables_count , &begin , 1 );
		}
	}
	declare_stack_variable( char , error_string , 1024 );
	::rux::diagnostics::os::get_os( ::rux::engine::_globals->_os , 128 , error_string , 1024 );
	rux_get_hostname( ::rux::engine::_globals->_hostname , 256 );
	rux_get_executable_filename_only( ::rux::engine::_globals->_executable );
	rux_get_executable_directory( ::rux::engine::_globals->_executable_directory );
#ifdef __WINDOWS__
	rux_concatenation( ::rux::engine::_globals->_ruxiver_filename , ::rux::engine::_globals->_executable_directory , "/tools/ruxiver/ruxiver.exe" );
	rux_concatenation( ::rux::engine::_globals->_neficl_filename , ::rux::engine::_globals->_executable_directory , "/tools/rux.neficl/rux.neficl.exe" );
#else
	rux_concatenation( ::rux::engine::_globals->_ruxiver_filename , ::rux::engine::_globals->_executable_directory , "/tools/ruxiver/ruxiver" );
	rux_concatenation( ::rux::engine::_globals->_neficl_filename , ::rux::engine::_globals->_executable_directory , "/tools/rux.neficl/rux.neficl" );
#endif					
	::rux::diagnostics::module::get_current_module_filename( ::rux::engine::_globals->_engine_full_filename , 2048 );
#ifdef __WINDOWS__
	TIMECAPS tc;
	if( ::rux::engine::_globals->_winmm_module.timeGetDevCaps( &tc , sizeof( TIMECAPS ) ) == TIMERR_NOERROR ) 
	{
		::rux::engine::_globals->_timer_res = min( max( tc.wPeriodMin , 1 ) , tc.wPeriodMax );
		if( ::rux::engine::_globals->_winmm_module.timeBeginPeriod( ::rux::engine::_globals->_timer_res ) != TIMERR_NOERROR )
			::rux::engine::_globals->_timer_res = UINT_MAX;
	}
#endif
	::rux::task::_tasks = alloc_object_macros( XMallocArray< ::rux::task::Task* > );
	::rux::task::_tasks_event_handlers = alloc_object_macros( XMallocArray< ::rux::task::TaskEventHandler* > );
	if( rux::engine::_globals->_symsrv[ 0 ] == 0 )
		strcpy( rux::engine::_globals->_symsrv , "$(origin);$(origin)\\lib;$(origin)\\plugins;srv*\\\\192.168.40.37\\video7" );
	//declare_stack_variable( char , last_dir , 1024 );
	//declare_stack_variable( char , dir , 1024 );
	//strcpy( dir , ::rux::engine::_globals->_executable_directory );
	//strcat( dir , "/stable" );
	//strcpy( last_dir , ::rux::engine::_globals->_executable_directory );
	//strcat( last_dir , "/stable_last" );
	//rux::io::directory::remove_all( last_dir );
	//rux::io::directory::rename( dir , last_dir );
	//if( ::rux::engine::_diagnostic == 1 )
	//{
	//	rux::io::directory::create( dir );
	//	::chmod( dir , 0777 );
	//}
	//strcpy( dir , ::rux::engine::_globals->_executable_directory );
	//strcat( dir , "/cpu" );
	//strcpy( last_dir , ::rux::engine::_globals->_executable_directory );
	//strcat( last_dir , "/cpu_last" );
	//rux::io::directory::remove_all( last_dir );
	//rux::io::directory::rename( dir , last_dir );
	//if( ::rux::engine::_diagnostic )
	//{
	//	rux::io::directory::create( dir );
	//	::chmod( dir , 0777 );
	//}
	//strcpy( dir , ::rux::engine::_globals->_executable_directory );
	//strcat( dir , "/memory" );
	//strcpy( last_dir , ::rux::engine::_globals->_executable_directory );
	//strcat( last_dir , "/memory_last" );
	//rux::io::directory::remove_all( last_dir );
	//rux::io::directory::rename( dir , last_dir );
	//if( ::rux::engine::_diagnostic == 1 )
	//{
	//	rux::io::directory::create( dir );
	//	::chmod( dir , 0777 );
	//}	
};
dll_internal ::rux::byte post_deinitialize( void )
{
	if( ::rux::engine::_globals->_engine_started == 0 )
	{
		::rux::debug::deinitialize();
		rux::deinitialize();
		::rux::memory::release< XMallocArray< ::rux::task::TaskEventHandler* > >( ::rux::task::_tasks_event_handlers );
		::rux::memory::release< XMallocArray< ::rux::task::Task* > >( ::rux::task::_tasks );
		if( _rux_threads )
		{
			for( size_t index0 = 0 ; index0 < _rux_threads_count ; index0++ )
			{
				if( _rux_threads[ index0 ] )
				{
					_rux_threads[ index0 ]->lock();
					if( XInterlocked::CompareExchange( &_rux_threads[ index0 ]->_registered , 0 , 0 ) )
						_rux_threads[ index0 ]->unregister();
					_rux_threads[ index0 ]->unlock();
					::rux::engine::free_object< XThreadInfo >( _rux_threads[ index0 ] );
				}
			}
			::rux::memory::release< XThreadInfo* >( _rux_threads );
		}

		rux::engine::_globals->_service_globals->~globals();
		XMemoryManager::Free( rux::engine::_globals->_service_globals );

		rux::engine::_globals->_log_globals->~globals();
		XMemoryManager::Free( rux::engine::_globals->_log_globals );

		rux::engine::_globals->_loader_globals->~globals();
		XMemoryManager::Free( rux::engine::_globals->_loader_globals );

		rux::engine::_globals->_gui_globals->~globals();
		XMemoryManager::Free( rux::engine::_globals->_gui_globals );
#ifdef __WINDOWS__
		if( rux::engine::_globals->_timer_res != UINT_MAX )
		{
			::rux::engine::_globals->_winmm_module.timeEndPeriod( ::rux::engine::_globals->_timer_res );
			::rux::engine::_globals->_timer_res = UINT_MAX;
		}
#endif
		rux::engine::_globals->~globals();
		XMemoryManager::Free( rux::engine::_globals );

		rux::engine::_globals = NULL;

		XMemoryManager::memory_unload_module( _rux_current_module_index );
		rux::engine::_rux_alloc = 0;
		rux::engine::_rux_free = 0;

		for( size_t index0 = 0 ; index0 < ::rux::memory::_memory_clusters_count ; index0++ )
			::rux::delete_object< ::rux::memory::XMemoryCluster >( ::rux::memory::_memory_clusters[ index0 ] );
		::rux::delete_object< ::rux::threading::RdWrLock >( _cs_rux_search_directories );
		_cs_rux_search_directories = NULL;
		::rux::delete_object< ::rux::threading::RdWrLock >( ::rux::task::_cs_tasks_event_handlers );
		::rux::task::_cs_tasks_event_handlers = 0;
		::rux::delete_object< ::rux::threading::RdWrLock >( ::rux::task::_cs_tasks );
		::rux::task::_cs_tasks = 0;
		::rux::delete_object< ::rux::threading::RdWrLock >( _cs_rux_threads );
		_cs_rux_threads = NULL;
		::rux::delete_object< ::rux::threading::RdWrLock >( _cs_rux_modules_name );
		_cs_rux_modules_name = NULL;
		::rux::delete_object< ::rux::threading::RdWrLock >( _cs_first_last_memory );
		_cs_first_last_memory= NULL;
		::rux::delete_object< ::rux::threading::RdWrLock >( _rux_cs_exception_events );
		_rux_cs_exception_events = NULL;
		::rux::delete_object< ::rux::threading::RdWrLock >( _cs_cpu_logs );
		_cs_cpu_logs = NULL;
		::rux::delete_object< XCrtSect >( ::rux::gui::engine::_cs_gl_library );
		::rux::gui::engine::_cs_gl_library = NULL;
		::rux::delete_object< XCrtSect >( ::rux::gui::engine::_cs_rux_fonts );
		::rux::gui::engine::_cs_rux_fonts = NULL;
		::rux::delete_object< XCrtSect >( ::rux::gui::engine::_cs_font_bases );
		::rux::gui::engine::_cs_font_bases = NULL;
		::rux::delete_object< XCrtSect >( ::rux::gui::engine::_cs_engine );
		::rux::gui::engine::_cs_engine = NULL;
		::rux::delete_object< XCrtSect >( ::rux::gui::engine::_cs_windows );
		::rux::gui::engine::_cs_windows = NULL;
		::rux::delete_object< XCrtSect >( ::rux::gui::engine::_cs_dispatchers );
		::rux::gui::engine::_cs_dispatchers = NULL;
	#if defined( __UNIX__ )
	#ifdef __ANDROID__
	#else
	#endif	
		::rux::delete_object< XCrtSect >( ::rux::gui::engine::_cs_x_display_main );
		::rux::gui::engine::_cs_x_display_main = NULL;
	#endif
		return 1;
	}
	else
	{
		::rux::engine::_globals->_engine_post_deinitialize_after_stop = 1;
		return 0;
	}
};
void start( void* )
{
	::rux::engine::_globals->_engine_started = 1;
	{
		//LAZY_PERFORMANCE;
		declare_stack_variable( char , conf , 2048 );
		::rux::safe_strncpy( conf , ::rux::engine::_globals->_executable_directory , 2047 );
		chdir( conf );
		::rux::safe_strncat( conf , "/rux.conf" , 2047 );
		FILE* file = fopen( conf , "w" );
		if( file )
		{
			fprintf( file , "catch_exception = %s\n" , _rux_catch_exception == 1 ? "true" : "false" );
			fprintf( file , "detail_memory_leak = %s\n" , _rux_is_memory_leaks == 1 ? "true" : "false" );
			fprintf( file , "core_dump = %s\n" , rux::engine::_globals->_is_core_dump == 1 ? "true" : "false" );
			fprintf( file , "leak_watch_to_class = \"%s\"\n" , _leak_watch_to_class );
			if( ::rux::engine::_globals->_gui_globals && ::rux::engine::_globals->_gui_globals->_debug_utf8_control_names )
			{
				std::string debug_utf8_control_names_string;
				for( size_t index0 = 0 ; index0 < ::rux::engine::_globals->_gui_globals->_debug_utf8_control_names->Count() ; index0++ )
				{
					if( debug_utf8_control_names_string.length() )
						debug_utf8_control_names_string += ";";
					char uint32_string[ 1024 ] = { 0 };
					::rux::string::uint32_to_string( ::rux::engine::_globals->_gui_globals->_debug_utf8_control_names->operator[]( index0 ) , uint32_string );
					debug_utf8_control_names_string += uint32_string;
				}
				fprintf( file , "debug_utf8_control_names = \"%s\"\n" , debug_utf8_control_names_string.c_str() );
			}
			else
				fprintf( file , "debug_utf8_control_names = \"%s\"\n" , "" );

			if( ::rux::engine::_globals->_protect_markers_files && ::rux::engine::_globals->_protect_markers_lines )
			{
				std::string file_and_line_string;
				for( size_t index0 = 0 ; index0 < ::rux::engine::_globals->_protect_markers_files->Count() ; index0++ )
				{
					if( file_and_line_string.length() )
						file_and_line_string += ";";
					char uint32_string[ 1024 ] = { 0 };
					::rux::string::uint32_to_string( ::rux::engine::_globals->_protect_markers_lines->operator[]( index0 ) , uint32_string );
					file_and_line_string += ::rux::engine::_globals->_protect_markers_files->operator[]( index0 );
					file_and_line_string += ":";
					file_and_line_string += uint32_string;
				}
				fprintf( file , "protect_markers_file_and_line = \"%s\"\n" , file_and_line_string.c_str() );
			}
			else
				fprintf( file , "protect_markers_file_and_line = \"%s\"\n" , "" );
			fprintf( file , "diagnostic = %s\n" , ::rux::engine::_diagnostic ? "true" : "false" );
			fprintf( file , "heap_corruption_checker = %s\n" , rux::engine::_globals->_heap_corruption_checker == 1 ? "true" : "false" );

			fprintf( file , "opengl_check_support_framebuffer = %s//OpenGL FrameBuffer for improve display quality\n" , ::rux::gui::engine::_check_support_framebuffer == 1 ? "true" : "false" );
			fprintf( file , "opengl_check_support_arb_texture_rectangle = %s\n" , ::rux::gui::engine::_check_support_arb_texture_rectangle == 1 ? "true" : "false" );
			fprintf( file , "opengl_check_support_old_yv12_to_rgb_shader = %s//OpenGL old YV12 to RGB conversion shader(sometimes better than new YV12 to RGB conversion shader)\n" , ::rux::gui::engine::_check_support_old_yv12_to_rgb_shader == 1 ? "true" : "false" );
			fprintf( file , "opengl_check_support_old_yv12_to_rgb_shader2 = %s//OpenGL old YV12 to RGB conversion shader(sometimes better than new YV12 to RGB conversion shader)\n" , ::rux::gui::engine::_check_support_old_yv12_to_rgb_shader2 == 1 ? "true" : "false" );
			fprintf( file , "opengl_check_support_old_yuyv_to_rgb_shader = %s//OpenGL old YUYV to RGB conversion shader(sometimes better than new YUYV to RGB conversion shader)\n" , ::rux::gui::engine::_check_support_old_yuyv_to_rgb_shader == 1 ? "true" : "false" );
			fprintf( file , "opengl_check_support_yv12_to_rgb_shader = %s//OpenGL new YV12 to RGB conversion shader(sometimes need set 'false' to improve display speed)\n" , ::rux::gui::engine::_check_support_yv12_to_rgb_shader == 1 ? "true" : "false" );
			fprintf( file , "opengl_check_support_yuyv_to_rgb_shader = %s//OpenGL new YUYV to RGB conversion shader(sometimes need set 'false' to improve display speed)\n" , ::rux::gui::engine::_check_support_yuyv_to_rgb_shader == 1 ? "true" : "false" );
			fprintf( file , "opengl_check_support_fxaa = %s//OpenGL smooth shader(sometimes need set 'false' to improve display speed)\n" , ::rux::gui::engine::_check_support_fxaa == 1 ? "true" : "false" );
			fprintf( file , "opengl_vsync = %s//OpenGL vertical sync\n" , ::rux::gui::engine::_vsync == 1 ? "true" : "false" );
			fprintf( file , "opengl_use_gl_finish = %s//OpenGL use glFinish command\n" , ::rux::gui::engine::_use_gl_finish == 1 ? "true" : "false" );
			fprintf( file , "opengl_check_support_vbo = %s\n" , ::rux::gui::engine::_check_support_vbo == 1 ? "true" : "false" );
			fprintf( file , "opengl_check_gl_error = %s//just check glGetError(sometimes need set 'false' to improve display speed)\n" , ::rux::gui::engine::_check_gl_error == 1 ? "true" : "false" );
			fprintf( file , "opengl_check_opengl_functions_timeout = %s//OpenGL functions timeout measurement\n" , ::rux::gui::engine::_check_opengl_functions_timeout == 1 ? "true" : "false" );
			fprintf( file , "opengl_log = %s\n" , rux::gui::engine::_log_enable == 1 ? "true" : "false" );
			fprintf( file , "opengl_render = %s\n" , ::rux::engine::_globals->_opengl_render == 1 ? "true" : "false" );
			fprintf( file , "opengl_one_thread_swap_buffers = %s\n" , rux::gui::engine::_opengl_one_thread_swap_buffers == 1 ? "true" : "false" );

			fprintf( file , "winapi_trace = %s\n" , rux::gui::engine::_winapi_trace == 1 ? "true" : "false" );

			fprintf( file , "diagnostics_timeout = %u\n" , ::rux::engine::_diagnostics_timeout );
			fprintf( file , "diagnostics_cpu = %s//diagnostic CPU measurement(sometimes need set 'false' to improve display speed)\n" , rux::engine::_diagnostics_cpu == 1 ? "true" : "false" );
			

			if( ::rux::engine::_globals->_log_level == ::rux::log::XEnum_LogLevel_Debug )
				fprintf( file , "log_level = \"debug\"\n" );
			else if( ::rux::engine::_globals->_log_level == ::rux::log::XEnum_LogLevel_Error )
				fprintf( file , "log_level = \"error\"\n" );
			else if( ::rux::engine::_globals->_log_level == ::rux::log::XEnum_LogLevel_Warn )
				fprintf( file , "log_level = \"warn\"\n" );
			else if( ::rux::engine::_globals->_log_level == ::rux::log::XEnum_LogLevel_Trace )
				fprintf( file , "log_level = \"trace\"\n" );
			else if( ::rux::engine::_globals->_log_level == ::rux::log::XEnum_LogLevel_Verbose )
				fprintf( file , "log_level = \"verbose\"\n" );

			fprintf( file , "symsrv = \"%s\"\n" , ::rux::engine::_globals->_symsrv );

			fclose( file );
		}
	}
	{
		rux::engine::_globals->_rux_stop_threads = rux_stop_threads;
		
		rux::engine::_globals->_add_locked_crt_sect = XThreadInfo::add_locked_crt_sect;
		rux::engine::_globals->_remove_locked_crt_sect = XThreadInfo::remove_locked_crt_sect;
		rux::engine::_globals->_add_write_locked_rdwrlock = XThreadInfo::add_write_locked_rdwrlock;
		rux::engine::_globals->_remove_write_locked_rdwrlock = XThreadInfo::remove_write_locked_rdwrlock;
		rux::engine::_globals->_add_read_locked_rdwrlock = XThreadInfo::add_read_locked_rdwrlock;
		rux::engine::_globals->_remove_read_locked_rdwrlock = XThreadInfo::remove_read_locked_rdwrlock;

		rux::engine::_globals->_rux_free_all_threads = XThreadInfo::free_all_threads;		
		rux::engine::_globals->_rux_add_thread_to_global = XThreadInfo::add_thread_to_global;
		rux::engine::_globals->_rux_remove_thread_from_global = XThreadInfo::remove_thread_from_global;
		rux::engine::_globals->_rux_get_all_threads = XThreadInfo::get_all_threads;
		rux::engine::_globals->_get_memory_extended_info = XMemoryManager::get_memory_extended_info;
		rux::engine::_globals->_rux_add_thread_id_to_global = XThreadInfo::add_thread_id_to_global;
		rux::engine::_globals->_rux_add_gc_ref = XThreadInfo::AddGCRef;
		rux::engine::_globals->_rux_gc_thread_collect = rux_gc_thread_collect;
		
		rux::engine::_globals->_rux_add_search_directory = XMemoryManager::add_search_directory;
		rux::engine::_globals->_rux_get_search_directory_count = XMemoryManager::get_search_directory_count;
		rux::engine::_globals->_rux_get_search_directory = XMemoryManager::get_search_directory;
		
		rux::engine::_globals->_rux_memory_unload_module = XMemoryManager::memory_unload_module;
		rux::engine::_globals->_rux_memory_register_exception_event = rux_memory_register_exception_event;
		rux::engine::_globals->_rux_memory_unregister_exception_event = rux_memory_unregister_exception_event;
		rux::engine::_globals->_rux_memory_network_init = rux_memory_network_init;
		rux::engine::_globals->_rux_memory_network_deinit = rux_memory_network_deinit;
		rux::engine::_globals->_rux_memory_reregister_handler = rux_memory_reregister_handler;
		rux::engine::_globals->_rux_memory_add_label = XThreadInfo::add_label;
		rux::engine::_globals->_get_thread_index = XThreadInfo::get_thread_index;
		rux::engine::_globals->_set_thread_force_cpu = XThreadInfo::set_thread_force_cpu;
		rux::engine::_globals->_rux_memory_remove_label = XThreadInfo::remove_label;
		rux::engine::_globals->_rux_memory_add_descriptor = rux_memory_add_descriptor;
		rux::engine::_globals->_rux_memory_remove_descriptor = rux_memory_remove_descriptor;
		rux::engine::_globals->_loader_globals->_rux_set_conf_property = rux_set_conf_property;
		
		rux::engine::_globals->_register_constructor = rux_type_register_constructor;
		rux::engine::_globals->_register_property = rux_type_register_property;
		rux::engine::_globals->_register_function_1 = register_function_1;
		rux::engine::_globals->_loader_globals->_rux_load_module = ModuleManager::LoadModule;
		rux::engine::_globals->_loader_globals->_rux_add_exclude_directory = ModuleManager::AddExcludeDirectory;
		rux::engine::_globals->_loader_globals->_rux_loader_unload_loadable_module = ModuleManager::loader_unload_loadable_module;
		rux::engine::_globals->_loader_globals->_rux_unload_module = ModuleManager::UnLoadModule;
		rux::engine::_globals->_loader_globals->_rux_unload_module_by_name = ModuleManager::UnLoadModule;
		rux::engine::_globals->_loader_globals->_rux_loadable_increment = rux_loadable_increment;
		rux::engine::_globals->_loader_globals->_rux_loadable_decrement = rux_loadable_decrement;
		rux::engine::_globals->_loader_globals->_rux_add_module_method_info = rux_add_module_method_info;
		rux::engine::_globals->_loader_globals->_rux_get_loaded_modules = ModuleManager::get_LoadedModules;		

		rux::engine::_globals->_rux_typeof = XTypeManager::TypeOf;

		rux::engine::_globals->_gui_globals->_rux_gui_start = rux::gui::engine::start;
		rux::engine::_globals->_gui_globals->_rux_gui_stop = rux::gui::engine::stop;
		rux::engine::_globals->_gui_globals->_rux_gui_create_window = rux::gui::engine::create_window;
		rux::engine::_globals->_gui_globals->_rux_gui_windows_count = rux::gui::engine::windows_count;
		rux::engine::_globals->_gui_globals->_rux_gui_destroy_window = rux::gui::engine::destroy_window;
		rux::engine::_globals->_gui_globals->_rux_gui_create_font = rux::gui::engine::create_font;
		rux::engine::_globals->_gui_globals->_rux_gui_add_cache = rux::gui::engine::add_cache;
		rux::engine::_globals->_gui_globals->_rux_gui_dispatch = rux::gui::engine::dispatch;
		rux::engine::_globals->_gui_globals->_gui_add_schedule = rux::gui::engine::add_schedule;
		rux::engine::_globals->_gui_globals->_gui_remove_schedule = rux::gui::engine::remove_schedule;
		rux::engine::_globals->_gui_globals->_rux_gui_close_all_windows = rux::gui::engine::close_all_windows;
		rux::engine::_globals->_gui_globals->_rux_gui_get_screens = rux::gui::engine::get_screens;
		rux::engine::_globals->_gui_globals->_rux_gui_free_screens = rux::gui::engine::free_screens;
		rux::engine::_globals->_gui_globals->_rux_gui_get_message_processing_thread_id = rux_gui_get_message_processing_thread_id;
		rux::engine::_globals->_gui_globals->_rux_gui_pump_message = rux_gui_pump_message;
	}
	{
		::rux::safe_strncpy( _rux_cpu_filename , ::rux::engine::_globals->_executable_directory , 1023 );
		rux_concatenation( _rux_cpu_filename , "/cpu/" );

		if(::rux::engine::_diagnostic)
		{
			::rux::io::directory::create(_rux_cpu_filename);
			::chmod(_rux_cpu_filename, 0777);
		}

		rux_get_executable_filename_only( &_rux_cpu_filename[ strlen( _rux_cpu_filename ) ] );
		rux_concatenation( _rux_cpu_filename , ".cpu" );
		rux_remove_file( _rux_cpu_filename );
	}
	{
		g_type_manager = alloc_object_macros( XTypeManager );
		g_type_manager->init();
	}
	{
		g_rux_memory_manager = alloc_object_macros( XMemoryManager );		
	}
	::rux::task::_tasker = alloc_object_macros( ::rux::task::Tasker );
	::rux::engine::_globals->_environment = alloc_object_macros( ::rux::data::XDataObject );
#ifdef __WINDOWS__
	wchar_t* envs = GetEnvironmentStringsW();
	if( envs )
	{		
		wchar_t* ptr = envs;
		while( *ptr != 0 )
		{
			size_t len = wcslen( ptr );
			::rux::XString env_string( ptr );
			env_string.set_ByRef( env_string.ConvertToUTF8() );
			::rux::int32 index = env_string.IndexOf( '=' );
			if( index != -1 )
			{
				::rux::XString env_name , env_value;
				env_name.set_ByRef( env_string.Substring( 0 , index ) );
				env_value.set_ByRef( env_string.Substring( index + 1 ) );
				::rux::engine::_globals->_environment->SetValue( env_name , env_value , 0 );
			}
			ptr += len + 1;
		}
		FreeEnvironmentStringsW( envs );
	}
#else
	int i = 1;
	char* s = *environ;
	for( ; s ; i++ )
	{
		::rux::XString env_string( s );
		::rux::int32 index = env_string.IndexOf( '=' );
		if( index != -1 )
		{
			::rux::XString env_name , env_value;
			env_name.set_ByRef( env_string.Substring( 0 , index ) );
			env_value.set_ByRef( env_string.Substring( index + 1 ) );
			::rux::engine::_globals->_environment->SetValue( env_name , env_value , 0 );
		}
		s = *( environ + i );
	}
#endif
	if( ::rux::engine::_globals->_log_level >= ::rux::log::XEnum_LogLevel_Trace )
	{
		declare_stack_variable( char , working_directory , 1024 );
		rux_get_working_directory( working_directory , 1024 );

		::rux::log::WriteTrace( "os '%s'" , ::rux::engine::_globals->_os );
		::rux::log::WriteTrace( "executable '%s'" , ::rux::engine::_globals->_executable );
		::rux::log::WriteTrace( "executable directory '%s'" , ::rux::engine::_globals->_executable_directory );
		::rux::log::WriteTrace( "working directory '%s'" , working_directory );
		::rux::log::WriteTrace( "hostname '%s'" , ::rux::engine::_globals->_hostname );
		::rux::log::WriteTrace( "engine full name '%s'" , ::rux::engine::_globals->_engine_full_filename );
		::rux::log::WriteTrace( "main lib path '%s'" , ::rux::engine::_globals->_main_lib_path );
		for( size_t index0 = 0 ; index0 < ::rux::engine::_globals->_env_variables_count ; index0++ )
			::rux::log::WriteTrace( "env[ %u ] '%s'" , (::rux::uint32)index0 , ::rux::engine::_globals->_env_variables[ index0 ] );
	}
	{
		g_module_manager = alloc_object_macros( ModuleManager );
	}
	{
		g_rux_memory_manager->start_info_thread();
	}
};
/*end engine*/
rux_system_module_initialize( "rux.engine" , start , stop , pre_initialize , post_deinitialize );
