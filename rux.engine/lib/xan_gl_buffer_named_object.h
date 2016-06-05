#ifndef XAN_GL_NAMED_BUFFER_OBJECT_H
#define XAN_GL_NAMED_BUFFER_OBJECT_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_gui_defines.h>
#include <xan_event.h>
#include <xan_opengl_functions.h>
namespace rux
{
	namespace gui
	{
		namespace engine
		{
			class RenderContext;
			class RenderFont;
			class GLNamedBufferObject
			{			
				GLenum _target;
				GLuint _buffer_id;
				::rux::int32 _size;
				::rux::gui::engine::GLContext* _gl_context;
				GLenum _usage;
			public:
				GLNamedBufferObject( ::rux::gui::engine::GLContext* gl_context );
				~GLNamedBufferObject( void );
				//void Alloc( ::rux::int32 size , ::rux::byte& reinited , GLenum target = GL_PIXEL_UNPACK_BUFFER , GLenum usage = GL_STREAM_DRAW , size_t ___rux__thread_index1986 );
				//void Copy( ::rux::int32 size , void* data , ::rux::byte& reinited , GLenum target = GL_PIXEL_UNPACK_BUFFER , GLenum usage = GL_STREAM_DRAW , size_t ___rux__thread_index1986 );
				void Alloc( ::rux::int32 size , ::rux::byte& reinited , GLenum target , GLenum usage , size_t ___rux__thread_index1986 );
				void Copy( ::rux::int32 size , void* data , ::rux::byte& reinited , GLenum target , GLenum usage , size_t ___rux__thread_index1986 );
				void Bind( size_t ___rux__thread_index1986 );
				void UnBind( size_t ___rux__thread_index1986 );
			};
		};
	};
};
#endif
