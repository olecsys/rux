#ifndef XAN_SHELL_EXTENSION_H
#define XAN_SHELL_EXTENSION_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_string.h>
#include <xan_com_object.h>
#include <ShlObj.h>
namespace rux
{
	namespace shell
	{
		namespace extension
		{
			declare_rux_com_object_clsid( 23 , 23 , 23 , 23 , 23 , 23 , 23 , 23 , 23 , 23 , 23 , 23 , 23 , 23 , 23 , 23 , RuxConfigureShellExtension );

			begin_declare_rux_com_interface( "D6ED4CFB-F75C-4898-9B31-5C95D27095FA" , ShellExtension )
			end_declare_rux_com_interface();

			begin_declare_rux_com_object_class_2( ShellExtension , ::rux::shell::extension::IShellExtension , IShellExtInit );
		private:
		public:
				HRESULT STDMETHODCALLTYPE Initialize( PCIDLIST_ABSOLUTE pidlFolder , IDataObject* pdtobj , HKEY hkeyProgID );
			end_declare_rux_com_object_class( ShellExtension );
		};
	};
};
#endif