#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_global_initialization.h>
#include <xan_memorymanager_defines.h>
#include <xan_debug.h>
#include <xan_memory_helper.h>
dll_internal XCrtSect* _cs_engine = NULL;
dll_internal XCrtSect* _cs_rux_deinitialization_funcs = NULL;
namespace rux
{
	namespace debug
	{
		dll_internal XCrtSect* _cs = 0;
	};
	dll_internal XCrtSect* _cs_current_module = NULL;
	dll_internal XCrtSect* _g_cs_rux_console_before_exit_event = NULL;
#ifdef __WINDOWS__
	namespace pdh
	{		
		dll_internal XCrtSect* _cs = NULL;
	};
	void invalid_parameter_handler( const wchar_t* expression , const wchar_t* function , const wchar_t* file , unsigned int line , uintptr_t pReserved )
	{
		UNREFERENCED_PARAMETER( function );
		UNREFERENCED_PARAMETER( file );
		UNREFERENCED_PARAMETER( line );
		UNREFERENCED_PARAMETER( pReserved );
	    wprintf(L"Invalid parameter handler invoked: %s\n", expression);
	};
#endif
};
rux::global_initialization::global_initialization()
{
	rux_global_init();
};
rux::global_initialization::~global_initialization()
{
	rux_global_deinit();
};
#ifdef __UNIX__
__attribute__ ((constructor(101))) void dll_internal rux_global_init( void )
#else
void dll_internal rux_global_init( void )
#endif
{
	if( ::rux::_first_function )
		::rux::_first_function();
	::rux::_first_function = NULL;
	if( _cs_engine == NULL )
		_cs_engine = ::rux::new_object< XCrtSect >();
	if( _cs_rux_deinitialization_funcs == NULL )
		_cs_rux_deinitialization_funcs = ::rux::new_object< XCrtSect >();
	if( ::rux::debug::_cs == NULL )
		::rux::debug::_cs = ::rux::new_object< XCrtSect >();
	if( ::rux::_cs_current_module == NULL )
		::rux::_cs_current_module = ::rux::new_object< XCrtSect >();
	if( ::rux::_g_cs_rux_console_before_exit_event == NULL )
		::rux::_g_cs_rux_console_before_exit_event = ::rux::new_object< XCrtSect >();
#ifdef __WINDOWS__
	if( ::rux::pdh::_cs == NULL )
		::rux::pdh::_cs = ::rux::new_object< XCrtSect >();
#endif
	if( rux::cryptography::_cs == 0 )
		rux::cryptography::_cs = ::rux::new_object< XCrtSect >();
	if( ::rux::_pre_initialization )
		::rux::_pre_initialization();
	::rux::_pre_initialization = NULL;
};
#ifdef __UNIX__
#ifdef IS_RUX_ENGINE_MODULE
__attribute__ ((destructor(101))) void dll_internal rux_global_deinit( void )
#else
__attribute__ ((destructor(102))) void dll_internal rux_global_deinit( void )
#endif
#else
void dll_internal rux_global_deinit( void )
#endif
{
	::rux::byte res = 1;
	if( ::rux::_post_deinitialization )
		res = ::rux::_post_deinitialization();
	::rux::_post_deinitialization = NULL;
	if( res == 1 )
	{
		::rux::deinitialize();
		if( _cs_engine )
			::rux::delete_object< XCrtSect >( _cs_engine );
		_cs_engine = NULL;
		if( _cs_rux_deinitialization_funcs )
			::rux::delete_object< XCrtSect >( _cs_rux_deinitialization_funcs );
		_cs_rux_deinitialization_funcs = NULL;
		if( ::rux::debug::_cs )
			::rux::delete_object< XCrtSect >( ::rux::debug::_cs );
		::rux::debug::_cs = NULL;
		if( ::rux::_cs_current_module )
			::rux::delete_object< XCrtSect >( ::rux::_cs_current_module );
		::rux::_cs_current_module = NULL;
		if( ::rux::_g_cs_rux_console_before_exit_event )
			::rux::delete_object< XCrtSect >( ::rux::_g_cs_rux_console_before_exit_event );
		::rux::_g_cs_rux_console_before_exit_event = NULL;
	#ifdef __WINDOWS__
		if( ::rux::pdh::_cs )
			::rux::delete_object< XCrtSect >( ::rux::pdh::_cs );
		::rux::pdh::_cs = NULL;
	#endif
		if( ::rux::cryptography::_cs )
			::rux::delete_object< XCrtSect >( ::rux::cryptography::_cs );
		::rux::cryptography::_cs = 0;
		if( ::rux::_last_function )
			::rux::_last_function();
		::rux::_last_function = NULL;
	}
};