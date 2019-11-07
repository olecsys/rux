#ifndef XAN_JSONSERIALIZER_H
#define XAN_JSONSERIALIZER_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_dataobject.h"
#include "xan_int32.h"
#include "xan_int64.h"
#include "xan_uint32.h"
#include "xan_uint64.h"
#include "xan_boolean.h"
#include "xan_null.h"
#include "xan_double.h"
#include "xan_float.h"
#include "xan_numeric.h"
#include "xan_crtsect.h"
#include "xan_property_info.h"
namespace rux
{
	namespace data
	{	
		namespace json
		{
			enum serializer_state
			{
				serializer_state_object , 
				serializer_state_array ,
				serializer_state_unknown
			};
			class serializer
			{
			private:
				XMallocArray< ::rux::uint32 > _states;
				::rux::byte _json_delete;
				::rux::byte _formatted;
				char* _json;
				char* _json_cursor;
				size_t _step;
				size_t _json_size;
				uint32 _tab_count;
				::rux::byte _need_delimiter;
				::rux::byte check_formatted_and_name( const char* utf8_name , const char* __file__ , ::rux::int32 __line__ , size_t ___rux__thread_index1986 );
				void generate_object_name( const char* utf8_name , const char* __file__ , ::rux::int32 __line__ , size_t ___rux__thread_index1986 );
			public:
				serializer( ::rux::byte formatted , ::rux::byte json_delete );
				~serializer( void );
				void begin_object( const char* utf8_name , const char* __file__ , ::rux::int32 __line__ , size_t ___rux__thread_index1986 );
				void end_object( const char* __file__ , ::rux::int32 __line__ , size_t ___rux__thread_index1986 );
				void begin_array( const char* utf8_name , const char* __file__ , ::rux::int32 __line__ , size_t ___rux__thread_index1986 );
				void end_array( const char* __file__ , ::rux::int32 __line__ , size_t ___rux__thread_index1986 );
				void generate_boolean_value( const char* utf8_name , ::rux::byte object , const char* __file__ , ::rux::int32 __line__ , size_t ___rux__thread_index1986 );
				void generate_boolean_value( const char* utf8_name , bool object , const char* __file__ , ::rux::int32 __line__ , size_t ___rux__thread_index1986 );
				void generate_null_value( const char* utf8_name , const char* __file__ , ::rux::int32 __line__ , size_t ___rux__thread_index1986 );
				void generate_int64_value( const char* utf8_name , rux::int64 object , const char* __file__ , ::rux::int32 __line__ , size_t ___rux__thread_index1986 );
				void generate_uint64_value( const char* utf8_name , rux::uint64 object , const char* __file__ , ::rux::int32 __line__ , size_t ___rux__thread_index1986 );
				void generate_uint32_value( const char* utf8_name , rux::uint32 object , const char* __file__ , ::rux::int32 __line__ , size_t ___rux__thread_index1986 );
				void generate_int32_value( const char* utf8_name , rux::int32 object , const char* __file__ , ::rux::int32 __line__ , size_t ___rux__thread_index1986 );
				void generate_double_value( const char* utf8_name , double object , const char* __file__ , ::rux::int32 __line__ , size_t ___rux__thread_index1986 );
				void generate_float_value( const char* utf8_name , float object , const char* __file__ , ::rux::int32 __line__ , size_t ___rux__thread_index1986 );
				void generate_string_value( const char* utf8_name , const char* object , const char* __file__ , ::rux::int32 __line__ , size_t ___rux__thread_index1986 );
				void generate_object_value( const char* utf8_name , ::rux::data::DataObject* object , ::rux::XString& error , const char* __file__ , ::rux::int32 __line__ , size_t ___rux__thread_index1986 );
				char* json( void );
				size_t json_size( void );
				void json_detach( ::rux::XString& dst );
			};
		};
		class XJsonSerializer
		{
			friend class ::rux::data::json::serializer;
		public:	
			static ::rux::byte IsValidJSON( const rux::XString& json , rux::XString& error , const char* __file__ = NULL , ::rux::int32 __line__ = 0 , ::rux::uint32* hash = 0 , size_t ___rux__thread_index1986 = SIZE_MAX );
			static ::rux::byte IsValidJSON( char* json_ptr , uint8 is_utf8 , rux::XString& error , const char* __file__ = NULL , ::rux::int32 __line__ = 0 , ::rux::uint32* hash = 0 , size_t ___rux__thread_index1986 = SIZE_MAX );
			static Object& Deserialize( const rux::String& json , rux::XString& error , const char* __file__ = NULL , ::rux::int32 __line__ = 0 , ::rux::uint32* hash = 0 , size_t ___rux__thread_index1986 = SIZE_MAX );
			static Object& Deserialize( const rux::XString& json , rux::XString& error , const char* __file__ = NULL , ::rux::int32 __line__ = 0 , ::rux::uint32* hash = 0 , size_t ___rux__thread_index1986 = SIZE_MAX );
			static Object& Deserialize( char* json_ptr , uint8 is_utf8 , rux::XString& error , const char* __file__ = NULL , ::rux::int32 __line__ = 0 , ::rux::uint32* hash = 0 , size_t ___rux__thread_index1986 = SIZE_MAX );
			static XGCRef* DeserializeAndReturnGCRef( const rux::XString& json , rux::XString& error , const char* __file__ = NULL , ::rux::int32 __line__ = 0 , ::rux::uint32* hash = 0 , size_t ___rux__thread_index1986 = SIZE_MAX );
			static XGCRef* DeserializeAndReturnGCRef( const rux::String& json , rux::XString& error , const char* __file__ = NULL , ::rux::int32 __line__ = 0 , ::rux::uint32* hash = 0 , size_t ___rux__thread_index1986 = SIZE_MAX );
			static XGCRef* DeserializeAndReturnGCRef( char* json_ptr , uint8 is_utf8 , rux::XString& error , const char* __file__ = NULL , ::rux::int32 __line__ = 0 , ::rux::uint32* hash = 0 , size_t ___rux__thread_index1986 = SIZE_MAX );
			static rux::String& Serialize( XObject& object , uint8 is_formatted , rux::XString& error , size_t step = 32 , size_t ___rux__thread_index1986 = SIZE_MAX );
			static rux::String& Serialize( XGCRef* object , uint8 is_formatted , rux::XString& error , size_t step = 32 , size_t ___rux__thread_index1986 = SIZE_MAX );
		private:
			static XGCRef* deserialize_json_value( char*& json_ptr , uint8& is_error , const char* function , const char* __file__ , ::rux::int32 __line__ , ::rux::uint32* hash , ::rux::data::key_value_pair_t* key_value_pair , size_t ___rux__thread_index1986 , char*& temp_str , size_t& temp_str_size , ::rux::byte is_valid_json_check_only );
			static XGCRef* deserialize_json_object_value( char*& ptr , uint8& is_error , const char* function , const char* __file__ , ::rux::int32 __line__ , ::rux::uint32* hash , size_t ___rux__thread_index1986 , char*& temp_str , size_t& temp_str_size , ::rux::byte is_valid_json_check_only );
			static XGCRef* deserialize_json_true_boolean_value( char*& ptr , uint8& is_error , const char* function , const char* __file__ , ::rux::int32 __line__ , ::rux::uint32* hash , ::rux::data::key_value_pair_t* key_value_pair , size_t ___rux__thread_index1986 , ::rux::byte is_valid_json_check_only );
			static XGCRef* deserialize_json_false_boolean_value( char*& ptr , uint8& is_error , const char* function , const char* __file__ , ::rux::int32 __line__ , ::rux::uint32* hash , ::rux::data::key_value_pair_t* key_value_pair , size_t ___rux__thread_index1986 , ::rux::byte is_valid_json_check_only );
			static XGCRef* deserialize_json_null_value( char*& ptr , uint8& is_error , const char* function , const char* __file__ , ::rux::int32 __line__ , ::rux::uint32* hash , ::rux::data::key_value_pair_t* key_value_pair , size_t ___rux__thread_index1986 , ::rux::byte is_valid_json_check_only );
			static XGCRef* deserialize_json_number_value( char*& ptr , uint8& is_error , const char* function , const char* __file__ , ::rux::int32 __line__ , ::rux::uint32* hash , ::rux::data::key_value_pair_t* key_value_pair , size_t ___rux__thread_index1986 , ::rux::byte is_valid_json_check_only );
			static void deserialize_json_string_value( char*& ptr , uint8& is_error , const char* function , char*& value , size_t* length , size_t* allocated_size , const char* __file__ , ::rux::int32 __line__ , ::rux::uint32* hash , ::rux::data::key_value_pair_t* key_value_pair , size_t ___rux__thread_index1986 , ::rux::byte is_valid_json_check_only );
			static XGCRef* deserialize_json_array_value( char*& ptr , uint8& is_error , const char* function , const char* __file__ , ::rux::int32 __line__ , ::rux::uint32* hash , size_t ___rux__thread_index1986 , char*& temp_str , size_t& temp_str_size , ::rux::byte is_valid_json_check_only );

			static void serialize_json_value( XGCRef* object , char*& json_ptr , size_t& json_ptr_size , char*& json_cursor , uint8 is_formatted , uint32 tab_count , size_t& step , rux::XString& error , const char* __file__ , ::rux::int32 __line__ , size_t ___rux__thread_index1986 );
			static void serialize_json_value( XObject& object , char*& json_ptr , size_t& json_ptr_size , char*& json_cursor , uint8 is_formatted , uint32 tab_count , size_t& step , rux::XString& error , const char* __file__ , ::rux::int32 __line__ , size_t ___rux__thread_index1986 );
			static void serialize_json_object_value( rux::data::DataObject* object , char*& json_ptr , size_t& json_ptr_size , char*& json_cursor , uint8 is_formatted , uint32 tab_count , size_t& step , rux::XString& error , const char* __file__ , ::rux::int32 __line__ , size_t ___rux__thread_index1986 );
			static void serialize_json_boolean_value( ::rux::byte object , char*& json_ptr , size_t& json_ptr_size , char*& json_cursor , size_t& step , const char* __file__ , ::rux::int32 __line__ , size_t ___rux__thread_index1986 );
			static void serialize_json_null_value( char*& json_ptr , size_t& json_ptr_size , char*& json_cursor , size_t& step , const char* __file__ , ::rux::int32 __line__ , size_t ___rux__thread_index1986 );
			static void serialize_json_int64_value( rux::int64 object , char*& json_ptr , size_t& json_ptr_size , char*& json_cursor , size_t& step , const char* __file__ , ::rux::int32 __line__ , size_t ___rux__thread_index1986 );
			static void serialize_json_uint64_value( rux::uint64 object , char*& json_ptr , size_t& json_ptr_size , char*& json_cursor , size_t& step , const char* __file__ , ::rux::int32 __line__ , size_t ___rux__thread_index1986 );
			static void serialize_json_uint32_value( rux::uint32 object , char*& json_ptr , size_t& json_ptr_size , char*& json_cursor , size_t& step , const char* __file__ , ::rux::int32 __line__ , size_t ___rux__thread_index1986 );
			static void serialize_json_int32_value( rux::int32 object , char*& json_ptr , size_t& json_ptr_size , char*& json_cursor , size_t& step , const char* __file__ , ::rux::int32 __line__ , size_t ___rux__thread_index1986 );
			static void serialize_json_double_value( double object , char*& json_ptr , size_t& json_ptr_size , char*& json_cursor , size_t& step , const char* __file__ , ::rux::int32 __line__ , size_t ___rux__thread_index1986 );
			static void serialize_json_float_value( float object , char*& json_ptr , size_t& json_ptr_size , char*& json_cursor , size_t& step , const char* __file__ , ::rux::int32 __line__ , size_t ___rux__thread_index1986 );
			static void serialize_json_string_value( char* object , char*& json_ptr , size_t& json_ptr_size , char*& json_cursor , size_t& step , const char* __file__ , ::rux::int32 __line__ , size_t ___rux__thread_index1986 );
			static void serialize_json_array_value( const XGCRef* object , char*& json_ptr , size_t& json_ptr_size , char*& json_cursor , uint8 is_formatted , uint32 tab_count , size_t& step , rux::XString& error , const char* __file__ , ::rux::int32 __line__ , size_t ___rux__thread_index1986 );
		public:
			template< class T >
			static typename T::InternalClass& Deserialize( const rux::XString& json , rux::XString& error , const char* __file__ = 0 , ::rux::int32 __line__ = 0 , ::rux::uint32* hash = 0 )
			{
				T res;				
				XObject obj( ::rux::data::XJsonSerializer::Deserialize( json , error , __file__ , __line__ , hash ) );
				if( error.Length() == 0 )
					XDataObject::ToStruct( obj , res );
				return res++;
			};
			template< class T >
			static typename T::InternalClass& Deserialize( char* json_ptr , uint8 is_utf8 , rux::XString& error , const char* __file__ = 0 , ::rux::int32 __line__ = 0 , ::rux::uint32* hash = 0 )
			{
				T res;				
				XObject obj( ::rux::data::XJsonSerializer::Deserialize( json_ptr , is_utf8 , error , __file__ , __line__ , hash ) );
				if( error.Length() == 0 )
					XDataObject::ToStruct( obj , res );
				return res++;
			};
		};
		#define rux_json_start( object ) {\
			declare_variable_param( rux::data::XDataObject , _____1986__object , object );\
			XObject _____1986__value;\
			for( size_t _____1986__index0 = 0 ; _____1986__index0 < _____1986__object.FieldsCount() ; _____1986__index0++ )\
			{\
				size_t _____1986__find_field_index0 = 0;\
				_____1986__value.set_ByRef( _____1986__object.get_FieldValue( _____1986__index0 ) );
		#define rux_json_get( field ) if( field.IsObjectType() || rux_same_types_of_objects( _____1986__value , field ) )\
			{\
				::rux::uint32 field##_hash = rux::cryptography::hash::times33_hash( #field , SIZE_MAX );\
				if( _____1986__object.get_FieldNameHash( _____1986__index0 ) == field##_hash )\
				{\
					field.set_ByRef( _____1986__value );\
					continue;\
				}\
			}\
			_____1986__find_field_index0++
		#define rux_json_get_and_exists( field ) if( field.IsObjectType() || rux_same_types_of_objects( _____1986__value , field ) )\
			{\
				::rux::uint32 field##_hash = rux::cryptography::hash::times33_hash( #field , SIZE_MAX );\
				if( _____1986__object.get_FieldNameHash( _____1986__index0 ) == field##_hash )\
				{\
					field##_exists = 1;\
					field.set_ByRef( _____1986__value );\
					continue;\
				}\
			}\
			_____1986__find_field_index0++
		#define rux_json_get_by_name( field , name ) if( field.IsObjectType() || rux_same_types_of_objects( _____1986__value , field ) )\
			{\
				::rux::uint32 field##_hash = rux::cryptography::hash::times33_hash( name , SIZE_MAX );\
				if( _____1986__object.get_FieldNameHash( _____1986__index0 ) == field##_hash )\
				{\
					field.set_ByRef( _____1986__value );\
					continue;\
				}\
			}\
			_____1986__find_field_index0++
		#define rux_json_get_by_name_and_exists( field , name ) if( field.IsObjectType() || rux_same_types_of_objects( _____1986__value , field ) )\
			{\
				::rux::uint32 field##_hash = rux::cryptography::hash::times33_hash( name , SIZE_MAX );\
				if( _____1986__object.get_FieldNameHash( _____1986__index0 ) == field##_hash )\
				{\
					field.set_ByRef( _____1986__value );\
					field##_exists = 1;\
					continue;\
				}\
			}\
			_____1986__find_field_index0++
		#define rux_json_get_first( field , find_index ) if( field.IsObjectType() || rux_same_types_of_objects( _____1986__value , field ) )\
			{\
				::rux::uint32 field##_hash = rux::cryptography::hash::times33_hash( #field , SIZE_MAX );\
				if( _____1986__object.get_FieldNameHash( _____1986__index0 ) == field##_hash )\
				{\
					field.set_ByRef( _____1986__value );\
					find_index = _____1986__find_field_index0;\
					break;\
				}\
			}\
			find_index = 0xffffffff;\
			_____1986__find_field_index0++
		#define rux_json_get_first_by_name( field , name , find_index ) if( field.IsObjectType() || rux_same_types_of_objects( _____1986__value , field ) )\
			{\
				::rux::uint32 field##_hash = rux::cryptography::hash::times33_hash( name , SIZE_MAX );\
				if( _____1986__object.get_FieldNameHash( _____1986__index0 ) == field##_hash )\
				{\
					field.set_ByRef( _____1986__value );\
					find_index = _____1986__find_field_index0;\
					break;\
				}\
			}\
			find_index = 0xffffffff;\
			_____1986__find_field_index0++
		#define rux_json_end() }\
			}
	};
};
#endif
