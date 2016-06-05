#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_gl_texture.h>
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
			GLTexture::GLTexture( ::rux::gui::engine::GLContext* gl_context )
			{
				_named_buffer_object = 0;
				_gl_context = gl_context;
				_target = GL_TEXTURE_2D;
				_internalformat = GL_RGBA;
				_format = GL_RGBA;
				_type = GL_UNSIGNED_BYTE;
				_texture_width = 0;
				_texture_height = 0;
				_texture_id = 0;
				_data_width = 0;
				_data_height = 0;
				_texture_min_filter_texparameter = GL_NEAREST_MIPMAP_LINEAR;
				_texture_mag_filter_texparameter = GL_LINEAR;
			};
			GLTexture::~GLTexture( void )
			{
				if( _named_buffer_object )
					::rux::engine::free_object< ::rux::gui::engine::GLNamedBufferObject >( _named_buffer_object );
				if( _texture_width > 0 && _texture_height > 0 )
				{
					rux::gui::engine::OpenGL::glDeleteTextures( 1 , &_texture_id , __FUNCTION__ , __LINE__ , SIZE_MAX );
				}
			};
			::rux::int32 GLTexture::Size( GLenum format , GLenum type , ::rux::int32 width , ::rux::int32 height )
			{
				size_t size = 1;
				if( type == GL_FLOAT )
					size = 4;
				if( format == GL_RGBA || format == GL_BGRA_EXT )
					return 4 * size * width * height;
				else if( format == GL_RGB || format == GL_BGR_EXT )
					return 3 * size * width * height;
				else if( format == GL_LUMINANCE_ALPHA )
					return 2 * size * width * height;
				else
					return size * width * height;
			};
			void GLTexture::Copy00( GLenum target , GLint internalformat , GLenum format , GLenum type , ::rux::int32 width
				, ::rux::int32 height , void* data , GLint texture_min_filter_texparameter , GLint texture_mag_filter_texparameter
				, ::rux::byte& reinited , size_t ___rux__thread_index1986 )
			{
				Alloc( target , internalformat , format , type , width , height , texture_min_filter_texparameter , texture_mag_filter_texparameter , reinited , ___rux__thread_index1986 );
				if( _named_buffer_object )
				{
					_named_buffer_object->Alloc( ::rux::gui::engine::GLTexture::Size( _format , _type , _texture_width , _texture_height ) , reinited , GL_PIXEL_UNPACK_BUFFER , GL_STREAM_DRAW , ___rux__thread_index1986 );

					_named_buffer_object->Copy( ::rux::gui::engine::GLTexture::Size( _format , _type , width , height ) , data , reinited , GL_PIXEL_UNPACK_BUFFER , GL_STREAM_DRAW , ___rux__thread_index1986 );

					rux::gui::engine::OpenGL::glEnable( _target , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					rux::gui::engine::OpenGL::glBindTexture( _target , _texture_id , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					rux::gui::engine::OpenGL::glPixelStorei( GL_UNPACK_ALIGNMENT , 1 , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					_named_buffer_object->Bind( ___rux__thread_index1986 );
					if( _target == GL_TEXTURE_1D )
						rux::gui::engine::OpenGL::glTexImage1D( _target , 0 , _internalformat , _texture_width , 0 , _format , _type , NULL , __FILE__ , __LINE__ , ___rux__thread_index1986 );
					else
						rux::gui::engine::OpenGL::glTexImage2D( _target , 0 , _internalformat , _texture_width , _texture_height , 0 , _format , _type , NULL , __FILE__ , __LINE__ , ___rux__thread_index1986 );
					_named_buffer_object->UnBind( ___rux__thread_index1986 );
					rux::gui::engine::OpenGL::glBindTexture( _target , 0 , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					rux::gui::engine::OpenGL::glDisable( _target , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
				}
				else if( _texture_width > 0 && _texture_height > 0 )
				{
					rux::gui::engine::OpenGL::glEnable( _target , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					rux::gui::engine::OpenGL::glBindTexture( _target , _texture_id , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					rux::gui::engine::OpenGL::glPixelStorei( GL_UNPACK_ALIGNMENT , 1 , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					if( _target == GL_TEXTURE_1D )
					{
						rux::gui::engine::OpenGL::glTexParameteri( _target , GL_TEXTURE_MIN_FILTER , texture_min_filter_texparameter
							, __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						rux::gui::engine::OpenGL::glTexParameteri( _target , GL_TEXTURE_MAG_FILTER , texture_mag_filter_texparameter 
							, __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						rux::gui::engine::OpenGL::glTexSubImage1D( _target, 0 , 0 , width , _format , _type , data , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					}
					else
					{
						rux::gui::engine::OpenGL::glTexParameteri( _target , GL_TEXTURE_BASE_LEVEL , 0 , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						rux::gui::engine::OpenGL::glTexParameteri( _target , GL_TEXTURE_MAG_FILTER , texture_mag_filter_texparameter 
							, __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						rux::gui::engine::OpenGL::glTexParameteri( _target , GL_TEXTURE_MIN_FILTER , texture_min_filter_texparameter
							, __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						rux::gui::engine::OpenGL::glTexSubImage2D( _target, 0 , 0 , 0 , width , height , _format , _type , data , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					}
					rux::gui::engine::OpenGL::glBindTexture( _target , 0 , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					rux::gui::engine::OpenGL::glDisable( _target , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
				}
			};
			::rux::int32 GLTexture::get_TextureHeight( void )
			{
				return _texture_height;
			};
			::rux::int32 GLTexture::get_DataWidth( void )
			{
				return _data_width;
			};
			::rux::int32 GLTexture::get_DataHeight( void )
			{
				return _data_height;
			};
			::rux::int32 GLTexture::get_TextureWidth( void )
			{
				return _texture_width;
			};
			void GLTexture::Enable( size_t ___rux__thread_index1986 )
			{
				rux::gui::engine::OpenGL::glEnable( _target , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
			};
			void GLTexture::Disable( size_t ___rux__thread_index1986 )
			{
				rux::gui::engine::OpenGL::glBindTexture( _target , 0 , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
				rux::gui::engine::OpenGL::glDisable( _target , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
			};
			void GLTexture::set_ActiveShaderTexture( GLenum texture , GLint shader_location , size_t ___rux__thread_index1986 )
			{
				rux::gui::engine::OpenGL::glEnable( _target , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
				
				rux::gui::engine::OpenGL::glActiveTexture( texture , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );

				rux::gui::engine::OpenGL::glUniform1i( shader_location , texture - GL_TEXTURE0 , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
				
				/*rux::gui::engine::OpenGL::glEnable( _target , _gl_context , __FUNCTION__ , __LINE__ );
				rux::gui::engine::OpenGL::glBindTexture( _target , _texture_id , __FUNCTION__ , __LINE__ );
				rux::gui::engine::OpenGL::glPixelStorei( GL_UNPACK_ALIGNMENT , 1 , _gl_context , __FUNCTION__ , __LINE__ );
				_named_buffer_object->Bind();
				if( _target == GL_TEXTURE_1D )
					rux::gui::engine::OpenGL::glTexImage1D( _target , 0 , _internalformat , _texture_width , 0 , _format , _type , NULL , __FILE__ , __LINE__ );
				else
					rux::gui::engine::OpenGL::glTexImage2D( _target , 0 , _internalformat , _texture_width , _texture_height , 0 , _format , _type , NULL , __FILE__ , __LINE__ );
				_named_buffer_object->UnBind();
				rux::gui::engine::OpenGL::glBindTexture( _target , 0 , __FUNCTION__ , __LINE__ );
				rux::gui::engine::OpenGL::glDisable( _target , _gl_context , __FUNCTION__ , __LINE__ );*/

				
				rux::gui::engine::OpenGL::glBindTexture( _target , _texture_id , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
			};
			void GLTexture::Alloc( GLenum target , GLint internalformat , GLenum format , GLenum type , ::rux::int32 width
				, ::rux::int32 height , GLint texture_min_filter_texparameter , GLint texture_mag_filter_texparameter
				, ::rux::byte& reinited , size_t ___rux__thread_index1986 )
			{
				if( width > 0 && height > 0 )
				{
					if( _texture_width > 0 && _texture_height > 0 && 
						( _data_height != height || _data_width != width || _target != target
						|| _format != format || _internalformat != internalformat || _type != type 
						|| _texture_min_filter_texparameter != texture_min_filter_texparameter 
						|| _texture_mag_filter_texparameter != texture_mag_filter_texparameter ) )
					{
						rux::gui::engine::OpenGL::glDeleteTextures( 1 , &_texture_id , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						_texture_width = 0;
						_texture_height = 0;
					}
					if( _texture_width == 0 && _texture_height == 0 )
					{
						reinited = 1;
						if( target != GL_TEXTURE_RECTANGLE )
						{
							_texture_width = get_NumberPowerOf2( width );
							if( target == GL_TEXTURE_1D )
								_texture_height = 1;
							else
								_texture_height = get_NumberPowerOf2( height );
						}
						else
						{
							_texture_width = width;
							_texture_height = height;
						}
						_texture_mag_filter_texparameter = texture_mag_filter_texparameter;
						_texture_min_filter_texparameter = texture_min_filter_texparameter;
						_data_width = width;
						_data_height = height;
						_target = target;
						_format = format;
						_internalformat = internalformat;	
						_type = type;
						rux::gui::engine::OpenGL::glGenTextures( 1 , &_texture_id , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						if( _gl_context->_is_support_vbo )
						{
							if( _named_buffer_object == 0 )
								_named_buffer_object = alloc_object_1_param_macros( ::rux::gui::engine::GLNamedBufferObject , _gl_context );
						}
						if( _named_buffer_object )
						{
							rux::gui::engine::OpenGL::glEnable( _target , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glBindTexture( _target , _texture_id , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glPixelStorei( GL_UNPACK_ALIGNMENT , 1 , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glTexParameteri( _target , GL_TEXTURE_MIN_FILTER , _texture_min_filter_texparameter
								, __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glTexParameteri( _target , GL_TEXTURE_MAG_FILTER , _texture_mag_filter_texparameter 
								, __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glBindTexture( _target , 0 , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glDisable( _target , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							_named_buffer_object->Alloc( ::rux::gui::engine::GLTexture::Size( _format , _type , _texture_width , _texture_height ) , reinited , GL_PIXEL_UNPACK_BUFFER , GL_STREAM_DRAW , ___rux__thread_index1986 );
						}
						else
						{
							rux::gui::engine::OpenGL::glEnable( _target , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glBindTexture( _target , _texture_id , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::gui::engine::OpenGL::glPixelStorei( GL_UNPACK_ALIGNMENT , 1 , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							if( _target == GL_TEXTURE_1D )
							{
								rux::gui::engine::OpenGL::glTexParameteri( _target , GL_TEXTURE_MIN_FILTER 
									, _texture_min_filter_texparameter , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								rux::gui::engine::OpenGL::glTexParameteri( _target , GL_TEXTURE_MAG_FILTER 
									, _texture_mag_filter_texparameter , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								rux::gui::engine::OpenGL::glTexImage1D( _target , 0 , _internalformat , _texture_width , 0 , _format , _type , NULL , __FILE__ , __LINE__ , ___rux__thread_index1986 );
							}
							else
							{
								rux::gui::engine::OpenGL::glTexParameteri( _target , GL_TEXTURE_BASE_LEVEL , 0 , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								rux::gui::engine::OpenGL::glTexParameteri( _target , GL_TEXTURE_MAG_FILTER 
									, _texture_mag_filter_texparameter , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								rux::gui::engine::OpenGL::glTexParameteri( _target , GL_TEXTURE_MIN_FILTER 
									, _texture_min_filter_texparameter , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								rux::gui::engine::OpenGL::glTexImage2D( _target , 0 , _internalformat , _texture_width , _texture_height , 0 , _format , _type , NULL , __FILE__ , __LINE__ , ___rux__thread_index1986 );
							}
							rux::gui::engine::OpenGL::glDisable( _target , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						}
					}
				}
			};
			::rux::byte GLTexture::ConvertFourccToTextureFormatAndCheckTextureSize( ::rux::int32 fourcc , ::rux::uint32& texture_format
				, ::rux::int32& texture_width , ::rux::int32& texture_height , ::rux::uint32& texture_buffer_ptr_size 
				, ::rux::int32& texture_data_width , ::rux::int32& texture_data_height , GLenum& type
				, GLenum& target , ::rux::uint32 image_width , ::rux::uint32 image_height , ::rux::int16 bit_count 
				, ::rux::gui::engine::GLContext* gl_context )
			{
				::rux::byte check_pass = 1;
				if( fourcc == ::rux::media::XEnum_Fourcc_YV12 && ( gl_context->_is_supported_yv12_to_rgb_shader == 1 || gl_context->_is_supported_old_yv12_to_rgb_shader == 1 ) )
				{	
					target = GL_TEXTURE_RECTANGLE;

					type = GL_UNSIGNED_BYTE;
					texture_format = GL_LUMINANCE;
					texture_data_width = image_width;
					texture_data_height = image_height * 3 / 2;

					texture_width = texture_data_width;
					texture_height = texture_data_height;
				}
				else if( fourcc == ::rux::media::XEnum_Fourcc_YUYV && ( gl_context->_is_supported_yuyv_to_rgb_shader == 1 || gl_context->_is_supported_old_yuyv_to_rgb_shader == 1 ) )
				{
					target = GL_TEXTURE_RECTANGLE;
					type = GL_UNSIGNED_BYTE;
					texture_format = GL_LUMINANCE_ALPHA;
					texture_data_width = image_width;
					texture_data_height = image_height;
					texture_width =	image_width;
					texture_height = image_height;
				}
				else if( ( fourcc == ::rux::media::XEnum_Fourcc_RGB || fourcc == ::rux::media::XEnum_Fourcc_BI_RGB ) && bit_count == 32 && gl_context->_is_supported_rgba_shader == 1 )
				{
					target = GL_TEXTURE_RECTANGLE;
					type = GL_UNSIGNED_BYTE;
					texture_format = GL_RGBA;
					texture_data_width = image_width;
					texture_data_height = image_height;
					texture_width =	image_width;
					texture_height = image_height;
				}
				else if( ( fourcc == ::rux::media::XEnum_Fourcc_RGB || fourcc == ::rux::media::XEnum_Fourcc_BI_RGB ) && bit_count == 24 && gl_context->_is_supported_rgb_shader == 1 )
				{
					target = GL_TEXTURE_RECTANGLE;
					type = GL_UNSIGNED_BYTE;
					texture_format = GL_RGB;
					texture_data_width = image_width;
					texture_data_height = image_height;
					texture_width =	image_width;
					texture_height = image_height;
				}
				else
					check_pass = 0;
				if( check_pass )
				{
					if( gl_context->_max_texture_size[ 0 ] <= texture_width
						|| gl_context->_max_texture_size[ 0 ] <= texture_height )
						check_pass = 0;
				}
				if( check_pass )
					texture_buffer_ptr_size = ::rux::gui::engine::GLTexture::Size( texture_format , type , texture_width , texture_height );
				return check_pass;
			};
		};
	};
};
