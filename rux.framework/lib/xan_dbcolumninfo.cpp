#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_dbcolumninfo.h>
#include <xan_keyvaluepair.h>
#include <xan_uint64.h>
#include <xan_boolean.h>
begin_implement_rux_class_ns( DBColumnInfo , rux::data );
	_is_primary_key = 0;
	_is_not_null = 0;
	_is_auto_increment = 0;
end_implement_rux_class();
namespace rux
{
	namespace data
	{
		uint8 DBColumnInfo::get_IsPrimaryKey( void )
		{
			return _is_primary_key;
		};
		implement_duplicate_internal_result_function( rux::String& , DBColumnInfo , get_ColumnName );
		implement_duplicate_internal_result_function( rux::String& , DBColumnInfo , get_ColumnType );
		implement_duplicate_internal_result_function( rux::String& , DBColumnInfo , get_DefaultValue );
		implement_duplicate_internal_function_1( DBColumnInfo , get_ColumnName , rux::XString& );
		implement_duplicate_internal_function_1( DBColumnInfo , get_ColumnType , rux::XString& );
		implement_duplicate_internal_function_1( DBColumnInfo , get_DefaultValue , rux::XString& );
		implement_duplicate_internal_result_function( uint8 , DBColumnInfo , get_IsNotNull );
		implement_duplicate_internal_result_function( uint8 , DBColumnInfo , get_IsAutoIncrement );
		XDBColumnInfo::XDBColumnInfo( const rux::XString& column_name , const rux::XString& column_type , const rux::XString& default_value , uint8 is_not_null , uint8 is_primary_key )
		{
			(*this)()->_column_name = column_name;
			(*this)()->_column_type.set_ByRef( column_type.ToLower() );
			(*this)()->_default_value = default_value;
			(*this)()->_is_not_null = is_not_null;
			(*this)()->_is_primary_key = is_primary_key;
		};
		rux::String& DBColumnInfo::get_ColumnName( void )
		{
			return _column_name++;
		};
		rux::String& DBColumnInfo::get_ColumnType( void )
		{
			return _column_type++;
		};
		rux::String& DBColumnInfo::get_DefaultValue( void )
		{
			return _default_value++;
		};	
		void DBColumnInfo::get_ColumnName( rux::XString& column_name )
		{
			column_name = _column_name;
		};
		void DBColumnInfo::get_ColumnType( rux::XString& column_type )
		{
			column_type = _column_type;
		};
		void DBColumnInfo::get_DefaultValue( rux::XString& column_default_value )
		{
			column_default_value = _default_value;
		};
		uint8 XDBColumnInfo::get_IsPrimaryKey( void )
		{
			return (*this)()->_is_primary_key;
		};
		uint8 DBColumnInfo::get_IsNotNull( void )
		{
			return _is_not_null;
		};
		uint8 DBColumnInfo::get_IsAutoIncrement( void )
		{
			return _is_auto_increment;
		};
	};
};