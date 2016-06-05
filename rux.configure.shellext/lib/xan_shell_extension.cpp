#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_shell_extension.h>
namespace rux
{
	namespace shell
	{
		namespace extension
		{
			begin_implement_rux_com_object_class( ShellExtension )
			end_implement_rux_com_object_class();
			HRESULT STDMETHODCALLTYPE ShellExtension::Initialize( PCIDLIST_ABSOLUTE pidlFolder , IDataObject* pdtobj , HKEY hkeyProgID )
			{
				return S_FALSE;
			};
		};
	};
};