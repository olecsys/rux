#ifndef XAN_ARRAY_SORT_H
#define XAN_ARRAY_SORT_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_diagnostic_defines.h>
namespace rux
{
	namespace data
	{
		class array_sort
		{
		public:
			template< class T >
			static void bubble_sort( T* elements , size_t elements_count )
			{
				uint8 swapped = 0;
				if( elements_count > 0 )
				{
					for( size_t index0 = 0 ; index0 < elements_count - 1 ; index0++ )
					{
						swapped = 0;
						for( size_t index1 = 0 ; index1 < elements_count - index0 - 1 ; index1++ )
						{						
							T elem0 = elements[ index1 ];
							T elem1 = elements[ index1 + 1 ];
							if( elem0 > elem1 )
							{
								elements[ index1 ] = elem1;
								elements[ index1 + 1 ] = elem0;
								swapped = 1;
							}
						}
						if( swapped == 0 )
							break;
					}
				}
			};
		};
	};
};
#endif
