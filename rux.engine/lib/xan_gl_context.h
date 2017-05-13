#ifndef XAN_GL_CONTEXT_H
#define XAN_GL_CONTEXT_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_gui_defines.h>
#include <xan_opengl_functions.h>
#include <xan_thread.h>
#include <xan_math.h>
#include <xan_render_cache.h>
#include <xan_render_thread.h>
#include <xan_gui_fps.h>
namespace rux
{
	namespace gui
	{
		namespace engine
		{
#define declare_opengl_function_object_in_class_macros( FUNC ) ::rux::gui::engine::PFN##FUNC##PROC _##FUNC
			class RenderContext;
			class Window;
			class RenderFont;
			class GLTexture;
			class GLNamedBufferObject;
			class GLContext
			{
				friend class Window;
				friend class ::rux::gui::engine::OpenGL;
				friend class ::rux::gui::engine::RenderThread;
				friend class RenderFont;
				friend class ::rux::gui::engine::RenderUniversalCache;
				friend class ::rux::gui::engine::RenderContext;
				friend class ::rux::gui::engine::GLTexture;
				friend class ::rux::gui::engine::GLNamedBufferObject;
#ifdef __WINDOWS__
				friend LRESULT CALLBACK WndProc( HWND hwnd , rux::uint32 msg , WPARAM wparam , LPARAM lparam );
				friend dll_internal rux::gui::engine::Window* find_window( HWND hwnd );
#elif defined( __UNIX__ )
#ifdef __ANDROID__
#else
				friend dll_internal void WndProc( XEvent& event );
				friend dll_internal rux::gui::engine::Window* find_window( ::Window& xwindow );
				friend dll_internal void start( rux::gui::on_event_t on_event , void* owner );
#endif
#endif
			private:
#ifdef __UNIX__
				Display* _x_display;
#endif		
				GLboolean _gl_ensable_client_state_gl_color_array;
				GLboolean _gl_ensable_client_state_gl_texture_coord_array;
				GLboolean _gl_ensable_client_state_gl_vertex_array;

				GLint _gl_color_pointer_size;
				GLenum _gl_color_pointer_type;
				GLsizei _gl_color_pointer_stride;
				GLvoid* _gl_color_pointer_pointer;

				GLint _gl_tex_coord_pointer_size;
				GLenum _gl_tex_coord_pointer_type;
				GLsizei _gl_tex_coord_pointer_stride;
				GLvoid* _gl_tex_coord_pointer_pointer;

				GLint _gl_vertex_pointer_size;
				GLenum _gl_vertex_pointer_type;
				GLsizei _gl_vertex_pointer_stride;
				GLvoid* _gl_vertex_pointer_pointer;

				GLenum _gl_blend_func_sfactor;
				GLenum _gl_blend_func_dfactor;

				GLint _gl_clear_stencil;

				GLboolean _gl_color_mask[ 4 ];
												
				GLint _gl_pixel_storei_gl_pack_alignment;
				GLint _gl_pixel_storei_gl_unpack_alignment;

				GLenum _gl_active_texture;

				GLfloat _gl_scalef_gl_texture[ 3 ];
				GLfloat _gl_scalef_gl_projection[ 3 ];
				GLfloat _gl_scalef_gl_modelview[ 3 ];

				GLenum _gl_matrix_mode;
				
				GLfloat _gl_clear_color[ 4 ];

				GLenum _gl_depth_func;

				GLenum _gl_shade_model;

				GLint _gl_viewport_x;
				GLint _gl_viewport_y;
				GLsizei _gl_viewport_width;
				GLsizei _gl_viewport_height;

				GLboolean _gl_cull_face;
				GLboolean _gl_stencil_test;
				GLboolean _gl_depth_test;
				GLboolean _gl_alpha_test;
				GLboolean _gl_blend;
				GLboolean _gl_texture_rectangle;
				GLboolean _gl_texture_2d;

				GLuint _gl_bind_buffer_gl_array_buffer;
				GLuint _gl_bind_buffer_gl_pixel_unpack_buffer;

				::rux::byte _gl_map_buffer_mapped;

			private:
				rux_volatile _ref;
				::rux::pid_t _current_context_thread_id;
				rux::gui::engine::Window* _current_context_window;
				rux::uint8 _is_vsync;
				rux::int32 _width;
				rux::int32 _height;
			private:
#ifdef __WINDOWS__
				HGLRC _gl_context;
				PIXELFORMATDESCRIPTOR _pfd;
#elif defined( __UNIX__ )
#ifdef __ANDROID__
				EGLContext _gl_context;
#else
				GLXContext _gl_context;			
#endif
#endif
				rux::uint32* _framebuffer_color_texture_ptr;
				rux::uint32* _framebuffer_depth_stencil_ptr;
				rux::uint32* _framebuffer_ptr;
				rux::uint32* _smooth_framebuffer_color_texture_ptr;
				rux::uint32* _smooth_framebuffer_depth_stencil_ptr;
				rux::uint32* _smooth_framebuffer_ptr;
				rux::int32 _framebuffer_color_texture_width;
				rux::int32 _framebuffer_color_texture_height;

				rux::uint32 _fxaa_program;
				rux::uint32 _fxaa_fragment_shader;	
				rux::uint32 _fxaa_vertex_shader;
				rux::int32 _fxaa_texture;
				rux::int32 _fxaa_vertex_width;
				rux::int32 _fxaa_vertex_height;
				rux::int32 _fxaa_background;
				rux::int32 _stencil_ref;
				rux::uint32 _yv12_to_rgb_program;
				rux::uint32 _yv12_to_rgb_3_textures_program;
				rux::uint32 _yv12_to_rgb_fragment_shader;	
				rux::uint32 _yv12_to_rgb_3_textures_fragment_shader;	
				rux::uint32 _yv12_to_rgb_vertex_shader;
				rux::uint32 _yv12_to_rgb_3_textures_vertex_shader;
				rux::int32 _yv12_to_rgb_frame_width;
				rux::int32 _yv12_to_rgb_3_textures_v_tex;
				rux::int32 _yv12_to_rgb_3_textures_contrast;
				rux::int32 _yv12_to_rgb_3_textures_brightness;
				rux::int32 _yv12_to_rgb_3_textures_opacity;
				rux::int32 _yv12_to_rgb_3_textures_texture_height;
				rux::int32 _yv12_to_rgb_3_textures_texture_width;
				rux::int32 _yv12_to_rgb_3_textures_frame_height;
				rux::int32 _yv12_to_rgb_3_textures_frame_width;
				rux::int32 _yv12_to_rgb_3_textures_u_tex;
				rux::int32 _yv12_to_rgb_3_textures_y_tex;
				rux::int32 _yv12_to_rgb_texture_width;
				rux::int32 _yv12_to_rgb_texture_height;
				rux::int32 _yv12_to_rgb_window_width;
				rux::int32 _yv12_to_rgb_window_height;
				rux::int32 _yv12_to_rgb_window_x;
				rux::int32 _yv12_to_rgb_window_y;
				rux::int32 _yv12_to_rgb_frame_height;
				rux::int32 _yv12_to_rgb_frame_texture;
				rux::int32 _yv12_to_rgb_frame_opacity;
				rux::uint32 _yuyv_to_rgb_program;
				rux::uint32 _yuyv_to_rgb_fragment_shader;
				rux::uint32 _yuyv_to_rgb_vertex_shader;
				rux::uint32 _simple_rectangle_program;
				rux::uint32 _simple_rectangle_fragment_shader;
				rux::uint32 _simple_rectangle_vertex_shader;
				rux::int32 _simple_rectangle_shader_common;
				rux::int32 _simple_rectangle_shader_texture_width;
				rux::int32 _yuyv_to_rgb_frame_width;
				rux::int32 _yuyv_to_rgb_frame_height;
				rux::int32 _yuyv_to_rgb_frame_texture;
				rux::int32 _yuyv_to_rgb_frame_opacity;
				rux::int32 _yuyv_to_rgb_frame_brightness;
				rux::int32 _yuyv_to_rgb_frame_contrast;
				rux::int32 _yv12_to_rgb_frame_brightness;
				rux::int32 _yv12_to_rgb_frame_contrast;
				rux::uint32 _rgb_program;
				rux::uint32 _rgb_fragment_shader;
				rux::uint32 _rgb_vertex_shader;
				rux::int32 _rgb_frame_width;
				rux::int32 _rgb_frame_height;
				rux::int32 _rgb_frame_opacity;
				rux::int32 _rgb_frame_texture;
				rux::int32 _rgb_frame_brightness;
				rux::int32 _rgb_frame_contrast;

				rux::uint32 _rgba_program;
				rux::uint32 _rgba_fragment_shader;
				rux::uint32 _rgba_vertex_shader;
				rux::int32 _rgba_frame_width;
				rux::int32 _rgba_frame_height;
				rux::int32 _rgba_frame_texture;
				rux::int32 _rgba_frame_opacity;
				rux::int32 _rgba_frame_brightness;
				rux::int32 _rgba_frame_contrast;

				PFNGLDELETEBUFFERSPROC _glDeleteBuffers;
				PFNGLLINKPROGRAMARBPROC _glLinkProgram;
				PFNGLGETATTRIBLOCATIONPROC _glGetAttribLocation;
				PFNGLGETUNIFORMLOCATIONPROC _glGetUniformLocation;
				PFNGLUSEPROGRAMOBJECTARBPROC _glUseProgram;
				PFNGLVALIDATEPROGRAMARBPROC _glValidateProgram;
				PFNGLGETPROGRAMIVPROC _glGetProgramiv;
				declare_opengl_function_object_in_class_macros( glGetShaderiv );
				declare_opengl_function_object_in_class_macros( glDeleteProgram );
				declare_opengl_function_object_in_class_macros( glMultiDrawArrays );
				PFNGLDISABLEVERTEXATTRIBARRAYPROC _glDisableVertexAttribArray;
				PFNGLUNIFORM4FVPROC _glUniform4fv;
				PFNGLMAPBUFFERPROC _glMapBuffer;
				PFNGLUNMAPBUFFERPROC _glUnmapBuffer;
				PFNGLUNIFORM1FPROC _glUniform1f;
				PFNGLUNIFORM1FVPROC _glUniform1fv;
				PFNGLUNIFORM4FPROC _glUniform4f;
#ifdef __ANDROID__
#else
				PFNGLGETINFOLOGARBPROC _glGetInfoLog;
				PFNGLGETOBJECTPARAMETERIVARBPROC _glGetObjectParameteriv;
#endif
				PFNGLSTENCILOPSEPARATEPROC _glStencilOpSeparate;
				PFNGLACTIVESTENCILFACEEXTPROC _glActiveStencilFace;
#ifdef __WINDOWS__
				PFNWGLCHOOSEPIXELFORMATARBPROC _wglChoosePixelFormat;
				PFNWGLSWAPINTERVALEXTPROC _wglSwapInterval;
				PFNWGLEXTGETSWAPINTERVALPROC _wglGetSwapInterval;
#endif	
				PFNGLBINDFRAMEBUFFERPROC _glBindFramebuffer;
				PFNGLDELETEFRAMEBUFFERSPROC _glDeleteFramebuffers;
				PFNGLGENFRAMEBUFFERSPROC _glGenFramebuffers;
				PFNGLGENERATEMIPMAPPROC _glGenerateMipmap;
				PFNGLCHECKFRAMEBUFFERSTATUSPROC _glCheckFramebufferStatus;
				PFNGLFRAMEBUFFERTEXTURE2DPROC _glFramebufferTexture2D;
				PFNGLBINDRENDERBUFFERPROC _glBindRenderbuffer;
				PFNGLDELETERENDERBUFFERSPROC _glDeleteRenderbuffers;
				PFNGLGENRENDERBUFFERSPROC _glGenRenderbuffers;
				PFNGLRENDERBUFFERSTORAGEPROC _glRenderbufferStorage;
				PFNGLGETINTERNALFORMATIVPROC _glGetInternalformativ;
				PFNGLFRAMEBUFFERRENDERBUFFERPROC _glFramebufferRenderbuffer;
				PFNGLACTIVETEXTUREPROC _glActiveTexture;
				PFNGLUNIFORM1IPROC _glUniform1i;
				PFNGLGENBUFFERSPROC _glGenBuffers;
				PFNGLENABLEVERTEXATTRIBARRAYPROC _glEnableVertexAttribArray;
				PFNGLBINDFRAGDATALOCATIONPROC _glBindFragDataLocation;
				PFNGLBINDBUFFERPROC _glBindBuffer;
				PFNGLVERTEXATTRIBPOINTERPROC _glVertexAttribPointer;
				PFNGLBUFFERDATAPROC _glBufferData;
				PFNGLBUFFERSUBDATAPROC _glBufferSubData;
				PFNGLCREATEPROGRAMOBJECTARBPROC _glCreateProgram;
				PFNGLCREATESHADEROBJECTARBPROC _glCreateShader;
				PFNGLSHADERSOURCEARBPROC _glShaderSource;
				PFNGLCOMPILESHADERPROC _glCompileShader;
				PFNGLDELETEOBJECTARBPROC _glDeleteShader;
				PFNGLDETACHOBJECTARBPROC _glDetachShader;
				PFNGLATTACHOBJECTARBPROC _glAttachShader;

				XMallocArray< char* > _gl_extensions;
				const char* _gl_version;
				const char* _gl_renderer;
				const char* _gl_shader_version;
				char* _gl_extensions_string;
				rux::uint8 _is_support_arb_texture_rectangle;
				rux::uint8 _is_support_smooth;
				rux::uint8 _is_support_vbo;
				rux::int32 _max_texture_size[ 1 ];
				rux::int32 _max_uniform_locations[ 1 ];
				rux::uint8 _is_support_framebuffer_object;
				rux::uint8 _is_supported_yv12_to_rgb_shader;	
				::rux::byte _is_supported_yv12_to_rgb_3_textures_shader;
				rux::uint8 _is_supported_yuyv_to_rgb_shader;	
				::rux::byte _is_supported_rgba_shader;
				::rux::byte _is_supported_rgb_shader;
				rux::uint8 _is_supported_fxaa;
				rux::uint8 _is_supported_old_yv12_to_rgb_shader;	
				rux::uint8 _is_supported_old_yuyv_to_rgb_shader;
				::rux::uint8 _is_supported_simple_rectangle_shader;
				rux::uint8 _is_supported_wgl_swap_buffer_interval;
				rux::byte _is_supported_ext_swap_control_tear;
				::rux::uint32 _shader_version_major_version;
				::rux::uint32 _shader_version_minor_version;
				bool _draw_pixels;
			public:
				XMallocArray< rux::gui::engine::RenderFont* > _fonts;
				::rux::threading::RdWrLock _cs_caches;
				XMallocArray< rux::gui::engine::RenderUniversalCache* > _render_caches;
				void SwapBuffers( size_t ___rux__thread_index1986 );
				void set_CurrentContext( ::rux::gui::engine::Window* window );
				void set_EmptyContext( void );
			public:
				GLContext( ::rux::gui::engine::Window* window );
				~GLContext();
				void initialize_opengl_functions( void );
				rux::uint8 IsSupportShaders( void );
				rux::uint8 IsSupportVBO( size_t ___rux__thread_index1986 );
				rux::uint8 IsSupportFrameBufferObject( size_t ___rux__thread_index1986 );	
				rux::uint8 IsSupportExtension( const char* extension , size_t ___rux__thread_index1986 );
				GLenum get_Format( rux::int32 fourcc , int16 bit_count );
				::rux::uint8 try_compile_yv12_shader( size_t ___rux__thread_index1986 );
				::rux::byte try_compile_yv12_to_rgb_3_textures_shader( size_t ___rux__thread_index1986 );
				::rux::uint8 try_compile_yv12_old_shader( size_t ___rux__thread_index1986 );
				::rux::uint8 try_compile_yuyv_shader( size_t ___rux__thread_index1986 );
				::rux::uint8 try_compile_rgb_shader( size_t ___rux__thread_index1986 );
				::rux::uint8 try_compile_rgba_shader( size_t ___rux__thread_index1986 );
				::rux::uint8 try_compile_yuyv_old_shader( size_t ___rux__thread_index1986 );
				::rux::uint8 try_compile_fxaa_shader( size_t ___rux__thread_index1986 );
				::rux::uint8 try_compile_simple_rectangle_shader( size_t ___rux__thread_index1986 );
				char* load_shader_from_file( const char* shader_filename );
				GLint check_shader_compile_status( ::rux::uint32& program , ::rux::uint32& fragment_shader , ::rux::uint32& vertex_shader , const char* name , size_t ___rux__thread_index1986 );
				GLint check_shader_link_status( ::rux::uint32& program , ::rux::uint32& fragment_shader , ::rux::uint32& vertex_shader , const char* name , size_t ___rux__thread_index1986 );
				void delete_shader_and_program( ::rux::uint32& program , ::rux::uint32& fragment_shader , ::rux::uint32& vertex_shader , ::rux::byte attached , size_t ___rux__thread_index1986 );
				void Resize(bool& need_restart, rux::int32 width , rux::int32 height , size_t ___rux__thread_index1986 );
				void AddRef( void );
				void Release( void );
			};
		};
	};
};
#endif
