#include <xan_console.h>
#include <xan_file.h>
#include <xan_directory.h>
#include <xan_rsa.h>
#include <xan_path.h>
#include <xan_zip.h>
#include <xan_jsonserializer.h>
#include <xan_loader_defines.h>
rux::int32 start( rux::XArray< rux::XString >& args )
{
	rux::engine::initialize();
	if( args.Count() % 2 != 0 )
		args.Add( rux::XString() );	
	rux::XString out_filename , silent_string , in , command;
	rux::XString error;
	for( size_t index0 = 0 ; index0 < args.Count() ; index0++ )
	{
		if( args[ index0 ] == "--command" )
		{
			command = args[ index0 + 1 ];
			args.RemoveAt( index0 );
			args.RemoveAt( index0 );
			break;
		}
	}
	if( command == "args_from_file" )
	{
		declare_stack_variable( char , dir , 1024 );
		rux_get_executable_directory( dir );
		rux_concatenation( dir , "/ruxiver.conf" );
		rux::io::XFile cmd = rux::io::XFile::Open( dir , error );
		if( cmd.Exists() == 1 )
		{
			rux::XString js;
			js.set_ByRef( cmd.ReadTextAll( error ) );
			if( error.Length() == 0 )
			{
				XObject obj( rux::data::XJsonSerializer::Deserialize( js , error ) );
				if( rux_is_object( obj , rux::data::XDataObject ) )
				{					
					rux::data::XDataObject data_obj( obj );
					for( size_t index0 = 0 ; index0 < data_obj.FieldsCount() ; index0++ )
					{
						args.Add( data_obj.get_FieldName( index0 ) );
						XObject value( data_obj.get_FieldValue( index0 ) );
						if( rux_is_object( value , rux::XArray< XObject > ) )
						{
							rux::XArray< XObject > arr;
							arr.set_ByRef( value );
							for( size_t index1 = 0 ; index1 < arr.Count() ; index1++ )
							{
								value.set_ByRef( arr[ index1 ] );
								args.Add( *value.ObjectToString() );
							}
						}
						else
							args.Add( *value.ObjectToString() );
					}
				}
			}
		}
	}
	rux::int32 result = 1;
	rux::uint8 silent = 0;
	rux::uint8 abcense_is_not_error = 1;
	declare_stack_variable( char , temp_working_directory , 1024 );
	declare_stack_variable( char , exe_dir , 1024 );
	rux_get_working_directory( temp_working_directory , 1024 );
	rux_get_executable_directory( exe_dir );
	rux::XString working_directory( temp_working_directory );	
	rux::XArray< rux::XString > in_array;
	rux::uint8 in_exists = 0;
	for( size_t index0 = 0 ; index0 < args.Count() ; index0++ )
	{			
		if( args[ index0 ] == "--in" )
		{
			index0++;			
			in.set_ByRef( args[ index0 ] );
			if( in.Length() > 0 
				&& in.get_UTF8Char( 0 ) == '.' )
				in.set_ByRef( working_directory + "/" + in );
			in.set_ByRef( in.ReplaceAll( "$(ExecutableDir)" , exe_dir ) );
			in.set_ByRef( rux::io::XPath::ExcludeRelative( in ) );			
			if( in.Length() > 0 )
				in_array.Add( in );
			in_exists = 1;
		}
		else if( args[ index0 ] == "--command" )
		{
			index0++;
			command = args[ index0 ];
			in_exists = 0;
		}
		else if( args[ index0 ] == "--out" )
		{
			index0++;
			out_filename = args[ index0 ];
			if( out_filename.Length() > 0 
				&& out_filename.get_UTF8Char( 0 ) == '.' )
				out_filename.set_ByRef( working_directory + "/" + out_filename );
			out_filename.set_ByRef( out_filename.ReplaceAll( "$(ExecutableDir)" , exe_dir ) );
			out_filename.set_ByRef( rux::io::XPath::ExcludeRelative( out_filename ) );			
			in_exists = 0;
		}
		else if( args[ index0 ] == "--silent" )
		{
			index0++;
			silent_string = args[ index0 ];
			if( silent_string.Equals( "true" , 4 ) )
				silent = 1;
			in_exists = 0;
		}
		else if( args[ index0 ] == "--abscense-is-not-error" )
		{
			index0++;
			silent_string = args[ index0 ];
			if( silent_string.Equals( "true" , 4 ) )
				abcense_is_not_error = 1;
			else
				abcense_is_not_error = 0;
			in_exists = 0;
		}
		else if( in_exists == 1 )
		{
			in.set_ByRef( args[ index0 ] );
			if( in.Length() > 0 
				&& in.get_UTF8Char( 0 ) == '.' )
				in.set_ByRef( working_directory + "/" + in );
			in.set_ByRef( in.ReplaceAll( "$(ExecutableDir)" , exe_dir ) );
			in.set_ByRef( rux::io::XPath::ExcludeRelative( in ) );			
			if( in.Length() > 0 )
				in_array.Add( in );
		}
	}				
	if( command == "compress" )
	{
		if( rux::io::XFile::Exists( out_filename ) == 0 )
		{							
			if( in_array.Count() > 0 )
			{				
				XZip::Compress( in_array , out_filename , abcense_is_not_error , error );
				if( error.Length() == 0 )
				{
					rux::XConsole::Print( stdout , XGreen , "zip archive '" + out_filename + "' is created successfully" );
					result = 0;
				}
			}
			else
				rux::XConsole::Print( stderr , XRed , "no --in file or directory" );								
		}
		else
			rux::XConsole::Print( stderr , XRed , "out file '%s' already exists" , out_filename.str() );								
	}
	if( silent == 0 )
	{
		rux::XConsole::Print( stdout , XWhite , "Press <ENTER>" );
		rux::XConsole::WaitForEnter();
	}
	return result;
};
rux_simple_console_initialize_args( "ruxiver" , start );