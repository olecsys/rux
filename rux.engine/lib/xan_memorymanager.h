#ifndef XAN_MEMORYMANAGER_H
#define XAN_MEMORYMANAGER_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_crtsect.h>
#include <xan_malloc_array.h>
#include <xan_debug.h>
#include <xan_memory_helper.h>                                                                    
#include <xan_thread.h>
#include <xan_thread_info.h>
#define RUX_MAX_MODULES_SIZE 1024
#define RUX_CPU_LOGS_COUNT 128

typedef void (*statement_t)(void* udata, unsigned int& lineid, unsigned int label, int __booline__);
typedef unsigned int (*lineid_t)(void* udata, unsigned int label, const char* __boofile__, const char* __boofunction__ 
	, int __booline__);
#define rux_debuginfo_macros_statement( label ) \
{\
	static unsigned int lineid = _lineid(udata, label, __FILE__, __FUNCTION__, __LINE__);\
	_statement(udata, lineid, label, __LINE__);\
}
class XMemoryManager
{
	friend class XThreadInfo;
private:	
	::rux::string _write_locked_rdwrlock_string;
	::rux::string _thread_info_rux_modules_name;
	::rux::string _rux_threads_name;
	::rux::string _file;
	::rux::string _function;
	::rux::string _other_module;
	::rux::string _other_function;
	::rux::string _crtsect_string;
	::rux::string _read_locked_rdwrlock_string;
	::rux::string _stable_info_filename;
	::rux::uint64 _last_diagnostics_time;
	rux::uint8 _delete_pidfile;	
	rux::uint8 _is_network_inited;
#ifdef __UNIX__
	void* _signal_stack;
	void* _program_stack;
#endif
public:		
	size_t _info_task_index;
	XMemoryManager();
	~XMemoryManager();			
	void StopThreads( void );
	static void* Alloc( size_t size , const char* type_name_ptr, const char* file_name_ptr, rux::int32 line , rux::int32 module_index , const char* variable_name );
	static void Free( void* mem_ptr );
	static ::rux::int32 get_memory_module_index( void* memory );
	static size_t get_memory_size( void* memory );
	static void add_thread_to_global( XGCRef* memory , rux::int32 module_index );
	static void memory_unload_module( rux::int32 module_index );
	static void exception_event( rux::int32 tid );
	void register_exception_event( const char* module_name , rux::rux_exception_event_t exception_event );
	void unregister_exception_event( rux::rux_exception_event_t exception_event );
	void network_init( void );
	void network_deinit( void );
	void reregister_handler( void );
#ifdef __WINDOWS__
	static void add_descriptor( rux::int32 module_index , const char* file_name_ptr , HANDLE handle , rux::int32 line );
	static void remove_descriptor( HANDLE handle , rux::int32 module_index );
#elif defined( __UNIX__ )
	static void add_descriptor( rux::int32 module_index , const char* file_name_ptr , rux::int32 handle , rux::int32 line );
	static void remove_descriptor( rux::int32 handle , rux::int32 module_index );
#endif
	static void private_info_thread(void* param, size_t ___rux__thread_index1986, lineid_t _lineid
		, statement_t _statement, void* udata);
	static void add_memory( void* ptr , size_t size , rux::int32 module_index , const char* file_name_ptr , rux::int32 line );
	static void remove_memory( void* ptr , size_t size , rux::int32 module_index );
	void start_info_thread( void );
	friend void stop( void );
	static rux::int32 register_module( const char* module_name );
	static rux::uint32 get_search_directory_count( void );
	static void add_search_directory( const char* directory );
	static void get_search_directory( rux::uint32 index , char directory[ 512 ] );	
	rux::uint8 already_executing_in_current_path( void );	
	static void remove_stack_var( ::rux::memory::begin_memory_chunk* _begin_memory_chunk );
	static void add_stack_var( ::rux::memory::begin_memory_chunk* _begin_memory_chunk , const char* file , ::rux::uint32 line , char* variable_name , const char* type_name );
	static ::rux::memory::begin_memory_chunk* find_begin_memory_chunk( const void* mem );
	static void check_is_heap_ehough( void );
	static void check_corrupted_memory( void );
	static void write_heap_is_not_enough( rux::int32 module_index );
	static void write_cpu_logs( void );
	void write_diagnostics( char* now_string , ::rux::uint8& is_interrupt );
	static void get_memory_extended_info( void* memory , ::rux::memory::extended_memory_info*& extended_memory_info );
	static ::rux::uint32 add_times33_hash( const char* value_string , size_t value_string_length );
	static void add_or_remove_thread_event( ::rux::byte event_type , ::rux::byte add , ::rux::on_thread_registered_or_unregistered );
	static char* get_time33_hash_value( ::rux::uint32 times33_hash );
};
extern dll_internal XMemoryManager* g_rux_memory_manager;
extern dll_internal ::rux::threading::RdWrLock* _rux_cs_exception_events;
extern dll_internal rux::rux_exception_event_t _rux_exception_events[ 1024 ];
extern dll_internal char _rux_exception_events_module_name[ 1024 ][ 1024 ];
extern dll_internal ::rux::threading::RdWrLock* _cs_rux_threads;
extern dll_internal XThreadInfo** _rux_threads;
extern dll_internal size_t _rux_threads_count;
declare_global_variable( char , _leak_watch_to_class , 32 );
extern dll_internal rux::uint8 _rux_is_memory_leaks;
namespace rux
{
	namespace engine
	{
		extern dll_internal ::rux::byte _diagnostics_cpu;
		extern dll_internal ::rux::uint32 _diagnostics_timeout;
		extern dll_internal ::rux::byte _diagnostic;
		extern dll_internal size_t _page_size;
	};
};
extern dll_internal rux::uint8 _rux_is_label_delay;
extern dll_internal rux_volatile _rux_old_total_memory_size[ RUX_MAX_MODULES_SIZE ];
extern dll_internal rux_volatile _rux_total_memory_size[ RUX_MAX_MODULES_SIZE ];
extern dll_internal rux_volatile _rux_old_total_descriptors_count[ RUX_MAX_MODULES_SIZE ];
extern dll_internal rux_volatile _rux_total_descriptors_count[ RUX_MAX_MODULES_SIZE ];
extern dll_internal ::rux::threading::RdWrLock* _cs_rux_modules_name;
extern dll_internal char _rux_modules_name[ RUX_MAX_MODULES_SIZE ][ 64 ];
extern dll_internal char _rux_search_directories[ 16 ][ 512 ];
extern dll_internal rux::uint32 _rux_search_directories_count;
extern dll_internal ::rux::threading::RdWrLock* _cs_rux_search_directories;
extern dll_internal rux::uint8 _rux_catch_exception;
declare_global_variable( char , _rux_cpu_filename , 1024 );
extern dll_internal ::rux::threading::RdWrLock* _cs_first_last_memory;
extern dll_internal ::rux::memory::begin_memory_chunk* _first_last_memory;
extern dll_internal XMallocArray< ::rux::memory::summary_memory_info* > _detailed_memory_infos;
extern dll_internal ::rux::int64 _working_set;
extern dll_internal ::rux::int64 _process_threads_count;
extern dll_internal ::rux::int64 _process_descriptors_count;
extern dll_internal ::rux::int64 _virtual_bytes;
extern dll_internal ::rux::threading::RdWrLock* _cs_cpu_logs;
extern dll_internal size_t _cpu_logs_write_index;
extern dll_internal size_t _cpu_logs_read_index;
extern dll_internal char _cpu_logs[ RUX_CPU_LOGS_COUNT ][ 1024 ];
#endif
