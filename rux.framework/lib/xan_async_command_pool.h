#ifndef XAN_ASYNC_COMMAND_POOL_H
#define XAN_ASYNC_COMMAND_POOL_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_thread.h"
#include "xan_array.h"
#include "xan_keyvaluepair.h"
#include "xan_crtsect.h"
namespace rux
{
	namespace threading
	{
		class AsyncCommandPool;
		class XAsyncCommandPoolItem
		{
			friend class AsyncCommandPool;
		private:
			::rux::uint32 _last_execute_time;
			::rux::threading::Thread* _thread;
			AsyncCommandPool* _async_command_pool;
			size_t _items_index;
		public:
			XAsyncCommandPoolItem( AsyncCommandPool* async_command_pool );
			~XAsyncCommandPoolItem( void );
		};
	begin_declare_rux_class( AsyncCommandPool );
		void clean_unneccessary_items( void );
	public:
		~AsyncCommandPool();
		void Execute( XTHREADFUNC command , void* param );
		void set_MaxThreads( size_t max_threads );
		void Stop( void );
		void Block( void );
		void UnBlock( void );
		::rux::byte Blocked( void );
		::rux::byte IsThreadIdInPool( ::rux::pid_t tid );
	begin_declare_rux_class_members( AsyncCommandPool );
		rux_volatile _blocked;
		rux_volatile _stopping;
		rux_volatile _threads_count;
		::rux::threading::RdWrLock _cs_items;		
		size_t _max_threads;
		::rux::threading::RdWrLock _cs_commands;
		XMallocArray< void* > _params;
		XMallocArray< void* > _commands;
		XMallocArray< XAsyncCommandPoolItem* > _items00;
		static void command( void* param , size_t ___rux__thread_index1986 );
		static void thread_before_exit( void* param );
	end_declare_rux_class_members( AsyncCommandPool );
		void Execute( XTHREADFUNC command , void* param );
		void set_MaxThreads( size_t max_threads );
		void Stop( void );
		void Block( void );
		void UnBlock( void );
		::rux::byte Blocked( void );
		::rux::byte IsThreadIdInPool( ::rux::pid_t tid );
	end_declare_rux_class();
	};
};
#endif