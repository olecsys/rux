#ifndef XAN_TASKER_H
#define XAN_TASKER_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_task.h>
#include <xan_io_defines.h>
#include <xan_async_command_pool.h>
#include <xan_memorymanager.h>
namespace rux
{
	namespace task
	{
		class Tasker;
		class Task
		{
			friend class Tasker;
		private:
			XTHREADFUNC _func;
			void* _param;
			::rux::uint32 _timeout_in_milliseconds;
			::rux::uint32 _execute_time;
			XThreadMode _mode;
			rux_volatile _locked;
			size_t _task_index;
			::rux::pid_t _locked_tid;
			::rux::byte _release_after_execute;
		public:
			Task( XTHREADFUNC func , void* param , ::rux::uint32 timeout_in_milliseconds , XThreadMode mode );
		};
		class TaskEventHandler
		{
			friend class Tasker;
		private:
			::rux::task::event_handler_type _event_handler_type;
			::rux::task::event_handler_t _event_handler;
		public:
			TaskEventHandler( ::rux::task::event_handler_type event_handler_type , ::rux::task::event_handler_t event_handler );
		};
		extern dll_internal ::rux::threading::RdWrLock* _cs_tasks_event_handlers;
		extern dll_internal XMallocArray< ::rux::task::TaskEventHandler* >* _tasks_event_handlers;
		extern dll_internal ::rux::threading::RdWrLock* _cs_tasks;
		extern dll_internal XMallocArray< ::rux::task::Task* >* _tasks;
		class Tasker
		{
		public:
			::rux::threading::XThread _thread;
		private:
			::rux::threading::XAsyncCommandPool _command_pool;

			static void task_thread( void* param , size_t ___rux__thread_index1986 );
			static void execute_task_command( void* param , size_t ___rux__thread_index1986 );
		public:
			Tasker( void );
			~Tasker( void );
			static void add( size_t& task_index , XTHREADFUNC func , void* param , ::rux::uint32 timeout_in_milliseconds , XThreadMode mode );
			static void remove( size_t& task_index );
			static void force_execute_task( size_t task_index );
			static size_t add_event_handler( ::rux::task::event_handler_type event_handler_type , ::rux::task::event_handler_t event_handler );
			static void remove_event_handler( size_t event_handler_index );
		};
		extern dll_internal Tasker* _tasker;
	};
};
#endif