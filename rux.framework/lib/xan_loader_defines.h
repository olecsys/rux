#ifndef XAN_LOADER_DEFINES_H
#define XAN_LOADER_DEFINES_H
#include <xan_module.h>
namespace rux
{	
	typedef void (* rux_add_exclude_directory_t)( const ::rux::XString& directory );
	typedef void (*rux_loader_unload_loadable_module_t)( ::rux::int32 loadable_module_index );
	typedef Module& (* rux_load_module_t)( const ::rux::XString& module_name , void* init_param , ::rux::XString& error , ::rux::int32 loadable_module_index );
	typedef void (* rux_unload_module_t)( const XModule& module , ::rux::int32 loadable_module_index );
	typedef void (*rux_set_conf_property_t)( const char* conf_property , const char* value );
	typedef void (* rux_unload_module_by_name_t)( const ::rux::XString& module_name , ::rux::int32 loadable_module_index );
	typedef void (* rux_add_module_method_info_t)( const Module* module_ptr , const ::rux::XString& method_name , void* address_ptr );
	typedef ::rux::Array< ::rux::XString >& (* rux_get_loaded_modules_t)( void );
	typedef void (* rux_loadable_increment_t)( ::rux::int32 loadable_module_index );
	namespace loader
	{
		Module& LoadModule( const ::rux::XString& module_name , void* init_param , ::rux::XString& error , ::rux::int32 loadable_module_index = _rux_current_module_index );
		void UnLoadModule(const Module &module , ::rux::int32 loadable_module_index = _rux_current_module_index );
		void UnLoadModule(const XModule &module , ::rux::int32 loadable_module_index = _rux_current_module_index );
		void UnLoadModule(const ::rux::XString& module_name , ::rux::int32 loadable_module_index = _rux_current_module_index );
		Array< ::rux::XString >& get_LoadedModules( void );
		void AddExcludeDirectory( const ::rux::XString& directory );
		namespace engine
		{
			class globals
			{
			public:
				globals( void );
				rux_load_module_t _rux_load_module;
				rux_loader_unload_loadable_module_t _rux_loader_unload_loadable_module;
				rux_add_exclude_directory_t _rux_add_exclude_directory;
				rux_unload_module_t _rux_unload_module;
				rux_set_conf_property_t _rux_set_conf_property;
				rux_unload_module_by_name_t _rux_unload_module_by_name;		
				rux_add_module_method_info_t _rux_add_module_method_info;
				rux_get_loaded_modules_t _rux_get_loaded_modules;
				rux_loadable_increment_t _rux_loadable_increment;
				rux_loadable_increment_t _rux_loadable_decrement;
			};
		};
	};
};
#endif
