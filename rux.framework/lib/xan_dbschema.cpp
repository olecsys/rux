#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_dbschema.h>
#include <xan_jsonserializer.h>
#include <xan_keyvaluepair.h>
begin_implement_rux_class_ns( DBSchema , rux::data );
end_implement_rux_class();
namespace rux
{
	namespace data
	{
		rux::Array<XDBTableInfo>& XDBSchema::get_DBTableInfos( void )
		{
			return (*this)()->_db_table_infos++;
		};
		rux::Array<rux::data::XDBForeignKeyInfo>& XDBSchema::get_DBForeignKeyInfos( void )
		{
			return (*this)()->_db_foreign_key_infos++;
		};
		rux::Array<rux::data::XDBForeignKeyInfo>& XDBSchema::get_DBParentForeignKeyInfos( const rux::XString& parent_table_name )
		{
			rux::XArray<rux::data::XDBForeignKeyInfo> parent_foreign_key_infos;
			rux::XString lower_parent_table0 , lower_parent_table1;
			lower_parent_table0.set_ByRef( parent_table_name.ToLower() );	
			for( uint32 index1 = 0 ; index1 < (*this)()->_db_foreign_key_infos.Count() ; index1++ )
			{
				lower_parent_table1.set_ByRef( (*this)()->_db_foreign_key_infos[ index1 ]._parent_db_table_name.ToLower() );
				if( lower_parent_table0 == lower_parent_table1 )
				{
					parent_foreign_key_infos.Add( (*this)()->_db_foreign_key_infos[ index1 ] );
				}
			}
			return parent_foreign_key_infos++;
		};
		rux::Array<rux::data::XDBForeignKeyInfo>& XDBSchema::get_DBChildForeignKeyInfos( const rux::XString& child_table_name )
		{
			rux::XArray<rux::data::XDBForeignKeyInfo> child_foreign_key_infos;
			rux::XString lower_child_table_name0 , lower_child_table_name1;
			lower_child_table_name0.set_ByRef( child_table_name.ToLower() );
			for( uint32 index1 = 0 ; index1 < (*this)()->_db_foreign_key_infos.Count() ; index1++ )
			{
				lower_child_table_name1.set_ByRef( (*this)()->_db_foreign_key_infos[ index1 ]._child_db_table_name.ToLower() );
				if( lower_child_table_name1 == lower_child_table_name0 )
				{
					child_foreign_key_infos.Add( (*this)()->_db_foreign_key_infos[ index1 ] );
				}
			}
			return child_foreign_key_infos++;
		};
		void XDBSchema::get_DBTableInfos( rux::XArray<XDBTableInfo>& db_table_infos )
		{
			db_table_infos = (*this)()->_db_table_infos;
		};
		void XDBSchema::get_DBForeignKeyInfos( rux::XArray<rux::data::XDBForeignKeyInfo>& db_foreign_key_infos )
		{
			db_foreign_key_infos = (*this)()->_db_foreign_key_infos;
		};
		DBTableInfo& XDBSchema::get_DBTableInfo( const rux::XString& table_name , rux::XString& error )
		{
			error.Clear();
			rux::data::XDBTableInfo db_table_info;
			uint32 index = 0;
			rux::XString lower_table_name0 , lower_table_name1;
			lower_table_name0.set_ByRef( table_name.ToLower() );
			for( index ; index < (*this)()->_db_table_infos.Count() ; index++ )
			{
				lower_table_name1.set_ByRef( (*this)()->_db_table_infos[ index ]._db_table_name.ToLower() );
				if( lower_table_name0 == lower_table_name1 )
				{
					db_table_info = (*this)()->_db_table_infos[ index ];
					break;
				}
			}
			if( index >= (*this)()->_db_table_infos.Count() )
				error = "Error : cannot find database table '" + table_name + "' info";
			return db_table_info++;
		};
		DBForeignKeyInfo& XDBSchema::get_DBForeignKeyInfo( const rux::XString& parent_table_name , const rux::XString& child_table_name , rux::XString& error )
		{
			error.Clear();
			XDBForeignKeyInfo db_foreign_key_info;
			uint32 index = 0;
			rux::XString lower_parent_table_name0 , lower_parent_table_name1 , lower_child_table_name0 , lower_child_table_name1;
			lower_child_table_name0.set_ByRef( child_table_name.ToLower() );
			lower_parent_table_name0.set_ByRef( parent_table_name.ToLower() );
			for( index ; index < (*this)()->_db_foreign_key_infos.Count() ; index++ )
			{
				lower_child_table_name1.set_ByRef( (*this)()->_db_foreign_key_infos[ index ]._child_db_table_name.ToLower() );
				lower_parent_table_name1.set_ByRef( (*this)()->_db_foreign_key_infos[ index ]._parent_db_table_name.ToLower() );
				if( lower_parent_table_name1 == lower_parent_table_name0
					&& lower_child_table_name1 == lower_child_table_name0 )
				{
					db_foreign_key_info = (*this)()->_db_foreign_key_infos[ index ];
					break;
				}
			}
			if( index >= (*this)()->_db_foreign_key_infos.Count() )
				error = "Error : cannot find database relationship info between table '" + parent_table_name + "' and table '" + child_table_name + "'";
			return db_foreign_key_info++;
		};
		DBForeignKeyInfo& XDBSchema::get_DBForeignKeyInfoV2( const rux::XString& parent_table_name , const rux::XString& child_column_name , rux::XString& error )
		{
			error.Clear();
			XDBForeignKeyInfo db_foreign_key_info;
			uint32 index = 0;
			rux::XString lower_parent_table_name0 , lower_parent_table_name1 , lower_child_column_name0 , lower_child_column_name1;
			lower_parent_table_name0.set_ByRef( parent_table_name.ToLower() );
			lower_child_column_name0.set_ByRef( child_column_name.ToLower() );
			for( index ; index < (*this)()->_db_foreign_key_infos.Count() ; index++ )
			{
				lower_parent_table_name1.set_ByRef( (*this)()->_db_foreign_key_infos[ index ]._parent_db_table_name.ToLower() );
				lower_child_column_name1.set_ByRef( (*this)()->_db_foreign_key_infos[ index ]._child_db_column_name.ToLower() );
				if( lower_parent_table_name1 == lower_parent_table_name0
					&& lower_child_column_name1 == lower_child_column_name0 )
				{
					db_foreign_key_info = (*this)()->_db_foreign_key_infos[ index ];
					break;
				}
			}
			if( index >= (*this)()->_db_foreign_key_infos.Count() )
				error = "Error : cannot find database relationship info between parent table '" + parent_table_name + "' and child table with column'" + child_column_name + "'";
			return db_foreign_key_info++;
		};
		DBForeignKeyInfo& XDBSchema::get_DBForeignKeyInfoV3( const rux::XString& parent_column_name , const rux::XString& child_table_name , rux::XString& error )
		{
			error.Clear();
			XDBForeignKeyInfo db_foreign_key_info;
			uint32 index = 0;
			rux::XString lower_parent_column_name0 , lower_parent_column_name1 , lower_child_table_name0 , lower_child_table_name1;
			lower_parent_column_name0.set_ByRef( parent_column_name.ToLower() );
			lower_child_table_name0.set_ByRef( child_table_name.ToLower() );
			for( index ; index < (*this)()->_db_foreign_key_infos.Count() ; index++ )
			{
				lower_parent_column_name1.set_ByRef( (*this)()->_db_foreign_key_infos[ index ]._parent_db_column_name.ToLower() );
				lower_child_table_name1.set_ByRef( (*this)()->_db_foreign_key_infos[ index ]._child_db_table_name.ToLower() );
				if( lower_parent_column_name1 == lower_parent_column_name0
					&& lower_child_table_name1 == lower_child_table_name0 )
				{
					db_foreign_key_info = (*this)()->_db_foreign_key_infos[ index ];
					break;
				}
			}
			if( index >= (*this)()->_db_foreign_key_infos.Count() )
				error = "Error : cannot find database relationship info between parent table column '" + parent_column_name + "' and child table '" + child_table_name + "'";
			return db_foreign_key_info++;
		};
		rux::String& XDBSchema::ToJSON( void )
		{
			rux::XString json( 512 );
			json = "{\"tables\":[";
			uint32 index0 = 0 , index1 = 0;
			for( ; index0 < (*this)()->_db_table_infos.Count() ; index0++ )
			{
				if( index0 > 0 )
					json += ",";
				json += "{\"name\":\"" + (*this)()->_db_table_infos[ index0 ]._db_table_name + "\",\"columns\":[";
				for( index1 = 0 ; index1 < (*this)()->_db_table_infos[ index0 ]._db_column_infos.Count() ; index1++ )
				{
					if( index1 > 0 )
						json += ",";
					json += "{\"name\":\"" + (*this)()->_db_table_infos[ index0 ]._db_column_infos[ index1 ]._column_name +
						"\",\"type\":\"" + (*this)()->_db_table_infos[ index0 ]._db_column_infos[ index1 ]._column_type +
						"\",\"dflt_value\":\"" + (*this)()->_db_table_infos[ index0 ]._db_column_infos[ index1 ]._default_value +
						"\",\"is_pk\":";
					if( (*this)()->_db_table_infos[ index0 ]._db_column_infos[ index1 ]._is_primary_key == 1 )
						json += "true,\"is_null\":";
					else
						json += "false,\"is_null\":";
					if( (*this)()->_db_table_infos[ index0 ]._db_column_infos[ index1 ]._is_not_null == 1 )
						json += "false,\"is_auto_inc\":";
					else
						json += "true,\"is_auto_inc\":";
					if( (*this)()->_db_table_infos[ index0 ]._db_column_infos[ index1 ]._is_auto_increment == 1 )
						json += "true}";
					else
						json += "false}";
				}
				json += "]}";
			}
			json += "],\"foreign_keys\":[";
			for( index0 = 0 ; index0 < (*this)()->_db_foreign_key_infos.Count() ; index0++ )
			{
				if( index0 > 0 )
					json += ",";
				json += "{\"p_table\":\"" + (*this)()->_db_foreign_key_infos[ index0 ]._parent_db_table_name + "\",\"c_table\":\"" +
					(*this)()->_db_foreign_key_infos[ index0 ]._child_db_table_name + "\",\"p_col\":\"" + 
					(*this)()->_db_foreign_key_infos[ index0 ]._parent_db_column_name + "\",\"c_col\":\"" + 
					(*this)()->_db_foreign_key_infos[ index0 ]._child_db_column_name + "\"}";
			}
			json += "]}";
			return json++;
		};
		rux::data::DBSchema& XDBSchema::FromJSON( const rux::XString& json , rux::XString& error )
		{
			XDBSchema db_schema;	
			XObject json_object( rux::data::XJsonSerializer::Deserialize( json , error , __FILE__ , __LINE__ ) );
			if( rux_is_object( json_object , XDataObject ) )
			{
				declare_variable_param( rux::data::XDataObject , data_object0 , json_object );
				declare_variable( ::rux::data::XDataObject , data_object1 );
				declare_variable( ::rux::data::XDataObject , data_object2 );
				if( error.Length() == 0 )
				{
					rux::XString string0 , string1;
					XObject value0;
					rux::XArray< XObject > values0 , values1;
					uint32 index1 = 0 , index2 = 0 , index3 = 0 , index4 = 0;
					rux::data::XDBTableInfo db_table_info;
					rux::data::XDBColumnInfo db_column_info;
					XDBForeignKeyInfo db_foreign_key_info;
					for( uint32 index0 = 0 ; index0 < data_object0.FieldsCount() ; index0++ )
					{
						string0.set_ByRef( data_object0.get_FieldName( index0 ) );
						if( string0 == "tables" )
						{
							value0 = data_object0.get_FieldValue( index0 );
							if( rux_is_object( value0 , rux::XArray<XObject> ) )
							{
								values0.set_ByRef( value0 );
								for( index1 = 0 ; index1 < values0.Count() ; index1++ )
								{
									value0 = values0[ index1 ];
									if( rux_is_object( value0 , XDataObject ) )
									{
										db_table_info = rux::data::XDBTableInfo();
										data_object1 = value0;
										for( index2 = 0 ; index2 < data_object1.FieldsCount() ; index2++ )
										{
											string0.set_ByRef( data_object1.get_FieldName( index2 ) );
											if( string0 == "name" )
											{
												value0 = data_object1.get_FieldValue( index2 );
												if( rux_is_object( value0 , rux::XString ) )
													db_table_info()->_db_table_name.set_ByRef( value0 );
												else
												{
													error = g_current_module_name;
													error += " module,incorrect format of the JSON string for the database schema";
													break;
												}
											}
											else if( string0 == "columns" )
											{
												value0 = data_object1.get_FieldValue( index2 );
												if( rux_is_object( value0 , rux::XArray<XObject> ) )
												{
													values1.set_ByRef( value0 );
													for( index3 = 0 ; index3 < values1.Count() ; index3++ )
													{
														value0 = values1[ index3 ];
														if( rux_is_object( value0 , XDataObject ) )
														{
															data_object2 = value0;
															db_column_info = XDBColumnInfo();
															for( index4 = 0 ; index4 < data_object2.FieldsCount() ; index4++ )
															{
																string0.set_ByRef( data_object2.get_FieldName( index4 ) );
																value0 = data_object2.get_FieldValue( index4 );
																if( rux_is_object( value0 , rux::XString ) )
																{
																	if( string0 == "name" )
																		db_column_info()->_column_name.set_ByRef( value0 );
																	else if( string0 == "type" )
																		db_column_info()->_column_type.set_ByRef( value0 );
																	else if( string0 == "dflt_value" )
																		db_column_info()->_default_value.set_ByRef( value0 );
																	else if( string0 == "is_pk" )															
																	{
																		string0.set_ByRef( value0 );
																		if( string0 == "true" )
																			db_column_info()->_is_primary_key = 1;
																		else 
																			db_column_info()->_is_primary_key = 0;
																	}
																	else if( string0 == "is_null" )
																	{
																		string0.set_ByRef( value0 );
																		if( string0 == "true" )
																			db_column_info()->_is_not_null = 0;
																		else 
																			db_column_info()->_is_not_null = 1;
																	}
																	else if( string0 == "is_auto_inc" )
																	{
																		string0.set_ByRef( value0 );
																		if( string0 == "true" )
																			db_column_info()->_is_auto_increment = 1;
																		else 
																			db_column_info()->_is_auto_increment = 0;
																	}
																	else
																	{
																		error = g_current_module_name;
																		error += " module,incorrect format of the JSON string for the database schema";
																		break;
																	}
																}
																else
																{
																	error = g_current_module_name;
																	error += " module,incorrect format of the JSON string for the database schema";
																	break;
																}
															}
															if( error.Length() > 0 )
																break;
															if( db_column_info()->_is_primary_key == 1 )
																db_table_info()->_db_primary_key_column_infos.Add( db_column_info );
															db_table_info()->_db_column_infos.Add( db_column_info );
														}
														else
														{
															error = g_current_module_name;
															error += " module,incorrect format of the JSON string for the database schema";
															break;
														}
													}
													if( error.Length() > 0 )
														break;
												}
												else
												{
													error = g_current_module_name;
													error += " module,incorrect format of the JSON string for the database schema";
													break;
												}
											}
											else
											{
												error = g_current_module_name;
												error += " module,incorrect format of the JSON string for the database schema";
												break;
											}
										}
										if( error.Length() > 0 )
											break;
										db_schema()->_db_table_infos.Add( db_table_info );
									}
									else
									{
										error = g_current_module_name;
										error += " module,incorrect format of the JSON string for the database schema";
										break;
									}
								}
								if( error.Length() > 0 )
									break;
							}
							else
							{
								error = g_current_module_name;
								error += " module,incorrect format of the JSON string for the database schema";
								break;
							}
						}
						else if( string0 == "foreign_keys" )
						{
							value0 = data_object0.get_FieldValue( index0 );
							if( rux_is_object( value0 , rux::XArray<XObject> ) )
							{
								values0.set_ByRef( value0 );
								for( index1 = 0 ; index1 < values0.Count() ; index1++ )
								{
									value0 = values0[ index1 ];
									if( rux_is_object( value0 , XDataObject ) )
									{
										db_foreign_key_info = XDBForeignKeyInfo();
										data_object1 = value0;
										for( index2 = 0 ; index2 < data_object1.FieldsCount() ; index2++ )
										{
											string0.set_ByRef( data_object1.get_FieldName( index2 ) );
											value0 = data_object1.get_FieldValue( index2 );
											if( rux_is_object( value0 , rux::XString ) )
											{
												if( string0 == "p_table" )
												{									
													db_foreign_key_info()->_parent_db_table_name.set_ByRef( value0 );
												}								
												else if( string0 == "c_table" )
												{
													value0 = data_object1.get_FieldValue( index2 );
													db_foreign_key_info()->_child_db_table_name.set_ByRef( value0 );
												}								
												else if( string0 == "p_col" )
												{
													value0 = data_object1.get_FieldValue( index2 );
													db_foreign_key_info()->_parent_db_column_name.set_ByRef( value0 );
												}								
												else if( string0 == "c_col" )
												{
													value0 = data_object1.get_FieldValue( index2 );
													db_foreign_key_info()->_child_db_column_name.set_ByRef( value0 );
												}								
												else
												{
													error = g_current_module_name;
													error += " module,incorrect format of the JSON string for the database schema";
													break;
												}
											}
											else
											{
												error = g_current_module_name;
												error += " module,incorrect format of the JSON string for the database schema";
												break;
											}
										}
										if( error.Length() > 0 )
											break;
										db_schema()->_db_foreign_key_infos.Add( db_foreign_key_info );
									}
									else
									{
										error = g_current_module_name;
										error += " module,incorrect format of the JSON string for the database schema";
										break;
									}
								}
								if( error.Length() > 0 )
									break;
							}
							else
							{
								error = g_current_module_name;
								error += " module,incorrect format of the JSON string for the database schema";
								break;
							}
						}
					}
					if( error.Length() == 0 )
					{
						for( index1 = 0 ; index1 < db_schema()->_db_foreign_key_infos.Count() ; index1++ )
						{
							string0.set_ByRef( db_schema()->_db_foreign_key_infos[ index1 ]._parent_db_table_name.ToLower() );
							string1.set_ByRef( db_schema()->_db_foreign_key_infos[ index1 ]._child_db_table_name.ToLower() );
							index3 = 0;
							index4 = 0;
							rux::XString lower_db_table_name0;
							for( index2 = 0 ; index2 < db_schema()->_db_table_infos.Count() ; index2++ )
							{
								lower_db_table_name0.set_ByRef( db_schema()->_db_table_infos[ index2 ]._db_table_name.ToLower() );
								if( lower_db_table_name0 == string0 )
								{
									db_schema()->_db_foreign_key_infos[ index1 ]._parent_db_table_info = db_schema()->_db_table_infos[ index2 ];
									index3 = 1;
									if( index4 == 1 )
										break;						
								}
								else if( lower_db_table_name0 == string1 )
								{
									db_schema()->_db_foreign_key_infos[ index1 ]._child_db_table_info = db_schema()->_db_table_infos[ index2 ];
									index4 = 1;
									if( index3 == 1 )
										break;						
								}
							}
							if( index3 == 0 || index4 == 0 )
							{
								error = g_current_module_name;
								error += " module,incorrect format of the JSON string for the database schema";
								break;
							}
							string0.set_ByRef( db_schema()->_db_foreign_key_infos[ index1 ]._child_db_column_name.ToLower() );
							for( index2 = 0 ; index2 < db_schema()->_db_foreign_key_infos[ index1 ]._child_db_table_info()->_db_column_infos.Count() ; index2++ )
							{
								lower_db_table_name0.set_ByRef( db_schema()->_db_foreign_key_infos[ index1 ]._child_db_table_info()->_db_column_infos[ index2 ]._column_name.ToLower() );
								if( lower_db_table_name0 == string0 )
								{
									db_schema()->_db_foreign_key_infos[ index1 ]._child_db_column_info = db_schema()->_db_foreign_key_infos[ index1 ]._child_db_table_info()->_db_column_infos[ index2 ];
									break;
								}
							}
							if( index2 >= db_schema()->_db_foreign_key_infos[ index1 ]._child_db_table_info()->_db_column_infos.Count() )
							{
								error = g_current_module_name;
								error += " module,incorrect format of the JSON string for the database schema";
								break;
							}
							string0.set_ByRef( db_schema()->_db_foreign_key_infos[ index1 ]._parent_db_column_name.ToLower() );
							for( index2 = 0 ; index2 < db_schema()->_db_foreign_key_infos[ index1 ]._parent_db_table_info()->_db_column_infos.Count() ; index2++ )
							{
								lower_db_table_name0.set_ByRef( db_schema()->_db_foreign_key_infos[ index1 ]._parent_db_table_info()->_db_column_infos[ index2 ]._column_name.ToLower() );
								if( lower_db_table_name0 == string0 )
								{
									db_schema()->_db_foreign_key_infos[ index1 ]._parent_db_column_info = db_schema()->_db_foreign_key_infos[ index1 ]._parent_db_table_info()->_db_column_infos[ index2 ];
									break;
								}
							}
							if( index2 >= db_schema()->_db_foreign_key_infos[ index1 ]._parent_db_table_info()->_db_column_infos.Count() )
							{
								error = g_current_module_name;
								error += " module,incorrect format of the JSON string for the database schema";
								break;
							}
						}
					}
				}	
			}
			return db_schema++;
		};
	};
};