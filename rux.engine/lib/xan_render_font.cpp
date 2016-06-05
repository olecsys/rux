#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_render_font.h>
#include <xan_gui_engine.h>
#include <xan_render_context.h>
#include <xan_hash.h>
#include <xan_log.h>
namespace rux
{
	namespace gui
	{
		namespace engine
		{
			freetype_font::freetype_font()
			{
				_ft_face = NULL;	
				_font_size_width = 0;
				_font_size_height = 0;
			};
			freetype_font::~freetype_font()
			{	
				free();
			};
			void freetype_font::load( const char* font_file_name , ft_structs_h::FT_UInt font_size_width , ft_structs_h::FT_UInt font_size_height )
			{				
				if( _rux_ft_library == NULL )
				{
					if( FT_Init_FreeType( &_rux_ft_library ) != 0 )
						_rux_ft_library = NULL;
				}
				if( _rux_ft_library )
				{
					if( _ft_face == NULL )
					{
						rux::XString ansi_font_file_name( font_file_name , XEnumCodePage_UTF8 , 1 , NULL , __FILE__ , __LINE__ );
						ansi_font_file_name.set_ByRef( ansi_font_file_name.ConvertToAnsi() );
						if( FT_New_Face( _rux_ft_library , ansi_font_file_name.str() , 0 , &_ft_face ) != 0 )
							_ft_face = NULL;
						else
						{
							if( font_file_name )
								_font_filename_hash = ::rux::cryptography::hash::times33_hash( font_file_name , SIZE_MAX );
							_font_size_width = font_size_width;
							_font_size_height = font_size_height;
						}
					}
				}			
			};
			void freetype_font::free( void )
			{				
				if( _ft_face )
				{
					FT_Done_Face( _ft_face );
					_ft_face = NULL;
				}
			};						
			RenderFont::RenderFont( GLContext* gl_context , const ::rux::XString& font_file_name , ft_structs_h::FT_UInt font_width , ft_structs_h::FT_UInt font_height  )
			{	
				_glyph_slot = NULL;				
				_is_reload_texture = 0;
				_atlas_width = 0;
				_atlas_height = 0;
				_space_width = 0;
				_gl_context = gl_context;	
				_character_height = 0;
				_all_characters_width = 0;	
				_free_type_font = NULL;
				_font_size_width = 0;
				_font_size_height = 0;
				_atlas_texture = NULL;
				_reset = 0;
				declare_variable( rux::XString , cp_font_file_name );
				cp_font_file_name.set_ByRef( font_file_name.ConvertToAnsi() );
				declare_stack_variable( char , filename_only , 2048 );
				rux_get_executable_filename_only( cp_font_file_name.str() , filename_only );
				if( cp_font_file_name == filename_only )
				{
					::rux::safe_strncpy( filename_only , ::rux::engine::_globals->_executable_directory , 2048 );
					cp_font_file_name = filename_only;
					cp_font_file_name += "/" + font_file_name;
					cp_font_file_name.set_ByRef( cp_font_file_name.ConvertToAnsi() );
					if( rux_is_exists_file( cp_font_file_name.str() ) == 1 )
					{
						cp_font_file_name.set_ByRef( cp_font_file_name.ConvertToUTF8() );
						memcpy( _font_file_name , cp_font_file_name.str() , cp_font_file_name.Size() );
						_font_file_name_hash = ::rux::cryptography::hash::times33_hash( _font_file_name , SIZE_MAX );
					}
					else
					{
						rux::uint32 count = 0;
						char directories[ 16 ][ 512 ] = { 0 , 0 };
						rux_get_search_directories( count , directories );			
						for( ::rux::uint32 index0 = 0 ; index0 < count ; index0++ )
						{
							cp_font_file_name = directories[ index0 ];
							cp_font_file_name += "/" + font_file_name;
							cp_font_file_name.set_ByRef( cp_font_file_name.ConvertToAnsi() );
							if( rux_is_exists_file( cp_font_file_name.str() ) == 1 )
							{
								cp_font_file_name.set_ByRef( cp_font_file_name.ConvertToUTF8() );
								memcpy( _font_file_name , cp_font_file_name.str() , cp_font_file_name.Size() );
								_font_file_name_hash = ::rux::cryptography::hash::times33_hash( _font_file_name , SIZE_MAX );
								break;
							}
						}			
					}
				}
				else
				{
					cp_font_file_name.set_ByRef( cp_font_file_name.ConvertToUTF8() );
					memcpy( _font_file_name , cp_font_file_name.str() , cp_font_file_name.Size() );
					_font_file_name_hash = ::rux::cryptography::hash::times33_hash( _font_file_name , SIZE_MAX );
				}
				declare_stack_variable( char , font_short_name , 1024 );
				rux_get_executable_filename_only( _font_file_name , font_short_name );
				_font_short_name_hash = ::rux::cryptography::hash::times33_hash( font_short_name , SIZE_MAX );
				_font_size_width = font_width;
				_font_size_height = font_height;
			};			
			RenderFont::~RenderFont()
			{
				for( ; _characters.Count() > 0 ; )
				{
					rux::engine::free_object< RenderCharacterInfo >( _characters[ _characters.Count() - 1 ] );
					_characters.RemoveAt( _characters.Count() - 1 );
				}
				_glyph_slot = NULL;	
				_atlas_width = 0;
				_atlas_height = 0;
				_space_width = 0;				
				_character_height = 0;
				_all_characters_width = 0;	
				_free_type_font = NULL;
				if( _atlas_texture )
				{
					if( _gl_context )
					{
						rux::gui::engine::OpenGL::glDeleteTextures( 1 , _atlas_texture , __FUNCTION__ , __LINE__ , SIZE_MAX );
					}
					rux::engine::free_mem( _atlas_texture );		
					_atlas_texture = NULL;
				}	
			};
			void RenderFont::RenderingStop( void )
			{
				if( _reset == 1 )
					_reset = 2;
				else if( _reset == 2 )
					_reset = 0;
			};
			RenderCharacterInfo* RenderFont::find_character( ::rux::uint32 utf8_character )
			{
				RenderCharacterInfo* ui_font_info = NULL;
				READ_LOCK( _cs_characters );
				for( size_t index0 = 0 ; index0 < _characters.Count() ; index0++ )
				{
					if( _characters[ index0 ]->_utf8 == utf8_character )
					{
						ui_font_info = _characters[ index0 ];
						break;
					}
				}
				_cs_characters.ReadUnlock();
				return ui_font_info;
			};
			void RenderFont::LoadToTexture( const char* utf8text , size_t ___rux__thread_index1986 )
			{
				_reset = 1;	
				create_or_find();
				if( _free_type_font )
				{
					CS_PTR_LOCK( ::rux::gui::engine::_cs_rux_fonts );
					if( ((freetype_font*)_free_type_font)->_ft_face )
					{
						if( _glyph_slot == NULL )
						{
							if( FT_Set_Pixel_Sizes( ((freetype_font*)_free_type_font)->_ft_face , ((freetype_font*)_free_type_font)->_font_size_width , ((freetype_font*)_free_type_font)->_font_size_height ) == 0 )
								_glyph_slot = ((freetype_font*)_free_type_font)->_ft_face->glyph;
						}
					}
					_is_reload_texture = _gl_context->_current_context_thread_id == ::rux::threading::XThread::get_CurrentThreadId() ? 0 : 1;
					if( _is_reload_texture == 0 )
					{
						rux::gui::engine::OpenGL::glEnable( GL_BLEND , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						rux::gui::engine::OpenGL::glDisable( GL_DEPTH_TEST , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						rux::gui::engine::OpenGL::glBlendFunc( GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						if( _atlas_texture )
						{
							rux::gui::engine::OpenGL::glDeleteTextures( 1 , _atlas_texture , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							rux::engine::free_mem( _atlas_texture );		
							_atlas_texture = NULL;
						}	
						_atlas_texture = alloc_array_macros( GLuint , 1 );
					}
					rux::uint32 index0 = 0 , index1 = 0 , index2 = 0 , gray_count = 0;// , y_offset = 0;
					rux::uint32 utf8_character = 0;
					_declare_stackvar( ::rux::uint32 , utf32_character , 0 );
					RenderCharacterInfo* ui_font_info = NULL;
					const ::rux::byte* ptr = (::rux::byte*)utf8text;
					::rux::byte* utf8_character_bytes = (::rux::byte*)&utf8_character , need_break = 0;
					for( ; ; )
					{
						if( ::rux::utils::string::utf8::validate_character_with_utf8_character( ptr , utf8_character_bytes ) == false )
						{
							utf8_character = 0x0000FFFD;
							need_break = 1;
						}
						if( utf8_character == 0 )
							break;
						if( utf8_character != ' ' && utf8_character != '\t' )
						{
							if( find_character( utf8_character ) == NULL )
							{
								::rux::uint32 temp_utf8_character = utf8_character;
								::rux::XString::utf8_to_utf32( (char*)&temp_utf8_character , ::rux::XString::utf8_symbol_length( (char*)&temp_utf8_character , 0 ) , (char*)&utf32_character , 4 );
								ui_font_info = alloc_object_macros( RenderCharacterInfo );
								if( FT_Load_Char( ((freetype_font*)_free_type_font)->_ft_face , utf32_character , FT_LOAD_RENDER ) == 0 ) 
								{
									_all_characters_width += _glyph_slot->bitmap.width;
									ui_font_info->_utf32 = utf32_character;						
								}
								else if( FT_Load_Char( ((freetype_font*)_free_type_font)->_ft_face , 0x0000FFFD , FT_LOAD_RENDER ) == 0 ) 
								{
									_all_characters_width += _glyph_slot->bitmap.width;
									ui_font_info->_utf32 = 0x0000FFFD;
								}				
								ui_font_info->_utf8 = utf8_character;
								WRITE_LOCK( _cs_characters );
								_characters.Add( ui_font_info );
								_cs_characters.WriteUnlock();
							}
						}
						else
						{				
							if( _space_width == 0 )
							{	
								::rux::uint32 temp_utf8_character = utf8_character;
								::rux::XString::utf8_to_utf32( (char*)&temp_utf8_character , ::rux::XString::utf8_symbol_length( (char*)&temp_utf8_character , 0 ) , (char*)&utf32_character , 4 );
								if( FT_Load_Char( ((freetype_font*)_free_type_font)->_ft_face , utf32_character , FT_LOAD_RENDER ) == 0 ) 
									_space_width = _glyph_slot->metrics.horiAdvance / 64;
							}
							ui_font_info = alloc_object_macros( RenderCharacterInfo );							
							ui_font_info->_utf8 = utf8_character;				
							WRITE_LOCK( _cs_characters );
							_characters.Add( ui_font_info );
							_cs_characters.WriteUnlock();
						}
						if( need_break )
							break;
					}
					_atlas_height = ( ((freetype_font*)_free_type_font)->_ft_face->size->metrics.ascender - ((freetype_font*)_free_type_font)->_ft_face->size->metrics.descender ) / 64;
					_character_height = _atlas_height;
					if( _is_reload_texture == 0 )
					{
						rux::gui::engine::OpenGL::glEnable( GL_TEXTURE_2D , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						rux::gui::engine::OpenGL::glTexEnvf( GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						rux::gui::engine::OpenGL::glGenTextures(1, _atlas_texture , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						rux::gui::engine::OpenGL::glBindTexture( GL_TEXTURE_2D , _atlas_texture[ 0 ] , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						rux::gui::engine::OpenGL::glPixelStorei( GL_UNPACK_ALIGNMENT , 1 , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						rux::gui::engine::OpenGL::glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_NEAREST , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						rux::gui::engine::OpenGL::glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_NEAREST , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					}
					_atlas_width = ::rux::get_NumberPowerOf2( _all_characters_width );	
					::rux::int32 atlas_height = ::rux::get_NumberPowerOf2( _atlas_height );	
					if( _is_reload_texture == 0 )
					{
						if( _gl_context->_max_texture_size[ 0 ] < _atlas_width )
						{
							::rux::int32 mul = _atlas_width / _gl_context->_max_texture_size[ 0 ];
							if( _atlas_width % _gl_context->_max_texture_size[ 0 ] > 0 )
								mul++;
							atlas_height = mul * _atlas_height;
							atlas_height = ::rux::get_NumberPowerOf2( atlas_height );	
							_atlas_width = _gl_context->_max_texture_size[ 0 ];
						}
						rux::gui::engine::OpenGL::glTexImage2D( GL_TEXTURE_2D , 0 , GL_RGBA , _atlas_width , atlas_height , 0 , GL_RGBA , GL_UNSIGNED_BYTE , 0 , __FILE__ , __LINE__ , ___rux__thread_index1986 );
					}
					_atlas_height = atlas_height;
					rux::int32 x = 0 , y = 0;
					rux::uint32* u32rgba_ptr = NULL;
					rux::uint32 u32rgba_ptr_size = 0;
					READ_LOCK( _cs_characters );	

					ft_structs_h::FT_Pos max_horiBearingY = 0;
					for( index0 = 0; index0 < _characters.Count() ; index0++ ) 
					{			
						if( _characters[ index0 ]->_utf8 != '\t' && _characters[ index0 ]->_utf8 != ' ' )
						{
							if( FT_Load_Char( ((freetype_font*)_free_type_font)->_ft_face , _characters[ index0 ]->_utf32 , FT_LOAD_RENDER ) == 0 )
							{
								if( _glyph_slot->metrics.horiBearingY > max_horiBearingY )
									max_horiBearingY = _glyph_slot->metrics.horiBearingY;
							}
						}
					}	
					max_horiBearingY >>= 6;

					for( index0 = 0; index0 < _characters.Count() ; index0++ ) 
					{			
						if( _characters[ index0 ]->_utf8 != '\t' && _characters[ index0 ]->_utf8 != ' ' )
						{
							if( FT_Load_Char( ((freetype_font*)_free_type_font)->_ft_face , _characters[ index0 ]->_utf32 , FT_LOAD_RENDER ) == 0 )
							{
								gray_count = _character_height * _glyph_slot->bitmap.width;
								if( u32rgba_ptr && u32rgba_ptr_size < gray_count )
								{
									rux::engine::free_mem( u32rgba_ptr );
									u32rgba_ptr = 0;
									u32rgba_ptr_size = 0;
								}
								if( u32rgba_ptr == 0 )
								{
									u32rgba_ptr_size = gray_count;
									u32rgba_ptr = alloc_array_macros( ::rux::uint32 , u32rgba_ptr_size ); 					
								}
								memset( u32rgba_ptr , 0 , gray_count * sizeof( ::rux::uint32 ) );
								for( size_t h = 0 ; h < _glyph_slot->bitmap.rows ; h++ )
								{
									for( size_t w = 0 ; w < _glyph_slot->bitmap.width ; w++ )
										u32rgba_ptr[ w + ( h + max_horiBearingY - _glyph_slot->bitmap_top ) * _glyph_slot->bitmap.width ] = ( _glyph_slot->bitmap.buffer[ w + _glyph_slot->bitmap.width * h ] << 24 ) | 0x00ffffff;
								}
								if( x + _glyph_slot->bitmap.width > _atlas_width )
								{
									y += _character_height;;
									x = 0;
								}
								if( _is_reload_texture == 0 )
									rux::gui::engine::OpenGL::glTexSubImage2D( GL_TEXTURE_2D , 0 , x , y , _glyph_slot->bitmap.width , _character_height , GL_RGBA , GL_UNSIGNED_BYTE , u32rgba_ptr , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
								_characters[ index0 ]->_width = _glyph_slot->bitmap.width;
								_characters[ index0 ]->_height = _glyph_slot->bitmap.rows;
								_characters[ index0 ]->_x_offset = x;
								_characters[ index0 ]->_y_offset = y;
								x += _glyph_slot->bitmap.width;
							}
						}
					}	
					_cs_characters.ReadUnlock();
					if( u32rgba_ptr )
						rux::engine::free_mem( u32rgba_ptr );
					if( _is_reload_texture == 0 )
					{
						rux::gui::engine::OpenGL::glBindTexture( GL_TEXTURE_2D , 0 , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						rux::gui::engine::OpenGL::glDisable( GL_TEXTURE_2D , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						rux::gui::engine::OpenGL::glDisable( GL_BLEND , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						rux::gui::engine::OpenGL::glEnable( GL_DEPTH_TEST , _gl_context , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					}
					CS_PTR_UNLOCK( ::rux::gui::engine::_cs_rux_fonts );
				}
			};
			void RenderFont::Render( RenderContext* render_context , const char* utf8text , float left , float top , float width , float height , ::rux::gui::Color* foreground , ::rux::gui::engine::RenderUniversalCache* text_cache , float opacity , float& _selected_z_index , ::rux::int32& _stencil_ref , size_t ___rux__thread_index1986 )
			{
				create_or_find();								
				if( utf8text && utf8text[ 0 ] != 0 )
				{
					XMallocArray< RenderCharacterInfo* > characters;
					::rux::uint32 text_hash = ::rux::cryptography::hash::times33_hash( utf8text , strlen( utf8text ) );
					if( text_cache->_text_hash != text_hash )
					{
						text_cache->Reset();
						text_cache->_text_hash = text_hash;					
					}
					if( _atlas_texture == NULL || text_cache->_is_reset == 1 )
					{						
						rux::uint32 utf8_symbol = 0;
						rux::uint8 reset = 0;
						const ::rux::byte* ptr = (::rux::byte*)utf8text;
						::rux::byte* utf8_symbol_bytes = (::rux::byte*)&utf8_symbol , need_break = 0;
						for( ; ; ) 
						{ 
							if( *ptr == 0 || need_break )
								break;
							if( ::rux::utils::string::utf8::validate_character_with_utf8_character( ptr , utf8_symbol_bytes ) == false )
							{
								utf8_symbol = 0x0000FFFD;
								need_break = 1;
							}
							RenderCharacterInfo* ui_font_info = find_character( utf8_symbol );
							if( ui_font_info == NULL || _is_reload_texture == 1 )
							{				
								reset = 1;
								LoadToTexture( utf8text , ___rux__thread_index1986 );
								ui_font_info = find_character( utf8_symbol );
								if( ui_font_info )
									characters.Add( ui_font_info );
							}
							else
								characters.Add( ui_font_info );
						}		
						if( reset == 1 || _reset == 2 )
							text_cache->Reset();
					}
					if( _atlas_texture )
					{
						text_cache->_render_font = this;										
						if( text_cache->_is_reset == 1 )
						{	
							XUIColor color( foreground->get_Red() , foreground->get_Green() , foreground->get_Blue() , (GLubyte)( (float)foreground->get_Alpha() * opacity ) );

							XMallocArray< ::rux::gui::Rectangle > cuts;
							if( left < render_context->_clip._left )
								cuts.Add( ::rux::gui::Rectangle( left , top , render_context->_clip._left - left , height ) );					
							if( top < render_context->_clip._top )
								cuts.Add( ::rux::gui::Rectangle( left , top , width , render_context->_clip._top - top ) );
							if( left + width > render_context->_clip._left + render_context->_clip._width )
								cuts.Add( ::rux::gui::Rectangle( render_context->_clip._left + render_context->_clip._width , top , left + width - render_context->_clip._left - render_context->_clip._width , height ) );
							if( top + height > render_context->_clip._top + render_context->_clip._height )
								cuts.Add( ::rux::gui::Rectangle( left , render_context->_clip._top + render_context->_clip._height , width , top + height - render_context->_clip._top - render_context->_clip._height ) );
							if( cuts.Count() > 0 )
							{
								if( text_cache->get_FillStencilVertexesCount( render_context ) == 0 )
									render_context->rux_draw_rectangle_and_save_to_fill_stencil( render_context , render_context->_clip._left , render_context->_clip._top , render_context->_clip._width , render_context->_clip._height , _selected_z_index , text_cache ); 				
								for( ::rux::uint32 index0 = 0 ; index0 < cuts.Count() ; index0++ )
									render_context->rux_draw_rectangle_and_save_to_cut_stencil( render_context , cuts[ index0 ]._left , cuts[ index0 ]._top , cuts[ index0 ]._width , cuts[ index0 ]._height , _selected_z_index , text_cache );
							}
							text_cache->set_Alpha( render_context , 1 );
							rux::int32 x_offset = (rux::int32)left , y_offset = (rux::int32)top;		
							rux::int32 character_height = _character_height;
							rux::int32 tex_coord_left = 0;
							rux::int32 vertex_left = 0 , vertex_top0 = 0 , vertex_top1 = 0;
							if( height < _character_height )
								character_height = _character_height - height;
							rux::uint8 wait_for_new_line = 0;
							for( size_t index1 = 0 ; index1 < characters.Count() ; index1++) 
							{
								if( wait_for_new_line == 0 && characters[ index1 ]->_utf8 == ' ' )
									x_offset = x_offset + _space_width;
								else if( wait_for_new_line == 0 && characters[ index1 ]->_utf8 == '\t' )
									x_offset = x_offset + 2 * _space_width;
								else if( characters[ index1 ]->_utf8 == '\n' )
								{						
									x_offset = (rux::int32)left;
									y_offset += _character_height;
									height -= _character_height;
									if( height <= 0.f )
										break;
									if( height < _character_height )
										character_height = (rux::int32)height;
									wait_for_new_line = 0;
								}
								else if( wait_for_new_line == 0 )
								{
									if( left + width < x_offset + characters[ index1 ]->_width )
									{
										rux::int32 character_width = (rux::int32)( left + width - x_offset );						
										tex_coord_left = (rux::int32)( characters[ index1 ]->_x_offset + character_width );
										vertex_left = x_offset + character_width;
										vertex_top0 = y_offset;
										vertex_top1 = vertex_top0 + character_height;		

										XUITextureCoordinate texture_coordinate( (GLshort)characters[ index1 ]->_x_offset , (GLshort)characters[ index1 ]->_y_offset );
										text_cache->AddDrawingVertex( render_context , XUIVertexF( (float)x_offset , (float)vertex_top0 , _selected_z_index ) , &color , &texture_coordinate );
										texture_coordinate = XUITextureCoordinate( (GLshort)tex_coord_left , (GLshort)characters[ index1 ]->_y_offset );
										text_cache->AddDrawingVertex( render_context , XUIVertexF( (float)x_offset + (float)character_width , (float)vertex_top0 , _selected_z_index ) , &color , &texture_coordinate );
										texture_coordinate = XUITextureCoordinate( (GLshort)tex_coord_left , (GLshort)characters[ index1 ]->_y_offset + character_height );
										text_cache->AddDrawingVertex( render_context , XUIVertexF( (float)x_offset + (float)character_width , (float)vertex_top0 + (float)character_height , _selected_z_index ) , &color , &texture_coordinate );
										texture_coordinate = XUITextureCoordinate( (GLshort)tex_coord_left , (GLshort)characters[ index1 ]->_y_offset + character_height );
										text_cache->AddDrawingVertex( render_context , XUIVertexF( (float)x_offset + (float)character_width , (float)vertex_top0 + (float)character_height , _selected_z_index ) , &color , &texture_coordinate );
										texture_coordinate = XUITextureCoordinate( (GLshort)characters[ index1 ]->_x_offset , (GLshort)characters[ index1 ]->_y_offset );
										text_cache->AddDrawingVertex( render_context , XUIVertexF( (float)x_offset , (float)vertex_top0 , _selected_z_index ) , &color , &texture_coordinate );
										texture_coordinate = XUITextureCoordinate( (GLshort)characters[ index1 ]->_x_offset , (GLshort)characters[ index1 ]->_y_offset + character_height );
										text_cache->AddDrawingVertex( render_context , XUIVertexF( (float)x_offset , (float)vertex_top0 + (float)character_height , _selected_z_index ) , &color , &texture_coordinate );

										wait_for_new_line = 1;
									}						
									else
									{						
										tex_coord_left = characters[ index1 ]->_x_offset + characters[ index1 ]->_width;
										//vertex_left = x_offset + characters[ index1 ]->_width;
										vertex_top0 = y_offset;
										//vertex_top1 = vertex_top0 + character_height;
										
										XUITextureCoordinate texture_coordinate( (GLshort)characters[ index1 ]->_x_offset , (GLshort)characters[ index1 ]->_y_offset );
										text_cache->AddDrawingVertex( render_context , XUIVertexF( (float)x_offset , (float)vertex_top0 , _selected_z_index ) , &color , &texture_coordinate );
										texture_coordinate = XUITextureCoordinate( (GLshort)tex_coord_left , (GLshort)characters[ index1 ]->_y_offset );
										text_cache->AddDrawingVertex( render_context , XUIVertexF( (float)x_offset + (float)characters[ index1 ]->_width , (float)vertex_top0 , _selected_z_index ) , &color , &texture_coordinate );
										texture_coordinate = XUITextureCoordinate( (GLshort)tex_coord_left , (GLshort)characters[ index1 ]->_y_offset + character_height );
										text_cache->AddDrawingVertex( render_context , XUIVertexF( (float)x_offset + (float)characters[ index1 ]->_width , (float)vertex_top0 + (float)character_height , _selected_z_index ) , &color , &texture_coordinate );
										texture_coordinate = XUITextureCoordinate( (GLshort)tex_coord_left , (GLshort)characters[ index1 ]->_y_offset + character_height );
										text_cache->AddDrawingVertex( render_context , XUIVertexF( (float)x_offset + (float)characters[ index1 ]->_width , (float)vertex_top0 + (float)character_height , _selected_z_index ) , &color , &texture_coordinate );
										texture_coordinate = XUITextureCoordinate( (GLshort)characters[ index1 ]->_x_offset , (GLshort)characters[ index1 ]->_y_offset );
										text_cache->AddDrawingVertex( render_context , XUIVertexF( (float)x_offset , (float)vertex_top0 , _selected_z_index ) , &color , &texture_coordinate );
										texture_coordinate = XUITextureCoordinate( (GLshort)characters[ index1 ]->_x_offset , (GLshort)characters[ index1 ]->_y_offset + character_height );
										text_cache->AddDrawingVertex( render_context , XUIVertexF( (float)x_offset , (float)vertex_top0 + (float)character_height , _selected_z_index ) , &color , &texture_coordinate );

										x_offset = x_offset + characters[ index1 ]->_width;
									}
								}
							}
						}
						else if( text_cache->_is_reset_color == 1 )
						{
							text_cache->set_Alpha( render_context , 1 );
							XUIColor color( foreground->get_Red() , foreground->get_Green() , foreground->get_Blue() , (GLubyte)( (float)foreground->get_Alpha() * opacity ) );
							while( text_cache->_drawing_vertexes_count != text_cache->_colors_count )
								text_cache->UpdateDrawingColor( render_context , color );
						}
						text_cache->set_ZIndex( render_context , _selected_z_index );
					}
				}
			};
			float RenderFont::get_Height( const char* utf8text , size_t ___rux__thread_index1986 )
			{
				if( _character_height == 0 )
				{
					if( _gl_context )
						LoadToTexture( utf8text , ___rux__thread_index1986 );
					else
						get_Height();
				}
				float height = (float)_character_height;
				if( utf8text && utf8text[ 0 ] != 0 )
				{
					::rux::uint32 utf8char = 0;
					const ::rux::byte* ptr = (::rux::byte*)utf8text;
					::rux::byte* utf8charbytes = (::rux::byte*)&utf8char;
					for( ; ; )
					{
						if( *ptr == 0 )
							break;
						if( ::rux::utils::string::utf8::validate_character_with_utf8_character( ptr , utf8charbytes ) == false )
							break;
						if( utf8char == '\n' )
							height += (float)_character_height;
					}
				}
				return ::rux::math::round( height , 2 );
			};
			float RenderFont::get_Height( void )
			{
				if( _character_height == 0 )
				{	
					create_or_find();
					CS_PTR_LOCK( ::rux::gui::engine::_cs_rux_fonts );
					if( _free_type_font )
					{
						if( ((freetype_font*)_free_type_font)->_ft_face )
						{
							if( _glyph_slot == NULL )
							{
								if( FT_Set_Pixel_Sizes( ((freetype_font*)_free_type_font)->_ft_face , ((freetype_font*)_free_type_font)->_font_size_width , ((freetype_font*)_free_type_font)->_font_size_height ) == 0 )
								{
									_glyph_slot = ((freetype_font*)_free_type_font)->_ft_face->glyph;								
									_atlas_height = ( ((freetype_font*)_free_type_font)->_ft_face->size->metrics.ascender - ((freetype_font*)_free_type_font)->_ft_face->size->metrics.descender ) / 64; 		
									_character_height = _atlas_height;						
								}	
							}
						}																	
					}
					CS_PTR_UNLOCK( ::rux::gui::engine::_cs_rux_fonts );
				}
				return (float)_character_height;
			};
			float RenderFont::get_TextWidth( const char* utf8text , size_t ___rux__thread_index1986 )
			{
				float width = 0.0f , temp_width = 0.f;
				if( utf8text && utf8text[ 0 ] != 0 )
				{
					rux::uint32 utf8char = 0;
					const ::rux::byte* ptr = (::rux::byte*)utf8text;
					::rux::byte* utf8charbytes = (::rux::byte*)&utf8char;
					for( ; ; )
					{
						if( ::rux::utils::string::utf8::validate_character_with_utf8_character( ptr , utf8charbytes ) == false
							|| utf8char == 0 )
							break;
						if( utf8char == '\n' )
						{
							if( temp_width > width )
								width = temp_width;
							temp_width = 0.f;
						}
						else
							temp_width = temp_width + get_CharacterWidth( utf8char , ___rux__thread_index1986 );
					}
					if( temp_width > width )
						width = temp_width;
				}
				return ::rux::math::round( width , 2 );
			};
			void RenderFont::create_or_find( void )
			{
				if( _free_type_font == NULL )
				{
					CS_PTR_LOCK( ::rux::gui::engine::_cs_rux_fonts );
					for( ::rux::uint32 index0 = 0 ; index0 < ::rux::gui::engine::_rux_fonts.Count() ; index0++ )
					{
						if( ::rux::gui::engine::_rux_fonts[ index0 ]->_font_filename_hash == _font_file_name_hash
							&& ::rux::gui::engine::_rux_fonts[ index0 ]->_font_size_width == _font_size_width
							&& ::rux::gui::engine::_rux_fonts[ index0 ]->_font_size_height == _font_size_height )
						{
							_free_type_font = ::rux::gui::engine::_rux_fonts[ index0 ];
							break;
						}
					}
					if( _free_type_font == NULL )
					{
						_free_type_font = alloc_object_macros( freetype_font );
						((freetype_font*)_free_type_font)->load( _font_file_name , _font_size_width , _font_size_height );	
						rux::gui::engine::_rux_fonts.Add( (freetype_font*)_free_type_font );
					}
					CS_PTR_UNLOCK( ::rux::gui::engine::_cs_rux_fonts );
				}
			};
			float RenderFont::get_CharacterWidth( ::rux::uint32 utf8_character , size_t ___rux__thread_index1986 )
			{	
				create_or_find();
				if( utf8_character == ' ' )
				{
					if( _space_width == 0 )
					{						
						if( _gl_context )
						{
							char temp[] = 
							{
								((char*)&utf8_character)[ 0 ] , ((char*)&utf8_character)[ 1 ] , ((char*)&utf8_character)[ 2 ]
								, ((char*)&utf8_character)[ 3 ] , 0
							};
							LoadToTexture( temp , ___rux__thread_index1986 );
						}
						else
						{
							if( _free_type_font )
							{					
								CS_PTR_LOCK( ::rux::gui::engine::_cs_rux_fonts );
								if( ((freetype_font*)_free_type_font)->_ft_face )
								{
									if( _glyph_slot == NULL )
									{
										if( FT_Set_Pixel_Sizes( ((freetype_font*)_free_type_font)->_ft_face , ((freetype_font*)_free_type_font)->_font_size_width , ((freetype_font*)_free_type_font)->_font_size_height ) == 0 )
											_glyph_slot = ((freetype_font*)_free_type_font)->_ft_face->glyph;
									}
								}
								if( _space_width == 0 )
								{		
									_declare_stackvar( ::rux::uint32 , utf32_character , 0 );
									::rux::uint32 temp_utf8_character = utf8_character;
									::rux::XString::utf8_to_utf32( (char*)&temp_utf8_character , ::rux::XString::utf8_symbol_length( (char*)&temp_utf8_character , 0 ) , (char*)&utf32_character , 4 );
									if( FT_Load_Char( ((freetype_font*)_free_type_font)->_ft_face , utf32_character , FT_LOAD_RENDER ) == 0 )
										_space_width = _glyph_slot->metrics.horiAdvance / 64;
								}										
								_atlas_height = ( ((freetype_font*)_free_type_font)->_ft_face->size->metrics.ascender - ((freetype_font*)_free_type_font)->_ft_face->size->metrics.descender ) / 64; 		
								_character_height = _atlas_height;
								CS_PTR_UNLOCK( ::rux::gui::engine::_cs_rux_fonts );
							}
						}
					}
					return (float)_space_width;
				}
				else if( utf8_character == '\t' )
				{
					if( _space_width == 0 )
					{
						if( _gl_context )
						{
							char temp[] = 
							{
								((char*)&utf8_character)[ 0 ] , ((char*)&utf8_character)[ 1 ] , ((char*)&utf8_character)[ 2 ]
								, ((char*)&utf8_character)[ 3 ] , 0
							};
							LoadToTexture( temp , ___rux__thread_index1986 );
						}
						else
						{							
							if( _free_type_font )
							{
								CS_PTR_LOCK( ::rux::gui::engine::_cs_rux_fonts );
								if( ((freetype_font*)_free_type_font)->_ft_face )
								{
									if( _glyph_slot == NULL )
									{
										if( FT_Set_Pixel_Sizes( ((freetype_font*)_free_type_font)->_ft_face , ((freetype_font*)_free_type_font)->_font_size_width , ((freetype_font*)_free_type_font)->_font_size_height ) == 0 )
											_glyph_slot = ((freetype_font*)_free_type_font)->_ft_face->glyph;
									}
								}
								if( _space_width == 0 )
								{	
									_declare_stackvar( ::rux::uint32 , utf32_character , 0 );
									::rux::uint32 temp_utf8_character = utf8_character;
									::rux::XString::utf8_to_utf32( (char*)&temp_utf8_character , ::rux::XString::utf8_symbol_length( (char*)&temp_utf8_character , 0 ) , (char*)&utf32_character , 4 );
									if( FT_Load_Char( ((freetype_font*)_free_type_font)->_ft_face , utf32_character , FT_LOAD_RENDER ) == 0 ) 
										_space_width = _glyph_slot->metrics.horiAdvance / 64;
								}										
								_atlas_height = ( ((freetype_font*)_free_type_font)->_ft_face->size->metrics.ascender - ((freetype_font*)_free_type_font)->_ft_face->size->metrics.descender ) / 64; 		
								_character_height = _atlas_height;
								CS_PTR_UNLOCK( ::rux::gui::engine::_cs_rux_fonts );
							}
						}
					}
					return 2.f * _space_width;
				}
				else
				{
					RenderCharacterInfo* ui_font_info = find_character( utf8_character );
					if( ui_font_info )
						return (float)ui_font_info->_width;
					else
					{						
						if( _gl_context )
						{
							char temp[] = 
							{
								((char*)&utf8_character)[ 0 ] , ((char*)&utf8_character)[ 1 ] , ((char*)&utf8_character)[ 2 ]
								, ((char*)&utf8_character)[ 3 ] , 0
							};
							LoadToTexture( temp , ___rux__thread_index1986 );
							ui_font_info = find_character( utf8_character );
							if( ui_font_info )
								return (float)ui_font_info->_width;
							else
								return 0;
						}
						else
						{
							if( _free_type_font )
							{					
								CS_PTR_LOCK( ::rux::gui::engine::_cs_rux_fonts );
								if( ((freetype_font*)_free_type_font)->_ft_face )
								{
									if( _glyph_slot == NULL )
									{
										if( FT_Set_Pixel_Sizes( ((freetype_font*)_free_type_font)->_ft_face , ((freetype_font*)_free_type_font)->_font_size_width , ((freetype_font*)_free_type_font)->_font_size_height ) == 0 )
											_glyph_slot = ((freetype_font*)_free_type_font)->_ft_face->glyph;
									}
								}
								_declare_stackvar( ::rux::uint32 , utf32_character , 0 );
								::rux::uint32 temp_utf8_character = utf8_character;
								::rux::XString::utf8_to_utf32( (char*)&temp_utf8_character , ::rux::XString::utf8_symbol_length( (char*)&temp_utf8_character , 0 ) , (char*)&utf32_character , 4 );
								if( FT_Load_Char( ((freetype_font*)_free_type_font)->_ft_face , utf32_character , FT_LOAD_RENDER ) == 0 ) 
								{						
									_atlas_height = ( ((freetype_font*)_free_type_font)->_ft_face->size->metrics.ascender - ((freetype_font*)_free_type_font)->_ft_face->size->metrics.descender ) / 64; 		
									_character_height = _atlas_height;
									ui_font_info = alloc_object_macros( RenderCharacterInfo );
									ui_font_info->_utf8 = utf8_character;
									ui_font_info->_width = _glyph_slot->bitmap.width;
									ui_font_info->_height = _glyph_slot->bitmap.rows;
									CS_PTR_UNLOCK( ::rux::gui::engine::_cs_rux_fonts );									
									WRITE_LOCK( _cs_characters );
									_characters.Add( ui_font_info );
									_cs_characters.WriteUnlock();
									return (float)ui_font_info->_width;
								}
								else
								{
									ui_font_info = find_character( 0x0000FFFD );
									if( ui_font_info )
									{
										CS_PTR_UNLOCK( ::rux::gui::engine::_cs_rux_fonts );
										return (float)ui_font_info->_width;
									}
									else
									{
										if( FT_Load_Char( ((freetype_font*)_free_type_font)->_ft_face , 0x0000FFFD , FT_LOAD_RENDER ) == 0 ) 
										{						
											_atlas_height = ( ((freetype_font*)_free_type_font)->_ft_face->size->metrics.ascender - ((freetype_font*)_free_type_font)->_ft_face->size->metrics.descender ) / 64; 		
											_character_height = _atlas_height;
											ui_font_info = alloc_object_macros( RenderCharacterInfo );
											ui_font_info->_utf8 = 0x0000FFFD;
											ui_font_info->_width = _glyph_slot->bitmap.width;
											ui_font_info->_height = _glyph_slot->bitmap.rows;
											CS_PTR_UNLOCK( ::rux::gui::engine::_cs_rux_fonts );											
											WRITE_LOCK( _cs_characters );
											_characters.Add( ui_font_info );
											_cs_characters.WriteUnlock();
											return (float)ui_font_info->_width;
										}						
										else
										{
											CS_PTR_UNLOCK( ::rux::gui::engine::_cs_rux_fonts );
											return 0;
										}
									}
								}
							}
							else return 0;
						}			
					}
				}
			};
		};
	};
};