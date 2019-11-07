#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_directory.h"
#include "xan_error.h"
#ifdef __WINDOWS__
#include <AclAPI.h>
#else
#include <pwd.h>
#endif
#include "xan_uint32.h"
#include "xan_keyvaluepair.h"
#include "xan_error.h"
#include "xan_path.h"
#include "xan_uint64.h"
#include "xan_boolean.h"
#include "xan_memory_helper.h"
begin_implement_rux_class_ns( Directory , rux::io );
	_on_directory_changed = NULL;
end_implement_rux_class();
namespace rux
{
	namespace io
	{
		Directory::~Directory()
		{
			CRITICAL_SECTION_LOCK( _cs_watcher_thread );
			if( _watcher_thread.get_IsStarted() == 1 )
				_watcher_thread.Stop();
			_cs_watcher_thread.UnLock();
		};
		implement_duplicate_internal_result_function( rux::uint8 , Directory , Delete );
		implement_duplicate_internal_result_function( rux::XString& , Directory , get_DirectoryName );
		implement_duplicate_internal_result_function( rux::Array<XFile>& , Directory , GetFiles );
		implement_duplicate_internal_result_function( rux::Array<rux::io::XDirectory>& , Directory , GetDirectories );
		XDirectory::XDirectory( const rux::XString& directory_name )
		{
			(*this)()->_directory_name = directory_name;
		};
		rux::uint8 rux::io::XDirectory::CreateLink( const rux::XString& shortcut_path , const rux::XString& path_to_link, const rux::XString& description , const rux::XString& arguments , const rux::XString& working_directory ) 
		{ 
			rux::uint8 result = 0;
		#ifdef __WINDOWS__
			IShellLinkW* shell_link_ptr = NULL; 	
			HRESULT hresult = CoInitialize( NULL );
			rux::uint8 is_already_initialized = hresult == S_FALSE ? 1 : 0;
			hresult = CoCreateInstance( CLSID_ShellLink , NULL , CLSCTX_INPROC_SERVER , IID_IShellLinkW , (void**)&shell_link_ptr );
			if( SUCCEEDED( hresult ) ) 
			{ 
				rux::XString utf16_shortcut_path , utf16_description , utf16_arguments , utf16_working_directory;
				utf16_shortcut_path.set_ByRef( shortcut_path.ConvertToUTF16() );
				utf16_description.set_ByRef( description.ConvertToUTF16() );
				utf16_arguments.set_ByRef( arguments.ConvertToUTF16() );
				utf16_working_directory.set_ByRef( working_directory.ConvertToUTF16() );
				IPersistFile* persist_file_ptr = NULL;
				if( utf16_arguments.Length() > 0 )
					shell_link_ptr->SetArguments( (wchar_t*)utf16_arguments.str() );
				if( utf16_working_directory.Length() > 0 )
					shell_link_ptr->SetWorkingDirectory( (wchar_t*)utf16_working_directory.str() );
				shell_link_ptr->SetPath( (wchar_t*)utf16_shortcut_path.str() ); 
				shell_link_ptr->SetDescription( (wchar_t*)utf16_description.str() );
				hresult = shell_link_ptr->QueryInterface( IID_IPersistFile , (void**)&persist_file_ptr ); 
				if( SUCCEEDED( hresult ) ) 
				{
					rux::XString utf16_path_to_link;
					utf16_path_to_link.set_ByRef( path_to_link.ConvertToUTF16() );
					hresult = persist_file_ptr->Save( (wchar_t*)utf16_path_to_link.str() , TRUE ); 
					if( SUCCEEDED( hresult ) )
						result = 1;
					persist_file_ptr->Release(); 
				} 
				shell_link_ptr->Release(); 
			} 
			if( is_already_initialized == 0 )
				CoUninitialize();
		#endif
			return result; 
		};
		rux::String& rux::io::XDirectory::get_SpecialFolderPath( const char* type_ptr , rux::XString& error)
		{
			error.Clear();
			rux::XString string( type_ptr );
#ifdef __WINDOWS__
			rux::int32 folder = CSIDL_PERSONAL;
			declare_stack_variable( char , documents_path_ptr , MAX_PATH );
			if( string == "MYDOCUMENTS" )
				folder = CSIDL_PERSONAL;
			else if( string == "SYSTEM" )
				folder = CSIDL_SYSTEM;
			else if( string == "WINDOWS" )
				folder = CSIDL_WINDOWS;
			else if( string == "PROGRAM FILES" )
				folder = CSIDL_PROGRAM_FILES;
			else if( string == "START_MENU_PROGRAMS" )
				folder = CSIDL_COMMON_PROGRAMS;
			if( SUCCEEDED( ::SHGetFolderPath( NULL , folder , NULL , SHGFP_TYPE_CURRENT , documents_path_ptr ) ) )
			{
				string = documents_path_ptr;
				string.set_ByRef( string.ReplaceAll( '\\' , '/' ) );
				if( string.get_UTF8Char( string.Length() - 1 ) == '/' )
					string.RemoveAt( string.Length() - 1 );
			}
			else 
				error = "folder '" + string + "' does not exists";
#else
			if( string == "MYDOCUMENTS" )
			{
				struct ::passwd pwent;
				struct ::passwd* pwentp = NULL;
				char buf[ 1024 ] = { 0 };
				::rux::int32 res = 0;
				if( ( res = getpwuid_r( getuid() , &pwent , buf , sizeof( buf ) , &pwentp ) ) )
					error = "getpwuid_r return error '" + ::rux::XInt32( res ).ToString() + "'";
				else
				{
					string =  pwent.pw_dir;
					string.set_ByRef( string.ReplaceAll( '\\' , '/' ) );
					if( string.get_UTF8Char( string.Length() - 1 ) == '/' )
						string.RemoveAt( string.Length() - 1 );
				}
			}
#endif
			return string++;
		};
		rux::String& rux::io::XDirectory::get_SpecialFolderPath( const char* type_ptr )
		{
			rux::XString string( type_ptr );
		#ifdef __WINDOWS__
			rux::int32 folder = CSIDL_PERSONAL;
			declare_stack_variable( char , documents_path_ptr , MAX_PATH );
			if( string == "MYDOCUMENTS" )
				folder = CSIDL_PERSONAL;
			else if( string == "SYSTEM" )
				folder = CSIDL_SYSTEM;
			else if( string == "WINDOWS" )
				folder = CSIDL_WINDOWS;
			else if( string == "PROGRAM FILES" )
				folder = CSIDL_PROGRAM_FILES;
			if( SUCCEEDED( ::SHGetFolderPath( NULL , folder , NULL , SHGFP_TYPE_CURRENT , documents_path_ptr ) ) )
			{
				string = documents_path_ptr;
				string.set_ByRef( string.ReplaceAll( '\\' , '/' ) );
				if( string.get_UTF8Char( string.Length() - 1 ) == '/' )
					string.RemoveAt( string.Length() - 1 );
			}
#else
			if( string == "MYDOCUMENTS" )
			{
				struct ::passwd pwent;
				struct ::passwd* pwentp = NULL;
				char buf[ 1024 ] = { 0 };
				::rux::int32 res = 0;
				if( ( res = getpwuid_r( getuid() , &pwent , buf , sizeof( buf ) , &pwentp ) ) == 0 )
				{
					string =  pwent.pw_dir;
					string.set_ByRef( string.ReplaceAll( '\\' , '/' ) );
					if( string.get_UTF8Char( string.Length() - 1 ) == '/' )
						string.RemoveAt( string.Length() - 1 );
				}
			}
		#endif
			return string++;
		};
		rux::String& rux::io::XDirectory::TransformToRuxDirectoryName( const rux::XString& directory_name )
		{
			rux::XString string;
			string.set_ByRef( directory_name.ReplaceAll( '\\' , '/' ) );
			if( string.get_UTF8Char( string.Length() - 1 ) == '/' )
				string.RemoveAt( string.Length() - 1 );
			return string++;
		};
		XString& rux::io::Directory::get_DirectoryName( void )
		{
			return _directory_name;
		};
		rux::Array<XFile>& rux::io::Directory::GetFiles( void )
		{
			rux::XArray<XFile> files;
			if( _directory_name.Length() > 0 )
			{		
				rux::XString directory1;		
		#ifdef __WINDOWS__
				rux::XString directory_name;
				directory_name.set_ByRef( _directory_name.ConvertToUTF16() );
				directory_name.set_ByRef( directory_name.ReplaceAll( '\\' , '/' ) );
				rux::uint32 symbol = directory_name.get_UTF8Char( directory_name.Length() - 1 );
				if( symbol == '/' )
					directory_name.set_ByRef( directory_name + L"*" );
				else
					directory_name.set_ByRef( directory_name + L"/*" );		
				directory_name.set_ByRef( directory_name.ConvertToUTF16() );
				HANDLE find_handle = NULL;
				WIN32_FIND_DATAW win32_find_data;
				if( ( find_handle = FindFirstFileW( (wchar_t*)directory_name.str() , &win32_find_data ) ) != INVALID_HANDLE_VALUE )
				{
					XFile file;			
					do
					{
						if( !( win32_find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
						{
							directory1 = _directory_name;
							directory1 = directory1.ReplaceAll( '\\' , '/' );
							symbol = directory1.get_UTF8Char( directory1.Length() - 1 );
							if( symbol == '/' )
								directory1 += 	win32_find_data.cFileName;		
							else
							{
								directory1 += "/";
								directory1 += win32_find_data.cFileName;		
							}
							file = XFile();
							file()->_file_path = directory1;
							file()->_file_size = ( win32_find_data.nFileSizeHigh * ( static_cast<int64>( MAXDWORD ) + 1 ) ) + win32_find_data.nFileSizeLow;					
							files.Add( file );
						}
					}
					while( FindNextFileW( find_handle , &win32_find_data ) );
					FindClose( find_handle );
				}
		#endif
		#ifdef __UNIX__
				rux::XString directory_name;
				directory_name.set_ByRef( _directory_name.ConvertToUTF8() );
				directory_name.set_ByRef( directory_name.ReplaceAll( '\\' , '/' ) );
				DIR* dir_ptr = opendir( directory_name.str() );
				if( dir_ptr )
				{
					::rux::int32 name_max = pathconf( directory_name.str() , _PC_NAME_MAX );
					if( name_max == -1 )
						name_max = 255;
					size_t dirent_len = offsetof( struct dirent , d_name ) + name_max + 1;
					dirent* dirent_object_ptr = (dirent*)alloc_array_macros( char* , dirent_len );
					dirent* dirent_result = 0;
					XFile file;
					rux::uint32 symbol = 0;
					struct stat io_stat;
					while( readdir_r( dir_ptr , dirent_object_ptr , &dirent_result ) == 0 && dirent_result )
					{
						if( XString::IsEqualA( dirent_object_ptr->d_name , "." ) == 0
							&& XString::IsEqualA( dirent_object_ptr->d_name , ".." ) == 0 )
						{
							directory1 = _directory_name;
							directory1 = directory1.ReplaceAll( '\\' , '/' );
							symbol = directory1.get_UTF8Char( directory1.Length() - 1 );
							if( symbol == '/' )
									directory1 += dirent_object_ptr->d_name;
							else
							{
									directory1 += "/";
									directory1 += dirent_object_ptr->d_name;
							}
							stat( directory1.str() , &io_stat );
		#ifdef __LINUX__
							if( dirent_object_ptr->d_type == DT_REG )
		#elif defined( __SOLARIS__ )                                        
												if( io_stat.st_mode & S_IFREG )
		#endif
							{												
								file = XFile();
								file()->_file_path = directory1;
								//file()->_file_size = ( win32_find_data.nFileSizeHigh * ( static_cast<int64>( MAXDWORD ) + 1 ) ) + win32_find_data.nFileSizeLow;					
								files.Add( file );
							}
						}
					}
					closedir( dir_ptr );
					::rux::engine::free_mem( dirent_object_ptr );	
				}
		#endif
			}
			return files++;
		};
		rux::Array<rux::io::XDirectory>& rux::io::Directory::GetDirectories( void )
		{
			rux::XArray<rux::io::XDirectory> directories;
			if( _directory_name.Length() > 0 )
			{
				rux::XString directory_name( _directory_name );
				directory_name = directory_name.ReplaceAll( '\\' , '/' );
		#ifdef __WINDOWS__		
				rux::uint32 symbol = directory_name.get_UTF8Char( directory_name.Length() - 1 );
				if( symbol == '/' )
					directory_name += L"*";
				else
					directory_name += L"/*";		
				directory_name.set_ByRef( directory_name.ConvertToUTF16() );
				HANDLE find_handle = NULL;
				WIN32_FIND_DATAW win32_find_data;
				if( ( find_handle = FindFirstFileW( (wchar_t*)directory_name.str() , &win32_find_data ) ) != INVALID_HANDLE_VALUE )
				{
					rux::io::XDirectory directory;
					do
					{
						if( ( win32_find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
						{
							if( XString::IsEqualUTF16( (uint16*)win32_find_data.cFileName , (uint16*)L"." ) == 0
								&& XString::IsEqualUTF16( (uint16*)win32_find_data.cFileName , (uint16*)L".." ) == 0 )
							{
								directory = rux::io::XDirectory();
								directory()->_directory_name = _directory_name;
								directory()->_directory_name = directory()->_directory_name.ReplaceAll( '\\' , '/' );
								symbol = directory()->_directory_name.get_UTF8Char( directory()->_directory_name.Length() - 1 );
								if( symbol == '/' )
									directory()->_directory_name += 	win32_find_data.cFileName;		
								else
								{
									directory()->_directory_name += "/";
									directory()->_directory_name += win32_find_data.cFileName;		
								}
								directories.Add( directory );
							}
						}
					}
					while( FindNextFileW( find_handle , &win32_find_data ) );
					FindClose( find_handle );
				}
		#endif
		#ifdef __UNIX__		
				directory_name.set_ByRef( directory_name.ConvertToUTF8() );
				DIR* dir_ptr = opendir( directory_name.str() );
				if( dir_ptr )
				{
					::rux::int32 name_max = pathconf( directory_name.str() , _PC_NAME_MAX );
					if( name_max == -1 )
						name_max = 255;
					size_t dirent_len = offsetof( struct dirent , d_name ) + name_max + 1;
					dirent* dirent_object_ptr = (dirent*)alloc_array_macros( char* , dirent_len );
					dirent* dirent_result = 0;
					rux::io::XDirectory directory;
					rux::uint32 symbol = 0;
		#ifdef __SOLARIS__
					struct stat io_stat;
					rux::XString full_directory;
		#endif
					while( readdir_r( dir_ptr , dirent_object_ptr , &dirent_result ) == 0 && dirent_result )
					{
						if( XString::IsEqualA( dirent_object_ptr->d_name , "." ) == 0
							&& XString::IsEqualA( dirent_object_ptr->d_name , ".." ) == 0 )
						{
		#ifdef __LINUX__
							if( dirent_object_ptr->d_type == DT_DIR )
		#elif defined( __SOLARIS__ )
							full_directory = _directory_name;
							full_directory = full_directory.ReplaceAll( '\\' , '/' );
							symbol = full_directory.get_UTF8Char( full_directory.Length() - 1 );
							if( symbol == '/' )
								full_directory += dirent_object_ptr->d_name;
							else
							{
								full_directory += "/";
								full_directory += dirent_object_ptr->d_name;
				  }
				  stat( full_directory.str() , &io_stat );
				  if( io_stat.st_mode & S_IFDIR )
		#endif
							{					
								directory = rux::io::XDirectory();
		#ifdef __SOLARIS__
														directory()->_directory_name = full_directory;
		#elif defined( __LINUX__ )
								directory()->_directory_name = _directory_name;
								directory()->_directory_name = directory()->_directory_name.ReplaceAll( '\\' , '/' );						
								symbol = directory()->_directory_name.get_UTF8Char( directory()->_directory_name.Length() - 1 );
								if( symbol == '/' )
									directory()->_directory_name += dirent_object_ptr->d_name;		
								else
								{
									directory()->_directory_name += "/";
									directory()->_directory_name += dirent_object_ptr->d_name;		
								}
		#endif
								directories.Add( directory );
							}
						}
					}
					closedir( dir_ptr );
					::rux::engine::free_mem( dirent_object_ptr );	
				}
		#endif
			}
			return directories++;	
		};
		void rux::io::XDirectory::GetFiles( const rux::XString& directory_name , XSearchOption search_option , rux::XArray<XFile>& files )
		{
			if( search_option == XAllDirectories )
			{		
				if( directory_name.Length() > 0 )
				{			
					rux::XString directory1;			
		#ifdef __WINDOWS__
					rux::XString directory_name1;
					directory_name1.set_ByRef( directory_name.ConvertToUTF16() );
					directory_name1.set_ByRef( directory_name1.ReplaceAll( '\\' , '/' ) );				
					rux::uint32 symbol = directory_name1.get_UTF8Char( directory_name1.Length() - 1 );
					if( symbol == '/' )
						directory_name1.set_ByRef( directory_name1 + L"*" );
					else
						directory_name1.set_ByRef( directory_name1 + L"/*" );			
					directory_name1.set_ByRef( directory_name1.ConvertToUTF16() );
					HANDLE find_handle = NULL;
					WIN32_FIND_DATAW win32_find_data;
					if( ( find_handle = FindFirstFileW( (wchar_t*)directory_name1.str() , &win32_find_data ) ) != INVALID_HANDLE_VALUE )
					{
						XFile file;
						do
						{
							if( !( win32_find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
							{
								directory1 = directory_name;
								directory1 = directory1.ReplaceAll( '\\' , '/' );
								symbol = directory1.get_UTF8Char( directory1.Length() - 1 );
								if( symbol == '/' )
									directory1 += 	win32_find_data.cFileName;		
								else
								{
									directory1 += "/";
									directory1 += win32_find_data.cFileName;		
								}						
								file = XFile();
								file()->_file_path = directory1;
								file()->_file_size = ( win32_find_data.nFileSizeHigh * ( static_cast<int64>( MAXDWORD ) + 1 ) ) + win32_find_data.nFileSizeLow;					
								files.Add( file );
							}
							else
							{
								if( XString::IsEqualUTF16( (uint16*)win32_find_data.cFileName , (uint16*)L"." ) == 0
									&& XString::IsEqualUTF16( (uint16*)win32_find_data.cFileName , (uint16*)L".." ) == 0 )
								{
									directory1 = directory_name;
									directory1 = directory1.ReplaceAll( '\\' , '/' );
									symbol = directory1.get_UTF8Char( directory1.Length() - 1 );
									if( symbol == '/' )
										directory1 += 	win32_find_data.cFileName;		
									else
									{
										directory1 += "/";
										directory1 += win32_find_data.cFileName;		
									}							
									GetFiles( directory1 , XAllDirectories , files );
								}
							}
						}
						while( FindNextFileW( find_handle , &win32_find_data ) );
						FindClose( find_handle );
					}
			#endif
			#ifdef __UNIX__
					rux::XString directory_name1;
					directory_name1.set_ByRef( directory_name.ConvertToUTF8() );
					directory_name1.set_ByRef( directory_name1.ReplaceAll( '\\' , '/' ) );
					DIR* dir_ptr = opendir( directory_name1.str() );
					if( dir_ptr )
					{
						::rux::int32 name_max = pathconf( directory_name1.str() , _PC_NAME_MAX );
						if( name_max == -1 )
							name_max = 255;
						size_t dirent_len = offsetof( struct dirent , d_name ) + name_max + 1;
						dirent* dirent_object_ptr = (dirent*)alloc_array_macros( char* , dirent_len );
						dirent* dirent_result = 0;
						XFile file;
						rux::uint32 symbol = 0;
						struct stat io_stat;
						while( readdir_r( dir_ptr , dirent_object_ptr , &dirent_result ) == 0 && dirent_result )
						{
							if( XString::IsEqualA( dirent_object_ptr->d_name , "." ) == 0
								&& XString::IsEqualA( dirent_object_ptr->d_name , ".." ) == 0 )
							{
								directory1.set_ByRef( directory_name.ConvertToUTF8() );
								directory1.set_ByRef( directory1.ReplaceAll( '\\' , '/' ) );
								symbol = directory1.get_UTF8Char( directory1.Length() - 1 );
								if( symbol == '/' )
									directory1.set_ByRef( directory1 + dirent_object_ptr->d_name );
								else
								{
									directory1.set_ByRef( directory1 + "/" );
									directory1.set_ByRef( directory1 + dirent_object_ptr->d_name );
								}
								stat( directory1.str() , &io_stat );
		#ifdef __LINUX__
								if( dirent_object_ptr->d_type == DT_REG )
		#elif defined( __SOLARIS__ )
								if( io_stat.st_mode & S_IFREG )
		#endif
								{
									file = XFile();
									file()->_file_path.set_ByRef( directory1 );
									//file()->_file_size = ( win32_find_data.nFileSizeHigh * ( static_cast<int64>( MAXDWORD ) + 1 ) ) + win32_find_data.nFileSizeLow;					
									files.Add( file );
								}
		#ifdef __LINUX__
								else if( dirent_object_ptr->d_type == DT_DIR )
		#elif defined( __SOLARIS__ )
								else if( io_stat.st_mode & S_IFDIR )
		#endif
								{														
									GetFiles( directory1 , XAllDirectories , files );
								}
							}
						}
						closedir( dir_ptr );
						::rux::engine::free_mem( dirent_object_ptr );	
					}
			#endif
				}	
			}
			else
			{
				if( directory_name.Length() > 0 )
				{			
					rux::XString directory1;			
		#ifdef __WINDOWS__
					rux::XString directory_name1;
					directory_name1.set_ByRef( directory_name.ConvertToUTF16() );
					directory_name1.set_ByRef( directory_name1.ReplaceAll( '\\' , '/' ) );	
					rux::uint32 symbol = directory_name1.get_UTF8Char( directory_name1.Length() - 1 );
					if( symbol == '/' )
						directory_name1.set_ByRef( directory_name1 + L"*" );
					else
						directory_name1.set_ByRef( directory_name1 + L"/*" );			
					directory_name1.set_ByRef( directory_name1.ConvertToUTF16() );
					HANDLE find_handle = NULL;
					WIN32_FIND_DATAW win32_find_data;
					if( ( find_handle = FindFirstFileW( (wchar_t*)directory_name1.str() , &win32_find_data ) ) != INVALID_HANDLE_VALUE )
					{
						XFile file;
						do
						{
							if( !( win32_find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
							{
								directory1.set_ByRef( directory_name.ConvertToUTF16() );
								directory1.set_ByRef( directory1.ReplaceAll( '\\' , '/' ) );
								symbol = directory1.get_UTF8Char( directory1.Length() - 1 );
								if( symbol == '/' )
									directory1.set_ByRef( directory1 + 	win32_find_data.cFileName );		
								else
								{
									directory1.set_ByRef( directory1 + "/" );
									directory1.set_ByRef( directory1 + win32_find_data.cFileName );		
								}						
								file = XFile();
								file()->_file_path.set_ByRef( directory1 );
								file()->_file_size = ( win32_find_data.nFileSizeHigh * ( static_cast<int64>( MAXDWORD ) + 1 ) ) + win32_find_data.nFileSizeLow;					
								files.Add( file );
							}
						}
						while( FindNextFileW( find_handle , &win32_find_data ) );
						FindClose( find_handle );
					}
			#endif
			#ifdef __UNIX__
					rux::XString directory_name1;
					directory_name1.set_ByRef( directory_name.ConvertToUTF8() );
					directory_name1.set_ByRef( directory_name1.ReplaceAll( '\\' , '/' ) );
					DIR* dir_ptr = opendir( directory_name1.str() );
					if( dir_ptr )
					{
						::rux::int32 name_max = pathconf( directory_name1.str() , _PC_NAME_MAX );
						if( name_max == -1 )
							name_max = 255;
						size_t dirent_len = offsetof( struct dirent , d_name ) + name_max + 1;
						dirent* dirent_object_ptr = (dirent*)alloc_array_macros( char* , dirent_len );
						dirent* dirent_result = 0;
						XFile file;
						rux::uint32 symbol = 0;
						struct stat io_stat;
						while( readdir_r( dir_ptr , dirent_object_ptr , &dirent_result ) == 0 && dirent_result )
						{
							if( XString::IsEqualA( dirent_object_ptr->d_name , "." ) == 0
								&& XString::IsEqualA( dirent_object_ptr->d_name , ".." ) == 0 )
							{
								directory1.set_ByRef( directory_name.ConvertToUTF8() );
								directory1.set_ByRef( directory1.ReplaceAll( '\\' , '/' ) );
								symbol = directory1.get_UTF8Char( directory1.Length() - 1 );
								if( symbol == '/' )
									directory1.set_ByRef( directory1 + dirent_object_ptr->d_name );
								else
								{
									directory1.set_ByRef( directory1 + "/" );
									directory1.set_ByRef( directory1 + dirent_object_ptr->d_name );
								}
								stat( directory1.str() , &io_stat );
		#ifdef __LINUX__
								if( dirent_object_ptr->d_type == DT_REG )
		#elif defined( __SOLARIS__ )
								if( io_stat.st_mode & S_IFREG )
		#endif
								{
									file = XFile();
									file()->_file_path.set_ByRef( directory1 );
									//file()->_file_size = ( win32_find_data.nFileSizeHigh * ( static_cast<int64>( MAXDWORD ) + 1 ) ) + win32_find_data.nFileSizeLow;					
									files.Add( file );
								}						
							}
						}
						closedir( dir_ptr );
						::rux::engine::free_mem( dirent_object_ptr );	
					}
			#endif
				}	
			}
		};
		void rux::io::XDirectory::GetFiles( const char* directory_name_ptr , XSearchOption search_option , rux::XArray<XFile>& files )
		{
			if( search_option == XAllDirectories )
			{		
				if( directory_name_ptr && XString::LengthA( directory_name_ptr ) > 0 )
				{			
					rux::XString directory1;			
		#ifdef __WINDOWS__
					rux::XString directory_name1( directory_name_ptr );
					directory_name1.set_ByRef( directory_name1.ConvertToUTF16() );
					directory_name1.set_ByRef( directory_name1.ReplaceAll( '\\' , '/' ) );				
					rux::uint32 symbol = directory_name1.get_UTF8Char( directory_name1.Length() - 1 );
					if( symbol == '/' )
						directory_name1.set_ByRef( directory_name1 + L"*" );
					else
						directory_name1.set_ByRef( directory_name1 + L"/*" );
					directory_name1.set_ByRef( directory_name1.ConvertToUTF16() );
					HANDLE find_handle = NULL;
					WIN32_FIND_DATAW win32_find_data;
					if( ( find_handle = FindFirstFileW( (wchar_t*)directory_name1.str() , &win32_find_data ) ) != INVALID_HANDLE_VALUE )
					{
						XFile file;
						do
						{
							if( !( win32_find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
							{
								directory1 = directory_name_ptr;
								directory1 = directory1.ReplaceAll( '\\' , '/' );
								symbol = directory1.get_UTF8Char( directory1.Length() - 1 );
								if( symbol == '/' )
									directory1 += 	win32_find_data.cFileName;		
								else
								{
									directory1 += "/";
									directory1 += win32_find_data.cFileName;		
								}						
								file = XFile();
								file()->_file_path = directory1;						
								file()->_file_size = ( win32_find_data.nFileSizeHigh * ( static_cast<int64>( MAXDWORD ) + 1 ) ) + win32_find_data.nFileSizeLow;					
								files.Add( file );
							}
							else
							{
								if( XString::IsEqualUTF16( (uint16*)win32_find_data.cFileName , (uint16*)L"." ) == 0
									&& XString::IsEqualUTF16( (uint16*)win32_find_data.cFileName , (uint16*)L".." ) == 0 )
								{
									directory1 = directory_name_ptr;
									directory1 = directory1.ReplaceAll( '\\' , '/' );
									symbol = directory1.get_UTF8Char( directory1.Length() - 1 );
									if( symbol == '/' )
										directory1 += 	win32_find_data.cFileName;		
									else
									{
										directory1 += "/";
										directory1 += win32_find_data.cFileName;		
									}							
									GetFiles( directory1 , XAllDirectories , files );
								}
							}
						}
						while( FindNextFileW( find_handle , &win32_find_data ) );
						FindClose( find_handle );
					}
			#endif
			#ifdef __UNIX__
					rux::XString directory_name1( directory_name_ptr );
					directory_name1.set_ByRef( directory_name1.ConvertToUTF8() );
					directory_name1.set_ByRef( directory_name1.ReplaceAll( '\\' , '/' ) );
					DIR* dir_ptr = opendir( directory_name1.str() );
					if( dir_ptr )
					{
						::rux::int32 name_max = pathconf( directory_name1.str() , _PC_NAME_MAX );
						if( name_max == -1 )
							name_max = 255;
						size_t dirent_len = offsetof( struct dirent , d_name ) + name_max + 1;
						dirent* dirent_object_ptr = (dirent*)alloc_array_macros( char* , dirent_len );
						dirent* dirent_result = 0;
						XFile file;
						rux::uint32 symbol = 0;
						struct stat io_stat;
						while( readdir_r( dir_ptr , dirent_object_ptr , &dirent_result ) == 0 && dirent_result )
						{
							if( XString::IsEqualA( dirent_object_ptr->d_name , "." ) == 0
								&& XString::IsEqualA( dirent_object_ptr->d_name , ".." ) == 0 )
							{
								directory1 = directory_name_ptr;
								directory1 = directory1.ReplaceAll( '\\' , '/' );
								symbol = directory1.get_UTF8Char( directory1.Length() - 1 );
								if( symbol == '/' )
									directory1 += dirent_object_ptr->d_name;
								else
								{
									directory1 += "/";
									directory1 += dirent_object_ptr->d_name;
								}
								stat( directory1.str() , &io_stat );
		#ifdef __LINUX__
								if( dirent_object_ptr->d_type == DT_REG )
		#elif defined( __SOLARIS__ )
								if( io_stat.st_mode & S_IFREG )
		#endif
								{
									file = XFile();
									file()->_file_path = directory1;
									//file()->_file_size = ( win32_find_data.nFileSizeHigh * ( static_cast<int64>( MAXDWORD ) + 1 ) ) + win32_find_data.nFileSizeLow;					
									files.Add( file );
								}
		#ifdef __LINUX__
								else if( dirent_object_ptr->d_type == DT_DIR )
		#elif defined( __SOLARIS__ )
								else if( io_stat.st_mode & S_IFDIR )
		#endif                                          
								{
									GetFiles( directory1 , XAllDirectories , files );
								}
							}
						}
						closedir( dir_ptr );
						::rux::engine::free_mem( dirent_object_ptr );	
					}
			#endif
				}	
			}
			else
			{
				if( directory_name_ptr && XString::LengthA( directory_name_ptr ) > 0 )
				{			
					rux::XString directory1;			
		#ifdef __WINDOWS__
					rux::XString directory_name1( directory_name_ptr );
					directory_name1.set_ByRef( directory_name1.ConvertToUTF16() );
					directory_name1.set_ByRef( directory_name1.ReplaceAll( '\\' , '/' ) );	
					rux::uint32 symbol = directory_name1.get_UTF8Char( directory_name1.Length() - 1 );
					if( symbol == '/' )
						directory_name1.set_ByRef( directory_name1 + L"*" );
					else
						directory_name1.set_ByRef( directory_name1 + L"/*" );			
					directory_name1.set_ByRef( directory_name1.ConvertToUTF16() );
					HANDLE find_handle = NULL;
					WIN32_FIND_DATAW win32_find_data;
					if( ( find_handle = FindFirstFileW( (wchar_t*)directory_name1.str() , &win32_find_data ) ) != INVALID_HANDLE_VALUE )
					{
						XFile file;
						do
						{
							if( !( win32_find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
							{
								directory1 = directory_name_ptr;
								directory1 = directory1.ReplaceAll( '\\' , '/' );
								symbol = directory1.get_UTF8Char( directory1.Length() - 1 );
								if( symbol == '/' )
									directory1 += 	win32_find_data.cFileName;		
								else
								{
									directory1 += "/";
									directory1 += win32_find_data.cFileName;		
								}						
								file = XFile();
								file()->_file_path = directory1;						
								file()->_file_size = ( win32_find_data.nFileSizeHigh * ( static_cast<int64>( MAXDWORD ) + 1 ) ) + win32_find_data.nFileSizeLow;					
								files.Add( file );
							}
						}
						while( FindNextFileW( find_handle , &win32_find_data ) );
						FindClose( find_handle );
					}
			#endif
			#ifdef __UNIX__
					rux::XString directory_name1( directory_name_ptr );
					directory_name1 = directory_name1.ConvertToUTF8();
					directory_name1 = directory_name1.ReplaceAll( '\\' , '/' );
					DIR* dir_ptr = opendir( directory_name1.str() );
					if( dir_ptr )
					{
						::rux::int32 name_max = pathconf( directory_name1.str() , _PC_NAME_MAX );
						if( name_max == -1 )
							name_max = 255;
						size_t dirent_len = offsetof( struct dirent , d_name ) + name_max + 1;
						dirent* dirent_object_ptr = (dirent*)alloc_array_macros( char* , dirent_len );
						dirent* dirent_result = 0;
						XFile file;
						rux::uint32 symbol = 0;
						struct stat io_stat;
						while( readdir_r( dir_ptr , dirent_object_ptr , &dirent_result ) == 0 && dirent_result )
						{
							if( XString::IsEqualA( dirent_object_ptr->d_name , "." ) == 0
								&& XString::IsEqualA( dirent_object_ptr->d_name , ".." ) == 0 )
							{
								directory1 = directory_name_ptr;
								directory1 = directory1.ConvertToUTF8();
								directory1 = directory1.ReplaceAll( '\\' , '/' );
								symbol = directory1.get_UTF8Char( directory1.Length() - 1 );
								if( symbol == '/' )
									directory1 += dirent_object_ptr->d_name;
								else
								{
									directory1 += "/";
									directory1 += dirent_object_ptr->d_name;
								}
								stat( directory1.str() , &io_stat );
		#ifdef __LINUX__
								if( dirent_object_ptr->d_type == DT_REG )
		#elif defined( __SOLARIS__ )
								if( io_stat.st_mode & S_IFREG )
		#endif
								{												
									file = XFile();
									file()->_file_path = directory1;
									//file()->_file_size = ( win32_find_data.nFileSizeHigh * ( static_cast<int64>( MAXDWORD ) + 1 ) ) + win32_find_data.nFileSizeLow;					
									files.Add( file );
								}						
							}
						}
						closedir( dir_ptr );
						::rux::engine::free_mem( dirent_object_ptr );	
					}
			#endif
				}	
			}
		};
		rux::Array<XFile>& rux::io::XDirectory::GetFiles( const rux::XString& directory_name , XSearchOption search_option )
		{
			rux::XArray<XFile> files;
			if( search_option == XAllDirectories )
			{		
				if( directory_name.Length() > 0 )
				{			
					rux::XString directory1;			
		#ifdef __WINDOWS__
					rux::XString directory_name1;
					directory_name1.set_ByRef( directory_name.ConvertToUTF16() );
					directory_name1.set_ByRef( directory_name1.ReplaceAll( '\\' , '/' ) );	
					rux::uint32 symbol = directory_name1.get_UTF8Char( directory_name1.Length() - 1 );
					if( symbol == '/' )
						directory_name1.set_ByRef( directory_name1 + L"*" );
					else
						directory_name1.set_ByRef( directory_name1 + L"/*" );			
					directory_name1.set_ByRef( directory_name1.ConvertToUTF16() );
					HANDLE find_handle = NULL;
					WIN32_FIND_DATAW win32_find_data;
					if( ( find_handle = FindFirstFileW( (wchar_t*)directory_name1.str() , &win32_find_data ) ) != INVALID_HANDLE_VALUE )
					{
						XFile file;
						do
						{
							if( !( win32_find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
							{
								directory1 = directory_name;
								directory1 = directory1.ReplaceAll( '\\' , '/' );
								symbol = directory1.get_UTF8Char( directory1.Length() - 1 );
								if( symbol == '/' )
									directory1 += 	win32_find_data.cFileName;		
								else
								{
									directory1 += "/";
									directory1 += win32_find_data.cFileName;		
								}						
								file = XFile();
								file()->_file_path = directory1;
								file()->_file_size = ( win32_find_data.nFileSizeHigh * ( static_cast<int64>( MAXDWORD ) + 1 ) ) + win32_find_data.nFileSizeLow;					
								files.Add( file );
							}
							else
							{
								if( XString::IsEqualUTF16( (uint16*)win32_find_data.cFileName , (uint16*)L"." ) == 0
									&& XString::IsEqualUTF16( (uint16*)win32_find_data.cFileName , (uint16*)L".." ) == 0 )
								{
									directory1 = directory_name;
									directory1 = directory1.ReplaceAll( '\\' , '/' );
									symbol = directory1.get_UTF8Char( directory1.Length() - 1 );
									if( symbol == '/' )
										directory1 += 	win32_find_data.cFileName;		
									else
									{
										directory1 += "/";
										directory1 += win32_find_data.cFileName;		
									}							
									GetFiles( directory1 , XAllDirectories , files );
								}
							}
						}
						while( FindNextFileW( find_handle , &win32_find_data ) );
						FindClose( find_handle );
					}
			#endif
			#ifdef __UNIX__
					rux::XString directory_name1;
					directory_name1.set_ByRef( directory_name.ConvertToUTF8() );
					directory_name1.set_ByRef( directory_name1.ReplaceAll( '\\' , '/' ) );
					DIR* dir_ptr = opendir( directory_name1.str() );
					if( dir_ptr )
					{
						::rux::int32 name_max = pathconf( directory_name1.str() , _PC_NAME_MAX );
						if( name_max == -1 )
							name_max = 255;
						size_t dirent_len = offsetof( struct dirent , d_name ) + name_max + 1;
						dirent* dirent_object_ptr = (dirent*)alloc_array_macros( char* , dirent_len );
						dirent* dirent_result = 0;
						XFile file;
						rux::uint32 symbol = 0;
						struct stat io_stat;
						while( readdir_r( dir_ptr , dirent_object_ptr , &dirent_result ) == 0 && dirent_result )
						{
							if( XString::IsEqualA( dirent_object_ptr->d_name , "." ) == 0
								&& XString::IsEqualA( dirent_object_ptr->d_name , ".." ) == 0 )
							{
								directory1.set_ByRef( directory_name.ConvertToUTF8() );
								directory1.set_ByRef( directory1.ReplaceAll( '\\' , '/' ) );
								symbol = directory1.get_UTF8Char( directory1.Length() - 1 );
								if( symbol == '/' )
									directory1.set_ByRef( directory1 + dirent_object_ptr->d_name );
								else
								{
									directory1.set_ByRef( directory1 + "/" );
									directory1.set_ByRef( directory1 + dirent_object_ptr->d_name );
								}
								stat( directory1.str() , &io_stat );
		#ifdef __LINUX__
								if( dirent_object_ptr->d_type == DT_REG )
		#elif defined( __SOLARIS__ )
														if( io_stat.st_mode & S_IFREG )
		#endif
								{													
									file = XFile();
									file()->_file_path.set_ByRef( directory1 );
									//file()->_file_size = ( win32_find_data.nFileSizeHigh * ( static_cast<int64>( MAXDWORD ) + 1 ) ) + win32_find_data.nFileSizeLow;					
									files.Add( file );
								}
		#ifdef __LINUX__
														else if( dirent_object_ptr->d_type == DT_DIR )
		#elif defined( __SOLARIS__ )
														else if( io_stat.st_mode & S_IFDIR )
		#endif                                                
								{
															GetFiles( directory1 , XAllDirectories , files );
								}
							}
						}
						closedir( dir_ptr );
						::rux::engine::free_mem( dirent_object_ptr );	
					}
			#endif
				}	
			}
			else
			{
				if( directory_name.Length() > 0 )
				{			
					rux::XString directory1;			
		#ifdef __WINDOWS__
					rux::XString directory_name1;
					directory_name1.set_ByRef( directory_name.ConvertToUTF16() );
					directory_name1.set_ByRef( directory_name1.ReplaceAll( '\\' , '/' ) );	
					rux::uint32 symbol = directory_name1.get_UTF8Char( directory_name1.Length() - 1 );
					if( symbol == '/' )
						directory_name1.set_ByRef( directory_name1 + L"*" );
					else
						directory_name1.set_ByRef( directory_name1 + L"/*" );			
					directory_name1.set_ByRef( directory_name1.ConvertToUTF16() );
					HANDLE find_handle = NULL;
					WIN32_FIND_DATAW win32_find_data;
					if( ( find_handle = FindFirstFileW( (wchar_t*)directory_name1.str() , &win32_find_data ) ) != INVALID_HANDLE_VALUE )
					{
						XFile file;
						do
						{
							if( !( win32_find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
							{
								directory1 = directory_name;
								directory1 = directory1.ReplaceAll( '\\' , '/' );
								symbol = directory1.get_UTF8Char( directory1.Length() - 1 );
								if( symbol == '/' )
									directory1 += 	win32_find_data.cFileName;		
								else
								{
									directory1 += "/";
									directory1 += win32_find_data.cFileName;		
								}						
								file = XFile();
								file()->_file_path = directory1;						
								file()->_file_size = ( win32_find_data.nFileSizeHigh * ( static_cast<int64>( MAXDWORD ) + 1 ) ) + win32_find_data.nFileSizeLow;					
								files.Add( file );
							}
						}
						while( FindNextFileW( find_handle , &win32_find_data ) );
						FindClose( find_handle );
					}
			#endif
			#ifdef __UNIX__
					rux::XString directory_name1;
					directory_name1.set_ByRef( directory_name.ConvertToUTF8() );
					directory_name1.set_ByRef( directory_name1.ReplaceAll( '\\' , '/' ) );
					DIR* dir_ptr = opendir( directory_name1.str() );
					if( dir_ptr )
					{
						::rux::int32 name_max = pathconf( directory_name1.str() , _PC_NAME_MAX );
						if( name_max == -1 )
							name_max = 255;
						size_t dirent_len = offsetof( struct dirent , d_name ) + name_max + 1;
						dirent* dirent_object_ptr = (dirent*)alloc_array_macros( char* , dirent_len );
						dirent* dirent_result = 0;
						XFile file;
						rux::uint32 symbol = 0;
						struct stat io_stat;
						while( readdir_r( dir_ptr , dirent_object_ptr , &dirent_result ) == 0 && dirent_result )
						{
							if( XString::IsEqualA( dirent_object_ptr->d_name , "." ) == 0
								&& XString::IsEqualA( dirent_object_ptr->d_name , ".." ) == 0 )
							{
								directory1.set_ByRef( directory_name.ConvertToUTF8() );
								directory1.set_ByRef( directory1.ReplaceAll( '\\' , '/' ) );
								symbol = directory1.get_UTF8Char( directory1.Length() - 1 );
								if( symbol == '/' )
									directory1.set_ByRef( directory1 + dirent_object_ptr->d_name );
								else
								{
									directory1.set_ByRef( directory1 + "/" );
									directory1.set_ByRef( directory1 + dirent_object_ptr->d_name );
								}
								stat( directory1.str() , &io_stat );
		#ifdef __LINUX__
								if( dirent_object_ptr->d_type == DT_REG )
		#elif defined( __SOLARIS__ )            
								if( io_stat.st_mode & S_IFREG )
		#endif
								{												
									file = XFile();
									file()->_file_path.set_ByRef( directory1 );
									//file()->_file_size = ( win32_find_data.nFileSizeHigh * ( static_cast<int64>( MAXDWORD ) + 1 ) ) + win32_find_data.nFileSizeLow;					
									files.Add( file );
								}						
							}
						}
						closedir( dir_ptr );
						::rux::engine::free_mem( dirent_object_ptr );	
					}
			#endif
				}	
			}
			return files++;
		};
		rux::Array<XFile>& rux::io::XDirectory::GetFiles( const char* directory_name_ptr , XSearchOption search_option )
		{
			rux::XArray<XFile> files;
			if( search_option == XAllDirectories )
			{		
				if( directory_name_ptr && XString::LengthA( directory_name_ptr ) > 0 )
				{			
					rux::XString directory1;			
		#ifdef __WINDOWS__
					rux::XString directory_name1( directory_name_ptr );
					directory_name1.set_ByRef( directory_name1.ConvertToUTF16() );
					directory_name1.set_ByRef( directory_name1.ReplaceAll( '\\' , '/' ) );	
					rux::uint32 symbol = directory_name1.get_UTF8Char( directory_name1.Length() - 1 );
					if( symbol == '/' )
						directory_name1.set_ByRef( directory_name1 + L"*" );
					else
						directory_name1.set_ByRef( directory_name1 + L"/*" );						
					directory_name1.set_ByRef( directory_name1.ConvertToUTF16() );
					HANDLE find_handle = NULL;
					WIN32_FIND_DATAW win32_find_data;
					if( ( find_handle = FindFirstFileW( (wchar_t*)directory_name1.str() , &win32_find_data ) ) != INVALID_HANDLE_VALUE )
					{
						XFile file;
						do
						{
							if( !( win32_find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
							{
								directory1 = directory_name_ptr;
								directory1 = directory1.ReplaceAll( '\\' , '/' );
								symbol = directory1.get_UTF8Char( directory1.Length() - 1 );
								if( symbol == '/' )
									directory1 += 	win32_find_data.cFileName;		
								else
								{
									directory1 += "/";
									directory1 += win32_find_data.cFileName;		
								}						
								file = XFile();
								file()->_file_path = directory1;						
								file()->_file_size = ( win32_find_data.nFileSizeHigh * ( static_cast<int64>( MAXDWORD ) + 1 ) ) + win32_find_data.nFileSizeLow;					
								files.Add( file );
							}
							else
							{
								if( XString::IsEqualUTF16( (uint16*)win32_find_data.cFileName , (uint16*)L"." ) == 0
									&& XString::IsEqualUTF16( (uint16*)win32_find_data.cFileName , (uint16*)L".." ) == 0 )
								{
									directory1 = directory_name_ptr;
									directory1 = directory1.ReplaceAll( '\\' , '/' );
									symbol = directory1.get_UTF8Char( directory1.Length() - 1 );
									if( symbol == '/' )
										directory1 += 	win32_find_data.cFileName;		
									else
									{
										directory1 += "/";
										directory1 += win32_find_data.cFileName;		
									}							
									GetFiles( directory1 , XAllDirectories , files );
								}
							}
						}
						while( FindNextFileW( find_handle , &win32_find_data ) );
						FindClose( find_handle );
					}
			#endif
			#ifdef __UNIX__
					rux::XString directory_name1( directory_name_ptr );
					directory_name1.set_ByRef( directory_name1.ConvertToUTF8() );
					directory_name1.set_ByRef( directory_name1.ReplaceAll( '\\' , '/' ) );
					DIR* dir_ptr = opendir( directory_name1.str() );
					if( dir_ptr )
					{
						::rux::int32 name_max = pathconf( directory_name1.str() , _PC_NAME_MAX );
						if( name_max == -1 )
							name_max = 255;
						size_t dirent_len = offsetof( struct dirent , d_name ) + name_max + 1;
						dirent* dirent_object_ptr = (dirent*)alloc_array_macros( char* , dirent_len );
						dirent* dirent_result = 0;
						XFile file;
						rux::uint32 symbol = 0;
						struct stat io_stat;
						while( readdir_r( dir_ptr , dirent_object_ptr , &dirent_result ) == 0 && dirent_result )
						{
							if( XString::IsEqualA( dirent_object_ptr->d_name , "." ) == 0
								&& XString::IsEqualA( dirent_object_ptr->d_name , ".." ) == 0 )
							{
								directory1 = directory_name_ptr;
								directory1 = directory1.ReplaceAll( '\\' , '/' );
								symbol = directory1.get_UTF8Char( directory1.Length() - 1 );
								if( symbol == '/' )
									directory1 += dirent_object_ptr->d_name;
								else
								{
									directory1 += "/";
									directory1 += dirent_object_ptr->d_name;
								}
								stat( directory1.str() , &io_stat );
		#ifdef __SOLARIS__                                                
								if( io_stat.st_mode & S_IFREG )
		#elif defined( __LINUX__ )
								if( dirent_object_ptr->d_type == DT_REG )
		#endif
								{													
									file = XFile();
									file()->_file_path = directory1;
									//file()->_file_size = ( win32_find_data.nFileSizeHigh * ( static_cast<int64>( MAXDWORD ) + 1 ) ) + win32_find_data.nFileSizeLow;					
									files.Add( file );
								}
		#ifdef __LINUX__
								else if( dirent_object_ptr->d_type == DT_DIR )
		#elif defined( __SOLARIS__ )
								else if( io_stat.st_mode & S_IFDIR )
		#endif
								{													
									GetFiles( directory1 , XAllDirectories , files );
								}
							}
						}
						closedir( dir_ptr );
						::rux::engine::free_mem( dirent_object_ptr );	
					}
			#endif
				}	
			}
			else
			{
				if( directory_name_ptr && XString::LengthA( directory_name_ptr ) > 0 )
				{			
					rux::XString directory1;			
		#ifdef __WINDOWS__
					rux::XString directory_name1( directory_name_ptr );
					directory_name1.set_ByRef( directory_name1.ConvertToUTF16() );
					directory_name1.set_ByRef( directory_name1.ReplaceAll( '\\' , '/' ) );	
					rux::uint32 symbol = directory_name1.get_UTF8Char( directory_name1.Length() - 1 );
					if( symbol == '/' )
						directory_name1.set_ByRef( directory_name1 + L"*" );
					else
						directory_name1.set_ByRef( directory_name1 + L"/*" );
					directory_name1.set_ByRef( directory_name1.ConvertToUTF16() );
					HANDLE find_handle = NULL;
					WIN32_FIND_DATAW win32_find_data;
					if( ( find_handle = FindFirstFileW( (wchar_t*)directory_name1.str() , &win32_find_data ) ) != INVALID_HANDLE_VALUE )
					{
						XFile file;
						do
						{
							if( !( win32_find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
							{
								directory1 = directory_name_ptr;
								directory1 = directory1.ReplaceAll( '\\' , '/' );
								symbol = directory1.get_UTF8Char( directory1.Length() - 1 );
								if( symbol == '/' )
									directory1 += 	win32_find_data.cFileName;		
								else
								{
									directory1 += "/";
									directory1 += win32_find_data.cFileName;		
								}						
								file = XFile();
								file()->_file_path = directory1;						
								file()->_file_size = ( win32_find_data.nFileSizeHigh * ( static_cast<int64>( MAXDWORD ) + 1 ) ) + win32_find_data.nFileSizeLow;					
								files.Add( file );
							}
						}
						while( FindNextFileW( find_handle , &win32_find_data ) );
						FindClose( find_handle );
					}
			#endif
			#ifdef __UNIX__
					rux::XString directory_name1( directory_name_ptr );
					directory_name1.set_ByRef( directory_name1.ConvertToUTF8() );
					directory_name1.set_ByRef( directory_name1.ReplaceAll( '\\' , '/' ) );
					DIR* dir_ptr = opendir( directory_name1.str() );
					if( dir_ptr )
					{
						::rux::int32 name_max = pathconf( directory_name1.str() , _PC_NAME_MAX );
						if( name_max == -1 )
							name_max = 255;
						size_t dirent_len = offsetof( struct dirent , d_name ) + name_max + 1;
						dirent* dirent_object_ptr = (dirent*)alloc_array_macros( char* , dirent_len );
						dirent* dirent_result = 0;
						XFile file;
						rux::uint32 symbol = 0;
						struct stat io_stat;
						while( readdir_r( dir_ptr , dirent_object_ptr , &dirent_result ) == 0 && dirent_result )
						{
							if( XString::IsEqualA( dirent_object_ptr->d_name , "." ) == 0
								&& XString::IsEqualA( dirent_object_ptr->d_name , ".." ) == 0 )
							{
								directory1 = directory_name_ptr;
								directory1 = directory1.ConvertToUTF8();
								directory1 = directory1.ReplaceAll( '\\' , '/' );
								symbol = directory1.get_UTF8Char( directory1.Length() - 1 );
								if( symbol == '/' )
									directory1 += dirent_object_ptr->d_name;
								else
								{
									directory1 += "/";
									directory1 += dirent_object_ptr->d_name;
								}
								stat( directory1.str() , &io_stat );
		#ifdef __LINUX__
								if( dirent_object_ptr->d_type == DT_REG )
		#elif defined( __SOLARIS__ )                                                
								if( io_stat.st_mode & S_IFREG )
		#endif
								{													
									file = XFile();
									file()->_file_path = directory1;
									//file()->_file_size = ( win32_find_data.nFileSizeHigh * ( static_cast<int64>( MAXDWORD ) + 1 ) ) + win32_find_data.nFileSizeLow;					
									files.Add( file );
								}						
							}
						}
						closedir( dir_ptr );
						::rux::engine::free_mem( dirent_object_ptr );	
					}
			#endif
				}	
			}
			return files++;
		};
		void rux::io::XDirectory::GetFiles( const char* directory_name_ptr , rux::XArray<XFile>& files )
		{	
			if( directory_name_ptr && XString::LengthA( directory_name_ptr ) > 0 )
			{		
				rux::XString directory1;		
		#ifdef __WINDOWS__
				rux::XString directory_name1( directory_name_ptr );
				directory_name1.set_ByRef( directory_name1.ConvertToUTF16() );
				directory_name1.set_ByRef( directory_name1.ReplaceAll( '\\' , '/' ) );
				rux::uint32 symbol = directory_name1.get_UTF8Char( directory_name1.Length() - 1 );
				if( symbol == '/' )
					directory_name1.set_ByRef( directory_name1 + L"*" );
				else
					directory_name1.set_ByRef( directory_name1 + L"/*" );
				directory_name1.set_ByRef( directory_name1.ConvertToUTF16() );
				HANDLE find_handle = NULL;
				WIN32_FIND_DATAW win32_find_data;
				if( ( find_handle = FindFirstFileW( (wchar_t*)directory_name1.str() , &win32_find_data ) ) != INVALID_HANDLE_VALUE )
				{
					XFile file;
					do
					{
						if( !( win32_find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
						{
							directory1 = directory_name_ptr;
							directory1 = directory1.ReplaceAll( '\\' , '/' );
							symbol = directory1.get_UTF8Char( directory1.Length() - 1 );
							if( symbol == '/' )
								directory1 += win32_find_data.cFileName;		
							else
							{
								directory1 += "/";
								directory1 += win32_find_data.cFileName;		
							}					
							file = XFile();
							file()->_file_path = directory1;					
							file()->_file_size = ( win32_find_data.nFileSizeHigh * ( static_cast<int64>( MAXDWORD ) + 1 ) ) + win32_find_data.nFileSizeLow;					
							files.Add( file );
						}
					}
					while( FindNextFileW( find_handle , &win32_find_data ) );
					FindClose( find_handle );
				}
		#endif
		#ifdef __UNIX__
				rux::XString directory_name1( directory_name_ptr );
				directory_name1.set_ByRef( directory_name1.ConvertToUTF8() );
				directory_name1.set_ByRef( directory_name1.ReplaceAll( '\\' , '/' ) );
				DIR* dir_ptr = opendir( directory_name1.str() );
				if( dir_ptr )
				{
					::rux::int32 name_max = pathconf( directory_name1.str() , _PC_NAME_MAX );
					if( name_max == -1 )
						name_max = 255;
					size_t dirent_len = offsetof( struct dirent , d_name ) + name_max + 1;
					dirent* dirent_object_ptr = (dirent*)alloc_array_macros( char* , dirent_len );
					dirent* dirent_result = 0;
					XFile file;
					rux::uint32 symbol = 0;
					struct stat io_stat;
					while( readdir_r( dir_ptr , dirent_object_ptr , &dirent_result ) == 0 && dirent_result )
					{
						if( XString::IsEqualA( dirent_object_ptr->d_name , "." ) == 0
							&& XString::IsEqualA( dirent_object_ptr->d_name , ".." ) == 0 )
						{
							directory1 = directory_name_ptr;
							directory1 = directory1.ConvertToUTF8();
							directory1 = directory1.ReplaceAll( '\\' , '/' );
							symbol = directory1.get_UTF8Char( directory1.Length() - 1 );
							if( symbol == '/' )
								directory1 += dirent_object_ptr->d_name;
							else
							{
								directory1 += "/";
								directory1 += dirent_object_ptr->d_name;
							}
							stat( directory1.str() , &io_stat );
		#ifdef __LINUX__
							if( dirent_object_ptr->d_type == DT_REG )
		#elif defined( __SOLARIS__ )                                        
							if( io_stat.st_mode & S_IFREG )
		#endif
							{												
								file = XFile();
								file()->_file_path = directory1;
								//file()->_file_size = ( win32_find_data.nFileSizeHigh * ( static_cast<int64>( MAXDWORD ) + 1 ) ) + win32_find_data.nFileSizeLow;					
								files.Add( file );
							}						
						}
					}
					closedir( dir_ptr );
					::rux::engine::free_mem( dirent_object_ptr );	
				}
		#endif
			}	
		};
		rux::Array<XFile>& rux::io::XDirectory::GetFiles( const char* directory_name_ptr )
		{
			rux::XArray<XFile> files;
			if( directory_name_ptr && XString::LengthA( directory_name_ptr ) > 0 )
			{		
				rux::XString directory1;		
		#ifdef __WINDOWS__
				rux::XString directory_name1( directory_name_ptr );
				directory_name1.set_ByRef( directory_name1.ConvertToUTF16() );
				directory_name1.set_ByRef( directory_name1.ReplaceAll( '\\' , '/' ) );
				rux::uint32 symbol = directory_name1.get_UTF8Char( directory_name1.Length() - 1 );
				if( symbol == '/' )
					directory_name1.set_ByRef( directory_name1 + L"*" );
				else
					directory_name1.set_ByRef( directory_name1 + L"/*" );
				directory_name1.set_ByRef( directory_name1.ConvertToUTF16() );
				HANDLE find_handle = NULL;
				WIN32_FIND_DATAW win32_find_data;
				if( ( find_handle = FindFirstFileW( (wchar_t*)directory_name1.str() , &win32_find_data ) ) != INVALID_HANDLE_VALUE )
				{
					XFile file;
					do
					{
						if( !( win32_find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
						{
							directory1 = directory_name_ptr;
							directory1 = directory1.ReplaceAll( '\\' , '/' );
							symbol = directory1.get_UTF8Char( directory1.Length() - 1 );
							if( symbol == '/' )
								directory1 += 	win32_find_data.cFileName;		
							else
							{
								directory1 += "/";
								directory1 += win32_find_data.cFileName;		
							}					
							file = XFile();
							file()->_file_path = directory1;					
							file()->_file_size = ( win32_find_data.nFileSizeHigh * ( (int64)MAXDWORD + 1 ) ) + win32_find_data.nFileSizeLow;					
							files.Add( file );
						}
					}
					while( FindNextFileW( find_handle , &win32_find_data ) );
					FindClose( find_handle );
				}
		#endif
		#ifdef __UNIX__
				rux::XString directory_name1( directory_name_ptr );
				directory_name1.set_ByRef( directory_name1.ConvertToUTF8() );
				directory_name1.set_ByRef( directory_name1.ReplaceAll( '\\' , '/' ) );
				DIR* dir_ptr = opendir( directory_name1.str() );
				if( dir_ptr )
				{
					::rux::int32 name_max = pathconf( directory_name1.str() , _PC_NAME_MAX );
					if( name_max == -1 )
						name_max = 255;
					size_t dirent_len = offsetof( struct dirent , d_name ) + name_max + 1;
					dirent* dirent_object_ptr = (dirent*)alloc_array_macros( char* , dirent_len );
					dirent* dirent_result = 0;
					XFile file;
					rux::uint32 symbol = 0;
					struct stat io_stat;
					while( readdir_r( dir_ptr , dirent_object_ptr , &dirent_result ) == 0 && dirent_result )
					{
						if( XString::IsEqualA( dirent_object_ptr->d_name , "." ) == 0
							&& XString::IsEqualA( dirent_object_ptr->d_name , ".." ) == 0 )
						{
							directory1 = directory_name_ptr;
							directory1 = directory1.ConvertToUTF8();
							directory1 = directory1.ReplaceAll( '\\' , '/' );
							symbol = directory1.get_UTF8Char( directory1.Length() - 1 );
							if( symbol == '/' )
								directory1 += dirent_object_ptr->d_name;
							else
							{
								directory1 += "/";
								directory1 += dirent_object_ptr->d_name;
							}
							stat( directory1.str() , &io_stat );
		#ifdef __LINUX__
							if( dirent_object_ptr->d_type == DT_REG )
		#elif defined( __SOLARIS__ )
							if( io_stat.st_mode & S_IFREG )
		#endif
							{												
								file = XFile();
								file()->_file_path = directory1;
								//file()->_file_size = ( win32_find_data.nFileSizeHigh * ( static_cast<int64>( MAXDWORD ) + 1 ) ) + win32_find_data.nFileSizeLow;					
								files.Add( file );
							}						
						}
					}
					closedir( dir_ptr );
					::rux::engine::free_mem( dirent_object_ptr );	
				}
		#endif
			}
			return files++;
		};
		rux::Array<rux::io::XDirectory>& rux::io::XDirectory::GetDirectories( const char* directory_name_ptr )
		{
			rux::XArray<rux::io::XDirectory> directories;
			if( directory_name_ptr && XString::LengthA( directory_name_ptr ) > 0 )
			{
				rux::XString directory_name( directory_name_ptr );
		#ifdef __WINDOWS__
				directory_name = directory_name.ReplaceAll( '\\' , '/' );
				rux::uint32 symbol = directory_name.get_UTF8Char( directory_name.Length() - 1 );
				if( symbol == '/' )
					directory_name += L"*";
				else
					directory_name += L"/*";		
				directory_name.set_ByRef( directory_name.ConvertToUTF16() );
				HANDLE find_handle = NULL;
				WIN32_FIND_DATAW win32_find_data;
				if( ( find_handle = FindFirstFileW( (wchar_t*)directory_name.str() , &win32_find_data ) ) != INVALID_HANDLE_VALUE )
				{
					rux::io::XDirectory directory;
					do
					{
						if( ( win32_find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
						{
							if( XString::IsEqualUTF16( (uint16*)win32_find_data.cFileName , (uint16*)L"." ) == 0
								&& XString::IsEqualUTF16( (uint16*)win32_find_data.cFileName , (uint16*)L".." ) == 0 )
							{
								directory = rux::io::XDirectory();
								directory()->_directory_name = directory_name_ptr;
								directory()->_directory_name = directory()->_directory_name.ReplaceAll( '\\' , '/' );
								symbol = directory()->_directory_name.get_UTF8Char( directory()->_directory_name.Length() - 1 );
								if( symbol == '/' )
									directory()->_directory_name += 	win32_find_data.cFileName;		
								else
								{
									directory()->_directory_name += "/";
									directory()->_directory_name += win32_find_data.cFileName;		
								}						
								directories.Add( directory );
							}
						}
					}
					while( FindNextFileW( find_handle , &win32_find_data ) );
					FindClose( find_handle );
				}
		#endif
		#ifdef __UNIX__
				directory_name.set_ByRef( directory_name.ReplaceAll( '\\' , '/' ) );
				directory_name.set_ByRef( directory_name.ConvertToUTF8() );
				DIR* dir_ptr = opendir( directory_name.str() );
				if( dir_ptr )
				{
					::rux::int32 name_max = pathconf( directory_name.str() , _PC_NAME_MAX );
					if( name_max == -1 )
						name_max = 255;
					size_t dirent_len = offsetof( struct dirent , d_name ) + name_max + 1;
					dirent* dirent_object_ptr = (dirent*)alloc_array_macros( char* , dirent_len );
					dirent* dirent_result = 0;
					rux::io::XDirectory directory;
					rux::uint32 symbol = 0;
		#ifdef __SOLARIS__
					struct stat io_stat;
					rux::XString full_directory;
		#endif
					while( readdir_r( dir_ptr , dirent_object_ptr , &dirent_result ) == 0 && dirent_result )
					{
						if( XString::IsEqualA( dirent_object_ptr->d_name , "." ) == 0
							&& XString::IsEqualA( dirent_object_ptr->d_name , ".." ) == 0 )
						{
		#ifdef __LINUX__
							if( dirent_object_ptr->d_type == DT_DIR )
		#elif defined( __SOLARIS__ )
							full_directory = directory_name_ptr;
							full_directory = full_directory.ReplaceAll( '\\' , '/' );
							symbol = full_directory.get_UTF8Char( full_directory.Length() - 1 );
							if( symbol == '/' )
								full_directory += dirent_object_ptr->d_name;
							else
							{
								full_directory += "/";
								full_directory += dirent_object_ptr->d_name;
							}
							stat( full_directory.str() , &io_stat );
							if( io_stat.st_mode & S_IFDIR )
		#endif
							{					
								directory = rux::io::XDirectory();
		#ifdef __SOLARIS__
														directory()->_directory_name = full_directory;
		#elif defined( __LINUX__ )
								directory()->_directory_name = directory_name_ptr;
								directory()->_directory_name = directory()->_directory_name.ReplaceAll( '\\' , '/' );						
								symbol = directory()->_directory_name.get_UTF8Char( directory()->_directory_name.Length() - 1 );
								if( symbol == '/' )
									directory()->_directory_name += dirent_object_ptr->d_name;		
								else
								{
									directory()->_directory_name += "/";
									directory()->_directory_name += dirent_object_ptr->d_name;		
								}
		#endif
								directories.Add( directory );
							}
						}
					}
					closedir( dir_ptr );
					::rux::engine::free_mem( dirent_object_ptr );	
				}
		#endif
			}
			return directories++;
		};
		void rux::io::XDirectory::GetDirectories( const rux::XString& directory_name , rux::XArray<rux::io::XDirectory>& directories )
		{	
			if( directory_name.Length() > 0 )
			{
				rux::XString local_directory_name( directory_name );
		#ifdef __WINDOWS__
				local_directory_name = local_directory_name.ReplaceAll( '\\' , '/' );
				rux::uint32 symbol = local_directory_name.get_UTF8Char( local_directory_name.Length() - 1 );
				if( symbol == '/' )
					local_directory_name += L"*";
				else
					local_directory_name += L"/*";		
				local_directory_name.set_ByRef( local_directory_name.ConvertToUTF16() );
				HANDLE find_handle = NULL;
				WIN32_FIND_DATAW win32_find_data;
				if( ( find_handle = FindFirstFileW( (wchar_t*)local_directory_name.str() , &win32_find_data ) ) != INVALID_HANDLE_VALUE )
				{
					rux::io::XDirectory directory;
					do
					{
						if( ( win32_find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
						{
							if( XString::IsEqualUTF16( (uint16*)win32_find_data.cFileName , (uint16*)L"." ) == 0
								&& XString::IsEqualUTF16( (uint16*)win32_find_data.cFileName , (uint16*)L".." ) == 0 )
							{
								directory = rux::io::XDirectory();
								directory()->_directory_name = directory_name;
								directory()->_directory_name = directory()->_directory_name.ReplaceAll( '\\' , '/' );
								symbol = directory()->_directory_name.get_UTF8Char( directory()->_directory_name.Length() - 1 );
								if( symbol == '/' )
									directory()->_directory_name += 	win32_find_data.cFileName;		
								else
								{
									directory()->_directory_name += "/";
									directory()->_directory_name += win32_find_data.cFileName;		
								}						
								directories.Add( directory );
							}
						}
					}
					while( FindNextFileW( find_handle , &win32_find_data ) );
					FindClose( find_handle );
				}
		#endif
		#ifdef __UNIX__
				local_directory_name.set_ByRef( local_directory_name.ReplaceAll( '\\' , '/' ) );
				local_directory_name.set_ByRef( local_directory_name.ConvertToUTF8() );
				DIR* dir_ptr = opendir( local_directory_name.str() );
				if( dir_ptr )
				{
					::rux::int32 name_max = pathconf( local_directory_name.str() , _PC_NAME_MAX );
					if( name_max == -1 )
						name_max = 255;
					size_t dirent_len = offsetof( struct dirent , d_name ) + name_max + 1;
					dirent* dirent_object_ptr = (dirent*)alloc_array_macros( char* , dirent_len );
					dirent* dirent_result = 0;
					rux::io::XDirectory directory;				
					rux::uint32 symbol = 0;
		#ifdef __SOLARIS__
					struct stat io_stat;
					rux::XString full_directory;
		#endif
					while( readdir_r( dir_ptr , dirent_object_ptr , &dirent_result ) == 0 && dirent_result )
					{
						if( XString::IsEqualA( dirent_object_ptr->d_name , "." ) == 0
							&& XString::IsEqualA( dirent_object_ptr->d_name , ".." ) == 0 )
						{
		#ifdef __LINUX__
							if( dirent_object_ptr->d_type == DT_DIR )
		#elif defined( __SOLARIS__)
							full_directory = directory_name;
							full_directory = full_directory.ReplaceAll( '\\' , '/' );
							symbol = full_directory.get_UTF8Char( full_directory.Length() - 1 );
							if( symbol == '/' )
								full_directory += dirent_object_ptr->d_name;
							else
							{
								full_directory += "/";
								full_directory += dirent_object_ptr->d_name;
							}
							stat( full_directory.str() , &io_stat );
							if( io_stat.st_mode & S_IFDIR )
		#endif
							{					
								directory = rux::io::XDirectory();
		#ifdef __SOLARIS__
														directory()->_directory_name = full_directory;
		#elif defined( __LINUX__ )
								directory()->_directory_name = directory_name;
								directory()->_directory_name = directory()->_directory_name.ReplaceAll( '\\' , '/' );						
								symbol = directory()->_directory_name.get_UTF8Char( directory()->_directory_name.Length() - 1 );
								if( symbol == '/' )
									directory()->_directory_name += dirent_object_ptr->d_name;		
								else
								{
									directory()->_directory_name += "/";
									directory()->_directory_name += dirent_object_ptr->d_name;		
								}
		#endif
								directories.Add( directory );
							}
						}
					}
					closedir( dir_ptr );
					::rux::engine::free_mem( dirent_object_ptr );					
				}
		#endif
			}	
		};
		void rux::io::XDirectory::GetDirectories( const char* directory_name_ptr , rux::XArray<rux::io::XDirectory>& directories )
		{	
			if( directory_name_ptr && XString::LengthA( directory_name_ptr ) > 0 )
			{
				rux::XString directory_name( directory_name_ptr );
		#ifdef __WINDOWS__
				directory_name = directory_name.ReplaceAll( '\\' , '/' );
				rux::uint32 symbol = directory_name.get_UTF8Char( directory_name.Length() - 1 );
				if( symbol == '/' )
					directory_name += L"*";
				else
					directory_name += L"/*";		
				directory_name.set_ByRef( directory_name.ConvertToUTF16() );
				HANDLE find_handle = NULL;
				WIN32_FIND_DATAW win32_find_data;
				if( ( find_handle = FindFirstFileW( (wchar_t*)directory_name.str() , &win32_find_data ) ) != INVALID_HANDLE_VALUE )
				{
					rux::io::XDirectory directory;
					do
					{
						if( ( win32_find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
						{
							if( XString::IsEqualUTF16( (uint16*)win32_find_data.cFileName , (uint16*)L"." ) == 0
								&& XString::IsEqualUTF16( (uint16*)win32_find_data.cFileName , (uint16*)L".." ) == 0 )
							{
								directory = rux::io::XDirectory();
								directory()->_directory_name = directory_name_ptr;
								directory()->_directory_name = directory()->_directory_name.ReplaceAll( '\\' , '/' );
								symbol = directory()->_directory_name.get_UTF8Char( directory()->_directory_name.Length() - 1 );
								if( symbol == '/' )
									directory()->_directory_name += win32_find_data.cFileName;		
								else
								{
									directory()->_directory_name += "/";
									directory()->_directory_name += win32_find_data.cFileName;		
								}						
								directories.Add( directory );
							}
						}
					}
					while( FindNextFileW( find_handle , &win32_find_data ) );
					FindClose( find_handle );
				}
		#endif
		#ifdef __UNIX__
				directory_name.set_ByRef( directory_name.ReplaceAll( '\\' , '/' ) );
				directory_name.set_ByRef( directory_name.ConvertToUTF8() );
				DIR* dir_ptr = opendir( directory_name.str() );
				if( dir_ptr )
				{
					::rux::int32 name_max = pathconf( directory_name.str() , _PC_NAME_MAX );
					if( name_max == -1 )
						name_max = 255;
					size_t dirent_len = offsetof( struct dirent , d_name ) + name_max + 1;
					dirent* dirent_object_ptr = (dirent*)alloc_array_macros( char* , dirent_len );
					dirent* dirent_result = 0;
					rux::io::XDirectory directory;
					rux::uint32 symbol = 0;
		#ifdef __SOLARIS__
					struct stat io_stat;
					XString full_directory;
		#endif
					while( readdir_r( dir_ptr , dirent_object_ptr , &dirent_result ) == 0 && dirent_result )
					{
						if( XString::IsEqualA( dirent_object_ptr->d_name , "." ) == 0
							&& XString::IsEqualA( dirent_object_ptr->d_name , ".." ) == 0 )
						{
		#ifdef __LINUX__
							if( dirent_object_ptr->d_type == DT_DIR )
		#elif defined( __SOLARIS__ )
												full_directory = directory_name_ptr;
												full_directory = full_directory.ReplaceAll( '\\' , '/' );
												symbol = full_directory.get_UTF8Char( full_directory.Length() - 1 );
												if( symbol == '/' )
														full_directory += dirent_object_ptr->d_name;
												else
												{
														full_directory += "/";
														full_directory += dirent_object_ptr->d_name;
												}
												stat( full_directory.str() , &io_stat );
												if( io_stat.st_mode & S_IFDIR )
		#endif
							{					
								directory = rux::io::XDirectory();
		#ifdef __SOLARIS__
														directory()->_directory_name = full_directory;
		#elif defined( __LINUX__ )
								directory()->_directory_name = directory_name_ptr;
								directory()->_directory_name = directory()->_directory_name.ReplaceAll( '\\' , '/' );						
								symbol = directory()->_directory_name.get_UTF8Char( directory()->_directory_name.Length() - 1 );
								if( symbol == '/' )
									directory()->_directory_name += dirent_object_ptr->d_name;		
								else
								{
									directory()->_directory_name += "/";
									directory()->_directory_name += dirent_object_ptr->d_name;		
								}
		#endif
								directories.Add( directory );
							}
						}
					}
					closedir( dir_ptr );
					::rux::engine::free_mem( dirent_object_ptr );	
				}
		#endif
			}	
		};
		rux::Array<rux::io::XDirectory>& rux::io::XDirectory::GetDirectories( const rux::XString& directory_name , XSearchOption search_option )
		{
			rux::XArray<rux::io::XDirectory> directories;
			if( search_option == XAllDirectories )
			{		
				if( directory_name.Length() > 0 )
				{
					rux::XString local_directory_name( directory_name );			
		#ifdef __WINDOWS__			
					local_directory_name = local_directory_name.ReplaceAll( '\\' , '/' );	
					rux::uint32 symbol = local_directory_name.get_UTF8Char( local_directory_name.Length() - 1 );
					if( symbol == '/' )
						local_directory_name += L"*";
					else
						local_directory_name += L"/*";			
					local_directory_name.set_ByRef( local_directory_name.ConvertToUTF16() );
					HANDLE find_handle = NULL;
					WIN32_FIND_DATAW win32_find_data;
					if( ( find_handle = FindFirstFileW( (wchar_t*)local_directory_name.str() , &win32_find_data ) ) != INVALID_HANDLE_VALUE )
					{
						rux::io::XDirectory directory;
						do
						{
							if( ( win32_find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
							{
								if( XString::IsEqualUTF16( (uint16*)win32_find_data.cFileName , (uint16*)L"." ) == 0
									&& XString::IsEqualUTF16( (uint16*)win32_find_data.cFileName , (uint16*)L".." ) == 0 )
								{
									directory = rux::io::XDirectory();
									directory()->_directory_name = directory_name;
									directory()->_directory_name = directory()->_directory_name.ReplaceAll( '\\' , '/' );
									symbol = directory()->_directory_name.get_UTF8Char( directory()->_directory_name.Length() - 1 );
									if( symbol == '/' )
										directory()->_directory_name += 	win32_find_data.cFileName;		
									else
									{
										directory()->_directory_name += "/";
										directory()->_directory_name += win32_find_data.cFileName;		
									}							
									directories.Add( directory );							
								}
							}
						}
						while( FindNextFileW( find_handle , &win32_find_data ) );
						FindClose( find_handle );
						for( rux::uint32 index1 = 0 ; index1 < directories.Count() ; index1++ )
							GetDirectories( directories[ index1 ]._directory_name , XTopDirectoryOnly , directories );				
					}
			#endif 
			#ifdef __UNIX__
					local_directory_name.set_ByRef( local_directory_name.ReplaceAll( '\\' , '/' ) );
					local_directory_name.set_ByRef( local_directory_name.ConvertToUTF8() );
					DIR* dir_ptr = opendir( local_directory_name.str() );
					if( dir_ptr )
					{
						::rux::int32 name_max = pathconf( local_directory_name.str() , _PC_NAME_MAX );
						if( name_max == -1 )
							name_max = 255;
						size_t dirent_len = offsetof( struct dirent , d_name ) + name_max + 1;
						dirent* dirent_object_ptr = (dirent*)alloc_array_macros( char* , dirent_len );
						dirent* dirent_result = 0;
						rux::io::XDirectory directory;
						rux::uint32 symbol = 0;
		#ifdef __SOLARIS__
						struct stat io_stat;
						rux::XString full_directory;
		#endif
						while( readdir_r( dir_ptr , dirent_object_ptr , &dirent_result ) == 0 && dirent_result )
						{
							if( XString::IsEqualA( dirent_object_ptr->d_name , "." ) == 0
								&& XString::IsEqualA( dirent_object_ptr->d_name , ".." ) == 0 )
							{
		#ifdef __LINUX__
								if( dirent_object_ptr->d_type == DT_DIR )
		#elif defined( __SOLARIS__ )
								full_directory = directory_name;
								full_directory = full_directory.ReplaceAll( '\\' , '/' );
								symbol = full_directory.get_UTF8Char( full_directory.Length() - 1 );
								if( symbol == '/' )
									full_directory += dirent_object_ptr->d_name;
								else
								{
									full_directory += "/";                                                        
									full_directory += dirent_object_ptr->d_name;
								}
								stat( full_directory.str() , &io_stat );
								if( io_stat.st_mode & S_IFDIR )
		#endif
								{					
									directory = rux::io::XDirectory();
		#ifdef __SOLARIS__
									directory()->_directory_name = full_directory;
		#elif defined( __LINUX__ )
									directory()->_directory_name = directory_name;
									directory()->_directory_name = directory()->_directory_name.ReplaceAll( '\\' , '/' );						
									symbol = directory()->_directory_name.get_UTF8Char( directory()->_directory_name.Length() - 1 );
									if( symbol == '/' )
										directory()->_directory_name += dirent_object_ptr->d_name;		
									else
									{
										directory()->_directory_name += "/";
										directory()->_directory_name += dirent_object_ptr->d_name;		
									}
		#endif
									directories.Add( directory );
								}
							}
						}
						closedir( dir_ptr );
						::rux::engine::free_mem( dirent_object_ptr );
						for( rux::uint32 index1 = 0 ; index1 < directories.Count() ; index1++ )
							GetDirectories( directories[ index1 ]._directory_name , XTopDirectoryOnly , directories );				
					}
		#endif
				}	
			}
			else
			{
				if( directory_name.Length() > 0 )
				{
					rux::XString local_directory_name( directory_name );			
		#ifdef __WINDOWS__			
					local_directory_name = local_directory_name.ReplaceAll( '\\' , '/' );	
					rux::uint32 symbol = local_directory_name.get_UTF8Char( local_directory_name.Length() - 1 );
					if( symbol == '/' )
						local_directory_name += L"*";
					else
						local_directory_name += L"/*";			
					local_directory_name.set_ByRef( local_directory_name.ConvertToUTF16() );
					HANDLE find_handle = NULL;
					WIN32_FIND_DATAW win32_find_data;
					if( ( find_handle = FindFirstFileW( (wchar_t*)local_directory_name.str() , &win32_find_data ) ) != INVALID_HANDLE_VALUE )
					{
						rux::io::XDirectory directory;
						do
						{
							if( ( win32_find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
							{
								if( XString::IsEqualUTF16( (uint16*)win32_find_data.cFileName , (uint16*)L"." ) == 0
									&& XString::IsEqualUTF16( (uint16*)win32_find_data.cFileName , (uint16*)L".." ) == 0 )
								{
									directory = rux::io::XDirectory();
									directory()->_directory_name = directory_name;
									directory()->_directory_name = directory()->_directory_name.ReplaceAll( '\\' , '/' );
									symbol = directory()->_directory_name.get_UTF8Char( directory()->_directory_name.Length() - 1 );
									if( symbol == '/' )
										directory()->_directory_name += 	win32_find_data.cFileName;		
									else
									{
										directory()->_directory_name += "/";
										directory()->_directory_name += win32_find_data.cFileName;		
									}							
									directories.Add( directory );
								}
							}
						}
						while( FindNextFileW( find_handle , &win32_find_data ) );
						FindClose( find_handle );
					}		
		#endif
		#ifdef __UNIX__
					local_directory_name.set_ByRef( local_directory_name.ReplaceAll( '\\' , '/' ) );
					local_directory_name.set_ByRef( local_directory_name.ConvertToUTF8() );
					DIR* dir_ptr = opendir( local_directory_name.str() );
					if( dir_ptr )
					{
						::rux::int32 name_max = pathconf( local_directory_name.str() , _PC_NAME_MAX );
						if( name_max == -1 )
							name_max = 255;
						size_t dirent_len = offsetof( struct dirent , d_name ) + name_max + 1;
						dirent* dirent_object_ptr = (dirent*)alloc_array_macros( char* , dirent_len );
						dirent* dirent_result = 0;
						rux::io::XDirectory directory;
						rux::uint32 symbol = 0;
		#ifdef __SOLARIS__
						struct stat io_stat;
						rux::XString full_directory;
		#endif
						while( readdir_r( dir_ptr , dirent_object_ptr , &dirent_result ) == 0 && dirent_result )
						{
							if( XString::IsEqualA( dirent_object_ptr->d_name , "." ) == 0
								&& XString::IsEqualA( dirent_object_ptr->d_name , ".." ) == 0 )
							{
		#ifdef __LINUX__
								if( dirent_object_ptr->d_type == DT_DIR )
		#elif defined( __SOLARIS__ )
								full_directory = directory_name;
								full_directory = full_directory.ReplaceAll( '\\' , '/' );
								symbol = full_directory.get_UTF8Char( full_directory.Length() - 1 );
								if( symbol == '/' )
									full_directory += dirent_object_ptr->d_name;
								else
								{
									full_directory += "/";
									full_directory += dirent_object_ptr->d_name;
								}
								stat( full_directory.str() , &io_stat );
								if( io_stat.st_mode & S_IFDIR );
		#endif
								{					
									directory = rux::io::XDirectory();
		#ifdef __SOLARIS__
									directory()->_directory_name = full_directory;
		#elif defined( __LINUX__ )
									directory()->_directory_name = directory_name;
									directory()->_directory_name = directory()->_directory_name.ReplaceAll( '\\' , '/' );						
									symbol = directory()->_directory_name.get_UTF8Char( directory()->_directory_name.Length() - 1 );
									if( symbol == '/' )
										directory()->_directory_name += dirent_object_ptr->d_name;		
									else
									{
										directory()->_directory_name += "/";
										directory()->_directory_name += dirent_object_ptr->d_name;		
									}
		#endif
									directories.Add( directory );
								}
							}
						}
						closedir( dir_ptr );
						::rux::engine::free_mem( dirent_object_ptr );	
					}
		#endif
				}
			}
			return directories++;
		};
		rux::Array<rux::io::XDirectory>& rux::io::XDirectory::GetDirectories( const char* directory_name_ptr , XSearchOption search_option )
		{
			rux::XArray<rux::io::XDirectory> directories;
			if( search_option == XAllDirectories )
			{		
				if( directory_name_ptr && XString::LengthA( directory_name_ptr ) > 0 )
				{
					rux::XString directory_name( directory_name_ptr );
		#ifdef __WINDOWS__
					directory_name = directory_name.ReplaceAll( '\\' , '/' );	
					rux::uint32 symbol = directory_name.get_UTF8Char( directory_name.Length() - 1 );
					if( symbol == '/' )
						directory_name += L"*";
					else
						directory_name += L"/*";
					directory_name.set_ByRef( directory_name.ConvertToUTF16() );
					HANDLE find_handle = NULL;
					WIN32_FIND_DATAW win32_find_data;
					if( ( find_handle = FindFirstFileW( (wchar_t*)directory_name.str() , &win32_find_data ) ) != INVALID_HANDLE_VALUE )
					{
						rux::io::XDirectory directory;				
						do
						{
							if( ( win32_find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
							{
								if( XString::IsEqualUTF16( (uint16*)win32_find_data.cFileName , (uint16*)L"." ) == 0
									&& XString::IsEqualUTF16( (uint16*)win32_find_data.cFileName , (uint16*)L".." ) == 0 )
								{
									directory = rux::io::XDirectory();
									directory()->_directory_name = directory_name_ptr;
									directory()->_directory_name = directory()->_directory_name.ReplaceAll( '\\' , '/' );
									symbol = directory()->_directory_name.get_UTF8Char( directory()->_directory_name.Length() - 1 );
									if( symbol == '/' )
										directory()->_directory_name += 	win32_find_data.cFileName;		
									else
									{
										directory()->_directory_name += "/";
										directory()->_directory_name += win32_find_data.cFileName;		
									}							
									directories.Add( directory );							
								}
							}
						}
						while( FindNextFileW( find_handle , &win32_find_data ) );
						FindClose( find_handle );
						for( rux::uint32 index1 = 0 ; index1 < directories.Count() ; index1++ )
							GetDirectories( directories[ index1 ]._directory_name , XTopDirectoryOnly , directories );				
					}
			#endif
			#ifdef __UNIX__
					directory_name.set_ByRef( directory_name.ReplaceAll( '\\' , '/' ) );
					directory_name.set_ByRef( directory_name.ConvertToUTF8() );
					DIR* dir_ptr = opendir( directory_name.str() );
					if( dir_ptr )
					{
						::rux::int32 name_max = pathconf( directory_name.str() , _PC_NAME_MAX );
						if( name_max == -1 )
							name_max = 255;
						size_t dirent_len = offsetof( struct dirent , d_name ) + name_max + 1;
						dirent* dirent_object_ptr = (dirent*)alloc_array_macros( char* , dirent_len );
						dirent* dirent_result = 0;
						rux::io::XDirectory directory;
						rux::uint32 index1 = directories.Count();
						rux::uint32 symbol = 0;
		#ifdef __SOLARIS__
						struct stat io_stat;
						XString full_directory;
		#endif
						while( readdir_r( dir_ptr , dirent_object_ptr , &dirent_result ) == 0 && dirent_result )
						{
							if( XString::IsEqualA( dirent_object_ptr->d_name , "." ) == 0
								&& XString::IsEqualA( dirent_object_ptr->d_name , ".." ) == 0 )
							{
		#ifdef __LINUX__
								if( dirent_object_ptr->d_type == DT_DIR )
		#elif defined( __SOLARIS__ )
														full_directory = directory_name;
														full_directory = full_directory.ReplaceAll( '\\' , '/' );
														symbol = full_directory.get_UTF8Char( full_directory.Length() - 1 );
														if( symbol == '/' )
																full_directory += dirent_object_ptr->d_name;
														else
														{
																full_directory += "/";
																full_directory += dirent_object_ptr->d_name;
														}
														stat( full_directory.str() , &io_stat );
														if( io_stat.st_mode & S_IFDIR )
		#endif
								{					
									directory = rux::io::XDirectory();
		#ifdef __SOLARIS__
																directory()->_directory_name = full_directory;
		#elif defined( __LINUX__ )
									directory()->_directory_name = directory_name_ptr;
									directory()->_directory_name = directory()->_directory_name.ReplaceAll( '\\' , '/' );						
									symbol = directory()->_directory_name.get_UTF8Char( directory()->_directory_name.Length() - 1 );
									if( symbol == '/' )
										directory()->_directory_name += dirent_object_ptr->d_name;		
									else
									{
										directory()->_directory_name += "/";
										directory()->_directory_name += dirent_object_ptr->d_name;		
									}
		#endif
									directories.Add( directory );
								}
							}
						}
						closedir( dir_ptr );
						::rux::engine::free_mem( dirent_object_ptr );
						for( ; index1 < directories.Count() ; index1++ )
							GetDirectories( directories[ index1 ]._directory_name , XTopDirectoryOnly , directories );
					}
			#endif
				}	
			}
			else
			{
				if( directory_name_ptr && XString::LengthA( directory_name_ptr ) > 0 )
				{
					rux::XString directory_name( directory_name_ptr );
		#ifdef __WINDOWS__
					directory_name = directory_name.ReplaceAll( '\\' , '/' );	
					rux::uint32 symbol = directory_name.get_UTF8Char( directory_name.Length() - 1 );
					if( symbol == '/' )
						directory_name += L"*";
					else
						directory_name += L"/*";			
					directory_name.set_ByRef( directory_name.ConvertToUTF16() );
					HANDLE find_handle = NULL;
					WIN32_FIND_DATAW win32_find_data;
					if( ( find_handle = FindFirstFileW( (wchar_t*)directory_name.str() , &win32_find_data ) ) != INVALID_HANDLE_VALUE )
					{
						rux::io::XDirectory directory;
						do
						{
							if( ( win32_find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
							{
								if( XString::IsEqualUTF16( (uint16*)win32_find_data.cFileName , (uint16*)L"." ) == 0
									&& XString::IsEqualUTF16( (uint16*)win32_find_data.cFileName , (uint16*)L".." ) == 0 )
								{
									directory = rux::io::XDirectory();
									directory()->_directory_name = directory_name_ptr;
									directory()->_directory_name = directory()->_directory_name.ReplaceAll( '\\' , '/' );
									symbol = directory()->_directory_name.get_UTF8Char( directory()->_directory_name.Length() - 1 );
									if( symbol == '/' )
										directory()->_directory_name += 	win32_find_data.cFileName;		
									else
									{
										directory()->_directory_name += "/";
										directory()->_directory_name += win32_find_data.cFileName;		
									}							
									directories.Add( directory );
								}
							}
						}
						while( FindNextFileW( find_handle , &win32_find_data ) );
						FindClose( find_handle );
					}		
		#endif
		#ifdef __UNIX__
					directory_name.set_ByRef( directory_name.ReplaceAll( '\\' , '/' ) );
					directory_name.set_ByRef( directory_name.ConvertToUTF8() );
					DIR* dir_ptr = opendir( directory_name.str() );
					if( dir_ptr )
					{
						::rux::int32 name_max = pathconf( directory_name.str() , _PC_NAME_MAX );
						if( name_max == -1 )
							name_max = 255;
						size_t dirent_len = offsetof( struct dirent , d_name ) + name_max + 1;
						dirent* dirent_object_ptr = (dirent*)alloc_array_macros( char* , dirent_len );
						dirent* dirent_result = 0;
						rux::io::XDirectory directory;				
						rux::uint32 symbol = 0;
		#ifdef __SOLARIS__
						struct stat io_stat;
						XString full_directory;
		#endif
						while( readdir_r( dir_ptr , dirent_object_ptr , &dirent_result ) == 0 && dirent_result )
						{
							if( XString::IsEqualA( dirent_object_ptr->d_name , "." ) == 0
								&& XString::IsEqualA( dirent_object_ptr->d_name , ".." ) == 0 )
							{
		#ifdef __LINUX__
								if( dirent_object_ptr->d_type == DT_DIR )
		#elif defined( __SOLARIS__ )
														full_directory = directory_name;
														full_directory = full_directory.ReplaceAll( '\\' , '/' );
														symbol = full_directory.get_UTF8Char( full_directory.Length() - 1 );
														if( symbol == '/' )
																full_directory += dirent_object_ptr->d_name;
														else
														{
																full_directory += "/";
																full_directory += dirent_object_ptr->d_name;
														}
														stat( full_directory.str() , &io_stat );
														if( io_stat.st_mode & S_IFDIR )
		#endif
								{					
									directory = rux::io::XDirectory();
		#ifdef __SOLARIS__
																directory()->_directory_name = full_directory;
		#elif defined( __LINUX__ )
									directory()->_directory_name = directory_name_ptr;
									directory()->_directory_name = directory()->_directory_name.ReplaceAll( '\\' , '/' );						
									symbol = directory()->_directory_name.get_UTF8Char( directory()->_directory_name.Length() - 1 );
									if( symbol == '/' )
										directory()->_directory_name += dirent_object_ptr->d_name;		
									else
									{
										directory()->_directory_name += "/";
										directory()->_directory_name += dirent_object_ptr->d_name;		
									}
		#endif
									directories.Add( directory );
								}
							}
						}
						closedir( dir_ptr );
						::rux::engine::free_mem( dirent_object_ptr );
					}
		#endif
				}
			}
			return directories++;
		};
		void rux::io::XDirectory::GetDirectories( const rux::XString& directory_name , XSearchOption search_option , rux::XArray<rux::io::XDirectory>& directories )
		{	
			if( search_option == XAllDirectories )
			{		
				if( directory_name.Length() > 0 )
				{
					rux::XString local_directory_name( directory_name );			
		#ifdef __WINDOWS__
					local_directory_name = local_directory_name.ReplaceAll( '\\' , '/' );	
					rux::uint32 symbol = local_directory_name.get_UTF8Char( local_directory_name.Length() - 1 );
					if( symbol == '/' )
						local_directory_name += L"*";
					else
						local_directory_name += L"/*";			
					local_directory_name.set_ByRef( local_directory_name.ConvertToUTF16() );
					HANDLE find_handle = NULL;
					WIN32_FIND_DATAW win32_find_data;
					if( ( find_handle = FindFirstFileW( (wchar_t*)local_directory_name.str() , &win32_find_data ) ) != INVALID_HANDLE_VALUE )
					{
						rux::io::XDirectory directory;
						rux::uint32 index1 = directories.Count();
						do
						{
							if( ( win32_find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
							{
								if( XString::IsEqualUTF16( (uint16*)win32_find_data.cFileName , (uint16*)L"." ) == 0
									&& XString::IsEqualUTF16( (uint16*)win32_find_data.cFileName , (uint16*)L".." ) == 0 )
								{
									directory = rux::io::XDirectory();
									directory()->_directory_name = directory_name;
									directory()->_directory_name = directory()->_directory_name.ReplaceAll( '\\' , '/' );
									symbol = directory()->_directory_name.get_UTF8Char( directory()->_directory_name.Length() - 1 );
									if( symbol == '/' )
										directory()->_directory_name += 	win32_find_data.cFileName;		
									else
									{
										directory()->_directory_name += "/";
										directory()->_directory_name += win32_find_data.cFileName;		
									}							
									directories.Add( directory );							
								}
							}
						}
						while( FindNextFileW( find_handle , &win32_find_data ) );
						FindClose( find_handle );
						for( ; index1 < directories.Count() ; index1++ )
							GetDirectories( directories[ index1 ]._directory_name , XTopDirectoryOnly , directories );				
					}
			#endif
			#ifdef __UNIX__
					local_directory_name.set_ByRef( local_directory_name.ReplaceAll( '\\' , '/' ) );
					local_directory_name.set_ByRef( local_directory_name.ConvertToUTF8() );
					DIR* dir_ptr = opendir( local_directory_name.str() );
					if( dir_ptr )
					{
						::rux::int32 name_max = pathconf( local_directory_name.str() , _PC_NAME_MAX );
						if( name_max == -1 )
							name_max = 255;
						size_t dirent_len = offsetof( struct dirent , d_name ) + name_max + 1;
						dirent* dirent_object_ptr = (dirent*)alloc_array_macros( char* , dirent_len );
						dirent* dirent_result = 0;
						rux::io::XDirectory directory;
						rux::uint32 index1 = directories.Count();
						rux::uint32 symbol = 0;
		#ifdef __SOLARIS__
										struct stat io_stat;
										XString full_directory;
		#endif
						while( readdir_r( dir_ptr , dirent_object_ptr , &dirent_result ) == 0 && dirent_result )
						{
							if( XString::IsEqualA( dirent_object_ptr->d_name , "." ) == 0
								&& XString::IsEqualA( dirent_object_ptr->d_name , ".." ) == 0 )
							{
		#ifdef __LINUX__
								if( dirent_object_ptr->d_type == DT_DIR )
		#elif defined( __SOLARIS__ )
														full_directory = directory_name;
														full_directory = full_directory.ReplaceAll( '\\' , '/' );
														symbol = full_directory.get_UTF8Char( full_directory.Length() - 1 );
														if( symbol == '/' )
																full_directory += dirent_object_ptr->d_name;
														else
														{
																full_directory += "/";
																full_directory += dirent_object_ptr->d_name;
														}
														stat( full_directory.str() , &io_stat );
														if( io_stat.st_mode & S_IFDIR )
		#endif
								{					
									directory = rux::io::XDirectory();
		#ifdef __SOLARIS__
																directory()->_directory_name = full_directory;
		#elif defined( __LINUX__ )
									directory()->_directory_name = directory_name;
									directory()->_directory_name = directory()->_directory_name.ReplaceAll( '\\' , '/' );						
									symbol = directory()->_directory_name.get_UTF8Char( directory()->_directory_name.Length() - 1 );
									if( symbol == '/' )
										directory()->_directory_name += dirent_object_ptr->d_name;		
									else
									{
										directory()->_directory_name += "/";
										directory()->_directory_name += dirent_object_ptr->d_name;		
									}
		#endif
									directories.Add( directory );
								}
							}
						}
						closedir( dir_ptr );
						::rux::engine::free_mem( dirent_object_ptr );	
						for( ; index1 < directories.Count() ; index1++ )
							GetDirectories( directories[ index1 ]._directory_name , XTopDirectoryOnly , directories );				
					}
			#endif
				}	
			}
			else
			{
				if( directory_name.Length() > 0 )
				{
					rux::XString local_directory_name( directory_name );			
		#ifdef __WINDOWS__
					local_directory_name = local_directory_name.ReplaceAll( '\\' , '/' );	
					rux::uint32 symbol = local_directory_name.get_UTF8Char( local_directory_name.Length() - 1 );
					if( symbol == '/' )
						local_directory_name += L"*";
					else
						local_directory_name += L"/*";			
					local_directory_name.set_ByRef( local_directory_name.ConvertToUTF16() );
					HANDLE find_handle = NULL;
					WIN32_FIND_DATAW win32_find_data;
					if( ( find_handle = FindFirstFileW( (wchar_t*)local_directory_name.str() , &win32_find_data ) ) != INVALID_HANDLE_VALUE )
					{
						rux::io::XDirectory directory;
						do
						{
							if( ( win32_find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
							{
								if( XString::IsEqualUTF16( (uint16*)win32_find_data.cFileName , (uint16*)L"." ) == 0
									&& XString::IsEqualUTF16( (uint16*)win32_find_data.cFileName , (uint16*)L".." ) == 0 )
								{
									directory = rux::io::XDirectory();
									directory()->_directory_name = directory_name;
									directory()->_directory_name = directory()->_directory_name.ReplaceAll( '\\' , '/' );
									symbol = directory()->_directory_name.get_UTF8Char( directory()->_directory_name.Length() - 1 );
									if( symbol == '/' )
										directory()->_directory_name +=	win32_find_data.cFileName;		
									else
									{
										directory()->_directory_name += "/";
										directory()->_directory_name += win32_find_data.cFileName;		
									}							
									directories.Add( directory );
								}
							}
						}
						while( FindNextFileW( find_handle , &win32_find_data ) );
						FindClose( find_handle );
					}		
		#endif
		#ifdef __UNIX__
					local_directory_name.set_ByRef( local_directory_name.ReplaceAll( '\\' , '/' ) );
					local_directory_name.set_ByRef( local_directory_name.ConvertToUTF8() );
					DIR* dir_ptr = opendir( local_directory_name.str() );
					if( dir_ptr )
					{
						::rux::int32 name_max = pathconf( local_directory_name.str() , _PC_NAME_MAX );
						if( name_max == -1 )
							name_max = 255;
						size_t dirent_len = offsetof( struct dirent , d_name ) + name_max + 1;
						dirent* dirent_object_ptr = (dirent*)alloc_array_macros( char* , dirent_len );
						dirent* dirent_result = 0;
						rux::io::XDirectory directory;				
						rux::uint32 symbol = 0;
		#ifdef __SOLARIS__
										struct stat io_stat;
										XString full_directory;
		#endif
						while( readdir_r( dir_ptr , dirent_object_ptr , &dirent_result ) == 0 && dirent_result )
						{
							if( XString::IsEqualA( dirent_object_ptr->d_name , "." ) == 0
								&& XString::IsEqualA( dirent_object_ptr->d_name , ".." ) == 0 )
							{
		#ifdef __LINUX__
								if( dirent_object_ptr->d_type == DT_DIR )
		#elif defined( __SOLARIS__ )
														full_directory = directory_name;
														full_directory = full_directory.ReplaceAll( '\\' , '/' );
														symbol = full_directory.get_UTF8Char( full_directory.Length() - 1 );
														if( symbol == '/' )
																full_directory += dirent_object_ptr->d_name;
														else
														{
																full_directory += "/";
																full_directory += dirent_object_ptr->d_name;
														}
														stat( full_directory.str() , &io_stat );
														if( io_stat.st_mode & S_IFDIR )
		#endif
								{					
									directory = rux::io::XDirectory();
		#ifdef __SOLARIS__
																directory()->_directory_name = full_directory;
		#elif defined( __LINUX__ )
									directory()->_directory_name = directory_name;
									directory()->_directory_name = directory()->_directory_name.ReplaceAll( '\\' , '/' );						
									symbol = directory()->_directory_name.get_UTF8Char( directory()->_directory_name.Length() - 1 );
									if( symbol == '/' )
										directory()->_directory_name += dirent_object_ptr->d_name;		
									else
									{
										directory()->_directory_name += "/";
										directory()->_directory_name += dirent_object_ptr->d_name;		
									}
		#endif
									directories.Add( directory );
								}
							}
						}
						closedir( dir_ptr );
						::rux::engine::free_mem( dirent_object_ptr );					
					}
		#endif
				}
			}	
		};
		void rux::io::XDirectory::GetDirectories( const char* directory_name_ptr , XSearchOption search_option , rux::XArray<rux::io::XDirectory>& directories )
		{	
			if( search_option == XAllDirectories )
			{		
				if( directory_name_ptr && XString::LengthA( directory_name_ptr ) > 0 )
				{
					rux::XString directory_name( directory_name_ptr );
		#ifdef __WINDOWS__
					directory_name = directory_name.ReplaceAll( '\\' , '/' );	
					rux::uint32 symbol = directory_name.get_UTF8Char( directory_name.Length() - 1 );
					if( symbol == '/' )
						directory_name += L"*";
					else
						directory_name += L"/*";			
					directory_name.set_ByRef( directory_name.ConvertToUTF16() );
					HANDLE find_handle = NULL;
					WIN32_FIND_DATAW win32_find_data;
					if( ( find_handle = FindFirstFileW( (wchar_t*)directory_name.str() , &win32_find_data ) ) != INVALID_HANDLE_VALUE )
					{
						rux::io::XDirectory directory;
						rux::uint32 index1 = directories.Count();
						do
						{
							if( ( win32_find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
							{
								if( XString::IsEqualUTF16( (uint16*)win32_find_data.cFileName , (uint16*)L"." ) == 0
									&& XString::IsEqualUTF16( (uint16*)win32_find_data.cFileName , (uint16*)L".." ) == 0 )
								{
									directory = rux::io::XDirectory();
									directory()->_directory_name = directory_name_ptr;
									directory()->_directory_name = directory()->_directory_name.ReplaceAll( '\\' , '/' );
									symbol = directory()->_directory_name.get_UTF8Char( directory()->_directory_name.Length() - 1 );
									if( symbol == '/' )
										directory()->_directory_name += 	win32_find_data.cFileName;		
									else
									{
										directory()->_directory_name += "/";
										directory()->_directory_name += win32_find_data.cFileName;		
									}							
									directories.Add( directory );							
								}
							}
						}
						while( FindNextFileW( find_handle , &win32_find_data ) );
						FindClose( find_handle );
						for( ; index1 < directories.Count() ; index1++ )
							GetDirectories( directories[ index1 ]._directory_name , XTopDirectoryOnly , directories );				
					}
			#endif
			#ifdef __UNIX__
					directory_name.set_ByRef( directory_name.ReplaceAll( '\\' , '/' ) );
					directory_name.set_ByRef( directory_name.ConvertToUTF8() );
					DIR* dir_ptr = opendir( directory_name.str() );
					if( dir_ptr )
					{
						::rux::int32 name_max = pathconf( directory_name.str() , _PC_NAME_MAX );
						if( name_max == -1 )
							name_max = 255;
						size_t dirent_len = offsetof( struct dirent , d_name ) + name_max + 1;
						dirent* dirent_object_ptr = (dirent*)alloc_array_macros( char* , dirent_len );
						dirent* dirent_result = 0;
						rux::io::XDirectory directory;
						rux::uint32 index1 = directories.Count();
						rux::uint32 symbol = 0;
		#ifdef __SOLARIS__
										struct stat io_stat;
										XString full_directory;
		#endif
						while( readdir_r( dir_ptr , dirent_object_ptr , &dirent_result ) == 0 && dirent_result )
						{
							if( XString::IsEqualA( dirent_object_ptr->d_name , "." ) == 0
								&& XString::IsEqualA( dirent_object_ptr->d_name , ".." ) == 0 )
							{
		#ifdef __LINUX__
								if( dirent_object_ptr->d_type == DT_DIR )
		#elif defined( __SOLARIS__ )
														full_directory = directory_name_ptr;
														full_directory = full_directory.ReplaceAll( '\\' , '/' );
														symbol = full_directory.get_UTF8Char( full_directory.Length() - 1 );
														if( symbol == '/' )
																full_directory += dirent_object_ptr->d_name;
														else
														{
																full_directory += "/";
																full_directory += dirent_object_ptr->d_name;
														}
														stat( full_directory.str() , &io_stat );
														if( io_stat.st_mode & S_IFDIR )
		#endif
								{					
									directory = rux::io::XDirectory();
		#ifdef __SOLARIS__
																directory()->_directory_name = full_directory;
		#elif defined( __LINUX__ )
									directory()->_directory_name = directory_name_ptr;
									directory()->_directory_name = directory()->_directory_name.ReplaceAll( '\\' , '/' );						
									symbol = directory()->_directory_name.get_UTF8Char( directory()->_directory_name.Length() - 1 );
									if( symbol == '/' )
										directory()->_directory_name += dirent_object_ptr->d_name;		
									else
									{
										directory()->_directory_name += "/";
										directory()->_directory_name += dirent_object_ptr->d_name;		
									}
		#endif
									directories.Add( directory );
								}
							}
						}
						closedir( dir_ptr );
						::rux::engine::free_mem( dirent_object_ptr );
						for( ; index1 < directories.Count() ; index1++ )
							GetDirectories( directories[ index1 ]._directory_name , XTopDirectoryOnly , directories );
					}
		#endif
				}	
			}
			else
			{
				if( directory_name_ptr && XString::LengthA( directory_name_ptr ) > 0 )
				{
					rux::XString directory_name( directory_name_ptr );
		#ifdef __WINDOWS__
					directory_name = directory_name.ReplaceAll( '\\' , '/' );	
					rux::uint32 symbol = directory_name.get_UTF8Char( directory_name.Length() - 1 );
					if( symbol == '/' )
						directory_name += L"*";
					else
						directory_name += L"/*";			
					directory_name.set_ByRef( directory_name.ConvertToUTF16() );
					HANDLE find_handle = NULL;
					WIN32_FIND_DATAW win32_find_data;
					if( ( find_handle = FindFirstFileW( (wchar_t*)directory_name.str() , &win32_find_data ) ) != INVALID_HANDLE_VALUE )
					{
						rux::io::XDirectory directory;
						do
						{
							if( ( win32_find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
							{
								if( XString::IsEqualUTF16( (uint16*)win32_find_data.cFileName , (uint16*)L"." ) == 0
									&& XString::IsEqualUTF16( (uint16*)win32_find_data.cFileName , (uint16*)L".." ) == 0 )
								{
									directory = rux::io::XDirectory();
									directory()->_directory_name = directory_name_ptr;
									directory()->_directory_name = directory()->_directory_name.ReplaceAll( '\\' , '/' );
									symbol = directory()->_directory_name.get_UTF8Char( directory()->_directory_name.Length() - 1 );
									if( symbol == '/' )
										directory()->_directory_name +=	win32_find_data.cFileName;		
									else
									{
										directory()->_directory_name += "/";
										directory()->_directory_name += win32_find_data.cFileName;		
									}							
									directories.Add( directory );
								}
							}
						}
						while( FindNextFileW( find_handle , &win32_find_data ) );
						FindClose( find_handle );
					}		
		#endif
		#ifdef __UNIX__
					directory_name.set_ByRef( directory_name.ReplaceAll( '\\' , '/' ) );
					directory_name.set_ByRef( directory_name.ConvertToUTF8() );
					DIR* dir_ptr = opendir( directory_name.str() );
					if( dir_ptr )
					{
						::rux::int32 name_max = pathconf( directory_name.str() , _PC_NAME_MAX );
						if( name_max == -1 )
							name_max = 255;
						size_t dirent_len = offsetof( struct dirent , d_name ) + name_max + 1;
						dirent* dirent_object_ptr = (dirent*)alloc_array_macros( char* , dirent_len );
						dirent* dirent_result = 0;
						rux::io::XDirectory directory;				
						rux::uint32 symbol = 0;
		#ifdef __SOLARIS__
										struct stat io_stat;
										XString full_directory;
		#endif
						while( readdir_r( dir_ptr , dirent_object_ptr , &dirent_result ) == 0 && dirent_result )
						{
							if( XString::IsEqualA( dirent_object_ptr->d_name , "." ) == 0
								&& XString::IsEqualA( dirent_object_ptr->d_name , ".." ) == 0 )
							{
		#ifdef __LINUX__
								if( dirent_object_ptr->d_type == DT_DIR )
		#elif defined( __SOLARIS__ )
														full_directory = directory_name;
														full_directory = full_directory.ReplaceAll( '\\' , '/' );
														symbol = full_directory.get_UTF8Char( full_directory.Length() - 1 );
														if( symbol == '/' )
																full_directory += dirent_object_ptr->d_name;
														else
														{
																full_directory += "/";
																full_directory += dirent_object_ptr->d_name;
														}
														stat( full_directory.str() , &io_stat );
														if( io_stat.st_mode & S_IFDIR )
		#endif
								{					
									directory = rux::io::XDirectory();
		#ifdef __SOLARIS__
																directory()->_directory_name = full_directory;
		#elif defined( __LINUX__ )
									directory()->_directory_name = directory_name_ptr;
									directory()->_directory_name = directory()->_directory_name.ReplaceAll( '\\' , '/' );						
									symbol = directory()->_directory_name.get_UTF8Char( directory()->_directory_name.Length() - 1 );
									if( symbol == '/' )
										directory()->_directory_name += dirent_object_ptr->d_name;		
									else
									{
										directory()->_directory_name += "/";
										directory()->_directory_name += dirent_object_ptr->d_name;		
									}
		#endif
									directories.Add( directory );
								}
							}
						}
						closedir( dir_ptr );
						::rux::engine::free_mem( dirent_object_ptr );
					}
		#endif
				}
			}	
		};
		void rux::io::XDirectory::Copy( const rux::XString& source_directory_name , const rux::XString& destination_directory_name , rux::XString& error_string )
		{
			error_string.Clear();
			rux::int32 error = 0;
			rux::XString source( source_directory_name );
			source = source.ReplaceAll( '\\' , '/' );
			rux::XString destination( destination_directory_name );
			destination = destination.ReplaceAll( '\\' , '/' );
			if( source.Length() > 0 && destination.Length() > 0 )
			{
				if( source.get_UTF8Char( source.Length() - 1 ) != '/' )
					source += "/";		
				if( destination.get_UTF8Char( destination.Length() - 1 ) != '/' )
					destination += "/";		
				rux::XArray<XString> parts = destination.Split( '/' );
				rux::uint32 index0 = 1;
				rux::XString string( parts[0] );
				while( index0 < parts.Count() )
				{
					string += "/" + parts[ index0 ];
		#ifdef __WINDOWS__
					if( string()->_code_page == XEnumCodePage_UTF16 )
					{
						if( _waccess( (wchar_t*)string.str() , 0 ) == -1 )
						{
							if( _wmkdir( (wchar_t*)string.str() ) != 0 )
								error = errno;
						}
						else
						{
							struct _stat st;
							if( _wstat( (wchar_t*)string.str() , &st ) == 0 )
							{
								if( ( st.st_mode & _S_IFDIR ) == 0 )
								{
									if( _wmkdir( (wchar_t*)string.str() ) != 0 )
										error = errno;
								}
							}
						}
					}
					else if( string()->_code_page == XEnumCodePage_CP1251 )
					{
						if( _access( string.str() , 00 ) == -1 )
						{
							if( _mkdir( string.str() ) != 0 )
								error = errno;
						}
						else
						{
							struct _stat st;
							if( _stat( string.str() , &st ) == 0 )
							{
								if( ( st.st_mode & _S_IFDIR ) == 0 )
								{
									if( _mkdir( string.str() ) != 0 )
										error = errno;
								}
							}
						}
					}
					else if( string()->_code_page == XEnumCodePage_UTF8 )
					{
						string = string.ConvertToUTF16();
						if( _waccess( (wchar_t*)string.str() , 0 ) == -1 )
						{
							if( _wmkdir( (wchar_t*)string.str() ) != 0 )
								error = errno;
						}
						else
						{
							struct _stat st;
							if( _wstat( (wchar_t*)string.str() , &st ) == 0 )
							{
								if( ( st.st_mode & _S_IFDIR ) == 0 )
								{
									if( _wmkdir( (wchar_t*)string.str() ) != 0 )
										error = errno;
								}
							}
						}
					}
		#endif
		#ifdef __UNIX__
					string = string.ConvertToUTF8();
					if( access( string.str() , F_OK ) == -1 )
					{
						if( mkdir( string.str() , S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH ) != 0 )
							error = errno;
					}
					else
					{
						struct stat st;
						if( stat( string.str() , &st ) == 0 )
						{
							if( S_ISDIR( st.st_mode ) == 0 )
							{
								if( mkdir( string.str() , S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH ) != 0 )
									error = errno;
							}
						}
					}
		#endif
					index0++;
				}					
				rux::XArray<rux::io::XDirectory> directories = GetDirectories( source , XAllDirectories );		
				index0 = 0;
				while( index0 < directories.Count() )
				{
					string = directories[ index0 ]._directory_name.ReplaceFirst( source , "" );
					string = destination + string;
		#ifdef __WINDOWS__
					if( string()->_code_page == XEnumCodePage_UTF16 )
					{
						if( _waccess( (wchar_t*)string.str() , 0 ) == -1 )
						{
							if( _wmkdir( (wchar_t*)string.str() ) != 0 )
								error = errno;
						}
						else
						{
							struct _stat st;
							if( _wstat( (wchar_t*)string.str() , &st ) == 0 )
							{
								if( ( st.st_mode & _S_IFDIR ) == 0 )
								{
									if( _wmkdir( (wchar_t*)string.str() ) != 0 )
										error = errno;
								}
							}
						}
					}
					else if( string()->_code_page == XEnumCodePage_CP1251 )
					{
						if( _access( string.str() , 0 ) == -1 )
						{
							if( _mkdir( string.str() ) != 0 )
								error = errno;
						}
						else
						{
							struct _stat st;
							if( _stat( string.str() , &st ) == 0 )
							{
								if( ( st.st_mode & _S_IFDIR ) == 0 )
								{
									if( _mkdir( string.str() ) != 0 )
										error = errno;
								}
							}
						}
					}
					else if( string()->_code_page == XEnumCodePage_UTF8 )
					{
						string = string.ConvertToUTF16();
						if( _waccess( (wchar_t*)string.str() , 0 ) == -1 )
						{
							if( _wmkdir( (wchar_t*)string.str() ) != 0 )
								error = errno;
						}
						else
						{
							struct _stat st;
							if( _wstat( (wchar_t*)string.str() , &st ) == 0 )
							{
								if( ( st.st_mode & _S_IFDIR ) == 0 )
								{
									if( _wmkdir( (wchar_t*)string.str() ) != 0 )
										error = errno;
								}
							}
						}
					}
		#endif
		#ifdef __UNIX__
					string = string.ConvertToUTF8();
					if( access( string.str() , F_OK ) == -1 )
					{
						if( mkdir( string.str() , S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH ) != 0 )
							error = errno;
					}
					else
					{
						struct stat st;
						if( stat( string.str() , &st ) == 0 )
						{
							if( S_ISDIR( st.st_mode ) == 0 )
							{
								if( mkdir( string.str() , S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH ) != 0 )
									error = errno;
							}
						}
					}
		#endif
					index0++;
				}
				rux::XArray<XFile> files = GetFiles( source , XAllDirectories );
				index0 = 0;		
				size_t readen_size = 0;
				rux::int32 buffer_ptr_size = 8 * 1024;
				char* buffer_ptr = alloc_array_macros( char , buffer_ptr_size );
				while( index0 < files.Count() )
				{
					string = files[ index0 ]._file_path.ReplaceFirst( source , "" );
					string = destination + string;			
					string.set_ByRef( string.ConvertToAnsi() );
					FILE* file_ptr = rux::io::fopen( string.str() , "w+b" );
					if( file_ptr )
					{
						XFile file_item( files[ index0 ] );
						file_item.ReadBytes( buffer_ptr , buffer_ptr_size , readen_size , error_string );				
						while( readen_size > 0 )
						{
							if( rux::io::fwrite( buffer_ptr , sizeof( char ) , readen_size , file_ptr ) != readen_size )
								error = errno;
							file_item.ReadBytes( buffer_ptr , buffer_ptr_size , readen_size , error_string );
						}
						rux::io::fclose( file_ptr );
					}
					else
						error = errno;
					index0++;
				}
				rux::engine::free_mem( buffer_ptr );
			}
		};
		void rux::io::XDirectory::Copy( const char* source_directory_name_ptr , const char* destination_directory_name_ptr , rux::XString& error_string )
		{
			error_string.Clear();
			rux::int32 error = 0;
			rux::XString source( source_directory_name_ptr );
			source = source.ReplaceAll( '\\' , '/' );
			rux::XString destination( destination_directory_name_ptr );
			destination = destination.ReplaceAll( '\\' , '/' );
			if( source.Length() > 0 && destination.Length() > 0 )
			{
				if( source.get_UTF8Char( source.Length() - 1 ) != '/' )
					source += "/";		
				if( destination.get_UTF8Char( destination.Length() - 1 ) != '/' )
					destination += "/";		
				rux::XArray<XString> parts = destination.Split( '/' );
				rux::uint32 index0 = 1;
				rux::XString string( parts[0] );
				while( index0 < parts.Count() )
				{
					string += "/" + parts[ index0 ];
		#ifdef __WINDOWS__
					if( string()->_code_page == XEnumCodePage_UTF16 )
					{
						if( _waccess( (wchar_t*)string.str() , 0 ) == -1 )
						{
							if( _wmkdir( (wchar_t*)string.str() ) != 0 )
								error = errno;
						}
						else
						{
							struct _stat st;
							if( _wstat( (wchar_t*)string.str() , &st ) == 0 )
							{
								if( ( st.st_mode & _S_IFDIR ) == 0 )
								{
									if( _wmkdir( (wchar_t*)string.str() ) != 0 )
										error = errno;
								}
							}
						}
					}
					else if( string()->_code_page == XEnumCodePage_UTF8 )
					{
						rux::XString utf16 = string.ConvertToUTF16();
						if( _waccess( (wchar_t*)utf16.str() , 0 ) == -1 )
						{
							if( _wmkdir( (wchar_t*)utf16.str() ) != 0 )
								error = errno;
						}
						else
						{
							struct _stat st;
							if( _wstat( (wchar_t*)utf16.str() , &st ) == 0 )
							{
								if( ( st.st_mode & _S_IFDIR ) == 0 )
								{
									if( _wmkdir( (wchar_t*)utf16.str() ) != 0 )
										error = errno;
								}
							}
						}
					}
					else if( string()->_code_page == XEnumCodePage_CP1251 )
					{
						if( _access( string.str() , 0 ) == -1 )
						{
							if( _mkdir( string.str() ) != 0 )
								error = errno;
						}				
						else
						{
							struct _stat st;
							if( _stat( string.str() , &st ) == 0 )
							{
								if( ( st.st_mode & _S_IFDIR ) == 0 )
								{
									if( _mkdir( string.str() ) != 0 )
										error = errno;
								}
							}
						}
					}
		#endif
		#ifdef __UNIX__
					string = string.ConvertToUTF8();
					if( access( string.str() , F_OK ) == -1 )
					{
						if( mkdir( string.str() , S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH ) != 0 )
							error = errno;
					}
					else
					{
						struct stat st;
						if( stat( string.str() , &st ) == 0 )
						{
							if( S_ISDIR( st.st_mode ) == 0 )
							{
								if( mkdir( string.str() , S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH ) != 0 )
									error = errno;
							}
						}
					}
		#endif
					index0++;
				}					
				rux::XArray<rux::io::XDirectory> directories = GetDirectories( source , XAllDirectories );		
				index0 = 0;
				while( index0 < directories.Count() )
				{
					string = directories[ index0 ]._directory_name.ReplaceFirst( source , "" );
					string = destination + string;
		#ifdef __WINDOWS__
					if( string()->_code_page == XEnumCodePage_UTF16 )
					{
						if( _waccess( (wchar_t*)string.str() , 0 ) == -1 )
						{
							if( _wmkdir( (wchar_t*)string.str() ) != 0 )
								error = errno;
						}
						else
						{
							struct _stat st;
							if( _wstat( (wchar_t*)string.str() , &st ) == 0 )
							{
								if( ( st.st_mode & _S_IFDIR ) == 0 )
								{
									if( _wmkdir( (wchar_t*)string.str() ) != 0 )
										error = errno;
								}
							}
						}
					}
					else if( string()->_code_page == XEnumCodePage_UTF8 )
					{
						string = string.ConvertToUTF16();
						if( _waccess( (wchar_t*)string.str() , 0 ) == -1 )
						{
							if( _wmkdir( (wchar_t*)string.str() ) != 0 )
								error = errno;
						}

						else
						{
							struct _stat st;
							if( _wstat( (wchar_t*)string.str() , &st ) == 0 )
							{
								if( ( st.st_mode & _S_IFDIR ) == 0 )
								{
									if( _wmkdir( (wchar_t*)string.str() ) != 0 )
										error = errno;
								}
							}
						}
					}
					else if( string()->_code_page == XEnumCodePage_CP1251 )
					{
						if( _access( string.str() , 0 ) == -1 )
						{
							if( _mkdir( string.str() ) != 0 )
								error = errno;
						}
						else
						{
							struct _stat st;
							if( _stat( string.str() , &st ) == 0 )
							{
								if( ( st.st_mode & _S_IFDIR ) == 0 )
								{
									if( _mkdir( string.str() ) != 0 )
										error = errno;
								}
							}
						}
					}
		#endif
		#ifdef __UNIX__
					string = string.ConvertToUTF8();
					if( access( string.str() , F_OK ) == -1 )
					{
						if( mkdir( string.str() , S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH ) != 0 )
							error = errno;
					}
					else
					{
						struct stat st;
						if( stat( string.str() , &st ) == 0 )
						{
							if( S_ISDIR( st.st_mode ) == 0 )
							{
								if( mkdir( string.str() , S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH ) != 0 )
									error = errno;
							}
						}
					}
		#endif
					index0++;
				}
				rux::XArray<XFile> files = GetFiles( source , XAllDirectories );
				index0 = 0;		
				size_t readen_size = 0;
				rux::int32 buffer_ptr_size = 8 * 1024;
				char* buffer_ptr = alloc_array_macros( char , buffer_ptr_size );
				while( index0 < files.Count() )
				{
					string = files[ index0 ]._file_path.ReplaceFirst( source , "" );
					string = destination + string;			
					string.set_ByRef( string.ConvertToAnsi() );
					FILE* file_ptr = rux::io::fopen( string.str() , "w+b" );
					if( file_ptr )
					{
						XFile file_item( files[ index0 ] );
						file_item.ReadBytes( buffer_ptr , buffer_ptr_size , readen_size , error_string );				
						while( readen_size > 0 )
						{
							if( static_cast<rux::int32>( rux::io::fwrite( buffer_ptr , sizeof( char ) , readen_size , file_ptr ) ) != readen_size )
								error = errno;
							file_item.ReadBytes( buffer_ptr , buffer_ptr_size , readen_size , error_string );
						}
						rux::io::fclose( file_ptr );
					}
					else
						error = errno;
					index0++;
				}
				rux::engine::free_mem( buffer_ptr );
			}
		};
		rux::uint8 rux::io::XDirectory::Exists( const char* directory_name_ptr )
		{
			rux::uint8 is_exists = 0;
			if( directory_name_ptr && directory_name_ptr[ 0 ] != 0 ) 
			{
		#ifdef __WINDOWS__
				is_exists = _access( directory_name_ptr , 0 ) != -1 ? 1 : 0;
				if( is_exists == 1 )
				{
					struct _stat st;
					if( _stat( directory_name_ptr , &st ) == 0 )
					{
						if( ( st.st_mode & _S_IFDIR ) == 0 )
							is_exists = 0;
					}
				}
		#endif
		#ifdef __UNIX__
				rux::XString utf8( directory_name_ptr );
				utf8.set_ByRef( utf8.ConvertToUTF8() );
				is_exists = access( utf8.str() , F_OK ) != -1 ? 1 : 0;
				if( is_exists == 1 )
				{
					struct stat st;
					if( stat( utf8.str() , &st ) == 0 )
					{
						if( S_ISDIR( st.st_mode ) == 0 )
							is_exists = 0;
					}
				}
		#endif
			}	
			return is_exists;
		};
		rux::uint8 rux::io::XDirectory::Exists( const rux::XString& directory_name )
		{
			rux::uint8 is_exists = 0;
			if( directory_name.Length() > 0 )
			{
		#ifdef __WINDOWS__
				if( directory_name()->_code_page == XEnumCodePage_UTF16 )
				{
					is_exists = _waccess( (wchar_t*)directory_name.str() , 0 ) != -1 ? 1 : 0;
					if( is_exists == 1 )
					{
						struct _stat st;
						if( _wstat( (wchar_t*)directory_name.str() , &st ) == 0 )
						{
							if( ( st.st_mode & _S_IFDIR ) == 0 )
								is_exists = 0;
						}
					}
				}
				else if( directory_name()->_code_page == XEnumCodePage_UTF8 )
				{
					rux::XString utf16;
					utf16.set_ByRef( directory_name.ConvertToUTF16() );			
					is_exists = _waccess( (wchar_t*)utf16.str() , 0 ) != -1 ? 1 : 0;
					if( is_exists == 1 )
					{
						struct _stat st;
						if( _wstat( (wchar_t*)utf16.str() , &st ) == 0 )
						{
							if( ( st.st_mode & _S_IFDIR ) == 0 )
								is_exists = 0;
						}
					}
				}
				else if( directory_name()->_code_page == XEnumCodePage_CP1251 )
				{
					is_exists = _access( directory_name.str() , 0 ) != -1 ? 1 : 0;
					if( is_exists == 1 )
					{
						struct _stat st;
						if( _stat( directory_name.str() , &st ) == 0 )
						{
							if( ( st.st_mode & _S_IFDIR ) == 0 )
								is_exists = 0;
						}
					}
				}
		#endif
		#ifdef __UNIX__
				rux::XString utf8;
				utf8.set_ByRef( directory_name.ConvertToUTF8() );
				is_exists = access( utf8.str() , F_OK ) != -1 ? 1 : 0;
				if( is_exists == 1 )
				{
					struct stat st;
					if( stat( utf8.str() , &st ) == 0 )
					{
						if( S_ISDIR( st.st_mode ) == 0 )
							is_exists = 0;
					}
				}
		#endif
			}
			return is_exists;
		};
		rux::uint8 rux::io::XDirectory::Exists( const rux::String& directory_name )
		{
			directory_name.AddRef();
			rux::uint8 is_exists = 0;
			if( directory_name.Length() > 0 )
			{
		#ifdef __WINDOWS__
				if( directory_name._code_page == XEnumCodePage_UTF16 )
				{
					is_exists = _waccess( (wchar_t*)directory_name.str() , 0 ) != -1 ? 1 : 0;
					if( is_exists == 1 )
					{
						struct _stat st;
						if( _wstat( (wchar_t*)directory_name.str() , &st ) == 0 )
						{
							if( ( st.st_mode & _S_IFDIR ) == 0 )
								is_exists = 0;
						}
					}
				}
				else if( directory_name._code_page == XEnumCodePage_UTF8 )
				{
					rux::XString utf16 = directory_name;
					utf16.set_ByRef( utf16.ConvertToUTF16() );
					is_exists = _waccess( (wchar_t*)utf16.str() , 0 ) != -1 ? 1 : 0;
					if( is_exists == 1 )
					{
						struct _stat st;
						if( _wstat( (wchar_t*)utf16.str() , &st ) == 0 )
						{
							if( ( st.st_mode & _S_IFDIR ) == 0 )
								is_exists = 0;
						}
					}
				}
				else if( directory_name._code_page == XEnumCodePage_CP1251 )
				{
					is_exists = _access( directory_name.str() , 0 ) != -1 ? 1 : 0;
					if( is_exists == 1 )
					{
						struct _stat st;
						if( _stat( directory_name.str() , &st ) == 0 )
						{
							if( ( st.st_mode & _S_IFDIR ) == 0 )
								is_exists = 0;
						}
					}
				}
		#endif
		#ifdef __UNIX__
				rux::XString utf8( directory_name );
				utf8.set_ByRef( utf8.ConvertToUTF8() );
				is_exists = access( utf8.str() , F_OK ) != -1 ? 1 : 0;
				if( is_exists == 1 )
				{
					struct stat st;
					if( stat( utf8.str() , &st ) == 0 )
					{
						if( S_ISDIR( st.st_mode ) == 0 )
							is_exists = 0;
					}
				}
		#endif
			}
			directory_name.Release();
			return is_exists;
		};
		void rux::io::XDirectory::Delete( const rux::XString& directory_name , XSearchOption search_option )
		{
			rux::int32 error = 0;
			if( search_option == XAllDirectories )
			{
				if( directory_name.Length() > 0 ) 
				{
		#ifdef __WINDOWS__
					if( directory_name()->_code_page == XEnumCodePage_UTF16 )
					{
						if( _waccess( (wchar_t*)directory_name.str() , 0 ) != -1 )
						{
							struct _stat st;
							if( _wstat( (wchar_t*)directory_name.str() , &st ) == 0 )
							{
								if( ( st.st_mode & _S_IFDIR ) )
								{
									rux::XArray<XFile> files;
									GetFiles( directory_name , XAllDirectories , files );
									while( files.Count() > 0 )
									{
										files[ files.Count() - 1 ].Delete();
										files.RemoveAt( files.Count() - 1 );
									}
									rux::XArray<rux::io::XDirectory> directories;
									GetDirectories( directory_name , XAllDirectories , directories );
									while( directories.Count() > 0 )
									{
										directories[ directories.Count() - 1 ].Delete();
										directories.RemoveAt( directories.Count() - 1 );
									}
									if( _wrmdir( (wchar_t*)directory_name.str() ) != 0 )
										error = errno;
								}
							}	
						}
					}
					else if( directory_name()->_code_page == XEnumCodePage_CP1251 )

					{
						if( _access( directory_name.str() , 0 ) != -1 )
						{
							struct _stat st;
							if( _stat( directory_name.str() , &st ) == 0 )
							{
								if( ( st.st_mode & _S_IFDIR ) )
								{
									rux::XArray<XFile> files;
									GetFiles( directory_name , XAllDirectories , files );
									while( files.Count() > 0 )
									{
										files[ files.Count() - 1 ].Delete();
										files.RemoveAt( files.Count() - 1 );
									}
									rux::XArray<rux::io::XDirectory> directories;
									GetDirectories( directory_name , XAllDirectories , directories );
									while( directories.Count() > 0 )
									{
										directories[ directories.Count() - 1 ].Delete();
										directories.RemoveAt( directories.Count() - 1 );
									}
									if( _rmdir( directory_name.str() ) != 0 )
										error = errno;
								}
							}
						}
					}
					else
					{
						rux::XString utf16_directory_name;
						utf16_directory_name.set_ByRef( directory_name.ConvertToUTF16() );
						if( _waccess( (wchar_t*)utf16_directory_name.str() , 0 ) != -1 )
						{
							struct _stat st;
							if( _wstat( (wchar_t*)utf16_directory_name.str() , &st ) == 0 )
							{
								if( ( st.st_mode & _S_IFDIR ) )
								{
									rux::XArray<XFile> files;
									GetFiles( directory_name , XAllDirectories , files );
									while( files.Count() > 0 )
									{
										files[ files.Count() - 1 ].Delete();
										files.RemoveAt( files.Count() - 1 );
									}
									rux::XArray<rux::io::XDirectory> directories;
									GetDirectories( directory_name , XAllDirectories , directories );
									while( directories.Count() > 0 )
									{
										directories[ directories.Count() - 1 ].Delete();
										directories.RemoveAt( directories.Count() - 1 );
									}
									if( _wrmdir( (wchar_t*)utf16_directory_name.str() ) != 0 )
										error = errno;
								}
							}		
						}
					}
		#endif
		#ifdef __UNIX__
					rux::XString utf8;
					utf8.set_ByRef( directory_name.ConvertToUTF8() );
					if( access( utf8.str() , F_OK ) != -1 )
					{
						struct stat st;
						if( stat( utf8.str() , &st ) == 0 )
						{
							if( S_ISDIR( st.st_mode ) )
							{
								rux::XArray<XFile> files;
								GetFiles( utf8 , XAllDirectories , files );
								while( files.Count() > 0 )
								{
									files[ files.Count() - 1 ].Delete();
									files.RemoveAt( files.Count() - 1 );
								}
								rux::XArray<rux::io::XDirectory> directories;
								GetDirectories( utf8 , XAllDirectories , directories );
								while( directories.Count() > 0 )
								{
									directories[ directories.Count() - 1 ].Delete();
									directories.RemoveAt( directories.Count() - 1 );
								}
								if( rmdir( utf8.str() ) != 0 )
									error = errno;
							}
						}
					}
		#endif
				}
			}
			else if( search_option == XTopDirectoryOnly )
			{
				if( directory_name.Length() > 0 ) 
				{
		#ifdef __WINDOWS__
					if( directory_name()->_code_page == XEnumCodePage_UTF16 )
					{
						if( _waccess( (wchar_t*)directory_name.str() , 0 ) != -1 )
						{
							struct _stat st;
							if( _wstat( (wchar_t*)directory_name.str() , &st ) == 0 )
							{
								if( ( st.st_mode & _S_IFDIR ) )
								{
									if( _wrmdir( (wchar_t*)directory_name.str() ) != 0 )
										error = errno;
								}
							}
						}
					}
					else if( directory_name()->_code_page == XEnumCodePage_CP1251 )
					{
						if( _access( directory_name.str() , 0 ) != -1 )
						{	
							struct _stat st;
							if( _stat( directory_name.str() , &st ) == 0 )
							{
								if( ( st.st_mode & _S_IFDIR ) )
								{
									if( _rmdir( directory_name.str() ) != 0 )
										error = errno;
								}
							}	
						}
					}
					else
					{
						rux::XString utf16_directory_name;
						utf16_directory_name.set_ByRef( directory_name.ConvertToUTF16() );
						if( _waccess( (wchar_t*)utf16_directory_name.str() , 0 ) != -1 )
						{	
							struct _stat st;
							if( _wstat( (wchar_t*)utf16_directory_name.str() , &st ) == 0 )
							{
								if( ( st.st_mode & _S_IFDIR ) )
								{
									if( _wrmdir( (wchar_t*)utf16_directory_name.str() ) != 0 )
										error = errno;
								}
							}		
						}
					}
		#endif
		#ifdef __UNIX__
					rux::XString utf8;
					utf8.set_ByRef( directory_name.ConvertToUTF8() );
					if( access( utf8.str() , F_OK ) != -1 )
					{
						struct stat st;
						if( stat( utf8.str() , &st ) == 0 )
						{
							if( S_ISDIR( st.st_mode ) )
							{
								if( rmdir( utf8.str() ) != 0 )
									error = errno;
							}
						}
					}
		#endif
				}
			}
		};
		rux::uint8 rux::io::Directory::Delete( void )
		{
			rux::int32 error = 0;
			if( _directory_name.Length() > 0 ) 
			{
		#ifdef __WINDOWS__
				if( _directory_name()->_code_page == XEnumCodePage_UTF16 )
				{
					if( _waccess( (wchar_t*)_directory_name.str() , 0 ) != -1 )
					{
						struct _stat st;
						if( _wstat( (wchar_t*)_directory_name.str() , &st ) == 0 )
						{
							if( ( st.st_mode & _S_IFDIR ) )
							{
								if( _wrmdir( (wchar_t*)_directory_name.str() ) != 0 )
									error = errno;
							}
						}
					}
				}
				else if( _directory_name()->_code_page == XEnumCodePage_CP1251 )
				{
					if( _access( _directory_name.str() , 0 ) != -1 )
					{
						struct _stat st;
						if( _stat( _directory_name.str() , &st ) == 0 )
						{
							if( ( st.st_mode & _S_IFDIR ) )
							{
								if( _rmdir( _directory_name.str() ) != 0 )
									error = errno;
							}
						}
					}
				}
				else
				{
					rux::XString utf16_directory_name;
					utf16_directory_name.set_ByRef( _directory_name.ConvertToUTF16() );
					if( _waccess( (wchar_t*)utf16_directory_name.str() , 0 ) != -1 )
					{
						struct _stat st;
						if( _wstat( (wchar_t*)utf16_directory_name.str() , &st ) == 0 )
						{
							if( ( st.st_mode & _S_IFDIR ) )
							{
								if( _wrmdir( (wchar_t*)utf16_directory_name.str() ) != 0 )
									error = errno;
							}
						}
					}
				}
		#endif
		#ifdef __UNIX__
				rux::XString utf8;
				utf8.set_ByRef( _directory_name.ConvertToUTF8() );
				if( access( utf8.str() , F_OK ) != -1 )
				{
					struct stat st;
					if( stat( utf8.str() , &st ) == 0 )
					{
						if( S_ISDIR( st.st_mode ) )
						{
							if( rmdir( utf8.str() ) != 0 )
								error = errno;
						}
					}
				}
		#endif
			}
			return error == 0 ? 1 : 0;
		};
		void rux::io::XDirectory::Delete( const rux::XString& directory_name )
		{
			rux::int32 error = 0;
			if( directory_name.Length() > 0 ) 
			{
		#ifdef __WINDOWS__
				if( directory_name()->_code_page == XEnumCodePage_UTF16 )
				{
					if( _waccess( (wchar_t*)directory_name.str() , 0 ) != -1 )
					{
						struct _stat st;
						if( _wstat( (wchar_t*)directory_name.str() , &st ) == 0 )
						{
							if( ( st.st_mode & _S_IFDIR ) )
							{
								if( _wrmdir( (wchar_t*)directory_name.str() ) != 0 )
									error = errno;
							}
						}
					}
				}
				else if( directory_name()->_code_page == XEnumCodePage_CP1251 )
				{
					if( _access( directory_name.str() , 0 ) != -1 )
					{
						struct _stat st;
						if( _stat( directory_name.str() , &st ) == 0 )
						{
							if( ( st.st_mode & _S_IFDIR ) )
							{
								if( _rmdir( directory_name.str() ) != 0 )
									error = errno;
							}
						}
					}
				}
				else
				{
					rux::XString utf16_directory_name;
					utf16_directory_name.set_ByRef( directory_name.ConvertToUTF16() );
					if( _waccess( (wchar_t*)utf16_directory_name.str() , 0 ) != -1 )
					{
						struct _stat st;
						if( _wstat( (wchar_t*)utf16_directory_name.str() , &st ) == 0 )
						{
							if( ( st.st_mode & _S_IFDIR ) )
							{
								if( _wrmdir( (wchar_t*)utf16_directory_name.str() ) != 0 )
									error = errno;
							}
						}
					}
				}
		#endif
		#ifdef __UNIX__
				rux::XString utf8;
				utf8.set_ByRef( directory_name.ConvertToUTF8() );
				if( access( utf8.str() , F_OK ) != -1 )
				{
					struct stat st;
					if( stat( utf8.str() , &st ) == 0 )
					{
						if( S_ISDIR( st.st_mode ) )
						{
							if( rmdir( utf8.str() ) != 0 )
								error = errno;
						}
					}
				}
		#endif
			}
		};
		void rux::io::XDirectory::Delete( const char* directory_name_ptr )
		{
			rux::int32 error = 0;
			if( directory_name_ptr && XString::LengthA( directory_name_ptr ) > 0 ) 
			{
		#ifdef __WINDOWS__
				if( _access( directory_name_ptr , 0 ) != -1 )
				{
					struct _stat st;
					if( _stat( directory_name_ptr , &st ) == 0 )
					{
						if( ( st.st_mode & _S_IFDIR ) )
						{
							if( _rmdir( directory_name_ptr ) != 0 )
								error = errno;
						}
					}
				}
		#endif
		#ifdef __UNIX__
				rux::XString utf8( directory_name_ptr );
				utf8.set_ByRef( utf8.ConvertToUTF8() );
				if( access( utf8.str() , F_OK ) != -1 )
				{			
					struct stat st;
					if( stat( utf8.str() , &st ) == 0 )
					{
						if( S_ISDIR( st.st_mode ) )
						{
							if( rmdir( utf8.str() ) != 0 )
								error = errno;
						}
					}
				}
		#endif		
			}
		};
		void rux::io::XDirectory::Create( const char* directory_name_ptr )
		{
			rux::int32 error = 0;
			if( directory_name_ptr && XString::LengthA( directory_name_ptr ) > 0 ) 
			{
		#ifdef __WINDOWS__
				if( _access( directory_name_ptr , 0 ) == -1 )
				{
					if( _mkdir( directory_name_ptr ) != 0 )
							error = errno;
				}
				else
				{
					struct _stat st;
					if( _stat( directory_name_ptr , &st ) == 0 )
					{
						if( ( st.st_mode & _S_IFDIR ) == 0 )
						{
							if( _mkdir( directory_name_ptr ) != 0 )
								error = errno;
						}
					}
				}
		#endif
		#ifdef __UNIX__
				rux::XString utf8( directory_name_ptr );
				utf8.set_ByRef( utf8.ConvertToUTF8() );
				if( access( utf8.str() , F_OK ) == -1 )
				{
					if( mkdir( utf8.str() , S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH ) != 0 )
							error = errno;
				}
				else
				{
					struct stat st;
					if( stat( utf8.str() , &st ) == 0 )
					{
						if( S_ISDIR( st.st_mode ) == 0 )
						{
							if( mkdir( utf8.str() , S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH ) != 0 )
								error = errno;
						}
					}
				}
		#endif
			}
			if( error != 0 )
				printf( "rux::io::XDirectory::Create , errno=%d\n" , error );
		};
		void rux::io::XDirectory::Create( const rux::XString& directory_name , rux::XString& error_string )
		{
			error_string.Clear();
			rux::int32 error = 0;
			if( directory_name.Length() > 0 ) 
			{
		#ifdef __WINDOWS__
				if( directory_name()->_code_page == XEnumCodePage_UTF16 )
				{
					if( _waccess( (wchar_t*)directory_name.str() , 0 ) == -1 )
					{
						if( _wmkdir( (wchar_t*)directory_name.str() ) != 0 )
							error = errno;
					}
					else
					{
						struct _stat st;
						if( _wstat( (wchar_t*)directory_name.str() , &st ) == 0 )
						{
							if( ( st.st_mode & _S_IFDIR ) == 0 )
							{
								if( _wmkdir( (wchar_t*)directory_name.str() ) != 0 )
									error = errno;
							}
						}
					}
				}
				else if( directory_name()->_code_page == XEnumCodePage_UTF8 )
				{			
					rux::XString utf16;
					utf16.set_ByRef( directory_name.ConvertToUTF16() );
					if( _waccess( (wchar_t*)utf16.str() , 0 ) == -1 )
					{
						if( _wmkdir( (wchar_t*)utf16.str() ) != 0 )
							error = errno;
					}
					else
					{
						struct _stat st;
						if( _wstat( (wchar_t*)utf16.str() , &st ) == 0 )
						{
							if( ( st.st_mode & _S_IFDIR ) == 0 )
							{
								if( _wmkdir( (wchar_t*)utf16.str() ) != 0 )
									error = errno;
							}
						}
					}
				}
				else if( directory_name()->_code_page == XEnumCodePage_CP1251 )
				{
					if( _access( directory_name.str() , 0 ) == -1 )
					{
						if( _mkdir( directory_name.str() ) != 0 )
							error = errno;
					}
					else
					{
						struct _stat st;
						if( _stat( directory_name.str() , &st ) == 0 )
						{
							if( ( st.st_mode & _S_IFDIR ) == 0 )
							{
								if( _mkdir( directory_name.str() ) != 0 )
									error = errno;
							}
						}
					}
				}		
		#endif
		#ifdef __UNIX__
				rux::XString utf8;
				utf8.set_ByRef( directory_name.ConvertToUTF8() );		
				if( access( utf8.str() , F_OK ) == -1 )
				{
					if( mkdir( utf8.str() , S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH ) != 0 )
						error = errno;
				}
				else
				{
					struct stat st;
					if( stat( utf8.str() , &st ) == 0 )
					{
						if( S_ISDIR( st.st_mode ) == 0 )
						{
							if( mkdir( utf8.str() , S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH ) != 0 )
								error = errno;
						}
					}
				}
		#endif
			}
			if( error != 0 )
				error_string = XError::ConvertErrorCodeToString( error , XEnum_ErrorType_Errno );
		};
		void rux::io::XDirectory::Create( const rux::String& directory_name )
		{
			directory_name.AddRef();
			rux::int32 error = 0;
			if( directory_name.Length() > 0 ) 
			{
		#ifdef __WINDOWS__
				if( directory_name._code_page == XEnumCodePage_UTF16 )
				{
					if( _waccess( (wchar_t*)directory_name.str() , 0 ) == -1 )
					{
						if( _wmkdir( (wchar_t*)directory_name.str() ) != 0 )
							error = errno;
					}
					else
					{
						struct _stat st;
						if( _wstat( (wchar_t*)directory_name.str() , &st ) == 0 )
						{
							if( ( st.st_mode & _S_IFDIR ) == 0 )
							{
								if( _wmkdir( (wchar_t*)directory_name.str() ) != 0 )
									error = errno;
							}
						}
					}
				}
				else if( directory_name._code_page == XEnumCodePage_UTF8 )
				{
					rux::XString utf16 = directory_name;
					utf16.set_ByRef( utf16.ConvertToUTF16() );
					if( _waccess( (wchar_t*)utf16.str() , 0 ) == -1 )
					{
						if( _wmkdir( (wchar_t*)utf16.str() ) != 0 )
							error = errno;
					}
					else
					{
						struct _stat st;
						if( _wstat( (wchar_t*)utf16.str() , &st ) == 0 )
						{
							if( ( st.st_mode & _S_IFDIR ) == 0 )
							{
								if( _wmkdir( (wchar_t*)utf16.str() ) != 0 )
									error = errno;
							}
						}
					}
				}
				else if( directory_name._code_page == XEnumCodePage_CP1251 )
				{
					if( _access( directory_name.str() , 0 ) == -1 )
					{
						if( _mkdir( directory_name.str() ) != 0 )
							error = errno;
					}
					else
					{
						struct _stat st;
						if( _stat( directory_name.str() , &st ) == 0 )
						{
							if( ( st.st_mode & _S_IFDIR ) == 0 )
							{
								if( _mkdir( directory_name.str() ) != 0 )
									error = errno;
							}
						}
					}
				}		
		#endif
		#ifdef __UNIX__
				rux::XString utf8( directory_name );
				utf8.set_ByRef( utf8.ConvertToUTF8() );
				if( access( utf8.str() , F_OK ) == -1 )
				{
					if( mkdir( utf8.str() , S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH ) != 0 )
						error = errno;
				}
				else
				{
					struct stat st;
					if( stat( utf8.str() , &st ) == 0 )
					{
						if( S_ISDIR( st.st_mode ) == 0 )
						{
							if( mkdir( utf8.str() , S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH ) != 0 )
								error = errno;
						}
					}
				}
		#endif
			}
			directory_name.Release();
			if( error != 0 )
				printf( "rux::io::XDirectory::Create , errno=%d\n" , error );
		};
		void rux::io::XDirectory::CreateAll( const rux::XString& directory_name , rux::XString& error )
		{
			error.Clear();
			rux::XString directory;
			directory.set_ByRef( directory_name.ReplaceAll( '\\' , '/' ) );
			directory.set_ByRef( directory.ConvertToAnsi() );
			rux::XArray< XString > splits;
			splits.set_ByRef(	directory.Split( "/" ) );
			if( splits.Count() > 0 && directory_name.Length() > 0 && directory_name.get_UTF8Char( 0 ) == '/' )
				splits.set_ItemByRef( 0 , "/" + splits[ 0 ] );
			if( splits.Count() > 0 )
			{														
				if( rux::io::XDirectory::Exists( splits[ 0 ] + "/" ) == 1 )
				{
					rux::uint32 index = 1;
					while( splits.Count() > index )
					{
						splits.set_ItemByRef( 0 , splits[ 0 ] + "/" + splits[ index ] );	
						if( rux::io::XDirectory::Exists( splits[ 0 ] ) == 0 )
							rux::io::XDirectory::Create( splits[ 0 ] , error );
						if( error.Length() > 0 )
							break;
						index++;
					}
				}
			}			
		};
		void rux::io::XDirectory::AddAllowAddSubdirectoryAccess( const rux::XString& directory_name , const rux::XString& user_or_group , rux::XString& error )
		{
			error.Clear();
			if( rux::io::XDirectory::Exists( directory_name ) == 1 )
			{
		#ifdef __WINDOWS__
				WELL_KNOWN_SID_TYPE well_known_sid_type = WinNullSid;
				rux::XString lower_user_or_group;
				lower_user_or_group.set_ByRef( user_or_group.ToLower() );
				if( lower_user_or_group == "network service" )
					well_known_sid_type = WinNetworkServiceSid;
				else if( lower_user_or_group.Equals( "everyone" , 8 ) )
					well_known_sid_type = WinWorldSid;	
				unsigned long sid_size = SECURITY_MAX_SID_SIZE;
				PSID sid_ptr = LocalAlloc( LMEM_FIXED , sid_size );
				if( sid_ptr )
				{
					if( CreateWellKnownSid( well_known_sid_type , NULL , sid_ptr , &sid_size ) )
					{
						rux::uint32 num_of_aces = 1;
						rux::uint32 acl_size = sizeof( ACL ) + ( (sizeof( ACCESS_ALLOWED_ACE ) - sizeof( DWORD ) ) * num_of_aces );
						for ( rux::uint32 index0 = 0 ; index0 < num_of_aces ; index0++ )
						{
							acl_size += GetLengthSid( sid_ptr );
						}
						PACL acl_ptr = (PACL)alloc_array_macros( char , acl_size );
						if( InitializeAcl( acl_ptr , acl_size , ACL_REVISION ) )
						{
							if( AddAccessAllowedAce( acl_ptr , ACL_REVISION , FILE_ADD_SUBDIRECTORY , sid_ptr ) )
							{
								rux::XString utf16_directory_name;
								utf16_directory_name.set_ByRef( directory_name.ConvertToUTF16() );
								rux::uint32 error_uint32 = SetNamedSecurityInfoW( (wchar_t*)utf16_directory_name.str() , SE_FILE_OBJECT , DACL_SECURITY_INFORMATION , sid_ptr , NULL ,  acl_ptr , NULL );
								if( error_uint32 != ERROR_SUCCESS )
								{
									rux_write_error( "SetNamedSecurityInfoW , error " + XUInt32( error_uint32 ).ToString() , error );
								}
							}
							else
							{
								rux_write_error( XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError ) , error );
							}
						}
						else
						{
							rux_write_error( XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError ) , error );
						}
					}
					else
					{
						rux_write_error( XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError ) , error );
					}
					LocalFree( sid_ptr );
				}
		#endif
			}
			else
			{
				rux_write_error( "directory '" + directory_name + "' does not exist" , error );
			}
		};
		void rux::io::XDirectory::AddAllowReadAccess( const rux::XString& directory_name , const rux::XString& user_or_group , rux::XString& error )
		{
			error.Clear();
			if( rux::io::XDirectory::Exists( directory_name ) == 1 )
			{
		#ifdef __WINDOWS__
				WELL_KNOWN_SID_TYPE well_known_sid_type = WinNullSid;
				rux::XString lower_user_or_group;
				lower_user_or_group.set_ByRef( user_or_group.ToLower() );
				if( lower_user_or_group == "network service" )
					well_known_sid_type = WinNetworkServiceSid;	
				else if( lower_user_or_group.Equals( "everyone" , 8 ) )
					well_known_sid_type = WinWorldSid;	
				unsigned long sid_size = SECURITY_MAX_SID_SIZE;
				PSID sid_ptr = LocalAlloc( LMEM_FIXED , sid_size );
				if( sid_ptr )
				{
					if( CreateWellKnownSid( well_known_sid_type , NULL , sid_ptr , &sid_size ) )
					{
						rux::uint32 num_of_aces = 1;
						rux::uint32 acl_size = sizeof( ACL ) + ( (sizeof( ACCESS_ALLOWED_ACE ) - sizeof( DWORD ) ) * num_of_aces );
						for ( rux::uint32 index0 = 0 ; index0 < num_of_aces ; index0++ )
						{
							acl_size += GetLengthSid( sid_ptr );
						}
						PACL acl_ptr = (PACL)alloc_array_macros( char , acl_size );
						if( InitializeAcl( acl_ptr , acl_size , ACL_REVISION ) )
						{
							if( AddAccessAllowedAce( acl_ptr , ACL_REVISION , FILE_GENERIC_READ , sid_ptr ) )
							{
								rux::XString utf16_directory_name;
								utf16_directory_name.set_ByRef( directory_name.ConvertToUTF16() );
								rux::uint32 error_uint32 = SetNamedSecurityInfoW( (wchar_t*)utf16_directory_name.str() , SE_FILE_OBJECT , DACL_SECURITY_INFORMATION , sid_ptr , NULL ,  acl_ptr , NULL );
								if( error_uint32 != ERROR_SUCCESS )
								{
									rux_write_error( "SetNamedSecurityInfoW , error " + XUInt32( error_uint32 ).ToString() , error );
								}
							}
							else
							{
								rux_write_error( XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError ) , error );
							}
						}
						else
						{
							rux_write_error( XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError ) , error );
						}
					}
					else
					{
						rux_write_error( XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError ) , error );
					}
					LocalFree( sid_ptr );
				}
		#endif
			}
			else
			{
				rux_write_error( "directory '" + directory_name + "' does not exist" , error );
			}
		};
		void rux::io::XDirectory::AddAllowAllAccess( const rux::XString& directory_name , const rux::XString& user_or_group , rux::XString& error )
		{
			error.Clear();
			if( rux::io::XDirectory::Exists( directory_name ) == 1 )
			{
		#ifdef __WINDOWS__
				WELL_KNOWN_SID_TYPE well_known_sid_type = WinNullSid;
				rux::XString lower_user_or_group;
				lower_user_or_group.set_ByRef( user_or_group.ToLower() );
				if( lower_user_or_group == "network service" )
					well_known_sid_type = WinNetworkServiceSid;	
				else if( lower_user_or_group.Equals( "everyone" , 8 ) )
					well_known_sid_type = WinWorldSid;	
				unsigned long sid_size = SECURITY_MAX_SID_SIZE;
				PSID sid_ptr = LocalAlloc( LMEM_FIXED , sid_size );
				if( sid_ptr )
				{
					if( CreateWellKnownSid( well_known_sid_type , NULL , sid_ptr , &sid_size ) )
					{
						rux::uint32 num_of_aces = 1;
						rux::uint32 acl_size = sizeof( ACL ) + ( (sizeof( ACCESS_ALLOWED_ACE ) - sizeof( DWORD ) ) * num_of_aces );
						for ( rux::uint32 index0 = 0 ; index0 < num_of_aces ; index0++ )
						{
							acl_size += GetLengthSid( sid_ptr );
						}
						PACL acl_ptr = (PACL)alloc_array_macros( char , acl_size );
						if( InitializeAcl( acl_ptr , acl_size , ACL_REVISION ) )
						{
							if( AddAccessAllowedAce( acl_ptr , ACL_REVISION , FILE_ALL_ACCESS , sid_ptr ) )
							{
								rux::XString utf16_directory_name;
								utf16_directory_name.set_ByRef( directory_name.ConvertToUTF16() );
								rux::uint32 error_uint32 = SetNamedSecurityInfoW( (wchar_t*)utf16_directory_name.str() , SE_FILE_OBJECT , DACL_SECURITY_INFORMATION , sid_ptr , NULL ,  acl_ptr , NULL );
								if( error_uint32 != ERROR_SUCCESS )
								{
									rux_write_error( "SetNamedSecurityInfoW , error " + XUInt32( error_uint32 ).ToString() , error );
								}
							}
							else
							{
								rux_write_error( XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError ) , error );
							}
						}
						else
						{
							rux_write_error( XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError ) , error );
						}
					}
					else
					{
						rux_write_error( XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError ) , error );
					}
					LocalFree( sid_ptr );
				}
		#endif
			}
			else
			{
				rux_write_error( "directory '" + directory_name + "' does not exist" , error );
			}
		};
		void rux::io::XDirectory::set_Watcher( rux_on_directory_changed on_directory_changed )
		{
			CRITICAL_SECTION_LOCK( (*this)()->_cs_watcher_thread );
			if( (*this)()->_watcher_thread.get_IsStarted() == 1 )
				(*this)()->_watcher_thread.Stop();
			(*this)()->_watcher_thread.Start();
			(*this)()->_on_directory_changed = on_directory_changed;
			(*this)()->_watcher_thread.set_ThreadName( "directory watcher" );
			(*this)()->_watcher_thread.set_ThreadMode( XThreadMode_Repeat );
			(*this)()->_watcher_thread.set_Timeout( 1000 );
			(*this)()->_watcher_thread.Push( watcher_thread , (*this)() );
			(*this)()->_cs_watcher_thread.UnLock();    
		};
		void rux::io::XDirectory::watcher_thread( void* param , size_t ___rux__thread_index1986 )
		{
			UNREFERENCED_PARAMETER( ___rux__thread_index1986 );
			Directory* directory = (Directory*)param;
			rux::threading::XThread current_thread( rux::threading::XThread::get_CurrentThread() );
			rux::XString directory_name;
			directory_name.set_ByRef( directory->_directory_name.ConvertToAnsi() );
			directory_name.set_ByRef( directory_name.ReplaceAll( '\\' , '/' ) );
			if( directory_name.Length() > 0 
				&& directory_name.get_UTF8Char( directory_name.Length() - 1 ) == '/' )
				directory_name.RemoveAt( directory_name.Length() - 1 );
		#ifdef __WINDOWS__
			HANDLE directory_handle = CreateFileA( directory_name.str() ,
				GENERIC_READ ,
				FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE ,
				NULL ,
				OPEN_EXISTING ,
				FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED ,
				NULL );
			if( directory_handle != INVALID_HANDLE_VALUE )
			{
			OVERLAPPED overlapped;
				memset( &overlapped , 0 , sizeof( overlapped ) );
				overlapped.hEvent = CreateEvent( 0 , FALSE , FALSE , 0 );
			rux::uint32 buffer_length = 60 * 1024;
				DWORD returned_bytes = 0 , seek = 0;
				rux::uint8* buffer = alloc_array_macros( rux::uint8 , buffer_length );    
			while( current_thread.get_IsWaitForExit() == 0 )
			{ 
				returned_bytes = 0;
				ReadDirectoryChangesW( directory_handle , buffer , buffer_length , false ,
							FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME ,
							&returned_bytes , &overlapped , 0 );
				switch( WaitForSingleObject( overlapped.hEvent , 1000 ) ) 
				{ 
				case WAIT_OBJECT_0: 
					{
						seek = 0;
						while( seek < buffer_length )
						{
							PFILE_NOTIFY_INFORMATION notify = PFILE_NOTIFY_INFORMATION( buffer + seek );
							seek += notify->NextEntryOffset;
												if( directory->_cs_watcher_thread.TryLock() )
												{
													if( directory->_on_directory_changed )
													{
														rux::XString changed_filename( notify->FileName , 0 , notify->FileNameLength / 2 );
														changed_filename.set_ByRef( changed_filename.ReplaceAll( '\\' , '/' ) );
														changed_filename.set_ByRef( changed_filename.ConvertToUTF8() );
														changed_filename.set_ByRef( directory_name.ConvertToUTF8() + "/" + changed_filename );
														if( notify->Action == FILE_ACTION_MODIFIED )
															directory->_on_directory_changed( changed_filename , XEnum_DirectoryChanged_WriteFile );
													}
													directory->_cs_watcher_thread.UnLock();
												}
												if( notify->NextEntryOffset == 0 )
													break;
						}
										break;
					}
				}
			}
			CloseHandle( overlapped.hEvent );
			rux::engine::free_mem( buffer );
				CloseHandle( directory_handle );
			}
		#endif
		};
		void rux::io::XDirectory::Rename( const rux::XString& old_dir , const rux::XString& new_dir , rux::XString& error )
		{
			error.Clear();	
			rux::XString current_cp_new_dir;
			current_cp_new_dir.set_ByRef( new_dir.ConvertToAnsi() );
			current_cp_new_dir.set_ByRef( XPath::ExcludeRelative( current_cp_new_dir ) );
			rux::XString current_cp_old_dir;
			current_cp_old_dir.set_ByRef( old_dir.ConvertToAnsi() );
			current_cp_old_dir.set_ByRef( XPath::ExcludeRelative( current_cp_old_dir ) );
			if( rename( current_cp_old_dir.str() , current_cp_new_dir.str() ) != 0 )
				error = XError::ConvertErrorCodeToString( errno , XEnum_ErrorType_Errno );
		};
	};
};
