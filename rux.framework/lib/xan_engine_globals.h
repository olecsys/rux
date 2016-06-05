#ifndef XAN_ENGINE_SETTINGS_H
#define XAN_ENGINE_SETTINGS_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_typedefs.h>
#include <xan_wtsapi32_module.h>
#include <xan_Iphlpapi_module.h>
#include <xan_ole32_module.h>
#include <xan_oleaut32_module.h>
#include <xan_psapi_module.h>
#include <xan_user32_module.h>
#include <xan_userenv32_module.h>
#include <xan_winmm_module.h>
#include <xan_ws2_32_module.h>
#include <xan_x11_module.h>
#include <xan_xrandr_module.h>
#include <xan_dbghelp_module.h>
#include <xan_symsrv_module.h>
#include <xan_kernel32_module.h>
#include <xan_enums.h>
#include <xan_event_handler.h>
namespace rux
{
	namespace data
	{
		class XDataObject;
	};
	namespace gui
	{
		namespace engine
		{
			class globals;
		};
	};
	namespace loader
	{
		namespace engine
		{
			class globals;
		};
	};
	namespace log
	{
		namespace engine
		{
			class globals;
		};
	};
	namespace service
	{
		namespace engine
		{
			class globals;
		};
	};
	namespace engine
	{
		dll_internal void add_thread_registered_event( ::rux::on_thread_registered_or_unregistered on_thread_registered );
		dll_internal void add_thread_unregistered_event( ::rux::on_thread_registered_or_unregistered on_thread_unregistered );
		class globals
		{
		public:
			globals( void );
			~globals( void );
			::rux::gui::engine::globals* _gui_globals;
			::rux::loader::engine::globals* _loader_globals;
			::rux::log::engine::globals* _log_globals;
			::rux::service::engine::globals* _service_globals;
#ifdef __WINDOWS__
			::rux::system::kernel32_module _kernel32_module;
			::rux::system::dbghelp_module _dbghelp_module;
			::rux::system::symsrv_module _symsrv_module;
			::rux::system::wtsapi32_module _wtsapi32_module;
			::rux::system::user32_module _user32_module;
			::rux::system::iphlpapi_module _iphlpapi_module;
			::rux::system::ole32_module _ole32_module;
			::rux::system::psapi_module _psapi_module;
			::rux::system::oleaut32_module _oleaut32_module;
			::rux::system::userenv_module _userenv_module;
			::rux::system::winmm_module _winmm_module;
			::rux::system::ws2_32_module _ws2_32_module;
			UINT _timer_res;
#else
			::rux::system::x11_module _x11_module;
			::rux::system::xrandr_module _xrandr_module;
#endif
			::rux::rux_fopen_t _rux_fopen;
			::rux::rux_errno_t _rux_errno;
			::rux::ftell64_t _rux_ftell64;
			::rux::fseek64_t _rux_fseek64;
			::rux::freopen_t _rux_freopen;
			::rux::fwrite_t _rux_fwrite;
			::rux::fread_t _rux_fread;
			::rux::ferror_t _rux_ferror;
			::rux::feof_t _rux_feof;
			::rux::fputs_t _rux_fputs;
			::rux::vfprintf_t _vfprintf;
			::rux::fflush_t _rux_fflush;
			::rux::chsize_t _rux_chsize;
			::rux::rux_fclose_t _rux_fclose;
			::rux::rux_module_free_t _rux_memory_free;
			::rux::rux_module_init_t _rux_memory_init;
			::rux::rux_add_thread_to_global_t _rux_add_thread_to_global;
			::rux::rux_remove_thread_from_global_t _rux_remove_thread_from_global;
			::rux::rux_get_all_threads_t _rux_get_all_threads;
			::rux::rux_add_search_directory_t _rux_add_search_directory;
			::rux::rux_already_executing_in_current_path_t _rux_already_executing_in_current_path;
			::rux::rux_get_search_directory_count_t _rux_get_search_directory_count;
			::rux::rux_get_search_directory_t _rux_get_search_directory;
			::rux::rux_free_all_threads_t _rux_free_all_threads;
			::rux::rux_memory_unload_module_t _rux_memory_unload_module;
			::rux::get_memory_size_t _rux_get_memory_size;
			::rux::stop_threads_t _rux_stop_threads;
			::rux::add_stack_var_t _add_stack_var;
			::rux::remove_stack_var_t _remove_stack_var;
			::rux::find_begin_memory_chunk_t _find_begin_memory_chunk;
			::rux::rux_memory_register_exception_event_t _rux_memory_register_exception_event;
			::rux::rux_memory_unregister_exception_event_t _rux_memory_unregister_exception_event;
			::rux::rux_memory_network_init_t _rux_memory_network_init;
			::rux::rux_memory_register_module_t _rux_memory_register_module;
			::rux::rux_memory_network_deinit_t _rux_memory_network_deinit;
			::rux::rux_memory_reregister_handler_t _rux_memory_reregister_handler;
			::rux::add_label_t _rux_memory_add_label;
			::rux::get_thread_index_t _get_thread_index;
			::rux::set_thread_force_cpu_t _set_thread_force_cpu;
			::rux::rux_memory_remove_label_t _rux_memory_remove_label;
			::rux::rux_memory_add_descriptor_t _rux_memory_add_descriptor;
			::rux::rux_memory_remove_descriptor_t _rux_memory_remove_descriptor;
			::rux::rux_add_thread_id_to_global_t _rux_add_thread_id_to_global;
			::rux::typeof_t _rux_typeof;
			::rux::register_constructor_t _register_constructor;
			::rux::register_property_t _register_property;
			::rux::register_function_1_t _register_function_1;
			::rux::rux_gc_ref_t _rux_add_gc_ref;
			::rux::rux_gc_thread_collect_t _rux_gc_thread_collect;
			::rux::add_locked_crt_sect_t _add_locked_crt_sect;
			::rux::add_write_locked_rdwrlock_t _add_write_locked_rdwrlock;
			::rux::remove_write_locked_rdwrlock_t _remove_write_locked_rdwrlock;
			::rux::add_write_locked_rdwrlock_t _add_read_locked_rdwrlock;
			::rux::remove_write_locked_rdwrlock_t _remove_read_locked_rdwrlock;
			::rux::get_memory_extended_info_t _get_memory_extended_info;
			::rux::remove_locked_crt_sect_t _remove_locked_crt_sect;
			::rux::add_task_t _add_task;
			::rux::add_times33_hash_t _add_times33_hash;
			::rux::add_or_remove_thread_event_t _add_or_remove_thread_event;
			::rux::remove_task_t _remove_task;
			::rux::force_execute_task_t _force_execute_task;
			::rux::task::add_event_handler_t _task_add_event_handler;
			::rux::task::remove_event_handler_t _task_remove_event_handler;
			::rux::rux_alloc_t _alloc;
			::rux::rux_free_t _free;
#ifdef __WINDOWS__
			::rux::uint64 _query_performance_frequency;
#endif
			char* _env;
			char** _env_variables;
			size_t _env_variables_count;
			char _os[ 128 ];
			char _hostname[ 256 ];
			char _neficl_filename[ 1024 ];
			char _ruxiver_filename[ 1024 ];
			char _executable[ 1024 ];
			char _executable_directory[ 1024 ];
			::rux::uint8 _heap_corruption_checker;
			::rux::uint8 _is_core_dump;
			char _symsrv[ 128 ];
			char _engine_full_filename[ 2048 ];
			char _main_lib_path[ 2048 ];
			::rux::byte _engine_started;
			::rux::byte _engine_post_deinitialize_after_stop;
			::rux::byte _opengl_render;
			::rux::log::XEnum_LogLevel _log_level;
			::rux::data::XDataObject* _environment;
			::rux::rux_exception_event_t _exception_event;
			::rux::uint8 _exception_raised;
			::rux::uint64 _exception_time;
			::rux::uint32 _exception_tid;
#ifdef __WINDOWS__
			::rux::uint8 _is_call_stack_initialized;
			HANDLE _current_process_handle;
#else
			struct sigaction _old_sigactions[ 22 ];
			::rux::byte* _alarm_is_addr2line;
#endif
			::rux::byte _diagnostic;
			::rux::threading::RdWrLock _cs_times33_hashes;
			std::map< ::rux::uint32 , char* > _times33_hashes;
			::rux::EventHandler< ::rux::on_thread_registered_or_unregistered > _on_thread_registered;
			::rux::EventHandler< ::rux::on_thread_registered_or_unregistered > _on_thread_unregistered;
			::rux::EventHandler< ::rux::on_thread_registered_or_unregistered > _on_before_load_module;
			::rux::EventHandler< ::rux::on_thread_registered_or_unregistered > _on_after_load_module;
			::rux::EventHandler< ::rux::on_thread_registered_or_unregistered > _on_before_module_dll_init;
			::rux::EventHandler< ::rux::on_thread_registered_or_unregistered > _on_after_module_dll_init;
			XMallocArray< char* >* _protect_markers_files;
			XMallocArray< ::rux::uint32 >* _protect_markers_lines;
			void add_thread_registered_event( ::rux::on_thread_registered_or_unregistered on_thread_registered );
			void remove_thread_registered_event( ::rux::on_thread_registered_or_unregistered on_thread_registered );
			void add_thread_unregistered_event( ::rux::on_thread_registered_or_unregistered on_thread_unregistered );
			void remove_thread_unregistered_event( ::rux::on_thread_registered_or_unregistered on_thread_unregistered );
			::rux::uint32 add_times33_hash( const char* value_string , size_t value_string_length );
			char* get_time33_hash_value( ::rux::uint32 times33_hash );
			void add_before_load_module_event( ::rux::on_thread_registered_or_unregistered on_engine_event );
			void add_after_load_module_event( ::rux::on_thread_registered_or_unregistered on_engine_event );
			void add_before_module_dll_init_event( ::rux::on_thread_registered_or_unregistered on_engine_event );
			void add_after_module_dll_init_event( ::rux::on_thread_registered_or_unregistered on_engine_event );
			void remove_before_load_module_event( ::rux::on_thread_registered_or_unregistered on_engine_event );
			void remove_afterload_module_event( ::rux::on_thread_registered_or_unregistered on_engine_event );
			void remove_before_module_dll_init_event( ::rux::on_thread_registered_or_unregistered on_engine_event );
			void remove_after_module_dll_init_event( ::rux::on_thread_registered_or_unregistered on_engine_event );
		};
		extern dll_internal ::rux::on_thread_registered_or_unregistered _on_thread_registered;
		extern dll_internal ::rux::on_thread_registered_or_unregistered _on_thread_unregistered;
	};
};
#endif
