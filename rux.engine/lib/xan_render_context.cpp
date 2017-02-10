#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_render_context.h>
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
			RenderContext::RenderContext( ::rux::gui::engine::Window* window , ::rux::uint32 width , ::rux::uint32 height )
				: _enable_state(1), _need_restart(false)
			{
				::rux::log::WriteDebug( "%s:%d BEGIN" , __FUNCTION__ , __LINE__ );
				if( ::rux::gui::engine::_displays_count > 0 && ::rux::gui::engine::_display_frequencies[ 0 ] != 0 )
					_render_timeout = 1000U / ::rux::gui::engine::_display_frequencies[ 0 ];
				else
					_render_timeout = 17U;
				_render_time = 0U;
				_colors_ptr0 = NULL;
				_vertices_ptr0 = NULL;
				_texture_coords_ptr0 = NULL;
				_colors_ptr1 = NULL;
				_vertices_ptr1 = NULL;
				_texture_coords_ptr1 = NULL;

				_colors_ptr0_count = 0;
				_vertices_ptr0_count = 0;
				_texture_coords_ptr0_count = 0;
				_colors_ptr1_count = 0;
				_vertices_ptr1_count = 0;
				_texture_coords_ptr1_count = 0;

				_gl_multi_draw_arrays_is_alpha = 0;
				_draw_immediately_index = 0;
				_vertices_offset = 0;
				_gl_multi_draw_arrays_mode = GL_TRIANGLES;

				_render_objects_count = 0;
				_window = window;
				_new_width = width;
				_new_height = height;
				::rux::byte create_gl_context = 1;
#ifdef __WINDOWS__
				if( 0 && rux::gui::engine::_opengl_one_thread_swap_buffers )
				{
					PIXELFORMATDESCRIPTOR pfd;
					pfd.nSize = sizeof( PIXELFORMATDESCRIPTOR );
					pfd.nVersion = 1;
					pfd.dwFlags = window->_is_transparent_window == 1 ? PFD_DRAW_TO_BITMAP | PFD_SUPPORT_OPENGL : PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
					pfd.iPixelType = PFD_TYPE_RGBA;
					pfd.cColorBits = 24;//pfd.cColorBits     = 32;
					pfd.cRedBits = 24;//_pfd.cRedBits       = 8;
					pfd.cRedShift = 0;//_pfd.cRedShift      = 16;
					pfd.cGreenBits = 24;//_pfd.cGreenBits     = 8;
					pfd.cGreenShift = 0;//_pfd.cGreenShift    = 8;
					pfd.cBlueBits = 24;//_pfd.cBlueBits      = 8;
					pfd.cBlueShift = 0;
					pfd.cAlphaBits = 0;
					pfd.cAlphaShift = 0;
					pfd.cAccumBits = 64;    //_pfd.cAccumBits     = 64;    
					pfd.cAccumRedBits = 16;//_pfd.cAccumRedBits  = 16;
					pfd.cAccumGreenBits = 16;//_pfd.cAccumGreenBits   = 16;
					pfd.cAccumBlueBits = 16;//_pfd.cAccumBlueBits    = 16;
					pfd.cAccumAlphaBits = 0;
					pfd.cDepthBits = 24;
					pfd.cStencilBits = 24;//_pfd.cStencilBits      = 8;
					pfd.cAuxBuffers = 0;
					pfd.iLayerType = PFD_MAIN_PLANE;
					pfd.bReserved = 0;
					pfd.dwLayerMask = 0;
					pfd.dwVisibleMask = 0;
					pfd.dwDamageMask = 0;
					rux::int32 pixel_format_index = ChoosePixelFormat( window->_hdc , &pfd );
					if( pixel_format_index == 0 )
					{
						pixel_format_index = 1;
						if( DescribePixelFormat( window->_hdc , pixel_format_index , sizeof( PIXELFORMATDESCRIPTOR ) , &pfd ) == 0 )
						{
						}
					}
					if ( SetPixelFormat( window->_hdc, pixel_format_index, &pfd ) == FALSE )
						printf( "SetPixelFormat FALSE\n" );
					else
					{
						CS_PTR_LOCK( ::rux::gui::engine::_cs_windows );
						for( size_t index0 = 0 ; index0 < ::rux::gui::engine::_windows.Count() ; index0++ )
						{
							if( ::rux::gui::engine::_windows[ index0 ] )
							{
								::rux::gui::engine::RenderContext* render_context = ::rux::gui::engine::_windows[ index0 ]->_render_context;
								if( render_context )
								{
									if( memcmp( &pfd , &render_context->_gl->_pfd , sizeof( PIXELFORMATDESCRIPTOR ) ) == 0 )
									{
										create_gl_context = 0;
										render_context->_gl->AddRef();
										_gl = render_context->_gl;
										break;
									}
								}
							}
						}
						CS_PTR_UNLOCK( ::rux::gui::engine::_cs_windows );
					}
				}
#endif
				if( create_gl_context )
					_gl = alloc_object_1_param_macros( ::rux::gui::engine::GLContext , window );	
				_gl->set_CurrentContext( window );
				_gl->Resize(_need_restart, width , height , SIZE_MAX );
				_gl->set_EmptyContext();
				if( rux::gui::engine::_opengl_one_thread_swap_buffers == 0 )
					_render_thread = alloc_object_1_param_macros( ::rux::gui::engine::RenderThread , this );
				else
					_render_thread = 0;
				::rux::log::WriteDebug( "%s:%d END" , __FUNCTION__ , __LINE__ );
			};
			void RenderContext::disable_render(void)
			{
				for(;;)
				{
					::booldog::interlocked::atomic_return res = 
						::booldog::interlocked::compare_exchange(&_enable_state, 0, 1);
					if(res == ::booldog::interlocked::max)
						continue;
					break;
				}
			};
			void RenderContext::enable_render(void)
			{
				for(;;)
				{
					::booldog::interlocked::atomic_return res = 
						::booldog::interlocked::compare_exchange(&_enable_state, 1, 0);
					if(res == ::booldog::interlocked::max)
						continue;
					break;
				}
			};
			void RenderContext::DestroyRenderThread( void )
			{
				if( _render_thread )
				{
					::rux::engine::free_object< ::rux::gui::engine::RenderThread >( _render_thread );
					_render_thread = NULL;
				}
			};
			RenderContext::~RenderContext()
			{
				DestroyRenderThread();
				_gl->set_CurrentContext( _window );
				for( size_t index0 = 0 ; index0 < _draw_immediately0.Count() ; index0++ )
					_draw_immediately0[ index0 ]->Release( __FILE__ , __LINE__ );
				for( size_t index0 = 0 ; index0 < _draw_immediately1.Count() ; index0++ )
					_draw_immediately1[ index0 ]->Release( __FILE__ , __LINE__ );
				_gl->set_EmptyContext();
				_gl->Release();
				::rux::memory::release< ::rux::gui::engine::XUIColor >( _colors_ptr0 );
				::rux::memory::release< ::rux::gui::engine::XUIVertexF >( _vertices_ptr0 );
				::rux::memory::release< ::rux::gui::engine::XUITextureCoordinate >( _texture_coords_ptr0 );
				::rux::memory::release< ::rux::gui::engine::XUIColor >( _colors_ptr1 );
				::rux::memory::release< ::rux::gui::engine::XUIVertexF >( _vertices_ptr1 );
				::rux::memory::release< ::rux::gui::engine::XUITextureCoordinate >( _texture_coords_ptr1 );
			};
			static inline double GET_ABS(double x) {return x>0?x:-x;}
			void RenderContext::line( float x1, float y1, float x2, float y2, //coordinates of the line
				float thickness,			//width/thickness of the line in pixel
				GLubyte Cr, GLubyte Cg, GLubyte Cb,	//RGB color components
				GLubyte Br , RenderUniversalCache* ui_cache , RenderUniversalCache* ui_cap1_cache , RenderUniversalCache* ui_cap2_cache , float selected_z_index )		//use alpha blend or not
			{
				double t = 0;
				double R = 0;
				double f = thickness - static_cast< int >( thickness );
				float A = 0;
				A = Br;
				//determine parameters t,R
				if( thickness >= 0.0 && thickness < 1.0 )
				{
					t = 0.05;
					R = 0.48 + 0.32 * f;
					A *= f;
				} 
				else if( thickness >= 1.0 && thickness < 2.0 )
				{
					t = 0.05 + f * 0.33;
					R = 0.768 + 0.312 * f;
				}
				else if( thickness >= 2.0 && thickness < 3.0 )
				{
					t = 0.38 + f * 0.58;
					R = 1.08;
				}
				else if( thickness >= 3.0 && thickness < 4.0 )
				{
					t = 0.96 + f * 0.48;
					R = 1.08;
				}
				else if( thickness >= 4.0 && thickness < 5.0 )
				{
					t = 1.44 + f * 0.46;
					R = 1.08;
				}
				else if( thickness >= 5.0 && thickness < 6.0 )
				{
					t = 1.9 + f * 0.6;
					R = 1.08;
				}
				else if( thickness >= 6.0 )
				{
					double ff = thickness - 6.0;
					t = 2.5 + ff * 0.50;
					R = 1.08;
				}
				//determine angle of the line to horizontal
				double tx = 0 , ty = 0; //core thinkness of a line
				double Rx = 0 , Ry = 0; //fading edge of a line
				double cx = 0 , cy = 0; //cap of a line
				double ALW = 0.01;
				double dx = x2 - x1;
				double dy = y2 - y1;
				if( GET_ABS( dx ) < ALW )
				{
					//vertical
					tx = t;
					ty = 0;
					Rx = R;
					Ry = 0;
					if( thickness > 0.0 && thickness <= 1.0 )
					{
						tx = 0.5;
						Rx = 0.0;
					}
				}
				else if( GET_ABS( dy ) < ALW )
				{
					//horizontal
					tx = 0;
					ty = t;
					Rx = 0; 
					Ry = R;
					if( thickness > 0.0 && thickness <= 1.0 )
					{
						ty = 0.5;
						Ry = 0.0;
					}
				} 
				else
				{
					if( thickness < 3 )
					{ //approximate to make things even faster
						double m = dy / dx;
						//and calculate tx,ty,Rx,Ry
						if( m > -0.4142 && m <= 0.4142 )
						{
							// -22.5< angle <= 22.5, approximate to 0 (degree)
							tx = t*0.1;
							ty = t;
							Rx = R*0.6;
							Ry = R;
						} 
						else if( m > 0.4142 && m <= 2.4142 )
						{
							// 22.5< angle <= 67.5, approximate to 45 (degree)
							tx=t*-0.7071; ty=t*0.7071;
							Rx=R*-0.7071; Ry=R*0.7071;
						} else if ( m>2.4142 || m<=-2.4142) {
							// 67.5 < angle <=112.5, approximate to 90 (degree)
							tx=t; ty=t*0.1;
							Rx=R; Ry=R*0.6;
						} else if ( m>-2.4142 && m<-0.4142) {
							// 112.5 < angle < 157.5, approximate to 135 (degree)
							tx=t*0.7071; ty=t*0.7071;
							Rx=R*0.7071; Ry=R*0.7071;
						} else {
							// error in determining angle
							//printf( "error in determining angle: m=%.4f\n",m);
						}
					} else { //calculate to exact
						dx=y1-y2;
						dy=x2-x1;
						double L=sqrt(dx*dx+dy*dy);
						dx/=L;
						dy/=L;
						cx=-dy; cy=dx;
						tx=t*dx; ty=t*dy;
						Rx=R*dx; Ry=R*dy;
					}
				}	
				x1 += cx * 0.5f;
				y1 += cy * 0.5f;
				x2 -= cx * 0.5f;
				y2 -= cy * 0.5f;
				XUIColor colorf_empty( Cr , Cg , Cb , 0 );
				XUIColor colorf( Cr , Cg , Cb , A );
				ui_cache->AddDrawingVertex( this , XUIVertexF( x1 - tx - Rx - cx , y1 - ty - Ry - cy , selected_z_index ) , &colorf_empty , NULL );
				ui_cache->AddDrawingVertex( this , XUIVertexF( x2-tx-Rx+cx, y2-ty-Ry+cy , selected_z_index ) , &colorf_empty , NULL );
				ui_cache->AddDrawingVertex( this , XUIVertexF( x1-tx-cx,y1-ty-cy , selected_z_index ) , &colorf , NULL );
				ui_cache->AddDrawingVertex( this , XUIVertexF( x2-tx+cx,y2-ty+cy , selected_z_index ) , &colorf , NULL );
				ui_cache->AddDrawingVertex( this , XUIVertexF( x1+tx-cx,y1+ty-cy , selected_z_index ) , &colorf , NULL );
				ui_cache->AddDrawingVertex( this , XUIVertexF( x2+tx+cx,y2+ty+cy , selected_z_index ) , &colorf , NULL );
				ui_cache->AddDrawingVertex( this , XUIVertexF( x1+tx+Rx-cx, y1+ty+Ry-cy ,  selected_z_index ) , &colorf_empty , NULL );
				ui_cache->AddDrawingVertex( this , XUIVertexF( x2+tx+Rx+cx, y2+ty+Ry+cy , selected_z_index ) , &colorf_empty , NULL );
				//cap
				if ( thickness < 3) 
				{
				}
				else
				{
					ui_cap1_cache->AddDrawingVertex( this , XUIVertexF( x1-tx-Rx-cx, y1-ty-Ry-cy, selected_z_index ) , &colorf_empty , NULL );
					ui_cap1_cache->AddDrawingVertex( this , XUIVertexF( x1-tx-Rx, y1-ty-Ry, selected_z_index ) , &colorf_empty , NULL );
					ui_cap1_cache->AddDrawingVertex( this , XUIVertexF( x1-tx-cx, y1-ty-cy, selected_z_index ) , &colorf , NULL );
					ui_cap1_cache->AddDrawingVertex( this , XUIVertexF( x1+tx+Rx, y1+ty+Ry, selected_z_index ) , &colorf_empty , NULL );
					ui_cap1_cache->AddDrawingVertex( this , XUIVertexF( x1+tx-cx, y1+ty-cy, selected_z_index ) , &colorf , NULL );
					ui_cap1_cache->AddDrawingVertex( this , XUIVertexF( x1+tx+Rx-cx, y1+ty+Ry-cy, selected_z_index ) , &colorf_empty , NULL );

					ui_cap2_cache->AddDrawingVertex( this , XUIVertexF( x2-tx-Rx+cx, y2-ty-Ry+cy, selected_z_index ) , &colorf_empty , NULL );
					ui_cap2_cache->AddDrawingVertex( this , XUIVertexF( x2-tx-Rx, y2-ty-Ry, selected_z_index ) , &colorf_empty , NULL );
					ui_cap2_cache->AddDrawingVertex( this , XUIVertexF( x2-tx+cx, y2-ty+cy, selected_z_index ) , &colorf , NULL );
					ui_cap2_cache->AddDrawingVertex( this , XUIVertexF( x2+tx+Rx, y2+ty+Ry, selected_z_index ) , &colorf_empty , NULL );
					ui_cap2_cache->AddDrawingVertex( this , XUIVertexF( x2+tx+cx, y2+ty+cy, selected_z_index ) , &colorf , NULL );
					ui_cap2_cache->AddDrawingVertex( this , XUIVertexF( x2+tx+Rx+cx, y2+ty+Ry+cy , selected_z_index ) , &colorf_empty , NULL );
				}
			};
			void RenderContext::Resize( ::rux::int32 width , ::rux::int32 height )
			{
				_new_width = width;
				_new_height = height;
			};
			void RenderContext::DrawImmediately( size_t ___rux__thread_index1986 )
			{
				rux::gui::engine::OpenGL::glEnableClientState( GL_VERTEX_ARRAY , _gl , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
				rux::gui::engine::OpenGL::glEnableClientState( GL_TEXTURE_COORD_ARRAY , _gl , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
				rux::gui::engine::OpenGL::glEnableClientState( GL_COLOR_ARRAY , _gl , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
				{
					if( _draw_immediately_index == 0 )
					{
						rux::gui::engine::OpenGL::glVertexPointer( 3 , GL_FLOAT , 0 , _vertices_ptr0 , _gl , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						rux::gui::engine::OpenGL::glColorPointer( 4 , GL_UNSIGNED_BYTE , 0 , _colors_ptr0 , _gl , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					}
					else
					{
						rux::gui::engine::OpenGL::glVertexPointer( 3 , GL_FLOAT , 0 , _vertices_ptr1 , _gl , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
						rux::gui::engine::OpenGL::glColorPointer( 4 , GL_UNSIGNED_BYTE , 0 , _colors_ptr1 , _gl , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					}
				}
				if( _draw_immediately_index == 0 )
					rux::gui::engine::OpenGL::glTexCoordPointer( 2 , GL_SHORT , 0 , _texture_coords_ptr0 , _gl , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
				else
					rux::gui::engine::OpenGL::glTexCoordPointer( 2 , GL_SHORT , 0 , _texture_coords_ptr1 , _gl , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
				if( _draw_immediately_index == 0 )
				{	
					{
						for( size_t index0 = 0 ; index0 < _draw_immediately0.Count() ; index0++ )
							_draw_immediately0[ index0 ]->Draw( this , ___rux__thread_index1986 );
					}
					{
						for( size_t index1 = 0 ; index1 < _draw_immediately1.Count() ; index1++ )
						{
							if( _draw_immediately1[ index1 ]->_not_drawn )
							{
								_draw_immediately1[ index1 ]->Clean();
								_draw_immediately1[ index1 ]->Reset();								
							}
							_draw_immediately1[ index1 ]->Release( __FILE__ , __LINE__ );
						}
						_draw_immediately1.Clear();
					}
					{
						for( size_t index0 = 0 ; index0 < _draw_immediately0.Count() ; index0++ )
							_draw_immediately0[ index0 ]->_not_drawn = 1;
					}
					_draw_immediately_index = 1;
				}
				else
				{
					{
						for( size_t index0 = 0 ; index0 < _draw_immediately1.Count() ; index0++ )
							_draw_immediately1[ index0 ]->Draw( this , ___rux__thread_index1986 );
					}
					{
						for( size_t index1 = 0 ; index1 < _draw_immediately0.Count() ; index1++ )
						{
							if( _draw_immediately0[ index1 ]->_not_drawn )
							{
								_draw_immediately0[ index1 ]->Clean();								
								_draw_immediately0[ index1 ]->Reset();								
							}
							_draw_immediately0[ index1 ]->Release( __FILE__ , __LINE__ );
						}
						_draw_immediately0.Clear();
					}
					{
						for( size_t index0 = 0 ; index0 < _draw_immediately1.Count() ; index0++ )
							_draw_immediately1[ index0 ]->_not_drawn = 1;
					}
					_draw_immediately_index = 0;
				}
			};
			#define BUG 0
			void RenderContext::CleanAfterRender( void )
			{
				WRITE_LOCK( _gl->_cs_caches );
				size_t copy_items = _gl->_render_caches.ItemsCount() / 2;
				if( copy_items )
				{
					for( size_t index0 = 0 ; index0 < _gl->_render_caches.Count() ; index0++ )
					{
						::rux::gui::engine::RenderUniversalCache* cache = _gl->_render_caches[ index0 ];
						if( cache )
						{
							_gl->_render_caches.set_EmptyAt( index0 );
							_gl->_cs_caches.WriteUnlock();
							cache->Release( __FILE__ , __LINE__ );
							copy_items--;
							if( copy_items == 0 )
							{
								WRITE_LOCK( _gl->_cs_caches );
								break;
							}
							WRITE_LOCK( _gl->_cs_caches );
						}
					}		
				}
				_gl->_cs_caches.WriteUnlock();
			};
			void RenderContext::Render( size_t ___rux__thread_index1986 )
			{
				_gl->Resize(_need_restart, _new_width , _new_height , ___rux__thread_index1986 );
				_render_objects_count = 0;
				if( 0 && _gl->_is_support_framebuffer_object == 1 )
					rux::gui::engine::OpenGL::glBindFramebuffer( GL_FRAMEBUFFER , _gl->_framebuffer_ptr[ 0 ] , _gl , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
#ifdef __WINDOWS__
				HDC hdc = NULL;
				PAINTSTRUCT paint_struct;
				if( _window->_is_transparent_window == 1 )
					hdc = BeginPaint( _window->_hwnd , &paint_struct );
#endif
				_vertices_offset = 0;
				_stencil_ref = 1;
				rux::gui::engine::OpenGL::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
				rux::gui::engine::OpenGL::glLoadIdentity( __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
				rux::gui::engine::OpenGL::glEnable( GL_DEPTH_TEST , _gl , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
				::rux::gui::engine::OpenGL::glScalef( 1.0f , 1.0f , 1.0f / 1000000.f , _gl , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
#if BUG				
				if( colors.Count() == 0 )
				{
					float z = 0 , x = 0 , r = 1;
					for( size_t index0 = 0 ; index0 < 10000 ; index0++ )
					{
						colors.Add( XUIColor( r , 0 , 0 , 1 ) );
						colors.Add( XUIColor( r , 0 , 0 , 1 ) );
						colors.Add( XUIColor( r , 0 , 0 , 1 ) );
						r -= 0.1f;
						if( r <= 0 || r > 1 )
							r = 1;

						colors.Add( XUIColor( r , 1 , 0 , 1 ) );
						colors.Add( XUIColor( r , 1 , 0 , 1 ) );
						colors.Add( XUIColor( r , 1 , 0 , 1 ) );
						colors.Add( XUIColor( r , 1 , 0 , 1 ) );
						colors.Add( XUIColor( r , 1 , 0 , 1 ) );
						colors.Add( XUIColor( r , 1 , 0 , 1 ) );

				
				
						vertices.Add( XUIVertexF( x , x , z ) );
						vertices.Add( XUIVertexF( x , 60 + x , z ) );
						vertices.Add( XUIVertexF( 40 + x , 40 + x , z ) );
						z += 1;
						rux_draw_rectangle_and_save_to_drawing_vertex( x + 60 , 70 + x , 40 , 30 , z , vertices );
						z += 1;
						x += 5;

						
					}
					GLint first = 0;
					for( size_t index0 = 0 ; index0 < 10000 ; index0++ )
					{
						firsts.Add( first );
						counts.Add( 3 );
						first += 3;
						firsts.Add( first );
						counts.Add( 6 );
						first += 6;
					}
				}				

				rux::gui::engine::OpenGL::glEnableClientState( GL_COLOR_ARRAY , this );
				rux::gui::engine::OpenGL::glColorPointer( 4 , GL_UNSIGNED_BYTE , 0 , colors._array_ptr , this );

				rux::gui::engine::OpenGL::glEnableClientState( GL_VERTEX_ARRAY , this );
				rux::gui::engine::OpenGL::glVertexPointer( 3 , GL_FLOAT , 0 , vertices._array_ptr , this );

				if( _glMultiDrawArrays )
				{
					::rux::gui::engine::OpenGL::glMultiDrawArrays( GL_TRIANGLES , firsts._array_ptr , counts._array_ptr , firsts.Count() , this );
				}
				else
				{
					GLint first = 0;
					for( size_t index0 = 0 ; index0 < 10000 ; index0++ )
					{
						rux::gui::engine::OpenGL::glDrawArrays( GL_TRIANGLES , first , 3 );
						first += 3;

						rux::gui::engine::OpenGL::glDrawArrays( GL_TRIANGLES , first , 6 );
						first += 6;
					}
				}

				/*rux::gui::engine::OpenGL::glColor4f( 1 , 0 , 0 , 1 );
				rux::gui::engine::OpenGL::glBegin( GL_TRIANGLES );		
				rux::gui::engine::OpenGL::glVertex3f( 0 , 0 , 0 );
				rux::gui::engine::OpenGL::glVertex3f( 0 , 60 , 0 );
				rux::gui::engine::OpenGL::glVertex3f( 40 , 40 , 0 );
				rux::gui::engine::OpenGL::glEnd();*/
#else			
				if( ::rux::engine::_globals->_opengl_render )
				{
					rux::gui::WindowRenderEvent xevent( _window , this );
					_window->raise_event( xevent );
					DrawImmediately( ___rux__thread_index1986 );
				}
#endif		
				if( 0 && _gl->_is_support_framebuffer_object == 1 )
				{	
					rux::gui::engine::OpenGL::glDisableClientState( GL_COLOR_ARRAY , _gl , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					rux::gui::engine::OpenGL::glEnable( GL_TEXTURE_2D , _gl , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					rux::gui::engine::OpenGL::glBindFramebuffer( GL_FRAMEBUFFER , 0 , _gl , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					rux::gui::engine::OpenGL::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					rux::gui::engine::OpenGL::glLoadIdentity( __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					rux::gui::engine::OpenGL::glMatrixMode( GL_TEXTURE , _gl , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					rux::gui::engine::OpenGL::glPushMatrix( __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					rux::gui::engine::OpenGL::glScalef( 1.0f / _gl->_framebuffer_color_texture_width , 1.0f / _gl->_framebuffer_color_texture_height , 1.0f , _gl , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					rux::gui::engine::OpenGL::glBindTexture( GL_TEXTURE_2D , _gl->_framebuffer_color_texture_ptr[ 0 ] , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					rux::gui::engine::OpenGL::glGenerateMipmap( GL_TEXTURE_2D , _gl , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					rux::gui::engine::OpenGL::glBindTexture( GL_TEXTURE_2D , 0 , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					rux::gui::engine::OpenGL::glBindTexture( GL_TEXTURE_2D , _gl->_framebuffer_color_texture_ptr[ 0 ] , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					rux::gui::engine::OpenGL::glEnableClientState( GL_TEXTURE_COORD_ARRAY , _gl , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					float texture_coords[] =
					{
						0.f , (float)_new_height , 
						(float)_new_width , (float)_new_height ,
						(float)_new_width , 0.f,
						(float)_new_width , 0.f ,
						0.f , (float)_new_height ,
						0.f , 0.f
					};						
					rux::gui::engine::OpenGL::glTexCoordPointer( 2 , GL_FLOAT , 0 , texture_coords , _gl , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					rux::gui::engine::OpenGL::glEnableClientState( GL_VERTEX_ARRAY , _gl , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					float vertexes[] =
					{
						0.f , 0.f , 0.f ,
						(float)_new_width , 0.f , 0.f ,
						(float)_new_width , (float)_new_height , 0.f ,
						(float)_new_width , (float)_new_height , 0.f ,
						0.f , 0.f , 0.f ,
						0.f , (float)_new_height , 0.f
					};	
					rux::gui::engine::OpenGL::glVertexPointer( 3 , GL_FLOAT , 0 , vertexes , _gl , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					rux::gui::engine::OpenGL::glDrawArrays( GL_TRIANGLES , 0 , 6 , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					rux::gui::engine::OpenGL::glPopMatrix( __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					rux::gui::engine::OpenGL::glMatrixMode( GL_MODELVIEW , _gl , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					rux::gui::engine::OpenGL::glBindTexture( GL_TEXTURE_2D , 0 , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
					rux::gui::engine::OpenGL::glDisable( GL_TEXTURE_2D , _gl , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
				}
#ifdef __WINDOWS__	
				if( _gl->_is_supported_wgl_swap_buffer_interval == 1 )
				{	
					if( ::rux::gui::engine::_vsync )
					{
						if( _gl->_is_vsync == 1 )
							_gl->_is_vsync = ::rux::gui::engine::OpenGL::wglGetSwapInterval( _gl , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 ) ? 1 : 0;
						if( _gl->_is_vsync == 0 ) 
						{
							if( _gl->_is_supported_ext_swap_control_tear == 1 )
								rux::gui::engine::OpenGL::wglSwapInterval( -1 , _gl , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							else
								rux::gui::engine::OpenGL::wglSwapInterval( 1 , _gl , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							_gl->_is_vsync = 1;
						}
					}
					else
					{
						if( _gl->_is_vsync == 1 )
							_gl->_is_vsync = ::rux::gui::engine::OpenGL::wglGetSwapInterval( _gl , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 ) ? 1 : 0;
						if( _gl->_is_vsync == 1 ) 
						{
							rux::gui::engine::OpenGL::wglSwapInterval( 0 , _gl , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
							_gl->_is_vsync = 0;
						}
					}
				}
#endif
				if( _window->_is_transparent_window == 1 )
				{
#ifdef __WINDOWS__
					BitBlt( hdc , 0 , 0 , _new_width , _new_height , _window->_hdc , 0 , 0 , SRCCOPY );
					EndPaint( _window->_hwnd , &paint_struct );
#endif
				}
				for( size_t copy_items = 0 ; copy_items < _gl->_fonts.Count() ; copy_items++ )
					_gl->_fonts[ copy_items ]->RenderingStop();
			};
			bool RenderContext::check_cache( rux::gui::RenderCacheBase** cache , const char* __filename__ , rux::int32 __line__ )
			{
				RenderUniversalCache* ui_cache = (RenderUniversalCache*)(*cache);
				if( ui_cache == NULL )
				{		
					WRITE_LOCK( _gl->_cs_caches );
					if( _gl->_render_caches.ItemsCount() > 0 )
					{		
						for( size_t index0 = 0 ; index0 < _gl->_render_caches.Count() ; index0++ )
						{
							ui_cache = _gl->_render_caches[ index0 ];
							if( ui_cache )
							{
								_gl->_render_caches.set_EmptyAt( index0 );
								break;
							}
						}
						_gl->_cs_caches.WriteUnlock();
						ui_cache->Clean();
					}
					else
					{
						_gl->_cs_caches.WriteUnlock();
						ui_cache = ::rux::engine::alloc_object< ::rux::gui::engine::RenderUniversalCache >( (::rux::gui::engine::GLContext*)_gl , (const char*)__filename__ , (::rux::uint32)__line__ , (char*)0 , (::rux::malloc_t)0 );
					}
					*cache = ui_cache;
					return true;
				}
				return false;
			};
			void RenderContext::DrawRectangleSimpleBorder( ::rux::int32 left , ::rux::int32 top , ::rux::int32 width , ::rux::int32 height , ::rux::int32 border_width , rux::gui::ColorBase* border_color , float opacity , rux::gui::RenderCacheBase** cache , float& selected_z_index )
			{				
				if( border_width > 0.f && ::rux::gui::rux_color_is_visible( border_color ) == 1 )
				{
					RenderUniversalCache* univ_cache1 = (RenderUniversalCache*)(*cache);
					if( univ_cache1 )
					{						
						if( univ_cache1->_is_reset == 0 && 
							( _clip._left != univ_cache1->_old_clip._left 
							|| _clip._top != univ_cache1->_old_clip._top
							|| _clip._height != univ_cache1->_old_clip._height
							|| _clip._width != univ_cache1->_old_clip._width
							|| univ_cache1->_old_left != left
							|| univ_cache1->_old_top != top
							|| univ_cache1->_old_height != height
							|| univ_cache1->_old_width != width
							|| univ_cache1->_old_border_width != border_width )
							)
							univ_cache1->Reset();						
						if( univ_cache1->_is_reset == 0 && univ_cache1->_old_color && border_color )
						{
							if( univ_cache1->_old_color->Equals( border_color ) == 0 )
							{
								if( univ_cache1->_old_color->_color_type != border_color->_color_type )
									univ_cache1->Reset();
								else
								{
									if( univ_cache1->_old_color->_color_type == 1 )
									{
										::rux::gui::LinearGradientColor* linear_gradient_color0 = (::rux::gui::LinearGradientColor*)border_color;
										::rux::gui::LinearGradientColor* linear_gradient_color1 = (::rux::gui::LinearGradientColor*)univ_cache1->_old_color;
										CRITICAL_SECTION_LOCK( linear_gradient_color0->_cs_gradient_stops );
										CRITICAL_SECTION_LOCK( linear_gradient_color1->_cs_gradient_stops );
										if( linear_gradient_color0->_gradient_stops.Count() == linear_gradient_color1->_gradient_stops.Count() )
											univ_cache1->ResetColor();
										else
											univ_cache1->Reset();										
										linear_gradient_color1->_cs_gradient_stops.UnLock();
										linear_gradient_color0->_cs_gradient_stops.UnLock();
									}
									else
										univ_cache1->ResetColor();								
								}
							}
						}
						if( univ_cache1->_is_reset == 0 && univ_cache1->_old_opacity != opacity )
							univ_cache1->ResetColor();

						copy_color( univ_cache1->_old_color , border_color );						
						univ_cache1->_old_clip = _clip;
						univ_cache1->_old_border_width = border_width;
						univ_cache1->_old_opacity = opacity;
						univ_cache1->_old_left = left;
						univ_cache1->_old_top = top;
						univ_cache1->_old_height = height;
						univ_cache1->_old_width = width;
					}

					check_cache( cache , __FILE__ , __LINE__ );
					RenderUniversalCache* ui_cache = (RenderUniversalCache*)(*cache);
					if( ui_cache->_is_reset == 1 )
					{
						XMallocArray< ::rux::gui::Rectangle > cuts;
						if( left < _clip._left )
							cuts.Add( ::rux::gui::Rectangle( left , top , _clip._left - left , height ) );					
						if( top < _clip._top )
							cuts.Add( ::rux::gui::Rectangle( left , top , width , _clip._top - top ) );
						if( left + width > _clip._left + _clip._width )
							cuts.Add( ::rux::gui::Rectangle( _clip._left + _clip._width , top , left + width - _clip._left - _clip._width , height ) );
						if( top + height > _clip._top + _clip._height )
							cuts.Add( ::rux::gui::Rectangle( left , _clip._top + _clip._height , width , top + height - _clip._top - _clip._height ) );						
						if( cuts.Count() > 0 )
						{	
							if( ui_cache->get_FillStencilVertexesCount( this ) == 0 )
								rux_draw_rectangle_and_save_to_fill_stencil( this , _clip._left , _clip._top , _clip._width , _clip._height , selected_z_index , ui_cache );
							for( size_t index0 = 0 ; index0 < cuts.Count() ; index0++ )
								rux_draw_rectangle_and_save_to_cut_stencil( this, cuts[ index0 ]._left , cuts[ index0 ]._top , cuts[ index0 ]._width , cuts[ index0 ]._height , selected_z_index , ui_cache );
						}

						add_border_color_and_vertex_to_cache( left , top , width , height , border_width , selected_z_index , opacity , border_color , ui_cache );
					}
					else if( ui_cache->_is_reset_color == 1 )
						update_border_color_to_cache( width , height , border_width , opacity , border_color , ui_cache );
					ui_cache->set_ZIndex( this , selected_z_index );
				}
			};
			void RenderContext::DrawRectangleSimple( ::rux::int32 left , ::rux::int32 top , ::rux::int32 width , ::rux::int32 height , ::rux::int32 border_width , rux::gui::ColorBase* background_object_color , float opacity , rux::gui::RenderCacheBase** cache , float& selected_z_index )
			{				
				if( ::rux::gui::rux_color_is_visible( background_object_color ) == 1 )
				{
					RenderUniversalCache* univ_cache0 = (RenderUniversalCache*)(*cache);
					if( univ_cache0 )
					{
						if( univ_cache0->_is_reset == 0 && ( _clip._left != univ_cache0->_old_clip._left 
							|| _clip._top != univ_cache0->_old_clip._top
							|| _clip._height != univ_cache0->_old_clip._height
							|| _clip._width != univ_cache0->_old_clip._width
							|| univ_cache0->_old_left != left
							|| univ_cache0->_old_top != top
							|| univ_cache0->_old_height != height
							|| univ_cache0->_old_width != width
							|| univ_cache0->_old_border_width != border_width )
							)
							univ_cache0->Reset();
						if( univ_cache0->_is_reset == 0 && univ_cache0->_old_color && background_object_color )
						{
							if( univ_cache0->_old_color->Equals( background_object_color ) == 0 )
							{
								if( univ_cache0->_old_color->_color_type != background_object_color->_color_type )
									univ_cache0->Reset();
								else
								{
									if( univ_cache0->_old_color->_color_type == 1 )
									{
										::rux::gui::LinearGradientColor* linear_gradient_color0 = (::rux::gui::LinearGradientColor*)background_object_color;
										::rux::gui::LinearGradientColor* linear_gradient_color1 = (::rux::gui::LinearGradientColor*)univ_cache0->_old_color;
										CRITICAL_SECTION_LOCK( linear_gradient_color0->_cs_gradient_stops );
										CRITICAL_SECTION_LOCK( linear_gradient_color1->_cs_gradient_stops );
										if( linear_gradient_color0->_gradient_stops.Count() == linear_gradient_color1->_gradient_stops.Count() )
											univ_cache0->ResetColor();
										else
											univ_cache0->Reset();										
										linear_gradient_color1->_cs_gradient_stops.UnLock();
										linear_gradient_color0->_cs_gradient_stops.UnLock();
									}
									else
										univ_cache0->ResetColor();								
								}
							}
						}
						if( univ_cache0->_is_reset == 0 && univ_cache0->_old_opacity != opacity )
							univ_cache0->ResetColor();

						copy_color( univ_cache0->_old_color , background_object_color );						
						univ_cache0->_old_clip = _clip;
						univ_cache0->_old_border_width = border_width;
						univ_cache0->_old_opacity = opacity;
						univ_cache0->_old_left = left;
						univ_cache0->_old_top = top;
						univ_cache0->_old_height = height;
						univ_cache0->_old_width = width;
					}
					check_cache( cache , __FILE__ , __LINE__ );
					RenderUniversalCache* ui_cache = (RenderUniversalCache*)(*cache);
					if( ui_cache->_is_reset == 1 )
					{
						XMallocArray< ::rux::gui::Rectangle > cuts;
						if( left < _clip._left )
							cuts.Add( ::rux::gui::Rectangle( left , top , _clip._left - left , height ) );					
						if( top < _clip._top )
							cuts.Add( ::rux::gui::Rectangle( left , top , width , _clip._top - top ) );
						if( left + width > _clip._left + _clip._width )
							cuts.Add( ::rux::gui::Rectangle( _clip._left + _clip._width , top , left + width - _clip._left - _clip._width , height ) );
						if( top + height > _clip._top + _clip._height )
							cuts.Add( ::rux::gui::Rectangle( left , _clip._top + _clip._height , width , top + height - _clip._top - _clip._height ) );						
						if( cuts.Count() > 0 )
						{	
							if( ui_cache->get_FillStencilVertexesCount( this ) == 0 )
								rux_draw_rectangle_and_save_to_fill_stencil( this , _clip._left , _clip._top , _clip._width , _clip._height , selected_z_index , ui_cache );
							for( size_t index0 = 0 ; index0 < cuts.Count() ; index0++ )
								rux_draw_rectangle_and_save_to_cut_stencil( this , cuts[ index0 ]._left , cuts[ index0 ]._top , cuts[ index0 ]._width , cuts[ index0 ]._height , selected_z_index , ui_cache );
						}

						add_color_and_vertex_to_cache( left , top , width , height , border_width , selected_z_index , opacity , background_object_color , ui_cache );
					}
					else if( ui_cache->_is_reset_color == 1 )
						update_color_to_cache( width , height , border_width , opacity , background_object_color , ui_cache );
					ui_cache->set_ZIndex( this , selected_z_index );
				}
			};
			void RenderContext::DrawRectangleRoundBorder( ::rux::int32 left , ::rux::int32 top , ::rux::int32 width , ::rux::int32 height , ::rux::int32 border_width , ::rux::gui::ColorBase* border_color , const ::rux::gui::Thickness& corner , float opacity , ::rux::gui::RenderCacheBase** cache , float& selected_z_index )
			{
				if( border_width > 0.f && ::rux::gui::rux_color_is_visible( border_color ) == 1 )
				{
					RenderUniversalCache* univ_cache1 = (RenderUniversalCache*)(*cache);
					if( univ_cache1 )
					{
						if( univ_cache1->_is_reset == 0 && ( _clip._left != univ_cache1->_old_clip._left 
							|| _clip._top != univ_cache1->_old_clip._top
							|| _clip._height != univ_cache1->_old_clip._height
							|| _clip._width != univ_cache1->_old_clip._width
							|| univ_cache1->_old_left != left
							|| univ_cache1->_old_top != top
							|| univ_cache1->_old_height != height
							|| univ_cache1->_old_width != width
							|| univ_cache1->_old_border_width != border_width
							|| univ_cache1->_old_corner != corner )
							)
							univ_cache1->Reset();						
						if( univ_cache1->_is_reset == 0 && univ_cache1->_old_color && border_color )
						{
							if( univ_cache1->_old_color->Equals( border_color ) == 0 )
							{
								if( univ_cache1->_old_color->_color_type != border_color->_color_type )
									univ_cache1->Reset();
								else
								{
									if( univ_cache1->_old_color->_color_type == 1 )
									{
										::rux::gui::LinearGradientColor* linear_gradient_color0 = (::rux::gui::LinearGradientColor*)border_color;
										::rux::gui::LinearGradientColor* linear_gradient_color1 = (::rux::gui::LinearGradientColor*)univ_cache1->_old_color;
										CRITICAL_SECTION_LOCK( linear_gradient_color0->_cs_gradient_stops );
										CRITICAL_SECTION_LOCK( linear_gradient_color1->_cs_gradient_stops );
										if( linear_gradient_color0->_gradient_stops.Count() == linear_gradient_color1->_gradient_stops.Count() )
											univ_cache1->ResetColor();
										else
											univ_cache1->Reset();										
										linear_gradient_color1->_cs_gradient_stops.UnLock();
										linear_gradient_color0->_cs_gradient_stops.UnLock();
									}
									else
										univ_cache1->ResetColor();								
								}
							}
						}
						if( univ_cache1->_is_reset == 0 && univ_cache1->_old_opacity != opacity )
							univ_cache1->ResetColor();

						copy_color( univ_cache1->_old_color , border_color );						
						univ_cache1->_old_clip = _clip;
						univ_cache1->_old_corner = corner;
						univ_cache1->_old_border_width = border_width;
						univ_cache1->_old_opacity = opacity;
						univ_cache1->_old_left = left;
						univ_cache1->_old_top = top;
						univ_cache1->_old_height = height;
						univ_cache1->_old_width = width;
					}

					check_cache( cache , __FILE__ , __LINE__ );
					RenderUniversalCache* ui_cache = (RenderUniversalCache*)(*cache);
					if( ui_cache->_is_reset == 1 )
					{
						add_round_fill_stencil_vertex_to_cache( left , top , width , height , 0.f , corner , ui_cache , selected_z_index );
						add_round_cut_stencil_vertex_to_cache( left , top , width , height , border_width , corner , ui_cache , selected_z_index );

						XMallocArray< ::rux::gui::Rectangle > cuts;
						if( left < _clip._left )
							cuts.Add( ::rux::gui::Rectangle( left , top , _clip._left - left , height ) );					
						if( top < _clip._top )
							cuts.Add( ::rux::gui::Rectangle( left , top , width , _clip._top - top ) );
						if( left + width > _clip._left + _clip._width )
							cuts.Add( ::rux::gui::Rectangle( _clip._left + _clip._width , top , left + width - _clip._left - _clip._width , height ) );
						if( top + height > _clip._top + _clip._height )
							cuts.Add( ::rux::gui::Rectangle( left , _clip._top + _clip._height , width , top + height - _clip._top - _clip._height ) );						
						if( cuts.Count() > 0 )
						{							 				
							if( ui_cache->get_FillStencilVertexesCount( this ) == 0 )
								rux_draw_rectangle_and_save_to_fill_stencil( this , _clip._left , _clip._top , _clip._width , _clip._height , selected_z_index , ui_cache );
							for( size_t index0 = 0 ; index0 < cuts.Count() ; index0++ )
								rux_draw_rectangle_and_save_to_cut_stencil( this , cuts[ index0 ]._left , cuts[ index0 ]._top , cuts[ index0 ]._width , cuts[ index0 ]._height , selected_z_index , ui_cache );
						}

						add_color_and_vertex_to_cache( left , top , width , height , 0.f , selected_z_index , opacity , border_color , ui_cache );
					}
					else if( ui_cache->_is_reset_color == 1 )
						update_color_to_cache( width , height , 0.f , opacity , border_color , ui_cache );
					ui_cache->set_ZIndex( this , selected_z_index );
				}
			};
			void RenderContext::DrawRectangleRound( ::rux::int32 left , ::rux::int32 top , ::rux::int32 width , ::rux::int32 height , ::rux::int32 border_width , ::rux::gui::ColorBase* background_object_color , const ::rux::gui::Thickness& corner , float opacity , ::rux::gui::RenderCacheBase** cache , float& selected_z_index )
			{
				if( ::rux::gui::rux_color_is_visible( background_object_color ) == 1 )
				{
					RenderUniversalCache* univ_cache0 = (RenderUniversalCache*)(*cache);
					if( univ_cache0 )
					{
						if( univ_cache0->_is_reset == 0 && ( _clip._left != univ_cache0->_old_clip._left 
							|| _clip._top != univ_cache0->_old_clip._top
							|| _clip._height != univ_cache0->_old_clip._height
							|| _clip._width != univ_cache0->_old_clip._width
							|| univ_cache0->_old_left != left
							|| univ_cache0->_old_top != top
							|| univ_cache0->_old_height != height
							|| univ_cache0->_old_width != width
							|| univ_cache0->_old_border_width != border_width
							|| univ_cache0->_old_corner != corner )
							)
							univ_cache0->Reset();						
						if( univ_cache0->_is_reset == 0 && univ_cache0->_old_color && background_object_color )
						{
							if( univ_cache0->_old_color->Equals( background_object_color ) == 0 )
							{
								if( univ_cache0->_old_color->_color_type != background_object_color->_color_type )
									univ_cache0->Reset();
								else
								{
									if( univ_cache0->_old_color->_color_type == 1 )
									{
										::rux::gui::LinearGradientColor* linear_gradient_color0 = (::rux::gui::LinearGradientColor*)background_object_color;
										::rux::gui::LinearGradientColor* linear_gradient_color1 = (::rux::gui::LinearGradientColor*)univ_cache0->_old_color;
										CRITICAL_SECTION_LOCK( linear_gradient_color0->_cs_gradient_stops );
										CRITICAL_SECTION_LOCK( linear_gradient_color1->_cs_gradient_stops );
										if( linear_gradient_color0->_gradient_stops.Count() == linear_gradient_color1->_gradient_stops.Count() )
											univ_cache0->ResetColor();
										else
											univ_cache0->Reset();										
										linear_gradient_color1->_cs_gradient_stops.UnLock();
										linear_gradient_color0->_cs_gradient_stops.UnLock();
									}
									else
										univ_cache0->ResetColor();								
								}
							}
						}
						if( univ_cache0->_is_reset == 0 && univ_cache0->_old_opacity != opacity )
							univ_cache0->ResetColor();

						copy_color( univ_cache0->_old_color , background_object_color );						
						univ_cache0->_old_clip = _clip;
						univ_cache0->_old_corner = corner;
						univ_cache0->_old_border_width = border_width;
						univ_cache0->_old_opacity = opacity;
						univ_cache0->_old_left = left;
						univ_cache0->_old_top = top;
						univ_cache0->_old_height = height;
						univ_cache0->_old_width = width;
					}
					check_cache( cache , __FILE__ , __LINE__ );
					RenderUniversalCache* ui_cache = (RenderUniversalCache*)(*cache);
					if( ui_cache->_is_reset == 1 )
					{
						add_round_color_and_vertex_to_cache( left , top , width , height , border_width , background_object_color , corner , opacity , ui_cache , selected_z_index );
					}
					else if( ui_cache->_is_reset_color == 1 )
						update_round_color_to_cache( left , top , width , height , border_width , background_object_color , corner , opacity , ui_cache );
					ui_cache->set_ZIndex( this , selected_z_index );
				}
			};
			void RenderContext::DrawRectangleByShader( ::rux::int32 left , ::rux::int32 top , ::rux::int32 width , ::rux::int32 height , ::rux::int32 border_width , ::rux::gui::ColorBase* background_object_color , ::rux::gui::Color* border_color , const ::rux::gui::Thickness& corner , float opacity , ::rux::gui::RenderCacheBase** cache , float& selected_z_index , size_t ___rux__thread_index1986 )
			{
				if( ( border_width > 0.f && ::rux::gui::rux_color_is_visible( border_color ) == 1 ) || ::rux::gui::rux_color_is_visible( background_object_color ) == 1 )
				{
					RenderUniversalCache* univ_cache0 = (RenderUniversalCache*)(*cache);
					if( univ_cache0 )
					{						
						if( univ_cache0->_is_reset == 0 && 
							( _clip._left != univ_cache0->_old_clip._left 
							|| _clip._top != univ_cache0->_old_clip._top
							|| _clip._height != univ_cache0->_old_clip._height
							|| _clip._width != univ_cache0->_old_clip._width
							|| univ_cache0->_old_left != left
							|| univ_cache0->_old_top != top
							|| univ_cache0->_old_height != height
							|| univ_cache0->_old_width != width
							|| univ_cache0->_old_border_width != border_width
							|| univ_cache0->_old_gl_context_height != _gl->_height )
							)
							univ_cache0->Reset();						
						if( univ_cache0->_is_reset == 0 && univ_cache0->_old_border_color && border_color )
						{
							if( univ_cache0->_old_border_color->Equals( border_color ) == 0 )
							{
								if( univ_cache0->_old_border_color->_color_type != border_color->_color_type )
									univ_cache0->Reset();
								else
								{
									if( univ_cache0->_old_border_color->_color_type == 1 )
									{
										::rux::gui::LinearGradientColor* linear_gradient_color0 = (::rux::gui::LinearGradientColor*)border_color;
										::rux::gui::LinearGradientColor* linear_gradient_color1 = (::rux::gui::LinearGradientColor*)univ_cache0->_old_border_color;
										CRITICAL_SECTION_LOCK( linear_gradient_color0->_cs_gradient_stops );
										CRITICAL_SECTION_LOCK( linear_gradient_color1->_cs_gradient_stops );
										if( linear_gradient_color0->_gradient_stops.Count() == linear_gradient_color1->_gradient_stops.Count() )
											univ_cache0->ResetColor();
										else
											univ_cache0->Reset();										
										linear_gradient_color1->_cs_gradient_stops.UnLock();
										linear_gradient_color0->_cs_gradient_stops.UnLock();
									}
									else
										univ_cache0->ResetColor();								
								}
							}
						}
						if( univ_cache0->_is_reset == 0 && univ_cache0->_old_color && background_object_color )
						{
							if( univ_cache0->_old_color->Equals( background_object_color ) == 0 )
							{
								if( univ_cache0->_old_color->_color_type != background_object_color->_color_type )
									univ_cache0->Reset();
								else
								{
									if( univ_cache0->_old_color->_color_type == 1 )
									{
										::rux::gui::LinearGradientColor* linear_gradient_color0 = (::rux::gui::LinearGradientColor*)background_object_color;
										::rux::gui::LinearGradientColor* linear_gradient_color1 = (::rux::gui::LinearGradientColor*)univ_cache0->_old_color;
										CRITICAL_SECTION_LOCK( linear_gradient_color0->_cs_gradient_stops );
										CRITICAL_SECTION_LOCK( linear_gradient_color1->_cs_gradient_stops );
										if( linear_gradient_color0->_gradient_stops.Count() == linear_gradient_color1->_gradient_stops.Count() )
											univ_cache0->ResetColor();
										else
											univ_cache0->Reset();										
										linear_gradient_color1->_cs_gradient_stops.UnLock();
										linear_gradient_color0->_cs_gradient_stops.UnLock();
									}
									else
										univ_cache0->ResetColor();								
								}
							}
						}
						if( univ_cache0->_is_reset == 0 && univ_cache0->_old_opacity != opacity )
							univ_cache0->ResetColor();

						copy_color( univ_cache0->_old_color , background_object_color );
						copy_color( univ_cache0->_old_border_color , border_color );						
						univ_cache0->_old_clip = _clip;
						univ_cache0->_old_border_width = border_width;
						univ_cache0->_old_opacity = opacity;
						univ_cache0->_old_left = left;
						univ_cache0->_old_top = top;
						univ_cache0->_old_height = height;
						univ_cache0->_old_width = width;
						univ_cache0->_old_gl_context_height = _gl->_height;
					}
					check_cache( cache , __FILE__ , __LINE__ );
					RenderUniversalCache* ui_cache = (RenderUniversalCache*)(*cache);
					ui_cache->_is_simple_rectangle_shader = 1;
					if( ui_cache->_is_reset || ui_cache->_is_reset_color )
					{
						if( ui_cache->_gl_texture0 == 0 )
							ui_cache->_gl_texture0 = alloc_object_1_param_macros( ::rux::gui::engine::GLTexture , _gl );

						if( opacity < 1.f )
							ui_cache->_is_alpha = 1;
						::rux::byte gradient_type = 0;

						float common[ 52 ] = 
						{
							0.f , 0.f , 0.f , 0.f , 0.f , 0.f ,
							0.f , 0.f , 0.f , 0.f , 0.f , 0.f ,
							0.f , 0.f , 0.f , 0.f , 0.f , 0.f ,
							0.f , 0.f , 0.f , 0.f , 0.f , 0.f ,
							0.f , 0.f , 0.f , 0.f , 0.f , 0.f ,
							0.f , 0.f , 0.f , 0.f , 0.f , 0.f ,
							0.f , 0.f , 0.f , 0.f , 0.f , 0.f ,
							0.f , 0.f , 0.f , 0.f , 0.f , 0.f ,
							0.f , 0.f , 0.f , 0.f
						};
						size_t colors_count = 0;
						if( background_object_color )
						{
							if( background_object_color->get_IsAlpha() == 1 )
								ui_cache->_is_alpha = 1;
							if( background_object_color->_color_type == 0 )
							{
								::rux::gui::Color* color = (::rux::gui::Color*)background_object_color;
								common[ 0 ] = (float)color->_red / 255.f;
								common[ 1 ] = (float)color->_green / 255.f;
								common[ 2 ] = (float)color->_blue / 255.f;
								common[ 3 ] = (float)color->_alpha / 255.f;
								colors_count = 1;
							}
							else if( background_object_color->_color_type == 1 )
							{
								::rux::gui::LinearGradientColor* color = (::rux::gui::LinearGradientColor*)background_object_color;
								CRITICAL_SECTION_LOCK( color->_cs_gradient_stops );
								if( color->_linear_gradient_type == ::rux::gui::XEnum_LinearGradientType_Horizontal )
									gradient_type = 0;
								else
									gradient_type = 1;
								colors_count = color->_gradient_stops.Count();
								if( color->_gradient_stops.Count() > 0 )
								{
									::rux::gui::GradientStop* gradiant_stop = (::rux::gui::GradientStop*)color->_gradient_stops[ 0 ];
									common[ 32 ] = gradiant_stop->_offset;
									common[ 0 ] = (float)gradiant_stop->_color._red / 255.f;
									common[ 1 ] = (float)gradiant_stop->_color._green / 255.f;
									common[ 2 ] = (float)gradiant_stop->_color._blue / 255.f;
									common[ 3 ] = (float)gradiant_stop->_color._alpha / 255.f;
								}
								if( color->_gradient_stops.Count() > 1 )
								{
									::rux::gui::GradientStop* gradiant_stop = (::rux::gui::GradientStop*)color->_gradient_stops[ 1 ];
									common[ 33 ] = gradiant_stop->_offset;
									common[ 4 ] = (float)gradiant_stop->_color._red / 255.f;
									common[ 5 ] = (float)gradiant_stop->_color._green / 255.f;
									common[ 6 ] = (float)gradiant_stop->_color._blue / 255.f;
									common[ 7 ] = (float)gradiant_stop->_color._alpha / 255.f;
								}
								if( color->_gradient_stops.Count() > 2 )
								{
									::rux::gui::GradientStop* gradiant_stop = (::rux::gui::GradientStop*)color->_gradient_stops[ 2 ];
									common[ 34 ] = gradiant_stop->_offset;
									common[ 8 ] = (float)gradiant_stop->_color._red / 255.f;
									common[ 9 ] = (float)gradiant_stop->_color._green / 255.f;
									common[ 10 ] = (float)gradiant_stop->_color._blue / 255.f;
									common[ 11 ] = (float)gradiant_stop->_color._alpha / 255.f;
								}
								if( color->_gradient_stops.Count() > 3 )
								{
									::rux::gui::GradientStop* gradiant_stop = (::rux::gui::GradientStop*)color->_gradient_stops[ 3 ];
									common[ 35 ] = gradiant_stop->_offset;
									common[ 12 ] = (float)gradiant_stop->_color._red / 255.f;
									common[ 13 ] = (float)gradiant_stop->_color._green / 255.f;
									common[ 14 ] = (float)gradiant_stop->_color._blue / 255.f;
									common[ 15 ] = (float)gradiant_stop->_color._alpha / 255.f;
								}
								if( color->_gradient_stops.Count() > 4 )
								{
									::rux::gui::GradientStop* gradiant_stop = (::rux::gui::GradientStop*)color->_gradient_stops[ 4 ];
									common[ 36 ] = gradiant_stop->_offset;
									common[ 16 ] = (float)gradiant_stop->_color._red / 255.f;
									common[ 17 ] = (float)gradiant_stop->_color._green / 255.f;
									common[ 18 ] = (float)gradiant_stop->_color._blue / 255.f;
									common[ 19 ] = (float)gradiant_stop->_color._alpha / 255.f;
								}
								if( color->_gradient_stops.Count() > 5 )
								{
									::rux::gui::GradientStop* gradiant_stop = (::rux::gui::GradientStop*)color->_gradient_stops[ 5 ];
									common[ 37 ] = gradiant_stop->_offset;
									common[ 20 ] = (float)gradiant_stop->_color._red / 255.f;
									common[ 21 ] = (float)gradiant_stop->_color._green / 255.f;
									common[ 22 ] = (float)gradiant_stop->_color._blue / 255.f;
									common[ 23 ] = (float)gradiant_stop->_color._alpha / 255.f;
								}
								if( color->_gradient_stops.Count() > 6 )
								{
									::rux::gui::GradientStop* gradiant_stop = (::rux::gui::GradientStop*)color->_gradient_stops[ 6 ];
									common[ 38 ] = gradiant_stop->_offset;
									common[ 24 ] = (float)gradiant_stop->_color._red / 255.f;
									common[ 25 ] = (float)gradiant_stop->_color._green / 255.f;
									common[ 26 ] = (float)gradiant_stop->_color._blue / 255.f;
									common[ 27 ] = (float)gradiant_stop->_color._alpha / 255.f;
								}
								color->_cs_gradient_stops.UnLock();
							}
						}
						common[ 28 ] = colors_count;

						common[ 40 ] = width;
						common[ 41 ] = height;
						common[ 42 ] = left;
						common[ 43 ] = top;

						common[ 48 ] = border_width;
						common[ 49 ] = gradient_type;
						common[ 50 ] = ui_cache->_gl_context->_height;
						common[ 51 ] = opacity;
						if( border_color && border_color->_color_type == 0 )
						{
							if( border_color->get_IsAlpha() == 1 )
								ui_cache->_is_alpha = 1;
							::rux::gui::Color* color = (::rux::gui::Color*)border_color;
							common[ 44 ] = (float)color->_red / 255.f;
							common[ 45 ] = (float)color->_green / 255.f;
							common[ 46 ] = (float)color->_blue / 255.f;
							common[ 47 ] = (float)color->_alpha / 255.f;
						}
						::rux::byte reinited = 0;
						ui_cache->_gl_texture0->Copy00( GL_TEXTURE_1D , GL_RGBA32F_ARB , GL_RGBA , GL_FLOAT , 13 , 1 , common , GL_NEAREST , GL_NEAREST , reinited , ___rux__thread_index1986 );
					}
					if( ui_cache->_is_reset == 1 )
					{
						XMallocArray< ::rux::gui::Rectangle > cuts;
						if( left < _clip._left )
							cuts.Add( ::rux::gui::Rectangle( left , top , _clip._left - left , height ) );
						if( top < _clip._top )
							cuts.Add( ::rux::gui::Rectangle( left , top , width , _clip._top - top ) );
						if( left + width > _clip._left + _clip._width )
							cuts.Add( ::rux::gui::Rectangle( _clip._left + _clip._width , top , left + width - _clip._left - _clip._width , height ) );
						if( top + height > _clip._top + _clip._height )
							cuts.Add( ::rux::gui::Rectangle( left , _clip._top + _clip._height , width , top + height - _clip._top - _clip._height ) );						
						if( cuts.Count() > 0 )
						{	
							if( ui_cache->get_FillStencilVertexesCount( this ) == 0 )
								rux_draw_rectangle_and_save_to_fill_stencil( this , _clip._left , _clip._top , _clip._width , _clip._height , selected_z_index , ui_cache );
							for( size_t index0 = 0 ; index0 < cuts.Count() ; index0++ )
								rux_draw_rectangle_and_save_to_cut_stencil( this , cuts[ index0 ]._left , cuts[ index0 ]._top , cuts[ index0 ]._width , cuts[ index0 ]._height , selected_z_index , ui_cache );
						}
						float right = left + width;
						float bottom =  top + height;
						ui_cache->AddDrawingVertex( this , XUIVertexF( left , top , selected_z_index ) , NULL , NULL );
						ui_cache->AddDrawingVertex( this , XUIVertexF( right , top , selected_z_index ) , NULL , NULL );
						ui_cache->AddDrawingVertex( this , XUIVertexF( right , bottom , selected_z_index ) , NULL , NULL );
						ui_cache->AddDrawingVertex( this , XUIVertexF( right , bottom , selected_z_index ) , NULL , NULL );
						ui_cache->AddDrawingVertex( this , XUIVertexF( left , bottom , selected_z_index ) , NULL , NULL );
						ui_cache->AddDrawingVertex( this , XUIVertexF( left , top , selected_z_index ) , NULL , NULL );
					}
					ui_cache->set_ZIndex( this , selected_z_index );
				}
			};
			void RenderContext::DrawRectangle( ::rux::int32 left , ::rux::int32 top , ::rux::int32 width , ::rux::int32 height , ::rux::int32 border_width , ::rux::gui::ColorBase* background_object_color , ::rux::gui::Color* border , const ::rux::gui::Thickness& corner , float opacity , ::rux::gui::RenderCacheBase** border_cache , ::rux::gui::RenderCacheBase** cache , float& selected_z_index , ::rux::uint8 , const char* __filename__ , ::rux::int32 __line__ , size_t ___rux__thread_index1986 )
			{//return;
				_render_objects_count++;
				if( _clip._width > 0 
					&& _clip._height > 0
					&& left <= _clip._left + _clip._width
					&& top <= _clip._top + _clip._height
					&& left + width >= _clip._left
					&& top + height >= _clip._top )
				{
					if( background_object_color )
						background_object_color->AddRef();
					if( border )
						border->AddRef();				
					if( corner._left_down_corner == 0 && corner._left_up_corner == 0 && corner._right_down_corner == 0 && corner._right_up_corner == 0 )
					{						
						if( _gl->_is_supported_simple_rectangle_shader == 1 )
						{
							DrawRectangleByShader( left , top , width , height , border_width , background_object_color , border , corner , opacity , cache , selected_z_index , ___rux__thread_index1986 );
						}
						else
						{
							DrawRectangleSimpleBorder( left , top , width , height , border_width , border , opacity , border_cache , selected_z_index );
							DrawRectangleSimple( left , top , width , height , border_width , background_object_color , opacity , cache , selected_z_index );
						}
					}
					else
					{
						DrawRectangleRoundBorder( left , top , width , height , border_width , border , corner , opacity , border_cache , selected_z_index );
						DrawRectangleRound( left , top , width , height , border_width , background_object_color , corner , opacity , cache , selected_z_index );
					}
					if( background_object_color )
						background_object_color->Release();
					if( border )
						border->Release();
				}
			};
			void RenderContext::set_Clip( ::rux::gui::Rectangle clip )
			{
				_clip = clip;
			};
			rux::gui::Rectangle& RenderContext::get_Clip( void )
			{
				return _clip;
			};
			void RenderContext::DrawLine( ::rux::int32 start_left , ::rux::int32 start_top , ::rux::int32 end_left , ::rux::int32 end_top , ::rux::int32 thickness , float opacity , ::rux::gui::ColorBase* background_object_color , ::rux::gui::RenderCacheBase** cache , ::rux::gui::RenderCacheBase** cap1_cache , ::rux::gui::RenderCacheBase** cap2_cache , float& _selected_z_index , size_t ___rux__thread_index1986 )
			{
				_render_objects_count++;
				float calc_top = start_top > end_top ? end_top : start_top;
				calc_top -= thickness;
				float calc_left = start_left > end_left ? end_left : start_left;
				calc_left -= thickness;
				float calc_width = start_left > end_left ? start_left - end_left : end_left - start_left;
				calc_width += thickness;
				float calc_height = start_top > end_top ? start_top - end_top : end_top - start_top;
				calc_height += thickness;
				if( _clip._width > 0 
					&& _clip._height > 0
					&& calc_left <= _clip._left + _clip._width
					&& calc_top <= _clip._top + _clip._height
					&& calc_left + calc_width >= _clip._left
					&& calc_top + calc_height >= _clip._top )
				{
					if( background_object_color)
						background_object_color->AddRef();
					if( background_object_color->_color_type == 0 )
					{	
						rux::gui::Color* background = (rux::gui::Color*)background_object_color;
						if( background->_alpha > 0 && opacity > 0 )
						{
							RenderUniversalCache* ui_cache = (RenderUniversalCache*)(*cache) , * ui_cap1_cache = (RenderUniversalCache*)(*cap1_cache) , * ui_cap2_cache = (RenderUniversalCache*)(*cap2_cache);
							if( ui_cache )
							{
								if( ui_cache->_is_reset == 0 && ( _clip._left != ui_cache->_old_clip._left 
									|| _clip._top != ui_cache->_old_clip._top
									|| _clip._height != ui_cache->_old_clip._height
									|| _clip._width != ui_cache->_old_clip._width
									|| ui_cache->_old_left != start_left
									|| ui_cache->_old_top != start_top
									|| ui_cache->_old_height != end_top
									|| ui_cache->_old_width != end_left
									|| ui_cache->_old_border_width != thickness )
									)
								{
									ui_cache->Reset();
									if( ui_cap1_cache )
										ui_cap1_cache->Reset();
									if( ui_cap2_cache )
										ui_cap2_cache->Reset();
								}
								if( ui_cache->_is_reset == 0 && ui_cache->_old_color && background_object_color )
								{
									if( ui_cache->_old_color->Equals( background_object_color ) == 0 )
									{
										if( ui_cache->_old_color->_color_type != background_object_color->_color_type )
										{
											ui_cache->Reset();
											if( ui_cap1_cache )
												ui_cap1_cache->Reset();
											if( ui_cap2_cache )
												ui_cap2_cache->Reset();
										}
										else
										{
											if( ui_cache->_old_color->_color_type == 1 )
											{
												::rux::gui::LinearGradientColor* linear_gradient_color0 = (::rux::gui::LinearGradientColor*)background_object_color;
												::rux::gui::LinearGradientColor* linear_gradient_color1 = (::rux::gui::LinearGradientColor*)ui_cache->_old_color;
												CRITICAL_SECTION_LOCK( linear_gradient_color0->_cs_gradient_stops );
												CRITICAL_SECTION_LOCK( linear_gradient_color1->_cs_gradient_stops );
												if( linear_gradient_color0->_gradient_stops.Count() == linear_gradient_color1->_gradient_stops.Count() )
												{
													ui_cache->ResetColor();
													if( ui_cap1_cache )
														ui_cap1_cache->ResetColor();
													if( ui_cap2_cache )
														ui_cap2_cache->ResetColor();
												}
												else
												{
													ui_cache->Reset();
													if( ui_cap1_cache )
														ui_cap1_cache->Reset();
													if( ui_cap2_cache )
														ui_cap2_cache->Reset();
												}
												linear_gradient_color1->_cs_gradient_stops.UnLock();
												linear_gradient_color0->_cs_gradient_stops.UnLock();
											}
											else
											{
												ui_cache->ResetColor();
												if( ui_cap1_cache )
													ui_cap1_cache->ResetColor();
												if( ui_cap2_cache )
													ui_cap2_cache->ResetColor();
											}
										}
									}
								}
								if( ui_cache->_is_reset == 0 && ui_cache->_old_opacity != opacity )
								{
									ui_cache->ResetColor();
									if( ui_cap1_cache )
										ui_cap1_cache->ResetColor();
									if( ui_cap2_cache )
										ui_cap2_cache->ResetColor();
								}
								copy_color( ui_cache->_old_color , background_object_color );						
								ui_cache->_old_clip = _clip;
								ui_cache->_old_border_width = thickness;
								ui_cache->_old_opacity = opacity;
								ui_cache->_old_left = start_left;
								ui_cache->_old_top = start_top;
								ui_cache->_old_height = end_top;
								ui_cache->_old_width = end_left;
							}
							check_cache( cache , __FILE__ , __LINE__ );
							ui_cache = (RenderUniversalCache*)(*cache);
							ui_cache->set_DrawType( GL_TRIANGLE_STRIP );
							if( thickness >= 3 )
							{
								check_cache( cap1_cache , __FILE__ , __LINE__ );
								ui_cap1_cache = (RenderUniversalCache*)(*cap1_cache);
								ui_cap1_cache->set_DrawType( GL_TRIANGLE_STRIP );

								check_cache( cap2_cache , __FILE__ , __LINE__ );
								ui_cap2_cache = (RenderUniversalCache*)(*cap2_cache);
								ui_cap2_cache->set_DrawType( GL_TRIANGLE_STRIP );
							}
							if( ui_cache->_is_reset == 1 )
							{
								XMallocArray< ::rux::gui::Rectangle > cuts;
								if( calc_left < _clip._left )
									cuts.Add( ::rux::gui::Rectangle( calc_left , calc_top , _clip._left - calc_left , calc_height ) );					
								if( calc_top < _clip._top )
									cuts.Add( ::rux::gui::Rectangle( calc_left , calc_top , calc_width , _clip._top - calc_top ) );
								if( calc_left + calc_width > _clip._left + _clip._width )
									cuts.Add( ::rux::gui::Rectangle( _clip._left + _clip._width , calc_top , calc_left + calc_width - _clip._left - _clip._width , calc_height ) );
								if( calc_top + calc_height > _clip._top + _clip._height )
									cuts.Add( ::rux::gui::Rectangle( calc_left , _clip._top + _clip._height , calc_width , calc_top + calc_height - _clip._top - _clip._height ) );					
								if( cuts.Count() > 0 )
								{
									if( ui_cache->get_FillStencilVertexesCount( this ) == 0 )
										rux_draw_rectangle_and_save_to_fill_stencil( this , _clip._left , _clip._top , _clip._width , _clip._height , _selected_z_index , ui_cache ); 				
									for( ::rux::uint32 index0 = 0 ; index0 < cuts.Count() ; index0++ )
										rux_draw_rectangle_and_save_to_cut_stencil( this , cuts[ index0 ]._left , cuts[ index0 ]._top , cuts[ index0 ]._width , cuts[ index0 ]._height , _selected_z_index , ui_cache );
								}							
								line( start_left , start_top , end_left , end_top ,	//coordinates
									thickness ,			//thickness in px
									background->_red , background->_green , background->_blue , (GLubyte)( (float)background->_alpha * opacity ),	//line color RGBA
									ui_cache , ui_cap1_cache , ui_cap2_cache , _selected_z_index );			//enable alphablend
								ui_cache->set_Alpha( this , 1 );
								if( thickness >= 3 )
								{
									ui_cap1_cache->set_Alpha( this , 1 );
									ui_cap2_cache->set_Alpha( this , 1 );
								}
							}
							else if( ui_cache->_is_reset_color == 1 )
							{
								XUIColor colorf_empty( background->_red , background->_green , background->_blue , 0 );
								XUIColor colorf( background->_red , background->_green , background->_blue , (GLubyte)( (float)background->_alpha * opacity ) );
								ui_cache->UpdateDrawingColor( this , colorf_empty );
								ui_cache->UpdateDrawingColor( this , colorf_empty );
								ui_cache->UpdateDrawingColor( this , colorf );
								ui_cache->UpdateDrawingColor( this , colorf );
								ui_cache->UpdateDrawingColor( this , colorf );
								ui_cache->UpdateDrawingColor( this , colorf );
								ui_cache->UpdateDrawingColor( this , colorf_empty );
								ui_cache->UpdateDrawingColor( this , colorf_empty );
								//cap
								if ( thickness < 3) 
								{
								}
								else
								{
									ui_cap1_cache->UpdateDrawingColor( this , colorf_empty );
									ui_cap1_cache->UpdateDrawingColor( this , colorf_empty );
									ui_cap1_cache->UpdateDrawingColor( this , colorf );
									ui_cap1_cache->UpdateDrawingColor( this , colorf_empty );
									ui_cap1_cache->UpdateDrawingColor( this , colorf );
									ui_cap1_cache->UpdateDrawingColor( this , colorf_empty );

									ui_cap2_cache->UpdateDrawingColor( this , colorf_empty );
									ui_cap2_cache->UpdateDrawingColor( this , colorf_empty );
									ui_cap2_cache->UpdateDrawingColor( this , colorf );
									ui_cap2_cache->UpdateDrawingColor( this , colorf_empty );
									ui_cap2_cache->UpdateDrawingColor( this , colorf );
									ui_cap2_cache->UpdateDrawingColor( this , colorf_empty );
								}
							}
							ui_cache->set_ZIndex( this , _selected_z_index );
							if( thickness >= 3 )
							{
								ui_cap1_cache->set_ZIndex( this , _selected_z_index );
								ui_cap2_cache->set_ZIndex( this , _selected_z_index );
							}
						}
					}
					if( background_object_color)
						background_object_color->Release();
				}
			};
			void RenderContext::DrawPath( ::rux::int32 left , ::rux::int32 top , ::rux::int32 width , ::rux::int32 height , XMallocArray< float >& point_x , XMallocArray< float >& point_y , float opacity , ::rux::gui::ColorBase* background_object_color , ::rux::gui::RenderCacheBase** cache , float& _selected_z_index , ::rux::uint8 , size_t ___rux__thread_index1986 )
			{
				return;
				_render_objects_count++;
				if( background_object_color)
					background_object_color->AddRef();
				check_cache( cache , __FILE__ , __LINE__ );
				RenderUniversalCache* ui_cache = (RenderUniversalCache*)(*cache);
				if( _clip._width > 0 
					&& _clip._height > 0
					&& left <= _clip._left + _clip._width
					&& top <= _clip._top + _clip._height
					&& left + width >= _clip._left
					&& top + height >= _clip._top )
				{		
					if( ui_cache->_is_reset == 1 )
					{
						XMallocArray< ::rux::gui::Rectangle > cuts;
						float selected_z_index = _selected_z_index;							
						if( left < _clip._left )
							cuts.Add( ::rux::gui::Rectangle( left , top , _clip._left - left , height ) );					
						if( top < _clip._top )
							cuts.Add( ::rux::gui::Rectangle( left , top , width , _clip._top - top ) );
						if( left + width > _clip._left + _clip._width )
							cuts.Add( ::rux::gui::Rectangle( _clip._left + _clip._width , top , left + width - _clip._left - _clip._width , height ) );
						if( top + height > _clip._top + _clip._height )
							cuts.Add( ::rux::gui::Rectangle( left , _clip._top + _clip._height , width , top + height - _clip._top - _clip._height ) );
						rux::uint32 index0 = 0 , index1 = 0;
						for( index0 = 0 ; index0 < cuts.Count() ; index0++ )
							rux_draw_rectangle_and_save_to_cut_stencil( this , cuts[ index0 ]._left , cuts[ index0 ]._top , cuts[ index0 ]._width , cuts[ index0 ]._height , selected_z_index , ui_cache ); 
						XMallocArray< XMallocArray< float >* > polygon_x , polygon_y;
						polygon_x.Add( alloc_object_macros( XMallocArray< float > ) );
						polygon_y.Add( alloc_object_macros( XMallocArray< float > ) );			
						for( index0 = 0 ; index0 < point_x.Count() ; index0++ )
						{
							polygon_x[ 0 ]->Add( point_x[ index0 ] );
							polygon_y[ 0 ]->Add( point_y[ index0 ] );
						}	
						add_color_and_vertex_to_cache( left , top , width , height , 0.f , selected_z_index , opacity , background_object_color , ui_cache );
						float intersection_x = 0.f , intersection_y = 0.f , intersection_x2 = 0.f , intersection_y2 = 0.f;
						rux::uint32 second_point_index0 = 0 , second_point_index1 = 0 , index2 = 0 , polygon_index = 0 , index3 = 0 , second_point_index2 = 0;
						for( polygon_index = 0 ; polygon_index < polygon_x.Count() ; polygon_index++ )
						{
							if( polygon_index == 500 )
								break;
							for( index0 = 0 ; index0 < polygon_x[ polygon_index ]->Count() ; index0++ )
							{	
								second_point_index0 = index0 + 1;
								if( second_point_index0 >= polygon_x[ polygon_index ]->Count() )
									second_point_index0 = 0;
								for( index1 = 0 ; index1 < polygon_x[ polygon_index ]->Count() ; index1++ )
								{
									second_point_index1 = index1 + 1;
									if( second_point_index1 >= polygon_x[ polygon_index ]->Count() )
										second_point_index1 = 0;
									if( index0 != index1 && second_point_index0 != second_point_index1
										&& index0 != second_point_index1 && second_point_index0 != index1 )
									{
										if( ::rux::XMath::is_lines_intersect( polygon_x[ polygon_index ]->operator[]( index0 ) , polygon_y[ polygon_index ]->operator[]( index0 ) ,
											polygon_x[ polygon_index ]->operator[]( second_point_index0 ) , polygon_y[ polygon_index ]->operator[]( second_point_index0 ) ,
											polygon_x[ polygon_index ]->operator[]( index1 ) , polygon_y[ polygon_index ]->operator[]( index1 ) ,
											polygon_x[ polygon_index ]->operator[]( second_point_index1 ) , polygon_y[ polygon_index ]->operator[]( second_point_index1 ) ,
											intersection_x , intersection_y ) == 1 )
										{
											if( ::rux::XMath::is_point_belongs_to_segment( polygon_x[ polygon_index ]->operator[]( index1 ) , polygon_y[ polygon_index ]->operator[]( index1 ) ,
												polygon_x[ polygon_index ]->operator[]( second_point_index1 ) , polygon_y[ polygon_index ]->operator[]( second_point_index1 ) ,
												intersection_x , intersection_y ) == 1 )
											{
												for( index2 = 0 ; index2 < polygon_x[ polygon_index ]->Count() ; index2++ )
												{
													second_point_index2 = index2 + 1;
													if( second_point_index2 >= polygon_x[ polygon_index ]->Count() )
														second_point_index2 = 0;
													if( index0 != index2 && second_point_index0 != second_point_index2
														&& index0 != second_point_index2 && second_point_index0 != index2
														&& index1 != index2 && second_point_index1 != second_point_index2
														&& index1 != second_point_index2 && second_point_index1 != index2)
													{
														if( ::rux::XMath::is_segments_intersect( polygon_x[ polygon_index ]->operator[]( index1 ) , polygon_y[ polygon_index ]->operator[]( index1 ) ,
															polygon_x[ polygon_index ]->operator[]( second_point_index1 ) , polygon_y[ polygon_index ]->operator[]( second_point_index1 ) ,
															polygon_x[ polygon_index ]->operator[]( index2 ) , polygon_y[ polygon_index ]->operator[]( index2 ) ,
															polygon_x[ polygon_index ]->operator[]( second_point_index2 ) , polygon_y[ polygon_index ]->operator[]( second_point_index2 ) ,
															intersection_x2 , intersection_y2 ) == 1 )
															break;
													}
												}
												if( index2 >= polygon_x[ polygon_index ]->Count() )
												{
													if( ::rux::XMath::is_point_belongs_to_segment( 
														polygon_x[ polygon_index ]->operator[]( index0 ) ,
														polygon_y[ polygon_index ]->operator[]( index0 ) , 
														intersection_x , intersection_y , 
														polygon_x[ polygon_index ]->operator[]( second_point_index0 ) ,
														polygon_y[ polygon_index ]->operator[]( second_point_index0 ) ) == 1 )
													{
														rux::XMath::segment_midpoint( intersection_x , intersection_y , 
															polygon_x[ polygon_index ]->operator[]( second_point_index0 ) ,
															polygon_y[ polygon_index ]->operator[]( second_point_index0 ) , 
															intersection_x2 , intersection_y2 );											
													}
													else
														rux::XMath::segment_midpoint( intersection_x , intersection_y , 
															polygon_x[ polygon_index ]->operator[]( index0 ) ,
															polygon_y[ polygon_index ]->operator[]( index0 ) , 
															intersection_x2 , intersection_y2 );
													if( ::rux::XMath::is_point_inside_polygon( *( polygon_x[ polygon_index ] ) ,
														*( polygon_y[ polygon_index ] ) , intersection_x2 , intersection_y2 ) == 1 )
													{
														polygon_x.Add( alloc_object_macros( XMallocArray< float > ) );
														polygon_y.Add( alloc_object_macros( XMallocArray< float > ) );
														index3 = polygon_x.Count() - 1;
														if( index1 > index0 )
															index2 = index1 - index0;
														else
															index2 = index1 + 1 + polygon_x[ polygon_index ]->Count() - second_point_index0;
														while( index2 > 0 )
														{
															if( second_point_index0 >= polygon_x[ polygon_index ]->Count() )
																second_point_index0 = 0;
															polygon_x[ index3 ]->Add( polygon_x[ polygon_index ]->operator[]( second_point_index0 ) );
															polygon_y[ index3 ]->Add( polygon_y[ polygon_index ]->operator[]( second_point_index0 ) );
															polygon_x[ polygon_index ]->RemoveAt( second_point_index0 );
															polygon_y[ polygon_index ]->RemoveAt( second_point_index0 );
															index2--;
														}
														polygon_x[ index3 ]->Add( intersection_x );
														polygon_y[ index3 ]->Add( intersection_y );
														polygon_x[ polygon_index ]->Insert( second_point_index0 , intersection_x );
														polygon_y[ polygon_index ]->Insert( second_point_index0 , intersection_y );			
														index0 = 0;
														break;
													}
												}
											}
										}
									}
								}
							}
						}			
						for( polygon_index = 0 ; polygon_index < polygon_x.Count() ; polygon_index++ )
						{
							if( polygon_x[ polygon_index ]->Count() > 0 )
							{
								for( index0 = 1 ; index0 < polygon_x[ polygon_index ]->Count() - 1 ; index0++ )
								{
									ui_cache->AddFillStencilVertex( this , XUIVertexF( polygon_x[ polygon_index ]->operator[]( 0 ) , polygon_y[ polygon_index ]->operator[]( 0 ) , selected_z_index ) );
									ui_cache->AddFillStencilVertex( this , XUIVertexF( polygon_x[ polygon_index ]->operator[]( index0 ) , polygon_y[ polygon_index ]->operator[]( index0 ) , selected_z_index ) );
									ui_cache->AddFillStencilVertex( this , XUIVertexF( polygon_x[ polygon_index ]->operator[]( index0 + 1 ) , polygon_y[ polygon_index ]->operator[]( index0 + 1 ) , selected_z_index ) );				
								}
							}
							rux::engine::free_object< XMallocArray< float > >( polygon_x[ polygon_index ] );
							rux::engine::free_object< XMallocArray< float > >( polygon_y[ polygon_index ] );
						}
						ui_cache->set_ZIndex( this , _selected_z_index );

					}
					else
					{
						if( ui_cache->_is_reset_color == 1 )
							update_color_to_cache( width , height , 0.f , opacity , background_object_color , ui_cache );
						ui_cache->set_ZIndex( this , _selected_z_index );
					}
				}
				if( background_object_color)
					background_object_color->Release();
			};
			void RenderContext::add_round_fill_stencil_vertex_to_cache( float left , float top , float width , float height , float border_width , const ::rux::gui::Thickness& corner , RenderUniversalCache* ui_cache , float& selected_z_index )
			{					
				size_t sectors_count = 12;
				left += border_width;
				top += border_width;
				width -= 2 * border_width;
				height -= 2 * border_width;
				float left_up_radius = 0 , left_down_radius = 0 , right_up_radius = 0 , right_down_radius = 0 , x_center = 0 , y_center = 0;
				float radian = ( 3.14f / 2.0f ) / float( sectors_count );									
				if( corner._left_up_corner != 0.0f )
				{											
					if( width > height )
						left_up_radius = height / 2.0f;
					else
						left_up_radius = width / 2.0f;
					if( corner._left_up_corner < 1.0f )
						left_up_radius = left_up_radius * corner._left_up_corner;
					y_center = top + left_up_radius;
					x_center = left + left_up_radius;
					float x_last = left;
					float y_last = y_center;
					float x_next = 0 , y_next = 0;
					float corner_in_rad = radian;
					for( size_t index0 = 0 ; index0 < sectors_count ; index0++ )
					{											
						x_next = x_center - left_up_radius * cos( corner_in_rad );
						y_next = y_center - left_up_radius * sin( corner_in_rad );																				
						ui_cache->AddFillStencilVertex( this , XUIVertexF( x_last , y_last , selected_z_index ) );
						ui_cache->AddFillStencilVertex( this , XUIVertexF( x_next , y_next , selected_z_index ) );
						ui_cache->AddFillStencilVertex( this , XUIVertexF( x_center , y_center , selected_z_index ) );
						x_last = x_next;
						y_last = y_next;
						corner_in_rad += radian;
						if( index0 == sectors_count - 1 )
						{												
							ui_cache->AddFillStencilVertex( this , XUIVertexF( x_last , y_last , selected_z_index ) );
							ui_cache->AddFillStencilVertex( this , XUIVertexF( x_center , top , selected_z_index ) );
							ui_cache->AddFillStencilVertex( this , XUIVertexF( x_center , y_center , selected_z_index ) );
						}
					}
				}
				if( corner._left_down_corner != 0.0f )
				{										
					if( width > height )
						left_down_radius = height / 2.0f;
					else
						left_down_radius = width / 2.0f;
					if( corner._left_down_corner < 1.0f )
						left_down_radius *= corner._left_down_corner;
					x_center = left + left_down_radius;
					y_center = top + height - left_down_radius;		
					float x_last = left;
					float y_last = y_center;
					float x_next = 0 , y_next = 0;
					float corner_in_rad = radian;				
					for( size_t index0 = 0 ; index0 < sectors_count ; index0++ )
					{											
						x_next = x_center - left_down_radius * cos( corner_in_rad );
						y_next = y_center + left_down_radius * sin( corner_in_rad );
						ui_cache->AddFillStencilVertex( this , XUIVertexF( x_last , y_last , selected_z_index ) );
						ui_cache->AddFillStencilVertex( this , XUIVertexF( x_next , y_next , selected_z_index ) );
						ui_cache->AddFillStencilVertex( this , XUIVertexF( x_center , y_center , selected_z_index ) );
						x_last = x_next;
						y_last = y_next;
						corner_in_rad += radian;
						if( index0 == sectors_count - 1 )
						{												
							x_next = top + height;
							ui_cache->AddFillStencilVertex( this , XUIVertexF( x_last , y_last , selected_z_index ) );
							ui_cache->AddFillStencilVertex( this , XUIVertexF( x_center , x_next , selected_z_index ) );
							ui_cache->AddFillStencilVertex( this , XUIVertexF( x_center , y_center , selected_z_index ) );
						}
					}
				}
				if( corner._right_up_corner != 0.0f )
				{										
					if( width > height )
						right_up_radius = height / 2.0f;
					else
						right_up_radius = width / 2.0f;
					if( corner._right_up_corner < 1.0f )
						right_up_radius *= corner._right_up_corner;
					x_center = left + width - right_up_radius;
					y_center = top + right_up_radius;		
					float x_last = left + width;
					float y_last = y_center;
					float x_next = 0 , y_next = 0;
					float corner_in_rad = radian;				
					for( size_t index0 = 0 ; index0 < sectors_count ; index0++ )
					{											
						x_next = x_center + right_up_radius * cos( corner_in_rad );
						y_next = y_center - right_up_radius * sin( corner_in_rad );
						ui_cache->AddFillStencilVertex( this , XUIVertexF( x_last , y_last , selected_z_index ) );
						ui_cache->AddFillStencilVertex( this , XUIVertexF( x_next , y_next , selected_z_index ) );
						ui_cache->AddFillStencilVertex( this , XUIVertexF( x_center , y_center , selected_z_index ) );
						x_last = x_next;
						y_last = y_next;
						corner_in_rad += radian;
						if( index0 == sectors_count - 1 )
						{												
							ui_cache->AddFillStencilVertex( this , XUIVertexF( x_last , y_last , selected_z_index ) );
							ui_cache->AddFillStencilVertex( this , XUIVertexF( x_center , top , selected_z_index ) );
							ui_cache->AddFillStencilVertex( this , XUIVertexF( x_center , y_center , selected_z_index ) );
						}
					}
				}
				if( corner._right_down_corner != 0.0f )
				{										
					if( width > height )
						right_down_radius = height / 2.0f;
					else
						right_down_radius = width / 2.0f;
					if( corner._right_down_corner < 1.0f )
						right_down_radius *= corner._right_down_corner;
					x_center = left + width - right_down_radius;
					y_center = top + height - right_down_radius;		
					float x_last = left + width;
					float y_last = y_center;
					float x_next = 0 , y_next = 0;
					float corner_in_rad = radian;				
					for( size_t index0 = 0 ; index0 < sectors_count ; index0++ )
					{											
						x_next = x_center + right_down_radius * cos( corner_in_rad );
						y_next = y_center + right_down_radius * sin( corner_in_rad );
						ui_cache->AddFillStencilVertex( this , XUIVertexF( x_last , y_last , selected_z_index ) );
						ui_cache->AddFillStencilVertex( this , XUIVertexF( x_next , y_next , selected_z_index ) );
						ui_cache->AddFillStencilVertex( this , XUIVertexF( x_center , y_center , selected_z_index ) );
						x_last = x_next;
						y_last = y_next;
						corner_in_rad += radian;
						if( index0 == sectors_count - 1 )
						{												
							x_next = top + height;
							ui_cache->AddFillStencilVertex( this , XUIVertexF( x_last , y_last , selected_z_index ) );
							ui_cache->AddFillStencilVertex( this , XUIVertexF( x_center , x_next , selected_z_index ) );
							ui_cache->AddFillStencilVertex( this , XUIVertexF( x_center , y_center , selected_z_index ) );
						}
					}
				}
				y_center = left_up_radius > left_down_radius ? left_up_radius : left_down_radius;
				y_center = y_center > right_down_radius ? y_center : right_down_radius;
				y_center = y_center > right_up_radius ? y_center : right_up_radius;
				x_center = y_center - left_up_radius;
				if( x_center > 0 )
				{
					rux_draw_rectangle_and_save_to_fill_stencil( this , left + left_up_radius , top , x_center , left_up_radius + x_center , selected_z_index , ui_cache );
					rux_draw_rectangle_and_save_to_fill_stencil( this , left , top + left_up_radius , left_up_radius , x_center , selected_z_index , ui_cache );					
				}
				x_center = y_center - left_down_radius;
				if( x_center > 0 )
				{										
					rux_draw_rectangle_and_save_to_fill_stencil( this , left + left_down_radius , top + height - left_down_radius - x_center  , x_center , left_down_radius + x_center , selected_z_index , ui_cache );
					rux_draw_rectangle_and_save_to_fill_stencil( this , left , top + height - left_down_radius - x_center , left_down_radius , x_center , selected_z_index , ui_cache );					
				}
				x_center = y_center - right_down_radius;
				if( x_center > 0 )
				{										
					rux_draw_rectangle_and_save_to_fill_stencil( this , left + width - right_down_radius - x_center , top + height - right_down_radius - x_center  , x_center , right_down_radius + x_center , selected_z_index , ui_cache );
					rux_draw_rectangle_and_save_to_fill_stencil( this , left + width - right_down_radius , top + height - right_down_radius - x_center , right_down_radius , x_center , selected_z_index , ui_cache );					
				}
				x_center = y_center - right_up_radius;
				if( x_center > 0 )
				{										
					rux_draw_rectangle_and_save_to_fill_stencil( this , left + width - right_up_radius - x_center , top , x_center , right_up_radius + x_center , selected_z_index , ui_cache );
					rux_draw_rectangle_and_save_to_fill_stencil( this , left + width - right_up_radius , top + right_up_radius , right_up_radius , x_center , selected_z_index , ui_cache );					
				}
				x_center = height - 2 * y_center;
				if( x_center > 0 )
					rux_draw_rectangle_and_save_to_fill_stencil( this , left , top + y_center , width , x_center , selected_z_index , ui_cache );

				x_center = width - 2 * y_center;
				if( x_center > 0 )
				{										
					rux_draw_rectangle_and_save_to_fill_stencil( this , left + y_center , top , x_center , y_center , selected_z_index , ui_cache );
					rux_draw_rectangle_and_save_to_fill_stencil( this , left + y_center , top + height - y_center , x_center , y_center , selected_z_index , ui_cache );
				}
			};
			void RenderContext::add_round_cut_stencil_vertex_to_cache( float left , float top , float width , float height , float border_width , const ::rux::gui::Thickness& corner , RenderUniversalCache* ui_cache , float& selected_z_index )
			{					
				size_t sectors_count = 12;
				left += border_width;
				top += border_width;
				width -= 2 * border_width;
				height -= 2 * border_width;
				float left_up_radius = 0 , left_down_radius = 0 , right_up_radius = 0 , right_down_radius = 0 , x_center = 0 , y_center = 0;
				float radian = ( 3.14f / 2.0f ) / float( sectors_count );									
				if( corner._left_up_corner != 0.0f )
				{											
					if( width > height )
						left_up_radius = height / 2.0f;
					else
						left_up_radius = width / 2.0f;
					if( corner._left_up_corner < 1.0f )
						left_up_radius = left_up_radius * corner._left_up_corner;
					y_center = top + left_up_radius;
					x_center = left + left_up_radius;
					float x_last = left;
					float y_last = y_center;
					float x_next = 0 , y_next = 0;
					float corner_in_rad = radian;
					for( size_t index0 = 0 ; index0 < sectors_count ; index0++ )
					{											
						x_next = x_center - left_up_radius * cos( corner_in_rad );
						y_next = y_center - left_up_radius * sin( corner_in_rad );																				
						ui_cache->AddCutStencilVertex( this , XUIVertexF( x_last , y_last , selected_z_index ) );
						ui_cache->AddCutStencilVertex( this , XUIVertexF( x_next , y_next , selected_z_index ) );
						ui_cache->AddCutStencilVertex( this , XUIVertexF( x_center , y_center , selected_z_index ) );
						x_last = x_next;
						y_last = y_next;
						corner_in_rad += radian;
						if( index0 == sectors_count - 1 )
						{												
							ui_cache->AddCutStencilVertex( this , XUIVertexF( x_last , y_last , selected_z_index ) );
							ui_cache->AddCutStencilVertex( this , XUIVertexF( x_center , top , selected_z_index ) );
							ui_cache->AddCutStencilVertex( this , XUIVertexF( x_center , y_center , selected_z_index ) );
						}
					}
				}
				if( corner._left_down_corner != 0.0f )
				{										
					if( width > height )
						left_down_radius = height / 2.0f;
					else
						left_down_radius = width / 2.0f;
					if( corner._left_down_corner < 1.0f )
						left_down_radius *= corner._left_down_corner;
					x_center = left + left_down_radius;
					y_center = top + height - left_down_radius;		
					float x_last = left;
					float y_last = y_center;
					float x_next = 0 , y_next = 0;
					float corner_in_rad = radian;				
					for( size_t index0 = 0 ; index0 < sectors_count ; index0++ )
					{											
						x_next = x_center - left_down_radius * cos( corner_in_rad );
						y_next = y_center + left_down_radius * sin( corner_in_rad );
						ui_cache->AddCutStencilVertex( this , XUIVertexF( x_last , y_last , selected_z_index ) );
						ui_cache->AddCutStencilVertex( this , XUIVertexF( x_next , y_next , selected_z_index ) );
						ui_cache->AddCutStencilVertex( this , XUIVertexF( x_center , y_center , selected_z_index ) );
						x_last = x_next;
						y_last = y_next;
						corner_in_rad += radian;
						if( index0 == sectors_count - 1 )
						{												
							x_next = top + height;
							ui_cache->AddCutStencilVertex( this , XUIVertexF( x_last , y_last , selected_z_index ) );
							ui_cache->AddCutStencilVertex( this , XUIVertexF( x_center , x_next , selected_z_index ) );
							ui_cache->AddCutStencilVertex( this , XUIVertexF( x_center , y_center , selected_z_index ) );
						}
					}
				}
				if( corner._right_up_corner != 0.0f )
				{										
					if( width > height )
						right_up_radius = height / 2.0f;
					else
						right_up_radius = width / 2.0f;
					if( corner._right_up_corner < 1.0f )
						right_up_radius *= corner._right_up_corner;
					x_center = left + width - right_up_radius;
					y_center = top + right_up_radius;		
					float x_last = left + width;
					float y_last = y_center;
					float x_next = 0 , y_next = 0;
					float corner_in_rad = radian;				
					for( size_t index0 = 0 ; index0 < sectors_count ; index0++ )
					{											
						x_next = x_center + right_up_radius * cos( corner_in_rad );
						y_next = y_center - right_up_radius * sin( corner_in_rad );
						ui_cache->AddCutStencilVertex( this , XUIVertexF( x_last , y_last , selected_z_index ) );
						ui_cache->AddCutStencilVertex( this , XUIVertexF( x_next , y_next , selected_z_index ) );
						ui_cache->AddCutStencilVertex( this , XUIVertexF( x_center , y_center , selected_z_index ) );
						x_last = x_next;
						y_last = y_next;
						corner_in_rad += radian;
						if( index0 == sectors_count - 1 )
						{												
							ui_cache->AddCutStencilVertex( this , XUIVertexF( x_last , y_last , selected_z_index ) );
							ui_cache->AddCutStencilVertex( this , XUIVertexF( x_center , top , selected_z_index ) );
							ui_cache->AddCutStencilVertex( this , XUIVertexF( x_center , y_center , selected_z_index ) );
						}
					}
				}
				if( corner._right_down_corner != 0.0f )
				{										
					if( width > height )
						right_down_radius = height / 2.0f;
					else
						right_down_radius = width / 2.0f;
					if( corner._right_down_corner < 1.0f )
						right_down_radius *= corner._right_down_corner;
					x_center = left + width - right_down_radius;
					y_center = top + height - right_down_radius;		
					float x_last = left + width;
					float y_last = y_center;
					float x_next = 0 , y_next = 0;
					float corner_in_rad = radian;				
					for( size_t index0 = 0 ; index0 < sectors_count ; index0++ )
					{											
						x_next = x_center + right_down_radius * cos( corner_in_rad );
						y_next = y_center + right_down_radius * sin( corner_in_rad );
						ui_cache->AddCutStencilVertex( this , XUIVertexF( x_last , y_last , selected_z_index ) );
						ui_cache->AddCutStencilVertex( this , XUIVertexF( x_next , y_next , selected_z_index ) );
						ui_cache->AddCutStencilVertex( this , XUIVertexF( x_center , y_center , selected_z_index ) );
						x_last = x_next;
						y_last = y_next;
						corner_in_rad += radian;
						if( index0 == sectors_count - 1 )
						{												
							x_next = top + height;
							ui_cache->AddCutStencilVertex( this , XUIVertexF( x_last , y_last , selected_z_index ) );
							ui_cache->AddCutStencilVertex( this , XUIVertexF( x_center , x_next , selected_z_index ) );
							ui_cache->AddCutStencilVertex( this , XUIVertexF( x_center , y_center , selected_z_index ) );
						}
					}
				}
				y_center = left_up_radius > left_down_radius ? left_up_radius : left_down_radius;
				y_center = y_center > right_down_radius ? y_center : right_down_radius;
				y_center = y_center > right_up_radius ? y_center : right_up_radius;
				x_center = y_center - left_up_radius;
				if( x_center > 0 )
				{
					rux_draw_rectangle_and_save_to_cut_stencil( this , left + left_up_radius , top , x_center , left_up_radius + x_center , selected_z_index , ui_cache );
					rux_draw_rectangle_and_save_to_cut_stencil( this , left , top + left_up_radius , left_up_radius , x_center , selected_z_index , ui_cache );					
				}
				x_center = y_center - left_down_radius;
				if( x_center > 0 )
				{										
					rux_draw_rectangle_and_save_to_cut_stencil( this , left + left_down_radius , top + height - left_down_radius - x_center  , x_center , left_down_radius + x_center , selected_z_index , ui_cache );
					rux_draw_rectangle_and_save_to_cut_stencil( this , left , top + height - left_down_radius - x_center , left_down_radius , x_center , selected_z_index , ui_cache );					
				}
				x_center = y_center - right_down_radius;
				if( x_center > 0 )
				{										
					rux_draw_rectangle_and_save_to_cut_stencil( this , left + width - right_down_radius - x_center , top + height - right_down_radius - x_center  , x_center , right_down_radius + x_center , selected_z_index , ui_cache );
					rux_draw_rectangle_and_save_to_cut_stencil( this , left + width - right_down_radius , top + height - right_down_radius - x_center , right_down_radius , x_center , selected_z_index , ui_cache );					
				}
				x_center = y_center - right_up_radius;
				if( x_center > 0 )
				{										
					rux_draw_rectangle_and_save_to_cut_stencil( this , left + width - right_up_radius - x_center , top , x_center , right_up_radius + x_center , selected_z_index , ui_cache );
					rux_draw_rectangle_and_save_to_cut_stencil( this , left + width - right_up_radius , top + right_up_radius , right_up_radius , x_center , selected_z_index , ui_cache );					
				}
				x_center = height - 2 * y_center;
				if( x_center > 0 )
					rux_draw_rectangle_and_save_to_cut_stencil( this , left , top + y_center , width , x_center , selected_z_index , ui_cache );

				x_center = width - 2 * y_center;
				if( x_center > 0 )
				{										
					rux_draw_rectangle_and_save_to_cut_stencil( this , left + y_center , top , x_center , y_center , selected_z_index , ui_cache );
					rux_draw_rectangle_and_save_to_cut_stencil( this , left + y_center , top + height - y_center , x_center , y_center , selected_z_index , ui_cache );
				}
			};
			void RenderContext::update_round_color_to_cache( float left , float top , float width , float height , float border_width , ::rux::gui::ColorBase* color , const ::rux::gui::Thickness& corner , float opacity , RenderUniversalCache* ui_cache )
			{	
				if( color->_color_type == 0 )
				{							
					rux::gui::Color* background = (rux::gui::Color*)color;
					if( background->_alpha > 0 && opacity > 0 )
					{								
						if( background->_alpha < 255 || opacity < 1.0f )
							ui_cache->set_Alpha( this , 1 );
						else
							ui_cache->set_Alpha( this , 0 );
						XUIColor colorf( background->_red , background->_green , background->_blue , (GLubyte)( (float)background->_alpha * opacity ) );
						size_t sectors_count = 12;
						left += border_width;
						top += border_width;
						width -= 2 * border_width;
						height -= 2 * border_width;
						float left_up_radius = 0 , left_down_radius = 0 , right_up_radius = 0 , right_down_radius = 0;
						if( corner._left_up_corner != 0.0f )
						{											
							if( width > height )
								left_up_radius = height / 2.0f;
							else
								left_up_radius = width / 2.0f;
							if( corner._left_up_corner < 1.0f )
								left_up_radius = left_up_radius * corner._left_up_corner;
							for( size_t index0 = 0 ; index0 < sectors_count ; index0++ )
							{																		
								ui_cache->UpdateDrawingColor( this , colorf );
								ui_cache->UpdateDrawingColor( this , colorf );
								ui_cache->UpdateDrawingColor( this , colorf );
								if( index0 == sectors_count - 1 )
								{												
									ui_cache->UpdateDrawingColor( this , colorf );
									ui_cache->UpdateDrawingColor( this , colorf );
									ui_cache->UpdateDrawingColor( this , colorf );
								}
							}
						}
						if( corner._left_down_corner != 0.0f )
						{										
							if( width > height )
								left_down_radius = height / 2.0f;
							else
								left_down_radius = width / 2.0f;
							if( corner._left_down_corner < 1.0f )
								left_down_radius *= corner._left_down_corner;
							for( size_t index0 = 0 ; index0 < sectors_count ; index0++ )
							{
								ui_cache->UpdateDrawingColor( this , colorf );
								ui_cache->UpdateDrawingColor( this , colorf );
								ui_cache->UpdateDrawingColor( this , colorf );
								if( index0 == sectors_count - 1 )
								{
									ui_cache->UpdateDrawingColor( this , colorf );
									ui_cache->UpdateDrawingColor( this , colorf );
									ui_cache->UpdateDrawingColor( this , colorf );
								}
							}
						}
						if( corner._right_up_corner != 0.0f )
						{										
							if( width > height )
								right_up_radius = height / 2.0f;
							else
								right_up_radius = width / 2.0f;
							if( corner._right_up_corner < 1.0f )
								right_up_radius *= corner._right_up_corner;
							for( size_t index0 = 0 ; index0 < sectors_count ; index0++ )
							{
								ui_cache->UpdateDrawingColor( this , colorf );
								ui_cache->UpdateDrawingColor( this , colorf );
								ui_cache->UpdateDrawingColor( this , colorf );
								if( index0 == sectors_count - 1 )
								{												
									ui_cache->UpdateDrawingColor( this , colorf );
									ui_cache->UpdateDrawingColor( this , colorf );
									ui_cache->UpdateDrawingColor( this , colorf );
								}
							}
						}
						if( corner._right_down_corner != 0.0f )
						{										
							if( width > height )
								right_down_radius = height / 2.0f;
							else
								right_down_radius = width / 2.0f;
							if( corner._right_down_corner < 1.0f )
								right_down_radius *= corner._right_down_corner;
							for( size_t index0 = 0 ; index0 < sectors_count ; index0++ )
							{
								ui_cache->UpdateDrawingColor( this , colorf );
								ui_cache->UpdateDrawingColor( this , colorf );
								ui_cache->UpdateDrawingColor( this , colorf );
								if( index0 == sectors_count - 1 )
								{
									ui_cache->UpdateDrawingColor( this , colorf );
									ui_cache->UpdateDrawingColor( this , colorf );
									ui_cache->UpdateDrawingColor( this , colorf );
								}
							}
						}
						float y_center = left_up_radius > left_down_radius ? left_up_radius : left_down_radius;
						y_center = y_center > right_down_radius ? y_center : right_down_radius;
						y_center = y_center > right_up_radius ? y_center : right_up_radius;
						float x_center = y_center - left_up_radius;
						if( x_center > 0 )
						{
							ui_cache->UpdateDrawingColor( this , colorf );
							ui_cache->UpdateDrawingColor( this , colorf );
							ui_cache->UpdateDrawingColor( this , colorf );
							ui_cache->UpdateDrawingColor( this , colorf );
							ui_cache->UpdateDrawingColor( this , colorf );
							ui_cache->UpdateDrawingColor( this , colorf );
							ui_cache->UpdateDrawingColor( this , colorf );
							ui_cache->UpdateDrawingColor( this , colorf );
							ui_cache->UpdateDrawingColor( this , colorf );
							ui_cache->UpdateDrawingColor( this , colorf );
							ui_cache->UpdateDrawingColor( this , colorf );
							ui_cache->UpdateDrawingColor( this , colorf );
						}
						x_center = y_center - left_down_radius;
						if( x_center > 0 )
						{										
							ui_cache->UpdateDrawingColor( this , colorf );
							ui_cache->UpdateDrawingColor( this , colorf );
							ui_cache->UpdateDrawingColor( this , colorf );
							ui_cache->UpdateDrawingColor( this , colorf );
							ui_cache->UpdateDrawingColor( this , colorf );
							ui_cache->UpdateDrawingColor( this , colorf );
							ui_cache->UpdateDrawingColor( this , colorf );
							ui_cache->UpdateDrawingColor( this , colorf );
							ui_cache->UpdateDrawingColor( this , colorf );
							ui_cache->UpdateDrawingColor( this , colorf );
							ui_cache->UpdateDrawingColor( this , colorf );
							ui_cache->UpdateDrawingColor( this , colorf );
						}
						x_center = y_center - right_down_radius;
						if( x_center > 0 )
						{										
							ui_cache->UpdateDrawingColor( this , colorf );
							ui_cache->UpdateDrawingColor( this , colorf );
							ui_cache->UpdateDrawingColor( this , colorf );
							ui_cache->UpdateDrawingColor( this , colorf );
							ui_cache->UpdateDrawingColor( this , colorf );
							ui_cache->UpdateDrawingColor( this , colorf );
							ui_cache->UpdateDrawingColor( this , colorf );
							ui_cache->UpdateDrawingColor( this , colorf );
							ui_cache->UpdateDrawingColor( this , colorf );
							ui_cache->UpdateDrawingColor( this , colorf );
							ui_cache->UpdateDrawingColor( this , colorf );
							ui_cache->UpdateDrawingColor( this , colorf );
						}
						x_center = y_center - right_up_radius;
						if( x_center > 0 )
						{										
							ui_cache->UpdateDrawingColor( this , colorf );
							ui_cache->UpdateDrawingColor( this , colorf );
							ui_cache->UpdateDrawingColor( this , colorf );
							ui_cache->UpdateDrawingColor( this , colorf );
							ui_cache->UpdateDrawingColor( this , colorf );
							ui_cache->UpdateDrawingColor( this , colorf );
							ui_cache->UpdateDrawingColor( this , colorf );
							ui_cache->UpdateDrawingColor( this , colorf );
							ui_cache->UpdateDrawingColor( this , colorf );
							ui_cache->UpdateDrawingColor( this , colorf );
							ui_cache->UpdateDrawingColor( this , colorf );
							ui_cache->UpdateDrawingColor( this , colorf );
						}
						x_center = height - 2 * y_center;
						if( x_center > 0 )
						{
							ui_cache->UpdateDrawingColor( this , colorf );
							ui_cache->UpdateDrawingColor( this , colorf );
							ui_cache->UpdateDrawingColor( this , colorf );
							ui_cache->UpdateDrawingColor( this , colorf );
							ui_cache->UpdateDrawingColor( this , colorf );
							ui_cache->UpdateDrawingColor( this , colorf );
						}
						x_center = width - 2 * y_center;
						if( x_center > 0 )
						{										
							ui_cache->UpdateDrawingColor( this , colorf );
							ui_cache->UpdateDrawingColor( this , colorf );
							ui_cache->UpdateDrawingColor( this , colorf );
							ui_cache->UpdateDrawingColor( this , colorf );
							ui_cache->UpdateDrawingColor( this , colorf );
							ui_cache->UpdateDrawingColor( this , colorf );
							ui_cache->UpdateDrawingColor( this , colorf );
							ui_cache->UpdateDrawingColor( this , colorf );
							ui_cache->UpdateDrawingColor( this , colorf );
							ui_cache->UpdateDrawingColor( this , colorf );
							ui_cache->UpdateDrawingColor( this , colorf );
							ui_cache->UpdateDrawingColor( this , colorf );
						}
					}
				}
				else if( color->_color_type == 1 )
					update_color_to_cache( width , height , 0.f , opacity , color , ui_cache );
			};
			void RenderContext::add_round_color_and_vertex_to_cache( float left , float top , float width , float height , float border_width , ::rux::gui::ColorBase* color , const ::rux::gui::Thickness& corner , float opacity , RenderUniversalCache* ui_cache , float& selected_z_index )
			{	
				if( color->_color_type == 0 )
				{							
					rux::gui::Color* background = (rux::gui::Color*)color;
					if( background->_alpha > 0 && opacity > 0 )
					{	
						XMallocArray< ::rux::gui::Rectangle > cuts;
						if( left < _clip._left )
							cuts.Add( ::rux::gui::Rectangle( left , top , _clip._left - left , height ) );					
						if( top < _clip._top )
							cuts.Add( ::rux::gui::Rectangle( left , top , width , _clip._top - top ) );
						if( left + width > _clip._left + _clip._width )
							cuts.Add( ::rux::gui::Rectangle( _clip._left + _clip._width , top , left + width - _clip._left - _clip._width , height ) );
						if( top + height > _clip._top + _clip._height )
							cuts.Add( ::rux::gui::Rectangle( left , _clip._top + _clip._height , width , top + height - _clip._top - _clip._height ) );						
						if( cuts.Count() > 0 )
						{	
							if( ui_cache->get_FillStencilVertexesCount( this ) == 0 )
								rux_draw_rectangle_and_save_to_fill_stencil( this , _clip._left , _clip._top , _clip._width , _clip._height , selected_z_index , ui_cache );
							for( size_t index0 = 0 ; index0 < cuts.Count() ; index0++ )
								rux_draw_rectangle_and_save_to_cut_stencil( this , cuts[ index0 ]._left , cuts[ index0 ]._top , cuts[ index0 ]._width , cuts[ index0 ]._height , selected_z_index , ui_cache );
						}

						if( background->_alpha < 255 || opacity < 1.0f )
							ui_cache->set_Alpha( this , 1 );
						else
							ui_cache->set_Alpha( this , 0 );
						XUIColor colorf( background->_red , background->_green , background->_blue , (GLubyte)( (float)background->_alpha * opacity ) );
						size_t sectors_count = 12;
						left += border_width;
						top += border_width;
						width -= 2 * border_width;
						height -= 2 * border_width;
						float left_up_radius = 0 , left_down_radius = 0 , right_up_radius = 0 , right_down_radius = 0 , x_center = 0 , y_center = 0;
						float radian = ( 3.14f / 2.0f ) / float( sectors_count );									
						if( corner._left_up_corner != 0.0f )
						{											
							if( width > height )
								left_up_radius = height / 2.0f;
							else
								left_up_radius = width / 2.0f;
							if( corner._left_up_corner < 1.0f )
								left_up_radius = left_up_radius * corner._left_up_corner;
							y_center = top + left_up_radius;
							x_center = left + left_up_radius;
							float x_last = left;
							float y_last = y_center;
							float x_next = 0 , y_next = 0;
							float corner_in_rad = radian;
							for( size_t index0 = 0 ; index0 < sectors_count ; index0++ )
							{											
								x_next = x_center - left_up_radius * cos( corner_in_rad );
								y_next = y_center - left_up_radius * sin( corner_in_rad );																				
								ui_cache->AddDrawingVertex( this , XUIVertexF( x_last , y_last , selected_z_index ) , &colorf , NULL );
								ui_cache->AddDrawingVertex( this , XUIVertexF( x_next , y_next , selected_z_index ) , &colorf , NULL );
								ui_cache->AddDrawingVertex( this , XUIVertexF( x_center , y_center , selected_z_index ) , &colorf , NULL );
								x_last = x_next;
								y_last = y_next;
								corner_in_rad += radian;
								if( index0 == sectors_count - 1 )
								{												
									ui_cache->AddDrawingVertex( this , XUIVertexF( x_last , y_last , selected_z_index ) , &colorf , NULL );
									ui_cache->AddDrawingVertex( this , XUIVertexF( x_center , top , selected_z_index ) , &colorf , NULL );
									ui_cache->AddDrawingVertex( this , XUIVertexF( x_center , y_center , selected_z_index ) , &colorf , NULL );
								}
							}
						}
						if( corner._left_down_corner != 0.0f )
						{										
							if( width > height )
								left_down_radius = height / 2.0f;
							else
								left_down_radius = width / 2.0f;
							if( corner._left_down_corner < 1.0f )
								left_down_radius *= corner._left_down_corner;
							x_center = left + left_down_radius;
							y_center = top + height - left_down_radius;		
							float x_last = left;
							float y_last = y_center;
							float x_next = 0 , y_next = 0;
							float corner_in_rad = radian;				
							for( size_t index0 = 0 ; index0 < sectors_count ; index0++ )
							{											
								x_next = x_center - left_down_radius * cos( corner_in_rad );
								y_next = y_center + left_down_radius * sin( corner_in_rad );
								ui_cache->AddDrawingVertex( this , XUIVertexF( x_last , y_last , selected_z_index ) , &colorf , NULL );
								ui_cache->AddDrawingVertex( this , XUIVertexF( x_next , y_next , selected_z_index ) , &colorf , NULL );
								ui_cache->AddDrawingVertex( this , XUIVertexF( x_center , y_center , selected_z_index ) , &colorf , NULL );
								x_last = x_next;
								y_last = y_next;
								corner_in_rad += radian;
								if( index0 == sectors_count - 1 )
								{												
									x_next = top + height;
									ui_cache->AddDrawingVertex( this , XUIVertexF( x_last , y_last , selected_z_index ) , &colorf , NULL );
									ui_cache->AddDrawingVertex( this , XUIVertexF( x_center , x_next , selected_z_index ) , &colorf , NULL );
									ui_cache->AddDrawingVertex( this , XUIVertexF( x_center , y_center , selected_z_index ) , &colorf , NULL );
								}
							}
						}
						if( corner._right_up_corner != 0.0f )
						{										
							if( width > height )
								right_up_radius = height / 2.0f;
							else
								right_up_radius = width / 2.0f;
							if( corner._right_up_corner < 1.0f )
								right_up_radius *= corner._right_up_corner;
							x_center = left + width - right_up_radius;
							y_center = top + right_up_radius;		
							float x_last = left + width;
							float y_last = y_center;
							float x_next = 0 , y_next = 0;
							float corner_in_rad = radian;				
							for( size_t index0 = 0 ; index0 < sectors_count ; index0++ )
							{											
								x_next = x_center + right_up_radius * cos( corner_in_rad );
								y_next = y_center - right_up_radius * sin( corner_in_rad );
								ui_cache->AddDrawingVertex( this , XUIVertexF( x_last , y_last , selected_z_index ) , &colorf , NULL );
								ui_cache->AddDrawingVertex( this , XUIVertexF( x_next , y_next , selected_z_index ) , &colorf , NULL );
								ui_cache->AddDrawingVertex( this , XUIVertexF( x_center , y_center , selected_z_index ) , &colorf , NULL );
								x_last = x_next;
								y_last = y_next;
								corner_in_rad += radian;
								if( index0 == sectors_count - 1 )
								{												
									ui_cache->AddDrawingVertex( this , XUIVertexF( x_last , y_last , selected_z_index ) , &colorf , NULL );
									ui_cache->AddDrawingVertex( this , XUIVertexF( x_center , top , selected_z_index ) , &colorf , NULL );
									ui_cache->AddDrawingVertex( this , XUIVertexF( x_center , y_center , selected_z_index ) , &colorf , NULL );
								}
							}
						}
						if( corner._right_down_corner != 0.0f )
						{										
							if( width > height )
								right_down_radius = height / 2.0f;
							else
								right_down_radius = width / 2.0f;
							if( corner._right_down_corner < 1.0f )
								right_down_radius *= corner._right_down_corner;
							x_center = left + width - right_down_radius;
							y_center = top + height - right_down_radius;		
							float x_last = left + width;
							float y_last = y_center;
							float x_next = 0 , y_next = 0;
							float corner_in_rad = radian;				
							for( size_t index0 = 0 ; index0 < sectors_count ; index0++ )
							{											
								x_next = x_center + right_down_radius * cos( corner_in_rad );
								y_next = y_center + right_down_radius * sin( corner_in_rad );
								ui_cache->AddDrawingVertex( this , XUIVertexF( x_last , y_last , selected_z_index ) , &colorf , NULL );
								ui_cache->AddDrawingVertex( this , XUIVertexF( x_next , y_next , selected_z_index ) , &colorf , NULL );
								ui_cache->AddDrawingVertex( this , XUIVertexF( x_center , y_center , selected_z_index ) , &colorf , NULL );
								x_last = x_next;
								y_last = y_next;
								corner_in_rad += radian;
								if( index0 == sectors_count - 1 )
								{												
									x_next = top + height;
									ui_cache->AddDrawingVertex( this , XUIVertexF( x_last , y_last , selected_z_index ) , &colorf , NULL );
									ui_cache->AddDrawingVertex( this , XUIVertexF( x_center , x_next , selected_z_index ) , &colorf , NULL );
									ui_cache->AddDrawingVertex( this , XUIVertexF( x_center , y_center , selected_z_index ) , &colorf , NULL );
								}
							}
						}
						y_center = left_up_radius > left_down_radius ? left_up_radius : left_down_radius;
						y_center = y_center > right_down_radius ? y_center : right_down_radius;
						y_center = y_center > right_up_radius ? y_center : right_up_radius;
						x_center = y_center - left_up_radius;
						if( x_center > 0 )
						{
							rux_draw_rectangle_and_save_to_drawing_vertex( this , left + left_up_radius , top , x_center , left_up_radius + x_center , selected_z_index , ui_cache , &colorf );

							rux_draw_rectangle_and_save_to_drawing_vertex( this , left , top + left_up_radius , left_up_radius , x_center , selected_z_index , ui_cache , &colorf );
						}
						x_center = y_center - left_down_radius;
						if( x_center > 0 )
						{										
							rux_draw_rectangle_and_save_to_drawing_vertex( this , left + left_down_radius , top + height - left_down_radius - x_center  , x_center , left_down_radius + x_center , selected_z_index , ui_cache , &colorf );

							rux_draw_rectangle_and_save_to_drawing_vertex( this , left , top + height - left_down_radius - x_center , left_down_radius , x_center , selected_z_index , ui_cache , &colorf );
						}
						x_center = y_center - right_down_radius;
						if( x_center > 0 )
						{										
							rux_draw_rectangle_and_save_to_drawing_vertex( this , left + width - right_down_radius - x_center , top + height - right_down_radius - x_center  , x_center , right_down_radius + x_center , selected_z_index , ui_cache , &colorf );

							rux_draw_rectangle_and_save_to_drawing_vertex( this , left + width - right_down_radius , top + height - right_down_radius - x_center , right_down_radius , x_center , selected_z_index , ui_cache , &colorf );
						}
						x_center = y_center - right_up_radius;
						if( x_center > 0 )
						{										
							rux_draw_rectangle_and_save_to_drawing_vertex( this , left + width - right_up_radius - x_center , top , x_center , right_up_radius + x_center , selected_z_index , ui_cache , &colorf );
							
							rux_draw_rectangle_and_save_to_drawing_vertex( this , left + width - right_up_radius , top + right_up_radius , right_up_radius , x_center , selected_z_index , ui_cache , &colorf );
						}
						x_center = height - 2 * y_center;
						if( x_center > 0 )
						{
							rux_draw_rectangle_and_save_to_drawing_vertex( this , left , top + y_center , width , x_center , selected_z_index , ui_cache , &colorf );
						}
						x_center = width - 2 * y_center;
						if( x_center > 0 )
						{										
							rux_draw_rectangle_and_save_to_drawing_vertex( this , left + y_center , top , x_center , y_center , selected_z_index , ui_cache , &colorf );

							rux_draw_rectangle_and_save_to_drawing_vertex( this , left + y_center , top + height - y_center , x_center , y_center , selected_z_index , ui_cache , &colorf );
						}
					}
				}
				else if( color->_color_type == 1 )
				{	
					add_round_fill_stencil_vertex_to_cache( left , top , width , height , border_width , corner , ui_cache , selected_z_index );

					XMallocArray< ::rux::gui::Rectangle > cuts;
					if( left < _clip._left )
						cuts.Add( ::rux::gui::Rectangle( left , top , _clip._left - left , height ) );					
					if( top < _clip._top )
						cuts.Add( ::rux::gui::Rectangle( left , top , width , _clip._top - top ) );
					if( left + width > _clip._left + _clip._width )
						cuts.Add( ::rux::gui::Rectangle( _clip._left + _clip._width , top , left + width - _clip._left - _clip._width , height ) );
					if( top + height > _clip._top + _clip._height )
						cuts.Add( ::rux::gui::Rectangle( left , _clip._top + _clip._height , width , top + height - _clip._top - _clip._height ) );					
					if( cuts.Count() > 0 )
					{							 				
						if( ui_cache->get_FillStencilVertexesCount( this ) == 0 )
							rux_draw_rectangle_and_save_to_fill_stencil( this , _clip._left , _clip._top , _clip._width , _clip._height , selected_z_index , ui_cache );
						for( size_t index0 = 0 ; index0 < cuts.Count() ; index0++ )
							rux_draw_rectangle_and_save_to_cut_stencil( this , cuts[ index0 ]._left , cuts[ index0 ]._top , cuts[ index0 ]._width , cuts[ index0 ]._height , selected_z_index , ui_cache );
					}

					add_color_and_vertex_to_cache( left , top , width , height , 0.f , selected_z_index , opacity , color , ui_cache );
				}
			};
			void RenderContext::add_border_color_and_vertex_to_cache( float x , float y , float width , float height , float border_width , float selected_z_index , float opacity , ::rux::gui::ColorBase* color , ::rux::gui::engine::RenderUniversalCache* cache )
			{
				if( color->_color_type == 1 )
				{												
				}						
				else if( color->_color_type == 0 )
				{							
					rux::gui::Color* background = (rux::gui::Color*)color;
					if( background->_alpha > 0 && opacity > 0 )
					{								
						if( background->_alpha < 255 || opacity < 1.0f )
							cache->set_Alpha( this , 1 );
						else
							cache->set_Alpha( this , 0 );								
						add_color_and_vertex_to_cache( x , y , width , border_width , 0.f , selected_z_index , opacity , background , cache );

						add_color_and_vertex_to_cache( x + width - border_width , y + border_width , border_width , height - border_width , 0.f , selected_z_index , opacity , background , cache );

						add_color_and_vertex_to_cache( x , y + height - border_width , width - border_width , border_width , 0.f , selected_z_index , opacity , background , cache );

						add_color_and_vertex_to_cache( x , y + border_width , border_width , height - 2 * border_width , 0.f , selected_z_index , opacity , background , cache );
					}							
				}							
			};
			void RenderContext::update_border_color_to_cache( float width , float height , float border_width , float opacity , ::rux::gui::ColorBase* color , ::rux::gui::engine::RenderUniversalCache* cache )
			{
				if( color->_color_type == 1 )
				{
				}						
				else if( color->_color_type == 0 )
				{							
					rux::gui::Color* background = (rux::gui::Color*)color;
					if( background->_alpha > 0 && opacity > 0 )
					{								
						if( background->_alpha < 255 || opacity < 1.0f )
							cache->set_Alpha( this , 1 );
						else
							cache->set_Alpha( this , 0 );								
						update_color_to_cache( width , border_width , 0.f , opacity , background , cache );

						update_color_to_cache( border_width , height - border_width , 0.f , opacity , background , cache );

						update_color_to_cache( width - border_width , border_width , 0.f , opacity , background , cache );

						update_color_to_cache( border_width , height - 2 * border_width , 0.f , opacity , background , cache );
					}			
				}							
			};
			void RenderContext::add_color_and_vertex_to_cache( float x , float y , float width , float height , float border_width , float selected_z_index , float opacity , ::rux::gui::ColorBase* color , ::rux::gui::engine::RenderUniversalCache* cache )
			{
				if( color->_color_type == 1 )
				{							
					rux::gui::LinearGradientColor* background = (rux::gui::LinearGradientColor*)color;
					rux::uint8 visible = background->get_IsVisible() , alpha = 0;
					rux::uint32 index0 = 0;
					CRITICAL_SECTION_LOCK( background->_cs_gradient_stops );
					alpha = opacity < 1.0f ? 1 : background->get_IsAlpha();
					if( visible == 1 )
					{								
						cache->set_Alpha( this , alpha );								
						float x_next = x + border_width;
						float y_next = x + width - border_width;
						float y_center = y + border_width;
						float x_center = width - 2 * border_width;
						float x_last = height - 2 * border_width;								
						if( background->_gradient_stops.Count() == 1 )
						{									
							XUIColor color( background->_gradient_stops[ 0 ]->_color._red , background->_gradient_stops[ 0 ]->_color._green , background->_gradient_stops[ 0 ]->_color._blue , (GLubyte)( (float)background->_gradient_stops[ 0 ]->_color._alpha * opacity ) );
							cache->AddDrawingVertex( this , XUIVertexF( x_next , y_center , selected_z_index ) , &color , NULL );
							cache->AddDrawingVertex( this , XUIVertexF( y_next , y_center , selected_z_index ) , &color , NULL );
							cache->AddDrawingVertex( this , XUIVertexF( y_next , y_center + x_last , selected_z_index ) , &color , NULL );
							cache->AddDrawingVertex( this , XUIVertexF( y_next , y_center + x_last , selected_z_index ) , &color , NULL );
							cache->AddDrawingVertex( this , XUIVertexF( x_next , y_center + x_last , selected_z_index ) , &color , NULL );
							cache->AddDrawingVertex( this , XUIVertexF( x_next , y_center , selected_z_index ) , &color , NULL );
						}
						else if( background->_linear_gradient_type == XEnum_LinearGradientType_Horizontal )
						{									
							float x_offset = 0 , width_offset = 0;
							index0 = 1;
							if( background->_gradient_stops[ 0 ]->_offset > 0 )
							{										
								x_offset = x_center * background->_gradient_stops[ 0 ]->_offset;
								if( x_offset > x_center )
									x_offset = x_center;						
								XUIColor color( background->_gradient_stops[ 0 ]->_color._red , background->_gradient_stops[ 0 ]->_color._green , background->_gradient_stops[ 0 ]->_color._blue , (GLubyte)( (float)background->_gradient_stops[ 0 ]->_color._alpha * opacity ) );
								cache->AddDrawingVertex( this , XUIVertexF( x_next + x_offset , y_center + x_last , selected_z_index ) , &color , NULL );
								cache->AddDrawingVertex( this , XUIVertexF( x_next , y_center + x_last , selected_z_index ) , &color , NULL );
								cache->AddDrawingVertex( this , XUIVertexF( x_next , y_center , selected_z_index ) , &color , NULL );
								cache->AddDrawingVertex( this , XUIVertexF( x_next , y_center , selected_z_index ) , &color , NULL );
								cache->AddDrawingVertex( this , XUIVertexF( x_next + x_offset , y_center , selected_z_index ) , &color , NULL );
								cache->AddDrawingVertex( this , XUIVertexF( x_next + x_offset , y_center + x_last , selected_z_index ) , &color , NULL );
							}
							for( ; index0 < background->_gradient_stops.Count() ; index0++ )
							{										
								if( x_offset >= x_center )
									break;
								width_offset = x_center * background->_gradient_stops[ index0 ]->_offset;
								if( x_offset + width_offset > x_center )
									width_offset = x_center - x_offset;
								if( width_offset > 0 )
								{											
									XUIColor color( background->_gradient_stops[ index0 - 1 ]->_color._red , background->_gradient_stops[ index0 - 1 ]->_color._green , background->_gradient_stops[ index0 - 1 ]->_color._blue , (GLubyte)( (float)background->_gradient_stops[ index0 - 1 ]->_color._alpha * opacity ) );							
									cache->AddDrawingVertex( this , XUIVertexF( x_next + x_offset , y_center , selected_z_index ) , &color , NULL );
									color = XUIColor( background->_gradient_stops[ index0 ]->_color._red , background->_gradient_stops[ index0 ]->_color._green , background->_gradient_stops[ index0 ]->_color._blue , (GLubyte)( (float)background->_gradient_stops[ index0 ]->_color._alpha * opacity ) );							
									cache->AddDrawingVertex( this , XUIVertexF( x_next + x_offset + width_offset , y_center , selected_z_index ) , &color , NULL );
									cache->AddDrawingVertex( this , XUIVertexF( x_next + x_offset + width_offset , y_center + x_last , selected_z_index ) , &color , NULL );
									cache->AddDrawingVertex( this , XUIVertexF( x_next + x_offset + width_offset , y_center + x_last , selected_z_index ) , &color , NULL );
									color = XUIColor( background->_gradient_stops[ index0 - 1 ]->_color._red , background->_gradient_stops[ index0 - 1 ]->_color._green , background->_gradient_stops[ index0 - 1 ]->_color._blue , (GLubyte)( (float)background->_gradient_stops[ index0 - 1 ]->_color._alpha * opacity ) );							
									cache->AddDrawingVertex( this , XUIVertexF( x_next + x_offset , y_center + x_last , selected_z_index ) , &color , NULL );
									cache->AddDrawingVertex( this , XUIVertexF( x_next + x_offset , y_center , selected_z_index ) , &color , NULL );
									x_offset += width_offset;
								}
							}
						}			
						else if( background->_linear_gradient_type == XEnum_LinearGradientType_Vertical )
						{									
							float y_offset = 0 , height_offset = 0;
							index0 = 1;
							if( background->_gradient_stops[ 0 ]->_offset > 0 )
							{										
								y_offset = x_last * background->_gradient_stops[ 0 ]->_offset;
								if( y_offset > x_last )
									y_offset = x_last;
								XUIColor color( background->_gradient_stops[ 0 ]->_color._red , background->_gradient_stops[ 0 ]->_color._green , background->_gradient_stops[ 0 ]->_color._blue , (GLubyte)( (float)background->_gradient_stops[ 0 ]->_color._alpha * opacity ) );																
								cache->AddDrawingVertex( this , XUIVertexF( y_next , y_center + y_offset , selected_z_index ) , &color , NULL );
								cache->AddDrawingVertex( this , XUIVertexF( x_next , y_center + y_offset , selected_z_index ) , &color , NULL );
								cache->AddDrawingVertex( this , XUIVertexF( x_next , y_center , selected_z_index ) , &color , NULL );
								cache->AddDrawingVertex( this , XUIVertexF( x_next , y_center , selected_z_index ) , &color , NULL );
								cache->AddDrawingVertex( this , XUIVertexF( y_next , y_center , selected_z_index ) , &color , NULL );
								cache->AddDrawingVertex( this , XUIVertexF( y_next , y_center + y_offset , selected_z_index ) , &color , NULL );
							}
							for( ; index0 < background->_gradient_stops.Count() ; index0++ )
							{										
								if( y_offset >= x_last )
									break;
								height_offset = x_last * background->_gradient_stops[ index0 ]->_offset;
								if( y_offset + height_offset > x_last )
									height_offset = x_last - y_offset;
								if( height_offset > 0 )
								{											
									XUIColor color( background->_gradient_stops[ index0 - 1 ]->_color._red , background->_gradient_stops[ index0 - 1 ]->_color._green , background->_gradient_stops[ index0 - 1 ]->_color._blue , (GLubyte)( (float)background->_gradient_stops[ index0 - 1 ]->_color._alpha * opacity ) );																	
									cache->AddDrawingVertex( this , XUIVertexF( x_next , y_center + y_offset , selected_z_index ) , &color , NULL );
									cache->AddDrawingVertex( this , XUIVertexF( y_next , y_center + y_offset , selected_z_index ) , &color , NULL );
									color = XUIColor( background->_gradient_stops[ index0 ]->_color._red , background->_gradient_stops[ index0 ]->_color._green , background->_gradient_stops[ index0 ]->_color._blue , (GLubyte)( (float)background->_gradient_stops[ index0 ]->_color._alpha * opacity ) );																	
									cache->AddDrawingVertex( this , XUIVertexF( y_next , y_center + height_offset + y_offset , selected_z_index ) , &color , NULL );
									cache->AddDrawingVertex( this , XUIVertexF( y_next , y_center + height_offset + y_offset , selected_z_index ) , &color , NULL );
									cache->AddDrawingVertex( this , XUIVertexF( x_next , y_center + height_offset + y_offset , selected_z_index ) , &color , NULL );
									color = XUIColor( background->_gradient_stops[ index0 - 1 ]->_color._red , background->_gradient_stops[ index0 - 1 ]->_color._green , background->_gradient_stops[ index0 - 1 ]->_color._blue , (GLubyte)( (float)background->_gradient_stops[ index0 - 1 ]->_color._alpha * opacity ) );							
									cache->AddDrawingVertex( this , XUIVertexF( x_next , y_center + y_offset , selected_z_index ) , &color , NULL );
									y_offset += height_offset;
								}
							}
						}								
						selected_z_index += 1.f;								
					}
					CRITICAL_SECTION_UNLOCK( background->_cs_gradient_stops );							
				}						
				else if( color->_color_type == 0 )
				{							
					rux::gui::Color* background = (rux::gui::Color*)color;
					if( background->_alpha > 0 && opacity > 0 )
					{								
						if( background->_alpha < 255 || opacity < 1.0f )
							cache->set_Alpha( this , 1 );
						else
							cache->set_Alpha( this , 0 );								
						XUIColor color( background->_red , background->_green , background->_blue , (GLubyte)( (float)background->_alpha * opacity ) );
						float x_center = x + border_width;
						float y_center = x + width - border_width;
						float x_last = y + border_width;
						float y_last =  x_last + ( height - 2 * border_width );
						cache->AddDrawingVertex( this , XUIVertexF( x_center , x_last , selected_z_index ) , &color , NULL );
						cache->AddDrawingVertex( this , XUIVertexF( y_center , x_last , selected_z_index ) , &color , NULL );
						cache->AddDrawingVertex( this , XUIVertexF( y_center , y_last , selected_z_index ) , &color , NULL );
						cache->AddDrawingVertex( this , XUIVertexF( y_center , y_last , selected_z_index ) , &color , NULL );
						cache->AddDrawingVertex( this , XUIVertexF( x_center , y_last , selected_z_index ) , &color , NULL );
						cache->AddDrawingVertex( this , XUIVertexF( x_center , x_last , selected_z_index ) , &color , NULL );
					}							
				}							
			};
			void RenderContext::update_color_to_cache( float width , float height , float border_width , float opacity , ::rux::gui::ColorBase* color , ::rux::gui::engine::RenderUniversalCache* cache )
			{
				if( color->_color_type == 1 )
				{							
					rux::gui::LinearGradientColor* background = (rux::gui::LinearGradientColor*)color;
					rux::uint8 visible = background->get_IsVisible() , alpha = 0;
					rux::uint32 index0 = 0;
					CRITICAL_SECTION_LOCK( background->_cs_gradient_stops );
					alpha = opacity < 1.0f ? 1 : background->get_IsAlpha();
					if( visible == 1 )
					{								
						cache->set_Alpha( this , alpha );														
						float x_center = width - 2 * border_width;
						float x_last = height - 2 * border_width;								
						if( background->_gradient_stops.Count() == 1 )
						{									
							XUIColor color( background->_gradient_stops[ 0 ]->_color._red , background->_gradient_stops[ 0 ]->_color._green , background->_gradient_stops[ 0 ]->_color._blue , (GLubyte)( (float)background->_gradient_stops[ 0 ]->_color._alpha * opacity ) );
							cache->UpdateDrawingColor( this , color );
							cache->UpdateDrawingColor( this , color );
							cache->UpdateDrawingColor( this , color );
							cache->UpdateDrawingColor( this , color );
							cache->UpdateDrawingColor( this , color );
							cache->UpdateDrawingColor( this , color );
						}
						else if( background->_linear_gradient_type == XEnum_LinearGradientType_Horizontal )
						{									
							float x_offset = 0;
							index0 = 1;
							if( background->_gradient_stops[ 0 ]->_offset > 0 )
							{										
								x_offset = x_center * background->_gradient_stops[ 0 ]->_offset;
								if( x_offset > x_center )
									x_offset = x_center;						
								XUIColor color( background->_gradient_stops[ 0 ]->_color._red , background->_gradient_stops[ 0 ]->_color._green , background->_gradient_stops[ 0 ]->_color._blue , (GLubyte)( (float)background->_gradient_stops[ 0 ]->_color._alpha * opacity ) );
								cache->UpdateDrawingColor( this , color );
								cache->UpdateDrawingColor( this , color );
								cache->UpdateDrawingColor( this , color );
								cache->UpdateDrawingColor( this , color );
								cache->UpdateDrawingColor( this , color );
								cache->UpdateDrawingColor( this , color );					
							}
							for( ; index0 < background->_gradient_stops.Count() ; index0++ )
							{										
								if( x_offset >= x_center )
									break;
								float width_offset = x_center * background->_gradient_stops[ index0 ]->_offset;
								if( x_offset + width_offset > x_center )
									width_offset = x_center - x_offset;
								if( width_offset > 0 )
								{											
									XUIColor color( background->_gradient_stops[ index0 - 1 ]->_color._red , background->_gradient_stops[ index0 - 1 ]->_color._green , background->_gradient_stops[ index0 - 1 ]->_color._blue , (GLubyte)( (float)background->_gradient_stops[ index0 - 1 ]->_color._alpha * opacity ) );							
									cache->UpdateDrawingColor( this , color );						
									color = XUIColor( background->_gradient_stops[ index0 ]->_color._red , background->_gradient_stops[ index0 ]->_color._green , background->_gradient_stops[ index0 ]->_color._blue , (GLubyte)( (float)background->_gradient_stops[ index0 ]->_color._alpha * opacity ) );							
									cache->UpdateDrawingColor( this , color );
									cache->UpdateDrawingColor( this , color );
									cache->UpdateDrawingColor( this , color );						
									color = XUIColor( background->_gradient_stops[ index0 - 1 ]->_color._red , background->_gradient_stops[ index0 - 1 ]->_color._green , background->_gradient_stops[ index0 - 1 ]->_color._blue , (GLubyte)( (float)background->_gradient_stops[ index0 - 1 ]->_color._alpha * opacity ) );							
									cache->UpdateDrawingColor( this , color );
									cache->UpdateDrawingColor( this , color );						
									x_offset += width_offset;
								}
							}
						}			
						else if( background->_linear_gradient_type == XEnum_LinearGradientType_Vertical )
						{
							float y_offset = 0;
							index0 = 1;
							if( background->_gradient_stops[ 0 ]->_offset > 0 )
							{										
								y_offset = x_last * background->_gradient_stops[ 0 ]->_offset;
								if( y_offset > x_last )
									y_offset = x_last;
								XUIColor color( background->_gradient_stops[ 0 ]->_color._red , background->_gradient_stops[ 0 ]->_color._green , background->_gradient_stops[ 0 ]->_color._blue , (GLubyte)( (float)background->_gradient_stops[ 0 ]->_color._alpha * opacity ) );																
								cache->UpdateDrawingColor( this , color );
								cache->UpdateDrawingColor( this , color );
								cache->UpdateDrawingColor( this , color );
								cache->UpdateDrawingColor( this , color );
								cache->UpdateDrawingColor( this , color );
								cache->UpdateDrawingColor( this , color );					
							}
							for( ; index0 < background->_gradient_stops.Count() ; index0++ )
							{										
								if( y_offset >= x_last )
									break;
								float height_offset = x_last * background->_gradient_stops[ index0 ]->_offset;
								if( y_offset + height_offset > x_last )
									height_offset = x_last - y_offset;
								if( height_offset > 0 )
								{											
									XUIColor color( background->_gradient_stops[ index0 - 1 ]->_color._red , background->_gradient_stops[ index0 - 1 ]->_color._green , background->_gradient_stops[ index0 - 1 ]->_color._blue , (GLubyte)( (float)background->_gradient_stops[ index0 - 1 ]->_color._alpha * opacity ) );																	
									cache->UpdateDrawingColor( this , color );
									cache->UpdateDrawingColor( this , color );						
									color = XUIColor( background->_gradient_stops[ index0 ]->_color._red , background->_gradient_stops[ index0 ]->_color._green , background->_gradient_stops[ index0 ]->_color._blue , (GLubyte)( (float)background->_gradient_stops[ index0 ]->_color._alpha * opacity ) );																	
									cache->UpdateDrawingColor( this , color );
									cache->UpdateDrawingColor( this , color );
									cache->UpdateDrawingColor( this , color );						
									color = XUIColor( background->_gradient_stops[ index0 - 1 ]->_color._red , background->_gradient_stops[ index0 - 1 ]->_color._green , background->_gradient_stops[ index0 - 1 ]->_color._blue , (GLubyte)( (float)background->_gradient_stops[ index0 - 1 ]->_color._alpha * opacity ) );							
									cache->UpdateDrawingColor( this , color );																
									y_offset += height_offset;
								}
							}
						}											
					}
					CRITICAL_SECTION_UNLOCK( background->_cs_gradient_stops );							
				}						
				else if( color->_color_type == 0 )
				{							
					rux::gui::Color* background = (rux::gui::Color*)color;
					if( background->_alpha > 0 && opacity > 0 )
					{								
						if( background->_alpha < 255 || opacity < 1.0f )
							cache->set_Alpha( this , 1 );
						else
							cache->set_Alpha( this , 0 );								
						XUIColor color( background->_red , background->_green , background->_blue , (GLubyte)( (float)background->_alpha * opacity ) );																											
						cache->UpdateDrawingColor( this , color );							
						cache->UpdateDrawingColor( this , color );							
						cache->UpdateDrawingColor( this , color );							
						cache->UpdateDrawingColor( this , color );							
						cache->UpdateDrawingColor( this , color );							
						cache->UpdateDrawingColor( this , color );
					}							
				}							
			};
			void RenderContext::DrawText( ::rux::int32 left , ::rux::int32 top , ::rux::int32 width , ::rux::int32 height , const char* utf8text , ::rux::gui::FontBase* font , ::rux::gui::Color* color , ::rux::gui::RenderCacheBase** cache , float opacity , float& _selected_z_index , const char* __filename__ , ::rux::int32 __line__ , size_t ___rux__thread_index1986 )
			{//return;
				if( _clip._width > 0 
					&& _clip._height > 0
					&& left <= _clip._left + _clip._width
					&& top <= _clip._top + _clip._height
					&& left + width >= _clip._left
					&& top + height >= _clip._top )
				{
					_render_objects_count++;
					if( color )
						color->AddRef();

					RenderUniversalCache* univ_cache1 = (RenderUniversalCache*)(*cache);
					if( univ_cache1 )
					{
						if( univ_cache1->_is_reset == 0 && ( _clip._left != univ_cache1->_old_clip._left 
							|| _clip._top != univ_cache1->_old_clip._top
							|| _clip._height != univ_cache1->_old_clip._height
							|| _clip._width != univ_cache1->_old_clip._width
							|| univ_cache1->_old_left != left
							|| univ_cache1->_old_top != top
							|| univ_cache1->_old_height != height
							|| univ_cache1->_old_width != width )
							)
							univ_cache1->Reset();						
						if( univ_cache1->_is_reset == 0 && univ_cache1->_old_color && color )
						{
							if( univ_cache1->_old_color->Equals( color ) == 0 )
							{
								if( univ_cache1->_old_color->_color_type != color->_color_type )
									univ_cache1->Reset();
								else
								{
									if( univ_cache1->_old_color->_color_type == 1 )
									{
										::rux::gui::LinearGradientColor* linear_gradient_color0 = (::rux::gui::LinearGradientColor*)color;
										::rux::gui::LinearGradientColor* linear_gradient_color1 = (::rux::gui::LinearGradientColor*)univ_cache1->_old_color;
										CRITICAL_SECTION_LOCK( linear_gradient_color0->_cs_gradient_stops );
										CRITICAL_SECTION_LOCK( linear_gradient_color1->_cs_gradient_stops );
										if( linear_gradient_color0->_gradient_stops.Count() == linear_gradient_color1->_gradient_stops.Count() )
											univ_cache1->ResetColor();
										else
											univ_cache1->Reset();										
										linear_gradient_color1->_cs_gradient_stops.UnLock();
										linear_gradient_color0->_cs_gradient_stops.UnLock();
									}
									else
										univ_cache1->ResetColor();								
								}
							}
						}
						if( univ_cache1->_is_reset == 0 && univ_cache1->_old_opacity != opacity )
							univ_cache1->ResetColor();

						copy_color( univ_cache1->_old_color , color );						
						univ_cache1->_old_clip = _clip;
						univ_cache1->_old_opacity = opacity;
						univ_cache1->_old_left = left;
						univ_cache1->_old_top = top;
						univ_cache1->_old_height = height;
						univ_cache1->_old_width = width;
					}

					check_cache( cache , __FILE__ , __LINE__ );
					RenderUniversalCache* ui_cache = (RenderUniversalCache*)(*cache);					

					size_t index0 = 0;
					rux::gui::engine::RenderFont* render_font = NULL;
					for( ; index0 < _gl->_fonts.Count() ; index0++ )
					{
						if( ( font->_font_file_name_hash == _gl->_fonts[ index0 ]->_font_file_name_hash
							|| font->_font_short_name_hash == _gl->_fonts[ index0 ]->_font_short_name_hash )
							&& font->_font_size_width == _gl->_fonts[ index0 ]->_font_size_width 
							&& font->_font_size_height == _gl->_fonts[ index0 ]->_font_size_height )
						{
							render_font = _gl->_fonts[ index0 ];
							break;
						}
					}
					if( index0 >= _gl->_fonts.Count() )
					{						
						ft_structs_h::FT_UInt font_size_height = font->_font_size_height;
						ft_structs_h::FT_UInt font_size_width = font->_font_size_width;
						render_font = alloc_object_4_params_macros( ::rux::gui::engine::RenderFont , _gl , ::rux::XString( font->_font_file_name , XEnumCodePage_UTF8 , 1 , NULL , __FILE__ , __LINE__ ), font_size_width , font_size_height );
						_gl->_fonts.Add( render_font );
					}
					if( ui_cache->_render_font != render_font )
						ui_cache->Reset();
					render_font->Render( this , utf8text , left , top , width , height , color , ui_cache , opacity , _selected_z_index , _stencil_ref , ___rux__thread_index1986 );		
					if( color )
						color->Release();
				}
			};			
			void RenderContext::DrawPath( ::rux::int32 left , ::rux::int32 top , ::rux::int32 width , ::rux::int32 height , XMallocArray< ::rux::gui::Point* >& points , float opacity , ::rux::gui::ColorBase* background_object_color , ::rux::gui::RenderCacheBase** cache , float& _selected_z_index , ::rux::uint8 , size_t ___rux__thread_index1986 )
			{
				return;
				XMallocArray< float > point_x , point_y;
				for( ::rux::uint32 index0 = 0 ; index0 < points.Count() ; index0++ )
				{				
					if( points[ index0 ]->_point_unit == ::rux::gui::XEnum_PointUnit_Percent )
					{
						point_x.Add( ( left + points[ index0 ]->_x * width ) );
						point_y.Add( ( top + points[ index0 ]->_y * height ) );
					}
					else if( points[ index0 ]->_point_unit == ::rux::gui::XEnum_PointUnit_Pixel )
					{
						point_x.Add( ( left+ points[ index0 ]->_x ) );
						point_y.Add( ( top + points[ index0 ]->_y ) );
					}
				}
				DrawPath( left , top , width , height , point_x , point_y , opacity , background_object_color , cache , _selected_z_index , 0 , ___rux__thread_index1986 );
			};
			void RenderContext::MapBuffer( rux::uint32* buffer_ptr , GLubyte*& mapped_buffer , size_t ___rux__thread_index1986 )
			{
				if( mapped_buffer == NULL || buffer_ptr[ 0 ] != _gl->_gl_bind_buffer_gl_array_buffer )
				{					
					rux::gui::engine::OpenGL::glBindBuffer( GL_ARRAY_BUFFER, buffer_ptr[ 0 ] , _gl , __FILE__ , __LINE__ , ___rux__thread_index1986 );
					mapped_buffer = (GLubyte*)rux::gui::engine::OpenGL::glMapBuffer( GL_ARRAY_BUFFER , GL_WRITE_ONLY , _gl , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
				}
			};
			void RenderContext::DeleteBuffers( rux::uint32*& buffer_ptr , ::rux::uint32& buffer_ptr_size , size_t ___rux__thread_index1986 )
			{
				rux::gui::engine::OpenGL::glDeleteBuffers( 1 , buffer_ptr , _gl , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
				rux::engine::free_mem( buffer_ptr );
				buffer_ptr = NULL;
				buffer_ptr_size = 0;
			};
			void RenderContext::GenBuffers( rux::uint32*& buffer_ptr , ::rux::uint32& buffer_ptr_size , size_t new_size , size_t ___rux__thread_index1986 )
			{
				buffer_ptr = alloc_array_macros( ::rux::uint32 , 1 );
				rux::gui::engine::OpenGL::glBindBuffer( GL_ARRAY_BUFFER , 0 , _gl , __FILE__ , __LINE__ , ___rux__thread_index1986 );
				rux::gui::engine::OpenGL::glGenBuffers( 1 , buffer_ptr , _gl , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
				rux::gui::engine::OpenGL::glBindBuffer( GL_ARRAY_BUFFER , buffer_ptr[ 0 ] , _gl , __FILE__ , __LINE__ , ___rux__thread_index1986 );
				buffer_ptr_size = new_size;
				rux::gui::engine::OpenGL::glBufferData( GL_ARRAY_BUFFER , buffer_ptr_size , NULL, GL_STREAM_DRAW , _gl , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
				rux::gui::engine::OpenGL::glMapBuffer( GL_ARRAY_BUFFER , GL_WRITE_ONLY , _gl , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
				rux::gui::engine::OpenGL::glUnmapBuffer( GL_ARRAY_BUFFER , _gl , __FUNCTION__ , __LINE__ , ___rux__thread_index1986 );
				rux::gui::engine::OpenGL::glBindBuffer( GL_ARRAY_BUFFER , 0 , _gl , __FILE__ , __LINE__ , ___rux__thread_index1986 );
			};
			void RenderContext::DrawImage( ::rux::int32 fourcc , ::rux::int32 left , ::rux::int32 top , ::rux::int32 width , ::rux::int32 height , float opacity , ::rux::gui::RenderCacheBase** cache , float brightness , float contrast , float& selected_z_index , ::rux::uint8 is_alpha , rux::int32 image_left , rux::int32 image_top , rux::int32 image_width , rux::int32 image_height , const char* __filename__ , ::rux::int32 __line__ , size_t ___rux__thread_index1986 )
			{
				_render_objects_count++;
				if( _clip._width > 0 
					&& _clip._height > 0
					&& left <= _clip._left + _clip._width
					&& top <= _clip._top + _clip._height
					&& left + width >= _clip._left
					&& top + height >= _clip._top )
				{
					RenderUniversalCache* univ_cache0 = (RenderUniversalCache*)(*cache);
					if( univ_cache0 )
					{
						if( image_left < 0 || image_left >= univ_cache0->_texture_data_width )
							image_left = 0;
						if( image_top < 0 || image_top >= univ_cache0->_texture_data_height )
							image_top = 0;
						if( image_width < 0 || image_width > univ_cache0->_texture_data_width )
							image_width = univ_cache0->_texture_data_width;
						if( image_height < 0 || image_height > univ_cache0->_texture_data_height )
							image_height = univ_cache0->_texture_data_height;
						if( image_left + image_width > univ_cache0->_texture_data_width )
							image_left = univ_cache0->_texture_data_width - image_width;
						if( image_top + image_height > univ_cache0->_texture_data_height )
							image_top = univ_cache0->_texture_data_height - image_height;
						if( univ_cache0->_is_reset == 0 && ( _clip._left != univ_cache0->_old_clip._left 
							|| _clip._top != univ_cache0->_old_clip._top
							|| _clip._height != univ_cache0->_old_clip._height
							|| _clip._width != univ_cache0->_old_clip._width
							|| univ_cache0->_old_left != left
							|| univ_cache0->_old_top != top
							|| univ_cache0->_old_height != height
							|| univ_cache0->_old_width != width 
							|| univ_cache0->_old_image_left != image_left
							|| univ_cache0->_old_image_top != image_top
							|| univ_cache0->_old_image_height != image_height
							|| univ_cache0->_old_image_width != image_width )
							)
							univ_cache0->Reset();
						if( univ_cache0->_is_reset == 0 && univ_cache0->_old_opacity != opacity )
							univ_cache0->ResetColor();
				
						univ_cache0->_old_clip = _clip;
						univ_cache0->_old_opacity = opacity;
						univ_cache0->_old_left = left;
						univ_cache0->_old_top = top;
						univ_cache0->_old_height = height;
						univ_cache0->_old_width = width;
						univ_cache0->_old_image_left = image_left;
						univ_cache0->_old_image_top = image_top;
						univ_cache0->_old_image_height = image_height;
						univ_cache0->_old_image_width = image_width;
					}
					if( check_cache( cache , __filename__ , __line__ ) )
						return;
					RenderUniversalCache* ui_cache = (RenderUniversalCache*)(*cache);
					if( ui_cache->_texture_ptr == NULL && ui_cache->_gl_texture0 == 0 && ui_cache->_is_draw_pixels == 0 )
						return;
					ui_cache->_fourcc = fourcc;
					ui_cache->_opacity = opacity;
					ui_cache->_brightness = brightness;
					ui_cache->_contrast = contrast;
					if( ui_cache->_is_reset == 1 )
					{
						ui_cache->set_Alpha( this , is_alpha );
						if( opacity < 1.0f )
							ui_cache->set_Alpha( this , 1 );
						XMallocArray< ::rux::gui::Rectangle > cuts;
						if( left < _clip._left )
							cuts.Add( ::rux::gui::Rectangle( left , top , _clip._left - left , height ) );					
						if( top < _clip._top )
							cuts.Add( ::rux::gui::Rectangle( left , top , width , _clip._top - top ) );
						if( left + width > _clip._left + _clip._width )
							cuts.Add( ::rux::gui::Rectangle( _clip._left + _clip._width , top , left + width - _clip._left - _clip._width , height ) );
						if( top + height > _clip._top + _clip._height )
							cuts.Add( ::rux::gui::Rectangle( left , _clip._top + _clip._height , width , top + height - _clip._top - _clip._height ) );					
						if( cuts.Count() > 0 )
						{
							if( ui_cache->get_FillStencilVertexesCount( this ) == 0 )
								rux_draw_rectangle_and_save_to_fill_stencil( this , _clip._left , _clip._top , _clip._width , _clip._height , selected_z_index , ui_cache ); 				
							for( ::rux::uint32 index0 = 0 ; index0 < cuts.Count() ; index0++ )
								rux_draw_rectangle_and_save_to_cut_stencil( this , cuts[ index0 ]._left , cuts[ index0 ]._top , cuts[ index0 ]._width , cuts[ index0 ]._height , selected_z_index , ui_cache );
						}
						XUITextureCoordinate texture_coordinate( image_left , image_top );
						ui_cache->AddDrawingVertex( this , XUIVertexF( left , top , selected_z_index ) , NULL , &texture_coordinate );
						texture_coordinate = XUITextureCoordinate( (GLshort)( image_left + image_width ) , image_top );
						ui_cache->AddDrawingVertex( this , XUIVertexF( left + width , top , selected_z_index ) , NULL , &texture_coordinate );
						texture_coordinate = XUITextureCoordinate( (GLshort)( image_left + image_width ) , (GLshort)( image_top + image_height ) );
						ui_cache->AddDrawingVertex( this , XUIVertexF( left + width , top + height , selected_z_index ) , NULL , &texture_coordinate );
						texture_coordinate = XUITextureCoordinate( (GLshort)( image_left + image_width ) , (GLshort)( image_top + image_height ) );
						ui_cache->AddDrawingVertex( this , XUIVertexF( left + width , top + height , selected_z_index ) , NULL , &texture_coordinate );
						texture_coordinate = XUITextureCoordinate( image_left , image_top );
						ui_cache->AddDrawingVertex( this , XUIVertexF( left , top , selected_z_index ) , NULL , &texture_coordinate );
						texture_coordinate = XUITextureCoordinate( image_left , (GLshort)( image_top + image_height ) );
						ui_cache->AddDrawingVertex( this , XUIVertexF( left , top + height , selected_z_index ) , NULL , &texture_coordinate );
					}
					ui_cache->set_ZIndex( this , selected_z_index );
				}
			};
		};
	};
};
