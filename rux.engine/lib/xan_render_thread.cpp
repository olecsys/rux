#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_render_context.h>
#include <xan_render_thread.h>
#include <xan_thread.h>
#include <xan_gui_window.h>
#include <xan_gui_engine.h>
#include <xan_log.h>
#include <xan_time.h>
namespace rux
{
	namespace gui
	{
		namespace engine
		{
			RenderThread::RenderThread( ::rux::gui::engine::RenderContext* render_context )
			{	
				::rux::log::WriteDebug( "%s:%d BEGIN" , __FUNCTION__ , __LINE__ );

				_force_thread.set_BeforeExit( before_exit_force_render_thread , render_context );
				_force_thread.set_ThreadMode( XThreadMode_Repeat );
				_force_thread.set_StackSize( 512 * 1024 );
				_force_thread.set_Timeout( 10 );
				_force_thread.set_ThreadName( "ui force thread" );				
				_force_thread.Start();
				_force_thread.Push( force_render_thread , render_context );

				::rux::log::WriteDebug( "%s:%d END" , __FUNCTION__ , __LINE__ );
			};
			RenderThread::~RenderThread()
			{
				if( _force_thread.get_IsStarted() == 1 )
					_force_thread.Stop();
			};
			void RenderThread::before_exit_force_render_thread( void* param )
			{
				RenderContext* render_context = (RenderContext*)param;
				render_context->_gl->set_EmptyContext();
			};
			void RenderThread::force_render_thread( void* param , size_t ___rux__thread_index1986 )
			{
				RenderContext* render_context = (RenderContext*)param;
				if(::booldog::interlocked::compare_exchange(&render_context->_enable_state, ::booldog::interlocked::max
					, 1) == 1)
				{
					::rux::uint32 now_tickcount = ::rux::XTime::GetTickCount();
					if( render_context->_render_time > now_tickcount )
						render_context->_render_time = now_tickcount;
					if( now_tickcount - render_context->_render_time >= render_context->_render_timeout )
					{
						render_context->_render_time = now_tickcount;
						render_context->_gl->set_CurrentContext( render_context->_window );
						render_context->Render( ___rux__thread_index1986 );
						render_context->_gl->SwapBuffers( ___rux__thread_index1986 );
						if( rux::gui::engine::_log_enable == 1 )
						{
							if( render_context->_fps.get_Runtime() >= 1000ULL * 10000ULL )
							{
								::rux::log::WriteTrace( "window, OpenGL render fps=%.1f" , render_context->_fps.get_FPS() );
								render_context->_fps.Reset();
							}
							else
								render_context->_fps.Increment();
						}
					}
					else
						render_context->_gl->set_CurrentContext( render_context->_window );
					render_context->CleanAfterRender();
					::booldog::interlocked::exchange(&render_context->_enable_state, 1);
					::rux::threading::XThread::Sleep( 1 );
				}
			};
		};
	};
};
