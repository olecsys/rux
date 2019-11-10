#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_dataobject.h"
#include "xan_keyvaluepair.h"
#include "xan_boolean.h"
#include "xan_int64.h"
#include "xan_double.h"
#include "xan_float.h"
#include "xan_property_info.h"
#include "xan_constructor_info.h"
#include "xan_function_info.h"
#include "xan_uint64.h"
#include "xan_null.h"
#include "xan_int32.h"
#include "xan_uint32.h"
#include "xan_numeric.h"
begin_implement_rux_class_ns( DataObject , rux::data );
end_implement_rux_class();
namespace rux
{
	namespace data
	{
		XDataObject::XDataObject( const char* field0 , ::rux::int64 value0 , const char* field1 , ::rux::int64 value1 )
		{
			AddValue( field0 , value0 , "->" );
			AddValue( field1 , value1 , "->" );
		};
		key_value_pair_t::key_value_pair_t()
		{
			_type = key_value_pair_t_type_null;
			_name_hash = 0ULL;
		};
		key_value_pair_t::~key_value_pair_t()
		{
			clear();
		};
		void key_value_pair_t::clear( void )
		{
			if( _type == key_value_pair_t_type_large_utf8_string )
				::rux::engine::free_mem( _large_utf8_string );
			else if( _type == key_value_pair_t_type_gcref )
				_gcref->Release( __FILE__ , __LINE__ );
			_type = key_value_pair_t_type_null;
		};
		Type* key_value_pair_t::get_type( void )
		{
			if( _type == key_value_pair_t_type_small_utf8_string || _type == key_value_pair_t_type_large_utf8_string )
				return typeofptr( ::rux::XString );
			else if( _type == key_value_pair_t_type_gcref )
				return &_gcref->get_Type();
			else if( _type == key_value_pair_t_type_float )
				return typeofptr( ::rux::XFloat );
			else if( _type == key_value_pair_t_type_double )
				return typeofptr( ::rux::XDouble );
			else if( _type == key_value_pair_t_type_int32 )
				return typeofptr( ::rux::XInt32 );
			else if( _type == key_value_pair_t_type_uint32 )
				return typeofptr( ::rux::XUInt32 );
			else if( _type == key_value_pair_t_type_uint64 )
				return typeofptr( ::rux::XUInt64 );
			else if( _type == key_value_pair_t_type_int64 )
				return typeofptr( ::rux::XInt64 );
			else if( _type == key_value_pair_t_type_boolean )
				return typeofptr( ::rux::XBoolean );
			else
				return typeofptr( ::rux::XNull );
		};
		Object& key_value_pair_t::convert_to_object( XGCRef* value )
		{
			return XObject( value , 1 , "obj" , __FILE__ , __LINE__ )++;
		};
		Object& key_value_pair_t::convert_to_object( float value )
		{
			::rux::XFloat obj( value ); 
			return XObject( obj , "obj" , __FILE__ , __LINE__ )++;
		};
		Object& key_value_pair_t::convert_to_object( double value )
		{
			::rux::XDouble obj( value );
			return XObject( obj , "obj" , __FILE__ , __LINE__ )++;
		};
		Object& key_value_pair_t::convert_to_object( ::rux::int32 value )
		{
			::rux::XInt32 obj( value ); 
			return XObject( obj , "obj" , __FILE__ , __LINE__ )++;
		};
		Object& key_value_pair_t::convert_to_object( ::rux::uint32 value )
		{
			::rux::XUInt32 obj( value ); 
			return XObject( obj , "obj" , __FILE__ , __LINE__ )++;
		};
		Object& key_value_pair_t::convert_to_object( ::rux::uint64 value )
		{
			::rux::XUInt64 obj( value ); 
			return XObject( obj , "obj" , __FILE__ , __LINE__ )++;
		};
		Object& key_value_pair_t::convert_to_object( ::rux::int64 value )
		{
			::rux::XInt64 obj( value ); 
			return XObject( obj , "obj" , __FILE__ , __LINE__ )++;
		};
		Object& key_value_pair_t::convert_to_object( ::rux::byte value )
		{
			::rux::XBoolean obj( value );
			return XObject( obj , "obj" , __FILE__ , __LINE__ )++;
		};
		Object& key_value_pair_t::convert_to_object( void )
		{
			return XObject( ::rux::XNull() , "obj" , __FILE__ , __LINE__ )++;
		};
		Object& key_value_pair_t::convert_to_object( char* value , size_t length )
		{
			if( length != SIZE_MAX )
			{
				::rux::XString string( value , 0 , length , XEnumCodePage_UTF8 );
				return XObject( string , "obj" , __FILE__ , __LINE__ )++;
			}
			else
			{
				::rux::XString string( value , XEnumCodePage_UTF8 , 1 , "string" , __FILE__ , __LINE__ );
				return XObject( string , "obj" , __FILE__ , __LINE__ )++;
			}
		};
		Object& key_value_pair_t::convert_to_object00( void* value , key_value_pair_t_type type , size_t length )
		{
			if( type == key_value_pair_t_type_small_utf8_string || type == key_value_pair_t_type_large_utf8_string )
			{
				char* str = (char*)value;
				if( length != SIZE_MAX )
				{
					::rux::XString string( str , 0 , length , XEnumCodePage_UTF8 );
					return XObject( string , "obj" , __FILE__ , __LINE__ )++;
				}
				else
				{
					::rux::XString string( str , XEnumCodePage_UTF8 , 1 , "string" , __FILE__ , __LINE__ );
					return XObject( string , "obj" , __FILE__ , __LINE__ )++;
				}
			}
			else if( type == key_value_pair_t_type_gcref )
				return XObject( (XGCRef*)value , 1 , "obj" , __FILE__ , __LINE__ )++;
			else if( type == key_value_pair_t_type_float )
			{
				float val = *(float*)value;
				::rux::XFloat obj( val ); 
				return XObject( obj , "obj" , __FILE__ , __LINE__ )++;
			}
			else if( type == key_value_pair_t_type_double )
			{
				double val = *(double*)value;
				::rux::XDouble obj( val ); 
				return XObject( obj , "obj" , __FILE__ , __LINE__ )++;
			}
			else if( type == key_value_pair_t_type_int32 )
			{
				::rux::int32 val = *(rux::int32*)value;
				::rux::XInt32 obj( val ); 
				return XObject( obj , "obj" , __FILE__ , __LINE__ )++;
			}
			else if( type == key_value_pair_t_type_uint32 )
			{
				::rux::uint32 val = *(::rux::uint32*)value;
				::rux::XUInt32 obj( val ); 
				return XObject( obj , "obj" , __FILE__ , __LINE__ )++;
			}
			else if( type == key_value_pair_t_type_uint64 )
			{
				::rux::uint64 val = *(::rux::uint64*)value;
				::rux::XUInt64 obj( val ); 
				return XObject( obj , "obj" , __FILE__ , __LINE__ )++;
			}
			else if( type == key_value_pair_t_type_int64 )
			{
				::rux::int64 val = *(::rux::int64*)value;
				::rux::XInt64 obj( val ); 
				return XObject( obj , "obj" , __FILE__ , __LINE__ )++;
			}
			else if( type == key_value_pair_t_type_boolean )
			{
				::rux::byte val = *(::rux::byte*)value;
				::rux::XBoolean obj( val ); 
				return XObject( obj , "obj" , __FILE__ , __LINE__ )++;
			}
			else
				return XObject( ::rux::XNull() , "obj" , __FILE__ , __LINE__ )++;
		};
		Object& key_value_pair_t::get( void )
		{
			if( _type == key_value_pair_t_type_small_utf8_string )
				return ::rux::data::key_value_pair_t::convert_to_object( _small_utf8_string , SIZE_MAX );
			else if( _type == key_value_pair_t_type_large_utf8_string )
				return ::rux::data::key_value_pair_t::convert_to_object( _large_utf8_string , SIZE_MAX );
			else if( _type == key_value_pair_t_type_gcref )
				return ::rux::data::key_value_pair_t::convert_to_object( _gcref );
			else if( _type == key_value_pair_t_type_float )
				return ::rux::data::key_value_pair_t::convert_to_object( _float );
			else if( _type == key_value_pair_t_type_double )
				return ::rux::data::key_value_pair_t::convert_to_object( _double );
			else if( _type == key_value_pair_t_type_int32 )
				return ::rux::data::key_value_pair_t::convert_to_object( _int32_val );
			else if( _type == key_value_pair_t_type_uint32 )
				return ::rux::data::key_value_pair_t::convert_to_object( _uint32 );
			else if( _type == key_value_pair_t_type_uint64 )
				return ::rux::data::key_value_pair_t::convert_to_object( _uint64 );
			else if( _type == key_value_pair_t_type_int64 )
				return ::rux::data::key_value_pair_t::convert_to_object( _int64_val );
			else if( _type == key_value_pair_t_type_boolean )
				return ::rux::data::key_value_pair_t::convert_to_object( _boolean );
			else if( _type == key_value_pair_t_type_null )
				return ::rux::data::key_value_pair_t::convert_to_object();
			else
				return ::rux::data::key_value_pair_t::convert_to_object();
		};
		void key_value_pair_t::set00( void* value , key_value_pair_t_type type , ::rux::byte gcref_addref , size_t length , size_t ___rux__thread_index1986 )
		{
			CODE_LABEL_START( 0 , 0 , 5 );
			clear();
			_type = type;
			if( _type == key_value_pair_t_type_small_utf8_string || _type == key_value_pair_t_type_large_utf8_string )
			{
				char* val = (char*)value;
				size_t len = length;
				if( len == SIZE_MAX )
					len = val ? strlen( val ) : 0;
				if( len >= sizeof( _small_utf8_string ) )
				{
					char* new_str = alloc_array_macros( char , len + 1 );
					memcpy( new_str , val , len + 1 );
					_large_utf8_string = new_str;
					_type = key_value_pair_t_type_large_utf8_string;
				}
				else
				{
					if( len > 0 )
						memcpy( _small_utf8_string , val , len + 1 );
					else
						_small_utf8_string[ 0 ] = 0;
					_type = key_value_pair_t_type_small_utf8_string;
				}
			}
			else if( _type == key_value_pair_t_type_gcref )
			{
				XGCRef* obj = (XGCRef*)value;
				XGCRef* gcref = obj;
				if( obj->IsObjectType() )
				{
					gcref = ((Object*)obj->get_ObjectAddress())->_gc_ref_obj;
					if( gcref == NULL )
						gcref = obj;
				}
				::rux::byte next = 0;
				if( rux_is_object_ptr( gcref , ::rux::XInt64 ) )
				{
					_type = ::rux::data::key_value_pair_t_type_null;
					set00( &((::rux::Int64*)gcref )->_value , ::rux::data::key_value_pair_t_type_int64 , gcref_addref , SIZE_MAX , ___rux__thread_index1986 );
				}
				else if( rux_is_object_ptr( gcref , ::rux::XUInt64 ) )
				{
					_type = ::rux::data::key_value_pair_t_type_null;
					set00( &((::rux::UInt64*)gcref )->_value , ::rux::data::key_value_pair_t_type_uint64 , gcref_addref , SIZE_MAX , ___rux__thread_index1986 );
				}
				else if( rux_is_object_ptr( gcref , ::rux::XInt32 ) )
				{
					_type = ::rux::data::key_value_pair_t_type_null;
					set00( &((::rux::Int32*)gcref )->_value , ::rux::data::key_value_pair_t_type_int32 , gcref_addref , SIZE_MAX , ___rux__thread_index1986 );
				}
				else if( rux_is_object_ptr( gcref , ::rux::XUInt32 ) )
				{
					_type = ::rux::data::key_value_pair_t_type_null;
					set00( &((::rux::UInt32*)gcref )->_value , ::rux::data::key_value_pair_t_type_uint32 , gcref_addref , SIZE_MAX , ___rux__thread_index1986 );
				}
				else if( rux_is_object_ptr( gcref , ::rux::XDouble ) )
				{
					_type = ::rux::data::key_value_pair_t_type_null;
					set00( &((::rux::Double*)gcref )->_value , ::rux::data::key_value_pair_t_type_double , gcref_addref , SIZE_MAX , ___rux__thread_index1986 );
				}
				else if( rux_is_object_ptr( gcref , ::rux::XFloat ) )
				{
					_type = ::rux::data::key_value_pair_t_type_null;
					set00( &((::rux::Float*)gcref )->_value , ::rux::data::key_value_pair_t_type_float , gcref_addref , SIZE_MAX , ___rux__thread_index1986 );
				}
				else if( rux_is_object_ptr( gcref , ::rux::XBoolean ) )
				{
					_type = ::rux::data::key_value_pair_t_type_null;
					set00( &((::rux::Boolean*)gcref )->_value , ::rux::data::key_value_pair_t_type_boolean , gcref_addref , SIZE_MAX , ___rux__thread_index1986 );
				}
				else if( rux_is_object_ptr( gcref , ::rux::XNull ) )
					_type = ::rux::data::key_value_pair_t_type_null;
				else if( rux_is_object_ptr( gcref , ::rux::XString ) )
				{
					_type = ::rux::data::key_value_pair_t_type_null;
					::rux::String* string = ((::rux::String*)gcref->get_ObjectAddress());
					if( string->_code_page != XEnumCodePage_UTF8 )
					{
						declare_variable( ::rux::XString , utf8 );
						utf8.set_ByRef( string->ConvertToUTF8() );
						char* str = utf8.str();
						set00( str , ::rux::data::key_value_pair_t_type_small_utf8_string , 1 , utf8.Size() - 1 , ___rux__thread_index1986 );
					}
					else
					{
						char* str = string->str();
						set00( str , ::rux::data::key_value_pair_t_type_small_utf8_string , 1 , string->Size() - 1 , ___rux__thread_index1986 );
					}
				}
				else if( rux_is_object_ptr( gcref , ::rux::XNumeric ) )
				{
					_type = ::rux::data::key_value_pair_t_type_null;
					::rux::Numeric* value00 = (::rux::Numeric*)gcref->get_ObjectAddress();
					if( value00->_value_gcref )
						set00( value00->_value_gcref , ::rux::data::key_value_pair_t_type_gcref , gcref_addref , SIZE_MAX , ___rux__thread_index1986 );
				}
				else
					next = 1;
				if( next )
				{
					if( gcref_addref )
						gcref->AddRef( __FILE__ , __LINE__ );
					_gcref = gcref;
				}
			}
			else if( _type == key_value_pair_t_type_float )
				_float = *(float*)value;
			else if( _type == key_value_pair_t_type_double )
				_double = *(double*)value;
			else if( _type == key_value_pair_t_type_int32 )
				_int32_val = *(::rux::int32*)value;
			else if( _type == key_value_pair_t_type_uint32 )
				_uint32 = *(::rux::uint32*)value;
			else if( _type == key_value_pair_t_type_uint64 )
				_uint64 = *(::rux::uint64*)value;
			else if( _type == key_value_pair_t_type_int64 )
				_int64_val = *(::rux::int64*)value;
			else if( _type == key_value_pair_t_type_boolean )
				_boolean = *(::rux::byte*)value;
			CODE_LABEL_END();
		};
		DataObject::~DataObject()
		{
			::rux::data::DoubleLinkedListItem< ::rux::data::key_value_pair_t >* item = _pairs.Begin();
			while( item )
			{
				::rux::data::DoubleLinkedListItem< ::rux::data::key_value_pair_t >* delete_item = item;
				item = item->Next();
				::rux::engine::free_object< ::rux::data::DoubleLinkedListItem< ::rux::data::key_value_pair_t > >( delete_item );
			}
		};
		implement_duplicate_internal_result_function_1( Object& , DataObject , operator[] , const ::rux::XString& );
		implement_duplicate_internal_result_function_1( Object& , DataObject , operator[] , const char* );
		Object& DataObject::operator[] ( const ::rux::XString& field_name )
		{
			return operator[]( field_name.ConvertToUTF8().str() );
		};
		Object& XDataObject::FromStruct( XObject& object )
		{
			if( rux_is_object( object , rux::data::XDataObject ) 
				|| rux_is_object( object , rux::XInt64 )
				|| rux_is_object( object , rux::XUInt64 )
				|| rux_is_object( object , rux::XInt32 )
				|| rux_is_object( object , rux::XUInt32 )
				|| rux_is_object( object , rux::XDouble )
				|| rux_is_object( object , rux::XFloat )
				|| rux_is_object( object , rux::XBoolean )
				|| rux_is_object( object , rux::XNull )
				|| rux_is_object( object , rux::XString ) 
				|| rux_is_object( object , rux::XNumeric ) )
				return object++;
			else
			{
				XType type( object.get_Type() );
				if( type.get_IsArray() == 1 )
				{
					::rux::XArray< XObject > array_value;
					::rux::PropertyInfo* property_info = type.get_PropertyInfo( ::rux::compile::times33_hash< 'C','o','u','n','t' >::value );
					if( property_info )
					{
						::rux::FunctionInfo* function_info = type.get_FunctionInfo( ::rux::compile::times33_hash< 'g','e','t','_','I','t','e','m' >::value );
						if( function_info )
						{
							::rux::XUInt64 count( property_info->Get( object ) );
							for( size_t index0 = 0 ; index0 < (size_t)count() ; index0++ )
							{
								XObject item( function_info->InvokeResult( object , XObject( ::rux::XUInt64( index0 ) ) ) , "obj" , __FILE__ , __LINE__ );
								array_value.Add( XDataObject::FromStruct( item ) );
							}
						}
					}
					return XObject( array_value , "obj" , __FILE__ , __LINE__ )++;
				}
				else
				{
					declare_variable( ::rux::data::XDataObject , data_object_value );
					XMallocArray< ::rux::PropertyInfo* >* properties = type.get_Properties();
					if( properties )
					{
						for( size_t index0 = 0 ; index0 < properties->Count() ; index0++ )
						{
							XObject property_obj( properties->operator[]( index0 )->Get( object ) , "obj" , __FILE__ , __LINE__ );
							data_object_value.SetValue( properties->operator[]( index0 )->get_Name() , XDataObject::FromStruct( property_obj ) , 0 );
						}
					}
					return XObject( data_object_value , "obj" , __FILE__ , __LINE__ )++;
				}
			}
		};
		void XDataObject::ToStruct( XObject& data_obj , XObject& struct_obj )
		{
			Type* type = &struct_obj.get_Type();
			if( rux_same_types_of_objects( data_obj , struct_obj ) || rux_typeptr_is_class( type , XObject ) )
				struct_obj.set_ByRef( data_obj );
			else if( type->get_IsArray() == 1 )
			{
				if( rux_is_object( data_obj , ::rux::XArray< XObject > ) )
				{
					::rux::FunctionInfo* function_info = type->get_FunctionInfo( ::rux::compile::times33_hash< 'A','d','d' >::value );
					if( function_info )
					{
						::rux::XArray< XObject > array_obj;
						array_obj.set_ByRef( data_obj );
						XType item_type( type->get_ArrayItemType() );					
						::rux::ConstructorInfo* constructor_info = item_type.get_ConstructorInfo();
						if( constructor_info )						
						{
							for( size_t index0 = 0 ; index0 < array_obj.Count() ; index0++ )
							{
								XObject item( constructor_info->Create() , "obj" , __FILE__ , __LINE__ ) , item_data_obj( array_obj[ index0 ] );
								XDataObject::ToStruct( item_data_obj , item );
								function_info->Invoke( struct_obj , item );
							}
						}
					}
				}
			}
			else if( rux_is_object( data_obj , ::rux::data::XDataObject ) )
			{
				declare_variable_param( ::rux::data::XDataObject , data_obj0 , data_obj );
				for( size_t index0 = 0 ; index0 < data_obj0.FieldsCount() ; index0++ )
				{
					XObject value( data_obj0.get_FieldValue( index0 ) , "obj" , __FILE__ , __LINE__ );
					::rux::PropertyInfo* property_info = type->get_PropertyInfo( data_obj0.get_FieldNameHash( index0 ) );
					if( property_info )
					{
						XObject property_obj( property_info->Get( struct_obj ) , "obj" , __FILE__ , __LINE__ );
						XDataObject::ToStruct( value , property_obj );
						property_info->Set( struct_obj , property_obj );
					}
				}
			}
		};
		implement_duplicate_internal_function_1( DataObject , ToStruct , XObject& );
		void DataObject::ToStruct( XObject& obj )
		{
			XObject data_obj( *this , "obj" , __FILE__ , __LINE__ );
			XDataObject::ToStruct( data_obj , obj );
		};		
		Object& DataObject::operator[] ( const char* field_name )
		{
			uint8 found = 0;
			return GetValue< XObject >( field_name , found , SIZE_MAX , "->" );
		};
		implement_duplicate_internal_function_3( DataObject , SetValue , const char* , const XObject& , const char* );
		implement_duplicate_internal_function_4( DataObject , AddValue , const char* , const XObject& , const char* , size_t );
		implement_duplicate_internal_function_2( DataObject , SetValue , size_t , const XObject& );
		implement_duplicate_internal_function_3( DataObject , AddValueToArray , const char* , const XObject& , const char* );
		implement_duplicate_internal_function_5( DataObject , SetValue , const char* , const char* , ::rux::byte , size_t , const char* );
		void DataObject::SetValue( const char* field_name , const char* object , ::rux::byte utf8 , size_t length , const char* field_names_delimiter )
		{
			//CODE_LABELS_INITIALIZE();
			//CODE_LABEL( 0 , 0 , 5 );
			if( utf8 )
				SetValue00( field_name , (void*)object , ::rux::data::key_value_pair_t_type_small_utf8_string , length , field_names_delimiter );
			else
				SetValue( field_name , ::rux::XString( object ).ConvertToUTF8().str() , 1 , SIZE_MAX , field_names_delimiter );
		};
		implement_duplicate_internal_result_function_4( ::rux::uint32 , DataObject , GetStringValueHash , const ::rux::XString& , uint8& , size_t , const char* );
		::rux::uint32 DataObject::GetStringValueHash( const ::rux::XString& field_name , uint8& found , size_t field_name_length , const char* field_names_delimiter )
		{
			declare_variable( ::rux::XString , utf8_field_name );
			utf8_field_name.set_ByRef( field_name.ConvertToUTF8() );
			return GetStringValueHash( utf8_field_name.str() , found , field_name_length , field_names_delimiter );
		};
		implement_duplicate_internal_result_function_4( ::rux::uint32 , DataObject , GetStringValueHash , const char* , uint8& , size_t , const char* );
		::rux::uint32 DataObject::GetStringValueHash( const char* field_name , uint8& found , size_t field_name_length , const char* field_names_delimiter )
		{
			declare_variable( ::rux::XString , value_string );
			value_string.set_ByRef( GetValue< ::rux::XString >( field_name , found , field_name_length , field_names_delimiter ) );
			if( found )
				return ::rux::engine::_globals->add_times33_hash( value_string.str() , value_string.Size() - 1 );
			else
				return 0;
		};
		implement_duplicate_internal_result_function_2( ::rux::uint32 , DataObject , GetStringValueHash , ::rux::uint32 , uint8& );
		::rux::uint32 DataObject::GetStringValueHash( ::rux::uint32 field_hash , uint8& found )
		{
			declare_variable( ::rux::XString , value_string );
			value_string.set_ByRef( GetValue< ::rux::XString >( field_hash , found ) );
			if( found )
				return ::rux::engine::_globals->add_times33_hash( value_string.str() , value_string.Size() - 1 );
			else
				return 0;
		};
		implement_duplicate_internal_function_6( DataObject , AddValue , const char* , const char* , ::rux::byte , size_t , const char* , size_t );
		void DataObject::AddValue( const char* field_name , const char* object , ::rux::byte utf8 , size_t length , const char* field_names_delimiter , size_t ___rux__thread_index1986 )
		{
			CODE_LABEL_START( 0 , 0 , 5 );
			if( utf8 )
				AddValue00( field_name , (void*)object , ::rux::data::key_value_pair_t_type_small_utf8_string , length , field_names_delimiter );
			else
				AddValue( field_name , ::rux::XString( object ).ConvertToUTF8().str() , 1 , SIZE_MAX , field_names_delimiter );
			CODE_LABEL_END();
		};
		implement_duplicate_internal_function_4( DataObject , SetValue , size_t , const char* , ::rux::byte , size_t );
		void DataObject::SetValue( size_t field_index , const char* object , ::rux::byte utf8 , size_t length )
		{
			//CODE_LABELS_INITIALIZE();
			//CODE_LABEL( 0 , 0 , 5 );
			if( utf8 )
				SetValue00( field_index , (void*)object , ::rux::data::key_value_pair_t_type_small_utf8_string , length );
			else
				SetValue( field_index , ::rux::XString( object ).ConvertToUTF8().str() , 1 );
		};
		implement_duplicate_internal_function_5( DataObject , AddValueToArray , const char* , const char* , ::rux::byte , size_t , const char* );
		void DataObject::AddValueToArray( const char* field_name , const char* object , ::rux::byte utf8 , size_t length , const char* field_names_delimiter )
		{
			//CODE_LABELS_INITIALIZE();
			//CODE_LABEL( 0 , 0 , 5 );
			if( utf8 )
				AddValueToArray00( field_name , (void*)object , ::rux::data::key_value_pair_t_type_small_utf8_string , length , field_names_delimiter );
			else
				AddValueToArray( field_name , ::rux::XString( object ).ConvertToUTF8().str() , 1 , SIZE_MAX , field_names_delimiter );
		};
		implement_duplicate_internal_function_5( DataObject , SetValue , const ::rux::XString& , const char* , ::rux::byte , size_t , const char* );
		void DataObject::SetValue( const ::rux::XString& field_name , const char* object , ::rux::byte utf8 , size_t length , const char* field_names_delimiter )
		{
			//CODE_LABELS_INITIALIZE();
			//CODE_LABEL( 0 , 0 , 5 );
			if( utf8 )
				SetValue00( field_name.ConvertToUTF8().str() , (void*)object , ::rux::data::key_value_pair_t_type_small_utf8_string , length , field_names_delimiter );
			else
				SetValue( field_name.ConvertToUTF8().str() , ::rux::XString( object ).ConvertToUTF8().str() , 1 , SIZE_MAX , field_names_delimiter );
		};
		implement_duplicate_internal_function_6( DataObject , AddValue , const ::rux::XString& , const char* , ::rux::byte , size_t , const char* , size_t );
		void DataObject::AddValue( const ::rux::XString& field_name , const char* object , ::rux::byte utf8 , size_t length , const char* field_names_delimiter , size_t ___rux__thread_index1986 )
		{
			CODE_LABEL_START( 0 , 0 , 5 );
			if( utf8 )
				AddValue00( field_name.ConvertToUTF8().str() , (void*)object , ::rux::data::key_value_pair_t_type_small_utf8_string , length , field_names_delimiter );
			else
				AddValue( field_name.ConvertToUTF8().str() , ::rux::XString( object ).ConvertToUTF8().str() , 1 , SIZE_MAX , field_names_delimiter );
			CODE_LABEL_END();
		};
		implement_duplicate_internal_function_5( DataObject , AddValueToArray , const ::rux::XString& , const char* , ::rux::byte , size_t , const char* );
		void DataObject::AddValueToArray( const ::rux::XString& field_name , const char* object , ::rux::byte utf8 , size_t length , const char* field_names_delimiter )
		{
			//CODE_LABELS_INITIALIZE();
			//CODE_LABEL( 0 , 0 , 5 );
			if( utf8 )
				AddValueToArray00( field_name.ConvertToUTF8().str() , (void*)object , ::rux::data::key_value_pair_t_type_small_utf8_string , length , field_names_delimiter );
			else
				AddValueToArray( field_name.ConvertToUTF8().str() , ::rux::XString( object ).ConvertToUTF8().str() , 1 , SIZE_MAX , field_names_delimiter );
		};
		implement_duplicate_internal_function_3( DataObject , SetValue , const char* , bool , const char* );
		void DataObject::SetValue( const char* field_name , bool object , const char* field_names_delimiter )
		{
			//CODE_LABELS_INITIALIZE();
			//CODE_LABEL( 0 , 0 , 5 );
			::rux::byte val = object ? 1 : 0;
			SetValue00( field_name , &val , ::rux::data::key_value_pair_t_type_boolean , SIZE_MAX , field_names_delimiter );
		};
		implement_duplicate_internal_function_4( DataObject , AddValue , const char* , bool , const char* , size_t );
		void DataObject::AddValue( const char* field_name , bool object , const char* field_names_delimiter , size_t ___rux__thread_index1986 )
		{
			CODE_LABEL_START( 0 , 0 , 5 );
			::rux::byte val = object ? 1 : 0;
			AddValue00( field_name , &val , ::rux::data::key_value_pair_t_type_boolean , SIZE_MAX , field_names_delimiter );
			CODE_LABEL_END();
		};
		implement_duplicate_internal_function_2( DataObject , SetValue , size_t , bool );
		void DataObject::SetValue( size_t field_index , bool object )
		{
			//CODE_LABELS_INITIALIZE();
			//CODE_LABEL( 0 , 0 , 5 );
			::rux::byte val = object ? 1 : 0;
			SetValue00( field_index , &val , ::rux::data::key_value_pair_t_type_boolean , SIZE_MAX );
		};
		implement_duplicate_internal_function_3( DataObject , AddValueToArray , const char* , bool , const char* );
		void DataObject::AddValueToArray( const char* field_name , bool object , const char* field_names_delimiter )
		{
			//CODE_LABELS_INITIALIZE();
			//CODE_LABEL( 0 , 0 , 5 );
			::rux::byte val = object ? 1 : 0;
			AddValueToArray00( field_name , &val , ::rux::data::key_value_pair_t_type_boolean , SIZE_MAX , field_names_delimiter );
		};
		implement_duplicate_internal_function_3( DataObject , SetValue , const ::rux::XString& , bool , const char* );
		void DataObject::SetValue( const ::rux::XString& field_name , bool object , const char* field_names_delimiter )
		{
			//CODE_LABELS_INITIALIZE();
			//CODE_LABEL( 0 , 0 , 5 );
			SetValue( field_name.ConvertToUTF8().str() , object , field_names_delimiter );
		};
		implement_duplicate_internal_function_4( DataObject , AddValue , const ::rux::XString& , bool , const char* , size_t );
		void DataObject::AddValue( const ::rux::XString& field_name , bool object , const char* field_names_delimiter , size_t ___rux__thread_index1986 )
		{
			CODE_LABEL_START( 0 , 0 , 5 );
			AddValue( field_name.ConvertToUTF8().str() , object , field_names_delimiter );
			CODE_LABEL_END();
		};
		implement_duplicate_internal_function_3( DataObject , AddValueToArray , const ::rux::XString& , bool , const char* );
		void DataObject::AddValueToArray( const ::rux::XString& field_name , bool object , const char* field_names_delimiter )
		{
			//CODE_LABELS_INITIALIZE();
			//CODE_LABEL( 0 , 0 , 5 );
			AddValueToArray( field_name.ConvertToUTF8().str() , object , field_names_delimiter );
		};
		implement_duplicate_internal_function_3( DataObject , SetValue , const char* , uint8 , const char* );
		void DataObject::SetValue( const char* field_name , uint8 object , const char* field_names_delimiter )
		{
			//CODE_LABELS_INITIALIZE();
			//CODE_LABEL( 0 , 0 , 5 );
			SetValue00( field_name , (void*)&object , ::rux::data::key_value_pair_t_type_boolean , SIZE_MAX , field_names_delimiter );
		};
		implement_duplicate_internal_function_4( DataObject , AddValue , const char* , uint8 , const char* , size_t );
		void DataObject::AddValue( const char* field_name , uint8 object , const char* field_names_delimiter , size_t ___rux__thread_index1986 )
		{
			UNREFERENCED_PARAMETER( ___rux__thread_index1986 );
			AddValue00( field_name , (void*)&object , ::rux::data::key_value_pair_t_type_boolean , SIZE_MAX , field_names_delimiter );
		};
		implement_duplicate_internal_function_2( DataObject , SetValue , size_t , uint8 );
		void DataObject::SetValue( size_t field_index , uint8 object )
		{
			//CODE_LABELS_INITIALIZE();
			//CODE_LABEL( 0 , 0 , 5 );
			SetValue00( field_index , (void*)&object , ::rux::data::key_value_pair_t_type_boolean , SIZE_MAX );
		};
		implement_duplicate_internal_function_3( DataObject , AddValueToArray , const char* , uint8 , const char* );
		void DataObject::AddValueToArray( const char* field_name , uint8 object , const char* field_names_delimiter )
		{
			//CODE_LABELS_INITIALIZE();
			//CODE_LABEL( 0 , 0 , 5 );
			AddValueToArray00( field_name , (void*)&object , ::rux::data::key_value_pair_t_type_boolean , SIZE_MAX , field_names_delimiter );
		};
		implement_duplicate_internal_function_3( DataObject , SetValue , const ::rux::XString& , uint8 , const char* );
		void DataObject::SetValue( const ::rux::XString& field_name , uint8 object , const char* field_names_delimiter )
		{
			//CODE_LABELS_INITIALIZE();
			//CODE_LABEL( 0 , 0 , 5 );
			SetValue00( field_name.ConvertToUTF8().str() , (void*)&object , ::rux::data::key_value_pair_t_type_boolean , SIZE_MAX , field_names_delimiter );
		};
		implement_duplicate_internal_function_4( DataObject , AddValue , const ::rux::XString& , uint8 , const char* , size_t );
		void DataObject::AddValue( const ::rux::XString& field_name , uint8 object , const char* field_names_delimiter , size_t ___rux__thread_index1986 )
		{
			CODE_LABEL_START( 0 , 0 , 5 );
			AddValue00( field_name.ConvertToUTF8().str() , (void*)&object , ::rux::data::key_value_pair_t_type_boolean , SIZE_MAX , field_names_delimiter );
			CODE_LABEL_END();
		};
		implement_duplicate_internal_function_3( DataObject , AddValueToArray , const ::rux::XString& , uint8 , const char* );
		void DataObject::AddValueToArray( const ::rux::XString& field_name , uint8 object , const char* field_names_delimiter )
		{
			//CODE_LABELS_INITIALIZE();
			//CODE_LABEL( 0 , 0 , 5 );
			AddValueToArray00( field_name.ConvertToUTF8().str() , (void*)&object , ::rux::data::key_value_pair_t_type_boolean , SIZE_MAX , field_names_delimiter );
		};
		implement_duplicate_internal_function_3( DataObject , SetValue , const char* , uint64 , const char* );
		void DataObject::SetValue( const char* field_name , uint64 object , const char* field_names_delimiter )
		{
			//CODE_LABELS_INITIALIZE();
			//CODE_LABEL( 0 , 0 , 5 );
			SetValue00( field_name , (void*)&object , ::rux::data::key_value_pair_t_type_uint64 , SIZE_MAX , field_names_delimiter );
		};
		implement_duplicate_internal_function_4( DataObject , AddValue , const char* , uint64 , const char* , size_t );
		void DataObject::AddValue( const char* field_name , uint64 object , const char* field_names_delimiter , size_t ___rux__thread_index1986 )
		{
			UNREFERENCED_PARAMETER( ___rux__thread_index1986 );
			AddValue00( field_name , (void*)&object , ::rux::data::key_value_pair_t_type_uint64 , SIZE_MAX , field_names_delimiter );
		};
		implement_duplicate_internal_function_2( DataObject , SetValue , size_t , uint64 );
		void DataObject::SetValue( size_t field_index , uint64 object )
		{
			//CODE_LABELS_INITIALIZE();
			//CODE_LABEL( 0 , 0 , 5 );
			SetValue00( field_index , (void*)&object , ::rux::data::key_value_pair_t_type_uint64 , SIZE_MAX );
		};
		implement_duplicate_internal_function_3( DataObject , AddValueToArray , const char* , uint64 , const char* );
		void DataObject::AddValueToArray( const char* field_name , uint64 object , const char* field_names_delimiter )
		{
			//CODE_LABELS_INITIALIZE();
			//CODE_LABEL( 0 , 0 , 5 );
			AddValueToArray00( field_name , (void*)&object , ::rux::data::key_value_pair_t_type_uint64 , SIZE_MAX , field_names_delimiter );
		};
		implement_duplicate_internal_function_3( DataObject , SetValue , const ::rux::XString& , uint64 , const char* );
		void DataObject::SetValue( const ::rux::XString& field_name , uint64 object , const char* field_names_delimiter )
		{
			//CODE_LABELS_INITIALIZE();
			//CODE_LABEL( 0 , 0 , 5 );
			SetValue00( field_name.ConvertToUTF8().str() , (void*)&object , ::rux::data::key_value_pair_t_type_uint64 , SIZE_MAX , field_names_delimiter );
		};
		implement_duplicate_internal_function_4( DataObject , AddValue , const ::rux::XString& , uint64 , const char* , size_t );
		void DataObject::AddValue( const ::rux::XString& field_name , uint64 object , const char* field_names_delimiter , size_t ___rux__thread_index1986 )
		{
			CODE_LABEL_START( 0 , 0 , 5 );
			AddValue00( field_name.ConvertToUTF8().str() , (void*)&object , ::rux::data::key_value_pair_t_type_uint64 , SIZE_MAX , field_names_delimiter );
			CODE_LABEL_END();
		};
		implement_duplicate_internal_function_3( DataObject , AddValueToArray , const ::rux::XString& , uint64 , const char* );
		void DataObject::AddValueToArray( const ::rux::XString& field_name , uint64 object , const char* field_names_delimiter )
		{
			//CODE_LABELS_INITIALIZE();
			//CODE_LABEL( 0 , 0 , 5 );
			AddValueToArray00( field_name.ConvertToUTF8().str() , (void*)&object , ::rux::data::key_value_pair_t_type_uint64 , SIZE_MAX , field_names_delimiter );
		};
		implement_duplicate_internal_function_3( DataObject , SetValue , const char* , uint32 , const char* );
		void DataObject::SetValue( const char* field_name , uint32 object , const char* field_names_delimiter , size_t ___rux__thread_index1986 )
		{
			UNREFERENCED_PARAMETER( ___rux__thread_index1986 );
			SetValue00( field_name , (void*)&object , ::rux::data::key_value_pair_t_type_uint32 , SIZE_MAX , field_names_delimiter );
		};
		implement_duplicate_internal_function_4( DataObject , AddValue , const char* , uint32 , const char* , size_t );
		void DataObject::AddValue( const char* field_name , uint32 object , const char* field_names_delimiter , size_t ___rux__thread_index1986 )
		{
			UNREFERENCED_PARAMETER( ___rux__thread_index1986 );
			AddValue00( field_name , (void*)&object , ::rux::data::key_value_pair_t_type_uint32 , SIZE_MAX , field_names_delimiter );
		};
		implement_duplicate_internal_function_2( DataObject , SetValue , size_t , uint32 );
		void DataObject::SetValue( size_t field_index , uint32 object )
		{
			//CODE_LABELS_INITIALIZE();
			//CODE_LABEL( 0 , 0 , 5 );
			SetValue00( field_index , (void*)&object , ::rux::data::key_value_pair_t_type_uint32 , SIZE_MAX );
		};
		implement_duplicate_internal_function_3( DataObject , AddValueToArray , const char* , uint32 , const char* );
		void DataObject::AddValueToArray( const char* field_name , uint32 object , const char* field_names_delimiter )
		{
			//CODE_LABELS_INITIALIZE();
			//CODE_LABEL( 0 , 0 , 5 );
			AddValueToArray00( field_name , (void*)&object , ::rux::data::key_value_pair_t_type_uint32 , SIZE_MAX , field_names_delimiter );
		};
		implement_duplicate_internal_function_3( DataObject , SetValue , const ::rux::XString& , uint32 , const char* );
		void DataObject::SetValue( const ::rux::XString& field_name , uint32 object , const char* field_names_delimiter )
		{
			//CODE_LABELS_INITIALIZE();
			//CODE_LABEL( 0 , 0 , 5 );
			SetValue00( field_name.ConvertToUTF8().str() , (void*)&object , ::rux::data::key_value_pair_t_type_uint32 , SIZE_MAX , field_names_delimiter );
		};
		implement_duplicate_internal_function_4( DataObject , AddValue , const ::rux::XString& , uint32 , const char* , size_t );
		void DataObject::AddValue( const ::rux::XString& field_name , uint32 object , const char* field_names_delimiter , size_t ___rux__thread_index1986 )
		{
			CODE_LABEL_START( 0 , 0 , 5 );
			AddValue00( field_name.ConvertToUTF8().str() , (void*)&object , ::rux::data::key_value_pair_t_type_uint32 , SIZE_MAX , field_names_delimiter );
			CODE_LABEL_END();
		};
		implement_duplicate_internal_function_3( DataObject , AddValueToArray , const ::rux::XString& , uint32 , const char* );
		void DataObject::AddValueToArray( const ::rux::XString& field_name , uint32 object , const char* field_names_delimiter )
		{
			//CODE_LABELS_INITIALIZE();
			//CODE_LABEL( 0 , 0 , 5 );
			AddValueToArray00( field_name.ConvertToUTF8().str() , (void*)&object , ::rux::data::key_value_pair_t_type_uint32 , SIZE_MAX , field_names_delimiter );
		};
		implement_duplicate_internal_function_3( DataObject , SetValue , const char* , int64 , const char* );
		void DataObject::SetValue( const char* field_name , int64 object , const char* field_names_delimiter )
		{
			//CODE_LABELS_INITIALIZE();
			//CODE_LABEL( 0 , 0 , 5 );
			SetValue00( field_name , (void*)&object , ::rux::data::key_value_pair_t_type_int64 , SIZE_MAX , field_names_delimiter );
		};
		implement_duplicate_internal_function_4( DataObject , AddValue , const char* , int64 , const char* , size_t );
		void DataObject::AddValue( const char* field_name , int64 object , const char* field_names_delimiter , size_t ___rux__thread_index1986 )
		{
			UNREFERENCED_PARAMETER( ___rux__thread_index1986 );
			AddValue00( field_name , (void*)&object , ::rux::data::key_value_pair_t_type_int64 , SIZE_MAX , field_names_delimiter );
		};
		implement_duplicate_internal_function_2( DataObject , SetValue , size_t , int64 );
		void DataObject::SetValue( size_t field_index , int64 object )
		{
			//CODE_LABELS_INITIALIZE();
			//CODE_LABEL( 0 , 0 , 5 );
			SetValue00( field_index , (void*)&object , ::rux::data::key_value_pair_t_type_int64 , SIZE_MAX );
		};
		implement_duplicate_internal_function_3( DataObject , AddValueToArray , const char* , int64 , const char* );
		void DataObject::AddValueToArray( const char* field_name , int64 object , const char* field_names_delimiter )
		{
			//CODE_LABELS_INITIALIZE();
			//CODE_LABEL( 0 , 0 , 5 );
			AddValueToArray00( field_name , (void*)&object , ::rux::data::key_value_pair_t_type_int64 , SIZE_MAX , field_names_delimiter );
		};
		implement_duplicate_internal_function_3( DataObject , SetValue , const ::rux::XString& , int64 , const char* );
		void DataObject::SetValue( const ::rux::XString& field_name , int64 object , const char* field_names_delimiter )
		{
			//CODE_LABELS_INITIALIZE();
			//CODE_LABEL( 0 , 0 , 5 );
			SetValue00( field_name.ConvertToUTF8().str() , (void*)&object , ::rux::data::key_value_pair_t_type_int64 , SIZE_MAX , field_names_delimiter );
		};
		implement_duplicate_internal_function_4( DataObject , AddValue , const ::rux::XString& , int64 , const char* , size_t );
		void DataObject::AddValue( const ::rux::XString& field_name , int64 object , const char* field_names_delimiter , size_t ___rux__thread_index1986 )
		{
			CODE_LABEL_START( 0 , 0 , 5 );
			AddValue00( field_name.ConvertToUTF8().str() , (void*)&object , ::rux::data::key_value_pair_t_type_int64 , SIZE_MAX , field_names_delimiter );
			CODE_LABEL_END();
		};
		implement_duplicate_internal_function_3( DataObject , AddValueToArray , const ::rux::XString& , int64 , const char* );
		void DataObject::AddValueToArray( const ::rux::XString& field_name , int64 object , const char* field_names_delimiter )
		{
			//CODE_LABELS_INITIALIZE();
			//CODE_LABEL( 0 , 0 , 5 );
			AddValueToArray00( field_name.ConvertToUTF8().str() , (void*)&object , ::rux::data::key_value_pair_t_type_int64 , SIZE_MAX , field_names_delimiter );
		};
		implement_duplicate_internal_function_3( DataObject , SetValue , const char* , double , const char* );
		void DataObject::SetValue( const char* field_name , double object , const char* field_names_delimiter )
		{
			//CODE_LABELS_INITIALIZE();
			//CODE_LABEL( 0 , 0 , 5 );
			SetValue00( field_name , (void*)&object , ::rux::data::key_value_pair_t_type_double , SIZE_MAX , field_names_delimiter );
		};
		implement_duplicate_internal_function_4( DataObject , AddValue , const char* , double , const char* , size_t );
		void DataObject::AddValue( const char* field_name , double object , const char* field_names_delimiter , size_t ___rux__thread_index1986 )
		{
			UNREFERENCED_PARAMETER( ___rux__thread_index1986 );
			AddValue00( field_name , (void*)&object , ::rux::data::key_value_pair_t_type_double , SIZE_MAX , field_names_delimiter );
		};
		implement_duplicate_internal_function_2( DataObject , SetValue , size_t , double );
		void DataObject::SetValue( size_t field_index , double object )
		{
			//CODE_LABELS_INITIALIZE();
			//CODE_LABEL( 0 , 0 , 5 );
			SetValue00( field_index , (void*)&object , ::rux::data::key_value_pair_t_type_double , SIZE_MAX );
		};
		implement_duplicate_internal_function_3( DataObject , AddValueToArray , const char* , double , const char* );
		void DataObject::AddValueToArray( const char* field_name , double object , const char* field_names_delimiter )
		{
			//CODE_LABELS_INITIALIZE();
			//CODE_LABEL( 0 , 0 , 5 );
			AddValueToArray00( field_name , (void*)&object , ::rux::data::key_value_pair_t_type_double , SIZE_MAX , field_names_delimiter );
		};
		implement_duplicate_internal_function_3( DataObject , SetValue , const ::rux::XString& , double , const char* );
		void DataObject::SetValue( const ::rux::XString& field_name , double object , const char* field_names_delimiter )
		{
			//CODE_LABELS_INITIALIZE();
			//CODE_LABEL( 0 , 0 , 5 );
			SetValue00( field_name.ConvertToUTF8().str() , (void*)&object , ::rux::data::key_value_pair_t_type_double , SIZE_MAX , field_names_delimiter );
		};
		implement_duplicate_internal_function_4( DataObject , AddValue , const ::rux::XString& , double , const char* , size_t );
		void DataObject::AddValue( const ::rux::XString& field_name , double object , const char* field_names_delimiter , size_t ___rux__thread_index1986 )
		{
			CODE_LABEL_START( 0 , 0 , 5 );
			AddValue00( field_name.ConvertToUTF8().str() , (void*)&object , ::rux::data::key_value_pair_t_type_double , SIZE_MAX , field_names_delimiter );
			CODE_LABEL_END();
		};
		implement_duplicate_internal_function_3( DataObject , AddValueToArray , const ::rux::XString& , double , const char* );
		void DataObject::AddValueToArray( const ::rux::XString& field_name , double object , const char* field_names_delimiter )
		{
			//CODE_LABELS_INITIALIZE();
			//CODE_LABEL( 0 , 0 , 5 );
			AddValueToArray00( field_name.ConvertToUTF8().str() , (void*)&object , ::rux::data::key_value_pair_t_type_double , SIZE_MAX , field_names_delimiter );
		};
		implement_duplicate_internal_function_3( DataObject , SetValue , const char* , float , const char* );
		void DataObject::SetValue( const char* field_name , float object , const char* field_names_delimiter )
		{
			//CODE_LABELS_INITIALIZE();
			//CODE_LABEL( 0 , 0 , 5 );
			SetValue00( field_name , (void*)&object , ::rux::data::key_value_pair_t_type_float , SIZE_MAX , field_names_delimiter );
		};
		implement_duplicate_internal_function_4( DataObject , AddValue , const char* , float , const char* , size_t );
		void DataObject::AddValue( const char* field_name , float object , const char* field_names_delimiter , size_t ___rux__thread_index1986 )
		{
			UNREFERENCED_PARAMETER( ___rux__thread_index1986 );
			AddValue00( field_name , (void*)&object , ::rux::data::key_value_pair_t_type_float , SIZE_MAX , field_names_delimiter );
		};
		implement_duplicate_internal_function_2( DataObject , SetValue , size_t , float );
		void DataObject::SetValue( size_t field_index , float object )
		{
			//CODE_LABELS_INITIALIZE();
			//CODE_LABEL( 0 , 0 , 5 );
			SetValue00( field_index , (void*)&object , ::rux::data::key_value_pair_t_type_float , SIZE_MAX );
		};
		implement_duplicate_internal_function_3( DataObject , AddValueToArray , const char* , float , const char* );
		void DataObject::AddValueToArray( const char* field_name , float object , const char* field_names_delimiter )
		{
			//CODE_LABELS_INITIALIZE();
			//CODE_LABEL( 0 , 0 , 5 );
			AddValueToArray00( field_name , (void*)&object , ::rux::data::key_value_pair_t_type_float , SIZE_MAX , field_names_delimiter );
		};
		implement_duplicate_internal_function_3( DataObject , SetValue , const ::rux::XString& , float , const char* );
		void DataObject::SetValue( const ::rux::XString& field_name , float object , const char* field_names_delimiter )
		{
			//CODE_LABELS_INITIALIZE();
			//CODE_LABEL( 0 , 0 , 5 );
			SetValue00( field_name.ConvertToUTF8().str() , (void*)&object , ::rux::data::key_value_pair_t_type_float , SIZE_MAX , field_names_delimiter );
		};
		implement_duplicate_internal_function_4( DataObject , AddValue , const ::rux::XString& , float , const char* , size_t );
		void DataObject::AddValue( const ::rux::XString& field_name , float object , const char* field_names_delimiter , size_t ___rux__thread_index1986 )
		{
			CODE_LABEL_START( 0 , 0 , 5 );
			AddValue00( field_name.ConvertToUTF8().str() , (void*)&object , ::rux::data::key_value_pair_t_type_float , SIZE_MAX , field_names_delimiter );
			CODE_LABEL_END();
		};
		implement_duplicate_internal_function_3( DataObject , AddValueToArray , const ::rux::XString& , float , const char* );
		void DataObject::AddValueToArray( const ::rux::XString& field_name , float object , const char* field_names_delimiter )
		{
			//CODE_LABELS_INITIALIZE();
			//CODE_LABEL( 0 , 0 , 5 );
			AddValueToArray00( field_name.ConvertToUTF8().str() , (void*)&object , ::rux::data::key_value_pair_t_type_float , SIZE_MAX , field_names_delimiter );
		};
		implement_duplicate_internal_function_3( DataObject , SetValue , const char* , XGCRef& , const char* );
		implement_duplicate_internal_function_4( DataObject , AddValue , const char* , XGCRef& , const char* , size_t );
		implement_duplicate_internal_function_2( DataObject , SetValue , size_t , XGCRef& );
		implement_duplicate_internal_function_3( DataObject , AddValueToArray , const char* , XGCRef& , const char* );
		implement_duplicate_internal_function_3( DataObject , SetValue , const ::rux::XString& , XGCRef& , const char* );
		void DataObject::SetValue( const ::rux::XString& field_name , XGCRef& object , const char* field_names_delimiter )
		{
			//CODE_LABELS_INITIALIZE();
			//CODE_LABEL( 0 , 0 , 5 );
			XGCRef* gcref = &object;
			SetValue00( field_name.ConvertToUTF8().str() , (void*)gcref , ::rux::data::key_value_pair_t_type_gcref , SIZE_MAX , field_names_delimiter );
		};
		implement_duplicate_internal_function_4( DataObject , AddValue , const ::rux::XString& , XGCRef& , const char* , size_t );
		void DataObject::AddValue( const ::rux::XString& field_name , XGCRef& object , const char* field_names_delimiter , size_t ___rux__thread_index1986 )
		{
			CODE_LABEL_START( 0 , 0 , 5 );
			XGCRef* gcref = &object;
			AddValue00( field_name.ConvertToUTF8().str() , (void*)gcref , ::rux::data::key_value_pair_t_type_gcref , SIZE_MAX , field_names_delimiter );
			CODE_LABEL_END();
		};
		implement_duplicate_internal_function_3( DataObject , AddValueToArray , const ::rux::XString& , XGCRef& , const char* );
		void DataObject::AddValueToArray( const ::rux::XString& field_name , XGCRef& object , const char* field_names_delimiter )
		{
			//CODE_LABELS_INITIALIZE();
			//CODE_LABEL( 0 , 0 , 5 );
			XGCRef* gcref = &object;
			AddValueToArray00( field_name.ConvertToUTF8().str() , (void*)gcref , ::rux::data::key_value_pair_t_type_gcref , SIZE_MAX , field_names_delimiter );
		};
		implement_duplicate_internal_function_3( DataObject , SetValue , const ::rux::XString& , const XObject& , const char* );
		void DataObject::SetValue( const ::rux::XString& field_name , const XObject& object , const char* field_names_delimiter )
		{
			//CODE_LABELS_INITIALIZE();
			//CODE_LABEL( 0 , 0 , 5 );
			XGCRef* gcref = object.get_GCRef();
			SetValue00( field_name.ConvertToUTF8().str() , (void*)gcref , ::rux::data::key_value_pair_t_type_gcref , SIZE_MAX , field_names_delimiter );
		};
		implement_duplicate_internal_function_4( DataObject , AddValue , const ::rux::XString& , const XObject& , const char* , size_t );
		void DataObject::AddValue( const ::rux::XString& field_name , const XObject& object , const char* field_names_delimiter , size_t ___rux__thread_index1986 )
		{
			CODE_LABEL_START( 0 , 0 , 5 );
			XGCRef* gcref = object.get_GCRef();
			AddValue00( field_name.ConvertToUTF8().str() , (void*)gcref , ::rux::data::key_value_pair_t_type_gcref , SIZE_MAX , field_names_delimiter );
			CODE_LABEL_END();
		};
		implement_duplicate_internal_function_3( DataObject , AddValueToArray , const ::rux::XString& , const XObject& , const char* );
		void DataObject::AddValueToArray( const ::rux::XString& field_name , const XObject& object , const char* field_names_delimiter )
		{
			//CODE_LABELS_INITIALIZE();
			//CODE_LABEL( 0 , 0 , 5 );
			XGCRef* gcref = object.get_GCRef();
			AddValueToArray00( field_name.ConvertToUTF8().str() , (void*)gcref , ::rux::data::key_value_pair_t_type_gcref , SIZE_MAX , field_names_delimiter );
		};
		implement_duplicate_internal_function_5( DataObject , SetValue00 , const char* , void* , key_value_pair_t_type , size_t , const char* );
		void DataObject::SetValue00( const char* field_name , void* field_value , key_value_pair_t_type type , size_t global_length , const char* field_names_delimiter )
		{
			//CODE_LABELS_INITIALIZE();
			//CODE_LABEL( 0 , 0 , 5 );
			uint8 found = 0;
			char* ptr = 0;
			if( field_names_delimiter && field_names_delimiter[ 0 ] != 0 )
				ptr = strstr( const_cast< char* >( field_name ) , field_names_delimiter );
			if( ptr )
			{			
				size_t length = 0;
				if( ( length = ptr - field_name ) > 1 )
				{
					ptr += strlen( field_names_delimiter );
					if( *ptr != 0 )
					{
						::rux::uint32 field_hash = ::rux::engine::_globals->add_times33_hash( field_name , length );
						declare_variable_param( ::rux::data::XDataObject , section , GetValue< XDataObject >( field_hash , found ) );
						if( found == 0 )
						{
							::rux::data::DoubleLinkedListItem< ::rux::data::key_value_pair_t >* dl_item = alloc_object_macros( ::rux::data::DoubleLinkedListItem< ::rux::data::key_value_pair_t > );
							::rux::data::key_value_pair_t* key_value_pair = dl_item->operator->();
							key_value_pair->_name_hash = field_hash;
							key_value_pair->set00( section.get_GCRef( 1 , 1 ) , key_value_pair_t_type_gcref , 1 , SIZE_MAX , SIZE_MAX );
							_pairs.Add( dl_item );
						}
						section.SetValue00( ptr , field_value , type , global_length , field_names_delimiter );
					}
				}
			}
			else
			{
				::rux::uint32 field_hash = ::rux::engine::_globals->add_times33_hash( field_name , SIZE_MAX );
				SetValueByHash00( field_hash , field_value , type , global_length );
			}
		};
		void DataObject::SetValueByHash00( ::rux::uint32 field_hash , void* field_value , key_value_pair_t_type type , size_t global_length )
		{
			//CODE_LABELS_INITIALIZE();
			//CODE_LABEL( 0 , 0 , 5 );
			::rux::byte found = 0;
			if( _pairs.Count() > 0 )
			{
				key_value_pair_t* pair = 0;
				::rux::data::DoubleLinkedListItem< ::rux::data::key_value_pair_t >* dlitem = _pairs.Begin();
				while( dlitem )
				{
					pair = dlitem->operator->();
					dlitem = dlitem->Next();
					if( pair->_name_hash == field_hash )
					{			
						pair->set00( field_value , type , 1 , global_length , SIZE_MAX );
						found = 1;
						break;
					}					
				}
			}
			if( found == 0 )
			{
				::rux::data::DoubleLinkedListItem< ::rux::data::key_value_pair_t >* dl_item = alloc_object_macros( ::rux::data::DoubleLinkedListItem< ::rux::data::key_value_pair_t > );
				::rux::data::key_value_pair_t* key_value_pair = dl_item->operator->();
				key_value_pair->_name_hash = field_hash;
				key_value_pair->set00( field_value , type , 1 , global_length , SIZE_MAX );
				_pairs.Add( dl_item );
			}
		};
		void DataObject::SetValue( const char* field_name , XGCRef& object , const char* field_names_delimiter )
		{
			//CODE_LABELS_INITIALIZE();
			//CODE_LABEL( 0 , 0 , 5 );
			XGCRef* gcref = &object;
			SetValue00( field_name , (void*)gcref , ::rux::data::key_value_pair_t_type_gcref , SIZE_MAX , field_names_delimiter );
		};
		implement_duplicate_internal_function_6( DataObject , AddValue00 , const char* , void* , ::rux::data::key_value_pair_t_type , size_t , const char* , size_t );
		void DataObject::AddValue00( const char* field_name , void* value , ::rux::data::key_value_pair_t_type type , size_t global_length , const char* field_names_delimiter , size_t ___rux__thread_index1986 )
		{
			CODE_LABEL_START( 0 , 0 , 5 );
			uint8 found = 0;
			char* ptr = 0;
			if( field_names_delimiter && field_names_delimiter[ 0 ] != 0 )
				ptr = strstr( const_cast< char* >( field_name ) , field_names_delimiter );
			if( ptr )
			{			
				CODE_LABEL_START( 0 , 0 , 5 );
				size_t length = 0;
				if( ( length = ptr - field_name ) > 1 )
				{
					CODE_LABEL_START( 0 , 0 , 5 );
					ptr += strlen( field_names_delimiter );
					if( *ptr != 0 )
					{
						CODE_LABEL_START( 0 , 0 , 5 );
						::rux::uint32 field_hash = ::rux::engine::_globals->add_times33_hash( field_name , length );
						declare_variable_param( ::rux::data::XDataObject , section , GetValue< XDataObject >( field_hash , found ) );
						if( found == 0 )
						{
							CODE_LABEL_START( 0 , 0 , 5 );
							::rux::data::DoubleLinkedListItem< ::rux::data::key_value_pair_t >* dl_item = alloc_object_macros( ::rux::data::DoubleLinkedListItem< ::rux::data::key_value_pair_t > );
							::rux::data::key_value_pair_t* key_value_pair = dl_item->operator->();
							key_value_pair->_name_hash = field_hash;
							key_value_pair->set00( section.get_GCRef( 1 , 1 ) , ::rux::data::key_value_pair_t_type_gcref , 1 , SIZE_MAX , ___rux__thread_index1986 );
							_pairs.Add( dl_item );
							CODE_LABEL_END();
						}
						section.AddValue00( ptr , value , type , global_length , field_names_delimiter , ___rux__thread_index1986 );
						CODE_LABEL_END();
					}
					CODE_LABEL_END();
				}
				CODE_LABEL_END();
			}
			else
			{
				CODE_LABEL_START( 0 , 0 , 5 );
				::rux::data::DoubleLinkedListItem< ::rux::data::key_value_pair_t >* dl_item = alloc_object_macros( ::rux::data::DoubleLinkedListItem< ::rux::data::key_value_pair_t > );
				::rux::data::key_value_pair_t* key_value_pair = dl_item->operator->();
				key_value_pair->_name_hash = ::rux::engine::_globals->add_times33_hash( field_name , SIZE_MAX );
				key_value_pair->set00( value , type , 1 , global_length , ___rux__thread_index1986 );
				_pairs.Add( dl_item );
				CODE_LABEL_END();
			}
			CODE_LABEL_END();
		};
		void DataObject::AddValue( const char* field_name , XGCRef& object , const char* field_names_delimiter , size_t ___rux__thread_index1986 )
		{
			UNREFERENCED_PARAMETER( ___rux__thread_index1986 );
			XGCRef* gcref = &object;
			AddValue00( field_name , (void*)gcref , ::rux::data::key_value_pair_t_type_gcref , SIZE_MAX , field_names_delimiter );
		};
		implement_duplicate_internal_function_5( DataObject , AddValueToArray00 , const char* , void* , ::rux::data::key_value_pair_t_type , size_t , const char* );
		void DataObject::AddValueToArray00( const char* field_name , void* value , ::rux::data::key_value_pair_t_type type , size_t global_length , const char* field_names_delimiter )
		{
			//CODE_LABELS_INITIALIZE();
			//CODE_LABEL( 0 , 0 , 5 );
			uint8 found = 0;
			char* ptr = 0;
			if( field_names_delimiter && field_names_delimiter[ 0 ] != 0 )
				ptr = strstr( const_cast< char* >( field_name ) , field_names_delimiter );
			if( ptr )
			{			
				size_t length = 0;
				if( ( length = ptr - field_name ) > 1 )
				{
					ptr += strlen( field_names_delimiter );
					if( *ptr != 0 )
					{
						::rux::uint32 field_hash = ::rux::engine::_globals->add_times33_hash( field_name , length );
						declare_variable_param( ::rux::data::XDataObject , section , GetValue< XDataObject >( field_hash , found ) );
						if( found == 0 )
						{
							::rux::data::DoubleLinkedListItem< ::rux::data::key_value_pair_t >* dl_item = alloc_object_macros( ::rux::data::DoubleLinkedListItem< ::rux::data::key_value_pair_t > );
							::rux::data::key_value_pair_t* key_value_pair = dl_item->operator->();
							key_value_pair->_name_hash = field_hash;
							key_value_pair->set00( section.get_GCRef( 1 , 1 ) , ::rux::data::key_value_pair_t_type_gcref , 1 , SIZE_MAX , SIZE_MAX );
							_pairs.Add( dl_item );
						}
						section.AddValueToArray00( ptr , value , type , global_length , field_names_delimiter );
					}
				}
			}
			else
			{
				::rux::XArray< XObject > values;
				values.set_ByRef( GetValue< ::rux::XArray< XObject > >( field_name , found , SIZE_MAX , field_names_delimiter ) );
				if( found == 0 )
				{
					::rux::data::DoubleLinkedListItem< ::rux::data::key_value_pair_t >* dl_item = alloc_object_macros( ::rux::data::DoubleLinkedListItem< ::rux::data::key_value_pair_t > );
					::rux::data::key_value_pair_t* key_value_pair = dl_item->operator->();
					key_value_pair->_name_hash = ::rux::engine::_globals->add_times33_hash( field_name , SIZE_MAX );
					key_value_pair->set00( values.get_GCRef( 1 , 1 ) , ::rux::data::key_value_pair_t_type_gcref , 1 , SIZE_MAX , SIZE_MAX );
					_pairs.Add( dl_item );
				}
				values.Add( ::rux::data::key_value_pair_t::convert_to_object00( value , type , global_length ) );
			}
		};
		void DataObject::AddValueToArray( const char* field_name , XGCRef& object , const char* field_names_delimiter )
		{
			//CODE_LABELS_INITIALIZE();
			//CODE_LABEL( 0 , 0 , 5 );
			XGCRef* gcref = &object;
			AddValueToArray00( field_name , (void*)gcref , ::rux::data::key_value_pair_t_type_gcref , SIZE_MAX , field_names_delimiter );
		};
		void DataObject::AddValueToArray( const char* field_name , const XObject& object , const char* field_names_delimiter )
		{
			//CODE_LABELS_INITIALIZE();
			//CODE_LABEL( 0 , 0 , 5 );
			XGCRef* gcref = object.get_GCRef();
			AddValueToArray00( field_name , (void*)gcref , ::rux::data::key_value_pair_t_type_gcref , SIZE_MAX , field_names_delimiter );
		};
		void DataObject::SetValue( const char* field_name , const XObject& object , const char* field_names_delimiter )
		{
			//CODE_LABELS_INITIALIZE();
			//CODE_LABEL( 0 , 0 , 5 );
			XGCRef* gcref = object.get_GCRef();
			SetValue00( field_name , (void*)gcref , ::rux::data::key_value_pair_t_type_gcref , SIZE_MAX , field_names_delimiter );
		};
		void DataObject::AddValue( const char* field_name , const XObject& object , const char* field_names_delimiter , size_t ___rux__thread_index1986 )
		{
			CODE_LABEL_START( 0 , 0 , 5 );
			XGCRef* gcref = object.get_GCRef();
			AddValue00( field_name , (void*)gcref , ::rux::data::key_value_pair_t_type_gcref , SIZE_MAX , field_names_delimiter );
			CODE_LABEL_END();
		};
		void DataObject::SetValue( size_t field_index , const XObject& object )
		{
			//CODE_LABELS_INITIALIZE();
			//CODE_LABEL( 0 , 0 , 5 );
			XGCRef* gcref = object.get_GCRef();
			SetValue00( field_index , (void*)gcref , ::rux::data::key_value_pair_t_type_gcref , SIZE_MAX );
		};
		void DataObject::SetValue00( size_t field_index , void* value , key_value_pair_t_type type , size_t length )
		{
			//CODE_LABELS_INITIALIZE();
			//CODE_LABEL( 0 , 0 , 5 );
			_pairs[ field_index ]->operator->()->set00( value , type , 1 , length , SIZE_MAX );
		};
		void DataObject::SetValue( size_t field_index , XGCRef& object )
		{
			//CODE_LABELS_INITIALIZE();
			//CODE_LABEL( 0 , 0 , 5 );
			XGCRef* gcref = &object;
			_pairs[ field_index ]->operator->()->set00( (void*)gcref , ::rux::data::key_value_pair_t_type_gcref , 1 , SIZE_MAX , SIZE_MAX );
		};
		implement_duplicate_internal_function_3( DataObject , Merge , ::rux::data::XDataObject& , uint8& , ::rux::byte );
		void DataObject::Merge( ::rux::data::XDataObject& obj , uint8& changed , ::rux::byte remove )
		{
			Merge( &obj , changed , remove );
		};
		void DataObject::Merge( XDataObject* obj_ptr , uint8& changed , ::rux::byte remove )
		{
			changed = 0;
			::rux::data::DataObject* obj = (*obj_ptr)();
			declare_variable( XObject , value );
			::rux::data::key_value_pair_t* pair = 0;
			for( size_t index0 = 0 ; index0 < obj->FieldsCount() ; index0++ )
			{
				pair = obj->_pairs[ index0 ]->operator->();
				size_t field_index = GetFieldIndex< XObject >( obj->get_FieldNameHash( index0 ) );
				if( field_index == SIZE_MAX )
				{
					if( pair->_type > key_value_pair_t_type_gcref )
						AddValue00( obj->get_FieldNamePointer( index0 ) , &pair->_void_val , pair->_type , SIZE_MAX , 0 );
					else
						AddValue00( obj->get_FieldNamePointer( index0 ) , pair->_void_val , pair->_type , SIZE_MAX , 0 );
					changed = 1;
				}
				else
				{
					value.set_ByRef( get_FieldValue( field_index ) );
					Type* type = pair->get_type();
					if( rux_is_object_and_type_equal( value , type ) == false )
					{
						if( pair->_type > key_value_pair_t_type_gcref )
							SetValue00( field_index , &pair->_void_val , pair->_type , SIZE_MAX );
						else
							SetValue00( field_index , pair->_void_val , pair->_type , SIZE_MAX );
						changed = 1;
					}
					else if( rux_typeptr_is_class( type , ::rux::data::XDataObject )
						&& rux_is_object( value , XDataObject ) )
					{						
						declare_variable_param( ::rux::data::XDataObject , child_obj0 , pair->get() );
						declare_variable_param( ::rux::data::XDataObject , child_obj1 , value );
						::rux::byte local_changed = 0;
						child_obj1.Merge( child_obj0  , local_changed , remove );
						if( local_changed )
							changed = 1;
					}
				}
			}
			if( remove )
			{
				for( size_t index0 = 0 ; index0 < FieldsCount() ; index0++ )
				{
					size_t field_index = obj->GetFieldIndex< XObject >( get_FieldNameHash( index0 ) );
					if( field_index == SIZE_MAX )
					{
						changed = 1;
						RemoveAt( index0 );
						index0--;
					}
				}
			}
		};		
		implement_duplicate_internal_result_function_2( uint8 , DataObject , RemoveValue , const ::rux::XString& , const char* );
		uint8 DataObject::RemoveValue( const ::rux::XString& field_name , const char* field_names_delimiter )
		{
			return RemoveValue( field_name.ConvertToUTF8().str() , field_names_delimiter );
		};
		implement_duplicate_internal_result_function_2( uint8 , DataObject , RemoveValue , const char* , const char* );
		implement_duplicate_internal_result_function_1( uint8 , DataObject , RemoveAt , size_t );
		uint8 DataObject::RemoveAt( size_t field_index )
		{
			if( field_index < _pairs.Count() )
			{
				::rux::data::DoubleLinkedListItem< ::rux::data::key_value_pair_t >* item = _pairs[ field_index ];
				_pairs.Remove( item );
				::rux::engine::free_object< ::rux::data::DoubleLinkedListItem< ::rux::data::key_value_pair_t > >( item );
				return 1;
			}
			else
				return 0;
		};
		implement_duplicate_internal_result_function_1( ::rux::uint32 , DataObject , get_FieldNameHash , size_t );
		::rux::uint32 DataObject::get_FieldNameHash( size_t field_index )
		{
			return _pairs[ field_index ]->operator->()->_name_hash;
		};
		uint8 DataObject::RemoveValue( const char* field_name , const char* field_names_delimiter )
		{
			uint8 found = 0;
			if( _pairs.Count() > 0 )
			{
				::rux::uint32 field_hash = ::rux::cryptography::hash::times33_hash( field_name , SIZE_MAX );
				key_value_pair_t* pair = 0;
				::rux::data::DoubleLinkedListItem< ::rux::data::key_value_pair_t >* dlitem = _pairs.Begin();
				while( dlitem )
				{
					pair = dlitem->operator->();
					if( pair->_name_hash == field_hash )
					{		
						found = 1;
						_pairs.Remove( dlitem );
						::rux::engine::free_object< ::rux::data::DoubleLinkedListItem< ::rux::data::key_value_pair_t > >( dlitem );
						break;
					}
					dlitem = dlitem->Next();
				}
				if( found == 0 )
				{			
					char* ptr = 0;
					if( field_names_delimiter && field_names_delimiter[ 0 ] != 0 )
						ptr = strstr( const_cast< char* >( field_name ) , field_names_delimiter );
					size_t length = 0;
					if( ptr && ( length = ptr - field_name ) > 1 )
					{
						ptr += strlen( field_names_delimiter );
						if( *ptr != 0 )
						{
							XObject section( GetValue< XObject >( field_name , found , length , field_names_delimiter ) , "obj" , __FILE__ , __LINE__ );
							if( found == 1 )
							{
								XGCRef* gcref = section.get_GCRef();
								if( gcref && gcref->get_RuxTypeIndex() == XDataObject::get_static_RuxTypeIndex() )
								{
									DataObject* child = (DataObject*)gcref->get_ObjectAddress();
									found = child->RemoveValue( ptr , field_names_delimiter );
								}
								else
									found = 0;
							}
						}
					}
				}
			}
			return found;
		};
		implement_duplicate_internal_result_function_with_const( size_t , DataObject , FieldsCount );
		size_t DataObject::FieldsCount( void )
		{
			return _pairs.Count();
		};
		implement_duplicate_internal_result_function_1( Object& , DataObject , operator[] , size_t );
		Object& DataObject::operator[] ( size_t field_index )
		{
			return get_FieldValue( field_index );
		};
		implement_duplicate_internal_result_function_1_with_const( ::rux::String& , DataObject , get_FieldName , size_t );
		implement_duplicate_internal_result_function_1_with_const( Object& , DataObject , get_FieldValue , size_t );
		implement_duplicate_internal_result_function_1_with_const( char* , DataObject , get_FieldNamePointer , size_t );
		char* DataObject::get_FieldNamePointer( size_t field_index )
		{
			return ::rux::engine::_globals->get_time33_hash_value( _pairs[ field_index ]->operator->()->_name_hash );
		};
		::rux::String& DataObject::get_FieldName( size_t field_index )
		{
			return ::rux::XString( get_FieldNamePointer( field_index ) , XEnumCodePage_UTF8 , 1 , NULL , __FILE__ , __LINE__ )++;
		};
		Object& DataObject::get_FieldValue( size_t field_index )
		{
			return _pairs[ field_index ]->operator->()->get();
		};
	};
};