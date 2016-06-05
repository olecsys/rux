#ifndef XAN_RENDER_FONT_H
#define XAN_RENDER_FONT_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include<xan_gui_defines.h>
#include <xan_opengl_functions.h>
#include <xan_string.h>
#include <xan_freetype.h>
#include <xan_render_cache.h>
namespace rux
{
	namespace gui
	{
		namespace engine
		{
			class freetype_font
			{
			public:	
				ft_structs_h::FT_Face _ft_face;
				ft_structs_h::FT_UInt _font_size_width;
				ft_structs_h::FT_UInt _font_size_height;	
				::rux::uint32 _font_filename_hash;
				freetype_font();
				~freetype_font();
				void load( const char* font_file_name , ft_structs_h::FT_UInt font_size_width , ft_structs_h::FT_UInt _font_size_height );
				void free( void );
			};
			class RenderCharacterInfo
			{
				friend class RenderFont;
			private:
				rux::uint32 _utf8;
				rux::uint32 _utf32;
				rux::int32 _width; // bitmap.width;
				rux::int32 _height; // bitmap.rows;
				rux::int32 _x_offset; // x offset of glyph in texture coordinates	
				rux::int32 _y_offset;	
			public:
				RenderCharacterInfo()
				{
					_utf8 = 0;
					_utf32 = 0;		
					_width = 0;
					_height = 0;
					_x_offset = 0;		
					_y_offset = 0;
				};
			};
			class RenderContext;
			class RenderFont : public rux::gui::FontBase
			{
				friend class RenderUniversalCache;
			private:
				ft_structs_h::FT_GlyphSlot _glyph_slot;				
				::rux::threading::RdWrLock _cs_characters;
				XMallocArray< rux::gui::engine::RenderCharacterInfo* , 5 > _characters;
				rux::int32 _atlas_width;
				rux::int32 _atlas_height;
				rux::int32 _character_height;
				rux::int32 _all_characters_width;
				rux::int32 _space_width;
				GLContext* _gl_context;
				rux::uint32* _atlas_texture;
				rux::uint8 _reset;
				rux::uint8 _is_reload_texture;
			public:				
				RenderFont( GLContext* gl_context , const rux::XString& font_file_name , ft_structs_h::FT_UInt font_width , ft_structs_h::FT_UInt font_height );
				~RenderFont();
				void LoadToTexture( const char* utf8text , size_t ___rux__thread_index1986 );
				void Render( RenderContext* render_context , const char* text , float left , float top , float width , float height , rux::gui::Color* foreground , rux::gui::engine::RenderUniversalCache* text_cache , float opacity , float& _selected_z_index , rux::int32& _stencil_ref , size_t ___rux__thread_index1986 );
				virtual float get_Height( const char* utf8text , size_t ___rux__thread_index1986 );
				virtual float get_Height( void );
				virtual float get_CharacterWidth( rux::uint32 utf8_character , size_t ___rux__thread_index1986 );
				virtual float get_TextWidth( const char* utf8text , size_t ___rux__thread_index1986 );
				void RenderingStop( void );
			private:
				void create_or_find( void );
				RenderCharacterInfo* find_character( rux::uint32 utf8_character );
			};
		};
	};
};
#endif
