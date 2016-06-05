#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_gl_buffer_named_object.h>
#include <xan_opengl_functions.h>
#include <xan_thread.h>
#include <xan_render_context.h>
#include <xan_render_font.h>
#include <xan_log.h>
#include <xan_gui_window.h>
namespace rux
{
	namespace gui
	{
		namespace engine
		{				
			GLNamedBufferObject::GLNamedBufferObject( ::rux::gui::engine::GLContext* gl_context )
			{
				_gl_context = gl_context;
				_size = 0;
				_buffer_id = 0;
				_target = GL_PIXEL_UNPACK_BUFFER;
				_usage = GL_STREAM_DRAW;
			};
			GLNamedBufferObject::~GLNamedBufferObject( void )
			{
				if( _size > 0 )
				{
					rux::gui::engine::OpenGL::glDeleteBuffers( 1 , &_buffer_id , _gl_context , __FUNCTION__ , __LINE__ , SIZE_MAX );
				}
			};
			void GLNamedBufferObject::Copy( ::rux::int32 size , void* data , ::rux::byte& reinited , GLenum target , GLenum usage , size_t ___rux__thread_index1986 )
			{
				if( _gl_context->_is_support_vbo )
				{
					if( size > 0 )
					{
						Alloc( size , reinited , target , usage , ___rux__thread_index1986 );
						if( _size > 0 )
						{
							rux::gui::engine::OpenGL::glBindBuffer( _target , _buffer_id , _gl_context , __FILE__ , __LINE__ , ___rux__thread_index1986 );
							if( _gl_context->_glMapBuffer )
							{
								GLvoid* data_ptr = ::rux::gui::engine::OpenGL::glMapBuffer( _target , GL_WRITE_ONLY , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								if( data_ptr )
								{
									memcpy( data_ptr , data , size );
									rux::gui::engine::OpenGL::glUnmapBuffer( _target , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								}
							}
							else
								rux::gui::engine::OpenGL::glBufferSubData( _target , 0 , size , data, _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glBindBuffer( _target , 0 , _gl_context , __FILE__ , __LINE__ , ___rux__thread_index1986 );
						}
					}
				}
			};
			void GLNamedBufferObject::Bind( size_t ___rux__thread_index1986 )
			{
				rux::gui::engine::OpenGL::glBindBuffer( _target , _buffer_id , _gl_context , __FILE__ , __LINE__ , ___rux__thread_index1986 );
			};
			void GLNamedBufferObject::UnBind( size_t ___rux__thread_index1986 )
			{
				rux::gui::engine::OpenGL::glBindBuffer( _target , 0 , _gl_context , __FILE__ , __LINE__ , ___rux__thread_index1986 );
			};
			void GLNamedBufferObject::Alloc( ::rux::int32 size , ::rux::byte& reinited , GLenum target , GLenum usage , size_t ___rux__thread_index1986 )
			{
				if( _gl_context->_is_support_vbo )
				{
					if( size > 0 )
					{
						if( _size > 0 && ( _size < size || _target != target || _usage != usage ) )
						{
							rux::gui::engine::OpenGL::glDeleteBuffers( 1 , &_buffer_id , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							_size = 0;
						}
						if( _size == 0 )
						{
							reinited = 1;
							_target = target;
							_usage = usage;
							_size = size;
							rux::gui::engine::OpenGL::glBindBuffer( _target , 0 , _gl_context , __FILE__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glGenBuffers( 1 , &_buffer_id , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glBindBuffer( _target , _buffer_id , _gl_context , __FILE__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glBufferData( _target , _size , NULL, _usage , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glBindBuffer( _target , 0 , _gl_context , __FILE__ , __LINE__ , ___rux__thread_index1986 );
						}
					}
				}
			};
		};
	};
};
