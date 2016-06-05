#include <xan_file.h>
#include <xan_console.h>
#include <xan_zip.h>
#include <xan_directory.h>
const char* g_current_module_name = "rux.io.test.exe";
dll_internal ::rux::uint8 _rux_is_memory_manager = 0;
namespace rux
{
	dll_internal rux_simple_t _first_function = NULL;
	dll_internal rux_simple_t _last_function = NULL;
	dll_internal rux_simple_t _pre_initialization = NULL;
	dll_internal rux_simple_t _post_deinitialization = NULL;
};
rux::uint32 _test_index = 0;
rux::uint32 _succeeded_tests = 0;
rux::uint32 _failed_tests = 0;
int main(int argc, char ** args)
{	
	{
		THREAD_REGISTER( "main" );
		if( argc == 2 && strcmp( args[ 1 ] , "--file-write" ) == 0 )
		{
			rux::XString error;
			declare_stack_variable( char , filename , 1024 );
			rux_get_executable_directory( filename );
#ifdef __WINDOWS__
			rux_concatenation( filename , "/../../addons/json_test.txt" );
#else
			rux_concatenation( filename , "/../addons/json_test.txt" );
#endif			
			rux::io::XFile read_file( rux::io::XFile::Open( filename , error ) );	
			if( read_file.Exists() == 1 )
			{
				rux::int64 file_size = read_file.get_FileSize( error );
				if( error.Length() == 0 )
				{
					size_t readen_size = 0;
					char* buffer = read_file.ReadBytes( file_size , readen_size , error );
					if( error.Length() == 0 )
					{
						rux::uint32 test_count = 0;
						while( test_count < 10 )
						{							

							declare_stack_variable( char , write_filename , 1024 );
							rux_get_executable_directory( write_filename );
							rux_concatenation( write_filename , "/write_test" );



							//{
							//	CODE_LABEL( NULL , NULL , 100 );
							//	DWORD flags = FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH;
							//	HANDLE handle = CreateFileA( write_filename , GENERIC_WRITE , 0 , 0 , CREATE_NEW , flags , 0 );
							//	if( handle != INVALID_HANDLE_VALUE )
							//	{
							//		LARGE_INTEGER position;								
							//		position.QuadPart = readen_size;
							//		SetFilePointerEx( handle , position , 0 , FILE_BEGIN );
							//		SetEndOfFile( handle );
							//		SetFilePointer( handle , 0 , 0 , FILE_BEGIN );
							//		DWORD written_size = 0;
							//		
							//		size_t to_write = readen_size - readen_size % 512;
							//		WriteFile( handle , buffer , to_write , &written_size , NULL );
							//		
							//		/*char* ptr = buffer;
							//		do
							//		{
							//			if( WriteFile( handle , ptr , 128 * 1024 , &written_size , NULL ) )
							//				ptr += written_size;
							//			else
							//				break;
							//		}
							//		while( ptr - buffer != to_write );*/
							//		
							//		
							//		CloseHandle( handle );
							//	}
							//	//_aligned_malloc( 
							//}
							//DeleteFileA( write_filename );

							//{
							//	CODE_LABEL( NULL , NULL , 100 );
							//	FILE* file = rux::engine::fopen( write_filename , "wb" );
							//	if( file )
							//	{	
							//		fseek( file , readen_size - 1 , SEEK_SET );
							//		rux::engine::fwrite( "\0" , 1 , 1 , file );
							//		fseek( file , 0 , SEEK_SET );
							//		fflush( file );
							//		rux::engine::fwrite( buffer , sizeof( char ) , readen_size , file );
							//		//char* ptr = buffer;
							//		//size_t written_size = 0 , size_to_write = 0;
							//		//do
							//		//{
							//		//	size_to_write = readen_size - ( ptr - buffer );

							//		//	//printf( "size = %u\n" , size_to_write );

							//		//	if( size_to_write > 2 * 1024 * 1024 )
							//		//		written_size = rux::engine::fwrite( ptr , sizeof( char ) , 2 * 1024 * 1024 , file );
							//		//	else
							//		//		written_size = rux::engine::fwrite( ptr , sizeof( char ) , size_to_write , file );
							//		//	if( written_size > 0 )
							//		//		ptr += written_size;
							//		//	else
							//		//		break;
							//		//}
							//		//while( ptr - buffer != readen_size );

							//		rux::engine::fclose( file );
							//	}
							//}
							//rux::io::XFile::Delete( write_filename , error );			
							
							{
								CODE_LABEL( NULL , NULL , 100 );
								rux::io::XFile write_file( rux::io::XFile::Create( write_filename , error , readen_size , 1 ) );
								if( write_file.Exists() == 1 )
								{
									write_file.WriteBytes( buffer , readen_size , error );
									write_file.Close();								
								}
							}
							rux::io::XFile::Delete( write_filename , error );			


							test_count++;
						}
					}
					if( buffer )
						rux::engine::free_mem( buffer );
					//_aligned_free( buffer );
				}
				read_file.Close();
			}
		}
		else if( argc == 2 && strcmp( args[ 1 ] , "--zip-test" ) == 0 )
		{
			declare_stack_variable( char , filename , 1024 );
			declare_stack_variable( char , new_filename , 1024 );
			declare_stack_variable( char , zip_filename , 1024 );
			rux_get_executable_directory( filename );
			rux_get_executable_directory( new_filename );
			rux_get_executable_directory( zip_filename );
#ifdef __WINDOWS__
			rux_concatenation( filename , "/../../build/zip_test.txt" );
			rux_concatenation( new_filename , "/../../build/new_zip_test" );
			rux_concatenation( zip_filename , "/../../build/zip_test.zip" );
#else
			rux_concatenation( filename , "/../build/zip_test.txt" );
			rux_concatenation( new_filename , "/../build/new_zip_test" );
			rux_concatenation( zip_filename , "/../build/zip_test.zip" );
#endif	
			rux::XString error;
			rux::io::XFile test_file = rux::io::XFile::Create( filename , error );
			if( error.Length() == 0 )
			{
				test_file.AppendText( "It's all for our test!!!\n" , error );
				test_file.Close();
				if( error.Length() == 0 )
				{
					XZip::Compress( filename , zip_filename , error );
					if( error.Length() == 0 )
					{
						rux::io::XDirectory::Create( new_filename );
						XZip::UnCompress( new_filename , zip_filename , error );
						if( error.Length() == 0 )
						{
							declare_stack_variable( char , filename1 , 1024 );
							rux_concatenation( filename1 , new_filename , "/zip_test.txt" );
							if( rux::io::XFile::Equals( filename , filename1 , error ) == false )
								error = "error";
						}
						rux::io::XDirectory::Delete( new_filename , XAllDirectories );
						rux::XString error0;
						rux::io::XFile::Delete( zip_filename , error0 );
					}
				}
				rux::XString error0;
				rux::io::XFile::Delete( filename , error0 );
			}
			if( error.Length() > 0 )
				_failed_tests++;
			else
				_succeeded_tests++;

			declare_stack_variable( char , build_dir , 1024 );
			rux_get_executable_directory( build_dir );
#ifdef __WINDOWS__
			rux_concatenation( build_dir , "/../../docs" );
#else
			rux_concatenation( build_dir , "/../docs" );
#endif	
			XZip::Compress( build_dir , zip_filename , error );
			if( error.Length() == 0 )
			{
				rux::XString error0;
				rux::io::XFile::Delete( zip_filename , error0 );
			}
			if( error.Length() > 0 )
				_failed_tests++;
			else
				_succeeded_tests++;
		}
		printf( "\nSucceeded tests : %u\nFailed tests : %u\n\n" , _succeeded_tests , _failed_tests );
		printf( "Press <ENTER>" );
		rux::XConsole::WaitForEnter();
	}		
	rux::deinitialize();
	return 0;
};
