#ifndef XAN_PNG_H
#define XAN_PNG_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_defines.h>
#include <xan_stream.h>
#include <xan_media_image.h>
#include <xan_gui_defines.h>
namespace rux
{
	namespace media
	{
begin_struct_pack( 1 )
		struct png_ihdr
		{
			::rux::uint8 _width[ 4 ];
			::rux::uint8  _height[ 4 ];
			::rux::uint8 _bit_depth;
			::rux::uint8 _color_type;
			::rux::uint8 _compression_method;
			::rux::uint8 _filter_method;
			::rux::uint8 _interlace_method;
			::rux::uint32 _crc;
			::rux::uint32 width( void );
			::rux::uint32 height( void );
		}
end_struct_pack( 1 )
begin_struct_pack( 1 )
		struct png_fcTL
		{
			::rux::uint8 _sequence_number[ 4 ];
			::rux::uint8 _width[ 4 ];
			::rux::uint8 _height[ 4 ];
			::rux::uint8 _x_offset[ 4 ];
			::rux::uint8 _y_offset[ 4 ];
			::rux::uint8 _delay_num[ 2 ];
			::rux::uint8 _delay_den[ 2 ];
			::rux::uint8 _dispose_op;
			::rux::uint8 _blend_op;
			::rux::uint32 _crc;
			::rux::uint32 sequence_number( void );
			::rux::uint32 width( void );
			::rux::uint32 height( void );
			::rux::uint32 x_offset( void );
			::rux::uint32 y_offset( void );
			::rux::int16 delay_num( void );
			::rux::int16 delay_den( void );
		  }
end_struct_pack( 1 )
begin_struct_pack( 1 )
		struct png_acTL
		{
			::rux::uint8 _num_frames[ 4 ];
			::rux::uint8 _num_plays[ 4 ];
			::rux::uint32 _crc;
			::rux::uint32 num_frames( void );
			::rux::uint32 num_plays( void );
		}
end_struct_pack( 1 )
begin_struct_pack( 1 )
		struct png_idat
		{
			

			::rux::uint32 _crc;
		}
end_struct_pack( 1 )	
begin_struct_pack( 1 )
		struct png_pHYs
		{
			::rux::uint32 _pixels_per_unit_x_axis;
			::rux::uint32 _pixels_per_unit_y_axis;
			::rux::uint8 _unit_specifier;
			::rux::uint32 _crc;
		}
end_struct_pack( 1 )
begin_struct_pack( 1 )
		struct png_cHRM
		{
			::rux::uint8 _white_point_x[ 4 ];
			::rux::uint8 _white_point_y[ 4 ];
			::rux::uint8 _red_x[ 4 ];
			::rux::uint8 _red_y[ 4 ];
			::rux::uint8 _green_x[ 4 ];
			::rux::uint8 _green_y[ 4 ];
			::rux::uint8 _blue_x[ 4 ];
			::rux::uint8 _blue_y[ 4 ];
			::rux::uint32 _crc;
			::rux::uint8 _exists;
			png_cHRM();
			::rux::uint32 white_point_x();
			::rux::uint32 white_point_y();
			::rux::uint32 red_x();
			::rux::uint32 red_y();
			::rux::uint32 green_x();
			::rux::uint32 green_y();
			::rux::uint32 blue_x();
			::rux::uint32 blue_y();
		}
end_struct_pack( 1 )
begin_struct_pack( 1 )
		struct png_chunk
		{
			::rux::uint8 _length[ 4 ];
			::rux::uint32  _chunk_type;
			bool is_ihdr( void );
			bool is_pHYs( void );
			bool is_iCCP( void );
			bool is_cHRM( void );
			bool is_IDAT( void );
			bool is_IEND( void );
			bool is_acTL( void );
			bool is_fcTL( void );
			bool is_bKGD( void );
			bool is_sRGB( void );
			bool is_fdAT( void );
			::rux::uint32 length( void );
		}
end_struct_pack( 1 )	
		class png : public ::rux::media::image
		{		
			::rux::byte _dispose_op;
			png_acTL _png_acTL;
			::rux::byte _animation_stopped;
			::rux::byte _is_animation;
			::rux::media::XBitmapInfoHeader _bitmap_info_header;
			::rux::io::stream* _stream;
			::rux::uint64 _image_begin_position;
			::rux::uint32 _next_image_last_time;
			size_t _frame_index;
			size_t _play_index;
			XMallocArray< ::rux::byte* > _image_buffers;
			XMallocArray< ::rux::uint32 > _image_buffers_delay;
			size_t _image_buffer_size;
			::rux::byte* _image_region_buffer;
			size_t _image_region_buffer_size;
			//::rux::byte _bkgd[ 3 ];
		public:
			png( const char* filename );
			png( ::rux::io::stream* stream );
			~png( void );
			virtual ::rux::byte is_animation( void );
			virtual ::rux::byte* next_image( ::rux::byte& new_image );
			virtual ::rux::media::XBitmapInfoHeader& get_BitmapInfoHeader( void );
			virtual void copy_from( ::rux::media::image* src );
			bool to_fourcc( const char* filename );
			bool get_image( png_chunk& chunk , png_fcTL* fcTL , ::rux::byte is_fdAT );
			static bool to_fourcc( const char* filename , ::rux::media::png& png );
			static bool to_fourcc( ::rux::io::stream* stream , ::rux::media::png& png );

			static const ::rux::uint64 _signature = 727905341920923785;			
			static void filter_sub( ::rux::uint8* dest , ::rux::uint8* scanline , size_t scanline_size , size_t pixel_size );
			static void filter_up( ::rux::uint8* dest , ::rux::uint8* scanline , ::rux::uint8* previous_scanline , size_t scanline_size );
			static void filter_average( ::rux::uint8* dest , ::rux::uint8* scanline , ::rux::uint8* previous_scanline , size_t scanline_size , size_t pixel_size );
			static void filter_paeth( ::rux::uint8* dest , ::rux::uint8* scanline , ::rux::uint8* previous_scanline , size_t scanline_size , size_t pixel_size );
			static ::rux::uint8 paeth_predictor( ::rux::uint8 a , ::rux::uint8 b , ::rux::uint8 c );
		};
	};
};
#endif
