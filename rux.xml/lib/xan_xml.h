#ifndef XAN_XML_H
#define XAN_XML_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#ifdef RUX_USE_HEADER_PREFIX
#include "rux_extern_includes/xan_utils.h"
#else
#include "xan_utils.h"
#endif
#include <stdlib.h>
#include <string.h>
namespace rux
{
	namespace data
	{
		namespace xml
		{
			enum deserialize_step
			{
				deserialize_goto_attribute_value_quote_return_from_name_parse ,
				deserialize_goto_attribute_value_doublequote_return_from_name_parse ,
				deserialize_goto_element_attributes_parse ,
				deserialize_goto_eq_parse , 
				deserialize_goto_element_end_tag_parse , 
				deserialize_goto_element_content_parse_return_from_name_parse , 
				deserialize_goto_comment_parse ,
				deserialize_goto_CDATA_parse ,
				deserialize_goto_element_content_parse_after_validate_CHAR ,
				deserialize_goto_element_content_parse ,
				deserialize_goto_attribute_value_parse_quote ,
				deserialize_goto_attribute_value_parse_doublequote , 
				deserialize_goto_element_content_processing_instructions_parse , 
				deserialize_goto_attribute_value_parse ,
				deserialize_goto_xml_declaration_version_info ,
				deserialize_goto_xml_declaration_encoding , 
				deserialize_goto_xml_declaration_standalone ,
				deserialize_goto_xml_after_xml_declaration_parse ,
				deserialize_goto_element_content_processing_instructions_char_data_parse ,
				deserialize_goto_element_content_parse_entity_reference , 
				deserialize_goto_element_content_parse_end_tag , 
				deserialize_goto_element_content_parse_comment ,
				deserialize_goto_element_content_parse_processing_instruction ,
				deserialize_goto_element_parse , 
				deserialize_goto_after_add_child1 ,
				deserialize_goto_after_add_child2
			};
			enum node_type
			{
				node_type_element ,
				node_type_element_content ,
				node_type_attribute ,
				node_type_xml_declaration ,
				node_type_document_type_declaration
			};
			struct node_memory
			{
				::rux::byte* nodes;
				size_t size;
				size_t offset;
				node_memory* next;
				node_memory* last;
			};
			rux_inline void node_memory_free( ::rux::data::xml::node_memory* node_memory )
			{
				::rux::data::xml::node_memory* next = 0;
				while( node_memory )
				{
					next = node_memory->next;
					free( node_memory->nodes );
					node_memory = next;
				}
			};
			rux_inline void node_memory_init( ::rux::data::xml::node_memory*& node_memory , size_t new_node_size )
			{
				if( node_memory == 0 )
				{
					new_node_size += sizeof( ::rux::data::xml::node_memory );
					void* nodes = malloc( new_node_size );
					node_memory = (::rux::data::xml::node_memory*)nodes;
					node_memory->last = node_memory;
					node_memory->last->nodes = (::rux::byte*)nodes;
					node_memory->last->size = new_node_size;
					node_memory->last->offset = sizeof( ::rux::data::xml::node_memory );			
					node_memory->last->next = 0;
				}
			};
			rux_inline void* node_memory_alloc( size_t size , ::rux::data::xml::node_memory*& node_memory )
			{
 				if( ( node_memory->last->size - node_memory->last->offset ) >= size )
				{
node_memory_alloc_begin:
					void* ptr = &node_memory->last->nodes[ node_memory->last->offset ];
					node_memory->last->offset += size;
					return ptr;
				}
				else
				{				
					size_t new_node_size = sizeof( ::rux::data::xml::node_memory ) + ( ( node_memory->last->size - sizeof( ::rux::data::xml::node_memory ) ) * 3 / 2 );
					void* nodes = malloc( new_node_size );
					node_memory->last->next = (::rux::data::xml::node_memory*)nodes;
					node_memory->last = node_memory->last->next;
					node_memory->last->nodes = (::rux::byte*)nodes;
					node_memory->last->size = new_node_size;
					node_memory->last->offset = sizeof( ::rux::data::xml::node_memory );			
					node_memory->last->next = 0;
					goto node_memory_alloc_begin;
				}				
				return 0;
			};
			const size_t node_childs_count = 20;
			struct node
			{				
				union
				{
					char* name;
					struct
					{
						char* name;
						size_t count;
						::rux::data::xml::node* childs[ node_childs_count ];
						::rux::data::xml::node* next;
						::rux::data::xml::node* last;
					} elem;
					struct
					{
						char* content;
						char replace_char;
					} elem_content;
					struct 
					{
						char* name;
					} attr;
					struct
					{
						char* version;
						char* encoding;
						char* standalone;
					} xml_decl;
				};
				node_type type;
				::rux::data::xml::node* parent;
				::rux::data::xml::node* next;
			};
			class document
			{
			private:
				char error[ 128 ];
				::rux::data::xml::node* xml_declaration;
				::rux::data::xml::node* document_declaration;
				::rux::data::xml::node* root;
				::rux::data::xml::node_memory* node_memory;
				char* xml;
			public:
				document( void );
				~document( void );
				bool deserialize( const char* utf8 );
				void free( void );
			};
		};
	};
};
#endif
