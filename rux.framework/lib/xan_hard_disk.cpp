#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_hard_disk.h>
#ifdef __UNIX__
#include <fcntl.h>
#include <linux/hdreg.h>
#endif
namespace rux
{
	namespace io
	{
		bool hard_disk::get_serial_number( size_t hard_disk_index , char* serial_number , size_t serial_number_size )
		{
			bool res = false;
			serial_number[ 0 ] = 0;
#ifdef __WINDOWS__
			char hard_disk_str[ 1024 ] = {0};
			::rux::safe_sprintf( hard_disk_str , 1024 , "\\\\.\\PhysicalDrive%u" , hard_disk_index );
			HANDLE device = ::CreateFileA( hard_disk_str , 0 , FILE_SHARE_READ | FILE_SHARE_WRITE , NULL , OPEN_EXISTING , 0 , NULL );
			if( INVALID_HANDLE_VALUE != device )
			{
				STORAGE_PROPERTY_QUERY storage_property_query;
				ZeroMemory( &storage_property_query , sizeof( STORAGE_PROPERTY_QUERY ) );
				storage_property_query.PropertyId = StorageDeviceProperty;
				storage_property_query.QueryType = PropertyStandardQuery;
				STORAGE_DESCRIPTOR_HEADER storage_descriptor_header = { 0 };
				DWORD bytes_returned = 0;
				if( ::DeviceIoControl( device , IOCTL_STORAGE_QUERY_PROPERTY , &storage_property_query , sizeof( STORAGE_PROPERTY_QUERY ) , &storage_descriptor_header , sizeof( STORAGE_DESCRIPTOR_HEADER ) , &bytes_returned , NULL ) )
				{
					const DWORD buffer_size = storage_descriptor_header.Size;
					::rux::byte* buffer = alloc_array_macros( ::rux::byte , buffer_size );
					ZeroMemory( buffer , buffer_size );
					if( ::DeviceIoControl( device , IOCTL_STORAGE_QUERY_PROPERTY , &storage_property_query , sizeof( STORAGE_PROPERTY_QUERY ) , buffer , buffer_size , &bytes_returned , NULL ) )
					{
						STORAGE_DEVICE_DESCRIPTOR* device_descriptor = (STORAGE_DEVICE_DESCRIPTOR*)buffer;
						const DWORD serial_number_offset = device_descriptor->SerialNumberOffset;
						if( serial_number_offset != 0 )
						{
							char* str = (char*)( buffer + serial_number_offset );
							::rux::safe_strncpy( serial_number , str , serial_number_size );
							res = true;
						}
					}
					::rux::engine::free_mem( buffer );
				}
				::CloseHandle( device );
			}
#else
			/*struct hd_driveid hd;
int ide;
ide=open("/dev/hda",O_RDONLY);
(void)ioctl(ide,HDIO_GET_IDENTITY,&hd);
(void)printf("Serial number - %s\n",hd.serial_no);
return (0);*/
#endif
			return res;
		};
	};
};