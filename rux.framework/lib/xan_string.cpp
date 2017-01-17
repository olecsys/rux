#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_includes.h>
#include "xan_string.h"
#include <xan_int64.h>
#include <xan_uint32.h>
#include <xan_console.h>
dll_public XEnumCodePage g_code_page = XEnumCodePage_Unknown;
dll_public ::rux::uint32 _rux_ansi_code_page = 0;
#include <xan_keyvaluepair.h>
#include <xan_uint64.h>
#include <xan_file_descriptor_waiter.h>
#include <xan_boolean.h>
implement_rux_base_methods_ns( String , rux );
implement_rux_set_operators_ns_( String , rux );
namespace rux
{	
	implement_ctor_and_register_type( String );
	#ifdef __UNIX__
	void String::get_all_locales( char locales[ 64 ][ 64 ] , size_t& locales_count )
	{
		if( rux_is_exists_file( "/usr/bin/locale" ) == 1 )
		{		
			::rux::int32 pipes[ 2 ] = {0};
			if( pipe( pipes ) == 0 )
			{		
				::rux::pid_t child_pid = fork();
				if( child_pid == 0 )
				{				
					close( pipes[ 0 ] );				
					dup2( pipes[ 1 ] , STDOUT_FILENO );				
					dup2( pipes[ 1 ] , STDERR_FILENO );
					close( pipes[ 1 ] );
					execl( "/usr/bin/locale" , "/usr/bin/locale" , "-a" , (char*)0 );
					_exit( 1 );
				}
				else if( child_pid != 0 )
				{				
					close( pipes[ 1 ] );
					::rux::io::file_descriptor_waiter fd_waiter;
					fd_waiter.add( pipes[ 0 ] );
					if( fd_waiter.wait( 5000 , 1 , 0 ) )
					{
						if( fd_waiter.get_read_fd() != -1 )
						{						
							declare_stack_variable( char , result , 8 * 1024 );
							::rux::int32 readen_size = read( pipes[ 0 ] , result , 8 *1024 );
							if( readen_size > 0 )
							{
								size_t offset = 0 , begin = 0;
								while( offset < readen_size )
								{
									if( result[ offset ] == '\n' )
									{
										if( offset - begin + 1 < 64 )
										{
											::memcpy( locales[ locales_count ] , &result[ begin ] , offset - begin );
											locales[ locales_count ][ offset - begin ] = '\0';
											locales_count++;
											if( locales_count == 64 )
												break;
										}
										begin = offset + 1;
									}
									offset++;
								}
							}
						}
					}
					close( pipes[ 0 ] );											
					waitpid( child_pid , NULL , 0 );
				}
			}	
		}
	};
	#endif
	XEnumCodePage XString::get_global_code_page( void )
	{
		if( g_code_page == XEnumCodePage_Unknown )
		{		
	#ifdef __WINDOWS__
			g_code_page = XEnumCodePage_CP1251;
			_rux_ansi_code_page = GetACP();		
	#elif defined( __UNIX__ )
			/*char locales[ 64 ][ 64 ] = { 0 , 0 };
			size_t locales_count = 0;
			String::get_all_locales( locales , locales_count );
			declare_stack_variable( char , locale_string , 64 );
			for( size_t index0 = 0 ; index0 < locales_count ; index0++ )
			{
				if( strcmp( locales[ index0 ] , "ru_RU.utf8" ) == 0
					|| strcmp( locales[ index0 ] , "ru_RU.utf-8" ) == 0
					|| strcmp( locales[ index0 ] , "ru_RU.UTF8" ) == 0
					|| strcmp( locales[ index0 ] , "ru_RU.UTF-8" ) == 0 )
				{
					::rux::engine::memcpy( locale_string , 0 , locales[ index0 ] , 64 );
					break;
				}
				else if( strcmp( locales[ index0 ] , "en_US.utf8" ) == 0 
					|| strcmp( locales[ index0 ] , "en_US.utf-8" ) == 0 
					|| strcmp( locales[ index0 ] , "en_US.UTF8" ) == 0 
					|| strcmp( locales[ index0 ] , "en_US.UTF-8" ) == 0 )
					::rux::engine::memcpy( locale_string , 0 , locales[ index0 ] , 64 );
				else if( ( strcmp( locales[ index0 ] , "en_GB.utf8" ) == 0
					|| strcmp( locales[ index0 ] , "en_GB.utf-8" ) == 0
					|| strcmp( locales[ index0 ] , "en_GB.UTF8" ) == 0
					|| strcmp( locales[ index0 ] , "en_GB.UTF-8" ) == 0 )
					&& ( strcmp( locale_string , "en_US.utf8" ) != 0
					|| strcmp( locale_string , "en_US.utf-8" ) != 0 
					|| strcmp( locale_string , "en_US.UTF8" ) != 0 
					|| strcmp( locale_string , "en_US.UTF-8" ) != 0 ) )
					::rux::engine::memcpy( locale_string , 0 , locales[ index0 ] , 64 );
				else if( rux_index_of( locales[ index0 ] , "utf8" ) != -1 
					|| rux_index_of( locales[ index0 ] , "utf-8" ) != -1 
					|| rux_index_of( locales[ index0 ] , "UTF-8" ) != -1 
					|| rux_index_of( locales[ index0 ] , "UTF8" ) != -1 )
					::rux::engine::memcpy( locale_string , 0 , locales[ index0 ] , 64 );
			}
			if( locale_string[ 0 ] != 0 )
				setlocale( LC_ALL , locale_string );
			setlocale( LC_NUMERIC , "C" );*/
			g_code_page = XEnumCodePage_UTF8;
	#endif
		}
		return g_code_page;
	};
	String::String( const char* , const char* , ::rux::int32 )
		: XGCRef( get_static_Type )
	{
		detach();
	};
	String::~String()
	{
		if( _str )
			::rux::engine::free_mem( _str );
	};
	implement_duplicate_internal_function_2( String , RemoveRange , size_t , size_t );
	implement_duplicate_internal_function_2( String , InsertRange , size_t , const ::rux::XString& );	
	implement_duplicate_internal_function_2( String , InsertRange , size_t , const ::rux::String& );
	implement_duplicate_internal_result_function_1_with_const( ::rux::uint32 , String , get_UTF32Char , size_t );
	implement_duplicate_internal_function_1( String , RemoveAt , size_t );
	implement_duplicate_internal_result_function_with_const( size_t , String , get_step );
	implement_duplicate_internal_function_3( String , InsertRange , size_t , const char* , size_t );
	implement_duplicate_internal_function_3( String , InsertRange , size_t , const wchar_t* , size_t );
	implement_duplicate_internal_result_function( ::rux::uint32 , String , ToUInt32 );
	implement_duplicate_internal_function_2( String , Insert , size_t , const char );
	implement_duplicate_internal_result_function_2_with_const( String& , String , Substring , size_t , size_t );
	implement_duplicate_internal_result_function_2_with_const( ::rux::int32 , String , IndexOf , const ::rux::XString& , size_t );
	implement_duplicate_internal_result_function_2_with_const( ::rux::int32 , String , IndexOf , const String& , size_t );
	implement_duplicate_internal_result_function_1_with_const( ::rux::uint32 , String , get_UTF8Char , size_t );
	implement_duplicate_internal_result_function_with_const( ::rux::String& , String , ConvertToUTF8 );
	implement_duplicate_internal_function_2( String , SwapHere , size_t , size_t );
	implement_duplicate_internal_result_function_with_const( String& , String , Trim );
	implement_duplicate_internal_result_function_1_with_const( ::rux::Array<XString>& , String , Split , const ::rux::XArray<XString>& );
	implement_duplicate_internal_result_function_2_with_const( ::rux::Array<XString>& , String , Split , const ::rux::XArray<XString>& , EStringSplitOptions );
	implement_duplicate_internal_result_function_1_with_const( Array<XString>& , String , Split , const char );
	implement_duplicate_internal_result_function_1_with_const( Array<XString>& , String , Split , const char* );
	implement_duplicate_internal_result_function_2_with_const( Array<XString>& , String , Split , const char* , EStringSplitOptions );
	implement_duplicate_internal_result_function( ::rux::int64 , String , ToInt64 );
	implement_duplicate_internal_result_function( ::rux::uint64 , String , ToUInt64 );
	implement_duplicate_internal_result_function( double , String , ToDouble );
	implement_duplicate_internal_result_function_2( ::rux::int32 , String , LastIndexOf , const char* , size_t );
	implement_duplicate_internal_result_function_2( ::rux::int32 , String , LastIndexOf , const String& , size_t );
	implement_duplicate_internal_result_function_1( bool , String , Equals , const char* );
	implement_duplicate_internal_result_function_2_with_const( bool , String , Equals , const char* , size_t );
	implement_duplicate_internal_result_function_1( bool , String , Equals , const String& );
	implement_duplicate_internal_result_function_1( bool , String , Equals , const ::rux::XString& );
	implement_duplicate_internal_result_function_with_const( ::rux::String& , String , ConvertToAnsi );

	implement_duplicate_internal_function( String , Clear );	
	implement_duplicate_internal_result_function_2( ::rux::int32 , String , IndexOfAny , const ::rux::XArray< ::rux::XChar>& , size_t );
	implement_duplicate_internal_result_function_2( ::rux::int32 , String , IndexOfAny , const ::rux::XArray<XString>& , size_t );
	implement_duplicate_internal_result_function_3( ::rux::int32 , String , IndexOfAny , const ::rux::XArray< ::rux::XChar>& , size_t , size_t& );
	implement_duplicate_internal_result_function_3( ::rux::int32 , String , IndexOfAny , const ::rux::XArray<XString>& , size_t , size_t& );
	implement_duplicate_internal_result_function_2_with_const( ::rux::int32 , String , IndexOf , const char , size_t );
	implement_duplicate_internal_result_function_2_with_const( ::rux::int32 , String , IndexOf , const char* , size_t );
	implement_duplicate_internal_result_function_2_with_const( ::rux::int32 , String , LastIndexOf , const char , size_t );
	implement_duplicate_internal_result_function_2_with_const( ::rux::int32 , String , LastIndexOf , const ::rux::XString& , size_t );
	implement_duplicate_internal_result_function_3_with_const( ::rux::String& , String , ReplaceAll , const char , const char , size_t );	
	implement_duplicate_internal_result_function_3_with_const( ::rux::String& , String , ReplaceAll , const char* , const char* , size_t );	
	implement_duplicate_internal_result_function_3_with_const( ::rux::String& , String , ReplaceAll , const ::rux::XString& , const ::rux::XString& , size_t );
	implement_duplicate_internal_result_function_3_with_const( ::rux::String& , String , ReplaceAll , const ::rux::XString& , const char* , size_t );
	implement_duplicate_internal_result_function_3_with_const( ::rux::String& , String , ReplaceAll , const ::rux::XString& , const String& , size_t );
	implement_duplicate_internal_result_function_3_with_const( ::rux::String& , String , ReplaceAll , const String& , const ::rux::XString& , size_t );
	implement_duplicate_internal_result_function_3_with_const( ::rux::String& , String , ReplaceAll , const String& , const char* , size_t );
	implement_duplicate_internal_result_function_3_with_const( ::rux::String& , String , ReplaceAll , const char* , const ::rux::XString& , size_t );
	implement_duplicate_internal_result_function_3_with_const( ::rux::String& , String , ReplaceAll , const char* , const String& , size_t );
	implement_duplicate_internal_result_function_3_with_const( ::rux::String& , String , ReplaceAll , const ::rux::String& , const ::rux::String& , size_t );
	implement_duplicate_internal_result_function_3_with_const( ::rux::String& , String , ReplaceFirst , const char , const char , size_t );	
	implement_duplicate_internal_result_function_3_with_const( ::rux::String& , String , ReplaceFirst , const char* , const char* , size_t );	
	implement_duplicate_internal_result_function_3_with_const( ::rux::String& , String , ReplaceFirst , const ::rux::XString& , const ::rux::XString& , size_t );
	implement_duplicate_internal_result_function_3_with_const( ::rux::String& , String , ReplaceFirst , const ::rux::XString& , const char* , size_t );
	implement_duplicate_internal_result_function_3_with_const( ::rux::String& , String , ReplaceFirst , const ::rux::XString& , const String& , size_t );
	implement_duplicate_internal_result_function_3_with_const( ::rux::String& , String , ReplaceFirst , const String& , const ::rux::XString& , size_t );
	implement_duplicate_internal_result_function_3_with_const( ::rux::String& , String , ReplaceFirst , const ::rux::String& , const ::rux::String& , size_t );
	implement_duplicate_internal_result_function_3_with_const( ::rux::String& , String , ReplaceFirst , const String& , const char* , size_t );
	implement_duplicate_internal_result_function_3_with_const( ::rux::String& , String , ReplaceFirst , const char* , const ::rux::XString& , size_t );
	implement_duplicate_internal_result_function_3_with_const( ::rux::String& , String , ReplaceFirst , const char* , const String& , size_t );	
	implement_duplicate_internal_result_function_3_with_const( ::rux::String& , String , ReplaceLast , const char , const char , size_t );	
	implement_duplicate_internal_result_function_3_with_const( ::rux::String& , String , ReplaceLast , const char* , const char* , size_t );	
	implement_duplicate_internal_result_function_3_with_const( ::rux::String& , String , ReplaceLast , const ::rux::XString& , const ::rux::XString& , size_t );
	implement_duplicate_internal_result_function_3_with_const( ::rux::String& , String , ReplaceLast , const ::rux::String& , const ::rux::String& , size_t );
	implement_duplicate_internal_result_function_3_with_const( ::rux::String& , String , ReplaceLast , const ::rux::XString& , const char* , size_t );
	implement_duplicate_internal_result_function_3_with_const( ::rux::String& , String , ReplaceLast , const ::rux::XString& , const String& , size_t );
	implement_duplicate_internal_result_function_3_with_const( ::rux::String& , String , ReplaceLast , const String& , const ::rux::XString& , size_t );
	implement_duplicate_internal_result_function_3_with_const( ::rux::String& , String , ReplaceLast , const String& , const char* , size_t );
	implement_duplicate_internal_result_function_3_with_const( ::rux::String& , String , ReplaceLast , const char* , const ::rux::XString& , size_t );
	implement_duplicate_internal_result_function_3_with_const( ::rux::String& , String , ReplaceLast , const char* , const String& , size_t );
	implement_duplicate_internal_result_function_2_with_const( ::rux::String& , String , ToLower , size_t , size_t );
	implement_duplicate_internal_result_function_2_with_const( ::rux::String& , String , ToUpper , size_t , size_t );
	implement_duplicate_internal_result_function_1_with_const( ::rux::uint16 , String , get_UTF16Char , size_t );
	implement_duplicate_internal_result_function_1_with_const( char , String , get_AnsiChar , size_t );
	implement_duplicate_internal_result_function( bool , String , IsInt32 );
	implement_duplicate_internal_result_function( bool , String , IsDouble );
	implement_duplicate_internal_result_function_with_const( ::rux::String& , String , ConvertToUTF16 );
	implement_duplicate_internal_result_function( size_t , String , get_AllocatedMemory );
	implement_duplicate_internal_function_1( String , IncreaseMemory , size_t );




	size_t String::Length() const
	{
		if( _str )
		{
			if( _code_page == XEnumCodePage_UTF16 )
				return ( _count / sizeof( ::rux::uint16 ) ) - 1;
			else if( _code_page == XEnumCodePage_CP1251 )
				return _count - 1;
			else if( _code_page == XEnumCodePage_UTF8 )	
			{
				if( _count != 1 && _utf8_length == 0 )
					( const_cast< String* >( this ) )->_utf8_length = XString::LengthUTF8( str() );
				return _utf8_length;
			}
		}
		return 0;
	};
	#ifdef __WINDOWS__
	implement_duplicate_internal_result_function_with_const( BSTR , String , Convert );
	BSTR String::Convert( void ) const
	{
		if( Length() > 0 )
		{
			if( _code_page == XEnumCodePage_CP1251 )
			{
				size_t ansi_to_utf16_bytes = 0;
				wchar_t* wide_char_array = (wchar_t*)XString::ansi_to_utf16( str() , _count - 1 , ansi_to_utf16_bytes );
				BSTR bstr = ::SysAllocString( wide_char_array );
				::rux::engine::free_mem( wide_char_array );
				return bstr;
			}	
			else if( _code_page == XEnumCodePage_UTF16 )
				return ::SysAllocString( (wchar_t*)str() );
			else if( _code_page == XEnumCodePage_UTF8 )	
			{
				size_t utf8_to_utf16_bytes = 0;
				return ::SysAllocString( (wchar_t*)XString::utf8_to_utf16( str() , _count - 1 , utf8_to_utf16_bytes ) );
			}
		}
		return ::SysAllocString( L"\0" );
	};
	#endif
	void String::ConvertToUTF16( ::rux::alloc_t alloc , ::rux::uint16** string_ptr_ptr ) const
	{
		if( string_ptr_ptr )
		{
			if( Length() > 0 )
			{
				if( _code_page == XEnumCodePage_UTF16 )
				{
					*string_ptr_ptr = (::rux::uint16*)alloc( _count );
					::rux::engine::memcpy( *string_ptr_ptr , 0 , str() , _count );
				}
				else if( _code_page == XEnumCodePage_CP1251 )
				{
					*string_ptr_ptr = (::rux::uint16*)alloc( _count * sizeof( ::rux::uint16 ) );
					size_t ansi_to_utf16_bytes = 0;
					XString::ansi_to_utf16( str() , _count - 1 , *string_ptr_ptr , _count , ansi_to_utf16_bytes );	
				}	
				else if( _code_page == XEnumCodePage_UTF8 )
				{	
					*string_ptr_ptr = (::rux::uint16*)alloc( _count * sizeof( ::rux::uint16 ) );
					size_t utf8_to_utf16_bytes = 0;
					XString::utf8_to_utf16( str() , _count - 1 , *string_ptr_ptr , _count , utf8_to_utf16_bytes );
				}
			}
			else
			{
				*string_ptr_ptr = (::rux::uint16*)alloc( sizeof( ::rux::uint16 ) );
				(*string_ptr_ptr)[ 0 ] = 0;
			}
		}
	};
	void XString::x_initialize( void )
	{
	};
	XString::XString()
	{
	};
	XString::XString( size_t capacity , const char* variable_name , const char* __file__ , ::rux::int32 __line__ )
		: XGCHandle< String >( variable_name ? variable_name : "string" , __file__ ? __file__ : __FILE__ , __line__ ? __line__ : __LINE__ )
	{			
		operator()()->ctor_set( capacity );
	};	
	void String::ctor_set( size_t capacity )
	{			
		if( capacity > 0 )
		{
			_count = 1;
			_size = capacity;
			if( capacity > STRING_MAX_STEP )
				_step = STRING_MAX_STEP;
			else
				_step = capacity;
		}
	};	
	XString::XString( const char* text_ptr , size_t start_index , size_t length , XEnumCodePage code_page )
	{	
		operator()()->ctor_set( text_ptr , start_index , length , code_page , 32 );
	};
	void String::ctor_set( const char* text_ptr , size_t start_index , size_t length , XEnumCodePage code_page , size_t step )
	{
		if( step > 0 )
			_step = step;
		if( text_ptr )
		{
			if( code_page == XEnumCodePage_CP1251 )
			{
				if( text_ptr[ 0 ] != 0 )
				{
					size_t local_length = ::strlen( text_ptr );
					if( start_index < local_length )
					{
						length = local_length - start_index > length ? length : local_length - start_index;			
						if( length > 0 )
						{
							_count = length + 1;
							_size += length + get_step();
							_str = alloc_array_macros( char , _size );
							::rux::engine::memcpy( _str , 0 , &text_ptr[ start_index ] , length );
							_str[ length ] = 0;
							_code_page = code_page;
						}
					}
				}
			}		
			else if( code_page == XEnumCodePage_UTF8 )
			{		
				if( text_ptr[ 0 ] != 0 )
				{
					size_t byte_length = XString::utf8_symbol_length_to_byte_length( text_ptr , start_index , length );
					if( byte_length > 0 )
					{
						size_t byte_start_index = XString::utf8_symbol_index_to_byte_index( text_ptr , start_index );
						if( byte_start_index != SIZE_MAX )
						{
							_count = byte_length + 1;
							_size += byte_length + get_step();
							_str = alloc_array_macros( char , _size );
							::rux::engine::memcpy( _str , 0 , &text_ptr[ byte_start_index ] , byte_length );
							_str[ byte_length ] = 0;
							_code_page = code_page;
						}
					}
				}
			}
			else if( code_page == XEnumCodePage_UTF16 )
			{		
				if( *(::rux::uint16*)text_ptr != 0 )
				{
					size_t local_length = XString::LengthUTF16( (::rux::uint16*)text_ptr );			
					if( local_length > 0 )
					{
						if( start_index < local_length )
						{
							length = local_length - start_index > length ? length : local_length - start_index;			
							if( length > 0 )
							{
								length *= 2;
								_count = length + 2;
								_size += length + get_step();
								_str = alloc_array_macros( char , _size );
								start_index *= 2;
								::rux::engine::memcpy( _str , 0 , &text_ptr[ start_index ] , length );
								*(::rux::uint16*)&_str[ length ] = 0;
								_code_page = code_page;
							}
						}
					}
				}
			}
		}
	};
	void String::set( const char* string_ptr , XEnumCodePage code_page , ::rux::uint8 copy_string )
	{
		if( copy_string == 0 )
			attach( (char*)string_ptr , SIZE_MAX , SIZE_MAX , code_page );
		else
		{
			_declare_stackvar( ::rux::String , attached_string , 0 );
			attached_string.attach( (char*)string_ptr , SIZE_MAX , SIZE_MAX , code_page );
			Clear();
			InsertRange( 0 , attached_string );
			attached_string.detach();
		}
	};
	XString::XString( const char* string_ptr , XEnumCodePage code_page , ::rux::uint8 copy_string , const char* variable_name , const char* __file__ , ::rux::int32 __line__ )
		: XGCHandle< String >( variable_name ? variable_name : "string" , __file__ ? __file__ : __FILE__ , __line__ ? __line__ : __LINE__ )
	{	
		operator()()->set( string_ptr , code_page , copy_string );
	};
	XString::XString( const char* text_ptr )
	{	
		operator()()->ctor_set( text_ptr , 0 , SIZE_MAX , XString::get_global_code_page() , 32 );
	};
	XString::XString( const char* text_ptr , size_t start_index , size_t length , size_t step )
	{	
		operator()()->ctor_set( text_ptr , start_index , length , XString::get_global_code_page() , step );
	};
	XString::XString( const char* text_ptr , size_t start_index , XEnumCodePage code_page )
	{	
		operator()()->ctor_set( text_ptr , start_index , SIZE_MAX , code_page , 32 );
	};
	XString::XString( const char* text_ptr , size_t start_index )
	{	
		operator()()->ctor_set( text_ptr , start_index , SIZE_MAX , XString::get_global_code_page() , 32 );
	};
	void String::ctor_set( const wchar_t* text_ptr , size_t start_index , size_t length , size_t step )
	{	
		if( step > 0 )
			_step = step;
	#ifdef __WINDOWS__
	#pragma warning( push )
	#pragma warning( disable: 4127 )
	#endif
		if( sizeof( wchar_t ) == 2 )
	#ifdef __WINDOWS__
	#pragma warning( pop )
	#endif
		{
			if( text_ptr && ((wchar_t*)text_ptr)[ 0 ] != '\0' )
			{
				size_t local_length = XString::LengthUTF16( (::rux::uint16*)text_ptr );
				if( start_index < local_length )
				{
					length = local_length - start_index > length ? length : local_length - start_index;
					length *= 2;				
					_count = length + 2;
					_size += length + get_step();
					_str = alloc_array_macros( char , _size );
					::rux::engine::memcpy( _str , 0 , &text_ptr[start_index] , length );
					*(::rux::uint16*)&_str[ length ] = 0;
					_code_page = XEnumCodePage_UTF16;	
				}
			}
		}
	};
	XString::XString( const wchar_t* text_ptr )
	{	
		operator()()->ctor_set( text_ptr , 0 , SIZE_MAX , 32 );
	};
	XString::XString( const wchar_t* text_ptr , size_t start_index , size_t length , size_t step )
	{
		operator()()->ctor_set( text_ptr , start_index , length , step );
	};
	XString::XString( const wchar_t* text_ptr , size_t start_index )
	{	
		operator()()->ctor_set( text_ptr , start_index , SIZE_MAX , 32 );
	};
	void String::ctor_set( const String& string , const char* str )
	{
		size_t str_length = 0;
		if( str )
			str_length = ::strlen( str );
		if( string.Length() > 0 )
		{
			size_t count = 0;
			if( string._code_page == XEnumCodePage_UTF16 )
				count = string._count - 2;		
			else
				count = string._count - 1;			
			if( count > 0 )
			{		
				_size += count + 2 * str_length + get_step();
				_str = alloc_array_macros( char , _size );
				if( string._code_page == XEnumCodePage_UTF16 )
					_count = count + 2;
				else if( string._code_page == XEnumCodePage_UTF8
					|| string._code_page == XEnumCodePage_CP1251 )
					_count = count + 1;
				::rux::engine::memcpy( _str , 0 , string.str() , count );
				if( string._code_page == XEnumCodePage_UTF16 )
					*(::rux::uint16*)&_str[ count ] = 0;
				else if( string._code_page == XEnumCodePage_UTF8
					|| string._code_page == XEnumCodePage_CP1251 )
					_str[ count ] = 0;
				_code_page = string._code_page;
			}
		}
		else
		{
			_code_page = string._code_page;
			if( string._code_page == XEnumCodePage_UTF16 )
				_count = 2;
			else
				_count = 1;
		}
		if( str_length > 0 )
			InsertRange( SIZE_MAX , str , str_length );
	};
	void String::ctor_set( const String& string , const wchar_t* str )
	{
		size_t str_length = 0;
		if( str )
			str_length = XString::LengthUTF16( (::rux::uint16*)str );
		if( string.Length() > 0 )
		{
			size_t count = 0;
			if( string._code_page == XEnumCodePage_UTF16 )
				count = string._count - 2;		
			else
				count = string._count - 1;			
			if( count > 0 )
			{		
				_size += count + 2 * str_length + get_step();
				_str = alloc_array_macros( char , _size );
				if( string._code_page == XEnumCodePage_UTF16 )
					_count = count + 2;
				else if( string._code_page == XEnumCodePage_UTF8
					|| string._code_page == XEnumCodePage_CP1251 )
					_count = count + 1;
				::rux::engine::memcpy( _str , 0 , string.str() , count );
				if( string._code_page == XEnumCodePage_UTF16 )
					*(::rux::uint16*)&_str[ count ] = 0;
				else if( string._code_page == XEnumCodePage_UTF8
					|| string._code_page == XEnumCodePage_CP1251 )
					_str[ count ] = 0;
				_code_page = string._code_page;
			}
		}
		else
		{
			_code_page = string._code_page;
			if( string._code_page == XEnumCodePage_UTF16 )
				_count = 2;
			else
				_count = 1;
		}
		if( str_length > 0 )
			InsertRange( SIZE_MAX , str , str_length );
	};
	XString::XString( const String& string )
	{	
		operator()()->ctor_set( string , (char*)0 );
	};
	XString::XString( const ::rux::XString& string )
	{	
		operator()()->ctor_set( *string() , (const char*)0 );
	};
	XString::XString( const XObject& object )
	{	
		*static_cast<XGCHandle<String>*>(this) = object;
	};
	::rux::XString& XString::operator =( const XGCRef* object )
	{
		*static_cast<XGCHandle<String>*>(this) = object;
		return *this;
	};
	size_t String::get_AllocatedMemory( void )
	{
		return _size;
	};
	void String::attach( const char* str , size_t str_size , size_t str_length , XEnumCodePage code_page )
	{
		if( _str )
			::rux::engine::free_mem( _str );
		if( str && str[ 0 ] != 0 )
		{
			if( str_length == SIZE_MAX )
			{
				if( code_page == XEnumCodePage_UTF16 )
					_count = ( XString::LengthUTF16( (::rux::uint16*)str ) + 1 ) * 2;
				else if( code_page == XEnumCodePage_CP1251 || code_page == XEnumCodePage_UTF8 )
					_count = ::strlen( str ) + 1;
			}
			else if( code_page == XEnumCodePage_UTF8 )
			{
				_count = XString::utf8_symbol_length_to_byte_length( str , 0 , str_length );
				if( _count == SIZE_MAX )
					_count = ::strlen( str ) + 1;
				else
					_count++;
			}
			else if( code_page == XEnumCodePage_UTF16 )
				_count = ( str_length + 1 ) * 2;
			else
				_count = str_length + 1;

			if( str_size == SIZE_MAX || str_size < _count )
				str_size = _count;
			_size = str_size;
			_step = 32;
			_code_page = code_page;
			_utf8_length = 0;
			_str = const_cast< char* >( str );
		}
		else
			detach();		
	};
	char* String::detach( void )
	{
		char* str = _str;
		_step = 32;
		_code_page = XString::get_global_code_page();
		_utf8_length = 0;
		_str = 0;
		_size = 0;
		if( _code_page == XEnumCodePage_UTF16 )
			_count = 2;
		else if( _code_page == XEnumCodePage_CP1251 || _code_page == XEnumCodePage_UTF8 )
			_count = 1;
		return str;
	};
	implement_duplicate_internal_function( String , Shrink );
	void String::Shrink( void )
	{
		if( _str )
		{
			if( _count < _size )
			{
				_size = _count;
				char* array_ptr = alloc_array_macros( char , _size );
				::rux::engine::memcpy( array_ptr , 0 , _str , _count );
				::rux::engine::free_mem( _str );
				_str = array_ptr;
			}
		}
	};
	void String::IncreaseMemory( size_t size )
	{
		_size += size;
		if( _str )
		{
			char* array_ptr = alloc_array_macros( char , _size );
			::rux::engine::memcpy( array_ptr , 0 , _str , _count );
			::rux::engine::free_mem( _str );
			_str = array_ptr;
		}
	};
	void String::Insert( size_t index , char symbol )
	{
		_declare_stackvar( ::rux::String , attached_string , 0 );
		attached_string.attach( &symbol , 1 , 1 , XString::get_global_code_page() );
		InsertRange( index , attached_string );
		attached_string.detach();
	};
	void String::InsertRange( size_t index , const ::rux::XString& string )
	{
		InsertRange( index , *string() );
	};
	void String::InsertRange( size_t index , const ::rux::String& string )
	{
		if( string.Length() > 0 )
		{
			if( _str == 0 )
			{
				_code_page = string._code_page;
				if( _code_page == XEnumCodePage_UTF16 )
					_count = 2;
				else
					_count = 1;
			}
			if( string._code_page == XEnumCodePage_UTF16 )
			{
				if( _code_page == XEnumCodePage_CP1251 )
				{
					if( _str )
					{
						if( _size < _count * 2 )
							_size = _count * 2;
						char* new_array_ptr = alloc_array_macros( char , _size );
						size_t ansi_to_utf16_bytes = 0;
						if( _count == 1 )							
							*(::rux::uint16*)&new_array_ptr[ 0 ] = 0;
						else			
							XString::ansi_to_utf16( _str , _count - 1 , (::rux::uint16*)new_array_ptr , _count , ansi_to_utf16_bytes );
						::rux::engine::free_mem( _str );
						_str = new_array_ptr;
						_count = ansi_to_utf16_bytes + 2;
					}
					else
						_count = _count * 2;
					_code_page = XEnumCodePage_UTF16;
				}		
			}
			else if( string._code_page == XEnumCodePage_UTF8 )
			{
				if( _code_page == XEnumCodePage_CP1251 )
				{
					if( _str )
					{
						if( _size < _count * 4 )
							_size = _count * 4;
						char* new_array_ptr = alloc_array_macros( char , _size );
						size_t ansi_to_utf8_length = 0;
						if( _count == 1 )							
							new_array_ptr[ 0 ] = 0;
						else			
							XString::ansi_to_utf8( _str , _count - 1 , new_array_ptr , _count * 4 , ansi_to_utf8_length );
						::rux::engine::free_mem( _str );
						_str = new_array_ptr;
						_count = ansi_to_utf8_length + 1;
					}
					_code_page = XEnumCodePage_UTF8;
				}		
				else if( _code_page == XEnumCodePage_UTF16 )
				{
					if( _str )
					{
						if( _size < _count * 2 )
							_size = _count * 2;
						char* new_array_ptr = alloc_array_macros( char , _size );
						size_t utf16_to_utf8_bytes = 0;
						if( _count == 2 )							
							new_array_ptr[ 0 ] = 0;
						else			
							XString::utf16_to_utf8( (::rux::uint16*)_str , Length() , new_array_ptr , _count * 2 , utf16_to_utf8_bytes );
						::rux::engine::free_mem( _str );
						_str = new_array_ptr;
						_count = utf16_to_utf8_bytes + 1;
					}
					_code_page = XEnumCodePage_UTF8;
				}
				_utf8_length = 0;
			}
			size_t count = 0;
			if( _code_page == XEnumCodePage_UTF16 )
			{
				if( string._code_page == XEnumCodePage_UTF16 )
					count = string._count - 2;
				else
					count = string._count - 1;
				char* source_array_ptr = 0;
				if( _code_page != string._code_page )
				{
					if( string._code_page == XEnumCodePage_CP1251 )
					{
						size_t ansi_to_utf16_bytes = 0;
						source_array_ptr = (char*)XString::ansi_to_utf16( string._str , string._count - 1 , ansi_to_utf16_bytes );
						count *= 2;
					}
				}		
				else
					source_array_ptr = string._str;
				index *= 2;
				if( count > 0 )
				{
					::rux::uint8 is_equal = _str == string._str ? 1 : 0;	
					if( index > _count - 2 )
						index = _count - 2;
					_count += count;
					size_t index1 = 0;
					if( _count > _size 
						|| is_equal == 1 )
					{
						if( _count > _size )
						{
							if( _count > _step )
								_size += _count;
							else
								_size += get_step();
						}
						char* new_array = alloc_array_macros( char , _size );
						if( _str )
						{
							::rux::engine::memcpy( new_array , 0 , _str , index );
							for( index1 = _count - 1 ; index1 > index + count - 1 ; index1-- )
								new_array[ index1 ] = _str[ index1 - count ];
							::rux::engine::free_mem( _str );
						}
						::rux::engine::memcpy( new_array , index , source_array_ptr , count );
						_str = new_array;
					}	
					else
					{
						if( _str )
						{
							for( index1 = _count - 1 ; index1 > index + count - 1 ; index1-- )
								_str[ index1 ] = _str[ index1 - count ];
						}
						else
							str();
						::rux::engine::memcpy( _str , index , source_array_ptr , count );
					}		
				}
				if( _code_page != string._code_page )
					::rux::engine::free_mem( source_array_ptr );	
			}
			else if( _code_page == XEnumCodePage_CP1251 )
			{
				count = string._count - 1;	
				char* source_array_ptr = string._str;
				if( count > 0 )
				{
					::rux::uint8 is_equal = _str == string._str ? 1 : 0;		
					if( index > _count - 1 )
						index = _count - 1;		
					_count += count;
					size_t index1 = 0;
					if( _count > _size 
						|| is_equal == 1 )
					{
						if( _count > _size )
						{
							if( _count > _step )
								_size += _count;
							else
								_size += get_step();
						}
						char* new_array = alloc_array_macros( char , _size );
						if( _str )
						{
							::rux::engine::memcpy( new_array , 0 , _str , index );
							for( index1 = _count - 1 ; index1 > index + count - 1 ; index1-- )
								new_array[ index1 ] = _str[ index1 - count ];
							::rux::engine::free_mem( _str );
						}
						::rux::engine::memcpy( new_array , index , source_array_ptr , count );
						_str = new_array;
					}	
					else
					{
						if( _str )
						{
							for( index1 = _count - 1 ; index1 > index + count - 1 ; index1-- )
								_str[ index1 ] = _str[ index1 - count ];
						}
						else
							str();
						::rux::engine::memcpy( _str , index , source_array_ptr , count );
					}		
				}
			}	
			else if( _code_page == XEnumCodePage_UTF8 )
			{				
				char* source_array_ptr = NULL;
				if( _code_page != string._code_page )
				{
					if( string._code_page == XEnumCodePage_CP1251 )
					{
						size_t ansi_to_utf8_bytes = 0;
						source_array_ptr = XString::ansi_to_utf8( string._str , string._count - 1 , ansi_to_utf8_bytes );
						count = ansi_to_utf8_bytes;
					}
					else if( string._code_page == XEnumCodePage_UTF16 )
					{
						size_t utf16_to_utf8_bytes = 0;
						source_array_ptr = XString::utf16_to_utf8( (::rux::uint16*)string._str , string.Length() , utf16_to_utf8_bytes );
						count = utf16_to_utf8_bytes;
					}
				}		
				else
				{
					count = string._count - 1;
					source_array_ptr = string._str;
				}			
				index = XString::utf8_symbol_index_to_byte_index( _str , index );
				if( index == SIZE_MAX || index > _count - 1 )
					index = _count - 1;
				if( index != SIZE_MAX && count > 0 )
				{
					::rux::uint8 is_equal = _str == string._str ? 1 : 0;				
					_count += count;
					size_t index1 = 0;
					if( _count > _size 
						|| is_equal == 1 )
					{
						if( _count > _size )
						{
							if( _count > _step )
								_size += _count;
							else
								_size += get_step();
						}
						char* new_array = alloc_array_macros( char , _size );
						if( _str )
						{
							::rux::engine::memcpy( new_array , 0 , _str , index );
							for( index1 = _count - 1 ; index1 > index + count - 1 ; index1-- )
								new_array[ index1 ] = _str[ index1 - count ];
							::rux::engine::free_mem( _str );
						}
						::rux::engine::memcpy( new_array , index , source_array_ptr , count );
						_str = new_array;
					}	
					else
					{
						if( _str )
						{
							for( index1 = _count - 1 ; index1 > index + count - 1 ; index1-- )
								_str[ index1 ] = _str[ index1 - count ];
						}
						else
							str();
						::rux::engine::memcpy( _str , index , source_array_ptr , count );
					}		
				}
				if( _code_page != string._code_page )
					::rux::engine::free_mem( source_array_ptr );		
				_utf8_length = 0;
			}
		}
	};
	void String::InsertRange( size_t index , const wchar_t* string_ptr , size_t count )
	{
		_declare_stackvar( ::rux::String , attached_string , 0 );
		attached_string.attach( (char*)string_ptr , count , count , XEnumCodePage_UTF16 );
		InsertRange( index , attached_string );
		attached_string.detach();
	};
	void String::InsertRange( size_t index , const char* string_ptr , size_t count )
	{
		_declare_stackvar( ::rux::String , attached_string , 0 );
		attached_string.attach( string_ptr , count , count , XString::get_global_code_page() );
		InsertRange( index , attached_string );
		attached_string.detach();
	};
	void String::RemoveAt( size_t index )
	{
		RemoveRange( index , 1 );
	};
	void String::RemoveRange( size_t index , size_t count )
	{
		if( Length() > 0 )
		{
			if( _code_page == XEnumCodePage_UTF16 )		
			{
				size_t local_count = ( _count / 2 ) - 1;
				if( index < local_count
					&& count > 0 )
				{
					if( count == SIZE_MAX || index + count > local_count )
						count = local_count - index;						
					for( ; index < _count / 2 ; index++ )
					{
						if( index + count < _count / 2 )
							*(::rux::uint16*)&_str[ index * 2 ] = *(::rux::uint16*)&_str[ ( index + count ) * 2 ];
						else
							break;
					}			
					_count -= count * 2;
				}
			}
			else if( _code_page == XEnumCodePage_CP1251 )
			{
				size_t local_count = _count - 1;			
				if( index < local_count
					&& count > 0 )
				{
					if( count == SIZE_MAX || index + count > local_count )
						count = local_count - index;						
					for( ; index < _count ; index++ )
					{
						if( index + count < _count )
							_str[ index ] = _str[ index + count ];
						else
							break;
					}			
					_count -= count;
				}
			}
			else if( _code_page == XEnumCodePage_UTF8 )
			{
				size_t local_count = _count - 1;			
				size_t byte_length = XString::utf8_symbol_length_to_byte_length( _str , index , count );
				index = XString::utf8_symbol_index_to_byte_index( _str , index );
				if( index != SIZE_MAX && index < local_count && byte_length > 0 )
				{
					if( index + byte_length > local_count )
						byte_length = local_count - index;						
					for( ; index < _count ; index++ )
					{
						if( index + byte_length < _count )
							_str[ index ] = _str[ index + byte_length ];
						else
							break;
					}			
					_count -= byte_length;
				}
				_utf8_length = 0;
			}
		}
	};	
	void String::Clear()
	{	
		if( _code_page == XEnumCodePage_UTF16 )
		{
			if( _str )
				*(::rux::uint16*)&_str[ 0 ] = 0;
			_count = 2;
		}
		else
		{
			if( _str )
				_str[ 0 ] = 0;
			_count = 1;	
		}
		_utf8_length = 0;
	}
	size_t XString::Length() const
	{
		return operator()()->Length();
	};
	size_t String::Size() const
	{	
		return _count;	
	};
	::rux::int32 String::IndexOf( char letter , size_t index )
	{
		_declare_stackvar( ::rux::String , attached_string , 0 );
		attached_string.attach( &letter , 1 , 1 , XString::get_global_code_page() );
		::rux::int32 res = IndexOf( attached_string , index );
		attached_string.detach();
		return res;
	};
	::rux::int32 String::IndexOf( const char* text_ptr , size_t index )
	{	
		_declare_stackvar( String , attached_string , 0 );
		attached_string.attach( text_ptr , SIZE_MAX , SIZE_MAX , XString::get_global_code_page() );
		::rux::int32 res = IndexOf( attached_string , index );
		attached_string.detach();
		return res;
	};
	::rux::int32 String::IndexOf( const ::rux::XString& string , size_t index ) const
	{
		return IndexOf( *string() , index );
	};
	::rux::int32 String::IndexOf( const String& string , size_t index ) const
	{
		::rux::int32 local_index = -1;
		size_t length_this = Length();
		if( length_this > 0 && length_this >= string.Length() )
		{
			if( _code_page == XEnumCodePage_UTF16 )
			{
				::rux::uint16* utf16_ptr = (::rux::uint16*)string._str;
				size_t utf16_ptr_count = string._count - 2;
				if( string._code_page == XEnumCodePage_CP1251 )
				{
					size_t ansi_to_utf16_bytes = 0;
					utf16_ptr = XString::ansi_to_utf16( string._str , string._count - 1 , ansi_to_utf16_bytes );
					utf16_ptr_count = ansi_to_utf16_bytes;
				}
				else if( string._code_page == XEnumCodePage_UTF8 )
				{
					size_t utf8_to_utf16_bytes = 0;
					utf16_ptr = XString::utf8_to_utf16( string._str , string._count - 1 , utf8_to_utf16_bytes );
					utf16_ptr_count = utf8_to_utf16_bytes;
				}
				index *= 2;
				for( ; index < _count - 2 ; index += 2 )
				{
					if( index + utf16_ptr_count <= _count - 2 )
					{
						if( memcmp( &_str[ index ] , utf16_ptr , utf16_ptr_count ) == 0 )
						{
							local_index = index / 2;
							break;
						}
					}
					else
						break;
				}
				if( string._code_page != XEnumCodePage_UTF16 )
					::rux::engine::free_mem( utf16_ptr );			
			}
			else if( _code_page == XEnumCodePage_CP1251 )
			{
				if( string._code_page == XEnumCodePage_UTF16 )
				{
					size_t ansi_to_utf16_bytes = 0;
					char* utf16_ptr = (char*)XString::ansi_to_utf16( _str , _count - 1 , ansi_to_utf16_bytes );
					size_t utf16_ptr_count = ansi_to_utf16_bytes;
					index *= 2;
					for( ; index < utf16_ptr_count ; index += 2 )
					{
						if( index + string._count - 2 <= utf16_ptr_count )
						{
							if( memcmp( &utf16_ptr[ index ] , string._str , string._count - 2 ) == 0 )
							{
								local_index = index / 2;
								break;
							}
						}
						else
							break;
					}			
					::rux::engine::free_mem( utf16_ptr );
				}
				else if( string._code_page == XEnumCodePage_CP1251 )
				{
					for( ; index < _count - 1 ; index ++ )
					{
						if( index + string._count - 1 <= _count - 1 )
						{
							if( memcmp( &_str[ index ] , string._str , string._count - 1 ) == 0 )
							{
								local_index = index;
								break;
							}
						}
						else
							break;
					}			
				}
				else if( string._code_page == XEnumCodePage_UTF8 )
				{
					size_t ansi_to_utf8_bytes = 0;
					char* 	utf8_ptr = XString::ansi_to_utf8( _str , _count - 1 , ansi_to_utf8_bytes );
					size_t utf8_ptr_count = ansi_to_utf8_bytes;
					local_index = index;
					index = XString::utf8_symbol_index_to_byte_index( utf8_ptr , index );
					if( index != SIZE_MAX )
					{
						size_t symbol_length0 = 0;
						for( ; index < utf8_ptr_count ; index += symbol_length0 )
						{
							symbol_length0 = XString::utf8_symbol_length( utf8_ptr , index );
							if( index + string._count - 1 <= utf8_ptr_count )
							{
								if( memcmp( &utf8_ptr[ index ] , string._str , string._count - 1 ) == 0 )
									break;
							}
							else
							{
								local_index = -1;
								break;
							}
							local_index++;
						}
						if( index >= utf8_ptr_count )
							local_index = -1;
					}
					else
						local_index = -1;
					::rux::engine::free_mem( utf8_ptr );
				}
			}		
			else if( _code_page == XEnumCodePage_UTF8 )
			{
				char* 	utf8_ptr = string._str;
				size_t utf8_ptr_count = string._count - 1;
				if( string._code_page == XEnumCodePage_CP1251 )
				{
					size_t ansi_to_utf8_bytes = 0;
					utf8_ptr = XString::ansi_to_utf8( string._str , string._count - 1 , ansi_to_utf8_bytes );
					utf8_ptr_count = ansi_to_utf8_bytes;
				}
				else if( string._code_page == XEnumCodePage_UTF16 )
				{
					size_t utf16_to_utf8_bytes = 0;
					utf8_ptr = XString::utf16_to_utf8( (::rux::uint16*)string._str , string.Length() , utf16_to_utf8_bytes );
					utf8_ptr_count = utf16_to_utf8_bytes;
				}
				local_index = index;
				index = XString::utf8_symbol_index_to_byte_index( _str , index );
				if( index != SIZE_MAX )
				{
					size_t symbol_length0 = 0;
					for( ; index < _count - 1 ; index += symbol_length0 )
					{
						symbol_length0 = XString::utf8_symbol_length( _str , index );
						if( index + utf8_ptr_count <= _count - 1 )
						{
							if( memcmp( &_str[ index ] , utf8_ptr , utf8_ptr_count ) == 0 )
								break;
						}
						else
						{
							local_index = -1;
							break;
						}
						local_index++;
					}
					if( index >= _count - 1 )
						local_index = -1;
				}
				else
					local_index = -1;
				if( string._code_page != XEnumCodePage_UTF8 )
					::rux::engine::free_mem( utf8_ptr );
			}
		}
		return local_index;
	};
	implement_duplicate_internal_function_2_with_const( String , CopyToArrayAsCurrentCodePage , char* , size_t );
	
	::rux::int32 String::IndexOfAny( const ::rux::XArray<XChar>& char_array , size_t index )
	{	
		size_t array_index = 0;
		return IndexOfAny( char_array , index , array_index );
	};
	::rux::int32 String::IndexOfAny( const ::rux::XArray<XString>& string_array , size_t index )
	{	
		size_t array_index = 0;
		return IndexOfAny( string_array , index , array_index );
	};
	::rux::int32 String::IndexOfAny( const ::rux::XArray<XChar>& char_array , size_t index , size_t& array_index )
	{
		::rux::int32 local_index = -1;	
		if( Length() && char_array.Count() > 0 )
		{
			if( _code_page == XEnumCodePage_UTF16 )
			{	
				size_t index1 = 0;
				char symbol = ' ';
				_declare_stackvar( ::rux::uint16 , wide_symbol11 , 0 );
				wide_symbol11 = 0;
				index *= 2;
				for( ; index <= _count - 2 ; index += 2 )
				{
					for( index1 = 0 ; index1 < char_array.Count() ; index1++ )
					{
						symbol = char_array[ index1 ]._char;
						if( XString::get_global_code_page() == XEnumCodePage_CP1251 )
						{
							size_t ansi_to_utf16_bytes = 0;
							XString::ansi_to_utf16( &symbol , 1 , &wide_symbol11 , 1 , ansi_to_utf16_bytes );
						}
						else if( XString::get_global_code_page() == XEnumCodePage_UTF8 )
						{
							size_t utf8_to_utf16_bytes = 0;
							XString::utf8_to_utf16( &symbol , 1 , &wide_symbol11 , 1 , utf8_to_utf16_bytes );
						}
						if( *(::rux::uint16*)&_str[ index ] == wide_symbol11 )
						{
							local_index = index / 2;
							array_index = index1;
							break;					
						}
					}
					if( local_index > -1 )				
						break;
				}
			}
			else if( _code_page == XEnumCodePage_CP1251 )
			{
				size_t index1 = 0;
				_declare_stackvar( char , symbol00 , 0 );
				symbol00 = 0;
				char symbol1 = ' ';			
				for( ; index <= _count - 1 ; index++ )
				{
					for( index1 = 0 ; index1 < char_array.Count() ; index1++ )
					{
						if( XString::get_global_code_page() == XEnumCodePage_UTF8 )
						{
							symbol1 = char_array[ index1 ]._char;
							size_t utf8_to_ansi_byte = 0;
							XString::utf8_to_ansi( &symbol1 , 1 , &symbol00 , 1 , utf8_to_ansi_byte );
						}
						else if( XString::get_global_code_page() == XEnumCodePage_CP1251 )
							symbol00 = char_array[ index1 ]._char;
						if( _str[ index ] == symbol00 )
						{
							local_index = index;
							array_index = index1;
							break;					
						}
					}
					if( local_index > -1 )
						break;
				}
			}
			else if( _code_page == XEnumCodePage_UTF8 )
			{
				local_index = index;
				index = XString::utf8_symbol_index_to_byte_index( _str , index );
				if( index != SIZE_MAX )
				{
					size_t index1 = 0;			
					size_t symbol_length0 = 0 , symbol_length1 = 0;
					declare_stack_variable( char , utf8_symbol , 4 );
					char symbol0 = ' ';
					for( ; index < _count - 1 ; index += symbol_length0 )
					{
						symbol_length0 = XString::utf8_symbol_length( _str , index );
						for( index1 = 0 ; index1 < char_array.Count() ; index1++ )
						{
							if( XString::get_global_code_page() == XEnumCodePage_UTF8 )
							{
								symbol_length1 = 1;
								utf8_symbol[ 0 ] = char_array[ index1 ]._char;
							}
							else if( XString::get_global_code_page() == XEnumCodePage_CP1251 )
							{
								symbol0 = char_array[ index1 ]._char;
								size_t ansi_to_utf8_length = 0;
								XString::ansi_to_utf8( &symbol0 , 1 , utf8_symbol , 4 , ansi_to_utf8_length );
								symbol_length1 = ansi_to_utf8_length;
							}
							if( index + symbol_length1 <= _count - 1 )
							{
								if( memcmp( &_str[ index ] , utf8_symbol , symbol_length1 ) == 0 )				
								{
									array_index = index1;
									break;
								}
							}
						}
						if( index1 < char_array.Count() )
							break;
						local_index++;
					}
					if( index >= _count - 1 )
						local_index = -1;
				}
				else 
					local_index = -1;
			}
		}
		return local_index;
	};
	::rux::int32 String::IndexOfAny( const ::rux::XArray<XString>& string_array , size_t index , size_t& array_index )
	{
		::rux::int32 local_index = -1;	
		if( Length() > 0 && string_array.Count() > 0 )
		{		
			if( _code_page == XEnumCodePage_UTF16 )
			{
				XMallocArray<void*> utf16_ptrs;
				size_t utf16_count = 0;
				size_t index1 = 0;
				char* utf16_ptr = NULL;
				index *= 2;
				for( ; index < _count - 2 ; index += 2 )
				{
					for( index1 = 0 ; index1 < string_array.Count() ; index1++ )
					{
						if( index1 >= utf16_ptrs.Count() )
						{
							if( string_array[ index1 ]._code_page == XEnumCodePage_UTF16 )
								utf16_ptrs.Add( string_array[ index1 ]._str );
							else if( string_array[ index1 ]._code_page == XEnumCodePage_CP1251 )
							{
								size_t ansi_to_utf16_bytes = 0;
								utf16_ptr = (char*)XString::ansi_to_utf16( string_array[ index1 ]._str , string_array[ index1 ]._count - 1 , ansi_to_utf16_bytes );
								utf16_ptrs.Add( utf16_ptr );
							}
							else if( string_array[ index1 ]._code_page == XEnumCodePage_UTF8 )
							{
								size_t utf8_to_utf16_bytes = 0;
								utf16_ptr = (char*)XString::utf8_to_utf16( string_array[ index1 ]._str , string_array[ index1 ]._count - 1 , utf8_to_utf16_bytes );
								utf16_ptrs.Add( utf16_ptr );
							}
						}
						utf16_ptr = (char*)utf16_ptrs[ index1 ];
						utf16_count = XString::LengthUTF16( (::rux::uint16*)utf16_ptr ) * 2;
						if( utf16_count > 0 && index + utf16_count <= _count - 2 )
						{
							if( memcmp( &_str[ index ] , utf16_ptr , utf16_count ) == 0 )
							{
								local_index = index / 2;
								array_index = index1;
								break;
							}
						}
					}
					if( local_index > -1 )
						break;
				}
				for( index1 = 0 ; index1 < utf16_ptrs.Count() ; index1++ )
				{
					if( string_array[ index1 ]._code_page != XEnumCodePage_UTF16 )
						::rux::engine::free_mem( utf16_ptrs[ index1 ] );
				}
			}
			else if( _code_page == XEnumCodePage_UTF8 )
			{			
				local_index = index;
				index = XString::utf8_symbol_index_to_byte_index( _str , index );
				if( index != SIZE_MAX )
				{
					XMallocArray<void*> utf8_ptrs;
					XMallocArray< size_t > utf8_ptrs_bytes;
					size_t utf8_count = 0;
					size_t index1 = 0;
					char* utf8_ptr = NULL;
					size_t symbol_length0 = 0;
					for( ; index < _count - 1 ; index += symbol_length0 )
					{
						symbol_length0 = XString::utf8_symbol_length( _str , index );
						for( index1 = 0 ; index1 < string_array.Count() ; index1++ )
						{
							if( index1 >= utf8_ptrs.Count() )
							{
								if( string_array[ index1 ]._code_page == XEnumCodePage_UTF16 )
								{
									size_t utf16_to_utf8_bytes = 0;
									utf8_ptr = XString::utf16_to_utf8( (::rux::uint16*)string_array[ index1 ]._str , string_array[ index1 ].Length() , utf16_to_utf8_bytes );
									utf8_ptrs.Add( utf8_ptr );
									utf8_ptrs_bytes.Add( utf16_to_utf8_bytes );
								}
								else if( string_array[ index1 ]._code_page == XEnumCodePage_CP1251 )
								{
									size_t ansi_to_utf8_bytes = 0;
									utf8_ptr = XString::ansi_to_utf8( string_array[ index1 ]._str , string_array[ index1 ]._count - 1 , ansi_to_utf8_bytes );
									utf8_ptrs.Add( utf8_ptr );
									utf8_ptrs_bytes.Add( ansi_to_utf8_bytes );
								}
								else if( string_array[ index1 ]._code_page == XEnumCodePage_UTF8 )
								{
									utf8_ptrs.Add( string_array[ index1 ]._str );
									utf8_ptrs_bytes.Add( string_array[ index1 ]._count - 1 );
								}
							}
							utf8_ptr = (char*)utf8_ptrs[ index1 ];
							utf8_count = utf8_ptrs_bytes[ index1 ];
							if( utf8_count > 0 && index + utf8_count <= _count - 1 )
							{
								if( memcmp( &_str[ index ] , utf8_ptr , utf8_count ) == 0 )					
								{
									array_index = index1;
									break;
								}
							}
						}
						if( index1 < string_array.Count() )
							break;
						local_index++;
					}
					if( index >= _count - 1 )
						local_index = -1;
					for( index1 = 0 ; index1 < utf8_ptrs.Count() ; index1++ )
					{
						if( string_array[ index1 ]._code_page != XEnumCodePage_UTF8 )
							::rux::engine::free_mem( utf8_ptrs[ index1 ] );
					}
				}
				else 
					local_index = -1;
			}
			else if( _code_page == XEnumCodePage_CP1251 )
			{			
				size_t index1 = 0;
				::rux::uint8 is_utf8 = 0 , is_utf16 = 0;
				for( index1 = 0 ; index1 < string_array.Count() ; index1++ )
				{
					if( string_array[ index1 ]._code_page == XEnumCodePage_UTF16 )
						is_utf16 = 1;
					else if( string_array[ index1 ]._code_page == XEnumCodePage_UTF8 )
					{
						is_utf8 = 1;
						break;
					}
				}
				if( is_utf8 == 1 )
				{
					size_t ansi_to_utf8_bytes = 0;
					char* utf8_this_ptr = XString::ansi_to_utf8( _str , _count - 1 , ansi_to_utf8_bytes );
					size_t utf8_this_ptr_count = ansi_to_utf8_bytes;
					local_index = index;
					index = XString::utf8_symbol_index_to_byte_index( utf8_this_ptr , index );
					if( index != SIZE_MAX )
					{
						XMallocArray<void*> utf8_ptrs;
						XMallocArray< size_t > utf8_ptrs_bytes;
						size_t utf8_count = 0;
						char* utf8_ptr = NULL;
						size_t symbol_length0 = 0;
						for( ; index < utf8_this_ptr_count ; index += symbol_length0 )
						{
							symbol_length0 = XString::utf8_symbol_length( utf8_this_ptr , index );
							for( index1 = 0 ; index1 < string_array.Count() ; index1++ )
							{
								if( index1 >= utf8_ptrs.Count() )
								{
									if( string_array[ index1 ]._code_page == XEnumCodePage_UTF16 )
									{
										size_t utf16_to_utf8_bytes = 0;
										utf8_ptr = XString::utf16_to_utf8( (::rux::uint16*)string_array[ index1 ]._str , string_array[ index1 ].Length() , utf16_to_utf8_bytes );
										utf8_ptrs.Add( utf8_ptr );
										utf8_ptrs_bytes.Add( utf16_to_utf8_bytes );
									}
									else if( string_array[ index1 ]._code_page == XEnumCodePage_CP1251 )
									{
										size_t ansi_to_utf8_bytes = 0;
										utf8_ptr = XString::ansi_to_utf8( string_array[ index1 ]._str , string_array[ index1 ]._count - 1 , ansi_to_utf8_bytes );
										utf8_ptrs.Add( utf8_ptr );
										utf8_ptrs_bytes.Add( ansi_to_utf8_bytes );
									}
									else if( string_array[ index1 ]._code_page == XEnumCodePage_UTF8 )
									{
										utf8_ptrs.Add( string_array[ index1 ]._str );
										utf8_ptrs_bytes.Add( string_array[ index1 ]._count - 1 );
									}
								}
								utf8_ptr = (char*)utf8_ptrs[ index1 ];
								utf8_count = utf8_ptrs_bytes[ index1 ];
								if( utf8_count > 0 && index + utf8_count <= utf8_this_ptr_count )
								{
									if( memcmp( &utf8_this_ptr[ index ] , utf8_ptr , utf8_count ) == 0 )						
									{
										array_index = index1;
										break;
									}
								}
							}
							if( index1 < string_array.Count() )
								break;
							local_index++;
						}
						if( index >= utf8_this_ptr_count )
							local_index = -1;
						for( index1 = 0 ; index1 < utf8_ptrs.Count() ; index1++ )
						{
							if( string_array[ index1 ]._code_page != XEnumCodePage_UTF8 )
								::rux::engine::free_mem( utf8_ptrs[ index1 ] );
						}
					}
					else
						local_index = -1;
					::rux::engine::free_mem( utf8_this_ptr );
				}
				else if( is_utf16 == 1 )
				{
					size_t ansi_to_utf16_bytes = 0;
					char* utf16_this_ptr = (char*)XString::ansi_to_utf16( _str , _count - 1 , ansi_to_utf16_bytes );
					size_t utf16_this_ptr_count = ansi_to_utf16_bytes;
					XMallocArray<void*> utf16_ptrs;
					size_t utf16_count = 0;				
					char* utf16_ptr = NULL;
					index *= 2;
					for( ; index < utf16_this_ptr_count ; index += 2 )
					{
						for( index1 = 0 ; index1 < string_array.Count() ; index1++ )
						{
							if( index1 >= utf16_ptrs.Count() )
							{
								if( string_array[ index1 ]._code_page == XEnumCodePage_UTF16 )
									utf16_ptrs.Add( string_array[ index1 ]._str );
								else if( string_array[ index1 ]._code_page == XEnumCodePage_CP1251 )
								{
									size_t ansi_to_utf16_bytes = 0;
									utf16_ptr = (char*)XString::ansi_to_utf16( string_array[ index1 ]._str , string_array[ index1 ]._count - 1 , ansi_to_utf16_bytes );
									utf16_ptrs.Add( utf16_ptr );
								}
								else if( string_array[ index1 ]._code_page == XEnumCodePage_UTF8 )
								{
									size_t utf8_to_utf16_bytes = 0;
									utf16_ptr = (char*)XString::utf8_to_utf16( string_array[ index1 ]._str , string_array[ index1 ]._count - 1 , utf8_to_utf16_bytes );
									utf16_ptrs.Add( utf16_ptr );
								}
							}
							utf16_ptr = (char*)utf16_ptrs[ index1 ];
							utf16_count = XString::LengthUTF16( (::rux::uint16*)utf16_ptr ) * 2;
							if( utf16_count > 0 && index + utf16_count <= utf16_this_ptr_count )
							{
								if( memcmp( &utf16_this_ptr[ index ] , utf16_ptr , utf16_count ) == 0 )
								{
									local_index = index / 2;
									array_index = index1;
									break;
								}
							}
						}
						if( local_index > -1 )
							break;
					}
					for( index1 = 0 ; index1 < utf16_ptrs.Count() ; index1++ )
					{
						if( string_array[ index1 ]._code_page != XEnumCodePage_UTF16 )
							::rux::engine::free_mem( utf16_ptrs[ index1 ] );
					}
					::rux::engine::free_mem( utf16_this_ptr );
				}
				else
				{
					for( ; index < _count - 1 ; index ++ )
					{
						for( index1 = 0 ; index1 < string_array.Count() ; index1++ )
						{						
							if( string_array[ index1 ]._count - 1 > 0 && index + string_array[ index1 ]._count - 1 <= _count - 1 )
							{
								if( memcmp( &_str[ index ] , string_array[ index1 ]._str , string_array[ index1 ]._count - 1 ) == 0 )
								{
									local_index = index;
									array_index = index1;
									break;
								}
							}
						}
						if( local_index > -1 )
							break;
					}
				}
			}	
		}
		return local_index;
	};
	::rux::int32 String::LastIndexOf( char letter , size_t index ) const
	{
		_declare_stackvar( ::rux::String , attached_string , 0 );
		attached_string.attach( &letter , 1 , 1 , XString::get_global_code_page() );
		::rux::int32 res = LastIndexOf( attached_string , index );
		attached_string.detach();
		return res;
	};
	::rux::int32 String::LastIndexOf( const char* text_ptr , size_t index ) const
	{
		_declare_stackvar( ::rux::String  , attached_string , 0 );
		attached_string.attach( text_ptr, SIZE_MAX , SIZE_MAX , XString::get_global_code_page() );
		::rux::int32 res = LastIndexOf( attached_string , index );
		attached_string.detach();
		return res;
	};
	::rux::int32 String::LastIndexOf( const ::rux::XString& string , size_t index ) const
	{	
		return LastIndexOf( *string() , index );
	};
	::rux::int32 String::LastIndexOf( const String& string , size_t index ) const
	{
		::rux::int32 local_index = -1;	
		if( Length() > 0 && string.Length() > 0 )
		{
			if( _code_page == XEnumCodePage_UTF16 )
			{			
				::rux::uint16* utf16_ptr = (::rux::uint16*)string._str;
				size_t utf16_ptr_count = string._count - 2;
				if( string._code_page == XEnumCodePage_CP1251 )
				{
					size_t ansi_to_utf16_bytes = 0;
					utf16_ptr = XString::ansi_to_utf16( string._str , string._count - 1 , ansi_to_utf16_bytes );
					utf16_ptr_count = ansi_to_utf16_bytes;
				}
				else if( string._code_page == XEnumCodePage_UTF8 )
				{
					size_t utf8_to_utf16_bytes = 0;
					utf16_ptr = XString::utf8_to_utf16( string._str , string._count - 1 , utf8_to_utf16_bytes );
					utf16_ptr_count = utf8_to_utf16_bytes;
				}
				index *= 2;
				if( index > _count - 4 )
					index = _count - 4;
				if( utf16_ptr_count - 2 <= index )
				{
					index -= utf16_ptr_count - 2;
					for( ; ; index -= 2 )
					{					
						if( memcmp( &_str[ index ] , utf16_ptr , utf16_ptr_count ) == 0 )
						{
							local_index = index / 2;
							break;
						}
						if( index == 0 )
							break;
					}
				}
				if( string._code_page != XEnumCodePage_UTF16 )
					::rux::engine::free_mem( utf16_ptr );			
			}
			else if( _code_page == XEnumCodePage_CP1251 )
			{
				if( string._code_page == XEnumCodePage_UTF16 )
				{
					size_t ansi_to_utf16_bytes = 0;
					char* utf16_ptr = (char*)XString::ansi_to_utf16( _str , _count - 1 , ansi_to_utf16_bytes );
					size_t utf16_ptr_count = ansi_to_utf16_bytes;
					index *= 2;
					if( index > utf16_ptr_count - 2 )
						index = utf16_ptr_count - 2;
					if( string._count - 4 <= index )
					{
						index -= string._count - 4;
						for( ; ; index -= 2 )
						{						
							if( memcmp( &utf16_ptr[ index ] , string._str , string._count - 2 ) == 0 )
							{
								local_index = index / 2;
								break;
							}
							if( index == 0 )
								break;
						}			
					}
					::rux::engine::free_mem( utf16_ptr );
				}
				else if( string._code_page == XEnumCodePage_CP1251 )
				{
					if( index > _count - 2 )
						index = _count - 2;
					if( string._count - 2 <= index )
					{
						index -= string._count - 2;
						for( ; ; index-- )
						{
							if( memcmp( &_str[ index ] , string._str , string._count - 1 ) == 0 )
							{
								local_index = index;
								break;
							}
							if( index == 0 )
								break;
						}
					}
				}
				else if( string._code_page == XEnumCodePage_UTF8 )
				{
					size_t ansi_to_utf8_bytes = 0;
					char* 	utf8_ptr = XString::ansi_to_utf8( _str , _count - 1 , ansi_to_utf8_bytes );
					size_t utf8_ptr_count = ansi_to_utf8_bytes;
					index = XString::utf8_symbol_index_to_byte_index( utf8_ptr , index );				
					local_index = 0;				
					::rux::int32 local_index1 = -1;
					size_t symbol_length0 = 0 , index0 = 0;
					for( ; index0 < utf8_ptr_count ; index0 += symbol_length0 )
					{
						symbol_length0 = XString::utf8_symbol_length( utf8_ptr , index0 );
						if( index0 <= index )
						{
							if( index0 + string._count - 1 <= utf8_ptr_count )
							{
								if( memcmp( &utf8_ptr[ index0 ] , string._str , string._count - 1 ) == 0 )
									local_index1 = local_index;
							}
							else
								break;
						}
						local_index++;
					}
					local_index = local_index1;
					::rux::engine::free_mem( utf8_ptr );
				}
			}
			else if( _code_page == XEnumCodePage_UTF8 )
			{
				char* 	utf8_ptr = string._str;
				size_t utf8_ptr_count = string._count - 1;
				if( string._code_page == XEnumCodePage_CP1251 )
				{
					size_t ansi_to_utf8_bytes = 0;
					utf8_ptr = XString::ansi_to_utf8( string._str , string._count - 1 , ansi_to_utf8_bytes );
					utf8_ptr_count = ansi_to_utf8_bytes;
				}
				else if( string._code_page == XEnumCodePage_UTF16 )
				{
					size_t utf16_to_utf8_bytes = 0;
					utf8_ptr = XString::utf16_to_utf8( (::rux::uint16*)string._str , string.Length() , utf16_to_utf8_bytes );
					utf8_ptr_count = utf16_to_utf8_bytes;
				}
				index = XString::utf8_symbol_index_to_byte_index( _str , index );
				local_index = 0;			
				::rux::int32 local_index1 = -1;
				size_t symbol_length0 = 0 , index0 = 0;
				for( ; index0 < _count - 1 ; index0 += symbol_length0 )
				{
					symbol_length0 = XString::utf8_symbol_length( _str , index0 );
					if( index0 <= index )
					{
						if( index0 + utf8_ptr_count <= _count - 1 )
						{
							if( memcmp( &_str[ index0 ] , utf8_ptr , utf8_ptr_count ) == 0 )
								local_index1 = local_index;
						}
						else
							break;
					}
					local_index++;
				}
				local_index = local_index1;
				if( string._code_page != XEnumCodePage_UTF8 )
					::rux::engine::free_mem( utf8_ptr );
			}
		}	
		return local_index;
	};
	bool String::Equals( XGCRef* gcref )
	{
		if( gcref == this )
			return true;
		else if( rux_is_object_ptr( gcref , ::rux::XString ) )
		{
			::rux::XString string( *gcref );
			return string == *this;
		}
		else
			return false;
	};
	bool String::Equals( const char* text_ptr , size_t length ) const
	{	
		bool result = false;
		if( text_ptr )
		{
			if( Length() == length )
			{
				if( _code_page == XEnumCodePage_UTF16 )
				{
					size_t count = ::strlen( text_ptr );
					::rux::uint16* source_array_ptr = NULL;
					if( XString::get_global_code_page() == XEnumCodePage_CP1251 )
					{
						size_t ansi_to_utf16_bytes = 0;
						source_array_ptr = XString::ansi_to_utf16( text_ptr , count , ansi_to_utf16_bytes );
					}
					else if( XString::get_global_code_page() == XEnumCodePage_UTF8 )
					{
						size_t utf8_to_utf16_bytes = 0;
						source_array_ptr = XString::utf8_to_utf16( text_ptr , count , utf8_to_utf16_bytes );
					}
					result = XString::IsEqualUTF16( (::rux::uint16*)_str , source_array_ptr ) == 1;
					::rux::engine::free_mem( source_array_ptr );
				}
				else if( _code_page == XEnumCodePage_CP1251 )
				{
					if( XString::get_global_code_page() == XEnumCodePage_CP1251 )
						result = XString::IsEqualA( _str , text_ptr ) == 1;
					else if( XString::get_global_code_page() == XEnumCodePage_UTF8 )
					{
						size_t ansi_to_utf8_bytes = 0;
						char* source_array_ptr = XString::ansi_to_utf8( _str , _count - 1 , ansi_to_utf8_bytes );
						size_t count0 = ansi_to_utf8_bytes;
						size_t count1 = ::strlen( text_ptr );
						result = false;
						if( count0 == count1	)
							result = memcmp( text_ptr , source_array_ptr , count0 ) == 0;
						::rux::engine::free_mem( source_array_ptr );					
					}
				}
				else if( _code_page == XEnumCodePage_UTF8 )
				{					
					char* source_array_ptr = NULL;
					size_t ansi_to_utf8_bytes = 0;
					if( XString::get_global_code_page() == XEnumCodePage_CP1251 )
					{
						size_t count = ::strlen( text_ptr );
						source_array_ptr = XString::ansi_to_utf8( text_ptr , count , ansi_to_utf8_bytes );
					}
					else if( XString::get_global_code_page() == XEnumCodePage_UTF8 )
					{
						source_array_ptr = const_cast<char*>( text_ptr );
						ansi_to_utf8_bytes = ::strlen( source_array_ptr );
					}
					result = false;
					if( _count - 1 == ansi_to_utf8_bytes )
						result = memcmp( _str , source_array_ptr , ansi_to_utf8_bytes ) == 0;
					if( XString::get_global_code_page() != XEnumCodePage_UTF8 )
						::rux::engine::free_mem( source_array_ptr );				
				}
			}
		}
		return result;
	};	
	bool String::Equals( const char* text_ptr )
	{
		return *this == text_ptr;
	};
	bool String::Equals( const String& string )
	{	
		return *this == string;
	};
	bool String::Equals( const ::rux::XString& string )
	{	
		return Equals( *string() );
	};
	::rux::String& XString::Join( const ::rux::XString& string , const ::rux::XArray<XString>& array , size_t start_index , size_t count )
	{	
		return Join( *string() , array , start_index , count );
	};
	::rux::String& XString::Join( const String& string , const ::rux::XArray<XString>& array , size_t start_index , size_t count )
	{
		if( start_index < array.Count() )
		{
			if( count == SIZE_MAX || start_index + count > array.Count() )
				count = array.Count();
			else
				count = start_index + count;
		}
		else 
			count = 0;
		::rux::XString string0;
		if( count > 0 )
		{
			::rux::XArray< ::rux::XString > converted_array;
			XEnumCodePage code_page = string._code_page;
			size_t size = 0;
			::rux::byte first = 1;
			for( size_t index0 = start_index ; index0 < count ; index0++ )
			{
				if( first == 0 )
				{
					if( code_page == XEnumCodePage_UTF16 )
						size += string.Length() * 2;
					else if( code_page == XEnumCodePage_UTF8 )
						size += string.Length() * 4;
					else if( code_page == XEnumCodePage_CP1251 )
						size += string.Length();
				}
				if( code_page == XEnumCodePage_UTF16 )
				{
					size_t length = array[ index0 ].Length();
					if( length > 0 )
					{
						size += length * 2;
						if( array[ index0 ]._code_page == XEnumCodePage_UTF8 )
						{
							size *= 2;
							code_page = XEnumCodePage_UTF8;
						}
					}
				}
				else if( code_page == XEnumCodePage_UTF8 )
					size += array[ index0 ].Length() * 4;
				else if( code_page == XEnumCodePage_CP1251 )
				{
					size_t length = array[ index0 ].Length();
					if( length > 0 )
					{
						size += length;
						if( array[ index0 ]._code_page == XEnumCodePage_UTF16 )
						{
							size *= 2;
							code_page = XEnumCodePage_UTF16;
						}
						else if( array[ index0 ]._code_page == XEnumCodePage_UTF8 )
						{
							size *= 4;
							code_page = XEnumCodePage_UTF8;
						}
					}
				}
				first = 0;
			}
			for( size_t index0 = start_index ; index0 < count ; index0++ )
			{
				if( array[ index0 ]._code_page != code_page )
					converted_array.AddByRef( array[ index0 ].ConvertToCodePage( code_page ) );
				else
					converted_array.AddByRef( array[ index0 ] );
			}
			string0.IncreaseMemory( size );
			::rux::String* string0_that = string0();
			string0_that->_code_page = code_page;
			if( string0_that->_code_page == XEnumCodePage_UTF16 )
				string0_that->_count = 2;
			else
				string0_that->_count = 1;
			::rux::XString code_page_string;
			if( code_page != string._code_page )
				code_page_string.set_ByRef( string.ConvertToCodePage( code_page ) );
			else
				code_page_string.set_ByRef( string );
			first = 1;
			for( size_t index0 = 0 ; index0 < converted_array.Count() ; index0++ )
			{
				if( first == 0 )
					string0 += code_page_string;
				string0 += converted_array[ index0 ];
				first = 0;
			}
		}
		return string0++;	
	};
	implement_duplicate_internal_result_function_with_const( char* , String , str );
	char* String::str( void ) const
	{
		String* string_this = const_cast< String* >( this );
		if( string_this->_str == 0 )
		{
			string_this->_size += get_step();
			string_this->_str = alloc_array_macros( char , _size );
			::memset( string_this->_str , 0 , _size );
		}
		return _str;
	};
	::rux::String& XString::Join( const char* text_ptr , const ::rux::XArray<XString>& array , size_t start_index , size_t count )
	{	
		::rux::XString res_string;
		_declare_stackvar( ::rux::String , attached_string , 0 );
		attached_string.attach( (char*)text_ptr , SIZE_MAX , SIZE_MAX , XString::get_global_code_page() );
		res_string.set_ByRef( Join( attached_string , array , start_index , count ) );
		attached_string.detach();
		return res_string++;
	};
	void XString::Join( const char* text_ptr , const ::rux::XArray<XString>& array , ::rux::XString& destination )
	{
		destination.set_ByRef( Join( text_ptr , array ) );
	};
	::rux::String& String::ReplaceAll( const char* old_text_ptr , const char* new_text_ptr , size_t index ) const
	{
		declare_variable( ::rux::XString , res );
		_declare_stackvar( ::rux::String , old_attached_string , 0 );
		old_attached_string.attach( old_text_ptr, SIZE_MAX , SIZE_MAX , XString::get_global_code_page() );
		_declare_stackvar( ::rux::String , new_attached_string , 0 );
		new_attached_string.attach( new_text_ptr, SIZE_MAX , SIZE_MAX , XString::get_global_code_page() );
		res.set_ByRef( ReplaceAll( old_attached_string , new_attached_string , index ) );
		new_attached_string.detach();
		old_attached_string.detach();
		return res++;
	};
	::rux::String& String::ReplaceAll( char old_letter , char new_letter , size_t index ) const
	{	
		declare_variable( ::rux::XString , res );
		_declare_stackvar( ::rux::String , old_attached_string , 0 );
		old_attached_string.attach( &old_letter, 1 , 1 , XString::get_global_code_page() );
		_declare_stackvar( ::rux::String , new_attached_string , 0 );
		new_attached_string.attach( &new_letter, 1 , 1 , XString::get_global_code_page() );
		res.set_ByRef( ReplaceAll( old_attached_string , new_attached_string , index ) );
		new_attached_string.detach();
		old_attached_string.detach();
		return res++;
	};
	::rux::String& String::ReplaceAll( const ::rux::XString& old_string , const char* new_text_ptr , size_t index ) const
	{	
		declare_variable( ::rux::XString , res );
		_declare_stackvar( ::rux::String , new_attached_string , 0 );
		new_attached_string.attach( new_text_ptr, SIZE_MAX , SIZE_MAX , XString::get_global_code_page() );
		res.set_ByRef( ReplaceAll( *old_string() , new_attached_string , index ) );
		new_attached_string.detach();
		return res++;
	};
	::rux::String& String::ReplaceAll( const ::rux::XString& old_string , const String& new_string , size_t index ) const
	{
		return ReplaceAll( *old_string() , new_string , index );
	};
	::rux::String& String::ReplaceAll( const String& old_string , const ::rux::XString& new_string , size_t index ) const
	{	
		return ReplaceAll( old_string , *new_string() , index );
	};
	::rux::String& String::ReplaceAll( const String& old_string , const ::rux::String& new_string , size_t index ) const
	{	
		if( Length() > 0 && old_string.Length() > 0 )
		{
			XEnumCodePage code_page = _code_page;
			if( code_page != XEnumCodePage_UTF8 )
			{
				if( code_page == XEnumCodePage_CP1251 || old_string._code_page != XEnumCodePage_CP1251 )
					code_page = old_string._code_page;
				if( code_page != XEnumCodePage_UTF8 )
				{
					if( new_string.Length() > 0 )
					{
						if( code_page == XEnumCodePage_CP1251 || new_string._code_page != XEnumCodePage_CP1251 )
							code_page = new_string._code_page;
					}
				}
			}
			if( _code_page != code_page || old_string._code_page != code_page || ( new_string.Length() > 0 && new_string._code_page != code_page ) )
			{
				::rux::XString converted_this;
				if( _code_page != code_page )
					converted_this.set_ByRef( ConvertToCodePage( code_page ) );
				else
					converted_this.set_ByRef( this );
				::rux::XString converted_old_string;
				if( old_string._code_page != code_page )
					converted_old_string.set_ByRef( old_string.ConvertToCodePage( code_page ) );
				else
					converted_old_string.set_ByRef( old_string );
				::rux::XString converted_new_string;
				if( new_string._code_page != code_page )
					converted_new_string.set_ByRef( new_string.ConvertToCodePage( code_page ) );
				else
					converted_new_string.set_ByRef( new_string );
				return converted_this.ReplaceAll( *converted_old_string() , *converted_new_string() , index );
			}
			else
			{
				if( code_page == XEnumCodePage_UTF16 )
				{		
					size_t local_count = _count - 2;				
					char* utf16_old_text_ptr = NULL;
					char* utf16_new_text_ptr = NULL;
					size_t utf16_old_text_ptr_length = 0 , utf16_new_text_ptr_length = 0;
				
					utf16_old_text_ptr = old_string._str;
					utf16_old_text_ptr_length = old_string._count - 2;
					if( new_string.Length() > 0 )
					{
						utf16_new_text_ptr = new_string._str;
						utf16_new_text_ptr_length = new_string._count - 2;
					}
					::rux::XString string1( utf16_new_text_ptr_length > utf16_old_text_ptr_length ? _count + ( ( utf16_new_text_ptr_length - utf16_old_text_ptr_length ) * _count / utf16_old_text_ptr_length ) : _count );
					::rux::String* string1_that = string1();
					string1_that->str();
					string1_that->_code_page = XEnumCodePage_UTF16;		
					size_t index1 = 0;
					index *= 2;
					for( size_t index0 = 0 ; index0 < local_count ; index0 += 2 )
					{
						if( index0 >= index )
						{
							if( index0 + utf16_old_text_ptr_length <= local_count )
							{
								if( memcmp( &_str[ index0 ] , utf16_old_text_ptr , utf16_old_text_ptr_length ) == 0 )
								{
									if( utf16_new_text_ptr )
										::rux::engine::memcpy( string1_that->_str , index1 , utf16_new_text_ptr , utf16_new_text_ptr_length );
									index0 += utf16_old_text_ptr_length - 2;
									index1 += utf16_new_text_ptr_length;
								}
								else
								{
									::rux::engine::memcpy( string1_that->_str , index1 , &_str[ index0 ] , 2 );
									index1 += 2;
								}
							}
							else
							{
								::rux::engine::memcpy( string1_that->_str , index1 , &_str[ index0 ] , local_count - index0 );
								index1 += local_count - index0;
								break;
							}
						}
						else
						{
							::rux::engine::memcpy( string1_that->_str , index1 , &_str[ index0 ] , 2 );
							index1 += 2;
						}
					}	
					*(::rux::uint16*)&string1_that->_str[ index1 ] = 0;
					string1_that->_count = index1 + 2;
					return string1++;
				}
				else if( code_page == XEnumCodePage_CP1251 )
				{
					size_t local_count = _count - 1;							
					size_t old_text_ptr_length = old_string._count - 1 , new_text_ptr_length = new_string._count - 1;
					::rux::XString string1( new_text_ptr_length > old_text_ptr_length ? _count + ( ( new_text_ptr_length - old_text_ptr_length ) * _count / old_text_ptr_length ) : _count );
					::rux::String* string1_that = string1();
					string1_that->_code_page = XEnumCodePage_CP1251;
					string1_that->str();
					size_t index1 = 0;
					for( size_t index0 = 0 ; index0 < local_count ; index0++ )
					{
						if( index0 >= index )
						{
							if( index0 + old_text_ptr_length <= local_count )
							{
								if( memcmp( &_str[ index0 ] , old_string._str , old_text_ptr_length ) == 0 )
								{
									if( new_string.Length() > 0 )
										::rux::engine::memcpy( string1_that->_str , index1 , new_string._str , new_text_ptr_length );
									index0 += old_text_ptr_length - 1;
									index1 += new_text_ptr_length;
								}
								else
								{
									::rux::engine::memcpy( string1_that->_str , index1 , &_str[ index0 ] , 1 );
									index1++;
								}
							}
							else
							{
								::rux::engine::memcpy( string1_that->_str , index1 , &_str[ index0 ] , local_count - index0 );
								index1 += local_count - index0;
								break;
							}
						}
						else
						{
							::rux::engine::memcpy( string1_that->_str , index1 , &_str[ index0 ] , 1 );
							index1++;
						}
					}	
					string1_that->_str[ index1 ] = 0;
					string1_that->_count = index1 + 1;
					return string1++;
				}
				else if( code_page == XEnumCodePage_UTF8 )
				{
					char* utf8_old_text_ptr = old_string._str;
					size_t utf8_old_text_ptr_length = old_string._count - 1;

					char* utf8_new_text_ptr = 0;
					size_t utf8_new_text_ptr_length = 0;
					if( new_string.Length() > 0 )
					{
						utf8_new_text_ptr = new_string._str;
						utf8_new_text_ptr_length = new_string._count - 1;
					}
					::rux::XString string1( utf8_new_text_ptr_length > utf8_old_text_ptr_length ? _count + ( ( utf8_new_text_ptr_length - utf8_old_text_ptr_length ) * _count / utf8_old_text_ptr_length ) : _count );
					::rux::String* string1_that = string1();
					string1_that->str();
					string1_that->_code_page = XEnumCodePage_UTF8;
					size_t index0 = 0 , index1 = 0 , symbol_length0 = 0;
					index = XString::utf8_symbol_index_to_byte_index( _str , index );
					for( ; index0 < _count - 1 ; index0 += symbol_length0 )
					{
						symbol_length0 = XString::utf8_symbol_length( _str , index0 );
						if( index0 >= index )
						{
							if( index0 + utf8_old_text_ptr_length <= _count - 1 )
							{				
								if( memcmp( &_str[ index0 ] , utf8_old_text_ptr , utf8_old_text_ptr_length ) == 0 )
								{
									if( utf8_new_text_ptr )
										::rux::engine::memcpy( string1_that->_str , index1 , utf8_new_text_ptr , utf8_new_text_ptr_length );
									index0 += utf8_old_text_ptr_length - symbol_length0;
									index1 += utf8_new_text_ptr_length;
								}
								else
								{
									::rux::engine::memcpy( string1_that->_str , index1 , &_str[ index0 ] , symbol_length0 );
									index1 += symbol_length0;
								}				
							}
							else
							{
								::rux::engine::memcpy( string1_that->_str , index1 , &_str[ index0 ] , _count - 1 - index0 );
								index1 += _count - 1 - index0;
								break;
							}
						}
						else
						{
							::rux::engine::memcpy( string1_that->_str , index1 , &_str[ index0 ] , symbol_length0 );
							index1 += symbol_length0;
						}				
					}
					string1_that->_str[ index1 ] = 0;
					string1_that->_count = index1 + 1;
					return string1++;
				}
			}
		}
		return XString( *this )++;
	};
	::rux::String& String::ReplaceAll( const String& old_string , const char* new_text_ptr , size_t index ) const
	{	
		declare_variable( ::rux::XString , res );
		_declare_stackvar( ::rux::String , new_attached_string , 0 );
		new_attached_string.attach( new_text_ptr, SIZE_MAX , SIZE_MAX , XString::get_global_code_page() );
		res.set_ByRef( ReplaceAll( old_string , new_attached_string , index ) );
		new_attached_string.detach();
		return res++;
	};
	::rux::String& String::ReplaceAll( const char* old_text_ptr , const ::rux::XString& new_string , size_t index ) const
	{	
		return ReplaceAll( old_text_ptr , *new_string() , index );
	};
	::rux::String& String::ReplaceAll( const char* old_text_ptr , const String& new_string , size_t index ) const
	{	
		declare_variable( ::rux::XString , res );
		_declare_stackvar( ::rux::String , old_attached_string , 0 );
		old_attached_string.attach( old_text_ptr, SIZE_MAX , SIZE_MAX , XString::get_global_code_page() );
		res.set_ByRef( ReplaceAll( old_attached_string , new_string , index ) );
		old_attached_string.detach();
		return res++;
	};
	::rux::String& String::ReplaceAll( const ::rux::XString& old_string , const ::rux::XString& new_string , size_t index ) const
	{
		return ReplaceAll( *old_string() , *new_string() , index );
	};
	::rux::String& String::ReplaceFirst( char old_letter , char new_letter , size_t index ) const
	{
		declare_variable( ::rux::XString , res );
		_declare_stackvar( ::rux::String , old_attached_string , 0 );
		old_attached_string.attach( &old_letter, 1 , 1 , XString::get_global_code_page() );
		_declare_stackvar( ::rux::String , new_attached_string , 0 );
		new_attached_string.attach( &new_letter , 1 , 1 , XString::get_global_code_page() );
		res.set_ByRef( ReplaceFirst( old_attached_string , new_attached_string , index ) );
		new_attached_string.detach();
		old_attached_string.detach();
		return res++;
	};
	::rux::String& String::ReplaceFirst( const char* old_text_ptr , const char* new_text_ptr , size_t index ) const
	{
		declare_variable( ::rux::XString , res );
		_declare_stackvar( ::rux::String , old_attached_string , 0 );
		old_attached_string.attach( old_text_ptr, SIZE_MAX , SIZE_MAX , XString::get_global_code_page() );
		_declare_stackvar( ::rux::String , new_attached_string , 0 );
		new_attached_string.attach( new_text_ptr, SIZE_MAX , SIZE_MAX , XString::get_global_code_page() );
		res.set_ByRef( ReplaceFirst( old_attached_string , new_attached_string , index ) );
		new_attached_string.detach();
		old_attached_string.detach();
		return res++;
	};
	::rux::String& String::ReplaceFirst( const ::rux::XString& old_string , const char* new_text_ptr , size_t index ) const
	{
		return ReplaceFirst( *old_string() , new_text_ptr , index );
	};
	::rux::String& String::ReplaceFirst( const ::rux::XString& old_string , const String& new_string , size_t index ) const
	{
		return ReplaceFirst( *old_string() , new_string , index );
	};
	::rux::String& String::ReplaceFirst( const String& old_string , const ::rux::XString& new_string , size_t index ) const
	{
		return ReplaceFirst( old_string , *new_string() , index );
	};
	::rux::String& String::ReplaceFirst( const String& old_string , const char* new_text_ptr , size_t index ) const
	{
		declare_variable( ::rux::XString , res );
		_declare_stackvar( ::rux::String , new_attached_string , 0 );
		new_attached_string.attach( new_text_ptr, SIZE_MAX , SIZE_MAX , XString::get_global_code_page() );
		res.set_ByRef( ReplaceFirst( old_string , new_attached_string , index ) );
		new_attached_string.detach();
		return res++;
	};
	::rux::String& String::ReplaceFirst( const char* old_text_ptr , const ::rux::XString& new_string , size_t index ) const
	{
		return ReplaceFirst( old_text_ptr , *new_string() , index );
	};
	::rux::String& String::ReplaceFirst( const char* old_text_ptr , const String& new_string , size_t index ) const
	{
		declare_variable( ::rux::XString , res );
		_declare_stackvar( ::rux::String , old_attached_string , 0 );
		old_attached_string.attach( old_text_ptr, SIZE_MAX , SIZE_MAX , XString::get_global_code_page() );
		res.set_ByRef( ReplaceFirst( old_attached_string , new_string , index ) );
		old_attached_string.detach();
		return res++;
	};
	::rux::String& String::ReplaceFirst( const ::rux::XString& old_string , const ::rux::XString& new_string , size_t index ) const
	{
		return ReplaceFirst( *old_string() , *new_string() , index );
	};
	::rux::String& String::ReplaceFirst( const ::rux::String& old_string , const ::rux::String& new_string , size_t index ) const
	{	
		if( Length() > 0 && old_string.Length() > 0 )
		{
			XEnumCodePage code_page = _code_page;
			if( code_page != XEnumCodePage_UTF8 )
			{
				if( code_page == XEnumCodePage_CP1251 || old_string._code_page != XEnumCodePage_CP1251 )
					code_page = old_string._code_page;
				if( code_page != XEnumCodePage_UTF8 )
				{
					if( new_string.Length() > 0 )
					{
						if( code_page == XEnumCodePage_CP1251 || new_string._code_page != XEnumCodePage_CP1251 )
							code_page = new_string._code_page;
					}
				}
			}
			if( _code_page != code_page || old_string._code_page != code_page || ( new_string.Length() > 0 && new_string._code_page != code_page ) )
			{
				::rux::XString converted_this;
				if( _code_page != code_page )
					converted_this.set_ByRef( ConvertToCodePage( code_page ) );
				else
					converted_this.set_ByRef( this );
				::rux::XString converted_old_string;
				if( old_string._code_page != code_page )
					converted_old_string.set_ByRef( old_string.ConvertToCodePage( code_page ) );
				else
					converted_old_string.set_ByRef( old_string );
				::rux::XString converted_new_string;
				if( new_string._code_page != code_page )
					converted_new_string.set_ByRef( new_string.ConvertToCodePage( code_page ) );
				else
					converted_new_string.set_ByRef( new_string );
				return converted_this.ReplaceFirst( *converted_old_string() , *converted_new_string() , index );
			}
			else
			{
				if( _code_page == XEnumCodePage_UTF16 )
				{		
					size_t local_count = _count - 2;				
					char* utf16_old_text_ptr = NULL;
					char* utf16_new_text_ptr = NULL;
					size_t utf16_old_text_ptr_length = 0 , utf16_new_text_ptr_length = 0;
					utf16_old_text_ptr = old_string._str;
					utf16_old_text_ptr_length = old_string._count - 2;
					if( new_string.Length() > 0 )
					{
						utf16_new_text_ptr = new_string._str;
						utf16_new_text_ptr_length = new_string._count - 2;
					}
					::rux::XString string1( utf16_new_text_ptr_length > utf16_old_text_ptr_length ? _count + ( utf16_new_text_ptr_length - utf16_old_text_ptr_length ) : _count );
					::rux::String* string1_that = string1();
					string1_that->str();
					string1_that->_code_page = XEnumCodePage_UTF16;		
					size_t index1 = 0;
					index *= 2;
					for( size_t index0 = 0 ; index0 < local_count ; index0 += 2 )
					{
						if( index0 >= index )
						{
							if( index0 + utf16_old_text_ptr_length <= local_count )
							{
								if( memcmp( &_str[ index0 ] , utf16_old_text_ptr , utf16_old_text_ptr_length ) == 0 )
								{
									if( utf16_new_text_ptr )
										::rux::engine::memcpy( string1_that->_str , index1 , utf16_new_text_ptr , utf16_new_text_ptr_length );
									index0 += utf16_old_text_ptr_length;
									index1 += utf16_new_text_ptr_length;
									::rux::engine::memcpy( string1_that->_str , index1 , &_str[ index0 ] , local_count - index0 );
									index1 += local_count - index0;
									break;
								}
								else
								{
									::rux::engine::memcpy( string1_that->_str , index1 , &_str[ index0 ] , 2 );
									index1 += 2;
								}
							}
							else
							{
								::rux::engine::memcpy( string1_that->_str , index1 , &_str[ index0 ] , local_count - index0 );
								index1 += local_count - index0;
								break;
							}
						}
						else
						{
							::rux::engine::memcpy( string1_that->_str , index1 , &_str[ index0 ] , 2 );
							index1 += 2;
						}
					}	
					*(::rux::uint16*)&string1_that->_str[ index1 ] = 0;
					string1_that->_count = index1 + 2;
					return string1++;
				}
				else if( _code_page == XEnumCodePage_CP1251 )
				{
					size_t local_count = _count - 1;							
					size_t old_text_ptr_length = old_string._count - 1 , new_text_ptr_length = new_string._count - 1;
					::rux::XString string1( new_text_ptr_length > old_text_ptr_length ? _count + ( new_text_ptr_length - old_text_ptr_length ) : _count );
					::rux::String* string1_that = string1();
					string1_that->str();
					string1_that->_code_page = XEnumCodePage_CP1251;
					size_t index1 = 0;
					for( size_t index0 = 0 ; index0 < local_count ; index0++ )
					{
						if( index0 >= index )
						{
							if( index0 + old_text_ptr_length <= local_count )
							{
								if( memcmp( &_str[ index0 ] , old_string._str , old_text_ptr_length ) == 0 )
								{
									if( new_string.Length() > 0 )
										::rux::engine::memcpy( string1_that->_str , index1 , new_string._str , new_text_ptr_length );
									index0 += old_text_ptr_length;
									index1 += new_text_ptr_length;
									::rux::engine::memcpy( string1_that->_str , index1 , &_str[ index0 ] , local_count - index0 );
									index1 += local_count - index0;
									break;
								}
								else
								{
									::rux::engine::memcpy( string1_that->_str , index1 , &_str[ index0 ] , 1 );
									index1++;
								}
							}
							else
							{
								::rux::engine::memcpy( string1_that->_str , index1 , &_str[ index0 ] , local_count - index0 );
								index1 += local_count - index0;
								break;
							}
						}
						else
						{
							::rux::engine::memcpy( string1_that->_str , index1 , &_str[ index0 ] , 1 );
							index1++;
						}
					}	
					string1_that->_str[ index1 ] = '\0';
					string1_that->_count = index1 + 1;
					return string1++;
				}
				else if( _code_page == XEnumCodePage_UTF8 )
				{
					char* utf8_old_text_ptr = old_string._str;					
					size_t utf8_old_text_ptr_length = old_string._count - 1;
					char* utf8_new_text_ptr = 0;
					size_t utf8_new_text_ptr_length = 0;
					if( new_string.Length() > 0 )
					{
						utf8_new_text_ptr = new_string._str;
						utf8_new_text_ptr_length = new_string._count - 1;
					}
					::rux::XString string1( utf8_new_text_ptr_length > utf8_old_text_ptr_length ? _count + ( utf8_new_text_ptr_length - utf8_old_text_ptr_length ) : _count );
					::rux::String* string1_that = string1();
					string1_that->str();
					string1_that->_code_page = XEnumCodePage_UTF8;		
					string1_that->_count = 0;
					size_t index0 = 0 , index1 = 0 , symbol_length0 = 0;
					index = XString::utf8_symbol_index_to_byte_index( _str , index );
					for( ; index0 < _count - 1 ; index0 += symbol_length0 )
					{
						symbol_length0 = XString::utf8_symbol_length( _str , index0 );
						if( index0 >= index )
						{
							if( index0 + utf8_old_text_ptr_length <= _count - 1 )
							{				
								if( memcmp( &_str[ index0 ] , utf8_old_text_ptr , utf8_old_text_ptr_length ) == 0 )
								{
									if( utf8_new_text_ptr )
										::rux::engine::memcpy( string1_that->_str , index1 , utf8_new_text_ptr , utf8_new_text_ptr_length );
									index0 += utf8_old_text_ptr_length;
									index1 += utf8_new_text_ptr_length;
									::rux::engine::memcpy( string1_that->_str , index1 , &_str[ index0 ] , _count - 1 - index0 );
									index1 += _count - 1 - index0;
									break;
								}
								else
								{
									::rux::engine::memcpy( string1_that->_str , index1 , &_str[ index0 ] , symbol_length0 );
									index1 += symbol_length0;
								}				
							}
							else
							{
								::rux::engine::memcpy( string1_that->_str , index1 , &_str[ index0 ] , _count - 1 - index0 );
								index1 += _count - 1 - index0;
								break;
							}
						}
						else
						{
							::rux::engine::memcpy( string1_that->_str , index1 , &_str[ index0 ] , symbol_length0 );
							index1 += symbol_length0;
						}				
					}
					string1_that->_str[ index1 ] = '\0';
					string1_that->_count = index1 + 1;
					return string1++;
				}
			}
		}
		return XString( *this )++;
	};
	::rux::String& String::ReplaceLast( char old_letter , char new_letter , size_t index ) const
	{
		declare_variable( ::rux::XString , res );
		_declare_stackvar( ::rux::String , old_attached_string , 0 );
		old_attached_string.attach( &old_letter, 1 , 1 , XString::get_global_code_page() );
		_declare_stackvar( ::rux::String , new_attached_string , 0 );
		new_attached_string.attach( &new_letter, 1 , 1 , XString::get_global_code_page() );
		res.set_ByRef( ReplaceLast( old_attached_string , new_attached_string , index ) );
		new_attached_string.detach();
		old_attached_string.detach();
		return res++;
	};
	::rux::String& String::ReplaceLast( const char* old_text_ptr , const char* new_text_ptr , size_t index ) const
	{
		declare_variable( ::rux::XString , res );
		_declare_stackvar( ::rux::String , old_attached_string , 0 );
		old_attached_string.attach( old_text_ptr, SIZE_MAX , SIZE_MAX , XString::get_global_code_page() );
		_declare_stackvar( ::rux::String , new_attached_string , 0 );
		new_attached_string.attach( new_text_ptr, SIZE_MAX , SIZE_MAX , XString::get_global_code_page() );
		res.set_ByRef( ReplaceLast( old_attached_string , new_attached_string , index ) );
		new_attached_string.detach();
		old_attached_string.detach();
		return res++;
	};
	::rux::String& String::ReplaceLast( const ::rux::XString& old_string , const char* new_text_ptr , size_t index ) const
	{
		return ReplaceLast( *old_string() , new_text_ptr , index );
	};
	::rux::String& String::ReplaceLast( const ::rux::XString& old_string , const String& new_string , size_t index ) const
	{
		return ReplaceLast( *old_string() , new_string , index );
	};
	::rux::String& String::ReplaceLast( const String& old_string , const ::rux::XString& new_string , size_t index ) const
	{
		return ReplaceLast( old_string , *new_string() , index );
	};
	::rux::String& String::ReplaceLast( const String& old_string , const char* new_string , size_t index ) const
	{
		declare_variable( ::rux::XString , res );
		_declare_stackvar( ::rux::String , new_attached_string , 0 );
		new_attached_string.attach( new_string, SIZE_MAX , SIZE_MAX , XString::get_global_code_page() );
		res.set_ByRef( ReplaceLast( old_string , new_attached_string , index ) );
		new_attached_string.detach();
		return res++;
	};
	::rux::String& String::ReplaceLast( const char* old_text_ptr , const ::rux::XString& new_string , size_t index ) const
	{
		return ReplaceLast( old_text_ptr , *new_string() , index );
	};
	::rux::String& String::ReplaceLast( const char* old_text_ptr , const String& new_string , size_t index ) const
	{
		declare_variable( ::rux::XString , res );
		_declare_stackvar( ::rux::String , old_attached_string , 0 );
		old_attached_string.attach( old_text_ptr, SIZE_MAX , SIZE_MAX , XString::get_global_code_page() );
		res.set_ByRef( ReplaceLast( old_attached_string , new_string , index ) );
		old_attached_string.detach();
		return res++;
	};
	::rux::String& String::ReplaceLast( const ::rux::XString& old_string , const ::rux::XString& new_string , size_t index ) const
	{
		return ReplaceLast( *old_string() , *new_string() , index );
	};
	::rux::String& String::ReplaceLast( const ::rux::String& old_string , const ::rux::String& new_string , size_t index ) const
	{
		if( Length() > 0 && old_string.Length() > 0 )
		{
			XEnumCodePage code_page = _code_page;
			if( code_page != XEnumCodePage_UTF8 )
			{
				if( code_page == XEnumCodePage_CP1251 || old_string._code_page != XEnumCodePage_CP1251 )
					code_page = old_string._code_page;
				if( code_page != XEnumCodePage_UTF8 )
				{
					if( new_string.Length() > 0 )
					{
						if( code_page == XEnumCodePage_CP1251 || new_string._code_page != XEnumCodePage_CP1251 )
							code_page = new_string._code_page;
					}
				}
			}
			if( _code_page != code_page || old_string._code_page != code_page || ( new_string.Length() > 0 && new_string._code_page != code_page ) )
			{
				::rux::XString converted_this;
				if( _code_page != code_page )
					converted_this.set_ByRef( ConvertToCodePage( code_page ) );
				else
					converted_this.set_ByRef( this );
				::rux::XString converted_old_string;
				if( old_string._code_page != code_page )
					converted_old_string.set_ByRef( old_string.ConvertToCodePage( code_page ) );
				else
					converted_old_string.set_ByRef( old_string );
				::rux::XString converted_new_string;
				if( new_string._code_page != code_page )
					converted_new_string.set_ByRef( new_string.ConvertToCodePage( code_page ) );
				else
					converted_new_string.set_ByRef( new_string );
				return converted_this.ReplaceLast( *converted_old_string() , *converted_new_string() , index );
			}
			else
			{
				::rux::int32 last_index_of = LastIndexOf( old_string , index );
				if( last_index_of != -1 )
				{
					::rux::XString string1( *this );
					string1.RemoveRange( last_index_of , old_string.Length() );
					if( new_string.Length() > 0 )
						string1.InsertRange( last_index_of , new_string );
					return string1++;
				}
			}
		}
		return XString( *this )++;
	};
	::rux::String& String::Substring( size_t start_index , size_t length ) const
	{
		if( Length() > 0 )
		{
			if( _code_page == XEnumCodePage_UTF16 )
			{
				start_index *= 2;
				if( length == SIZE_MAX )
					length = _count - 2 - start_index;
				else
					length *= 2;
				if( start_index < _count - 1 && length > 0 )
				{		
					if( start_index + length > _count - 2 )
						length = _count - 2 - start_index;
					::rux::XString string( (size_t)length + 2 , "string" , __FILE__ , __LINE__ );
					::rux::String* string_that = string();
					string_that->_count = length + 2;
					string_that->_code_page = XEnumCodePage_UTF16;
					::rux::engine::memcpy( string_that->str() , 0 , &_str[ start_index ] , string_that->_count - 2 );
					*(::rux::uint16*)&string_that->_str[ string_that->_count - 2 ] = 0;
					return string++;
				}
			}
			else if( _code_page == XEnumCodePage_CP1251 )
			{
				if( start_index < _count - 1 && length > 0 )
				{
					if( length == SIZE_MAX || start_index + length > _count - 1 )
						length = _count - 1 - start_index;
					::rux::XString string( (size_t)length + 1 , "string" , __FILE__ , __LINE__ );
					::rux::String* string_that = string();
					string_that->_count = length + 1;
					string_that->_code_page = XEnumCodePage_CP1251;
					::rux::engine::memcpy( string_that->str() , 0 , &_str[ start_index ] , string_that->_count - 1 );
					string_that->_str[ string_that->_count - 1 ] = 0;
					return string++;
				}
			}	
			else if( _code_page == XEnumCodePage_UTF8 )
			{
				length = XString::utf8_symbol_length_to_byte_length( _str , start_index , length );
				if( length != SIZE_MAX )
				{
					start_index = XString::utf8_symbol_index_to_byte_index( _str , start_index );
					if( start_index != SIZE_MAX && start_index < _count - 1 && length > 0 )
					{
						if( length == SIZE_MAX || start_index + length > _count - 1 )
							length = _count - 1 - start_index;
						::rux::XString string( (size_t)length + 1 , "string" , __FILE__ , __LINE__ );
						::rux::String* string_that = string();
						string_that->_count = length + 1;
						string_that->_code_page = XEnumCodePage_UTF8;
						::rux::engine::memcpy( string_that->str() , 0 , &str()[ start_index ] , string_that->_count - 1 );
						string_that->_str[ string_that->_count - 1 ] = 0;
						return string++;
					}
				}
			}
		}
		return ::rux::XString()++;
	};
	::rux::String& String::ToLower( size_t start_index , size_t length ) const
	{
		size_t thislen = Length();
		if( thislen > 0 )
		{
			if( start_index < thislen
				&& length > 0 )
			{
				if( length == SIZE_MAX || start_index + length > thislen )
					length = thislen - start_index;

				size_t allocated_size = _count;
				if( _code_page == XEnumCodePage_UTF8 )
				{
					length = XString::utf8_symbol_length_to_byte_length( _str , start_index , length );
					start_index = XString::utf8_symbol_index_to_byte_index( _str , start_index );
					allocated_size *= 4;
				}
				else if( _code_page == XEnumCodePage_UTF16 )
				{
					length *= 2;
					start_index *= 2;
				}
				else if( _code_page == XEnumCodePage_UTF32 )
				{
					length *= 4;
					start_index *= 4;
				}
				else if( _code_page != XEnumCodePage_CP1251 )
				{
					char* lower = (char*)alloc_array_macros( char , _count );
					::memcpy( lower , _str , _count );
					::rux::XString string;
					string()->attach( lower , _count - 1 , _count - 1 , _code_page );
					return string++;
				}
				char* lower = (char*)alloc_array_macros( char , allocated_size );
				size_t dst_index = 0;
				if( start_index > 0 )
				{
					::memcpy( lower , _str , start_index );
					dst_index = start_index;
				}
				if( length > 0 )
				{
					size_t bytes = 0;
					if( _code_page == XEnumCodePage_UTF8 )
						::rux::utils::string::utf8::to_lower( &_str[ start_index ] , length , &lower[ dst_index ] , allocated_size - start_index , bytes );
					else if( _code_page == XEnumCodePage_UTF16 )
						::rux::utils::string::utf16::to_lower( &_str[ start_index ] , length , &lower[ dst_index ] , allocated_size - start_index , bytes );
					else if( _code_page == XEnumCodePage_UTF32 )
						::rux::utils::string::utf32::to_lower( &_str[ start_index ] , length , &lower[ dst_index ] , allocated_size - start_index , bytes );
					else if( _code_page == XEnumCodePage_CP1251 )
						::rux::utils::string::cp1251::to_lower( &_str[ start_index ] , length , &lower[ dst_index ] , allocated_size - start_index , bytes );
					start_index += length;
					dst_index += bytes;					
				}
				if( start_index < thislen )
				{
					::memcpy( &lower[ dst_index ] , &_str[ start_index ] , thislen - start_index );
					dst_index += thislen - start_index;
				}
				lower[ dst_index ] = 0;
				::rux::XString string;
				string()->attach( lower , allocated_size , dst_index , _code_page );
				return string++;
			}
			::rux::XString return_string( *this );
			return return_string++;
		}
		return XString()++;
	};
	::rux::String& String::ToUpper( size_t start_index , size_t length ) const
	{
		size_t thislen = Length();
		if( thislen > 0 )
		{
			if( start_index < thislen
				&& length > 0 )
			{
				if( length == SIZE_MAX || start_index + length > thislen )
					length = thislen - start_index;

				size_t allocated_size = _count;
				if( _code_page == XEnumCodePage_UTF8 )
				{
					length = XString::utf8_symbol_length_to_byte_length( _str , start_index , length );
					start_index = XString::utf8_symbol_index_to_byte_index( _str , start_index );
					allocated_size *= 4;
				}
				else if( _code_page == XEnumCodePage_UTF16 )
				{
					length *= 2;
					start_index *= 2;
				}
				else if( _code_page == XEnumCodePage_UTF32 )
				{
					length *= 4;
					start_index *= 4;
				}
				else if( _code_page != XEnumCodePage_CP1251 )
				{
					char* lower = (char*)alloc_array_macros( char , _count );
					::memcpy( lower , _str , _count );
					::rux::XString string;
					string()->attach( lower , _count - 1 , _count - 1 , _code_page );
					return string++;
				}
				char* lower = (char*)alloc_array_macros( char , allocated_size );
				size_t dst_index = 0;
				if( start_index > 0 )
				{
					::memcpy( lower , _str , start_index );
					dst_index = start_index;
				}
				if( length > 0 )
				{
					size_t bytes = 0;
					if( _code_page == XEnumCodePage_UTF8 )
						::rux::utils::string::utf8::to_upper( &_str[ start_index ] , length , &lower[ dst_index ] , allocated_size - start_index , bytes );
					else if( _code_page == XEnumCodePage_UTF16 )
						::rux::utils::string::utf16::to_upper( &_str[ start_index ] , length , &lower[ dst_index ] , allocated_size - start_index , bytes );
					else if( _code_page == XEnumCodePage_UTF32 )
						::rux::utils::string::utf32::to_upper( &_str[ start_index ] , length , &lower[ dst_index ] , allocated_size - start_index , bytes );
					else if( _code_page == XEnumCodePage_CP1251 )
						::rux::utils::string::cp1251::to_upper( &_str[ start_index ] , length , &lower[ dst_index ] , allocated_size - start_index , bytes );
					start_index += length;
					dst_index += bytes;					
				}
				if( start_index < thislen )
				{
					::memcpy( &lower[ dst_index ] , &_str[ start_index ] , thislen - start_index );
					dst_index += thislen - start_index;
				}
				lower[ dst_index ] = 0;
				::rux::XString string;
				string()->attach( lower , allocated_size , dst_index , _code_page );
				return string++;
			}
			::rux::XString return_string( *this );
			return return_string++;
		}
		return XString()++;
	};
	Array<XString>& String::Split( const char* separator , EStringSplitOptions string_split_options ) const
	{
		::rux::XString separator_string;
		::rux::String* separator_string_ptr = separator_string();
		separator_string_ptr->attach( separator , SIZE_MAX , SIZE_MAX , XString::get_global_code_page() );
		::rux::XArray< ::rux::XString > separators;
		separators.AddByRef( separator_string );
		separators.set_ByRef( Split( separators , string_split_options ) );
		separator_string_ptr->detach();
		return separators++;
	};
	Array<XString>& String::Split( const ::rux::XArray<XString>& separators , EStringSplitOptions string_split_options ) const
	{
		::rux::XArray< XString > splits;	
		if( Length() > 0 )
		{
			if( separators.Count() == 0 )
			{
				::rux::XString copy_string( *this );
				splits.AddByRef( copy_string );
			}
			else
			{
				::rux::byte convert = 0;
				XEnumCodePage code_page = _code_page;
				if( code_page != XEnumCodePage_UTF8 )
				{
					for( size_t index0 = 0 ; index0 < separators.Count() ; index0++ )
					{
						if( code_page == XEnumCodePage_CP1251 || separators[ index0 ]._code_page != XEnumCodePage_CP1251 )
						{
							if( code_page != separators[ index0 ]._code_page )
								convert = 1;
							code_page = separators[ index0 ]._code_page;
						}
						if( code_page == XEnumCodePage_UTF8 )
							break;
					}
				}
				if( convert )
				{
					::rux::XString converted_this;
					if( _code_page != code_page )
						converted_this.set_ByRef( ConvertToCodePage( code_page ) );
					else
						converted_this.set_ByRef( this );
					::rux::XArray< ::rux::XString > converted_separators;
					for( size_t index0 = 0 ; index0 < separators.Count() ; index0++ )
					{
						::rux::XString converted_old_string;
						if( separators[ index0 ]._code_page != code_page )
							converted_old_string.set_ByRef( separators[ index0 ].ConvertToCodePage( code_page ) );
						else
							converted_old_string.set_ByRef( separators[ index0 ] );
						converted_separators.AddByRef( converted_old_string );
					}
					return converted_this.Split( converted_separators , string_split_options );
				}
				else
				{
					if( code_page == XEnumCodePage_CP1251 )
					{
						size_t start_index = 0;
						for( size_t index0 = 0 ; index0 < _count - 1 ; index0++ )
						{
							for( size_t index1 = 0 ; index1 < separators.Count() ; index1++ )
							{
								if( separators[ index1 ]._str )
								{
									if( index0 + separators[ index1 ]._count - 1 <= _count - 1 )
									{
										if( memcmp( &_str[ index0 ] , separators[ index1 ]._str , separators[ index1 ]._count - 1 ) == 0 )
										{
											if( index0 > start_index )					
												splits.AddByRef( XString( _str , start_index , index0 - start_index ) );			
											else if( string_split_options == EStringSplitOptions_None )
												splits.AddByRef( XString() );
											start_index = index0 + separators[ index1 ]._count - 1;
											index0 = start_index - 1;
											break;
										}
									}
								}
							}
						}
						if( _count - 1 > start_index )
							splits.AddByRef( XString( _str , start_index , _count - 1 - start_index ) );		
					}
					else if( code_page == XEnumCodePage_UTF16 )
					{
						char* utf16_ptr = _str;
						size_t utf16_ptr_length = _count - 2;
						size_t start_index = 0;
						for( size_t index0 = 0 ; index0 < utf16_ptr_length ; index0 += 2 )
						{
							for( size_t index1 = 0 ; index1 < separators.Count() ; index1++ )
							{
								if( separators[ index1 ]._str )
								{
									if( index0 + separators[ index1 ]._count - 2 <= utf16_ptr_length )
									{
										if( memcmp( &utf16_ptr[ index0 ] , separators[ index1 ]._str , separators[ index1 ]._count - 2 ) == 0 )
										{
											if( index0 > start_index )					
												splits.AddByRef( XString( utf16_ptr , start_index / 2 , ( index0 - start_index ) / 2 , XEnumCodePage_UTF16 ) );					
											else if( string_split_options == EStringSplitOptions_None )
												splits.AddByRef( XString() );
											start_index = index0 + separators[ index1 ]._count - 2;
											index0 = start_index - 2;
											break;
										}
									}
								}
							}
						}
						if( utf16_ptr_length > start_index )
							splits.AddByRef( XString( utf16_ptr , start_index / 2 , ( utf16_ptr_length - start_index ) / 2 , XEnumCodePage_UTF16 ) );
					}
				else if( code_page == XEnumCodePage_UTF8 )
				{
						char* utf8_ptr = _str;
						size_t utf8_ptr_length = _count - 1;
						size_t start_index = 0 , symbol_length = 0;
						for( size_t index0 = 0 ; index0 < utf8_ptr_length ; index0 += symbol_length )
						{
							symbol_length = XString::utf8_symbol_length( utf8_ptr , index0 );
							for( size_t index1 = 0 ; index1 < separators.Count() ; index1++ )
							{
								if( separators[ index1 ]._str )
								{
									if( index0 + separators[ index1 ]._count - 1 <= utf8_ptr_length )
									{				
										if( memcmp( &utf8_ptr[ index0 ] , separators[ index1 ]._str , separators[ index1 ]._count - 1 ) == 0 )
										{
											if( index0 > start_index )					
											{
												::rux::XString string;
												::rux::String* string_that = string();
												string_that->_count = index0 - start_index + 1;
												string_that->_size = string_that->_count;
												string_that->_code_page = XEnumCodePage_UTF8;
												string_that->_str = alloc_array_macros( char , string_that->_size );
												::rux::engine::memcpy( string_that->_str , 0 , &utf8_ptr[ start_index ] , index0 - start_index );
												string_that->_str[ string_that->_count - 1 ] = 0;
												splits.AddByRef( string );						
											}
											else if( string_split_options == EStringSplitOptions_None )
												splits.AddByRef( XString() );
											start_index = index0 + separators[ index1 ]._count - 1;
											index0 = start_index - symbol_length;
										}				
									}
								}
							}
						}
						if( utf8_ptr_length > start_index )
						{
							::rux::XString string;
							::rux::String* string_that = string();
							string_that->_count = utf8_ptr_length + 1 - start_index;
							string_that->_size = string_that->_count;
							string_that->_code_page = XEnumCodePage_UTF8;
							string_that->_str = alloc_array_macros( char , string_that->_size );
							::rux::engine::memcpy( string_that->_str , 0 , &utf8_ptr[ start_index ] , utf8_ptr_length - start_index );
							string_that->_str[ string_that->_count - 1 ] = 0;
							splits.AddByRef( string );						
						}
					}
				}
			}
		}
		return splits++;
	};
	Array<XString>& String::Split( char separator ) const
	{
		::rux::XString separator_string;
		::rux::String* separator_string_ptr = separator_string();
		separator_string_ptr->attach( &separator , 1 , 1 , XString::get_global_code_page() );
		::rux::XArray< ::rux::XString > separators;
		separators.AddByRef( separator_string );
		separators.set_ByRef( Split( separators ) );
		separator_string_ptr->detach();
		return separators++;
	};
	::rux::uint8 XString::IsInt32( ::rux::uint32 letter )
	{
		if( letter == '0'	|| letter == '1' || letter == '2'
			|| letter == '3' || letter == '4' || letter == '5'
			|| letter == '6' || letter == '7' || letter == '8'
			|| letter == '9' )
			return 1;
		else
			return 0;
	};
	::rux::uint8 XString::IsInt32( const char* word )
	{
		::rux::uint8 is_int32 = 0;
		if( word )
		{
			size_t length = strlen( word );
			for( size_t index = 0 ; index < length ; index++ )
			{
				if( word[ index ] == '0'	|| word[ index ] == '1' || word[ index ] == '2'
					|| word[ index ] == '3' || word[ index ] == '4' || word[ index ] == '5'
					|| word[ index ] == '6' || word[ index ] == '7' || word[ index ] == '8'
					|| word[ index ] == '9' )
				{
					is_int32 = 1;
				}
				else
				{
					is_int32 = 0;
					break;
				}
			}
		}
		return is_int32;
	};
	bool String::IsInt32( void )
	{
		::rux::uint8 is_int32 = 0;
		if( _code_page == XEnumCodePage_UTF16 )
		{
			for( size_t index = 0 ; index < _count - 2 ; index+=2 )
			{
				if( *(::rux::uint16*)&_str[ index ] == '0'	|| *(::rux::uint16*)&_str[ index ] == '1' || *(::rux::uint16*)&_str[ index ] == '2'
					|| *(::rux::uint16*)&_str[ index ] == '3' || *(::rux::uint16*)&_str[ index ] == '4' || *(::rux::uint16*)&_str[ index ] == '5'
					|| *(::rux::uint16*)&_str[ index ] == '6' || *(::rux::uint16*)&_str[ index ] == '7' || *(::rux::uint16*)&_str[ index ] == '8'
					|| *(::rux::uint16*)&_str[ index ] == '9' )
				{
					is_int32 = 1;
				}
				else
				{
					is_int32 = 0;
					break;
				}
			}
		}
		else
		{
			for( size_t index = 0 ; index < _count - 1 ; index++ )
			{
				if( _str[ index ] == '0'	|| _str[ index ] == '1' || _str[ index ] == '2'
					|| _str[ index ] == '3' || _str[ index ] == '4' || _str[ index ] == '5'
					|| _str[ index ] == '6' || _str[ index ] == '7' || _str[ index ] == '8'
					|| _str[ index ] == '9' )
				{
					is_int32 = 1;
				}
				else
				{
					is_int32 = 0;
					break;
				}
			}
		}
		return is_int32 == 1;
	};
	bool String::IsDouble( void )
	{
		::rux::uint8 is_double = 0;
		::rux::uint8 has_point = 0; 
		if( _code_page == XEnumCodePage_UTF16 )
		{
			for( size_t index = 0 ; index < _count - 2 ; index+=2 )
			{
				if( *(::rux::uint16*)&_str[ index ] == '0'	|| *(::rux::uint16*)&_str[ index ] == '1' || *(::rux::uint16*)&_str[ index ] == '2'
					|| *(::rux::uint16*)&_str[ index ] == '3' || *(::rux::uint16*)&_str[ index ] == '4' 	|| *(::rux::uint16*)&_str[ index ] == '5'
					|| *(::rux::uint16*)&_str[ index ] == '6' || *(::rux::uint16*)&_str[ index ] == '7' 	|| *(::rux::uint16*)&_str[ index ] == '8'
					|| *(::rux::uint16*)&_str[ index ] == '9' )
				{
					is_double = 1;
				}
				else if( *(::rux::uint16*)&_str[ index ] == '.' )
				{
					if( index == 0 
						|| has_point == 1 )
					{
						is_double = 0;
						break;
					}
					else
					{
						has_point = 1;
					}
				}
				else
				{
					is_double = 0;
					break;
				}
			}
		}
		else
		{
			for( size_t index = 0 ; index < _count - 1 ; index++ )
			{
				if( _str[ index ] == '0'	|| _str[ index ] == '1' || _str[ index ] == '2'
					|| _str[ index ] == '3' || _str[ index ] == '4' 	|| _str[ index ] == '5'
					|| _str[ index ] == '6' || _str[ index ] == '7' 	|| _str[ index ] == '8'
					|| _str[ index ] == '9' )
				{
					is_double = 1;
				}
				else if( _str[ index ] == '.' )
				{
					if( index == 0 
						|| has_point == 1 )
					{
						is_double = 0;
						break;
					}
					else
					{
						has_point = 1;
					}
				}
				else
				{
					is_double = 0;
					break;
				}
			}
		}
		return is_double == 1;
	};
	::rux::String& operator +(const ::rux::XString& string1, const ::rux::XString& string2)
	{	
		return ( *string1() + *string2() );
	};
	::rux::String& operator +(const ::rux::XString& string1, const wchar_t* text_ptr)
	{
		return *string1() + text_ptr;
	};
	::rux::String& operator +(const ::rux::XString& string1, const char* text_ptr)
	{
		return operator +( *string1() , text_ptr );
	};
	::rux::String& operator +(const ::rux::XString& string1, const String& string2)
	{	
		return operator +( *string1() , string2 );
	};	
	::rux::String& operator +(const char* text_ptr , const ::rux::XString& string2)
	{	
		return operator +( text_ptr , *string2() );
	};
	::rux::String& operator +(const wchar_t* text_ptr , const ::rux::XString& string2)
	{	
		return operator +( text_ptr , *string2() );
	};
	::rux::String& operator +(const wchar_t* text_ptr , const String& string2)
	{
		if( text_ptr && text_ptr[ 0 ] != 0 )
		{
			::rux::XString result_string( text_ptr , 0 , SIZE_MAX , string2._count + 32 );
			result_string.InsertRange( SIZE_MAX , string2 );
			return result_string++;
		}
		else
		{
			::rux::XString result_string( string2 );
			return result_string++;
		}
	};
	::rux::String& operator +(const char* text_ptr , const String& string2)
	{
		if( text_ptr && text_ptr[ 0 ] != 0 )
		{
			::rux::XString result_string( text_ptr , 0 , SIZE_MAX , string2._count + 32 );
			result_string.InsertRange( SIZE_MAX , string2 );
			return result_string++;
		}
		else
		{
			::rux::XString result_string( string2 );
			return result_string++;
		}
	};
	::rux::String& operator +(const String& string1,const wchar_t* text_ptr)
	{	
		::rux::XString result_string;
		result_string()->ctor_set( string1 , text_ptr );
		return result_string++;
	};
	::rux::String& operator +(const String& string1,const char* text_ptr)
	{	
		::rux::XString result_string;
		result_string()->ctor_set( string1 , text_ptr );
		return result_string++;
	};
	::rux::String& operator +(const String& string1,const ::rux::XString& string2)
	{
		return operator +( string1 , *string2() );
	};
	::rux::String& operator +(const String& string1,const String& string2)
	{
		if( string1.Length() > 0 )
		{
			if( string2.Length() > 0 )
			{
				::rux::XString result_string;
				XEnumCodePage code_page = string1._code_page;
				if( code_page != XEnumCodePage_UTF8 )
				{
					if( code_page == XEnumCodePage_CP1251 || string2._code_page != XEnumCodePage_CP1251 )
						code_page = string2._code_page;
				}
				::rux::String* string_that = result_string();
				string_that->_code_page = code_page;
				if( string_that->_code_page == XEnumCodePage_UTF16 )
					string_that->_count = 2;
				else
					string_that->_count = 1;
				string_that->IncreaseMemory( string1.Size() + string2.Size() + 32 );
				string_that->InsertRange( 0 , string1 );
				string_that->InsertRange( SIZE_MAX , string2 );
				return result_string++;
			}
			else
				return XString( string1 )++;
		}
		else if( string2.Length() > 0 )
			return XString( string2 )++;
		return XString()++;
	};
	::rux::String& String::operator =(const wchar_t* string_ptr)
	{
		Clear();
		InsertRange( 0 , string_ptr , SIZE_MAX );
		return *this;
	};	
	::rux::XString& XString::operator =(const wchar_t* string_ptr)
	{
		operator()()->operator=( string_ptr );
		return *this;
	};	
	::rux::String& String::operator =(const char* string_ptr)
	{
		Clear();
		InsertRange( 0 , string_ptr , SIZE_MAX );
		return *this;
	};
	::rux::XString& XString::operator =(const char* string_ptr)
	{
		operator()()->operator=( string_ptr );
		return *this;
	};
	::rux::XString& XString::operator =(const String& string)
	{
		operator()()->operator=( string );
		return *this;
	};
	::rux::String& String::operator =(const String& string)
	{		
		Clear();
		InsertRange( 0 , string );
		return *this;
	}
	::rux::XString& XString::operator =(const ::rux::XString& string)
	{	
		operator()()->operator=( *string() );
		return *this;
	};
	bool XString::operator ==(const ::rux::XString& string) const
	{
		return operator()()->operator==( *string() );
	};
	bool String::operator ==(const ::rux::XString& string) const
	{
		return operator==( *string() );
	};
	bool XString::operator ==( const char* text_ptr ) const
	{
		return operator()()->operator==( text_ptr );
	};
	bool String::operator ==( const char* text_ptr ) const
	{
		_declare_stackvar( ::rux::String , attached_string , 0 );
		attached_string.attach( text_ptr, SIZE_MAX , SIZE_MAX , XString::get_global_code_page() );
		bool res = operator==( attached_string );
		attached_string.detach();
		return res;
	}	
	bool XString::operator ==( const XString* string_ptr ) const
	{
		return operator()()->operator==( *string_ptr->operator()() );
	};
	bool String::operator ==( const XString* string_ptr ) const
	{
		return operator==( *string_ptr->operator()() );
	}
	bool XString::operator ==( const String& string ) const
	{
		return operator()()->operator==( string );
	};
	bool String::operator ==(const String& string) const
	{
		size_t len0 = Length(), len1 = string.Length();
		if(len0 == len1)
		{
			if(len0 == 0)
				return true;
			return IndexOf( string ) == 0;
		}
		return false;
	};
	bool operator ==(const char* text_ptr,const String& string2)
	{
		return string2.operator==( text_ptr );
	};
	bool operator ==(const char* text_ptr,const ::rux::XString& string2)
	{	
		return operator ==( text_ptr , *string2() );
	};
	bool operator !=(const char* text_ptr,const String& string2)
	{
		return string2.operator!=( text_ptr );
	};
	bool operator !=(const char* text_ptr,const ::rux::XString& string2)
	{
		return operator !=( text_ptr , *string2() );
	};
	bool XString::operator >( const ::rux::XString& string ) const
	{
		return operator()()->operator >( *string() );
	};
	bool String::operator >( const ::rux::String& string ) const
	{
		bool result = false;
		if( Length() > 0 )
		{
			result = true;
			if( string.Length() > 0 )
			{
				if( _code_page == XEnumCodePage_UTF16 )
				{
					wchar_t* source_array_ptr = NULL;
					if( string._code_page == XEnumCodePage_UTF16 )
						source_array_ptr = (wchar_t*)string._str;
					else if( string._code_page == XEnumCodePage_CP1251 )
					{
						size_t ansi_to_utf16_bytes = 0;
						source_array_ptr = (wchar_t*)XString::ansi_to_utf16( string._str , string._count - 1 , ansi_to_utf16_bytes );
					}
					else if( string._code_page == XEnumCodePage_UTF8 )
					{
						size_t utf8_to_utf16_bytes = 0;
						source_array_ptr = (wchar_t*)XString::utf8_to_utf16( string._str , string._count - 1 , utf8_to_utf16_bytes );
					}
					result = wcscmp( (wchar_t*)_str , source_array_ptr ) > 0;
					if( string._code_page != XEnumCodePage_UTF16 )
						::rux::engine::free_mem( source_array_ptr );
				}
				else if( _code_page == XEnumCodePage_CP1251 )
				{
					if( string._code_page == XEnumCodePage_UTF16 )
					{
						size_t ansi_to_utf16_bytes = 0;
						wchar_t* source_array_ptr = (wchar_t*)XString::ansi_to_utf16( _str , _count - 1 , ansi_to_utf16_bytes );
						result = wcscmp( (wchar_t*)string._str , source_array_ptr ) > 0;
						::rux::engine::free_mem( source_array_ptr );
					}
					else if( string._code_page == XEnumCodePage_CP1251 )
						result = strcmp( _str , string._str ) > 0;
					else if( string._code_page == XEnumCodePage_UTF8 )
					{
						size_t ansi_to_utf8_bytes = 0;
						char* utf8_ptr = XString::ansi_to_utf8( _str , _count - 1 , ansi_to_utf8_bytes );
						result = strcmp( utf8_ptr , string._str ) > 0;
						::rux::engine::free_mem( utf8_ptr );
					}
				}			
				else if( _code_page == XEnumCodePage_UTF8 )
				{
					char* source_array_ptr = NULL;
					if( string._code_page == XEnumCodePage_UTF16 )
					{
						size_t utf16_to_utf8_bytes = 0;
						source_array_ptr = XString::utf16_to_utf8( (::rux::uint16*)string._str , string.Length() , utf16_to_utf8_bytes );
					}
					else if( string._code_page == XEnumCodePage_CP1251 )
					{
						size_t ansi_to_utf8_bytes = 0;
						source_array_ptr = XString::ansi_to_utf8( string._str , string._count - 1 , ansi_to_utf8_bytes );
					}
					else if( string._code_page == XEnumCodePage_UTF8 )
						source_array_ptr = string._str;
					result = strcmp( _str , source_array_ptr ) > 0;
					if( string._code_page != XEnumCodePage_UTF8 )
						::rux::engine::free_mem( source_array_ptr );
				}
			}
		}
		return result;
	};
	bool XString::operator !=(const ::rux::XString& string) const
	{
		return operator()()->operator!=( *string() );
	};
	bool String::operator !=(const ::rux::XString& string) const
	{
		return operator!=( *string() );
	};
	bool String::operator !=(const ::rux::String& string) const
	{
		return !operator==(string);
	};
	bool XString::operator !=(const char* text_ptr) const
	{
		return operator()()->operator!=( text_ptr );
	};
	bool String::operator !=(const char* text_ptr) const
	{
		_declare_stackvar( ::rux::String , attached_string , 0 );
		attached_string.attach( text_ptr, SIZE_MAX , SIZE_MAX , XString::get_global_code_page() );
		bool res = operator!=( attached_string );
		attached_string.detach();
		return res;
	}	
	bool XString::operator !=( const XString* string_ptr ) const
	{
		return operator()()->operator!=( *string_ptr->operator()() );
	};
	bool String::operator !=( const XString* string_ptr ) const
	{
		return operator!=( *string_ptr->operator()() );
	};
	bool XString::operator !=( const String& string ) const
	{
		return operator()()->operator!=( string );
	};
	::rux::String& String::operator +=(const ::rux::XString& string2)
	{
		return operator +=( *string2() );
	};
	::rux::String& String::operator +=(const wchar_t* string_ptr)
	{
		InsertRange( SIZE_MAX , string_ptr , SIZE_MAX );
		return *this;
	};
	::rux::String& String::operator +=(const char* string_ptr)
	{
		InsertRange( SIZE_MAX , string_ptr , SIZE_MAX );
		return *this;
	};
	::rux::XString& XString::operator +=( const ::rux::XString& string)
	{
		operator()()->operator+=( string );
		return *this;
	}
	::rux::XString& XString::operator +=(const char* text)
	{
		operator()()->operator+=( text );
		return *this;
	};
	::rux::XString& XString::operator +=(const wchar_t* string_ptr)
	{
		operator()()->operator+=( string_ptr );
		return *this;
	};
	::rux::XString& XString::operator +=( const String& string )
	{
		operator()()->operator+=( string );
		return *this;
	};
	::rux::String& String::operator +=( const String& string2 )
	{
		InsertRange( SIZE_MAX , string2 );
		return *this;
	};
	::rux::String& XString::Format( const char* format, ... )
	{
		::rux::XString x_string;
		va_list ap;
		va_start( ap , format );
		x_string.set_ByRef( FormatVAList( format , ap ) );
		va_end( ap );
		return x_string++;
	};
	::rux::String& XString::FormatVAList( const char* format, va_list ap )
	{
		int size = ::rux::_vscprintf( format , ap ) + 1;
		char* temp_ptr = alloc_array_macros( char , size );
		vsnprintf( temp_ptr , size , format , ap );
		::rux::XString x_string( temp_ptr , ::rux::XString::get_global_code_page() , 0 , "x_string" , __FILE__ , __LINE__ );
		return x_string++;
	};	
	implement_duplicate_internal_result_function_with_const( ::rux::int32 , String , ToInt32 );
	void String::CopyToArrayAsCurrentCodePage( char destination[] , size_t destination_size ) const
	{
		destination[ 0 ] = 0;
		if( Length() > 0 )
		{
			if( _code_page == XEnumCodePage_UTF16 )
			{
				if( XString::get_global_code_page() == XEnumCodePage_CP1251 )
				{
					size_t utf16_to_ansi_bytes = 0;
					XString::utf16_to_ansi( (::rux::uint16*)_str , ( _count - 2 ) / 2 , destination , destination_size , utf16_to_ansi_bytes );
				}
				else if( XString::get_global_code_page() == XEnumCodePage_UTF8 )
				{
					size_t utf16_to_utf8_bytes = 0;
					XString::utf16_to_utf8( (::rux::uint16*)_str , Length() , destination , destination_size , utf16_to_utf8_bytes );
				}
			}
			else if( _code_page == XEnumCodePage_CP1251 )
			{
				if( XString::get_global_code_page() == XEnumCodePage_CP1251 )
					::rux::engine::memcpy( destination , 0 , _str , _count );
				else if( XString::get_global_code_page() == XEnumCodePage_UTF8 )
				{
					if( _code_page == XEnumCodePage_CP1251 )
					{
						size_t ansi_to_utf8_length = 0;
						XString::ansi_to_utf8( _str , ( _count - 2 ) / 2 , destination , destination_size , ansi_to_utf8_length );
					}
				}
			}	
			else if( _code_page == XEnumCodePage_UTF8 )
			{
				if( XString::get_global_code_page() == XEnumCodePage_CP1251 )
				{
					size_t utf8_to_ansi_byte = 0;
					XString::utf8_to_ansi( _str , _count - 1 , destination , destination_size , utf8_to_ansi_byte );
				}
				else if( XString::get_global_code_page() == XEnumCodePage_UTF8 )
					::rux::engine::memcpy( destination , 0 , _str , _count );
			}
		}
	};
	::rux::String* String::ObjectToString( void )
	{
		set_Auto();
		return this;
	};
	implement_duplicate_internal_result_function_with_const( ::rux::String& , String , ConvertToCurrentAnsiPage );
	::rux::String& String::ConvertToCurrentAnsiPage( void ) const
	{
		if( Length() > 0 )
		{
			if( _code_page == XEnumCodePage_UTF16 )
			{
				::rux::XString string1( _count / 2 );
				::rux::String* string1_that = string1();
				size_t utf16_to_ansi_bytes = 0;
				XString::utf16_to_ansi( (::rux::uint16*)_str , Length() , string1_that->str() , _count / 2 , utf16_to_ansi_bytes );
				string1_that->_code_page = XEnumCodePage_CP1251;
				string1_that->_count = utf16_to_ansi_bytes + 1;
				return string1++;
			}
			else if( _code_page == XEnumCodePage_CP1251 )
				return ::rux::XString( *this )++;
			else if( _code_page == XEnumCodePage_UTF8 )
			{
				::rux::XString string1( _count );
				::rux::String* string1_that = string1();
				size_t utf8_to_ansi_byte = 0;
				XString::utf8_to_ansi( _str , _count - 1 , string1_that->str() , _count , utf8_to_ansi_byte );
				string1_that->_code_page = XEnumCodePage_CP1251;
				string1_that->_count = utf8_to_ansi_byte;
				string1_that->_count++;
				return string1++;
			}
		}
		::rux::XString string1;
		::rux::String* string1_that = string1();
		string1_that->_code_page = XEnumCodePage_CP1251;
		return string1++;
	};
	::rux::String& String::ConvertToAnsi( void ) const
	{
		if( Length() > 0 )
		{
			if( _code_page == XEnumCodePage_UTF16 )
			{
				if( XString::get_global_code_page() == XEnumCodePage_CP1251 )
				{
					::rux::XString string1( _count / 2 );
					::rux::String* string1_that = string1();
					size_t utf16_to_ansi_bytes = 0;
					XString::utf16_to_ansi( (::rux::uint16*)_str , Length() , string1_that->str() , _count / 2 , utf16_to_ansi_bytes );
					string1_that->_code_page = XEnumCodePage_CP1251;
					string1_that->_count = utf16_to_ansi_bytes + 1;
					return string1++;
				}
				else if( XString::get_global_code_page() == XEnumCodePage_UTF8 )
				{
					::rux::XString string1( _count * 2 );
					::rux::String* string1_that = string1();
					size_t utf16_to_utf8_bytes = 0;
					XString::utf16_to_utf8( (::rux::uint16*)_str , Length() , string1_that->str() , _count * 2 , utf16_to_utf8_bytes );
					string1_that->_code_page = XEnumCodePage_UTF8;
					string1_that->_count = utf16_to_utf8_bytes;
					string1_that->_count++;		
					return string1++;
				}
			}
			else if( _code_page == XEnumCodePage_CP1251 )
			{
				if( XString::get_global_code_page() == XEnumCodePage_CP1251 )
					return ::rux::XString( *this )++;
				else if( XString::get_global_code_page() == XEnumCodePage_UTF8 )
				{
					::rux::XString string1( _count * 4 );
					::rux::String* string1_that = string1();
					size_t ansi_to_utf8_length = 0;
					XString::ansi_to_utf8( _str , Length() , string1_that->str() , _count * 4 , ansi_to_utf8_length );
					string1_that->_code_page = XEnumCodePage_UTF8;
					string1_that->_count = ansi_to_utf8_length;
					string1_that->_count++;		
					return string1++;
				}	
			}	
			else if( _code_page == XEnumCodePage_UTF8 )
			{
				if( XString::get_global_code_page() == XEnumCodePage_CP1251 )
				{
					::rux::XString string1( _count + 32 );
					::rux::String* string1_that = string1();
					size_t utf8_to_ansi_byte = 0;
					XString::utf8_to_ansi( _str , _count - 1 , string1_that->str() , _count , utf8_to_ansi_byte );
					string1_that->_code_page = XEnumCodePage_CP1251;
					string1_that->_count = utf8_to_ansi_byte;
					string1_that->_count++;
					return string1++;
				}			
				else if( XString::get_global_code_page() == XEnumCodePage_UTF8 )
					return ::rux::XString( *this )++;
			}
		}
		::rux::XString string1;
		::rux::String* string1_that = string1();
		string1_that->_code_page = XString::get_global_code_page();
		return string1++;
	};
	::rux::String& String::ConvertToUTF16( void ) const
	{
		if( Length() > 0 )
		{
			if( _code_page == XEnumCodePage_UTF16 )
				return ::rux::XString( *this )++;
			else 
			{
				::rux::XString string1( _count * 2 );
				::rux::String* string1_that = string1();
				size_t utf16_bytes = 0;
				if( _code_page == XEnumCodePage_CP1251 )
					XString::ansi_to_utf16( _str , _count - 1 , (::rux::uint16*)string1_that->str() , _count , utf16_bytes );
				else if(  _code_page == XEnumCodePage_UTF8 )
					XString::utf8_to_utf16( _str , _count - 1 , (::rux::uint16*)string1_that->str() , _count , utf16_bytes );
				string1_that->_code_page = XEnumCodePage_UTF16;
				string1_that->_count = utf16_bytes + 2;
				return string1++;
			}
		}
		::rux::XString string1;
		::rux::String* string1_that = string1();
		string1_that->_code_page = XEnumCodePage_UTF16;
		string1_that->_count = 2;
		return string1++;
	};
	::rux::String& String::ConvertToCodePage( XEnumCodePage code_page ) const
	{
		if( code_page == XEnumCodePage_UTF8 )
			return ConvertToUTF8();
		else if( code_page == XEnumCodePage_UTF16 )
			return ConvertToUTF16();
		else if( code_page == XEnumCodePage_CP1251 )
			return ConvertToCurrentAnsiPage();
		else
			return XString()++;
	};
	::rux::String& String::ConvertToUTF8( void ) const
	{	
		if( Length() )
		{
			if( _code_page == XEnumCodePage_UTF8 )	
				return ::rux::XString( *this )++;
			else
			{
				size_t size = 0;
				if( _code_page == XEnumCodePage_UTF16 )
					size = _count * 2;
				else if( _code_page == XEnumCodePage_CP1251 )
					size = _count * 4;

				::rux::XString string1( size );
				::rux::String* string1_that = string1();
				size_t utf8_bytes = 0;
				if( _code_page == XEnumCodePage_UTF16 )
					XString::utf16_to_utf8( (::rux::uint16*)_str , Length() , string1_that->str() , size , utf8_bytes );
				else if( _code_page == XEnumCodePage_CP1251 )
					XString::ansi_to_utf8( _str , Length() , string1_that->str() , size , utf8_bytes );
				string1_that->_code_page = XEnumCodePage_UTF8;
				string1_that->_count = utf8_bytes + 1;
				return string1++;
			}
		}
		::rux::XString string1;
		::rux::String* string1_that = string1();
		string1_that->_code_page = XEnumCodePage_UTF8;
		return string1++;
	};
	void String::SwapHere( size_t index1 , size_t index2 )
	{
		if( Length() > 0 )
		{
			if( index1 != index2 )
			{
				if( _code_page == XEnumCodePage_UTF16 )
				{
					index1 *= 2;
					index2 *= 2;
					if( _count - 2 > index1
						&& _count - 2 > index2 )
					{
						::rux::uint16 symbol = *(::rux::uint16*)&_str[ index1 ];
						*(::rux::uint16*)&_str[ index1 ] = *(::rux::uint16*)&_str[ index2 ];
						*(::rux::uint16*)&_str[ index2 ] = symbol;
					}
				}
				else if( _code_page == XEnumCodePage_CP1251 )
				{
					if( _count - 1 > index1
						&& _count - 1 > index2 )
					{
						char symbol = _str[ index1 ];
						_str[ index1 ] = _str[ index2 ];
						_str[ index2 ] = symbol;
					}
				}
				else if( _code_page == XEnumCodePage_UTF8 )
				{
					index1 = XString::utf8_symbol_index_to_byte_index( _str , index1 );
					index2 = XString::utf8_symbol_index_to_byte_index( _str , index2 );
					if( index1 != SIZE_MAX && _count - 1 > index1
						&& index2 != SIZE_MAX && _count - 1 > index2 )
					{
						size_t symbol_length1 = XString::utf8_symbol_length( _str , index1 );				
						size_t symbol_length2 = XString::utf8_symbol_length( _str , index2 );
						declare_stack_variable( char , utf8_symbol , 4 );
						if( symbol_length1 > symbol_length2 )
						{
							::rux::engine::memcpy( utf8_symbol , 0 , &_str[ index1 ] , symbol_length1 );
							::rux::engine::memcpy( _str , index1 , &_str[ index2 ] , symbol_length2 );
							XString::array_left_shift( _str , index1 + symbol_length1 - symbol_length2 , symbol_length1 - symbol_length2 , _count );
							if( index1 < index2 )					
								index2 -= symbol_length1 - symbol_length2;
							XString::array_right_shift( _str , index2 + symbol_length2 , symbol_length1 - symbol_length2 , _count );
							::rux::engine::memcpy( _str , index2 , utf8_symbol , symbol_length1 );
						}
						else if( symbol_length2 > symbol_length1 )
						{
							::rux::engine::memcpy( utf8_symbol , 0 , &_str[ index2 ] , symbol_length2 );
							::rux::engine::memcpy( _str , index2 , &_str[ index1 ] , symbol_length1 );
							XString::array_left_shift( _str , index2 + symbol_length2 - symbol_length1 , symbol_length2 - symbol_length1 , _count );
							if( index2 < index1 )					
								index1 -= symbol_length2 - symbol_length1;
							XString::array_right_shift( _str , index1 + symbol_length1 , symbol_length2 - symbol_length1 , _count );
							::rux::engine::memcpy( _str , index1 , utf8_symbol , symbol_length1 );
						}
						else
						{
							::rux::engine::memcpy( utf8_symbol , 0 , &_str[ index2 ] , symbol_length2 );
							::rux::engine::memcpy( _str , index2 , &_str[ index1 ] , symbol_length1 );					
							::rux::engine::memcpy( _str , index1 , utf8_symbol , symbol_length1 );
						}
					}
				}
			}
		}
	};
	::rux::int32 XString::ToInt32( const char* text_ptr )
	{
		if( text_ptr && text_ptr[ 0 ] != 0 )
			return atoi( text_ptr );
		return 0;
	};
	::rux::uint32 XString::ToUInt32( const char* text_ptr )
	{
		if( text_ptr && text_ptr[ 0 ] != 0 )
			return strtoul( text_ptr , NULL , 10 );
		return 0;
	};
	::rux::int64 XString::ToInt64( const char* text_ptr )
	{
		if( text_ptr && text_ptr[ 0 ] != 0 )
			return atoll( text_ptr );
		return 0;
	};
	::rux::uint64 XString::ToUInt64( const char* text_ptr )
	{
		if( text_ptr && text_ptr[ 0 ] != 0 )
	#ifdef __WINDOWS__
			return _strtoui64( text_ptr , NULL , 10 );
	#elif defined( __UNIX__ )
			return strtoull( text_ptr , NULL , 10 );
	#endif
		return 0;
	};
	implement_duplicate_internal_result_function_with_const( size_t , String , Size );
	::rux::int32 String::ToInt32( void ) const
	{
		if( _code_page == XEnumCodePage_UTF16 )
	#ifdef __WINDOWS__		
			return _wtoi( (wchar_t*)_str );
	#endif
	#ifdef __UNIX__
			return wcstol( (wchar_t*)_str , NULL , 10 );
	#endif		
		else
			return atoi( _str );
	};
	::rux::uint32 String::ToUInt32( void )
	{
		if( _code_page == XEnumCodePage_UTF16 )
	#ifdef __WINDOWS__		
			return wcstoul( (wchar_t*)_str , NULL , 10 );
	#endif
	#ifdef __UNIX__
			return wcstoul( (wchar_t*)_str , NULL , 10 );
	#endif		
		else
			return strtoul( _str , NULL , 10 );
	};
	::rux::uint64 String::ToUInt64( void )
	{
		if( _code_page == XEnumCodePage_UTF16 )
			return ::rux::string::to_uint64( (wchar_t*)_str );
		else
	#ifdef __WINDOWS__
			return _strtoui64( _str , NULL , 10 );
	#elif defined( __UNIX__ )
			return strtoull( _str , NULL , 10 );
	#endif
	};
	::rux::int64 String::ToInt64( void )
	{
		if( _code_page == XEnumCodePage_UTF16 )
			return ::rux::string::to_int64( (wchar_t*)_str );
		else
			return atoll( _str );
	};
	double XString::ToDouble( const char* text_ptr )
	{
		return atof( text_ptr );
	};
	double String::ToDouble( void )
	{
		if( _code_page == XEnumCodePage_UTF16 )
	#ifdef __WINDOWS__
			return _wtof( (wchar_t*)_str );
	#endif
	#ifdef __UNIX__
			return wcstod( (wchar_t*)_str , NULL );
	#endif
		else
			return atof( _str );
	};
	::rux::uint8 XString::IsEqualUTF16( const ::rux::uint16* string_ptr1 , const ::rux::uint16* string_ptr2 )
	{	
		::rux::uint8 is_equal = string_ptr1 == NULL && string_ptr2 == NULL;
		if( string_ptr1
			&& string_ptr2 )
		{
			is_equal = static_cast< ::rux::uint8 >( 1 );
			size_t index = 0;	
			::rux::uint16 letter1 = 0xffff;
			::rux::uint16 letter2 = 0xffff;
			for( ; ; )
			{
				if( letter1 != 0 )
					letter1 = string_ptr1[ index ];
				if( letter2 != 0 ) 
					letter2 = string_ptr2[ index ];			
				if( letter1 != letter2 ) 
				{				
					is_equal = 0;
					break;
				}			
				if( letter1 == 0
					&& letter2 == 0 ) break;
				index++;
			}
		}
		return is_equal;
	};
	::rux::uint8 XString::IsEqualA( const char* string1 , const char* string2 )
	{	
		::rux::uint8 isequal = string1 == NULL && string2 == NULL ? 1 : 0;
		if( string1
			&& string2 )
		{
			isequal = 1;
			size_t index = 0;	
			char letter1 = ' ';
			char letter2 = ' ';
			for( ; ; )
			{
				if( letter1 != '\0' )	letter1 = string1[ index ];
				if( letter2 != '\0' ) letter2 = string2[ index ];
			
				if( letter1 != letter2 ) 
				{				
					isequal = 0;
					break;
				}			
			
				if( letter1 == '\0'
					&& letter2 == '\0' ) break;
				index++;
			}
		}
		return isequal;
	};
	::rux::uint8 XString::IsEqualA( const char* string1 , const char* string2 , size_t length )
	{		
		::rux::uint8 isequal = 1;	
		char letter1 = ' ';
		char letter2 = ' ';
		if( length == 0 ) return 0;
		length--;
		for( ; ; )
		{
			letter1 = string1[ length ];
			letter2 = string2[ length ];
		
			if( letter1 != letter2 ) 
			{				
				isequal = 0;
				break;
			}			
		
			if( length == 0 )
			{
				break;
			}
		
			length--;
		}	
		return isequal;
	};	

	size_t XString::LengthA( const char* string )
	{
		return ::strlen( string );
	};
	size_t XString::LengthUTF16( const ::rux::uint16* string )
	{
#ifdef __WINDOWS__
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif
		if( sizeof( wchar_t ) == 2 )
#ifdef __WINDOWS__
#pragma warning( pop )
#endif
			return ::wcslen( (wchar_t*)string );
		else
		{
			size_t length = 0;
			::rux::uint16 letter = 0;
			for( ; ; )
			{
				letter = string[ length ];		
				if( letter == 0 ) break;		
				length++;
			}
			return length;
		}
	};
	::rux::String& String::Trim( void ) const
	{
		if( Length() > 0 )
		{
			if( _code_page == XEnumCodePage_UTF16 )
			{
				size_t index1 = 0;
				for( index1 ; index1 < _count - 2 ; index1 += 2 )
				{
					if( *(::rux::uint16*)&_str[ index1 ] != '\n'
						&& *(::rux::uint16*)&_str[ index1 ] != '\t'
						&& *(::rux::uint16*)&_str[ index1 ] != '\r'
						&& *(::rux::uint16*)&_str[ index1 ] != ' ' )				
						break;
				}
				if( index1 < _count - 2 )
				{
					size_t index2 = _count - 4;
					for( index2 ; index2 >= index1 ; index2 -= 2 )
					{
						if( *(::rux::uint16*)&_str[ index2 ] != '\n'
							&& *(::rux::uint16*)&_str[ index2 ] != '\t'
							&& *(::rux::uint16*)&_str[ index2 ] != '\r'
							&& *(::rux::uint16*)&_str[ index2 ] != ' ' )						
							break;
						if( index2 == 0 )
							break;
					}
					::rux::XString return_string( _str , index1 / 2 , ( index2 - index1 ) / 2 + 1 , XEnumCodePage_UTF16 );
					return return_string++;
				}
			}
			else if( _code_page == XEnumCodePage_CP1251 )
			{
				size_t index1 = 0;
				for( index1 ; index1 < _count - 1 ; index1++ )
				{
					if( _str[ index1 ] != '\n'
						&& _str[ index1 ] != '\t'
						&& _str[ index1 ] != '\r'
						&& _str[ index1 ] != ' ' )
						break;
				}
				if( index1 < _count - 1 )
				{
					size_t index2 = _count - 2;
					for( index2 ; index2 >= index1 ; index2-- )
					{
						if( _str[ index2 ] != '\n'
							&& _str[ index2 ] != '\t'
							&& _str[ index2 ] != '\r'
							&& _str[ index2 ] != ' ' )
							break;
						if( index2 == 0 )
							break;
					}
					::rux::XString return_string( _str , index1 , index2 - index1 + 1 );
					return return_string++;				
				}
			}	
			else if( _code_page == XEnumCodePage_UTF8 )
			{
				size_t index1 = 0 , symbol_length0 = 0;
				for( index1 ; index1 < _count - 1 ; index1 += symbol_length0 )
				{
					symbol_length0 = XString::utf8_symbol_length( _str , index1 );
					if( symbol_length0 != 1 || ( _str[ index1 ] != '\n'
						&& _str[ index1 ] != '\t'
						&& _str[ index1 ] != '\r'
						&& _str[ index1 ] != ' ' ) )
						break;
				}
				if( index1 < _count - 1 )
				{
					size_t index2 = index1;
					size_t last_index0 = SIZE_MAX;
					for( ; index2 < _count - 1 ; index2 += symbol_length0 )
					{
						symbol_length0 = XString::utf8_symbol_length( _str , index2 );
						if( symbol_length0 == 1 && ( _str[ index2 ] == '\n'
							|| _str[ index2 ] == '\t'
							|| _str[ index2 ] == '\r'
							|| _str[ index2 ] == ' ' ) )
						{
							if( last_index0 == SIZE_MAX )
								last_index0 = index2;						
						}
						else
							last_index0 = SIZE_MAX;
					}
					if( last_index0 == SIZE_MAX )
						last_index0 = _count - 1;
					::rux::XString string1( last_index0 - index1 + 32 );
					::rux::String* string1_that = string1();
					::rux::engine::memcpy( string1_that->str() , 0 , &_str[ index1 ] , last_index0 - index1 );
					string1_that->_count = last_index0 - index1 + 1;
					string1_that->_str[ string1_that->_count - 1 ] = '\0';
					string1_that->_code_page = XEnumCodePage_UTF8;
					return string1++;
				}
			}
		}
		return XString()++;
	};
	::rux::uint32 String::get_UTF32Char( size_t index ) const
	{
		_declare_stackvar( ::rux::uint32 , utf32_symbol0 , 0 );
		utf32_symbol0 = 0;
		if( Length() > 0 )
		{
			if( _code_page == XEnumCodePage_UTF16 )
			{
				index *= 2;
				if( _count - 2 > index )		
					XString::utf16_to_utf32( (::rux::uint16*)&_str[ index ] , 1 , (char*)&utf32_symbol0 , 4 );
			}
			else if( _code_page == XEnumCodePage_CP1251 )
			{
				if( _count - 1 > index )
					XString::ansi_to_utf32( &_str[ index ] , 1 , (char*)&utf32_symbol0 , 4 );
			}
			else if( _code_page == XEnumCodePage_UTF8 )
			{
				index = XString::utf8_symbol_index_to_byte_index( _str , index );
				if( index != SIZE_MAX && _count - 1 > index )
					XString::utf8_to_utf32( &_str[ index ] , XString::utf8_symbol_length( _str , index ) , (char*)&utf32_symbol0 , 4 );
			}
		}
		return utf32_symbol0;
	};
	::rux::uint32 XString::get_UTF8Char( const char* utf8_ptr , size_t index )
	{	
		size_t utf8_symbol = 0;
		if( utf8_ptr && utf8_ptr[ 0 ] != 0 )
		{
			size_t index0 = 0;
			size_t index1 = 0 , index2 = 0;
			while( utf8_ptr[ index0 ] != 0 )
			{
				if( index1 != index + 1 )
				{
					if( ( utf8_ptr[ index0 ] & 0xc0 ) != 0x80 )
						index1++;
					if( index1 == index + 1 )
					{					
						((::rux::uint8*)&utf8_symbol)[ index2 ] = (::rux::uint8)utf8_ptr[ index0 ];
						index2++;
					}
				}
				else
				{
					if( ( utf8_ptr[ index0 ] & 0xc0 ) != 0x80 )
					{
						if( index2 > 0 )
							break;
					}
					((::rux::uint8*)&utf8_symbol)[ index2 ] = (::rux::uint8)utf8_ptr[ index0 ];
					index2++;
				}
				index0++;
			}
		}
		return utf8_symbol;
	};
	::rux::uint32 String::get_UTF8Char( size_t index ) const
	{
		_declare_stackvar( ::rux::uint32 , utf8_symbol00 , 0 );
		utf8_symbol00 = 0;
		if( Length() > 0 )
		{
			if( _code_page == XEnumCodePage_UTF16 )
			{
				index *= 2;
				if( _count - 2 > index )		
				{
					size_t utf16_to_utf8_bytes = 0;
					XString::utf16_to_utf8( (::rux::uint16*)&_str[ index ] , 1 , (char*)&utf8_symbol00 , 4 , utf16_to_utf8_bytes );
				}
			}
			else if( _code_page == XEnumCodePage_CP1251 )
			{
				if( _count - 1 > index )
				{
					size_t ansi_to_utf8_length = 0;
					XString::ansi_to_utf8( &_str[ index ] , 1 , (char*)&utf8_symbol00 , 4 , ansi_to_utf8_length );
				}
			}
			else if( _code_page == XEnumCodePage_UTF8 )
				utf8_symbol00 = XString::get_UTF8Char( _str , index );
		}
		return utf8_symbol00;
	};
	::rux::uint16 String::get_UTF16Char( size_t index ) const
	{
		if( Length() > 0 )
		{
			if( _code_page == XEnumCodePage_UTF16 )
			{
				index *= 2;
				if( _count - 2 > index )
					return *(::rux::uint16*)&_str[ index ];
			}
			else if( _code_page == XEnumCodePage_CP1251 )
			{
				if( _count - 1 > index )
				{
					_declare_stackvar( ::rux::uint16 , wide_char00 , 0 );
					wide_char00 = 0;
					size_t ansi_to_utf16_bytes = 0;
					XString::ansi_to_utf16( &_str[ index ] , 1 , &wide_char00 , 1 , ansi_to_utf16_bytes );
					return wide_char00;
				}
			}	
			else if( _code_page == XEnumCodePage_UTF8 )
			{
				index = XString::utf8_symbol_index_to_byte_index( _str , index );
				if( index != SIZE_MAX && _count - 1 > index )
				{
					_declare_stackvar( ::rux::uint16 , wide_char00 , 0 );
					wide_char00 = 0;
					size_t utf8_to_utf16_bytes = 0;
					XString::utf8_to_utf16( &_str[ index ] , XString::utf8_symbol_length( _str , index ) , &wide_char00 , 1 , utf8_to_utf16_bytes );
					return wide_char00;
				}
			}
		}
		return 0;
	};
	char String::get_AnsiChar( size_t index )
	{
		if( Length() > 0 )
		{
			if( _code_page == XEnumCodePage_CP1251 )
			{		
				if( _count - 1 > index )
					return _str[ index ];
			}
			else if( _code_page == XEnumCodePage_UTF16 )
			{
				index *= 2;
				if( _count - 2 > index )
				{
					_declare_stackvar( char , symbol00 , 0 );
					symbol00 = 0;
					size_t utf16_to_ansi_bytes = 0;
					XString::utf16_to_ansi( (::rux::uint16*)&_str[ index ] , 1 , &symbol00 , 1 , utf16_to_ansi_bytes );
					return symbol00;
				}
			}
			else if( _code_page == XEnumCodePage_UTF8 )
			{
				index = XString::utf8_symbol_index_to_byte_index( _str , index );
				if( index != SIZE_MAX && index < _count - 1 )
				{
					_declare_stackvar( char , symbol00 , 0 );
					symbol00 = 0;
					size_t utf8_to_ansi_byte = 0;
					XString::utf8_to_ansi( &_str[ index ] , XString::utf8_symbol_length( _str , index ) , &symbol00 , 1 , utf8_to_ansi_byte );
					return symbol00;
				}
			}
		}
		return 0;
	};
	size_t XString::LengthUTF8( const char* utf8_string_ptr )
	{	
		size_t count = 0;
		if( utf8_string_ptr )
		{
			char* ptr = const_cast< char* >( utf8_string_ptr );
			while( *ptr != 0 )
			{
				if( ( (*ptr) & 0xc0 ) != 0x80 )
					count++;
				ptr++;
			}
		}
		return count;
	};
	void XString::utf8_to_utf32( const char* utf8_ptr , size_t utf8_ptr_count , char* utf32_ptr , size_t utf32_ptr_count )
	{
		size_t bytes = 0;
		::rux::utils::string::utf8::to_utf32( utf8_ptr , utf8_ptr_count , utf32_ptr , utf32_ptr_count , bytes );
		::rux::memory::check_memory( utf32_ptr );
	};
	void XString::utf8_to_ansi( const char* utf8_ptr , size_t utf8_ptr_count , char* cp1251_ptr , size_t cp1251_ptr_count , size_t& result_bytes )
	{
#ifdef __WINDOWS__
		if( _rux_ansi_code_page == XEnumCodePage_CP1251 )
			::rux::utils::string::utf8::to_cp1251( utf8_ptr , utf8_ptr_count , cp1251_ptr , cp1251_ptr_count , result_bytes );
		else
		{
			wchar_t* wide_array_ptr = alloc_array_macros( wchar_t , utf8_ptr_count + 1 );
			size_t wide_array_ptr_length = MultiByteToWideChar( XEnumCodePage_UTF8 , 0 , utf8_ptr , utf8_ptr_count  , wide_array_ptr , utf8_ptr_count );	
			result_bytes = WideCharToMultiByte( _rux_ansi_code_page , WC_NO_BEST_FIT_CHARS , wide_array_ptr , wide_array_ptr_length , cp1251_ptr , cp1251_ptr_count , NULL , NULL );
			::rux::engine::free_mem( wide_array_ptr );
		}
#else
		::rux::utils::string::utf8::to_cp1251( utf8_ptr , utf8_ptr_count , cp1251_ptr , cp1251_ptr_count , result_bytes );
#endif
		if( result_bytes > 0 )
		{
			if( cp1251_ptr[ result_bytes - 1 ] == 0 )
				result_bytes--;
		}
		cp1251_ptr[ result_bytes ] = 0;
		::rux::memory::check_memory( cp1251_ptr );
	};
	::rux::uint16* XString::ansi_to_utf16( const char* cp1251_ptr , size_t cp1251_ptr_count , size_t& result_bytes )
	{
		::rux::uint16* wide_char_array = alloc_array_macros( ::rux::uint16 , cp1251_ptr_count + 1 );
#ifdef __WINDOWS__
		if( _rux_ansi_code_page == XEnumCodePage_CP1251 )
			::rux::utils::string::cp1251::to_utf16( cp1251_ptr , cp1251_ptr_count , (char*)wide_char_array , 2 * ( cp1251_ptr_count + 1 ) , result_bytes );
		else
		{
			result_bytes = MultiByteToWideChar( _rux_ansi_code_page , MB_USEGLYPHCHARS , cp1251_ptr , cp1251_ptr_count , (wchar_t*)wide_char_array , cp1251_ptr_count );
			result_bytes *= 2;
		}
#else
		::rux::utils::string::cp1251::to_utf16( cp1251_ptr , cp1251_ptr_count , (char*)wide_char_array , 2 * ( cp1251_ptr_count + 1 ) , result_bytes );
#endif	
		if( result_bytes > 0 )
		{
			if( wide_char_array[ result_bytes / 2 - 1 ] == 0 )
				result_bytes -= 2;
		}
		wide_char_array[ result_bytes / 2 ] = 0;
		::rux::memory::check_memory( wide_char_array );
		return wide_char_array;
	};
	void XString::ansi_to_utf16( const char* cp1251_ptr , size_t cp1251_ptr_count , ::rux::uint16* utf16_ptr , size_t utf16_ptr_count , size_t& result_bytes )
	{
#ifdef __WINDOWS__	
		if( _rux_ansi_code_page == XEnumCodePage_CP1251 )
			::rux::utils::string::cp1251::to_utf16( cp1251_ptr , cp1251_ptr_count , (char*)utf16_ptr , utf16_ptr_count * 2 , result_bytes );
		else
		{
			result_bytes = MultiByteToWideChar( _rux_ansi_code_page , MB_USEGLYPHCHARS , cp1251_ptr , cp1251_ptr_count , (wchar_t*)utf16_ptr , utf16_ptr_count );
			result_bytes *= 2;
		}
#else
		::rux::utils::string::cp1251::to_utf16( cp1251_ptr , cp1251_ptr_count , (char*)utf16_ptr , utf16_ptr_count * 2 , result_bytes );
#endif
		if( result_bytes > 0 )
		{
			if( utf16_ptr[ result_bytes / 2 - 1 ] == 0 )
				result_bytes -= 2;
		}
		utf16_ptr[ result_bytes / 2 ] = 0;
		::rux::memory::check_memory( utf16_ptr );
	};
	::rux::uint16* XString::utf8_to_utf16( const char* utf8_ptr , size_t utf8_ptr_count , size_t& result_bytes )
	{
		::rux::uint16* wide_array_ptr = alloc_array_macros( ::rux::uint16 , utf8_ptr_count + 1 );
		::rux::utils::string::utf8::to_utf16( utf8_ptr , utf8_ptr_count , (char*)wide_array_ptr , 2 * ( utf8_ptr_count + 1 ) , result_bytes );
		if( result_bytes > 0 )
		{
			if( wide_array_ptr[ result_bytes / 2 - 1 ] == 0 )
				result_bytes -= 2;
		}
		wide_array_ptr[ result_bytes / 2 ] = 0;
		::rux::memory::check_memory( wide_array_ptr );
		return wide_array_ptr;
	};
	void XString::utf32_to_utf16( const char* utf32_ptr , size_t utf32_ptr_count , ::rux::uint16* utf16_ptr , size_t utf16_ptr_count )
	{
		size_t utf16_bytes = 0;
		::rux::utils::string::utf32::to_utf16( utf32_ptr , utf32_ptr_count , (char*)utf16_ptr , utf16_ptr_count * 2 ,  utf16_bytes );
		::rux::memory::check_memory( utf16_ptr );
	};
	void XString::utf8_to_utf16( const char* utf8_ptr , size_t utf8_ptr_count , ::rux::uint16* utf16_ptr , size_t utf16_ptr_count , size_t& result_bytes )
	{
		::rux::utils::string::utf8::to_utf16( utf8_ptr , utf8_ptr_count , (char*)utf16_ptr , 2 * utf16_ptr_count , result_bytes );
		if( result_bytes > 0 )
		{
			if( utf16_ptr[ result_bytes / 2 - 1 ] == 0 )
				utf16_ptr -= 2;
		}
		utf16_ptr[ result_bytes / 2 ] = 0;
		::rux::memory::check_memory( utf16_ptr );
	};
	void XString::ansi_to_utf32( const char* cp1251_ptr , size_t cp1251_ptr_count , char* utf32_ptr , size_t utf32_ptr_count )
	{
#ifdef __WINDOWS__	
		if( _rux_ansi_code_page == XEnumCodePage_CP1251 )
		{
			size_t utf32_bytes = 0;
			::rux::utils::string::cp1251::to_utf32( cp1251_ptr , cp1251_ptr_count , utf32_ptr , utf32_ptr_count , utf32_bytes );
		}
		else
		{
			::rux::uint16* source_array_ptr = alloc_array_macros( ::rux::uint16 , cp1251_ptr_count + 1 );
			MultiByteToWideChar( _rux_ansi_code_page , MB_USEGLYPHCHARS , cp1251_ptr , cp1251_ptr_count , (wchar_t*)source_array_ptr , cp1251_ptr_count );				
			utf16_to_utf32( (::rux::uint16*)source_array_ptr , cp1251_ptr_count , utf32_ptr , utf32_ptr_count );
			::rux::engine::free_mem( source_array_ptr );					
		}
#else
		size_t utf32_bytes = 0;
		::rux::utils::string::cp1251::to_utf32( cp1251_ptr , cp1251_ptr_count , utf32_ptr , utf32_ptr_count , utf32_bytes );
#endif
		::rux::memory::check_memory( utf32_ptr );
	};
	void XString::ansi_to_utf8( const char* cp1251_ptr , size_t cp1251_ptr_count , char* utf8_ptr , size_t utf8_ptr_count , size_t& result_bytes )
	{
#ifdef __WINDOWS__	
		if( _rux_ansi_code_page == XEnumCodePage_CP1251 )
			::rux::utils::string::cp1251::to_utf8( cp1251_ptr , cp1251_ptr_count , utf8_ptr , utf8_ptr_count , result_bytes );
		else
		{
			::rux::uint16* source_array_ptr = alloc_array_macros( ::rux::uint16 , cp1251_ptr_count + 1 );
			MultiByteToWideChar( _rux_ansi_code_page , MB_USEGLYPHCHARS , cp1251_ptr , cp1251_ptr_count , (wchar_t*)source_array_ptr , cp1251_ptr_count );				
			result_bytes = WideCharToMultiByte( XEnumCodePage_UTF8 , 0 , (wchar_t*)source_array_ptr , cp1251_ptr_count , utf8_ptr , utf8_ptr_count , NULL , NULL );
			::rux::engine::free_mem( source_array_ptr );					
		}
#else
		::rux::utils::string::cp1251::to_utf8( cp1251_ptr , cp1251_ptr_count , utf8_ptr , utf8_ptr_count , result_bytes );
#endif
		if( result_bytes > 0 )
		{
			if( utf8_ptr[ result_bytes - 1 ] == 0 )
				utf8_ptr--;
		}
		utf8_ptr[ result_bytes ] = 0;
		::rux::memory::check_memory( utf8_ptr );
	};
	char* XString::ansi_to_utf8( const char* cp1251_ptr , size_t cp1251_ptr_count , size_t& res_length )
	{
		char* utf8_ptr = alloc_array_macros( char , cp1251_ptr_count * 4 + 1 );
#ifdef __WINDOWS__
		if( _rux_ansi_code_page == XEnumCodePage_CP1251 )
			::rux::utils::string::cp1251::to_utf8( cp1251_ptr , cp1251_ptr_count , utf8_ptr , cp1251_ptr_count * 4 + 1 , res_length );
		else
		{
			::rux::uint16* source_array_ptr = alloc_array_macros( ::rux::uint16 , cp1251_ptr_count + 1 );
			MultiByteToWideChar( _rux_ansi_code_page , MB_USEGLYPHCHARS , cp1251_ptr , cp1251_ptr_count , (wchar_t*)source_array_ptr , cp1251_ptr_count );				
			res_length = WideCharToMultiByte( XEnumCodePage_UTF8 , 0 , (wchar_t*)source_array_ptr , cp1251_ptr_count , utf8_ptr , cp1251_ptr_count * 4 , NULL , NULL );
			::rux::engine::free_mem( source_array_ptr );
		}
#else
		::rux::utils::string::cp1251::to_utf8( cp1251_ptr , cp1251_ptr_count , utf8_ptr , cp1251_ptr_count * 4 + 1 , res_length );
#endif
		if( res_length > 0 )
		{
			if( utf8_ptr[ res_length - 1 ] == 0 )
				res_length--;
		}
		utf8_ptr[ res_length ] = 0;
		::rux::memory::check_memory( utf8_ptr );
		return utf8_ptr;
	};
	void XString::utf32_to_utf8( const char* utf32_ptr , size_t utf32_ptr_count , char* utf8_ptr , size_t utf8_ptr_count )
	{
		size_t utf8_bytes = 0;
		::rux::utils::string::utf32::to_utf8( utf32_ptr , utf32_ptr_count , utf8_ptr , utf8_ptr_count , utf8_bytes );
		::rux::memory::check_memory( utf8_ptr );
	};
	char* XString::utf16_to_utf8( const ::rux::uint16* utf16_ptr , size_t utf16_ptr_count , size_t& res_length )
	{
		char* utf8_array_ptr = alloc_array_macros( char , utf16_ptr_count * 4 + 1 );
		::rux::utils::string::utf16::to_utf8( (char*)utf16_ptr , 2 * utf16_ptr_count , utf8_array_ptr , utf16_ptr_count * 4 + 1 , res_length );
		if( res_length > 0 )
		{
			if( utf8_array_ptr[ res_length - 1 ] == 0 )
				res_length--;
		}
		utf8_array_ptr[ res_length ] = 0;
		::rux::memory::check_memory( utf8_array_ptr );
		return utf8_array_ptr;
	};
	void XString::utf16_to_utf8( const ::rux::uint16* utf16_ptr , size_t utf16_ptr_count , char* utf8_ptr , size_t utf8_ptr_count , size_t& res_length )
	{
		::rux::utils::string::utf16::to_utf8( (char*)utf16_ptr , 2 * utf16_ptr_count , utf8_ptr , utf8_ptr_count , res_length );
		if( res_length > 0 )
		{
			if( utf8_ptr[ res_length - 1 ] == 0 )
				res_length--;
		}
		utf8_ptr[ res_length ] = 0;
		::rux::memory::check_memory( utf8_ptr );
	};
	void XString::utf16_to_utf32( const ::rux::uint16* utf16_ptr , size_t utf16_ptr_count , char* utf32_ptr , size_t utf32_ptr_count )
	{
		size_t bytes = 0;
		::rux::utils::string::utf16::to_utf32( (char*)utf16_ptr , 2 * utf16_ptr_count , utf32_ptr , utf32_ptr_count , bytes );
		::rux::memory::check_memory( utf32_ptr );
	};
	void XString::utf16_to_ansi( const ::rux::uint16* utf16_ptr , size_t utf16_ptr_count , char* cp1251_ptr , size_t cp1251_ptr_count , size_t& result_bytes )
	{	
#ifdef __WINDOWS__
		if( _rux_ansi_code_page == XEnumCodePage_CP1251 )
			::rux::utils::string::utf16::to_cp1251( (char*)utf16_ptr , 2 * utf16_ptr_count , cp1251_ptr , cp1251_ptr_count , result_bytes );
		else
			result_bytes = WideCharToMultiByte( _rux_ansi_code_page , WC_NO_BEST_FIT_CHARS , (wchar_t*)utf16_ptr , utf16_ptr_count , cp1251_ptr , cp1251_ptr_count , NULL , NULL );
#else
		::rux::utils::string::utf16::to_cp1251( (char*)utf16_ptr , 2 * utf16_ptr_count , cp1251_ptr , cp1251_ptr_count , result_bytes );
#endif
		if( cp1251_ptr > 0 )
		{
			if( cp1251_ptr[ result_bytes - 1 ] == 0 )
				result_bytes--;
		}
		cp1251_ptr[ result_bytes ] = 0;
		::rux::memory::check_memory( cp1251_ptr );
	};
	size_t XString::utf8_symbol_index_to_byte_index( const char* utf8_ptr , size_t symbol_index )
	{
		if( utf8_ptr == 0 || symbol_index == SIZE_MAX )
			return SIZE_MAX;
		else
		{
			size_t byte_index = 0;
			size_t symbol_index1 = 0;
			for( ; ; )
			{
				if( utf8_ptr[ byte_index ] == 0 )		
				{
					byte_index = SIZE_MAX;
					break;
				}
				if( ( utf8_ptr[ byte_index ] & 0xc0 ) != 0x80 )
					symbol_index1++;
				if( symbol_index1 == symbol_index + 1 )
					break;
				byte_index++;
			}
			return byte_index;
		}
	};
	size_t XString::utf8_symbol_length_to_byte_length( const char* utf8_ptr , size_t symbol_index , size_t symbol_length )
	{
		if( symbol_index == SIZE_MAX )
			return SIZE_MAX;
		else
		{
			size_t byte_index = 0;
			size_t symbol_index1 = 0;
			for( ; ; )
			{
				if( utf8_ptr[ byte_index ] == 0 )
				{
					byte_index = SIZE_MAX;
					break;
				}
				if( ( utf8_ptr[ byte_index ] & 0xc0 ) != 0x80 )
					symbol_index1++;
				if( symbol_index1 == symbol_index + 1 )
					break;
				byte_index++;
			}
			if( byte_index != SIZE_MAX )
			{
				size_t byte_length = byte_index;
				symbol_index1 = 0;
				for( ; ; )
				{
					if( utf8_ptr[ byte_length ] == 0 )		
						break;
					if( ( utf8_ptr[ byte_length ] & 0xc0 ) != 0x80 )
						symbol_index1++;
					if( symbol_length != SIZE_MAX && symbol_index1 == symbol_length + 1 )
						break;
					byte_length++;
				}
				byte_length -= byte_index;
				return byte_length;
			}
			else
				return SIZE_MAX;
		}
	};
	void XString::array_right_shift( char* array_ptr , size_t index , size_t shift_count , size_t array_count )
	{
		if( shift_count > 0 && array_count > 0 )
		{
			for( size_t index1 = array_count - 1 ; index1 > index + shift_count - 1 ; index1-- )	
				array_ptr[ index1 ] = array_ptr[ index1 - shift_count ];
		}
	};
	void XString::array_left_shift( char* array_ptr , size_t index , size_t shift_count , size_t array_count )
	{
		for( ; index + shift_count < array_count ; index++ )	
			array_ptr[ index ] = array_ptr[ index + shift_count ];
	};
	size_t String::get_step( void ) const
	{
		if( _step < STRING_MAX_STEP )
			return const_cast< String* >( this )->_step *= 2;
		else
			return _step;
	};
	::rux::uint32 XString::HexToUInt32( const ::rux::XString& hex )
	{
		size_t value = 0;
		if( hex.Length() > 0 )
		{
			::rux::uint8 even = 0;
			if( hex.Length() % 2 == 0 )
				even = 1;
			size_t index0 = 0 , utf8 = 0;
			if( hex.get_UTF8Char( 0 ) == '0'
				&& ( hex.Length() > 1 && hex.get_UTF8Char( 1 ) == 'x' ) )
				index0 = 2;		
			if( even == 0 && index0 < hex.Length() )
			{
				utf8 = hex.get_UTF8Char( index0 );
				switch( utf8 )
				{
				case '1':
					{
						value = 1;
						break;
					}
				case '2':
					{
						value = 2;
						break;
					}
				case '3':
					{
						value = 3;
						break;
					}
				case '4':
					{
						value = 4;
						break;
					}
				case '5':
					{
						value = 5;
						break;
					}
				case '6':
					{
						value = 6;
						break;
					}
				case '7':
					{
						value = 7;
						break;
					}
				case '8':
					{
						value = 8;
						break;
					}
				case '9':
					{
						value = 9;
						break;
					}
				case '0':
					{
						value = 0;
						break;
					}
				case 'a':
				case 'A':
					{
						value = 10;
						break;
					}
				case 'b':
				case 'B':
					{
						value = 11;
						break;
					}
				case 'c':
				case 'C':
					{
						value = 12;
						break;
					}
				case 'd':
				case 'D':
					{
						value = 13;
						break;
					}
				case 'e':
				case 'E':
					{
						value = 14;
						break;
					}
				case 'f':
				case 'F':
					{
						value = 15;
						break;
					}
				}
				index0++;
			}
			::rux::uint8 byte = 0;
			while( index0 < hex.Length() )
			{
				value <<= 8;			
				utf8 = hex.get_UTF8Char( index0 );
				switch( utf8 )
				{
				case '1':
					{
						byte = 1 * 16;
						break;
					}
				case '2':
					{
						byte = 2 * 16;
						break;
					}
				case '3':
					{
						byte = 3 * 16;
						break;
					}
				case '4':
					{
						byte = 4 * 16;
						break;
					}
				case '5':
					{
						byte = 5 * 16;
						break;
					}
				case '6':
					{
						byte = 6 * 16;
						break;
					}
				case '7':
					{
						byte = 7 * 16;
						break;
					}
				case '8':
					{
						byte = 8 * 16;
						break;
					}
				case '9':
					{
						byte = 9 * 16;
						break;
					}
				case '0':
					{
						byte = 0;
						break;
					}
				case 'a':
				case 'A':
					{
						byte = 10 * 16;
						break;
					}
				case 'b':
				case 'B':
					{
						byte = 11 * 16;
						break;
					}
				case 'c':
				case 'C':
					{
						byte = 12 * 16;
						break;
					}
				case 'd':
				case 'D':
					{
						byte = 13 * 16;
						break;
					}
				case 'e':
				case 'E':
					{
						byte = 14 * 16;
						break;
					}
				case 'f':
				case 'F':
					{
						byte = 15 * 16;
						break;
					}
				}
				index0++;
				utf8 = hex.get_UTF8Char( index0 );
				switch( utf8 )
				{
				case '1':
					{
						byte += 1;
						break;
					}
				case '2':
					{
						byte += 2;
						break;
					}
				case '3':
					{
						byte += 3;
						break;
					}
				case '4':
					{
						byte += 4;
						break;
					}
				case '5':
					{
						byte += 5;
						break;
					}
				case '6':
					{
						byte += 6;
						break;
					}
				case '7':
					{
						byte += 7;
						break;
					}
				case '8':
					{
						byte += 8;
						break;
					}
				case '9':
					{
						byte += 9;
						break;
					}
				case '0':
					{
						byte += 0;
						break;
					}
				case 'a':
				case 'A':
					{
						byte += 10;
						break;
					}
				case 'b':
				case 'B':
					{
						byte += 11;
						break;
					}
				case 'c':
				case 'C':
					{
						byte += 12;
						break;
					}
				case 'd':
				case 'D':
					{
						byte += 13;
						break;
					}
				case 'e':
				case 'E':
					{
						byte += 14;
						break;
					}
				case 'f':
				case 'F':
					{
						byte += 15;
						break;
					}
				}
				value |= byte;
				index0++;
			}
		}
		return value;
	};
	static void cp1251_to_utf8(char *out, const char *in)
	{
		static const int table[128] = {                    
			0x82D0,0x83D0,0x9A80E2,0x93D1,0x9E80E2,0xA680E2,0xA080E2,0xA180E2,
			0xAC82E2,0xB080E2,0x89D0,0xB980E2,0x8AD0,0x8CD0,0x8BD0,0x8FD0,    
			0x92D1,0x9880E2,0x9980E2,0x9C80E2,0x9D80E2,0xA280E2,0x9380E2,0x9480E2,
			0,0xA284E2,0x99D1,0xBA80E2,0x9AD1,0x9CD1,0x9BD1,0x9FD1,               
			0xA0C2,0x8ED0,0x9ED1,0x88D0,0xA4C2,0x90D2,0xA6C2,0xA7C2,              
			0x81D0,0xA9C2,0x84D0,0xABC2,0xACC2,0xADC2,0xAEC2,0x87D0,              
			0xB0C2,0xB1C2,0x86D0,0x96D1,0x91D2,0xB5C2,0xB6C2,0xB7C2,              
			0x91D1,0x9684E2,0x94D1,0xBBC2,0x98D1,0x85D0,0x95D1,0x97D1,            
			0x90D0,0x91D0,0x92D0,0x93D0,0x94D0,0x95D0,0x96D0,0x97D0,
			0x98D0,0x99D0,0x9AD0,0x9BD0,0x9CD0,0x9DD0,0x9ED0,0x9FD0,
			0xA0D0,0xA1D0,0xA2D0,0xA3D0,0xA4D0,0xA5D0,0xA6D0,0xA7D0,
			0xA8D0,0xA9D0,0xAAD0,0xABD0,0xACD0,0xADD0,0xAED0,0xAFD0,
			0xB0D0,0xB1D0,0xB2D0,0xB3D0,0xB4D0,0xB5D0,0xB6D0,0xB7D0,
			0xB8D0,0xB9D0,0xBAD0,0xBBD0,0xBCD0,0xBDD0,0xBED0,0xBFD0,
			0x80D1,0x81D1,0x82D1,0x83D1,0x84D1,0x85D1,0x86D1,0x87D1,
			0x88D1,0x89D1,0x8AD1,0x8BD1,0x8CD1,0x8DD1,0x8ED1,0x8FD1
		};
		while (*in)
			if (*in & 0x80) {
				int v = table[(int)(0x7f & *in++)];
				if (!v)
					continue;
				*out++ = (char)v;
				*out++ = (char)(v >> 8);
				if (v >>= 16)
					*out++ = (char)v;
			}
			else
				*out++ = *in++;
		*out = 0;
	}
};