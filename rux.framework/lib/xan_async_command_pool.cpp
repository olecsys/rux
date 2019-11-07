#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_async_command_pool.h"
#include "xan_uint64.h"
#include "xan_boolean.h"
#include "xan_log.h"
#include "xan_time.h"
begin_implement_rux_class_ns( AsyncCommandPool , rux::threading );
	_threads_count = 0;
	_max_threads = 3;
	_stopping = 0;
end_implement_rux_class();
namespace rux
{
	namespace threading
	{	
		XAsyncCommandPoolItem::XAsyncCommandPoolItem( AsyncCommandPool* async_command_pool )
		{
			_last_execute_time = ::rux::XTime::GetTickCount();
			_items_index = SIZE_MAX;
			_async_command_pool = async_command_pool;
			_thread = alloc_object_macros( ::rux::threading::Thread ); 
		};
		XAsyncCommandPoolItem::~XAsyncCommandPoolItem( void )
		{
			_thread->Stop();
			_thread->Release();
		};
		AsyncCommandPool::~AsyncCommandPool()
		{
			Stop();
		};
		void AsyncCommandPool::set_MaxThreads( size_t max_threads )
		{
			if( max_threads > 0 )
			{
				WRITE_LOCK( _cs_items );
				_max_threads = max_threads;
				_cs_items.WriteUnlock();
			}
		};
		void XAsyncCommandPool::set_MaxThreads( size_t max_threads )
		{
			(*this)()->set_MaxThreads( max_threads );
		};
		void AsyncCommandPool::Execute( XTHREADFUNC command , void* param )
		{
			if( XInterlocked::CompareExchange( &_blocked , 0 , 0 ) == 0 )
			{
				if( XInterlocked::CompareExchange( &_stopping , 0 , 0 ) == 0 )
				{
					_cs_commands.WriteLock();
					_params.Add( param );
					_commands.Add( (void*)command );
					_cs_commands.WriteUnlock();
					WRITE_LOCK( _cs_items );
					size_t index0 = 0;
					for( ; index0 < _items00.Count() ; index0++ )
					{
						if( _items00[ index0 ] )
						{
							if( _items00[ index0 ]->_thread->get_IsBusy() == false )
							{
								_items00[ index0 ]->_thread->Push( AsyncCommandPool::command , _items00[ index0 ] );
								break;
							}
						}
					}
					if( index0 >= _items00.Count() )				
					{
						::XInterlocked::Increment( &_threads_count );
						XAsyncCommandPoolItem* async_command_pool_item = alloc_object_1_param_macros( XAsyncCommandPoolItem , this );
						
						if( ::rux::engine::_globals && ::rux::engine::_globals->_log_level == ::rux::log::XEnum_LogLevel_Verbose )
							::rux::log::WriteTrace( "%s, %s:%d, new item %p" , __FUNCTION__ , __FILE__ , __LINE__ , async_command_pool_item );
						::rux::memory::check_memory( async_command_pool_item , 0 , sizeof( XAsyncCommandPoolItem ) );
						
						async_command_pool_item->_thread->set_BeforeExit( thread_before_exit , async_command_pool_item );
						async_command_pool_item->_thread->set_Timeout( 200 );
						async_command_pool_item->_thread->set_ThreadName( "async command pool" );
						async_command_pool_item->_thread->set_ThreadMode( XThreadMode_Repeat );
						
						::rux::memory::check_memory( async_command_pool_item , 0 , sizeof( XAsyncCommandPoolItem ) );
						
						async_command_pool_item->_items_index = _items00.FillEmptyOrAdd( async_command_pool_item );

						async_command_pool_item->_thread->Start();
						
						::rux::memory::check_memory( async_command_pool_item , 0 , sizeof( XAsyncCommandPoolItem ) );
						
						async_command_pool_item->_thread->Push( AsyncCommandPool::command , async_command_pool_item );
					}
					_cs_items.WriteUnlock();
				}
			}
		};
		void XAsyncCommandPool::Execute( XTHREADFUNC command , void* param )
		{
			(*this)()->Execute( command , param );
		};
		implement_duplicate_internal_function( AsyncCommandPool , Block );
		void AsyncCommandPool::Block( void )
		{
			XInterlocked::Exchange( &_blocked , 1 );
			Stop();
		};
		implement_duplicate_internal_result_function( ::rux::byte , AsyncCommandPool , Blocked );
		::rux::byte AsyncCommandPool::Blocked( void )
		{
			return XInterlocked::CompareExchange( &_blocked , 0 , 0 ) == 0 ? 0 : 1;
		};
		implement_duplicate_internal_function( AsyncCommandPool , UnBlock );
		void AsyncCommandPool::UnBlock( void )
		{
			XInterlocked::Exchange( &_blocked , 0 );
		};
		void XAsyncCommandPool::Stop( void )
		{
			(*this)()->Stop();
		};
		void AsyncCommandPool::Stop( void )
		{
			XInterlocked::Exchange( &_stopping , 1 );
			WRITE_LOCK( _cs_items );
			while( _items00.ItemsCount() > 0 )
			{
				for( size_t index0 = 0 ; index0 <  _items00.Count() ; index0++ )
				{
					if( _items00[ index0 ] )
					{
						declare_variable( ::rux::threading::XThread , thread );
						thread.set_ByRef( _items00[ index0 ]->_thread );
						_items00.set_EmptyAt( index0 );
						_cs_items.WriteUnlock();
						thread.Stop();
						WRITE_LOCK( _cs_items );
					}
				}
			}
			_cs_items.WriteUnlock();
			while( ::XInterlocked::CompareExchange( &_threads_count , 0 , 0 ) > 0 )
				::rux::threading::XThread::Sleep( 1 );
			XInterlocked::Exchange( &_stopping , 0 );
		};
		void AsyncCommandPool::thread_before_exit( void* param )
		{
			XAsyncCommandPoolItem* async_command_pool_item = (XAsyncCommandPoolItem*)param;
			
			::rux::memory::check_memory( async_command_pool_item , 0 , sizeof( XAsyncCommandPoolItem ) );
			
			AsyncCommandPool* async_command_pool = async_command_pool_item->_async_command_pool;
			
			if( ::rux::engine::_globals && ::rux::engine::_globals->_log_level == ::rux::log::XEnum_LogLevel_Verbose )
				::rux::log::WriteTrace( "%s, %s:%d, delete item %p" , __FUNCTION__ , __FILE__ , __LINE__ , async_command_pool_item );
			
			::rux::engine::free_object< XAsyncCommandPoolItem >( async_command_pool_item );
			::XInterlocked::Decrement( &async_command_pool->_threads_count );
		};
		void AsyncCommandPool::command( void* param , size_t ___rux__thread_index1986 )
		{
			XAsyncCommandPoolItem* item = (XAsyncCommandPoolItem*)param;
			
			::rux::memory::check_memory( item , 0 , sizeof( XAsyncCommandPoolItem ) );
			
			item->_async_command_pool->_cs_commands.WriteLock();
			while( item->_async_command_pool->_commands.Count() > 0 )
			{
				void* _param = item->_async_command_pool->_params[ 0 ];
				item->_async_command_pool->_params.RemoveAt( 0 );
				XTHREADFUNC _command = (XTHREADFUNC)item->_async_command_pool->_commands[ 0 ];
				item->_async_command_pool->_commands.RemoveAt( 0 );
				item->_async_command_pool->_cs_commands.WriteUnlock();
				if( _command )
				{
					item->_last_execute_time = ::rux::XTime::GetTickCount();
					_command( _param , ___rux__thread_index1986 );
				}
				item->_async_command_pool->_cs_commands.WriteLock();
			}
			item->_async_command_pool->_cs_commands.WriteUnlock();
			item->_async_command_pool->clean_unneccessary_items();
		};
		void AsyncCommandPool::clean_unneccessary_items( void )
		{
			if( _cs_items.TryWriteLock( __FILE__ , __FUNCTION__ , __LINE__ , 1 ) )
			{
				size_t count = 0;
				::rux::uint32 now = ::rux::XTime::GetTickCount();
				XAsyncCommandPoolItem* item = 0;
				::rux::uint32 timeout_in_ms = 5000;
				for( size_t index0 = 0 ; index0 < _items00.Count() ; index0++ )
				{
					count++;
					item = _items00[ index0 ];
					if( item )
					{
						if( item->_thread->get_IsBusy() == false )
						{
							if( item->_last_execute_time > now )
								item->_last_execute_time = now;
							if( now - item->_last_execute_time >= timeout_in_ms )
							{
								declare_variable( ::rux::threading::XThread , thread );
								thread.set_ByRef( _items00[ index0 ]->_thread );
								_items00.set_EmptyAt( index0 );
								_cs_items.WriteUnlock();
								thread.Stop();
								WRITE_LOCK( _cs_items );
								count--;
							}
						}
					}
					if( count >= _max_threads )
						timeout_in_ms = 500;
				}
				_cs_items.WriteUnlock();
			}
		};
		implement_duplicate_internal_result_function_1( ::rux::byte , AsyncCommandPool , IsThreadIdInPool , ::rux::pid_t );
		::rux::byte AsyncCommandPool::IsThreadIdInPool( ::rux::pid_t tid )
		{
			::rux::byte res = 0;
			READ_LOCK( _cs_items );
			for( size_t index0 = 0 ; index0 <  _items00.Count() ; index0++ )
			{
				if( _items00[ index0 ] )
				{
					if( _items00[ index0 ]->_thread->get_ThreadId() == tid )
					{
						res = 1;
						break;
					}
				}
			}
			_cs_items.ReadUnlock();
			return res;
		};
	};
};