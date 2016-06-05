#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_tasker.h>
#include <xan_interlocked.h>
#include <xan_time.h>
namespace rux
{
	namespace task
	{
		dll_internal Tasker* _tasker = 0;
		dll_internal ::rux::threading::RdWrLock* _cs_tasks = 0;
		dll_internal XMallocArray< ::rux::task::Task* >* _tasks = 0;
		dll_internal ::rux::threading::RdWrLock* _cs_tasks_event_handlers = 0;
		dll_internal XMallocArray< ::rux::task::TaskEventHandler* >* _tasks_event_handlers = 0;
		Task::Task( XTHREADFUNC func , void* param , ::rux::uint32 timeout_in_milliseconds , XThreadMode mode )
		{
			_func = func;
			_param = param;
			_timeout_in_milliseconds = timeout_in_milliseconds;
			_execute_time = 0;
			_mode = mode;
			_locked = 0;
			_task_index = SIZE_MAX;
			_locked_tid = 0;
			_release_after_execute = 0;
		};
		TaskEventHandler::TaskEventHandler( ::rux::task::event_handler_type event_handler_type , ::rux::task::event_handler_t event_handler )
		{
			_event_handler_type = event_handler_type;
			_event_handler = event_handler;
		};
		Tasker::Tasker( void )
		{
			_command_pool.set_MaxThreads( 64 );
			_thread.set_ThreadName( "tasker" );
			_thread.set_ThreadMode( XThreadMode_Repeat );
			_thread.set_Timeout( 10 );
		};
		Tasker::~Tasker( void )
		{
			_thread.Stop();
			::rux::task::_cs_tasks->WriteLock( __FILE__ , __FUNCTION__ , __LINE__ );
			while( ::rux::task::_tasks->ItemsCount() > 0 )
			{
				for( size_t schedule_index = 0 ; schedule_index < ::rux::task::_tasks->Count() ; schedule_index++ )
				{
					::rux::task::Task* task = ::rux::task::_tasks->operator[]( schedule_index );
					if( task )
					{
						if( XInterlocked::CompareExchange( &task->_locked , 0 , 0 ) == 0 )
						{
							::rux::task::_tasks->set_EmptyAt( task->_task_index );

							::rux::task::_cs_tasks_event_handlers->ReadLock( __FILE__ , __FUNCTION__ , __LINE__ );
							for( size_t event_handlers_index = 0 ; event_handlers_index < ::rux::task::_tasks_event_handlers->Count() ; event_handlers_index++ )
							{
								::rux::task::TaskEventHandler* task_event_handler = ::rux::task::_tasks_event_handlers->operator[]( event_handlers_index );
								if( task_event_handler && task_event_handler->_event_handler_type == ::rux::task::event_handler_type_wait_for_exit )
								{
									::rux::task::event event;
									event._func = task->_func;
									event._param = task->_param;
									event._task_index = task->_task_index;
									event._wait_for_exit = 1;
									task_event_handler->_event_handler( &event );
								}
							}
							::rux::task::_cs_tasks_event_handlers->ReadUnlock();

							::rux::memory::release< ::rux::task::Task >( task );
						}
					}
				}
				if( ::rux::task::_tasks->ItemsCount() > 0 )
					::rux::threading::XThread::Sleep( 1 );
			}
			::rux::task::_cs_tasks->WriteUnlock();
			_command_pool.Stop();
		};
		void Tasker::execute_task_command( void* param , size_t ___rux__thread_index1986 )
		{
			Task* task = (Task*)param;
			task->_locked_tid = ::rux::threading::XThread::get_CurrentThreadId();
			task->_execute_time = ::rux::XTime::GetTickCount();
			task->_func( task->_param , ___rux__thread_index1986 );
			task->_locked_tid = 0;
			if( task->_mode == XThreadMode_OneTime )
			{
				::rux::task::_cs_tasks->WriteLock( __FILE__ , __FUNCTION__ , __LINE__ );
				if( task->_task_index < ::rux::task::_tasks->Count() && ::rux::task::_tasks->operator[]( task->_task_index ) == task )
				{
					::rux::task::_tasks->set_EmptyAt( task->_task_index );
					::rux::task::_cs_tasks->WriteUnlock();

					::rux::task::_cs_tasks_event_handlers->ReadLock( __FILE__ , __FUNCTION__ , __LINE__ );
					for( size_t event_handlers_index = 0 ; event_handlers_index < ::rux::task::_tasks_event_handlers->Count() ; event_handlers_index++ )
					{
						::rux::task::TaskEventHandler* task_event_handler = ::rux::task::_tasks_event_handlers->operator[]( event_handlers_index );
						if( task_event_handler && task_event_handler->_event_handler_type == ::rux::task::event_handler_type_wait_for_exit )
						{
							::rux::task::event event;
							event._func = task->_func;
							event._param = task->_param;
							event._task_index = task->_task_index;
							event._wait_for_exit = 1;
							task_event_handler->_event_handler( &event );
						}
					}
					::rux::task::_cs_tasks_event_handlers->ReadUnlock();

					::rux::memory::release< ::rux::task::Task >( task );
				}
				else
				{
					::rux::task::_cs_tasks->WriteUnlock();
					XInterlocked::Exchange( &task->_locked , 0 );
				}
			}
			else
				XInterlocked::Exchange( &task->_locked , 0 );
			if( task && task->_release_after_execute )
				::rux::memory::release< ::rux::task::Task >( task );
		};
		void Tasker::task_thread( void* param , size_t ___rux__thread_index1986 )
		{
			Tasker* tasker = (Tasker*)param;
			::rux::task::_cs_tasks->WriteLock( __FILE__ , __FUNCTION__ , __LINE__ );
			if( ::rux::task::_tasks->ItemsCount() > 0 )
			{
				::rux::uint32 now = ::rux::XTime::GetTickCount();
				for( size_t schedule_index = 0 ; schedule_index < ::rux::task::_tasks->Count() ; schedule_index++ )
				{
					::rux::task::Task* task = ::rux::task::_tasks->operator[]( schedule_index );
					if( task )
					{
						if( XInterlocked::CompareExchange( &task->_locked , 1 , 0 ) == 0 )
						{						
							if( task->_mode == XThreadMode_OneTime )
								tasker->_command_pool.Execute( execute_task_command , task );
							else
							{
								::rux::task::_cs_tasks->WriteUnlock();
								if( task->_execute_time > now )
									task->_execute_time = now;
								if( now - task->_execute_time >= task->_timeout_in_milliseconds - 15 )
									tasker->_command_pool.Execute( execute_task_command , task );
								else
									XInterlocked::Exchange( &task->_locked , 0 );
								::rux::task::_cs_tasks->WriteLock( __FILE__ , __FUNCTION__ , __LINE__ );
							}
						}
					}
				}
			}
			::rux::task::_cs_tasks->WriteUnlock();
		};
		void Tasker::add( size_t& task_index , XTHREADFUNC func , void* param , ::rux::uint32 timeout_in_milliseconds , XThreadMode mode )
		{
			::rux::task::Task* task = alloc_object_4_params_macros( ::rux::task::Task , func , param , timeout_in_milliseconds , mode ); 
			::rux::task::_cs_tasks->WriteLock( __FILE__ , __FUNCTION__ , __LINE__ );
			task->_task_index = ::rux::task::_tasks->FillEmptyOrAdd( task );
			task_index = task->_task_index;
			::rux::task::_cs_tasks->WriteUnlock();
			if( ::rux::task::_tasker )
			{
				::rux::task::_tasker->_thread.Start();
				::rux::task::_tasker->_thread.Push( task_thread , ::rux::task::_tasker );
			}
		};
		void Tasker::remove( size_t& task_index )
		{
			::rux::task::Task* task = 0;
			::rux::task::_cs_tasks->WriteLock( __FILE__ , __FUNCTION__ , __LINE__ );
			if( task_index < ::rux::task::_tasks->Count() )
			{
				task = ::rux::task::_tasks->operator[]( task_index );
				::rux::task::_tasks->set_EmptyAt( task_index );
				task_index = SIZE_MAX;
			}
			::rux::task::_cs_tasks->WriteUnlock();
			if( task )
			{
				::rux::task::_cs_tasks_event_handlers->ReadLock( __FILE__ , __FUNCTION__ , __LINE__ );
				for( size_t event_handlers_index = 0 ; event_handlers_index < ::rux::task::_tasks_event_handlers->Count() ; event_handlers_index++ )
				{
					::rux::task::TaskEventHandler* task_event_handler = ::rux::task::_tasks_event_handlers->operator[]( event_handlers_index );
					if( task_event_handler && task_event_handler->_event_handler_type == ::rux::task::event_handler_type_wait_for_exit )
					{
						::rux::task::event event;
						event._func = task->_func;
						event._param = task->_param;
						event._task_index = task->_task_index;
						event._wait_for_exit = 1;
						task_event_handler->_event_handler( &event );
					}
				}
				::rux::task::_cs_tasks_event_handlers->ReadUnlock();
				while( XInterlocked::CompareExchange( &task->_locked , 0 , 0 ) != 0 )
				{
					if( task->_locked_tid == ::rux::threading::XThread::get_CurrentThreadId() )
					{
						task->_release_after_execute = 1;
						break;
					}
					::rux::threading::XThread::Sleep( 1 );
				}
				if( task->_release_after_execute == 0 )
					::rux::memory::release< ::rux::task::Task >( task );
			}
		};
		size_t Tasker::add_event_handler( ::rux::task::event_handler_type event_handler_type , ::rux::task::event_handler_t event_handler )
		{
			::rux::task::TaskEventHandler* task = alloc_object_2_params_macros( ::rux::task::TaskEventHandler , event_handler_type , event_handler ); 
			::rux::task::_cs_tasks_event_handlers->WriteLock( __FILE__ , __FUNCTION__ , __LINE__ );
			size_t task_index = ::rux::task::_tasks_event_handlers->FillEmptyOrAdd( task );
			::rux::task::_cs_tasks_event_handlers->WriteUnlock();
			return task_index;
		};
		void Tasker::remove_event_handler( size_t event_handler_index )
		{
			::rux::task::_cs_tasks_event_handlers->WriteLock( __FILE__ , __FUNCTION__ , __LINE__ );
			if( event_handler_index < ::rux::task::_tasks_event_handlers->Count() )
			{
				::rux::task::TaskEventHandler* task = ::rux::task::_tasks_event_handlers->operator[]( event_handler_index );
				::rux::task::_tasks_event_handlers->set_EmptyAt( event_handler_index );
				::rux::memory::release< ::rux::task::TaskEventHandler >( task );
			}
			::rux::task::_cs_tasks_event_handlers->WriteUnlock();
		};
		void Tasker::force_execute_task( size_t task_index )
		{
			::rux::task::Task* task = 0;
			::rux::task::_cs_tasks->WriteLock( __FILE__ , __FUNCTION__ , __LINE__ );
			if( task_index < ::rux::task::_tasks->Count() )
			{
				task = ::rux::task::_tasks->operator[]( task_index );
				if( task )
				{
					task->_execute_time = 0;
					if( ::rux::task::_tasker )
					{
						::rux::task::_tasker->_thread.Start();
						::rux::task::_tasker->_thread.Push( task_thread , ::rux::task::_tasker );
					}
				}
			}
			::rux::task::_cs_tasks->WriteUnlock();
		};
	};
};