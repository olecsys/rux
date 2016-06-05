#include <xan_console.h>
#include <xan_file.h>
#include <xan_directory.h>
#include <xan_rsa.h>
#include <xan_path.h>
rux::int32 start( rux::XArray< rux::XString >& names , rux::XArray< rux::XString >& values )
{	
	rux::int32 result = 1;
	if( names.Count() >= 1 && values.Count() >= 1 )
	{		
		rux::uint8 silent = 0;
		if( names[ 0 ] == "--command" )
		{
			if( values[ 0 ] == "sign" )
			{
				char temp_working_directory[ 1024 ] = {0};
				rux_get_working_directory( temp_working_directory , 1024 );
				rux::XString working_directory( temp_working_directory );
				rux::XString in_filename , out_filename , pem_filename , silent_string;
				for( size_t index0 = 1 ; index0 < names.Count() ; index0++ )
				{
					if( index0 >= values.Count() )
						break;
					else
					{
						if( names[ index0 ] == "--pem-file" )
						{
							pem_filename = values[ index0 ];
							if( pem_filename.Length() > 0 
								&& pem_filename.get_UTF8Char( 0 ) == '.' )
								pem_filename.set_ByRef( working_directory + "/" + pem_filename );
							pem_filename.set_ByRef( rux::io::XPath::ExcludeRelative( pem_filename ) );
						}
						else if( names[ index0 ] == "--in-file" )
						{
							in_filename = values[ index0 ];
							if( in_filename.Length() > 0 
								&& in_filename.get_UTF8Char( 0 ) == '.' )
								in_filename.set_ByRef( working_directory + "/" + in_filename );
							in_filename.set_ByRef( rux::io::XPath::ExcludeRelative( in_filename ) );
						}
						else if( names[ index0 ] == "--out-file" )
						{
							out_filename = values[ index0 ];
							if( out_filename.Length() > 0 
								&& out_filename.get_UTF8Char( 0 ) == '.' )
								out_filename.set_ByRef( working_directory + "/" + out_filename );
							out_filename.set_ByRef( rux::io::XPath::ExcludeRelative( out_filename ) );
						}
						else if( names[ index0 ] == "--silent" )
						{
							silent_string = values[ index0 ];
							if( silent_string.Equals( "true" , 4 ) )
								silent = 1;
						}
					}
				}
				if( rux::io::XFile::Exists( pem_filename ) == 1 )
				{
					if( rux::io::XFile::Exists( in_filename ) == 1 )
					{
						if( rux::io::XFile::Exists( out_filename ) == 0 )
						{							
							rux::io::XFile in_file;
							rux::XString error;
							in_file = rux::io::XFile::Open( in_filename , error );
							if( in_file.Exists() == 1 )
							{
								size_t readen_size = 0;
								char* in_bytes = in_file.ReadBytes( in_file.get_FileSize( error ) , readen_size , error );
								in_file.Close();
								if( error.Length() == 0 )
								{
									rux::cryptography::XRSA rsa;
									rux::cryptography::XRSA::FromPEMFile( rsa , pem_filename , error );
									if( error.Length() == 0 )
									{
										rux::uint8* out_bytes = NULL;
										rux::uint32 out_bytes_size = 0 , out_bytes_len = 0;
										rsa.Sign( (rux::uint8*)in_bytes , readen_size , out_bytes , out_bytes_size , out_bytes_len , error );
										if( error.Length() == 0 )
										{
											rux::io::XDirectory::CreateAll( rux::io::XPath::GetDirectoryName( out_filename ) , error );
											rux::io::XFile out_file = rux::io::XFile::Create( out_filename , error );
											if( out_file.Exists() == 1 )
											{
												out_file.WriteBytes( (char*)out_bytes , out_bytes_len , error );
												if( error.Length() == 0 )
													result = 0;
												out_file.Close();
											}
										}
										if( out_bytes )
											rux::engine::free_mem( out_bytes );
									}
								}
								if( in_bytes )
									rux::engine::free_mem( in_bytes );
							}
						}
						else
							rux::XConsole::Print( stderr , XRed , "out file '%s' already exists" , out_filename.str() );
					}
					else
						rux::XConsole::Print( stderr , XRed , "in file '%s' does not exist" , in_filename.str() );
				}
				else
					rux::XConsole::Print( stderr , XRed , "PEM file '%s' does not exist" , pem_filename.str() );				
			}
			if( silent == 0 )
			{
				rux::XConsole::Print( stdout , XWhite , "Press <ENTER>" );
				rux::XConsole console;
				console.Read();
			}
		}
	}
	return result;
};
rux_simple_console_initialize( "rux.crypto" , start );