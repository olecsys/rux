#ifndef XAN_CODEGENERATOR_H
#define XAN_CODEGENERATOR_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_string.h>
#include <xan_io_defines.h>
#include <xan_makefile_generator.h>
namespace rux
{
	namespace generator
	{
		begin_declare_rux_class( RUXProject );
			::rux::Array< ::rux::generator::XCPPMakefileProject >& get_CPPMakefileProjects( void );
			::rux::Array< ::rux::generator::XNDKMakefileProject >& get_NDKMakefileProjects( void );
		begin_declare_rux_class_members( RUXProject );
			::rux::XArray< ::rux::generator::XCPPMakefileProject > _cpp_makefile_projects;
			::rux::XArray< ::rux::generator::XNDKMakefileProject > _ndk_makefile_projects;
		end_declare_rux_class_members( RUXProject );
			static RUXProject& load( const ::rux::XString& ruxprj_string , ::rux::XString& error );
			::rux::Array< ::rux::generator::XCPPMakefileProject >& get_CPPMakefileProjects( void );
			::rux::Array< ::rux::generator::XNDKMakefileProject >& get_NDKMakefileProjects( void );
		end_declare_rux_class();
	};
};
#endif