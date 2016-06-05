#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_dbtableinfo.h>
#include <xan_keyvaluepair.h>
#include <xan_uint64.h>
#include <xan_boolean.h>
begin_implement_rux_class_ns( DBTableInfo , rux::data );
end_implement_rux_class();
namespace rux
{
	namespace data
	{
		implement_duplicate_internal_result_function( rux::String& , DBTableInfo , get_TableName );
		implement_duplicate_internal_function_1( DBTableInfo , get_TableName , rux::XString& );
		implement_duplicate_internal_result_function( rux::Array<rux::data::XDBColumnInfo>& , DBTableInfo , get_ColumnInfos );
		implement_duplicate_internal_result_function( rux::Array<rux::data::XDBColumnInfo>& , DBTableInfo , get_PrimaryKeyColumnInfos );
		implement_duplicate_internal_result_function_2( rux::data::DBColumnInfo& , DBTableInfo , get_ColumnInfo , const rux::XString& , rux::XString& );
		implement_duplicate_internal_function_1( DBTableInfo , get_ColumnInfos , rux::XArray<rux::data::XDBColumnInfo>& );
		implement_duplicate_internal_function_1( DBTableInfo , get_PrimaryKeyColumnInfos , rux::XArray<rux::data::XDBColumnInfo>& );
		XDBTableInfo::XDBTableInfo( const rux::XString& db_table_name )
		{
			(*this)()->_db_table_name = db_table_name;
		};
		rux::String& DBTableInfo::get_TableName( void )
		{
			return _db_table_name++;
		};
		void DBTableInfo::get_TableName( rux::XString& table_name )
		{
			table_name = _db_table_name;
		}
		rux::Array<rux::data::XDBColumnInfo>& DBTableInfo::get_ColumnInfos( void )
		{
			return _db_column_infos++;
		};
		rux::Array<rux::data::XDBColumnInfo>& DBTableInfo::get_PrimaryKeyColumnInfos( void )
		{
			return _db_primary_key_column_infos++;
		};
		void DBTableInfo::get_ColumnInfos( rux::XArray<rux::data::XDBColumnInfo>& column_infos )
		{
			column_infos = _db_column_infos;
		};
		void DBTableInfo::get_PrimaryKeyColumnInfos( rux::XArray<rux::data::XDBColumnInfo>& primary_key_column_infos )
		{
			primary_key_column_infos = _db_primary_key_column_infos;
		};
		DBColumnInfo& DBTableInfo::get_ColumnInfo( const rux::XString& column_name , rux::XString& error )
		{
			error.Clear();
			rux::data::XDBColumnInfo db_column_info;
			uint32 index = 0;
			rux::XString lower_column_name0 , lower_column_name1;
			lower_column_name0.set_ByRef( column_name.ToLower() );
			for( ; index < _db_column_infos.Count() ; index++ )
			{
				lower_column_name1.set_ByRef( _db_column_infos[ index ]._column_name.ToLower() );
				if( lower_column_name0 == lower_column_name1 )
				{
					db_column_info = _db_column_infos[ index ];
					break;
				}
			}
			if( index >= _db_column_infos.Count() )
				error = "Error : cannot find database column '" + column_name + "' info in table '" + _db_table_name + "'";
			return db_column_info++;
		};
	};
};