#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_engine_globals.h"
#include "xan_gui_defines.h"
#include "xan_loader_defines.h"
#include "xan_dataobject.h"
namespace rux
{
	namespace gui
	{
		namespace engine
		{
			globals::globals( void )
			{
				_rux_gui_start = 0;
				_rux_gui_stop = 0;
				_rux_gui_create_window = 0;
				_rux_gui_windows_count = 0;
				_rux_gui_destroy_window = 0;
				_rux_gui_create_font = 0;
				_rux_gui_add_cache = 0;
				_rux_gui_dispatch = 0;
				_gui_add_schedule = 0;
				_gui_remove_schedule = 0;
				_rux_gui_close_all_windows = 0;
				_rux_gui_get_screens = 0;
				_rux_gui_free_screens = 0;
				_rux_gui_get_message_processing_thread_id = 0;
				_rux_gui_pump_message = 0;
				_cs_gui_schedule = 0;
				_gui_schedule = 0;
				_debug_utf8_control_names = 0;
			};
			globals::~globals( void )
			{
				if( _debug_utf8_control_names )
					::rux::engine::free_object< XMallocArray< ::rux::uint32 > >( _debug_utf8_control_names );
				if( _cs_gui_schedule )
					::rux::engine::free_object< ::rux::threading::RdWrLock >(_cs_gui_schedule );
				if( _gui_schedule )
					::rux::engine::free_object< XMallocArray< ::rux::gui::engine::schedule* > >( _gui_schedule );
			};
		};
	};
	namespace loader
	{
		namespace engine
		{
			globals::globals( void )
			{
				_rux_load_module = 0;
				_rux_loader_unload_loadable_module = 0;
				_rux_add_exclude_directory = 0;
				_rux_unload_module = 0;
				_rux_set_conf_property = 0;
				_rux_unload_module_by_name = 0;		
				_rux_add_module_method_info = 0;
				_rux_get_loaded_modules = 0;
				_rux_loadable_increment = 0;
				_rux_loadable_decrement = 0;
			};
		};
	};
	namespace engine
	{
		globals::globals( void )
		{
			_alloc = 0;
			_free = 0;
#ifdef __WINDOWS__
			LARGE_INTEGER qpc;
			if( QueryPerformanceFrequency( &qpc ) )
				_query_performance_frequency = qpc.QuadPart;
#endif
			_gui_globals = 0;
			_loader_globals = 0;
			_log_globals = 0;
			
			_add_locked_crt_sect = 0;
			_add_write_locked_rdwrlock = 0;
			_remove_write_locked_rdwrlock = 0;
			_add_read_locked_rdwrlock = 0;
			_remove_read_locked_rdwrlock = 0;
			_get_memory_extended_info = 0;
			_remove_locked_crt_sect = 0;
			_rux_fopen = 0;
			_rux_errno = 0;
			_rux_ftell64 = 0;
			_rux_fseek64 = 0;
			_rux_freopen = 0;
			_rux_fwrite = 0;
			_rux_fread = 0;
			_rux_ferror = 0;
			_rux_feof = 0;
			_rux_fputs = 0;
			_vfprintf = 0;
			_rux_fflush = 0;
			_rux_chsize = 0;
			_rux_fclose = 0;
			_rux_memory_free = 0;
			_rux_memory_init = 0;
			_rux_add_thread_to_global = 0;
			_rux_remove_thread_from_global = 0;
			_rux_get_all_threads = 0;
			_rux_add_search_directory = 0;
			_rux_already_executing_in_current_path = 0;
			_rux_get_search_directory_count = 0;
			_rux_get_search_directory = 0;
			_rux_free_all_threads = 0;
			_rux_memory_unload_module = 0;
			_rux_get_memory_size = 0;
			_rux_stop_threads = 0;
			_add_stack_var = 0;
			_remove_stack_var = 0;
			_find_begin_memory_chunk = 0;
			_rux_memory_register_exception_event = 0;
			_rux_memory_unregister_exception_event = 0;
			_rux_memory_network_init = 0;
			_rux_memory_register_module = 0;
			_rux_memory_network_deinit = 0;
			_rux_memory_reregister_handler = 0;
			_rux_memory_add_label = 0;
			_get_thread_index = 0;
			_set_thread_force_cpu = 0;
			_rux_memory_remove_label = 0;
			_rux_memory_add_descriptor = 0;
			_rux_memory_remove_descriptor = 0;
			_rux_add_thread_id_to_global = 0;
			_rux_typeof = 0;
			_register_constructor = 0;
			_register_property = 0;
			_register_function_1 = 0;
			_add_task = 0;
			_add_times33_hash = 0;
			_add_or_remove_thread_event = 0;
			_remove_task = 0;
			_force_execute_task = 0;
			_task_add_event_handler = 0;
			_task_remove_event_handler = 0;
			_rux_add_gc_ref = 0;
			_rux_gc_thread_collect = 0;
			_env = 0;
			_os[ 0 ] = 0;
			_hostname[ 0 ] = 0;
			_neficl_filename[ 0 ] = 0;
			_ruxiver_filename[ 0 ] = 0;
			_executable[ 0 ] = 0;
			_executable_directory[ 0 ] = 0;
			_heap_corruption_checker = 0;
			_is_core_dump = 0;
			_symsrv[ 0 ] = 0;
			_diagnostic = 1;
			_engine_full_filename[ 0 ] = 0;
			_main_lib_path[ 0 ] = 0;
			_env_variables = 0;
			_env_variables_count = 0;
			_engine_started = 0;
			_engine_post_deinitialize_after_stop = 0;
			_opengl_render = 1;
			_environment = 0;
#ifdef __WINDOWS__
			_timer_res = UINT_MAX;
#endif
			_exception_event = 0;
			_exception_time = 0ULL;
#ifdef __WINDOWS__
			_is_call_stack_initialized = 0;
			_current_process_handle = 0;
#else
			_alarm_is_addr2line = 0;
#endif
			_exception_raised = 0;
			_exception_tid = 0;
			_protect_markers_files = 0;
			_protect_markers_lines = 0;
			_end = (unsigned char*)&_end + sizeof(_end);
		};
		globals::~globals( void )
		{
			if( _protect_markers_files )
			{
				for( size_t index0 = 0 ; index0 < _protect_markers_files->Count() ; index0++ )
					::rux::engine::free_mem( _protect_markers_files->operator[]( index0 ) );
				::rux::engine::free_object< XMallocArray< char* > >( _protect_markers_files );
			}
			if( _protect_markers_lines )
				::rux::engine::free_object< XMallocArray< ::rux::uint32 > >( _protect_markers_lines );
			std::map< ::rux::uint32 , char* >::iterator times33_hashes_iter;
			WRITE_LOCK( _cs_times33_hashes );
			for( times33_hashes_iter = _times33_hashes.begin() ; times33_hashes_iter != _times33_hashes.end() ; times33_hashes_iter++ )
				::rux::engine::free_mem( times33_hashes_iter->second );
			_cs_times33_hashes.WriteUnlock();
			if( _environment )
				::rux::engine::free_object< ::rux::data::XDataObject >( _environment );
			if( _env )
				::rux::engine::free_mem( _env );
			if( _env_variables )
				::rux::engine::free_mem( _env_variables );
		};
		::rux::uint32 globals::add_times33_hash( const char* value_string , size_t value_string_length )
		{
			return _add_times33_hash( value_string , value_string_length );
		};
		char* globals::get_time33_hash_value( ::rux::uint32 times33_hash )
		{
			return _get_time33_hash_value(times33_hash);
		}
		void globals::add_thread_registered_event( ::rux::on_thread_registered_or_unregistered on_thread_registered )
		{
			_add_or_remove_thread_event( ::rux::engine::XEnum_EngineEvent_RegisteredThread , 1 , on_thread_registered );
		};
		void globals::remove_thread_registered_event( ::rux::on_thread_registered_or_unregistered on_thread_registered )
		{
			_add_or_remove_thread_event( ::rux::engine::XEnum_EngineEvent_RegisteredThread , 0 , on_thread_registered );
		};
		void globals::add_thread_unregistered_event( ::rux::on_thread_registered_or_unregistered on_thread_unregistered )
		{
			_add_or_remove_thread_event( ::rux::engine::XEnum_EngineEvent_UnregisteredThread , 1 , on_thread_unregistered );
		};
		void globals::remove_thread_unregistered_event( ::rux::on_thread_registered_or_unregistered on_thread_unregistered )
		{
			_add_or_remove_thread_event( ::rux::engine::XEnum_EngineEvent_UnregisteredThread , 0 , on_thread_unregistered );
		};
		void globals::add_before_load_module_event( ::rux::on_thread_registered_or_unregistered on_engine_event )
		{
			_add_or_remove_thread_event( ::rux::engine::XEnum_EngineEvent_BeforeLoadModule , 1 , on_engine_event );
		};
		void globals::add_after_load_module_event( ::rux::on_thread_registered_or_unregistered on_engine_event )
		{
			_add_or_remove_thread_event( ::rux::engine::XEnum_EngineEvent_AfterLoadModule , 1 , on_engine_event );
		};
		void globals::add_before_module_dll_init_event( ::rux::on_thread_registered_or_unregistered on_engine_event )
		{
			_add_or_remove_thread_event( ::rux::engine::XEnum_EngineEvent_BeforeModuleDllInit , 1 , on_engine_event );
		};
		void globals::add_after_module_dll_init_event( ::rux::on_thread_registered_or_unregistered on_engine_event )
		{
			_add_or_remove_thread_event( ::rux::engine::XEnum_EngineEvent_AfterModuleDllInit , 1 , on_engine_event );
		};
		void globals::remove_before_load_module_event( ::rux::on_thread_registered_or_unregistered on_engine_event )
		{
			_add_or_remove_thread_event( ::rux::engine::XEnum_EngineEvent_BeforeLoadModule , 0 , on_engine_event );
		};
		void globals::remove_afterload_module_event( ::rux::on_thread_registered_or_unregistered on_engine_event )
		{
			_add_or_remove_thread_event( ::rux::engine::XEnum_EngineEvent_AfterLoadModule , 0 , on_engine_event );
		};
		void globals::remove_before_module_dll_init_event( ::rux::on_thread_registered_or_unregistered on_engine_event )
		{
			_add_or_remove_thread_event( ::rux::engine::XEnum_EngineEvent_BeforeModuleDllInit , 0 , on_engine_event );
		};
		void globals::remove_after_module_dll_init_event( ::rux::on_thread_registered_or_unregistered on_engine_event )
		{
			_add_or_remove_thread_event( ::rux::engine::XEnum_EngineEvent_AfterModuleDllInit , 0 , on_engine_event );
		};
		bool globals::has_field(void* field_address) {
			return _end > field_address;
		}
		dll_internal void add_thread_registered_event( ::rux::on_thread_registered_or_unregistered on_thread_registered )
		{
			if( ::rux::engine::_globals )
				::rux::engine::_globals->add_thread_registered_event( on_thread_registered );
			else
				::rux::engine::_on_thread_registered = on_thread_registered;
		};
		dll_internal void add_thread_unregistered_event( ::rux::on_thread_registered_or_unregistered on_thread_unregistered )
		{
			if( ::rux::engine::_globals )
				::rux::engine::_globals->add_thread_unregistered_event( on_thread_unregistered );
			else
				::rux::engine::_on_thread_unregistered = on_thread_unregistered;
		};		
		dll_internal ::rux::on_thread_registered_or_unregistered _on_thread_registered = 0;
		dll_internal ::rux::on_thread_registered_or_unregistered _on_thread_unregistered = 0;
	}
};