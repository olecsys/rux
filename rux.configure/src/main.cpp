#include <xan_jsonserializer.h>
#include <xan_console.h>
#include <xan_file.h>
#include <xan_directory.h>
#include <xan_module.h>
#include <xan_makefile_generator.h>
#include <xan_installer_generator.h>
#include <xan_time.h>
#include <xan_path.h>
#include <xan_codegenerator.h>
#include <xan_error.h>
#ifdef __WINDOWS__
#include <xan_registry.h>
#endif
rux::int32 start( rux::XArray< rux::XString >& names , rux::XArray< rux::XString >& values )
{
	rux::XString error;
	rux::int32 result = 1;
	if( names[ 0 ] == "--register-file-association" )
	{
#ifdef __WINDOWS__
		char executable[ 1024 ] = {0};
		rux_get_executable_filename( executable );
		::rux::XString executable_string( executable );
		executable_string += " --generate-makefile %1";
		::rux::XRegistry registry;
		
		::rux::XArray< ::rux::XString > key_names;
		key_names.Add( "rux.configure" );
		key_names.Add( "shell" );
		key_names.Add( "open" );
		key_names.Add( "command" );
		registry.CreateRegKey( XEnum_PredefinedRegKey_HKEY_CLASSES_ROOT , key_names , error );

		registry.set_RegValueAsString( XEnum_PredefinedRegKey_HKEY_CLASSES_ROOT , "rux.configure" , "" , "RUX Configure" , error );		
		registry.set_RegValueAsString( XEnum_PredefinedRegKey_HKEY_CLASSES_ROOT , key_names , "" , executable_string , error );		

		registry.CreateRegKey( XEnum_PredefinedRegKey_HKEY_CLASSES_ROOT , ".RUXPRJ" , error );
		registry.set_RegValueAsString( XEnum_PredefinedRegKey_HKEY_CLASSES_ROOT , ".RUXPRJ" , "" , "rux.configure" , error );				

		registry.CreateRegKey( XEnum_PredefinedRegKey_HKEY_CLASSES_ROOT , ".ruxprj" , error );
		registry.set_RegValueAsString( XEnum_PredefinedRegKey_HKEY_CLASSES_ROOT , ".ruxprj" , "" , "rux.configure" , error );				
#endif
	}
	else if( names.Count() >= 1 && values.Count() >= 1 )
	{		
		rux::uint8 silent = 0;
		if( names[ 0 ] == "--rebuild-project" || names[ 0 ] == "--build-project" )
		{
			::rux::XString configuration_name;
			if( names.Count() > 1 && names[ 1 ] == "--configuration" )
				configuration_name.set_ByRef( values[ 1 ] );

			char temp_working_directory[ 1024 ] = {0};
			rux_get_working_directory( temp_working_directory , 1024 );
			rux::XString working_directory( temp_working_directory );
			rux::io::XFile ruxprj;
			rux::XString ruxprj_string;
			ruxprj_string.set_ByRef( values[ 0 ] );
			ruxprj_string.set_ByRef( ruxprj_string.ReplaceAll( '\\' , '/' ) );
			ruxprj_string.set_ByRef( ruxprj_string.ReplaceAll( "//" , "/" ) );
			if( ruxprj_string.Length() > 0 
				&& ruxprj_string.get_UTF8Char( 0 ) == '.' )
				ruxprj_string.set_ByRef( working_directory + "/" + ruxprj_string );
			rux::generator::XRUXProject project( rux::generator::XRUXProject::load( ruxprj_string , error ) );
			if( error.Length() == 0 )
			{
				::rux::XArray< ::rux::generator::XNDKMakefileProject > ndk_makefiles_project;
				ndk_makefiles_project.set_ByRef( project.get_NDKMakefileProjects() );
				for( size_t index0 = 0 ; index0 < ndk_makefiles_project.Count() ; index0++ )
				{
					if( configuration_name.Length() == 0 
						|| ndk_makefiles_project[ index0 ].get_ConfigurationName() == configuration_name )
					{
						::rux::XString android_mk , application_mk , default_properties , jni_dir;
						jni_dir.set_ByRef( ndk_makefiles_project[ index0 ].get_JNIDirectory() );
						ndk_makefiles_project[ index0 ].generate_jni( android_mk , application_mk , default_properties );
												
						::rux::io::XDirectory::CreateAll( jni_dir , error );
						if( error.Length() == 0 )
						{
							{//Android.mk								
								::rux::XString android_mk_filename;
								android_mk_filename.set_ByRef( jni_dir + "/Android.mk"  );
								if( rux::io::XFile::Exists( android_mk_filename ) == 1 )
									rux::io::XFile::Delete( android_mk_filename , error );
								rux::io::XFile file = rux::io::XFile::Create( android_mk_filename , error );
								if( file.Exists() == 1 )
								{
									android_mk.set_ByRef( android_mk.ConvertToUTF8() );
									if( android_mk.Size() > 1 )
										file.WriteBytes( android_mk.str() , android_mk.Size() - 1 , error );							
									file.Close();
								}
							}
							if( error.Length() == 0 )
							{//Application.mk
								::rux::XString application_mk_filename;
								application_mk_filename.set_ByRef( jni_dir + "/Application.mk"  );
								if( rux::io::XFile::Exists( application_mk_filename ) == 1 )
									rux::io::XFile::Delete( application_mk_filename , error );
								rux::io::XFile file = rux::io::XFile::Create( application_mk_filename , error );
								if( file.Exists() == 1 )
								{
									application_mk.set_ByRef( application_mk.ConvertToUTF8() );
									if( application_mk.Size() > 1 )
										file.WriteBytes( application_mk.str() , application_mk.Size() - 1 , error );							
									file.Close();
								}
							}
							if( error.Length() == 0 )
							{//default.properties
								::rux::XString default_properties_filename;
								default_properties_filename.set_ByRef( ::rux::io::XPath::GetDirectoryName( jni_dir ) + "/default.properties" );
								if( rux::io::XFile::Exists( default_properties_filename ) == 1 )
									rux::io::XFile::Delete( default_properties_filename , error );
								rux::io::XFile file = rux::io::XFile::Create( default_properties_filename , error );
								if( file.Exists() == 1 )
								{
									default_properties.set_ByRef( default_properties.ConvertToUTF8() );
									if( default_properties.Size() > 1 )
										file.WriteBytes( default_properties.str() , default_properties.Size() - 1 , error );							
									file.Close();
								}
							}
							if( error.Length() == 0 )
								ndk_makefiles_project[ index0 ].rebuild( error );
						}
						if( error.Length() > 0 )
							break;
					}
				}
				if( error.Length() == 0 )
				{
					::rux::XArray< ::rux::generator::XCPPMakefileProject > cpp_makefiles_project;
					cpp_makefiles_project.set_ByRef( project.get_CPPMakefileProjects() );
					for( size_t index0 = 0 ; index0 < cpp_makefiles_project.Count() ; index0++ )
					{
						if( configuration_name.Length() == 0 
							|| cpp_makefiles_project[ index0 ].get_ConfigurationName() == configuration_name )
						{
							::rux::XString makefile_text , makefile , makefile_dir;
							makefile.set_ByRef( cpp_makefiles_project[ index0 ].get_MakefileFilename() );
							makefile_text.set_ByRef( cpp_makefiles_project[ index0 ].generate_makefile() );

							makefile_dir.set_ByRef( ::rux::io::XPath::GetDirectoryName( makefile ) );
							::rux::io::XDirectory::CreateAll( makefile_dir , error );
							if( error.Length() == 0 )
							{
								if( rux::io::XFile::Exists( makefile ) == 1 )
									rux::io::XFile::Delete( makefile , error );
								rux::io::XFile file = rux::io::XFile::Create( makefile , error );
								if( file.Exists() == 1 )
								{
									makefile_text.set_ByRef( makefile_text.ConvertToUTF8() );
									if( makefile_text.Size() > 1 )
										file.WriteBytes( makefile_text.str() , makefile_text.Size() - 1 , error );							
									file.Close();
									if( names[ 0 ] == "--rebuild-project" )
										cpp_makefiles_project[ index0 ].rebuild( error );
									else if( names[ 0 ] == "--build-project" )
										cpp_makefiles_project[ index0 ].build( error );
								}
							}
							if( error.Length() > 0 )
								break;
						}
					}		
				}
			}
		}
		else if( names[ 0 ] == "--generate-makefile" )
		{
			char temp_working_directory[ 1024 ] = {0};
			rux_get_working_directory( temp_working_directory , 1024 );
			rux::XString working_directory( temp_working_directory );
			rux::io::XFile ruxprj;
			rux::XString ruxprj_string;
			ruxprj_string.set_ByRef( values[ 0 ] );
			ruxprj_string.set_ByRef( ruxprj_string.ReplaceAll( '\\' , '/' ) );
			ruxprj_string.set_ByRef( ruxprj_string.ReplaceAll( "//" , "/" ) );
			if( ruxprj_string.Length() > 0 
				&& ruxprj_string.get_UTF8Char( 0 ) == '.' )
				ruxprj_string.set_ByRef( working_directory + "/" + ruxprj_string );
			rux::generator::XRUXProject project( rux::generator::XRUXProject::load( ruxprj_string , error ) );
			if( error.Length() == 0 )
			{
				::rux::XArray< ::rux::generator::XCPPMakefileProject > cpp_makefiles_project;
				cpp_makefiles_project.set_ByRef( project.get_CPPMakefileProjects() );
				for( size_t index0 = 0 ; index0 < cpp_makefiles_project.Count() ; index0++ )
				{
					::rux::XString makefile_text , makefile , makefile_dir;
					makefile.set_ByRef( cpp_makefiles_project[ index0 ].get_MakefileFilename() );
					makefile_text.set_ByRef( cpp_makefiles_project[ index0 ].generate_makefile() );

					makefile_dir.set_ByRef( ::rux::io::XPath::GetDirectoryName( makefile ) );
					::rux::io::XDirectory::CreateAll( makefile_dir , error );
					if( error.Length() == 0 )
					{
						if( rux::io::XFile::Exists( makefile ) == 1 )
							rux::io::XFile::Delete( makefile , error );
						rux::io::XFile file = rux::io::XFile::Create( makefile , error );
						if( file.Exists() == 1 )
						{
							makefile_text.set_ByRef( makefile_text.ConvertToUTF8() );
							if( makefile_text.Size() > 1 )
								file.WriteBytes( makefile_text.str() , makefile_text.Size() - 1 , error );							
							file.Close();
						}
					}
					if( error.Length() > 0 )
						break;
				}
			}
		}
		else
			RUX_ERROR( error , "unknown input command" );
	}
	else
		RUX_ERROR( error , "incorrect input parameters" );
	if( error.Length() == 0 )
		result = 0;
	else
		::rux::XConsole::Print( stderr , XRed , error );
	return result;
};
rux_simple_console_initialize( "rux.configure" , start );