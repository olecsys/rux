#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_task.h>
#include <xan_global_variables.h>
#include <xan_engine_globals.h>
namespace rux
{
	namespace task
	{
		dll_internal void add( size_t& task_index , XTHREADFUNC func , void* param , ::rux::uint32 timeout_in_milliseconds , XThreadMode mode )
		{
			::rux::engine::_globals->_add_task( task_index , func , param , timeout_in_milliseconds , mode );
		};
		dll_internal void remove( size_t task_index )
		{
			if( ::rux::engine::_globals && ::rux::engine::_globals->_remove_task )
				::rux::engine::_globals->_remove_task( task_index );
		};
		dll_internal void force_execute( size_t task_index )
		{
			::rux::engine::_globals->_force_execute_task( task_index );
		};
		dll_internal size_t add_event_handler( ::rux::task::event_handler_type event_handler_type , ::rux::task::event_handler_t event_handler )
		{
			return ::rux::engine::_globals->_task_add_event_handler( event_handler_type , event_handler );
		};
		dll_internal void remove_event_handler( size_t event_handler_index )
		{
			::rux::engine::_globals->_task_remove_event_handler( event_handler_index );
		};
	};
};