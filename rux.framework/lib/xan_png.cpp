#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_png.h>
#include <xan_error.h>
#include <xan_io_defines.h>
#include <xan_gui_defines.h>
#include <zlib.h>
namespace rux
{
	namespace media
	{
#define APNG_DISPOSE_OP_NONE 0
#define APNG_DISPOSE_OP_BACKGROUND 1
#define APNG_DISPOSE_OP_PREVIOUS 2
#define APNG_BLEND_OP_SOURCE 0
#define APNG_BLEND_OP_OVER 1
#define png_clip( x ) x > 0 ? x < 255 ? x : 255 : 0
#define png_revert_bytes_32bits( field ) ( field[ 0 ] << 24 ) | ( field[ 1 ] << 16 ) | ( field[ 2 ] << 8 ) | field[ 3 ]
#define png_revert_bytes_16bits( field ) ( field[ 0 ] << 8 ) | field[ 1 ]
		png_cHRM::png_cHRM()
			: _exists( 0 )
		{
		};
		::rux::uint32 png_cHRM::white_point_x()
		{
			return png_revert_bytes_32bits( _white_point_x );
		};
		::rux::uint32 png_cHRM::white_point_y()
		{
			return png_revert_bytes_32bits( _white_point_y );
		};
		::rux::uint32 png_cHRM::red_x()
		{
			return png_revert_bytes_32bits( _red_x );
		};
		::rux::uint32 png_cHRM::red_y()
		{
			return png_revert_bytes_32bits( _red_y );
		};
		::rux::uint32 png_cHRM::green_x()
		{
			return png_revert_bytes_32bits( _green_x );
		};
		::rux::uint32 png_cHRM::green_y()
		{
			return png_revert_bytes_32bits( _green_y );
		};
		::rux::uint32 png_cHRM::blue_x()
		{
			return png_revert_bytes_32bits( _blue_x );
		};
		::rux::uint32 png_cHRM::blue_y()
		{
			return png_revert_bytes_32bits( _blue_y );
		};
		bool png_chunk::is_ihdr( void )
		{
			return _chunk_type == 1380206665;
		};
		bool png_chunk::is_pHYs( void )			
		{
			return _chunk_type == 1935231088;
		};
		bool png_chunk::is_iCCP( void )			
		{
			return _chunk_type == 1346585449;
		};		
		bool png_chunk::is_cHRM( void )			
		{
			return _chunk_type == 1297238115;
		};
		bool png_chunk::is_IDAT( void )			
		{
			return _chunk_type == 1413563465;
		};
		bool png_chunk::is_IEND( void )			
		{
			return _chunk_type == 1145980233;
		};
		bool png_chunk::is_acTL( void )
		{
			return _chunk_type == 1280598881;
		};
		bool png_chunk::is_sRGB( void )
		{
			return _chunk_type == 1111970419;
		};
		bool png_chunk::is_bKGD( void )
		{
			return _chunk_type == 1145523042;
		};
		bool png_chunk::is_fcTL( void )
		{
			return _chunk_type == 1280598886;
		};
		bool png_chunk::is_fdAT( void )
		{
			return _chunk_type == 1413571686;
		};
		::rux::uint32 png_chunk::length( void )
		{
			return png_revert_bytes_32bits( _length );
		};
		::rux::uint32 png_ihdr::width( void )
		{
			return png_revert_bytes_32bits( _width );
		};
		::rux::uint32 png_ihdr::height( void )
		{
			return png_revert_bytes_32bits( _height );
		};
		::rux::uint32 png_fcTL::sequence_number( void )
		{
			return png_revert_bytes_32bits( _sequence_number );
		};
		::rux::uint32 png_fcTL::width( void )
		{
			return png_revert_bytes_32bits( _width );
		};
		::rux::uint32 png_fcTL::height( void )
		{
			return png_revert_bytes_32bits( _height );
		};
		::rux::uint32 png_fcTL::x_offset( void )
		{
			return png_revert_bytes_32bits( _x_offset );
		};
		::rux::uint32 png_fcTL::y_offset( void )
		{
			return png_revert_bytes_32bits( _y_offset );
		};
		::rux::int16 png_fcTL::delay_num( void )
		{
			return png_revert_bytes_16bits( _delay_num );
		};
		::rux::int16 png_fcTL::delay_den( void )
		{
			return png_revert_bytes_16bits( _delay_den );
		};
		::rux::uint32 png_acTL::num_frames( void )
		{
			return png_revert_bytes_32bits( _num_frames );
		};
		::rux::uint32 png_acTL::num_plays( void )
		{
			return png_revert_bytes_32bits( _num_plays );
		};
		png::png( const char* filename )
		{	
			//memset( _bkgd , 0 , 3 );
			_dispose_op = APNG_DISPOSE_OP_BACKGROUND;
			_play_index = 0;
			_frame_index = 0;
			_animation_stopped = 0;
			_is_animation = 0;
			_stream = NULL;
			_image_begin_position = 0ULL;
			_image_buffer_size = 0;
			_next_image_last_time = 0;
			_image_region_buffer = NULL;
			_image_region_buffer_size = 0;
			to_fourcc( filename );
		};
		png::png( ::rux::io::stream* stream )
		{		
			//memset( _bkgd , 0 , 3 );
			_dispose_op = APNG_DISPOSE_OP_BACKGROUND;
			_play_index = 0;
			_frame_index = 0;
			_animation_stopped = 0;
			_is_animation = 0;
			_stream = NULL;
			_image_begin_position = 0ULL;
			_image_buffer_size = 0;
			_next_image_last_time = 0;
			_image_region_buffer = NULL;
			_image_region_buffer_size = 0;
			to_fourcc( stream , *this );
		};
		png::~png( void )
		{
			if( _stream )
			{
				_stream->release();
				_stream = NULL;
			}
			for( size_t index0 = 0 ; index0 < _image_buffers.Count() ; index0++ )
				::rux::engine::free_mem( _image_buffers[ index0 ] );
			if( _image_region_buffer )
				::rux::engine::free_mem( _image_region_buffer );
		};
		::rux::byte png::is_animation( void )
		{
			return _is_animation;
		};
		::rux::media::XBitmapInfoHeader& png::get_BitmapInfoHeader( void )
		{
			return _bitmap_info_header;
		};
		void png::copy_from( ::rux::media::image* src )
		{
		};
		::rux::byte* png::next_image( ::rux::byte& new_image )
		{
			new_image = 0;
			::rux::byte* next_image = NULL;
			if( _stream )
			{
				if( _animation_stopped == 0 )
				{
					if( _is_animation )
					{
						if( _frame_index >= _png_acTL.num_frames() )
						{
							_frame_index = 0;
							_play_index++;
							if( _png_acTL.num_plays() != 0 && _play_index >= _png_acTL.num_plays() )
								_animation_stopped = 1;
						}
					}
					else
						_animation_stopped = 1;
					if( _frame_index < _image_buffers.Count() )
					{
						::rux::uint32 now = ::rux::XTime::GetTickCount();
						if( _next_image_last_time > now )
							_next_image_last_time = now;
						if( _next_image_last_time == 0 || now - _next_image_last_time >= _image_buffers_delay[ _frame_index ] )
						{
							if( _frame_index + 1 >= _png_acTL.num_frames() )
								next_image = _image_buffers[ 0 ];
							else
								next_image = _image_buffers[ _frame_index + 1 ];
							_next_image_last_time = now;
							new_image = 1;
						}
					}
					else
					{
						::rux::uint32 unix_delay_tickcount = 0;
						if( _frame_index == 0 )
							_stream->seek( _image_begin_position );
						::rux::media::png_chunk png_chunk;
						size_t readen = 0;
						::rux::media::png_fcTL png_fcTL , * png_fcTL_ptr = NULL;
						for( ; ; )
						{
							if( _stream->read_bytes( (char*)&png_chunk , sizeof( ::rux::media::png_chunk ) , readen ) )
							{										
								if( readen == sizeof( ::rux::media::png_chunk ) )
								{
									if( png_chunk.is_IDAT() || png_chunk.is_fdAT() )
									{
										if( get_image( png_chunk , png_fcTL_ptr , png_chunk.is_fdAT() ? 1 : 0 ) )
										{
											next_image = _image_buffers[ _frame_index ];
											_image_buffers_delay.Add( unix_delay_tickcount );
											_next_image_last_time = ::rux::XTime::GetTickCount();
											new_image = 1;
										}
										break;									
									}
									else if( png_chunk.is_IEND() )
										break;
									else if( png_chunk.is_fcTL() )
									{										
										readen = 0;
										if( _stream->read_bytes( (char*)&png_fcTL , sizeof( ::rux::media::png_fcTL ) , readen ) )
										{										
											if( readen == sizeof( ::rux::media::png_fcTL ) )
											{
												::rux::int16 delay_num = png_fcTL.delay_num();
												if( delay_num == 0 )
													png_fcTL_ptr = &png_fcTL;
												else
												{
													::rux::int16 delay_den = png_fcTL.delay_den();
													if( delay_den == 0 )
														delay_den = 100;
													unix_delay_tickcount = ( (float)delay_num / (float)delay_den ) * 1000U;
													::rux::uint32 now = ::rux::XTime::GetTickCount();
													if( _next_image_last_time > now )
														_next_image_last_time = now;
													if( _next_image_last_time == 0 || now - _next_image_last_time >= unix_delay_tickcount )
														png_fcTL_ptr = &png_fcTL;
													else
													{
														::rux::uint64 pos = _stream->position();
														_stream->seek( pos - sizeof( ::rux::media::png_fcTL ) - sizeof( ::rux::media::png_chunk ) );
														break;
													}
												}
											}
											else
												break;
										}
										else
											break;
									}
									else
										_stream->seek( png_chunk.length() + 4 , ::rux::io::XEnum_SetPosition_FromCurrentPosition );
								}
								else
									break;
							}
							else
								break;
						}
					}
					if( _is_animation && new_image )
						_frame_index++;					
				}
				else
				{
					if( _image_buffers.Count() > 0 )
					{
						if( _frame_index >= _image_buffers.Count() )
							_frame_index = 0;
						next_image = _image_buffers[ _frame_index ];
					}
				}
			}
			return next_image;
		};
		bool png::to_fourcc( const char* filename )
		{
			return to_fourcc( filename , *this );
		};
		bool png::get_image( png_chunk& chunk , png_fcTL* fcTL , ::rux::byte is_fdAT )
		{
			bool res = false;
			if( _stream )
			{				
				_image_buffer_size = _bitmap_info_header._size_image + _bitmap_info_header._width;
				::rux::byte* _image_buffer = alloc_array_macros( ::rux::byte , _image_buffer_size );
				if( _dispose_op == APNG_DISPOSE_OP_NONE && _image_buffers.Count() > 0 )
					memcpy( _image_buffer , _image_buffers[ _image_buffers.Count() - 1 ] , _image_buffer_size );
				else if( _dispose_op == APNG_DISPOSE_OP_BACKGROUND || _image_buffers.Count() == 0 )
				{
					/*if( _bkgd[ 0 ] == _bkgd[ 1 ] && _bkgd[ 1 ] == _bkgd[ 2 ] )
						memset( _image_buffer , _bkgd[ 0 ] , _bitmap_info_header._size_image );
					else if( _bitmap_info_header._compression == ::rux::media::XEnum_Fourcc_RGB )
					{
						size_t pixel_components = _bitmap_info_header._bit_count / 8;
						if( pixel_components >= 3 )
						{
							::rux::byte* ptr = _image_buffer;
							::rux::byte* end = _image_buffer + _bitmap_info_header._size_image;
							while( ptr < end )
							{
								memcpy( ptr , _bkgd , 3 );
								ptr += pixel_components;
							}
						}
					}
					else*/
						memset( _image_buffer , 0 , _bitmap_info_header._size_image );
				}
				else if( _dispose_op == APNG_DISPOSE_OP_PREVIOUS && _image_buffers.Count() > 1 )
					memcpy( _image_buffer , _image_buffers[ _image_buffers.Count() - 2 ] , _image_buffer_size );
				::rux::uint32 length = chunk.length();
				::rux::uint64 img_pos = _stream->position();
				if( _stream->seek( length , ::rux::io::XEnum_SetPosition_FromCurrentPosition ) )
				{
					::rux::uint32 crc = 0;
					size_t readen = 0;
					if( _stream->read_bytes( (char*)&crc , 4 , readen ) )
					{										
						if( readen == 4 )
						{
							size_t pixel_size = 0;
							if( _bitmap_info_header._compression == ::rux::media::XEnum_Fourcc_GREY )
								pixel_size = 1;
							else if( _bitmap_info_header._compression == ::rux::media::XEnum_Fourcc_RGB )
								pixel_size = _bitmap_info_header._bit_count / 8;
							else
								res = false;
							res = true;
							if( is_fdAT )
							{
								if( _stream->seek( img_pos + 4 , ::rux::io::XEnum_SetPosition_FromBegin ) == false )
									res = false;
								length -= 4;
							}
							else
							{
								if( _stream->seek( img_pos , ::rux::io::XEnum_SetPosition_FromBegin ) == false )
									res = false;
							}
							if( res )
							{
								::rux::byte** image_buffer = &_image_buffer;
								size_t* image_buffer_size = &_image_buffer_size;
								::rux::uint32 width = _bitmap_info_header._width;
								::rux::uint32 size_image = _bitmap_info_header._size_image;
								if( fcTL )
								{
									::rux::uint32 height = fcTL->height();
									width = fcTL->width();
									::rux::uint32 x_offset = fcTL->x_offset();
									::rux::uint32 y_offset = fcTL->y_offset();
									if( x_offset >= 0 && y_offset >= 0 && width > 0 && height > 0 && x_offset + width <= _bitmap_info_header._width && y_offset + height <= _bitmap_info_header._height )
									{
										if( _bitmap_info_header._width != width || _bitmap_info_header._height != height )
										{
											size_image = ::rux::media::image_size( _bitmap_info_header._compression , width , height , _bitmap_info_header._bit_count );
											image_buffer = &_image_region_buffer;
											image_buffer_size = &_image_region_buffer_size;
										}
										else if( fcTL->_blend_op != APNG_BLEND_OP_SOURCE )
										{
											image_buffer = &_image_region_buffer;
											image_buffer_size = &_image_region_buffer_size;
										}
										_dispose_op = fcTL->_dispose_op;
									}
									else
										res = false;
								}
								if( size_image > 0 )
								{
									res = false;
									size_image += width;
									if( image_buffer != &_image_buffer )
									{
										if( (*image_buffer) && size_image > (::rux::uint32)(*image_buffer_size) )
										{
											::rux::engine::free_mem( (*image_buffer) );
											(*image_buffer) = NULL;
											(*image_buffer_size) = 0;
										}
										if( (*image_buffer) == NULL )
										{
											(*image_buffer) = alloc_array_macros( ::rux::uint8 , size_image );
											(*image_buffer_size) = size_image;
										}
									}
									::rux::io::memory_stream dst_stream( (rux::byte*)(*image_buffer) , size_image , 0 , 0 );

									::rux::int32 z_res = Z_OK;
									::rux::uint32 have = 0;
									z_stream strm;
									::rux::byte in[ 16384 ] = {0};
									::rux::byte out[ 16384 ] = {0};
									strm.zalloc = Z_NULL;
									strm.zfree = Z_NULL;
									strm.opaque = Z_NULL;
									strm.avail_in = 0;
									strm.next_in = Z_NULL;
									z_res = inflateInit( &strm );
									::rux::uint32 available = length;
									if( z_res == Z_OK )
									{
										res = true;
										do
										{
											if( res == false && available == 0 )
												break;
											if( _stream->read_bytes( (char*)in , 16384 , readen ) == false )
												res = false;
											if( readen > available )
												strm.avail_in = available;
											else
												strm.avail_in = readen;
											available -= strm.avail_in; 
											if( strm.avail_in == 0 )
												break;
											strm.next_in = in;
											do
											{
												strm.avail_out = 16384;
												strm.next_out = out;
												z_res = inflate( &strm , Z_NO_FLUSH );
												if( z_res == Z_NEED_DICT || z_res == Z_DATA_ERROR || z_res == Z_MEM_ERROR )
												{
													res = false;
													break;
												}
												else
												{
													have = 16384 - strm.avail_out;
													if( dst_stream.write_bytes( (char*)out , have , readen ) == false )
													{
														res = false;
														break;
													}
												}
											} 
											while( strm.avail_out == 0 );
										}
										while( z_res != Z_STREAM_END );
										inflateEnd( &strm );
										if( res && ( dst_stream.position() < ( size_image - width ) || dst_stream.position() > size_image ) )
											res = false;
										size_image -= width;
										if( res )
										{
											size_t scanline_size = 0;
											
											if( res )
											{
												scanline_size = pixel_size * width + 1;
												::rux::uint8* ptr = (*image_buffer);
												::rux::uint8* img_line = (*image_buffer);
												while( (::rux::uint32)( ptr - (*image_buffer) ) < size_image )
												{
													switch( *ptr )
													{
													case 0:
														{
															::rux::engine::memmove( img_line , 0 , ptr , 1 , scanline_size - 1 );
															break;
														}
													case 1:
														{
															filter_sub( img_line , &ptr[ 1 ] , scanline_size - 1 , pixel_size );
															break;
														}
													case 2:
														{
															::rux::uint8* prev_img_line = NULL;
															if( img_line != (*image_buffer) )
																prev_img_line = img_line - scanline_size + 1;
															filter_up( img_line , &ptr[ 1 ] , prev_img_line , scanline_size - 1 );
															break;
														}
													case 3:
														{
															::rux::uint8* prev_img_line = NULL;
															if( img_line != (*image_buffer) )
																prev_img_line = img_line - scanline_size + 1;
															filter_average( img_line , &ptr[ 1 ] , prev_img_line , scanline_size - 1 , pixel_size );
															break;
														}
													case 4:
														{
															::rux::uint8* prev_img_line = NULL;
															if( img_line != (*image_buffer) )
																prev_img_line = img_line - scanline_size + 1;
															filter_paeth( img_line , &ptr[ 1 ] , prev_img_line , scanline_size - 1 , pixel_size );
															break;
														}
													default:
														{
															res = false;
															break;
														}
													}				
													if( res == false )
														break;
													img_line += scanline_size - 1;
													ptr += scanline_size;
												}
												if( res )
												{													
													/*if( png_ihdr._interlace_method == 1 )
													{
														printf( "%u\n" , (::rux::uint32)readen );
													}*/

													if( image_buffer != &_image_buffer )
													{
														if( fcTL )
														{
															if( _bitmap_info_header._compression == ::rux::media::XEnum_Fourcc_RGB )
															{
																size_t pixel_components = _bitmap_info_header._bit_count / 8;
																if( pixel_components == 4 )
																{
																	::rux::byte* ptr = _image_buffer + ( fcTL->y_offset() * _bitmap_info_header._width * pixel_components ) + fcTL->x_offset() * pixel_components;

																	::rux::uint32 foreground_height = fcTL->height();
																	::rux::uint32 foreground_width = fcTL->width();

																	::rux::byte* foreground_ptr = *image_buffer;
																	::rux::byte* foreground_end = foreground_ptr + ( pixel_components * foreground_height * foreground_width );

																	while( foreground_ptr < foreground_end )
																	{
																		::rux::byte alpha = foreground_ptr[ 3 ];
																		if( alpha == 255 )
																			memcpy( ptr , foreground_ptr , 4 );
																		else
																		{
																			float falpha = (float)alpha / 255.f;
																			for( size_t i = 0 ; i < 3 ; i++ )
																			{
																				::rux::byte bkgd = ptr[ i ];
																				::rux::byte frgd = foreground_ptr[ i ];
																				ptr[ i ] = (::rux::int32)( falpha * frgd + ( 1.f - alpha ) * bkgd );
																			}
																		}
																		foreground_ptr += pixel_components;
																		ptr += pixel_components;
																	}
																}
															}
														}
													}
													_image_buffers.Add( _image_buffer );
												}
											}			
										}
									}
								}
							}
							_stream->seek( img_pos + chunk.length() + 4 );
						}
					}
				}
				if( res == false && _image_buffer )
					::rux::engine::free_mem( _image_buffer );
			}
			return res;
		};
		bool png::to_fourcc( ::rux::io::stream* png , ::rux::media::png& png_file )
		{			
			if( png_file._stream )
			{
				png_file._stream->release();
				png_file._stream = NULL;
				png_file._image_begin_position = 0;
				png_file._frame_index = 0;
				png_file._play_index = 0;
				png_file._animation_stopped = 0;
			}			
			bool res = false;			
			if( png->opened() )
			{
				png->seek( 0 , ::rux::io::XEnum_SetPosition_FromBegin );
				::rux::uint64 signature = 0;
				size_t readen = 0;
				if( png->read_bytes( (char*)&signature , sizeof( ::rux::uint64 ) , readen ) )
				{
					if( readen == sizeof( ::rux::uint64 ) )
					{
						if( signature == ::rux::media::png::_signature )
						{
							::rux::media::png_chunk png_chunk;
							size_t readen = 0;
							if( png->read_bytes( (char*)&png_chunk , sizeof( ::rux::media::png_chunk ) , readen ) )
							{										
								if( readen == sizeof( ::rux::media::png_chunk ) )
								{
									if( png_chunk.is_ihdr() )
									{		
										::rux::media::png_ihdr png_ihdr;
										readen = 0;
										if( png->read_bytes( (char*)&png_ihdr , sizeof( ::rux::media::png_ihdr ) , readen ) )
										{										
											if( readen == sizeof( ::rux::media::png_ihdr ) )
											{
												png_file._bitmap_info_header._width = png_ihdr.width() , png_file._bitmap_info_header._height = png_ihdr.height();
												png_file._bitmap_info_header._size_image = 0;
												if( png_ihdr._color_type == 0 )
												{
													png_file._bitmap_info_header._compression = ::rux::media::XEnum_Fourcc_GREY;
													png_file._bitmap_info_header._bit_count = 8;
													png_file._bitmap_info_header._size_image = png_file._bitmap_info_header._width * png_file._bitmap_info_header._height * png_ihdr._bit_depth / 8;
												}
												else if( png_ihdr._color_type == 2 )
												{
													png_file._bitmap_info_header._compression = ::rux::media::XEnum_Fourcc_RGB;
													png_file._bitmap_info_header._bit_count = 24;
													png_file._bitmap_info_header._size_image = png_file._bitmap_info_header._width * png_file._bitmap_info_header._height * 3 * png_ihdr._bit_depth / 8;
												}
												else if( png_ihdr._color_type == 6 )
												{
													png_file._bitmap_info_header._compression = ::rux::media::XEnum_Fourcc_RGB;
													png_file._bitmap_info_header._bit_count = 32;
													png_file._bitmap_info_header._size_image = png_file._bitmap_info_header._width * png_file._bitmap_info_header._height * 4 * png_ihdr._bit_depth / 8;
												}
												if( png_file._image_buffers.Count() > 0 && png_file._image_buffer_size < png_file._bitmap_info_header._size_image + png_file._bitmap_info_header._width )
												{
													for( size_t index0 = 0 ; index0 < png_file._image_buffers.Count() ; index0++ )
														::rux::engine::free_mem( png_file._image_buffers[ index0 ] );
													png_file._image_buffers.Clear();
													png_file._image_buffer_size = 0;
												}																							
												for( ; ; )
												{
													readen = 0;
													if( png->read_bytes( (char*)&png_chunk , sizeof( ::rux::media::png_chunk ) , readen ) )
													{										
														if( readen == sizeof( ::rux::media::png_chunk ) )
														{
															if( png_chunk.is_pHYs() )
															{
																::rux::media::png_pHYs png_pHYs;
																readen = 0;
																if( png->read_bytes( (char*)&png_pHYs , sizeof( ::rux::media::png_pHYs ) , readen ) )
																{										
																	if( readen == sizeof( ::rux::media::png_pHYs ) )
																	{
																	}
																	else
																		break;
																}
																else
																	break;
															}
															else if( png_chunk.is_bKGD() )
															{
																::rux::byte bkgd[ 6 ] = {0};
																if( png->read_bytes( (char*)bkgd , png_chunk.length() , readen ) )
																{										
																	if( readen == png_chunk.length() )
																	{
																		/*if( readen == 6 )
																		{
																			png_file._bkgd[ 0 ] = bkgd[ 1 ];
																			png_file._bkgd[ 1 ] = bkgd[ 3 ];
																			png_file._bkgd[ 2 ] = bkgd[ 5 ];
																		}*/
																		png->seek( 4 , ::rux::io::XEnum_SetPosition_FromCurrentPosition );
																	}
																	else
																		break;
																}
																else
																	break;
															}
															else if( png_chunk.is_sRGB() )
															{
																png->seek( png_chunk.length() + 4 , ::rux::io::XEnum_SetPosition_FromCurrentPosition );
															}
															else if( png_chunk.is_iCCP() )
															{
																png->seek( png_chunk.length() + 4 , ::rux::io::XEnum_SetPosition_FromCurrentPosition );
															}
															else if( png_chunk.is_cHRM() )
															{
																::rux::media::png_cHRM png_cHRM;
																::rux::uint32 length = png_chunk.length();
																if( length == sizeof( ::rux::media::png_cHRM ) - 5 )
																{
																	readen = 0;
																	if( png->read_bytes( (char*)&png_cHRM , length + 4 , readen ) )
																	{										
																		if( readen == length + 4 )
																			png_cHRM._exists = 1;
																		else
																			break;
																	}
																	else
																		break;
																}
																else
																	break;
															}
															else if( png_chunk.is_IDAT() )
															{
																if( png_file._image_begin_position == 0 )
																	png_file._image_begin_position = png->position() - sizeof( ::rux::media::png_chunk );
																png->seek( png_chunk.length() + 4 , ::rux::io::XEnum_SetPosition_FromCurrentPosition );																
															}
															else if( png_chunk.is_IEND() )
															{
																res = true;
																break;
															}
															else if( png_chunk.is_acTL() )
															{
																readen = 0;
																if( png->read_bytes( (char*)&png_file._png_acTL , sizeof( ::rux::media::png_acTL ) , readen ) )
																{										
																	if( readen == sizeof( ::rux::media::png_acTL ) )
																		png_file._is_animation = 1;
																	else
																		break;
																}
																else
																	break;
															}
															else if( png_chunk.is_fcTL() )
															{
																if( png_file._image_begin_position == 0 )
																	png_file._image_begin_position = png->position() - sizeof( ::rux::media::png_chunk );
																png->seek( png_chunk.length() + 4 , ::rux::io::XEnum_SetPosition_FromCurrentPosition );
															}
															else
																png->seek( png_chunk.length() + 4 , ::rux::io::XEnum_SetPosition_FromCurrentPosition );
														}
													}
													else
														break;
												}
											}
										}											
									}
									//error
								}
								//error
							}
						}
					}
				}
			}
			//error
			if( res == false )
			{
				png_file._bitmap_info_header._width = 0;
				png_file._bitmap_info_header._height = 0;
				png_file._bitmap_info_header._size_image = 0;
				png_file._bitmap_info_header._compression = ::rux::media::XEnum_Fourcc_Unknown;
				png_file._bitmap_info_header._bit_count = 0;
			}
			else
				png_file._stream = png;
			return res;
		};
		bool png::to_fourcc( const char* filename , ::rux::media::png& png_file )
		{
			::rux::io::file* png = alloc_object_2_params_macros( ::rux::io::file , filename , XReadBinary );
			bool res = to_fourcc( png , png_file );
			if( res == false )
				::rux::engine::free_object< ::rux::io::file >( png );
			return res;
		};		
		void png::filter_sub( ::rux::uint8* dest , ::rux::uint8* scanline , size_t scanline_size , size_t pixel_size )
		{
			for( size_t index0 = 0 ; index0 < scanline_size ; index0++ )
			{
				if( index0 >= pixel_size )
					dest[ index0 ] = scanline[ index0 ] + dest[ index0 - pixel_size ];
				else
					dest[ index0 ] = scanline[ index0 ];
			}
		};
		void png::filter_up( ::rux::uint8* dest , ::rux::uint8* scanline , ::rux::uint8* previous_scanline , size_t scanline_size )
		{
			if( previous_scanline )
			{
				for( size_t index0 = 0 ; index0 < scanline_size ; index0++ )
					dest[ index0 ] = scanline[ index0 ] + previous_scanline[ index0 ];
			}
			else
			{
				for( size_t index0 = 0 ; index0 < scanline_size ; index0++ )
					dest[ index0 ] = scanline[ index0 ];
			}
		};
		void png::filter_average( ::rux::uint8* dest , ::rux::uint8* scanline , ::rux::uint8* previous_scanline , size_t scanline_size , size_t pixel_size )
		{
			if( previous_scanline )
			{
				for( size_t index0 = 0 ; index0 < scanline_size ; index0++ )
				{
					if( index0 >= pixel_size )
						dest[ index0 ] = scanline[ index0 ] + floor( ( ( (float)dest[ index0 - pixel_size ] + (float)previous_scanline[ index0 ] ) / 2.f ) );
					else
						dest[ index0 ] = scanline[ index0 ] + floor( ( ( (float)previous_scanline[ index0 ] ) / 2.f ) );
				}
			}
			else
			{	
				for( size_t index0 = 0 ; index0 < scanline_size ; index0++ )
				{
					if( index0 >= pixel_size )
						dest[ index0 ] = scanline[ index0 ] + floor( ( ( (float)dest[ index0 - pixel_size ] ) / 2.f ) );
					else
						dest[ index0 ] = scanline[ index0 ];
				}
			}
		};
		void png::filter_paeth( ::rux::uint8* dest , ::rux::uint8* scanline , ::rux::uint8* previous_scanline , size_t scanline_size , size_t pixel_size )
		{
			if( previous_scanline )
			{				
				for( size_t index0 = 0 ; index0 < scanline_size ; index0++ )
				{
					if( index0 >= pixel_size )
						dest[ index0 ] = scanline[ index0 ] + paeth_predictor( dest[ index0 - pixel_size ] , previous_scanline[ index0 ] , previous_scanline[ index0 - pixel_size ] );
					else
						dest[ index0 ] = scanline[ index0 ] + paeth_predictor( 0 , previous_scanline[ index0 ] , 0 );
				}
			}
			else
			{
				for( size_t index0 = 0 ; index0 < scanline_size ; index0++ )
				{
					if( index0 >= pixel_size )
						dest[ index0 ] = scanline[ index0 ] + paeth_predictor( dest[ index0 - pixel_size ] , 0 , 0 );
					else
						dest[ index0 ] = scanline[ index0 ];
				}
			}
		};
		::rux::uint8 png::paeth_predictor( ::rux::uint8 a , ::rux::uint8 b , ::rux::uint8 c )
		{
			::rux::int32 p = (::rux::int32)a + (::rux::int32)b - (::rux::int32)c;
			::rux::int32 pa = abs( (::rux::int32)p - (::rux::int32)a );
			::rux::int32 pb = abs( (::rux::int32)p - (::rux::int32)b );
			::rux::int32 pc = abs( (::rux::int32)p - (::rux::int32)c );
			if( pa <= pb && pa <= pc )
				return a;
			else if( pb <= pc )
				return b;
			else
				return c;
		};
	};
};