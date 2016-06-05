#ifndef XAN_DIRECTORY_H
#define XAN_DIRECTORY_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_file.h>
#include <xan_thread.h>
enum XEnum_DirectoryChanged
{
	XEnum_DirectoryChanged_WriteFile
};
typedef void (*rux_on_directory_changed)( const rux::XString& filename , XEnum_DirectoryChanged directory_changed_type );
enum XSearchOption
{
	XAllDirectories,
	XTopDirectoryOnly
};
namespace rux
{	
	namespace io
	{	
		begin_declare_rux_class_with_namespace( Directory , rux::io );
			~Directory();
			rux::uint8 Delete( void );
			rux::XString& get_DirectoryName( void );
			rux::Array<XFile>& GetFiles( void );
			rux::Array<rux::io::XDirectory>& GetDirectories( void );
		begin_declare_rux_class_members( Directory );
			rux::XString _directory_name;
			rux::threading::XThread _watcher_thread;
			XCrtSect _cs_watcher_thread;
			rux_on_directory_changed _on_directory_changed;
		end_declare_rux_class_members( Directory );
			XDirectory( const rux::XString& directory_name );
			rux::Array<XFile>& GetFiles( void );
			rux::Array<rux::io::XDirectory>& GetDirectories( void );
			rux::XString& get_DirectoryName( void );
			rux::uint8 Delete( void );
			void set_Watcher( rux_on_directory_changed on_directory_changed );
			static rux::String& get_SpecialFolderPath( const char* type_ptr );
			static rux::String& get_SpecialFolderPath( const char* type_ptr , rux::XString& error);
			static rux::Array<XFile>& GetFiles( const char* directory_name_ptr ); 	
			static rux::Array<XFile>& GetFiles( const char* directory_name_ptr , XSearchOption search_option );
			static rux::Array<XFile>& GetFiles( const rux::XString& directory_name , XSearchOption search_option );
			static rux::Array<rux::io::XDirectory>& GetDirectories( const char* directory_name_ptr ); 
			static rux::Array<rux::io::XDirectory>& GetDirectories( const char* directory_name_ptr , XSearchOption search_option ); 
			static rux::Array<rux::io::XDirectory>& GetDirectories( const rux::XString& directory_name , XSearchOption search_option ); 
			static void GetDirectories( const char* directory_name_ptr , rux::XArray<rux::io::XDirectory>& directories ); 
			static void GetDirectories( const char* directory_name_ptr , XSearchOption search_option , rux::XArray<rux::io::XDirectory>& directories ); 
			static void GetDirectories( const rux::XString& directory_name , rux::XArray<rux::io::XDirectory>& directories ); 
			static void GetDirectories( const rux::XString& directory_name , XSearchOption search_option , rux::XArray<rux::io::XDirectory>& directories ); 
			static void GetFiles( const rux::XString& directory_name , XSearchOption search_option , rux::XArray<XFile>& files );
			static void GetFiles( const char* directory_name_ptr , rux::XArray<XFile>& files ); 	
			static void GetFiles( const char* directory_name_ptr , XSearchOption search_option , rux::XArray<XFile>& files );
			static void Copy( const char* source_directory_name_ptr , const char* destination_directory_name_ptr , rux::XString& error_string );
			static void Copy( const rux::XString& source_directory_name_ptr , const rux::XString& destination_directory_name_ptr , rux::XString& error_string);
			static void Create( const char* directory_name_ptr );
			static void Create( const rux::XString& directory_name , rux::XString& error );
			static void Create( const rux::String& directory_name );
			static void CreateAll( const rux::XString& directory_name , rux::XString& error );
			static rux::uint8 Exists( const char* directory_name_ptr );
			static rux::uint8 Exists( const rux::XString& directory_name );
			static rux::uint8 Exists( const rux::String& directory_name );
			static void Delete( const char* directory_name_ptr );
			static void Delete( const rux::XString& directory_name );
			static void Delete( const rux::XString& directory_name , XSearchOption search_option );
			static rux::uint8 CreateLink( const rux::XString& shortcut_path , const rux::XString& path_to_link, const rux::XString& description , const rux::XString& arguments , const rux::XString& working_directory );
			static void AddAllowAllAccess( const rux::XString& directory_name , const rux::XString& user_or_group , rux::XString& error );
			static void AddAllowReadAccess( const rux::XString& directory_name , const rux::XString& user_or_group , rux::XString& error );
			static void AddAllowAddSubdirectoryAccess( const rux::XString& directory_name , const rux::XString& user_or_group , rux::XString& error );
			static void CreateDirectoriesStructureFromJSON( const rux::XString& root_directory , const rux::XString& json , rux::XString& error );
			static void DeleteDirectoriesStructureFromJSON( const rux::XString& root_directory , const rux::XString& json , rux::XString& error );
			static rux::String& TransformToRuxDirectoryName( const rux::XString& directory_name );
			static rux::String& get_JSONDirectoriesStructure( const rux::XString& directory_name );
			static void Rename( const rux::XString& old_dir , const rux::XString& new_dir , rux::XString& error );
		private:
			static void watcher_thread( void* param , size_t ___rux__thread_index1986 );
		end_declare_rux_class();
	};
};
#endif
