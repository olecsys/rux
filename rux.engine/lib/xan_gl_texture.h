#ifndef XAN_GL_TEXTURE_H
#define XAN_GL_TEXTURE_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_gui_defines.h>
#include <xan_event.h>
#include <xan_opengl_functions.h>
#include <xan_gl_buffer_named_object.h>
namespace rux
{
	namespace gui
	{
		namespace engine
		{
			class RenderContext;
			class RenderFont;
			class GLTexture
			{
				GLenum _target;
				GLint _internalformat;
				GLenum _format;
				GLenum _type;
				::rux::int32 _texture_width;
				::rux::int32 _texture_height;
				::rux::int32 _data_width;
				::rux::int32 _data_height;
				GLuint _texture_id;
				::rux::gui::engine::GLContext* _gl_context;
				::rux::gui::engine::GLNamedBufferObject* _named_buffer_object;
				GLint _texture_min_filter_texparameter;
				GLint _texture_mag_filter_texparameter;
			public:
				GLTexture( ::rux::gui::engine::GLContext* gl_context );
				~GLTexture( void );
				void Alloc( GLenum target , GLint internalformat , GLenum format , GLenum type , ::rux::int32 width 
					, ::rux::int32 height , GLint texture_min_filter_texparameter , GLint texture_mag_filter_texparameter 
					, ::rux::byte& reinited , size_t ___rux__thread_index1986 );
				void Copy00( GLenum target , GLint internalformat , GLenum format , GLenum type , ::rux::int32 width
					, ::rux::int32 height , void* data , GLint texture_min_filter_texparameter , GLint texture_mag_filter_texparameter 
					, ::rux::byte& reinited , size_t ___rux__thread_index1986 );
				void set_ActiveShaderTexture( GLenum texture , GLint shader_location , size_t ___rux__thread_index1986 );
				::rux::int32 get_TextureWidth( void );
				::rux::int32 get_TextureHeight( void );
				::rux::int32 get_DataWidth( void );
				::rux::int32 get_DataHeight( void );
				static ::rux::int32 Size( GLenum format , GLenum type , ::rux::int32 width , ::rux::int32 height );
				static ::rux::byte ConvertFourccToTextureFormatAndCheckTextureSize( ::rux::int32 fourcc , ::rux::uint32& texture_format 
					, ::rux::int32& texture_width , ::rux::int32& texture_height , ::rux::uint32& texture_buffer_ptr_size
					, ::rux::int32& texture_data_width , ::rux::int32& texture_data_height , GLenum& type , GLenum& target 
					, ::rux::uint32 image_width , ::rux::uint32 image_height , ::rux::int16 bit_count 
					, ::rux::gui::engine::GLContext* gl_context );
				void Enable( size_t ___rux__thread_index1986 );
				void Disable( size_t ___rux__thread_index1986 );
			};
		};
	};
};
#endif
