#ifndef XAN_DBTABLEINFO_H
#define XAN_DBTABLEINFO_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_dbcolumninfo.h>
namespace rux
{	
	namespace data
	{	
		class XSqliteProvider;
		class SqliteProvider;
		class XDBSchema;
		begin_declare_rux_class( DBTableInfo );
			friend class XSqliteProvider;
			friend class SqliteProvider;
			friend class XDBSchema;
			friend rux::int32 sqlite3_fill_table_info_callback( void* ptr , rux::int32 argc , char** values_ptr , char** columns_ptr );
			rux::String& get_TableName( void );
			void get_TableName( rux::XString& table_name );
			rux::Array<rux::data::XDBColumnInfo>& get_ColumnInfos( void );
			rux::Array<rux::data::XDBColumnInfo>& get_PrimaryKeyColumnInfos( void );
			rux::data::DBColumnInfo& get_ColumnInfo( const rux::XString& column_name , rux::XString& error );
			void get_ColumnInfos( rux::XArray<rux::data::XDBColumnInfo>& column_infos );
			void get_PrimaryKeyColumnInfos( rux::XArray<rux::data::XDBColumnInfo>& primary_key_column_infos );
		begin_declare_rux_class_members( DBTableInfo );
			rux::XString _db_table_name;
			rux::XArray<rux::data::XDBColumnInfo> _db_column_infos;
			rux::XArray<rux::data::XDBColumnInfo> _db_primary_key_column_infos;
		end_declare_rux_class_members( DBTableInfo );
			XDBTableInfo( const rux::XString& db_table_name );
			rux::String& get_TableName( void );
			void get_TableName( rux::XString& table_name );
			rux::Array<rux::data::XDBColumnInfo>& get_ColumnInfos( void );
			rux::Array<rux::data::XDBColumnInfo>& get_PrimaryKeyColumnInfos( void );
			rux::data::DBColumnInfo& get_ColumnInfo( const rux::XString& column_name , rux::XString& error );
			void get_ColumnInfos( rux::XArray<rux::data::XDBColumnInfo>& column_infos );
			void get_PrimaryKeyColumnInfos( rux::XArray<rux::data::XDBColumnInfo>& primary_key_column_infos );
		end_declare_rux_class();
	};
};
#endif
