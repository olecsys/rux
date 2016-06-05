#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_xml.h>
#include <xan_xml_consts.h>
namespace rux
{
	namespace data
	{
		namespace xml
		{
			document::document( void )
			{	
				error[ 0 ] = 0;
				xml_declaration = 0;
				document_declaration = 0;
				root = 0;
				node_memory = 0;
				xml = 0;
			};
			document::~document( void )
			{
				free();
			};
			const ::rux::byte _xml_deserialize_table_decimals_quote[] = 
			{
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, '\'', 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0, 1, 2, 3, 4, 5, 6, 7, 8, 9 , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff
			};
			const ::rux::byte _xml_deserialize_table_decimals_doublequote[] = 
			{
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, '"', 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0, 1, 2, 3, 4, 5, 6, 7, 8, 9 , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff
			};
			const ::rux::byte _xml_deserialize_table_white_space[] = 
			{
				0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, ' ' , ' ' , 0xff , 0xff , ' ' , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				' ', 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, '/',
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , '<' , '=' , '>' , '?',
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff
			};
			const ::rux::byte _xml_deserialize_encoding_StartEncName[] = 
			{
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, '"', 0xff, 0xff, 0xff, 0xff, '\'', 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I' , 'J' , 'K' , 'L' , 'M' , 'N', 'O',
				'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y' , 'Z' , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i' , 'j' , 'k' , 'l' , 'm' , 'n', 'o',
				'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y' , 'z' , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff
			};
			const ::rux::byte _xml_deserialize_encoding_EncName[] = 
			{
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, '"', 0xff, 0xff, 0xff, 0xff, '\'', 0xff, 0xff , 0xff , 0xff , 0xff , '-' , '.', 0xff,
				'0', '1', '2', '3', '4', '5', '6', '7', '8', '9' , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I' , 'J' , 'K' , 'L' , 'M' , 'N', 'O',
				'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y' , 'Z' , 0xff , 0xff , 0xff , 0xff, '_',
				0xff, 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i' , 'j' , 'k' , 'l' , 'm' , 'n', 'o',
				'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y' , 'z' , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff
			};
			::rux::byte _xml_deserialize_NameChar_table_0[] = 
			{
				' ' , 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, ' '/*\t*/ , ' '/*\n*/ , 0xff , 0xff , ' '/*\r*/ , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				' '/* */, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, ' '/*/*/,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , ' '/*;*/ , 0xff , ' '/*=*/ , ' '/*>*/, ' '/*?*/,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff , 0xff , 0xff , 0xff , 0xff , 0xff, 0xff
			};
			void document::free( void )
			{
				if( xml )
				{
					::free( xml );
					xml = 0;
				}
				if( node_memory )
				{
					node_memory_free( node_memory );					
					node_memory = 0;
					xml_declaration = 0;
					document_declaration = 0;
					root = 0;
				}
			};
			bool document::deserialize( const char* utf8 )
			{
				free();
				error[ 0 ] = 0;
				const ::rux::byte* ptr = (const ::rux::byte*)utf8 , * temp = 0;
				size_t utf8_size = ::strlen( utf8 ) + 1 , temp_size = 0;
				
				::rux::uint32 utf8_character = 0;
				::rux::byte* utf8_character_bytes = (::rux::byte*)&utf8_character;

				size_t nodes_size = 64 * sizeof( ::rux::data::xml::node );

				node_memory_init( node_memory , nodes_size );

				::rux::data::xml::node* elem = 0 , * prev_elem = 0 , * elem_child = 0;
				xml = (char*)malloc( utf8_size );
				size_t xml_index = 0;

				::rux::data::xml::deserialize_step step = ::rux::data::xml::deserialize_goto_attribute_value_quote_return_from_name_parse , prev_step = ::rux::data::xml::deserialize_goto_attribute_value_quote_return_from_name_parse;
	
serializer_deserialize_main:
				switch( _xml_deserialize_table_white_space[ *ptr++ ] )
				{
				case '<':
					{
						switch( *ptr )
						{
						case '?':
							goto xml_declaration_parse;
						case '!':
							goto document_type_declaration_parse;
						default:
							goto element_parse;
						}
					}
				case 0:
					{	
						temp = (const ::rux::byte*)"unexpected end of XML";
						goto serializer_deserialize_error;
					}
				case ' ':
					goto serializer_deserialize_main;
				default:
					{
						temp = (const ::rux::byte*)"incorrect syntax in XML document";
						goto serializer_deserialize_error;
					}
				}

xml_declaration_parse:
				xml_declaration = (::rux::data::xml::node*)node_memory_alloc( sizeof( ::rux::data::xml::node ) , node_memory );
				xml_declaration->parent = 0;
				xml_declaration->type = ::rux::data::xml::node_type_xml_declaration;
				xml_declaration->next = 0;
				xml_declaration->xml_decl.encoding = 0;
				xml_declaration->xml_decl.standalone = 0;
				xml_declaration->xml_decl.version = 0;
				step = ::rux::data::xml::deserialize_goto_eq_parse;
				switch( *++ptr )
				{
				case 'x':
					{
						switch( *++ptr )
						{
						case 'm':
							{
								switch( *++ptr )
								{
								case 'l':
									{
										switch( _xml_deserialize_table_white_space[ *++ptr ] )
										{
										case ' ':
											{
												switch( *++ptr )
												{
												case 'v':
													{
														switch( *++ptr )
														{
														case 'e':
															{
																switch( *++ptr )
																{
																case 'r':
																	{
																		switch( *++ptr )
																		{
																		case 's':
																			{
																				switch( *++ptr )
																				{
																				case 'i':
																					{
																						switch( *++ptr )
																						{
																						case 'o':
																							{
																								switch( *++ptr )
																								{
																								case 'n':
																									{
																										step = ::rux::data::xml::deserialize_goto_xml_declaration_version_info;
																										ptr++;
																										goto eq_parse;
																									}
																								default:
																									{
																										temp = (const ::rux::byte*)"incorrect syntax in XML declaration version info, 'n' is not found";
																										goto serializer_deserialize_error;
																									}
																								}
																							}
																						default:
																							{
																								temp = (const ::rux::byte*)"incorrect syntax in XML declaration version info, 'o' is not found";
																								goto serializer_deserialize_error;
																							}
																						}
																					}
																				default:
																					{
																						temp = (const ::rux::byte*)"incorrect syntax in XML declaration version info, 'i' is not found";
																						goto serializer_deserialize_error;
																					}
																				}
																			}
																		default:
																			{
																				temp = (const ::rux::byte*)"incorrect syntax in XML declaration version info, 's' is not found";
																				goto serializer_deserialize_error;
																			}
																		}
																	}
																default:
																	{
																		temp = (const ::rux::byte*)"incorrect syntax in XML declaration version info, 'r' is not found";
																		goto serializer_deserialize_error;
																	}
																}
															}
														default:
															{
																temp = (const ::rux::byte*)"incorrect syntax in XML declaration version info, 'e' is not found";
																goto serializer_deserialize_error;
															}
														}
													}
												default:
													{
														temp = (const ::rux::byte*)"incorrect syntax in XML declaration version info, 'v' is not found";
														goto serializer_deserialize_error;
													}
												}
											}
										default:
											{
												temp = (const ::rux::byte*)"incorrect syntax in XML declaration, white space is not found";
												goto serializer_deserialize_error;
											}
										}
									}
								default:
									{
										temp = (const ::rux::byte*)"incorrect syntax in XML declaration, 'l' is not found";
										goto serializer_deserialize_error;
									}
								}
							}
						default:
							{
								temp = (const ::rux::byte*)"incorrect syntax in XML declaration, 'm' is not found";
								goto serializer_deserialize_error;
							}
						}
					}
				default:
					{
						temp = (const ::rux::byte*)"incorrect syntax in XML declaration, 'x' is not found";
						goto serializer_deserialize_error;
					}
				}
				goto serializer_deserialize_main;

xml_declaration_version_info:
				switch( *ptr )
				{
				case '\'':
					{
						switch( *++ptr )
						{
						case '1':
							{
								switch( *++ptr )
								{
								case '.':
									{
										::memcpy( &xml[ xml_index ] , "<?xml version='1." , 17 );
										xml_index += 15;
										xml_declaration->xml_decl.version = &xml[ xml_index ];
										xml_index += 2;
xml_declaration_version_info_quote_cycle:
										ptr++;
										switch( _xml_deserialize_table_decimals_quote[ *ptr ] )
										{
										case '\'':
											{
												xml[ xml_index++ ] = 0x07;
												goto xml_declaration_version_info_success;
											}
										case 0xff:
											{
												temp = (const ::rux::byte*)"incorrect syntax in XML declaration version info, decimal is not found";
												goto serializer_deserialize_error;
											}
										default:
											{
												xml[ xml_index++ ] = *ptr;
												goto xml_declaration_version_info_quote_cycle;
											}
										}
									}
								default:
									{
										temp = (const ::rux::byte*)"incorrect syntax in XML declaration version info, '.' is not found";
										goto serializer_deserialize_error;
									}
								}
							}
						default:
							{
								temp = (const ::rux::byte*)"incorrect syntax in XML declaration version info, '1' is not found";
								goto serializer_deserialize_error;
							}
						}
					}
				case '"':
					{
						switch( *++ptr )
						{
						case '1':
							{
								switch( *++ptr )
								{
								case '.':
									{
										::memcpy( &xml[ xml_index ] , "<?xml version=\"1." , 17 );
										xml_index += 15;
										xml_declaration->xml_decl.version = &xml[ xml_index ];
										xml_index += 2;
xml_declaration_version_info_doublequote_cycle:
										ptr++;
										switch( _xml_deserialize_table_decimals_doublequote[ *ptr ] )
										{
										case '"':
											{
												xml[ xml_index++ ] = 0x07;
												goto xml_declaration_version_info_success;
											}
										case 0xff:
											{
												temp = (const ::rux::byte*)"incorrect syntax in XML declaration version info, decimal is not found";
												goto serializer_deserialize_error;
											}
										default:
											{
												xml[ xml_index++ ] = *ptr;
												goto xml_declaration_version_info_doublequote_cycle;
											}
										}
									}
								default:
									{
										temp = (const ::rux::byte*)"incorrect syntax in XML declaration version info, '.' is not found";
										goto serializer_deserialize_error;
									}
								}
							}
						default:
							{
								temp = (const ::rux::byte*)"incorrect syntax in XML declaration version info, '1' is not found";
								goto serializer_deserialize_error;
							}
						}
					}
				default:
					{
						temp = (const ::rux::byte*)"incorrect syntax in XML declaration version info, ''' or '\"' is not found";
						goto serializer_deserialize_error;
					}
				}
xml_declaration_version_info_success:
				switch( _xml_deserialize_table_white_space[ *++ptr ] )
				{
				case ' ':
					{
						switch( *++ptr )
						{
						case 'e':
							{
								switch( *++ptr )
								{
								case 'n':
								{
									switch( *++ptr )
									{
									case 'c':
									{
										switch( *++ptr )
										{
										case 'o':
										{
											switch( *++ptr )
											{
											case 'd':
											{
												switch( *++ptr )
												{
												case 'i':
												{
													switch( *++ptr )
													{
													case 'n':
													{
														switch( *++ptr )
														{
														case 'g':
														{
															step = ::rux::data::xml::deserialize_goto_xml_declaration_encoding;
															ptr++;
															goto eq_parse;
xml_declaration_encoding_parse:
															switch( *ptr )
															{
															case '\'':
																{
																	::memcpy( &xml[ xml_index ] , " encoding='" , 11 );
																	xml_index += 11;
																	xml_declaration->xml_decl.encoding = &xml[ xml_index ];
																	ptr++;
																	switch( _xml_deserialize_encoding_StartEncName[ *ptr ] )
																	{
																	case 0xff:
																	case '"':
																		{
																			temp = (const ::rux::byte*)"incorrect syntax in XML declaration encoding";
																			goto serializer_deserialize_error;
																		}
																	case '\'':
																		{
																			temp = (const ::rux::byte*)"XML declaration encoding cannot be empty";
																			goto serializer_deserialize_error;
																		}
																	default:
																		xml[ xml_index++ ] = *ptr;
																	}
xml_declaration_encoding_parse_quote_cycle:
																	ptr++;
																	switch( _xml_deserialize_encoding_EncName[ *ptr ] )
																	{
																	case 0xff:
																	case '"':
																		{
																			temp = (const ::rux::byte*)"incorrect syntax in XML declaration encoding";
																			goto serializer_deserialize_error;
																		}
																	case '\'':
																		{
																			xml[ xml_index++ ] = 0x07;
																			//SUCCESS
																			switch( _xml_deserialize_table_white_space[ *++ptr ] )
																			{
																			case ' ':
																				{
																					switch( *++ptr )
																					{
																					case 's':
																						goto xml_declaration_maybe_standalone_parse;
																					case '?':
																						goto xml_declaration_end_parse;
																					default:
																						{
																							temp = (const ::rux::byte*)"incorrect syntax in XML declaration, 's' or '?' is not found";
																							goto serializer_deserialize_error;
																						}
																					}
																				}
																			case '?':
																				goto xml_declaration_end_parse;
																			default:
																				{
																					temp = (const ::rux::byte*)"incorrect syntax in XML declaration, ' ' or '?' is not found";
																					goto serializer_deserialize_error;
																				}
																			}
																		}
																	default:
																		{
																			xml[ xml_index++ ] = *ptr;
																			goto xml_declaration_encoding_parse_quote_cycle;
																		}
																	}
																}
															case '"':
																{
																	::memcpy( &xml[ xml_index ] , " encoding=\"" , 11 );
																	xml_index += 11;
																	xml_declaration->xml_decl.encoding = &xml[ xml_index ];
																	ptr++;
																	switch( _xml_deserialize_encoding_StartEncName[ *ptr ] )
																	{
																	case 0xff:
																	case '\'':
																		{
																			temp = (const ::rux::byte*)"incorrect syntax in XML declaration encoding";
																			goto serializer_deserialize_error;
																		}
																	case '"':
																		{
																			temp = (const ::rux::byte*)"XML declaration encoding cannot be empty";
																			goto serializer_deserialize_error;
																		}
																	default:
																		xml[ xml_index++ ] = *ptr;
																	}
xml_declaration_encoding_parse_doublequote_cycle:
																	ptr++;
																	switch( _xml_deserialize_encoding_EncName[ *ptr ] )
																	{
																	case 0xff:
																	case '\'':
																		{
																			temp = (const ::rux::byte*)"incorrect syntax in XML declaration encoding";
																			goto serializer_deserialize_error;
																		}
																	case '"':
																		{
																			xml[ xml_index++ ] = 0x07;
																			//SUCCESS
																			switch( _xml_deserialize_table_white_space[ *++ptr ] )
																			{
																			case ' ':
																				{
																					switch( *++ptr )
																					{
																					case 's':
																						goto xml_declaration_maybe_standalone_parse;
																					case '?':
																						goto xml_declaration_end_parse;
																					default:
																						{
																							temp = (const ::rux::byte*)"incorrect syntax in XML declaration, 's' or '?' is not found";
																							goto serializer_deserialize_error;
																						}
																					}
																				}
																			case '?':
																				goto xml_declaration_end_parse;
																			default:
																				{
																					temp = (const ::rux::byte*)"incorrect syntax in XML declaration, ' ' or '?' is not found";
																					goto serializer_deserialize_error;
																				}
																			}
																		}
																	default:
																		{
																			xml[ xml_index++ ] = *ptr;
																			goto xml_declaration_encoding_parse_doublequote_cycle;
																		}
																	}
																}
															default:
																{
																	temp = (const ::rux::byte*)"incorrect syntax in XML declaration encoding, ''' or '\"' is not found";
																	goto serializer_deserialize_error;
																}
															}
														}
														default:
														{
															temp = (const ::rux::byte*)"incorrect syntax in XML declaration, 'encoding' or 'standalone' is not found";
															goto serializer_deserialize_error;
														}
														}
													}
													default:
													{
														temp = (const ::rux::byte*)"incorrect syntax in XML declaration, 'encoding' or 'standalone' is not found";
														goto serializer_deserialize_error;
													}
													}
												}
												default:
												{
													temp = (const ::rux::byte*)"incorrect syntax in XML declaration, 'encoding' or 'standalone' is not found";
													goto serializer_deserialize_error;
												}
												}
											}
											default:
											{
												temp = (const ::rux::byte*)"incorrect syntax in XML declaration, 'encoding' or 'standalone' is not found";
												goto serializer_deserialize_error;
											}
											}
										}
										default:
										{
											temp = (const ::rux::byte*)"incorrect syntax in XML declaration, 'encoding' or 'standalone' is not found";
											goto serializer_deserialize_error;
										}
										}
									}
									default:
									{
										temp = (const ::rux::byte*)"incorrect syntax in XML declaration, 'encoding' or 'standalone' is not found";
										goto serializer_deserialize_error;
									}
									}
								}
								default:
								{
									temp = (const ::rux::byte*)"incorrect syntax in XML declaration, 'encoding' or 'standalone' is not found";
									goto serializer_deserialize_error;
								}
								}
							}
						case 's':
							{
xml_declaration_maybe_standalone_parse:
								switch( *++ptr )
								{
								case 't':
									{
										if( *++ptr == 'a' )
										{
											if( *++ptr == 'n' )
											{
												if( *++ptr == 'd' )
												{
													if( *++ptr == 'a' )
													{
														if( *++ptr == 'l' )
														{
															if( *++ptr == 'o' )
															{
																if( *++ptr == 'n' )
																{
																	if( *++ptr == 'e' )
																	{
																		step = ::rux::data::xml::deserialize_goto_xml_declaration_standalone;
																		ptr++;
																		goto eq_parse;
xml_declaration_standalone_parse:
																		switch( *ptr )
																		{
																		case '\'':
																			{
																				switch( *++ptr )
																				{
																				case 'y':
																					{
																						if( *++ptr == 'e' )
																						{
																							if( *++ptr == 's' )
																							{
																								if( *++ptr != '\'' )
																								{
																									temp = (const ::rux::byte*)"incorrect syntax in XML declaration standalone, ''' is not found";
																									goto serializer_deserialize_error;
																								}
																								else
																								{
																									::memcpy( &xml[ xml_index ] , " standalone='yes\0" , 17 );

																									xml[ xml_index + 16 ] = 0x07;

																									xml_index += 13;
																									xml_declaration->xml_decl.standalone = &xml[ xml_index ];
																									xml_index += 4;
																									goto xml_declaration_standalone_parse_success;
																								}
																							}
																							else
																							{
																								temp = (const ::rux::byte*)"incorrect syntax in XML declaration standalone, 'yes' or 'no' is not found";
																								goto serializer_deserialize_error;
																							}
																						}
																						else
																						{
																							temp = (const ::rux::byte*)"incorrect syntax in XML declaration standalone, 'yes' or 'no' is not found";
																							goto serializer_deserialize_error;
																						}
																					}
																				case 'n':
																					{
																						if( *++ptr == 'o' )
																						{
																							if( *++ptr != '\'' )
																							{
																								temp = (const ::rux::byte*)"incorrect syntax in XML declaration standalone, ''' is not found";
																								goto serializer_deserialize_error;
																							}
																							else
																							{
																								::memcpy( &xml[ xml_index ] , " standalone='no\0" , 16 );

																								xml[ xml_index + 15 ] = 0x07;

																								xml_index += 13;
																								xml_declaration->xml_decl.standalone = &xml[ xml_index ];
																								xml_index += 3;
																								goto xml_declaration_standalone_parse_success;
																							}
																						}
																						else
																						{
																							temp = (const ::rux::byte*)"incorrect syntax in XML declaration standalone, 'yes' or 'no' is not found";
																							goto serializer_deserialize_error;
																						}
																					}
																				default:
																					{
																						temp = (const ::rux::byte*)"incorrect syntax in XML declaration standalone, 'yes' or 'no' is not found";
																						goto serializer_deserialize_error;
																					}
																				}															
																				break;
																			}
																		case '"':
																			{
																				switch( *++ptr )
																				{
																				case 'y':
																					{
																						if( *++ptr == 'e' )
																						{
																							if( *++ptr == 's' )
																							{
																								if( *++ptr != '"' )
																								{
																									temp = (const ::rux::byte*)"incorrect syntax in XML declaration standalone, '\"' is not found";
																									goto serializer_deserialize_error;
																								}
																								else
																								{
																									::memcpy( &xml[ xml_index ] , " standalone=\"yes\0" , 17 );

																									xml[ xml_index + 16 ] = 0x07;

																									xml_index += 13;
																									xml_declaration->xml_decl.standalone = &xml[ xml_index ];
																									xml_index += 4;
																									goto xml_declaration_standalone_parse_success;
																								}
																							}
																							else
																							{
																								temp = (const ::rux::byte*)"incorrect syntax in XML declaration standalone, 'yes' or 'no' is not found";
																								goto serializer_deserialize_error;
																							}
																						}
																						else
																						{
																							temp = (const ::rux::byte*)"incorrect syntax in XML declaration standalone, 'yes' or 'no' is not found";
																							goto serializer_deserialize_error;
																						}
																					}
																				case 'n':
																					{
																						if( *++ptr == 'o' )
																						{
																							if( *++ptr != '"' )
																							{
																								temp = (const ::rux::byte*)"incorrect syntax in XML declaration standalone, '\"' is not found";
																								goto serializer_deserialize_error;
																							}
																							else
																							{
																								::memcpy( &xml[ xml_index ] , " standalone='no\0" , 16 );

																								xml[ xml_index + 15 ] = 0x07;

																								xml_index += 13;
																								xml_declaration->xml_decl.standalone = &xml[ xml_index ];
																								xml_index += 3;
																								goto xml_declaration_standalone_parse_success;
																							}
																						}
																						else
																						{
																							temp = (const ::rux::byte*)"incorrect syntax in XML declaration standalone, 'yes' or 'no' is not found";
																							goto serializer_deserialize_error;
																						}
																					}
																				default:
																					{
																						temp = (const ::rux::byte*)"incorrect syntax in XML declaration standalone, 'yes' or 'no' is not found";
																						goto serializer_deserialize_error;
																					}
																				}
																			}
																		default:
																			{
																				temp = (const ::rux::byte*)"incorrect syntax in XML declaration standalone, ''' or '\"' is not found";
																				goto serializer_deserialize_error;
																			}
																		}
xml_declaration_standalone_parse_success:
																		switch( _xml_deserialize_table_white_space[ *++ptr ] )
																		{
																		case ' ':
																			{
																				switch( *++ptr )
																				{
																				case '?':
																					goto xml_declaration_end_parse;
																				default:
																					{
																						temp = (const ::rux::byte*)"incorrect syntax in XML declaration, ' ' or '?' is not found";
																						goto serializer_deserialize_error;
																					}
																				}
																			}
																		case '?':
																			goto xml_declaration_end_parse;
																		default:
																			{
																				temp = (const ::rux::byte*)"incorrect syntax in XML declaration, ' ' or '?' is not found";
																				goto serializer_deserialize_error;
																			}
																		}																		
																	}
																	else
																	{
																		temp = (const ::rux::byte*)"incorrect syntax in XML declaration standalone";
																		goto serializer_deserialize_error;
																	}
																}
																else
																{
																	temp = (const ::rux::byte*)"incorrect syntax in XML declaration standalone";
																	goto serializer_deserialize_error;
																}
															}
															else
															{
																temp = (const ::rux::byte*)"incorrect syntax in XML declaration, 'encoding' or 'standalone' is not found";
																goto serializer_deserialize_error;
															}
														}
														else
														{
															temp = (const ::rux::byte*)"incorrect syntax in XML declaration standalone";
															goto serializer_deserialize_error;
														}
													}
													else
													{
														temp = (const ::rux::byte*)"incorrect syntax in XML declaration standalone";
														goto serializer_deserialize_error;
													}
												}
												else
												{
													temp = (const ::rux::byte*)"incorrect syntax in XML declaration standalone";
													goto serializer_deserialize_error;
												}
											}
											else
											{
												temp = (const ::rux::byte*)"incorrect syntax in XML declaration standalone";
												goto serializer_deserialize_error;
											}
										}
										else
										{
											temp = (const ::rux::byte*)"incorrect syntax in XML declaration standalone";
											goto serializer_deserialize_error;
										}
									}
								default:
									{
										temp = (const ::rux::byte*)"incorrect syntax in XML declaration standalone";
										goto serializer_deserialize_error;
									}
								}
							}
						case '?':
							goto xml_declaration_end_parse;
						default:
							{
								temp = (const ::rux::byte*)"incorrect syntax in XML declaration, 'encoding' or 'standalone' is not found";
								goto serializer_deserialize_error;
							}
						}
					}
				case '?':
					{
xml_declaration_end_parse:
						switch( *++ptr )
						{
						case '>':
							{
								::memcpy( &xml[ xml_index ] , "?>" , 2 );
								xml_index += 2;
								goto xml_after_xml_declaration_parse;
							}
						default:
							{
								temp = (const ::rux::byte*)"incorrect syntax in XML declaration, '>' is not found";
								goto serializer_deserialize_error;
							}
						}
					}
				default:
					{
						temp = (const ::rux::byte*)"incorrect syntax in XML declaration, ' ' or '?' is not found";
						goto serializer_deserialize_error;
					}
				}

xml_after_xml_declaration_parse:
				switch( _xml_deserialize_table_white_space[ *++ptr ] )
				{
				case '<':
					{
						switch( *++ptr )
						{
						case '?':
							{
								step = ::rux::data::xml::deserialize_goto_xml_after_xml_declaration_parse;
								goto processing_instructions_parse;
							}
						case '!':
							{
								switch( *++ptr )
								{
								case '-':
									{
										switch( *++ptr )
										{
										case '-':
											{
												ptr++;
												step = ::rux::data::xml::deserialize_goto_xml_after_xml_declaration_parse;
												prev_step = step;
												goto comment_parse;
											}
										default:
											{
												temp = (const ::rux::byte*)"incorrect syntax in XML comment, '<!--' is not found";
												goto serializer_deserialize_error;
											}
										}
									}
								default:
									goto document_type_declaration_parse;
								}
							}
						default:
							goto element_parse;
						}
						goto xml_after_xml_declaration_parse;
					}
				case 0:
					{	
						temp = (const ::rux::byte*)"unexpected end of XML";
						goto serializer_deserialize_error;
					}
				case ' ':
					goto xml_after_xml_declaration_parse;
				default:
					{
						temp = (const ::rux::byte*)"incorrect syntax in XML document";
						goto serializer_deserialize_error;
					}
				}


document_type_declaration_parse:

				goto serializer_deserialize_main;
element_content_parse:
element_content_parse_cycle:
				for( ; ; )
				{
					switch( *ptr )
					{
					case '&':
						{
							switch( *++ptr )
							{
							case '#':
								{
									elem_child = (::rux::data::xml::node*)node_memory_alloc( sizeof( ::rux::data::xml::node ) , node_memory );
									elem_child->parent = elem;
									elem_child->type = ::rux::data::xml::node_type_element_content;
									elem_child->next = 0;
									elem_child->elem_content.content = (char*)&xml[ xml_index ];
element_content_parse_decimal_or_hex:
									switch( *++ptr )
									{
									case 'x':
										{
											temp = ++ptr;
											utf8_character = ::rux::utils::hex_string_to_number< ::rux::uint32 >( ptr );
											if( temp == ptr )
											{
												temp = (const ::rux::byte*)"hex string is empty in XML element attribute value";
												goto serializer_deserialize_error;
											}
											else if( *ptr != ';' )
											{
												temp = (const ::rux::byte*)"incorrect hex string syntax in XML element attribute value";
												goto serializer_deserialize_error;
											}
											else
											{
												if( utf8_character >= sizeof( ::rux::data::xml::_Char ) 
													|| ::rux::data::xml::_Char[ utf8_character ] == 0 )
												{
													temp = (const ::rux::byte*)"not valid Char";
													goto serializer_deserialize_error;
												}
												else
												{
													ptr++;
													temp = ptr;
													goto element_content_parse_after_validate_CHAR;
												}
											}
										}
									case 0:
										{
											temp = (const ::rux::byte*)"unexpected end of XML";
											goto serializer_deserialize_error;
										}
									default:
										{
											temp = ++ptr;
											utf8_character = ::rux::utils::decimal_string_to_number< ::rux::uint32 >( ptr );
											if( temp == ptr )
											{
												temp = (const ::rux::byte*)"incorrect decimal string in XML element attribute value";
												goto serializer_deserialize_error;
											}
											else if( *ptr != ';' )
											{
												temp = (const ::rux::byte*)"incorrect decimal string syntax in XML element attribute value";
												goto serializer_deserialize_error;
											}
											else
											{
												if( utf8_character >= sizeof( ::rux::data::xml::_Char ) 
													|| ::rux::data::xml::_Char[ utf8_character ] == 0 )
												{
													temp = (const ::rux::byte*)"not valid Char";
													goto serializer_deserialize_error;
												}
												else
												{
													ptr++;
													temp = ptr;
													goto element_content_parse_after_validate_CHAR;
												}
											}
										}
									}
								}
							case 0:
								{
									temp = (const ::rux::byte*)"unexpected end of XML";
									goto serializer_deserialize_error;
								}
							default:
								{
element_content_parse_entity_reference:
									step = ::rux::data::xml::deserialize_goto_element_content_parse_return_from_name_parse;
									goto name_parse;
element_content_parse_return_from_name_parse:
									if( *ptr != ';' )
									{
										temp = (const ::rux::byte*)"incorrect entity reference syntax in XML element content";
										goto serializer_deserialize_error;
									}
									else
										ptr++;
								}
							}
							break;
						}
					case '<':
						{
							switch( *++ptr )
							{
							case '/':
								{
element_content_parse_end_tag:
									::memcpy( &xml[ xml_index ] , "</" , 2 );
									xml_index += 2;

									temp = ++ptr;

									step = ::rux::data::xml::deserialize_goto_element_end_tag_parse;
									goto name_parse;
element_end_tag_parse:
									switch( _xml_deserialize_table_white_space[ *ptr ] )
									{
									case ' ':
										{
											switch( *++ptr )
											{
											case '>':
												{
													xml[ xml_index++ ] = '>';
													if( elem->parent == 0 )
													{
														root = elem;
														goto serializer_deserialize_stop;
													}
													else
													{
														if( elem->parent->elem.count >= node_childs_count )
														{
															elem->parent->elem.count++;
															if( elem->parent->elem.next == 0 )
																elem->parent->elem.next = elem;
															else
																elem->parent->elem.last->next = elem;
															elem->parent->elem.last = elem;
														}
														else
															elem->parent->elem.childs[ elem->parent->elem.count++ ] = elem;
														elem = elem->parent;
													}
												}
											case 0:
												{
													temp = (const ::rux::byte*)"unexpected end of XML";
													goto serializer_deserialize_error;
												}
											default:
												{
													temp = (const ::rux::byte*)"incorrect syntax in XML element end tag";
													goto serializer_deserialize_error;
												}
											}
											break;
										}
									case '>':
										{
											xml[ xml_index++ ] = '>';
											if( elem->parent == 0 )
											{
												root = elem;
												goto serializer_deserialize_stop;
											}
											else
											{
												if( elem->parent->elem.count >= node_childs_count )
												{
													elem->parent->elem.count++;
													if( elem->parent->elem.next == 0 )
														elem->parent->elem.next = elem;
													else
														elem->parent->elem.last->next = elem;
													elem->parent->elem.last = elem;
												}
												else
													elem->parent->elem.childs[ elem->parent->elem.count++ ] = elem;
												elem = elem->parent;
											}
											break;
										}
									case 0:
										{
											temp = (const ::rux::byte*)"unexpected end of XML";
											goto serializer_deserialize_error;
										}
									default:
										{
											temp = (const ::rux::byte*)"incorrect syntax in XML element end tag";
											goto serializer_deserialize_error;
										}
									}
									break;
								}
							case '!':
								{
									switch( *++ptr )
									{
									case '-':
										{
element_content_parse_comment:
											if( *++ptr == '-' )
											{
												ptr++;
												step = ::rux::data::xml::deserialize_goto_element_content_parse;
												prev_step = step;
												for( ; ; )
												{
comment_parse:
													step = prev_step;
													switch( *ptr )
													{
													case '-':
														{
															ptr++;
															switch( *ptr )
															{
															case '-':
																{
																	switch( *++ptr )
																	{
																	case '>':
																		{
																			//SUCCESS
																			switch( step )
																			{
																			case ::rux::data::xml::deserialize_goto_element_content_parse:
																				{
																					ptr++;
																					goto element_content_parse;
																				}
																			case ::rux::data::xml::deserialize_goto_xml_after_xml_declaration_parse:
																				goto xml_after_xml_declaration_parse;
																			}
																		}
																	case 0:
																		{
																			temp = (const ::rux::byte*)"unexpected end of XML";
																			goto serializer_deserialize_error;												
																		}
																	default:
																		{
																			temp = (const ::rux::byte*)"String \"--\" is not allowed within comments";
																			goto serializer_deserialize_error;	
																		}
																	}
																}
															case 0:
																{
																	temp = (const ::rux::byte*)"unexpected end of XML";
																	goto serializer_deserialize_error;												
																}
															default:
																{
																	prev_step = step;
																	step = ::rux::data::xml::deserialize_goto_comment_parse;
																	goto validate_CHAR;
																}
															}
														}
													case 0:
														{
															temp = (const ::rux::byte*)"unexpected end of XML";
															goto serializer_deserialize_error;												
														}
													default:
														{															
															prev_step = step;
															step = ::rux::data::xml::deserialize_goto_comment_parse;
															goto validate_CHAR;
														}
													}
												}
											}
											else
											{
												temp = (const ::rux::byte*)"not valid '<' in XML element content";
												goto serializer_deserialize_error;
											}
											break;
										}
									case '[':
										{
element_content_parse_CDATA:
											if( *++ptr == 'C' )
											{
												if( *++ptr == 'D' )
												{
													if( *++ptr == 'A' )
													{
														if( *++ptr == 'T' )
														{
															if( *++ptr == 'A' )
															{
																if( *++ptr == '[' )
																{
																	ptr++;
CDATA_parse:
																	switch( *ptr )
																	{
																	case ']':
																		{
																			ptr++;
																			switch( *ptr )
																			{
																			case ']':
																				{
																					ptr++;
																					switch( *ptr )
																					{
																					case '>':
																						{
																							ptr++;
																							temp = ptr;
																							goto element_content_parse_after_validate_CHAR;
																						}
																					case 0:
																						{
																							temp = (const ::rux::byte*)"unexpected end of XML";
																							goto serializer_deserialize_error;												
																						}
																					default:
																						{
																							step = ::rux::data::xml::deserialize_goto_CDATA_parse;
																							goto validate_CHAR;
																						}
																					}
																				}
																			case 0:
																				{
																					temp = (const ::rux::byte*)"unexpected end of XML";
																					goto serializer_deserialize_error;												
																				}
																			default:
																				{
																					step = ::rux::data::xml::deserialize_goto_CDATA_parse;
																					goto validate_CHAR;
																				}
																			}
																		}
																	case 0:
																		{
																			temp = (const ::rux::byte*)"unexpected end of XML";
																			goto serializer_deserialize_error;												
																		}
																	default:
																		{
																			step = ::rux::data::xml::deserialize_goto_CDATA_parse;
																			goto validate_CHAR;
																		}
																	}
																}
																else
																{
																	temp = (const ::rux::byte*)"not valid '<' in XML element content";
																	goto serializer_deserialize_error;
																}
															}
															else
															{
																temp = (const ::rux::byte*)"not valid '<' in XML element content";
																goto serializer_deserialize_error;
															}
														}
														else
														{
															temp = (const ::rux::byte*)"not valid '<' in XML element content";
															goto serializer_deserialize_error;
														}
													}
													else
													{
														temp = (const ::rux::byte*)"not valid '<' in XML element content";
														goto serializer_deserialize_error;
													}
												}
												else
												{
													temp = (const ::rux::byte*)"not valid '<' in XML element content";
													goto serializer_deserialize_error;
												}
											}
											else
											{
												temp = (const ::rux::byte*)"not valid '<' in XML element content";
												goto serializer_deserialize_error;
											}
											break;
										}
									case 0:
										{
											temp = (const ::rux::byte*)"unexpected end of XML";
											goto serializer_deserialize_error;
										}
									default:
										{
											temp = (const ::rux::byte*)"not valid '<' in XML element content";
											goto serializer_deserialize_error;
										}
									}
									break;
								}
							case '?':
								{
element_content_parse_processing_instruction:
									step = ::rux::data::xml::deserialize_goto_element_content_parse;
processing_instructions_parse:
									::memcpy( &xml[ xml_index ] , "<?" , 2 );
									xml_index += 2;
									ptr++;
									prev_step = step;
									step = ::rux::data::xml::deserialize_goto_element_content_processing_instructions_parse;
									goto name_parse;
element_content_processing_instructions_parse:
									step = prev_step;
									switch( _xml_deserialize_table_white_space[ *ptr++ ] )
									{
									case ' ':
										{
											temp = ptr;
											for( ; ; )
											{
element_content_processing_instructions_char_data_parse:
												step = prev_step;
												switch( *ptr )
												{
												case '?':
													{
														ptr++;
														switch( *ptr )
														{
														case '>':
															{	
																temp_size = ( ptr - temp ) + 1;
																xml[ xml_index++ ] = ' ';
																::memcpy( &xml[ xml_index ] , temp , temp_size );
																xml_index += temp_size;
																switch( step )
																{
																case ::rux::data::xml::deserialize_goto_element_content_parse:
																	{
																		ptr++;
																		goto element_content_parse;
																	}
																case ::rux::data::xml::deserialize_goto_xml_after_xml_declaration_parse:
																	goto xml_after_xml_declaration_parse;
																}
															}
														case 0:
															{
																temp = (const ::rux::byte*)"unexpected end of XML";
																goto serializer_deserialize_error;												
															}
														default:
															{
																prev_step = step;
																step = ::rux::data::xml::deserialize_goto_element_content_processing_instructions_char_data_parse;
																goto validate_CHAR;														
															}
														}
													}
												case 0:
													{
														temp = (const ::rux::byte*)"unexpected end of XML";
														goto serializer_deserialize_error;												
													}
												default:
													{
														prev_step = step;
														step = ::rux::data::xml::deserialize_goto_element_content_processing_instructions_char_data_parse;
														goto validate_CHAR;
													}
												}
											}
										}
									case '?':
										{
											switch( *ptr )
											{
											case '>':
												{		
													::memcpy( &xml[ xml_index ] , "?>" , 2 );
													xml_index += 2;
													switch( step )
													{
													case ::rux::data::xml::deserialize_goto_element_content_parse:
														{
															ptr++;
															goto element_content_parse;
														}
													case ::rux::data::xml::deserialize_goto_xml_after_xml_declaration_parse:
														goto xml_after_xml_declaration_parse;
													}
												}
											default:
												{
													temp = (const ::rux::byte*)"incorrect syntax in XML processing instruction";
													goto serializer_deserialize_error;
												}
											}
										}
									default:
										{
											temp = (const ::rux::byte*)"incorrect syntax in XML processing instruction";
											goto serializer_deserialize_error;
										}
									}
									//Processing Instructions
									break;
								}
							case 0:
								{
									temp = (const ::rux::byte*)"unexpected end of XML";
									goto serializer_deserialize_error;
								}
							default:
								goto element_parse;
							}
							break;
						}
					case 0:
						{
							temp = (const ::rux::byte*)"unexpected end of XML";
							goto serializer_deserialize_error;
						}
					default:
						{
							elem_child = (::rux::data::xml::node*)node_memory_alloc( sizeof( ::rux::data::xml::node ) , node_memory );
							elem_child->parent = elem;
							elem_child->type = ::rux::data::xml::node_type_element_content;
							elem_child->next = 0;
							elem_child->elem_content.content = (char*)&xml[ xml_index ];

							temp = ptr;

							step = ::rux::data::xml::deserialize_goto_element_content_parse_after_validate_CHAR;
							goto validate_CHAR;
element_content_parse_after_validate_CHAR:
							switch( *ptr )
							{
							case '&':
								{
									temp_size = ( ptr - temp );
									if( temp_size )
									{
										::memcpy( &xml[ xml_index ] , temp , temp_size );
										xml_index += temp_size;
									}

									switch( *++ptr )
									{
									case '#':
										goto element_content_parse_decimal_or_hex;
									case 0:
										{
											temp = (const ::rux::byte*)"unexpected end of XML";
											goto serializer_deserialize_error;
										}
									default:
										{
											xml[ xml_index++ ] = 0x07;
											elem_child->elem_content.replace_char = '&';

											step = ::rux::data::xml::deserialize_goto_element_content_parse_entity_reference;
											goto goto_add_elem_child_to_elem;
										}
									}
								}
							case '<':
								{
									temp_size = ( ptr - temp );
									if( temp_size )
									{
										::memcpy( &xml[ xml_index ] , temp , temp_size );
										xml_index += temp_size;
									}


									switch( *++ptr )
									{
									case '/':
										{
											xml[ xml_index++ ] = 0x07;
											elem_child->elem_content.replace_char = '<';

											step = ::rux::data::xml::deserialize_goto_element_content_parse_end_tag;
											goto goto_add_elem_child_to_elem;
										}
									case '!':
										{
											switch( *++ptr )
											{
											case '-':
												{
													step = ::rux::data::xml::deserialize_goto_element_content_parse_comment;
													goto goto_add_elem_child_to_elem;
												}
											case '[':
												goto element_content_parse_CDATA;
											case 0:
												{
													temp = (const ::rux::byte*)"unexpected end of XML";
													goto serializer_deserialize_error;
												}
											default:
												{
													temp = (const ::rux::byte*)"not valid '<' in XML element content";
													goto serializer_deserialize_error;
												}
											}
											break;
										}
									case '?':
										{
											xml[ xml_index++ ] = 0x07;
											elem_child->elem_content.replace_char = '<';

											step = ::rux::data::xml::deserialize_goto_element_content_parse_processing_instruction;
											goto goto_add_elem_child_to_elem;
										}
									case 0:
										{
											temp = (const ::rux::byte*)"unexpected end of XML";
											goto serializer_deserialize_error;
										}
									default:
										{
											xml[ xml_index++ ] = 0x07;
											elem_child->elem_content.replace_char = '&';

											step = ::rux::data::xml::deserialize_goto_element_parse;
											goto goto_add_elem_child_to_elem;
										}
									}
								}
							default:
								{
									step = ::rux::data::xml::deserialize_goto_element_content_parse_after_validate_CHAR;
									goto validate_CHAR;
								}
							}
						}
					}
					ptr++;
				}
validate_CHAR:
				if( ::rux::utils::string::utf8::validate_character_with_utf8_character( ptr , utf8_character_bytes ) )
				{
					/*if( utf8_character >= sizeof( ::rux::data::xml::_Char ) 
						|| ::rux::data::xml::_Char[ utf8_character ] == 0 )
					{
						temp = (const ::rux::byte*)"not valid Char";
						goto serializer_deserialize_error;
					}*/
				}
				else
				{
					temp = (const ::rux::byte*)"is not utf8";
					goto serializer_deserialize_error;
				}
				switch( step )
				{
				case deserialize_goto_element_content_parse_after_validate_CHAR:
					goto element_content_parse_after_validate_CHAR;
				case ::rux::data::xml::deserialize_goto_comment_parse:
					goto comment_parse;
				case ::rux::data::xml::deserialize_goto_CDATA_parse:
					goto CDATA_parse;
				case ::rux::data::xml::deserialize_goto_element_content_parse:
					goto element_content_parse;
				case ::rux::data::xml::deserialize_goto_attribute_value_parse_quote:
					goto attribute_value_parse_quote;
				case ::rux::data::xml::deserialize_goto_attribute_value_parse_doublequote:
					goto attribute_value_parse_doublequote;
				case ::rux::data::xml::deserialize_goto_element_content_processing_instructions_parse:
					goto element_content_processing_instructions_parse;
				case ::rux::data::xml::deserialize_goto_element_content_processing_instructions_char_data_parse:
					goto element_content_processing_instructions_char_data_parse;
				};
name_parse:
				temp = ptr;
				if( ::rux::utils::string::utf8::validate_character_with_utf8_character( ptr , utf8_character_bytes ) )
				{
					if( utf8_character < sizeof( ::rux::data::xml::_NameStartChar ) && ::rux::data::xml::_NameStartChar[ utf8_character ] )
					{
						for( ; ; )
						{
							if( _xml_deserialize_NameChar_table_0[ *ptr ] == ' ' )
							{
								temp_size = ptr - temp;
								::memcpy( &xml[ xml_index ] , temp , temp_size );
								xml_index += temp_size;
								switch( step )
								{
								case ::rux::data::xml::deserialize_goto_element_attributes_parse:
									xml[ xml_index++ ] = 0x07;
								}
								break;
							}
							if( ::rux::utils::string::utf8::validate_character_with_utf8_character( ptr , utf8_character_bytes ) )
							{
								if( utf8_character >= sizeof( ::rux::data::xml::_NameChar ) 
									|| ::rux::data::xml::_NameChar[ utf8_character ] == 0 )
								{
									temp = (const ::rux::byte*)"not valid NameChar";
									goto serializer_deserialize_error;
								}
							}
							else
							{
								temp = (const ::rux::byte*)"is not utf8";
								goto serializer_deserialize_error;
							}
						}
					}
					else
					{
						temp = (const ::rux::byte*)"not valid NameStartChar";
						goto serializer_deserialize_error;
					}
				}
				else
				{
					temp = (const ::rux::byte*)"is not utf8";
					goto serializer_deserialize_error;
				}
				switch( step )
				{
				case ::rux::data::xml::deserialize_goto_attribute_value_quote_return_from_name_parse:
					goto attribute_value_quote_return_from_name_parse;
				case ::rux::data::xml::deserialize_goto_attribute_value_doublequote_return_from_name_parse:
					goto attribute_value_doublequote_return_from_name_parse;
				case ::rux::data::xml::deserialize_goto_element_attributes_parse:
					goto element_attributes_parse;
				case ::rux::data::xml::deserialize_goto_eq_parse:
					{
						step = ::rux::data::xml::deserialize_goto_attribute_value_parse;
						goto eq_parse;
					}
				case ::rux::data::xml::deserialize_goto_element_end_tag_parse:
					goto element_end_tag_parse;
				case ::rux::data::xml::deserialize_goto_element_content_parse_return_from_name_parse:
					goto element_content_parse_return_from_name_parse;
				case ::rux::data::xml::deserialize_goto_element_content_processing_instructions_parse:
					goto element_content_processing_instructions_parse;
				};
attribute_parse:
				xml[ xml_index++ ] = ' ';
				elem_child = (::rux::data::xml::node*)node_memory_alloc( sizeof( ::rux::data::xml::node ) , node_memory );
				elem_child->parent = elem;
				elem_child->type = ::rux::data::xml::node_type_attribute;
				elem_child->next = 0;
				elem_child->attr.name = (char*)&xml[ xml_index ];
				step = ::rux::data::xml::deserialize_goto_eq_parse;
				goto name_parse;
eq_parse:
				switch( _xml_deserialize_table_white_space[ *ptr ] )
				{
				case '=':
					{
						if( *++ptr == ' ' )
							ptr++;
						switch( step )
						{
						case ::rux::data::xml::deserialize_goto_attribute_value_parse:
							{
								xml[ xml_index++ ] = 0x07;
								break;
							}
						default:
							xml[ xml_index++ ] = '=';
						}
						break;
					}
				case ' ':
					{
						switch( _xml_deserialize_table_white_space[ *++ptr ] )
						{
						case '=':
							{
								if( *++ptr == ' ' )
									ptr++;
								switch( step )
								{
								case ::rux::data::xml::deserialize_goto_attribute_value_parse:
									{
										xml[ xml_index++ ] = 0x07;
										break;
									}
								default:
									xml[ xml_index++ ] = '=';
								}
								break;
							}
						case 0:
							{
								temp = (const ::rux::byte*)"unexpected end of XML";
								goto serializer_deserialize_error;
							}
						default:
							{
								temp = (const ::rux::byte*)"incorrect symbol in XML element attribute";
								goto serializer_deserialize_error;
							}
						};
					}
				case 0:
					{
						temp = (const ::rux::byte*)"unexpected end of XML";
						goto serializer_deserialize_error;
					}
				default:
					{
						temp = (const ::rux::byte*)"incorrect symbol in XML element attribute";
						goto serializer_deserialize_error;
					}
				};
				switch( step )
				{
				case ::rux::data::xml::deserialize_goto_attribute_value_parse:
					goto attribute_value_parse;
				case ::rux::data::xml::deserialize_goto_xml_declaration_version_info:
					goto xml_declaration_version_info;
				case ::rux::data::xml::deserialize_goto_xml_declaration_encoding:
					goto xml_declaration_encoding_parse;
				case ::rux::data::xml::deserialize_goto_xml_declaration_standalone:
					goto xml_declaration_standalone_parse;
				}
attribute_value_parse:
				switch( *ptr )
				{
				case '\'':
					{
						ptr++;
						for( ; ; )
						{
attribute_value_parse_quote:
							switch( *ptr )
							{
							case '&':
								{
									switch( *++ptr )
									{
									case '#':
										{
											switch( *++ptr )
											{
											case 'x':
												{
													temp = ++ptr;
													utf8_character = ::rux::utils::hex_string_to_number< ::rux::uint32 >( ptr );
													if( temp == ptr )
													{
														temp = (const ::rux::byte*)"hex string is empty in XML element attribute value";
														goto serializer_deserialize_error;
													}
													else if( *ptr != ';' )
													{
														temp = (const ::rux::byte*)"incorrect hex string syntax in XML element attribute value";
														goto serializer_deserialize_error;
													}
													else
													{
														if( utf8_character >= sizeof( ::rux::data::xml::_Char ) 
															|| ::rux::data::xml::_Char[ utf8_character ] == 0 )
														{
															temp = (const ::rux::byte*)"not valid Char";
															goto serializer_deserialize_error;
														}
														else
															ptr++;
													}
												}
											case 0:
												{
													temp = (const ::rux::byte*)"unexpected end of XML";
													goto serializer_deserialize_error;
												}
											default:
												{
													temp = ++ptr;
													utf8_character = ::rux::utils::decimal_string_to_number< ::rux::uint32 >( ptr );
													if( temp == ptr )
													{
														temp = (const ::rux::byte*)"incorrect decimal string in XML element attribute value";
														goto serializer_deserialize_error;
													}
													else if( *ptr != ';' )
													{
														temp = (const ::rux::byte*)"incorrect decimal string syntax in XML element attribute value";
														goto serializer_deserialize_error;
													}
													else
													{
														if( utf8_character >= sizeof( ::rux::data::xml::_Char ) 
															|| ::rux::data::xml::_Char[ utf8_character ] == 0 )
														{
															temp = (const ::rux::byte*)"not valid Char";
															goto serializer_deserialize_error;
														}
														else
															ptr++;
													}
												}
											}
										}
									case 0:
										{
											temp = (const ::rux::byte*)"unexpected end of XML";
											goto serializer_deserialize_error;
										}
									default:
										{
											step = ::rux::data::xml::deserialize_goto_attribute_value_quote_return_from_name_parse;
											goto name_parse;
attribute_value_quote_return_from_name_parse:
											if( *ptr != ';' )
											{
												temp = (const ::rux::byte*)"incorrect entity reference syntax in XML element attribute value";
												goto serializer_deserialize_error;
											}
											else
												ptr++;
										}
									}
								}
							case '<':
								{
									temp = (const ::rux::byte*)"not valid '<' in XML element attribute value";
									goto serializer_deserialize_error;
								}
							case '\'':
								{
									step = ::rux::data::xml::deserialize_goto_after_add_child1;
									goto goto_add_elem_child_to_elem;
after_add_child1:					
									ptr++;
									goto element_attributes_parse;
								}
							case 0:
								{
									temp = (const ::rux::byte*)"unexpected end of XML";
									goto serializer_deserialize_error;
								}
							default:
								{
									step = ::rux::data::xml::deserialize_goto_attribute_value_parse_quote;
									goto validate_CHAR;
								}
							}
						}

						break;
					}
				case '"':
					{
						ptr++;
						for( ; ; )
						{
attribute_value_parse_doublequote:
							switch( *ptr )
							{
							case '&':
								{
									switch( *++ptr )
									{
									case '#':
										{
											switch( *++ptr )
											{
											case 'x':
												{
													temp = ++ptr;
													utf8_character = ::rux::utils::hex_string_to_number< ::rux::uint32 >( ptr );
													if( temp == ptr )
													{
														temp = (const ::rux::byte*)"hex string is empty in XML element attribute value";
														goto serializer_deserialize_error;
													}
													else if( *ptr != ';' )
													{
														temp = (const ::rux::byte*)"incorrect hex string syntax in XML element attribute value";
														goto serializer_deserialize_error;
													}
													else
													{
														if( utf8_character >= sizeof( ::rux::data::xml::_Char ) 
															|| ::rux::data::xml::_Char[ utf8_character ] == 0 )
														{
															temp = (const ::rux::byte*)"not valid Char";
															goto serializer_deserialize_error;
														}
														else
															ptr++;
													}
												}
											case 0:
												{
													temp = (const ::rux::byte*)"unexpected end of XML";
													goto serializer_deserialize_error;
												}
											default:
												{
													temp = ++ptr;
													utf8_character = ::rux::utils::decimal_string_to_number< ::rux::uint32 >( ptr );
													if( temp == ptr )
													{
														temp = (const ::rux::byte*)"incorrect decimal string in XML element attribute value";
														goto serializer_deserialize_error;
													}
													else if( *ptr != ';' )
													{
														temp = (const ::rux::byte*)"incorrect decimal string syntax in XML element attribute value";
														goto serializer_deserialize_error;
													}
													else
													{
														if( utf8_character >= sizeof( ::rux::data::xml::_Char ) 
															|| ::rux::data::xml::_Char[ utf8_character ] == 0 )
														{
															temp = (const ::rux::byte*)"not valid Char";
															goto serializer_deserialize_error;
														}
														else
															ptr++;
													}
												}
											}
										}
									case 0:
										{
											temp = (const ::rux::byte*)"unexpected end of XML";
											goto serializer_deserialize_error;
										}
									default:
										{
											step = ::rux::data::xml::deserialize_goto_attribute_value_doublequote_return_from_name_parse;
											goto name_parse;
attribute_value_doublequote_return_from_name_parse:
											if( *ptr != ';' )
											{
												temp = (const ::rux::byte*)"incorrect entity reference syntax in XML element attribute value";
												goto serializer_deserialize_error;
											}
											else
												ptr++;
										}
									}
								}
							case '<':
								{
									temp = (const ::rux::byte*)"not valid '<' in XML element attribute value";
									goto serializer_deserialize_error;
								}
							case '"':
								{
									step = ::rux::data::xml::deserialize_goto_after_add_child2;
									goto goto_add_elem_child_to_elem;
after_add_child2:									
									ptr++;
									goto element_attributes_parse;
								}
							case 0:
								{
									temp = (const ::rux::byte*)"unexpected end of XML";
									goto serializer_deserialize_error;
								}
							default:
								{
									step = ::rux::data::xml::deserialize_goto_attribute_value_parse_doublequote;
									goto validate_CHAR;
								}
							}
						}
						break;
					}
				case 0:
					{
						temp = (const ::rux::byte*)"unexpected end of XML";
						goto serializer_deserialize_error;
					}
				default:
					{
						temp = (const ::rux::byte*)"incorrect symbol in XML element attribute";
						goto serializer_deserialize_error;
					}
				};
element_parse:
				xml[ xml_index++ ] = '<';
				prev_elem = elem;
				elem = (::rux::data::xml::node*)node_memory_alloc( sizeof( ::rux::data::xml::node ) , node_memory );
				elem->parent = prev_elem;
				elem->type = ::rux::data::xml::node_type_element;
				elem->next = 0;
				elem->elem.name = (char*)&xml[ xml_index ];
				elem->elem.count = 0;
				elem->elem.next = 0;
				elem->elem.last = 0;
				step = ::rux::data::xml::deserialize_goto_element_attributes_parse;
				goto name_parse;
element_attributes_parse:
				switch( _xml_deserialize_table_white_space[ *ptr ] )
				{
				case '/':
					{
						if( *++ptr == '>' )
						{
							::memcpy( &xml[ xml_index ] , "/>" , 2 );
							xml_index += 2;
							if( elem->parent == 0 )
							{
								root = elem;
								goto serializer_deserialize_stop;
							}
							else
							{
								if( elem->parent->elem.count >= ( sizeof( elem->parent->elem.childs ) / sizeof( elem->parent->elem.childs[ 0 ] ) ) )
								{
									elem->parent->elem.count++;
									if( elem->parent->elem.next == 0 )
										elem->parent->elem.next = elem;
									else
										elem->parent->elem.last->next = elem;
									elem->parent->elem.last = elem;
								}
								else
									elem->parent->elem.childs[ elem->parent->elem.count++ ] = elem;
								elem = elem->parent;
							}
							ptr++;
							goto element_content_parse;
						}
						else
						{
							temp = (const ::rux::byte*)"incorrect empty element tag";
							goto serializer_deserialize_error;
						}
						break;
					}
				case '>':
					{
						xml[ xml_index++ ] = '>';
						ptr++;
						goto element_content_parse;
					}
				case ' ':
					{
						switch( _xml_deserialize_table_white_space[ *++ptr ] )
						{
						case '/':
							{
								if( *++ptr == '>' )
								{
									::memcpy( &xml[ xml_index ] , "/>" , 2 );
									xml_index += 2;
									if( elem->parent == 0 )
									{
										root = elem;
										goto serializer_deserialize_stop;
									}
									else
									{
										if( elem->parent->elem.count >= ( sizeof( elem->parent->elem.childs ) / sizeof( elem->parent->elem.childs[ 0 ] ) ) )
										{
											elem->parent->elem.count++;
											if( elem->parent->elem.next == 0 )
												elem->parent->elem.next = elem;
											else
												elem->parent->elem.last->next = elem;
											elem->parent->elem.last = elem;
										}
										else
											elem->parent->elem.childs[ elem->parent->elem.count++ ] = elem;
										elem = elem->parent;
									}
									ptr++;
									goto element_content_parse;
								}
								else
								{
									temp = (const ::rux::byte*)"incorrect empty element tag";
									goto serializer_deserialize_error;
								}
							}
						case '>':
							{
								xml[ xml_index++ ] = '>';
								ptr++;
								goto element_content_parse;
							}
						case 0:
							{
								temp = (const ::rux::byte*)"unexpected end of XML";
								goto serializer_deserialize_error;
							}
						default:
							goto attribute_parse;
						}
					}
				case 0:
					{
						temp = (const ::rux::byte*)"unexpected end of XML";
						goto serializer_deserialize_error;
					}
				default:
					{
						temp = (const ::rux::byte*)"incorrect symbol in XML element";
						goto serializer_deserialize_error;
					}
				}
goto_add_elem_child_to_elem:
				if( elem_child->parent->elem.count >= ( sizeof( elem_child->parent->elem.childs ) / sizeof( elem_child->parent->elem.childs[ 0 ] ) ) )
				{
					elem_child->parent->elem.count++;
					if( elem_child->parent->elem.next == 0 )
						elem_child->parent->elem.next = elem_child;
					else
						elem_child->parent->elem.last->next = elem_child;
					elem_child->parent->elem.last = elem_child;
				}
				else
					elem_child->parent->elem.childs[ elem_child->parent->elem.count++ ] = elem_child;
				elem_child = 0;
				switch( step )
				{
				case ::rux::data::xml::deserialize_goto_element_content_parse_entity_reference:
					goto element_content_parse_entity_reference;
				case ::rux::data::xml::deserialize_goto_element_content_parse_end_tag:
					goto element_content_parse_end_tag;
				case ::rux::data::xml::deserialize_goto_element_content_parse_comment:
					goto element_content_parse_comment;
				case ::rux::data::xml::deserialize_goto_element_content_parse_processing_instruction:
					goto element_content_parse_processing_instruction;
				case ::rux::data::xml::deserialize_goto_element_parse:
					goto element_parse;
				case ::rux::data::xml::deserialize_goto_after_add_child1:
					goto after_add_child1;
				case ::rux::data::xml::deserialize_goto_after_add_child2:
					goto after_add_child2;
				}
serializer_deserialize_stop:
				return true;
serializer_deserialize_error:
				::rux::uint64 line = 1;
				::rux::uint64 column = 1;
				const ::rux::byte* begin = (const ::rux::byte*)utf8;
				while( begin != ptr )
				{
					if( *--ptr == '\n' )
					{
						line++;
						break;
					}
					else
						column++;
				}							
				while( begin != ptr )
				{
					if( *--ptr == '\n' )
						line++;
				}
				::rux::utils::string::safe_sprintf( error , 128 , "xml parse error \"%s\" at " I64u ":" I64u , temp , line , column );
				free();
				return false;
			};
		};
	};
};