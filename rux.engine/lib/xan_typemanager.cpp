#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <xan_typemanager.h>
#include <xan_interlocked.h>
#include <xan_console.h>
dll_internal XTypeManager* g_type_manager = NULL;
XTypeManager::XTypeManager()
{	
	_initing = 0;
};
XTypeManager::~XTypeManager()
{
	WRITE_LOCK( _cs_types );
	for( size_t index0 = 0 ; index0 < _types.Count() ; index0++ )
		_types[ index0 ]->Release( __FILE__ , __LINE__ );
	_cs_types.WriteUnlock();
};
void XTypeManager::init( void )
{
	_initing = 1;
	Type* _Type_type = ::rux::engine::alloc_object< Type >( (const char*)__FILE__ , (::rux::uint32)__LINE__ , (char*)"Type" , (::rux::malloc_t)NULL );
	_Type_type->set_ModuleIndex( _rux_current_module_index );
	_Type_type->_type_hash = ::rux::engine::_globals->add_times33_hash( XType::get_Alias() , SIZE_MAX );
	XInterlocked::CompareExchange( &Type::_rux_type_index , 0 , rux_volatile_max );
	WRITE_LOCK( _cs_types );
	_types.Add( _Type_type );
	_cs_types.WriteUnlock();
	_initing = 0;
};
Type* XTypeManager::TypeOf( rux::int32 module_index , const char *type_name , rux_volatile* rux_type_index , ::rux::register_type_t register_type , ::rux::uint8 is_array , Type* item_type )
{	
	if( g_type_manager->_initing == 0 )
	{
		Type* type_ptr = 0;
		rux_interlocked_return type_index = XInterlocked::CompareExchange( rux_type_index , 0 , 0 );
		if( type_index == rux_volatile_max )
		{
			::rux::uint32 type_hash = ::rux::engine::_globals->add_times33_hash( type_name , SIZE_MAX );
			WRITE_LOCK( g_type_manager->_cs_types );
			type_index = 0;
			for( ; type_index < g_type_manager->_types.Count() ; type_index++ )
			{
				if( g_type_manager->_types[ type_index ]->_type_hash == type_hash )
				{
					type_ptr = g_type_manager->_types[ type_index ];		
					type_ptr->set_ModuleIndex( module_index );		
					XInterlocked::CompareExchange( rux_type_index , type_index , rux_volatile_max );
					break;
				}
			}
			if( type_index >= g_type_manager->_types.Count() )
			{		
				type_ptr = ::rux::engine::alloc_object< Type >( (const char*)__FILE__ , (::rux::uint32)__LINE__ , (char*)type_name , (::rux::malloc_t)NULL );
				type_ptr->_is_array = is_array;
				type_ptr->_array_item_type = item_type;
				type_ptr->_type_hash = type_hash;
				type_ptr->_type_index = type_index;
				type_ptr->set_ModuleIndex( module_index );			
				XInterlocked::CompareExchange( rux_type_index , type_index , rux_volatile_max );
				g_type_manager->_types.Add( type_ptr );
				if( register_type )
					register_type( type_ptr );
			}
			g_type_manager->_cs_types.WriteUnlock();
		}
		else
		{
			READ_LOCK( g_type_manager->_cs_types );
			type_ptr = g_type_manager->_types[ type_index ];
			g_type_manager->_cs_types.ReadUnlock();
		}
		return type_ptr;
	}
	return 0;
};
void XTypeManager::FreeTypes( rux::int32 module_index )
{
	WRITE_LOCK( _cs_types );
	size_t index0 = 0;
	while( index0 < _types.Count() )
	{
		if( g_type_manager->_types[ index0 ]->get_ModuleIndex() != 0 )
		{
			if( g_type_manager->_types[ index0 ]->get_ModuleIndex() == module_index )
			{				
				g_type_manager->_types[ index0 ]->set_ModuleIndex( 0 );
			}
		}
		index0++;
	}
	g_type_manager->_cs_types.WriteUnlock();		
};