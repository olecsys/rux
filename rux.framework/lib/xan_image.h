#ifndef XAN_IMAGE_H
#define XAN_IMAGE_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_string.h>
#include <xan_gui_defines.h>
#include <xan_media_image.h>
class Frame;
namespace rux
{	
	namespace media
	{		
		class XFrame;
		begin_declare_rux_class( Frame );
			~Frame();	
			::rux::uint8* get_ImageData( ::rux::byte& new_image ) const;
			Frame& ConvertToRGB32( uint8 alpha , rux::XString& error );
			void set_ImageData( const rux::XString& filename , rux::XString& error );
			void set_ImageData( char* image_data , rux::uint32 image_width , rux::uint32 image_height , rux::media::XEnum_Fourcc fourcc , rux::XString& error );	
			Frame& ConvertToBGR24( ::rux::XString& error );
			void ConvertToRGB24( XFrame& destination , ::rux::XString& error );
			Frame& ConvertToBGR32( ::rux::uint8 alpha , ::rux::XString& error );
			void Resize( XFrame& destination , ::rux::uint32 new_image_width , ::rux::uint32 new_image_height , ::rux::XString& error );
			void SaveAsBmp( const ::rux::XString& filename , ::rux::XString& error );
			void ConvertToBmpData( ::rux::uint8*& destination_data_ptr , uint64& destination_data_ptr_size , uint64& size , ::rux::XString& error );
			void SaveToFile( const ::rux::XString& filename_without_extension , ::rux::XString& error );
			void set_EmptyData( void );
			void set_Opacity( float opacity , ::rux::XString& error );
			void ReplaceNotTransparentColors( ::rux::gui::Color* base_replace_color , ::rux::XString& error );
			Frame& ConvertToRGB24( ::rux::XString& error );
			void set_ImageData( char* image_data , ::rux::uint32 image_width , ::rux::uint32 image_height , ::rux::media::XEnum_Fourcc fourcc , uint16 bits_count , ::rux::byte copy = 1 );
			Frame& Resize( ::rux::uint32 new_image_width , ::rux::uint32 new_image_height , ::rux::XString& error );
			::rux::uint8* ConvertToBmpData( uint64& size , ::rux::XString& error );
			void ConvertToBGR24( XFrame& destination , ::rux::XString& error );
			void set_ImageData( char* image_data , ::rux::uint32 image_width , ::rux::uint32 image_height , ::rux::media::XEnum_Fourcc fourcc );
			void ConvertToRGB32( XFrame& destination , ::rux::uint8 alpha , ::rux::XString& error );
			void set_ImageData( char* image_data , ::rux::uint32 image_width , ::rux::uint32 image_height , ::rux::media::XEnum_Fourcc fourcc , uint16 bits_count , ::rux::XString& error );
			rux::uint32 get_ImageSize( void );
			void CopyFrom( ::rux::media::XFrame& frame );
			void set_ImageData( char* image_data , size_t image_data_size , ::rux::XString& error );
			uint16 get_BitCount( void );
			void LockData( void );
			void UnLockData( void );
			::rux::int32 get_Fourcc( void ) const;
			rux::uint32 get_ImageHeight( void ) const;
			rux::uint32 get_ImageWidth( void ) const;
		begin_declare_rux_class_members( Frame );
			::rux::threading::RdWrLock _cs_image_data;
			uint8* _image_data;
			int32 _image_data_size;	
			::rux::media::XBitmapInfoHeader _bitmap_info_header;
			::rux::media::image* _media_image;
			void check_size( int32 size );
		end_declare_rux_class_members( Frame );
			void set_ImageData( char* image_data , rux::uint32 image_width , rux::uint32 image_height , rux::media::XEnum_Fourcc fourcc , rux::XString& error );
			void set_ImageData( char* image_data , rux::uint32 image_width , rux::uint32 image_height , rux::media::XEnum_Fourcc fourcc );
			void set_ImageData( char* image_data , rux::uint32 image_width , rux::uint32 image_height , rux::media::XEnum_Fourcc fourcc , uint16 bits_count , rux::XString& error );
			void set_ImageData( char* image_data , rux::uint32 image_width , rux::uint32 image_height , rux::media::XEnum_Fourcc fourcc , uint16 bits_count , ::rux::byte copy = 1 );
			void set_ImageData( const rux::XString& filename , rux::XString& error );
			void set_ImageData( char* image_data , size_t image_data_size , ::rux::XString& error );
			Frame& ConvertToRGB24( rux::XString& error );
			Frame& ConvertToBGR24( rux::XString& error ) const;
			Frame& ConvertToRGB32( uint8 alpha , rux::XString& error );
			Frame& ConvertToBGR32( uint8 alpha , rux::XString& error ) const;
			void ConvertToRGB24( XFrame& destination , rux::XString& error );
			void ConvertToBGR24( XFrame& destination , rux::XString& error );
			void ConvertToRGB32( XFrame& destination , uint8 alpha , rux::XString& error );
			static void ConvertToRGB24( uint8* image_data , rux::uint32 image_width , rux::uint32 image_height , rux::uint32 image_data_size , int16 bit_count , int32 compression , XFrame& destination , rux::XString& error );
			static void ConvertToBGR24( uint8* image_data , rux::uint32 image_width , rux::uint32 image_height , rux::uint32 image_data_size , int16 bit_count , int32 compression , XFrame& destination , rux::XString& error );
			static void ConvertToRGB32( uint8* image_data , rux::uint32 image_width , rux::uint32 image_height , rux::uint32 image_data_size , int16 bit_count , int32 compression , XFrame& destination , uint8 alpha , rux::XString& error );

			static void ConvertToRGB24( uint8* image_data , rux::uint32 image_width , rux::uint32 image_height , rux::uint32 image_data_size , int16 bit_count , int32 compression , uint8*& destination , int32& destination_size , rux::XString& error );
			static void ConvertToBGR24( uint8* image_data , rux::uint32 image_width , rux::uint32 image_height , rux::uint32 image_data_size , int16 bit_count , int32 compression , uint8*& destination , int32& destination_size , rux::XString& error );
			static void ConvertToRGB32( uint8* image_data , rux::uint32 image_width , rux::uint32 image_height , rux::uint32 image_data_size , int16 bit_count , int32 compression , uint8*& destination , int32& destination_size , uint8 alpha , rux::XString& error );

			uint8* get_ImageData( ::rux::byte& new_image ) const;
			rux::uint32 get_ImageSize( void ) const;
			rux::uint32 get_ImageWidth( void ) const;
			rux::uint32 get_ImageHeight( void ) const;
			Frame& Resize( rux::uint32 new_image_width , rux::uint32 new_image_height , rux::XString& error );
			void Resize( XFrame& destination , rux::uint32 new_image_width , rux::uint32 new_image_height , rux::XString& error );
			void SaveAsBmp( const rux::XString& filename , rux::XString& error ) const;
			void SaveToFile( const rux::XString& filename_without_extension , rux::XString& error ) const;
			uint8* ConvertToBmpData( uint64& size , rux::XString& error );
			void ConvertToBmpData( uint8*& destination_data_ptr , uint64& destination_data_ptr_size , uint64& size , rux::XString& error );
			int32 get_Fourcc( void ) const;
			uint16 get_BitCount( void );
			void LockData( void );
			void UnLockData( void );
			void set_EmptyData( void );
			static rux::uint32 get_ImageSize( int32 fourcc , rux::uint32 width , rux::uint32 height , int16 bit_count );
			void ReplaceNotTransparentColors( rux::gui::Color* base_replace_color , rux::XString& error );
			void set_Opacity( float opacity , rux::XString& error );
			static ::rux::byte Exists( const rux::XString& filename );
			void CopyFrom( ::rux::media::XFrame& frame );
		end_declare_rux_class();
	};
};
#endif
