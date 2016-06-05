#ifndef XAN_ZIP_H
#define XAN_ZIP_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_string.h>
class XZip
{
public:
	static void Compress( const rux::XArray< rux::XString >& files_or_directories , const rux::XString& zip_file_name , rux::uint8 abscence_is_not_error , rux::XString& error );
	static void Compress( const rux::XString& file_or_directory_name , const rux::XString& zip_file_name , rux::XString& error );
	static void UnCompress( const rux::XString& destination_directory_name , const rux::XString& zip_file_name , rux::XString& error );
};
#endif
