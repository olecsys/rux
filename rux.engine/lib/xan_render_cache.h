#ifndef XAN_RENDER_CACHE_H
#define XAN_RENDER_CACHE_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_gui_defines.h>
#include <xan_event.h>
#include <xan_opengl_functions.h>
#include <xan_gl_texture.h>
#define USE_texture_ptr 0
namespace rux
{
	namespace gui
	{
		namespace engine
		{
			class RenderContext;
			class RenderFont;
			class XUIVertexF
			{
			public:
				float _x;
				float _y;
				float _z;
				XUIVertexF( const rux::int32 )
				{
					_x = 0;
					_y = 0;
					_z = 0;
				};
				XUIVertexF( float x , float y , float z )
				{
					_x = x;
					_y = y;
					_z = z;
				};
				XUIVertexF& operator =( const XUIVertexF& ui_vertex_d )
				{
					_x = ui_vertex_d._x;
					_y = ui_vertex_d._y;
					_z = ui_vertex_d._z;
					return *this;
				};
				XUIVertexF& operator =( const rux::int32 value )
				{
					if( value == 0 )
					{
						_x = 0;
						_y = 0;
						_z = 0;
					}
					return *this;
				};
				void set_Z( float z )
				{
					_z = z;
				};
				void set( const XUIVertexF& value )
				{
					_x = value._x;
					_y = value._y;
					_z = value._z;
				};
			};
			class XUIColor
			{
			public:
				GLubyte _r;
				GLubyte _g;
				GLubyte _b;
				GLubyte _a;
				XUIColor( const rux::int32 )
				{
					_r = 0;
					_g = 0;
					_b = 0;
					_a = 0;
				};
				XUIColor( GLubyte r , GLubyte g , GLubyte b , GLubyte a )
				{
					_r = r;
					_g = g;
					_b = b;
					_a = a;
				};
				XUIColor& operator =( const XUIColor& ui_color_d )
				{
					_r = ui_color_d._r;
					_g = ui_color_d._g;
					_b = ui_color_d._b;
					_a = ui_color_d._a;
					return *this;
				};
				XUIColor& operator =( const rux::int32 value )
				{
					if( value == 0 )
					{
						_r = 0;
						_g = 0;
						_b = 0;
						_a = 0;
					}
					return *this;
				};
				void set( const XUIColor& value )
				{
					_r = value._r;
					_g = value._g;
					_b = value._b;
					_a = value._a;
				};
			};
			class XUITextureCoordinate
			{
			public:
				GLshort _x;
				GLshort _y;
				XUITextureCoordinate( const rux::int32 )
				{
					_x = 0;
					_y = 0;
				}
				XUITextureCoordinate( ::rux::uint16 x , ::rux::uint16 y )
				{
					_x = x;
					_y = y;
				};
				XUITextureCoordinate& operator =( const XUITextureCoordinate& ui_color_d )
				{
					_x = ui_color_d._x;
					_y = ui_color_d._y;
					return *this;
				};
				XUITextureCoordinate& operator =( const rux::int32 value )
				{
					if( value == 0 )
					{
						_x = 0;
						_y = 0;
					}
					return *this;
				};
				void set( const XUITextureCoordinate& value )
				{
					_x = value._x;
					_y = value._y;
				};
			};
			class RenderUniversalCache : public rux::gui::RenderCacheBase
			{
				friend class RenderContext;
				friend class RenderFont;
				friend class ::rux::gui::engine::OpenGL;
			private:
				::rux::byte _rendering;
				::rux::byte _not_drawn;
				rux_volatile _ref;
				::rux::gui::Rectangle _old_clip;
				rux::gui::ColorBase* _old_color;
				rux::gui::ColorBase* _old_border_color;
				::rux::gui::Thickness _old_corner;
				float _old_border_width;
				float _old_opacity;
				float _old_left;
				float _old_top;
				float _old_width;
				float _old_height;
				::rux::int32 _old_image_left;
				::rux::int32 _old_image_top;
				::rux::int32 _old_image_height;
				::rux::int32 _old_image_width;
				::rux::int32 _old_gl_context_height;

				RenderFont* _render_font;
				size_t _fill_stencil_vertexes_count;
				size_t _cut_stencil_vertexes_count;
				size_t _drawing_vertexes_count;
				size_t _colors_count;
				size_t _texture_coords_count;				
				size_t _first0;
				size_t _first1;
				rux::int32 _texture_buffer_ptr_size;
				rux::int32	_cache_image_data_size;
				::rux::uint32 _text_hash;
				rux::uint32 _texture_format;
				rux::uint8 _is_draw_pixels;	
				rux::uint8* _cache_image_data;
				float _draw_z_index0;
				float _draw_z_index1;
								
				rux::uint8 _is_reset;
				rux::uint8 _is_alpha;
				rux::uint8 _is_reset_color;

				rux::uint32 _draw_type;

				::rux::uint32 _draw_stencil_ref;
				rux::uint32* _texture_buffer_ptr;
				rux::int32 _texture_data_width;
				rux::int32 _texture_data_height;
				rux::int32 _texture_width;
				rux::int32 _texture_height;
				rux::uint32* _texture_ptr;
				float _down_left_x;
				float _down_left_y;
				float _width;
				float _height;
				::rux::int32 _fourcc;
				::rux::int16 _bit_count;
				float _opacity;
				float _brightness;
				float _contrast;
				::rux::byte _is_simple_rectangle_shader;
				::rux::byte _color_changed;
				::rux::gui::engine::GLTexture* _gl_texture0;
				::rux::gui::engine::GLTexture* _gl_texture1;
				::rux::gui::engine::GLTexture* _gl_texture2;
			public:
				GLContext* _gl_context;
				RenderUniversalCache( GLContext* gl_context );
				~RenderUniversalCache();
				void check_draw_end( RenderContext* render_context );
				void set_DrawType( ::rux::uint32 draw_type );
				virtual void Reset( void );
				virtual void ResetColor( void );
				void AddFillStencilVertex( RenderContext* render_context , const XUIVertexF& fill_stencil_vertex );
				void AddCutStencilVertex( RenderContext* render_context , const XUIVertexF& cut_stencil_vertex );
				void set_Alpha( RenderContext* render_context , rux::uint8 is_alpha );
				void UpdateDrawingColor( RenderContext* render_context , const XUIColor& drawing_color );
				void AddDrawingVertex( RenderContext* render_context , const XUIVertexF& drawing_vertex , XUIColor* drawing_color , XUITextureCoordinate* texture_coordinate );
				rux::uint32 get_FillStencilVertexesCount( RenderContext* render_context );
				void Draw( RenderContext* render_context , size_t ___rux__thread_index1986 );
				void DrawDrawingVertexes( RenderContext* render_context , size_t ___rux__thread_index1986 );
				void DrawFillStencilVertexes( RenderContext* render_context , size_t ___rux__thread_index1986 );
				void DrawCutStencilVertexes( RenderContext* render_context , size_t ___rux__thread_index1986 );
				virtual rux::uint8 CopyToTexture( rux::uint8* image_data , rux::uint32 image_width , rux::uint32 image_height , rux::uint32 image_size , int16 bit_count , rux::int32 fourcc , float opacity , size_t ___rux__thread_index1986 );
				virtual void set_ZIndex( RenderContext* render_context , float z_index );
				virtual ::rux::byte IsAlpha( RenderContext* render_context );
				void set_LocationAndSize( float down_left_x , float down_left_y , float width , float height );
				void FreeVertexes( void );
				void FreeColors( void );
				void AddRef( const char* __file__ , ::rux::int32 __line__ );
				void Release( const char* __file__ , ::rux::int32 __line__ );
				void Clean( void );
			};			
		};
	};
};
#endif
