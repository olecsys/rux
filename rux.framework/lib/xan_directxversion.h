#ifndef XAN_DIRECTXVERSION_H
#define XAN_DIRECTXVERSION_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_string.h>
namespace rux
{	
	namespace diagnostics
	{	
		begin_declare_rux_class( DirectXVersion );
		begin_declare_rux_class_members( DirectXVersion );
			uint32 _version_major;
			uint32 _version_minor;
			rux::XString _version_letter;
		end_declare_rux_class_members( DirectXVersion );
			rux::String& ToString( void );
			static DirectXVersion& get_DirectXVersion( rux::XString& error );
		end_declare_rux_class();
	};
};
#endif
