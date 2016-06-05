#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_media_image.h>
#include <xan_memorymanager_defines.h>
namespace rux
{
	namespace media
	{
		image::~image()
		{
		};
		void image::release( void ) const
		{
			::rux::engine::free_object< ::rux::media::image >( const_cast< ::rux::media::image* >( this ) );
		};
	};
};