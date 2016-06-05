#ifndef XAN_STRING_H
#define XAN_STRING_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_defines.h>
#include <xan_array.h>
#include <xan_object.h>
#include <xan_char.h>
enum EStringSplitOptions
{
	EStringSplitOptions_None,
	EStringSplitOptions_RemoveEmptyEntries
};
enum XEnumCodePage
{
	XEnumCodePage_Unknown = 0,
	XEnumCodePage_UTF16 = 1200,
	XEnumCodePage_CP1251 = 1251,		
	XEnumCodePage_UTF8 = 65001 ,
	XEnumCodePage_UTF32 = 12000
};
extern dll_public XEnumCodePage g_code_page;
extern dll_public ::rux::uint32 _rux_ansi_code_page;
namespace rux
{
	class XConsole;
	namespace data
	{
		class 	XJsonSerializer;
	};
	class String;
	#define STRING_MAX_STEP (size_t)524288
	class XString : public XGCHandle<String>
	{
		friend class ::rux::XConsole;
		friend class String;
		friend class ::rux::data::XJsonSerializer;
		declare_rux_base_methods( String );
	public:
		declare_rux_set_operators( String );
		XString();	
		explicit XString( size_t capacity , const char* variable_name = NULL , const char* __file__ = NULL , ::rux::int32 __line__ = 0 );
		XString( const char* text );	
		explicit XString( const char* string_ptr , XEnumCodePage code_page , ::rux::uint8 copy_string = 1 , const char* variable_name = NULL , const char* __file__ = NULL , ::rux::int32 __line__ = 0 );
		explicit XString( const char* text_ptr , size_t start_index , size_t length , size_t step = 32 );
		explicit XString( const char* text_ptr , size_t start_index , size_t length , XEnumCodePage code_page );
		explicit XString( const char* text_ptr , size_t start_index );	
		explicit XString( const char* text_ptr , size_t start_index , XEnumCodePage code_page );	
		XString( const wchar_t* text );
		explicit XString( const wchar_t* text_ptr , size_t start_index , size_t length , size_t step = 32 );
		explicit XString( const wchar_t* text_ptr , size_t start_index );
		XString( const String& string );
		explicit XString( const XObject& object );
		XString( const XString& string );
		char* str( void ) const;
		void Insert( size_t index , const char letter );
		void InsertRange( size_t index , const ::rux::XString& string );	
		void InsertRange( size_t index , const ::rux::String& string );
		void InsertRange( size_t index , const char* string_ptr , size_t count );
		void InsertRange( size_t index , const wchar_t* string_ptr , size_t count );
		void RemoveAt( size_t index );
		void RemoveRange( size_t index , size_t count );
		void Clear();	
		size_t Length() const;	
		size_t Size() const;


		::rux::int32 IndexOfAny( const ::rux::XArray< ::rux::XChar>& char_array , size_t index = 0 );
		::rux::int32 IndexOfAny( const ::rux::XArray<XString>& string_array , size_t index = 0 );
		::rux::int32 IndexOfAny( const ::rux::XArray< ::rux::XChar>& char_array , size_t index , size_t& array_index );
		::rux::int32 IndexOfAny( const ::rux::XArray<XString>& string_array , size_t index , size_t& array_index );
		::rux::int32 IndexOf( const char letter , size_t index = 0 ) const;	
		::rux::int32 IndexOf( const char* text_ptr , size_t index = 0 ) const;
		::rux::int32 IndexOf( const ::rux::XString& string , size_t index = 0 ) const;
		::rux::int32 IndexOf( const String& string , size_t index = 0 ) const;
		::rux::int32 LastIndexOf( const char letter , size_t index = SIZE_MAX ) const;
		::rux::int32 LastIndexOf( const char* letter , size_t index = SIZE_MAX );
		::rux::int32 LastIndexOf( const ::rux::XString& string , size_t index = SIZE_MAX ) const;
		::rux::int32 LastIndexOf( const String& string , size_t index = SIZE_MAX );
		bool Equals( const char* text );
		bool Equals( const char* text , size_t length ) const;
		bool Equals( const String& string );
		bool Equals( const ::rux::XString& string );
		static String& Join( const ::rux::XString& string , const ::rux::XArray<XString>& array , size_t start_index = 0 , size_t count = SIZE_MAX );
		static String& Join( const String& string , const ::rux::XArray<XString>& array , size_t start_index = 0 , size_t count = SIZE_MAX );
		static String& Join( const char* text_ptr , const ::rux::XArray<XString>& array , size_t start_index = 0 , size_t count = SIZE_MAX );
		static void Join( const char* text , const ::rux::XArray<XString>& array , ::rux::XString& destination );
		
		::rux::String& ReplaceAll( const char old_letter , const char new_letter , size_t index = 0 ) const;
		::rux::String& ReplaceAll( const char* old_text , const char* new_text , size_t index = 0 ) const;
		::rux::String& ReplaceAll( const ::rux::XString& old_string , const ::rux::XString& new_string , size_t index = 0 ) const;
		::rux::String& ReplaceAll( const ::rux::XString& old_string , const char* new_text , size_t index = 0 ) const;
		::rux::String& ReplaceAll( const ::rux::XString& old_string , const String& new_string , size_t index = 0 ) const;
		::rux::String& ReplaceAll( const String& OldString , const ::rux::XString& NewString , size_t index = 0 ) const;
		::rux::String& ReplaceAll( const String& old_string , const char* new_string , size_t index = 0 ) const;
		::rux::String& ReplaceAll( const char* old_text , const ::rux::XString& new_string , size_t index = 0 ) const;
		::rux::String& ReplaceAll( const char* old_text , const String& new_string , size_t index = 0 ) const;
		::rux::String& ReplaceAll( const ::rux::String& old_string , const ::rux::String& new_string , size_t index = 0 ) const;

		void Shrink( void );

		::rux::String& ReplaceFirst( const char old_letter , const char new_letter , size_t index = 0 ) const;	
		::rux::String& ReplaceFirst( const char* old_text , const char* new_text , size_t index = 0 ) const;
		::rux::String& ReplaceFirst( const ::rux::XString& old_string , const ::rux::XString& new_string , size_t index = 0 ) const;
		::rux::String& ReplaceFirst( const ::rux::XString& old_string , const char* new_text , size_t index = 0 ) const;
		::rux::String& ReplaceFirst( const ::rux::XString& old_string , const String& new_string , size_t index = 0 ) const;
		::rux::String& ReplaceFirst( const String& old_string , const ::rux::XString& new_string , size_t index = 0 ) const;
		::rux::String& ReplaceFirst( const String& old_string , const char* new_string , size_t index = 0 ) const;
		::rux::String& ReplaceFirst( const char* old_text , const ::rux::XString& new_string , size_t index = 0 ) const;
		::rux::String& ReplaceFirst( const char* old_text , const String& new_string , size_t index = 0 ) const;
		::rux::String& ReplaceFirst( const ::rux::String& old_string , const ::rux::String& new_string , size_t index = 0 ) const;
		
		::rux::String& ReplaceLast( const char old_letter , const char new_letter , size_t index = SIZE_MAX ) const;
		::rux::String& ReplaceLast( const char* old_text , const char* new_text , size_t index = SIZE_MAX ) const;
		::rux::String& ReplaceLast( const ::rux::XString& old_string , const ::rux::XString& new_string , size_t index = SIZE_MAX ) const;
		::rux::String& ReplaceLast( const ::rux::XString& old_string , const char* new_text , size_t index = SIZE_MAX ) const;
		::rux::String& ReplaceLast( const ::rux::XString& old_string , const String& new_string , size_t index = SIZE_MAX ) const;
		::rux::String& ReplaceLast( const String& old_string , const ::rux::XString& new_string , size_t index = SIZE_MAX ) const;
		::rux::String& ReplaceLast( const String& OldString , const char* NewString , size_t index = SIZE_MAX ) const;
		::rux::String& ReplaceLast( const char* old_text , const ::rux::XString& new_string , size_t index = SIZE_MAX ) const;
		::rux::String& ReplaceLast( const char* old_text , const String& new_string , size_t index = SIZE_MAX ) const;
		::rux::String& ReplaceLast( const ::rux::String& old_string , const ::rux::String& new_string , size_t index = SIZE_MAX ) const;

		String& Substring( size_t start_index , size_t length = SIZE_MAX ) const;
		String& ToLower( size_t start_index = 0 , size_t length = SIZE_MAX ) const;
		String& ToUpper( size_t start_index = 0 , size_t length = SIZE_MAX ) const;
		::rux::Array<XString>& Split( const ::rux::XArray<XString>& separators ) const;
		::rux::Array<XString>& Split( const ::rux::XArray<XString>& separators , EStringSplitOptions string_split_options) const;	
		Array<XString>& Split( const char separator ) const;
		Array<XString>& Split( const char* separator ) const;
		Array<XString>& Split( const char* separator , EStringSplitOptions string_split_options) const;
		String& Trim( void ) const;	
		::rux::uint32 get_UTF32Char( size_t index ) const;
		::rux::uint16 get_UTF16Char( size_t index ) const;
		char get_AnsiChar( size_t index ) const;
		static ::rux::uint32 get_UTF8Char( const char* utf8_ptr , size_t index );
		::rux::uint32 get_UTF8Char( size_t index ) const;
		bool IsInt32( void );
		static ::rux::uint8 IsInt32( ::rux::uint32 letter );
		static ::rux::uint8 IsInt32( const char* word );
		bool IsDouble( void );
		void CopyToArrayAsCurrentCodePage( char destination[] , size_t destination_size ) const; 
	#ifdef __WINDOWS__
		BSTR Convert( void ) const;
	#endif
		::rux::String& ConvertToAnsi( void ) const;
		::rux::String& ConvertToUTF16( void ) const;
		::rux::String& ConvertToUTF8( void ) const;
		::rux::String& ConvertToCurrentAnsiPage( void ) const;
		void SwapHere( size_t index1 , size_t index2 );
		::rux::int32 ToInt32( void ) const;
		::rux::uint32 ToUInt32( void );
		::rux::int64 ToInt64( void );
		::rux::uint64 ToUInt64( void );
		static ::rux::uint32 HexToUInt32( const ::rux::XString& hex );
		static ::rux::int32 ToInt32( const char* text_ptr );
		static ::rux::uint32 ToUInt32( const char* text_ptr );
		static ::rux::int64 ToInt64( const char* text_ptr );
		static ::rux::uint64 ToUInt64( const char* text_ptr );
		static double ToDouble( const char* text_ptr );
		double ToDouble( void );	
		friend String& operator +( const ::rux::XString& string, const ::rux::XString& string2);	
		friend String& operator +( const ::rux::XString& string, const char* text);
		friend String& operator +( const ::rux::XString& string, const wchar_t* string_ptr);
		friend String& operator +( const ::rux::XString& string, const String& string2);	
		friend String& operator +(const char* text, const ::rux::XString& string);
		friend String& operator +(const wchar_t* string_ptr, const ::rux::XString& string);
		friend String& operator +(const char* text , const String& string);
		friend String& operator +(const wchar_t* string_ptr , const String& string);
		friend String& operator +( const String& string,const char* text);
		friend String& operator +( const String& string,const wchar_t* text_ptr);
		friend String& operator +( const String& string, const ::rux::XString& string2);
		friend String& operator +( const String& string, const String& string2);	

		friend bool operator ==(const char* text_ptr, const String& string2);
		friend bool operator ==(const char* text_ptr, const ::rux::XString& string2);	
		friend bool operator !=(const char* text_ptr, const String& string2);
		friend bool operator !=(const char* text_ptr, const ::rux::XString& string2);	
		::rux::XString& operator =(const char* text_ptr);	
		::rux::XString& operator =(const wchar_t* text_ptr);
		::rux::XString& operator =( const XGCRef* object );
		bool operator ==( const ::rux::XString& string) const;
		bool operator ==(const char* text) const;
		bool operator ==( const XString* string_ptr ) const;
		bool operator ==( const String& string ) const;
		bool operator !=( const ::rux::XString& string) const;
		bool operator >( const ::rux::XString& string ) const;
		bool operator !=(const char* text) const;
		bool operator !=( const XString* string_ptr ) const;
		bool operator !=( const String& string ) const;
		::rux::XString& operator +=( const ::rux::XString& string);	
		::rux::XString& operator +=(const char* text);		
		::rux::XString& operator +=(const wchar_t* string_ptr);	
		::rux::XString& operator +=( const String& string );	
		size_t get_step( void ) const;
		size_t get_AllocatedMemory( void );
		void IncreaseMemory( size_t size );
		
		static String& Format( const char* format, ... );	
		static String& FormatVAList( const char* format, va_list ap );
	
		static ::rux::uint8 IsEqualA( const char* string1 , const char* string2 );
		static ::rux::uint8 IsEqualA( const char* string1 , const char* string2 , size_t length );	
		static ::rux::uint8 IsEqualUTF16( const ::rux::uint16* string1 , const ::rux::uint16* string2 );
		
		static size_t LengthUTF16( const ::rux::uint16* string_ptr );
		static size_t LengthUTF8( const char* string_ptr );

		static size_t LengthA( const char* string );	
	private:
		static ::rux::uint16* utf8_to_utf16( const char* utf8_ptr , size_t utf8_ptr_count , size_t& result_bytes );
		static void utf8_to_utf16( const char* utf8_ptr , size_t utf8_ptr_count , ::rux::uint16* utf16_ptr , size_t utf16_ptr_count , size_t& result_bytes );
		static void utf32_to_utf16( const char* utf8_ptr , size_t utf8_ptr_count , ::rux::uint16* utf16_ptr , size_t utf16_ptr_count );

		static void utf8_to_ansi( const char* utf8_ptr , size_t utf8_ptr_count , char* cp1251_ptr , size_t cp1251_ptr_count , size_t& result_bytes );
	public:
		static void utf8_to_utf32( const char* utf8_ptr , size_t utf8_ptr_count , char* utf32_ptr , size_t utf32_ptr_count );
	private:
	public:
		static ::rux::uint16* ansi_to_utf16( const char* cp1251_ptr , size_t cp1251_ptr_count , size_t& result_bytes );
	private:
	public:
		static void ansi_to_utf16( const char* cp1251_ptr , size_t cp1251_ptr_count , ::rux::uint16* utf16_ptr , size_t utf16_ptr_count , size_t& result_bytes );
	private:
		static void ansi_to_utf8( const char* cp1251_ptr , size_t cp1251_ptr_count , char* utf8_ptr , size_t utf8_ptr_count , size_t& result_bytes );
		static void ansi_to_utf32( const char* cp1251_ptr , size_t cp1251_ptr_count , char* utf32_ptr , size_t utf32_ptr_count );
		static char* ansi_to_utf8( const char* cp1251_ptr , size_t cp1251_ptr_count , size_t& res_length );
		static void utf32_to_utf8( const char* utf32_ptr , size_t utf32_ptr_count , char* utf8_ptr , size_t utf8_ptr_count );	
		static char* utf16_to_utf8( const ::rux::uint16* utf16_ptr , size_t utf16_ptr_count , size_t& res_length );
		static void utf16_to_utf8( const ::rux::uint16* utf16_ptr , size_t utf16_ptr_count , char* utf8_ptr , size_t utf8_ptr_count , size_t& res_length );
		static void utf16_to_utf32( const ::rux::uint16* utf16_ptr , size_t utf16_ptr_count , char* utf32_ptr , size_t utf32_ptr_count );
		static void utf16_to_ansi( const ::rux::uint16* utf16_ptr , size_t utf16_ptr_count , char* cp1251_ptr , size_t cp1251_ptr_count , size_t& result_bytes );	
		static size_t utf8_symbol_index_to_byte_index( const char* utf8_ptr , size_t symbol_index );
		static size_t utf8_symbol_length_to_byte_length( const char* utf8_ptr , size_t symbol_index , size_t symbol_length );	
	public:
		static XEnumCodePage get_global_code_page( void );
		static rux_inline size_t utf8_symbol_length( const char* utf8_ptr , size_t byte_index )		
		{
			size_t count = 0;
			if( utf8_ptr )
			{		
				while( utf8_ptr[ byte_index ] != 0 )
				{
					if( ( utf8_ptr[ byte_index ] & 0xc0 ) != 0x80 )
					{
						if( count > 0 )
							break;
					}
					count++;
					byte_index++;
				}
			}
			return count;
		};
		static rux_inline size_t utf8_symbol_length( const char* utf8_ptr , size_t utf8_ptr_size , size_t byte_index )		
		{
			size_t count = 0;
			if( utf8_ptr )
			{		
				for( ; ; )
				{
					if( ( utf8_ptr[ byte_index ] & 0xc0 ) != 0x80 )
					{
						if( count > 0 )
							break;
					}
					count++;
					byte_index++;
					if( utf8_ptr_size == 0 )
						break;
					utf8_ptr_size--;
				}
			}
			return count;
		};
	private:
		static void array_right_shift( char* array_ptr , size_t index , size_t shift_count , size_t array_count );
		static void array_left_shift( char* array_ptr , size_t index , size_t shift_count , size_t array_count );
	};
	class String : public XGCRef
	{
		friend class XString;
		friend class XPath;
		friend class ::rux::data::XJsonSerializer;
		declare_rux_base_internal_methods();
	private:
		char* _str;
	public:	
		size_t _step;
		size_t _size;
		size_t _count;
		XEnumCodePage _code_page;
		size_t _utf8_length;	
	public:
		explicit String( const char* variable_name = 0 , const char* __file__ = 0 , ::rux::int32 __line__ = 0 );
		~String();
		char* str( void ) const;
		void CopyToArrayAsCurrentCodePage( char destination[] , size_t destination_size ) const; 
		virtual ::rux::String* ObjectToString( void );
		size_t Length() const;		
	#ifdef __WINDOWS__
		BSTR Convert( void ) const;
	#endif
		char get_AnsiChar( size_t index );
		void ConvertToUTF16( ::rux::alloc_t alloc , ::rux::uint16** string_ptr_ptr ) const;
		::rux::int32 ToInt32( void ) const;
		size_t Size() const;
		void RemoveRange( size_t index , size_t count );
		void InsertRange( size_t index , const ::rux::XString& string );
		::rux::String& ConvertToAnsi( void ) const;
		::rux::String& ConvertToCurrentAnsiPage( void ) const;
		bool Equals( const char* text );
		bool Equals( const char* text , size_t length ) const;
		bool Equals( const String& string );
		bool Equals( const ::rux::XString& string );
		
		String& Trim( void ) const;
		double ToDouble( void );
		::rux::Array<XString>& Split( const ::rux::XArray<XString>& separators , EStringSplitOptions string_split_options = EStringSplitOptions_RemoveEmptyEntries ) const;	
		Array<XString>& Split( const char separator ) const;
		Array<XString>& Split( const char* separator , EStringSplitOptions string_split_options = EStringSplitOptions_RemoveEmptyEntries ) const;
		::rux::String& ConvertToUTF8( void ) const;
		::rux::String& ConvertToCodePage( XEnumCodePage code_page ) const;
		::rux::uint32 get_UTF32Char( size_t index ) const;
		void RemoveAt( size_t index );
		size_t get_step( void ) const;
		::rux::uint32 ToUInt32( void );
		::rux::int64 ToInt64( void );
		::rux::uint64 ToUInt64( void );
		void InsertRange( size_t index , const char* string_ptr , size_t count );
		void Insert( size_t index , const char letter );
		String& Substring( size_t start_index , size_t length = SIZE_MAX ) const;
		
		::rux::uint32 get_UTF8Char( size_t index ) const;
		::rux::String& operator +=( const ::rux::XString& string);	
		::rux::String& operator +=(const char* text);		
		::rux::String& operator +=(const wchar_t* string_ptr);	
		::rux::String& operator +=( const String& string );	
		void SwapHere( size_t index1 , size_t index2 );
		friend String& operator +( const ::rux::XString& string, const ::rux::XString& string2);	
		friend String& operator +( const ::rux::XString& string, const char* text);
		friend String& operator +( const ::rux::XString& string, const wchar_t* string_ptr);
		friend String& operator +( const ::rux::XString& string, const String& string2);	
		friend String& operator +(const char* text, const ::rux::XString& string);
		friend String& operator +(const wchar_t* string_ptr, const ::rux::XString& string);
		friend String& operator +(const char* text , const String& string);
		friend String& operator +(const wchar_t* string_ptr , const String& string);
		friend String& operator +( const String& string,const char* text);
		friend String& operator +( const String& string,const wchar_t* text_ptr);
		friend String& operator +( const String& string, const ::rux::XString& string2);
		friend String& operator +( const String& string, const String& string2);	

		friend bool operator ==(const char* text_ptr, const String& string2);
		friend bool operator ==(const char* text_ptr, const ::rux::XString& string2);
		friend bool operator !=(const char* text_ptr, const String& string2);
		friend bool operator !=(const char* text_ptr, const ::rux::XString& string2);
	private:
	#ifdef __UNIX__
		static void get_all_locales( char locales[ 64 ][ 64 ] , size_t& locales_count );
	#endif
		virtual bool Equals( XGCRef* gcref );
		void ctor_set( const char* text_ptr , size_t start_index , size_t length , XEnumCodePage code_page , size_t step );
		void ctor_set( size_t capacity );
		void ctor_set( const String& string , const char* str );
		void ctor_set( const String& string , const wchar_t* str );
		void ctor_set( const wchar_t* text_ptr , size_t start_index , size_t length , size_t step );
	public:
		void set( const char* string_ptr , XEnumCodePage code_page , ::rux::uint8 copy_string );
		::rux::String& operator =(const wchar_t* string_ptr);
		::rux::String& operator =(const char* string_ptr);
		bool operator ==( const ::rux::XString& string ) const;
		bool operator ==( const char* text_ptr ) const;
		bool operator ==( const XString* string_ptr ) const;
		bool operator ==( const String& string ) const;
		::rux::String& operator =(const String& string);
		bool operator >( const ::rux::String& string ) const;
		bool operator !=(const ::rux::String& string) const;
		bool operator !=(const ::rux::XString& string) const;
		bool operator !=(const char* text_ptr) const;
		bool operator !=( const XString* string_ptr ) const;
		void InsertRange( size_t index , const ::rux::String& string );
		void InsertRange( size_t index , const wchar_t* string_ptr , size_t count );


		void Clear();
		::rux::int32 IndexOfAny( const ::rux::XArray< ::rux::XChar>& char_array , size_t index = 0 );
		::rux::int32 IndexOfAny( const ::rux::XArray<XString>& string_array , size_t index = 0 );
		::rux::int32 IndexOfAny( const ::rux::XArray< ::rux::XChar>& char_array , size_t index , size_t& array_index );
		::rux::int32 IndexOfAny( const ::rux::XArray<XString>& string_array , size_t index , size_t& array_index );
		
		::rux::int32 IndexOf( const char letter , size_t index = 0 );	
		::rux::int32 IndexOf( const char* text_ptr , size_t index = 0 );
		::rux::int32 IndexOf( const ::rux::XString& string , size_t index = 0 ) const;
		::rux::int32 IndexOf( const String& string , size_t index = 0 ) const;
		::rux::int32 IndexOf( const char letter , size_t index = 0 ) const;	
		::rux::int32 IndexOf( const char* text_ptr , size_t index = 0 ) const;

		::rux::int32 LastIndexOf( const char* letter , size_t index = SIZE_MAX ) const;
		::rux::int32 LastIndexOf( const String& string , size_t index = SIZE_MAX ) const;
		::rux::int32 LastIndexOf( const char letter , size_t index = SIZE_MAX ) const;
		::rux::int32 LastIndexOf( const ::rux::XString& string , size_t index = SIZE_MAX ) const;		

		::rux::String& ReplaceAll( const char old_letter , const char new_letter , size_t index = 0 ) const;
		::rux::String& ReplaceAll( const char* old_text , const char* new_text , size_t index = 0 ) const;
		::rux::String& ReplaceAll( const ::rux::XString& old_string , const ::rux::XString& new_string , size_t index = 0 ) const;
		::rux::String& ReplaceAll( const ::rux::XString& old_string , const char* new_text , size_t index = 0 ) const;
		::rux::String& ReplaceAll( const ::rux::XString& old_string , const String& new_string , size_t index = 0 ) const;
		::rux::String& ReplaceAll( const String& OldString , const ::rux::XString& NewString , size_t index = 0 ) const;
		::rux::String& ReplaceAll( const String& old_string , const char* new_string , size_t index = 0 ) const;
		::rux::String& ReplaceAll( const char* old_text , const ::rux::XString& new_string , size_t index = 0 ) const;
		::rux::String& ReplaceAll( const char* old_text , const String& new_string , size_t index = 0 ) const;
		::rux::String& ReplaceAll( const ::rux::String& old_string , const ::rux::String& new_string , size_t index = 0 ) const;

		::rux::String& ReplaceFirst( const char old_letter , const char new_letter , size_t index = 0 ) const;
		::rux::String& ReplaceFirst( const char* old_text , const char* new_text , size_t index = 0 ) const;
		::rux::String& ReplaceFirst( const ::rux::XString& old_string , const ::rux::XString& new_string , size_t index = 0 ) const;
		::rux::String& ReplaceFirst( const ::rux::XString& old_string , const char* new_text , size_t index = 0 ) const;
		::rux::String& ReplaceFirst( const ::rux::XString& old_string , const String& new_string , size_t index = 0 ) const;
		::rux::String& ReplaceFirst( const String& old_string , const ::rux::XString& new_string , size_t index = 0 ) const;
		::rux::String& ReplaceFirst( const String& old_string , const char* new_string , size_t index = 0 ) const;
		::rux::String& ReplaceFirst( const char* old_text , const ::rux::XString& new_string , size_t index = 0 ) const;
		::rux::String& ReplaceFirst( const char* old_text , const String& new_string , size_t index = 0 ) const;
		::rux::String& ReplaceFirst( const ::rux::String& old_string , const ::rux::String& new_string , size_t index = 0 ) const;

		::rux::String& ReplaceLast( const char old_letter , const char new_letter , size_t index = SIZE_MAX ) const;
		::rux::String& ReplaceLast( const char* old_text , const char* new_text , size_t index = SIZE_MAX ) const;
		::rux::String& ReplaceLast( const ::rux::XString& old_string , const ::rux::XString& new_string , size_t index = SIZE_MAX ) const;
		::rux::String& ReplaceLast( const ::rux::XString& old_string , const char* new_text , size_t index = SIZE_MAX ) const;
		::rux::String& ReplaceLast( const ::rux::XString& old_string , const String& new_string , size_t index = SIZE_MAX ) const;
		::rux::String& ReplaceLast( const String& old_string , const ::rux::XString& new_string , size_t index = SIZE_MAX ) const;
		::rux::String& ReplaceLast( const String& OldString , const char* NewString , size_t index = SIZE_MAX ) const;
		::rux::String& ReplaceLast( const char* old_text , const ::rux::XString& new_string , size_t index = SIZE_MAX ) const;
		::rux::String& ReplaceLast( const char* old_text , const String& new_string , size_t index = SIZE_MAX ) const;
		::rux::String& ReplaceLast( const ::rux::String& old_string , const ::rux::String& new_string , size_t index = SIZE_MAX ) const;

		::rux::String& ToLower( size_t start_index = 0 , size_t length = SIZE_MAX ) const;
		::rux::String& ToUpper( size_t start_index = 0 , size_t length = SIZE_MAX ) const;
		::rux::uint16 get_UTF16Char( size_t index ) const;
		char get_AnsiChar( size_t index ) const;
		bool IsInt32( void );
		bool IsDouble( void );
		::rux::String& ConvertToUTF16( void ) const;		
		size_t get_AllocatedMemory( void );
		void IncreaseMemory( size_t size );
		void Shrink( void );
		void attach( const char* str , size_t str_size , size_t str_length , XEnumCodePage code_page );
		char* detach( void );
	};
};
namespace rux
{
#define utf8_symbol_length_macros( utf8_ptr , index , count ) {\
		size_t byte_index = index;\
		count = 0;\
		if( utf8_ptr )\
		{\
			while( utf8_ptr[ byte_index ] != 0 )\
			{\
				if( ( utf8_ptr[ byte_index ] & 0xc0 ) != 0x80 )\
				{\
					if( count > 0 )\
						break;\
				}\
				count++;\
				byte_index++;\
			}\
		}\
	}
};
#endif
