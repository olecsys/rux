#ifndef XAN_HARD_DISK_H
#define XAN_HARD_DISK_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_string.h>
namespace rux
{
	namespace io
	{
		class hard_disk
		{
		public:
			static bool get_serial_number( size_t hard_disk_index , char* serial_number , size_t serial_number_size );
		};
	};
};
#endif
