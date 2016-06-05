#ifndef XAN_TASK_H
#define XAN_TASK_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_includes.h>
#include <xan_enums.h>
#include <xan_thread.h>
namespace rux
{
	namespace task
	{
		dll_internal void add( size_t& task_index , XTHREADFUNC func , void* param , ::rux::uint32 timeout_in_milliseconds , XThreadMode mode );
		dll_internal void remove( size_t task_index );
		dll_internal void force_execute( size_t task_index );
		dll_internal size_t add_event_handler( ::rux::task::event_handler_type event_handler_type , ::rux::task::event_handler_t event_handler );
		dll_internal void remove_event_handler( size_t event_handler_index );
	};
};

#endif