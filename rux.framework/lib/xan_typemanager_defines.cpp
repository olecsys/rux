#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_typemanager_defines.h"
dll_internal Type* _typeof( const char* class_name , rux_volatile* rux_type_index , ::rux::register_type_t register_type , ::rux::uint8 is_array , Type* item_type )
{	
	if( ::rux::engine::_globals && ::rux::engine::_globals->_rux_typeof )
		return ::rux::engine::_globals->_rux_typeof( _rux_current_module_index , class_name , rux_type_index , register_type , is_array , item_type );
	else
		return NULL;
};