#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_opengl_functions.h>
#include <xan_memorymanager_defines.h>
#include <xan_gui_defines.h>
#include <xan_io_defines.h>
#include <xan_render_thread.h>
#include <xan_render_context.h>
#include <xan_log.h>
namespace rux
{
	namespace gui
	{
		namespace engine
		{			
#define opengl_render_context_function_body( FUNC ) if( gl_context->_##FUNC )\
				gl_context->_##FUNC
#define opengl_function_body( FUNC ) if( _##FUNC )\
				_##FUNC
#define opengl_render_context_function_body_with_return( FUNC ) if( gl_context->_##FUNC )\
				result = gl_context->_##FUNC
#define opengl_function_body_with_return( FUNC ) if( _##FUNC )\
				result = _##FUNC
			dll_internal rux_volatile _gl_library_inited = 0;
#ifdef __WINDOWS__
			dll_internal HMODULE _gl_library = NULL;
#else
			dll_internal void* _gl_library = NULL;
#endif
			dll_internal rux::uint8 _log_enable = 0;
			dll_internal rux::byte _opengl_one_thread_swap_buffers = 0;
			dll_internal rux::byte _winapi_trace = 0;
			dll_internal ::rux::byte _check_gl_error = 0;
			dll_internal ::rux::byte _check_opengl_functions_timeout = 0;
			dll_internal ::rux::byte _check_support_framebuffer = 0;
			dll_internal ::rux::byte _check_support_arb_texture_rectangle = 1;
			dll_internal ::rux::byte _check_support_old_yv12_to_rgb_shader = 1;
			dll_internal ::rux::byte _check_support_old_yv12_to_rgb_shader2 = 1;
			dll_internal ::rux::byte _check_support_old_yuyv_to_rgb_shader = 1;
			dll_internal ::rux::byte _check_support_yv12_to_rgb_shader = 1;
			dll_internal ::rux::byte _check_support_yuyv_to_rgb_shader = 1;
			dll_internal ::rux::byte _check_support_fxaa = 1;
			dll_internal ::rux::byte _vsync = 0;
			dll_internal ::rux::byte _use_gl_finish = 0;
			dll_internal ::rux::byte _check_support_vbo = 1;
			implement_global_variable( char , _error_filename , 1024 );
			implement_global_variable( char , _log_filename , 1024 );
			declare_opengl_function_object_in_cpp_macros( glColor4f );
			declare_opengl_function_object_in_cpp_macros( glRotatef );
			declare_opengl_function_object_in_cpp_macros( glGetError );
			declare_opengl_function_object_in_cpp_macros( glDisableClientState );
			declare_opengl_function_object_in_cpp_macros( glDrawArrays );
			declare_opengl_function_object_in_cpp_macros( glVertexPointer );
			declare_opengl_function_object_in_cpp_macros( glPixelStorei );
			declare_opengl_function_object_in_cpp_macros( glEnable );
			declare_opengl_function_object_in_cpp_macros( glBindTexture );
			declare_opengl_function_object_in_cpp_macros( glDisable );
			declare_opengl_function_object_in_cpp_macros( glStencilFunc );
			declare_opengl_function_object_in_cpp_macros( glColorMask );
			declare_opengl_function_object_in_cpp_macros( glStencilOp );
			declare_opengl_function_object_in_cpp_macros( glBlendFunc );
			declare_opengl_function_object_in_cpp_macros( glGetFloatv );
			declare_opengl_function_object_in_cpp_macros( glGetBooleanv );
			declare_opengl_function_object_in_cpp_macros( glTexParameteri );
			declare_opengl_function_object_in_cpp_macros( glTexEnvf );
			declare_opengl_function_object_in_cpp_macros( glTexImage2D );
			declare_opengl_function_object_in_cpp_macros( glTexImage1D );
			declare_opengl_function_object_in_cpp_macros( glLoadIdentity );
#ifdef __ANDROID__
			declare_opengl_function_object_in_cpp_macros( glOrthof );
#else
			declare_opengl_function_object_in_cpp_macros( glOrtho );
#endif
			declare_opengl_function_object_in_cpp_macros( glGenTextures );
			declare_opengl_function_object_in_cpp_macros( glDeleteTextures );
			declare_opengl_function_object_in_cpp_macros( glEnableClientState );
			declare_opengl_function_object_in_cpp_macros( glTexCoordPointer );
			declare_opengl_function_object_in_cpp_macros( glClearStencil );
			declare_opengl_function_object_in_cpp_macros( glColorPointer );
			declare_opengl_function_object_in_cpp_macros( glTexSubImage2D );
			declare_opengl_function_object_in_cpp_macros( glTexSubImage1D );
			declare_opengl_function_object_in_cpp_macros( glPushMatrix );
			declare_opengl_function_object_in_cpp_macros( glMatrixMode );
			declare_opengl_function_object_in_cpp_macros( glDepthFunc );
			declare_opengl_function_object_in_cpp_macros( glClearDepthf );
			declare_opengl_function_object_in_cpp_macros( glClearColor );
			declare_opengl_function_object_in_cpp_macros( glShadeModel );
			declare_opengl_function_object_in_cpp_macros( glViewport );
			declare_opengl_function_object_in_cpp_macros( glGetString );
			declare_opengl_function_object_in_cpp_macros( glClear );
			declare_opengl_function_object_in_cpp_macros( glScalef );
			declare_opengl_function_object_in_cpp_macros( glHint );
			declare_opengl_function_object_in_cpp_macros( glFlush );
			declare_opengl_function_object_in_cpp_macros( glFinish );
			declare_opengl_function_object_in_cpp_macros( glPopMatrix );
			declare_opengl_function_object_in_cpp_macros( glEnd );
			declare_opengl_function_object_in_cpp_macros( glVertex3f );
			declare_opengl_function_object_in_cpp_macros( glTexCoord2f );
			declare_opengl_function_object_in_cpp_macros( glBegin );
			declare_opengl_function_object_in_cpp_macros( glGetIntegerv );
			declare_opengl_function_object_in_cpp_macros( glRasterPos3f );
			declare_opengl_function_object_in_cpp_macros( glPixelZoom );
			declare_opengl_function_object_in_cpp_macros( glDrawPixels );
#ifdef __WINDOWS__
			declare_opengl_function_object_in_cpp_macros( wglShareLists );
			declare_opengl_function_object_in_cpp_macros( wglGetProcAddress );
			declare_opengl_function_object_in_cpp_macros( wglGetCurrentDC );
			declare_opengl_function_object_in_cpp_macros( wglCreateContext );
			declare_opengl_function_object_in_cpp_macros( wglDeleteContext );
			declare_opengl_function_object_in_cpp_macros( wglMakeCurrent );
			declare_opengl_function_object_in_cpp_macros( wglGetCurrentContext );
#else
#ifdef __ANDROID__
			declare_opengl_function_object_in_cpp_macros( eglGetProcAddress );
#else
			declare_opengl_function_object_in_cpp_macros( glXQueryExtension );
			declare_opengl_function_object_in_cpp_macros( glXGetConfig );
			declare_opengl_function_object_in_cpp_macros( glXGetProcAddress );
			declare_opengl_function_object_in_cpp_macros( glXCreateContext );
			declare_opengl_function_object_in_cpp_macros( glXDestroyContext );
			declare_opengl_function_object_in_cpp_macros( glXSwapBuffers );
			declare_opengl_function_object_in_cpp_macros( glXGetCurrentContext );
			declare_opengl_function_object_in_cpp_macros( glXMakeCurrent );
#endif
#endif			
			void OpenGL::glDeleteBuffers( GLsizei n , const GLuint* buffers , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				for( size_t index0 = 0 ; index0 < n ; index0++ )
				{
					if( gl_context->_gl_bind_buffer_gl_array_buffer == buffers[ index0 ] )
						gl_context->_gl_bind_buffer_gl_array_buffer = 0;
					if( gl_context->_gl_bind_buffer_gl_pixel_unpack_buffer == buffers[ index0 ] )
						gl_context->_gl_bind_buffer_gl_pixel_unpack_buffer = 0;
				}
				opengl_render_context_function_body( glDeleteBuffers )( n , buffers );				
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%d , %p" , n , buffers );
			};
			void OpenGL::glActiveTexture( GLenum texture , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				if( gl_context->_gl_active_texture != texture )
				{
					opengl_render_context_function_body( glActiveTexture )( texture );
					::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u" , texture );
					gl_context->_gl_active_texture = texture;
				}
			};
			void OpenGL::glUniform1i( GLint location , GLint v0 , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_render_context_function_body( glUniform1i )( location , v0 );
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%d , %d" , location , v0 );
			};			
			void OpenGL::glGenBuffers( GLsizei n , GLuint* buffers , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_render_context_function_body( glGenBuffers )( n , buffers );	
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%d , %p" , n , buffers );
			};
			void OpenGL::glEnableVertexAttribArray( GLuint index , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_render_context_function_body( glEnableVertexAttribArray )( index );
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u" , index );
			};
			void OpenGL::glBindBuffer( GLenum target , GLuint buffer , ::rux::gui::engine::GLContext* gl_context , const char* cpp_file , int32 cpp_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , cpp_file , __FUNCTION__ , cpp_line , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				::rux::byte executed = 0;
				if( target == GL_ARRAY_BUFFER )
				{
					if( gl_context->_gl_bind_buffer_gl_array_buffer != buffer )
					{
						if( gl_context->_gl_map_buffer_mapped )
							rux::gui::engine::OpenGL::glUnmapBuffer( GL_ARRAY_BUFFER , gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						opengl_render_context_function_body( glBindBuffer )( target , buffer );
						executed = 1;
						gl_context->_gl_bind_buffer_gl_array_buffer = buffer;
					}
				}
				else if( target == GL_PIXEL_UNPACK_BUFFER )
				{
					if( gl_context->_gl_bind_buffer_gl_pixel_unpack_buffer != buffer )
					{
						if( gl_context->_gl_map_buffer_mapped )
							rux::gui::engine::OpenGL::glUnmapBuffer( GL_ARRAY_BUFFER , gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						opengl_render_context_function_body( glBindBuffer )( target , buffer );
						executed = 1;
						gl_context->_gl_bind_buffer_gl_pixel_unpack_buffer = buffer;
					}
				}
				else
				{
					opengl_render_context_function_body( glBindBuffer )( target , buffer );
					executed = 1;
				}
				if( executed && ::rux::gui::engine::_check_gl_error )
				{
					GLenum error = rux::gui::engine::OpenGL::glGetError();
					if( error != GL_NO_ERROR )
					{
						declare_stack_variable( char , error_string , 256 );
						if( cpp_file && cpp_line > 0 )
							::rux::safe_sprintf( error_string , 256 , "file %s , line %d , %s , target = %x , OpenGL error=%u\n" , cpp_file , cpp_line , __FUNCTION__ , target , error );
						else
							::rux::safe_sprintf( error_string , 256 , "%s , target = %x , OpenGL error=%u\n" , __FUNCTION__ , target , error );
						rux::gui::engine::OpenGL::write_error( error_string );
						printf( "%s" , error_string );
					}
				}
			};
			void OpenGL::glVertexAttribPointer( GLuint index , GLint size , GLenum type , GLboolean normalized , GLsizei stride , const GLvoid* pointer , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_render_context_function_body( glVertexAttribPointer )( index , size , type , normalized , stride , pointer );
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u , %d , %u , %u , %d , %p" , index , size , type , (::rux::uint32)normalized , stride , pointer );
			};
			void OpenGL::glBufferData( GLenum target , GLsizeiptr size , const GLvoid* data , GLenum usage , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_render_context_function_body( glBufferData )( target , size , data , usage );
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u , " I64d " , %p , %u" , target , (::rux::int64)size , data , usage );
			};
			void OpenGL::glBufferSubData( GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid *data , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_render_context_function_body( glBufferSubData )( target, offset, size, data );
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u , " I64d " , " I64d " , %p" , target, (::rux::int64)offset, (::rux::int64)size, data );
			};
			GLuint OpenGL::glCreateProgram( ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				GLuint result = 0;
				opengl_render_context_function_body_with_return( glCreateProgram )();
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , NULL );
				return result;
			};
			GLuint OpenGL::glCreateShader( GLenum shaderType , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				GLuint result = 0;
				opengl_render_context_function_body_with_return( glCreateShader )( shaderType );
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u" , shaderType );
				return result;
			};
			void OpenGL::glShaderSource( GLuint shaderObj , GLsizei count , const GLchar** string , const GLint* length , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_render_context_function_body( glShaderSource )( shaderObj , count , string , length );
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u , %d , %p , %p" , shaderObj , count , string , length );
			};
			void OpenGL::glCompileShader( GLuint shaderObj , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_render_context_function_body( glCompileShader )( shaderObj );
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u" , shaderObj );
			};
			void OpenGL::glDeleteShader( GLuint shaderObj , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_render_context_function_body( glDeleteShader )( shaderObj );
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u" , shaderObj );
			};
			void OpenGL::glDetachShader( GLuint containerObj, GLuint attachedObj , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_render_context_function_body( glDetachShader )( containerObj , attachedObj );
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u , %u" , containerObj , attachedObj );
			};
			void OpenGL::glAttachShader( GLuint containerObj , GLuint obj , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_render_context_function_body( glAttachShader )( containerObj , obj );
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u , %u" , containerObj , obj );
			};
			void OpenGL::glBindFragDataLocation( GLuint program , GLuint colorNumber , const char * name , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_render_context_function_body( glBindFragDataLocation )( program , colorNumber , name );
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u" , program );
			};
			void OpenGL::glLinkProgram( GLuint programObj , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_render_context_function_body( glLinkProgram )( programObj );
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u" , programObj );
			};
			GLint OpenGL::glGetAttribLocation( GLuint programObj , const GLchar* name , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				GLint result = -1;
				opengl_render_context_function_body_with_return( glGetAttribLocation )( programObj , name );
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u , %s" , programObj , name );
				return result;
			};
			GLint OpenGL::glGetUniformLocation( GLuint programObj , const GLchar* name , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				GLint result = -1;
				opengl_render_context_function_body_with_return( glGetUniformLocation )( programObj , name );
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u , %s" , programObj , name );
				return result;
			};			
			void OpenGL::glGetProgramiv( GLuint program , GLenum pname , GLint* params , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_render_context_function_body( glGetProgramiv )( program , pname , params );
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u" , program );
			};
			void OpenGL::glValidateProgram( GLuint programObj , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_render_context_function_body( glValidateProgram )( programObj );
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u" , programObj );
			};
			void OpenGL::glUseProgram( GLuint programObj , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_render_context_function_body( glUseProgram )( programObj );
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u" , programObj );
			};
			void OpenGL::glDisableVertexAttribArray( GLuint index , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_render_context_function_body( glDisableVertexAttribArray )( index );
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u" , index );
			};
			void OpenGL::glUniform4fv( GLint location , GLsizei count , const GLfloat *value , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_render_context_function_body( glUniform4fv )( location , count , value );
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%d , %d , %p" , location , count , value );
			};
			GLvoid* OpenGL::glMapBuffer( GLenum target , GLenum access , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				GLvoid* result = NULL;
				opengl_render_context_function_body_with_return( glMapBuffer )( target , access );
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u , %u" , target , access );
				if( result )
					gl_context->_gl_map_buffer_mapped = 1;
				return result;
			};
			GLboolean OpenGL::glUnmapBuffer( GLenum target , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				GLboolean result = 0;
				if( gl_context->_gl_map_buffer_mapped == 1 )
				{
					opengl_render_context_function_body_with_return( glUnmapBuffer )( target );
					::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u" , target );
					gl_context->_gl_map_buffer_mapped = 0;
				}
				return result;
			};
			void OpenGL::glUniform4f( GLint location , GLfloat v0 , GLfloat v1 , GLfloat v2 , GLfloat v3 , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_render_context_function_body( glUniform4f )( location , v0 , v1 , v2 , v3 );
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%d , %f , %f , %f , %f" , location , v0 , v1 , v2 , v3 );
			};
			void OpenGL::glUniform1fv( GLint location , GLsizei count , const GLfloat* value , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_render_context_function_body( glUniform1fv )( location , count , value );
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%d , %f" , location , value[ 0 ] );
			};
			void OpenGL::glUniform1f( GLint location , GLfloat v0 , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_render_context_function_body( glUniform1f )( location , v0 );
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%d , %f" , location , v0 );
			};
#ifdef __ANDROID__
#else
			void OpenGL::glGetObjectParameteriv( GLuint obj , GLenum pname , GLint *params , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_render_context_function_body( glGetObjectParameteriv )( obj , pname , params );
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u , %u , %p" , obj , pname , params );
			};
#endif
#ifdef __ANDROID__
#else
			void OpenGL::glGetInfoLog( GLuint obj , GLsizei maxLength , GLsizei *length , GLchar *infoLog , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_render_context_function_body( glGetInfoLog )( obj , maxLength , length , infoLog );
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u , %d , %p , %p" , obj , maxLength , length , infoLog );
			};
#endif
			void OpenGL::glStencilOpSeparate( GLenum face , GLenum sfail , GLenum dpfail , GLenum dppass , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_render_context_function_body( glStencilOpSeparate )( face , sfail , dpfail , dppass );
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u , %u , %u , %u" , face , sfail , dpfail , dppass );
			};
			void OpenGL::glActiveStencilFace( GLenum face , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_render_context_function_body( glActiveStencilFace )( face );
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u" , face );
			};			
#ifdef __WINDOWS__
			int WINAPI OpenGL::wglGetSwapInterval( ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				int result = 0;
				opengl_render_context_function_body_with_return( wglGetSwapInterval )();
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , NULL );
				return result;
			};
			BOOL WINAPI OpenGL::wglSwapInterval( int interval , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				BOOL result = FALSE;
				opengl_render_context_function_body_with_return( wglSwapInterval )( interval );
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%d" , interval );
				return result;
			};
			BOOL OpenGL::wglChoosePixelFormat( HDC hdc , const int *piAttribIList , const FLOAT *pfAttribFList , UINT nMaxFormats , int *piFormats , UINT *nNumFormats , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				BOOL result = FALSE;
				opengl_render_context_function_body_with_return( wglChoosePixelFormat )( hdc , piAttribIList , pfAttribFList , nMaxFormats , piFormats , nNumFormats );
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , NULL );
				return result;
			};
#endif
			void OpenGL::glBindFramebuffer( GLenum target , GLuint framebuffer , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_render_context_function_body( glBindFramebuffer )( target , framebuffer );
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u , %u" , target , framebuffer );
			};
			void OpenGL::glDeleteFramebuffers( GLsizei n , const GLuint *framebuffers , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_render_context_function_body( glDeleteFramebuffers )( n , framebuffers );
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%d , %p" , n , framebuffers );
			};
			void OpenGL::glGenFramebuffers( GLsizei n , GLuint *framebuffers , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_render_context_function_body( glGenFramebuffers )( n , framebuffers );
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%d , %p" , n , framebuffers );
			};
			void OpenGL::glGenerateMipmap( GLenum target , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_render_context_function_body( glGenerateMipmap )( target );
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u" , target );
			};
			GLenum OpenGL::glCheckFramebufferStatus( GLenum target , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				GLenum result = 0;
				opengl_render_context_function_body_with_return( glCheckFramebufferStatus )( target );
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u" , target );
				return result;
			};
			void OpenGL::glFramebufferTexture2D( GLenum target , GLenum attachment , GLenum textarget , GLuint texture , GLint level , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_render_context_function_body( glFramebufferTexture2D )( target , attachment , textarget , texture , level );
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u , %u , %u , %u , %d" , target , attachment , textarget , texture , level );
			};
			void OpenGL::glBindRenderbuffer( GLenum target , GLuint renderbuffer , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_render_context_function_body( glBindRenderbuffer )( target , renderbuffer );
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u , %u" , target , renderbuffer );
			};
			void OpenGL::glDeleteRenderbuffers( GLsizei n , const GLuint *renderbuffers , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_render_context_function_body( glDeleteRenderbuffers )( n , renderbuffers );
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%d , %p" , n , renderbuffers );
			};
			void OpenGL::glGenRenderbuffers( GLsizei n , GLuint *renderbuffers , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_render_context_function_body( glGenRenderbuffers )( n , renderbuffers );
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%d , %p" , n , renderbuffers );
			};
			void OpenGL::glRenderbufferStorage( GLenum target , GLenum internalformat , GLsizei width , GLsizei height , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_render_context_function_body( glRenderbufferStorage )( target , internalformat , width , height );
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u , %u , %d , %d" , target , internalformat , width , height );
			};
			void OpenGL::glGetInternalformativ( GLenum target , GLenum internalformat , GLenum pname , GLsizei bufSize , GLint* params , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_render_context_function_body( glGetInternalformativ )( target , internalformat , pname , bufSize , params );
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u , %u , %u , %d , %p" , target , internalformat , pname , bufSize , params );
			};
			void OpenGL::glFramebufferRenderbuffer( GLenum target , GLenum attachment , GLenum renderbuffertarget , GLuint renderbuffer , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_render_context_function_body( glFramebufferRenderbuffer )( target , attachment , renderbuffertarget , renderbuffer );	 
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u , %u , %u , %u" , target , attachment , renderbuffertarget , renderbuffer );
			};
#ifdef __WINDOWS__
			implement_opengl_function_with_return_in_header_macros( wglShareLists , BOOL )( HGLRC hglrc1 , HGLRC  hglrc2 )
			{
				BOOL result = FALSE;
				opengl_function_body_with_return( wglShareLists )( hglrc1 , hglrc2 );
				return result;
			};
			implement_opengl_function_with_return_in_header_macros( wglGetProcAddress , PROC )( LPCSTR param )
			{
				PROC result = NULL;
				opengl_function_body_with_return( wglGetProcAddress )( param );
				return result;
			};
			implement_opengl_function_with_return_in_header_macros( wglGetCurrentDC , HDC )( VOID )
			{
				HDC result = 0;
				opengl_function_body_with_return( wglGetCurrentDC )();
				return result;
			};
			implement_opengl_function_with_return_in_header_macros( wglCreateContext , HGLRC )( HDC param )
			{
				HGLRC result = 0;
				opengl_function_body_with_return( wglCreateContext )( param );
				return result;
			};
			implement_opengl_function_with_return_in_header_macros( wglDeleteContext , BOOL )( HGLRC param )
			{
				BOOL result = 0;
				opengl_function_body_with_return( wglDeleteContext )( param );
				return result;
			};
			implement_opengl_function_with_return_in_header_macros( wglMakeCurrent , BOOL )( HDC param0 , HGLRC param1 )
			{
				BOOL result = 0;
				opengl_function_body_with_return( wglMakeCurrent )( param0 , param1 );
				return result;
			};
			implement_opengl_function_with_return_in_header_macros( wglGetCurrentContext , HGLRC )(VOID)
			{
				HGLRC result = 0;
				opengl_function_body_with_return( wglGetCurrentContext )();
				return result;
			};
#else
#ifdef __ANDROID__
			implement_opengl_function_with_return_in_header_macros( eglGetProcAddress , __eglMustCastToProperFunctionPointerType )(const char *  procName)
			{
				__eglMustCastToProperFunctionPointerType result = 0;
				opengl_function_body_with_return( eglGetProcAddress )( procName);
				return result;
			};
#else
			implement_opengl_function_with_return_in_header_macros( glXQueryExtension , Bool )(Display *  dpy,  int *  errorBase,  int *  eventBase)
			{
				Bool result = 0;
				opengl_function_body_with_return( glXQueryExtension )( dpy,  errorBase,  eventBase);
				return result;
			};
			implement_opengl_function_with_return_in_header_macros( glXGetConfig , int )(Display *  dpy,  XVisualInfo *  vis,  int  attrib,  int *  value)
			{
				int result = 0;
				opengl_function_body_with_return( glXGetConfig )( dpy, vis, attrib, value);
				return result;
			};
			implement_opengl_function_with_return_in_header_macros( glXGetProcAddress , PROC )(const GLubyte *  procName)
			{
				PROC result = 0;
				opengl_function_body_with_return( glXGetProcAddress )( procName);
				return result;
			};
			implement_opengl_function_with_return_in_header_macros( glXCreateContext , GLXContext )(Display *  dpy,  XVisualInfo *  vis,  GLXContext  shareList,  Bool  direct)
			{
				GLXContext result = 0;
				opengl_function_body_with_return( glXCreateContext )( dpy, vis, shareList, direct);
				return result;
			};
			implement_opengl_function_with_return_in_header_macros( glXDestroyContext , void )(Display *  dpy,  GLXContext  ctx , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_function_body( glXDestroyContext )( dpy, ctx );
				//CHECK_GL_ERROR();
			};
			implement_opengl_function_with_return_in_header_macros( glXSwapBuffers , void )(Display *  dpy,  GLXDrawable  drawable , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_function_body( glXSwapBuffers )( dpy , drawable );
				//CHECK_GL_ERROR();
			};
			implement_opengl_function_with_return_in_header_macros( glXGetCurrentContext , GLXContext )()
			{
				GLXContext result = 0;
				opengl_function_body_with_return( glXGetCurrentContext )();
				return result;
			};
			implement_opengl_function_with_return_in_header_macros( glXMakeCurrent , Bool )(Display *  dpy,  GLXDrawable  drawable,  GLXContext  ctx)
			{
				Bool result = 0;
				opengl_function_body_with_return( glXMakeCurrent )( dpy,  drawable, ctx);
				return result;
			};
#endif
#endif
			implement_opengl_function_with_return_in_header_macros( glGetError , GLenum )( void )
			{
				GLenum result = 0;
				opengl_function_body_with_return( glGetError )();
				return result;
			};
			implement_opengl_function_with_return_in_header_macros( glGetString , const GLubyte * )(GLenum name)
			{
				const GLubyte* result = 0;
				opengl_function_body_with_return( glGetString )( name);
				return result;
			};
			implement_opengl_function_in_header_macros( glColor4f )(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha , ::rux::gui::engine::GLContext* , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_function_body( glColor4f )( red , green , blue , alpha );
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%f , %f , %f , %f" , red, green, blue, alpha );
			};
			implement_opengl_function_in_header_macros( glRotatef )( GLfloat angle , GLfloat x , GLfloat y , GLfloat z , ::rux::gui::engine::GLContext* , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_function_body( glRotatef )( angle , x , y , z );
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%f , %f , %f , %f" , angle , x , y , z );
			};
			implement_opengl_function_in_header_macros( glDisableClientState )(GLenum array , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				if( array == GL_COLOR_ARRAY )
				{
					if( gl_context->_gl_ensable_client_state_gl_color_array == 1 )
					{
						opengl_function_body( glDisableClientState )( array );
						::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u" , array );
						gl_context->_gl_ensable_client_state_gl_color_array = 0;
					}
				}
				else if( array == GL_TEXTURE_COORD_ARRAY )
				{
					if( gl_context->_gl_ensable_client_state_gl_texture_coord_array == 1 )
					{
						opengl_function_body( glDisableClientState )( array );
						::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u" , array );
						gl_context->_gl_ensable_client_state_gl_texture_coord_array = 0;
					}
				}
				else if( array == GL_VERTEX_ARRAY )
				{
					if( gl_context->_gl_ensable_client_state_gl_vertex_array == 1 )
					{
						opengl_function_body( glDisableClientState )( array );
						::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u" , array );
						gl_context->_gl_ensable_client_state_gl_vertex_array = 0;
					}
				}
				else
				{
					opengl_function_body( glDisableClientState )( array );
					::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u" , array );
				}
			};
			implement_opengl_function_in_header_macros( glMultiDrawArrays )( GLenum mode ,  GLint* first ,  GLsizei* count , GLsizei primcount , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_render_context_function_body( glMultiDrawArrays )( mode , first , count , primcount );
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u , %p , %p , %d" , mode ,  first ,  count , primcount );
			};
			implement_opengl_function_in_header_macros( glDrawArrays )(GLenum mode, GLint first, GLsizei count , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_function_body( glDrawArrays )(mode, first, count);
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u , %d , %d" , mode, first, count );
			};
			implement_opengl_function_in_header_macros( glVertexPointer )(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				if( pointer == NULL
					|| gl_context->_gl_vertex_pointer_size != size 
					|| gl_context->_gl_vertex_pointer_type != type
					|| gl_context->_gl_vertex_pointer_stride != stride
					|| gl_context->_gl_vertex_pointer_pointer != pointer )
				{
					opengl_function_body( glVertexPointer )(size,  type,  stride, pointer);
					::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%d , %u , %d , %p" , size, type, stride, pointer );
					gl_context->_gl_vertex_pointer_size = size; 
					gl_context->_gl_vertex_pointer_type = type;
					gl_context->_gl_vertex_pointer_stride = stride;
					gl_context->_gl_vertex_pointer_pointer = (void*)pointer;					
				}
			};
			implement_opengl_function_in_header_macros( glPixelStorei )(GLenum pname, GLint param , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				if( pname == GL_PACK_ALIGNMENT )
				{
					if( gl_context->_gl_pixel_storei_gl_pack_alignment != param )
					{
						opengl_function_body( glPixelStorei )(pname,  param);
						::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u , %d" , pname, param );
						gl_context->_gl_pixel_storei_gl_pack_alignment = param;
					}
				}
				else if( pname == GL_UNPACK_ALIGNMENT )
				{
					if( gl_context->_gl_pixel_storei_gl_unpack_alignment != param )
					{
						opengl_function_body( glPixelStorei )(pname,  param);
						::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u , %d" , pname, param );
						gl_context->_gl_pixel_storei_gl_unpack_alignment = param;
					}
				}
			};
			implement_opengl_function_in_header_macros( glEnable )(GLenum cap , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				//XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				if( cap == GL_CULL_FACE )
				{
					if( gl_context->_gl_cull_face == GL_FALSE )
					{
						opengl_function_body( glEnable )( cap);
						::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u" , cap );
						gl_context->_gl_cull_face = GL_TRUE;
					}
				}
				else if( cap == GL_STENCIL_TEST )
				{
					if( gl_context->_gl_stencil_test == GL_FALSE )
					{
						opengl_function_body( glEnable )( cap);
						::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u" , cap );
						gl_context->_gl_stencil_test = GL_TRUE;
					}
				}
				else if( cap == GL_DEPTH_TEST )
				{
					if( gl_context->_gl_depth_test == GL_FALSE )
					{
						opengl_function_body( glEnable )( cap);
						::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u" , cap );
						gl_context->_gl_depth_test = GL_TRUE;
					}
				}
				else if( cap == GL_ALPHA_TEST )
				{
					if( gl_context->_gl_alpha_test == GL_FALSE )
					{
						opengl_function_body( glEnable )( cap);
						::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u" , cap );
						gl_context->_gl_alpha_test = GL_TRUE;
					}
				}
				else if( cap == GL_BLEND )
				{
					if( gl_context->_gl_blend == GL_FALSE )
					{
						opengl_function_body( glEnable )( cap);
						::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u" , cap );
						gl_context->_gl_blend = GL_TRUE;
					}
				}
				else if( cap == GL_TEXTURE_RECTANGLE )
				{
					if( gl_context->_gl_texture_rectangle == GL_FALSE )
					{
						opengl_function_body( glEnable )( cap);
						::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u" , cap );
						gl_context->_gl_texture_rectangle = GL_TRUE;
					}
				}
				else if( cap == GL_TEXTURE_2D )
				{
					if( gl_context->_gl_texture_2d == GL_FALSE )
					{
						opengl_function_body( glEnable )( cap);
						::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u" , cap );
						gl_context->_gl_texture_2d = GL_TRUE;
					}
				}
				else
				{
					opengl_function_body( glEnable )( cap);
					::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u" , cap );
				}
			};
			implement_opengl_function_in_header_macros( glBindTexture )(GLenum target, GLuint texture , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_function_body( glBindTexture )( target, texture);
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u , %u" , target, texture );
			};
			implement_opengl_function_in_header_macros( glDisable )(GLenum cap , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				//XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				if( cap == GL_CULL_FACE )
				{
					if( gl_context->_gl_cull_face == GL_TRUE )
					{
						opengl_function_body( glDisable )(cap);
						::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u" , cap );
						gl_context->_gl_cull_face = GL_FALSE;
					}
				}
				else if( cap == GL_STENCIL_TEST )
				{
					if( gl_context->_gl_stencil_test == GL_TRUE )
					{
						opengl_function_body( glDisable )(cap);
						::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u" , cap );
						gl_context->_gl_stencil_test = GL_FALSE;
					}
				}
				else if( cap == GL_DEPTH_TEST )
				{
					if( gl_context->_gl_depth_test == GL_TRUE )
					{
						opengl_function_body( glDisable )(cap);
						::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u" , cap );
						gl_context->_gl_depth_test = GL_FALSE;
					}
				}
				else if( cap == GL_ALPHA_TEST )
				{
					if( gl_context->_gl_alpha_test == GL_TRUE )
					{
						opengl_function_body( glDisable )(cap);
						::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u" , cap );
						gl_context->_gl_alpha_test = GL_FALSE;
					}
				}
				else if( cap == GL_BLEND )
				{
					if( gl_context->_gl_blend == GL_TRUE )
					{
						opengl_function_body( glDisable )(cap);
						::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u" , cap );
						gl_context->_gl_blend = GL_FALSE;
					}
				}
				else if( cap == GL_TEXTURE_RECTANGLE )
				{
					if( gl_context->_gl_texture_rectangle == GL_TRUE )
					{
						opengl_function_body( glDisable )(cap);
						::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u" , cap );
						gl_context->_gl_texture_rectangle = GL_FALSE;
					}
				}
				else if( cap == GL_TEXTURE_2D )
				{
					if( gl_context->_gl_texture_2d == GL_TRUE )
					{
						opengl_function_body( glDisable )(cap);
						::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u" , cap );
						gl_context->_gl_texture_2d = GL_FALSE;
					}
				}
				else
				{
					opengl_function_body( glDisable )(cap);
					::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u" , cap );
				}
			};
			implement_opengl_function_in_header_macros( glStencilFunc )(GLenum func, GLint ref, GLuint mask , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				//XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_function_body( glStencilFunc )( func,  ref, mask);
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u , %d , %u" , func, ref, mask );
			};
			implement_opengl_function_in_header_macros( glColorMask )(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				//XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );

				/*GLboolean gl_color_mask[ 4 ] = { 0 };
				::rux::gui::engine::OpenGL::glGetBooleanv( GL_COLOR_WRITEMASK , gl_color_mask );
				if( memcmp( gl_color_mask , gl_context->_gl_color_mask , 4 * sizeof( GLboolean ) ) != 0 )
				{
					printf( "%s" , "NOOOOOO!!!\n" );
				}*/

				if( gl_context->_gl_color_mask[ 0 ] != red
					|| gl_context->_gl_color_mask[ 1 ] != green
					|| gl_context->_gl_color_mask[ 2 ] != blue
					|| gl_context->_gl_color_mask[ 3 ] != alpha )
				{
					opengl_function_body( glColorMask )( red,  green,  blue, alpha);
					::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u , %u , %u , %u" , (::rux::uint32)red, (::rux::uint32)green, (::rux::uint32)blue, (::rux::uint32)alpha );
					gl_context->_gl_color_mask[ 0 ] = red;
					gl_context->_gl_color_mask[ 1 ] = green;
					gl_context->_gl_color_mask[ 2 ] = blue;
					gl_context->_gl_color_mask[ 3 ] = alpha;
				}
			};
			implement_opengl_function_in_header_macros( glStencilOp )(GLenum fail, GLenum zfail, GLenum zpass , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				//XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_function_body( glStencilOp )( fail,  zfail,  zpass);
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u , %u , %u" , fail, zfail, zpass );
			};
			implement_opengl_function_in_header_macros( glBlendFunc )(GLenum sfactor, GLenum dfactor , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				//XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				if( gl_context->_gl_blend_func_sfactor != sfactor 
					|| gl_context->_gl_blend_func_dfactor != dfactor )
				{
					opengl_function_body( glBlendFunc )( sfactor, dfactor);
					::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u , %u" , sfactor, dfactor );
					gl_context->_gl_blend_func_sfactor = sfactor;
					gl_context->_gl_blend_func_dfactor = dfactor;
				}
			};
			implement_opengl_function_in_header_macros( glGetBooleanv )(GLenum pname, GLboolean *params , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_function_body( glGetBooleanv )( pname, params);
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u , %p" , pname, params );
			};
			implement_opengl_function_in_header_macros( glGetFloatv )(GLenum pname, GLfloat *params , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_function_body( glGetFloatv )( pname, params);
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u , %p" , pname, params );
			};
			implement_opengl_function_in_header_macros( glTexParameteri )(GLenum target, GLenum pname, GLint param , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_function_body( glTexParameteri )( target,  pname,  param);
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u , %u , %d" , target, pname, param );
			};
			implement_opengl_function_in_header_macros( glTexEnvf )(GLenum target, GLenum pname, GLfloat param , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_function_body( glTexEnvf )( target,  pname,  param);
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u , %u , %f" , target, pname, param );
			};
			implement_opengl_function_in_header_macros( glTexImage1D )(	GLenum target , GLint level , GLint internalFormat , GLsizei width , GLint border , GLenum format , GLenum type , const GLvoid* data , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_function_body( glTexImage1D )( target,  level,  internalFormat,  width, border , format , type , data );
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u , %d , %d , %d , %d , %u , %u , %p" , target, level, internalFormat, width, border , format , type , data );
			};
			implement_opengl_function_in_header_macros( glTexImage2D )(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_function_body( glTexImage2D )( target,  level,  internalformat,  width,  height,  border,  format,  type, pixels);
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u , %d , %d , %d , %d , %d , %u , %u , %p" , target, level, internalformat, width, height, border, format, type, pixels );
			};			
			implement_opengl_function_in_header_macros( glLoadIdentity )( const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_function_body( glLoadIdentity )();
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , NULL );
			};
#ifdef __ANDROID__
			implement_opengl_function_in_header_macros( glOrthof )(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_function_body( glOrthof )( left, right, bottom, top, zNear, zFar);
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%f , %f , %f , %f , %f , %f" , left, right, bottom, top, zNear, zFar );
			};
#else
			implement_opengl_function_in_header_macros( glOrtho )(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_function_body( glOrtho )( left, right, bottom, top, zNear, zFar);
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%f , %f , %f , %f , %f , %f" , left, right, bottom, top, zNear, zFar );
			};
#endif
			implement_opengl_function_in_header_macros( glGenTextures )(GLsizei n, GLuint *textures , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_function_body( glGenTextures )( n, textures);
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%d , %p" , n, textures );
			};
			implement_opengl_function_in_header_macros( glDeleteTextures )(GLsizei n, const GLuint *textures , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_function_body( glDeleteTextures )( n, textures);
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%d , %p" , n, textures );
			};
			implement_opengl_function_in_header_macros( glEnableClientState )(GLenum array , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				if( array == GL_COLOR_ARRAY )
				{
					if( gl_context->_gl_ensable_client_state_gl_color_array == 0 )
					{
						opengl_function_body( glEnableClientState )( array );
						::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u" , array );
						gl_context->_gl_ensable_client_state_gl_color_array = 1;
					}
				}
				else if( array == GL_TEXTURE_COORD_ARRAY )
				{
					if( gl_context->_gl_ensable_client_state_gl_texture_coord_array == 0 )
					{
						opengl_function_body( glEnableClientState )( array );
						::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u" , array );
						gl_context->_gl_ensable_client_state_gl_texture_coord_array = 1;
					}
				}
				else if( array == GL_VERTEX_ARRAY )
				{
					if( gl_context->_gl_ensable_client_state_gl_vertex_array == 0 )
					{
						opengl_function_body( glEnableClientState )( array );
						::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u" , array );
						gl_context->_gl_ensable_client_state_gl_vertex_array = 1;
					}
				}
				else
				{
					opengl_function_body( glEnableClientState )( array );
					::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u" , array );
				}
			};
			implement_opengl_function_in_header_macros( glTexCoordPointer )(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				if( pointer == NULL 
					|| gl_context->_gl_tex_coord_pointer_size != size 
					|| gl_context->_gl_tex_coord_pointer_type != type
					|| gl_context->_gl_tex_coord_pointer_stride != stride
					|| gl_context->_gl_tex_coord_pointer_pointer != pointer )
				{
					opengl_function_body( glTexCoordPointer )( size, type, stride, pointer);
					::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%d , %u , %d , %p" , size , type, stride, pointer );
					gl_context->_gl_tex_coord_pointer_size = size;
					gl_context->_gl_tex_coord_pointer_type = type;
					gl_context->_gl_tex_coord_pointer_stride = stride;
					gl_context->_gl_tex_coord_pointer_pointer = (void*)pointer;
				}
			};
			implement_opengl_function_in_header_macros( glClearStencil )(GLint s , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				//XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				if( gl_context->_gl_clear_stencil != s )
				{
					opengl_function_body( glClearStencil )( s);
					::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%d" , s );
					gl_context->_gl_clear_stencil = s;
				}
			};
			implement_opengl_function_in_header_macros( glColorPointer )(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				if( pointer == NULL 
					|| gl_context->_gl_color_pointer_size != size 
					|| gl_context->_gl_color_pointer_type != type
					|| gl_context->_gl_color_pointer_stride != stride
					|| gl_context->_gl_color_pointer_pointer != pointer )
				{
					opengl_function_body( glColorPointer )( size, type, stride, pointer);
					::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%d , %u , %d , %p" , size , type, stride, pointer );
					gl_context->_gl_color_pointer_size = size; 
					gl_context->_gl_color_pointer_type = type;
					gl_context->_gl_color_pointer_stride = stride;
					gl_context->_gl_color_pointer_pointer = (void*)pointer;
				}
			};
			implement_opengl_function_in_header_macros( glTexSubImage1D )( GLenum target , GLint level , GLint xoffset , GLsizei width , GLenum format , GLenum type , const GLvoid* data , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_function_body( glTexSubImage1D )( target, level, xoffset, width, format, type, data );
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u , %d , %d , %d , %u , %u , %p" , target , level, xoffset, width, format, type, data );
			};
			implement_opengl_function_in_header_macros( glTexSubImage2D )(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_function_body( glTexSubImage2D )( target, level, xoffset, yoffset, width, height, format, type, pixels);
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u , %d , %d , %d , %d , %d , %u , %u , %p" , target , level, xoffset, yoffset, width, height, format, type, pixels );
			};
			implement_opengl_function_in_header_macros( glPushMatrix )( const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_function_body( glPushMatrix )();
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , NULL );
			};
			implement_opengl_function_in_header_macros( glMatrixMode )(GLenum mode , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				if( gl_context->_gl_matrix_mode != mode )
				{
					opengl_function_body( glMatrixMode )( mode);
					::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u" , mode );
					gl_context->_gl_matrix_mode = mode;
				}
			};
			implement_opengl_function_in_header_macros( glDepthFunc )(GLenum func , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				if( gl_context->_gl_depth_func != func )
				{
					opengl_function_body( glDepthFunc )( func);
					::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u" , func );
					gl_context->_gl_depth_func = func;
				}
			};
			implement_opengl_function_in_header_macros( glClearDepthf )(GLclampf depth , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_function_body( glClearDepthf )( depth );
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%f" , depth );
			};
			implement_opengl_function_in_header_macros( glClearColor )(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				if( gl_context->_gl_clear_color[ 0 ] != red
					|| gl_context->_gl_clear_color[ 1 ] != green
					|| gl_context->_gl_clear_color[ 2 ] != blue
					|| gl_context->_gl_clear_color[ 3 ] != alpha )
				{
					opengl_function_body( glClearColor )( red, green, blue, alpha);
					::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%f , %f , %f , %f" , red, green, blue, alpha );
					gl_context->_gl_clear_color[ 0 ] = red;
					gl_context->_gl_clear_color[ 1 ] = green;
					gl_context->_gl_clear_color[ 2 ] = blue;
					gl_context->_gl_clear_color[ 3 ] = alpha;
				}
			};
			implement_opengl_function_in_header_macros( glShadeModel )(GLenum mode , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				if( gl_context->_gl_shade_model != mode )
				{
					opengl_function_body( glShadeModel )( mode);
					::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u" , mode );
					gl_context->_gl_shade_model = mode;
				}
			};
			implement_opengl_function_in_header_macros( glViewport )(GLint x, GLint y, GLsizei width, GLsizei height , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				if( gl_context->_gl_viewport_x != x
					|| gl_context->_gl_viewport_y != y
					|| gl_context->_gl_viewport_width != width
					|| gl_context->_gl_viewport_height != height )
				{
					opengl_function_body( glViewport )( x,  y,  width,  height);
					::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%d , %d , %d , %d" , x , y , width, height );
					gl_context->_gl_viewport_x = x;
					gl_context->_gl_viewport_y = y;
					gl_context->_gl_viewport_width = width;
					gl_context->_gl_viewport_height = height;
				}
			};
			implement_opengl_function_in_header_macros( glClear )(GLbitfield mask , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_function_body( glClear )(mask);
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u" , mask );
			};
			implement_opengl_function_in_header_macros( glScalef )(GLfloat x, GLfloat y, GLfloat z , ::rux::gui::engine::GLContext* , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				//if( gl_context->_gl_matrix_mode == GL_TEXTURE )
				//{
				//	/*if( gl_context->_gl_scalef_gl_texture[ 0 ] != x
				//		|| gl_context->_gl_scalef_gl_texture[ 1 ] != y
				//		|| gl_context->_gl_scalef_gl_texture[ 2 ] != z )*/
				//	{
				//		opengl_function_body( glScalef )( x, y, z);
				//		CHECK_GL_ERROR();
				//		gl_context->_gl_scalef_gl_texture[ 0 ] = x;
				//		gl_context->_gl_scalef_gl_texture[ 1 ] = y;
				//		gl_context->_gl_scalef_gl_texture[ 2 ] = z;
				//	}
				//}
				//else if( gl_context->_gl_matrix_mode == GL_MODELVIEW )
				//{
				//	if( gl_context->_gl_scalef_gl_modelview[ 0 ] != x
				//		|| gl_context->_gl_scalef_gl_modelview[ 1 ] != y
				//		|| gl_context->_gl_scalef_gl_modelview[ 2 ] != z )
				//	{
				//		opengl_function_body( glScalef )( x, y, z);
				//		CHECK_GL_ERROR();
				//		gl_context->_gl_scalef_gl_modelview[ 0 ] = x;
				//		gl_context->_gl_scalef_gl_modelview[ 1 ] = y;
				//		gl_context->_gl_scalef_gl_modelview[ 2 ] = z;
				//	}
				//}
				//else if( gl_context->_gl_matrix_mode == GL_PROJECTION )
				//{
				//	if( gl_context->_gl_scalef_gl_projection[ 0 ] != x
				//		|| gl_context->_gl_scalef_gl_projection[ 1 ] != y
				//		|| gl_context->_gl_scalef_gl_projection[ 2 ] != z )
				//	{
				//		opengl_function_body( glScalef )( x, y, z);
				//		CHECK_GL_ERROR();
				//		gl_context->_gl_scalef_gl_projection[ 0 ] = x;
				//		gl_context->_gl_scalef_gl_projection[ 1 ] = y;
				//		gl_context->_gl_scalef_gl_projection[ 2 ] = z;
				//	}
				//}
				//else	
				{
					opengl_function_body( glScalef )( x, y, z);
					::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%f , %f , %f" , x, y, z );
				}
			};
			implement_opengl_function_in_header_macros( glHint )(GLenum target, GLenum mode , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_function_body( glHint )( target,  mode);
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u , %u" , target, mode );
			};
			implement_opengl_function_in_header_macros( glFlush )( const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_function_body( glFlush )();
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , NULL );
			};
			implement_opengl_function_in_header_macros( glFinish )( const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_function_body( glFinish )();
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , NULL );
			};
			implement_opengl_function_in_header_macros( glPopMatrix )( const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_function_body( glPopMatrix )();
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , NULL );
			};
			implement_opengl_function_in_header_macros( glEnd )( const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_function_body( glEnd )();
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , NULL );
			};
			implement_opengl_function_in_header_macros( glVertex3f )(GLfloat x, GLfloat y, GLfloat z , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_function_body( glVertex3f )( x,  y,  z);
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%f , %f , %f" , x, y , z );
			};
			implement_opengl_function_in_header_macros( glTexCoord2f )(GLfloat s, GLfloat t , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_function_body( glTexCoord2f )( s, t);
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%f , %f" , s , t );
			};
			implement_opengl_function_in_header_macros( glBegin )(GLenum mode , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_function_body( glBegin )( mode);
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u" , mode );
			};
			implement_opengl_function_in_header_macros( glGetIntegerv )(GLenum pname, GLint *params , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_function_body( glGetIntegerv )( pname , params );
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u , %p" , pname , params );
			};
			implement_opengl_function_in_header_macros( glGetShaderiv )( GLuint shader , GLenum pname , GLint *params , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_render_context_function_body( glGetShaderiv )( shader , pname , params );
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u , %u , %p" , shader , pname , params );
			};
			implement_opengl_function_in_header_macros( glDeleteProgram )( GLuint program , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_render_context_function_body( glDeleteProgram )( program );
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%u" , program );
			};
			implement_opengl_function_in_header_macros( glRasterPos3f )(GLfloat x, GLfloat y , GLfloat z , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_function_body( glRasterPos3f )( x, y , z );
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%f , %f , %f" , x , y , z );
			};
			implement_opengl_function_in_header_macros( glPixelZoom )(GLfloat xfactor, GLfloat yfactor , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_function_body( glPixelZoom )( xfactor, yfactor );
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%f , %f" , xfactor, yfactor );
			};
			implement_opengl_function_in_header_macros( glDrawPixels )(GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 )
			{
				XCodeLabel code_label( ___rux__thread_index1986 , __FILE__ , __FUNCTION__ , __LINE__ , NULL , NULL , 1 , ::rux::gui::engine::_check_opengl_functions_timeout );
				opengl_function_body( glDrawPixels )( width, height, format, type, pixels );
				::rux::gui::engine::OpenGL::check_gl_error( __FUNCTION__ , caller_function , caller_line , "%d , %d , %u , %u , %p" , width, height, format, type, pixels );
			};
#define get_opengl_function( FUNC ) if( _##FUNC == NULL )\
	_##FUNC = (PFN##FUNC##PROC)dlsym( rux::gui::engine::_gl_library , #FUNC );\
	if( _##FUNC == NULL )\
		_##FUNC = (PFN##FUNC##PROC)dlsym( rux::gui::engine::_gl_library , #FUNC"EXT" );\
	if( _##FUNC == NULL )\
		_##FUNC = (PFN##FUNC##PROC)dlsym( rux::gui::engine::_gl_library , #FUNC"OES" );\
	if( _##FUNC == NULL )\
		_##FUNC = (PFN##FUNC##PROC)dlsym( rux::gui::engine::_gl_library , #FUNC"ARB" );\
	if( _##FUNC == NULL )\
		printf( "OpenGL function '"#FUNC"' not found\n" )
#define null_opengl_function( FUNC ) _##FUNC = NULL
			void OpenGL::initialize( void )
			{
				if( XInterlocked::CompareExchange( &rux::gui::engine::_gl_library_inited , 2 , 0 ) != 1 )
				{
					CS_PTR_LOCK( rux::gui::engine::_cs_gl_library );
					if( XInterlocked::CompareExchange( &rux::gui::engine::_gl_library_inited , 0 , 0 ) == 2 )
					{						
#ifdef __WINDOWS__
						const wchar_t* library = L"opengl32.dll";
						::rux::engine::_globals->_on_before_load_module.raise< ::rux::pid_t , ::rux::threading::Thread* , size_t >( 0 , 0 , 0 );
						rux::gui::engine::_gl_library = LoadLibraryW( library );
						::rux::engine::_globals->_on_after_load_module.raise< ::rux::pid_t , ::rux::threading::Thread* , size_t >( 0 , 0 , 0 );
#else
						::rux::engine::_globals->_on_before_load_module.raise< ::rux::pid_t , ::rux::threading::Thread* , size_t >( 0 , 0 , 0 );
						rux::gui::engine::_gl_library = (void*)dlopen( "libGL.so.1" , RTLD_LAZY );										
						::rux::engine::_globals->_on_after_load_module.raise< ::rux::pid_t , ::rux::threading::Thread* , size_t >( 0 , 0 , 0 );
#endif						
						if( rux::gui::engine::_gl_library )
						{
							::rux::safe_strncpy( rux::gui::engine::_error_filename , ::rux::engine::_globals->_executable_directory , 1024 );
							rux_concatenation( rux::gui::engine::_error_filename , "/rux_logs/OpenGL_errors.txt" );								
							::rux::safe_strncpy( rux::gui::engine::_log_filename , ::rux::engine::_globals->_executable_directory , 1024 );
							rux_concatenation( rux::gui::engine::_log_filename , "/rux_logs/OpenGL_log.txt" );															
#ifdef __WINDOWS__
							declare_stack_variable( char , gl_library_filename , 1024 );
							GetModuleFileNameA( rux::gui::engine::_gl_library , gl_library_filename , 1024 );
#endif
#ifdef __WINDOWS__
							get_opengl_function( wglShareLists );
							get_opengl_function( wglGetProcAddress );
							get_opengl_function( wglGetCurrentDC );
							get_opengl_function( wglCreateContext );
							get_opengl_function( wglDeleteContext );
							get_opengl_function( wglMakeCurrent );
							get_opengl_function( wglGetCurrentContext );
#else
#ifdef __ANDROID__
							get_opengl_function( eglGetProcAddress );
#else
							get_opengl_function( glXQueryExtension );
							get_opengl_function( glXGetConfig );
							get_opengl_function( glXGetProcAddress );
							get_opengl_function( glXCreateContext );
							get_opengl_function( glXDestroyContext );
							get_opengl_function( glXSwapBuffers );
							get_opengl_function( glXGetCurrentContext );
							get_opengl_function( glXMakeCurrent );			
#endif
#endif							
							get_opengl_function( glColor4f );
							get_opengl_function( glRotatef );
							get_opengl_function( glGetError );
							get_opengl_function( glDisableClientState );
							get_opengl_function( glDrawArrays );
							get_opengl_function( glVertexPointer );
							get_opengl_function( glPixelStorei );
							get_opengl_function( glEnable );
							get_opengl_function( glBindTexture );
							get_opengl_function( glDisable );
							get_opengl_function( glStencilFunc );
							get_opengl_function( glColorMask );
							get_opengl_function( glStencilOp );
							get_opengl_function( glBlendFunc );
							get_opengl_function( glGetFloatv );
							get_opengl_function( glGetBooleanv );
							get_opengl_function( glTexParameteri );
							get_opengl_function( glTexEnvf );
							get_opengl_function( glTexImage2D );
							get_opengl_function( glTexImage1D );
							get_opengl_function( glLoadIdentity );							
							get_opengl_function( glGenTextures );
							get_opengl_function( glDeleteTextures );
							get_opengl_function( glEnableClientState );
							get_opengl_function( glTexCoordPointer );
							get_opengl_function( glClearStencil );
							get_opengl_function( glColorPointer );
							get_opengl_function( glTexSubImage2D );
							get_opengl_function( glTexSubImage1D );
							get_opengl_function( glPushMatrix );
							get_opengl_function( glMatrixMode );
							get_opengl_function( glDepthFunc );
							get_opengl_function( glClearDepthf );
							get_opengl_function( glScalef );
#ifdef __ANDROID__
							get_opengl_function( glOrthof );
#else
							get_opengl_function( glOrtho );
#endif
							get_opengl_function( glClearColor );
							get_opengl_function( glShadeModel );
							get_opengl_function( glViewport );
							get_opengl_function( glGetString );
							get_opengl_function( glClear );
							get_opengl_function( glScalef );
							get_opengl_function( glHint );
							get_opengl_function( glFlush );
							get_opengl_function( glFinish );
							get_opengl_function( glPopMatrix );
							get_opengl_function( glEnd );
							get_opengl_function( glVertex3f );
							get_opengl_function( glTexCoord2f );
							get_opengl_function( glBegin );							
							get_opengl_function( glGetIntegerv );
							get_opengl_function( glRasterPos3f );
							get_opengl_function( glPixelZoom );
							get_opengl_function( glDrawPixels );
						}
#if defined( __UNIX__ )
						else
						{
							const char* dl_error = dlerror();
						}
#endif
					}
					if( rux::gui::engine::_gl_library )
						XInterlocked::CompareExchange( &rux::gui::engine::_gl_library_inited , 1 , 2 );
					else
					{
#ifdef __UNIX__
						const char* dl_error = dlerror();
						if( dl_error )
							printf( "cannot load module 'libGL.so' , error = '%s'\n" , dl_error );
						else
							printf( "cannot load module 'libGL.so'\n" );					
#endif
						XInterlocked::CompareExchange( &rux::gui::engine::_gl_library_inited , 0 , 2 );
					}
					rux::gui::engine::_cs_gl_library->UnLock();
				}				
			};
			void OpenGL::write_error( const char* text )
			{
				if( rux::io::file::exists( rux::gui::engine::_error_filename ) == 1 
					&& rux::io::file::get_size( rux::gui::engine::_error_filename ) >= 1024ULL * 1024ULL )
					rux::io::file::remove( rux::gui::engine::_error_filename );
				rux::io::file::append(  rux::gui::engine::_error_filename , text );
			};
			void OpenGL::write_log( char* text )
			{
				if( rux::gui::engine::_log_enable == 1 )
				{
					if( rux::io::file::exists( rux::gui::engine::_log_filename ) == 1 
						&& rux::io::file::get_size( rux::gui::engine::_log_filename ) >= 1024ULL * 1024ULL )
					rux::io::file::remove( rux::gui::engine::_log_filename );
					rux::io::file::append(  rux::gui::engine::_log_filename , text );
				}
			};
			void OpenGL::deinitialize( void )
			{	
				if( XInterlocked::CompareExchange( &rux::gui::engine::_gl_library_inited , 2 , 1 ) == 1 )
				{
					CS_PTR_LOCK( rux::gui::engine::_cs_gl_library );
					if( rux::gui::engine::_gl_library )
					{					
						dlclose( rux::gui::engine::_gl_library );
						rux::gui::engine::_gl_library = NULL;
						null_opengl_function( glColor4f );
						null_opengl_function( glRotatef );
						null_opengl_function( glGetError );
						null_opengl_function( glDisableClientState );
						null_opengl_function( glDrawArrays );
						null_opengl_function( glVertexPointer );
						null_opengl_function( glPixelStorei );
						null_opengl_function( glEnable );
						null_opengl_function( glBindTexture );
						null_opengl_function( glDisable );
						null_opengl_function( glStencilFunc );
						null_opengl_function( glColorMask );
						null_opengl_function( glStencilOp );
						null_opengl_function( glBlendFunc );
						null_opengl_function( glGetFloatv );
						null_opengl_function( glGetBooleanv );
						null_opengl_function( glTexParameteri );
						null_opengl_function( glTexEnvf );
						null_opengl_function( glTexImage2D );
						null_opengl_function( glTexImage1D );
						null_opengl_function( glLoadIdentity );						
						null_opengl_function( glGenTextures );
						null_opengl_function( glDeleteTextures );
						null_opengl_function( glEnableClientState );
						null_opengl_function( glTexCoordPointer );
						null_opengl_function( glClearStencil );
						null_opengl_function( glColorPointer );
						null_opengl_function( glTexSubImage2D );
						null_opengl_function( glTexSubImage1D );
						null_opengl_function( glPushMatrix );
						null_opengl_function( glMatrixMode );
						null_opengl_function( glDepthFunc );
						null_opengl_function( glClearDepthf );
						null_opengl_function( glScalef );
#ifdef __ANDROID__
						null_opengl_function( glOrthof );
#else
						null_opengl_function( glOrtho );
#endif
						null_opengl_function( glClearColor );
						null_opengl_function( glShadeModel );
						null_opengl_function( glViewport );
						null_opengl_function( glGetString );
						null_opengl_function( glClear );
						null_opengl_function( glScalef );
						null_opengl_function( glHint );
						null_opengl_function( glFlush );
						null_opengl_function( glFinish );
						null_opengl_function( glPopMatrix );
						null_opengl_function( glEnd );
						null_opengl_function( glVertex3f );
						null_opengl_function( glTexCoord2f );
						null_opengl_function( glBegin );
						
						null_opengl_function( glGetIntegerv );
						null_opengl_function( glRasterPos3f );
						null_opengl_function( glPixelZoom );
						null_opengl_function( glDrawPixels );
#ifdef __WINDOWS__
						null_opengl_function( wglShareLists );
						null_opengl_function( wglGetProcAddress );
						null_opengl_function( wglGetCurrentDC );
						null_opengl_function( wglCreateContext );
						null_opengl_function( wglDeleteContext );
						null_opengl_function( wglMakeCurrent );
						null_opengl_function( wglGetCurrentContext );
#else
#ifdef __ANDROID__
						null_opengl_function( eglGetProcAddress );
#else
						null_opengl_function( glXQueryExtension );
						null_opengl_function( glXGetConfig );
						null_opengl_function( glXGetProcAddress );
						null_opengl_function( glXCreateContext );
						null_opengl_function( glXDestroyContext );
						null_opengl_function( glXSwapBuffers );
						null_opengl_function( glXGetCurrentContext );
						null_opengl_function( glXMakeCurrent );	
#endif
#endif
						XInterlocked::CompareExchange( &rux::gui::engine::_gl_library_inited , 0 , 2 );
					}
					rux::gui::engine::_cs_gl_library->UnLock();
				}
			};
			void OpenGL::check_gl_error( const char* function , const char* caller_function , ::rux::int32 caller_line , const char* format , ... )
			{
				if( ::rux::gui::engine::_check_gl_error )
				{
					GLenum error = ::rux::gui::engine::OpenGL::glGetError();
					if( error != GL_NO_ERROR )
					{
						declare_stack_variable( char , params_string , 1024 );
						params_string[ 0 ] = 0;
						if( format )
						{
							va_list ap;
							va_start( ap , format );
							vsnprintf( params_string , 1024 , format , ap );
							va_end( ap );
						}
						declare_stack_variable( char , error_string , 1024 );
						if( params_string[ 0 ] == 0 )
						{
							if( error == GL_INVALID_ENUM )
								::rux::safe_sprintf( error_string , 1024 , "%s, %s:%d, OpenGL error 'GL_INVALID_ENUM'\n" , function , caller_function , caller_line );
							else if( error == GL_INVALID_VALUE )
								::rux::safe_sprintf( error_string , 1024 , "%s, %s:%d, OpenGL error 'GL_INVALID_VALUE'\n" , function , caller_function , caller_line );
							else if( error == GL_INVALID_OPERATION )
								::rux::safe_sprintf( error_string , 1024 , "%s, %s:%d, OpenGL error 'GL_INVALID_OPERATION'\n" , function , caller_function , caller_line );
							else if( error == GL_STACK_OVERFLOW )
								::rux::safe_sprintf( error_string , 1024 , "%s, %s:%d, OpenGL error 'GL_STACK_OVERFLOW'\n" , function , caller_function , caller_line );
							else if( error == GL_STACK_UNDERFLOW )
								::rux::safe_sprintf( error_string , 1024 , "%s, %s:%d, OpenGL error 'GL_STACK_UNDERFLOW'\n" , function , caller_function , caller_line );
							else if( error == GL_OUT_OF_MEMORY )
								::rux::safe_sprintf( error_string , 1024 , "%s, %s:%d, OpenGL error 'GL_OUT_OF_MEMORY'\n" , function , caller_function , caller_line );
							else
								::rux::safe_sprintf( error_string , 1024 , "%s, %s:%d, OpenGL error '%u'\n" , function , caller_function , caller_line , error );
						}
						else
						{
							if( error == GL_INVALID_ENUM )
								::rux::safe_sprintf( error_string , 1024 , "%s( %s ), %s:%d, OpenGL error 'GL_INVALID_ENUM'\n" , function , params_string , caller_function , caller_line );
							else if( error == GL_INVALID_VALUE )
								::rux::safe_sprintf( error_string , 1024 , "%s( %s ), %s:%d, OpenGL error 'GL_INVALID_VALUE'\n" , function , params_string , caller_function , caller_line );
							else if( error == GL_INVALID_OPERATION )
								::rux::safe_sprintf( error_string , 1024 , "%s( %s ), %s:%d, OpenGL error 'GL_INVALID_OPERATION'\n" , function , params_string , caller_function , caller_line );
							else if( error == GL_STACK_OVERFLOW )
								::rux::safe_sprintf( error_string , 1024 , "%s( %s ), %s:%d, OpenGL error 'GL_STACK_OVERFLOW'\n" , function , params_string , caller_function , caller_line );
							else if( error == GL_STACK_UNDERFLOW )
								::rux::safe_sprintf( error_string , 1024 , "%s( %s ), %s:%d, OpenGL error 'GL_STACK_UNDERFLOW'\n" , function , params_string , caller_function , caller_line );
							else if( error == GL_OUT_OF_MEMORY )
								::rux::safe_sprintf( error_string , 1024 , "%s( %s ), %s:%d, OpenGL error 'GL_OUT_OF_MEMORY'\n" , function , params_string , caller_function , caller_line );
							else
								::rux::safe_sprintf( error_string , 1024 , "%s( %s ), %s:%d, OpenGL error %u\n" , function , params_string , caller_function , caller_line , error );
						}
						::rux::gui::engine::OpenGL::write_error( error_string );
						printf( "%s" , error_string );
					}
				}
			}
		};
	};
};