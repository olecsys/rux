#ifndef XAN_PDH_H
#define XAN_PDH_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_string.h>
#ifdef __WINDOWS__
#include <pdh.h>
typedef PDH_FUNCTION __stdcall pdh_get_formatted_counter_value_function( PDH_HCOUNTER hCounter, DWORD dwFormat, LPDWORD lpdwType, PPDH_FMT_COUNTERVALUE pValue );
typedef PDH_FUNCTION __stdcall pdh_collect_query_data_function( PDH_HQUERY hQuery );
typedef PDH_FUNCTION __stdcall pdh_add_counter_function( PDH_HQUERY hQuery, LPCWSTR szFullCounterPath, DWORD_PTR dwUserData, PDH_HCOUNTER * phCounter );
typedef PDH_FUNCTION __stdcall pdh_open_query_function( LPCWSTR szDataSource, DWORD_PTR dwUserData, PDH_HQUERY * phQuery );
typedef PDH_FUNCTION __stdcall pdh_make_counter_path_function( PPDH_COUNTER_PATH_ELEMENTS_W pCounterPathElements, LPWSTR szFullPathBuffer, LPDWORD pcchBufferSize, DWORD dwFlags );
typedef PDH_FUNCTION __stdcall pdh_lookup_perf_name_by_index_function( LPCWSTR szMachineName, DWORD dwNameIndex, LPWSTR szNameBuffer, LPDWORD pcchNameBufferSize );
typedef PDH_FUNCTION __stdcall pdh_remove_counter_function( PDH_HCOUNTER hCounter );
typedef PDH_FUNCTION __stdcall pdh_close_query_function( PDH_HQUERY hQuery );
namespace rux
{
	namespace pdh
	{
		dll_internal void initialize( void );
		dll_internal void deinitialize( void );
		dll_internal void counter( const char* instance_name , ::rux::uint32 object_index , ::rux::uint32 counter_index , ::rux::uint32 counter_value_format , void* counter_value_ptr , ::rux::XString* error );
	};
};
#endif
#endif