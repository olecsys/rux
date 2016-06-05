#ifndef XAN_GLOBAL_VARIABLES_H
#define XAN_GLOBAL_VARIABLES_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_typedefs.h>
#include <xan_global_initialization.h>
namespace rux
{
	class XConsole;
};
class XCrtSect;
#ifdef __WINDOWS__
extern dll_internal ::rux::global_initialization _global_initialization;
void dll_internal rux_global_init( void );
void dll_internal rux_global_deinit( void );
#else
void dll_internal rux_global_init( void ) __attribute__ ((constructor(101)));
#ifdef IS_RUX_ENGINE_MODULE
void dll_internal rux_global_deinit( void ) __attribute__ ((destructor(101)));
#else
void dll_internal rux_global_deinit( void ) __attribute__ ((destructor(102)));
#endif
#endif

#ifdef __WINDOWS__
typedef PDH_FUNCTION __stdcall pdh_get_formatted_counter_value_function( PDH_HCOUNTER hCounter, DWORD dwFormat, LPDWORD lpdwType, PPDH_FMT_COUNTERVALUE pValue );
typedef PDH_FUNCTION __stdcall pdh_collect_query_data_function( PDH_HQUERY hQuery );
typedef PDH_FUNCTION __stdcall pdh_add_counter_function( PDH_HQUERY hQuery, LPCWSTR szFullCounterPath, DWORD_PTR dwUserData, PDH_HCOUNTER * phCounter );
typedef PDH_FUNCTION __stdcall pdh_open_query_function( LPCWSTR szDataSource, DWORD_PTR dwUserData, PDH_HQUERY * phQuery );
typedef PDH_FUNCTION __stdcall pdh_make_counter_path_function( PPDH_COUNTER_PATH_ELEMENTS_W pCounterPathElements, LPWSTR szFullPathBuffer, LPDWORD pcchBufferSize, DWORD dwFlags );
typedef PDH_FUNCTION __stdcall pdh_lookup_perf_name_by_index_function( LPCWSTR szMachineName, DWORD dwNameIndex, LPWSTR szNameBuffer, LPDWORD pcchNameBufferSize );
typedef PDH_FUNCTION __stdcall pdh_remove_counter_function( PDH_HCOUNTER hCounter );
typedef PDH_FUNCTION __stdcall pdh_close_query_function( PDH_HQUERY hQuery );
#endif

namespace rux
{
	extern dll_internal XConsole* _rux_console;
	extern dll_internal XCrtSect* _g_cs_rux_console_before_exit_event;
	extern dll_internal before_exit_event_t _g_rux_console_before_exit_event;
	extern dll_internal rux_start_event_t _rux_console_start_event;
	namespace debug
	{
		extern dll_internal XCrtSect* _cs;
	};
};
extern dll_internal ::rux::uint8 _rux_is_service;
extern dll_internal ::rux::uint8 _rux_is_debug;
extern dll_internal XCrtSect* _cs_rux_deinitialization_funcs;
extern dll_internal ::rux::uint8 _rux_deinitialization_funcs_count;
extern dll_internal rux_simple_t _rux_deinitialization_funcs[ 16 ];
extern dll_internal XCrtSect* _cs_engine;
extern dll_internal ::rux::pid_t _engine_init_thread_id;
#ifdef __WINDOWS__
extern dll_internal HMODULE _engine_handle;
#elif defined( __UNIX__ )
extern dll_internal void* _engine_handle;
#endif
namespace rux
{
	extern dll_internal rux_simple_t _first_function;
	extern dll_internal rux_simple_t _last_function;
	extern dll_internal rux_simple_t _pre_initialization;
	extern dll_internal rux_simple_with_result_t _post_deinitialization;
	extern dll_internal rux_volatile _current_module_init;
	extern dll_internal rux_volatile _current_module_reference;
	extern dll_internal XCrtSect* _cs_current_module;
	extern dll_internal rux_volatile _current_module_is_locked;
};
namespace rux
{	
	namespace cryptography
	{
		class XCryptography;
		extern dll_internal rux_volatile _inited;
		extern dll_internal XCrtSect* _cs;
		extern dll_internal XCryptography* _cryptography;
	};
};
namespace rux
{
	namespace com
	{
		extern dll_internal rux_volatile _objects_count;
	};
};
extern dll_internal rux_simple_t _rux_native_on_any_exit;
extern dll_internal const char* g_current_module_name;
extern dll_internal ::rux::uint8 _rux_is_memory_manager;
namespace rux
{
	namespace engine
	{
		extern dll_internal rux_volatile _rux_engine_inited;
		extern dll_internal rux_volatile _loaded_engine;
	};
};
#ifdef __WINDOWS__
namespace rux
{
	namespace pdh
	{
		extern dll_internal pdh_lookup_perf_name_by_index_function* _pdh_lookup_perf_name_by_index;
		extern dll_internal pdh_make_counter_path_function* _pdh_make_counter_path;
		extern dll_internal pdh_open_query_function* _pdh_open_query;
		extern dll_internal pdh_add_counter_function* _pdh_add_counter;
		extern dll_internal pdh_collect_query_data_function* _pdh_collect_query_data;
		extern dll_internal pdh_get_formatted_counter_value_function* _pdh_get_formatted_counter_value;
		extern dll_internal pdh_remove_counter_function* _pdh_remove_counter;
		extern dll_internal pdh_close_query_function* _pdh_close_query;
		extern dll_internal HMODULE _handle;
		extern dll_internal XCrtSect* _cs;
		extern dll_internal rux_volatile _inited;
	};
};
#endif
namespace rux
{
	namespace gui
	{
		class Window;
		enum application_event_type
		{
			application_event_type_initialized ,
			application_event_type_deinitialized
		};
		typedef void (*application_event_t)( application_event_type application_event );
		extern dll_internal ::rux::uint8 _show;
		extern dll_internal Window* _main_window;
		extern dll_internal ::rux::uint8 _is_application_initialized;		
		extern dll_internal XCrtSect _cs_application;
		extern dll_internal application_event_t _on_event;
	};
};
#endif