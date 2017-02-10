#ifndef XAN_RENDER_CONTEXT_H
#define XAN_RENDER_CONTEXT_H
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
#include <xan_gl_context.h>

#ifndef BOOLDOG_HEADER
#define BOOLDOG_HEADER( header ) <booldog/header>
#endif
#include BOOLDOG_HEADER(boo_interlocked.h)

namespace rux
{
	namespace gui
	{
		namespace engine
		{
#define declare_opengl_function_object_in_class_macros( FUNC ) ::rux::gui::engine::PFN##FUNC##PROC _##FUNC
			class Window;
			class RenderFont;
			class RenderContext : public RenderContextBase
			{
				friend class Window;
				friend class ::rux::gui::engine::OpenGL;
				friend class ::rux::gui::engine::RenderThread;
				friend class RenderFont;
				friend class ::rux::gui::engine::RenderUniversalCache;
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
				::booldog::interlocked::atomic _enable_state;

				size_t _draw_immediately_index;
				XMallocArray< ::rux::gui::engine::RenderUniversalCache* > _draw_immediately0;
				XMallocArray< ::rux::gui::engine::RenderUniversalCache* > _draw_immediately1;
				XMallocArray< ::rux::gui::engine::RenderUniversalCache* > _vertices_owner;

				XUIColor* _colors_ptr0;				
				XUIVertexF* _vertices_ptr0;				
				XUITextureCoordinate* _texture_coords_ptr0;
				
				size_t _colors_ptr0_count;
				size_t _vertices_ptr0_count;
				size_t _texture_coords_ptr0_count;

				XUIColor* _colors_ptr1;
				XUIVertexF* _vertices_ptr1;
				XUITextureCoordinate* _texture_coords_ptr1;

				size_t _colors_ptr1_count;
				size_t _vertices_ptr1_count;
				size_t _texture_coords_ptr1_count;
				
				size_t _vertices_offset;
				GLenum _gl_multi_draw_arrays_mode;
				::rux::byte _gl_multi_draw_arrays_is_alpha;
				XMallocArray< GLint > _gl_multi_draw_arrays_first;
				XMallocArray< GLsizei > _gl_multi_draw_arrays_count;

				::rux::gui::engine::RenderThread* _render_thread;
				rux::int32 _new_width;
				rux::int32 _new_height;
				rux::gui::engine::Window* _window;
				rux::gui::Rectangle _clip;
				rux::int32 _stencil_ref;
			public:
				::rux::uint32 _render_timeout;
				::rux::uint32 _render_time;
				::rux::gui::engine::Fps _fps;
				::rux::gui::engine::GLContext* _gl;
				size_t _render_objects_count;
				bool _need_restart;
				void Render( size_t ___rux__thread_index1986 );
				void CleanAfterRender( void );
			public:
				RenderContext( rux::gui::engine::Window* window , rux::uint32 width , rux::uint32 height );
				~RenderContext();
				void disable_render(void);
				void enable_render(void);
				void DestroyRenderThread( void );
				friend dll_internal rux::uint8 dispatch( rux::gui::rux_dispatcher_t rux_dispatcher , void* param , rux::gui::WindowBase* window_base , rux::uint8 stop_render , rux::uint8 no_send_from_render , rux::uint8 wait );
				bool check_cache( rux::gui::RenderCacheBase** cache , const char* __filename__ , rux::int32 __line__ );
				void DrawRectangleByShader( ::rux::int32 left , ::rux::int32 top , ::rux::int32 width , ::rux::int32 height , ::rux::int32 border_width , ::rux::gui::ColorBase* background_object_color , ::rux::gui::Color* border_color , const ::rux::gui::Thickness& corner , float opacity , ::rux::gui::RenderCacheBase** cache , float& selected_z_index , size_t ___rux__thread_index1986 );
				void DrawRectangleSimpleBorder( ::rux::int32 left , ::rux::int32 top , ::rux::int32 width , ::rux::int32 height , ::rux::int32 border_width , rux::gui::ColorBase* background_object_color , float opacity , rux::gui::RenderCacheBase** cache , float& _selected_z_index );
				void DrawRectangleSimple( ::rux::int32 left , ::rux::int32 top , ::rux::int32 width , ::rux::int32 height , ::rux::int32 border_width , rux::gui::ColorBase* background_object_color , float opacity , rux::gui::RenderCacheBase** cache , float& _selected_z_index );
				void DrawRectangleRound( ::rux::int32 left , ::rux::int32 top , ::rux::int32 width , ::rux::int32 height , ::rux::int32 border_width , ::rux::gui::ColorBase* background_object_color , const ::rux::gui::Thickness& corner , float opacity , ::rux::gui::RenderCacheBase** cache , float& _selected_z_index );
				void DrawRectangleRoundBorder( ::rux::int32 left , ::rux::int32 top , ::rux::int32 width , ::rux::int32 height , ::rux::int32 border_width , ::rux::gui::ColorBase* border_color , const ::rux::gui::Thickness& corner , float opacity , ::rux::gui::RenderCacheBase** cache , float& _selected_z_index );
				void DrawImmediately( size_t ___rux__thread_index1986 );
				void Resize( ::rux::int32 width , ::rux::int32 height );
				virtual void DrawImage( rux::int32 fourcc , ::rux::int32 left , ::rux::int32 top , ::rux::int32 width , ::rux::int32 height , float opacity , rux::gui::RenderCacheBase** cache , float brightness , float contrast , float& _selected_z_index , rux::uint8 is_alpha , rux::int32 image_left , rux::int32 image_top , rux::int32 image_width , rux::int32 image_height , const char* __filename__ , ::rux::int32 __line__ , size_t ___rux__thread_index1986 );
				virtual void DrawRectangle( ::rux::int32 left , ::rux::int32 top , ::rux::int32 width , ::rux::int32 height , ::rux::int32 border_width , rux::gui::ColorBase* background_object_color , rux::gui::Color* border , const rux::gui::Thickness& corner , float opacity , rux::gui::RenderCacheBase** border_cache , rux::gui::RenderCacheBase** cache , float& _selected_z_index , rux::uint8 smooth , const char* __filename__ , rux::int32 __line__ , size_t ___rux__thread_index1986 );
				virtual void DrawPath( ::rux::int32 left , ::rux::int32 top , ::rux::int32 width , ::rux::int32 height , XMallocArray< rux::gui::Point* >& points , float opacity , rux::gui::ColorBase* background_object_color , rux::gui::RenderCacheBase** cache , float& _selected_z_index , rux::uint8 smooth , size_t ___rux__thread_index1986 );
				virtual void DrawPath( ::rux::int32 left , ::rux::int32 top , ::rux::int32 width , ::rux::int32 height , XMallocArray< float >& point_x , XMallocArray< float >& point_y , float opacity , rux::gui::ColorBase* background_object_color , rux::gui::RenderCacheBase** cache , float& _selected_z_index , rux::uint8 smooth , size_t ___rux__thread_index1986 );
				virtual void DrawText( ::rux::int32 left , ::rux::int32 top , ::rux::int32 width , ::rux::int32 height , const char* utf8text , rux::gui::FontBase* font , rux::gui::Color* color , rux::gui::RenderCacheBase** cache , float opacity , float& _selected_z_index , const char* __filename__ , rux::int32 __line__ , size_t ___rux__thread_index1986 );
				virtual void DrawLine( ::rux::int32 start_left , ::rux::int32 start_top , ::rux::int32 end_left , ::rux::int32 end_top , ::rux::int32 thickness , float opacity , rux::gui::ColorBase* background_object_color , ::rux::gui::RenderCacheBase** cache , ::rux::gui::RenderCacheBase** cap1_cache , ::rux::gui::RenderCacheBase** cap2_cache , float& _selected_z_index , size_t ___rux__thread_index1986 );
				virtual void set_Clip( rux::gui::Rectangle clip );
				virtual rux::gui::Rectangle& get_Clip( void );
				void add_color_and_vertex_to_cache( float x , float y , float width , float height , float border_width , float selected_z_index , float opacity , rux::gui::ColorBase* color , rux::gui::engine::RenderUniversalCache* cache );
				void update_color_to_cache( float width , float height , float border_width , float opacity , rux::gui::ColorBase* color , rux::gui::engine::RenderUniversalCache* cache );

				void update_round_color_to_cache( float left , float top , float width , float height , float border_width , ::rux::gui::ColorBase* color , const ::rux::gui::Thickness& corner , float opacity , RenderUniversalCache* ui_cache );
				void add_round_color_and_vertex_to_cache( float left , float top , float width , float height , float border_width , ::rux::gui::ColorBase* background_object_color , const ::rux::gui::Thickness& corner , float opacity , RenderUniversalCache* ui_cache , float& selected_z_index );
				void add_round_cut_stencil_vertex_to_cache( float left , float top , float width , float height , float border_width , const ::rux::gui::Thickness& corner , RenderUniversalCache* ui_cache , float& selected_z_index );
				void add_round_fill_stencil_vertex_to_cache( float left , float top , float width , float height , float border_width , const ::rux::gui::Thickness& corner , RenderUniversalCache* ui_cache , float& selected_z_index );
				void add_border_color_and_vertex_to_cache( float x , float y , float width , float height , float border_width , float selected_z_index , float opacity , rux::gui::ColorBase* color , rux::gui::engine::RenderUniversalCache* cache );
				void update_border_color_to_cache( float width , float height , float border_width , float opacity , rux::gui::ColorBase* color , rux::gui::engine::RenderUniversalCache* cache );

				void line( float x1 , float y1 , float x2 , float y2 , float w , GLubyte r , GLubyte g , GLubyte b , GLubyte a , RenderUniversalCache* ui_cache , RenderUniversalCache* ui_cap1_cache , RenderUniversalCache* ui_cap2_cache , float selected_z_index );
				void GenBuffers( rux::uint32*& buffer_ptr , ::rux::uint32& drawing_buffer_ptr_size , size_t new_size , size_t ___rux__thread_index1986 );
				void DeleteBuffers( rux::uint32*& buffer_ptr , ::rux::uint32& drawing_buffer_ptr_size , size_t ___rux__thread_index1986 );
				void MapBuffer( rux::uint32* buffer_ptr , GLubyte*& mapped_buffer , size_t ___rux__thread_index1986 );
				rux_inline void rux_draw_rectangle_and_save_to_fill_stencil( RenderContext* render_context , float x , float y , float width , float height , float selected_z_index , rux::gui::engine::RenderUniversalCache* cache )
				{		
					cache->AddFillStencilVertex( render_context , XUIVertexF( x , y , selected_z_index ) );
					cache->AddFillStencilVertex( render_context , XUIVertexF( x + width , y , selected_z_index ) );
					cache->AddFillStencilVertex( render_context , XUIVertexF( x + width , y + height , selected_z_index ) );		
					cache->AddFillStencilVertex( render_context , XUIVertexF( x + width , y + height , selected_z_index ) );
					cache->AddFillStencilVertex( render_context , XUIVertexF( x , y , selected_z_index ) );
					cache->AddFillStencilVertex( render_context , XUIVertexF( x , y + height , selected_z_index ) );
				};
				rux_inline void rux_draw_rectangle_and_save_to_cut_stencil( RenderContext* render_context , float x , float y , float width , float height , float selected_z_index , rux::gui::engine::RenderUniversalCache* cache )
				{		
					cache->AddCutStencilVertex( render_context , XUIVertexF( x , y , selected_z_index ) );
					cache->AddCutStencilVertex( render_context , XUIVertexF( x + width , y , selected_z_index ) );
					cache->AddCutStencilVertex( render_context , XUIVertexF( x + width , y + height , selected_z_index ) );
					cache->AddCutStencilVertex( render_context , XUIVertexF( x + width , y + height , selected_z_index ) );
					cache->AddCutStencilVertex( render_context , XUIVertexF( x , y , selected_z_index ) );
					cache->AddCutStencilVertex( render_context , XUIVertexF( x , y + height , selected_z_index ) );
				};	
				rux_inline void rux_draw_rectangle_and_save_to_drawing_vertex( float x , float y , float width , float height , float selected_z_index , XMallocArray< XUIVertexF >& cache )
				{
					cache.Add( XUIVertexF( x , y , selected_z_index ) );		
					cache.Add( XUIVertexF( x + width , y , selected_z_index ) );		
					cache.Add( XUIVertexF( x + width , y + height , selected_z_index ) );				
					cache.Add( XUIVertexF( x + width , y + height , selected_z_index ) );		
					cache.Add( XUIVertexF( x , y , selected_z_index ) );		
					cache.Add( XUIVertexF( x , y + height , selected_z_index ) );	
				};
				rux_inline void rux_draw_rectangle_and_save_to_drawing_vertex( RenderContext* render_context , float x , float y , float width , float height , float selected_z_index , rux::gui::engine::RenderUniversalCache* cache , ::rux::gui::engine::XUIColor* color )
				{
					cache->AddDrawingVertex( render_context , XUIVertexF( x , y , selected_z_index ) , color , NULL );
					cache->AddDrawingVertex( render_context , XUIVertexF( x + width , y , selected_z_index ) , color , NULL );
					cache->AddDrawingVertex( render_context , XUIVertexF( x + width , y + height , selected_z_index ) , color , NULL );
					cache->AddDrawingVertex( render_context , XUIVertexF( x + width , y + height , selected_z_index ) , color , NULL );
					cache->AddDrawingVertex( render_context , XUIVertexF( x , y , selected_z_index ) , color , NULL );
					cache->AddDrawingVertex( render_context , XUIVertexF( x , y + height , selected_z_index ) , color , NULL );
				};
			};
		};
	};
};
#endif
