#include <gtest/gtest.h>
#include <xan_xml.h>
#include <algorithm>
#define rapidxml_enable 0
#if rapidxml_enable
#include <rapidxml.hpp>
#endif
#include <map>
#include <locale>
class XmlTest : public ::testing::Test 
{
};
TEST_F( XmlTest , Deserialize ) 
{
	::std::string xml0( "<doc><!-- comment -->\n\
						H&#x65;llo, &#109;y friend<!-- comment --> \
						\nTab\
  <!-- comment -->\
<?PI0?><?PI1 processing instruction?>\
 try to explain\
 <t>&#109;&#x65;      </t>\
</doc>" );
#if rapidxml_enable
	::std::vector<char> xml_copy(xml0.begin(), xml0.end());
	xml_copy.push_back('\0');
	::rapidxml::xml_document<> doc;
	doc.parse<0>( &xml_copy[0] );

	::rapidxml::xml_node<> *node = doc.first_node("doc");

	doc.clear();
#else
	::rux::data::xml::document xml;
	xml.deserialize( xml0.c_str());
	xml.free();
#endif
	size_t len = ::rux::utils::strlen( "" );
	len = ::rux::utils::strlen( "wow" );
	::rux::int32 val_int32 = 0;
	::rux::utils::hex_string_to_number< ::rux::int32 >( "0xff2367AF" , SIZE_MAX , val_int32 );

	::rux::utils::hex_string_to_number< ::rux::int32 >( "0xff2367A;" );
		

	const char* utf32 = "0x00 , 0x01 , 0x02 , 0x03 , 0x04 , 0x05 , 0x06 , 0x07 , 0x08 , 0x09 , 0x0a , 0x0b , 0x0c , 0x0d , 0x0e , 0x0f , \
		0x10 , 0x11 , 0x12 , 0x13 , 0x14 , 0x15 , 0x16 , 0x17 , 0x18 , 0x19 , 0x1a , 0x1b , 0x1c , 0x1d , 0x1e , 0x1f , \
		0x20 , 0x21 , 0x22 , 0x23 , 0x24 , 0x25 , 0x26 , 0x27 , 0x28 , 0x29 , 0x2a , 0x2b , 0x2c , 0x2d , 0x2e , 0x2f , \
		0x30 , 0x31 , 0x32 , 0x33 , 0x34 , 0x35 , 0x36 , 0x37 , 0x38 , 0x39 , 0x3a , 0x3b , 0x3c , 0x3d , 0x3e , 0x3f , \
		0x40 , 0x41 , 0x42 , 0x43 , 0x44 , 0x45 , 0x46 , 0x47 , 0x48 , 0x49 , 0x4a , 0x4b , 0x4c , 0x4d , 0x4e , 0x4f , \
		0x50 , 0x51 , 0x52 , 0x53 , 0x54 , 0x55 , 0x56 , 0x57 , 0x58 , 0x59 , 0x5a , 0x5b , 0x5c , 0x5d , 0x5e , 0x5f , \
		0x60 , 0x61 , 0x62 , 0x63 , 0x64 , 0x65 , 0x66 , 0x67 , 0x68 , 0x69 , 0x6a , 0x6b , 0x6c , 0x6d , 0x6e , 0x6f , \
		0x70 , 0x71 , 0x72 , 0x73 , 0x74 , 0x75 , 0x76 , 0x77 , 0x78 , 0x79 , 0x7a , 0x7b , 0x7c , 0x7d , 0x7e , 0x7f , \
		0x0402 , 0x0403 , 0x201a , 0x0453 , 0x201e , 0x2026 , 0x2020 , 0x2021 , 0x20ac , 0x2030 , 0x0409 , 0x2039 , 0x040a , 0x040c , 0x040b , 0x040f , \
		0x0452 , 0x2018 , 0x2019 , 0x201c , 0x201d , 0x2022 , 0x2013 , 0x2014 , 0x98 , 0x2122 , 0x0459 , 0x203a , 0x045a , 0x045c , 0x045b , 0x045f , \
		0xa0 , 0x040e , 0x045e , 0x0408 , 0xa4 , 0x0490 , 0xa6 , 0xa7 , 0x0401 , 0xa9 , 0x0404 , 0xab , 0xac , 0xad , 0xae , 0x0407 , \
		0xb0 , 0xb1 , 0x0406 , 0x0456 , 0x0491 , 0xb5 , 0xb6 , 0xb7 , 0x0451 , 0x2116 , 0x0454 , 0xbb , 0x0458 , 0x0405 , 0x0455 , 0x0457 , \
		0x0410 , 0x0411 , 0x0412 , 0x0413 , 0x0414 , 0x0415 , 0x0416 , 0x0417 , 0x0418 , 0x0419 , 0x041a , 0x041b , 0x041c , 0x041d , 0x041e , 0x041f , \
		0x0420 , 0x0421 , 0x0422 , 0x0423 , 0x0424 , 0x0425 , 0x0426 , 0x0427 , 0x0428 , 0x0429 , 0x042a , 0x042b , 0x042c , 0x042d , 0x042e , 0x042f , \
		0x0430 , 0x0431 , 0x0432 , 0x0433 , 0x0434 , 0x0435 , 0x0436 , 0x0437 , 0x0438 , 0x0439 , 0x043a , 0x043b , 0x043c , 0x043d , 0x043e , 0x043f , \
		0x0440 , 0x0441 , 0x0442 , 0x0443 , 0x0444 , 0x0445 , 0x0446 , 0x0447 , 0x0448 , 0x0449 , 0x044a , 0x044b , 0x044c , 0x044d , 0x044e , 0x044f";

	/*const char* utf8 = "0x00 , 0x01 , 0x02 , 0x03 , 0x04 , 0x05 , 0x06 , 0x07 , 0x08 , 0x09 , 0x0a , 0x0b , 0x0c , 0x0d , 0x0e , 0x0f , \
		0x10 , 0x11 , 0x12 , 0x13 , 0x14 , 0x15 , 0x16 , 0x17 , 0x18 , 0x19 , 0x1a , 0x1b , 0x1c , 0x1d , 0x1e , 0x1f , \
		0x20 , 0x21 , 0x22 , 0x23 , 0x24 , 0x25 , 0x26 , 0x27 , 0x28 , 0x29 , 0x2a , 0x2b , 0x2c , 0x2d , 0x2e , 0x2f , \
		0x30 , 0x31 , 0x32 , 0x33 , 0x34 , 0x35 , 0x36 , 0x37 , 0x38 , 0x39 , 0x3a , 0x3b , 0x3c , 0x3d , 0x3e , 0x3f , \
		0x40 , 0x41 , 0x42 , 0x43 , 0x44 , 0x45 , 0x46 , 0x47 , 0x48 , 0x49 , 0x4a , 0x4b , 0x4c , 0x4d , 0x4e , 0x4f , \
		0x50 , 0x51 , 0x52 , 0x53 , 0x54 , 0x55 , 0x56 , 0x57 , 0x58 , 0x59 , 0x5a , 0x5b , 0x5c , 0x5d , 0x5e , 0x5f , \
		0x60 , 0x61 , 0x62 , 0x63 , 0x64 , 0x65 , 0x66 , 0x67 , 0x68 , 0x69 , 0x6a , 0x6b , 0x6c , 0x6d , 0x6e , 0x6f , \
		0x70 , 0x71 , 0x72 , 0x73 , 0x74 , 0x75 , 0x76 , 0x77 , 0x78 , 0x79 , 0x7a , 0x7b , 0x7c , 0x7d , 0x7e , 0x7f , \
		0x82d0 , 0x83d0 , 0x9a80e2 , 0x9a93d1 , 0x9e80e2 , 0xa680e2 , 0xa080e2 , 0xa180e2 , 0xac82e2 , 0xb080e2 , 0xb089d0 , 0xb980e2 , 0xb98ad0 , 0xb98cd0 , 0xb98bd0 , 0xb98fd0 , \
		0xb992d1 , 0x9880e2 , 0x9980e2 , 0x9c80e2 , 0x9d80e2 , 0xa280e2 , 0x9380e2 , 0x9480e2 , 0x9498c2 , 0xa284e2 , 0xa299d1 , 0xba80e2 , 0xba9ad1 , 0xba9cd1 , 0xba9bd1 , 0xba9fd1 , \
		0xbaa0c2 , 0xba8ed0 , 0xba9ed1 , 0xba88d0 , 0xbaa4c2 , 0xba90d2 , 0xbaa6c2 , 0xbaa7c2 , 0xba81d0 , 0xbaa9c2 , 0xba84d0 , 0xbaabc2 , 0xbaacc2 , 0xbaadc2 , 0xbaaec2 , 0xba87d0 , \
		0xbab0c2 , 0xbab1c2 , 0xba86d0 , 0xba96d1 , 0xba91d2 , 0xbab5c2 , 0xbab6c2 , 0xbab7c2 , 0xba91d1 , 0x9684e2 , 0x9694d1 , 0x96bbc2 , 0x9698d1 , 0x9685d0 , 0x9695d1 , 0x9697d1 , \
		0x9690d0 , 0x9691d0 , 0x9692d0 , 0x9693d0 , 0x9694d0 , 0x9695d0 , 0x9696d0 , 0x9697d0 , 0x9698d0 , 0x9699d0 , 0x969ad0 , 0x969bd0 , 0x969cd0 , 0x969dd0 , 0x969ed0 , 0x969fd0 , \
		0x96a0d0 , 0x96a1d0 , 0x96a2d0 , 0x96a3d0 , 0x96a4d0 , 0x96a5d0 , 0x96a6d0 , 0x96a7d0 , 0x96a8d0 , 0x96a9d0 , 0x96aad0 , 0x96abd0 , 0x96acd0 , 0x96add0 , 0x96aed0 , 0x96afd0 , \
		0x96b0d0 , 0x96b1d0 , 0x96b2d0 , 0x96b3d0 , 0x96b4d0 , 0x96b5d0 , 0x96b6d0 , 0x96b7d0 , 0x96b8d0 , 0x96b9d0 , 0x96bad0 , 0x96bbd0 , 0x96bcd0 , 0x96bdd0 , 0x96bed0 , 0x96bfd0 , \
		0x9680d1 , 0x9681d1 , 0x9682d1 , 0x9683d1 , 0x9684d1 , 0x9685d1 , 0x9686d1 , 0x9687d1 , 0x9688d1 , 0x9689d1 , 0x968ad1 , 0x968bd1 , 0x968cd1 , 0x968dd1 , 0x968ed1 , 0x968fd1";
*/
	::rux::uint32 value = 0;
	::std::map< ::rux::uint32 , ::rux::uint32 > map;
	//const ::rux::byte* ptr = (::rux::byte*)utf8;
	const ::rux::byte* ptr = (::rux::byte*)utf32;
	while( *ptr != 0 )
	{
		if( memcmp( ptr , "0x" , 2 ) == 0 )
		{
			::rux::uint32 key = ::rux::utils::hex_string_to_number< ::rux::uint32 >( ptr );
			if( key > 127 )
			{
				map[ key ] = value;
			}
			value++;
		}
		else
			ptr++;
	}
	std::map< ::rux::uint32 , ::rux::uint32 >::iterator map_iter;

	std::string str = "const ::rux::byte _utf32_to_cp1251_page_from_8211_to_8483[] = \n{\n";
	for( ::rux::uint32 i = 8211 ; i < 8483 ; i++ )
	{
		map_iter = map.find( i );
		if( map_iter != map.end() )
		{
			char hex[ 32 ] = {0};
			::rux::utils::number_to_hex_string< ::rux::uint32 >( hex , 32 , map_iter->second );
			str += hex;
			str += " , ";
		}
		else
		{
			str += "0x00 , ";
		}
	}
	str += "\n};";

	/*if( *source > 151 && *source < 188 )
	{
	}
	else if( *source > 1024 && *source < 1170 )
	{
	}
	else if( *source > 8210 && *source < 8483 )
	{
	}*/
	
	/*::std::map< ::rux::uint32 , ::rux::uint32 > map_256_from_9863377;
	::rux::uint32 from = 9863377;
	for( ; ; )
	{
		map_iter = map.find( from );
		if( map_iter != map.end() )
		{
			map_256_from_9863377[ map_iter->first ] = map_iter->second;
			map.erase( map_iter );

			from += 256;
		}
		else
			break;
	}

	::std::map< ::rux::uint32 , ::rux::uint32 > map_256_from_9867472;
	from = 9867472;
	for( ; ; )
	{
		map_iter = map.find( from );
		if( map_iter != map.end() )
		{
			map_256_from_9867472[ map_iter->first ] = map_iter->second;
			map.erase( map_iter );

			from += 256;
		}
		else
			break;
	}

	::std::map< ::rux::uint32 , ::rux::uint32 > map_256_from_12233666;
	from = 12233666;
	for( ; ; )
	{
		map_iter = map.find( from );
		if( map_iter != map.end() )
		{
			map_256_from_12233666[ map_iter->first ] = map_iter->second;
			map.erase( map_iter );

			from += 256;
		}
		else
			break;
	}


	::std::map< ::rux::uint32 , ::rux::uint32 > map_256_from_12236226;
	from = 12236226;
	for( ; ; )
	{
		map_iter = map.find( from );
		if( map_iter != map.end() )
		{
			map_256_from_12236226[ map_iter->first ] = map_iter->second;
			map.erase( map_iter );

			from += 256;
		}
		else
			break;
	}

	


	
	for( map_iter = map.begin() ; map_iter != map.end() ; map_iter++ )
	{
		map_iter->first;
	}*/

	/*const char* to_allow = "#x9 | #xA | #xD | [#x20-#xD7FF] | [#xE000-#xFFFD] | [#x10000-#x10FFFF]";
	const char* ptr = to_allow;
	::std::vector< ::rux::uint32 > allowed;
	while( *ptr != 0 )
	{
		switch( *ptr )
		{
		case '"':
			{
				allowed.push_back( (::rux::uint32)*++ptr );
				ptr += 2;
				break;
			}
		case '[':
			{
				switch( *++ptr )
				{
				case '#':
					{
						if( *++ptr == 'x' )
						{
							const char* hex = ++ptr;
							while( *++ptr != '-' ){};
							::rux::uint32 first = 0;
							::rux::utils::hex_string_to_number( hex , ptr - hex , first );
							ptr += 3;
							hex = ptr;
							while( *++ptr != ']' ){};
							::rux::uint32 last = 0;
							::rux::utils::hex_string_to_number( hex , ptr - hex , last );
							for( ::rux::uint32 cur = first ; cur <= last ; cur++ )
								allowed.push_back( cur );
						}
						break;
					}
				default:
					{
						::rux::uint32 first = (::rux::uint32)*ptr;
						while( *++ptr != '-' ){};
						::rux::uint32 last = (::rux::uint32)*++ptr;
						while( *++ptr != ']' ){};
						ptr++;
						for( ::rux::uint32 cur = first ; cur <= last ; cur++ )
							allowed.push_back( cur );
						break;
					}
				}
				break;
			}
		case '#':
			{
				if( *++ptr == 'x' )
				{
					const char* hex = ++ptr;
					while( *++ptr != ' ' ){};
					::rux::uint32 first = 0;
					::rux::utils::hex_string_to_number( hex , ptr - hex , first );
					allowed.push_back( first );
				}
				break;
			}
		default:
			{
				ptr++;
				break;
			}
		}
	}
	const char* to_deny = "[#x7F-#x84], [#x86-#x9F], [#xFDD0-#xFDEF],\
						  [#x1FFFE-#x1FFFF], [#x2FFFE-#x2FFFF], [#x3FFFE-#x3FFFF],\
						  [#x4FFFE-#x4FFFF], [#x5FFFE-#x5FFFF], [#x6FFFE-#x6FFFF],\
						  [#x7FFFE-#x7FFFF], [#x8FFFE-#x8FFFF], [#x9FFFE-#x9FFFF],\
						  [#xAFFFE-#xAFFFF], [#xBFFFE-#xBFFFF], [#xCFFFE-#xCFFFF],\
						  [#xDFFFE-#xDFFFF], [#xEFFFE-#xEFFFF], [#xFFFFE-#xFFFFF],\
						  [#x10FFFE-#x10FFFF]";
	ptr = to_deny;
	::std::vector< ::rux::uint32 > denied;
	while( *ptr != 0 )
	{
		switch( *ptr )
		{
		case '"':
			{
				denied.push_back( (::rux::uint32)*++ptr );
				ptr += 2;
				break;
			}
		case '[':
			{
				switch( *++ptr )
				{
				case '#':
					{
						if( *++ptr == 'x' )
						{
							const char* hex = ++ptr;
							while( *++ptr != '-' ){};
							::rux::uint32 first = 0;
							::rux::utils::hex_string_to_number( hex , ptr - hex , first );
							ptr += 3;
							hex = ptr;
							while( *++ptr != ']' ){};
							::rux::uint32 last = 0;
							::rux::utils::hex_string_to_number( hex , ptr - hex , last );
							for( ::rux::uint32 cur = first ; cur <= last ; cur++ )
								denied.push_back( cur );
						}
						break;
					}
				default:
					{
						::rux::uint32 first = (::rux::uint32)*ptr;
						while( *++ptr != '-' ){};
						::rux::uint32 last = (::rux::uint32)*++ptr;
						while( *++ptr != ']' ){};
						ptr++;
						for( ::rux::uint32 cur = first ; cur <= last ; cur++ )
							denied.push_back( cur );
						break;
					}
				}
				break;
			}
		case '#':
			{
				if( *++ptr == 'x' )
				{
					const char* hex = ++ptr;
					while( *++ptr != ' ' ){};
					::rux::uint32 first = 0;
					::rux::utils::hex_string_to_number( hex , ptr - hex , first );
					denied.push_back( first );
				}
				break;
			}
		default:
			{
				ptr++;
				break;
			}
		}
	}

	::std::sort( denied.begin() , denied.end() );
	::std::sort( allowed.begin() , allowed.end() );
	FILE* f = fopen( "C:\\files\\temp.txt" , "wt" );
	if( f )
	{
		size_t index1 = 0;
		::std::string res( "::rux::byte your_variable_name[] = \n{\n\t" );
		for( ::rux::uint32 index = 0 ; allowed.size() ; index++ )
		{
			std::vector< ::rux::uint32 >::iterator found = std::find( allowed.begin(), allowed.end(), index );
			if( found != allowed.end() )
			{
				allowed.erase( found );

				found = std::find( denied.begin(), denied.end(), index );
				if( found != denied.end() )
				{
					denied.erase( found );
					res += "0";
				}
				else
					res += "1";
			}
			else
				res += "0";
			if( index + 1 < UINT_MAX )
				res += ", ";
			if( index1 < 128 )
				index1++;
			else
			{
				res += "\n\t";
				index1 = 0;
			}
		}
		fprintf( f , "%s\n}" , res.c_str() );
		fclose( f );
	}/**/
};
static ::rux::byte* load_from_file( const char* filename , size_t& filename_size )
{
	FILE* file = fopen( filename , "rb" );
	if( file )
	{
		filename_size = 0;
		if( ::rux::utils::io::fseek64( file , 0L , SEEK_END ) == 0 )
		{
			::rux::int64 res = ::rux::utils::io::ftell64( file );
			if( res >= 0 )
				filename_size = res;
			::rux::utils::io::fseek64( file , 0 , SEEK_SET );
		}
		::rux::byte* buf = (::rux::byte*)malloc( filename_size );
		size_t readen = 0 , read_size = 0;
		if( fread( buf , sizeof( char ) , 3 , file ) == 3 )
		{
			if( memcmp( buf , ::rux::utils::string::utf8::byte_order_mark , 3 ) == 0 )
			{
				filename_size -= 3;
			}
			else
			{
				readen = 3;
			}
		}

		
		while( ( read_size = fread( &buf[ readen ] , sizeof( char ) , filename_size , file ) ) > 0 )
		{
			readen += read_size;
			if( readen == filename_size )
			{
				fclose( file );
				return buf;
			}
		}
		filename_size = 0;
		fclose( file );
		return 0;
	}
	else
	{
		filename_size = 0;
		return 0;
	}
};
static bool save_to_file( const char* filename , ::rux::byte* buf , size_t buf_size )
{
	FILE* file = fopen( filename , "wb" );
	if( file )
	{
		size_t write_size = 0 , written = 0;
		while( ( write_size = fwrite( &buf[ written ] , sizeof( char ) , buf_size , file ) ) > 0 )
		{
			written += write_size;
			if( written == buf_size )
			{
				fclose( file );
				return true;
			}
		}
		fclose( file );
		return false;
	}
	return false;
};
TEST_F( XmlTest , DeserializeSpeed )
{
//	::std::string xml0;
	size_t xml_length = 0;
	::rux::byte* xml = load_from_file( "../../addons/xml/xmltest0.xml" , xml_length );
	
	size_t utf16_allocated_size = xml_length * 2;
	char* utf16 = (char*)malloc( utf16_allocated_size );

	size_t utf32_allocated_size = xml_length * 4;
	char* utf32 = (char*)malloc( utf32_allocated_size );


	//size_t utf8_allocated_size = xml_length + 1;
	//char* utf8 = (char*)malloc( utf8_allocated_size );

	size_t utf16_res_allocated_size = xml_length * 2 , utf16_res_bytes = 0;
	char* utf16_res = (char*)malloc( utf16_res_allocated_size );
	::rux::utils::string::utf8::to_utf16( (char*)xml , xml_length , utf16_res , utf16_res_allocated_size , utf16_res_bytes );
	
	size_t utf32_res_allocated_size = xml_length * 4 , utf32_res_bytes = 0;
	char* utf32_res = (char*)malloc( utf32_res_allocated_size );
	::rux::utils::string::utf8::to_utf32( (char*)xml , xml_length , utf32_res , utf32_res_allocated_size , utf32_res_bytes );


	size_t cp1251_test0_length = 0;
	::rux::byte* cp1251_test0 = load_from_file( "../../addons/tests/string/encoding/test0.cp1251" , cp1251_test0_length );

	size_t utf8_test0_length = 0;
	::rux::byte* utf8_test0 = load_from_file( "../../addons/tests/string/encoding/test0.utf8" , utf8_test0_length );

	size_t utf16_test0_length = 0;
	::rux::byte* utf16_test0 = load_from_file( "../../addons/tests/string/encoding/test0.utf16" , utf16_test0_length );

	size_t utf32_test0_length = 0;
	::rux::byte* utf32_test0 = load_from_file( "../../addons/tests/string/encoding/test0.utf32" , utf32_test0_length );
#define CALC_MAPS 1
#if CALC_MAPS
	std::locale loc("");

	
	
	wchar_t utf16wchar = 0;
	::rux::uint32 utf8char = 0 , utf8lower = 0 , utf32lower = 0 , utf32upper = 0;
	::rux::byte* bytes = (::rux::byte*)&utf8char;
	size_t utf16bytes = 0 , count = 0 , chbytes = 0 , utf8size = 0 , utf32size = 0;
	::std::map< ::rux::uint32 , ::rux::uint32 > map;
	//std::string cp1251_tolower = "::rux::byte _toupper[] = {\n\t";
	//for( ::rux::byte ch = 0 ; ch < 256 ; ch++ )
	//{
	//	char hex[ 32 ] = {0};
	//	//::rux::byte chlower = std::toupper( ch , loc );
	//	if( std::isalnum( ch , loc ) )
	//	{			
	//		cp1251_tolower += "1";
	//		//char* end = ::rux::utils::number_to_hex_string< ::rux::uint32 >( hex , 32 , chlower );
	//		//*end = 0;
	//	}
	//	else
	//	{			
	//		cp1251_tolower += "0";
	//		//char* end = ::rux::utils::number_to_hex_string< ::rux::uint32 >( hex , 32 , ch );
	//		//*end = 0;
	//	}
	//	//cp1251_tolower += hex;
	//	if( ch < 255 )
	//		cp1251_tolower += " , ";
	//	else
	//		break;
	//}
	//cp1251_tolower += "\n};";


	/*map.clear();
	std::map< ::rux::uint32 , ::rux::uint32 >::iterator map_iter;
	for( ::rux::uint32 utf8upper = UINT32_MAX ; utf8upper > 0 ; utf8upper-- )
	{
		const ::rux::byte* ptr = (::rux::byte*)&utf8upper;
		if( ::rux::utils::get_byte( utf8upper , 3 ) != 0 )
			chbytes = 4;
		else if( ::rux::utils::get_byte( utf8upper , 2 ) != 0 )
			chbytes = 3;
		else if( ::rux::utils::get_byte( utf8upper , 1 ) != 0 )
			chbytes = 2;
		else
			chbytes = 1;
		if( ::rux::utils::string::utf8::validate_character_with_character_size( ptr , &utf8size ) && chbytes == utf8size )
		{
			utf16wchar = 0;
			if( ( sizeof( wchar_t ) == 2 && ::rux::utils::string::utf8::to_utf16( (char*)&utf8upper , chbytes , (char*)&utf16wchar , 2 , utf16bytes ) == ::rux::utils::string::ConversionResult_OK )
				|| ( sizeof( wchar_t ) == 4 && ::rux::utils::string::utf8::to_utf32( (char*)&utf8upper , chbytes , (char*)&utf16wchar , 4 , utf16bytes ) == ::rux::utils::string::ConversionResult_OK ) )
			{
				/*if( std::isdigit( utf16wchar , loc ) )
				{
					utf32upper = 0;
					if( ::rux::utils::string::utf8::to_utf32( (char*)&utf8upper , chbytes , (char*)&utf32upper , 4 , utf32size ) == ::rux::utils::string::ConversionResult_OK )
					{
						map[ utf32upper ] = 1;
					}
				}
				wchar_t t = std::toupper( utf16wchar , loc );
				if( t != utf16wchar )
				{
					map[ utf16wchar ] = t;
					/*utf8lower = 0;
					if( sizeof( wchar_t ) == 4 )
					{
						map[ utf16wchar ] = t;
					}
					else
					{
						if( ( sizeof( wchar_t ) == 2 && ::rux::utils::string::utf16::to_utf8( (char*)&t , 2 , (char*)&utf8lower , 4 , utf8size ) == ::rux::utils::string::ConversionResult_OK )
							|| ( sizeof( wchar_t ) == 4 && ::rux::utils::string::utf32::to_utf8( (char*)&t , 4 , (char*)&utf8lower , 4 , utf8size ) == ::rux::utils::string::ConversionResult_OK ) )
						{
							//map[ utf8upper ] = utf8lower;
							utf32lower = 0;
							if( ::rux::utils::string::utf8::to_utf32( (char*)&utf8lower , utf8size , (char*)&utf32lower , 4 , utf32size ) == ::rux::utils::string::ConversionResult_OK )
							{
								utf32upper = 0;
								if( ::rux::utils::string::utf8::to_utf32( (char*)&utf8upper , chbytes , (char*)&utf32upper , 4 , utf32size ) == ::rux::utils::string::ConversionResult_OK )
								{
									map_iter = map.find( utf32upper );
									if( map_iter == map.end() )
										map[ utf32upper ] = utf32lower;
									else
										printf( "" );
								}
								else
									printf( "" );
							}
							else
								printf( "" );
						}
						else
							printf( "" );
					}
				}
			}
			else
				printf( "" );
		}
	}
	printf( "sizeof(wchar_t) = %d, count = %u\n" , sizeof( wchar_t ) , (::rux::uint32)map.size() );
	*/
	
#endif
//#define XML_TEST 0
//#define UTF32_TO_UTF8_TEST
//#define UTF8_TO_UTF16_TEST
//#define UTF16_TO_UTF8_TEST
//#define UTF16_TO_UTF32_TEST
//#define UTF32_TO_UTF16_TEST
//#define CP1251_TO_UTF8_TEST
//#define CP1251_TO_UTF16_TEST
//#define CP1251_TO_UTF32_TEST
//#define UTF16_TO_CP1251_TEST
//#define UTF32_TO_CP1251_TEST
#define UTF8_TO_CP1251_TEST
//#define UTF8_TOLOWER_TEST
//#define UTF32_TOLOWER_TEST
//#define UTF16_TOLOWER_TEST
//#define UTF16_TOUPPER_TEST
#define CP1251_TOLOWER_TEST
	
#ifdef CP1251_TO_UTF8_TEST
	size_t res_size = utf8_test0_length;
#elif defined( CP1251_TO_UTF16_TEST )
	size_t res_size = utf16_test0_length;
#elif defined( CP1251_TO_UTF32_TEST )
	size_t res_size = utf32_test0_length;
#elif defined( UTF16_TO_CP1251_TEST )
	size_t res_size = cp1251_test0_length;
#elif defined( CP1251_TOLOWER_TEST )
	size_t res_size = cp1251_test0_length;
#elif defined( UTF32_TO_CP1251_TEST )
	size_t res_size = cp1251_test0_length;
#elif defined( UTF8_TO_CP1251_TEST )
	size_t res_size = cp1251_test0_length;
#elif defined( UTF8_TOLOWER_TEST )
	size_t res_size = xml_length;
#elif defined( UTF32_TOLOWER_TEST )
	size_t res_size = utf32_res_bytes;
#elif defined( UTF16_TOLOWER_TEST )
	size_t res_size = utf16_res_bytes;
#elif defined( UTF16_TOUPPER_TEST )
	size_t res_size = utf16_res_bytes;
#endif
	::rux::byte* res = (::rux::byte*)malloc( res_size );
	
	




	


	start_code_speed_performance
#if rapidxml_enable
			::std::vector<char> xml_copy(xml0.begin(), xml0.end());
			xml_copy.push_back('\0');
			::rapidxml::xml_document<> doc;
			doc.parse<0>( &xml_copy[0] );
			doc.clear();
#elif XML_TEST
			::rux::data::xml::document xml;
			xml.deserialize( xml0.c_str() );
			xml.free();
#elif UTF32_TO_UTF8_TEST
			size_t utf8_bytes = 0;
			::rux::utils::string::utf32::to_utf8( utf32_res , utf32_res_bytes , utf8 , utf8_allocated_size , utf8_bytes );
			//if( utf8_bytes != xml0.length() || memcmp( utf8 , xml0.c_str() , utf8_bytes ) != 0 )
			{
				//printf( "\n" );
			}
#elif defined( UTF8_TO_UTF16_TEST )
			size_t utf16_bytes = 0;

			//*(int*)utf16 = 0;
			//int result_bytes = MultiByteToWideChar( 65001 , 0 , (char*)xml , xml_length , (wchar_t*)utf16 , utf16_allocated_size / 2 );
			
			::rux::utils::string::utf8::to_utf16( (char*)xml , xml_length , utf16 , utf16_allocated_size , utf16_bytes );
			//if( utf16_bytes != utf16_res_bytes || memcmp( utf16 , utf16_res , utf16_bytes ) != 0 )
			{
				//printf( "\n" );
			}
#elif defined( UTF16_TO_UTF8_TEST )
			size_t utf8_bytes = 0;
			::rux::utils::string::utf16::to_utf8( utf16_res , utf16_res_bytes , utf8 , utf8_allocated_size , utf8_bytes );
			//if( utf8_bytes != xml_length || memcmp( utf8 , (char*)xml , utf8_bytes ) != 0 )
			{
				//printf( "\n" );
			}
#elif defined( UTF16_TO_UTF32_TEST )
			size_t utf32_bytes = 0;
			::rux::utils::string::utf16::to_utf32( utf16_res , utf16_res_bytes , utf32 , utf32_allocated_size , utf32_bytes );
			//if( utf32_bytes != utf32_res_bytes || memcmp( utf32 , utf32_res , utf32_res_bytes ) != 0 )
			{
				//printf( "\n" );
			}
#elif defined( UTF32_TO_UTF16_TEST )
			size_t utf16_bytes = 0;
			::rux::utils::string::utf32::to_utf16( utf32_res , utf32_res_bytes , utf16 , utf16_allocated_size , utf16_bytes );
			//if( utf16_bytes != utf16_res_bytes || memcmp( utf16 , utf16_res , utf16_bytes ) != 0 )
			{
				//printf( "\n" );
			}
#elif defined( CP1251_TO_UTF32_TEST )
			size_t res_bytes = 0;
			::rux::utils::string::cp1251::to_utf32( (char*)cp1251_test0 , cp1251_test0_length , (char*)res , res_size , res_bytes );
			if( res_bytes != utf32_test0_length || memcmp( utf32_test0 , res , res_bytes ) != 0 )
			{
				printf( "\n" );
			}
#elif defined( UTF16_TO_CP1251_TEST )
			size_t res_bytes = 0;
			::rux::utils::string::utf16::to_cp1251( (char*)utf16_test0 , utf16_test0_length , (char*)res , res_size , res_bytes );
			if( res_bytes != cp1251_test0_length || memcmp( cp1251_test0 , res , res_bytes ) != 0 )
			{
				printf( "\n" );
			}
#elif defined( UTF32_TO_CP1251_TEST )
			size_t res_bytes = 0;
			::rux::utils::string::utf32::to_cp1251( (char*)utf32_test0 , utf32_test0_length , (char*)res , res_size , res_bytes );
			//if( res_bytes != cp1251_test0_length || memcmp( cp1251_test0 , res , res_bytes ) != 0 )
			{
				//printf( "\n" );
			}
#elif defined( UTF8_TO_CP1251_TEST )
			size_t res_bytes = 0;
			::rux::utils::string::utf8::to_cp1251( (char*)utf8_test0 , utf8_test0_length , (char*)res , res_size , res_bytes );
			if( res_bytes != cp1251_test0_length || memcmp( cp1251_test0 , res , res_bytes ) != 0 )
			{
				//printf( "\n" );
			}
#elif defined( CP1251_TO_UTF16_TEST )
			size_t res_bytes = 0;
			::rux::utils::string::cp1251::to_utf16( (char*)cp1251_test0 , cp1251_test0_length , (char*)res , res_size , res_bytes );
			//if( res_bytes != utf16_test0_length || memcmp( utf16_test0 , res , res_bytes ) != 0 )
			{
				//printf( "\n" );
			}
#elif defined( CP1251_TOLOWER_TEST )
			size_t res_bytes = 0;
			::rux::utils::string::cp1251::to_lower( (char*)cp1251_test0 , cp1251_test0_length , (char*)res , res_size , res_bytes );
			if( res_bytes != cp1251_test0_length )//|| memcmp( utf16_test0 , res , res_bytes ) != 0 )
			{
				//printf( "\n" );
			}
#elif defined( CP1251_TO_UTF8_TEST )
			size_t res_bytes = 0;
			::rux::utils::string::cp1251::to_utf8( (char*)cp1251_test0 , cp1251_test0_length , (char*)res , res_size , res_bytes );
			//if( res_bytes != utf8_test0_length || memcmp( utf8_test0 , res , res_bytes ) != 0 )
			{
				//printf( "\n" );
			}
#elif defined( UTF8_TOLOWER_TEST )
			size_t utf8_bytes = 0;
			::rux::utils::string::utf8::to_lower( (char*)xml , xml_length , (char*)res , res_size , utf8_bytes );
			if( utf8_bytes != xml_length )//|| memcmp( utf32 , utf32_res , utf32_res_bytes ) != 0 )
			{
				//printf( "\n" );
			}	
#elif defined( UTF32_TOLOWER_TEST )
			size_t utf32_bytes = 0;
			::rux::utils::string::utf32::to_lower( (char*)utf32_res , utf32_res_bytes , (char*)res , res_size , utf32_bytes );
			if( utf32_bytes != utf32_res_bytes )//|| memcmp( utf32 , utf32_res , utf32_res_bytes ) != 0 )
			{
				printf( "\n" );
			}
#elif defined( UTF16_TOLOWER_TEST )
			size_t utf16_bytes = 0;
			::rux::utils::string::utf16::to_lower( (char*)utf16_res , utf16_res_bytes , (char*)res , res_size , utf16_bytes );
			if( utf16_bytes != utf16_res_bytes )//|| memcmp( utf32 , utf32_res , utf32_res_bytes ) != 0 )
			{
				printf( "\n" );
			}
#elif defined( UTF16_TOUPPER_TEST )
			size_t utf16_bytes = 0;
			::rux::utils::string::utf16::to_upper( (char*)utf16_res , utf16_res_bytes , (char*)res , res_size , utf16_bytes );
			if( utf16_bytes != utf16_res_bytes )//|| memcmp( utf32 , utf32_res , utf32_res_bytes ) != 0 )
			{
				printf( "\n" );
			}
#else
			size_t utf32_bytes = 0;
			::rux::utils::string::utf8::to_utf32( (char*)xml , xml_length , utf32 , utf32_allocated_size , utf32_bytes );
			//if( utf32_bytes != utf32_res_bytes || memcmp( utf32 , utf32_res , utf32_res_bytes ) != 0 )
			{
				//printf( "\n" );
			}
#endif
	end_code_speed_performance
	//free( utf32 );
	printf( "%s" , "======================\n" );
};
int main( int argc , char **argv )
{
    ::testing::InitGoogleTest( &argc , argv );
    return RUN_ALL_TESTS();
};