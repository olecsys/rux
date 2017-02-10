#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_gl_context.h>
#include <xan_thread.h>
#include <xan_gui_window.h>
#include <xan_gui_engine.h>
#include <xan_log.h>
#include <xan_io_defines.h>
namespace rux
{
	namespace gui
	{
		namespace engine
		{
			const GLchar* _rux_vertex_shader = 
				"#version 130\n"
				"void main() {\n"
				"  gl_Position = ftransform();\n"
				"}\n";			
			const GLchar* _rux_fxaa_fragment_shader = 
				"#version 120\n"
				"#define FXAA_REDUCE_MIN (1.0/128.0)\n"
				"#define FXAA_REDUCE_MUL (1.0/8.0)\n"
				"#define FXAA_SPAN_MAX 8.0\n"
				"#define FXAA_SUBPIX_SHIFT ( 1.0/4.0 )\n"
				"uniform sampler2D texture;\n"
				"uniform sampler2D texture1;\n"
				"uniform float vertex_width;\n"
				"uniform float vertex_height;\n"
				"void main(){\n"
				" vec2 resolution = vec2( vertex_width , vertex_height );\n"
				"  vec2 inverse_resolution=vec2(1.0/resolution.x,1.0/resolution.y);\n"	
				"	vec2 coords = gl_FragCoord.xy - ( inverse_resolution * ( 0.5 + FXAA_SUBPIX_SHIFT ) );\n"
				"  vec4 rgbNWvec4 = mix( texture2D( texture , coords.xy ) , texture2D(texture1, coords.xy) , texture2D( texture , coords.xy ).a );\n"
				"  vec4 rgbNEvec4 = mix( texture2D(texture, (coords.xy + vec2(1.0,0.0))) , texture2D(texture1, (coords.xy + vec2(1.0,0.0))) , texture2D(texture, (coords.xy + vec2(1.0,0.0))).a );\n"
				"  vec4 rgbSWvec4 = mix( texture2D(texture, (coords.xy + vec2(0.0,1.0))) , texture2D(texture1, (coords.xy + vec2(0.0,1.0))) , texture2D(texture, (coords.xy + vec2(0.0,1.0))).a );\n"
				"  vec4 rgbSEvec4 = mix( texture2D(texture, (coords.xy + vec2(1.0,1.0))) , texture2D(texture1, (coords.xy + vec2(1.0,1.0))) , texture2D(texture, (coords.xy + vec2(1.0,1.0))).a );\n"
				"  vec4 rgbMvec4  = mix( texture2D(texture,  gl_FragCoord.xy ) , texture2D( texture1 , gl_FragCoord.xy ) , texture2D(texture,  gl_FragCoord.xy ).a );\n"	
				"  vec3 rgbNW = rgbNWvec4.xyz;\n"					
				"  vec3 rgbNE = rgbNEvec4.xyz;\n"				
				"  vec3 rgbSW = rgbSWvec4.xyz;\n"				
				"  vec3 rgbSE = rgbSEvec4.xyz;\n"				
				"  vec3 rgbM  = rgbMvec4.xyz;\n"
				"  vec3 luma = vec3(0.299, 0.587, 0.114);\n"
				"  float lumaNW = dot(rgbNW, luma);\n"
				"  float lumaNE = dot(rgbNE, luma);\n"
				"  float lumaSW = dot(rgbSW, luma);\n"
				"  float lumaSE = dot(rgbSE, luma);\n"
				"  float lumaM  = dot(rgbM,  luma);\n"
				"  float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));\n"
				"  float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE))); \n"
				"  vec2 dir;\n"
				"  dir.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE));\n"
				"  dir.y =  ((lumaNW + lumaSW) - (lumaNE + lumaSE));\n"
				"  float dirReduce = max((lumaNW + lumaNE + lumaSW + lumaSE) * (0.25 * FXAA_REDUCE_MUL),FXAA_REDUCE_MIN);\n"
				"  float rcpDirMin = 1.0/(min(abs(dir.x), abs(dir.y)) + dirReduce);\n"
				"  dir = min(vec2( FXAA_SPAN_MAX,  FXAA_SPAN_MAX),max(vec2(-FXAA_SPAN_MAX, -FXAA_SPAN_MAX),dir * rcpDirMin)) * inverse_resolution;\n"
				"  vec3 rgbA = 0.5 * (mix( texture2DLod(texture,   gl_FragCoord.xy  * inverse_resolution + dir * (1.0/3.0 - 0.5) , 0.0 ) , texture2DLod(texture1,   gl_FragCoord.xy  * inverse_resolution + dir * (1.0/3.0 - 0.5) , 0.0) , 0.5 ).xyz + mix( texture2DLod(texture,   gl_FragCoord.xy  * inverse_resolution + dir * (2.0/3.0 - 0.5) , 0.0) , texture2DLod(texture1,   gl_FragCoord.xy  * inverse_resolution + dir * (2.0/3.0 - 0.5) , 0.0 ) , 0.5 ).xyz);\n"
				"  float opacityA = 0.5 * (texture2DLod(texture,   gl_FragCoord.xy  * inverse_resolution + dir * (1.0/3.0 - 0.5) , 0.0).w + texture2DLod(texture,   gl_FragCoord.xy  * inverse_resolution + dir * (2.0/3.0 - 0.5) , 0.0).w);\n"
				"  vec3 rgbB = rgbA * 0.5 + 0.25 * (mix( texture2DLod(texture,  gl_FragCoord.xy  * inverse_resolution + dir *  - 0.5 , 0.0) , texture2DLod(texture1,  gl_FragCoord.xy  * inverse_resolution + dir *  - 0.5 , 0.0) , 0.5 ).xyz + mix( texture2DLod(texture,  gl_FragCoord.xy  * inverse_resolution + dir * 0.5 , 0.0) , texture2DLod(texture1,  gl_FragCoord.xy  * inverse_resolution + dir * 0.5 , 0.0) , 0.5 ).xyz);\n"
				"  float opacityB  = opacityA * 0.5 + 0.25 * (texture2DLod(texture,  gl_FragCoord.xy  * inverse_resolution + dir *  - 0.5 , 0.0).w + texture2DLod(texture,  gl_FragCoord.xy  * inverse_resolution + dir * 0.5 , 0.0).w);\n"
				"  float lumaB = dot(rgbB, luma);\n"
				"  if((lumaB < lumaMin) || (lumaB > lumaMax)) {\n"
				"     gl_FragColor = vec4(rgbA,opacityA);\n"
				//"     gl_FragColor = vec4(rgbA,1.0);\n"
				"  } else {\n"
				"     gl_FragColor = vec4(rgbB,opacityB);\n"
				//"     gl_FragColor = vec4(rgbB,1.0);\n"
				"  }\n"
				"}\n";
			GLContext::GLContext( ::rux::gui::engine::Window* window )
			{
				::rux::log::WriteDebug( "%s:%d BEGIN" , __FUNCTION__ , __LINE__ );
				_ref = 1;
#ifdef __UNIX__
				_x_display = window->_x_display;
#endif
				_shader_version_major_version = 0;
				_shader_version_minor_version = 0;
				_current_context_thread_id = 0;
				_gl_ensable_client_state_gl_color_array = 0;
				_gl_ensable_client_state_gl_texture_coord_array = 0;
				_gl_ensable_client_state_gl_vertex_array = 0;
				_gl_map_buffer_mapped = 0;
				_gl_bind_buffer_gl_array_buffer = 0;
				_gl_bind_buffer_gl_pixel_unpack_buffer = 0;
				_gl_cull_face = GL_FALSE;
				_gl_stencil_test = GL_FALSE;
				_gl_depth_test = GL_FALSE;
				_gl_alpha_test = GL_FALSE;
				_gl_blend = GL_FALSE;
				_gl_texture_rectangle = GL_FALSE;
				_gl_texture_2d = GL_FALSE;
				_gl_viewport_x = 0;
				_gl_viewport_y = 0;
				_gl_viewport_width = 0;
				_gl_viewport_height = 0;

				_gl_shade_model = GL_SMOOTH;
				_gl_depth_func = GL_LESS;
				_gl_clear_color[ 0 ] = 0;
				_gl_clear_color[ 1 ] = 0;
				_gl_clear_color[ 2 ] = 0;
				_gl_clear_color[ 3 ] = 0;
				_gl_matrix_mode = GL_MODELVIEW;
				_gl_scalef_gl_texture[ 0 ] = 1;
				_gl_scalef_gl_texture[ 1 ] = 1;
				_gl_scalef_gl_texture[ 2 ] = 1;
				_gl_scalef_gl_projection[ 0 ] = 1;
				_gl_scalef_gl_projection[ 1 ] = 1;
				_gl_scalef_gl_projection[ 2 ] = 1;
				_gl_scalef_gl_modelview[ 0 ] = 1;
				_gl_scalef_gl_modelview[ 1 ] = 1;
				_gl_scalef_gl_modelview[ 2 ] = 1;
				_gl_active_texture = GL_TEXTURE0;
				_gl_pixel_storei_gl_pack_alignment = 4;
				_gl_pixel_storei_gl_unpack_alignment = 4;
				_gl_vertex_pointer_size = 4;
				_gl_vertex_pointer_type = GL_FLOAT;
				_gl_vertex_pointer_stride = 0;
				_gl_vertex_pointer_pointer = 0;
				_gl_color_pointer_size = 4;
				_gl_color_pointer_type = GL_FLOAT;
				_gl_color_pointer_stride = 0;
				_gl_color_pointer_pointer = NULL;
				_gl_tex_coord_pointer_size = 4;
				_gl_tex_coord_pointer_type = GL_FLOAT;
				_gl_tex_coord_pointer_stride = 0;
				_gl_tex_coord_pointer_pointer = NULL;
				_gl_clear_stencil = 0;
				_gl_color_mask[ 0 ] = GL_TRUE;
				_gl_color_mask[ 1 ] = GL_TRUE;
				_gl_color_mask[ 2 ] = GL_TRUE;
				_gl_color_mask[ 3 ] = GL_TRUE;
				_gl_blend_func_sfactor = GL_ONE;
				_gl_blend_func_dfactor = GL_ZERO;
				_gl_version = NULL;
				_gl_renderer = NULL;
				_gl_shader_version = NULL;
				_gl_extensions_string = NULL;
				_is_support_arb_texture_rectangle = 0;
				_is_support_smooth = 0;
				_is_support_vbo = 0;
				_max_texture_size[ 0 ] = 0;
				_max_uniform_locations[ 0 ] = 0;
				_is_support_framebuffer_object = 0;
				_is_supported_yv12_to_rgb_shader = 0;
				_is_supported_yv12_to_rgb_3_textures_shader = 0;
				_is_supported_yuyv_to_rgb_shader = 0;
				_is_supported_rgb_shader = 0;
				_is_supported_rgba_shader = 0;
				_is_supported_fxaa = 0;
				_is_supported_old_yv12_to_rgb_shader = 0;
				_is_supported_old_yuyv_to_rgb_shader = 0;
				_is_supported_simple_rectangle_shader = 0;
				_is_supported_wgl_swap_buffer_interval = 0;
				_is_supported_ext_swap_control_tear = 0;
				_yv12_to_rgb_program = 0;
				_yv12_to_rgb_3_textures_program = 0;
				_yv12_to_rgb_fragment_shader = 0;
				_yv12_to_rgb_3_textures_fragment_shader = 0;
				_yv12_to_rgb_vertex_shader = 0;
				_yv12_to_rgb_3_textures_vertex_shader = 0;
				_yv12_to_rgb_frame_width = -1;
				_yv12_to_rgb_3_textures_v_tex = -1;
				_yv12_to_rgb_3_textures_contrast = -1;
				_yv12_to_rgb_3_textures_brightness = -1;
				_yv12_to_rgb_3_textures_opacity = -1;
				_yv12_to_rgb_3_textures_texture_height = -1;
				_yv12_to_rgb_3_textures_texture_width = -1;
				_yv12_to_rgb_3_textures_frame_height = -1;
				_yv12_to_rgb_3_textures_frame_width = -1;
				_yv12_to_rgb_3_textures_u_tex = -1;
				_yv12_to_rgb_3_textures_y_tex = -1;
				_yv12_to_rgb_texture_width = -1;
				_yv12_to_rgb_texture_height = -1;
				_yv12_to_rgb_window_width = -1;
				_yv12_to_rgb_window_x = -1;
				_yv12_to_rgb_window_y = -1;
				_yv12_to_rgb_window_height = -1;
				_yv12_to_rgb_frame_height = -1;
				_yv12_to_rgb_frame_texture = -1;
				_yv12_to_rgb_frame_opacity = -1;
				_yuyv_to_rgb_program = 0;
				_yuyv_to_rgb_fragment_shader = 0;
				_yuyv_to_rgb_vertex_shader = 0;
				_simple_rectangle_program = 0;
				_simple_rectangle_fragment_shader = 0;
				_simple_rectangle_vertex_shader = 0;
				_simple_rectangle_shader_common = -1;
				_simple_rectangle_shader_texture_width = -1;
				_rgb_program = 0;
				_rgb_fragment_shader = 0;
				_rgb_vertex_shader = 0;
				_rgb_frame_width = -1;
				_rgb_frame_height = -1;
				_rgb_frame_opacity = -1;
				_rgb_frame_texture = -1;
				_rgb_frame_brightness = -1;
				_rgb_frame_contrast = -1;
				_rgba_program = 0;
				_rgba_fragment_shader = 0;
				_rgba_vertex_shader = 0;
				_rgba_frame_width = -1;
				_rgba_frame_height = -1;
				_rgba_frame_texture = -1;
				_rgba_frame_opacity = -1;
				_rgba_frame_brightness = -1;
				_rgba_frame_contrast = -1;
				_yuyv_to_rgb_frame_width = -1;
				_yuyv_to_rgb_frame_height = -1;
				_yuyv_to_rgb_frame_texture = -1;
				_yuyv_to_rgb_frame_opacity = -1;
				_yuyv_to_rgb_frame_brightness = -1;
				_yuyv_to_rgb_frame_contrast = -1;
				_yv12_to_rgb_frame_brightness = -1;
				_yv12_to_rgb_frame_contrast = -1;
				_fxaa_program = 0;
				_fxaa_fragment_shader = 0;
				_fxaa_vertex_shader = 0;
				_fxaa_texture = -1;
				_fxaa_vertex_width = -1;
				_fxaa_vertex_height = -1;
				_fxaa_background = -1;
				_is_vsync = 1;
				_framebuffer_color_texture_ptr = NULL;
				_framebuffer_depth_stencil_ptr = NULL;
				_framebuffer_ptr = NULL;
				_smooth_framebuffer_color_texture_ptr = NULL;
				_smooth_framebuffer_depth_stencil_ptr = NULL;
				_smooth_framebuffer_ptr = NULL;
				_framebuffer_color_texture_width = 0;
				_framebuffer_color_texture_height = 0;
				_width = 0;
				_height = 0;
#ifdef __WINDOWS__
				rux::gui::engine::OpenGL::initialize();
				_pfd.nSize = sizeof( PIXELFORMATDESCRIPTOR );
				_pfd.nVersion = 1;
				_pfd.dwFlags = window->_is_transparent_window == 1 ? PFD_DRAW_TO_BITMAP | PFD_SUPPORT_OPENGL : PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
				_pfd.iPixelType = PFD_TYPE_RGBA;
				if( ::rux::gui::engine::_displays_count > 0 )
					_pfd.cColorBits = ::rux::gui::engine::_display_bits[ 0 ];
				else
					_pfd.cColorBits = 32;//pfd.cColorBits     = 32;
				_pfd.cRedBits = 0;//_pfd.cRedBits       = 8;
				_pfd.cRedShift = 0;//_pfd.cRedShift      = 16;
				_pfd.cGreenBits = 0;//_pfd.cGreenBits     = 8;
				_pfd.cGreenShift = 0;//_pfd.cGreenShift    = 8;
				_pfd.cBlueBits = 0;//_pfd.cBlueBits      = 8;
				_pfd.cBlueShift = 0;
				_pfd.cAlphaBits = 0;
				_pfd.cAlphaShift = 0;
				_pfd.cAccumBits = 0;    //_pfd.cAccumBits     = 64;    
				_pfd.cAccumRedBits = 0;//_pfd.cAccumRedBits  = 16;
				_pfd.cAccumGreenBits = 0;//_pfd.cAccumGreenBits   = 16;
				_pfd.cAccumBlueBits = 0;//_pfd.cAccumBlueBits    = 16;
				_pfd.cAccumAlphaBits = 0;
				_pfd.cDepthBits = 32;
				_pfd.cStencilBits = 32;//_pfd.cStencilBits      = 8;
				_pfd.cAuxBuffers = 0;
				_pfd.iLayerType = PFD_MAIN_PLANE;
				_pfd.bReserved = 0;
				_pfd.dwLayerMask = 0;
				_pfd.dwVisibleMask = 0;
				_pfd.dwDamageMask = 0;		
				rux::int32 pixel_format_index = ChoosePixelFormat( window->_hdc , &_pfd );
				if( pixel_format_index == 0 )
				{
					pixel_format_index = 1;    
					if( DescribePixelFormat( window->_hdc , pixel_format_index , sizeof( PIXELFORMATDESCRIPTOR ) , &_pfd ) == 0 )
					{
					}
				}
				if ( SetPixelFormat( window->_hdc, pixel_format_index, &_pfd ) == FALSE )
					printf( "SetPixelFormat FALSE\n" );
				_gl_context = ::rux::gui::engine::OpenGL::wglCreateContext( window->_hdc );
#else
#ifdef __ANDROID__
#else
				_gl_context = ::rux::gui::engine::OpenGL::glXCreateContext( window->_x_display , ::rux::gui::engine::_x_visual_info , NULL , true );
				if( _gl_context == NULL )
					_gl_context = ::rux::gui::engine::OpenGL::glXCreateContext( window->_x_display , ::rux::gui::engine::_x_visual_info , NULL , false );
#endif
#endif				
				if( _gl_context == NULL )
				{
					GLenum error = ::rux::gui::engine::OpenGL::glGetError();
					if( error != GL_NO_ERROR )
						printf( "wglCreateContext , OpenGL error=%u\n" , error );
				}
				else
					printf( "wglCreateContext , context=%p\n" , _gl_context );
				set_CurrentContext( window );
				initialize_opengl_functions();
				IsSupportExtension( "" , SIZE_MAX );
				if( _is_supported_simple_rectangle_shader == 1 && ( IsSupportExtension( "GL_ARB_texture_float" , SIZE_MAX ) == 0 || try_compile_simple_rectangle_shader( SIZE_MAX ) == 0 ) )
					_is_supported_simple_rectangle_shader = 0;
				if( _is_supported_yv12_to_rgb_3_textures_shader == 1
					&& try_compile_yv12_to_rgb_3_textures_shader( SIZE_MAX ) == 0 )
					_is_supported_yv12_to_rgb_3_textures_shader = 0;
				if( _is_supported_yv12_to_rgb_shader == 1 && try_compile_yv12_shader( SIZE_MAX ) == 0 )
					_is_supported_yv12_to_rgb_shader = 0;
				if( _is_supported_old_yv12_to_rgb_shader == 1 && try_compile_yv12_old_shader( SIZE_MAX ) == 0 )
					_is_supported_old_yv12_to_rgb_shader = 0;
				if( _is_supported_yuyv_to_rgb_shader == 1 && try_compile_yuyv_shader( SIZE_MAX ) == 0 )
					_is_supported_yuyv_to_rgb_shader = 0;
				if( _is_supported_old_yuyv_to_rgb_shader == 1 && try_compile_yuyv_old_shader( SIZE_MAX ) == 0 )
					_is_supported_old_yuyv_to_rgb_shader = 0;
				if( _is_supported_rgb_shader == 1 && try_compile_rgb_shader( SIZE_MAX ) == 0 )
					_is_supported_rgb_shader = 0;
				if( _is_supported_rgba_shader == 1 && try_compile_rgba_shader( SIZE_MAX ) == 0 )
					_is_supported_rgba_shader = 0;
				rux::gui::engine::OpenGL::glClearColor( 0.0f , 0.0f , 0.0f , 0.0f , this , __FUNCTION__ , __LINE__ , SIZE_MAX );
				rux::gui::engine::OpenGL::glShadeModel( GL_SMOOTH , this , __FUNCTION__ , __LINE__ , SIZE_MAX );
				rux::gui::engine::OpenGL::glClearDepthf( 1.0f , __FUNCTION__ , __LINE__ , SIZE_MAX );
				rux::gui::engine::OpenGL::glEnable( GL_DEPTH_TEST , this , __FUNCTION__ , __LINE__ , SIZE_MAX );
				rux::gui::engine::OpenGL::glDepthFunc( GL_LESS , this , __FUNCTION__ , __LINE__ , SIZE_MAX );
				set_EmptyContext();

				::rux::log::WriteDebug( "%s:%d END" , __FUNCTION__ , __LINE__ );
			};
#define opengl_proc_addr( FUNC ) if( gl_error == 0 )\
	{\
		this->_##FUNC = (PFN##FUNC##PROC)glGetProcAddress( #FUNC );\
		if( this->_##FUNC == NULL )\
			this->_##FUNC = (PFN##FUNC##PROC)glGetProcAddress( #FUNC"EXT" );\
		if( this->_##FUNC == NULL )\
			this->_##FUNC = (PFN##FUNC##PROC)glGetProcAddress( #FUNC"OES" );\
		if( this->_##FUNC == NULL )\
			this->_##FUNC = (PFN##FUNC##PROC)glGetProcAddress( #FUNC"ARB" );\
		if( this->_##FUNC == NULL )\
		{\
			gl_error = rux::gui::engine::_glGetError();\
			if( gl_error == 0 )\
				printf( "OpenGL function '"#FUNC"' not found\n" );\
		}\
	}
			void GLContext::initialize_opengl_functions( void )
			{
				GLenum gl_error = 0;		
				opengl_proc_addr( glDeleteBuffers );
				opengl_proc_addr( glLinkProgram );
				opengl_proc_addr( glBindFragDataLocation );
				opengl_proc_addr( glGetAttribLocation );
				opengl_proc_addr( glGetUniformLocation );
				opengl_proc_addr( glUseProgram );
				opengl_proc_addr( glValidateProgram );
				opengl_proc_addr( glGetProgramiv );
				opengl_proc_addr( glGetShaderiv );
				opengl_proc_addr( glDeleteProgram );
				opengl_proc_addr( glDisableVertexAttribArray );
				opengl_proc_addr( glUniform4fv );
				opengl_proc_addr( glMapBuffer );
				opengl_proc_addr( glUnmapBuffer );
				opengl_proc_addr( glUniform1f );
				opengl_proc_addr( glUniform1fv );
				opengl_proc_addr( glUniform4f );
#ifdef __ANDROID__
#else
				opengl_proc_addr( glGetObjectParameteriv );
				opengl_proc_addr( glGetInfoLog );
#endif
				opengl_proc_addr( glStencilOpSeparate );
				opengl_proc_addr( glActiveStencilFace );
#ifdef __WINDOWS__
				opengl_proc_addr( wglGetSwapInterval );
				opengl_proc_addr( wglSwapInterval );
				opengl_proc_addr( wglChoosePixelFormat );
#endif
				opengl_proc_addr( glMultiDrawArrays );
				opengl_proc_addr( glBindFramebuffer );
				opengl_proc_addr( glDeleteFramebuffers );
				opengl_proc_addr( glGenFramebuffers );
				opengl_proc_addr( glGenerateMipmap );
				opengl_proc_addr( glCheckFramebufferStatus );
				opengl_proc_addr( glFramebufferTexture2D );
				opengl_proc_addr( glBindRenderbuffer );
				opengl_proc_addr( glDeleteRenderbuffers );
				opengl_proc_addr( glGenRenderbuffers );
				opengl_proc_addr( glRenderbufferStorage );
				opengl_proc_addr( glGetInternalformativ );
				opengl_proc_addr( glFramebufferRenderbuffer );
				opengl_proc_addr( glActiveTexture );
				opengl_proc_addr( glUniform1i );
				opengl_proc_addr( glGenBuffers );
				opengl_proc_addr( glEnableVertexAttribArray );
				opengl_proc_addr( glBindBuffer );
				opengl_proc_addr( glVertexAttribPointer );
				opengl_proc_addr( glBufferData );
				opengl_proc_addr( glBufferSubData );
				opengl_proc_addr( glCreateProgram );
				opengl_proc_addr( glCreateShader );
				opengl_proc_addr( glShaderSource );
				opengl_proc_addr( glCompileShader );
				opengl_proc_addr( glDeleteShader );
				opengl_proc_addr( glDetachShader );
				opengl_proc_addr( glAttachShader );
			};
			GLContext::~GLContext()
			{
				set_CurrentContext( _current_context_window );
				WRITE_LOCK( _cs_caches );
				for( size_t index0 = 0 ; index0 < _render_caches.Count() ; index0++ )
				{
					::rux::gui::engine::RenderUniversalCache* cache = _render_caches[ index0 ];
					if( cache )
						cache->Release( __FILE__ , __LINE__ );
				}
				_cs_caches.WriteUnlock();
				for( ; _fonts.Count() > 0 ; )
				{
					rux::engine::free_object< ::rux::gui::engine::RenderFont >( _fonts[ _fonts.Count() - 1 ] );
					_fonts.RemoveAt( _fonts.Count() - 1 );
				}
				delete_shader_and_program( _yv12_to_rgb_3_textures_program, _yv12_to_rgb_3_textures_fragment_shader , _yv12_to_rgb_3_textures_vertex_shader , 1 , SIZE_MAX );
				delete_shader_and_program( _yv12_to_rgb_program , _yv12_to_rgb_fragment_shader , _yv12_to_rgb_vertex_shader , 1 , SIZE_MAX );
				delete_shader_and_program( _yuyv_to_rgb_program , _yuyv_to_rgb_fragment_shader , _yuyv_to_rgb_vertex_shader , 1 , SIZE_MAX );
				delete_shader_and_program( _simple_rectangle_program , _simple_rectangle_fragment_shader , _simple_rectangle_vertex_shader , 1 , SIZE_MAX );
				delete_shader_and_program( _rgb_program , _rgb_fragment_shader , _rgb_vertex_shader , 1 , SIZE_MAX );
				delete_shader_and_program( _rgba_program , _rgba_fragment_shader , _rgba_vertex_shader , 1 , SIZE_MAX );
				delete_shader_and_program( _fxaa_program , _fxaa_fragment_shader , _fxaa_vertex_shader , 1 , SIZE_MAX );
				set_EmptyContext();
				if( _gl_context )
				{
					printf( "wglDeleteContext , context=%p\n" , _gl_context );
#ifdef __WINDOWS__			
					rux::gui::engine::OpenGL::wglDeleteContext( _gl_context );
#elif defined( __UNIX__ )		
#ifdef __ANDROID__
#else
					rux::gui::engine::OpenGL::glXDestroyContext( _x_display , _gl_context , SIZE_MAX );
#endif
#endif
				}
				if( _gl_extensions_string )
					rux::engine::free_mem( _gl_extensions_string );
			};
			void GLContext::AddRef( void )
			{
				XInterlocked::Increment( &_ref );
			};
			void GLContext::Release( void )
			{
				if(	XInterlocked::Decrement( &_ref ) == 0 )
					::rux::engine::free_object< GLContext >( this );
			};
			void GLContext::Resize(bool& need_restart, ::rux::int32 width , ::rux::int32 height , size_t ___rux__thread_index1986 )
			{
				if(need_restart || _width != width || _height != height )
				{
					need_restart = false;
					if( _gl_context )
					{
						_width = width;
						_height = height;
	#ifdef __WINDOWS__					
						if( _current_context_window->_is_transparent_window == 1 )
						{
							if( _current_context_window->_transparent_window_bitmap )
							{
								DeleteObject( _current_context_window->_transparent_window_bitmap );
								_current_context_window->_transparent_window_bitmap = NULL;
							}
							void* dib_bits = NULL;
							BITMAPINFOHEADER bitmap_info_header;
							rux::int32 size = sizeof( BITMAPINFOHEADER );
							memset( &bitmap_info_header , 0 , size );
							bitmap_info_header.biSize = size;
							bitmap_info_header.biWidth = _width;
							bitmap_info_header.biHeight = _height;
							bitmap_info_header.biPlanes = 1;
							bitmap_info_header.biBitCount = 24;
							bitmap_info_header.biCompression = BI_RGB;			
							_current_context_window->_transparent_window_bitmap = CreateDIBSection( _current_context_window->_hdc , (BITMAPINFO*)&bitmap_info_header , DIB_RGB_COLORS , &dib_bits , NULL , 0 );
							SelectObject( _current_context_window->_hdc , _current_context_window->_transparent_window_bitmap );
						}
	#endif
						//rux::gui::engine::OpenGL::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT , __FUNCTION__ , __LINE__ );
						rux::gui::engine::OpenGL::glLoadIdentity( __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						rux::gui::engine::OpenGL::glViewport( 0 , 0 , _width , _height , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						//rux::gui::engine::OpenGL::glShadeModel( GL_SMOOTH , this , __FUNCTION__ , __LINE__ );
						//rux::gui::engine::OpenGL::glClearColor( 0.0f , 0.0f , 0.0f , 0.0f , this , __FUNCTION__ , __LINE__ );
						//rux::gui::engine::OpenGL::glClearDepthf( 1.0f , __FUNCTION__ , __LINE__ );
						//rux::gui::engine::OpenGL::glEnable( GL_DEPTH_TEST , this , __FUNCTION__ , __LINE__ );
						//rux::gui::engine::OpenGL::glDepthFunc( GL_LESS , this , __FUNCTION__ , __LINE__ );
						rux::gui::engine::OpenGL::glMatrixMode( GL_PROJECTION , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						rux::gui::engine::OpenGL::glLoadIdentity( __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
	#ifdef __ANDROID__
						rux::gui::engine::OpenGL::glOrthof( 0 , _width , _height , 0 , -1.f , 1.f , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
	#else
						rux::gui::engine::OpenGL::glOrtho( 0 , _width , _height , 0 , -1.f , 1.f , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
	#endif
						rux::gui::engine::OpenGL::glMatrixMode( GL_MODELVIEW , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						rux::gui::engine::OpenGL::glLoadIdentity( __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
#if 0
						if( _framebuffer_ptr )
						{
							rux::gui::engine::OpenGL::glDeleteFramebuffers( 1 , _framebuffer_ptr , this , __FUNCTION__ , __LINE__ );
							rux::engine::free_mem( _framebuffer_ptr );
							_framebuffer_ptr = NULL;
						}
						if( _framebuffer_depth_stencil_ptr )
						{
							rux::gui::engine::OpenGL::glDeleteRenderbuffers( 1 , _framebuffer_depth_stencil_ptr , this , __FUNCTION__ , __LINE__ );
							rux::engine::free_mem( _framebuffer_depth_stencil_ptr );
							_framebuffer_depth_stencil_ptr = NULL;
						}
						if( _framebuffer_color_texture_ptr )
						{
							rux::gui::engine::OpenGL::glEnable( GL_TEXTURE_2D , this , __FUNCTION__ , __LINE__ );
							rux::gui::engine::OpenGL::glBindTexture( GL_TEXTURE_2D , 0 , __FUNCTION__ , __LINE__ );
							rux::gui::engine::OpenGL::glDeleteTextures( 1 , _framebuffer_color_texture_ptr , __FUNCTION__ , __LINE__ );
							rux::engine::free_mem( _framebuffer_color_texture_ptr );
							_framebuffer_color_texture_ptr = NULL;
							rux::gui::engine::OpenGL::glDisable( GL_TEXTURE_2D , this , __FUNCTION__ , __LINE__ );
						}
						if( _smooth_framebuffer_ptr )
						{
							rux::gui::engine::OpenGL::glDeleteFramebuffers( 1 , _smooth_framebuffer_ptr , this , __FUNCTION__ , __LINE__ );
							rux::engine::free_mem( _smooth_framebuffer_ptr );
							_smooth_framebuffer_ptr = NULL;
						}
						if( _smooth_framebuffer_depth_stencil_ptr )
						{
							rux::gui::engine::OpenGL::glDeleteRenderbuffers( 1 , _smooth_framebuffer_depth_stencil_ptr , this , __FUNCTION__ , __LINE__ );
							rux::engine::free_mem( _smooth_framebuffer_depth_stencil_ptr );
							_smooth_framebuffer_depth_stencil_ptr = NULL;
						}
						if( _smooth_framebuffer_color_texture_ptr )
						{
							rux::gui::engine::OpenGL::glEnable( GL_TEXTURE_2D , this , __FUNCTION__ , __LINE__ );
							rux::gui::engine::OpenGL::glBindTexture( GL_TEXTURE_2D , 0 , __FUNCTION__ , __LINE__ );
							rux::gui::engine::OpenGL::glDeleteTextures( 1 , _smooth_framebuffer_color_texture_ptr , __FUNCTION__ , __LINE__ );
							rux::engine::free_mem( _smooth_framebuffer_color_texture_ptr );
							_smooth_framebuffer_color_texture_ptr = NULL;
							rux::gui::engine::OpenGL::glDisable( GL_TEXTURE_2D , this , __FUNCTION__ , __LINE__ );
						}
						if( 0 && _is_support_framebuffer_object == 1 )
						{
							if( _framebuffer_color_texture_ptr == NULL )
							{
								_framebuffer_color_texture_ptr = alloc_array_macros( GLuint , 1 );
								rux::gui::engine::OpenGL::glGenTextures( 1 , _framebuffer_color_texture_ptr , __FUNCTION__ , __LINE__ );
								rux::gui::engine::OpenGL::glEnable( GL_TEXTURE_2D , this , __FUNCTION__ , __LINE__ );
								rux::gui::engine::OpenGL::glBindTexture( GL_TEXTURE_2D , _framebuffer_color_texture_ptr[ 0 ] , __FUNCTION__ , __LINE__ );
								rux::gui::engine::OpenGL::glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_NEAREST , __FUNCTION__ , __LINE__ );
								rux::gui::engine::OpenGL::glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_NEAREST , __FUNCTION__ , __LINE__ );
								rux::gui::engine::OpenGL::glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_CLAMP_TO_EDGE , __FUNCTION__ , __LINE__ );
								rux::gui::engine::OpenGL::glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , GL_CLAMP_TO_EDGE , __FUNCTION__ , __LINE__ );
								_framebuffer_color_texture_width = get_NumberPowerOf2( _width );
								_framebuffer_color_texture_height = get_NumberPowerOf2( _height );
								rux::gui::engine::OpenGL::glTexImage2D( GL_TEXTURE_2D , 0 , GL_RGBA , _framebuffer_color_texture_width , _framebuffer_color_texture_height , 0 , GL_RGBA , GL_UNSIGNED_BYTE , NULL , __FILE__ , __LINE__ );
								rux::gui::engine::OpenGL::glBindTexture( GL_TEXTURE_2D , 0 , __FUNCTION__ , __LINE__ );
								_framebuffer_depth_stencil_ptr = alloc_array_macros( GLuint , 1 );
								rux::gui::engine::OpenGL::glGenRenderbuffers( 1, _framebuffer_depth_stencil_ptr , this , __FUNCTION__ , __LINE__ );
								rux::gui::engine::OpenGL::glBindRenderbuffer( GL_RENDERBUFFER, _framebuffer_depth_stencil_ptr[ 0 ] , this , __FUNCTION__ , __LINE__ );
								rux::gui::engine::OpenGL::glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH24_STENCIL8 , _framebuffer_color_texture_width , _framebuffer_color_texture_height , this , __FUNCTION__ , __LINE__ );
								rux::gui::engine::OpenGL::glBindRenderbuffer( GL_RENDERBUFFER, 0 , this , __FUNCTION__ , __LINE__ );
								_framebuffer_ptr = alloc_array_macros( GLuint , 1 );
								rux::gui::engine::OpenGL::glGenFramebuffers( 1 , _framebuffer_ptr , this , __FUNCTION__ , __LINE__ );
								rux::gui::engine::OpenGL::glBindFramebuffer( GL_FRAMEBUFFER , _framebuffer_ptr[ 0 ] , this , __FUNCTION__ , __LINE__ );
								rux::gui::engine::OpenGL::glFramebufferTexture2D( GL_FRAMEBUFFER , GL_COLOR_ATTACHMENT0 , GL_TEXTURE_2D , _framebuffer_color_texture_ptr[ 0 ] , 0 , this , __FUNCTION__ , __LINE__ );
								rux::gui::engine::OpenGL::glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,_framebuffer_depth_stencil_ptr[ 0 ] , this , __FUNCTION__ , __LINE__ );
								rux::gui::engine::OpenGL::glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_STENCIL_ATTACHMENT,GL_RENDERBUFFER, _framebuffer_depth_stencil_ptr[ 0 ] , this , __FUNCTION__ , __LINE__ );
								GLenum status = ::rux::gui::engine::OpenGL::glCheckFramebufferStatus( GL_FRAMEBUFFER , this , __FUNCTION__ , __LINE__ );
								if( status == GL_FRAMEBUFFER_COMPLETE )
								{								
								}									
								else
								{
									_is_support_framebuffer_object = 0;
									_is_support_smooth = 0;
									switch( status )
									{
									case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
										{
											printf( "%s\n" , "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT" );
											break;
										}
									case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
										{
											printf( "%s\n" , "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT" );
											break;
										}
									case GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT:
										{
											printf( "%s\n" , "GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT" );
											break;
										}
									case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
										{
											printf( "%s\n" , "GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT" );
											break;
										}
									case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
										{
											printf( "%s\n" , "GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT" );
											break;
										}
	#ifdef __ANDROID__
	#else
									case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
										{
											printf( "%s\n" , "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER" );
											break;
										}
									case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
										{
											printf( "%s\n" , "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER" );
											break;
										}
	#endif
									case GL_FRAMEBUFFER_UNSUPPORTED:
										{
											printf( "%s\n" , "GL_FRAMEBUFFER_UNSUPPORTED" );
											break;
										}
									case GL_FRAMEBUFFER_STATUS_ERROR_EXT:
										{
											printf( "%s\n" , "GL_FRAMEBUFFER_STATUS_ERROR_EXT" );
											break;
										}
									default:
										{
											printf( "%s\n" , "Unknown ERROR" );
											break;
										};
									}
								}
								rux::gui::engine::OpenGL::glBindFramebuffer( GL_FRAMEBUFFER , 0 , this , __FUNCTION__ , __LINE__ );
								rux::gui::engine::OpenGL::glDisable( GL_TEXTURE_2D , this , __FUNCTION__ , __LINE__ );
							}			
							if( 0 && _smooth_framebuffer_color_texture_ptr == NULL && _is_support_framebuffer_object == 1 )
							{
								_smooth_framebuffer_color_texture_ptr = alloc_array_macros( GLuint , 1 );
								rux::gui::engine::OpenGL::glGenTextures( 1 , _smooth_framebuffer_color_texture_ptr , __FUNCTION__ , __LINE__ );
								rux::gui::engine::OpenGL::glEnable( GL_TEXTURE_2D , this , __FUNCTION__ , __LINE__ );
								rux::gui::engine::OpenGL::glBindTexture( GL_TEXTURE_2D , _smooth_framebuffer_color_texture_ptr[ 0 ] , __FUNCTION__ , __LINE__ );
								rux::gui::engine::OpenGL::glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_NEAREST , __FUNCTION__ , __LINE__ );
								rux::gui::engine::OpenGL::glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_NEAREST , __FUNCTION__ , __LINE__ );
								rux::gui::engine::OpenGL::glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_CLAMP_TO_EDGE , __FUNCTION__ , __LINE__ );
								rux::gui::engine::OpenGL::glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , GL_CLAMP_TO_EDGE , __FUNCTION__ , __LINE__ );
								_framebuffer_color_texture_width = get_NumberPowerOf2( _width );
								_framebuffer_color_texture_height = get_NumberPowerOf2( _height );
								rux::gui::engine::OpenGL::glTexImage2D( GL_TEXTURE_2D , 0 , GL_RGBA , _framebuffer_color_texture_width , _framebuffer_color_texture_height , 0 , GL_RGBA , GL_UNSIGNED_BYTE , NULL , __FILE__ , __LINE__ );
								rux::gui::engine::OpenGL::glBindTexture( GL_TEXTURE_2D , 0 , __FUNCTION__ , __LINE__ );
								_smooth_framebuffer_depth_stencil_ptr = alloc_array_macros( GLuint , 1 );
								rux::gui::engine::OpenGL::glGenRenderbuffers( 1, _smooth_framebuffer_depth_stencil_ptr , this , __FUNCTION__ , __LINE__ );
								rux::gui::engine::OpenGL::glBindRenderbuffer( GL_RENDERBUFFER, _smooth_framebuffer_depth_stencil_ptr[ 0 ] , this , __FUNCTION__ , __LINE__ );
								rux::gui::engine::OpenGL::glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH24_STENCIL8 , _framebuffer_color_texture_width , _framebuffer_color_texture_height , this , __FUNCTION__ , __LINE__ );
								rux::gui::engine::OpenGL::glBindRenderbuffer( GL_RENDERBUFFER, 0 , this , __FUNCTION__ , __LINE__ );
								_smooth_framebuffer_ptr = alloc_array_macros( GLuint , 1 );
								rux::gui::engine::OpenGL::glGenFramebuffers( 1 , _smooth_framebuffer_ptr , this , __FUNCTION__ , __LINE__ );
								rux::gui::engine::OpenGL::glBindFramebuffer( GL_FRAMEBUFFER , _smooth_framebuffer_ptr[ 0 ] , this , __FUNCTION__ , __LINE__ );
								rux::gui::engine::OpenGL::glFramebufferTexture2D( GL_FRAMEBUFFER , GL_COLOR_ATTACHMENT0 , GL_TEXTURE_2D , _smooth_framebuffer_color_texture_ptr[ 0 ] , 0 , this , __FUNCTION__ , __LINE__ );
								rux::gui::engine::OpenGL::glFramebufferRenderbuffer(GL_FRAMEBUFFER , GL_DEPTH_ATTACHMENT , GL_RENDERBUFFER , _smooth_framebuffer_depth_stencil_ptr[ 0 ] , this , __FUNCTION__ , __LINE__ );
								rux::gui::engine::OpenGL::glFramebufferRenderbuffer(GL_FRAMEBUFFER , GL_STENCIL_ATTACHMENT , GL_RENDERBUFFER , _smooth_framebuffer_depth_stencil_ptr[ 0 ] , this , __FUNCTION__ , __LINE__ );
								GLenum status = ::rux::gui::engine::OpenGL::glCheckFramebufferStatus( GL_FRAMEBUFFER , this , __FUNCTION__ , __LINE__ );
								if( status == GL_FRAMEBUFFER_COMPLETE )
								{
								}								
								else
								{
									_is_support_framebuffer_object = 0;
									_is_support_smooth = 0;
									switch( status )
									{
									case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
										{
											printf( "%s\n" , "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT" );
											break;
										}
									case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
										{
											printf( "%s\n" , "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT" );
											break;
										}
									case GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT:
										{
											printf( "%s\n" , "GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT" );
											break;
										}
									case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
										{
											printf( "%s\n" , "GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT" );
											break;
										}
									case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
										{
											printf( "%s\n" , "GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT" );
											break;
										}
	#ifdef __ANDROID__
	#else
									case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
										{
											printf( "%s\n" , "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER" );
											break;
										}
									case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
										{
											printf( "%s\n" , "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER" );
											break;
										}
	#endif
									case GL_FRAMEBUFFER_UNSUPPORTED:
										{
											printf( "%s\n" , "GL_FRAMEBUFFER_UNSUPPORTED" );
											break;
										}
									case GL_FRAMEBUFFER_STATUS_ERROR_EXT:
										{
											printf( "%s\n" , "GL_FRAMEBUFFER_STATUS_ERROR_EXT" );
											break;
										}
									default:
										{
											printf( "%s\n" , "Unknown ERROR" );
											break;
										};
									}
								}
								rux::gui::engine::OpenGL::glBindFramebuffer( GL_FRAMEBUFFER , 0 , this , __FUNCTION__ , __LINE__ );
								rux::gui::engine::OpenGL::glDisable( GL_TEXTURE_2D , this , __FUNCTION__ , __LINE__ );
							}			
							if( _is_support_framebuffer_object == 0 )
							{
								if( _framebuffer_ptr )
								{
									rux::gui::engine::OpenGL::glDeleteFramebuffers( 1 , _framebuffer_ptr , this , __FUNCTION__ , __LINE__ );
									rux::engine::free_mem( _framebuffer_ptr );
									_framebuffer_ptr = NULL;
								}
								if( _framebuffer_depth_stencil_ptr )
								{
									rux::gui::engine::OpenGL::glDeleteRenderbuffers( 1 , _framebuffer_depth_stencil_ptr , this , __FUNCTION__ , __LINE__ );
									rux::engine::free_mem( _framebuffer_depth_stencil_ptr );
									_framebuffer_depth_stencil_ptr = NULL;
								}
								if( _framebuffer_color_texture_ptr )
								{
									rux::gui::engine::OpenGL::glEnable( GL_TEXTURE_2D , this , __FUNCTION__ , __LINE__ );
									rux::gui::engine::OpenGL::glBindTexture( GL_TEXTURE_2D , 0 , __FUNCTION__ , __LINE__ );
									rux::gui::engine::OpenGL::glDeleteTextures( 1 , _framebuffer_color_texture_ptr , __FUNCTION__ , __LINE__ );
									rux::engine::free_mem( _framebuffer_color_texture_ptr );
									_framebuffer_color_texture_ptr = NULL;
									rux::gui::engine::OpenGL::glDisable( GL_TEXTURE_2D , this , __FUNCTION__ , __LINE__ );
								}
								if( _smooth_framebuffer_ptr )
								{
									rux::gui::engine::OpenGL::glDeleteFramebuffers( 1 , _smooth_framebuffer_ptr , this , __FUNCTION__ , __LINE__ );
									rux::engine::free_mem( _smooth_framebuffer_ptr );
									_smooth_framebuffer_ptr = NULL;
								}
								if( _smooth_framebuffer_depth_stencil_ptr )
								{
									rux::gui::engine::OpenGL::glDeleteRenderbuffers( 1 , _smooth_framebuffer_depth_stencil_ptr , this , __FUNCTION__ , __LINE__ );
									rux::engine::free_mem( _smooth_framebuffer_depth_stencil_ptr );
									_smooth_framebuffer_depth_stencil_ptr = NULL;
								}
								if( _smooth_framebuffer_color_texture_ptr )
								{
									rux::gui::engine::OpenGL::glEnable( GL_TEXTURE_2D , this , __FUNCTION__ , __LINE__ );
									rux::gui::engine::OpenGL::glBindTexture( GL_TEXTURE_2D , 0 , __FUNCTION__ , __LINE__ );
									rux::gui::engine::OpenGL::glDeleteTextures( 1 , _smooth_framebuffer_color_texture_ptr , __FUNCTION__ , __LINE__ );
									rux::engine::free_mem( _smooth_framebuffer_color_texture_ptr );
									_smooth_framebuffer_color_texture_ptr = NULL;
									rux::gui::engine::OpenGL::glDisable( GL_TEXTURE_2D , this , __FUNCTION__ , __LINE__ );
								}
							}
						}
#endif
					}
				}
			};
			void GLContext::SwapBuffers( size_t ___rux__thread_index1986 )
			{
				if( _current_context_window )
				{
#ifdef __WINDOWS__
					if( _current_context_window->_is_transparent_window == 0 )
					{
						BOOL res = FALSE;
						CODE_LABEL_ANOTHER_CONTEXT_PARAMETER_RESULT( NULL , "SwapBuffers" , ::SwapBuffers , _current_context_window->_hdc , res );
						if( res == FALSE )
							rux::gui::engine::OpenGL::write_error( "SwapBuffers FALSE\n" );
						else
						{
							if( ::rux::gui::engine::_use_gl_finish )
								::rux::gui::engine::OpenGL::glFinish( __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						}
					}
#elif defined( __UNIX__ )		
#ifdef __ANDROID__
#else
					rux::gui::engine::OpenGL::glXSwapBuffers( _current_context_window->_x_display , _current_context_window->_x_window , ___rux__thread_index1986 );
#endif
#endif		
				}
			};
			void GLContext::set_EmptyContext( void )
			{
				if( _current_context_thread_id != 0 )
				{
#ifdef __WINDOWS__
					if( ::rux::gui::engine::OpenGL::wglMakeCurrent( _current_context_window->_hdc , NULL ) == FALSE )
#elif defined( __UNIX__ )				
#ifdef __ANDROID__
#else
					if( ::rux::gui::engine::OpenGL::glXMakeCurrent( _current_context_window->_x_display , None , NULL ) == 0 )
#endif
#endif
					{
						GLenum error = ::rux::gui::engine::OpenGL::glGetError();
						printf( "wglMakeCurrent , OpenGL error=%u\n , context=NULL\n" , error );
					}
					else
						_current_context_thread_id = 0;
				}
			};
			void GLContext::set_CurrentContext( ::rux::gui::engine::Window* window )
			{
				if( _gl_context )
				{
					if( _current_context_thread_id == 0 )
					{	
						_current_context_window = window;
#ifdef __WINDOWS__
						if( ::rux::gui::engine::OpenGL::wglMakeCurrent( _current_context_window->_hdc , _gl_context ) == FALSE )
#elif defined( __UNIX__ )
#ifdef __ANDROID__
#else
						if( ::rux::gui::engine::OpenGL::glXMakeCurrent( _current_context_window->_x_display , _current_context_window->_x_window , _gl_context ) == 0 )
#endif
#endif
						{
							_current_context_window = 0;
							GLenum error = ::rux::gui::engine::OpenGL::glGetError();
							printf( "wglMakeCurrent , OpenGL error=%u\n , context=%p\n" , 
								error , _gl_context );
						}
						else
							_current_context_thread_id = ::rux::threading::XThread::get_CurrentThreadId();
					}
				}
			};
			void GLContext::delete_shader_and_program( ::rux::uint32& program , ::rux::uint32& fragment_shader , ::rux::uint32& vertex_shader , ::rux::byte attached , size_t ___rux__thread_index1986 )
			{
				if( program != 0 )
				{
					if( fragment_shader != 0 )
					{
						if( attached )
							rux::gui::engine::OpenGL::glDetachShader( program , fragment_shader , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						rux::gui::engine::OpenGL::glDeleteShader( fragment_shader , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						fragment_shader = 0;
					}
					if( vertex_shader != 0 )
					{
						if( attached )
							rux::gui::engine::OpenGL::glDetachShader( program , vertex_shader , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						rux::gui::engine::OpenGL::glDeleteShader( vertex_shader , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						vertex_shader = 0;
					}
					rux::gui::engine::OpenGL::glDeleteProgram( program , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					program = 0;
				}
			};
			GLint GLContext::check_shader_link_status( ::rux::uint32& program , ::rux::uint32& fragment_shader , ::rux::uint32& vertex_shader , const char* name , size_t ___rux__thread_index1986 )
			{
				GLint link_status = GL_TRUE;
				if( _glGetProgramiv )
				{
					::rux::gui::engine::OpenGL::glGetProgramiv( program , GL_LINK_STATUS , &link_status , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					if( link_status == GL_FALSE )
					{
						char s[ 32768 ] = {0} , log[ 32768 ] = {0};
						rux::gui::engine::OpenGL::glGetInfoLog( fragment_shader , 32768 , NULL , s , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						::rux::safe_sprintf( log , 32768 , "%s fragment shader link error: %s\n" , name , s );
						printf( "%s\n" , log );
						rux::gui::engine::OpenGL::write_error( log );
						delete_shader_and_program( program , fragment_shader , vertex_shader , 1 , ___rux__thread_index1986 );
					}
					int total = -1;
					::rux::gui::engine::OpenGL::glGetProgramiv( program , GL_ACTIVE_UNIFORMS , &total , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
				}
				return link_status;
			};
			GLint GLContext::check_shader_compile_status( ::rux::uint32& program , ::rux::uint32& fragment_shader , ::rux::uint32& vertex_shader , const char* name , size_t ___rux__thread_index1986 )
			{
				GLint compile_status = GL_TRUE;
				if( _glGetShaderiv )
				{
					::rux::gui::engine::OpenGL::glGetShaderiv( fragment_shader , GL_COMPILE_STATUS , &compile_status , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					if( compile_status == GL_FALSE )
					{
						char s[ 32768 ] = {0} , log[ 32768 ] = {0};
						rux::gui::engine::OpenGL::glGetInfoLog( fragment_shader , 32768 , NULL , s , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						::rux::safe_sprintf( log , 32768 , "%s fragment shader compile error: %s\n" , name , s );
						printf( "%s\n" , log );
						rux::gui::engine::OpenGL::write_error( log );
						delete_shader_and_program( program , fragment_shader , vertex_shader , 0 , ___rux__thread_index1986 );
					}
					if( compile_status != GL_FALSE && vertex_shader != 0 )
					{
						::rux::gui::engine::OpenGL::glGetShaderiv( vertex_shader , GL_COMPILE_STATUS , &compile_status , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						if( compile_status == GL_FALSE )
						{
							char s[ 32768 ] = {0} , log[ 32768 ] = {0};
							rux::gui::engine::OpenGL::glGetInfoLog( vertex_shader , 32768 , NULL , s , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							::rux::safe_sprintf( log , 32768 , "%s vertex shader compile error: %s\n" , name , s );
							printf( "%s\n" , log );
							rux::gui::engine::OpenGL::write_error( log );
							delete_shader_and_program( program , fragment_shader , vertex_shader , 0 , ___rux__thread_index1986 );
						}
					}
				}
#ifdef __ANDROID__
#else				
				else if( _glGetObjectParameteriv )
				{
					rux::gui::engine::OpenGL::glGetObjectParameteriv( fragment_shader , GL_COMPILE_STATUS , &compile_status , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					if( compile_status == GL_FALSE )
					{
						char s[ 32768 ] = {0} , log[ 32768 ] = {0};
						rux::gui::engine::OpenGL::glGetInfoLog( fragment_shader , 32768 , NULL , s , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						::rux::safe_sprintf( log , 32768 , "%s fragment shader compile error: %s\n" , name , s );
						printf( "%s\n" , log );
						rux::gui::engine::OpenGL::write_error( log );
						delete_shader_and_program( program , fragment_shader , vertex_shader , 0 , ___rux__thread_index1986 );
					}
					if( compile_status != GL_FALSE && vertex_shader != 0 )
					{
						rux::gui::engine::OpenGL::glGetObjectParameteriv( vertex_shader , GL_COMPILE_STATUS , &compile_status , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						if( compile_status == GL_FALSE )
						{
							char s[ 32768 ] = {0} , log[ 32768 ] = {0};
							rux::gui::engine::OpenGL::glGetInfoLog( vertex_shader , 32768 , NULL , s , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							::rux::safe_sprintf( log , 32768 , "%s vertex shader compile error: %s\n" , name , s );
							printf( "%s\n" , log );
							rux::gui::engine::OpenGL::write_error( log );
							delete_shader_and_program( program , fragment_shader , vertex_shader , 0 , ___rux__thread_index1986 );
						}
					}
				}
#endif	
				return compile_status;
			};
			::rux::uint8 GLContext::try_compile_fxaa_shader( size_t ___rux__thread_index1986 )
			{
				::rux::uint8 res = 0;
				if( _fxaa_program == 0 
					&& _fxaa_fragment_shader == 0 )
				{						
					_fxaa_program = ::rux::gui::engine::OpenGL::glCreateProgram( this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					_fxaa_fragment_shader = ::rux::gui::engine::OpenGL::glCreateShader( GL_FRAGMENT_SHADER , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					rux::gui::engine::OpenGL::glShaderSource( _fxaa_fragment_shader , 1 , &_rux_fxaa_fragment_shader , NULL , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					rux::gui::engine::OpenGL::glCompileShader( _fxaa_fragment_shader , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					//_fxaa_vertex_shader = ::rux::gui::engine::OpenGL::glCreateShader( GL_VERTEX_SHADER , this );
					//rux::gui::engine::OpenGL::glShaderSource( _fxaa_vertex_shader , 1 , &_rux_vertex_shader , NULL , this );
					//rux::gui::engine::OpenGL::glCompileShader( _fxaa_vertex_shader , this );
					if( check_shader_compile_status( _fxaa_program , _fxaa_fragment_shader , _fxaa_vertex_shader , "fxaa" , ___rux__thread_index1986 ) == GL_FALSE )
						res = 0;
					else
						res = 1;
					if( res )
					{
						rux::gui::engine::OpenGL::glAttachShader( _fxaa_program , _fxaa_fragment_shader , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						//rux::gui::engine::OpenGL::glAttachShader( _fxaa_program , _fxaa_vertex_shader , this );
						rux::gui::engine::OpenGL::glLinkProgram( _fxaa_program , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						if( check_shader_link_status( _fxaa_program , _fxaa_fragment_shader , _fxaa_vertex_shader , "fxaa" , ___rux__thread_index1986 ) == GL_FALSE )
							res = 0;
						else
							res = 1;
						if( res )
						{
							rux::gui::engine::OpenGL::glUseProgram( _fxaa_program , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							_fxaa_texture = ::rux::gui::engine::OpenGL::glGetUniformLocation( _fxaa_program , "texture" , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							_fxaa_vertex_width = ::rux::gui::engine::OpenGL::glGetUniformLocation( _fxaa_program , "vertex_width" , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							_fxaa_vertex_height = ::rux::gui::engine::OpenGL::glGetUniformLocation( _fxaa_program , "vertex_height" , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							_fxaa_background = ::rux::gui::engine::OpenGL::glGetUniformLocation( _fxaa_program , "texture1" , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						}
					}
				}
				return res;
			};
			::rux::uint8 GLContext::try_compile_yv12_to_rgb_3_textures_shader( size_t ___rux__thread_index1986 )
			{
				::rux::uint8 res = 0;
				if( _yv12_to_rgb_3_textures_program == 0 
					&& _yv12_to_rgb_3_textures_fragment_shader == 0 )
				{
					char* shader_text = load_shader_from_file( _shader_version_major_version > 1 || _shader_version_minor_version >= 30 ? "yv12_to_rgb_3_textures130.frag" : "yv12_to_rgb_3_textures.frag" );
					if( shader_text )
					{
						const GLchar* shaders[] = { shader_text };
						_yv12_to_rgb_3_textures_program = ::rux::gui::engine::OpenGL::glCreateProgram( this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						_yv12_to_rgb_3_textures_fragment_shader = ::rux::gui::engine::OpenGL::glCreateShader( GL_FRAGMENT_SHADER , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						rux::gui::engine::OpenGL::glShaderSource( _yv12_to_rgb_3_textures_fragment_shader , 1 , shaders , NULL , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						::rux::engine::free_mem( shader_text );
						rux::gui::engine::OpenGL::glCompileShader( _yv12_to_rgb_3_textures_fragment_shader , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						//_yv12_to_rgb_vertex_shader = ::rux::gui::engine::OpenGL::glCreateShader( GL_VERTEX_SHADER , this );
						//rux::gui::engine::OpenGL::glShaderSource( _yv12_to_rgb_vertex_shader , 1 , &_rux_vertex_shader , NULL , this );
						//rux::gui::engine::OpenGL::glCompileShader( _yv12_to_rgb_vertex_shader , this );
						if( check_shader_compile_status( _yv12_to_rgb_3_textures_program , _yv12_to_rgb_3_textures_fragment_shader 
							, _yv12_to_rgb_3_textures_vertex_shader , "yv12_to_rgb_3_textures" , ___rux__thread_index1986 ) == GL_FALSE )
							res = 0;
						else
							res = 1;
						if( res )
						{
							if( _shader_version_major_version > 1 || _shader_version_minor_version >= 30 )
								rux::gui::engine::OpenGL::glBindFragDataLocation( _yv12_to_rgb_3_textures_program , 0 , "out_frag_color" , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glAttachShader( _yv12_to_rgb_3_textures_program 
								, _yv12_to_rgb_3_textures_fragment_shader , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							//rux::gui::engine::OpenGL::glAttachShader( _yv12_to_rgb_program , _yv12_to_rgb_vertex_shader , this );
							rux::gui::engine::OpenGL::glLinkProgram( _yv12_to_rgb_3_textures_program , this , __FUNCTION__ , __LINE__ 
								, ___rux__thread_index1986 );
							if( check_shader_link_status( _yv12_to_rgb_3_textures_program , _yv12_to_rgb_3_textures_fragment_shader 
								, _yv12_to_rgb_3_textures_vertex_shader , "yv12_to_rgb_3_textures" , ___rux__thread_index1986 ) == GL_FALSE )
								res = 0;
							else
								res = 1;
							if( res )
							{
								rux::gui::engine::OpenGL::glUseProgram( _yv12_to_rgb_3_textures_program , this , __FUNCTION__ , __LINE__ 
									, ___rux__thread_index1986 );
								_yv12_to_rgb_3_textures_y_tex = ::rux::gui::engine::OpenGL::glGetUniformLocation( 
									_yv12_to_rgb_3_textures_program , "y_tex222" , this , __FUNCTION__ , __LINE__ 
									, ___rux__thread_index1986 );
								_yv12_to_rgb_3_textures_u_tex = ::rux::gui::engine::OpenGL::glGetUniformLocation( 
									_yv12_to_rgb_3_textures_program , "u_tex" , this , __FUNCTION__ , __LINE__ 
									, ___rux__thread_index1986 );
								_yv12_to_rgb_3_textures_v_tex = ::rux::gui::engine::OpenGL::glGetUniformLocation( 
									_yv12_to_rgb_3_textures_program , "v_tex" , this , __FUNCTION__ , __LINE__ 
									, ___rux__thread_index1986 );
								_yv12_to_rgb_3_textures_frame_width = ::rux::gui::engine::OpenGL::glGetUniformLocation( 
									_yv12_to_rgb_3_textures_program , "frame_width" , this , __FUNCTION__ , __LINE__ 
									, ___rux__thread_index1986 );
								_yv12_to_rgb_3_textures_frame_height = ::rux::gui::engine::OpenGL::glGetUniformLocation( 
									_yv12_to_rgb_3_textures_program , "frame_height" , this , __FUNCTION__ , __LINE__ 
									, ___rux__thread_index1986 );
								_yv12_to_rgb_3_textures_texture_width = ::rux::gui::engine::OpenGL::glGetUniformLocation( 
									_yv12_to_rgb_3_textures_program , "texture_width" , this , __FUNCTION__ , __LINE__ 
									, ___rux__thread_index1986 );
								_yv12_to_rgb_3_textures_texture_height = ::rux::gui::engine::OpenGL::glGetUniformLocation( 
									_yv12_to_rgb_3_textures_program , "texture_height" , this , __FUNCTION__ , __LINE__ 
									, ___rux__thread_index1986 );
								_yv12_to_rgb_3_textures_opacity = ::rux::gui::engine::OpenGL::glGetUniformLocation(
									_yv12_to_rgb_3_textures_program , "opacity" , this , __FUNCTION__ , __LINE__ , 
									___rux__thread_index1986 );
								_yv12_to_rgb_3_textures_brightness = ::rux::gui::engine::OpenGL::glGetUniformLocation(
									_yv12_to_rgb_3_textures_program , "brightness" , this , __FUNCTION__ , __LINE__ , 
									___rux__thread_index1986 );
								_yv12_to_rgb_3_textures_contrast = ::rux::gui::engine::OpenGL::glGetUniformLocation(
									_yv12_to_rgb_3_textures_program , "contrast" , this , __FUNCTION__ , __LINE__ , 
									___rux__thread_index1986 );
								rux::gui::engine::OpenGL::glUseProgram( 0 , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							}
						}
					}
				}			
				return res;
			};
			::rux::uint8 GLContext::try_compile_yv12_shader( size_t ___rux__thread_index1986 )
			{
				::rux::uint8 res = 0;
				if( _yv12_to_rgb_program == 0 
					&& _yv12_to_rgb_fragment_shader == 0 )
				{
					char* shader_text = load_shader_from_file( _shader_version_major_version > 1 || _shader_version_minor_version >= 30 ? "yv12_to_rgb130.frag" : "yv12_to_rgb.frag" );
					if( shader_text )
					{
						const GLchar* shaders[] = { shader_text };
						_yv12_to_rgb_program = ::rux::gui::engine::OpenGL::glCreateProgram( this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						_yv12_to_rgb_fragment_shader = ::rux::gui::engine::OpenGL::glCreateShader( GL_FRAGMENT_SHADER , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						rux::gui::engine::OpenGL::glShaderSource( _yv12_to_rgb_fragment_shader , 1 , shaders , NULL , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						::rux::engine::free_mem( shader_text );
						rux::gui::engine::OpenGL::glCompileShader( _yv12_to_rgb_fragment_shader , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						//_yv12_to_rgb_vertex_shader = ::rux::gui::engine::OpenGL::glCreateShader( GL_VERTEX_SHADER , this );
						//rux::gui::engine::OpenGL::glShaderSource( _yv12_to_rgb_vertex_shader , 1 , &_rux_vertex_shader , NULL , this );
						//rux::gui::engine::OpenGL::glCompileShader( _yv12_to_rgb_vertex_shader , this );
						if( check_shader_compile_status( _yv12_to_rgb_program , _yv12_to_rgb_fragment_shader , _yv12_to_rgb_vertex_shader , "yv12_to_rgb" , ___rux__thread_index1986 ) == GL_FALSE )
							res = 0;
						else
							res = 1;
						if( res )
						{
							if( _shader_version_major_version > 1 || _shader_version_minor_version >= 30 )
								rux::gui::engine::OpenGL::glBindFragDataLocation( _yv12_to_rgb_program , 0 , "out_frag_color" , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glAttachShader( _yv12_to_rgb_program , _yv12_to_rgb_fragment_shader , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							//rux::gui::engine::OpenGL::glAttachShader( _yv12_to_rgb_program , _yv12_to_rgb_vertex_shader , this );
							rux::gui::engine::OpenGL::glLinkProgram( _yv12_to_rgb_program , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							if( check_shader_link_status( _yv12_to_rgb_program , _yv12_to_rgb_fragment_shader , _yv12_to_rgb_vertex_shader , "yv12_to_rgb" , ___rux__thread_index1986 ) == GL_FALSE )
								res = 0;
							else
								res = 1;
							if( res )
							{
								rux::gui::engine::OpenGL::glUseProgram( _yv12_to_rgb_program , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								_yv12_to_rgb_frame_width = ::rux::gui::engine::OpenGL::glGetUniformLocation( _yv12_to_rgb_program , "frame_width" , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								_yv12_to_rgb_frame_height = ::rux::gui::engine::OpenGL::glGetUniformLocation( _yv12_to_rgb_program , "frame_height" , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								_yv12_to_rgb_frame_texture = ::rux::gui::engine::OpenGL::glGetUniformLocation( _yv12_to_rgb_program , "yv12_tex" , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								_yv12_to_rgb_frame_opacity = ::rux::gui::engine::OpenGL::glGetUniformLocation( _yv12_to_rgb_program , "opacity" , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								_yv12_to_rgb_frame_brightness = ::rux::gui::engine::OpenGL::glGetUniformLocation( _yv12_to_rgb_program , "brightness" , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								_yv12_to_rgb_frame_contrast = ::rux::gui::engine::OpenGL::glGetUniformLocation( _yv12_to_rgb_program , "contrast" , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								rux::gui::engine::OpenGL::glUseProgram( 0 , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							}
						}
					}
				}			
				return res;
			};
			::rux::uint8 GLContext::try_compile_yv12_old_shader( size_t ___rux__thread_index1986 )
			{
				::rux::uint8 res = 0;
				if( _yv12_to_rgb_program == 0 
					&& _yv12_to_rgb_fragment_shader == 0 )
				{
					char* shader_text = load_shader_from_file( _shader_version_major_version > 1 || _shader_version_minor_version >= 30 ? "old_yv12_to_rgb130.frag" : "old_yv12_to_rgb.frag" );
					if( shader_text )
					{
						const GLchar* shaders[] = { shader_text };
						_yv12_to_rgb_program = ::rux::gui::engine::OpenGL::glCreateProgram( this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						_yv12_to_rgb_fragment_shader = ::rux::gui::engine::OpenGL::glCreateShader( GL_FRAGMENT_SHADER , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						rux::gui::engine::OpenGL::glShaderSource( _yv12_to_rgb_fragment_shader , 1 , shaders , NULL , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						::rux::engine::free_mem( shader_text );
						rux::gui::engine::OpenGL::glCompileShader( _yv12_to_rgb_fragment_shader , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						//_yv12_to_rgb_vertex_shader = ::rux::gui::engine::OpenGL::glCreateShader( GL_VERTEX_SHADER , this );
						//rux::gui::engine::OpenGL::glShaderSource( _yv12_to_rgb_vertex_shader , 1 , &_rux_vertex_shader , NULL , this );
						//rux::gui::engine::OpenGL::glCompileShader( _yv12_to_rgb_vertex_shader , this );
						if( check_shader_compile_status( _yv12_to_rgb_program , _yv12_to_rgb_fragment_shader , _yv12_to_rgb_vertex_shader , "old yv12_to_rgb" , ___rux__thread_index1986 ) == GL_FALSE )
							res = 0;
						else
							res = 1;
						if( res )
						{
							if( _shader_version_major_version > 1 || _shader_version_minor_version >= 30 )
								rux::gui::engine::OpenGL::glBindFragDataLocation( _yv12_to_rgb_program , 0 , "out_frag_color" , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glAttachShader( _yv12_to_rgb_program , _yv12_to_rgb_fragment_shader , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							//rux::gui::engine::OpenGL::glAttachShader( _yv12_to_rgb_program , _yv12_to_rgb_vertex_shader , this );
							rux::gui::engine::OpenGL::glLinkProgram( _yv12_to_rgb_program , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							if( check_shader_link_status( _yv12_to_rgb_program , _yv12_to_rgb_fragment_shader , _yv12_to_rgb_vertex_shader , "old yv12_to_rgb" , ___rux__thread_index1986 ) == GL_FALSE )
								res = 0;
							else
								res = 1;
							if( res )
							{
								rux::gui::engine::OpenGL::glUseProgram( _yv12_to_rgb_program , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								_yv12_to_rgb_frame_width = ::rux::gui::engine::OpenGL::glGetUniformLocation( _yv12_to_rgb_program , "frame_width" , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								_yv12_to_rgb_frame_height = ::rux::gui::engine::OpenGL::glGetUniformLocation( _yv12_to_rgb_program , "frame_height" , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								_yv12_to_rgb_frame_texture = ::rux::gui::engine::OpenGL::glGetUniformLocation( _yv12_to_rgb_program , "yv12_tex" , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								_yv12_to_rgb_frame_opacity = ::rux::gui::engine::OpenGL::glGetUniformLocation( _yv12_to_rgb_program , "opacity" , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								_yv12_to_rgb_frame_brightness = ::rux::gui::engine::OpenGL::glGetUniformLocation( _yv12_to_rgb_program , "brightness" , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								_yv12_to_rgb_frame_contrast = ::rux::gui::engine::OpenGL::glGetUniformLocation( _yv12_to_rgb_program , "contrast" , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								rux::gui::engine::OpenGL::glUseProgram( 0 , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							}
						}
					}
				}
				return res;			
			};
			::rux::uint8 GLContext::try_compile_yuyv_shader( size_t ___rux__thread_index1986 )
			{
				::rux::uint8 res = 0;
				if( _yuyv_to_rgb_program == 0 
					&& _yuyv_to_rgb_fragment_shader == 0 )
				{
					char* shader_text = load_shader_from_file( _shader_version_major_version > 1 || _shader_version_minor_version >= 30 ? "yuyv_to_rgb130.frag" : "yuyv_to_rgb.frag" );
					if( shader_text )
					{
						const GLchar* shaders[] = { shader_text };
						_yuyv_to_rgb_program = ::rux::gui::engine::OpenGL::glCreateProgram( this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						_yuyv_to_rgb_fragment_shader = ::rux::gui::engine::OpenGL::glCreateShader( GL_FRAGMENT_SHADER , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						rux::gui::engine::OpenGL::glShaderSource( _yuyv_to_rgb_fragment_shader , 1 , shaders , NULL , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						::rux::engine::free_mem( shader_text );
						rux::gui::engine::OpenGL::glCompileShader( _yuyv_to_rgb_fragment_shader , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						//_yuyv_to_rgb_vertex_shader = ::rux::gui::engine::OpenGL::glCreateShader( GL_VERTEX_SHADER , this );
						//rux::gui::engine::OpenGL::glShaderSource( _yuyv_to_rgb_vertex_shader , 1 , &_rux_vertex_shader , NULL , this );
						//rux::gui::engine::OpenGL::glCompileShader( _yuyv_to_rgb_vertex_shader , this );
						if( check_shader_compile_status( _yuyv_to_rgb_program , _yuyv_to_rgb_fragment_shader , _yuyv_to_rgb_vertex_shader , "yuyv_to_rgb" , ___rux__thread_index1986 ) == GL_FALSE )
							res = 0;
						else
							res = 1;
						if( res )
						{
							if( _shader_version_major_version > 1 || _shader_version_minor_version >= 30 )
								rux::gui::engine::OpenGL::glBindFragDataLocation( _yuyv_to_rgb_program , 0 , "out_frag_color" , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glAttachShader( _yuyv_to_rgb_program , _yuyv_to_rgb_fragment_shader , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							//rux::gui::engine::OpenGL::glAttachShader( _yuyv_to_rgb_program , _yuyv_to_rgb_vertex_shader , this );
							rux::gui::engine::OpenGL::glLinkProgram( _yuyv_to_rgb_program , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							if( check_shader_link_status( _yuyv_to_rgb_program , _yuyv_to_rgb_fragment_shader , _yuyv_to_rgb_vertex_shader , "yuyv_to_rgb" , ___rux__thread_index1986 ) == GL_FALSE )
								res = 0;
							else
								res = 1;
							if( res )
							{
								rux::gui::engine::OpenGL::glUseProgram( _yuyv_to_rgb_program , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								_yuyv_to_rgb_frame_width = ::rux::gui::engine::OpenGL::glGetUniformLocation( _yuyv_to_rgb_program , "frame_width" , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								_yuyv_to_rgb_frame_height = ::rux::gui::engine::OpenGL::glGetUniformLocation( _yuyv_to_rgb_program , "frame_height" , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								_yuyv_to_rgb_frame_texture = ::rux::gui::engine::OpenGL::glGetUniformLocation( _yuyv_to_rgb_program , "yuyv_tex" , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								_yuyv_to_rgb_frame_opacity = ::rux::gui::engine::OpenGL::glGetUniformLocation( _yuyv_to_rgb_program , "opacity" , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								_yuyv_to_rgb_frame_brightness = ::rux::gui::engine::OpenGL::glGetUniformLocation( _yuyv_to_rgb_program , "brightness" , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								_yuyv_to_rgb_frame_contrast = ::rux::gui::engine::OpenGL::glGetUniformLocation( _yuyv_to_rgb_program , "contrast" , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								rux::gui::engine::OpenGL::glUseProgram( 0 , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							}
						}
					}
				}
				return res;
			};
			::rux::uint8 GLContext::try_compile_rgba_shader( size_t ___rux__thread_index1986 )
			{
				::rux::uint8 res = 0;
				if( _rgba_program == 0 
					&& _rgba_fragment_shader == 0 )
				{
					char* shader_text = load_shader_from_file( _shader_version_major_version > 1 || _shader_version_minor_version >= 30 ? "rgba130.frag" : "rgba.frag" );
					if( shader_text )
					{
						const GLchar* shaders[] = { shader_text };
						_rgba_program = ::rux::gui::engine::OpenGL::glCreateProgram( this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						_rgba_fragment_shader = ::rux::gui::engine::OpenGL::glCreateShader( GL_FRAGMENT_SHADER , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						rux::gui::engine::OpenGL::glShaderSource( _rgba_fragment_shader , 1 , shaders , NULL , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						::rux::engine::free_mem( shader_text );
						rux::gui::engine::OpenGL::glCompileShader( _rgba_fragment_shader , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						//_rgba_vertex_shader = ::rux::gui::engine::OpenGL::glCreateShader( GL_VERTEX_SHADER , this );
						//rux::gui::engine::OpenGL::glShaderSource( _rgba_vertex_shader , 1 , &_rux_vertex_shader , NULL , this );
						//rux::gui::engine::OpenGL::glCompileShader( _rgba_vertex_shader , this );
						if( check_shader_compile_status( _rgba_program , _rgba_fragment_shader , _rgba_vertex_shader , "rgba" , ___rux__thread_index1986 ) == GL_FALSE )
							res = 0;
						else
							res = 1;
						if( res )
						{
							if( _shader_version_major_version > 1 || _shader_version_minor_version >= 30 )
								rux::gui::engine::OpenGL::glBindFragDataLocation( _rgba_program , 0 , "out_frag_color" , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glAttachShader( _rgba_program , _rgba_fragment_shader , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							//rux::gui::engine::OpenGL::glAttachShader( _rgba_program , _rgba_vertex_shader , this );
							rux::gui::engine::OpenGL::glLinkProgram( _rgba_program , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							if( check_shader_link_status( _rgba_program , _rgba_fragment_shader , _rgba_vertex_shader , "rgba" , ___rux__thread_index1986 ) == GL_FALSE )
								res = 0;
							else
								res = 1;
							if( res )
							{
								rux::gui::engine::OpenGL::glUseProgram( _rgba_program , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								_rgba_frame_width = ::rux::gui::engine::OpenGL::glGetUniformLocation( _rgba_program , "frame_width" , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								_rgba_frame_height = ::rux::gui::engine::OpenGL::glGetUniformLocation( _rgba_program , "frame_height" , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								_rgba_frame_texture = ::rux::gui::engine::OpenGL::glGetUniformLocation( _rgba_program , "rgba_tex" , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								_rgba_frame_opacity = ::rux::gui::engine::OpenGL::glGetUniformLocation( _rgba_program , "opacity" , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								_rgba_frame_brightness = ::rux::gui::engine::OpenGL::glGetUniformLocation( _rgba_program , "brightness" , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								_rgba_frame_contrast = ::rux::gui::engine::OpenGL::glGetUniformLocation( _rgba_program , "contrast" , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								rux::gui::engine::OpenGL::glUseProgram( 0 , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							}
						}
					}
				}
				return res;
			};
			::rux::uint8 GLContext::try_compile_rgb_shader( size_t ___rux__thread_index1986 )
			{
				::rux::uint8 res = 0;
				if( _rgb_program == 0 
					&& _rgb_fragment_shader == 0 )
				{
					char* shader_text = load_shader_from_file( _shader_version_major_version > 1 || _shader_version_minor_version >= 30 ? "rgb130.frag" : "rgb.frag" );
					if( shader_text )
					{
						const GLchar* shaders[] = { shader_text };
						_rgb_program = ::rux::gui::engine::OpenGL::glCreateProgram( this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						_rgb_fragment_shader = ::rux::gui::engine::OpenGL::glCreateShader( GL_FRAGMENT_SHADER , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						rux::gui::engine::OpenGL::glShaderSource( _rgb_fragment_shader , 1 , shaders , NULL , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						::rux::engine::free_mem( shader_text );
						rux::gui::engine::OpenGL::glCompileShader( _rgb_fragment_shader , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						//_rgb_vertex_shader = ::rux::gui::engine::OpenGL::glCreateShader( GL_VERTEX_SHADER , this );
						//rux::gui::engine::OpenGL::glShaderSource( _rgb_vertex_shader , 1 , &_rux_vertex_shader , NULL , this );
						//rux::gui::engine::OpenGL::glCompileShader( _rgb_vertex_shader , this );
						if( check_shader_compile_status( _rgb_program , _rgb_fragment_shader , _rgb_vertex_shader , "rgb" , ___rux__thread_index1986 ) == GL_FALSE )
							res = 0;
						else
							res = 1;
						if( res )
						{
							if( _shader_version_major_version > 1 || _shader_version_minor_version >= 30 )
								rux::gui::engine::OpenGL::glBindFragDataLocation( _rgb_program , 0 , "out_frag_color" , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glAttachShader( _rgb_program , _rgb_fragment_shader , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							//rux::gui::engine::OpenGL::glAttachShader( _rgb_program , _rgb_vertex_shader , this );
							rux::gui::engine::OpenGL::glLinkProgram( _rgb_program , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							if( check_shader_link_status( _rgb_program , _rgb_fragment_shader , _rgb_vertex_shader , "rgb" , ___rux__thread_index1986 ) == GL_FALSE )
								res = 0;
							else
								res = 1;
							if( res )
							{
								rux::gui::engine::OpenGL::glUseProgram( _rgb_program , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								_rgb_frame_width = ::rux::gui::engine::OpenGL::glGetUniformLocation( _rgb_program , "frame_width" , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								_rgb_frame_height = ::rux::gui::engine::OpenGL::glGetUniformLocation( _rgb_program , "frame_height" , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								_rgb_frame_opacity = ::rux::gui::engine::OpenGL::glGetUniformLocation( _rgb_program , "opacity" , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								_rgb_frame_texture = ::rux::gui::engine::OpenGL::glGetUniformLocation( _rgb_program , "rgb_tex" , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								_rgb_frame_brightness = ::rux::gui::engine::OpenGL::glGetUniformLocation( _rgb_program , "brightness" , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								_rgb_frame_contrast = ::rux::gui::engine::OpenGL::glGetUniformLocation( _rgb_program , "contrast" , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								rux::gui::engine::OpenGL::glUseProgram( 0 , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							}
						}
					}
				}
				return res;
			};
			char* GLContext::load_shader_from_file( const char* shader_filename )
			{
				char* shader_text = NULL;
				declare_stack_variable( char , shader_file , 1024 );
				::rux::safe_sprintf( shader_file , 1024 , "%s/shaders/%s" , ::rux::engine::_globals->_executable_directory , shader_filename );
				::rux::io::file file( shader_file );
				if( file.opened() )
				{
					::rux::uint64 size = file.size();
					if( size <= SIZE_MAX )
					{
						size_t readen = 0;
						shader_text = alloc_array_macros( char , (size_t)size + 1 );
						if( file.read_text( shader_text , size + 1 , readen ) == false )
						{							
							::rux::engine::free_mem( shader_text );
							shader_text = NULL;
						}
					}
					file.close();
				}
				return shader_text;
			};
			::rux::uint8 GLContext::try_compile_simple_rectangle_shader( size_t ___rux__thread_index1986 )
			{
				::rux::uint8 res = 0;
				if( _simple_rectangle_program == 0 
					&& _simple_rectangle_fragment_shader == 0 )
				{
					char* shader_text = load_shader_from_file( _shader_version_major_version > 1 || _shader_version_minor_version >= 30 ? "rux_shader_simple_rectangle130.frag" : "rux_shader_simple_rectangle.frag" );
					if( shader_text )
					{
						const GLchar* shaders[] = { shader_text };
						_simple_rectangle_program = ::rux::gui::engine::OpenGL::glCreateProgram( this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						_simple_rectangle_fragment_shader = ::rux::gui::engine::OpenGL::glCreateShader( GL_FRAGMENT_SHADER , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						rux::gui::engine::OpenGL::glShaderSource( _simple_rectangle_fragment_shader , 1 , shaders , NULL , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						::rux::engine::free_mem( shader_text );
						rux::gui::engine::OpenGL::glCompileShader( _simple_rectangle_fragment_shader , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						//_simple_rectangle_vertex_shader = ::rux::gui::engine::OpenGL::glCreateShader( GL_VERTEX_SHADER , this );
						//rux::gui::engine::OpenGL::glShaderSource( _simple_rectangle_vertex_shader , 1 , &_rux_vertex_shader , NULL , this );
						//rux::gui::engine::OpenGL::glCompileShader( _simple_rectangle_vertex_shader , this );
						if( check_shader_compile_status( _simple_rectangle_program , _simple_rectangle_fragment_shader , _simple_rectangle_vertex_shader , "simple_rectangle" , ___rux__thread_index1986 ) == GL_FALSE )
							res = 0;
						else
							res = 1;
						if( res )
						{
							if( _shader_version_major_version > 1 || _shader_version_minor_version >= 30 )
								rux::gui::engine::OpenGL::glBindFragDataLocation( _simple_rectangle_program , 0 , "out_frag_color" , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glAttachShader( _simple_rectangle_program , _simple_rectangle_fragment_shader , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							//rux::gui::engine::OpenGL::glAttachShader( _simple_rectangle_program , _simple_rectangle_vertex_shader , this );
							rux::gui::engine::OpenGL::glLinkProgram( _simple_rectangle_program , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							if( check_shader_link_status( _simple_rectangle_program , _simple_rectangle_fragment_shader , _simple_rectangle_vertex_shader , "simple_rectangle" , ___rux__thread_index1986 ) == GL_FALSE )
								res = 0;
							else
								res = 1;
							if( res )
							{
								rux::gui::engine::OpenGL::glUseProgram( _simple_rectangle_program , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );

								_simple_rectangle_shader_common = ::rux::gui::engine::OpenGL::glGetUniformLocation( _simple_rectangle_program , "common_texture1d" , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								_simple_rectangle_shader_texture_width = ::rux::gui::engine::OpenGL::glGetUniformLocation( _simple_rectangle_program , "texture_width" , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );

								rux::gui::engine::OpenGL::glUseProgram( 0 , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							}
						}
					}
					else
						res = 0;
				}
				return res;
			};
			::rux::uint8 GLContext::try_compile_yuyv_old_shader( size_t ___rux__thread_index1986 )
			{
				::rux::uint8 res = 0;
				if( _yuyv_to_rgb_program == 0
					&& _yuyv_to_rgb_fragment_shader == 0 )
				{
					char* shader_text = load_shader_from_file( _shader_version_major_version > 1 || _shader_version_minor_version >= 30 ? "old_yuyv_to_rgb130.frag" : "old_yuyv_to_rgb.frag" );
					if( shader_text )
					{
						const GLchar* shaders[] = { shader_text };
						_yuyv_to_rgb_program = ::rux::gui::engine::OpenGL::glCreateProgram( this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						_yuyv_to_rgb_fragment_shader = ::rux::gui::engine::OpenGL::glCreateShader( GL_FRAGMENT_SHADER , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						rux::gui::engine::OpenGL::glShaderSource( _yuyv_to_rgb_fragment_shader , 1 , shaders , NULL , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						::rux::engine::free_mem( shader_text );
						rux::gui::engine::OpenGL::glCompileShader( _yuyv_to_rgb_fragment_shader , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						//_yuyv_to_rgb_vertex_shader = ::rux::gui::engine::OpenGL::glCreateShader( GL_VERTEX_SHADER , this );
						//rux::gui::engine::OpenGL::glShaderSource( _yuyv_to_rgb_vertex_shader , 1 , &_rux_vertex_shader , NULL , this );
						//rux::gui::engine::OpenGL::glCompileShader( _yuyv_to_rgb_vertex_shader , this );
						if( check_shader_compile_status( _yuyv_to_rgb_program , _yuyv_to_rgb_fragment_shader , _yuyv_to_rgb_vertex_shader , "old yuyv to rgb" , ___rux__thread_index1986 ) == GL_FALSE )
							res = 0;
						else
							res = 1;
						if( res )
						{
							if( _shader_version_major_version > 1 || _shader_version_minor_version >= 30 )
								rux::gui::engine::OpenGL::glBindFragDataLocation( _yuyv_to_rgb_program , 0 , "out_frag_color" , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glAttachShader( _yuyv_to_rgb_program , _yuyv_to_rgb_fragment_shader , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							//rux::gui::engine::OpenGL::glAttachShader( _yuyv_to_rgb_program , _yuyv_to_rgb_vertex_shader , this );
							rux::gui::engine::OpenGL::glLinkProgram( _yuyv_to_rgb_program , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							if( check_shader_link_status( _yuyv_to_rgb_program , _yuyv_to_rgb_fragment_shader , _yuyv_to_rgb_vertex_shader , "old yuyv to rgb" , ___rux__thread_index1986 ) == GL_FALSE )
								res = 0;
							else
								res = 1;
							if( res )
							{
								rux::gui::engine::OpenGL::glUseProgram( _yuyv_to_rgb_program , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								_yuyv_to_rgb_frame_width = ::rux::gui::engine::OpenGL::glGetUniformLocation( _yuyv_to_rgb_program , "frame_width" , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								_yuyv_to_rgb_frame_height = ::rux::gui::engine::OpenGL::glGetUniformLocation( _yuyv_to_rgb_program , "frame_height" , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								_yuyv_to_rgb_frame_texture = ::rux::gui::engine::OpenGL::glGetUniformLocation( _yuyv_to_rgb_program , "yuyv_tex" , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								_yuyv_to_rgb_frame_opacity = ::rux::gui::engine::OpenGL::glGetUniformLocation( _yuyv_to_rgb_program , "opacity" , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								_yuyv_to_rgb_frame_brightness = ::rux::gui::engine::OpenGL::glGetUniformLocation( _yuyv_to_rgb_program , "brightness" , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								_yuyv_to_rgb_frame_contrast = ::rux::gui::engine::OpenGL::glGetUniformLocation( _yuyv_to_rgb_program , "contrast" , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								rux::gui::engine::OpenGL::glUseProgram( 0 , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							}
						}
					}
				}
				return res;
			};
			rux::uint8 GLContext::IsSupportVBO( size_t ___rux__thread_index1986 )
			{
#if TURNOFF_VBO
				return 0;
#else
				rux::uint8 check_unpack_buffer = 0;
				if( _glBindBuffer )
				{
					_glBindBuffer( GL_PIXEL_UNPACK_BUFFER , 0 );
					GLenum gl_error = rux::gui::engine::_glGetError();
					if( gl_error == 0 )
						check_unpack_buffer = 1;
				}
				return check_unpack_buffer == 1
					&& IsSupportExtension( "GL_ARB_vertex_buffer_object" , ___rux__thread_index1986 ) == 1 
					&& _glBindBuffer
					&& _glGenBuffers
					&& _glBufferData
					&& _glMapBuffer
					&& _glUnmapBuffer ? 1 : 0;
#endif
			};
#define TURNOFF_FrameBufferObject 0
			rux::uint8 GLContext::IsSupportFrameBufferObject( size_t ___rux__thread_index1986 )
			{
#if TURNOFF_FrameBufferObject
				return 0;
#else
				return IsSupportExtension( "GL_EXT_framebuffer_object" , ___rux__thread_index1986 ) == 1 
					&& _glBindFramebuffer
					&& _glDeleteFramebuffers
					&& _glGenFramebuffers
					&& _glGenerateMipmap
					&& _glCheckFramebufferStatus
					&& _glFramebufferTexture2D 
					&& _glBindRenderbuffer
					&& _glDeleteRenderbuffers
					&& _glGenRenderbuffers
					&& _glRenderbufferStorage
					&& _glFramebufferRenderbuffer ? 1 : 0;
#endif
			};

			rux::uint8 GLContext::IsSupportShaders( void )
			{
#if TURNOFF_Shaders
				return 0;
#else
				return _glBindBuffer && _glGenBuffers && 
					_glBufferData && _glCreateProgram &&
					_glCreateShader && _glCompileShader &&
					_glShaderSource && _glAttachShader &&
					_glLinkProgram && _glUseProgram &&
					_glGetUniformLocation && _glActiveTexture &&
					_glMapBuffer && _glUnmapBuffer &&
					_glUniform1f && _glUniform1i && _glUniform4f ? 1 : 0;
#endif
			};
			GLenum GLContext::get_Format( rux::int32 fourcc , int16 bit_count )
			{
				if( fourcc == rux::media::XEnum_Fourcc_GREY
					|| fourcc == rux::media::XEnum_Fourcc_Y800
					|| fourcc == rux::media::XEnum_Fourcc_Y8 )
					return GL_LUMINANCE;
				else if( fourcc == rux::media::XEnum_Fourcc_YV12 && ( _is_supported_yv12_to_rgb_shader == 1 || _is_supported_old_yv12_to_rgb_shader == 1 ) )
					return GL_LUMINANCE;
				else if( fourcc == rux::media::XEnum_Fourcc_YUYV && ( _is_supported_yuyv_to_rgb_shader == 1 || _is_supported_old_yuyv_to_rgb_shader == 1 ) )
					return GL_LUMINANCE_ALPHA;
				else if( fourcc == rux::media::XEnum_Fourcc_BGR3 )
				{
					if( bit_count == 24 )
#ifdef __ANDROID__
						return GL_RGB;
#else
						return GL_BGR_EXT;
#endif
					else if( bit_count == 32 )
						return GL_BGRA_EXT;
					else
						return GL_RGB;
				}
				else if( fourcc == rux::media::XEnum_Fourcc_RGB	|| fourcc == rux::media::XEnum_Fourcc_BI_RGB )
				{
					if( bit_count == 24 )
						return GL_RGB;
					else if( bit_count == 32 )
						return GL_RGBA;
					else
						return GL_RGBA;
				}
				else
					return GL_RGBA;
			};
			rux::uint8 GLContext::IsSupportExtension( const char* extension , size_t ___rux__thread_index1986 )
			{
				::rux::log::WriteDebug( "%s:%d BEGIN" , __FUNCTION__ , __LINE__ );
				if( _gl_extensions.Count() == 0 )
				{
					declare_stack_variable( char , filename , 1024 );
					::rux::safe_strncpy( filename , ::rux::engine::_globals->_executable_directory , 1024 );
					rux_concatenation( filename , "/rux_logs/OpenGL.txt" );		
					::rux::io::file iofile( filename , XOpenWriteText );
					if( iofile.opened() == false )
						iofile.open( filename , XCreateWriteText );
					if( iofile.opened() )
						iofile.size( 0 );
					_gl_version = (char*)rux::gui::engine::OpenGL::glGetString( GL_VERSION );
					_gl_renderer = (char*)rux::gui::engine::OpenGL::glGetString ( GL_RENDERER );
					_gl_shader_version = (char*)rux::gui::engine::OpenGL::glGetString( GL_SHADING_LANGUAGE_VERSION );

					const ::rux::byte* _gl_shader_version_ptr = (::rux::byte*)_gl_shader_version;
					if( _gl_shader_version_ptr )
					{
						_shader_version_major_version = ::rux::utils::decimal_string_to_number< ::rux::uint32 >( _gl_shader_version_ptr );
						_gl_shader_version_ptr++;
						_shader_version_minor_version = ::rux::utils::decimal_string_to_number< ::rux::uint32 >( _gl_shader_version_ptr );
					}

					printf( "OpenGL renderer = '%s'\nOpenGL version = '%s'\nShader version = '%s'\n" ,
						_gl_renderer ,
						_gl_version ,
						_gl_shader_version );
#ifdef __WINDOWS__
					PROC wglGetExtString = glGetProcAddress("wglGetExtensionsStringARB"); 
					const char* wgl_extensions = NULL;
					if( wglGetExtString )
						wgl_extensions = ( (char* ( __stdcall* )( HDC ) )wglGetExtString )( rux::gui::engine::OpenGL::wglGetCurrentDC() );					
					const char* extensions = (char*)rux::gui::engine::OpenGL::glGetString( GL_EXTENSIONS );
					_gl_extensions_string = alloc_array_macros( char , 
						( wgl_extensions ? strlen( wgl_extensions ) : 0 ) +
						( extensions ? strlen( extensions ) : 0 ) + 1 );
					if( wgl_extensions && strlen( wgl_extensions ) > 0 )
						memcpy( _gl_extensions_string , wgl_extensions , strlen( wgl_extensions ) );
					if( extensions && strlen( extensions ) > 0 )
						memcpy( &_gl_extensions_string[ ( wgl_extensions ? strlen( wgl_extensions ) : 0 ) ] ,
						extensions , strlen( extensions ) );
					_gl_extensions_string[ ( wgl_extensions ? strlen( wgl_extensions ) : 0 ) +
						( extensions ? strlen( extensions ) : 0 ) ] = '\0';
#else
					const char* extensions = (char*)rux::gui::engine::OpenGL::glGetString( GL_EXTENSIONS );
					_gl_extensions_string = alloc_array_macros( char , 
						( extensions ? strlen( extensions ) : 0 ) + 1 );					
					if( extensions && strlen( extensions ) > 0 )
						memcpy( _gl_extensions_string , extensions , strlen( extensions ) );
					_gl_extensions_string[ ( extensions ? strlen( extensions ) : 0 ) ] = '\0';
#endif
					char* ptr = _gl_extensions_string;
					while( ptr && *ptr != 0 )
					{
						if( *ptr == ' ' )
						{
							*ptr = 0;
							ptr++;
							if( *ptr != 0 
								&& *ptr != ' ' )
								_gl_extensions.Add( ptr );
						}
						else
						{
							if( *ptr != 0 )
								_gl_extensions.Add( ptr );
						}
						ptr = strstr( ptr , " " );
					}
					if( IsSupportExtension( "GL_ARB_shader_objects" , ___rux__thread_index1986 ) == 1
						&& IsSupportExtension( "GL_ARB_shading_language_100" , ___rux__thread_index1986 ) == 1
						&& IsSupportExtension( "GL_ARB_vertex_shader" , ___rux__thread_index1986 ) == 1
						&& IsSupportExtension( "GL_ARB_fragment_shader" , ___rux__thread_index1986 ) == 1 )					
						printf( "OpenGL shaders are supported\n" );
					rux::int32 preferred_format = 0;
					rux::gui::engine::OpenGL::glGetInternalformativ( GL_TEXTURE_2D , GL_RGBA , GL_TEXTURE_IMAGE_FORMAT , 1 , &preferred_format , this , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					rux::gui::engine::OpenGL::glGetIntegerv( GL_MAX_TEXTURE_SIZE , _max_texture_size , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					rux::gui::engine::OpenGL::glGetIntegerv( GL_MAX_FRAGMENT_UNIFORM_COMPONENTS  , _max_uniform_locations , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					
					//	GL_MAX_UNIFORM_LOCATIONS
					_is_supported_yv12_to_rgb_3_textures_shader = IsSupportShaders();
					if( _check_support_arb_texture_rectangle )
						_is_support_arb_texture_rectangle = IsSupportExtension( "GL_ARB_texture_rectangle" , ___rux__thread_index1986 );
					if( _check_support_old_yv12_to_rgb_shader )
						_is_supported_old_yv12_to_rgb_shader = _is_support_arb_texture_rectangle == 1 && IsSupportShaders() == 1 ? 1 : 0;
					if( _check_support_old_yuyv_to_rgb_shader )
						_is_supported_old_yuyv_to_rgb_shader = _is_support_arb_texture_rectangle == 1 && IsSupportShaders() == 1 ? 1 : 0;					
					if( _check_support_yv12_to_rgb_shader )
						_is_supported_yv12_to_rgb_shader = _is_supported_old_yv12_to_rgb_shader == 1 &&  IsSupportExtension( "GL_EXT_gpu_shader4" , ___rux__thread_index1986 ) == 1 ? 1 : 0;
					if( _check_support_yuyv_to_rgb_shader )
						_is_supported_yuyv_to_rgb_shader = _is_supported_old_yuyv_to_rgb_shader == 1 &&  IsSupportExtension( "GL_EXT_gpu_shader4" , ___rux__thread_index1986 ) == 1 ? 1 : 0;
					_is_supported_rgb_shader = _is_supported_old_yuyv_to_rgb_shader;// &&  IsSupportExtension( "GL_EXT_gpu_shader4" ) == 1 ? 1 : 0;
					_is_supported_rgba_shader = _is_supported_rgb_shader;
					_is_supported_simple_rectangle_shader = _is_supported_rgb_shader;
					if( _check_support_fxaa )
						_is_supported_fxaa = _is_supported_yv12_to_rgb_shader;
#if !is_supported_wgl_swap_buffer_interval_off
					_is_supported_wgl_swap_buffer_interval = IsSupportExtension( "WGL_EXT_extensions_string" , ___rux__thread_index1986 );
#endif
#ifdef __WINDOWS__
					_is_supported_ext_swap_control_tear = IsSupportExtension( "WGL_EXT_swap_control_tear" , ___rux__thread_index1986 );
#else
					_is_supported_ext_swap_control_tear = IsSupportExtension( "GLX_EXT_swap_control_tear" , ___rux__thread_index1986 );
#endif
					if( _check_support_vbo )
						_is_support_vbo = IsSupportVBO( ___rux__thread_index1986 );
					if( _check_support_framebuffer )
						_is_support_framebuffer_object = IsSupportFrameBufferObject( ___rux__thread_index1986 );
#if !is_support_smooth_off
					_is_support_smooth = _is_support_framebuffer_object == 1 && _is_supported_fxaa == 1 ? 1 : 0;
#endif
					if( iofile.opened() )
						iofile.write_text( "OpenGL renderer='%s'\n"
						"OpenGL version='%s'\n"
						"Shader version='%s'\n"
						"Support ARB texture rectangle='%s'\n"
						"Support old YV12 to RGB shader='%s'\n"
						"Support YV12 to RGB shader='%s'\n"
						"Support YV12 to RGB 3 textures shader='%s'\n"
						"Support old YUYV to RGB shader='%s'\n"
						"Support YUYV to RGB shader='%s'\n"
						"Support FXAA='%s'\n"
						"Support WGL swap buffer interval='%s'\n"
						"Support EXT swap control tear='%s'\n"
						"Support VBO='%s'\n"
						"Support Frame Buffer Object='%s'\n"
						"Max Texture Size='%d'\n"
						"Max Uniform Locations='%d'\n"
						,
						_gl_renderer ,
						_gl_version , 
						_gl_shader_version ,
						_is_support_arb_texture_rectangle == 1 ? "true" : "false" ,
						_is_supported_old_yv12_to_rgb_shader  == 1 ? "true" : "false" ,
						_is_supported_yv12_to_rgb_shader  == 1 ? "true" : "false" ,
						_is_supported_yv12_to_rgb_3_textures_shader == 1 ? "true" : "false" ,
						_is_supported_old_yuyv_to_rgb_shader  == 1 ? "true" : "false" ,
						_is_supported_yuyv_to_rgb_shader  == 1 ? "true" : "false" ,
						_is_supported_fxaa  == 1 ? "true" : "false" ,
						_is_supported_wgl_swap_buffer_interval  == 1 ? "true" : "false" ,
						_is_supported_ext_swap_control_tear == 1 ? "true" : "false" ,
						_is_support_vbo  == 1 ? "true" : "false" ,
						_is_support_framebuffer_object  == 1 ? "true" : "false" ,
						_max_texture_size[ 0 ] , _max_uniform_locations[ 0 ]
						);
#ifdef __WINDOWS__
					DISPLAY_DEVICEA display_device;
					memset( &display_device , 0 , sizeof( DISPLAY_DEVICEA ) );
					display_device.cb = sizeof( display_device );
					char device_name[ 32 ] = { 0 } , device_string[ 128 ] = { 0 };
					::rux::int32 device_index = 0;
					while( EnumDisplayDevicesA( NULL , device_index , &display_device , 0 ) )
					{
						if( ( display_device.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP ) )
						{
							strcpy( device_name , display_device.DeviceName );
							strcpy( device_string, display_device.DeviceString );
							::rux::int32 monitor_index = 0;
							if( EnumDisplayDevicesA( device_name , monitor_index , &display_device , 0 ) )
							{
								if( iofile.opened() )
									iofile.write_text( "\nDisplay device '%s', display monitor '%s':\n=======================================\n" , device_string , display_device.DeviceString );
								DEVMODEA dev_mode;
								memset( &dev_mode , 0 , sizeof( DEVMODEA ) );
								dev_mode.dmSize = sizeof( DEVMODEA );
								dev_mode.dmDriverExtra = 0;
								if( EnumDisplaySettingsExA( device_name , ENUM_CURRENT_SETTINGS , &dev_mode , EDS_RAWMODE ) )
								{
									if( iofile.opened() )
										iofile.write_text( "Current screen settings: width=%u, height=%u, frequency=%u, bits per pixel=%u\nPossible screen settings:\n" , dev_mode.dmPelsWidth , dev_mode.dmPelsHeight , dev_mode.dmDisplayFrequency , dev_mode.dmBitsPerPel );
								}
								::rux::int32 settings_index = 0;
								memset( &dev_mode , 0 , sizeof( DEVMODEA ) );
								dev_mode.dmSize = sizeof( DEVMODEA );
								dev_mode.dmDriverExtra = 0;
								while( EnumDisplaySettingsExA( device_name , settings_index , &dev_mode , EDS_RAWMODE ) )
								{
									if( iofile.opened() )
										iofile.write_text( "#%d: width=%u, height=%u, frequency=%u, bits per pixel=%u\n" , settings_index , dev_mode.dmPelsWidth , dev_mode.dmPelsHeight , dev_mode.dmDisplayFrequency , dev_mode.dmBitsPerPel );
									memset( &dev_mode , 0 , sizeof( DEVMODEA ) );
									dev_mode.dmSize = sizeof( DEVMODEA );
									dev_mode.dmDriverExtra = 0;
									settings_index++;
								}
								monitor_index++;
								if( iofile.opened() )
									iofile.write_text( "%s" , "=======================================\n" );
							}
						}
						device_index++;
					} 
#else
#if 0
					if( ::rux::engine::_globals->_xrandr_module._XRRGetScreenInfo )
					{
						Display* display = ::rux::engine::_globals->_x11_module.XOpenDisplay( NULL );
						if( display )
						{
							size_t count = ScreenCount( display );
							::Window window;
							for( size_t index0 = 0 ; index0 < count ; index0++ )
							{
								if( iofile.opened() )
									iofile.write_text( "\nDisplay monitor '%u':\n=======================================\n" , (::rux::uint32)index0 );

								window = RootWindow( display , index0 );
								int num_sizes = 0;
								Rotation current_rotation;
								XRRScreenSize* xrrs = ::rux::engine::_globals->_xrandr_module.XRRSizes( display , 0 , &num_sizes );

								XRRScreenConfiguration* conf = ::rux::engine::_globals->_xrandr_module.XRRGetScreenInfo( display , window );
								if( conf )
								{
									int current_rate = ::rux::engine::_globals->_xrandr_module.XRRConfigCurrentRate( conf );
									SizeID current_size_id = ::rux::engine::_globals->_xrandr_module.XRRConfigCurrentConfiguration( conf , &current_rotation );

									int current_width = xrrs[ current_size_id ].width;
									int current_height = xrrs[ current_size_id ].height;

									if( iofile.opened() )
										iofile.write_text( "Current screen settings: width=%d, height=%d, frequency=%d\n=======================================\n" , current_width , current_height , current_rate );
									::rux::engine::_globals->_xrandr_module.XRRFreeScreenConfigInfo( conf );
								}
							}
							::rux::engine::_globals->_x11_module.XCloseDisplay( display );
						}
					}
#endif
#endif
					if( iofile.opened() )
						iofile.close();
					::chmod( filename , 0777 );
				}
				rux::uint8 is_supported = 0;	
				if( extension )
				{
					for( rux::uint32 index0 = 0 ; index0 < _gl_extensions.Count() ; index0++ )
					{
						if( strcmp( _gl_extensions[ index0 ] , extension ) == 0 )
						{
							is_supported = 1;
							break;
						}
					}
				}
				::rux::log::WriteDebug( "%s:%d END" , __FUNCTION__ , __LINE__ );
				return is_supported;
			};
		};
	};
};
