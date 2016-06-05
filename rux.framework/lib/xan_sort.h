#ifndef XAN_SORT_H
#define XAN_SORT_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_dataobject.h>
namespace rux
{
	namespace data
	{
		class sort
		{
		public:
			static void bubble_sort( rux::XArray< XObject >& elements , const char* field_name , const char* field_name_delimiter = "->" );
			static void bubble_sort( rux::XArray< XObject >& elements , const rux::XString& field_name , const char* field_name_delimiter = "->" );
		};
	};
};
#endif
