#ifndef XAN_DEFINES_H
#define XAN_DEFINES_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_crtsect.h>
#include <xan_interlocked.h>
#include <xan_global_variables.h>
#include <xan_engine_globals.h>

#ifndef UNREFERENCED_PARAMETER
#define UNREFERENCED_PARAMETER(P)          \
    { \
        (P) = (P); \
    }
#endif

#ifdef __WINDOWS__
#define _CRT_SECURE_NO_WARNINGS
#endif
#define MAX_CLASS_NAME 256
#ifdef __cplusplus
#define externc extern "C"
#endif

#ifdef __WINDOWS__
//#ifdef __EXECUTABLE__
//#define dll_public //__declspec( dllimport )
//#else
#define dll_public __declspec( dllexport )
//#endif
#define exports externc __declspec(dllexport)

#ifndef WINVER                          // Specifies that the minimum required platform is Windows Vista.
#define WINVER 0x0600           // Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT            // Specifies that the minimum required platform is Windows Vista.
#define _WIN32_WINNT 0x0600     // Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINDOWS          // Specifies that the minimum required platform is Windows 98.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE                       // Specifies that the minimum required platform is Internet Explorer 7.0.
#define _WIN32_IE 0x0700        // Change this to the appropriate value to target other versions of IE.
#endif

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define PathSlash "\\"
#define max_ulong32 ((::rux::uint32)~((::rux::uint32)0))
#define atoll(String)( _atoi64(String) )
#define mkdir(String)( _mkdir(String) )
#define dynamic_library_extension ".dll"
#define dynamic_library_prefix ""
const char dynamic_library_extension_ptr[] = { '.' , 'd' , 'l' , 'l' };
#ifndef SM_TABLETPC
	#define SM_TABLETPC     86
#endif
#ifndef SM_MEDIACENTER
	#define SM_MEDIACENTER  87
#endif
#endif

#ifdef __UNIX__
#define INVALID_SOCKET -1
#ifdef __LINUX__
#if __GNUC__ >= 4
#define dll_public __attribute__ ((visibility ("default")))
#else
//#ifdef __EXECUTABLE__
//#define dll_public __declspec( dllimport )
//#else
#define dll_public __declspec( dllexport )
//#endif
#endif
#elif defined( __SOLARIS__ )
#define dll_public __global
#endif
#ifdef __SOLARIS__
#define exports externc __symbolic
#else
#define exports externc dll_public
#endif
#ifdef __LINUX__
#define __cdecl
#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE  1
#endif
#elif defined( __SOLARIS__ )
#define __cdecl
#endif
#define _atoi64(String)( atoll(String) )
#define PathSlash "/"
#define _mkdir(String)( mkdir(String) )
#define _UI64_MAX     0xffffffffffffffffll
#define dynamic_library_extension ".so"
#define dynamic_library_prefix "lib"
const char dynamic_library_extension_ptr[] = { '.' , 's' , 'o' };
#ifdef __UNIX__
rux_inline ::rux::pid_t rux_gettid( void )
{
#ifdef __LINUX__
	//return (::rux::pid_t)syscall( __NR_gettid );
	return (::rux::pid_t)syscall( SYS_gettid );
#elif defined( __SOLARIS__ )
		return pthread_self();
#endif
};
#endif
#endif
#define SOCKET_ERROR (-1)

extern dll_internal ::rux::uint8 rux_lock_module( ::rux::uint8 is_code_label = 1 );
extern dll_internal void rux_unlock_module( void );
dll_internal void rux_add_deinitialization_func( rux_simple_t func );
#ifdef __WINDOWS__
#define vsnprintf( str , size , format , ap ) vsprintf_s( str , size , format , ap )
#define snprintf sprintf_s
#define strerror_r( errnum , buf , n ) strerror_s( buf , n , errnum )
#elif defined( __UNIX__ )
#define vsprintf_s( str , size , format , ap ) vsnprintf( str , size , format , ap )
#define sprintf_s snprintf
#define strerror_s( _Buf , _SizeInBytes , _ErrNum ) strerror_r( _ErrNum , _Buf , _SizeInBytes )
#endif
dll_internal void rux_network_init( void );
dll_internal void rux_network_deinit( void );
dll_internal void rux_get_working_directory( char string[] , size_t string_size );
dll_internal ::rux::uint8 rux_start_detached_process( const char* module , const char* arguments , ::rux::uint8 show , rux_start_process_method start_process_method = rux_start_process_method_create_process );
dll_internal void rux_from_big_endian_to_little_endian( ::rux::uint8* source_ptr , ::rux::uint8* destination_ptr , ::rux::uint32 length );
dll_internal ::rux::int64 rux_from_memory_address_to_int64( void* memory_address );
#define RUX_MAX_STACK_FRAMES 30
const ::rux::uint8 _rux_utf8_header[] = { 0xef , 0xbb , 0xbf };
dll_internal char* rux_get_module_directory( void );
#ifdef __WINDOWS__
#define rux_putenv( env_string ) ::_putenv( env_string )
#define rux_open( filename , flags ) ::_open( filename , flags , _S_IREAD | _S_IWRITE )
#define rux_lseek( file , offset , origin ) ::_lseek( file , offset , origin )
#define rux_write( file , bytes , size ) ::_write( file , bytes , size )
#define rux_close( file ) ::_close( file )
#define rux_access( filename , mode ) ::_access( filename , mode )
#define rux_tzset() ::_tzset()
#elif defined( __UNIX__ )
#define rux_putenv( env_string ) ::putenv( env_string )
#define rux_open( filename , flags ) ::open( filename , flags , S_IRUSR | S_IWUSR )
#define rux_lseek( file , offset , origin ) ::lseek( file , offset , origin )
#define rux_write( file , bytes , size ) ::write( file , bytes , size )
#define rux_close( file ) ::close( file )
#define rux_access( filename , mode ) ::access( filename , mode )
#define rux_tzset() ::tzset()
#endif
dll_internal void rux_add_search_directory( const char* directory );
dll_internal void rux_get_search_directories( ::rux::uint32& count , char directories[ 16 ][ 512 ] );
dll_internal void rux_concatenation( char* destination , const char* string1 , const char* string2 );
dll_internal void rux_concatenation( char* destination , const char* string2 );
dll_internal void rux_replace( char* destination , char old_char , char new_char );
dll_internal ::rux::int32 rux_index_of( const char* destination , char letter );
dll_internal ::rux::int32 rux_index_of( const char* destination , const char* to_find );
dll_internal ::rux::int32 rux_last_index_of( const char* destination , char letter );
dll_internal ::rux::uint64 rux_hash( unsigned char* data_ptr, ::rux::int32 data_size );
dll_internal void rux_substring( char* destination , const char* source , ::rux::uint32 start_index , ::rux::uint32 length );
dll_internal ::rux::uint8 rux_is_exists_fifo( const char* filename );
dll_internal ::rux::uint8 rux_is_already_executing_in_current_path( void );
dll_internal ::rux::uint8 rux_is_exists_file_or_link_or_fifo( const char* filename );
dll_internal ::rux::uint8 rux_is_exists_file_or_link( const char* filename );
dll_internal ::rux::uint8 rux_is_exists_file( const char* filename );
dll_internal void rux_split( const char* string , size_t string_size , const char* delimiter , char splits[ 64 ][ 64 ] , size_t& splits_count );
dll_internal void rux_remove_file( const char* filename );
dll_internal void rux_string_remove_all( char* destination , char letter );
dll_internal void rux_clear_and_write_to_file( const char* filename , const char* text );
dll_internal void rux_native_set_on_any_exit( rux_simple_t on_any_exit );
dll_internal void rux_append_to_file( const char* filename , const char* text );
dll_internal void rux_append_to_file( const char* filename , const ::rux::uint8* bytes , size_t bytes_size );
dll_internal void* rux_read_all_from_file( const char* filename , size_t& readen_size );
dll_internal void rux_get_executable_filename( char executable_filename[] );
dll_internal void rux_get_executable_directory( char module_directory[] );
dll_internal ::rux::uint32 rux_days_in_month( ::rux::uint32 month , ::rux::uint32 year );
dll_internal ::rux::uint32 rux_days_in_year( ::rux::uint32 year );
dll_internal ::rux::uint8 rux_get_hostname( char* hostname , size_t hostname_size );
class XGCRef;
class Object;
namespace rux
{
	dll_internal int safe_sprintf( char * str , size_t str_size , const char* format , ... );
	dll_internal int safe_sprintf( char * str , size_t str_size , const char* format );
	dll_internal int _scprintf( const char* format , ... );
	dll_internal int _vscprintf( const char* format , va_list pargs );
	dll_internal struct tm* gmtime_r( const time_t* timep , struct tm* result );
	dll_internal char* arguments_string_to_array( const char* arguments_string , char** arguments_array , size_t& arguments_array_count  );
	dll_internal ::rux::int32 get_NumberPowerOf2( ::rux::int32 number );
	dll_internal bool is_boolean_and_true( XGCRef* object );
	dll_internal ::rux::uint32 get_numeric_rux_type_index( void );
	dll_internal ::rux::uint32 get_float_rux_type_index( void );
	dll_internal ::rux::uint32 get_double_rux_type_index( void );
	dll_internal ::rux::uint32 get_array_object_rux_type_index( void );
	dll_internal ::rux::uint32 get_data_object_rux_type_index( void );
	dll_internal ::rux::uint32 get_int32_rux_type_index( void );
	dll_internal ::rux::uint32 get_group_rux_type_index( void );
	dll_internal ::rux::uint32 get_uint32_rux_type_index( void );
	dll_internal ::rux::uint32 get_int64_rux_type_index( void );
	dll_internal ::rux::uint32 get_uint64_rux_type_index( void );
	dll_internal ::rux::uint64 object_to_uint64( Object* object );
	dll_internal ::rux::uint64 gcref_to_uint64( const XGCRef* object );
	dll_internal ::rux::int32 gcref_to_int32( const XGCRef* object );
	dll_internal float gcref_to_float( const XGCRef* object );
	dll_internal ::rux::byte gcref_to_byte( const XGCRef* object );
	dll_internal Object* uint64_to_object( ::rux::uint64 value );
	dll_internal XGCRef* uint64_to_gcref( ::rux::uint64 value );
	class string
	{
		static const ::rux::uint8 DeleteStr = 0x01;
		static const ::rux::uint8 DeleteObject = 0x02;		
		::rux::uint8 _need_free;
		size_t _length;
	public:
		char* _str;
		string( ::rux::string* string );
		explicit string( const char* str , ::rux::uint8 by_ref = 0 , ::rux::uint8 need_free = 0 );
		~string( void );
		::rux::string& operator +=( const char* str );
		::rux::string& operator +=( const ::rux::string& string );
		friend ::rux::string* operator +( const char* str , const ::rux::string& string );
		friend bool operator ==( const ::rux::string& string , const char* str );
		bool operator ==( const ::rux::string& string );
		size_t length( void ) const;
		void replace_all( char old_char , char new_char );
		void replace_all( const char* old_string , const char* new_string );
		::rux::int64 index_of( const char* find );
		::rux::int64 last_index_of( const char* find );
		void substring( size_t start_index , size_t length );
		void substring( size_t start_index );
		::rux::uint32 to_uint32( void );
		void clear( void );

		static void replace_all( char* destination , char old_char , char new_char );
		static void substring( char* destination , const char* source , size_t start_index , size_t length , size_t source_length = 0 );
		static void substring( char* destination , const char* source , size_t start_index , size_t source_length = 0 );
		static void uint64_to_hex_string( ::rux::uint64 number1 , char string[] );
		static void uint32_to_hex_string( ::rux::uint32 number1 , char string[] );
		static void uint32_to_string( ::rux::uint32 number1 , char string[] );
		static void uint64_to_string( ::rux::uint64 number1 , char string[] );
		static ::rux::uint64 to_uint64( const wchar_t* string , size_t string_size );
		static ::rux::int64 to_int64( const wchar_t* string , size_t string_size );
		static ::rux::uint64 to_uint64( const wchar_t* string );
		static ::rux::int64 to_int64( const wchar_t* string );
		static ::rux::uint64 to_uint64( const char* string , size_t string_size );
		static size_t to_size_t( char string[] , size_t string_size );
		static ::rux::int64 to_int64( const char* string , size_t string_size );
		static ::rux::uint64 to_uint64( const char* string );
		static ::rux::uint32 to_uint32( const char* string );
		static ::rux::int64 to_int64( const char* string );
		static ::rux::uint8 is_utf8( const char* string , size_t length );
		static ::rux::int64 index_of( const char* string , const char* find );
		static ::rux::int64 last_index_of( const char* string , const char* find );
		static ::rux::uint8 hex_string_to_uint64( const char* hex , ::rux::uint64& value , size_t length = 0 );
	};
	namespace threading
	{
#ifdef __WINDOWS__
		dll_internal HANDLE get_current_thread( void );
#else
		dll_internal pthread_t get_current_thread( void );
#endif
	};
	namespace diagnostics
	{
		typedef void (*process_stdout_or_stderr_t)( void* udata , const char* data , size_t data_size );
		void on_default_redirect_stdout_or_stderr_handler( void* udata , const char* data , size_t data_size );
		void on_lines_count_redirect_stdout_or_stderr_handler( void* udata , const char* data , size_t data_size );
		enum XEnum_BinaryType
		{
			XEnum_BinaryType_Windows_x86 , 
			XEnum_BinaryType_Windows_x64 ,
			XEnum_BinaryType_Linux_x86 ,
			XEnum_BinaryType_Linux_x64 ,
			XEnum_BinaryType_Unknown
		};
		dll_internal XEnum_BinaryType get_binary_type( const char* filename );
		dll_internal ::rux::uint8 start_process( const char* module_name , const char* arguments );
		dll_internal ::rux::uint8 is_admin_user_process( void );
		class process
		{
		public:			
			static ::rux::int32 start_with_redirect( const char* executable , const char* arguments , process_stdout_or_stderr_t on_process_stdout , process_stdout_or_stderr_t on_process_stderr , void* udata , char** env , size_t env_size , char error_string[ 1024 ] = NULL );
			static ::rux::int32 start_with_redirect( const char* executable , char** arguments , size_t arguments_size , process_stdout_or_stderr_t on_process_stdout , process_stdout_or_stderr_t on_process_stderr , void* udata , char** env , size_t env_size , char error_string[ 1024 ] = NULL );
			static ::rux::uint8 kill( const char* module_name );
			static ::rux::uint8 killall( const char* module_name );
			static ::rux::pid_t get_process_id( void );
			static bool executing( const char* process_name , ::rux::pid_t* pid , ::rux::pid_t except_pid = 0 );
			static bool executing( const char* module_name , ::rux::pid_t pid );
			static float get_process_cpu_time( void );
#ifdef __WINDOWS__
			static ::rux::uint64 get_thread_cpu_time( HANDLE thread = NULL );
#else
			static ::rux::uint64 get_thread_cpu_time( pthread_t* thread = NULL );
#endif
			static size_t get_max_descriptors( void );
#ifdef __WINDOWS__
			static ::rux::uint8 adjust_token_privileges( HANDLE process_handle = NULL ,  char* error_string = NULL , size_t error_string_size = 0 );
#endif
			static void get_working_directory( char* string , size_t string_size );
		};
		class os
		{
		public:
			static bool restart( char error_string[ 1024 ] );
			static bool get_os( char* os_string , size_t os_string_size , char* error_string = 0 , size_t error_string_size = 0 );
		};
		class module
		{
		public:
			static void get_current_module_filename( char* module_filename , size_t module_filename_size );
			static void get_current_executable_filename( char* module_filename , size_t module_filename_size );
			static void get_current_module_directory( char* module_filename , size_t module_filename_size );
			static void get_current_executable_directory( char* module_filename , size_t module_filename_size );
		};
	};
};
#ifdef __WINDOWS__
dll_internal DWORD rux_get_pid( void );
dll_internal size_t rux_get_process_virtual_memory( DWORD pid );
dll_internal size_t rux_get_process_detailed_virtual_memory( DWORD pid , char modules[ 512 ][ 512 ] , size_t modules_vm[ 512 ] , size_t& modules_count );
#elif defined( __UNIX__ )
dll_internal ::rux::pid_t rux_get_pid( void );
dll_internal size_t rux_get_process_virtual_memory( ::rux::pid_t pid );
dll_internal size_t rux_get_process_detailed_virtual_memory( ::rux::pid_t pid , char modules[ 512 ][ 512 ] , size_t modules_vm[ 512 ] , size_t& modules_count );
#endif
#define DELTA_EPOCH_IN_USEC   11644473600000000LL;
dll_internal ::rux::uint64 rux_get_now_as_unix_time( void );
dll_internal ::rux::uint64 rux_get_now_as_local_unix_time( void );
dll_internal void rux_start_process_with_redirect( const char* executable , char** arguments , size_t arguments_size , char* result , size_t result_size , size_t& readen_size );
dll_internal ::rux::uint8 rux_is_hex_digit( char letter );
dll_internal ::rux::uint8 rux_hex_string_to_uint32( const char* hex , ::rux::uint32& value , size_t length = 0 );
dll_internal ::rux::uint8 rux_get_byte( ::rux::uint64 data , ::rux::uint32 index );
dll_internal ::rux::uint8 rux_get_bit( ::rux::uint8* bytes , size_t bitindex );
dll_internal void rux_unset_bit( ::rux::uint8* bytes , size_t bit_index );
dll_internal void rux_set_bit( ::rux::uint8* bytes , size_t bit_index );
dll_internal ::rux::uint64 rux_get_file_size( char* filename );
dll_internal void rux_unix_time_to_string( ::rux::uint64 unix_time_in_microseconds , char unix_time_string[] , size_t unix_time_string_size );
dll_internal void rux_get_executable_filename_only( char module_directory[] );
dll_internal void rux_get_executable_filename_only( const char* module_path , char module_directory[] );
dll_internal void rux_get_directory_only( const char* module_path , char module_directory[] );
#define rux_get_argument_by_separator( value , arguments , separator ) value.Clear();\
	XArray< XString > _______rux____pair;\
	for( ::rux::uint32 index0 = 0 ; index0 < arguments.Count() ; index0++ )\
	{\
		_______rux____pair.set_ByRef( arguments[ index0 ].Split( separator , EStringSplitOptions_None ) );\
		if( _______rux____pair.Count() == 2 )\
		{\
			if( _______rux____pair[ 0 ] == "--"#value\
				|| _______rux____pair[ 0 ] == "-"#value\
				|| _______rux____pair[ 0 ] == #value )\
			{\
				value = _______rux____pair[ 1 ];\
				break;\
			}\
		}\
	}
#define rux_native_get_argument( value , argument_names , argument_values , argument_count ) {\
	value = NULL;\
	for( ::rux::uint32 index0 = 0 ; index0 < argument_count ; index0++ )\
	{\
		if( index0 < argument_count )\
		{\
			if( strcmp( argument_names[ index0 ] , "--"#value ) == 0\
				|| strcmp( argument_names[ index0 ] , "-"#value ) == 0\
				|| strcmp( argument_names[ index0 ] , #value ) == 0 )\
			{\
				value = argument_values[ index0 ];\
				break;\
			}\
		}\
	}\
}
#define rux_get_argument( value , argument_names , argument_values ) value.Clear();\
	for( ::rux::uint32 index0 = 0 ; index0 < argument_names.Count() ; index0++ )\
	{\
		if( index0 < argument_values.Count() )\
		{\
			if( argument_names[ index0 ] == "--"#value\
				|| argument_names[ index0 ] == "-"#value\
				|| argument_names[ index0 ] == #value )\
			{\
				value = argument_values[ index0 ];\
				break;\
			}\
		}\
	}
#define declare_rux_base_internal_methods() public:\
	static ::rux::uint32 get_static_RuxTypeIndex( void );\
	static Type* get_static_Type( rux::uint32* rux_type_index );\
	static rux_volatile _rux_type_index;\
	static ::rux::uint8 _rux_is_array;\
	static Type* get_rux_array_item_type( void );\
	static const char* get_Alias( void );\
	virtual const void* get_ObjectAddress( void ) const;\
	static void register_type( Type* type );\
	static ::rux::register_type_t _register_methods_and_properties;\
	static XGCRef& ctor( void )	
#define declare_rux_base_methods_for_object() public:\
	static const char* get_Alias( void );\
	static Type* get_static_Type( rux::uint32* rux_type_index );\
	static ::rux::uint32 get_static_RuxTypeIndex( void )
#define declare_rux_base_methods( rux_class ) public:\
	static const char* get_Alias( void );\
	static Type* get_static_Type( rux::uint32* rux_type_index );\
	static ::rux::uint32 get_static_RuxTypeIndex( void );\
	X##rux_class( const Object& object );\
	X##rux_class( const XGCRef& gc_ref );\
	X##rux_class( const char* variable_name , const char* __file__ , ::rux::int32 __line__ );\
	X##rux_class( const Object& object , const char* variable_name , const char* __file__ , ::rux::int32 __line__ );\
	X##rux_class( const XGCRef& gc_ref , const char* variable_name , const char* __file__ , ::rux::int32 __line__ );\
	explicit X##rux_class( const XObject& object , const char* variable_name , const char* __file__ , ::rux::int32 __line__ );\
	void x_initialize( void )
#define declare_rux_set_operators( rux_class ) public:\
	X##rux_class& operator=( const XObject& object );\
	X##rux_class& operator=( XGCRef* object );\
	X##rux_class& operator=( const XGCRef& gc_ref );\
	X##rux_class& operator=( const rux_class& rux_object );\
	X##rux_class& operator=( const Object& object );\
	X##rux_class& operator=( const X##rux_class& rux_object )
#define declare_rux_constructors( rux_class ) public:\
	X##rux_class();\
	X##rux_class( const rux_class& rux_object );\
	X##rux_class( const X##rux_class& rux_object );\
	explicit X##rux_class( const XObject& object );\
	explicit X##rux_class( XGCRef* object )
#define declare_rux_constructors_and_operators( rux_class ) public:\
	declare_rux_constructors( rux_class );\
	declare_rux_set_operators( rux_class )
#define implement_rux_constructors_( rux_class , rux_register_methods_and_properties ) implement_rux_base_methods_( rux_class , rux_register_methods_and_properties );\
	X##rux_class::X##rux_class()\
	{\
	};\
	X##rux_class::X##rux_class( const rux_class& rux_object )\
	{\
		(*this) = rux_object;\
	};\
	X##rux_class::X##rux_class( const XObject& object )\
	{\
		(*this) = object;\
	};\
	X##rux_class::X##rux_class( const X##rux_class& rux_object )\
	{\
		(*this) = rux_object;\
	};\
	X##rux_class& X##rux_class::operator =( const XObject& object )\
	{\
		(*(static_cast< XGCHandle< rux_class >* >( this ))) = object;\
		return *this;\
	};\
	void X##rux_class::x_initialize( void )\
	{\
	}
#define implement_rux_constructors_and_operators_( rux_class , rux_register_methods_and_properties ) implement_rux_constructors_( rux_class , rux_register_methods_and_properties );\
	X##rux_class& X##rux_class::operator =( const rux_class& rux_object )\
	{\
		(*(static_cast<XGCHandle<rux_class>*>( this ))) = rux_object;\
		return *this;\
	};\
	X##rux_class& X##rux_class::operator =( const X##rux_class& rux_object )\
	{\
		(*(static_cast< XGCHandle< rux_class >* >( this ))) = rux_object;\
		return *this;\
	}
#define implement_rux_constructors_ns_( rux_class , rux_namespace , rux_register_methods_and_properties ) implement_rux_base_methods_ns_( rux_class , rux_namespace , rux_register_methods_and_properties );\
	rux_namespace::X##rux_class::X##rux_class()\
	{\
	};\
	rux_namespace::X##rux_class::X##rux_class( const rux_namespace::rux_class& rux_object )\
	{\
		(*this) = rux_object;\
	};\
	rux_namespace::X##rux_class::X##rux_class( const XObject& object )\
	{\
		(*this) = object;\
	};\
	rux_namespace::X##rux_class::X##rux_class( XGCRef* gcref )\
	{\
		(*this) = gcref;\
	};\
	rux_namespace::X##rux_class::X##rux_class( const rux_namespace::X##rux_class& rux_object )\
	{\
		(*this) = rux_object;\
	};\
	void rux_namespace::X##rux_class::x_initialize( void )\
	{\
	}
#define implement_rux_constructors_and_operators_ns_( rux_class , rux_namespace , rux_register_methods_and_properties ) implement_rux_constructors_ns_( rux_class , rux_namespace , rux_register_methods_and_properties );\
	rux_namespace::X##rux_class& rux_namespace::X##rux_class::operator =( const rux_namespace::rux_class& rux_object )\
	{\
		(*(static_cast<XGCHandle<rux_namespace::rux_class>*>( this ))) = rux_object;\
		return *this;\
	};\
	rux_namespace::X##rux_class& rux_namespace::X##rux_class::operator =( const rux_namespace::X##rux_class& rux_object )\
	{\
		(*(static_cast< XGCHandle< rux_namespace::rux_class >* >( this ))) = rux_object;\
		return *this;\
	}
#define implement_ctor_and_register_type( rux_class ) XGCRef& rux_class::ctor( void )\
	{\
		X##rux_class obj;\
		return obj++;\
	};\
	void rux_class::register_type( Type* type )\
	{\
		type->RegisterConstructor( (void*)rux_class::ctor );\
		if( rux_class::_register_methods_and_properties )\
			rux_class::_register_methods_and_properties( type );\
	};\
	Type* rux_class::get_rux_array_item_type( void )\
	{\
		return NULL;\
	}
#define implement_ctor_and_register_type_ns( rux_namespace , rux_class ) XGCRef& rux_namespace::rux_class::ctor( void )\
	{\
		rux_namespace::X##rux_class obj;\
		return obj++;\
	};\
	void rux_namespace::rux_class::register_type( Type* type )\
	{\
		type->RegisterConstructor( (void*)rux_namespace::rux_class::ctor );\
		if( rux_namespace::rux_class::_register_methods_and_properties )\
			rux_namespace::rux_class::_register_methods_and_properties( type );\
	};\
	Type* rux_namespace::rux_class::get_rux_array_item_type( void )\
	{\
		return NULL;\
	}
#define begin_implement_rux_class_without_operators( rux_class ) implement_rux_constructors_( rux_class , NULL )\
	rux_class::rux_class( const char* variable_name , const char* __file__ , ::rux::int32 __line__ )\
		: XGCRef( rux_class::get_static_Type )\
	{\
		initialize_members( variable_name , __file__ , __line__ );\
	};\
	implement_ctor_and_register_type( rux_class );\
	void rux_class::initialize_members( const char* variable_name , const char* __file__ , ::rux::int32 __line__ )\
	{
#define implement_duplicate_internal_result_function( result_class , rux_class , rux_function ) result_class X##rux_class::rux_function( void )\
	{\
		return (*static_cast< XGCHandle< rux_class >*>( this ))()->rux_function();\
	}
#define implement_duplicate_internal_result_function_with_const( result_class , rux_class , rux_function ) result_class X##rux_class::rux_function( void ) const\
	{\
	return (*static_cast< XGCHandle< rux_class >*>( const_cast< X##rux_class* >( this ) ))()->rux_function();\
	}
#define implement_duplicate_internal_result_function_1( result_class , rux_class , rux_function , p0_class ) result_class X##rux_class::rux_function( p0_class p0 )\
	{\
		return (*static_cast< XGCHandle< rux_class >*>( this ))()->rux_function( p0 );\
	}
#define implement_duplicate_internal_result_function_1_with_const( result_class , rux_class , rux_function , p0_class ) result_class X##rux_class::rux_function( p0_class p0 ) const\
	{\
		return (*static_cast< XGCHandle< rux_class >*>( const_cast< X##rux_class* >( this ) ))()->rux_function( p0 );\
	}
#define implement_duplicate_internal_result_function_2( result_class , rux_class , rux_function , p0_class , p1_class ) result_class X##rux_class::rux_function( p0_class p0 , p1_class p1 )\
	{\
		return (*static_cast< XGCHandle< rux_class >*>( this ))()->rux_function( p0 , p1 );\
	}
#define implement_duplicate_internal_result_function_2_with_const( result_class , rux_class , rux_function , p0_class , p1_class ) result_class X##rux_class::rux_function( p0_class p0 , p1_class p1 ) const\
	{\
		return (*static_cast< XGCHandle< rux_class >*>( const_cast< X##rux_class* >( this ) ))()->rux_function( p0 , p1 );\
	}
#define implement_duplicate_internal_result_function_3( result_class , rux_class , rux_function , p0_class , p1_class , p2_class ) result_class X##rux_class::rux_function( p0_class p0 , p1_class p1 , p2_class p2 )\
	{\
		return (*static_cast< XGCHandle< rux_class >*>( this ))()->rux_function( p0 , p1 , p2 );\
	}
#define implement_duplicate_internal_result_function_3_with_const( result_class , rux_class , rux_function , p0_class , p1_class , p2_class ) result_class X##rux_class::rux_function( p0_class p0 , p1_class p1 , p2_class p2 ) const\
	{\
		return (*static_cast< XGCHandle< rux_class >*>( const_cast< X##rux_class* >( this ) ))()->rux_function( p0 , p1 , p2 );\
	}
#define implement_duplicate_internal_result_function_4( result_class , rux_class , rux_function , p0_class , p1_class , p2_class , p3_class ) result_class X##rux_class::rux_function( p0_class p0 , p1_class p1 , p2_class p2 , p3_class p3 )\
	{\
		return (*static_cast< XGCHandle< rux_class >*>( this ))()->rux_function( p0 , p1 , p2 , p3 );\
	}
#define implement_duplicate_internal_result_function_5( result_class , rux_class , rux_function , p0_class , p1_class , p2_class , p3_class , p4_class ) result_class X##rux_class::rux_function( p0_class p0 , p1_class p1 , p2_class p2 , p3_class p3 , p4_class p4 )\
	{\
		return (*static_cast< XGCHandle< rux_class >*>( this ))()->rux_function( p0 , p1 , p2 , p3 , p4 );\
	}
#define implement_duplicate_internal_result_function_6( result_class , rux_class , rux_function , p0_class , p1_class , p2_class , p3_class , p4_class , p5_class ) result_class X##rux_class::rux_function( p0_class p0 , p1_class p1 , p2_class p2 , p3_class p3 , p4_class p4 , p5_class p5 )\
	{\
		return (*static_cast< XGCHandle< rux_class >*>( this ))()->rux_function( p0 , p1 , p2 , p3 , p4 , p5 );\
	}
#define implement_duplicate_internal_result_function_7( result_class , rux_class , rux_function , p0_class , p1_class , p2_class , p3_class , p4_class , p5_class , p6_class ) result_class X##rux_class::rux_function( p0_class p0 , p1_class p1 , p2_class p2 , p3_class p3 , p4_class p4 , p5_class p5 , p6_class p6 )\
	{\
		return (*static_cast< XGCHandle< rux_class >*>( this ))()->rux_function( p0 , p1 , p2 , p3 , p4 , p5 , p6 );\
	}
#define implement_duplicate_internal_function( rux_class , rux_function ) void X##rux_class::rux_function( void )\
	{\
		(*static_cast< XGCHandle< rux_class >*>( this ))()->rux_function();\
	}
#define implement_duplicate_internal_function_with_const( rux_class , rux_function ) void X##rux_class::rux_function( void ) const\
	{\
		(*static_cast< XGCHandle< rux_class >*>( const_cast< X##rux_class* >( this ) ))()->rux_function();\
	}
#define implement_duplicate_internal_function_1_with_const( rux_class , rux_function , p0_class ) void X##rux_class::rux_function( p0_class p0 ) const\
	{\
		(*static_cast< XGCHandle< rux_class >*>( const_cast< X##rux_class* >( this ) ))()->rux_function( p0 );\
	}
#define implement_duplicate_internal_function_1( rux_class , rux_function , p0_class ) void X##rux_class::rux_function( p0_class p0 )\
	{\
		(*static_cast< XGCHandle< rux_class >*>( this ))()->rux_function( p0 );\
	}
#define implement_duplicate_internal_function_2_with_const( rux_class , rux_function , p0_class , p1_class ) void X##rux_class::rux_function( p0_class p0 , p1_class p1 ) const\
	{\
		(*static_cast< XGCHandle< rux_class >*>( const_cast< X##rux_class* >( this ) ))()->rux_function( p0 , p1 );\
	}
#define implement_duplicate_internal_function_2( rux_class , rux_function , p0_class , p1_class ) void X##rux_class::rux_function( p0_class p0 , p1_class p1 )\
	{\
		(*static_cast< XGCHandle< rux_class >*>( this ))()->rux_function( p0 , p1 );\
	}
#define implement_duplicate_internal_function_3( rux_class , rux_function , p0_class , p1_class , p2_class ) void X##rux_class::rux_function( p0_class p0 , p1_class p1 , p2_class p2 )\
	{\
		(*static_cast< XGCHandle< rux_class >*>( this ))()->rux_function( p0 , p1 , p2 );\
	}
#define implement_duplicate_internal_function_4( rux_class , rux_function , p0_class , p1_class , p2_class , p3_class ) void X##rux_class::rux_function( p0_class p0 , p1_class p1 , p2_class p2 , p3_class p3 )\
	{\
		(*static_cast< XGCHandle< rux_class >*>( this ))()->rux_function( p0 , p1 , p2 , p3 );\
	}
#define implement_duplicate_internal_function_5( rux_class , rux_function , p0_class , p1_class , p2_class , p3_class , p4_class ) void X##rux_class::rux_function( p0_class p0 , p1_class p1 , p2_class p2 , p3_class p3 , p4_class p4 )\
	{\
		(*static_cast< XGCHandle< rux_class >*>( this ))()->rux_function( p0 , p1 , p2 , p3 , p4 );\
	}
#define implement_duplicate_internal_function_6( rux_class , rux_function , p0_class , p1_class , p2_class , p3_class , p4_class , p5_class ) void X##rux_class::rux_function( p0_class p0 , p1_class p1 , p2_class p2 , p3_class p3 , p4_class p4 , p5_class p5 )\
	{\
		(*static_cast< XGCHandle< rux_class >*>( this ))()->rux_function( p0 , p1 , p2 , p3 , p4 , p5 );\
	}
#define implement_duplicate_internal_function_7( rux_class , rux_function , p0_class , p1_class , p2_class , p3_class , p4_class , p5_class , p6_class ) void X##rux_class::rux_function( p0_class p0 , p1_class p1 , p2_class p2 , p3_class p3 , p4_class p4 , p5_class p5 , p6_class p6 )\
	{\
		(*static_cast< XGCHandle< rux_class >*>( this ))()->rux_function( p0 , p1 , p2 , p3 , p4 , p5 , p6 );\
	}
#define implement_duplicate_internal_function_8( rux_class , rux_function , p0_class , p1_class , p2_class , p3_class , p4_class , p5_class , p6_class , p7_class ) void X##rux_class::rux_function( p0_class p0 , p1_class p1 , p2_class p2 , p3_class p3 , p4_class p4 , p5_class p5 , p6_class p6 , p7_class p7 )\
	{\
		(*static_cast< XGCHandle< rux_class >*>( this ))()->rux_function( p0 , p1 , p2 , p3 , p4 , p5 , p6 , p7 );\
	}
#define implement_duplicate_internal_function_9( rux_class , rux_function , p0_class , p1_class , p2_class , p3_class , p4_class , p5_class , p6_class , p7_class , p8_class ) void X##rux_class::rux_function( p0_class p0 , p1_class p1 , p2_class p2 , p3_class p3 , p4_class p4 , p5_class p5 , p6_class p6 , p7_class p7 , p8_class p8 )\
	{\
		(*static_cast< XGCHandle< rux_class >*>( this ))()->rux_function( p0 , p1 , p2 , p3 , p4 , p5 , p6 , p7 , p8 );\
	}
#define implement_duplicate_internal_function_10( rux_class , rux_function , p0_class , p1_class , p2_class , p3_class , p4_class , p5_class , p6_class , p7_class , p8_class , p9_class ) void X##rux_class::rux_function( p0_class p0 , p1_class p1 , p2_class p2 , p3_class p3 , p4_class p4 , p5_class p5 , p6_class p6 , p7_class p7 , p8_class p8 , p9_class p9 )\
	{\
		(*static_cast< XGCHandle< rux_class >*>( this ))()->rux_function( p0 , p1 , p2 , p3 , p4 , p5 , p6 , p7 , p8 , p9 );\
	}
#define begin_implement_rux_class_without_set_operators_ns( rux_class , rux_namespace ) implement_rux_constructors_ns_( rux_class , rux_namespace , NULL )\
	rux_namespace::rux_class::rux_class( const char* variable_name , const char* __file__ , ::rux::int32 __line__ )\
		: XGCRef( rux_namespace::rux_class::get_static_Type )\
	{\
		initialize_members( variable_name , __file__ , __line__ );\
	};\
	implement_ctor_and_register_type_ns( rux_namespace , rux_class );\
	void rux_namespace::rux_class::initialize_members( const char* variable_name , const char* __file__ , ::rux::int32 __line__ )\
	{
#define begin_implement_rux_class_without_operators_ns( rux_class , rux_namespace ) implement_rux_constructors_ns_( rux_class , rux_namespace , NULL )\
	implement_rux_set_operators_ns_( rux_class , rux_namespace );\
	rux_namespace::rux_class::rux_class( const char* variable_name , const char* __file__ , ::rux::int32 __line__ )\
		: XGCRef( rux_namespace::rux_class::get_static_Type )\
	{\
		initialize_members( variable_name , __file__ , __line__ );\
	};\
	implement_ctor_and_register_type_ns( rux_namespace , rux_class );\
	void rux_namespace::rux_class::initialize_members( const char* variable_name , const char* __file__ , ::rux::int32 __line__ )\
	{
#define begin_implement_rux_class( rux_class ) begin_implement_rux_class_( rux_class , NULL , XGCRef )
#define begin_implement_rux_class_base_class( rux_class , rux_base_class ) begin_implement_rux_class_( rux_class , NULL , rux_base_class )
#define begin_implement_rux_class_with_properties( rux_class ) begin_implement_rux_class_( rux_class , rux_class::register_methods_and_properties , XGCRef )
#define begin_implement_rux_class_with_properties_ns_base_class( rux_class , rux_namespace , rux_base_class ) implement_rux_set_operators_ns_( rux_class , rux_namespace );\
	begin_implement_rux_class_ns_( rux_class , rux_namespace , rux_namespace::rux_class::register_methods_and_properties , rux_base_class )
#define begin_implement_rux_class_with_properties_ns( rux_class , rux_namespace ) begin_implement_rux_class_ns_( rux_class , rux_namespace , rux_namespace::rux_class::register_methods_and_properties , XGCRef )
#define begin_implement_rux_class_( rux_class , rux_register_methods_and_properties , rux_base_class ) implement_rux_constructors_and_operators_( rux_class , rux_register_methods_and_properties )\
	rux_class::rux_class( const char* variable_name , const char* __file__ , ::rux::int32 __line__ )\
		: rux_base_class( rux_class::get_static_Type )\
	{\
		initialize_members( variable_name , __file__ , __line__ );\
	};\
	implement_ctor_and_register_type( rux_class );\
	void rux_class::initialize_members( const char* variable_name , const char* __file__ , ::rux::int32 __line__ )\
	{
#define begin_implement_rux_class_ns( rux_class , rux_namespace ) implement_rux_set_operators_ns_( rux_class , rux_namespace );\
	begin_implement_rux_class_ns_( rux_class , rux_namespace , NULL , XGCRef )
#define begin_implement_rux_class_ns_( rux_class , rux_namespace , rux_register_methods_and_properties , rux_base_class ) implement_rux_constructors_and_operators_ns_( rux_class , rux_namespace , rux_register_methods_and_properties )\
	rux_namespace::rux_class::rux_class( const char* variable_name , const char* __file__ , ::rux::int32 __line__ )\
		: rux_base_class( rux_namespace::rux_class::get_static_Type )\
	{\
		initialize_members( variable_name , __file__ , __line__ );\
	};\
	implement_ctor_and_register_type_ns( rux_namespace , rux_class );\
	void rux_namespace::rux_class::initialize_members( const char* variable_name , const char* __file__ , ::rux::int32 __line__ )\
	{
#define end_implement_rux_class() }
#define begin_declare_rux_class_members( rux_class ) private:
#define end_declare_rux_class_members( rux_class ) };\
	class X##rux_class : public XGCHandle< rux_class >\
	{\
	declare_rux_base_methods( rux_class );\
public:\
	declare_rux_constructors_and_operators( rux_class )
#define end_declare_rux_class_members_without_set_operators( rux_class ) };\
	class X##rux_class : public XGCHandle< rux_class >\
	{\
	declare_rux_base_methods( rux_class );\
public:\
	declare_rux_constructors( rux_class )
#define begin_declare_rux_class_with_namespace( rux_class , rux_namespace ) class X##rux_class;\
	class rux_class : public XGCRef\
	{\
		friend class rux_namespace::X##rux_class;\
		declare_rux_base_internal_methods();\
		void initialize_members( const char* variable_name , const char* __file__ , ::rux::int32 __line__ );\
		public:\
		rux_class( const char* variable_name = 0 , const char* __file__ = 0 , ::rux::int32 __line__ = 0 )
#define begin_declare_rux_class_( rux_class , rux_parent_class ) class rux_class : public rux_parent_class\
	{\
		friend class X##rux_class;\
		declare_rux_base_internal_methods();\
		void initialize_members( const char* variable_name , const char* __file__ , ::rux::int32 __line__ );\
		public:\
		rux_class( const char* variable_name = 0 , const char* __file__ = 0 , ::rux::int32 __line__ = 0 )
#define begin_declare_rux_class( rux_class ) begin_declare_rux_class_( rux_class , XGCRef )
#define begin_declare_rux_class_2( rux_class , base_class0 ) class rux_class : public XGCRef , public base_class0\
	{\
		friend class X##rux_class;\
		declare_rux_base_internal_methods();\
		void initialize_members( const char* variable_name , const char* __file__ , ::rux::int32 __line__ );\
		public:\
		rux_class( const char* variable_name = 0 , const char* __file__ = 0 , ::rux::int32 __line__ = 0 )
#define end_declare_rux_class() }
#define implement_rux_base_methods( rux_class ) implement_rux_base_methods_( rux_class , NULL )
#define implement_rux_base_methods_( rux_class , rux_register_methods_and_properties ) const char* X##rux_class::get_Alias( void )\
	{\
		return #rux_class;\
	};\
	X##rux_class::X##rux_class( const XGCRef& gcref )\
	{\
		*this = gcref;\
	};\
	X##rux_class::X##rux_class( const char* variable_name , const char* __file__ , ::rux::int32 __line__ )\
		: XGCHandle< rux_class >( variable_name , __file__ , __line__ )\
	{\
		x_initialize();\
	};\
	X##rux_class::X##rux_class( const XGCRef& gcref , const char* variable_name , const char* __file__ , ::rux::int32 __line__ )\
		: XGCHandle< rux_class >( variable_name , __file__ , __line__ )\
	{\
		*this = gcref;\
	};\
	X##rux_class::X##rux_class( const Object& object , const char* variable_name , const char* __file__ , ::rux::int32 __line__ )\
		: XGCHandle< rux_class >( variable_name , __file__ , __line__ )\
	{\
		*static_cast<XGCHandle< rux_class >*>(this) = object;\
	};\
	X##rux_class::X##rux_class( const XObject& object , const char* variable_name , const char* __file__ , ::rux::int32 __line__ )\
		: XGCHandle< rux_class >( variable_name , __file__ , __line__ )\
	{\
		(*this) = object;\
	};\
	X##rux_class::X##rux_class( const Object& object )\
	{\
		*static_cast<XGCHandle< rux_class >*>(this) = object;\
	};\
	X##rux_class& X##rux_class::operator=( const Object& object )\
	{\
		*static_cast<XGCHandle< rux_class >*>(this) = object;\
		return *this;\
	};	\
	X##rux_class& X##rux_class::operator=( const XGCRef& gcref )\
	{\
		*static_cast<XGCHandle< rux_class >*>(this) = gcref;\
		return *this;\
	};\
	X##rux_class& X##rux_class::operator=( XGCRef* gcref )\
	{\
		*static_cast<XGCHandle< rux_class >*>(this) = gcref;\
		return *this;\
	};\
	const void* rux_class::get_ObjectAddress( void ) const\
	{\
		return this;\
	};\
	const char* rux_class::get_Alias( void )\
	{\
		return #rux_class;\
	};\
	implement_get_static_Type( rux_class );\
	rux_volatile rux_class::_rux_type_index = rux_volatile_max;\
	::rux::uint8 rux_class::_rux_is_array = 0;\
	::rux::register_type_t rux_class::_register_methods_and_properties = rux_register_methods_and_properties;\
	namespace rux\
	{\
		template class XKeyValuePair< X##rux_class >;\
		template class XArray< XKeyValuePair< X##rux_class > >;\
		template class XArray< X##rux_class >;\
		template<> rux_volatile Array< X##rux_class >::_rux_type_index = rux_volatile_max;\
		template<> rux_volatile KeyValuePair< X##rux_class >::_rux_type_index = rux_volatile_max;\
		template<> rux_volatile Array< XKeyValuePair< X##rux_class > >::_rux_type_index = rux_volatile_max;\
		template<> rux_volatile Array< XKeyValuePair< XArray< X##rux_class > > >::_rux_type_index = rux_volatile_max;\
		template<> rux_volatile Array< XArray< X##rux_class > >::_rux_type_index = rux_volatile_max;\
		template<> rux_volatile Array< XArray< XKeyValuePair< X##rux_class > > >::_rux_type_index = rux_volatile_max;\
		template<> rux_volatile Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > > >::_rux_type_index = rux_volatile_max;\
		template<> rux_volatile Array< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > >::_rux_type_index = rux_volatile_max;\
		template<> rux_volatile KeyValuePair< XArray< X##rux_class > >::_rux_type_index = rux_volatile_max;\
		template<> rux_volatile KeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > >::_rux_type_index = rux_volatile_max;\
		template<> rux_volatile KeyValuePair< XArray< XKeyValuePair< X##rux_class > > >::_rux_type_index = rux_volatile_max;\
		template<> ::rux::uint8 Array< X##rux_class >::_rux_is_array = 1;\
		template<> ::rux::uint8 KeyValuePair< X##rux_class >::_rux_is_array = 0;\
		template<> ::rux::uint8 Array< XKeyValuePair< X##rux_class > >::_rux_is_array = 1;\
		template<> ::rux::uint8 Array< XKeyValuePair< XArray< X##rux_class > > >::_rux_is_array = 1;\
		template<> ::rux::uint8 Array< XArray< X##rux_class > >::_rux_is_array = 1;\
		template<> ::rux::uint8 Array< XArray< XKeyValuePair< X##rux_class > > >::_rux_is_array = 1;\
		template<> ::rux::uint8 Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > > >::_rux_is_array = 1;\
		template<> ::rux::uint8 Array< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > >::_rux_is_array = 1;\
		template<> ::rux::uint8 KeyValuePair< XArray< X##rux_class > >::_rux_is_array = 0;\
		template<> ::rux::uint8 KeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > >::_rux_is_array = 0;\
		template<> ::rux::uint8 KeyValuePair< XArray< XKeyValuePair< X##rux_class > > >::_rux_is_array = 0;\
		template<> ::rux::register_type_t KeyValuePair< X##rux_class >::_register_methods_and_properties = NULL;\
		template<> ::rux::register_type_t KeyValuePair< XArray< X##rux_class > >::_register_methods_and_properties = NULL;\
		template<> ::rux::register_type_t KeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > >::_register_methods_and_properties = NULL;\
		template<> ::rux::register_type_t KeyValuePair< XArray< XKeyValuePair< X##rux_class > > >::_register_methods_and_properties = NULL;\
		template<> const char* KeyValuePair< X##rux_class >::get_Alias( void )\
		{\
			return "KeyValuePair< "#rux_class" >";\
		};\
		template<> const char* Array< X##rux_class >::get_Alias( void )\
		{\
			return "Array< "#rux_class" >";\
		};\
		template<> const char* Array< XKeyValuePair< X##rux_class > >::get_Alias( void )\
		{\
			return "Array< KeyValuePair< "#rux_class" > >";\
		};\
		template<> const char* Array< XArray< XKeyValuePair< X##rux_class > > >::get_Alias( void )\
		{\
			return "Array< Array< KeyValuePair< "#rux_class" > > >";\
		};\
		template<> const char* Array< XArray< X##rux_class > >::get_Alias( void )\
		{\
			return "Array< Array< "#rux_class" > >";\
		};\
		template<> const char* Array< XKeyValuePair< XArray< X##rux_class > > >::get_Alias( void )\
		{\
			return "Array< KeyValuePair< Array< "#rux_class" > > >";\
		};\
		template<> const char* Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > > >::get_Alias( void )\
		{\
			return "Array< KeyValuePair< Array< KeyValuePair< Array< KeyValuePair< "#rux_class" > > > > > >";\
		};\
		template<> const char* Array< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > >::get_Alias( void )\
		{\
			return "Array< KeyValuePair< Array< KeyValuePair< "#rux_class" > > > >";\
		};\
		template<> const char* KeyValuePair< XArray< X##rux_class > >::get_Alias( void )\
		{\
			return "KeyValuePair< Array< "#rux_class" > >";\
		};\
		template<> const char* KeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > >::get_Alias( void )\
		{\
			return "KeyValuePair< Array< KeyValuePair< Array< KeyValuePair< "#rux_class" > > > > >";\
		};\
		template<> const char* KeyValuePair< XArray< XKeyValuePair< X##rux_class > > >::get_Alias( void )\
		{\
			return "KeyValuePair< Array< KeyValuePair< "#rux_class" > > >";\
		};\
		template<> const void* KeyValuePair< X##rux_class >::get_ObjectAddress( void ) const\
		{\
			return this;\
		};\
		template<> const void* Array< X##rux_class >::get_ObjectAddress( void ) const\
		{\
			return this;\
		};\
		template<> const void* Array< XKeyValuePair< X##rux_class > >::get_ObjectAddress( void ) const\
		{\
			return this;\
		};\
		template<> const void* Array< XArray< XKeyValuePair< X##rux_class > > >::get_ObjectAddress( void ) const\
		{\
			return this;\
		};\
		template<> const void* Array< XArray< X##rux_class > >::get_ObjectAddress( void ) const\
		{\
			return this;\
		};\
		template<> const void* Array< XKeyValuePair< XArray< X##rux_class > > >::get_ObjectAddress( void ) const\
		{\
			return this;\
		};\
		template<> const void* Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > > >::get_ObjectAddress( void ) const\
		{\
			return this;\
		};\
		template<> const void* Array< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > >::get_ObjectAddress( void ) const\
		{\
			return this;\
		};\
		template<> const void* KeyValuePair< XArray< X##rux_class > >::get_ObjectAddress( void ) const\
		{\
			return this;\
		};\
		template<> const void* KeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > >::get_ObjectAddress( void ) const\
		{\
			return this;\
		};\
		template<> const void* KeyValuePair< XArray< XKeyValuePair< X##rux_class > > >::get_ObjectAddress( void ) const\
		{\
			return this;\
		};\
		template<> void KeyValuePair< X##rux_class >::register_type( Type* type )\
		{\
			type->RegisterConstructor( (void*)KeyValuePair< X##rux_class >::ctor );\
			if( KeyValuePair< X##rux_class >::_register_methods_and_properties )\
				KeyValuePair< X##rux_class >::_register_methods_and_properties( type );\
		};\
		template<> void Array< X##rux_class >::register_type( Type* type )\
		{\
			type->RegisterConstructor( (void*)Array< X##rux_class >::ctor );\
			if( Array< X##rux_class >::_register_methods_and_properties )\
				Array< X##rux_class >::_register_methods_and_properties( type );\
		};\
		template<> void Array< XArray< X##rux_class > >::register_type( Type* type )\
		{\
			type->RegisterConstructor( (void*)Array< XArray< X##rux_class > >::ctor );\
			if( Array< XArray< X##rux_class > >::_register_methods_and_properties )\
				Array< XArray< X##rux_class > >::_register_methods_and_properties( type );\
		};\
		template<> void Array< XArray< XKeyValuePair< X##rux_class > > >::register_type( Type* type )\
		{\
			type->RegisterConstructor( (void*)Array< XArray< XKeyValuePair< X##rux_class > > >::ctor );\
			if( Array< XArray< XKeyValuePair< X##rux_class > > >::_register_methods_and_properties )\
				Array< XArray< XKeyValuePair< X##rux_class > > >::_register_methods_and_properties( type );\
		};\
		template<> void Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > > >::register_type( Type* type )\
		{\
			type->RegisterConstructor( (void*)Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > > >::ctor );\
			if( Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > > >::_register_methods_and_properties )\
				Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > > >::_register_methods_and_properties( type );\
		};\
		template<> void Array< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > >::register_type( Type* type )\
		{\
			type->RegisterConstructor( (void*)Array< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > >::ctor );\
			if( Array< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > >::_register_methods_and_properties )\
				Array< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > >::_register_methods_and_properties( type );\
		};\
		template<> void KeyValuePair< XArray< X##rux_class > >::register_type( Type* type )\
		{\
			type->RegisterConstructor( (void*)KeyValuePair< XArray< X##rux_class > >::ctor );\
			if( KeyValuePair< XArray< X##rux_class > >::_register_methods_and_properties )\
				KeyValuePair< XArray< X##rux_class > >::_register_methods_and_properties( type );\
		};\
		template<> void KeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > >::register_type( Type* type )\
		{\
			type->RegisterConstructor( (void*)KeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > >::ctor );\
			if( KeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > >::_register_methods_and_properties )\
				KeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > >::_register_methods_and_properties( type );\
		};\
		template<> void KeyValuePair< XArray< XKeyValuePair< X##rux_class > > >::register_type( Type* type )\
		{\
			type->RegisterConstructor( (void*)KeyValuePair< XArray< XKeyValuePair< X##rux_class > > >::ctor );\
			if( KeyValuePair< XArray< XKeyValuePair< X##rux_class > > >::_register_methods_and_properties )\
				KeyValuePair< XArray< XKeyValuePair< X##rux_class > > >::_register_methods_and_properties( type );\
		};\
		template<> void Array< XKeyValuePair< XArray< X##rux_class > > >::register_type( Type* type )\
		{\
			type->RegisterConstructor( (void*)Array< XKeyValuePair< XArray< X##rux_class > > >::ctor );\
			if( Array< XKeyValuePair< XArray< X##rux_class > > >::_register_methods_and_properties )\
				Array< XKeyValuePair< XArray< X##rux_class > > >::_register_methods_and_properties( type );\
		};\
		template<> void Array< XKeyValuePair< X##rux_class > >::register_type( Type* type )\
		{\
			type->RegisterConstructor( (void*)Array< XKeyValuePair< X##rux_class > >::ctor );\
			if( Array< XKeyValuePair< X##rux_class > >::_register_methods_and_properties )\
				Array< XKeyValuePair< X##rux_class > >::_register_methods_and_properties( type );\
		};\
		template<> Type* KeyValuePair< XArray< X##rux_class > >::get_rux_array_item_type( void )\
		{\
			return NULL;\
		};\
		template<> Type* KeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > >::get_rux_array_item_type( void )\
		{\
			return NULL;\
		};\
		template<> Type* KeyValuePair< XArray< XKeyValuePair< X##rux_class > > >::get_rux_array_item_type( void )\
		{\
			return NULL;\
		};\
		template<> Type* KeyValuePair< X##rux_class >::get_rux_array_item_type( void )\
		{\
			return NULL;\
		};\
		template<> Type* Array< XKeyValuePair< X##rux_class > >::get_rux_array_item_type( void )\
		{\
			return _typeof( XKeyValuePair< X##rux_class >::get_Alias() , &XKeyValuePair< X##rux_class >::InternalClass::_rux_type_index , XKeyValuePair< X##rux_class >::InternalClass::register_type , XKeyValuePair< X##rux_class >::InternalClass::_rux_is_array , XKeyValuePair< X##rux_class >::InternalClass::get_rux_array_item_type() );\
		};\
		template<> Type* Array< X##rux_class >::get_rux_array_item_type( void )\
		{\
			return _typeof( X##rux_class::get_Alias() , &X##rux_class::InternalClass::_rux_type_index , X##rux_class::InternalClass::register_type , X##rux_class::InternalClass::_rux_is_array , X##rux_class::InternalClass::get_rux_array_item_type() );\
		};\
		template<> Type* Array< XArray< X##rux_class > >::get_rux_array_item_type( void )\
		{\
			return _typeof( XArray< X##rux_class >::get_Alias() , &XArray< X##rux_class >::InternalClass::_rux_type_index , XArray< X##rux_class >::InternalClass::register_type , XArray< X##rux_class >::InternalClass::_rux_is_array , XArray< X##rux_class >::InternalClass::get_rux_array_item_type() );\
		};\
		template<> Type* Array< XArray< XKeyValuePair< X##rux_class > > >::get_rux_array_item_type( void )\
		{\
			return _typeof( XArray< XKeyValuePair< X##rux_class > >::get_Alias() , &XArray< XKeyValuePair< X##rux_class > >::InternalClass::_rux_type_index , XArray< XKeyValuePair< X##rux_class > >::InternalClass::register_type , XArray< XKeyValuePair< X##rux_class > >::InternalClass::_rux_is_array , XArray< XKeyValuePair< X##rux_class > >::InternalClass::get_rux_array_item_type() );\
		};\
		template<> Type* Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > > >::get_rux_array_item_type( void )\
		{\
			return _typeof( XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > >::get_Alias() , &XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > >::InternalClass::_rux_type_index , XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > >::InternalClass::register_type , XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > >::InternalClass::_rux_is_array , XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > >::InternalClass::get_rux_array_item_type() );\
		};\
		template<> Type* Array< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > >::get_rux_array_item_type( void )\
		{\
			return _typeof( XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > >::get_Alias() , &XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > >::InternalClass::_rux_type_index , XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > >::InternalClass::register_type , XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > >::InternalClass::_rux_is_array , XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > >::InternalClass::get_rux_array_item_type() );\
		};\
		template<> Type* Array< XKeyValuePair< XArray< X##rux_class > > >::get_rux_array_item_type( void )\
		{\
			return _typeof( XKeyValuePair< XArray< X##rux_class > >::get_Alias() , &XKeyValuePair< XArray< X##rux_class > >::InternalClass::_rux_type_index , XKeyValuePair< XArray< X##rux_class > >::InternalClass::register_type , XKeyValuePair< XArray< X##rux_class > >::InternalClass::_rux_is_array , XKeyValuePair< XArray< X##rux_class > >::InternalClass::get_rux_array_item_type() );\
		};\
		implement_get_static_RuxTypeIndex_for_template( KeyValuePair< X##rux_class > );\
		implement_get_static_RuxTypeIndex_for_template( Array< X##rux_class > );\
		implement_get_static_RuxTypeIndex_for_template( Array< XArray< X##rux_class > > );\
		implement_get_static_RuxTypeIndex_for_template( Array< XArray< XKeyValuePair< X##rux_class > > > );\
		implement_get_static_RuxTypeIndex_for_template( Array< XKeyValuePair< XArray< X##rux_class > > > );\
		implement_get_static_RuxTypeIndex_for_template( Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > > > );\
		implement_get_static_RuxTypeIndex_for_template( Array< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > );\
		implement_get_static_RuxTypeIndex_for_template( KeyValuePair< XArray< X##rux_class > > );\
		implement_get_static_RuxTypeIndex_for_template( KeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > > );\
		implement_get_static_RuxTypeIndex_for_template( KeyValuePair< XArray< XKeyValuePair< X##rux_class > > > );\
		implement_get_static_RuxTypeIndex_for_template( Array< XKeyValuePair< X##rux_class > > );\
		implement_get_static_Type_for_template( KeyValuePair< X##rux_class > );\
		implement_get_static_Type_for_template( Array< X##rux_class > );\
		implement_get_static_Type_for_template( Array< XArray< X##rux_class > > );\
		implement_get_static_Type_for_template( Array< XArray< XKeyValuePair< X##rux_class > > > );\
		implement_get_static_Type_for_template( Array< XKeyValuePair< XArray< X##rux_class > > > );\
		implement_get_static_Type_for_template( Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > > > );\
		implement_get_static_Type_for_template( Array< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > );\
		implement_get_static_Type_for_template( KeyValuePair< XArray< X##rux_class > > );\
		implement_get_static_Type_for_template( KeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > > );\
		implement_get_static_Type_for_template( KeyValuePair< XArray< XKeyValuePair< X##rux_class > > > );\
		implement_get_static_Type_for_template( Array< XKeyValuePair< X##rux_class > > );\
		template<> XGCRef& Array< XKeyValuePair< X##rux_class > >::static_CheckAllItemType( const XGCRef* object , const XGCRef* param0 )\
		{\
			Array< XKeyValuePair< X##rux_class > >* arr = (Array< XKeyValuePair< X##rux_class > >*)object->get_ObjectAddress();\
			bool res = false;\
			size_t count = arr->Count();\
			if( count > 0 )\
			{\
				Type* type = (Type*)param0->get_ObjectAddress();\
				for( size_t index0 = 0 ; index0 < count ; index0++ )\
				{\
					if( rux_is_object_and_type_equal( arr->operator[]( index0 ) , type ) )\
					{\
						res = true;\
						break;\
					}\
				}\
			}\
			return ::rux::XBoolean( res )++;\
		};\
		template<> XGCRef& Array< XKeyValuePair< X##rux_class > >::static_get_Item( const XGCRef* object , const XGCRef* param0 )\
		{\
			Array< XKeyValuePair< X##rux_class > >* arr = (Array< XKeyValuePair< X##rux_class > >*)object->get_ObjectAddress();\
			return arr->operator[]( (size_t)::rux::gcref_to_uint64( param0 ) );\
		};\
		template<> XGCRef& Array< X##rux_class >::static_CheckAllItemType( const XGCRef* object , const XGCRef* param0 )\
		{\
			Array< X##rux_class >* arr = (Array< X##rux_class >*)object->get_ObjectAddress();\
			bool res = false;\
			size_t count = arr->Count();\
			if( count > 0 )\
			{\
				Type* type = (Type*)param0->get_ObjectAddress();\
				for( size_t index0 = 0 ; index0 < count ; index0++ )\
				{\
					if( rux_is_object_and_type_equal( arr->operator[]( index0 ) , type ) )\
					{\
						res = true;\
						break;\
					}\
				}\
			}\
			return ::rux::XBoolean( res )++;\
		};\
		template<> XGCRef& Array< X##rux_class >::static_get_Item( const XGCRef* object , const XGCRef* param0 )\
		{\
			Array< X##rux_class >* arr = (Array< X##rux_class >*)object->get_ObjectAddress();\
			return arr->operator[]( (size_t)::rux::gcref_to_uint64( param0 ) );\
		};\
		template<> XGCRef& Array< XArray< X##rux_class > >::static_CheckAllItemType( const XGCRef* object , const XGCRef* param0 )\
		{\
			Array< XArray< X##rux_class > >* arr = (Array< XArray< X##rux_class > >*)object->get_ObjectAddress();\
			bool res = false;\
			size_t count = arr->Count();\
			if( count > 0 )\
			{\
				Type* type = (Type*)param0->get_ObjectAddress();\
				for( size_t index0 = 0 ; index0 < count ; index0++ )\
				{\
					if( rux_is_object_and_type_equal( arr->operator[]( index0 ) , type ) )\
					{\
						res = true;\
						break;\
					}\
				}\
			}\
			return ::rux::XBoolean( res )++;\
		};\
		template<> XGCRef& Array< XArray< X##rux_class > >::static_get_Item( const XGCRef* object , const XGCRef* param0 )\
		{\
			Array< XArray< X##rux_class > >* arr = (Array< XArray< X##rux_class > >*)object->get_ObjectAddress();\
			return arr->operator[]( (size_t)::rux::gcref_to_uint64( param0 ) );\
		};\
		template<> XGCRef& Array< XArray< XKeyValuePair< X##rux_class > > >::static_CheckAllItemType( const XGCRef* object , const XGCRef* param0 )\
		{\
			Array< XArray< XKeyValuePair< X##rux_class > > >* arr = (Array< XArray< XKeyValuePair< X##rux_class > > >*)object->get_ObjectAddress();\
			bool res = false;\
			size_t count = arr->Count();\
			if( count > 0 )\
			{\
				Type* type = (Type*)param0->get_ObjectAddress();\
				for( size_t index0 = 0 ; index0 < count ; index0++ )\
				{\
					if( rux_is_object_and_type_equal( arr->operator[]( index0 ) , type ) )\
					{\
						res = true;\
						break;\
					}\
				}\
			}\
			return ::rux::XBoolean( res )++;\
		};\
		template<> XGCRef& Array< XArray< XKeyValuePair< X##rux_class > > >::static_get_Item( const XGCRef* object , const XGCRef* param0 )\
		{\
			Array< XArray< XKeyValuePair< X##rux_class > > >* arr = (Array< XArray< XKeyValuePair< X##rux_class > > >*)object->get_ObjectAddress();\
			return arr->operator[]( (size_t)::rux::gcref_to_uint64( param0 ) );\
		};\
		template<> XGCRef& Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > > >::static_CheckAllItemType( const XGCRef* object , const XGCRef* param0 )\
		{\
			Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > > >* arr = (Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > > >*)object->get_ObjectAddress();\
			bool res = false;\
			size_t count = arr->Count();\
			if( count > 0 )\
			{\
				Type* type = (Type*)param0->get_ObjectAddress();\
				for( size_t index0 = 0 ; index0 < count ; index0++ )\
				{\
					if( rux_is_object_and_type_equal( arr->operator[]( index0 ) , type ) )\
					{\
						res = true;\
						break;\
					}\
				}\
			}\
			return ::rux::XBoolean( res )++;\
		};\
		template<> XGCRef& Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > > >::static_get_Item( const XGCRef* object , const XGCRef* param0 )\
		{\
			Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > > >* arr = (Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > > >*)object->get_ObjectAddress();\
			return arr->operator[]( (size_t)::rux::gcref_to_uint64( param0 ) );\
		};\
		template<> XGCRef& Array< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > >::static_CheckAllItemType( const XGCRef* object , const XGCRef* param0 )\
		{\
			Array< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > >* arr = (Array< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > >*)object->get_ObjectAddress();\
			bool res = false;\
			size_t count = arr->Count();\
			if( count > 0 )\
			{\
				Type* type = (Type*)param0->get_ObjectAddress();\
				for( size_t index0 = 0 ; index0 < count ; index0++ )\
				{\
					if( rux_is_object_and_type_equal( arr->operator[]( index0 ) , type ) )\
					{\
						res = true;\
						break;\
					}\
				}\
			}\
			return ::rux::XBoolean( res )++;\
		};\
		template<> XGCRef& Array< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > >::static_get_Item( const XGCRef* object , const XGCRef* param0 )\
		{\
			Array< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > >* arr = (Array< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > >*)object->get_ObjectAddress();\
			return arr->operator[]( (size_t)::rux::gcref_to_uint64( param0 ) );\
		};\
		template<> XGCRef& Array< XKeyValuePair< XArray< X##rux_class > > >::static_CheckAllItemType( const XGCRef* object , const XGCRef* param0 )\
		{\
			Array< XKeyValuePair< XArray< X##rux_class > > >* arr = (Array< XKeyValuePair< XArray< X##rux_class > > >*)object->get_ObjectAddress();\
			bool res = false;\
			size_t count = arr->Count();\
			if( count > 0 )\
			{\
				Type* type = (Type*)param0->get_ObjectAddress();\
				for( size_t index0 = 0 ; index0 < count ; index0++ )\
				{\
					if( rux_is_object_and_type_equal( arr->operator[]( index0 ) , type ) )\
					{\
						res = true;\
						break;\
					}\
				}\
			}\
			return ::rux::XBoolean( res )++;\
		};\
		template<> XGCRef& Array< XKeyValuePair< XArray< X##rux_class > > >::static_get_Item( const XGCRef* object , const XGCRef* param0 )\
		{\
			Array< XKeyValuePair< XArray< X##rux_class > > >* arr = (Array< XKeyValuePair< XArray< X##rux_class > > >*)object->get_ObjectAddress();\
			return arr->operator[]( (size_t)::rux::gcref_to_uint64( param0 ) );\
		};\
		template<> XGCRef& Array< XKeyValuePair< X##rux_class > >::static_Count( const XGCRef* object )\
		{\
			Array< XKeyValuePair< X##rux_class > >* arr = (Array< XKeyValuePair< X##rux_class > >*)object->get_ObjectAddress();\
			return ::rux::XUInt64( arr->Count() )++;\
		};\
		template<> XGCRef& Array< X##rux_class >::static_Count( const XGCRef* object )\
		{\
			Array< X##rux_class >* arr = (Array< X##rux_class >*)object->get_ObjectAddress();\
			return ::rux::XUInt64( arr->Count() )++;\
		};\
		template<> XGCRef& Array< XArray< X##rux_class > >::static_Count( const XGCRef* object )\
		{\
			Array< XArray< X##rux_class > >* arr = (Array< XArray< X##rux_class > >*)object->get_ObjectAddress();\
			return ::rux::XUInt64( arr->Count() )++;\
		};\
		template<> XGCRef& Array< XArray< XKeyValuePair< X##rux_class > > >::static_Count( const XGCRef* object )\
		{\
			Array< XArray< XKeyValuePair< X##rux_class > > >* arr = (Array< XArray< XKeyValuePair< X##rux_class > > >*)object->get_ObjectAddress();\
			return ::rux::XUInt64( arr->Count() )++;\
		};\
		template<> XGCRef& Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > > >::static_Count( const XGCRef* object )\
		{\
			Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > > >* arr = (Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > > >*)object->get_ObjectAddress();\
			return ::rux::XUInt64( arr->Count() )++;\
		};\
		template<> XGCRef& Array< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > >::static_Count( const XGCRef* object )\
		{\
			Array< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > >* arr = (Array< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > >*)object->get_ObjectAddress();\
			return ::rux::XUInt64( arr->Count() )++;\
		};\
		template<> XGCRef& Array< XKeyValuePair< XArray< X##rux_class > > >::static_Count( const XGCRef* object )\
		{\
			Array< XKeyValuePair< XArray< X##rux_class > > >* arr = (Array< XKeyValuePair< XArray< X##rux_class > > >*)object->get_ObjectAddress();\
			return ::rux::XUInt64( arr->Count() )++;\
		};\
		template<> void Array< XKeyValuePair< X##rux_class > >::register_methods_and_properties( Type* type )\
		{\
			XKeyValuePair< X##rux_class > param( "obj" , __FILE__ , __LINE__ );\
			type->RegisterFunction( 0 , "Add" , (void*)Array< XKeyValuePair< X##rux_class > >::static_Add , &param.get_Type() );\
			type->RegisterFunction( 0 , "AddByRef" , (void*)Array< XKeyValuePair< X##rux_class > >::static_AddByRef , &param.get_Type() );\
			::rux::XUInt64 uint64_value;\
			type->RegisterFunction( 1 , "get_Item" , (void*)Array< XKeyValuePair< X##rux_class > >::static_get_Item , &uint64_value.get_Type() );\
			type->RegisterFunction( 1 , "CheckAllItemType" , (void*)Array< XKeyValuePair< X##rux_class > >::static_CheckAllItemType , typeofptr( ::XType ) );\
			type->RegisterProperty( "Count" , &uint64_value.get_Type() , NULL , (void*)Array< XKeyValuePair< X##rux_class > >::static_Count );\
		};\
		template<> void Array< X##rux_class >::register_methods_and_properties( Type* type )\
		{\
			X##rux_class param( "obj" , __FILE__ , __LINE__ );\
			type->RegisterFunction( 0 , "Add" , (void*)Array< X##rux_class >::static_Add , &param.get_Type() );\
			type->RegisterFunction( 0 , "AddByRef" , (void*)Array< X##rux_class >::static_AddByRef , &param.get_Type() );\
			::rux::XUInt64 uint64_value;\
			type->RegisterFunction( 1 , "get_Item" , (void*)Array< X##rux_class >::static_get_Item , &uint64_value.get_Type() );\
			type->RegisterFunction( 1 , "CheckAllItemType" , (void*)Array< X##rux_class >::static_CheckAllItemType , typeofptr( ::XType ) );\
			type->RegisterProperty( "Count" , &uint64_value.get_Type() , NULL , (void*)Array< X##rux_class >::static_Count );\
		};\
		template<> void Array< XArray< X##rux_class > >::register_methods_and_properties( Type* type )\
		{\
			XArray< X##rux_class > param( "obj" , __FILE__ , __LINE__ );\
			type->RegisterFunction( 0 , "Add" , (void*)Array< XArray< X##rux_class > >::static_Add , &param.get_Type() );\
			type->RegisterFunction( 0 , "AddByRef" , (void*)Array< XArray< X##rux_class > >::static_AddByRef , &param.get_Type() );\
			::rux::XUInt64 uint64_value;\
			type->RegisterFunction( 1 , "get_Item" , (void*)Array< XArray< X##rux_class > >::static_get_Item , &uint64_value.get_Type() );\
			type->RegisterFunction( 1 , "CheckAllItemType" , (void*)Array< XArray< X##rux_class > >::static_CheckAllItemType , typeofptr( ::XType ) );\
			type->RegisterProperty( "Count" , &uint64_value.get_Type() , NULL , (void*)Array< XArray< X##rux_class > >::static_Count );\
		};\
		template<> void Array< XArray< XKeyValuePair< X##rux_class > > >::register_methods_and_properties( Type* type )\
		{\
			XArray< XKeyValuePair< X##rux_class > > param( "obj" , __FILE__ , __LINE__ );\
			type->RegisterFunction( 0 , "Add" , (void*)Array< XArray< XKeyValuePair< X##rux_class > > >::static_Add , &param.get_Type() );\
			type->RegisterFunction( 0 , "AddByRef" , (void*)Array< XArray< XKeyValuePair< X##rux_class > > >::static_AddByRef , &param.get_Type() );\
			::rux::XUInt64 uint64_value;\
			type->RegisterFunction( 1 , "get_Item" , (void*)Array< XArray< XKeyValuePair< X##rux_class > > >::static_get_Item , &uint64_value.get_Type() );\
			type->RegisterFunction( 1 , "CheckAllItemType" , (void*)Array< XArray< XKeyValuePair< X##rux_class > > >::static_CheckAllItemType , typeofptr( ::XType ) );\
			type->RegisterProperty( "Count" , &uint64_value.get_Type() , NULL , (void*)Array< XArray< XKeyValuePair< X##rux_class > > >::static_Count );\
		};\
		template<> void Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > > >::register_methods_and_properties( Type* type )\
		{\
			XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > > param( "obj" , __FILE__ , __LINE__ );\
			type->RegisterFunction( 0 , "Add" , (void*)Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > > >::static_Add , &param.get_Type() );\
			type->RegisterFunction( 0 , "AddByRef" , (void*)Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > > >::static_AddByRef , &param.get_Type() );\
			::rux::XUInt64 uint64_value;\
			type->RegisterFunction( 1 , "get_Item" , (void*)Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > > >::static_get_Item , &uint64_value.get_Type() );\
			type->RegisterFunction( 1 , "CheckAllItemType" , (void*)Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > > >::static_CheckAllItemType , typeofptr( ::XType ) );\
			type->RegisterProperty( "Count" , &uint64_value.get_Type() , NULL , (void*)Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > > >::static_Count );\
		};\
		template<> void Array< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > >::register_methods_and_properties( Type* type )\
		{\
			XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > param( "obj" , __FILE__ , __LINE__ );\
			type->RegisterFunction( 0 , "Add" , (void*)Array< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > >::static_Add , &param.get_Type() );\
			type->RegisterFunction( 0 , "AddByRef" , (void*)Array< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > >::static_AddByRef , &param.get_Type() );\
			::rux::XUInt64 uint64_value;\
			type->RegisterFunction( 1 , "get_Item" , (void*)Array< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > >::static_get_Item , &uint64_value.get_Type() );\
			type->RegisterFunction( 1 , "CheckAllItemType" , (void*)Array< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > >::static_CheckAllItemType , typeofptr( ::XType ) );\
			type->RegisterProperty( "Count" , &uint64_value.get_Type() , NULL , (void*)Array< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > >::static_Count );\
		};\
		template<> void Array< XKeyValuePair< XArray< X##rux_class > > >::register_methods_and_properties( Type* type )\
		{\
			XKeyValuePair< XArray< X##rux_class > > param( "obj" , __FILE__ , __LINE__ );\
			type->RegisterFunction( 0 , "Add" , (void*)Array< XKeyValuePair< XArray< X##rux_class > > >::static_Add , &param.get_Type() );\
			type->RegisterFunction( 0 , "AddByRef" , (void*)Array< XKeyValuePair< XArray< X##rux_class > > >::static_AddByRef , &param.get_Type() );\
			::rux::XUInt64 uint64_value;\
			type->RegisterFunction( 1 , "get_Item" , (void*)Array< XKeyValuePair< XArray< X##rux_class > > >::static_get_Item , &uint64_value.get_Type() );\
			type->RegisterFunction( 1 , "CheckAllItemType" , (void*)Array< XKeyValuePair< XArray< X##rux_class > > >::static_CheckAllItemType , typeofptr( ::XType ) );\
			type->RegisterProperty( "Count" , &uint64_value.get_Type() , NULL , (void*)Array< XKeyValuePair< XArray< X##rux_class > > >::static_Count );\
		};\
		template<> ::rux::register_type_t Array< X##rux_class >::_register_methods_and_properties = Array< X##rux_class >::register_methods_and_properties;\
		template<> ::rux::register_type_t Array< XKeyValuePair< X##rux_class > >::_register_methods_and_properties = Array< XKeyValuePair< X##rux_class > >::register_methods_and_properties;\
		template<> ::rux::register_type_t Array< XKeyValuePair< XArray< X##rux_class > > >::_register_methods_and_properties = Array< XKeyValuePair< XArray< X##rux_class > > >::register_methods_and_properties;\
		template<> ::rux::register_type_t Array< XArray< X##rux_class > >::_register_methods_and_properties = Array< XArray< X##rux_class > >::register_methods_and_properties;\
		template<> ::rux::register_type_t Array< XArray< XKeyValuePair< X##rux_class > > >::_register_methods_and_properties = Array< XArray< XKeyValuePair< X##rux_class > > >::register_methods_and_properties;\
		template<> ::rux::register_type_t Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > > >::_register_methods_and_properties = Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > > >::register_methods_and_properties;\
		template<> ::rux::register_type_t Array< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > >::_register_methods_and_properties = Array< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > >::register_methods_and_properties;\
	}
#define implement_rux_base_methods_for_object( rux_class ) implement_rux_base_methods_for_object_( rux_class , NULL )
#define implement_rux_base_methods_for_object_( rux_class , rux_register_methods_and_properties ) const char* X##rux_class::get_Alias( void )\
	{\
		return #rux_class;\
	};\
	const char* rux_class::get_Alias( void )\
	{\
		return #rux_class;\
	};\
	const void* rux_class::get_ObjectAddress( void ) const\
	{\
		return this;\
	};\
	implement_get_static_Type( rux_class );\
	rux_volatile rux_class::_rux_type_index = rux_volatile_max;\
	::rux::uint8 rux_class::_rux_is_array = 0;\
	::rux::register_type_t rux_class::_register_methods_and_properties = rux_register_methods_and_properties;\
	namespace rux\
	{\
		template class XKeyValuePair< X##rux_class >;\
		template class XArray< XKeyValuePair< X##rux_class > >;\
		template class XArray< X##rux_class >;\
		template<> rux_volatile Array< X##rux_class >::_rux_type_index = rux_volatile_max;\
		template<> rux_volatile KeyValuePair< X##rux_class >::_rux_type_index = rux_volatile_max;\
		template<> rux_volatile Array< XKeyValuePair< X##rux_class > >::_rux_type_index = rux_volatile_max;\
		template<> rux_volatile Array< XKeyValuePair< XArray< X##rux_class > > >::_rux_type_index = rux_volatile_max;\
		template<> rux_volatile Array< XArray< X##rux_class > >::_rux_type_index = rux_volatile_max;\
		template<> rux_volatile Array< XArray< XKeyValuePair< X##rux_class > > >::_rux_type_index = rux_volatile_max;\
		template<> rux_volatile Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > > >::_rux_type_index = rux_volatile_max;\
		template<> rux_volatile Array< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > >::_rux_type_index = rux_volatile_max;\
		template<> rux_volatile KeyValuePair< XArray< X##rux_class > >::_rux_type_index = rux_volatile_max;\
		template<> rux_volatile KeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > >::_rux_type_index = rux_volatile_max;\
		template<> rux_volatile KeyValuePair< XArray< XKeyValuePair< X##rux_class > > >::_rux_type_index = rux_volatile_max;\
		template<> ::rux::uint8 Array< X##rux_class >::_rux_is_array = 1;\
		template<> ::rux::uint8 KeyValuePair< X##rux_class >::_rux_is_array = 0;\
		template<> ::rux::uint8 Array< XKeyValuePair< X##rux_class > >::_rux_is_array = 1;\
		template<> ::rux::uint8 Array< XKeyValuePair< XArray< X##rux_class > > >::_rux_is_array = 1;\
		template<> ::rux::uint8 Array< XArray< X##rux_class > >::_rux_is_array = 1;\
		template<> ::rux::uint8 Array< XArray< XKeyValuePair< X##rux_class > > >::_rux_is_array = 1;\
		template<> ::rux::uint8 Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > > >::_rux_is_array = 1;\
		template<> ::rux::uint8 Array< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > >::_rux_is_array = 1;\
		template<> ::rux::uint8 KeyValuePair< XArray< X##rux_class > >::_rux_is_array = 0;\
		template<> ::rux::uint8 KeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > >::_rux_is_array = 0;\
		template<> ::rux::uint8 KeyValuePair< XArray< XKeyValuePair< X##rux_class > > >::_rux_is_array = 0;\
		template<> ::rux::register_type_t KeyValuePair< X##rux_class >::_register_methods_and_properties = NULL;\
		template<> ::rux::register_type_t KeyValuePair< XArray< X##rux_class > >::_register_methods_and_properties = NULL;\
		template<> ::rux::register_type_t KeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > >::_register_methods_and_properties = NULL;\
		template<> ::rux::register_type_t KeyValuePair< XArray< XKeyValuePair< X##rux_class > > >::_register_methods_and_properties = NULL;\
		template<> const char* KeyValuePair< X##rux_class >::get_Alias( void )\
		{\
			return "KeyValuePair< "#rux_class" >";\
		};\
		template<> const char* Array< X##rux_class >::get_Alias( void )\
		{\
			return "Array< "#rux_class" >";\
		};\
		template<> const char* Array< XKeyValuePair< X##rux_class > >::get_Alias( void )\
		{\
			return "Array< KeyValuePair< "#rux_class" > >";\
		};\
		template<> const char* Array< XArray< XKeyValuePair< X##rux_class > > >::get_Alias( void )\
		{\
			return "Array< Array< KeyValuePair< "#rux_class" > > >";\
		};\
		template<> const char* Array< XArray< X##rux_class > >::get_Alias( void )\
		{\
			return "Array< Array< "#rux_class" > >";\
		};\
		template<> const char* Array< XKeyValuePair< XArray< X##rux_class > > >::get_Alias( void )\
		{\
			return "Array< KeyValuePair< Array< "#rux_class" > > >";\
		};\
		template<> const char* Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > > >::get_Alias( void )\
		{\
			return "Array< KeyValuePair< Array< KeyValuePair< Array< KeyValuePair< "#rux_class" > > > > > >";\
		};\
		template<> const char* Array< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > >::get_Alias( void )\
		{\
			return "Array< KeyValuePair< Array< KeyValuePair< "#rux_class" > > > >";\
		};\
		template<> const char* KeyValuePair< XArray< X##rux_class > >::get_Alias( void )\
		{\
			return "KeyValuePair< Array< "#rux_class" > >";\
		};\
		template<> const char* KeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > >::get_Alias( void )\
		{\
			return "KeyValuePair< Array< KeyValuePair< Array< KeyValuePair< "#rux_class" > > > > >";\
		};\
		template<> const char* KeyValuePair< XArray< XKeyValuePair< X##rux_class > > >::get_Alias( void )\
		{\
			return "KeyValuePair< Array< KeyValuePair< "#rux_class" > > >";\
		};\
		template<> const void* KeyValuePair< X##rux_class >::get_ObjectAddress( void ) const\
		{\
			return this;\
		};\
		template<> const void* Array< X##rux_class >::get_ObjectAddress( void ) const\
		{\
			return this;\
		};\
		template<> const void* Array< XKeyValuePair< X##rux_class > >::get_ObjectAddress( void ) const\
		{\
			return this;\
		};\
		template<> const void* Array< XArray< XKeyValuePair< X##rux_class > > >::get_ObjectAddress( void ) const\
		{\
			return this;\
		};\
		template<> const void* Array< XArray< X##rux_class > >::get_ObjectAddress( void ) const\
		{\
			return this;\
		};\
		template<> const void* Array< XKeyValuePair< XArray< X##rux_class > > >::get_ObjectAddress( void ) const\
		{\
			return this;\
		};\
		template<> const void* Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > > >::get_ObjectAddress( void ) const\
		{\
			return this;\
		};\
		template<> const void* Array< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > >::get_ObjectAddress( void ) const\
		{\
			return this;\
		};\
		template<> const void* KeyValuePair< XArray< X##rux_class > >::get_ObjectAddress( void ) const\
		{\
			return this;\
		};\
		template<> const void* KeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > >::get_ObjectAddress( void ) const\
		{\
			return this;\
		};\
		template<> const void* KeyValuePair< XArray< XKeyValuePair< X##rux_class > > >::get_ObjectAddress( void ) const\
		{\
			return this;\
		};\
		template<> void KeyValuePair< X##rux_class >::register_type( Type* type )\
		{\
			type->RegisterConstructor( (void*)KeyValuePair< X##rux_class >::ctor );\
			if( KeyValuePair< X##rux_class >::_register_methods_and_properties )\
				KeyValuePair< X##rux_class >::_register_methods_and_properties( type );\
		};\
		template<> void Array< X##rux_class >::register_type( Type* type )\
		{\
			type->RegisterConstructor( (void*)Array< X##rux_class >::ctor );\
			if( Array< X##rux_class >::_register_methods_and_properties )\
				Array< X##rux_class >::_register_methods_and_properties( type );\
		};\
		template<> void Array< XArray< X##rux_class > >::register_type( Type* type )\
		{\
			type->RegisterConstructor( (void*)Array< XArray< X##rux_class > >::ctor );\
			if( Array< XArray< X##rux_class > >::_register_methods_and_properties )\
				Array< XArray< X##rux_class > >::_register_methods_and_properties( type );\
		};\
		template<> void Array< XArray< XKeyValuePair< X##rux_class > > >::register_type( Type* type )\
		{\
			type->RegisterConstructor( (void*)Array< XArray< XKeyValuePair< X##rux_class > > >::ctor );\
			if( Array< XArray< XKeyValuePair< X##rux_class > > >::_register_methods_and_properties )\
				Array< XArray< XKeyValuePair< X##rux_class > > >::_register_methods_and_properties( type );\
		};\
		template<> void Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > > >::register_type( Type* type )\
		{\
			type->RegisterConstructor( (void*)Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > > >::ctor );\
			if( Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > > >::_register_methods_and_properties )\
				Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > > >::_register_methods_and_properties( type );\
		};\
		template<> void Array< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > >::register_type( Type* type )\
		{\
			type->RegisterConstructor( (void*)Array< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > >::ctor );\
			if( Array< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > >::_register_methods_and_properties )\
				Array< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > >::_register_methods_and_properties( type );\
		};\
		template<> void KeyValuePair< XArray< X##rux_class > >::register_type( Type* type )\
		{\
			type->RegisterConstructor( (void*)KeyValuePair< XArray< X##rux_class > >::ctor );\
			if( KeyValuePair< XArray< X##rux_class > >::_register_methods_and_properties )\
				KeyValuePair< XArray< X##rux_class > >::_register_methods_and_properties( type );\
		};\
		template<> void KeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > >::register_type( Type* type )\
		{\
			type->RegisterConstructor( (void*)KeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > >::ctor );\
			if( KeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > >::_register_methods_and_properties )\
				KeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > >::_register_methods_and_properties( type );\
		};\
		template<> void KeyValuePair< XArray< XKeyValuePair< X##rux_class > > >::register_type( Type* type )\
		{\
			type->RegisterConstructor( (void*)KeyValuePair< XArray< XKeyValuePair< X##rux_class > > >::ctor );\
			if( KeyValuePair< XArray< XKeyValuePair< X##rux_class > > >::_register_methods_and_properties )\
				KeyValuePair< XArray< XKeyValuePair< X##rux_class > > >::_register_methods_and_properties( type );\
		};\
		template<> void Array< XKeyValuePair< XArray< X##rux_class > > >::register_type( Type* type )\
		{\
			type->RegisterConstructor( (void*)Array< XKeyValuePair< XArray< X##rux_class > > >::ctor );\
			if( Array< XKeyValuePair< XArray< X##rux_class > > >::_register_methods_and_properties )\
				Array< XKeyValuePair< XArray< X##rux_class > > >::_register_methods_and_properties( type );\
		};\
		template<> void Array< XKeyValuePair< X##rux_class > >::register_type( Type* type )\
		{\
			type->RegisterConstructor( (void*)Array< XKeyValuePair< X##rux_class > >::ctor );\
			if( Array< XKeyValuePair< X##rux_class > >::_register_methods_and_properties )\
				Array< XKeyValuePair< X##rux_class > >::_register_methods_and_properties( type );\
		};\
		template<> Type* KeyValuePair< XArray< X##rux_class > >::get_rux_array_item_type( void )\
		{\
			return NULL;\
		};\
		template<> Type* KeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > >::get_rux_array_item_type( void )\
		{\
			return NULL;\
		};\
		template<> Type* KeyValuePair< XArray< XKeyValuePair< X##rux_class > > >::get_rux_array_item_type( void )\
		{\
			return NULL;\
		};\
		template<> Type* KeyValuePair< X##rux_class >::get_rux_array_item_type( void )\
		{\
			return NULL;\
		};\
		template<> Type* Array< XKeyValuePair< X##rux_class > >::get_rux_array_item_type( void )\
		{\
			return _typeof( XKeyValuePair< X##rux_class >::get_Alias() , &XKeyValuePair< X##rux_class >::InternalClass::_rux_type_index , XKeyValuePair< X##rux_class >::InternalClass::register_type , XKeyValuePair< X##rux_class >::InternalClass::_rux_is_array , XKeyValuePair< X##rux_class >::InternalClass::get_rux_array_item_type() );\
		};\
		template<> Type* Array< X##rux_class >::get_rux_array_item_type( void )\
		{\
			return _typeof( X##rux_class::get_Alias() , &X##rux_class::InternalClass::_rux_type_index , X##rux_class::InternalClass::register_type , X##rux_class::InternalClass::_rux_is_array , X##rux_class::InternalClass::get_rux_array_item_type() );\
		};\
		template<> Type* Array< XArray< X##rux_class > >::get_rux_array_item_type( void )\
		{\
			return _typeof( XArray< X##rux_class >::get_Alias() , &XArray< X##rux_class >::InternalClass::_rux_type_index , XArray< X##rux_class >::InternalClass::register_type , XArray< X##rux_class >::InternalClass::_rux_is_array , XArray< X##rux_class >::InternalClass::get_rux_array_item_type() );\
		};\
		template<> Type* Array< XArray< XKeyValuePair< X##rux_class > > >::get_rux_array_item_type( void )\
		{\
			return _typeof( XArray< XKeyValuePair< X##rux_class > >::get_Alias() , &XArray< XKeyValuePair< X##rux_class > >::InternalClass::_rux_type_index , XArray< XKeyValuePair< X##rux_class > >::InternalClass::register_type , XArray< XKeyValuePair< X##rux_class > >::InternalClass::_rux_is_array , XArray< XKeyValuePair< X##rux_class > >::InternalClass::get_rux_array_item_type() );\
		};\
		template<> Type* Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > > >::get_rux_array_item_type( void )\
		{\
			return _typeof( XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > >::get_Alias() , &XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > >::InternalClass::_rux_type_index , XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > >::InternalClass::register_type , XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > >::InternalClass::_rux_is_array , XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > >::InternalClass::get_rux_array_item_type() );\
		};\
		template<> Type* Array< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > >::get_rux_array_item_type( void )\
		{\
			return _typeof( XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > >::get_Alias() , &XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > >::InternalClass::_rux_type_index , XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > >::InternalClass::register_type , XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > >::InternalClass::_rux_is_array , XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > >::InternalClass::get_rux_array_item_type() );\
		};\
		template<> Type* Array< XKeyValuePair< XArray< X##rux_class > > >::get_rux_array_item_type( void )\
		{\
			return _typeof( XKeyValuePair< XArray< X##rux_class > >::get_Alias() , &XKeyValuePair< XArray< X##rux_class > >::InternalClass::_rux_type_index , XKeyValuePair< XArray< X##rux_class > >::InternalClass::register_type , XKeyValuePair< XArray< X##rux_class > >::InternalClass::_rux_is_array , XKeyValuePair< XArray< X##rux_class > >::InternalClass::get_rux_array_item_type() );\
		};\
		implement_get_static_RuxTypeIndex_for_template( KeyValuePair< X##rux_class > );\
		implement_get_static_RuxTypeIndex_for_template( Array< X##rux_class > );\
		implement_get_static_RuxTypeIndex_for_template( Array< XArray< X##rux_class > > );\
		implement_get_static_RuxTypeIndex_for_template( Array< XArray< XKeyValuePair< X##rux_class > > > );\
		implement_get_static_RuxTypeIndex_for_template( Array< XKeyValuePair< XArray< X##rux_class > > > );\
		implement_get_static_RuxTypeIndex_for_template( Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > > > );\
		implement_get_static_RuxTypeIndex_for_template( Array< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > );\
		implement_get_static_RuxTypeIndex_for_template( KeyValuePair< XArray< X##rux_class > > );\
		implement_get_static_RuxTypeIndex_for_template( KeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > > );\
		implement_get_static_RuxTypeIndex_for_template( KeyValuePair< XArray< XKeyValuePair< X##rux_class > > > );\
		implement_get_static_RuxTypeIndex_for_template( Array< XKeyValuePair< X##rux_class > > );\
		implement_get_static_Type_for_template( KeyValuePair< X##rux_class > );\
		implement_get_static_Type_for_template( Array< X##rux_class > );\
		implement_get_static_Type_for_template( Array< XArray< X##rux_class > > );\
		implement_get_static_Type_for_template( Array< XArray< XKeyValuePair< X##rux_class > > > );\
		implement_get_static_Type_for_template( Array< XKeyValuePair< XArray< X##rux_class > > > );\
		implement_get_static_Type_for_template( Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > > > );\
		implement_get_static_Type_for_template( Array< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > );\
		implement_get_static_Type_for_template( KeyValuePair< XArray< X##rux_class > > );\
		implement_get_static_Type_for_template( KeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > > );\
		implement_get_static_Type_for_template( KeyValuePair< XArray< XKeyValuePair< X##rux_class > > > );\
		implement_get_static_Type_for_template( Array< XKeyValuePair< X##rux_class > > );\
		template<> XGCRef& Array< XKeyValuePair< X##rux_class > >::static_CheckAllItemType( const XGCRef* object , const XGCRef* param0 )\
		{\
			Array< XKeyValuePair< X##rux_class > >* arr = (Array< XKeyValuePair< X##rux_class > >*)object->get_ObjectAddress();\
			bool res = false;\
			size_t count = arr->Count();\
			if( count > 0 )\
			{\
				Type* type = (Type*)param0->get_ObjectAddress();\
				for( size_t index0 = 0 ; index0 < count ; index0++ )\
				{\
					if( rux_is_object_and_type_equal( arr->operator[]( index0 ) , type ) )\
					{\
						res = true;\
						break;\
					}\
				}\
			}\
			return ::rux::XBoolean( res )++;\
		};\
		template<> XGCRef& Array< XKeyValuePair< X##rux_class > >::static_get_Item( const XGCRef* object , const XGCRef* param0 )\
		{\
			Array< XKeyValuePair< X##rux_class > >* arr = (Array< XKeyValuePair< X##rux_class > >*)object->get_ObjectAddress();\
			return arr->operator[]( (size_t)::rux::gcref_to_uint64( param0 ) );\
		};\
		template<> XGCRef& Array< X##rux_class >::static_CheckAllItemType( const XGCRef* object , const XGCRef* param0 )\
		{\
			Array< X##rux_class >* arr = (Array< X##rux_class >*)object->get_ObjectAddress();\
			bool res = false;\
			size_t count = arr->Count();\
			if( count > 0 )\
			{\
				Type* type = (Type*)param0->get_ObjectAddress();\
				for( size_t index0 = 0 ; index0 < count ; index0++ )\
				{\
					if( rux_is_object_and_type_equal( arr->operator[]( index0 ) , type ) )\
					{\
						res = true;\
						break;\
					}\
				}\
			}\
			return ::rux::XBoolean( res )++;\
		};\
		template<> XGCRef& Array< X##rux_class >::static_get_Item( const XGCRef* object , const XGCRef* param0 )\
		{\
			Array< X##rux_class >* arr = (Array< X##rux_class >*)object->get_ObjectAddress();\
			return arr->operator[]( (size_t)::rux::gcref_to_uint64( param0 ) );\
		};\
		template<> XGCRef& Array< XArray< X##rux_class > >::static_CheckAllItemType( const XGCRef* object , const XGCRef* param0 )\
		{\
			Array< XArray< X##rux_class > >* arr = (Array< XArray< X##rux_class > >*)object->get_ObjectAddress();\
			bool res = false;\
			size_t count = arr->Count();\
			if( count > 0 )\
			{\
				Type* type = (Type*)param0->get_ObjectAddress();\
				for( size_t index0 = 0 ; index0 < count ; index0++ )\
				{\
					if( rux_is_object_and_type_equal( arr->operator[]( index0 ) , type ) )\
					{\
						res = true;\
						break;\
					}\
				}\
			}\
			return ::rux::XBoolean( res )++;\
		};\
		template<> XGCRef& Array< XArray< X##rux_class > >::static_get_Item( const XGCRef* object , const XGCRef* param0 )\
		{\
			Array< XArray< X##rux_class > >* arr = (Array< XArray< X##rux_class > >*)object->get_ObjectAddress();\
			return arr->operator[]( (size_t)::rux::gcref_to_uint64( param0 ) );\
		};\
		template<> XGCRef& Array< XArray< XKeyValuePair< X##rux_class > > >::static_CheckAllItemType( const XGCRef* object , const XGCRef* param0 )\
		{\
			Array< XArray< XKeyValuePair< X##rux_class > > >* arr = (Array< XArray< XKeyValuePair< X##rux_class > > >*)object->get_ObjectAddress();\
			bool res = false;\
			size_t count = arr->Count();\
			if( count > 0 )\
			{\
				Type* type = (Type*)param0->get_ObjectAddress();\
				for( size_t index0 = 0 ; index0 < count ; index0++ )\
				{\
					if( rux_is_object_and_type_equal( arr->operator[]( index0 ) , type ) )\
					{\
						res = true;\
						break;\
					}\
				}\
			}\
			return ::rux::XBoolean( res )++;\
		};\
		template<> XGCRef& Array< XArray< XKeyValuePair< X##rux_class > > >::static_get_Item( const XGCRef* object , const XGCRef* param0 )\
		{\
			Array< XArray< XKeyValuePair< X##rux_class > > >* arr = (Array< XArray< XKeyValuePair< X##rux_class > > >*)object->get_ObjectAddress();\
			return arr->operator[]( (size_t)::rux::gcref_to_uint64( param0 ) );\
		};\
		template<> XGCRef& Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > > >::static_CheckAllItemType( const XGCRef* object , const XGCRef* param0 )\
		{\
			Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > > >* arr = (Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > > >*)object->get_ObjectAddress();\
			bool res = false;\
			size_t count = arr->Count();\
			if( count > 0 )\
			{\
				Type* type = (Type*)param0->get_ObjectAddress();\
				for( size_t index0 = 0 ; index0 < count ; index0++ )\
				{\
					if( rux_is_object_and_type_equal( arr->operator[]( index0 ) , type ) )\
					{\
						res = true;\
						break;\
					}\
				}\
			}\
			return ::rux::XBoolean( res )++;\
		};\
		template<> XGCRef& Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > > >::static_get_Item( const XGCRef* object , const XGCRef* param0 )\
		{\
			Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > > >* arr = (Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > > >*)object->get_ObjectAddress();\
			return arr->operator[]( (size_t)::rux::gcref_to_uint64( param0 ) );\
		};\
		template<> XGCRef& Array< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > >::static_CheckAllItemType( const XGCRef* object , const XGCRef* param0 )\
		{\
			Array< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > >* arr = (Array< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > >*)object->get_ObjectAddress();\
			bool res = false;\
			size_t count = arr->Count();\
			if( count > 0 )\
			{\
				Type* type = (Type*)param0->get_ObjectAddress();\
				for( size_t index0 = 0 ; index0 < count ; index0++ )\
				{\
					if( rux_is_object_and_type_equal( arr->operator[]( index0 ) , type ) )\
					{\
						res = true;\
						break;\
					}\
				}\
			}\
			return ::rux::XBoolean( res )++;\
		};\
		template<> XGCRef& Array< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > >::static_get_Item( const XGCRef* object , const XGCRef* param0 )\
		{\
			Array< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > >* arr = (Array< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > >*)object->get_ObjectAddress();\
			return arr->operator[]( (size_t)::rux::gcref_to_uint64( param0 ) );\
		};\
		template<> XGCRef& Array< XKeyValuePair< XArray< X##rux_class > > >::static_CheckAllItemType( const XGCRef* object , const XGCRef* param0 )\
		{\
			Array< XKeyValuePair< XArray< X##rux_class > > >* arr = (Array< XKeyValuePair< XArray< X##rux_class > > >*)object->get_ObjectAddress();\
			bool res = false;\
			size_t count = arr->Count();\
			if( count > 0 )\
			{\
				Type* type = (Type*)param0->get_ObjectAddress();\
				for( size_t index0 = 0 ; index0 < count ; index0++ )\
				{\
					if( rux_is_object_and_type_equal( arr->operator[]( index0 ) , type ) )\
					{\
						res = true;\
						break;\
					}\
				}\
			}\
			return ::rux::XBoolean( res )++;\
		};\
		template<> XGCRef& Array< XKeyValuePair< XArray< X##rux_class > > >::static_get_Item( const XGCRef* object , const XGCRef* param0 )\
		{\
			Array< XKeyValuePair< XArray< X##rux_class > > >* arr = (Array< XKeyValuePair< XArray< X##rux_class > > >*)object->get_ObjectAddress();\
			return arr->operator[]( (size_t)::rux::gcref_to_uint64( param0 ) );\
		};\
		template<> XGCRef& Array< XKeyValuePair< X##rux_class > >::static_Count( const XGCRef* object )\
		{\
			Array< XKeyValuePair< X##rux_class > >* arr = (Array< XKeyValuePair< X##rux_class > >*)object->get_ObjectAddress();\
			return ::rux::XUInt64( arr->Count() )++;\
		};\
		template<> XGCRef& Array< X##rux_class >::static_Count( const XGCRef* object )\
		{\
			Array< X##rux_class >* arr = (Array< X##rux_class >*)object->get_ObjectAddress();\
			return ::rux::XUInt64( arr->Count() )++;\
		};\
		template<> XGCRef& Array< XArray< X##rux_class > >::static_Count( const XGCRef* object )\
		{\
			Array< XArray< X##rux_class > >* arr = (Array< XArray< X##rux_class > >*)object->get_ObjectAddress();\
			return ::rux::XUInt64( arr->Count() )++;\
		};\
		template<> XGCRef& Array< XArray< XKeyValuePair< X##rux_class > > >::static_Count( const XGCRef* object )\
		{\
			Array< XArray< XKeyValuePair< X##rux_class > > >* arr = (Array< XArray< XKeyValuePair< X##rux_class > > >*)object->get_ObjectAddress();\
			return ::rux::XUInt64( arr->Count() )++;\
		};\
		template<> XGCRef& Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > > >::static_Count( const XGCRef* object )\
		{\
			Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > > >* arr = (Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > > >*)object->get_ObjectAddress();\
			return ::rux::XUInt64( arr->Count() )++;\
		};\
		template<> XGCRef& Array< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > >::static_Count( const XGCRef* object )\
		{\
			Array< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > >* arr = (Array< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > >*)object->get_ObjectAddress();\
			return ::rux::XUInt64( arr->Count() )++;\
		};\
		template<> XGCRef& Array< XKeyValuePair< XArray< X##rux_class > > >::static_Count( const XGCRef* object )\
		{\
			Array< XKeyValuePair< XArray< X##rux_class > > >* arr = (Array< XKeyValuePair< XArray< X##rux_class > > >*)object->get_ObjectAddress();\
			return ::rux::XUInt64( arr->Count() )++;\
		};\
		template<> void Array< XKeyValuePair< X##rux_class > >::register_methods_and_properties( Type* type )\
		{\
			XKeyValuePair< X##rux_class > param( "obj" , __FILE__ , __LINE__ );\
			type->RegisterFunction( 0 , "Add" , (void*)Array< XKeyValuePair< X##rux_class > >::static_Add , &param.get_Type() );\
			type->RegisterFunction( 0 , "AddByRef" , (void*)Array< XKeyValuePair< X##rux_class > >::static_AddByRef , &param.get_Type() );\
			::rux::XUInt64 uint64_value;\
			type->RegisterFunction( 1 , "get_Item" , (void*)Array< XKeyValuePair< X##rux_class > >::static_get_Item , &uint64_value.get_Type() );\
			type->RegisterFunction( 1 , "CheckAllItemType" , (void*)Array< XKeyValuePair< X##rux_class > >::static_CheckAllItemType , typeofptr( ::XType ) );\
			type->RegisterProperty( "Count" , &uint64_value.get_Type() , NULL , (void*)Array< XKeyValuePair< X##rux_class > >::static_Count );\
		};\
		template<> void Array< X##rux_class >::register_methods_and_properties( Type* type )\
		{\
			X##rux_class param( "obj" , __FILE__ , __LINE__ );\
			type->RegisterFunction( 0 , "Add" , (void*)Array< X##rux_class >::static_Add , &param.get_Type() );\
			type->RegisterFunction( 0 , "AddByRef" , (void*)Array< X##rux_class >::static_AddByRef , &param.get_Type() );\
			::rux::XUInt64 uint64_value;\
			type->RegisterFunction( 1 , "get_Item" , (void*)Array< X##rux_class >::static_get_Item , &uint64_value.get_Type() );\
			type->RegisterFunction( 1 , "CheckAllItemType" , (void*)Array< X##rux_class >::static_CheckAllItemType , typeofptr( ::XType ) );\
			type->RegisterProperty( "Count" , &uint64_value.get_Type() , NULL , (void*)Array< X##rux_class >::static_Count );\
		};\
		template<> void Array< XArray< X##rux_class > >::register_methods_and_properties( Type* type )\
		{\
			XArray< X##rux_class > param( "obj" , __FILE__ , __LINE__ );\
			type->RegisterFunction( 0 , "Add" , (void*)Array< XArray< X##rux_class > >::static_Add , &param.get_Type() );\
			type->RegisterFunction( 0 , "AddByRef" , (void*)Array< XArray< X##rux_class > >::static_AddByRef , &param.get_Type() );\
			::rux::XUInt64 uint64_value;\
			type->RegisterFunction( 1 , "get_Item" , (void*)Array< XArray< X##rux_class > >::static_get_Item , &uint64_value.get_Type() );\
			type->RegisterFunction( 1 , "CheckAllItemType" , (void*)Array< XArray< X##rux_class > >::static_CheckAllItemType , typeofptr( ::XType ) );\
			type->RegisterProperty( "Count" , &uint64_value.get_Type() , NULL , (void*)Array< XArray< X##rux_class > >::static_Count );\
		};\
		template<> void Array< XArray< XKeyValuePair< X##rux_class > > >::register_methods_and_properties( Type* type )\
		{\
			XArray< XKeyValuePair< X##rux_class > > param( "obj" , __FILE__ , __LINE__ );\
			type->RegisterFunction( 0 , "Add" , (void*)Array< XArray< XKeyValuePair< X##rux_class > > >::static_Add , &param.get_Type() );\
			type->RegisterFunction( 0 , "AddByRef" , (void*)Array< XArray< XKeyValuePair< X##rux_class > > >::static_AddByRef , &param.get_Type() );\
			::rux::XUInt64 uint64_value;\
			type->RegisterFunction( 1 , "get_Item" , (void*)Array< XArray< XKeyValuePair< X##rux_class > > >::static_get_Item , &uint64_value.get_Type() );\
			type->RegisterFunction( 1 , "CheckAllItemType" , (void*)Array< XArray< XKeyValuePair< X##rux_class > > >::static_CheckAllItemType , typeofptr( ::XType ) );\
			type->RegisterProperty( "Count" , &uint64_value.get_Type() , NULL , (void*)Array< XArray< XKeyValuePair< X##rux_class > > >::static_Count );\
		};\
		template<> void Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > > >::register_methods_and_properties( Type* type )\
		{\
			XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > > param( "obj" , __FILE__ , __LINE__ );\
			type->RegisterFunction( 0 , "Add" , (void*)Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > > >::static_Add , &param.get_Type() );\
			type->RegisterFunction( 0 , "AddByRef" , (void*)Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > > >::static_AddByRef , &param.get_Type() );\
			::rux::XUInt64 uint64_value;\
			type->RegisterFunction( 1 , "get_Item" , (void*)Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > > >::static_get_Item , &uint64_value.get_Type() );\
			type->RegisterFunction( 1 , "CheckAllItemType" , (void*)Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > > >::static_CheckAllItemType , typeofptr( ::XType ) );\
			type->RegisterProperty( "Count" , &uint64_value.get_Type() , NULL , (void*)Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > > >::static_Count );\
		};\
		template<> void Array< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > >::register_methods_and_properties( Type* type )\
		{\
			XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > param( "obj" , __FILE__ , __LINE__ );\
			type->RegisterFunction( 0 , "Add" , (void*)Array< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > >::static_Add , &param.get_Type() );\
			type->RegisterFunction( 0 , "AddByRef" , (void*)Array< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > >::static_AddByRef , &param.get_Type() );\
			::rux::XUInt64 uint64_value;\
			type->RegisterFunction( 1 , "get_Item" , (void*)Array< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > >::static_get_Item , &uint64_value.get_Type() );\
			type->RegisterFunction( 1 , "CheckAllItemType" , (void*)Array< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > >::static_CheckAllItemType , typeofptr( ::XType ) );\
			type->RegisterProperty( "Count" , &uint64_value.get_Type() , NULL , (void*)Array< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > >::static_Count );\
		};\
		template<> void Array< XKeyValuePair< XArray< X##rux_class > > >::register_methods_and_properties( Type* type )\
		{\
			XKeyValuePair< XArray< X##rux_class > > param( "obj" , __FILE__ , __LINE__ );\
			type->RegisterFunction( 0 , "Add" , (void*)Array< XKeyValuePair< XArray< X##rux_class > > >::static_Add , &param.get_Type() );\
			type->RegisterFunction( 0 , "AddByRef" , (void*)Array< XKeyValuePair< XArray< X##rux_class > > >::static_AddByRef , &param.get_Type() );\
			::rux::XUInt64 uint64_value;\
			type->RegisterFunction( 1 , "get_Item" , (void*)Array< XKeyValuePair< XArray< X##rux_class > > >::static_get_Item , &uint64_value.get_Type() );\
			type->RegisterFunction( 1 , "CheckAllItemType" , (void*)Array< XKeyValuePair< XArray< X##rux_class > > >::static_CheckAllItemType , typeofptr( ::XType ) );\
			type->RegisterProperty( "Count" , &uint64_value.get_Type() , NULL , (void*)Array< XKeyValuePair< XArray< X##rux_class > > >::static_Count );\
		};\
		template<> ::rux::register_type_t Array< XKeyValuePair< X##rux_class > >::_register_methods_and_properties = Array< XKeyValuePair< X##rux_class > >::register_methods_and_properties;\
		template<> ::rux::register_type_t Array< XKeyValuePair< XArray< X##rux_class > > >::_register_methods_and_properties = Array< XKeyValuePair< XArray< X##rux_class > > >::register_methods_and_properties;\
		template<> ::rux::register_type_t Array< XArray< X##rux_class > >::_register_methods_and_properties = Array< XArray< X##rux_class > >::register_methods_and_properties;\
		template<> ::rux::register_type_t Array< XArray< XKeyValuePair< X##rux_class > > >::_register_methods_and_properties = Array< XArray< XKeyValuePair< X##rux_class > > >::register_methods_and_properties;\
		template<> ::rux::register_type_t Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > > >::_register_methods_and_properties = Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > > > >::register_methods_and_properties;\
		template<> ::rux::register_type_t Array< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > >::_register_methods_and_properties = Array< XKeyValuePair< XArray< XKeyValuePair< X##rux_class > > > >::register_methods_and_properties;\
		template<> ::rux::register_type_t Array< X##rux_class >::_register_methods_and_properties = Array< X##rux_class >::register_methods_and_properties;\
	}
#define implement_rux_set_operators_ns_( rux_class , rux_namespace ) rux_namespace::X##rux_class& rux_namespace::X##rux_class::operator=( const Object& object )\
	{\
		*static_cast<XGCHandle< rux_namespace::rux_class >*>(this) = object;\
		return *this;\
	};\
	rux_namespace::X##rux_class& rux_namespace::X##rux_class::operator =( const XObject& object )\
	{\
		(*(static_cast< XGCHandle< rux_namespace::rux_class >* >( this ))) = object;\
		return *this;\
	};\
	rux_namespace::X##rux_class& rux_namespace::X##rux_class::operator =( XGCRef* gcref )\
	{\
		(*(static_cast< XGCHandle< rux_namespace::rux_class >* >( this ))) = gcref;\
		return *this;\
	};\
	rux_namespace::X##rux_class& rux_namespace::X##rux_class::operator=( const XGCRef& gcref )\
	{\
		*static_cast<XGCHandle< rux_namespace::rux_class >*>(this) = gcref;\
		return *this;\
	}
#define implement_rux_base_methods_ns( rux_class , rux_namespace ) implement_rux_base_methods_ns_( rux_class , rux_namespace , NULL )
#define implement_rux_base_methods_ns_( rux_class , rux_namespace , rux_register_methods_and_properties ) const char* rux_namespace::X##rux_class::get_Alias( void )\
	{\
		return #rux_class;\
	};\
	rux_namespace::X##rux_class::X##rux_class( const XGCRef& gcref )\
	{\
		*this = gcref;\
	};\
	rux_namespace::X##rux_class::X##rux_class( const char* variable_name , const char* __file__ , ::rux::int32 __line__ )\
		: XGCHandle< rux_namespace::rux_class >( variable_name , __file__ , __line__ )\
	{\
		x_initialize();\
	};\
	rux_namespace::X##rux_class::X##rux_class( const XGCRef& gcref , const char* variable_name , const char* __file__ , ::rux::int32 __line__ )\
		: XGCHandle< rux_namespace::rux_class >( variable_name , __file__ , __line__ )\
	{\
		*this = gcref;\
	};\
	rux_namespace::X##rux_class::X##rux_class( const Object& object , const char* variable_name , const char* __file__ , ::rux::int32 __line__ )\
		: XGCHandle< rux_namespace::rux_class >( variable_name , __file__ , __line__ )\
	{\
		*static_cast<XGCHandle< rux_namespace::rux_class >*>(this) = object;\
	};\
	rux_namespace::X##rux_class::X##rux_class( const XObject& object , const char* variable_name , const char* __file__ , ::rux::int32 __line__ )\
		: XGCHandle< rux_class >( variable_name , __file__ , __line__ )\
	{\
		(*this) = object;\
	};\
	rux_namespace::X##rux_class::X##rux_class( const Object& object )\
	{\
		*static_cast<XGCHandle< rux_namespace::rux_class >*>(this) = object;\
	};\
	const void* rux_namespace::rux_class::get_ObjectAddress( void ) const\
	{\
		return this;\
	};\
	const char* rux_namespace::rux_class::get_Alias( void )\
	{\
		return #rux_class;\
	};\
	implement_get_static_Type_ns( rux_namespace , rux_class );\
	rux_volatile rux_namespace::rux_class::_rux_type_index = rux_volatile_max;\
	::rux::uint8 rux_namespace::rux_class::_rux_is_array = 0;\
	::rux::register_type_t rux_namespace::rux_class::_register_methods_and_properties = rux_register_methods_and_properties;\
	namespace rux\
	{\
		template class XKeyValuePair< rux_namespace::X##rux_class >;\
		template class XArray< XKeyValuePair< rux_namespace::X##rux_class > >;\
		template class XArray< rux_namespace::X##rux_class >;\
		template<> rux_volatile Array< rux_namespace::X##rux_class >::_rux_type_index = rux_volatile_max;\
		template<> rux_volatile KeyValuePair< rux_namespace::X##rux_class >::_rux_type_index = rux_volatile_max;\
		template<> rux_volatile Array< XKeyValuePair< rux_namespace::X##rux_class > >::_rux_type_index = rux_volatile_max;\
		template<> rux_volatile Array< XKeyValuePair< XArray< rux_namespace::X##rux_class > > >::_rux_type_index = rux_volatile_max;\
		template<> rux_volatile Array< XArray< rux_namespace::X##rux_class > >::_rux_type_index = rux_volatile_max;\
		template<> rux_volatile Array< XArray< XKeyValuePair< rux_namespace::X##rux_class > > >::_rux_type_index = rux_volatile_max;\
		template<> rux_volatile Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > > > >::_rux_type_index = rux_volatile_max;\
		template<> rux_volatile Array< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > >::_rux_type_index = rux_volatile_max;\
		template<> rux_volatile KeyValuePair< XArray< rux_namespace::X##rux_class > >::_rux_type_index = rux_volatile_max;\
		template<> rux_volatile KeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > > >::_rux_type_index = rux_volatile_max;\
		template<> rux_volatile KeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > >::_rux_type_index = rux_volatile_max;\
		template<> ::rux::uint8 Array< rux_namespace::X##rux_class >::_rux_is_array = 1;\
		template<> ::rux::uint8 KeyValuePair< rux_namespace::X##rux_class >::_rux_is_array = 0;\
		template<> ::rux::uint8 Array< XKeyValuePair< rux_namespace::X##rux_class > >::_rux_is_array = 1;\
		template<> ::rux::uint8 Array< XKeyValuePair< XArray< rux_namespace::X##rux_class > > >::_rux_is_array = 1;\
		template<> ::rux::uint8 Array< XArray< rux_namespace::X##rux_class > >::_rux_is_array = 1;\
		template<> ::rux::uint8 Array< XArray< XKeyValuePair< rux_namespace::X##rux_class > > >::_rux_is_array = 1;\
		template<> ::rux::uint8 Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > > > >::_rux_is_array = 1;\
		template<> ::rux::uint8 Array< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > >::_rux_is_array = 1;\
		template<> ::rux::uint8 KeyValuePair< XArray< rux_namespace::X##rux_class > >::_rux_is_array = 0;\
		template<> ::rux::uint8 KeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > > >::_rux_is_array = 0;\
		template<> ::rux::uint8 KeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > >::_rux_is_array = 0;\
		template<> ::rux::register_type_t KeyValuePair< rux_namespace::X##rux_class >::_register_methods_and_properties = NULL;\
		template<> ::rux::register_type_t KeyValuePair< XArray< rux_namespace::X##rux_class > >::_register_methods_and_properties = NULL;\
		template<> ::rux::register_type_t KeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > > >::_register_methods_and_properties = NULL;\
		template<> ::rux::register_type_t KeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > >::_register_methods_and_properties = NULL;\
		template<> const char* KeyValuePair< rux_namespace::X##rux_class >::get_Alias( void )\
		{\
			return "KeyValuePair< "#rux_class" >";\
		};\
		template<> const char* Array< rux_namespace::X##rux_class >::get_Alias( void )\
		{\
			return "Array< "#rux_class" >";\
		};\
		template<> const char* Array< XKeyValuePair< rux_namespace::X##rux_class > >::get_Alias( void )\
		{\
			return "Array< KeyValuePair< "#rux_class" > >";\
		};\
		template<> const char* Array< XKeyValuePair< XArray< rux_namespace::X##rux_class > > >::get_Alias( void )\
		{\
			return "Array< KeyValuePair< Array< "#rux_class" > > >";\
		};\
		template<> const char* Array< XArray< XKeyValuePair< rux_namespace::X##rux_class > > >::get_Alias( void )\
		{\
			return "Array< Array< KeyValuePair< "#rux_class" > > >";\
		};\
		template<> const char* Array< XArray< rux_namespace::X##rux_class > >::get_Alias( void )\
		{\
			return "Array< Array< "#rux_class" > >";\
		};\
		template<> const char* Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > > > >::get_Alias( void )\
		{\
			return "Array< KeyValuePair< Array< KeyValuePair< Array< KeyValuePair< "#rux_class" > > > > > >";\
		};\
		template<> const char* Array< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > >::get_Alias( void )\
		{\
			return "Array< KeyValuePair< Array< KeyValuePair< "#rux_class" > > > >";\
		};\
		template<> const char* KeyValuePair< XArray< rux_namespace::X##rux_class > >::get_Alias( void )\
		{\
			return "KeyValuePair< Array< "#rux_class" > >";\
		};\
		template<> const char* KeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > > >::get_Alias( void )\
		{\
			return "KeyValuePair< Array< KeyValuePair< Array< KeyValuePair< "#rux_class" > > > > >";\
		};\
		template<> const char* KeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > >::get_Alias( void )\
		{\
			return "KeyValuePair< Array< KeyValuePair< "#rux_class" > > >";\
		};\
		template<> const void* KeyValuePair< rux_namespace::X##rux_class >::get_ObjectAddress( void ) const\
		{\
			return this;\
		};\
		template<> const void* Array< rux_namespace::X##rux_class >::get_ObjectAddress( void ) const\
		{\
			return this;\
		};\
		template<> const void* Array< XKeyValuePair< rux_namespace::X##rux_class > >::get_ObjectAddress( void ) const\
		{\
			return this;\
		};\
		template<> const void* Array< XKeyValuePair< XArray< rux_namespace::X##rux_class > > >::get_ObjectAddress( void ) const\
		{\
			return this;\
		};\
		template<> const void* Array< XArray< XKeyValuePair< rux_namespace::X##rux_class > > >::get_ObjectAddress( void ) const\
		{\
			return this;\
		};\
		template<> const void* Array< XArray< rux_namespace::X##rux_class > >::get_ObjectAddress( void ) const\
		{\
			return this;\
		};\
		template<> const void* Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > > > >::get_ObjectAddress( void ) const\
		{\
			return this;\
		};\
		template<> const void* Array< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > >::get_ObjectAddress( void ) const\
		{\
			return this;\
		};\
		template<> const void* KeyValuePair< XArray< rux_namespace::X##rux_class > >::get_ObjectAddress( void ) const\
		{\
			return this;\
		};\
		template<> const void* KeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > > >::get_ObjectAddress( void ) const\
		{\
			return this;\
		};\
		template<> const void* KeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > >::get_ObjectAddress( void ) const\
		{\
			return this;\
		};\
		template<> void KeyValuePair< rux_namespace::X##rux_class >::register_type( Type* type )\
		{\
			type->RegisterConstructor( (void*)KeyValuePair< rux_namespace::X##rux_class >::ctor );\
			if( KeyValuePair< rux_namespace::X##rux_class >::_register_methods_and_properties )\
				KeyValuePair< rux_namespace::X##rux_class >::_register_methods_and_properties( type );\
		};\
		template<> void Array< rux_namespace::X##rux_class >::register_type( Type* type )\
		{\
			type->RegisterConstructor( (void*)Array< rux_namespace::X##rux_class >::ctor );\
			if( Array< rux_namespace::X##rux_class >::_register_methods_and_properties )\
				Array< rux_namespace::X##rux_class >::_register_methods_and_properties( type );\
		};\
		template<> void Array< XArray< rux_namespace::X##rux_class > >::register_type( Type* type )\
		{\
			type->RegisterConstructor( (void*)Array< XArray< rux_namespace::X##rux_class > >::ctor );\
			if( Array< XArray< rux_namespace::X##rux_class > >::_register_methods_and_properties )\
				Array< XArray< rux_namespace::X##rux_class > >::_register_methods_and_properties( type );\
		};\
		template<> void Array< XArray< XKeyValuePair< rux_namespace::X##rux_class > > >::register_type( Type* type )\
		{\
			type->RegisterConstructor( (void*)Array< XArray< XKeyValuePair< rux_namespace::X##rux_class > > >::ctor );\
			if( Array< XArray< XKeyValuePair< rux_namespace::X##rux_class > > >::_register_methods_and_properties )\
				Array< XArray< XKeyValuePair< rux_namespace::X##rux_class > > >::_register_methods_and_properties( type );\
		};\
		template<> void Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > > > >::register_type( Type* type )\
		{\
			type->RegisterConstructor( (void*)Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > > > >::ctor );\
			if( Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > > > >::_register_methods_and_properties )\
				Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > > > >::_register_methods_and_properties( type );\
		};\
		template<> void Array< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > >::register_type( Type* type )\
		{\
			type->RegisterConstructor( (void*)Array< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > >::ctor );\
			if( Array< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > >::_register_methods_and_properties )\
				Array< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > >::_register_methods_and_properties( type );\
		};\
		template<> void KeyValuePair< XArray< rux_namespace::X##rux_class > >::register_type( Type* type )\
		{\
			type->RegisterConstructor( (void*)KeyValuePair< XArray< rux_namespace::X##rux_class > >::ctor );\
			if( KeyValuePair< XArray< rux_namespace::X##rux_class > >::_register_methods_and_properties )\
				KeyValuePair< XArray< rux_namespace::X##rux_class > >::_register_methods_and_properties( type );\
		};\
		template<> void KeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > > >::register_type( Type* type )\
		{\
			type->RegisterConstructor( (void*)KeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > > >::ctor );\
			if( KeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > > >::_register_methods_and_properties )\
				KeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > > >::_register_methods_and_properties( type );\
		};\
		template<> void KeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > >::register_type( Type* type )\
		{\
			type->RegisterConstructor( (void*)KeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > >::ctor );\
			if( KeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > >::_register_methods_and_properties )\
				KeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > >::_register_methods_and_properties( type );\
		};\
		template<> void Array< XKeyValuePair< XArray< rux_namespace::X##rux_class > > >::register_type( Type* type )\
		{\
			type->RegisterConstructor( (void*)Array< XKeyValuePair< XArray< rux_namespace::X##rux_class > > >::ctor );\
			if( Array< XKeyValuePair< XArray< rux_namespace::X##rux_class > > >::_register_methods_and_properties )\
				Array< XKeyValuePair< XArray< rux_namespace::X##rux_class > > >::_register_methods_and_properties( type );\
		};\
		template<> void Array< XKeyValuePair< rux_namespace::X##rux_class > >::register_type( Type* type )\
		{\
			type->RegisterConstructor( (void*)Array< XKeyValuePair< rux_namespace::X##rux_class > >::ctor );\
			if( Array< XKeyValuePair< rux_namespace::X##rux_class > >::_register_methods_and_properties )\
				Array< XKeyValuePair< rux_namespace::X##rux_class > >::_register_methods_and_properties( type );\
		};\
		template<> Type* KeyValuePair< XArray< rux_namespace::X##rux_class > >::get_rux_array_item_type( void )\
		{\
			return NULL;\
		};\
		template<> Type* KeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > > >::get_rux_array_item_type( void )\
		{\
			return NULL;\
		};\
		template<> Type* KeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > >::get_rux_array_item_type( void )\
		{\
			return NULL;\
		};\
		template<> Type* KeyValuePair< rux_namespace::X##rux_class >::get_rux_array_item_type( void )\
		{\
			return NULL;\
		};\
		template<> Type* Array< XKeyValuePair< rux_namespace::X##rux_class > >::get_rux_array_item_type( void )\
		{\
			return _typeof( XKeyValuePair< rux_namespace::X##rux_class >::get_Alias() , &XKeyValuePair< rux_namespace::X##rux_class >::InternalClass::_rux_type_index , XKeyValuePair< rux_namespace::X##rux_class >::InternalClass::register_type , XKeyValuePair< rux_namespace::X##rux_class >::InternalClass::_rux_is_array , XKeyValuePair< rux_namespace::X##rux_class >::InternalClass::get_rux_array_item_type() );\
		};\
		template<> Type* Array< rux_namespace::X##rux_class >::get_rux_array_item_type( void )\
		{\
			return _typeof( rux_namespace::X##rux_class::get_Alias() , &rux_namespace::X##rux_class::InternalClass::_rux_type_index , rux_namespace::X##rux_class::InternalClass::register_type , rux_namespace::X##rux_class::InternalClass::_rux_is_array , rux_namespace::X##rux_class::InternalClass::get_rux_array_item_type() );\
		};\
		template<> Type* Array< XArray< rux_namespace::X##rux_class > >::get_rux_array_item_type( void )\
		{\
			return _typeof( XArray< rux_namespace::X##rux_class >::get_Alias() , &XArray< rux_namespace::X##rux_class >::InternalClass::_rux_type_index , XArray< rux_namespace::X##rux_class >::InternalClass::register_type , XArray< rux_namespace::X##rux_class >::InternalClass::_rux_is_array , XArray< rux_namespace::X##rux_class >::InternalClass::get_rux_array_item_type() );\
		};\
		template<> Type* Array< XArray< XKeyValuePair< rux_namespace::X##rux_class > > >::get_rux_array_item_type( void )\
		{\
			return _typeof( XArray< XKeyValuePair< rux_namespace::X##rux_class > >::get_Alias() , &XArray< XKeyValuePair< rux_namespace::X##rux_class > >::InternalClass::_rux_type_index , XArray< XKeyValuePair< rux_namespace::X##rux_class > >::InternalClass::register_type , XArray< XKeyValuePair< rux_namespace::X##rux_class > >::InternalClass::_rux_is_array , XArray< XKeyValuePair< rux_namespace::X##rux_class > >::InternalClass::get_rux_array_item_type() );\
		};\
		template<> Type* Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > > > >::get_rux_array_item_type( void )\
		{\
			return _typeof( XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > > >::get_Alias() , &XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > > >::InternalClass::_rux_type_index , XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > > >::InternalClass::register_type , XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > > >::InternalClass::_rux_is_array , XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > > >::InternalClass::get_rux_array_item_type() );\
		};\
		template<> Type* Array< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > >::get_rux_array_item_type( void )\
		{\
			return _typeof( XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > >::get_Alias() , &XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > >::InternalClass::_rux_type_index , XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > >::InternalClass::register_type , XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > >::InternalClass::_rux_is_array , XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > >::InternalClass::get_rux_array_item_type() );\
		};\
		template<> Type* Array< XKeyValuePair< XArray< rux_namespace::X##rux_class > > >::get_rux_array_item_type( void )\
		{\
			return _typeof( XKeyValuePair< XArray< rux_namespace::X##rux_class > >::get_Alias() , &XKeyValuePair< XArray< rux_namespace::X##rux_class > >::InternalClass::_rux_type_index , XKeyValuePair< XArray< rux_namespace::X##rux_class > >::InternalClass::register_type , XKeyValuePair< XArray< rux_namespace::X##rux_class > >::InternalClass::_rux_is_array , XKeyValuePair< XArray< rux_namespace::X##rux_class > >::InternalClass::get_rux_array_item_type() );\
		};\
		implement_get_static_RuxTypeIndex_for_template( KeyValuePair< rux_namespace::X##rux_class > );\
		implement_get_static_RuxTypeIndex_for_template( Array< rux_namespace::X##rux_class > );\
		implement_get_static_RuxTypeIndex_for_template( Array< XArray< rux_namespace::X##rux_class > > );\
		implement_get_static_RuxTypeIndex_for_template( Array< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > );\
		implement_get_static_RuxTypeIndex_for_template( Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > > > > );\
		implement_get_static_RuxTypeIndex_for_template( Array< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > > );\
		implement_get_static_RuxTypeIndex_for_template( KeyValuePair< XArray< rux_namespace::X##rux_class > > );\
		implement_get_static_RuxTypeIndex_for_template( KeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > > > );\
		implement_get_static_RuxTypeIndex_for_template( KeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > );\
		implement_get_static_RuxTypeIndex_for_template( Array< XKeyValuePair< XArray< rux_namespace::X##rux_class > > > );\
		implement_get_static_RuxTypeIndex_for_template( Array< XKeyValuePair< rux_namespace::X##rux_class > > );\
		implement_get_static_Type_for_template( KeyValuePair< rux_namespace::X##rux_class > );\
		implement_get_static_Type_for_template( Array< rux_namespace::X##rux_class > );\
		implement_get_static_Type_for_template( Array< XArray< rux_namespace::X##rux_class > > );\
		implement_get_static_Type_for_template( Array< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > );\
		implement_get_static_Type_for_template( Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > > > > );\
		implement_get_static_Type_for_template( Array< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > > );\
		implement_get_static_Type_for_template( KeyValuePair< XArray< rux_namespace::X##rux_class > > );\
		implement_get_static_Type_for_template( KeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > > > );\
		implement_get_static_Type_for_template( KeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > );\
		implement_get_static_Type_for_template( Array< XKeyValuePair< XArray< rux_namespace::X##rux_class > > > );\
		implement_get_static_Type_for_template( Array< XKeyValuePair< rux_namespace::X##rux_class > > );\
		template<> XGCRef& Array< XKeyValuePair< rux_namespace::X##rux_class > >::static_CheckAllItemType( const XGCRef* object , const XGCRef* param0 )\
		{\
			Array< XKeyValuePair< rux_namespace::X##rux_class > >* arr = (Array< XKeyValuePair< rux_namespace::X##rux_class > >*)object->get_ObjectAddress();\
			bool res = false;\
			size_t count = arr->Count();\
			if( count > 0 )\
			{\
				Type* type = (Type*)param0->get_ObjectAddress();\
				for( size_t index0 = 0 ; index0 < count ; index0++ )\
				{\
					if( rux_is_object_and_type_equal( arr->operator[]( index0 ) , type ) )\
					{\
						res = true;\
						break;\
					}\
				}\
			}\
			return ::rux::XBoolean( res )++;\
		};\
		template<> XGCRef& Array< XKeyValuePair< rux_namespace::X##rux_class > >::static_get_Item( const XGCRef* object , const XGCRef* param0 )\
		{\
			Array< XKeyValuePair< rux_namespace::X##rux_class > >* arr = (Array< XKeyValuePair< rux_namespace::X##rux_class > >*)object->get_ObjectAddress();\
			return arr->operator[]( (size_t)::rux::gcref_to_uint64( param0 ) );\
		};\
		template<> XGCRef& Array< rux_namespace::X##rux_class >::static_CheckAllItemType( const XGCRef* object , const XGCRef* param0 )\
		{\
			Array< rux_namespace::X##rux_class >* arr = (Array< rux_namespace::X##rux_class >*)object->get_ObjectAddress();\
			bool res = false;\
			size_t count = arr->Count();\
			if( count > 0 )\
			{\
				Type* type = (Type*)param0->get_ObjectAddress();\
				for( size_t index0 = 0 ; index0 < count ; index0++ )\
				{\
					if( rux_is_object_and_type_equal( arr->operator[]( index0 ) , type ) )\
					{\
						res = true;\
						break;\
					}\
				}\
			}\
			return ::rux::XBoolean( res )++;\
		};\
		template<> XGCRef& Array< rux_namespace::X##rux_class >::static_get_Item( const XGCRef* object , const XGCRef* param0 )\
		{\
			Array< rux_namespace::X##rux_class >* arr = (Array< rux_namespace::X##rux_class >*)object->get_ObjectAddress();\
			return arr->operator[]( (size_t)::rux::gcref_to_uint64( param0 ) );\
		};\
		template<> XGCRef& Array< XArray< rux_namespace::X##rux_class > >::static_CheckAllItemType( const XGCRef* object , const XGCRef* param0 )\
		{\
			Array< XArray< rux_namespace::X##rux_class > >* arr = (Array< XArray< rux_namespace::X##rux_class > >*)object->get_ObjectAddress();\
			bool res = false;\
			size_t count = arr->Count();\
			if( count > 0 )\
			{\
				Type* type = (Type*)param0->get_ObjectAddress();\
				for( size_t index0 = 0 ; index0 < count ; index0++ )\
				{\
					if( rux_is_object_and_type_equal( arr->operator[]( index0 ) , type ) )\
					{\
						res = true;\
						break;\
					}\
				}\
			}\
			return ::rux::XBoolean( res )++;\
		};\
		template<> XGCRef& Array< XArray< rux_namespace::X##rux_class > >::static_get_Item( const XGCRef* object , const XGCRef* param0 )\
		{\
			Array< XArray< rux_namespace::X##rux_class > >* arr = (Array< XArray< rux_namespace::X##rux_class > >*)object->get_ObjectAddress();\
			return arr->operator[]( (size_t)::rux::gcref_to_uint64( param0 ) );\
		};\
		template<> XGCRef& Array< XArray< XKeyValuePair< rux_namespace::X##rux_class > > >::static_CheckAllItemType( const XGCRef* object , const XGCRef* param0 )\
		{\
			Array< XArray< XKeyValuePair< rux_namespace::X##rux_class > > >* arr = (Array< XArray< XKeyValuePair< rux_namespace::X##rux_class > > >*)object->get_ObjectAddress();\
			bool res = false;\
			size_t count = arr->Count();\
			if( count > 0 )\
			{\
				Type* type = (Type*)param0->get_ObjectAddress();\
				for( size_t index0 = 0 ; index0 < count ; index0++ )\
				{\
					if( rux_is_object_and_type_equal( arr->operator[]( index0 ) , type ) )\
					{\
						res = true;\
						break;\
					}\
				}\
			}\
			return ::rux::XBoolean( res )++;\
		};\
		template<> XGCRef& Array< XArray< XKeyValuePair< rux_namespace::X##rux_class > > >::static_get_Item( const XGCRef* object , const XGCRef* param0 )\
		{\
			Array< XArray< XKeyValuePair< rux_namespace::X##rux_class > > >* arr = (Array< XArray< XKeyValuePair< rux_namespace::X##rux_class > > >*)object->get_ObjectAddress();\
			return arr->operator[]( (size_t)::rux::gcref_to_uint64( param0 ) );\
		};\
		template<> XGCRef& Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > > > >::static_CheckAllItemType( const XGCRef* object , const XGCRef* param0 )\
		{\
			Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > > > >* arr = (Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > > > >*)object->get_ObjectAddress();\
			bool res = false;\
			size_t count = arr->Count();\
			if( count > 0 )\
			{\
				Type* type = (Type*)param0->get_ObjectAddress();\
				for( size_t index0 = 0 ; index0 < count ; index0++ )\
				{\
					if( rux_is_object_and_type_equal( arr->operator[]( index0 ) , type ) )\
					{\
						res = true;\
						break;\
					}\
				}\
			}\
			return ::rux::XBoolean( res )++;\
		};\
		template<> XGCRef& Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > > > >::static_get_Item( const XGCRef* object , const XGCRef* param0 )\
		{\
			Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > > > >* arr = (Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > > > >*)object->get_ObjectAddress();\
			return arr->operator[]( (size_t)::rux::gcref_to_uint64( param0 ) );\
		};\
		template<> XGCRef& Array< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > >::static_CheckAllItemType( const XGCRef* object , const XGCRef* param0 )\
		{\
			Array< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > >* arr = (Array< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > >*)object->get_ObjectAddress();\
			bool res = false;\
			size_t count = arr->Count();\
			if( count > 0 )\
			{\
				Type* type = (Type*)param0->get_ObjectAddress();\
				for( size_t index0 = 0 ; index0 < count ; index0++ )\
				{\
					if( rux_is_object_and_type_equal( arr->operator[]( index0 ) , type ) )\
					{\
						res = true;\
						break;\
					}\
				}\
			}\
			return ::rux::XBoolean( res )++;\
		};\
		template<> XGCRef& Array< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > >::static_get_Item( const XGCRef* object , const XGCRef* param0 )\
		{\
			Array< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > >* arr = (Array< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > >*)object->get_ObjectAddress();\
			return arr->operator[]( (size_t)::rux::gcref_to_uint64( param0 ) );\
		};\
		template<> XGCRef& Array< XKeyValuePair< XArray< rux_namespace::X##rux_class > > >::static_CheckAllItemType( const XGCRef* object , const XGCRef* param0 )\
		{\
			Array< XKeyValuePair< XArray< rux_namespace::X##rux_class > > >* arr = (Array< XKeyValuePair< XArray< rux_namespace::X##rux_class > > >*)object->get_ObjectAddress();\
			bool res = false;\
			size_t count = arr->Count();\
			if( count > 0 )\
			{\
				Type* type = (Type*)param0->get_ObjectAddress();\
				for( size_t index0 = 0 ; index0 < count ; index0++ )\
				{\
					if( rux_is_object_and_type_equal( arr->operator[]( index0 ) , type ) )\
					{\
						res = true;\
						break;\
					}\
				}\
			}\
			return ::rux::XBoolean( res )++;\
		};\
		template<> XGCRef& Array< XKeyValuePair< XArray< rux_namespace::X##rux_class > > >::static_get_Item( const XGCRef* object , const XGCRef* param0 )\
		{\
			Array< XKeyValuePair< XArray< rux_namespace::X##rux_class > > >* arr = (Array< XKeyValuePair< XArray< rux_namespace::X##rux_class > > >*)object->get_ObjectAddress();\
			return arr->operator[]( (size_t)::rux::gcref_to_uint64( param0 ) );\
		};\
		template<> XGCRef& Array< XKeyValuePair< rux_namespace::X##rux_class > >::static_Count( const XGCRef* object )\
		{\
			Array< XKeyValuePair< rux_namespace::X##rux_class > >* arr = (Array< XKeyValuePair< rux_namespace::X##rux_class > >*)object->get_ObjectAddress();\
			return ::rux::XUInt64( arr->Count() )++;\
		};\
		template<> XGCRef& Array< rux_namespace::X##rux_class >::static_Count( const XGCRef* object )\
		{\
			Array< rux_namespace::X##rux_class >* arr = (Array< rux_namespace::X##rux_class >*)object->get_ObjectAddress();\
			return ::rux::XUInt64( arr->Count() )++;\
		};\
		template<> XGCRef& Array< XArray< rux_namespace::X##rux_class > >::static_Count( const XGCRef* object )\
		{\
			Array< XArray< rux_namespace::X##rux_class > >* arr = (Array< XArray< rux_namespace::X##rux_class > >*)object->get_ObjectAddress();\
			return ::rux::XUInt64( arr->Count() )++;\
		};\
		template<> XGCRef& Array< XArray< XKeyValuePair< rux_namespace::X##rux_class > > >::static_Count( const XGCRef* object )\
		{\
			Array< XArray< XKeyValuePair< rux_namespace::X##rux_class > > >* arr = (Array< XArray< XKeyValuePair< rux_namespace::X##rux_class > > >*)object->get_ObjectAddress();\
			return ::rux::XUInt64( arr->Count() )++;\
		};\
		template<> XGCRef& Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > > > >::static_Count( const XGCRef* object )\
		{\
			Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > > > >* arr = (Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > > > >*)object->get_ObjectAddress();\
			return ::rux::XUInt64( arr->Count() )++;\
		};\
		template<> XGCRef& Array< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > >::static_Count( const XGCRef* object )\
		{\
			Array< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > >* arr = (Array< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > >*)object->get_ObjectAddress();\
			return ::rux::XUInt64( arr->Count() )++;\
		};\
		template<> XGCRef& Array< XKeyValuePair< XArray< rux_namespace::X##rux_class > > >::static_Count( const XGCRef* object )\
		{\
			Array< XKeyValuePair< XArray< rux_namespace::X##rux_class > > >* arr = (Array< XKeyValuePair< XArray< rux_namespace::X##rux_class > > >*)object->get_ObjectAddress();\
			return ::rux::XUInt64( arr->Count() )++;\
		};\
		template<> void Array< XKeyValuePair< rux_namespace::X##rux_class > >::register_methods_and_properties( Type* type )\
		{\
			XKeyValuePair< rux_namespace::X##rux_class > param( "obj" , __FILE__ , __LINE__ );\
			type->RegisterFunction( 0 , "Add" , (void*)Array< XKeyValuePair< rux_namespace::X##rux_class > >::static_Add , &param.get_Type() );\
			type->RegisterFunction( 0 , "AddByRef" , (void*)Array< XKeyValuePair< rux_namespace::X##rux_class > >::static_AddByRef , &param.get_Type() );\
			::rux::XUInt64 uint64_value;\
			type->RegisterFunction( 1 , "get_Item" , (void*)Array< XKeyValuePair< rux_namespace::X##rux_class > >::static_get_Item , &uint64_value.get_Type() );\
			type->RegisterFunction( 1 , "CheckAllItemType" , (void*)Array< XKeyValuePair< rux_namespace::X##rux_class > >::static_CheckAllItemType , typeofptr( ::XType ) );\
			type->RegisterProperty( "Count" , &uint64_value.get_Type() , NULL , (void*)Array< XKeyValuePair< rux_namespace::X##rux_class > >::static_Count );\
		};\
		template<> void Array< rux_namespace::X##rux_class >::register_methods_and_properties( Type* type )\
		{\
			rux_namespace::X##rux_class param( "obj" , __FILE__ , __LINE__ );\
			type->RegisterFunction( 0 , "Add" , (void*)Array< rux_namespace::X##rux_class >::static_Add , &param.get_Type() );\
			type->RegisterFunction( 0 , "AddByRef" , (void*)Array< rux_namespace::X##rux_class >::static_AddByRef , &param.get_Type() );\
			::rux::XUInt64 uint64_value;\
			type->RegisterFunction( 1 , "get_Item" , (void*)Array< rux_namespace::X##rux_class >::static_get_Item , &uint64_value.get_Type() );\
			type->RegisterFunction( 1 , "CheckAllItemType" , (void*)Array< rux_namespace::X##rux_class >::static_CheckAllItemType , typeofptr( ::XType ) );\
			type->RegisterProperty( "Count" , &uint64_value.get_Type() , NULL , (void*)Array< rux_namespace::X##rux_class >::static_Count );\
		};\
		template<> void Array< XArray< rux_namespace::X##rux_class > >::register_methods_and_properties( Type* type )\
		{\
			XArray< rux_namespace::X##rux_class > param( "obj" , __FILE__ , __LINE__ );\
			type->RegisterFunction( 0 , "Add" , (void*)Array< XArray< rux_namespace::X##rux_class > >::static_Add , &param.get_Type() );\
			type->RegisterFunction( 0 , "AddByRef" , (void*)Array< XArray< rux_namespace::X##rux_class > >::static_AddByRef , &param.get_Type() );\
			::rux::XUInt64 uint64_value;\
			type->RegisterFunction( 1 , "get_Item" , (void*)Array< XArray< rux_namespace::X##rux_class > >::static_get_Item , &uint64_value.get_Type() );\
			type->RegisterFunction( 1 , "CheckAllItemType" , (void*)Array< XArray< rux_namespace::X##rux_class > >::static_CheckAllItemType , typeofptr( ::XType ) );\
			type->RegisterProperty( "Count" , &uint64_value.get_Type() , NULL , (void*)Array< XArray< rux_namespace::X##rux_class > >::static_Count );\
		};\
		template<> void Array< XArray< XKeyValuePair< rux_namespace::X##rux_class > > >::register_methods_and_properties( Type* type )\
		{\
			XArray< XKeyValuePair< rux_namespace::X##rux_class > > param( "obj" , __FILE__ , __LINE__ );\
			type->RegisterFunction( 0 , "Add" , (void*)Array< XArray< XKeyValuePair< rux_namespace::X##rux_class > > >::static_Add , &param.get_Type() );\
			type->RegisterFunction( 0 , "AddByRef" , (void*)Array< XArray< XKeyValuePair< rux_namespace::X##rux_class > > >::static_AddByRef , &param.get_Type() );\
			::rux::XUInt64 uint64_value;\
			type->RegisterFunction( 1 , "get_Item" , (void*)Array< XArray< XKeyValuePair< rux_namespace::X##rux_class > > >::static_get_Item , &uint64_value.get_Type() );\
			type->RegisterFunction( 1 , "CheckAllItemType" , (void*)Array< XArray< XKeyValuePair< rux_namespace::X##rux_class > > >::static_CheckAllItemType , typeofptr( ::XType ) );\
			type->RegisterProperty( "Count" , &uint64_value.get_Type() , NULL , (void*)Array< XArray< XKeyValuePair< rux_namespace::X##rux_class > > >::static_Count );\
		};\
		template<> void Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > > > >::register_methods_and_properties( Type* type )\
		{\
			XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > > > param( "obj" , __FILE__ , __LINE__ );\
			type->RegisterFunction( 0 , "Add" , (void*)Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > > > >::static_Add , &param.get_Type() );\
			type->RegisterFunction( 0 , "AddByRef" , (void*)Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > > > >::static_AddByRef , &param.get_Type() );\
			::rux::XUInt64 uint64_value;\
			type->RegisterFunction( 1 , "get_Item" , (void*)Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > > > >::static_get_Item , &uint64_value.get_Type() );\
			type->RegisterFunction( 1 , "CheckAllItemType" , (void*)Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > > > >::static_CheckAllItemType , typeofptr( ::XType ) );\
			type->RegisterProperty( "Count" , &uint64_value.get_Type() , NULL , (void*)Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > > > >::static_Count );\
		};\
		template<> void Array< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > >::register_methods_and_properties( Type* type )\
		{\
			XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > param( "obj" , __FILE__ , __LINE__ );\
			type->RegisterFunction( 0 , "Add" , (void*)Array< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > >::static_Add , &param.get_Type() );\
			type->RegisterFunction( 0 , "AddByRef" , (void*)Array< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > >::static_AddByRef , &param.get_Type() );\
			::rux::XUInt64 uint64_value;\
			type->RegisterFunction( 1 , "get_Item" , (void*)Array< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > >::static_get_Item , &uint64_value.get_Type() );\
			type->RegisterFunction( 1 , "CheckAllItemType" , (void*)Array< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > >::static_CheckAllItemType , typeofptr( ::XType ) );\
			type->RegisterProperty( "Count" , &uint64_value.get_Type() , NULL , (void*)Array< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > >::static_Count );\
		};\
		template<> void Array< XKeyValuePair< XArray< rux_namespace::X##rux_class > > >::register_methods_and_properties( Type* type )\
		{\
			XKeyValuePair< XArray< rux_namespace::X##rux_class > > param( "obj" , __FILE__ , __LINE__ );\
			type->RegisterFunction( 0 , "Add" , (void*)Array< XKeyValuePair< XArray< rux_namespace::X##rux_class > > >::static_Add , &param.get_Type() );\
			type->RegisterFunction( 0 , "AddByRef" , (void*)Array< XKeyValuePair< XArray< rux_namespace::X##rux_class > > >::static_AddByRef , &param.get_Type() );\
			::rux::XUInt64 uint64_value;\
			type->RegisterFunction( 1 , "get_Item" , (void*)Array< XKeyValuePair< XArray< rux_namespace::X##rux_class > > >::static_get_Item , &uint64_value.get_Type() );\
			type->RegisterFunction( 1 , "CheckAllItemType" , (void*)Array< XKeyValuePair< XArray< rux_namespace::X##rux_class > > >::static_CheckAllItemType , typeofptr( ::XType ) );\
			type->RegisterProperty( "Count" , &uint64_value.get_Type() , NULL , (void*)Array< XKeyValuePair< XArray< rux_namespace::X##rux_class > > >::static_Count );\
		};\
		template<> ::rux::register_type_t Array< XKeyValuePair< rux_namespace::X##rux_class > >::_register_methods_and_properties = Array< XKeyValuePair< rux_namespace::X##rux_class > >::register_methods_and_properties;\
		template<> ::rux::register_type_t Array< XKeyValuePair< XArray< rux_namespace::X##rux_class > > >::_register_methods_and_properties = Array< XKeyValuePair< XArray< rux_namespace::X##rux_class > > >::register_methods_and_properties;\
		template<> ::rux::register_type_t Array< XArray< rux_namespace::X##rux_class > >::_register_methods_and_properties = Array< XArray< rux_namespace::X##rux_class > >::register_methods_and_properties;\
		template<> ::rux::register_type_t Array< XArray< XKeyValuePair< rux_namespace::X##rux_class > > >::_register_methods_and_properties = Array< XArray< XKeyValuePair< rux_namespace::X##rux_class > > >::register_methods_and_properties;\
		template<> ::rux::register_type_t Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > > > >::_register_methods_and_properties = Array< XKeyValuePair< XArray< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > > > >::register_methods_and_properties;\
		template<> ::rux::register_type_t Array< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > >::_register_methods_and_properties = Array< XKeyValuePair< XArray< XKeyValuePair< rux_namespace::X##rux_class > > > >::register_methods_and_properties;\
		template<> ::rux::register_type_t Array< rux_namespace::X##rux_class >::_register_methods_and_properties = Array< rux_namespace::X##rux_class >::register_methods_and_properties;\
	}
#define declare_register_rux_property() static void register_methods_and_properties( Type* type )
#define declare_rux_property_without_member( rux_property_class , rux_property ) private:\
	static XGCRef& static_get_##rux_property( const XGCRef* object );\
	static void static_set_##rux_property( const XGCRef* object , const XGCRef* value )
#define declare_rux_property( rux_property_class , rux_property ) private:\
	rux_property_class _##rux_property;\
	declare_rux_property_without_member( rux_property_class , rux_property )
#define implement_rux_property( rux_class , rux_property ) XGCRef& rux_class::static_get_##rux_property( const XGCRef* object )\
	{\
		rux_class* obj = (rux_class*)object->get_ObjectAddress();\
		return obj->_##rux_property++;\
	};\
	void rux_class::static_set_##rux_property( const XGCRef* object , const XGCRef* value )\
	{\
		rux_class* obj = (rux_class*)object->get_ObjectAddress();\
		if( obj->_##rux_property.get_GCRef( 1 , 1 ) != value )\
			obj->_##rux_property = const_cast< XGCRef* >( value );\
	}
#define implement_rux_property_with_property_class( rux_class , rux_property_class , rux_property ) XGCRef& rux_class::static_get_##rux_property( const XGCRef* object )\
			{\
				rux_class* obj = (rux_class*)object->get_ObjectAddress();\
				return rux_property_class( obj->get_##rux_property() )++;\
			};\
			void rux_class::static_set_##rux_property( const XGCRef* object , const XGCRef* value )\
			{\
				rux_class* obj = (rux_class*)object->get_ObjectAddress();\
				rux_property_class val;\
				val.set_ByRef( value );\
				obj->set_##rux_property( val() );\
			}
#define begin_register_rux_property( rux_class ) void rux_class::register_methods_and_properties( Type* type )\
	{
#define register_rux_property( rux_property_class , rux_property ) {\
		rux_property_class param;\
		type->RegisterProperty( #rux_property , &param.get_Type() , (void*)static_set_##rux_property , (void*)static_get_##rux_property );\
	}
#define register_any_class_rux_property( rux_property ) {\
		type->RegisterProperty( #rux_property , NULL , (void*)static_set_##rux_property , (void*)static_get_##rux_property );\
	}
#define end_register_rux_property() }
#define rux_simple_console_initialize_args( module_name , start ) dll_internal const char* g_current_module_name = module_name;\
	dll_internal ::rux::uint8 _rux_is_service = 0;\
	dll_internal ::rux::uint8 _rux_is_debug = 0;\
	dll_internal ::rux::uint8 _rux_is_memory_manager = 0;\
	namespace rux\
	{\
		dll_internal rux_simple_t _first_function = NULL;\
		dll_internal rux_simple_t _last_function = NULL;\
		dll_internal rux_simple_t _pre_initialization = NULL;\
		dll_internal rux_simple_with_result_t _post_deinitialization = NULL;\
	};\
	int main( int argc , char ** args )\
	{\
		::rux::int32 result = 0;\
		{\
			::rux::engine::initialize();\
			THREAD_REGISTER( "main" );\
			::rux::int32 index0 = 0;\
			::rux::XArray< ::rux::XString > arguments;\
			for( index0 = 1 ; index0 < argc ; index0++ )\
					arguments.Add( ::rux::XString( args[ index0 ] ) );\
			result = start( arguments );\
		}\
		if( ::rux::engine::_globals && ::rux::engine::_globals->_rux_stop_threads )\
			::rux::engine::_globals->_rux_stop_threads();\
		return result;\
	}
#define rux_simple_console_initialize( module_name , start ) dll_internal const char* g_current_module_name = module_name;\
	dll_internal ::rux::uint8 _rux_is_service = 0;\
	dll_internal ::rux::uint8 _rux_is_debug = 0;\
	dll_internal ::rux::uint8 _rux_is_memory_manager = 0;\
	namespace rux\
	{\
		dll_internal rux_simple_t _first_function = NULL;\
		dll_internal rux_simple_t _last_function = NULL;\
		dll_internal rux_simple_t _pre_initialization = NULL;\
		dll_internal rux_simple_with_result_t _post_deinitialization = NULL;\
	};\
	int main( int argc , char ** args )\
	{\
		::rux::int32 result = 0;\
		{\
			::rux::engine::initialize();\
			THREAD_REGISTER( "main" );\
			::rux::int32 index0 = 0;\
			::rux::XArray< ::rux::XString > argument_names , argument_values;\
			for( index0 = 1 ; index0 < argc ; index0++ )\
			{\
				if( index0 + 1 < argc )\
				{\
					argument_names.Add( ::rux::XString( args[ index0 ] ) );\
					index0++;\
					argument_values.Add( ::rux::XString( args[ index0 ] ) );\
				}\
				else\
				{\
					argument_names.Add( ::rux::XString( args[ index0 ] ) );\
					argument_values.Add( ::rux::XString() );\
				}\
			}\
			result = start( argument_names , argument_values );\
		}\
		if( ::rux::engine::_globals && ::rux::engine::_globals->_rux_stop_threads )\
			::rux::engine::_globals->_rux_stop_threads();\
		return result;\
	}
#define rux_application_initialize( module_name , start , stop , check_rux_executing_in_current_path , before_start , after_stop ) \
	dll_internal const char* g_current_module_name = module_name;\
	dll_internal ::rux::uint8 _rux_is_service = 0;\
	dll_internal ::rux::uint8 _rux_is_debug = 0;\
	dll_internal ::rux::uint8 _rux_is_memory_manager = 0;\
	namespace rux\
	{\
		dll_internal rux_simple_t _first_function = before_start;\
		dll_internal rux_simple_t _last_function = after_stop;\
		dll_internal rux_simple_t _pre_initialization = NULL;\
		dll_internal rux_simple_with_result_t _post_deinitialization = NULL;\
	};\
	int main( int argc , char ** args )\
	{\
		::booldog::allocators::easy::heap easyheap;\
		::booldog::allocators::single_threaded::mixed<2048> easymixed(&easyheap);\
		::rux::int32 main_res = 0;\
		{\
			::booldog::result_mbchar mbchar0(&easymixed), mbchar1(&easymixed);\
			::rux::int32 index0 = 0;\
			const char* command = NULL;\
			size_t argument_count = 0;\
			const char* argument_names[ 64 ] = {0} , * argument_values[ 64 ] = {0};\
			for( index0 = 1 ; index0 < argc ; index0++ )\
			{\
				if( index0 == 1 )\
					command = args[ index0 ];\
				else if( index0 + 1 < argc )\
				{\
					argument_names[ argument_count ] = args[ index0 ];\
					index0++;\
					argument_values[ argument_count ] = args[ index0 ];\
					argument_count++;\
				}\
			}\
			const char* debug = 0;\
			rux_native_get_argument(debug, argument_names, argument_values, argument_count);\
			if(debug && strcmp(debug, "true") == 0 )\
			{\
				::rux::threading::XThread::Sleep(10000);\
				_rux_is_debug = 1;\
			}\
			const char* name = 0;\
			rux_native_get_argument( name , argument_names , argument_values , argument_count );\
			::rux::service::private_report_info_event( name , "started" );\
			if( command && strcmp( command , "reg" ) == 0 )\
			{\
				::rux::engine::initialize();\
				const char* display_name = 0, * user = 0, * group = 0, * interactive = 0, * autorecovery = 0, * pidfile = 0, * coredump = 0;\
				rux_native_get_argument( user , argument_names , argument_values , argument_count );\
				rux_native_get_argument( group , argument_names , argument_values , argument_count );\
				rux_native_get_argument( display_name , argument_names , argument_values , argument_count );\
				rux_native_get_argument( interactive , argument_names , argument_values , argument_count );\
				rux_native_get_argument( autorecovery , argument_names , argument_values , argument_count );\
				rux_native_get_argument( pidfile , argument_names , argument_values , argument_count );\
				rux_native_get_argument(coredump, argument_names, argument_values, argument_count);\
				if( user == NULL || strlen( user ) == 0 )\
					user = "root";\
				if( group == NULL || strlen( group ) == 0 )\
					group = "root";\
				if( name && strlen( name ) > 0 )\
				{\
					::rux::service::initialize( start , stop );\
					if( interactive && strcmp( interactive , "true" ) == 0 )\
						::rux::service::set_Interactive( 1 );\
					if( autorecovery && strcmp( autorecovery , "true" ) == 0 )\
						::rux::service::set_Autorecovery( 1 );\
					if( pidfile && strcmp( pidfile , "true" ) == 0 )\
						::rux::service::set_WithPidFile( 1 );\
					else\
						::rux::service::set_WithPidFile( 0 );\
					if(coredump && strcmp(coredump, "true") == 0)\
						::rux::service::set_CoreDump(1);\
					else\
						::rux::service::set_CoreDump(0);\
					::rux::service::set_User( user );\
					::rux::service::set_UserGroup( group );\
					::rux::service::set_ServiceName( name );\
					if( display_name && strlen( display_name ) > 0 )\
						::rux::service::set_ServiceDisplayName( display_name );\
					else\
						::rux::service::set_ServiceDisplayName( name );\
					declare_stack_variable( char , error , 1024 );\
					::rux::service::Install( error );\
					if( error[ 0 ] == 0 )\
						::rux::service::StartByName( name , error );\
					if( error[ 0 ] != 0 )\
					{\
						main_res = 1;\
						::booldog::utils::console::err::mbs::printf(0, &easymixed, ::booldog::enums::console::red, debuginfo_macros,"%s" , error );\
					}\
				}\
				else\
				{\
					main_res = 1;\
					::booldog::utils::console::err::mbs::printf(0, &easymixed, ::booldog::enums::console::red, debuginfo_macros,"%s", "Command 'reg' is failed.Argument 'name' is empty\n");\
				}\
			}\
			else if( command && strcmp( command , "unreg" ) == 0 )\
			{\
				::rux::engine::initialize();\
				if( name && strlen( name ) > 0 )\
				{\
					::rux::service::set_ServiceName( name );\
					declare_stack_variable( char , error , 1024 );\
					::rux::service::DeInstall( error );\
					if( error[ 0 ] != 0 )\
					{\
						::rux::XConsole::Print( stderr , XRed , "%s" , error );\
						main_res = 1;\
					}\
				}\
				else\
				{\
					main_res = 1;\
					::booldog::utils::console::err::mbs::printf(0, &easymixed, ::booldog::enums::console::red, debuginfo_macros,"%s" , "Command 'unreg' is failed.Argument 'name' is empty\n" );\
				}\
			}\
			else if( command && strcmp( command , "-d" ) == 0 )\
			{\
				::rux::XConsole::DeInitialize();\
				::rux::service::initialize(&mbchar0, &mbchar1);\
				::rux::engine::load_engine();\
				if( check_rux_executing_in_current_path == 0\
					|| rux_is_already_executing_in_current_path() == 0 )\
				{\
					const char* display_name = 0, * logfile = 0, * pidfile = 0, * autorecovery = 0, * coredump = 0;\
					rux_native_get_argument( name , argument_names , argument_values , argument_count );\
					rux_native_get_argument( display_name , argument_names , argument_values , argument_count );\
					rux_native_get_argument( logfile , argument_names , argument_values , argument_count );\
					rux_native_get_argument( pidfile , argument_names , argument_values , argument_count );\
					rux_native_get_argument( autorecovery , argument_names , argument_values , argument_count );\
					rux_native_get_argument(coredump, argument_names, argument_values, argument_count);\
					if( name && name[ 0 ] != 0 )\
					{\
						::rux::service::initialize( start , stop );\
						::rux::service::set_ServiceName( name );\
						if( pidfile && strcmp( pidfile , "true" ) == 0 )\
							::rux::service::set_WithPidFile( 1 );\
						else\
							::rux::service::set_WithPidFile( 0 );\
						if(coredump && strcmp(coredump, "true") == 0)\
							::rux::service::set_CoreDump(1);\
						else\
							::rux::service::set_CoreDump(0);\
						if( autorecovery && strcmp( autorecovery , "true" ) == 0 )\
							::rux::service::set_Autorecovery( 1 );\
						if( display_name && display_name[ 0 ] != 0 )\
							::rux::service::set_ServiceDisplayName( display_name );\
						else\
							::rux::service::set_ServiceDisplayName( name );\
						declare_stack_variable( char , error , 1024 );\
						_rux_is_service = 1;\
						if( ::rux::service::Start( logfile , error , check_rux_executing_in_current_path ) == 0 )\
						{\
							::rux::service::private_report_error_event( name , "Service could not start" );\
							main_res = 1;\
							::booldog::utils::console::err::mbs::printf(0, &easymixed, ::booldog::enums::console::red, debuginfo_macros,"%s\n", "Service could not start");\
						}\
					}\
					else\
					{\
						::rux::service::private_report_error_event( name , "name is empty" );\
						main_res = 1;\
						::booldog::utils::console::err::mbs::printf(0, &easymixed, ::booldog::enums::console::red, debuginfo_macros,"%s" , "Service starting is failed.Argument 'name' is empty\n" );\
					}\
				}\
				else\
				{\
					::rux::service::private_report_error_event( name , "already executing in current path" );\
					main_res = 1;\
					::booldog::utils::console::err::mbs::printf(0, &easymixed, ::booldog::enums::console::red, debuginfo_macros,"%s" , module_name" is already executing in current path\n" );\
				}\
			}\
			else\
			{\
				::rux::engine::initialize();\
				if( check_rux_executing_in_current_path == 0\
					|| rux_is_already_executing_in_current_path() == 0 )\
				{\
					THREAD_REGISTER( "main" );\
					start( argc , args );\
				}\
				else\
				{\
					main_res = 1;\
					::booldog::utils::console::err::mbs::printf(0, &easymixed, ::booldog::enums::console::red, debuginfo_macros,"%s" , module_name" is already executing in current path\n" );\
				}\
			}\
		}\
		if( ::rux::engine::_globals && ::rux::engine::_globals->_rux_stop_threads )\
			::rux::engine::_globals->_rux_stop_threads();\
		return main_res;\
	}
#ifdef __WINDOWS__
#define rux_module_preinitialize() extern "C"\
	BOOL WINAPI DllMain( HINSTANCE instance , DWORD reason , LPVOID )\
	{\
		if( reason == DLL_PROCESS_ATTACH )\
			DisableThreadLibraryCalls( instance );\
		return TRUE;\
	}
#else
#define rux_module_preinitialize()
#endif
#define _rux_module_initialize_( module_name , start , stop , pre_initialization , post_deinitialization ) dll_internal ::rux::uint8 _rux_is_service = 0;\
	dll_internal ::rux::uint8 _rux_is_debug = 0;\
	dll_internal const char* g_current_module_name = dynamic_library_prefix module_name dynamic_library_extension;\
	dll_internal ::rux::uint8 _rux_is_memory_manager = 0;\
	namespace rux\
	{\
		dll_internal rux_simple_t _first_function = NULL;\
		dll_internal rux_simple_t _last_function = NULL;\
		dll_internal rux_simple_t _pre_initialization = pre_initialization;\
		dll_internal rux_simple_with_result_t _post_deinitialization = post_deinitialization;\
		dll_internal rux_volatile _current_module_init = 0;\
		dll_internal rux_volatile _current_module_reference = 0;\
		dll_internal ::rux::int32 module_initialize( void* param )\
		{\
			rux_interlocked_return res = 0;\
			XInterlocked::Increment( &::rux::_current_module_reference );\
			if( ( res = XInterlocked::CompareExchange( &::rux::_current_module_init , 2 , 0 ) ) != 1 )\
			{\
				_CS_PTR_LOCK_( ::rux::_cs_current_module , 0 );\
				if( res == 0 && XInterlocked::CompareExchange( &::rux::_current_module_init , 0 , 0 ) == 2 )\
				{\
					::rux::engine::initialize();\
					start( param );\
					XInterlocked::CompareExchange( &::rux::_current_module_init , 1 , 2 );\
				}\
				::rux::_cs_current_module->UnLock( 0 );\
			}\
			return 0;\
		};\
		dll_internal ::rux::int32 module_deinitialize( void )\
		{\
			return ::rux::engine::this_module_deinitialize( &::rux::_current_module_init , &::rux::_current_module_reference , stop );\
		}\
	};\
	rux_module_preinitialize();\
	exports ::rux::int32 dll_init( void* param )\
	{\
		return ::rux::module_initialize( param );\
	};\
	exports ::rux::int32 rux_module_init( void* param )\
	{\
		return ::rux::module_initialize( param );\
	};\
	exports ::rux::int32 dll_free( void )\
	{\
		return ::rux::module_deinitialize();\
	};\
	exports ::rux::int32 rux_module_free( void )\
	{\
		return ::rux::module_deinitialize();\
	};\
	exports void is_rux_module( void )\
	{\
	};\
	dll_internal ::rux::uint8 rux_lock_module( ::rux::uint8 )\
	{\
		while( XInterlocked::CompareExchange( &::rux::_current_module_init , 0 , 0 ) == 2 )\
		{\
		}\
		if( XInterlocked::CompareExchange( &::rux::_current_module_init , 0 , 0 ) == 0 )\
			return 0;\
		else\
		{\
			XInterlocked::Increment( &::rux::_current_module_is_locked );\
			return 1;\
		}\
	};\
	dll_internal void rux_unlock_module( void )\
	{\
		XInterlocked::Decrement( &::rux::_current_module_is_locked );\
	};
#define rux_module_initialize( module_name , start , stop ) _rux_module_initialize_( module_name , start , stop , NULL , NULL )
#define rux_system_module_initialize( module_name , start , stop , pre_initialization , post_deinitialization ) _rux_module_initialize_( module_name , start , stop , pre_initialization , post_deinitialization )
class XCodeLabel
{
	size_t ___rux__thread_index1986;
public:
	XCodeLabel( size_t rux__thread_index1986 , const char* filename , const char* function , ::rux::int32 line , const char* other_module , const char* other_function , ::rux::uint32 max_delay , ::rux::byte debug = 1 );
	~XCodeLabel();
};
#define declare_variable( object_class , var ) object_class var( #var , __FILE__ , __LINE__ )
#define declare_variable_param( object_class , var , param0 ) object_class var( param0 , #var , __FILE__ , __LINE__ )
#define declare_variable_2_params( object_class , var , param0 , param1 ) object_class var( param0 , param1 , #var , __FILE__ , __LINE__ )
#define declare_class_member( var ) var( #var , __FILE__ , __LINE__ )
#define CRITICAL_SECTION_LOCK( crt ) _CRITICAL_SECTION_LOCK_( crt , 1 )
#define _CRITICAL_SECTION_LOCK_( crt , code_label ) crt.Lock( code_label , __FILE__" "#crt , __FUNCTION__ , __LINE__ )
#define CS_PTR_LOCK( crt ) _CS_PTR_LOCK_( crt , 1 )
#define _CS_PTR_LOCK_( crt , code_label ) crt->Lock( 1 , __FILE__" "#crt , __FUNCTION__ , __LINE__ )
#define CS_PTR_UNLOCK( crt ) crt->UnLock( 1 , __FILE__" "#crt , __FUNCTION__ , __LINE__ )
#define EVENT_WAIT( evt , timeout ) evt.Wait( timeout , __FILE__" "#evt , __FUNCTION__ , __LINE__ )
#define CRITICAL_SECTION_UNLOCK( crt ) crt.UnLock( 1 , __FILE__" "#crt , __FUNCTION__ , __LINE__ )
#define EXCLUSIVE_LOCK( crt ) crt.WriteLock( __FILE__" "#crt , __FUNCTION__ , __LINE__ )
#define SHARED_LOCK( crt ) crt.ReadLock( __FILE__" "#crt , __FUNCTION__ , __LINE__ )
#define WRITE_LOCK( crt ) crt.WriteLock( __FILE__" "#crt , __FUNCTION__ , __LINE__ )
#define READ_LOCK( crt ) crt.ReadLock( __FILE__" "#crt , __FUNCTION__ , __LINE__ )
#define CODE_LABELS_INITIALIZE() size_t ___rux__thread_index1986 = SIZE_MAX;\
		{\
			if( ::rux::engine::_globals && ::rux::engine::_globals->_get_thread_index )\
				___rux__thread_index1986 = ::rux::engine::_globals->_get_thread_index( _rux_current_module_index );\
		}
#define CODE_CPU_INIT() {\
			if( ::rux::engine::_globals && ::rux::engine::_globals->_set_thread_force_cpu )\
				::rux::engine::_globals->_set_thread_force_cpu( ___rux__thread_index1986 , 1 );\
		}
#define CODE_CPU_DEINIT() {\
			if( ::rux::engine::_globals && ::rux::engine::_globals->_set_thread_force_cpu )\
				::rux::engine::_globals->_set_thread_force_cpu( ___rux__thread_index1986 , 0 );\
		}
#define CODE_LABEL( other_module , other_function , max_delay ) XCodeLabel ____rux__label1986( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , other_module , other_function , max_delay )
#define CODE_LABEL_START( other_module , other_function , max_delay ) ::rux::byte ___rux__thread_index1986_diagnostic = 0;\
		if( ___rux__thread_index1986 != SIZE_MAX && ::rux::engine::_globals && ::rux::engine::_globals->_rux_memory_add_label )\
			___rux__thread_index1986_diagnostic = ::rux::engine::_globals->_diagnostic;\
		if( ___rux__thread_index1986_diagnostic )\
			::rux::engine::_globals->_rux_memory_add_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , other_module , other_function , _rux_current_module_index , 1 , max_delay );\
		else {}
#define CODE_LABEL_END() if( ___rux__thread_index1986_diagnostic )\
		{\
			::rux::engine::_globals->_rux_memory_remove_label( ___rux__thread_index1986 );\
			___rux__thread_index1986_diagnostic = 0;\
		}
#define CODE_LABEL_ANOTHER_CONTEXT( other_module , other_function , function ) {\
		::rux::byte diagnostic = 0;\
		if( ::rux::engine::_globals && ::rux::engine::_globals->_rux_memory_add_label )\
			diagnostic = ::rux::engine::_globals->_diagnostic;\
		if( diagnostic )\
			::rux::engine::_globals->_rux_memory_add_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , other_module , other_function , _rux_current_module_index , 1 , 10 );\
		function();\
		if( diagnostic )\
			::rux::engine::_globals->_rux_memory_remove_label( ___rux__thread_index1986 );\
	}
#define CODE_LABEL_ANOTHER_CONTEXT_PARAMETER( other_module , other_function , function , parameter ) {\
		::rux::byte diagnostic = 0;\
		if( ::rux::engine::_globals && ::rux::engine::_globals->_rux_memory_add_label )\
			diagnostic = ::rux::engine::_globals->_diagnostic;\
		if( diagnostic )\
			::rux::engine::_globals->_rux_memory_add_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , other_module , other_function , _rux_current_module_index , 1 , 10 );\
		function( parameter );\
		if( diagnostic )\
			::rux::engine::_globals->_rux_memory_remove_label( ___rux__thread_index1986 );\
	}
#define CODE_LABEL_ANOTHER_CONTEXT_2_PARAMETERS( other_module , other_function , function , parameter0 , parameter1 ) {\
		::rux::byte diagnostic = 0;\
		if( ::rux::engine::_globals && ::rux::engine::_globals->_rux_memory_add_label )\
			diagnostic = ::rux::engine::_globals->_diagnostic;\
		if( diagnostic )\
			::rux::engine::_globals->_rux_memory_add_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , other_module , other_function , _rux_current_module_index , 1 , 10 );\
		function( parameter0 , parameter1 );\
		if( diagnostic )\
			::rux::engine::_globals->_rux_memory_remove_label( ___rux__thread_index1986 );\
	}
#define CODE_LABEL_ANOTHER_CONTEXT_3_PARAMETERS( other_module , other_function , function , parameter0 , parameter1 , parameter2 ) {\
		::rux::byte diagnostic = 0;\
		if( ::rux::engine::_globals && ::rux::engine::_globals->_rux_memory_add_label )\
			diagnostic = ::rux::engine::_globals->_diagnostic;\
		if( diagnostic )\
			::rux::engine::_globals->_rux_memory_add_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , other_module , other_function , _rux_current_module_index , 1 , 10 );\
		function( parameter0 , parameter1 , parameter2 );\
		if( diagnostic )\
			::rux::engine::_globals->_rux_memory_remove_label( ___rux__thread_index1986 );\
	}
#define CODE_LABEL_ANOTHER_CONTEXT_4_PARAMETERS( other_module , other_function , function , parameter0 , parameter1 , parameter2 , parameter3 ) {\
		::rux::byte diagnostic = 0;\
		if( ::rux::engine::_globals && ::rux::engine::_globals->_rux_memory_add_label )\
			diagnostic = ::rux::engine::_globals->_diagnostic;\
		if( diagnostic )\
			::rux::engine::_globals->_rux_memory_add_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , other_module , other_function , _rux_current_module_index , 1 , 10 );\
		function( parameter0 , parameter1 , parameter2 , parameter3 );\
		if( diagnostic )\
			::rux::engine::_globals->_rux_memory_remove_label( ___rux__thread_index1986 );\
	}
#define CODE_LABEL_ANOTHER_CONTEXT_RESULT( other_module , other_function , function , result ) {\
		::rux::byte diagnostic = 0;\
		if( ::rux::engine::_globals && ::rux::engine::_globals->_rux_memory_add_label )\
			diagnostic = ::rux::engine::_globals->_diagnostic;\
		if( diagnostic )\
			::rux::engine::_globals->_rux_memory_add_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , other_module , other_function , _rux_current_module_index , 1 , 10 );\
		result = function();\
		if( diagnostic )\
			::rux::engine::_globals->_rux_memory_remove_label( ___rux__thread_index1986 );\
	}
#define CODE_LABEL_ANOTHER_CONTEXT_PARAMETER_RESULT( other_module , other_function , function , parameter , result ) {\
		::rux::byte diagnostic = 0;\
		if( ::rux::engine::_globals && ::rux::engine::_globals->_rux_memory_add_label )\
			diagnostic = ::rux::engine::_globals->_diagnostic;\
		if( diagnostic )\
			::rux::engine::_globals->_rux_memory_add_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , other_module , other_function , _rux_current_module_index , 1 , 10 );\
		result = function( parameter );\
		if( diagnostic )\
			::rux::engine::_globals->_rux_memory_remove_label( ___rux__thread_index1986 );\
	}
#define CODE_LABEL_ANOTHER_CONTEXT_2_PARAMETERS_RESULT( other_module , other_function , function , parameter0 , parameter1 , result ) {\
		::rux::byte diagnostic = 0;\
		if( ::rux::engine::_globals && ::rux::engine::_globals->_rux_memory_add_label )\
			diagnostic = ::rux::engine::_globals->_diagnostic;\
		if( diagnostic )\
			::rux::engine::_globals->_rux_memory_add_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , other_module , other_function , _rux_current_module_index , 1 , 10 );\
		result = function( parameter0 , parameter1 );\
		if( diagnostic )\
			::rux::engine::_globals->_rux_memory_remove_label( ___rux__thread_index1986 );\
	}
#define CODE_LABEL_ANOTHER_CONTEXT_3_PARAMETERS_RESULT( other_module , other_function , function , parameter0 , parameter1 , parameter2 , result ) {\
		::rux::byte diagnostic = 0;\
		if( ::rux::engine::_globals && ::rux::engine::_globals->_rux_memory_add_label )\
			diagnostic = ::rux::engine::_globals->_diagnostic;\
		if( diagnostic )\
			::rux::engine::_globals->_rux_memory_add_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , other_module , other_function , _rux_current_module_index , 1 , 10 );\
		result = function( parameter0 , parameter1 , parameter2 );\
		if( diagnostic )\
			::rux::engine::_globals->_rux_memory_remove_label( ___rux__thread_index1986 );\
	}
#define CODE_LABEL_ANOTHER_CONTEXT_4_PARAMETERS_RESULT( other_module , other_function , function , parameter0 , parameter1 , parameter2 , parameter3 , result ) {\
		::rux::byte diagnostic = 0;\
		if( ::rux::engine::_globals && ::rux::engine::_globals->_rux_memory_add_label )\
			diagnostic = ::rux::engine::_globals->_diagnostic;\
		if( diagnostic )\
			::rux::engine::_globals->_rux_memory_add_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , other_module , other_function , _rux_current_module_index , 1 , 10 );\
		result = function( parameter0 , parameter1 , parameter2 , parameter3 );\
		if( diagnostic )\
			::rux::engine::_globals->_rux_memory_remove_label( ___rux__thread_index1986 );\
	}
#define CODE_LABEL_ANOTHER_CONTEXT_5_PARAMETERS_RESULT( other_module , other_function , function , parameter0 , parameter1 , parameter2 , parameter3 , parameter4 , result ) {\
		::rux::byte diagnostic = 0;\
		if( ::rux::engine::_globals && ::rux::engine::_globals->_rux_memory_add_label )\
			diagnostic = ::rux::engine::_globals->_diagnostic;\
		if( diagnostic )\
			::rux::engine::_globals->_rux_memory_add_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , other_module , other_function , _rux_current_module_index , 1 , 10 );\
		result = function( parameter0 , parameter1 , parameter2 , parameter3 , parameter4 );\
		if( diagnostic )\
			::rux::engine::_globals->_rux_memory_remove_label( ___rux__thread_index1986 );\
	}
namespace rux
{
	namespace threading
	{
		class XThreadRegister
		{
		public:
			size_t _thread_index;
			XThreadRegister( const char* thread_name );
			~XThreadRegister();	
		};
	};
};
#define THREAD_REGISTER( thread_name ) ::rux::threading::XThreadRegister ____rux__thread1986( thread_name );\
	size_t ___rux__thread_index1986 = ____rux__thread1986._thread_index
dll_internal void rux_free_loader_engine( void );
dll_internal void rux_free_gc_engine( void );
dll_internal void rux_free_type_engine( void );
namespace rux
{
	namespace engine
	{
		dll_internal void deinitialize( void );
		dll_internal ::rux::int32 this_module_deinitialize( rux_volatile* current_module_init , rux_volatile* current_module_reference , rux_simple_t stop );
	};
	dll_internal void deinitialize( void );	
#ifdef __WINDOWS__
	dll_internal ::rux::pid_t get_thread_id( HANDLE ThreadHandle );
#endif
};
#define check_rux_type_index( rux_class ) if( XInterlocked::CompareExchange( &rux_class::_rux_type_index , 0 , 0 ) == rux_volatile_max )\
	{\
		_typeof( rux_class::get_Alias() , &rux_class::_rux_type_index , rux_class::register_type , rux_class::_rux_is_array , rux_class::get_rux_array_item_type() );\
	}
#define rux_static_get_type( rux_class ) _typeof( rux_class::get_Alias() , &rux_class::_rux_type_index , rux_class::register_type , rux_class::_rux_is_array , rux_class::get_rux_array_item_type() )
#define implement_get_static_Type_ns( rux_namespace , rux_class ) ::rux::uint32 rux_namespace::rux_class::get_static_RuxTypeIndex( void )\
	{\
		check_rux_type_index( rux_namespace::rux_class );\
		return XInterlocked::CompareExchange( &rux_namespace::rux_class::_rux_type_index , 0 , 0 );\
	};\
	::rux::uint32 rux_namespace::X##rux_class::get_static_RuxTypeIndex( void )\
	{\
		check_rux_type_index( rux_namespace::rux_class );\
		return XInterlocked::CompareExchange( &rux_namespace::rux_class::_rux_type_index , 0 , 0 );\
	};\
	Type* rux_namespace::rux_class::get_static_Type( rux::uint32* rux_type_index )\
	{\
		if( rux_type_index )\
		{\
			*rux_type_index = rux_namespace::rux_class::get_static_RuxTypeIndex();\
			return 0;\
		}\
		return rux_static_get_type( rux_namespace::rux_class );\
	};\
	Type* rux_namespace::X##rux_class::get_static_Type( rux::uint32* rux_type_index )\
	{\
		if( rux_type_index )\
		{\
			*rux_type_index = rux_namespace::rux_class::get_static_RuxTypeIndex();\
			return 0;\
		}\
		return rux_static_get_type( rux_namespace::rux_class );\
	}
#define implement_get_static_Type( rux_class ) ::rux::uint32 rux_class::get_static_RuxTypeIndex( void )\
	{\
		check_rux_type_index( rux_class );\
		return XInterlocked::CompareExchange( &rux_class::_rux_type_index , 0 , 0 );\
	};\
	::rux::uint32 X##rux_class::get_static_RuxTypeIndex( void )\
	{\
		check_rux_type_index( rux_class );\
		return XInterlocked::CompareExchange( &rux_class::_rux_type_index , 0 , 0 );\
	};\
	Type* rux_class::get_static_Type( rux::uint32* rux_type_index )\
	{\
		if( rux_type_index )\
		{\
			*rux_type_index = rux_class::get_static_RuxTypeIndex();\
			return 0;\
		}\
		return rux_static_get_type( rux_class );\
	};\
	Type* X##rux_class::get_static_Type( rux::uint32* rux_type_index )\
	{\
		if( rux_type_index )\
		{\
			*rux_type_index = rux_class::get_static_RuxTypeIndex();\
			return 0;\
		}\
		return rux_static_get_type( rux_class );\
	}
#define implement_get_static_RuxTypeIndex_for_template( rux_class ) template<> rux::uint32 rux_class::get_static_RuxTypeIndex( void )\
	{\
		check_rux_type_index( rux_class );\
		return XInterlocked::CompareExchange( &rux_class::_rux_type_index , 0 , 0 );\
	};\
	template<> rux::uint32 X##rux_class::get_static_RuxTypeIndex( void )\
	{\
		check_rux_type_index( rux_class );\
		return XInterlocked::CompareExchange( &rux_class::_rux_type_index , 0 , 0 );\
	}
#define implement_get_static_Type_for_template( rux_class ) template<> Type* rux_class::get_static_Type( rux::uint32* rux_type_index )\
	{\
		if( rux_type_index )\
		{\
			*rux_type_index = rux_class::get_static_RuxTypeIndex();\
			return 0;\
		}\
		return rux_static_get_type( rux_class );\
	};\
	template<> Type* X##rux_class::get_static_Type( rux::uint32* rux_type_index )\
	{\
		if( rux_type_index )\
		{\
			*rux_type_index = rux_class::get_static_RuxTypeIndex();\
			return 0;\
		}\
		return rux_static_get_type( rux_class );\
	}
#define cpp_ptr_cast( xclass , object ) static_cast< xclass >( static_cast< void* >( object ) )
#define cpp_offsetof( xclass , xmember ) ( (size_t)( &reinterpret_cast< xclass* >( 100000 )->xmember ) - 100000 )
namespace rux
{
	namespace compile
	{
		template< const char n0 = 0 , const char n1 = 0 , const char n2 = 0 , const char n3 = 0 , const char n4 = 0 , const char n5 = 0 , const char n6 = 0 , const char n7 = 0 , const char n8 = 0 , const char n9 = 0 , const char n10 = 0 , const char n11 = 0 , const char n12 = 0 , const char n13 = 0 , const char n14 = 0 , const char n15 = 0 , const char n16 = 0 , const char n17 = 0 , const char n18 = 0 , const char n19 = 0 >
		struct times33_hash
		{
			template< const char k , ::rux::uint32 hash = 0 >
			struct hash_calc
			{
				enum { value = ( k == 0 ) ? hash : hash * 33 + k };
			};
			enum
			{
				value = hash_calc< n19 , hash_calc< n18 , hash_calc< n17 , hash_calc< n16 , hash_calc< n15 , hash_calc< n14 , hash_calc< n13 , hash_calc< n12 , hash_calc< n11 , hash_calc< n10 , hash_calc< n9 , hash_calc< n8 , hash_calc< n7 , hash_calc< n6 , hash_calc< n5 , hash_calc< n4 , hash_calc< n3 , hash_calc< n2 , hash_calc< n1 , hash_calc< n0 >::value >::value >::value >::value >::value >::value >::value >::value >::value >::value >::value >::value >::value >::value >::value >::value >::value >::value >::value >::value
			};
		};
	};
};
#endif
