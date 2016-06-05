#ifndef XAN_TYPEMANAGER_H
#define XAN_TYPEMANAGER_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_crtsect.h>
#include <xan_malloc_array.h>
#include <xan_typemanager_defines.h>
#include <xan_std_allocator.h>
class XTypeManager
{
private:		
	::rux::threading::RdWrLock _cs_types;
	XMallocArray< Type* , 5 > _types;
	::rux::byte _initing;
public:
	XTypeManager();
	~XTypeManager();	
	static Type* TypeOf( rux::int32 module_index , const char* class_name , rux_volatile* rux_type_index , ::rux::register_type_t register_type , ::rux::uint8 is_array , Type* item_type );	
	void FreeTypes( rux::int32 module_index );
	void init( void );
};
extern dll_internal XTypeManager* g_type_manager;
#endif
