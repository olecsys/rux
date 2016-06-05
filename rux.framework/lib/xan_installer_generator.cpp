#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_installer_generator.h>
#include <xan_time.h>
#include <xan_directory.h>
#include <xan_visualstudiogenerator.h>
#include <xan_zip.h>
#include <xan_console.h>
#include <xan_process.h>
#include <xan_registry.h>
begin_implement_rux_class_ns( InstallerAction , rux::generator );
end_implement_rux_class();
namespace rux
{
	namespace generator
	{
		void XInstallerAction::correct_string( rux::XString& string )
		{
			string.set_ByRef( string.ReplaceAll( "\\" , "\\\\" ) );
			string.set_ByRef( string.ReplaceAll( "\"" , "\\\"" ) );	
			string.set_ByRef( string.ReplaceAll( "\n" , "\\n" ) );	
			string.set_ByRef( string.ReplaceAll( "\r" , "\\r" ) );	
			string.set_ByRef( string.ReplaceAll( "\t" , "\\t" ) );	
		};
		XInstallerAction::XInstallerAction( const rux::data::XDataObject& object )
		{
			rux::uint8 found = 0;
			(*this)()->_type = object.GetValue< rux::XString >( "type" , found , 4 , 0 );
			if( (*this)()->_type.Equals( "create_link" , 11 ) )
			{
				(*this)()->_file = object.GetValue< rux::XString >( "file" , found , 4 , 0 );
				XInstallerAction::correct_string( (*this)()->_file );
				(*this)()->_link_file = object.GetValue< rux::XString >( "link_file" , found , 9 , 0 );
				XInstallerAction::correct_string( (*this)()->_link_file );
				(*this)()->_link_description = object.GetValue< rux::XString >( "link_description" , found , 16 , 0 );
				XInstallerAction::correct_string( (*this)()->_link_description );
				(*this)()->_link_arguments = object.GetValue< rux::XString >( "link_arguments" , found , 14 , 0 );
				XInstallerAction::correct_string( (*this)()->_link_arguments );
				(*this)()->_link_working_directory = object.GetValue< rux::XString >( "link_working_directory" , found , 22 , 0 );
				XInstallerAction::correct_string( (*this)()->_link_working_directory );
			}
			else if( (*this)()->_type.Equals( "copy_product_files" , 18 ) )
			{
			}
			else if( (*this)()->_type.Equals( "directory_rights" , 16 ) )
			{
				(*this)()->_directory = object.GetValue< rux::XString >( "directory" , found , 9 , 0 );
				XInstallerAction::correct_string( (*this)()->_directory );
				(*this)()->_cmd = object.GetValue< rux::XString >( "cmd" , found , 3 , 0 );
				XInstallerAction::correct_string( (*this)()->_cmd );
				(*this)()->_access = object.GetValue< rux::XString >( "access" , found , 6 , 0 );
				XInstallerAction::correct_string( (*this)()->_access );
				(*this)()->_user_or_group = object.GetValue< rux::XString >( "user_or_group" , found , 13 , 0 );
				XInstallerAction::correct_string( (*this)()->_user_or_group );
			}
			else if( (*this)()->_type.Equals( "file_rights" , 11 ) )
			{
				(*this)()->_file = object.GetValue< rux::XString >( "file" , found , 4 , 0 );
				XInstallerAction::correct_string( (*this)()->_file );
				(*this)()->_cmd = object.GetValue< rux::XString >( "cmd" , found , 3 , 0 );
				XInstallerAction::correct_string( (*this)()->_cmd );
				(*this)()->_access = object.GetValue< rux::XString >( "access" , found , 6 , 0 );
				XInstallerAction::correct_string( (*this)()->_access );
				(*this)()->_user_or_group = object.GetValue< rux::XString >( "user_or_group" , found , 13 , 0 );
				XInstallerAction::correct_string( (*this)()->_user_or_group );
			}
			else if( (*this)()->_type.Equals( "process", 7 ) )
			{
				(*this)()->_file = object.GetValue< rux::XString >( "file" , found , 4 , 0 );
				XInstallerAction::correct_string( (*this)()->_file );
				(*this)()->_cmd = object.GetValue< rux::XString >( "cmd" , found , 3 , 0 );
				XInstallerAction::correct_string( (*this)()->_cmd );
				(*this)()->_method = object.GetValue< rux::XString >( "method" , found , 6 , 0 );
				XInstallerAction::correct_string( (*this)()->_method );
				(*this)()->_arguments = object.GetValue< rux::XString >( "arguments" , found , 9 , 0 );
				XInstallerAction::correct_string( (*this)()->_arguments );
				(*this)()->_variable = object.GetValue< rux::XString >( "variable" , found , 8 , 0 );
				XInstallerAction::correct_string( (*this)()->_variable );
			}
			else if( (*this)()->_type.Equals( "service", 7 ) )
			{
				(*this)()->_cmd = object.GetValue< rux::XString >( "cmd" , found , 3 , 0 );
				XInstallerAction::correct_string( (*this)()->_cmd );
				(*this)()->_service_name = object.GetValue< rux::XString >( "service_name" , found , 12 , 0 );
				XInstallerAction::correct_string( (*this)()->_service_name );
			}
			else if( (*this)()->_type.Equals( "file", 4 ) )
			{
				(*this)()->_cmd = object.GetValue< rux::XString >( "cmd" , found , 3 , 0 );
				XInstallerAction::correct_string( (*this)()->_cmd );
				(*this)()->_text = object.GetValue< rux::XString >( "text" , found , 4 , 0 );
				XInstallerAction::correct_string( (*this)()->_text );
				(*this)()->_file = object.GetValue< rux::XString >( "file" , found , 4 , 0 );
				XInstallerAction::correct_string( (*this)()->_file );
			}
			else if( (*this)()->_type.Equals( "network", 7 ) )
			{
				(*this)()->_cmd = object.GetValue< rux::XString >( "cmd" , found , 3 , 0 );
				XInstallerAction::correct_string( (*this)()->_cmd );
				(*this)()->_variable = object.GetValue< rux::XString >( "variable" , found , 8 , 0 );
				XInstallerAction::correct_string( (*this)()->_variable );
			}
			else if( (*this)()->_type.Equals( "console", 7 ) )
			{
				(*this)()->_cmd = object.GetValue< rux::XString >( "cmd" , found , 3 , 0 );
				XInstallerAction::correct_string( (*this)()->_cmd );
				(*this)()->_message = object.GetValue< rux::XString >( "message" , found , 7 , 0 );
				XInstallerAction::correct_string( (*this)()->_message );
				(*this)()->_answers = object.GetValue< rux::XArray< XObject > >( "answers" , found , 7 , 0 );
				(*this)()->_variable = object.GetValue< rux::XString >( "variable" , found , 8 , 0 );
				XInstallerAction::correct_string( (*this)()->_variable );
			}
			else if( (*this)()->_type.Equals( "registry", 8 ) )
			{
				(*this)()->_cmd = object.GetValue< rux::XString >( "cmd" , found , 3 , 0 );
				XInstallerAction::correct_string( (*this)()->_cmd );
				(*this)()->_predefined_key = object.GetValue< rux::XString >( "predefined_key" , found , 14 , 0 );
				XInstallerAction::correct_string( (*this)()->_predefined_key );
				(*this)()->_key = object.GetValue< rux::XString >( "key" , found , 3 , 0 );
				XInstallerAction::correct_string( (*this)()->_key );
				(*this)()->_name = object.GetValue< rux::XString >( "name" , found , 4 , 0 );
				XInstallerAction::correct_string( (*this)()->_name );
				(*this)()->_variable = object.GetValue< rux::XString >( "variable" , found , 8 , 0 );
				XInstallerAction::correct_string( (*this)()->_variable );
				(*this)()->_value = object.GetValue< rux::XString >( "value" , found , 5 , 0 );
				XInstallerAction::correct_string( (*this)()->_value );
			}
			else if( (*this)()->_type.Equals( "variable", 8 ) )
			{
				(*this)()->_cmd = object.GetValue< rux::XString >( "cmd" , found , 3 , 0 );
				XInstallerAction::correct_string( (*this)()->_cmd );
				(*this)()->_variable = object.GetValue< rux::XString >( "variable" , found , 8 , 0 );
				XInstallerAction::correct_string( (*this)()->_variable );
				(*this)()->_old_string = object.GetValue< rux::XString >( "old_string" , found , 10 , 0 );
				XInstallerAction::correct_string( (*this)()->_old_string );
				(*this)()->_new_string = object.GetValue< rux::XString >( "new_string" , found , 10 , 0 );
				XInstallerAction::correct_string( (*this)()->_new_string );
				(*this)()->_value_object = object.GetValue< XObject >( "value" , found , 5 , 0 );
			}
			(*this)()->_conditions = object.GetValue< rux::XArray< XObject > >( "conditions" , found , 10 , 0 );
			(*this)()->_nofatal = object.GetValue< rux::XBoolean >( "nofatal" , found , 7 , 0 );
		};
		rux::String& InstallerAction::get_variable_type( rux::XString& variable , rux::data::XDataObject& variables )
		{
			rux::XString type_name;
			for( rux::uint32 index0 = 0 ; index0 < variables.FieldsCount() ; index0++ )
			{
				if( variables.get_FieldName( index0 ) == variable )
				{
					XObject value( variables.get_FieldValue( index0 ) );
					XType type( value.get_Type() );
					type_name = "rux::X";
					type_name += type.get_TypeName();
					break;
				}
			}
			return type_name++;
		};
		rux::String& XInstallerAction::add_process_string_to_cpp( const rux::XString& name , rux::data::XDataObject& variables , size_t tabs_count )
		{
			rux::XString cpp_code;
			rux::XString tabs;
			for( size_t index0 = 0 ; index0 < tabs_count ; index0++ )
				tabs += "\t";
			cpp_code += tabs + name + ".set_ByRef( " + name + ".ReplaceAll( \"$(install_dir)\" , install_dir_string ) );\n";
			cpp_code += tabs + name + ".set_ByRef( " + name + ".ReplaceAll( \"$(start_menu_programs_dir)\" , start_menu_programs_string ) );\n";
			XObject value;
			rux::XString _name;
			for( rux::uint32 index0 = 0 ; index0 < variables.FieldsCount() ; index0++ )
			{
				value = variables.get_FieldValue( index0 );
				if( rux_is_object( value , rux::XString ) )
				{
					_name.set_ByRef( variables.get_FieldName( index0 ) );
					cpp_code += tabs + name + ".set_ByRef( " + name + ".ReplaceAll( \"$(" + _name + ")\" , " + _name + " ) );\n";
				}
				else if( rux_is_object( value , rux::XInt64 ) 
					|| rux_is_object( value , rux::XUInt64 ) 
					|| rux_is_object( value , rux::XInt32 ) 
					|| rux_is_object( value , rux::XUInt32 )
					|| rux_is_object( value , rux::XFloat )
					|| rux_is_object( value , rux::XDouble ) )
				{
					_name.set_ByRef( variables.get_FieldName( index0 ) );
					cpp_code += tabs + name + ".set_ByRef( " + name + ".ReplaceAll( \"$(" + _name + ")\" , " + _name + ".ToString() ) );\n";
				}
			}
			return cpp_code++;
		};
		rux::String& InstallerAction::get_conditions( rux::uint8 exclude_states )
		{
			rux::XString cpp_code;
			rux::uint32 conditions = 0;
			for( rux::uint32 index0 = 0 ; index0 < _conditions.Count() ; index0++ )
			{
				if( rux_is_object( _conditions[ index0 ] , rux::data::XDataObject ) )
				{
					rux::data::XDataObject condition_object( _conditions[ index0 ] );
					rux::XString variable , _operator , _state;
					rux::XArray< XObject > states;
					rux::uint8 found = 0;
					_state = condition_object.GetValue< rux::XString >( "state" , found , 5 , 0 );
					rux::uint32 states_count = 0;
					if( exclude_states == 0 )
					{
						if( _state.Length() > 0 )
						{
							if( _state == "update" )
							{
								if( conditions > 0 )
									cpp_code += " && ";
								else
									cpp_code += "			if( ";
								cpp_code += "_state == XEnum_InstallerState_Update";
								conditions++;
								states_count++;
							}
							else if( _state == "install" )
							{
								if( conditions > 0 )
									cpp_code += " && ";
								else
									cpp_code += "			if( ";
								cpp_code += "_state == XEnum_InstallerState_Install";
								conditions++;
								states_count++;
							}
							else if( _state == "uninstall" )
							{
								if( conditions > 0 )
									cpp_code += " && ";
								else
									cpp_code += "			if( ";
								cpp_code += "_state == XEnum_InstallerState_UnInstall";
								conditions++;
								states_count++;
							}
							else if( _state == "cancel" )
							{
								if( conditions > 0 )
									cpp_code += " && ";
								else
									cpp_code += "			if( ";
								cpp_code += "_state == XEnum_InstallerState_Cancel";
								conditions++;
								states_count++;
							}
						}
						else
						{				
							states = condition_object.GetValue< rux::XArray< XObject > >( "state" , found , 5 , 0 );
							for( rux::uint32 index1 = 0 ; index1 < states.Count() ; index1++ )
							{
								if( rux_is_object( states[ index1 ] , rux::XString ) )
								{
									rux::XString value_string;
									value_string.set_ByRef( states[ index1 ] );
									if( value_string == "update"
										|| value_string == "install"
										|| value_string == "uninstall"
										|| value_string == "cancel" )
									{							
										if( states_count == 0 )
										{
											if( conditions == 0 )
												cpp_code += "			if( ";
											else
												cpp_code += "	&& ";
											cpp_code += "( ";
										}
										else
											cpp_code += " || ";
										if( value_string == "update" )
											cpp_code += "_state == XEnum_InstallerState_Update";
										else if( value_string == "install" )
											cpp_code += "_state == XEnum_InstallerState_Install";
										else if( value_string == "uninstall" )
											cpp_code += "_state == XEnum_InstallerState_UnInstall";
										else if( value_string == "cancel" )
											cpp_code += "_state == XEnum_InstallerState_Cancel";
										conditions++;
										states_count++;
									}
								}
							}
							if( states_count > 0 )
								cpp_code += " )";
						}
					}
					if( states_count == 0 )
					{
						XObject value;
						variable = condition_object.GetValue< rux::XString >( "variable" , found , 8 , 0 );
						_operator = condition_object.GetValue< rux::XString >( "operator" , found , 8 , 0 );
						value = condition_object.GetValue< XObject >( "value" , found , 5 , 0 );
						if( variable.Length() > 0
							&& ( _operator == "=" || _operator == ">" || _operator == "<" 
							|| _operator == ">=" || _operator == "<=" || _operator == "!="
							|| _operator == "<>" ) )
						{
							if( _operator == "=" )
								_operator ="==";
							else if( _operator == "<>" )
								_operator = "!=";
							rux::XString value_string;
							if( rux_is_object( value , rux::XBoolean ) )
							{
								rux::XBoolean boolean( value );
								value_string = boolean.ToString();
							}	
							else if( rux_is_object( value , rux::XInt64 ) )
							{
								rux::XInt64 int64_val( value );
								value_string = int64_val.ToString();
							}
							else if( rux_is_object( value , rux::XString ) )
							{
								rux::XString string;
								string.set_ByRef( value );
								XInstallerAction::correct_string( string );
								value_string = "\"" + string + "\"";
							}		
							if( value_string.Length() > 0 )
							{
								if( conditions > 0 )
									cpp_code += " && ";
								else
									cpp_code += "			if( ";
								cpp_code += variable + " " + _operator + " " + value_string;
								conditions++;
							}
						}
					}
				}
			}
			if( conditions > 0 )
				cpp_code += " )\n";
			return cpp_code++;
		};
		implement_duplicate_internal_result_function_3( rux::String& , InstallerAction , ToCppCode , const rux::XString& , rux::data::XDataObject& , rux::uint8 );
		rux::String& InstallerAction::ToCppCode( const rux::XString& product_name , rux::data::XDataObject& variables , rux::uint8 exclude_states )
		{
			rux::XString cpp_code;
			cpp_code += get_conditions( exclude_states );
			cpp_code += "			{\n";
			if( _type.Equals( "create_link" , 11 ) )
			{
				cpp_code += "				rux::XString file( \"" + _file + "\" , XEnumCodePage_UTF8 );\n";		
				cpp_code += "				rux::XString link_file( \"" + _link_file + "\" , XEnumCodePage_UTF8 );\n";
				cpp_code += "				rux::XString link_description( \"" + _link_description + "\" , XEnumCodePage_UTF8 );\n";
				cpp_code += "				rux::XString link_arguments( \"" + _link_arguments + "\" , XEnumCodePage_UTF8 );\n";
				cpp_code += "				rux::XString link_working_directory( \"" + _link_working_directory + "\" , XEnumCodePage_UTF8 );\n";
				cpp_code += XInstallerAction::add_process_string_to_cpp( "file" , variables );		
				cpp_code += XInstallerAction::add_process_string_to_cpp( "link_file" , variables );		
				cpp_code += XInstallerAction::add_process_string_to_cpp( "link_description" , variables );		
				cpp_code += XInstallerAction::add_process_string_to_cpp( "link_arguments" , variables );		
				cpp_code += XInstallerAction::add_process_string_to_cpp( "link_working_directory" , variables );		
				cpp_code += "				if( rux::io::XFile::Exists( file ) == 1 )\n";
				cpp_code += "				{\n";
				cpp_code += "					rux::XString link_dir;\n";
				cpp_code += "					link_dir.set_ByRef( rux::io::XPath::GetDirectoryName( link_file ) );\n";
				cpp_code += "					rux::io::XDirectory::CreateAll( link_dir , error_string );\n";
				cpp_code += "					if( error_string.Length() == 0 )\n";
				cpp_code += "						rux::io::XDirectory::CreateLink( file , link_file , link_description , link_arguments , link_working_directory );\n";
				cpp_code += "				}\n";
				cpp_code += "				else\n";		
				cpp_code += "					error_string = \"file '\" + file + \"' does not exist\";\n";		
				if( _nofatal == true )
				{
					cpp_code += "				if( error_string.Length() > 0 )\n";			
					cpp_code += "					error_string.Clear();\n";
				}
			}
			else if( _type.Equals( "copy_product_files" , 18 ) )
			{
				cpp_code += "				if( install_dir_string.Length() == 0 )\n";
				cpp_code += "				{\n";
				cpp_code += "					rux::XString error;\n";
				cpp_code += "					install_dir_string = rux::io::XDirectory::get_SpecialFolderPath( \"PROGRAM FILES\" , error );\n";
				cpp_code += "					install_dir_string += \"/" + product_name + "\";\n";
				cpp_code += "				}\n";
				cpp_code += "				rux::io::XDirectory::CreateAll( install_dir_string , error_string );\n";
				cpp_code += "				if( error_string.Length() == 0 )\n";
				cpp_code += "					rux::resources::XVSRCResources::InstallFromResources( install_dir_string , error_string );\n";
				cpp_code += "				if( install_dir_string.Length() > 0 )\n";
				cpp_code += "				{\n";	
				cpp_code += "					rux::XString ansi_install_path;\n";	
				cpp_code += "					ansi_install_path.set_ByRef( install_dir_string.ConvertToAnsi() );\n";	
				cpp_code += "					if( ansi_install_path.Size() < 1024 )\n";
				cpp_code += "						memcpy( _install_path , ansi_install_path.str() , ansi_install_path.Size() );\n";	
				cpp_code += "				}\n";
			}
			else if( _type.Equals( "directory_rights" , 16 ) )
			{
				cpp_code += "				rux::XString directory( \"" + _directory + "\" , XEnumCodePage_UTF8 );\n";		
				cpp_code += "				rux::XString user_or_group( \"" + _user_or_group + "\" , XEnumCodePage_UTF8 );\n";		
				cpp_code += XInstallerAction::add_process_string_to_cpp( "directory" , variables );		
				cpp_code += XInstallerAction::add_process_string_to_cpp( "user_or_group" , variables );		
				if( _cmd.ToLower() == "allow" )
				{
					if( _access.ToLower() == "read" )
						cpp_code += "				rux::io::XDirectory::AddAllowReadAccess( directory , user_or_group , error_string );\n";
					else if( _access.ToLower() == "add_subdirectory" )
						cpp_code += "				rux::io::XDirectory::AddAllowAddSubdirectoryAccess( directory , user_or_group , error_string );\n";
					else if( _access.ToLower() == "all" )
						cpp_code += "				rux::io::XDirectory::AddAllowAllAccess( directory , user_or_group , error_string );\n";
				}
				if( _nofatal == true )
				{
					cpp_code += "				if( error_string.Length() > 0 )\n";			
					cpp_code += "					error_string.Clear();\n";
				}
			}
			else if( _type.Equals( "file_rights" , 11 ) )
			{
				cpp_code += "				rux::XString file( \"" + _file + "\" , XEnumCodePage_UTF8 );\n";		
				cpp_code += "				rux::XString user_or_group( \"" + _user_or_group + "\" , XEnumCodePage_UTF8 );\n";		
				cpp_code += XInstallerAction::add_process_string_to_cpp( "file" , variables );		
				cpp_code += XInstallerAction::add_process_string_to_cpp( "user_or_group" , variables );
				if( _cmd.ToLower() == "allow" )
				{
					if( _access.ToLower() == "execute" )
						cpp_code += "				rux::io::XFile::AddAllowExecuteAccess( file , user_or_group , error_string );\n";
					else if( _access.ToLower() == "all" )
						cpp_code += "				rux::io::XFile::AddAllowAllAccess( file , user_or_group , error_string );\n";
					if( _nofatal == true )
					{
						cpp_code += "				if( error_string.Length() > 0 )\n";			
						cpp_code += "					error_string.Clear();\n";
					}
				}
			}
			else if( _type.Equals( "process" , 7 ) )
			{
				cpp_code += "				rux::XString file( \"" + _file + "\" , XEnumCodePage_UTF8 );\n";		
				cpp_code += "				rux::XString arguments( \"" + _arguments + "\" , XEnumCodePage_UTF8 );\n";		
				cpp_code += XInstallerAction::add_process_string_to_cpp( "file" , variables );
				cpp_code += XInstallerAction::add_process_string_to_cpp( "arguments" , variables );
				if( _cmd.ToLower() == "start" )
				{			
					cpp_code += "				file.set_ByRef( file.ConvertToAnsi() );\n";
					cpp_code += "				arguments.set_ByRef( arguments.ConvertToAnsi() );\n";
					cpp_code += "				declare_stack_variable( char , err , 1024 );\n";
					cpp_code += "				::rux::memory::heap_chunk_t chunk( ::rux::memory::string_type );\n";
					cpp_code += "				rux::int32 result = rux::diagnostics::process::start_with_redirect( file.str() , arguments.str() , ::rux::diagnostics::on_default_redirect_stdout_or_stderr_handler , ::rux::diagnostics::on_default_redirect_stdout_or_stderr_handler , &chunk , err );\n";
					cpp_code += "				if( err[ 0 ] != 0 )\n";
					cpp_code += "					error_string = err;\n";
					cpp_code += "				if( chunk.actual_size() )\n";
					cpp_code += "					printf( \"%s\\n\" , (char*)chunk._data );\n";
					if( _nofatal == true )
					{
						cpp_code += "				if( error_string.Length() > 0 )\n";			
						cpp_code += "					error_string.Clear();\n";
					}			
					if( _variable.Length() > 0 )
					{
						cpp_code += "				" + _variable + ".set_ByRef( ";
						rux::XString variable_type;
						variable_type.set_ByRef( get_variable_type( _variable , variables ) );
						if( variable_type.Equals( "rux::XString" , 12 ) )
							cpp_code += "rux::XInt32( result ).ToString()";
						else if( variable_type.Equals( "rux::XInt64" , 11 ) )
							cpp_code += "rux::XInt64( result )";
						cpp_code += " );\n";
					}
				}		
				else if( _cmd.ToLower() == "start_detached" )
				{
					cpp_code += "				file.set_ByRef( file.ConvertToAnsi() );\n";
					cpp_code += "				arguments.set_ByRef( arguments.ConvertToAnsi() );\n";
					if( _method.ToLower() == "shell_execute" )
						cpp_code += "				rux_start_detached_process( file.str() , arguments.str() , 1 , rux_start_process_method_shell_execute );\n";
					else
						cpp_code += "				rux_start_detached_process( file.str() , arguments.str() , 1 );\n";
				}
				else if( _cmd.ToLower() == "kill" )
				{
					cpp_code += "				file.set_ByRef( file.ConvertToAnsi() );\n";
					cpp_code += "				rux::uint8 result = rux::diagnostics::process::kill( file.str() );\n";
					cpp_code += "				if( result == 0 )\n";
					cpp_code += "					error_string = \"rux::diagnostics::process::kill is failed\";\n";
				}
				else if( _cmd.ToLower() == "killall" )
				{
					cpp_code += "				file.set_ByRef( file.ConvertToAnsi() );\n";
					cpp_code += "				rux::uint8 result = rux::diagnostics::process::killall( file.str() );\n";
					cpp_code += "				if( result == 0 )\n";
					cpp_code += "					error_string = \"rux::diagnostics::process::killall is failed\";\n";
				}
				if( _nofatal == true )
				{
					cpp_code += "				if( error_string.Length() > 0 )\n";			
					cpp_code += "					error_string.Clear();\n";
				}
			}
			else if( _type.Equals( "service", 7 ) )
			{
				cpp_code += "				rux::XString service_name( \"" + _service_name + "\" , XEnumCodePage_UTF8 );\n";	
				cpp_code += XInstallerAction::add_process_string_to_cpp( "service_name" , variables );
				cpp_code += "				service_name.set_ByRef( service_name.ConvertToAnsi() );\n";
				cpp_code += "				declare_stack_variable( char , error0 , 1024 );\n";
				if( _cmd.ToLower() == "start" )
					cpp_code += "				rux::service::StartByName( service_name.str() , error0 );\n";
				else if( _cmd.ToLower() == "stop" )
					cpp_code += "				rux::service::Stop( service_name.str() , error0 );\n";
				else if( _cmd.ToLower() == "unregister" )
					cpp_code += "				rux::service::DeInstall( service_name.str() , error0 );\n";
				cpp_code += "				error_string = error0;\n";
				if( _nofatal == true )
				{
					cpp_code += "				if( error_string.Length() > 0 )\n";			
					cpp_code += "					error_string.Clear();\n";
				}
			}
			else if( _type.Equals( "file", 4 ) )
			{
				cpp_code += "				rux::XString text( \"" + _text + "\" , XEnumCodePage_UTF8 );\n";		
				cpp_code += "				rux::XString file( \"" + _file + "\" , XEnumCodePage_UTF8 );\n";		
				cpp_code += XInstallerAction::add_process_string_to_cpp( "file" , variables );
				cpp_code += XInstallerAction::add_process_string_to_cpp( "text" , variables );
				if( _cmd.ToLower() == "create" )
				{
					cpp_code += "				rux::io::XFile xfile = rux::io::XFile::Create( file , error_string );\n";
					cpp_code += "				if( error_string.Length() == 0 )\n";
					cpp_code += "				{\n";
					cpp_code += "					if( xfile.Exists() == 1 )\n";
					cpp_code += "					{\n";
					cpp_code += "						xfile.AppendText( text , error_string );\n";
					cpp_code += "						xfile.Close();\n";
					cpp_code += "					}\n";
					cpp_code += "				}\n";
					if( _nofatal == true )
					{
						cpp_code += "				if( error_string.Length() > 0 )\n";			
						cpp_code += "					error_string.Clear();\n";
					}
				}		
			}
			else if( _type.Equals( "registry" , 8 ) )
			{
				if( _cmd.ToLower() == "delete" )
				{
					cpp_code += "				rux::XRegistry registry;\n";
					cpp_code += "				rux::XString key( \"" + _key + "\" , XEnumCodePage_UTF8 );\n";	
					cpp_code += XInstallerAction::add_process_string_to_cpp( "key" , variables );			
					cpp_code += "				registry.DeleteRegKey( ";
					if( _predefined_key == "HKEY_LOCAL_MACHINE" )
						cpp_code += "XEnum_PredefinedRegKey_HKEY_LOCAL_MACHINE , ";
					cpp_code += "key , error_string );\n";			
					if( _nofatal == true )
					{
						cpp_code += "				if( error_string.Length() > 0 )\n";			
						cpp_code += "					error_string.Clear();\n";
					}
				}
				else if( _cmd.ToLower() == "set" )
				{
					cpp_code += "				rux::XRegistry registry;\n";
					cpp_code += "				rux::XString key( \"" + _key + "\" , XEnumCodePage_UTF8 );\n";	
					cpp_code += XInstallerAction::add_process_string_to_cpp( "key" , variables );
					cpp_code += "				rux::XString name( \"" + _name + "\" , XEnumCodePage_UTF8 );\n";	
					cpp_code += XInstallerAction::add_process_string_to_cpp( "name" , variables );
					cpp_code += "				rux::XString value( \"" + _value + "\" , XEnumCodePage_UTF8 );\n";	
					cpp_code += XInstallerAction::add_process_string_to_cpp( "value" , variables );
					cpp_code += "				registry.CreateRegKey( ";
					if( _predefined_key == "HKEY_LOCAL_MACHINE" )
						cpp_code += "XEnum_PredefinedRegKey_HKEY_LOCAL_MACHINE , ";
					cpp_code += "key , error_string );\n";
					cpp_code += "				if( error_string.Length() == 0 )\n";
					cpp_code += "					registry.set_RegValueAsString( ";
					if( _predefined_key == "HKEY_LOCAL_MACHINE" )
						cpp_code += "XEnum_PredefinedRegKey_HKEY_LOCAL_MACHINE , ";
					cpp_code += "key , name , value , error_string );\n";
					if( _nofatal == true )
					{
						cpp_code += "				if( error_string.Length() > 0 )\n";			
						cpp_code += "					error_string.Clear();\n";
					}
				}
				else if( _cmd.ToLower() == "get" )
				{
					if( _nofatal == true )
					{
						cpp_code += "				" + get_variable_type( _variable , variables ) + " old_value;\n";
						cpp_code += "				old_value = " + _variable + ";\n";
					}
					cpp_code += "				rux::XRegistry registry;\n";
					cpp_code += "				rux::XString key( \"" + _key + "\" , XEnumCodePage_UTF8 );\n";	
					cpp_code += XInstallerAction::add_process_string_to_cpp( "key" , variables );
					cpp_code += "				rux::XString name( \"" + _name + "\" , XEnumCodePage_UTF8 );\n";	
					cpp_code += XInstallerAction::add_process_string_to_cpp( "name" , variables );
					cpp_code += "				" + _variable + ".set_ByRef( registry.get_RegValueAsString( ";
					if( _predefined_key == "HKEY_LOCAL_MACHINE" )
						cpp_code += "XEnum_PredefinedRegKey_HKEY_LOCAL_MACHINE , ";
					cpp_code += "key , name , error_string ) );\n";
					if( _nofatal == true )
					{
						cpp_code += "				if( error_string.Length() > 0 )\n";
						cpp_code += "				{\n";
						cpp_code += "					" + _variable + " = old_value;\n";
						cpp_code += "					error_string.Clear();\n";
						cpp_code += "				}\n";				
					}
				}
			}
			else if( _type.Equals( "network" , 7 ) )
			{
				if( _cmd.ToLower() == "get_hostname" )
				{
					cpp_code += "				declare_stack_variable( char , hostname , 1024 );\n";
					cpp_code += "				rux_get_hostname( hostname , 1024 );\n";
					cpp_code += "				" + _variable + " = hostname;\n";
				}
			}		
			else if( _type.Equals( "variable" , 8 ) )
			{
				if( _cmd.ToLower() == "set" )
				{			
					if( rux_is_object( _value_object , rux::XBoolean ) )
					{
						cpp_code += "				" + _variable + " = ";
						rux::XBoolean value( _value_object );
						cpp_code += value.ToString() + ";\n";
					}
					else if( rux_is_object( _value_object , rux::XInt64 ) )
					{
						cpp_code += "				" + _variable + " = ";
						rux::XInt64 value( _value_object );
						cpp_code += value.ToString() + ";\n";
					}
					else if( rux_is_object( _value_object , rux::XString ) )
					{
						rux::XString value( _value_object );
						XInstallerAction::correct_string( value );
						cpp_code += "				rux::XString value( \"" + value + "\" , XEnumCodePage_UTF8 );\n";	
						cpp_code += XInstallerAction::add_process_string_to_cpp( "value" , variables );
						cpp_code += "				" + _variable + " = value;\n";
					}
				}
				else if( _cmd.ToLower() == "replace_all" )
				{			
					cpp_code += "				rux::XString old_string( \"" + _old_string + "\" , XEnumCodePage_UTF8 );\n";	
					cpp_code += XInstallerAction::add_process_string_to_cpp( "old_string" , variables );
					cpp_code += "				rux::XString new_string( \"" + _new_string + "\" , XEnumCodePage_UTF8 );\n";	
					cpp_code += XInstallerAction::add_process_string_to_cpp( "new_string" , variables );
					cpp_code += "				" + _variable + ".set_ByRef( " + _variable + ".ReplaceAll( old_string , new_string ) );\n";
				}
			}
			else if( _type.Equals( "console", 7 ) )
			{		
				if( _cmd.ToLower() == "write" )
				{			
					cpp_code += "				rux::XString message( \"" + _message + "\" , XEnumCodePage_UTF8 );\n";	
					cpp_code += XInstallerAction::add_process_string_to_cpp( "message" , variables );
					cpp_code += "				rux::XConsole::Print( stdout , XWhite , message );\n";
				}
				else if( _cmd.ToLower() == "read" )
				{
					if( _message.Length() > 0 )
					{
						cpp_code += "				rux::XString message( \"" + _message + "\" , XEnumCodePage_UTF8 );\n";	
						cpp_code += XInstallerAction::add_process_string_to_cpp( "message" , variables );
					}
					cpp_code += "				rux::XString answer;\n";
					cpp_code += "				rux::XConsole console;\n";
					cpp_code += "				do\n";
					cpp_code += "				{\n";
					if( _message.Length() > 0 )
						cpp_code += "					rux::XConsole::Print( stdout , XWhite , message );\n";
					cpp_code += "					answer.set_ByRef( console.Read() );\n";
					cpp_code += "				}\n";
					cpp_code += "				while( ";			
					rux::uint32 answers = 0;
					for( rux::uint32 index0 = 0 ; index0 < _answers.Count() ; index0++ )
					{
						if( rux_is_object( _answers[ index0 ] , rux::data::XDataObject ) )
						{
							rux::data::XDataObject answer_object( _answers[ index0 ] );
							rux::XString answer;
							rux::uint8 found = 0;
							answer = answer_object.GetValue< rux::XString >( "answer" , found , 6 , 0 );
							if( answer.Length() > 0 )
							{
								if( answers > 0 )
									cpp_code += " && ";
								cpp_code += "answer != \"" + answer + "\"";
								answers++;
							}
						}
					}
					if( answers == 0 )
						cpp_code += "false ";
					cpp_code += ");\n";
					answers = 0;
					for( rux::uint32 index0 = 0 ; index0 < _answers.Count() ; index0++ )
					{
						if( rux_is_object( _answers[ index0 ] , rux::data::XDataObject ) )
						{
							rux::data::XDataObject answer_object( _answers[ index0 ] );
							rux::XString answer;
							rux::uint8 found = 0;
							answer = answer_object.GetValue< rux::XString >( "answer" , found , 6 , 0 );
							if( answer.Length() > 0 )
							{
								if( answers > 0 )
									cpp_code += "				else ";
								else
									cpp_code += "				";
								cpp_code += "if( answer == \"" + answer + "\" )\n";
								cpp_code += "				{\n";
								XObject value;
								rux::XString name;
								for( rux::uint32 index1 = 0 ; index1 < answer_object.FieldsCount() ; index1++ )
								{
									name.set_ByRef( answer_object.get_FieldName( index1 ) );
									if( name.Equals( "answer" , 6 ) == false )
									{
										XInstallerAction::correct_string( name );
										value = answer_object.get_FieldValue( index1 );
										if( rux_is_object( value , rux::XBoolean ) )
										{
											rux::XBoolean boolean( value );
											cpp_code += "					" + name + " = " + boolean.ToString() + ";\n";
										}	
										else if( rux_is_object( value , rux::XInt64 ) )
										{
											rux::XInt64 int64_val( value );
											cpp_code += "					" + name + " = " + int64_val.ToString() + ";\n";
										}	
										else if( rux_is_object( value , rux::XString ) )
										{
											rux::XString string;
											string.set_ByRef( value );
											XInstallerAction::correct_string( string );
											cpp_code += "					" + name + " = \"" + string + "\";\n";
											XInstallerAction::add_process_string_to_cpp( name , variables );
										}							
									}
								}
								cpp_code += "				}\n";
								answers++;
							}
						}
					}
					if( _variable.Length() > 0 )
						cpp_code += "				" + _variable + ".set_ByRef( answer );\n";
				}
			}
			cpp_code += "			}\n";
			return cpp_code++;
		};
		void XInstallerGenerator::GenerateInstaller( const rux::XString& product_name , const rux::XString& product_version , 
				const rux::XString& registry_name , const rux::XString& architecture , rux::XArray< rux::XString >& rux_libraries_paths ,
				const rux::XString& output_installer_name , const rux::XString& os ,
				rux::XArray< XInstallerAction >& install_actions , rux::XArray< XInstallerAction >& uninstall_actions ,
				rux::XArray< XInstallerAction >& update_actions , const rux::XString& intermediate_directory , 
				rux::XArray< rux::XString >& rux_include_paths , const rux::XString& product_files_directory , rux::data::XDataObject& variables , rux::XString& error )
		{
			error.Clear();
			rux::XString rux_intermediate_directory , installer_project_dir;
			rux_intermediate_directory.set_ByRef( rux::io::XDirectory::TransformToRuxDirectoryName( intermediate_directory ) );
			rux::XString installer_cpp_main;
			installer_cpp_main.set_ByRef( GenerateInstallerCppMain( registry_name , output_installer_name , os , product_name , install_actions , uninstall_actions , update_actions , variables ) );
			installer_cpp_main.set_ByRef( installer_cpp_main.ConvertToUTF8() );
			installer_project_dir.set_ByRef( rux_intermediate_directory + "/installer" );
			rux::io::XDirectory::CreateAll( installer_project_dir , error );
			if( error.Length() == 0 )
			{
				rux::io::XDirectory::Create( installer_project_dir + "/src" );
				rux::io::XFile file = rux::io::XFile::Create( installer_project_dir + "/src/main.cpp" , error );
				if( file.Exists() == 1 )
				{
					if( error.Length() == 0 )
					{
						file.WriteBytes( installer_cpp_main.str() , installer_cpp_main.Size() - 1 , error );
						file.Close();
						if( error.Length() == 0 )
						{
							rux::XArray< XVisualStudioConfiguration > installer_configurations;
							rux::XArray< rux::XString > installer_excluded_sources, installer_preprocessors , installer_libs , installer_sources , installer_include_paths;
							installer_include_paths = rux_include_paths;
							installer_include_paths.Add( installer_project_dir );
							installer_libs.Add( ::rux::XString( "librux.service.lib" ) );
							installer_libs.Add( ::rux::XString( "librux.resources.lib" ) );
							installer_configurations.Add( XVisualStudioGenerator::GenerateVS2010CppProjectConfiguration( architecture , "Release" , installer_excluded_sources ,
								XEnum_OutputFile_Console , output_installer_name , installer_project_dir , installer_preprocessors ,
								installer_include_paths , installer_libs , "" , "" , rux_libraries_paths , 1 ) );
							installer_sources.Add( installer_project_dir + "/src/main.cpp" );
							installer_sources.Add( installer_project_dir + "/resource.h" );
							installer_sources.Add( installer_project_dir + "/rux.h" );
							installer_sources.Add( installer_project_dir + "/resource.rc" );
							installer_sources.Add( installer_project_dir + "/rux.rc" );
							rux::XString installer_vs2010_cpp_project;
							installer_vs2010_cpp_project.set_ByRef( XVisualStudioGenerator::GenerateVS2010CppProject( installer_configurations , installer_sources , installer_project_dir ) );
							file = rux::io::XFile::Create( installer_project_dir + "/installer.vcxproj" , error );
							if( file.Exists() == 1 )
							{
								if( error.Length() == 0 )
								{
									file.AppendText( installer_vs2010_cpp_project , error );
									file.Close();
									if( error.Length() == 0 )
									{
										file = rux::io::XFile::Create( installer_project_dir + "/resource.h" , error );
										if( file.Exists() == 1 )
										{
											if( error.Length() == 0 )
											{
												rux::XString resource_header( "#pragma once\n#define _rux_installer_zip 19859\n" );
												resource_header.set_ByRef( resource_header.ConvertToUTF8() );
												file.WriteBytes( resource_header.str() , resource_header.Size() - 1 , error );
												file.Close();
												if( error.Length() == 0 )
												{
													file = rux::io::XFile::Create( installer_project_dir + "/rux.h" , error );
													if( file.Exists() == 1 )
													{
														if( error.Length() == 0 )
														{
															rux::XString rux_header( "#pragma once\n#define IDR_RUX_ENGINE               105\n" );
															rux_header.set_ByRef( rux_header.ConvertToUTF8() );
															file.WriteBytes( rux_header.str() , rux_header.Size() - 1 , error );
															file.Close();
															if( error.Length() == 0 )
															{														
																file = rux::io::XFile::Create( installer_project_dir + "/resource.rc" , error );
																if( file.Exists() == 1 )
																{
																	if( error.Length() == 0 )
																	{
																		rux::XString resource_rc( "#include <resource.h>\n#include <windows.h>\n_rux_installer_zip RCDATA \"" + installer_project_dir + "/installer.zip\"\n" );
																		resource_rc.set_ByRef( resource_rc.ConvertToAnsi() );
																		file.AppendText( resource_rc , error );
																		file.Close();
																		if( error.Length() == 0 )
																		{
																			rux::XString rux_engine_filename;
																			for( rux::uint32 index0 = 0 ; index0 < rux_libraries_paths.Count() ; index0++ )
																			{
																				if( rux_engine_filename.Length() == 0 )
																				{
																					rux_engine_filename.set_ByRef( rux::io::XDirectory::TransformToRuxDirectoryName( rux_libraries_paths[ index0 ] ) + "/rux.engine.dll" );
																					if( rux::io::XFile::Exists( rux_engine_filename ) == 0 )
																						rux_engine_filename.Clear();
																				}																		
																			}
																			if( rux_engine_filename.Length() > 0 )
																			{																		
																				XZip::Compress( rux_engine_filename , installer_project_dir + "/rux.engine.dll.zip" , error );
																				if( error.Length() == 0 )
																				{
																					file = rux::io::XFile::Create( installer_project_dir + "/rux.rc" , error );
																					if( file.Exists() == 1 )
																					{
																						if( error.Length() == 0 )
																						{																				
																							rux::XString rux_rc( "#include \"rux.h\"\n#include \"windows.h\"\nIDR_RUX_ENGINE             RCDATA              \"" + rux_engine_filename + "\"\n" );
																							rux_rc.set_ByRef( rux_rc.ConvertToAnsi() );
																							file.AppendText( rux_rc , error );
																							file.Close();
																							if( error.Length() == 0 )
																							{
																								rux::XString uninstaller_cpp_main , uninstaller_project_dir;
																								uninstaller_cpp_main.set_ByRef( GenerateUnInstallerCppMain( registry_name , os , product_name , uninstall_actions , variables ) );
																								uninstaller_cpp_main.set_ByRef( uninstaller_cpp_main.ConvertToUTF8() );
																								uninstaller_project_dir.set_ByRef( rux_intermediate_directory + "/uninstaller" );
																								rux::io::XDirectory::CreateAll( uninstaller_project_dir , error );
																								if( error.Length() == 0 )
																								{
																									rux::io::XDirectory::Create( uninstaller_project_dir + "/src" );
																									file = rux::io::XFile::Create( uninstaller_project_dir + "/src/main.cpp" , error );
																									if( file.Exists() == 1 )
																									{
																										if( error.Length() == 0 )
																										{
																											file.WriteBytes( uninstaller_cpp_main.str() , uninstaller_cpp_main.Size() - 1 , error );
																											file.Close();
																											if( error.Length() == 0 )
																											{
																												rux::XArray< rux::io::XFile > files;
																												files.set_ByRef( rux::io::XDirectory::GetFiles( product_files_directory , XAllDirectories ) );
																												rux::XString resource_header_string = "#pragma once\n" , resource_rc_string = "#include <resource.h>\n#include <windows.h>\nSTRINGTABLE\nBEGIN\n" , string;
																												rux::uint32 resource_incrementer = 10000;
																												if( files.Count() > 0 )
																												{
																													for( rux::uint32 index0 = files.Count() - 1 ; index0 >= 0 ; index0-- )
																													{																											
																														resource_header_string += "#define _" + rux::XUInt32( resource_incrementer ).ToString() + "_ " + rux::XUInt32( resource_incrementer ).ToString() + "\n";
																														resource_rc_string += " _" + rux::XUInt32( resource_incrementer ).ToString() + "_ , ";																											
																														string = files[ index0 ].get_FileName();
																														string.RemoveRange( 0 , product_files_directory.Length() + 1 );
																														resource_rc_string += "\"" + string + "\"\n";
																														resource_incrementer++;
																														if( index0 == 0 )
																															break;
																													}
																												}
																												rux::XString json_directories_structure , local_json_directories_structure;
																												json_directories_structure.set_ByRef( rux::io::XDirectory::get_JSONDirectoriesStructure( product_files_directory ) );
																												resource_incrementer = 19860;
																												while( json_directories_structure.Length() > 0 )
																												{
																													if( json_directories_structure.Length() > 4096 )
																													{
																														if( json_directories_structure.Length() > 4096 
																															&& json_directories_structure.get_UTF8Char( 4095 ) == '"'
																															&& json_directories_structure.get_UTF8Char( 4096 ) == '"' )
																														{
																															local_json_directories_structure.set_ByRef( json_directories_structure.Substring( 0 , 4095 ) );
																															json_directories_structure.RemoveRange( 0 , 4095 );
																														}
																														else
																														{
																															local_json_directories_structure.set_ByRef( json_directories_structure.Substring( 0 , 4096 ) );
																															json_directories_structure.RemoveRange( 0 , 4096 );
																														}
																													}
																													else
																													{
																														local_json_directories_structure.set_ByRef( json_directories_structure.Substring( 0 , 4096 ) );
																														json_directories_structure.Clear();
																													}																									
																													resource_rc_string += " _" + rux::XUInt32( resource_incrementer ).ToString() + "_ , \"" + local_json_directories_structure + "\"\n";
																													resource_header_string += "#define _" + rux::XUInt32( resource_incrementer ).ToString() + "_ " + rux::XUInt32( resource_incrementer ).ToString() + "\n";
																													resource_incrementer++;
																												}
																												resource_rc_string += "END\n";			
																												file = rux::io::XFile::Create( uninstaller_project_dir + "/resource.rc" , error );
																												if( file.Exists() == 1 )
																												{
																													if( error.Length() == 0 )
																													{																												
																														resource_rc_string.set_ByRef( resource_rc_string.ConvertToAnsi() );
																														file.AppendText( resource_rc_string , error );
																														file.Close();
																														if( error.Length() == 0 )
																														{
																															file = rux::io::XFile::Create( uninstaller_project_dir + "/resource.h" , error );
																															if( file.Exists() == 1 )
																															{
																																if( error.Length() == 0 )
																																{																															
																																	resource_header_string.set_ByRef( resource_header_string.ConvertToUTF8() );
																																	file.WriteBytes( resource_header_string.str() , resource_header_string.Size() - 1 , error );
																																	file.Close();
																																	if( error.Length() == 0 )
																																	{
																																		file = rux::io::XFile::Create( uninstaller_project_dir + "/rux.h" , error );
																																		if( file.Exists() == 1 )
																																		{
																																			if( error.Length() == 0 )
																																			{
																																				rux::XString rux_header( "#pragma once\n#define IDR_RUX_ENGINE               105\n" );
																																				rux_header.set_ByRef( rux_header.ConvertToUTF8() );
																																				file.WriteBytes( rux_header.str() , rux_header.Size() - 1 , error );
																																				file.Close();
																																				if( error.Length() == 0 )
																																				{
																																					file = rux::io::XFile::Create( uninstaller_project_dir + "/rux.rc" , error );
																																					if( file.Exists() == 1 )
																																					{
																																						if( error.Length() == 0 )
																																						{
																																							rux::XString rux_rc( "#include \"rux.h\"\n#include \"windows.h\"\nIDR_RUX_ENGINE             RCDATA              \"" + rux_engine_filename + "\"\n" );
																																							rux_rc.set_ByRef( rux_rc.ConvertToAnsi() );
																																							file.AppendText( rux_rc , error );
																																							file.Close();
																																							if( error.Length() == 0 )
																																							{
																																								rux::XArray< XVisualStudioConfiguration > uninstaller_configurations;
																																								rux::XArray< rux::XString > uninstaller_excluded_sources, uninstaller_preprocessors , uninstaller_libs , uninstaller_sources , uninstaller_include_paths;
																																								uninstaller_include_paths = rux_include_paths;
																																								uninstaller_include_paths.Add( uninstaller_project_dir );
																																								uninstaller_libs.Add( ::rux::XString( "librux.service.lib" ) );
																																								uninstaller_libs.Add( ::rux::XString( "librux.resources.lib" ) );
																																								uninstaller_configurations.Add( XVisualStudioGenerator::GenerateVS2010CppProjectConfiguration( architecture , "Release" , uninstaller_excluded_sources ,
																																									XEnum_OutputFile_Console , product_files_directory + "/uninstaller.exe" , uninstaller_project_dir , uninstaller_preprocessors ,
																																									uninstaller_include_paths , uninstaller_libs , "" , "" , rux_libraries_paths , 1 ) );
																																								uninstaller_sources.Add( uninstaller_project_dir + "/src/main.cpp" );
																																								uninstaller_sources.Add( uninstaller_project_dir + "/resource.h" );
																																								uninstaller_sources.Add( uninstaller_project_dir + "/rux.h" );
																																								uninstaller_sources.Add( uninstaller_project_dir + "/resource.rc" );
																																								uninstaller_sources.Add( uninstaller_project_dir + "/rux.rc" );
																																								rux::XString uninstaller_vs2010_cpp_project;
																																								uninstaller_vs2010_cpp_project.set_ByRef( XVisualStudioGenerator::GenerateVS2010CppProject( uninstaller_configurations , uninstaller_sources , uninstaller_project_dir ) );
																																								file = rux::io::XFile::Create( uninstaller_project_dir + "/uninstaller.vcxproj" , error );
																																								if( file.Exists() == 1 )
																																								{
																																									if( error.Length() == 0 )
																																									{
																																										file.AppendText( uninstaller_vs2010_cpp_project , error );
																																										file.Close();
																																										if( error.Length() == 0 )
																																										{
																																											rux::XString windows_directory;
																																											windows_directory.set_ByRef( rux::io::XDirectory::get_SpecialFolderPath( "WINDOWS" , error ) );
																																											if( error.Length() == 0 )
																																											{
																																												if( rux::io::XFile::Exists( windows_directory + "/Microsoft.NET/Framework/v4.0.30319/MSBuild.exe" ) == 1 )
																																												{
																																													rux::diagnostics::XProcess process;
																																													process.set_ModuleName( windows_directory + "/Microsoft.NET/Framework/v4.0.30319/MSBuild.exe" );
																																													rux::XString arguments;
																																													if( architecture == "x86" )
																																														arguments = "\"" + uninstaller_project_dir + "/uninstaller.vcxproj\" /verbosity:n /p:Configuration=Release /p:Platform=Win32 /t:Clean";
																																													else
																																														arguments = "\"" + uninstaller_project_dir + "/uninstaller.vcxproj\" /verbosity:n /p:Configuration=Release /p:Platform=x64 /t:Clean";
																																													if( process.Start( arguments , error ) == 1 )
																																													{
																																														if( error.Length() == 0 )
																																														{
																																															if( architecture == "x86" )
																																																arguments = "\"" + uninstaller_project_dir + "/uninstaller.vcxproj\" /verbosity:n /p:Configuration=Release /p:Platform=Win32 /t:build";
																																															else
																																																arguments = "\"" + uninstaller_project_dir + "/uninstaller.vcxproj\" /verbosity:n /p:Configuration=Release /p:Platform=x64 /t:build";
																																															if( process.Start( arguments , error ) == 1 )
																																															{
																																																if( error.Length() == 0 )
																																																{
																																																	if( rux::io::XFile::Exists( product_files_directory + "/uninstaller.pdb" ) == 1 )
																																																	{
																																																		rux::io::XFile::Copy( product_files_directory + "/uninstaller.pdb" , uninstaller_project_dir + "/uninstaller.pdb" , error );																																													
																																																		rux::io::XFile::Delete( product_files_directory + "/uninstaller.pdb" , error );
																																																		error.Clear();
																																																	}
																																																	if( architecture == "x86" )
																																																		arguments = "\"" + installer_project_dir + "/installer.vcxproj\" /verbosity:n /p:Configuration=Release /p:Platform=Win32 /t:Clean";
																																																	else
																																																		arguments = "\"" + installer_project_dir + "/installer.vcxproj\" /verbosity:n /p:Configuration=Release /p:Platform=x64 /t:Clean";
																																																	if( process.Start( arguments , error ) == 1 )
																																																	{
																																																		if( error.Length() == 0 )
																																																		{
																																																			XZip::Compress( product_files_directory , installer_project_dir + "/installer.zip" , error );
																																																			if( error.Length() == 0 )
																																																			{
																																																				if( architecture == "x86" )
																																																					arguments = "\"" + installer_project_dir + "/installer.vcxproj\" /verbosity:n /p:Configuration=Release /p:Platform=Win32 /t:build";
																																																				else
																																																					arguments = "\"" + installer_project_dir + "/installer.vcxproj\" /verbosity:n /p:Configuration=Release /p:Platform=x64 /t:build";
																																																				if( process.Start( arguments , error ) == 1 )
																																																				{
																																																					if( error.Length() == 0 )
																																																					{																																														
																																																					}
																																																				}
																																																				else
																																																					error = "installer project build is failed";
																																																			}
																																																		}
																																																	}
																																																	else
																																																		error = "installer project clean is failed";
																																																}
																																															}
																																															else
																																																error = "uninstaller project build is failed";
																																														}
																																													}
																																													else
																																														error = "uninstaller project clean is failed";
																																												}
																																												else
																																													error = "Executable '" + windows_directory + "/Microsoft.NET/Framework/v4.0.30319/MSBuild.exe' is not found";
																																											}
																																										}
																																									}
																																								}
																																							}
																																						}
																																					}
																																				}
																																			}
																																		}
																																	}
																																}
																															}
																														}
																													}
																												}
																											}
																										}
																									}
																								}
																							}
																						}
																						else
																							file.Close();
																					}
																				}
																			}
																			else
																				error = "main rux dynamic modules not found,please correct field 'rux_libraries_paths'";
																		}
																	}
																	else
																		file.Close();
																}
															}
														}
														else
															file.Close();
													}
												}
											}
											else
												file.Close();
										}
									}
								}
								else
									file.Close();
							}
						}
					}
					else
						file.Close();
				}
			}
			if( error.Length() > 0 )
			{
				rux::XString error0;
				if( rux::io::XFile::Exists( product_files_directory + "/uninstaller.exe" ) == 1 )
					rux::io::XFile::Delete( product_files_directory + "/uninstaller.exe" , error0 );
				if( rux::io::XFile::Exists( output_installer_name ) == 1 )
					rux::io::XFile::Delete( output_installer_name , error0 );
			}
		};
		rux::String& XInstallerGenerator::GenerateUnInstallerCppMain( const rux::XString& registry_name , const rux::XString& os , const rux::XString& product_name , rux::XArray< XInstallerAction >& uninstall_actions , rux::data::XDataObject& variables )
		{
			rux::XString main;
			rux::XString module_extension , module_lib , name , rux_engine_name;
			rux::uint64 time = rux::XTime::get_NowAsUnixTime();
			rux_engine_name.set_ByRef( rux::XUInt64( time ).ToString() + "." );
			if( os == "windows" )
				module_extension = "dll";
			else
			{
				module_extension = "so";
				module_lib = "lib";
			}	
			main += "#ifdef HAVE_CONFIG_H\n";
			main += "#include <config.h>\n";
			main += "#endif\n";
			main += "#include <resource.h>\n";
			main += "#include <rux.h>\n";
			main += "#include <xan_path.h>\n";
			main += "#include <xan_vsrcresources.h>\n";
			main += "#include <xan_file.h>\n";
			main += "#include <xan_directory.h>\n";
			main += "#include <xan_registry.h>\n";
			main += "#include <xan_console.h>\n";	
			main += "#include <xan_service.h>\n";	
			main += "#include <xan_boolean.h>\n";
			main += "#include <xan_loader_defines.h>\n";
			main += "enum XEnum_InstallerState\n";
			main += "{\n";
			main += "	XEnum_InstallerState_Install ,\n";
			main += "	XEnum_InstallerState_UnInstall ,\n";
			main += "	XEnum_InstallerState_Update ,\n";
			main += "	XEnum_InstallerState_Cancel\n";
			main += "};\n";
			main += "dll_internal XEnum_InstallerState _state = XEnum_InstallerState_Install;\n";
			main += "const char* g_current_module_name = \"uninstaller.exe\";\n";
			main += "dll_internal rux::uint8 _rux_is_exited = 0;\n";
			main += "dll_internal XCrtSect _cs_rux_current_module;\n";
			main += "dll_internal char _install_path[ 1024 ] = {0};\n";
			main += "dll_internal char _rux_engine_filename[ 1024 ] = {0};\n";
			main += "rux::uint8 is_delete_conf = 0 , is_delete_engine = 0 , is_uninstall = 0 , _is_uninstall_configuration = 1;\n";	
			main += "void onexit( void )\n";
			main += "{\n";
			main += "	declare_stack_variable( char , executable_directory , 1024 );\n";
			main += "	declare_stack_variable( char , filename , 1024 );\n";
			main += "	::rux::safe_strncpy( executable_directory , ::rux::engine::_globals->_executable_directory , 1024 );;\n";
			main += "	rux::deinitialize();\n";	
			main += "	if( is_delete_engine == 1 || is_uninstall == 1 )\n";
			main += "	{\n";
			main += "		rux_concatenation( filename , executable_directory , \"/" + module_lib + rux_engine_name + module_extension + "\" );\n";
			main += "		rux_remove_file( filename );\n";
			main += "	}\n";	
			main += "	if( is_delete_conf == 1 || is_uninstall == 1 )\n";
			main += "	{\n";
			main += "		rux_concatenation( filename , executable_directory , \"/rux.conf\" );\n";
			main += "		rux_remove_file( filename );\n";
			main += "	}\n";
			main += "}\n";
			main += "rux::uint8 rux_uninstall_software( char error[ 1024 ] )\n";
			main += "{\n";	
			XObject value;
			for( rux::uint32 index0 = 0 ; index0 < variables.FieldsCount() ; index0++ )
			{
				name.set_ByRef( variables.get_FieldName( index0 ) );
				value = variables.get_FieldValue( index0 );
				if( rux_is_object( value , rux::XBoolean ) )
				{
					rux::XBoolean boolean_value( value );
					main += "	rux::XBoolean " + name + "( " + boolean_value.ToString() + " );\n";
				}
				else if( rux_is_object( value , rux::XString ) )
				{
					rux::XString string_value;
					string_value.set_ByRef( value );
					XInstallerAction::correct_string( string_value );
					main += "	rux::XString " + name + "( \"" + string_value + "\" , XEnumCodePage_UTF8 );\n";
				}
				else if( rux_is_object( value , rux::XInt64 ) )
				{
					rux::XInt64 int64_value( value );
					main += "	rux::XInt64 " + name + "( " + int64_value.ToString() + " );\n";
				}
			}
			main += "	error[ 0 ] = '\\0';\n";
			main += "	rux::XString error_string , install_dir_string;\n";
			main += "	rux::XRegistry registry;\n";
			main += "	install_dir_string.set_ByRef( registry.get_RegValueAsString( XEnum_PredefinedRegKey_HKEY_LOCAL_MACHINE , \"Software\\\\" + registry_name + "\" , \"InstallDir\" , error_string ) );\n";
			main += "	install_dir_string.set_ByRef( rux::io::XDirectory::TransformToRuxDirectoryName( install_dir_string ) );\n";
			main += "	rux::XString start_menu_programs_string;\n";
			main += "	start_menu_programs_string.set_ByRef( rux::io::XDirectory::get_SpecialFolderPath( \"START_MENU_PROGRAMS\" , error_string ) );\n";	
			main += "	if( error_string.Length() == 0 )\n";
			main += "	{\n";	
			for( rux::uint32 index0 = 0 ; index0 < variables.FieldsCount() ; index0++ )
			{
				name.set_ByRef( variables.get_FieldName( index0 ) );
				value = variables.get_FieldValue( index0 );
				if( rux_is_object( value , rux::XString ) )
					main += XInstallerAction::add_process_string_to_cpp( name , variables , 2 );
			}
			for( rux::uint32 index0 = 0 ; index0 < uninstall_actions.Count() ; index0++ ) 
			{
				main += "		if( error_string.Length() == 0 && _state != XEnum_InstallerState_Cancel )\n";
				main += "		{\n";
				main += uninstall_actions[ index0 ].ToCppCode( product_name , variables );
				main += "		}\n";
			}
			main += "	}\n";
			main += "	if( error_string.Length() == 0 && _state != XEnum_InstallerState_Cancel && _state != XEnum_InstallerState_Update && _is_uninstall_configuration == 1 )\n";
			main += "	{\n";
			main += "		rux::XArray< rux::io::XDirectory > dirs;\n";
			main += "		dirs.set_ByRef( rux::io::XDirectory::GetDirectories( install_dir_string , XTopDirectoryOnly ) );\n";
			main += "		rux::uint32 index0 = 0;\n";
			main += "		for( ; index0 < dirs.Count() ; index0++ )\n";
			main += "			rux::io::XDirectory::Delete( dirs[ index0 ].get_DirectoryName() , XAllDirectories );\n";
			main += "		rux::XArray< rux::io::XFile > files;\n";
			main += "		files.set_ByRef( rux::io::XDirectory::GetFiles( install_dir_string , XTopDirectoryOnly ) );\n";
			main += "		for( index0 = 0 ; index0 < files.Count() ; index0++ )\n";
			main += "		{\n";
			main += "			if( files[ index0 ].get_FileName() != install_dir_string + \"/" + module_lib + rux_engine_name + module_extension + "\" )\n";
			main += "				rux::io::XFile::Delete( files[ index0 ].get_FileName() , error_string );\n";
			main += "		}\n";
			main += "		if( rux::io::XDirectory::Exists( start_menu_programs_string + \"/" + product_name + "\" ) == 1 )\n";
			main += "			rux::io::XDirectory::Delete( start_menu_programs_string + \"/" + product_name + "\" , XAllDirectories );\n";
			main += "		rux::io::XDirectory::Delete( install_dir_string , XAllDirectories );\n";
			main += "		rux::XRegistry registry;\n";
			main += "		registry.DeleteRegKey( XEnum_PredefinedRegKey_HKEY_LOCAL_MACHINE , \"Software\\\\" + registry_name + "\" , error_string );\n";
			main += "	}\n";
			main += "	else if( error_string.Length() == 0 && ( _state == XEnum_InstallerState_Update || _is_uninstall_configuration == 0 ) )\n";
			main += "	{\n";
			main += "		rux::XArray< rux::XString > exclude_files;\n";
			main += "		exclude_files.Add( \"" + module_lib + rux_engine_name + module_extension + "\" );\n";
			main += "		rux::resources::XVSRCResources::UnInstallFromResources( install_dir_string , exclude_files , error_string );\n";	
			main += "		if( _state != XEnum_InstallerState_Update )\n";
			main += "		{\n";
			main += "			if( rux::io::XDirectory::Exists( start_menu_programs_string + \"/" + product_name + "\" ) == 1 )\n";
			main += "				rux::io::XDirectory::Delete( start_menu_programs_string + \"/" + product_name + "\" , XAllDirectories );\n";	
			main += "			rux::XRegistry registry;\n";
			main += "			registry.DeleteRegKey( XEnum_PredefinedRegKey_HKEY_LOCAL_MACHINE , \"Software\\\\" + registry_name + "\" , error_string );\n";
			main += "		}\n";
			main += "	}\n";
			main += "	if( error_string.Length() > 0 )\n";
			main += "	{\n";
			main += "		error_string.set_ByRef( error_string.ConvertToAnsi() );\n";
			main += "		if( error_string.Size() > 1024 )\n";
			main += "		{\n";
			main += "			memcpy( error , error_string.str() , 1023 );\n";
			main += "			error[ 1023 ] = '\\0';\n";
			main += "		}\n";
			main += "		else\n";
			main += "			memcpy( error , error_string.str() , error_string.Size() );\n";
			main += "	}\n";
			main += "	return error_string.Length() > 0 ? 0 : 1;\n";
			main += "};\n";
			main += "void	rux_force_uninstall_software( void )\n";
			main += "{\n";	
			main += "	if( _install_path[ 0 ] != '\\0' )\n";
			main += "	{\n";
			for( rux::uint32 index0 = 0 ; index0 < variables.FieldsCount() ; index0++ )
			{
				name.set_ByRef( variables.get_FieldName( index0 ) );
				value = variables.get_FieldValue( index0 );
				if( rux_is_object( value , rux::XBoolean ) )
				{
					rux::XBoolean boolean_value( value );
					main += "		rux::XBoolean " + name + "( " + boolean_value.ToString() + " );\n";
				}
				else if( rux_is_object( value , rux::XString ) )
				{
					rux::XString string_value;
					string_value.set_ByRef( value );
					XInstallerAction::correct_string( string_value );
					main += "		rux::XString " + name + "( \"" + string_value + "\" , XEnumCodePage_UTF8 );\n";
				}
				else if( rux_is_object( value , rux::XInt64 ) )
				{
					rux::XInt64 int64_value( value );
					main += "		rux::XInt64 " + name + "( " + int64_value.ToString() + " );\n";
				}
			}	
			main += "		rux::XString error_string , install_dir_string;\n";	
			main += "		install_dir_string = _install_path;\n";
			main += "		install_dir_string.set_ByRef( rux::io::XDirectory::TransformToRuxDirectoryName( install_dir_string ) );\n";
			main += "		rux::XString start_menu_programs_string;\n";
			main += "		start_menu_programs_string.set_ByRef( rux::io::XDirectory::get_SpecialFolderPath( \"START_MENU_PROGRAMS\" , error_string ) );\n";		
			for( rux::uint32 index0 = 0 ; index0 < variables.FieldsCount() ; index0++ )
			{
				name.set_ByRef( variables.get_FieldName( index0 ) );
				value = variables.get_FieldValue( index0 );
				if( rux_is_object( value , rux::XString ) )
					main += XInstallerAction::add_process_string_to_cpp( name , variables , 2 );
			}
			for( rux::uint32 index0 = 0 ; index0 < uninstall_actions.Count() ; index0++ ) 
			{
				main += "		{\n";
				main += uninstall_actions[ index0 ].ToCppCode( product_name , variables );
				main += "		}\n";
			}
			main += "		if( rux::io::XDirectory::Exists( start_menu_programs_string + \"/" + product_name + "\" ) == 1 )\n";
			main += "			rux::io::XDirectory::Delete( start_menu_programs_string + \"/" + product_name + "\" , XAllDirectories );\n";
			main += "		rux::io::XDirectory::Delete( install_dir_string , XAllDirectories );\n";
			main += "		rux::XRegistry registry;\n";
			main += "		registry.DeleteRegKey( XEnum_PredefinedRegKey_HKEY_LOCAL_MACHINE , \"Software\\\\" + registry_name + "\" , error_string );\n";	
			main += "	}\n";
			main += "};\n";	
			main += "int main( int argc , char** args )\n";
			main += "{\n";
			main += "	rux_native_set_on_any_exit( onexit );\n";
			main += "	if( rux::diagnostics::is_admin_user_process() == 1 )\n";
			main += "	{\n";
			main += "		declare_stack_variable( char , executable_directory , 1024 );\n";
			main += "		declare_stack_variable( char , filename , 1024 );\n";
			main += "		declare_stack_variable( char , error_filename , 1024 );\n";
			main += "		::rux::safe_strncpy( executable_directory , ::rux::engine::_globals->_executable_directory , 1024 );\n";
			main += "		rux_concatenation( filename , executable_directory , \"/rux.conf\" );\n";
			main += "		rux_concatenation( error_filename , executable_directory , \"/installer_error.log\" );\n";
			main += "		is_delete_conf = rux_is_exists_file( filename ) == 1 ? 0 : 1;\n";
			main += "		rux_concatenation( filename , executable_directory , \"/" + module_lib + rux_engine_name + module_extension + "\" );\n";
			main += "		rux::uint8 is_success = 0;\n";
			main += "		char* error_ptr = NULL;\n";
			main += "		is_success = rux::resources::XVSRCResources::ExtractInFile( filename , IDR_RUX_ENGINE , malloc , &error_ptr );\n";
			main += "		is_delete_engine = is_success;\n";
			main += "		if( is_success == 1 )\n";
			main += "		{\n";
			main += "			rux::engine::initialize( filename );\n";
			main += "			::rux::engine::_globals->_loader_globals->_rux_set_conf_property( \"diagnostic\" , \"false\" );\n";
			main += "			if( argc == 2 && strcmp( args[ 1 ] , \"update\" ) == 0 )\n";
			main += "			{\n";			
			main += "				_state = XEnum_InstallerState_Update;\n";
			main += "				rux::XString install_path , error;\n";
			main += "				rux::XRegistry registry;\n";
			main += "				install_path.set_ByRef( registry.get_RegValueAsString( XEnum_PredefinedRegKey_HKEY_LOCAL_MACHINE , \"Software\\\\" + registry_name + "\" , \"InstallDir\" , error ) );\n";
			main += "				if( error.Length() > 0 )\n";
			main += "					install_path.Clear();\n";
			main += "				else if( rux::io::XDirectory::Exists( install_path ) == 0 )\n";
			main += "					install_path.Clear();\n";
			main += "				rux::uint8 is_installed = install_path.Length() == 0 ? 0 : 1;\n";
			main += "				if( is_installed == 1 )\n";
			main += "				{\n";	
			main += "					declare_stack_variable( char , error_string , 1024 );\n";
			main += "					rux_uninstall_software( error_string );\n";
			main += "					if( error_string[ 0 ] != '\\0' )\n";
			main += "					{\n";		
			main += "						size_t length = strlen( error_string );\n";
			main += "						error_ptr = (char*)malloc( length + 1 );\n";
			main += "						memcpy( error_ptr , error_string , length + 1 );\n";		
			main += "					}\n";	
			main += "					else\n";	
			main += "						is_uninstall = 1;\n";	
			main += "				}\n";
			main += "			}\n";			
			main += "			else\n";
			main += "			{\n";			
			main += "				rux::XString install_path , error;\n";
			main += "				rux::XRegistry registry;\n";
			main += "				install_path.set_ByRef( registry.get_RegValueAsString( XEnum_PredefinedRegKey_HKEY_LOCAL_MACHINE , \"Software\\\\" + registry_name + "\" , \"InstallDir\" , error ) );\n";
			main += "				if( error.Length() > 0 )\n";
			main += "					install_path.Clear();\n";
			main += "				else if( rux::io::XDirectory::Exists( install_path ) == 0 )\n";
			main += "					install_path.Clear();\n";
			main += "				rux::uint8 is_installed = install_path.Length() == 0 ? 0 : 1;\n";
			main += "				if( is_installed == 1 )\n";
			main += "				{\n";
			main += "					rux::XString answer;\n";
			main += "					rux::XConsole console;\n";
			main += "					do\n";
			main += "					{\n";
			main += "						rux::XConsole::Print( stdout , XWhite , rux::XString( \"Вы уверены,что хотите удалить программу?Выберите действие ( 1 или 2 )\\n1.Удалить программу\\n2.Отменить установку\\n\" , XEnumCodePage_UTF8 ) );\n";
			main += "						answer.set_ByRef( console.Read() );\n";
			main += "					}\n";
			main += "					while( answer != \"1\" && answer != \"2\" );\n";
			main += "					if( answer == \"1\" )\n";
			main += "					{\n";
			main += "						do\n";
			main += "						{\n";
			main += "							rux::XConsole::Print( stdout , XWhite , rux::XString( \"Вы хотите удалить файлы конфигурации?Выберите действие ( 1 или 2 )\\n1.Удалить файлы конфигурации\\n2.Не удалять файлы конфигурации\\n\" , XEnumCodePage_UTF8 ) );\n";
			main += "							answer.set_ByRef( console.Read() );\n";
			main += "						}\n";
			main += "						while( answer != \"1\" && answer != \"2\" );\n";
			main += "						_is_uninstall_configuration = answer == \"1\" ? 1 : 0;\n";
			main += "						_state = XEnum_InstallerState_UnInstall;\n";
			main += "						declare_stack_variable( char , error_string , 1024 );\n";
			main += "						rux_uninstall_software( error_string );\n";
			main += "						if( error_string[ 0 ] != '\\0' )\n";
			main += "						{\n";	
			main += "							do\n";
			main += "							{\n";
			main += "								rux::XConsole::Print( stdout , XWhite , rux::XString( \"При удалении программы возникла ошибка.Желаете принудительно удалить программу?Выберите действие ( 1 или 2 )\\n1.Принудительно удалить программу\\n2.Отменить установку\\n\" , XEnumCodePage_UTF8 ) );\n";
			main += "								answer.set_ByRef( console.Read() );\n";
			main += "							}\n";
			main += "							while( answer != \"1\" && answer != \"2\" );\n";
			main += "							if( answer == \"1\" )\n";
			main += "							{\n";
			main += "								if( install_path.Length() > 0 )\n";
			main += "								{\n";	
			main += "									rux::XString ansi_install_path;\n";	
			main += "									ansi_install_path.set_ByRef( install_path.ConvertToAnsi() );\n";	
			main += "									if( ansi_install_path.Size() < 1024 )\n";
			main += "										memcpy( _install_path , ansi_install_path.str() , ansi_install_path.Size() );\n";	
			main += "								}\n";
			main += "								rux_force_uninstall_software();\n";
			main += "								is_uninstall = 1;\n";	
			main += "							}\n";
			main += "							else\n";
			main += "							{\n";
			main += "								size_t length = strlen( error_string );\n";
			main += "								error_ptr = (char*)malloc( length + 1 );\n";
			main += "								memcpy( error_ptr , error_string , length + 1 );\n";
			main += "							}\n";
			main += "						}\n";
			main += "						else\n";	
			main += "							is_uninstall = 1;\n";	
			main += "					}\n";	
			main += "				}\n";
			main += "				else\n";
			main += "				{\n";	
			main += "					rux::XConsole::Print( stderr , XRed , rux::XString( \"Программа не установлена\\n\" , XEnumCodePage_UTF8 ) );\n";	
			main += "					rux::XConsole::Print( stdout , XWhite , rux::XString( \"Нажмите <ENTER>\" , XEnumCodePage_UTF8 ) );\n";
			main += "					rux::XConsole::WaitForEnter();\n";
			main += "				}\n";
			main += "			}\n";			
			main += "			if( error_ptr && error_ptr[ 0 ] != '\\0' )\n";
			main += "			{\n";
			main += "				rux::XConsole::Print( stderr , XRed , rux::XString( \"\\nУстановка программы прервана из-за ошибки '\" , XEnumCodePage_UTF8 ) + error_ptr + \"'\\n\" );\n";
			main += "				rux::uint64 now = rux_get_now_as_local_unix_time();\n";
			main += "				declare_stack_variable( char , now_string , 64 );\n";
			main += "				rux_unix_time_to_string( now , now_string , 64 );\n";
			main += "				declare_stack_variable( char , lines , 4096 );\n";
			main += "				::rux::safe_sprintf( lines , \"%s : uninstaller error '%s'\\n\" , now_string , error_ptr );\n";
			main += "				rux_append_to_file( error_filename , lines );\n";
			main += "				rux::XConsole::Print( stdout , XWhite , rux::XString( \"Нажмите <ENTER>\" , XEnumCodePage_UTF8 ) );\n";
			main += "				rux::XConsole::WaitForEnter();\n";
			main += "			}\n";
			main += "		}\n";
			main += "		else\n";
			main += "		{\n";	
			main += "			rux::XConsole::Print( stderr , XRed , rux::XString( \"\\nУстановка программы прервана из-за ошибки '\" , XEnumCodePage_UTF8 ) + error_ptr + \"'\\n\" );\n";	
			main += "			rux::uint64 now = rux_get_now_as_local_unix_time();\n";
			main += "			declare_stack_variable( char , now_string , 64 );\n";
			main += "			rux_unix_time_to_string( now , now_string , 64 );\n";
			main += "			declare_stack_variable( char , lines , 4096 );\n";
			main += "			::rux::safe_sprintf( lines , \"%s : uninstaller error '%s'\\n\" , now_string , error_ptr );\n";
			main += "			rux_append_to_file( error_filename , lines );\n";
			main += "			rux::XConsole::Print( stdout , XWhite , rux::XString( \"Нажмите <ENTER>\" , XEnumCodePage_UTF8 ) );\n";
			main += "			rux::XConsole::WaitForEnter();\n";
			main += "		}\n";	
			main += "		if( is_uninstall == 1 )\n";
			main += "		{\n";
			main += "#ifdef __WINDOWS__\n";
			main += "			rux::XString bat_file( executable_directory ) , error;\n";
			main += "			bat_file += \"\\\\delself.bat\";\n";
			main += "			declare_stack_variable( char , executable_filename , 1024 );\n";
			main += "			rux_get_executable_filename( executable_filename );\n";
			main += "			rux::XString bat_text( executable_filename );\n";
			main += "			bat_text.set_ByRef( \"@echo off\\n:try\\ndel \\\"\" +  bat_text + \"\\\"\\nif exist \\\"\" + bat_text + \"\\\" goto try\\ndel \\\"\" + bat_file + \"\\\"\\nrmdir /S /Q \\\"\" + executable_directory + \"\\\"\" );\n";
			main += "			rux::io::XFile bat = rux::io::XFile::Create( bat_file , error );\n";
			main += "			if( bat.Exists() == 1 )\n";
			main += "			{\n";
			main += "				bat.AppendText( bat_text , error );\n";
			main += "				bat.Close();\n";
			main += "			}\n";
			main += "#endif\n";
			main += "		}\n";
			main += "		rux::deinitialize();\n";	
			main += "		if( is_delete_engine == 1 || is_uninstall == 1 )\n";
			main += "		{\n";
			main += "			rux_concatenation( filename , executable_directory , \"/" + module_lib + rux_engine_name + module_extension + "\" );\n";
			main += "			rux_remove_file( filename );\n";
			main += "		}\n";	
			main += "		if( is_delete_conf == 1 || is_uninstall == 1 )\n";
			main += "		{\n";
			main += "			rux_concatenation( filename , executable_directory , \"/rux.conf\" );\n";
			main += "			rux_remove_file( filename );\n";
			main += "		}\n";
			main += "		if( error_ptr )\n";
			main += "			free( error_ptr );\n";		
			main += "		if( is_uninstall == 1 )\n";
			main += "		{\n";
			main += "			rux_concatenation( filename , executable_directory , \"/delself.bat\" );\n";
			main += "			rux_start_detached_process( filename , \"\" , 0 );\n";
			main += "		}\n";
			main += "	}\n";
			main += "	else\n";
			main += "	{\n";	
			main += "		rux::XConsole::Print( stderr , XRed , rux::XString( \"Запустите установку программы от имени администратора системы\\n\" , XEnumCodePage_UTF8 ) );\n";	
			main += "		rux::XConsole::Print( stdout , XWhite , rux::XString( \"Нажмите <ENTER>\" , XEnumCodePage_UTF8 ) );\n";
			main += "		rux::XConsole::WaitForEnter();\n";
			main += "	}\n";
			main += "	return 0;\n";
			main += "};\n";
			return main++;
		};
		rux::String& XInstallerGenerator::GenerateInstallerCppMain( const rux::XString& registry_name ,
			const rux::XString& output_installer_name , const rux::XString& os , const rux::XString& product_name , 
			rux::XArray< XInstallerAction >& install_actions , rux::XArray< XInstallerAction >& uninstall_actions , 
			rux::XArray< XInstallerAction >& update_actions , rux::data::XDataObject& variables )
		{
			rux::XString module_extension , module_lib , name , rux_engine_name;
			rux::uint64 time = rux::XTime::get_NowAsUnixTime();
			rux_engine_name.set_ByRef( rux::XUInt64( time ).ToString() + "." );
			if( os == "windows" )
				module_extension = "dll";
			else
			{
				module_extension = "so";
				module_lib = "lib";
			}
			rux::XString main;
			main += "#ifdef HAVE_CONFIG_H\n";
			main += "#include <config.h>\n";
			main += "#endif\n";
			main += "#include <resource.h>\n";
			main += "#include <rux.h>\n";
			main += "#include <xan_path.h>\n";
			main += "#include <xan_vsrcresources.h>\n";
			main += "#include <xan_file.h>\n";
			main += "#include <xan_directory.h>\n";
			main += "#include <xan_registry.h>\n";
			main += "#include <xan_console.h>\n";	
			main += "#include <xan_service.h>\n";	
			main += "#include <xan_boolean.h>\n";
			main += "#include <xan_loader_defines.h>\n";
			main += "enum XEnum_InstallerState\n";
			main += "{\n";
			main += "	XEnum_InstallerState_Install ,\n";
			main += "	XEnum_InstallerState_UnInstall ,\n";
			main += "	XEnum_InstallerState_Update ,\n";
			main += "	XEnum_InstallerState_Cancel\n";
			main += "};\n";
			main += "dll_internal XEnum_InstallerState _state = XEnum_InstallerState_Install;\n";
			main += "const char* g_current_module_name = \"" + output_installer_name + "\";\n";
			main += "dll_internal rux::uint8 _rux_is_exited = 0;\n";
			main += "dll_internal rux::uint8 _is_uninstall_configuration = 1;\n";
			main += "dll_internal XCrtSect _cs_rux_current_module;\n";
			main += "dll_internal char _install_path[ 1024 ] = {0};\n";
			main += "rux::uint8 rux_install_software( const char* install_path , char error[ 1024 ] )\n";
			main += "{\n";
			XObject value;
			for( rux::uint32 index0 = 0 ; index0 < variables.FieldsCount() ; index0++ )
			{
				name.set_ByRef( variables.get_FieldName( index0 ) );
				value = variables.get_FieldValue( index0 );
				if( rux_is_object( value , rux::XBoolean ) )
				{
					rux::XBoolean boolean_value( value );
					main += "	rux::XBoolean " + name + "( " + boolean_value.ToString() + " );\n";
				}
				else if( rux_is_object( value , rux::XString ) )
				{
					rux::XString string_value;
					string_value.set_ByRef( value );
					XInstallerAction::correct_string( string_value );
					main += "	rux::XString " + name + "( \"" + string_value + "\" , XEnumCodePage_UTF8 );\n";
				}
				else if( rux_is_object( value , rux::XInt64 ) )
				{
					rux::XInt64 int64_value( value );
					main += "	rux::XInt64 " + name + "( " + int64_value.ToString() + " );\n";
				}
			}
			main += "	error[ 0 ] = '\\0';\n";
			main += "	rux::XString error_string , install_dir_string( install_path );\n";
			main += "	install_dir_string.set_ByRef( rux::io::XDirectory::TransformToRuxDirectoryName( install_dir_string ) );\n";
			main += "	rux::XString start_menu_programs_string;\n";
			main += "	start_menu_programs_string.set_ByRef( rux::io::XDirectory::get_SpecialFolderPath( \"START_MENU_PROGRAMS\" , error_string ) );\n";
			main += "	if( error_string.Length() == 0 )\n";
			main += "	{\n";
			for( rux::uint32 index0 = 0 ; index0 < variables.FieldsCount() ; index0++ )
			{
				name.set_ByRef( variables.get_FieldName( index0 ) );
				value = variables.get_FieldValue( index0 );
				if( rux_is_object( value , rux::XString ) )
					main += XInstallerAction::add_process_string_to_cpp( name , variables , 2 );
			}
			for( rux::uint32 index0 = 0 ; index0 < install_actions.Count() ; index0++ ) 
			{
				main += "		if( error_string.Length() == 0 && _state != XEnum_InstallerState_Cancel )\n";
				main += "		{\n";
				main += install_actions[ index0 ].ToCppCode( product_name , variables );
				main += "		}\n";
			}
			main += "	}\n";	
			main +=	"	if( error_string.Length() == 0 && _state != XEnum_InstallerState_Cancel )\n";
			main += "	{\n";
			main += "		if( rux::io::XDirectory::Exists( start_menu_programs_string + \"/" + product_name + "\" ) == 0 )\n";
			main += "			rux::io::XDirectory::Create( start_menu_programs_string + \"/" + product_name + "\" );\n";
			main += "		rux::io::XDirectory::CreateLink( install_dir_string + \"/uninstaller.exe\" , start_menu_programs_string + \"/" + product_name + "/UnInstall.lnk\" , \"UnInstall '" + product_name + "'\" , \"\" , install_dir_string );\n";
			main += "		rux::XRegistry registry;\n";
			main += "		registry.CreateRegKey( XEnum_PredefinedRegKey_HKEY_LOCAL_MACHINE , \"Software\\\\" + registry_name + "\" , error_string );\n";
			main += "		if( error_string.Length() == 0 )\n";
			main += "			registry.set_RegValueAsString( XEnum_PredefinedRegKey_HKEY_LOCAL_MACHINE , \"Software\\\\" + registry_name + "\" , \"InstallDir\" , install_dir_string , error_string );\n";		
			main += "	}\n";
			main +=	"	if( error_string.Length() > 0 || _state == XEnum_InstallerState_Cancel )\n";
			main += "	{\n";
			main += "		error_string.set_ByRef( error_string.ConvertToAnsi() );\n";
			main += "		if( error_string.Size() > 1024 )\n";
			main += "		{\n";
			main += "			memcpy( error , error_string.str() , 1023 );\n";
			main += "			error[ 1023 ] = '\\0';\n";
			main += "		}\n";
			main += "		else\n";
			main += "			memcpy( error , error_string.str() , error_string.Size() );\n";	
			for( rux::uint32 index0 = 0 ; index0 < uninstall_actions.Count() ; index0++ ) 
			{
				main += "		if( _state != XEnum_InstallerState_Update )\n";
				main += "		{\n";
				main += uninstall_actions[ index0 ].ToCppCode( product_name , variables , 1 );
				main += "		}\n";
			}
			main += "		if( _state != XEnum_InstallerState_Update )\n";
			main += "		{\n";
			main += "			if( rux::io::XDirectory::Exists( start_menu_programs_string + \"/" + product_name + "\" ) == 1 )\n";
			main += "				rux::io::XDirectory::Delete( start_menu_programs_string + \"/" + product_name + "\" , XAllDirectories );\n";
			main += "			rux::io::XDirectory::Delete( install_dir_string , XAllDirectories );\n";
			main += "			rux::XRegistry registry;\n";
			main += "			registry.DeleteRegKey( XEnum_PredefinedRegKey_HKEY_LOCAL_MACHINE , \"Software\\\\" + registry_name + "\" , error_string );\n";	
			main += "		}\n";
			main += "	}\n";
			main += "	return error[ 0 ] == '\\0' ? 1 : 0;\n";
			main += "};\n";
			main += "rux::uint8 rux_uninstall_software( char error[ 1024 ] )\n";
			main += "{\n";	
			for( rux::uint32 index0 = 0 ; index0 < variables.FieldsCount() ; index0++ )
			{
				name.set_ByRef( variables.get_FieldName( index0 ) );
				value = variables.get_FieldValue( index0 );
				if( rux_is_object( value , rux::XBoolean ) )
				{
					rux::XBoolean boolean_value( value );
					main += "	rux::XBoolean " + name + "( " + boolean_value.ToString() + " );\n";
				}
				else if( rux_is_object( value , rux::XString ) )
				{
					rux::XString string_value;
					string_value.set_ByRef( value );
					XInstallerAction::correct_string( string_value );
					main += "	rux::XString " + name + "( \"" + string_value + "\" , XEnumCodePage_UTF8 );\n";
				}
				else if( rux_is_object( value , rux::XInt64 ) )
				{
					rux::XInt64 int64_value( value );
					main += "	rux::XInt64 " + name + "( " + int64_value.ToString() + " );\n";
				}
			}
			main += "	error[ 0 ] = '\\0';\n";
			main += "	rux::XString error_string , install_dir_string;\n";
			main += "	rux::XRegistry registry;\n";
			main += "	install_dir_string.set_ByRef( registry.get_RegValueAsString( XEnum_PredefinedRegKey_HKEY_LOCAL_MACHINE , \"Software\\\\" + registry_name + "\" , \"InstallDir\" , error_string ) );\n";
			main += "	install_dir_string.set_ByRef( rux::io::XDirectory::TransformToRuxDirectoryName( install_dir_string ) );\n";
			main += "	rux::XString start_menu_programs_string;\n";
			main += "	start_menu_programs_string.set_ByRef( rux::io::XDirectory::get_SpecialFolderPath( \"START_MENU_PROGRAMS\" , error_string ) );\n";	
			main += "	if( error_string.Length() == 0 )\n";
			main += "	{\n";
			for( rux::uint32 index0 = 0 ; index0 < variables.FieldsCount() ; index0++ )
			{
				name.set_ByRef( variables.get_FieldName( index0 ) );
				value = variables.get_FieldValue( index0 );
				if( rux_is_object( value , rux::XString ) )
					main += XInstallerAction::add_process_string_to_cpp( name , variables , 2 );
			}
			for( rux::uint32 index0 = 0 ; index0 < uninstall_actions.Count() ; index0++ ) 
			{
				main += "		if( error_string.Length() == 0 && _state != XEnum_InstallerState_Cancel )\n";
				main += "		{\n";
				main += uninstall_actions[ index0 ].ToCppCode( product_name , variables );
				main += "		}\n";
			}
			main += "	}\n";
			main += "	if( error_string.Length() == 0 && _state != XEnum_InstallerState_Cancel && _is_uninstall_configuration == 1 )\n";
			main += "	{\n";
			main += "		if( rux::io::XDirectory::Exists( start_menu_programs_string + \"/" + product_name + "\" ) == 1 )\n";
			main += "			rux::io::XDirectory::Delete( start_menu_programs_string + \"/" + product_name + "\" , XAllDirectories );\n";
			main += "		rux::io::XDirectory::Delete( install_dir_string , XAllDirectories );\n";
			main += "		rux::XRegistry registry;\n";
			main += "		registry.DeleteRegKey( XEnum_PredefinedRegKey_HKEY_LOCAL_MACHINE , \"Software\\\\" + registry_name + "\" , error_string );\n";
			main += "	}\n";
			main += "	else if( error_string.Length() == 0 && _is_uninstall_configuration == 0 )\n";
			main += "	{\n";
			main += "		if( rux::io::XFile::Exists( install_dir_string + \"/uninstaller.exe\" ) == 1 )\n";
			main += "		{\n";
			main += "			rux::XString exe( install_dir_string + \"/uninstaller.exe\"  );\n";
			main += "			exe.set_ByRef( exe.ConvertToAnsi() );\n";
			main += "			rux::uint8 result = rux::diagnostics::start_process( exe.str() , \"update\" );\n";
			main += "			if( result == 0 )\n";
			main += "				error_string = \"rux::diagnostics::start_process '\" + exe + \"' is failed\";\n";
			main += "		}\n";
			main += "		else if( rux::io::XFile::Exists( install_dir_string + \"/video-server-uninstaller.exe\" ) == 1 )\n";
			main += "		{\n";
			main += "			rux::XString exe( install_dir_string + \"/video-server-uninstaller.exe\"  );\n";
			main += "			exe.set_ByRef( exe.ConvertToAnsi() );\n";
			main += "			rux::uint8 result = rux::diagnostics::start_process( exe.str() , \"update\" );\n";
			main += "			if( result == 0 )\n";
			main += "				error_string = \"rux::diagnostics::start_process '\" + exe + \"' is failed\";\n";
			main += "		}\n";
			main += "		else\n";
			main += "			rux::io::XDirectory::Delete( install_dir_string , XAllDirectories );\n";
			main += "		if( rux::io::XDirectory::Exists( start_menu_programs_string + \"/" + product_name + "\" ) == 1 )\n";
			main += "			rux::io::XDirectory::Delete( start_menu_programs_string + \"/" + product_name + "\" , XAllDirectories );\n";	
			main += "		rux::XRegistry registry;\n";
			main += "		registry.DeleteRegKey( XEnum_PredefinedRegKey_HKEY_LOCAL_MACHINE , \"Software\\\\" + registry_name + "\" , error_string );\n";
			main += "	}\n";
			main +=	"	if( error_string.Length() > 0 )\n";
			main += "	{\n";
			main += "		error_string.set_ByRef( error_string.ConvertToAnsi() );\n";
			main += "		if( error_string.Size() > 1024 )\n";
			main += "		{\n";
			main += "			memcpy( error , error_string.str() , 1023 );\n";
			main += "			error[ 1023 ] = '\\0';\n";
			main += "		}\n";
			main += "		else\n";
			main += "			memcpy( error , error_string.str() , error_string.Size() );\n";
			main += "	}\n";
			main += "	return error_string.Length() > 0 ? 0 : 1;\n";
			main += "};\n";	
			main += "rux::uint8 rux_update_software( char error[ 1024 ] )\n";
			main += "{\n";	
			main += "	error[ 0 ] = '\\0';\n";
			main += "	rux::XString error_string , install_dir_string;\n";
			main += "	rux::XRegistry registry;\n";
			main += "	install_dir_string.set_ByRef( registry.get_RegValueAsString( XEnum_PredefinedRegKey_HKEY_LOCAL_MACHINE , \"Software\\\\" + registry_name + "\" , \"InstallDir\" , error_string ) );\n";
			main += "	install_dir_string.set_ByRef( rux::io::XDirectory::TransformToRuxDirectoryName( install_dir_string ) );\n";
			main += "	rux::XString start_menu_programs_string;\n";
			main += "	start_menu_programs_string.set_ByRef( rux::io::XDirectory::get_SpecialFolderPath( \"START_MENU_PROGRAMS\" , error_string ) );\n";
			main += "	if( error_string.Length() == 0 )\n";
			main += "	{\n";
			main += "		if( rux::io::XFile::Exists( install_dir_string + \"/uninstaller.exe\" ) == 1 )\n";
			main += "		{\n";
			main += "			rux::XString exe( install_dir_string + \"/uninstaller.exe\"  );\n";
			main += "			exe.set_ByRef( exe.ConvertToAnsi() );\n";
			main += "			rux::uint8 result = rux::diagnostics::start_process( exe.str() , \"update\" );\n";
			main += "			if( result == 0 )\n";
			main += "				error_string = \"rux::diagnostics::start_process '\" + exe + \"' is failed\";\n";
			main += "		}\n";
			main += "		else if( rux::io::XFile::Exists( install_dir_string + \"/video-server-uninstaller.exe\" ) == 1 )\n";
			main += "		{\n";
			main += "			rux::XString exe( install_dir_string + \"/video-server-uninstaller.exe\"  );\n";
			main += "			exe.set_ByRef( exe.ConvertToAnsi() );\n";
			main += "			rux::uint8 result = rux::diagnostics::start_process( exe.str() , \"update\" );\n";
			main += "			if( result == 0 )\n";
			main += "				error_string = \"rux::diagnostics::start_process '\" + exe + \"' is failed\";\n";
			main += "		}\n";
			main += "		else\n";
			main += "		{\n";
			main += "			rux_uninstall_software( error );\n";
			main += "			if( strlen( error )  > 0 )\n";
			main += "				error_string = error;\n";
			main += "		}\n";
			main += "	}\n";
			main += "	if( error_string.Length() == 0 )\n";
			main += "	{\n";
			main += "		install_dir_string.set_ByRef( install_dir_string.ConvertToAnsi() );\n";
			main += "		rux_install_software( install_dir_string.str() , error );\n";
			main += "		if( strlen( error )  > 0 )\n";
			main += "			error_string = error;\n";
			main += "	}\n";
			main +=	"	if( error_string.Length() > 0 )\n";
			main += "	{\n";
			main += "		error_string.set_ByRef( error_string.ConvertToAnsi() );\n";
			main += "		if( error_string.Size() > 1024 )\n";
			main += "		{\n";
			main += "			memcpy( error , error_string.str() , 1023 );\n";
			main += "			error[ 1023 ] = '\\0';\n";
			main += "		}\n";
			main += "		else\n";
			main += "			memcpy( error , error_string.str() , error_string.Size() );\n";
			main += "	}\n";
			main += "	return error_string.Length() > 0 ? 0 : 1;\n";
			main += "};\n";
			main += "void	rux_force_uninstall_software( void )\n";
			main += "{\n";	
			main += "	if( _install_path[ 0 ] != '\\0' )\n";
			main += "	{\n";
			for( rux::uint32 index0 = 0 ; index0 < variables.FieldsCount() ; index0++ )
			{
				name.set_ByRef( variables.get_FieldName( index0 ) );
				value = variables.get_FieldValue( index0 );
				if( rux_is_object( value , rux::XBoolean ) )
				{
					rux::XBoolean boolean_value( value );
					main += "		rux::XBoolean " + name + "( " + boolean_value.ToString() + " );\n";
				}
				else if( rux_is_object( value , rux::XString ) )
				{
					rux::XString string_value;
					string_value.set_ByRef( value );
					XInstallerAction::correct_string( string_value );
					main += "		rux::XString " + name + "( \"" + string_value + "\" , XEnumCodePage_UTF8 );\n";
				}
				else if( rux_is_object( value , rux::XInt64 ) )
				{
					rux::XInt64 int64_value( value );
					main += "		rux::XInt64 " + name + "( " + int64_value.ToString() + " );\n";
				}
			}	
			main += "		rux::XString error_string , install_dir_string;\n";	
			main += "		install_dir_string = _install_path;\n";
			main += "		install_dir_string.set_ByRef( rux::io::XDirectory::TransformToRuxDirectoryName( install_dir_string ) );\n";
			main += "		rux::XString start_menu_programs_string;\n";
			main += "		start_menu_programs_string.set_ByRef( rux::io::XDirectory::get_SpecialFolderPath( \"START_MENU_PROGRAMS\" , error_string ) );\n";		
			for( rux::uint32 index0 = 0 ; index0 < variables.FieldsCount() ; index0++ )
			{
				name.set_ByRef( variables.get_FieldName( index0 ) );
				value = variables.get_FieldValue( index0 );
				if( rux_is_object( value , rux::XString ) )
					main += XInstallerAction::add_process_string_to_cpp( name , variables , 2 );
			}
			for( rux::uint32 index0 = 0 ; index0 < uninstall_actions.Count() ; index0++ ) 
			{
				main += "		{\n";
				main += uninstall_actions[ index0 ].ToCppCode( product_name , variables );
				main += "		}\n";
			}
			main += "		if( rux::io::XDirectory::Exists( start_menu_programs_string + \"/" + product_name + "\" ) == 1 )\n";
			main += "			rux::io::XDirectory::Delete( start_menu_programs_string + \"/" + product_name + "\" , XAllDirectories );\n";
			main += "		rux::io::XDirectory::Delete( install_dir_string , XAllDirectories );\n";
			main += "		rux::XRegistry registry;\n";
			main += "		registry.DeleteRegKey( XEnum_PredefinedRegKey_HKEY_LOCAL_MACHINE , \"Software\\\\" + registry_name + "\" , error_string );\n";	
			main += "	}\n";
			main += "};\n";	
			main += "rux::uint8 is_delete_engine = 0 , is_delete_conf = 0;\n";
			main += "void onexit( void )\n";
			main += "{\n";
			main += "	declare_stack_variable( char , executable_directory , 1024 );\n";
			main += "	declare_stack_variable( char , filename , 1024 );\n";
			main += "	::rux::safe_strncpy( executable_directory , ::rux::engine::_globals->_executable_directory , 1024 );\n";	
			main += "	rux::deinitialize();\n";	
			main += "	if( is_delete_engine == 1 )\n";
			main += "	{\n";
			main += "		rux_concatenation( filename , executable_directory , \"/" + module_lib + rux_engine_name + module_extension + "\" );\n";
			main += "		rux_remove_file( filename );\n";
			main += "	}\n";	
			main += "	if( is_delete_conf == 1 )\n";
			main += "	{\n";
			main += "		rux_concatenation( filename , executable_directory , \"/rux.conf\" );\n";
			main += "		rux_remove_file( filename );\n";
			main += "	}\n";
			main += "}\n";
			main += "int main( int argc , char** args )\n";
			main += "{\n";
			main += "	rux_native_set_on_any_exit( onexit );\n";
			main += "	rux::uint8 force_update = argc == 2 && strcmp( args[ 1 ] , \"--force-update\" ) == 0 ? 1 : 0;\n";
			main += "	if( rux::diagnostics::is_admin_user_process() == 1 )\n";
			main += "	{\n";
			main += "		declare_stack_variable( char , executable_directory , 1024 );\n";
			main += "		declare_stack_variable( char , filename , 1024 );\n";
			main += "		declare_stack_variable( char , error_filename , 1024 );\n";
			main += "		::rux::safe_strncpy( executable_directory , ::rux::engine::_globals->_executable_directory , 1024 );\n";	
			main += "		rux_concatenation( filename , executable_directory , \"/rux.conf\" );\n";
			main += "		rux_concatenation( error_filename , executable_directory , \"/installer_error.log\" );\n";
			main += "		rux::uint8 is_success = 0;\n";
			main += "		is_delete_engine = 0 , is_delete_conf = rux_is_exists_file( filename ) == 1 ? 0 : 1;\n";
			main += "		char* error_ptr = NULL;\n";
			main += "		rux_concatenation( filename , executable_directory , \"/" + module_lib + rux_engine_name + module_extension + "\" );\n";
			main += "		is_success = rux::resources::XVSRCResources::ExtractInFile( filename , IDR_RUX_ENGINE , malloc , &error_ptr );\n";
			main += "		if( is_success == 1 )\n";
			main += "		{\n";
			main += "			rux::engine::initialize( filename );\n";
			main += "			::rux::engine::_globals->_loader_globals->_rux_set_conf_property( \"diagnostic\" , \"false\" );\n";
			main += "			is_delete_engine = 1;\n";
			main += "			rux::XString install_path , error;\n";
			main += "			rux::XRegistry registry;\n";
			main += "			install_path.set_ByRef( registry.get_RegValueAsString( XEnum_PredefinedRegKey_HKEY_LOCAL_MACHINE , \"Software\\\\" + registry_name + "\" , \"InstallDir\" , error ) );\n";
			main += "			if( error.Length() > 0 )\n";
			main += "				install_path.Clear();\n";
			main += "			else if( rux::io::XDirectory::Exists( install_path ) == 0 )\n";
			main += "				install_path.Clear();\n";
			main += "			rux::uint8 is_installed = install_path.Length() == 0 ? 0 : 1;\n";
			main += "			if( is_installed == 1 )\n";
			main += "			{\n";
			main += "				rux::XString answer;\n";
			main += "				rux::XConsole console;\n";
			main += "				if( force_update == 0 )\n";
			main += "				{\n";
			main += "					do\n";
			main += "					{\n";
			main += "						rux::XConsole::Print( stdout , XWhite , rux::XString( \"Программа уже установлена.Выберите действие ( 1 , 2 или 3 )\\n1.Обновить программу\\n2.Удалить программу\\n3.Отменить установку\\n\" , XEnumCodePage_UTF8 ) );\n";
			main += "						answer.set_ByRef( console.Read() );\n";
			main += "					}\n";
			main +=	"					while( answer != \"1\" && answer != \"2\" && answer != \"3\" );\n";
			main += "				}\n";
			main += "				else\n";
			main += "					answer = \"1\";\n";
			main += "				if( answer == \"1\" )\n";
			main += "				{\n";
			main += "					_state = XEnum_InstallerState_Update;\n";
			main += "					declare_stack_variable( char , error_string , 1024 );\n";
			main += "					rux_update_software( error_string );\n";
			main += "					if( error_string[ 0 ] != '\\0' )\n";
			main += "					{\n";
			main += "						size_t length = strlen( error_string );\n";
			main += "						error_ptr = (char*)malloc( length + 1 );\n";
			main += "						memcpy( error_ptr , error_string , length + 1 );\n";
			main += "					}\n";
			main += "				}\n";
			main += "				else if( answer == \"2\" )\n";
			main += "				{\n";
			main += "					do\n";
			main += "					{\n";
			main += "						rux::XConsole::Print( stdout , XWhite , rux::XString( \"Вы уверены,что хотите удалить программу?Выберите действие ( 1 или 2 )\\n1.Удалить программу\\n2.Отменить установку\\n\" , XEnumCodePage_UTF8 ) );\n";
			main += "						answer.set_ByRef( console.Read() );\n";
			main += "					}\n";
			main += "					while( answer != \"1\" && answer != \"2\" );\n";
			main += "					if( answer == \"1\" )\n";
			main += "					{\n";
			main += "						do\n";
			main += "						{\n";
			main += "							rux::XConsole::Print( stdout , XWhite , rux::XString( \"Вы хотите удалить файлы конфигурации?Выберите действие ( 1 или 2 )\\n1.Удалить файлы конфигурации\\n2.Не удалять файлы конфигурации\\n\" , XEnumCodePage_UTF8 ) );\n";
			main += "							answer.set_ByRef( console.Read() );\n";
			main += "						}\n";
			main += "						while( answer != \"1\" && answer != \"2\" );\n";
			main += "						_is_uninstall_configuration = answer == \"1\" ? 1 : 0;\n";
			main += "						_state = XEnum_InstallerState_UnInstall;\n";
			main += "						declare_stack_variable( char , error_string , 1024 );\n";
			main += "						rux_uninstall_software( error_string );\n";
			main += "						if( error_string[ 0 ] != '\\0' )\n";
			main += "						{\n";	
			main += "							do\n";
			main += "							{\n";
			main += "								rux::XConsole::Print( stdout , XWhite , rux::XString( \"При удалении программы возникла ошибка.Желаете принудительно удалить программу?Выберите действие ( 1 или 2 )\\n1.Принудительно удалить программу\\n2.Отменить установку\\n\" , XEnumCodePage_UTF8 ) );\n";
			main += "								answer.set_ByRef( console.Read() );\n";
			main += "							}\n";
			main += "							while( answer != \"1\" && answer != \"2\" );\n";
			main += "							if( answer == \"1\" )\n";
			main += "							{\n";
			main += "								if( install_path.Length() > 0 )\n";
			main += "								{\n";	
			main += "									rux::XString ansi_install_path;\n";	
			main += "									ansi_install_path.set_ByRef( install_path.ConvertToAnsi() );\n";	
			main += "									if( ansi_install_path.Size() < 1024 )\n";
			main += "										memcpy( _install_path , ansi_install_path.str() , ansi_install_path.Size() );\n";	
			main += "								}\n";
			main += "								rux_force_uninstall_software();\n";
			main += "							}\n";
			main += "							else\n";
			main += "							{\n";
			main += "								size_t length = strlen( error_string );\n";
			main += "								error_ptr = (char*)malloc( length + 1 );\n";
			main += "								memcpy( error_ptr , error_string , length + 1 );\n";
			main += "							}\n";
			main += "						}\n";
			main += "					}\n";
			main += "				}\n";
			main += "			}\n";
			main += "			else\n";
			main += "			{\n";	
			main += "				_state = XEnum_InstallerState_Install;\n";
			main += "				declare_stack_variable( char , error_string , 1024 );\n";
			main += "				rux_install_software( install_path.str() , error_string );\n";
			main += "				if( error_string[ 0 ] != '\\0' )\n";
			main += "				{\n";
			main += "					size_t length = strlen( error_string );\n";
			main += "					error_ptr = (char*)malloc( length + 1 );\n";
			main += "					memcpy( error_ptr , error_string , length + 1 );\n";	
			main += "				}\n";
			main += "			}\n";
			main += "			if( error_ptr )\n";
			main += "			{\n";
			main += "				rux::XConsole::Print( stderr , XRed , rux::XString( \"\\nУстановка программы прервана из-за ошибки '\" , XEnumCodePage_UTF8 ) + error_ptr + \"'\\n\" );\n";
			main += "				rux::uint64 now = rux_get_now_as_local_unix_time();\n";
			main += "				declare_stack_variable( char , now_string , 64 );\n";
			main += "				rux_unix_time_to_string( now , now_string , 64 );\n";
			main += "				declare_stack_variable( char , lines , 4096 );\n";
			main += "				::rux::safe_sprintf( lines , \"%s : installer error '%s'\\n\" , now_string , error_ptr );\n";
			main += "				rux_append_to_file( error_filename , lines );\n";
			main += "			}\n";
			main += "			else\n";
			main += "			{\n";
			main += "				rux::XConsole::Print( stdout , XWhite , rux::XString( \"\\nУстановка программы успешно завершена\\n\" , XEnumCodePage_UTF8 ) );\n";
			main += "			}\n";
			main += "			rux::XConsole::Print( stdout , XWhite , rux::XString( \"Нажмите <ENTER>\" , XEnumCodePage_UTF8 ) );\n";
			main += "			if( force_update == 0 )\n";
			main += "				rux::XConsole::WaitForEnter();\n";	
			main += "		}\n";
			main += "		else\n";
			main += "		{\n";	
			main += "			rux::XConsole::Print( stderr , XRed , rux::XString( \"\\nУстановка программы прервана из-за ошибки '\" , XEnumCodePage_UTF8 ) + error_ptr + \"'\\n\" );\n";	
			main += "			rux::uint64 now = rux_get_now_as_local_unix_time();\n";
			main += "			declare_stack_variable( char , now_string , 64 );\n";
			main += "			rux_unix_time_to_string( now , now_string , 64 );\n";
			main += "			declare_stack_variable( char , lines , 4096 );\n";
			main += "			::rux::safe_sprintf( lines , \"%s : installer error '%s'\\n\" , now_string , error_ptr );\n";
			main += "			rux_append_to_file( error_filename , lines );\n";
			main += "			rux::XConsole::Print( stdout , XWhite , rux::XString( \"Нажмите <ENTER>\" , XEnumCodePage_UTF8 ) );\n";
			main += "			if( force_update == 0 )\n";
			main += "				rux::XConsole::WaitForEnter();\n";
			main += "		}\n";
			main += "		rux::deinitialize();\n";	
			main += "		if( is_delete_engine == 1 )\n";
			main += "		{\n";
			main += "			rux_concatenation( filename , executable_directory , \"/" + module_lib + rux_engine_name + module_extension + "\" );\n";
			main += "			rux_remove_file( filename );\n";
			main += "		}\n";	
			main += "		if( is_delete_conf == 1 )\n";
			main += "		{\n";
			main += "			rux_concatenation( filename , executable_directory , \"/rux.conf\" );\n";
			main += "			rux_remove_file( filename );\n";
			main += "		}\n";
			main += "		if( error_ptr )\n";
			main += "			free( error_ptr );\n";		
			main += "	}\n";
			main += "	else\n";
			main += "	{\n";	
			main += "		rux::XConsole::Print( stderr , XRed , rux::XString( \"Запустите установку программы от имени администратора системы\\n\" , XEnumCodePage_UTF8 ) );\n";	
			main += "		rux::XConsole::Print( stdout , XWhite , rux::XString( \"Нажмите <ENTER>\" , XEnumCodePage_UTF8 ) );\n";
			main += "		if( force_update == 0 )\n";
			main += "			rux::XConsole::WaitForEnter();\n";
			main += "	}\n";	
			main += "	return 0;\n";
			main += "};\n";
			return main++;
		};
	};
};