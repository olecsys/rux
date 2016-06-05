#ifndef XAN_TYPEDEFS_H
#define XAN_TYPEDEFS_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_includes.h>
namespace rux
{
	class XConsole;
};
enum XThreadMode
{
	XThreadMode_OneTime,
	XThreadMode_Repeat
};
typedef void (* XTHREADFUNC)( void* , size_t );
typedef ::rux::uint8 (*rux_install_software_t)( const char* install_path , char error[ 1024 ] );
typedef ::rux::uint8 (*rux_uninstall_software_t)( char error[ 1024 ] );
typedef ::rux::uint8 (*rux_update_software_t)( char error[ 1024 ] );
typedef void (*rux_init_install_ui_t)( ::rux::uint8 is_installed , rux_install_software_t rux_install_software , rux_uninstall_software_t rux_uninstall_software , rux_update_software_t rux_update_software );
typedef void (*rux_init_uninstall_ui_t)( rux_uninstall_software_t rux_uninstall_software );
typedef void (*before_exit_event_t)( void );
typedef int (*DES_set_key_t)( const_DES_cblock* key , DES_key_schedule* schedule );
typedef void (*ERR_clear_error_t)(void );
typedef void (*DES_ecb_encrypt_t)( const_DES_cblock* input , DES_cblock* output , DES_key_schedule* ks , int enc );
typedef unsigned char* (*MD4_t)(const unsigned char *d, size_t n, unsigned char *md);
typedef void (*DES_set_odd_parity_t)(DES_cblock *key);
typedef ::rux::uint8 (*rux_start_event_t)( int argc , char ** args );
typedef void (*LPXPROCESSFN)( char* , ::rux::XConsole* );
typedef void (*exception_event_t)( void );
class XGCRef;
class Type;
class XCrtSect;
namespace rux
{	
	namespace task
	{
		enum event_handler_type
		{
			event_handler_type_wait_for_exit
		};
		struct event
		{
			::rux::byte _wait_for_exit;
			size_t _task_index;
			XTHREADFUNC _func;
			void* _param;
		};
		typedef void (*event_handler_t)( ::rux::task::event* event );
		typedef size_t (*add_event_handler_t)( ::rux::task::event_handler_type event_handler_type , ::rux::task::event_handler_t event_handler );
		typedef void (*remove_event_handler_t)( size_t event_handler_index );
	};
	namespace threading
	{
		class Thread;
	};
	typedef void (*on_thread_registered_or_unregistered)( ::rux::pid_t tid , ::rux::threading::Thread* thread , size_t ___rux__thread_index1986 );
	namespace threading
	{
		class RdWrLock;
	};
	namespace engine
	{
		class globals;
	};
	namespace memory
	{
		struct extended_memory_info;
	};
	typedef void (*add_task_t)( size_t& task_index , XTHREADFUNC func , void* param , ::rux::uint32 timeout_in_milliseconds , XThreadMode mode );
	typedef ::rux::uint32 (*add_times33_hash_t)( const char* value_string , size_t value_string_length );
	typedef void (*add_or_remove_thread_event_t)( ::rux::byte event_type , ::rux::byte add , ::rux::on_thread_registered_or_unregistered );
	typedef void (*remove_task_t)( size_t& task_index );
	typedef void (*force_execute_task_t)( size_t task_index );
	typedef void (*get_memory_extended_info_t)( void* memory , ::rux::memory::extended_memory_info*& extended_memory_info );
	typedef void (*add_locked_crt_sect_t)( size_t thread_index , XCrtSect* crt_sect , size_t& index , const char* file , ::rux::uint32 line , ::rux::int32 module_index );
	typedef void (*remove_locked_crt_sect_t)( size_t thread_index , XCrtSect* crt_sect , size_t& index , ::rux::byte release );
	typedef void (*add_write_locked_rdwrlock_t)( size_t thread_index , ::rux::threading::RdWrLock* rdwrlock , size_t& index , const char* file , ::rux::uint32 line , ::rux::int32 module_index );
	typedef void (*remove_write_locked_rdwrlock_t)( size_t thread_index , ::rux::threading::RdWrLock* rdwrlock , size_t& index , ::rux::byte release );
	typedef ::rux::int32 (* rux_module_init_t)(void*);
	typedef void (* rux_gc_ref_t)( ::rux::int32 module_index , const XGCRef* gc_ref_ptr );
	typedef void (*rux_gc_thread_collect_t)( size_t thread_index );
	typedef ::rux::int32 (* rux_module_free_t)( void );
	typedef void (* rux_loader_module_free_t)( ::rux::uint8 );
	typedef void (* is_rux_module_t)();
	typedef void (*register_type_t)( Type* type );
	typedef void* (*malloc_t)( size_t size );
	typedef void (*free_t)( void* ptr );
	typedef void (*register_constructor_t)( Type* type , void* ctor_address );
	typedef void (*register_property_t)( Type* type , const char* name , Type* param_type , void* set_property_address , void* get_property_address );
	typedef void (*register_function_1_t)( Type* type , ::rux::uint8 with_return , const char* name , void* function_address , Type* param_type0 );
	typedef Type* (*typeof_t)(::rux::int32 module_index , const char* class_name , rux_volatile* rux_type_index , ::rux::register_type_t register_type , ::rux::uint8 is_array , Type* item_type );
	typedef void (*free_types_t)(::rux::int32 module_index);
	typedef ::rux::engine::globals* (*rux_get_engine_globals_t)( void );
	namespace diagnostics
	{
		typedef void (*process_stdout_or_stderr_t)( void* udata , const char* data , size_t data_size );
	};
	namespace memory
	{
		class begin_memory_chunk;
		class end_memory_chunk;
	};
	typedef FILE* (*rux_fopen_t)( const char * _Filename, const char * _Mode );
	typedef ::rux::int32 (*rux_errno_t)( void );
	typedef ::rux::int32 (*rux_fclose_t)( FILE * _File );
	typedef ::rux::int64 (*ftell64_t)( FILE * _File );
	typedef ::rux::int32 (*fseek64_t)( FILE* _File , ::rux::int64 _Offset , ::rux::int32 _Origin );
	typedef FILE* (*freopen_t)( const char * _Filename , const char * _Mode , FILE* _File );
	typedef size_t (*fwrite_t)( const void* _Str , size_t _Size , size_t _Count , FILE* _File );
	typedef size_t (*fread_t)( void* _DstBuf , size_t _ElementSize , size_t _Count, FILE* _File );
	typedef ::rux::int32 (*ferror_t)( FILE* _File );
	typedef ::rux::int32 (*feof_t)( FILE* _File );
	typedef ::rux::int32 (*fputs_t)( const char* _Str, FILE* _File );
	typedef int (*vfprintf_t)( FILE* stream , const char* format , va_list arg );
	typedef ::rux::int32 (*fflush_t)( FILE* _File );
	typedef ::rux::int32 (*chsize_t)( ::rux::int32 _FileHandle, long _Size );
	typedef void* (* alloc_t)(size_t size);
	typedef size_t (* rux_add_thread_to_global_t)( XGCRef* thread , ::rux::int32 module_index );
	typedef void (* rux_memory_set_is_kill_child_t)( ::rux::uint8 value );
	typedef void (* rux_remove_thread_from_global_t)( size_t index);
	typedef XMallocArray< XGCRef* >* (* rux_get_all_threads_t)(void);
	typedef ::rux::uint8 (*rux_already_executing_in_current_path_t)( void );
	typedef void (*rux_add_search_directory_t)( const char* );
	typedef ::rux::uint32 (*rux_get_search_directory_count_t)( void );
	typedef void (*rux_get_search_directory_t)( ::rux::uint32 , char[ 512 ] );
	typedef void (*rux_free_all_threads_t)( XMallocArray< XGCRef* >* threads );
	typedef void (*rux_memory_unload_module_t)( ::rux::int32 );
	typedef size_t (*get_memory_size_t)( void* memory );
	typedef void (*stop_threads_t)( void );
	typedef void (*add_stack_var_t)( ::rux::memory::begin_memory_chunk* _begin_memory_chunk , const char* file , ::rux::uint32 line , char* variable_name , const char* type_name );
	typedef void (*remove_stack_var_t)( ::rux::memory::begin_memory_chunk* _begin_memory_chunk );
	typedef ::rux::memory::begin_memory_chunk* (*find_begin_memory_chunk_t)( const void* mem );
	typedef void (*rux_exception_event_t)( ::rux::int32 );
	typedef void (*rux_memory_register_exception_event_t)( const char* module_name , ::rux::rux_exception_event_t exception_event );
	typedef void (*rux_memory_unregister_exception_event_t)( ::rux::rux_exception_event_t exception_event );
	typedef void (*rux_memory_network_init_t)( void );
	typedef ::rux::int32 (*rux_memory_register_module_t)( const char* module_name );
	typedef void (*rux_memory_network_deinit_t)( void );
	typedef void (*rux_memory_reregister_handler_t)( void );
	typedef void (*add_label_t)( size_t thread_index , const char* filename , const char* function , ::rux::int32 line , const char* other_module , const char* other_function , ::rux::int32 module_index , ::rux::uint8 check_delay , ::rux::uint32 max_delay );
	typedef size_t (*get_thread_index_t)( ::rux::int32 module_index );
	typedef void (*set_thread_force_cpu_t)( size_t thread_index , ::rux::byte force_cpu );
	typedef void (*rux_memory_remove_label_t)( size_t thread_index );
	typedef ::rux::pid_t (*rux_gui_get_message_processing_thread_id_t)( void );
	typedef void (*rux_gui_pump_message_t)( size_t thread_index );
	#ifdef __WINDOWS__
	typedef void (*rux_memory_add_descriptor_t)( ::rux::int32 module_index , const char* file_name_ptr , void* handle );
	typedef void (*rux_memory_remove_descriptor_t)( void* handle , ::rux::int32 module_index );
	#elif defined( __UNIX__ )
	typedef void (*rux_memory_add_descriptor_t)( ::rux::int32 module_index , const char* file_name_ptr , ::rux::int32 handle );
	typedef void (*rux_memory_remove_descriptor_t)( ::rux::int32 handle , ::rux::int32 module_index );
	#endif
	typedef size_t (*rux_add_thread_id_to_global_t)( ::rux::pid_t thread_id , const char* thread_name , ::rux::int32 module_index , ::rux::byte );
	namespace threading
	{
		typedef void (*thread_before_exit_t)( void* );
	};
};
typedef void (* rux_simple_t)( void );
typedef ::rux::byte (* rux_simple_with_result_t)( void );
typedef rux::uint8 (*rux_service_task_t)( int argc , char ** args );
typedef void (*rux_service_stop_t)( void );
#endif