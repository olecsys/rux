#ifndef XAN_MODULEMANAGER_H
#define XAN_MODULEMANAGER_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_module.h>
#include <xan_crtsect.h>
#include <xan_console.h>
#include <xan_time.h>
#include <xan_thread.h>
#include <xan_error.h>
#include <xan_debug.h>
class ModuleManager
{
	friend class rux::XModule;
	friend class rux::Module;
private:
	XMallocArray< rux::Module* > _loaded_modules;
	rux::XArray<rux::XString> _search_directories;
	rux::XArray<rux::XString> _exclude_directories;
	XCrtSect _cs_search_directories;
	XCrtSect _cs_loaded_modules;		
	XCrtSect _cs_busy_modules;
	XMallocArray< char* > _busy_modules;	
public:	
	rux_volatile _loaded_modules_by_loadable_module[ 128 ];
	ModuleManager( void );
	~ModuleManager( void );
	static void AddExcludeDirectory( const rux::XString& directory );
	static rux::Module& LoadModule( const rux::XString& module_name , void* init_param , rux::XString& error , rux::int32 loadable_module_index = _rux_current_module_index );
	static void UnLoadModule(const rux::XModule &module , rux::int32 loadable_module_index = _rux_current_module_index );
	static void UnLoadModule(const rux::Module* module_ptr , rux::int32 loadable_module_index = _rux_current_module_index );
	static void UnLoadModule(const rux::XString& module_name , rux::int32 loadable_module_index = _rux_current_module_index );
	static void AddModuleMethodInfo( const rux::Module*  module_ptr , const rux::XString& method_name , void* address_ptr );
	static rux::Array< rux::XString >& get_LoadedModules( void );
	static void loader_unload_loadable_module( rux::int32 loadable_module_index );
private:
	static void update_search_path( void );
};
extern dll_internal ModuleManager* g_module_manager;
#endif
