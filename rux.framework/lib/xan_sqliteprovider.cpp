#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_sqliteprovider.h>
#include <xan_console.h>
#include <xan_keyvaluepair.h>
#include <xan_uint64.h>
#include <xan_boolean.h>
begin_implement_rux_class_ns( SqliteProvider , rux::data );
end_implement_rux_class();
namespace rux
{
	namespace data
	{
		SqliteProvider::~SqliteProvider()
		{
			if( _sqlite3_module.IsLoaded() == 1 )
				rux::loader::UnLoadModule( _sqlite3_module );
		};
		XSqliteProvider::XSqliteProvider( const rux::XString& connection )
		{
			(*this)()->_connection = connection.ConvertToUTF8();
		};
		rux::int32 sqlite3_fill_schema_callback( void* ptr , rux::int32 argc , char** values_ptr , char** columns_ptr )
		{
			DBSchema* db_schema_ptr = (DBSchema*)ptr;
			char** values_ptr0 = values_ptr;
			char** columns_ptr0 = columns_ptr;
			rux::XString table_name( "tbl_name" );
			while( argc > 0 )
			{
				if( table_name == (*columns_ptr0) )
					table_name = (*values_ptr0);
				else
					table_name.Clear();		
				values_ptr0++;
				columns_ptr0++;
				argc--;
			}	
			if( table_name.Length() > 0 )
				db_schema_ptr->_db_table_infos.Add( XDBTableInfo( table_name ) );	
			return 0;
		};
		rux::int32 sqlite3_fill_table_info_callback( void* ptr , rux::int32 argc , char** values_ptr , char** columns_ptr )
		{
			DBTableInfo* db_table_info_ptr = (DBTableInfo*)ptr;
			char** values_ptr0 = values_ptr;
			char** columns_ptr0 = columns_ptr;	
			rux::XString field_name;
			rux::XString field_type;
			rux::XString field_default_value;
			uint8 is_not_null = 0;
			uint8 is_pk = 0;
			while( argc > 0 )
			{
				if( rux::XString::IsEqualA( (*columns_ptr0) , "name" ) == 1 )
					field_name = (*values_ptr0);
				else if( rux::XString::IsEqualA( (*columns_ptr0) , "type" ) == 1 )
					field_type = (*values_ptr0);
				else if( rux::XString::IsEqualA( (*columns_ptr0) , "notnull" ) == 1 )
					is_not_null = rux::XString::IsEqualA( (*values_ptr0) , "1" ) == 1 ? 1 : 0;
				else if( rux::XString::IsEqualA( (*columns_ptr0) , "dflt_value" ) == 1 )
					field_default_value = (*values_ptr0);
				else if( rux::XString::IsEqualA( (*columns_ptr0) , "pk" ) == 1 )
					is_pk = rux::XString::IsEqualA( (*values_ptr0) , "1" ) == 1 ? 1 : 0;
				values_ptr0++;
				columns_ptr0++;
				argc--;
			}
			if( field_name.Length() > 0 
				&& field_type.Length() > 0 )
			{
				rux::data::XDBColumnInfo db_column_info = XDBColumnInfo( field_name , field_type , field_default_value , is_not_null , is_pk );
				db_table_info_ptr->_db_column_infos.Add( db_column_info );
				if( is_pk == 1 )
					db_table_info_ptr->_db_primary_key_column_infos.Add( db_column_info );
			}
			return 0;
		};
		rux::int32 sqlite3_fill_foreign_key_info_callback( void* ptr , rux::int32 argc , char** values_ptr , char** columns_ptr )
		{
			DBSchema* db_schema_ptr = (DBSchema*)ptr;
			char** values_ptr0 = values_ptr;
			char** columns_ptr0 = columns_ptr;	
			rux::XString parent_db_table_name;
			rux::XString parent_db_column_name;
			rux::XString child_db_column_name;	
			while( argc > 0 )
			{
				if( rux::XString::IsEqualA( (*columns_ptr0) , "from" ) == 1 )
					child_db_column_name = (*values_ptr0);
				else if( rux::XString::IsEqualA( (*columns_ptr0) , "table" ) == 1 )
					parent_db_table_name = (*values_ptr0);
				else if( rux::XString::IsEqualA( (*columns_ptr0) , "to" ) == 1 )
					parent_db_column_name = (*values_ptr0);
				values_ptr0++;
				columns_ptr0++;
				argc--;
			}	
			if( child_db_column_name.Length() > 0 
				&& parent_db_table_name.Length() > 0
				&& parent_db_column_name.Length() > 0 )
				db_schema_ptr->_db_foreign_key_infos.Add( XDBForeignKeyInfo( parent_db_table_name , parent_db_column_name , child_db_column_name  ) );
			return 0;
		};
		implement_duplicate_internal_result_function_1( DBSchema& , SqliteProvider , get_Schema , rux::XString& );
		DBSchema& SqliteProvider::get_Schema( rux::XString& error )
		{
			error.Clear();
			XDBSchema db_schema;
			rux::XString sql( "select tbl_name from sqlite_master where type = 'table'" );
			Execute( sql , sqlite3_fill_schema_callback , db_schema() , error );	
			if( error.Length() == 0 )
			{
				if( db_schema()->_db_table_infos.Count() > 0 )
				{
					size_t index0 = 0;
					size_t index1 = 0;
					size_t index2 = 0;
					for( index0 = 0 ; index0 < db_schema()->_db_table_infos.Count() ; index0++ )
					{
						sql = "pragma table_info(" + db_schema()->_db_table_infos[ index0 ]._db_table_name + ");";
						Execute( sql , sqlite3_fill_table_info_callback , &db_schema()->_db_table_infos[ index0 ] , error );
						if( error.Length() == 0 )
						{
							if( db_schema()->_db_table_infos[ index0 ]._db_column_infos.Count() > 0 )
							{
								sql = "pragma foreign_key_list(" + db_schema()->_db_table_infos[ index0 ]._db_table_name + ");";
								Execute( sql , sqlite3_fill_foreign_key_info_callback , db_schema() , error );
								if( error.Length() == 0 )
								{
									if( db_schema()->_db_foreign_key_infos.Count() > 0 )
									{
										index1 = db_schema()->_db_foreign_key_infos.Count() - 1;
										for( ; ; )
										{
											if( db_schema()->_db_foreign_key_infos[ index1 ]._child_db_table_name.Length() > 0 )
												break;
											else
												db_schema()->_db_foreign_key_infos[ index1 ]._child_db_table_name = db_schema()->_db_table_infos[ index0 ]._db_table_name;						
											if( index1 == 0 )
												break;
											index1--;
										}
									}
								}
								else
									break;
							}
						}
						else
							break;
					}
					if( error.Length() == 0 )
					{
						rux::XString local_string0 , local_string1;
						for( index0 = 0 ; index0 < db_schema()->_db_foreign_key_infos.Count() ; index0++ )
						{
							for( index1 = 0 ; index1 < db_schema()->_db_table_infos.Count() ; index1++ )
							{
								local_string0.set_ByRef( db_schema()->_db_foreign_key_infos[ index0 ]._child_db_table_name.ToLower() );
								local_string1.set_ByRef( db_schema()->_db_table_infos[ index1 ]._db_table_name.ToLower() );
								if( local_string0 == local_string1 )
								{
									for( index2 = 0 ; index2 < db_schema()->_db_table_infos[ index1 ]._db_column_infos.Count() ; index2++ )
									{
										local_string0.set_ByRef( db_schema()->_db_foreign_key_infos[ index0 ]._child_db_column_name.ToLower() );
										local_string1.set_ByRef( db_schema()->_db_table_infos[ index1 ]._db_column_infos[ index2 ]._column_name.ToLower() );
										if( local_string0 == local_string1 )
										{
											db_schema()->_db_foreign_key_infos[ index0 ]._child_db_column_info = db_schema()->_db_table_infos[ index1 ]._db_column_infos[ index2 ];
											db_schema()->_db_foreign_key_infos[ index0 ]._child_db_table_info = db_schema()->_db_table_infos[ index1 ];
											break;
										}
									}
									break;
								}
							}
							for( index1 = 0 ; index1 < db_schema()->_db_table_infos.Count() ; index1++ )
							{
								local_string0.set_ByRef( db_schema()->_db_foreign_key_infos[ index0 ]._parent_db_table_name.ToLower() );
								local_string1.set_ByRef( db_schema()->_db_table_infos[ index1 ]._db_table_name.ToLower() );
								if( local_string0 == local_string1 )
								{
									for( index2 = 0 ; index2 < db_schema()->_db_table_infos[ index1 ]._db_column_infos.Count() ; index2++ )
									{
										local_string0.set_ByRef( db_schema()->_db_foreign_key_infos[ index0 ]._parent_db_column_name.ToLower() );
										local_string1.set_ByRef( db_schema()->_db_table_infos[ index1 ]._db_column_infos[ index2 ]._column_name.ToLower() );
										if( local_string0 == local_string1 )
										{
											db_schema()->_db_foreign_key_infos[ index0 ]._parent_db_column_info = db_schema()->_db_table_infos[ index1 ]._db_column_infos[ index2 ];
											db_schema()->_db_foreign_key_infos[ index0 ]._parent_db_table_info = db_schema()->_db_table_infos[ index1 ];
											break;
										}
									}
									break;
								}
							}
						}
						rux::XString local_string2 , local_string3;
						for( index0 = 0 ; index0 < db_schema()->_db_table_infos.Count() ; index0++ )
						{
							for( index1 = 0 ; index1 < db_schema()->_db_table_infos[ index0 ]._db_column_infos.Count() ; index1++ )
							{
								if( db_schema()->_db_table_infos[ index0 ]._db_column_infos[ index1 ]._is_primary_key == 1 )
								{
									if( db_schema()->_db_table_infos[ index0 ]._db_column_infos[ index1 ]._column_type == "integer" )
									{
										for( index2 = 0 ; index2 < db_schema()->_db_foreign_key_infos.Count() ; index2++ )
										{
											local_string0.set_ByRef( db_schema()->_db_foreign_key_infos[ index2 ]._child_db_table_name.ToLower() );
											local_string1.set_ByRef( db_schema()->_db_table_infos[ index0 ]._db_table_name.ToLower() );
											local_string2.set_ByRef( db_schema()->_db_foreign_key_infos[ index2 ]._child_db_column_name.ToLower() );
											local_string3.set_ByRef( db_schema()->_db_table_infos[ index0 ]._db_column_infos[ index1 ]._column_name.ToLower() );
											if( local_string0 == local_string1 && local_string2 == local_string3 )
											{
												db_schema()->_db_table_infos[ index0 ]._db_column_infos[ index1 ]._is_auto_increment = 0;
												break;
											}
										}
										if( index2 >= db_schema()->_db_foreign_key_infos.Count() )
											db_schema()->_db_table_infos[ index0 ]._db_column_infos[ index1 ]._is_auto_increment = 1;
									}
								}
							}
						}
					}
				}
			}
			return db_schema++;
		};
		int on_sqlite3_busy( void* , int )
		{	
			return 1;
		};
		implement_duplicate_internal_function_4( SqliteProvider , Execute , const rux::XString& , sqlite3_callback_t , void* , rux::XString& );
		void SqliteProvider::Execute( const rux::XString& sql , sqlite3_callback_t sqlite3_callback , void* user_data , rux::XString& error )
		{
			error.Clear();
			if( _connection.Length() > 0 )
			{
				if( _sqlite3_module.IsLoaded() == 0 )
					_sqlite3_module = rux::loader::LoadModule( "sqlite3" , NULL , error );
				if( _sqlite3_module.IsLoaded() == 1 )
				{			
					sqlite3* sqlite3_ptr = NULL;
					rux::int32 database_flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_NOMUTEX;			
					sqlite3_open_v2_t sqlite3_open_v2 = (sqlite3_open_v2_t)_sqlite3_module.get_MethodAddress( "sqlite3_open_v2" , error );
					if( sqlite3_open_v2 == NULL || sqlite3_open_v2( _connection.str() , &sqlite3_ptr , database_flags , NULL ) )
					{
						XConsole::Print( stderr , XRed , "%s" , "Error : cannot open database\n" );
						error = "Error : cannot open database '" + _connection + "'";
					}
					else
					{
						sqlite3_busy_handler_t sqlite3_busy_handler_ptr = (sqlite3_busy_handler_t)_sqlite3_module.get_MethodAddress( "sqlite3_busy_handler" , error );
						sqlite3_busy_handler_ptr( sqlite3_ptr , on_sqlite3_busy , NULL );
						char* error_ptr = NULL;				
						sqlite3_exec_t sqlite3_exec = (sqlite3_exec_t)_sqlite3_module.get_MethodAddress( "sqlite3_exec" , error );
						if( sqlite3_exec && sqlite3_exec( sqlite3_ptr , "PRAGMA FOREIGN_KEYS=ON;" , NULL , NULL , &error_ptr ) == 0 )
						{					
							if( sqlite3_exec( sqlite3_ptr , "PRAGMA JOURNAL_MODE=WAL;" , NULL , NULL , &error_ptr ) == 0 )
							{						
								if( sqlite3_exec( sqlite3_ptr , sql.str() , sqlite3_callback , user_data , &error_ptr ) != 0 )
								{
									if( error_ptr )
									{
										XConsole::Print( stderr , XRed , "Error : sqlite3_exec, %s" , error_ptr );
										error = "Error : sqlite3_exec , ";
										error += error_ptr;
										sqlite3_free_t sqlite3_free = (sqlite3_free_t)_sqlite3_module.get_MethodAddress( "sqlite3_free" , error );
										if( sqlite3_free )
											sqlite3_free( error_ptr );
									}
								}						
							}
							else
							{
								if( error_ptr )
								{
									XConsole::Print( stderr , XRed , "Error : sqlite3_exec, %s" , error_ptr );
									error = "Error : sqlite3_exec, ";
									error += error_ptr;
									sqlite3_free_t sqlite3_free = (sqlite3_free_t)_sqlite3_module.get_MethodAddress( "sqlite3_free" , error );
									if( sqlite3_free )
										sqlite3_free( error_ptr );
								}
							}
						}
						else
						{
							if( error_ptr )
							{
								XConsole::Print( stderr , XRed , "Error : sqlite3_exec, %s" , error_ptr );
								error = "Error : sqlite3_exec , ";
								error += error_ptr;
								sqlite3_free_t sqlite3_free = (sqlite3_free_t)_sqlite3_module.get_MethodAddress( "sqlite3_free" , error );
								if( sqlite3_free )
									sqlite3_free( error_ptr );
							}
						}
						sqlite3_close_t sqlite3_close = (sqlite3_close_t)_sqlite3_module.get_MethodAddress( "sqlite3_close" , error );
						if( sqlite3_close )
							sqlite3_close( sqlite3_ptr );
					}			
				}
			}
		};		
		implement_duplicate_internal_function_1( SqliteProvider , CreateDatabase , rux::XString& );
		void SqliteProvider::CreateDatabase( rux::XString& error )
		{
			error = "";
			if( _connection.Length() > 0 )
			{
				if( _sqlite3_module.IsLoaded() == 0 )
					_sqlite3_module = rux::loader::LoadModule( "sqlite3" , NULL , error );
				if( _sqlite3_module.IsLoaded() == 1 )
				{			
					sqlite3* sqlite3_ptr = NULL;
					rux::int32 database_flags = SQLITE_OPEN_READWRITE|SQLITE_OPEN_NOMUTEX|SQLITE_OPEN_CREATE;
					sqlite3_open_v2_t sqlite3_open_v2 = (sqlite3_open_v2_t)_sqlite3_module.get_MethodAddress( "sqlite3_open_v2" , error );
					if( sqlite3_open_v2 == NULL || sqlite3_open_v2( _connection.str() , &sqlite3_ptr , database_flags , NULL ) )
					{
						XConsole::Print( stderr , XRed , "%s" , "Error : cannot create database\n" );
						error = "Error : cannot create database '" + _connection + "'";
					}
					else
					{
						sqlite3_close_t sqlite3_close = (sqlite3_close_t)_sqlite3_module.get_MethodAddress( "sqlite3_close" , error );
						if( sqlite3_close )
							sqlite3_close( sqlite3_ptr );
					}
				}
			}
		};
		void XSqliteProvider::ExecuteNonQuery( const rux::XString& sql , rux::XString& error )
		{
			rux::XString utf8_sql( sql.ConvertToUTF8() );
			Execute( utf8_sql , NULL , NULL , error );
		};
	};
};