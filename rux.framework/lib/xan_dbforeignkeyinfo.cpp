#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_dbforeignkeyinfo.h>
#include <xan_keyvaluepair.h>
#include <xan_uint64.h>
#include <xan_boolean.h>
begin_implement_rux_class_ns( DBForeignKeyInfo , rux::data );
end_implement_rux_class();
namespace rux
{
	namespace data
	{
		XDBForeignKeyInfo::XDBForeignKeyInfo( const rux::XString& parent_db_table_name , const rux::XString& parent_db_column_name , const rux::XString& child_db_column_name )
		{
			(*this)()->_parent_db_table_name = parent_db_table_name;
			(*this)()->_parent_db_column_name = parent_db_column_name;	
			(*this)()->_child_db_column_name = child_db_column_name;	
		};
		rux::String& XDBForeignKeyInfo::get_ParentTableName( void )
		{
			return (*this)()->_parent_db_table_name++;
		};
		rux::String& XDBForeignKeyInfo::get_ChildTableName( void )
		{
			return (*this)()->_child_db_table_name++;
		};
		rux::String& XDBForeignKeyInfo::get_ParentColumnName( void )
		{
			return (*this)()->_parent_db_column_name++;
		};
		rux::String& XDBForeignKeyInfo::get_ChildColumnName( void )
		{
			return (*this)()->_child_db_column_name++;
		};
		void XDBForeignKeyInfo::get_ParentTableName( rux::XString& parent_table_name )
		{
			parent_table_name = (*this)()->_parent_db_table_name;
		};
		void XDBForeignKeyInfo::get_ChildTableName( rux::XString& child_table_name )
		{
			child_table_name = (*this)()->_child_db_table_name;
		};
		void XDBForeignKeyInfo::get_ParentColumnName( rux::XString& parent_column_name )
		{
			parent_column_name = (*this)()->_parent_db_column_name;
		};
		void XDBForeignKeyInfo::get_ChildColumnName( rux::XString& child_column_name )
		{
			child_column_name = (*this)()->_child_db_column_name;
		};
		DBColumnInfo& XDBForeignKeyInfo::get_ParentColumnInfo( void )
		{
			return (*this)()->_parent_db_column_info++;
		};
		DBColumnInfo& XDBForeignKeyInfo::get_ChildColumnInfo( void )
		{
			return (*this)()->_child_db_column_info++;
		};
		void XDBForeignKeyInfo::get_ParentColumnInfo( rux::data::XDBColumnInfo& parent_column_info )
		{
			parent_column_info = (*this)()->_parent_db_column_info;
		};
		void XDBForeignKeyInfo::get_ChildColumnInfo( rux::data::XDBColumnInfo& child_column_info )
		{
			child_column_info = (*this)()->_child_db_column_info;
		};
		rux::data::DBTableInfo& XDBForeignKeyInfo::get_ParentTableInfo( void )
		{
			return (*this)()->_parent_db_table_info++;
		};
		rux::data::DBTableInfo& XDBForeignKeyInfo::get_ChildTableInfo( void )
		{
			return (*this)()->_child_db_table_info++;
		};
	};
};