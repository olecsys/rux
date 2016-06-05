#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_render_cache.h>
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
			void RenderUniversalCache::set_LocationAndSize( float down_left_x , float down_left_y , float width , float height )
			{
				_down_left_x = down_left_x;
				_down_left_y = down_left_y;
				_width = width;
				_height = height;
			};
			rux::uint8 RenderUniversalCache::CopyToTexture( ::rux::uint8* image_data , ::rux::uint32 image_width , ::rux::uint32 image_height , ::rux::uint32 image_size , ::rux::int16 bit_count , ::rux::int32 fourcc , float opacity , size_t ___rux__thread_index1986 )
			{
				if( _gl_context->_current_context_thread_id != ::rux::threading::XThread::get_CurrentThreadId() )
					return 0;
				_bit_count = bit_count;
				::rux::int32 __texture_width__ = 0 , __texture_height__ = 0;
				if( fourcc == ::rux::media::XEnum_Fourcc_YV12 && _gl_context->_is_supported_yv12_to_rgb_3_textures_shader )
				{
					__texture_width__ = get_NumberPowerOf2( image_width );
					__texture_height__ = get_NumberPowerOf2( image_height );
					if( _gl_context->_max_texture_size[ 0 ] > __texture_width__
						&& _gl_context->_max_texture_size[ 0 ] > __texture_height__ )
					{
						_is_draw_pixels = 0;
						if( _gl_texture0 == 0 )
							_gl_texture0 = alloc_object_1_param_macros( ::rux::gui::engine::GLTexture , _gl_context );
						if( _gl_texture1 == 0 )
							_gl_texture1 = alloc_object_1_param_macros( ::rux::gui::engine::GLTexture , _gl_context );
						if( _gl_texture2 == 0 )
							_gl_texture2 = alloc_object_1_param_macros( ::rux::gui::engine::GLTexture , _gl_context );

						::rux::uint32 y_count = image_width * image_height;
						::rux::uint32 uv_count = y_count / 4;

						::rux::uint32 v_u_width = image_width / 2;
						::rux::uint32 v_u_height = image_height / 2;

						::rux::byte reinited = 0;

						_gl_texture0->Copy00( GL_TEXTURE_2D , GL_RGBA , GL_LUMINANCE , GL_UNSIGNED_BYTE , image_width , image_height
							, image_data , GL_LINEAR , GL_LINEAR , reinited , ___rux__thread_index1986 );
						
						//_gl_texture1->Copy00( GL_TEXTURE_2D , GL_RGBA , GL_LUMINANCE , GL_UNSIGNED_BYTE , v_u_width , v_u_height
						//	, &image_data[ y_count ] , GL_NEAREST , GL_NEAREST , reinited , ___rux__thread_index1986 );

						//_gl_texture2->Copy00( GL_TEXTURE_2D , GL_RGBA , GL_LUMINANCE , GL_UNSIGNED_BYTE , v_u_width , v_u_height
						//	, &image_data[ y_count + uv_count ] , GL_NEAREST , GL_NEAREST , reinited , ___rux__thread_index1986 );
						if( reinited )
							Reset();
						return 1;
					}
				}
				::rux::uint32 __texture_format__ = GL_LUMINANCE , __texture_buffer_ptr_size__ = 0;
				::rux::int32 __texture_data_width__ = 0 , __texture_data_height__ = 0;
				__texture_width__ = 0 , __texture_height__ = 0;
				GLenum __type__ = GL_UNSIGNED_BYTE;
				GLenum __target__ = GL_TEXTURE_RECTANGLE;
				::rux::byte check_pass = ::rux::gui::engine::GLTexture::ConvertFourccToTextureFormatAndCheckTextureSize( fourcc
					, __texture_format__ , __texture_width__ , __texture_height__ , __texture_buffer_ptr_size__ , __texture_data_width__
					, __texture_data_height__ , __type__ , __target__ , image_width , image_height , bit_count , _gl_context );
				if( check_pass )
				{
					_is_draw_pixels = 0;
#if USE_texture_ptr
					if( _texture_buffer_ptr && static_cast< ::rux::uint32 >( _texture_buffer_ptr_size ) < __texture_buffer_ptr_size__ )
					{
						rux::gui::engine::OpenGL::glDeleteBuffers( 1 , _texture_buffer_ptr , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						rux::engine::free_mem( _texture_buffer_ptr );
						_texture_buffer_ptr = NULL;
						_texture_buffer_ptr_size = 0;
					}
					if( _texture_buffer_ptr == NULL )
					{
						_texture_buffer_ptr = alloc_array_macros( GLuint , 1 );
						rux::gui::engine::OpenGL::glBindBuffer( GL_PIXEL_UNPACK_BUFFER , 0 , _gl_context , __FILE__ , __LINE__ , ___rux__thread_index1986 );
						rux::gui::engine::OpenGL::glGenBuffers( 1 , _texture_buffer_ptr , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						rux::gui::engine::OpenGL::glBindBuffer( GL_PIXEL_UNPACK_BUFFER , _texture_buffer_ptr[ 0 ] , _gl_context , __FILE__ , __LINE__ , ___rux__thread_index1986 );
						_texture_buffer_ptr_size = __texture_buffer_ptr_size__;
						rux::gui::engine::OpenGL::glBufferData( GL_PIXEL_UNPACK_BUFFER , _texture_buffer_ptr_size , NULL, GL_STREAM_DRAW , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						rux::gui::engine::OpenGL::glBindBuffer( GL_PIXEL_UNPACK_BUFFER , 0 , _gl_context , __FILE__ , __LINE__ , ___rux__thread_index1986 );
					}
					if( _texture_ptr != 0 && ( (rux::uint32)_texture_height != __texture_height__
						|| (rux::uint32)_texture_width != __texture_width__
						|| _texture_format != __texture_format__ ) )
					{
						if( _texture_ptr )
						{
							rux::gui::engine::OpenGL::glDeleteTextures( 1 , _texture_ptr , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::engine::free_mem( _texture_ptr );
							_texture_ptr = NULL;
						}
						Reset();
					}
					if( _texture_ptr == NULL )
					{	
						_texture_width = __texture_width__;
						_texture_height = __texture_height__;
						_texture_data_width = image_width;
						_texture_data_height = image_height;
						_texture_format = __texture_format__;
						_texture_ptr = alloc_array_macros( GLuint , 1 );						
						rux::gui::engine::OpenGL::glGenTextures( 1 , _texture_ptr , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					}
					rux::gui::engine::OpenGL::glEnable( __target__ , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					rux::gui::engine::OpenGL::glBindTexture( __target__ , _texture_ptr[ 0 ] , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					rux::gui::engine::OpenGL::glPixelStorei( GL_UNPACK_ALIGNMENT , 1 , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					rux::gui::engine::OpenGL::glBindBuffer( GL_PIXEL_UNPACK_BUFFER , _texture_buffer_ptr[ 0 ] , _gl_context , __FILE__ , __LINE__ , ___rux__thread_index1986 );
					GLvoid* data_ptr = ::rux::gui::engine::OpenGL::glMapBuffer( GL_PIXEL_UNPACK_BUFFER , GL_WRITE_ONLY , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					if( data_ptr )
					{
						memcpy( data_ptr , image_data , image_size );
						rux::gui::engine::OpenGL::glUnmapBuffer( GL_PIXEL_UNPACK_BUFFER , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					}
					rux::gui::engine::OpenGL::glTexImage2D( __target__ , 0 , GL_RGBA , _texture_width , _texture_height , 0 , __texture_format__ , __type__ , NULL , __FILE__ , __LINE__ , ___rux__thread_index1986 );
					rux::gui::engine::OpenGL::glBindBuffer( GL_PIXEL_UNPACK_BUFFER , 0 , _gl_context , __FILE__ , __LINE__ , ___rux__thread_index1986 );
					rux::gui::engine::OpenGL::glBindTexture( __target__ , 0 , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					rux::gui::engine::OpenGL::glDisable( __target__ , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
#else
					if( _gl_texture0 == 0 )
						_gl_texture0 = alloc_object_1_param_macros( ::rux::gui::engine::GLTexture , _gl_context );

					::rux::byte reinited = 0;
					
					_texture_data_width = image_width;
					_texture_data_height = image_height;
					if( fourcc == ::rux::media::XEnum_Fourcc_BGR3
						|| fourcc == ::rux::media::XEnum_Fourcc_RGB
						|| fourcc == ::rux::media::XEnum_Fourcc_BI_RGB
						|| fourcc == ::rux::media::XEnum_Fourcc_BI_RGB )
						_gl_texture0->Copy00( __target__ , GL_RGBA , __texture_format__ , __type__ , __texture_data_width__ 
							, __texture_data_height__ , image_data , GL_NEAREST_MIPMAP_LINEAR , GL_LINEAR , reinited , ___rux__thread_index1986 );
					else
						_gl_texture0->Copy00( __target__ , GL_RGBA , __texture_format__ , __type__ , __texture_data_width__ 
							, __texture_data_height__ , image_data , GL_NEAREST_MIPMAP_LINEAR/*GL_NEAREST*/ , GL_LINEAR/*GL_NEAREST*/ , reinited , ___rux__thread_index1986 );
					if( reinited )
						Reset();
#endif			
				}				
				else if( fourcc != ::rux::media::XEnum_Fourcc_Unknown )
				{											
					rux::int32 local_fourcc = 0;
					int16 local_bit_count = 0;
					if( opacity < 1.0f )
					{
						_is_alpha = 1;
						local_fourcc = ::rux::media::XEnum_Fourcc_RGB;
						local_bit_count = 32;
					}
					else
					{			
						if( fourcc == ::rux::media::XEnum_Fourcc_GREY
							|| fourcc == ::rux::media::XEnum_Fourcc_Y800
							|| fourcc == ::rux::media::XEnum_Fourcc_Y8 )
						{
							local_fourcc = ::rux::media::XEnum_Fourcc_GREY;
							local_bit_count = 8;
						}
						else if( fourcc != ::rux::media::XEnum_Fourcc_BGR3 
							&& fourcc != ::rux::media::XEnum_Fourcc_RGB
							&& fourcc != ::rux::media::XEnum_Fourcc_BI_RGB )
						{
							local_fourcc = ::rux::media::XEnum_Fourcc_RGB;
							local_bit_count = 24;
						}
						else
						{
							local_fourcc = fourcc;
							local_bit_count = bit_count;
							if( local_bit_count == 32 )
								_is_alpha = 1;
						}
					}
					GLenum format = _gl_context->get_Format( local_fourcc , local_bit_count );
					if( (rux::uint32)_texture_data_height != image_height
						|| (rux::uint32)_texture_data_width != image_width 
						|| _texture_format != format )
					{
						if( _texture_buffer_ptr && static_cast< ::rux::uint32 >( _texture_buffer_ptr_size ) < ( local_bit_count / 8 ) * image_width * image_height )
						{
							rux::gui::engine::OpenGL::glDeleteBuffers( 1 , _texture_buffer_ptr , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::engine::free_mem( _texture_buffer_ptr );
							_texture_buffer_ptr = NULL;
							_texture_buffer_ptr_size = 0;
						}
						if( _texture_ptr )
						{
							rux::gui::engine::OpenGL::glDeleteTextures( 1 , _texture_ptr , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::engine::free_mem( _texture_ptr );
							_texture_ptr = NULL;
						}
						Reset();
					}
					if( _texture_ptr == NULL )
					{
						_texture_data_width = image_width;
						_texture_data_height = image_height;
						_texture_format = format;							
						if( _gl_context->_is_support_arb_texture_rectangle == 1 )
						{
							_texture_width = _texture_data_width;
							_texture_height = _texture_data_height;			
							if( _gl_context->_max_texture_size[ 0 ] < _texture_width
								|| _gl_context->_max_texture_size[ 0 ] < _texture_height )
								_is_draw_pixels = 1;
							if( _is_draw_pixels == 0 )
							{
								_texture_ptr = alloc_array_macros( GLuint , 1 );		
								rux::gui::engine::OpenGL::glGenTextures( 1 , _texture_ptr , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								rux::gui::engine::OpenGL::glEnable( GL_TEXTURE_RECTANGLE , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								rux::gui::engine::OpenGL::glBindTexture( GL_TEXTURE_RECTANGLE , _texture_ptr[ 0 ] , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								rux::gui::engine::OpenGL::glPixelStorei( GL_UNPACK_ALIGNMENT , 1 , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								rux::gui::engine::OpenGL::glTexImage2D( GL_TEXTURE_RECTANGLE , 0 , GL_RGBA , _texture_width , _texture_height , 0 , format , GL_UNSIGNED_BYTE , NULL , __FILE__ , __LINE__ , ___rux__thread_index1986 );
							}
						}
						else
						{
							_texture_width = ::rux::get_NumberPowerOf2( _texture_data_width );
							_texture_height = ::rux::get_NumberPowerOf2( _texture_data_height );								
							if( _gl_context->_max_texture_size[ 0 ] < _texture_width
								|| _gl_context->_max_texture_size[ 0 ] < _texture_height )
								_is_draw_pixels = 1;
							if( _is_draw_pixels == 0 )
							{
								_texture_ptr = alloc_array_macros( GLuint , 1 );
								rux::gui::engine::OpenGL::glGenTextures( 1 , _texture_ptr , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								rux::gui::engine::OpenGL::glEnable( GL_TEXTURE_2D , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								rux::gui::engine::OpenGL::glBindTexture( GL_TEXTURE_2D , _texture_ptr[ 0 ] , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								rux::gui::engine::OpenGL::glPixelStorei( GL_UNPACK_ALIGNMENT , 1 , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								rux::gui::engine::OpenGL::glTexImage2D( GL_TEXTURE_2D , 0 , GL_RGBA , _texture_width , _texture_height , 0 , format , GL_UNSIGNED_BYTE , NULL , __FILE__ , __LINE__ , ___rux__thread_index1986 );
							}
						}
					}				
					else
					{
						if( _gl_context->_is_support_arb_texture_rectangle == 1 )
						{
							rux::gui::engine::OpenGL::glEnable( GL_TEXTURE_RECTANGLE , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glBindTexture( GL_TEXTURE_RECTANGLE , _texture_ptr[ 0 ] , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glPixelStorei( GL_UNPACK_ALIGNMENT , 1 , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						}
						else
						{
							rux::gui::engine::OpenGL::glEnable( GL_TEXTURE_2D , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glBindTexture( GL_TEXTURE_2D , _texture_ptr[ 0 ] , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glPixelStorei( GL_UNPACK_ALIGNMENT , 1 , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						}
					}
					if( _is_draw_pixels == 0 && _gl_context->_is_support_vbo == 1 )
					{
						if( _texture_buffer_ptr == NULL )
						{
							_texture_buffer_ptr = alloc_array_macros( GLuint , 1 );
							rux::gui::engine::OpenGL::glBindBuffer( GL_PIXEL_UNPACK_BUFFER , 0 , _gl_context , __FILE__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glGenBuffers( 1 , _texture_buffer_ptr , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glBindBuffer( GL_PIXEL_UNPACK_BUFFER , _texture_buffer_ptr[ 0 ] , _gl_context , __FILE__ , __LINE__ , ___rux__thread_index1986 );
							_texture_buffer_ptr_size = ( local_bit_count / 8 ) * image_width * image_height;
							rux::gui::engine::OpenGL::glBufferData( GL_PIXEL_UNPACK_BUFFER , _texture_buffer_ptr_size , NULL, GL_STREAM_DRAW , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glBindBuffer( GL_PIXEL_UNPACK_BUFFER , 0 , _gl_context , __FILE__ , __LINE__ , ___rux__thread_index1986 );
						}		
						rux::gui::engine::OpenGL::glBindBuffer( GL_PIXEL_UNPACK_BUFFER , _texture_buffer_ptr[ 0 ] , _gl_context , __FILE__ , __LINE__ , ___rux__thread_index1986 );
						if( _cache_image_data )
							rux::engine::free_mem( _cache_image_data );
						_cache_image_data = (rux::uint8*)rux::gui::engine::OpenGL::glMapBuffer( GL_PIXEL_UNPACK_BUFFER , GL_WRITE_ONLY , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						_cache_image_data_size = _texture_buffer_ptr_size;
					}						
					else
					{
						if( _is_draw_pixels == 0
							&& local_fourcc == fourcc
							&& local_bit_count == bit_count )
						{
							if( _cache_image_data )
								rux::engine::free_mem( _cache_image_data );
							_cache_image_data = image_data;
							_cache_image_data_size = image_size;
						}
						else
						{
							rux::uint32 img_size = ::rux::media::image_size( local_fourcc , image_width , image_height , local_bit_count );
							if( img_size > _cache_image_data_size )
							{
								if( _cache_image_data )
									rux::engine::free_mem( _cache_image_data );
								_cache_image_data = NULL;
							}
							if( _cache_image_data == NULL )
							{
								_cache_image_data = alloc_array_macros( ::rux::uint8 , img_size );
								_cache_image_data_size = img_size;
							}
						}
					}
					if( _cache_image_data
						&& _cache_image_data != image_data )
					{
						if( opacity < 1.0f )
							rux::media::convert_to_rgb32( image_data , image_width , image_height , image_size , bit_count , fourcc , _cache_image_data , _cache_image_data_size , opacity );				
						else
						{		
							if( fourcc == ::rux::media::XEnum_Fourcc_GREY
								|| fourcc == ::rux::media::XEnum_Fourcc_Y800
								|| fourcc == ::rux::media::XEnum_Fourcc_Y8 )
								memcpy( _cache_image_data , image_data , image_size );
							else if( fourcc != ::rux::media::XEnum_Fourcc_BGR3 
								&& fourcc != ::rux::media::XEnum_Fourcc_RGB
								&& fourcc != ::rux::media::XEnum_Fourcc_BI_RGB )
								rux::media::convert_to_rgb24( image_data , image_width , image_height , image_size , bit_count , fourcc , _cache_image_data , _cache_image_data_size );					
							else
								memcpy( _cache_image_data , image_data , image_size );
						}						
						if( _is_draw_pixels == 0 
							&& _gl_context->_is_support_vbo == 1 )
						{
							rux::gui::engine::OpenGL::glUnmapBuffer( GL_PIXEL_UNPACK_BUFFER , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							_cache_image_data = NULL;
							_cache_image_data_size = 0;
						}
					}
					if( _is_draw_pixels == 0 )
					{
						if( _gl_context->_is_support_arb_texture_rectangle == 1 )
						{
							if( _gl_context->_is_support_vbo == 1 )
								rux::gui::engine::OpenGL::glTexSubImage2D( GL_TEXTURE_RECTANGLE , 0 , 0 , 0 , _texture_data_width , _texture_data_height , format , GL_UNSIGNED_BYTE , NULL , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							else
								rux::gui::engine::OpenGL::glTexSubImage2D( GL_TEXTURE_RECTANGLE , 0 , 0 , 0 , _texture_data_width , _texture_data_height , format , GL_UNSIGNED_BYTE , _cache_image_data , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glBindTexture( GL_TEXTURE_RECTANGLE , 0 , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glDisable( GL_TEXTURE_RECTANGLE , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						}
						else
						{		
							if( _gl_context->_is_support_vbo == 1 )
								rux::gui::engine::OpenGL::glTexSubImage2D( GL_TEXTURE_2D, 0 , 0 , 0 , _texture_data_width , _texture_data_height , format , GL_UNSIGNED_BYTE , NULL , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							else
								rux::gui::engine::OpenGL::glTexSubImage2D( GL_TEXTURE_2D, 0 , 0 , 0 , _texture_data_width , _texture_data_height , format , GL_UNSIGNED_BYTE , _cache_image_data , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glBindTexture( GL_TEXTURE_2D , 0 , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glDisable( GL_TEXTURE_2D , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						}
						if( _gl_context->_is_support_vbo == 1 )
							rux::gui::engine::OpenGL::glBindBuffer( GL_PIXEL_UNPACK_BUFFER , 0 , _gl_context , __FILE__ , __LINE__ , ___rux__thread_index1986 );
						else if( local_fourcc == fourcc
							&& local_bit_count == bit_count )
						{
							_cache_image_data = NULL;
							_cache_image_data_size = 0;
						}
					}	
				}
				return 1;
			};
			RenderUniversalCache::RenderUniversalCache( GLContext* gl_context )
				: ::rux::gui::RenderCacheBase( 3 )
			{
				_gl_texture0 = 0;
				_gl_texture1 = 0;
				_gl_texture2 = 0;
				_bit_count = 0;
				_color_changed = 0;
				_rendering = 0;
				_not_drawn = 1;
				_is_simple_rectangle_shader = 0;
				_ref = 1;
				_old_left = 0.f;
				_old_top = 0.f;
				_old_width = 0.f;
				_old_gl_context_height = 0;
				_old_height = 0.f;
				_old_image_left = 0.f;
				_old_image_top = 0.f;
				_old_image_height = 0.f;
				_old_image_width = 0.f;
				_old_opacity = 0.f;
				_old_border_width = 0.f;
				_old_color = NULL;
				_old_border_color = NULL;
				_fourcc = ::rux::media::XEnum_Fourcc_Unknown;
				_opacity = 0.f;
				_brightness = 0.f;
				_contrast = 0.f;
				
				_render_font = NULL;
				_fill_stencil_vertexes_count = 0;
				_cut_stencil_vertexes_count = 0;
				_drawing_vertexes_count = 0;
				_texture_coords_count = 0;
				_colors_count = 0;
				_first0 = SIZE_MAX;
				_first1 = SIZE_MAX;

				_draw_z_index0 = FLT_MAX;
				_draw_z_index1 = FLT_MAX;
				_texture_buffer_ptr_size = 0;
				_cache_image_data_size = 0;
				_down_left_x = 0.f;
				_down_left_y = 0.f;
				_width = 0.f;
				_height = 0.f;
				_draw_stencil_ref = 0;
				_is_draw_pixels = 0;
				_texture_buffer_ptr = NULL;
				_gl_context = gl_context;
				_draw_type = GL_TRIANGLES;
				_is_reset = 1;
				_is_reset_color = 1;
				_is_alpha = 0;

				_texture_data_width = 0;
				_texture_data_height = 0;
				_texture_width = 0;
				_texture_height = 0;
				_texture_ptr = NULL;
				_cache_image_data = NULL; 
				_text_hash = 0;
			};
			void RenderUniversalCache::AddRef( const char* __file__ , ::rux::int32 __line__ )
			{
				XInterlocked::Increment( &_ref );
			};
			void RenderUniversalCache::Release( const char* __file__ , ::rux::int32 __line__ )
			{
				if(	XInterlocked::Decrement( &_ref ) == 0 )
					::rux::engine::free_object< RenderUniversalCache >( this );
			};
			RenderUniversalCache::~RenderUniversalCache()
			{	
				if( _old_border_color )
					_old_border_color->Release();
				if( _old_color )
					_old_color->Release();
				if( _cache_image_data )
					rux::engine::free_mem( _cache_image_data );				
				if( _texture_ptr )
				{		
					rux::gui::engine::OpenGL::glDeleteTextures( 1 , _texture_ptr , __FUNCTION__ , __LINE__ , SIZE_MAX );
					rux::engine::free_mem( _texture_ptr );
					_texture_ptr = NULL;
				}
				if( _texture_buffer_ptr )
				{		
					rux::gui::engine::OpenGL::glDeleteBuffers( 1 , _texture_buffer_ptr , _gl_context , __FUNCTION__ , __LINE__ , SIZE_MAX );
					rux::engine::free_mem( _texture_buffer_ptr );
					_texture_buffer_ptr = NULL;
				}
				if( _gl_texture0 )
					::rux::engine::free_object< ::rux::gui::engine::GLTexture >( _gl_texture0 );
				if( _gl_texture1 )
					::rux::engine::free_object< ::rux::gui::engine::GLTexture >( _gl_texture1 );
				if( _gl_texture2 )
					::rux::engine::free_object< ::rux::gui::engine::GLTexture >( _gl_texture2 );
			};
			void RenderUniversalCache::DrawDrawingVertexes( RenderContext* render_context , size_t ___rux__thread_index1986 )
			{
				if( _drawing_vertexes_count > 0 )
				{
					if( _colors_count == 0 )
						rux::gui::engine::OpenGL::glDisableClientState( GL_COLOR_ARRAY , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					else
						rux::gui::engine::OpenGL::glEnableClientState( GL_COLOR_ARRAY , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					if( _texture_coords_count == 0 )
						rux::gui::engine::OpenGL::glDisableClientState( GL_TEXTURE_COORD_ARRAY , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					else
					{
						if( _texture_ptr || _gl_texture0 )
							::rux::gui::engine::OpenGL::glColor4f( 1 , 1 , 1 , 1 , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						rux::gui::engine::OpenGL::glEnableClientState( GL_TEXTURE_COORD_ARRAY , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					}
					{
						if( render_context->_draw_immediately_index == 0 )
							rux::gui::engine::OpenGL::glDrawArrays( _draw_type , _first0 + _fill_stencil_vertexes_count + _cut_stencil_vertexes_count , _drawing_vertexes_count , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						else
							rux::gui::engine::OpenGL::glDrawArrays( _draw_type , _first1 + _fill_stencil_vertexes_count + _cut_stencil_vertexes_count , _drawing_vertexes_count , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					}
				}
			};
			void RenderUniversalCache::DrawFillStencilVertexes( RenderContext* render_context , size_t ___rux__thread_index1986 )
			{
				if( _fill_stencil_vertexes_count > 0 )
				{
					rux::gui::engine::OpenGL::glDisableClientState( GL_COLOR_ARRAY , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					rux::gui::engine::OpenGL::glDisableClientState( GL_TEXTURE_COORD_ARRAY , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					if( render_context->_draw_immediately_index == 0 )
						rux::gui::engine::OpenGL::glDrawArrays( GL_TRIANGLES , _first0 , _fill_stencil_vertexes_count , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					else
						rux::gui::engine::OpenGL::glDrawArrays( GL_TRIANGLES , _first1 , _fill_stencil_vertexes_count , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
				}
			};
			void RenderUniversalCache::DrawCutStencilVertexes( RenderContext* render_context , size_t ___rux__thread_index1986 )
			{
				if( _cut_stencil_vertexes_count > 0 )
				{
					rux::gui::engine::OpenGL::glDisableClientState( GL_COLOR_ARRAY , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					rux::gui::engine::OpenGL::glDisableClientState( GL_TEXTURE_COORD_ARRAY , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					if( render_context->_draw_immediately_index == 0 )
						rux::gui::engine::OpenGL::glDrawArrays( GL_TRIANGLES , _first0 + _fill_stencil_vertexes_count , _cut_stencil_vertexes_count , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					else
						rux::gui::engine::OpenGL::glDrawArrays( GL_TRIANGLES , _first1 + _fill_stencil_vertexes_count , _cut_stencil_vertexes_count , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
				}
			};
			void RenderUniversalCache::Draw( RenderContext* render_context , size_t ___rux__thread_index1986 )
			{
				rux::gui::engine::OpenGL::glEnableClientState( GL_VERTEX_ARRAY , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
				if( _texture_coords_count > 0 )
				{	
					if( _is_draw_pixels == 0 )
					{
						if( _render_font )
						{
							rux::gui::engine::OpenGL::glEnable( GL_TEXTURE_2D , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glBindTexture( GL_TEXTURE_2D , _render_font->_atlas_texture[ 0 ] , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glMatrixMode( GL_TEXTURE , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glPushMatrix( __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glScalef( 1.0f / _render_font->_atlas_width , 1.0f / _render_font->_atlas_height , 1.0f , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						}
						else if( _fourcc == ::rux::media::XEnum_Fourcc_YV12 && _gl_context->_is_supported_yv12_to_rgb_3_textures_shader )
						{
							rux::gui::engine::OpenGL::glUseProgram( _gl_context->_yv12_to_rgb_3_textures_program , _gl_context 
								, __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glUniform1f( _gl_context->_yv12_to_rgb_3_textures_contrast
								, (GLfloat)_contrast , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							
							rux::gui::engine::OpenGL::glUniform1f( _gl_context->_yv12_to_rgb_3_textures_brightness
								, (GLfloat)_brightness , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							
							rux::gui::engine::OpenGL::glUniform1f( _gl_context->_yv12_to_rgb_3_textures_opacity
								, (GLfloat)_opacity , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							
							rux::gui::engine::OpenGL::glUniform1f( _gl_context->_yv12_to_rgb_3_textures_texture_height
								, (GLfloat)_gl_texture0->get_TextureHeight() , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							
							rux::gui::engine::OpenGL::glUniform1f( _gl_context->_yv12_to_rgb_3_textures_texture_width
								, (GLfloat)_gl_texture0->get_TextureWidth() , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							
							rux::gui::engine::OpenGL::glUniform1f( _gl_context->_yv12_to_rgb_3_textures_frame_height
								, (GLfloat)_gl_texture0->get_DataHeight() , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							
							rux::gui::engine::OpenGL::glUniform1f( _gl_context->_yv12_to_rgb_3_textures_frame_width
								, (GLfloat)_gl_texture0->get_DataWidth() , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );

							_gl_texture0->set_ActiveShaderTexture( GL_TEXTURE0 , _gl_context->_yv12_to_rgb_3_textures_y_tex 
								, ___rux__thread_index1986 );

							//_gl_texture1->set_ActiveShaderTexture( GL_TEXTURE1 , _gl_context->_yv12_to_rgb_3_textures_v_tex 
							//	, ___rux__thread_index1986 );

							//_gl_texture2->set_ActiveShaderTexture( GL_TEXTURE2 , _gl_context->_yv12_to_rgb_3_textures_u_tex 
							//	, ___rux__thread_index1986 );

							if( ::rux::gui::engine::_check_gl_error )
							{
								rux::gui::engine::OpenGL::glValidateProgram( _gl_context->_yv12_to_rgb_3_textures_program , _gl_context
									, __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								GLint status = GL_TRUE;
								rux::gui::engine::OpenGL::glGetProgramiv( _gl_context->_yv12_to_rgb_3_textures_program , GL_VALIDATE_STATUS 
									, &status , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								if( status != GL_TRUE )
									rux::gui::engine::OpenGL::write_error( "simple_rectangle_program GL_VALIDATE_STATUS FALSE\n" );
							}
						}
						else if( _fourcc == ::rux::media::XEnum_Fourcc_YV12 && ( _gl_context->_is_supported_yv12_to_rgb_shader == 1 || _gl_context->_is_supported_old_yv12_to_rgb_shader == 1 ) )
						{	
#if USE_texture_ptr
							GLenum target = GL_TEXTURE_RECTANGLE;
							rux::gui::engine::OpenGL::glEnable( target , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glUseProgram( _gl_context->_yv12_to_rgb_program , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );

							rux::gui::engine::OpenGL::glUniform1f( _gl_context->_yv12_to_rgb_frame_width , (GLfloat)_texture_data_width , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glUniform1f( _gl_context->_yv12_to_rgb_frame_height , (GLfloat)_texture_data_height , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glUniform1f( _gl_context->_yv12_to_rgb_frame_opacity , (GLfloat)_opacity , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glUniform1i( _gl_context->_yv12_to_rgb_frame_texture , 0 , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glUniform1f( _gl_context->_yv12_to_rgb_frame_brightness , (GLfloat)_brightness , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glUniform1f( _gl_context->_yv12_to_rgb_frame_contrast , (GLfloat)_contrast , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
												
							rux::gui::engine::OpenGL::glActiveTexture( GL_TEXTURE0 , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glBindTexture( target , _texture_ptr[ 0 ] , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
#else		
							rux::gui::engine::OpenGL::glUseProgram( _gl_context->_yv12_to_rgb_program , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );

							rux::gui::engine::OpenGL::glUniform1f( _gl_context->_yv12_to_rgb_frame_width , (GLfloat)_texture_data_width
								, _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glUniform1f( _gl_context->_yv12_to_rgb_frame_height , (GLfloat)_texture_data_height
								, _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glUniform1f( _gl_context->_yv12_to_rgb_frame_opacity , (GLfloat)_opacity
								, _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );

							rux::gui::engine::OpenGL::glUniform1f( _gl_context->_yv12_to_rgb_frame_brightness , (GLfloat)_brightness
								, _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glUniform1f( _gl_context->_yv12_to_rgb_frame_contrast , (GLfloat)_contrast 
								, _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							_gl_texture0->set_ActiveShaderTexture( GL_TEXTURE0 , _gl_context->_yv12_to_rgb_frame_texture 
								, ___rux__thread_index1986 );
#endif
							if( ::rux::gui::engine::_check_gl_error )
							{
								rux::gui::engine::OpenGL::glValidateProgram( _gl_context->_yv12_to_rgb_program , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								GLint status = GL_TRUE;
								rux::gui::engine::OpenGL::glGetProgramiv( _gl_context->_yv12_to_rgb_program , GL_VALIDATE_STATUS , &status , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								if( status != GL_TRUE )
									rux::gui::engine::OpenGL::write_error( "yv12_to_rgb_program GL_VALIDATE_STATUS FALSE\n" );
							}
						}
						else if( _fourcc == ::rux::media::XEnum_Fourcc_YUYV && ( _gl_context->_is_supported_yuyv_to_rgb_shader == 1 || _gl_context->_is_supported_old_yuyv_to_rgb_shader == 1 ) )
						{
#if USE_texture_ptr
							rux::gui::engine::OpenGL::glEnable( GL_TEXTURE_RECTANGLE , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glUseProgram( _gl_context->_yuyv_to_rgb_program , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );

							rux::gui::engine::OpenGL::glUniform1f( _gl_context->_yuyv_to_rgb_frame_width , (GLfloat)_texture_data_width , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glUniform1f( _gl_context->_yuyv_to_rgb_frame_height , (GLfloat)_texture_data_height , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glUniform1f( _gl_context->_yuyv_to_rgb_frame_opacity , (GLfloat)_opacity , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glUniform1i( _gl_context->_yuyv_to_rgb_frame_texture , 0 , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glUniform1f( _gl_context->_yuyv_to_rgb_frame_brightness , (GLfloat)_brightness , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glUniform1f( _gl_context->_yuyv_to_rgb_frame_contrast , (GLfloat)_contrast , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						
							rux::gui::engine::OpenGL::glActiveTexture( GL_TEXTURE0 , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glBindTexture( GL_TEXTURE_RECTANGLE , _texture_ptr[ 0 ] , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
#else
							rux::gui::engine::OpenGL::glUseProgram( _gl_context->_yuyv_to_rgb_program , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glUniform1f( _gl_context->_yuyv_to_rgb_frame_width , (GLfloat)_texture_data_width , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glUniform1f( _gl_context->_yuyv_to_rgb_frame_height , (GLfloat)_texture_data_height , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glUniform1f( _gl_context->_yuyv_to_rgb_frame_opacity , (GLfloat)_opacity , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glUniform1f( _gl_context->_yuyv_to_rgb_frame_brightness , (GLfloat)_brightness , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glUniform1f( _gl_context->_yuyv_to_rgb_frame_contrast , (GLfloat)_contrast , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							_gl_texture0->set_ActiveShaderTexture( GL_TEXTURE0 , _gl_context->_yuyv_to_rgb_frame_texture 
								, ___rux__thread_index1986 );
#endif
							if( ::rux::gui::engine::_check_gl_error )
							{
								rux::gui::engine::OpenGL::glValidateProgram( _gl_context->_yuyv_to_rgb_program , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								GLint status = GL_TRUE;
								rux::gui::engine::OpenGL::glGetProgramiv( _gl_context->_yuyv_to_rgb_program , GL_VALIDATE_STATUS , &status , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								if( status != GL_TRUE )
									rux::gui::engine::OpenGL::write_error( "yuyv_to_rgb_program GL_VALIDATE_STATUS FALSE\n" );
							}
						}
						else if( ( _fourcc == ::rux::media::XEnum_Fourcc_RGB || _fourcc == ::rux::media::XEnum_Fourcc_BI_RGB ) && _bit_count == 32 && _gl_context->_is_supported_rgba_shader == 1 )
						{
#if USE_texture_ptr
							rux::gui::engine::OpenGL::glEnable( GL_TEXTURE_RECTANGLE , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glUseProgram( _gl_context->_rgba_program , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );

							rux::gui::engine::OpenGL::glUniform1f( _gl_context->_rgba_frame_width , (GLfloat)_texture_data_width , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glUniform1f( _gl_context->_rgba_frame_height , (GLfloat)_texture_data_height , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glUniform1f( _gl_context->_rgba_frame_opacity , (GLfloat)_opacity , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glUniform1i( _gl_context->_rgba_frame_texture , 0 , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glUniform1f( _gl_context->_rgba_frame_brightness , (GLfloat)_brightness , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glUniform1f( _gl_context->_rgba_frame_contrast , (GLfloat)_contrast , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
												
							rux::gui::engine::OpenGL::glActiveTexture( GL_TEXTURE0 , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glBindTexture( GL_TEXTURE_RECTANGLE , _texture_ptr[ 0 ] , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
#else						
							rux::gui::engine::OpenGL::glUseProgram( _gl_context->_rgba_program , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glUniform1f( _gl_context->_rgba_frame_width , (GLfloat)_texture_data_width , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glUniform1f( _gl_context->_rgba_frame_height , (GLfloat)_texture_data_height , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glUniform1f( _gl_context->_rgba_frame_opacity , (GLfloat)_opacity , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glUniform1f( _gl_context->_rgba_frame_brightness , (GLfloat)_brightness , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glUniform1f( _gl_context->_rgba_frame_contrast , (GLfloat)_contrast , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							_gl_texture0->set_ActiveShaderTexture( GL_TEXTURE0 , _gl_context->_rgba_frame_texture 
								, ___rux__thread_index1986 );
#endif
							if( ::rux::gui::engine::_check_gl_error )
							{
								rux::gui::engine::OpenGL::glValidateProgram( _gl_context->_rgba_program , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								GLint status = GL_TRUE;
								rux::gui::engine::OpenGL::glGetProgramiv( _gl_context->_rgba_program , GL_VALIDATE_STATUS , &status , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								if( status != GL_TRUE )
									rux::gui::engine::OpenGL::write_error( "rgba_program GL_VALIDATE_STATUS FALSE\n" );
							}
						}
						else if( ( _fourcc == ::rux::media::XEnum_Fourcc_RGB || _fourcc == ::rux::media::XEnum_Fourcc_BI_RGB ) && _bit_count == 24 && _gl_context->_is_supported_rgb_shader == 1 )
						{
#if USE_texture_ptr
							rux::gui::engine::OpenGL::glEnable( GL_TEXTURE_RECTANGLE , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glUseProgram( _gl_context->_rgb_program , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );

							rux::gui::engine::OpenGL::glUniform1f( _gl_context->_rgb_frame_width , (GLfloat)_texture_data_width , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glUniform1f( _gl_context->_rgb_frame_height , (GLfloat)_texture_data_height , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glUniform1f( _gl_context->_rgb_frame_opacity , (GLfloat)_opacity , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glUniform1i( _gl_context->_rgb_frame_texture , 0 , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glUniform1f( _gl_context->_rgb_frame_brightness , (GLfloat)_brightness , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glUniform1f( _gl_context->_rgb_frame_contrast , (GLfloat)_contrast , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						
							/*rux::gui::engine::OpenGL::glBindTexture( GL_TEXTURE_RECTANGLE , _texture_ptr[ 0 ] , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glPixelStorei( GL_UNPACK_ALIGNMENT , 1 , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glBindBuffer( GL_PIXEL_UNPACK_BUFFER , _texture_buffer_ptr[ 0 ] , _gl_context , __FILE__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glTexImage2D( GL_TEXTURE_RECTANGLE , 0 , GL_RGBA , _texture_data_width , _texture_data_height , 0 , GL_RGB , GL_UNSIGNED_BYTE , NULL , __FILE__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glBindBuffer( GL_PIXEL_UNPACK_BUFFER , 0 , _gl_context , __FILE__ , __LINE__ , ___rux__thread_index1986 );
						*/
							rux::gui::engine::OpenGL::glActiveTexture( GL_TEXTURE0 , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glBindTexture( GL_TEXTURE_RECTANGLE , _texture_ptr[ 0 ] , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );

#else
							rux::gui::engine::OpenGL::glUseProgram( _gl_context->_rgb_program , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glUniform1f( _gl_context->_rgb_frame_width , (GLfloat)_texture_data_width , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glUniform1f( _gl_context->_rgb_frame_height , (GLfloat)_texture_data_height , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glUniform1f( _gl_context->_rgb_frame_opacity , (GLfloat)_opacity , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glUniform1f( _gl_context->_rgb_frame_brightness , (GLfloat)_brightness , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glUniform1f( _gl_context->_rgb_frame_contrast , (GLfloat)_contrast , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );							
							_gl_texture0->set_ActiveShaderTexture( GL_TEXTURE0 , _gl_context->_rgb_frame_texture 
								, ___rux__thread_index1986 );
#endif
							if( ::rux::gui::engine::_check_gl_error )
							{
								rux::gui::engine::OpenGL::glValidateProgram( _gl_context->_rgb_program , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								GLint status = GL_TRUE;
								rux::gui::engine::OpenGL::glGetProgramiv( _gl_context->_rgb_program , GL_VALIDATE_STATUS , &status , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								if( status != GL_TRUE )
									rux::gui::engine::OpenGL::write_error( "rgb_program GL_VALIDATE_STATUS FALSE\n" );
							}
						}
						else if( _fourcc != ::rux::media::XEnum_Fourcc_Unknown )
						{
							if( _gl_context->_is_support_arb_texture_rectangle == 1 )
							{
								rux::gui::engine::OpenGL::glEnable( GL_TEXTURE_RECTANGLE , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								if( _texture_ptr )
									rux::gui::engine::OpenGL::glBindTexture( GL_TEXTURE_RECTANGLE , _texture_ptr[ 0 ] , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							}
							else
							{
								rux::gui::engine::OpenGL::glEnable( GL_TEXTURE_2D , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								if( _texture_ptr )
									rux::gui::engine::OpenGL::glBindTexture( GL_TEXTURE_2D , _texture_ptr[ 0 ] , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								rux::gui::engine::OpenGL::glMatrixMode( GL_TEXTURE , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								rux::gui::engine::OpenGL::glPushMatrix( __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								rux::gui::engine::OpenGL::glScalef( 1.0f / _texture_width , 1.0f / _texture_height , 1.0f , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							}
						}
					}
				}
				if( _is_simple_rectangle_shader )
				{
					rux::gui::engine::OpenGL::glUseProgram( _gl_context->_simple_rectangle_program , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					rux::gui::engine::OpenGL::glUniform1f( _gl_context->_simple_rectangle_shader_texture_width 
						, (GLfloat)_gl_texture0->get_TextureWidth() , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					_gl_texture0->set_ActiveShaderTexture( GL_TEXTURE0 , _gl_context->_simple_rectangle_shader_common , ___rux__thread_index1986 );
					if( ::rux::gui::engine::_check_gl_error )
					{
						rux::gui::engine::OpenGL::glValidateProgram( _gl_context->_simple_rectangle_program , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						GLint status = GL_TRUE;
						rux::gui::engine::OpenGL::glGetProgramiv( _gl_context->_simple_rectangle_program , GL_VALIDATE_STATUS , &status , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						if( status != GL_TRUE )
							rux::gui::engine::OpenGL::write_error( "simple_rectangle_program GL_VALIDATE_STATUS FALSE\n" );
					}
				}
				rux::uint8 is_stencil = 0;		
				if( _fill_stencil_vertexes_count > 0 
					|| _cut_stencil_vertexes_count > 0 )
				{	
					/* disable _is_alpha begin*/
					rux::gui::engine::OpenGL::glDisable( GL_BLEND , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					rux::gui::engine::OpenGL::glDisable( GL_ALPHA_TEST , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					/* disable _is_alpha end*/

					rux::gui::engine::OpenGL::glEnable( GL_STENCIL_TEST , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					rux::gui::engine::OpenGL::glDisable( GL_CULL_FACE , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					rux::gui::engine::OpenGL::glStencilFunc( GL_ALWAYS , _draw_stencil_ref , 0xff , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					rux::gui::engine::OpenGL::glColorMask( GL_FALSE , GL_FALSE , GL_FALSE , GL_FALSE , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					rux::gui::engine::OpenGL::glDisable( GL_DEPTH_TEST , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					is_stencil = 1;
				}
				else
				{
					rux::gui::engine::OpenGL::glClearStencil( 0 , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					rux::gui::engine::OpenGL::glDisable( GL_STENCIL_TEST , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
				}
				if( _fill_stencil_vertexes_count > 0 )
				{
					rux::gui::engine::OpenGL::glStencilOp( GL_KEEP , GL_KEEP , GL_REPLACE , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					DrawFillStencilVertexes( render_context , ___rux__thread_index1986 );
				}	
				if( _cut_stencil_vertexes_count > 0 )
				{
					rux::gui::engine::OpenGL::glStencilOp( GL_KEEP , GL_KEEP , GL_ZERO , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					DrawCutStencilVertexes( render_context , ___rux__thread_index1986 );
				}
				if( is_stencil == 1 )
				{
					rux::gui::engine::OpenGL::glEnable( GL_DEPTH_TEST , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					rux::gui::engine::OpenGL::glColorMask( GL_TRUE , GL_TRUE , GL_TRUE , GL_TRUE , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					rux::gui::engine::OpenGL::glStencilFunc( GL_EQUAL , _draw_stencil_ref , 0xff , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					rux::gui::engine::OpenGL::glStencilOp( GL_KEEP , GL_KEEP , GL_KEEP , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
				}
				if( _is_alpha == 1 )
				{
					rux::gui::engine::OpenGL::glEnable( GL_ALPHA_TEST , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					rux::gui::engine::OpenGL::glEnable( GL_BLEND , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					rux::gui::engine::OpenGL::glDisable( GL_DEPTH_TEST , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					rux::gui::engine::OpenGL::glBlendFunc( GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
				}
				else
				{
					rux::gui::engine::OpenGL::glDisable( GL_ALPHA_TEST , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					rux::gui::engine::OpenGL::glDisable( GL_BLEND , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					rux::gui::engine::OpenGL::glEnable( GL_DEPTH_TEST , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
				}
				if( _is_draw_pixels == 1 )
				{
					if( _cache_image_data )
					{							
						if( render_context->_draw_immediately_index == 0 )
							rux::gui::engine::OpenGL::glRasterPos3f( _old_left , _old_top , _draw_z_index0 , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						else
							rux::gui::engine::OpenGL::glRasterPos3f( _old_left , _old_top , _draw_z_index1 , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						rux::gui::engine::OpenGL::glPixelZoom( _old_width / _texture_data_width , - _old_height / _texture_data_height , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						rux::gui::engine::OpenGL::glPixelStorei( GL_UNPACK_ALIGNMENT , 1 , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						rux::gui::engine::OpenGL::glDrawPixels( _texture_data_width , _texture_data_height, _texture_format , GL_UNSIGNED_BYTE , _cache_image_data , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					}
				}
				else
				{
					if( _drawing_vertexes_count > 0 )
						DrawDrawingVertexes( render_context , ___rux__thread_index1986 );
				}
				if( _is_alpha == 1 )
				{
					/*rux::gui::engine::OpenGL::glDisable( GL_BLEND , _gl_context );
					rux::gui::engine::OpenGL::glDisable( GL_ALPHA_TEST , _gl_context );
					rux::gui::engine::OpenGL::glEnable( GL_DEPTH_TEST , _gl_context );*/
				}
				if( is_stencil == 1 )
				{
					//rux::gui::engine::OpenGL::glClearStencil( 0 , _gl_context );
					//rux::gui::engine::OpenGL::glDisable( GL_STENCIL_TEST , _gl_context );
				}
				if( _texture_coords_count > 0 )
				{
					if( _render_font )
					{
						rux::gui::engine::OpenGL::glPopMatrix( __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						rux::gui::engine::OpenGL::glMatrixMode( GL_MODELVIEW , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						rux::gui::engine::OpenGL::glBindTexture( GL_TEXTURE_2D , 0 , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						rux::gui::engine::OpenGL::glDisable( GL_TEXTURE_2D , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					}
					else if( _fourcc == ::rux::media::XEnum_Fourcc_YV12 && _gl_context->_is_supported_yv12_to_rgb_3_textures_shader )
					{
						rux::gui::engine::OpenGL::glUseProgram( 0 , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						_gl_texture2->Disable( ___rux__thread_index1986 );
						_gl_texture1->Disable( ___rux__thread_index1986 );
						_gl_texture0->Disable( ___rux__thread_index1986 );
					}
					else if( _fourcc == ::rux::media::XEnum_Fourcc_YV12 && ( _gl_context->_is_supported_yv12_to_rgb_shader == 1 || _gl_context->_is_supported_old_yv12_to_rgb_shader == 1 ) )
					{
						rux::gui::engine::OpenGL::glUseProgram( 0 , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
#if USE_texture_ptr
						rux::gui::engine::OpenGL::glBindTexture( GL_TEXTURE_RECTANGLE , 0 , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						rux::gui::engine::OpenGL::glDisable( GL_TEXTURE_RECTANGLE , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
#else
						_gl_texture0->Disable( ___rux__thread_index1986 );
#endif
					}
					else if( _fourcc == ::rux::media::XEnum_Fourcc_YUYV && ( _gl_context->_is_supported_yuyv_to_rgb_shader == 1 || _gl_context->_is_supported_old_yuyv_to_rgb_shader == 1 ) )
					{
						rux::gui::engine::OpenGL::glUseProgram( 0 , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
#if USE_texture_ptr
						rux::gui::engine::OpenGL::glBindTexture( GL_TEXTURE_RECTANGLE , 0 , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						rux::gui::engine::OpenGL::glDisable( GL_TEXTURE_RECTANGLE , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
#else
						_gl_texture0->Disable( ___rux__thread_index1986 );
#endif
					}
					else if( ( _fourcc == ::rux::media::XEnum_Fourcc_RGB || _fourcc == ::rux::media::XEnum_Fourcc_BI_RGB ) && _bit_count == 32 && _gl_context->_is_supported_rgba_shader == 1 )
					{
						rux::gui::engine::OpenGL::glUseProgram( 0 , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
#if USE_texture_ptr
						rux::gui::engine::OpenGL::glBindTexture( GL_TEXTURE_RECTANGLE , 0 , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						rux::gui::engine::OpenGL::glDisable( GL_TEXTURE_RECTANGLE , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
#else
						_gl_texture0->Disable( ___rux__thread_index1986 );
#endif
					}
					else if( ( _fourcc == ::rux::media::XEnum_Fourcc_RGB || _fourcc == ::rux::media::XEnum_Fourcc_BI_RGB ) && _bit_count == 24 && _gl_context->_is_supported_rgb_shader == 1 )
					{
						rux::gui::engine::OpenGL::glUseProgram( 0 , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
#if USE_texture_ptr
						rux::gui::engine::OpenGL::glBindTexture( GL_TEXTURE_RECTANGLE , 0 , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						rux::gui::engine::OpenGL::glDisable( GL_TEXTURE_RECTANGLE , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
#else
						_gl_texture0->Disable( ___rux__thread_index1986 );
#endif
					}
					else if( _fourcc != ::rux::media::XEnum_Fourcc_Unknown )
					{
						if( _gl_context->_is_support_arb_texture_rectangle == 1 )
						{
							rux::gui::engine::OpenGL::glBindTexture( GL_TEXTURE_RECTANGLE , 0 , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glDisable( GL_TEXTURE_RECTANGLE , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						}
						else
						{
							rux::gui::engine::OpenGL::glPopMatrix( __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glMatrixMode( GL_MODELVIEW , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glBindTexture( GL_TEXTURE_2D , 0 , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glDisable( GL_TEXTURE_2D , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						}
					}
				}
				if( _is_simple_rectangle_shader )
				{
					rux::gui::engine::OpenGL::glUseProgram( 0 , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					_gl_texture0->Disable( ___rux__thread_index1986 );
				}
				_not_drawn = 0;
				_rendering = 0;
			};
			void RenderUniversalCache::FreeVertexes( void )
			{
				_colors_count = 0;
				_fill_stencil_vertexes_count = 0;
				_cut_stencil_vertexes_count = 0;
				_drawing_vertexes_count = 0;
				_texture_coords_count = 0;
			};
			void RenderUniversalCache::FreeColors( void )
			{
				_colors_count = 0;
			};
			void RenderUniversalCache::Clean( void )
			{
				if( _old_border_color )
				{
					_old_border_color->Release();
					_old_border_color = NULL;
				}
				if( _old_color )
				{
					_old_color->Release();
					_old_color = NULL;
				}
				_bit_count = 0;
				_color_changed = 0;
				_rendering = 0;
				_not_drawn = 1;
				_is_simple_rectangle_shader = 0;
				_old_left = 0.f;
				_old_top = 0.f;
				_old_width = 0.f;
				_old_gl_context_height = 0;
				_old_height = 0.f;
				_old_opacity = 0.f;
				_old_image_left = 0.f;
				_old_image_top = 0.f;
				_old_image_height = 0.f;
				_old_image_width = 0.f;
				_old_border_width = 0.f;
				
				_old_color = NULL;
				_old_border_color = NULL;
				
				_fourcc = ::rux::media::XEnum_Fourcc_Unknown;
				_opacity = 0.f;
				_brightness = 0.f;
				_contrast = 0.f;
				
				_render_font = NULL;
				_fill_stencil_vertexes_count = 0;
				_cut_stencil_vertexes_count = 0;
				_drawing_vertexes_count = 0;
				_texture_coords_count = 0;
				_colors_count = 0;
				_first0 = SIZE_MAX;
				_first1 = SIZE_MAX;

				_draw_z_index0 = FLT_MAX;
				_draw_z_index1 = FLT_MAX;
				_down_left_x = 0.f;
				_down_left_y = 0.f;
				_width = 0.f;
				_height = 0.f;
				_draw_stencil_ref = 0;
				_is_draw_pixels = 0;
				_draw_type = GL_TRIANGLES;
				_is_reset = 1;
				_is_reset_color = 1;
				_is_alpha = 0;

				_text_hash = 0;
			};
			void RenderUniversalCache::check_draw_end( RenderContext* render_context )
			{
				if( _rendering == 0 )
				{		
					_rendering = 1;
					if( _is_reset == 0 )
					{
						size_t size = _cut_stencil_vertexes_count + _fill_stencil_vertexes_count + _drawing_vertexes_count;
						if( render_context->_draw_immediately_index == 0 )
						{
							if( ( _first0 == SIZE_MAX || _first0 != render_context->_vertices_offset ) && _first1 != SIZE_MAX )
							{							
								_first0 = render_context->_vertices_offset;
								::rux::memory::copy< ::rux::gui::engine::XUIColor , 256 >( render_context->_colors_ptr0 , render_context->_colors_ptr0_count , _first0 , &render_context->_colors_ptr1[ _first1 ] ,  size );
								::rux::memory::copy< ::rux::gui::engine::XUIVertexF , 256 >( render_context->_vertices_ptr0 , render_context->_vertices_ptr0_count , _first0 , &render_context->_vertices_ptr1[ _first1 ] ,  size );
								::rux::memory::copy< ::rux::gui::engine::XUITextureCoordinate , 256 >( render_context->_texture_coords_ptr0 , render_context->_texture_coords_ptr0_count , _first0 , &render_context->_texture_coords_ptr1[ _first1 ] ,  size );
							}
							else
							{
								_first0 = render_context->_vertices_offset;
								if( _color_changed )
									::rux::memory::copy< ::rux::gui::engine::XUIColor , 256 >( render_context->_colors_ptr0 , render_context->_colors_ptr0_count , _first0 , &render_context->_colors_ptr1[ _first1 ] ,  size );
							}
						}
						else
						{							
							if( ( _first1 == SIZE_MAX || _first1 != render_context->_vertices_offset ) && _first0 != SIZE_MAX )
							{
								_first1 = render_context->_vertices_offset;
								::rux::memory::copy< ::rux::gui::engine::XUIColor , 256 >( render_context->_colors_ptr1 , render_context->_colors_ptr1_count , _first1 , &render_context->_colors_ptr0[ _first0 ] ,  size );
								::rux::memory::copy< ::rux::gui::engine::XUIVertexF , 256 >( render_context->_vertices_ptr1 , render_context->_vertices_ptr1_count , _first1 , &render_context->_vertices_ptr0[ _first0 ] ,  size );
								::rux::memory::copy< ::rux::gui::engine::XUITextureCoordinate , 256 >( render_context->_texture_coords_ptr1 , render_context->_texture_coords_ptr1_count , _first1 , &render_context->_texture_coords_ptr0[ _first0 ] ,  size );
							}
							else
							{
								_first1 = render_context->_vertices_offset;
								if( _color_changed )
									::rux::memory::copy< ::rux::gui::engine::XUIColor , 256 >( render_context->_colors_ptr1 , render_context->_colors_ptr1_count , _first1 , &render_context->_colors_ptr0[ _first0 ] ,  size );
							}
						}
						render_context->_vertices_offset += size;
					}
					else if( _is_reset )
					{
						FreeVertexes();
						if( render_context->_draw_immediately_index == 0 )
						{
							_first1 = SIZE_MAX;
							_first0 = render_context->_vertices_offset;
						}
						else
						{
							_first0 = SIZE_MAX;
							_first1 = render_context->_vertices_offset;						
						}
					}
					if( _is_reset_color )
						FreeColors();
					_is_reset = 0;
					_is_reset_color = 0;
					_color_changed = 0;
				}
			};
			void RenderUniversalCache::AddCutStencilVertex( RenderContext* render_context , const XUIVertexF& cut_stencil_vertex )
			{	
				check_draw_end( render_context );
				if( render_context->_draw_immediately_index == 0 )
				{
					size_t insert_index = _first0 + _fill_stencil_vertexes_count + _cut_stencil_vertexes_count;					
					::rux::memory::copy< ::rux::gui::engine::XUIVertexF , 256 >( render_context->_vertices_ptr0 , render_context->_vertices_ptr0_count , insert_index , &cut_stencil_vertex ,  1 );
					::rux::memory::expand< ::rux::gui::engine::XUITextureCoordinate , 256 >( render_context->_texture_coords_ptr0 , render_context->_texture_coords_ptr0_count , insert_index , 1 );
					::rux::memory::expand< ::rux::gui::engine::XUIColor , 256 >( render_context->_colors_ptr0 , render_context->_colors_ptr0_count , insert_index , 1 );
				}
				else
				{
					size_t insert_index = _first1 + _fill_stencil_vertexes_count + _cut_stencil_vertexes_count;
					::rux::memory::copy< ::rux::gui::engine::XUIVertexF , 256 >( render_context->_vertices_ptr1 , render_context->_vertices_ptr1_count , insert_index , &cut_stencil_vertex ,  1 );
					::rux::memory::expand< ::rux::gui::engine::XUITextureCoordinate , 256 >( render_context->_texture_coords_ptr1 , render_context->_texture_coords_ptr1_count , insert_index , 1 );
					::rux::memory::expand< ::rux::gui::engine::XUIColor , 256 >( render_context->_colors_ptr1 , render_context->_colors_ptr1_count , insert_index , 1 );
				}
				render_context->_vertices_offset++;
				_cut_stencil_vertexes_count++;
			};
			void RenderUniversalCache::set_Alpha( RenderContext* render_context , ::rux::uint8 is_alpha )
			{	
				check_draw_end( render_context );
				_is_alpha = is_alpha;
			};
			rux::uint32 RenderUniversalCache::get_FillStencilVertexesCount( RenderContext* render_context )
			{
				check_draw_end( render_context );
				return _fill_stencil_vertexes_count;
			}
			void RenderUniversalCache::set_ZIndex( RenderContext* render_context , float z_index )
			{
				check_draw_end( render_context );
				_draw_stencil_ref = 0;
				if( render_context->_draw_immediately_index == 0 )
				{
					if( _draw_z_index0 != z_index )
					{
						for( size_t index0 = 0 ; index0 < _fill_stencil_vertexes_count + _cut_stencil_vertexes_count + _drawing_vertexes_count ; index0++ )
							render_context->_vertices_ptr0[ _first0 + index0 ].set_Z( z_index );
						_draw_z_index0 = z_index;
					}
				}
				else
				{
					if( _draw_z_index1 != z_index )
					{
						for( size_t index0 = 0 ; index0 < _fill_stencil_vertexes_count + _cut_stencil_vertexes_count + _drawing_vertexes_count ; index0++ )
							render_context->_vertices_ptr1[ _first1 + index0 ].set_Z( z_index );
						_draw_z_index1 = z_index;
					}
				}
				if( _fill_stencil_vertexes_count > 0 || _cut_stencil_vertexes_count > 0 )
				{
					_draw_stencil_ref = _gl_context->_stencil_ref;
					_gl_context->_stencil_ref++;
				}
				AddRef( __FILE__ , __LINE__ );
				if( render_context->_draw_immediately_index == 0 )
					render_context->_draw_immediately0.Add( this );
				else
					render_context->_draw_immediately1.Add( this );
			};
			::rux::byte RenderUniversalCache::IsAlpha( RenderContext* render_context )
			{
				check_draw_end( render_context );
				return _is_alpha;
			};			
			void RenderUniversalCache::AddDrawingVertex( RenderContext* render_context , const XUIVertexF& drawing_vertex , XUIColor* drawing_color , XUITextureCoordinate* texture_coordinate )
			{	
				check_draw_end( render_context );
				if( render_context->_draw_immediately_index == 0 )
				{
					size_t insert_index = _first0 + _fill_stencil_vertexes_count + _cut_stencil_vertexes_count + _drawing_vertexes_count;
					::rux::memory::copy< ::rux::gui::engine::XUIVertexF , 256 >( render_context->_vertices_ptr0 , render_context->_vertices_ptr0_count , insert_index , &drawing_vertex ,  1 );
					if( drawing_color )
					{
						::rux::memory::copy< ::rux::gui::engine::XUIColor , 256 >( render_context->_colors_ptr0 , render_context->_colors_ptr0_count , insert_index , drawing_color ,  1 );
						_colors_count++;
					}
					else
						::rux::memory::expand< ::rux::gui::engine::XUIColor , 256 >( render_context->_colors_ptr0 , render_context->_colors_ptr0_count , insert_index , 1 );
					if( texture_coordinate )
					{
						::rux::memory::copy< ::rux::gui::engine::XUITextureCoordinate , 256 >( render_context->_texture_coords_ptr0 , render_context->_texture_coords_ptr0_count , insert_index , texture_coordinate ,  1 );
						_texture_coords_count++;
					}
					else
						::rux::memory::expand< ::rux::gui::engine::XUITextureCoordinate , 256 >( render_context->_texture_coords_ptr0 , render_context->_texture_coords_ptr0_count , insert_index , 1 );
				}
				else
				{
					size_t insert_index = _first1 + _fill_stencil_vertexes_count + _cut_stencil_vertexes_count + _drawing_vertexes_count;
					::rux::memory::copy< ::rux::gui::engine::XUIVertexF , 256 >( render_context->_vertices_ptr1 , render_context->_vertices_ptr1_count , insert_index , &drawing_vertex ,  1 );
					if( drawing_color )
					{
						::rux::memory::copy< ::rux::gui::engine::XUIColor , 256 >( render_context->_colors_ptr1 , render_context->_colors_ptr1_count , insert_index , drawing_color ,  1 );
						_colors_count++;
					}
					else
						::rux::memory::expand< ::rux::gui::engine::XUIColor , 256 >( render_context->_colors_ptr1 , render_context->_colors_ptr1_count , insert_index , 1 );
					if( texture_coordinate )
					{
						::rux::memory::copy< ::rux::gui::engine::XUITextureCoordinate , 256 >( render_context->_texture_coords_ptr1 , render_context->_texture_coords_ptr1_count , insert_index , texture_coordinate ,  1 );
						_texture_coords_count++;
					}
					else
						::rux::memory::expand< ::rux::gui::engine::XUITextureCoordinate , 256 >( render_context->_texture_coords_ptr1 , render_context->_texture_coords_ptr1_count , insert_index , 1 );
				}
				render_context->_vertices_offset++;
				_drawing_vertexes_count++;
			};
			void RenderUniversalCache::UpdateDrawingColor( RenderContext* render_context , const XUIColor& drawing_color )
			{	
				check_draw_end( render_context );
				if( render_context->_draw_immediately_index == 0 )
					::rux::memory::copy< ::rux::gui::engine::XUIColor , 256 >( render_context->_colors_ptr0 , render_context->_colors_ptr0_count , _first0 + _fill_stencil_vertexes_count + _cut_stencil_vertexes_count + _colors_count , &drawing_color , 1 );
				else
					::rux::memory::copy< ::rux::gui::engine::XUIColor , 256 >( render_context->_colors_ptr1 , render_context->_colors_ptr1_count , _first1 + _fill_stencil_vertexes_count + _cut_stencil_vertexes_count + _colors_count , &drawing_color , 1 );
				_colors_count++;
				_color_changed = 1;
			};
			void RenderUniversalCache::AddFillStencilVertex( RenderContext* render_context , const XUIVertexF& fill_stencil_vertex )
			{	
				check_draw_end( render_context );
				if( render_context->_draw_immediately_index == 0 )
				{
					size_t insert_index = _first0 + _fill_stencil_vertexes_count;					
					::rux::memory::copy< ::rux::gui::engine::XUIVertexF , 256 >( render_context->_vertices_ptr0 , render_context->_vertices_ptr0_count , insert_index , &fill_stencil_vertex ,  1 );
					::rux::memory::expand< ::rux::gui::engine::XUITextureCoordinate , 256 >( render_context->_texture_coords_ptr0 , render_context->_texture_coords_ptr0_count , insert_index , 1 );
					::rux::memory::expand< ::rux::gui::engine::XUIColor , 256 >( render_context->_colors_ptr0 , render_context->_colors_ptr0_count , insert_index , 1 );
				}
				else
				{
					size_t insert_index = _first1 + _fill_stencil_vertexes_count;
					::rux::memory::copy< ::rux::gui::engine::XUIVertexF , 256 >( render_context->_vertices_ptr1 , render_context->_vertices_ptr1_count , insert_index , &fill_stencil_vertex ,  1 );
					::rux::memory::expand< ::rux::gui::engine::XUITextureCoordinate , 256 >( render_context->_texture_coords_ptr1 , render_context->_texture_coords_ptr1_count , insert_index , 1 );
					::rux::memory::expand< ::rux::gui::engine::XUIColor , 256 >( render_context->_colors_ptr1 , render_context->_colors_ptr1_count , insert_index , 1 );
				}
				render_context->_vertices_offset++;
				_fill_stencil_vertexes_count++;
			};			
			void RenderUniversalCache::set_DrawType( ::rux::uint32 draw_type )
			{
				_draw_type = draw_type;
			};
			void RenderUniversalCache::Reset( void )
			{
				_is_reset = 1;
				_is_reset_color = 1;
				_is_alpha = 0;
			};
			void RenderUniversalCache::ResetColor( void )
			{
				_is_reset_color = 1;
			};
		};
	};
};
