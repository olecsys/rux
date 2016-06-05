#include <xan_jpeg.h>
#include <xan_string.h>
#include <xan_file.h>
void start( XArray< XString >& names , XArray< XString >& values )
{	
	if( names.Count() >= 1 && values.Count() >= 1 )
	{
		if( names[ 0 ] == "--jpeg-decode-test" )
		{
			XString error;
			XFile jpeg = XFile::Open( values[ 0 ] , error );
			if( error.Length() == 0 )
			{
				int64 file_size = jpeg.get_FileSize( error );
				if( error.Length() == 0 )
				{
					size_t jpeg_data_size = 0;
					uint8* jpeg_data = (uint8*)jpeg.ReadBytes( file_size , jpeg_data_size , error );
					if( jpeg_data )
					{
						rux::media::Jpeg::decode( jpeg_data , jpeg_data_size );
						rux::engine::free_mem( jpeg_data );
					}
				}
			}
		}
	}
};
rux_simple_console_initialize( "rux.media.test" , start );