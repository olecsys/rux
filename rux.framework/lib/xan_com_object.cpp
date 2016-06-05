#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_com_object.h>
#ifdef __WINDOWS__
#include <xan_registry.h>
#include <xan_module.h>
namespace rux
{
	namespace com
	{
		dll_internal rux_volatile _objects_count = 0;
		
		::rux::String& RefiidToString( REFIID riid )
		{
			LPOLESTR ole_str = NULL;
			HRESULT hr = ::StringFromCLSID( riid , &ole_str );
			if( FAILED( hr ) )
				return ::rux::XString()++;
			::rux::XString riid_string( ole_str );
			CoTaskMemFree( ole_str );
			return riid_string++;
		};
		::rux::uint8 RegisterSelfComServer( REFIID riid , const ::rux::XString& LibId , const ::rux::XString& ClassId )
		{
			LPOLESTR ole_str = NULL;
			HRESULT hr = ::StringFromCLSID( riid , &ole_str );
			if( FAILED( hr ) )
				return 0;
			::rux::XString riid_string( ole_str );
			CoTaskMemFree( ole_str );
	
			::rux::XArray< ::rux::XString > key_names;	

			::rux::XString error;
			::rux::XRegistry reg;

			key_names.Add( LibId + "." + ClassId );
			key_names.Add( ::rux::XString( "CLSID" ) );
			reg.CreateRegKey( XEnum_PredefinedRegKey_HKEY_CLASSES_ROOT , key_names , error );
			if( error.Length() == 0 )
			{
				reg.set_RegValueAsString( XEnum_PredefinedRegKey_HKEY_CLASSES_ROOT , key_names , "" , riid_string , error );
				if( error.Length() == 0 )
				{
					key_names.Clear();
					key_names.Add( ::rux::XString( "CLSID" ) );
					key_names.Add( riid_string );
					reg.CreateRegKey( XEnum_PredefinedRegKey_HKEY_CLASSES_ROOT , key_names , error );
					if( error.Length() == 0 )
					{
						reg.set_RegValueAsString( XEnum_PredefinedRegKey_HKEY_CLASSES_ROOT , key_names , "" , ClassId + " Class" , error );
						if( error.Length() == 0 )
						{
							key_names.Add( ::rux::XString( "ProgId" ) );
							reg.CreateRegKey( XEnum_PredefinedRegKey_HKEY_CLASSES_ROOT , key_names , error );
							if( error.Length() == 0 )
							{
								reg.set_RegValueAsString( XEnum_PredefinedRegKey_HKEY_CLASSES_ROOT , key_names , "" , LibId + "." + ClassId , error );
								if( error.Length() == 0 )
								{
									::rux::XModule current( ::rux::XModule::get_Current( error ) );
									if( error.Length() == 0 )
									{
										::rux::XString module_filename;
										 module_filename.set_ByRef( current.get_FileName() );
										 if( module_filename.Length() > 0 )
										 {
											 module_filename.set_ByRef( module_filename.ReplaceAll( '/' , '\\' ) );
											 key_names.Clear();
											 key_names.Add( ::rux::XString( "CLSID" ) );
											 key_names.Add( riid_string );
											 key_names.Add( ::rux::XString( "InProcServer32" ) );
											 reg.CreateRegKey( XEnum_PredefinedRegKey_HKEY_CLASSES_ROOT , key_names , error );
											if( error.Length() == 0 )
											{
												reg.set_RegValueAsString( XEnum_PredefinedRegKey_HKEY_CLASSES_ROOT , key_names , "" , module_filename , error );
												if( error.Length() == 0 )
													return 1;
											}								 
										 }
									}
								}
							}
						}
					}
				}
			}
			return 0;
		};
		::rux::uint8 UnregisterComServer( REFIID riid , const ::rux::XString& LibId , const ::rux::XString& ClassId )
		{
			LPOLESTR ole_str = NULL;
			HRESULT hr = ::StringFromCLSID( riid , &ole_str );
			if( FAILED( hr ) )
				return 0;
			::rux::XString riid_string( ole_str );
			CoTaskMemFree( ole_str );
	
			::rux::XString error;
			::rux::XRegistry reg;

			reg.DeleteRegKey( XEnum_PredefinedRegKey_HKEY_CLASSES_ROOT , LibId + "." + ClassId , error );
			if( error.Length() == 0 )
			{
				reg.DeleteRegKey( XEnum_PredefinedRegKey_HKEY_CLASSES_ROOT , "CLSID\\" + riid_string , error );
				if( error.Length() == 0 )
					return 1;
			}
			return 0;
		};
	};
};
#endif