#ifndef XAN_FOREIGNKEYINFO_H
#define XAN_FOREIGNKEYINFO_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_dbtableinfo.h>
namespace rux
{	
	namespace data
	{	
		class XDBForeignKeyInfo;
		class XSqliteProvider;
		class SqliteProvider;
		class XDBSchema;
		begin_declare_rux_class( DBForeignKeyInfo );
			friend class XSqliteProvider;
			friend class SqliteProvider;
			friend class XDBSchema;
		begin_declare_rux_class_members( DBForeignKeyInfo );
			rux::XString _parent_db_table_name;
			rux::XString _child_db_table_name;
			rux::XString _parent_db_column_name;
			rux::XString _child_db_column_name;
			rux::data::XDBColumnInfo _parent_db_column_info;
			rux::data::XDBColumnInfo _child_db_column_info;
			rux::data::XDBTableInfo _parent_db_table_info;
			rux::data::XDBTableInfo _child_db_table_info;
		end_declare_rux_class_members( DBForeignKeyInfo );
			XDBForeignKeyInfo( const rux::XString& parent_db_table_name , const rux::XString& parent_db_column_name , const rux::XString& child_db_column_name );
			rux::String& get_ParentTableName( void );
			rux::String& get_ChildTableName( void );
			rux::String& get_ParentColumnName( void );
			rux::String& get_ChildColumnName( void );
			DBTableInfo& get_ParentTableInfo( void );
			DBTableInfo& get_ChildTableInfo( void );
			void get_ParentTableName( rux::XString& parent_table_name );
			void get_ChildTableName( rux::XString& child_table_name );
			void get_ParentColumnName( rux::XString& parent_column_name );
			void get_ChildColumnName( rux::XString& child_column_name );
			rux::data::DBColumnInfo& get_ParentColumnInfo( void );
			rux::data::DBColumnInfo& get_ChildColumnInfo( void );	
			void get_ParentColumnInfo( rux::data::XDBColumnInfo& parent_column_info );
			void get_ChildColumnInfo( rux::data::XDBColumnInfo& child_column_info );	
		end_declare_rux_class();
	};
};
#endif
