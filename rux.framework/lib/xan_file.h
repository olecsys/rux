#ifndef XAN_FILE_H
#define XAN_FILE_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_string.h>
#include <xan_io_defines.h>
namespace rux
{	
	namespace io
	{		
		class Directory;
		class XDirectory;
		begin_declare_rux_class_with_namespace( File , rux::io );
			friend class rux::io::Directory;
			friend class rux::io::XDirectory;
			~File();
			rux::uint8 Delete( void );
			virtual void Close( void );
			rux::uint8 IsEOF( void );
			int64 get_FileSize( rux::XString& error );
			rux::XString& get_FileName( void );
			void ReadBytes( char* buffer_ptr , size_t buffer_size , size_t& readen_size , rux::XString& error );
			rux::uint8 Exists();
			void ReOpen( XFileMode file_mode , rux::XString& error );
			void private_UpdateFileSize( rux::XString& error );
			rux::String& ReadTextAll( rux::XString& error );
			rux::String& get_FileExtension( void );
		begin_declare_rux_class_members( File );
			FILE* _file_ptr;
			rux::XString _file_path;
			int64 _file_size;
			XFileMode _file_mode;	
		end_declare_rux_class_members( File );
	public:	
			static XFile* ctor();
			rux::uint8 Delete( void );
			rux::uint8 Exists();
			void ReadBytes( char* buffer_ptr , size_t buffer_size , size_t& readen_size , rux::XString& error );
			char* ReadBytes( size_t portion_size , size_t& readen_size , rux::XString& error );
			rux::uint8 IsEOF( void );
			rux::String& ReadTextAll( rux::XString& error );	
			void WriteBytes( char* buffer_ptr , int64 buffer_ptr_size , rux::XString& error );
			void AppendText( const char* text_ptr , rux::XString& error );
			virtual void AppendText( const rux::XString& text , rux::XString& error );
			void AppendText( const rux::String& text , rux::XString& error );
			rux::XString& get_FileName( void );
			rux::String& get_FileExtension( void );
			int64 get_FileSize( rux::XString& error );
			void Rename( const rux::XString& new_filename , rux::XString& error );
			void set_FileSize( int64 size , rux::XString& error );
			void set_CreationFileTime( int64 creation_file_time_utc , rux::XString& error );
			void set_WriteFileTime( int64 creation_file_time_utc , rux::XString& error );
			int64 get_CreationFileTime( void );
			int64 get_LastAccessFileTime( void );
			int64 get_LastWriteFileTime( void );
			virtual void Close( void );
			void set_Position( rux::uint32 position , rux::XString& error );
			void set_PositionFromCurrentPosition( rux::uint32 position , rux::XString& error );
			XFile& operator =(rux::int32 null_ptr);
			static File& Create( const rux::XString& path , rux::XString& error , uint64 file_size = 0 , rux::uint8 is_binary = 0 );	
			static File& Open( const rux::XString& file_path , rux::XString& error );
			static File& Open( const rux::String& file_path , rux::XString& error );
			static rux::uint8 Delete( char* file_path_ptr , rux::XString& error );
			static rux::uint8 Delete( const rux::XString& file_path , rux::XString& error );
			static File& Open( char* file_path_ptr , rux::XString& error );
			static rux::uint8 Exists( char* file_path_ptr );
			static rux::uint8 Exists( wchar_t* file_path_ptr );
			static rux::uint8 Exists( const rux::XString& file_path );
			static rux::int32 Copy( char* source_file_name_ptr , char* destination_file_name_ptr );
			static rux::uint8 Copy( const rux::XString& source_file_name , const rux::XString& destination_file_name , rux::XString& error);	
			static void AddAllowExecuteAccess( const rux::XString& file_name , const rux::XString& user_or_group , rux::XString& error );
			static void AddAllowAllAccess( const rux::XString& file_name , const rux::XString& user_or_group , rux::XString& error );
			static bool Equals( const rux::XString& file_path0 , const rux::XString& file_path1 , rux::XString& error );
		private:
			void private_UpdateFileSize( rux::XString& error );
			void ReOpen( XFileMode file_mode , rux::XString& error );
			static void ReOpen( FILE*& file , XFileMode& current_file_mode , const rux::XString& filename , XFileMode file_mode , rux::XString& error );
		end_declare_rux_class();
	};
};
#endif
