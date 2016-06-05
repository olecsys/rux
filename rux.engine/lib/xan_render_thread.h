#ifndef XAN_RENDER_THREAD_H
#define XAN_RENDER_THREAD_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_thread.h>
namespace rux
{
	namespace gui
	{
		namespace engine
		{
			class Window;
			class RenderContext;
			class RenderFont;
			class RenderThread
			{
				friend class ::rux::gui::engine::RenderContext;
				friend class ::rux::gui::engine::RenderFont;
			private:
				rux::threading::XThread _force_thread;
			private:
				static void force_render_thread( void* param , size_t ___rux__thread_index1986 );
				static void before_exit_force_render_thread( void* param );
			public:
				RenderThread( ::rux::gui::engine::RenderContext* render_context );
				~RenderThread( void );
			};
		};
	};
};
#endif
