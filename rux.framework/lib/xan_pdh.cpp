#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_pdh.h"
#include "xan_error.h"
namespace rux
{
#ifdef __WINDOWS__
	namespace pdh
	{
		dll_internal pdh_lookup_perf_name_by_index_function* _pdh_lookup_perf_name_by_index = NULL;
		dll_internal pdh_make_counter_path_function* _pdh_make_counter_path = NULL;
		dll_internal pdh_open_query_function* _pdh_open_query = NULL;
		dll_internal pdh_add_counter_function* _pdh_add_counter = NULL;
		dll_internal pdh_collect_query_data_function* _pdh_collect_query_data = NULL;
		dll_internal pdh_get_formatted_counter_value_function* _pdh_get_formatted_counter_value = NULL;
		dll_internal pdh_remove_counter_function* _pdh_remove_counter = NULL;
		dll_internal pdh_close_query_function* _pdh_close_query = NULL;
		dll_internal HMODULE _handle = NULL;
		dll_internal rux_volatile _inited = 0;
		dll_internal void initialize( void )
		{
			if( XInterlocked::CompareExchange( &rux::pdh::_inited , 2 , 0 ) != 1 )
			{
				CS_PTR_LOCK( ::rux::pdh::_cs );
				if( ::rux::pdh::_handle == NULL )
				{
					if( XInterlocked::CompareExchange( &rux::pdh::_inited , 0 , 0 ) == 2 )
					{
						::rux::engine::initialize();
						rux_add_deinitialization_func( ::rux::pdh::deinitialize );

						::rux::pdh::_handle = LoadLibraryA( "pdh.dll" );
						if( ::rux::pdh::_handle )
						{
							::rux::pdh::_pdh_lookup_perf_name_by_index = (pdh_lookup_perf_name_by_index_function*)dlsym( ::rux::pdh::_handle , "PdhLookupPerfNameByIndexW" );
							::rux::pdh::_pdh_make_counter_path = (pdh_make_counter_path_function*)dlsym( ::rux::pdh::_handle , "PdhMakeCounterPathW" );
							::rux::pdh::_pdh_open_query = (pdh_open_query_function*)dlsym( ::rux::pdh::_handle , "PdhOpenQueryW" );
							::rux::pdh::_pdh_add_counter = (pdh_add_counter_function*)dlsym( ::rux::pdh::_handle , "PdhAddCounterW" );
							::rux::pdh::_pdh_collect_query_data = (pdh_collect_query_data_function*)dlsym( ::rux::pdh::_handle , "PdhCollectQueryData" );
							::rux::pdh::_pdh_get_formatted_counter_value = (pdh_get_formatted_counter_value_function*)dlsym( ::rux::pdh::_handle , "PdhGetFormattedCounterValue" );
							::rux::pdh::_pdh_remove_counter = (pdh_remove_counter_function*)dlsym( ::rux::pdh::_handle , "PdhRemoveCounter" );
							::rux::pdh::_pdh_close_query = (pdh_close_query_function*)dlsym( ::rux::pdh::_handle , "PdhCloseQuery" );
							
							XInterlocked::CompareExchange( &rux::pdh::_inited , 1 , 2 );						
						}
						else
							XInterlocked::CompareExchange( &rux::pdh::_inited , 0 , 2 );						
					}
				}
				::rux::pdh::_cs->UnLock();
			}
		};
		dll_internal void deinitialize( void )
		{	
			if( XInterlocked::CompareExchange( &rux::pdh::_inited , 2 , 1 ) == 1 )
			{
				_CS_PTR_LOCK_( ::rux::pdh::_cs , 0 );
				if( ::rux::pdh::_handle )
				{
					dlclose( ::rux::pdh::_handle );
					::rux::pdh::_handle = NULL;						
					XInterlocked::CompareExchange( &rux::pdh::_inited , 0 , 2 );
				}
				::rux::pdh::_pdh_lookup_perf_name_by_index = NULL;
				::rux::pdh::_pdh_make_counter_path = NULL;
				::rux::pdh::_pdh_open_query = NULL;
				::rux::pdh::_pdh_add_counter = NULL;
				::rux::pdh::_pdh_collect_query_data = NULL;
				::rux::pdh::_pdh_get_formatted_counter_value = NULL;
				::rux::pdh::_pdh_remove_counter = NULL;
				::rux::pdh::_pdh_close_query = NULL;
				::rux::pdh::_cs->UnLock( 0 );
			}
		};		
		dll_internal void counter( const char* instance_name , ::rux::uint32 object_index , ::rux::uint32 counter_index , ::rux::uint32 counter_value_format , void* counter_value_ptr , ::rux::XString* error )
		{
			if( error )
				error->Clear();
			::rux::pdh::initialize();
			if( XInterlocked::CompareExchange( &rux::pdh::_inited , 0 , 0 ) == 1 )
			{
				PDH_HQUERY h_query = NULL;
				PDH_HCOUNTER h_counter = NULL;
				try
				{
					PDH_STATUS result = 0;
					wchar_t object_name[ 1024 ] = L"";
					unsigned long object_name_length = 1024;			
					if( ( result = ::rux::pdh::_pdh_lookup_perf_name_by_index( NULL , object_index , object_name , &object_name_length ) ) == ERROR_SUCCESS )
					{
						wchar_t counter_name[ 1024 ] = L"";
						unsigned long counter_name_length = 1024;
						if( ( result = ::rux::pdh::_pdh_lookup_perf_name_by_index( NULL , counter_index , counter_name , &counter_name_length ) ) == ERROR_SUCCESS )
						{
							::rux::XString utf16_instance_name;
							utf16_instance_name.set_ByRef( ::rux::XString( instance_name ).ConvertToUTF16() );
							PDH_COUNTER_PATH_ELEMENTS_W pcpe;
							pcpe.szMachineName = NULL;
							pcpe.szObjectName = object_name;
							pcpe.szInstanceName = utf16_instance_name.Length() == 0 ? L"*" : (wchar_t*)utf16_instance_name.str();
							pcpe.szParentInstance = NULL;
							pcpe.dwInstanceIndex = 0;
							pcpe.szCounterName = counter_name;
					
							wchar_t path[ 1024 ] = L"";
							unsigned long pathlength = 1024;
							if( ( result = ::rux::pdh::_pdh_make_counter_path( &pcpe, path, &pathlength, 0) ) == ERROR_SUCCESS )
							{
								if( ( result = ::rux::pdh::_pdh_open_query( NULL, 0, &h_query) ) == ERROR_SUCCESS)
								{
									if( ( result = ::rux::pdh::_pdh_add_counter( h_query , path, 0, &h_counter) ) == ERROR_SUCCESS )
									{
										if( ( result = ::rux::pdh::_pdh_collect_query_data( h_query ) ) == ERROR_SUCCESS )
										{
											Sleep( 100 );
											if( ( result = ::rux::pdh::_pdh_collect_query_data( h_query ) ) == ERROR_SUCCESS)
											{
												PDH_FMT_COUNTERVALUE countervalue;
												unsigned long dwtype = 0;										
												if( ( result = ::rux::pdh::_pdh_get_formatted_counter_value( h_counter, counter_value_format, &dwtype, &countervalue) ) == ERROR_SUCCESS )
												{
													if( counter_value_format == PDH_FMT_DOUBLE )
														(*(double*)counter_value_ptr) = countervalue.doubleValue;
													else if( counter_value_format == PDH_FMT_LONG )
														(*(long*)counter_value_ptr) = countervalue.longValue;
													else if( counter_value_format == PDH_FMT_LARGE )
														(*(__int64*)counter_value_ptr) = countervalue.largeValue;
												}
											}
										}
										else if( error )
											RUX_ERROR( (*error) , XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError ) );
									}
									else if( error )
										RUX_ERROR( (*error) , XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError ) );
								}
							}
						}
					}
				}
				catch( ... )
				{			
				}
				if( h_counter != NULL )
				{
					::rux::pdh::_pdh_remove_counter( h_counter );
					h_counter = NULL;
				}
				if( h_query != NULL )
				{
					::rux::pdh::_pdh_close_query( h_query );
					h_query = NULL;
				}
			}
		};
	};
#endif
};