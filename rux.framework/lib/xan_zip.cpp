#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_zip.h>
#include <zip.h>
#include <unzip.h>
#include <xan_directory.h>
#include <xan_error.h>
#include <xan_keyvaluepair.h>
#include <xan_path.h>
#include <xan_int32.h>
void XZip::Compress( const rux::XArray< rux::XString >& files_or_directories , const rux::XString& zip_file_name , rux::uint8 abscence_is_not_error , rux::XString& error )
{
	error.Clear();
	if( rux::io::XFile::Exists( zip_file_name ) == 0 )
	{
		rux::XString current_cp_zip_file_name;
		current_cp_zip_file_name.set_ByRef( zip_file_name.ConvertToAnsi() );
		zipFile zip_file = zipOpen64( current_cp_zip_file_name.str() , APPEND_STATUS_CREATE );
		if( zip_file )
		{
			for( size_t files_or_directories_index = 0 ; files_or_directories_index < files_or_directories.Count() ; files_or_directories_index++ )
			{
				if( rux::io::XDirectory::Exists( files_or_directories[ files_or_directories_index ] ) == 1 )
				{
					rux::XString dir;
					dir.set_ByRef( rux::io::XPath::GetDirectoryName( files_or_directories[ files_or_directories_index ] ) );

					rux::XArray< rux::io::XFile > files;
					files.set_ByRef( rux::io::XDirectory::GetFiles( files_or_directories[ files_or_directories_index ] , XAllDirectories ) );
					rux::XArray< rux::io::XDirectory > directories;
					directories.set_ByRef( rux::io::XDirectory::GetDirectories( files_or_directories[ files_or_directories_index ] , XAllDirectories ) );							
					directories.Insert( 0 ,  files_or_directories[ files_or_directories_index ] );
					zip_fileinfo zip_file_info;
					memset( &zip_file_info , 0 , sizeof( zip_file_info ) );
					rux::XString file_name_in_zip;
					rux::uint8 is_zip64 = 0;					
					rux::int32 error_int32 = 0;
					rux::uint32 index0 = 0;
					for( ; index0 < directories.Count() ; index0++ )
					{					
						file_name_in_zip = directories[ index0 ].get_DirectoryName();
						file_name_in_zip.RemoveRange( 0 , dir.Length() + 1 );
						file_name_in_zip.set_ByRef( file_name_in_zip.ReplaceAll( '\\' , '/' ) );
						file_name_in_zip.set_ByRef( file_name_in_zip.ConvertToAnsi() );
						if( file_name_in_zip.get_UTF8Char( file_name_in_zip.Length() - 1 ) != '/' )
							file_name_in_zip += "/";
						memset( &zip_file_info , 0 , sizeof( zip_file_info ) );
						if( ( error_int32 = zipOpenNewFileInZip3_64( zip_file , file_name_in_zip.str() , &zip_file_info , NULL , 0 , NULL , 0 , NULL , Z_DEFLATED , Z_DEFAULT_COMPRESSION , 0 , -MAX_WBITS , DEF_MEM_LEVEL , Z_DEFAULT_STRATEGY , NULL , 0 , is_zip64 ) ) == ZIP_OK )
							zipCloseFileInZip( zip_file );
						else
						{
							rux_write_error( "error open new directory '" + directories[ index0 ].get_DirectoryName() + "' in zip archive '" + zip_file_name + "' , error " + rux::XInt32( error_int32 ).ToString() , error );
							break;
						}
					}
					if( error.Length() == 0 )
					{
						for( index0 = 0 ; index0 < files.Count() ; index0++ )
						{
							is_zip64 = files[ index0 ].get_FileSize( error ) > 0xffffffff;
							file_name_in_zip = files[ index0 ].get_FileName();
							file_name_in_zip.RemoveRange( 0 , dir.Length() + 1 );
							file_name_in_zip.set_ByRef( file_name_in_zip.ReplaceAll( '\\' , '/' ) );
							file_name_in_zip.set_ByRef( file_name_in_zip.ConvertToAnsi() );
							memset( &zip_file_info , 0 , sizeof( zip_file_info ) );
							if( ( error_int32 = zipOpenNewFileInZip3_64( zip_file , file_name_in_zip.str() , &zip_file_info , NULL , 0 , NULL , 0 , NULL , Z_DEFLATED , Z_DEFAULT_COMPRESSION , 0 , -MAX_WBITS , DEF_MEM_LEVEL , Z_DEFAULT_STRATEGY , NULL , 0 , is_zip64 ) ) == ZIP_OK )
							{
								do
								{
									declare_stack_variable( char , buffer_ptr , 16384 );
									size_t readen_size = 0;
									files[ index0 ].ReadBytes( buffer_ptr , 16384 , readen_size , error );
									if( readen_size > 0 )
									{
										error_int32 = zipWriteInFileInZip( zip_file , buffer_ptr , (rux::uint32)readen_size );
										if( error_int32 < 0 )
										{
											rux_write_error( "error in writing file '" + files[ index0 ].get_FileName() + "' in zip archive '" + zip_file_name + "' , error " + rux::XInt32( error_int32 ).ToString() , error );
											break;
										}
									}
									else
										break;
								}
								while( files[ index0 ].IsEOF() != 1 );
								if( files[ index0 ].Exists() == 1 )
									files[ index0 ].Close();
								zipCloseFileInZip( zip_file );
								if( error.Length() > 0 )
									break;
							}
							else
							{
								rux_write_error( "error open new file '" + files[ index0 ].get_FileName() + "' in zip archive '" + zip_file_name + "' , error " + rux::XInt32( error_int32 ).ToString() , error );
								break;
							}
						}
					}		
				}
				else if( rux::io::XFile::Exists( files_or_directories[ files_or_directories_index ] ) == 1 )
				{	
					rux::io::XFile file = rux::io::XFile::Open( files_or_directories[ files_or_directories_index ] , error );
					if( error.Length() == 0 )
					{			
						zip_fileinfo zip_file_info;						
						rux::XString file_name_in_zip;
						rux::uint8 is_zip64 = 0;						
						rux::int32 error_int32 = 0;
						is_zip64 = file.get_FileSize( error ) > 0xffffffff;
						file_name_in_zip = file.get_FileName();
						file_name_in_zip.set_ByRef( rux::io::XPath::GetFileName( file_name_in_zip ) );
						file_name_in_zip.set_ByRef( file_name_in_zip.ReplaceAll( '\\' , '/' ) );
						file_name_in_zip.set_ByRef( file_name_in_zip.ConvertToAnsi() );
						memset( &zip_file_info , 0 , sizeof( zip_file_info ) );
						if( ( error_int32 = zipOpenNewFileInZip3_64( zip_file , file_name_in_zip.str() , &zip_file_info , NULL , 0 , NULL , 0 , NULL , Z_DEFLATED , Z_DEFAULT_COMPRESSION , 0 , -MAX_WBITS , DEF_MEM_LEVEL , Z_DEFAULT_STRATEGY , NULL , 0 , is_zip64 ) ) == ZIP_OK )
						{
							do
							{
								declare_stack_variable( char , buffer_ptr , 16384 );
								size_t readen_size = 0;
								file.ReadBytes( buffer_ptr , 16384 , readen_size , error );
								if( readen_size > 0 )
								{
									error_int32 = zipWriteInFileInZip( zip_file , buffer_ptr , (rux::uint32)readen_size );
									if( error_int32 < 0 )
									{
										rux_write_error( "error in writing file '" + file.get_FileName() + "' in zip archive '" + zip_file_name + "' , error " + rux::XInt32( error_int32 ).ToString() , error );
										break;
									}
								}
								else
									break;
							}
							while( file.IsEOF() != 1 );								
							zipCloseFileInZip( zip_file );
						}
						else
						{
							rux_write_error( "error open new file '" + file.get_FileName() + "' in zip archive '" + zip_file_name + "' , error " + rux::XInt32( error_int32 ).ToString() , error );
						}								
						file.Close();
					}
				}
				else
				{
					if( abscence_is_not_error == 0 )
						rux_write_error( "file or directory '" + files_or_directories[ files_or_directories_index ] + "' does not exist" , error );
				}				
				if( error.Length() > 0 )
					break;
			}
			if( error.Length() > 0 
				&& rux::io::XFile::Exists( zip_file_name ) == 1 )
			{
				rux::XString error0;
				rux::io::XFile::Delete( zip_file_name , error0 );
			}
			zipClose( zip_file , NULL );				
		}
		else
		{
			rux_write_error( "could not create zip file '" + zip_file_name + "'" , error );
		}
	}
	else
	{
		rux_write_error( "zip file '" + zip_file_name + "' already exists" , error );
	}
};
void XZip::Compress( const rux::XString& file_or_directory_name , const rux::XString& zip_file_name , rux::XString& error )
{
	error.Clear();
	if( rux::io::XFile::Exists( zip_file_name ) == 0 )
	{
		if( rux::io::XDirectory::Exists( file_or_directory_name ) == 1 )
		{
			rux::XArray< rux::io::XFile > files;
			files.set_ByRef( rux::io::XDirectory::GetFiles( file_or_directory_name , XAllDirectories ) );
			rux::XArray< rux::io::XDirectory > directories;
			directories.set_ByRef( rux::io::XDirectory::GetDirectories( file_or_directory_name , XAllDirectories ) );
			rux::XString current_cp_zip_file_name;
			current_cp_zip_file_name.set_ByRef( zip_file_name.ConvertToAnsi() );
			zipFile zip_file = zipOpen64( current_cp_zip_file_name.str() , APPEND_STATUS_CREATE );
			if( zip_file )
			{				
				zip_fileinfo zip_file_info;
				memset( &zip_file_info , 0 , sizeof( zip_file_info ) );
				rux::XString file_name_in_zip;
				rux::uint8 is_zip64 = 0;				
				rux::int32 error_int32 = 0;
				rux::uint32 index0 = 0;
				for( ; index0 < directories.Count() ; index0++ )
				{					
					file_name_in_zip = directories[ index0 ].get_DirectoryName();
					file_name_in_zip.RemoveRange( 0 , file_or_directory_name.Length() + 1 );
					file_name_in_zip.set_ByRef( file_name_in_zip.ReplaceAll( '\\' , '/' ) );
					file_name_in_zip.set_ByRef( file_name_in_zip.ConvertToAnsi() );
					if( file_name_in_zip.get_UTF8Char( file_name_in_zip.Length() - 1 ) != '/' )
						file_name_in_zip += "/";
					if( ( error_int32 = zipOpenNewFileInZip3_64( zip_file , file_name_in_zip.str() , &zip_file_info , NULL , 0 , NULL , 0 , NULL , Z_DEFLATED , Z_DEFAULT_COMPRESSION , 0 , -MAX_WBITS , DEF_MEM_LEVEL , Z_DEFAULT_STRATEGY , NULL , 0 , is_zip64 ) ) == ZIP_OK )
					{						
						zipCloseFileInZip( zip_file );						
					}
					else
					{
						rux_write_error( "error open new directory '" + directories[ index0 ].get_DirectoryName() + "' in zip archive '" + zip_file_name + "' , error " + rux::XInt32( error_int32 ).ToString() , error );
						break;
					}
				}
				if( error.Length() == 0 )
				{
					for( index0 = 0 ; index0 < files.Count() ; index0++ )
					{
						is_zip64 = files[ index0 ].get_FileSize( error ) > 0xffffffff;
						file_name_in_zip = files[ index0 ].get_FileName();
						file_name_in_zip.RemoveRange( 0 , file_or_directory_name.Length() + 1 );
						file_name_in_zip.set_ByRef( file_name_in_zip.ReplaceAll( '\\' , '/' ) );
						file_name_in_zip.set_ByRef( file_name_in_zip.ConvertToAnsi() );
						if( ( error_int32 = zipOpenNewFileInZip3_64( zip_file , file_name_in_zip.str() , &zip_file_info , NULL , 0 , NULL , 0 , NULL , Z_DEFLATED , Z_DEFAULT_COMPRESSION , 0 , -MAX_WBITS , DEF_MEM_LEVEL , Z_DEFAULT_STRATEGY , NULL , 0 , is_zip64 ) ) == ZIP_OK )
						{
							do
							{
								declare_stack_variable( char , buffer_ptr , 16384 );
								size_t readen_size = 0;
								files[ index0 ].ReadBytes( buffer_ptr , 16384 , readen_size , error );
								if( readen_size > 0 )
								{
									error_int32 = zipWriteInFileInZip( zip_file , buffer_ptr , (rux::uint32)readen_size );
									if( error_int32 < 0 )
									{
										rux_write_error( "error in writing file '" + files[ index0 ].get_FileName() + "' in zip archive '" + zip_file_name + "' , error " + rux::XInt32( error_int32 ).ToString() , error );
										break;
									}
								}
								else
									break;
							}
							while( files[ index0 ].IsEOF() != 1 );
							if( files[ index0 ].Exists() == 1 )
								files[ index0 ].Close();
							zipCloseFileInZip( zip_file );
							if( error.Length() > 0 )
								break;
						}
						else
						{
							rux_write_error( "error open new file '" + files[ index0 ].get_FileName() + "' in zip archive '" + zip_file_name + "' , error " + rux::XInt32( error_int32 ).ToString() , error );
							break;
						}
					}
				}
				zipClose( zip_file , NULL );
			}
			else
			{
				rux_write_error( "could not create zip file '" + zip_file_name + "'" , error );
			}
		}
		else if( rux::io::XFile::Exists( file_or_directory_name ) == 1 )
		{	
			rux::io::XFile file = rux::io::XFile::Open( file_or_directory_name , error );
			if( error.Length() == 0 )
			{
				rux::XString current_cp_zip_file_name;
				current_cp_zip_file_name.set_ByRef( zip_file_name.ConvertToAnsi() );
				zipFile zip_file = zipOpen64( current_cp_zip_file_name.str() , APPEND_STATUS_CREATE );
				if( zip_file )
				{				
					zip_fileinfo zip_file_info;
					memset( &zip_file_info , 0 , sizeof( zip_file_info ) );
					rux::XString file_name_in_zip;
					rux::uint8 is_zip64 = 0;					
					rux::int32 error_int32 = 0;
					is_zip64 = file.get_FileSize( error ) > 0xffffffff;
					file_name_in_zip = file.get_FileName();
					file_name_in_zip.set_ByRef( rux::io::XPath::GetFileName( file_name_in_zip ) );
					file_name_in_zip.set_ByRef( file_name_in_zip.ReplaceAll( '\\' , '/' ) );
					file_name_in_zip.set_ByRef( file_name_in_zip.ConvertToAnsi() );
					if( ( error_int32 = zipOpenNewFileInZip3_64( zip_file , file_name_in_zip.str() , &zip_file_info , NULL , 0 , NULL , 0 , NULL , Z_DEFLATED , Z_DEFAULT_COMPRESSION , 0 , -MAX_WBITS , DEF_MEM_LEVEL , Z_DEFAULT_STRATEGY , NULL , 0 , is_zip64 ) ) == ZIP_OK )
					{
						do
						{
							declare_stack_variable( char , buffer_ptr , 16384 );
							size_t readen_size = 0;
							file.ReadBytes( buffer_ptr , 16384 , readen_size , error );
							if( readen_size > 0 )
							{
								error_int32 = zipWriteInFileInZip( zip_file , buffer_ptr , (rux::uint32)readen_size );
								if( error_int32 < 0 )
								{
									rux_write_error( "error in writing file '" + file.get_FileName() + "' in zip archive '" + zip_file_name + "' , error " + rux::XInt32( error_int32 ).ToString() , error );
									break;
								}
							}
							else
								break;
						}
						while( file.IsEOF() != 1 );								
						zipCloseFileInZip( zip_file );
					}
					else
					{
						rux_write_error( "error open new file '" + file.get_FileName() + "' in zip archive '" + zip_file_name + "' , error " + rux::XInt32( error_int32 ).ToString() , error );
					}
					zipClose( zip_file , NULL );
				}
				else
				{
					rux_write_error( "could not create zip file '" + zip_file_name + "'" , error );
				}
				file.Close();
			}
		}
		else
		{
			rux_write_error( "file or directory '" + file_or_directory_name + "' does not exist" , error );
		}
		if( error.Length() > 0 
			&& rux::io::XFile::Exists( zip_file_name ) == 1 )
		{
			rux::XString error0;
			rux::io::XFile::Delete( zip_file_name , error0 );
		}
	}
	else
	{
		rux_write_error( "zip file '" + zip_file_name + "' already exists" , error );
	}
};
void XZip::UnCompress(  const rux::XString& destination_directory_name , const rux::XString& zip_file_name , rux::XString& error )
{
	error.Clear();
	if( rux::io::XDirectory::Exists( destination_directory_name ) == 1)
	{
		if( rux::io::XFile::Exists( zip_file_name ) == 1 )
		{
			rux::XString current_cp_zip_file_name , current_cp_destination_directory_name , destination_file_name;
			current_cp_destination_directory_name.set_ByRef( destination_directory_name.ConvertToAnsi() );
			current_cp_destination_directory_name.set_ByRef( current_cp_destination_directory_name.ReplaceAll( '\\' , '/' ) );
			if( current_cp_destination_directory_name.get_UTF8Char( current_cp_destination_directory_name.Length() - 1 ) == '/' )
				current_cp_destination_directory_name.RemoveAt( current_cp_destination_directory_name.Length() - 1 );
			current_cp_zip_file_name.set_ByRef( zip_file_name.ConvertToAnsi() );
			unzFile unz_file = unzOpen( current_cp_zip_file_name.str() );
			if ( unz_file != NULL )
			{
				rux::int32 error_int32 = UNZ_OK;
				unz_global_info global_info;
				if ( ( error_int32 = unzGetGlobalInfo( unz_file, &global_info ) ) != UNZ_OK )
				{
					rux_write_error( "could not read file '" + zip_file_name + "' global info , error " + rux::XInt32( error_int32 ).ToString() , error );					
				}
				else
				{
					rux::uint32 read_size = 4096;
					declare_stack_variable( char , read_buffer , 4096 );
					unz_file_info file_info;
					declare_stack_variable( char , filename , 1024 );
					rux::uint32 index0 = 0;
					rux::XString filename_string , create_all_directory;
					rux::io::XFile destination_file;
					rux::XArray< rux::XString > splits;
					for ( ; index0 < global_info.number_entry ; ++index0 )
					{
						if( ( error_int32 = unzGetCurrentFileInfo( unz_file , &file_info , filename , 1024 , NULL , 0 , NULL , 0 ) ) != UNZ_OK )
						{
							rux_write_error( "could not read file info , error " + rux::XInt32( error_int32 ).ToString() , error );							
							break;
						}
						else
						{
							filename_string = filename;
							filename_string.set_ByRef( filename_string.ReplaceAll( '\\' , '/' ) );
							destination_file_name.set_ByRef( current_cp_destination_directory_name + "/" + filename_string );
							if( destination_file_name.get_UTF8Char( destination_file_name.Length() - 1 ) == '/' )
							{
								if( rux::io::XDirectory::Exists( destination_file_name ) == 0 )
								{
									rux::io::XDirectory::Create( destination_file_name , error );
									if( error.Length() > 0 )
									{
										rux_write_error( "could not create directory '" + filename_string + "' from zip archive '" + zip_file_name + "'" , error );									
										break;
									}
								}
							}
							else
							{							
								if( ( error_int32 = unzOpenCurrentFile( unz_file ) ) != UNZ_OK )
								{
									rux_write_error( "could not open file '" + filename_string + "' from zip archive '" + zip_file_name + "' , error " + rux::XInt32( error_int32 ).ToString() , error );									
									break;
								}
								else
								{									
									splits.set_ByRef( destination_file_name.Split( '/' ) );
									if( splits.Count() > 1 )
									{
										splits.RemoveAt( splits.Count() - 1 );
										create_all_directory.set_ByRef( rux::XString::Join( "/" , splits ) );
										rux::io::XDirectory::CreateAll( create_all_directory , error );
										if( error.Length() > 0 )
										{
											rux_write_error( "could not create directory '" + filename_string + "' from zip archive '" + zip_file_name + "'" , error );
											break;
										}
									}
									destination_file = rux::io::XFile::Create( destination_file_name , error );
									if( destination_file.Exists() == 0 )
									{
										rux_write_error( "could not open destination file '" + destination_file_name + "'" , error );
										unzCloseCurrentFile( unz_file );										
										break;
									}
									else
									{
										error_int32 = UNZ_OK;
										do    
										{
											error_int32 = unzReadCurrentFile( unz_file , read_buffer , read_size );
											if( error_int32 < 0 )
											{
												rux_write_error(  "could not read file '" + filename_string + "' from zip archive '" + zip_file_name + "' , error " + rux::XInt32( error_int32 ).ToString() , error );
												unzCloseCurrentFile( unz_file );												
												break;
											}
											if( error_int32 > 0 )
											{
												destination_file.WriteBytes( read_buffer , error_int32 , error );
												if( error.Length() > 0 )
													break;
											}
										} 
										while( error_int32 > 0 );
									}
									if( error.Length() > 0 )
										break;
								}
								unzCloseCurrentFile( unz_file );
							}
							if( ( index0 + 1 ) < global_info.number_entry )
							{
								if( ( error_int32 = unzGoToNextFile( unz_file ) ) != UNZ_OK )
								{
									rux_write_error( "cound not read next file from zip archive '" + zip_file_name + "' , error " + rux::XInt32( error_int32 ).ToString() , error );									
									break;
								}
							}
						}
					}					
				}
				unzClose( unz_file );
			}
		}
		else
		{
			rux_write_error( "zip file '" + zip_file_name + "' does not exist" , error );
		}
	}
	else
	{
		rux_write_error( "destination directory '" + destination_directory_name + "' does not exist" , error );
	}
};