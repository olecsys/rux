#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_codegenerator.h>
#include <xan_jsonserializer.h>
#include <xan_io_defines.h>
#include <xan_path.h>
#include <xan_error.h>
#include <xan_dataobject.h>
#include <xan_memory_helper.h>
#include <xan_function_info.h>
begin_implement_rux_class_ns( RUXProject , rux::generator )
end_implement_rux_class();
namespace rux
{
	namespace generator
	{
		implement_duplicate_internal_result_function( ::rux::Array< ::rux::generator::XCPPMakefileProject >& , RUXProject , get_CPPMakefileProjects );
		::rux::Array< ::rux::generator::XCPPMakefileProject >& RUXProject::get_CPPMakefileProjects( void )
		{
			return _cpp_makefile_projects++;
		};
		implement_duplicate_internal_result_function( ::rux::Array< ::rux::generator::XNDKMakefileProject >& , RUXProject , get_NDKMakefileProjects );
		::rux::Array< ::rux::generator::XNDKMakefileProject >& RUXProject::get_NDKMakefileProjects( void )
		{
			return _ndk_makefile_projects++;
		};
		RUXProject& XRUXProject::load( const ::rux::XString& ruxprj_string , ::rux::XString& error )
		{
			error.Clear();
			::rux::generator::XRUXProject project;			
			::rux::XString cp_filename;
			declare_stack_variable( char , temp_working_directory , 1024 );
			rux_get_working_directory( temp_working_directory , 1024 );
			::rux::XString working_directory( temp_working_directory );
			cp_filename.set_ByRef( ruxprj_string.ConvertToUTF8() );
			if( cp_filename.Length() > 0 
				&& cp_filename.get_UTF8Char( 0 ) == '.' )
				cp_filename.set_ByRef( working_directory + "/" + cp_filename );
			cp_filename.set_ByRef( ruxprj_string.ConvertToAnsi() );
			::rux::io::file ruxprj( cp_filename.str() );
			if( ruxprj.opened() )
			{				
				::rux::uint64 ruxprj_size = ruxprj.size() + 1;
				if( ruxprj_size > 0 )
				{
					char* string_text00 = alloc_array_macros( char , ruxprj_size + 1 );
					if( string_text00 )
					{
						size_t readen = 0;
						if( ruxprj.read_text( string_text00 , ruxprj_size , readen ) )
						{			
							char* ptr = string_text00;
							if( memcmp( string_text00 , _rux_utf8_header , 3 ) == 0 )
								ptr = &string_text00[ 3 ];
							XObject obj( ::rux::data::XJsonSerializer::Deserialize( ptr , 1 , error ) );
							if( error.Length() == 0 )
							{
								if( rux_is_object( obj , ::rux::data::XDataObject ) )
								{
									::rux::XString relative_dir , project_name;
									relative_dir.set_ByRef( ::rux::io::XPath::GetDirectoryName( cp_filename ) );
									relative_dir.set_ByRef( ::rux::io::XPath::ExcludeRelative( relative_dir ) );
																											
									::rux::data::XDataObject data_object( obj ) , configurations , configurations_global;
									::rux::uint8 found = 0;
									project_name.set_ByRef( data_object.GetValue< ::rux::XString >( "project_name" , found , 12 , 0 ) );
									configurations.set_ByRef( data_object.GetValue< ::rux::data::XDataObject >( "configurations" , found , 14 , 0 ) );									
									if( found == 1 )
									{
										configurations_global.set_ByRef( configurations.GetValue< ::rux::data::XDataObject >( "global" , found , 6 , 0 ) );
/* android makefile begin */
										::rux::data::XDataObject android;
										android.set_ByRef( configurations.GetValue< ::rux::data::XDataObject >( "android" , found , 7 , 0 ) );
										if( found )
										{
											::rux::data::XDataObject android_global;
											android_global.set_ByRef( android.GetValue< ::rux::data::XDataObject >( "global" , found , 6 , 0 ) );
											for( size_t index0 = 0 ; index0 < android.FieldsCount() ; index0++ )
											{
												if( android.get_FieldName( index0 ) != "global" )
												{
													XObject value( android.get_FieldValue( index0 ) );
													if( rux_is_object( value , ::rux::data::XDataObject ) )
													{
														::rux::data::XDataObject configuration( value );
														for( size_t index1 = 0 ; index1 < configuration.FieldsCount() ; index1++ )
														{
															XObject object_value1( configuration.get_FieldValue( index1 ) );
															if( rux_is_object( object_value1 , ::rux::XString ) )
															{
																::rux::XString string_value1;
																string_value1.set_ByRef( object_value1 );
																::rux::XString string_value2;
																string_value2.set_ByRef( android_global.GetValue< ::rux::XString >( configuration.get_FieldNamePointer( index1 ) , found , SIZE_MAX , 0 ) );
																if( found )
																	configuration.SetValue( index1 , string_value1 + " " + string_value2 );
															}
														}
														for( size_t index1 = 0 ; index1 < configurations_global.FieldsCount() ; index1++ )
														{
															configuration.GetValue< XObject >( configurations_global.get_FieldNamePointer( index1 ) , found , SIZE_MAX , 0 );
															if( found == 0 )
																configuration.SetValue( configurations_global.get_FieldName( index1 ) , configurations_global.get_FieldValue( index1 ) , 0 );
														}
														for( size_t index1 = 0 ; index1 < android_global.FieldsCount() ; index1++ )
														{
															configuration.GetValue< XObject >( android_global.get_FieldNamePointer( index1 ) , found , SIZE_MAX , 0 );
															if( found == 0 )
																configuration.SetValue( android_global.get_FieldName( index1 ) , android_global.get_FieldValue( index1 ) , 0 );
														}
														configuration.SetValue( "project_name" , project_name , 0 );

														::rux::XString js;
														js.set_ByRef( ::rux::data::XJsonSerializer::Serialize( configuration , 1 , error ) );
														if( error.Length() == 0 )
														{
															::rux::generator::XNDKMakefileProject ndk_makefile_project( ::rux::generator::XNDKMakefileProject::load( android.get_FieldName( index0 ) , js , relative_dir , error ) );
															if( error.Length() == 0 )
																project()->_ndk_makefile_projects.Add( ndk_makefile_project );
														}
														if( error.Length() > 0 )
															break;
													}
												}
												if( error.Length() > 0 )
													break;
											}
										}
/* android makefile end */
/* linux makefile begin */
										::rux::data::XDataObject linux_data_object;
										linux_data_object.set_ByRef( configurations.GetValue< ::rux::data::XDataObject >( "linux" , found , 5 , 0 ) );
										if( found )
										{
											::rux::data::XDataObject linux_global;
											linux_global.set_ByRef( linux_data_object.GetValue< ::rux::data::XDataObject >( "global" , found , 6 , 0 ) );
											for( size_t index0 = 0 ; index0 < linux_data_object.FieldsCount() ; index0++ )
											{
												if( linux_data_object.get_FieldName( index0 ) != "global" )
												{
													XObject value( linux_data_object.get_FieldValue( index0 ) );
													if( rux_is_object( value , ::rux::data::XDataObject ) )
													{
														::rux::data::XDataObject configuration( value );
														for( size_t index1 = 0 ; index1 < configuration.FieldsCount() ; index1++ )
														{
															XObject object_value1( configuration.get_FieldValue( index1 ) );
															if( rux_is_object( object_value1 , ::rux::XString ) )
															{
																::rux::XString string_value1;
																string_value1.set_ByRef( object_value1 );
																::rux::XString string_value2;
																string_value2.set_ByRef( linux_global.GetValue< ::rux::XString >( configuration.get_FieldNamePointer( index1 ) , found , SIZE_MAX , 0 ) );
																if( found )
																	configuration.SetValue( index1 , string_value1 + " " + string_value2 );
															}
														}
														for( size_t index1 = 0 ; index1 < configurations_global.FieldsCount() ; index1++ )
														{
															configuration.GetValue< XObject >( configurations_global.get_FieldNamePointer( index1 ) , found , SIZE_MAX , 0 );
															if( found == 0 )
																configuration.SetValue( configurations_global.get_FieldName( index1 ) , configurations_global.get_FieldValue( index1 ) , 0 );
														}
														for( size_t index1 = 0 ; index1 < linux_global.FieldsCount() ; index1++ )
														{
															configuration.GetValue< XObject >( linux_global.get_FieldNamePointer( index1 ) , found , SIZE_MAX , 0 );
															if( found == 0 )
																configuration.SetValue( linux_global.get_FieldName( index1 ) , linux_global.get_FieldValue( index1 ) , 0 );
														}
														configuration.SetValue( "project_name" , project_name , 0 );

														::rux::XString js;
														js.set_ByRef( ::rux::data::XJsonSerializer::Serialize( configuration , 1 , error ) );
														if( error.Length() == 0 )
														{
															::rux::generator::XCPPMakefileProject cpp_makefile_project( ::rux::generator::XCPPMakefileProject::load( linux_data_object.get_FieldName( index0 ) , js , relative_dir , error ) );
															if( error.Length() == 0 )
																project()->_cpp_makefile_projects.Add( cpp_makefile_project );
														}
														if( error.Length() > 0 )
															break;
													}
												}
												if( error.Length() > 0 )
													break;
											}
										}
/* linux makefile end */
									}
									else
										RUX_ERROR( error , "field 'configurations' is not found" );
								}
								else
									RUX_ERROR( error , "json data is not XDataObject" );
							}
						}
						else
							RUX_ERROR( error , "cannot read text from file '" + cp_filename + "'" );
						::rux::engine::free_mem( string_text00 );
					}
					else
						RUX_ERROR( error , "cannot alloc memory" );
				}
				else
					RUX_ERROR( error , "file '" + cp_filename + "' size is equal 0" );
			}
			else
				RUX_ERROR( error , "cannot open file '" + cp_filename + "'" );
			return project++;
		};
	};
};