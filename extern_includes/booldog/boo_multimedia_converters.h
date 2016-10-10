#ifndef BOO_MULTIMEDIA_CONVERTERS_H
#define BOO_MULTIMEDIA_CONVERTERS_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#ifndef BOOLDOG_HEADER
#define BOOLDOG_HEADER( header ) <header>
#endif
#include BOOLDOG_HEADER(boo_result.h)
#include BOOLDOG_HEADER(boo_error.h)
#define booclip(x) x > 0 ? x < 255 ? x : 255 : 0
namespace booldog
{
	namespace converters
	{
		namespace multimedia
		{
			namespace image
			{
				namespace yuy2
				{
					booinline bool to_rgb24(::booldog::result_buffer* pres, booldog::allocator* allocator
						, const ::booldog::byte* src, ::booldog::uint32 width, ::booldog::uint32 height
						, const ::booldog::debug::info& debuginfo = debuginfo_macros )
					{
						::booldog::result_buffer locres( allocator );
						BOOINIT_RESULT( ::booldog::result_buffer );

						::booldog::uint32 temp = width * height;
						res->bufdatasize = 3 * temp;
						::booldog::uint32 pixels_count = 2 * temp;
						::booldog::int32 index1 = 0, r_coef = 0, g_coef = 0, b_coef = 0, D = 0, C = 0;
						if(res->bufdatasize > res->bufsize)
						{
							res->bufsize = res->bufdatasize;
							res->buf = res->allocator->realloc_array< unsigned char >(res->buf, res->bufsize, debuginfo);
							if(res->buf == 0)
							{
								res->booerr(::booldog::enums::result::booerr_type_cannot_alloc_memory);
								goto goto_return;
							}
						}
						for(::booldog::uint32 index0 = 0;index0 < pixels_count;index0 += 4)
						{						
							C = 298 * (src[index0] - 16);//Y - 16					
							D = src[index0 + 1] - 128;				//U - 128
							r_coef = 409 * (src[index0 + 3] - 128) + 128;//409 * ( V - 128 ) + 128
							g_coef = -100 * D - 208 * (src[index0 + 3] - 128) + 128;//- 100 * D - 208 * ( V - 128 ) + 128
							b_coef = 516 * D + 128;
							res->buf[index1] = (::booldog::byte)(booclip((C + b_coef) >> 8));
							res->buf[++index1] = (::booldog::byte)(booclip((C + g_coef) >> 8));
							res->buf[++index1] = (::booldog::byte)(booclip((C + r_coef) >> 8));
												
							C = 298 * (src[index0 + 2] - 16);				
							res->buf[++index1] = (::booldog::byte)(booclip((C + b_coef) >> 8));
							res->buf[++index1] = (::booldog::byte)(booclip((C + g_coef) >> 8));
							res->buf[++index1] = (::booldog::byte)(booclip((C + r_coef) >> 8));						
							++index1;
						}
		goto_return:
						return res->succeeded();
					};
				};
			};
		};
	};
};
#endif
