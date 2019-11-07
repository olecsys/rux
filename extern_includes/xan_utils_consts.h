#ifndef XAN_UTILS_CONSTS_H
#define XAN_UTILS_CONSTS_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_types.h"
namespace rux
{
	namespace utils
	{
		namespace string
		{
			namespace utf8
			{
				static const char _trailing_bytes_for_UTF8[ 256 ] = 
				{
					 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
					 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
					 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
					 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
					 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
					 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
					 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 ,
					 2 , 2 , 2 , 2 , 2 , 2 , 2 , 2 , 2 , 2 , 2 , 2 , 2 , 2 , 2 , 2 , 3 , 3 , 3 , 3 , 3 , 3 , 3 , 3 , 4 , 4 , 4 , 4 , 5 , 5 , 5 , 5
				};
				static const ::rux::uint32 _offsets_from_UTF8[ 7 ] =
				{
					0x00000000UL , 0x00000000UL , 0x00003080UL , 0x000E2080UL , 
					0x03C82080UL , 0xFA082080UL , 0x82082080UL 
				};				
			};
			namespace utf32
			{
				static const ::rux::byte _firstByteMark[7] = { 0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };
				const ::rux::byte _utf32_to_cp1251_page_from_152_to_187[] = 
				{
					0x98 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0xa0 , 0x00 , 0x00 , 0x00 , 0xa4 , 0x00 , 0xa6 , 
					0xa7 , 0x00 , 0xa9 , 0x00 , 0xab , 0xac , 0xad , 0xae , 0x00 , 0xb0 , 0xb1 , 0x00 , 0x00 , 0x00 , 0xb5 , 
					0xb6 , 0xb7 , 0x00 , 0x00 , 0x00 , 0xbb
				};
				const ::rux::byte _utf32_to_cp1251_page_from_1025_to_1169[] = 
				{
					0xa8 , 0x80 , 0x81 , 0xaa , 0xbd , 0xb2 , 0xaf , 0xa3 , 0x8a , 0x8c , 0x8e , 0x8d , 0x00 , 0xa1 , 0x8f ,
					0xc0 , 0xc1 , 0xc2 , 0xc3 , 0xc4 , 0xc5 , 0xc6 , 0xc7 , 0xc8 , 0xc9 , 0xca , 0xcb , 0xcc , 0xcd , 0xce ,
					0xcf , 0xd0 , 0xd1 , 0xd2 , 0xd3 , 0xd4 , 0xd5 , 0xd6 , 0xd7 , 0xd8 , 0xd9 , 0xda , 0xdb , 0xdc , 0xdd , 
					0xde , 0xdf , 0xe0 , 0xe1 , 0xe2 , 0xe3 , 0xe4 , 0xe5 , 0xe6 , 0xe7 , 0xe8 , 0xe9 , 0xea , 0xeb , 0xec ,
					0xed , 0xee , 0xef , 0xf0 , 0xf1 , 0xf2 , 0xf3 , 0xf4 , 0xf5 , 0xf6 , 0xf7 , 0xf8 , 0xf9 , 0xfa , 0xfb , 
					0xfc , 0xfd , 0xfe , 0xff , 0x00 , 0xb8 , 0x90 , 0x83 , 0xba , 0xbe , 0xb3 , 0xbf , 0xbc , 0x9a , 0x9c , 
					0x9e , 0x9d , 0x00 , 0xa2 , 0x9f , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 
					0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 
					0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 ,
					0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0xa5 , 0xb4
				};
				const ::rux::byte _utf32_to_cp1251_page_from_8211_to_8482[] = 
				{
					0x96 , 0x97 , 0x00 , 0x00 , 0x00 , 0x91 , 0x92 , 0x82 , 0x00 , 0x93 , 0x94 , 0x84 , 0x00 , 0x86 , 0x87 , 
					0x95 , 0x00 , 0x00 , 0x00 , 0x85 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x89 ,
					0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x8b , 0x9b , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 ,
					0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 
					0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 ,
					0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 ,
					0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 
					0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 ,
					0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 
					0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 ,
					0x00 , 0x00 , 0x00 , 0x88 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 ,
					0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 ,
					0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 ,
					0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 ,
					0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 ,
					0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 ,
					0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 
					0x00 , 0x00 , 0x00 , 0x00 , 0xb9 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 ,
					0x00 , 0x99
				};
			};
			namespace utf16
			{
				const ::rux::byte _utf16_to_cp1251_page_from_152_to_187[] = 
				{
					0x98 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0xa0 , 0x00 , 0x00 , 0x00 , 0xa4 , 0x00 , 0xa6 , 
					0xa7 , 0x00 , 0xa9 , 0x00 , 0xab , 0xac , 0xad , 0xae , 0x00 , 0xb0 , 0xb1 , 0x00 , 0x00 , 0x00 , 0xb5 , 
					0xb6 , 0xb7 , 0x00 , 0x00 , 0x00 , 0xbb
				};
				const ::rux::byte _utf16_to_cp1251_page_from_1025_to_1169[] = 
				{
					0xa8 , 0x80 , 0x81 , 0xaa , 0xbd , 0xb2 , 0xaf , 0xa3 , 0x8a , 0x8c , 0x8e , 0x8d , 0x00 , 0xa1 , 0x8f , 
					0xc0 , 0xc1 , 0xc2 , 0xc3 , 0xc4 , 0xc5 , 0xc6 , 0xc7 , 0xc8 , 0xc9 , 0xca , 0xcb , 0xcc , 0xcd , 0xce , 
					0xcf , 0xd0 , 0xd1 , 0xd2 , 0xd3 , 0xd4 , 0xd5 , 0xd6 , 0xd7 , 0xd8 , 0xd9 , 0xda , 0xdb , 0xdc , 0xdd , 
					0xde , 0xdf , 0xe0 , 0xe1 , 0xe2 , 0xe3 , 0xe4 , 0xe5 , 0xe6 , 0xe7 , 0xe8 , 0xe9 , 0xea , 0xeb , 0xec , 
					0xed , 0xee , 0xef , 0xf0 , 0xf1 , 0xf2 , 0xf3 , 0xf4 , 0xf5 , 0xf6 , 0xf7 , 0xf8 , 0xf9 , 0xfa , 0xfb , 
					0xfc , 0xfd , 0xfe , 0xff , 0x00 , 0xb8 , 0x90 , 0x83 , 0xba , 0xbe , 0xb3 , 0xbf , 0xbc , 0x9a , 0x9c , 
					0x9e , 0x9d , 0x00 , 0xa2 , 0x9f , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 
					0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 
					0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 
					0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0xa5 , 0xb4
				};
				const ::rux::byte _utf16_to_cp1251_page_from_8211_to_8482[] = 
				{
					0x96 , 0x97 , 0x00 , 0x00 , 0x00 , 0x91 , 0x92 , 0x82 , 0x00 , 0x93 , 0x94 , 0x84 , 0x00 , 0x86 , 0x87 , 
					0x95 , 0x00 , 0x00 , 0x00 , 0x85 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x89 , 
					0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x8b , 0x9b , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 
					0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 
					0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 
					0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 
					0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 
					0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 
					0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 
					0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 ,
					0x00 , 0x00 , 0x00 , 0x88 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 ,
					0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 ,
					0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 
					0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 
					0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 
					0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 ,
					0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 ,
					0x00 , 0x00 , 0x00 , 0x00 , 0xb9 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 
					0x00 , 0x99
				};
			};
			namespace cp1251
			{
				const ::rux::byte _isalnum[] =
				{
					0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
					0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 1 , 1 , 1 , 1 , 1 , 
					1 , 1 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 
					1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 
					1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 1 , 1 , 0 , 1 , 0 , 0 , 0 , 
					0 , 0 , 0 , 1 , 0 , 1 , 1 , 1 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 0 , 1 , 1 , 1 , 1 , 0 , 1 ,
					1 , 1 , 0 , 1 , 0 , 0 , 1 , 0 , 1 , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 1 , 1 , 1 , 1 , 0 , 0 , 1 , 0 , 1 , 0 , 1 , 
					1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 
					1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 
					1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1
				};
				const ::rux::byte _isalpha[] = 
				{
					0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 
					0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
					0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 ,
					1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 ,
					1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 1 , 1 , 0 , 1 , 0 , 0 , 0 , 
					0 , 0 , 0 , 1 , 0 , 1 , 1 , 1 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 0 , 1 , 1 , 1 , 1 , 0 , 1 ,
					1 , 1 , 0 , 1 , 0 , 0 , 1 , 0 , 1 , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 1 , 1 , 1 , 1 , 0 , 0 , 1 , 0 , 1 , 0 , 1 , 
					1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 
					1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 
					1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1
				};
				const ::rux::byte _toupper[] = 
				{
					0x00 , 0x01 , 0x02 , 0x03 , 0x04 , 0x05 , 0x06 , 0x07 , 0x08 , 0x09 , 0x0a , 0x0b , 0x0c , 0x0d , 0x0e , 0x0f , 
					0x10 , 0x11 , 0x12 , 0x13 , 0x14 , 0x15 , 0x16 , 0x17 , 0x18 , 0x19 , 0x1a , 0x1b , 0x1c , 0x1d , 0x1e , 0x1f ,
					0x20 , 0x21 , 0x22 , 0x23 , 0x24 , 0x25 , 0x26 , 0x27 , 0x28 , 0x29 , 0x2a , 0x2b , 0x2c , 0x2d , 0x2e , 0x2f ,
					0x30 , 0x31 , 0x32 , 0x33 , 0x34 , 0x35 , 0x36 , 0x37 , 0x38 , 0x39 , 0x3a , 0x3b , 0x3c , 0x3d , 0x3e , 0x3f ,
					0x40 , 0x41 , 0x42 , 0x43 , 0x44 , 0x45 , 0x46 , 0x47 , 0x48 , 0x49 , 0x4a , 0x4b , 0x4c , 0x4d , 0x4e , 0x4f ,
					0x50 , 0x51 , 0x52 , 0x53 , 0x54 , 0x55 , 0x56 , 0x57 , 0x58 , 0x59 , 0x5a , 0x5b , 0x5c , 0x5d , 0x5e , 0x5f ,
					0x60 , 0x41 , 0x42 , 0x43 , 0x44 , 0x45 , 0x46 , 0x47 , 0x48 , 0x49 , 0x4a , 0x4b , 0x4c , 0x4d , 0x4e , 0x4f ,
					0x50 , 0x51 , 0x52 , 0x53 , 0x54 , 0x55 , 0x56 , 0x57 , 0x58 , 0x59 , 0x5a , 0x7b , 0x7c , 0x7d , 0x7e , 0x7f ,
					0x80 , 0x81 , 0x82 , 0x81 , 0x84 , 0x85 , 0x86 , 0x87 , 0x88 , 0x89 , 0x8a , 0x8b , 0x8c , 0x8d , 0x8e , 0x8f ,
					0x80 , 0x91 , 0x92 , 0x93 , 0x94 , 0x95 , 0x96 , 0x97 , 0x98 , 0x99 , 0x8a , 0x9b , 0x8c , 0x8d , 0x8e , 0x8f ,
					0xa0 , 0xa1 , 0xa1 , 0xa3 , 0xa4 , 0xa5 , 0xa6 , 0xa7 , 0xa8 , 0xa9 , 0xaa , 0xab , 0xac , 0xad , 0xae , 0xaf ,
					0xb0 , 0xb1 , 0xb2 , 0xb2 , 0xa5 , 0xb5 , 0xb6 , 0xb7 , 0xa8 , 0xb9 , 0xaa , 0xbb , 0xa3 , 0xbd , 0xbd , 0xaf , 
					0xc0 , 0xc1 , 0xc2 , 0xc3 , 0xc4 , 0xc5 , 0xc6 , 0xc7 , 0xc8 , 0xc9 , 0xca , 0xcb , 0xcc , 0xcd , 0xce , 0xcf ,
					0xd0 , 0xd1 , 0xd2 , 0xd3 , 0xd4 , 0xd5 , 0xd6 , 0xd7 , 0xd8 , 0xd9 , 0xda , 0xdb , 0xdc , 0xdd , 0xde , 0xdf ,
					0xc0 , 0xc1 , 0xc2 , 0xc3 , 0xc4 , 0xc5 , 0xc6 , 0xc7 , 0xc8 , 0xc9 , 0xca , 0xcb , 0xcc , 0xcd , 0xce , 0xcf ,
					0xd0 , 0xd1 , 0xd2 , 0xd3 , 0xd4 , 0xd5 , 0xd6 , 0xd7 , 0xd8 , 0xd9 , 0xda , 0xdb , 0xdc , 0xdd , 0xde , 0xdf
				};
				const ::rux::byte _tolower[] =
				{
					0x00 , 0x01 , 0x02 , 0x03 , 0x04 , 0x05 , 0x06 , 0x07 , 0x08 , 0x09 , 0x0a , 0x0b , 0x0c , 0x0d , 0x0e , 0x0f ,
					0x10 , 0x11 , 0x12 , 0x13 , 0x14 , 0x15 , 0x16 , 0x17 , 0x18 , 0x19 , 0x1a , 0x1b , 0x1c , 0x1d , 0x1e , 0x1f ,
					0x20 , 0x21 , 0x22 , 0x23 , 0x24 , 0x25 , 0x26 , 0x27 , 0x28 , 0x29 , 0x2a , 0x2b , 0x2c , 0x2d , 0x2e , 0x2f ,
					0x30 , 0x31 , 0x32 , 0x33 , 0x34 , 0x35 , 0x36 , 0x37 , 0x38 , 0x39 , 0x3a , 0x3b , 0x3c , 0x3d , 0x3e , 0x3f ,
					0x40 , 0x61 , 0x62 , 0x63 , 0x64 , 0x65 , 0x66 , 0x67 , 0x68 , 0x69 , 0x6a , 0x6b , 0x6c , 0x6d , 0x6e , 0x6f , 
					0x70 , 0x71 , 0x72 , 0x73 , 0x74 , 0x75 , 0x76 , 0x77 , 0x78 , 0x79 , 0x7a , 0x5b , 0x5c , 0x5d , 0x5e , 0x5f ,
					0x60 , 0x61 , 0x62 , 0x63 , 0x64 , 0x65 , 0x66 , 0x67 , 0x68 , 0x69 , 0x6a , 0x6b , 0x6c , 0x6d , 0x6e , 0x6f ,
					0x70 , 0x71 , 0x72 , 0x73 , 0x74 , 0x75 , 0x76 , 0x77 , 0x78 , 0x79 , 0x7a , 0x7b , 0x7c , 0x7d , 0x7e , 0x7f , 
					0x90 , 0x83 , 0x82 , 0x83 , 0x84 , 0x85 , 0x86 , 0x87 , 0x88 , 0x89 , 0x9a , 0x8b , 0x9c , 0x9d , 0x9e , 0x9f , 
					0x90 , 0x91 , 0x92 , 0x93 , 0x94 , 0x95 , 0x96 , 0x97 , 0x98 , 0x99 , 0x9a , 0x9b , 0x9c , 0x9d , 0x9e , 0x9f ,
					0xa0 , 0xa2 , 0xa2 , 0xbc , 0xa4 , 0xb4 , 0xa6 , 0xa7 , 0xb8 , 0xa9 , 0xba , 0xab , 0xac , 0xad , 0xae , 0xbf ,
					0xb0 , 0xb1 , 0xb3 , 0xb3 , 0xb4 , 0xb5 , 0xb6 , 0xb7 , 0xb8 , 0xb9 , 0xba , 0xbb , 0xbc , 0xbe , 0xbe , 0xbf ,
					0xe0 , 0xe1 , 0xe2 , 0xe3 , 0xe4 , 0xe5 , 0xe6 , 0xe7 , 0xe8 , 0xe9 , 0xea , 0xeb , 0xec , 0xed , 0xee , 0xef , 
					0xf0 , 0xf1 , 0xf2 , 0xf3 , 0xf4 , 0xf5 , 0xf6 , 0xf7 , 0xf8 , 0xf9 , 0xfa , 0xfb , 0xfc , 0xfd , 0xfe , 0xff ,
					0xe0 , 0xe1 , 0xe2 , 0xe3 , 0xe4 , 0xe5 , 0xe6 , 0xe7 , 0xe8 , 0xe9 , 0xea , 0xeb , 0xec , 0xed , 0xee , 0xef , 
					0xf0 , 0xf1 , 0xf2 , 0xf3 , 0xf4 , 0xf5 , 0xf6 , 0xf7 , 0xf8 , 0xf9 , 0xfa , 0xfb , 0xfc , 0xfd , 0xfe , 0xff
				};
				const ::rux::uint16 _cp1251_to_utf16[] = {
					0x00 , 0x01 , 0x02 , 0x03 , 0x04 , 0x05 , 0x06 , 0x07 , 0x08 , 0x09 , 0x0a , 0x0b , 0x0c , 0x0d , 0x0e , 0x0f , 
					0x10 , 0x11 , 0x12 , 0x13 , 0x14 , 0x15 , 0x16 , 0x17 , 0x18 , 0x19 , 0x1a , 0x1b , 0x1c , 0x1d , 0x1e , 0x1f , 
					0x20 , 0x21 , 0x22 , 0x23 , 0x24 , 0x25 , 0x26 , 0x27 , 0x28 , 0x29 , 0x2a , 0x2b , 0x2c , 0x2d , 0x2e , 0x2f , 
					0x30 , 0x31 , 0x32 , 0x33 , 0x34 , 0x35 , 0x36 , 0x37 , 0x38 , 0x39 , 0x3a , 0x3b , 0x3c , 0x3d , 0x3e , 0x3f , 
					0x40 , 0x41 , 0x42 , 0x43 , 0x44 , 0x45 , 0x46 , 0x47 , 0x48 , 0x49 , 0x4a , 0x4b , 0x4c , 0x4d , 0x4e , 0x4f , 
					0x50 , 0x51 , 0x52 , 0x53 , 0x54 , 0x55 , 0x56 , 0x57 , 0x58 , 0x59 , 0x5a , 0x5b , 0x5c , 0x5d , 0x5e , 0x5f , 
					0x60 , 0x61 , 0x62 , 0x63 , 0x64 , 0x65 , 0x66 , 0x67 , 0x68 , 0x69 , 0x6a , 0x6b , 0x6c , 0x6d , 0x6e , 0x6f , 
					0x70 , 0x71 , 0x72 , 0x73 , 0x74 , 0x75 , 0x76 , 0x77 , 0x78 , 0x79 , 0x7a , 0x7b , 0x7c , 0x7d , 0x7e , 0x7f , 
					0x0402 , 0x0403 , 0x201a , 0x0453 , 0x201e , 0x2026 , 0x2020 , 0x2021 , 0x20ac , 0x2030 , 0x0409 , 0x2039 , 0x040a , 0x040c , 0x040b , 0x040f , 
					0x0452 , 0x2018 , 0x2019 , 0x201c , 0x201d , 0x2022 , 0x2013 , 0x2014 , 0x98 , 0x2122 , 0x0459 , 0x203a , 0x045a , 0x045c , 0x045b , 0x045f , 
					0xa0 , 0x040e , 0x045e , 0x0408 , 0xa4 , 0x0490 , 0xa6 , 0xa7 , 0x0401 , 0xa9 , 0x0404 , 0xab , 0xac , 0xad , 0xae , 0x0407 , 
					0xb0 , 0xb1 , 0x0406 , 0x0456 , 0x0491 , 0xb5 , 0xb6 , 0xb7 , 0x0451 , 0x2116 , 0x0454 , 0xbb , 0x0458 , 0x0405 , 0x0455 , 0x0457 , 
					0x0410 , 0x0411 , 0x0412 , 0x0413 , 0x0414 , 0x0415 , 0x0416 , 0x0417 , 0x0418 , 0x0419 , 0x041a , 0x041b , 0x041c , 0x041d , 0x041e , 0x041f , 
					0x0420 , 0x0421 , 0x0422 , 0x0423 , 0x0424 , 0x0425 , 0x0426 , 0x0427 , 0x0428 , 0x0429 , 0x042a , 0x042b , 0x042c , 0x042d , 0x042e , 0x042f , 
					0x0430 , 0x0431 , 0x0432 , 0x0433 , 0x0434 , 0x0435 , 0x0436 , 0x0437 , 0x0438 , 0x0439 , 0x043a , 0x043b , 0x043c , 0x043d , 0x043e , 0x043f , 
					0x0440 , 0x0441 , 0x0442 , 0x0443 , 0x0444 , 0x0445 , 0x0446 , 0x0447 , 0x0448 , 0x0449 , 0x044a , 0x044b , 0x044c , 0x044d , 0x044e , 0x044f
				};
				const ::rux::uint8 _cp1251_to_utf8_length[] = {
					0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 
					0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 
					0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 
					0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 
					0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 
					0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 
					0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 
					0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 0x01 , 
					0x02 , 0x02 , 0x03 , 0x02 , 0x03 , 0x03 , 0x03 , 0x03 , 0x03 , 0x03 , 0x02 , 0x03 , 0x02 , 0x02 , 0x02 , 0x02 , 
					0x02 , 0x03 , 0x03 , 0x03 , 0x03 , 0x03 , 0x03 , 0x03 , 0x02 , 0x03 , 0x02 , 0x03 , 0x02 , 0x02 , 0x02 , 0x02 , 
					0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 
					0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 0x03 , 0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 
					0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 
					0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 
					0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 
					0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 0x02 , 0x02
				};
				static ::rux::uint32 _cp1251_to_utf8[] = {
					0x00 , 0x01 , 0x02 , 0x03 , 0x04 , 0x05 , 0x06 , 0x07 , 0x08 , 0x09 , 0x0a , 0x0b , 0x0c , 0x0d , 0x0e , 0x0f , 
					0x10 , 0x11 , 0x12 , 0x13 , 0x14 , 0x15 , 0x16 , 0x17 , 0x18 , 0x19 , 0x1a , 0x1b , 0x1c , 0x1d , 0x1e , 0x1f , 
					0x20 , 0x21 , 0x22 , 0x23 , 0x24 , 0x25 , 0x26 , 0x27 , 0x28 , 0x29 , 0x2a , 0x2b , 0x2c , 0x2d , 0x2e , 0x2f , 
					0x30 , 0x31 , 0x32 , 0x33 , 0x34 , 0x35 , 0x36 , 0x37 , 0x38 , 0x39 , 0x3a , 0x3b , 0x3c , 0x3d , 0x3e , 0x3f , 
					0x40 , 0x41 , 0x42 , 0x43 , 0x44 , 0x45 , 0x46 , 0x47 , 0x48 , 0x49 , 0x4a , 0x4b , 0x4c , 0x4d , 0x4e , 0x4f , 
					0x50 , 0x51 , 0x52 , 0x53 , 0x54 , 0x55 , 0x56 , 0x57 , 0x58 , 0x59 , 0x5a , 0x5b , 0x5c , 0x5d , 0x5e , 0x5f , 
					0x60 , 0x61 , 0x62 , 0x63 , 0x64 , 0x65 , 0x66 , 0x67 , 0x68 , 0x69 , 0x6a , 0x6b , 0x6c , 0x6d , 0x6e , 0x6f , 
					0x70 , 0x71 , 0x72 , 0x73 , 0x74 , 0x75 , 0x76 , 0x77 , 0x78 , 0x79 , 0x7a , 0x7b , 0x7c , 0x7d , 0x7e , 0x7f , 
					0x82d0 , 0x83d0 , 0x9a80e2 , 0x9a93d1 , 0x9e80e2 , 0xa680e2 , 0xa080e2 , 0xa180e2 , 0xac82e2 , 0xb080e2 , 0xb089d0 , 0xb980e2 , 0xb98ad0 , 0xb98cd0 , 0xb98bd0 , 0xb98fd0 , 
					0xb992d1 , 0x9880e2 , 0x9980e2 , 0x9c80e2 , 0x9d80e2 , 0xa280e2 , 0x9380e2 , 0x9480e2 , 0x9498c2 , 0xa284e2 , 0xa299d1 , 0xba80e2 , 0xba9ad1 , 0xba9cd1 , 0xba9bd1 , 0xba9fd1 , 
					0xbaa0c2 , 0xba8ed0 , 0xba9ed1 , 0xba88d0 , 0xbaa4c2 , 0xba90d2 , 0xbaa6c2 , 0xbaa7c2 , 0xba81d0 , 0xbaa9c2 , 0xba84d0 , 0xbaabc2 , 0xbaacc2 , 0xbaadc2 , 0xbaaec2 , 0xba87d0 , 
					0xbab0c2 , 0xbab1c2 , 0xba86d0 , 0xba96d1 , 0xba91d2 , 0xbab5c2 , 0xbab6c2 , 0xbab7c2 , 0xba91d1 , 0x9684e2 , 0x9694d1 , 0x96bbc2 , 0x9698d1 , 0x9685d0 , 0x9695d1 , 0x9697d1 , 
					0x9690d0 , 0x9691d0 , 0x9692d0 , 0x9693d0 , 0x9694d0 , 0x9695d0 , 0x9696d0 , 0x9697d0 , 0x9698d0 , 0x9699d0 , 0x969ad0 , 0x969bd0 , 0x969cd0 , 0x969dd0 , 0x969ed0 , 0x969fd0 , 
					0x96a0d0 , 0x96a1d0 , 0x96a2d0 , 0x96a3d0 , 0x96a4d0 , 0x96a5d0 , 0x96a6d0 , 0x96a7d0 , 0x96a8d0 , 0x96a9d0 , 0x96aad0 , 0x96abd0 , 0x96acd0 , 0x96add0 , 0x96aed0 , 0x96afd0 , 
					0x96b0d0 , 0x96b1d0 , 0x96b2d0 , 0x96b3d0 , 0x96b4d0 , 0x96b5d0 , 0x96b6d0 , 0x96b7d0 , 0x96b8d0 , 0x96b9d0 , 0x96bad0 , 0x96bbd0 , 0x96bcd0 , 0x96bdd0 , 0x96bed0 , 0x96bfd0 , 
					0x9680d1 , 0x9681d1 , 0x9682d1 , 0x9683d1 , 0x9684d1 , 0x9685d1 , 0x9686d1 , 0x9687d1 , 0x9688d1 , 0x9689d1 , 0x968ad1 , 0x968bd1 , 0x968cd1 , 0x968dd1 , 0x968ed1 , 0x968fd1
				};
				const ::rux::uint32 _cp1251_to_utf32[] = {
					0x00 , 0x01 , 0x02 , 0x03 , 0x04 , 0x05 , 0x06 , 0x07 , 0x08 , 0x09 , 0x0a , 0x0b , 0x0c , 0x0d , 0x0e , 0x0f , 
					0x10 , 0x11 , 0x12 , 0x13 , 0x14 , 0x15 , 0x16 , 0x17 , 0x18 , 0x19 , 0x1a , 0x1b , 0x1c , 0x1d , 0x1e , 0x1f , 
					0x20 , 0x21 , 0x22 , 0x23 , 0x24 , 0x25 , 0x26 , 0x27 , 0x28 , 0x29 , 0x2a , 0x2b , 0x2c , 0x2d , 0x2e , 0x2f , 
					0x30 , 0x31 , 0x32 , 0x33 , 0x34 , 0x35 , 0x36 , 0x37 , 0x38 , 0x39 , 0x3a , 0x3b , 0x3c , 0x3d , 0x3e , 0x3f , 
					0x40 , 0x41 , 0x42 , 0x43 , 0x44 , 0x45 , 0x46 , 0x47 , 0x48 , 0x49 , 0x4a , 0x4b , 0x4c , 0x4d , 0x4e , 0x4f , 
					0x50 , 0x51 , 0x52 , 0x53 , 0x54 , 0x55 , 0x56 , 0x57 , 0x58 , 0x59 , 0x5a , 0x5b , 0x5c , 0x5d , 0x5e , 0x5f , 
					0x60 , 0x61 , 0x62 , 0x63 , 0x64 , 0x65 , 0x66 , 0x67 , 0x68 , 0x69 , 0x6a , 0x6b , 0x6c , 0x6d , 0x6e , 0x6f , 
					0x70 , 0x71 , 0x72 , 0x73 , 0x74 , 0x75 , 0x76 , 0x77 , 0x78 , 0x79 , 0x7a , 0x7b , 0x7c , 0x7d , 0x7e , 0x7f , 
					0x0402 , 0x0403 , 0x201a , 0x0453 , 0x201e , 0x2026 , 0x2020 , 0x2021 , 0x20ac , 0x2030 , 0x0409 , 0x2039 , 0x040a , 0x040c , 0x040b , 0x040f , 
					0x0452 , 0x2018 , 0x2019 , 0x201c , 0x201d , 0x2022 , 0x2013 , 0x2014 , 0x98 , 0x2122 , 0x0459 , 0x203a , 0x045a , 0x045c , 0x045b , 0x045f , 
					0xa0 , 0x040e , 0x045e , 0x0408 , 0xa4 , 0x0490 , 0xa6 , 0xa7 , 0x0401 , 0xa9 , 0x0404 , 0xab , 0xac , 0xad , 0xae , 0x0407 , 
					0xb0 , 0xb1 , 0x0406 , 0x0456 , 0x0491 , 0xb5 , 0xb6 , 0xb7 , 0x0451 , 0x2116 , 0x0454 , 0xbb , 0x0458 , 0x0405 , 0x0455 , 0x0457 , 
					0x0410 , 0x0411 , 0x0412 , 0x0413 , 0x0414 , 0x0415 , 0x0416 , 0x0417 , 0x0418 , 0x0419 , 0x041a , 0x041b , 0x041c , 0x041d , 0x041e , 0x041f , 
					0x0420 , 0x0421 , 0x0422 , 0x0423 , 0x0424 , 0x0425 , 0x0426 , 0x0427 , 0x0428 , 0x0429 , 0x042a , 0x042b , 0x042c , 0x042d , 0x042e , 0x042f , 
					0x0430 , 0x0431 , 0x0432 , 0x0433 , 0x0434 , 0x0435 , 0x0436 , 0x0437 , 0x0438 , 0x0439 , 0x043a , 0x043b , 0x043c , 0x043d , 0x043e , 0x043f , 
					0x0440 , 0x0441 , 0x0442 , 0x0443 , 0x0444 , 0x0445 , 0x0446 , 0x0447 , 0x0448 , 0x0449 , 0x044a , 0x044b , 0x044c , 0x044d , 0x044e , 0x044f
				};
			};
		};
	};
};
#endif
