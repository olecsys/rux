#ifndef XAN_DBFIELDINFO_H
#define XAN_DBFIELDINFO_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_string.h>
namespace rux
{	
	namespace data
	{	
		class XSqliteProvider;
		class SqliteProvider;
		class DBTableInfo;
		class XDBTableInfo;
		class XDBSchema;
		begin_declare_rux_class( DBColumnInfo );
			friend class XSqliteProvider;
			friend class SqliteProvider;
			friend class DBTableInfo;
			friend class XDBTableInfo;
			friend class XDBSchema;
			uint8 get_IsPrimaryKey( void );
			rux::String& get_ColumnName( void );
			rux::String& get_ColumnType( void );
			rux::String& get_DefaultValue( void );
			void get_ColumnName( rux::XString& column_name );
			void get_ColumnType( rux::XString& column_type );
			void get_DefaultValue( rux::XString& column_default_value );
			uint8 get_IsNotNull( void );
			uint8 get_IsAutoIncrement( void );
		begin_declare_rux_class_members( DBColumnInfo );
			rux::XString _column_name;
			rux::XString _column_type;
			rux::XString _default_value;
			uint8 _is_primary_key;
			uint8 _is_not_null;
			uint8 _is_auto_increment;
		end_declare_rux_class_members( DBColumnInfo );
			XDBColumnInfo( const rux::XString& column_name , const rux::XString& column_type , const rux::XString& default_value , uint8 is_not_null , uint8 is_primary_key );
			rux::String& get_ColumnName( void );
			rux::String& get_ColumnType( void );
			rux::String& get_DefaultValue( void );
			void get_ColumnName( rux::XString& column_name );
			void get_ColumnType( rux::XString& column_type );
			void get_DefaultValue( rux::XString& column_default_value );
			uint8 get_IsPrimaryKey( void );
			uint8 get_IsNotNull( void );
			uint8 get_IsAutoIncrement( void );
		end_declare_rux_class();
	};
};
#endif
