#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_fileversion.h>
#include <xan_keyvaluepair.h>
#include <xan_uint64.h>
#include <xan_boolean.h>
begin_implement_rux_class_ns( FileVersion , rux::diagnostics );
	_major = 0;
	_minor = 0;
	_build = 0;
	_revision = 0;
end_implement_rux_class();
namespace rux
{
	namespace diagnostics
	{
		FileVersion& XFileVersion::get_FileVersion( const rux::XString& file_name , rux::XString& error )
		{
			error.Clear();
			XFileVersion file_version;
			if( file_name.Length() > 0 )
			{
		#ifdef __WINDOWS__
				if( file_name()->_code_page == XEnumCodePage_UTF16 )
				{
					uint32 file_version_size = 0;
					unsigned long dummy = 0;
					file_version_size = GetFileVersionInfoSizeW( (wchar_t*)file_name.str() , &dummy );
					if( file_version_size > 0 )
					{
						wchar_t* buffer_ptr = alloc_array_macros( wchar_t , file_version_size );
						if( GetFileVersionInfoW( (wchar_t*)file_name.str() , NULL , file_version_size , (void*)buffer_ptr ) != 0 )
						{
							VS_FIXEDFILEINFO* version = NULL;
							uint32 length = 0;
							if( VerQueryValueW( buffer_ptr , L"\\" , (void**)&version , &length ) != 0 )
							{
								if( version && length == sizeof( VS_FIXEDFILEINFO ) )
								{
									file_version()->_major = HIWORD( version->dwFileVersionMS );
									file_version()->_minor = LOWORD( version->dwFileVersionMS );
									file_version()->_revision = HIWORD( version->dwFileVersionLS );
									file_version()->_build = LOWORD( version->dwFileVersionLS );
								}
								else 						
									error = "file '" + file_name + "' has not a file version";
							}				
							else 
								error = "file '" + file_name + "' has not a file version";
						}
						else 
							error = "file '" + file_name + "' has not a file version";
						rux::engine::free_mem( buffer_ptr );
					}
					else 
						error = "file '" + file_name + "' has not a file version";
				}
				else
				{
					uint32 file_version_size = 0;
					unsigned long dummy = 0;
					file_version_size = GetFileVersionInfoSizeA( file_name.str() , &dummy );
					if( file_version_size > 0 )
					{
						char* buffer_ptr = alloc_array_macros( char , file_version_size );
						if( GetFileVersionInfoA( file_name.str() , NULL , file_version_size , (void*)buffer_ptr ) != 0 )
						{
							VS_FIXEDFILEINFO* version = NULL;
							uint32 length = 0;
							if( VerQueryValueA( buffer_ptr , "\\" , (void**)&version , &length ) != 0 )
							{
								if( version && length == sizeof( VS_FIXEDFILEINFO ) )
								{
									file_version()->_major = HIWORD( version->dwFileVersionMS );
									file_version()->_minor = LOWORD( version->dwFileVersionMS );
									file_version()->_revision = HIWORD( version->dwFileVersionLS );
									file_version()->_build = LOWORD( version->dwFileVersionLS );
								}
								else 
									error = rux::XString::Format( "file '%s' has not a file version" , file_name.str() );
							}				
							else 
								error = rux::XString::Format( "file '%s' has not a file version" , file_name.str() );
						}
						else 
							error = rux::XString::Format( "file '%s' has not a file version" , file_name.str() );
						rux::engine::free_mem( buffer_ptr );
					}
					else 
						error = rux::XString::Format( "file '%s' has not a file version" , file_name.str() );
				}
		#endif
			}
			return file_version++;
		};
		FileVersion& XFileVersion::get_FileVersion( char* file_name_ptr , rux::XString& error )
		{
			error.Clear();
			XFileVersion file_version;
			if( file_name_ptr && rux::XString::LengthA( file_name_ptr ) > 0 )
			{
		#ifdef __WINDOWS__
				uint32 file_version_size = 0;
				unsigned long dummy = 0;
				file_version_size = GetFileVersionInfoSize( file_name_ptr , &dummy );
				if( file_version_size > 0 )
				{
					char* buffer_ptr = alloc_array_macros( char , file_version_size );
					if( GetFileVersionInfo( file_name_ptr , NULL , file_version_size , (void*)buffer_ptr ) != 0 )
					{
						VS_FIXEDFILEINFO* version = NULL;
						uint32 length = 0;
						if( VerQueryValue( buffer_ptr , "\\" , (void**)&version , &length ) != 0 )
						{
							if( version && length == sizeof( VS_FIXEDFILEINFO ) )
							{
								file_version()->_major = HIWORD( version->dwFileVersionMS );
								file_version()->_minor = LOWORD( version->dwFileVersionMS );
								file_version()->_revision = HIWORD( version->dwFileVersionLS );
								file_version()->_build = LOWORD( version->dwFileVersionLS );
							}
							else 
								error = rux::XString::Format( "file '%s' has not a file version" , file_name_ptr );
						}				
						else 
							error = rux::XString::Format( "file '%s' has not a file version" , file_name_ptr );
					}
					else 
						error = rux::XString::Format( "file '%s' has not a file version" , file_name_ptr );
					rux::engine::free_mem( buffer_ptr );
				}
				else 
					error = rux::XString::Format( "file '%s' has not a file version" , file_name_ptr );
		#endif
			}
			return file_version++;
		};
		FileVersion& XFileVersion::get_FileVersion( const rux::XString& file_name )
		{	
			XFileVersion file_version;
			if( file_name.Length() > 0 )
			{
		#ifdef __WINDOWS__
				if( file_name()->_code_page == XEnumCodePage_UTF16 )
				{
					uint32 file_version_size = 0;
					unsigned long dummy = 0;
					file_version_size = GetFileVersionInfoSizeW( (wchar_t*)file_name.str() , &dummy );
					if( file_version_size > 0 )
					{
						wchar_t* buffer_ptr = alloc_array_macros( wchar_t , file_version_size );
						if( GetFileVersionInfoW( (wchar_t*)file_name.str() , NULL , file_version_size , (void*)buffer_ptr ) != 0 )
						{
							VS_FIXEDFILEINFO* version = NULL;
							uint32 length = 0;
							if( VerQueryValueW( buffer_ptr , L"\\" , (void**)&version , &length ) != 0 )
							{
								if( version && length == sizeof( VS_FIXEDFILEINFO ) )
								{
									file_version()->_major = HIWORD( version->dwFileVersionMS );
									file_version()->_minor = LOWORD( version->dwFileVersionMS );
									file_version()->_revision = HIWORD( version->dwFileVersionLS );
									file_version()->_build = LOWORD( version->dwFileVersionLS );
								}						
							}									
						}				
						rux::engine::free_mem( buffer_ptr );
					}			
				}
				else
				{
					uint32 file_version_size = 0;
					unsigned long dummy = 0;
					file_version_size = GetFileVersionInfoSizeA( file_name.str() , &dummy );
					if( file_version_size > 0 )
					{
						char* buffer_ptr = alloc_array_macros( char , file_version_size );
						if( GetFileVersionInfoA( file_name.str() , NULL , file_version_size , (void*)buffer_ptr ) != 0 )
						{
							VS_FIXEDFILEINFO* version = NULL;
							uint32 length = 0;
							if( VerQueryValueA( buffer_ptr , "\\" , (void**)&version , &length ) != 0 )
							{
								if( version && length == sizeof( VS_FIXEDFILEINFO ) )
								{
									file_version()->_major = HIWORD( version->dwFileVersionMS );
									file_version()->_minor = LOWORD( version->dwFileVersionMS );
									file_version()->_revision = HIWORD( version->dwFileVersionLS );
									file_version()->_build = LOWORD( version->dwFileVersionLS );
								}						
							}									
						}				
						rux::engine::free_mem( buffer_ptr );
					}			
				}
		#endif
			}
			return file_version++;
		};
		FileVersion& XFileVersion::get_FileVersion( char* file_name_ptr )
		{
			XFileVersion file_version;
			if( file_name_ptr && rux::XString::LengthA( file_name_ptr ) > 0 )
			{
		#ifdef __WINDOWS__
				uint32 file_version_size = 0;
				unsigned long dummy = 0;
				file_version_size = GetFileVersionInfoSize( file_name_ptr , &dummy );
				if( file_version_size > 0 )
				{
					char* buffer_ptr = alloc_array_macros( char , file_version_size );
					if( GetFileVersionInfo( file_name_ptr , NULL , file_version_size , (void*)buffer_ptr ) != 0 )
					{
						VS_FIXEDFILEINFO* version = NULL;
						uint32 length = 0;
						if( VerQueryValue( buffer_ptr , "\\" , (void**)&version , &length ) != 0 )
						{
							if( version && length == sizeof( VS_FIXEDFILEINFO ) )
							{
								file_version()->_major = HIWORD( version->dwFileVersionMS );
								file_version()->_minor = LOWORD( version->dwFileVersionMS );
								file_version()->_revision = HIWORD( version->dwFileVersionLS );
								file_version()->_build = LOWORD( version->dwFileVersionLS );
							}
						}				
					}
					rux::engine::free_mem( buffer_ptr );
				}
		#endif
			}
			return file_version++;
		};
		uint8 XFileVersion::IsEmpty( void )
		{
			return (*this)()->_major == 0 && (*this)()->_minor == 0 && (*this)()->_build == 0 && (*this)()->_revision == 0;
		};
		uint32 XFileVersion::get_Major( void )
		{
			return (*this)()->_major;
		};
		uint32 XFileVersion::get_Minor( void )
		{
			return (*this)()->_minor;
		};
		uint32 XFileVersion::get_Build( void )
		{
			return (*this)()->_build;
		};
		uint32 XFileVersion::get_Revision( void )
		{
			return (*this)()->_revision;
		};
	};
};