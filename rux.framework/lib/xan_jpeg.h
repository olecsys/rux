#ifndef XAN_JPEG_H
#define XAN_JPEG_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_defines.h>
namespace rux
{
	namespace media
	{
		class Jpeg
		{
		public:
			static uint8 decode( uint8* jpeg_data , size_t jpeg_data_size );
		};
	};
};
#endif