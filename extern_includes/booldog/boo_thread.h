#ifndef BOO_THREAD_H
#define BOO_THREAD_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "boo_interlocked.h"
#include "boo_threading_utils.h"
#include "boo_result.h"
#include "boo_error.h"

#ifdef __WINDOWS__
#include <process.h>
#else
#ifndef _LARGEFILE64_SOURCE 
#define _LARGEFILE64_SOURCE 
#endif
#include <pthread.h>
#include <unistd.h>
#include <limits.h>
#endif
#ifndef BOO_THREAD_CPU_MIN
#define BOO_THREAD_CPU_MIN 0
#endif
#ifndef BOO_THREAD_CPU_MAX
#define BOO_THREAD_CPU_MAX INT_MAX
#endif
#ifndef BOO_THREAD_EXCLUDE_CPU
#define BOO_THREAD_EXCLUDE_CPU INT_MAX
#endif
namespace booldog
{
	namespace enums
	{
		namespace threading
		{
			enum state
			{
				state_pending_in_start ,
				state_start ,
				state_stop ,
				state_pending_in_stop
			};
		}
	}
	namespace threading
	{
		class thread;
	};
	namespace events
	{
		namespace typedefs
		{
			typedef void (*onthreadstarted)(::booldog::threading::thread* thread);
			typedef void (*onthreadstopped)(::booldog::threading::thread* thread);
			typedef void (*onthreadprocedure)(::booldog::threading::thread* thread);
		};
	};
	namespace threading
	{
		class thread
		{
			::booldog::interlocked::atomic _state;
			booldog::allocator* _allocator;
			void* _udata;
			::booldog::events::typedefs::onthreadstarted _onthreadstarted;
			::booldog::events::typedefs::onthreadstopped _onthreadstopped;
			::booldog::events::typedefs::onthreadprocedure _onthreadprocedure;
#ifdef __WINDOWS__
			HANDLE _handle;
#else
			pthread_t _handle;
#endif
		public:
			::booldog::threading::thread* volatile _next;
		public:
			thread(::booldog::allocator* allocator)
				: _allocator(allocator)
			{
			}
			void* udata()
			{
				return _udata;
			}
			bool pending_in_stop()
			{
				return booldog::interlocked::compare_exchange(&_state, 0, 0)
					== ::booldog::enums::threading::state_pending_in_stop;
			}
		private:
#ifdef __WINDOWS__
			static unsigned __stdcall func(void* param)
#else
			static void* func(void* param)
#endif
			{
#ifndef __WINDOWS__
				pthread_detach(pthread_self());
#endif
				::booldog::threading::thread* thr = (::booldog::threading::thread*)param;
				::booldog::interlocked::exchange( &thr->_state , ::booldog::enums::threading::state_start );
				if( thr->_onthreadstarted )
					thr->_onthreadstarted( thr );
				thr->_onthreadprocedure( thr );
#ifdef __WINDOWS__
				CloseHandle( thr->_handle ); 
#endif
				if( thr->_onthreadstopped )
					thr->_onthreadstopped( thr );
				::booldog::interlocked::exchange( &thr->_state , ::booldog::enums::threading::state_stop );
				return 0;
			}
		public:
			booinline static bool create(::booldog::result* pres, ::booldog::threading::thread* thr, size_t stack_size
				, ::booldog::events::typedefs::onthreadstarted onthreadstarted
				, ::booldog::events::typedefs::onthreadstopped onthreadstopped
				, ::booldog::events::typedefs::onthreadprocedure onthreadprocedure, void* udata
				, const ::booldog::debug::info& debuginfo = debuginfo_macros)
			{
				debuginfo = debuginfo;
				::booldog::result locres;
				BOOINIT_RESULT(::booldog::result);
				::booldog::interlocked::exchange(&thr->_state, ::booldog::enums::threading::state_pending_in_start);
				thr->_udata = udata;
				thr->_onthreadstarted = onthreadstarted;
				thr->_onthreadstopped = onthreadstopped;
				thr->_onthreadprocedure = onthreadprocedure;
#ifdef __WINDOWS__
				thr->_handle = (HANDLE)_beginthreadex(0, (unsigned)stack_size, ::booldog::threading::thread::func, thr
					, 0, 0);
				if(thr->_handle == 0)
				{
					res->seterrno();
					goto goto_destroy;
				}
#else
				cpu_set_t cpus;
				int numberOfProcessors = 0;
				pthread_attr_t pthread_attr;		
				int result = pthread_attr_init(&pthread_attr);		
				if(result != 0)
				{
					res->setpthreaderror(result);
					goto goto_destroy;
				}
				if(stack_size != 0)
				{
#ifdef PTHREAD_STACK_MIN
					if(stack_size < PTHREAD_STACK_MIN)
						stack_size = PTHREAD_STACK_MIN;
					else
#endif
					{
						long sz = sysconf(_SC_PAGESIZE);
						if(stack_size % sz)
							stack_size = sz * (stack_size / sz) + sz;
					}
					result = pthread_attr_setstacksize(&pthread_attr, stack_size);							
					if(result != 0)
					{
						res->setpthreaderror(result);
						pthread_attr_destroy(&pthread_attr);
						goto goto_destroy;
					}
				}
				result = pthread_attr_setdetachstate(&pthread_attr, PTHREAD_CREATE_DETACHED);
				if(result != 0)
				{
					res->setpthreaderror(result);
					pthread_attr_destroy(&pthread_attr);
					goto goto_destroy;
				}
				numberOfProcessors = sysconf(_SC_NPROCESSORS_ONLN);
				if(numberOfProcessors > BOO_THREAD_CPU_MAX)
					numberOfProcessors = BOO_THREAD_CPU_MAX;
				CPU_ZERO(&cpus);
				for(int i = BOO_THREAD_CPU_MIN;i < numberOfProcessors; ++i)
				{
					if(i != BOO_THREAD_EXCLUDE_CPU)
						CPU_SET(i, &cpus);
				}
				pthread_attr_setaffinity_np(&pthread_attr, sizeof(cpu_set_t), &cpus);
				result = pthread_create(&thr->_handle, &pthread_attr, func, (void*)thr);
				if(result != 0)
				{
					res->setpthreaderror(result);
					pthread_attr_destroy(&pthread_attr);
					goto goto_destroy;
				}
#endif
				goto goto_return;
goto_destroy:
				::booldog::interlocked::exchange(&thr->_state, ::booldog::enums::threading::state_stop);
				return false;
goto_return:
				return true;
			}
			booinline static ::booldog::threading::thread* create(::booldog::result* pres, booldog::allocator* allocator
				, size_t stack_size, ::booldog::events::typedefs::onthreadstarted onthreadstarted
				, ::booldog::events::typedefs::onthreadstopped onthreadstopped
				, ::booldog::events::typedefs::onthreadprocedure onthreadprocedure, void* udata
				, const ::booldog::debug::info& debuginfo = debuginfo_macros)
			{
				::booldog::result locres;
				BOOINIT_RESULT(::booldog::result);
				::booldog::threading::thread* thr = allocator->create< ::booldog::threading::thread >(allocator
					, debuginfo);
				if(thr)
				{
					if(::booldog::threading::thread::create(res, thr, stack_size, onthreadstarted, onthreadstopped
						, onthreadprocedure, udata, debuginfo))
						return thr;
					else
					{
						allocator->destroy< ::booldog::threading::thread >(thr);
						thr = 0;
					}
				}
				else
					res->booerr(::booldog::enums::result::booerr_type_cannot_alloc_memory);
				return thr;
			}
			booinline static void wait_for_start( ::booldog::threading::thread* thr )
			{
				if( booldog::interlocked::compare_exchange( &thr->_state , 0 , 0 ) == ::booldog::enums::threading::state_pending_in_start )
				{
					booldog::byte tries = 0;
					while( booldog::interlocked::compare_exchange( &thr->_state , 0 , 0 ) != ::booldog::enums::threading::state_start )
					{
						tries++;
						if( tries == 5 )
						{
							::booldog::threading::sleep( 1 );
							tries = 0;
						}
					}
				}
			};
			booinline static void stop( ::booldog::threading::thread* thr )
			{
				::booldog::threading::thread::wait_for_start( thr );
				booldog::interlocked::compare_exchange( &thr->_state , ::booldog::enums::threading::state_pending_in_stop
					, ::booldog::enums::threading::state_start );
			};			
			booinline static void wait_for_stop( ::booldog::threading::thread* thr )
			{
				booldog::byte tries = 0;
				while( booldog::interlocked::compare_exchange( &thr->_state , 0 , 0 ) != ::booldog::enums::threading::state_stop )
				{
					tries++;
					if( tries == 5 )
					{
						::booldog::threading::sleep( 1 );
						tries = 0;
					}
				}
			}
			booinline static void destroy( ::booldog::threading::thread* thr )
			{
				::booldog::threading::thread::wait_for_stop( thr );
				thr->_allocator->destroy< ::booldog::threading::thread >( thr );
			}
			booinline bool stopped(void)
			{
                                return ::booldog::interlocked::compare_exchange(&_state, 0, 0) == ::booldog::enums::threading::state_stop;
			}
		};
	};
};
#endif
