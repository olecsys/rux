#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_makefile_generator.h>
#include <xan_path.h>
#include <xan_time.h>
#include <xan_uint64.h>
#include <xan_jsonserializer.h>
#include <xan_regexp.h>
#include <xan_directory.h>
#include <xan_error.h>
begin_implement_rux_class_ns( CPPMakefileProject , rux::generator );
end_implement_rux_class();

begin_implement_rux_class_ns( NDKMakefileProject , rux::generator );
end_implement_rux_class();
namespace rux
{
	namespace generator
	{		
		void replace_specific_value( const ::rux::XString& field_name , const XObject& object_field_value , ::rux::data::XDataObject& configuration , ::rux::XArray< ::rux::XString >& replaces_name , const ::rux::XString& main_directory , const ::rux::XString& relative_dir , ::rux::XArray< ::rux::XString >& processed_fields , ::rux::XString& error )
		{
			::rux::uint8 already_processed = 0;
			for( size_t index0 = 0 ; index0 < processed_fields.Count() ; index0++ )
			{
				if( processed_fields[ index0 ] == field_name )
				{
					already_processed = 1;
					break;
				}
			}
			if( already_processed == 0 ) 
			{
				if( rux_is_object( object_field_value , ::rux::XString ) )
				{
					::rux::XString field_value;
					field_value.set_ByRef( object_field_value );
					for( size_t index0 = 0 ; index0 < replaces_name.Count() ; index0++ )
					{
						if( field_value.IndexOf( replaces_name[ index0 ] ) > -1 )
						{				
							::rux::uint8 found = 0;
							::rux::XString replace , replace_field_name;
							replace_field_name.set_ByRef( replaces_name[ index0 ].Substring( 2 , replaces_name[ index0 ].Length() - 3 ) );
							if( replace_field_name != field_name )
							{
								replace.set_ByRef( configuration.GetValue< ::rux::XString >( replace_field_name , found , SIZE_MAX , 0 ) );
								if( found )
								{
									replace_specific_value( replaces_name[ index0 ].Substring( 2 , replaces_name[ index0 ].Length() - 3 ) , replace , configuration , replaces_name , main_directory , relative_dir , processed_fields , error );

									::rux::XString temp = replace;
									if( field_name.Equals( "intermediate_directory" , 22 )
										|| field_name.Equals( "output_directory" , 16 )
										|| field_name.Equals( "output_file" , 11 )
										|| field_name.Equals( "source_directory" , 16 )
										|| field_name.Equals( "ndk" , 3 )
										|| field_name.Equals( "sdk" , 3 )
										|| field_name.Equals( "ant" , 3 ) )
									{
										if( replace_field_name.Equals( "intermediate_directory" , 22 )
											|| replace_field_name.Equals( "output_directory" , 16 )
											|| replace_field_name.Equals( "output_file" , 11 )
											|| replace_field_name.Equals( "source_directory" , 16 )
											|| replace_field_name.Equals( "ndk" , 3 )
											|| replace_field_name.Equals( "sdk" , 3 )
											|| replace_field_name.Equals( "ant" , 3 ) )
										{												
											if( ::rux::io::XPath::IsAbsolutePath( temp ) == 0 )
												temp.set_ByRef( main_directory + "/" + temp );
											temp.set_ByRef( ::rux::io::XPath::CalculateRelative( relative_dir , temp ) );
										}
									}
									field_value = field_value.ReplaceAll( replaces_name[ index0 ] , temp );
									configuration.SetValue( field_name , field_value , 0 );
								}
							}
						}
					}
					if( field_name.Equals( "intermediate_directory" , 22 )
						|| field_name.Equals( "output_directory" , 16 )
						|| field_name.Equals( "output_file" , 11 )
						|| field_name.Equals( "source_directory" , 16 )
						|| field_name.Equals( "ndk" , 3 )
						|| field_name.Equals( "sdk" , 3 )
						|| field_name.Equals( "ant" , 3 ) )
					{
						if( ::rux::io::XPath::IsAbsolutePath( field_value ) == 0 )
							field_value = relative_dir + "/" + field_value;
						field_value = ::rux::io::XPath::CalculateRelative( main_directory , field_value );
						configuration.SetValue( field_name , field_value , 0 );
					}
				}
				else if( rux_is_object( object_field_value , ::rux::XArray< XObject > ) )
				{
					::rux::XArray< XObject > array_field_value;
					array_field_value.set_ByRef( object_field_value );
					for( size_t index1 = 0 ; index1 < array_field_value.Count() ; index1++ )
					{
						if( rux_is_object( array_field_value[ index1 ] , ::rux::XString ) )
						{
							::rux::XString field_value;
							field_value.set_ByRef( array_field_value[ index1 ] );
							for( size_t index0 = 0 ; index0 < replaces_name.Count() ; index0++ )
							{
								if( field_value.IndexOf( replaces_name[ index0 ] ) > -1 )
								{				
									::rux::uint8 found = 0;
									::rux::XString replace , replace_field_name;
									replace_field_name.set_ByRef( replaces_name[ index0 ].Substring( 2 , replaces_name[ index0 ].Length() - 3 ) );
									if( replace_field_name != field_name )
									{
										replace.set_ByRef( configuration.GetValue< ::rux::XString >( replace_field_name , found , SIZE_MAX , 0 ) );
										if( found )
										{
											replace_specific_value( replaces_name[ index0 ].Substring( 2 , replaces_name[ index0 ].Length() - 3 ) , replace , configuration , replaces_name , main_directory , relative_dir , processed_fields , error );
											::rux::XString temp = replace;
											if( field_name.Equals( "lib_paths" , 9 )
												|| field_name.Equals( "include_paths" , 13 )
												|| field_name.Equals( "sources" , 7 ) )
											{
												if( replace_field_name.Equals( "intermediate_directory" , 22 )
													|| replace_field_name.Equals( "output_directory" , 16 )
													|| replace_field_name.Equals( "output_file" , 11 )
													|| replace_field_name.Equals( "source_directory" , 16 )
													|| replace_field_name.Equals( "ndk" , 3 )
													|| replace_field_name.Equals( "sdk" , 3 )
													|| replace_field_name.Equals( "ant" , 3 ) )
												{												
													if( ::rux::io::XPath::IsAbsolutePath( temp ) == 0 )
														temp.set_ByRef( main_directory + "/" + temp );
													temp.set_ByRef( ::rux::io::XPath::CalculateRelative( relative_dir , temp ) );
												}
											}
											field_value.set_ByRef( field_value.ReplaceAll( replaces_name[ index0 ] , temp ) );
											array_field_value.set_ItemByRef( index1 , field_value );
										}
									}
								}
							}
							if( field_name.Equals( "lib_paths" , 9 )
								|| field_name.Equals( "include_paths" , 13 )
								|| field_name.Equals( "sources" , 7 ) )
							{
								if( ::rux::io::XPath::IsAbsolutePath( field_value ) == 0 )
									field_value.set_ByRef( relative_dir + "/" + field_value );
								field_value.set_ByRef( ::rux::io::XPath::CalculateRelative( main_directory , field_value ) );
								array_field_value.set_ItemByRef( index1 , field_value );
							}			
						}
					}
				}
				processed_fields.Add( field_name );
			}
		};
		void replace_specific_value( ::rux::data::XDataObject& configuration , ::rux::XArray< ::rux::XString >& replaces_name , const ::rux::XString& main_directory , const ::rux::XString& relative_dir , ::rux::XArray< ::rux::XString >& processed_fields , ::rux::XString& error )
		{			
			for( size_t index0 = 0 ; index0 < configuration.FieldsCount() ; index0++ )
				replace_specific_value( configuration.get_FieldName( index0 ) , XObject( configuration.get_FieldValue( index0 ) ) , configuration , replaces_name , main_directory , relative_dir , processed_fields , error );
		};
		implement_duplicate_internal_result_function( ::rux::String& , NDKMakefileProject , get_JNIDirectory );
		::rux::String& NDKMakefileProject::get_JNIDirectory( void )
		{
			return _jni_directory++;
		};
		implement_duplicate_internal_result_function( ::rux::String& , CPPMakefileProject , get_MakefileFilename );
		::rux::String& CPPMakefileProject::get_MakefileFilename( void )
		{
			return _makefile++;
		};
		NDKMakefileProject& XNDKMakefileProject::load( const ::rux::XString configuration_name , const ::rux::XString& js , const ::rux::XString& relative_dir , ::rux::XString& error )
		{
			error.Clear();
			::rux::generator::XNDKMakefileProject ndk_makefile_project;
			XObject obj( ::rux::data::XJsonSerializer::Deserialize( js , error ) );
			if( error.Length() == 0 )
			{
				if( rux_is_object( obj , ::rux::data::XDataObject ) )
				{
					::rux::data::XDataObject configuration( obj );

					::rux::XArray< ::rux::XString > replaces_name;
					::rux::XRegexpPattern regexp_pattern( ::rux::XRegexp::get_RegexpPattern( "\\$\\(.*)" , error ) );
					replaces_name.set_ByRef( ::rux::XRegexp::FindAll( js , regexp_pattern ) );
					for( size_t index0 = 0 ; index0 < replaces_name.Count() ; index0++ )
					{
						for( size_t index1 = index0 + 1 ; index1 < replaces_name.Count() ; index1++ )
						{
							if( replaces_name[ index0 ] == replaces_name[ index1 ] )
							{
								replaces_name.RemoveAt( index1 );
								index1--;
							}
						}
					}
					::rux::XString jni_directory;
					::rux::uint8 found = 0;
					jni_directory.set_ByRef( configuration.GetValue< ::rux::XString >( "jni_directory" , found , 13 , 0 ) );
					if( found )
					{
						if( ::rux::io::XPath::IsAbsolutePath( jni_directory ) == 0 )
							jni_directory.set_ByRef( relative_dir + "/" + jni_directory );
						jni_directory.set_ByRef( ::rux::io::XPath::ExcludeRelative( jni_directory ) );
						jni_directory += "/jni";
						::rux::XArray< ::rux::XString > processed_fields;
						replace_specific_value( "jni_directory" , jni_directory , configuration , replaces_name , jni_directory , relative_dir , processed_fields , error );
						
/* relative by makefile dir begin*/
						::rux::XString source_directory;
						source_directory.set_ByRef( configuration.GetValue< ::rux::XString >( "source_directory" , found , 16 , 0 ) );
						if( found )
						{
							if( ::rux::io::XPath::IsAbsolutePath( source_directory ) == 0 )
								source_directory.set_ByRef( relative_dir + "/" + source_directory );						
						}
						replace_specific_value( configuration , replaces_name  , jni_directory , relative_dir , processed_fields , error );
/* relative by makefile dir end*/
						::rux::XString output_file_type;
						output_file_type.set_ByRef( configuration.GetValue< ::rux::XString >( "output_file_type" , found , 16 , 0 ) );
						if( output_file_type == "static library"
								|| output_file_type == "dynamic library" 
								|| output_file_type == "console" )
						{											
							if( output_file_type == "static library" )
								ndk_makefile_project()->_output_file_type_enum = XEnum_OutputFile_StaticLibrary;
							else if( output_file_type == "dynamic library" )
								ndk_makefile_project()->_output_file_type_enum = XEnum_OutputFile_DynamicLibrary;
							else if( output_file_type == "console" )
								ndk_makefile_project()->_output_file_type_enum = XEnum_OutputFile_Console;	

							ndk_makefile_project()->_sources.set_ByRef( configuration.GetValue< ::rux::XArray< ::rux::XString > >( "sources" , found , 7 , 0 ) );
														
							if( source_directory.Length() > 0 )
							{
								::rux::XArray< ::rux::io::XFile > source_files;
								source_files.set_ByRef( ::rux::io::XDirectory::GetFiles( source_directory , XAllDirectories ) );
								for( size_t index0 = 0 ; index0 < source_files.Count() ; index0++ )
									ndk_makefile_project()->_sources.AddByRef( ::rux::io::XPath::CalculateRelative( jni_directory , source_files[ index0 ].get_FileName() ) );
							}
										
							::rux::XArray< ::rux::XString > exclude_files;
							exclude_files.set_ByRef( configuration.GetValue< ::rux::XArray< ::rux::XString > >( "exclude_files" , found , 13 , 0 ) );
							for( size_t index0 = 0 ; index0 < exclude_files.Count() ; index0++ )
							{
								for( size_t index1 = 0 ; index1 < ndk_makefile_project()->_sources.Count() ; index1++ )
								{
									if( ::rux::io::XPath::GetFileName( ndk_makefile_project()->_sources[ index1 ] ) == exclude_files[ index0 ] )
									{
										ndk_makefile_project()->_sources.RemoveAt( index1 );
										index1--;
									}
								}
							}
							ndk_makefile_project()->_jni_directory.set_ByRef( configuration.GetValue< ::rux::XString >( "jni_directory" , found , 13 , 0 ) );
							ndk_makefile_project()->_configuration_name = configuration_name;
											
							ndk_makefile_project()->_platform.set_ByRef( configuration.GetValue< ::rux::XInt64 >( "platform" , found , 8 , 0 ) );
							ndk_makefile_project()->_ndk.set_ByRef( configuration.GetValue< ::rux::XString >( "ndk" , found , 3 , 0 ) );
							ndk_makefile_project()->_sdk.set_ByRef( configuration.GetValue< ::rux::XString >( "sdk" , found , 3 , 0 ) );
							ndk_makefile_project()->_ant.set_ByRef( configuration.GetValue< ::rux::XString >( "ant" , found , 3 , 0 ) );
							ndk_makefile_project()->_output_directory.set_ByRef( configuration.GetValue< ::rux::XString >( "output_directory" , found , 16 , 0 ) );
							ndk_makefile_project()->_debug.set_ByRef( configuration.GetValue< ::rux::XBoolean >( "debug" , found , 5 , 0 ) );
							ndk_makefile_project()->_rtti.set_ByRef( configuration.GetValue< ::rux::XBoolean >( "rtti" , found , 4 , 0 ) );
							ndk_makefile_project()->_exceptions.set_ByRef( configuration.GetValue< ::rux::XBoolean >( "exceptions" , found , 10 , 0 ) );
							ndk_makefile_project()->_gnustl_static.set_ByRef( configuration.GetValue< ::rux::XBoolean >( "gnustl_static" , found , 13 , 0 ) );
							ndk_makefile_project()->_compiler_flags.set_ByRef( configuration.GetValue< ::rux::XString >( "compiler_flags" , found , 14 , 0 ) );
							ndk_makefile_project()->_local_module.set_ByRef( configuration.GetValue< ::rux::XString >( "local_module" , found , 12 , 0 ) );
							ndk_makefile_project()->_include_paths.set_ByRef( configuration.GetValue< ::rux::XArray< ::rux::XString > >( "include_paths" , found , 13 , 0 ) );
							ndk_makefile_project()->_lib_paths.set_ByRef( configuration.GetValue< ::rux::XArray< ::rux::XString > >( "lib_paths" , found , 9 , 0 ) );
							ndk_makefile_project()->_libs.set_ByRef( configuration.GetValue< ::rux::XArray< ::rux::XString > >( "libs" , found , 4 , 0 ) );
							ndk_makefile_project()->_prebuilt_static_libs.set_ByRef( configuration.GetValue< ::rux::XArray< ::rux::XString > >( "prebuilt_static_libs" , found , 20 , 0 ) );
							ndk_makefile_project()->_preprocessors.set_ByRef( configuration.GetValue< ::rux::XArray< ::rux::XString > >( "preprocessors" , found , 13 , 0 ) );
							ndk_makefile_project()->_architectures.set_ByRef( configuration.GetValue< ::rux::XArray< ::rux::XString > >( "architectures" , found , 13 , 0 ) );
							ndk_makefile_project()->_post_builds.set_ByRef( configuration.GetValue< ::rux::XArray< ::rux::XString > >( "post_builds" , found , 11 , 0 ) );
						}
						else
							RUX_ERROR( error , "unknown output file type" );
					}
					else
						RUX_ERROR( error , "field 'jni_directory' is not found" );
				}
				else
					RUX_ERROR( error , "configuration is not XDataObject" );
			}
			return ndk_makefile_project++;
		};
		implement_duplicate_internal_function_3( NDKMakefileProject , generate_jni , ::rux::XString& , ::rux::XString& , ::rux::XString& );
		void NDKMakefileProject::generate_jni( ::rux::XString& android_mk , ::rux::XString& application_mk , ::rux::XString& default_properties )
		{
			android_mk.Clear();

			android_mk += "LOCAL_PATH := $(call my-dir)\n";
			android_mk += "include $(CLEAR_VARS)\n\n";

			if( _prebuilt_static_libs.Count() > 0 )
			{
				for( size_t index0 = 0 ; index0 < _prebuilt_static_libs.Count() ; index0++ )
				{
					::rux::XString static_lib;
					for( size_t index1 = 0 ; index1 < _lib_paths.Count() ; index1++ )
					{
						static_lib.set_ByRef( _lib_paths[ index1 ] + "/lib" + _prebuilt_static_libs[ index0 ] + ".a" );
						if( ::rux::io::XFile::Exists( static_lib ) == 1 )
							break;
					}
					android_mk += "LOCAL_MODULE := " + _prebuilt_static_libs[ index0 ] + "\n";
					android_mk += "LOCAL_SRC_FILES := " + static_lib + "\n";
					android_mk += "include $(PREBUILT_STATIC_LIBRARY)\n\n";
					android_mk += "include $(CLEAR_VARS)\n\n";
				}			
			}

			android_mk += "LOCAL_MODULE := " + _local_module + "\n";

			android_mk += "LOCAL_SRC_FILES =";
			for( size_t index0 = 0 ; index0 < _sources.Count() ; index0++ )
			{
				if( ::rux::io::XPath::GetFileNameExtension( _sources[ index0 ] ) == "cpp" )
				{
					::rux::XString source( _sources[ index0 ] );
					source.RemoveAt( 0 );
					source.InsertRange( 0 , ::rux::XString( "$(PoInT)" ) );
					android_mk += " " + source;
				}
			}
			android_mk += "\n\n";

			if( _include_paths.Count() > 0 )
			{
				android_mk += "LOCAL_C_INCLUDES = \\\n";
				for( size_t index0 = 0 ; index0 < _include_paths.Count() ; index0++ )
					android_mk += "\t\t\t\t\t" + _include_paths[ index0 ] + "\\\n";
				android_mk += "\n";
			}
			if( _output_file_type_enum != ::rux::generator::XEnum_OutputFile_StaticLibrary )
			{
				if( _libs.Count() > 0 )
				{
					android_mk += "LOCAL_LDLIBS :=";
					for( size_t index0 = 0 ; index0 < _libs.Count() ; index0++ )
						android_mk += " -l" + _libs[ index0 ];
					android_mk += "\n\n";
				}
			}
			if( _rtti == true || _exceptions == true )
			{
				android_mk += "LOCAL_CPP_FEATURES :=";
				if( _rtti == true )
					android_mk += " rtti";
				if( _exceptions == true )
					android_mk += " exceptions";
				android_mk += "\n\n";
			}
			if( _compiler_flags.Length() > 0 )
			{
				android_mk += "LOCAL_CPPFLAGS := " + _compiler_flags;
				android_mk += "\n\n";
			}
			if( _prebuilt_static_libs.Count() > 0 )
			{
				android_mk += "LOCAL_STATIC_LIBRARIES :=";
				for( size_t index0 = 0 ; index0 < _prebuilt_static_libs.Count() ; index0++ )
					android_mk += " " + _prebuilt_static_libs[ index0 ];
				if( _output_file_type_enum == ::rux::generator::XEnum_OutputFile_Console )
					android_mk += " android_native_app_glue";
				android_mk += "\n\n";
			}
			if( _output_file_type_enum == ::rux::generator::XEnum_OutputFile_Console
				|| _output_file_type_enum == ::rux::generator::XEnum_OutputFile_DynamicLibrary )
				android_mk += "include $(BUILD_SHARED_LIBRARY)\n\n";
			else if( _output_file_type_enum == ::rux::generator::XEnum_OutputFile_StaticLibrary )
				android_mk += "include $(BUILD_STATIC_LIBRARY)\n\n";
			if( _output_file_type_enum == ::rux::generator::XEnum_OutputFile_Console )
				android_mk += "$(call import-module,android/native_app_glue)";
			if( _post_builds.Count() > 0 )
			{
				android_mk += "all: $(TARGET_ARCH_ABI)_post_build\n\n";
				android_mk += "define gen_post_build\n";
				android_mk += "$(TARGET_ARCH_ABI)_post_build:\n";
				for( size_t index0 = 0 ; index0 < _post_builds.Count() ; index0++ )
					android_mk += "\t" + _post_builds[ index0 ] + "\n";
				android_mk += "endef\n\n";
				android_mk += "$(eval $(call gen_post_build,$(TARGET_ARCH_ABI)))\n";
			}
			android_mk.set_ByRef( android_mk.ReplaceAll( "\"./" , "\"$(LOCAL_PATH)/" ) );
			android_mk.set_ByRef( android_mk.ReplaceAll( " ./" , " $(LOCAL_PATH)/" ) );
			android_mk.set_ByRef( android_mk.ReplaceAll( "\t./" , "\t$(LOCAL_PATH)/" ) );
			android_mk.set_ByRef( android_mk.ReplaceAll( "$(PoInT)" , "." ) );			

			application_mk.Clear();
			if( _debug == true )
				application_mk += "APP_OPTIM := debug\n";
			else
				application_mk += "APP_OPTIM := release\n";
			application_mk += "APP_PLATFORM := android-" + _platform.ToString() + "\n";
			if( _gnustl_static == true )
				application_mk += "APP_STL := gnustl_static\n";
			if( _architectures.Count() > 0 )
			{
				application_mk += "APP_ABI :=";
				for( size_t index0 = 0 ; index0 < _architectures.Count() ; index0++ )
					application_mk += " " + _architectures[ index0 ];
				application_mk += "\n";
			}
			application_mk += "APP_MODULES := " + _local_module + "\n";

			default_properties.Clear();
			default_properties += "target=android-" + _platform.ToString() + "\n";
		};
		CPPMakefileProject& XCPPMakefileProject::load( const ::rux::XString configuration_name , const ::rux::XString& js , const ::rux::XString& relative_dir , ::rux::XString& error )
		{
			error.Clear();
			::rux::generator::XCPPMakefileProject cpp_makefile_project;
			XObject obj( ::rux::data::XJsonSerializer::Deserialize( js , error ) );
			if( error.Length() == 0 )
			{
				if( rux_is_object( obj , ::rux::data::XDataObject ) )
				{
					::rux::data::XDataObject configuration( obj );

					::rux::XArray< ::rux::XString > replaces_name;
					::rux::XRegexpPattern regexp_pattern( ::rux::XRegexp::get_RegexpPattern( "\\$\\(.*)" , error ) );
					replaces_name.set_ByRef( ::rux::XRegexp::FindAll( js , regexp_pattern ) );
					for( size_t index0 = 0 ; index0 < replaces_name.Count() ; index0++ )
					{
						for( size_t index1 = index0 + 1 ; index1 < replaces_name.Count() ; index1++ )
						{
							if( replaces_name[ index0 ] == replaces_name[ index1 ] )
							{
								replaces_name.RemoveAt( index1 );
								index1--;
							}
						}
					}
					::rux::XString makefile , makefile_dir;
					::rux::uint8 found = 0;
					makefile.set_ByRef( configuration.GetValue< ::rux::XString >( "makefile" , found , 8 , 0 ) );
					if( found )
					{
						if( ::rux::io::XPath::IsAbsolutePath( makefile ) == 0 )
							makefile.set_ByRef( relative_dir + "/" + makefile );
						makefile.set_ByRef( ::rux::io::XPath::ExcludeRelative( makefile ) );
						makefile_dir.set_ByRef( ::rux::io::XPath::GetDirectoryName( makefile ) );
						::rux::XArray< ::rux::XString > processed_fields;
						replace_specific_value( "makefile" , makefile , configuration , replaces_name , makefile_dir , relative_dir , processed_fields , error );
						makefile_dir.set_ByRef( ::rux::io::XPath::GetDirectoryName( makefile ) );
/* relative by makefile dir begin*/
						::rux::XString source_dir;
						source_dir.set_ByRef( configuration.GetValue< ::rux::XString >( "source_directory" , found , 16 , 0 ) );
						if( found )
						{
							if( ::rux::io::XPath::IsAbsolutePath( source_dir ) == 0 )
								source_dir.set_ByRef( relative_dir + "/" + source_dir );
						}
						replace_specific_value( configuration , replaces_name , makefile_dir , relative_dir , processed_fields , error );						
/* relative by makefile dir end*/						
						if( error.Length() == 0 )
						{
							::rux::XString output_file_type;
							output_file_type.set_ByRef( configuration.GetValue< ::rux::XString >( "output_file_type" , found , 16 , 0 ) );
							if( output_file_type == "static library" 
									|| output_file_type == "dynamic library" 
									|| output_file_type == "console" )
							{											
								if( output_file_type == "static library" )
									cpp_makefile_project()->_output_file_type_enum = XEnum_OutputFile_StaticLibrary;
								else if( output_file_type == "dynamic library" )
									cpp_makefile_project()->_output_file_type_enum = XEnum_OutputFile_DynamicLibrary;
								else if( output_file_type == "console" )
									cpp_makefile_project()->_output_file_type_enum = XEnum_OutputFile_Console;	

								cpp_makefile_project()->_sources_files.set_ByRef( configuration.GetValue< ::rux::XArray< ::rux::XString > >( "sources" , found , 7 , 0 ) );
								
								cpp_makefile_project()->_source_dirs.Add( ::rux::io::XPath::CalculateRelative( makefile_dir , source_dir ) );
																
								cpp_makefile_project()->_exclude_files.set_ByRef( configuration.GetValue< ::rux::XArray< ::rux::XString > >( "exclude_files" , found , 13 , 0 ) );
								
								cpp_makefile_project()->_makefile.set_ByRef( configuration.GetValue< ::rux::XString >( "makefile" , found , 8 , 0 ) );
								cpp_makefile_project()->_configuration_name = configuration_name;
								cpp_makefile_project()->_compiler.set_ByRef( configuration.GetValue< ::rux::XString >( "compiler" , found , 8 , 0 ) );
								cpp_makefile_project()->_compiler_flags.set_ByRef( configuration.GetValue< ::rux::XString >( "compiler_flags" , found , 14 , 0 ) );
								cpp_makefile_project()->_isystem.set_ByRef( configuration.GetValue< ::rux::XString >( "isystem" , found , 7 , 0 ) );
								cpp_makefile_project()->_linker.set_ByRef( configuration.GetValue< ::rux::XString >( "linker" , found , 6 , 0 ) );
								cpp_makefile_project()->_linker_flags.set_ByRef( configuration.GetValue< ::rux::XString >( "linker_flags" , found , 12 , 0 ) );
								cpp_makefile_project()->_archiver.set_ByRef( configuration.GetValue< ::rux::XString >( "archiver" , found , 8 , 0 ) );
								cpp_makefile_project()->_archiver_flags.set_ByRef( configuration.GetValue< ::rux::XString >( "archiver_flags" , found , 14 , 0 ) );
								cpp_makefile_project()->_asm_compiler.set_ByRef( configuration.GetValue< ::rux::XString >( "asm_compiler" , found , 12 , 0 ) );
								cpp_makefile_project()->_asm_compiler_flags.set_ByRef( configuration.GetValue< ::rux::XString >( "asm_compiler_flags" , found , 18 , 0 ) );

								cpp_makefile_project()->_post_builds.set_ByRef( configuration.GetValue< ::rux::XArray< ::rux::XString > >( "post_builds" , found , 11 , 0 ) );
								cpp_makefile_project()->_pre_builds.set_ByRef( configuration.GetValue< ::rux::XArray< ::rux::XString > >( "pre_builds" , found , 10 , 0 ) );
								cpp_makefile_project()->_intermediate_directory.set_ByRef( configuration.GetValue< ::rux::XString >( "intermediate_directory" , found , 22 , 0 ) );
								cpp_makefile_project()->_rm_command.set_ByRef( configuration.GetValue< ::rux::XString >( "rm_command" , found , 10 , 0 ) );
								cpp_makefile_project()->_mk_dir_command.set_ByRef( configuration.GetValue< ::rux::XString >( "mk_dir_command" , found , 14 , 0 ) );
								cpp_makefile_project()->_object_suffix.set_ByRef( configuration.GetValue< ::rux::XString >( "object_suffix" , found , 13 , 0 ) );
								cpp_makefile_project()->_source_switch.set_ByRef( configuration.GetValue< ::rux::XString >( "source_switch" , found , 13 , 0 ) );
								cpp_makefile_project()->_object_switch.set_ByRef( configuration.GetValue< ::rux::XString >( "object_switch" , found , 13 , 0 ) );
								cpp_makefile_project()->_include_switch.set_ByRef( configuration.GetValue< ::rux::XString >( "include_switch" , found , 14 , 0 ) );
								cpp_makefile_project()->_output_switch.set_ByRef( configuration.GetValue< ::rux::XString >( "output_switch" , found , 13 , 0 ) );
								cpp_makefile_project()->_lib_path_switch.set_ByRef( configuration.GetValue< ::rux::XString >( "lib_path_switch" , found , 15 , 0 ) );
								cpp_makefile_project()->_lib_switch.set_ByRef( configuration.GetValue< ::rux::XString >( "lib_switch" , found , 10 , 0 ) );
								cpp_makefile_project()->_preprocessor_switch.set_ByRef( configuration.GetValue< ::rux::XString >( "preprocessor_switch" , found , 19 , 0 ) );
								if( found == 0 )
									cpp_makefile_project()->_preprocessor_switch = "-D";
								cpp_makefile_project()->_include_paths.set_ByRef( configuration.GetValue< ::rux::XArray< ::rux::XString > >( "include_paths" , found , 13 , 0 ) );
								cpp_makefile_project()->_lib_paths.set_ByRef( configuration.GetValue< ::rux::XArray< ::rux::XString > >( "lib_paths" , found , 9 , 0 ) );
								cpp_makefile_project()->_libs.set_ByRef( configuration.GetValue< ::rux::XArray< ::rux::XString > >( "libs" , found , 4 , 0 ) );
								cpp_makefile_project()->_preprocessors.set_ByRef( configuration.GetValue< ::rux::XArray< ::rux::XString > >( "preprocessors" , found , 13 , 0 ) );										
								cpp_makefile_project()->_merged_libs.set_ByRef( configuration.GetValue< ::rux::XArray< ::rux::XString > >( "merged_libs" , found , 11 , 0 ) );
								cpp_makefile_project()->_output_file.set_ByRef( configuration.GetValue< ::rux::XString >( "output_file" , found , 11 , 0 ) );
								cpp_makefile_project()->_debug.set_ByRef( configuration.GetValue< ::rux::XBoolean >( "debug" , found , 5 , 0 ) );
							}
							else
								RUX_ERROR( error , "unknown output file type" );
						}
					}
					else
						RUX_ERROR( error , "field 'makefile' is not found" );
				}
				else
					RUX_ERROR( error , "configuration is not XDataObject" );
			}
			return cpp_makefile_project++;
		};
		void on_process_stdout( void* , const char* data , size_t )
		{
			::rux::XConsole::Print( stdout , XWhite , "%s" , data );
		};
		void on_process_stderr( void* , const char* data , size_t )
		{
			::rux::XConsole::Print( stderr , XRed , "%s" , data );
		};
		implement_duplicate_internal_result_function( ::rux::String& , CPPMakefileProject , get_ConfigurationName );
		::rux::String& CPPMakefileProject::get_ConfigurationName( void )
		{
			return _configuration_name++;
		};
		implement_duplicate_internal_result_function( ::rux::String& , NDKMakefileProject , get_ConfigurationName );
		::rux::String& NDKMakefileProject::get_ConfigurationName( void )
		{
			return _configuration_name++;
		};
		implement_duplicate_internal_result_function_2( ::rux::uint8 , CPPMakefileProject , rebuild , ::rux::XString& , ::rux::uint8 );
		implement_duplicate_internal_result_function_2( ::rux::uint8 , CPPMakefileProject , clean , ::rux::XString& , ::rux::uint8 );
		implement_duplicate_internal_result_function_2( ::rux::uint8 , CPPMakefileProject , build , ::rux::XString& , ::rux::uint8 );
		::rux::uint8 CPPMakefileProject::clean( ::rux::XString& error , ::rux::uint8 is_printf_info )
		{
			::rux::uint8 res = 0;
			declare_stack_variable( char , temp_working_directory , 4096 );
			rux_get_working_directory( temp_working_directory , 4096 );
			if( is_printf_info )
				::rux::XConsole::Print( stdout , XWhite , "Clean, process file '" + _makefile + "'\n==================================\n" );

			::rux::XString makefile_filename , makefile_directory;
			makefile_filename.set_ByRef( ::rux::io::XPath::GetFileName( _makefile ) );
			makefile_directory.set_ByRef( ::rux::io::XPath::GetDirectoryName( _makefile ) );
			makefile_directory.set_ByRef( makefile_directory.ConvertToAnsi() );
			if( chdir( makefile_directory.str() ) == 0 )
			{
				char* args_clean[] =
				{
					(char*)"-j" ,
					(char*)(char*)"4" ,
					(char*)"-f" ,
					makefile_filename.str() ,
					(char*)"clean"
				};
				declare_stack_variable( char , error_str , 1024 );
				::rux::diagnostics::process::start_with_redirect( "make" , args_clean , 5 , on_process_stdout , on_process_stderr , NULL , 0 , 0 , error_str );
				if( error_str[ 0 ] == 0 )
					res = 1;
				else
					RUX_ERROR( error , error_str );
				chdir( temp_working_directory );
			}
			else
				RUX_ERROR( error , XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_Errno ) );
			if( is_printf_info )
				::rux::XConsole::Print( stdout , XWhite , "%s" , "\n==================================\n" );
			return res;
		};
		::rux::uint8 CPPMakefileProject::build( ::rux::XString& error , ::rux::uint8 is_printf_info )
		{
			::rux::uint8 res = 0;
			declare_stack_variable( char , temp_working_directory , 4096 );
			rux_get_working_directory( temp_working_directory , 4096 );
			if( is_printf_info )
				::rux::XConsole::Print( stdout , XWhite , "Build, process file '" + _makefile + "'\n==================================\n" );

			::rux::XString makefile_filename , makefile_directory;
			makefile_filename.set_ByRef( ::rux::io::XPath::GetFileName( _makefile ) );
			makefile_directory.set_ByRef( ::rux::io::XPath::GetDirectoryName( _makefile ) );
			makefile_directory.set_ByRef( makefile_directory.ConvertToAnsi() );
			if( chdir( makefile_directory.str() ) == 0 )
			{
				char* args_build[] =
				{
					(char*)"-j" ,
					(char*)"4" ,
					(char*)"-f" ,
					makefile_filename.str()
				};
				declare_stack_variable( char , error_str , 1024 );
				if( ::rux::diagnostics::process::start_with_redirect( "make" , args_build , 4 , on_process_stdout , on_process_stderr , NULL , 0 , 0 , error_str ) == 0 )
				{
					char* args_post_build[] =
					{
						(char*)"-j" ,
						(char*)"4" ,
						(char*)"-f" ,
						makefile_filename.str() ,
						(char*)"PostBuild"
					};
					if( ::rux::diagnostics::process::start_with_redirect( "make" , args_post_build , 5 , on_process_stdout , on_process_stderr , NULL , 0 , 0 , error_str ) == 0 )
					{
						::rux::XString output_file;
						output_file.set_ByRef( ::rux::io::XPath::CalculateAbsolute( makefile_directory , _output_file ) );
						output_file.set_ByRef( output_file.ConvertToAnsi() );
						if( _output_file_type_enum == ::rux::generator::XEnum_OutputFile_StaticLibrary
							&& _merged_libs.Count() > 0 )
						{
							::rux::XString temp_path;
							temp_path.set_ByRef( makefile_directory + "/" + ::rux::XUInt64( ::rux::XTime::get_NowAsUnixTime() ).ToString() );
							::rux::io::XDirectory::CreateAll( temp_path , error );
							if( error.Length() == 0 )
							{
								temp_path.set_ByRef( temp_path.ConvertToAnsi() );
								if( chdir( temp_path.str() ) == 0 )
								{
									::rux::XString archiver;
									archiver.set_ByRef( _archiver.ConvertToAnsi() );
									for( size_t index0 = 0 ; index0 < _merged_libs.Count() ; index0++ )
									{
										::rux::XString lib_path( _merged_libs[ index0 ] );
										::rux::uint8 found = 0;
										if( ::rux::io::XFile::Exists( lib_path ) )
											found = 1;
										if( found == 0 )
										{
											for( size_t index1 = 0 ; index1 < _lib_paths.Count() ; index1++ )
											{											
												lib_path.set_ByRef( _lib_paths[ index1 ] + "/" + _merged_libs[ index0 ] );
												lib_path.set_ByRef( ::rux::io::XPath::CalculateAbsolute( makefile_directory , lib_path ) );
												if( ::rux::io::XFile::Exists( lib_path ) )
												{
													found = 1;
													break;
												}
											}
										}
										if( found )
										{												
											lib_path.set_ByRef( lib_path.ConvertToAnsi() );
											char* archiver_extract[] =
											{
												(char*)"x" ,
												lib_path.str()
											};
											if( ::rux::diagnostics::process::start_with_redirect( archiver.str() , archiver_extract , 2 , on_process_stdout , on_process_stderr , NULL , 0 , 0 , error_str ) != 0 )
											{
												if( error_str[ 0 ] != 0 )
												{
													RUX_ERROR( error , error_str );
												}
												else
													RUX_ERROR( error , "archiver return unknown error" );
											}
										}
										else
										{
											RUX_ERROR( error , "cannot found merged lib '" + _merged_libs[ index0 ] + "'" );
											break;
										}
									}
									if( chdir( makefile_directory.str() ) != 0 )
										RUX_ERROR( error , XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_Errno ) );
									if( error.Length() == 0 )
									{											
										::rux::XArray< ::rux::XString > objects;
										::rux::XArray< ::rux::io::XFile > object_files;
										object_files.set_ByRef( ::rux::io::XDirectory::GetFiles( temp_path , XAllDirectories ) );
										for( size_t index0 = 0 ; index0 < object_files.Count() ; index0++ )
										{
											::rux::XString object_file;
											object_file.set_ByRef( object_files[ index0 ].get_FileName() );
											object_file.set_ByRef( object_file.ConvertToAnsi() );
											if( ::rux::io::XPath::GetFileNameExtension( object_file ) == "o" )
											{
												if( is_printf_info )
													::rux::XConsole::Print( stdout , XWhite , "Add file '" + object_file + "' to '" + output_file + "'\n" );													
												objects.Add( object_file );
											}
										}
										if( objects.Count() > 0 )
										{
											char** archiver_compress = alloc_array_macros( char* , objects.Count() + 2 );
#ifdef __LINUX__
											archiver_compress[ 0 ] = (char*)"rcus";
#elif defined( __SOLARIS__ )
											archiver_compress[ 0 ] = (char*)"-rv";
#endif
											archiver_compress[ 1 ] = output_file.str();
											for( size_t index0 = 0 ; index0 < objects.Count() ; index0++ )
												archiver_compress[ 2 + index0 ] = objects[ index0 ].str();
											if( ::rux::diagnostics::process::start_with_redirect( archiver.str() , archiver_compress , objects.Count() + 2 , on_process_stdout , on_process_stderr , NULL , 0 , 0 , error_str ) != 0 )
											{
												if( error_str[ 0 ] != 0 )
												{
													RUX_ERROR( error , error_str );
												}
												else
													RUX_ERROR( error , "archiver return unknown error" );
											}
											::rux::engine::free_mem( archiver_compress );
											if( error.Length() == 0 )
											{												
												char* ranlib[] =
												{
													output_file.str()
												};
												if( ::rux::diagnostics::process::start_with_redirect( "ranlib" , ranlib , 1 , on_process_stdout , on_process_stderr , NULL , 0 , 0 , error_str ) != 0 )
												{
													if( error_str[ 0 ] != 0 )
													{
														RUX_ERROR( error , error_str );
													}
													else
														RUX_ERROR( error , "archiver return unknown error" );
												}
											}
										}
									}
								}
								else
									RUX_ERROR( error , XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_Errno ) );
							}
							::rux::io::XDirectory::Delete( temp_path , XAllDirectories );
						}
						if( error.Length() == 0 )
							res = 1;
					}
					else
					{
						if( error_str[ 0 ] != 0 )
						{
							RUX_ERROR( error , error_str );
						}
						else
							RUX_ERROR( error , "make return unknown error" );
					}
				}
				else
				{
					if( error_str[ 0 ] != 0 )
					{
						RUX_ERROR( error , error_str );
					}
					else
						RUX_ERROR( error , "make return unknown error" );
				}
				chdir( temp_working_directory );
			}
			else
				RUX_ERROR( error , XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_Errno ) );
			if( is_printf_info )
				::rux::XConsole::Print( stdout , XWhite , "%s" , "\n==================================\n" );
			return res;
		};
		::rux::uint8 CPPMakefileProject::rebuild( ::rux::XString& error , ::rux::uint8 is_printf_info )
		{
			if( is_printf_info )
				::rux::XConsole::Print( stdout , XWhite , "ReBuild, process file '" + _makefile + "'\n==================================\n" );
			::rux::uint8 res = 0;
			res = clean( error , is_printf_info );
			if( res )
				res = build( error , is_printf_info );
			if( is_printf_info )
				::rux::XConsole::Print( stdout , XWhite , "%s" , "\n==================================\n" );
			return res;
		};
		implement_duplicate_internal_result_function_2( ::rux::uint8 , NDKMakefileProject , rebuild , ::rux::XString& , ::rux::uint8 );
		::rux::uint8 NDKMakefileProject::rebuild( ::rux::XString& error , ::rux::uint8 is_printf_info )
		{
			::rux::uint8 res = 0;
			declare_stack_variable( char , temp_working_directory , 4096 );
			rux_get_working_directory( temp_working_directory , 4096 );
			if( is_printf_info )
				::rux::XConsole::Print( stdout , XWhite , "Process jni directory '" + _jni_directory + "'\n" );
			::rux::XString ndk_build_file;

			ndk_build_file.set_ByRef( ::rux::io::XPath::CalculateAbsolute( _jni_directory , _ndk ) + "/ndk-build.cmd" );
			ndk_build_file.set_ByRef( ndk_build_file.ConvertToAnsi() );
			if( ::rux::io::XFile::Exists( ndk_build_file ) )
			{
				::rux::XString working_directory;
				working_directory.set_ByRef( ::rux::io::XPath::GetDirectoryName( _jni_directory ) );
				working_directory.set_ByRef( working_directory.ConvertToAnsi() );
				if( chdir( working_directory.str() ) == 0 )
				{
					char* args_clean[] =
					{
						(char*)"clean"
					};
					declare_stack_variable( char , error_str , 1024 );
					::rux::diagnostics::process::start_with_redirect( ndk_build_file.str() , args_clean , 1 , on_process_stdout , on_process_stderr , NULL , 0 , 0 , error_str );
					if( error_str[ 0 ] == 0 )
					{
						if( ::rux::diagnostics::process::start_with_redirect( ndk_build_file.str() , NULL , 0 , on_process_stdout , on_process_stderr , NULL , 0 , 0 , error_str ) == 0 )
						{
						}
						else
						{
							if( error_str[ 0 ] != 0 )
							{
								RUX_ERROR( error , error_str );
							}
							else
								RUX_ERROR( error , "ndk-build return unknown error" );
						}
					}
					else
						RUX_ERROR( error , error_str );
					chdir( temp_working_directory );
				}
				else
					RUX_ERROR( error , XError::ConvertErrorCodeToString( 0 , XEnum_ErrorType_Errno ) );
			}
			else
				RUX_ERROR( error , "cannot find file 'ndk-build'" );
			return res;
		};
		implement_duplicate_internal_result_function( ::rux::String& , CPPMakefileProject , generate_makefile );
		::rux::String& CPPMakefileProject::generate_makefile( void )
		{
			::rux::XString makefile;
			::rux::XString unique;
			unique.set_ByRef( ::rux::io::XPath::GetFileName( _output_file ) );
			makefile = "##\n## makefile generated by rux\n##";
			makefile += "\nintermediate_directory :=" + _intermediate_directory;
			makefile += "\nobjects_directory :=" + _intermediate_directory + "/" + unique + "_objects";
			makefile += "\noutput_file :=" + _output_file;
			makefile += "\nrm_command :=" + _rm_command;
			makefile += "\nmk_dir_command :=" + _mk_dir_command;
			makefile += "\nobject_suffix :=" + _object_suffix;
			makefile += "\nsource_switch :=" + _source_switch;
			makefile += "\nobject_switch :=" + _object_switch;
			makefile += "\ninclude_switch :=" + _include_switch;
			makefile += "\noutput_switch :=" + _output_switch;
			makefile += "\nlib_path_switch :=" + _lib_path_switch;
			makefile += "\nlib_switch :=" + _lib_switch;
			makefile += "\npreprocessor_switch :=" + _preprocessor_switch;
			makefile += "\ninclude_path :=";
			for( size_t index0 = 0 ; index0 < _include_paths.Count() ; index0++ )
				makefile += " $(include_switch)" + _include_paths[ index0 ];
			makefile += "\npreprocessors :=";
			for( size_t index0 = 0 ; index0 < _preprocessors.Count() ; index0++ )
				makefile += " $(preprocessor_switch)" + _preprocessors[ index0 ];
			makefile += "\nlib_path :=";
			for( size_t index0 = 0 ; index0 < _lib_paths.Count() ; index0++ )
				makefile += " $(lib_path_switch)" + _lib_paths[ index0 ];
			makefile += "\nlibs :=";
			for( size_t index0 = 0 ; index0 < _libs.Count() ; index0++ )
				makefile += " $(lib_switch)" + _libs[ index0 ];
			makefile += "\ncompiler :=" + _compiler;
			if( _debug == true )
			{
				_compiler_flags.set_ByRef( _compiler_flags.ReplaceAll( "-O0 " , "" ) );
				_compiler_flags.set_ByRef( _compiler_flags.ReplaceAll( "  " , " " ) );
				_compiler_flags.set_ByRef( _compiler_flags.ReplaceAll( "-O2 " , "" ) );
				_compiler_flags.set_ByRef( _compiler_flags.ReplaceAll( "  " , " " ) );
				_compiler_flags.set_ByRef( _compiler_flags.ReplaceAll( "-g " , "" ) );
				_compiler_flags.set_ByRef( _compiler_flags.ReplaceAll( "  " , " " ) );
				_compiler_flags.set_ByRef( "-g -O0 " + _compiler_flags );
			}
			makefile += "\ncompiler_flags :=";
			if( _isystem.Length() > 0 )
				makefile += " -isystem " + _isystem;
			makefile += " -MD " + _compiler_flags;
			makefile += " $(preprocessors)";
			makefile += "\nlinker :=" + _linker;
			if( _debug == true )
			{
				_linker_flags.set_ByRef( _linker_flags.ReplaceAll( "-O2" , "" ) );
				_linker_flags.set_ByRef( _linker_flags.ReplaceAll( ",," , "," ) );
				_linker_flags.set_ByRef( _linker_flags.ReplaceAll( "-flto" , "" ) );
				_linker_flags.set_ByRef( _linker_flags.ReplaceAll( ",," , "," ) );
				_linker_flags.set_ByRef( _linker_flags.ReplaceAll( "--strip-all" , "" ) );
				_linker_flags.set_ByRef( _linker_flags.ReplaceAll( ",," , "," ) );
				if( _linker_flags.Length() > 0 && _linker_flags.get_UTF8Char( _linker_flags.Length() - 1 ) == ',' )
					_linker_flags.RemoveAt( _linker_flags.Length() - 1 );
				if( _linker_flags.Length() > 0 && _linker_flags.get_UTF8Char( 0 ) == ',' )
					_linker_flags.RemoveAt( 0 );
			}
			makefile += "\nlinker_flags := " + _linker_flags;
			makefile += "\narchiver :=" + _archiver;
			makefile += "\narchiver_flags := " + _archiver_flags;
			makefile += "\nasm_compiler :=" + _asm_compiler;
			makefile += "\nasm_compiler_flags := " + _asm_compiler_flags;

			makefile += "\nexclude_source_files :=";
			for( size_t index0 = 0 ; index0 < _exclude_files.Count() ; index0++ )
				makefile += " " + _exclude_files[ index0 ];

			makefile += "\nsource_files :=";
			for( size_t index0 = 0 ; index0 < _sources_files.Count() ; index0++ )
				makefile += " " + _sources_files[ index0 ];

			makefile += "\nsource_dirs :=";
			for( size_t index0 = 0 ; index0 < _source_dirs.Count() ; index0++ )
				makefile += " " + _source_dirs[ index0 ];			

			makefile += "\nvpath_dirs := $(source_dirs)\nvpath_dirs += $(dir $(source_files))\ncpp_suffix := .cpp\nasm_suffix := .S\ncpp_object_suffix := $(cpp_suffix)$(object_suffix)\nasm_object_suffix := $(asm_suffix)$(object_suffix)";
			makefile += "\n##\n## Objects\n##\nobjects0 := $(addsuffix $(object_suffix),$(addprefix $(objects_directory)/,$(filter-out $(exclude_source_files),$(notdir $(wildcard $(addsuffix /*.cpp, $(source_dirs)))))))\nobjects0 += $(addsuffix $(object_suffix),$(addprefix $(objects_directory)/,$(filter-out $(exclude_source_files),$(notdir $(wildcard $(addsuffix /*.S, $(source_dirs)))))))\nobjects0 += $(addsuffix $(object_suffix),$(addprefix $(objects_directory)/,$(filter-out $(exclude_source_files),$(notdir $(source_files)))))\nObjects=$(objects0)\n\n##\n## Build targets\n##\n.PHONY: all clean PreBuild  PostBuild\nall: $(intermediate_directory) $(objects_directory) $(output_file)\n\n$(output_file): $(Objects)\n\t";
			if( _output_file_type_enum == XEnum_OutputFile_Console
				|| _output_file_type_enum == XEnum_OutputFile_DynamicLibrary )
				makefile += "$(linker) $(output_switch)$(output_file) $(objects0) $(lib_path) $(libs) $(linker_flags)\n\n";
			else
				makefile += "$(archiver) $(archiver_flags) $(output_file) $(objects0)\n\n";
			makefile += "$(intermediate_directory):\n\t@test -d $(intermediate_directory) || $(mk_dir_command) $(intermediate_directory)\n\n";
			makefile += "$(objects_directory):\n\t@test -d $(objects_directory) || $(mk_dir_command) $(objects_directory)\n\n";
			makefile += "PostBuild:";
			for( size_t index0 = 0 ; index0 < _post_builds.Count() ; index0++ )
				makefile += "\n\t" + _post_builds[ index0 ];
			makefile += "\n\nPreBuild:";
			for( size_t index0 = 0 ; index0 < _pre_builds.Count() ; index0++ )
				makefile += "\n\t" + _pre_builds[ index0 ];
			makefile += "\n\n##\n## Objects\n##";
			makefile += "\n\nVPATH := $(vpath_dirs)\n\n$(filter $(addsuffix $(cpp_object_suffix),$(addprefix $(objects_directory)/,%)),$(objects0)): $(addsuffix $(object_suffix),$(addprefix $(objects_directory)/,%)): %\n\t$(compiler) $(source_switch) \"$<\" $(compiler_flags) $(object_switch)$@ $(include_path)\n\n$(filter $(addsuffix $(asm_object_suffix),$(addprefix $(objects_directory)/,%)),$(objects0)): $(addsuffix $(object_suffix),$(addprefix $(objects_directory)/,%)): %\n\t";			
			if( _asm_compiler.Length() == 0 )
				makefile += "$(compiler) $(source_switch) \"$<\" $(compiler_flags) $(object_switch)$@ $(include_path)\n";
			else
				makefile += "$(asm_compiler) \"$<\" $(asm_compiler_flags) $(object_switch)$@\n";
			makefile += "\n##\n## Clean\n##\nclean:";
			makefile += "\n\t$(rm_command) $(output_file)\n\n\trm -rf $(objects_directory)\n\ninclude $(wildcard $(addsuffix /*.d, $(objects_directory)))";
			return makefile++;
		};
	};
};