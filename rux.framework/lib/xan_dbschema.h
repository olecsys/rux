#ifndef XAN_DBSCHEMA_H
#define XAN_DBSCHEMA_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_dbtableinfo.h>
#include <xan_dbforeignkeyinfo.h>
namespace rux
{	
	namespace data
	{	
		class XSqliteProvider;
		class SqliteProvider;
		begin_declare_rux_class( DBSchema );
			friend class XSqliteProvider;
			friend class SqliteProvider;
			friend rux::int32 sqlite3_fill_foreign_key_info_callback( void* ptr , rux::int32 argc , char** values_ptr , char** columns_ptr );
			friend rux::int32 sqlite3_fill_schema_callback( void* ptr , rux::int32 argc , char** values_ptr , char** columns_ptr );
		begin_declare_rux_class_members( DBSchema );
			rux::XArray<XDBTableInfo> _db_table_infos;
			rux::XArray<rux::data::XDBForeignKeyInfo> _db_foreign_key_infos;
		end_declare_rux_class_members( DBSchema );
			rux::Array<XDBTableInfo>& get_DBTableInfos( void );
			rux::Array<rux::data::XDBForeignKeyInfo>& get_DBForeignKeyInfos( void );
			rux::Array<rux::data::XDBForeignKeyInfo>& get_DBParentForeignKeyInfos( const rux::XString& parent_table_name );
			rux::Array<rux::data::XDBForeignKeyInfo>& get_DBChildForeignKeyInfos( const rux::XString& child_table_name );
			DBTableInfo& get_DBTableInfo( const rux::XString& table_name , rux::XString& error );
			rux::data::DBForeignKeyInfo& get_DBForeignKeyInfo( const rux::XString& parent_table_name , const rux::XString& child_table_name , rux::XString& error );
			rux::data::DBForeignKeyInfo& get_DBForeignKeyInfoV2( const rux::XString& parent_table_name , const rux::XString& child_column_name , rux::XString& error );
			rux::data::DBForeignKeyInfo& get_DBForeignKeyInfoV3( const rux::XString& parent_column_name , const rux::XString& child_table_name , rux::XString& error );
			void get_DBTableInfos( rux::XArray<XDBTableInfo>& db_table_infos );
			void get_DBForeignKeyInfos( rux::XArray<rux::data::XDBForeignKeyInfo>& db_foreign_key_infos );	
			rux::String& ToJSON( void );
			static DBSchema& FromJSON( const rux::XString& json , rux::XString& error );
		end_declare_rux_class();
	};
};
#endif
