#ifndef XAN_MAKEFILE_GENERATOR_H
#define XAN_MAKEFILE_GENERATOR_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_string.h>
#include <xan_boolean.h>
#include <xan_int64.h>
namespace rux
{
	namespace generator
	{
		enum XEnum_OutputFile
		{
			XEnum_OutputFile_StaticLibrary , 
			XEnum_OutputFile_DynamicLibrary ,
			XEnum_OutputFile_Console
		};
		begin_declare_rux_class( NDKMakefileProject );
			void generate_jni( ::rux::XString& android_mk , ::rux::XString& application_mk , ::rux::XString& default_properties );
			::rux::String& get_JNIDirectory( void );
			::rux::String& get_ConfigurationName( void );
			::rux::uint8 rebuild( ::rux::XString& error , ::rux::uint8 is_printf_info = 1 );
		begin_declare_rux_class_members( NDKMakefileProject );
			::rux::XString _configuration_name;
			::rux::XString _jni_directory;
			::rux::generator::XEnum_OutputFile _output_file_type_enum;
			::rux::XArray< ::rux::XString > _sources;
			::rux::XString _ndk;
			::rux::XString _sdk;
			::rux::XString _ant;
			::rux::XString _output_directory;			
			::rux::XBoolean _debug;	
			::rux::XBoolean _rtti;
			::rux::XBoolean _exceptions;
			::rux::XBoolean _gnustl_static;
			::rux::XString _compiler_flags;
			::rux::XArray< ::rux::XString > _include_paths;
			::rux::XArray< ::rux::XString > _lib_paths;
			::rux::XArray< ::rux::XString > _libs;
			::rux::XArray< ::rux::XString > _prebuilt_static_libs;
			::rux::XArray< ::rux::XString > _preprocessors;
			::rux::XArray< ::rux::XString > _post_builds;
			::rux::XInt64 _platform;
			::rux::XString _local_module;
			::rux::XArray< ::rux::XString > _architectures;
		end_declare_rux_class_members( NDKMakefileProject );
			static NDKMakefileProject& load( const ::rux::XString configuration_name , const ::rux::XString& js , const ::rux::XString& relative_dir , ::rux::XString& error );
			void generate_jni( ::rux::XString& android_mk , ::rux::XString& application_mk , ::rux::XString& default_properties );
			::rux::String& get_JNIDirectory( void );
			::rux::String& get_ConfigurationName( void );
			::rux::uint8 rebuild( ::rux::XString& error , ::rux::uint8 is_printf_info = 1 );
		end_declare_rux_class();
		
		begin_declare_rux_class( CPPMakefileProject );
			::rux::String& generate_makefile( void );
			::rux::String& get_MakefileFilename( void );
			::rux::uint8 clean( ::rux::XString& error , ::rux::uint8 is_printf_info );
			::rux::uint8 build( ::rux::XString& error , ::rux::uint8 is_printf_info = 1 );
			::rux::uint8 rebuild( ::rux::XString& error , ::rux::uint8 is_printf_info = 1 );
			::rux::String& get_ConfigurationName( void );
		begin_declare_rux_class_members( CPPMakefileProject );
			::rux::XString _configuration_name;
			::rux::XString _makefile;
			::rux::XString _compiler;
			::rux::XString _compiler_flags;
			::rux::XString _isystem;
			::rux::XString _linker;
			::rux::XString _linker_flags;
			::rux::XString _archiver;
			::rux::XString _archiver_flags;
			::rux::XString _asm_compiler;
			::rux::XString _asm_compiler_flags;

			::rux::XArray< ::rux::XString > _exclude_files;
			::rux::XArray< ::rux::XString > _sources_files;
			::rux::XArray< ::rux::XString > _source_dirs;

			::rux::XArray< ::rux::XString > _post_builds;
			::rux::XArray< ::rux::XString > _pre_builds;
			::rux::XString _intermediate_directory;
			::rux::XString _rm_command;
			::rux::XString _mk_dir_command;
			::rux::XString _object_suffix;
			::rux::XString _source_switch;
			::rux::XString _object_switch;
			::rux::XString _include_switch;
			::rux::XString _output_switch;
			::rux::XString _lib_path_switch;
			::rux::XString _lib_switch;
			::rux::XString _preprocessor_switch;
			::rux::XArray< ::rux::XString > _include_paths;
			::rux::XArray< ::rux::XString > _lib_paths;
			::rux::XArray< ::rux::XString > _libs;
			::rux::XArray< ::rux::XString > _preprocessors;
			::rux::generator::XEnum_OutputFile _output_file_type_enum;
			::rux::XString _output_file;
			::rux::XBoolean _debug;
			::rux::XArray< ::rux::XString > _merged_libs;
			::rux::XString _ranlib;
		end_declare_rux_class_members( CPPMakefileProject );
			static CPPMakefileProject& load( const ::rux::XString configuration_name , const ::rux::XString& js , const ::rux::XString& relative_dir , ::rux::XString& error );
			rux::String& generate_makefile( void );
			::rux::String& get_MakefileFilename( void );
			::rux::uint8 clean( ::rux::XString& error , ::rux::uint8 is_printf_info = 1 );
			::rux::uint8 build( ::rux::XString& error , ::rux::uint8 is_printf_info = 1 );
			::rux::uint8 rebuild( ::rux::XString& error , ::rux::uint8 is_printf_info = 1 );
			::rux::String& get_ConfigurationName( void );
		end_declare_rux_class();
	};
};
#endif