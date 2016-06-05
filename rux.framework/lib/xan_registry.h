#ifndef XAN_REGISTRY_H
#define XAN_REGISTRY_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_string.h>
#ifdef __WINDOWS__
enum XEnum_PredefinedRegKey
{
	XEnum_PredefinedRegKey_HKEY_CURRENT_USER ,
	XEnum_PredefinedRegKey_HKEY_LOCAL_MACHINE ,
	XEnum_PredefinedRegKey_HKEY_USERS ,
	XEnum_PredefinedRegKey_HKEY_CLASSES_ROOT
};
namespace rux
{
	namespace registry
	{
		void get_RegValueAsString( XEnum_PredefinedRegKey predefined_key , const char* key_name , const char* value_name , char value[ 1024 ] , char error[ 512 ] );
		void set_RegValueAsBinary( XEnum_PredefinedRegKey predefined_key , const char* key_name , const char* value_name , ::rux::uint8* value , size_t value_size , char error[ 1024 ] );
		void set_RegValueAsDWORD( XEnum_PredefinedRegKey predefined_key , const char* key_name , const char* value_name , DWORD value , char error[ 1024 ] );
		void set_RegValueAsString( XEnum_PredefinedRegKey predefined_key , const char* key_name , const char* value_name , const char* value , char error[ 1024 ] );
		void set_RegValueAsExpandString( XEnum_PredefinedRegKey predefined_key , const char* key_name , const char* value_name , const char* value , char error[ 1024 ] );
		DWORD get_RegValueAsDWORD( XEnum_PredefinedRegKey predefined_key , const char* key_name , const char* value_name , char error[ 1024 ] );
	};
};
namespace rux
{	
	begin_declare_rux_class( Registry );
	begin_declare_rux_class_members( Registry );
	end_declare_rux_class_members( Registry );
		void CreateRegKey( XEnum_PredefinedRegKey predefined_key , const ::rux::XString& key_name , ::rux::XString& error );
		void CreateRegKey( XEnum_PredefinedRegKey predefined_key , ::rux::XArray< ::rux::XString >& key_names , ::rux::XString& error );
		void DeleteRegKey(  XEnum_PredefinedRegKey predefined_key , const ::rux::XString& key_name , ::rux::XString& error );
		::rux::String& get_RegValueAsString( XEnum_PredefinedRegKey predefined_key , const ::rux::XString& key_name , const ::rux::XString& value_name , ::rux::XString& error );
		DWORD get_RegValueAsDWORD( XEnum_PredefinedRegKey predefined_key , const ::rux::XString& key_name , const ::rux::XString& value_name , ::rux::XString& error );	
		void set_RegValueAsString( XEnum_PredefinedRegKey predefined_key , const ::rux::XString& key_name , const ::rux::XString& value_name , const ::rux::XString& value , ::rux::XString& error );
		void set_RegValueAsString( XEnum_PredefinedRegKey predefined_key , ::rux::XArray< ::rux::XString >& key_names , const ::rux::XString& value_name , const ::rux::XString& value , ::rux::XString& error );
		void set_RegValueAsDWORD( XEnum_PredefinedRegKey predefined_key , const ::rux::XString& key_name , const ::rux::XString& value_name , DWORD value , ::rux::XString& error );
		void set_RegValueAsBinary( XEnum_PredefinedRegKey predefined_key , const ::rux::XString& key_name , const ::rux::XString& value_name , ::rux::uint8* value , size_t value_size , ::rux::XString& error );
		void set_RegValueAsExpandString( XEnum_PredefinedRegKey predefined_key , const ::rux::XString& key_name , const ::rux::XString& value_name , const ::rux::XString& value , ::rux::XString& error );	
		Array<XString>& get_RegKeySubKeys( XEnum_PredefinedRegKey predefined_key , const ::rux::XString& key_name , ::rux::XString& error );
		Array<XString>& get_RegKeyValueNames( XEnum_PredefinedRegKey predefined_key , const ::rux::XString& key_name , ::rux::XString& error );
	end_declare_rux_class();
};
#endif
#endif
