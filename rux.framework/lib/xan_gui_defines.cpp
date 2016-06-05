#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_gui_defines.h>
#include <xan_error.h>
namespace rux
{
	namespace media
	{
#define clip( x ) x > 0 ? x < 255 ? x : 255 : 0
		dll_internal void rux_BGR24_to_RGB24( const ::rux::uint8* BGR24_data_ptr , ::rux::uint32 BGR24_data_width , ::rux::uint32 BGR24_data_height , ::rux::uint8*& RGB24_data_ptr , ::rux::int32& RGB24_data_size )
		{
			if( BGR24_data_ptr )
			{
				if( BGR24_data_width > 0 )
				{
					if( BGR24_data_height > 0 )
					{					
						::rux::uint32 data_size = BGR24_data_width * 3 * BGR24_data_height;
						if( RGB24_data_ptr && (::rux::uint32)RGB24_data_size < data_size )
						{
							::rux::engine::free_mem( RGB24_data_ptr );
							RGB24_data_ptr = NULL;
						}
						if( RGB24_data_ptr == NULL )
						{
							RGB24_data_ptr = alloc_array_macros( ::rux::uint8 , data_size );
							RGB24_data_size = data_size;
						}
						for( ::rux::uint32 index0 = 0 ; index0 < data_size ; index0 += 3 )
						{
							RGB24_data_ptr[ index0 ] = BGR24_data_ptr[ index0 + 2 ]; 
							RGB24_data_ptr[ index0 + 1 ] = BGR24_data_ptr[ index0 + 1 ]; 
							RGB24_data_ptr[ index0 + 2 ] = BGR24_data_ptr[ index0 ]; 
						}
					}
				}
			}
		};
		dll_internal void rux_BGR32_to_RGB24( const ::rux::uint8* BGR24_data_ptr , ::rux::uint32 BGR24_data_width , ::rux::uint32 BGR24_data_height , ::rux::uint8*& RGB24_data_ptr , ::rux::int32& RGB24_data_size )
		{
			if( BGR24_data_ptr )
			{
				if( BGR24_data_width > 0 )
				{
					if( BGR24_data_height > 0 )
					{					
						::rux::uint32 data_size = BGR24_data_width * 3 * BGR24_data_height;
						if( RGB24_data_ptr && (::rux::uint32)RGB24_data_size < data_size )
						{
							::rux::engine::free_mem( RGB24_data_ptr );
							RGB24_data_ptr = NULL;
						}
						if( RGB24_data_ptr == NULL )
						{
							RGB24_data_ptr = alloc_array_macros( ::rux::uint8 , data_size );
							RGB24_data_size = data_size;
						}
						size_t index1 = 0;
						for( ::rux::uint32 index0 = 0 ; index0 < data_size ; index0 += 3 )
						{
							RGB24_data_ptr[ index0 + 2 ] = BGR24_data_ptr[ index1++ ]; 
							RGB24_data_ptr[ index0 + 1 ] = BGR24_data_ptr[ index1++ ]; 							
							RGB24_data_ptr[ index0 ] = BGR24_data_ptr[ index1++ ];							
							index1++;
						}
					}
				}
			}
		};
		dll_internal void rux_RGB24_to_BGR24( const ::rux::uint8* RGB24_data_ptr , ::rux::uint32 RGB24_data_width , ::rux::uint32 RGB24_data_height , ::rux::uint8*& BGR24_data_ptr , ::rux::int32& BGR24_data_size )
		{
			if( RGB24_data_ptr )
			{
				if( RGB24_data_width > 0 )
				{
					if( RGB24_data_height > 0 )
					{					
						::rux::uint32 data_size = RGB24_data_width * 3 * RGB24_data_height;
						if( BGR24_data_ptr && (::rux::uint32)BGR24_data_size < data_size )
						{
							::rux::engine::free_mem( BGR24_data_ptr );
							BGR24_data_ptr = NULL;
						}
						if( BGR24_data_ptr == NULL )
						{
							BGR24_data_ptr = alloc_array_macros( ::rux::uint8 , data_size );
							BGR24_data_size = data_size;
						}
						for( ::rux::uint32 index0 = 0 ; index0 < data_size ; index0 += 3 )
						{
							BGR24_data_ptr[ index0 ] = RGB24_data_ptr[ index0 + 2 ]; 
							BGR24_data_ptr[ index0 + 1 ] = RGB24_data_ptr[ index0 + 1 ]; 
							BGR24_data_ptr[ index0 + 2 ] = RGB24_data_ptr[ index0 ]; 
						}
					}
				}
			}
		};
		dll_internal void rux_RGB32_to_BGR32( const ::rux::uint8* RGB24_data_ptr , ::rux::uint32 RGB24_data_width , ::rux::uint32 RGB24_data_height , ::rux::uint8*& BGR24_data_ptr , ::rux::int32& BGR24_data_size )
		{
			if( RGB24_data_ptr )
			{
				if( RGB24_data_width > 0 )
				{
					if( RGB24_data_height > 0 )
					{					
						::rux::uint32 data_size = RGB24_data_width * 4 * RGB24_data_height;
						if( BGR24_data_ptr && (::rux::uint32)BGR24_data_size < data_size )
						{
							::rux::engine::free_mem( BGR24_data_ptr );
							BGR24_data_ptr = NULL;
						}
						if( BGR24_data_ptr == NULL )
						{
							BGR24_data_ptr = alloc_array_macros( ::rux::uint8 , data_size );
							BGR24_data_size = data_size;
						}
						for( ::rux::uint32 index0 = 0 ; index0 < data_size ; index0 += 4 )
						{
							BGR24_data_ptr[ index0 ] = RGB24_data_ptr[ index0 + 2 ]; 
							BGR24_data_ptr[ index0 + 1 ] = RGB24_data_ptr[ index0 + 1 ]; 
							BGR24_data_ptr[ index0 + 2 ] = RGB24_data_ptr[ index0 ]; 
							BGR24_data_ptr[ index0 + 3 ] = RGB24_data_ptr[ index0 + 3 ]; 
						}
					}
				}
			}
		};
		dll_internal void rux_RGB24_to_BGR32( const ::rux::uint8* RGB24_data_ptr , ::rux::uint32 RGB24_data_width , ::rux::uint32 RGB24_data_height , ::rux::uint8*& BGR24_data_ptr , ::rux::int32& BGR24_data_size )
		{
			if( RGB24_data_ptr )
			{
				if( RGB24_data_width > 0 )
				{
					if( RGB24_data_height > 0 )
					{					
						::rux::uint32 data_size = RGB24_data_width * 4 * RGB24_data_height;
						if( BGR24_data_ptr && (::rux::uint32)BGR24_data_size < data_size )
						{
							::rux::engine::free_mem( BGR24_data_ptr );
							BGR24_data_ptr = NULL;
						}
						if( BGR24_data_ptr == NULL )
						{
							BGR24_data_ptr = alloc_array_macros( ::rux::uint8 , data_size );
							BGR24_data_size = data_size;
						}
						size_t index1 = 0;
						for( ::rux::uint32 index0 = 0 ; index0 < data_size ; index0 += 3 )
						{
							BGR24_data_ptr[ index1++ ] = RGB24_data_ptr[ index0 + 2 ]; 
							BGR24_data_ptr[ index1++ ] = RGB24_data_ptr[ index0 + 1 ]; 
							BGR24_data_ptr[ index1++ ] = RGB24_data_ptr[ index0 ]; 
							BGR24_data_ptr[ index1++ ] = 255;
						}
					}
				}
			}
		};
		dll_internal void rux_BGR24_to_RGB32( const ::rux::uint8* BGR24_data_ptr , ::rux::uint32 BGR24_data_width , ::rux::uint32 BGR24_data_height , ::rux::uint8*& RGB32_data_ptr , ::rux::int32& RGB32_data_size , ::rux::uint8 alpha )
		{
			if( BGR24_data_ptr )
			{
				if( BGR24_data_width > 0 )
				{
					if( BGR24_data_height > 0 )
					{					
						::rux::uint32 data_size = BGR24_data_width * 4 * BGR24_data_height;
						if( RGB32_data_ptr && (::rux::uint32)RGB32_data_size < data_size )
						{
							::rux::engine::free_mem( RGB32_data_ptr );
							RGB32_data_ptr = NULL;
						}
						if( RGB32_data_ptr == NULL )
						{
							RGB32_data_ptr = alloc_array_macros( ::rux::uint8 , data_size );
							RGB32_data_size = data_size;
						}				
						::rux::uint32 index1 = 0;
						for( ::rux::uint32 index0 = 0 ; index0 < data_size ; index0 += 4 )
						{
							RGB32_data_ptr[ index0 + 2 ] = BGR24_data_ptr[ index1++ ];
							RGB32_data_ptr[ index0 + 1 ] = BGR24_data_ptr[ index1++ ]; 
							RGB32_data_ptr[ index0 ] = BGR24_data_ptr[ index1++ ];							
							RGB32_data_ptr[ index0 + 3 ] = alpha;
						}
					}
				}
			}
		};
		dll_internal void rux_BGR32_to_RGB32( const ::rux::uint8* BGR24_data_ptr , ::rux::uint32 BGR24_data_width , ::rux::uint32 BGR24_data_height , ::rux::uint8*& RGB32_data_ptr , ::rux::int32& RGB32_data_size )
		{
			if( BGR24_data_ptr )
			{
				if( BGR24_data_width > 0 )
				{
					if( BGR24_data_height > 0 )
					{					
						::rux::uint32 data_size = BGR24_data_width * 4 * BGR24_data_height;
						if( RGB32_data_ptr && (::rux::uint32)RGB32_data_size < data_size )
						{
							::rux::engine::free_mem( RGB32_data_ptr );
							RGB32_data_ptr = NULL;
						}
						if( RGB32_data_ptr == NULL )
						{
							RGB32_data_ptr = alloc_array_macros( ::rux::uint8 , data_size );
							RGB32_data_size = data_size;
						}
						for( ::rux::uint32 index0 = 0 ; index0 < data_size ; index0 += 4 )
						{
							RGB32_data_ptr[ index0 ] = BGR24_data_ptr[ index0 + 2 ]; 
							RGB32_data_ptr[ index0 + 1 ] = BGR24_data_ptr[ index0 + 1 ]; 
							RGB32_data_ptr[ index0 + 2 ] = BGR24_data_ptr[ index0 ]; 						
							RGB32_data_ptr[ index0 + 3 ] = BGR24_data_ptr[ index0 + 3 ];
						}
					}
				}
			}
		};
		dll_internal void rux_I420_to_BGR24( const ::rux::uint8* I420_data_ptr , ::rux::uint32 I420_data_width , ::rux::uint32 I420_data_height , ::rux::uint8*& BGR24_data_ptr , ::rux::int32& BGR24_data_size  )		
		{					
			if( I420_data_ptr )
			{
				if( I420_data_width > 0 )
				{
					if( I420_data_height > 0 )
					{
						::rux::uint32 y_count = I420_data_width * I420_data_height;
						::rux::uint32 uv_count = y_count / 4;
						::rux::uint32 data_size = y_count * 3;
						if( BGR24_data_ptr && (::rux::uint32)BGR24_data_size < data_size )
						{
							::rux::engine::free_mem( BGR24_data_ptr );
							BGR24_data_ptr = NULL;
						}
						if( BGR24_data_ptr == NULL )
						{
							BGR24_data_ptr = alloc_array_macros( ::rux::uint8 , data_size );
							BGR24_data_size = data_size;
						}				
						::rux::int32 index1 = 0 , r_coef = 0 , g_coef = 0 , b_coef = 0 , D = 0 , C = 0;
						for( ::rux::uint32 index0 = 0 ; index0 < uv_count ; index0++ )
						{						
							C = 298 * ( I420_data_ptr[ index1 ] - 16 );
							D = I420_data_ptr[ y_count + index0 ] - 128;				
							r_coef = 409 * ( I420_data_ptr[ y_count + uv_count + index0 ] - 128 ) + 128;
							g_coef = - 100 * D - 208 * ( I420_data_ptr[ y_count + uv_count + index0 ] - 128 ) + 128;
							b_coef = 516 * D + 128;			
							BGR24_data_ptr[ index1 * 3 + 2 ] = (::rux::uint8)( clip( ( C + r_coef ) >> 8 ) );
							BGR24_data_ptr[ index1 * 3 + 1 ] = (::rux::uint8)( clip( ( C + g_coef ) >> 8 ) );
							BGR24_data_ptr[ index1 * 3 ] = (::rux::uint8)( clip( ( C + b_coef ) >> 8 ) );	
							C = 298 * ( I420_data_ptr[ index1 + 1 ] - 16 );				
							BGR24_data_ptr[ ( index1 + 1 ) * 3 + 2 ] = (::rux::uint8)( clip( ( C + r_coef ) >> 8 ) );
							BGR24_data_ptr[ ( index1 + 1 ) * 3 + 1 ] = (::rux::uint8)( clip( ( C + g_coef ) >> 8 ) );
							BGR24_data_ptr[ ( index1 + 1 ) * 3 ] = (::rux::uint8)( clip( ( C + b_coef ) >> 8 ) );				
							C = 298 * ( I420_data_ptr[ index1 + I420_data_width ] - 16 );
							BGR24_data_ptr[ ( I420_data_width + index1 ) * 3 + 2 ] = (::rux::uint8)( clip( ( C + r_coef ) >> 8 ) );
							BGR24_data_ptr[ ( I420_data_width + index1 ) * 3 + 1 ] = (::rux::uint8)( clip( ( C + g_coef ) >> 8 ) );
							BGR24_data_ptr[ ( I420_data_width + index1 ) * 3 ] = (::rux::uint8)( clip( ( C + b_coef ) >> 8 ) );				
							C = 298 * ( I420_data_ptr[ index1 + I420_data_width + 1 ] - 16 );				
							BGR24_data_ptr[ ( I420_data_width + index1 + 1 ) * 3 + 2 ] = (::rux::uint8)( clip( ( C + r_coef ) >> 8 ) );
							BGR24_data_ptr[ ( I420_data_width + index1 + 1 ) * 3 + 1 ] = (::rux::uint8)( clip( ( C + g_coef ) >> 8 ) );
							BGR24_data_ptr[ ( I420_data_width + index1 + 1 ) * 3 ] = (::rux::uint8)( clip( ( C + b_coef ) >> 8 ) );			
							index1 += 2;
							if( index1 % I420_data_width == 0 )
							{
								if( ( index1 / I420_data_width ) % 2 != 0 )
									index1 += I420_data_width;					
							}
						}
					}
				}
			}
		};
		dll_internal void rux_I420_to_BGR32( const ::rux::uint8* I420_data_ptr , ::rux::uint32 I420_data_width , ::rux::uint32 I420_data_height , ::rux::uint8*& RGB32_data_ptr , ::rux::int32& RGB32_data_size  )		
		{					
			if( I420_data_ptr )
			{
				if( I420_data_width > 0 )
				{
					if( I420_data_height > 0 )
					{
						::rux::uint32 y_count = I420_data_width * I420_data_height;
						::rux::uint32 uv_count = y_count / 4;
						::rux::uint32 data_size = y_count * 4;
						if( RGB32_data_ptr && (::rux::uint32)RGB32_data_size < data_size )
						{
							::rux::engine::free_mem( RGB32_data_ptr );
							RGB32_data_ptr = NULL;
						}
						if( RGB32_data_ptr == NULL )
						{
							RGB32_data_ptr = alloc_array_macros( ::rux::uint8 , data_size );
							RGB32_data_size = data_size;
						}				
						::rux::int32 index1 = 0 , r_coef = 0 , g_coef = 0 , b_coef = 0 , D = 0 , C = 0;
						for( ::rux::uint32 index0 = 0 ; index0 < uv_count ; index0++ )
						{						
							C = 298 * ( I420_data_ptr[ index1 ] - 16 );
							D = I420_data_ptr[ y_count + index0 ] - 128;				
							r_coef = 409 * ( I420_data_ptr[ y_count + uv_count + index0 ] - 128 ) + 128;
							g_coef = - 100 * D - 208 * ( I420_data_ptr[ y_count + uv_count + index0 ] - 128 ) + 128;
							b_coef = 516 * D + 128;			
							RGB32_data_ptr[ index1 * 4 + 2] = (::rux::uint8)( clip( ( C + r_coef ) >> 8 ) );
							RGB32_data_ptr[ index1 * 4 + 1 ] = (::rux::uint8)( clip( ( C + g_coef ) >> 8 ) );
							RGB32_data_ptr[ index1 * 4 ] = (::rux::uint8)( clip( ( C + b_coef ) >> 8 ) );	
							RGB32_data_ptr[ index1 * 4 + 3 ] = 255;
							C = 298 * ( I420_data_ptr[ index1 + 1 ] - 16 );				
							RGB32_data_ptr[ ( index1 + 1 ) * 4 + 2 ] = (::rux::uint8)( clip( ( C + r_coef ) >> 8 ) );
							RGB32_data_ptr[ ( index1 + 1 ) * 4 + 1 ] = (::rux::uint8)( clip( ( C + g_coef ) >> 8 ) );
							RGB32_data_ptr[ ( index1 + 1 ) * 4 ] = (::rux::uint8)( clip( ( C + b_coef ) >> 8 ) );				
							RGB32_data_ptr[ ( index1 + 1 ) * 4 + 3 ] = 255;
							C = 298 * ( I420_data_ptr[ index1 + I420_data_width ] - 16 );
							RGB32_data_ptr[ ( I420_data_width + index1 ) * 4 + 2 ] = (::rux::uint8)( clip( ( C + r_coef ) >> 8 ) );
							RGB32_data_ptr[ ( I420_data_width + index1 ) * 4 + 1 ] = (::rux::uint8)( clip( ( C + g_coef ) >> 8 ) );
							RGB32_data_ptr[ ( I420_data_width + index1 ) * 4 ] = (::rux::uint8)( clip( ( C + b_coef ) >> 8 ) );				
							RGB32_data_ptr[ ( I420_data_width + index1 ) * 4 + 3 ] = 255;
							C = 298 * ( I420_data_ptr[ index1 + I420_data_width + 1 ] - 16 );				
							RGB32_data_ptr[ ( I420_data_width + index1 + 1 ) * 4 + 2 ] = (::rux::uint8)( clip( ( C + r_coef ) >> 8 ) );
							RGB32_data_ptr[ ( I420_data_width + index1 + 1 ) * 4 + 1 ] = (::rux::uint8)( clip( ( C + g_coef ) >> 8 ) );
							RGB32_data_ptr[ ( I420_data_width + index1 + 1 ) * 4 ] = (::rux::uint8)( clip( ( C + b_coef ) >> 8 ) );			
							RGB32_data_ptr[ ( I420_data_width + index1 + 1 ) * 4 + 3 ] = 255;
							index1 += 2;
							if( index1 % I420_data_width == 0 )
							{
								if( ( index1 / I420_data_width ) % 2 != 0 )
									index1 += I420_data_width;					
							}
						}
					}
				}
			}
		};
		dll_internal void rux_YUYV_to_BGR24( const ::rux::uint8* YUYV_data_ptr , ::rux::uint32 YUYV_data_width , ::rux::uint32 YUYV_data_height , ::rux::uint8*& BGR24_data_ptr , ::rux::int32& BGR24_data_size  )		
		{					
			if( YUYV_data_ptr )
			{
				if( YUYV_data_width > 0 )
				{
					if( YUYV_data_height > 0 )
					{
						::rux::uint32 temp = YUYV_data_width * YUYV_data_height;
						::rux::uint32 pixels_count = 2 * temp;
						::rux::uint32 data_size = temp * 3;
						if( BGR24_data_ptr && (::rux::uint32)BGR24_data_size < data_size )
						{
							::rux::engine::free_mem( BGR24_data_ptr );
							BGR24_data_ptr = NULL;
						}
						if( BGR24_data_ptr == NULL )
						{
							BGR24_data_ptr = alloc_array_macros( ::rux::uint8 , data_size );
							BGR24_data_size = data_size;
						}				
						::rux::int32 index1 = 0 , r_coef = 0 , g_coef = 0 , b_coef = 0 , D = 0 , C = 0;
						for( ::rux::uint32 index0 = 0 ; index0 < pixels_count ; index0 += 4 )
						{						
							C = 298 * ( YUYV_data_ptr[ index0 ] - 16 );//Y - 16					
							D = YUYV_data_ptr[ index0 + 1 ] - 128;				//U - 128
							r_coef = 409 * ( YUYV_data_ptr[ index0 + 3 ] - 128 ) + 128;//409 * ( V - 128 ) + 128
							g_coef = - 100 * D - 208 * ( YUYV_data_ptr[ index0 + 3 ] - 128 ) + 128;//- 100 * D - 208 * ( V - 128 ) + 128
							b_coef = 516 * D + 128;			
							BGR24_data_ptr[ index1 ] = (::rux::uint8)( clip( ( C + r_coef ) >> 8 ) );
							BGR24_data_ptr[ index1 + 1 ] = (::rux::uint8)( clip( ( C + g_coef ) >> 8 ) );
							BGR24_data_ptr[ index1 + 2 ] = (::rux::uint8)( clip( ( C + b_coef ) >> 8 ) );							
							C = 298 * ( YUYV_data_ptr[ index0 + 2 ] - 16 );				
							BGR24_data_ptr[ index1 + 3 ] = (::rux::uint8)( clip( ( C + r_coef ) >> 8 ) );
							BGR24_data_ptr[ index1 + 4 ] = (::rux::uint8)( clip( ( C + g_coef ) >> 8 ) );
							BGR24_data_ptr[ index1 + 5 ] = (::rux::uint8)( clip( ( C + b_coef ) >> 8 ) );												
							index1 += 6;						
						}
					}
				}
			}
		};
		dll_internal void rux_YUYV_to_BGR32( const ::rux::uint8* YUYV_data_ptr , ::rux::uint32 YUYV_data_width , ::rux::uint32 YUYV_data_height , ::rux::uint8*& BGR24_data_ptr , ::rux::int32& BGR24_data_size  )		
		{					
			if( YUYV_data_ptr )
			{
				if( YUYV_data_width > 0 )
				{
					if( YUYV_data_height > 0 )
					{
						::rux::uint32 temp = YUYV_data_width * YUYV_data_height;
						::rux::uint32 pixels_count = 2 * temp;
						::rux::uint32 data_size = temp * 3;
						if( BGR24_data_ptr && (::rux::uint32)BGR24_data_size < data_size )
						{
							::rux::engine::free_mem( BGR24_data_ptr );
							BGR24_data_ptr = NULL;
						}
						if( BGR24_data_ptr == NULL )
						{
							BGR24_data_ptr = alloc_array_macros( ::rux::uint8 , data_size );
							BGR24_data_size = data_size;
						}				
						::rux::int32 index1 = 0 , r_coef = 0 , g_coef = 0 , b_coef = 0 , D = 0 , C = 0;
						for( ::rux::uint32 index0 = 0 ; index0 < pixels_count ; index0 += 4 )
						{						
							C = 298 * ( YUYV_data_ptr[ index0 ] - 16 );//Y - 16					
							D = YUYV_data_ptr[ index0 + 1 ] - 128;				//U - 128
							r_coef = 409 * ( YUYV_data_ptr[ index0 + 3 ] - 128 ) + 128;//409 * ( V - 128 ) + 128
							g_coef = - 100 * D - 208 * ( YUYV_data_ptr[ index0 + 3 ] - 128 ) + 128;//- 100 * D - 208 * ( V - 128 ) + 128
							b_coef = 516 * D + 128;			
							BGR24_data_ptr[ index1++ ] = (::rux::uint8)( clip( ( C + r_coef ) >> 8 ) );
							BGR24_data_ptr[ index1++ ] = (::rux::uint8)( clip( ( C + g_coef ) >> 8 ) );
							BGR24_data_ptr[ index1++ ] = (::rux::uint8)( clip( ( C + b_coef ) >> 8 ) );							
							BGR24_data_ptr[ index1++ ] = 255;
							C = 298 * ( YUYV_data_ptr[ index0 + 2 ] - 16 );				
							BGR24_data_ptr[ index1++ ] = (::rux::uint8)( clip( ( C + r_coef ) >> 8 ) );
							BGR24_data_ptr[ index1++ ] = (::rux::uint8)( clip( ( C + g_coef ) >> 8 ) );
							BGR24_data_ptr[ index1++ ] = (::rux::uint8)( clip( ( C + b_coef ) >> 8 ) );												
							BGR24_data_ptr[ index1++ ] = 255;
						}
					}
				}
			}
		};
		dll_internal void rux_YV12_to_RotatedBGR24( const ::rux::uint8* YV12_data_ptr , ::rux::uint32 YV12_data_width , ::rux::uint32 YV12_data_height , ::rux::uint8*& BGR24_data_ptr , ::rux::int32& BGR24_data_size  )		
		{					
			if( YV12_data_ptr )
			{
				if( YV12_data_width > 0 )
				{
					if( YV12_data_height > 0 )
					{
						::rux::uint32 y_count = YV12_data_width * YV12_data_height;
						::rux::uint32 uv_count = y_count / 4;
						::rux::uint32 data_size = y_count * 3;
						if( BGR24_data_ptr && (::rux::uint32)BGR24_data_size < data_size )
						{
							::rux::engine::free_mem( BGR24_data_ptr );
							BGR24_data_ptr = NULL;
						}
						if( BGR24_data_ptr == NULL )
						{
							BGR24_data_ptr = alloc_array_macros( ::rux::uint8 , data_size );
							BGR24_data_size = data_size;
						}										
						::rux::int32 temp0 = 0;
						::rux::int32 index1 = 0 , r_coef = 0 , g_coef = 0 , b_coef = 0 , D = 0 , C = 0 , index2 = data_size - 6 * YV12_data_width;
						::rux::uint8 y = 0 , v = 0 , u = 0;
						for( ::rux::uint32 index0 = 0 ; index0 < uv_count ; index0++ )
						{						
							y = YV12_data_ptr[ index1 + YV12_data_width + 1 ];
							v = YV12_data_ptr[ y_count + index0 ];
							u = YV12_data_ptr[ y_count + uv_count + index0 ];
						
							C = 298 * ( y - 16 );													
							D = u - 128;				//U - 128
							r_coef = 409 * ( v - 128 ) + 128;//409 * ( V - 128 ) + 128
							g_coef = - 100 * D - 208 * ( v - 128 ) + 128;//- 100 * D - 208 * ( V - 128 ) + 128
							b_coef = 516 * D + 128;			
							temp0 = index2 + 3;
							BGR24_data_ptr[ temp0 + 2 ] = (::rux::uint8)( clip( ( C + r_coef ) >> 8 ) );			
							BGR24_data_ptr[ temp0 + 1 ] = (::rux::uint8)( clip( ( C + g_coef ) >> 8 ) );
							BGR24_data_ptr[ temp0 ] = (::rux::uint8)( clip( ( C + b_coef ) >> 8 ) );
							y = YV12_data_ptr[ index1 + YV12_data_width ];
							C = 298 * ( y - 16 );
							BGR24_data_ptr[ index2 + 2 ] = (::rux::uint8)( clip( ( C + r_coef ) >> 8 ) );													
							BGR24_data_ptr[ index2 + 1 ] = (::rux::uint8)( clip( ( C + g_coef ) >> 8 ) );
							BGR24_data_ptr[ index2 ] = (::rux::uint8)( clip( ( C + b_coef ) >> 8 ) );
							y = YV12_data_ptr[ index1 + 1 ];
							C = 298 * ( y - 16 );//Y - 16					
							temp0 = ( 3 * ( YV12_data_width + 1 ) + index2 );
							BGR24_data_ptr[ temp0 + 2 ] = (::rux::uint8)( clip( ( C + r_coef ) >> 8 ) );					
							BGR24_data_ptr[ temp0 + 1 ] = (::rux::uint8)( clip( ( C + g_coef ) >> 8 ) );
							BGR24_data_ptr[ temp0 ] = (::rux::uint8)( clip( ( C + b_coef ) >> 8 ) );
							y = YV12_data_ptr[ index1 ];
							C = 298 * ( y - 16 );										
							temp0 -= 3;
							BGR24_data_ptr[ temp0 + 2 ] = (::rux::uint8)( clip( ( C + r_coef ) >> 8 ) );										
							BGR24_data_ptr[ temp0 + 1 ] = (::rux::uint8)( clip( ( C + g_coef ) >> 8 ) );
							BGR24_data_ptr[ temp0 ] = (::rux::uint8)( clip( ( C + b_coef ) >> 8 ) );
							index1 += 2;
							index2 += 6;
							if( index1 != 0 && index1 % YV12_data_width == 0 )
							{
								index1 += YV12_data_width;
								index2 -= 9 * YV12_data_width;
							}						
						}
					}
				}
			}
		};
		dll_internal void rux_YV12_to_BGR24( const ::rux::uint8* YV12_data_ptr , ::rux::uint32 YV12_data_width , ::rux::uint32 YV12_data_height , ::rux::uint8*& BGR24_data_ptr , ::rux::int32& BGR24_data_size  )		
		{					
			if( YV12_data_ptr )
			{
				if( YV12_data_width > 0 )
				{
					if( YV12_data_height > 0 )
					{
						::rux::uint32 y_count = YV12_data_width * YV12_data_height;
						::rux::uint32 uv_count = y_count / 4;
						::rux::uint32 data_size = y_count * 3;
						if( BGR24_data_ptr && (::rux::uint32)BGR24_data_size < data_size )
						{
							::rux::engine::free_mem( BGR24_data_ptr );
							BGR24_data_ptr = NULL;
						}
						if( BGR24_data_ptr == NULL )
						{
							BGR24_data_ptr = alloc_array_macros( ::rux::uint8 , data_size );
							BGR24_data_size = data_size;
						}					
						::rux::int32 index1 = 0 , r_coef = 0 , g_coef = 0 , b_coef = 0 , D = 0 , C = 0;
						::rux::uint8 y = 0 , u = 0 , v = 0;
						::rux::int32 rgb_index = 0;
						for( ::rux::uint32 index0 = 0 ; index0 < uv_count ; index0++ )
						{						
							y = YV12_data_ptr[ index1 + 1 ];
							v = YV12_data_ptr[ y_count + index0 ];
							u = YV12_data_ptr[ y_count + uv_count + index0 ];
						
							C = 298 * ( y - 16 );//Y - 16					
							D = u - 128;				//U - 128
							r_coef = 409 * ( v - 128 ) + 128;//409 * ( V - 128 ) + 128
							g_coef = - 100 * D - 208 * ( v - 128 ) + 128;//- 100 * D - 208 * ( V - 128 ) + 128
							b_coef = 516 * D + 128;			
							BGR24_data_ptr[ rgb_index + 3 + 2 ] = (::rux::uint8)( clip( ( C + r_coef ) >> 8 ) );												
							BGR24_data_ptr[ rgb_index + 3 + 1 ] = (::rux::uint8)( clip( ( C + g_coef ) >> 8 ) );
							BGR24_data_ptr[ rgb_index + 3 ] = (::rux::uint8)( clip( ( C + b_coef ) >> 8 ) );
							y = YV12_data_ptr[ index1 ];
							C = 298 * ( y - 16 );				
							BGR24_data_ptr[ rgb_index + 2 ] = (::rux::uint8)( clip( ( C + r_coef ) >> 8 ) );
							BGR24_data_ptr[ rgb_index + 1 ] = (::rux::uint8)( clip( ( C + g_coef ) >> 8 ) );
							BGR24_data_ptr[ rgb_index ] = (::rux::uint8)( clip( ( C + b_coef ) >> 8 ) );
							index1 += YV12_data_width;
							rgb_index += 3 * YV12_data_width;
							y = YV12_data_ptr[ index1 + 1 ];
							C = 298 * ( y - 16 );							
							BGR24_data_ptr[ rgb_index + 3 + 2 ] = (::rux::uint8)( clip( ( C + r_coef ) >> 8 ) );						
							BGR24_data_ptr[ rgb_index + 3 + 1 ] = (::rux::uint8)( clip( ( C + g_coef ) >> 8 ) );
							BGR24_data_ptr[ rgb_index + 3 ] = (::rux::uint8)( clip( ( C + b_coef ) >> 8 ) );
							y = YV12_data_ptr[ index1 ];
							C = 298 * ( y - 16 );										
							BGR24_data_ptr[ rgb_index + 2 ] = (::rux::uint8)( clip( ( C + r_coef ) >> 8 ) );				
							BGR24_data_ptr[ rgb_index + 1 ] = (::rux::uint8)( clip( ( C + g_coef ) >> 8 ) );
							BGR24_data_ptr[ rgb_index ] = (::rux::uint8)( clip( ( C + b_coef ) >> 8 ) );
							index1 -= YV12_data_width;
							index1 += 2;
							rgb_index -= 3 * YV12_data_width;
							rgb_index += 6;
							if( index1 != 0 && index1 % YV12_data_width == 0 )
							{
								index1 += YV12_data_width;
								rgb_index += 3 * YV12_data_width;
							}
						}				
					}
				}
			}
		};
		dll_internal void rux_YV12_to_BGR32( const ::rux::uint8* YV12_data_ptr , ::rux::uint32 YV12_data_width , ::rux::uint32 YV12_data_height , ::rux::uint8*& RGB32_data_ptr , ::rux::int32& RGB32_data_size , ::rux::uint8 alpha )		
		{					
			if( YV12_data_ptr )
			{
				if( YV12_data_width > 0 )
				{
					if( YV12_data_height > 0 )
					{
						::rux::uint32 y_count = YV12_data_width * YV12_data_height;
						::rux::uint32 uv_count = y_count / 4;
						::rux::uint32 data_size = y_count * 4;
						if( RGB32_data_ptr && (::rux::uint32)RGB32_data_size < data_size )
						{
							::rux::engine::free_mem( RGB32_data_ptr );
							RGB32_data_ptr = NULL;
						}
						if( RGB32_data_ptr == NULL )
						{
							RGB32_data_ptr = alloc_array_macros( ::rux::uint8 , data_size );
							RGB32_data_size = data_size;
						}
						::rux::int32 index1 = 0 , r_coef = 0 , g_coef = 0 , b_coef = 0 , D = 0 , C = 0;
						::rux::uint8 y = 0 , v = 0 , u = 0;
						for( ::rux::uint32 index0 = 0 ; index0 < uv_count ; index0++ )
						{						
							y = YV12_data_ptr[ index1 + 1 ];
							v = YV12_data_ptr[ y_count + index0 ];
							u = YV12_data_ptr[ y_count + uv_count + index0 ];

							C = 298 * ( y - 16 );//Y - 16					
							D = u - 128;				//U - 128
							r_coef = 409 * ( v - 128 ) + 128;//409 * ( V - 128 ) + 128
							g_coef = - 100 * D - 208 * ( v - 128 ) + 128;//- 100 * D - 208 * ( V - 128 ) + 128
							b_coef = 516 * D + 128;			
							RGB32_data_ptr[ ( index1 + 1 ) * 3 + 2 ] = (::rux::uint8)( clip( ( C + r_coef ) >> 8 ) );												
							RGB32_data_ptr[ ( index1 + 1 ) * 3 + 1 ] = (::rux::uint8)( clip( ( C + g_coef ) >> 8 ) );
							RGB32_data_ptr[ ( index1 + 1 ) * 3 ] = (::rux::uint8)( clip( ( C + b_coef ) >> 8 ) );
							RGB32_data_ptr[ ( index1 + 1 ) * 3 + 3 ] = alpha;
							y = YV12_data_ptr[ index1 ];
							C = 298 * ( y - 16 );				
							RGB32_data_ptr[ index1 * 3 + 2 ] = (::rux::uint8)( clip( ( C + r_coef ) >> 8 ) );													
							RGB32_data_ptr[ index1 * 3 + 1 ] = (::rux::uint8)( clip( ( C + g_coef ) >> 8 ) );
							RGB32_data_ptr[ index1 * 3 ] = (::rux::uint8)( clip( ( C + b_coef ) >> 8 ) );
							RGB32_data_ptr[ index1 * 3 + 3 ] = alpha;
							y = YV12_data_ptr[ index1 + YV12_data_width + 1 ];
							C = 298 * ( y - 16 );							
							RGB32_data_ptr[ ( YV12_data_width + index1 + 1 ) * 3 + 2 ] = (::rux::uint8)( clip( ( C + r_coef ) >> 8 ) );						
							RGB32_data_ptr[ ( YV12_data_width + index1 + 1 ) * 3 + 1 ] = (::rux::uint8)( clip( ( C + g_coef ) >> 8 ) );
							RGB32_data_ptr[ ( YV12_data_width + index1 + 1 ) * 3 ] = (::rux::uint8)( clip( ( C + b_coef ) >> 8 ) );
							RGB32_data_ptr[ ( YV12_data_width + index1 + 1 ) * 3 + 3 ] = alpha;
							y = YV12_data_ptr[ index1 + YV12_data_width ];
							C = 298 * ( y - 16 );										
							RGB32_data_ptr[ ( YV12_data_width + index1 ) * 3 + 2 ] = (::rux::uint8)( clip( ( C + r_coef ) >> 8 ) );										
							RGB32_data_ptr[ ( YV12_data_width + index1 ) * 3 + 1 ] = (::rux::uint8)( clip( ( C + g_coef ) >> 8 ) );
							RGB32_data_ptr[ ( YV12_data_width + index1 ) * 3 ] = (::rux::uint8)( clip( ( C + b_coef ) >> 8 ) );
							RGB32_data_ptr[ ( YV12_data_width + index1 ) * 3 + 3 ] = alpha;
							index1 += 2;
							if( index1 != 0 && index1 % YV12_data_width == 0 )
									index1 += YV12_data_width;
						}
					}
				}
			}
		};
		dll_internal void rux_I420_to_RGB24( const ::rux::uint8* I420_data_ptr , ::rux::uint32 I420_data_width , ::rux::uint32 I420_data_height , ::rux::uint8*& RGB24_data_ptr , ::rux::int32& RGB24_data_size  )		
		{					
			if( I420_data_ptr )
			{
				if( I420_data_width > 0 )
				{
					if( I420_data_height > 0 )
					{
						::rux::uint32 y_count = I420_data_width * I420_data_height;
						::rux::uint32 uv_count = y_count / 4;
						::rux::uint32 data_size = y_count * 3;
						if( RGB24_data_ptr && (::rux::uint32)RGB24_data_size < data_size )
						{
							::rux::engine::free_mem( RGB24_data_ptr );
							RGB24_data_ptr = NULL;
						}
						if( RGB24_data_ptr == NULL )
						{
							RGB24_data_ptr = alloc_array_macros( ::rux::uint8 , data_size );
							RGB24_data_size = data_size;
						}				
						::rux::int32 index1 = 0 , r_coef = 0 , g_coef = 0 , b_coef = 0 , D = 0 , C = 0;
						for( ::rux::uint32 index0 = 0 ; index0 < uv_count ; index0++ )
						{						
							C = 298 * ( I420_data_ptr[ index1 ] - 16 );
							D = I420_data_ptr[ y_count + index0 ] - 128;				
							r_coef = 409 * ( I420_data_ptr[ y_count + uv_count + index0 ] - 128 ) + 128;
							g_coef = - 100 * D - 208 * ( I420_data_ptr[ y_count + uv_count + index0 ] - 128 ) + 128;
							b_coef = 516 * D + 128;			
							RGB24_data_ptr[ index1 * 3 ] = (::rux::uint8)( clip( ( C + r_coef ) >> 8 ) );
							RGB24_data_ptr[ index1 * 3 + 1 ] = (::rux::uint8)( clip( ( C + g_coef ) >> 8 ) );
							RGB24_data_ptr[ index1 * 3 + 2 ] = (::rux::uint8)( clip( ( C + b_coef ) >> 8 ) );	
							C = 298 * ( I420_data_ptr[ index1 + 1 ] - 16 );				
							RGB24_data_ptr[ ( index1 + 1 ) * 3 ] = (::rux::uint8)( clip( ( C + r_coef ) >> 8 ) );
							RGB24_data_ptr[ ( index1 + 1 ) * 3 + 1 ] = (::rux::uint8)( clip( ( C + g_coef ) >> 8 ) );
							RGB24_data_ptr[ ( index1 + 1 ) * 3 + 2 ] = (::rux::uint8)( clip( ( C + b_coef ) >> 8 ) );				
							C = 298 * ( I420_data_ptr[ index1 + I420_data_width ] - 16 );
							RGB24_data_ptr[ ( I420_data_width + index1 ) * 3 ] = (::rux::uint8)( clip( ( C + r_coef ) >> 8 ) );
							RGB24_data_ptr[ ( I420_data_width + index1 ) * 3 + 1 ] = (::rux::uint8)( clip( ( C + g_coef ) >> 8 ) );
							RGB24_data_ptr[ ( I420_data_width + index1 ) * 3 + 2 ] = (::rux::uint8)( clip( ( C + b_coef ) >> 8 ) );				
							C = 298 * ( I420_data_ptr[ index1 + I420_data_width + 1 ] - 16 );				
							RGB24_data_ptr[ ( I420_data_width + index1 + 1 ) * 3 ] = (::rux::uint8)( clip( ( C + r_coef ) >> 8 ) );
							RGB24_data_ptr[ ( I420_data_width + index1 + 1 ) * 3 + 1 ] = (::rux::uint8)( clip( ( C + g_coef ) >> 8 ) );
							RGB24_data_ptr[ ( I420_data_width + index1 + 1 ) * 3 + 2 ] = (::rux::uint8)( clip( ( C + b_coef ) >> 8 ) );			
							index1 += 2;
							if( index1 % I420_data_width == 0 )
							{
								if( ( index1 / I420_data_width ) % 2 != 0 )
									index1 += I420_data_width;					
							}
						}
					}
				}
			}
		};
		dll_internal void rux_YUYV_to_RGB24( const ::rux::uint8* YUYV_data_ptr , ::rux::uint32 YUYV_data_width , ::rux::uint32 YUYV_data_height , ::rux::uint8*& RGB24_data_ptr , ::rux::int32& RGB24_data_size  )		
		{					
			if( YUYV_data_ptr )
			{
				if( YUYV_data_width > 0 )
				{
					if( YUYV_data_height > 0 )
					{
						::rux::uint32 temp = YUYV_data_width * YUYV_data_height;
						::rux::uint32 pixels_count = 2 * temp;
						::rux::uint32 data_size = temp * 3;
						if( RGB24_data_ptr && (::rux::uint32)RGB24_data_size < data_size )
						{
							::rux::engine::free_mem( RGB24_data_ptr );
							RGB24_data_ptr = NULL;
						}
						if( RGB24_data_ptr == NULL )
						{
							RGB24_data_ptr = alloc_array_macros( ::rux::uint8 , data_size );
							RGB24_data_size = data_size;
						}				
						::rux::int32 index1 = 0 , r_coef = 0 , g_coef = 0 , b_coef = 0 , D = 0 , C = 0;
						for( ::rux::uint32 index0 = 0 ; index0 < pixels_count ; index0 += 4 )
						{						
							C = 298 * ( YUYV_data_ptr[ index0 ] - 16 );//Y - 16					
							D = YUYV_data_ptr[ index0 + 1 ] - 128;				//U - 128
							r_coef = 409 * ( YUYV_data_ptr[ index0 + 3 ] - 128 ) + 128;//409 * ( V - 128 ) + 128
							g_coef = - 100 * D - 208 * ( YUYV_data_ptr[ index0 + 3 ] - 128 ) + 128;//- 100 * D - 208 * ( V - 128 ) + 128
							b_coef = 516 * D + 128;			
							RGB24_data_ptr[ index1 + 2 ] = (::rux::uint8)( clip( ( C + r_coef ) >> 8 ) );
							RGB24_data_ptr[ index1 + 1 ] = (::rux::uint8)( clip( ( C + g_coef ) >> 8 ) );
							RGB24_data_ptr[ index1 ] = (::rux::uint8)( clip( ( C + b_coef ) >> 8 ) );							
							C = 298 * ( YUYV_data_ptr[ index0 + 2 ] - 16 );				
							RGB24_data_ptr[ index1 + 5 ] = (::rux::uint8)( clip( ( C + r_coef ) >> 8 ) );
							RGB24_data_ptr[ index1 + 4 ] = (::rux::uint8)( clip( ( C + g_coef ) >> 8 ) );
							RGB24_data_ptr[ index1 + 3 ] = (::rux::uint8)( clip( ( C + b_coef ) >> 8 ) );
							index1 += 6;						
						}
					}
				}
			}
		};
		dll_internal void rux_YV12_to_RGB24( const ::rux::uint8* YV12_data_ptr , ::rux::uint32 YV12_data_width , ::rux::uint32 YV12_data_height , ::rux::uint8*& RGB24_data_ptr , ::rux::int32& RGB24_data_size  )		
		{					
			if( YV12_data_ptr )
			{
				if( YV12_data_width > 0 )
				{
					if( YV12_data_height > 0 )
					{
						::rux::uint32 y_count = YV12_data_width * YV12_data_height;
						::rux::uint32 uv_count = y_count / 4;
						::rux::uint32 data_size = y_count * 3;
						if( RGB24_data_ptr && (::rux::uint32)RGB24_data_size < data_size )
						{
							::rux::engine::free_mem( RGB24_data_ptr );
							RGB24_data_ptr = NULL;
						}
						if( RGB24_data_ptr == NULL )
						{
							RGB24_data_ptr = alloc_array_macros( ::rux::uint8 , data_size );
							RGB24_data_size = data_size;
						}
						::rux::int32 index1 = 0 , r_coef = 0 , g_coef = 0 , b_coef = 0 , D = 0 , C = 0;
						::rux::uint8 y = 0 , v = 0 , u = 0;
						for( ::rux::uint32 index0 = 0 ; index0 < uv_count ; index0++ )
						{						
							y = YV12_data_ptr[ index1 + 1 ];
							v = YV12_data_ptr[ y_count + index0 ];
							u = YV12_data_ptr[ y_count + uv_count + index0 ];

							C = 298 * ( y - 16 );//Y - 16					
							D = u - 128;				//U - 128
							r_coef = 409 * ( v - 128 ) + 128;//409 * ( V - 128 ) + 128
							g_coef = - 100 * D - 208 * ( v - 128 ) + 128;//- 100 * D - 208 * ( V - 128 ) + 128
							b_coef = 516 * D + 128;			
							RGB24_data_ptr[ ( index1 + 1 ) * 3 ] = (::rux::uint8)( clip( ( C + r_coef ) >> 8 ) );												
							RGB24_data_ptr[ ( index1 + 1 ) * 3 + 1 ] = (::rux::uint8)( clip( ( C + g_coef ) >> 8 ) );
							RGB24_data_ptr[ ( index1 + 1 ) * 3 + 2 ] = (::rux::uint8)( clip( ( C + b_coef ) >> 8 ) );
							y = YV12_data_ptr[ index1 ];
							C = 298 * ( y - 16 );				
							RGB24_data_ptr[ index1 * 3 ] = (::rux::uint8)( clip( ( C + r_coef ) >> 8 ) );													
							RGB24_data_ptr[ index1 * 3 + 1 ] = (::rux::uint8)( clip( ( C + g_coef ) >> 8 ) );
							RGB24_data_ptr[ index1 * 3 + 2 ] = (::rux::uint8)( clip( ( C + b_coef ) >> 8 ) );
							y = YV12_data_ptr[ index1 + YV12_data_width + 1 ];
							C = 298 * ( y - 16 );							
							RGB24_data_ptr[ ( YV12_data_width + index1 + 1 ) * 3 ] = (::rux::uint8)( clip( ( C + r_coef ) >> 8 ) );						
							RGB24_data_ptr[ ( YV12_data_width + index1 + 1 ) * 3 + 1 ] = (::rux::uint8)( clip( ( C + g_coef ) >> 8 ) );
							RGB24_data_ptr[ ( YV12_data_width + index1 + 1 ) * 3 + 2 ] = (::rux::uint8)( clip( ( C + b_coef ) >> 8 ) );
							y = YV12_data_ptr[ index1 + YV12_data_width ];
							C = 298 * ( y - 16 );										
							RGB24_data_ptr[ ( YV12_data_width + index1 ) * 3 ] = (::rux::uint8)( clip( ( C + r_coef ) >> 8 ) );										
							RGB24_data_ptr[ ( YV12_data_width + index1 ) * 3 + 1 ] = (::rux::uint8)( clip( ( C + g_coef ) >> 8 ) );
							RGB24_data_ptr[ ( YV12_data_width + index1 ) * 3 + 2 ] = (::rux::uint8)( clip( ( C + b_coef ) >> 8 ) );
							index1 += 2;
							if( index1 != 0 && index1 % YV12_data_width == 0 )
									index1 += YV12_data_width;
						}
					}
				}
			}
		};
		dll_internal void rux_YV12_to_RGB32( const ::rux::uint8* YV12_data_ptr , ::rux::uint32 YV12_data_width , ::rux::uint32 YV12_data_height , ::rux::uint8*& RGB32_data_ptr , ::rux::int32& RGB32_data_size , ::rux::uint8 alpha )		
		{					
			if( YV12_data_ptr )
			{
				if( YV12_data_width > 0 )
				{
					if( YV12_data_height > 0 )
					{
						::rux::uint32 y_count = YV12_data_width * YV12_data_height;
						::rux::uint32 uv_count = y_count / 4;
						::rux::uint32 data_size = y_count * 4;
						if( RGB32_data_ptr && (::rux::uint32)RGB32_data_size < data_size )
						{
							::rux::engine::free_mem( RGB32_data_ptr );
							RGB32_data_ptr = NULL;
						}
						if( RGB32_data_ptr == NULL )
						{
							RGB32_data_ptr = alloc_array_macros( ::rux::uint8 , data_size );
							RGB32_data_size = data_size;
						}
						::rux::int32 index1 = 0 , r_coef = 0 , g_coef = 0 , b_coef = 0 , D = 0 , C = 0;
						::rux::uint8 y = 0 , v = 0 , u = 0;
						for( ::rux::uint32 index0 = 0 ; index0 < uv_count ; index0++ )
						{						
							y = YV12_data_ptr[ index1 + 1 ];
							v = YV12_data_ptr[ y_count + index0 ];
							u = YV12_data_ptr[ y_count + uv_count + index0 ];

							C = 298 * ( y - 16 );//Y - 16					
							D = u - 128;				//U - 128
							r_coef = 409 * ( v - 128 ) + 128;//409 * ( V - 128 ) + 128
							g_coef = - 100 * D - 208 * ( v - 128 ) + 128;//- 100 * D - 208 * ( V - 128 ) + 128
							b_coef = 516 * D + 128;			
							RGB32_data_ptr[ ( index1 + 1 ) * 3 ] = (::rux::uint8)( clip( ( C + r_coef ) >> 8 ) );												
							RGB32_data_ptr[ ( index1 + 1 ) * 3 + 1 ] = (::rux::uint8)( clip( ( C + g_coef ) >> 8 ) );
							RGB32_data_ptr[ ( index1 + 1 ) * 3 + 2 ] = (::rux::uint8)( clip( ( C + b_coef ) >> 8 ) );
							RGB32_data_ptr[ ( index1 + 1 ) * 3 + 3 ] = alpha;
							y = YV12_data_ptr[ index1 ];
							C = 298 * ( y - 16 );				
							RGB32_data_ptr[ index1 * 3 ] = (::rux::uint8)( clip( ( C + r_coef ) >> 8 ) );													
							RGB32_data_ptr[ index1 * 3 + 1 ] = (::rux::uint8)( clip( ( C + g_coef ) >> 8 ) );
							RGB32_data_ptr[ index1 * 3 + 2 ] = (::rux::uint8)( clip( ( C + b_coef ) >> 8 ) );
							RGB32_data_ptr[ index1 * 3 + 3 ] = alpha;
							y = YV12_data_ptr[ index1 + YV12_data_width + 1 ];
							C = 298 * ( y - 16 );							
							RGB32_data_ptr[ ( YV12_data_width + index1 + 1 ) * 3 ] = (::rux::uint8)( clip( ( C + r_coef ) >> 8 ) );						
							RGB32_data_ptr[ ( YV12_data_width + index1 + 1 ) * 3 + 1 ] = (::rux::uint8)( clip( ( C + g_coef ) >> 8 ) );
							RGB32_data_ptr[ ( YV12_data_width + index1 + 1 ) * 3 + 2 ] = (::rux::uint8)( clip( ( C + b_coef ) >> 8 ) );
							RGB32_data_ptr[ ( YV12_data_width + index1 + 1 ) * 3 + 3 ] = alpha;
							y = YV12_data_ptr[ index1 + YV12_data_width ];
							C = 298 * ( y - 16 );										
							RGB32_data_ptr[ ( YV12_data_width + index1 ) * 3 ] = (::rux::uint8)( clip( ( C + r_coef ) >> 8 ) );										
							RGB32_data_ptr[ ( YV12_data_width + index1 ) * 3 + 1 ] = (::rux::uint8)( clip( ( C + g_coef ) >> 8 ) );
							RGB32_data_ptr[ ( YV12_data_width + index1 ) * 3 + 2 ] = (::rux::uint8)( clip( ( C + b_coef ) >> 8 ) );
							RGB32_data_ptr[ ( YV12_data_width + index1 ) * 3 + 3 ] = alpha;
							index1 += 2;
							if( index1 != 0 && index1 % YV12_data_width == 0 )
									index1 += YV12_data_width;
						}
					}
				}
			}
		};
		dll_internal void rux_I420_to_RGB32( const ::rux::uint8* I420_data_ptr , ::rux::uint32 I420_data_width , ::rux::uint32 I420_data_height , ::rux::uint8*& RGB32_data_ptr , ::rux::int32& RGB32_data_size , ::rux::uint8 alpha )		
		{					
			if( I420_data_ptr )
			{
				if( I420_data_width > 0 )
				{
					if( I420_data_height > 0 )
					{
						::rux::uint32 y_count = I420_data_width * I420_data_height;
						::rux::uint32 uv_count = y_count / 4;
						::rux::uint32 data_size = y_count * 4;
						if( RGB32_data_ptr && (::rux::uint32)RGB32_data_size < data_size )
						{
							::rux::engine::free_mem( RGB32_data_ptr );
							RGB32_data_ptr = NULL;
						}
						if( RGB32_data_ptr == NULL )
						{
							RGB32_data_ptr = alloc_array_macros( ::rux::uint8 , data_size );
							RGB32_data_size = data_size;
						}				
						::rux::int32 index1 = 0 , r_coef = 0 , g_coef = 0 , b_coef = 0 , D = 0 , C = 0;
						for( ::rux::uint32 index0 = 0 ; index0 < uv_count ; index0++ )
						{						
							C = 298 * ( I420_data_ptr[ index1 ] - 16 );
							D = I420_data_ptr[ y_count + index0 ] - 128;				
							r_coef = 409 * ( I420_data_ptr[ y_count + uv_count + index0 ] - 128 ) + 128;
							g_coef = - 100 * D - 208 * ( I420_data_ptr[ y_count + uv_count + index0 ] - 128 ) + 128;
							b_coef = 516 * D + 128;			
							RGB32_data_ptr[ index1 * 4 ] = (::rux::uint8)( clip( ( C + r_coef ) >> 8 ) );
							RGB32_data_ptr[ index1 * 4 + 1 ] = (::rux::uint8)( clip( ( C + g_coef ) >> 8 ) );
							RGB32_data_ptr[ index1 * 4 + 2 ] = (::rux::uint8)( clip( ( C + b_coef ) >> 8 ) );	
							RGB32_data_ptr[ index1 * 4 + 3 ] = alpha;
							C = 298 * ( I420_data_ptr[ index1 + 1 ] - 16 );				
							RGB32_data_ptr[ ( index1 + 1 ) * 4 ] = (::rux::uint8)( clip( ( C + r_coef ) >> 8 ) );
							RGB32_data_ptr[ ( index1 + 1 ) * 4 + 1 ] = (::rux::uint8)( clip( ( C + g_coef ) >> 8 ) );
							RGB32_data_ptr[ ( index1 + 1 ) * 4 + 2 ] = (::rux::uint8)( clip( ( C + b_coef ) >> 8 ) );				
							RGB32_data_ptr[ ( index1 + 1 ) * 4 + 3 ] = alpha;
							C = 298 * ( I420_data_ptr[ index1 + I420_data_width ] - 16 );
							RGB32_data_ptr[ ( I420_data_width + index1 ) * 4 ] = (::rux::uint8)( clip( ( C + r_coef ) >> 8 ) );
							RGB32_data_ptr[ ( I420_data_width + index1 ) * 4 + 1 ] = (::rux::uint8)( clip( ( C + g_coef ) >> 8 ) );
							RGB32_data_ptr[ ( I420_data_width + index1 ) * 4 + 2 ] = (::rux::uint8)( clip( ( C + b_coef ) >> 8 ) );				
							RGB32_data_ptr[ ( I420_data_width + index1 ) * 4 + 3 ] = alpha;
							C = 298 * ( I420_data_ptr[ index1 + I420_data_width + 1 ] - 16 );				
							RGB32_data_ptr[ ( I420_data_width + index1 + 1 ) * 4 ] = (::rux::uint8)( clip( ( C + r_coef ) >> 8 ) );
							RGB32_data_ptr[ ( I420_data_width + index1 + 1 ) * 4 + 1 ] = (::rux::uint8)( clip( ( C + g_coef ) >> 8 ) );
							RGB32_data_ptr[ ( I420_data_width + index1 + 1 ) * 4 + 2 ] = (::rux::uint8)( clip( ( C + b_coef ) >> 8 ) );			
							RGB32_data_ptr[ ( I420_data_width + index1 + 1 ) * 4 + 3 ] = alpha;
							index1 += 2;
							if( index1 % I420_data_width == 0 )
							{
								if( ( index1 / I420_data_width ) % 2 != 0 )
									index1 += I420_data_width;					
							}
						}
					}
				}
			}
		};
		dll_internal void rux_RGB24_to_RGB32( const ::rux::uint8* RGB24_data_ptr , ::rux::uint32 RGB24_data_width , ::rux::uint32 RGB24_data_height , ::rux::uint8*& RGB32_data_ptr , ::rux::int32& RGB32_data_size , ::rux::uint8 alpha )		
		{					
			if( RGB24_data_ptr )
			{
				if( RGB24_data_width > 0 )
				{
					if( RGB24_data_height > 0 )
					{
						::rux::uint32 resolution = RGB24_data_width * RGB24_data_height;					
						::rux::uint32 data_size = resolution * 4;
						if( RGB32_data_ptr && (::rux::uint32)RGB32_data_size < data_size )
						{
							::rux::engine::free_mem( RGB32_data_ptr );
							RGB32_data_ptr = NULL;
						}
						if( RGB32_data_ptr == NULL )
						{
							RGB32_data_ptr = alloc_array_macros( ::rux::uint8 , data_size );
							RGB32_data_size = data_size;
						}									
						for( ::rux::uint32 index0 = 0 ; index0 < resolution ; index0++ )
						{			
							RGB32_data_ptr[ index0 * 4 ] = RGB24_data_ptr[ index0 * 3 ];
							RGB32_data_ptr[ index0 * 4 + 1 ] = RGB24_data_ptr[ index0 * 3 + 1 ];
							RGB32_data_ptr[ index0 * 4 + 2 ] = RGB24_data_ptr[ index0 * 3 + 2 ];
							RGB32_data_ptr[ index0 * 4 + 3 ] = alpha;						
						}
					}
				}
			}
		};
		dll_internal void rux_BGR24_to_BGR32( const ::rux::uint8* RGB24_data_ptr , ::rux::uint32 RGB24_data_width , ::rux::uint32 RGB24_data_height , ::rux::uint8*& RGB32_data_ptr , ::rux::int32& RGB32_data_size  )		
		{					
			if( RGB24_data_ptr )
			{
				if( RGB24_data_width > 0 )
				{
					if( RGB24_data_height > 0 )
					{
						::rux::uint32 resolution = RGB24_data_width * RGB24_data_height;					
						::rux::uint32 data_size = resolution * 4;
						if( RGB32_data_ptr && (::rux::uint32)RGB32_data_size < data_size )
						{
							::rux::engine::free_mem( RGB32_data_ptr );
							RGB32_data_ptr = NULL;
						}
						if( RGB32_data_ptr == NULL )
						{
							RGB32_data_ptr = alloc_array_macros( ::rux::uint8 , data_size );
							RGB32_data_size = data_size;
						}									
						for( ::rux::uint32 index0 = 0 ; index0 < resolution ; index0++ )
						{			
							RGB32_data_ptr[ index0 * 4 ] = RGB24_data_ptr[ index0 * 3 ];
							RGB32_data_ptr[ index0 * 4 + 1 ] = RGB24_data_ptr[ index0 * 3 + 1 ];
							RGB32_data_ptr[ index0 * 4 + 2 ] = RGB24_data_ptr[ index0 * 3 + 2 ];
							RGB32_data_ptr[ index0 * 4 + 3 ] = 255;						
						}
					}
				}
			}
		};
		dll_internal bool convert_to_rgb32( ::rux::uint8* image_data , ::rux::uint32 image_width , ::rux::uint32 image_height , ::rux::uint32 image_data_size , ::rux::int16 bit_count , ::rux::int32 compression , ::rux::uint8*& destination , ::rux::int32& destination_size , float alpha )
		{
			if( compression == ::rux::media::XEnum_Fourcc_YV12 )
				::rux::media::rux_YV12_to_RGB32( image_data , image_width , image_height , destination , destination_size , (::rux::uint8)( alpha * 255 ) );
			else if( compression == ::rux::media::XEnum_Fourcc_I420 )
				::rux::media::rux_I420_to_RGB32( image_data , image_width , image_height , destination , destination_size , alpha * 255 );
			else if( compression == ::rux::media::XEnum_Fourcc_RGB
				|| compression == ::rux::media::XEnum_Fourcc_BI_RGB )
			{
				if( bit_count == 24 )
					::rux::media::rux_RGB24_to_RGB32( image_data , image_width , image_height , destination , destination_size , alpha * 255 );
				else if( bit_count == 32 )
				{
					::memcpy( destination , image_data , image_data_size );
					for( size_t index = 3 ; index < image_data_size ; index += 4 )
						destination[ index ] *= alpha;
				}
				else
					return false;
			}
			else if( compression == XEnum_Fourcc_BGR3 )
				::rux::media::rux_BGR24_to_RGB32( image_data , image_width , image_height , destination , destination_size , alpha * 255 );
			else
				return false;
			return true;
		};
		dll_internal bool convert_to_rgb24( ::rux::uint8* image_data , ::rux::uint32 image_width , ::rux::uint32 image_height , ::rux::uint32 image_data_size , ::rux::int16 bit_count , ::rux::int32 compression , ::rux::uint8*& destination , ::rux::int32& destination_size )
		{
			if( compression == ::rux::media::XEnum_Fourcc_I420 )
				::rux::media::rux_I420_to_RGB24( image_data , image_width , image_height , destination , destination_size );		
			else if( compression == ::rux::media::XEnum_Fourcc_YV12 )
				::rux::media::rux_YV12_to_RGB24( image_data , image_width , image_height , destination , destination_size );
			else if( compression == ::rux::media::XEnum_Fourcc_YUYV )
				::rux::media::rux_YUYV_to_RGB24( image_data , image_width , image_height , destination , destination_size );
			else if( compression == ::rux::media::XEnum_Fourcc_BGR3 )
				::rux::media::rux_BGR24_to_RGB24( image_data , image_width , image_height , destination , destination_size );
			else if( compression == ::rux::media::XEnum_Fourcc_RGB
				|| compression == ::rux::media::XEnum_Fourcc_BI_RGB )
			{
				if( bit_count == 24 )
					::memcpy( destination , image_data , image_data_size );			
				else
					return false;
			}
			else
				return false;
			return true;
		};		
		dll_internal ::rux::uint32 image_size( ::rux::int32 fourcc , ::rux::uint32 width , ::rux::uint32 height , ::rux::int16 bit_count )
		{
			if( fourcc == ::rux::media::XEnum_Fourcc_I420 )
				return width * height + width * height / 2;
			else if( fourcc == ::rux::media::XEnum_Fourcc_YV12 )
				return width * height + width * height / 2;
			else if( fourcc == ::rux::media::XEnum_Fourcc_RGB
				|| fourcc == ::rux::media::XEnum_Fourcc_BI_RGB
				|| fourcc == ::rux::media::XEnum_Fourcc_BGR3
				|| fourcc == ::rux::media::XEnum_Fourcc_GREY
				|| fourcc == ::rux::media::XEnum_Fourcc_Y800
				|| fourcc == ::rux::media::XEnum_Fourcc_Y8 )
				return width * height * bit_count / 8;
			else if( fourcc == ::rux::media::XEnum_Fourcc_YUYV )
				return 2 * width * height;
			else
				return 0;
		};
	};
	namespace gui
	{
		ColorBase::ColorBase( ::rux::int32 color_type )
		{
			_color_type = color_type;
			_ref = 0;
		};
		void ColorBase::Release( void )
		{
			if( XInterlocked::CompareExchange( &_ref , 0 , 0 ) == 0 
				|| XInterlocked::Decrement( &_ref ) == 0 )
			{
				if( _color_type == 0 )
				{
					::rux::gui::Color* color = (::rux::gui::Color*)this;
					::rux::engine::free_object< ::rux::gui::Color >( color );
				}
				else if( _color_type == 1 )
				{
					::rux::gui::LinearGradientColor* color = (::rux::gui::LinearGradientColor*)this;
					::rux::engine::free_object< ::rux::gui::LinearGradientColor >( color );
				}
			}
		};
		void ColorBase::AddRef( void )
		{
			XInterlocked::Increment( &_ref );
		};
		RenderCacheBase::RenderCacheBase( ::rux::int32 cache_type )
		{
			_cache_type = cache_type;
		};
		Color::Color( void ) 
			: ::rux::gui::ColorBase( 0 )
		{
			_shader_color_map = NULL;
			_red = 0;
			_green = 0;
			_blue = 0;
			_alpha = 255;
		};
		Color::~Color()
		{
			if( _shader_color_map != NULL )
			{
				::rux::engine::free_mem( _shader_color_map );
				_shader_color_map = NULL;
			}
		}			
		Color::Color( ::rux::uint8 red , ::rux::uint8 green , ::rux::uint8 blue , ::rux::uint8 alpha )
			: ::rux::gui::ColorBase( 0 )
		{
			_shader_color_map = NULL;
			_red = red;
			_green = green;
			_blue = blue;
			_alpha = alpha;
		};
		Color::Color( const ::rux::gui::Color& color )
			: ::rux::gui::ColorBase( 0 )
		{
			_shader_color_map = NULL;
			_red = color._red;
			_green = color._green;
			_blue = color._blue;
			_alpha = color._alpha;				
		};
		::rux::gui::Color& Color::operator =( const ::rux::gui::Color& color )
		{
			_red = color._red;
			_green = color._green;
			_blue = color._blue;
			_alpha = color._alpha;
			return *this;
		};			
		void Color::set_Red( ::rux::uint8 red )
		{
			_red = red;
		};
		void Color::set_Green( ::rux::uint8 green )
		{
			_green = green;
		};
		void Color::set_Blue( ::rux::uint8 blue )
		{
			_blue = blue;
		};
		void Color::set_Alpha( ::rux::uint8 alpha )
		{
			_alpha = alpha;
		};
		::rux::uint8 Color::get_IsAlpha( void )
		{
			return _alpha < 255 ? 1 : 0;
		};
		::rux::uint8 Color::get_Alpha( void )
		{
			return _alpha;
		};
		::rux::uint8 Color::get_Red( void )
		{
			return _red;
		};
		::rux::uint8 Color::get_Blue( void )
		{
			return _blue;
		};
		::rux::uint8 Color::get_Green( void )
		{
			return _green;
		};
		bool Color::operator ==( const ::rux::gui::Color& color )
		{
			return _alpha == color._alpha && _red == color._red
				&& _green == color._green && _blue == color._blue;
		};
		bool GradientStop::operator ==( const GradientStop& gradient_stop )
		{
			return _offset == gradient_stop._offset && _color == gradient_stop._color;
		};
		::rux::uint8 Color::Equals( ColorBase* color )
		{
			if( _color_type == 0 )
			{
				Color* c = (Color*)color;
				return *this == *c ? 1 : 0;
			}
			else
				return 0;
		};
		::rux::uint8 Color::get_IsVisible( void )
		{	
			return _alpha > 0 ? 1 : 0;
		};
		::rux::gui::Color* Colors::LightGrey( float opacity )
		{
			return alloc_object_4_params_macros( ::rux::gui::Color , (::rux::uint8)204 , (::rux::uint8)204 , (::rux::uint8)204 , (::rux::uint8)( 255.f * opacity ) );
		};
		::rux::gui::Color* Colors::Grey( float opacity )
		{
			return alloc_object_4_params_macros( ::rux::gui::Color , (::rux::uint8)153 , (::rux::uint8)153 , (::rux::uint8)153 , (::rux::uint8)( 255.f * opacity ) );
		};
		::rux::gui::Color* Colors::DarkGrey( float opacity )
		{
			return alloc_object_4_params_macros( ::rux::gui::Color , (::rux::uint8)102 , (::rux::uint8)102 , (::rux::uint8)102 , (::rux::uint8)( 255.f * opacity ) );
		};
		::rux::gui::Color* Colors::Green( float opacity )
		{
			return alloc_object_4_params_macros( ::rux::gui::Color , (::rux::uint8)0 , (::rux::uint8)255 , (::rux::uint8)0 , (::rux::uint8)( 255.f * opacity ) );
		};
		::rux::gui::Color* Colors::Red( float opacity )
		{
			return alloc_object_4_params_macros( ::rux::gui::Color , (::rux::uint8)255 , (::rux::uint8)0 , (::rux::uint8)0 , (::rux::uint8)( 255.f * opacity ) );
		};
		::rux::gui::Color* Colors::Blue( float opacity )
		{
			return alloc_object_4_params_macros( ::rux::gui::Color , (::rux::uint8)0 , (::rux::uint8)0 , (::rux::uint8)255 , (::rux::uint8)( 255.f * opacity ) );
		};
		::rux::gui::Color* Colors::White( float opacity )
		{
			return alloc_object_4_params_macros( ::rux::gui::Color , (::rux::uint8)255 , (::rux::uint8)255 , (::rux::uint8)255 , (::rux::uint8)( 255.f * opacity ) );
		};
		::rux::gui::Color* Colors::Black( float opacity , const char* __file__ , ::rux::int32 __line__ )
		{
			return ::rux::engine::alloc_object< ::rux::gui::Color >( (::rux::uint8)0 , (::rux::uint8)0 , (::rux::uint8)0 , (::rux::uint8)( 255.f * opacity ) , __file__ ? (const char*)__file__ : __FILE__ , __file__ ? (::rux::uint32)__line__ : (::rux::uint32)__LINE__, (char*)NULL , (::rux::malloc_t)NULL );
		};
		::rux::gui::Color* Colors::DarkSlateGray( float opacity )
		{
			return alloc_object_4_params_macros( ::rux::gui::Color , (::rux::uint8)47 , (::rux::uint8)79 , (::rux::uint8)79 , (::rux::uint8)( 255.f * opacity ) );
		};
		::rux::gui::Color* Colors::Arsenic( float opacity )
		{
			return alloc_object_4_params_macros( ::rux::gui::Color , (::rux::uint8)59 , (::rux::uint8)68 , (::rux::uint8)75 , (::rux::uint8)( 255.f * opacity ) );
		};
		::rux::gui::Color* Colors::BattleshipGrey( float opacity )
		{
			return alloc_object_4_params_macros( ::rux::gui::Color , (::rux::uint8)132 , (::rux::uint8)132 , (::rux::uint8)130 , (::rux::uint8)( 255.f * opacity ) );
		};
		::rux::gui::Color* Colors::Transparent( void )
		{
			return alloc_object_4_params_macros( ::rux::gui::Color , (::rux::uint8)0 , (::rux::uint8)0 , (::rux::uint8)0 , (::rux::uint8)0 );
		};
		::rux::gui::Color* Colors::HighlightColor( void )
		{
			return alloc_object_4_params_macros( ::rux::gui::Color , (::rux::uint8)0x31 , (::rux::uint8)0x6A , (::rux::uint8)0xC5 , (::rux::uint8)0xFF );
		};
		::rux::gui::Color* Colors::ControlDarkDarkColor( void )
		{
			return alloc_object_4_params_macros( ::rux::gui::Color , (::rux::uint8)0x71 , (::rux::uint8)0x6F , (::rux::uint8)0x64 , (::rux::uint8)0xFF );
		};
		::rux::gui::Color* Colors::SliderTrackNormalBackground( void )
		{	
			return alloc_object_4_params_macros( ::rux::gui::Color , (::rux::uint8)0x32 , (::rux::uint8)0x36 , (::rux::uint8)0x39 , (::rux::uint8)0xFF );
		};
		::rux::uint32 Color::get_ShaderColorMapWidth( void )
		{
			return 4;
		};
		::rux::uint8* Color::get_ShaderColorMap( void )
		{
			if( _shader_color_map == NULL )
				_shader_color_map = alloc_array_macros( ::rux::uint8 , 4 );
			_shader_color_map[ 0 ] = _red;
			_shader_color_map[ 1 ] = _green;
			_shader_color_map[ 2 ] = _blue;
			_shader_color_map[ 3 ] = _alpha;
			return _shader_color_map;
		};
		::rux::gui::Color* Color::FromString( const char* color_string )
		{				
			if( color_string && color_string[ 0 ] != 0 )
			{
				if( color_string[ 0 ] == '#' )
				{
					::rux::uint32 value = 0;
					if( rux_hex_string_to_uint32( &color_string[ 1 ] , value ) == 1 )
						return alloc_object_4_params_macros( ::rux::gui::Color , rux_get_byte( value , 2 ) ,
							rux_get_byte( value , 1 ) ,
							rux_get_byte( value , 0 ) , 
							rux_get_byte( value , 3 ) );
				}
				else
				{
					::rux::uint32 value = 0;
					if( rux_hex_string_to_uint32( &color_string[ 0 ] , value ) == 1 )
						return alloc_object_4_params_macros( ::rux::gui::Color , rux_get_byte( value , 2 ) ,
							rux_get_byte( value , 1 ) ,
							rux_get_byte( value , 0 ) , 
							rux_get_byte( value , 3 ) );
				}
			}
			return alloc_object_macros( ::rux::gui::Color );
		};
		GradientStop::GradientStop( void )
		{
			_offset = 0.0;
		};
		GradientStop::GradientStop( const ::rux::gui::Color& color , float offset )
		{
			_color = color;
			if( offset <= 1.0 )
				_offset = offset;
		};
		GradientStop::GradientStop( const ::rux::gui::GradientStop& gradientstop )				
		{
			_color = gradientstop._color;
			if( gradientstop._offset <= 1.0 )
				_offset = gradientstop._offset;
		};
		::rux::gui::GradientStop& GradientStop::operator =( const ::rux::gui::GradientStop& gradientstop )
		{
			_color = gradientstop._color;
			if( gradientstop._offset <= 1.0 )
				_offset = gradientstop._offset;
			return *this;
		};
		LinearGradientColor::LinearGradientColor( void )
			: ::rux::gui::ColorBase( 1 )
		{
			_linear_gradient_type = XEnum_LinearGradientType_Horizontal;
			_shader_color_map = NULL;
			_shader_color_offset_map = NULL;
		};
		LinearGradientColor::~LinearGradientColor()
		{
			for( ; _gradient_stops.Count() > 0 ; )
			{
				::rux::engine::free_object< ::rux::gui::GradientStop >( _gradient_stops[ _gradient_stops.Count() - 1 ] );
				_gradient_stops.RemoveAt( _gradient_stops.Count() - 1 );
			}
			if( _shader_color_map != NULL )
				::rux::engine::free_mem( _shader_color_map );
			_shader_color_map = NULL;
			if( _shader_color_offset_map != NULL )
				::rux::engine::free_mem( _shader_color_offset_map );
		};
		LinearGradientColor::LinearGradientColor( const ::rux::gui::LinearGradientColor& lineargradientcolor )
			: ::rux::gui::ColorBase( 1 )
		{
			CRITICAL_SECTION_LOCK( _cs_gradient_stops );
			_shader_color_map = NULL;
			_shader_color_offset_map = NULL;
			_linear_gradient_type = lineargradientcolor._linear_gradient_type;
			::rux::gui::LinearGradientColor* lgc = const_cast< ::rux::gui::LinearGradientColor* >( &lineargradientcolor );
			for( size_t index0 = 0 ; index0 < lgc->_gradient_stops.Count() ; index0++ )
				_gradient_stops.Add( alloc_object_2_params_macros( ::rux::gui::GradientStop , lgc->_gradient_stops[ index0 ]->_color , lgc->_gradient_stops[ index0 ]->_offset ) );				
			CRITICAL_SECTION_UNLOCK( _cs_gradient_stops );
		};
		::rux::gui::LinearGradientColor& LinearGradientColor::operator =( const ::rux::gui::LinearGradientColor& lineargradientcolor )
		{
			CRITICAL_SECTION_LOCK( _cs_gradient_stops );
			if( _shader_color_map != NULL )
			{
				::rux::engine::free_mem( _shader_color_map );
				_shader_color_map = NULL;
			}
			if( _shader_color_offset_map != NULL )
			{
				::rux::engine::free_mem( _shader_color_offset_map );
				_shader_color_offset_map = NULL;
			}
			_linear_gradient_type = lineargradientcolor._linear_gradient_type;
			::rux::gui::LinearGradientColor* lgc = const_cast< ::rux::gui::LinearGradientColor* >( &lineargradientcolor );
			for( ; _gradient_stops.Count() > 0 ; )
			{
				::rux::engine::free_object< ::rux::gui::GradientStop >( _gradient_stops[ _gradient_stops.Count() - 1 ] );
				_gradient_stops.RemoveAt( _gradient_stops.Count() - 1 );
			}
			for( size_t index0 = 0 ; index0 < lgc->_gradient_stops.Count() ; index0++ )
				_gradient_stops.Add( alloc_object_2_params_macros( ::rux::gui::GradientStop , lgc->_gradient_stops[ index0 ]->_color , lgc->_gradient_stops[ index0 ]->_offset ) );				
			CRITICAL_SECTION_UNLOCK( _cs_gradient_stops );
			return *this;
		};
		void LinearGradientColor::AddGradientStop( const ::rux::gui::GradientStop& gradient_stop )
		{
			CRITICAL_SECTION_LOCK( _cs_gradient_stops );
			_gradient_stops.Add( alloc_object_2_params_macros( ::rux::gui::GradientStop , gradient_stop._color , gradient_stop._offset ) );
			if( _shader_color_map != NULL )
			{
				::rux::engine::free_mem( _shader_color_map );
				_shader_color_map = NULL;
			}
			if( _shader_color_offset_map != NULL )
			{
				::rux::engine::free_mem( _shader_color_offset_map );
				_shader_color_offset_map = NULL;
			}
			CRITICAL_SECTION_UNLOCK( _cs_gradient_stops );
		};
		void LinearGradientColor::set_LinearGradientType( XEnum_LinearGradientType linear_gradient_type )
		{
			_linear_gradient_type = linear_gradient_type;
		};
		::rux::uint32 LinearGradientColor::get_ShaderColorMapWidth( void )
		{
			return 4 * _gradient_stops.Count();
		};
		::rux::uint8* LinearGradientColor::get_ShaderColorMap( void )
		{
			CRITICAL_SECTION_LOCK( _cs_gradient_stops );
			if( _shader_color_map == NULL )
			{
				::rux::uint32 count = 4 * _gradient_stops.Count();
				_shader_color_map = alloc_array_macros( ::rux::uint8 , count );
				::rux::uint32 index1 = 0;
				for( ::rux::uint32 index0 = 0 ; index0 < _gradient_stops.Count() ; index0++ )
				{
					_shader_color_map[ index1 ] = _gradient_stops[ index0 ]->_color.get_Red();
					index1++;
					_shader_color_map[ index1 ] = _gradient_stops[ index0 ]->_color.get_Green();
					index1++;
					_shader_color_map[ index1 ] = _gradient_stops[ index0 ]->_color.get_Blue();
					index1++;
					_shader_color_map[ index1 ] = _gradient_stops[ index0 ]->_color.get_Alpha();
					index1++;
				}
			}
			CRITICAL_SECTION_UNLOCK( _cs_gradient_stops );
			return _shader_color_map;
		};
		::rux::uint8* LinearGradientColor::get_ShaderColorOffsetMap( void )
		{
			CRITICAL_SECTION_LOCK( _cs_gradient_stops );
			if( _shader_color_offset_map == NULL )
			{
				_shader_color_offset_map = alloc_array_macros( ::rux::uint8 , _gradient_stops.Count() );
				for( ::rux::uint32 index0 = 0 ; index0 < _gradient_stops.Count() ; index0++ )
					_shader_color_offset_map[ index0 ] = static_cast< ::rux::uint8 >( _gradient_stops[ index0 ]->_offset * 100.f );
			}
			CRITICAL_SECTION_UNLOCK( _cs_gradient_stops );
			return _shader_color_offset_map;
		};
		::rux::uint32 LinearGradientColor::get_ShaderColorOffsetMapWidth( void )
		{
			return _gradient_stops.Count();
		}
		bool LinearGradientColor::operator ==( const LinearGradientColor& linear_gradient_color )
		{
			LinearGradientColor* that = const_cast< LinearGradientColor* >( &linear_gradient_color );
			CRITICAL_SECTION_LOCK( _cs_gradient_stops );
			CRITICAL_SECTION_LOCK( that->_cs_gradient_stops );
			bool res = _linear_gradient_type == that->_linear_gradient_type 
				&& _gradient_stops.Count() == that->_gradient_stops.Count();
			if( res )
			{
				for( size_t index0 = 0 ; index0 < _gradient_stops.Count() ; index0++ )
				{
					if( ( _gradient_stops[ index0 ] == that->_gradient_stops[ index0 ] ) == false )
					{
						res = false;
						break;
					}
				}
			}			
			that->_cs_gradient_stops.UnLock();
			_cs_gradient_stops.UnLock();
			return res;
		};
		::rux::uint8 LinearGradientColor::Equals( ColorBase* color_base )
		{
			if( color_base->_color_type == 1 )
			{
				LinearGradientColor* linear_gradient_color = (LinearGradientColor*)color_base;
				return *this == *linear_gradient_color;
			}
			else
				return 0;
		};
		::rux::uint8 LinearGradientColor::get_IsAlpha( void )
		{
			::rux::uint8 alpha = 0;
			CRITICAL_SECTION_LOCK( _cs_gradient_stops );
			for( size_t index0 = 0 ; index0 < _gradient_stops.Count() ; index0++ )
			{
				if( _gradient_stops[ index0 ]->_color._alpha < 255 )
				{
					alpha = 1;
					break;
				}			
			}
			CRITICAL_SECTION_UNLOCK( _cs_gradient_stops );
			return alpha;
		};
		::rux::uint8 LinearGradientColor::get_IsVisible( void )
		{
			::rux::uint8 visible = 0;
			CRITICAL_SECTION_LOCK( _cs_gradient_stops );
			for( size_t index0 = 0 ; index0 < _gradient_stops.Count() ; index0++ )
			{
				if( _gradient_stops[ index0 ]->_color._alpha > 0 )
				{
					visible = 1;
					break;
				}			
			}
			CRITICAL_SECTION_UNLOCK( _cs_gradient_stops );
			return visible;
		};
		::rux::gui::LinearGradientColor* Colors::VerticalSliderPanelColor( void )
		{
			::rux::gui::LinearGradientColor* linear_gradient = alloc_object_macros( ::rux::gui::LinearGradientColor );
			linear_gradient->set_LinearGradientType( XEnum_LinearGradientType_Horizontal );
			linear_gradient->AddGradientStop( ::rux::gui::GradientStop( ::rux::gui::Color( 0x32 , 0x36 , 0x39 , 255 ) , 0.0f ) );	
			linear_gradient->AddGradientStop( ::rux::gui::GradientStop( ::rux::gui::Color( 0x45 , 0x49 , 0x4B , 255 ) , 1.f ) );
			return linear_gradient;
		};
		::rux::gui::LinearGradientColor* Colors::VerticalSliderThumbColor( void )
		{
			::rux::gui::LinearGradientColor* linear_gradient = alloc_object_macros( ::rux::gui::LinearGradientColor );
			linear_gradient->set_LinearGradientType( XEnum_LinearGradientType_Horizontal );
			linear_gradient->AddGradientStop( ::rux::gui::GradientStop( ::rux::gui::Color( 0x95 , 0x9A , 0x9C , 255 ) , 0.0f ) );	
			linear_gradient->AddGradientStop( ::rux::gui::GradientStop( ::rux::gui::Color( 0x68 , 0x6B , 0x6D , 255 ) , 0.499f ) );
			linear_gradient->AddGradientStop( ::rux::gui::GradientStop( ::rux::gui::Color( 0x55 , 0x59 , 0x5C , 255 ) , 0.5f ) );
			linear_gradient->AddGradientStop( ::rux::gui::GradientStop( ::rux::gui::Color( 0x3B , 0x40 , 0x44 , 255 ) , 1.f ) );
			return linear_gradient;
		};
		::rux::gui::LinearGradientColor* Colors::VerticalSliderOverThumbColor( void )
		{
			::rux::gui::LinearGradientColor* linear_gradient = alloc_object_macros( ::rux::gui::LinearGradientColor );
			linear_gradient->set_LinearGradientType( XEnum_LinearGradientType_Horizontal );
			linear_gradient->AddGradientStop( ::rux::gui::GradientStop( ::rux::gui::Color( 0xA4 , 0xA5 , 0xA5 , 255 ) , 0.0f ) );	
			linear_gradient->AddGradientStop( ::rux::gui::GradientStop( ::rux::gui::Color( 0x83 , 0x85 , 0x85 , 255 ) , 0.499f ) );
			linear_gradient->AddGradientStop( ::rux::gui::GradientStop( ::rux::gui::Color( 0x69 , 0x6B , 0x6C , 255 ) , 0.5f ) );
			linear_gradient->AddGradientStop( ::rux::gui::GradientStop( ::rux::gui::Color( 0x51 , 0x56 , 0x58 , 255 ) , 1.f ) );
			return linear_gradient;
		};
		::rux::gui::LinearGradientColor* Colors::VerticalSliderDraggingThumbColor( void )
		{
			::rux::gui::LinearGradientColor* linear_gradient = alloc_object_macros( ::rux::gui::LinearGradientColor );
			linear_gradient->set_LinearGradientType( XEnum_LinearGradientType_Horizontal );
			linear_gradient->AddGradientStop( ::rux::gui::GradientStop( ::rux::gui::Color( 0x66 , 0xD8 , 0xFB , 255 ) , 0.0f ) );	
			linear_gradient->AddGradientStop( ::rux::gui::GradientStop( ::rux::gui::Color( 0x31 , 0xB1 , 0xE5 , 255 ) , 0.499f ) );
			linear_gradient->AddGradientStop( ::rux::gui::GradientStop( ::rux::gui::Color( 0x29 , 0x9E , 0xD0 , 255 ) , 0.5f ) );
			linear_gradient->AddGradientStop( ::rux::gui::GradientStop( ::rux::gui::Color( 0x19 , 0x78 , 0xAF , 255 ) , 1.f ) );
			return linear_gradient;
		};
		::rux::gui::LinearGradientColor* Colors::WindowHeaderColor( void )
		{
			::rux::gui::LinearGradientColor* linear_gradient = alloc_object_macros( ::rux::gui::LinearGradientColor );
			linear_gradient->set_LinearGradientType( XEnum_LinearGradientType_Vertical );
			linear_gradient->AddGradientStop( ::rux::gui::GradientStop( ::rux::gui::Color( 0x2B , 0x2F , 0x32 , 255 ) , 0.0f ) );
			linear_gradient->AddGradientStop( ::rux::gui::GradientStop( ::rux::gui::Color( 0x45 , 0x49 , 0x4B , 255 ) , 1.f ) );
			return linear_gradient;
		};
		::rux::gui::ColorBase* Colors::ButtonOverColor( void )
		{	
			::rux::gui::LinearGradientColor* linear_gradient = alloc_object_macros( ::rux::gui::LinearGradientColor );
			linear_gradient->set_LinearGradientType( ::rux::gui::XEnum_LinearGradientType_Vertical );
			linear_gradient->AddGradientStop( ::rux::gui::GradientStop( ::rux::gui::Color( 0x94 , 0x98 , 0x9A , 255 ) , 0.0f ) );
			linear_gradient->AddGradientStop( ::rux::gui::GradientStop( ::rux::gui::Color( 0x7B , 0x7D , 0x7E , 255 ) , 0.499f ) );	
			linear_gradient->AddGradientStop( ::rux::gui::GradientStop( ::rux::gui::Color( 0x69 , 0x6C , 0x6D , 255 ) , 0.001f ) );	
			linear_gradient->AddGradientStop( ::rux::gui::GradientStop( ::rux::gui::Color( 0x51 , 0x56 , 0x58 , 255 ) , 0.5f ) );
			return linear_gradient;
		};
		::rux::gui::ColorBase* Colors::ButtonAlertDownColor( void )
		{
			::rux::gui::LinearGradientColor* linear_gradient = alloc_object_macros( ::rux::gui::LinearGradientColor );
			linear_gradient->set_LinearGradientType( ::rux::gui::XEnum_LinearGradientType_Vertical );
			linear_gradient->AddGradientStop( ::rux::gui::GradientStop( ::rux::gui::Color( 204 , 51 , 51 , 255 ) , 0.0f ) );
			linear_gradient->AddGradientStop( ::rux::gui::GradientStop( ::rux::gui::Color( 204 , 51 , 51 , 255 ) , 0.499f ) );	
			linear_gradient->AddGradientStop( ::rux::gui::GradientStop( ::rux::gui::Color( 153 , 0 , 0 , 255 ) , 0.001f ) );	
			linear_gradient->AddGradientStop( ::rux::gui::GradientStop( ::rux::gui::Color( 153 , 0 , 0 , 255 ) , 0.5f ) );
			return linear_gradient;
		};
		::rux::gui::ColorBase* Colors::ButtonAlertOverColor( void )
		{
			::rux::gui::LinearGradientColor* linear_gradient = alloc_object_macros( ::rux::gui::LinearGradientColor );
			linear_gradient->set_LinearGradientType( ::rux::gui::XEnum_LinearGradientType_Vertical );		
			linear_gradient->AddGradientStop( ::rux::gui::GradientStop( ::rux::gui::Color( 0xFB , 0x63 , 0x63 , 255 ) , 0.0f ) );
			linear_gradient->AddGradientStop( ::rux::gui::GradientStop( ::rux::gui::Color( 0xd4 , 0x2c , 0x2c , 255 ) , 0.499f ) );	
			linear_gradient->AddGradientStop( ::rux::gui::GradientStop( ::rux::gui::Color( 0xc1 , 0x14 , 0x14 , 255 ) , 0.001f ) );	
			linear_gradient->AddGradientStop( ::rux::gui::GradientStop( ::rux::gui::Color( 0x82 , 0x04 , 0x04 , 255 ) , 0.5f ) );
			return linear_gradient;
		};
		::rux::gui::ColorBase* Colors::ButtonNormalColor( void )
		{
			::rux::gui::LinearGradientColor* linear_gradient = alloc_object_macros( ::rux::gui::LinearGradientColor );
			linear_gradient->set_LinearGradientType( ::rux::gui::XEnum_LinearGradientType_Vertical );	
			linear_gradient->AddGradientStop( ::rux::gui::GradientStop( ::rux::gui::Color( 0x97 , 0x9B , 0x9D , 255 ) , 0.0f ) );
			linear_gradient->AddGradientStop( ::rux::gui::GradientStop( ::rux::gui::Color( 0x67 , 0x6A , 0x6C , 255 ) , 0.499f ) );	
			linear_gradient->AddGradientStop( ::rux::gui::GradientStop( ::rux::gui::Color( 0x55 , 0x5A , 0x5D , 255 ) , 0.001f ) );	
			linear_gradient->AddGradientStop( ::rux::gui::GradientStop( ::rux::gui::Color( 0x3B , 0x40 , 0x44 , 255 ) , 0.5f ) );
			return linear_gradient;
		};
		::rux::gui::ColorBase* Colors::HorizontalButtonNormalColor( void )
		{
			::rux::gui::LinearGradientColor* linear_gradient = alloc_object_macros( ::rux::gui::LinearGradientColor );
			linear_gradient->set_LinearGradientType( ::rux::gui::XEnum_LinearGradientType_Horizontal );	
			linear_gradient->AddGradientStop( ::rux::gui::GradientStop( ::rux::gui::Color( 0x3B , 0x40 , 0x44 , 255 ) , 0.0f ) );
			linear_gradient->AddGradientStop( ::rux::gui::GradientStop( ::rux::gui::Color( 0x55 , 0x5A , 0x5D , 255 ) , 0.499f ) );	
			linear_gradient->AddGradientStop( ::rux::gui::GradientStop( ::rux::gui::Color( 0x67 , 0x6A , 0x6C , 255 ) , 0.001f ) );	
			linear_gradient->AddGradientStop( ::rux::gui::GradientStop( ::rux::gui::Color( 0x97 , 0x9B , 0x9D , 255 ) , 0.5f ) );
			return linear_gradient;
		};
		::rux::gui::ColorBase* Colors::ButtonDownColor( float opacity )
		{
			::rux::gui::LinearGradientColor* linear_gradient = alloc_object_macros( ::rux::gui::LinearGradientColor );
			linear_gradient->set_LinearGradientType( ::rux::gui::XEnum_LinearGradientType_Vertical );
			linear_gradient->AddGradientStop( ::rux::gui::GradientStop( ::rux::gui::Color( 0x85 , 0xDE , 0xF9 , (rux::byte)( 255.f * opacity ) ) , 0.0 ) );
			linear_gradient->AddGradientStop( ::rux::gui::GradientStop( ::rux::gui::Color( 0x42 , 0xB6 , 0xE4 , (rux::byte)( 255.f * opacity ) ) , 0.499f ) );	
			linear_gradient->AddGradientStop( ::rux::gui::GradientStop( ::rux::gui::Color( 0x29 , 0xA0 , 0xD2 , (rux::byte)( 255.f * opacity ) ) , 0.001f ) );	
			linear_gradient->AddGradientStop( ::rux::gui::GradientStop( ::rux::gui::Color( 0x19 , 0x78 , 0xAF , (rux::byte)( 255.f * opacity ) ) , 0.5f ) );
			return linear_gradient;
		};
		::rux::gui::ColorBase* Colors::ButtonDisabledColor( void )
		{
			::rux::gui::LinearGradientColor* linear_gradient = alloc_object_macros( ::rux::gui::LinearGradientColor );
			linear_gradient->set_LinearGradientType( ::rux::gui::XEnum_LinearGradientType_Vertical );
			linear_gradient->AddGradientStop( ::rux::gui::GradientStop( ::rux::gui::Color( 0x42 , 0x45 , 0x47 , 255 ) , 0.27f ) );
			linear_gradient->AddGradientStop( ::rux::gui::GradientStop( ::rux::gui::Color( 0x28 , 0x2A , 0x2C , 255 ) , 0.73f ) );		
			return linear_gradient;
		};
		::rux::gui::ColorBase* Colors::WindowBackgroundColor( const char* __file__ , ::rux::int32 __line__ )
		{
			return ::rux::engine::alloc_object< ::rux::gui::Color >( (::rux::uint8)0x30 , (::rux::uint8)0x32 , (::rux::uint8)0x34 , (::rux::uint8)255 , __file__ ? (const char*)__file__ : __FILE__ , __file__ ? (::rux::uint32)__line__ : (::rux::uint32)__LINE__, (char*)NULL , (::rux::malloc_t)NULL );
		};
		::rux::gui::ColorBase* Colors::GlyphColor( void )
		{
			return create_color( 0x04 , 0x04 , 0x04 , 51 );
		};
		::rux::gui::ColorBase* Colors::WindowHeaderBackgroundColor( void )
		{
			::rux::gui::LinearGradientColor* linear_gradient = alloc_object_macros( ::rux::gui::LinearGradientColor );
			linear_gradient->set_LinearGradientType( ::rux::gui::XEnum_LinearGradientType_Vertical );
			linear_gradient->AddGradientStop( ::rux::gui::GradientStop( ::rux::gui::Color( 0x2B , 0x2F , 0x32 , 255 ) , 0.0 ) );
			linear_gradient->AddGradientStop( ::rux::gui::GradientStop( ::rux::gui::Color( 0x45 , 0x49 , 0x4B , 255 ) , 1.0 ) );		
			return linear_gradient;
		};
		::rux::gui::Color* Colors::CellBorderColor( void )
		{
			return create_color( 0x3C , 0x41 , 0x45 , 0xFF );
		};
		::rux::gui::ColorBase* Colors::CellBackgroundColor( void )
		{
			::rux::gui::LinearGradientColor* linear_gradient = alloc_object_macros( ::rux::gui::LinearGradientColor );
			linear_gradient->set_LinearGradientType( ::rux::gui::XEnum_LinearGradientType_Vertical );
			linear_gradient->AddGradientStop( ::rux::gui::GradientStop( ::rux::gui::Color( 0x2C , 0x2E , 0x24 , 255 ) , 0.004f ) );
			linear_gradient->AddGradientStop( ::rux::gui::GradientStop( ::rux::gui::Color( 0x1F , 0x22 , 0x24 , 255 ) , 1.0f ) );		
			return linear_gradient;
		};
		Thickness::Thickness( void )
		{
			_left_down_corner = 0.0;
			_left_up_corner = 0.0;	
			_right_down_corner = 0.0;
			_right_up_corner = 0.0;
		};
		Thickness::Thickness( float left_down_corner , float left_up_corner , float right_down_corner , float right_up_corner )
		{
			_left_down_corner = left_down_corner;
			_left_up_corner = left_up_corner;
			_right_down_corner = right_down_corner;
			_right_up_corner = right_up_corner;
		};
		Thickness::Thickness( const Thickness& thickness )
		{
			_left_down_corner = thickness._left_down_corner;
			_left_up_corner = thickness._left_up_corner;
			_right_down_corner = thickness._right_down_corner;
			_right_up_corner = thickness._right_up_corner;
		};
		Thickness& Thickness::operator =( const Thickness& thickness )
		{
			_left_down_corner = thickness._left_down_corner;
			_left_up_corner = thickness._left_up_corner;
			_right_down_corner = thickness._right_down_corner;
			_right_up_corner = thickness._right_up_corner;
			return *this;
		};
		bool Thickness::operator !=( const Thickness& thickness )
		{
			return _left_down_corner != thickness._left_down_corner
				|| _left_up_corner != thickness._left_up_corner
				|| _right_down_corner != thickness._right_down_corner
				|| _right_up_corner != thickness._right_up_corner;
		};		
		Margin::Margin( void )
		{
			_left_offset = 0.0f;
			_right_offset = 0.0f;	
			_top_offset = 0.0f;
			_bottom_offset = 0.0f;
		};
		Margin::Margin( float left_offset , float right_offset , float top_offset , float bottom_offset )
		{
			_left_offset = left_offset;
			_right_offset = right_offset;
			_top_offset = top_offset;
			_bottom_offset = bottom_offset;
		};
		Margin::Margin( const Margin& margin )
		{
			_left_offset = margin._left_offset;
			_right_offset = margin._right_offset;
			_top_offset = margin._top_offset;
			_bottom_offset = margin._bottom_offset;
		};
		Margin& Margin::operator =( const Margin& margin )
		{
			_left_offset = margin._left_offset;
			_right_offset = margin._right_offset;
			_top_offset = margin._top_offset;
			_bottom_offset = margin._bottom_offset;
			return *this;
		};
		bool Margin::operator !=( const Margin& margin )
		{
			return _left_offset != margin._left_offset
				|| _right_offset != margin._right_offset
				|| _top_offset != margin._top_offset
				|| _bottom_offset != margin._bottom_offset;
		};		
		Point::Point( void )
		{
			_point_unit = XEnum_PointUnit_Percent;
		};		
		Point::Point( float left , float top , ::rux::gui::XEnum_PointUnit point_unit )
		{
			_x = left;
			_y = top;
			_point_unit = point_unit;
		};
		Point::Point( const Point& point )
		{
			::rux::gui::Point* p = (::rux::gui::Point*)&point;
			_x = p->_x;
			_y = p->_y;
			_point_unit = p->_point_unit;
		};
		Point& Point::operator =( const Point& point )
		{
			::rux::gui::Point* p = (::rux::gui::Point*)&point;
			_x = p->_x;
			_y = p->_y;
			_point_unit = p->_point_unit;
			return *this;
		};
		Rectangle::Rectangle( void )
		{
			_left = 0.f , _top = 0.f , _width = 0.f , _height = 0.f;
		};
		Rectangle::Rectangle( ::rux::int32 left , ::rux::int32 top , ::rux::int32 width , ::rux::int32 height )
		{
			_left = left , _top = top , _width = width , _height = height;
		};
		Rectangle& Rectangle::operator =( const Rectangle& rectangle )
		{
			_left = rectangle._left , _top = rectangle._top , _width = rectangle._width , _height = rectangle._height;
			return *this;
		};
		::rux::uint8 Rectangle::IsIntersects( ::rux::gui::Rectangle& rectangle )
		{
			float right = _left + _width;
			float bottom = _top + _height;
			float rectangle_right = rectangle._left + rectangle._width;
			float rectangle_bottom = rectangle._top + rectangle._height;
			return ( _width + rectangle._width ) > ::std::max< float >( right , rectangle_right ) - ::std::min< float >( _left , rectangle._left )
				&& ( _height + rectangle._height ) > ::std::max< float >( bottom , rectangle_bottom ) - ::std::min< float >( _top , rectangle._top ) ? 1 : 0;
		};
		::rux::uint8 Rectangle::IsPointInside( ::rux::int32 x , ::rux::int32 y )
		{
			return _left <= x && _top <= y
				&& _left + _width >= x
				&& _top + _height >= y ? 1 : 0;
		};
	};
};