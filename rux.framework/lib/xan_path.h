#ifndef XAN_PATH_H
#define XAN_PATH_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_defines.h>
#include <xan_string.h>
namespace rux
{
	namespace io
	{
		class XPath
		{
		private:
		public:	
			static void GetDirectoryName( char* file_name_ptr , rux::alloc_t alloc , char** directory_name_ptr_ptr );
			static rux::String& GetDirectoryName( const rux::XString& file_name );
			static rux::String& GetDirectoryName( const char* source_ptr );	
			static rux::String& GetFileName( const char* path_ptr );
			static rux::String& GetFileName( const rux::XString& path_ptr );
			static rux::String& GetFileNameWithoutExtension( const char* file_name_ptr );	
			static void GetFileNameWithoutExtension( const wchar_t* file_name_ptr , rux::alloc_t alloc , wchar_t** file_name_without_extension );	
			static void GetFileNameWithoutExtension( const rux::XString& file_name , rux::alloc_t alloc , wchar_t** file_name_without_extension );	
			static rux::String& GetFileNameWithoutExtension( const rux::XString& path );	
			static rux::String& GetFileNameExtension( const rux::XString& path );	
			static rux::String& ExcludeRelative( const rux::XString& path );
			static rux::String& CalculateRelative( const rux::XString& relative_path , const rux::XString& path );
			static rux::String& CalculateAbsolute( const rux::XString& working_path , const rux::XString& path );
			static rux::uint8 IsAbsolutePath( const rux::XString& path );
			static rux::uint8 IsAbsolutePath( const char* path , size_t path_length = SIZE_MAX );
			static const char* get_Alias( void );
		};
	};
};
#endif
