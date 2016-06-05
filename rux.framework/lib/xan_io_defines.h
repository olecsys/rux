#ifndef XAN_IO_DEFINES_H
#define XAN_IO_DEFINES_H
#include <xan_string.h>
#include <xan_stream.h>
enum XFileMode
{
	XReadText,	
	XReadBinary,
	XOpenWriteBinary,
	XCreateWriteBinary,
	XOpenWriteText,
	XCreateWriteText
};
namespace rux
{
	class bytes
	{
	public:
		static bool revert( ::rux::uint8* data , size_t data_size );
		static bool revert( ::rux::uint8* dest , ::rux::uint8* src , size_t data_size );
	};
	class security
	{
	public:
		static bool get_user_id( const char* username , ::rux::uint32& uid , ::rux::uint32& gid );
		static bool get_current_user_id( ::rux::uint32& uid );
		static bool get_current_user_name( char* user_name , size_t user_name_size );
	};
	namespace io
	{	
		dll_internal FILE* fopen( const char * _Filename, const char * _Mode );
		dll_internal ::rux::int32 get_errno( void );
		dll_internal ::rux::int32 fclose( FILE * _File );
		dll_internal ::rux::int64 ftell64( FILE * _File );
		dll_internal ::rux::int32 fseek64( FILE* _File , ::rux::int64 _Offset , ::rux::int32 _Origin );
		dll_internal FILE* freopen( const char * _Filename , const char * _Mode , FILE* _File );
		dll_internal size_t fwrite( const void* _Str , size_t _Size , size_t _Count , FILE* _File );
		dll_internal size_t fread( void* _DstBuf , size_t _ElementSize , size_t _Count, FILE* _File );
		dll_internal ::rux::int32 _ferror( FILE* _File );
		dll_internal ::rux::int32 _feof( FILE* _File );
		dll_internal ::rux::int32 fputs( const char* _Str, FILE* _File );
		dll_internal int vfprintf( FILE* stream , const char* format , va_list arg );
		dll_internal ::rux::int32 fflush( FILE* _File );
		dll_internal ::rux::int32 chsize( ::rux::int32 _FileHandle, long _Size );
		void GetDirectories( const ::rux::XString& directory_name , ::rux::XArray< ::rux::XString >& directories );
		Array< ::rux::XString >& GetAllDirectories( const ::rux::XString& directory_name );
		class file;
		class directory
		{
		private:
			char* _dir;
		public:
			directory( const char* dir );
			~directory();
			char* get_directory_path( void );
			static bool create( const char* dir );
			static bool remove( const char* dir );
			static bool remove_all( const char* dir );
			static bool rename( const char* old_dir , const char* new_dir );
			static bool get_files( const char* dir , XMallocArray< file* >& files );
			static bool get_all_files( const char* dir , XMallocArray< file* >& files );
			static bool get_directories( const char* dir , XMallocArray< directory* >& directories );
			static bool get_all_directories( const char* dir , XMallocArray< directory* >& directories );
			static bool exists( const char* filename );
		};
		class file : public ::rux::io::stream
		{
		private:
			char* _filename;
			FILE* _file;
			XFileMode _file_mode;
			bool read( char* buffer , size_t buffer_size , size_t& readen );
			bool write( char* buffer , size_t buffer_size , size_t& written );
		public:
			file();
			file( const char* filename , XFileMode file_mode = XReadText );
			virtual ~file();
			bool write_bytes( char* buffer , size_t buffer_size , size_t& written );
			bool write_text( const char* format , ... );
			bool write_text_va_list( const char* format , va_list pargs );
			bool read_text( char* buffer , size_t buffer_size , size_t& readen );
			bool read_bytes( char* buffer , size_t buffer_size , size_t& readen );
			bool clear( void );
			bool opened( void );
			bool reopen( XFileMode file_mode = XReadText );
			bool open( const char* filename , XFileMode file_mode = XReadText );
			bool close( void );
			bool copy( const char* dst_filename );
			bool seek( ::rux::uint64 position , XEnum_SetPosition type = XEnum_SetPosition_FromBegin );
			::rux::uint64 size( void );
			::rux::uint64 position( void );
			bool size( ::rux::uint64 size );
			bool rename( const char* new_file );
			bool eof( void );
			static ::rux::uint8 exists( const char* filename , ::rux::uint8 find_link_too = 0 );
			static void* read_all( const char* filename , size_t& readen_size );
			static void append( const char* filename , const char* text );
			static ::rux::uint64 get_size( const char* filename );
			static void remove( const char* filename );
			bool remove( void );
			static bool rename( const char* old_file , const char* new_file );
		};
		class path
		{
		public:
			static void get_filename_without_extension( const char* filename , char destination[ 1024 ] );
			static void get_filename_without_extension( const char* filename );
			static void get_filename_without_extension( ::rux::string& filename );
			static void get_filename( ::rux::string& filename );
		};
	};
};
#endif