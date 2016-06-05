#ifndef XAN_DATAOBJECT_H
#define XAN_DATAOBJECT_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_keyvaluepair.h>
#include <xan_hash.h>
#include <xan_double_linked_list.h>
#define OLD_KEY_VALUE_PAIR 0
namespace rux
{
	namespace io
	{
		class XDirectory;
	};
	namespace data
	{		
		enum key_value_pair_t_type
		{
			key_value_pair_t_type_large_utf8_string = 0 ,
			key_value_pair_t_type_gcref = 1 , 
			key_value_pair_t_type_small_utf8_string = 2 , 
			key_value_pair_t_type_float = 3 ,
			key_value_pair_t_type_double = 4 ,
			key_value_pair_t_type_int32 = 5 ,
			key_value_pair_t_type_uint32 = 6 ,
			key_value_pair_t_type_uint64 = 7 ,
			key_value_pair_t_type_int64 = 8 ,
			key_value_pair_t_type_boolean = 9 , 
			key_value_pair_t_type_null = 10
		};
		struct key_value_pair_t
		{
			friend class rux::data::DataObject;
			union
			{
				void* _void_val;
				char _small_utf8_string[ 16 ];
				char* _large_utf8_string;
				XGCRef* _gcref;
				float _float;
				double _double;
				::rux::int32 _int32_val;
				::rux::uint32 _uint32;
				::rux::uint64 _uint64;
				::rux::int64 _int64_val;
				::rux::byte _boolean;
			};
			::rux::uint32 _name_hash;
			key_value_pair_t_type _type;
			key_value_pair_t();
			~key_value_pair_t();
			void set00( void* value , key_value_pair_t_type type , ::rux::byte gcref_addref , size_t length , size_t ___rux__thread_index1986 );
			Object& get( void );

			
			static Object& convert_to_object( char* value , size_t length );
			static Object& convert_to_object( XGCRef* value );
			static Object& convert_to_object( float value );
			static Object& convert_to_object( double value );

			static Object& convert_to_object( ::rux::int32 value );
			static Object& convert_to_object( ::rux::uint32 value );
			static Object& convert_to_object( ::rux::uint64 value );
			static Object& convert_to_object( ::rux::int64 value );
			static Object& convert_to_object( ::rux::byte value );
			static Object& convert_to_object( void );

			Type* get_type( void );
			void clear( void );
		private:
			static Object& convert_to_object00( void* value , key_value_pair_t_type type , size_t length );
		};
		class XDataObject;
		class DataObject : public XGCRef
		{
			declare_rux_base_internal_methods();
			friend class XDataObject;
			friend class XJsonSerializer;
			friend class XXmlSerializer;
			friend class XDBSchema;
			friend class rux::io::XDirectory;
		private:
			::rux::data::DoubleLinkedList< key_value_pair_t > _pairs;
			void initialize_members( const char* variable_name , const char* __file__ , ::rux::int32 __line__ );
		public:
			explicit DataObject( const char* variable_name = 0 , const char* __file__ = 0 , ::rux::int32 __line__ = 0 );
			~DataObject();
			Object& operator[] ( const rux::XString& field_name );
			Object& operator[] ( const char* field_name );	
			Object& operator[] ( size_t field_index );
			::rux::uint32 GetStringValueHash( const ::rux::XString& field_name , uint8& found , size_t field_name_length = SIZE_MAX , const char* field_names_delimiter = 0 );
			::rux::uint32 GetStringValueHash( const char* field_name , uint8& found , size_t field_name_length = SIZE_MAX , const char* field_names_delimiter = 0 );
			::rux::uint32 GetStringValueHash( ::rux::uint32 field_hash , uint8& found );
			uint8 RemoveAt( size_t field_index );
			uint8 RemoveValue( const char* field_name , const char* field_names_delimiter );
			template< class T >
			typename T::InternalClass& GetValue( ::rux::uint32 field_hash , uint8& found )
			{	
				found = 0;
				T field_value;
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
							if( rux_is_class( T , XObject ) )
							{
								field_value.set_ByRef( pair->get() );
								found = 1;
							}
							else
							{
								XObject pairs_value( pair->get() );
								if( rux_is_object( pairs_value , T ) )
								{
									field_value.set_ByRef( pairs_value );
									found = 1;
								}
							}
							break;
						}
					}
				}
				return field_value++;
			};
			template< class T >
			size_t GetFieldIndex( ::rux::uint32 field_hash )
			{
				size_t field_index = SIZE_MAX;
				if( _pairs.Count() > 0 )
				{	
					key_value_pair_t* pair = 0;
					size_t index0 = 0;
					::rux::data::DoubleLinkedListItem< ::rux::data::key_value_pair_t >* dlitem = _pairs.Begin();
					while( dlitem )
					{
						pair = dlitem->operator->();
						dlitem = dlitem->Next();
						if( pair->_name_hash == field_hash )
						{		
							if( rux_is_class( T , XObject ) )
								field_index = index0;
							else
							{
								XObject pairs_value( pair->get() );
								if( rux_is_object( pairs_value , T ) )
									field_index = index0;
							}
							break;
						}
						index0++;
					}
				}
				return field_index;
			};
			template< class T >
			typename T::InternalClass& GetValue( const char* field_name , uint8& found , size_t field_name_length = SIZE_MAX , const char* field_names_delimiter = 0 )
			{	
				found = 0;
				T field_value;
				if( _pairs.Count() > 0 )
				{	
					::rux::uint32 field_hash = ::rux::cryptography::hash::times33_hash( field_name , field_name_length );
					field_value.set_ByRef( GetValue< T >( field_hash , found ) );
					if( found == 0 )
					{			
						char* ptr = 0;
						if( field_names_delimiter && field_names_delimiter[ 0 ] != 0 )
							ptr = strstr( const_cast< char* >( field_name ) , field_names_delimiter );
						size_t length = 0;
						if( ptr && ( length = ptr - field_name ) > 1 )
						{			
							if( field_name_length > length )
							{
								ptr += strlen( field_names_delimiter );
								if( *ptr != 0 )
								{
									XObject section( GetValue< XObject >( field_name , found , length , 0 ) );
									if( found == 1 )
									{
										XGCRef* gcref = section.get_GCRef();
										if( gcref && gcref->get_RuxTypeIndex() == DataObject::get_static_RuxTypeIndex() )
										{
											DataObject* child = cpp_ptr_cast( DataObject* , const_cast< void* >( gcref->get_ObjectAddress() ) );
											field_value.set_ByRef( child->GetValue< T >( ptr , found , SIZE_MAX , field_names_delimiter ) );
										}
										else
											found = 0;
									}
								}
							}
						}
					}
				}
				return field_value++;
			};
			void AddValue( const char* field_name , XGCRef& object , const char* field_names_delimiter , size_t ___rux__thread_index1986 = SIZE_MAX );
			void AddValue( const char* field_name , const XObject& object , const char* field_names_delimiter , size_t ___rux__thread_index1986 = SIZE_MAX );
			void SetValue( const char* field_name , const XObject& object , const char* field_names_delimiter );
			void AddValueToArray( const char* field_name , const XObject& object , const char* field_names_delimiter );
			void SetValue( const char* field_name , XGCRef& object , const char* field_names_delimiter );			
			void AddValueToArray( const char* field_name , XGCRef& object , const char* field_names_delimiter );
			void SetValue( size_t field_index , const XObject& object );
			void SetValue( size_t field_index , XGCRef& object );
			void Merge( XDataObject* obj , uint8& changed , ::rux::byte remove );
			size_t FieldsCount( void );
			uint64 get_FieldNameHash( size_t field_index );
			rux::String& get_FieldName( size_t field_index );
			char* get_FieldNamePointer( size_t field_index );
			Object& get_FieldValue( size_t field_index );
			void ToStruct( XObject& obj );
			
			char* get_FieldNamePointer( size_t field_index ) const;
			void SetValue( const rux::XString& field_name , const XObject& object , const char* field_names_delimiter );
			void AddValueToArray( const rux::XString& field_name , const XObject& object , const char* field_names_delimiter );
			void SetValue( const rux::XString& field_name , XGCRef& object , const char* field_names_delimiter );
			void AddValueToArray( const rux::XString& field_name , XGCRef& object , const char* field_names_delimiter );
			void SetValue( const char* field_name , const char* object , ::rux::byte utf8 = 0 , size_t length = SIZE_MAX , const char* field_names_delimiter = 0 );
			void AddValueToArray( const char* field_name , const char* object , ::rux::byte utf8 = 0 , size_t length = SIZE_MAX , const char* field_names_delimiter = 0 );
			void SetValue( const rux::XString& field_name , const char* object , ::rux::byte utf8 = 0 , size_t length = SIZE_MAX , const char* field_names_delimiter = 0 );
			void AddValueToArray( const rux::XString& field_name , const char* object , ::rux::byte utf8 = 0 , size_t length = SIZE_MAX , const char* field_names_delimiter = 0 );
			void SetValue( const char* field_name , bool object , const char* field_names_delimiter );
			void AddValueToArray( const char* field_name , bool object , const char* field_names_delimiter );
			void SetValue( const rux::XString& field_name , bool object , const char* field_names_delimiter );
			void AddValueToArray( const rux::XString& field_name , bool object , const char* field_names_delimiter );
			void SetValue( const char* field_name , int64 object , const char* field_names_delimiter );
			void AddValueToArray( const char* field_name , int64 object , const char* field_names_delimiter );
			void SetValue( const rux::XString& field_name , int64 object , const char* field_names_delimiter );
			void AddValueToArray( const rux::XString& field_name , int64 object , const char* field_names_delimiter );
			void SetValue( const char* field_name , uint64 object , const char* field_names_delimiter );
			void AddValueToArray( const char* field_name , uint64 object , const char* field_names_delimiter );
			void SetValue( const rux::XString& field_name , uint64 object , const char* field_names_delimiter );
			void AddValueToArray( const rux::XString& field_name , uint64 object , const char* field_names_delimiter );
			void SetValue( const char* field_name , double object , const char* field_names_delimiter );
			void AddValueToArray( const char* field_name , double object , const char* field_names_delimiter );
			void SetValue( const rux::XString& field_name , double object , const char* field_names_delimiter );
			void AddValueToArray( const rux::XString& field_name , double object , const char* field_names_delimiter );
			void SetValue( const char* field_name , float object , const char* field_names_delimiter );
			void AddValueToArray( const char* field_name , float object , const char* field_names_delimiter );
			void SetValue( const rux::XString& field_name , float object , const char* field_names_delimiter );
			void AddValueToArray( const rux::XString& field_name , float object , const char* field_names_delimiter );
			void SetValue( const char* field_name , uint8 object , const char* field_names_delimiter );
			void AddValueToArray( const char* field_name , uint8 object , const char* field_names_delimiter );
			void SetValue( const rux::XString& field_name , uint8 , const char* field_names_delimiter );
			void AddValueToArray( const rux::XString& field_name , uint8 object , const char* field_names_delimiter );
			void Merge( rux::data::XDataObject& obj , uint8& changed , ::rux::byte remove );
			uint8 RemoveValue( const rux::XString& field_name , const char* field_names_delimiter );
			size_t FieldsCount( void ) const;
			rux::String& get_FieldName( size_t field_index ) const;
			Object& get_FieldValue( size_t field_index ) const;
			void SetValue( size_t field_index , const char* object , ::rux::byte utf8 = 0 , size_t length = SIZE_MAX );
			void SetValue( size_t field_index , bool object );
			void SetValue( size_t field_index , int64 object );
			void SetValue( size_t field_index , double object );
			void SetValue( size_t field_index , float object );
			void SetValue( size_t field_index , uint8 object );

			void AddValue( const rux::XString& field_name , const XObject& object , const char* field_names_delimiter , size_t ___rux__thread_index1986 = SIZE_MAX );
			void AddValue( const rux::XString& field_name , XGCRef& object , const char* field_names_delimiter , size_t ___rux__thread_index1986 = SIZE_MAX );
			void AddValue( const char* field_name , const char* object , ::rux::byte utf8 = 0 , size_t length = SIZE_MAX , const char* field_names_delimiter = "->" , size_t ___rux__thread_index1986 = SIZE_MAX );
			void AddValue( const rux::XString& field_name , const char* object , ::rux::byte utf8 = 0 , size_t length = SIZE_MAX , const char* field_names_delimiter = "->" , size_t ___rux__thread_index1986 = SIZE_MAX );
			void AddValue( const char* field_name , bool object , const char* field_names_delimiter , size_t ___rux__thread_index1986 = SIZE_MAX );
			void AddValue( const rux::XString& field_name , bool object , const char* field_names_delimiter , size_t ___rux__thread_index1986 = SIZE_MAX );
			void AddValue( const char* field_name , int64 object , const char* field_names_delimiter , size_t ___rux__thread_index1986 = SIZE_MAX );
			void AddValue( const rux::XString& field_name , int64 object , const char* field_names_delimiter , size_t ___rux__thread_index1986 = SIZE_MAX );
			void AddValue( const char* field_name , double object , const char* field_names_delimiter , size_t ___rux__thread_index1986 = SIZE_MAX );
			void AddValue( const rux::XString& field_name , double object , const char* field_names_delimiter , size_t ___rux__thread_index1986 = SIZE_MAX );
			void AddValue( const char* field_name , float object , const char* field_names_delimiter , size_t ___rux__thread_index1986 = SIZE_MAX );
			void AddValue( const rux::XString& field_name , float object , const char* field_names_delimiter , size_t ___rux__thread_index1986 = SIZE_MAX );
			void AddValue( const char* field_name , uint8 object , const char* field_names_delimiter , size_t ___rux__thread_index1986 = SIZE_MAX );
			void AddValue( const rux::XString& field_name , uint8 , const char* field_names_delimiter , size_t ___rux__thread_index1986 = SIZE_MAX );
			void SetValue( const char* field_name , uint32 object , const char* field_names_delimiter , size_t ___rux__thread_index1986 = SIZE_MAX );
			void AddValue( const char* field_name , uint32 object , const char* field_names_delimiter , size_t ___rux__thread_index1986 = SIZE_MAX );
			void SetValue( size_t field_index , uint32 object );
			void AddValueToArray( const char* field_name , uint32 object , const char* field_names_delimiter );
			void SetValue( const ::rux::XString& field_name , uint32 object , const char* field_names_delimiter );
			void AddValue( const ::rux::XString& field_name , uint32 object , const char* field_names_delimiter , size_t ___rux__thread_index1986 = SIZE_MAX );
			void AddValueToArray( const ::rux::XString& field_name , uint32 object , const char* field_names_delimiter );
			void AddValue( const ::rux::XString& field_name , uint64 object , const char* field_names_delimiter , size_t ___rux__thread_index1986 = SIZE_MAX );
			void SetValue( size_t field_index , uint64 object );
			void AddValue( const char* field_name , uint64 object , const char* field_names_delimiter , size_t ___rux__thread_index1986 = SIZE_MAX );			
		private:
			void AddValueToArray00( const char* field_name , void* value , ::rux::data::key_value_pair_t_type type , size_t length , const char* field_names_delimiter );
			void AddValue00( const char* field_name , void* value , ::rux::data::key_value_pair_t_type type , size_t length , const char* field_names_delimiter , size_t ___rux__thread_index1986 = SIZE_MAX );
			void SetValue00( size_t field_index , void* value , key_value_pair_t_type type , size_t length );
			void SetValueByHash00( ::rux::uint32 field_hash , void* field_value , key_value_pair_t_type type , size_t global_length );
			void SetValue00( const char* field_name , void* field_value , key_value_pair_t_type type , size_t length , const char* field_names_delimiter );
		};
		class XDataObject : public XGCHandle<DataObject>
		{
			friend class DataObject;
			declare_rux_base_methods( DataObject );
		private:
		public:
			declare_rux_set_operators( DataObject );
			XDataObject();
			XDataObject( const char* field0 , ::rux::int64 value0 , const char* field1 , ::rux::int64 value1 );
			XDataObject( const rux::data::DataObject& dataobject );
			XDataObject( const rux::data::XDataObject& dataobject );

			explicit XDataObject( const XObject& object );
			explicit XDataObject( XGCRef* gcref );

			Object& operator[] ( const rux::XString& field_name );
			Object& operator[] ( const char* field_name );
			::rux::uint32 GetStringValueHash( const ::rux::XString& field_name , uint8& found , size_t field_name_length = SIZE_MAX , const char* field_names_delimiter = 0 );
			::rux::uint32 GetStringValueHash( const char* field_name , uint8& found , size_t field_name_length = SIZE_MAX , const char* field_names_delimiter = 0 );
			::rux::uint32 GetStringValueHash( ::rux::uint32 field_hash , uint8& found );
			uint8 RemoveAt( size_t field_index );
			uint64 get_FieldNameHash( size_t field_index );
			char* get_FieldNamePointer( size_t field_index ) const;
			template< class T >
			typename T::InternalClass& GetValue( const char* field_name , uint8& found , size_t field_name_length , const char* field_names_delimiter ) const
			{	
				return (*this)()->GetValue< T >( field_name , found , field_name_length , field_names_delimiter );
			};
			template< class T >
			typename T::InternalClass& GetValue( const rux::XString& field_name , uint8& found , size_t field_name_length , const char* field_names_delimiter ) const
			{	
				rux::XString utf8_field_name;
				utf8_field_name.set_ByRef( field_name.ConvertToUTF8() );	
				return (*this)()->GetValue< T >( utf8_field_name.str() , found , field_name_length , field_names_delimiter );
			};
			template< class T >
			typename T::InternalClass& GetValue( ::rux::uint32 field_hash , uint8& found )
			{
				return (*this)()->GetValue< T >( field_hash , found );
			};
			void SetValue( const char* field_name , const XObject& object , const char* field_names_delimiter );
			void AddValueToArray( const char* field_name , const XObject& object , const char* field_names_delimiter );
			void SetValue( const rux::XString& field_name , const XObject& object , const char* field_names_delimiter );
			void AddValueToArray( const rux::XString& field_name , const XObject& object , const char* field_names_delimiter );

			void SetValue( const char* field_name , XGCRef& object , const char* field_names_delimiter );
			void AddValueToArray( const char* field_name , XGCRef& object , const char* field_names_delimiter );
			void SetValue( const rux::XString& field_name , XGCRef& object , const char* field_names_delimiter );
			void AddValueToArray( const rux::XString& field_name , XGCRef& object , const char* field_names_delimiter );

			void SetValue( const char* field_name , const char* object , ::rux::byte utf8 = 0 , size_t length = SIZE_MAX , const char* field_names_delimiter = 0 );
			void AddValueToArray( const char* field_name , const char* object , ::rux::byte utf8 = 0 , size_t length = SIZE_MAX , const char* field_names_delimiter = 0 );
			void SetValue( const rux::XString& field_name , const char* object , ::rux::byte utf8 = 0 , size_t length = SIZE_MAX , const char* field_names_delimiter = 0 );
			void AddValueToArray( const rux::XString& field_name , const char* object , ::rux::byte utf8 = 0 , size_t length = SIZE_MAX , const char* field_names_delimiter = 0 );
			void SetValue( const char* field_name , bool object , const char* field_names_delimiter );
			void AddValueToArray( const char* field_name , bool object , const char* field_names_delimiter );
			void SetValue( const rux::XString& field_name , bool object , const char* field_names_delimiter );
			void AddValueToArray( const rux::XString& field_name , bool object , const char* field_names_delimiter );
			void SetValue( const char* field_name , int64 object , const char* field_names_delimiter );
			void AddValueToArray( const char* field_name , int64 object , const char* field_names_delimiter );
			void SetValue( const rux::XString& field_name , int64 object , const char* field_names_delimiter );
			void AddValueToArray( const rux::XString& field_name , int64 object , const char* field_names_delimiter );
			void SetValue( const char* field_name , uint64 object , const char* field_names_delimiter );
			void AddValueToArray( const char* field_name , uint64 object , const char* field_names_delimiter );
			void SetValue( const rux::XString& field_name , uint64 object , const char* field_names_delimiter );
			void AddValueToArray( const rux::XString& field_name , uint64 object , const char* field_names_delimiter );
			void SetValue( const char* field_name , double object , const char* field_names_delimiter );
			void AddValueToArray( const char* field_name , double object , const char* field_names_delimiter );
			void SetValue( const rux::XString& field_name , double object , const char* field_names_delimiter );
			void AddValueToArray( const rux::XString& field_name , double object , const char* field_names_delimiter );
			void SetValue( const char* field_name , float object , const char* field_names_delimiter );
			void AddValueToArray( const char* field_name , float object , const char* field_names_delimiter );
			void SetValue( const rux::XString& field_name , float object , const char* field_names_delimiter );
			void AddValueToArray( const rux::XString& field_name , float object , const char* field_names_delimiter );
			void SetValue( const char* field_name , uint8 object , const char* field_names_delimiter );
			void AddValueToArray( const char* field_name , uint8 object , const char* field_names_delimiter );
			void SetValue( const rux::XString& field_name , uint8 , const char* field_names_delimiter );
			void AddValueToArray( const rux::XString& field_name , uint8 object , const char* field_names_delimiter );
			void Merge( rux::data::XDataObject& obj , uint8& changed , ::rux::byte remove );
			uint8 RemoveValue( const rux::XString& field_name , const char* field_names_delimiter );
			uint8 RemoveValue( const char* field_name , const char* field_names_delimiter );
			size_t FieldsCount( void ) const;
			Object& operator[] ( size_t field_index );
			rux::String& get_FieldName( size_t field_index ) const;
			Object& get_FieldValue( size_t field_index ) const;
			void SetValue( size_t field_index , const XObject& object );
			void SetValue( size_t field_index , XGCRef& object );
			void SetValue( size_t field_index , const char* object , ::rux::byte utf8 = 0 , size_t length = SIZE_MAX );
			void SetValue( size_t field_index , bool object );
			void SetValue( size_t field_index , int64 object );
			void SetValue( size_t field_index , double object );
			void SetValue( size_t field_index , float object );
			void SetValue( size_t field_index , uint8 object );

			void AddValue( const char* field_name , const XObject& object , const char* field_names_delimiter , size_t ___rux__thread_index1986 = SIZE_MAX );
			void AddValue( const rux::XString& field_name , const XObject& object , const char* field_names_delimiter , size_t ___rux__thread_index1986 = SIZE_MAX );
			void AddValue( const char* field_name , XGCRef& object , const char* field_names_delimiter , size_t ___rux__thread_index1986 = SIZE_MAX );
			void AddValue( const rux::XString& field_name , XGCRef& object , const char* field_names_delimiter , size_t ___rux__thread_index1986 = SIZE_MAX );
			void AddValue( const char* field_name , const char* object , ::rux::byte utf8 = 0 , size_t length = SIZE_MAX , const char* field_names_delimiter = "->" , size_t ___rux__thread_index1986 = SIZE_MAX );
			void AddValue( const rux::XString& field_name , const char* object , ::rux::byte utf8 = 0 , size_t length = SIZE_MAX , const char* field_names_delimiter = "->" , size_t ___rux__thread_index1986 = SIZE_MAX );
			void AddValue( const char* field_name , bool object , const char* field_names_delimiter , size_t ___rux__thread_index1986 = SIZE_MAX );
			void AddValue( const rux::XString& field_name , bool object , const char* field_names_delimiter , size_t ___rux__thread_index1986 = SIZE_MAX );
			void AddValue( const char* field_name , int64 object , const char* field_names_delimiter , size_t ___rux__thread_index1986 = SIZE_MAX );
			void AddValue( const rux::XString& field_name , int64 object , const char* field_names_delimiter , size_t ___rux__thread_index1986 = SIZE_MAX );
			void AddValue( const char* field_name , double object , const char* field_names_delimiter , size_t ___rux__thread_index1986 = SIZE_MAX );
			void AddValue( const rux::XString& field_name , double object , const char* field_names_delimiter , size_t ___rux__thread_index1986 = SIZE_MAX );
			void AddValue( const char* field_name , float object , const char* field_names_delimiter , size_t ___rux__thread_index1986 = SIZE_MAX );
			void AddValue( const rux::XString& field_name , float object , const char* field_names_delimiter , size_t ___rux__thread_index1986 = SIZE_MAX );
			void AddValue( const char* field_name , uint8 object , const char* field_names_delimiter , size_t ___rux__thread_index1986 = SIZE_MAX );
			void AddValue( const rux::XString& field_name , uint8 , const char* field_names_delimiter , size_t ___rux__thread_index1986 = SIZE_MAX );
			void ToStruct( XObject& obj );
			static void ToStruct( XObject& data_obj , XObject& struct_obj );
			static Object& FromStruct( XObject& struct_obj );
			void SetValue( const char* field_name , uint32 object , const char* field_names_delimiter );
			void AddValue( const char* field_name , uint32 object , const char* field_names_delimiter , size_t ___rux__thread_index1986 = SIZE_MAX );
			void SetValue( size_t field_index , uint32 object );
			void AddValueToArray( const char* field_name , uint32 object , const char* field_names_delimiter );
			void SetValue( const ::rux::XString& field_name , uint32 object , const char* field_names_delimiter );
			void AddValue( const ::rux::XString& field_name , uint32 object , const char* field_names_delimiter , size_t ___rux__thread_index1986 = SIZE_MAX );
			void AddValueToArray( const ::rux::XString& field_name , uint32 object , const char* field_names_delimiter );
			void AddValue( const ::rux::XString& field_name , uint64 object , const char* field_names_delimiter , size_t ___rux__thread_index1986 = SIZE_MAX );
			void SetValue( size_t field_index , uint64 object );
			void AddValue( const char* field_name , uint64 object , const char* field_names_delimiter , size_t ___rux__thread_index1986 = SIZE_MAX );
		private:
			void SetValue00( const char* field_name , void* field_value , key_value_pair_t_type type , size_t length , const char* field_names_delimiter );
			void SetValue00( size_t field_index , void* value , key_value_pair_t_type type , size_t length );
			void AddValueToArray00( const char* field_name , void* value , ::rux::data::key_value_pair_t_type type , size_t length , const char* field_names_delimiter );
			void AddValue00( const char* field_name , void* value , ::rux::data::key_value_pair_t_type type , size_t length , const char* field_names_delimiter , size_t ___rux__thread_index1986 = SIZE_MAX );
		};
	};
};
#endif
