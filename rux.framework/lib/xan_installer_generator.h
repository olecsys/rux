#ifndef XAN_INSTALLER_GENERATOR_H
#define XAN_INSTALLER_GENERATOR_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_jsonserializer.h>
namespace rux
{
	namespace generator
	{
		begin_declare_rux_class( InstallerAction );
			rux::String& ToCppCode( const rux::XString& product_name , rux::data::XDataObject& variables , rux::uint8 exclude_states = 0 );
			rux::String& get_conditions( rux::uint8 exclude_states = 0 );
			rux::String& get_variable_type( rux::XString& variable , rux::data::XDataObject& variables );
		begin_declare_rux_class_members( InstallerAction );	
		public:	
			rux::XString _type;
			rux::XString _file;	
			rux::XString _link_file;
			rux::XString _link_description;
			rux::XString _link_arguments;
			rux::XString _link_working_directory;
			rux::XString _directory;
			rux::XString _user_or_group;
			rux::XString _cmd;
			rux::XString _access;
			rux::XString _arguments;
			rux::XString _service_name;
			rux::XString _message;
			rux::XString _predefined_key;
			rux::XString _key;
			rux::XString _name;
			rux::XString _variable;
			rux::XString _value;
			rux::XString _text;
			XObject _value_object;
			rux::XArray< XObject > _answers;
			rux::XArray< XObject > _conditions;
			rux::XString _method;
			rux::XBoolean _nofatal;
			rux::XString _old_string;
			rux::XString _new_string;
		end_declare_rux_class_members( InstallerAction );
			friend class XInstallerGenerator;
			XInstallerAction( const rux::data::XDataObject& object );
			rux::String& ToCppCode( const rux::XString& product_name , rux::data::XDataObject& variables , rux::uint8 exclude_states = 0 );
			static rux::String& add_process_string_to_cpp( const rux::XString& name , rux::data::XDataObject& variables , size_t tabs_count = 4 );		
			static void correct_string( rux::XString& string );
		end_declare_rux_class();
		class XInstallerGenerator
		{
		public:
			static void GenerateInstaller( const rux::XString& product_name , const rux::XString& product_version , 
				const rux::XString& registry_name , const rux::XString& architecture , rux::XArray< rux::XString >& rux_libraries_paths , 
				const rux::XString& output_installer_name , const rux::XString& os ,
				rux::XArray< XInstallerAction >& install_actions , rux::XArray< XInstallerAction >& uninstall_actions ,
				rux::XArray< XInstallerAction >& update_actions , const rux::XString& intermediate_directory , 
				rux::XArray< rux::XString >& rux_include_paths , const rux::XString& product_files_directory , rux::data::XDataObject& variables , rux::XString& error );
		private:
			static rux::String& GenerateUnInstallerCppMain( const rux::XString& registry_name , const rux::XString& os , const rux::XString& product_name , rux::XArray< XInstallerAction >& uninstall_actions , rux::data::XDataObject& variables );
			static rux::String& GenerateInstallerCppMain( const rux::XString& registry_name , const rux::XString& output_installer_name , const rux::XString& os , const rux::XString& product_name , rux::XArray< XInstallerAction >& install_actions , rux::XArray< XInstallerAction >& uninstall_actions , rux::XArray< XInstallerAction >& update_actions , rux::data::XDataObject& variables );
		};
	};
};
#endif