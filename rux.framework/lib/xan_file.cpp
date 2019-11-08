#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_file.h"
#include "xan_error.h"
#include "xan_console.h"
#include "xan_path.h"
#ifdef __WINDOWS__
#include <AclAPI.h>
#endif
#include "xan_uint32.h"
#include "xan_keyvaluepair.h"
#include "xan_uint64.h"
#include "xan_boolean.h"
const rux::uint8 g_utf8_header[] = { 0xef , 0xbb , 0xbf };
begin_implement_rux_class_ns( File , rux::io );
	_file_ptr = NULL;
	_file_mode = XReadText;
end_implement_rux_class();
namespace rux
{
	namespace io
	{
		File::~File()
		{
			if( _file_ptr != NULL )
				rux::io::fclose( _file_ptr );
		};	
		implement_duplicate_internal_result_function_1( rux::String& , File , ReadTextAll , rux::XString& );	
		implement_duplicate_internal_result_function( rux::String& , File , get_FileExtension );
		XFile* XFile::ctor()
		{
			return rux::engine::alloc_object<XFile>();
		};
		rux::uint8 File::Delete( void )
		{
			rux::uint8 is_success = 0;
			if( _file_ptr )
			{
				rux::io::fclose( _file_ptr );
				_file_ptr = NULL;
			}
			if( _file_path.Length() > 0 )
			{
		#ifdef __WINDOWS__			
				if( _file_path()->_code_page == XEnumCodePage_UTF16 )	
					is_success = _wremove( (wchar_t*)_file_path.str() ) == 0 ? 1 : 0;			
				else if( _file_path()->_code_page == XEnumCodePage_CP1251 )
					is_success = remove( _file_path.str() ) == 0 ? 1 : 0;				
				else if( _file_path()->_code_page == XEnumCodePage_UTF8 )
				{
					rux::XString utf16_file_path;
					utf16_file_path.set_ByRef( _file_path.ConvertToUTF16() );
					is_success = _wremove( (wchar_t*)utf16_file_path.str() ) == 0 ? 1 : 0;			
				}
		#endif
		#ifdef __UNIX__
				rux::XString utf8;
				utf8.set_ByRef( _file_path.ConvertToUTF8() );
				is_success = remove( utf8.str() ) == 0 ? 1 : 0;					
		#endif
			}
			return is_success;
		}
		void File::Close()
		{
			if( _file_ptr )
			{
				rux::io::fclose( _file_ptr );
				_file_ptr = NULL;
			}
			_file_path.Clear();
			_file_size = 0;	
		}
		rux::uint8 File::Exists()
		{
			rux::uint8 is_exists = 0;
			if( _file_path.Length() > 0 )
			{
		#ifdef __WINDOWS__		
				if( _file_path()->_code_page == XEnumCodePage_UTF16 )
				{
					is_exists = _waccess( (wchar_t*)_file_path.str() , 0 ) != -1 ? 1 : 0;
					if( is_exists == 1 )
					{
						struct _stat st;
						if( _wstat( (wchar_t*)_file_path.str() , &st ) == 0 )
						{
							if( ( st.st_mode & _S_IFREG ) == 0 )
								is_exists = 0;
						}
					}
				}
				else if( _file_path()->_code_page == XEnumCodePage_CP1251 )
				{
					is_exists = _access( _file_path.str() , 0 ) != -1 ? 1 : 0;
					if( is_exists == 1 )
					{
						struct _stat st;
						if( _stat( _file_path.str() , &st ) == 0 )
						{
							if( ( st.st_mode & _S_IFREG ) == 0 )
								is_exists = 0;
						}
					}
				}
				else
				{
					rux::XString utf16_file_path;
					utf16_file_path.set_ByRef( _file_path.ConvertToUTF16() );
					is_exists = _waccess( (wchar_t*)utf16_file_path.str() , 0 ) != -1 ? 1 : 0;
					if( is_exists == 1 )
					{
						struct _stat st;
						if( _wstat( (wchar_t*)utf16_file_path.str() , &st ) == 0 )
						{
							if( ( st.st_mode & _S_IFREG ) == 0 )
								is_exists = 0;
						}
					}
				}
		#endif
		#ifdef __UNIX__
				rux::XString utf8;
				utf8.set_ByRef( _file_path.ConvertToUTF8() );
				is_exists = access( utf8.str() , F_OK ) != -1 ? 1 : 0;
				if( is_exists == 1 )
				{
					struct stat st;
					if( stat( utf8.str() , &st ) == 0 )
					{
						if( S_ISREG( st.st_mode ) == 0 )
							is_exists = 0;
					}
				}
		#endif
			}	
			return is_exists;
		};
		void File::ReadBytes( char* buffer_ptr , size_t buffer_size , size_t& readen_size , rux::XString& error )
		{
			error.Clear();
			readen_size = 0;
			if( buffer_ptr && buffer_size > 0 )
			{
				ReOpen( XReadBinary , error );		
				if( _file_ptr )
				{
					do
					{
						size_t size = rux::io::fread( &buffer_ptr[ readen_size ] , sizeof( char ) , buffer_size - readen_size , _file_ptr );
						if( size > 0 )
							readen_size += size;
						else
							break;
					}
					while( buffer_size != readen_size );
					if( readen_size < buffer_size )
					{				
						if( rux::io::_feof( _file_ptr ) == 0 )
						{
							error = XError::ConvertErrorCodeToString( rux::io::get_errno() , XEnum_ErrorType_Errno );
							readen_size = 0;					
						}
					}
				}
			}
		}
		implement_duplicate_internal_function_2( File , ReOpen , XFileMode , rux::XString& );
		char* XFile::ReadBytes( size_t portion_size , size_t& readen_size , rux::XString& error )
		{
			readen_size = 0;
			ReOpen( XReadBinary , error );
			char* buffer = NULL;
			if( (*this)()->_file_ptr )
			{
				buffer = alloc_array_macros( char , portion_size );
				readen_size = rux::io::fread( buffer , sizeof( char ) , portion_size , (*this)()->_file_ptr );
				if( readen_size < portion_size )
				{			
					if( rux::io::_feof( (*this)()->_file_ptr ) == 0 )
					{
						error = XError::ConvertErrorCodeToString( rux::io::get_errno() , XEnum_ErrorType_Errno );
						rux::engine::free_mem( buffer );
						buffer = NULL;
						readen_size = 0;
					}
				}
			}
			return buffer;
		}
		implement_duplicate_internal_result_function_1( int64 , File , get_FileSize , rux::XString& );
		implement_duplicate_internal_result_function( rux::XString& , File , get_FileName );
		rux::uint8 File::IsEOF( void )
		{
			return _file_ptr && rux::io::_feof( _file_ptr ) != 0 ? 1 : 0;
		}
		rux::String& File::ReadTextAll( rux::XString& error )
		{	
			int64 size = get_FileSize( error );
			if( size > 0 )
			{		
				ReOpen( XReadText , error );
				if( _file_ptr != NULL )
				{
					rux::XString str( size + 1 );
					rux::io::fseek64( _file_ptr , 0L , SEEK_SET );
					size_t readen_size = 0;			
					if( size >= 3 )
					{
						rux::uint8 buffer[ 3 ] = {0};
						readen_size = rux::io::fread( buffer, sizeof( rux::uint8 ) , 3 , _file_ptr );
						if( readen_size != 3 || memcmp( buffer , g_utf8_header , 3 ) != 0 )
							rux::io::fseek64( _file_ptr , 0L , SEEK_SET );
						else
							str()->_code_page = XEnumCodePage_UTF8;
					}
					size_t offset = 0;
					while( ( readen_size = rux::io::fread( &str.str()[ offset ] , sizeof( char ) , size , _file_ptr ) ) > 0 )
					{
						offset += readen_size;
						if( readen_size == size )
							break;
						size -= readen_size;
					}			
					if( rux::io::_ferror( _file_ptr ) && rux::io::_feof( _file_ptr ) == 0 )
						error = XError::ConvertErrorCodeToString( rux::io::get_errno() , XEnum_ErrorType_Errno );
					str.str()[ offset ] = '\0';
					str()->_count = offset + 1;
					return str++;
				}		
			}
			return XString()++;
		};
		void XFile::WriteBytes( char* buffer_ptr , int64 buffer_ptr_size , rux::XString& error )
		{
			ReOpen( XOpenWriteBinary , error );	
			if( (*this)()->_file_ptr )
			{
				if( buffer_ptr )
				{
					if( buffer_ptr_size > 0 )
					{
						int64 offset = 0 , written_size = 0;
						do
						{					
							written_size = rux::io::fwrite( &buffer_ptr[ offset ] , sizeof( char ) , buffer_ptr_size - offset , (*this)()->_file_ptr );
							if( written_size > 0 )
								offset += written_size;
							else
								break;
						}
						while( buffer_ptr_size > offset );
						if( buffer_ptr_size != offset )
							error = XError::ConvertErrorCodeToString( rux::io::get_errno() , XEnum_ErrorType_Errno );
						else
							rux::io::fflush( (*this)()->_file_ptr );
					}
					else
						error = "buffer size is less or equal 0";
				}
				else
					error = "buffer pointer is NULL";
			}
			else
				error = "file '" + (*this)()->_file_path + "' is not opened or do not exists";
		}
		void XFile::AppendText( const char* text_ptr , rux::XString& error )
		{
			ReOpen( XOpenWriteText , error );
			if( (*this)()->_file_ptr != NULL 
				&& text_ptr )
			{
				if( XString::get_global_code_page() == XEnumCodePage_UTF8 )
				{
					rux::io::fseek64( (*this)()->_file_ptr , 0L , SEEK_SET );
					if( rux::io::_feof( (*this)()->_file_ptr ) != 0 || rux::io::get_errno() == EINVAL )
					{
						if( rux::io::fwrite( g_utf8_header , sizeof( char ) , 3 , (*this)()->_file_ptr ) == 0 )
							error = XError::ConvertErrorCodeToString( rux::io::get_errno() , XEnum_ErrorType_Errno );
					}
				}
				if( error.Length() == 0 )
				{
					rux::io::fseek64( (*this)()->_file_ptr , 0L , SEEK_END );
					if( XString::LengthA( text_ptr ) > 0 )
					{
						if( rux::io::fputs( text_ptr , (*this)()->_file_ptr ) == EOF )
							error = XError::ConvertErrorCodeToString( rux::io::get_errno() , XEnum_ErrorType_Errno );
					}
				}
				rux::io::fflush( (*this)()->_file_ptr );
			}
		}
		void XFile::AppendText( const rux::XString& text , rux::XString& error )
		{
			ReOpen( XOpenWriteText , error );
			if( (*this)()->_file_ptr != NULL )
			{
				if( XString::get_global_code_page() == XEnumCodePage_UTF8 || text()->_code_page == XEnumCodePage_UTF8 )
				{
					rux::io::fseek64( (*this)()->_file_ptr , 0L , SEEK_SET );
					if( rux::io::_feof( (*this)()->_file_ptr ) != 0 || rux::io::get_errno() == EINVAL )			
					{
						if( rux::io::fwrite( g_utf8_header , sizeof( char ) , 3 , (*this)()->_file_ptr ) == 0 )
							error = XError::ConvertErrorCodeToString( rux::io::get_errno() , XEnum_ErrorType_Errno );
					}
				}
				if( error.Length() == 0 )
				{
					rux::io::fseek64( (*this)()->_file_ptr , 0L , SEEK_END );		
					if( text.Length() > 0 )
					{
						if( XString::get_global_code_page() == XEnumCodePage_UTF8 )			
						{
							rux::XString utf8;
							utf8.set_ByRef( text.ConvertToUTF8() );
							if( rux::io::fputs( utf8.str() , (*this)()->_file_ptr ) == EOF )
								error = XError::ConvertErrorCodeToString( rux::io::get_errno() , XEnum_ErrorType_Errno );
						}
						else
						{
							if( text()->_code_page == XEnumCodePage_UTF16 )
							{
								if( fputws( (wchar_t*)text.str() , (*this)()->_file_ptr ) == EOF )
									error = XError::ConvertErrorCodeToString( rux::io::get_errno() , XEnum_ErrorType_Errno );
							}
							else if( text()->_code_page == XEnumCodePage_CP1251 )
							{
								if( rux::io::fputs( text.str() , (*this)()->_file_ptr ) == EOF )
									error = XError::ConvertErrorCodeToString( rux::io::get_errno() , XEnum_ErrorType_Errno );
							}
							else if( text()->_code_page == XEnumCodePage_UTF8 )
							{
								int64 buffer_ptr_size = text.Size() - 1;
								int64 offset = 0 , written_size = 0;
								do
								{
									written_size = rux::io::fwrite( &text.str()[ offset ] , sizeof( char ) , buffer_ptr_size - offset , (*this)()->_file_ptr );
									if( written_size > 0 )
										offset += written_size;
									else
										break;
								}
								while( buffer_ptr_size > offset );
								if( buffer_ptr_size != offset )
									error = XError::ConvertErrorCodeToString( rux::io::get_errno() , XEnum_ErrorType_Errno );
								else
									rux::io::fflush( (*this)()->_file_ptr );
							}
							else
							{
								rux::XString utf16_text;
								utf16_text.set_ByRef( text.ConvertToUTF16() );
								if( fputws( (wchar_t*)utf16_text.str() , (*this)()->_file_ptr ) == EOF )
									error = XError::ConvertErrorCodeToString( rux::io::get_errno() , XEnum_ErrorType_Errno );
							}
						}			
					}
					rux::io::fflush( (*this)()->_file_ptr );
				}
			}
		};
		void XFile::AppendText( const rux::String& text , rux::XString& error )
		{
			text.AddRef();
			if( (*this)()->_file_ptr != NULL )
			{
				if( XString::get_global_code_page() == XEnumCodePage_UTF8 || text._code_page == XEnumCodePage_UTF8 )
				{
					rux::io::fseek64( (*this)()->_file_ptr , 0L , SEEK_SET );
					if( rux::io::_feof( (*this)()->_file_ptr ) != 0 || rux::io::get_errno() == EINVAL )			
					{
						if( rux::io::fwrite( g_utf8_header , sizeof( char ) , 3 , (*this)()->_file_ptr ) == 0 )
							error = XError::ConvertErrorCodeToString( rux::io::get_errno() , XEnum_ErrorType_Errno );
					}
				}
				if( error.Length() == 0 )
				{
					rux::io::fseek64( (*this)()->_file_ptr , 0L , SEEK_END );		
					if( text.Length() > 0 )
					{
						if( XString::get_global_code_page() == XEnumCodePage_UTF8 )			
						{
							rux::XString utf8 = text;
							utf8.set_ByRef( utf8.ConvertToUTF8() );
							if( rux::io::fputs( utf8.str() , (*this)()->_file_ptr ) == EOF )
								error = XError::ConvertErrorCodeToString( rux::io::get_errno() , XEnum_ErrorType_Errno );
						}
						else
						{
							if( text._code_page == XEnumCodePage_UTF16 )
							{
								if( fputws( (wchar_t*)text.str() , (*this)()->_file_ptr ) == EOF )
									error = XError::ConvertErrorCodeToString( rux::io::get_errno() , XEnum_ErrorType_Errno );
							}
							else if( text._code_page == XEnumCodePage_CP1251 )
							{
								if( rux::io::fputs( text.str() , (*this)()->_file_ptr ) == EOF )
									error = XError::ConvertErrorCodeToString( rux::io::get_errno() , XEnum_ErrorType_Errno );
							}
							else if( text._code_page == XEnumCodePage_UTF8 )
							{
								int64 buffer_ptr_size = text._count - 1;
								int64 offset = 0 , written_size = 0;
								do
								{
									written_size = ::rux::io::fwrite( &text.str()[ offset ] , sizeof( char ) , buffer_ptr_size - offset , (*this)()->_file_ptr );
									if( written_size > 0 )
										offset += written_size;
									else
										break;
								}
								while( buffer_ptr_size > offset );
								if( buffer_ptr_size != offset )
									error = XError::ConvertErrorCodeToString( rux::io::get_errno() , XEnum_ErrorType_Errno );
								else
									rux::io::fflush( (*this)()->_file_ptr );
							}
							else
							{
								rux::XString utf16_text = text;
								utf16_text.set_ByRef( utf16_text.ConvertToUTF16() );
								if( fputws( (wchar_t*)utf16_text.str() , (*this)()->_file_ptr ) == EOF )
									error = XError::ConvertErrorCodeToString( rux::io::get_errno() , XEnum_ErrorType_Errno );
							}
						}			
					}
					rux::io::fflush( (*this)()->_file_ptr );
				}
			}
			text.Release();
		};
		implement_duplicate_internal_result_function( rux::uint8 , File , Exists );
		implement_duplicate_internal_function_4( File , ReadBytes , char* , size_t , size_t& , rux::XString& );
		XString& File::get_FileName( void )
		{
			return _file_path;
		};
		rux::String& File::get_FileExtension( void )
		{
			return XPath::GetFileNameExtension( _file_path );
		};
		void XFile::Rename( const rux::XString& new_filename , rux::XString& error )
		{
			error.Clear();
			if( (*this)()->_file_ptr )
			{
				rux::io::fclose( (*this)()->_file_ptr );
				(*this)()->_file_ptr = NULL;
			}
			rux::XString current_cp_new_filename;
			current_cp_new_filename.set_ByRef( new_filename.ConvertToAnsi() );
			current_cp_new_filename.set_ByRef( XPath::ExcludeRelative( current_cp_new_filename ) );
			if( rename( (*this)()->_file_path.str() , current_cp_new_filename.str() ) == 0 )
				(*this)()->_file_path.set_ByRef( current_cp_new_filename );
			else
				error = XError::ConvertErrorCodeToString( errno , XEnum_ErrorType_Errno );
		};
		void XFile::set_FileSize( int64 size , rux::XString& error )
		{	
			if( size >= 0 )
			{		
				ReOpen( XOpenWriteText , error );
				if( (*this)()->_file_ptr != NULL )
				{
		#ifdef __WINDOWS__
					if( rux::io::chsize( _fileno( (*this)()->_file_ptr ) , size ) != 0 )
		#else
					if( rux::io::chsize( fileno( (*this)()->_file_ptr ) , size ) != 0 )
		#endif
						error = XError::ConvertErrorCodeToString( rux::io::get_errno() , XEnum_ErrorType_Errno );
				}
			}
			else
				error = "size is negative";
		};
		implement_duplicate_internal_function_1( File , private_UpdateFileSize , rux::XString& );
		int64 File::get_FileSize( rux::XString& error )
		{	
			private_UpdateFileSize( error );
			return _file_size;
		};
		void XFile::set_WriteFileTime( int64 last_write_file_time , rux::XString& error )
		{
			error.Clear();
			if( (*this)()->_file_ptr != NULL )
				rux::io::fclose( (*this)()->_file_ptr );
		#ifdef __WINDOWS__				
			HANDLE handle = INVALID_HANDLE_VALUE;
			if( (*this)()->_file_path()->_code_page == XEnumCodePage_CP1251 )
				handle = CreateFileA( (*this)()->_file_path.str() , GENERIC_READ | GENERIC_WRITE , FILE_SHARE_READ | FILE_SHARE_WRITE , NULL , OPEN_EXISTING , FILE_ATTRIBUTE_NORMAL , NULL );
			else if( (*this)()->_file_path()->_code_page == XEnumCodePage_UTF16 )
				handle = CreateFileW( (wchar_t*)(*this)()->_file_path.str() , GENERIC_READ | GENERIC_WRITE , FILE_SHARE_READ | FILE_SHARE_WRITE , NULL , OPEN_EXISTING , FILE_ATTRIBUTE_NORMAL , NULL );
			else
			{
				rux::XString utf16_file_path;
				utf16_file_path.set_ByRef( (*this)()->_file_path.ConvertToUTF16() );
				handle = CreateFileW( (wchar_t*)utf16_file_path.str() , GENERIC_READ | GENERIC_WRITE , FILE_SHARE_READ | FILE_SHARE_WRITE , NULL , OPEN_EXISTING , FILE_ATTRIBUTE_NORMAL , NULL );
			}
			if( handle != INVALID_HANDLE_VALUE )
			{
				if( !SetFileTime( handle , NULL , NULL , (FILETIME*)&last_write_file_time ) )		
					error = XError::ConvertErrorCodeToString( GetLastError() , XEnum_ErrorType_GetLastError );
				CloseHandle( handle );
			}		
			else
				error = XError::ConvertErrorCodeToString( GetLastError() , XEnum_ErrorType_GetLastError );
		#endif
		#ifdef __UNIX__
			(void)last_write_file_time;
			//stat
		#endif		
			ReOpen( (*this)()->_file_mode , error );
		};
		void XFile::set_CreationFileTime( int64 creation_file_time_utc , rux::XString& error )
		{		
			error.Clear();
			if( (*this)()->_file_ptr != NULL )
				rux::io::fclose( (*this)()->_file_ptr );
		#ifdef __WINDOWS__				
			HANDLE handle = INVALID_HANDLE_VALUE;
			if( (*this)()->_file_path()->_code_page == XEnumCodePage_CP1251 )
				handle = CreateFileA( (*this)()->_file_path.str() , GENERIC_READ | GENERIC_WRITE , FILE_SHARE_READ | FILE_SHARE_WRITE , NULL , OPEN_EXISTING , FILE_ATTRIBUTE_NORMAL , NULL );
			else if( (*this)()->_file_path()->_code_page == XEnumCodePage_UTF16 )
				handle = CreateFileW( (wchar_t*)(*this)()->_file_path.str() , GENERIC_READ | GENERIC_WRITE , FILE_SHARE_READ | FILE_SHARE_WRITE , NULL , OPEN_EXISTING , FILE_ATTRIBUTE_NORMAL , NULL );
			else
			{
				rux::XString utf16_file_path;
				utf16_file_path.set_ByRef( (*this)()->_file_path.ConvertToUTF16() );
				handle = CreateFileW( (wchar_t*)utf16_file_path.str() , GENERIC_READ | GENERIC_WRITE , FILE_SHARE_READ | FILE_SHARE_WRITE , NULL , OPEN_EXISTING , FILE_ATTRIBUTE_NORMAL , NULL );
			}
			if( handle != INVALID_HANDLE_VALUE )
			{
				if( !SetFileTime( handle , (FILETIME*)&creation_file_time_utc , NULL , NULL ) )		
					error = XError::ConvertErrorCodeToString( GetLastError() , XEnum_ErrorType_GetLastError );
				CloseHandle( handle );
			}		
			else
				error = XError::ConvertErrorCodeToString( GetLastError() , XEnum_ErrorType_GetLastError );
		#endif
		#ifdef __UNIX__
			(void)creation_file_time_utc;
			//stat
		#endif		
			ReOpen( (*this)()->_file_mode , error );
		};
		int64 XFile::get_CreationFileTime( void )
		{	
			int64 creation_file_time_utc = 0;
		#ifdef __WINDOWS__				
			HANDLE handle = INVALID_HANDLE_VALUE;
			if( (*this)()->_file_path()->_code_page == XEnumCodePage_CP1251 )
				handle = CreateFileA( (*this)()->_file_path.str() , GENERIC_READ , FILE_SHARE_READ , NULL , OPEN_EXISTING , FILE_ATTRIBUTE_NORMAL , NULL );
			else if( (*this)()->_file_path()->_code_page == XEnumCodePage_UTF16 )
				handle = CreateFileW( (wchar_t*)(*this)()->_file_path.str() , GENERIC_READ , FILE_SHARE_READ , NULL , OPEN_EXISTING , FILE_ATTRIBUTE_NORMAL , NULL );
			else
			{
				rux::XString utf16_file_path;
				utf16_file_path.set_ByRef( (*this)()->_file_path.ConvertToUTF16() );
				handle = CreateFileW( (wchar_t*)utf16_file_path.str() , GENERIC_READ , FILE_SHARE_READ , NULL , OPEN_EXISTING , FILE_ATTRIBUTE_NORMAL , NULL );
			}
			if( handle != INVALID_HANDLE_VALUE )
			{
				if( !GetFileTime( handle , (FILETIME*)&creation_file_time_utc , NULL , NULL ) )
				{
					creation_file_time_utc = 0;
				}
				CloseHandle( handle );
			}		
		#endif
		#ifdef __UNIX__
			//stat
		#endif
			return creation_file_time_utc;
		};
		int64 XFile::get_LastAccessFileTime( void )
		{	
			int64 last_access_file_time = 0;
		#ifdef __WINDOWS__				
			HANDLE handle = INVALID_HANDLE_VALUE;
			if( (*this)()->_file_path()->_code_page == XEnumCodePage_CP1251 )
				handle = CreateFileA( (*this)()->_file_path.str() , GENERIC_READ , FILE_SHARE_READ , NULL , OPEN_EXISTING , FILE_ATTRIBUTE_NORMAL , NULL );
			else if( (*this)()->_file_path()->_code_page == XEnumCodePage_UTF16 )
				handle = CreateFileW( (wchar_t*)(*this)()->_file_path.str() , GENERIC_READ , FILE_SHARE_READ , NULL , OPEN_EXISTING , FILE_ATTRIBUTE_NORMAL , NULL );
			else
			{
				rux::XString utf16_file_path;
				utf16_file_path.set_ByRef( (*this)()->_file_path.ConvertToUTF16() );
				handle = CreateFileW( (wchar_t*)utf16_file_path.str() , GENERIC_READ , FILE_SHARE_READ , NULL , OPEN_EXISTING , FILE_ATTRIBUTE_NORMAL , NULL );
			}
			if( handle != INVALID_HANDLE_VALUE )
			{
				if( !GetFileTime( handle , NULL , (FILETIME*)&last_access_file_time , NULL ) )
				{
					last_access_file_time = 0;
				}
				CloseHandle( handle );
			}		
		#endif
		#ifdef __UNIX__
			//stat
		#endif
			return last_access_file_time;
		};
		int64 XFile::get_LastWriteFileTime( void )
		{	
			int64 last_write_file_time = 0;
		#ifdef __WINDOWS__				
			HANDLE handle = INVALID_HANDLE_VALUE;
			if( (*this)()->_file_path()->_code_page == XEnumCodePage_CP1251 )
				handle = CreateFileA( (*this)()->_file_path.str() , GENERIC_READ , FILE_SHARE_READ , NULL , OPEN_EXISTING , FILE_ATTRIBUTE_NORMAL , NULL );
			else if( (*this)()->_file_path()->_code_page == XEnumCodePage_UTF16 )
				handle = CreateFileW( (wchar_t*)(*this)()->_file_path.str() , GENERIC_READ , FILE_SHARE_READ , NULL , OPEN_EXISTING , FILE_ATTRIBUTE_NORMAL , NULL );
			else
			{
				rux::XString utf16_file_path;
				utf16_file_path.set_ByRef( (*this)()->_file_path.ConvertToUTF16() );
				handle = CreateFileW( (wchar_t*)utf16_file_path.str() , GENERIC_READ , FILE_SHARE_READ , NULL , OPEN_EXISTING , FILE_ATTRIBUTE_NORMAL , NULL );
			}
			if( handle != INVALID_HANDLE_VALUE )
			{
				if( !GetFileTime( handle , NULL , NULL , (FILETIME*)&last_write_file_time ) )
				{
					last_write_file_time = 0;
				}
				CloseHandle( handle );
			}		
		#endif
		#ifdef __UNIX__
			//stat
		#endif
			return last_write_file_time;
		};
		XFile& XFile::operator =(rux::int32 null_ptr)
		{
			(*(static_cast<XGCHandle<File>*>( this ))) = null_ptr;
			return *this;
		};
		//"r" - READONLY file begin NOT CREATE
		//"r+" - READ AND WRITE file begin NOT CREATE
		//"w"  - WRITE file begin CREATE
		//"w+" - READ AND WRITE file begin CREATE
		//"a"  - 
		//"a+"
		File& XFile::Create( const rux::XString& path , rux::XString& error , uint64 file_size , rux::uint8 is_binary )
		{
			error.Clear();
			rux::io::XFile file;
			file()->_file_path.set_ByRef( XPath::ExcludeRelative( path ) );	
			XFileMode filemode = XCreateWriteBinary;
			if( is_binary == 0 )
				filemode = XCreateWriteText;	
			FILE* file_ptr = NULL;
			ReOpen( file_ptr , file()->_file_mode , file()->_file_path , filemode , error );	
			if( file_ptr )
			{
				if( file_size > 0 )
				{
					rux::io::fseek64( file_ptr , file_size - 1 , SEEK_SET );
					rux::io::fwrite( "\0" , 1 , 1 , file_ptr );
					rux::io::fseek64( file_ptr , 0 , SEEK_SET );
					rux::io::fflush( file_ptr );
				}
				file()->_file_ptr = file_ptr;
				file()->_file_mode = filemode;
			}
			return file++;
		};
		File& XFile::Open( const rux::XString& file_path , rux::XString& error )
		{
			error.Clear();
			rux::io::XFile file;	
			file()->_file_path.set_ByRef( XPath::ExcludeRelative( file_path ) );
			file()->_file_path.set_ByRef( file()->_file_path.ConvertToAnsi() );
			FILE* file_ptr = NULL;
			file_ptr = rux::io::fopen( file()->_file_path.str() , "r" );
			if( file_ptr )
			{
				file()->_file_ptr = file_ptr;
				file()->_file_mode = XReadText;			
			}
			else
				error = XError::ConvertErrorCodeToString( rux::io::get_errno() , XEnum_ErrorType_Errno );
			return file++;
		};
		File& XFile::Open( const rux::String& file_path , rux::XString& error )
		{
			file_path.AddRef();
			error.Clear();
			rux::io::XFile file;
			file()->_file_path.set_ByRef( XPath::ExcludeRelative( file_path ) );
			file()->_file_path.set_ByRef( file()->_file_path.ConvertToAnsi() );
			FILE* file_ptr = NULL;
			file_ptr = rux::io::fopen( file()->_file_path.str() , "r" );
			if( file_ptr )
			{
				file()->_file_ptr = file_ptr;
				file()->_file_mode = XReadText;		
			}
			else
				error = XError::ConvertErrorCodeToString( rux::io::get_errno() , XEnum_ErrorType_Errno );
			file_path.Release();
			return file++;
		};
		rux::uint8 XFile::Delete( const rux::XString& file_path , rux::XString& error )
		{
			rux::uint8 is_success = 0;
			if( file_path.Length() > 0 )
			{
				rux::XString _file_name;
				_file_name.set_ByRef( XPath::ExcludeRelative( file_path ) );
		#ifdef __WINDOWS__
				if( _file_name()->_code_page == XEnumCodePage_CP1251 )
					is_success = remove( _file_name.str() ) == 0 ? 1 : 0;
				else if( _file_name()->_code_page == XEnumCodePage_UTF16 )
					is_success = _wremove( (wchar_t*)_file_name.str() ) == 0 ? 1 : 0;
				else
				{
					rux::XString utf16_file_path;
					utf16_file_path.set_ByRef( _file_name.ConvertToUTF16() );
					is_success = _wremove( (wchar_t*)utf16_file_path.str() ) == 0 ? 1 : 0;
				}
		#endif
		#ifdef __UNIX__
				rux::XString utf8;
				utf8.set_ByRef( _file_name.ConvertToUTF8() );
				is_success = remove( utf8.str() ) == 0 ? 1 : 0;
		#endif
				if( is_success == 0 )
					error = XError::ConvertErrorCodeToString( errno , XEnum_ErrorType_Errno );
			}
			return is_success;
		}
		rux::uint8 XFile::Delete( char* file_path_ptr , rux::XString& error )
		{
			error.Clear();
			rux::uint8 is_success = 0;
			if( file_path_ptr && XString::LengthA( file_path_ptr ) > 0 )	
			{
				is_success = remove( file_path_ptr ) == 0 ? 1 : 0;
				if( is_success == 0 )
					error = XError::ConvertErrorCodeToString( errno , XEnum_ErrorType_Errno );
			}
			return is_success;
		}
		File& XFile::Open( char* file_path_ptr , rux::XString& error )
		{
			error.Clear();
			rux::io::XFile file;
			file()->_file_path.set_ByRef( XPath::ExcludeRelative( file_path_ptr ) );
			FILE* file_ptr = NULL;
		#ifdef __WINDOWS__
			file_ptr = rux::io::fopen( file()->_file_path.str() , "r" );
		#endif
		#ifdef __UNIX__
			rux::XString utf8;
			utf8.set_ByRef( file()->_file_path.ConvertToUTF8() );
			file_ptr = rux::io::fopen( utf8.str() , "r" );
		#endif
			if( file_ptr )
			{
				file()->_file_ptr = file_ptr;
				file()->_file_mode = XReadText;			
			}
			else
				error = XError::ConvertErrorCodeToString( rux::io::get_errno() , XEnum_ErrorType_Errno );
			return file++;
		};
		rux::uint8 XFile::Exists( char* file_path_ptr )
		{
			rux::uint8 is_exists = 0;
			if( file_path_ptr && XString::LengthA( file_path_ptr ) > 0 )
			{
		#ifdef __WINDOWS__
				is_exists = _access( file_path_ptr , 0 ) != -1 ? 1 : 0;
				if( is_exists == 1 )
				{
					struct _stat st;
					if( _stat( file_path_ptr , &st ) == 0 )
					{
						if( ( st.st_mode & _S_IFREG ) == 0 )
							is_exists = 0;
					}
				}
		#endif		
		#ifdef __UNIX__
				rux::XString utf8( file_path_ptr );		
				utf8.set_ByRef( utf8.ConvertToUTF8() );
				is_exists = access( utf8.str() , F_OK ) != -1 ? 1 : 0;
				if( is_exists == 1 )
				{
					struct stat st;
					if( stat( utf8.str() , &st ) == 0 )
					{
						if( S_ISREG( st.st_mode ) == 0 )
							is_exists = 0;
					}
				}
		#endif		
			}
			return is_exists;
		};
		rux::uint8 XFile::Exists( wchar_t* file_path_ptr )
		{
			rux::uint8 is_exists = 0;
			if( file_path_ptr && *(wchar_t*)file_path_ptr != '\0' )
			{
		#ifdef __WINDOWS__
				is_exists = _waccess( file_path_ptr , 0 ) != -1 ? 1 : 0;
				if( is_exists == 1 )
				{
					struct _stat st;
					if( _wstat( file_path_ptr , &st ) == 0 )
					{
						if( ( st.st_mode & _S_IFREG ) == 0 )
							is_exists = 0;
					}
				}
		#endif
		#ifdef __UNIX__
				rux::XString utf8( file_path_ptr );		
				utf8.set_ByRef( utf8.ConvertToUTF8() );
				is_exists = access( utf8.str() , F_OK ) != -1 ? 1 : 0;
				if( is_exists == 1 )
				{
					struct stat st;
					if( stat( utf8.str() , &st ) == 0 )
					{
						if( S_ISREG( st.st_mode ) == 0 )
							is_exists = 0;
					}
				}
		#endif		
			}
			return is_exists;
		};
		rux::uint8 XFile::Exists( const rux::XString& file_path )
		{
			rux::uint8 is_exists = 0;
			if( file_path.Length() > 0 )
			{
				rux::XString _file_path;
				_file_path.set_ByRef( XPath::ExcludeRelative( file_path ) );
		#ifdef __WINDOWS__
				if( _file_path()->_code_page == XEnumCodePage_UTF16 )
				{
					is_exists = _waccess( (wchar_t*)_file_path.str() , 0 ) != -1 ? 1 : 0;
					if( is_exists == 1 )
					{
						struct _stat st;
						if( _wstat( (wchar_t*)_file_path.str() , &st ) == 0 )
						{
							if( ( st.st_mode & _S_IFREG ) == 0 )
								is_exists = 0;
						}
					}
				}
				else if( _file_path()->_code_page == XEnumCodePage_CP1251 )
				{
					is_exists = _access( _file_path.str() , 0 ) != -1 ? 1 : 0;
					if( is_exists == 1 )
					{
						struct _stat st;
						if( _stat( _file_path.str() , &st ) == 0 )
						{
							if( ( st.st_mode & _S_IFREG ) == 0 )
								is_exists = 0;
						}
					}
				}
				else
				{
					rux::XString utf16_file_path;
					utf16_file_path.set_ByRef( _file_path.ConvertToUTF16() );
					is_exists = _waccess( (wchar_t*)utf16_file_path.str() , 0 ) != -1 ? 1 : 0;
					if( is_exists == 1 )
					{
						struct _stat st;
						if( _wstat( (wchar_t*)utf16_file_path.str() , &st ) == 0 )
						{
							if( ( st.st_mode & _S_IFREG ) == 0 )
								is_exists = 0;
						}
					}
				}
		#endif
		#ifdef __UNIX__
				rux::XString utf8;
				utf8.set_ByRef( _file_path.ConvertToUTF8() );
				is_exists = access( utf8.str() , F_OK ) != -1 ? 1 : 0;
				if( is_exists == 1 )
				{
					struct stat st;
					if( stat( utf8.str() , &st ) == 0 )
					{
						if( S_ISREG( st.st_mode ) == 0 )
							is_exists = 0;
					}
				}
		#endif		
			}
			return is_exists;
		};
		rux::uint8 XFile::Copy( const rux::XString& source_file_name , const rux::XString& destination_file_name , rux::XString& error )
		{
			error.Clear();	
			rux::int32 result = 0;
			rux::XString source;
			source.set_ByRef( XPath::ExcludeRelative( source_file_name ) );	
			rux::XString destination;
			destination.set_ByRef( XPath::ExcludeRelative( destination_file_name ) );
			if( source.Length() > 0 && destination.Length() > 0 )
			{					
				rux::XArray<XString> parts = destination.Split( '/' );
				rux::uint32 index0 = 1;
				rux::XString string( parts[0] );
				while( index0 < parts.Count() - 1 )
				{
					string += "/" + parts[ index0 ];
		#ifdef __WINDOWS__
					if( string()->_code_page == XEnumCodePage_UTF16 )
					{
						if( _waccess( (wchar_t*)string.str() , 0 ) == -1 )
						{
							if( _wmkdir( (wchar_t*)string.str() ) != 0 )
							{
								result = errno;
								error = XError::ConvertErrorCodeToString( result , XEnum_ErrorType_Errno );
							}
						}
						else
						{	
							struct _stat st;
							if( _wstat( (wchar_t*)string.str() , &st ) == 0 )
							{
								if( ( st.st_mode & _S_IFDIR ) == 0 )
								{
									if( _wmkdir( (wchar_t*)string.str() ) != 0 )
									{
										result = errno;
										error = XError::ConvertErrorCodeToString( result , XEnum_ErrorType_Errno );
									}
								}
							}
						}
					}
					else if( string()->_code_page == XEnumCodePage_CP1251 )
					{
						if( _access( string.str() , 0 ) == -1 )
						{
							if( _mkdir( string.str() ) != 0 )
							{
								result = errno;
								error = XError::ConvertErrorCodeToString( result , XEnum_ErrorType_Errno );
							}
						}
						else
						{	
							struct _stat st;
							if( _stat( string.str() , &st ) == 0 )
							{
								if( ( st.st_mode & _S_IFDIR ) == 0 )
								{
									if( _mkdir( string.str() ) != 0 )
									{
										result = errno;
										error = XError::ConvertErrorCodeToString( result , XEnum_ErrorType_Errno );
									}
								}
							}	
						}
					}
					else
					{
						rux::XString utf16_string;
						utf16_string.set_ByRef( string.ConvertToUTF16() );
						if( _waccess( (wchar_t*)utf16_string.str() , 0 ) == -1 )
						{
							if( _wmkdir( (wchar_t*)utf16_string.str() ) != 0 )
							{
								result = errno;
								error = XError::ConvertErrorCodeToString( result , XEnum_ErrorType_Errno );
							}
						}
						else
						{
							struct _stat st;
							if( _wstat( (wchar_t*)utf16_string.str() , &st ) == 0 )
							{
								if( ( st.st_mode & _S_IFDIR ) == 0 )
								{
									if( _wmkdir( (wchar_t*)utf16_string.str() ) != 0 )
									{
										result = errno;
										error = XError::ConvertErrorCodeToString( result , XEnum_ErrorType_Errno );
									}
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
						{
							result = errno;
							error = XError::ConvertErrorCodeToString( result , XEnum_ErrorType_Errno );
						}
					}
					else
					{	
						struct stat st;
						if( stat( string.str() , &st ) == 0 )
						{
							if( S_ISDIR( st.st_mode ) == 0 )
							{
								if( mkdir( string.str() , S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH ) != 0 )
								{
									result = errno;
									error = XError::ConvertErrorCodeToString( result , XEnum_ErrorType_Errno );
								}
							}
						}
					}				
		#endif
					index0++;
				}						
				FILE* source_file_ptr = NULL;
				source.set_ByRef( source.ConvertToAnsi() );
				source_file_ptr= rux::io::fopen( source.str() , "rb" );
				if( source_file_ptr )
				{			
					FILE* destination_file_ptr = NULL;
					destination.set_ByRef( destination.ConvertToAnsi() );
					destination_file_ptr= rux::io::fopen( destination.str() , "w+b" );
					if( destination_file_ptr )
					{
						size_t readen_size = 0;
						size_t buffer_ptr_size = 8 * 1024;
						char* buffer_ptr = alloc_array_macros( char , buffer_ptr_size );
						readen_size = rux::io::fread( buffer_ptr , sizeof( char ) , buffer_ptr_size , source_file_ptr );
						while( readen_size > 0 )
						{
							if( readen_size < buffer_ptr_size )
							{
								if( rux::io::_feof( source_file_ptr ) == 0 )
								{
									result = rux::io::get_errno();
									error = XError::ConvertErrorCodeToString( result , XEnum_ErrorType_Errno );
									break;
								}
								else
								{
									if( rux::io::fwrite( buffer_ptr , sizeof( char ) , readen_size , destination_file_ptr ) != readen_size )
									{
										result = rux::io::get_errno();
										error = XError::ConvertErrorCodeToString( result , XEnum_ErrorType_Errno );
										break;
									}
								}
							}
							else
							{
								if( rux::io::fwrite( buffer_ptr , sizeof( char ) , readen_size , destination_file_ptr ) != readen_size )
								{
									result = rux::io::get_errno();
									error = XError::ConvertErrorCodeToString( result , XEnum_ErrorType_Errno );
									break;
								}
							}
							readen_size = rux::io::fread( buffer_ptr , sizeof( char ) , buffer_ptr_size , source_file_ptr );
						}
						if( result == 0 )
						{
							if( rux::io::_feof( source_file_ptr ) == 0 )
							{
								result = rux::io::get_errno();
								error = XError::ConvertErrorCodeToString( result , XEnum_ErrorType_Errno );
							}
						}
						rux::engine::free_mem( buffer_ptr );
						rux::io::fclose( destination_file_ptr );
					}
					else
					{
						result = rux::io::get_errno();
						error = XError::ConvertErrorCodeToString( result , XEnum_ErrorType_Errno );
					}
					rux::io::fclose( source_file_ptr );
				}
				else
				{
					result = rux::io::get_errno();			
					error = XError::ConvertErrorCodeToString( result , XEnum_ErrorType_Errno );
				}
			}
			return result == 0 ? 1 : 0;
		};
		rux::int32 XFile::Copy( char* source_file_name_ptr , char* destination_file_name_ptr )
		{
			rux::int32 error = 0;		
			rux::XString source( source_file_name_ptr );
			source.set_ByRef( XPath::ExcludeRelative( source ) );
			rux::XString destination( destination_file_name_ptr );
			destination.set_ByRef( XPath::ExcludeRelative( destination ) );
			if( source.Length() > 0 && destination.Length() > 0 )
			{			
				rux::XArray<XString> parts = destination.Split( '/' );
				rux::uint32 index0 = 1;
				rux::XString string( parts[0] );
				while( index0 < parts.Count() - 1 )
				{
					string += "/" + parts[ index0 ];
		#ifdef __WINDOWS__
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
		#ifdef __WINDOWS__
				FILE* source_file_ptr = rux::io::fopen( source.str() , "rb" );
		#endif
		#ifdef __UNIX__
				source.set_ByRef( source.ConvertToUTF8() );
				FILE* source_file_ptr = rux::io::fopen( source.str() , "rb" );
		#endif
				if( source_file_ptr )
				{	
		#ifdef __WINDOWS__
					FILE* destination_file_ptr = rux::io::fopen( destination.str() , "w+b" );
		#endif
		#ifdef __UNIX__
					destination.set_ByRef( destination.ConvertToUTF8() );
					FILE* destination_file_ptr = rux::io::fopen( destination.str() , "w+b" );
		#endif
					if( destination_file_ptr )
					{
						size_t readen_size = 0;
						size_t buffer_ptr_size = 8 * 1024;
						char* buffer_ptr = alloc_array_macros( char , buffer_ptr_size );
						readen_size = rux::io::fread( buffer_ptr , sizeof( char ) , buffer_ptr_size , source_file_ptr );
						while( readen_size > 0 )
						{
							if( readen_size < buffer_ptr_size )
							{
								if( rux::io::_feof( source_file_ptr ) == 0 )
								{
									error = rux::io::get_errno();
									break;
								}
								else
								{
									if( rux::io::fwrite( buffer_ptr , sizeof( char ) , readen_size , destination_file_ptr ) != readen_size )
									{
										error = rux::io::get_errno();
										break;
									}
								}
							}
							else
							{
								if( rux::io::fwrite( buffer_ptr , sizeof( char ) , readen_size , destination_file_ptr ) != readen_size )
								{
									error = rux::io::get_errno();
									break;
								}
							}
							readen_size = rux::io::fread( buffer_ptr , sizeof( char ) , buffer_ptr_size , source_file_ptr );
						}
						if( error == 0 )
						{
							if( rux::io::_feof( source_file_ptr ) == 0 )
							{
								error = rux::io::get_errno();
							}
						}
						rux::engine::free_mem( buffer_ptr );
						rux::io::fclose( destination_file_ptr );
					}
					else
						error = rux::io::get_errno();
					rux::io::fclose( source_file_ptr );
				}
				else
					error = rux::io::get_errno();			
			}
			return error;
		};
		void File::private_UpdateFileSize( rux::XString& error )
		{		
			error.Clear();
		#ifdef __WINDOWS__
			if( _file_ptr == NULL )
				ReOpen( XReadText , error );
			if( _file_ptr )
			{
				uint64 cur = rux::io::ftell64( _file_ptr );
				rux::io::fseek64( _file_ptr , 0L , SEEK_END );
				_file_size = rux::io::ftell64( _file_ptr );
				rux::io::fseek64( _file_ptr , cur , SEEK_SET );
			}
		#elif defined( __UNIX__ )
			rux::XString file_path;
			file_path.set_ByRef( _file_path.ConvertToUTF8() );
			struct stat statbuf;
			if( stat( file_path.str() , &statbuf ) != -1 )
				_file_size = statbuf.st_size;
			else
				error = XError::ConvertErrorCodeToString( errno , XEnum_ErrorType_Errno );
		#endif
		};
		void XFile::set_PositionFromCurrentPosition( rux::uint32 position , rux::XString& error )
		{
			error.Clear();
			if( (*this)()->_file_ptr == NULL )
				ReOpen( XReadText , error );
			if( (*this)()->_file_ptr )
				rux::io::fseek64( (*this)()->_file_ptr , position , SEEK_CUR );
		};
		void XFile::set_Position( rux::uint32 position , rux::XString& error )
		{
			error.Clear();
			if( (*this)()->_file_ptr == NULL )
				ReOpen( XReadText , error );
			if( (*this)()->_file_ptr )
				rux::io::fseek64( (*this)()->_file_ptr , position , SEEK_SET );
		}
		void File::ReOpen( XFileMode file_mode , rux::XString& error )
		{
			error.Clear();
			if( _file_ptr && _file_mode != file_mode )
			{
				rux::io::fclose( _file_ptr );
				_file_ptr = NULL;
			}
			if( _file_ptr == NULL )
			{
				_file_path.set_ByRef( _file_path.ConvertToAnsi() );
				if( file_mode == XReadText )
					_file_ptr = rux::io::fopen( _file_path.str() , "rt" );			
				else if( file_mode == XReadBinary )
					_file_ptr = rux::io::fopen( _file_path.str() , "rb" );			
				else if( file_mode == XCreateWriteText )
					_file_ptr = rux::io::fopen( _file_path.str() , "wt" );			
				else if( file_mode == XOpenWriteText )
					_file_ptr = rux::io::fopen( _file_path.str() , "r+t" );			
				else if( file_mode == XCreateWriteBinary )
					_file_ptr = rux::io::fopen( _file_path.str() , "wb" );			
				else if( file_mode == XOpenWriteBinary )
					_file_ptr = rux::io::fopen( _file_path.str() , "r+b" );
			}
			if( _file_ptr == NULL )
				error = XError::ConvertErrorCodeToString( rux::io::get_errno() , XEnum_ErrorType_Errno );
			_file_mode = file_mode;
		};
		void XFile::ReOpen( FILE*& file , XFileMode& current_file_mode , const rux::XString& filename , XFileMode file_mode , rux::XString& error )
		{
			error.Clear();
			if( file && current_file_mode != file_mode )
			{
				rux::io::fclose( file );
				file = NULL;
			}
			if( file == NULL )
			{
				rux::XString file_path;
				file_path.set_ByRef( filename.ConvertToAnsi() );
				if( file_mode == XReadText )
					file = rux::io::fopen( file_path.str() , "rt" );			
				else if( file_mode == XReadBinary )
					file = rux::io::fopen( file_path.str() , "rb" );			
				else if( file_mode == XCreateWriteText )
					file = rux::io::fopen( file_path.str() , "wt" );			
				else if( file_mode == XOpenWriteText )
					file = rux::io::fopen( file_path.str() , "r+t" );			
				else if( file_mode == XCreateWriteBinary )
					file = rux::io::fopen( file_path.str() , "wb" );			
				else if( file_mode == XOpenWriteBinary )
					file = rux::io::fopen( file_path.str() , "r+b" );
			}
			if( file == NULL )
				error = XError::ConvertErrorCodeToString( rux::io::get_errno() , XEnum_ErrorType_Errno );
			current_file_mode = file_mode;
		};
		void XFile::AddAllowAllAccess( const rux::XString& file_name , const rux::XString& user_or_group , rux::XString& error )
		{
			error.Clear();
			if( XFile::Exists( file_name ) == 1 )
			{
		#ifdef __WINDOWS__
				WELL_KNOWN_SID_TYPE well_known_sid_type = WinNullSid;
				rux::XString lower_user_or_group;
				lower_user_or_group.set_ByRef( user_or_group.ToLower() );
				if( lower_user_or_group.Equals( "network service" , 15 ) )
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
								rux::XString utf16_file_name;
								utf16_file_name.set_ByRef( file_name.ConvertToUTF16() );
								rux::uint32 error_uint32 = SetNamedSecurityInfoW( (wchar_t*)utf16_file_name.str() , SE_FILE_OBJECT , DACL_SECURITY_INFORMATION , sid_ptr , NULL ,  acl_ptr , NULL );
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
		#else
		(void)user_or_group;
		#endif
			}
			else
			{
				rux_write_error( "directory '" + file_name + "' does not exist" , error );
			}
		};
		void XFile::AddAllowExecuteAccess( const rux::XString& file_name , const rux::XString& user_or_group , rux::XString& error )
		{
			(void)user_or_group;
			error.Clear();
			if( XFile::Exists( file_name ) == 1 )
			{
		#ifdef __WINDOWS__
				WELL_KNOWN_SID_TYPE well_known_sid_type = WinNullSid;
				rux::XString lower_user_or_group;
				lower_user_or_group.set_ByRef( user_or_group.ToLower() );
				if( lower_user_or_group.Equals( "network service" , 15 ) )
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
							if( AddAccessAllowedAce( acl_ptr , ACL_REVISION , FILE_GENERIC_EXECUTE , sid_ptr ) )
							{
								rux::XString utf16_file_name;
								utf16_file_name.set_ByRef( file_name.ConvertToUTF16() );
								rux::uint32 error_uint32 = SetNamedSecurityInfoW( (wchar_t*)utf16_file_name.str() , SE_FILE_OBJECT , DACL_SECURITY_INFORMATION , sid_ptr , NULL ,  acl_ptr , NULL );
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
				rux_write_error( "directory '" + file_name + "' does not exist" , error );
			}
		};
		implement_duplicate_internal_result_function( rux::uint8 , File , Delete );
		implement_duplicate_internal_function( File , Close );
		implement_duplicate_internal_result_function( rux::uint8 , File , IsEOF );
		bool XFile::Equals( const rux::XString& file_path0 , const rux::XString& file_path1 , rux::XString& error )
		{
			bool equals = false;
			error.Clear();
			if( XFile::Exists( file_path0 ) == 1 )
			{
				if( XFile::Exists( file_path1 ) == 1 )
				{
					rux::io::XFile file0 = XFile::Open( file_path0 , error );
					if( error.Length() == 0 )
					{
						int64 file_size0 = file0.get_FileSize( error );
						if( error.Length() == 0 )
						{
							rux::io::XFile file1 = XFile::Open( file_path1 , error );
							if( error.Length() == 0 )
							{
								int64 file_size1 = file1.get_FileSize( error );
								if( error.Length() == 0 )
								{
									if( file_size0 == file_size1 )
									{
										declare_stack_variable( char , buffer_ptr0 , 16384 );
										declare_stack_variable( char , buffer_ptr1 , 16384 );
										size_t readen_size0 = 0 , readen_size1 = 0;
										equals = true;
										do
										{
											file0.ReadBytes( buffer_ptr0 , 16384 , readen_size0 , error );
											if( error.Length() == 0 )
											{
												file1.ReadBytes( buffer_ptr1 , 16384 , readen_size1 , error );
												if( error.Length() == 0 )
												{
													if( readen_size0 == readen_size1 )
													{
														if( readen_size0 > 0 )
														{
															if( memcmp( buffer_ptr0 , buffer_ptr1 , readen_size0 ) != 0 )
															{
																equals = false;
																break;
															}
														}
														else
															break;
													}
													else
													{
														equals = false;
														break;
													}
												}
												else
												{
													equals = false;
													break;
												}
											}
											else
											{
												equals = false;
												break;							
											}
										}
										while( file0.IsEOF() != 1
											&& file1.IsEOF() != 1 );
										if( equals == true )
										{
											if( file0.IsEOF() != 1 || file1.IsEOF() != 1 )
												equals = false;
										}
									}
								}
								file1.Close();				
							}
						}	
						file0.Close();				
					}
				}
				else
				{
					rux_write_error( "file '" + file_path1 + "' does not exists" , error );
				}
			}
			else
			{
				rux_write_error( "file '" + file_path0 + "' does not exists" , error );
			}
			return equals;
		};
	};
};
