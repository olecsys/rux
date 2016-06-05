#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_registry.h>
#include <xan_error.h>
#ifdef __WINDOWS__
#include <xan_keyvaluepair.h>
#include <xan_uint64.h>
#include <xan_boolean.h>
begin_implement_rux_class_ns( Registry , rux );
end_implement_rux_class();
namespace rux
{	
	void XRegistry::DeleteRegKey(  XEnum_PredefinedRegKey predefined_key , const ::rux::XString& key_name , ::rux::XString& error )
	{
		error.Clear();
		::rux::XArray< ::rux::XString > sub_keys;
		sub_keys.set_ByRef( get_RegKeySubKeys( predefined_key , key_name , error ) );
		if( error.Length() == 0 )
		{
			for( size_t index0 = 0 ; index0 < sub_keys.Count() ; index0++ )
			{
				DeleteRegKey( predefined_key , key_name + "\\" + sub_keys[ index0 ] , error );
				if( error.Length() > 0 )
					break;
			}
			if( error.Length() == 0 )
			{
				::rux::XString utf16_key_name;
				utf16_key_name.set_ByRef( key_name.ConvertToUTF16() );
				HKEY predefined_key_handle = HKEY_CLASSES_ROOT;	
				if( predefined_key == XEnum_PredefinedRegKey_HKEY_CURRENT_USER )
					predefined_key_handle = HKEY_CURRENT_USER;
				else if( predefined_key == XEnum_PredefinedRegKey_HKEY_LOCAL_MACHINE )
					predefined_key_handle = HKEY_LOCAL_MACHINE;
				else if( predefined_key == XEnum_PredefinedRegKey_HKEY_USERS )
					predefined_key_handle = HKEY_USERS;
				HKEY key_handle = NULL;
				::rux::int32 status = RegOpenKeyExW( predefined_key_handle , (wchar_t*)utf16_key_name.str() , 0 , DELETE | KEY_ENUMERATE_SUB_KEYS | KEY_QUERY_VALUE , &key_handle );
				if( status == ERROR_SUCCESS)
				{
					RegCloseKey( key_handle );
					status = RegDeleteKeyW( predefined_key_handle , (wchar_t*)utf16_key_name.str() );
					if( status != ERROR_SUCCESS)
					{		
						declare_stack_variable( char , error_ptr , 1024 );
						FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM , NULL ,status , 0 , error_ptr , 1024 , NULL );
						rux_write_error( XString( "cannot delete registry key , reason:" ) + error_ptr , error );
					}
				}
			}
		}
	};
	void XRegistry::CreateRegKey( XEnum_PredefinedRegKey predefined_key , ::rux::XArray< ::rux::XString >& key_names , ::rux::XString& error )
	{
		HKEY predefined_key_handle = HKEY_CLASSES_ROOT;	
		if( predefined_key == XEnum_PredefinedRegKey_HKEY_CURRENT_USER )
			predefined_key_handle = HKEY_CURRENT_USER;
		else if( predefined_key == XEnum_PredefinedRegKey_HKEY_LOCAL_MACHINE )
			predefined_key_handle = HKEY_LOCAL_MACHINE;
		else if( predefined_key == XEnum_PredefinedRegKey_HKEY_USERS )
			predefined_key_handle = HKEY_USERS;
		error.Clear();
		XMallocArray< HKEY > hkeys;
		for( size_t index0 = 0 ; index0 < key_names.Count() ; index0++ )
		{
			::rux::XString utf16_key_name( key_names[ index0 ] );
			utf16_key_name.set_ByRef( utf16_key_name.ConvertToUTF16() );
			
			HKEY key_handle = NULL;
			::rux::int32 status = RegOpenKeyExW( predefined_key_handle , (wchar_t*)utf16_key_name.str() , 0 , KEY_READ , &key_handle );
			if( status != ERROR_SUCCESS)
			{
				status = RegCreateKeyW( predefined_key_handle , (wchar_t*)utf16_key_name.str() , &key_handle );		
				if( status == ERROR_SUCCESS )
				{
					predefined_key_handle = key_handle;
					hkeys.Add( key_handle );
				}
				else
				{
					declare_stack_variable( char , error_ptr , 1024 );
					FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM , NULL ,status , 0 , error_ptr , 1024 , NULL );
					rux_write_error( XString( "cannot create registry key , reason:" ) + error_ptr , error );
					break;
				}
			}
			else
			{
				predefined_key_handle = key_handle;
				hkeys.Add( key_handle );
			}
		}
		for( size_t index0 = 0 ; index0 < hkeys.Count() ; index0++ )
			RegCloseKey( hkeys[ index0 ] );
	};
	void XRegistry::CreateRegKey( XEnum_PredefinedRegKey predefined_key , const ::rux::XString& key_name , ::rux::XString& error )
	{
		error.Clear();
		::rux::XString utf16_key_name;
		utf16_key_name.set_ByRef( key_name.ConvertToUTF16() );
		HKEY predefined_key_handle = HKEY_CLASSES_ROOT;	
		if( predefined_key == XEnum_PredefinedRegKey_HKEY_CURRENT_USER )
			predefined_key_handle = HKEY_CURRENT_USER;
		else if( predefined_key == XEnum_PredefinedRegKey_HKEY_LOCAL_MACHINE )
			predefined_key_handle = HKEY_LOCAL_MACHINE;
		else if( predefined_key == XEnum_PredefinedRegKey_HKEY_USERS )
			predefined_key_handle = HKEY_USERS;
		HKEY key_handle = NULL;
		::rux::int32 status = RegOpenKeyExW( predefined_key_handle , (wchar_t*)utf16_key_name.str() , 0 , KEY_READ , &key_handle );
		if( status != ERROR_SUCCESS)
		{
			status = RegCreateKeyW( predefined_key_handle , (wchar_t*)utf16_key_name.str() , &key_handle );		
			if( status == ERROR_SUCCESS )
				RegCloseKey( key_handle );
			else
			{
				declare_stack_variable( char , error_ptr , 1024 );
				FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM , NULL ,status , 0 , error_ptr , 1024 , NULL );
				rux_write_error( XString( "cannot create registry key , reason:" ) + error_ptr , error );
			}
		}
		else
			RegCloseKey( key_handle );
	};
	void XRegistry::set_RegValueAsBinary( XEnum_PredefinedRegKey predefined_key , const ::rux::XString& key_name , const ::rux::XString& value_name , ::rux::uint8* value , size_t value_size , ::rux::XString& error )
	{
		error.Clear();
		::rux::XString utf16_key_name , utf16_value_name;
		utf16_key_name.set_ByRef( key_name.ConvertToUTF16() );
		utf16_value_name.set_ByRef( value_name.ConvertToUTF16() );	
		HKEY predefined_key_handle = HKEY_CLASSES_ROOT;	
		if( predefined_key == XEnum_PredefinedRegKey_HKEY_CURRENT_USER )
			predefined_key_handle = HKEY_CURRENT_USER;
		else if( predefined_key == XEnum_PredefinedRegKey_HKEY_LOCAL_MACHINE )
			predefined_key_handle = HKEY_LOCAL_MACHINE;
		else if( predefined_key == XEnum_PredefinedRegKey_HKEY_USERS )
			predefined_key_handle = HKEY_USERS;
		HKEY key_handle = NULL;
		::rux::int32 status = RegOpenKeyExW( predefined_key_handle , (wchar_t*)utf16_key_name.str() , 0 , KEY_WRITE , &key_handle );
		if( status == ERROR_SUCCESS)
		{
			status = RegSetValueExW( key_handle , (wchar_t*)utf16_value_name.str() , 0 , REG_BINARY , value , value_size );
			if( status != ERROR_SUCCESS)
			{
				declare_stack_variable( char , error_ptr , 1024 );
				FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM , NULL ,status , 0 , error_ptr , 1024 , NULL );
				rux_write_error( XString( "cannot set registry key value , reason:" ) + error_ptr , error );
			}
			RegCloseKey( key_handle );
		}
		else
		{
			declare_stack_variable( char , error_ptr , 1024 );
			FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM , NULL ,status , 0 , error_ptr , 1024 , NULL );
			rux_write_error( XString( "cannot set registry key value , reason:" ) + error_ptr , error );
		}
	};
	void XRegistry::set_RegValueAsDWORD( XEnum_PredefinedRegKey predefined_key , const ::rux::XString& key_name , const ::rux::XString& value_name , DWORD value , ::rux::XString& error )
	{
		error.Clear();
		::rux::XString utf16_key_name , utf16_value_name;
		utf16_key_name.set_ByRef( key_name.ConvertToUTF16() );
		utf16_value_name.set_ByRef( value_name.ConvertToUTF16() );	
		HKEY predefined_key_handle = HKEY_CLASSES_ROOT;	
		if( predefined_key == XEnum_PredefinedRegKey_HKEY_CURRENT_USER )
			predefined_key_handle = HKEY_CURRENT_USER;
		else if( predefined_key == XEnum_PredefinedRegKey_HKEY_LOCAL_MACHINE )
			predefined_key_handle = HKEY_LOCAL_MACHINE;
		else if( predefined_key == XEnum_PredefinedRegKey_HKEY_USERS )
			predefined_key_handle = HKEY_USERS;
		HKEY key_handle = NULL;
		::rux::int32 status = RegOpenKeyExW( predefined_key_handle , (wchar_t*)utf16_key_name.str() , 0 , KEY_WRITE , &key_handle );
		if( status == ERROR_SUCCESS)
		{
			status = RegSetValueExW( key_handle , (wchar_t*)utf16_value_name.str() , 0 , REG_DWORD , (::rux::uint8*)&value , sizeof( DWORD ) );
			if( status != ERROR_SUCCESS)
			{
				declare_stack_variable( char , error_ptr , 1024 );
				FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM , NULL ,status , 0 , error_ptr , 1024 , NULL );
				rux_write_error( XString( "cannot set registry key value , reason:" ) + error_ptr , error );
			}
			RegCloseKey( key_handle );
		}
		else
		{
			declare_stack_variable( char , error_ptr , 1024 );
			FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM , NULL ,status , 0 , error_ptr , 1024 , NULL );
			rux_write_error( XString( "cannot set registry key value , reason:" ) + error_ptr , error );
		}
	};
	void XRegistry::set_RegValueAsString( XEnum_PredefinedRegKey predefined_key , ::rux::XArray< ::rux::XString >& key_names , const ::rux::XString& value_name , const ::rux::XString& value , ::rux::XString& error )
	{
		HKEY predefined_key_handle = HKEY_CLASSES_ROOT;	
		if( predefined_key == XEnum_PredefinedRegKey_HKEY_CURRENT_USER )
			predefined_key_handle = HKEY_CURRENT_USER;
		else if( predefined_key == XEnum_PredefinedRegKey_HKEY_LOCAL_MACHINE )
			predefined_key_handle = HKEY_LOCAL_MACHINE;
		else if( predefined_key == XEnum_PredefinedRegKey_HKEY_USERS )
			predefined_key_handle = HKEY_USERS;
		error.Clear();
		XMallocArray< HKEY > hkeys;
		for( size_t index0 = 0 ; index0 < key_names.Count() ; index0++ )
		{
			::rux::XString utf16_key_name( key_names[ index0 ] );
			utf16_key_name.set_ByRef( utf16_key_name.ConvertToUTF16() );
			
			HKEY key_handle = NULL;
			::rux::int32 status = RegOpenKeyExW( predefined_key_handle , (wchar_t*)utf16_key_name.str() , 0 , KEY_WRITE , &key_handle );
			if( status == ERROR_SUCCESS)
			{				
				predefined_key_handle = key_handle;
				hkeys.Add( key_handle );
			}
			else
			{
				declare_stack_variable( char , error_ptr , 1024 );
				FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM , NULL ,status , 0 , error_ptr , 1024 , NULL );
				rux_write_error( XString( "cannot create registry key , reason:" ) + error_ptr , error );
				break;
			}
		}
		if( error.Length() == 0 )
		{
			::rux::XString utf16_value_name , utf16_value;
			utf16_value_name.set_ByRef( value_name.ConvertToUTF16() );
			utf16_value.set_ByRef( value.ConvertToUTF16() );
			::rux::int32 status = RegSetValueExW( hkeys[ hkeys.Count() - 1 ] , (wchar_t*)utf16_value_name.str() , 0 , REG_SZ , (::rux::uint8*)utf16_value.str() , utf16_value.Size() );
			if( status != ERROR_SUCCESS)
			{
				declare_stack_variable( char , error_ptr , 1024 );
				FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM , NULL ,status , 0 , error_ptr , 1024 , NULL );
				rux_write_error( XString( "cannot set registry key value , reason:" ) + error_ptr , error );
			}
		}
		for( size_t index0 = 0 ; index0 < hkeys.Count() ; index0++ )
			RegCloseKey( hkeys[ index0 ] );
	};
	void XRegistry::set_RegValueAsString( XEnum_PredefinedRegKey predefined_key , const ::rux::XString& key_name , const ::rux::XString& value_name , const ::rux::XString& value , ::rux::XString& error )
	{
		error.Clear();
		::rux::XString utf16_key_name , utf16_value_name , utf16_value;
		utf16_key_name.set_ByRef( key_name.ConvertToUTF16() );
		utf16_value_name.set_ByRef( value_name.ConvertToUTF16() );
		utf16_value.set_ByRef( value.ConvertToUTF16() );
		HKEY predefined_key_handle = HKEY_CLASSES_ROOT;	
		if( predefined_key == XEnum_PredefinedRegKey_HKEY_CURRENT_USER )
			predefined_key_handle = HKEY_CURRENT_USER;
		else if( predefined_key == XEnum_PredefinedRegKey_HKEY_LOCAL_MACHINE )
			predefined_key_handle = HKEY_LOCAL_MACHINE;
		else if( predefined_key == XEnum_PredefinedRegKey_HKEY_USERS )
			predefined_key_handle = HKEY_USERS;
		HKEY key_handle = NULL;
		::rux::int32 status = RegOpenKeyExW( predefined_key_handle , (wchar_t*)utf16_key_name.str() , 0 , KEY_WRITE , &key_handle );
		if( status == ERROR_SUCCESS)
		{
			status = RegSetValueExW( key_handle , (wchar_t*)utf16_value_name.str() , 0 , REG_SZ , (::rux::uint8*)utf16_value.str() , utf16_value.Size() );
			if( status != ERROR_SUCCESS)
			{
				declare_stack_variable( char , error_ptr , 1024 );
				FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM , NULL ,status , 0 , error_ptr , 1024 , NULL );
				rux_write_error( XString( "cannot set registry key value , reason:" ) + error_ptr , error );
			}
			RegCloseKey( key_handle );
		}
		else
		{
			declare_stack_variable( char , error_ptr , 1024 );
			FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM , NULL ,status , 0 , error_ptr , 1024 , NULL );
			rux_write_error( XString( "cannot set registry key value , reason:" ) + error_ptr , error );
		}
	};
	void XRegistry::set_RegValueAsExpandString( XEnum_PredefinedRegKey predefined_key , const ::rux::XString& key_name , const ::rux::XString& value_name , const ::rux::XString& value , ::rux::XString& error )
	{
		error.Clear();
		::rux::XString utf16_key_name , utf16_value_name , utf16_value;
		utf16_key_name.set_ByRef( key_name.ConvertToUTF16() );
		utf16_value_name.set_ByRef( value_name.ConvertToUTF16() );
		utf16_value.set_ByRef( value.ConvertToUTF16() );
		HKEY predefined_key_handle = HKEY_CLASSES_ROOT;	
		if( predefined_key == XEnum_PredefinedRegKey_HKEY_CURRENT_USER )
			predefined_key_handle = HKEY_CURRENT_USER;
		else if( predefined_key == XEnum_PredefinedRegKey_HKEY_LOCAL_MACHINE )
			predefined_key_handle = HKEY_LOCAL_MACHINE;
		else if( predefined_key == XEnum_PredefinedRegKey_HKEY_USERS )
			predefined_key_handle = HKEY_USERS;
		HKEY key_handle = NULL;
		::rux::int32 status = RegOpenKeyExW( predefined_key_handle , (wchar_t*)utf16_key_name.str() , 0 , KEY_WRITE , &key_handle );
		if( status == ERROR_SUCCESS)
		{
			status = RegSetValueExW( key_handle , (wchar_t*)utf16_value_name.str() , 0 , REG_EXPAND_SZ , (::rux::uint8*)utf16_value.str() , utf16_value.Size() );
			if( status != ERROR_SUCCESS)
			{
				declare_stack_variable( char , error_ptr , 1024 );
				FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM , NULL ,status , 0 , error_ptr , 1024 , NULL );
				rux_write_error( XString( "cannot set registry key value , reason:" ) + error_ptr , error );
			}
			RegCloseKey( key_handle );
		}
		else
		{
			declare_stack_variable( char , error_ptr , 1024 );
			FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM , NULL ,status , 0 , error_ptr , 1024 , NULL );
			rux_write_error( XString( "cannot set registry key value , reason:" ) + error_ptr , error );
		}
	};
	DWORD XRegistry::get_RegValueAsDWORD( XEnum_PredefinedRegKey predefined_key , const ::rux::XString& key_name , const ::rux::XString& value_name , ::rux::XString& error )
	{
		error.Clear();
		DWORD value = 0;
		HKEY predefined_key_handle = HKEY_CLASSES_ROOT;	
		if( predefined_key == XEnum_PredefinedRegKey_HKEY_CURRENT_USER )
			predefined_key_handle = HKEY_CURRENT_USER;
		else if( predefined_key == XEnum_PredefinedRegKey_HKEY_LOCAL_MACHINE )
			predefined_key_handle = HKEY_LOCAL_MACHINE;
		else if( predefined_key == XEnum_PredefinedRegKey_HKEY_USERS )
			predefined_key_handle = HKEY_USERS;
		HKEY key_handle = NULL;
		if( key_name()->_code_page == XEnumCodePage_UTF16 )
		{
			::rux::int32 status = RegOpenKeyExW( predefined_key_handle , (wchar_t*)key_name.str() , 0 , KEY_READ , &key_handle );
			if( status == ERROR_SUCCESS)
			{
				declare_stack_variable( char , data_ptr , 2048 );
				unsigned long size = 2048;
				unsigned long type = 0;
				if( value_name()->_code_page == XEnumCodePage_UTF16 )			
					status = RegQueryValueExW( key_handle , (wchar_t*)value_name.str() , 0 , &type , (::rux::uint8*)data_ptr , &size);
				else
					status = RegQueryValueExA( key_handle , value_name.str(), 0, &type , (::rux::uint8*)data_ptr , &size );
				if( status == ERROR_SUCCESS )
				{
					if( type == REG_DWORD )
						value = *(DWORD*)data_ptr;
					else
						error = "registry value is not DWORD";
				}
				else
				{
					declare_stack_variable( char , error_ptr , 1024 );
					FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM , NULL ,status , 0 , error_ptr , 512 , NULL );
					error = "cannot query registry value,reason:";
					error += error_ptr;
				}
				RegCloseKey( key_handle );
			}
			else
			{
				declare_stack_variable( char , error_ptr , 1024 );
				FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM , NULL ,status , 0 , error_ptr , 512 , NULL );
				error = "cannot open registry key,reason:";
				error += error_ptr;
			}
		}
		else
		{
			::rux::int32 status = RegOpenKeyExA( predefined_key_handle , key_name.str() , 0 , KEY_READ , &key_handle);
			if( status == ERROR_SUCCESS)
			{
				declare_stack_variable( char , data_ptr , 2048 );
				unsigned long size = 2048;
				unsigned long type = 0;
				if( value_name()->_code_page == XEnumCodePage_UTF16 )			
					status = RegQueryValueExW( key_handle , (wchar_t*)value_name.str() , 0 , &type , (::rux::uint8*)data_ptr , &size);
				else
					status = RegQueryValueExA( key_handle , value_name.str() , 0, &type , (::rux::uint8*)data_ptr , &size );
				if( status == ERROR_SUCCESS )
				{
					if( type == REG_DWORD )
						value = *(DWORD*)data_ptr;
					else
						error = "registry value is not DWORD";
				}
				else
				{
					declare_stack_variable( char , error_ptr , 1024 );
					FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM , NULL ,status , 0 , error_ptr , 512 , NULL );
					error = "cannot query registry value,reason:";
					error += error_ptr;
				}
				RegCloseKey( key_handle );
			}
			else
			{
				declare_stack_variable( char , error_ptr , 1024 );
				FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM , NULL ,status , 0 , error_ptr , 512 , NULL );
				error = "cannot open registry key,reason:";
				error += error_ptr;
			}
		}
		return value++;
	};
	::rux::String& XRegistry::get_RegValueAsString( XEnum_PredefinedRegKey predefined_key , const ::rux::XString& key_name , const ::rux::XString& value_name , ::rux::XString& error )
	{
		error.Clear();
		::rux::XString value;
		HKEY predefined_key_handle = HKEY_CLASSES_ROOT;	
		if( predefined_key == XEnum_PredefinedRegKey_HKEY_CURRENT_USER )
			predefined_key_handle = HKEY_CURRENT_USER;
		else if( predefined_key == XEnum_PredefinedRegKey_HKEY_LOCAL_MACHINE )
			predefined_key_handle = HKEY_LOCAL_MACHINE;
		else if( predefined_key == XEnum_PredefinedRegKey_HKEY_USERS )
			predefined_key_handle = HKEY_USERS;
		HKEY key_handle = NULL;
		if( key_name()->_code_page == XEnumCodePage_UTF16 )
		{
			::rux::int32 status = RegOpenKeyExW( predefined_key_handle , (wchar_t*)key_name.str() , 0 , KEY_READ , &key_handle );
			if( status == ERROR_SUCCESS)
			{
				declare_stack_variable( char , data_ptr , 2048 );
				unsigned long size = 2048;
				unsigned long type = 0;
				if( value_name()->_code_page == XEnumCodePage_UTF16 )			
					status = RegQueryValueExW( key_handle , (wchar_t*)value_name.str() , 0 , &type , (::rux::uint8*)data_ptr , &size);
				else
					status = RegQueryValueExA( key_handle , value_name.str(), 0, &type , (::rux::uint8*)data_ptr , &size );
				if( status == ERROR_SUCCESS )
				{
					if( type == REG_BINARY )
						error = "cannot convert to string from binary data";				
					else if( type == REG_DWORD 
						|| type == REG_DWORD_LITTLE_ENDIAN 
						|| type == REG_DWORD_BIG_ENDIAN )				
						value = XString::Format( "%ld" , *(::rux::uint32*)data_ptr );
					else if( type == REG_EXPAND_SZ )
					{
						if( value_name()->_code_page == XEnumCodePage_UTF16 )
						{
							value = (wchar_t*)data_ptr;
							if( ExpandEnvironmentStringsW( (wchar_t*)value.str() , (wchar_t*)data_ptr , 1024 ) != 0 )						
								value = (wchar_t*)data_ptr;
							else
								error = "cannot expand environment strings '" + value + "'";
						}
						else
						{
							value = data_ptr;
							if( ExpandEnvironmentStringsA( value.str() , data_ptr , 2048 ) != 0 )						
								value = data_ptr;
							else
								error = "cannot expand environment strings '" + value + "'";
						}
					}
					else if( type == REG_LINK )				
						value = (wchar_t*)data_ptr;
					else if( type == REG_MULTI_SZ )
					{
						if( value_name()->_code_page == XEnumCodePage_UTF16 )
						{						
							::rux::uint16* ptr = (::rux::uint16*)data_ptr;
							for( ; ; )
							{
								value += (wchar_t*)ptr;
								ptr += XString::LengthUTF16( (::rux::uint16*)ptr ) + 1;
								if( ptr[ 0 ] == '\0' )
									break;
								value += "/1div";
							}
						}
						else
						{
							char* ptr = data_ptr;
							for( ; ; )
							{
								value += ptr;
								ptr += ::strlen( ptr ) + 1;
								if( ptr[ 0 ] == '\0' )
									break;
								value += "/1div";
							}
						}
					}
					else if( type == REG_NONE )
						error = "cannot convert to string, registry value has not type";				
					else if( type == REG_QWORD 
						|| type == REG_QWORD_LITTLE_ENDIAN )
						value = XString::Format( I64d , *(::rux::int64*)data_ptr );
					else if( type == REG_SZ )
					{
						if( value_name()->_code_page == XEnumCodePage_UTF16 )
							value = (wchar_t*)data_ptr;					
						else
							value = data_ptr;
					}
				}
				else
				{
					declare_stack_variable( char , error_ptr , 1024 );
					FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM , NULL ,status , 0 , error_ptr , 512 , NULL );
					error = "cannot query registry value,reason:";
					error += error_ptr;
				}
				RegCloseKey( key_handle );
			}
			else
			{
				declare_stack_variable( char , error_ptr , 1024 );
				FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM , NULL ,status , 0 , error_ptr , 512 , NULL );
				error = "cannot open registry key,reason:";
				error += error_ptr;
			}
		}
		else
		{
			::rux::int32 status = RegOpenKeyExA( predefined_key_handle , key_name.str() , 0 , KEY_READ , &key_handle);
			if( status == ERROR_SUCCESS)
			{
				declare_stack_variable( char , data_ptr , 2048 );
				unsigned long size = 2048;
				unsigned long type = 0;
				if( value_name()->_code_page == XEnumCodePage_UTF16 )			
					status = RegQueryValueExW( key_handle , (wchar_t*)value_name.str() , 0 , &type , (::rux::uint8*)data_ptr , &size);
				else
					status = RegQueryValueExA( key_handle , value_name.str() , 0, &type , (::rux::uint8*)data_ptr , &size );
				if( status == ERROR_SUCCESS )
				{
					if( type == REG_BINARY )
						error = "cannot convert to string from binary data";				
					else if( type == REG_DWORD 
						|| type == REG_DWORD_LITTLE_ENDIAN 
						|| type == REG_DWORD_BIG_ENDIAN )				
						value = XString::Format( "%ld" , *(::rux::uint32*)data_ptr );
					else if( type == REG_EXPAND_SZ )
					{
						if( value_name()->_code_page == XEnumCodePage_UTF16 )
						{
							value = (wchar_t*)data_ptr;
							if( ExpandEnvironmentStringsW( (wchar_t*)value.str() , (wchar_t*)data_ptr , 1024 ) != 0 )						
								value = (wchar_t*)data_ptr;
							else
								error = "cannot expand environment strings '" + value + "'";
						}
						else
						{
							value = data_ptr;
							if( ExpandEnvironmentStringsA( value.str() , data_ptr , 2048 ) != 0 )						
								value = data_ptr;
							else
								error = "cannot expand environment strings '" + value + "'";
						}
					}
					else if( type == REG_LINK )				
						value = (wchar_t*)data_ptr;
					else if( type == REG_MULTI_SZ )
					{
						if( value_name()->_code_page == XEnumCodePage_UTF16 )
						{						
							::rux::uint16* ptr = (::rux::uint16*)data_ptr;
							for( ; ; )
							{
								value += (wchar_t*)ptr;
								ptr += XString::LengthUTF16( (::rux::uint16*)ptr ) + 1;
								if( ptr[ 0 ] == '\0' )
									break;
								value += "/1div";
							}
						}
						else
						{
							char* ptr = data_ptr;
							for( ; ; )
							{
								value += ptr;
								ptr += ::strlen( ptr ) + 1;
								if( ptr[ 0 ] == '\0' )
									break;
								value += "/1div";
							}
						}
					}
					else if( type == REG_NONE )
						error = "cannot convert to string, registry value has not type";				
					else if( type == REG_QWORD 
						|| type == REG_QWORD_LITTLE_ENDIAN )
						value = XString::Format( I64d , *(::rux::int64*)data_ptr );
					else if( type == REG_SZ )
					{
						if( value_name()->_code_page == XEnumCodePage_UTF16 )
							value = (wchar_t*)data_ptr;					
						else
							value = data_ptr;
					}
				}
				else
				{
					declare_stack_variable( char , error_ptr , 1024 );
					FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM , NULL ,status , 0 , error_ptr , 512 , NULL );
					error = "cannot query registry value,reason:";
					error += error_ptr;
				}
				RegCloseKey( key_handle );
			}
			else
			{
				declare_stack_variable( char , error_ptr , 1024 );
				FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM , NULL ,status , 0 , error_ptr , 512 , NULL );
				error = "cannot open registry key,reason:";
				error += error_ptr;
			}
		}
		return value++;
	};
	Array<XString>& XRegistry::get_RegKeyValueNames( XEnum_PredefinedRegKey predefined_key , const ::rux::XString& key_name , ::rux::XString& error )
	{
		error.Clear();
		XArray<XString> values_names;
		HKEY predefined_key_handle = HKEY_CLASSES_ROOT;	
		if( predefined_key == XEnum_PredefinedRegKey_HKEY_CURRENT_USER )
			predefined_key_handle = HKEY_CURRENT_USER;
		else if( predefined_key == XEnum_PredefinedRegKey_HKEY_LOCAL_MACHINE )
			predefined_key_handle = HKEY_LOCAL_MACHINE;
		else if( predefined_key == XEnum_PredefinedRegKey_HKEY_USERS )
			predefined_key_handle = HKEY_USERS;
		HKEY key_handle = NULL;
		if( key_name()->_code_page == XEnumCodePage_UTF16 )
		{
			::rux::int32 status = RegOpenKeyExW( predefined_key_handle , (wchar_t*)key_name.str() , 0 , KEY_READ , &key_handle );
			if( status == ERROR_SUCCESS)
			{				
				unsigned long key_ptr_length = 0;
				wchar_t class_name_ptr[ 2048 ] = L"";
				unsigned long class_name_size = 2048;
				unsigned long sub_keys_count = 0;
				unsigned long max_sub_key_size = 0;
				unsigned long max_class_name_size = 0;
				unsigned long values_count = 0;
				unsigned long max_value_name_size = 0;
				unsigned long max_value_data_size = 0;
				unsigned long security_descriptor_size = 0;
				FILETIME last_write_time;
				status = RegQueryInfoKeyW( key_handle , class_name_ptr , &class_name_size , NULL , &sub_keys_count , &max_sub_key_size , &max_class_name_size , &values_count , &max_value_name_size , &max_value_data_size , &security_descriptor_size , &last_write_time );
				if( status == ERROR_SUCCESS )
				{
					if( values_count > 0 )
					{
						for( sub_keys_count = 0 ; sub_keys_count < values_count ; sub_keys_count++) 
						{ 
							key_ptr_length = 2048;						
							wchar_t key_ptr[ 2048 ] = L"";
							status = RegEnumValueW( key_handle , sub_keys_count , key_ptr , &key_ptr_length , NULL , NULL , NULL , NULL );
							if( status == ERROR_SUCCESS ) 
							{
								values_names.Add( XString( key_ptr ) );
							}
							else
							{
								declare_stack_variable( char , error_ptr , 1024 );
								FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM , NULL , status , 0 , error_ptr , 512 , NULL );
								error = "cannot enum registry key value,reason:";
								error += error_ptr;
							}
							if( error.Length() > 0 )
								break;
						}
					}
				}
				else
				{
					declare_stack_variable( char , error_ptr , 1024 );
					FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM , NULL , status , 0 , error_ptr , 512 , NULL );
					error = "cannot query registry key info,reason:";
					error += error_ptr;
				}
				RegCloseKey( key_handle );
			}
			else
			{
				declare_stack_variable( char , error_ptr , 1024 );
				FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM , NULL , status , 0 , error_ptr , 512 , NULL );
				error = "cannot open registry key,reason:";
				error += error_ptr;
			}
		}
		else
		{
			::rux::int32 status = RegOpenKeyExA( predefined_key_handle , key_name.str() , 0 , KEY_READ , &key_handle);
			if( status == ERROR_SUCCESS)
			{				
				unsigned long key_ptr_length = 0;
				wchar_t class_name_ptr[ 2048 ] = L"";
				unsigned long class_name_size = 2048;
				unsigned long sub_keys_count = 0;
				unsigned long max_sub_key_size = 0;
				unsigned long max_class_name_size = 0;
				unsigned long values_count = 0;
				unsigned long max_value_name_size = 0;
				unsigned long max_value_data_size = 0;
				unsigned long security_descriptor_size = 0;
				FILETIME last_write_time;
				status = RegQueryInfoKeyW( key_handle , class_name_ptr , &class_name_size , NULL , &sub_keys_count , &max_sub_key_size , &max_class_name_size , &values_count , &max_value_name_size , &max_value_data_size , &security_descriptor_size , &last_write_time );
				if( status == ERROR_SUCCESS )
				{
					if( values_count > 0 )
					{
						for( sub_keys_count = 0 ; sub_keys_count < values_count ; sub_keys_count++) 
						{ 
							key_ptr_length = 2048;						
							wchar_t key_ptr[ 2048 ] = L"";
							status = RegEnumValueW( key_handle , sub_keys_count , key_ptr , &key_ptr_length , NULL , NULL , NULL , NULL );
							if( status == ERROR_SUCCESS ) 
							{
								values_names.Add( XString( key_ptr ) );
							}
							else
							{
								declare_stack_variable( char , error_ptr , 1024 );
								FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM , NULL , status , 0 , error_ptr , 512 , NULL );
								error = "cannot enum registry key value,reason:";
								error += error_ptr;
							}
							if( error.Length() > 0 )
								break;
						}
					}
				}
				else
				{
					declare_stack_variable( char , error_ptr , 1024 );
					FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM , NULL , status , 0 , error_ptr , 512 , NULL );
					error = "cannot query registry key info,reason:";
					error += error_ptr;
				}
				RegCloseKey( key_handle );
			}
			else
			{
				declare_stack_variable( char , error_ptr , 1024 );
				FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM , NULL , status , 0 , error_ptr , 512 , NULL );
				error = "cannot open registry key,reason:";
				error += error_ptr;
			}
		}
		return values_names++;
	};
	Array<XString>& XRegistry::get_RegKeySubKeys( XEnum_PredefinedRegKey predefined_key , const ::rux::XString& key_name , ::rux::XString& error )
	{
		error.Clear();
		XArray<XString> sub_keys;
		HKEY predefined_key_handle = HKEY_CLASSES_ROOT;
		if( predefined_key == XEnum_PredefinedRegKey_HKEY_CURRENT_USER )
			predefined_key_handle = HKEY_CURRENT_USER;
		else if( predefined_key == XEnum_PredefinedRegKey_HKEY_LOCAL_MACHINE )
			predefined_key_handle = HKEY_LOCAL_MACHINE;
		else if( predefined_key == XEnum_PredefinedRegKey_HKEY_USERS )
			predefined_key_handle = HKEY_USERS;
		HKEY key_handle = NULL;
		if( key_name()->_code_page == XEnumCodePage_UTF16 )
		{
			::rux::int32 status = RegOpenKeyExW( predefined_key_handle , (wchar_t*)key_name.str() , 0 , KEY_READ , &key_handle );
			if( status == ERROR_SUCCESS)
			{				
				unsigned long key_ptr_length = 0;
				wchar_t class_name_ptr[ 2048 ] = L"";
				unsigned long class_name_size = 2048;
				unsigned long sub_keys_count = 0;
				unsigned long max_sub_key_size = 0;
				unsigned long max_class_name_size = 0;
				unsigned long values_count = 0;
				unsigned long max_value_name_size = 0;
				unsigned long max_value_data_size = 0;
				unsigned long security_descriptor_size = 0;
				FILETIME last_write_time;
				status = RegQueryInfoKeyW( key_handle , class_name_ptr , &class_name_size , NULL , &sub_keys_count , &max_sub_key_size , &max_class_name_size , &values_count , &max_value_name_size , &max_value_data_size , &security_descriptor_size , &last_write_time );
				if( status == ERROR_SUCCESS )
				{
					if( sub_keys_count > 0 )
					{
						for( values_count = 0 ; values_count < sub_keys_count ; values_count++) 
						{ 
							wchar_t key_ptr[ 2048 ] = L"";
							key_ptr_length = 2048;
							status = RegEnumKeyExW( key_handle , values_count , key_ptr , &key_ptr_length , NULL , NULL , NULL , &last_write_time ); 
							if( status == ERROR_SUCCESS ) 
							{
								sub_keys.Add( XString( key_ptr ) );
							}
							else
							{
								declare_stack_variable( char , error_ptr , 1024 );
								FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM , NULL , status , 0 , error_ptr , 512 , NULL );
								error = "cannot enum registry key,reason:";
								error += error_ptr;
							}
							if( error.Length() > 0 )
								break;
						}
					}
				}
				else
				{
					declare_stack_variable( char , error_ptr , 1024 );
					FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM , NULL , status , 0 , error_ptr , 512 , NULL );
					error = "cannot query registry key info,reason:";
					error += error_ptr;
				}
				RegCloseKey( key_handle );
			}
			else
			{
				declare_stack_variable( char , error_ptr , 1024 );
				FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM , NULL , status , 0 , error_ptr , 512 , NULL );
				error = "cannot open registry key,reason:";
				error += error_ptr;
			}
		}
		else
		{
			::rux::int32 status = RegOpenKeyExA( predefined_key_handle , key_name.str() , 0 , KEY_READ , &key_handle);
			if( status == ERROR_SUCCESS)
			{				
				unsigned long key_ptr_length = 0;
				wchar_t class_name_ptr[ 2048 ] = L"";
				unsigned long class_name_size = 2048;
				unsigned long sub_keys_count = 0;
				unsigned long max_sub_key_size = 0;
				unsigned long max_class_name_size = 0;
				unsigned long values_count = 0;
				unsigned long max_value_name_size = 0;
				unsigned long max_value_data_size = 0;
				unsigned long security_descriptor_size = 0;
				FILETIME last_write_time;
				status = RegQueryInfoKeyW( key_handle , class_name_ptr , &class_name_size , NULL , &sub_keys_count , &max_sub_key_size , &max_class_name_size , &values_count , &max_value_name_size , &max_value_data_size , &security_descriptor_size , &last_write_time );
				if( status == ERROR_SUCCESS )
				{
					if( sub_keys_count > 0 )
					{
						for( values_count = 0 ; values_count < sub_keys_count ; values_count++) 
						{ 
							wchar_t key_ptr[ 2048 ] = L"";
							key_ptr_length = 2048;
							status = RegEnumKeyExW( key_handle , values_count , key_ptr , &key_ptr_length , NULL , NULL , NULL , &last_write_time ); 
							if( status == ERROR_SUCCESS ) 
							{
								sub_keys.Add( XString( key_ptr ) );
							}
							else
							{
								declare_stack_variable( char , error_ptr , 1024 );
								FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM , NULL , status , 0 , error_ptr , 512 , NULL );
								error = "cannot enum registry key,reason:";
								error += error_ptr;
							}
							if( error.Length() > 0 )
								break;
						}
					}
				}
				else
				{
					declare_stack_variable( char , error_ptr , 1024 );
					FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM , NULL , status , 0 , error_ptr , 512 , NULL );
					error = "cannot query registry key info,reason:";
					error += error_ptr;
				}
				RegCloseKey( key_handle );
			}
			else
			{
				declare_stack_variable( char , error_ptr , 1024 );
				FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM , NULL , status , 0 , error_ptr , 512 , NULL );
				error = "cannot open registry key,reason:";
				error += error_ptr;
			}
		}
		return sub_keys++;
	};
};
namespace rux
{
	namespace registry
	{
		void set_RegValueAsBinary( XEnum_PredefinedRegKey predefined_key , const char* key_name , const char* value_name , ::rux::uint8* value , size_t value_size , char error[ 1024 ] )
		{
			error[ 0 ] = 0;
			declare_stack_variable( wchar_t , utf16_key_name , 1024 );
			declare_stack_variable( wchar_t , utf16_value_name , 1024 );
			size_t ansi_to_utf16_bytes = 0;
			XString::ansi_to_utf16( key_name , strlen( key_name ) , (::rux::uint16*)utf16_key_name , 1024 , ansi_to_utf16_bytes );
			XString::ansi_to_utf16( value_name , strlen( value_name ) , (::rux::uint16*)utf16_value_name , 1024 , ansi_to_utf16_bytes );
			HKEY predefined_key_handle = HKEY_CLASSES_ROOT;	
			if( predefined_key == XEnum_PredefinedRegKey_HKEY_CURRENT_USER )
				predefined_key_handle = HKEY_CURRENT_USER;
			else if( predefined_key == XEnum_PredefinedRegKey_HKEY_LOCAL_MACHINE )
				predefined_key_handle = HKEY_LOCAL_MACHINE;
			else if( predefined_key == XEnum_PredefinedRegKey_HKEY_USERS )
				predefined_key_handle = HKEY_USERS;
			HKEY key_handle = NULL;
			::rux::int32 status = RegOpenKeyExW( predefined_key_handle , utf16_key_name , 0 , KEY_WRITE , &key_handle );
			if( status == ERROR_SUCCESS)
			{
				status = RegSetValueExW( key_handle , utf16_value_name , 0 , REG_BINARY , value , value_size );
				if( status != ERROR_SUCCESS)
					FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM , NULL ,status , 0 , error , 1024 , NULL );
				RegCloseKey( key_handle );
			}
			else
				FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM , NULL ,status , 0 , error , 1024 , NULL );
		};
		void set_RegValueAsDWORD( XEnum_PredefinedRegKey predefined_key , const char* key_name , const char* value_name , DWORD value , char error[ 1024 ] )
		{
			error[ 0 ] = 0;
			declare_stack_variable( wchar_t , utf16_key_name , 1024 );
			declare_stack_variable( wchar_t , utf16_value_name , 1024 );
			size_t ansi_to_utf16_bytes = 0;
			XString::ansi_to_utf16( key_name , strlen( key_name ) , (::rux::uint16*)utf16_key_name , 1024 , ansi_to_utf16_bytes );
			XString::ansi_to_utf16( value_name , strlen( value_name ) , (::rux::uint16*)utf16_value_name , 1024 , ansi_to_utf16_bytes );
			HKEY predefined_key_handle = HKEY_CLASSES_ROOT;	
			if( predefined_key == XEnum_PredefinedRegKey_HKEY_CURRENT_USER )
				predefined_key_handle = HKEY_CURRENT_USER;
			else if( predefined_key == XEnum_PredefinedRegKey_HKEY_LOCAL_MACHINE )
				predefined_key_handle = HKEY_LOCAL_MACHINE;
			else if( predefined_key == XEnum_PredefinedRegKey_HKEY_USERS )
				predefined_key_handle = HKEY_USERS;
			HKEY key_handle = NULL;
			::rux::int32 status = RegOpenKeyExW( predefined_key_handle , utf16_key_name , 0 , KEY_WRITE , &key_handle );
			if( status == ERROR_SUCCESS)
			{
				status = RegSetValueExW( key_handle , utf16_value_name , 0 , REG_DWORD , (::rux::uint8*)&value , sizeof( DWORD ) );
				if( status != ERROR_SUCCESS)
					FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM , NULL , status , 0 , error , 1024 , NULL );
				RegCloseKey( key_handle );
			}
			else
				FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM , NULL , status , 0 , error , 1024 , NULL );
		};
		void set_RegValueAsString( XEnum_PredefinedRegKey predefined_key , const char* key_name , const char* value_name , const char* value , char error[ 1024 ] )
		{
			error[ 0 ] = 0;
			declare_stack_variable( wchar_t , utf16_key_name , 1024 );
			declare_stack_variable( wchar_t , utf16_value_name , 1024 );
			declare_stack_variable( wchar_t , utf16_value , 1024 );
			size_t ansi_to_utf16_bytes = 0;
			XString::ansi_to_utf16( key_name , strlen( key_name ) , (::rux::uint16*)utf16_key_name , 1024 , ansi_to_utf16_bytes );
			XString::ansi_to_utf16( value_name , strlen( value_name ) , (::rux::uint16*)utf16_value_name , 1024 , ansi_to_utf16_bytes );
			XString::ansi_to_utf16( value , strlen( value ) , (::rux::uint16*)utf16_value , 1024 , ansi_to_utf16_bytes );
			HKEY predefined_key_handle = HKEY_CLASSES_ROOT;	
			if( predefined_key == XEnum_PredefinedRegKey_HKEY_CURRENT_USER )
				predefined_key_handle = HKEY_CURRENT_USER;
			else if( predefined_key == XEnum_PredefinedRegKey_HKEY_LOCAL_MACHINE )
				predefined_key_handle = HKEY_LOCAL_MACHINE;
			else if( predefined_key == XEnum_PredefinedRegKey_HKEY_USERS )
				predefined_key_handle = HKEY_USERS;
			HKEY key_handle = NULL;
			::rux::int32 status = RegOpenKeyExW( predefined_key_handle , utf16_key_name , 0 , KEY_WRITE , &key_handle );
			if( status == ERROR_SUCCESS)
			{
				status = RegSetValueExW( key_handle , utf16_value_name , 0 , REG_SZ , (::rux::uint8*)utf16_value , 2 * ( wcslen( utf16_value ) + 1 ) );
				if( status != ERROR_SUCCESS)
					FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM , NULL , status , 0 , error , 1024 , NULL );
				RegCloseKey( key_handle );
			}
			else
				FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM , NULL , status , 0 , error , 1024 , NULL );
		};
		void set_RegValueAsExpandString( XEnum_PredefinedRegKey predefined_key , const char* key_name , const char* value_name , const char* value , char error[ 1024 ] )
		{
			error[ 0 ] = 0;
			declare_stack_variable( wchar_t , utf16_key_name , 1024 );
			declare_stack_variable( wchar_t , utf16_value_name , 1024 );
			declare_stack_variable( wchar_t , utf16_value , 1024 );
			size_t ansi_to_utf16_bytes = 0;
			XString::ansi_to_utf16( key_name , strlen( key_name ) , (::rux::uint16*)utf16_key_name , 1024 , ansi_to_utf16_bytes );
			XString::ansi_to_utf16( value_name , strlen( value_name ) , (::rux::uint16*)utf16_value_name , 1024 , ansi_to_utf16_bytes );
			XString::ansi_to_utf16( value , strlen( value ) , (::rux::uint16*)utf16_value , 1024 , ansi_to_utf16_bytes );
			HKEY predefined_key_handle = HKEY_CLASSES_ROOT;	
			if( predefined_key == XEnum_PredefinedRegKey_HKEY_CURRENT_USER )
				predefined_key_handle = HKEY_CURRENT_USER;
			else if( predefined_key == XEnum_PredefinedRegKey_HKEY_LOCAL_MACHINE )
				predefined_key_handle = HKEY_LOCAL_MACHINE;
			else if( predefined_key == XEnum_PredefinedRegKey_HKEY_USERS )
				predefined_key_handle = HKEY_USERS;
			HKEY key_handle = NULL;
			::rux::int32 status = RegOpenKeyExW( predefined_key_handle , utf16_key_name , 0 , KEY_WRITE , &key_handle );
			if( status == ERROR_SUCCESS)
			{
				status = RegSetValueExW( key_handle , utf16_value_name , 0 , REG_EXPAND_SZ , (::rux::uint8*)utf16_value , 2 * ( wcslen( utf16_value ) + 1 ) );
				if( status != ERROR_SUCCESS)
					FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM , NULL ,status , 0 , error , 1024 , NULL );
				RegCloseKey( key_handle );
			}
			else
				FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM , NULL ,status , 0 , error , 1024 , NULL );
		};
		DWORD get_RegValueAsDWORD( XEnum_PredefinedRegKey predefined_key , const char* key_name , const char* value_name , char error[ 1024 ] )
		{
			error[ 0 ] = 0;
			DWORD value = 0;
			HKEY predefined_key_handle = HKEY_CLASSES_ROOT;	
			if( predefined_key == XEnum_PredefinedRegKey_HKEY_CURRENT_USER )
				predefined_key_handle = HKEY_CURRENT_USER;
			else if( predefined_key == XEnum_PredefinedRegKey_HKEY_LOCAL_MACHINE )
				predefined_key_handle = HKEY_LOCAL_MACHINE;
			else if( predefined_key == XEnum_PredefinedRegKey_HKEY_USERS )
				predefined_key_handle = HKEY_USERS;
			HKEY key_handle = NULL;
			::rux::int32 status = RegOpenKeyExA( predefined_key_handle , key_name , 0 , KEY_READ , &key_handle);
			if( status == ERROR_SUCCESS)
			{
				declare_stack_variable( char , data_ptr , 2048 );
				unsigned long size = 2048;
				unsigned long type = 0;
				status = RegQueryValueExA( key_handle , value_name , 0, &type , (::rux::uint8*)data_ptr , &size );
				if( status == ERROR_SUCCESS )
				{
					if( type == REG_DWORD )
						value = *(DWORD*)data_ptr;
					else
						rux_concatenation( error , "registry value is not DWORD" );
				}
				else
					FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM , NULL ,status , 0 , error , 1024 , NULL );
				RegCloseKey( key_handle );
			}
			else
				FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM , NULL ,status , 0 , error , 1024 , NULL );
			return value++;
		};
		void get_RegValueAsString( XEnum_PredefinedRegKey predefined_key , const char* key_name , const char* value_name , char value[ 1024 ] , char error[ 512 ] )
		{
			error[ 0 ] = 0;
			HKEY predefined_key_handle = HKEY_CLASSES_ROOT;	
			if( predefined_key == XEnum_PredefinedRegKey_HKEY_CURRENT_USER )
				predefined_key_handle = HKEY_CURRENT_USER;
			else if( predefined_key == XEnum_PredefinedRegKey_HKEY_LOCAL_MACHINE )
				predefined_key_handle = HKEY_LOCAL_MACHINE;
			else if( predefined_key == XEnum_PredefinedRegKey_HKEY_USERS )
				predefined_key_handle = HKEY_USERS;
			HKEY key_handle = NULL;			
			::rux::int32 status = RegOpenKeyExA( predefined_key_handle , key_name , 0 , KEY_READ , &key_handle );
			if( status == ERROR_SUCCESS)
			{
				declare_stack_variable( char , data_ptr , 2048 );
				unsigned long size = 2048;
				unsigned long type = 0;
				status = RegQueryValueExA( key_handle , value_name , 0, &type , (::rux::uint8*)data_ptr , &size );
				if( status == ERROR_SUCCESS )
				{
					if( type == REG_BINARY )
						rux_concatenation( error , "cannot convert to string from binary data" );
					else if( type == REG_DWORD 
						|| type == REG_DWORD_LITTLE_ENDIAN 
						|| type == REG_DWORD_BIG_ENDIAN )				
						::rux::safe_sprintf( value , 1024 , "%ld" , *(::rux::uint32*)data_ptr );
					else if( type == REG_EXPAND_SZ )
					{						
						declare_stack_variable( char , data_ptr2 , 2048 );
						if( ExpandEnvironmentStringsA( data_ptr , data_ptr2 , 2048 ) != 0 )
							::rux::engine::memcpy( value , 0 , data_ptr , strlen( data_ptr ) + 1 );
						else
						{
							rux_concatenation( error , "cannot expand environment strings '" , value );
							rux_concatenation( error , "'" );
						}
					}					
					else if( type == REG_NONE )
						rux_concatenation( error , "cannot convert to string, registry value has not type" );
					else if( type == REG_QWORD 
						|| type == REG_QWORD_LITTLE_ENDIAN )
						::rux::safe_sprintf( value , 1024 , I64d , *(::rux::int64*)data_ptr );
					else if( type == REG_SZ )
						rux_concatenation( value , data_ptr );
				}
				else
					FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM , NULL , status , 0 , error , 1024 , NULL );
				RegCloseKey( key_handle );
			}
			else
				FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM , NULL ,status , 0 , error , 1024 , NULL );
		};
	};
};
#endif