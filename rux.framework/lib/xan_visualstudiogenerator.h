#ifndef XAN_VISUALSTUDIOGENERATOR_H
#define XAN_VISUALSTUDIOGENERATOR_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_string.h>
#include <xan_makefile_generator.h>
namespace rux
{
	namespace generator
	{
		begin_declare_rux_class( VisualStudioConfiguration );
			friend class XVisualStudioGenerator;
		begin_declare_rux_class_members( VisualStudioConfiguration );
			rux::XString _project_configuration;
			rux::XArray< rux::XString > _excluded_sources;
			rux::XString _architecture;
			rux::XString _release_or_debug;
			rux::XString _configuration;
			rux::XString _out_int_dir_target_name;
			rux::XString _item_definition_group;
		end_declare_rux_class_members( VisualStudioConfiguration );
		end_declare_rux_class();
		class XVisualStudioGenerator
		{
		public:
			static rux::String& GenerateVS2010CppProject( rux::XArray< XVisualStudioConfiguration >& configurations , rux::XArray< rux::XString >& sources , const rux::XString& project_directory );
			static VisualStudioConfiguration& GenerateVS2010CppProjectConfiguration( const rux::XString& platform , const rux::XString& configuration , rux::XArray< rux::XString >& excluded_sources , XEnum_OutputFile output_file_type_enum , const rux::XString& output_file , const rux::XString& project_directory , rux::XArray< rux::XString >& preprocessors , rux::XArray< rux::XString >& include_paths , rux::XArray< rux::XString >& libs , const rux::XString& post_build , const rux::XString& pre_build , rux::XArray< rux::XString >& lib_paths , rux::uint8 require_administrator );
		};
	};
};
#endif
