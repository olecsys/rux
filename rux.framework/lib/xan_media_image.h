#ifndef XAN_MEDIA_IMAGE_H
#define XAN_MEDIA_IMAGE_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_includes.h>
#include <xan_gui_defines.h>
namespace rux
{
	namespace media
	{		
		class image
		{
		public:
			virtual ::rux::byte is_animation( void ) = 0;
			virtual ::rux::byte* next_image( ::rux::byte& new_image ) = 0;
			virtual ::rux::media::XBitmapInfoHeader& get_BitmapInfoHeader( void ) = 0;
			virtual void copy_from( ::rux::media::image* src ) = 0;
			void release( void ) const;
			virtual ~image();
		};
	};
};
#endif