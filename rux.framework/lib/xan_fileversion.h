#ifndef XAN_FILEVERSION_H
#define XAN_FILEVERSION_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_string.h>
namespace rux
{	
	namespace diagnostics
	{	
		class XDirectXVersion;
		begin_declare_rux_class( FileVersion );
			friend class XDirectXVersion;
		begin_declare_rux_class_members( FileVersion );
			uint32 _major;
			uint32 _minor;
			uint32 _build;
			uint32 _revision;
		end_declare_rux_class_members( FileVersion );
			uint8 IsEmpty( void );
			uint32 get_Major( void );
			uint32 get_Minor( void );
			uint32 get_Build( void );
			uint32 get_Revision( void );
			static FileVersion& get_FileVersion( char* file_name_ptr );
			static FileVersion& get_FileVersion( const rux::XString& file_name );
			static FileVersion& get_FileVersion( char* file_name_ptr , rux::XString& error );
			static FileVersion& get_FileVersion( const rux::XString& file_name , rux::XString& error );
		end_declare_rux_class();
	};
};
#endif
