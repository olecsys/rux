#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_shell_extension.h>
#include <xan_registry.h>
bool additional_register( REFIID riid , const ::rux::XString& LibId , const ::rux::XString& ClassId )
{
	::rux::XString riid_string;
	riid_string.set_ByRef( ::rux::com::RefiidToString( riid ) );
	if( riid_string.Length() > 0 )
	{
		::rux::XArray< ::rux::XString > key_names;	

		::rux::XString error;
		::rux::XRegistry reg;

		key_names.Add( LibId + "." + ClassId );
		key_names.Add( "CLSID" );
		//reg.CreateRegKey( XEnum_PredefinedRegKey_HKEY_CLASSES_ROOT , key_names , error );
		if( error.Length() == 0 )
		{
			//reg.set_RegValueAsString( XEnum_PredefinedRegKey_HKEY_CLASSES_ROOT , key_names , "" , riid_string , error );
			if( error.Length() == 0 )
			{
			}
		}
	}
	return true;
};
bool additional_unregister( REFIID riid , const ::rux::XString& LibId , const ::rux::XString& ClassId )
{
	return true;
};

dll_internal const char* g_current_module_name = "rux.configure.shellext"dynamic_library_extension;
STDAPI DllGetClassObject( REFCLSID rclsid , REFIID riid , LPVOID* ppvOut )
{
	*ppvOut = NULL;
    if( IsEqualIID( rclsid , ::rux::shell::extension::CLSID_RuxConfigureShellExtension ) )
    {
		::rux::shell::extension::ShellExtensionFactory* factory = ::rux::engine::alloc_object< ::rux::shell::extension::ShellExtensionFactory >();
		return factory->QueryInterface( riid , ppvOut );
    }
    return CLASS_E_CLASSNOTAVAILABLE;
};
STDAPI DllCanUnloadNow(void)
{
    return XInterlocked::CompareExchange( &::rux::com::_objects_count , 0 , 0 ) == 0 ? S_OK : S_FALSE;
};
STDAPI DllRegisterServer( void )
{
	HRESULT hr = S_FALSE;
	if( ::rux::com::RegisterSelfComServer( ::rux::shell::extension::CLSID_RuxConfigureShellExtension , "RuxConfigure" , "ShellExtension" ) )
		hr = additional_register( ::rux::shell::extension::CLSID_RuxConfigureShellExtension , "RuxConfigure" , "ShellExtension" ) ? S_OK : S_FALSE;
	::rux::engine::deinitialize();
	return hr;
};
STDAPI DllUnregisterServer(void)
{
	HRESULT hr = S_FALSE;
	if( ::rux::com::UnregisterComServer( ::rux::shell::extension::CLSID_RuxConfigureShellExtension , "RuxConfigure" , "ShellExtension" ) )
		hr = additional_unregister( ::rux::shell::extension::CLSID_RuxConfigureShellExtension , "RuxConfigure" , "ShellExtension" ) ? S_OK : S_FALSE;
	::rux::engine::deinitialize();
	return hr;
};



//rux_system_module_initialize( "rux.engine" , start , stop );
