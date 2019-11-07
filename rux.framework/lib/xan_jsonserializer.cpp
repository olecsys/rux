#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_jsonserializer.h"
#include "xan_error.h"
#include "xan_directory.h"
namespace rux
{
	namespace data
	{
		char _json_deserialize_help_table[] = 
		{
			0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, ' ' , ' ' , ' ' , ' ' , ' ' , 0x01, 0x01,
			0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
			' ',  0x01, '"',  0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, '0', ',',  '0',  0x01, '/',
			'0',  '0',  '0',  '0',  '0',  '0',  '0',  '0',  '0',  '0',  ':',  0x01, 0x01, 0x01, 0x01, 0x01,
			0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
			0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, '[',  '\\', ']',  0x01, 0x01,
			0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 'f',  0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 'n',  0x01,
			0x01, 0x01, 0x01, 0x01, 't',  0x01, 0x01, 0x01, 0x01, 0x01, 0x01, '{',  0x01, '}',  0x01, 0x01,
			0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
			0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
			0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
			0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
			0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
			0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
			0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
			0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01
		};

#define rux_check_json_string_memory( json_begin , json_ptr_size , json_cursor , adding_size , step , __file , __line ) {\
		CODE_LABEL_START( 0 , 0 , 5 );\
		if( adding_size > 0 )\
		{\
			size_t json_index = json_cursor - json_begin;\
			if( json_begin == NULL || ( json_ptr_size - json_index ) < adding_size )\
			{\
				CODE_LABEL_START( 0 , 0 , 5 );\
				if( step < adding_size )\
					step = adding_size;\
				{\
					CODE_LABEL_START( 0 , 0 , 5 );\
					json_begin = ::rux::engine::realloc_array< char >( json_begin , json_ptr_size + step , __file , __line );\
					CODE_LABEL_END();\
				}\
				json_ptr_size += step;\
				if( step < RUX_ARRAY_MAX_STEP )\
					step += 1024;\
				json_cursor = &json_begin[ json_index ];\
				CODE_LABEL_END();\
			}\
		}\
		CODE_LABEL_END();\
	}
		namespace json
		{
			serializer::serializer( ::rux::byte formatted , ::rux::byte json_delete )
			{
				_json_delete = json_delete;
				_formatted = formatted;
				_json = 0;
				_json_cursor = 0;
				_step = 32;
				_json_size = 0;
				_tab_count = 0;
				_need_delimiter = 0;
			};
			serializer::~serializer( void )
			{
				if( _json_delete && _json )
					::rux::engine::free_mem( _json );
			};
			char* serializer::json( void )
			{
				return _json;
			};
			size_t serializer::json_size( void )
			{
				return _json_size;
			};
			void serializer::json_detach( ::rux::XString& dst )
			{
				if( _json )
					dst()->attach( _json , _json_size , ((::rux::byte*)_json_cursor) - ((::rux::byte*)_json) , XEnumCodePage_UTF8 );
				_json = 0;
				_json_cursor = 0;
				_step = 32;
				_json_size = 0;
				_tab_count = 0;
				_need_delimiter = 0;
			};
			void serializer::begin_object( const char* utf8_name , const char* __file__ , ::rux::int32 __line__ , size_t ___rux__thread_index1986 )
			{
				CODE_LABEL_START( 0 , 0 , 5 );
				if( check_formatted_and_name( utf8_name , __file__ , __line__ , ___rux__thread_index1986 ) && _formatted )
				{
					rux_check_json_string_memory( _json , _json_size , _json_cursor , _tab_count + 4 , _step , __file__ , __line__ );
					*_json_cursor++ = '\n';
					for( size_t index1 = 0 ; index1 < _tab_count ; index1++ )
						*_json_cursor++ = '\t';
				}
				else
					rux_check_json_string_memory( _json , _json_size , _json_cursor , 3 , _step , __FILE__ , __LINE__ );
				*_json_cursor++ = '{';
				if( _formatted == 1 )
				{
					*_json_cursor++ = '\n';
					_tab_count++;
				}
				*_json_cursor = 0;
				_states.Add( serializer_state_object );
				_need_delimiter = 0;
				CODE_LABEL_END();
			};
			void serializer::end_object( const char* __file__ , ::rux::int32 __line__ , size_t ___rux__thread_index1986 )
			{
				UNREFERENCED_PARAMETER( ___rux__thread_index1986 );
				CODE_LABEL_START( 0 , 0 , 5 );
				if( _formatted == 1 )
				{
					rux_check_json_string_memory( _json , _json_size , _json_cursor , _tab_count + 2 , _step , __FILE__ , __LINE__ );
					if( _need_delimiter == 1 )
						*_json_cursor++ = '\n';
					_tab_count--;					
					for( size_t index1 = 0 ; index1 < _tab_count ; index1++ )
						*_json_cursor++ = '\t';
				}
				else
					rux_check_json_string_memory( _json , _json_size , _json_cursor , 2 , _step , __FILE__ , __LINE__ );
				*_json_cursor++ = '}';
				*_json_cursor = 0;
				_states.RemoveAt( _states.Count() - 1 );
				_need_delimiter = 1;
				CODE_LABEL_END();
			};
			void serializer::begin_array( const char* utf8_name , const char* __file__ , ::rux::int32 __line__ , size_t ___rux__thread_index1986 )
			{
				CODE_LABEL_START( 0 , 0 , 5 );
				if( check_formatted_and_name( utf8_name , __file__ , __line__ , ___rux__thread_index1986 ) && _formatted )
				{
					rux_check_json_string_memory( _json , _json_size , _json_cursor , _tab_count + 4 , _step , __file__ , __line__ );
					*_json_cursor++ = '\n';
					for( size_t index1 = 0 ; index1 < _tab_count ; index1++ )
						*_json_cursor++ = '\t';
				}
				else
					rux_check_json_string_memory( _json , _json_size , _json_cursor , 3 , _step , __FILE__ , __LINE__ );
				*_json_cursor++ = '[';
				if( _formatted == 1 )
				{
					*_json_cursor++ = '\n';
					_tab_count++;
				}
				*_json_cursor = 0;
				_states.Add( serializer_state_array );
				_need_delimiter = 0;
				CODE_LABEL_END();
			};
			void serializer::end_array( const char* __file__ , ::rux::int32 __line__ , size_t ___rux__thread_index1986 )
			{
				(void)__file__;
				(void)__line__;
				UNREFERENCED_PARAMETER( ___rux__thread_index1986 );
				CODE_LABEL_START( 0 , 0 , 5 );
				if( _formatted == 1 )
				{
					rux_check_json_string_memory( _json , _json_size , _json_cursor , _tab_count + 2 , _step , __FILE__ , __LINE__ );
					if( _need_delimiter == 1 )
						*_json_cursor++ = '\n';
					_tab_count--;
					for( size_t index1 = 0 ; index1 < _tab_count ; index1++ )
						*_json_cursor++ = '\t';
				}
				else
					rux_check_json_string_memory( _json , _json_size , _json_cursor , 2 , _step , __FILE__ , __LINE__ );
				*_json_cursor++ = ']';
				*_json_cursor = 0;
				_states.RemoveAt( _states.Count() - 1 );
				_need_delimiter = 1;
				CODE_LABEL_END();
			};
			::rux::byte serializer::check_formatted_and_name( const char* utf8_name , const char* __file__ , ::rux::int32 __line__ , size_t ___rux__thread_index1986 )
			{
				if( _formatted == 1 )
				{
					if( _need_delimiter )
					{
						rux_check_json_string_memory( _json , _json_size , _json_cursor , _tab_count + 3 , _step , __file__ , __line__ );
						*_json_cursor++ = ',';
						*_json_cursor++ = '\n';
					}
					else
					{
						rux_check_json_string_memory( _json , _json_size , _json_cursor , _tab_count + 1 , _step , __file__ , __line__ );
					}
					for( size_t index1 = 0 ; index1 < _tab_count ; index1++ )
						*_json_cursor++ = '\t';
					*_json_cursor = 0;
				}
				else if( _need_delimiter )
				{
					rux_check_json_string_memory( _json , _json_size , _json_cursor , 2 , _step , __file__ , __line__ );
					*_json_cursor++ = ',';
					*_json_cursor = 0;
				}
				if( _states.Count() > 0 && _states[ _states.Count() - 1 ] == serializer_state_object )
				{
					generate_object_name( utf8_name , __file__ , __line__ , ___rux__thread_index1986 );
					return 1;
				}
				else
					return 0;
			};
			void serializer::generate_object_name( const char* utf8_name , const char* __file__ , ::rux::int32 __line__ , size_t ___rux__thread_index1986 )
			{
				::rux::data::XJsonSerializer::serialize_json_string_value( const_cast< char* >( utf8_name ) , _json , _json_size , _json_cursor , _step , __file__ , __line__ , ___rux__thread_index1986 );
				rux_check_json_string_memory( _json , _json_size , _json_cursor , 2 , _step , __file__ , __line__ );
				*_json_cursor++ = ':';
				*_json_cursor = 0;
			};
			void serializer::generate_boolean_value( const char* utf8_name , bool object , const char* __file__ , ::rux::int32 __line__ , size_t ___rux__thread_index1986 )
			{
				check_formatted_and_name( utf8_name , __file__ , __line__ , ___rux__thread_index1986 );
				::rux::data::XJsonSerializer::serialize_json_boolean_value( object ? 1 : 0 , _json , _json_size , _json_cursor , _step , __file__ , __line__ , ___rux__thread_index1986 );
				_need_delimiter = 1;
			};
			void serializer::generate_boolean_value( const char* utf8_name , ::rux::byte object , const char* __file__ , ::rux::int32 __line__ , size_t ___rux__thread_index1986 )
			{
				check_formatted_and_name( utf8_name , __file__ , __line__ , ___rux__thread_index1986 );
				::rux::data::XJsonSerializer::serialize_json_boolean_value( object , _json , _json_size , _json_cursor , _step , __file__ , __line__ , ___rux__thread_index1986 );
				_need_delimiter = 1;
			};
			void serializer::generate_null_value( const char* utf8_name , const char* __file__ , ::rux::int32 __line__ , size_t ___rux__thread_index1986 )
			{
				check_formatted_and_name( utf8_name , __file__ , __line__ , ___rux__thread_index1986 );
				::rux::data::XJsonSerializer::serialize_json_null_value( _json , _json_size , _json_cursor , _step , __file__ , __line__ , ___rux__thread_index1986 );
				_need_delimiter = 1;
			};
			void serializer::generate_int64_value( const char* utf8_name , rux::int64 object , const char* __file__ , ::rux::int32 __line__ , size_t ___rux__thread_index1986 )
			{
				check_formatted_and_name( utf8_name , __file__ , __line__ , ___rux__thread_index1986 );
				::rux::data::XJsonSerializer::serialize_json_int64_value( object , _json , _json_size , _json_cursor , _step , __file__ , __line__ , ___rux__thread_index1986 );
				_need_delimiter = 1;
			};
			void serializer::generate_uint64_value( const char* utf8_name , rux::uint64 object , const char* __file__ , ::rux::int32 __line__ , size_t ___rux__thread_index1986 )
			{
				check_formatted_and_name( utf8_name , __file__ , __line__ , ___rux__thread_index1986 );
				::rux::data::XJsonSerializer::serialize_json_uint64_value( object , _json , _json_size , _json_cursor , _step , __file__ , __line__ , ___rux__thread_index1986 );
				_need_delimiter = 1;
			};
			void serializer::generate_uint32_value( const char* utf8_name , rux::uint32 object , const char* __file__ , ::rux::int32 __line__ , size_t ___rux__thread_index1986 )
			{
				check_formatted_and_name( utf8_name , __file__ , __line__ , ___rux__thread_index1986 );
				::rux::data::XJsonSerializer::serialize_json_uint32_value( object , _json , _json_size , _json_cursor , _step , __file__ , __line__ , ___rux__thread_index1986 );
				_need_delimiter = 1;
			};
			void serializer::generate_int32_value( const char* utf8_name , rux::int32 object , const char* __file__ , ::rux::int32 __line__ , size_t ___rux__thread_index1986 )
			{
				check_formatted_and_name( utf8_name , __file__ , __line__ , ___rux__thread_index1986 );
				::rux::data::XJsonSerializer::serialize_json_int32_value( object , _json , _json_size , _json_cursor , _step , __file__ , __line__ , ___rux__thread_index1986 );
				_need_delimiter = 1;
			};
			void serializer::generate_double_value( const char* utf8_name , double object , const char* __file__ , ::rux::int32 __line__ , size_t ___rux__thread_index1986 )
			{
				check_formatted_and_name( utf8_name , __file__ , __line__ , ___rux__thread_index1986 );
				::rux::data::XJsonSerializer::serialize_json_double_value( object , _json , _json_size , _json_cursor , _step , __file__ , __line__ , ___rux__thread_index1986 );
				_need_delimiter = 1;
			};
			void serializer::generate_float_value( const char* utf8_name , float object , const char* __file__ , ::rux::int32 __line__ , size_t ___rux__thread_index1986 )
			{
				check_formatted_and_name( utf8_name , __file__ , __line__ , ___rux__thread_index1986 );
				::rux::data::XJsonSerializer::serialize_json_float_value( object , _json , _json_size , _json_cursor , _step , __file__ , __line__ , ___rux__thread_index1986 );
				_need_delimiter = 1;
			};
			void serializer::generate_string_value( const char* utf8_name , const char* object , const char* __file__ , ::rux::int32 __line__ , size_t ___rux__thread_index1986 )
			{
				check_formatted_and_name( utf8_name , __file__ , __line__ , ___rux__thread_index1986 );
				::rux::data::XJsonSerializer::serialize_json_string_value( const_cast< char* >( object ) , _json , _json_size , _json_cursor , _step , __file__ , __line__ , ___rux__thread_index1986 );
				_need_delimiter = 1;
			};
			void serializer::generate_object_value( const char* utf8_name , ::rux::data::DataObject* object , ::rux::XString& error , const char* __file__ , ::rux::int32 __line__ , size_t ___rux__thread_index1986 )
			{
				if( check_formatted_and_name( utf8_name , __file__ , __line__ , ___rux__thread_index1986 ) && _formatted )
				{
					rux_check_json_string_memory( _json , _json_size , _json_cursor , _tab_count + 3 , _step , __file__ , __line__ );
					*_json_cursor++ = '\n';
					for( size_t index1 = 0 ; index1 < _tab_count ; index1++ )
						*_json_cursor++ = '\t';
				}
				::rux::data::XJsonSerializer::serialize_json_object_value( object , _json , _json_size , _json_cursor , _formatted , _tab_count , _step , error , __file__ , __line__ , ___rux__thread_index1986 );
				_need_delimiter = 1;
			};
		};
		enum json_status_enum
		{
			json_status_enum_ok = 0,
			json_status_enum_wait_for_name = 1,
			json_status_enum_wait_for_value = 2,
			json_status_enum_wait_for_colon = 3
		};
		XGCRef* XJsonSerializer::deserialize_json_object_value( char*& ptr , uint8& is_error , const char* function , const char* __file__ , ::rux::int32 __line__ , ::rux::uint32* hash , size_t ___rux__thread_index1986 , char*& temp_str , size_t& temp_str_size , ::rux::byte is_valid_json_check_only )
		{			
			CODE_LABEL_START( 0 , 0 , 30 );
			rux::data::DataObject* object = 0;
			if( is_valid_json_check_only == 0 )
				object = ::rux::engine::alloc_object< ::rux::data::DataObject >( (const char*)__file__ , (::rux::uint32)__line__ , (char*)"data object" , (::rux::malloc_t)0 );
			json_status_enum json_status = json_status_enum_ok;
			is_error = 1;
			uint8 pairs_greater_zero = 0;
			::rux::uint32 field_name_hash = 0;
			if( hash )
				*hash = *hash * 33 + *ptr;
			ptr++;
			char symbol = 0;
			for( ; ; )
			{					
				symbol = ::rux::data::_json_deserialize_help_table[ (::rux::byte)*ptr ];
				if( symbol == 0 )
					break;
				else if( symbol == ':' )
				{
					if( hash )
						*hash = *hash * 33 + *ptr;
					if( json_status != json_status_enum_wait_for_colon )
						break;
					else
					{
						json_status = json_status_enum_wait_for_value;
						ptr++;
					}
				}
				else if( symbol == ',' )
				{
					if( hash )
						*hash = *hash * 33 + *ptr;
					if( pairs_greater_zero == 0
						|| json_status > 0 )
						break;
					else
					{
						json_status = json_status_enum_wait_for_name;
						ptr++;
					}
				}
				else if( symbol == '}' )
				{
					if( hash )
						*hash = *hash * 33 + *ptr;
					if( json_status == 0 )
						is_error = 0;
					break;
				}
				else if( symbol == '"' )
				{
					if( json_status == json_status_enum_wait_for_value )
					{
						is_error = 0;
						::rux::data::DoubleLinkedListItem< ::rux::data::key_value_pair_t >* dl_item = 0;
						::rux::data::key_value_pair_t* key_value_pair = 0;
						if( is_valid_json_check_only == 0 )
						{
							dl_item = ::rux::engine::alloc_object< ::rux::data::DoubleLinkedListItem< ::rux::data::key_value_pair_t >  >( (const char*)__file__ , (::rux::uint32)__line__ , (char*)"data object" , (::rux::malloc_t)0 );
							key_value_pair = dl_item->operator->();
						}
						deserialize_json_string_value( ptr , is_error , function , temp_str , 0 , &temp_str_size , __file__ , __line__ , hash , key_value_pair , ___rux__thread_index1986 , is_valid_json_check_only );
						if( is_error == 0 )
						{
							if( key_value_pair )
							{
								key_value_pair->_name_hash = field_name_hash;
								object->_pairs.Add( dl_item );
							}
							pairs_greater_zero = 1;
							json_status = json_status_enum_ok;
							is_error = 1;
						}
						else
						{
							if( dl_item )
								::rux::engine::free_object< ::rux::data::DoubleLinkedListItem< ::rux::data::key_value_pair_t > >( dl_item );
							break;
						}
					}
					else if( pairs_greater_zero == 0
						|| json_status == json_status_enum_wait_for_name )
					{			
						is_error = 0;
						deserialize_json_string_value( ptr , is_error , function , temp_str , NULL , &temp_str_size , __file__ , __line__ , hash , 0 , ___rux__thread_index1986 , is_valid_json_check_only );
						if( is_error == 0 )
						{
							if( is_valid_json_check_only == 0 )
								field_name_hash = ::rux::engine::_globals->add_times33_hash( temp_str , SIZE_MAX );
							json_status = json_status_enum_wait_for_colon;
							is_error = 1;
						}
						else
							break;
					}
					else
						break;
				}
				else if( symbol == ' ' )
				{
					while( symbol == ' ' )
					{
						ptr++;
						symbol = _json_deserialize_help_table[ (::rux::byte)*ptr ];
					}
				}
				else
				{
					if( json_status == json_status_enum_wait_for_value )
					{
						is_error = 0;
						::rux::data::DoubleLinkedListItem< ::rux::data::key_value_pair_t >* dl_item = 0;
						::rux::data::key_value_pair_t* key_value_pair = 0;
						if( is_valid_json_check_only == 0 )
						{
							dl_item = ::rux::engine::alloc_object< ::rux::data::DoubleLinkedListItem< ::rux::data::key_value_pair_t > >( (const char*)__file__ , (::rux::uint32)__line__ , (char*)"data object" , (::rux::malloc_t)0 );
							key_value_pair = dl_item->operator->();
						}
						deserialize_json_value( ptr , is_error , function , __file__ , __line__ , hash , key_value_pair , ___rux__thread_index1986 , temp_str , temp_str_size , is_valid_json_check_only );
						if( is_error == 0 )
						{
							if( key_value_pair )
							{
								key_value_pair->_name_hash = field_name_hash;
								object->_pairs.Add( dl_item );
							}
							pairs_greater_zero = 1;
							json_status = json_status_enum_ok;
							is_error = 1;
						}
						else
						{
							if( dl_item )
								::rux::engine::free_object< ::rux::data::DoubleLinkedListItem< ::rux::data::key_value_pair_t > >( dl_item );
							break;
						}
					}
					else
						break;
				}
			}
			if( is_error == 1 && function == 0 )
				function = __FUNCTION__;
			CODE_LABEL_END();
			return object;
		};
		XGCRef* XJsonSerializer::deserialize_json_true_boolean_value( char*& ptr , uint8& is_error , const char* function , const char* __file__ , ::rux::int32 __line__ , ::rux::uint32* hash , ::rux::data::key_value_pair_t* key_value_pair , size_t ___rux__thread_index1986 , ::rux::byte is_valid_json_check_only )
		{
			CODE_LABEL_START( 0 , 0 , 5 );
			XGCRef* gcref = 0;
			if( hash )
				*hash = *hash * 33 + *ptr;
			ptr++;
			if( *ptr == 'r' )
			{
				if( hash )
					*hash = *hash * 33 + *ptr;
				ptr++;
				if( *ptr == 'u' )
				{
					if( hash )
						*hash = *hash * 33 + *ptr;
					ptr++;
					if( *ptr == 'e' )
					{
						if( hash )
							*hash = *hash * 33 + *ptr;
						if( is_valid_json_check_only == 0 )
						{
							if( key_value_pair )
							{
								::rux::uint8 val = 1;
								key_value_pair->set00( &val , ::rux::data::key_value_pair_t_type_boolean , 1 , SIZE_MAX , ___rux__thread_index1986 );
							}
							else
							{
								rux::XBoolean object( "object" , __file__ , __line__ );
								object = true;
								gcref = object.get_GCRef( 1 , 1 );
								gcref->AddRef( __FILE__ , __LINE__ );
							}
						}
					}
					else
						is_error = 1;
				}
				else
					is_error = 1;
			}
			else
				is_error = 1;
			if( is_error == 1 && function == 0 )
				function = __FUNCTION__;
			CODE_LABEL_END();
			return gcref;
		};
		XGCRef* XJsonSerializer::deserialize_json_false_boolean_value( char*& ptr , uint8& is_error , const char* function , const char* __file__ , ::rux::int32 __line__ , ::rux::uint32* hash , ::rux::data::key_value_pair_t* key_value_pair , size_t ___rux__thread_index1986 , ::rux::byte is_valid_json_check_only )
		{
			CODE_LABEL_START( 0 , 0 , 5 );
			XGCRef* gcref = 0;
			if( hash )
				*hash = *hash * 33 + *ptr;
			ptr++;
			if( *ptr == 'a' )
			{
				if( hash )
					*hash = *hash * 33 + *ptr;
				ptr++;
				if( *ptr == 'l' )
				{
					if( hash )
						*hash = *hash * 33 + *ptr;
					ptr++;
					if( *ptr == 's' )
					{
						if( hash )
							*hash = *hash * 33 + *ptr;
						ptr++;
						if( *ptr == 'e' )
						{
							if( hash )
								*hash = *hash * 33 + *ptr;
							if( is_valid_json_check_only == 0 )
							{
								if( key_value_pair )
								{
									::rux::uint8 val = 0;
									key_value_pair->set00( &val , ::rux::data::key_value_pair_t_type_boolean , 1 , SIZE_MAX , ___rux__thread_index1986 );
								}
								else
								{
									rux::XBoolean object( "object" , __file__ , __line__ );
									object = false;
									gcref = object.get_GCRef( 1 , 1 );
									gcref->AddRef( __FILE__ , __LINE__ );
								}
							}
						}
						else
							is_error = 1;
					}
					else
						is_error = 1;
				}
				else
					is_error = 1;
			}
			else
				is_error = 1;			
			if( is_error == 1 && function == 0 )
				function = __FUNCTION__;
			CODE_LABEL_END();
			return gcref;
		};
		XGCRef* XJsonSerializer::deserialize_json_null_value( char*& ptr , uint8& is_error , const char* function , const char* __file__ , ::rux::int32 __line__ , ::rux::uint32* hash , ::rux::data::key_value_pair_t* key_value_pair , size_t ___rux__thread_index1986 , ::rux::byte is_valid_json_check_only )
		{
			CODE_LABEL_START( 0 , 0 , 5 );
			if( hash )
			{
				*hash = *hash * 33 + *ptr;
				ptr++;
				if( *ptr == 'u' )
				{
					*hash = *hash * 33 + *ptr;
					ptr++;
					if( *ptr == 'l' )
					{
						*hash = *hash * 33 + *ptr;
						ptr++;
						if( *ptr == 'l' )
							*hash = *hash * 33 + *ptr;
						else
							is_error = 1;
					}
					else
						is_error = 1;
				}
				else
					is_error = 1;
			}
			else
			{
				if( strncmp( ptr , "null" , 4 ) == 0 )
					ptr += 3;
				else
					is_error = 1;
			}
			if( is_error == 1 && function == 0 )
				function = __FUNCTION__;
			XGCRef* gcref = 0;
			if( is_valid_json_check_only == 0 )
			{
				if( key_value_pair )
					key_value_pair->clear();
				else
				{
					rux::XNull object( "object" , __file__ , __line__ );
					gcref = object.get_GCRef( 1 , 1 );
					gcref->AddRef( __FILE__ , __LINE__ );
				}
			}
			CODE_LABEL_END();
			return gcref;
		};
#define MAX_VALUE_TO_MULTIPLY ( ( LLONG_MAX / 10LL ) + ( LLONG_MAX % 10LL ) )
#define UNSIGNED_MAX_VALUE_TO_MULTIPLY ( ( ULLONG_MAX / 10ULL ) + ( ULLONG_MAX % 10ULL ) )
		XGCRef* XJsonSerializer::deserialize_json_number_value( char*& ptr , uint8& is_error , const char* function , const char* __file__ , ::rux::int32 __line__ , ::rux::uint32* hash , ::rux::data::key_value_pair_t* key_value_pair , size_t ___rux__thread_index1986 , ::rux::byte is_valid_json_check_only )
		{		
			CODE_LABEL_START( 0 , 0 , 5 );
			XGCRef* gcref = NULL;
			uint8 positive = 1;
			if( *ptr == '-' )
			{
				if( hash )
					*hash = *hash * 33 + *ptr;
				positive = 0;
				ptr++;
			}
			else if( *ptr == '+' )
			{
				if( hash )
					*hash = *hash * 33 + *ptr;
				ptr++;
			}
			::rux::int64 val_int64 = 0LL;
			::rux::uint64 val_uint64 = 0ULL;
			if( *ptr >= '0' && *ptr <= '9' )
			{				
				while( *ptr >= '0' && *ptr <= '9' )
				{
					if( val_uint64 == 0 )
					{
						if( val_int64 > MAX_VALUE_TO_MULTIPLY )
						{
							if( positive )
								val_uint64 = val_int64;
							else
							{
								is_error = 1;
								break;
							}
						}
						else
						{
							val_int64 *= 10LL;
							if( LLONG_MAX - val_int64 < ( *ptr - '0' ) )
							{
								if( positive )
								{
									val_uint64 = val_int64;
									if( hash )
										*hash = *hash * 33 + *ptr;
									val_uint64 += ( *ptr++ - '0' );
								}
								else
								{
									is_error = 1;
									break;
								}
							}
							else
							{
								if( hash )
									*hash = *hash * 33 + *ptr;
								val_int64 += ( *ptr++ - '0' );
							}
						}
					}
					else
					{
						if( val_uint64 > UNSIGNED_MAX_VALUE_TO_MULTIPLY )
						{
							is_error = 1;
							break;
						}
						val_uint64 *= 10ULL;
						if( ULLONG_MAX - val_uint64 < ( *ptr - '0' ) )
						{
							is_error = 1;
							break;
						}
						if( hash )
							*hash = *hash * 33 + *ptr;
						val_uint64 += ( *ptr++ - '0' );
					}
				}
			}
			else
				is_error = 1;
			if( is_error == 0 )
			{
				if( *ptr == '.' )
				{
					if( hash )
						*hash = *hash * 33 + *ptr;
					ptr++;					
					if( *ptr >= '0' && *ptr <= '9' )
					{
						double val_double = 0 , delimiter = 10.0;
						while( *ptr >= '0' && *ptr <= '9' )
						{
							if( hash )
								*hash = *hash * 33 + *ptr;
							val_double += ( (double)( *ptr++ - '0' ) ) / delimiter;
							delimiter *= 10.0;
						}
						val_double += val_uint64 ? (double)val_uint64 : (double)val_int64;
						if( is_valid_json_check_only == 0 )
						{
							if( key_value_pair )
							{
								val_double = positive ? val_double : -val_double;
								key_value_pair->set00( &val_double , ::rux::data::key_value_pair_t_type_double , 1 , SIZE_MAX , SIZE_MAX );
							}
							else
							{
								rux::XDouble val( "val" , __file__ , __line__ );
								val = positive ? val_double : -val_double;
								gcref = val.get_GCRef( 1 , 1 );
								gcref->AddRef( __FILE__ , __LINE__ );
							}
						}
					}
					else
						is_error = 1;
				}
				else if( val_uint64 )
				{
					if( is_valid_json_check_only == 0 )
					{
						if( key_value_pair )
							key_value_pair->set00( &val_uint64 , ::rux::data::key_value_pair_t_type_uint64 , 1 , SIZE_MAX , SIZE_MAX );
						else
						{
							rux::XUInt64 val( "val" , __file__ , __line__ );
							val = val_uint64;
							gcref = val.get_GCRef( 1 , 1 );
							gcref->AddRef( __FILE__ , __LINE__ );
						}
					}
				}
				else
				{
					if( is_valid_json_check_only == 0 )
					{
						if( key_value_pair )
						{
							val_int64 = positive ? val_int64 : -val_int64;
							key_value_pair->set00( &val_int64 , ::rux::data::key_value_pair_t_type_int64 , 1 , SIZE_MAX , SIZE_MAX );
						}
						else
						{
							rux::XInt64 val( "val" , __file__ , __line__ );
							val = positive ? val_int64 : -val_int64;
							gcref = val.get_GCRef( 1 , 1 );
							gcref->AddRef( __FILE__ , __LINE__ );
						}
					}
				}
				if( is_error == 0 )
					ptr--;
			}
			if( is_error == 1 && function == 0 )
				function = __FUNCTION__;
			CODE_LABEL_END();
			return gcref;
		};
		void XJsonSerializer::deserialize_json_string_value( char*& ptr , uint8& is_error , const char* function , char*& value , size_t* length , size_t* allocated_size , const char* __file__ , ::rux::int32 __line__ , ::rux::uint32* hash , ::rux::data::key_value_pair_t* key_value_pair , size_t ___rux__thread_index1986 , ::rux::byte is_valid_json_check_only )
		{
			CODE_LABEL_START( 0 , 0 , 10 );
			::rux::byte success = 0;
			if( key_value_pair )
				key_value_pair->clear();
			if( *ptr == '"' )
			{
				char* slash = 0 , * begin = ptr , * end = 0;
				begin++;
				if( hash )
					*hash = *hash * 33 + *ptr;
#if 0
				//char symbol = 0;
				::rux::byte even_slash = 0;
				::rux::byte need_break0 = 0;
				for( ; need_break0 == 0 ; )
				{
					//char symbol = *++ptr;
					switch( *++ptr )
					{
					case 0:
						{
							need_break0 = 1;
							break;
						}
					case '"':
						{
							if( even_slash )
							{
								even_slash = 0;
								end = 0;
							}
							else
							{
								end = ptr;
								ptr++;
								need_break0 = 1;
							}
							break;
						}
					case '\\':
						{
							if( even_slash == 0 )
								even_slash = 1;
							else
								even_slash = 0;
							if( slash == 0 )
								slash = ptr;
							break;
						}
					default:
						{
							if( even_slash )
								even_slash = 0;
							break;
						}
					}
				}
#else

				::rux::byte even_slash = 0;
				::rux::byte utf8_valid = 1;
				::rux::byte byte_symbol = 0;
				for( ; ; )
				{
					ptr++;					
					//check utf8
					byte_symbol = (::rux::uint8)*ptr;

					if( byte_symbol <= 0x7f )
					{
						//main code
						if( *ptr == '"' )
						{
							if( hash )
								*hash = *hash * 33 + *ptr;
							if( even_slash )
							{
								even_slash = 0;
								end = NULL;
							}
							else
							{
								end = ptr;
								ptr++;
								break;
							}
						}
						else if( *ptr == '\\' )
						{
							if( hash )
								*hash = *hash * 33 + *ptr;
							if( even_slash == 0 )
								even_slash = 1;
							else
								even_slash = 0;
							if( slash == 0 )
								slash = ptr;
						}
						else
						{
							if( *ptr == 0 )
								break;
							else
							{
								if( hash )
									*hash = *hash * 33 + *ptr;
								even_slash = 0;
							}
						}
						//end main code
					}
					else if( ( byte_symbol >> 5 ) == 0x6 )
					{
						ptr++;
						if( *ptr == 0 )
							break;
						if( hash )
							*hash = *hash * 33 + *ptr;
						byte_symbol = (::rux::uint8)*ptr;
						if( !( ( byte_symbol >> 6 ) == 0x2 ) )
						{
							utf8_valid = 0;
							break;
						}
					}
					/* three byte */
					else if( ( byte_symbol >> 4 ) == 0x0e ) 
					{
						ptr++;
						if( *ptr == 0 )
							break;
						if( hash )
							*hash = *hash * 33 + *ptr;
						byte_symbol = (::rux::uint8)*ptr;
						if( !( ( byte_symbol >> 6 ) == 0x2 ) )
						{
							utf8_valid = 0;
							break;
						}
						ptr++;
						if( *ptr == 0 )
							break;
						if( hash )
							*hash = *hash * 33 + *ptr;
						byte_symbol = (::rux::uint8)*ptr;
						if( !( ( byte_symbol >> 6 ) == 0x2 ) )
						{
							utf8_valid = 0;
							break;
						}
					}
					/* four byte */        
					else if( ( *ptr >> 3 ) == 0x1e )
					{
						ptr++;
						if( *ptr == 0 )
							break;
						if( hash )
							*hash = *hash * 33 + *ptr;
						byte_symbol = (::rux::uint8)*ptr;

						if( !( ( byte_symbol >> 6 ) == 0x2 ) )
						{
							utf8_valid = 0;
							break;
						}
						ptr++;
						if( *ptr == 0 )
							break;
						if( hash )
							*hash = *hash * 33 + *ptr;
						byte_symbol = (::rux::uint8)*ptr;

						if( !( ( byte_symbol >> 6 ) == 0x2 ) )
						{
							utf8_valid = 0;
							break;
						}
						ptr++;
						if( *ptr == 0 )
							break;
						if( hash )
							*hash = *hash * 33 + *ptr;
						byte_symbol = (::rux::uint8)*ptr;

						if( !( ( byte_symbol >> 6 ) == 0x2 ) )
						{
							utf8_valid = 0;
							break;
						}
					}
					else
					{
						utf8_valid = 0;
						break;
					}

					//end check utf8

					
				}
#endif

				if( end )
				{
					if( is_valid_json_check_only == 0 )
					{
						size_t size = ptr - begin;
						if( size > 0 )
						{
							char* name = 0;
							if( key_value_pair )
							{
								if( size > sizeof( key_value_pair->_small_utf8_string ) )
								{
									name = ::rux::engine::alloc_array< char >( size , __file__ ? (const char*)__file__ : (const char*)__FILE__ , __line__ ? (::rux::uint32)__line__ : (::rux::uint32)__LINE__ , (char*)NULL );
									key_value_pair->_type = ::rux::data::key_value_pair_t_type_large_utf8_string;
									key_value_pair->_large_utf8_string = name;
								}
								else
								{
									key_value_pair->_type = ::rux::data::key_value_pair_t_type_small_utf8_string;
									name = key_value_pair->_small_utf8_string;
								}
							}
							else
							{
								if( size > *allocated_size )
								{
									if( value )
										::rux::engine::free_mem( value );
									value = ::rux::engine::alloc_array< char >( size , __file__ ? (const char*)__file__ : (const char*)__FILE__ , __line__ ? (::rux::uint32)__line__ : (::rux::uint32)__LINE__ , (char*)NULL );
									*allocated_size = size;
								}
								name = value;
							}
							if( slash )
							{
								size_t name_count = 1;
								if( slash > begin )
								{
									name_count = slash - begin + 1;
									::memcpy( name , begin , name_count - 1 );
								}
								_declare_stackvar( uint32 , value_uint32 , 0 );
								value_uint32 = 0;
								begin = slash;
								while( slash )
								{
									if( slash[ 1 ] == 'b' )
									{
										name[ name_count - 1 ] = '\b';
										slash += 2;
										name_count++;
										begin = slash;
									}
									else if( slash[ 1 ] == 'n' )
									{
										name[ name_count - 1 ] = '\n';
										slash += 2;
										name_count++;
										begin = slash;
									}
									else if( slash[ 1 ] == 'f' )
									{
										name[ name_count - 1 ] = '\f';
										slash += 2;
										name_count++;
										begin = slash;
									}
									else if( slash[ 1 ] == 'r' )
									{
										name[ name_count - 1 ] = '\r';
										slash += 2;
										name_count++;
										begin = slash;
									}
									else if( slash[ 1 ] == 't' )
									{
										name[ name_count - 1 ] = '\t';
										slash += 2;
										name_count++;
										begin = slash;
									}
									else if( slash[ 1 ] == 'u' && ( end - slash ) >= 6
										&& rux_hex_string_to_uint32( &slash[ 2 ] , value_uint32 , 4 ) == 1
										&& value_uint32 >= 0x20 )
									{					
										uint16 value_uint16 = (uint16)value_uint32;
										size_t utf16_to_utf8_bytes = 0;
										rux::XString::utf16_to_utf8( &value_uint16 , 1 , (char*)&value_uint32 , 4 , utf16_to_utf8_bytes );
										size_t length1 = rux::XString::utf8_symbol_length( (char*)&value_uint32 , 4 , 0 );
										::memcpy( &name[ name_count - 1 ] , &value_uint32 , length1 );
										slash += 6;
										name_count += length1;				
										begin = slash;
									}
									else
									{
										size_t length = 0;
										slash++;
										begin = slash;
										for( ; ; )
										{
											if( ( *slash & 0xc0 ) != 0x80 )
											{
												if( length == 0 )
													length = 1;
												else
												{
													::memcpy( &name[ name_count - 1 ] , begin , length );								
													name_count += length;
													begin = slash;
													break;
												}
											}
											else
												length++;
											slash++;
										}
									}
									for( ; ; )
									{
										if( slash[ 0 ] == '\\' )
											break;
										else if( slash > end )
										{
											slash = 0;
											break;
										}
										slash++;
									}
									if( slash )
									{
										if( slash > begin )
										{
											::memcpy( &name[ name_count - 1 ] , begin , slash - begin );
											name_count += slash - begin;
										}
										begin = slash;
									}
								}
								if( end > begin )
								{
									::memcpy( &name[ name_count - 1 ] , begin , end - begin );
									name_count += end - begin;
								}
								name[ name_count - 1 ] = 0;
								if( length )
									*length = name_count - 1;
								success = 1;
							}
							else
							{				
								::memcpy( name , begin , size - 1 );
								name[ size - 1 ] = 0;
								if( length )
									*length = size - 1;
								success = 1;
							}
						}
					}
				}	
				else
					is_error = 1;
			}
			if( is_error == 1 && function == 0 )
				function = __FUNCTION__;
			if( success == 0 && is_error == 0 && is_valid_json_check_only == 0 )
			{
				if( key_value_pair )
				{
					key_value_pair->_type = ::rux::data::key_value_pair_t_type_small_utf8_string;
					key_value_pair->_small_utf8_string[ 0 ] = 0;
				}
				else
				{
					if( value == 0 )
					{
						value = ::rux::engine::alloc_array< char >( 1 , __file__ ? (const char*)__file__ : (const char*)__FILE__ , __line__ ? (::rux::uint32)__line__ : (::rux::uint32)__LINE__ , (char*)NULL );
						if( allocated_size )
							*allocated_size = 1;
					}
					value[ 0 ] = 0;
					if( length )
						*length = 0;
				}
			}
			CODE_LABEL_END();
		};
		XGCRef* XJsonSerializer::deserialize_json_array_value( char*& ptr , uint8& is_error , const char* function , const char* __file__ , ::rux::int32 __line__ , ::rux::uint32* hash , size_t ___rux__thread_index1986 , char*& temp_str , size_t& temp_str_size , ::rux::byte is_valid_json_check_only )
		{
			CODE_LABEL_START( 0 , 0 , 30 );
			rux::Array< XObject >* object = 0;
			if( is_valid_json_check_only == 0 )
				object = ::rux::engine::alloc_object< rux::Array< XObject > >( (const char*)__file__ , (::rux::uint32)__line__ , (char*)"array" , (::rux::malloc_t)0 );
			is_error = 1;
			uint8 wait_for_value = 0 , object_greater_zero = 0;
			if( hash )
				*hash = *hash * 33 + *ptr;
			ptr++;			
			char symbol = 0;
			for( ; ; )
			{	
				symbol = ::rux::data::_json_deserialize_help_table[ (::rux::byte)*ptr ];
				if( symbol == 0 )
					break;
				else if( symbol == ',' )
				{
					if( hash )
						*hash = *hash * 33 + *ptr;
					if( object_greater_zero == 0 )
						break;
					else
					{
						wait_for_value = 1;
						ptr++;
					}
				}
				else if( symbol == ']' )
				{
					if( hash )
						*hash = *hash * 33 + *ptr;
					if( wait_for_value != 1 )
						is_error = 0;
					break;
				}
				else if( symbol == ' ' )
				{
					while( symbol == ' ' )
					{
						ptr++;
						symbol = _json_deserialize_help_table[ (::rux::byte)*ptr ];
					}
				}
				else
				{
					object_greater_zero = 1;
					is_error = 0;
					XGCRef* item = deserialize_json_value( ptr , is_error , function , __file__ , __line__ , hash , 0 , ___rux__thread_index1986 , temp_str , temp_str_size , is_valid_json_check_only );
					if( is_error == 0 )
					{
						if( item )
						{
							if( object )
								object->Add( *item , __file__ , __line__ );
							item->Release( __FILE__ , __LINE__ );
						}
						wait_for_value = 0;
					}
					if( is_error == 1 )
					{
						if( item )
							item->Release();
						break;
					}
					else
						is_error = 1;
				}
			}
			if( is_error == 1 && function == 0 )
				function = __FUNCTION__;
			CODE_LABEL_END();
			return object;
		};
		XGCRef* XJsonSerializer::deserialize_json_value( char*& ptr , uint8& is_error , const char* function , const char* __file__ , ::rux::int32 __line__ , ::rux::uint32* hash , ::rux::data::key_value_pair_t* key_value_pair , size_t ___rux__thread_index1986 , char*& temp_str , size_t& temp_str_size , ::rux::byte is_valid_json_check_only )
		{
			CODE_LABEL_START( 0 , 0 , 100 );
			XGCRef* gcref = NULL;
			is_error = 1;
			char symbol = 0;
			for( ; ; )
			{	
				symbol = _json_deserialize_help_table[ (::rux::byte)*ptr ];
				if( symbol == 0 )
					break;
				else if( symbol == '{' )
				{
					is_error = 0;
					gcref = deserialize_json_object_value( ptr , is_error , function , __file__ , __line__ , hash , ___rux__thread_index1986 , temp_str , temp_str_size , is_valid_json_check_only );
					if( is_error == 0 )
					{
						if( key_value_pair )
						{
							key_value_pair->set00( gcref , ::rux::data::key_value_pair_t_type_gcref , 0 , SIZE_MAX , SIZE_MAX );
							gcref = 0;
						}					
						ptr++;
					}
					else if( gcref )
					{
						gcref->Release();
						gcref = 0;
					}
					break;
				}
				else if( symbol == '[' )
				{				
					is_error = 0;
					gcref = deserialize_json_array_value( ptr , is_error , function , __file__ , __line__ , hash , ___rux__thread_index1986 , temp_str , temp_str_size , is_valid_json_check_only );
					if( is_error == 0 )
					{
						if( key_value_pair )
						{
							key_value_pair->set00( gcref , ::rux::data::key_value_pair_t_type_gcref , 0 , SIZE_MAX , SIZE_MAX );
							gcref = 0;
						}					
						ptr++;
					}
					else if( gcref )
					{
						gcref->Release();
						gcref = 0;
					}
					break;
				}
				else if( symbol == 'n' )
				{				
					is_error = 0;
					gcref = deserialize_json_null_value( ptr , is_error , function , __file__ , __line__ , hash , key_value_pair , ___rux__thread_index1986 , is_valid_json_check_only );
					if( is_error == 0 )
						ptr++;
					else if( gcref )
					{
						gcref->Release();
						gcref = 0;
					}
					break;
				}
				else if( symbol == 't' )
				{					
					is_error = 0;
					gcref = deserialize_json_true_boolean_value( ptr , is_error , function , __file__ , __line__ , hash , key_value_pair , ___rux__thread_index1986 , is_valid_json_check_only );
					if( is_error == 0 )
						ptr++;
					else if( gcref )
					{
						gcref->Release();
						gcref = 0;
					}
					break;
				}
				else if( symbol == 'f' )
				{					
					is_error = 0;
					gcref = deserialize_json_false_boolean_value( ptr , is_error , function , __file__ , __line__ , hash , key_value_pair , ___rux__thread_index1986 , is_valid_json_check_only );
					if( is_error == 0 )
						ptr++;
					else if( gcref )
					{
						gcref->Release();
						gcref = 0;
					}
					break;
				}
				else if( symbol == '"' )
				{	
					is_error = 0;
					if( is_valid_json_check_only == 0 )
					{
						if( key_value_pair )
							deserialize_json_string_value( ptr , is_error , function , temp_str , 0 , &temp_str_size , __file__ , __line__ , hash , key_value_pair , ___rux__thread_index1986 , is_valid_json_check_only );
						else
						{
							rux::XString new_str( (size_t)0 , "new_str" , __file__ , __line__ );
							new_str()->_code_page = XEnumCodePage_UTF8;
							deserialize_json_string_value( ptr , is_error , function , new_str()->_str , &new_str()->_count , &new_str()->_size , __file__ , __line__ , hash , 0 , ___rux__thread_index1986 , is_valid_json_check_only );
							new_str()->_count++;
							gcref = new_str.get_GCRef( 1 , 1 );
							gcref->AddRef( __FILE__ , __LINE__ );
						}
					}
					else
						deserialize_json_string_value( ptr , is_error , function , temp_str , 0 , &temp_str_size , __file__ , __line__ , hash , 0 , ___rux__thread_index1986 , is_valid_json_check_only );
					break;
				}
				else if( symbol == '0' )
				{			
					is_error = 0;
					gcref = deserialize_json_number_value( ptr , is_error , function , __file__ , __line__ , hash , key_value_pair , ___rux__thread_index1986 , is_valid_json_check_only );
					if( is_error == 0 )
						ptr++;
					else if( gcref )
					{
						gcref->Release();
						gcref = 0;
					}
					break;
				}
				else if( symbol == ' ' )
				{
					while( symbol == ' ' )
					{
						ptr++;
						symbol = _json_deserialize_help_table[ (::rux::byte)*ptr ];
					}
				}
				else
					break;
			}
			if( is_error == 1 && function == 0 )
				function = __FUNCTION__;
			CODE_LABEL_END();
			return gcref;
		};
		::rux::byte XJsonSerializer::IsValidJSON( const rux::XString& json , rux::XString& error , const char* __file__ , ::rux::int32 __line__ , ::rux::uint32* hash , size_t ___rux__thread_index1986 )
		{
			declare_variable( rux::XString , string );	
			string.set_ByRef( json.ConvertToUTF8() );	
			return IsValidJSON( string.str() , 1 , error , __file__ , __line__ , hash , ___rux__thread_index1986 );
		};
		::rux::byte XJsonSerializer::IsValidJSON( char* json_ptr , uint8 is_utf8 , rux::XString& error , const char* __file__ , ::rux::int32 __line__ , ::rux::uint32* hash , size_t ___rux__thread_index1986 )
		{
			if( is_utf8 == 0 )
				return IsValidJSON( json_ptr , error );
			else
			{	
				error.Clear();
				uint8 is_error = 0;		
				char* ptr = json_ptr;
				const char* function = 0;
				char* temp_str = 0;
				size_t temp_str_size = 0;
				deserialize_json_value( ptr , is_error , function , __file__ , __line__ , hash , 0 , ___rux__thread_index1986 , temp_str , temp_str_size , 1 );
				if( temp_str )
					::rux::engine::free_mem( temp_str );
				if( is_error == 0 )
				{				
					for( ; ; )
					{
						if( *ptr == 0 )
							break;
						switch( *ptr )
						{
						case ' ':
						case '\n':
						case '\r':
						case '\t':
							{
								break;
							}
						default:
							{
								is_error = 1;
								break;
							}
						}
						if( is_error == 1 )
							break;
						ptr++;
					}
				}
				if( is_error == 1 )
				{
					if( function == 0 )
						function = __FUNCTION__;
					size_t length = 0;
					uint32 column = 0 , line = 1;
					char* next = json_ptr;
					while( next != ptr )
					{
						if( ( *next & 0xc0 ) != 0x80 )
						{					
							if( length == 1 )
							{
								next--;
								if( *next == '\n' )
									line++;
								next++;
							}
							column++;
							length = 1;
						}
						else if( length != 0 )
							length++;
						if( *next == 0 )
							break;
						next++;
					}
					if( ptr - json_ptr >= 20 )
						ptr -= 20;
					else
						ptr = json_ptr;
					rux::XString json_part( ptr );
					json_part.set_ByRef( json_part.Substring( 0 , 40 ) );
					rux_write_error( "incorrect json string format here '..." + json_part + "...' , line " + rux::XUInt32( line ).ToString() + " , column " + rux::XUInt32( column ).ToString() + " , internal function = '" + function + "'"  , error );
				}
				return is_error == 1 ? 0 : 1;
			}
		};
		XGCRef* XJsonSerializer::DeserializeAndReturnGCRef( char* json_ptr , uint8 is_utf8 , rux::XString& error , const char* __file__ , ::rux::int32 __line__ , ::rux::uint32* hash , size_t ___rux__thread_index1986 )
		{
			CODE_LABEL_START( 0 , 0 , 30 );
			if( __file__ == 0 || __line__ == 0 )
			{
				__file__ = __FILE__;
				__line__ = __LINE__;
			}
			if( hash )
				*hash = 0;
			if( is_utf8 == 0 )
			{
				CODE_LABEL_END();
				::rux::String str;
				str.attach( json_ptr , SIZE_MAX , SIZE_MAX , ::rux::XString::get_global_code_page() );
				XGCRef* gcref = DeserializeAndReturnGCRef( str , error , __file__ , __line__ , hash , ___rux__thread_index1986 );
				str.detach();
				return gcref;
			}
			else
			{	
				error.Clear();
				uint8 is_error = 0;		
				char* ptr = json_ptr;
				const char* function = 0;
				char* temp_str = 0;
				size_t temp_str_size = 0;
				XGCRef* gcref = deserialize_json_value( ptr , is_error , function , __file__ , __line__ , hash , 0 , ___rux__thread_index1986 , temp_str , temp_str_size , 0 );
				if( temp_str )
					::rux::engine::free_mem( temp_str );
				if( is_error == 0 )
				{				
					for( ; ; )
					{
						if( *ptr == 0 )
							break;
						switch( *ptr )
						{
						case ' ':
						case '\n':
						case '\r':
						case '\t':
							{
								break;
							}
						default:
							{
								is_error = 1;
								break;
							}
						}
						if( is_error == 1 )
							break;
						ptr++;
					}
				}
				if( is_error == 1 )
				{
					if( gcref )
					{
						gcref->Release( __FILE__ , __LINE__ );
						gcref = 0;
					}
					if( function == 0 )
						function = __FUNCTION__;
					size_t length = 0;
					uint32 column = 0 , line = 1;
					char* next = json_ptr;
					while( next != ptr )
					{
						if( ( *next & 0xc0 ) != 0x80 )
						{					
							if( length == 1 )
							{
								next--;
								if( *next == '\n' )
									line++;
								next++;
							}
							column++;
							length = 1;
						}
						else if( length != 0 )
							length++;
						if( *next == 0 )
							break;
						next++;
					}
					if( ptr - json_ptr >= 20 )
						ptr -= 20;
					else
						ptr = json_ptr;
					rux::XString json_part( ptr );
					json_part.set_ByRef( json_part.Substring( 0 , 40 ) );
					rux_write_error( "incorrect json string format here '..." + json_part + "...' , line " + rux::XUInt32( line ).ToString() + " , column " + rux::XUInt32( column ).ToString() + " , internal function = '" + function + "'"  , error );
				}
				CODE_LABEL_END();
				return gcref;
			}
		};
		Object& XJsonSerializer::Deserialize( char* json_ptr , uint8 is_utf8 , rux::XString& error , const char* __file__ , ::rux::int32 __line__ , ::rux::uint32* hash , size_t ___rux__thread_index1986 )
		{
			return XObject( DeserializeAndReturnGCRef( json_ptr , is_utf8 , error , __file__ , __line__ , hash , ___rux__thread_index1986 ) , 0 , "object" , __file__ , __line__ )++;
		}
		Object& XJsonSerializer::Deserialize( const rux::String& json , rux::XString& error , const char* __file__ , ::rux::int32 __line__ , ::rux::uint32* hash , size_t ___rux__thread_index1986 )
		{
			if( json._code_page == XEnumCodePage_UTF8 )
				return Deserialize( json.str() , 1 , error , __file__ , __line__ , hash , ___rux__thread_index1986 );
			else
			{
				declare_variable( rux::XString , string );
				string.set_ByRef( json.ConvertToUTF8() );	
				return Deserialize( string.str() , 1 , error , __file__ , __line__ , hash , ___rux__thread_index1986 );
			}
		};
		Object& XJsonSerializer::Deserialize( const rux::XString& json , rux::XString& error , const char* __file__ , ::rux::int32 __line__ , ::rux::uint32* hash , size_t ___rux__thread_index1986 )
		{
			return Deserialize( *json() , error , __file__ , __line__ , hash , ___rux__thread_index1986 );
		};
		XGCRef* XJsonSerializer::DeserializeAndReturnGCRef( const rux::String& json , rux::XString& error , const char* __file__ , ::rux::int32 __line__ , ::rux::uint32* hash , size_t ___rux__thread_index1986 )
		{
			if( json._code_page == XEnumCodePage_UTF8 )
				return DeserializeAndReturnGCRef( json.str() , 1 , error , __file__ , __line__ , hash , ___rux__thread_index1986 );
			else
			{
				declare_variable( rux::XString , string );	
				string.set_ByRef( json.ConvertToUTF8() );	
				return DeserializeAndReturnGCRef( string.str() , 1 , error , __file__ , __line__ , hash , ___rux__thread_index1986 );
			}
		};
		XGCRef* XJsonSerializer::DeserializeAndReturnGCRef( const rux::XString& json , rux::XString& error , const char* __file__ , ::rux::int32 __line__ , ::rux::uint32* hash , size_t ___rux__thread_index1986 )
		{
			return DeserializeAndReturnGCRef( *json() , error , __file__ , __line__ , hash , ___rux__thread_index1986 );
		};
		rux::String& XJsonSerializer::Serialize( XGCRef* object , uint8 is_formatted , rux::XString& error , size_t step , size_t ___rux__thread_index1986 )
		{
			CODE_LABEL_START( 0 , 0 , 30 );
			error.Clear();
			char* json = NULL , * json_cursor = NULL;
			if( step == 0 )
				step = 32;
			size_t json_size = 0;
			uint32 tab_count= 0;
			serialize_json_value( object , json , json_size , json_cursor , is_formatted , tab_count , step , error , __FILE__ , __LINE__ , ___rux__thread_index1986 );
			if( error.Length() == 0 )
			{
				size_t json_index = json_cursor - json;
				rux_check_json_string_memory( json , json_size , json_cursor , 1 , step , __FILE__ , __LINE__ );
				json[ json_index ] = '\0';
				json_index++;
				rux::XString new_str( (size_t)0 , "new_str" , __FILE__ , __LINE__ );
				rux::String* json_result_internal = new_str();
				json_result_internal->_code_page = XEnumCodePage_UTF8;
				json_result_internal->_str = json;
				json_result_internal->_count = json_index;
				json_result_internal->_size = json_size;
				CODE_LABEL_END();
				return new_str++;
			}
			else
			{
				CODE_LABEL_END();
				return rux::XString()++;
			}
		};
		rux::String& XJsonSerializer::Serialize( XObject& object , uint8 is_formatted , rux::XString& error , size_t step , size_t ___rux__thread_index1986 )
		{
			return Serialize( object.get_GCRef( 1 ) , is_formatted , error , step , ___rux__thread_index1986 );
		};		
		void XJsonSerializer::serialize_json_value( XObject& object , char*& json_ptr , size_t& json_ptr_size , char*& json_cursor , uint8 is_formatted , uint32 tab_count , size_t& step , rux::XString& error , const char* __file__ , ::rux::int32 __line__ , size_t ___rux__thread_index1986 )
		{
			serialize_json_value( object.get_GCRef( 1 ) , json_ptr , json_ptr_size , json_cursor , is_formatted , tab_count , step , error , __file__ , __line__ , ___rux__thread_index1986 );
		};
		void XJsonSerializer::serialize_json_value( XGCRef* object , char*& json_ptr , size_t& json_ptr_size , char*& json_cursor , uint8 is_formatted , uint32 tab_count , size_t& step , rux::XString& error , const char* __file__ , ::rux::int32 __line__ , size_t ___rux__thread_index1986 )
		{				
			CODE_LABEL_START( 0 , 0 , 5 );
			if( rux_is_object_ptr( object , rux::data::XDataObject ) )
				serialize_json_object_value( (rux::data::DataObject*)( object ) , json_ptr , json_ptr_size , json_cursor , is_formatted , tab_count , step , error , __file__ , __line__ , ___rux__thread_index1986 );
			else if( rux_is_object_ptr( object , rux::XInt64 ) )
				serialize_json_int64_value( ((rux::Int64*)object )->_value , json_ptr , json_ptr_size , json_cursor , step , __file__ , __line__ , ___rux__thread_index1986 );
			else if( rux_is_object_ptr( object , rux::XUInt64 ) )
				serialize_json_uint64_value( ((rux::UInt64*)object )->_value , json_ptr , json_ptr_size , json_cursor , step , __file__ , __line__ , ___rux__thread_index1986 );
			else if( rux_is_object_ptr( object , rux::XInt32 ) )
				serialize_json_int32_value( ((rux::Int32*)object )->_value , json_ptr , json_ptr_size , json_cursor , step , __file__ , __line__ , ___rux__thread_index1986 );
			else if( rux_is_object_ptr( object , rux::XUInt32 ) )
				serialize_json_uint32_value( ((rux::UInt32*)object )->_value , json_ptr , json_ptr_size , json_cursor , step , __file__ , __line__ , ___rux__thread_index1986 );
			else if( rux_is_object_ptr( object , rux::XDouble ) )
				serialize_json_double_value( ((rux::Double*)object )->_value , json_ptr , json_ptr_size , json_cursor , step , __file__ , __line__ , ___rux__thread_index1986 );
			else if( rux_is_object_ptr( object , rux::XFloat ) )
				serialize_json_float_value( ((rux::Float*)object )->_value , json_ptr , json_ptr_size , json_cursor , step , __file__ , __line__ , ___rux__thread_index1986 );
			else if( rux_is_object_ptr( object , rux::XBoolean ) )
				serialize_json_boolean_value( ((rux::Boolean*)object )->_value , json_ptr , json_ptr_size , json_cursor , step , __file__ , __line__ , ___rux__thread_index1986 );
			else if( rux_is_object_ptr( object , rux::XNull ) )
				serialize_json_null_value( json_ptr , json_ptr_size , json_cursor , step , __file__ , __line__ , ___rux__thread_index1986 );
			else if( rux_is_object_ptr( object , rux::XString ) )
			{
				if( ((rux::String*)object)->_code_page != XEnumCodePage_UTF8 )
				{
					declare_variable( ::rux::XString , utf8 );
					utf8.set_ByRef( ((rux::String*)object)->ConvertToUTF8() );
					serialize_json_string_value( utf8.str() , json_ptr , json_ptr_size , json_cursor , step , __file__ , __line__ , ___rux__thread_index1986 );
				}
				else
					serialize_json_string_value( ((rux::String*)object )->_str , json_ptr , json_ptr_size , json_cursor , step , __file__ , __line__ , ___rux__thread_index1986 );
			}
			else if( object->get_Type().get_IsArray() )
				serialize_json_array_value( object , json_ptr , json_ptr_size , json_cursor , is_formatted , tab_count , step , error , __file__ , __line__ , ___rux__thread_index1986 );
			else if( rux_is_object_ptr( object , rux::XNumeric ) )
			{
				rux::Numeric* value = (rux::Numeric*)object->get_ObjectAddress();
				if( value->_value_gcref )
					serialize_json_value( value->_value_gcref , json_ptr , json_ptr_size , json_cursor , is_formatted , tab_count , step , error , __file__ , __line__ , ___rux__thread_index1986 );
				else
				{
					XObject obj( *object , "object" , __file__ , __line__ );
					XObject res( XDataObject::FromStruct( obj ) , "object" , __file__ , __line__ );
					serialize_json_value( res , json_ptr , json_ptr_size , json_cursor , is_formatted , tab_count , step , error , __file__ , __line__ , ___rux__thread_index1986 );
				}
			}
			else
			{
				XObject obj( *object , "object" , __file__ , __line__ );
				XObject res( XDataObject::FromStruct( obj ) , "object" , __file__ , __line__ );
				serialize_json_value( res , json_ptr , json_ptr_size , json_cursor , is_formatted , tab_count , step , error , __file__ , __line__ , ___rux__thread_index1986 );
			}
			CODE_LABEL_END();
		};
		void XJsonSerializer::serialize_json_object_value( rux::data::DataObject* object , char*& json_ptr , size_t& json_ptr_size , char*& json_cursor , uint8 is_formatted , uint32 tab_count , size_t& step , rux::XString& error , const char* __file__ , ::rux::int32 __line__ , size_t ___rux__thread_index1986 )
		{
			CODE_LABEL_START( 0 , 0 , 5 );
			rux_check_json_string_memory( json_ptr , json_ptr_size , json_cursor , 2 , step , __FILE__ , __LINE__ );
			*json_cursor++ = '{';
			if( is_formatted == 1 )
				*json_cursor++ = '\n';
			tab_count++;
			//object->_pairs.Begin();
			for( uint32 index0 = 0 ; index0 < object->_pairs.Count() ; index0++ )
			{
				if( is_formatted == 1 )
				{
					rux_check_json_string_memory( json_ptr , json_ptr_size , json_cursor , tab_count , step , __FILE__ , __LINE__ );
					for( uint32 index1 = 0 ; index1 < tab_count ; index1++ )
						*json_cursor++ = '\t';
				}
				serialize_json_string_value( object->get_FieldNamePointer( index0 ) , json_ptr , json_ptr_size , json_cursor , step , __file__ , __line__ , ___rux__thread_index1986 );
				rux_check_json_string_memory( json_ptr , json_ptr_size , json_cursor , 1 , step , __FILE__ , __LINE__ );
				*json_cursor++ = ':';
				::rux::data::key_value_pair_t* pair = object->_pairs[ index0 ]->operator->();
				if( pair->_type == ::rux::data::key_value_pair_t_type_gcref )
				{
					XGCRef* gcref = pair->_gcref;
					if( gcref->IsObjectType() )
					{
						gcref = ((Object*)gcref )->_gc_ref_obj;
						if( gcref == 0 )
							gcref = pair->_gcref;
					}
					if( is_formatted == 1 )
					{
						if( rux_is_object_ptr( gcref , rux::data::XDataObject ) 
							|| rux_is_object_ptr( gcref , rux::XArray< XObject > ) )  
						{
							rux_check_json_string_memory( json_ptr , json_ptr_size , json_cursor , tab_count + 1 , step , __FILE__ , __LINE__ );
							*json_cursor++ = '\n';
							for( uint32 index1 = 0 ; index1 < tab_count ; index1++ )
								*json_cursor++ = '\t';
						}
					}
					serialize_json_value( gcref , json_ptr , json_ptr_size , json_cursor , is_formatted , tab_count , step , error , __file__ , __line__ , ___rux__thread_index1986 );
				}
				else
				{
					if( pair->_type == key_value_pair_t_type_small_utf8_string )
						serialize_json_string_value( pair->_small_utf8_string , json_ptr , json_ptr_size , json_cursor , step , __file__ , __line__ , ___rux__thread_index1986 );
					else if( pair->_type == key_value_pair_t_type_large_utf8_string )
						serialize_json_string_value( pair->_large_utf8_string , json_ptr , json_ptr_size , json_cursor , step , __file__ , __line__ , ___rux__thread_index1986 );
					else if( pair->_type == key_value_pair_t_type_float )
						serialize_json_float_value( pair->_float , json_ptr , json_ptr_size , json_cursor , step , __file__ , __line__ , ___rux__thread_index1986 );
					else if( pair->_type == key_value_pair_t_type_double )
						serialize_json_double_value( pair->_double , json_ptr , json_ptr_size , json_cursor , step , __file__ , __line__ , ___rux__thread_index1986 );
					else if( pair->_type == key_value_pair_t_type_int32 )
						serialize_json_int32_value( pair->_int32_val , json_ptr , json_ptr_size , json_cursor , step , __file__ , __line__ , ___rux__thread_index1986 );
					else if( pair->_type == key_value_pair_t_type_uint32 )
						serialize_json_uint32_value( pair->_uint32 , json_ptr , json_ptr_size , json_cursor , step , __file__ , __line__ , ___rux__thread_index1986 );
					else if( pair->_type == key_value_pair_t_type_uint64 )
						serialize_json_uint64_value( pair->_uint64 , json_ptr , json_ptr_size , json_cursor , step , __file__ , __line__ , ___rux__thread_index1986 );
					else if( pair->_type == key_value_pair_t_type_int64 )
						serialize_json_int64_value( pair->_int64_val , json_ptr , json_ptr_size , json_cursor , step , __file__ , __line__ , ___rux__thread_index1986 );
					else if( pair->_type == key_value_pair_t_type_boolean )
						serialize_json_boolean_value( pair->_boolean , json_ptr , json_ptr_size , json_cursor , step , __file__ , __line__ , ___rux__thread_index1986 );
					else
						serialize_json_null_value( json_ptr , json_ptr_size , json_cursor , step , __file__ , __line__ , ___rux__thread_index1986 );
				}
				if( error.Length() > 0 )
					break;
				if( index0 + 1 < object->_pairs.Count() )
				{
					rux_check_json_string_memory( json_ptr , json_ptr_size , json_cursor , 1 , step , __FILE__ , __LINE__ );
					*json_cursor++ = ',';
				}
				if( is_formatted == 1 )
				{
					rux_check_json_string_memory( json_ptr , json_ptr_size , json_cursor , 1 , step , __FILE__ , __LINE__ );
					*json_cursor++ = '\n';
				}
			}
			tab_count--;
			if( error.Length() == 0 )
			{
				if( is_formatted == 1 )
				{
					rux_check_json_string_memory( json_ptr , json_ptr_size , json_cursor , tab_count , step , __FILE__ , __LINE__ );
					for( uint32 index1 = 0 ; index1 < tab_count ; index1++ )
						*json_cursor++ = '\t';
				}
				rux_check_json_string_memory( json_ptr , json_ptr_size , json_cursor , 2 , step , __FILE__ , __LINE__ );
				*json_cursor++ = '}';
				*json_cursor = 0;
			}
			CODE_LABEL_END();
		};
		void XJsonSerializer::serialize_json_boolean_value( ::rux::byte object , char*& json_ptr , size_t& json_ptr_size , char*& json_cursor , size_t& step , const char* __file__ , ::rux::int32 __line__ , size_t ___rux__thread_index1986 )
		{
			CODE_LABEL_START( 0 , 0 , 5 );
			if( object )
			{
				rux_check_json_string_memory( json_ptr , json_ptr_size , json_cursor , 5 , step , __file__ , __line__ );
				::memcpy( json_cursor , "true" , 4 );
				json_cursor += 4;
			}
			else
			{
				rux_check_json_string_memory( json_ptr , json_ptr_size , json_cursor , 6 , step , __file__ , __line__ );
				::memcpy( json_cursor , "false" , 5 );
				json_cursor += 5;
			}
			*json_cursor = 0;
			CODE_LABEL_END();
		};
		void XJsonSerializer::serialize_json_null_value( char*& json_ptr , size_t& json_ptr_size , char*& json_cursor , size_t& step , const char* __file__ , ::rux::int32 __line__ , size_t ___rux__thread_index1986 )
		{
			CODE_LABEL_START( 0 , 0 , 5 );
			rux_check_json_string_memory( json_ptr , json_ptr_size , json_cursor , 5 , step , __file__ , __line__ );
			::memcpy( json_cursor , "null" , 4 );
			json_cursor += 4;
			*json_cursor = 0;
			CODE_LABEL_END();
		};
		void XJsonSerializer::serialize_json_int64_value( rux::int64 object , char*& json_ptr , size_t& json_ptr_size , char*& json_cursor , size_t& step , const char* __file__ , ::rux::int32 __line__ , size_t ___rux__thread_index1986 )
		{
			CODE_LABEL_START( 0 , 0 , 5 );
			size_t length = ::rux::_scprintf( I64d , object ) + 1;
			rux_check_json_string_memory( json_ptr , json_ptr_size , json_cursor , length + 1 , step , __file__ , __line__ );
			::rux::safe_sprintf( json_cursor , length , I64d , object );
			json_cursor += length - 1;
			*json_cursor = 0;
			CODE_LABEL_END();
		};
		void XJsonSerializer::serialize_json_uint64_value( rux::uint64 object , char*& json_ptr , size_t& json_ptr_size , char*& json_cursor , size_t& step , const char* __file__ , ::rux::int32 __line__ , size_t ___rux__thread_index1986 )
		{
			CODE_LABEL_START( 0 , 0 , 5 );
			size_t length = ::rux::_scprintf( I64u , object ) + 1;
			rux_check_json_string_memory( json_ptr , json_ptr_size , json_cursor , length + 1 , step , __file__ , __line__ );
			::rux::safe_sprintf( json_cursor , length , I64u , object );
			json_cursor += length - 1;
			*json_cursor = 0;
			CODE_LABEL_END();
		};
		void XJsonSerializer::serialize_json_uint32_value( rux::uint32 object , char*& json_ptr , size_t& json_ptr_size , char*& json_cursor , size_t& step , const char* __file__ , ::rux::int32 __line__ , size_t ___rux__thread_index1986 )
		{
			CODE_LABEL_START( 0 , 0 , 5 );
			size_t length = ::rux::_scprintf( "%u" , object ) + 1;
			rux_check_json_string_memory( json_ptr , json_ptr_size , json_cursor , length + 1 , step , __file__ , __line__ );
			::rux::safe_sprintf( json_cursor , length , "%u" , object );
			json_cursor += length - 1;
			*json_cursor = 0;
			CODE_LABEL_END();
		};
		void XJsonSerializer::serialize_json_int32_value( rux::int32 object , char*& json_ptr , size_t& json_ptr_size , char*& json_cursor , size_t& step , const char* __file__ , ::rux::int32 __line__ , size_t ___rux__thread_index1986 )
		{
			CODE_LABEL_START( 0 , 0 , 5 );
			size_t length = ::rux::_scprintf( "%d" , object ) + 1;
			rux_check_json_string_memory( json_ptr , json_ptr_size , json_cursor , length + 1 , step , __file__ , __line__ );
			::rux::safe_sprintf( json_cursor , length , "%d" , object );
			json_cursor += length - 1;
			*json_cursor = 0;
			CODE_LABEL_END();
		};
		void XJsonSerializer::serialize_json_double_value( double object , char*& json_ptr , size_t& json_ptr_size , char*& json_cursor , size_t& step , const char* __file__ , ::rux::int32 __line__ , size_t ___rux__thread_index1986 )
		{
			CODE_LABEL_START( 0 , 0 , 5 );
			size_t length = ::rux::_scprintf( "%f" , object ) + 1;
			rux_check_json_string_memory( json_ptr , json_ptr_size , json_cursor , length + 1 , step , __file__ , __line__ );
			::rux::safe_sprintf( json_cursor , length , "%f" , object );
			char* ptr = strstr( json_cursor , "," );
			if( ptr )
				*ptr = '.';
			json_cursor += length - 1;
			*json_cursor = 0;
			CODE_LABEL_END();
		};
		void XJsonSerializer::serialize_json_float_value( float object , char*& json_ptr , size_t& json_ptr_size , char*& json_cursor , size_t& step , const char* __file__ , ::rux::int32 __line__ , size_t ___rux__thread_index1986 )
		{
			CODE_LABEL_START( 0 , 0 , 5 );
			size_t length = ::rux::_scprintf( "%f" , object ) + 1;
			rux_check_json_string_memory( json_ptr , json_ptr_size , json_cursor , length + 1 , step , __file__ , __line__ );
			::rux::safe_sprintf( json_cursor , length , "%f" , object );
			char* ptr = strstr( json_cursor , "," );
			if( ptr )
				*ptr = '.';
			json_cursor += length - 1;
			*json_cursor = 0;
			CODE_LABEL_END();
		};
		void XJsonSerializer::serialize_json_string_value( char* object , char*& json_ptr , size_t& json_ptr_size , char*& json_cursor , size_t& step , const char* __file__ , ::rux::int32 __line__ , size_t ___rux__thread_index1986 )
		{
			CODE_LABEL_START( 0 , 0 , 5 );
			if( object )
			{
				size_t object_len = strlen( object ) + 1;
				rux_check_json_string_memory( json_ptr , json_ptr_size , json_cursor , object_len + 1 , step , __file__ , __line__ );
				*json_cursor++ = '"';
				char* begin = object , * end = &begin[ object_len - 1 ] , * ptr = NULL;
				for( ; ; )
				{
					ptr = strpbrk( begin , "\\/\b\n\f\r\t\"" );
					if( ptr )
					{			
						rux_check_json_string_memory( json_ptr , json_ptr_size , json_cursor , end - begin + 2 , step , __file__ , __line__ );
						if( ptr != begin )
						{
							::memcpy( json_cursor , begin , ptr - begin );
							json_cursor += ptr - begin;
						}
						*json_cursor++ = '\\';
						switch( *ptr )
						{
						case '\b':
							{
								*json_cursor++ = 'b';
								break;
							}
						case '\n':
							{
								*json_cursor++ = 'n';
								break;
							}
						case '\f':
							{
								*json_cursor++ = 'f';
								break;
							}
						case '\r':
							{
								*json_cursor++ = 'r';
								break;
							}
						case '\t':
							{
								*json_cursor++ = 't';
								break;
							}
						default:
							{
								*json_cursor++ = *ptr;
								break;
							}
						}
						begin = ptr + 1;
					}
					else if( end != begin )
					{
						rux_check_json_string_memory( json_ptr , json_ptr_size , json_cursor , end - begin , step , __file__ , __line__ );
						::memcpy( json_cursor , begin , end - begin );
						json_cursor += end - begin;
						break;
					}
					else
						break;
				}	
				rux_check_json_string_memory( json_ptr , json_ptr_size , json_cursor , 2 , step , __file__ , __line__ );
				*json_cursor++ = '"';
				*json_cursor = 0;
			}
			else
			{
				rux_check_json_string_memory( json_ptr , json_ptr_size , json_cursor , 3 , step , __file__ , __line__ );
				*json_cursor++ = '"';
				*json_cursor++ = '"';
				*json_cursor = 0;
			}
			CODE_LABEL_END();
		};
		void XJsonSerializer::serialize_json_array_value( const XGCRef* object , char*& json_ptr , size_t& json_ptr_size , char*& json_cursor , uint8 is_formatted , uint32 tab_count , size_t& step , rux::XString& error , const char* __file__ , ::rux::int32 __line__ , size_t ___rux__thread_index1986 )
		{
			CODE_LABEL_START( 0 , 0 , 5 );
			rux_check_json_string_memory( json_ptr , json_ptr_size , json_cursor , 2 , step , __file__ , __line__ );
			*json_cursor++ = '[';
			if( is_formatted == 1 )
				*json_cursor++ = '\n';
			tab_count++;
			Type* type = &object->get_Type();
			if( type->get_IsArray() == 1 )
			{
				::rux::PropertyInfo* property_info = type->get_PropertyInfo( ::rux::compile::times33_hash< 'C','o','u','n','t' >::value );
				if( property_info )
				{
					::rux::FunctionInfo* function_info = type->get_FunctionInfo( ::rux::compile::times33_hash< 'g','e','t','_','I','t','e','m' >::value );
					if( function_info )
					{
						size_t count = (size_t)::rux::gcref_to_uint64( &property_info->Get( object ) );
						for( size_t index0 = 0 ; index0 < count ; index0++ )
						{
							XGCRef* item = &function_info->InvokeResult( object , ::rux::uint64_to_gcref( index0 ) );
							XGCRef* gcref = item;
							if( item->IsObjectType() )
							{
								gcref = ((Object*)item->get_ObjectAddress())->_gc_ref_obj;
								if( gcref == NULL )
									gcref = item;
							}
							if( is_formatted == 1 )
							{
								rux_check_json_string_memory( json_ptr , json_ptr_size , json_cursor , tab_count , step , __FILE__ , __LINE__ );
								for( uint32 index1 = 0 ; index1 < tab_count ; index1++ )
									*json_cursor++ = '\t';
							}
							serialize_json_value( gcref , json_ptr , json_ptr_size , json_cursor , is_formatted , tab_count , step , error , __file__ , __line__ , ___rux__thread_index1986 );
							if( error.Length() > 0 )
								break;
							if( index0 + 1 < count )
							{
								rux_check_json_string_memory( json_ptr , json_ptr_size , json_cursor , 1 , step , __file__ , __line__ );
								*json_cursor++ = ',';
							}
							if( is_formatted == 1 )
							{
								rux_check_json_string_memory( json_ptr , json_ptr_size , json_cursor , 1 , step , __file__ , __line__ );
								*json_cursor++ = '\n';
							}
						}
					}
				}
			}
			tab_count--;
			if( error.Length() == 0 )
			{
				if( is_formatted == 1 )
				{
					rux_check_json_string_memory( json_ptr , json_ptr_size , json_cursor , tab_count , step , __FILE__ , __LINE__ );
					for( uint32 index1 = 0 ; index1 < tab_count ; index1++ )
						*json_cursor++ = '\t';
				}
				rux_check_json_string_memory( json_ptr , json_ptr_size , json_cursor , 1 , step , __file__ , __line__ );
				*json_cursor++ = ']';
			}
			CODE_LABEL_END();
		};
	};
};
void rux::io::XDirectory::CreateDirectoriesStructureFromJSON( const rux::XString& root_directory , const rux::XString& json , rux::XString& error )
{
	if( XDirectory::Exists( root_directory ) == 1 )
	{		
		XObject json_object( rux::data::XJsonSerializer::Deserialize( json , error , __FILE__ , __LINE__ ) , "object" , __FILE__ , __LINE__ );
		if( error.Length() == 0 )
		{
			if( rux_is_object( json_object , rux::data::XDataObject ) )
			{
				declare_variable_param( rux::data::XDataObject , data_object0 , json_object );
				if( data_object0.FieldsCount() == 1 )
				{
					if( data_object0.get_FieldName( 0 ).Equals( "directories" , 11 ) )
					{
						json_object = data_object0.get_FieldValue( 0 );
						if( rux_is_object( json_object , rux::XArray< XObject > ) )
						{
							rux::XArray< XObject > values( "object" , __FILE__ , __LINE__ );
							declare_variable( rux::XString , directory );
							declare_variable( ::rux::XString , current_cp_root_directory );
							current_cp_root_directory = root_directory;
							current_cp_root_directory.set_ByRef( current_cp_root_directory.ReplaceAll( '\\' , '/' ) );
							if( current_cp_root_directory.get_UTF8Char( 0 ) == '/' )
								current_cp_root_directory.RemoveAt( 0 );
							rux::XArray< rux::XString > dirs;
							values.set_ByRef( json_object );
							uint32 index1 = 0;
							for( uint32 index0 = 0 ; index0 < values.Count() ; index0++ )
							{
								if( rux_is_object( values[ index0 ] , rux::XString ) )
								{
									directory.set_ByRef( values[ index0 ] );								
									directory.set_ByRef( directory.ReplaceAll( '\\' , '/' ) );
									if( directory.get_UTF8Char( 0 ) == '/' )
										directory.RemoveAt( 0 );
									dirs.set_ByRef( directory.Split( "/" , EStringSplitOptions_RemoveEmptyEntries ) );
									directory = current_cp_root_directory;
									for( index1 = 0 ; index1 < dirs.Count() ; index1++ )
									{
										directory += "/" + dirs[ index1 ];
										if( XDirectory::Exists( directory ) == 0 )
											XDirectory::Create( directory , error );
										if( error.Length() == 0 )
											break;
									}
								}
								if( error.Length() == 0 )
									break;
							}
						}
					}
				}
			}
		}
	}
	else 
	{
		rux_write_error( "root directory '" + root_directory + "' does not exist" , error );
	}
};
rux::String& rux::io::XDirectory::get_JSONDirectoriesStructure( const rux::XString& directory_name )
{
	rux::XArray< rux::io::XDirectory > dirs;
	dirs.set_ByRef( XDirectory::GetDirectories( directory_name , XAllDirectories ) );
	rux::XString json = "{\"directories\":[" , string;
	if( dirs.Count() > 0 )
	{
		for( uint32 index0 = dirs.Count() - 1 ; index0 >= 0 ; index0-- )
		{
			if( index0 < dirs.Count() - 1 )
				json += ",";
			string = dirs[ index0 ].get_DirectoryName();
			string.RemoveRange( 0 , directory_name.Length() + 1 );
			json += "\"" + string + "\"";
			if( index0 == 0 )
				break;
		}
	}
	json += "]}";
	json.set_ByRef( json.ReplaceAll( "\"" , "\"\"" ) );
	json.set_ByRef( json.ConvertToUTF8() );
	return json++;
};
void rux::io::XDirectory::DeleteDirectoriesStructureFromJSON( const rux::XString& root_directory , const rux::XString& json , rux::XString& error )
{
	if( XDirectory::Exists( root_directory ) == 1 )
	{		
		XObject json_object( rux::data::XJsonSerializer::Deserialize( json , error , __FILE__ , __LINE__ ) , "object" , __FILE__ , __LINE__ );
		if( error.Length() == 0 )
		{
			if( rux_is_object( json_object , rux::data::XDataObject ) )
			{
				declare_variable_param( rux::data::XDataObject , data_object0 , json_object );
				if( data_object0.FieldsCount() == 1 )
				{
					if( data_object0.get_FieldName( 0 ).Equals( "directories" , 11 ) )
					{
						json_object = data_object0.get_FieldValue( 0 );
						if( rux_is_object( json_object , rux::XArray< XObject > ) )
						{
							rux::XArray< XObject > values( "object" , __FILE__ , __LINE__ );
							declare_variable( rux::XString , directory );
							declare_variable( ::rux::XString , current_cp_root_directory );
							current_cp_root_directory = root_directory;
							current_cp_root_directory.set_ByRef( current_cp_root_directory.ReplaceAll( '\\' , '/' ) );
							if( current_cp_root_directory.get_UTF8Char( 0 ) == '/' )
								current_cp_root_directory.RemoveAt( 0 );							
							values.set_ByRef( json_object );							
							rux::XArray< XFile > files;
							for( uint32 index0 = 0 ; index0 < values.Count() ; index0++ )
							{
								if( rux_is_object( values[ index0 ] , rux::XString ) )
								{
									directory.set_ByRef( values[ index0 ] );								
									directory.set_ByRef( directory.ReplaceAll( '\\' , '/' ) );
									if( directory.get_UTF8Char( 0 ) == '/' )
										directory.RemoveAt( 0 );									
									directory.set_ByRef( current_cp_root_directory + "/" + directory );
									if( XDirectory::Exists( directory ) == 1 )
									{
										files.set_ByRef( XDirectory::GetFiles( directory , XAllDirectories )  );
										if( files.Count() == 0 )									
											XDirectory::Delete( directory );
									}
								}
							}
						}
					}
				}
			}
		}
	}
	else 
	{
		rux_write_error( "root directory '" + root_directory + "' does not exist" , error );
	}
};