#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_vsrcresources.h>
#include <xan_directory.h>
#include <xan_error.h>
#include <xan_zip.h>
#include <xan_path.h>
#include <xan_keyvaluepair.h>
namespace rux
{
	namespace resources
	{
		#ifdef __WINDOWS__
		BOOL XVSRCResources::EnumResName( HMODULE , const char* , char* name , LONG_PTR param )
		{
			XArray< XInt64 >* resources_ptr = (XArray< XInt64 >*)param;
			resources_ptr->Add( XInt64( rux_from_memory_address_to_int64( name ) ) );
			return true;
		};
		#endif
		rux::Array< XInt64 >& XVSRCResources::GetResources( void )
		{
			XArray< XInt64 > resources;
		#ifdef __WINDOWS__
			EnumResourceNamesA( GetModuleHandle( NULL ) , RT_RCDATA , EnumResName , (LONG_PTR)&resources );
		#endif
			return resources++;
		};
		rux::Array< XInt64 >& XVSRCResources::GetStringResources( void )
		{
			XArray< XInt64 > resources;
		#ifdef __WINDOWS__
			EnumResourceNamesA( GetModuleHandle( NULL ) , RT_STRING , EnumResName , (LONG_PTR)&resources );
		#endif
			return resources++;
		};
		void XVSRCResources::UnInstallFromResources( const XString& install_path , XArray< XString >& exclude_files , XString& error )
		{
		#ifdef __WINDOWS__
			declare_stack_variable( char , buffer , 64000 );
			XArray< XInt64 > resources;
			resources.set_ByRef( GetStringResources() );		
			XString current_cp_install_path , install_file_name;
			current_cp_install_path.set_ByRef( install_path.ConvertToAnsi() );
			current_cp_install_path.set_ByRef( rux::io::XDirectory::TransformToRuxDirectoryName( current_cp_install_path ) );	
			uint32 first_string = 0 , index1 = 0 , index2 = 0;
			for( uint32 index0 = 0 ; index0 < resources.Count() ; index0++ )
			{
				first_string = ( (uint32)resources[ index0 ]() - 1 ) *16;
				for( index1 = 0 ; index1 < 16 ; index1++ )
				{
					if( LoadStringA( GetModuleHandle( NULL ) , first_string + index1 , buffer , MAX_PATH ) > 0 )
					{
						install_file_name = buffer;			
						install_file_name.set_ByRef( install_file_name.ReplaceAll( '\\' , '/' ) );
						if( install_file_name.get_UTF8Char( 0 ) == '/' )
							install_file_name.RemoveAt( 0 );				
						for( index2 = 0 ; index2 < exclude_files.Count() ; index2++ )
						{
							if( install_file_name == exclude_files[ index2 ] )
								break;
						}
						install_file_name.set_ByRef( current_cp_install_path + "/" + install_file_name );
						if( index2 >= exclude_files.Count() )
						{
							if( rux::io::XFile::Exists( install_file_name ) == 1 )
								rux::io::XFile::Delete( install_file_name , error );
						}
					}
				}
			}
			XString directories_structure;
			uint32 resource_incrementer = 19860;
			while( LoadStringA( GetModuleHandle( NULL ) , (uint32)MAKEINTRESOURCEA( resource_incrementer ) , buffer , 64000 ) > 0 )
			{
				directories_structure += buffer;
				resource_incrementer++;
			}
			if( directories_structure.Length() == 0 )
				error = XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError );
			else
				rux::io::XDirectory::DeleteDirectoriesStructureFromJSON( install_path , directories_structure , error );
		#endif
		};
		void XVSRCResources::InstallFromResources( const XString& install_path , XString& error )
		{
			error.Clear();
		#ifdef __WINDOWS__
			declare_stack_variable( char , buffer , 64000 );
			HRSRC res_handle = NULL;
			HGLOBAL global_mem_ptr = NULL;
			void* mem_ptr = NULL;
			FILE* destination_file_ptr = NULL;
			XString current_cp_install_path = install_path.ConvertToAnsi() , install_file_name , directory;
			current_cp_install_path.set_ByRef( current_cp_install_path.ReplaceAll( '\\' , '/' ) );
			if( current_cp_install_path.get_UTF8Char( current_cp_install_path.Length() - 1 )  == '/' )
				current_cp_install_path.RemoveAt( current_cp_install_path.Length() - 1 );	
			uint8 is_zip = 0;
			uint32 size = 0;
			res_handle = FindResourceA( GetModuleHandle( NULL ) , MAKEINTRESOURCE( _rux_installer_zip ) , RT_RCDATA );
			if( res_handle )
			{
				global_mem_ptr = LoadResource( GetModuleHandle( NULL ) , res_handle );
				if( global_mem_ptr )
				{			
					mem_ptr = LockResource( global_mem_ptr );
					if( mem_ptr )
					{
						install_file_name = "installer.zip";
						directory = current_cp_install_path;				
						install_file_name.set_ByRef( current_cp_install_path + "/" + install_file_name );
						install_file_name.set_ByRef( install_file_name.ConvertToAnsi() );
						size = SizeofResource( 0 , res_handle );
						destination_file_ptr = fopen( install_file_name.str() , "w+b" );
						if( destination_file_ptr )
						{
							char* buffer_ptr = (char*)mem_ptr;
							int64 offset = 0 , written_size = 0;
							do
							{
								written_size = fwrite( &buffer_ptr[ offset ] , sizeof( char ) , size - offset , destination_file_ptr );
								if( written_size > 0 )
									offset += written_size;
								else
									break;
							}
							while( size > offset );
							if( size != offset )
							{
								rux_write_error( "cannot create file '" + install_file_name + "'" , error );						
							}
							else
								fflush( destination_file_ptr );
							fclose( destination_file_ptr );
						}
						UnlockResource( mem_ptr );
						if( error.Length() == 0 )
						{
							XZip::UnCompress( current_cp_install_path , install_file_name , error );
							if( error.Length() == 0 )
								is_zip = 1;
							if( rux::io::XFile::Exists( install_file_name ) == 1 )
								rux::io::XFile::Delete( install_file_name , error );
						}
					}
					else
						error = XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError );
				}
				else
					error = XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError );
			}
			if( error.Length() == 0 
				&& is_zip == 0 )
			{	
				XString directories_structure;
				uint32 resource_incrementer = 19860;
				while( LoadStringA( GetModuleHandle( NULL ) , (uint32)MAKEINTRESOURCEA( resource_incrementer ) , buffer , 64000 ) > 0 )
				{
					directories_structure += buffer;
					resource_incrementer++;
				}
				if( directories_structure.Length() == 0 )
					error = XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_GetLastError );
				else
					rux::io::XDirectory::CreateDirectoriesStructureFromJSON( install_path , directories_structure , error );
				if( error.Length() == 0 )
				{
					XArray< XInt64 > resources;
					resources.set_ByRef( GetResources() );		
					XArray< XString > dirs;
					uint32 index1 = 0;		
					for( uint32 index0 = 0 ; index0 < resources.Count() ; index0++ )
					{
						if( LoadStringA( GetModuleHandle( NULL ) , (uint32)resources[ index0 ]() , buffer , MAX_PATH ) > 0 )
						{			
							res_handle = FindResourceA( GetModuleHandle( NULL ) , (char*)resources[ index0 ]() , RT_RCDATA );
							if( res_handle )
							{
								global_mem_ptr = LoadResource( GetModuleHandle( NULL ) , res_handle );
								if( global_mem_ptr )
								{			
									mem_ptr = LockResource( global_mem_ptr );
									if( mem_ptr )
									{
										install_file_name = buffer;			
										install_file_name.set_ByRef( install_file_name.ReplaceAll( '\\' , '/' ) );
										if( install_file_name.get_UTF8Char( 0 ) == '/' )
											install_file_name.RemoveAt( 0 );
										dirs.set_ByRef( install_file_name.Split( "/" , EStringSplitOptions_RemoveEmptyEntries ) );
										directory = current_cp_install_path;
										for( index1 = 0 ; index1 < dirs.Count() - 1 ; index1++ )
										{
											directory += "/" + dirs[ index1 ];
											if( rux::io::XDirectory::Exists( directory ) == 0 )
												rux::io::XDirectory::Create( directory , error );
											if( error.Length() == 0 )
												break;
										}
										if( error.Length() == 0 )
										{
											UnlockResource( mem_ptr );
											break;
										}
										install_file_name.set_ByRef( current_cp_install_path + "/" + install_file_name );
										install_file_name.set_ByRef( install_file_name.ConvertToAnsi() );
										size = SizeofResource( 0 , res_handle );
										destination_file_ptr = fopen( install_file_name.str() , "w+b" );
										if( destination_file_ptr )
										{
											char* buffer_ptr = (char*)mem_ptr;
											int64 offset = 0 , written_size = 0;
											do
											{
												written_size = fwrite( &buffer_ptr[ offset ] , sizeof( char ) , size - offset , destination_file_ptr );
												if( written_size > 0 )
													offset += written_size;
												else
													break;
											}
											while( size > offset );
											if( size != offset )
											{
												rux_write_error( "cannot create file '" + install_file_name + "'" , error );						
												fclose( destination_file_ptr );
												UnlockResource( mem_ptr );
												break;
											}
											else
												fflush( destination_file_ptr );
											fclose( destination_file_ptr );
										}
										UnlockResource( mem_ptr );
									}
								}
							}
						}
					}
				}
			}
		#endif
		};
		uint8 XVSRCResources::ExtractInFile( char* file_name_ptr , int32 rc_id , rux::alloc_t alloc , char** error_ptr_ptr )
		{
			uint8 is_success = 0;
		#ifdef __WINDOWS__		
			if( file_name_ptr && XString::LengthA( file_name_ptr ) > 0 )
			{

				if( _access( file_name_ptr , 0 ) != -1 )
				{
					struct _stat st;
					if( _stat( file_name_ptr , &st ) == 0 )
					{
						if( ( st.st_mode & _S_IFDIR ) )
						{
							if( alloc )
							{
								if( error_ptr_ptr )
								{
									const char* temp_ptr = "directory '%s' already exists.Try to relocate this directory"; 
									uint32 size = XString::LengthA( file_name_ptr ) + XString::LengthA( temp_ptr ) - 1;
									*error_ptr_ptr = (char*)alloc( size );
									::rux::safe_sprintf( *error_ptr_ptr , size , temp_ptr , file_name_ptr );
								}
							}
						}
						else if( ( st.st_mode & _S_IFREG ) )
						{
							if( alloc )
							{
								if( error_ptr_ptr )
								{
									const char* temp_ptr = "file '%s' already exists.Try to relocate this file"; 
									uint32 size = XString::LengthA( file_name_ptr ) + XString::LengthA( temp_ptr ) - 1;
									*error_ptr_ptr = (char*)alloc( size );
									::rux::safe_sprintf( *error_ptr_ptr , size , temp_ptr , file_name_ptr );
								}
							}
						}
					}
				}
				else
				{
					HRSRC res_handle = FindResourceA( GetModuleHandle( NULL ) , MAKEINTRESOURCE( rc_id ) , RT_RCDATA );
					if( res_handle )
					{
						HGLOBAL global_mem_ptr = LoadResource( GetModuleHandle( NULL ) , res_handle );
						if( global_mem_ptr )
						{			
							void* mem_ptr = LockResource( global_mem_ptr );
							if( mem_ptr )
							{
								uint32 size = SizeofResource( 0 , res_handle );
								FILE* destination_file_ptr = fopen( file_name_ptr , "w+b" );
								if( destination_file_ptr )
								{
									char* buffer_ptr = (char*)mem_ptr;
									int64 offset = 0 , written_size = 0;
									do
									{
										written_size = fwrite( &buffer_ptr[ offset ] , sizeof( char ) , size - offset , destination_file_ptr );
										if( written_size > 0 )
											offset += written_size;
										else
											break;
									}
									while( size > offset );
									if( size == offset )
									{
										fflush( destination_file_ptr );
										is_success = 1;
									}
									fclose( destination_file_ptr );
								}
								UnlockResource( mem_ptr );
							}
						}
					}		
				}
			}
		#endif
			return is_success;
		};
		uint8 XVSRCResources::ExtractInFile( char* file_name_ptr , const XInt64& rc_id , rux::alloc_t alloc , char** error_ptr_ptr )
		{
			uint8 is_success = 0;
		#ifdef __WINDOWS__
			if( file_name_ptr && XString::LengthA( file_name_ptr ) > 0 )
			{
				if( _access( file_name_ptr , 0 ) != -1 )
				{
					struct _stat st;
					if( _stat( file_name_ptr , &st ) == 0 )
					{
						if( ( st.st_mode & _S_IFDIR ) )
						{
							if( alloc )
							{
								if( error_ptr_ptr )
								{
									const char* temp_ptr = "directory '%s' already exists.Try to relocate this directory"; 
									uint32 size = XString::LengthA( file_name_ptr ) + XString::LengthA( temp_ptr ) - 1;
									*error_ptr_ptr = (char*)alloc( size );
									::rux::safe_sprintf( *error_ptr_ptr , size , temp_ptr , file_name_ptr );
								}
							}
						}
						else if( ( st.st_mode & _S_IFREG ) )
						{
							if( alloc )
							{
								if( error_ptr_ptr )
								{
									const char* temp_ptr = "file '%s' already exists.Try to relocate this file"; 
									uint32 size = XString::LengthA( file_name_ptr ) + XString::LengthA( temp_ptr ) - 1;
									*error_ptr_ptr = (char*)alloc( size );
									sprintf( *error_ptr_ptr , temp_ptr , file_name_ptr );
								}
							}
						}
					}
				}
				else
				{
					HRSRC res_handle = FindResourceA( GetModuleHandle( NULL ) , (char*)rc_id() , RT_RCDATA );
					if( res_handle )
					{
						HGLOBAL global_mem_ptr = LoadResource( GetModuleHandle( NULL ) , res_handle );
						if( global_mem_ptr )
						{			
							void* mem_ptr = LockResource( global_mem_ptr );
							if( mem_ptr )
							{
								uint32 size = SizeofResource( 0 , res_handle );
								FILE* destination_file_ptr = fopen( file_name_ptr , "w+b" );
								if( destination_file_ptr )
								{
									char* buffer_ptr = (char*)mem_ptr;
									int64 offset = 0 , written_size = 0;
									do
									{
										written_size = fwrite( &buffer_ptr[ offset ] , sizeof( char ) , size - offset , destination_file_ptr );
										if( written_size > 0 )
											offset += written_size;
										else
											break;
									}
									while( size > offset );
									if( size == offset )
									{
										fflush( destination_file_ptr );
										is_success = 1;
									}
									fclose( destination_file_ptr );
								}
								UnlockResource( mem_ptr );
							}
						}
					}		
				}
			}
		#endif
			return is_success;
		};
	};
};