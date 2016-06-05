#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_jpeg.h>
namespace rux
{
	namespace media
	{		
		const uint8 _jpeg_SOI[] =
		{ 
			0xFF, 0xD8
		};//none 	Start Of Image 	
		const uint8 _jpeg_SOF0[] =
		{
			0xFF, 0xC0 	
		};//variable size 	Start Of Frame (Baseline DCT) 	Indicates that this is a baseline DCT-based JPEG, and specifies the width, height, number of components, and component subsampling (e.g., 4:2:0).
		const uint8 _jpeg_SOF2[] =
		{ 	
			0xFF, 0xC2
		};//variable size 	Start Of Frame (Progressive DCT) 	Indicates that this is a progressive DCT-based JPEG, and specifies the width, height, number of components, and component subsampling (e.g., 4:2:0).
		const uint8 _jpeg_DHT[] =
		{ 	
			0xFF, 0xC4
		};// 	variable size 	Define Huffman Table(s) 	Specifies one or more Huffman tables.
		const uint8 _jpeg_DQT[] =
		{ 	
			0xFF, 0xDB
		};// 	variable size 	Define Quantization Table(s) 	Specifies one or more quantization tables.
		const uint8 _jpeg_DRI[] =
		{ 	
			0xFF, 0xDD
		};// 	2 bytes 	Define Restart Interval 	Specifies the interval between RSTn markers, in macroblocks. This marker is followed by two bytes indicating the fixed size so it can be treated like any other variable size segment.
		const uint8 _jpeg_SOS[] =
		{ 	
			0xFF, 0xDA
		};//variable size 	Start Of Scan 	Begins a top-to-bottom scan of the image. In baseline DCT JPEG images, there is generally a single scan. Progressive DCT JPEG images usually contain multiple scans. This marker specifies which slice of data it will contain, and is immediately followed by entropy-coded data.
		const uint8 _jpeg_RST0[] =
		{ 	
			0xFF, 0xD0
		};
		const uint8 _jpeg_RST1[] =
		{ 	
			0xFF, 0xD1
		};
		const uint8 _jpeg_RST2[] =
		{ 	
			0xFF, 0xD2
		};
		const uint8 _jpeg_RST3[] =
		{ 	
			0xFF, 0xD3
		};
		const uint8 _jpeg_RST4[] =
		{ 	
			0xFF, 0xD4
		};
		const uint8 _jpeg_RST5[] =
		{ 	
			0xFF, 0xD5
		};
		const uint8 _jpeg_RST6[] =
		{ 	
			0xFF, 0xD6
		};
		const uint8 _jpeg_RST7[] =
		{ 	
			0xFF, 0xD7
		};
		//none 	Restart 	Inserted every r macroblocks, where r is the restart interval set by a DRI marker. Not used if there was no DRI marker. The low 3 bits of the marker code cycle in value from 0 to 7.
		const uint8 _jpeg_APP0[] =
		{ 	
			0xFF, 0xE0
		};
		const uint8 _jpeg_APP1[] =
		{ 	
			0xFF, 0xE1
		};
		const uint8 _jpeg_APP2[] =
		{ 	
			0xFF, 0xE2
		};
		const uint8 _jpeg_APP3[] =
		{ 	
			0xFF, 0xE3
		};
		const uint8 _jpeg_APP4[] =
		{ 	
			0xFF, 0xE4
		};
		const uint8 _jpeg_APP5[] =
		{ 	
			0xFF, 0xE5
		};
		const uint8 _jpeg_APP6[] =
		{ 	
			0xFF, 0xE6
		};
		const uint8 _jpeg_APP7[] =
		{ 	
			0xFF, 0xE7
		};
		const uint8 _jpeg_APP8[] =
		{ 	
			0xFF, 0xE8
		};
		const uint8 _jpeg_APP9[] =
		{ 	
			0xFF, 0xE9
		};
		const uint8 _jpeg_APPA[] =
		{ 	
			0xFF, 0xEA
		};
		const uint8 _jpeg_APPB[] =
		{ 	
			0xFF, 0xEB
		};
		const uint8 _jpeg_APPC[] =
		{ 	
			0xFF, 0xEC
		};
		const uint8 _jpeg_APPD[] =
		{ 	
			0xFF, 0xED
		};
		const uint8 _jpeg_APPE[] =
		{ 	
			0xFF, 0xEE
		};
		const uint8 _jpeg_APPF[] =
		{ 	
			0xFF, 0xEF
		};
		//variable size 	Application-specific 	For example, an Exif JPEG file uses an APP1 marker to store metadata, laid out in a structure based closely on TIFF.*/
		const uint8 _jpeg_COM[] =
		{ 	
			0xFF, 0xFE
		};// 	variable size 	Comment 	Contains a text comment.
		const uint8 _jpeg_EOI[] =
		{ 	
			0xFF, 0xD9 	
		};//none 	End Of Image
		void revert_memcpy( void* dst , const void* src , size_t size )
		{
			uint8* src_ptr = ( (uint8*)src ) + size , * dst_ptr = (uint8*)dst;
			while( src_ptr != src )
			{
				src_ptr--;
				*dst_ptr = *src_ptr;
				dst_ptr++;
			}
		};
		uint8 Jpeg::decode( uint8* jpeg_data , size_t jpeg_data_size )
		{			
			uint8 res = 0;
			uint8* ptr = jpeg_data , * end = jpeg_data + jpeg_data_size;
			if( memcmp( jpeg_data , _jpeg_SOI , 2 ) == 0 )
			{				
				ptr += 2;
				uint8 dc_codes_count_index = 0 , ac_codes_count_index = 0;
				uint8* dc_codes_count[ 8 ] = {0} , * ac_codes_count[ 8 ] = {0};
				rux::uint32 dc_codes_values_count[ 8 ] = {0} , ac_codes_values_count[ 8 ] = {0};
				uint8 dc_codes_table_id[ 8 ] = {0} , ac_codes_table_id[ 8 ] = {0};
				while( ptr < end )
				{
					if( memcmp( ptr , _jpeg_COM , 2 ) == 0 )
					{
						ptr += 2;
						uint16 com_size = 0;
						revert_memcpy( &com_size , ptr , 2 );


						ptr += com_size;
					}
					else if( memcmp( ptr , _jpeg_EOI , 2 ) == 0 )
						break;
					else if( memcmp( ptr , _jpeg_SOF0 , 2 ) == 0 )
					{
						ptr += 2;
						uint16 sof0_size = 0;
						revert_memcpy( &sof0_size , ptr , 2 );
						ptr += 2;
						uint8 precision = *ptr;
						ptr++; 
						uint16 width = 0;
						revert_memcpy( &width , ptr , 2 );
						ptr += 2;
						uint16 height = 0;
						revert_memcpy( &height , ptr , 2 );
						ptr += 2;
						uint8 components_count = *ptr;
						ptr++;
						uint8 components_index = 0;
						while( components_index < components_count )
						{
							uint8 id = *ptr;
							ptr++;
							uint8 h = ( *ptr & 0xf0 ) >> 4;
							uint8 v = *ptr & 0x0f;
							ptr++;
							uint8 dqt_index = *ptr;
							ptr++;
							components_index++;
						}
					}
					else if( memcmp( ptr , _jpeg_SOF2 , 2 ) == 0 )
					{
						ptr += 2;
						uint16 sof2_size = 0;
						revert_memcpy( &sof2_size , ptr , 2 );

						ptr += sof2_size;
					}
					else if( memcmp( ptr , _jpeg_DHT , 2 ) == 0 )
					{
						ptr += 2;
						uint16 dht_size = 0;
						revert_memcpy( &dht_size , ptr , 2 );
						ptr += 2;
						uint8 class_id = ( *ptr & 0xf0 ) >> 4;						
						if( class_id == 0 )
						{
							dc_codes_table_id[ dc_codes_count_index ] = *ptr & 0x0f;
							ptr++;
							dc_codes_count[ dc_codes_count_index ] = ptr;
							dc_codes_values_count[ dc_codes_count_index ] = dht_size - 19;
							ptr += dc_codes_values_count[ dc_codes_count_index ];
							dc_codes_count_index++;							
						}
						else if( class_id == 1 )
						{
							ac_codes_table_id[ ac_codes_count_index ] = *ptr & 0x0f;
							ptr++;
							ac_codes_count[ ac_codes_count_index ] = ptr;
							ac_codes_values_count[ ac_codes_count_index ] = dht_size - 19;
							ptr += ac_codes_values_count[ ac_codes_count_index ];
							ac_codes_count_index++;
						}
						ptr += 16;
					}
					else if( memcmp( ptr , _jpeg_DQT , 2 ) == 0 )
					{
						ptr += 2;
						uint16 dqt_size = 0;
						revert_memcpy( &dqt_size , ptr , 2 );
						ptr += 2;
						uint8 precision = ( *ptr & 0xf0 ) == 0x10 ? 2 : 1;
						uint8 id = *ptr & 0x0f;
						ptr++;
						ptr += dqt_size - 3;
					}
					else if( memcmp( ptr , _jpeg_DRI , 2 ) == 0 )
					{
						ptr += 2;
						uint16 dri_size = 0;
						revert_memcpy( &dri_size , ptr , 2 );

						ptr += dri_size;
					}
					else if( memcmp( ptr , _jpeg_SOS , 2 ) == 0 )
					{
						ptr += 2;
						uint16 sos_size = 0;
						revert_memcpy( &sos_size , ptr , 2 );
						ptr += 2;
						uint8 components_count = *ptr;
						ptr++;
						uint8 components_index = 0;
						while( components_index < components_count )
						{
							uint8 component_id = *ptr;
							ptr++;
							uint8 dc_table_id = ( *ptr & 0xf0 ) >> 4;
							uint8 ac_table_id = *ptr & 0x0f;
							ptr++;
							components_index++;
						}
						ptr += 3;	
						size_t huffman_index = 0;
						uint8 dc = 1;
						uint8 dc_or_ac_index = 0;
						while( memcmp( ptr , _jpeg_EOI , 2 ) != 0 )
						{
							size_t bit_index = 0;
							while( bit_index < 8 )
							{
								if( rux_get_bit( ptr , bit_index ) == 1 )
								{
									uint8* values = dc_codes_count[ dc_or_ac_index ] + 16;

									//uint8 
									//dc_codes_values_count[ dc_or_ac_index ][ huffman_index ]

								}
								else
								{
								}
								bit_index++;
							}
							ptr++;
						}
						int32 u = 0;
						//ptr += sos_size;
					}
					else if( memcmp( ptr , _jpeg_RST0 , 2 ) == 0 )
					{
						ptr += 2;
						uint16 rst_size = 0;
						revert_memcpy( &rst_size , ptr , 2 );

						ptr += rst_size;
					}
					else if( memcmp( ptr , _jpeg_RST1 , 2 ) == 0 )
					{
						ptr += 2;
						uint16 rst_size = 0;
						revert_memcpy( &rst_size , ptr , 2 );

						ptr += rst_size;
					}
					else if( memcmp( ptr , _jpeg_RST2 , 2 ) == 0 )
					{
						ptr += 2;
						uint16 rst_size = 0;
						revert_memcpy( &rst_size , ptr , 2 );

						ptr += rst_size;
					}
					else if( memcmp( ptr , _jpeg_RST3 , 2 ) == 0 )
					{
						ptr += 2;
						uint16 rst_size = 0;
						revert_memcpy( &rst_size , ptr , 2 );

						ptr += rst_size;
					}
					else if( memcmp( ptr , _jpeg_RST4 , 2 ) == 0 )
					{
						ptr += 2;
						uint16 rst_size = 0;
						revert_memcpy( &rst_size , ptr , 2 );

						ptr += rst_size;
					}
					else if( memcmp( ptr , _jpeg_RST5 , 2 ) == 0 )
					{
						ptr += 2;
						uint16 rst_size = 0;
						revert_memcpy( &rst_size , ptr , 2 );

						ptr += rst_size;
					}
					else if( memcmp( ptr , _jpeg_RST6 , 2 ) == 0 )
					{
						ptr += 2;
						uint16 rst_size = 0;
						revert_memcpy( &rst_size , ptr , 2 );

						ptr += rst_size;
					}
					else if( memcmp( ptr , _jpeg_RST7 , 2 ) == 0 )
					{
						ptr += 2;
						uint16 rst_size = 0;
						revert_memcpy( &rst_size , ptr , 2 );

						ptr += rst_size;
					}
					else if( memcmp( ptr , _jpeg_APP0 , 2 ) == 0 )
					{
						ptr += 2;
						uint16 app_size = 0;
						revert_memcpy( &app_size , ptr , 2 );
						ptr += 2;						
						if( memcmp( ptr , "JFIF" , 5 ) == 0 )
						{
							ptr += 5;
							uint8 major = *ptr;
							ptr++;
							uint8 minor = *ptr;
							ptr++;
							uint8 point_unit = *ptr;
							ptr++;
							uint16 Xdensity = 0;
							revert_memcpy( &Xdensity , ptr , 2 );
							ptr += 2;
							uint16 Ydensity = 0;
							revert_memcpy( &Ydensity , ptr , 2 );
							ptr += 2;
							uint8 Xthumbnail = *ptr;
							ptr++;
							uint8 Ythumbnail = *ptr;
							ptr++;
						}
						else
							ptr += app_size - 2;
					}
					else if( memcmp( ptr , _jpeg_APP1 , 2 ) == 0 )
					{
						ptr += 2;
						uint16 app_size = *(uint16*)ptr;

						ptr += app_size;
					}
					else if( memcmp( ptr , _jpeg_APP2 , 2 ) == 0 )
					{
						ptr += 2;
						uint16 app_size = 0;
						revert_memcpy( &app_size , ptr , 2 );

						ptr += app_size;
					}
					else if( memcmp( ptr , _jpeg_APP3 , 2 ) == 0 )
					{
						ptr += 2;
						uint16 app_size = 0;
						revert_memcpy( &app_size , ptr , 2 );

						ptr += app_size;
					}
					else if( memcmp( ptr , _jpeg_APP4 , 2 ) == 0 )
					{
						ptr += 2;
						uint16 app_size = 0;
						revert_memcpy( &app_size , ptr , 2 );

						ptr += app_size;
					}
					else if( memcmp( ptr , _jpeg_APP5 , 2 ) == 0 )
					{
						ptr += 2;
						uint16 app_size = 0;
						revert_memcpy( &app_size , ptr , 2 );

						ptr += app_size;
					}
					else if( memcmp( ptr , _jpeg_APP6 , 2 ) == 0 )
					{
						ptr += 2;
						uint16 app_size = 0;
						revert_memcpy( &app_size , ptr , 2 );

						ptr += app_size;
					}
					else if( memcmp( ptr , _jpeg_APP7 , 2 ) == 0 )
					{
						ptr += 2;
						uint16 app_size = 0;
						revert_memcpy( &app_size , ptr , 2 );

						ptr += app_size;
					}
					else if( memcmp( ptr , _jpeg_APP8 , 2 ) == 0 )
					{
						ptr += 2;
						uint16 app_size = 0;
						revert_memcpy( &app_size , ptr , 2 );

						ptr += app_size;
					}
					else if( memcmp( ptr , _jpeg_APP9 , 2 ) == 0 )
					{
						ptr += 2;
						uint16 app_size = 0;
						revert_memcpy( &app_size , ptr , 2 );

						ptr += app_size;
					}
					else if( memcmp( ptr , _jpeg_APPA , 2 ) == 0 )
					{
						ptr += 2;
						uint16 app_size = 0;
						revert_memcpy( &app_size , ptr , 2 );

						ptr += app_size;
					}
					else if( memcmp( ptr , _jpeg_APPB , 2 ) == 0 )
					{
						ptr += 2;
						uint16 app_size = 0;
						revert_memcpy( &app_size , ptr , 2 );

						ptr += app_size;
					}
					else if( memcmp( ptr , _jpeg_APPC , 2 ) == 0 )
					{
						ptr += 2;
						uint16 app_size = 0;
						revert_memcpy( &app_size , ptr , 2 );

						ptr += app_size;
					}
					else if( memcmp( ptr , _jpeg_APPD , 2 ) == 0 )
					{
						ptr += 2;
						uint16 app_size = 0;
						revert_memcpy( &app_size , ptr , 2 );

						ptr += app_size;
					}
					else if( memcmp( ptr , _jpeg_APPE , 2 ) == 0 )
					{
						ptr += 2;
						uint16 app_size = 0;
						revert_memcpy( &app_size , ptr , 2 );

						ptr += app_size;
					}
					else if( memcmp( ptr , _jpeg_APPF , 2 ) == 0 )
					{
						ptr += 2;
						uint16 app_size = 0;
						revert_memcpy( &app_size , ptr , 2 );

						ptr += app_size;
					}

					
				}
			}
			return res;
		};
	};
};